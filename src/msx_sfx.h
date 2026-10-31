#ifndef MSX_SFX_H
#define MSX_SFX_H
#include <stdbool.h>
#include <stdint.h>
/* Gameplay sound effects: original 68000 sound request words (ROM 0120F8)
 * mapped to isolated renders of the original driver output, and the voice
 * policy measured on that driver (docs/AUDIO_JUEGO.md). Pure C: no
 * libdragon, no ROM-derived data; the runtime glue lives in game_audio.c.
 *
 * Policy (pair renders of tools/render_sound_commands.py, 0.2 s apart):
 * every effect belongs to one driver channel; a new request on a busy
 * channel replaces the playing sound when its priority is >= the playing
 * one (same sound = restart from the beginning) and is dropped otherwise.
 * Effects on different channels overlap freely. */
/* The rest of the effects of the Mission 1 world, with their channels and
 * levels, come from the pair measurement of tools/measure_sound_pairs.py
 * (data/sound_policy.json -> tools/compile_sound_policy.py). */
#include "msx_sfx_policy.h"
enum { MSX_SFX_CH_WEAPON, MSX_SFX_CH_IMPACT, MSX_SFX_CH_VOICE, MSX_SFX_POLICY_CHANNELS, MSX_SFX_CHANNELS };
typedef enum {
    MSX_SFX_PISTOL, MSX_SFX_VEHICLE_HIT,
    MSX_SFX_EXPLOSION, MSX_SFX_KNIFE_HIT, MSX_SFX_VEHICLE_SMOKE, MSX_SFX_VEHICLE_BURST,
    MSX_SFX_REBEL_103E, MSX_SFX_REBEL_103F, MSX_SFX_REBEL_1040, MSX_SFX_REBEL_1041,
    MSX_SFX_REBEL_1042, MSX_SFX_REBEL_1043,
    MSX_SFX_POLICY_IDS,
    MSX_SFX_COUNT
} msx_sfx_id_t;
typedef struct {
    uint16_t word;        /* original request word */
    uint8_t channel,priority;
    const char *name;     /* rom:/audio/<name>.wav64 */
} msx_sfx_def_t;
extern const msx_sfx_def_t msx_sfx_defs[MSX_SFX_COUNT];
/* Index of an original request word, or -1 when it has no packed sound. */
int msx_sfx_lookup(uint16_t word);
typedef struct {
    int8_t current[MSX_SFX_CHANNELS];   /* sound on each channel, -1 = idle */
    unsigned started,replaced,dropped,unmapped;
} msx_sfx_state_t;
void msx_sfx_reset(msx_sfx_state_t *state);
/* Mark a channel idle (its sound finished). */
void msx_sfx_finished(msx_sfx_state_t *state,unsigned channel);
/* Apply the policy to one request. Returns the channel the sound must start
 * on (the caller restarts that voice) or -1 when it is dropped/unmapped. */
int msx_sfx_request(msx_sfx_state_t *state,uint16_t word);
#endif
