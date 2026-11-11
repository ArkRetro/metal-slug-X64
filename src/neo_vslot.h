#ifndef MSX_NEO_VSLOT_H
#define MSX_NEO_VSLOT_H
#include <stdbool.h>
#include <stdint.h>
/* Vehicle slots of the players (docs/ARMAS_VEHICULOS.md §7): manual
 * translation of the slot services ROM 0AA458..0AABF2. The slot is kept as
 * the original 0x24 bytes (RAM 10C608 P1 / 10C62C P2) so a fixture compares
 * it byte for byte:
 *   +00.w vehicle node (low word of its address; native: a nonzero node id)
 *   +02/+04.w seat X/Y, +06.l vehicle +38 (block tag), +0A/+0E.l vehicle +28/+2C
 *   +12.w frame stamp (RAM 10000E) of the last binding
 *   +14.w D4 of the mount, +16.w pending health change, +18.w vehicle health
 *   +1A.w turret angle, +1C binding (FF bound this update, 0 ridden, 2 exit
 *   requested, 1 just released), +1D exit kind, +1E cause, +1F shells,
 *   +20 bomb type, +21 vehicle kind (Camel 2, SV-001 1), +22 vehicle +15
 * Node records (`vehicle`, `player`) are the original actor layout (+00..+BF,
 * big-endian), as neo_gactor_t.m: the services read and write those offsets.
 * RAM 10C2E8/10C2EC (`record`) holds the ROM label of an 8-byte record
 * (0x016DBE, 0x016DC6 for a kind 4 vehicle). */
enum { NEO_VSLOT_BYTES=0x24, NEO_VSLOT_RECORD=0x16dbeu, NEO_VSLOT_RECORD_KIND4=0x16dc6u };
typedef struct { uint8_t b[NEO_VSLOT_BYTES]; } neo_vslot_t;
typedef struct {
    neo_vslot_t slot[2];        /* RAM 10C608 / 10C62C */
    uint32_t record[2];         /* RAM 10C2E8 / 10C2EC */
} neo_vslots_t;
/* A player as the services see it: 0040AC carry (`exists`) and its node. */
typedef struct { bool exists; uint8_t *m; } neo_vslot_player_t;
static inline uint16_t neo_vslot_node(const neo_vslot_t *s) {return (uint16_t)(s->b[0]<<8|s->b[1]);}
/* 0AA4F6 (both) / 0AA50C..0AA54E (one): a free slot, bomb type 4. */
void neo_vslot_reset(neo_vslot_t *s);
void neo_vslot_reset_all(neo_vslots_t *v);
/* 0AA458 (player p, from its common processing 0C3352): a slot bound in
 * this update (+1C != 0 and stamp = `video`, RAM 10000E) is taken (+1C = 0,
 * carry) unless 018F24 (RAM 10C301 != 0, `halted`); otherwise it is freed
 * (+00 = 0, +1C = 1; with no vehicle +1C >>= 1) and the record is 016DBE. */
bool neo_vslot_accept(neo_vslots_t *v,unsigned p,uint16_t video,bool halted);
/* 0AA4C8: the player leaves (+00 = 0, +1C = 2, record 016DBE). */
void neo_vslot_release(neo_vslots_t *v,unsigned p);
/* 0AA550: a ridden player follows the slot: +15 bit 6 from the vehicle's
 * +15, +28/+2C = vehicle + seat, +30/+34 = 0, +38 = vehicle's +38. True when
 * it wrote (the caller then projects, 01ABC8). */
bool neo_vslot_follow(neo_vslots_t *v,unsigned p,uint8_t *player);
/* 0AA5AC up to its 0143EE call: the player at the vehicle's position with
 * the seat as velocity (+38/+28/+2C/+30/+34); *kind = +1D, *cause = +1E.
 * The caller runs 0143EE, then clears +30/+34 (0AA5F2). */
void neo_vslot_exit_place(neo_vslots_t *v,unsigned p,uint8_t *player,uint8_t *kind,uint8_t *cause);
/* 0AA610 with the test 0C0556 (`players` = 1014C4/101574): a falling player
 * (+34 < 0) off the ground, alive and not in form 3, inside the rectangle
 * `rect` (four words, ROM data at A0) around `vehicle`, whose slot is free.
 * `system` = RAM 10C300 (019424), `halted` = 018F24, `rng` = 0029C6 for a
 * tie. Returns the chosen player (carry) or -1. */
/* 0AA606: 0AA616 with the test 0C069E (the crank 095C98): D1 (`d1`: the
 * player must face left) and the players' new presses (0CA2CE +0). */
typedef struct { bool d1; uint16_t fresh[2]; } neo_vslot_grab_t;
int neo_vslot_select_grab(const neo_vslots_t *v,const uint8_t *vehicle,const neo_vslot_player_t players[2],
    const int16_t rect[4],bool halted,const neo_vslot_grab_t *grab,uint16_t (*rng)(void *),void *ctx);
