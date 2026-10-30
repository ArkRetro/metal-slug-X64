#include "neo_region.h"
/* Arabian swordsmen of the covered wagon (spawn-table entries 039590 and
 * 0395B6, Mission 1 camera 2336) and the shared death of the swordsman
 * figures (0A93CE: pieces 0A9510..0A9814 moving with 0A98BE), manual
 * translation; ROM labels are provenance only.
 *
 * 039590 waits for the camera progress +74, 0395B6 for bit +74 of RAM
 * 10C606 to change; then (03965C) the swordsman walks towards the players
 * (039694, speed +86 from the rank table 2D257C, animation by +90), turns
 * (039796), waits (0397CC) and attacks (03973A: slash 039B60 whose program
 * end sets the swordsman's +60, attack row 03A004 while its +91 is set).
 * Every state ends in 039B16 (cull 03A014, 004C24 registration; a killing
 * hit goes to 039820). 039820 picks, from the hit type +5A, the direction
 * +5B and one 0029F2 draw, either the death in pieces 0A93CE or the flight
 * 03993A (sword 039C62, blood 040F88, 0399DC: blinking from +8E, then
 * 004188). 03C526 asks 019240 for a mission bonus: RAM 10C2F6 is 0 in
 * Mission 1 (RAM dumps), so it never creates 03C62A here.
 *
 * Every path that would reach the cartridge check (2FFFE0) ends in 004188
 * (delete) on genuine hardware: that is what is translated. RAM 10FD8F is 0
 * in the recordings (01B2BE carry): the piece palettes of that setting. */
