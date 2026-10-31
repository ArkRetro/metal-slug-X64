#include "neo_region.h"
#include <string.h>
#include "neo_vulcan.h"
/* Camel Slug 03458E (the player's vehicle of Mission 1, camera 2216, script
 * spawn uncounted) and the vulcan turret 03B4E0/03B57C it carries: manual
 * translation of ROM 034408..0354B8 and 03B4E0..03B86C; ROM labels are
 * provenance only.
 *
 * Camel fields: +6E player (0AA610), +70 the player's +6E (0AA760), +72 the
 * program table 034368 (state +76: 0 walk, 1 stand, 2 crouch, 3 down, 5
 * sink, 6 jump, 7 walk back), +78 the turret's anchor (x, y, flag; +79 from
 * the pose table 034340), +7E crouch height (0..0x380), +80 mount grace
 * (0x1E updates without damage), +82 updates ridden, +86 pose (034340: the
 * hurtbox 0342B8[(+86-29)&1C] and the bomb height), +87 set by the rider's
 * program (034EB0), +88 the no-bomb/bomb effect 034FD2 alive, +89 no rider
 * (its children leave), +8A dust step, +8F on water (kind 5), +90/+84 the
 * riderless charge.
 * States: wait 0345A0/0345E0 (health 0, no rider, the marker 03523C; falls
 * with 0143AC, squats back with 014574; cull 034308 -> 004188; 0AA610 with
 * the rectangle 034320, then 0AA760 (seat 0,18) and 0AA83A (kind 2)), mount
 * 034698 (sound 116E, the rider graphic 034F1A, palette 034328[+70]),
 * stand 0346EA/0346FA, crouch 034786/03478C/03479C, down 034852/034868,
 * walk 0348D8/0348F0 (+62 = +-0x38000 by the stick), jump 0349BE/0349C6/
 * 0349D8 (+34 = 0x8F5C2, gravity 0xB780, steering 0x9555 up to 0x38000;
 * 0143EE), eject 034BA0 (A+B: 0AA7FC kind 1, program 23A0C6, the riderless
 * charge 034BF0 with attack row 034348, turned back by walls 034D38 with row
 * 034358) and the common tail 034E1C (hurtbox, 0AA9E4 damage, push rows
 * 0354BA/0354CA by the stick on the ground, 0AA760 again: no rider ->
 * 0345A0). Exits 03517C: RAM 10C301 set, the camel killed (+5F bit 7 with
 * +80 = 0 unless RAM 10F000 bit 7: 0AA7FC kind 0, the rider dies) or down+B
 * on the ground (0AA7FC kind 1); A+B (035168) ejects. C (035100): a bomb by
 * 0C0808 when the rider has bombs and fewer than 2 in flight, and the effect
 * 034FD2 in every case but "bombs and 2 in flight".
 * Ground/air steps: 014594 / 0143EE (0128E8: RAM 10C00D set, the screen box
 * of the rider's record 10C2E8; 004ED2 with the solid-body table 106756,
 * empty in every vehicle trace: a registered body is an explicit stop),
 * 0143AC / 014574 (10C00B and 10C00D set: no bound).
 *
 * Explicit stops: water (034AC0, kind 5: no class-5 cell in Mission 1), the
 * turret's end 03B86E (the vehicle gone) and spark 03BA76, the camel's
 * other children 03533A.. (never created in the traces), a ROM table
 * outside data/region_rom.json. Not reproduced: 009CCE (palette fade RAM
 * 1068EA, no node byte) and the cartridge checks (2FFFE0) before 004188,
 * which end in the deletion on genuine hardware. */
