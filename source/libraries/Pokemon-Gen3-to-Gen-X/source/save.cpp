// Originally created by Lorenzooone and modified with their permission
// Source: https://github.com/Lorenzooone/Pokemon-Gen3-to-Gen-X

#include "libraries/Pokemon-Gen3-to-Gen-X/include/base_include.h"
#include "libraries/Pokemon-Gen3-to-Gen-X/include/delays.h"
#include "libraries/Pokemon-Gen3-to-Gen-X/include/save.h"
#include "libraries/Pokemon-Gen3-to-Gen-X/include/useful_qualifiers.h"
#include "libraries/Pokemon-Gen3-to-Gen-X/include/timing_basic.h"

#include <stddef.h>


#define IS_FLASH 1
#define SAVE_POS SRAM

#define BASE_FLASH_CMD *((vu8*)(SAVE_POS+0x5555)) = 0xAA; *((vu8*)(SAVE_POS+0x2AAA)) = 0x55;
#define FLASH_WRITE_CMD BASE_FLASH_CMD *((vu8*)(SAVE_POS+0x5555)) = 0xA0;
#define FLASH_BANK_CMD BASE_FLASH_CMD *((vu8*)(SAVE_POS+0x5555)) = 0xB0;
#define FLASH_ERASE_SECTOR_BASE_CMD BASE_FLASH_CMD *((vu8*)(SAVE_POS+0x5555)) = 0x80; BASE_FLASH_CMD
#define FLASH_TERM_CMD *((vu8*)(SAVE_POS+0x5555)) = 0xF0;
#define FLASH_ENTER_MAN_CMD BASE_FLASH_CMD *((vu8*)(SAVE_POS+0x5555)) = 0x90;
#define FLASH_EXIT_MAN_CMD BASE_FLASH_CMD FLASH_TERM_CMD FLASH_TERM_CMD
#define ID_TIMEOUT_CYCLES CLOCK_CYCLES_PER_MS(28)
#define TIMEOUT_CYCLES CLOCK_CYCLES_PER_MS(10)
#define ERASE_TIMEOUT_CYCLES CLOCK_CYCLES_PER_MS(2000)
#define ERASED_BYTE 0xFF
#define BANK_SIZE 0x10000
#define NUM_BANKS 2

#define MACRONIX_MAN_ID 0xC2
#define SANYO_MAN_ID 0x62
#define DEFAULT_MAN_ID 0
#define ERASE_SECTOR_FINAL_CMD 0x30

#if IS_FLASH
#define BANK_LIMIT BANK_SIZE
#else
#define BANK_LIMIT (NUM_BANKS * BANK_SIZE)
#endif

static uintptr_t bank_check(uintptr_t);
static size_t sanitize_save_size(uintptr_t, size_t);
static u8 read_direct_single_byte_save(uintptr_t);
static void write_direct_single_byte_save(uintptr_t, u8);

u8 current_bank;
u8 is_macronix;

IWRAM_CODE void init_bank() {
    REG_WAITCNT &= NON_SRAM_MASK;
    REG_WAITCNT |= SRAM_READING_VALID_WAITCYCLES;
    current_bank = NUM_BANKS;
    is_macronix = 0;
    #if IS_FLASH
    FLASH_ENTER_MAN_CMD
    delay_cycles(ID_TIMEOUT_CYCLES);
    u8 man_id = *((vu8*)SAVE_POS);
    if((man_id == MACRONIX_MAN_ID) || (man_id == SANYO_MAN_ID) || (man_id == DEFAULT_MAN_ID))
        is_macronix = 1;
    FLASH_EXIT_MAN_CMD
    delay_cycles(ID_TIMEOUT_CYCLES);
    #endif
}

IWRAM_CODE uintptr_t bank_check(uintptr_t address) {
    address %= (NUM_BANKS * BANK_SIZE);
    #if IS_FLASH
    u8 bank = address / BANK_SIZE;
    address %= BANK_SIZE;

    if(bank != current_bank) {
        FLASH_BANK_CMD
        *((vu8*)SAVE_POS) = bank;
        current_bank = bank;
    }
    #endif
    return address;
}

IWRAM_CODE void erase_sector(uintptr_t address) {
    address = bank_check(address);
    address >>= SECTOR_SIZE_BITS;
    address <<= SECTOR_SIZE_BITS;
    u8 failed = 1;
    vu8* save_data = (vu8*)SAVE_POS;
    for(size_t i = 0; failed && (i < 3); i++) {
        #if IS_FLASH
        FLASH_ERASE_SECTOR_BASE_CMD
        save_data[address] = ERASE_SECTOR_FINAL_CMD;
        #else
        for(size_t j = 0; j < SECTOR_SIZE; j++)
            save_data[address+(SECTOR_SIZE-1)-j] = ERASED_BYTE;
        #endif
        
        delay_cycles_until(ERASE_TIMEOUT_CYCLES, &save_data[address], 0xFF, SRAM_ACCESS_CYCLES);

        failed = 0;
        if(read_direct_single_byte_save(address + SECTOR_SIZE - 1) != ERASED_BYTE)
        {
            failed = 1;
        }
       if(is_macronix && failed)
            FLASH_TERM_CMD
    }
}

