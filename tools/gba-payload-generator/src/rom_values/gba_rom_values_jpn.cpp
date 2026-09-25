#include "rom_values/gba_rom_values.h"

const struct ROM_DATA rom_data_values_jpn[] = {
    { // JPN_RUBY_v0
        .is_valid = false,

        .gamecode = RUBY_ID,
        .version = VERS_1_0,
        .language = LANG_JPN,

        .loc_copyMonToPC = 0x803ad14, // Known as "SendMonToPC"
        .loc_getSetPokedexFlag = 0x808d884,
        .loc_readFlashSector = 0x8120a34, // Known as "DoReadFlashWholeSection" in RS
        .loc_loadSaveSection30 = 0,
        .loc_m4aMPlayStop = 0x81af1d8,
        .loc_MPlayStart = 0x81af0f4,
        .loc_CreateFanfareTask = 0x8071f80,
        .loc_voicegroup = 0x8402498,
        .loc_sPicTable_NPC = 0x8346c28,

        // TODO
        .loc_gMonFrontPicTable = 0,
        .loc_gMonPaletteTable = 0,
        .loc_gMonShinyPaletteTable = 0,
        .loc_gMonIconTable = 0,
        .loc_gMonIconPaletteIndices = 0,
        .loc_gMonIconPalettes = 0,

        // The following should be consistant across languages and revisions.
        .loc_gSaveBlock1 = 0x02025734 - 0x2A0,
        .loc_gSaveDataBuffer = 0x02000000, // Known as "gSharedMem" in RS
        .loc_gSpecialVar_0x8000 = 0x0202E8C4 - 0x2AC,
        .loc_gMPlayInfo_BGM = 0x03007380 - 0x120,
        .loc_gMPlayInfo_SE2 = 0x03007400 - 0x120,
        .loc_sFanfareCounter = 0x030006DA - 0x98,
        .loc_gPlttBufferFaded = 0x0202EEC8 - 0x2E0,
        .loc_gSprites = 0x02020004,

        .offset_ramscript = 0x3690, // Ramscript offset as found within the SaveBlock1 struct in global.h
        .offset_flags = 0x1220,     // Flag offset as found within the SaveBlock1 struct in global.h
        .offset_wondercard = 0,
        .offset_script = 0x0810,
        .text_region = TEXT_HOENN,
        .special_DrawWholeMapView = 0x8E,

        // PKHeX's list of flags is useful for making sure the detection is accurate: https://github.com/kwsch/PKHeX/blob/78a557c3cdaa6f48b42cc96df8ccb4d20b897937/PKHeX.Core/Resources/text/other/flags_rs.txt
        .e4_flag = 0x800 + 0x04,   // The flag that is set when you become champion. Often listed as "GAME_CLEAR"
        .mg_flag = 0x800 + 0x4C,   // The flag that is set when you enable Mystery Gift. Known as "EXDATA_ENABLE" in RS
        .unused_flag_start = 0x21, // The start of the unused flags and must have 31 open flags in a row

        .map_bank = 14,
        .map_id = 11,
        .npc_id = 1,
        .npc_palette = 5,

        .def_map_bank = 8,
        .def_map_id = 1,
        .def_npc_id = 1,

        .old_map_bank = 20,
        .old_map_id = 2,
        .old_npc_id = 1,
        .loc_gSaveBlock1PTR = 0 // TODO: Only used for old script, can be removed later
    },
    { // JPN_SAPPHIRE_v0
        .is_valid = false,

        .gamecode = SAPPHIRE_ID,
        .version = VERS_1_0,
        .language = LANG_JPN,

        .loc_copyMonToPC = 0x803ad14, // Known as "SendMonToPC"
        .loc_getSetPokedexFlag = 0x808d884,
        .loc_readFlashSector = 0x8120a34, // Known as "DoReadFlashWholeSection" in RS
        .loc_loadSaveSection30 = 0,
        .loc_m4aMPlayStop = 0x81af168,
        .loc_MPlayStart = 0x81af084,
        .loc_CreateFanfareTask = 0x8071f84,
        .loc_voicegroup = 0x8402480,
        .loc_sPicTable_NPC = 0x8346bb8,

        // TODO
        .loc_gMonFrontPicTable = 0,
        .loc_gMonPaletteTable = 0,
        .loc_gMonShinyPaletteTable = 0,
        .loc_gMonIconTable = 0,
        .loc_gMonIconPaletteIndices = 0,
        .loc_gMonIconPalettes = 0,

        // The following should be consistant across languages and revisions.
        .loc_gSaveBlock1 = 0x02025734 - 0x2A0,
        .loc_gSaveDataBuffer = 0x02000000, // Known as "gSharedMem" in RS
        .loc_gSpecialVar_0x8000 = 0x0202E8C4 - 0x2AC,
        .loc_gMPlayInfo_BGM = 0x03007380 - 0x120,
        .loc_gMPlayInfo_SE2 = 0x03007400 - 0x120,
        .loc_sFanfareCounter = 0x030006DA - 0x98,
        .loc_gPlttBufferFaded = 0x0202EEC8 - 0x2E0,
        .loc_gSprites = 0x02020004,

        .offset_ramscript = 0x3690, // Ramscript offset as found within the SaveBlock1 struct in global.h
        .offset_flags = 0x1220,     // Flag offset as found within the SaveBlock1 struct in global.h
        .offset_wondercard = 0,
        .offset_script = 0x0810,
        .text_region = TEXT_HOENN,
        .special_DrawWholeMapView = 0x8E,

        // PKHeX's list of flags is useful for making sure the detection is accurate: https://github.com/kwsch/PKHeX/blob/78a557c3cdaa6f48b42cc96df8ccb4d20b897937/PKHeX.Core/Resources/text/other/flags_rs.txt
        .e4_flag = 0x800 + 0x04,   // The flag that is set when you become champion. Often listed as "GAME_CLEAR"
        .mg_flag = 0x800 + 0x4C,   // The flag that is set when you enable Mystery Gift. Known as "EXDATA_ENABLE" in RS
        .unused_flag_start = 0x21, // The start of the unused flags and must have 31 open flags in a row

        .map_bank = 14,
        .map_id = 11,
        .npc_id = 1,
        .npc_palette = 5,

        .def_map_bank = 8,
        .def_map_id = 1,
        .def_npc_id = 1,

        .old_map_bank = 20,
        .old_map_id = 2,
        .old_npc_id = 1,
        .loc_gSaveBlock1PTR = 0 // TODO: Only used for old script, can be removed later
    },
    { // JPN_RUBY_v1
        .is_valid = false,

        .gamecode = RUBY_ID,
        .version = VERS_1_1,
        .language = LANG_JPN,

        .loc_copyMonToPC = 0x803ad14, // Known as "SendMonToPC"
        .loc_getSetPokedexFlag = 0x808d884,
        .loc_readFlashSector = 0x8120a34, // Known as "DoReadFlashWholeSection" in RS
        .loc_loadSaveSection30 = 0,
        .loc_m4aMPlayStop = 0x81af1d8,
        .loc_MPlayStart = 0x81af0f4,
        .loc_CreateFanfareTask = 0x8071f80,
        .loc_voicegroup = 0x8402498,
        .loc_sPicTable_NPC = 0x8346c28,

        // TODO
        .loc_gMonFrontPicTable = 0,
        .loc_gMonPaletteTable = 0,
        .loc_gMonShinyPaletteTable = 0,
        .loc_gMonIconTable = 0,
        .loc_gMonIconPaletteIndices = 0,
        .loc_gMonIconPalettes = 0,

        // The following should be consistant across languages and revisions.
        .loc_gSaveBlock1 = 0x02025734 - 0x2A0,
        .loc_gSaveDataBuffer = 0x02000000, // Known as "gSharedMem" in RS
        .loc_gSpecialVar_0x8000 = 0x0202E8C4 - 0x2AC,
        .loc_gMPlayInfo_BGM = 0x03007380 - 0x120,
        .loc_gMPlayInfo_SE2 = 0x03007400 - 0x120,
        .loc_sFanfareCounter = 0x030006DA - 0x98,
        .loc_gPlttBufferFaded = 0x0202EEC8 - 0x2E0,
        .loc_gSprites = 0x02020004,

        .offset_ramscript = 0x3690, // Ramscript offset as found within the SaveBlock1 struct in global.h
        .offset_flags = 0x1220,     // Flag offset as found within the SaveBlock1 struct in global.h
        .offset_wondercard = 0,
        .offset_script = 0x0810,
        .text_region = TEXT_HOENN,
        .special_DrawWholeMapView = 0x8E,

        // PKHeX's list of flags is useful for making sure the detection is accurate: https://github.com/kwsch/PKHeX/blob/78a557c3cdaa6f48b42cc96df8ccb4d20b897937/PKHeX.Core/Resources/text/other/flags_rs.txt
        .e4_flag = 0x800 + 0x04,   // The flag that is set when you become champion. Often listed as "GAME_CLEAR"
        .mg_flag = 0x800 + 0x4C,   // The flag that is set when you enable Mystery Gift. Known as "EXDATA_ENABLE" in RS
        .unused_flag_start = 0x21, // The start of the unused flags and must have 31 open flags in a row

        .map_bank = 14,
        .map_id = 11,
        .npc_id = 1,
        .npc_palette = 5,

        .def_map_bank = 8,
        .def_map_id = 1,
        .def_npc_id = 1,

        .old_map_bank = 20,
        .old_map_id = 2,
        .old_npc_id = 1,
        .loc_gSaveBlock1PTR = 0 // TODO: Only used for old script, can be removed later
    },
    { // JPN_SAPPHIRE_v1
        .is_valid = false,

        .gamecode = SAPPHIRE_ID,
        .version = VERS_1_1,
        .language = LANG_JPN,

        .loc_copyMonToPC = 0x803ad14, // Known as "SendMonToPC"
        .loc_getSetPokedexFlag = 0x808d884,
        .loc_readFlashSector = 0x8120a34, // Known as "DoReadFlashWholeSection" in RS
        .loc_loadSaveSection30 = 0,
        .loc_m4aMPlayStop = 0x81af168,
        .loc_MPlayStart = 0x81af084,
        .loc_CreateFanfareTask = 0x8071f84,
        .loc_voicegroup = 0x8402480,
        .loc_sPicTable_NPC = 0x8346bb8,

        // TODO
        .loc_gMonFrontPicTable = 0,
        .loc_gMonPaletteTable = 0,
        .loc_gMonShinyPaletteTable = 0,
        .loc_gMonIconTable = 0,
        .loc_gMonIconPaletteIndices = 0,
        .loc_gMonIconPalettes = 0,

        // The following should be consistant across languages and revisions.
        .loc_gSaveBlock1 = 0x02025734 - 0x2A0,
        .loc_gSaveDataBuffer = 0x02000000, // Known as "gSharedMem" in RS
        .loc_gSpecialVar_0x8000 = 0x0202E8C4 - 0x2AC,
        .loc_gMPlayInfo_BGM = 0x03007380 - 0x120,
        .loc_gMPlayInfo_SE2 = 0x03007400 - 0x120,
        .loc_sFanfareCounter = 0x030006DA - 0x98,
        .loc_gPlttBufferFaded = 0x0202EEC8 - 0x2E0,
        .loc_gSprites = 0x02020004,

        .offset_ramscript = 0x3690, // Ramscript offset as found within the SaveBlock1 struct in global.h
        .offset_flags = 0x1220,     // Flag offset as found within the SaveBlock1 struct in global.h
        .offset_wondercard = 0,
        .offset_script = 0x0810,
        .text_region = TEXT_HOENN,
        .special_DrawWholeMapView = 0x8E,

        // PKHeX's list of flags is useful for making sure the detection is accurate: https://github.com/kwsch/PKHeX/blob/78a557c3cdaa6f48b42cc96df8ccb4d20b897937/PKHeX.Core/Resources/text/other/flags_rs.txt
        .e4_flag = 0x800 + 0x04,   // The flag that is set when you become champion. Often listed as "GAME_CLEAR"
        .mg_flag = 0x800 + 0x4C,   // The flag that is set when you enable Mystery Gift. Known as "EXDATA_ENABLE" in RS
        .unused_flag_start = 0x21, // The start of the unused flags and must have 31 open flags in a row

        .map_bank = 14,
        .map_id = 11,
        .npc_id = 1,
        .npc_palette = 5,

        .def_map_bank = 8,
        .def_map_id = 1,
        .def_npc_id = 1,

        .old_map_bank = 20,
        .old_map_id = 2,
        .old_npc_id = 1,
        .loc_gSaveBlock1PTR = 0 // TODO: Only used for old script, can be removed later
    },
    { // JPN_FIRERED_v0
        .is_valid = false,

        .gamecode = FIRERED_ID,
        .version = VERS_1_0,
        .language = LANG_JPN,

        .loc_copyMonToPC = 0x8040284, // Known as "SendMonToPC"
        .loc_getSetPokedexFlag = 0x8088a50,
        .loc_readFlashSector = 0x810586c,
        .loc_loadSaveSection30 = 0x8160a4c, // The location of the function that loads save section 30: CEReaderTool_LoadTrainerTower_r in FRLG, TryReadTrainerHill_Internal in Emerald
        .loc_m4aMPlayStop = 0x81c18f8,
        .loc_MPlayStart = 0x81c1814,
        .loc_CreateFanfareTask = 0x8071490,
        .loc_voicegroup = 0x845d4b8,
        .loc_sPicTable_NPC = 0x8364b40,

        // TODO
        .loc_gMonFrontPicTable = 0,
        .loc_gMonPaletteTable = 0,
        .loc_gMonShinyPaletteTable = 0,
        .loc_gMonIconTable = 0,
        .loc_gMonIconPaletteIndices = 0,
        .loc_gMonIconPalettes = 0,

        // The following should be consistant across languages and revisions.
        .loc_gSaveBlock1 = 0,
        .loc_gSaveDataBuffer = 0x02039A38 - 0x88, // The location of the saveDataBuffer
        .loc_gSpecialVar_0x8000 = 0x020370B8 - 0xCC,
        .loc_gMPlayInfo_BGM = 0x03007300 + 0x50,
        .loc_gMPlayInfo_SE2 = 0x03007380 + 0x50,
        .loc_sFanfareCounter = 0x03000FC6,
        .loc_gPlttBufferFaded = 0x020375F8 - 0xCC,
        .loc_gSprites = 0x0202063C - 0x84,

        .offset_ramscript = 0x361C, // Ramscript offset as found within the SaveBlock1 struct in global.h
        .offset_flags = 0x0EE0,     // Flag offset as found within the SaveBlock1 struct in global.h
        .offset_wondercard = 0x0460,
        .offset_script = 0x079C,
        .text_region = TEXT_KANTO,
        .special_DrawWholeMapView = 0x8E,

        // PKHeX's list of flags is useful for making sure the detection is accurate: https://github.com/kwsch/PKHeX/blob/78a557c3cdaa6f48b42cc96df8ccb4d20b897937/PKHeX.Core/Resources/text/other/flags_rs.txt
        .e4_flag = 0x800 + 0x2C,   // The flag that is set when you become champion. Often listed as "GAME_CLEAR"
        .mg_flag = 0x800 + 0x39,   // The flag that is set when you enable Mystery Gift. Known as "EXDATA_ENABLE" in RS
        .unused_flag_start = 0xAF, // The start of the unused flags and must have 31 open flags in a row

        .map_bank = 31,
        .map_id = 0,
        .npc_id = 1,
        .npc_palette = 3,

        .def_map_bank = 0xFF,
        .def_map_id = 0xFF,
        .def_npc_id = 0xFF,

        .old_map_bank = 30,
        .old_map_id = 0,
        .old_npc_id = 1,

        .loc_gSaveBlock1PTR = 0x03005008, // TODO: Only used for old script, can be removed later
    },
    { // JPN_LEAFGREEN_v0
        .is_valid = false,

        .gamecode = LEAFGREEN_ID,
        .version = VERS_1_0,
        .language = LANG_JPN,

        .loc_copyMonToPC = 0x8040284,
        .loc_getSetPokedexFlag = 0x8088a24,
        .loc_readFlashSector = 0x8105844,
        .loc_loadSaveSection30 = 0x8160a28, // The location of the function that loads save section 30: CEReaderTool_LoadTrainerTower_r in FRLG, TryReadTrainerHill_Internal in Emerald
        .loc_m4aMPlayStop = 0x81c18d4,
        .loc_MPlayStart = 0x81c17f0,
        .loc_CreateFanfareTask = 0x8071490,
        .loc_voicegroup = 0x845d2fc,
        .loc_sPicTable_NPC = 0x8364b20,

        // TODO
        .loc_gMonFrontPicTable = 0,
        .loc_gMonPaletteTable = 0,
        .loc_gMonShinyPaletteTable = 0,
        .loc_gMonIconTable = 0,
        .loc_gMonIconPaletteIndices = 0,
        .loc_gMonIconPalettes = 0,

        // The following should be consistant across languages and revisions.
        .loc_gSaveBlock1 = 0,
        .loc_gSaveDataBuffer = 0x02039A38 - 0x88, // The location of the saveDataBuffer
        .loc_gSpecialVar_0x8000 = 0x020370B8 - 0xCC,
        .loc_gMPlayInfo_BGM = 0x03007300 + 0x50,
        .loc_gMPlayInfo_SE2 = 0x03007380 + 0x50,
        .loc_sFanfareCounter = 0x03000FC6,
        .loc_gPlttBufferFaded = 0x020375F8 - 0xCC,
        .loc_gSprites = 0x0202063C - 0x84,

        .offset_ramscript = 0x361C, // Ramscript offset as found within the SaveBlock1 struct in global.h
        .offset_flags = 0x0EE0,     // Flag offset as found within the SaveBlock1 struct in global.h
        .offset_wondercard = 0x0460,
        .offset_script = 0x079C,
        .text_region = TEXT_KANTO,
        .special_DrawWholeMapView = 0x8E,

        // PKHeX's list of flags is useful for making sure the detection is accurate: https://github.com/kwsch/PKHeX/blob/78a557c3cdaa6f48b42cc96df8ccb4d20b897937/PKHeX.Core/Resources/text/other/flags_rs.txt
        .e4_flag = 0x800 + 0x2C,   // The flag that is set when you become champion. Often listed as "GAME_CLEAR"
        .mg_flag = 0x800 + 0x39,   // The flag that is set when you enable Mystery Gift. Known as "EXDATA_ENABLE" in RS
        .unused_flag_start = 0xAF, // The start of the unused flags and must have 31 open flags in a row

        .map_bank = 31,
        .map_id = 0,
        .npc_id = 1,
        .npc_palette = 3,

        .def_map_bank = 0xFF,
        .def_map_id = 0xFF,
        .def_npc_id = 0xFF,

        .old_map_bank = 30,
        .old_map_id = 0,
        .old_npc_id = 1,

        .loc_gSaveBlock1PTR = 0x03005008, // TODO: Only used for old script, can be removed later
    },
    { // JPN_FIRERED_v1
        .is_valid = false,

        .gamecode = FIRERED_ID,
        .version = VERS_1_1,
        .language = LANG_JPN,

        .loc_copyMonToPC = 0x80401f0,
        .loc_getSetPokedexFlag = 0x80889e0,
        .loc_readFlashSector = 0x8105870,
        .loc_loadSaveSection30 = 0x81605c8, // The location of the function that loads save section 30: CEReaderTool_LoadTrainerTower_r in FRLG, TryReadTrainerHill_Internal in Emerald
        .loc_m4aMPlayStop = 0x81c14b4,
        .loc_MPlayStart = 0x81c13d0,
        .loc_CreateFanfareTask = 0x8071408,
        .loc_voicegroup = 0x8458cb4,
        .loc_sPicTable_NPC = 0x8360320,

        // TODO
        .loc_gMonFrontPicTable = 0,
        .loc_gMonPaletteTable = 0,
        .loc_gMonShinyPaletteTable = 0,
        .loc_gMonIconTable = 0,
        .loc_gMonIconPaletteIndices = 0,
        .loc_gMonIconPalettes = 0,

        // The following should be consistant across languages and revisions.
        .loc_gSaveBlock1 = 0,
        .loc_gSaveDataBuffer = 0x02039A38 - 0x88, // The location of the saveDataBuffer
        .loc_gSpecialVar_0x8000 = 0x020370B8 - 0xCC,
        .loc_gMPlayInfo_BGM = 0x03007300 - 0x50,
        .loc_gMPlayInfo_SE2 = 0x03007380 - 0x50,
        .loc_sFanfareCounter = 0x03000FC6,
        .loc_gPlttBufferFaded = 0x020375F8 - 0xCC,
        .loc_gSprites = 0x0202063C - 0x84,

        .offset_ramscript = 0x361C, // Ramscript offset as found within the SaveBlock1 struct in global.h
        .offset_flags = 0x0EE0,     // Flag offset as found within the SaveBlock1 struct in global.h
        .offset_wondercard = 0x0460,
        .offset_script = 0x079C,
        .text_region = TEXT_KANTO,
        .special_DrawWholeMapView = 0x8E,

        // PKHeX's list of flags is useful for making sure the detection is accurate: https://github.com/kwsch/PKHeX/blob/78a557c3cdaa6f48b42cc96df8ccb4d20b897937/PKHeX.Core/Resources/text/other/flags_rs.txt
        .e4_flag = 0x800 + 0x2C,   // The flag that is set when you become champion. Often listed as "GAME_CLEAR"
        .mg_flag = 0x800 + 0x39,   // The flag that is set when you enable Mystery Gift. Known as "EXDATA_ENABLE" in RS
        .unused_flag_start = 0xAF, // The start of the unused flags and must have 31 open flags in a row

        .map_bank = 31,
        .map_id = 0,
        .npc_id = 1,
        .npc_palette = 3,

        .def_map_bank = 0xFF,
        .def_map_id = 0xFF,
        .def_npc_id = 0xFF,

        .old_map_bank = 30,
        .old_map_id = 0,
        .old_npc_id = 1,

        .loc_gSaveBlock1PTR = 0x03005008, // TODO: Only used for old script, can be removed later
    },
    { // JPN_LEAFGREEN_v1
        .is_valid = false,

        .gamecode = LEAFGREEN_ID,
        .version = VERS_1_1,
        .language = LANG_JPN,

        .loc_copyMonToPC = 0x80401f0,
        .loc_getSetPokedexFlag = 0x80889b4,
        .loc_readFlashSector = 0x8105848,
        .loc_loadSaveSection30 = 0x81605a4, // The location of the function that loads save section 30: CEReaderTool_LoadTrainerTower_r in FRLG, TryReadTrainerHill_Internal in Emerald
        .loc_m4aMPlayStop = 0x81c1490,
        .loc_MPlayStart = 0x81c13ac,
        .loc_CreateFanfareTask = 0x8071408,
        .loc_voicegroup = 0x8458af8,
        .loc_sPicTable_NPC = 0x8360300,

        // TODO
        .loc_gMonFrontPicTable = 0,
        .loc_gMonPaletteTable = 0,
        .loc_gMonShinyPaletteTable = 0,
        .loc_gMonIconTable = 0,
        .loc_gMonIconPaletteIndices = 0,
        .loc_gMonIconPalettes = 0,

        // The following should be consistant across languages and revisions.
        .loc_gSaveBlock1 = 0,
        .loc_gSaveDataBuffer = 0x02039A38 - 0x88, // The location of the saveDataBuffer
        .loc_gSpecialVar_0x8000 = 0x020370B8 - 0xCC,
        .loc_gMPlayInfo_BGM = 0x03007300 - 0x50,
        .loc_gMPlayInfo_SE2 = 0x03007380 - 0x50,
        .loc_sFanfareCounter = 0x03000FC6,
        .loc_gPlttBufferFaded = 0x020375F8 - 0xCC,
        .loc_gSprites = 0x0202063C - 0x84,

        .offset_ramscript = 0x361C, // Ramscript offset as found within the SaveBlock1 struct in global.h
        .offset_flags = 0x0EE0,     // Flag offset as found within the SaveBlock1 struct in global.h
        .offset_wondercard = 0x0460,
        .offset_script = 0x079C,
        .text_region = TEXT_KANTO,
        .special_DrawWholeMapView = 0x8E,

        // PKHeX's list of flags is useful for making sure the detection is accurate: https://github.com/kwsch/PKHeX/blob/78a557c3cdaa6f48b42cc96df8ccb4d20b897937/PKHeX.Core/Resources/text/other/flags_rs.txt
        .e4_flag = 0x800 + 0x2C,   // The flag that is set when you become champion. Often listed as "GAME_CLEAR"
        .mg_flag = 0x800 + 0x39,   // The flag that is set when you enable Mystery Gift. Known as "EXDATA_ENABLE" in RS
        .unused_flag_start = 0xAF, // The start of the unused flags and must have 31 open flags in a row

        .map_bank = 31,
        .map_id = 0,
        .npc_id = 1,
        .npc_palette = 3,

        .def_map_bank = 0xFF,
        .def_map_id = 0xFF,
        .def_npc_id = 0xFF,

        .old_map_bank = 30,
        .old_map_id = 0,
        .old_npc_id = 1,

        .loc_gSaveBlock1PTR = 0x03005008, // TODO: Only used for old script, can be removed later
    },
    { // JPN_EMERALD_v0
        .is_valid = false,

        .gamecode = EMERALD_ID,
        .version = VERS_1_0,
        .language = LANG_JPN,

        .loc_copyMonToPC = 0x806af58,
        .loc_getSetPokedexFlag = 0x80bfbbc,
        .loc_readFlashSector = 0x815309c,
        .loc_loadSaveSection30 = 0x81d3158, // The location of the function that loads save section 30: CEReaderTool_LoadTrainerTower_r in FRLG, TryReadTrainerHill_Internal in Emerald
        .loc_m4aMPlayStop = 0x828ef00,
        .loc_MPlayStart = 0x828ee1c,
        .loc_CreateFanfareTask = 0x80a2a38,
        .loc_voicegroup = 0x860e594,
        .loc_sPicTable_NPC = 0x84de858,

        // TODO
        .loc_gMonFrontPicTable = 0,
        .loc_gMonPaletteTable = 0,
        .loc_gMonShinyPaletteTable = 0,
        .loc_gMonIconTable = 0,
        .loc_gMonIconPaletteIndices = 0,
        .loc_gMonIconPalettes = 0,

        // The following should be consistant across languages and revisions.
        .loc_gSaveBlock1 = 0,
        .loc_gSaveDataBuffer = 0x0203ABBC - 0x334, // The location of the saveDataBuffer
        .loc_gSpecialVar_0x8000 = 0x020375D8 - 0x360,
        .loc_gMPlayInfo_BGM = 0x03007420 + 0xB0,
        .loc_gMPlayInfo_SE2 = 0x03007630 - 0xE0,
        .loc_sFanfareCounter = 0x03000F4E,
        .loc_gPlttBufferFaded = 0x02037B14 - 0x360,
        .loc_gSprites = 0x02020630 - 0x84,

        .offset_ramscript = 0x3728, // Ramscript offset as found within the SaveBlock1 struct in global.h
        .offset_flags = 0x1270,     // Flag offset as found within the SaveBlock1 struct in global.h
        .offset_wondercard = 0x056C,
        .offset_script = 0x08A8,
        .text_region = TEXT_HOENN,
        .special_DrawWholeMapView = 0x91,

        // PKHeX's list of flags is useful for making sure the detection is accurate: https://github.com/kwsch/PKHeX/blob/78a557c3cdaa6f48b42cc96df8ccb4d20b897937/PKHeX.Core/Resources/text/other/flags_rs.txt
        .e4_flag = 0x860 + 0x04,   // The flag that is set when you become champion. Often listed as "GAME_CLEAR"
        .mg_flag = 0x860 + 0x7B,   // The flag that is set when you enable Mystery Gift. Known as "EXDATA_ENABLE" in RS
        .unused_flag_start = 0x20, // The start of the unused flags and must have 31 open flags in a row

        .map_bank = 15,
        .map_id = 13,
        .npc_id = 1,
        .npc_palette = 5,

        .def_map_bank = 0xFF,
        .def_map_id = 0xFF,
        .def_npc_id = 0xFF,

        .old_map_bank = 20,
        .old_map_id = 2,
        .old_npc_id = 1,

        .loc_gSaveBlock1PTR = 0x03005D8C, // TODO: Only used for old script, can be removed later
    }
};

const u16 rom_data_values_jpn_size = static_cast<u16>(sizeof(rom_data_values_jpn) / sizeof(struct ROM_DATA));