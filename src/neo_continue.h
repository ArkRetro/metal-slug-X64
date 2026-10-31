#ifndef MSX_NEO_CONTINUE_H
#define MSX_NEO_CONTINUE_H
#include "neo_end.h"
/* The big CONTINUE? and the countdown digit of the continue (docs/
 * FIN_PARTIDA.md): the mission actor 101364's own animation program 018120
 * (started at the mission start, 01868A), stepped by its 00427A at 01883C
 * only while the continue counter +7E runs (0187F0..01883C: continue DIP,
 * counter nonzero, no player in play mode, 10F003, 10E9E3 clear). The
 * counter itself is neo_hud.c's (continue_timer). */
enum { NEO_CONTINUE_DRAWS=16 };
typedef struct {
    neo_gactor_t node;              /* the mission actor's animation bytes */
    neo_end_draw_t draws[NEO_CONTINUE_DRAWS];unsigned draw_count;
    unsigned steps,unsupported;uint32_t unsupported_at;
} neo_continue_t;
/* 01868A: 004256 with 018120, speed 0x100. */
void neo_continue_start(neo_continue_t *c,const neo_anim_program_t *(*program)(uint32_t tag));
/* 01883C: one 00427A with the counter word +7E; front_layer = 012022's
 * test (RAM 10B954 < 10B95C). The draws of this update replace the list. */
void neo_continue_draw(neo_continue_t *c,uint16_t counter,bool front_layer,const neo_anim_program_t *(*program)(uint32_t tag));
/* Draws are kept only for the updates that step the program. */
static inline void neo_continue_clear(neo_continue_t *c) {c->draw_count=0;}
#endif
