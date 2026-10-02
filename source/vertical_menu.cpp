#include "vertical_menu.h"
#include "sprite_data.h"
#include "global_frame_controller.h"
#include "text_engine.h"
#include "translated_text.h"
#include "text_tables.h"

#define TILE_HEIGHT 8
#define TILE_WIDTH 8

i_item_widget::~i_item_widget()
{
}

i_vertical_menu_state_changed_handler::~i_vertical_menu_state_changed_handler()
{
}

i_run_cycle_handler::~i_run_cycle_handler()
{
}

static unsigned get_num_visible_items(unsigned vertical_menu_height, unsigned margin_top, unsigned margin_bottom, unsigned item_height)
{
    return ((vertical_menu_height - margin_top - margin_bottom) / item_height);
}

static unsigned get_viewport_end_index(unsigned viewport_start_index, unsigned num_visible_items, unsigned total_items)
{
    unsigned end_index = viewport_start_index + num_visible_items;
    // bounds check to make sure we don't go past the total number of items
    end_index = std::min(end_index, total_items);

    return end_index;
}

vertical_menu::vertical_menu(const vertical_menu_settings &settings)
    : settings_(settings), state_changed_handler_(nullptr), run_cycle_handler_(nullptr), focused_index_(settings.initial_focus_index), viewport_start_index_(0), items_(), is_focused_(true)
{
}

vertical_menu::~vertical_menu()
{
    clear_item_widgets();
}

void vertical_menu::show()
{
    add_menu_box(settings_.x / TILE_WIDTH, settings_.y / TILE_HEIGHT, settings_.width, settings_.height);

    update_viewport();

    if (state_changed_handler_)
    {
        state_changed_handler_->on_show();
    }

    // initial selection change notification
    const unsigned render_index = focused_index_ - viewport_start_index_;
    handle_selection_change(focused_index_, settings_.x, settings_.y + settings_.margin_top + (render_index * settings_.item_height));
}

i_item_widget *vertical_menu::get_item_widget_at(unsigned index) const
{
    if (index >= items_.size())
    {
        return nullptr;
    }
    return items_[index];
}

void vertical_menu::hide()
{
    if (state_changed_handler_)
    {
        state_changed_handler_->on_hide();
    }

    clear_viewport();

    reload_textbox_background();
}

const vertical_menu_settings &vertical_menu::get_settings() const
{
    return settings_;
}

void vertical_menu::set_settings(const vertical_menu_settings &settings)
{
    settings_ = settings;
}

void vertical_menu::add_item_widgets(i_item_widget **item_widgets, unsigned num_widgets)
{
    items_.reserve(items_.size() + num_widgets);
    for (unsigned i = 0; i < num_widgets; ++i)
    {
        add_item_widget(item_widgets[i]);
    }
}

void vertical_menu::add_item_widget(i_item_widget *item_widget)
{
    items_.push_back(item_widget);
}

void vertical_menu::clear_item_widgets()
{
    if (settings_.should_delete_item_widgets_on_destruct)
    {
        for (unsigned i = 0; i < items_.size(); ++i)
        {
            delete items_[i];
        }
    }
    items_.clear();
    focused_index_ = settings_.initial_focus_index;
    viewport_start_index_ = 0;
}

void vertical_menu::set_focused(bool is_focused)
{
    is_focused_ = is_focused;
    if (state_changed_handler_ && settings_.should_hide_state_changed_handler_on_not_focused)
    {
        if (is_focused_)
        {
            state_changed_handler_->on_show();
        }
        else
        {
            state_changed_handler_->on_hide();
        }
    }
}

MenuInputHandleState vertical_menu::handle_input()
{
    MenuInputHandleState result;
    bool did_navigate = false;
    bool viewport_changed = false;

    // If no items or not focused, there's nothing to handle.
    if (items_.size() <= 1 || !is_focused_)
    {
        return MenuInputHandleState::NOT_HANDLED;
    }

    // the focused item widget gets the first chance to handle input,
    // since it might have some special behavior for certain keys.
    result = items_[focused_index_]->handle_input();
    if (result == MenuInputHandleState::HANDLED_UPDATE_VIEWPORT)
    {
        // the child widget is requesting that we update the viewport.
        //  so we do that here and then degrade the result to HANDLED,
        // since we've already done the viewport update that the child widget requested.
        update_viewport();
        result = MenuInputHandleState::HANDLED; // degrade to HANDLED after updating the viewport
    }
    if (result != MenuInputHandleState::NOT_HANDLED)
    {
        return result;
    }

    // if the user presses B, and the current settings allow it,
    // we will return the CANCELLED state, which the caller can use to know that they should exit the menu.
    if (settings_.allow_cancel && key_hit(KEY_B))
    {
        return MenuInputHandleState::CANCELLED;
    }

    if (key_hit(KEY_DOWN))
    {
        const unsigned num_visible_items = get_num_visible_items(settings_.height, settings_.margin_top, settings_.margin_bottom, settings_.item_height);
        const unsigned current_viewport_end_index = get_viewport_end_index(viewport_start_index_, num_visible_items, items_.size());
        ++focused_index_;
        if (focused_index_ == current_viewport_end_index)
        {
            ++viewport_start_index_;
            viewport_changed = true;
        }
        if (focused_index_ >= items_.size())
        {
            // reached the end, so wrap around to the beginning.
            viewport_start_index_ = 0;
            focused_index_ = 0;
            viewport_changed = true;
        }
        did_navigate = true;
    }
    else if (key_hit(KEY_UP))
    {
        if (focused_index_ == 0)
        {
            // currently the first item was selected, so do wrap around to the end.
            const unsigned num_visible_items = get_num_visible_items(settings_.height, settings_.margin_top, settings_.margin_bottom, settings_.item_height);
            const unsigned max_index = items_.size() - 1;
            
            viewport_start_index_ = (max_index - num_visible_items) + 1;
            focused_index_ = max_index;
            viewport_changed = true;
        }
        else
        {
            --focused_index_;
        }
        did_navigate = true;
    }

    // if we did navigate, we need to update the viewport and state changed handler.
    if (did_navigate)
    {
        const unsigned render_index = focused_index_ - viewport_start_index_;
        if (viewport_changed)
        {
            update_viewport();
        }
        handle_selection_change(focused_index_, settings_.x, settings_.y + settings_.margin_top + (render_index * settings_.item_height));
        return MenuInputHandleState::HANDLED;
    }

    return MenuInputHandleState::NOT_HANDLED;
}

