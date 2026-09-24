#ifndef PAYLOAD_H
#define PAYLOAD_H

#include <cstdint>

#if defined(__arm__)
#include "dbg/debug_mode.h"
#endif

#if PAYLOAD_EXPORT_TEST || !defined(__arm__)
typedef uint8_t u8, byte, uchar, echar;
typedef uint16_t u16, hword, ushort, eshort;
typedef uint32_t u32, word, uint, eint;
typedef unsigned long long u64;
typedef int32_t i32;
#else
extern "C"
{
#include <tonc_types.h>
}
#endif

#define JPN_ID 1
#define ENG_ID 2
#define FRE_ID 3
#define ITA_ID 4
#define GER_ID 5
#define SPA_ID 7
#define KOR_ID 8

#define GREEN_ID 0
#define RED_ID 1
#define BLUE_ID 2
#define YELLOW_ID 3
#define GOLD_ID 4
#define SILVER_ID 5
#define CRYSTAL_ID 6

#define METHOD_NEWLINE 0
#define METHOD_MEW 1
#define METHOD_GEN2 2

#define PAYLOAD_SIZE 672

#define GB_TILE_WIDTH 20

// TODO? : Since we moved payload_builder into data-generator, we no longer need most of the fields of GB_ROM in our PTGB rom.
// It might be better to split it up into a struct that contains everything for data-generator and a separate one that gets
// included into the ptgb rom, which only includes whatever PTGB needs at runtime. (which is probably a fraction of the data)
// However, right now the compressed gb_rom_values_eng file is only 353 bytes and the gb_rom_values_fre is only 351 bytes.
// Therefore we'd save at most about 0,5 KB if we do this optimization. So it might not be worth it right now.
// If we expand the data at some point, however, it might be!

// WARNING: We must be explicit in the type declarations of the struct members here.
// After all: data-generator runs on x86 and the structs will be read by the GBA's ARM processor.
// So we must ensure the sizes of the struct members, as well as padding, remains consistent across platforms.
// So please make sure to use data types which are explicit in their size. (such as uint32_t, uint8_t, etc.)
// Because the regular "int", "bool", "char" etc are platform-dependent in size.
struct GB_ROM
{
public:
    int32_t language;
    int32_t version;
    int32_t generation;
    int32_t method;
    int32_t payload_size;
    int32_t box_data_size;

    u16 print_string_start;       // Start of where text is written in VRAM
    u16 stack_overwrite_location; // The part of the stack that we're overwriting
    u8 short_pkmn_name;           // The name of the Pokemon that moves us through RAM
    u8 pointer_pkmn_name;         // The name of the Pokemon that overwrites the stack
    u16 padding_1;

    u32 clearScreen;             // location of the clearScreen function in the ROM
    u32 CableClub_TextBoxBorder; // location of the CableClub_TextBoxBorder function in the ROM
    u32 placeString;             // location of the placeString function in the ROM
    u32 Serial_ExchangeBytes;    // location of the Serial_ExchangeBytes function in the ROM
    u32 _RemovePokemon;          // location of the _RemovePokemon function in the ROM
    u32 SaveSAVtoSRAM1;          // location of the SaveSAVtoSRAM1 function in the ROM
    u32 SaveSAVtoSRAM2;          // location of the SaveSAVtoSRAM2 function in the ROM
    u32 LoadCurrentBoxData;      // location of the LoadCurrentBoxData function in the ROM
    u32 OpenSRAM;                // location of the OpenSRAM function in the ROM
    u32 SaveBox;                 // location of the SaveBox function in the ROM
    u32 Bankswitch;              // location of the BankswitchCommon function in the ROM
    u32 SoftReset;               // location of the SoftReset function in the ROM
    u32 CloseSRAM;               // location of the OpenSRAM function in the ROM
    u32 garbageDataLocation;     // location of random data starting with 0xFD in the ROM

    u32 wRemoveMonFromBox;     // location of wRemoveMonFromBox in RAM
    u32 wCurrentBoxNum;        // location of wCurrentBoxNum in RAM
    u32 wBoxCount;             // location of wBoxCount in RAM
    u32 wWhichPokemon;         // location of wWhichPokemon in RAM
    u32 wBoxDataStart;         // location of wBoxDataStart in RAM
    u32 wBoxDataEnd;           // location of wBoxDataEnd in RAM
    u32 wSerialEnemyDataBlock; // location of wSerialEnemyDataBlock in RAM
    u32 wEnemyMonSpecies;      // location of wEnemyMonSpecies in RAM

    u32 wSerialEnemyMonsPatchList;                // location of wSerialEnemyMonsPatchList in RAM
    u32 wSerialOtherGameboyRandomNumberListBlock; // location of wSerialOtherGameboyRandomNumberListBlock in RAM
    u32 hSerialConnectionStatus;                  // location of hSerialConnectionStatus in RAM

    u16 transferStringLocation; // location in VRAM to start writing the transfer string to
    u16 textBorderUppLeft;      // location in VRAM to put the upper left corner of the border
    u8 textBorderWidth;         // the width of the text box border
    u8 textBorderHeight;        // the height of the text box border
    u16 padding_2;
};

#endif