#include "neo_region.h"
#include "marco_ammo.h"
#if defined(VEHDBG)
#include <stdio.h>
#endif
/* Mission 3 vehicle 0AC866 (script record 2DB108, camera 5160; marker "IN"):
 * manual translation of ROM 0AC6D4..0AE96A and its own children, not a copy
 * of another vehicle (docs/PLAN_MISION3.md step 5). ROM labels are
 * provenance only. A flying seat: the stick moves +30/+34 (0AE2D2), B
 * thrusts (0ACB96), the C/fire button drops bombs in the air (0ADA16) or
 * throws the player's grenades on the ground (0C0808), Down lands, A+B or
 * a hit with no health ejects. The vulcan turret 03B4E0 (+72 = 1) is the
 * Camel Slug's (neo_slug.c), the marker 03523C / 03533A neo_slug.c, the
 * white screen 0AE1DC and the pieces 0AD9B8 neo_tank.c, the burst 0AD8A2
 * neo_slugnoid.c, the explosions 01C706 / 01C71E neo_effects.c.
 *
 * Fields: +4E health (0x30), +50 hurtbox 0AACA4, +6E rider (player 0/1),
 * +70 the rider's +6E (its character), +72 the seat's bob (long; its high
 * word is the seat's d3 of 0AA760), +76 shells, +78 the program table
 * 0AAF72 (by +7C), +7C the program index, +7E/+80 the stick's vertical /
 * horizontal state (0..0x500, rest 0x280), +82/+83 the pose's anchor (the
 * turret, the pilot and the pieces), +84 flicker byte, +86 the death palette
 * step, +88 the take-off thrust, +8C hit flicker timer, +8E knock-back
 * updates, +94 empty, +95 pose, +96 leaving, +97 bombs in flight allowed,
 * +98 fire cool-down, +99 down held on landing, +9A bob step, +9B dust in
 * flight, +9D ejected.
 *
 * Explicit stops (no capture holds them): the bomb's homing (+9C set by its
 * program, 0ADA7C) and the death palette index 0xF2 (0AE752: trap). */
#define B(o) ga_b(a,(o))
#define W(o) ga_w(a,(o))
#define L(o) ga_l(a,(o))
#define SB(o,v) ga_sb(a,(o),(uint8_t)(v))
#define SW(o,v) ga_sw(a,(o),(uint16_t)(v))
#define SL(o,v) ga_sl(a,(o),(uint32_t)(v))
#define BIT(o,b) ((B(o)>>(b))&1u)
#define START(t) do { if (!ga_anim_start(a,e,(t),0x100)) return GA_UNSUPPORTED; } while (0)
#define STOP(at) do { ga_fail(e,(at));return GA_UNSUPPORTED; } while (0)
#define STEP(r) do { if (((r)=ga_anim_step(a,e,call))<0) return GA_UNSUPPORTED; } while (0)
/* 0AE8B0 (the vehicle's programs): an engine flame 0AD3E2. */
static bool call(neo_gactor_t *a,neo_genv_t *e,neo_anim_kind_t kind,uint32_t routine,uint16_t parameter) {
    (void)parameter;
    if (kind!=NEO_ANIM_CALL || routine!=0xae8b0) return false;
    if (e->spawn) (void)e->spawn(e->ctx,a,0xad3e2,false);
    return true;
}
static uint16_t rw(uint32_t address) {const uint8_t *p=ga_rom(address);return p?(uint16_t)(p[0]<<8|p[1]):0;}
static uint32_t rl(uint32_t address) {return (uint32_t)rw(address)<<16|rw(address+2);}
static void sound(neo_genv_t *e,uint16_t id) {if (e->sound) e->sound(e->ctx,id);}
static neo_gactor_t *creator(neo_gactor_t *a,neo_genv_t *e) {return e->creator?e->creator(e->ctx,a):NULL;}
static uint16_t node_word(neo_genv_t *e,const neo_gactor_t *a) {return e->node_word?e->node_word(e->ctx,a):(uint16_t)(0x8000u|(uint16_t)a->index);}
static void touched(neo_gactor_t *a) {static const uint8_t o[]={0x4e,0x4f,0x5e,0x5f};for (unsigned i=0;i<sizeof(o);i++) ga_sb(a,o[i],a->m[o[i]]);}
static unsigned rider(const neo_gactor_t *a) {return ga_w(a,0x6e)&1u;}
static neo_vslot_player_t player(neo_genv_t *e,unsigned p) {return (neo_vslot_player_t){e->vehicle->exists[p],e->vehicle->player[p]};}
static uint16_t pad(neo_gactor_t *a,neo_genv_t *e) {return e->vehicle->input[rider(a)][0];}      /* 0CA2CE +0 */
static uint16_t rng(void *ctx) {return ga_rand((neo_genv_t *)ctx);}
static bool grounded(const neo_gactor_t *a) {return (ga_b(a,0x15)>>6)&1u;}
/* 01AD8C: +1E follows +66 - y when +66 is 8xxx. */
static void depth(neo_gactor_t *a) {uint16_t d0=W(0x66);if ((d0&0xf000u)==0x8000u) SW(0x1e,d0-W(0x40));}
/* 0AE244: program d0 of the table +78 (+7C = d0). */
static bool prog(neo_gactor_t *a,neo_genv_t *e,uint16_t d0) {SW(0x7c,d0);return ga_anim_start(a,e,rl(L(0x78)+4u*d0),0x100);}
static neo_gactor_t *child(neo_gactor_t *a,neo_genv_t *e,uint32_t routine) {return e->spawn?e->spawn(e->ctx,a,routine,false):NULL;}   /* 0040BC */
static neo_gactor_t *after_script(neo_gactor_t *a,neo_genv_t *e,uint32_t routine,int16_t dx,int16_t dy) {      /* 00411C, 01ACF2 */
    neo_gactor_t *c=e->spawn?e->spawn(e->ctx,NULL,routine,true):NULL;
    if (c) ga_place_at(c,a,e,dx,dy);
    return c;
}
/* 0AE220: the white screen 0AE1DC after the script actor (+6E = d0, +70 = d1). */
static void flashout(neo_gactor_t *a,neo_genv_t *e,uint16_t d0,uint8_t d1) {
    neo_gactor_t *c=e->spawn?e->spawn(e->ctx,NULL,0xae1dc,true):NULL;
    if (c) {ga_sb(c,0x70,d1);ga_sw(c,0x6e,d0);}
}
/* 0AD9E6: a piece 0AD9B8 at the vehicle with +30/+34/+4E. */
static void piece(neo_gactor_t *a,neo_genv_t *e,uint32_t vx,uint32_t vy,uint16_t n) {
    neo_gactor_t *c=after_script(a,e,0xad9b8,0,0);
    if (c) {ga_sl(c,0x30,vx);ga_sl(c,0x34,vy);ga_sw(c,0x4e,n);}
}
/* 0353A4 / 0353C2: the marker 03523C / 03533A (+6E/+70 its offset). */
static void marker(neo_gactor_t *a,neo_genv_t *e,uint32_t routine,uint16_t d0,uint16_t d1) {
    neo_gactor_t *c=child(a,e,routine);
    if (c) {ga_sw(c,0x6e,d0);ga_sw(c,0x70,d1);}
}
/* 0AE4EC: the knock-back table by +5B (+8F bit 0: the other side). */
static void knock(neo_gactor_t *a) {
    unsigned d7=(B(0x5b)&0x1fu)*8u;uint32_t t=(B(0x8f)&1u)?0xae3ecu:0xae46cu;
    SL(0x30,rl(t+d7));SL(0x34,rl(t+d7+4u));
}
/* 0AE2D2: the stick in flight: +80 / +30 by right/left, +7E / +34 by up /
 * down, both easing back to 0x280. */
