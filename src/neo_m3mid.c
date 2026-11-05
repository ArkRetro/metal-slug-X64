#include "neo_region.h"
#if !defined(N64) || defined(MSX_M3MID)
/* Mission 3: the mid-boss aircraft 0A4A94 (script record 2DAE20, camera
 * 3944), manual translation of 0A4A94..0A7140 (ROM labels as provenance;
 * docs/JEFE_MISION3.md, "Jefe intermedio").
 * - Body 0A4A94: its picture is the fourth layer object 10E8C6 (map 2A6142,
 *   0A6C18: 0C7BD0, 0C6D04, 0C703C, 0C6F34, 0C73CE, 0C6CFC), the body sits
 *   on it at (0x98, 0) and moves it: +7E/+82 is the layer's target, the
 *   layer's +8/+C the step (0A5B3C). Health 2C001A (3000), phase 2 below
 *   2C0096 (0A6BB2), hurtbox 2A5FDE, palette slot 0x5E (0A56D8). It comes
 *   down (0A4B52) and then repeats the cycle 2A610A (0A5882..0A59CE: rise
 *   and drift to the player, sounds 10EB/10EC) with the sway 0A5796. Every
 *   100 points of damage a stage of 2A6340 (0A6BE2) sets a bit of +74:
 *   0A5C04 patches the layer (0A6D7C -> 0C714E, lists 2A6210), throws four
 *   debris 0A6598 and lights the engines 0A5D6A (stages 0 and 10).
 * - Side pieces 0A606E/0A6078 (programs E61BC/E61C8 by the stages), the
 *   guns 0A61E8/0A61F2 (health 2C007C, hurtbox 2A5FF2, beam 2A5FAE while
 *   their parts set +7F, cycle 0A6260..0A6326 on the body's +A2 bits,
 *   points 08D1D0 when killed), parts 0A64E2 (table 2A601A), the hatch
 *   0A5E88 and, in phase 2, the bay 0A4DB6 dropping 0A4F3A/0A4F52 (health
 *   2C0C0A, bombs 0A5398 by the callback 0A530C, items 2C0C6C).
 * - Death 0A4BDA..0A4D52: 0A4DB0 (kill request 004BFE), debris lists
 *   2A6526/2A647A, the wreck map 2A619A (0A6F30), the white screen
 *   0AE220(3, 1), 004192, 30 items 55 (0A5670), the layer released
 *   (0A5604) and 0041AC.
 * The layer object's header (RAM 10E8C6..10E94D) is e->m3mid->layer; its
 * drawing (SCB writes of 0C6D04/0C703C/0C714E) is the renderer's: the map
 * and the patches go through e->m3mid->draw. Untraced branches stop
 * explicitly: the second cycle table 2A6126 (+A3 is only ever 0), the
 * hatch's opening 0A5EDA (body +A2 bit 3, set only by that table), the
 * debug traps of 0A6918/0A6A32. The cartridge check before the deletions
 * (10F00A/10F00C) is not reproduced. */
#define B(o) ga_b(a,(o))
#define W(o) ga_w(a,(o))
#define L(o) ga_l(a,(o))
#define SB(o,v) ga_sb(a,(o),(uint8_t)(v))
#define SW(o,v) ga_sw(a,(o),(uint16_t)(v))
#define SL(o,v) ga_sl(a,(o),(uint32_t)(v))
#define BIT(o,b) ((B(o)>>(b))&1u)
#define SET(o,b) SB(o,B(o)|(1u<<(b)))
#define CLR(o,b) SB(o,B(o)&~(1u<<(b)))
#define S8(v) ((int8_t)(uint8_t)(v))
#define S16(v) ((int16_t)(uint16_t)(v))
#define STOP(at) do { ga_fail(e,(at)); return GA_UNSUPPORTED; } while (0)
#define START(t) do { if (!ga_anim_start(a,e,(t),0x100)) return GA_UNSUPPORTED; } while (0)
#define LAYER 0x10e8c6u
#define ROOT 0x10e968u
static uint8_t rb(neo_genv_t *e,uint32_t at) {const uint8_t *p=ga_rom(at);if (!p) {ga_fail(e,at);return 0;}return p[0];}
static uint16_t rw(neo_genv_t *e,uint32_t at) {const uint8_t *p=ga_rom(at);if (!p) {ga_fail(e,at);return 0;}return (uint16_t)(p[0]<<8|p[1]);}
static uint32_t rl(neo_genv_t *e,uint32_t at) {return (uint32_t)rw(e,at)<<16|rw(e,at+2);}
static void sound(neo_genv_t *e,uint16_t id) {if (e->sound) e->sound(e->ctx,id);}
static void stop_sound(neo_genv_t *e,uint16_t id) {sound(e,(uint16_t)(id|0x8000u));}   /* 012192 */
/* 0260C8 (type 2 tables are random: the boss's rank words). */
static uint16_t rank(neo_gactor_t *a,neo_genv_t *e,uint32_t table) {
    const uint8_t *t=ga_rom(table);
    if (!t) {ga_fail(e,table);return 0;}
    if (t[0]!=2u) return ga_rank_word(a,e,t);
    unsigned coop=e->players.active_mask==3u;
    unsigned rk=e->players.mode88==2?e->players.rank:3u;
    unsigned mask=(1u<<((rk&7u)+1u))-1u,row=0,count=0;
    for (unsigned b=t[1]&mask&0xffu;b;b&=b-1u) row++;
    for (unsigned b=t[1];b;b&=b-1u) count++;
    if (!row) {ga_fail(e,0x26118);return 0;}
    unsigned d0=((row-1u)*2u+coop)*6u+B(0x68);
    unsigned o=2u+(unsigned)t[2+d0]*16u+count*12u+(ga_rand(e)&0xeu);
    return rw(e,table+o);
}
static neo_gactor_t *creator(neo_gactor_t *a,neo_genv_t *e) {return e->creator?e->creator(e->ctx,a):NULL;}
/* The body: +6E of every node of the family (0A6CF0: the body's own). */
static neo_gactor_t *body(neo_gactor_t *a,neo_genv_t *e) {return e->actor?e->actor(e->ctx,L(0x6e)):NULL;}
/* 0040BC with the reserve D0 and, on success, +6E from the creator. */
static neo_gactor_t *child(neo_gactor_t *a,neo_genv_t *e,uint32_t routine,uint16_t reserve) {
    neo_gactor_t *c=e->spawn_reserve?e->spawn_reserve(e->ctx,a,routine,reserve):e->spawn?e->spawn(e->ctx,a,routine,false):NULL;
    if (c) ga_sl(c,0x6e,L(0x6e));
    return c;
}
/* 00411C after the script actor 101414. */
static neo_gactor_t *after_script(neo_genv_t *e,uint32_t routine) {return e->spawn?e->spawn(e->ctx,NULL,routine,true):NULL;}
/* 01AD6E: (dx, dy) relative to the creator's block. */
static void on_creator(neo_gactor_t *a,neo_genv_t *e,int16_t dx,int16_t dy) {neo_gactor_t *p=creator(a,e);ga_place(a,e,dx,dy,p?NEO_GA_TAG(p->index):0);}
static void k1b1dc(neo_gactor_t *a) {SL(0x30,0);SL(0x34,0);SL(0x62,0);}
/* 01AD52: the block re-parented to the scene root (01AC4C), projected. */
static void to_root(neo_gactor_t *a,neo_genv_t *e) {
    uint32_t tag=L(0x38);
    if (tag!=ROOT) {
        neo_contact_parent_t op,np;bool has_old=tag && ga_parent(e,tag,&op),has_new=ga_parent(e,ROOT,&np);
        uint32_t d0=L(0x28),d1=L(0x2c),d2=L(0x30),d3=L(0x34);
        d2+=d0;d3+=d1;
        if (has_old) {d0+=(uint32_t)op.previous_x;d1+=(uint32_t)op.previous_y;d2+=(uint32_t)op.x;d3+=(uint32_t)op.y;}
        if (has_new) {d0-=(uint32_t)np.previous_x;d1-=(uint32_t)np.previous_y;d2-=(uint32_t)np.x;d3-=(uint32_t)np.y;}
        d2-=d0;d3-=d1;
        SL(0x28,d0);SL(0x2c,d1);SL(0x30,d2);SL(0x34,d3);SL(0x38,ROOT);
    }
    ga_project(a,e);
}
/* 01DA40: the explosion list emitter 01D764 (+86 = list) with the layer
 * word `layer` (the callers' +66/+1E). */
static neo_gactor_t *debris_list(neo_gactor_t *a,neo_genv_t *e,uint32_t list,uint16_t layer) {
    neo_gactor_t *c=e->spawn_reserve?e->spawn_reserve(e->ctx,a,0x1d764,0x1c):e->spawn?e->spawn(e->ctx,a,0x1d764,false):NULL;
    if (!c) return NULL;
    ga_place_from(c,a,e,0,0);
    if (B(0x15)&4u) ga_sb(c,0x15,(uint8_t)(ga_b(c,0x15)|4u));
    ga_sl(c,0x86,list);ga_sw(c,0x66,layer);ga_sw(c,0x1e,layer);
    return c;
}
/* 0A6918: d0 * (d2 - d3) / d1 + d3 with the sign of d0 (0 stays 0). The
 * trap "GENSOKU_SPEED_ERR" (d2 < d3) and a divu overflow stop. */
