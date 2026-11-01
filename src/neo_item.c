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
 * when both touch it). The pickup routine of the record (08B568..08B9B0)
 * serves the player's weapon block and score (neo_genv_t.pickup, see
 * pickup() below); then 08C628: the sparkle 08C68E (program 28617E until
 * its end), +8C = 2 updates of 08C66A and the item goes. It leaves when outside 0E5624 (0C8BEC) or, with +8B, when its
 * creator is gone (00409A).
 *
 * Native kinds: the motions 08B9B2 (fall with 0xA000 gravity, grow +12 / +13
 * by 8 up to FF), 08BA10, 08BA38 and 08BAB2 (programs of every such kind are
 * imported; 36 is dropped by the hidden boxes 09A67E, 1A by the bomber, 29,
 * 35 and 4B.. by the prisoners). The constructor hands any other kind
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
/* Program callbacks: 2881CA (kind 36: resting on the ground, +15 bit 6 and
 * +30 = 0, sets +7D when +34 = 0 too), 287D50 (kinds 34, 39: +7D = on the
 * ground), 28AA7E (kind 54: +7E = 0029F2 of 3). The other callbacks of the
 * fall-motion programs (kinds 31, 39, 44) stop explicitly. Kind 37's
 * program (288396) is translated; its 2884C8 turns it into kind 28, whose
 * motion 08BD5A walks on the ground (0x01456A) and falls in the air. */
/* 08CD58 -> 08CDB2: item `kind` (+72 = d5) after the script actor at the
 * actor's place (01ACF2). */
