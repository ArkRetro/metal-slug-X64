#ifndef MSX_NEO_ANIMATION_H
#define MSX_NEO_ANIMATION_H
#include <stdbool.h>
#include <stdint.h>
/* Offline-converted animation commands. These enums are native data, not
 * M68000 opcodes. Descriptors and field50 values are opaque tags, never CPU
 * pointers; field50's complete gameplay semantics remain unverified. */
typedef enum { NEO_ANIM_WAIT,NEO_ANIM_POSE,NEO_ANIM_JUMP,NEO_ANIM_FIELD50,
    NEO_ANIM_FINISH,NEO_ANIM_FLIP_FACING,NEO_ANIM_UNRECOVERED,
    NEO_ANIM_REPLACE_PALETTE,NEO_ANIM_SPAWN_THROW,NEO_ANIM_SELECT_POSE,
    NEO_ANIM_WRITE_BYTE,NEO_ANIM_WRITE_WORD,NEO_ANIM_WRITE_LONG,
    NEO_ANIM_DECREMENT_BYTE,NEO_ANIM_BRANCH_FIELD,NEO_ANIM_SELECT_FIELD_POSE,
    NEO_ANIM_EVENT,
    /* Soldier/death actor interface (neo_anim_step_actor): original
     * 0x18/0x1C/0x20 field writes, 0x24 decrement, 0x0C conditional jump,
     * 0x2C switch, 0x30 sound request and 0x14 callbacks without a dedicated
     * kind. Writes/decrement: target = field offset, asset = value.
     * COND_JUMP: asset = field, operand = flag, target = command index.
     * SWITCH: asset = field, operand = entry count, followed by that many JUMP
     * entries. SOUND: target = request id. CALL: asset = ROM routine label
     * (provenance only), target = parameter word. */
    NEO_ANIM_SET_BYTE,NEO_ANIM_SET_WORD,NEO_ANIM_SET_LONG,NEO_ANIM_DEC_BYTE,
    NEO_ANIM_COND_JUMP,NEO_ANIM_SWITCH,NEO_ANIM_SOUND,NEO_ANIM_CALL } neo_anim_kind_t;
/* Packed to 12 bytes (was 16): ~30k converted commands live in the program
 * image, so the layout is a RAM budget item (docs/RENDIMIENTO.md). Field
 * order and values are unchanged; asset is read unaligned. */
typedef struct __attribute__((packed,aligned(4))) {
    neo_anim_kind_t kind:8;
    uint32_t asset;
    uint16_t target;
    uint8_t operand,flags;
    int8_t dx,dy;
} neo_anim_command_t;
_Static_assert(sizeof(neo_anim_command_t)==12,"neo_anim_command_t layout");
typedef struct { const neo_anim_command_t *commands; uint16_t count; } neo_anim_program_t;
typedef struct {
    const neo_anim_program_t *program;
    uint32_t field50_tag;
    uint16_t cursor,timer,speed;
    uint8_t flash,field18;
} neo_anim_state_t;
typedef struct {
    uint16_t x,y,attributes,z;
    uint8_t correction,flash_palette,palettes[4];
    bool suppress;
    uint8_t variant61; // Native actor selector consumed by original 004412.
} neo_anim_render_t;
/* attributes is in/out: the recovered callback toggles facing bit 0. A game
 * controller must retain that change for physics/AI, not only for rendering. */
typedef struct { uint32_t descriptor; uint16_t x,y,attributes,z; } neo_anim_draw_t;
typedef void (*neo_anim_emit_t)(void *context,const neo_anim_draw_t *draw);
typedef enum { NEO_ANIM_MORE,NEO_ANIM_COMPLETE,NEO_ANIM_INVALID,NEO_ANIM_UNSUPPORTED,
    NEO_ANIM_BUDGET_EXHAUSTED } neo_anim_result_t;
/* Explicit native effects of recovered callbacks; handler owns native palette
 * resources/spawn queues. It never receives a callable original CPU address. */
typedef bool (*neo_anim_event_t)(void *context,neo_anim_kind_t kind,uint32_t asset,
    neo_anim_render_t *render);
/* Typed field boundary for converted player animation data. Field identifiers
 * are offline provenance tags; this callback maps them to native C members,
 * never to emulated RAM. Return false for fields not yet recovered. */
typedef bool (*neo_anim_field_t)(void *context,unsigned field,unsigned bytes,bool write,uint32_t *value);
neo_anim_result_t neo_anim_step_fields(neo_anim_state_t *state,neo_anim_render_t *render,
    neo_anim_emit_t emit,void *context,unsigned budget,neo_anim_event_t event,void *event_context,
    neo_anim_field_t field,void *field_context);
/* ROM 004256: reset timer/field18, preserve flash and field50 tag. */
void neo_anim_start(neo_anim_state_t *state,const neo_anim_program_t *program,uint16_t speed);
/* ROM 00427A control flow, with RDP-independent output. budget is a caller's
 * malformed-data guard, not an invented original animation/frame limit. */
neo_anim_result_t neo_anim_step(neo_anim_state_t *state,neo_anim_render_t *render,
    neo_anim_emit_t emit,void *context,unsigned budget);
neo_anim_result_t neo_anim_step_events(neo_anim_state_t *state,neo_anim_render_t *render,
    neo_anim_emit_t emit,void *context,unsigned budget,neo_anim_event_t event,void *event_context);
/* Native actor interface for the soldier commands. Field numbers are the
 * original offsets used as names; the actor maps each one it models to a
 * named C field and returns false for anything else (explicit failure). */
typedef struct {
    neo_anim_event_t event;
    bool (*read_byte)(void *context,uint16_t field,uint8_t *value);
    bool (*write_field)(void *context,uint16_t field,unsigned size,uint32_t value);
    void (*sound)(void *context,uint16_t id);
    bool (*call)(void *context,uint32_t routine,uint16_t parameter,neo_anim_render_t *render);
    void *context;
} neo_anim_actor_t;
neo_anim_result_t neo_anim_step_actor(neo_anim_state_t *state,neo_anim_render_t *render,
    neo_anim_emit_t emit,void *context,unsigned budget,const neo_anim_actor_t *actor);
#endif
