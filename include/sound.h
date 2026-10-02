#ifndef _SOUND_H
#define _SOUND_H

#include <tonc.h>

#include "soundbank.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned short PTGBSFXHandle;

/**
 * @brief The API's defined here are a thin abstraction layer over the sound engine functions.
 * This allows the underlying sound engine to be swapped out without affecting the rest of the codebase.
 */

/**
 * @brief This function initializes the sound engine and sets up a VBLANK handler
 * to process audio every frame. It should be called during the initialization phase of the program.
 */
bool sound_init(void);

/**
 * @brief This function starts playing the song at the given index. 
 * If loop is true, the song will loop indefinitely until stopped or another song is played.
 * 
 * @param song_index The index of the song to play.
 * @param loop Whether the song should loop indefinitely.
 */
void play_song(u32 song_index, bool loop);

/**
 * @brief This function checks if a song is currently playing.
 * 
 * @return true if a song is playing, false otherwise.
 */
bool is_song_playing(void);

/**
 * @brief This function stops the currently playing song.
 */
void stop_song(void);

/**
 * @brief This function plays the sound effect at the given index. 
 * Sound effects are typically short audio clips that play in response to specific events in the game, 
 * such as button presses or character actions.
 * These are usually raw PCM samples (e.g., WAV files)
 * 
 * To play a MOD- S3M- XM- or IT format based sound effect,
 * use the play_jingle function instead.
 */
PTGBSFXHandle play_sound_effect(u32 sound_effect_index);

/**
 * @brief This function stops the given sound effect from playing and invalidates the handle
 */
void stop_sound_effect(PTGBSFXHandle handle);

/**
 * @brief This function stops all currently playing sound effects and resets 
 */
void stop_all_sound_effects(void);

/**
 * @brief This function marks the sound effect as unimportant,
 * allowing the sound engine to stop it if it needs to free up channels for new sound effects.
 * 
 * It also invalidates the handle, so it should not be used after calling this function.
 */
void release_sound_effect(PTGBSFXHandle handle);

/**
 * @brief This function plays a jingle at the given index.
 * A jingle is a sound effect that is based on a tracker format like MOD, S3M, XM, or IT.
 * They can be played simultaneously with songs.
 * If you want to play PCM-based sound effects (e.g., WAV files), use the play_sound_effect function instead.
 * 
 * Note that jingles must be limited to 4 channels only. (https://blocksds.skylyrac.net/maxmod/group__gba__jingle__playback.html)
 */
void play_jingle(u32 jingle_index);

/**
 * @brief Checks if a jingle is actively playing.
 */
bool is_jingle_playing(void);

#ifdef __cplusplus
}
#endif

#endif