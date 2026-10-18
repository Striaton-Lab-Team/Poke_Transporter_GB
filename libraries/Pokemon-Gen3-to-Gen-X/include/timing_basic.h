// Originally created by Lorenzooone and modified with their permission
// Source: https://github.com/Lorenzooone/Pokemon-Gen3-to-Gen-X

#ifndef TIMING_BASIC__
#define TIMING_BASIC__

#define FPS 60
#define NS_PER_SECOND 1000000000
#define NS_PER_CYCLE ((NS_PER_SECOND + CLOCK_SPEED - 1)/CLOCK_SPEED)
#define NS_PER_SCANLINE ((NS_PER_SECOND + (FPS*SCANLINES) - 1)/(FPS*SCANLINES))
#define CYCLES_PER_SCANLINE ((NS_PER_SCANLINE + NS_PER_CYCLE - 1)/NS_PER_CYCLE)

#define GBA_CLOCK_SPEED 16777216

#endif
