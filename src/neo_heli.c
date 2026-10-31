#include "neo_region.h"
#include "neo_soldier_ai.h"   /* neo_angle (ROM 002EB4) */
/* Helicopter formation 04857C (Mission 1 camera 3192, script records 2D7092
 * (player left of the stop) / 2D70A6 (right): +6E altitude index, +70 path
 * set, +72 count), manual translation of ROM 047874..048FFE with its
 * services 04900C..04A1AC and the program callbacks 04A1AE/04A208/04A262.
 * ROM labels are provenance only.
 *
 * Nodes: the formation controller 04857C (invisible: live count +96, slots
 * +76/+90 the bodies follow, chain-explosion bits +9A, bombing rhythm
 * +8A/+8C/+8E/+95/+99) creates +72 leaders 04870C (+61 = index; invisible:
 * they fly to the altitude 4982A (04876C/048780) and then run the path
 * program E0AF2/E0AFE by +70 with 021288 (0487D0/048832), writing their
 * position and heading into the controller's slots). Each leader creates
 * its helicopter 048882 (hurtbox E0AB2, health rank 2D1530, rotor child
 * 047874): it waits on its leader (04893E), then follows the slot (049EFA)
 * with the bank index +96 (+3 offset; 049C82 by the heading E09F4), bombs
 * on 04A064 (048A06/048A2C: droppers 047A6C by rank 2D1840 every rank
 * 2D18D6 updates) and dies by 049030: hit (+5F bit 7) -> 048AF6 (or 048AB6
 * with the points 08D1D0 when the formation is complete and the leader 0
 * is hit: then every helicopter explodes in turn by the controller's +9A
 * bits), coin 048B10: explosion 048E22 (debris 048EDA..048F4E, smoke list
 * E0A3E, 01C71E in place) or the fall 048B2E/048BBC (smoke E0A50, puffs
 * 01C71E/01C6EE, crash 048C8C -> wreck 048D22/048D5E with the attack row
 * E0AE2). The dropper 047A6C runs 244F06 whose callback 04A262 makes the
 * bomb 047B24 (speed rank 2D19B4, polar by the dropper's +80, offsets
 * E0A1E, attack row E0ABE, impact 048510 on the ground, explosion 047C48
 * with 22E504 on a hit) and the shell 047DE0 after the script actor
 * (program 244F9E, gravity, one bounce +61, 0x12 updates).
 * The cartridge check (2FFFE0) before the deletions is not reproduced (it
 * falls through to 004188 on genuine hardware).
 *
 * Not in Mission 1 and not translated (explicit stops): the lone
 * helicopter 047F7E..048510 (states 047800/04783A/048002..0484FE with the
 * droppers 0479B8/047A12 and bombs 047AFC/047B10 of the callbacks
 * 04A1AE/04A208), the spawn word +6E > 0x13 (trap #15 "takasa err"), the
 * timed explosion 048CD2 (unreachable: 048C36 tests the high byte of the
 * word +8C). 048AB6 (the points) is translated but was not recorded. */
#define B(o) ga_b(a,(o))
#define W(o) ga_w(a,(o))
#define L(o) ga_l(a,(o))
#define SB(o,v) ga_sb(a,(o),(uint8_t)(v))
#define SW(o,v) ga_sw(a,(o),(uint16_t)(v))
#define SL(o,v) ga_sl(a,(o),(uint32_t)(v))
#define BIT(o,b) ((B(o)>>(b))&1u)
#define SET(o,b) SB(o,B(o)|(1u<<(b)))
#define CLR(o,b) SB(o,B(o)&~(1u<<(b)))
#define FACING BIT(0x15,0)
#define START(t) do { if (!ga_anim_start(a,e,(t),0x100)) return GA_UNSUPPORTED; } while (0)
#define STEP(d) do { int r_=ga_anim_step(a,e,heli_call); if (r_<0) return GA_UNSUPPORTED; (d)=r_!=0; } while (0)
static int16_t S16(uint16_t v) {return (int16_t)v;}
static int8_t S8(uint8_t v) {return (int8_t)v;}
static uint8_t rb(uint32_t address) {const uint8_t *p=ga_rom(address);return p?p[0]:0;}
static uint16_t rw(uint32_t address) {const uint8_t *p=ga_rom(address);return p?(uint16_t)(p[0]<<8|p[1]):0;}
static uint32_t rl(uint32_t address) {const uint8_t *p=ga_rom(address);return p?(uint32_t)p[0]<<24|(uint32_t)p[1]<<16|(uint32_t)p[2]<<8|p[3]:0;}
static uint16_t rank(neo_gactor_t *a,neo_genv_t *e,uint32_t table) {
    const uint8_t *t=ga_rom(table);
    if (!t) {ga_fail(e,table);return 0;}
    return ga_rank_word(a,e,t);
}
static void sound(neo_genv_t *e,uint16_t id) {if (e->sound) e->sound(e->ctx,id);}
static neo_gactor_t *creator(neo_gactor_t *a,neo_genv_t *e) {return e->creator?e->creator(e->ctx,a):NULL;}
/* The formation controller of a helicopter (+0C of +0C). */
static neo_gactor_t *controller(neo_gactor_t *a,neo_genv_t *e) {neo_gactor_t *l=creator(a,e);return l?creator(l,e):NULL;}
/* 0040BC + 01ACF2 (0,0). */
static neo_gactor_t *child_at(neo_gactor_t *a,neo_genv_t *e,uint32_t routine,int16_t dx,int16_t dy) {
    neo_gactor_t *c=e->spawn?e->spawn(e->ctx,a,routine,false):NULL;
    if (c) ga_place_at(c,a,e,dx,dy);
    return c;
}
/* 00411C after the script actor + 01ACF2. */
static neo_gactor_t *after_script(neo_gactor_t *a,neo_genv_t *e,uint32_t routine,int16_t dx,int16_t dy) {
    neo_gactor_t *c=e->spawn?e->spawn(e->ctx,NULL,routine,true):NULL;
    if (c) ga_place_at(c,a,e,dx,dy);
    return c;
}
/* 01AD6E: position block relative to the creator's. */
static void on_creator(neo_gactor_t *a,neo_genv_t *e) {neo_gactor_t *p=creator(a,e);ga_place(a,e,0,0,p?NEO_GA_TAG(p->index):0);}
/* 01AD52: re-parent the position block (+28..+37) to the scene root
 * 10E968 (01AC4C), then project (01ABC8). */
