#include <tonc.h>
#include "button_menu.h"
#include "button_handler.h"
#include "ptgb_save_data_manager.h"
#include "global_frame_controller.h"
#include "string"

Button_Menu::Button_Menu(int nRows, int nColumns, int nButton_width, int nButton_height, bool enable_cancel)
{
    columns = nColumns;
    rows = nRows;
    button_height = nButton_height;
    button_width = nButton_width;
    curr_position = 0;
    x_min = 0;
    x_max = 240;
    y_min = 0;
    y_max = 160;
    bottom_offset = 0;
    cancel_enabled = enable_cancel;
}

void Button_Menu::set_xy_min_max(int nX_min, int nX_max, int nY_min, int nY_max)
{
    x_min = nX_min;
    x_max = nX_max;
    y_min = nY_min;
    y_max = nY_max;
}

int Button_Menu::button_main()
{
    organize_buttons();
    show_buttons();
    button_vector.at(curr_position).set_highlight(true);
    int curr_x = 0;
    int curr_y = 0;

    VBlankIntrWait(); // Reset the buttons

    while (true)
    {
        curr_x = get_x_from_pos(curr_position);
        curr_y = get_y_from_pos(curr_position);

        if (key_hit(KEY_RIGHT) && (curr_x < (columns - 1)))
        {
            curr_x++;
            if (get_pos_from_xy(curr_x, curr_y) >= button_vector.size())
            {
                curr_y--;
                curr_x += offset_value / 2;
            }
        }
        else if (key_hit(KEY_DOWN) && (curr_y < (rows - 1)))
        {
            curr_y++;
            if (get_pos_from_xy(curr_x, curr_y) >= button_vector.size())
            {
                curr_x = get_x_from_pos(button_vector.size() - 1);
            }
        }
        else if (key_hit(KEY_LEFT) && ((curr_x > 0) || ((curr_x == 0) && (curr_y == rows - 1) && (offset_value > 0))))
        {
            if (curr_x == 0 && offset_value > 0)
            {
                curr_y -= 1;
            }
            else
            {
                curr_x--;
            }
        }
        else if (key_hit(KEY_UP) && (curr_y > 0))
        {
            if (curr_y == rows - 1)
            {
                curr_x += offset_value / 2;
            }
            curr_y--;
        }
        else if (key_hit(KEY_A))
        {
            hide_buttons();
            return return_values.at(curr_position);
        }
        else if (cancel_enabled && key_hit(KEY_B))
        {
            hide_buttons();
            return BUTTON_CANCEL;
        }

        if (get_pos_from_xy(curr_x, curr_y) != curr_position)
        {
            button_vector.at(curr_position).set_highlight(false);
            curr_position = get_pos_from_xy(curr_x, curr_y);
            button_vector.at(curr_position).set_highlight(true);
        }
        VBlankIntrWait();
    }
    return 0;
}

void Button_Menu::add_button(Button btn, int return_val)
{
    button_vector.push_back(btn);
    return_values.push_back(return_val);
}

void Button_Menu::show_buttons()
{
    for (unsigned int i = 0; i < button_vector.size(); i++)
    {
        button_vector.at(i).show();
    }
}

void Button_Menu::hide_buttons()
{
    for(size_t i=0; i < button_vector.size(); ++i)
    {
        button_vector[i].hide();
    }
}

void Button_Menu::organize_buttons()
{
    // Total space, minus the space taken up by the buttons, divided by the spaces between the buttons.
    int vertical_space =
        ((y_max - y_min) - (rows * button_height)) / (rows + 1);
    int horizontal_space =
        ((x_max - x_min) - (columns * button_width)) / (columns + 1);
    bottom_offset = ((horizontal_space + button_width) * ((rows * columns) - button_vector.size()) / 2);
    offset_value = (rows * columns) - button_vector.size();
    for (unsigned int i = 0; i < button_vector.size(); i++)
    {
        button_vector.at(i).set_location(
            ((horizontal_space + button_width) * get_x_from_pos(i)) + horizontal_space + x_min +
                (get_y_from_pos(i) == (unsigned int)(rows - 1) ? bottom_offset : 0),
            ((vertical_space + button_height) * get_y_from_pos(i)) + vertical_space + y_min);
    }
}

unsigned int Button_Menu::get_pos_from_xy(int nX, int nY)
{
    return (nY * columns) + nX;
}

unsigned int Button_Menu::get_x_from_pos(int nPos)
{
    return (nPos % columns);
}

unsigned int Button_Menu::get_y_from_pos(int nPos)
{
    return (nPos / columns);
}

void Button_Menu::set_rows_and_columns(int nRows, int nColumns)
{
    rows = nRows;
    columns = nColumns;
}

void Button_Menu::set_bottom_row_offset(int nBottom_offset)
{
    bottom_offset = nBottom_offset;
}

void Button_Menu::clear_vector()
{
    button_vector.clear();
}