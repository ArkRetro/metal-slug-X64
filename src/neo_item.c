#include "neo_region.h"
/* Items 08C374 (dropped: +8A set) / 08C37C (script), manual translation;
 * ROM labels are provenance only.
 *
 * The kind +6E selects a record of 0E581A (08D2DA, 0x18 bytes: +1 kept in
 * the item list 10C5C2, +2 appearance program 2861BC first, +4 program, +8
 * palettes, +10 pickup routine, +14 motion routine). 08C53C: lifetime +70
 * (0x258 by default, FFFF = forever; at 0 it blinks (+5F bit 3) for 30 more
 * updates and goes), +86 counts 0x78 updates (+7C), the motion routine,
 * +78 updates before it can be taken, then 08C0CC: once full size (+12 =
 * FF) the rectangle 0E561C against the pickup boxes of the players (RAM
 * 10C5EC/10C5F4, written by the player's code; one player is drawn at random
 * when both touch it). The pickup routines give weapons (C0928), bombs or
 * points (C0BF4, 057948) to the original player: none of that exists for the
 * port's player, so a pickup is an explicit stop (the item is removed and
 * counted). It leaves when outside 0E5624 (0C8BEC) or, with +8B, when its
 * creator is gone (00409A).
 *
 * Only the kinds whose motion is 08B9B2 (fall with 0xA000 gravity, grow +12
 * / +13 by 8 up to FF) are native: 06, 18, 1C, 29, 2A, 2B, 2F, 4B, 4F (H
 * heavy machine gun, family test), 51. The constructor hands any other kind
 * back to the world as an external actor (counted, not created). The item
 * list 10C5C2 (08CBCA/08CBF8) has no reader in the port and is not kept.
 * The path that would reach the cartridge check (2FFFE0, +6E = FFFF) ends in
 * 004188 (delete) on genuine hardware: that is what is translated. */
