#include "neo_end.h"
#include <string.h>
#include "neo_soldier_ai.h"   /* neo_angle (ROM 002EB4) */
/* MISSION COMPLETE letters 06090A, manual translation of ROM 06090A..060F42
 * (ROM labels are provenance only). Called by the mission actor at 0188DE
 * (0040BC, A6 = 101364).
 *
 * Root 06090A: missions >= 6 delete it; +7D = 1, at (0, 0x1FF) on the screen
 * (01AB98 without parent), +66/+1E = FFFF, +72 = 0. Mission 5 creates the
 * letters of the table 60728 (060E3C), the others the mission number 060B06
 * (target F0,1A8, delay 0x0E) and the letters of 606A6 (060E02). 060D86: per
 * 8-byte record a letter 060A72 (01B220 from the root) with +60 program,
 * +61, +72 delay and the target +6E/+70. Then it waits for +61 (each letter
 * adds 1 on arrival) to reach 0x11 (0x15 in mission 5), counts +72 by 2 up
 * to 0x26 (the letters with +61 != 0 blink by its bit 3), sets +60 = FF (the
 * letters leave) and the mission actor's +60 = 1 (0188B0 / 0188F4 read it),
 * and deletes itself (004188). The cartridge check of 0609D4 is not
 * reproduced (it only acts on a failed check).
 *
 * Letter 060A72 (number 060B06): at (0, 0x1FF) on the screen, +74 = +72,
 * 012022 (+15 bit 2: front layer); 060A98 waits +72 updates (subq/bmi), then
 * palette 0x3BE, +15 bit 0, +76 = 0x1FF, +13 = FF, +7C = 0, +66/+1E = FFFF
 * (the number keeps its own), 01B1DC, the program 60F44[+60] (number:
 * 60F7C[mission]); +7A = 0x18 (root +7D set) else 0x30. 060BAA: velocity a
 * quarter of the distance to the target (060E44; 060E7A with the clamps when
 * +7D = 0), 01AC38, 00427A, 060EB0: inside 0x30 px (01B29C) +76 shrinks by
 * +7A and +12 = its low byte (negated below 0x100; the first time palette
 * 0x3AA, +15 bits 0/1 cleared); +76 = 0 is the arrival: palette 0x200,
 * 060C2A +14 = +1A, root +61 + 1, +12/+13 = FF. 060C4A animates unless +61
 * and root +72 bit 3; root +60 = FF starts 060C8C: speed +78 += 0x1FF (max
 * 0x7FFF) towards the angle from (A0,190) + 0x80 (002EB4/002E7C), 01AC38,
 * 00427A, deleted by 01945C with the rectangle 60F94. */
