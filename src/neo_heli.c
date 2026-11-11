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
 * The lone helicopter 0474B6 of Mission 5 (0474B6..047872, its dropper
 * 0479B8 and bomb 047AFC) is further down. Not translated (explicit stops):
 * the other lone helicopter 047F7E..0484FE (dropper 047A12, bomb 047B10,
 * callback 04A208), the spawn word +6E > 0x13 (trap #15 "takasa err"), the
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
    case 0x479b8: {                                                     /* the lone helicopter's dropper */
        neo_gactor_t *p=creator(a,e);
        if (!p) {ga_fail(e,0x479b8);return GA_UNSUPPORTED;}
        if (ga_b(p,0x15)&1u) SET(0x15,0); else CLR(0x15,0);
        SW(0x80,ga_w(p,0x80));SB(0x96,ga_b(p,0x96));
        if (S8(B(0x96))>7) SB(0x96,(uint8_t)(0u-(uint8_t)(B(0x96)-0x0eu)));
        START(0x244dd6);
        SB(0x1d,0);ga_palette(a,0x22);SW(0x66,0x8600);SW(0x1e,0x8600);  /* 047AAA */
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
    if (routine==0x4a208u) {ga_fail(e,routine);return true;}             /* the bomb 047B10 of the helicopter 047F7E: not translated */
    if (routine!=0x4a262u && routine!=0x4a1aeu) return false;
    if (B(0x60)) return true;
    child_at(a,e,routine==0x4a1aeu?0x47afcu:0x47b24u,0,0);
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
    case 0x47afc:
    case 0x47b24: {
        SW(0x76,rank(a,e,pc==0x47afcu?0x2d18f0u:0x2d19b4u));
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
/* ---- the lone helicopter 0474B6 (Mission 5 street and metro; script records
 * with +6E altitude index, +70 the angle set of 0E0962, +72 its stay) --------
 * 0474B6: 01ABC8, +86 = (+72 << 5) & 7FFF (049EBE), hurtbox E0AB2, palette
 * 20, flash 15C, +4E = rank 2D146C, +76 = rank 2D1A16 (top speed), facing left
 * when born left of A0, the rotor 047874 (0040BC, +60 = 0), program 244712.
 * 04755A: sound 10E8. 04756A: 049324 (+7A = 100 + 16 * +6E, +78 = the nearest
 * player's x or A0, +7C = that player), 0499A4, 049A92, 0143E4, 00427A;
 * within 16 px (049E46) -> 047598: stopped, program E0C32[+96 & 3]; 0475CE:
 * the hover 049E9E (+82 -= 4, +34 = the y of 4000 along it), at the program's
 * end 0475EE: +84 = rank 2D1592, +7C = a player (01B04A); 047608: program
 * 24473E, +9B = +A8 = +90 = 0. 047630: 049E9E, 04936C (the target x: by the
 * angle to the player 16 px higher (049CB2, 01B12E) the bombing angle +80 of
 * the set 0E0962[+70] and the x that gives it; at the screen's sides 18 / 128
 * the player's x, +80 = C0, +9B = FF), 049F9E (speed +90 up to +76 towards
 * (+78, +7A), there at once inside +90 px), 0143E4, 049B20 (the bank +96 by
 * +30, nine steps, held by the bank of +80 unless +9B; facing left from 5),
 * 00427A; at the x (049E74, 2 px) or +84 run out -> 04768E; 049ECE (+86 run
 * out) -> 04783A. 04769C: stopped; a player alive (01B04A): +9B -> 047608;
 * +84 = 0 and +30 = 0 -> 04771A; else off the x by more than 14 px -> 047608.
 * 047720: the bank +96 a step per update to the one of +80 (E09F4), then
 * 04778E: +98 = rank 2D177C bombs, one every +97 = rank 2D18A2 updates
 * (0477BE: the dropper 0479B8, 0040BC with D0 = 2A), then 0475EE again.
 * 04783A: +7A = 2FF, program 24473E; 047856: 049A1E, 0143E4, 049B20: it leaves
 * upwards. Every state ends in 04900C: 004C24, +5F bit 7 -> 048B06, outside
 * E0AA2 (049166): +61 = FF, 004188.
 * The dropper 0479B8 (facing, +80 and the bank of the helicopter; program
 * 244DD6 with the sound 10FF and the callback 04A1AE: the bomb 047AFC, speed
 * rank 2D18F0, and the shell 047DE0). */
static neo_gactor_t *kid(neo_gactor_t *a,neo_genv_t *e,uint32_t routine,uint16_t reserve) {   /* 0040BC, no placement */
    return e->spawn_reserve?e->spawn_reserve(e->ctx,a,routine,reserve):e->spawn?e->spawn(e->ctx,a,routine,false):NULL;
}
static uint8_t rbx(neo_genv_t *e,uint32_t address) {const uint8_t *p=ga_rom(address);if (!p) {ga_fail(e,address);return 0;} return p[0];}
static uint16_t rwx(neo_genv_t *e,uint32_t address) {const uint8_t *p=ga_rom(address);if (!p) {ga_fail(e,address);return 0;} return (uint16_t)(p[0]<<8|p[1]);}
#define P1_NODE 0x1014c4u
#define P2_NODE 0x101574u
static uint16_t node_x(const neo_genv_t *e,uint32_t node) {return node==P2_NODE?e->players.p2x:e->players.p1x;}
static uint16_t node_y(const neo_genv_t *e,uint32_t node) {return node==P2_NODE?e->players.p2y:e->players.p1y;}
/* 01AF9A: the nearest live player's node; carry (true) when none (player 1). */
static bool lone_nearest(neo_gactor_t *a,neo_genv_t *e,uint32_t *node) {
    unsigned m=e->players.active_mask&0xffu;
    *node=P1_NODE;
    if (!m) return true;
    if (m==1u) return false;
    if (m==2u) {*node=P2_NODE;return false;}
    uint16_t d1=(uint16_t)(e->players.p1x-W(0x3c)),d2=(uint16_t)(e->players.p2x-W(0x3c));
    if (d1&0x8000u) d1=(uint16_t)(0u-d1);
    if (d2&0x8000u) d2=(uint16_t)(0u-d2);
    if (S16(d2)<S16(d1)) *node=P2_NODE;
    return false;
}
/* 01B04A: player 1, 2 or (both live) one at random; carry when none. */
static bool lone_any(neo_genv_t *e,uint32_t *node) {
    unsigned d0=e->players.active_mask&0xffu;
    if (d0>=3u) d0=(ga_rand(e)&1u)+1u;
    *node=d0==2u?P2_NODE:P1_NODE;
    return d0==0;
}
/* 049324 / 04936C head: the altitude +7A (trap #15 "takasa err" past 13). */
static bool lone_altitude(neo_gactor_t *a,neo_genv_t *e) {
    if (W(0x6e)>0x13u) {ga_fail(e,0x49330);return false;}
    SW(0x7a,W(0x6e)*16u+0x100u);
    return true;
}
/* 049CB2: the bombing angle +80 of the set 0E0962[+70] for the angle to the
 * player +7C (16 px higher: 01B12E) and the x +78 that gives it; carry when
 * both neighbours of the angle are marked (+80 = FFFF). */
static bool lone_angle(neo_gactor_t *a,neo_genv_t *e) {
    uint32_t node=L(0x7c);
    int16_t d0=(int16_t)(neo_angle(S16((uint16_t)(node_x(e,node)-W(0x3c))),S16((uint16_t)(node_y(e,node)+0x10u-W(0x40))))&0xffu);
    uint32_t t=(uint32_t)rwx(e,0xe0962u+4u*W(0x70))<<16|rwx(e,0xe0962u+4u*W(0x70)+2u);
    if (e->unsupported) return false;
    unsigned i=0;
    for (;;) {
        if (d0<S16(rwx(e,t+4u*i))) break;
        i++;
        if (rwx(e,t+4u*i)==0xffffu || e->unsupported) {ga_fail(e,0x49e42);return false;}   /* trap #15 */
    }
    if (!i) {ga_fail(e,0x49cfa);return false;}                           /* the entry before the first: outside the table */
    unsigned d5=(rwx(e,t+4u*i+2u)?1u:0u)|(rwx(e,t+4u*i-2u)?2u:0u);
    int16_t d7=S16(rwx(e,t+4u*i)),d6=S16(rwx(e,t+4u*i-4u)),d1;
    if (d5==3u) {SW(0x80,0xffff);return true;}
    if (d5) d1=(d5&2u)?d7:d6;
    else {
        int16_t n1=(int16_t)(d6-d0),n2=(int16_t)(d7-d0);
        if (n1<0) n1=(int16_t)-n1;
        if (n2<0) n2=(int16_t)-n2;
        d1=n1>n2?d7:d6;
    }
    if (d1<0x80) {SW(0x80,(uint16_t)(0u-(uint16_t)d1)&0xffu);SW(0x78,W(0x3c)+(uint16_t)(d1-d0));}
    else {SW(0x80,d1);SW(0x78,W(0x3c)-(uint16_t)(d1-d0));}
    return false;
}
/* 04936C */
static void lone_target(neo_gactor_t *a,neo_genv_t *e) {
    if (!lone_altitude(a,e)) return;
    if (B(0x9b)) return;
    uint32_t node=L(0x7c);
    bool live=(node==P1_NODE && (e->players.active_mask&1u)) || (node==P2_NODE && (e->players.active_mask&2u));   /* 01AEDA */
    if (!live) {lone_nearest(a,e,&node);SL(0x7c,node);}
    if (lone_angle(a,e)) SW(0x78,node_x(e,L(0x7c)));
    if (S16(W(0x3c))<0x18) {SW(0x78,node_x(e,L(0x7c)));SW(0x80,0xc0);SB(0x9b,0xff);}
    if (S16(W(0x3c))>0x128) {SW(0x78,node_x(e,L(0x7c)));SW(0x80,0xc0);SB(0x9b,0xff);}
}
/* 049E9E: the hover. */
static void lone_hover(neo_gactor_t *a) {
    SW(0x82,(W(0x82)-4u)&0xffu);
    uint32_t vx,vy;ga_polar(0x4000,(uint8_t)W(0x82),&vx,&vy);SL(0x34,vy);
}
/* 049ECE: carry once the stay +86 has run out (it stays at 1). */
static bool lone_leave(neo_gactor_t *a) {
    if (!W(0x86)) return false;
    SW(0x86,W(0x86)-1u);
    if (W(0x86)) return false;
    SW(0x86,1);return true;
}
/* 049F9E: the speed +90 (+ d0 per update, at most +76) towards (+78, +7A);
 * inside +90 px it is put there (the projected longs; +30 cleared twice, +34
 * kept, as the original). */
static void lone_fly(neo_gactor_t *a,neo_genv_t *e,uint16_t accel) {
    if (W(0x6e)>0x13u) {ga_fail(e,0x49fb0);return;}
    int32_t d=(int32_t)((uint32_t)distance(W(0x78),W(0x6e)*16u+0x100u,W(0x3c),W(0x40))<<16);
    if (!(d>(int32_t)((uint32_t)W(0x90)<<8))) {
        SL(0x30,0);
        SL(0x3c,(uint32_t)W(0x78)<<16);SL(0x40,(uint32_t)W(0x7a)<<16);
        SW(0x90,0);
        return;
    }
    SW(0x90,W(0x90)+accel);
    if (S16(W(0x76))<S16(W(0x90))) SW(0x90,W(0x76));
    uint8_t ang=neo_angle(S16((uint16_t)(W(0x78)-W(0x3c))),S16((uint16_t)(W(0x7a)-W(0x40))));
    SB(0xaa,ang);
    uint32_t vx,vy;ga_polar((uint32_t)W(0x90)<<8,ang,&vx,&vy);SL(0x30,vx);SL(0x34,vy);
}
/* 049A1E: as 0499A4 with the speed distance << 12. */
static void lone_steer(neo_gactor_t *a) {
    int16_t dx=(int16_t)(S16((uint16_t)(W(0x78)-W(0x3c)))>>1),dy=(int16_t)(S16((uint16_t)(W(0x7a)-W(0x40)))>>1);
    uint16_t d0=neo_angle(dx,dy),d1=W(0x74)&0x1fu;
    if (FACING) d1=(uint16_t)(0u-d1);
    SW(0x82,(d0+d1)&0xffu);
    uint32_t s=((uint32_t)distance(W(0x3c),W(0x40),W(0x78),W(0x7a))<<16)>>4;
    uint32_t vx,vy;ga_polar(s,(uint8_t)W(0x82),&vx,&vy);
    int32_t ex=(int32_t)(vx-L(0x30)),ey=(int32_t)(vy-L(0x34));
    SL(0x34,L(0x34)+(uint32_t)(ey>>4));SL(0x30,L(0x30)+(uint32_t)(ex>>3));
}
/* 049B20: the bank +96 (less its offset 3; the caller adds it back) one step
 * towards the one of the speed +30 (0..8), held by the bank of the bombing
 * angle +80 (E09F4) unless +9B; the carry of the last compare or add decides
 * the way down, as the original's flags; facing left from 5. */
static void lone_bank(neo_gactor_t *a,neo_genv_t *e) {
    SB(0x96,B(0x96)-3u);
    int32_t v=(int32_t)L(0x30);
    uint8_t d1=v<(int32_t)0xffff0000?0:v<(int32_t)0xffff4000?1:v<(int32_t)0xffff8000?2:v<(int32_t)0xffffc000?3:v<0x4000?4:v<0x8000?5:v<0xc000?6:v<0x10000?7:8;
    uint8_t b=B(0x96);bool carry;
    if (d1>b) {
        bool up=true;carry=false;
        if (!B(0x9b)) {
            uint8_t d0=(uint8_t)(rbx(e,0xe09f4u+(W(0x80)>>4))-3u);
            if (S8(d0)<4 && S8(d0)>=S8(b)) {up=false;carry=d0<b;}
        }
        if (up) {carry=b==0xffu;SB(0x96,b+1u);}
    } else carry=d1<b;
    if (carry) {
        bool down=true;
        if (!B(0x9b)) {
            uint8_t d0=(uint8_t)(rbx(e,0xe09f4u+(W(0x80)>>4))-3u);
            if (S8(d0)>4 && S8(d0)<=S8(B(0x96))) down=false;
        }
        if (down) SB(0x96,B(0x96)-1u);
    }
    if (S8(B(0x96))>=5) SET(0x15,0); else CLR(0x15,0);
}
/* 04900C: hurtbox, the death 048B06, the cull 049166 (the cartridge check
 * before 004188 is not reproduced). */
static ga_result_t lone_tail(neo_gactor_t *a,neo_genv_t *e) {
    if (ga_register(a,e,NULL,NULL)) SB(0x5a,0);
    if (BIT(0x5f,7)) SL(0,0x48b06);
    if (ga_cull(a,e,0xe0aa2)) {SB(0x61,0xff);return e->unsupported?GA_UNSUPPORTED:GA_DELETE;}
    return e->unsupported?GA_UNSUPPORTED:GA_MORE;
}
static ga_result_t lone_tick(neo_gactor_t *a,neo_genv_t *e) {
    bool done=false;neo_gactor_t *c;uint32_t node;
    for (unsigned guard=0;guard<8;guard++) switch (L(0)) {
    case 0x474b6:
        ga_project(a,e);
        SW(0x86,(W(0x72)<<5)&0x7fffu);                                  /* 049EBE */
        SB(0x1d,0);                                                     /* 009F88 */
        SW(0x66,0x8600);SW(0x1e,0x8600);SL(0x50,0xe0ab2);
        ga_palette(a,0x20);ga_flash_palette(a,0x15c);
        SW(0x4e,rank(a,e,0x2d146c));SW(0x76,rank(a,e,0x2d1a16));
        SB(0x61,0);SB(0x60,0);SB(0x96,0);
        if (S16(W(0x3c))<0xa0) SET(0x15,0);
        c=kid(a,e,0x47874,0x1c);
        if (c) ga_sb(c,0x60,0);
        START(0x244712);SL(0,0x4755a);
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    case 0x4755a:
        sound(e,0x10e8);SL(0,0x4756a);
        /* fall through */
    case 0x4756a: {
        if (!lone_altitude(a,e)) return GA_UNSUPPORTED;                 /* 049324 */
        SW(0x78,0xa0);
        if (!lone_nearest(a,e,&node)) SW(0x78,node_x(e,node));
        SL(0x7c,node);
        steer(a);bank_by_speed(a);ga_air(a,e);STEP(done);
        if (near_target(a,true,0x10)) SL(0,0x47598);
        return lone_tail(a,e);}
    case 0x47598: {
        uint32_t t=0xe0c32u+4u*(B(0x96)&3u);
        SL(0x30,0);SL(0x34,0);SW(0x74,0);SW(0x82,0);
        START((uint32_t)rwx(e,t)<<16|rwx(e,t+2u));SL(0,0x475ce);}
        /* fall through */
    case 0x475ce:
        lone_hover(a);ga_air(a,e);STEP(done);
        if (done) SL(0,0x475ee);
        return lone_tail(a,e);
    case 0x475ee:
        SW(0x84,rank(a,e,0x2d1592));
        lone_any(e,&node);SL(0x7c,node);
        /* fall through */
    case 0x47608:
        CLR(0x15,0);SB(0x9b,0);SB(0xa8,0);SW(0x90,0);
        START(0x24473e);SL(0,0x47630);
        /* fall through */
    case 0x47630:
        lone_hover(a);lone_target(a,e);lone_fly(a,e,0x10);ga_air(a,e);lone_bank(a,e);STEP(done);
        if (near_target(a,false,2)) SL(0,0x4768e);
        if (!W(0x84)) SL(0,0x4768e); else SW(0x84,W(0x84)-1u);
        if (lone_leave(a)) SL(0,0x4783a);
        SB(0x96,B(0x96)+3u);
        return lone_tail(a,e);
    case 0x4768e:
        SL(0x30,0);SL(0x34,0);SL(0,0x4769c);
        /* fall through */
    case 0x4769c:
        lone_hover(a);lone_target(a,e);ga_air(a,e);lone_bank(a,e);STEP(done);
        SB(0x96,B(0x96)+3u);
        if (!lone_any(e,&node)) {
            if (B(0x9b)) {SB(0x9b,0);SL(0,0x47608);}
            else if (!W(0x84)) {if (!L(0x30)) SL(0,0x4771a);}
            else {
                if (!near_target(a,false,0xe)) SL(0,0x47608);
                SW(0x84,W(0x84)-1u);
            }
        }
        if (lone_leave(a)) SL(0,0x4783a);
        return lone_tail(a,e);
    case 0x4771a:
        SL(0,0x47720);
        /* fall through */
    case 0x47720: {
        lone_hover(a);
        uint8_t d1=rbx(e,0xe09f4u+(W(0x80)>>4));
        if (d1==B(0x96)) SL(0,0x4778e);
        else if (S8(d1)<=S8(B(0x96))) SB(0x96,B(0x96)-1u);
        else SB(0x96,B(0x96)+1u);
        goto facing;}
    case 0x4778e:
        SB(0x98,rank(a,e,0x2d177c));SB(0x97,rank(a,e,0x2d18a2));SB(0x94,B(0x97));SB(0x60,0);
        SL(0,0x477be);
        /* fall through */
    case 0x477be:
        lone_hover(a);
        if (B(0x94)) SB(0x94,B(0x94)-1u);
        else if (B(0x98)) {
            SB(0x98,B(0x98)-1u);SB(0x94,B(0x97));
            c=kid(a,e,0x479b8,0x2a);
            if (c) ga_sb(c,0x60,0);
        } else SL(0,0x475ee);
    facing:
        if (S8(B(0x96))>7) SET(0x15,0); else CLR(0x15,0);
        SB(0x96,B(0x96)-3u);
        ga_air(a,e);STEP(done);
        SB(0x96,B(0x96)+3u);
        return lone_tail(a,e);
    case 0x4783a:
        SW(0x7a,0x2ff);
        START(0x24473e);SL(0,0x47856);
        /* fall through */
    case 0x47856:
        lone_steer(a);ga_air(a,e);lone_bank(a,e);STEP(done);
        SB(0x96,B(0x96)+3u);
        return lone_tail(a,e);
    default: ga_fail(e,L(0));return GA_UNSUPPORTED;
    }
    ga_fail(e,L(0));return GA_UNSUPPORTED;
}
static bool lone(uint32_t r) {
    return r==0x474b6 || r==0x4755a || r==0x4756a || r==0x47598 || r==0x475ce || r==0x475ee || r==0x47608 || r==0x47630 || r==0x4768e || r==0x4769c ||
           r==0x4771a || r==0x47720 || r==0x4778e || r==0x477be || r==0x4783a || r==0x47856;
}
/* ---- Mission 4 helicopters 08514A..08604A (families m4heli / m4heli2 / m4heli3) ----
 * Manual translation of ROM 08514A..086062.
 * 08571C (script; +6E target x, +70 fires, +72 scale, AF when 0): a scaled
 *   helicopter in the background, layers 0, no hurtbox; speed rank 2D5320 <<
 *   8 by its facing (it faces right when born left of 0xA0); when it passes
 *   +72 (08579A..0857D0) it waits rank 2D533A updates (0857DE, with the flash
 *   085952 when +85 bit 0) and turns round the target (FF80 / 1C0). Cull
 *   0E4DA4 (01945C) -> 0858C4.
 * 085952: the flash on its creator (01ACA2, the creator's scale, offset 10 /
 *   18 scaled), program 28292C with +86 = (wait - 8) / 2.
 * 08514A (script; +6E path, +70 count, +72 points): the squadron: every
 *   3000 / rank 2D50A6 updates one helicopter 085294 (bit +8F in its +8C
 *   while alive, +8E while not shot down), every rank 2D5024 updates one of
 *   the live ones is told to fire (+8D, 0853E2); gone when all are.
 * 085294: path 0E4D04 / 0E4D10 [+8A] (021288), health rank 2D4FF0, hurtbox
 *   0E4E20; told to fire inside 10..130 it makes the flash 0859E4 for the
 *   nearest player; shot down -> 004192, 08537A (the points 08D1D0 with
 *   +84 when the whole squadron was shot down) and 085880.
 * 085522 (script; +6E / +70 its item, +72 the camera where it leaves): health
 *   rank 2D5132, +7E bombs rank 2D51CE (0: FFFF), speed limits 2D514C[rank
 *   2D515C], bomb speed rank 2D529E; 0855EA: after rank 2D521C updates and
 *   with no player inside 0E4DB4 (01B300) it stops inside the screen and
 *   makes the flash 0859E4 (085670 until the flash ends, +60); else it
 *   steers (085D2E: heading 085EC6 by the scores 085FB8 / 085FC1, speed
 *   085FE4 by the distance, 1/16 per update, +-8 0000) with 0143E4. It
 *   leaves (0856CA: 03B332) when the camera passes +80 or the bombs run out.
 * 0859E4: the flash on its creator; its program's call 085DEE makes the
 *   bomb 085ACE after the script actor with the heading to the target / 16;
 *   when the program ends the creator's +60 = FF.
 * 085ACE: the bomb: polar speed +82 << 8 by +88 << 4, 0143E4, attack row
 *   0E4DBC[+89] -> 085BBE (01C706), the ground (+15 bit 7) -> 085BEA
 *   (282AEA, palette D5), out of 0E4DAC (019504) -> 004188.
 * 085880: 004192, sound 102C, program 282834 (its call 08604A: the item
 *   +7A / +7C by 08CDB2 at (0, 8)), then 0858C4 -> 004188.
 * The players' +3C / +40 longs: the projected words and their low words
 * (RAM 101502 / 101506). */
static bool m4h_call(neo_gactor_t *a,neo_genv_t *e,neo_anim_kind_t kind,uint32_t routine,uint16_t parameter);
#define MSTEP(d) do { int r_=ga_anim_step(a,e,m4h_call); if (r_<0) return GA_UNSUPPORTED; (d)=r_!=0; } while (0)
enum { M4H_P1=0x1014c4, M4H_P2=0x101574 };
/* 0260C8 with its type 2 (a byte row, 16-byte sets, a random word of 8: 02617A). */
static uint16_t m4h_rank(neo_gactor_t *a,neo_genv_t *e,uint32_t table) {
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
    return rw(table+2u+(unsigned)t[2+d0]*16u+count*12u+(ga_rand(e)&0xeu));
}
static bool m4h_live(const neo_genv_t *e,uint32_t node) {   /* 01AEDA */
    if (node!=M4H_P1 && node!=M4H_P2) return false;
    return (e->players.active_mask>>(node==M4H_P2?1u:0u))&1u;
}
static uint16_t m4h_px(const neo_genv_t *e,uint32_t node) {return node==M4H_P2?e->players.p2x:e->players.p1x;}
static uint16_t m4h_py(const neo_genv_t *e,uint32_t node) {return node==M4H_P2?e->players.p2y:e->players.p1y;}
static uint32_t m4h_pxl(const neo_genv_t *e,uint32_t node) {return (uint32_t)m4h_px(e,node)<<16|e->player_low[node==M4H_P2?1:0];}
static uint32_t m4h_pyl(const neo_genv_t *e,uint32_t node) {return (uint32_t)m4h_py(e,node)<<16|e->player_ylow[node==M4H_P2?1:0];}
static uint32_t m4h_nearest(neo_gactor_t *a,neo_genv_t *e) {   /* 01AF9A */
    unsigned m=e->players.active_mask&0xffu;
    if (!m || m==1u) return M4H_P1;
    if (m==2u) return M4H_P2;
    int16_t d1=(int16_t)(e->players.p1x-W(0x3c)),d2=(int16_t)(e->players.p2x-W(0x3c));
    if (d1<0) d1=(int16_t)-d1;
    if (d2<0) d2=(int16_t)-d2;
    return d2<d1?M4H_P2:M4H_P1;
}
/* 01B04A: a live player's node (a random one of two); carry (false) when none. */
static bool m4h_any(neo_genv_t *e,uint32_t *node) {
    unsigned d0=e->players.active_mask&0xffu;
    if (d0>=3u) d0=(ga_rand(e)&1u)+1u;
    *node=d0==2u?M4H_P2:M4H_P1;
    return d0!=0;
}
static neo_gactor_t *m4h_kid(neo_gactor_t *a,neo_genv_t *e,uint32_t r) {   /* 0040BC, D0 = E */
    return e->spawn_reserve?e->spawn_reserve(e->ctx,a,r,0x0e):e->spawn?e->spawn(e->ctx,a,r,false):NULL;
}
static bool m4h_look(neo_gactor_t *a,neo_genv_t *e) {   /* 085CC0 */
    if (!ga_anim_start(a,e,0x2825ce,0x100)) return false;
    ga_palette(a,B(0x12)==0xffu?0xd2:0x88f);ga_palette(a,0xd6);ga_palette(a,0xd4);
    SB(0x14,B(0x19));
    return true;
}
static ga_result_t m4h_tail(neo_gactor_t *a,neo_genv_t *e) {   /* 085872 */
    if (B(0x87)) SB(0x87,B(0x87)-1u);
    return e->unsupported?GA_UNSUPPORTED:GA_MORE;
}
static ga_result_t m4h_cull(neo_gactor_t *a,neo_genv_t *e) {   /* 085858 */
    if (ga_cull(a,e,0xe4da4)) SL(0,0x858c4);
    return m4h_tail(a,e);
}
static ga_result_t m4h_hit(neo_gactor_t *a,neo_genv_t *e) {   /* 08568C */
    if (ga_register(a,e,NULL,NULL)) {
        SB(0x87,B(0x87)|6u);
        if (BIT(0x5f,7)) SL(0,0x85880);
        SB(0x5a,0);
    }
    return m4h_cull(a,e);
}
/* 0854BC: the flash 0859E4 for the target +6E; carry. */
static bool m4h_flash(neo_gactor_t *a,neo_genv_t *e) {
    neo_gactor_t *c=m4h_kid(a,e,0x859e4);
    if (!c) return false;
    ga_place_from(c,a,e,0,0);ga_sl(c,0x6e,L(0x6e));ga_sw(c,0x82,W(0x82));
    return true;
}
/* 085E60: keeps a live target, else a new one (01B04A); carry. */
static bool m4h_target(neo_gactor_t *a,neo_genv_t *e) {
    if (L(0x6e) && m4h_live(e,L(0x6e))) return true;
    uint32_t n;bool ok=m4h_any(e,&n);
    SL(0x6e,n);return ok;
}
static void m4h_85fd2(neo_gactor_t *a,neo_genv_t *e) {SB(0x8b,(uint8_t)((ga_rand(e)&3u)-1u));}
static uint8_t m4h_aim(neo_gactor_t *a,const neo_genv_t *e,uint32_t n) {   /* 002EB4 to the target, y + 10 */
    return neo_angle((int16_t)(uint16_t)(m4h_px(e,n)-W(0x3c)),(int16_t)(uint16_t)(m4h_py(e,n)+0x10u-W(0x40)));
}
/* 085EC6: the heading +88 (<< 4) of the best score among the allowed sixteenths. */
static void m4h_heading(neo_gactor_t *a,neo_genv_t *e) {
    uint16_t d6=0x80ff;uint32_t n=L(0x6e);
    int16_t tx=S16(m4h_px(e,n)),x=S16(W(0x3c)),y=S16(W(0x40));
    if (tx>0xf0) d6&=0xf007u;
    if (tx<0x50) d6&=0x07f0u;
    if (x>0x110) d6&=0xf007u;
    if (x<0x30) d6&=0x07f0u;
    if (y>0x1d0) d6&=0xc1c1u;
    if (y<0x130) d6&=0xc1c1u;
    if (B(0x87)) d6&=0xe3e3u;
    if (!d6) d6=0x80ff;
    uint8_t d5=m4h_aim(a,e,n),d0=0;int16_t d7=0;
    while (d6) {
        bool carry=(d6&0x8000u)!=0;d6=(uint16_t)(d6<<1);
        if (carry) {
            uint8_t d2=(uint8_t)((uint8_t)(d0<<4)-d5);                 /* 085F84 */
            if (d2&0x80u) d2=(uint8_t)(0u-d2);
            d2=(uint8_t)((uint8_t)(d2+8u)>>4)&0xfu;
            int16_t d1=(int16_t)rb(0x85fb8u+d2);
            d2=(uint8_t)(d0-B(0x8a)+B(0x8b))&0xfu;
            d1=(int16_t)(d1+rb(0x85fc1u+d2));
            if (!(d1<d7)) {SB(0x88,d0);d7=d1;}
        }
        d0=(uint8_t)(d0+1u);
    }
    SB(0x88,(uint8_t)(B(0x88)<<4));
}
/* 085FE4: the speed wanted towards the target (D0 / D1). */
static void m4h_speed(neo_gactor_t *a,neo_genv_t *e,uint32_t *vx,uint32_t *vy) {
    uint32_t n=L(0x6e);
    uint32_t dx=m4h_pxl(e,n)-L(0x3c),dy=m4h_pyl(e,n)+0x100000u-L(0x40);
    int16_t wx=(int16_t)(uint16_t)(dx>>16),wy=(int16_t)(uint16_t)(dy>>16);
    uint16_t ax=(uint16_t)(wx<0?-wx:wx),ay=(uint16_t)(wy<0?-wy:wy);      /* 002F1A */
    uint16_t d0=ay>ax?(uint16_t)((ax>>1)+ay):(uint16_t)(ax+(ay>>1));
    d0&=0xfff0u;
    if (d0>W(0x76)) d0=W(0x76);
    if (d0<W(0x78)) d0=W(0x78);
    uint32_t sp=(uint32_t)((int32_t)((uint32_t)d0<<16)>>8),px,py;
    ga_polar(sp,B(0x88),&px,&py);                                        /* 002E7C */
    px<<=8;py<<=8;
    *vx=dx-px;*vy=dy-py;
}
/* 085D2E: steer. */
static void m4h_steer(neo_gactor_t *a,neo_genv_t *e) {
    uint32_t d0=0,d1=0;
    if (m4h_live(e,L(0x6e))) {m4h_heading(a,e);m4h_speed(a,e,&d0,&d1);}
    d0=(uint32_t)((int32_t)d0>>3);d1=(uint32_t)((int32_t)d1>>3);
    d0-=L(0x30);d1-=L(0x34);
    d0=(uint32_t)((int32_t)d0>>4);d1=(uint32_t)((int32_t)d1>>4);
    SL(0x30,L(0x30)+d0);SL(0x34,L(0x34)+d1);
    if ((int32_t)L(0x30)>0x80000) SL(0x30,0x80000);
    if ((int32_t)L(0x30)<-0x80000) SL(0x30,0xfff80000u);
    if ((int32_t)L(0x34)>0x80000) SL(0x34,0x80000);
    if ((int32_t)L(0x34)<-0x80000) SL(0x34,0xfff80000u);
    ga_air(a,e);                                                         /* 0143E4 */
    if (!(ga_rand(e)&0x1fu)) m4h_85fd2(a,e);
}
/* 01B300 with the rectangle `rect`: a live player inside (01AF3C, both edges). */
static bool m4h_inside(neo_gactor_t *a,neo_genv_t *e,uint32_t rect) {
    for (unsigned p=0;p<2;p++) {
        if (!((e->players.active_mask>>p)&1u)) continue;
        uint32_t n=p?M4H_P2:M4H_P1;
        int16_t d1=(int16_t)(uint16_t)(m4h_py(e,n)-W(0x40)),d2=S16(rw(rect+2)),d3=(int16_t)(uint16_t)((uint16_t)d2+rw(rect+6));
        if (BIT(0x15,1)) d1=(int16_t)-d1;
        if (d1<d2 || d1>d3) continue;
        d1=(int16_t)(uint16_t)(m4h_px(e,n)-W(0x3c));d2=S16(rw(rect));d3=(int16_t)(uint16_t)((uint16_t)d2+rw(rect+4));
        if (FACING) d1=(int16_t)-d1;
        if (d1<d2 || d1>d3) continue;
        return true;
    }
    return false;
}
/* 08537A: the helicopter leaves its squadron. */
static void m4h_leave(neo_gactor_t *a,neo_genv_t *e) {
    uint8_t d0=B(0x8f);
    if (d0&0x80u) return;
    neo_gactor_t *p=creator(a,e);
    if (!p) {ga_fail(e,0x85382);return;}
    ga_sb(p,0x8c,ga_b(p,0x8c)&(uint8_t)~(1u<<(d0&7u)));
    if (BIT(0x5f,7)) {
        ga_sb(p,0x8e,ga_b(p,0x8e)&(uint8_t)~(1u<<(d0&7u)));
        if (!ga_b(p,0x8e) && B(0x84)) {                                  /* 0854F8 -> 08D1D0 */
            ga_project(a,e);
            uint8_t pl=(uint8_t)((B(0x5e)>>2)&3u);uint16_t d3=B(0x84);
            if (pl) {
                if (e->pickup && pl<=2u) neo_item_add_score(e->pickup->score[pl-1u],rl(0xe5582u+d3*4u));   /* 057948 */
                sound(e,0x1121);
                neo_gactor_t *c=after_script(a,e,0x8d0d2,0,0);
                if (c) {ga_sb(c,0x87,0);ga_sb(c,0x71,(uint8_t)((pl-1u)&1u));ga_sb(c,0x70,(uint8_t)d3);}
            }
        }
    }
    SB(0x8f,0xff);
}
static ga_result_t m4h_bg_fly(neo_gactor_t *a,neo_genv_t *e) {
    bool d;
    uint16_t d0=m4h_rank(a,e,0x2d5320);                                      /* 085788 */
    if (e->unsupported) return GA_UNSUPPORTED;
    if (!FACING) d0=(uint16_t)(0u-d0);
    SL(0x30,(uint32_t)(int32_t)S16(d0)<<8);SL(0x34,0);SL(0,0x857b2);
    ga_move(a,e);MSTEP(d);(void)d;                                       /* 0857B2 */
    if ((S16(W(0x72))>S16(W(0x3c)))!=(FACING!=0)) SL(0,0x857de);
    return m4h_cull(a,e);
}
static ga_result_t m4h_death(neo_gactor_t *a,neo_genv_t *e) {            /* 085880 */
    bool d;
    if (L(0)==0x85880) {
        if (e->kill_children) e->kill_children(e->ctx,a);               /* 004192 */
        sound(e,0x102c);START(0x282834);SB(0x14,B(0x1a));SL(0,0x858ac);
    }
    ga_project(a,e);MSTEP(d);
    if (d) SL(0,0x858c4);
    return e->unsupported?GA_UNSUPPORTED:GA_MORE;
}
static ga_result_t m4h_tick(neo_gactor_t *a,neo_genv_t *e) {
    bool d;neo_gactor_t *c,*p;
    switch (L(0)) {
    case 0x8571c: {
        uint16_t d0=W(0x6e),d1=W(0x70),d2=W(0x72);
        SW(0x6e,0);SW(0x70,0);SW(0x72,d0);
        if (d1) SET(0x85,0);
        uint8_t s=(uint8_t)d2;if (!s) s=0xaf;
        SB(0x12,s);SB(0x13,s);
        if (S16(W(0x3c))<0xa0) SET(0x15,0);
        if (!m4h_look(a,e)) return GA_UNSUPPORTED;
        SW(0x66,0);SW(0x1e,0);
        /* 012040: RAM 10B954 against 10B95C (unsigned; the port keeps the signed test of 012022: both are slot numbers below 0x180). */
        if (e->front_layer && e->front_layer(e->ctx)) CLR(0x15,2); else SET(0x15,2);
        SL(0x50,0);
        return m4h_bg_fly(a,e);
    }
    case 0x857b2:
        ga_move(a,e);MSTEP(d);
        if ((S16(W(0x72))>S16(W(0x3c)))!=(FACING!=0)) SL(0,0x857de);
        return m4h_cull(a,e);
    case 0x857de: {
        SW(0x74,m4h_rank(a,e,0x2d533a));
        if (e->unsupported) return GA_UNSUPPORTED;
        if (BIT(0x85,0)) {
            /* a failed 0040BC leaves the dummy node 101624 in A0: the write is lost */
            int16_t v=(int16_t)(uint16_t)(W(0x74)-8u);
            if ((c=m4h_kid(a,e,0x85952))) ga_sb(c,0x86,(uint8_t)((uint16_t)(v<0?0:v)>>1));
        }
        SL(0,0x85820);
    }
        /* fall through */
    case 0x85820: {
        uint16_t t=(uint16_t)(W(0x74)-1u);SW(0x74,t);
        if (t&0x8000u) {SW(0x72,FACING?0x1c0:0xff80);return m4h_bg_fly(a,e);}
        ga_project(a,e);MSTEP(d);
        return m4h_cull(a,e);
    }
    case 0x85880: case 0x858ac: return m4h_death(a,e);
    case 0x858c4: case 0x85354:
        if (L(0)==0x85354) m4h_leave(a,e);
        return e->unsupported?GA_UNSUPPORTED:GA_DELETE;
    case 0x85952: {
        START(0x28292c);ga_palette(a,0xfe);SW(0x66,0);SW(0x1e,0);
        if (!(p=creator(a,e))) {ga_fail(e,0x8597c);return GA_UNSUPPORTED;}
        SB(0x15,ga_b(p,0x15)&3u);                                        /* 01ACA2 */
        SB(0x12,ga_b(p,0x12));SB(0x13,ga_b(p,0x13));
        int16_t d0=0x10,d1=0x18;
        if (!FACING) d0=(int16_t)-d0;
        d0=(int16_t)(((int32_t)d0*(int32_t)B(0x12))/0xff);d1=(int16_t)(((int32_t)d1*(int32_t)B(0x13))/0xff);
        ga_place(a,e,d0,d1,NEO_GA_TAG(p->index));                        /* 01AD6E */
        SB(0x12,0xff);SB(0x13,0xff);SL(0,0x859cc);
    }
        /* fall through */
    case 0x859cc:
        ga_project(a,e);MSTEP(d);
        if (d) return GA_DELETE;
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    case 0x8514a: {
        uint16_t d0=W(0x6e),d1=W(0x70),d2=W(0x72);
        SW(0x6e,0);SW(0x70,0);SW(0x72,0);
        SB(0x8a,(uint8_t)d0);
        d1=(uint16_t)(d1-1u);if (d1&0x8000u) d1=0;
        SB(0x8f,(uint8_t)d1);SB(0x84,(uint8_t)d2);
        SW(0xa8,m4h_rank(a,e,0x2d50a6));SW(0x82,m4h_rank(a,e,0x2d50c0));
        if (S16(W(0x3c))<0xa0) SET(0x15,0);
        SW(0x74,0);SW(0x72,m4h_rank(a,e,0x2d500a));                          /* 0853AE */
        if (e->unsupported) return GA_UNSUPPORTED;
        unsigned sh=(unsigned)(uint8_t)(B(0x8f)+1u)&63u;
        SB(0x8e,(uint8_t)((sh<16u?(uint16_t)(1u<<sh):0u)-1u));
        SL(0,0x851c6);
    }
        /* fall through */
    case 0x851c6: {
        ga_project(a,e);
        if ((B(0x8f)&0x80u) && !B(0x8c)) return GA_DELETE;               /* 004188 */
        uint16_t t=(uint16_t)(W(0x74)-1u);SW(0x74,t);
        if (t&0x8000u) {
            SW(0x74,0);
            if (!(B(0x8f)&0x80u) && (c=m4h_kid(a,e,0x85294))) {          /* 08541E */
                ga_place_from(c,a,e,0,0);
                ga_sw(c,0x7a,0xffff);ga_sb(c,0x8a,B(0x8a));ga_sw(c,0xa8,W(0xa8));ga_sw(c,0x82,W(0x82));ga_sb(c,0x8f,B(0x8f));ga_sb(c,0x84,B(0x84));
                SB(0x8f,B(0x8f)-1u);
                if (!W(0xa8)) {ga_fail(e,0x853d8);return GA_UNSUPPORTED;}   /* divu by 0 */
                SW(0x74,(uint16_t)(0x3000u/W(0xa8)));                    /* 0853D2 */
            }
        }
        t=(uint16_t)(W(0x72)-1u);SW(0x72,t);
        if (t&0x8000u) {
            SW(0x72,m4h_rank(a,e,0x2d5024));                                 /* 0853C0 */
            uint8_t d1=B(0x8c);                                          /* 0853E2 */
            if (d1) {
                uint16_t n=0;for (unsigned i=0;i<8;i++) n=(uint16_t)(n+((d1>>i)&1u));
                uint8_t d0=(uint8_t)ga_rand_range(e,n);
                for (unsigned d2=7;;d2=(d2-1u)&0xffu) {
                    if (d2<8u && ((d1>>d2)&1u)) {
                        d0=(uint8_t)(d0-1u);
                        if (d0&0x80u) {SB(0x8d,B(0x8d)|(uint8_t)(1u<<d2));break;}
                    }
                    if (d2>=8u) {ga_fail(e,0x8541a);return GA_UNSUPPORTED;}
                }
            }
        }
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    }
    case 0x85294: {
        if (!(p=creator(a,e))) {ga_fail(e,0x8536c);return GA_UNSUPPORTED;}
        ga_sb(p,0x8c,ga_b(p,0x8c)|(uint8_t)(1u<<(B(0x8f)&7u)));          /* 08536C */
        if (!m4h_look(a,e)) return GA_UNSUPPORTED;
        SW(0x66,0x8800);SW(0x1e,0x8800);
        SW(0x4e,m4h_rank(a,e,0x2d4ff0));SL(0x50,0xe4e20);
        SL(0xa0,rl((FACING?0xe4d10u:0xe4d04u)+4u*B(0x8a)));SW(0xa4,0);SB(0xa6,0);
        if (e->unsupported) return GA_UNSUPPORTED;
        SL(0,0x85306);
    }
        /* fall through */
    case 0x85306: {
        (void)path(a,e);                                                 /* 021288 */
        if (e->unsupported) return GA_UNSUPPORTED;
        MSTEP(d);
        if (S16(W(0x3c))>=0x10 && S16(W(0x3c))<=0x130 && !(B(0x8f)&0x80u)) {   /* 085484 */
            if (!(p=creator(a,e))) {ga_fail(e,0x85498);return GA_UNSUPPORTED;}
            uint8_t bit=(uint8_t)(1u<<(B(0x8f)&7u));
            if (ga_b(p,0x8d)&bit) {
                ga_sb(p,0x8d,ga_b(p,0x8d)&(uint8_t)~bit);
                SL(0x6e,m4h_nearest(a,e));
                (void)m4h_flash(a,e);
            }
        }
        if (ga_register(a,e,NULL,NULL)) {
            SB(0x87,B(0x87)|6u);
            if (BIT(0x5f,7)) SL(0,0x8535e);
            SB(0x5a,0);
        }
        if (ga_cull(a,e,0xe4da4)) SL(0,0x85354);
        return m4h_tail(a,e);
    }
    case 0x8535e:
        if (e->kill_children) e->kill_children(e->ctx,a);               /* 004192 */
        m4h_leave(a,e);
        if (e->unsupported) return GA_UNSUPPORTED;
        SL(0,0x85880);
        return m4h_death(a,e);
    case 0x85522: {
        uint16_t d0=W(0x6e),d1=W(0x70),d2=W(0x72);
        SW(0x6e,0);SW(0x70,0);SW(0x72,0);
        SW(0x7a,d0);SW(0x7c,d1);SW(0x80,d2);
        if (!m4h_look(a,e)) return GA_UNSUPPORTED;
        SW(0x66,0x8800);SW(0x1e,0x8800);SET(0x15,5);
        SW(0x4e,m4h_rank(a,e,0x2d5132));SL(0x50,0xe4e20);
        SW(0x7e,m4h_rank(a,e,0x2d51ce));if (!W(0x7e)) SW(0x7e,0xffff);
        uint16_t k=(uint16_t)(m4h_rank(a,e,0x2d515c)*4u);
        SW(0x76,rw(0x2d514cu+k));SW(0x78,rw(0x2d514cu+k+2u));
        SW(0x82,m4h_rank(a,e,0x2d529e));
        if (e->unsupported) return GA_UNSUPPORTED;
        m4h_85fd2(a,e);
        (void)m4h_target(a,e);                                           /* 085E94 */
        uint8_t ang=m4h_aim(a,e,L(0x6e));
        if (!(ang&0x80u)) ang=(uint8_t)(0u-ang);
        SB(0x8a,(uint8_t)(ang>>4));
    }
        /* fall through */
    case 0x855ea: case 0x85670: {
        bool again=L(0)==0x85522;
        if (L(0)==0x85670) {
            if (B(0x60)) again=true;
            else {ga_stationary(a,e);MSTEP(d);return m4h_hit(a,e);}      /* 01436A */
        }
        for (unsigned guard=0;guard<8;guard++) {
            if (again) {                                                 /* 0855CC */
                if (!W(0x7e)) goto leave;
                SW(0x74,m4h_rank(a,e,0x2d521c));SL(0,0x855ea);
                if (e->unsupported) return GA_UNSUPPORTED;
            }
            again=true;
            bool attack=false;
            if (W(0x74)) SW(0x74,W(0x74)-1u);
            else attack=m4h_live(e,L(0x6e)) && !m4h_inside(a,e,0xe4db4);  /* 085DCE */
            if (!attack) {
                if (W(0x80) && W(0x80)<e->camera) goto leave;            /* 085E52 */
                (void)m4h_target(a,e);m4h_steer(a,e);MSTEP(d);
                return m4h_hit(a,e);
            }
            if (S16(W(0x3c))<=0x10 || S16(W(0x3c))>=0x130 || S16(W(0x40))<=0x110 || S16(W(0x40))>=0x1f0) continue;
            if (!m4h_target(a,e)) continue;
            bool made=m4h_flash(a,e);
            SB(0x60,made?0:0xff);
            if (!made) continue;
            if (S16(W(0x7e))>0) SW(0x7e,W(0x7e)-1u);
            SL(0x30,0);SL(0x34,0);SL(0x62,0);SL(0,0x85670);             /* 01B1DC */
            if (B(0x60)) continue;
            ga_stationary(a,e);MSTEP(d);
            return m4h_hit(a,e);
        }
        ga_fail(e,0x85688);return GA_UNSUPPORTED;
    leave: {
        to_root(a,e);                                                    /* 0856CA: 01AD52 */
        int16_t x=(int16_t)0xffc0;
        if (!((uint8_t)(B(0x88)-0x40u)&0x80u)) x=0x40;
        uint32_t g=0x6000,q,vx,vy;uint16_t n=0x28;                       /* 03B332 */
        q=0x10000u/n;
        vx=(uint32_t)((int32_t)x*(int16_t)(uint16_t)q);vy=(uint32_t)((int32_t)0x50*(int16_t)(uint16_t)q);
        uint32_t t=(g&0xffffu)*n+(((g>>16)*n)<<16);
        t=(uint32_t)((int32_t)t>>1);
        vy-=t;
        SL(0x30,L(0x30)+vx);SL(0x34,L(0x34)+vy);SL(0,0x85704);
    }
    }
        /* fall through */
    case 0x85704:
        SL(0x34,L(0x34)+0x6000u);ga_move(a,e);MSTEP(d);
        return m4h_hit(a,e);
    case 0x859e4:
        START(0x282998);ga_palette(a,0x88e);
        on_creator(a,e);                                                 /* 01AD6E (0, 0) */
        sound(e,0x10d1);SL(0,0x85a1c);
        /* fall through */
    case 0x85a1c:
        if (!(p=creator(a,e))) {ga_fail(e,0x85a1c);return GA_UNSUPPORTED;}
        SW(0x1e,ga_w(p,0x1e));
        ga_project(a,e);MSTEP(d);
        if (d) {ga_sb(p,0x60,0xff);return GA_DELETE;}
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    case 0x85ace: {
        START(0x282a2c);ga_palette(a,0x88e);SW(0x66,0xe0ef);SW(0x1e,0xe0ef);SET(0x15,5);
        uint8_t d0=(uint8_t)(B(0x88)<<4);
        d0=(uint8_t)(d0-0x40u);
        if (d0&0x80u) {d0=(uint8_t)(0u-d0);SET(0x15,0);}
        d0=(uint8_t)(d0-0x40u);
        if (d0&0x80u) {d0=(uint8_t)(0u-d0);SET(0x15,1);}
        d0>>=4;
        if (S8(d0)>4) {ga_fail(e,0x85b2e);return GA_UNSUPPORTED;}        /* trap #15 */
        SB(0x89,d0);
        uint32_t vx,vy;ga_polar((uint32_t)(int32_t)S16(W(0x82))<<8,(uint8_t)(B(0x88)<<4),&vx,&vy);   /* 002E7C */
        SL(0x30,vx);SL(0x34,vy);SL(0,0x85b62);
    }
        /* fall through */
    case 0x85b62:
        ga_air(a,e);MSTEP(d);
        if (BIT(0x15,7)) SL(0,0x85bea);
        if (ga_attack(a,e,rl(0xe4dbcu+4u*B(0x89)))) SL(0,0x85bbe);
        if (ga_cull_rect(a,e,0xe4dac)) return GA_DELETE;
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    case 0x85bbe:
        to_root(a,e);SB(0x1d,0);SW(0x66,0x1001);SW(0x1e,0x1001);SL(0x50,0);SB(0x17,0xff);
        SL(0,0x1c706);
        return neo_effects_tick(a,e);                                    /* jmp 01C706 */
    case 0x85bea:
        to_root(a,e);START(0x282aea);ga_palette(a,0xd5);SW(0x66,0x1001);SW(0x1e,0x1001);SL(0,0x85c20);
        /* fall through */
    case 0x85c20:
        ga_project(a,e);MSTEP(d);
        if (d) return GA_DELETE;
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    }
    ga_fail(e,L(0));return GA_UNSUPPORTED;
}
static bool m4h_call(neo_gactor_t *a,neo_genv_t *e,neo_anim_kind_t kind,uint32_t routine,uint16_t parameter) {
    (void)parameter;
    if (kind!=NEO_ANIM_CALL) return false;
    switch (routine) {
    case 0x85d1e:SB(0x86,B(0x87)?1:0);return true;
    case 0x85dee: {                                                      /* the bomb after the script actor */
        neo_gactor_t *c=after_script(a,e,0x85ace,0,0);
        if (!c) return true;                                           /* the dummy node 101624 */
        ga_sw(c,0x82,W(0x82));
        uint8_t d0=0x80;
        if (m4h_live(e,L(0x6e))) d0=m4h_aim(a,e,L(0x6e));
        ga_sb(c,0x88,(uint8_t)((uint8_t)(d0+8u)>>4));
        return true;
    }
    case 0x8604a: {                                                      /* 08CDB2 (0, 8, +7A, +7C, 0) */
        if (W(0x7a)<0x5au) {
            neo_gactor_t *c=e->spawn?e->spawn(e->ctx,NULL,0x8c374,true):NULL;
            if (c) {ga_sb(c,0x8b,0);ga_sw(c,0x70,W(0x7c));ga_sw(c,0x6e,W(0x7a));ga_sw(c,0x72,0);ga_sb(c,0x7f,0);ga_place_at(c,a,e,0,8);}
        }
        return true;
    }
    default:return false;
    }
}
static bool m4h_routine(uint32_t r) {
    static const uint32_t k[]={0x8514a,0x851c6,0x85294,0x85306,0x85354,0x8535e,0x85522,0x855ea,0x85670,0x85704,0x8571c,0x857b2,0x857de,0x85820,
        0x85880,0x858ac,0x858c4,0x85952,0x859cc,0x859e4,0x85a1c,0x85ace,0x85b62,0x85bbe,0x85bea,0x85c20};
    for (unsigned i=0;i<sizeof k/sizeof k[0];i++) if (r==k[i]) return true;
    return false;
}
static bool body(uint32_t r) {
    static const uint32_t own[]={0x48882,0x4893e,0x48982,0x489be,0x489c8,0x48a06,0x48a2c,0x48ab6,0x48af6,0x48b06,0x48b10,
        0x48bbc,0x48c8c,0x48d5e,0x48e22,0x48eda,0x48efe,0x48f26,0x48f4e,0x48f8c};
    for (unsigned i=0;i<sizeof(own)/sizeof(own[0]);i++) if (own[i]==r) return true;
    return false;
}
static bool leader(uint32_t r) {return r==0x4870c || r==0x4871c || r==0x4876c || r==0x48780 || r==0x487d0 || r==0x48832;}
static bool bomb(uint32_t r) {return r==0x47afc || r==0x47b24 || r==0x47bec || r==0x47c48 || r==0x47c90 || r==0x48510 || r==0x48560;}
bool neo_heli_routine(uint32_t r) {
    return m4h_routine(r) || r==0x4857c || r==0x485ec || leader(r) || body(r) || lone(r) || r==0x47874 || r==0x478c4 || r==0x479b8 || r==0x47a6c || r==0x47ae0 || bomb(r) || r==0x47de0 || r==0x47e92;
}
ga_result_t neo_heli_tick(neo_gactor_t *a,neo_genv_t *e) {
    uint32_t r=L(0);
    if (m4h_routine(r)) return m4h_tick(a,e);
    if (r==0x4857c || r==0x485ec) return controller_tick(a,e);
    if (leader(r)) return leader_tick(a,e);
    if (body(r)) return body_tick(a,e);
    if (lone(r)) return lone_tick(a,e);
    if (r==0x47874 || r==0x478c4) return rotor_tick(a,e);
    if (r==0x479b8 || r==0x47a6c || r==0x47ae0) return dropper_tick(a,e);
    if (bomb(r)) return bomb_tick(a,e);
    return shell_tick(a,e);
}