static void stick(neo_gactor_t *a,uint16_t d0) {
    if (W(0x8e)) {knock(a);return;}
    if (d0&8u) {uint16_t v=(uint16_t)(W(0x80)+0x80u);SW(0x80,v>=0x500u?0x480u:v);SL(0x30,0x40000u);}
    else if (d0&4u) {SW(0x80,W(0x80)>=0x80u?W(0x80)-0x80u:0);SL(0x30,0xfffc0000u);}
    else {
        uint16_t d7=W(0x80);
        if (d7>0x280u) {d7=(uint16_t)(d7-0x40u);if (d7<0x280u) d7=0x280;}
        else if (d7<0x280u) {d7=(uint16_t)(d7+0x40u);if (d7>0x280u) d7=0x280;}
        SW(0x80,d7);SL(0x30,0);
    }
    if (d0&1u) {uint16_t v=(uint16_t)(W(0x7e)+0x80u);SW(0x7e,v>=0x500u?0x480u:v);SL(0x34,0x40000u);}
    else if (d0&2u) {SW(0x7e,W(0x7e)>=0x80u?W(0x7e)-0x80u:0);SL(0x34,0xfffc0000u);}
    else {
        uint16_t d7=W(0x7e);
        if (d7>0x280u) {d7=(uint16_t)(d7-0x40u);if (d7<0x280u) d7=0x280;}
        else if (d7<0x280u) {d7=(uint16_t)(d7+0x40u);if (d7>0x280u) d7=0x280;}
        SW(0x7e,d7);SL(0x34,0);
    }
}
/* 0AE51C: the stick near the ground: +30 by right/left; up lifts; down
 * charges +7E (by 0xE0, 0x820 after 0x900) and sinks, else +7E drains. */
static void stick_low(neo_gactor_t *a,uint16_t d0) {
    if (W(0x8e)) {knock(a);return;}
    SL(0x30,(d0&8u)?0x40000u:(d0&4u)?0xfffc0000u:0u);
    if (d0&1u) {SL(0x34,0x40000u);return;}
    if (d0&2u) {uint16_t v=(uint16_t)(W(0x7e)+0xe0u);SW(0x7e,v>=0x900u?0x820u:v);SL(0x34,0xfffc0000u);return;}
    SW(0x7e,W(0x7e)>=0xe0u?W(0x7e)-0xe0u:0);SL(0x34,0);
}
/* 0AE5A0 / 0AE5BA: the pose +95 and its anchor +82. */
static void pose_low(neo_gactor_t *a) {SB(0x95,B(0x7e));SW(0x82,rw(0xaad10u+2u*B(0x95)));}
static bool pose_fly(neo_gactor_t *a,neo_genv_t *e) {
    uint8_t d1=(uint8_t)((uint8_t)(B(0x7e)<<3)+B(0x80));
    if (d1!=B(0x95)) {
        if (d1==0x12u) {if (!prog(a,e,5)) return false;}
        else if (B(0x95)==0x12u) {if (!prog(a,e,4)) return false;}
        SB(0x95,d1);
    }
    if (B(0x95)!=0x12u) SW(0x82,rw(0xaacc0u+2u*B(0x95)));
    return true;
}
/* 0AE800 / 0AE82E / 0AE852: 0AA994, then 0AA83A (kind 4, d4 0x30) with
 * the player's grenades (type 1) on the ground, the shells (type 6) aloft. */
static marco_weapons_t *weapons(neo_genv_t *e) {return e->pickup?e->pickup->weapons:NULL;}
static void mount_ground(neo_gactor_t *a,neo_genv_t *e) {
    unsigned p=rider(a);neo_vslot_damage_flag(e->vehicle->slots,p,a->m);
    marco_weapons_t *w=weapons(e);if (!w) {ga_fail(e,0xc07e0);return;}
    neo_vslot_mount(e->vehicle->slots,p,a->m,4,w->b[8u+p],1,0x30);
}
static void mount_air(neo_gactor_t *a,neo_genv_t *e,bool flag) {
    unsigned p=rider(a);
    if (flag) {
        neo_vslot_damage_flag(e->vehicle->slots,p,a->m);
        uint8_t kind,shells,type;uint16_t health,d4;
        if (!neo_vslot_ridden(e->vehicle->slots,p,e->vehicle->exists[p],&kind,&shells,&type,&health,&d4)) shells=type=0;
        if (type==6u) SW(0x76,shells);
    }
    neo_vslot_mount(e->vehicle->slots,p,a->m,4,(uint8_t)W(0x76),6,0x30);
}
static void mount_by(neo_gactor_t *a,neo_genv_t *e) {if (grounded(a)) mount_ground(a,e); else mount_air(a,e,true);}
/* 0AE720: leaving (+96 = +94 = FF): the seat (d0, d1) once more, then
 * 0AA7FC with kind d2. */
