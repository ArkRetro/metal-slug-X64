/* Synthetic service limits, not claims about unobserved gameplay branches. */
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "../src/marco_crouch.h"
#ifdef __mips__
#include <libdragon.h>
#endif
static unsigned selections,animations;
static bool finished;
static bool form_select(void *ctx,marco_upper_table_t key,bool direct,marco_upper_t *s,marco_animation_t *a) {
    (void)ctx;(void)a;assert(key==MUT_3942 && direct && !(s->input.flags9a&32u));
    ++selections;return true;
}
static bool form_animate(void *ctx,marco_upper_t *s,marco_animation_t *a,bool *done) {
    (void)ctx;(void)a;assert(s->mode==MUP_FORM_WAIT && !s->aim92 && !s->idle97);
    ++animations;*done=finished;return true;
}
static bool pose_select(void *ctx,marco_upper_table_t key,bool direct,marco_upper_t *s,marco_animation_t *a) {
    (void)ctx;(void)s;(void)a;assert(key==MUT_383C && !direct);++selections;return true;
}
static bool pose_animate(void *ctx,marco_upper_t *s,marco_animation_t *a,bool *done) {
    (void)ctx;(void)a;assert(s->mode==MUP_ACTIVE && s->active91==3);++animations;*done=false;return true;
}
static void form_limits(void) {
    marco_upper_t s={.mode=MUP_FORM_START,.desired90=255,.aim92=255,.idle97=23};
    s.input.flags9a=32;marco_animation_t a;marco_animation_reset(&a);
    marco_upper_services_t v={.select=form_select,.animate=form_animate};
    assert(marco_upper_update(&s,&a,&v) && selections==1 && animations==1 && s.active91==4);
    finished=true;
    assert(marco_upper_update(&s,&a,&v) && selections==1 && animations==2 && s.active91==255 && s.mode==MUP_FORM_WAIT);
    // Dispatch preempts a waiting animation when a new pose is requested.
    s.desired90=3;marco_upper_t next=s;
    static const uint16_t masks[4]={0,0,0,8};
    marco_upper_services_t pose={.select=pose_select,.animate=pose_animate,.compatibility=masks,.compatibility_count=4};
    assert(marco_upper_update(&next,&a,&pose) && next.mode==MUP_ACTIVE && next.active91==3 && animations==3 && selections==2);
    marco_upper_services_t missing={0};
    assert(!marco_upper_update(&s,&a,&missing) && animations==3 && s.failed && a.failed);
}
int main(void) {
#ifdef __mips__
    debug_init_emulog();console_init();console_set_render_mode(RENDER_MANUAL);
#endif
    form_limits();
    marco_upper_t s={.desired90=3,.active91=4,.action98=2,.aim92=255,.combo95=255,.bombs=1};
    marco_animation_t a;marco_animation_reset(&a);
    marco_upper_reset(&s);assert(s.mode==MUP_RESET && s.desired90==255 && s.active91==255 && s.action98==0 && s.aim92==255 && s.combo95==255);
    marco_upper_projectile_created(&s,false);assert(s.combo95==255);
    marco_upper_projectile_created(&s,true);assert(s.combo95==0);
    assert(marco_upper_consume_grenade(&s) && s.bombs==0);
    assert(!marco_upper_consume_grenade(&s) && s.bombs==0);
    s.unlimited_bombs=true;assert(marco_upper_consume_grenade(&s) && s.bombs==0);
    marco_animation_view_t view={0};bool done=true;a.flash=4;
    assert(marco_upper_animation_step(&s,&a,&view,NULL,NULL,16,NULL,NULL,&done) && !done && a.flash==4);
    neo_anim_command_t finish={.kind=NEO_ANIM_FINISH};neo_anim_program_t program={&finish,1};
    assert(marco_animation_start(&a,MARCO_WEAPON,&program,0,256));
    assert(marco_upper_animation_step(&s,&a,&view,NULL,NULL,16,NULL,NULL,&done) && done && a.flash==3);
    marco_upper_services_t unavailable={0};s.mode=MUP_ACTIVE;s.aim92=0;s.idle97=3;s.input.mask9c=255;
    assert(!marco_upper_update(&s,&a,&unavailable) && s.failed && a.failed);
    assert(!marco_upper_update(&s,&a,&unavailable));
    assert(!marco_upper_animation_step(&s,&a,NULL,NULL,NULL,16,NULL,NULL,&done));
    marco_crouch_t body={0};marco_animation_reset(&a);done=false;
    assert(marco_crouch_animation_step(&body,&a,&view,NULL,NULL,16,NULL,NULL,&done) && done);
    marco_crouch_services_t missing_body={0};
    assert(!marco_crouch_update(&body,&a,&missing_body) && body.failed && body.upper.failed && a.failed);
    assert(!marco_crouch_update(&body,&a,&missing_body));
    memset(&body,0,sizeof(body));marco_animation_reset(&a);body.mode=MCR_FIRE;body.upper.weapon_kind=16;
    assert(!marco_crouch_update(&body,&a,&missing_body) && body.failed);
    puts("PASS: synthetic upper-controller reset, allocation counters, grenade gates, D7/null semantics and missing-service stop");
#ifdef __mips__
    debugf("MSX UPPERFORM REGRESSION PASS\n");console_render();while(1) {}
#endif

}
