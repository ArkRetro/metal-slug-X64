#include "neo_region.h"
/* SV-001 092A32 (the player's tank of Mission 1, camera 2336, script record
 * 2D6E80, uncounted) and its own children: manual translation of ROM
 * 092810..093AFA, 094EAA..09503C, 0952F8..095448, 0954DC..095580 and
 * 0956CE..095790; ROM labels are provenance only. The vulcan turret
 * 03B4E0/03B57C (+72 = 0) is the Camel Slug's (neo_slug.c), the tread body
 * 014A4E..0153EE is neo_tread.c.
 *
 * Tank fields: +4E health (0x30), +50 hurtbox (092832: 0927CA in the air,
 * 092742 / 092786 by +77, indexed by +74), +5A/+5F damage, +6C kind word,
 * +70 the record of the pose (09280E/092820 from the programs: x/y of the
 * turret anchor, bomb offset, cannon offset), +74/+75 pose indices from the
 * angle +84 (092872..0928EA), +76 the angle is on a table step, +77 the
 * program's switch (shots in a row), +78 moving, +79/+7A/+7D pose memories,
 * +7B shells, +7C bomb kind, +7E flicker (+7E word 8 in the wait), +80 the
 * rider, +82 the rider's +6E, +84 the tread angle, +86/+87 the turret anchor
 * (0939BA), +8A the cannon's cool-down, +8C the tread node, +90 the palette
 * list 093882 (009D1E: the palette of the slot +14 from the list), +94/+95
 * dust.
 *
 * States: 092A32 constructor (palettes 0x3D3/0x39D/0x203, 0146E8, the
 * tread 014AE8, health 0x30, kind 4, ten shells, the turret 03B4E0 with +6E
 * = tank+86), 092AD8 (program 291CE4, the marker 0353A4(0, 0x20)), 092B0E
 * wait (093846, 092832, 0C8BEC 092A1A -> 004188, 0AA610 rectangle 092A2A,
 * 0AA760 (0, 0), 0AA83A kind 1) -> 092C18 mount (the entry 0954DC; its
 * absence: 092C38 waits +7E = 5 with 09380A) -> 092C76 (program 291DC2
 * until its end) -> 093278 stand-by (sound 1092, program 29282A) ->
 * 092C9A stand / 092CC2, 092DDA drive (+62 = +-0x2A000, programs by the
 * direction and +77, sounds of the dust 0956CE by +95), 092F34 / 092F94
 * stop, 093054 / 09307C fire (program 29A202), 093A08 / 093A46 exit (A+B:
 * sound 10E9, 0AA7FC kind 1, program 2964C8, +62 towards 0xC0000, attack
 * rows 0939C8 / 0939D8, cull 092A1A -> 093AFA), and the common tail 093692
 * (cool-down, +5F bit 6, the palette list, flicker +7E, RAM 10F000 bit 7,
 * 0AA760: no rider -> 092AD8; 0AAA88 kind 1 copies the shells/bomb kind;
 * 0AA9E4 damage; on the ground and moving the push row 0939E8; 0939BA).
 * Cannon 092934 (C): +8A = 0 and 0AA956 -> +8A = 0x18, the flash 0952F8 at
 * the record's cannon offset, the shell 094EAA (0950DC with kind 5: an
 * explicit stop) with +5E and +84, placed by 014A02 (D0 = FF).
 *
 * Explicit stops (no capture holds them): destruction 0938DA / 093AFA (the
 * tank killed: 0353C2's child, 0AE220, 0AD9E6 pieces), the damage reaction
 * 09351A, the jump 09333A (sound 1090), the air 09339E / 0933C2 / 09347A,
 * down 093138 / 09315E (sound 108F), 093204 / 09321A, 0937C6 (exit with
 * +77 >= 5), the bombs 0929E6 (0C0808) and the kind-5 shell 0950DC. */
#define B(o) ga_b(a,(o))
#define W(o) ga_w(a,(o))
#define L(o) ga_l(a,(o))
#define SB(o,v) ga_sb(a,(o),(uint8_t)(v))
#define SW(o,v) ga_sw(a,(o),(uint16_t)(v))
#define SL(o,v) ga_sl(a,(o),(uint32_t)(v))
#define BIT(o,b) ((B(o)>>(b))&1u)
#define START(t) do { if (!ga_anim_start(a,e,(t),0x100)) return GA_UNSUPPORTED; } while (0)
#define STOP(at) do { ga_fail(e,(at));return GA_UNSUPPORTED; } while (0)
static uint8_t rb(uint32_t address) {const uint8_t *p=ga_rom(address);return p?p[0]:0;}
static uint16_t rw(uint32_t address) {const uint8_t *p=ga_rom(address);return p?(uint16_t)(p[0]<<8|p[1]):0;}
static uint32_t rl(uint32_t address) {return (uint32_t)rw(address)<<16|rw(address+2);}
static int16_t S16(uint16_t v) {return (int16_t)v;}
static void sound(neo_genv_t *e,uint16_t id) {if (e->sound) e->sound(e->ctx,id);}
static neo_gactor_t *creator(neo_gactor_t *a,neo_genv_t *e) {return e->creator?e->creator(e->ctx,a):NULL;}
static uint16_t node_word(neo_genv_t *e,const neo_gactor_t *a) {return e->node_word?e->node_word(e->ctx,a):(uint16_t)(0x8000u|(uint16_t)a->index);}
/* 093C9A (Mission 3, docs/PLAN_MISION3.md step 5): the same code relocated,
 * a separate copy of the SV-001: the states 092BCA..093AFE at +0x11E0
 * (093DAA..094CDE, their tables 094A62/094B9A.. with them), the
 * constructor and the wait 092A32/092AD8/092B0E at +0x1268 (093C9A/093D40/
 * 093D76); other palettes (0x87D/0x39D/0x87A), the turret 03B4E0 with +72
 * = 4, drive speed 0x40000, air steering +-0x40000 (>> 2), gravity target
 * 0xFFA3D740; the shells, flashes, entries, dust, tread and burst are the
 * shared originals. tank_tick works on the original labels: orig() maps
 * a stored routine back, lab() the labels it stores and the tables it reads. */
