#include <assert.h>
#include <stdio.h>
#include "../src/neo_animation.h"
#include "animation_fixture.h"
#ifdef N64
#include <libdragon.h>
#endif
#define COUNT(a) (sizeof(a)/sizeof((a)[0]))
typedef struct { unsigned next,end; } draw_cursor_t;
static void emit(void *context,const neo_anim_draw_t *draw) {
    draw_cursor_t *cursor=context;assert(cursor->next<cursor->end);
    const neo_anim_draw_t *expected=&anim_expected_draws[cursor->next++];
    assert(draw->descriptor==expected->descriptor && draw->x==expected->x && draw->y==expected->y);
    assert(draw->attributes==expected->attributes && draw->z==expected->z);
}
int main(void) {
#ifdef N64
    debug_init_emulog();console_init();
#endif
    unsigned starts=0,steps=0,draws=0,speed_changes=0;
    neo_anim_state_t state={0};
    for (unsigned i=0;i<COUNT(anim_cases);i++) {
        const anim_fixture_state_t *b=&anim_cases[i].before,*a=&anim_cases[i].after;
        if (!i) state=(neo_anim_state_t){NULL,b->field50,b->cursor,b->timer,b->speed,b->flash,b->field18};
        // Replay continuously. Only the two measured +0x26 initializer writes
        // are external; never reload expected cursor/timer/output between calls.
        if (state.speed!=b->speed) {state.speed=b->speed;speed_changes++;}
        assert(state.timer==b->timer && state.flash==b->flash && state.field18==b->field18);
        assert(state.field50_tag==b->field50);
        neo_anim_render_t render=anim_cases[i].render;
        draw_cursor_t cursor={anim_cases[i].draw_start,anim_cases[i].draw_start+anim_cases[i].draw_count};
        if (anim_cases[i].init) {neo_anim_start(&state,&anim_programs[anim_cases[i].program],anim_cases[i].requested);starts++;}
        else {
            assert(state.program==&anim_programs[anim_cases[i].program] && state.cursor==b->cursor);
            neo_anim_result_t result=neo_anim_step(&state,&render,emit,&cursor,1024);
            assert(result==(anim_cases[i].complete?NEO_ANIM_COMPLETE:NEO_ANIM_MORE));
            assert(render.attributes==anim_cases[i].after_attr);steps++;
        }
        assert(cursor.next==cursor.end);draws+=anim_cases[i].draw_count;
        assert(state.cursor==a->cursor && state.timer==a->timer && state.speed==a->speed);
        assert(state.flash==a->flash && state.field18==a->field18 && state.field50_tag==a->field50);
    }
    assert(speed_changes==ANIM_EXTERNAL_SPEED_WRITES);
    printf("PASS: %u animation starts, %u helper steps and %u exact descriptor/position/palette draws\n",starts,steps,draws);
#ifdef N64
    debugf("MSX ANIMATION REGRESSION PASS starts=%u steps=%u draws=%u\n",starts,steps,draws);
    for (;;) {wait_ms(100);}
#endif
}
