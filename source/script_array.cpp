#include "script_array.h"
#include "background_engine.h"
#include "box_menu.h"
#include "button_menu.h"
#include "flash_mem.h"
#include "gb_rom_values/gb_rom_values.h"
#include "global_frame_controller.h"
#include "link_handler.h"
#include "mystery_gift_builder.h"
#include "mystery_gift_injector.h"
#include "pokemon_data.h"
#include "select_menu.h"
#include "sprite_data.h"
#include "text_engine.h"
#include "text_tables.h"
#include "translated_text.h"
#include <tonc.h>

int last_error;
bool newPkmn = false;
PokemonTables pokeTable;
PokeBox box(&pokeTable);

Select_Menu langs(false, LANG_MENU, 18, 0);
Select_Menu games(false, CART_MENU, 18, 0);
Box_Menu box_viewer;

// For documentation purposes, here's an overview of the categories of the
// transfer script:
// -------- TRANSFER SCRIPT --------
//
// Check that the conditions are set for the transfer
// T_SCRIPT_START
// COND_TUTORIAL_COMPLETE
// PTGB_DIA_OPEN
// CMD_SET_TUTOR_TRUE
// COND_BEAT_E4
// PTGB_DIA_E4
// COND_MG_ENABLED
// COND_IS_FRLGE
// PTGB_DIA_MG_FRLGE
// PTGB_DIA_MG_RS
// COND_MG_OTHER_EVENT
// COND_PKMN_TO_COLLECT
// PTGB_DIA_MG_OTHER_EVENT
// PTGB_DIA_PKMN_TO_COLLECT
//
// Ask the user what game and language they're using
// PTGB_DIA_WHAT_GAME_TRANS
// CMD_GAME_MENU
// PTGB_DIA_WHAT_LANG_TRANS
// CMD_LANG_MENU
// PTGB_DIA_ASK_QUEST
// CMD_SLIDE_PROF_LEFT
// CMD_SLIDE_PROF_RIGHT
// COND_GB_ROM_EXISTS
// PTGB_DIA_NO_GB_ROM
// PTGB_DIA_MENU_BACK
//
// Initiate the transfer and check for errors
// PTGB_DIA_LETS_START
// PTGB_DIA_START
// CMD_START_LINK
// COND_ERROR_TIMEOUT_ONE
// PTGB_DIA_ERROR_TIME_ONE
// COND_ERROR_TIMEOUT_TWO
// PTGB_DIA_ERROR_TIME_TWO
// COND_ERROR_COM_ENDED
// PTGB_DIA_ERROR_COM_ENDED
// COND_ERROR_COLOSSEUM
// PTGB_DIA_ERROR_COLOSSEUM
// COND_ERROR_DISCONNECT
// PTGB_DIA_ERROR_DISCONNECT
//
// Pause the transfer and show the user their box data
// CMD_IS_A_VALID_PKMN
// PTGB_DIA_NO_VALID_PKMN
// COND_SOME_INVALID_PKMN
// PTGB_DIA_SOME_INVALID_PKMN
// COND_CHECK_MYTHIC
// PTGB_DIA_MYTHIC_CONVERT
// CMD_MYTHIC_MENU
// COND_CHECK_MISSINGNO
// PTGB_DIA_IS_MISSINGNO
// PTGB_DIA_IN_BOX
// CMD_BOX_MENU
// PTGB_DIA_CANCEL
// CMD_IMPORT_POKEMON
// CMD_CONTINUE_LINK
// CMD_CANCEL_LINK
// CMD_END_MISSINGNO
//
// Complete the transfer and give messages based on the transfered Pokemon
// PTGB_DIA_TRANS_GOOD
// COND_NEW_POKEMON
// PTGB_DIA_NEW_DEX
// PTGB_DIA_NO_NEW_DEX
// COND_IS_HOENN_RS
// COND_IS_HOENN_E
// PTGB_DIA_SEND_FRIEND_HOENN_RS
// PTGB_DIA_SEND_FRIEND_HOENN_E
// PTGB_DIA_SEND_FRIEND_KANTO
// PTGB_DIA_THANK
//
// Hide the dialogue and professor
// CMD_END_SCRIPT
// CMD_BACK_TO_MENU

