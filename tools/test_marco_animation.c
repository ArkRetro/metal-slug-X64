/* Synthetic edge cases, separate from measured whole-helper journals. */
#include <assert.h>
#include <stdio.h>
#include "../src/marco_animation.h"
typedef struct {unsigned draws,events;neo_anim_draw_t draw;} output_t;
static void draw(void *context,const neo_anim_draw_t *d) {
    output_t *out=context;out->draws++;out->draw=*d;
}
static bool move_event(void *context,neo_anim_kind_t kind,uint32_t id,neo_anim_render_t *r) {
    output_t *out=context;assert(kind==NEO_ANIM_EVENT && id==7);out->events++;
    r->x=(uint16_t)(r->x+5);r->y=(uint16_t)(r->y-3);return true;
}
int main(void) {
    marco_animation_t a;marco_animation_reset(&a);output_t out={0};
    neo_anim_command_t commands[]={
        {.kind=NEO_ANIM_WRITE_LONG,.target=0x72,.asset=0x100020},
        {.kind=NEO_ANIM_EVENT,.asset=7},
        {.kind=NEO_ANIM_POSE,.asset=19,.flags=0x40},
        {.kind=NEO_ANIM_WAIT,.operand=0},
    };
    neo_anim_program_t program={commands,4};
    a.flash=4;a.field18=9;a.field50_tag=123;
    assert(marco_animation_start(&a,MARCO_WEAPON,&program,0,256) && a.field18==9);
    assert(marco_animation_start(&a,MARCO_BODY,&program,0,256) && !a.field18);
    assert(a.flash==4 && a.field50_tag==123);
    a.fields.weapon_dx=3;a.fields.weapon_dy=0xfffc;
    marco_animation_view_t v={.render={.x=1,.y=2,.attributes=3},.cached_x=0,.cached_y=65535};
    assert(marco_animation_step(&a,MARCO_WEAPON,&v,draw,&out,32,move_event,&out)==NEO_ANIM_MORE);
    /* Mirror produces dx=-3,dy=+4; event moves projected coords. Offset
     * restoration must use the old offsets, not the freshly written 16/32. */
    assert(v.render.x==6 && v.render.y==65535 && v.cached_x==0 && v.cached_y==65535);
    assert(out.draws==1 && out.events==1 && out.draw.x==3 && out.draw.y==3);
    assert(a.fields.weapon_dx==16 && a.fields.weapon_dy==32 && a.flash==3);
    assert(marco_animation_step(&a,MARCO_BODY,&v,draw,&out,32,move_event,&out)==NEO_ANIM_MORE && a.flash==2);
    assert(marco_animation_start(&a,MARCO_WEAPON,NULL,0,256));
    assert(marco_animation_step(&a,MARCO_WEAPON,&v,draw,&out,32,NULL,NULL)==NEO_ANIM_COMPLETE && a.flash==2);
    assert(!marco_animation_start(&a,MARCO_WEAPON,&program,4,256));
    assert(!marco_animation_start(&a,(marco_part_t)9,&program,0,256));
    /* A failed effect must stop the actor; retry may not repeat earlier ones. */
    neo_anim_command_t failure[]={ {.kind=NEO_ANIM_EVENT,.asset=7}, {.kind=NEO_ANIM_UNRECOVERED} };
    program=(neo_anim_program_t){failure,2};
    assert(marco_animation_start(&a,MARCO_BODY,&program,0,256));
    unsigned old=out.events;
    assert(marco_animation_step(&a,MARCO_BODY,&v,NULL,NULL,32,move_event,&out)==NEO_ANIM_UNSUPPORTED);
    assert(out.events==old+1 && a.failed);
    assert(marco_animation_step(&a,MARCO_BODY,&v,NULL,NULL,32,move_event,&out)==NEO_ANIM_UNSUPPORTED && out.events==old+1);
    assert(!marco_animation_start(&a,MARCO_BODY,&program,0,256));
    marco_animation_reset(&a);assert(marco_animation_start(&a,MARCO_BODY,&program,0,256));
    assert(marco_animation_step(&a,MARCO_BODY,&v,NULL,NULL,32,NULL,NULL)==NEO_ANIM_UNSUPPORTED);
    marco_animation_reset(&a);
    assert(marco_animation_step(&a,MARCO_BODY,NULL,NULL,NULL,32,NULL,NULL)==NEO_ANIM_INVALID && a.failed);
    puts("PASS: synthetic paired clocks, shared fields, latched anchors and failure boundaries");
}