static void leave(neo_gactor_t *a,neo_genv_t *e,int16_t d0,int16_t d1,uint8_t kind) {
    SB(0x96,0xff);SB(0x94,0xff);
    unsigned p=rider(a);neo_vslot_player_t pl=player(e,p);uint16_t p6e;
    (void)neo_vslot_bind(e->vehicle->slots,p,a->m,node_word(e,a),&pl,e->video,d0,d1,&p6e);
    neo_vslot_exit(e->vehicle->slots,p,a->m,node_word(e,a),kind);
}
/* 0AE260: 0AA994, then the way out: 0 the player leaves (018F24, or B
 * with down after a down landing), 1 the vehicle is done (+5F bit 7), -1
 * stays. */
static int exits(neo_gactor_t *a,neo_genv_t *e,uint16_t d1) {
    neo_vehicle_env_t *v=e->vehicle;
    neo_vslot_damage_flag(v->slots,rider(a),a->m);
    if (v->halted) return 0;
    if (v->dip10f000&0x80u) SB(0x5f,B(0x5f)&0x7fu);
    else if (BIT(0x5f,7)) return 1;
    if (B(0x99) && (d1&0x22u)==0x22u) return 0;
    return -1;
}
/* 0AE748: the death palette step: from +86 = 0x1E the list 0AADCA (by
 * 10C2E7, never set in the port: 0AAE9E is the two-player list), true at
 * its end (0); else +86 += d0. */
static bool palette_step(neo_gactor_t *a,neo_genv_t *e,uint16_t d0) {
    if (W(0x86)>=0xf2u) {ga_fail(e,0xae752);return false;}
    if (W(0x86)>=0x1eu) {
        uint16_t c=rw(0xaadcau+2u*(uint16_t)(W(0x86)-0x1eu));
        if (!c) return true;
        SB(0x1d,0);ga_palette(a,c);                                     /* 009F88, 009EA8 */
    }
    SW(0x86,W(0x86)+d0);return false;
}
/* 0AE86A: dust 0AD776 under the vehicle in flight, near a floor (01495C
 * by 0x20) that is not of group 1. */
static void dust(neo_gactor_t *a,neo_genv_t *e) {
    if (grounded(a) || B(0x9b)) return;
    int16_t dy=0;uint32_t kind=0;
    if (!ga_probe_vertical_kind(a,e,-0x20,&dy,&kind) || (kind>>16)==1u) return;
    neo_gactor_t *c=child(a,e,0xad776);
    if (c) ga_place_at(c,a,e,-8,dy);
}
/* 0C0808 (the player's code, as neo_tank.c's 0929E6): a grenade of the
 * rider's type at (d2, d3) with the vehicle's velocity. */
static void grenade(neo_gactor_t *a,neo_genv_t *e,unsigned p,int16_t d2,int16_t d3) {
    marco_weapons_t *w=weapons(e);uint8_t *pm=e->vehicle->player[p];
    if (!w || !pm) {ga_fail(e,0xc0808);return;}
    if (!marco_weapons_throw(w,p,(e->vehicle->dip10f001&4u)!=0)) return;   /* 0C0824 */
    uint8_t type=w->b[10+p];
    if (!type) return;
    unsigned idx=(unsigned)(type-1u);if (idx>=3u) idx=0;
    uint32_t table=pm[0x93]==2?0xc476eu:0xc4756u;
    uint32_t ctor=rl(table+idx*8u+(pm[0x90]==0xffu?4u:0u));
    neo_gactor_t *c=ctor&&e->spawn?e->spawn(e->ctx,NULL,ctor,true):NULL;
    if (!c) return;
    ga_sb(c,0x5e,B(0x5e));ga_sw(c,0x6e,(uint16_t)p);
    ga_sweep_place_d0(a,e,c,d2,d3,0);
    uint32_t vx=L(0x30),vy=L(0x34);neo_contact_parent_t q;
    if (L(0x38) && ga_parent(e,L(0x38),&q)) {vx+=(uint32_t)q.x-(uint32_t)q.previous_x;vy+=(uint32_t)q.y-(uint32_t)q.previous_y;}
    if (ga_l(c,0x38) && ga_parent(e,ga_l(c,0x38),&q)) {vx-=(uint32_t)q.x-(uint32_t)q.previous_x;vy-=(uint32_t)q.y-(uint32_t)q.previous_y;}
    ga_sl(c,0x30,vx);ga_sl(c,0x34,vy);
    e->vehicle->grenades++;e->vehicle->grenade_last=ctor;
}
/* 0AE610: fire: in the air a shell (0AA956) drops the bomb 0ADA16 with
 * its flash 0AD452; on the ground the player's grenade (0C07E0 / 0C07F8)
 * and the pilot's throw 0AE03A. */
