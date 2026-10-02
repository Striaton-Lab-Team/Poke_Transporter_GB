//Created by StevenChaulk https://github.com/stevenchaulk/arduino-poke-gen2

#ifndef GB_LINK_H

/*
typedef enum {
    FIRST_COMMUNICATION,
    POST_SAVE,
    SELECT_ROOM,
    TABLE_INTERACT,
    TEN_ZEROS_ONE,
    SEVENTY_SIX,
    TEN_ZEROS_TWO,
    FINAL_DESTINATION,
    SEND_POKEMON_DATA
} connection_state;


typedef enum
{
    NOT_CONNECTED,
    CONNECTED,
    TRADE_CENTRE,
    COLOSSEUM
} connection_state_t;

typedef enum
{
    INIT,
    READY_TO_GO,
    SEEN_FIRST_WAIT,
    SENDING_RANDOM_DATA,
    WAITING_TO_SEND_DATA,
    SENDING_DATA,
    SENDING_PATCH_DATA,
    MIMIC,
    TRADE_PENDING,
    TRADE_CONFIRMATION,
    DONE
} trade_centre_state_gen_II_t;

#define PKMN_BLANK					    0x00

#define ITEM_1_HIGHLIGHTED				0xD0
#define ITEM_2_HIGHLIGHTED				0xD1
#define ITEM_3_HIGHLIGHTED				0xD2
#define ITEM_1_SELECTED					0xD4
#define ITEM_2_SELECTED					0xD5
#define ITEM_2_SELECTED					0xD5
#define ITEM_3_SELECTED					0xD6

#define GEN_II_CABLE_TRADE_CENTER		0xD1
#define GEN_II_CABLE_CLUB_COLOSSEUM		0xD2

#define PKMN_MASTER					    0x01
#define PKMN_SLAVE					    0x02
#define PKMN_MASTER_GEN_III				0x8FFF	//??
#define PKMN_SLAVE_GEN_III				0xB9A0 	//??
#define PKMN_CONNECTED_I				0x60
#define PKMN_CONNECTED_II				0x61
#define PKMN_WAIT					    0x7F

#define PKMN_ACTION					    0x60

#define PKMN_TRADE_CENTRE				ITEM_1_SELECTED
#define PKMN_COLOSSEUM					ITEM_2_SELECTED
#define PKMN_BREAK_LINK					ITEM_3_SELECTED

#define TRADE_CENTRE_WAIT				0xFD

#define PLAYER_LENGTH_GEN_II 444	//11+8+2+(48*6)+(11*6)+(11*6)+3


#define GB_LINK_H
#include <tonc.h>

//void log(std::string text);
void wait(u32 verticalLines);
inline void VBLANK() {}
void init();
int start_link();
*/
#endif
