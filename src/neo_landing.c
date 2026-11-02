/* Parachute landing actors, manual translation of ROM 0C4A58..0C4B60 (the
 * canopy: constructor, descent, cut away and collapse) and
 * 0C5286..0C5320 (the player marker). Labels are provenance only.
 *   0C4A58  004256 with 0C4904, +30/+34 = 0, 01AD6E (on the parent's +28
 *           block), palette 0x38D (009EA8).
 *   0C4AAE  creator gone (0040AC: the port's player always exists) or its
 *           +5F bit 7 -> 0C4B20; parent on the ground (+15 bit 6) or +61 = 1
 *           -> 0C4B62; +61 = 2 -> 0C4B20; else +1E = the parent's, +28 word
 *           = -4 (+4 flipped), 01AC38, 00427A (then the player's 00427A and
 *           +7C: the port's player draws itself).
 *   0C4B20  +5F bit 3 (00436E: the poses show on even video phases only),
 *           004256 with 0C498C, 01AD52 (to the scene root, keeping the
 *           parent's motion as velocity), +34 = 0; 0C4B4A: 01AC38, 00427A,
 *           deleted at the end of the program.
 *   0C4B62  004256 with 0C49D4; 0C4B78: 01ACA2 (flip bits 0..1 from the
 *           parent), +1E = parent +1E - 1, 01AC38, 00427A; deleted when the
 *           program ends (the cartridge check 0C4B98 is not reproduced).
 *   0C5286  +66/+1E = FFFE; parent +70 = 0: palette 0x2A3 and 0C519E, else
 *           0x2A2 and 0C5212; 0C52E4 waits for the parent's ground bit;
 *           0C52FA: 012022, +3C/+40 words of the parent, 00427A, deleted when
 *           the program ends.
 *   0C4DD8  the mummy transformation's smoke (0C17D6, 0040BC after the
 *           player): palette 0xB0 (009EA8), 004256 with 2A97A0, +60 = 0,
 *           01AD6E (on the parent's +28 block), +6E = 0x10, sound 1085;
 *           0C4E1C: +1E = the parent's, +6E - 1; on its borrow (the 17th
 *           update) the parent's +60 = FF and 004188, else 01ABC8 and
 *           00427A. */