static bool v3;
static uint32_t lab(uint32_t x) {
    if (!v3) return x;
    if (x==0x92a32u || x==0x92ad8u || x==0x92b0eu || x==0x93bc2u) return x+0x1268u;   /* the burst too: 094E2A */
    if (x>=0x92bcau && x<=0x93afeu) return x+0x11e0u;
    return x;
}
static uint32_t orig(uint32_t r) {
    if (r==0x93c9au || r==0x93d40u || r==0x93d76u || r==0x94e2au || r==0x94e5au) return r-0x1268u;
    if (r>=0x93daau && r<=0x94cdeu) return r-0x11e0u;
    return r;
}
static bool tank_v3(uint32_t r) {return r>=0x93c9au && r<=0x94cdeu;}
#define SPEED (v3?0x40000u:0x2a000u)
#define NSPEED (v3?0xfffc0000u:0xfffd6000u)
#define GRAVITY_TARGET (v3?0xffa3d740u:0xffdc7200u)
static void touched(neo_gactor_t *a) {static const uint8_t o[]={0x4e,0x4f,0x5e,0x5f};for (unsigned i=0;i<sizeof(o);i++) ga_sb(a,o[i],a->m[o[i]]);}
static unsigned rider_p(const neo_gactor_t *a) {return (ga_w(a,0x80)&0xffu)?1u:0u;}   /* tst.b d0 */
static neo_vslot_player_t player(neo_genv_t *e,unsigned p) {return (neo_vslot_player_t){e->vehicle->exists[p],e->vehicle->player[p]};}
static uint16_t pad(neo_gactor_t *a,neo_genv_t *e) {return e->vehicle->input[rider_p(a)][0];}  /* 0CA2CE +0 */
static uint16_t rng(void *ctx) {return ga_rand((neo_genv_t *)ctx);}
/* Program callbacks: 09280E / 092820 (A0 = the table after the call: its
 * address's low word in the bank 29xxxx), 299178 (+15 bit 6 cleared). */
static bool call(neo_gactor_t *a,neo_genv_t *e,neo_anim_kind_t kind,uint32_t routine,uint16_t parameter) {
    if (kind!=NEO_ANIM_CALL) return false;
    switch (routine) {
    case 0x9280e: SL(0x70,rl(0x290000u+parameter+4u*B(0x74)));return true;
    case 0x92820: SL(0x70,rl(0x290000u+parameter+4u*B(0x75)));return true;
    case 0x299178: SB(0x15,B(0x15)&~0x40u);return true;
    }
    (void)e;return false;
}
/* 092832: hurtbox, moving flag, pose indices of the angle +84, then 00427A
 * (true: the program ended). */
static int k92832(neo_gactor_t *a,neo_genv_t *e) {
    uint32_t t=!BIT(0x15,6)?0x927ca:B(0x77)<1u?0x92742:0x92786;
    SL(0x50,rl(t+4u*B(0x74)));
    SB(0x78,L(0x30)?0xff:0);
    uint16_t d0=(uint16_t)(S16(W(0x84))>>5);
    if (!(d0&0x8000u)) {
        if (!d0) d0=0; else d0=(uint16_t)((d0-1u)&0xffu);
        d0=(uint16_t)(d0+1u);
    } else d0=(uint16_t)~(((uint16_t)~d0)&0xe0u);
    d0=(uint16_t)((uint16_t)(d0+0x100u)>>5);SB(0x74,(uint8_t)d0);
    int16_t d1=S16(W(0x84));
    SB(0x75,d1<S16(0xeb00)?0:d1<S16(0xfb00)?1:d1<=0x500?2:d1<=0x1500?3:4);
    uint16_t w=(uint16_t)d1;if (d1<0) w=(uint16_t)(w+0x1ffu);
    w=(uint16_t)((uint16_t)(S16(w)>>1)&0xff00u);
    SB(0x76,(w==0x1000u||w==0x500u||w==0||w==0xfb00u||w==0xf000u)?0xff:0);
    return ga_anim_step(a,e,call);
}
/* 0939BA: the turret's anchor from the pose record. */
static void anchor(neo_gactor_t *a) {SB(0x86,rb(L(0x70)));SB(0x87,rb(L(0x70)+1u));}
/* 09380A / 093846: the tread step (014C48 / 014C6A on the ground, 014C0C /
 * 014C2E in the air with +30 = +62 and +34 towards 0xFFDC7200). */
static bool tread(neo_gactor_t *a,neo_genv_t *e,bool ride) {
    unsigned entry;
    if (BIT(0x15,6)) entry=ride?2:3;
    else {
        SL(0x30,L(0x62));
        SL(0x34,L(0x34)+(uint32_t)((int32_t)(GRAVITY_TARGET-L(0x34))>>6));
        entry=ride?0:1;
    }
    int32_t r=neo_tread_step(a,e,entry);
    if (r<0) return false;
    SW(0x84,(uint16_t)r);return true;
}
/* 092934: the cannon (C). True when fired. */
static bool k92934(neo_gactor_t *a,neo_genv_t *e) {
    if (W(0x8a)) return false;
    if (!neo_vslot_shell(e->vehicle->slots,rider_p(a),(e->vehicle->dip10f001&4u)!=0)) return false;   /* 0AA956 */
    SW(0x8a,0x18);
    uint32_t rec=L(0x70);int16_t dx=(int8_t)rb(rec+4u),dy=(int8_t)rb(rec+5u);
    neo_gactor_t *f=e->spawn?e->spawn(e->ctx,a,0x952f8,false):NULL;
    if (f) {ga_place_at(f,a,e,0,0);ga_sw(f,0x30,(uint16_t)dx);ga_sw(f,0x34,(uint16_t)dy);ga_sw(f,0x1e,W(0x1e));}
    if (B(0x7c)==5u) {ga_fail(e,0x950dc);return true;}                 /* kind-5 shell: explicit stop */
    neo_gactor_t *s=e->spawn?e->spawn(e->ctx,a,0x94eaa,false):NULL;
    if (s) {ga_sb(s,0x5e,B(0x5e));ga_sw(s,0x6e,W(0x84));ga_sweep_place_d0(a,e,s,dx,dy,0xff);}   /* 014A02 */
    return true;
}
/* 009D1E: the hardware palette slot +14 takes the resource d1 (RAM
 * 1068EA slot table; here the actor's virtual slot +14 of a->palette[]). */
static void pal_swap(neo_gactor_t *a,uint16_t d1) {uint8_t s=B(0x14);if (s>=1u && s<=6u) a->palette[s-1u]=d1;}
/* 0929E6: a bomb (crouched, C): +8A = 6 and 0C0808 for the rider with
 * the pose record's bomb offset (+70 bytes 2, 3): the bomb type's
 * constructor (table 0C4756, 0C476E for the character 2; +4 with the
 * player's +90 = FF) after the script actor, 014A02 (D0 = 0) at the offset
 * and the tank's velocity in the bomb's frame (0C482E). */
