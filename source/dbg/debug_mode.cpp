#include "dbg/debug_mode.h"

debug_options g_debug_options = 
{
    .print_link_data = (true && DEBUG_MODE),
    .print_link_packets = (true && DEBUG_MODE),
    .instant_text_speed = (true && DEBUG_MODE),
    .ignore_game_pak = (true && DEBUG_MODE),
    .ignore_game_pak_sprites = (false && DEBUG_MODE),
    .ignore_link_cable = (false && DEBUG_MODE),
    .ignore_mg_e4_flags = (true && DEBUG_MODE),
    .ignore_unreceived_pkmn = (true && DEBUG_MODE),
    .force_tutorial = (false && DEBUG_MODE),
    .dont_hide_invalid_pkmn = (false && DEBUG_MODE),
    .ignore_dex_completion = (false && DEBUG_MODE),
    .force_all_caught = (false && DEBUG_MODE),
    .write_cable_data_to_save = (u8)WRITE_CABLE_DATA_MODE_OFF,
    .load_cable_data_from_save = (u8)WRITE_CABLE_DATA_MODE_OFF,
    .display_control_char = (false && DEBUG_MODE)
};