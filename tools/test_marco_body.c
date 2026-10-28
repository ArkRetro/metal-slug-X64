/* Synthetic cross-controller scheduling checks, not an original-game replay. */
#include <assert.h>
#include <stdio.h>
#include "../src/marco_body.h"
static unsigned selects,moves,animations;
static bool land;
static bool ms(void *c,marco_motion_table_t t,marco_motion_t *s,marco_animation_t *a) {
    (void)c;(void)t;(void)s;(void)a;++selects;return true;
}
static bool cs(void *c,marco_crouch_table_t t,marco_crouch_t *s,marco_animation_t *a) {
    (void)c;(void)t;(void)s;(void)a;++selects;return true;
}
static bool mm(void *c,marco_motion_service_t t,marco_motion_t *s,marco_animation_t *a) {
    (void)c;(void)a;++moves;assert(t==MMM_AIR);
    if(land) s->player.upper.input.attributes|=64;
    else s->player.upper.input.attributes&=(uint16_t)~64u;
    return true;
}
static bool cm(void *c,marco_crouch_t *s,marco_animation_t *a) {
    (void)c;(void)s;(void)a;++moves;return true;
}
static bool ma(void *c,marco_motion_t *s,marco_animation_t *a,bool *done) {
    (void)c;(void)s;(void)a;++animations;*done=false;return true;
}
static bool ca(void *c,marco_crouch_t *s,marco_animation_t *a,bool *done) {
    (void)c;(void)s;(void)a;++animations;*done=false;return true;
}
static marco_body_t actor(marco_body_domain_t domain,uint16_t buttons) {
    marco_body_t b={.domain=domain};
    b.motion.player.upper.input.attributes=64;
    b.motion.player.upper.input.mask9c=255;
    b.motion.player.upper.gate.limit=255;
    b.motion.player.upper.packed=(uint32_t)buttons<<16;
    b.motion.player.field6c=8;
    selects=moves=animations=0;land=false;return b;
}
int main(void) {
    marco_motion_services_t mv={.select=ms,.move=mm,.animate=ma};
    marco_crouch_services_t cv={.select=cs,.move=cm,.animate=ca};
    marco_animation_t a;marco_animation_reset(&a);
    /* Crouch -> jump must execute air physics and animate in this very tick. */
    marco_body_t b=actor(MARCO_BODY_CROUCH,32);b.motion.player.mode=MCR_IDLE;
    b.motion.player.upper.bombs=7;
    assert(marco_body_update(&b,&a,&mv,&cv)==MARCO_BODY_COMMON);
    assert(b.domain==MARCO_BODY_MOTION && b.motion.mode==MMR_AIR);
    assert(selects==1 && moves==1 && animations==1);
    assert(b.motion.player.upper.bombs==7 && b.motion.player.vy>0);
    /* Landing schedules C1EBE, but must not execute it until the next tick. */
    land=true;
    assert(marco_body_update(&b,&a,&mv,&cv)==MARCO_BODY_COMMON);
    assert(b.motion.mode==MMR_LAND_REQUEST && !b.motion.tail_call);
    assert(moves==2 && animations==2);
    assert(marco_body_update(&b,&a,&mv,&cv)==MARCO_BODY_EXTERNAL);
    assert(moves==2 && animations==2);
    /* Running -> crouch uses the same actor/clock, with no extra body step. */
    b=actor(MARCO_BODY_MOTION,8|2);b.motion.mode=MMR_RUN;
    marco_animation_reset(&a);b.motion.player.upper.combo95=19;
    assert(marco_body_update(&b,&a,&mv,&cv)==MARCO_BODY_COMMON);
    assert(b.domain==MARCO_BODY_CROUCH && b.motion.player.mode==MCR_ENTER_WAIT);
    assert(selects==1 && moves==1 && animations==1);
    assert(b.motion.player.upper.combo95==19 && a.field50_tag==0x2d0ab4);
    /* Stand-up is explicit unfinished work, never a silent no-op success. */
    b=actor(MARCO_BODY_CROUCH,0);b.motion.player.mode=MCR_IDLE;
    assert(marco_body_update(&b,&a,&mv,&cv)==MARCO_BODY_EXTERNAL);
    assert(b.motion.player.mode==MCR_STAND_REQUEST && b.motion.player.tail_call);
    assert(!selects && !moves && !animations);
    /* Missing service is sticky failure, not a fabricated animation. */
    b=actor(MARCO_BODY_CROUCH,32);b.motion.player.mode=MCR_IDLE;
    marco_animation_reset(&a);marco_motion_services_t missing={0};
    assert(marco_body_update(&b,&a,&missing,&cv)==MARCO_BODY_FAILED);
    assert(b.motion.player.failed && a.failed);
    assert(marco_body_update(&b,&a,&mv,&cv)==MARCO_BODY_FAILED);
    puts("PASS: cross-controller same-tick branches, deferred landing and explicit external/failure boundaries");
}
