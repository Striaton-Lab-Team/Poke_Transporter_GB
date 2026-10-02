#include <tonc.h>
#include "pokemon_data.h"
#include "background_engine.h"
#include "translated_text.h"

#define CBB 0
#define SBB 24
int x, y;

void background_frame(int global_frame_count)
{
        if (!get_missingno_enabled())
        {
                x++;
                y++;

                REG_BG0HOFS = x;
                REG_BG0VOFS = y;
        }
}

// This could honestly be an object... might want to do that in the future, depending on how complex using this gets
void create_textbox(int text_section, int text_key, bool eraseMainBox)
{
        const u8 box_type = text_box_type_tables[text_section][text_key];
        create_textbox(box_type, eraseMainBox);
}

void create_textbox(u8 textbox_type, bool eraseMainBox)
{
        const int startTileX = box_type_info[textbox_type][BOX_TYPE_VAL_START_TILE_X];
        const int startTileY = box_type_info[textbox_type][BOX_TYPE_VAL_START_TILE_Y];
        const int text_space_width = box_type_info[textbox_type][BOX_TYPE_VAL_PIXELS_PER_LINE];
        const int text_space_height = box_type_info[textbox_type][BOX_TYPE_VAL_NUM_OF_LINES] * 16;

        create_textbox(startTileX, startTileY, text_space_width, text_space_height, eraseMainBox);
}

void create_textbox(int startTileX, int startTileY, int text_space_width, int text_space_height, bool eraseMainBox)
{
        if (eraseMainBox)
        {
                erase_textbox_tiles();
        }
        add_menu_box(startTileX, startTileY, text_space_width + 16, text_space_height + 16);
        tte_set_pos((startTileX + 1) * 8, (startTileY + 1) * 8);
        tte_set_margins((startTileX + 1) * 8, (startTileY + 1) * 8,
                        ((startTileX + 1) * 8) + text_space_width, ((startTileY + 1) * 8) + text_space_height);
        tte_erase_rect(0, 0, H_MAX, V_MAX);
        show_textbox();
}

void show_textbox()
{
        BG_TEXTBOX = (BG_TEXTBOX & ~BG_PRIO_MASK) | BG_PRIO(1);
}

void hide_textbox()
{
        BG_TEXTBOX = (BG_TEXTBOX & ~BG_PRIO_MASK) | BG_PRIO(3);
}