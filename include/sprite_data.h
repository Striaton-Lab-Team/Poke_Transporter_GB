#ifndef SPRITE_DATA_H
#define SPRITE_DATA_H

#include <tonc.h>
#include "rom_data.h"
#include "box_menu.h"
#include "link_handler.h"

#define FENNEL_SHIFT 4

extern OBJ_ATTR obj_buffer[128];
extern OBJ_AFFINE *obj_aff_buffer;
extern int num_sprites;

#include "button_yes.h"
extern OBJ_ATTR *button_yes;
#include "button_no.h"
extern OBJ_ATTR *button_no;
#include "button_edge.h"

#include "types.h"
extern OBJ_ATTR *type_sprites[14];

#include "flag_jpn.h"
#include "flag_eng.h"
#include "flag_fre.h"
#include "flag_ita.h"
#include "flag_ger.h"
#include "flag_spa.h"
#include "flag_kor.h"
extern OBJ_ATTR *gb_flag;
extern OBJ_ATTR *gba_flag;
#include "Label_Green.h"
#include "Label_Red.h"
#include "Label_Blue.h"
#include "Label_Yellow.h"
#include "Label_Gold.h"
#include "Label_Silver.h"
#include "Label_Crystal.h"
#include "Label_Unknown.h"
extern OBJ_ATTR *cart_label;
#include "GB_Shell.h"
#include "GBC_Shell.h"
#include "GBS_Shell.h"
#include "GBCS_Shell.h"
extern OBJ_ATTR *cart_shell;

#include "arrows.h"
extern OBJ_ATTR *up_arrow;
extern OBJ_ATTR *down_arrow;
extern OBJ_ATTR *toggle_arrow_left;
extern OBJ_ATTR *toggle_arrow_right;
extern OBJ_ATTR *point_arrow;

#include "unique_duel_frame_menu_sprites.h"
extern OBJ_ATTR *party_sprites[30];
#include "box_select.h"
extern OBJ_ATTR *box_select;
#include "button_cancel_left.h"
extern OBJ_ATTR *button_cancel_left;
extern OBJ_ATTR *button_cancel_right;
#include "button_confirm_left.h"
extern OBJ_ATTR *button_confirm_left;
extern OBJ_ATTR *button_confirm_right;

#include "ptgb_logo_l.h"
extern OBJ_ATTR *ptgb_logo_l;
#include "ptgb_logo_r.h"
extern OBJ_ATTR *ptgb_logo_r;

#include "ruby_cart.h"
#include "sapphire_cart.h"
#include "fr_cart.h"
#include "lg_cart.h"
#include "emerald_cart.h"
extern OBJ_ATTR *gba_cart;

#include "link_frame1.h"
extern OBJ_ATTR *link_frame1;
#include "link_frame2.h"
extern OBJ_ATTR *link_frame2;
#include "link_frame3.h"
extern OBJ_ATTR *link_frame3;
#include "link_blobs.h"
extern OBJ_ATTR *link_blob1;
extern OBJ_ATTR *link_blob2;
extern OBJ_ATTR *link_blob3;

extern OBJ_ATTR *grabbed_front_sprite;

#define BG2VOF_SMALL_TEXTBOX 96
#define BG2VOF_LARGE_TEXTBOX 0

#define BTN_PAL 0
#define BTN_LIT_PAL 1
#define LOGO_PAL 2
#define TYPES_PAL1 3
#define TYPES_PAL2 4
#define GB_CART_PAL 5
#define GB_FLAG_PAL 6
#define GBA_FLAG_PAL 7
#define MENU_PAL_RED 8
#define MENU_PAL_BLU 9
#define MENU_PAL_GRN 10
#define MENU_PAL_BRN 11
#define MENU_PAL_PNK 12
#define MENU_PAL_START MENU_PAL_RED
#define GBA_CART_PAL 13
#define LINK_CABLE_PAL 14
#define PULLED_SPRITE_PAL 15

enum FlexBackground
{
    FBG_None = -1,
    FBG_Opening,
    FBG_Fennel,
    FBG_Dex,
    FBG_Main_Menu,
    FBG_Box_Green,
    FBG_Box_Red,
    FBG_Box_Blue,
    FBG_Box_Yellow,
    FBG_Box_Gold,
    FBG_Box_Silver,
    FBG_Box_Crystal,
};

//
// VRAM usage is heavily optimized so the rest of it can be used
// as additional work RAM in the future.
//
// VRAM Map
//   06000000 - 06000800 ( 2K): Eternal backdrop and textbox tileset
//   06000800 - 06002000 ( 6K): Flex BG tileset
//   06002000 - 06002400 ( 2K): BG0: Backdrop tilemap
//   06002800 - 06003000 ( 2K): BG1: Flex BG tilemap
//   06003000 - 06003800 ( 2K): BG2: Textbox tilemap
//   06003800 - 06004000 ( 2K): BG3: Text tilemap
//   06004000 -~06009000 (20K): Text rendering bitmap
//   06009000 - 06010000 (28K): Free work RAM (watch the 16-bit bus!)
//   06010000 - 06018000 (32K): Sprite tileset
//

#define TILESET_PTGB 0
#define TILESET_TEXT 1

#define TILESET_OFFSET_BACKDROP  0
#define TILESET_OFFSET_TEXTBOX  38
#define TILESET_OFFSET_FLEXBG   64

#define TILEMAP_BACKDROP 4
#define TILEMAP_FLEXBG   5
#define TILEMAP_TEXTBOX  6
#define TILEMAP_TEXT     7

extern rom_data curr_GBA_rom;

void load_sprite(OBJ_ATTR *sprite, const unsigned int objTiles[], int objTilesLen,
                 u32 &tile_id, u32 pal_bank, int attr0, int attr1, u32 priority);

void load_sprite_compressed(OBJ_ATTR *sprite, const unsigned int objTiles[],
                 u32 &tile_id, u32 pal_bank, int attr0, int attr1, u32 priority);
void load_background();
void set_background_pal(int curr_rom_id, bool dark, bool fade);
void load_textbox_background();
void load_flex_background(FlexBackground background_id, int layer);
void load_eternal_sprites();
void load_temp_box_sprites(PokeBox* box);
void load_type_sprites(const u8* pkmn_type_table, int pkmn_index, int dex_offset, bool is_caught);
void add_menu_box(int options, int startTileX, int startTileY);
void add_menu_box(int startTileX, int startTileY, int width, int height);
void reload_textbox_background();
void load_select_sprites(GameBoyROM currROM);
void fennel_blink(int frame);
void fennel_speak(int frame);
int get_curr_flex_background();
void update_y_offset();
void erase_textbox_tiles();
void update_front_box_sprite(Pokemon *curr_pkmn, bool make_greyscale);
void update_menu_sprite(PokeBox* box, int index, int frame);
#endif