// The array below is exactly in order of the DIA, commands and condition int
// values So if adding a new dia, command or condition, make sure to add it in
// the right position/index. defining it this way does not generate an expensive
// function. It's all stored as read-only data in EWRAM.
const script_obj_params transfer_script_params[SCRIPT_SIZE] = {
    // PTGB_DIA_OPEN
    {.text_entry_index = PTGB_DIA_OPEN, .next_if_true = CMD_SET_TUTOR_TRUE},
    // PTGB_DIA_E4
    {.text_entry_index = PTGB_DIA_E4, .next_if_true = CMD_END_SCRIPT},
    // PTGB_DIA_MG_FRLGE
    {.text_entry_index = PTGB_DIA_MG_FRLGE, .next_if_true = CMD_END_SCRIPT},
    // PTGB_DIA_MG_RS
    {.text_entry_index = PTGB_DIA_MG_RS, .next_if_true = CMD_END_SCRIPT},
    // PTGB_DIA_LETS_START
    {.text_entry_index = PTGB_DIA_LETS_START, .next_if_true = PTGB_DIA_START},
    // PTGB_DIA_START
    {.text_entry_index = PTGB_DIA_START, .next_if_true = CMD_START_LINK},
    // PTGB_DIA_TRANS_GOOD
    {.text_entry_index = PTGB_DIA_TRANS_GOOD, .next_if_true = COND_NEW_POKEMON},
    // PTGB_DIA_NEW_DEX
    {.text_entry_index = PTGB_DIA_NEW_DEX, .next_if_true = COND_IS_HOENN_RS},
    // PTGB_DIA_NO_NEW_DEX
    {.text_entry_index = PTGB_DIA_NO_NEW_DEX, .next_if_true = COND_IS_HOENN_RS},
    // PTGB_DIA_SEND_FRIEND_KANTO
    {.text_entry_index = PTGB_DIA_SEND_FRIEND_KANTO,
        .next_if_true = PTGB_DIA_THANK},
    // PTGB_DIA_SEND_FRIEND_HOENN_RS
    {.text_entry_index = PTGB_DIA_SEND_FRIEND_HOENN_RS,
        .next_if_true = PTGB_DIA_THANK},
    // PTGB_DIA_SEND_FRIEND_HOENN_E
    {.text_entry_index = PTGB_DIA_SEND_FRIEND_HOENN_E,
        .next_if_true = PTGB_DIA_THANK},
    // PTGB_DIA_THANK
    {.text_entry_index = PTGB_DIA_THANK, .next_if_true = CMD_END_SCRIPT},
    // PTGB_DIA_GET_MON (unused)
    {.text_entry_index = PTGB_DIA_GET_MON, .next_if_true = CMD_END_SCRIPT},
    // PTGB_DIA_MG_OTHER_EVENT
    {.text_entry_index = PTGB_DIA_MG_OTHER_EVENT,
        .next_if_true = PTGB_DIA_LETS_START},
    // PTGB_DIA_PKMN_TO_COLLECT
    {.text_entry_index = PTGB_DIA_PKMN_TO_COLLECT,
        .next_if_true = CMD_END_SCRIPT},
    // PTGB_DIA_NO_VALID_PKMN
    {.text_entry_index = PTGB_DIA_NO_VALID_PKMN,
        .next_if_true = CMD_CANCEL_LINK},
    // PTGB_DIA_ASK_QUEST
    {.text_entry_index = PTGB_DIA_ASK_QUEST,
        .next_if_true = CMD_SLIDE_PROF_LEFT},
    // PTGB_DIA_WHAT_GAME_TRANS
    {.text_entry_index = PTGB_DIA_WHAT_GAME_TRANS,
        .next_if_true = CMD_GAME_MENU},
    // PTGB_DIA_WHAT_LANG_TRANS
    {.text_entry_index = PTGB_DIA_WHAT_LANG_TRANS,
        .next_if_true = CMD_LANG_MENU},
    // PTGB_DIA_NO_GB_ROM
    {.text_entry_index = PTGB_DIA_NO_GB_ROM,
        .next_if_true = PTGB_DIA_WHAT_LANG_TRANS},
    // PTGB_DIA_IN_BOX
    {.text_entry_index = PTGB_DIA_IN_BOX, .next_if_true = CMD_BOX_MENU},
    // PTGB_DIA_MYTHIC_CONVERT
    {.text_entry_index = PTGB_DIA_MYTHIC_CONVERT,
        .next_if_true = CMD_MYTHIC_MENU},
    // PTGB_DIA_CANCEL
    {.text_entry_index = PTGB_DIA_CANCEL, .next_if_true = CMD_CANCEL_LINK},
    // PTGB_DIA_SOME_INVALID_PKMN
    {.text_entry_index = PTGB_DIA_SOME_INVALID_PKMN,
        .next_if_true = COND_CHECK_MYTHIC},
    // PTGB_DIA_MENU_BACK
    {.text_entry_index = PTGB_DIA_MENU_BACK, .next_if_true = CMD_END_SCRIPT},
    // PTGB_DIA_IS_MISSINGNO
    {.text_entry_index = PTGB_DIA_IS_MISSINGNO,
        .next_if_true = PTGB_DIA_IN_BOX},
    // PTGB_DIA_ERROR_COLOSSEUM
    {.text_entry_index = PTGB_DIA_ERROR_COLOSSEUM,
        .next_if_true = PTGB_DIA_START},
    // PTGB_DIA_ERROR_COM_ENDED
    {.text_entry_index = PTGB_DIA_ERROR_COM_ENDED,
        .next_if_true = PTGB_DIA_START},
    // PTGB_DIA_ERROR_DISCONNECT
    {.text_entry_index = PTGB_DIA_ERROR_DISCONNECT,
        .next_if_true = PTGB_DIA_START},
    // PTGB_DIA_ERROR_TIME_ONE
    {.text_entry_index = PTGB_DIA_ERROR_TIME_ONE,
        .next_if_true = PTGB_DIA_START},
    // PTGB_DIA_ERROR_TIME_TWO
    {.text_entry_index = PTGB_DIA_ERROR_TIME_TWO,
        .next_if_true = PTGB_DIA_START},
    // PTGB_DIA_WHAT_LANG_EVENT
    {.text_entry_index = PTGB_DIA_WHAT_LANG_EVENT,
        .next_if_true = CMD_LANG_MENU},
    // PTGB_DIA_WHAT_GAME_EVENT
    {.text_entry_index = PTGB_DIA_WHAT_GAME_EVENT,
        .next_if_true = CMD_GAME_MENU},
    // PTGB_DIA_K_DEX_NOT_FULL
    {.text_entry_index = PTGB_DIA_K_DEX_NOT_FULL,
        .next_if_true = CMD_END_SCRIPT},
    // PTGB_DIA_J_DEX_NOT_FULL
    {.text_entry_index = PTGB_DIA_J_DEX_NOT_FULL,
        .next_if_true = CMD_END_SCRIPT},
    // T_SCRIPT_START
    {.conditional_index = CMD_SHOW_PROF,
        .next_if_true = COND_TUTORIAL_COMPLETE},
    // E_SCRIPT_START
    {.conditional_index = CMD_SHOW_PROF, .next_if_true = PTGB_DIA_LETS_START},
    // CMD_START_LINK
    {.conditional_index = CMD_START_LINK,
        .next_if_true = COND_ERROR_TIMEOUT_ONE},
    // CMD_IMPORT_POKEMON
    {.conditional_index = CMD_IMPORT_POKEMON,
        .next_if_true = CMD_CONTINUE_LINK},
    // CMD_BACK_TO_MENU
    {.conditional_index = CMD_BACK_TO_MENU, .next_if_true = T_SCRIPT_START},
    // CMD_SHOW_PROF (unused in this manner (not used in a next_if_true).
    // Therefore this is a dummy entry)
    // we even need to define such unused values to keep the array element
    // positions in order
    {},
    // CMD_HIDE_PROF (unused in this manner (not used in a next_if_true).
    // Therefore this is a dummy entry)
    {},
    // CMD_SET_TUTOR_TRUE
    {.conditional_index = CMD_SET_TUTOR_TRUE, .next_if_true = CMD_END_SCRIPT},
    // CMD_END_SCRIPT
    {.conditional_index = CMD_END_SCRIPT, .next_if_true = CMD_BACK_TO_MENU},
    // CMD_GAME_MENU
    {.conditional_index = CMD_GAME_MENU,
        .next_if_true = COND_GB_ROM_EXISTS,
        .next_if_false = PTGB_DIA_WHAT_LANG_TRANS},
    // CMD_LANG_MENU
    {.conditional_index = CMD_LANG_MENU,
        .next_if_true = PTGB_DIA_WHAT_GAME_TRANS,
        .next_if_false = PTGB_DIA_CANCEL},
    // CMD_SLIDE_PROF_LEFT
    {.conditional_index = CMD_SLIDE_PROF_LEFT,
        .next_if_true = PTGB_DIA_WHAT_LANG_TRANS},
    // CMD_SLIDE_PROF_RIGHT
    {.conditional_index = CMD_SLIDE_PROF_RIGHT,
        .next_if_true = PTGB_DIA_LETS_START},
    // CMD_CONTINUE_LINK
    {.conditional_index = CMD_CONTINUE_LINK, .next_if_true = CMD_END_MISSINGNO},
    // CMD_BOX_MENU
    {.conditional_index = CMD_BOX_MENU,
        .next_if_true = CMD_IMPORT_POKEMON,
        .next_if_false = PTGB_DIA_CANCEL},
    // CMD_MYTHIC_MENU
    {.conditional_index = CMD_MYTHIC_MENU,
        .next_if_true = COND_CHECK_MISSINGNO},
    // CMD_IS_A_VALID_PKMN
    {.conditional_index = CMD_IS_A_VALID_PKMN,
        .next_if_true = COND_SOME_INVALID_PKMN,
        .next_if_false = PTGB_DIA_NO_VALID_PKMN},
    // CMD_CANCEL_LINK
    {.conditional_index = CMD_CANCEL_LINK, .next_if_true = CMD_END_SCRIPT},
    // CMD_END_MISSINGNO
    {.conditional_index = CMD_END_MISSINGNO,
        .next_if_true = PTGB_DIA_TRANS_GOOD},
    // COND_ERROR_TIMEOUT_ONE
    {.conditional_index = COND_ERROR_TIMEOUT_ONE,
        .next_if_true = COND_ERROR_TIMEOUT_TWO,
        .next_if_false = PTGB_DIA_ERROR_TIME_ONE},
    // COND_ERROR_DISCONNECT
    {.conditional_index = COND_ERROR_DISCONNECT,
        .next_if_true = CMD_IS_A_VALID_PKMN,
        .next_if_false = PTGB_DIA_ERROR_DISCONNECT},
    // COND_ERROR_COM_ENDED
    {.conditional_index = COND_ERROR_COM_ENDED,
        .next_if_true = COND_ERROR_COLOSSEUM,
        .next_if_false = PTGB_DIA_ERROR_COM_ENDED},
    // COND_ERROR_TIMEOUT_TWO
    {.conditional_index = COND_ERROR_TIMEOUT_TWO,
        .next_if_true = COND_ERROR_COM_ENDED,
        .next_if_false = PTGB_DIA_ERROR_TIME_TWO},
    // COND_ERROR_COLOSSEUM
    {.conditional_index = COND_ERROR_COLOSSEUM,
        .next_if_true = COND_ERROR_DISCONNECT,
        .next_if_false = PTGB_DIA_ERROR_COLOSSEUM},
    // COND_BEAT_E4
    {.conditional_index = COND_BEAT_E4,
        .next_if_true = COND_MG_ENABLED,
        .next_if_false = PTGB_DIA_E4},
    // COND_MG_ENABLED
    {.conditional_index = COND_MG_ENABLED,
        .next_if_true = COND_MG_OTHER_EVENT,
        .next_if_false = COND_IS_FRLGE},
    // COND_TUTORIAL_COMPLETE
    {.conditional_index = COND_TUTORIAL_COMPLETE,
        .next_if_true = COND_BEAT_E4,
        .next_if_false = PTGB_DIA_OPEN},
    // COND_NEW_POKEMON
    {.conditional_index = COND_NEW_POKEMON,
        .next_if_true = PTGB_DIA_NEW_DEX,
        .next_if_false = PTGB_DIA_NO_NEW_DEX},
    // COND_IS_HOENN_RS
    {.conditional_index = COND_IS_HOENN_RS,
        .next_if_true = PTGB_DIA_SEND_FRIEND_HOENN_RS,
        .next_if_false = COND_IS_HOENN_E},
    // COND_IS_FRLGE
    {.conditional_index = COND_IS_FRLGE,
        .next_if_true = PTGB_DIA_MG_FRLGE,
        .next_if_false = PTGB_DIA_MG_RS},
    // COND_MG_OTHER_EVENT
    {.conditional_index = COND_MG_OTHER_EVENT,
        .next_if_true = PTGB_DIA_MG_OTHER_EVENT,
        .next_if_false = COND_PKMN_TO_COLLECT},
    // COND_PKMN_TO_COLLECT
    {.conditional_index = COND_PKMN_TO_COLLECT,
        .next_if_true = PTGB_DIA_PKMN_TO_COLLECT,
        .next_if_false = PTGB_DIA_LETS_START},
    // COND_GB_ROM_EXISTS
    {.conditional_index = COND_GB_ROM_EXISTS,
        .next_if_true = CMD_SLIDE_PROF_RIGHT,
        .next_if_false = PTGB_DIA_NO_GB_ROM},
    // COND_CHECK_MYTHIC
    {.conditional_index = COND_CHECK_MYTHIC,
        .next_if_true = PTGB_DIA_MYTHIC_CONVERT,
        .next_if_false = COND_CHECK_MISSINGNO},
    // COND_CHECK_DEX
    {.conditional_index = COND_CHECK_DEX,
        .next_if_true = 0,
        .next_if_false = COND_CHECK_KANTO},
    // COND_CHECK_KANTO
    {.conditional_index = COND_CHECK_KANTO,
        .next_if_true = PTGB_DIA_K_DEX_NOT_FULL,
        .next_if_false = PTGB_DIA_J_DEX_NOT_FULL},
    // COND_SOME_INVALID_PKMN
    {.conditional_index = COND_SOME_INVALID_PKMN,
        .next_if_true = PTGB_DIA_SOME_INVALID_PKMN,
        .next_if_false = COND_CHECK_MYTHIC},
    // COND_IS_HOENN_E
    {.conditional_index = COND_IS_HOENN_E,
        .next_if_true = PTGB_DIA_SEND_FRIEND_HOENN_E,
        .next_if_false = PTGB_DIA_SEND_FRIEND_KANTO},
    // COND_CHECK_MISSINGNO
    {.conditional_index = COND_CHECK_MISSINGNO,
        .next_if_true = PTGB_DIA_IS_MISSINGNO,
        .next_if_false = PTGB_DIA_IN_BOX}};