static void k929e6(neo_gactor_t *a,neo_genv_t *e) {
    if (W(0x8a)) return;
    SW(0x8a,6);
    unsigned p=rider_p(a);
    marco_weapons_t *w=e->pickup?e->pickup->weapons:NULL;
    uint8_t *pm=e->vehicle->player[p];
    if (!w || !pm) {ga_fail(e,0xc0808);return;}
    if (!marco_weapons_throw(w,p,(e->vehicle->dip10f001&4u)!=0)) return;   /* 0C0824 */
    uint8_t type=w->b[10+p];
    if (!type) return;                                                  /* 0C08A2 */
    unsigned idx=(unsigned)(type-1u);if (idx>=3u) idx=0;
    uint32_t table=pm[0x93]==2?0xc476eu:0xc4756u;
    uint32_t ctor=rl(table+idx*8u+(pm[0x90]==0xffu?4u:0u));
    neo_gactor_t *c=ctor&&e->spawn?e->spawn(e->ctx,NULL,ctor,true):NULL;
    if (!c) return;
    ga_sb(c,0x5e,B(0x5e));ga_sw(c,0x6e,(uint16_t)p);
    uint32_t rec=L(0x70);
    ga_sweep_place_d0(a,e,c,(int16_t)(int8_t)rb(rec+2u),(int16_t)(int8_t)rb(rec+3u),0);
    uint32_t vx=L(0x30),vy=L(0x34);neo_contact_parent_t q;
    if (L(0x38) && ga_parent(e,L(0x38),&q)) {vx+=(uint32_t)q.x-(uint32_t)q.previous_x;vy+=(uint32_t)q.y-(uint32_t)q.previous_y;}
    if (ga_l(c,0x38) && ga_parent(e,ga_l(c,0x38),&q)) {vx-=(uint32_t)q.x-(uint32_t)q.previous_x;vy-=(uint32_t)q.y-(uint32_t)q.previous_y;}
    ga_sl(c,0x30,vx);ga_sl(c,0x34,vy);
    e->vehicle->grenades++;e->vehicle->grenade_last=ctor;
}
/* 014C0C called directly (the jump and the air): +84 from the tread. */
static bool air_step(neo_gactor_t *a,neo_genv_t *e) {
    int32_t r=neo_tread_step(a,e,0);
    if (r<0) return false;
    SW(0x84,(uint16_t)r);return true;
}
/* 0933EC / 093602: the stick steers +30 in the air. */
static void air_steer(neo_gactor_t *a,uint16_t d0) {
    uint32_t d7=0;unsigned d6=6;
    if (d0&4u) {d7=v3?0xfffc0000u:0xfffd4000u;d6=v3?2:3;}
    if (d0&8u) {d7=v3?0x40000u:0x2c000u;d6=v3?2:3;}
    SL(0x30,L(0x30)+(uint32_t)((int32_t)(d7-L(0x30))>>d6));
}
static void gravity(neo_gactor_t *a) {SL(0x34,L(0x34)+(uint32_t)((int32_t)(GRAVITY_TARGET-L(0x34))>>6));}
/* 093692: the common tail. */
static ga_result_t tail(neo_gactor_t *a,neo_genv_t *e) {
    neo_vehicle_env_t *v=e->vehicle;
    if (W(0x8a)) SW(0x8a,W(0x8a)-1u);
    if (BIT(0x5f,6)) {SB(0x5f,B(0x5f)&~0x40u);if (B(0x5a)!=0x0fu) {SB(0x5f,B(0x5f)&0x7fu);SB(0x5a,0);}}
    if (L(0x90)) {
        SB(0x14,B(0x1a));uint32_t p=L(0x90);
        uint16_t d1=rw(p);p+=2u;
        if (d1==0xffffu) {p+=(uint32_t)(int32_t)S16(rw(p));d1=rw(p);p+=2u;}
        SL(0x90,p);pal_swap(a,d1);                                      /* 009D1E */
        SB(0x5f,B(0x5f)|0x40u);
    } else {
        SB(0x14,B(0x1b));
        if (B(0x7e)) {
            uint8_t d0=(uint8_t)(B(0x7e)-1u);SB(0x7e,d0);
            if (d0&1u) {SB(0x14,B(0x19));SB(0x88,0xff);}
            SB(0x5f,B(0x5f)|0x40u);
        }
    }
    if (v->dip10f000&0x80u) SB(0x5f,B(0x5f)|0x40u);
    unsigned p=rider_p(a);
    neo_vslot_player_t pl=player(e,p);uint16_t p6e;
    if (!neo_vslot_bind(v->slots,p,a->m,node_word(e,a),&pl,e->video,0,0,&p6e)) SL(0,lab(0x92ad8));
    else {
        uint8_t kind,shells,type;uint16_t health,d4;
        if (neo_vslot_ridden(v->slots,p,v->exists[p],&kind,&shells,&type,&health,&d4) && kind==1u) {SB(0x7b,shells);SB(0x7c,type);}
    }
    touched(a);
    bool hit=ga_register(a,e,NULL,NULL);                                /* 0AA9E4 with 004C24 */
    bool carry=neo_vslot_damage(v->slots,p,a->m,hit);touched(a);
    if (carry || BIT(0x5f,7)) {
        SB(0x5f,B(0x5f)|0x40u);
        SL(0,lab(B(0x5a)==0x0fu?0x93afa:0x9351a));
    }
    if (BIT(0x15,6) && L(0x62)) ga_attack(a,e,lab(0x939e8));
    anchor(a);
    return e->unsupported?GA_UNSUPPORTED:GA_MORE;
}
/* 092D24 / 092D4C / 092D52 / 092D74 / 092D9C / 092DA2: +95 (the dust
 * count; 0: left as it is), +62, the drive program by +77 (092DC4), +7A =
 * +76, then 092DDA in the same update. */
