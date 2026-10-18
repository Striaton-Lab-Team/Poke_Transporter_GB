// Originally created by Lorenzooone and modified with their permission
// Source: https://github.com/Lorenzooone/Pokemon-Gen3-to-Gen-X

#ifndef BASE_INCLUDE__
#define BASE_INCLUDE__

// GBA defines and all
#include <gba.h>
#include "useful_qualifiers.h"
#define SCANLINES 0xE4
#define ROM 0x8000000
ALWAYS_INLINE MAX_OPTIMIZE void __set_next_vcount_interrupt_gba(int scanline) {
    REG_DISPSTAT  = (REG_DISPSTAT & 0xFF) | (scanline<<8);
}
ALWAYS_INLINE MAX_OPTIMIZE int __get_next_vcount_interrupt(void) {
    u16 reg_val = REG_DISPSTAT;
    return reg_val >> 8;
}
#define __set_next_vcount_interrupt(x) __set_next_vcount_interrupt_gba(x)
#define SCANLINE_IRQ_BIT LCDC_VCNT
#define REG_WAITCNT *(vu16*)(REG_BASE + 0x204) // Wait state Control
#define SRAM_READING_VALID_WAITCYCLES 3
#define SRAM_ACCESS_CYCLES 9
#define NON_SRAM_MASK 0xFFFC
#define BASE_WAITCNT_VAL 0x4314
#define OVRAM_START ((uintptr_t)OBJ_BASE_ADR)
#define TILE_1D_MAP 0
#define ACTIVATE_SCREEN_HW 0
#define EWRAM_SIZE 0x0003FF40
#define DIV_SWI_VAL "0x06"
#define VRAM_0 VRAM
#define HAS_SIO
#define CLOCK_SPEED 16777216
#define SAME_ON_BOTH_SCREENS 0
#define CONSOLE_LETTER 'G'
#define VBLANK_SCANLINES SCREEN_HEIGHT

#endif