int neo_vslot_select(const neo_vslots_t *v,const uint8_t *vehicle,const neo_vslot_player_t players[2],
    const int16_t rect[4],bool halted,bool system,uint16_t (*rng)(void *),void *ctx);
/* 0AA760: vehicle node `node` binds player p's slot in this update (+1C =
 * FF, stamp, seat d2/d3, +38/+28/+2C, cause +5A when nonzero, +1D = 1, +22 =
 * +15) and takes the player's +5E. Carry when bound; *player6e = its +6E. */
bool neo_vslot_bind(neo_vslots_t *v,unsigned p,uint8_t *vehicle,uint16_t node,const neo_vslot_player_t *player,
    uint16_t video,int16_t d2,int16_t d3,uint16_t *player6e);
/* 0AA7FC: vehicle node `node` drops its rider (+00 = 0, +1C = 2, +1D =
 * kind, +1E = its +5A when nonzero). Kind 0 kills the rider, 1 is the eject
 * jump (0C30F0). */
void neo_vslot_exit(neo_vslots_t *v,unsigned p,const uint8_t *vehicle,uint16_t node,uint8_t kind);
/* 0AA83A: on mounting, health (vehicle +4E), kind d1, shells d2 (99 at most),
 * bomb type d3, +14 = d4; record 016DC6 for kind 4. */
void neo_vslot_mount(neo_vslots_t *v,unsigned p,const uint8_t *vehicle,uint8_t kind,uint8_t shells,uint8_t type,uint16_t d4);
/* 0AA956: one shell (+1F - 1); carry when there was one or 10F001 bit 2. */
bool neo_vslot_shell(neo_vslots_t *v,unsigned p,bool unlimited);
/* 0AA994: +5F bit 7 taken (and +16 cleared unless +5F bit 6), health +=
 * +16, copied to +4E; bit 7 set again at 0. */
void neo_vslot_damage_flag(neo_vslots_t *v,unsigned p,uint8_t *vehicle);
/* 0AA9E4 with the 004C24 carry `hit`: cause 0F kills, 0D restores +4E, else
 * -0x10 (unless +5F bit 6); then +16 is added and copied to +4E. Carry when
 * +5A != 0. */
bool neo_vslot_damage(neo_vslots_t *v,unsigned p,uint8_t *vehicle,bool hit);
/* 0AAB66: 0/1 for the player whose slot names `node`, else -1. */
int neo_vslot_which(const neo_vslots_t *v,uint16_t node);
/* 0AAB84 (the turret): the player riding vehicle `node` this update (slot
 * +00 and +1C != 0), else -1. */
int neo_vslot_rider(const neo_vslots_t *v,uint16_t node,const neo_vslot_player_t players[2]);
/* 0AABDC: turret angle +1A. */
void neo_vslot_angle(neo_vslots_t *v,unsigned p,uint16_t angle);
/* 0AAA88: kind of the ridden vehicle (0 none) and +1F/+20/+18/+14. A slot
 * of kind 0 is the TRAP #F of the original: false (explicit stop). */
bool neo_vslot_ridden(const neo_vslots_t *v,unsigned p,bool exists,uint8_t *kind,uint8_t *shells,uint8_t *type,uint16_t *health,uint16_t *d4);
/* 0AAAE0 (0C9B94 with d1 = 0; the gas can 08B710 through 0C0BA8 with d1 =
 * 0x10): a bound slot (+00 nonzero) of a vehicle kind 1, 3, 4, 8, 9, A or B
 * has a gauge (carry, true): *health = +18, *max = +14. With d1 != 0 and
 * health < max, d1 is added to the pending change +16; with health >= max
 * the carry is clear (false, the item gives its points). */
bool neo_vslot_gauge(uint8_t *slot,uint16_t d1,uint16_t *health,uint16_t *max);
/* 0CA1B8: the player's input words (RAM 10E9EC+2 held / +0 new, 10E9F2 for
 * P2; 0CA2CE returns their address) from the BIOS bytes `held`/`pressed`
 * (RAM 10C35C/10C35D): bits 0-3 the stick (held, in both words), bits 4-7
 * from the mask table `masks` (0CA1B0 = 10 20 40 30: A, B, C and A+B
 * together; RAM 10F07C when 100088 = 2): a bit is held when all its mask
 * bits are held and new when any of them was just pressed. When the
 * table's C and B masks overlap, C held clears B (0CA1E4). */
extern const uint8_t neo_vslot_masks[4];
uint8_t neo_vslot_route(uint8_t held,uint8_t pressed,const uint8_t masks[4],uint8_t *fresh);
#endif
