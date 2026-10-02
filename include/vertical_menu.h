#ifndef _VERTICAL_MENU_H
#define _VERTICAL_MENU_H

#include "typeDefs.h"
#include "libstd_replacements.h"
#include "FileContainerReader.h"

enum class MenuInputHandleState
{
    /**
     * @brief This value means no input was handled
     */
    NOT_HANDLED,
    /**
     * @brief This value means that the input was handled.
     */
    HANDLED,
    /**
     * @brief Same as HANDLED, but also indicates that the viewport should be updated.
     * degrades to HANDLED after passing through vertical_menu::handle_input()
     */
    HANDLED_UPDATE_VIEWPORT,
    /**
     * @brief This value means that a choice was made.
     * This means that if the vertical_menu is running with ::run(), 
     * it should exit and return that choice.
     */
    CHOICE_MADE,
    /**
     * @brief The menu was cancelled.
     */
    CANCELLED
};

/**
 * @brief This interface represents an item widget in the vertical menu.
 * It's responsible for rendering itself and handling its input if
 * the widget is focused.
 */
class i_item_widget
{
public:
    virtual ~i_item_widget();
    virtual void render_item(FileContainerReader &text_table, unsigned x, unsigned y, bool is_focused) = 0;
    virtual MenuInputHandleState handle_input() = 0;
protected:
private:
};

/**
 * @brief This interface allows you to register a handler
 * to deal with selection changes.
 * This is useful for managing external components, such as a cursor.
 */
class i_vertical_menu_state_changed_handler
{
public:
    virtual ~i_vertical_menu_state_changed_handler();

    virtual void on_show() = 0;
    virtual void on_hide() = 0;
    virtual void on_selection_changed(unsigned new_index, unsigned x, unsigned y) = 0;
protected:
private:
};

/**
 * @brief If you are using the vertical_menu::run() function,
 * you may want to manage external components on very run cycle.
 * This interface allows you to do that by implementing the i_run_cycle_handler
 */
class i_run_cycle_handler
{
public:
    virtual ~i_run_cycle_handler();
    virtual void on_run_cycle() = 0;
protected:
private:
};

typedef struct vertical_menu_settings
{
    // bounds properties
    unsigned x;
    unsigned y;
    unsigned width;
    unsigned height;

    /**
     * @brief The top margin (in pixels) after which we start rendering the first menu item.
     */
    unsigned margin_top;

    /**
     * @brief The bottom margin (in pixels) after which we stop rendering the last menu item.
     */
    unsigned margin_bottom;

    /**
     * @brief The index of the first item to be focused when the menu is shown.
     */
    unsigned initial_focus_index;

    /**
     * @brief The height of each menu item in pixels. Will be used to position the items.
     */
    unsigned item_height;

    /**
     * @brief This index defines which text table to use for the menu entries.
     */
    int text_table_index;

    /**
     * @brief This boolean indicates whether the menu 
     * can be "cancelled with the B button."
     */
    bool allow_cancel;

    /**
     * @brief This boolean indicates whether the menu should take
     * ownership of its item widgets.
     * That means that if this boolean is true,
     * the destructor of vertical_menu will delete/free all of its 
     * item widgets.
     */
    bool should_delete_item_widgets_on_destruct;

    /**
     * @brief This boolean indicates whether the state changed handler (cursor?)
     * should be hidden when the menu is not focused.
     */
    bool should_hide_state_changed_handler_on_not_focused;
} vertical_menu_settings;

class vertical_menu
{
public:
    vertical_menu(const vertical_menu_settings &settings);
    ~vertical_menu();

    void show();
    void hide();

    /**
     * @brief Get the current settings of this widget
     */
    const vertical_menu_settings& get_settings() const;

    /**
     * @brief Replace the settings of this widget.
     */
    void set_settings(const vertical_menu_settings &settings);

    /**
     * @brief This function is a way to bulk add item widgets to the menu.
     * The main difference between calling this function and add_item_widget()
     * multiple times is that this function will reserve() enough space in the 
     * vector first, to avoid multiple resizes.
     */
    void add_item_widgets(i_item_widget **item_widgets, unsigned num_widgets);

