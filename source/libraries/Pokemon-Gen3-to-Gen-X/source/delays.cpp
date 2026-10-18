// Originally created by Lorenzooone and modified with their permission
// Source: https://github.com/Lorenzooone/Pokemon-Gen3-to-Gen-X

#include "libraries/Pokemon-Gen3-to-Gen-X/include/base_include.h"
#include <stddef.h>
#include "libraries/Pokemon-Gen3-to-Gen-X/include/useful_qualifiers.h"
#include "libraries/Pokemon-Gen3-to-Gen-X/include/timing_basic.h"
#include "libraries/Pokemon-Gen3-to-Gen-X/include/delays.h"

#define NUM_CYCLES_PER_ITER 4
#define NUM_CYCLES_PER_ITER_CHECK 8

NO_INLINE IWRAM_CODE void delay_cycles(u32 num_cycles) {
    num_cycles = (num_cycles + NUM_CYCLES_PER_ITER - 1 + (NUM_CYCLES_PER_ITER / 2)) / NUM_CYCLES_PER_ITER;
    __asm__ volatile (
        "L1%=: subs %[wait], #1;"
        "bne L1%=;"
        : [wait] "+l" (num_cycles)
    );
}

NO_INLINE IWRAM_CODE void delay_cycles_until(u32 num_cycles, vu8* data, u8 wanted, u8 cycles_per_load) {
    num_cycles = (num_cycles + (NUM_CYCLES_PER_ITER_CHECK + cycles_per_load) - 1 + ((NUM_CYCLES_PER_ITER_CHECK + cycles_per_load) / 2)) / (NUM_CYCLES_PER_ITER_CHECK + cycles_per_load);
    __asm__ volatile (
        "L1%=: ldrb r3, [%[data], #0];"
        "cmp r3, %[wanted];"
        "beq L2%=;"
        "subs %[wait], #1;"
        "bne L1%=;"
        "L2%=:;"
        : [wait] "+l" (num_cycles) : [data]"l"(data), [wanted]"l"(wanted) : "r3"
    );
}