static bool drive(neo_gactor_t *a,neo_genv_t *e,uint8_t dust,uint32_t v62,uint32_t prog,uint32_t prog7) {
    if (dust) SB(0x95,dust);
    SL(0x62,v62);
    if (!ga_anim_start(a,e,B(0x77)>=7u?prog7:prog,0x100)) return false;
    SB(0x7a,B(0x76));SL(0,lab(0x92dda));
    return true;
}
#define D24 drive(a,e,5,SPEED,0x294c6c,0x29667e)
#define D4C drive(a,e,3,SPEED,0x2952ec,0x296cfe)
#define D52 drive(a,e,0,SPEED,0x2952ec,0x296cfe)
#define D74 drive(a,e,5,NSPEED,0x29589a,0x2972ac)
#define D9C drive(a,e,3,NSPEED,0x295f1a,0x29792c)
#define DA2 drive(a,e,0,NSPEED,0x295f1a,0x29792c)
#define GO(x) do { if (!(x)) return GA_UNSUPPORTED; goto next; } while (0)
static ga_result_t tank_tick(neo_gactor_t *a,neo_genv_t *e) {
    if (!e->vehicle) STOP(L(0));
    neo_vehicle_env_t *v=e->vehicle;
    uint16_t d0;int r;
    v3=tank_v3(L(0));
    for (int guard=0;guard<8;guard++) { switch (orig(L(0))) {
    case 0x92a32: {
        ga_palette(a,v3?0x87d:0x3d3);ga_palette(a,0x39d);ga_palette(a,v3?0x87a:0x203);
        SW(0x66,0x8810);SW(0x1e,0x8810);SW(0x84,0);
        if (!W(0x6e)) ga_floor(a,e);
        neo_gactor_t *c=neo_tread_spawn(a,e);
        SL(0x8c,c?NEO_GA_TAG(c->index):0);
        SW(0x4e,0x30);SB(0x7c,4);SB(0x7b,10);
        neo_gactor_t *t=e->spawn?e->spawn(e->ctx,a,0x3b4e0,false):NULL;
        if (t) {ga_sl(t,0x6e,NEO_GA_TAG(a->index)|(0x86u<<16));ga_sb(t,0x72,v3?4:0);ga_sb(t,0x73,0);ga_sb(t,0x74,0);ga_sb(t,0x75,0);}
        SL(0,lab(0x92ad8));
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    }
    case 0x92ad8: {
        SB(0x14,B(0x1b));SL(0x90,0);START(0x291ce4);SL(0x62,0);
        neo_gactor_t *c=e->spawn?e->spawn(e->ctx,a,0x3523c,false):NULL;   /* 0353A4 */
        if (c) {ga_sw(c,0x6e,0);ga_sw(c,0x70,0x20);}
        SL(0,lab(0x92b0e));
    }   /* fall through */
    case 0x92b0e: {
        if (!W(0x4e)) STOP(lab(0x938da));
        if (BIT(0x5f,7)) {SL(0,lab(0x93afa));continue;}
        SB(0x5a,0);SW(0x7e,8);
        if (!tread(a,e,false)) return GA_UNSUPPORTED;
        if (k92832(a,e)<0) return GA_UNSUPPORTED;
        if (ga_outside_view(a,e,0x92a1a)) return GA_DELETE;            /* 0C8BEC -> 004188 */
        int16_t rect[4];for (unsigned i=0;i<4;i++) rect[i]=(int16_t)rw(0x92a2au+2u*i);
        neo_vslot_player_t pl[2]={player(e,0),player(e,1)};
        int p=neo_vslot_select(v->slots,a->m,pl,rect,v->halted!=0,v->system!=0,rng,e);
        if (p>=0) {
            SW(0x80,(uint16_t)p);uint16_t p6e;
            if (neo_vslot_bind(v->slots,(unsigned)p,a->m,node_word(e,a),&pl[p],e->video,0,0,&p6e)) {
                SW(0x82,p6e);
                neo_vslot_mount(v->slots,(unsigned)p,a->m,1,B(0x7b),B(0x7c),0x30);
                SL(0,lab(0x92c18));
            }
            touched(a);
        }
        anchor(a);
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    }
    case 0x92c18: {
        SW(0x8a,0);
        neo_gactor_t *c=e->spawn?e->spawn(e->ctx,a,0x954dc,false):NULL;
        SB(0x60,c?0:0xff);                                              /* scc: 0040BC sets the carry when made */
        SL(0,lab(0x92c38));
    }   /* fall through */
    case 0x92c38:
        SB(0x7e,0xf);
        if (!B(0x60)) {
            SB(0x7e,5);SL(0x62,0);
            if (!tread(a,e,true)) return GA_UNSUPPORTED;
            if (k92832(a,e)<0) return GA_UNSUPPORTED;
            return tail(a,e);
        }
        START(0x291dc2);SL(0,lab(0x92c76));
        /* fall through */
    case 0x92c76:
        SB(0x7e,0xf);SL(0x62,0);
        if (!tread(a,e,true)) return GA_UNSUPPORTED;
        if ((r=k92832(a,e))<0) return GA_UNSUPPORTED;
        if (r) SL(0,lab(0x93278));
        return tail(a,e);
    case 0x92c9a:
        SL(0x90,0);
        if (B(0x77)>=7u) {SL(0,lab(0x93204));continue;}
        START(0x2942d4);SL(0,lab(0x92cc2));
        /* fall through */
    case 0x92cc2:
        if (!BIT(0x15,6)) {SL(0,lab(0x9339e));continue;}
        d0=pad(a,e);
        if (d0&8u) GO(D24);
        if (d0&4u) GO(D74);
        if (d0&0x80u) {SL(0,lab(0x93a08));continue;}
        if (d0&0x20u) {SL(0,lab(0x9333a));continue;}
        if (d0&2u) {SL(0,lab(0x93138));continue;}
        if ((d0&0x40u) && k92934(a,e)) {if (e->unsupported) return GA_UNSUPPORTED;SL(0,lab(0x93054));continue;}
        SL(0x62,0);
        if (!tread(a,e,true)) return GA_UNSUPPORTED;
        if (k92832(a,e)<0) return GA_UNSUPPORTED;
        return tail(a,e);
    case 0x92dda:
        d0=pad(a,e);
        if (d0&1u) SB(0x5f,B(0x5f)|0x20u); else SB(0x5f,B(0x5f)&~0x20u);
        if (d0&2u) SB(0x5f,B(0x5f)|0x10u); else SB(0x5f,B(0x5f)&~0x10u);
        if (d0&8u) {if ((int32_t)L(0x62)<=0) GO(D4C);}
        else if (d0&4u) {if ((int32_t)L(0x62)>=0) GO(D9C);}
        else {
            /* 092F34 */
            SB(0x7a,B(0x76));
            if (!B(0x7a)) {SL(0,lab(0x92c9a));continue;}
            SB(0x7d,(L(0x62)&0x80000000u)?0xff:0);
            START(B(0x77)>=7u?(B(0x7d)&0x80u?0x298d3e:0x298904):(B(0x7d)&0x80u?0x2984ca:0x298090));
            SL(0x62,0);SL(0,lab(0x92f94));continue;
        }
        if (B(0x77)>=5u) {
            if (!(d0&2u)) {SL(0,lab(0x93278));continue;}
            if (d0&0x80u) {SL(0,lab(0x93a08));continue;}
            if (d0&0x20u) {SL(0,lab(0x937c6));continue;}
            if (d0&0x40u) k929e6(a,e);
        } else {
            if (d0&0x80u) {SL(0,lab(0x93a08));continue;}
            if (d0&2u) {SL(0,lab(0x93138));continue;}
            if (d0&0x20u) {SL(0,lab(0x9333a));continue;}
            if ((d0&0x40u) && k92934(a,e)) {if (e->unsupported) return GA_UNSUPPORTED;SL(0,lab(0x93054));continue;}
        }
        if (!tread(a,e,true)) return GA_UNSUPPORTED;
        {
            uint8_t n=B(0x76),old=B(0x7a);SB(0x7a,n);
            if ((uint8_t)(n^old)&0x80u) START(L(0x62)&0x80000000u?(B(0x77)>=7u?0x29792c:0x295f1a):(B(0x77)>=7u?0x296cfe:0x2952ec));
        }
        if (k92832(a,e)<0) return GA_UNSUPPORTED;
        if (!BIT(0x15,6)) SL(0,lab(0x9339e));
        if (B(0x95)) {
            uint8_t s=(uint8_t)(B(0x95)<<4),before=B(0x94);SB(0x94,(uint8_t)(before-s));
            if (before<s) {
                SB(0x95,B(0x95)-1u);
                if (e->spawn) (void)e->spawn(e->ctx,a,0x956ce,false);
            }
        }
        return tail(a,e);
    case 0x92f94:
        if (!BIT(0x15,6)) {SL(0,lab(0x9339e));continue;}
        d0=pad(a,e);
        if (d0&8u) GO(!(B(0x7d)&0x80u)?D24:DA2);
        if (d0&4u) GO(!(B(0x7d)&0x80u)?D52:D74);
        if (B(0x77)>=5u) {
            if (!(d0&2u)) {SL(0,lab(0x93278));continue;}
            if (d0&0x80u) {SL(0,lab(0x93a08));continue;}
            if (d0&0x20u) {SL(0,lab(0x937c6));continue;}
            if (d0&0x40u) k929e6(a,e);
        } else {
            if (d0&0x80u) {SL(0,lab(0x93a08));continue;}
            if (d0&2u) {SL(0,lab(0x93138));continue;}
            if (d0&0x20u) {SL(0,lab(0x9333a));continue;}
            if ((d0&0x40u) && k92934(a,e)) {if (e->unsupported) return GA_UNSUPPORTED;SL(0,lab(0x93054));continue;}
        }
        {uint8_t n=B(0x76),old=B(0x7a);SB(0x7a,n);if ((uint8_t)(n^old)&0x80u) {SL(0,lab(0x92c9a));continue;}}
        if (!tread(a,e,true)) return GA_UNSUPPORTED;
        if ((r=k92832(a,e))<0) return GA_UNSUPPORTED;
        if (r) SL(0,lab(0x92c9a));
        return tail(a,e);
    case 0x93054:
        SB(0x77,0);SB(0x7a,B(0x76));SB(0x79,0xff);START(0x29a202);SL(0,lab(0x9307c));
        /* fall through */
    case 0x9307c:
        if (!BIT(0x15,6)) {SL(0,lab(0x9339e));continue;}
        d0=pad(a,e);
        if (B(0x79)) SL(0x62,(d0&8u)?SPEED:(d0&4u)?NSPEED:0);
        else {
            if (d0&2u) {SL(0,lab(0x93138));continue;}
            if (d0&8u) GO(D52);
            if (d0&4u) GO(DA2);
            SL(0x62,0);
        }
        if (d0&0x80u) {SL(0,lab(0x93a08));continue;}
        if (d0&0x20u) {SL(0,lab(0x9333a));continue;}
        if ((d0&0x40u) && k92934(a,e)) {if (e->unsupported) return GA_UNSUPPORTED;SL(0,lab(0x93054));continue;}
        if (!tread(a,e,true)) return GA_UNSUPPORTED;
        if ((r=k92832(a,e))<0) return GA_UNSUPPORTED;
        if (r || (B(0x7a) && !B(0x76))) SL(0,lab(0x92c9a));
        return tail(a,e);
    case 0x93278:
        sound(e,0x1092);SB(0x7a,B(0x76));START(0x29282a);SL(0,lab(0x9329e));
        /* fall through */
    case 0x9329e:
        if (!BIT(0x15,6)) {SL(0,lab(0x9339e));continue;}
        if (B(0x76)!=B(0x7a)) {SB(0x7a,B(0x76));START(0x29282a);}
        d0=pad(a,e);
        SL(0x62,(d0&8u)?SPEED:(d0&4u)?NSPEED:0);
        if (d0&0x80u) {SL(0,lab(0x93a08));continue;}
        if (d0&0x20u) {SL(0,lab(0x9333a));continue;}
        if (d0&2u) {SL(0,lab(0x93138));continue;}
        if (d0&0x40u) {SL(0,lab(0x93054));continue;}
        if (!tread(a,e,true)) return GA_UNSUPPORTED;
        if ((r=k92832(a,e))<0) return GA_UNSUPPORTED;
        if (r) SL(0,lab(0x92c9a));
        return tail(a,e);
    case 0x93a08:
        sound(e,0x10e9);SL(0x90,lab(0x93882));START(0x2964c8);
        neo_vslot_exit(v->slots,rider_p(a),a->m,node_word(e,a),1);touched(a);
        SL(0x62,0);SL(0,lab(0x93a46));
        /* fall through */
    case 0x93a46: {
        if (L(0x90)) {
            SB(0x14,B(0x1a));uint32_t p=L(0x90);uint16_t d1=rw(p);p+=2u;
            if (d1==0xffffu) {p+=(uint32_t)(int32_t)S16(rw(p));d1=rw(p);p+=2u;}
            SL(0x90,p);pal_swap(a,d1);                                  /* 009D1E */
        }
        SL(0x62,L(0x62)+(uint32_t)((int32_t)(0xc0000u-L(0x62))>>5));
        if (!tread(a,e,false)) return GA_UNSUPPORTED;
        SW(0x86,0x8000);
        if (k92832(a,e)<0) return GA_UNSUPPORTED;
        bool gone=BIT(0x5f,0) || S16(W(0x3c))>0x120;
        if (!gone) {
            uint8_t t=ga_attack(a,e,lab(0x939c8));
            if (t<6u && (t==2u || t==3u)) gone=true;
            else if (ga_cull(a,e,0x92a1a)) gone=true;
        }
        if (gone) {ga_attack(a,e,lab(0x939d8));SL(0,lab(0x93afa));}
        anchor(a);
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    }
    case 0x93afa: {
        /* 093AFA: the empty (or killed) tank blows up: sound 1034, the
         * attack row 0939F8, +5A = 7, 0AA7FC kind 0 (a rider still bound
         * dies), the flash 0AE220(1, 0) (0AE1DC), four pieces 0AD9E6
         * (0AD9B8 at the tank with their +30/+34/+4E), the burst 093BC2
         * 16 px up (00411C, 01ACF2) and 004188. */
        sound(e,0x1034);
        ga_attack(a,e,lab(0x939f8));
        SB(0x5a,7);
        neo_vslot_exit(v->slots,rider_p(a),a->m,node_word(e,a),0);touched(a);
        neo_gactor_t *f=e->spawn?e->spawn(e->ctx,a,0xae1dc,true):NULL;
        if (f) {ga_sb(f,0x70,0);ga_sw(f,0x6e,1);}
        static const uint32_t piece[4][3]={{0xfff20000u,0x280000u,0xa},{0xa0000u,0x250000u,7},{0xfff70000u,0x10000u,7},{0x70000u,0xfffc0000u,5}};
        for (unsigned i=0;i<4;i++) {
            neo_gactor_t *q=e->spawn?e->spawn(e->ctx,a,0xad9b8,true):NULL;
            if (!q) continue;
            ga_place_at(q,a,e,0,0);
            ga_sl(q,0x30,piece[i][0]);ga_sl(q,0x34,piece[i][1]);ga_sw(q,0x4e,(uint16_t)piece[i][2]);
        }
        neo_gactor_t *b=e->spawn?e->spawn(e->ctx,a,lab(0x93bc2),true):NULL;
        if (b) ga_place_at(b,a,e,0,0x10);
        return e->unsupported?GA_UNSUPPORTED:GA_DELETE;
    }
    case 0x9351a: STOP(lab(0x9351a));
    case 0x93138:
        /* 093138: crouch (sound 108F, program 2934EC; restarted when +76
         * changes); the end -> 093204 (+8A = 0). */
        sound(e,0x108f);SB(0x7a,B(0x76));START(0x2934ec);SL(0,lab(0x9315e));
        /* fall through */
    case 0x9315e:
        if (B(0x76)!=B(0x7a)) {SB(0x7a,B(0x76));START(0x2934ec);}
        if (!BIT(0x15,6)) {SL(0,lab(0x9339e));continue;}
        d0=pad(a,e);
        SL(0x62,(d0&8u)?SPEED:(d0&4u)?NSPEED:0);
        if (d0&0x80u) {SL(0,lab(0x93a08));continue;}
        if (d0&0x20u) {SL(0,lab(0x937c6));continue;}
        if (!(d0&2u)) {SL(0,lab(0x93278));continue;}
        if (d0&0x40u) k929e6(a,e);
        if (!tread(a,e,true)) return GA_UNSUPPORTED;
        if ((r=k92832(a,e))<0) return GA_UNSUPPORTED;
        if (r) {SW(0x8a,0);SL(0,lab(0x93204));}
        return tail(a,e);
    case 0x93204:
        START(0x2947a0);SL(0,lab(0x9321a));
        /* fall through */
    case 0x9321a:
        if (!BIT(0x15,6)) {SL(0,lab(0x9339e));continue;}
        d0=pad(a,e);
        if (d0&8u) GO(D24);
        if (d0&4u) GO(D74);
        if (!(d0&2u)) {SL(0,lab(0x93278));continue;}
        if (d0&0x80u) {SL(0,lab(0x93a08));continue;}
        if (d0&0x20u) {SL(0,lab(0x937c6));continue;}
        if (d0&0x40u) k929e6(a,e);
        SL(0x62,0);
        if (!tread(a,e,true)) return GA_UNSUPPORTED;
        if (k92832(a,e)<0) return GA_UNSUPPORTED;
        return tail(a,e);
    case 0x9333a:
        /* 09333A: the jump (sound 1090, +34 = 0, program 299180 gives the
         * lift); 014C0C directly until +34 moves -> 0933C2. */
        sound(e,0x1090);SL(0x34,0);START(v3?0x299228:0x299180);SL(0,lab(0x93362));   /* 09452C: program 299228 */
        /* fall through */
    case 0x93362:
        d0=pad(a,e);
        if (d0&0x40u) {(void)k92934(a,e);if (e->unsupported) return GA_UNSUPPORTED;}
        if (!air_step(a,e)) return GA_UNSUPPORTED;
        if (k92832(a,e)<0) return GA_UNSUPPORTED;
        if (L(0x34)) SL(0,lab(0x933c2));
        return tail(a,e);
    case 0x9339e:
        /* 09339E: off the ground (program 2942D4, 29282A with +77). */
        START(B(0x77)?0x29282a:0x2942d4);SL(0,lab(0x933c2));
        /* fall through */
    case 0x933c2: {
        /* 0933C2: the tread node reset by 092A10[+75] (0153D2), gravity,
         * the stick, C, 014C0C, the program (2942D4 again at its end);
         * landing (+15 bit 6 with +34 < 0) -> 09347A. */
        neo_gactor_t *c=e->actor?e->actor(e->ctx,L(0x8c)):NULL;
        if (!c) STOP(lab(0x933d6));
        neo_tread_air_reset(c,rw(0x92a10u+2u*B(0x75)));
        gravity(a);
        d0=pad(a,e);air_steer(a,d0);
        if (d0&0x40u) {(void)k92934(a,e);if (e->unsupported) return GA_UNSUPPORTED;}
        if (!air_step(a,e)) return GA_UNSUPPORTED;
        if ((r=k92832(a,e))<0) return GA_UNSUPPORTED;
        if (r) START(0x2942d4);
        if (BIT(0x15,6) && (L(0x34)&0x80000000u)) SL(0,lab(0x9347a));
        return tail(a,e);
    }
    case 0x9347a:
        /* 09347A: landing (sound 1091, program 299756, +62 = 0). */
        sound(e,0x1091);START(0x299756);SL(0x62,0);SL(0,lab(0x934a2));
        /* fall through */
    case 0x934a2:
        if (!BIT(0x15,6)) {SL(0,lab(0x9339e));continue;}
        d0=pad(a,e);
        SL(0x62,(d0&8u)?SPEED:(d0&4u)?NSPEED:1);
        if (d0&0x20u) {SL(0,lab(0x9333a));continue;}
        if (d0&2u) {SL(0,lab(0x93138));continue;}
        if (d0&0x40u) {(void)k92934(a,e);if (e->unsupported) return GA_UNSUPPORTED;}
        if (!tread(a,e,true)) return GA_UNSUPPORTED;
        if ((r=k92832(a,e))<0) return GA_UNSUPPORTED;
        if (r) SL(0,lab(0x92c9a));
        return tail(a,e);
    case 0x937c6:
        /* 0937C6: jumping out (B crouched): program 292200, +62 = 0; at its
         * end 0AA7FC kind 1; +50 = 0 each update. */
        START(0x292200);SL(0x62,0);SL(0,lab(0x937e4));
        /* fall through */
    case 0x937e4:
        if (!tread(a,e,true)) return GA_UNSUPPORTED;
        if ((r=k92832(a,e))<0) return GA_UNSUPPORTED;
        if (r) {neo_vslot_exit(v->slots,rider_p(a),a->m,node_word(e,a),1);touched(a);}
        SL(0x50,0);
        return tail(a,e);
    default: ga_fail(e,L(0));return GA_UNSUPPORTED;
    }
    next:;
    }
    ga_fail(e,L(0));return GA_UNSUPPORTED;
}
/* ---- children -------------------------------------------------------------- */
/* 094EAA / 094F1C: the shell (sound 1084, program 290FC0, palette 0x258):
 * 0x120000 along +6E (002E7C) plus the tank's +30 and +6E << 5; then drag,
 * gravity and lift, the pose +79 by the angle, 0143E4, the attack row
 * 094E7A (types 2, 3: explode), the ground or a wall -> 094FF8 (sound 10F2,
 * row 094E8A, palette 0x378, program 2919FE until its end); outside the
 * screen -> 004188. */
