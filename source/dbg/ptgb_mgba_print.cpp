#include "dbg/ptgb_mgba_print.h"
#include "dbg/debug_mode.h"
#include <stdarg.h>

#if DEBUG_USE_MGBA_PRINT
#include "libraries/libmgba/mgba.h"

#if !DEBUG
static_assert(false, "DEBUG_USE_MGBA_PRINT is only allowed for debug builds!");
#endif

void ptgb_mgba_init(void)
{
    mgba_console_open();
}

void ptgb_mgba_deinit(void)
{
    mgba_close();
}

void ptgb_mgba_print(int level, const char *format_str, ...)
{
    va_list args;
    va_start(args, format_str);
    mgba_vprintf(level, format_str, args);
    va_end(args);
}

#else
void ptgb_mgba_init(void)
{
}

void ptgb_mgba_deinit(void)
{
}

void ptgb_mgba_print(int level, const char *format_str, ...)
{
    (void)level;
    (void)format_str;
}
#endif