static void to_root(neo_gactor_t *a,neo_genv_t *e) {
    uint32_t tag=L(0x38);
    if (tag!=0x10e968u) {
        neo_contact_parent_t op,np;bool has_old=tag && ga_parent(e,tag,&op),has_new=ga_parent(e,0x10e968u,&np);
        uint32_t d0=L(0x28),d1=L(0x2c),d2=L(0x30),d3=L(0x34);
        d2+=d0;d3+=d1;
        if (has_old) {d0+=(uint32_t)op.previous_x;d1+=(uint32_t)op.previous_y;d2+=(uint32_t)op.x;d3+=(uint32_t)op.y;}
        if (has_new) {d0-=(uint32_t)np.previous_x;d1-=(uint32_t)np.previous_y;d2-=(uint32_t)np.x;d3-=(uint32_t)np.y;}
        d2-=d0;d3-=d1;
        SL(0x28,d0);SL(0x2c,d1);SL(0x30,d2);SL(0x34,d3);SL(0x38,0x10e968u);
    }
    ga_project(a,e);
}
/* 01DA40: smoke emitter 01D764 with the list `list` (+86). */
static void smoke(neo_gactor_t *a,neo_genv_t *e,uint32_t list) {
    neo_gactor_t *c=ga_spawn_from(a,e,0x1d764,0,0);
    if (!c) return;
    if (B(0x15)&4u) ga_sb(c,0x15,(uint8_t)(ga_b(c,0x15)|4u));
    ga_sl(c,0x86,list);
}
/* 01C6EE / 01C71E puff after the actor (0040BC + 01ACF2), layer 0x1001. */
static neo_gactor_t *puff(neo_gactor_t *a,neo_genv_t *e,uint32_t routine) {
    neo_gactor_t *c=child_at(a,e,routine,0,0);
    if (c) {ga_sw(c,0x66,0x1001);ga_sw(c,0x1e,0x1001);}
    return c;
}
/* 01B29C: octagonal distance. */
static uint16_t distance(uint16_t x0,uint16_t y0,uint16_t x1,uint16_t y1) {
    uint16_t d0=(uint16_t)(x0-x1),d1=(uint16_t)(y0-y1);
    if (d0&0x8000u) d0=(uint16_t)(0u-d0);
    if (d1&0x8000u) d1=(uint16_t)(0u-d1);
    if (S16(d0)>S16(d1)) return (uint16_t)(d0+(d1>>1));
    return (uint16_t)((d0>>1)+d1);
}
/* 049E46 (dy = +7A) / 049E74 (y ignored): carry when the distance to the
 * target +78 is at most `limit`. */
static bool near_target(neo_gactor_t *a,bool with_y,int16_t limit) {
    uint16_t d=with_y?distance(W(0x3c),W(0x40),W(0x78),W(0x7a)):distance(W(0x3c),0,W(0x78),0);
    return S16(d)<=limit;
}
/* 0499A4: heading +82 towards (+78,+7A) with the twist +74, speed by the
 * distance (dist * 0x1800), eased into +30/+34. */
static void steer(neo_gactor_t *a) {
    int16_t dx=(int16_t)(S16((uint16_t)(W(0x78)-W(0x3c)))>>1),dy=(int16_t)(S16((uint16_t)(W(0x7a)-W(0x40)))>>1);
    uint16_t d0=neo_angle(dx,dy),d1=W(0x74)&0x1fu;
    if (FACING) d1=(uint16_t)(0u-d1);
    SW(0x82,(d0+d1)&0xffu);
    uint32_t s=(uint32_t)distance(W(0x3c),W(0x40),W(0x78),W(0x7a))<<16;
    s>>=5;s+=s>>1;
    uint32_t vx,vy;ga_polar(s,(uint8_t)W(0x82),&vx,&vy);
    int32_t ex=(int32_t)(vx-L(0x30)),ey=(int32_t)(vy-L(0x34));
    SL(0x30,L(0x30)+(uint32_t)(ex>>3));SL(0x34,L(0x34)+(uint32_t)(ey>>4));
}
/* 049A92: bank index +96 (0..3) one step towards the one of the speed +30. */
static void bank_by_speed(neo_gactor_t *a) {
    int32_t v=(int32_t)L(0x30);uint8_t d1;
    if (!FACING) d1=v<(int32_t)0xffff4000?3:v<(int32_t)0xffff8000?2:v<(int32_t)0xffffc000?1:0;
    else d1=v<0x4000?0:v<0x8000?1:v<0xc000?2:3;
    if (d1>B(0x96)) SB(0x96,B(0x96)+1u); else if (d1<B(0x96)) SB(0x96,B(0x96)-1u);
}
/* 049C82: +96 (minus its offset 3) one step towards the bank of the heading
 * +AA (table E09F4). The caller adds the 3 back after the program step. */
static void bank_by_heading(neo_gactor_t *a) {
    SB(0x96,B(0x96)-3u);
    uint8_t d0=(uint8_t)(rb(0xe09f4u+(B(0xaa)>>4))-3u);
    if (d0>B(0x96)) SB(0x96,B(0x96)+1u); else if (d0<B(0x96)) SB(0x96,B(0x96)-1u);
}
/* 049C52: +96 (minus 3) one step towards `d0`; carry when already there. */
static bool bank_towards(neo_gactor_t *a,uint8_t d0) {
    SB(0x96,B(0x96)-3u);
    if (d0==B(0x96)) return true;
    if (S8(d0)<S8(B(0x96))) SB(0x96,B(0x96)-1u); else SB(0x96,B(0x96)+1u);
    return false;
}
/* 049EFA: speed towards the controller's slot +95 (position +76+4i,
 * heading +90+i): straight to it within 4 px, else 4 px/update along the
 * angle 002EB4 (kept in +AA). */
