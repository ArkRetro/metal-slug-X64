/* Synthetic service limits, not claims about unobserved gameplay branches. */
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "../src/marco_crouch.h"
int main(void) {
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
}
