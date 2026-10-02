#include "vertical_menu_cursor_handler.h"
#include "sprite_data.h"

vertical_menu_cursor_handler::vertical_menu_cursor_handler(s16 cursor_offset_x, s16 cursor_offset_y)
    : cursor_offset_x_(cursor_offset_x)
    , cursor_offset_y_(cursor_offset_y)
{
}

vertical_menu_cursor_handler::~vertical_menu_cursor_handler()
{
}

void vertical_menu_cursor_handler::on_show()
{
    obj_unhide(point_arrow, 0);
}

void vertical_menu_cursor_handler::on_hide()
{
    obj_hide(point_arrow);
}

void vertical_menu_cursor_handler::on_selection_changed(unsigned new_index, unsigned x, unsigned y)
{
    // set the cursor pos accordingly
    const int cursor_x = static_cast<int>(x) + cursor_offset_x_;
    const int cursor_y = static_cast<int>(y) + cursor_offset_y_;

    obj_set_pos(point_arrow, cursor_x, cursor_y);
}