#define B(o) ga_b(a,(o))
#define W(o) ga_w(a,(o))
#define L(o) ga_l(a,(o))
#define SB(o,v) ga_sb(a,(o),(uint8_t)(v))
#define SW(o,v) ga_sw(a,(o),(uint16_t)(v))
#define SL(o,v) ga_sl(a,(o),(uint32_t)(v))
#define BIT(o,b) ((B(o)>>(b))&1u)
#define START(t) do { if (!ga_anim_start(a,e,(t),0x100)) return GA_UNSUPPORTED; } while (0)
#define STEP(res) do { if (((res)=ga_anim_step(a,e,call))<0) return GA_UNSUPPORTED; } while (0)
static uint32_t rl(uint32_t address) {const uint8_t *p=ga_rom(address);return p?(uint32_t)p[0]<<24|(uint32_t)p[1]<<16|(uint32_t)p[2]<<8|p[3]:0;}
static uint16_t rw(uint32_t address) {const uint8_t *p=ga_rom(address);return p?(uint16_t)(p[0]<<8|p[1]):0;}
static void sound(neo_genv_t *e,uint16_t id) {if (e->sound) e->sound(e->ctx,id);}
static neo_gactor_t *creator(neo_gactor_t *a,neo_genv_t *e) {return e->creator?e->creator(e->ctx,a):NULL;}
static uint16_t node_word(neo_genv_t *e,const neo_gactor_t *a) {return e->node_word?e->node_word(e->ctx,a):(uint16_t)(0x8000u|(uint16_t)a->index);}
/* Re-mark bytes the slot services wrote through the node record. */
static void touched(neo_gactor_t *a) {static const uint8_t o[]={0x4e,0x4f,0x5e,0x5f};for (unsigned i=0;i<sizeof(o);i++) ga_sb(a,o[i],a->m[o[i]]);}
static unsigned rider_p(const neo_gactor_t *a) {return (ga_w(a,0x6e)&0xffu)?1u:0u;}   /* tst.b d0 */
static uint16_t rng(void *ctx) {return ga_rand((neo_genv_t *)ctx);}
static neo_vslot_player_t player(neo_genv_t *e,unsigned p) {return (neo_vslot_player_t){e->vehicle->exists[p],e->vehicle->player[p]};}
static uint16_t pad(neo_gactor_t *a,neo_genv_t *e) {return e->vehicle->input[rider_p(a)][0];}  /* 0CA2CE +0 */
/* 0350E4: program d0 of the table +72. */
static bool program(neo_gactor_t *a,neo_genv_t *e,uint16_t d0) {
    SW(0x76,d0);uint32_t t=rl(L(0x72)+4u*d0);
    if (!t) {ga_fail(e,L(0x72));return false;}
    return ga_anim_start(a,e,t,0x100);
}
/* 0128E8: the screen box of the rider's record (0AAB66). */
static void config(neo_gactor_t *a,neo_genv_t *e) {
    int p=neo_vslot_which(e->vehicle->slots,node_word(e,a));
    uint32_t rec=p<0?NEO_VSLOT_RECORD:e->vehicle->slots->record[p];
    for (unsigned i=0;i<4;i++) e->kernel->config[i]=rw(rec+2u*i);
}
/* 004ED2 (after 0128E8): no push with the player lock (0C8CFC) or an empty
 * solid-body table; otherwise an explicit stop. */
static bool k4ed2(neo_gactor_t *a,neo_genv_t *e) {
    if (e->flags && e->flags->player_lock) return true;
    ga_project(a,e);                                                    /* 004EE0 */
    if (e->vehicle->solids) {ga_fail(e,0x4ed2);return false;}
    return true;
}
/* 014594 / 0143EE / 0143AC / 014574, then 03546A's +8F (kind 5). */
static uint16_t kind_of(uint32_t k) {return (uint16_t)(k>>16);}
static uint32_t ground_ride(neo_gactor_t *a,neo_genv_t *e) {
    uint32_t k=0,v30=L(0x30);config(a,e);SL(0x30,L(0x62));
    if (!k4ed2(a,e)) return 0;
    SL(0x30,v30);ga_ground_flags(a,e,0,0xff,&k);SB(0x8f,kind_of(k)==5u?0xff:0);return k;
}
static uint32_t air_ride(neo_gactor_t *a,neo_genv_t *e) {
    uint32_t k=0;config(a,e);
    if (!k4ed2(a,e)) return 0;
    ga_air_flags(a,e,0,0xff,&k);SB(0x8f,kind_of(k)==5u?0xff:0);return k;
}
static uint32_t air_free(neo_gactor_t *a,neo_genv_t *e) {uint32_t k=0;ga_air_flags(a,e,0xff,0xff,&k);return k;}
static uint32_t ground_free(neo_gactor_t *a,neo_genv_t *e) {uint32_t k=0;ga_ground_flags(a,e,0xff,0xff,&k);return k;}
/* 035496: gravity and 0143AC in the air, 014574 on the ground. */
static uint32_t k35496(neo_gactor_t *a,neo_genv_t *e) {
    if (!BIT(0x15,6)) {SL(0x34,L(0x34)+0xffffb000u);return air_free(a,e);}
    return ground_free(a,e);
}
/* 059EE0: dust 059E30 after the script actor at (dx,0), then 01B220 at (dx2,0). */
static void dust(neo_gactor_t *a,neo_genv_t *e,int16_t dx2) {
    neo_gactor_t *c=e->spawn?e->spawn(e->ctx,NULL,0x59e30,true):NULL;
    if (!c) return;
    int32_t vx=(int32_t)L(0x30),vy=(int32_t)L(0x34);
    ga_sl(c,0x6e,(uint32_t)(vx>>5));ga_sl(c,0x30,(uint32_t)(0-vx));
    ga_sl(c,0x72,(uint32_t)(vy>>5));ga_sl(c,0x34,(uint32_t)(0-vy));
    ga_place_at(c,a,e,0,0);                                             /* 01ACF2 */
    ga_place_from(c,a,e,dx2,0);                                         /* 01B220 */
}
static bool call(neo_gactor_t *a,neo_genv_t *e,neo_anim_kind_t kind,uint32_t routine,uint16_t parameter) {
    (void)parameter;
    if (kind!=NEO_ANIM_CALL) return false;
    neo_gactor_t *p;
    switch (routine) {
    case 0x34eb0: p=creator(a,e);if (p) ga_sb(p,0x87,0xff);return true;
    case 0x34eba:
        if (B(0x8a)) return true;
        SB(0x8a,0xff);
        if (BIT(0x15,6) && !B(0x8f)) dust(a,e,-0x1a);
        return true;
    case 0x34eee:
        if (!B(0x8a)) return true;
        SB(0x8a,0);
        if (BIT(0x15,6) && !B(0x8f)) dust(a,e,-8);
        return true;
    }
    return false;
}
/* 03517C: true (carry) when the rider leaves: 018F24, the camel killed
 * (0AA7FC kind 0) or down+B on the ground (kind 1). */