static neo_gactor_t *k8cd58(neo_gactor_t *a,neo_genv_t *e,uint16_t kind,uint16_t d5) {
    neo_gactor_t *c=e->spawn?e->spawn(e->ctx,NULL,0x8c374,true):NULL;
    if (!c) return NULL;
    ga_sb(c,0x8b,0);ga_sw(c,0x70,0);ga_sw(c,0x6e,kind);ga_sw(c,0x72,0);ga_sb(c,0x7f,0);
    ga_place_at(c,a,e,0,0);ga_sw(c,0x72,d5);
    return c;
}
/* 2881E6 (kind 35 landing): three items 36 thrown with random speeds. */
static void k2881e6(neo_gactor_t *a,neo_genv_t *e) {
    neo_gactor_t *c=k8cd58(a,e,0x36,0);
    uint16_t r=(uint16_t)((ga_rand(e)&1u)+1u);if (c) ga_sw(c,0x30,r);
    r=(uint16_t)((ga_rand(e)&1u)+3u);if (c) ga_sw(c,0x34,r);
    c=k8cd58(a,e,0x36,0);
    r=(uint16_t)(ga_rand(e)&1u);if (c) {ga_sw(c,0x30,r);ga_sw(c,0x32,0x8000);}
    r=(uint16_t)((ga_rand(e)&1u)+4u);if (c) ga_sw(c,0x34,r);
    c=k8cd58(a,e,0x36,1);
    r=(uint16_t)(0u-((ga_rand(e)&1u)+1u));if (c) ga_sw(c,0x30,r);
    r=(uint16_t)((ga_rand(e)&1u)+3u);if (c) ga_sw(c,0x34,r);
}
static bool item_call(neo_gactor_t *a,neo_genv_t *e,neo_anim_kind_t kind,uint32_t routine,uint16_t parameter) {
    if (kind!=NEO_ANIM_CALL) return false;
    switch (routine) {
    case 0x2881ca: if (BIT(0x15,6) && !L(0x30)) SB(0x7d,L(0x34)?0:0xff);return true;
    case 0x287d50: SB(0x7d,BIT(0x15,6)?0xff:0);return true;
    case 0x2881e6: k2881e6(a,e);return true;
    /* The foods (kinds 35.., motion 08BAB2): rolling speed and the
     * program's flags +7D/+7E. */
    case 0x28a61a: case 0x289a5e: {
        uint32_t d0=L(0x30);if (d0&0x80000000u) d0=0u-d0;
        d0>>=8;
        uint16_t w=(int16_t)(uint16_t)d0>0x200?0x200u:(uint16_t)d0;
        SW(0x26,w);
        if (routine==0x289a5eu) {SB(0x7d,w?0:0xff);return true;}
        if (!B(0x7e)) {
            uint32_t v=L(0x30);if (v&0x80000000u) v=0u-v;
            if ((int32_t)v<=0x9000 || (int16_t)W(0x26)<=0x90) {SL(0x30,0);SW(0x26,0);}
        }
        return true;
    }
    case 0x2887c2: SB(0x7d,(ga_rand(e)&1u)?0:0xff);return true;
    case 0x288786: SB(0x7e,(uint8_t)(B(0x7e)+1u));if (B(0x7e)>=0x1eu) SB(0x7e,0);return true;
    case 0x28906e: SB(0x7e,(uint8_t)(B(0x7e)+1u));if (B(0x7e)>=0x11u) SB(0x7e,0);return true;
    case 0x28879a: SB(0x7d,0);if (BIT(0x15,6) && !B(0x7e) && !L(0x30)) SB(0x7d,L(0x34)?0:0xff);return true;
    case 0x28904e: SB(0x7d,0);if (BIT(0x15,6) && !L(0x30)) SB(0x7d,L(0x34)?0:0xff);return true;
    case 0x288dec: case 0x288bee:
        SB(0x7d,0);
        if (BIT(0x15,6) && !L(0x30) && !L(0x34) && B(0x7e)==(routine==0x288decu?3u:0xcu)) SB(0x7d,0xff);
        return true;
    case 0x288502: SB(0x7d,BIT(0x15,6)?0xff:0);if (BIT(0x15,6)) {SL(0x30,0);SL(0x34,0);}return true;
    case 0x287d5c: SET(0x5f,3);return true;
    /* Kinds 3F and 58 (parameter: the frame count). */
    case 0x28ab28: SB(0x7e,(uint8_t)ga_rand_range(e,parameter));return true;
    case 0x28ab36: {
        uint8_t last=(uint8_t)(parameter-1u);
        uint8_t d1=(uint8_t)(((W(0x30)&0x8000u)?0xffu:0u)^(BIT(0x15,0)?0xffu:0u));
        d1=(uint8_t)((d1&0xfeu)+1u);
        SB(0x7e,(uint8_t)(B(0x7e)+d1));
        if (B(0x7e)&0x80u) SB(0x7e,last);
        if ((int8_t)last<(int8_t)B(0x7e)) SB(0x7e,0);
        /* 002F1A: |vx|,|vy| (pixels) -> larger + smaller/2. */
        uint16_t x=(uint16_t)(uint32_t)((int32_t)L(0x30)>>8),y=(uint16_t)(uint32_t)((int32_t)L(0x34)>>8);
        if (x&0x8000u) x=(uint16_t)(0u-x);
        if (y&0x8000u) y=(uint16_t)(0u-y);
        uint16_t d0=y>x?(uint16_t)((x>>1)+y):(uint16_t)((y>>1)+x);
        d0>>=2;if (d0) d0=(uint16_t)(d0+0x80u);
        SW(0x26,d0);return true;
    }
    case 0x28774c: if (!L(0x34)) SW(0x26,0);return true;           /* kind 24 */
    case 0x28aa7e: SB(0x7e,ga_rand_range(e,3));return true;
    case 0x286fa8: SW(0x26,(uint16_t)((ga_rand(e)&0x3fu)+0xc0u));return true;  /* kind 55: program speed */
    /* Kind 37 (program 288396). */
    case 0x2884aa: SW(0x26,(uint16_t)((ga_rand(e)&0xffu)+0x100u));return true;
    case 0x28849a: SB(0x7e,(uint8_t)(ga_rand(e)&0xfu));return true;
    case 0x2884be: SL(0x30,0);SL(0x34,0);return true;
    case 0x288baa: SB(0x7e,(uint8_t)(B(0x7e)+1u));if (B(0x7e)>=0x10u) SB(0x7e,0);return true;
    case 0x286b44: SB(0x7d,(ga_rand(e)&3u)?0:0xff);return true;
    case 0x286ae8: /* face +73 bit 2 (right)/3 (left), else the screen half */
        if (BIT(0x73,2)) SET(0x15,0);
        else if (BIT(0x73,3)) SB(0x15,B(0x15)&~1u);
        else {ga_project(a,e);SB(0x15,B(0x15)&~1u);if ((int16_t)W(0x3c)<0xa0) SET(0x15,0);}
        /* fall through */
    case 0x286b2c: SL(0x62,BIT(0x15,0)?0xc000u:0u-0xc000u);return true;
    case 0x2884c8: /* becomes kind 28 (08D312: record +82; its motion 08BD5A) */
        SW(0x6e,0x28);SL(0x82,RECORDS+0x18u*0x28u);
        SW(0x76,0);SW(0x74,0);SL(0x62,0);SW(0x26,0x100);return true;
    default: return false;
    }
}
/* 08B9B2: the fall, and the item grows to full size. */
static void k8b9b2(neo_gactor_t *a,neo_genv_t *e) {
    ga_air(a,e);                                                     /* 0143E4 */
    if (B(0x15)&0x40u) {SL(0x30,0);SL(0x34,0);}
    else SL(0x34,L(0x34)+0xffff6000u);
    SB(0x12,B(0x12)>0xf7u?0xff:B(0x12)+8u);
    SB(0x13,B(0x13)>0xf7u?0xff:B(0x13)+8u);
}
/* 08BA10: the fall with 0x5000 gravity (kind 1A). 08BA38 / 08BAB2: on the
 * ground a fall faster than 0x4000 bounces at half speed (else it stops),
 * in the air 0xA000 / 0x5000 gravity; +30 slows by 0x800 towards 0 (kinds
 * 24; 35, 38, 3A..3C, 3E, 43, 45..47, 58). All grow as 08B9B2 (08B9D8). */
