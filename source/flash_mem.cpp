#include <tonc.h>
#include "libstd_replacements.h"
#include "flash_mem.h"
#include "pokemon_data.h"
#include "rom_data.h"
#include "libraries/Pokemon-Gen3-to-Gen-X/include/save.h"
#include "text_engine.h"

#define READ_SAVE_SECTIONS 14
#define TOTAL_SAVE_SECTIONS 14

vu32 newest_save_offset = SAVE_A_OFFSET;

vu32 memory_section_array[READ_SAVE_SECTIONS];
u8 global_memory_buffer[0x1000];
u8 mem_id;

// Fills the variables with the current offset information
void initialize_memory_locations()
{
    u8 save_A_index[4];
    u8 save_B_index[4];
    copy_save_to_ram(SAVE_A_OFFSET + SAVE_INDEX_OFFSET, &save_A_index[0], 0x04);
    copy_save_to_ram(SAVE_B_OFFSET + SAVE_INDEX_OFFSET, &save_B_index[0], 0x04);

    // Save indices are little-endian in-sector; decode directly without byte swapping.
    const u32 saveAIndex = static_cast<u32>(save_A_index[0]) |
                           (static_cast<u32>(save_A_index[1]) << 8) |
                           (static_cast<u32>(save_A_index[2]) << 16) |
                           (static_cast<u32>(save_A_index[3]) << 24);
    const u32 saveBIndex = static_cast<u32>(save_B_index[0]) |
                           (static_cast<u32>(save_B_index[1]) << 8) |
                           (static_cast<u32>(save_B_index[2]) << 16) |
                           (static_cast<u32>(save_B_index[3]) << 24);

    // Determines if save A or B is more recent
    if (saveBIndex > saveAIndex)
    {
        newest_save_offset = SAVE_B_OFFSET;
    }

    // Populates the memory_section_array with the correct pointer locations
    // The sections within the save slot are rotated on every save. So it doesn't 
    // start at the first section. However, the next sections follow sequentially.
    // https://bulbapedia.bulbagarden.net/wiki/Save_data_structure_(Generation_III)#Section_ID
    copy_save_to_ram(newest_save_offset + SECTION_ID_OFFSET, &mem_id, 1);
    for (int i = 0; i < TOTAL_SAVE_SECTIONS; i++)
    {
        if (mem_id < READ_SAVE_SECTIONS)
        {
            memory_section_array[mem_id] = newest_save_offset + (i * 0x1000);
        }
        mem_id = (mem_id + 1) % TOTAL_SAVE_SECTIONS;
    }
    // Bring the Memory ID back to the first one
    mem_id = (mem_id + 1) % TOTAL_SAVE_SECTIONS;

    if (false) // This will print out a section of the FLASH mem for debugging purposes
    {
        int mem_start = 0xF80;
        int mem_section = 1;
        copy_save_to_ram(memory_section_array[mem_section], &global_memory_buffer[0], 0x1000);
        tte_set_pos(8, 0);
        tte_write("loc: ");
        tte_write(ptgb::to_string(static_cast<unsigned>(memory_section_array[mem_section] + mem_start)));
        tte_write("\n");
        for (int i = mem_start; i < (128 + mem_start); i++)
        {
            if (i % 2 == 0)
            {
                tte_write("#{cx:0xE000}");
            }
            else
            {
                tte_write("#{cx:0xD000}");
            }
            tte_write(ptgb::to_string(global_memory_buffer[i]));
            if (i % 8 == 7)
            {
                tte_write("\n");
            }
            else
            {
                if (global_memory_buffer[i] < 10)
                {
                    tte_write("  ");
                }
                else if (global_memory_buffer[i] < 100)
                {
                    tte_write(" ");
                }
                else
                {
                    tte_write("");
                }
            }
        }
        while (true)
        {
        };
    }
}

