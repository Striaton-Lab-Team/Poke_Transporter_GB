#include "dbg/debug_menu_functions.h"
#include "dbg/debug_menu.h"
#include "libraries/nanoprintf/nanoprintf.h"
#include "text_engine.h"
#include "pokemon_data.h"
#include "sprite_data.h"
#include "global_frame_controller.h"
#include "flash_mem.h"
#include "ptgb_save_data_manager.h"
#include "background_engine.h"
#include "libstd_replacements.h"
#include "sound.h"
#include "rom_data.h"
#include "Gen3CartridgeSaveReader.h"
#include "Gen3SaveManager.h"
#include "Gen3Pokemon.h"
#include "pokemon_data.h"
#include "translated_text.h"
#include "dbg/debug_mode.h"

#define LEFT 8
#define RIGHT (H_MAX - LEFT)
#define TOP 120
#define BOTTOM V_MAX

extern rom_data curr_GBA_rom;
extern u32 global_tile_id_end;

static const byte RSEFL_10_ANIV_Celebi_0BF5_ENG_[]  = {
  0xbe, 0x2b, 0x28, 0x78, 0x0a, 0x00, 0x00, 0x00, 0xbd, 0xbf, 0xc6, 0xbf, 0xbc, 0xc3, 0xff, 0x34, 
  0x01, 0x31, 0x02, 0x02, 0xa2, 0xa1, 0x00, 0xbb, 0xc8, 0xc3, 0xd0, 0x00, 0x17, 0x35, 0x00, 0x00, 
  0xfb, 0x00, 0x00, 0x00, 0x80, 0x43, 0x05, 0x00, 0x00, 0x64, 0x00, 0x00, 0xf6, 0x00, 0xf8, 0x00, 
  0xe2, 0x00, 0xc3, 0x00, 0x05, 0x0f, 0x28, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x46, 0x21, 0xe2, 0x31, 0xaf, 0x22, 0x00, 0x00, 0x00, 0x00
};

static Game convert_rom_game_id_to_ptgb_game(int game_id)
{
    switch (game_id)
    {
    case RUBY_ID:
        return RUBY;
    case SAPPHIRE_ID:
        return SAPPHIRE;
    case FIRERED_ID:
        return FIRERED;
    case LEAFGREEN_ID:
        return LEAFGREEN;
    case EMERALD_ID:
        return EMERALD;
    default:
        return GAME_UNKNOWN; // Default to unknown if the game ID is unrecognized
    }
}

static Language convert_rom_lang_to_ptgb_lang(int rom_lang)
{
    switch (rom_lang)
    {
    case LANG_JPN:
        return JAPANESE;
    case LANG_ENG:
        return ENGLISH;
    case LANG_FRE:
        return FRENCH;
    case LANG_ITA:
        return ITALIAN;
    case LANG_GER:
        return GERMAN;
    case LANG_SPA:
        return SPANISH;
    default:
        return LANGUAGE_UNKNOWN; // Default to unknown if the language is unrecognized
    }
}

/**
 * The trainer OT is encoded in the gen3 proprietary charset.
 * But if we want to print it with npf_snprintf, we need to convert it to UTF-8 first.
 */
static void convert_OT_to_utf8(const u8 *encoded_OT, u8 *output_buffer, const u16 *gen3_charset)
{
    u16 current_char;
    u32 num_bytes;
    u8 utf8_codepoint_buffer[4];
    u8 *cur_out = output_buffer;

    for (u32 i = 0; i < 7 && encoded_OT[i] != 0xFF; ++i)
    {
        // the charset maps each gen 3 character (represented as a byte) to a UTF-16 codepoint.
        // So we first convert the encoded OT character to UTF-16, and then convert that UTF-16 codepoint to UTF-8.
        current_char = convert_gen_3_char_to_utf16(gen3_charset, encoded_OT[i]);
        num_bytes = convert_utf16_to_utf8_char(current_char, utf8_codepoint_buffer);

        memcpy(cur_out, utf8_codepoint_buffer, num_bytes);
        cur_out += num_bytes;
    }
    *cur_out = '\0';
}

void show_debug_menu_section(void *context, unsigned user_param)
{
    (void)context; // unused
    const DebugMenuSection section_to_show = static_cast<DebugMenuSection>(user_param);
    push_debug_menu_section(section_to_show);
}