static void grow(neo_gactor_t *a) {
    SB(0x12,B(0x12)>0xf7u?0xff:B(0x12)+8u);
    SB(0x13,B(0x13)>0xf7u?0xff:B(0x13)+8u);
}
static void k8ba10(neo_gactor_t *a,neo_genv_t *e) {
    ga_air(a,e);
    if (B(0x15)&0x40u) {SL(0x30,0);SL(0x34,0);}
    else SL(0x34,L(0x34)+0xffffb000u);
    grow(a);
}
static void k8ba38(neo_gactor_t *a,neo_genv_t *e,uint32_t gravity) {
    ga_air(a,e);
    if (B(0x15)&0x40u) {
        uint32_t d7=L(0x34);
        if (d7&0x80000000u) {
            d7=(uint32_t)((int32_t)(0u-d7)>>1);SL(0x34,d7);
            if ((int32_t)d7<=0x4000) {SL(0x30,0);SL(0x34,0);}
        }
    } else SL(0x34,L(0x34)+gravity);
    uint32_t d7=L(0x30);
    if (d7) {
        if (d7&0x80000000u) {d7+=0x800u;if (!(d7&0x80000000u)) d7=0;}
        else {d7-=0x800u;if (d7&0x80000000u) d7=0;}
        SL(0x30,d7);
    }
    grow(a);
}
static void k8bd5a(neo_gactor_t *a,neo_genv_t *e);
/* 08B9F2: the item grows by 0x10 (kind 55's states). */
static void grow16(neo_gactor_t *a) {
    SB(0x12,B(0x12)>0xefu?0xff:B(0x12)+0x10u);
    SB(0x13,B(0x13)>0xefu?0xff:B(0x13)+0x10u);
}
/* 08BCCC (kind 55, the prisoners' gift and the ruin window's box): state
 * +76 of the table 08BCC0 (08BD66). 0 08BCD4: 01ABC8, four bounces (+74),
 * state 1. 1 08BCEA: 0xB000 gravity and 0143E4; on the ground the fall
 * speed (before the step) back up at half, +30 times 3/4, one bounce less
 * (state 2 after the last). 2 08B9B2: the fall. */