static void follow_slot(neo_gactor_t *a,neo_genv_t *e) {
    neo_gactor_t *r=controller(a,e);
    if (!r) {ga_fail(e,0x49efa);return;}
    unsigned i=ga_b(r,0x95);
    uint16_t px=ga_w(r,0x76+4u*i),py=ga_w(r,0x78+4u*i);
    int32_t d=(int32_t)((uint32_t)distance(px,py,W(0x3c),W(0x40))<<16);
    if (d>0x40000) {
        uint8_t ang=neo_angle(S16((uint16_t)(px-W(0x3c))),S16((uint16_t)(py-W(0x40))));
        SB(0xaa,ang);
        uint32_t vx,vy;ga_polar(0x40000,ang,&vx,&vy);SL(0x30,vx);SL(0x34,vy);
    } else {
        SL(0x30,((uint32_t)px<<16)-L(0x3c));SL(0x34,((uint32_t)py<<16)-L(0x40));
        SB(0xaa,ga_b(r,0x90+i));
    }
}
/* 04A15A: carry when the player at `px` is within 32 px on the side the
 * bank +96 points to (then +80 = 0xC0). */
static bool player_under(neo_gactor_t *a,uint16_t px) {
    int16_t d0=S16((uint16_t)(px-W(0x3c)));
    if (d0<0) {
        if (S8(B(0x96))>4) return false;
        d0=(int16_t)-d0;
    } else if (S8(B(0x96))<4) return false;
    if (d0>0x20) return false;
    SW(0x80,0xc0);
    return true;
}
/* 04A064: carry = start a bombing run. The controller's +95 counts the
 * helicopters already updated this frame (slot index): the first one looks
 * for a player below (01AE8E mask, 04A15A) and books the run (+99, +98 =
 * +80); the others join while +95 >= +99 and +95 * +8C <= +8A. */
static bool bombing(neo_gactor_t *a,neo_genv_t *e) {
    neo_gactor_t *r=controller(a,e);
    if (!r) {ga_fail(e,0x4a064);return false;}
    if (ga_b(r,0x95)) {
        if (S8(ga_b(r,0x95))<S8(ga_b(r,0x99))) return false;
        uint16_t d0=(uint16_t)(ga_b(r,0x95)*ga_w(r,0x8c));
        if (!(S16(d0)>S16(ga_w(r,0x8a)))) ga_sb(r,0x99,ga_b(r,0x99)+1u);
        return false;
    }
    if (ga_b(r,0x99)) return false;
    unsigned m=e->players.active_mask&0xffu;
    if (!m) return false;
    bool hit;
    if (m==3u) hit=player_under(a,e->players.p1x) || player_under(a,e->players.p2x);
    else if (m==1u) hit=player_under(a,e->players.p1x);
    else hit=player_under(a,e->players.p2x);
    if (!hit) return false;
    ga_sb(r,0x99,ga_b(r,0x99)+1u);ga_sb(r,0x98,(uint8_t)W(0x80));
    return true;
}
/* 0214B4: move `d0` along heading `d1` (sine table 0029FC). */
static void path_move(neo_gactor_t *a,uint16_t d0,uint8_t d1) {
    int32_t d2=(int32_t)S16(d0)*ga_sine(d1),d3=(int32_t)S16(d0)*ga_cosine(d1);
    SL(0x30,(uint32_t)d3);SL(0x34,(uint32_t)d2);
    SL(0x28,L(0x28)+(uint32_t)d3);SL(0x2c,L(0x2c)+(uint32_t)d2);
}
/* 021288: path interpreter (program +A0, heading +AA, speed +A8, segment
 * progress +A4). Returns carry (path finished). */
static bool path(neo_gactor_t *a,neo_genv_t *e) {
    ga_project(a,e);
    uint16_t d5=W(0xa8);
    for (unsigned guard=0;guard<256;guard++) {
        uint32_t at=L(0xa0);
        if (!ga_rom(at)) {ga_fail(e,at);return false;}
        uint8_t op=rb(at);
        if (op>=0x0b) {ga_fail(e,0x212a4);return false;}
        switch (op) {
        case 0:SW(0xaa,(uint16_t)rb(at+1)<<8);SL(0xa0,at+2);break;
        case 1:case 2: {
            if (!d5) return false;
            uint32_t d0=op==1?((uint32_t)rw(at+2)<<16):((uint32_t)rw(at+4)<<8);
            d0-=L(0xa4)&0xffffff00u;
            if (d0<0x1000000u && d5>=(uint16_t)(d0>>8)) {
                uint16_t step=(uint16_t)(d0>>8);
                d5=(uint16_t)(d5-step);
                SB(0xa6,0);SW(0xa4,0);
                path_move(a,step,B(0xaa));
                if (op==1) SL(0xa0,at+4);
                else {
                    SB(0xaa,B(0xaa)+rb(at+1));
                    if (B(0xaa)==rb(at+2)) SL(0xa0,at+6);
                }
                break;
            }
            SL(0xa4,L(0xa4)+((uint32_t)d5<<8));
            path_move(a,d5,B(0xaa));
            return false;
        }
        case 3:
            SW(0xa4,W(0xa4)+1u);
            if (rw(at+2)>W(0xa4)) return false;
            SW(0xa4,0);SL(0xa0,at+4);
            return false;
        case 4:SL(0xa0,rl(at+2));break;
        case 5: {uint16_t o=rw(at+2);if (o>=0xb0) {ga_fail(e,0x21396);return false;}SL(o,rl(at+4));SL(0xa0,at+8);break;}
        case 6: {uint16_t o=rw(at+4);if (o>=0xb0) {ga_fail(e,0x21376);return false;}SW(o,rw(at+2));SL(0xa0,at+6);break;}
        case 7: {uint16_t o=rw(at+2);if (o>=0xb0) {ga_fail(e,0x21356);return false;}SB(o,rb(at+1));SL(0xa0,at+4);break;}
        case 8:ga_fail(e,rl(at+2));return false;
        case 9:sound(e,rw(at+2));SL(0xa0,at+4);break;
        case 10:path_move(a,d5,B(0xaa));return true;
        }
    }
    ga_fail(e,0x21296);return false;
}
/* 04982A: the target altitude +7A from the controller's +6E, +78 = 0xA0. */
static ga_result_t altitude(neo_gactor_t *a,neo_genv_t *e) {
    neo_gactor_t *r=creator(a,e);
    if (!r) {ga_fail(e,0x4982a);return GA_UNSUPPORTED;}
    if (ga_w(r,0x6e)>0x13u) {ga_fail(e,0x4983a);return GA_UNSUPPORTED;}   /* trap #15 */
    SW(0x7a,ga_w(r,0x6e)*16u+0x100u);SW(0x78,0xa0);
    return GA_MORE;
}
/* 049E8E / 049E80: the leader's position and heading into the controller's
 * slot +61. */