#define B(o) ga_b(a,(o))
#define W(o) ga_w(a,(o))
#define L(o) ga_l(a,(o))
#define SB(o,v) ga_sb(a,(o),(uint8_t)(v))
#define SW(o,v) ga_sw(a,(o),(uint16_t)(v))
#define SL(o,v) ga_sl(a,(o),(uint32_t)(v))
#define SET(o,b) SB(o,B(o)|(1u<<(b)))
#define BIT(o,b) ((B(o)>>(b))&1u)
#define START(t) do { if (!ga_anim_start(a,e,(t),0x100)) return GA_UNSUPPORTED; } while (0)
enum { RECORDS=0xe581a, MOTION_FALL=0x8b9b2 };
static uint8_t rb(uint32_t address) {const uint8_t *p=ga_rom(address);return p?p[0]:0;}
static uint16_t rw(uint32_t address) {const uint8_t *p=ga_rom(address);return p?(uint16_t)(p[0]<<8|p[1]):0;}
static uint32_t rl(uint32_t address) {return (uint32_t)rw(address)<<16|rw(address+2);}
/* 08B9B2: the fall, and the item grows to full size. */
static void k8b9b2(neo_gactor_t *a,neo_genv_t *e) {
    ga_air(a,e);                                                     /* 0143E4 */
    if (B(0x15)&0x40u) {SL(0x30,0);SL(0x34,0);}
    else SL(0x34,L(0x34)+0xffff6000u);
    SB(0x12,B(0x12)>0xf7u?0xff:B(0x12)+8u);
    SB(0x13,B(0x13)>0xf7u?0xff:B(0x13)+8u);
}
/* 00288A: carry (true) when the boxes overlap (N flags of 16-bit words). */
static bool k288a(int16_t d0,int16_t d1,int16_t d2,int16_t d3,const uint16_t r[4],const uint16_t b[4]) {
    d0=(int16_t)(d0+r[0]);d1=(int16_t)(d1+r[1]);d2=(int16_t)(d2+b[0]);d3=(int16_t)(d3+b[1]);
    d2=(int16_t)(d2-d0);
    if (d2>=0) {if ((int16_t)(d2-r[2])>=0) return false;}
    else {d2=(int16_t)(d2+b[2]);if (d2<0) return false;}
    d3=(int16_t)(d3-d1);
    if (d3<0) {d3=(int16_t)(d3+b[3]);return d3>=0;}
    return (int16_t)(d3-r[3])<0;
}
/* 08C0CC: the player taking the item (0/1), -1 none. */
static int k8c0cc(neo_gactor_t *a,neo_genv_t *e,uint32_t rect) {
    if (B(0x12)!=0xffu) return -1;
    if (B(0x7a)) return 2;
    uint16_t r[4];for (unsigned i=0;i<4;i++) r[i]=rw(rect+2u*i);
    int16_t x=(int16_t)W(0x3c),y=(int16_t)W(0x40);
    bool p1=k288a(x,y,(int16_t)e->players.p1x,(int16_t)e->players.p1y,r,&e->pickup_box[0]);
    bool p2=k288a(x,y,(int16_t)e->players.p2x,(int16_t)e->players.p2y,r,&e->pickup_box[4]);
    if (!p1 && !p2) return -1;
    if (p1 && p2) return (ga_rand(e)&0x8000u)?1:0;
    return p2?1:0;
}
/* 08C5EA: kinds of 0E553E end with their program. */
static bool ends_with_program(uint16_t kind) {
    for (uint32_t p=0xe553e;;p+=2) {uint16_t k=rw(p);if (k&0x8000u) return false;if (k==kind) return true;}
}
static ga_result_t item_tick(neo_gactor_t *a,neo_genv_t *e) {
    uint32_t rec;
    switch (L(0)) {
    case 0x8c374: case 0x8c37c: {
        SB(0x8a,L(0)==0x8c374?0xff:0);
        if (W(0x6e)==0xffffu) return GA_DELETE;                      /* 004188 */
        if (W(0x6e)>=0x5au) {ga_fail(e,0x8d2e2);return GA_UNSUPPORTED;} /* TRAP #F */
        rec=RECORDS+0x18u*W(0x6e);
        if (rl(rec+0x14)!=MOTION_FALL) {if (e->external) e->external(e->ctx,L(0));return GA_DELETE;}
        if (B(0x7f)) ga_floor(a,e);
        if (BIT(0x73,0)) SET(0x15,0);
        if (BIT(0x73,1)) SET(0x15,1);
        if (BIT(0x73,4)) SB(0x8a,0);
        SW(0x66,0x8838);SW(0x1e,0x8838);
        SB(0x80,0);SB(0x7d,0);SW(0x76,0);SL(0x62,0);SB(0x7a,0);SB(0x7e,0);SW(0x78,5);
        SL(0x82,rec);
        ga_palette(a,rw(rec+8));ga_palette(a,rw(rec+10));ga_palette(a,0x380);
        SW(0x86,0x78);
        if (!W(0x70)) SW(0x70,0x258);
        if (!B(0x8a) && rb(rec+2)) {START(0x2861bc);SB(0x14,B(0x1b));SB(0x80,0xff);}
        else {START(rl(rec+4));SB(0x8a,0xff);SB(0x14,B(0x19));}
        ga_project(a,e);                                             /* 01ABC8 */
        SL(0,0x8c53c);
    }   /* fall through */
    case 0x8c53c:
        if (W(0x70)!=0xffffu) {
            SW(0x70,W(0x70)-1u);
            if (!W(0x70)) {SET(0x5f,3);SW(0x70,0x1e);SL(0,0x8c564);}
        }
        break;
    case 0x8c564:
        SW(0x70,W(0x70)-1u);
        if (!W(0x70)) return GA_DELETE;                              /* 08C684 */
        break;
    default:
        ga_fail(e,L(0));return GA_UNSUPPORTED;
    }
    /* 08C56C */
    rec=L(0x82);
    if (!W(0x86)) SB(0x7c,0xff);
    SW(0x86,W(0x86)-1u);
    k8b9b2(a,e);
    if (!B(0x80)) {
        if (W(0x78)) SW(0x78,W(0x78)-1u);
        else {
            int p=k8c0cc(a,e,0xe561c);
            if (p>=0) {
                /* The pickup routine (+10) serves the original player
                 * (weapon C0928, points C0BF4/057948, sound 1121). */
                ga_fail(e,rl(rec+0x10));return GA_UNSUPPORTED;
            }
        }
    }
    int r=ga_anim_step(a,e,NULL);
    if (r<0) return GA_UNSUPPORTED;
    if (r) {
        if (!B(0x8a)) {START(rl(rec+4));SB(0x8a,0xff);SB(0x14,B(0x19));}
        else if (ends_with_program(W(0x6e))) return GA_DELETE;
    }
    /* 08C602 */
    if (B(0x8b) && ga_creator_alive(a,e)) return GA_MORE;
    return ga_outside_view(a,e,0xe5624)?GA_DELETE:GA_MORE;
}
bool neo_item_routine(uint32_t r) {return r==0x8c374 || r==0x8c37c || r==0x8c53c || r==0x8c564;}
/* The pickup routines of the records (explicit stops of the native items). */
bool neo_item_pickup(uint32_t routine) {
    return routine==0x8b59e || routine==0x8b58c || routine==0x8b5b0 || routine==0x8b6d0 || routine==0x8b7fc;
}
ga_result_t neo_item_tick(neo_gactor_t *a,neo_genv_t *e) {return item_tick(a,e);}
