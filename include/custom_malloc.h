#ifndef CUSTOM_MALLOC_H
#define CUSTOM_MALLOC_H

#include "dbg/debug_mode.h"

#ifdef __cplusplus
extern "C" {
#endif
#include <stdint.h>
#include <stddef.h>

/// malloc.c defines a custom bitmap allocator implementation that replaces newlibs'
/// if the USE_CUSTOM_MALLOC flag is set. That makes sure the linker won't pull in all the malloc related code
/// in newlibc (alongside the 1KB IWRAM __malloc_av variable)

void malloc_init_default_pool(void);

#ifdef __cplusplus
}
#endif

#endif