void show_text_debug_screen(void *context, unsigned user_param)
{
    (void)context;
    (void)user_param;

    tte_set_ink(INK_DARK_GREY);
    BG_FLEX = (BG_FLEX & ~BG_PRIO_MASK) | BG_PRIO(3);
    text_loop(SCRIPT_DEBUG);
}

void show_debug_info_screen(void *context, unsigned user_param)
{
    (void)context;
    (void)user_param;

    char flags_hex_str[16];
    char def_lang_hex_str[16];
    char text_buffer[256];
    uint16_t charset[256];
    const char *game_code;
    u32 pkmn_flags = 0;

    load_localized_charset(charset, 3, ENGLISH);
    if (key_held(KEY_UP) && key_held(KEY_L) && key_held(KEY_R))
    {
        set_treecko(true);
    }

    bool e4_flag = read_flag(curr_GBA_rom.e4_flag);
    bool mg_flag = read_flag(curr_GBA_rom.mg_flag);
    bool all_collected_flag = read_flag(curr_GBA_rom.all_collected_flag);
    for (int i = 0; i < 30; i++)
    {
        pkmn_flags |= (read_flag(curr_GBA_rom.pkmn_collected_flag_start + i) << i);
    }

    bool tutorial = get_tutorial_flag();
    int def_lang = get_def_lang_num();

    switch (curr_GBA_rom.gamecode)
    {
    case RUBY_ID:
        game_code = "-R-";
        break;
    case SAPPHIRE_ID:
        game_code = "-S-";
        break;
    case FIRERED_ID:
        game_code = "-F-";
        break;
    case LEAFGREEN_ID:
        game_code = "-L-";
        break;
    case EMERALD_ID:
        game_code = "-E-";
        break;
    default:
        game_code = "-UNK-";
        break;
    }

    n2hexstr(flags_hex_str, pkmn_flags);
    n2hexstr(def_lang_hex_str, def_lang);

    //create_textbox(4, 1, 160, 80, true);
    show_textbox();

    npf_snprintf(text_buffer, sizeof(text_buffer),
        "Debug info:\n\nG: %d%s%d\nF: %d%d%d-%s\nS:   %d-%s\n%s%s",
        curr_GBA_rom.language,
        game_code,
        curr_GBA_rom.version,
        e4_flag,
        mg_flag,
        all_collected_flag,
        flags_hex_str,
        tutorial,
        def_lang_hex_str,
        BUILD_INFO,
        get_treecko_enabled() ? ".T" : ""
    );

    ptgb_write_debug(charset, text_buffer, true);

    while (true)
    {
        if (key_hit(KEY_B))
        {
            hide_textbox();
            reload_textbox_background();
            return;
        }
        VBlankIntrWait();
    }
}

void dbg_set_byte_val(void *context, unsigned user_param)
{
    u8 *flag_ptr = (u8*)context;
    *flag_ptr = (u8)user_param;
}

void dbg_play_song(void *context, unsigned user_param)
{
    if(user_param == UINT32_MAX)
    {
        stop_song();
        return;
    }
    play_song(user_param, true);
}

