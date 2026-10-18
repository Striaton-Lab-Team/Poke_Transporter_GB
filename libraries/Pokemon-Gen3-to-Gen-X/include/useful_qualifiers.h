// Originally created by Lorenzooone and modified with their permission
// Source: https://github.com/Lorenzooone/Pokemon-Gen3-to-Gen-X

#ifndef USEFUL_QUALIFIERS__
#define USEFUL_QUALIFIERS__

#define UNUSED(x) UNUSED_ ## x __attribute__((__unused__))
#define ALWAYS_INLINE __attribute__((always_inline)) static inline
#define NO_INLINE __attribute__((noinline))
#define MAX_OPTIMIZE __attribute__((optimize(3)))
#ifndef PACKED
#define PACKED __attribute__((packed))
#endif
#ifndef IWRAM_CODE
#define IWRAM_CODE 
#endif
#define ARM_TARGET __attribute__((target("arm"),noinline))
#define ALIGNED(x) __attribute__((aligned(x)))

#endif
