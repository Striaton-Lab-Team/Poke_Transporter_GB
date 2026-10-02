#include "select_menu.h"
#include "sprite_data.h"
#include "translated_text.h"
#include "global_frame_controller.h"

#define TEXT_HEIGHT 10
#define TEXT_WIDTH 8
#define TILE_HEIGHT 8
#define TILE_WIDTH 8

Select_Menu::Select_Menu(bool enable_cancel, u8 nMenu_type, unsigned nStartTileX, unsigned nStartTileY)
    : vertical_menu_cursor_handler(TEXT_WIDTH, 3)
    , menu_widget_(vertical_menu_settings{
        .x = static_cast<unsigned>(nStartTileX * TILE_WIDTH),
        .y = static_cast<unsigned>(nStartTileY * TILE_HEIGHT),
        .width = 10 * TEXT_WIDTH,
        .height = TILE_HEIGHT * 2, // to account for the margins
        .margin_top = TILE_HEIGHT,
        .margin_bottom = TILE_HEIGHT,
        .initial_focus_index = 0,
        .item_height = TEXT_HEIGHT,
        .text_table_index = GENERAL_INDEX,
        .allow_cancel = enable_cancel,
        .should_delete_item_widgets_on_destruct = true,
        .should_hide_state_changed_handler_on_not_focused = false
    })
    , menu_type(nMenu_type)
    , lang(0)
{
    menu_widget_.set_state_changed_handler(this);
    menu_widget_.set_run_cycle_handler(this);
}

void Select_Menu::add_option(const u8 option, u8 return_value)
{
    const simple_item_widget_data item_data = {
        .text = {
            .text_table_index = option,
            .margin_left = 2 * TILE_WIDTH,
            .margin_top = 0
        },
        .value = return_value,
        .on_execute_callback = nullptr
    };
    menu_widget_.add_item_widget(new simple_item_renderer(item_data));

    vertical_menu_settings settings = menu_widget_.get_settings();
    settings.height += TEXT_HEIGHT + (item_data.text.margin_top * 2);
    menu_widget_.set_settings(settings);
}

void Select_Menu::clear_options()
{
    menu_widget_.clear_item_widgets();

    vertical_menu_settings settings = menu_widget_.get_settings();
    settings.height = TILE_HEIGHT * 3; // reset to just the margins
    menu_widget_.set_settings(settings);
}

int Select_Menu::select_menu_main()
{
    unsigned choice_index;
    unsigned item_value;
    simple_item_renderer* widget = nullptr;

    menu_widget_.show();
    choice_index = menu_widget_.run();

    if(choice_index != UINT32_MAX)
    {
        widget = static_cast<simple_item_renderer*>(menu_widget_.get_item_widget_at(choice_index));
    }
    item_value = (widget) ? widget->get_data().value : UINT8_MAX;

    menu_widget_.hide();

    //global_next_frame();
    return item_value;
}

void Select_Menu::set_lang(u8 nLang)
{
    lang = nLang;
}

void Select_Menu::on_hide()
{
    // call the base implementation to hide the cursor
    vertical_menu_cursor_handler::on_hide();
    obj_hide(cart_shell);
    obj_hide(cart_label);
    obj_hide(gb_flag);
}

void Select_Menu::on_selection_changed(unsigned new_index, unsigned x, unsigned y)
{
    // call the base implementation to move the cursor
    vertical_menu_cursor_handler::on_selection_changed(new_index, x, y);

    simple_item_renderer* widget = static_cast<simple_item_renderer*>(menu_widget_.get_item_widget_at(new_index));
    const unsigned item_value = (widget) ? widget->get_data().value : UINT8_MAX;

    if (item_value == UINT8_MAX)
    {
        switch (menu_type)
        {
        case CART_MENU:
            obj_hide(cart_shell);
            obj_hide(cart_label);
            break;
        case LANG_MENU:
            obj_hide(gb_flag);
            break;
        }
    }
    else
    {
        switch (menu_type)
        {
        case CART_MENU:
            //load_select_sprites(item_value, lang);
            obj_unhide(cart_shell, 0);
            obj_unhide(cart_label, 0);
            break;
        case LANG_MENU:
            //load_select_sprites(0, item_value);
            obj_unhide(gb_flag, 0);
            break;
        }
    }
}

void Select_Menu::on_run_cycle()
{
    update_y_offset();
}