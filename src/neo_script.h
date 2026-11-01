#ifndef MSX_NEO_SCRIPT_H
#define MSX_NEO_SCRIPT_H
#include <stdbool.h>
#include <stdint.h>
/* Native interpreter of the original level script (ROM 019E72/019F1A main
 * actor, 01A056/01A0A6/01A372 statement walkers, 01A422/01A438 periodic
 * spawner "runner" actors, 01A5CA conditions). The script itself is imported
 * record data (data/soldier_ai_tables.json, ROM 2D6314..2D651A); positions in
 * it are byte offsets, reported as ROM addresses for provenance only.
 *
 * Scope: the opcodes and condition types used by Mission 1 (ROM
 * 2D6314..2D7132). Camera lock/release (0C7B0C/0C7C12) and the auto-scroll
 * pair 0C7B4E/0C7BBA (ops D/E) are reported to the caller (src/neo_camera.c
 * implements them); grouped runners 01A210 run as runners counted in their
 * group (+69, recounted by the caller); conditions 14/15 halt the
 * interpreter explicitly (`halted`/`halt_address`); nothing is approximated. The
 * cartridge protection probe interleaved in 019E8C/01A464/01A53A (writes to
 * 2FFFE0, branch to 0197D6 on a mismatch) has no gameplay effect on genuine
 * hardware and is not reproduced. */
enum { NEO_SCRIPT_RUNNERS=8, NEO_SCRIPT_GROUPS=9, NEO_SCRIPT_SPAWN_WORDS=12 };
typedef struct {
    bool active;
    uint16_t record;       /* offset of the op 8 record (+6E) */
    uint16_t timer72,count76;
    bool spawned74;        /* +74 set by a spawn during the current body run */
    uint8_t group69;       /* +69 of a grouped runner (op 11, 01A210); 0xFF otherwise */
} neo_script_runner_t;
typedef struct {
    const uint8_t *code;uint16_t size;uint32_t base;
    uint16_t pc;           /* +6E of the script actor */
    uint16_t timer72;      /* +72 (op 1 countdown) */
    bool spawned74;
    bool finished;         /* op 3 executed (+5F bit 7) */
    bool halted;uint32_t halt_address;
    unsigned skipped;      /* records of unported constructors skipped (neo_script_env_t.skip_unported) */
    uint32_t skipped_entry,skipped_address; /* the last one: constructor and record */
    /* RAM 10C326..10C32E: per-group live counts. Recounted by the caller at
     * the start of every original tick (ROM 019E40, called from the mission
     * actor 0186A0 before the script runs); incremented here on spawn. */
    uint8_t groups[NEO_SCRIPT_GROUPS];
    uint16_t w4330,w4332,w69b8,w69ba; /* op 10 / op F operands */
    neo_script_runner_t runners[NEO_SCRIPT_RUNNERS];
} neo_script_t;
typedef struct {
    uint32_t record;       /* ROM address of the op 7 record */
    uint32_t constructor;  /* entry of ROM 0197DC (bit 31: not group-counted) */
    uint8_t group69;       /* 0xFF when not counted */
    uint8_t variant68,flags;
    int32_t x,y;           /* Q16.16, relative to the scene root (01ABB6) */
    uint16_t words[NEO_SCRIPT_SPAWN_WORDS];uint8_t word_count; /* copied to +6E.. */
    int creator;           /* -1 = script actor, otherwise runner index */
} neo_script_spawn_t;
/* One player actor as ROM 01903A reads it. */
typedef struct {
    bool alive;            /* routine != 003F1E */
    uint8_t follow_x,follow_y; /* +7A/+7B: use +3C/+40 instead of the 69B8/69BA point */
    uint16_t px,py;        /* +3C/+40 projected words */
} neo_script_player_t;
typedef struct {
    uint16_t camera;       /* RAM 10E99E */
    int32_t scroll_x,scroll_y; /* RAM 10E97C/10E980 (negated camera, Q16.16) */
    neo_script_player_t players[2];
    uint16_t (*rng)(void *context);                          /* one ROM 0029C6 call */
    void (*spawn)(void *context,const neo_script_spawn_t *spawn);
    void (*runner)(void *context,int creator,unsigned index); /* runner created after `creator` */
    /* ROM 0C7B0C: lock the camera at x (RAM 69BC) with the four bounds
     * 69C0..69C6 (0x8000 = none); ROM 0C7C12 (op C): release (69BC = -1). */
    void (*camera_lock)(void *context,uint16_t x,const uint16_t bounds[4]);
    void (*camera_release)(void *context);
    /* ROM 0C7B4E (op D) / 0C7BBA (op E): camera autoscroll start/stop. */
    void (*autoscroll)(void *context,bool start);
    void *context;
    /* Optional: false for a constructor the port does not create. A record
     * counted in its group (group < 9, bit 31 clear) of such a constructor
     * halts the script there (explicit stop): its group would never empty
     * and the waits on it would hold the script forever. */
    bool (*native)(uint32_t constructor);
    /* Reference cartridge only: such a record is skipped (not created, not
     * counted) instead of halting, so the script reaches the records after
     * it (the stage 3 drone 03670E of Mission 2 is native, neo_drone.c;
     * its records at 2D8846.. are grouped runners, op 11). */
    bool skip_unported;
} neo_script_env_t;
void neo_script_init(neo_script_t *script,const uint8_t *code,uint16_t size,uint32_t base);
/* Script actor update (ROM 019F1A). */
void neo_script_tick(neo_script_t *script,const neo_script_env_t *env);
/* Runner update (ROM 01A438). Returns false when the runner deletes itself. */
bool neo_script_tick_runner(neo_script_t *script,unsigned index,const neo_script_env_t *env);
static inline uint32_t neo_script_address(const neo_script_t *s,uint16_t offset) { return s->base+offset; }
/* True once the script is blocked at or beyond `address`. */
bool neo_script_reached(const neo_script_t *script,uint32_t address);
#endif