static ga_result_t shell_tick(neo_gactor_t *a,neo_genv_t *e) {
    int r;
    switch (L(0)) {
    case 0x94eaa: {
        sound(e,0x1084);SB(0x15,B(0x15)|0x20u);SW(0x66,0xe000);SW(0x1e,0xe000);
        if (!ga_anim_start(a,e,0x290fc0,0)) return GA_UNSUPPORTED;
        ga_palette(a,0x258);
        uint32_t vx,vy;ga_polar(0x120000,B(0x6e),&vx,&vy);
        neo_gactor_t *t=creator(a,e);uint32_t d7=t?ga_l(t,0x30):0;
        vx+=d7;vy-=d7;
        d7=(uint32_t)((int32_t)S16(W(0x6e))<<5);vx+=d7;vy+=d7;
        SL(0x30,vx);SL(0x34,vy);SL(0,0x94f1c);
        return GA_MORE;
    }
    case 0x94f1c: {
        uint32_t d0=(uint32_t)((int32_t)L(0x30)>>3);SL(0x30,L(0x30)-d0);
        uint32_t d1=(uint32_t)((int32_t)(L(0x34)+0x19eb85u)>>3);
        if (d0&0x80000000u) d0=0u-d0;
        d1-=d0;d1-=d0;SL(0x34,L(0x34)-d1);
        SB(0x79,(uint8_t)(((uint16_t)(S16((uint16_t)(int16_t)(int8_t)B(0x6e))>>3))&0x1fu));
        ga_air(a,e);
        bool boom=BIT(0x15,6) || BIT(0x5f,0);
        if (!boom) {
            uint8_t t=ga_attack(a,e,0x94e7a);
            if (t<6u && (t==2u||t==3u)) boom=true;
            else if (BIT(0x5f,1)) {SB(0x15,B(0x15)|2u);boom=true;}
        }
        if (boom) SL(0,0x94ff8);
        if ((r=ga_anim_step(a,e,NULL))<0) return GA_UNSUPPORTED;
        if (L(0x34)&0x80000000u) {uint16_t x=(uint16_t)(W(0x6e)-0xc000u);SW(0x6e,W(0x6e)-(uint16_t)(S16(x)>>2));}
        {uint16_t x=(uint16_t)(W(0x26)-0x100u);SW(0x26,W(0x26)-(uint16_t)(S16(x)>>3));}
        if ((uint16_t)(W(0x3c)+0x10u)>=0x160u || S16((uint16_t)(W(0x40)-0x100u))<0) return GA_DELETE;
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    }
    case 0x94ff8:
        sound(e,0x10f2);ga_attack(a,e,0x94e8a);ga_palette(a,0x378);
        SW(0x66,0x1001);SW(0x1e,0x1001);START(0x2919fe);SL(0,0x9503c);
        /* fall through */
    case 0x9503c:
        ga_project(a,e);
        if ((r=ga_anim_step(a,e,NULL))<0) return GA_UNSUPPORTED;
        return r?GA_DELETE:(e->unsupported?GA_UNSUPPORTED:GA_MORE);
    }
    ga_fail(e,L(0));return GA_UNSUPPORTED;
}
/* 0952F8 / 09533C: the muzzle flash (palette 0x208) with the tank's +74/
 * +76; on the ground program 29090E following the tank (zoom +13 by the
 * callback 290F4C, then back to full size), in the air 095422 (program
 * 290C9E at the offset). */