    /**
     * @brief This function adds an item widget to the menu. 
     * The menu will take ownership of the pointer, if
     * settings_.should_delete_item_widgets_on_destruct is true. 
     * Otherwise, the caller is responsible for managing the memory of the item widgets.
     */
    void add_item_widget(i_item_widget *item_widget);

    /**
     * @brief This function clears all item widgets from the menu.
     * If settings_.should_delete_item_widgets_on_destruct is true,
     * it will also delete the item widgets.
     */
    void clear_item_widgets();

    /**
     * @brief This function gets the item widget
     * at the given index.
     */
    i_item_widget* get_item_widget_at(unsigned index) const;

    /**
     * @brief Sets whether the menu is focused or not.
     * A focused menu will handle input.
     */
    void set_focused(bool is_focused);

    /**
     * @brief This function handles button input for the menu.
     */
    MenuInputHandleState handle_input();

    /**
     * @brief This function updates the viewport by
     * re-rendering the visible items.
     */
    void update_viewport();

    void clear_viewport();

    /**
     * @brief This function can be used to manage a blocking call
     * to vertical_menu, in which you want vertical_menu
     * to handle everything.
     * 
     * This can be useful during one of those script_array commands.
     * Alternatively, you can just call handle_input() and manage the loop
     * manually.
     * 
     * @return Will return the index of the chosen item if a choice was made,
     * or UINT32_MAX if no choice was made and the menu was exited in some other way 
     * (ex: B button).
     */
    unsigned run();

    /**
     * @brief Sets the optional state changed handler.
     * This is useful for managing external components, such as a cursor.
     */
    void set_state_changed_handler(i_vertical_menu_state_changed_handler *handler);

    /**
     * @brief This function sets a handler that will run on every
     * cycle in the run() function.
     * 
     * It's only relevant if you use ::run() though.
     */
    void set_run_cycle_handler(i_run_cycle_handler *handler);
protected:
private:
    void handle_selection_change(unsigned new_index, unsigned x, unsigned y);

    vertical_menu_settings settings_;
    i_vertical_menu_state_changed_handler *state_changed_handler_;
    i_run_cycle_handler *run_cycle_handler_;
    unsigned focused_index_;
    unsigned viewport_start_index_;
    ptgb::vector<i_item_widget*> items_;
    bool is_focused_;
};

/**
 * @brief Metadata for the simple_item_renderer below.
 */
typedef struct simple_item_widget_data
{
    /**
     * @brief label text related properties.
     */
    struct
    {
        /**
         * @brief The index of the text table (see vertical_menu_settings) entry to use as the label of this item.
         */
        unsigned text_table_index;

        /**
         * @brief The left pixel offset after which we render the label text.
         */
        unsigned margin_left;
        /**
         * @brief The top pixel offset after which we render the label text.
         */
        unsigned margin_top;
    } text;

    /**
     * @brief The value this widget represents. This is useful for choice/input handling.
     * But it won't be used if an on_execute_callback is set.
     */
    unsigned value;

    /**
     * @brief Optional execute callback. This will be executed when the user presses A while this item is focused.
     * If this is nullptr, then MenuInputHandleState::CHOICE_MADE will be returned when the user presses A,
     * and the caller can check the value field to know what choice was made.
     */
    void (*on_execute_callback)(void *context);
} simple_item_widget_data;

/**
 * @brief This is the simplest possible item widget, which will just render a single line of text for a single item
 * and allow you to press A to execute a callback. It's useful for simple menus that don't require toggles or multiple options.
 */
class simple_item_renderer : public i_item_widget
{
public:
    simple_item_renderer(const simple_item_widget_data &data);
    virtual ~simple_item_renderer();

    const simple_item_widget_data& get_data() const;

    void render_item(FileContainerReader &text_table, unsigned x, unsigned y, bool is_focused) override;
    MenuInputHandleState handle_input() override;
protected:
private:
    simple_item_widget_data data_;
};

#endif