IWRAM_CODE u8 read_direct_single_byte_save(uintptr_t address) {
    return *(vu8*)(SAVE_POS+address);
}

IWRAM_CODE void write_direct_single_byte_save(uintptr_t address, u8 data) {
    vu8* save_data = (vu8*)SAVE_POS;
    for(int i = 0; (i < 3) && save_data[address] != data; i++) {
        #if IS_FLASH
        FLASH_WRITE_CMD
        #endif
        save_data[address] = data;
        delay_cycles_until(TIMEOUT_CYCLES, &save_data[address], data, SRAM_ACCESS_CYCLES);
        if(is_macronix && (save_data[address] != data))
            FLASH_TERM_CMD
    }
}

IWRAM_CODE u8 read_byte_save(uintptr_t address) {
    address = bank_check(address);
    return read_direct_single_byte_save(address);
}

IWRAM_CODE u16 read_short_save(uintptr_t address) {
    u16 data_out = 0;
    copy_save_to_ram(address, (u8*)&data_out, sizeof(u16));
    return data_out;
}

IWRAM_CODE u32 read_int_save(uintptr_t address) {
    u32 data_out = 0;
    copy_save_to_ram(address, (u8*)&data_out, sizeof(u32));
    return data_out;
}

IWRAM_CODE void write_byte_save(uintptr_t address, u8 data) {
    address = bank_check(address);
    write_direct_single_byte_save(address, data);
}

IWRAM_CODE void write_short_save(uintptr_t address, u16 data) {
    u16 data_in = data;
    copy_ram_to_save((u8*)&data_in, address, sizeof(u16));
}

IWRAM_CODE void write_int_save(uintptr_t address, u32 data) {
    u32 data_in = data;
    copy_ram_to_save((u8*)&data_in, address, sizeof(u32));
}

IWRAM_CODE size_t sanitize_save_size(uintptr_t address, size_t size) {
    address %= NUM_BANKS * BANK_SIZE;
    if((address + size) > (NUM_BANKS * BANK_SIZE))
        size = (NUM_BANKS * BANK_SIZE) - address;
    return size;
}

IWRAM_CODE void copy_save_to_ram(uintptr_t address, u8* destination, size_t size) {
    // Sanitize size
    size = sanitize_save_size(address, size);

    address = bank_check(address);
    size_t num_banks = (address + size + BANK_SIZE - 1) / BANK_SIZE;
    for(size_t n = 0; n < num_banks; n++) {
        size_t inner_size = size;
        if(n != (num_banks - 1))
            inner_size = BANK_SIZE - address;

        for(size_t i = 0; i < inner_size; i++)
            destination[i] = read_direct_single_byte_save(address + i);

        if(n != (num_banks - 1)) {
            address = bank_check(address + inner_size);
            destination += inner_size;
            size -= inner_size;
        }
    }
}

IWRAM_CODE void copy_ram_to_save(u8* base_address, uintptr_t save_address, size_t size) {
    // Sanitize size
    size = sanitize_save_size(save_address, size);

    save_address = bank_check(save_address);
    size_t num_banks = (save_address + size + BANK_SIZE - 1) / BANK_SIZE;
    for(size_t n = 0; n < num_banks; n++) {
        size_t inner_size = size;
        if(n != (num_banks - 1))
            inner_size = BANK_SIZE - save_address;

        for(size_t i = 0; i < inner_size; i++)
            write_direct_single_byte_save(save_address + i, base_address[i]);

        if(n != (num_banks - 1)) {
            save_address = bank_check(save_address + inner_size);
            base_address += inner_size;
            size -= inner_size;
        }
    }
}

IWRAM_CODE u8 is_save_correct(u8* base_address, uintptr_t save_address, size_t size) {
    // Sanitize size
    size = sanitize_save_size(save_address, size);

    save_address = bank_check(save_address);
    size_t num_banks = (save_address + size + BANK_SIZE - 1) / BANK_SIZE;
    for(size_t n = 0; n < num_banks; n++) {
        size_t inner_size = size;
        if(n != (num_banks - 1))
            inner_size = BANK_SIZE - save_address;

        for(size_t i = 0; i < inner_size; i++)
            if(read_direct_single_byte_save(save_address + i) != base_address[i])
                return 0;

        if(n != (num_banks - 1)) {
            save_address = bank_check(save_address + inner_size);
            base_address += inner_size;
            size -= inner_size;
        }
    }

    return 1;
}
