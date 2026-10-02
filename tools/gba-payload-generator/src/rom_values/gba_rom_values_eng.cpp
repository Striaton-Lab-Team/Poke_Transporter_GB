#include "rom_values/gba_rom_values.h"

const struct ROM_DATA rom_data_values_eng[] = {
    { // ENG_RUBY_v0
        .is_valid = true,

        .gamecode = RUBY_ID,
        .version = VERS_1_0,
        .language = LANG_ENG,

        .loc_copyMonToPC = 0x0803D998, // Known as "SendMonToPC"
        .loc_getSetPokedexFlag = 0x08090D90,
        .loc_readFlashSector = 0x08125BF8, // Known as "DoReadFlashWholeSection" in RS
        .loc_loadSaveSection30 = 0,
        .loc_m4aMPlayStop = 0x081DE740,
        .loc_MPlayStart = 0x081DE65C,
        .loc_CreateFanfareTask = 0x08074F48,
        .loc_voicegroup = 0x084401E8,
        .loc_sPicTable_NPC = 0x0836E788,

        .loc_gMonFrontPicTable = 0x081E8354,
        .loc_gMonPaletteTable = 0x081EA5B4,
        .loc_gMonShinyPaletteTable = 0x081EB374,
        .loc_gMonIconTable = 0x083BBD20,
        .loc_gMonIconPaletteIndices = 0x083BC400,
        .loc_gMonIconPalettes = 0x08E966D8,

        // The following should be consistant across languages and revisions.
        .loc_gSaveBlock1 = 0x02025734,
        .loc_gSaveDataBuffer = 0x02000000, // Known as "gSharedMem" in RS
        .loc_gSpecialVar_0x8000 = 0x0202E8C4,
        .loc_gMPlayInfo_BGM = 0x03007380,
        .loc_gMPlayInfo_SE2 = 0x03007400,
        .loc_sFanfareCounter = 0x030006DA,
        .loc_gPlttBufferFaded = 0x0202EEC8,
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
    { // ENG_SAPPHIRE_v0
        .is_valid = true,

        .gamecode = SAPPHIRE_ID,
        .version = VERS_1_0,
        .language = LANG_ENG,

        .loc_copyMonToPC = 0x0803D998, // Known as "SendMonToPC"
        .loc_getSetPokedexFlag = 0x08090D90,
        .loc_readFlashSector = 0x08125BF8, // Known as "DoReadFlashWholeSection" in RS
        .loc_loadSaveSection30 = 0,
        .loc_m4aMPlayStop = 0x081DE6D0,
        .loc_MPlayStart = 0x081DE5EC,
        .loc_CreateFanfareTask = 0x08074F4C,
        .loc_voicegroup = 0x08445644,
        .loc_sPicTable_NPC = 0x0836E718,

        .loc_gMonFrontPicTable = 0x081E82E4,
        .loc_gMonPaletteTable = 0x081EA544,
        .loc_gMonShinyPaletteTable = 0x081EB304,
        .loc_gMonIconTable = 0x083BBD78,
        .loc_gMonIconPaletteIndices = 0x083BC458,
        .loc_gMonIconPalettes = 0x08E966D8,

        // The following should be consistant across languages and revisions.
        .loc_gSaveBlock1 = 0x02025734,
        .loc_gSaveDataBuffer = 0x02000000, // Known as "gSharedMem" in RS
        .loc_gSpecialVar_0x8000 = 0x0202E8C4,
        .loc_gMPlayInfo_BGM = 0x03007380,
        .loc_gMPlayInfo_SE2 = 0x03007400,
        .loc_sFanfareCounter = 0x030006DA,
        .loc_gPlttBufferFaded = 0x0202EEC8,
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
    { // ENG_RUBY_v1
        .is_valid = true,

        .gamecode = RUBY_ID,
        .version = VERS_1_1,
        .language = LANG_ENG,

        .loc_copyMonToPC = 0x0803D998, // Known as "SendMonToPC"
        .loc_getSetPokedexFlag = 0x08090DB0,
        .loc_readFlashSector = 0x08125C18, // Known as "DoReadFlashWholeSection" in RS
        .loc_loadSaveSection30 = 0,
        .loc_m4aMPlayStop = 0x081DE758,
        .loc_MPlayStart = 0x081DE674,
        .loc_CreateFanfareTask = 0x08074F68,
        .loc_voicegroup = 0x084401FC,
        .loc_sPicTable_NPC = 0x0836E7A0,

        .loc_gMonFrontPicTable = 0x081E836C,
        .loc_gMonPaletteTable = 0x081EA5CC,
        .loc_gMonShinyPaletteTable = 0x081EB38C,
        .loc_gMonIconTable = 0x083BBD3C,
        .loc_gMonIconPaletteIndices = 0x083BC41C,
        .loc_gMonIconPalettes = 0x08E966D8,

        // The following should be consistant across languages and revisions.
        .loc_gSaveBlock1 = 0x02025734,
        .loc_gSaveDataBuffer = 0x02000000, // Known as "gSharedMem" in RS
        .loc_gSpecialVar_0x8000 = 0x0202E8C4,
        .loc_gMPlayInfo_BGM = 0x03007380,
        .loc_gMPlayInfo_SE2 = 0x03007400,
        .loc_sFanfareCounter = 0x030006DA,
        .loc_gPlttBufferFaded = 0x0202EEC8,
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
    { // ENG_SAPPHIRE_v1
        .is_valid = true,

        .gamecode = SAPPHIRE_ID,
        .version = VERS_1_1,
        .language = LANG_ENG,

        .loc_copyMonToPC = 0x0803D998, // Known as "SendMonToPC"
        .loc_getSetPokedexFlag = 0x08090DB0,
        .loc_readFlashSector = 0x08125C18, // Known as "DoReadFlashWholeSection" in RS
        .loc_loadSaveSection30 = 0,
        .loc_m4aMPlayStop = 0x081DE6E8,
        .loc_MPlayStart = 0x081DE604,
        .loc_CreateFanfareTask = 0x08074F6C,
        .loc_voicegroup = 0x0844025C,
        .loc_sPicTable_NPC = 0x0836E730,

        .loc_gMonFrontPicTable = 0x081E82FC,
        .loc_gMonPaletteTable = 0x081EA55C,
        .loc_gMonShinyPaletteTable = 0x081EB31C,
        .loc_gMonIconTable = 0x083BBD98,
        .loc_gMonIconPaletteIndices = 0x083BC478,
        .loc_gMonIconPalettes = 0x08E966D8,

        // The following should be consistant across languages and revisions.
        .loc_gSaveBlock1 = 0x02025734,
        .loc_gSaveDataBuffer = 0x02000000, // Known as "gSharedMem" in RS
        .loc_gSpecialVar_0x8000 = 0x0202E8C4,
        .loc_gMPlayInfo_BGM = 0x03007380,
        .loc_gMPlayInfo_SE2 = 0x03007400,
        .loc_sFanfareCounter = 0x030006DA,
        .loc_gPlttBufferFaded = 0x0202EEC8,
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
    { // ENG_RUBY_v2
        .is_valid = true,

        .gamecode = RUBY_ID,
        .version = VERS_1_2,
        .language = LANG_ENG,

        .loc_copyMonToPC = 0x0803D998, // Known as "SendMonToPC"
        .loc_getSetPokedexFlag = 0x08090DB0,
        .loc_readFlashSector = 0x08125C18, // Known as "DoReadFlashWholeSection" in RS
        .loc_loadSaveSection30 = 0,
        .loc_m4aMPlayStop = 0x081DE758,
        .loc_MPlayStart = 0x081DE674,
        .loc_CreateFanfareTask = 0x08074F68,
        .loc_voicegroup = 0x084401FC,
        .loc_sPicTable_NPC = 0x0836E7A0,

        .loc_gMonFrontPicTable = 0x081E836C,
        .loc_gMonPaletteTable = 0x081EA5CC,
        .loc_gMonShinyPaletteTable = 0x081EB38C,
        .loc_gMonIconTable = 0x083BBD3C,
        .loc_gMonIconPaletteIndices = 0x083BC41C,
        .loc_gMonIconPalettes = 0x08E966D8,

        // The following should be consistant across languages and revisions.
        .loc_gSaveBlock1 = 0x02025734,
        .loc_gSaveDataBuffer = 0x02000000, // Known as "gSharedMem" in RS
        .loc_gSpecialVar_0x8000 = 0x0202E8C4,
        .loc_gMPlayInfo_BGM = 0x03007380,
        .loc_gMPlayInfo_SE2 = 0x03007400,
        .loc_sFanfareCounter = 0x030006DA,
        .loc_gPlttBufferFaded = 0x0202EEC8,
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
    { // ENG_SAPPHIRE_v2
        .is_valid = true,

        .gamecode = SAPPHIRE_ID,
        .version = VERS_1_2,
        .language = LANG_ENG,

        .loc_copyMonToPC = 0x0803D998, // Known as "SendMonToPC"
        .loc_getSetPokedexFlag = 0x08090DB0,
        .loc_readFlashSector = 0x08125C18, // Known as "DoReadFlashWholeSection" in RS
        .loc_loadSaveSection30 = 0,
        .loc_m4aMPlayStop = 0x081DE6E8,
        .loc_MPlayStart = 0x081DE604,
        .loc_CreateFanfareTask = 0x08074F6C,
        .loc_voicegroup = 0x0844025C,
        .loc_sPicTable_NPC = 0x0836E730,

        .loc_gMonFrontPicTable = 0x081E82FC,
        .loc_gMonPaletteTable = 0x081EA55C,
        .loc_gMonShinyPaletteTable = 0x081EB31C,
        .loc_gMonIconTable = 0x083BBD98,
        .loc_gMonIconPaletteIndices = 0x083BC478,
        .loc_gMonIconPalettes = 0x08E966D8,

        // The following should be consistant across languages and revisions.
        .loc_gSaveBlock1 = 0x02025734,
        .loc_gSaveDataBuffer = 0x02000000, // Known as "gSharedMem" in RS
        .loc_gSpecialVar_0x8000 = 0x0202E8C4,
        .loc_gMPlayInfo_BGM = 0x03007380,
        .loc_gMPlayInfo_SE2 = 0x03007400,
        .loc_sFanfareCounter = 0x030006DA,
        .loc_gPlttBufferFaded = 0x0202EEC8,
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
    { // ENG_FIRERED_v0
        .is_valid = true,

        .gamecode = FIRERED_ID,
        .version = VERS_1_0,
        .language = LANG_ENG,

        .loc_copyMonToPC = 0x08040B90, // Known as "SendMonToPC"
        .loc_getSetPokedexFlag = 0x08088E74,
        .loc_readFlashSector = 0x080DA190,
        .loc_loadSaveSection30 = 0x0815D7BC, // The location of the function that loads save section 30: CEReaderTool_LoadTrainerTower_r in FRLG, TryReadTrainerHill_Internal in Emerald
        .loc_m4aMPlayStop = 0x081DD93C,
        .loc_MPlayStart = 0x081DD858,
        .loc_CreateFanfareTask = 0x08071CEC,
        .loc_voicegroup = 0x08498878,
        .loc_sPicTable_NPC = 0x083A0AB8,

        .loc_gMonFrontPicTable = 0x082350AC,
        .loc_gMonPaletteTable = 0x0823730C,
        .loc_gMonShinyPaletteTable = 0x082380CC,
        .loc_gMonIconTable = 0x083D37A0,
        .loc_gMonIconPaletteIndices = 0x083D3E80,
        .loc_gMonIconPalettes = 0x083D3740,

        // The following should be consistant across languages and revisions.
        .loc_gSaveBlock1 = 0,
        .loc_gSaveDataBuffer = 0x02039A38, // The location of the saveDataBuffer
        .loc_gSpecialVar_0x8000 = 0x020370B8,
        .loc_gMPlayInfo_BGM = 0x03007300,
        .loc_gMPlayInfo_SE2 = 0x03007380,
        .loc_sFanfareCounter = 0x03000FC6,
        .loc_gPlttBufferFaded = 0x020375F8,
        .loc_gSprites = 0x0202063C,

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
    { // ENG_LEAFGREEN_v0
        .is_valid = true,

        .gamecode = LEAFGREEN_ID,
        .version = VERS_1_0,
        .language = LANG_ENG,

        .loc_copyMonToPC = 0x08040B90,
        .loc_getSetPokedexFlag = 0x08088E48,
        .loc_readFlashSector = 0x080DA164,
        .loc_loadSaveSection30 = 0x0815D798, // The location of the function that loads save section 30: CEReaderTool_LoadTrainerTower_r in FRLG, TryReadTrainerHill_Internal in Emerald
        .loc_m4aMPlayStop = 0x081DD918,
        .loc_MPlayStart = 0x081DD834,
        .loc_CreateFanfareTask = 0x08071CEC,
        .loc_voicegroup = 0x08498154,
        .loc_sPicTable_NPC = 0x083A0A98,

        .loc_gMonFrontPicTable = 0x08235088,
        .loc_gMonPaletteTable = 0x082372E8,
        .loc_gMonShinyPaletteTable = 0x082380A8,
        .loc_gMonIconTable = 0x083D35DC,
        .loc_gMonIconPaletteIndices = 0x083D3CBC,
        .loc_gMonIconPalettes = 0x083D357C,

        // The following should be consistant across languages and revisions.
        .loc_gSaveBlock1 = 0,
        .loc_gSaveDataBuffer = 0x02039A38, // The location of the saveDataBuffer
        .loc_gSpecialVar_0x8000 = 0x020370B8,
        .loc_gMPlayInfo_BGM = 0x03007300,
        .loc_gMPlayInfo_SE2 = 0x03007380,
        .loc_sFanfareCounter = 0x03000FC6,
        .loc_gPlttBufferFaded = 0x020375F8,
        .loc_gSprites = 0x0202063C,

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
    { // ENG_FIRERED_v1
        .is_valid = true,

        .gamecode = FIRERED_ID,
        .version = VERS_1_1,
        .language = LANG_ENG,

        .loc_copyMonToPC = 0x08040BA4,
        .loc_getSetPokedexFlag = 0x08088E88,
        .loc_readFlashSector = 0x080DA1A4,
        .loc_loadSaveSection30 = 0x0815D834, // The location of the function that loads save section 30: CEReaderTool_LoadTrainerTower_r in FRLG, TryReadTrainerHill_Internal in Emerald
        .loc_m4aMPlayStop = 0x081DD9AC,
        .loc_MPlayStart = 0x081DD8C8,
        .loc_CreateFanfareTask = 0x08071D00,
        .loc_voicegroup = 0x084988D8,
        .loc_sPicTable_NPC = 0x083A0B28,

        .loc_gMonFrontPicTable = 0x0823511C,
        .loc_gMonPaletteTable = 0x0823737C,
        .loc_gMonShinyPaletteTable = 0x0823813C,
        .loc_gMonIconTable = 0x083D3810,
        .loc_gMonIconPaletteIndices = 0x083D3EF0,
        .loc_gMonIconPalettes = 0x083D37B0,

        // The following should be consistant across languages and revisions.
        .loc_gSaveBlock1 = 0,
        .loc_gSaveDataBuffer = 0x02039A38, // The location of the saveDataBuffer
        .loc_gSpecialVar_0x8000 = 0x020370B8,
        .loc_gMPlayInfo_BGM = 0x03007300,
        .loc_gMPlayInfo_SE2 = 0x03007380,
        .loc_sFanfareCounter = 0x03000FC6,
        .loc_gPlttBufferFaded = 0x020375F8,
        .loc_gSprites = 0x0202063C,

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
    { // ENG_LEAFGREEN_v1
        .is_valid = true,

        .gamecode = LEAFGREEN_ID,
        .version = VERS_1_1,
        .language = LANG_ENG,

        .loc_copyMonToPC = 0x08040BA4,
        .loc_getSetPokedexFlag = 0x08088E5C,
        .loc_readFlashSector = 0x080DA178,
        .loc_loadSaveSection30 = 0x0815D810, // The location of the function that loads save section 30: CEReaderTool_LoadTrainerTower_r in FRLG, TryReadTrainerHill_Internal in Emerald
        .loc_m4aMPlayStop = 0x081DD988,
        .loc_MPlayStart = 0x081DD8A4,
        .loc_CreateFanfareTask = 0x08071D00,
        .loc_voicegroup = 0x084981C4,
        .loc_sPicTable_NPC = 0x083A0B08,

        .loc_gMonFrontPicTable = 0x082350F8,
        .loc_gMonPaletteTable = 0x08237358,
        .loc_gMonShinyPaletteTable = 0x08238118,
        .loc_gMonIconTable = 0x083D364C,
        .loc_gMonIconPaletteIndices = 0x083D3D2C,
        .loc_gMonIconPalettes = 0x083D35EC,

        // The following should be consistant across languages and revisions.
        .loc_gSaveBlock1 = 0,
        .loc_gSaveDataBuffer = 0x02039A38, // The location of the saveDataBuffer
        .loc_gSpecialVar_0x8000 = 0x020370B8,
        .loc_gMPlayInfo_BGM = 0x03007300,
        .loc_gMPlayInfo_SE2 = 0x03007380,
        .loc_sFanfareCounter = 0x03000FC6,
        .loc_gPlttBufferFaded = 0x020375F8,
        .loc_gSprites = 0x0202063C,

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
    { // ENG_EMERALD_v0
        .is_valid = true,

        .gamecode = EMERALD_ID,
        .version = VERS_1_0,
        .language = LANG_ENG,

        .loc_copyMonToPC = 0x0806B490,
        .loc_getSetPokedexFlag = 0x080C0664,
        .loc_readFlashSector = 0x0815314C,
        .loc_loadSaveSection30 = 0x081D3AD8, // The location of the function that loads save section 30: CEReaderTool_LoadTrainerTower_r in FRLG, TryReadTrainerHill_Internal in Emerald
        .loc_m4aMPlayStop = 0x082E0978,
        .loc_MPlayStart = 0x082E0894,
        .loc_CreateFanfareTask = 0x080A3170,
        .loc_voicegroup = 0x08686A74,
        .loc_sPicTable_NPC = 0x08506404,

        .loc_gMonFrontPicTable = 0x08301418,
        .loc_gMonPaletteTable = 0x08303678,
        .loc_gMonShinyPaletteTable = 0x08304438,
        .loc_gMonIconTable = 0x0857BCA8,
        .loc_gMonIconPaletteIndices = 0x0857C388,
        .loc_gMonIconPalettes = 0x08DDE1F8,

        // The following should be consistant across languages and revisions.
        .loc_gSaveBlock1 = 0,
        .loc_gSaveDataBuffer = 0x0203ABBC, // The location of the saveDataBuffer
        .loc_gSpecialVar_0x8000 = 0x020375D8,
        .loc_gMPlayInfo_BGM = 0x03007420,
        .loc_gMPlayInfo_SE2 = 0x03007630,
        .loc_sFanfareCounter = 0x03000F4E,
        .loc_gPlttBufferFaded = 0x02037B14,
        .loc_gSprites = 0x02020630,

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

const u16 rom_data_values_eng_size = static_cast<u16>(sizeof(rom_data_values_eng) / sizeof(struct ROM_DATA));