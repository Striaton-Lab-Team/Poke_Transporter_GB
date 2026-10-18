// SPDX-License-Identifier: LGPL-3.0-only
//
// Copyright (c) 2020 Antonio Niño Díaz

#include <stdint.h>

#define MEM_IO_SIZE             (0x00000400)

uint64_t internal_io[MEM_IO_SIZE / sizeof(uint64_t)];

uintptr_t UGBA_MemIO(void)
{
    return (uintptr_t)(&internal_io[0]);
}