static int16_t scale(neo_genv_t *e,int16_t d0,uint16_t d1,uint16_t d2,uint16_t d3) {
    if (d2<d3) {ga_fail(e,0xa691e);return 0;}
    uint16_t m=(uint16_t)(d0<0?-d0:d0);
    uint32_t p=(uint32_t)m*(uint16_t)(d2-d3);
    uint32_t q=p/d1;
    if (q>0xffffu) {ga_fail(e,0xa693e);return 0;}
    if (!q) return 0;
    uint16_t r=(uint16_t)(q+d3);
    return (int16_t)(d0<0?(uint16_t)-r:r);
}
/* ---- the layer object 10E8C6 ---------------------------------------------- */
static neo_m3mid_env_t *menv(neo_genv_t *e) {if (!e->m3mid || !e->m3mid->layer) {ga_fail(e,LAYER);return NULL;}return e->m3mid;}
static uint16_t lw(const uint8_t *l,unsigned o) {return (uint16_t)(l[o]<<8|l[o+1]);}
static uint32_t ll(const uint8_t *l,unsigned o) {return (uint32_t)lw(l,o)<<16|lw(l,o+2);}
static void lsw(uint8_t *l,unsigned o,uint16_t v) {l[o]=(uint8_t)(v>>8);l[o+1]=(uint8_t)v;}
static void lsl(uint8_t *l,unsigned o,uint32_t v) {lsw(l,o,(uint16_t)(v>>16));lsw(l,o+2,(uint16_t)v);}
/* 01AB9C on the layer with the scene root as parent, then 01ABCC. */
static void layer_place(neo_genv_t *e,uint8_t *l,int16_t x,int16_t y) {
    lsl(l,0,(uint32_t)(int32_t)x<<16);lsl(l,4,(uint32_t)(int32_t)y<<16);lsl(l,0x10,ROOT);l[0x24]=0;l[0x25]=0;   /* +24 word */
    neo_contact_parent_t r;
    if (!ga_parent(e,ROOT,&r)) return;
    uint32_t d0=ll(l,0),d1=ll(l,4),d2=ll(l,0x14),d3=ll(l,0x18);
    uint8_t d7=0;
    if ((uint32_t)(uint16_t)d1+(uint16_t)r.y>0xffffu) d7=1;
    d7=(uint8_t)(d7<<1|(((uint32_t)(uint16_t)d0+(uint16_t)r.x>0xffffu)?1u:0u));
    d0+=(uint32_t)r.x;d1+=(uint32_t)r.y;d7|=r.correction;
    l[0x25]=d7;lsl(l,0x14,d0);lsl(l,0x18,d1);
    uint8_t epoch=e->epoch;
    if (!l[0x24]) {l[0x24]=epoch;lsl(l,0x1c,d0);lsl(l,0x20,d1);}
    else if (l[0x24]!=epoch) {l[0x24]=epoch;lsl(l,0x1c,d2);lsl(l,0x20,d3);}
}
/* 0C6F34: map descriptor `map`, block offsets (12-bit), column list. */
static void k0c6f34(uint8_t *l,uint32_t map,uint16_t d0,uint16_t d1,uint32_t list) {
    lsl(l,0x2e,map);
    d0=(uint16_t)(S16((uint16_t)(d0<<4))>>4);d1=(uint16_t)(S16((uint16_t)(d1<<4))>>4);
    lsw(l,0x32,d0);lsw(l,0x34,d1);
    if ((list&0xffffu)==0xffffu && (list>>16)==0xffffu) l[0x83]=(uint8_t)(l[0x34]-(uint8_t)d1);   /* cmpa.w #FFFF (sign-extended) */
    else l[0x83]=0;
    lsl(l,0x3e,list);
}
/* 0C6BDE: the column table +42.. (byte 0: row offset * 8, byte 1: height)
 * from the list +3E for d2 columns from map column d0 / ring column d4. */
static bool k0c6bde(neo_genv_t *e,uint8_t *l,uint16_t d0,uint16_t d2,uint16_t d4) {
    d0=(uint16_t)(S16((uint16_t)(d0<<4))>>4);
    uint32_t a2=ll(l,0x3e);
    if ((a2&0xffffu)==0xffffu && (a2>>16)==0xffffu) return true;
    uint32_t map=ll(l,0x2e);
    uint16_t width=rw(e,map+8);
    for (uint16_t n=0;n<d2;n++,d0++,d4++) {
        unsigned o=0x42u+2u*(d4&0x1fu);
        if (d0>=width) {if (l[0x82]&8u) l[o+1]=0;continue;}    /* andi.w #FF00 */
        if (!a2) {lsw(l,o,0x21);continue;}
        for (unsigned g=0;g<256;g++) {uint16_t c=rw(e,a2);if (!c || d0>=c) break;a2-=4;}
        for (unsigned g=0;g<256;g++) {if (d0<rw(e,a2+4)) break;a2+=4;}
        uint8_t h=rb(e,a2+3);
        if (h>=0x20u) {lsw(l,o,0x21);continue;}
        l[o+1]=h;
        int16_t r=(int16_t)(-(int16_t)(int8_t)rb(e,a2+2)+1+S16(lw(l,0x34)));
        l[o]=(uint8_t)((uint8_t)r<<3);
    }
    lsl(l,0x3e,a2);
    return !e->unsupported;
}
/* 0C73CE: draw d2 x d3 blocks of the map from (d0, d1) (0C6BDE + 0C714E),
 * then the window +36/+38 from the layer's position. */
static void k0c73ce(neo_genv_t *e,neo_m3mid_env_t *m,uint16_t d0,uint16_t d1,uint16_t d2,uint16_t d3) {
    uint8_t *l=m->layer;
    uint32_t map=ll(l,0x2e);
    if (!map) return;
    uint16_t d4=(uint16_t)(d0+lw(l,0x32)),d5=(uint16_t)(lw(l,0x34)-d1);
    k0c6bde(e,l,d0,d2,d4);
    if (m->draw) {int16_t d[6]={(int16_t)d0,(int16_t)d1,(int16_t)d2,(int16_t)d3,(int16_t)d4,(int16_t)d5};m->draw(e->ctx,map,d);}
    lsw(l,0x36,(uint16_t)(S16((uint16_t)((0xffc00000u-ll(l,0x14))>>16))>>4));
    lsw(l,0x38,(uint16_t)(S16((uint16_t)((0x800000u-ll(l,0x18))>>16))>>4));
}
/* 0A6C18: the layer set up (0A6C8C: map 2A6142, columns 2A61B0) and the
 * body put on it. */
static void layer_setup(neo_gactor_t *a,neo_genv_t *e) {
    neo_m3mid_env_t *m=menv(e);
    if (!m) return;
    uint8_t *l=m->layer;
    m->flags959=0x80;                                                   /* 0C7BD0: RAM 10E959 */
    lsl(l,8,0);lsl(l,0xc,0);
    l[0x82]&=0xfeu;                                                     /* 0C6D04 */
    if (m->clear) m->clear(e->ctx);                                     /* 0C702A ring, 0C703C/0C6D04 SCB */
    SW(0x92,W(0x28));SW(0x94,W(0x2c));
    layer_place(e,l,S16(W(0x28)),(int16_t)(S16(W(0x2c))+0xc0));
    l[0x82]=(uint8_t)((l[0x82]&~6u)|8u);
    k0c6f34(l,0x2a6142u,0,9,0x2a61b0u);
    k0c73ce(e,m,0,0,0x13,0xa);
    l[0x82]|=0x81u;                                                     /* 0C6CFC */
    ga_place(a,e,0x98,0,LAYER);                                         /* 01AB98 */
}
/* 0A6F30: the wreck map 2A619A (columns 2A61DA). */
static void wreck_map(neo_genv_t *e) {
    neo_m3mid_env_t *m=menv(e);
    if (!m) return;
    k0c6f34(m->layer,0x2a619au,0,9,0x2a61dau);
    k0c73ce(e,m,0,0,0x13,0xa);
}
/* 0A6D7C: patch `index` of the list 2A6210 (0C714E into the layer). */
static void patch(neo_genv_t *e,uint16_t index) {
    neo_m3mid_env_t *m=menv(e);
    if (!m) return;
    uint32_t at=0x2a6210u+10u*index;
    int16_t d[6];for (unsigned i=0;i<6;i++) d[i]=rb(e,at+4+i);
    uint32_t sub=rl(e,at);
    if (m->patch) m->patch(e->ctx,sub,d);
}
/* ---- body ------------------------------------------------------------------- */
/* 0A57BA: +A5 += step, the sine of it << shift. */
static uint16_t sway(neo_gactor_t *a,uint8_t step,unsigned shift) {
    uint8_t d2=(uint8_t)(B(0xa5)+step);SB(0xa5,d2);
    uint16_t d0=(uint16_t)ga_sine(d2);
    return (uint16_t)(d0<<shift);
}
static void k5770(neo_gactor_t *a) {
    SL(0x7e,L(0x7e)+(uint32_t)((int32_t)S16(W(0x9a))<<8));SW(0x9a,0);
    SL(0x82,L(0x82)+(uint32_t)((int32_t)S16(W(0x9c))<<8));SW(0x9c,0);
    SB(0xa5,0);
}
static void k5796(neo_gactor_t *a) {SW(0x9c,sway(a,3,3));}
static void k57a8(neo_gactor_t *a) {SW(0x9c,sway(a,0x10,1));}
/* 0A57DC / 0A580E: the vertical step to +94 (+ 0x10). */
static void k57dc(neo_gactor_t *a,neo_genv_t *e) {
    int16_t d0=scale(e,(int16_t)(W(0x94)+0x10u-W(0x82)),0x80,0x400,0xc0);
    SL(0x8a,(uint32_t)((int32_t)d0<<8));
    if (!L(0x8a)) k57a8(a);
}
static bool k580e(neo_gactor_t *a,neo_genv_t *e) {
    int16_t d0=scale(e,(int16_t)(W(0x94)-W(0x82)),0x80,0x400,0xc0);
    SL(0x8a,(uint32_t)((int32_t)d0<<8));
    return L(0x8a)!=0;
}
/* 0A6BB2: 4 in phase 2 (body +4E < +96), else 0. */
static unsigned phase(neo_gactor_t *a,neo_genv_t *e) {neo_gactor_t *b=body(a,e);if (!b) {ga_fail(e,0xa6bb2);return 0;}return ga_w(b,0x4e)<ga_w(b,0x96)?4u:0u;}
static uint16_t rank_of(neo_gactor_t *a,neo_genv_t *e,uint32_t pointers) {return rank(a,e,rl(e,pointers+phase(a,e)));}
/* 0A6B1A: the horizontal speed toward the nearest player's side (0x58 to
 * the side the player is on, or the side +A7 forces). */
