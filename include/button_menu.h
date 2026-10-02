#ifndef MAIN_MENU_H
#define MAIN_MENU_H

#include <tonc.h>
#include "libstd_replacements.h"

#include "button_handler.h"

#define BTN_MAIN_MENU 0
#define BTN_TRANSFER 1
#define BTN_POKEDEX 2
#define BTN_EVENTS 3
#define BTN_CREDITS 4
#define BTN_OPENING 5
#define BTN_DEBUG_MENU 6
#define BTN_TUTORIAL 7

#define BUTTON_CANCEL UINT8_MAX

class Button_Menu
{
public:
    Button_Menu(int nRows, int nColumns, int nButton_width, int nButton_height, bool enable_cancel);
    int button_main();
    void add_button(Button btn, int return_val);
    void hide_buttons();
    void show_buttons();
    void organize_buttons();
    unsigned int get_pos_from_xy(int nX, int nY);
    unsigned int get_x_from_pos(int nPos);
    unsigned int get_y_from_pos(int nPos);
    void set_xy_min_max(int nX_min, int nX_max, int nY_min, int nY_max);
    void set_rows_and_columns(int nRows, int nColumns);
    void set_bottom_row_offset(int nBottom_offset);
    void clear_vector();

private:
    ptgb::vector<Button> button_vector;
    ptgb::vector<int> return_values;
    int columns;
    int rows;
    int button_height;
    int button_width;
    unsigned int curr_position;
    int x_min;
    int x_max;
    int y_min;
    int y_max;
    int offset_value;
    int bottom_offset;
    bool cancel_enabled;
};

extern Button_Menu yes_no_menu;

#endif