static void record_slot(neo_gactor_t *a,neo_genv_t *e) {
    neo_gactor_t *r=creator(a,e);
    if (!r) {ga_fail(e,0x4879e);return;}
    unsigned i=B(0x61);
    ga_sw(r,0x76+4u*i,W(0x3c));ga_sw(r,0x78+4u*i,W(0x40));ga_sb(r,0x90+i,B(0xaa));
}
/* 0490AE (leader tail): gone with the controller's +61; 00409A must find
 * the controller alive. */
static ga_result_t leader_tail(neo_gactor_t *a,neo_genv_t *e) {
    neo_gactor_t *r=creator(a,e);
    if (r && ga_b(r,0x61)) return GA_DELETE;
    if (!ga_creator_alive(a,e)) {ga_fail(e,0x49152);return GA_UNSUPPORTED;}   /* trap #15 "MISTAKE" */
    return e->unsupported?GA_UNSUPPORTED:GA_MORE;
}
/* 049236: cull by the rectangle E0AAA (01945C -> 004188). */
static ga_result_t cull_small(neo_gactor_t *a,neo_genv_t *e) {
    if (ga_cull(a,e,0xe0aaa)) return GA_DELETE;
    return e->unsupported?GA_UNSUPPORTED:GA_MORE;
}
/* 04920C: cull by E0AA2; a culled helicopter leaves the formation (+96 of
 * the controller) with +61 = FF. */
static ga_result_t cull_body(neo_gactor_t *a,neo_genv_t *e) {
    if (ga_cull(a,e,0xe0aa2)) {
        neo_gactor_t *r=controller(a,e);
        if (r) ga_sb(r,0x96,ga_b(r,0x96)-1u);
        SB(0x61,0xff);
        return GA_DELETE;
    }
    return e->unsupported?GA_UNSUPPORTED:GA_MORE;
}
/* 049030 (helicopter tail): hurtbox (004C24), a hit (+5F bit 7) starts the
 * death (048AB6 with the chain explosion when the formation is complete and
 * the leader 0 is hit, else 048AF6), the controller's +9A bit of the leader
 * also; then the cull 04920C. */