// For documentation purposes, here's an overview of the categories of the
// events script:
// -------- EVENTS SCRIPT --------
// Start the dialogue and show the menu
// E_SCRIPT_START
// PTGB_DIA_ASK_QUEST
//
// Ask the user what game and language they're using
// PTGB_DIA_WHAT_GAME_EVENT
// CMD_GAME_MENU
// PTGB_DIA_WHAT_LANG_EVENT
// CMD_LANG_MENU
// PTGB_DIA_ASK_QUEST
// CMD_SLIDE_PROF_LEFT
// CMD_SLIDE_PROF_RIGHT
// COND_GB_ROM_EXISTS
// PTGB_DIA_NO_GB_ROM
//
// Check the player's dex
// COND_CHECK_DEX
// COND_CHECK_KANTO
// PTGB_DIA_K_DEX_NOT_FULL
// PTGB_DIA_J_DEX_NOT_FULL
//
// Hide the dialogue and professor
// CMD_END_SCRIPT
// CMD_BACK_TO_MENU

// The array below is exactly in order of the DIA, commands and condition int
// values So if adding a new dia, command or condition, make sure to add it in
// the right position/index. because things are in order, it also means we have
// to define every possible entry even if we don't use it. defining it this way
// does not generate an expensive function. It's all stored as read-only data in
// EWRAM. Although frankly given the small amount of defined entries, we ARE
// wasting a lot of space with this one. (probably around 500 bytes)
const script_obj_params event_script_params[SCRIPT_SIZE] = {
    {}, // PTGB_DIA_OPEN
    {}, // PTGB_DIA_E4
    {}, // PTGB_DIA_MG_FRLGE
    {}, // PTGB_DIA_MG_RS
    {}, // PTGB_DIA_LETS_START
    {}, // PTGB_DIA_START
    {}, // PTGB_DIA_TRANS_GOOD
    {}, // PTGB_DIA_NEW_DEX
    {}, // PTGB_DIA_NO_NEW_DEX
    {}, // PTGB_DIA_SEND_FRIEND_KANTO
    {}, // PTGB_DIA_SEND_FRIEND_HOENN_RS
    {}, // PTGB_DIA_SEND_FRIEND_HOENN_E
    {}, // PTGB_DIA_THANK
    {}, // PTGB_DIA_GET_MON
    {}, // PTGB_DIA_MG_OTHER_EVENT
    {}, // PTGB_DIA_PKMN_TO_COLLECT
    {}, // PTGB_DIA_NO_VALID_PKMN
    // PTGB_DIA_ASK_QUEST
    {.text_entry_index = PTGB_DIA_ASK_QUEST,
        .next_if_true = CMD_SLIDE_PROF_LEFT},
    {}, // PTGB_DIA_WHAT_GAME_TRANS
    {}, // PTGB_DIA_WHAT_LANG_TRANS
    // PTGB_DIA_NO_GB_ROM
    {.text_entry_index = PTGB_DIA_NO_GB_ROM,
        .next_if_true = PTGB_DIA_WHAT_LANG_EVENT},
    {}, // PTGB_DIA_IN_BOX
    {}, // PTGB_DIA_MYTHIC_CONVERT
    {}, // PTGB_DIA_CANCEL
    {}, // PTGB_DIA_SOME_INVALID_PKMN
    {}, // PTGB_DIA_MENU_BACK
    {}, // PTGB_DIA_IS_MISSINGNO
    {}, // PTGB_DIA_ERROR_COLOSSEUM
    {}, // PTGB_DIA_ERROR_COM_ENDED
    {}, // PTGB_DIA_ERROR_DISCONNECT
    {}, // PTGB_DIA_ERROR_TIME_ONE
    {}, // PTGB_DIA_ERROR_TIME_TWO
    // PTGB_DIA_WHAT_LANG_EVENT
    {.text_entry_index = PTGB_DIA_WHAT_LANG_EVENT,
        .next_if_true = CMD_LANG_MENU},
    // PTGB_DIA_WHAT_GAME_EVENT
    {.text_entry_index = PTGB_DIA_WHAT_GAME_EVENT,
        .next_if_true = CMD_GAME_MENU},
    // PTGB_DIA_K_DEX_NOT_FULL
    {.text_entry_index = PTGB_DIA_K_DEX_NOT_FULL,
        .next_if_true = CMD_END_SCRIPT},
    // PTGB_DIA_J_DEX_NOT_FULL
    {.text_entry_index = PTGB_DIA_J_DEX_NOT_FULL,
        .next_if_true = CMD_END_SCRIPT},
    {}, // T_SCRIPT_START
    // E_SCRIPT_START
    {.conditional_index = CMD_SHOW_PROF, .next_if_true = PTGB_DIA_ASK_QUEST},
    {}, // CMD_START_LINK
    {}, // CMD_IMPORT_POKEMON
    // CMD_BACK_TO_MENU
    {.conditional_index = CMD_BACK_TO_MENU, .next_if_true = T_SCRIPT_START},
    {}, // CMD_SHOW_PROF
    {}, // CMD_HIDE_PROF
    {}, // CMD_SET_TUTOR_TRUE
    // CMD_END_SCRIPT
    {.conditional_index = CMD_END_SCRIPT, .next_if_true = CMD_BACK_TO_MENU},
    // CMD_GAME_MENU
    {.conditional_index = CMD_GAME_MENU,
        .next_if_true = COND_GB_ROM_EXISTS,
        .next_if_false = PTGB_DIA_WHAT_LANG_EVENT},
    // CMD_LANG_MENU
    {.conditional_index = CMD_LANG_MENU,
        .next_if_true = PTGB_DIA_WHAT_GAME_EVENT},
    // CMD_SLIDE_PROF_LEFT
    {.conditional_index = CMD_SLIDE_PROF_LEFT,
        .next_if_true = PTGB_DIA_WHAT_LANG_EVENT},
    // CMD_SLIDE_PROF_RIGHT
    {.conditional_index = CMD_SLIDE_PROF_RIGHT, .next_if_true = COND_CHECK_DEX},
    {}, // CMD_CONTINUE_LINK
    {}, // CMD_BOX_MENU
    {}, // CMD_MYTHIC_MENU
    {}, // CMD_IS_A_VALID_PKMN
    {}, // CMD_CANCEL_LINK
    {}, // CMD_END_MISSINGNO
    {}, // COND_ERROR_TIMEOUT_ONE
    {}, // COND_ERROR_DISCONNECT
    {}, // COND_ERROR_COM_ENDED
    {}, // COND_ERROR_TIMEOUT_TWO
    {}, // COND_ERROR_COLOSSEUM
    {}, // COND_BEAT_E4
    {}, // COND_MG_ENABLED
    {}, // COND_TUTORIAL_COMPLETE
    {}, // COND_NEW_POKEMON
    {}, // COND_IS_HOENN_RS
    {}, // COND_IS_FRLGE
    {}, // COND_MG_OTHER_EVENT
    {}, // COND_PKMN_TO_COLLECT
    // COND_GB_ROM_EXISTS
    {.conditional_index = COND_GB_ROM_EXISTS,
        .next_if_true = CMD_SLIDE_PROF_RIGHT,
        .next_if_false = PTGB_DIA_NO_GB_ROM},
    {}, // COND_CHECK_MYTHIC
    // COND_CHECK_DEX
    {.conditional_index = COND_CHECK_DEX,
        .next_if_true = 0,
        .next_if_false = COND_CHECK_KANTO},
    // COND_CHECK_KANTO
    {.conditional_index = COND_CHECK_KANTO,
        .next_if_true = PTGB_DIA_K_DEX_NOT_FULL,
        .next_if_false = PTGB_DIA_J_DEX_NOT_FULL},
    {}, // COND_SOME_INVALID_PKMN
    {}, // COND_IS_HOENN_E
    {}, // COND_CHECK_MISSINGNO
};