void print_mem_section()
{
    return; // This function isn't really needed now
    /*
    uint16_t charset[256];
    byte out[4] = {0, 0, 0, 0xFF};
    const char mem_name = (newest_save_offset == SAVE_A_OFFSET) ? 'A' : 'B';

    load_localized_charset(charset, 3, ENGLISH);

    out[0] = get_char_from_charset(charset, mem_name);
    out[1] = get_char_from_charset(charset, '-');
    out[2] = get_char_from_charset(charset, mem_id + 0xA1); // Kinda a dumb way to 
    //tte_set_pos(0, 0);
    ptgb_write_simple(out, true);
    */
}

// Reverses the endian of the given array
void reverse_endian(u8 *data, size_t size)
{
    u8 temp;
    for (unsigned int i = 0; i < (size / 2); i++)
    {
        temp = data[i];
        data[i] = data[(size - 1) - i];
        data[(size - 1) - i] = temp;
    }
}

void update_memory_buffer_checksum(u8 *sector_buffer, bool hall_of_fame)
{
    u32 checksum = 0x00;

    // Section 13 is the last PC buffer (I) and that one only has 2000 bytes of data.
    // source: https://bulbapedia.bulbagarden.net/wiki/Save_data_structure_(Generation_III)#Section_ID
    const u32 num_of_bytes = (sector_buffer[SECTION_ID_OFFSET] != 13) ? 3968 : 2000;

    // Assemble words manually to avoid alignment and aliasing issues when the
    // sector buffer is only guaranteed to be byte-aligned. So not 32-bit aligned.
    for (u32 offset = 0; offset < num_of_bytes; offset += sizeof(u32))
    {
        const u32 cur = static_cast<u32>(sector_buffer[offset]) |
                        (static_cast<u32>(sector_buffer[offset + 1]) << 8) |
                        (static_cast<u32>(sector_buffer[offset + 2]) << 16) |
                        (static_cast<u32>(sector_buffer[offset + 3]) << 24);
        checksum += cur;
    }

    const u16 small_checksum = ((checksum & 0xFFFF0000) >> 16) + (checksum & 0x0000FFFF);
    const u32 checksum_offset = hall_of_fame ? 0x0FF4 : 0x0FF6;
    
    sector_buffer[checksum_offset] = small_checksum & 0x00FF;
    sector_buffer[checksum_offset + 1] = (small_checksum & 0xFF00) >> 8;

}

bool read_flag(u16 flag_id)
{
    if (false)
    {
        //tte_set_pos(0, 0);
        tte_write("#{cx:0xD000}Attempting to read byte ");
        tte_write(ptgb::to_string((curr_GBA_rom.offset_flags + (flag_id / 8)) % 0xF80));
        tte_write(" of memory section ");
        tte_write(ptgb::to_string(1 + ((curr_GBA_rom.offset_flags + (flag_id / 8)) / 0xF80)));
        tte_write(" for flag ");
        tte_write(ptgb::to_string(flag_id));
        tte_write(". Flag is ");
        copy_save_to_ram(memory_section_array[1 + ((curr_GBA_rom.offset_flags + (flag_id / 8)) / 0xF80)], &global_memory_buffer[0], 0x1000);
        u8 flags = global_memory_buffer[(curr_GBA_rom.offset_flags + (flag_id / 8)) % 0xF80];
        tte_write(ptgb::to_string((flags >> (flag_id % 8)) & 0b1));
        while (true)
        {
        };
    }

    copy_save_to_ram(memory_section_array[1 + ((curr_GBA_rom.offset_flags + (flag_id / 8)) / 0xF80)], &global_memory_buffer[0], 0x1000);
    u8 flags = global_memory_buffer[(curr_GBA_rom.offset_flags + (flag_id / 8)) % 0xF80];
    return (flags >> (flag_id % 8)) & 0b1;
}

bool compare_map_and_npc_data(int map_bank, int map_id, int npc_id)
{
    copy_save_to_ram(memory_section_array[4], &global_memory_buffer[0], 0x1000);
    return (global_memory_buffer[curr_GBA_rom.offset_script + 4] == MGSCRIPT_MAGIC_VALUE &&
            global_memory_buffer[curr_GBA_rom.offset_script + 5] == map_bank &&
            global_memory_buffer[curr_GBA_rom.offset_script + 6] == map_id &&
            global_memory_buffer[curr_GBA_rom.offset_script + 7] == npc_id);
}