static bool k3517c(neo_gactor_t *a,neo_genv_t *e,uint16_t d1) {
    neo_vehicle_env_t *v=e->vehicle;
    if (v->halted) return true;
    if (!(v->dip10f000&0x80u) && !W(0x80) && BIT(0x5f,7)) {neo_vslot_exit(v->slots,rider_p(a),a->m,node_word(e,a),0);return true;}
    if (BIT(0x15,6) && (d1&0x22u)==0x22u) {neo_vslot_exit(v->slots,rider_p(a),a->m,node_word(e,a),1);return true;}
    return false;
}
/* 035100: C pressed with no effect alive. */
static void k35100(neo_gactor_t *a,neo_genv_t *e,uint16_t d0) {
    if (!(d0&0x40u) || B(0x88)) return;
    unsigned p=rider_p(a);
    marco_weapons_t *w=e->pickup?e->pickup->weapons:NULL;
    if (!w) {ga_fail(e,0xc07e0);return;}
    if (w->b[8+p]) {                                                    /* 0C07E0: bombs */
        if (w->b[p]>=2u) return;                                        /* 0C07F8: two in flight */
        /* 0C0808: one bomb (10F001 bit 2: free), the constructor of the
         * bomb type (table 0C4756, 0C476E with the rider in form 2; the
         * second of the pair with its +90 = FF), after the script actor. */
        uint8_t *pm=e->vehicle->player[p];
        bool thrown=marco_weapons_throw(w,p,(e->vehicle->dip10f001&4u)!=0);
        if (thrown) {
            uint8_t type=w->b[10+p];
            if (type) {
                unsigned idx=(unsigned)(type-1u);if (idx>=3u) idx=0;
                uint32_t table=pm[0x93]==2?0xc476eu:0xc4756u;
                uint32_t ctor=rl(table+idx*8u+(pm[0x90]==0xffu?4u:0u));
                neo_gactor_t *c=ctor&&e->spawn?e->spawn(e->ctx,NULL,ctor,true):NULL;
                if (c) {
                    ga_sb(c,0x5e,B(0x5e));ga_sw(c,0x6e,(uint16_t)p);
                    /* 0C482E: 014A02 (D0 = 0) at (4, 4 + +86), then the
                     * camel's velocity in the bomb's frame. */
                    ga_sweep_place_d0(a,e,c,4,(int16_t)(uint8_t)(4u+B(0x86)),0);
                    uint32_t vx=L(0x30),vy=L(0x34);neo_contact_parent_t q;
                    if (L(0x38) && ga_parent(e,L(0x38),&q)) {vx+=(uint32_t)q.x-(uint32_t)q.previous_x;vy+=(uint32_t)q.y-(uint32_t)q.previous_y;}
                    if (ga_l(c,0x38) && ga_parent(e,ga_l(c,0x38),&q)) {vx-=(uint32_t)q.x-(uint32_t)q.previous_x;vy-=(uint32_t)q.y-(uint32_t)q.previous_y;}
                    ga_sl(c,0x30,vx);ga_sl(c,0x34,vy);
                    e->vehicle->grenades++;e->vehicle->grenade_last=ctor;
                }
            }
        } else return;                                                  /* 0C0824: rts */
    }
    neo_gactor_t *c=e->spawn?e->spawn(e->ctx,a,0x34fd2,false):NULL;     /* 03514A */
    if (c) ga_sw(c,0x70,W(0x70));
    SB(0x88,0xff);
}
/* 034620/034830: pose by the crouch height. */
static void pose(neo_gactor_t *a) {
    unsigned i=(unsigned)(B(0x7e))*2u;const uint8_t *t=ga_rom(0x34340u+i);
    if (t) {SB(0x86,t[0]);SB(0x79,t[1]);}
}
/* 0AA9E4 with 004C24. */
static void damage(neo_gactor_t *a,neo_genv_t *e) {
    bool hit=ga_register(a,e,NULL,NULL);
    neo_vslot_damage(e->vehicle->slots,rider_p(a),a->m,hit);touched(a);
}
/* 034E1C: the common tail of the ridden states. */
static ga_result_t tail(neo_gactor_t *a,neo_genv_t *e) {
    if (B(0x87)) return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    SL(0x50,rl(0x342b8u+((uint32_t)(uint16_t)(B(0x86)-0x29u)&0x1cu)));
    if (!W(0x80)) {
        damage(a,e);
        if (BIT(0x5f,7)) SB(0x5f,B(0x5f)&0x7fu);
        SB(0x5a,0);
    } else {
        SW(0x80,W(0x80)-1u);damage(a,e);
        SB(0x5f,B(0x5f)&0x7fu);SW(0x4e,0);SB(0x5a,0);
    }
    if (BIT(0x15,6)) {                                                  /* 0354DA */
        uint16_t d0=pad(a,e);
        if (d0&4u) ga_attack(a,e,0x354ba);
        if (d0&8u) ga_attack(a,e,0x354ca);
    }
    uint16_t p6e;neo_vslot_player_t pl=player(e,rider_p(a));
    if (!neo_vslot_bind(e->vehicle->slots,rider_p(a),a->m,node_word(e,a),&pl,e->video,0,0x18,&p6e)) SL(0,0x345a0);
    touched(a);
    SW(0x82,W(0x82)+1u);
    return e->unsupported?GA_UNSUPPORTED:GA_MORE;
}
/* 0345A0 entry (the rider gone). */
static ga_result_t wait_entry(neo_gactor_t *a,neo_genv_t *e) {
    SB(0x8a,0);SB(0x89,0xff);SB(0x1d,0);ga_palette(a,0x3ff);           /* 009F88, 009EA8 */
    if (!program(a,e,2)) return GA_UNSUPPORTED;
    SB(0x5f,B(0x5f)&0x7fu);SB(0x5a,0);SW(0x4e,0);
    neo_gactor_t *c=e->spawn?e->spawn(e->ctx,a,0x3523c,false):NULL;     /* 0353A4 */
    if (c) {ga_sw(c,0x6e,0xfffd);ga_sw(c,0x70,0x28);}
    SL(0,0x345e0);
    return GA_MORE;
}
/* The ridden states end alike (0346FA..034782 and friends): 03546A, water,
 * the air, the program, the tail. */