static bool k8bccc(neo_gactor_t *a,neo_genv_t *e) {
    switch (W(0x76)) {
    case 0: ga_project(a,e);SW(0x76,1);SW(0x74,4);grow16(a);return true;
    case 1: {
        SL(0x34,L(0x34)+0xffffb000u);
        uint32_t d0=L(0x34);
        ga_air(a,e);                                                 /* 0143E4 */
        if (BIT(0x15,6)) {
            SL(0x34,(uint32_t)((int32_t)(0u-d0)>>1));
            uint32_t v=(uint32_t)((int32_t)L(0x30)>>1);
            SL(0x30,v+(uint32_t)((int32_t)v>>1));
            SW(0x74,W(0x74)-1u);
            if (!W(0x74)) SW(0x76,2);
        }
        grow16(a);return true;
    }
    case 2: k8b9b2(a,e);return true;
    default: ga_fail(e,0x8bd66);return false;
    }
}
/* The motions the port has (record +14). */
static bool motion_known(uint32_t m) {return m==MOTION_FALL || m==0x8ba10u || m==0x8ba38u || m==0x8bab2u || m==0x8bcccu;}
static bool motion(neo_gactor_t *a,neo_genv_t *e,uint32_t m) {
    switch (m) {
    case MOTION_FALL: k8b9b2(a,e);return true;
    case 0x8ba10: k8ba10(a,e);return true;
    case 0x8ba38: k8ba38(a,e,0xffff6000u);return true;
    case 0x8bab2: k8ba38(a,e,0xffffb000u);return true;
    case 0x8bd5a: k8bd5a(a,e);return true;
    case 0x8bccc: return k8bccc(a,e);
    default: ga_fail(e,m);return false;
    }
}
/* 08BD5A: ground step (+62) on the ground, else the 0xB000 gravity and the
 * air step (0143DC); then the item grows as in 08B9B2. */
