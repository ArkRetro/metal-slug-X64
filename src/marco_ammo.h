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
#endif
