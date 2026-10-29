#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "../src/neo_throw.h"
#include "throw_fixture.h"
#ifdef N64
#include <libdragon.h>
#endif
#define COUNT(a) (sizeof(a)/sizeof((a)[0]))
#define SAME(f) assert(a->f==b->f)
static void equal(const neo_contact_actor_t *a,const neo_contact_actor_t *b) {
    SAME(x);SAME(y);SAME(vx);SAME(vy);SAME(scalar62);SAME(projected_x);SAME(projected_y);SAME(cached_x);SAME(cached_y);
    SAME(parent_tag);SAME(kind_angle);SAME(field66);SAME(word1e);SAME(flags15);SAME(flags5f);SAME(timer5a);SAME(mark);SAME(correction);
}
#undef SAME
int main(void) {
#ifdef N64
    debug_init_emulog();console_init();
#endif
    neo_contact_actor_t a;
    neo_throw_spawn(&a,&throw_source,&throw_scene,throw_spawn_epoch);
    assert(a.x==throw_spawned.x && a.y==throw_spawned.y && a.vx==throw_spawned.vx && a.vy==throw_spawned.vy);
    assert(a.parent_tag==throw_spawned.parent_tag && a.flags15==throw_spawned.flags15);
    neo_throw_initialize(&a,&throw_players,throw_field6e);equal(&a,&throw_initialized);
    for (unsigned i=0;i<COUNT(throw_cases);i++) {
        equal(&a,&throw_cases[i].before);
        neo_contact_actor_t intermediate=a;neo_throw_accelerate(&intermediate);equal(&intermediate,&throw_cases[i].accelerated);
        neo_contact_request_t r=neo_contact_prepare_air(&intermediate);
        const neo_contact_request_t *e=&throw_cases[i].request;
        assert(r.x==e->x && r.y==e->y && r.dx==e->dx && r.dy==e->dy && r.parent_tag==e->parent_tag);
        assert(r.mode==e->mode && r.mode8==e->mode8 && r.mode9==e->mode9 && r.modea==e->modea && r.moded==e->moded);
        neo_contact_result_t k;
        assert(neo_throw_free_step(&a,&throw_cases[i].parent,throw_cases[i].epoch,&k));equal(&a,&throw_cases[i].after);
        const neo_contact_result_t *expected=&throw_cases[i].kernel;
        assert(k.x==expected->x && k.y==expected->y && k.vx==expected->vx && k.vy==expected->vy);
        assert(k.parent_tag==expected->parent_tag && k.kind_angle_word==expected->kind_angle_word && k.flags==expected->flags);
    }
    neo_contact_parent_t moving=throw_scene;moving.previous_x++;
    neo_contact_actor_t saved=a;assert(!neo_throw_free_step(&a,&moving,1,NULL));equal(&a,&saved);
    a.y=0;saved=a;assert(!neo_throw_free_step(&a,&throw_scene,1,NULL));equal(&a,&saved);
    printf("PASS: native spawn, targeted launch and %u persistent free-flight/contact updates\n",(unsigned)COUNT(throw_cases));
#ifdef N64
    debugf("MSX THROW REGRESSION PASS steps=%u\n",(unsigned)COUNT(throw_cases));
    for (;;) {wait_ms(100);}
#endif
}
