#ifndef MSX_MARCO_HGUN_H
#define MSX_MARCO_HGUN_H
/* The heavy machine gun's upper-body states of the original player
 * (docs/ARMAS_VEHICULOS.md §10, "H en diagonal"): fire state 0C3E9C, burst
 * 0C3F1A, aim sweeps 0C3FA6..0C3FDE/0C3FFA/0C4006 and the fire callbacks of
 * their programs (Marco, form 1; tables 0BFDD8.. +0x10): 0C4372 (bullet with
 * the aim of 0C4244 and the fan 0C44A6) and 0C453A (bullet at the angle the
 * program names: the diagonal steps 08/18/28/38 of a sweep). Pure C. */
#include <stdbool.h>
#include <stdint.h>

typedef enum { MARCO_HGUN_IDLE,MARCO_HGUN_ENTER,MARCO_HGUN_BURST,MARCO_HGUN_SWEEP } marco_hgun_state_t;
/* Programs (index into the timelines of marco_hgun.c). */
typedef enum { HGUN_FIRE_FWD,HGUN_FIRE_UP,HGUN_FIRE_DOWN,HGUN_FWD_UP,HGUN_FWD_DOWN,HGUN_UP_FWD,HGUN_DOWN_FWD,HGUN_PROGRAMS } marco_hgun_program_t;
typedef struct {
    marco_hgun_state_t state;
    uint8_t program,t;   /* program and its update count */
    int8_t aim92;        /* +92: 1 up, 0 forward, -1 down */
    bool flag76;         /* +76: the burst may restart (written at t=7) */
    bool request;        /* +9B bit 4: fire pressed since it was last taken */
    uint8_t combo95;     /* +95 (shared with marco_weapon_player_t) */
    bool fat;            /* +93 = 2: the programs of the rows +0x20 (0C436A, 0C4610); kept by reset */
} marco_hgun_t;
typedef struct { bool up,down,facing_left; } marco_hgun_input_t;
/* One bullet request of a callback; the caller reserves it with 0C0D64 and
 * counts +95 only when the bullet exists (allowed). */
typedef struct { bool fire; bool sweep; uint8_t angle,base; } marco_hgun_shot_t;  /* base: before the fan */
void marco_hgun_reset(marco_hgun_t *h);
/* A bomb throw takes the upper body: +92 back to 0 (hfam 12000/12300/12360). */
void marco_hgun_throw(marco_hgun_t *h);
/* The fire button newly pressed (the controller's +9B bit 4). */
static inline void marco_hgun_press(marco_hgun_t *h) {h->request=true;}
/* One player update. reserve(context) is 0C0D64 for the H: true when a bullet
 * is created (then +95 and the fan apply). Returns the bullets created. */
typedef bool (*marco_hgun_reserve_t)(void *context);
unsigned marco_hgun_step(marco_hgun_t *h,const marco_hgun_input_t *in,marco_hgun_reserve_t reserve,void *context,
                         marco_hgun_shot_t *out,unsigned max);
#endif