static ga_result_t body_tail(neo_gactor_t *a,neo_genv_t *e) {
    if (ga_register(a,e,NULL,NULL)) SB(0x5a,0);
    neo_gactor_t *l=creator(a,e),*r=l?creator(l,e):NULL;
    if (!l || !r) {ga_fail(e,0x49048);return GA_UNSUPPORTED;}
    if (BIT(0x5f,7)) {
        if (ga_b(r,0x96)==ga_w(r,0x72) && !ga_b(l,0x61)) {
            ga_sb(r,0x9a,ga_b(r,0x9a)|1u);ga_sb(r,0x5e,B(0x5e));
            SL(0,0x48ab6);
        } else SL(0,0x48af6);
    }
    if ((ga_b(r,0x9a)>>(ga_b(l,0x61)&7u))&1u) SL(0,0x48ab6);
    return cull_body(a,e);
}
static bool heli_call(neo_gactor_t *a,neo_genv_t *e,neo_anim_kind_t kind,uint32_t routine,uint16_t parameter);
/* 04857C / 0485EC: the formation controller. */
static ga_result_t controller_tick(neo_gactor_t *a,neo_genv_t *e) {
    switch (L(0)) {
    case 0x4857c: {
        SB(0x95,0);SB(0x61,0);SB(0x60,0);SB(0x96,0);SW(0x8e,0);SB(0x9a,0);SB(0x9b,0);SB(0x9c,0);
        SW(0x8c,rank(a,e,0x2d1656));
        ga_project(a,e);
        uint16_t d7=W(0x72);
        if (!d7) {ga_fail(e,0x485b6);return GA_UNSUPPORTED;}   /* dbra-less loop: 65536 leaders */
        do {
            neo_gactor_t *c=child_at(a,e,0x4870c,0,0);
            if (c) ga_sb(c,0x61,(uint8_t)(d7-1u));
            SB(0x96,B(0x96)+1u);
        } while (--d7);
        SL(0,0x485ec);
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    }
    case 0x485ec:
        ga_project(a,e);SB(0x95,0);
        if (!B(0x96)) {SB(0x69,0xff);SB(0x61,0xff);return GA_DELETE;}   /* 019DFC, 004188 */
        if (S8(B(0x96))>S8(B(0x99))) SW(0x8e,0);
        else if (S16(W(0x8e))<0x40) SW(0x8e,W(0x8e)+1u);
        else {SB(0x99,0);SW(0x8a,0);}
        if (B(0x99)) SW(0x8a,W(0x8a)+1u);
        if (BIT(0x9a,0)) {
            if (!B(0x9b)) {
                if (S8(B(0x9c))<5) {SB(0x9b,5);SB(0x9a,B(0x9a)|(1u<<(B(0x9c)&7u)));SB(0x9c,B(0x9c)+1u);}
            } else SB(0x9b,B(0x9b)-1u);
        }
        return GA_MORE;
    }
    ga_fail(e,L(0));return GA_UNSUPPORTED;
}
/* 04870C..048832: the leader. */
static ga_result_t leader_tick(neo_gactor_t *a,neo_genv_t *e) {
    uint32_t pc=L(0);
    for (unsigned guard=0;guard<8;guard++) switch (pc) {
    case 0x4870c:
        SB(0x94,B(0x61)<<4);SL(0,0x4871c);
        /* fall through */
    case 0x4871c:
        ga_project(a,e);
        if (B(0x94)) SB(0x94,B(0x94)-1u);
        else {
            if (S16(W(0x3c))<0xa0) SET(0x15,0); else CLR(0x15,0);
            SB(0x60,0);
            if (e->spawn) e->spawn(e->ctx,a,0x48882,false);               /* 0040BC, no placement */
            SL(0,0x4876c);
        }
        return leader_tail(a,e);
    case 0x4876c: {
        neo_gactor_t *r=creator(a,e);
        if (!r) {ga_fail(e,0x4876c);return GA_UNSUPPORTED;}
        SW(0x74,ga_w(r,0x74));
        if (altitude(a,e)!=GA_MORE) return GA_UNSUPPORTED;
        SL(0,0x48780);
    }   /* fall through */
    case 0x48780:
        steer(a);bank_by_speed(a);ga_move(a,e);
        if (near_target(a,true,8)) SL(0,0x487d0);
        record_slot(a,e);
        return leader_tail(a,e);
    case 0x487d0: {
        neo_gactor_t *r=creator(a,e);
        if (!r) {ga_fail(e,0x487d0);return GA_UNSUPPORTED;}
        SW(0x74,0);SL(0x30,0);SL(0x34,0);SW(0xa8,0);SB(0x60,1);
        SL(0xa0,rl((FACING?0xe0afeu:0xe0af2u)+(uint32_t)ga_w(r,0x70)*4u));
        SW(0xa4,0);SB(0xa6,0);
        SW(0x76,rank(a,e,0x2d1a92));
        SL(0,0x48832);
    }   /* fall through */
    case 0x48832:
        SW(0xa8,W(0xa8)+0x30u);
        if (!(S16(W(0x76))>S16(W(0xa8)))) SW(0xa8,W(0x76));
        path(a,e);
        if (e->unsupported) return GA_UNSUPPORTED;
        record_slot(a,e);
        return leader_tail(a,e);
    default: ga_fail(e,pc);return GA_UNSUPPORTED;
    }
    ga_fail(e,pc);return GA_UNSUPPORTED;
}
/* 048882..048FFE: the helicopter, its deaths and debris. */
static ga_result_t body_tick(neo_gactor_t *a,neo_genv_t *e) {
    uint32_t pc=L(0);bool done=false;
    for (unsigned guard=0;guard<8;guard++) switch (pc) {
    case 0x48882: {
        SW(0x86,(W(0x72)<<5)&0x7fffu);                                  /* 049EBE */
        SB(0x1d,0);                                                     /* 009F88 */
        SW(0x66,0x8600);SW(0x1e,0x8600);SL(0x50,0xe0ab2);
        on_creator(a,e);
        SW(0x4e,rank(a,e,0x2d1530));
        SB(0x61,0);SB(0x60,0);SB(0x96,0);
        START(0x245178);SET(0x5f,6);
        neo_gactor_t *l=creator(a,e);
        if (!l) {ga_fail(e,0x488e0);return GA_UNSUPPORTED;}
        if (!ga_b(l,0x61)) {ga_palette(a,0x15a);sound(e,0x10e8);} else ga_palette(a,0x20);
        ga_flash_palette(a,0x15c);
        neo_gactor_t *c=e->spawn?e->spawn(e->ctx,a,0x47874,false):NULL;
        if (c) ga_sb(c,0x60,1);
        SL(0,0x4893e);
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    }
    case 0x4893e: {
        neo_gactor_t *l=creator(a,e);
        if (!l) {ga_fail(e,0x4893e);return GA_UNSUPPORTED;}
        if (ga_b(l,0x15)&1u) SET(0x15,0); else CLR(0x15,0);
        ga_project(a,e);STEP(done);
        if (ga_b(l,0x60)==1u) SL(0,0x48982);
        return body_tail(a,e);
    }
    case 0x48982:
        to_root(a,e);
        SB(0x96,FACING?0x0b:3);
        START(0x245188);CLR(0x15,0);CLR(0x5f,6);
        /* fall through */
    case 0x489be:
        SW(0x84,0);SL(0,0x489c8);
        /* fall through */
    case 0x489c8: {
        follow_slot(a,e);ga_move(a,e);
        if ((W(0x66)&0xf000u)==0x8000u) SW(0x1e,W(0x66)-W(0x40));      /* 01AD8C */
        bank_by_heading(a);
        if (bombing(a,e)) SL(0,0x48a06);
        if (e->unsupported) return GA_UNSUPPORTED;
        STEP(done);SB(0x96,B(0x96)+3u);
        neo_gactor_t *r=controller(a,e);
        if (!r) {ga_fail(e,0x489f4);return GA_UNSUPPORTED;}
        ga_sb(r,0x95,ga_b(r,0x95)+1u);
        return body_tail(a,e);
    }
    case 0x48a06:
        SB(0x97,rank(a,e,0x2d18d6));SB(0x98,rank(a,e,0x2d1840));SL(0,0x48a2c);
        /* fall through */
    case 0x48a2c: {
        follow_slot(a,e);ga_move(a,e);
        if ((W(0x66)&0xf000u)==0x8000u) SW(0x1e,W(0x66)-W(0x40));      /* 01AD8C */
        if (bank_towards(a,4)) {
            if (B(0x97)) SB(0x97,B(0x97)-1u);
            else {
                SB(0x97,rank(a,e,0x2d18d6));
                SW(0x80,(uint16_t)B(0x96)*8u+0xa0u);
                neo_gactor_t *c=e->spawn?e->spawn(e->ctx,a,0x47a6c,false):NULL;
                if (c) ga_sb(c,0x60,0);
                SB(0x98,B(0x98)-1u);
                if (!B(0x98)) SL(0,0x489be);
            }
        }
        if (e->unsupported) return GA_UNSUPPORTED;
        STEP(done);SB(0x96,B(0x96)+3u);
        neo_gactor_t *r=controller(a,e);
        if (!r) {ga_fail(e,0x48aa4);return GA_UNSUPPORTED;}
        ga_sb(r,0x95,ga_b(r,0x95)+1u);
        return body_tail(a,e);
    }
    case 0x48ab6: {
        /* The points (08D1D0: player of the controller's +5E, value
         * E5582 by the leader's index + 1, label 08D0D2 after the script
         * actor), then the explosion. */
        SB(0x17,0xff);SB(0x60,0xff);
        neo_gactor_t *l=creator(a,e),*r=l?creator(l,e):NULL;
        if (!l || !r) {ga_fail(e,0x48ac2);return GA_UNSUPPORTED;}
        uint16_t d3=(uint16_t)(ga_b(l,0x61)+1u);
        uint8_t d0=(uint8_t)((ga_b(r,0x5e)>>2)&3u);
        if (d0) {
            if (e->pickup && d0<=2u) neo_item_add_score(e->pickup->score[d0-1u],rl(0xe5582u+d3*4u));   /* 057948 */
            sound(e,0x1121);
            neo_gactor_t *c=after_script(a,e,0x8d0d2,0,-0x18);
            if (c) {ga_sb(c,0x87,0);ga_sb(c,0x71,(uint8_t)((d0-1u)&1u));ga_sb(c,0x70,(uint8_t)d3);}
        }
        to_root(a,e);SW(0x26,0);
        pc=0x48e22;continue;
    }
    case 0x48af6:
        SB(0x17,0xff);SB(0x60,0xff);pc=0x48b10;continue;
    case 0x48b06:
        SB(0x17,0xff);SB(0x60,0);
        /* fall through */
    case 0x48b10:
        to_root(a,e);SW(0x26,0);
        if (!(ga_rand(e)&1u)) {pc=0x48e22;continue;}
        /* 048B2E: the fall. */
        {uint32_t vx,vy;ga_polar(0x50000,(int32_t)L(0x30)<0?0x60:0x20,&vx,&vy);SL(0x30,vx);SL(0x34,vy);}
        smoke(a,e,0xe0a50);
        puff(a,e,0x1c71e);
        SW(0x94,5);SW(0x8e,W(0x94));
        SW(0x8c,(ga_rand(e)&0x1fu)+0x10u);
        sound(e,0x1022);SL(0,0x48bbc);
        if (e->unsupported) return GA_UNSUPPORTED;
        pc=0x48bbc;continue;                                            /* no rts at 048BB6 */
    case 0x48bbc: {
        ga_air(a,e);SL(0x34,L(0x34)-0xe000u);
        uint8_t saved=B(0x96);
        if (S8(saved)>=3) SB(0x96,saved-3u);
        STEP(done);SB(0x96,saved);
        if ((int32_t)L(0x34)<0 && ga_probe_vertical(a,e,(int16_t)(L(0x34)>>16))) SL(0,0x48c8c);
        SW(0x8e,W(0x8e)-1u);
        if (!W(0x8e)) {SW(0x8e,W(0x94));puff(a,e,0x1c6ee);}
        if (B(0x8c)) {                                                  /* the high byte: never */
            SW(0x8c,W(0x8c)-1u);
            if (!W(0x8c) && !(ga_rand(e)&1u)) {ga_fail(e,0x48cd2);return GA_UNSUPPORTED;}
        }
        if (ga_cull(a,e,0xe0aa2)) {
            if (B(0x60)) {neo_gactor_t *r=controller(a,e);if (r) ga_sb(r,0x96,ga_b(r,0x96)-1u);}
            SB(0x61,0xff);return GA_DELETE;
        }
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    }
    case 0x48c8c: {
        smoke(a,e,0xe0a3e);SB(0x61,0xff);SW(0x66,0x1001);SW(0x1e,0x1001);SB(0x1d,0);
        if (B(0x60)) {neo_gactor_t *r=controller(a,e);if (r) ga_sb(r,0x96,ga_b(r,0x96)-1u);}
        sound(e,0x1021);
        /* 048D22: the wreck. */
        START(0x22cf2c);ga_palette(a,0x204);
        if (!(ga_rand(e)&2u)) SB(0x15,B(0x15)^1u);                      /* 01DEA2 */
        CLR(0x15,1);SL(0x50,0);SL(0x30,0);SL(0x34,0);SL(0,0x48d5e);
    }   /* fall through */
    case 0x48d5e:
        ga_project(a,e);STEP(done);
        if (done) {SB(0x61,0xff);return GA_DELETE;}
        ga_attack(a,e,0xe0ae2);
        ga_cull_rect(a,e,0x48e1a);                                      /* 019504, result unused */
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    case 0x48e22:
        SB(0x61,0xff);smoke(a,e,0xe0a3e);
        child_at(a,e,0x48eda,0,0);child_at(a,e,0x48efe,0,0);child_at(a,e,0x48f26,0,0);child_at(a,e,0x48f4e,0,0);
        SW(0x66,0x1001);SW(0x1e,0x1001);SB(0x1d,0);
        if (B(0x60)) {neo_gactor_t *r=controller(a,e);if (r) ga_sb(r,0x96,ga_b(r,0x96)-1u);}
        SL(0x30,0);SL(0x34,0);sound(e,0x1021);
        SL(0,0x1c71e);
        return neo_effects_tick(a,e);                                   /* jmp 01C71E */
    case 0x48eda: case 0x48efe: case 0x48f26: case 0x48f4e: {
        uint8_t base=pc==0x48eda?0:pc==0x48efe?0x40:pc==0x48f26?0x80:0xc0;
        uint32_t vx,vy;ga_polar(0x180000,(uint8_t)((ga_rand(e)&0x3fu)+base),&vx,&vy);
        SL(0x30,vx);SL(0x34,vy);
        SW(0x94,4);SW(0x8e,6);SW(0x8c,4);SL(0,0x48f8c);
        return GA_MORE;
    }
    case 0x48f8c: {
        ga_project(a,e);
        SW(0x94,W(0x94)-1u);
        if (W(0x94)) return GA_MORE;
        SW(0x94,W(0x8e));
        SL(0x28,L(0x28)+L(0x30));SL(0x2c,L(0x2c)+L(0x34));
        /* 01B1EC: carry always clear. */
        neo_gactor_t *c=puff(a,e,0x1c6ee);
        if (c) {uint8_t v=rb(0x49006u+W(0x8c)-1u);ga_sb(c,0x12,v);ga_sb(c,0x13,v);}
        SW(0x8c,W(0x8c)-1u);
        if (!W(0x8c)) return GA_DELETE;
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    }
    default: ga_fail(e,pc);return GA_UNSUPPORTED;
    }
    ga_fail(e,pc);return GA_UNSUPPORTED;
}
/* 047874 / 0478C4: the rotor (program 24483E by the creator's bank +96,
 * mirrored past 7; flash timer shared, 0044F6). */