static ga_result_t ride_end(neo_gactor_t *a,neo_genv_t *e,bool pose_after) {
    uint32_t k=ground_ride(a,e);
    if (e->unsupported) return GA_UNSUPPORTED;
    if (BIT(0x15,6) && kind_of(k)==5u) {ga_fail(e,0x34ac0);return GA_UNSUPPORTED;}   /* water */
    if (!BIT(0x15,6)) SL(0,0x349c6);
    if (pose_after) pose(a);
    int r;STEP(r);(void)r;
    return tail(a,e);
}
/* 034BBE.. the riderless charge after an eject. */
static ga_result_t charge_entry(neo_gactor_t *a,neo_genv_t *e) {
    START(0x23a0c6);
    SL(0x62,0);SW(0x84,0xf);SL(0,0x34bf0);
    return GA_MORE;
}
static ga_result_t camel_tick(neo_gactor_t *a,neo_genv_t *e) {
    if (!e->vehicle) {ga_fail(e,L(0));return GA_UNSUPPORTED;}
    neo_vehicle_env_t *v=e->vehicle;
    uint16_t d0;int r;
    switch (L(0)) {
    case 0x3458e: {
        /* 034408 */
        SW(0x66,0x8810);SW(0x1e,0x8810);SL(0x50,0x342cc);ga_floor(a,e);SW(0x4e,0);
        SL(0x72,0x34368);SW(0x76,1);SB(0x87,0);SB(0x78,0xf0);SB(0x8d,0);
        neo_gactor_t *t=e->spawn?e->spawn(e->ctx,a,0x3b4e0,false):NULL;
        if (t) {ga_sl(t,0x6e,NEO_GA_TAG(a->index)|(0x78u<<16));ga_sb(t,0x72,5);ga_sb(t,0x73,0);ga_sb(t,0x74,0);ga_sb(t,0x75,0);}
        SW(0x7e,0x380);SL(0,0x345a0);
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    }
    case 0x345a0:
        if (wait_entry(a,e)!=GA_MORE) return GA_UNSUPPORTED;
        /* fall through */
    case 0x345e0: {
        if (!BIT(0x15,6)) {SL(0x30,0);SL(0x34,L(0x34)+0xffffb000u);air_free(a,e);}
        else {
            SW(0x7e,W(0x7e)+0x80u);if ((int16_t)W(0x7e)>0x380) SW(0x7e,0x380);
            SL(0x62,0);ground_free(a,e);
        }
        pose(a);STEP(r);
        if (ga_outside_view(a,e,0x34308)) return GA_DELETE;            /* 0C8BEC -> 004188 */
        int16_t rect[4];for (unsigned i=0;i<4;i++) rect[i]=(int16_t)rw(0x34320u+2u*i);
        neo_vslot_player_t pl[2]={player(e,0),player(e,1)};
        int p=neo_vslot_select(v->slots,a->m,pl,rect,v->halted!=0,v->system!=0,rng,e);
        if (p>=0) {
            SW(0x6e,(uint16_t)p);uint16_t p6e;
            if (neo_vslot_bind(v->slots,(unsigned)p,a->m,node_word(e,a),&pl[p],e->video,0,0x18,&p6e)) {
                SW(0x70,p6e);
                neo_vslot_mount(v->slots,(unsigned)p,a->m,2,0,0,0);
                SW(0x80,0x1e);SL(0,0x34698);
            }
            touched(a);
        }
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    }
    case 0x34698: {
        sound(e,0x116e);SW(0x82,0);SB(0x89,0);
        neo_gactor_t *c=e->spawn?e->spawn(e->ctx,a,0x34f1a,false):NULL;
        if (c) ga_sw(c,0x70,W(0x70));
        SB(0x1d,0);ga_palette(a,rw(0x34328u+2u*W(0x70)));                 /* 009F88, 009EA8 */
        SL(0x62,0);SB(0x88,0);
    }   /* 034852 */
        if (!program(a,e,3)) return GA_UNSUPPORTED;
        SW(0x7e,0x380);SL(0x62,0);SL(0,0x34868);
        /* fall through */
    case 0x34868:
        d0=pad(a,e);
        if (k3517c(a,e,d0)) goto rider_gone;
        if (d0&0x80u) goto eject;
        if (!(d0&2u)) SL(0,0x3478c);
        if (d0&0x20u) SL(0,0x349be);
        k35100(a,e,d0);
        return ride_end(a,e,false);
    case 0x346ea:
        SL(0x62,0);if (!program(a,e,1)) return GA_UNSUPPORTED;SL(0,0x346fa);
        /* fall through */
    case 0x346fa:
        d0=pad(a,e);
        if (k3517c(a,e,d0)) goto rider_gone;
        if (d0&0x80u) goto eject;
        if (d0&2u) SL(0,0x34786);
        if (d0&8u) SL(0,0x348d8);
        if (d0&4u) SL(0,0x348d8);
        if (d0&0x20u) SL(0,0x349be);
        k35100(a,e,d0);
        return ride_end(a,e,false);
    case 0x34786: SW(0x7e,0x80);
        /* fall through */
    case 0x3478c:
        if (!program(a,e,2)) return GA_UNSUPPORTED;
        SL(0x62,0);SL(0,0x3479c);
        /* fall through */
    case 0x3479c:
        d0=pad(a,e);
        if (k3517c(a,e,d0)) goto rider_gone;
        if (d0&0x80u) goto eject;
        if (d0&2u) {
            SW(0x7e,(W(0x7e)+0x80u)&0x3ffu);
            if (W(0x7e)>=0x380u) SL(0,0x34852);
        } else {
            if (W(0x7e)<0x80u) {SW(0x7e,0);SL(0,0x346ea);} else SW(0x7e,W(0x7e)-0x80u);
        }
        if (d0&0x20u) SL(0,0x349be);
        k35100(a,e,d0);
        return ride_end(a,e,true);
    case 0x34852:
        if (!program(a,e,3)) return GA_UNSUPPORTED;
        SW(0x7e,0x380);SL(0x62,0);SL(0,0x34868);
        return camel_tick(a,e);
    case 0x348d8:
        SB(0x14,B(0x19));if (!program(a,e,0)) return GA_UNSUPPORTED;
        SL(0x62,L(0x30));SL(0,0x348f0);
        /* fall through */
    case 0x348f0: {
        d0=pad(a,e);uint32_t d7=L(0x62);
        if (d0&8u) d7=0x38000u;
        else if (d0&4u) d7=0xfffc8000u;
        else SL(0,0x346ea);
        SL(0x62,d7);
        if (k3517c(a,e,d0)) goto rider_gone;
        if (d0&0x80u) goto eject;
        if (d0&2u) SL(0,0x34786);
        if (d0&0x20u) SL(0,0x349be);
        k35100(a,e,d0);
        if (L(0x62)&0x80000000u) {if (W(0x76)!=7u && !program(a,e,7)) return GA_UNSUPPORTED;}
        else if (W(0x76) && !program(a,e,0)) return GA_UNSUPPORTED;
        return ride_end(a,e,false);
    }
    case 0x349be: SL(0x34,0x8f5c2);
        /* fall through */
    case 0x349c6:
        if (!program(a,e,6)) return GA_UNSUPPORTED;
        SL(0x30,L(0x62));SL(0,0x349d8);
        /* fall through */
    case 0x349d8: {
        d0=pad(a,e);uint32_t d7=L(0x30);
        if (d0&8u) {d7+=0x9555u;if ((int32_t)d7>0x38000) d7=0x38000u;}
        else if (d0&4u) {d7+=0xffff6aabu;if ((int32_t)d7<(int32_t)0xfffc8000u) d7=0xfffc8000u;}
        SL(0x30,d7);SL(0x34,L(0x34)+0xffff4880u);
        k35100(a,e,d0);
        uint32_t k=air_ride(a,e);
        if (e->unsupported) return GA_UNSUPPORTED;
        if (BIT(0x15,6) && kind_of(k)==5u) {ga_fail(e,0x34ac0);return GA_UNSUPPORTED;}
        if (BIT(0x15,6) && (int32_t)L(0x34)<=0) {
            SL(0,0x34786);d0=pad(a,e);
            if (d0&8u) SL(0,0x348d8);
            if (d0&4u) SL(0,0x348d8);
            if (d0&2u) SL(0,0x34786);
        }
        d0=pad(a,e);
        if (k3517c(a,e,d0)) goto rider_gone;
        STEP(r);
        return tail(a,e);
    }
    case 0x34bf0:
    charge: {
        uint32_t t=BIT(0x15,0)?0xfffa0000u:0x60000u;
        SL(0x62,L(0x62)+(uint32_t)((int32_t)(t-L(0x62))>>3));
        uint32_t k=k35496(a,e);SB(0x8f,kind_of(k)==5u?0xff:0);
        ga_attack(a,e,0x34348);
        if (BIT(0x5f,0) || BIT(0x5f,1)) {
            if (B(0x90)) {
                SB(0x90,B(0x90)-1u);SW(0x84,0xc);SB(0x12,0xc0);ga_attack(a,e,0x34358);SL(0,0x34d58);
                goto turn;
            }
            if (!BIT(0x5f,3)) {
                SW(0x84,W(0x84)-1u);
                if ((int16_t)W(0x84)<0) {SB(0x5f,B(0x5f)|8u);SW(0x84,0x1e);}
            } else {
                SW(0x84,W(0x84)-1u);
                if ((int16_t)W(0x84)<0) return GA_DELETE;               /* 004188 */
            }
        } else {SB(0x5f,B(0x5f)&~8u);SW(0x84,0xf);}
        uint32_t s=L(0x62);if (s&0x80000000u) s=0u-s;
        SW(0x26,(uint16_t)(s>>9));
        STEP(r);
        if (ga_cull(a,e,0x34308)) return GA_DELETE;
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    }
    case 0x34d58:
    turn:
        if (L(0)==0x34d58) {
            SW(0x84,W(0x84)-1u);
            if ((int16_t)W(0x84)<0) {
                SB(0x15,B(0x15)^1u);                                     /* 034E12 */
                if (charge_entry(a,e)!=GA_MORE) return GA_UNSUPPORTED;
                goto charge;
            }
            uint32_t k=k35496(a,e);SB(0x8f,kind_of(k)==5u?0xff:0);
            STEP(r);
            if (ga_cull(a,e,0x34308)) return GA_DELETE;
            return e->unsupported?GA_UNSUPPORTED:GA_MORE;
        }
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    }
    ga_fail(e,L(0));return GA_UNSUPPORTED;
rider_gone:
    SL(0,0x345a0);
    return camel_tick(a,e);
eject:
    /* 034BA0 */
    neo_vslot_exit(v->slots,rider_p(a),a->m,node_word(e,a),1);
    SB(0x89,0xff);SB(0x5f,B(0x5f)|0x80u);SB(0x90,1);
    if (charge_entry(a,e)!=GA_MORE) return GA_UNSUPPORTED;
    goto charge;
}
/* The rider graphic 034F1A / 034F96 and the bomb effect 034FD2 / 035016. */
static ga_result_t rider_tick(neo_gactor_t *a,neo_genv_t *e) {
    neo_gactor_t *c=creator(a,e);int r;
    switch (L(0)) {
    case 0x34f1a:
        ga_place(a,e,0,0,c?NEO_GA_TAG(c->index):0);                      /* 01AD6E */
        SL(0x72,rl(0x23a58eu+4u*W(0x70)));
        ga_palette(a,rw(0x34330u+2u*W(0x70)));ga_palette(a,rw(0x34338u+2u*W(0x70)));
        /* 009CCE: palette fade RAM 1068EA (no node byte), not reproduced. */
        SB(0x14,B(0x19));
        if (!program(a,e,0)) return GA_UNSUPPORTED;
        SL(0,0x34f96);
        /* fall through */
    case 0x34f96:
        ga_project(a,e);
        if (!c || ga_b(c,0x89)) return GA_DELETE;                         /* 004188 */
        SW(0x1e,ga_w(c,0x1e));SW(0x7e,ga_w(c,0x7e));
        if (ga_w(c,0x76)!=W(0x76) && !program(a,e,ga_w(c,0x76))) return GA_UNSUPPORTED;
        STEP(r);(void)r;
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    case 0x34fd2:
        ga_place(a,e,0,0,c?NEO_GA_TAG(c->index):0);
        START(rl(0x23a59eu+4u*W(0x70)));
        ga_palette(a,rw(0x34330u+2u*W(0x70)));
        SL(0,0x35016);
        /* fall through */
    case 0x35016:
        ga_project(a,e);
        if (!c || ga_b(c,0x89)) return GA_DELETE;                         /* cartridge check -> 004188 */
        SW(0x40,W(0x40)+ga_b(c,0x86));
        SW(0x1e,(uint16_t)(ga_w(c,0x1e)-1u));
        STEP(r);
        if (r>0) {ga_sb(c,0x88,0);return GA_DELETE;}
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    case 0x3523c:
        START(0x23b288);ga_palette(a,0x3ad);SW(0x66,0xfff1);SW(0x1e,0xfff1);
        ga_place(a,e,(int16_t)W(0x6e),(int16_t)W(0x70),c?NEO_GA_TAG(c->index):0);   /* 01AD6E */
        SL(0,0x3527e);
        /* fall through */
    case 0x3527e: {
        ga_project(a,e);STEP(r);(void)r;
        if (!e->vehicle) {ga_fail(e,0x3527e);return GA_UNSUPPORTED;}
        if (e->vehicle->system) return GA_DELETE;                        /* 019424 */
        neo_vslot_player_t pl[2]={player(e,0),player(e,1)};
        if (!c || neo_vslot_rider(e->vehicle->slots,node_word(e,c),pl)>=0) return GA_DELETE;
        if (!ga_creator_alive(a,e)) return GA_DELETE;
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    }
    }
    ga_fail(e,L(0));return GA_UNSUPPORTED;
}
/* The vulcan turret 03B4E0 / 03B57C (neo_vulcan.c for the control). */
/* The turret's +6E points into its vehicle (+78 / +86): native tag of the
 * vehicle with the offset in bits 16..23. */