#define B(o) ga_b(a,(o))
#define W(o) ga_w(a,(o))
#define L(o) ga_l(a,(o))
#define SB(o,v) ga_sb(a,(o),(uint8_t)(v))
#define SW(o,v) ga_sw(a,(o),(uint16_t)(v))
#define SL(o,v) ga_sl(a,(o),(uint32_t)(v))
#define ROOT_TAG 0x10e968u
typedef struct {neo_end_t *s;const neo_end_inputs_t *in;int current;} ctx_t;
static uint16_t rw(uint32_t address) {const uint8_t *p=ga_rom(address);return p?(uint16_t)(p[0]<<8|p[1]):0;}
static uint32_t rl(uint32_t address) {return (uint32_t)rw(address)<<16|rw(address+2);}
static int index_of(const neo_end_t *s,const neo_gactor_t *a) {return (int)(a-s->nodes);}
static int order_of(const neo_end_t *s,int index) {
    for (unsigned i=0;i<s->count;i++) if (s->order[i]==index) return (int)i;
    return -1;
}
static neo_gactor_t *alloc(neo_end_t *s,int creator,uint32_t routine) {
    unsigned i;for (i=0;i<NEO_END_NODES && s->nodes[i].active;i++) {}
    if (i==NEO_END_NODES || s->count==NEO_END_NODES) {s->unsupported++;if (!s->unsupported_at) s->unsupported_at=0x40bc;return NULL;}
    neo_gactor_t *a=&s->nodes[i];memset(a,0,sizeof(*a));a->index=(int16_t)i;a->active=true;
    SL(0,routine);SL(0x0c,creator<0?0x101364u:NEO_GA_TAG((unsigned)creator));
    SB(0x12,0xff);SB(0x13,0xff);SW(0x1e,0xffff);SB(0x17,0xff);SW(0x58,0xffff);SB(0x69,0xff);SB(0x6a,0xff);SB(0x6b,0xff);
    memset(a->written,0,sizeof(a->written));
    /* 0040BC: inserted after its creator (the mission actor heads this list). */
    int at=creator<0?-1:order_of(s,creator);
    memmove(&s->order[at+2],&s->order[at+1],(s->count-(unsigned)(at+1))*sizeof(s->order[0]));
    s->order[at+1]=(uint8_t)i;s->count++;s->creator[i]=(int8_t)creator;
    if (s->spawned<NEO_END_NODES) s->spawn_order[s->spawned]=(int8_t)i;
    if (s->on_alloc) s->on_alloc(s->on_alloc_ctx,a,s->spawned);
    s->spawned++;
    return a;
}
void neo_end_reset(neo_end_t *s) {memset(s,0,sizeof(*s));}
neo_gactor_t *neo_end_start(neo_end_t *s,uint32_t routine) {return alloc(s,-1,routine);}
bool neo_end_routine(uint32_t r) {return r>=0x6090a && r<=0x60c8c;}
/* ---- environment ----------------------------------------------------------- */
static bool g_parent(void *c,uint32_t tag,neo_contact_parent_t *out) {
    ctx_t *t=c;
    if (tag==ROOT_TAG) {*out=t->in->root;return true;}
    if ((tag&0xff000000u)==NEO_GA_TAG_BASE && (tag&0xffffu)<NEO_END_NODES) {
        const neo_gactor_t *a=&t->s->nodes[tag&0xffffu];
        *out=(neo_contact_parent_t){tag,(int32_t)ga_l(a,0x3c),(int32_t)ga_l(a,0x40),(int32_t)ga_l(a,0x44),(int32_t)ga_l(a,0x48),ga_b(a,0x4d)};
        return true;
    }
    return false;
}
static neo_gactor_t *g_spawn(void *c,neo_gactor_t *creator,uint32_t routine,bool after_script) {
    ctx_t *t=c;(void)after_script;
    return alloc(t->s,creator?index_of(t->s,creator):-1,routine);
}
static neo_gactor_t *g_creator(void *c,neo_gactor_t *a) {
    ctx_t *t=c;int k=t->s->creator[index_of(t->s,a)];
    return k<0?NULL:&t->s->nodes[k];
}
static bool g_front(void *c) {return ((ctx_t *)c)->in->front_layer;}
static void g_emit(void *c,const neo_anim_draw_t *d) {
    ctx_t *t=c;neo_end_t *s=t->s;
    if (s->draw_count==NEO_END_DRAWS || t->current<0) return;
    const neo_gactor_t *a=&s->nodes[t->current];
    /* 0043D4: palette nibble >= 4 draws with +14, else +19..+1C; native
     * handles are slot+1 (ga_palette). */
    uint8_t h=(uint8_t)(d->attributes>>8);
    uint16_t resource=h>=1 && h<=4?a->palette[h-1]:0;
    s->draws[s->draw_count++]=(neo_end_draw_t){d->descriptor,d->x,d->y,d->attributes,resource,ga_b(a,0x12),ga_b(a,0x13)};
}
/* ---- letters 06090A --------------------------------------------------------- */
#define START(t) do { if (!ga_anim_start(a,e,(t),0x100)) return GA_UNSUPPORTED; } while (0)
#define STEP() do { if (ga_anim_step(a,e,NULL)<0) return GA_UNSUPPORTED; } while (0)
/* 060D86: a letter 060A72 per 8-byte record up to FFFF. */
static void letters(neo_gactor_t *a,neo_genv_t *e,uint32_t table) {
    for (uint32_t p=table;!(rw(p)&0x8000u);p+=8) {
        neo_gactor_t *c=e->spawn(e->ctx,a,0x60a72,false);
        if (!c) return;
        ga_place_from(c,a,e,0,0);
        ga_sb(c,0x60,(uint8_t)(rw(p)>>8));ga_sb(c,0x61,(uint8_t)rw(p));
        ga_sw(c,0x72,rw(p+2));ga_sw(c,0x6e,rw(p+4));ga_sw(c,0x70,rw(p+6));
    }
}
/* 01B29C: max + min/2 of the absolute differences. */
static int16_t distance(uint16_t x0,uint16_t y0,uint16_t x1,uint16_t y1) {
    uint16_t d0=(uint16_t)(x0-x1),d1=(uint16_t)(y0-y1);
    if (d0&0x8000u) d0=(uint16_t)-d0;
    if (d1&0x8000u) d1=(uint16_t)-d1;
    if ((int16_t)d0>(int16_t)d1) return (int16_t)(d0+(d1>>1));
    return (int16_t)((d0>>1)+d1);
}
/* 060EB0: carry = arrived. */
static bool approach(neo_gactor_t *a) {
    if (!W(0x76)) return true;
    if (distance(W(0x3c),W(0x40),W(0x6e),W(0x70))<=0x30) {
        uint16_t v=(uint16_t)(W(0x76)-W(0x7a));SW(0x76,v);
        if (v&0x8000u) {SW(0x76,0);return true;}
    }
    uint8_t d0=(uint8_t)W(0x76);
    if (!(W(0x76)&0x100u)) d0=(uint8_t)-d0;
    SB(0x12,d0);
    if (!(W(0x76)&0x100u) && !B(0x7c)) {ga_palette(a,0x3aa);SB(0x7c,0xff);SB(0x15,B(0x15)&0xfcu);}
    return false;
}
static ga_result_t tick(neo_gactor_t *a,neo_genv_t *e,const neo_end_inputs_t *in) {
    neo_gactor_t *p=e->creator(e->ctx,a);
    switch (L(0)) {
    case 0x6090a:
        if (in->mission>=6) return GA_DELETE;
        SB(0x7d,1);ga_place(a,e,0,0x1ff,0);SW(0x66,0xffff);SW(0x1e,0xffff);SW(0x72,0);
        if ((uint8_t)in->mission==5) letters(a,e,0x60728);                        /* 060E3C */
        else {                                                                     /* 060E02 */
            neo_gactor_t *c=e->spawn(e->ctx,a,0x60b06,false);
            if (c) {ga_place_from(c,a,e,0,0);ga_sb(c,0x61,0);ga_sw(c,0x72,0x0e);ga_sw(c,0x6e,0xf0);ga_sw(c,0x70,0x1a8);}
            letters(a,e,0x606a6);
        }
        SB(0x61,0);SB(0x60,0);SL(0,0x60974);
        /* fall through */
    case 0x60974:
        if (B(0x61)==((uint8_t)in->mission==5?0x15:0x11)) SL(0,0x6099e);
        return GA_MORE;
    case 0x6099e:
        SL(0,0x609a4);
        /* fall through */
    case 0x609a4:
        SW(0x72,W(0x72)+2u);
        if ((int16_t)W(0x72)>=0x26) SL(0,0x609ba);
        return GA_MORE;
    case 0x609ba:
        SB(0x60,0xff);
        return GA_DELETE;   /* 060A62 / 0609CA: the creator's +60 = 1 (neo_end_update) */
    case 0x60a72:
    case 0x60b06:
        ga_place(a,e,0,0x1ff,0);SW(0x74,W(0x72));
        if (e->front_layer(e->ctx)) SB(0x15,B(0x15)|4u); else SB(0x15,B(0x15)&0xfbu);   /* 012022 */
        SL(0,L(0)==0x60a72?0x60a98:0x60b2c);
        /* fall through */
    case 0x60a98:
    case 0x60b2c: {
        bool number=L(0)==0x60b2c;
        SW(0x72,W(0x72)-1u);
        if (!(W(0x72)&0x8000u)) return GA_MORE;
        ga_palette(a,0x3be);SB(0x15,B(0x15)|1u);SW(0x76,0x1ff);SB(0x13,0xff);SB(0x7c,0);
        if (!number) {SW(0x66,0xffff);SW(0x1e,0xffff);}
        SL(0x30,0);SL(0x34,0);SL(0x62,0);                                         /* 01B1DC */
        if (number) START(rl(0x60f7c+4u*(in->mission<=5?in->mission:0u)));
        else START(rl(0x60f44+4u*(B(0x60)>0x0d?0u:B(0x60))));
        SW(0x7a,p && ga_b(p,0x7d)?0x18:0x30);SL(0,0x60baa);
    }
        /* fall through */
    case 0x60baa:
        if (p && ga_b(p,0x7d)) {                                                   /* 060E44 */
            uint16_t d0=(uint16_t)(W(0x6e)-W(0x3c)),d1=(uint16_t)(W(0x70)-W(0x40));
            if (d0 || d1) {
                SL(0x30,(uint32_t)(int32_t)(int16_t)(uint16_t)(d0<<6)<<8);
                SL(0x34,(uint32_t)(int32_t)(int16_t)(uint16_t)(d1<<6)<<8);
            }
        } else {                                                                   /* 060E7A */
            uint16_t d0=(uint16_t)(W(0x6e)-W(0x3c)),d1=(uint16_t)(W(0x70)-W(0x40));
            if (d0 || d1) {
                SL(0x30,(uint32_t)d0*0xc000u);
                SL(0x34,0u-(uint32_t)(uint16_t)(0u-d1)*0xc000u);
            }
            uint32_t x=L(0x3c)+L(0x30),tx=(uint32_t)W(0x6e)<<16;
            if ((int32_t)x>(int32_t)tx) SL(0x30,tx-L(0x3c));
            uint32_t y=L(0x40)+L(0x34),ty=(uint32_t)W(0x70)<<16;
            if ((int32_t)y<(int32_t)ty) SL(0x34,ty-L(0x40));
        }
        ga_move(a,e);STEP();
        if (approach(a)) {ga_palette(a,0x200);SL(0,0x60c2a);}
        return GA_MORE;
    case 0x60c2a:
        SB(0x14,B(0x1a));
        if (p) ga_sb(p,0x61,(uint8_t)(ga_b(p,0x61)+1u));
        SB(0x13,0xff);SB(0x12,0xff);SL(0,0x60c4a);
        /* fall through */
    case 0x60c4a:
        if (!B(0x61) || !(p && (ga_w(p,0x72)&8u))) STEP();
        if (p && ga_b(p,0x60)==0xff) SL(0,0x60c80);
        return GA_MORE;
    case 0x60c80:
        SW(0x78,0);SL(0,0x60c8c);
        /* fall through */
    case 0x60c8c: {
        SW(0x78,W(0x78)+0x1ffu);
        if (W(0x78)&0x8000u) SW(0x78,0x7fff);
        uint8_t angle=(uint8_t)(neo_angle((int16_t)(uint16_t)(0xa0u-W(0x3c)),(int16_t)(uint16_t)(0x190u-W(0x40)))+0x80u);
        uint32_t vx,vy;ga_polar((uint32_t)(int32_t)(int16_t)W(0x78)<<8,angle,&vx,&vy);
        SL(0x30,vx);SL(0x34,vy);
        ga_move(a,e);STEP();
        if (ga_cull(a,e,0x60f94)) return GA_DELETE;                               /* 01945C */
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    }
    }
    ga_fail(e,L(0));return GA_UNSUPPORTED;
}
void neo_end_update(neo_end_t *s,const neo_end_inputs_t *in) {
    ctx_t t={s,in,-1};
    neo_ga_flags_t flags;memset(&flags,0,sizeof(flags));
    neo_genv_t e;memset(&e,0,sizeof(e));
    e.epoch=in->epoch;e.bounds=in->bounds;e.ctx=&t;e.parent=g_parent;e.program=in->program;e.emit=g_emit;e.emit_ctx=&t;
    e.spawn=g_spawn;e.creator=g_creator;e.front_layer=g_front;e.flags=&flags;
    s->draw_count=0;
    for (unsigned i=0;i<s->count;i++) {
        int n=s->order[i];neo_gactor_t *a=&s->nodes[n];
        if (!a->active || a->placeholder) continue;
        t.current=n;e.unsupported=0;
        ga_result_t r=neo_end_routine(ga_l(a,0))?tick(a,&e,in):
            neo_tally_routine(ga_l(a,0))?neo_tally_tick(s,a,&e,in->ram):(ga_fail(&e,ga_l(a,0)),GA_UNSUPPORTED);
        if (r==GA_UNSUPPORTED) {
            /* Explicit stop: the node stays frozen, counted once. */
            if (!s->unsupported) s->unsupported_at=e.unsupported_at;
            s->unsupported++;a->placeholder=true;continue;
        }
        if (r==GA_DELETE) {
            /* 004188: routine 003F1E; the bytes stay (read by its children). */
            if (ga_l(a,0)==0x609ba) {
                if (s->creator[n]<0) s->mission60=1; else ga_sb(&s->nodes[s->creator[n]],0x60,1);
            }
            ga_sl(a,0,0x3f1e);a->placeholder=true;
        }
        if (s->on_updated) s->on_updated(s->on_alloc_ctx,a);
    }
    /* The tally root's last update set the mission actor's +60 (030A6E). */
    if (in->ram && in->ram->mission60) {s->mission60=1;in->ram->mission60=0;}
    /* Nodes deleted with no node created after them leave the list. */
    for (unsigned i=s->count;i-->0;) {
        int n=s->order[i];
        bool referenced=false;
        for (unsigned j=0;j<s->count;j++) {int m=s->order[j];if (s->creator[m]==n && !s->nodes[m].placeholder) referenced=true;}
        if (s->nodes[n].placeholder && ga_l(&s->nodes[n],0)==0x3f1e && !referenced) {
            s->nodes[n].active=false;
            memmove(&s->order[i],&s->order[i+1],(s->count-i-1)*sizeof(s->order[0]));s->count--;
        }
    }
}
