#ifndef MSX_SFX_H
#define MSX_SFX_H
#include <stdbool.h>
#include <stdint.h>
/* Gameplay sound effects: original 68000 sound request words (ROM 0120F8)
 * mapped to isolated renders of the original driver output, and the rule of
 * the original Z80 sound driver (250-m1.m1, docs/AUDIO_JUEGO.md 19). Pure C:
 * no libdragon; the runtime glue lives in game_audio.c.
 *
 * Each effect's header (506F) gives its group D, the priority byte E of its
 * track and a stop list (src/msx_sfx_policy.h, tools/compile_sound_policy.py):
 *  - 5204: an effect first stops the playing effects of its stop list;
 *  - 50CF: with D != 0 the playing effect of the same group is stopped and
 *    replaced when its priority byte is >= E (the same word restarts), else
 *    the new effect is dropped (lower byte = higher priority); effects of
 *    other groups (and of group 0) overlap.
 * One output channel per group (group 0: one per effect). Not translated:
 * the chip channel allocation 4A2A/4422 (the FM/SSG drops of 109F/10A3/10A4
 * after other groups), listed in tools/test_msx_sfx.c. */
#include "msx_sfx_policy.h"
enum { MSX_SFX_COUNT=MSX_SFX_POLICY_COUNT,MSX_SFX_CHANNELS=MSX_SFX_POLICY_CHANNELS };
typedef struct {
    uint16_t word;        /* original request word */
    uint8_t group,priority,channel;
    uint16_t stops[4];    /* the header's stop list (0: none) */
    const char *name;     /* rom:/audio/<name>.wav64 */
} msx_sfx_def_t;
extern const msx_sfx_def_t msx_sfx_defs[MSX_SFX_COUNT];
/* Index of an original request word, or -1 when it has no packed sound. */
int msx_sfx_lookup(uint16_t word);
typedef struct {
    int8_t current[MSX_SFX_CHANNELS];   /* sound on each channel, -1 = idle */
    uint32_t stop_mask;                 /* channels the last request's stop list silenced */
    unsigned started,replaced,dropped,unmapped,stopped;
} msx_sfx_state_t;
_Static_assert(MSX_SFX_CHANNELS<=32,"stop_mask");
void msx_sfx_reset(msx_sfx_state_t *state);
/* Mark a channel idle (its sound finished). */
void msx_sfx_finished(msx_sfx_state_t *state,unsigned channel);
/* Apply the rule to one request. Returns the channel the sound must start
 * on (the caller restarts that voice) or -1 when it is dropped/unmapped;
 * either way the caller silences the channels of stop_mask. */
int msx_sfx_request(msx_sfx_state_t *state,uint16_t word);
#endif
