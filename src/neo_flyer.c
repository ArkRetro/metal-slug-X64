#include "neo_region.h"
#include "neo_soldier_ai.h"
/* Rebel bomber of the Mission 1 region (spawn-table entry ROM 0DC97E), its
 * child (0DD3E0, propeller layer), the bombs (0DD642) and their trails
 * (0DD802), manual translation. ROM labels are provenance. This entry clears
 * +7A, so the paratrooper path of the shared code (0DCBDE.., entry 0DC986)
 * is not reachable and is not translated.
 * Fields: +6E facing, +70 cruise height index, +72/+74 stay timers, +76
 * heading offset, +78 item kind (<0x5A: dropped when shot down), +7C palette
 * set, +8A/+8C target point, +8E heading, +90 bomb pause, +9A damping
 * shift, +9B bank (0..6), +9C/+9D bomb delay/count. */
#define B(o) ga_b(a,(o))
#define W(o) ga_w(a,(o))
#define L(o) ga_l(a,(o))
#define SB(o,v) ga_sb(a,(o),(uint8_t)(v))
#define SW(o,v) ga_sw(a,(o),(uint16_t)(v))
#define SL(o,v) ga_sl(a,(o),(uint32_t)(v))
static int16_t S16(uint16_t v) {return (int16_t)v;}
static int32_t S32(uint32_t v) {return v<=0x7fffffffu?(int32_t)v:(int32_t)((int64_t)v-0x100000000LL);}
static uint32_t ASR(uint32_t v,unsigned n) {return (uint32_t)(S32(v)>>n);}
static uint16_t rank(neo_gactor_t *a,neo_genv_t *e,uint32_t table) {
    const uint8_t *t=ga_rom(table);
    if (!t) {ga_fail(e,table);return 0;}
    return ga_rank_word(a,e,t);
}
static void sound(neo_genv_t *e,uint16_t id) {if (e->sound) e->sound(e->ctx,id);}
#define START(t) do { if (!ga_anim_start(a,e,(t),0x100)) return GA_UNSUPPORTED; } while (0)
#define STEP() do { if (ga_anim_step(a,e,NULL)<0) return GA_UNSUPPORTED; } while (0)
/* 0DD472: palette set +7C (tables 0DC8FE/0DC902), flash palette 038A. */
static void palettes(neo_gactor_t *a) {
    SW(0x7c,W(0x7c)&1u);
    const uint8_t *p0=ga_rom(0xdc8feu+2u*W(0x7c)),*p1=ga_rom(0xdc902u+2u*W(0x7c));
    ga_palette(a,p0?(uint16_t)(p0[0]<<8|p0[1]):0);ga_palette(a,p1?(uint16_t)(p1[0]<<8|p1[1]):0);
    ga_flash_palette(a,0x38a);
}
/* 01B29C: octagonal distance. */
static uint16_t distance(uint16_t x0,uint16_t y0,uint16_t x1,uint16_t y1) {
    uint16_t d0=(uint16_t)(x0-x1),d1=(uint16_t)(y0-y1);
    if (d0&0x8000u) d0=(uint16_t)(0u-d0);
    if (d1&0x8000u) d1=(uint16_t)(0u-d1);
    if (S16(d0)>S16(d1)) return (uint16_t)(d0+(d1>>1));
    return (uint16_t)((d0>>1)+d1);
}
/* 0DD850: target point: height from +70, x from the nearest active player. */
static bool target(neo_gactor_t *a,neo_genv_t *e) {
    if (W(0x70)>0x13u) {ga_fail(e,0xdd85c);return false;}
    SW(0x8c,W(0x70)*16u+0x100u);SW(0x8a,0xa0);
    /* 01AF9A */
    const neo_control_frame_t *f=&e->players;
    unsigned m=f->active_mask&3u;
    if (m==1u) SW(0x8a,f->p1x);
    else if (m==2u) SW(0x8a,f->p2x);
    else if (m==3u) {
        uint16_t d1=(uint16_t)(f->p1x-W(0x3c)),d2=(uint16_t)(f->p2x-W(0x3c));
        if (d1&0x8000u) d1=(uint16_t)(0u-d1);
        if (d2&0x8000u) d2=(uint16_t)(0u-d2);
        SW(0x8a,S16(d2)<S16(d1)?f->p2x:f->p1x);
    }
    return true;
}
/* 0DD892: steer towards the target point. */
static void steer(neo_gactor_t *a) {
    int16_t dx=(int16_t)(S16((uint16_t)(W(0x8a)-W(0x3c)))>>1),dy=(int16_t)(S16((uint16_t)(W(0x8c)-W(0x40)))>>1);
    uint16_t angle=neo_angle(dx,dy);
    uint16_t off=W(0x76)&0x1fu;
    if (B(0x15)&1u) off=(uint16_t)(0u-off);
    SW(0x8e,(angle+off)&0xffu);
    uint32_t d0=(uint32_t)distance(W(0x3c),W(0x40),W(0x8a),W(0x8c))<<16;
    d0=B(0xa1)?d0>>4:d0>>5;
    uint32_t vx,vy;ga_polar(d0,(uint8_t)W(0x8e),&vx,&vy);
    vx-=L(0x30);vy-=L(0x34);
    vx=ASR(vx,B(0x9a)&63u);vy=ASR(vy,4);
    SL(0x30,L(0x30)+vx);SL(0x34,L(0x34)+vy);
}
/* 0DD918: bank angle +9B follows the horizontal speed. */
static void bank(neo_gactor_t *a) {
    int32_t v=S32(L(0x30));uint8_t d1;
    static const int32_t edge[6]={-0xc000,-0x8000,-0x4000,0x4000,0x8000,0xc000};
    unsigned k=0;while (k<6 && v>=edge[k]) k++;
    d1=(uint8_t)((B(0x15)&1u)?6u-k:k);
    if (d1>B(0x9b)) SB(0x9b,B(0x9b)+1u);
    else if (d1<B(0x9b)) SB(0x9b,B(0x9b)-1u);
}
/* 0DD5A4: blink between the two palette handles. */
static void blink(neo_gactor_t *a,neo_genv_t *e) {SB(0x14,(e->video&1u)?B(0x1a):B(0x19));}
/* 0DD608: hurtbox registration; true when dead. */
static bool registered_dead(neo_gactor_t *a,neo_genv_t *e) {
    if (ga_register(a,e,NULL,NULL)) SB(0x5a,0);
    return (B(0x5f)&0x80u)!=0;
}
static bool near(neo_gactor_t *a,uint16_t limit) {return S16(distance(W(0x3c),W(0x40),W(0x8a),W(0x8c)))<=S16(limit);} /* 0DDAB4 */
/* 0DD57E: carry when the stay time or the camera distance is exhausted. */
static bool leave(neo_gactor_t *a,neo_genv_t *e) {
    SW(0x92,W(0x92)-1u);
    if (S16(W(0x92))<0) return true;
    return S16((uint16_t)(e->camera-W(0x96)))>S16(W(0x94));
}
/* 0DDAE0 (+7A clear): bomb timing and release. */
static void bombs(neo_gactor_t *a,neo_genv_t *e) {
    if (S16(W(0x90))>0) {SW(0x90,W(0x90)-1u);return;}
    if (B(0x9d)==0xffu) {SB(0x9c,rank(a,e,0x2d07f4));SB(0x9d,rank(a,e,0x2d0856));}
    if ((int8_t)B(0x9c)>0) {SB(0x9c,B(0x9c)-1u);return;}
    if (!B(0x9d)) {SB(0x9d,0xff);SW(0x90,rank(a,e,0x2d0730));SB(0xa2,B(0xa2)+1u);return;}
    ga_spawn_from(a,e,0xdd642,0,0);
    ga_spawn_from(a,e,0xdd802,0,0);
    SB(0x9c,rank(a,e,0x2d07f4));SB(0x9d,B(0x9d)-1u);
}
/* 0DDA06: leaving the view ends the bomber (and its engine sound). */
static ga_result_t tail(neo_gactor_t *a,neo_genv_t *e) {
    if (ga_cull(a,e,0xdc906)) {SB(0x61,1);sound(e,0x10e8|0x8000u);return GA_DELETE;}
    return GA_MORE;
}
static void smoke(neo_gactor_t *a,neo_genv_t *e,uint32_t list,neo_gactor_t **out) { /* 01DA40 */
    neo_gactor_t *c=ga_spawn_from(a,e,0x1d764,0,0);
    if (out) *out=c;
    if (!c) return;
    if (B(0x15)&4u) ga_sb(c,0x15,(uint8_t)(ga_b(c,0x15)|4u));
    ga_sl(c,0x86,list);
}
static ga_result_t bomber(neo_gactor_t *a,neo_genv_t *e) {
    for (unsigned guard=0;guard<8;guard++) switch (L(0)) {
    case 0xdc97e: /* constructor */
        SW(0x7a,0);
        palettes(a);
        sound(e,0x10e8);
        SB(0x15,B(0x15)&~1u);if (W(0x6e)) SB(0x15,B(0x15)|1u);
        SW(0x66,0x8800);SW(0x1e,0x8800);SB(0x61,0);
        ga_spawn_from(a,e,0xdd3e0,0,0);
        SW(0x4e,rank(a,e,0x2d06ce));SW(0x90,rank(a,e,0x2d0792));SB(0x9a,rank(a,e,0x2d0934));
        SB(0x9d,0xff);SL(0x50,0xdc91e);
        START(0x2af47a);
        SB(0x9b,3);SB(0xa1,0);SB(0xa2,0);
        /* 0DD55A */
        SW(0x92,(uint16_t)(W(0x72)<<5)&0x7fffu);SW(0x94,(uint16_t)(W(0x74)<<4)&0xfffu);SW(0x96,e->camera);
        SL(0,0xdca54);continue;
    case 0xdca54:
        if (!target(a,e)) return GA_UNSUPPORTED;
        steer(a);bank(a);blink(a,e);
        if (registered_dead(a,e)) {SL(0,0xdcadc);continue;}
        if (near(a,8)) {SL(0,0xdca9c);continue;}
        bombs(a,e);
        if (leave(a,e)) {SL(0,0xdcbb6);continue;}
        ga_move(a,e);STEP();
        return tail(a,e);
    case 0xdca9c: {
        if (!target(a,e)) return GA_UNSUPPORTED;
        /* 0DD61E: hover */
        SW(0x8e,(W(0x8e)-4u)&0xffu);
        uint32_t vx,vy;ga_polar(0x4000,(uint8_t)W(0x8e),&vx,&vy);(void)vx;
        SL(0x30,0);SL(0x34,vy);
        bank(a);blink(a,e);
        if (registered_dead(a,e)) {SL(0,0xdcadc);continue;}
        if (!near(a,0x10)) {SL(0,0xdca54);continue;}
        bombs(a,e);
        if (leave(a,e)) {SL(0,0xdcbb6);continue;}
        ga_move(a,e);STEP();
        return tail(a,e);
    }
    case 0xdcadc: /* shot down */
        SW(0x8c,0x1ff);START(0x2afb56);SL(0,0xdcaf8); /* fall through */
    case 0xdcaf8:
        steer(a);blink(a,e);
        if (!(e->video&3u)) { /* 0DD4CA */
            neo_gactor_t *c=NULL;smoke(a,e,0xdc8ac,&c);
            if (c) {ga_sw(c,0x28,(uint16_t)(ga_w(c,0x28)+((B(0x15)&1u)?0xffd8u:0x28u)));ga_sw(c,0x2c,(uint16_t)(ga_w(c,0x2c)-0x18u));}
        }
        ga_move(a,e);
        if (S16(W(0x40))>=0x1b0) { /* 0DD4FC: crash */
            neo_gactor_t *x=ga_spawn_from(a,e,0x1c71e,0,-16);(void)x;
            smoke(a,e,0xdc85a,NULL);
            sound(e,0x102f);sound(e,0x10e8|0x8000u);
            /* 0DD5C2 -> 08CDB2: item (+78 < 0x5A) after the script actor. */
            if (W(0x78)<0x5au) {
                neo_gactor_t *it=e->spawn?e->spawn(e->ctx,a,0x8c374,true):NULL;
                if (it) {ga_sb(it,0x8b,0);ga_sw(it,0x70,0);ga_sw(it,0x6e,W(0x78));ga_sw(it,0x72,0);ga_sb(it,0x7f,0);
                    ga_place_at(it,a,e,(B(0x15)&1u)?-16:16,0);}
            }
            SW(0x78,0xffff);
            SB(0x61,1);SB(0x69,0xff); /* 019DFC */
            return GA_DELETE;
        }
        STEP();
        return tail(a,e);
    case 0xdcbb6: /* leave */
        SW(0x8c,0x2ff);SL(0,0xdcbc2); /* fall through */
    case 0xdcbc2:
        steer(a);bank(a);blink(a,e);ga_move(a,e);STEP();
        return tail(a,e);
    default:return GA_UNSUPPORTED;
    }
    return GA_UNSUPPORTED;
}
/* 0DD44C: bank and facing from the creator. */
static void follow_bank(neo_gactor_t *a,neo_gactor_t *p) {
    SB(0x9b,ga_b(p,0x9b));SB(0x15,(B(0x15)&~1u)|(ga_b(p,0x15)&1u));
}
/* 0044F6: flash state from the creator; true (carry) on even values. */
static bool flash_sync(neo_gactor_t *a,neo_gactor_t *p) {
    uint8_t d0=ga_b(p,0x17);
    if (!d0) return false;
    if (!(d0&0x80u)) d0++;
    SB(0x17,d0);
    return !(d0&1u);
}
static ga_result_t propeller(neo_gactor_t *a,neo_genv_t *e) {
    neo_gactor_t *p=e->creator?e->creator(e->ctx,a):NULL;
    if (L(0)==0xdd3e0) {
        ga_place(a,e,0,0,p?NEO_GA_TAG(p->index):0); /* 01AD6E */
        ga_palette(a,0x389);ga_flash_palette(a,0x38b);
        START(0x2af960);SL(0,0xdd418);
    }
    if (!p || ga_b(p,0x61)==1u) return GA_DELETE; /* 0DD4B4 */
    follow_bank(a,p);
    ga_project(a,e);
    SB(0xa5,B(0xa5)+1u);
    if (flash_sync(a,p)) SB(0xa5,1);
    if (B(0xa5)&1u) STEP();
    return GA_MORE;
}
static ga_result_t bomb(neo_gactor_t *a,neo_genv_t *e) {
    for (unsigned guard=0;guard<4;guard++) switch (L(0)) {
    case 0xdd642:
        sound(e,0x109b);ga_palette(a,0x387);START(0x2afc60);
        SL(0x30,0);SL(0x34,0);
        SL(0x86,0u-((uint32_t)(int32_t)S16(rank(a,e,0x2d08b8))<<8));
        SW(0x2c,W(0x2c)-0x18u);SW(0x1e,W(0x1e)-1u);
        SW(0x4e,rank(a,e,0x2d091a));SL(0x50,0xdc92a);
        SB(0x5e,B(0x5e)|0x80u);SB(0x15,B(0x15)|0x20u);
        SL(0,0xdd6be);continue;
    case 0xdd6be: {
        neo_gactor_t *p=e->creator?e->creator(e->ctx,a):NULL;
        bool hit=ga_attack(a,e,0xdc942)!=0;
        if (!hit) {
            if (ga_register(a,e,NULL,NULL)) SB(0x5a,0);
            hit=(B(0x5f)&0x80u)!=0;
        }
        if (!hit && (B(0x15)&0x40u)) { /* landed: 0DD72E -> 09500C splash */
            for (unsigned i=0;i<4;i++) {a->palette[i]=0;ga_sb(a,0x19+i,0);}
            SB(0x1d,0);                             /* 009F88 */
            sound(e,0x1021);
            ga_palette(a,0x378);SW(0x66,0x1001);SW(0x1e,0x1001);START(0x2919fe);
            SL(0,0x9503c);continue;
        }
        if (!hit && p && ga_b(p,0x61)==1u) hit=true;
        if (!hit) {
            SL(0x34,L(0x34)+L(0x86));
            SL(0x82,ASR(0u-L(0x30),4));
            ga_air(a,e);STEP();
            if (ga_cull(a,e,0xdc90e)) return GA_DELETE;
            return GA_MORE;
        }
        sound(e,0x1022);
        ga_spawn_from(a,e,0x1c706,0,0);
        return GA_DELETE;
    }
    case 0x9503c: {
        ga_project(a,e);
        int r=ga_anim_step(a,e,NULL);
        if (r<0) return GA_UNSUPPORTED;
        return r?GA_DELETE:GA_MORE;
    }
    default:return GA_UNSUPPORTED;
    }
    return GA_UNSUPPORTED;
}
static ga_result_t trail(neo_gactor_t *a,neo_genv_t *e) {
    if (L(0)==0xdd802) {ga_palette(a,0x389);SW(0x1e,W(0x1e)-1u);START(0x2afcca);SL(0,0xdd82a);}
    ga_project(a,e);
    int r=ga_anim_step(a,e,NULL);
    if (r<0) return GA_UNSUPPORTED;
    if (r || ga_cull(a,e,0xdc90e)) return GA_DELETE;
    return GA_MORE;
}
bool neo_flyer_routine(uint32_t r) {
    return r==0xdc97e || r==0xdca54 || r==0xdca9c || r==0xdcadc || r==0xdcaf8 || r==0xdcbb6 || r==0xdcbc2 ||
        r==0xdd3e0 || r==0xdd418 || r==0xdd642 || r==0xdd6be || r==0x9503c || r==0xdd802 || r==0xdd82a;
}
ga_result_t neo_flyer_tick(neo_gactor_t *a,neo_genv_t *e) {
    uint32_t r=L(0);
    if (r==0xdd3e0 || r==0xdd418) return propeller(a,e);
    if (r==0xdd642 || r==0xdd6be || r==0x9503c) return bomb(a,e);
    if (r==0xdd802 || r==0xdd82a) return trail(a,e);
    return bomber(a,e);
}
