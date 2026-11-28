#ifndef MSX_GAME_AUDIO_H
#define MSX_GAME_AUDIO_H
#include <stdbool.h>
#include <stdint.h>
#include "gameplay.h"
/* Mission 1 gameplay audio (docs/AUDIO_JUEGO.md): original sound effects
 * on three effect voices with the measured channel policy (msx_sfx.c) and
 * the Mission 1 music on the music voice, looping at its measured point.
 * Requires msx_audio_init(). The assets (rom:/audio/NAME.wav64) are rendered
 * locally from the user's ROM; without them the game runs silently and logs
 * it once. */
bool game_audio_init(void);
/* Queue the resident effects for a background read (msx_ramfs_preload) in
 * the boot cinematics' spare time: game_audio_init then finds them in RAM
 * instead of reading ~1 MB at the mission handoff. */
void game_audio_preload(void);
/* Start (or restart) the mission music from its beginning. */
void game_audio_music(bool play);
/* A stage starts: its descriptor's music word (+C, 018434); a different
 * word than the playing one restarts the music voice with that music. */
void game_audio_stage_music(uint16_t word);
void game_audio_mission_start(void);
/* Forget previous gameplay state after a reset/handoff (no sounds). */
void game_audio_sync(msx_game_t *game);
/* Once per video frame after the simulation: drain the world's sound
 * requests and turn gameplay changes into the original requests. */
void game_audio_update(msx_game_t *game);
#endif