void dbg_inject_pkmn(void *context, unsigned user_param)
{
    const Game game_type = convert_rom_game_id_to_ptgb_game(curr_GBA_rom.gamecode);
    const Language game_lang = convert_rom_lang_to_ptgb_lang(curr_GBA_rom.language);
    u32 ret;
    char text_buffer[256];
    u8 encoded_OT[8];
    u8 decoded_OT_utf8[32];
    int boxIndex = 0;

    Gen3CartridgeSaveReader reader(global_memory_buffer);
    Gen3SaveManager save_manager(game_type, game_lang, reader);

    PokemonTables tables;
    tables.load_gen3_charset(game_lang);

    Gen3Pokemon celebi;
    celebi.loadData(RSEFL_10_ANIV_Celebi_0BF5_ENG_, false);

    // front_sprite_tile_id is reserved behind the box sprites
    if(!g_debug_options.ignore_game_pak && !g_debug_options.ignore_game_pak_sprites)
    {
        u32 front_sprite_tile_id = global_tile_id_end + (30 * 16);
        load_sprite_compressed(grabbed_front_sprite, (const unsigned int *)*(u32 *)(curr_GBA_rom.loc_gMonFrontPicTable + (0 * 8)), front_sprite_tile_id, PULLED_SPRITE_PAL, ATTR0_SQUARE, ATTR1_SIZE_64x64, 1);
        update_front_box_sprite(&celebi, false);
        obj_set_pos(grabbed_front_sprite, 88, 16);
        obj_unhide(grabbed_front_sprite, 0);
    }

    save_manager.setNationalDexUnlocked(true);

    do
    {
        ret = save_manager.addPokemonToBox(boxIndex, celebi);
        if(ret != UINT32_MAX)
        {
            break;
        }
        ++boxIndex;

    } while (boxIndex < 14);

    if(ret == UINT32_MAX)
    {
        // Failed to inject the Pokémon, all boxes are full. You can handle this case as needed (e.g., show an error message).
        return;
    }

    save_manager.finishSave();
    reader.flush();
    save_manager.readTrainerName(encoded_OT, ret);

    create_textbox(BOX_TYPE_DIALOUGEBOX, true);
    show_textbox();

    tte_set_pos(LEFT, TOP);
    tte_erase_rect(LEFT, TOP, RIGHT, BOTTOM);

    convert_OT_to_utf8(encoded_OT, decoded_OT_utf8, tables.gen3_charset);
    npf_snprintf(text_buffer, sizeof(text_buffer), "%s received a Celebi!\n Celebi was sent to box %d!", decoded_OT_utf8, boxIndex);
    ptgb_write_debug(tables.gen3_charset, text_buffer, false);

    wait_for_user_to_continue(H_MAX, V_MAX);

    if(!g_debug_options.ignore_game_pak && !g_debug_options.ignore_game_pak_sprites)
    {
        obj_hide(grabbed_front_sprite);
    }

    hide_textbox();
    tte_erase_rect(LEFT, TOP, RIGHT, BOTTOM);
}

void dbg_unlock_mystery(void *context, unsigned user_param)
{
    const Game game_type = convert_rom_game_id_to_ptgb_game(curr_GBA_rom.gamecode);
    const Language game_lang = convert_rom_lang_to_ptgb_lang(curr_GBA_rom.language);
    Gen3CartridgeSaveReader reader(global_memory_buffer);
    Gen3SaveManager save_manager(game_type, game_lang, reader);

    if(user_param == 0)
    {
        // mystery event not supported by fire red or leaf green
        if(game_type == FIRERED || game_type == LEAFGREEN)
        {
            return;
        }
    }
    else
    {
        // mystery gift not supported by ruby or sapphire
        if(game_type == RUBY || game_type == SAPPHIRE)
        {
            return;
        }
    }

    save_manager.setMysteryEventUnlocked(user_param == 0);
    save_manager.setMysteryGiftUnlocked(user_param != 0);

    save_manager.finishSave();
    reader.flush();
}

void dbg_inject_wc3(void *context, unsigned user_param)
{
    const u8* wc3_data_ptr = reinterpret_cast<const u8*>(context);
    size_t wc3_data_size = user_param;
    if(wc3_data_ptr == nullptr)
    {
        return;
    }

    const Game game_type = convert_rom_game_id_to_ptgb_game(curr_GBA_rom.gamecode);
    if(game_type != EMERALD)
    {
        return;
    }

    const Language game_lang = convert_rom_lang_to_ptgb_lang(curr_GBA_rom.language);
    Gen3CartridgeSaveReader reader(global_memory_buffer);
    Gen3SaveManager save_manager(game_type, game_lang, reader);

    save_manager.setMysteryGiftUnlocked(user_param != 0);
    save_manager.injectMysteryGift(wc3_data_ptr, wc3_data_size);

    save_manager.finishSave();
    reader.flush();
}

void dbg_inject_me3(void *context, unsigned user_param)
{
    const u8* me3_data_ptr = reinterpret_cast<const u8*>(context);
    size_t me3_data_size = user_param;
    if(me3_data_ptr == nullptr)
    {
        return;
    }

    const Game game_type = convert_rom_game_id_to_ptgb_game(curr_GBA_rom.gamecode);
    if(game_type != RUBY && game_type != SAPPHIRE)
    {
        return;
    }

    const Language game_lang = convert_rom_lang_to_ptgb_lang(curr_GBA_rom.language);
    Gen3CartridgeSaveReader reader(global_memory_buffer);
    Gen3SaveManager save_manager(game_type, game_lang, reader);

    save_manager.setMysteryEventUnlocked(true);
    save_manager.injectMysteryEvent(me3_data_ptr, me3_data_size);

    save_manager.finishSave();
    reader.flush();
}