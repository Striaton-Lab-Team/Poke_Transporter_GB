#ifndef VERTICAL_MENU_CURSOR_HANDLER_H
#define VERTICAL_MENU_CURSOR_HANDLER_H

#include "vertical_menu.h"

/**
 * @brief This class handles the cursor behavior for a vertical menu.
 */
class vertical_menu_cursor_handler : public i_vertical_menu_state_changed_handler
{
public:
    vertical_menu_cursor_handler(s16 cursor_offset_x, s16 cursor_offset_y);
    virtual ~vertical_menu_cursor_handler();

    void on_show() override;
    void on_hide() override;
    void on_selection_changed(unsigned new_index, unsigned x, unsigned y) override;
protected:
private:
    const s16 cursor_offset_x_;
    const s16 cursor_offset_y_;
};

#endif