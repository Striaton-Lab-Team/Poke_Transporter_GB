#include "rom_data.h"
#include "dbg/debug_mode.h"
#include "pokemon_data.h"
#include "text_engine.h"
#include "ptgb_save_data_manager.h"
#include "gba_rom_values/gba_rom_values.h"
#include "libraries/nanoprintf/nanoprintf.h"
#include "gba_rom_values_eng_lz10_bin.h"
#include "gba_rom_values_fre_lz10_bin.h"
#include "gba_rom_values_ger_lz10_bin.h"
#include "gba_rom_values_ita_lz10_bin.h"
#include "gba_rom_values_jpn_lz10_bin.h"
#include "gba_rom_values_spa_lz10_bin.h"

extern rom_data curr_GBA_rom;

rom_data::rom_data() {}
bool rom_data::load_rom(bool debug)
{
    u8 rom_list_buffer[2048];
    u32 rom_list_size;
    const u8 *compressed_rom_list;
    const u8 *cur;

    if (debug)
    {
        gamecode = DEBUG_GAME;
        version = DEBUG_VERS;
        language = DEBUG_LANG;
    }
    else
    {
        gamecode = (*(vu8 *)(0x80000AC)) << 0x10 |
                   (*(vu8 *)(0x80000AD)) << 0x08 |
                   (*(vu8 *)(0x80000AE)) << 0x00;
        language = (*(vu8 *)(0x80000AF));
        version = (*(vu8 *)(0x80000BC));
    }

    switch (language)
    {
    case LANG_JPN:
        compressed_rom_list = gba_rom_values_jpn_lz10_bin;
        break;
    case LANG_ENG:
        compressed_rom_list = gba_rom_values_eng_lz10_bin;
        break;
    case LANG_FRE:
        compressed_rom_list = gba_rom_values_fre_lz10_bin;
        break;
    case LANG_GER:
        compressed_rom_list = gba_rom_values_ger_lz10_bin;
        break;
    case LANG_ITA:
        compressed_rom_list = gba_rom_values_ita_lz10_bin;
        break;
    case LANG_SPA:
        compressed_rom_list = gba_rom_values_spa_lz10_bin;
        break;
    default:
        return false; // Unsupported language
    }

    // Game looks good, let's make sure it can save.
    /*if (!check_can_save())
    {
        return false;
    }*/

    // byte 2-4 of the compressed data store the decompressed size
    rom_list_size = compressed_rom_list[1] | (compressed_rom_list[2] << 8) | (compressed_rom_list[3] << 16);
    LZ77UnCompWram(compressed_rom_list, rom_list_buffer);

    cur = rom_list_buffer;

    while (cur < rom_list_buffer + rom_list_size)
    {
        const ROM_DATA *rom_values = reinterpret_cast<const ROM_DATA *>(cur);
        if (rom_values->is_valid && rom_values->gamecode == gamecode &&
            rom_values->version == version)
        {
            fill_values(rom_values);
            rom_loaded = true;
            return true;
        }
        cur += sizeof(ROM_DATA);
    }
    // If we reach here, no matching ROM_DATA was found
    return false;
}

void rom_data::fill_values(const ROM_DATA *rom_values)
{
    loc_gMonIconPalettes = rom_values->loc_gMonIconPalettes;
    loc_gMonFrontPicTable = rom_values->loc_gMonFrontPicTable;
    loc_gMonPaletteTable = rom_values->loc_gMonPaletteTable;
    loc_gMonShinyPaletteTable = rom_values->loc_gMonShinyPaletteTable;
    loc_gMonIconTable = rom_values->loc_gMonIconTable;
    loc_gMonIconPaletteIndices = rom_values->loc_gMonIconPaletteIndices;

    offset_flags = rom_values->offset_flags;
    offset_wondercard = rom_values->offset_wondercard;
    offset_script = rom_values->offset_script;

    e4_flag = rom_values->e4_flag;                                 // The flag that is set when you become champion. Often listed as "GAME_CLEAR"
    mg_flag = rom_values->mg_flag;                                 // The flag that is set when you enable Mystery Gift. Known as "EXDATA_ENABLE" in RS
    unused_flag_start = rom_values->unused_flag_start;             // The start of the unused flags and must have 31 open flags in a row
    all_collected_flag = rom_values->unused_flag_start;            // The flag for if everything has been collected
    pkmn_collected_flag_start = rom_values->unused_flag_start + 1; // The beginning of the flags for each of the Pokemon

    map_bank = rom_values->map_bank;
    map_id = rom_values->map_id;
    npc_id = rom_values->npc_id;

    def_map_bank = rom_values->def_map_bank;
    def_map_id = rom_values->def_map_id;
    def_npc_id = rom_values->def_npc_id;
}

bool rom_data::is_hoenn()
{
    return (gamecode == RUBY_ID || gamecode == SAPPHIRE_ID || gamecode == EMERALD_ID);
}

bool rom_data::is_ruby_sapphire()
{
    return (gamecode == RUBY_ID || gamecode == SAPPHIRE_ID);
}

void rom_data::print_rom_info()
{
    char buffer[64];
    char gameTypeChar;
    switch (gamecode)
    {
    case (RUBY_ID):
        gameTypeChar = 'R';
        break;
    case (SAPPHIRE_ID):
        gameTypeChar = 'S';
        break;
    case (FIRERED_ID):
        gameTypeChar = 'F';
        break;
    case (LEAFGREEN_ID):
        gameTypeChar = 'L';
        break;
    case (EMERALD_ID):
        gameTypeChar = 'E';
        break;
    default:
        gameTypeChar = '0';
        break;
    }

    npf_snprintf(buffer, sizeof(buffer), "%c-%d-%c", gameTypeChar, version, language);

    ptgb_write_simple(reinterpret_cast<const byte *>(buffer), true);
}

bool rom_data::verify_rom()
{
    return !rom_loaded ||
           g_debug_options.ignore_game_pak ||
           ((gamecode == ((*(vu8 *)(0x80000AC)) << 0x10 | (*(vu8 *)(0x80000AD)) << 0x08 | (*(vu8 *)(0x80000AE)) << 0x00)) &&
            (language == (*(vu8 *)(0x80000AF))) &&
            (version == (*(vu8 *)(0x80000BC))));
}