/* Synthetic cross-controller scheduling checks, not an original-game replay. */
#include <assert.h>
#include <stdio.h>
#include "../src/marco_body.h"
#ifdef __mips__
#include <libdragon.h>
#endif
static unsigned selects,moves,animations;
static bool land,finish;
static bool ms(void *c,marco_motion_table_t t,marco_motion_t *s,marco_animation_t *a) {
    (void)c;(void)t;(void)s;(void)a;++selects;return true;
}
static bool ss(void *c,marco_stance_table_t t,marco_stance_t *st,marco_motion_t *m,marco_animation_t *a) {
    (void)c;(void)t;(void)st;(void)m;(void)a;++selects;return true;
}
static bool cs(void *c,marco_crouch_table_t t,marco_crouch_t *s,marco_animation_t *a) {
    (void)c;(void)t;(void)s;(void)a;++selects;return true;
}
static bool mm(void *c,marco_motion_service_t t,marco_motion_t *s,marco_animation_t *a) {
    (void)c;(void)a;++moves;
    if(t==MMM_GROUND) return true;
    assert(t==MMM_AIR);
    if(land) s->player.upper.input.attributes|=64;
    else s->player.upper.input.attributes&=(uint16_t)~64u;
    return true;
}
static bool cm(void *c,marco_crouch_t *s,marco_animation_t *a) {
    (void)c;(void)s;(void)a;++moves;return true;
}
static bool ma(void *c,marco_motion_t *s,marco_animation_t *a,bool *done) {
    (void)c;(void)s;(void)a;++animations;*done=finish;return true;
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
    selects=moves=animations=0;land=finish=false;return b;
}
static unsigned idle_queries;
static bool idle_query(void *ctx,marco_idle_query_t key,int16_t arg,marco_motion_t *m,uint16_t *value,bool *carry) {
    (void)ctx;(void)arg;(void)m;
    assert(key==(idle_queries?MSQ_191FC:MSQ_18F24));++idle_queries;
    *value=0;*carry=false;return true;
}
int main(void) {
#ifdef __mips__
    debug_init_emulog();console_init();console_set_render_mode(RENDER_MANUAL);
#endif
    marco_motion_services_t mv={.select=ms,.move=mm,.animate=ma};
    marco_crouch_services_t cv={.select=cs,.move=cm,.animate=ca};
    marco_stance_services_t sv={.select=ss};
    marco_animation_t a;marco_animation_reset(&a);
    marco_body_t idle=actor(MARCO_BODY_STANCE,32);idle.stance.mode=MST_IDLE;
    assert(marco_body_update(&idle,&a,&mv,&cv,&sv)==MARCO_BODY_COMMON);
    assert(idle.domain==MARCO_BODY_MOTION && idle.motion.mode==MMR_AIR);
    assert(selects==1 && moves==1 && animations==1);
    idle=actor(MARCO_BODY_STANCE,2);idle.stance.mode=MST_IDLE;
    marco_animation_reset(&a);
    assert(marco_body_update(&idle,&a,&mv,&cv,&sv)==MARCO_BODY_COMMON);
    assert(idle.domain==MARCO_BODY_CROUCH && idle.motion.player.mode==MCR_ENTER_WAIT);
    assert(selects==1 && moves==1 && animations==1);
    idle=actor(MARCO_BODY_STANCE,0);idle.stance.mode=MST_IDLE;
    idle.motion.player.upper.input.flags9a|=32;
    sv.idle_query=idle_query;idle_queries=0;marco_animation_reset(&a);
    assert(marco_body_update(&idle,&a,&mv,&cv,&sv)==MARCO_BODY_COMMON);
    assert(idle_queries==2 && idle.stance.field96==1 && idle.stance.mode==MST_IDLE);
    assert(moves==1 && animations==1 && !selects);
    sv.idle_query=NULL;
    /* Crouch -> jump must execute air physics and animate in this very tick. */
    marco_body_t b=actor(MARCO_BODY_CROUCH,32);b.motion.player.mode=MCR_IDLE;
    b.motion.player.upper.bombs=7;
    assert(marco_body_update(&b,&a,&mv,&cv,&sv)==MARCO_BODY_COMMON);
    assert(b.domain==MARCO_BODY_MOTION && b.motion.mode==MMR_AIR);
    assert(selects==1 && moves==1 && animations==1);
    assert(b.motion.player.upper.bombs==7 && b.motion.player.vy>0);
    /* Landing schedules C1EBE, but must not execute it until the next tick. */
    land=true;
    assert(marco_body_update(&b,&a,&mv,&cv,&sv)==MARCO_BODY_COMMON);
    assert(b.motion.mode==MMR_LAND_REQUEST && !b.motion.tail_call);
    assert(moves==2 && animations==2);
    b.motion.player.upper.packed=0;
    assert(marco_body_update(&b,&a,&mv,&cv,&sv)==MARCO_BODY_COMMON);
    assert(moves==3 && animations==3 && selects==2);
    assert(b.domain==MARCO_BODY_STANCE && b.stance.mode==MST_IDLE);
    assert(b.stance.field1e==0x8834 && b.stance.field66==0x8834);
    /* Running -> crouch uses the same actor/clock, with no extra body step. */
    b=actor(MARCO_BODY_MOTION,8|2);b.motion.mode=MMR_RUN;
    marco_animation_reset(&a);b.motion.player.upper.combo95=19;
    assert(marco_body_update(&b,&a,&mv,&cv,&sv)==MARCO_BODY_COMMON);
    assert(b.domain==MARCO_BODY_CROUCH && b.motion.player.mode==MCR_ENTER_WAIT);
    assert(selects==1 && moves==1 && animations==1);
    assert(b.motion.player.upper.combo95==19 && a.field50_tag==0x2d0ab4);
    /* Stand-up initialization and its first body step share this tick. */
    b=actor(MARCO_BODY_CROUCH,0);b.motion.player.mode=MCR_IDLE;
    assert(marco_body_update(&b,&a,&mv,&cv,&sv)==MARCO_BODY_COMMON);
    assert(b.domain==MARCO_BODY_STANCE && b.stance.mode==MST_IDLE);
    assert(selects==1 && moves==1 && animations==1);
    /* Clock completion schedules the next entry, without advancing it twice. */
    finish=true;
    assert(marco_body_update(&b,&a,&mv,&cv,&sv)==MARCO_BODY_COMMON);
    assert(b.stance.mode==MST_IDLE_FAST && selects==1 && moves==2 && animations==2);
    /* Running stop enters stance in the same tick, preserving an active aim. */
    b=actor(MARCO_BODY_MOTION,0);b.motion.mode=MMR_RUN;
    b.motion.player.upper.aim92=1;b.motion.player.upper.mode=MUP_ACTIVE;
    assert(marco_body_update(&b,&a,&mv,&cv,&sv)==MARCO_BODY_EXTERNAL);
    assert(b.domain==MARCO_BODY_STANCE && b.stance.mode==MST_IDLE);
    assert(b.motion.player.upper.mode==MUP_ACTIVE && b.motion.player.upper.aim92==1);
    assert(selects==1 && !moves && !animations);
    /* Missing stance support fails instead of silently dropping the transition. */
    b=actor(MARCO_BODY_CROUCH,0);b.motion.player.mode=MCR_IDLE;
    marco_animation_reset(&a);
    assert(marco_body_update(&b,&a,&mv,&cv,NULL)==MARCO_BODY_FAILED);
    assert(b.motion.player.failed && a.failed);
    /* Missing service is sticky failure, not a fabricated animation. */
    b=actor(MARCO_BODY_CROUCH,32);b.motion.player.mode=MCR_IDLE;
    marco_animation_reset(&a);marco_motion_services_t missing={0};
    assert(marco_body_update(&b,&a,&missing,&cv,&sv)==MARCO_BODY_FAILED);
    assert(b.motion.player.failed && a.failed);
    assert(marco_body_update(&b,&a,&mv,&cv,&sv)==MARCO_BODY_FAILED);
    puts("PASS: cross-controller same-tick branches, deferred landing and explicit external/failure boundaries");
#ifdef __mips__
    debugf("MSX BODY REGRESSION PASS\n");console_render();while(1) {}
#endif
}