void populate_lang_menu()
{
    langs.clear_options();

    langs.add_option(GENERAL_option_english, ENG_ID);
    langs.add_option(GENERAL_option_japanese, JPN_ID);
    langs.add_option(GENERAL_option_spanish, SPA_ID);
    langs.add_option(GENERAL_option_french, FRE_ID);
    langs.add_option(GENERAL_option_german, GER_ID);
    langs.add_option(GENERAL_option_italian, ITA_ID);
    langs.add_option(GENERAL_option_korean, KOR_ID);
    // TODO: Removing the cancel option for the time being, since canceling the
    // link trade when there is no link connection crashes the game
    // langs.add_option(GENERAL_option_cancel, UINT8_MAX);
}

void populate_game_menu(int lang)
{
    games.clear_options();

    switch (lang)
    {
    case (JPN_ID):
        games.add_option(GENERAL_option_red, RED_ID);
        games.add_option(GENERAL_option_green, GREEN_ID);
        games.add_option(GENERAL_option_blue, BLUE_ID);
        games.add_option(GENERAL_option_yellow, YELLOW_ID);
        games.add_option(GENERAL_option_gold, GOLD_ID);
        games.add_option(GENERAL_option_silver, SILVER_ID);
        games.add_option(GENERAL_option_crystal, CRYSTAL_ID);
        games.add_option(GENERAL_option_cancel, UINT8_MAX);
        break;

    case (KOR_ID):
        games.add_option(GENERAL_option_gold, GOLD_ID);
        games.add_option(GENERAL_option_silver, SILVER_ID);
        games.add_option(GENERAL_option_cancel, UINT8_MAX);
        break;

    default:
        games.add_option(GENERAL_option_red, RED_ID);
        games.add_option(GENERAL_option_blue, BLUE_ID);
        games.add_option(GENERAL_option_yellow, YELLOW_ID);
        games.add_option(GENERAL_option_gold, GOLD_ID);
        games.add_option(GENERAL_option_silver, SILVER_ID);
        games.add_option(GENERAL_option_crystal, CRYSTAL_ID);
        games.add_option(GENERAL_option_cancel, UINT8_MAX);
        break;
    }
}