static ga_result_t rotor_tick(neo_gactor_t *a,neo_genv_t *e) {
    switch (L(0)) {
    case 0x47874:
        SB(0x1d,0);ga_palette(a,0x21);ga_flash_palette(a,0x15c);
        SW(0x66,0x8600);SW(0x1e,0x8600);on_creator(a,e);
        START(0x24483e);SL(0,0x478c4);
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    case 0x478c4: {
        neo_gactor_t *p=creator(a,e);
        if (!p) {ga_fail(e,0x478c4);return GA_UNSUPPORTED;}
        if (ga_b(p,0x61)) return GA_DELETE;
        SB(0x96,ga_b(p,0x96));
        if (ga_b(p,0x15)&1u) SET(0x15,0); else CLR(0x15,0);
        if (S8(B(0x96))>7) {
            if (B(0x60)) SB(0x15,B(0x15)^1u);
            SB(0x96,(uint8_t)(0u-(uint8_t)(B(0x96)-0x0eu)));
        }
        ga_project(a,e);
        {uint8_t f=ga_b(p,0x17);if (f) {if (!(f&0x80u)) f++;SB(0x17,f);}}   /* 0044F6 */
        int r=ga_anim_step(a,e,NULL);
        if (r<0) return GA_UNSUPPORTED;
        return GA_MORE;
    }
    }
    ga_fail(e,L(0));return GA_UNSUPPORTED;
}
/* 047A6C / 047AE0: the dropper (program 244F06 with the callback 04A262),
 * gone when it ends or leaves E0AAA. */