static void fire(neo_gactor_t *a,neo_genv_t *e,uint16_t d0) {
    mount_by(a,e);
    if (B(0x96) || !(d0&0x40u) || B(0x98) || !B(0x97)) return;
    unsigned p=rider(a);
    if (!grounded(a)) {
        uint8_t *slot=e->vehicle->slots->slot[p].b;bool unlimited=(e->vehicle->dip10f001&4u)!=0;
        if (!neo_vslot_shell(e->vehicle->slots,p,unlimited)) return;
        SW(0x76,unlimited?(uint16_t)p:slot[0x1f]);
        SB(0x98,0x20);
        (void)child(a,e,0xad452);
        neo_gactor_t *c=after_script(a,e,0xada16,0,0);
        if (c) ga_sb(c,0x5e,B(0x5e));
        return;
    }
    marco_weapons_t *w=weapons(e);if (!w) {ga_fail(e,0xc07e0);return;}
    if (w->b[8u+p]) {
        if (w->b[p]>=2u) return;                                        /* 0C07F8 */
        grenade(a,e,p,0xe,0x1f);mount_ground(a,e);
    }
    SB(0x98,0x20);
    neo_gactor_t *c=child(a,e,0xae03a);
    if (c) ga_sw(c,0x70,W(0x70));
}
/* 0AE91A: the attack rows 0AE8EA / 0AE8FA / 0AE90A while down / left / right. */
static void bump(neo_gactor_t *a,neo_genv_t *e) {
    uint16_t d0=pad(a,e);
    if (d0&2u) ga_attack(a,e,0xae8ea);
    if (d0&4u) ga_attack(a,e,0xae8fa);
    if (d0&8u) ga_attack(a,e,0xae90a);
}
/* 0ACA7C: the seat's state and the wings 0ADEDA, the pilot 0AD2AC. */
static void seat(neo_gactor_t *a,neo_genv_t *e) {
    SB(0x9b,0);SB(0x98,0);SB(0x94,0);SL(0x72,0);SB(0x96,0);SB(0x99,0);SB(0x9a,0);
    (void)child(a,e,0xadeda);
    neo_gactor_t *c=child(a,e,0xad2ac);
    if (c) ga_sw(c,0x70,W(0x70));
}
/* 0AD1C4: the common tail. */
static ga_result_t tail(neo_gactor_t *a,neo_genv_t *e) {
    neo_vehicle_env_t *v=e->vehicle;
    if (!B(0x96)) {
        unsigned p=rider(a);
        if (!W(0x8c)) {
            SB(0x5f,B(0x5f)&~0x40u);
            touched(a);bool hit=ga_register(a,e,NULL,NULL);
            bool carry=neo_vslot_damage(v->slots,p,a->m,hit);touched(a);  /* 0AE8C2 */
            if (carry) {
                if (!W(0x4e)) SB(0x5f,B(0x5f)|0x80u);
                SW(0x8c,0x3c);SB(0x5f,B(0x5f)|0x40u);SW(0x8e,2);sound(e,0x1089);
            }
            SB(0x5a,0);
        } else {
            touched(a);bool hit=ga_register(a,e,NULL,NULL);
            bool carry=neo_vslot_damage(v->slots,p,a->m,hit);touched(a);
            if (carry && !W(0x4e)) SB(0x5f,B(0x5f)|0x80u);
            SB(0x5f,B(0x5f)&0x7fu);SB(0x5a,0);
            if (W(0x8e)) SW(0x8e,W(0x8e)-1u);
            else {
                SW(0x8c,W(0x8c)-1u);
                if (!W(0x8c)) SB(0x5f,B(0x5f)&~0x40u);
                if (W(0x8c)&2u) {SB(0x14,B(0x19));SB(0x84,0xff);} else SB(0x14,B(0x1a));
            }
        }
        if (!BIT(0x5f,7)) {
            uint16_t d0=pad(a,e);
            if (!B(0x9d)) fire(a,e,d0);
            bump(a,e);
        }
        neo_vslot_player_t pl=player(e,p);uint16_t p6e;
        bool bound=neo_vslot_bind(v->slots,p,a->m,node_word(e,a),&pl,e->video,0xc,(int16_t)W(0x72),&p6e);
#if defined(VEHDBG)
        fprintf(stderr,"VEH tail p=%u bound=%d exists=%d 5f=%02x node=%04x slot0=%04x 1c=%02x video=%04x\n",p,bound,pl.exists,pl.m?pl.m[0x5f]:0,node_word(e,a),v->slots->slot[p].b[0]<<8|v->slots->slot[p].b[1],v->slots->slot[p].b[0x1c],e->video);
#endif
        if (!bound) SL(0,0xac870);
        if (e->dyn_set) e->dyn_set(e->ctx,a,0xaac94,0,0);               /* 0140C0 */
    }
    if (B(0x98)) SB(0x98,B(0x98)-1u);
    return e->unsupported?GA_UNSUPPORTED:GA_MORE;
}
/* 0ACE44 / 0ACEB0 / 0ACFB2 / 0AD01A: the ways out (see exits()). */
#define EXIT_TO(r) do { SL(0,(r)==0?0xace44u:(r)==2?0xacfb2u:0xaceb0u);continue; } while (0)
static ga_result_t vehicle_tick(neo_gactor_t *a,neo_genv_t *e) {
    if (!e->vehicle) STOP(L(0));
    neo_vehicle_env_t *v=e->vehicle;
    uint16_t d0;int r,x;
#if defined(VEHDBG)
    fprintf(stderr,"VEH tick %05x 96=%02x 15=%02x\n",(unsigned)L(0),B(0x96),B(0x15));
#endif
    for (int guard=0;guard<8;guard++) { switch (L(0)) {
    case 0xac866:
        ga_floor(a,e);                                                  /* 0146E8 */
        /* 0AC6D4 */
        SL(0x50,0xaaca4);SL(0x78,0xaaf72);SW(0x7e,0x280);SW(0x80,0x280);SB(0x95,0);SL(0x30,0);SL(0x34,0);
        SB(0x9d,0);SW(0x76,0xa);SB(0x97,1);
        {neo_gactor_t *t=child(a,e,0x3b4e0);
         if (t) {ga_sl(t,0x6e,NEO_GA_TAG(a->index)|(0x82u<<16));ga_sb(t,0x72,1);ga_sb(t,0x73,0);ga_sb(t,0x74,0);ga_sb(t,0x75,0);}}
        ga_flash_palette(a,0xce);ga_palette(a,0xd);ga_palette(a,0xce);SW(0x4e,0x30);   /* 10C2E7 = 0 */
        SL(0,0xac870);continue;
    case 0xac870:
        SW(0x7c,0);SB(0x14,B(0x1a));SB(0x94,0xff);SB(0x5f,B(0x5f)&0x7fu);SB(0x5a,0);
        if (!prog(a,e,0)) return GA_UNSUPPORTED;
        marker(a,e,0x3523c,0x10,0x30);
        if (e->dyn_release) e->dyn_release(e->ctx,a);                  /* 013F1E */
        SW(0x66,0x8810);SW(0x1e,0x8810);SW(0x8c,0);SB(0x5f,B(0x5f)&~0x40u);SW(0x8e,0);
        SL(0,0xac8d4);
        /* fall through */
    case 0xac8d4: {
        if (!grounded(a)) {SL(0x30,0);SL(0x34,L(0x34)+0xffffb000u);ga_air(a,e);}
        else {SL(0x62,0);ga_ground(a,e,NULL);}
        STEP(r);(void)r;
        if (BIT(0x5f,7)) {
            sound(e,0x1034);flashout(a,e,1,0);
            piece(a,e,0xfff30000u,0x180000u,0xa);piece(a,e,0xb0000u,0x150000u,7);piece(a,e,0x80000u,0xfffc0000u,5);
            (void)after_script(a,e,0xad8a2,0,0);
            return GA_DELETE;
        }
        if (ga_outside_view(a,e,0xaacb0)) return GA_DELETE;            /* 0C8BEC -> 004188 */
        int16_t rect[4];for (unsigned i=0;i<4;i++) rect[i]=(int16_t)rw(0xaacb8u+2u*i);
        neo_vslot_player_t pl[2]={player(e,0),player(e,1)};
        int p=neo_vslot_select(v->slots,a->m,pl,rect,v->halted!=0,v->system!=0,rng,e);
        if (p>=0) {
            SW(0x6e,(uint16_t)p);uint16_t p6e;
            if (neo_vslot_bind(v->slots,(unsigned)p,a->m,node_word(e,a),&pl[p],e->video,0xc,0,&p6e)) {
                SW(0x70,p6e);mount_air(a,e,false);
                SL(0,0xacac0);sound(e,0x1090);SW(0x8c,0x1e);SB(0x5f,B(0x5f)|0x40u);
            }
            touched(a);
        }
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    }
    case 0xacac0: {
        seat(a,e);SL(0x30,0);SL(0x34,0);SL(0x62,0);
        const uint8_t *pm=v->player[rider(a)];
        if (!prog(a,e,(pm && pm[0x93])?2:1)) return GA_UNSUPPORTED;     /* 0C05B8 */
        SL(0,0xacaf6);
    }   /* fall through */
    case 0xacaf6:
        if (!grounded(a)) {SL(0x30,0);SL(0x34,L(0x34)+0xffffb000u);(void)neo_slug_k143ee(a,e);}
        else {SL(0x62,0);(void)neo_slug_k14594(a,e);}
        depth(a);STEP(r);
        if (r) SL(0,0xacb3a);
        return tail(a,e);
    case 0xacb3a:
        (void)after_script(a,e,0xad856,-8,0);
        SL(0x30,0);SL(0x34,0);SB(0x96,0);
        if (!prog(a,e,5)) return GA_UNSUPPORTED;
        SB(0x95,0x12);SW(0x7e,0x280);SW(0x80,0x280);SL(0x88,0x100000);sound(e,0x108b);
        SL(0,0xacb96);
        /* fall through */
    case 0xacb96: {
        (void)neo_slug_k143ee(a,e);
        d0=pad(a,e);stick(a,d0);
        if ((x=exits(a,e,d0))>=0) EXIT_TO(x);
        int32_t d7=(int32_t)L(0x88),d6=d7>>2;d7=(d7>>1)+d6;
        SL(0x34,L(0x34)+(uint32_t)d7);SL(0x88,(uint32_t)d7);
        if (d7<0x1000) SL(0,0xacc0a);
        if (!pose_fly(a,e)) return GA_UNSUPPORTED;
        dust(a,e);depth(a);STEP(r);(void)r;
        return tail(a,e);
    }
    case 0xacc04:
        if (!prog(a,e,5)) return GA_UNSUPPORTED;
        /* fall through */
    case 0xacc0a:
        SB(0x9a,0);SL(0x72,0);SL(0,0xacc18);
        /* fall through */
    case 0xacc18:
        (void)neo_slug_k143ee(a,e);
        d0=pad(a,e);stick(a,d0);
        if (d0&0x80u) {SL(0,0xad01a);continue;}
        if ((x=exits(a,e,d0))>=0) EXIT_TO(x);
        if (W(0x7e)==0x280u && W(0x80)==0x280u) {
            uint8_t d7=B(0x9a);SB(0x9a,(uint8_t)((d7+1u)&0x3fu));
            int32_t bob=(int16_t)rw(0xaad4au+2u*d7);
            SL(0x34,L(0x34)+(uint32_t)bob);SL(0x72,L(0x72)-(uint32_t)bob);
        } else {SL(0x72,0);SB(0x9a,0);}
        if ((d0&2u) && grounded(a)) SL(0,0xacd76);                     /* 0AE704 */
        if (!pose_fly(a,e)) return GA_UNSUPPORTED;
        dust(a,e);depth(a);STEP(r);(void)r;
        return tail(a,e);
    case 0xaccbe:
        SL(0x34,0x20000);SB(0x99,0);SL(0x72,0);SL(0,0xaccd4);
        /* fall through */
    case 0xaccd4: {
        (void)neo_slug_k143ee(a,e);
        mount_by(a,e);
        d0=pad(a,e);uint16_t keep=W(0x7e);stick_low(a,d0);SW(0x7e,keep);
        if (d0&0x80u) {SL(0,0xad01a);continue;}
        if ((x=exits(a,e,d0))>=0) EXIT_TO(x);
        if ((d0&2u) && grounded(a)) SL(0,0xacd8c);
        if (W(0x7e)<0xe0u) {(void)child(a,e,0xad3e2);SW(0x7e,0);SL(0,0xacc04);}
        else SW(0x7e,W(0x7e)-0xe0u);
        pose_low(a);dust(a,e);depth(a);STEP(r);(void)r;
        return tail(a,e);
    }
    case 0xacd76:
        SL(0x72,0);SW(0x7e,0);
        if (!prog(a,e,6)) return GA_UNSUPPORTED;
        SL(0x30,0);SL(0x34,0);
        /* fall through */
    case 0xacd8c:
        SB(0x99,0xff);SL(0,0xacd96);
        /* fall through */
    case 0xacd96: {
        (void)neo_slug_k143ee(a,e);
        mount_by(a,e);
        d0=pad(a,e);uint16_t was=W(0x7e);stick_low(a,d0);
        if (d0&0x80u) {SL(0,0xad01a);continue;}
        if (!W(0x7e) && was) (void)child(a,e,0xad3e2);
        if ((x=exits(a,e,d0))>=0) EXIT_TO(x);
        if (!grounded(a) || !(d0&2u)) SL(0,0xaccbe);
        pose_low(a);dust(a,e);depth(a);STEP(r);(void)r;
        return tail(a,e);
    }
    case 0xace44:
        SL(0x72,0);SB(0x99,0);SL(0x30,0);SL(0x34,0);SL(0x62,0);
        if (!prog(a,e,0)) return GA_UNSUPPORTED;
        SL(0,0xace64);
        /* fall through */
    case 0xace64:
        if (!grounded(a)) {SL(0x30,0);SL(0x34,L(0x34)+0xffffb000u);(void)neo_slug_k143ee(a,e);}
        else {SL(0x62,0);(void)neo_slug_k14594(a,e);}
        depth(a);STEP(r);(void)r;
        leave(a,e,0xc,0x18,1);SL(0,0xac870);
        return tail(a,e);
    case 0xaceb0:
        sound(e,0x10e9);SL(0x72,0);marker(a,e,0x3533a,0x10,0x2c);
        SB(0x99,0);SL(0x30,0);SL(0x34,0);SL(0x62,0);
        if (!prog(a,e,7)) return GA_UNSUPPORTED;
        SW(0x86,0);SB(0x9d,0xff);SL(0,0xaceec);
        /* fall through */
    case 0xaceec:
        if (!grounded(a)) {SL(0x30,0);SL(0x34,L(0x34)+0xffffb000u);}
        else {SL(0x30,0);SL(0x34,0);}
        ga_air(a,e);
        mount_by(a,e);
        if (palette_step(a,e,1)) {SL(0,0xacfb2);continue;}
        if (e->unsupported) return GA_UNSUPPORTED;
        if (!B(0x96) && (pad(a,e)&0x20u)) {
            SB(0x96,0xff);
            if (!prog(a,e,8)) return GA_UNSUPPORTED;
            leave(a,e,0xc,0x18,4);
            (void)after_script(a,e,0xad5d4,0x12,0x12);(void)child(a,e,0xad6e4);
            if (e->dyn_release) e->dyn_release(e->ctx,a);
        }
        depth(a);STEP(r);(void)r;
        return tail(a,e);
    case 0xacfb2:
        if (!B(0x96)) leave(a,e,0xc,0x18,0);
        (void)after_script(a,e,0x1c71e,0,0);
        SB(0x96,0xff);sound(e,0x1034);SL(0,0xad014);
        depth(a);STEP(r);(void)r;
        ga_attack(a,e,0xaac34);
        return tail(a,e);
    case 0xad014: return GA_DELETE_TREE;                                /* 0041AC */
    case 0xad01a:
        sound(e,0x10e9);SL(0x72,0);SW(0x86,0x1e);SB(0x96,0xff);
        leave(a,e,0xc,0x18,4);
        (void)after_script(a,e,0xad5d4,0x12,0x12);(void)child(a,e,0xad6e4);
        if (!prog(a,e,9)) return GA_UNSUPPORTED;
        SL(0x34,0);SB(0x5f,B(0x5f)|0x80u);SL(0x30,0);
        if (e->dyn_release) e->dyn_release(e->ctx,a);
        SL(0,0xad08e);
        /* fall through */
    case 0xad08e: {
        if (!(ga_rand(e)&3u)) {                                         /* 059E98 */
            neo_gactor_t *c=e->spawn?e->spawn(e->ctx,NULL,0x59c82,true):NULL;
            if (c) {
                int32_t vx=(int32_t)L(0x30),vy=(int32_t)L(0x34);
                ga_sl(c,0x6e,(uint32_t)(vx>>5));ga_sl(c,0x30,(uint32_t)(0-vx));
                ga_sl(c,0x72,(uint32_t)(vy>>5));ga_sl(c,0x34,(uint32_t)(0-vy));
                ga_place_at(c,a,e,-0x28,0x18);
            }
        }
        SL(0x34,L(0x34)+(ga_probe_vertical(a,e,-0xe)?0x800u:0xfffff800u));
        int32_t d7=(int32_t)L(0x30)+0x12000;if (d7>0x180000) d7=0x180000;
        SL(0x30,(uint32_t)d7);
        (void)neo_slug_k143ee(a,e);
        (void)palette_step(a,e,8);
        if (e->unsupported) return GA_UNSUPPORTED;
        uint8_t t=ga_attack(a,e,0xaac14);
        if (!t && (int16_t)W(0x3c)<=0x110 && !BIT(0x5f,0)) {depth(a);STEP(r);(void)r;return tail(a,e);}
        sound(e,0x1034);flashout(a,e,1,0);
        piece(a,e,0xfff30000u,0x180000u,0xa);piece(a,e,0xb0000u,0x150000u,7);piece(a,e,0x80000u,0xfffc0000u,5);
        (void)after_script(a,e,0xad8a2,0,0);
        SL(0,0xad014);depth(a);STEP(r);(void)r;
        ga_attack(a,e,0xaac24);
        return tail(a,e);
    }
    } }
    ga_fail(e,L(0));return GA_UNSUPPORTED;
}
/* The children: the pilot 0AD2AC (the rider's character +70: table 0AB40C,
 * palette 034330; its program follows the vehicle's +7C), the engine
 * flames 0AD3E2 / 0AD6E4, the bomb's flash 0AD452, the eject's piece
 * 0AD5D4 and spark 0AD58C, the flight dust 0AD776, the take-off smoke
 * 0AD856, the bomb 0ADA16 and its trail 0ADD2C, the wings 0ADEDA (by the
 * vehicle's +7C: 0ADE66) and the pilot's throw 0AE03A. */