bool run_conditional(int index)
{
    // Here is most of the logic that drives what lines show up where. It's
    // probably not the best way to code it, but it works
    bool ret;
    switch (index)
    {

    case COND_ERROR_COM_ENDED:
        return globalLinkCable.lastError != COND_ERROR_COM_ENDED;

    case COND_ERROR_DISCONNECT:
        return globalLinkCable.lastError != COND_ERROR_DISCONNECT;

    case COND_ERROR_TIMEOUT_ONE:
        return globalLinkCable.lastError != COND_ERROR_TIMEOUT_ONE;

    case COND_ERROR_TIMEOUT_TWO:
        return globalLinkCable.lastError != COND_ERROR_TIMEOUT_TWO;

    case COND_ERROR_COLOSSEUM:
        return globalLinkCable.lastError != COND_ERROR_COLOSSEUM;

    case COND_BEAT_E4:
        return read_flag(curr_GBA_rom.e4_flag) ||
               g_debug_options.ignore_mg_e4_flags;

    case COND_MG_ENABLED:
        return read_flag(curr_GBA_rom.mg_flag) ||
               g_debug_options.ignore_mg_e4_flags;

    case COND_TUTORIAL_COMPLETE:
        return get_tutorial_flag() && !g_debug_options.force_tutorial;

    case COND_NEW_POKEMON:
        //return box.getHasNewPkmn();    
        return newPkmn;

    case COND_IS_HOENN_RS:
        return curr_GBA_rom.is_ruby_sapphire();

    case COND_IS_FRLGE:
        return !curr_GBA_rom.is_ruby_sapphire();

    case COND_MG_OTHER_EVENT:
        return compare_map_and_npc_data(curr_GBA_rom.def_map_bank,
                   curr_GBA_rom.def_map_id, curr_GBA_rom.def_npc_id) &&
               !g_debug_options.ignore_mg_e4_flags;

    case COND_PKMN_TO_COLLECT:
        return compare_map_and_npc_data(curr_GBA_rom.map_bank,
                   curr_GBA_rom.map_id, curr_GBA_rom.npc_id) &&
               !read_flag(curr_GBA_rom.all_collected_flag) &&
               !g_debug_options.ignore_unreceived_pkmn;

    case COND_GB_ROM_EXISTS:
        return true;

    case COND_CHECK_MYTHIC:
        return box.getContainsMythical();

    case COND_CHECK_DEX:
        if (globalLinkCable.gen == 1)
        {
            return (get_dex_completion(1, false) == 150) ||
                   g_debug_options.ignore_dex_completion;
        }
        else
        {
            return (get_dex_completion(1, false) == 99) ||
                   g_debug_options.ignore_dex_completion;
        }

    case COND_CHECK_KANTO:
        return globalLinkCable.gen == 1;

    case COND_SOME_INVALID_PKMN:
        return box.getContainsInvalid();

    case COND_IS_HOENN_E:
        return curr_GBA_rom.gamecode == EMERALD_ID;

    case COND_CHECK_MISSINGNO:
        if (box.getContainsMissingNo())
        {
            set_missingno(true);
            return true;
        }
        return false;

    case CMD_START_LINK:
        load_flex_background(FBG_Fennel, 3);
        link_animation_state(STATE_CONNECTION);
        box.reset();
        if (g_debug_options.ignore_link_cable)
        {
            // TODO - Make this less hardcoded
            box.loadData(1, ENGLISH, gen1_rb_debug_box_data);
        }
        else
        {

            u8 general_text_table_buffer[2048];
            u8 lineBuffer[1024]; // text_helper/main.py restricts the lines to
                                 // 1024 bytes.
            const u8 **chunkList;
            u32 numChunks;
            u32 chunkSize;

            get_text_table_chunks(
                GENERAL_INDEX, &chunkList, &numChunks, &chunkSize);

            FileContainerReader general_text_reader(
                chunkList, numChunks, chunkSize);
            general_text_reader.init(
                general_text_table_buffer, sizeof(general_text_table_buffer));

            general_text_reader.readFile(GENERAL_connecting, lineBuffer);
            ptgb_write_textbox(lineBuffer, true, false, GENERAL_INDEX,
                GENERAL_connecting, false);

            load_select_sprites(NO_GB_ROM);
            obj_unhide(gba_cart, 0);
            obj_set_pos(gba_cart, 17 * 8, 14 * 8);

            obj_unhide(cart_shell, 0);
            obj_set_pos(cart_shell, (8 * 8), (11 * 8) + 11);

            obj_unhide(cart_label, 0);
            obj_set_pos(cart_label, (8 * 8) + 8, (11 * 8) + 11 + 13);

            obj_unhide(gba_flag, 0);
            obj_set_pos(gba_flag, 23 * 8, 14 * 8);

            u16 debug_charset[256];
            load_localized_charset(debug_charset, 3, ENGLISH);
            globalLinkCable.setup(debug_charset);

            globalLinkCable.LinkCommand_InitalizeConnection(false);
            while (globalLinkCable.exitState != END)
            {
                if (globalLinkCable.subStateChanged &&
                    !g_debug_options.print_link_data)
                {
                    switch (globalLinkCable.exitState)
                    {
                    case SAVE_SUCCESS:
                        general_text_reader.readFile(
                            GENERAL_link_success, lineBuffer);
                        ptgb_write_textbox(lineBuffer, true, false,
                            GENERAL_INDEX, GENERAL_link_success, false);
                        break;
                    case TRADE_PREAMBLE:
                        general_text_reader.readFile(
                            GENERAL_transferring, lineBuffer);
                        ptgb_write_textbox(lineBuffer, true, false,
                            GENERAL_INDEX, GENERAL_transferring, false);
                        break;
                    default:
                        break;
                    }
                }
                globalLinkCable.handleCartIO();
                VBlankIntrWait();
            }
            globalLinkCable.handleCartIO(); // Does this need to be here?

            load_select_sprites(globalLinkCable.currROM);

            obj_unhide(gba_cart, 0);
            obj_set_pos(gba_cart, 17 * 8, 14 * 8);

            obj_unhide(cart_shell, 0);
            obj_set_pos(cart_shell, (8 * 8), (11 * 8) + 11);

            obj_unhide(cart_label, 0);
            obj_set_pos(cart_label, (8 * 8) + 8, (11 * 8) + 11 + 13);

            obj_unhide(gba_flag, 0);
            obj_set_pos(gba_flag, 23 * 8, 14 * 8);

            obj_unhide(gb_flag, 0);
            obj_set_pos(gb_flag, 1.5 * 8, 14 * 8);
        
            globalLinkCable.LinkCommand_ReadMemorySection(globalLinkCable.pccsROMptr->wCurrentBoxNum, &globalLinkCable.boxNum, 1);

            byte boxDataArray[globalLinkCable.pccsROMptr->box_data_size];
            globalLinkCable.LinkCommand_ReadMemorySection(globalLinkCable.pccsROMptr->wBoxDataStart,
            boxDataArray, globalLinkCable.pccsROMptr->box_data_size);

            box.loadData(globalLinkCable.gen, globalLinkCable.lang,
            boxDataArray);
        }
        reload_textbox_background();
        load_flex_background(FBG_Fennel, 2);
        link_animation_state(0);

        return true;

    case CMD_IMPORT_POKEMON:
        inject_mystery(&box);
        return true;

    case CMD_BACK_TO_MENU:
        set_text_exit();
        REG_BG1HOFS = 0;
        load_flex_background(FBG_Fennel, 3);
        return true;

    case CMD_SHOW_PROF:
        // load_flex_background(FBG_Fennel, 3);
        return true;

    case CMD_HIDE_PROF:
        // load_flex_background(FBG_Fennel, 3);
        return true;

    case CMD_SET_TUTOR_TRUE:
        set_tutorial_flag(true);
        return true;

    case CMD_END_SCRIPT:
        if (get_missingno_enabled())
        {
            set_missingno(false);
        }
        globalLinkCable.LinkCommand_SoftReset();
        return true;

    case CMD_LANG_MENU:
        // No longer used
        return true;

    case CMD_GAME_MENU:
        // No longer used
        return true;

    case CMD_SLIDE_PROF_LEFT:
        for (int i = 0; i <= (8 * 7); i += 2)
        {
            REG_BG1HOFS = i + FENNEL_SHIFT;
            if (!g_debug_options.instant_text_speed)
            {
                VBlankIntrWait();
            }
        }
        return true;

    case CMD_SLIDE_PROF_RIGHT:
        for (int i = (8 * 7); i >= 0; i -= 2)
        {
            REG_BG1HOFS = i + FENNEL_SHIFT;
            if (!g_debug_options.instant_text_speed)
            {
                VBlankIntrWait();
            }
        }
        return true;

    case CMD_CONTINUE_LINK:
    {
        byte boxRemovalPayload[31];
        int arrayIndex = 0;
        for (int i = (globalLinkCable.lang == JPN_ID ? 29 : 19); i >= 0; i--)
        {
            if (box.getGen3Pokemon(i)->isValid)
            {
                box.removePokemon(i);
                if (!(DONT_TRANSFER_POKEMON_AT_INDEX_X &&
                        i == POKEMON_INDEX_TO_SKIP))
                {
                    boxRemovalPayload[arrayIndex] = i;
                    arrayIndex++;
                }
            }
        }
        boxRemovalPayload[arrayIndex] = 0xFF;
        arrayIndex++;
        globalLinkCable.LinkCommand_TransferPokemon(globalLinkCable.boxNum, boxRemovalPayload, arrayIndex);
    }
        return true;

    case CMD_BOX_MENU:
        hide_textbox();
        ret = (box_viewer.box_main(&box, globalLinkCable.vers) == CONFIRM_BUTTON);
        show_textbox();
        return ret;

    case CMD_MYTHIC_MENU:
        box.stabilize_mythical = yes_no_menu.button_main();
        return true;

    case CMD_IS_A_VALID_PKMN:
        return box.getNumValid() > 0 || g_debug_options.dont_hide_invalid_pkmn;

    case CMD_CANCEL_LINK:
        globalLinkCable.LinkCommand_SoftReset();
        return true;

    case CMD_END_MISSINGNO:
        if (get_missingno_enabled())
        {
            set_missingno(false);
        }
        return true;

    default:
        tte_set_pos(0, 0);
        tte_write("ERROR! No conditional found.");
        return false;
    }
}