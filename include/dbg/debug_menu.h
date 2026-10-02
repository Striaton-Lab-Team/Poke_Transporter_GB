#ifndef _DEBUG_MENU_H
#define _DEBUG_MENU_H

#include "vertical_menu.h"
#include "debug_menu_entries.h"

#define MAX_MENU_LAYERS 3

// This file contains the implementation of the debug menu.
// But it doesn't actually define the entries or callback functions.
// For the menu entries, check debug_menu_entries.cpp
// For the callback functions, check debug_menu_functions.cpp

typedef void (*on_execute_callback)(void *context, unsigned user_param);

/**
 * @brief This struct represents the metadata associated to a single toggle option of a single row in the debug menu.
 */
typedef struct option_data
{
    const char *text;
    unsigned value;
} option_data;

/**
 * @brief This struct represents a single row in the debug menu. Each row has a label and a set of options.
 * 
 */
typedef struct debug_menu_row_data
{
    /**
     * @brief a charset with which to render the text in this row.
     */
    const u16 *charset;
    /**
     * @brief The text to be shown. This is just a plain old C string,
     * because we don't need translations in the debug menu.
     */
    const char *labelText;
    /**
     * @brief Toggle options below (optional):
     */
    struct {
        /**
         * @brief Number of options. If this is 0, then this row will just be a label with no options to select.
         */
        u8 num_options;

        /**
         * @brief The actual array of options alongside their values.
         * Note: if you want the widget to free() the array when the widget is getting destroyed,
         * set the should_free_on_destruct flag.
         */
        const option_data *options;

        /**
         * @brief This callback will be fired when the user selects an option. (just with LEFT/RIGHT)
         */
        on_execute_callback on_option_activate;

        /**
         * @brief The initial selected option index.
         * But this field will get maintained by the widget to track the current selected index.
         */
        u8 selected_option_index;

        /**
         * @brief This flag indicates whether the widget should call free() on the options pointer when the widget is getting destroyed.
         */
        bool should_free_on_destruct;
    } option_section;

    /**
     * @brief A callback that you can set to execute when the user selects this row and presses the A button. (optional)
     */
    on_execute_callback on_execute;

    /**
     * @brief Context for the on_option-activate and on_execute callbacks. (optional)
     */
    void *context;

    /**
     * @brief This user_param will be passed as an argument to the on_execute callback. (optional)
     */
    unsigned user_param;
} debug_menu_row_data;

/**
 * @brief This class is responsible for rendering and handling input for a single row in the vertical_menu based
 * debug menu.
 */
class debug_menu_row_widget : public i_item_widget
{
public:
    debug_menu_row_widget(const debug_menu_row_data &data);
    virtual ~debug_menu_row_widget();
    void render_item(FileContainerReader &text_table, unsigned x, unsigned y, bool is_focused) override;
    MenuInputHandleState handle_input() override;
protected:
private:
    debug_menu_row_data data_;
};

void show_debug_menu();
void push_debug_menu_section(DebugMenuSection section);

#endif