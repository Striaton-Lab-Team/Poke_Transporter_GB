#ifndef SELECT_MENU_H
#define SELECT_MENU_H

#include "vertical_menu.h"
#include "vertical_menu_cursor_handler.h"

#define LANG_MENU 1
#define CART_MENU 2

class Select_Menu : public vertical_menu_cursor_handler, public i_run_cycle_handler
{
public:
    Select_Menu(bool enable_cancel, u8 nMenu_type, unsigned nStartX, unsigned nStartY);
    int select_menu_main();
    void add_option(const u8 option, u8 return_value);
    void clear_options();
    void set_lang(u8 nLang);

    void on_hide() override;
    void on_selection_changed(unsigned new_index, unsigned x, unsigned y) override;

    void on_run_cycle() override;
private:
    vertical_menu menu_widget_;
    u8 menu_type;
    u8 lang;
};

#endif