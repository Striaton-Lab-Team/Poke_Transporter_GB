#ifndef _DEBUG_MENU_ENTRIES_H
#define _DEBUG_MENU_ENTRIES_H

#include "typeDefs.h"

class vertical_menu;

enum class DebugMenuSection
{
    MAIN,
    INJECTION
};

/**
 * @brief This function pointer is used to define a function filling a specifi DebugMenuSection
 */
typedef void (*fill_menu_entries_func)(vertical_menu &menu, u16 *charset);

typedef struct MenuSectionMapEntry
{
    DebugMenuSection section;
    fill_menu_entries_func fill_func;
} MenuSectionMapEntry;

/**
 * @brief This function constructs the actual debug menu entries and fills the provided menu with them.
 */
void fill_debug_menu_with_entries(vertical_menu &menu, u16 *charset, DebugMenuSection section);

#endif