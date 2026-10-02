#include "rom_values/gba_rom_values.h"

const struct ROM_DATA rom_data_values_ita[] = {
    {
        // ITA_RUBY_v0
        .is_valid = true,

        .gamecode = RUBY_ID,
        .version = VERS_1_0,
        .language = LANG_ITA,

        .loc_copyMonToPC = 0x803db64, // Known as "SendMonToPC"
        .loc_getSetPokedexFlag = 0x8091134,
        .loc_readFlashSector =
            0x8125f78, // Known as "DoReadFlashWholeSection" in RS
        .loc_loadSaveSection30 = 0,
        .loc_m4aMPlayStop = 0x81e03cc,
        .loc_MPlayStart = 0x81e02e8,
        .loc_CreateFanfareTask = 0x807531c,
        .loc_voicegroup = 0x844256c,
        .loc_sPicTable_NPC = 0x836ec78,

        .loc_gMonFrontPicTable = 0x081E9FF0,
        .loc_gMonPaletteTable = 0x081EC250,
        .loc_gMonShinyPaletteTable = 0x081ED010,
        .loc_gMonIconTable = 0x083BC974,
        .loc_gMonIconPaletteIndices = 0x083BD054,
        .loc_gMonIconPalettes = 0x08E96658,

        // The following should be consistant across languages and revisions.
        .loc_gSaveBlock1 = 0x02025734,
        .loc_gSaveDataBuffer = 0x02000000, // Known as "gSharedMem" in RS
        .loc_gSpecialVar_0x8000 = 0x0202E8C4,
        .loc_gMPlayInfo_BGM = 0x03007380 + 0x10,
        .loc_gMPlayInfo_SE2 = 0x03007400 + 0x10,
        .loc_sFanfareCounter = 0x030006DA,
        .loc_gPlttBufferFaded = 0x0202EEC8,
        .loc_gSprites = 0x02020004,

        .offset_ramscript = 0x3690, // Ramscript offset as found within the
                                    // SaveBlock1 struct in global.h
        .offset_flags = 0x1220, // Flag offset as found within the SaveBlock1
                                // struct in global.h
        .offset_wondercard = 0,
        .offset_script = 0x0810,
        .text_region = TEXT_HOENN,
        .special_DrawWholeMapView = 0x8E,

        // PKHeX's list of flags is useful for making sure the detection is
        // accurate:
        // https://github.com/kwsch/PKHeX/blob/78a557c3cdaa6f48b42cc96df8ccb4d20b897937/PKHeX.Core/Resources/text/other/flags_rs.txt
        .e4_flag = 0x800 + 0x04, // The flag that is set when you become
                                 // champion. Often listed as "GAME_CLEAR"
        .mg_flag = 0x800 + 0x4C, // The flag that is set when you enable Mystery
                                 // Gift. Known as "EXDATA_ENABLE" in RS
        .unused_flag_start = 0x21, // The start of the unused flags and must
                                   // have 31 open flags in a row

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
        .loc_gSaveBlock1PTR =
            0 // TODO: Only used for old script, can be removed later
    },
    {
        // ITA_SAPPHIRE_v0
        .is_valid = true,

        .gamecode = SAPPHIRE_ID,
        .version = VERS_1_0,
        .language = LANG_ITA,

        .loc_copyMonToPC = 0x803db64, // Known as "SendMonToPC"
        .loc_getSetPokedexFlag = 0x8091134,
        .loc_readFlashSector =
            0x8125f78, // Known as "DoReadFlashWholeSection" in RS
        .loc_loadSaveSection30 = 0,
        .loc_m4aMPlayStop = 0x81e035c,
        .loc_MPlayStart = 0x81e0278,
        .loc_CreateFanfareTask = 0x8075320,
        .loc_voicegroup = 0x8442210,
        .loc_sPicTable_NPC = 0x836ec08,

        .loc_gMonFrontPicTable = 0x081E9F80,
        .loc_gMonPaletteTable = 0x081EC1E0,
        .loc_gMonShinyPaletteTable = 0x081ECFA0,
        .loc_gMonIconTable = 0x083BC618,
        .loc_gMonIconPaletteIndices = 0x083BCCF8,
        .loc_gMonIconPalettes = 0x08E96658,

        // The following should be consistant across languages and revisions.
        .loc_gSaveBlock1 = 0x02025734,
        .loc_gSaveDataBuffer = 0x02000000, // Known as "gSharedMem" in RS
        .loc_gSpecialVar_0x8000 = 0x0202E8C4,
        .loc_gMPlayInfo_BGM = 0x03007380 + 0x10,
        .loc_gMPlayInfo_SE2 = 0x03007400 + 0x10,
        .loc_sFanfareCounter = 0x030006DA,
        .loc_gPlttBufferFaded = 0x0202EEC8,
        .loc_gSprites = 0x02020004,

        .offset_ramscript = 0x3690, // Ramscript offset as found within the
                                    // SaveBlock1 struct in global.h
        .offset_flags = 0x1220, // Flag offset as found within the SaveBlock1
                                // struct in global.h
        .offset_wondercard = 0,
        .offset_script = 0x0810,
        .text_region = TEXT_HOENN,
        .special_DrawWholeMapView = 0x8E,

        // PKHeX's list of flags is useful for making sure the detection is
        // accurate:
        // https://github.com/kwsch/PKHeX/blob/78a557c3cdaa6f48b42cc96df8ccb4d20b897937/PKHeX.Core/Resources/text/other/flags_rs.txt
        .e4_flag = 0x800 + 0x04, // The flag that is set when you become
                                 // champion. Often listed as "GAME_CLEAR"
        .mg_flag = 0x800 + 0x4C, // The flag that is set when you enable Mystery
                                 // Gift. Known as "EXDATA_ENABLE" in RS
        .unused_flag_start = 0x21, // The start of the unused flags and must
                                   // have 31 open flags in a row

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
        .loc_gSaveBlock1PTR =
            0 // TODO: Only used for old script, can be removed later
    },
    {
        // ITA_RUBY_v1
        .is_valid = true,

        .gamecode = RUBY_ID,
        .version = VERS_1_1,
        .language = LANG_ITA,

        .loc_copyMonToPC = 0x803db64, // Known as "SendMonToPC"
        .loc_getSetPokedexFlag = 0x8091134,
        .loc_readFlashSector =
            0x8125f78, // Known as "DoReadFlashWholeSection" in RS
        .loc_loadSaveSection30 = 0,
        .loc_m4aMPlayStop = 0x81e03cc,
        .loc_MPlayStart = 0x81e02e8,
        .loc_CreateFanfareTask = 0x807531c,
        .loc_voicegroup = 0x844256c,
        .loc_sPicTable_NPC = 0x836ec78,

        .loc_gMonFrontPicTable = 0x081E9FF0,
        .loc_gMonPaletteTable = 0x081EC250,
        .loc_gMonShinyPaletteTable = 0x081ED010,
        .loc_gMonIconTable = 0x083BC974,
        .loc_gMonIconPaletteIndices = 0x083BD054,
        .loc_gMonIconPalettes = 0x08E96658,

        // The following should be consistant across languages and revisions.
        .loc_gSaveBlock1 = 0x02025734,
        .loc_gSaveDataBuffer = 0x02000000, // Known as "gSharedMem" in RS
        .loc_gSpecialVar_0x8000 = 0x0202E8C4,
        .loc_gMPlayInfo_BGM = 0x03007380 + 0x10,
        .loc_gMPlayInfo_SE2 = 0x03007400 + 0x10,
        .loc_sFanfareCounter = 0x030006DA,
        .loc_gPlttBufferFaded = 0x0202EEC8,
        .loc_gSprites = 0x02020004,

        .offset_ramscript = 0x3690, // Ramscript offset as found within the
                                    // SaveBlock1 struct in global.h
        .offset_flags = 0x1220, // Flag offset as found within the SaveBlock1
                                // struct in global.h
        .offset_wondercard = 0,
        .offset_script = 0x0810,
        .text_region = TEXT_HOENN,
        .special_DrawWholeMapView = 0x8E,

        // PKHeX's list of flags is useful for making sure the detection is
        // accurate:
        // https://github.com/kwsch/PKHeX/blob/78a557c3cdaa6f48b42cc96df8ccb4d20b897937/PKHeX.Core/Resources/text/other/flags_rs.txt
        .e4_flag = 0x800 + 0x04, // The flag that is set when you become
                                 // champion. Often listed as "GAME_CLEAR"
        .mg_flag = 0x800 + 0x4C, // The flag that is set when you enable Mystery
                                 // Gift. Known as "EXDATA_ENABLE" in RS
        .unused_flag_start = 0x21, // The start of the unused flags and must
                                   // have 31 open flags in a row

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
        .loc_gSaveBlock1PTR =
            0 // TODO: Only used for old script, can be removed later
    },
    {
        // ITA_SAPPHIRE_v1
        .is_valid = true,

        .gamecode = SAPPHIRE_ID,
        .version = VERS_1_1,
        .language = LANG_ITA,

        .loc_copyMonToPC = 0x803db64, // Known as "SendMonToPC"
        .loc_getSetPokedexFlag = 0x8091134,
        .loc_readFlashSector =
            0x8125f78, // Known as "DoReadFlashWholeSection" in RS
        .loc_loadSaveSection30 = 0,
        .loc_m4aMPlayStop = 0x81e035c,
        .loc_MPlayStart = 0x81e0278,
        .loc_CreateFanfareTask = 0x8075320,
        .loc_voicegroup = 0x8442210,
        .loc_sPicTable_NPC = 0x836ec08,

        .loc_gMonFrontPicTable = 0x081E9F80,
        .loc_gMonPaletteTable = 0x081EC1E0,
        .loc_gMonShinyPaletteTable = 0x081ECFA0,
        .loc_gMonIconTable = 0x083BC618,
        .loc_gMonIconPaletteIndices = 0x083BCCF8,
        .loc_gMonIconPalettes = 0x08E96658,

        // The following should be consistant across languages and revisions.
        .loc_gSaveBlock1 = 0x02025734,
        .loc_gSaveDataBuffer = 0x02000000, // Known as "gSharedMem" in RS
        .loc_gSpecialVar_0x8000 = 0x0202E8C4,
        .loc_gMPlayInfo_BGM = 0x03007380 + 0x10,
        .loc_gMPlayInfo_SE2 = 0x03007400 + 0x10,
        .loc_sFanfareCounter = 0x030006DA,
        .loc_gPlttBufferFaded = 0x0202EEC8,
        .loc_gSprites = 0x02020004,

        .offset_ramscript = 0x3690, // Ramscript offset as found within the
                                    // SaveBlock1 struct in global.h
        .offset_flags = 0x1220, // Flag offset as found within the SaveBlock1
                                // struct in global.h
        .offset_wondercard = 0,
        .offset_script = 0x0810,
        .text_region = TEXT_HOENN,
        .special_DrawWholeMapView = 0x8E,

        // PKHeX's list of flags is useful for making sure the detection is
        // accurate:
        // https://github.com/kwsch/PKHeX/blob/78a557c3cdaa6f48b42cc96df8ccb4d20b897937/PKHeX.Core/Resources/text/other/flags_rs.txt
        .e4_flag = 0x800 + 0x04, // The flag that is set when you become
                                 // champion. Often listed as "GAME_CLEAR"
        .mg_flag = 0x800 + 0x4C, // The flag that is set when you enable Mystery
                                 // Gift. Known as "EXDATA_ENABLE" in RS
        .unused_flag_start = 0x21, // The start of the unused flags and must
                                   // have 31 open flags in a row

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
        .loc_gSaveBlock1PTR =
            0 // TODO: Only used for old script, can be removed later
    },
    {
        // ITA_FIRERED_v0
        .is_valid = true,

        .gamecode = FIRERED_ID,
        .version = VERS_1_0,
        .language = LANG_ITA,

        .loc_copyMonToPC = 0x8040a7c, // Known as "SendMonToPC"
        .loc_getSetPokedexFlag = 0x8088f44,
        .loc_readFlashSector = 0x8104d5c,
        .loc_loadSaveSection30 =
            0x815d49c, // The location of the function that loads save section
                       // 30: CEReaderTool_LoadTrainerTower_r in FRLG,
                       // TryReadTrainerHill_Internal in Emerald
        .loc_m4aMPlayStop = 0x81dada8,
        .loc_MPlayStart = 0x81dacc4,
        .loc_CreateFanfareTask = 0x8071c3c,
        .loc_voicegroup = 0x848c2cc,
        .loc_sPicTable_NPC = 0x8399b38,

        .loc_gMonFrontPicTable = 0x0822E150,
        .loc_gMonPaletteTable = 0x082303B0,
        .loc_gMonShinyPaletteTable = 0x08231170,
        .loc_gMonIconTable = 0x083CC270,
        .loc_gMonIconPaletteIndices = 0x083CC950,
        .loc_gMonIconPalettes = 0x083CC210,

        // The following should be consistant across languages and revisions.
        .loc_gSaveBlock1 = 0,
        .loc_gSaveDataBuffer = 0x02039A38, // The location of the saveDataBuffer
        .loc_gSpecialVar_0x8000 = 0x020370B8,
        .loc_gMPlayInfo_BGM = 0x03007300 - 0x110,
        .loc_gMPlayInfo_SE2 = 0x03007380 - 0x110,
        .loc_sFanfareCounter = 0x03000FC6,
        .loc_gPlttBufferFaded = 0x020375F8,
        .loc_gSprites = 0x0202063C,

        .offset_ramscript = 0x361C, // Ramscript offset as found within the
                                    // SaveBlock1 struct in global.h
        .offset_flags = 0x0EE0, // Flag offset as found within the SaveBlock1
                                // struct in global.h
        .offset_wondercard = 0x0460,
        .offset_script = 0x079C,
        .text_region = TEXT_KANTO,
        .special_DrawWholeMapView = 0x8E,

        // PKHeX's list of flags is useful for making sure the detection is
        // accurate:
        // https://github.com/kwsch/PKHeX/blob/78a557c3cdaa6f48b42cc96df8ccb4d20b897937/PKHeX.Core/Resources/text/other/flags_rs.txt
        .e4_flag = 0x800 + 0x2C, // The flag that is set when you become
                                 // champion. Often listed as "GAME_CLEAR"
        .mg_flag = 0x800 + 0x39, // The flag that is set when you enable Mystery
                                 // Gift. Known as "EXDATA_ENABLE" in RS
        .unused_flag_start = 0xAF, // The start of the unused flags and must
                                   // have 31 open flags in a row

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

        .loc_gSaveBlock1PTR =
            0x03005008, // TODO: Only used for old script, can be removed later
    },
    {
        // ITA_LEAFGREEN_v0
        .is_valid = true,

        .gamecode = LEAFGREEN_ID,
        .version = VERS_1_0,
        .language = LANG_ITA,

        .loc_copyMonToPC = 0x8040a7c,
        .loc_getSetPokedexFlag = 0x8088f18,
        .loc_readFlashSector = 0x8104d34,
        .loc_loadSaveSection30 =
            0x815d478, // The location of the function that loads save section
                       // 30: CEReaderTool_LoadTrainerTower_r in FRLG,
                       // TryReadTrainerHill_Internal in Emerald
        .loc_m4aMPlayStop = 0x81dad84,
        .loc_MPlayStart = 0x81daca0,
        .loc_CreateFanfareTask = 0x8071c3c,
        .loc_voicegroup = 0x848ba64,
        .loc_sPicTable_NPC = 0x8399b18,

        .loc_gMonFrontPicTable = 0x0822E12C,
        .loc_gMonPaletteTable = 0x0823038C,
        .loc_gMonShinyPaletteTable = 0x0823114C,
        .loc_gMonIconTable = 0x083CC0AC,
        .loc_gMonIconPaletteIndices = 0x083CC78C,
        .loc_gMonIconPalettes = 0x083CC04C,

        // The following should be consistant across languages and revisions.
        .loc_gSaveBlock1 = 0,
        .loc_gSaveDataBuffer = 0x02039A38, // The location of the saveDataBuffer
        .loc_gSpecialVar_0x8000 = 0x020370B8,
        .loc_gMPlayInfo_BGM = 0x03007300 - 0x110,
        .loc_gMPlayInfo_SE2 = 0x03007380 - 0x110,
        .loc_sFanfareCounter = 0x03000FC6,
        .loc_gPlttBufferFaded = 0x020375F8,
        .loc_gSprites = 0x0202063C,

        .offset_ramscript = 0x361C, // Ramscript offset as found within the
                                    // SaveBlock1 struct in global.h
        .offset_flags = 0x0EE0, // Flag offset as found within the SaveBlock1
                                // struct in global.h
        .offset_wondercard = 0x0460,
        .offset_script = 0x079C,
        .text_region = TEXT_KANTO,
        .special_DrawWholeMapView = 0x8E,

        // PKHeX's list of flags is useful for making sure the detection is
        // accurate:
        // https://github.com/kwsch/PKHeX/blob/78a557c3cdaa6f48b42cc96df8ccb4d20b897937/PKHeX.Core/Resources/text/other/flags_rs.txt
        .e4_flag = 0x800 + 0x2C, // The flag that is set when you become
                                 // champion. Often listed as "GAME_CLEAR"
        .mg_flag = 0x800 + 0x39, // The flag that is set when you enable Mystery
                                 // Gift. Known as "EXDATA_ENABLE" in RS
        .unused_flag_start = 0xAF, // The start of the unused flags and must
                                   // have 31 open flags in a row

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

        .loc_gSaveBlock1PTR =
            0x03005008, // TODO: Only used for old script, can be removed later
    },
    {
        // ITA_EMERALD_v0
        .is_valid = true,

        .gamecode = EMERALD_ID,
        .version = VERS_1_0,
        .language = LANG_ITA,

        .loc_copyMonToPC = 0x806b494,
        .loc_getSetPokedexFlag = 0x80c0428,
        .loc_readFlashSector = 0x8152ce0,
        .loc_loadSaveSection30 =
            0x81d3620, // The location of the function that loads save section
                       // 30: CEReaderTool_LoadTrainerTower_r in FRLG,
                       // TryReadTrainerHill_Internal in Emerald
        .loc_m4aMPlayStop = 0x82e1904,
        .loc_MPlayStart = 0x82e1820,
        .loc_CreateFanfareTask = 0x80a3184,
        .loc_voicegroup = 0x8683088,
        .loc_sPicTable_NPC = 0x8503148,

        .loc_gMonFrontPicTable = 0x08300DDC,
        .loc_gMonPaletteTable = 0x0830303C,
        .loc_gMonShinyPaletteTable = 0x08303DFC,
        .loc_gMonIconTable = 0x0857838C,
        .loc_gMonIconPaletteIndices = 0x08578A6C,
        .loc_gMonIconPalettes = 0x08DDE1D8,

        // The following should be consistant across languages and revisions.
        .loc_gSaveBlock1 = 0,
        .loc_gSaveDataBuffer = 0x0203ABBC, // The location of the saveDataBuffer
        .loc_gSpecialVar_0x8000 = 0x020375D8,
        .loc_gMPlayInfo_BGM = 0x03007420,
        .loc_gMPlayInfo_SE2 = 0x03007630,
        .loc_sFanfareCounter = 0x03000F4E,
        .loc_gPlttBufferFaded = 0x02037B14,
        .loc_gSprites = 0x02020630,

        .offset_ramscript = 0x3728, // Ramscript offset as found within the
                                    // SaveBlock1 struct in global.h
        .offset_flags = 0x1270, // Flag offset as found within the SaveBlock1
                                // struct in global.h
        .offset_wondercard = 0x056C,
        .offset_script = 0x08A8,
        .text_region = TEXT_HOENN,
        .special_DrawWholeMapView = 0x91,

        // PKHeX's list of flags is useful for making sure the detection is
        // accurate:
        // https://github.com/kwsch/PKHeX/blob/78a557c3cdaa6f48b42cc96df8ccb4d20b897937/PKHeX.Core/Resources/text/other/flags_rs.txt
        .e4_flag = 0x860 + 0x04, // The flag that is set when you become
                                 // champion. Often listed as "GAME_CLEAR"
        .mg_flag = 0x860 + 0x7B, // The flag that is set when you enable Mystery
                                 // Gift. Known as "EXDATA_ENABLE" in RS
        .unused_flag_start = 0x20, // The start of the unused flags and must
                                   // have 31 open flags in a row

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

        .loc_gSaveBlock1PTR =
            0x03005D8C, // TODO: Only used for old script, can be removed later
    }};

const u16 rom_data_values_ita_size =
    static_cast<u16>(sizeof(rom_data_values_ita) / sizeof(struct ROM_DATA));
