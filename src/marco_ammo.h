#ifndef MSX_MARCO_AMMO_H
#define MSX_MARCO_AMMO_H
#include <stdint.h>
#include <stdbool.h>
/* Per-player fields in the weapon service block at A5+4718:
 * +2 active, +4 cap, +6 kind, +C ammunition (word, stride two). */
typedef struct {uint16_t ammunition;uint8_t active,cap,kind;} marco_ammo_t;
/* ROM C0D64..C0DE2. Returns the original carry permission. It can consume
 * ammunition even when the active-projectile cap subsequently rejects firing.
 * Global RAM 10F001 bit 3 and nonzero 10C2E7 bypass only ammunition accounting.
 * Weapon swaps and HUD remain separate. */
bool marco_ammo_reserve(marco_ammo_t*,uint8_t requested_kind,
    bool unlimited,bool special);
/* C0DFC..C0E1E: release an active projectile of the current weapon kind.
 * Does not refund ammunition. Caller selects player 0/1 before this function;
 * the original rejects an out-of-range player index with TRAP #15. */
void marco_ammo_release(marco_ammo_t*,uint8_t projectile_kind);
/* Weapon block RAM 10C718..10C72B (A5+4718), both players, kept as the
 * original bytes: +0+p grenades in flight, +2+p active projectiles, +4+p
 * cap, +6+p weapon id (0 pistol), +8+p bombs, +A+p bomb type, +C+2p
 * ammunition (FFFF unlimited), +10+2p food counter. 10C72C (written by
 * the new-game reset 0C0E20) and 10C72E are not weapon fields and are not
 * kept. The weapon tables 0C08A8 (cap), 0C08B8 (ammunition), 0C08D8 (refill)
 * 0C08F8 (sound) and 0C0918 (upper-body form) are the original values. */
enum { MARCO_WEAPON_BLOCK=0x14, MARCO_WEAPON_IDS=16 };
typedef struct { uint8_t b[MARCO_WEAPON_BLOCK]; } marco_weapons_t;
/* The player actor fields the block's routines read or write: exists
 * (0040AC carry), +93 current form, +94 requested form (FF none), +95 shot
 * counter, +5F bit 7 dead, +9A bit 4 food eaten (0C0BF4 sets it; in form 2
 * 0C3A30 clears it and plays the weapon table C0528). */
typedef struct { bool exists,dead; uint8_t form93,form94,combo95; bool ate9a; } marco_weapon_player_t;
typedef enum { MW_REFUSED, MW_ACCEPTED, MW_STOP } marco_weapon_result_t;
extern const uint8_t marco_weapon_cap[MARCO_WEAPON_IDS],marco_weapon_form[MARCO_WEAPON_IDS];
extern const uint16_t marco_weapon_rounds[MARCO_WEAPON_IDS],marco_weapon_refill[MARCO_WEAPON_IDS],marco_weapon_sound[MARCO_WEAPON_IDS];
/* 0C0E20: a new game (both players' words +0..+8; bombs and ammunition of
 * a life come from marco_weapons_life). */
void marco_weapons_new_game(marco_weapons_t*);
/* 0C0E6E: a new life of player p (+93 = 0, 10 bombs, pistol, +94 = FF,
 * +95 = 0, bomb type 1). */
void marco_weapons_life(marco_weapons_t*,unsigned p,marco_weapon_player_t*);
/* 0C0928: give weapon `id` to player p. False (carry clear) when the player
 * is absent, in form 3 or id >= 16. *sound gets the request (0 none). */
bool marco_weapons_grant(marco_weapons_t*,unsigned p,uint8_t id,marco_weapon_player_t*,uint16_t *sound);
/* 0C0B32: give `count` bombs. `slot` is the vehicle slot 10C608/10C62C of
 * the player (0x24 bytes, NULL = none): 0AA898 hands the bombs to a ridden
 * vehicle of kind 1, 3, 4 or 8 (its +1F shells). Kind 4 reads the HUD
 * routine 0C0764 (not translated): MW_STOP. *sound gets 1124. */
marco_weapon_result_t marco_weapons_bombs(marco_weapons_t*,unsigned p,uint8_t count,const marco_weapon_player_t*,
    uint8_t *slot,uint16_t *sound);
/* 0C0A7C: bomb type `type` (08B5FA: 2, the molotov) with 10 bombs. Types
 * >= 4 are refused; 0C0B32 gives the bombs (*sound 1124); the same type
 * again is refused (the item then gives points), otherwise the type is set
 * and *voice gets 0C0A66[type] (1156 for 2). Types 1/2 never go to a
 * vehicle (0AA898 with D2 = 1/2); a kind-4 vehicle reads 0C0764: MW_STOP. */
marco_weapon_result_t marco_weapons_bomb_type(marco_weapons_t*,unsigned p,uint8_t type,const marco_weapon_player_t*,
    uint8_t *slot,uint16_t *sound,uint16_t *voice);
/* 0C2FA4..0C2FC6: the dead player gone (after 019028): no grenade in flight,
 * id 0, no bombs, ammunition 0 (0C337A then gives the pistol). */
void marco_weapons_death(marco_weapons_t*,unsigned p);
/* 0C0EE2: back to the pistol (id 0, no active projectile, cap and
 * ammunition of id 0; +95 = 0). */
void marco_weapons_pistol(marco_weapons_t*,unsigned p,marco_weapon_player_t*);
/* 0C337A..0C33AC: with no ammunition left or dead, the pistol (and form 0
 * requested from form 1). True when it reset. */
bool marco_weapons_expire(marco_weapons_t*,unsigned p,marco_weapon_player_t*);
/* 0C478E..0C47A6 (0C0808 the same for a vehicle): consume one bomb. False:
 * none left, nothing thrown. `unlimited` = RAM 10F001 bit 2. */
bool marco_weapons_throw(marco_weapons_t*,unsigned p,bool unlimited);
/* 0C0D34 / 0C0D48: a grenade in flight more / less (TRAP #15 for p >= 2:
 * false). */
bool marco_weapons_armed(marco_weapons_t*,unsigned p);
bool marco_weapons_landed(marco_weapons_t*,unsigned p);
/* The ammunition fields of player p as marco_ammo_t, and back. */
marco_ammo_t marco_weapons_ammo(const marco_weapons_t*,unsigned p);
void marco_weapons_set_ammo(marco_weapons_t*,unsigned p,const marco_ammo_t*);
static inline uint8_t marco_weapons_id(const marco_weapons_t *w,unsigned p) {return w->b[6+p];}
static inline uint8_t marco_weapons_bomb_count(const marco_weapons_t *w,unsigned p) {return w->b[8+p];}
static inline uint16_t marco_weapons_rounds(const marco_weapons_t *w,unsigned p) {return (uint16_t)(w->b[12+2*p]<<8|w->b[13+2*p]);}
#endif
