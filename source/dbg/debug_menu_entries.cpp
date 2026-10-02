#include "dbg/debug_menu.h"
#include "dbg/debug_menu_entries.h"
#include "dbg/debug_menu_functions.h"
#include "dbg/debug_mode.h"
#include "soundbank.h"
#include "vertical_menu.h"

#include <cstdlib>

#if ENABLE_MYSTERY_GIFT
#include "dbg/old_sea_map.h"
#include "dbg/eon_ticket_rs.h"
#endif

static const option_data toggle_options[] = {
    {
        .text = "Off",
        .value = 0
    },
    {
        .text = "On",
        .value = 1
    }
};

static void fill_debug_menu_with_main_menu_entries(vertical_menu &menu, u16 *charset);
static void fill_debug_menu_with_injection_entries(vertical_menu &menu, u16 *charset);

static const MenuSectionMapEntry menu_section_map[] = {
    {
        .section = DebugMenuSection::MAIN,
        .fill_func = fill_debug_menu_with_main_menu_entries
    },
    {
        .section = DebugMenuSection::INJECTION,
        .fill_func = fill_debug_menu_with_injection_entries
    }
};

static const option_data cbl_data_options[] = {
    {
        .text = "Off",
        .value = WRITE_CABLE_DATA_MODE_OFF
    },
    {
        .text = "SRAM",
        .value = WRITE_CABLE_DATA_MODE_SRAM
    },
    {
        .text = "CART",
        .value = WRITE_CABLE_DATA_MODE_CART
    }
};

#define CBL_DATA_OPTIONS_COUNT (sizeof(cbl_data_options) / sizeof(option_data))

/**
 * @brief This helper function makes it easy to define a multiple choice debug row.
 */
static debug_menu_row_widget* __attribute__((noinline)) define_debug_choice_row(const u16 *charset, const char *labelText, on_execute_callback option_activate_callback, const option_data *options_metadata, u8 num_options, bool should_delete_options, u8 default_option_index, void *context)
{
    const debug_menu_row_data row_data = {
        .charset = charset,
        .labelText = labelText,
        .option_section = {
            .num_options = num_options,
            .options = options_metadata,
            .on_option_activate = option_activate_callback,
            .selected_option_index = default_option_index,
            .should_free_on_destruct = should_delete_options
        },
        .on_execute = nullptr,
        .context = context,
        .user_param = 0
    };
    return new debug_menu_row_widget(row_data);
}

/**
 * @brief This helper function makes it easy to define a toggle row.
 */
static debug_menu_row_widget* define_toggle_row(const u16 *charset, const char* labelText, on_execute_callback on_toggle_callback, bool default_value, void *context)
{
    return define_debug_choice_row(charset, labelText, on_toggle_callback, toggle_options, 2, false, default_value ? 1 : 0, context);
}

/**
 * @brief This helper function will define the song row, which will let you toggle between any available maxmod song or None at all.
 */
[[maybe_unused]] static debug_menu_row_widget* __attribute__((noinline)) define_song_row(const u16 *charset, const char* labelText)
{
    option_data *song_options = (option_data*)malloc(sizeof(option_data) * (MSL_NSONGS + 1));
    song_options[0] = {
        .text = "None",
        .value = UINT32_MAX
    };

    for(unsigned i = 0; i < MSL_NSONGS; ++i)
    {
        song_options[i + 1] = {
            .text = nullptr,
            .value = i
        };
    }

    return define_debug_choice_row(charset, labelText, dbg_play_song, song_options,  MSL_NSONGS + 1, true, 0, nullptr);
}

/**
 * @brief This helper function makes it easy to define a row that executes a function when you select it and press A.
 */
static debug_menu_row_widget* __attribute__((noinline)) define_executable_row(const u16 *charset, const char* labelText, on_execute_callback on_execute, unsigned user_param, void *context)
{
    const debug_menu_row_data row_data = {
        .charset = charset,
        .labelText = labelText,
        .option_section = {
            .num_options = 0,
            .options = nullptr,
            .on_option_activate = nullptr,
            .selected_option_index = 0,
            .should_free_on_destruct = false
        },
        .on_execute = on_execute,
        .context = context,
        .user_param = user_param
    };
    return new debug_menu_row_widget(row_data);
}