static bool flash_call(neo_gactor_t *a,neo_genv_t *e,neo_anim_kind_t kind,uint32_t routine,uint16_t parameter) {
    (void)e;(void)parameter;
    if (kind!=NEO_ANIM_CALL || routine!=0x290f4c) return false;
    SB(0x13,rb(0x290f3au+B(0x74)));return true;
}
static ga_result_t flash_tick(neo_gactor_t *a,neo_genv_t *e) {
    int r;neo_gactor_t *t=creator(a,e);
    switch (L(0)) {
    case 0x952f8:
        ga_palette(a,0x208);
        if (!t) {ga_fail(e,0x95306);return GA_UNSUPPORTED;}
        SB(0x74,ga_b(t,0x74));SB(0x76,ga_b(t,0x76));
        if (!(ga_b(t,0x15)&0x40u)) {
            SW(0x28,W(0x28)+W(0x30));SW(0x2c,W(0x2c)+W(0x34));
            START(0x290c9e);SL(0,0x95448);
            /* fall through to 095448 */
            ga_project(a,e);
            if ((r=ga_anim_step(a,e,flash_call))<0) return GA_UNSUPPORTED;
            return r?GA_DELETE:GA_MORE;
        }
        START(0x29090e);SL(0,0x9533c);
        /* fall through */
    case 0x9533c: {
        ga_project(a,e);
        uint16_t d0=(uint16_t)(B(0x13)+1u);
        if (d0<=0xffu) {
            int32_t m=(int32_t)S16(d0)*S16(W(0x34));
            uint16_t q=(uint16_t)((uint32_t)(m>>8));q=(uint16_t)(q-W(0x34));
            SW(0x40,W(0x40)-q);
        }
        int16_t dx=(int16_t)((int8_t)B(0x86)-S16(W(0x30))),dy=(int16_t)((int8_t)B(0x87)-S16(W(0x34)));
        SW(0x3c,W(0x3c)-(uint16_t)dx);SW(0x40,W(0x40)-(uint16_t)dy);
        if ((r=ga_anim_step(a,e,flash_call))<0) return GA_UNSUPPORTED;
        if (r) return GA_DELETE;                                        /* 095382: protection check, then 004188 */
        SB(0x13,B(0x13)-(uint8_t)(S16((uint16_t)(B(0x13)-0xffu))>>3));
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    }
    case 0x95448:
        ga_project(a,e);
        if ((r=ga_anim_step(a,e,flash_call))<0) return GA_UNSUPPORTED;
        return r?GA_DELETE:GA_MORE;
    }
    ga_fail(e,L(0));return GA_UNSUPPORTED;
}
/* 0954DC / 095580: the rider climbing in (tables 0954A4 / 0954B4 / 0954C4
 * by the player's character +93 (0C05B8: 2, 1, else) and the rider's +6E
 * (tank +82), palette 0954D4[+82], 01AD6E on the tank); each update the
 * offset 095460[tank +74]; at the program's end tank +60 = FF and 004188
 * (after the protection check 0955B6, not reproduced). */