#define B(o) ga_b(a,(o))
#define W(o) ga_w(a,(o))
#define L(o) ga_l(a,(o))
#define SB(o,v) ga_sb(a,(o),(uint8_t)(v))
#define SW(o,v) ga_sw(a,(o),(uint16_t)(v))
#define SL(o,v) ga_sl(a,(o),(uint32_t)(v))
#define SET(o,b) SB(o,B(o)|(1u<<(b)))
#define CLR(o,b) SB(o,B(o)&~(1u<<(b)))
#define BIT(o,b) ((B(o)>>(b))&1u)
#define START(t) do { if (!ga_anim_start(a,e,(t),0x100)) return GA_UNSUPPORTED; } while (0)
#define STEP() do { if (ga_anim_step(a,e,arab_call)<0) return GA_UNSUPPORTED; } while (0)
static bool arab_call(neo_gactor_t *a,neo_genv_t *e,neo_anim_kind_t kind,uint32_t routine,uint16_t parameter);
static int16_t S16(uint16_t v) {return (int16_t)v;}
static int32_t S32(uint32_t v) {return (int32_t)v;}
static uint16_t rw(uint32_t address) {const uint8_t *p=ga_rom(address);return p?(uint16_t)(p[0]<<8|p[1]):0;}
static uint32_t rl(uint32_t address) {return (uint32_t)rw(address)<<16|rw(address+2);}
static neo_gactor_t *parent(neo_gactor_t *a,neo_genv_t *e) {return e->creator?e->creator(e->ctx,a):NULL;}
static uint16_t rank(neo_gactor_t *a,neo_genv_t *e,uint32_t table) {
    const uint8_t *t=ga_rom(table);
    if (!t) {ga_fail(e,table);return 0;}
    return ga_rank_word(a,e,t);
}
static neo_gactor_t *spawn(neo_gactor_t *a,neo_genv_t *e,uint32_t routine) {return e->spawn?e->spawn(e->ctx,a,routine,false):NULL;}
static void sound(neo_genv_t *e,uint16_t id) {if (e->sound) e->sound(e->ctx,id);}
static void gravity_air(neo_gactor_t *a,neo_genv_t *e) {SL(0x34,L(0x34)+0xffffb000u);ga_air(a,e);}   /* 0143DC */
/* 009F88: the actor's palettes are released. */
static void free_palettes(neo_gactor_t *a) {SB(0x1d,0);}
/* 01B242: depth words and flips (bits 0..2 set) to the child. */
static void k1b242(neo_gactor_t *c,const neo_gactor_t *a) {
    ga_sw(c,0x1e,W(0x1e));ga_sw(c,0x66,W(0x66));
    for (unsigned b=0;b<3;b++) if (BIT(0x15,b)) ga_sb(c,0x15,(uint8_t)(ga_b(c,0x15)|(1u<<b)));
}
/* 039D74: motion by state, then the program (00427A); 1 complete. */
static int k39d74(neo_gactor_t *a,neo_genv_t *e) {
    if (B(0x15)&0x40u) {
        if (!L(0x62)) ga_stationary(a,e); else {uint32_t kind;ga_ground(a,e,&kind);}
    } else gravity_air(a,e);
    return ga_anim_step(a,e,arab_call);
}
/* 039E6C: standing program by +6E. */
static bool k39e6c(neo_gactor_t *a,neo_genv_t *e) {return ga_anim_start(a,e,W(0x6e)?0x23c8f8:0x23c8b0,0x100);}
/* 039DA8: common construction. */
static bool k39da8(neo_gactor_t *a,neo_genv_t *e) {
    if (!k39e6c(a,e)) return false;
    SW(0x66,0x8820);SW(0x1e,0x8820);
    ga_palette(a,0x17);ga_palette(a,0x20b);ga_palette(a,0x3ba);SB(0x14,B(0x19));
    if (W(0x70)) SET(0x15,0); else CLR(0x15,0);
    ga_floor(a,e);
    SW(0x86,rank(a,e,0x2d257c));SW(0x88,rank(a,e,0x2d25de));SW(0x4e,rank(a,e,0x2d2640));
    uint16_t d0=(uint16_t)((ga_rand(e)&0xfu)-7u);
    SW(0x88,(uint16_t)(0u-(uint16_t)(W(0x88)+d0)));
    SW(0x72,W(0x72)&1u);
    SL(0x30,0);SL(0x34,0);SL(0x62,0);                               /* 01B1DC */
    SB(0x93,0);
    /* 03C526 -> 019240: RAM 10C2F6 = 0 (Mission 1) clears the carry: no 03C62A. */
    return true;
}
/* 039EDA: (rand & 7) * 8 + 0x40. */
static uint16_t k39eda(neo_genv_t *e) {return (uint16_t)(((ga_rand(e)&7u)<<3)+0x40u);}
/* 039EEC: walking speed +62 = +86 << 8 towards the facing. */
static void k39eec(neo_gactor_t *a) {uint32_t d0=(uint32_t)W(0x86)<<8;if (!BIT(0x15,0)) d0=0u-d0;SL(0x62,d0);}
/* 039E9A: carry (true) when no player is in the rank's rectangle. */
static bool k39e9a(neo_gactor_t *a,neo_genv_t *e) {
    uint16_t d0=rank(a,e,0x2d26a2);
    if (d0>3u) {ga_fail(e,0x39eae);return true;}                     /* TRAP #F */
    return ga_players_in(a,e,rl(0x3a01cu+4u*d0));
}
/* 039ED0: carry when no player is in 03A04C. */
static bool k39ed0(neo_gactor_t *a,neo_genv_t *e) {return ga_players_in(a,e,0x3a04c);}
/* 039F0C: carry when a player is within the reach +88 in front. */
static bool k39f0c(neo_gactor_t *a,neo_genv_t *e) {
    if (W(0x72)) return false;
    if (!(W(0x8a)&0x8000u)) {
        int16_t d0=S16((uint16_t)(W(0x8c)-W(0x28)));
        if (d0>=0) d0=(int16_t)-d0;
        if (d0<=S16(W(0x88))) return true;
    }
    int16_t d1=(int16_t)0x8000;
    for (int p=1;p>=0;p--) {                                         /* 01AEB0 */
        if (!(e->players.active_mask&(1u<<p))) continue;
        int16_t d2=S16((uint16_t)((p?e->players.p2x:e->players.p1x)-W(0x3c)));
        if (!BIT(0x15,0)) d2=(int16_t)-d2;
        if (d2>d1) d1=d2;
    }
    if (d1==(int16_t)0x8000) return false;
    if (d1<=S16(W(0x88))) return true;
    SW(0x8a,(uint16_t)d1);
    if (d1<0) SW(0x8c,W(0x28));
    return false;
}
/* 039B4A: cull (03A014): delete. */
static bool k39b4a(neo_gactor_t *a,neo_genv_t *e) {return ga_cull(a,e,0x3a014);}
/* 039B16: common end of the walking states. */
static ga_result_t k39b16(neo_gactor_t *a,neo_genv_t *e) {
    if (k39b4a(a,e)) return GA_DELETE;
    if (ga_register(a,e,NULL,NULL)) {
        if (B(0x5f)&0x80u) SL(0,0x39820); else SB(0x5a,0);
    }
    if (B(0x93)) SB(0x93,B(0x93)-1u);
    return GA_MORE;
}
/* 0399DC/039C8C tail: blink for +8E updates, then leave (004188). */
static ga_result_t blink(neo_gactor_t *a) {
    if (!BIT(0x5f,3)) {
        uint8_t d0=B(0x8e);
        if (d0) {if (!--d0) {SET(0x5f,3);d0=0x1e;}SB(0x8e,d0);}
        return GA_MORE;
    }
    uint8_t d0=B(0x8e);
    if (d0) {SB(0x8e,d0-1u);return GA_MORE;}
    return GA_DELETE;
}
/* 039AB2: flight motion. */
static void k39ab2(neo_gactor_t *a,neo_genv_t *e) {
    if (B(0x15)&0x40u) {
        SL(0x62,L(0x62)+(uint32_t)(S32(L(0x7e)-L(0x62))>>3));
        uint32_t kind;ga_ground(a,e,&kind);
    } else {
        SL(0x34,L(0x34)+(uint32_t)(S32(0xfff40000u-L(0x34))>>4));
        SL(0x30,L(0x30)+(uint32_t)(S32(L(0x7e)-L(0x30))>>1));
        SL(0x62,L(0x30));ga_air(a,e);
    }
    SB(0x8f,(B(0x15)&0x40u)?0xff:0);
    if (B(0x15)&0x40u) SL(0x7e,0);
}
/* 0A9E2E: polar speed of a piece from +8C/+92. */
static void k9e2e(neo_gactor_t *c) {uint32_t vx,vy;ga_polar(ga_l(c,0x8c),ga_b(c,0x92),&vx,&vy);ga_sl(c,0x30,vx);ga_sl(c,0x34,vy);}
/* 0A9CB6: the pieces of list `tables` (four lists by direction and draw). */
static void pieces(neo_gactor_t *a,neo_genv_t *e,uint32_t tables) {
    unsigned d0=ga_rand(e)&1u;if (B(0x5b)&0x80u) d0+=2u;
    uint32_t list=rl(tables+4u*d0);
    unsigned d7=ga_rand(e)&3u,d6=0;
    for (;;list+=8u) {
        uint32_t routine=rl(list);
        if (routine==0xffffffffu) return;
        if (routine==0xa9788u) {
            if (!W(0x76)) return;
            if (++d6>5u) return;
            if ((int16_t)d6>S16(W(0x76))) return;
        }
        neo_gactor_t *c=spawn(a,e,routine);
        if (c) ga_place_at(c,a,e,0,0);                              /* 01ACF2 */
        const uint8_t *idx=ga_rom(list+4u+d7);
        unsigned d1=idx?idx[0]:0;
        const uint8_t *ang=ga_rom(0xe6206u+d1);
        uint32_t speed=(uint32_t)((int32_t)S16(rw(0xe61d4u+2u*d1))<<8);
        uint32_t r=((((uint32_t)ga_rand(e))&0x1fffu)<<4)-0x10000u;
        speed+=r;if (S32(speed)>0x80000) speed=0x80000;
        uint8_t da=(uint8_t)((ga_rand(e)&7u)-4u);
        if (c) {ga_sb(c,0x92,(uint8_t)((ang?ang[0]:0)+da));ga_sl(c,0x8c,speed);k9e2e(c);}
    }
}
/* 0A93CE: death in pieces, then 0A946C. */
static ga_result_t death(neo_gactor_t *a,neo_genv_t *e) {
    pieces(a,e,0xe6220);
    if (W(0x78)) {ga_fail(e,0xa9dca);return GA_UNSUPPORTED;}          /* 0A9DC0 items: none here */
    SL(0x50,0);
    SB(0x14,B(0x1b));                                               /* 01B2BE carry: RAM 10FD8F = 0 */
    START(0x2a769c);
    SB(0x69,0xff);                                                  /* 019DFC */
    SL(0,0xa946c);
    ga_stationary(a,e);
    int r=ga_anim_step(a,e,arab_call);
    if (r<0) return GA_UNSUPPORTED;
    if (r) return GA_DELETE;
    return ga_cull(a,e,0xe64d8)?GA_DELETE:GA_MORE;                  /* 0A9A56 */
}
static ga_result_t arab_tick(neo_gactor_t *a,neo_genv_t *e) {
    for (;;) switch (L(0)) {
    case 0x39590: if (!k39da8(a,e)) return GA_UNSUPPORTED;SL(0,0x3959c);return GA_MORE;
    case 0x3959c:
        if (k39d74(a,e)<0) return GA_UNSUPPORTED;
        if (!(e->camera<W(0x74))) SL(0,0x3965c);
        return k39b16(a,e);
    case 0x395b6:
        if (!k39da8(a,e)) return GA_UNSUPPORTED;
        SB(0x94,e->flag606?*e->flag606:0);SL(0,0x395c8);return GA_MORE;
    case 0x395c8: {
        if (k39d74(a,e)<0) return GA_UNSUPPORTED;
        uint8_t d0=(uint8_t)((e->flag606?*e->flag606:0)^B(0x94));
        if ((d0>>(W(0x74)&31u))&1u) SL(0,0x3965c);
        return k39b16(a,e);
    }
    case 0x3965c:
        SL(0x50,0x39fe0);
        {uint16_t d0=k39eda(e);SW(0x82,d0);SW(0x84,d0);}
        SB(0x90,0);
        if (W(0x7c)!=W(0x70)) {SL(0,0x39796);continue;}
        /* fall through */
    case 0x39682:
        SW(0x8a,0);k39eec(a);
        if (!ga_anim_start(a,e,rl(0x39fb0u+4u*B(0x90)),0x100)) return GA_UNSUPPORTED;
        SL(0,0x39694);
        /* fall through */
    case 0x39694:
        if (k39d74(a,e)<0) return GA_UNSUPPORTED;
        if (B(0x15)&0x40u) {
            if (BIT(0x5f,0)) SL(0,0x39796);
            if (k39e9a(a,e)) {
                if (!W(0x72)) {uint16_t c=W(0x84);SW(0x84,c-1u);if (!c) SL(0,0x397cc);}
            } else {
                SW(0x84,W(0x82));SW(0x72,W(0x72)&0xfffdu);
                if (k39f0c(a,e)) SL(0,0x39796);
            }
            if (!k39ed0(a,e) && !B(0x93)) SL(0,0x3973a);          /* 039700 */
        }
        return k39b16(a,e);
    case 0x3973a: {
        neo_gactor_t *c=spawn(a,e,0x39b60);
        if (c) k1b242(c,a);
        SB(0x60,0);
        if (!ga_anim_start(a,e,rl(0x39fc8u+4u*B(0x90)),0x100)) return GA_UNSUPPORTED;
        SL(0,0x3975e);
    }   /* fall through */
    case 0x3975e:
        if (B(0x60)) {SB(0x93,0x0f);SL(0,0x39682);continue;}
        if (k39d74(a,e)<0) return GA_UNSUPPORTED;
        return k39b16(a,e);
    case 0x39796:
        START(0x23c97c);SL(0x62,0);SL(0,0x397b0);
        /* fall through */
    case 0x397b0: {
        int r=k39d74(a,e);if (r<0) return GA_UNSUPPORTED;
        if (r) {SB(0x15,B(0x15)^1u);SB(0x90,0);SL(0,0x39682);}
        return k39b16(a,e);
    }
    case 0x397cc:
        START(0x23c8f8);
        {uint16_t d0=k39eda(e);SW(0x82,d0);SW(0x84,d0);}
        SL(0x62,0);SL(0x30,0);SL(0,0x397f6);
        /* fall through */
    case 0x397f6: {
        if (k39d74(a,e)<0) return GA_UNSUPPORTED;
        uint16_t c=W(0x84);SW(0x84,c-1u);
        if (!c) {SW(0x72,W(0x72)|2u);SL(0,0x3965c);}
        if (!k39e9a(a,e)) SL(0,0x3965c);
        return k39b16(a,e);
    }
    case 0x39820: {
        sound(e,0x103f);
        uint8_t t=B(0x5a);bool fly;
        if (t==0x0bu) fly=false;
        else if (t!=0x0au && ((B(0x5b)+1u)&7u)>2u) fly=false;
        else if (ga_rand_range(e,4)) fly=t>=0x14u || !(t==2u || t==9u);
        else fly=t<0x14u && (t==3u || t==4u);
        if (!fly) return death(a,e);
        SB(0x69,0xff);                                              /* 019DFC */
        START(0x23c9ba);
        ga_spawn_from(a,e,0x39c62,0,0x10);
        SB(0x15,B(0x15)^1u);ga_spawn_from(a,e,0x40f88,8,0);SB(0x15,B(0x15)^1u);  /* 040ED0 */
        SL(0x30,t==0x0au?0x640000u:0x200000u);SL(0x34,0x60000);SL(0x7e,0x20000);
        if (B(0x5b)&0x80u) {SET(0x15,0);SL(0x30,0u-L(0x30));SL(0x7e,0u-L(0x7e));}
        else CLR(0x15,0);
        SL(0x62,L(0x30));CLR(0x15,6);SL(0,0x399dc);
    }   /* fall through */
    case 0x399dc:
        k39ab2(a,e);STEP();
        if (k39b4a(a,e)) return GA_DELETE;
        return blink(a);
    case 0xa946c: {
        ga_stationary(a,e);
        int r=ga_anim_step(a,e,arab_call);
        if (r<0) return GA_UNSUPPORTED;
        if (r) return GA_DELETE;
        return ga_cull(a,e,0xe64d8)?GA_DELETE:GA_MORE;
    }
    default: ga_fail(e,L(0));return GA_UNSUPPORTED;
    }
}
/* 039B60: slash; its program end sets the swordsman's +60. */
static ga_result_t slash_tick(neo_gactor_t *a,neo_genv_t *e) {
    if (L(0)==0x39b60) {
        START(0x23c836);ga_palette(a,0x17);
        neo_gactor_t *p=parent(a,e);ga_place(a,e,0,0,p?NEO_GA_TAG(p->index):0);
        SB(0x91,0);SL(0,0x39b92);
    }
    ga_project(a,e);
    int r=ga_anim_step(a,e,arab_call);
    if (r<0) return GA_UNSUPPORTED;
    neo_gactor_t *p=parent(a,e);
    if (r) {if (p) ga_sb(p,0x60,1);return GA_DELETE;}
    if (p && (ga_b(p,0x5f)&0x80u)) return GA_DELETE;
    if (B(0x91)) ga_attack(a,e,0x3a004);
    return GA_MORE;
}
/* 039C62: the dropped sword. */
static ga_result_t sword_tick(neo_gactor_t *a,neo_genv_t *e) {
    if (L(0)==0x39c62) {START(0x23ca32);ga_palette(a,0x17);SW(0x26,0);SL(0,0x39c8c);}
    SL(0x30,0);gravity_air(a,e);STEP();
    if (B(0x15)&0x40u) SW(0x26,0x100);
    return blink(a);
}
/* 0A9510..0A9814: pieces; 0A98BE moves them (0A9E4A). */
static ga_result_t piece_tick(neo_gactor_t *a,neo_genv_t *e) {
    static const struct {uint32_t routine,prog0,prog1;uint16_t pal_carry,pal;uint8_t b60,rnd;} T[]={
        {0xa9510,0x269340,0x269340,0x3fd,0x3fd,0,0},{0xa9550,0x2a765a,0x2a765a,0x17,0x17,0,0},
        {0xa9590,0x2a78f0,0x2a796e,0x19,0x18,0,1},{0xa9638,0x2a77f4,0x2a7872,0x1a,0x17,0,1},
        {0xa96e0,0x2a79ec,0x2a7a6a,0x1a,0x17,0,1},{0xa9788,0x2a7ae8,0x2a7b2a,0x17,0x17,0,1},
        {0xa9814,0x2a76f8,0x2a7776,0x19,0x18,1,1}};
    for (unsigned i=0;i<sizeof(T)/sizeof(T[0]);i++) {
        if (L(0)!=T[i].routine) continue;
        uint32_t prog=T[i].prog0;
        if (T[i].rnd && (ga_rand(e)&1u)) prog=T[i].prog1;
        if (!T[i].rnd) {free_palettes(a);ga_palette(a,T[i].pal);START(prog);}
        else {
            START(prog);
            uint8_t d0=ga_rand(e)&3u;
            if (d0) {if (d0&1u) SET(0x15,0);if (d0&2u) SET(0x15,1);}
            free_palettes(a);ga_palette(a,T[i].pal_carry);                /* 01B2BE carry: RAM 10FD8F = 0 */
        }
        SW(0x66,0x8820);SW(0x1e,0x8820);SB(0x61,0);SB(0x60,T[i].b60);
        SL(0,0xa98be);return GA_MORE;
    }
    /* 0A98BE */
    if (L(0x30)&0x80000000u) {if (S32(L(0x30))<=0xe00) SL(0x30,L(0x30)+0xe00u);}
    else if (S32(L(0x30))>=0xe00) SL(0x30,L(0x30)-0xe00u);
    SL(0x34,L(0x34)-0x1000u);gravity_air(a,e);
    if (B(0x15)&0x40u) {
        if (B(0x60)) return GA_DELETE;
        CLR(0x15,6);SET(0x5f,3);SB(0x60,1);
        SL(0x34,(uint32_t)(S32(0u-L(0x34))>>1));
        SW(0x26,W(0x26)+(uint16_t)(ga_rand_range(e,0x200)-0x80u));
    }
    STEP();
    return ga_cull(a,e,0xe64d8)?GA_DELETE:GA_MORE;
}
static bool arab_call(neo_gactor_t *a,neo_genv_t *e,neo_anim_kind_t kind,uint32_t routine,uint16_t parameter) {
    (void)a;(void)e;(void)routine;(void)parameter;
    (void)kind;
    return false;
}
bool neo_arab_routine(uint32_t r) {
    return r==0x39590 || r==0x3959c || r==0x395b6 || r==0x395c8 || r==0x3965c || r==0x39682 || r==0x39694 ||
        r==0x3973a || r==0x3975e || r==0x39796 || r==0x397b0 || r==0x397cc || r==0x397f6 || r==0x39820 ||
        r==0x399dc || r==0xa946c || r==0x39b60 || r==0x39b92 || r==0x39c62 || r==0x39c8c ||
        r==0xa9510 || r==0xa9550 || r==0xa9590 || r==0xa9638 || r==0xa96e0 || r==0xa9788 || r==0xa9814 || r==0xa98be;
}
ga_result_t neo_arab_tick(neo_gactor_t *a,neo_genv_t *e) {
    uint32_t r=L(0);
    if (r==0x39b60 || r==0x39b92) return slash_tick(a,e);
    if (r==0x39c62 || r==0x39c8c) return sword_tick(a,e);
    if (r>=0xa9510u && r!=0xa946cu) return piece_tick(a,e);
    return arab_tick(a,e);
}