/// Here we define the actual entries of the main debug menu, using the helper functions defined above.
static void fill_debug_menu_with_main_menu_entries(vertical_menu &menu, u16 *charset)
{
    i_item_widget* item_widgets[] = {
#if ENABLE_TEXT_DEBUG_SCREEN
        define_executable_row(charset, "Text Debug", show_text_debug_screen, 0, nullptr),
#endif
        define_executable_row(charset, "Info", show_debug_info_screen, 0, nullptr),
        define_executable_row(charset, "Injection", show_debug_menu_section, static_cast<unsigned>(DebugMenuSection::INJECTION), nullptr),
#if ENABLE_MYSTERY_GIFT
        define_executable_row(charset, "Unlock MystE", dbg_unlock_mystery, 0, nullptr),
        define_executable_row(charset, "Unlock MystG", dbg_unlock_mystery, 1, nullptr),
#endif
#if ENABLE_SOUND
        define_song_row(charset, "Song"),
#endif
        define_toggle_row(charset, "Print Link", dbg_set_byte_val, g_debug_options.print_link_data, &g_debug_options.print_link_data),
        define_toggle_row(charset, "Instant Text", dbg_set_byte_val, g_debug_options.instant_text_speed, &g_debug_options.instant_text_speed),
        define_toggle_row(charset, "Ign Cart", dbg_set_byte_val, g_debug_options.ignore_game_pak, &g_debug_options.ignore_game_pak),
        define_toggle_row(charset, "Ign Sprites", dbg_set_byte_val, g_debug_options.ignore_game_pak_sprites, &g_debug_options.ignore_game_pak_sprites),
        define_toggle_row(charset, "Ign Link", dbg_set_byte_val, g_debug_options.ignore_link_cable, &g_debug_options.ignore_link_cable),
        define_toggle_row(charset, "Ign MG/E4", dbg_set_byte_val, g_debug_options.ignore_mg_e4_flags, &g_debug_options.ignore_mg_e4_flags),
        define_toggle_row(charset, "Ign Unrec PKMN", dbg_set_byte_val, g_debug_options.ignore_unreceived_pkmn, &g_debug_options.ignore_unreceived_pkmn),
        define_toggle_row(charset, "Force Tut", dbg_set_byte_val, g_debug_options.force_tutorial, &g_debug_options.force_tutorial),
        define_toggle_row(charset, "Show Invalid", dbg_set_byte_val, g_debug_options.dont_hide_invalid_pkmn, &g_debug_options.dont_hide_invalid_pkmn),
        define_toggle_row(charset, "Ign Dex Compl", dbg_set_byte_val, g_debug_options.ignore_dex_completion, &g_debug_options.ignore_dex_completion),
        define_toggle_row(charset, "Force Caught", dbg_set_byte_val, g_debug_options.force_all_caught, &g_debug_options.force_all_caught),
        define_toggle_row(charset, "Write Cbl Data", dbg_set_byte_val, g_debug_options.write_cable_data_to_save, &g_debug_options.write_cable_data_to_save),
        define_toggle_row(charset, "Disp CtrlChr", dbg_set_byte_val, g_debug_options.display_control_char, &g_debug_options.display_control_char)
    };

    menu.add_item_widgets(item_widgets, sizeof(item_widgets) / sizeof(item_widgets[0]));
}

/// This function defines the entries of the injection submenu
static void fill_debug_menu_with_injection_entries(vertical_menu &menu, u16 *charset)
{
    i_item_widget* item_widgets[] = {
#if ENABLE_DEBUG_PKMN_INJECTION
        define_executable_row(charset, "Inject Celebi", dbg_inject_pkmn, 0, nullptr),
#endif
#if ENABLE_MYSTERY_GIFT
        define_executable_row(charset, "Unlock MystE", dbg_unlock_mystery, 0, nullptr),
        define_executable_row(charset, "Unlock MystG", dbg_unlock_mystery, 1, nullptr),
        define_executable_row(charset, "Inj OldSeaMap(E)", dbg_inject_wc3, sizeof(OldSeaMap_E_custom), const_cast<u8*>(OldSeaMap_E_custom)),
        define_executable_row(charset, "Inj Eon Ticket(RS)", dbg_inject_me3, sizeof(RS_Item_Eon_Ticket_e_Card_ENG_US_), const_cast<u8*>(RS_Item_Eon_Ticket_e_Card_ENG_US_)),
        
#endif
    };
    menu.add_item_widgets(item_widgets, sizeof(item_widgets) / sizeof(item_widgets[0]));
}

void fill_debug_menu_with_entries(vertical_menu &menu, u16 *charset, DebugMenuSection section)
{
    const size_t num_sections = sizeof(menu_section_map) / sizeof(menu_section_map[0]);
    size_t i = 0;
    while(i < num_sections)
    {
        if(menu_section_map[i].section == section)
        {
            menu_section_map[i].fill_func(menu, charset);
            break;
        }
        ++i;
    }
}