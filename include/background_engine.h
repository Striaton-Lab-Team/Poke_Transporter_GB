#ifndef BG_ENGINE_H
#define BG_ENGINE_H

#include "sprite_data.h"
#include "text_engine.h"
#include "global_frame_controller.h"

// This order does matter, as if two backgrounds are on the same layer,
// the lowest number will be the one that is displayed
#define BG_BACKDROP REG_BG0CNT
#define BG_FLEX REG_BG1CNT
#define BG_TEXTBOX REG_BG2CNT
#define BG_TEXT REG_BG3CNT

void background_frame(int global_frame_count);

/**
 * @brief This function draws the textbox (=copies the tiles) for a specific text table and text key.
 * Each text table entry could have a different type of textbox associated with it.
 * This function will look up the textbox type for the given text table and text key, and then draw the textbox accordingly.
 */
void create_textbox(int text_section, int text_key, bool eraseMainBox);

/**
 * @brief Given the textbox type, this function draws it (=copies the tiles).
 */
void create_textbox(u8 textbox_type, bool eraseMainBox);

/**
 * @brief This function draws a textbox at the specified tile coordinates, with the specified text space dimensions. 
 * This is the most low-level version of the create_textbox function, and it doesn't do any lookup for textbox types. 
 * It just draws a textbox with the given parameters.
 */
void create_textbox(int startTileX, int startTileY, int text_space_width, int text_space_height, bool eraseMainBox);
void show_textbox();
void hide_textbox();
#endif