static void k8bd5a(neo_gactor_t *a,neo_genv_t *e) {
    if (BIT(0x15,6)) {uint32_t kind;ga_ground(a,e,&kind);}
    else {SL(0x34,L(0x34)+0xffffb000u);ga_air(a,e);}
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
/* 057948 (D0 = player + 1): BCD sum of the 4 bytes of 10C5A0+4p into the
 * player's score; on overflow 999999 in its first three bytes. */
void neo_item_add_score(uint8_t score[4],uint32_t value) {
    unsigned carry=0;
    for (int i=3;i>=0;i--) {
        unsigned x=score[i],y=(value>>(8*(3-i)))&0xffu;
        unsigned lo=(x&15u)+(y&15u)+carry,hi=(x>>4)+(y>>4);
        if (lo>9u) {lo-=10u;hi++;}
        carry=hi>9u;if (carry) hi-=10u;
        score[i]=(uint8_t)(hi<<4|lo);
    }
    if (carry) {score[0]=0x99;score[1]=0x99;score[2]=0x99;}
}
/* 08B850: the value into the score (not for player index 2). */
static void k8b850(neo_pickup_env_t *pk,unsigned p) {if (p<2) neo_item_add_score(pk->score[p],pk->points[p]);}
/* 08B87A: refused, no value. */
static void k8b87a(neo_pickup_env_t *pk,unsigned p) {pk->points[p]=0;}
/* 08B896: the value of the pickup (record +0C, the special table E5526 for
 * +7B, or the combo tables E5556/E559A when record byte 0 is set and the
 * previous combo pickup is at most 15 frames old) and its score label
 * 08D330 (list 101414; an external actor here, counted). */
static bool k8b896(neo_gactor_t *a,neo_genv_t *e,neo_pickup_env_t *pk,unsigned p,uint32_t rec) {
    bool combo=rb(rec) && pk->frame-pk->stamp[p]<=0xfu;
    if (combo) {
        if (pk->combo_lock[p]) pk->points[p]=rl(0xe559a);
        else {
            pk->combo[p]=(uint16_t)(pk->combo[p]+4u);if (pk->combo[p]>0x28u) pk->combo[p]=0x28;
            pk->points[p]=rl(0xe5556u+pk->combo[p]);
        }
    } else {
        pk->points[p]=B(0x7b)?rl(0xe5526u+4u*(uint16_t)(W(0x6e)-0xfu)):rl(rec+0xc);
        pk->combo[p]=0;
    }
    /* 08B944: the label 08D330 after the script actor (00411C), +6E = the
     * value, at the item (01ACF2), +8A = the player. */
    neo_gactor_t *c=e->spawn?e->spawn(e->ctx,NULL,0x8d330,true):NULL;
    if (c) ga_sl(c,0x6e,pk->points[p]);
    if ((int32_t)pk->points[p]>0x50000) {ga_fail(e,0x8b97a);return false;}   /* TRAP #F */
    if (c) {ga_place_at(c,a,e,0,0);ga_sb(c,0x8a,(uint8_t)p);}
    pk->popups++;
    return true;
}
/* 08B834: a combo record stamps the frame. */
static void k8b834(neo_pickup_env_t *pk,unsigned p,uint32_t rec) {if (rb(rec)) pk->stamp[p]=pk->frame;}
static void sound(neo_genv_t *e,uint16_t id) {if (id && e->sound) e->sound(e->ctx,id);}
/* 08B568..08B5B6: moveq #id,d1 then 08B5BC. */
static int weapon_entry(uint32_t r) {
    static const uint8_t ids[14]={5,2,4,3,6,0xd,0xc,0xf,0xe,0xa,7,9,8,0xb};
    if (r<0x8b568u || r>0x8b5b6u || (r-0x8b568u)%6u) return -1;
    return ids[(r-0x8b568u)/6u];
}
/* The pickup routine (record +10) for player p. False: an explicit stop
 * (ga_fail names the untranslated code). */
static bool pickup(neo_gactor_t *a,neo_genv_t *e,unsigned p,uint32_t rec) {
    uint32_t r=rl(rec+0x10);neo_pickup_env_t *pk=e->pickup;
    if (!pk || p>=2 || !pk->weapons || !pk->player[p]) {ga_fail(e,r);return false;}
    int id=weapon_entry(r);
    if (id>=0) {                                                     /* 08B5BC */
        /* Accepted (carry set) only clears the value; a refused weapon
         * gives points instead (08B5CA BCS). */
        uint16_t snd;
        if (marco_weapons_grant(pk->weapons,p,(uint8_t)id,pk->player[p],&snd)) {sound(e,snd);k8b87a(pk,p);}
        else {
            if (!k8b896(a,e,pk,p,rec)) return false;
            k8b850(pk,p);sound(e,0x1121);
        }
    } else if (r==0x8b6d0) {                                         /* bombs */
        uint16_t snd;
        marco_weapon_result_t v=marco_weapons_bombs(pk->weapons,p,10,pk->player[p],pk->slot[p],&snd);
        if (v==MW_STOP) {ga_fail(e,0xc0764);return false;}
        if (v==MW_ACCEPTED) {
            /* 08B702: no value; 08CD10 creates the label 08C752 (list
             * 101414, text E5782[bomb type + 3]): external, counted. */
            sound(e,snd);k8b87a(pk,p);
            pk->labels++;if (e->external) e->external(e->ctx,0x8c752);
        } else {
            if (!k8b896(a,e,pk,p,rec)) return false;
            k8b850(pk,p);sound(e,0x1121);
        }
    } else if (r==0x8b750) {                                         /* medicine (kind 0x32) */
        /* 08B750: sound 1121, then 0C0C92: a live player in form 3 gets
         * +94 = 0 (the cure; carry) -> 08B87A and the label 08CD10 (d1 = 1,
         * external, counted); otherwise the value 08B896 and 08B850. */
        sound(e,0x1121);
        marco_weapon_player_t *m=pk->player[p];
        if (m->exists && !m->dead && m->form93==3) {
            m->form94=0;k8b87a(pk,p);pk->cures++;
            pk->labels++;if (e->external) e->external(e->ctx,0x8cd10);
        } else {
            if (!k8b896(a,e,pk,p,rec)) return false;
            k8b850(pk,p);
        }
    } else if (r==0x8b7fc) {                                         /* points / food */
        sound(e,0x1121);
        if (rb(rec+3) && !B(0x7a)) {
            /* 0C0BF4: food counts only in missions 4-7, 9, 10 (0C0BAE). */
            uint16_t m=pk->mission;
            if (m==4 || m==5 || m==6 || m==7 || m==9 || m==10) {ga_fail(e,0xc0c14);return false;}
        }
        if (!k8b896(a,e,pk,p,rec)) return false;
        k8b850(pk,p);
    } else {ga_fail(e,r);return false;}                              /* bomb types, others */
    k8b834(pk,p,rec);pk->taken++;
    return true;
}
/* ---- score label 08D330 --------------------------------------------------- */
/* The value +6E as up to eight digits (leading zeros skipped, +8C.., count
 * +72), 8 px apart from +28, each with its own height +74+4i. Digit i waits
 * +91+i updates, rises +96+i updates at 0x50000, then follows the speeds
 * 0E55EC by +9B+i; after the last digit's end, 15 updates still (08D63E),
 * then the blink of 0E55CA rising by 0x3000 (08D6A2/08D738) until its end.
 * The palette slot blinks every update (+A0). Each digit is its program of
 * 0E56EE (0E565A with RAM 10C2F6 nonzero, 08CF2E) started and stepped once
 * (08D7B4). It goes when four newer labels of its player exist (08D814). */
static bool k8d814(neo_gactor_t *a,neo_genv_t *e) {
    neo_pickup_env_t *pk=e->pickup;unsigned p=B(0x8a)&1u;
    return pk && (uint16_t)(W(0x88)+4u)<pk->label_count[p];
}
static void k8d7f4(neo_gactor_t *a,neo_genv_t *e,unsigned i) {
    uint32_t y=L(0x74+4u*i)+L(0x34);SL(0x74+4u*i,y);SL(0x2c,y);ga_project(a,e);
}
static bool k8d7b4(neo_gactor_t *a,neo_genv_t *e,unsigned i) {
    uint32_t table=(e->pickup && e->pickup->mission)?0xe565a:0xe56ee;
    if (!ga_anim_start(a,e,rl(table+4u*B(0x8c+i)),0x100)) return false;
    return ga_anim_step(a,e,NULL)>=0;
}
static void blink(neo_gactor_t *a) {SB(0x14,(B(0xa0)&1u)?B(0x19):B(0x1a));SB(0xa0,B(0xa0)+1u);}
static ga_result_t label_tick(neo_gactor_t *a,neo_genv_t *e) {
    switch (L(0)) {
    case 0x8d330: {
        unsigned p=B(0x8a);neo_pickup_env_t *pk=e->pickup;
        if (!pk || p>1u) {ga_fail(e,0x8d33c);return GA_UNSUPPORTED;}
        pk->label_count[p]++;SW(0x88,pk->label_count[p]);
        SW(0x66,0xffff);SW(0x1e,0xffff);
        uint32_t pal=rl(0xe559eu+4u*p);ga_palette(a,rw(pal));ga_palette(a,rw(pal+2u));
        SB(0x14,B(0x19));
        SB(0x8c,0);SB(0x8d,0xff);SB(0x8e,0xff);SB(0x8f,0xff);SB(0x90,0xff);
        uint32_t d4=L(0x6e);unsigned n=0;uint8_t any=0;
        for (unsigned k=0;k<8;k++) {d4=d4<<4|d4>>28;uint8_t d=(uint8_t)(d4&15u);any|=d;if (any) SB(0x8c+n++,d);}
        SW(0x72,n?n:1u);
        for (unsigned k=0;k<5;k++) SB(0x91+k,k);
        uint8_t d0=(uint8_t)(((W(0x88)+2u)&7u)+6u);
        for (unsigned k=0;k<5;k++) {SB(0x96+k,d0);SB(0x9b+k,0);}
        for (unsigned k=0;k<5;k++) SL(0x74+4u*k,0);
        SB(0xa0,0);SB(0xa1,0);SL(0x30,0);SL(0x34,0);
        ga_project(a,e);
        for (unsigned k=0;k<5;k++) SL(0x74+4u*k,L(0x2c));
        ga_project(a,e);
        if (p==0) SW(0x28,(uint16_t)(W(0x28)+(uint16_t)(e->players.p1x-W(0x3c))));
        else SW(0x28,(uint16_t)(W(0x28)+(uint16_t)(e->players.p2x-W(0x3c))));
        ga_project(a,e);
        if ((int16_t)W(0x3c)>0x100) SW(0x28,W(0x28)-0x20u);
        SL(0,0x8d4de);
    }   /* fall through */
    case 0x8d4de: {
        if (k8d814(a,e)) return GA_DELETE;                           /* 08D56E (cartridge check: 004188) */
        ga_project(a,e);blink(a);
        uint32_t x=L(0x28);
        for (unsigned i=0;i<W(0x72);i++) {
            if (B(0x91+i)) SB(0x91+i,B(0x91+i)-1u);
            else {
                if (B(0x96+i)) {SL(0x34,0x50000);SB(0x96+i,B(0x96+i)-1u);}
                else {
                    uint32_t v=rl(0xe55ecu+4u*B(0x9b+i));SL(0x34,v);
                    if (v==0x80000000u) {if (i+1u==W(0x72)) SL(0,0x8d62e);}
                    else SB(0x9b+i,B(0x9b+i)+1u);
                }
                k8d7f4(a,e,i);
                if (!k8d7b4(a,e,i)) return GA_UNSUPPORTED;
            }
            SW(0x28,W(0x28)+8u);
        }
        SL(0x28,x);
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    }
    case 0x8d62e:
        SL(0x34,0);SB(0x8b,0xf);SL(0,0x8d63e);
        /* fall through */
    case 0x8d63e: {
        if (k8d814(a,e)) return GA_DELETE;
        ga_project(a,e);
        SB(0x8b,B(0x8b)-1u);
        if (!B(0x8b)) {SL(0x34,0);SL(0,0x8d6a2);goto rise;}
        blink(a);
        uint32_t x=L(0x28);
        for (unsigned i=0;i<W(0x72);i++) {k8d7f4(a,e,i);if (!k8d7b4(a,e,i)) return GA_UNSUPPORTED;SW(0x28,W(0x28)+8u);}
        SL(0x28,x);
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    }
    case 0x8d698:
        SL(0x34,0);SL(0,0x8d6a2);
        /* fall through */
    case 0x8d6a2:
    rise: {
        if (k8d814(a,e)) return GA_DELETE;
        ga_project(a,e);
        SL(0x34,L(0x34)+0x3000u);
        uint8_t k=B(0xa1);SB(0xa1,k+1u);
        uint8_t d6=rb(0xe55cau+k);
        if (d6==0xffu) return GA_DELETE;                             /* 004188 */
        SB(0x14,d6?B(0x19):B(0x1a));
        uint32_t x=L(0x28);
        for (unsigned i=0;i<W(0x72);i++) {
            k8d7f4(a,e,i);
            if (d6!=2u && !k8d7b4(a,e,i)) return GA_UNSUPPORTED;
            SW(0x28,W(0x28)+8u);
        }
        SL(0x28,x);
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    }
    }
    ga_fail(e,L(0));return GA_UNSUPPORTED;
}
static bool label_routine(uint32_t r) {return r==0x8d330 || r==0x8d4de || r==0x8d62e || r==0x8d63e || r==0x8d698 || r==0x8d6a2;}
static ga_result_t item_tick(neo_gactor_t *a,neo_genv_t *e) {
    uint32_t rec;
    switch (L(0)) {
    case 0x8c68e:                                                    /* sparkle */
        START(0x28617e);ga_palette(a,0x380);SL(0,0x8c6b2);
        /* fall through */
    case 0x8c6b2: {
        ga_project(a,e);                                             /* 01ABC8 */
        int r=ga_anim_step(a,e,NULL);
        if (r<0) return GA_UNSUPPORTED;
        /* At the end, 08C6C2 counts RAM 10F00C (+3EF); on its carry the
         * cartridge check of 2FFFE0, which ends in 004188 on genuine
         * hardware like the other path (the counters are not kept). */
        return r?GA_DELETE:GA_MORE;
    }
    case 0x8c374: case 0x8c37c: {
        SB(0x8a,L(0)==0x8c374?0xff:0);
        if (W(0x6e)==0xffffu) return GA_DELETE;                      /* 004188 */
        if (W(0x6e)>=0x5au) {ga_fail(e,0x8d2e2);return GA_UNSUPPORTED;} /* TRAP #F */
        rec=RECORDS+0x18u*W(0x6e);
        if (!motion_known(rl(rec+0x14))) {if (e->external) e->external(e->ctx,L(0));return GA_DELETE;}
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
    case 0x8c66a:
        rec=L(0x82);goto taken;
    default:
        ga_fail(e,L(0));return GA_UNSUPPORTED;
    }
    /* 08C56C */
    rec=L(0x82);
    if (!W(0x86)) SB(0x7c,0xff);
    SW(0x86,W(0x86)-1u);
    if (!motion(a,e,rl(rec+0x14))) return GA_UNSUPPORTED;
    if (!B(0x80)) {
        if (W(0x78)) SW(0x78,W(0x78)-1u);
        else {
            int p=k8c0cc(a,e,0xe561c);
            if (p>=0) {
                if (!pickup(a,e,(unsigned)p,rec)) return GA_UNSUPPORTED;
                /* 08C628: 08CBF8 (item list 10C5C2, not kept), the sparkle
                 * 08C68E when record byte 2 is set, two updates of 08C66A. */
                if (rb(rec+2)) {
                    SB(0x14,B(0x1a));
                    neo_gactor_t *c=e->spawn(e->ctx,a,0x8c68e,true);   /* 00411C, list 101414 */
                    if (c) ga_place_at(c,a,e,0,0);                       /* 01ACF2 */
                }
                SB(0x8c,2);SL(0,0x8c66a);
                goto taken;
            }
        }
    }
    int r=ga_anim_step(a,e,item_call);
    if (r<0) return GA_UNSUPPORTED;
    if (r) {
        if (!B(0x8a)) {START(rl(rec+4));SB(0x8a,0xff);SB(0x14,B(0x19));}
        else if (ends_with_program(W(0x6e))) return GA_DELETE;
    }
    goto view;
taken:                                                               /* 08C66A */
    {/* 08C66A: the motion, then 00427A with the program's callbacks. */
     if (!motion(a,e,rl(rec+0x14))) return GA_UNSUPPORTED;
     if (ga_anim_step(a,e,item_call)<0) return GA_UNSUPPORTED;
     SB(0x8c,B(0x8c)-1u);
     return B(0x8c)?GA_MORE:GA_DELETE;}
view:
    /* 08C602 */
    if (B(0x8b) && ga_creator_alive(a,e)) return GA_MORE;
    return ga_outside_view(a,e,0xe5624)?GA_DELETE:GA_MORE;
}
bool neo_item_routine(uint32_t r) {return label_routine(r) || r==0x8c374 || r==0x8c37c || r==0x8c53c || r==0x8c564 || r==0x8c66a || r==0x8c68e || r==0x8c6b2;}
/* The code a pickup stops at when it cannot be served (no pickup state,
 * untranslated routine or refusal): explicit stops of the native items. */
bool neo_item_pickup(uint32_t routine) {
    return (routine>=0x8b568 && routine<=0x8b9b0) || routine==0xc0764 || routine==0xc0c14;
}
ga_result_t neo_item_tick(neo_gactor_t *a,neo_genv_t *e) {return label_routine(ga_l(a,0))?label_tick(a,e):item_tick(a,e);}