static uint8_t *anchor(neo_gactor_t *a,neo_genv_t *e,unsigned *off) {
    neo_gactor_t *c=creator(a,e);uint32_t t=L(0x6e);
    if (!c || (t&0xff00ffffu)!=NEO_GA_TAG(c->index)) {ga_fail(e,0x3b4e8);return NULL;}
    *off=(t>>16)&0xffu;return c->m+*off;
}
static ga_result_t turret_tick(neo_gactor_t *a,neo_genv_t *e) {
    neo_gactor_t *c=creator(a,e);uint8_t *an;int r;unsigned off=0;
    if (!e->vehicle) {ga_fail(e,L(0));return GA_UNSUPPORTED;}
    switch (L(0)) {
    case 0x3b4e0:
        if (!(an=anchor(a,e,&off))) return GA_UNSUPPORTED;
        {int16_t dx=(int8_t)an[0],dy=(int8_t)an[1];ga_sb(c,off+2u,0);
         ga_place(a,e,dx,dy,c?NEO_GA_TAG(c->index):0);}                  /* 01AB98 */
        SW(0x7c,rw(0x3b4a4u+2u*B(0x72)));
        ga_palette(a,rw(0x3b4c8u+2u*B(0x72)));ga_palette(a,rw(0x3b4d4u+2u*B(0x72)));
        SB(0x14,B(0x19));
        if (!B(0x73)) SB(0x73,2);
        if (!B(0x75)) SB(0x75,5);
        START(0x23ce0c);SL(0,0x3b57c);
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    case 0x3b57c: {
        if (!(an=anchor(a,e,&off))) return GA_UNSUPPORTED;
        neo_vslot_player_t pl[2]={player(e,0),player(e,1)};
        neo_vulcan_in_t in={ga_creator_alive(a,e) && c,c?ga_b(c,0x5f):0,(uint16_t)(an[0]<<8|an[1]),
            c?neo_vslot_rider(e->vehicle->slots,node_word(e,c),pl):-1,0,0,false,NULL};
        if (in.rider>=0) {
            in.fresh=e->vehicle->input[in.rider][0];in.held=e->vehicle->input[in.rider][1];
            in.repeat=e->vehicle->repeat[in.rider]!=0;
        }
        in.aim=ga_rom(rl(0x3b4b0u+4u*B(0x72)));
        if (!in.aim) {ga_fail(e,0x3b4b0);return GA_UNSUPPORTED;}
        uint8_t before[0xc0];memcpy(before,a->m,0xc0);
        neo_vulcan_out_t out;bool spark;
        neo_vulcan_step(a->m,&in,false,&out,&spark);
        for (unsigned o=0;o<0xc0;o++) if (a->m[o]!=before[o]) ga_sb(a,o,a->m[o]);
        if (out.state==NEO_VULCAN_GONE) {ga_fail(e,0x3b86e);return GA_UNSUPPORTED;}   /* explicit stop */
        if (out.state==NEO_VULCAN_HOLD) return GA_MORE;
        if (out.angle_set) neo_vslot_angle(e->vehicle->slots,(unsigned)in.rider,out.angle);
        if (out.fire) {
            neo_gactor_t *b=e->spawn?e->spawn(e->ctx,NULL,out.bullet,true):NULL;   /* 00411C */
            if (b) {
                ga_sb(b,0x5e,out.bullet5e);ga_sb(b,0x6e,out.bullet6e);
                uint32_t vx,vy;ga_polar(0x1c0000u,out.heading,&vx,&vy);
                uint16_t d6=ga_sweep_place_d0(a,e,b,(int16_t)(vx>>16),(int16_t)(vy>>16),0xff);
                if (d6 && !W(0x84)) {SW(0x84,0xff);ga_fail(e,0x3ba76);return GA_UNSUPPORTED;}   /* spark: explicit stop */
            }
        }
        /* 03B80C: on the anchor, flash from the vehicle, program. */
        SW(0x1e,c?ga_w(c,0x1e):W(0x1e));
        SW(0x28,(uint16_t)(int16_t)(int8_t)an[0]);SW(0x2c,(uint16_t)(int16_t)(int8_t)an[1]);
        ga_project(a,e);
        if (c && ga_b(c,0x17)) {
            uint8_t f=ga_b(c,0x17);if (!(f&0x80u)) f++;
            SB(0x17,f);if (!(f&1u)) SB(0x14,B(0x1a));                       /* 0044F6 */
        }
        SB(0x17,0xff);
        STEP(r);(void)r;
        SB(0x14,B(0x19));
        if (an[2]) {ga_sb(c,off+2u,0);SB(0x14,B(0x1a));}
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    }
    }
    ga_fail(e,L(0));return GA_UNSUPPORTED;
}
bool neo_slug_routine(uint32_t r) {
    switch (r) {
    case 0x3458e: case 0x345a0: case 0x345e0: case 0x34698: case 0x34868: case 0x346ea: case 0x346fa:
    case 0x34786: case 0x3478c: case 0x3479c: case 0x34852: case 0x348d8: case 0x348f0: case 0x349be:
    case 0x349c6: case 0x349d8: case 0x34bf0: case 0x34d58:
    case 0x34f1a: case 0x34f96: case 0x34fd2: case 0x35016: case 0x3523c: case 0x3527e:
    case 0x3b4e0: case 0x3b57c: return true;
    }
    return false;
}
ga_result_t neo_slug_tick(neo_gactor_t *a,neo_genv_t *e) {
    uint32_t r=L(0);
    if (r==0x3b4e0 || r==0x3b57c) return turret_tick(a,e);
    if (r==0x34f1a || r==0x34f96 || r==0x34fd2 || r==0x35016 || r==0x3523c || r==0x3527e) return rider_tick(a,e);
    return camel_tick(a,e);
}