static ga_result_t piece_end(neo_gactor_t *a,neo_genv_t *e) {
    int r;STEP(r);
    return r?GA_DELETE:(e->unsupported?GA_UNSUPPORTED:GA_MORE);
}
/* 0ADE96: the wings follow the vehicle's depth and anchor. */
static void wings_follow(neo_gactor_t *a,const neo_gactor_t *c) {
    SW(0x1e,ga_w(c,0x1e));SW(0x28,(uint16_t)(int16_t)(int8_t)ga_b(c,0x82));SW(0x2c,(uint16_t)(int16_t)(int8_t)ga_b(c,0x83));
}
/* The wings' states run as the ROM's code paths: 0ADEB6 (a new vehicle
 * program index jumps to the entry 0ADE66[+7C], whose code returns to the
 * caller of 0ADEB6) is called by BSR from 0ADF14 / 0ADF72 / 0ADFC6. */
static ga_result_t wings_run(neo_gactor_t *a,neo_genv_t *e,neo_gactor_t *c,uint32_t s);
static bool wings_check(neo_gactor_t *a,neo_genv_t *e,neo_gactor_t *c,ga_result_t *res) {
    if (ga_w(c,0x7c)==W(0x7c)) return false;
    SW(0x7c,ga_w(c,0x7c));
    *res=wings_run(a,e,c,rl(0xade66u+4u*W(0x7c)));
    return true;
}
static ga_result_t wings_common(neo_gactor_t *a,neo_genv_t *e,neo_gactor_t *c) {     /* 0ADFDC */
    int r;
    if (ga_b(c,0x94)) return wings_run(a,e,c,0xadf2a);
    wings_follow(a,c);ga_project(a,e);
    uint32_t t=0xae026u+4u*B(0x7e);
    SW(0x40,W(0x40)+rw(t));
    STEP(r);(void)r;
    SW(0x3c,W(0x3c)+6u);SW(0x40,W(0x40)+rw(t+2u));SW(0x1e,W(0x1e)-1u);
    STEP(r);(void)r;
    return e->unsupported?GA_UNSUPPORTED:GA_MORE;
}
static ga_result_t wings_run(neo_gactor_t *a,neo_genv_t *e,neo_gactor_t *c,uint32_t s) {
    int r;ga_result_t res=GA_MORE;
    switch (s) {
    case 0xadeda:
        ga_palette(a,0xf);ga_place(a,e,0,0,NEO_GA_TAG(c->index));SW(0x7c,0);SL(0,0xadefc);
        /* fall through */
    case 0xadefc:
        if (wings_check(a,e,c,&res)) return res;
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    case 0xadefe:
        START(0xac23e);SL(0,0xadf14);
        /* fall through */
    case 0xadf14:
        wings_follow(a,c);ga_project(a,e);STEP(r);
        if (r && wings_check(a,e,c,&res)) return res;
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    case 0xadf2a:
        START(0xac288);SL(0,0xadf40);
        /* fall through */
    case 0xadf40:
        wings_follow(a,c);ga_project(a,e);STEP(r);
        return r?GA_DELETE:(e->unsupported?GA_UNSUPPORTED:GA_MORE);
    case 0xadf5c:
        START(0xac2d2);SL(0,0xadf72);
        /* fall through */
    case 0xadf72:
        if (wings_check(a,e,c,&res) && res!=GA_MORE) return res;
        if (ga_l(c,0x30)!=L(0x30)) {
            int32_t d7=(int32_t)ga_l(c,0x30);SL(0x30,(uint32_t)d7);
            SB(0x80,d7==0?2:d7<0?0:4);SW(0x7e,0);
        }
        return wings_common(a,e,c);
    case 0xadfac:
        START(0xac2d2);SL(0,0xadfc6);
        /* fall through */
    case 0xadfc6:
        if (wings_check(a,e,c,&res) && res!=GA_MORE) return res;
        SB(0x7e,ga_b(c,0x7e));SB(0x80,ga_b(c,0x80));
        return wings_common(a,e,c);
    }
    STOP(s);
}
static ga_result_t wings_tick(neo_gactor_t *a,neo_genv_t *e) {
    neo_gactor_t *c=creator(a,e);
    if (!c) STOP(L(0));
    return wings_run(a,e,c,L(0));
}
static ga_result_t child_tick(neo_gactor_t *a,neo_genv_t *e) {
    neo_gactor_t *c=creator(a,e);int r;
    switch (L(0)) {
    case 0xad2ac:
        if (!c) STOP(0xad2ac);
        ga_place(a,e,0,0,NEO_GA_TAG(c->index));
        SL(0x78,rl(0xab40cu+4u*W(0x70)));
        ga_palette(a,rw(0x34330u+2u*W(0x70)));
        if (!prog(a,e,ga_w(c,0x7c))) return GA_UNSUPPORTED;
        SL(0,0xad2fc);
        /* fall through */
    case 0xad2fc:
        ga_project(a,e);
        if (!c || ga_b(c,0x94)) return GA_DELETE;
        if (!ga_creator_alive(a,e)) return GA_DELETE;
        SW(0x1e,ga_w(c,0x1e));SB(0x95,ga_b(c,0x95));
        SW(0x3c,W(0x3c)+ga_b(c,0x82));SW(0x40,W(0x40)+ga_b(c,0x83));
        if (ga_w(c,0x7c)!=W(0x7c) && !prog(a,e,ga_w(c,0x7c))) return GA_UNSUPPORTED;
        STEP(r);(void)r;
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    case 0xad3e2:
        if (!c) STOP(0xad3e2);
        ga_place(a,e,0,0,NEO_GA_TAG(c->index));ga_palette(a,0xf);START(0xac214);sound(e,0x1090);
        SL(0,0xad41a);
        /* fall through */
    case 0xad41a:
        ga_project(a,e);
        if (c) {SW(0x1e,ga_w(c,0x1e)+2u);SW(0x3c,W(0x3c)+ga_b(c,0x82));SW(0x40,W(0x40)+ga_b(c,0x83));}
        return piece_end(a,e);
    case 0xad452:
        if (!c) STOP(0xad452);
        {int16_t d1=(int16_t)((ga_rand(e)&3u)-1u);ga_place(a,e,0,d1,NEO_GA_TAG(c->index));}
        ga_palette(a,0xf);START(0xac5ca);SL(0,0xad48c);
        /* fall through */
    case 0xad48c:
        ga_project(a,e);
        if (c) SW(0x1e,ga_w(c,0x1e)+4u);
        return piece_end(a,e);
    case 0xad58c:
        ga_palette(a,0xf);START(0xad53a);SB(0x15,B(0x15)|3u);SL(0,0xad5bc);
        /* fall through */
    case 0xad5bc:
        ga_project(a,e);return piece_end(a,e);
    case 0xad5d4:
        ga_palette(a,0xf);START(0xac61c);SL(0x30,0xfffd8000u);SL(0x34,0x40000);SL(0,0xad608);
        /* fall through */
    case 0xad608:
        ga_air(a,e);
        if (grounded(a)) {(void)after_script(a,e,0xad58c,2,0xc);return GA_DELETE;}
        SL(0x34,L(0x34)+0xffffb000u);
        if (c) SW(0x1e,ga_w(c,0x1e)+3u);
        STEP(r);(void)r;
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    case 0xad6e4:
        if (!c) STOP(0xad6e4);
        ga_place(a,e,0,0,NEO_GA_TAG(c->index));ga_palette(a,0xf);START(0xac66e);SL(0,0xad712);
        /* fall through */
    case 0xad712:
        ga_project(a,e);
        if (c) SW(0x1e,ga_w(c,0x1e)-1u);
        return piece_end(a,e);
    case 0xad776:
        SW(0x66,0x1001);SW(0x1e,0x1001);ga_palette(a,0x2d);START(0xad738);
        if (c) ga_sb(c,0x9b,(uint8_t)(ga_b(c,0x9b)+1u));
        SL(0,0xad7ae);
        /* fall through */
    case 0xad7ae:
        ga_project(a,e);STEP(r);
        if (!r) return e->unsupported?GA_UNSUPPORTED:GA_MORE;
        if (c) ga_sb(c,0x9b,(uint8_t)(ga_b(c,0x9b)-1u));
        return GA_DELETE;
    case 0xad856:
        SW(0x66,0xe0ed);SW(0x1e,0xe0ed);ga_palette(a,0x204);START(0x22d372);SL(0,0xad88a);
        ga_project(a,e);return piece_end(a,e);                          /* 0AD884 -> 0AD88A */
    case 0xada16:
        if (c) ga_sb(c,0x97,(uint8_t)(ga_b(c,0x97)-1u));
        ga_palette(a,0xe);ga_palette(a,0xf);SB(0x14,B(0x19));START(0xac398);
        SB(0x15,B(0x15)|0x20u);sound(e,0x10f5);SB(0x9c,0);SL(0x90,0);ga_project(a,e);
        SL(0,0xada74);
        /* fall through */
    case 0xada74: {
        if (B(0x9c)) {ga_fail(e,0xada7c);return GA_UNSUPPORTED;}       /* the homing: explicit stop */
        uint32_t keep=L(0x34);SL(0x34,keep+L(0x90));ga_air(a,e);SL(0x34,keep);
        if (ga_cull(a,e,0xaacb0)) {if (c) ga_sb(c,0x97,(uint8_t)(ga_b(c,0x97)+1u));return GA_DELETE;}
        unsigned d7=(unsigned)(W(0x30)>>4);if (d7>3u) d7=3;
        uint8_t t=ga_attack(a,e,rl(0xaac44u+4u*d7));
        if (t || BIT(0x5f,0)) {
            piece(a,e,0x200000u,0x180000u,7);piece(a,e,0x1a0000u,0x30000u,6);
            (void)after_script(a,e,0xad8a2,0,0);
        } else if (grounded(a)) piece(a,e,0,0,0);
        else {
            if (c) SW(0x1e,ga_w(c,0x1e)+3u);
            STEP(r);(void)r;
            neo_gactor_t *q=child(a,e,0xadd2c);
            if (q) {ga_sw(q,0x6e,0xffec);ga_sw(q,0x70,4);}
            return e->unsupported?GA_UNSUPPORTED:GA_MORE;
        }
        /* 0ADC5A */
        sound(e,0x1025);if (c) ga_sb(c,0x97,(uint8_t)(ga_b(c,0x97)+1u));
        SB(0x1d,0);SW(0x66,0xe0ed);SW(0x1e,0xe0ed);
        SL(0,0x1c706);return neo_effects_tick(a,e);                     /* JMP 01C706 */
    }
    case 0xadd2c:
        if (!c) STOP(0xadd2c);
        ga_place(a,e,(int16_t)W(0x6e),(int16_t)W(0x70),NEO_GA_TAG(c->index));
        START(0xadc8a);ga_palette(a,0x37c);
        if (ga_b(c,0x15)&1u) SB(0x15,B(0x15)|1u);
        SL(0,0xadd72);
        /* fall through */
    case 0xadd72:
        if (c) {SW(0x1e,ga_w(c,0x1e)-1u);SL(0x30,0u-(0x70000u+ga_l(c,0x30)));}
        ga_move(a,e);STEP(r);
        if (r) return GA_DELETE;
        if (ga_cull(a,e,0xaacb0)) return GA_DELETE;
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    case 0xae03a:
        if (!c) STOP(0xae03a);
        ga_place(a,e,0xa,0x1b,NEO_GA_TAG(c->index));
        START(rl(0x23a59eu+4u*W(0x70)));ga_palette(a,rw(0x34330u+2u*W(0x70)));
        SL(0,0xae080);
        /* fall through */
    case 0xae080:
        ga_project(a,e);
        if (!c || ga_b(c,0x94) || !((ga_b(c,0x15)>>6)&1u)) return GA_DELETE;
        SW(0x1e,ga_w(c,0x1e)-1u);STEP(r);
        if (r) {ga_sb(c,0x98,0);return GA_DELETE;}
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    }
    ga_fail(e,L(0));return GA_UNSUPPORTED;
}
static bool wings(uint32_t r) {
    return r==0xadeda || r==0xadefc || r==0xadefe || r==0xadf14 || r==0xadf2a || r==0xadf40 || r==0xadf5c || r==0xadf72 || r==0xadfac || r==0xadfc6;
}
static bool vehicle(uint32_t r) {
    switch (r) {
    case 0xac866: case 0xac870: case 0xac8d4: case 0xacac0: case 0xacaf6: case 0xacb3a: case 0xacb96: case 0xacc04:
    case 0xacc0a: case 0xacc18: case 0xaccbe: case 0xaccd4: case 0xacd76: case 0xacd8c: case 0xacd96: case 0xace44:
    case 0xace64: case 0xaceb0: case 0xaceec: case 0xacfb2: case 0xad014: case 0xad01a: case 0xad08e: return true;
    }
    return false;
}
static bool children(uint32_t r) {
    switch (r) {
    case 0xad2ac: case 0xad2fc: case 0xad3e2: case 0xad41a: case 0xad452: case 0xad48c: case 0xad58c: case 0xad5bc:
    case 0xad5d4: case 0xad608: case 0xad6e4: case 0xad712: case 0xad776: case 0xad7ae: case 0xad856: case 0xada16:
    case 0xada74: case 0xadd2c: case 0xadd72: case 0xae03a: case 0xae080: return true;
    }
    return false;
}
bool neo_m3veh_routine(uint32_t r) {return vehicle(r) || children(r) || wings(r);}
ga_result_t neo_m3veh_tick(neo_gactor_t *a,neo_genv_t *e) {
    uint32_t r=ga_l(a,0);
    if (vehicle(r)) return vehicle_tick(a,e);
    if (wings(r)) return wings_tick(a,e);
    return child_tick(a,e);
}