static ga_result_t dropper_tick(neo_gactor_t *a,neo_genv_t *e) {
    switch (L(0)) {
    case 0x47a6c: {
        neo_gactor_t *p=creator(a,e);
        if (!p) {ga_fail(e,0x47a6c);return GA_UNSUPPORTED;}
        SW(0x80,ga_w(p,0x80));SB(0x96,ga_b(p,0x96));
        if (S8(B(0x96))>7) {SB(0x96,(uint8_t)(0u-(uint8_t)(B(0x96)-0x0eu)));SET(0x15,0);}
        START(0x244f06);
        SB(0x1d,0);ga_palette(a,0x22);SW(0x66,0x8600);SW(0x1e,0x8600);
        on_creator(a,e);SL(0,0x47ae0);
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    }
    case 0x47ae0: {
        ga_project(a,e);
        int r=ga_anim_step(a,e,heli_call);
        if (r<0) return GA_UNSUPPORTED;
        if (r) return GA_DELETE;
        return cull_small(a,e);
    }
    }
    ga_fail(e,L(0));return GA_UNSUPPORTED;
}
/* 04A262 (and 04A1AE/04A208 of the lone helicopter): the bomb after the
 * dropper and the shell 047DE0 after the script actor at (0,-8) with the
 * bank +96 and the facing. */