#include "neo_landing.h"
#include <string.h>
#define B(o) ga_b(a,(o))
#define W(o) ga_w(a,(o))
#define L(o) ga_l(a,(o))
#define SB(o,v) ga_sb(a,(o),(uint8_t)(v))
#define SW(o,v) ga_sw(a,(o),(uint16_t)(v))
#define SL(o,v) ga_sl(a,(o),(uint32_t)(v))
typedef struct {neo_landing_t *s;const neo_landing_input_t *in;neo_gactor_t *current;} ctx_t;
static bool g_parent(void *c,uint32_t tag,neo_contact_parent_t *out) {
    const neo_landing_input_t *in=((ctx_t *)c)->in;
    if (tag==NEO_LANDING_ROOT) {*out=(neo_contact_parent_t){tag,in->root_x,in->root_y,in->root_previous_x,in->root_previous_y,in->root_correction};return true;}
    if (tag!=NEO_LANDING_PLAYER) return false;
    *out=(neo_contact_parent_t){tag,in->x,in->y,in->previous_x,in->previous_y,in->correction};
    return true;
}
static bool g_front(void *c) {return ((ctx_t *)c)->in->front_layer;}
static void g_emit(void *c,const neo_anim_draw_t *d) {
    ctx_t *t=c;neo_landing_t *s=t->s;const neo_gactor_t *a=t->current;
    if (s->draw_count==NEO_LANDING_DRAWS || !a) return;
    if ((ga_b(a,0x5f)&8u) && t->in->odd_phase) return;                     /* 00436E */
    uint8_t h=(uint8_t)(d->attributes>>8);
    uint16_t resource=h>=1 && h<=4?a->palette[h-1]:0;
    s->draws[s->draw_count++]=(neo_end_draw_t){d->descriptor,d->x,d->y,d->attributes,resource,ga_b(a,0x12),ga_b(a,0x13)};
}
static void env(neo_genv_t *e,neo_ga_flags_t *flags,ctx_t *t,const neo_anim_program_t *(*program)(uint32_t)) {
    memset(e,0,sizeof(*e));memset(flags,0,sizeof(*flags));
    e->ctx=t;e->parent=g_parent;e->front_layer=g_front;e->emit=g_emit;e->emit_ctx=t;e->program=program;e->flags=flags;e->epoch=t->in->epoch;
}
#define START(tag) do { if (!ga_anim_start(a,e,(tag),0x100)) return GA_UNSUPPORTED; } while (0)
/* 01AD52: +28..+37 from the parent's block to the scene root (01AC4C). */
static void to_root(neo_gactor_t *a,neo_genv_t *e,const neo_landing_input_t *in) {
    uint32_t x=L(0x28),y=L(0x2c);
    uint32_t ex=x+L(0x30)+(uint32_t)in->x-(uint32_t)in->root_x,ey=y+L(0x34)+(uint32_t)in->y-(uint32_t)in->root_y;
    x+=(uint32_t)in->previous_x-(uint32_t)in->root_previous_x;y+=(uint32_t)in->previous_y-(uint32_t)in->root_previous_y;
    SL(0x28,x);SL(0x2c,y);SL(0x30,ex-x);SL(0x34,ey-y);SL(0x38,NEO_LANDING_ROOT);
    ga_project(a,e);
}
static ga_result_t canopy(neo_gactor_t *a,neo_genv_t *e,const neo_landing_input_t *in) {
    int r;
    switch (L(0)) {
    case 0xc4a58:
        START(0xc4904);SL(0x30,0);SL(0x34,0);
        ga_place(a,e,0,0,NEO_LANDING_PLAYER);ga_palette(a,0x38d);SL(0,0xc4aae);
        /* fall through */
    case 0xc4aae:
        if (in->flags5f&0x80u) goto cut;
        if (!(in->flags15&0x40u) && in->state61!=1) {
            if (in->state61==2) goto cut;
            SW(0x1e,in->z);SW(0x28,B(0x15)&1u?4u:0xfffcu);
            ga_move(a,e);
            return ga_anim_step(a,e,NULL)<0?GA_UNSUPPORTED:GA_MORE;
        }
        START(0xc49d4);SL(0,0xc4b78);
        /* fall through */
    case 0xc4b78:
        SB(0x15,(B(0x15)&0u)|(in->flags15&3u));                                  /* 01ACA2 */
        SW(0x1e,(uint16_t)(in->z-1u));
        ga_move(a,e);
        r=ga_anim_step(a,e,NULL);
        if (r<0) return GA_UNSUPPORTED;
        return r?GA_DELETE:GA_MORE;                                              /* 004188 */
    cut:
        SB(0x5f,B(0x5f)|8u);START(0xc498c);to_root(a,e,in);SL(0x34,0);SL(0,0xc4b4a);
        /* fall through */
    case 0xc4b4a:
        ga_move(a,e);
        r=ga_anim_step(a,e,NULL);
        if (r<0) return GA_UNSUPPORTED;
        return r?GA_DELETE:GA_MORE;
    }
    ga_fail(e,L(0));return GA_UNSUPPORTED;
}
static ga_result_t marker(neo_gactor_t *a,neo_genv_t *e,const neo_landing_input_t *in) {
    int r;
    switch (L(0)) {
    case 0xc5286:
        SW(0x66,0xfffe);SW(0x1e,0xfffe);
        if (!in->player) {ga_palette(a,0x2a3);START(0xc519e);}
        else {ga_palette(a,0x2a2);START(0xc5212);}
        SL(0,0xc52e4);
        /* fall through */
    case 0xc52e4:
        if (!(in->flags15&0x40u)) return GA_MORE;
        SL(0,0xc52fa);
        /* fall through */
    case 0xc52fa:
        if (in->front_layer) SB(0x15,B(0x15)|4u); else SB(0x15,B(0x15)&0xfbu);  /* 012022 */
        SW(0x3c,(uint16_t)((uint32_t)in->x>>16));SW(0x40,(uint16_t)((uint32_t)in->y>>16));
        r=ga_anim_step(a,e,NULL);
        if (r<0) return GA_UNSUPPORTED;
        return r?GA_DELETE:GA_MORE;                                              /* 004188 */
    }
    ga_fail(e,L(0));return GA_UNSUPPORTED;
}
static ga_result_t smoke(neo_gactor_t *a,neo_genv_t *e,const neo_landing_input_t *in,neo_landing_t *s) {
    switch (L(0)) {
    case 0xc4dd8:
        ga_palette(a,0xb0);START(0x2a97a0);SB(0x60,0);
        ga_place(a,e,0,0,NEO_LANDING_PLAYER);SW(0x6e,0x10);
        if (s->sound_count<2) s->sounds[s->sound_count++]=0x1085;
        SL(0,0xc4e1c);
        /* fall through */
    case 0xc4e1c:
        SW(0x1e,in->z);
        SW(0x6e,W(0x6e)-1u);
        if (W(0x6e)==0xffffu) {s->smoke_done=true;return GA_DELETE;}       /* 0C4E2E, 004188 */
        ga_project(a,e);                                                       /* 01ABC8 */
        return ga_anim_step(a,e,NULL)<0?GA_UNSUPPORTED:GA_MORE;
    }
    ga_fail(e,L(0));return GA_UNSUPPORTED;
}
static void node_init(neo_gactor_t *a,uint32_t routine) {
    memset(a,0,sizeof(*a));a->active=true;
    SL(0,routine);SL(0x0c,0x1014c4u);SB(0x12,0xff);SB(0x13,0xff);SW(0x1e,0xffff);SB(0x17,0xff);SW(0x58,0xffff);SB(0x69,0xff);SB(0x6a,0xff);SB(0x6b,0xff);
}
void neo_landing_start(neo_landing_t *s,const neo_landing_input_t *in,const neo_anim_program_t *(*program)(uint32_t tag)) {
    (void)program;
    memset(s,0,sizeof(*s));
    neo_gactor_t *a=&s->canopy;node_init(a,0xc4aae);
    /* 0C4A7C/0C4AFA: placed on the player (01AD6E, +38 = his +28 block),
     * +28 = -4 (+4 flipped) relative, palette 0x38D. */
    SB(0x15,in->flags15&3u);SL(0x28,(uint32_t)(int32_t)(B(0x15)&1u?4:-4)<<16);SL(0x38,NEO_LANDING_PLAYER);SW(0x1e,in->z);
    ga_palette(a,0x38d);
    node_init(&s->marker,0xc5286);
}
void neo_landing_descent(neo_landing_t *s) {
    memset(s,0,sizeof(*s));node_init(&s->canopy,0xc4a58);
}
void neo_landing_marker(neo_landing_t *s) {
    if (!s->marker.active) node_init(&s->marker,0xc5286);
}
void neo_landing_smoke(neo_landing_t *s) {
    node_init(&s->smoke,0xc4dd8);s->smoke_done=false;
}
bool neo_landing_active(const neo_landing_t *s) {return s->canopy.active || s->marker.active || s->smoke.active;}
void neo_landing_update(neo_landing_t *s,const neo_landing_input_t *in,const neo_anim_program_t *(*program)(uint32_t tag)) {
    s->draw_count=0;s->marker_draws=0;s->sound_count=0;
    /* List order: the player, then what 0040BC put right after him (the
     * newest first): the marker (0C186E) comes after the smoke's end. */
    neo_gactor_t *order[3]={&s->marker,&s->smoke,&s->canopy};
    for (unsigned i=0;i<3;i++) {
        neo_gactor_t *a=order[i];
        if (!a->active || a->placeholder) continue;
        ctx_t t={s,in,a};neo_genv_t e;neo_ga_flags_t flags;env(&e,&flags,&t,program);
        ga_result_t r=a==&s->canopy?canopy(a,&e,in):a==&s->smoke?smoke(a,&e,in,s):marker(a,&e,in);
        if (r==GA_UNSUPPORTED) {
            if (!s->unsupported++) s->unsupported_at=e.unsupported_at?e.unsupported_at:ga_l(a,0);
            a->placeholder=true;continue;
        }
        if (r==GA_DELETE) {ga_sl(a,0,0x3f1e);ga_sb(a,0x1d,0);a->placeholder=true;}   /* 004188 -> 004126 */
        if (s->on_updated) s->on_updated(s->ctx,a);
        if (r==GA_DELETE) a->active=false;
        if (a!=&s->canopy) s->marker_draws=s->draw_count;
    }
}
