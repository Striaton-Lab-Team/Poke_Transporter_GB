#ifndef BASE_GBA_ROM_STRUCT_H
#define BASE_GBA_ROM_STRUCT_H

#if defined(__arm__)
#include <tonc_types.h>
#else
#include <cstdint>
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
#endif

#define RUBY_ID 0x415856      // AXV
#define SAPPHIRE_ID 0x415850  // AXP
#define FIRERED_ID 0x425052   // BPR
#define LEAFGREEN_ID 0x425047 // BPG
#define EMERALD_ID 0x425045   // BPE

#define VERS_1_0 0
#define VERS_1_1 1
#define VERS_1_2 2

#define LANG_JPN 'J'
#define LANG_ENG 'E'
#define LANG_FRE 'F'
#define LANG_GER 'D'
#define LANG_ITA 'I'
#define LANG_SPA 'S'

#define TEXT_KANTO 0
#define TEXT_HOENN 1

// WARNING: We must be explicit in the type declarations of the struct members here.
// After all: data-generator runs on x86 and the structs will be read by the GBA's ARM processor.
// So we must ensure the sizes of the struct members, as well as padding and alignment, remains consistent across platforms.
// So please make sure to use data types which are explicit in their size. (such as uint32_t, uint8_t, etc.)
// Because the size of the regular "int", "bool", "char" etc changes based on the platform architecture.
struct ROM_DATA
{
    int32_t is_valid;
    int32_t gamecode;
    int32_t version;
    int32_t language;

    int32_t loc_copyMonToPC;
    int32_t loc_getSetPokedexFlag;
    int32_t loc_readFlashSector;
    int32_t loc_loadSaveSection30; // The location of the function that loads save section 30: CEReaderTool_LoadTrainerTower_r in FRLG, TryReadTrainerHill_Internal in Emerald
    int32_t loc_m4aMPlayStop;
    int32_t loc_MPlayStart;
    int32_t loc_CreateFanfareTask;
    int32_t loc_voicegroup;
    int32_t loc_sPicTable_NPC; // sPicTable_OldWoman in FRLG

    int32_t loc_gMonFrontPicTable; // gMonStillFrontPicTable in Emerald
    int32_t loc_gMonPaletteTable;
    int32_t loc_gMonShinyPaletteTable;
    int32_t loc_gMonIconTable;
    int32_t loc_gMonIconPaletteIndices;
    int32_t loc_gMonIconPalettes;

    int32_t loc_gSaveBlock1;     // Only used in R/S
    int32_t loc_gSaveDataBuffer; // The location of the saveDataBuffer
    int32_t loc_gSpecialVar_0x8000;
    int32_t loc_gMPlayInfo_BGM;
    int32_t loc_gMPlayInfo_SE2;
    int32_t loc_sFanfareCounter;
    int32_t loc_gPlttBufferFaded;
    int32_t loc_gSprites;

    int32_t offset_ramscript; // Ramscript offset as found within the SaveBlock1 struct in global.h
    int32_t offset_flags;     // Flag offset as found within the SaveBlock1 struct in global.h
    int32_t offset_wondercard;
    int32_t offset_script;
    int32_t text_region;
    int32_t special_DrawWholeMapView;

    // PKHeX's list of flags is useful for making sure the detection is accurate: https://github.com/kwsch/PKHeX/blob/78a557c3cdaa6f48b42cc96df8ccb4d20b897937/PKHeX.Core/Resources/text/other/flags_rs.txt
    int32_t e4_flag;           // The flag that is set when you become champion. Often listed as "GAME_CLEAR"
    int32_t mg_flag;           // The flag that is set when you enable Mystery Gift. Known as "EXDATA_ENABLE" in RS
    int32_t unused_flag_start; // The start of the unused flags and must have 31 open flags in a row

    u8 map_bank;
    u8 map_id;
    u8 npc_id;
    u8 npc_palette;

    u8 def_map_bank;
    u8 def_map_id;
    u8 def_npc_id;

    u8 old_map_bank;
    u8 old_map_id;
    u8 old_npc_id;

    int32_t loc_gSaveBlock1PTR; // TODO: Only used for old script, can be removed later
};

#endif