void vertical_menu::update_viewport()
{
    uint8_t decompression_buffer[2048];
    const u8 **textTableChunkList;
    u32 textTableNumChunks;
    u32 textTableChunkSize;

    get_text_table_chunks(settings_.text_table_index, &textTableChunkList, &textTableNumChunks, &textTableChunkSize);
    FileContainerReader text_table(textTableChunkList, textTableNumChunks, textTableChunkSize);

    clear_viewport();

    if (settings_.text_table_index != INT32_MAX)
    {
        text_table.init(decompression_buffer, sizeof(decompression_buffer));
    }

    const unsigned num_visible_items = get_num_visible_items(settings_.height, settings_.margin_top, settings_.margin_bottom, settings_.item_height);
    const unsigned viewport_end_index = get_viewport_end_index(viewport_start_index_, num_visible_items, items_.size());
    unsigned cur_y = settings_.y + settings_.margin_top;

    for (unsigned i = viewport_start_index_; i < viewport_end_index; ++i)
    {
        items_[i]->render_item(text_table, settings_.x, cur_y, (i == focused_index_));
        cur_y += settings_.item_height;
    }
}

void vertical_menu::clear_viewport()
{
    // tte_erase_rect expects (left, top, right, bottom), not (x, y, width, height)
    tte_erase_rect(settings_.x, settings_.y, settings_.x + settings_.width, settings_.y + settings_.height);
}

unsigned vertical_menu::run()
{
    MenuInputHandleState input_result;

    VBlankIntrWait(); // Reset the buttons

    while (true)
    {
        input_result = handle_input();

        switch (input_result)
        {
        case MenuInputHandleState::CHOICE_MADE:
            return focused_index_;
        case MenuInputHandleState::CANCELLED:
            return UINT32_MAX;
        default:
            break;
        }

        // if any global elements need to be updated
        // on every cycle (animations, for example)
        // the run_cycle_handler can be used for that.
        if (run_cycle_handler_)
        {
            run_cycle_handler_->on_run_cycle();
        }

        VBlankIntrWait();
    }
    // should never happen
    return UINT32_MAX;
}

void vertical_menu::set_state_changed_handler(i_vertical_menu_state_changed_handler *handler)
{
    state_changed_handler_ = handler;
}

void vertical_menu::set_run_cycle_handler(i_run_cycle_handler *handler)
{
    run_cycle_handler_ = handler;
}

void vertical_menu::handle_selection_change(unsigned new_index, unsigned x, unsigned y)
{
    if (state_changed_handler_)
    {
        state_changed_handler_->on_selection_changed(new_index, x, y);
    }
}

simple_item_renderer::simple_item_renderer(const simple_item_widget_data &data)
    : data_(data)
{
}

simple_item_renderer::~simple_item_renderer()
{
}

const simple_item_widget_data &simple_item_renderer::get_data() const
{
    return data_;
}

void simple_item_renderer::render_item(FileContainerReader &text_table, unsigned x, unsigned y, bool is_focused)
{
    // text_helper/main.py restricts each line to 1024 bytes.
    u8 lineBuffer[1024];

    text_table.readFile(data_.text.text_table_index, lineBuffer);
    tte_set_pos(x + data_.text.margin_left, y + data_.text.margin_top);
    ptgb_write_simple(lineBuffer, true);
}

MenuInputHandleState simple_item_renderer::handle_input()
{
    if (!key_hit(KEY_A))
    {
        return MenuInputHandleState::NOT_HANDLED;
    }

    if (data_.on_execute_callback)
    {
        data_.on_execute_callback(this);
        return MenuInputHandleState::HANDLED;
    }
    return MenuInputHandleState::CHOICE_MADE;
}