static ga_result_t entry_tick(neo_gactor_t *a,neo_genv_t *e) {
    neo_gactor_t *t=creator(a,e);int r;
    if (!t) {ga_fail(e,L(0));return GA_UNSUPPORTED;}
    switch (L(0)) {
    case 0x954dc: {
        unsigned p=ga_w(t,0x80)&0xffu?1u:0u;
        const uint8_t *pm=e->vehicle?e->vehicle->player[p]:NULL;
        if (!pm) {ga_fail(e,0x954e4);return GA_UNSUPPORTED;}
        uint8_t c=pm[0x93];uint32_t table=c==2u?0x954c4:c==1u?0x954b4:0x954a4;
        uint16_t i=ga_w(t,0x82);
        START(rl(table+4u*i));
        ga_palette(a,rw(0x954d4u+2u*i));
        ga_place(a,e,0,0,NEO_GA_TAG(t->index));                        /* 01AD6E */
        SL(0,0x95580);
    }   /* fall through */
    case 0x95580: {
        uint32_t o=0x95460u+4u*ga_b(t,0x74);
        SW(0x28,rw(o));SW(0x2c,rw(o+2u));
        ga_project(a,e);
        if ((r=ga_anim_step(a,e,NULL))<0) return GA_UNSUPPORTED;
        if (r) {ga_sb(t,0x60,0xff);return GA_DELETE;}
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    }
    }
    ga_fail(e,L(0));return GA_UNSUPPORTED;
}
/* 0956CE / 095768: the tread dust (program 291AF0, palette 0x204, +26 =
 * tank +94 + 0x100, +74/+75 from the tank, mirrored when the tank backs,
 * at 095646[+74] from the tank relative to the scene root, 01AB9C); then
 * while the tank lives (00409A) its +1E, 01ABC8 and the program. */
