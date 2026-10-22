#include "random.h"
#include <tonc.h>

unsigned int u32_rand;

void rand_set_seed(unsigned int init_seed) { u32_rand = init_seed + 1; }

unsigned int rand_get_seed() { return u32_rand; }

void rand_next_frame()
{
    u32_rand = (0x41C64E6D * u32_rand) + 0x6073; // This is the randomization method used in Gen 3
}

u32 get_rand_u32()
{
    rand_next_frame();
    return u32_rand;
}

u16 get_rand_u16()
{
    rand_next_frame();
    return (u32_rand >> 16);
}

u32 get_rev_rand_u32(u32 input) { return (input - 24691) * 4005161829; }

u16 get_rev_rand_u16(u32 input) { return (get_rev_rand_u32(input) >> 16); }

// Force ARM mode. In Thumb mode this imports __aeabi_lmul.
__attribute__((target("arm"), noinline)) static unsigned multiply_high(unsigned x, unsigned y)
{
    return ((unsigned long long)x * y) >> 32; // umull
}

unsigned get_rand_range(unsigned inc_min, unsigned exc_max)
{
    // fixed point: get_rand_u32() / (float)0x100000000 * (exc_max - inc_min) + inc_min
    return multiply_high(get_rand_u32(), exc_max - inc_min) + inc_min;
}