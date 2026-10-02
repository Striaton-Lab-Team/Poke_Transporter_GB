#include <maxmod.h>
#include "sound.h"
#include "soundbank_bin.h"

#if ENABLE_SOUND
static void sound_irq_handler(void)
{
    #ifdef INCLUDE_SOUND
    mmVBlank();
    mmFrame();
    #endif
}
#endif

bool sound_init(void)
{
#if ENABLE_SOUND
    irq_add(II_VBLANK, sound_irq_handler);

    mm_addr soundbank = (mm_addr)soundbank_bin;
    if (!soundbank)
    {
        return false;
    }

    mmInitDefault(soundbank, 16);
#endif
    return true;
}

void play_song(u32 song_index, bool loop)
{
#if ENABLE_SOUND
    mmStart(song_index, loop ? MM_PLAY_LOOP : MM_PLAY_ONCE);
#endif
}

bool is_song_playing(void)
{
#if ENABLE_SOUND
    return mmActive();
#else
    return false;
#endif
}

void stop_song(void)
{
#if ENABLE_SOUND
    mmStop();
#endif
}

PTGBSFXHandle play_sound_effect(u32 sound_effect_index)
{
#if ENABLE_SOUND
    return (PTGBSFXHandle)mmEffect(sound_effect_index);
#else
    return 0;
#endif
}

void stop_sound_effect(PTGBSFXHandle handle)
{
#if ENABLE_SOUND
    mmEffectCancel((mm_sfxhand)handle);
#endif
}

void stop_all_sound_effects(void)
{
#if ENABLE_SOUND
    mmEffectCancelAll();
#endif
}

void release_sound_effect(PTGBSFXHandle handle)
{
#if ENABLE_SOUND
    mmEffectRelease((mm_sfxhand)handle);
#endif
}

void play_jingle(u32 jingle_index)
{
#if ENABLE_SOUND
    mmJingle(jingle_index);
#endif
}

bool is_jingle_playing(void)
{
#if ENABLE_SOUND
    return mmActiveSub();
#else
    return false;
#endif
}