static ga_result_t dust_tick(neo_gactor_t *a,neo_genv_t *e) {
    neo_gactor_t *t=creator(a,e);int r;
    switch (L(0)) {
    case 0x956ce: {
        if (!t) {ga_fail(e,0x956ec);return GA_UNSUPPORTED;}
        START(0x291af0);ga_palette(a,0x204);
        SW(0x26,(uint16_t)(ga_b(t,0x94)+0x100u));
        SB(0x75,ga_b(t,0x75));SB(0x74,ga_b(t,0x74));
        if (ga_l(t,0x30)&0x80000000u) {SB(0x15,B(0x15)|1u);SB(0x75,(uint8_t)(4u-B(0x75)));}
        uint32_t o=0x95646u+8u*B(0x74);
        int16_t d0=(int16_t)rw(o),d1=(int16_t)rw(o+2u);
        if (BIT(0x15,0)) {d0=(int16_t)-d0;d1=(int16_t)-d1;}
        neo_contact_parent_t root;
        if (!ga_parent(e,0x10e968u,&root)) {ga_fail(e,0x9574c);return GA_UNSUPPORTED;}
        d0=(int16_t)(d0+S16(ga_w(t,0x3c))-(int16_t)((uint32_t)root.x>>16));
        d1=(int16_t)(d1+S16(ga_w(t,0x40))-(int16_t)((uint32_t)root.y>>16));
        ga_place(a,e,d0,d1,0x10e968u);                                  /* 01AB9C */
        SL(0,0x95768);
    }   /* fall through */
    case 0x95768:
        if (!t) return GA_MORE;                                         /* 00409A: carry clear */
        SW(0x1e,ga_w(t,0x1e));
        ga_project(a,e);
        if ((r=ga_anim_step(a,e,NULL))<0) return GA_UNSUPPORTED;
        return r?GA_DELETE:(e->unsupported?GA_UNSUPPORTED:GA_MORE);
    }
    ga_fail(e,L(0));return GA_UNSUPPORTED;
}
/* 093BC2 / 093BF2: the tank's burst (palette 0x363, layer 0x1001,
 * program 22CF2C, held still by 0143E4 until its end; then the cartridge
 * check before 004188, not reproduced). */
static ga_result_t burst_tick(neo_gactor_t *a,neo_genv_t *e) {
    int r;
    if (L(0)==0x93bc2 || L(0)==0x94e2a) {ga_palette(a,0x363);SW(0x66,0x1001);SW(0x1e,0x1001);START(0x22cf2c);SL(0,L(0)+0x30u);}   /* 094E2A/094E5A: the Mission 3 copy */
    SL(0x30,0);SL(0x34,0);ga_air(a,e);
    if ((r=ga_anim_step(a,e,NULL))<0) return GA_UNSUPPORTED;
    return r?GA_DELETE:(e->unsupported?GA_UNSUPPORTED:GA_MORE);
}
/* 0AE1DC / 0AE1F0: the white screen (009BB0, RAM 109944) for +6E + 1
 * updates; with +70 it blinks (+71). */
static ga_result_t flashout_tick(neo_gactor_t *a,neo_genv_t *e) {
    if (L(0)==0xae1dc) {
        if (e->flags) e->flags->flash=1;
        SB(0x71,0xff);SL(0,0xae1f0);return GA_MORE;
    }
    uint16_t n=W(0x6e);SW(0x6e,n-1u);
    if (!n) {if (e->flags) e->flags->flash=0;return GA_DELETE;}
    if (B(0x70)) {uint8_t d0=B(0x71)?0:0xff;SB(0x71,d0);if (e->flags) e->flags->flash=d0;}
    return GA_MORE;
}
/* 0AD9B8 / 0AD88A: a piece of the tank (layer 0x1001, palette 0x204,
 * program 0AD8CE in the fixed block until its end, 01ABC8). */
/* 0AD96C (program callback): while +4E, the next piece 0AD9B8 (00411C)
 * at the +30/+34 words from this one (01ACF2) with +4E - 1 and the same
 * +30/+34, +34 - 0x40000. */
static bool piece_call(neo_gactor_t *a,neo_genv_t *e,neo_anim_kind_t kind,uint32_t routine,uint16_t parameter) {
    (void)parameter;
    if (kind!=NEO_ANIM_CALL || routine!=0xad96c) return false;
    if (!W(0x4e)) return true;
    neo_gactor_t *q=e->spawn?e->spawn(e->ctx,a,0xad9b8,true):NULL;
    if (!q) return true;
    ga_place_at(q,a,e,(int16_t)W(0x30),(int16_t)W(0x34));
    ga_sw(q,0x4e,(uint16_t)(W(0x4e)-1u));
    ga_sl(q,0x30,L(0x30));ga_sl(q,0x34,L(0x34)+0xfffc0000u);
    return true;
}
static ga_result_t piece_tick(neo_gactor_t *a,neo_genv_t *e) {
    int r;
    if (L(0)==0xad9b8) {SW(0x66,0x1001);SW(0x1e,0x1001);ga_palette(a,0x204);START(0xad8ce);SL(0,0xad88a);}
    ga_project(a,e);
    if ((r=ga_anim_step(a,e,piece_call))<0) return GA_UNSUPPORTED;
    return r?GA_DELETE:(e->unsupported?GA_UNSUPPORTED:GA_MORE);
}
bool neo_tank_routine(uint32_t r) {
    switch (r) {
    case 0x92a32: case 0x92ad8: case 0x92b0e: case 0x92c18: case 0x92c38: case 0x92c76: case 0x92c9a:
    case 0x92cc2: case 0x92dda: case 0x92f94: case 0x93054: case 0x9307c: case 0x93278: case 0x9329e:
    case 0x93a08: case 0x93a46: case 0x93afa: case 0x9351a: case 0x9339e: case 0x93204:
    case 0x93138: case 0x9315e: case 0x9321a: case 0x9333a: case 0x93362: case 0x933c2: case 0x9347a: case 0x934a2:
    case 0x937c6: case 0x937e4:
    case 0x94eaa: case 0x94f1c: case 0x94ff8: case 0x9503c:
    case 0x952f8: case 0x9533c: case 0x95448:
    case 0x954dc: case 0x95580: case 0x956ce: case 0x95768:
    case 0x93bc2: case 0x93bf2: case 0xae1dc: case 0xae1f0: case 0xad9b8: case 0xad88a:
    case 0x14a4e: return true;
    }
    return tank_v3(r) || r==0x94e2au || r==0x94e5au;   /* 093C9A..094CDE and its burst: the Mission 3 copy */
}
ga_result_t neo_tank_tick(neo_gactor_t *a,neo_genv_t *e) {
    uint32_t r=ga_l(a,0);
    if (r==0x14a4e) return neo_tread_tick(a,e);
    if (r>=0x94eaa && r<=0x9503c) return shell_tick(a,e);
    if (r>=0x952f8 && r<=0x95448) return flash_tick(a,e);
    if (r==0x954dc || r==0x95580) return entry_tick(a,e);
    if (r==0x956ce || r==0x95768) return dust_tick(a,e);
    if (r==0x93bc2 || r==0x93bf2 || r==0x94e2a || r==0x94e5a) return burst_tick(a,e);
    if (r==0xae1dc || r==0xae1f0) return flashout_tick(a,e);
    if (r==0xad9b8 || r==0xad88a) return piece_tick(a,e);
    return tank_tick(a,e);
}