static uint32_t k6b1a(neo_gactor_t *a,neo_genv_t *e) {
    uint32_t d1=0x7fff;
    for (unsigned p=0;p<2;p++) {
        if (!((e->players.active_mask>>p)&1u)) continue;                /* 01AEB0 */
        uint16_t px=p?e->players.p2x:e->players.p1x;
        int16_t d3=(int16_t)(px-W(0x3c));
        uint16_t d2=d3>=0?0x58u:0xffa8u;
        if (B(0xa7)) d2=B(0xa7)==1u?0x58u:0xffa8u;
        d2=(uint16_t)(d2-(uint16_t)d3);
        uint32_t v=d2;
        if (S16(d2)<0) v=0xffff0000u|(uint16_t)-d2;
        if ((uint16_t)d1>(uint16_t)v) d1=v;
    }
    if ((uint16_t)d1==0x7fffu) return 0;
    uint16_t d0=(uint16_t)d1;
    if (!(d1&0x80000000u)) d0=(uint16_t)-d0;
    return (uint32_t)((int32_t)scale(e,S16(d0),0x80,W(0xa0),0x80)<<8);
}
static void k6b04(neo_gactor_t *a,neo_genv_t *e) {if (!(e->video&3u)) SL(0x86,k6b1a(a,e));}
static void k6f1a(neo_gactor_t *a,neo_genv_t *e) {neo_gactor_t *b=body(a,e);if (b) {ga_sl(b,0x86,0);ga_sl(b,0x8a,0);}}
static void a2_bits(neo_gactor_t *a,neo_genv_t *e,int set,int clr1,int clr2) {
    neo_gactor_t *b=body(a,e);if (!b) {ga_fail(e,0xa6e);return;}
    uint8_t v=ga_b(b,0xa2);
    if (set>=0) v|=(uint8_t)(1u<<set);
    if (clr1>=0) v&=(uint8_t)~(1u<<clr1);
    if (clr2>=0) v&=(uint8_t)~(1u<<clr2);
    ga_sb(b,0xa2,v);
}
/* 0A584E: the next step of the cycle; its end restarts it (0A4B9C). */
static void k584e(neo_gactor_t *a,neo_genv_t *e) {
    uint32_t table=rl(e,0x2a60feu+B(0xa3));
    uint8_t d0=(uint8_t)(B(0xa4)+1u);SB(0xa4,d0);
    if (rl(e,table+4u*d0)==0xffffffffu) SL(0,0xa4b9c);
}
/* 0A6BE2: the next damage stage of list `which` (2A6334) on the body. */
static void stage(neo_gactor_t *a,neo_genv_t *e,unsigned which) {
    uint32_t list=rl(e,0x2a6334u+4u*which);
    uint8_t d0=rb(e,list+B(0x7b));
    if (d0&0x80u) return;
    neo_gactor_t *b=body(a,e);if (!b) {ga_fail(e,0xa6c06);return;}
    ga_sw(b,0x74,(uint16_t)(ga_w(b,0x74)|(1u<<(d0&15u))));
    SB(0x7b,B(0x7b)+1u);
}
/* 0A6DE4: the part `index` (0A64E2, table 2A601A). */
static void part(neo_gactor_t *a,neo_genv_t *e,uint8_t index) {
    neo_gactor_t *c=e->spawn_reserve?e->spawn_reserve(e->ctx,a,0xa64e2,0x1c):e->spawn?e->spawn(e->ctx,a,0xa64e2,false):NULL;
    if (!c) return;
    ga_place_from(c,a,e,0,0);ga_sl(c,0x6e,L(0x6e));
    ga_sb(c,0x87,index);ga_sb(c,0x7a,B(0x7a));
    uint32_t at=0x2a601au+10u*index;
    ga_sw(c,0x82,rw(e,at+4));ga_sw(c,0x84,rw(e,at+6));
    ga_sw(c,0x1e,rw(e,0x2a6016u+2u*rb(e,at+8)));
    uint8_t d1=rb(e,at+9);ga_sb(c,0x86,d1);
    uint8_t d0=(uint8_t)((B(0x15)&0xfbu)^(d1&7u));
    ga_sb(c,0x15,(uint8_t)((ga_b(c,0x15)&0xf8u)|d0));
    if (ga_b(c,0x15)&1u) ga_sw(c,0x82,(uint16_t)(ga_w(c,0x82)+1u));
    ga_sl(c,0x7e,rl(e,at));
}
/* 0A6DBA: the parts of the list table[facing]. */
static void parts(neo_gactor_t *a,neo_genv_t *e,uint32_t table) {
    uint32_t list=rl(e,table+4u*(B(0x15)&1u));
    SB(0x7a,B(0x7a)+1u);
    for (unsigned i=0;i<16;i++) {uint8_t d0=rb(e,list+i);if (d0==0xffu) return;part(a,e,d0);}
}
/* 0A6E88: four debris 0A6598 at the middle of the patch 2A6210[index+10]. */
static void debris4(neo_gactor_t *a,neo_genv_t *e,uint16_t index) {
    neo_m3mid_env_t *m=menv(e);
    if (!m) return;
    uint32_t at=0x2a6210u+10u*(uint16_t)(index+10u);
    uint8_t d0=(uint8_t)(rb(e,at+8)+(rb(e,at+6)>>1)),d1=(uint8_t)(rb(e,at+9)+((uint8_t)(rb(e,at+7)-1u)>>1));
    uint32_t x=(uint32_t)d0<<20,y=(uint32_t)d1<<20;
    neo_contact_parent_t r;
    if (!ga_parent(e,ROOT,&r)) return;
    x=x+ll(m->layer,0x1c)-(uint32_t)r.previous_x;y=y+ll(m->layer,0x20)-(uint32_t)r.previous_y;    /* 01AC4C */
    for (unsigned i=0;i<4;i++) {
        neo_gactor_t *c=child(a,e,0xa6598,0x2a);
        if (c) {ga_sl(c,0x28,x);ga_sl(c,0x2c,y);}
    }
}
/* 0A5C04: damage stage `d0`. */
static bool list_has(neo_genv_t *e,uint32_t list,uint16_t d0) {
    for (unsigned i=0;i<16;i++) {uint8_t b=rb(e,list+i);if (b==0xffu) return false;if (b==d0) return true;}
    return false;
}
static void cannon_patches(neo_gactor_t *a,neo_genv_t *e,uint16_t which,uint32_t list) {   /* 0A5CAA / 0A5CBC */
    patch(e,which);
    uint16_t d0=W(0x74);
    for (unsigned i=0;i<16;i++) {uint8_t d1=rb(e,list+i);if (d1==0xffu) return;if ((d0>>(d1&31u))&1u) patch(e,(uint16_t)(d1+0xau));}
}
static void stage_hit(neo_gactor_t *a,neo_genv_t *e,uint16_t d0) {
    debris4(a,e,d0);
    sound(e,0x1023);
    if (d0==0 || d0==0xa) {
        neo_gactor_t *c=child(a,e,0xa5d6a,0x1c);
        if (c) {if (d0) ga_sb(c,0x15,(uint8_t)(ga_b(c,0x15)|1u)); else ga_sb(c,0x15,(uint8_t)(ga_b(c,0x15)&~1u));}
    }
    if (list_has(e,0x2a634fu,d0)) {cannon_patches(a,e,8,0x2a634fu);return;}
    if (list_has(e,0x2a6354u,d0)) {cannon_patches(a,e,9,0x2a6354u);return;}
    patch(e,(uint16_t)(d0+0xau));
}
/* 0A56E4: damage (the guns' +98 too), stages, flash. */
static void damage(neo_gactor_t *a,neo_genv_t *e) {
    uint16_t d0=(uint16_t)(W(0x72)-W(0x4e));
    uint16_t d1=(uint16_t)(W(0x78)+d0);SW(0x78,d1);
    if (d1>=0x64u) {SW(0x78,W(0x78)-0x64u);stage(a,e,0);}
    d0=W(0x4e);
    if (!(S16(d0)>S16(W(0x98)))) SW(0x98,d0);
    d0=(uint16_t)(d0-W(0x98));SW(0x98,0);SW(0x4e,d0);
    if (W(0x72)^d0) SB(0x17,B(0x17)|6u);
    SW(0x72,d0);
    if (BIT(0x5f,6)) SB(0x17,0);
    /* 00454C: the palette fade of slot +16 (RAM 1068EA, renderer). */
    if (S8(B(0x17))>0) SB(0x17,B(0x17)-1u);                            /* 00456C */
}
/* 0A5A98 / 0A5ABA: registration, the move, the layer step, stages. */
static ga_result_t tail(neo_gactor_t *a,neo_genv_t *e,bool reg) {
    if (reg) {
        if (ga_register(a,e,NULL,NULL)) SB(0x5a,0);
        if (S16(W(0x4e))<=0) {SET(0x5f,7);SL(0,0xa4bda);}
    }
    SL(0x7e,L(0x7e)+L(0x86));SL(0x82,L(0x82)+L(0x8a));
    uint16_t d1=B(0xa6),d0=(uint16_t)(W(0x90)-d1);
    if (S16(d0)<S16(W(0x7e))) {SW(0x7e,d0);SB(0xa7,1);}
    d0=(uint16_t)(W(0x8e)+d1);
    if (S16(d0)>S16(W(0x7e))) {SW(0x7e,d0);SB(0xa7,2);}
    d0=(uint16_t)(W(0x90)-0x98u);
    if (S16(d0)>S16(W(0x7e)) && B(0xa7)==1u) SB(0xa7,0);
    d0=(uint16_t)(W(0x8e)+0x98u);
    if (S16(d0)<S16(W(0x7e)) && B(0xa7)==2u) SB(0xa7,0);
    neo_m3mid_env_t *m=menv(e);
    if (!m) return GA_UNSUPPORTED;
    lsl(m->layer,8,L(0x7e)+(uint32_t)((int32_t)S16(W(0x9a))<<8)-ll(m->layer,0));
    lsl(m->layer,0xc,L(0x82)+(uint32_t)((int32_t)S16(W(0x9c))<<8)-ll(m->layer,4));
    damage(a,e);
    uint16_t bits=(uint16_t)(W(0x74)^W(0x76));
    for (uint16_t k=0;bits;k++,bits>>=1) if (bits&1u) stage_hit(a,e,k);
    if (BIT(0x7c,0) && !BIT(0x7c,2)) {SET(0x7c,2);part(a,e,9);part(a,e,0xb);}
    if (BIT(0x7c,1) && !BIT(0x7c,3)) {SET(0x7c,3);part(a,e,0xa);part(a,e,0xc);}
    SW(0x76,W(0x74));
    ga_project(a,e);
    return e->unsupported?GA_UNSUPPORTED:GA_MORE;
}
/* 0A4D90: the wreck's drift (0A4D9E sway, 0A4D64 to x 0xA0, 0A4D86 fall). */
static void drift(neo_gactor_t *a,neo_genv_t *e) {
    SW(0x9a,sway(a,0x40,1));
    SL(0x86,(uint32_t)((int32_t)scale(e,(int16_t)(0xa0u-W(0x3c)),0x80,0x200,0x40)<<8));
    SL(0x8a,0xffff8000u);
}
/* 0A5634: the death explosion lists 2A6526. */
static void death_lists(neo_gactor_t *a,neo_genv_t *e,neo_gactor_t **last) {
    for (unsigned i=0;i<16;i++) {
        uint32_t at=0x2a6526u+4u*i;
        if (rw(e,at)==0xffffu) return;
        neo_gactor_t *c=debris_list(a,e,rl(e,at),0x1001);
        if (last) *last=c;
    }
}
/* 0A5670: an item 55 (08CDB2, value 5A) thrown up. */
static void item55(neo_gactor_t *a,neo_genv_t *e) {
    int16_t d1=(int16_t)(ga_rand_range(e,0x40)+0x28u);
    int16_t d0=(int16_t)(ga_rand_range(e,0xa0)+0xffb0u);
    neo_gactor_t *c=after_script(e,0x8c374);
    if (c) {ga_sb(c,0x8b,0);ga_sw(c,0x70,0x5a);ga_sw(c,0x6e,0x55);ga_sw(c,0x72,0);ga_sb(c,0x7f,0);ga_place_at(c,a,e,d0,d1);}
    uint8_t angle=(uint8_t)((ga_rand(e)&0x1fu)+0x30u);
    uint32_t speed=(uint32_t)((ga_rand(e)&0x3ffu)+0x600u)<<8,vx,vy;
    ga_polar(speed,angle,&vx,&vy);
    if (c) {ga_sl(c,0x30,vx);ga_sl(c,0x34,vy);}
}
/* The cycle 2A610A (+A3 = 0): one step (0A5830). */
static void cycle(neo_gactor_t *a,neo_genv_t *e) {
    if (B(0xa3)) {ga_fail(e,0xa5830);return;}                          /* table 2A6126: never selected */
    switch (rl(e,0x2a610au+4u*B(0xa4))) {
    case 0xa5882:
        a2_bits(a,e,0,1,2);SW(0x9e,0x1e);SB(0xa7,0);k6f1a(a,e);k584e(a,e);sound(e,0x10eb);return;
    case 0xa58be:
        k57dc(a,e);
        if (W(0x9e)) {SW(0x9e,W(0x9e)-1u);return;}
        SW(0x9e,0xffff);
        SW(0x9e,rank_of(a,e,0x2a6388u));SW(0xa0,rank_of(a,e,0x2a6380u));k584e(a,e);return;
    case 0xa5900:
        k57dc(a,e);k6b04(a,e);
        if (W(0x9e)) {SW(0x9e,W(0x9e)-1u);return;}
        SW(0x9e,0xffff);
        SL(0x86,0);a2_bits(a,e,1,0,-1);SW(0x9e,0x1e);stop_sound(e,0x10eb);sound(e,0x10ec);k584e(a,e);return;
    case 0xa5948:
        k57dc(a,e);
        if (W(0x9e)) {SW(0x9e,W(0x9e)-1u);return;}
        SW(0x9e,0xffff);
        SW(0x9e,rank_of(a,e,0x2a6390u));SW(0xa0,rank_of(a,e,0x2a6398u));k584e(a,e);return;
    case 0xa598a:
        k57dc(a,e);k6b04(a,e);
        if (S16(W(0x9e))>=0) {SW(0x9e,W(0x9e)-1u);return;}
        SL(0x86,0);a2_bits(a,e,2,1,-1);k5770(a);stop_sound(e,0x10ec);k584e(a,e);return;
    case 0xa59ce:
        if (!k580e(a,e)) {SB(0xa6,0);k584e(a,e);}
        return;
    }
    ga_fail(e,0xa5830);
}
static ga_result_t body_tick(neo_gactor_t *a,neo_genv_t *e) {
    switch (L(0)) {
    case 0xa4a94: {
        uint16_t d0=W(0x6e),d1=W(0x70);SW(0x6e,0);SW(0x70,0);
        d0=(uint16_t)(d0+W(0x28));d1=(uint16_t)(d1+W(0x28));SW(0x8e,d0);SW(0x90,d1);
        sound(e,0x10ed);ga_project(a,e);
        layer_setup(a,e);
        /* 0A6CF0 */
        SL(0x6e,NEO_GA_TAG(a->index));CLR(0x15,0);SB(0xa5,0);SW(0x9c,0);SB(0xa5,0);SW(0x9a,0);
        SB(0xa2,0);SW(0x74,0);SW(0x76,0);SB(0xa8,0);SB(0xa6,0);SB(0xa7,0);
        {neo_m3mid_env_t *m=menv(e);if (!m) return GA_UNSUPPORTED;SL(0x7e,ll(m->layer,0));SL(0x82,ll(m->layer,4));}
        SW(0x4e,rank(a,e,0x2c001a));SW(0x72,W(0x4e));SW(0x78,0);SW(0x98,0);SL(0x50,0x2a5fde);
        SB(0x14,0x5e);SB(0x16,B(0x14));SB(0x17,0);                      /* 0A56D8, 00453E */
        SB(0x7b,0);SET(0x5f,6);SL(0x86,0);SL(0x8a,0);SW(0x96,rank(a,e,0x2c0096));
        child(a,e,0xa606e,0x1c);child(a,e,0xa6078,0x1c);child(a,e,0xa61e8,0x1c);child(a,e,0xa61f2,0x1c);
        SL(0x8a,0xfffeb000u);SL(0,0xa4b52);
    }   /* fall through */
    case 0xa4b52:
        if (S16(W(0x82))<S16(W(0x94))) SL(0,0xa4b68);
        return tail(a,e,true);
    case 0xa4b68:
        SL(0x82,(uint32_t)W(0x94)<<16);SL(0x8a,0);CLR(0x5f,6);
        child(a,e,0xa5e88,0x1c);
        /* fall through */
    case 0xa4b9c:
        SW(0x9e,rank_of(a,e,0x2a6370u));                               /* 0A6A60 */
        k5770(a);SL(0,0xa4baa);
        /* fall through */
    case 0xa4baa:
        k5796(a);
        SW(0x9e,W(0x9e)-1u);
        if (S16(W(0x9e))<0) SL(0,0xa4bc0);
        return tail(a,e,true);
    case 0xa4bc0:
        SB(0xa4,0);SB(0xa3,0);k5770(a);SL(0,0xa4bce);
        /* fall through */
    case 0xa4bce:
        if (phase(a,e) && !B(0xa8)) {SB(0xa8,1);child(a,e,0xa4db6,0x1c);}   /* 0A6A86 / 0A68F8 */
        cycle(a,e);
        if (e->unsupported) return GA_UNSUPPORTED;
        return tail(a,e,true);
    case 0xa4bda: {
        stop_sound(e,0x10eb);stop_sound(e,0x10ec);                      /* 0A6BCC */
        child(a,e,0xa4db0,0xe);
        k6f1a(a,e);k5770(a);
        cannon_patches(a,e,8,0x2a634fu);cannon_patches(a,e,9,0x2a6354u);  /* 0A5CFC */
        if (e->dyn_release) e->dyn_release(e->ctx,a);                   /* 013F1E */
        if (e->shake_pulse) e->shake_pulse(e->ctx);                     /* 0C8022 */
        death_lists(a,e,NULL);
        SW(0x9e,0x14);SL(0,0xa4c24);
    }   /* fall through */
    case 0xa4c24:
        drift(a,e);
        if (!W(0x9e)) SL(0,0xa4c3a);
        SW(0x9e,W(0x9e)-1u);
        return tail(a,e,false);
    case 0xa4c3a: {
        sound(e,0x1032);
        if (e->shake_pulse) e->shake_pulse(e->ctx);
        wreck_map(e);death_lists(a,e,NULL);
        neo_gactor_t *b=body(a,e);if (b) ga_sb(b,0xa2,(uint8_t)(ga_b(b,0xa2)|0x40u));
        SW(0x9e,0x1e);SL(0,0xa4c68);
    }   /* fall through */
    case 0xa4c68:
        drift(a,e);
        if (!W(0x9e)) SL(0,0xa4c7e);
        SW(0x9e,W(0x9e)-1u);
        return tail(a,e,false);
    case 0xa4c7e: {
        debris_list(a,e,0x2a647au,0x1001);
        neo_gactor_t *b=body(a,e);if (b) ga_sb(b,0xa2,(uint8_t)(ga_b(b,0xa2)|0x80u));
        if (e->shake_pulse) e->shake_pulse(e->ctx);
        SW(0x9e,0x3c);SL(0,0xa4cb2);
    }   /* fall through */
    case 0xa4cb2:
        drift(a,e);
        if (!W(0x9e)) SL(0,0xa4cc8);
        SW(0x9e,W(0x9e)-1u);
        return tail(a,e,false);
    case 0xa4cc8: {
        k6f1a(a,e);SW(0x86,0);SW(0x8a,0);SW(0x9e,3);
        neo_gactor_t *f=after_script(e,0xae1dc);                         /* 0AE220(3, 1) */
        if (f) {ga_sb(f,0x70,1);ga_sw(f,0x6e,W(0x9e));}
        SL(0,0xa4cf2);
    }   /* fall through */
    case 0xa4cf2:
        if (!W(0x9e)) SL(0,0xa4d04);
        SW(0x9e,W(0x9e)-1u);
        return tail(a,e,false);
    case 0xa4d04: {
        if (e->kill_children) e->kill_children(e->ctx,a);               /* 004192 */
        ga_project(a,e);sound(e,0x1033);
        neo_gactor_t *last=NULL;death_lists(a,e,&last);
        /* 0A5604: the layer off; 0C6F34 with the D0/D1 the last 01DA40
         * left (the explosion's projected words: the body's +3E/+42). */
        neo_m3mid_env_t *m=menv(e);if (!m) return GA_UNSUPPORTED;
        if (!last) STOP(0xa5604);
        m->layer[0x82]|=6u;m->layer[0x82]&=0xfeu;
        if (m->clear) m->clear(e->ctx);
        k0c6f34(m->layer,0,ga_w(last,0x3e),ga_w(last,0x42),0);
        if (e->shake) e->shake(e->ctx);                                 /* 0C801A */
        for (unsigned i=0;i<0x1e;i++) item55(a,e);
        SW(0x9e,0x1e);SL(0,0xa4d3c);
    }   /* fall through */
    case 0xa4d3c:
        ga_project(a,e);
        SW(0x9e,W(0x9e)-1u);
        if (S16(W(0x9e))<0) SL(0,0xa4d52);
        return GA_MORE;
    case 0xa4d52:
        sound(e,0x107b);
        return GA_DELETE_TREE;
    }
    STOP(L(0));
}
/* ---- program callbacks --------------------------------------------------------- */
static bool any_bit(neo_gactor_t *a,neo_genv_t *e,uint32_t list) {
    neo_gactor_t *b=body(a,e);if (!b) {ga_fail(e,list);return false;}
    uint16_t d0=ga_w(b,0x74);
    for (unsigned i=0;i<16;i++) {uint8_t d1=rb(e,list+i);if (d1==0xffu) return false;if ((d0>>(d1&31u))&1u) return true;}
    return false;
}
static void bomb_drop(neo_gactor_t *a,neo_genv_t *e);
static bool call(neo_gactor_t *a,neo_genv_t *e,neo_anim_kind_t kind,uint32_t routine,uint16_t parameter) {
    if (kind!=NEO_ANIM_CALL) return false;
    if (routine>=0xa6f7a && routine<=0xa703a && !((routine-0xa6f7a)%16u)) {ga_palette(a,(uint16_t)(0x26u+(routine-0xa6f7a)/16u));return true;}
    neo_gactor_t *c;
    switch (routine) {
    case 0xa6f62: SB(0x1d,0);return true;                               /* 009F88 */
    case 0xa6f6a: ga_palette(a,0x1d);return true;
    case 0xa704a: case 0xa7054:
        SB(0x7d,0);
        if (!any_bit(a,e,routine==0xa704a?0x2a6351u:0x2a6354u)) {SB(0x7d,1);SET(0x15,2);SW(0x66,0xfff0);SW(0x1e,0xfff0);}
        return !e->unsupported;
    case 0xa7092: CLR(0x15,2);SW(0x66,0);SW(0x1e,0);return true;
    case 0xa70a6: part(a,e,6);return !e->unsupported;
    case 0xa70ae: if (!(c=creator(a,e))) return false;ga_sb(c,0x7f,(uint8_t)(ga_b(c,0x7f)+1u));return true;
    case 0xa70b8: if (!(c=creator(a,e))) return false;ga_sb(c,0x7f,0xff);return true;
    case 0xa70c4: if (!(c=creator(a,e))) return false;ga_sb(c,0x60,1);return true;
    case 0xa70d6: case 0xa70f8:
        if (!any_bit(a,e,routine==0xa70d6?0x2a634fu:0x2a6354u)) patch(e,parameter);
        return !e->unsupported;
    case 0xa711c: case 0xa712e: {
        neo_gactor_t *b=body(a,e);if (!b) return false;
        ga_sw(b,0x74,(uint16_t)(ga_w(b,0x74)|(routine==0xa711c?0x1000u:0x2000u)));return true;
    }
    case 0xa530c: bomb_drop(a,e);return !e->unsupported;
    }
    return false;
}
#define STEP(d) do { int r_=ga_anim_step(a,e,call); if (r_<0) return GA_UNSUPPORTED; (d)=r_!=0; } while (0)
/* ---- side pieces 0A606E / 0A6078 ------------------------------------------------- */
static ga_result_t side_tick(neo_gactor_t *a,neo_genv_t *e) {
    bool done;
    if (L(0)!=0xa60be) {
        SB(0x7f,L(0)==0xa6078?1:0);
        on_creator(a,e,B(0x7f)?0x58:-0x58,0x30);
        SW(0x66,0xffef);SW(0x1e,0xffef);SET(0x15,2);ga_palette(a,0x26);SB(0x7e,0xff);CLR(0x15,0);SL(0,0xa60be);
    }
    /* 0A616C: the program by the stages. */
    neo_gactor_t *b=body(a,e);if (!b) STOP(0xa616e);
    uint8_t d0=0;
    if ((ga_w(b,0x74)>>(B(0x7f)?0xbu:0u))&1u) d0=1;
    if ((ga_b(b,0x7c)>>(B(0x7f)?1u:0u))&1u) d0=2;
    if (d0!=B(0x7e)) {SB(0x7e,d0);START(rl(e,(B(0x7f)?0xe61c8u:0xe61bcu)+4u*d0));}
    ga_project(a,e);STEP(done);(void)done;
    if (ga_b(b,0xa2)&0x40u) return GA_DELETE;
    return e->unsupported?GA_UNSUPPORTED:GA_MORE;
}
/* ---- guns 0A61E8 / 0A61F2 ----------------------------------------------------- */
static void gun_program(neo_gactor_t *a,neo_genv_t *e,uint32_t table) {          /* 0A64B8 */
    if (B(0x7e)) return;
    ga_anim_start(a,e,rl(e,table+((B(0x15)&1u)?4u:0u)),0x100);
}
static ga_result_t gun_tail(neo_gactor_t *a,neo_genv_t *e) {
    ga_project(a,e);
    neo_gactor_t *b=body(a,e);if (!b) STOP(0xa633e);
    if (ga_b(b,0x5f)&0x40u) SET(0x5f,6); else CLR(0x5f,6);
    if (ga_b(b,0x5f)&0x80u) return GA_DELETE_TREE;
    if (!B(0x7e)) {
        uint32_t list=(B(0x15)&1u)?0x2a6355u:0x2a6351u;
        uint16_t d0=ga_w(b,0x74);
        for (unsigned i=0;i<16;i++) {uint8_t d1=rb(e,list+i);if (d1==0xffu) break;if ((d0>>(d1&31u))&1u) SB(0x7e,1);}
        if (!B(0x7e)) {
            uint8_t keep=B(0x15);CLR(0x15,0);
            if (ga_anim_step(a,e,call)<0) return GA_UNSUPPORTED;
            SB(0x15,keep);
        }
    }
    if (S8(B(0x7f))>=0) ga_attack(a,e,rl(e,0x2a5faau+4u*B(0x7f)));
    unsigned side=B(0x15)&1u;
    if (!((ga_b(b,0x7c)>>side)&1u) && ga_register(a,e,NULL,NULL)) {
        SB(0x5a,0);
        if (BIT(0x5f,7)) {
            uint16_t d3=ga_b(b,0x7c)?5u:1u;
            uint8_t d0=(uint8_t)((B(0x5e)>>2)&3u);                       /* 08D1D0 */
            if (d0) {
                if (e->pickup && d0<=2u) neo_item_add_score(e->pickup->score[d0-1u],rl(e,0xe5582u+4u*d3));   /* 057948 */
                sound(e,0x1121);
                neo_gactor_t *c=after_script(e,0x8d0d2);
                if (c) {ga_sb(c,0x87,0);ga_sb(c,0x71,(uint8_t)((d0-1u)&1u));ga_sb(c,0x70,(uint8_t)d3);ga_place_at(c,a,e,0,0);}
            }
            ga_sb(b,0x7c,(uint8_t)(ga_b(b,0x7c)|(1u<<side)));
        }
    }
    ga_sb(b,0x5e,(uint8_t)(ga_b(b,0x5e)|B(0x5e)));
    uint16_t d1=W(0x4e),d0=(uint16_t)(W(0x72)-d1);
    ga_sw(b,0x98,(uint16_t)(ga_w(b,0x98)+d0));SW(0x72,d1);
    d1=(uint16_t)(W(0x78)+d0);SW(0x78,d1);
    if (d1>=0x64u) {SW(0x78,W(0x78)-0x64u);stage(a,e,side?2u:1u);}
    return e->unsupported?GA_UNSUPPORTED:GA_MORE;
}
static bool body_bit(neo_gactor_t *a,neo_genv_t *e,unsigned bit) {neo_gactor_t *b=body(a,e);if (!b) {ga_fail(e,0xa628c);return false;}return (ga_b(b,0xa2)>>bit)&1u;}
static ga_result_t gun_tick(neo_gactor_t *a,neo_genv_t *e) {
    switch (L(0)) {
    case 0xa61e8: case 0xa61f2:
        if (L(0)==0xa61e8) CLR(0x15,0); else SET(0x15,0);
        on_creator(a,e,(B(0x15)&1u)?0x58:-0x58,0x30);
        SW(0x66,0);SW(0x1e,0);SB(0x7f,0xff);SB(0x7e,0);SB(0x7b,0);SL(0x50,0x2a5ff2);
        SW(0x4e,rank(a,e,0x2c007c));SW(0x72,W(0x4e));SW(0x78,0);
        gun_program(a,e,0x2a60d2);SL(0,0xa6260);
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    case 0xa6260:
        parts(a,e,0x2a60b2);SL(0,0xa6270);
        /* fall through */
    case 0xa6270: {
        neo_gactor_t *c=creator(a,e);if (!c) STOP(0xa6270);
        if (ga_w(c,0x82)<0xa0u) {part(a,e,5);SL(0,0xa628c);}
    }   /* fall through */
    case 0xa628c:
        if (body_bit(a,e,0)) SL(0,0xa62a4);
        return gun_tail(a,e);
    case 0xa62a4:
        parts(a,e,0x2a60ba);gun_program(a,e,0x2a60da);SL(0,0xa62be);
        /* fall through */
    case 0xa62be:
        if (body_bit(a,e,1)) SL(0,0xa62d6);
        return gun_tail(a,e);
    case 0xa62d6:
        parts(a,e,0x2a60c2);gun_program(a,e,0x2a60e2);SL(0,0xa62f0);
        /* fall through */
    case 0xa62f0:
        if (body_bit(a,e,2)) SL(0,0xa6308);
        return gun_tail(a,e);
    case 0xa6308:
        parts(a,e,0x2a60ca);SB(0x60,0);gun_program(a,e,0x2a60d2);SL(0,0xa6326);
        /* fall through */
    case 0xa6326:
        if (B(0x60)) SL(0,0xa6260);
        return gun_tail(a,e);
    }
    STOP(L(0));
}
/* ---- parts 0A64E2 -------------------------------------------------------------- */
static ga_result_t part_tick(neo_gactor_t *a,neo_genv_t *e) {
    bool done;
    if (L(0)==0xa656c) return GA_DELETE_TREE;
    if (L(0)==0xa64e2) {
        START(L(0x7e));on_creator(a,e,0,0);SW(0x28,W(0x82));SW(0x2c,W(0x84));SL(0,0xa6504);
    }
    neo_gactor_t *c=creator(a,e);if (!c) STOP(0xa658a);
    if (ga_b(c,0x7a)!=B(0x7a)) return GA_DELETE_TREE;
    if (B(0x86)&8u) {
        neo_contact_parent_t r,p;
        if (!ga_parent(e,ROOT,&r)) return GA_UNSUPPORTED;
        uint32_t tag=L(0x38),d1=(uint32_t)W(0x84)<<16;
        if (tag!=ROOT) {
            d1+=(uint32_t)r.previous_y;
            if (tag) {if (!ga_parent(e,tag,&p)) return GA_UNSUPPORTED;d1-=(uint32_t)p.previous_y;}
        }
        SL(0x2c,d1);                                                    /* 01AC4C */
    }
    if (B(0x86)&0x10u) {on_creator(a,e,0,0);SW(0x28,W(0x82));SW(0x2c,W(0x84));ga_floor(a,e);}
    ga_project(a,e);STEP(done);
    if (done) SL(0,0xa656c);
    return e->unsupported?GA_UNSUPPORTED:GA_MORE;
}
/* ---- debris 0A6598 -------------------------------------------------------------- */
static ga_result_t debris_tick(neo_gactor_t *a,neo_genv_t *e) {
    bool done;
    if (L(0)==0xa6598) {
        ga_place(a,e,S16(W(0x28)),S16(W(0x2c)),ROOT);
        SW(0x66,0xe0ed);SW(0x1e,0xe0ed);
        uint16_t k=ga_rand_range(e,5);                                   /* 0A678A */
        START(rl(e,0x2a653cu+4u*k));ga_palette(a,rw(e,0x2a6550u+2u*k));
        SW(0x26,(uint16_t)(((ga_rand(e)&0x1fu)+0x10u)<<4));
        uint8_t angle=(uint8_t)(ga_rand(e)&0xffu);
        uint32_t speed=(uint32_t)(ga_rand_range(e,0x40)+0x40u)<<16,vx,vy;
        ga_polar(speed,angle,&vx,&vy);
        SL(0x7e,vx+L(0x28));SL(0x80,vy+L(0x2c));
        SL(0,0xa65f6);
    }
    SW(0x30,(uint16_t)scale(e,(int16_t)(W(0x7e)-W(0x28)),0x80,0x10,2));
    SW(0x34,(uint16_t)scale(e,(int16_t)(W(0x80)-W(0x2c)),0x80,0x10,2));
    ga_move(a,e);STEP(done);(void)done;
    if (!(W(0x30)|W(0x34))) return GA_DELETE;
    return e->unsupported?GA_UNSUPPORTED:GA_MORE;
}
/* ---- burning engine 0A5D6A ------------------------------------------------------ */
static ga_result_t engine_tick(neo_gactor_t *a,neo_genv_t *e) {
    bool done;
    if (L(0)==0xa5d6a) {
        on_creator(a,e,(B(0x15)&1u)?0x58:-0x58,0x30);
        START(0x2a5626);SW(0x66,0xfff1);SW(0x1e,0xfff1);SET(0x15,2);SW(0x7e,0x14);SL(0,0xa5db2);
    }
    neo_gactor_t *b=body(a,e);if (!b) STOP(0xa5db2);
    if (ga_b(b,0xa2)&0x40u) return GA_DELETE;
    if (!W(0x7e)) {
        SW(0x7e,0x14);
        neo_gactor_t *c=debris_list(a,e,0x2a6468u,1);
        if (c) ga_sb(c,0x15,(uint8_t)(ga_b(c,0x15)&~4u));
    } else SW(0x7e,W(0x7e)-1u);
    ga_project(a,e);STEP(done);(void)done;
    return e->unsupported?GA_UNSUPPORTED:GA_MORE;
}
/* ---- hatch 0A5E88 ------------------------------------------------------------- */
static ga_result_t hatch_tick(neo_gactor_t *a,neo_genv_t *e) {
    if (L(0)==0xa5e88) {on_creator(a,e,0,0x78);SW(0x66,0);SW(0x1e,0);ga_palette(a,0x26);SL(0,0xa5ea6);}
    if (L(0)==0xa5ea6) {START(0x2a5950);SL(0,0xa5ebc);}
    ga_project(a,e);
    if (body_bit(a,e,3)) STOP(0xa5eda);                                  /* the soldiers 09D846.. (2A63D0): untraced */
    ga_project(a,e);
    neo_gactor_t *b=body(a,e);if (!b) STOP(0xa6058);
    if (ga_b(b,0x5f)&0x80u) return GA_DELETE;
    return e->unsupported?GA_UNSUPPORTED:GA_MORE;
}
/* ---- bay 0A4DB6 and its soldiers 0A4F3A / 0A4F52 ------------------------------------ */
static ga_result_t bay_tick(neo_gactor_t *a,neo_genv_t *e) {
    if (L(0)==0xa4db6) {on_creator(a,e,0,0x78);SB(0x8b,0x10);SW(0x86,0x78);SL(0,0xa4dda);}
    neo_gactor_t *b=body(a,e);if (!b) STOP(0xa4dda);
    if (ga_b(b,0x5f)&0x80u) return GA_DELETE;
    ga_project(a,e);
    /* D0 bits 8..11 reach +8B through lsr.l #4 (0A4F1E): after 01ABC8 D0
     * is the projected +3C long; after 0040BC its low word is small. */
    uint32_t prev=L(0x3c);
    if (!BIT(0x8b,0)) {prev=0;neo_gactor_t *c=child(a,e,0xa4f3a,0x1c);if (c) {SET(0x8b,0);ga_sb(c,0x8d,0);}}
    if (!BIT(0x8b,1)) {prev=0;neo_gactor_t *c=child(a,e,0xa4f52,0x1c);if (c) {SET(0x8b,1);ga_sb(c,0x8d,1);}}
    if (W(0x86)) {SW(0x86,W(0x86)-1u);return GA_MORE;}
    uint8_t d0=B(0x8b)&3u;
    if (!d0) return GA_MORE;
    if (d0==3u) {SB(0x8b,B(0x8b)^0x30u);d0=(uint8_t)(((prev&0xf00u)>>4)|((B(0x8b)&0x30u)>>4));}
    SB(0x8b,B(0x8b)|(uint8_t)(d0<<2));
    SW(0x86,(ga_rand(e)&0x7fu)+0x96u);
    return GA_MORE;
}
static void k5570(neo_gactor_t *a,neo_genv_t *e) {                              /* 0A5570 */
    if (S8(B(0x8d))<0) return;
    neo_gactor_t *c=creator(a,e);
    if (c) ga_sb(c,0x8b,(uint8_t)(ga_b(c,0x8b)&~(1u<<(B(0x8d)&7u))));
    SB(0x8d,0xff);
}
/* 0A530C (program callback): a bomb 0A5398 after the script actor. */
static void bomb_drop(neo_gactor_t *a,neo_genv_t *e) {
    neo_gactor_t *c=e->spawn?e->spawn(e->ctx,NULL,0xa5398,true):NULL;
    if (!c) return;
    ga_sl(c,0x6e,L(0x6e));
    uint8_t d2=B(0x89);ga_sb(c,0x89,d2);
    ga_place_from(c,a,e,S16(rw(e,0xa534eu+4u*d2)),S16(rw(e,0xa5350u+4u*d2)));
}
/* 0A535E: the nearest player's side (0..3), +89 when none. */
static uint8_t aim(neo_gactor_t *a,neo_genv_t *e) {
    unsigned m=e->players.active_mask&3u;
    if (!m) return B(0x89);
    bool p2;
    if (m==1u) p2=false; else if (m==2u) p2=true;
    else {
        uint16_t d1=(uint16_t)(e->players.p1x-W(0x3c)),d2=(uint16_t)(e->players.p2x-W(0x3c));
        if (d1&0x8000u) d1=(uint16_t)(0u-d1);
        if (d2&0x8000u) d2=(uint16_t)(0u-d2);
        p2=S16(d2)<S16(d1);
    }
    int16_t d1=(int16_t)((p2?e->players.p2x:e->players.p1x)-W(0x3c));
    if (d1<-0x60) return 0;
    if (d1<0) return 1;
    if (d1<0x60) return 2;
    return 3;
}
static ga_result_t soldier_body_check(neo_gactor_t *a,neo_genv_t *e,uint32_t next) {        /* 0A5166 / 0A5280 */
    neo_gactor_t *b=body(a,e);if (!b) STOP(0xa5166);
    if (ga_b(b,0x5f)&0x80u) SL(0,next);
    return e->unsupported?GA_UNSUPPORTED:GA_MORE;
}
static ga_result_t soldier_tick(neo_gactor_t *a,neo_genv_t *e) {
    bool done;
    for (unsigned guard=0;guard<16;guard++) switch (L(0)) {
    case 0xa4f3a: case 0xa4f52:
        if (L(0)==0xa4f3a) {on_creator(a,e,-0x28,-0x40);SET(0x15,0);} else on_creator(a,e,0x28,-0x40);
        SB(0x5e,B(0x5e)|0x80u);ga_palette(a,0x26d);SW(0x66,0);SW(0x1e,0);SET(0x15,2);SB(0x89,2);
        SB(0x8c,(ga_rand(e)&3u)+1u);
        START(0x2a5a62);SL(0x30,(B(0x15)&1u)?(uint32_t)-0x1800:0x1800u);SL(0x34,0x11111);SW(0x86,0x1e);SL(0,0xa4fde);
        /* fall through */
    case 0xa4fde:
        SW(0x86,W(0x86)-1u);
        if (S16(W(0x86))>=0) {ga_move(a,e);STEP(done);(void)done;return soldier_body_check(a,e,0xa52d6);}
        START(0x2a5a90);k1b1dc(a);SL(0,0xa5012);
        /* fall through */
    case 0xa5012:
        ga_project(a,e);STEP(done);
        if (done) SL(0,0xa502c);
        return soldier_body_check(a,e,0xa52d6);
    case 0xa502c:
        SW(0x86,rank(a,e,0x2c0ba8));SW(0x86,W(0x86)+(ga_rand(e)&0x1fu));
        START(0x2a5b14);k1b1dc(a);SL(0,0xa5066);
        /* fall through */
    case 0xa5066: {
        neo_gactor_t *c=creator(a,e);if (!c) STOP(0xa5066);
        uint8_t bit=(uint8_t)((B(0x8d)+2u)&7u);
        if ((ga_b(c,0x8b)>>bit)&1u) {
            ga_sb(c,0x8b,(uint8_t)(ga_b(c,0x8b)&~(1u<<bit)));
            SB(0x8a,4);SL(0x7e,0xa5190);goto turn;
        }
        SW(0x86,W(0x86)-1u);
        if (S16(W(0x86))>=0) {ga_project(a,e);STEP(done);(void)done;return soldier_body_check(a,e,0xa52d6);}
        uint8_t d0=aim(a,e);
        if (BIT(0x15,0)) d0=(uint8_t)(3u-d0);
        SB(0x8a,(uint8_t)(d0+d0));SL(0x7e,0xa50b6);
    }
    turn:                                                               /* 0A5102 */
        START(0x2a5b66);SB(0x88,(uint8_t)(B(0x89)*2u));SL(0,0xa5122);
        /* fall through */
    case 0xa5122:
        if (!(e->video&3u)) {
            int8_t d0=(int8_t)(B(0x8a)-B(0x88));
            if (!d0) {SB(0x89,(uint8_t)(B(0x88)>>1));SL(0,L(0x7e));continue;}   /* 0A5156: jmp (+7E) */
            SB(0x88,(uint8_t)(B(0x88)+(d0<0?0xffu:1u)));
        }
        ga_project(a,e);STEP(done);(void)done;
        return soldier_body_check(a,e,0xa52d6);
    case 0xa50b6:
        sound(e,0x1067);
        START(rl(e,0x2a63dcu+4u*B(0x89)));SB(0x8c,B(0x8c)-1u);SL(0,0xa50e8);
        /* fall through */
    case 0xa50e8:
        ga_project(a,e);STEP(done);
        if (done) SL(0,0xa502c);
        return soldier_body_check(a,e,0xa52d6);
    case 0xa5190:
        START(0x2a5be8);k1b1dc(a);SL(0x34,0xa000);SL(0,0xa51b4);
        /* fall through */
    case 0xa51b4:
        ga_move(a,e);STEP(done);
        if (done) SL(0,0xa51ce);
        return soldier_body_check(a,e,0xa52d6);
    case 0xa51ce:
        sound(e,0x1020);START(0x2a5c4e);CLR(0x15,2);SW(0x66,0x8800);SW(0x1e,0x8800);SW(0x2c,W(0x2c)-0x20u);
        to_root(a,e);k1b1dc(a);SL(0x82,0xffffe000u);SL(0x50,0x2a600a);SW(0x4e,rank(a,e,0x2c0c0a));SL(0,0xa5232);
        /* fall through */
    case 0xa5232:
        SL(0x34,L(0x34)+L(0x82));ga_air(a,e);STEP(done);(void)done;
        if (ga_register(a,e,NULL,NULL)) SB(0x5a,0);
        if (BIT(0x5f,7)) SL(0,0xa52a4);
        ga_attack(a,e,0x2a5fbe);
        if (BIT(0x15,6)) SL(0,0xa5296);
        return soldier_body_check(a,e,0xa52d6);
    case 0xa52a4: {
        uint16_t d0=ga_rand(e)&0xffu;
        for (uint32_t at=0x2c0c6c;;at+=4) {
            uint16_t t=rw(e,at);if (e->unsupported) return GA_UNSUPPORTED;
            d0=(uint16_t)(d0-t);
            if (S16(d0)>0) continue;
            uint16_t kind=rw(e,at+2);
            if (kind<0x5au) {
                neo_gactor_t *c=after_script(e,0x8c374);
                if (c) {ga_sb(c,0x8b,0);ga_sw(c,0x70,0);ga_sw(c,0x6e,kind);ga_sw(c,0x72,0);ga_sb(c,0x7f,0);ga_place_at(c,a,e,0,0);}
            }
            break;
        }
    }   /* fall through */
    case 0xa5296: case 0xa52d6: {
        sound(e,0x102f);k5570(a,e);
        for (unsigned i=0;i<8;i++) {                                    /* 01DBAE */
            neo_gactor_t *c=e->spawn_reserve?e->spawn_reserve(e->ctx,a,0x1d31c,0x2a):e->spawn?e->spawn(e->ctx,a,0x1d31c,false):NULL;
            if (c) ga_place_from(c,a,e,0,0x18);
        }
        SB(0x1d,0);SW(0x66,0x1001);SW(0x1e,0x1001);SL(0x50,0);          /* 009F88 */
        SL(0,0x1c71e);return neo_effects_tick(a,e);
    }
    default: STOP(L(0));
    }
    STOP(L(0));
}
/* ---- bomb 0A5398 -------------------------------------------------------------- */
static ga_result_t bomb_tick(neo_gactor_t *a,neo_genv_t *e) {
    bool done;
    if (L(0)==0xa5398) {
        SET(0x15,5);CLR(0x15,2);SW(0x66,0xe0ef);SW(0x1e,0xe0ef);START(0x2a5f54);
        ga_palette(a,0x258);ga_palette(a,0x880);ga_palette(a,0x258);ga_palette(a,0x881);
        uint32_t at=0x2a63ecu+6u*B(0x89);
        int16_t d1=S16(rw(e,at)),d2=S16(rw(e,at+2)),d3=S16(rw(e,at+4));
        if (BIT(0x15,0)) d1=(int16_t)-d1;
        SL(0x30,(uint32_t)((int32_t)d1<<8));SL(0x82,(uint32_t)((int32_t)d2<<8));SL(0x34,(uint32_t)((int32_t)d3<<8));
        SL(0,0xa5442);
    }
    SL(0x34,L(0x34)+L(0x82));ga_air(a,e);STEP(done);(void)done;
    if (BIT(0x15,7)) SL(0,0xa54ac);
    if (ga_attack(a,e,0x2a5fce)) SL(0,0xa54b8);
    neo_gactor_t *b=body(a,e);if (!b) STOP(0xa5480);
    if (ga_b(b,0x5f)&0x80u) SL(0,0xa54b8);
    if (ga_cull_rect(a,e,0x2a6368)) SL(0,0xa54e2);
    return e->unsupported?GA_UNSUPPORTED:GA_MORE;
}
static ga_result_t bomb_end(neo_gactor_t *a,neo_genv_t *e) {
    switch (L(0)) {
    case 0xa54ac: SB(0x1d,0);SL(0,0x94ff8);return neo_tank_tick(a,e);
    case 0xa54b8: sound(e,0x1022);SB(0x1d,0);SW(0x66,0x1001);SW(0x1e,0x1001);SL(0x50,0);SL(0,0x1c706);return neo_effects_tick(a,e);
    case 0xa54e2: return GA_DELETE;
    }
    STOP(L(0));
}
/* ---- dispatch ------------------------------------------------------------------- */
bool neo_m3mid_routine(uint32_t r) {
    switch (r) {
    case 0xa4a94: case 0xa4b52: case 0xa4b68: case 0xa4b9c: case 0xa4baa: case 0xa4bc0: case 0xa4bce:
    case 0xa4bda: case 0xa4c24: case 0xa4c3a: case 0xa4c68: case 0xa4c7e: case 0xa4cb2: case 0xa4cc8: case 0xa4cf2:
    case 0xa4d04: case 0xa4d3c: case 0xa4d52: case 0xa4db0:
    case 0xa606e: case 0xa6078: case 0xa60be:
    case 0xa61e8: case 0xa61f2: case 0xa6260: case 0xa6270: case 0xa628c: case 0xa62a4: case 0xa62be: case 0xa62d6:
    case 0xa62f0: case 0xa6308: case 0xa6326:
    case 0xa64e2: case 0xa6504: case 0xa656c: case 0xa6598: case 0xa65f6: case 0xa5d6a: case 0xa5db2:
    case 0xa5e88: case 0xa5ea6: case 0xa5ebc: case 0xa4db6: case 0xa4dda:
    case 0xa4f3a: case 0xa4f52: case 0xa4fde: case 0xa5012: case 0xa502c: case 0xa5066: case 0xa5122: case 0xa50b6:
    case 0xa50e8: case 0xa5190: case 0xa51b4: case 0xa51ce: case 0xa5232: case 0xa52a4: case 0xa5296: case 0xa52d6:
    case 0xa5398: case 0xa5442: case 0xa54ac: case 0xa54b8: case 0xa54e2:
        return true;
    }
    return false;
}
ga_result_t neo_m3mid_tick(neo_gactor_t *a,neo_genv_t *e) {
    uint32_t r=L(0);
    if (r==0xa4db0) {if (e->flags) e->flags->kill_request=0xff;return GA_MORE;}     /* 004BFE */
    if (r>=0xa4a94 && r<=0xa4d52) return body_tick(a,e);
    if (r==0xa606e || r==0xa6078 || r==0xa60be) return side_tick(a,e);
    if (r==0xa61e8 || r==0xa61f2 || (r>=0xa6260 && r<=0xa6326)) return gun_tick(a,e);
    if (r==0xa64e2 || r==0xa6504 || r==0xa656c) return part_tick(a,e);
    if (r==0xa6598 || r==0xa65f6) return debris_tick(a,e);
    if (r==0xa5d6a || r==0xa5db2) return engine_tick(a,e);
    if (r==0xa5e88 || r==0xa5ea6 || r==0xa5ebc) return hatch_tick(a,e);
    if (r==0xa4db6 || r==0xa4dda) return bay_tick(a,e);
    if (r==0xa5398 || r==0xa5442) return bomb_tick(a,e);
    if (r==0xa54ac || r==0xa54b8 || r==0xa54e2) return bomb_end(a,e);
    return soldier_tick(a,e);
}
#endif