static bool heli_call(neo_gactor_t *a,neo_genv_t *e,neo_anim_kind_t kind,uint32_t routine,uint16_t parameter) {
    (void)parameter;
    if (kind!=NEO_ANIM_CALL) return false;
    if (routine==0x4a1aeu || routine==0x4a208u) {ga_fail(e,routine);return true;}
    if (routine!=0x4a262u) return false;
    if (B(0x60)) return true;
    child_at(a,e,0x47b24,0,0);
    neo_gactor_t *c=after_script(a,e,0x47de0,0,-8);
    if (c) {
        ga_sb(c,0x96,B(0x96));
        if (FACING) ga_sb(c,0x15,(uint8_t)(ga_b(c,0x15)|1u));
    }
    return true;
}
/* 047B24..048560: the bomb. */
static ga_result_t bomb_tick(neo_gactor_t *a,neo_genv_t *e) {
    uint32_t pc=L(0);
    for (unsigned guard=0;guard<4;guard++) switch (pc) {
    case 0x47b24: {
        SW(0x76,rank(a,e,0x2d19b4));
        SB(0x1d,0);SW(0x66,0xe0ef);SW(0x1e,0xe0ef);SET(0x15,5);
        ga_palette(a,0x22);ga_palette(a,0x178);ga_palette(a,0x179);ga_palette(a,0x202);
        SB(0x14,B(0x19));
        neo_gactor_t *p=creator(a,e);
        if (!p) {ga_fail(e,0x47b92);return GA_UNSUPPORTED;}
        uint32_t vx,vy;ga_polar((uint32_t)(int32_t)S16(W(0x76))<<8,(uint8_t)ga_w(p,0x80),&vx,&vy);
        SL(0x30,vx);SL(0x34,vy);
        START(0x244dbc);
        uint32_t t=0xe0a1eu+(uint32_t)ga_b(p,0x96)*4u;
        int16_t dx=S16(rw(t));
        if (ga_b(p,0x15)&1u) dx=(int16_t)-dx;
        SW(0x28,W(0x28)+(uint16_t)dx);SW(0x2c,W(0x2c)+rw(t+2u));
        SL(0,0x47bec);
    }   /* fall through */
    case 0x47bec:
        ga_air(a,e);
        if (W(0x8a)) SW(0x8a,W(0x8a)-1u);
        else {int r=ga_anim_step(a,e,NULL);if (r<0) return GA_UNSUPPORTED;}
        if (BIT(0x15,7)) SL(0,0x48510);
        if (BIT(0x5f,0)) SL(0,0x48510);
        if (ga_attack(a,e,0xe0abe)) SL(0,0x47c48);
        return cull_small(a,e);
    case 0x47c48:
        SB(0x14,B(0x1c));
        SW(0x28,W(0x28)+(uint16_t)((ga_rand(e)&0xfu)-7u));
        SW(0x2c,W(0x2c)+(uint16_t)((ga_rand(e)&0xfu)-7u));
        SW(0x66,0xe0ed);SW(0x1e,0xe0ed);
        START(0x22e504);SL(0,0x47c90);
        /* fall through */
    case 0x47c90: {
        ga_project(a,e);
        int r=ga_anim_step(a,e,NULL);
        if (r<0) return GA_UNSUPPORTED;
        if (r) return GA_DELETE;
        if (W(0x3c)>=0x140u) return GA_DELETE;
        if ((uint16_t)(W(0x40)-0x100u)>=0x100u) return GA_DELETE;
        return GA_MORE;
    }
    case 0x48510:
        SB(0x1d,0);ga_palette(a,0x22);SW(0x66,0x8600);SW(0x1e,0x8600);
        START(0x245106);SB(0x60,0);
        SW(0x28,W(0x28)+(uint16_t)(ga_rand_range(e,9)-4u));
        ga_floor(a,e);SL(0,0x48560);
        if (e->unsupported) return GA_UNSUPPORTED;
        pc=0x48560;continue;                                            /* no rts at 04855A */
    case 0x48560: {
        ga_project(a,e);
        int r=ga_anim_step(a,e,NULL);
        if (r<0) return GA_UNSUPPORTED;
        if (r) return GA_DELETE;
        return cull_small(a,e);
    }
    default: ga_fail(e,pc);return GA_UNSUPPORTED;
    }
    ga_fail(e,pc);return GA_UNSUPPORTED;
}
/* 047DE0 / 047E92: the shell. */
static ga_result_t shell_tick(neo_gactor_t *a,neo_genv_t *e) {
    switch (L(0)) {
    case 0x47de0: {
        SB(0x1d,0);ga_palette(a,0x23);SW(0x66,0x1110);SW(0x1e,0x1110);
        START(0x244f9e);
        int16_t dx=S16(rw(0xe0a0eu+(uint32_t)B(0x96)*2u));
        if (FACING) dx=(int16_t)-dx;
        SW(0x28,W(0x28)+(uint16_t)dx);
        uint32_t s=(ga_rand(e)&0x7fffu)+0x18000u;
        uint16_t d1=ga_rand(e)&7u;
        d1=FACING?(uint16_t)(0xfcu-d1):(uint16_t)(0x84u+d1);
        uint32_t vx,vy;ga_polar(s,(uint8_t)d1,&vx,&vy);
        SL(0x30,vx);SL(0x34,vy);SL(0x90,L(0x34));
        SB(0x61,0);SW(0x94,0x12);SL(0,0x47e92);
        return GA_MORE;
    }
    case 0x47e92: {
        SL(0x34,L(0x34)+0xffffb000u);ga_air(a,e);                      /* 0143DC */
        int r=ga_anim_step(a,e,NULL);
        if (r<0) return GA_UNSUPPORTED;
        if (BIT(0x15,6)) {
            if (B(0x61)) return GA_DELETE;
            CLR(0x15,6);SET(0x5f,3);SB(0x61,0xff);
            SL(0x34,(uint32_t)((int32_t)(0u-L(0x90))>>2));
        } else SL(0x90,L(0x34));
        SW(0x94,W(0x94)-1u);
        if (!W(0x94)) return GA_DELETE;
        return cull_small(a,e);
    }
    }
    ga_fail(e,L(0));return GA_UNSUPPORTED;
}
static bool body(uint32_t r) {
    static const uint32_t own[]={0x48882,0x4893e,0x48982,0x489be,0x489c8,0x48a06,0x48a2c,0x48ab6,0x48af6,0x48b06,0x48b10,
        0x48bbc,0x48c8c,0x48d5e,0x48e22,0x48eda,0x48efe,0x48f26,0x48f4e,0x48f8c};
    for (unsigned i=0;i<sizeof(own)/sizeof(own[0]);i++) if (own[i]==r) return true;
    return false;
}
static bool leader(uint32_t r) {return r==0x4870c || r==0x4871c || r==0x4876c || r==0x48780 || r==0x487d0 || r==0x48832;}
static bool bomb(uint32_t r) {return r==0x47b24 || r==0x47bec || r==0x47c48 || r==0x47c90 || r==0x48510 || r==0x48560;}
bool neo_heli_routine(uint32_t r) {
    return r==0x4857c || r==0x485ec || leader(r) || body(r) || r==0x47874 || r==0x478c4 || r==0x47a6c || r==0x47ae0 || bomb(r) || r==0x47de0 || r==0x47e92;
}
ga_result_t neo_heli_tick(neo_gactor_t *a,neo_genv_t *e) {
    uint32_t r=L(0);
    if (r==0x4857c || r==0x485ec) return controller_tick(a,e);
    if (leader(r)) return leader_tick(a,e);
    if (body(r)) return body_tick(a,e);
    if (r==0x47874 || r==0x478c4) return rotor_tick(a,e);
    if (r==0x47a6c || r==0x47ae0) return dropper_tick(a,e);
    if (bomb(r)) return bomb_tick(a,e);
    return shell_tick(a,e);
}
