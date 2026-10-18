// Originally created by Lorenzooone and modified with their permission
// Source: https://github.com/Lorenzooone/Pokemon-Gen3-to-Gen-X

#ifndef DELAYS__
#define DELAYS__

#define CLOCK_CYCLES_PER_MS(x) ((((x) * ((uint64_t)CLOCK_SPEED)) + 999) / 1000)
#define CLOCK_CYCLES_PER_MUS(x) ((((x) * ((uint64_t)CLOCK_SPEED)) + 999999) / 1000000)

void delay_cycles(u32);
void delay_cycles_until(u32, vu8*, u8, u8);

#endif
