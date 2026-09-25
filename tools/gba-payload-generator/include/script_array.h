#ifndef SCRIPT_ARRAY_H
#define SCRIPT_ARRAY_H

#include "script_obj.h"
#include "pokemon_party.h"
#include "rom_data.h"
#include "translated_text.h"

// Commands
#define T_SCRIPT_START DIA_END + 0
#define E_SCRIPT_START DIA_END + 1
#define CMD_START_LINK DIA_END + 2
#define CMD_IMPORT_POKEMON DIA_END + 3 // This one is special because it can be true or false
#define CMD_BACK_TO_MENU DIA_END + 4
#define CMD_SHOW_PROF DIA_END + 5
#define CMD_HIDE_PROF DIA_END + 6
#define CMD_SET_TUTOR_TRUE DIA_END + 7
#define CMD_END_SCRIPT DIA_END + 8
#define CMD_GAME_MENU DIA_END + 9
#define CMD_LANG_MENU DIA_END + 10
#define CMD_SLIDE_PROF_LEFT DIA_END + 11
#define CMD_SLIDE_PROF_RIGHT DIA_END + 12
#define CMD_CONTINUE_LINK DIA_END + 13
#define CMD_BOX_MENU DIA_END + 14
#define CMD_MYTHIC_MENU DIA_END + 15
#define CMD_IS_A_VALID_PKMN DIA_END + 16
#define CMD_CANCEL_LINK DIA_END + 17
#define CMD_END_MISSINGNO DIA_END + 18

#define CMD_SIZE 19
#define CMDS_END DIA_END + CMD_SIZE

// Conditionals
#define COND_ERROR_TIMEOUT_ONE CMDS_END + 0 // Didn't talk to the Cable Club attendant fast enough or cable not connected
#define COND_ERROR_DISCONNECT CMDS_END + 1  // If you unplug the cable after starting SPI
#define COND_ERROR_COM_ENDED CMDS_END + 2   // If you don't save the game
#define COND_ERROR_TIMEOUT_TWO CMDS_END + 3 // ???
#define COND_ERROR_COLOSSEUM CMDS_END + 4   // If the player selects the battle colosseum (shouldn't be possible)
#define COND_BEAT_E4 CMDS_END + 5
#define COND_MG_ENABLED CMDS_END + 6
#define COND_TUTORIAL_COMPLETE CMDS_END + 7
#define COND_NEW_POKEMON CMDS_END + 8
#define COND_IS_HOENN_RS CMDS_END + 9
#define COND_IS_FRLGE CMDS_END + 10
#define COND_MG_OTHER_EVENT CMDS_END + 11
#define COND_PKMN_TO_COLLECT CMDS_END + 12
#define COND_GB_ROM_EXISTS CMDS_END + 13
#define COND_CHECK_MYTHIC CMDS_END + 14
#define COND_CHECK_DEX CMDS_END + 15
#define COND_CHECK_KANTO CMDS_END + 16
#define COND_SOME_INVALID_PKMN CMDS_END + 17
#define COND_IS_HOENN_E CMDS_END + 18
#define COND_CHECK_MISSINGNO CMDS_END + 19

#define COND_SIZE 20
#define COND_END CMDS_END + COND_SIZE

// Ends

#define SCRIPT_SIZE COND_END

extern const script_obj_params transfer_script_params[];
extern const script_obj_params event_script_params[];
extern rom_data curr_GBA_rom;

bool run_conditional(int index);

#endif