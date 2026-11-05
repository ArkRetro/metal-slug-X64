#ifndef MSX_MARCO_CAMPAIGN_PISTOL_H
#define MSX_MARCO_CAMPAIGN_PISTOL_H
#include "marco_campaign.h"
#include "marco_ammo.h"

/* 01B782..01B78C invokes 0120F8 with this token when the deferred child
 * initializes. 012182..01218C sends bytes 10,F7 to its sound command queue. */
enum {MARCO_CAMPAIGN_PISTOL_SOUND=0x10f7};

/* Native output of C4372 -> C4244 -> normal-pistol initializer 01B782.
 * Position and velocity are world Q16.16, Y upwards. Velocity is the original
 * per-projectile-actor-call value. All 134 captured normal pistol allocations
 * first execute their child actor two video ticks later, then every two ticks.
 * The scene allocates immediately and owns that deferred actor-call cadence.
 * Angle is the original byte: right=0, up=64, left=128, down=192. */
typedef struct {
    int32_t x,y,vx,vy;
    uint8_t angle,kind;
} marco_campaign_shot_t;
typedef bool (*marco_campaign_pistol_spawn_t)(void*,const marco_campaign_shot_t*);
typedef struct {
    marco_ammo_t ammo;
    bool unlimited,special;
} marco_campaign_pistol_t;

/* Captured normal pistol service: kind 0, ammunition FFFF, cap FF. The native
 * world's own storage capacity remains an independent allocator constraint. */
void marco_campaign_pistol_init(marco_campaign_pistol_t*);
/* Handles only a reached NEO_ANIM_EVENT targeting C4372. Normal form/kind 0
 * is recovered here. Unsupported events/forms/weapons return false without
 * consuming ammunition. An ammunition or world-allocation rejection is a
 * handled event; only an actual successful spawn increments upper.combo95.
 * Reservation precedes allocation, and a failed allocation never refunds ammo.
 * The callback has no inline operands: current aim, pose offsets and position
 * are sampled live from the campaign. */
bool marco_campaign_pistol_event(marco_campaign_pistol_t*,marco_campaign_t*,
    const marco_campaign_event_t*,marco_campaign_pistol_spawn_t,void*);
/* Call once when a real world projectile retires. No ammunition refund. */
void marco_campaign_pistol_release(marco_campaign_pistol_t*,marco_campaign_t*,uint8_t kind);
#endif
