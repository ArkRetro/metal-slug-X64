#include "neo_region.h"
#include "neo_soldier_ai.h"   /* neo_angle (002EB4) */
/* Slugnoid 09692C (the player's walker of Mission 2 stage 3, script records
 * 2D9BEA/2D9BF6: always made when the camera progress reaches 3391, at x
 * 392 or 568; also Mission 5) and its own children: manual translation of
 * ROM 0966F0..097E06 and 097EDE..097FC4; ROM labels are provenance only.
 * The two vulcan turrets 03B4E0/03B57C (+72 = 3 and 2, burst step 4) are the
 * Camel Slug's (neo_slug.c); the ground/air steps 014594 / 0143EE are
 * neo_slug_k14594 / neo_slug_k143ee; the cannon's shell ends in the SV-001
 * shell's burst 094FF8 (neo_tank.c) or, on the ground, in the bomb of type 3
 * 01EC0E (neo_bullet.c).
 *
 * Fields: +4E health (0x30), +50 hurtbox 0966BC, +6E rider (player, FFFF
 * gone), +70 the rider's +6E, +72 the program table 096418 (offsets: 00
 * idle, 04 stand, 0C fall, 10/14 hit on the ground / in the air, 18 mount,
 * 1C rider gone, 20/24 crouch / rise (sub-tables by +88), 28 land, 2C jump,
 * 30 walk, 38 wreck, 3C empty wreck, 40 blown up; 0966F0 keeps it in +76),
 * +78..+7B / +7C..+7F the turrets' anchors (x, y, flash; 0x8000 = gone),
 * +80/+81 the turrets alive, +82 the cannon's height (programs), +84 ridden
 * and steady, +85 the rider's input byte, +86 the pose (096800, programs
 * select by it), +88/+89 crouch depth (programs), +8A shells in flight, +8B
 * shells, +8C the death palette step (097EDE / 097F52), +8E flicker, +90 B
 * allowed on landing (programs), +91 thrust (+93 = 1 only), +92 no rider,
 * +93 the variant (+6E of the record).
 *
 * Children: the pilot 097830/097886 (palette 034330[+70 of the walker],
 * table 09649C[+70], follows the walker's +76/+86/+89 and its layer - 1;
 * stops with program 0x40), the marker 0353A4 (neo_slug.c), the muzzle
 * flash 097972/097996 (palette 3, program 29D978), the shell 097A36/097A86
 * (0x100000 along +6E = 0xC0, palette 0x258, program 290FC0, 0143DC, row
 * 0963D8: types 2, 3 or a wall -> 094FF8; ground -> 01EC0E; out of 0966D0
 * -> 004188; 097B24 gives the walker its +8A back), the jump dust 097DD0
 * (00411C, program in +72), the legs' flames 097C46 / 097D22 (programs in
 * +72, layer of the walker - 1; 097D22 alternates 29DBD4/29DBDE while the
 * walker's +91 = 6), the burst 0AD8A2 (layer E0ED, palette 0x204, program
 * 22CF2C, then 0AD88A of neo_tank.c).
 *
 * Explicit stops: the death with a rider 0973E2 (0353C2's child 03533A is
 * not translated), the cartridge checks before 004188 are not reproduced
 * (the deletion), TRAP #F of 097EDE / 097F52 (+8C past the list). */
#define B(o) ga_b(a,(o))
#define W(o) ga_w(a,(o))
#define L(o) ga_l(a,(o))
#define SB(o,v) ga_sb(a,(o),(uint8_t)(v))
#define SW(o,v) ga_sw(a,(o),(uint16_t)(v))
#define SL(o,v) ga_sl(a,(o),(uint32_t)(v))
#define BIT(o,b) ((B(o)>>(b))&1u)
#define STOP(at) do { ga_fail(e,(at));return GA_UNSUPPORTED; } while (0)
#define CHECK do { if (e->unsupported) return GA_UNSUPPORTED; } while (0)
static uint16_t rw(uint32_t address) {const uint8_t *p=ga_rom(address);return p?(uint16_t)(p[0]<<8|p[1]):0;}
static uint32_t rl(uint32_t address) {return (uint32_t)rw(address)<<16|rw(address+2);}
static void sound(neo_genv_t *e,uint16_t id) {if (e->sound) e->sound(e->ctx,id);}
static neo_gactor_t *creator(neo_gactor_t *a,neo_genv_t *e) {return e->creator?e->creator(e->ctx,a):NULL;}
static uint16_t node_word(neo_genv_t *e,const neo_gactor_t *a) {return e->node_word?e->node_word(e->ctx,a):(uint16_t)(0x8000u|(uint16_t)a->index);}
static void touched(neo_gactor_t *a) {static const uint8_t o[]={0x4e,0x4f,0x5e,0x5f};for (unsigned i=0;i<sizeof(o);i++) ga_sb(a,o[i],a->m[o[i]]);}
static unsigned rider_p(const neo_gactor_t *a) {return (ga_w(a,0x6e)&0xffu)?1u:0u;}   /* tst.b d0 */
static neo_vslot_player_t player(neo_genv_t *e,unsigned p) {return (neo_vslot_player_t){e->vehicle->exists[p],e->vehicle->player[p]};}
static uint16_t rng(void *ctx) {return ga_rand((neo_genv_t *)ctx);}
static void k1b1dc(neo_gactor_t *a) {SL(0x30,0);SL(0x34,0);SL(0x62,0);}
/* 01AD8C */
static void depth(neo_gactor_t *a) {uint16_t d0=W(0x66);if ((d0&0xf000u)==0x8000u) SW(0x1e,d0-W(0x40));}
/* 0CA2CE +0, low byte into +85. */
static void read_pad(neo_gactor_t *a,neo_genv_t *e) {SB(0x85,(uint8_t)e->vehicle->input[rider_p(a)][0]);}
static bool call(neo_gactor_t *a,neo_genv_t *e,neo_anim_kind_t kind,uint32_t routine,uint16_t parameter) {
    (void)parameter;
    if (kind!=NEO_ANIM_CALL) return false;
    if (routine==0x963c8) {ga_palette(a,0x2d);return true;}               /* 0963C8: 009EA8 (0x2D) */
    (void)e;return false;
}
/* 00427A: true at the program's end; -1 unsupported. */
static int step(neo_gactor_t *a,neo_genv_t *e) {return ga_anim_step(a,e,call);}
/* 0966F0: +76 = d0, program (+72)[d0] (an offset). The crouch entries 0x20
 * and 0x24 are sub-tables: their callers start the entry +88 of it after
 * (the first 004256 is overwritten). */
static bool program(neo_gactor_t *a,neo_genv_t *e,uint16_t d0) {
    SW(0x76,d0);
    uint32_t t=rl(L(0x72)+d0);
    if (d0==0x20u || d0==0x24u) t=rl(t+B(0x88));
    if (!t) {ga_fail(e,L(0x72)+d0);return false;}
    return ga_anim_start(a,e,t,0x100);
}
/* 096916: 01ABC8; true when the projected +40 is not below RAM 10C72C
 * (0x100 from the new game 0C0E20: the bottom of the screen). */
static bool above(neo_gactor_t *a,neo_genv_t *e) {ga_project(a,e);return (int16_t)(W(0x40)-0x100u)>=0;}
/* 097EDE / 097F52: the palette list 097E88 from +8C - base (009F88 +
 * 009EA8 when nonzero; zero = the end: true). +8C counts each call. */
static bool fade(neo_gactor_t *a,neo_genv_t *e,uint16_t base,uint16_t limit,uint32_t at) {
    uint16_t c=W(0x8c);
    if (c>=limit) {ga_fail(e,at);return false;}                        /* TRAP #F */
    if (c>=base) {
        uint16_t d0=rw(0x97e88u+2u*(uint16_t)(c-base));
        if (!d0) return true;
        SB(0x1d,0);ga_palette(a,d0);
    }
    SW(0x8c,c+1u);return false;
}
/* 096794..0968BE: the pose +86 one step towards the stick / motion target
 * (002EB4 of the velocity, 0..0x10, 8 = level). */
static void pose_step(neo_gactor_t *a) {
    int32_t d0=-(int32_t)L(0x30),d1=(int32_t)L(0x34);
    if (d1<=0) d1=-d1;
    if (d1<=0x38000) d1=0x38000;
    uint8_t t=(uint8_t)((uint8_t)(neo_angle((int16_t)((uint32_t)d0<<4>>16),(int16_t)((uint32_t)d1<<4>>16))>>2)-8u);
    int8_t s=(int8_t)t;
    if (s>8) s++;
    else if (s==8 && !L(0x30)) s=8;
    else s++;
    if (s>=0x10) s=0x10;
    if (s<0) s=0;
    int32_t vx=(int32_t)L(0x30);
    if (!B(0x91)) {
        if ((int32_t)L(0x34)<0) s=vx<0?7:vx>0?9:8;
    } else if (B(0x85)&4u) s=0;
    else if (B(0x85)&8u) s=0x10;
    else s=vx<0?7:vx>0?9:8;
    int8_t p=(int8_t)B(0x86);
    if (s<p) p--; else if (s>p) p++;
    SB(0x86,(uint8_t)p);
}
/* 0968C0: the thrust of the variant 1 (B held: +91 = 6 while rising). */
static void thrust(neo_gactor_t *a,neo_genv_t *e) {
    if (B(0x93)!=1u) return;
    if (B(0x91)) {SB(0x91,B(0x91)-1u);SL(0x34,0);}
    uint8_t d1=(uint8_t)e->vehicle->input[rider_p(a)][1];
    if ((d1&0x20u) && (int32_t)L(0x34)<=0) SB(0x91,6);
}
/* 0040BC (after the walker) / 00411C (after the script actor) and 01B220
 * (0, 0) or 01ACF2 (0, 0). */
static neo_gactor_t *child(neo_gactor_t *a,neo_genv_t *e,uint32_t routine,uint16_t reserve,bool script) {
    neo_gactor_t *c=NULL;
    if (script) c=e->spawn?e->spawn(e->ctx,NULL,routine,true):NULL;   /* +0C = the script actor */
    else c=e->spawn_reserve?e->spawn_reserve(e->ctx,a,routine,reserve):e->spawn?e->spawn(e->ctx,a,routine,false):NULL;
    return c;
}
/* 097DD0 dust after the script actor (program 29DCDE) at the walker (01ACF2). */
static void dust(neo_gactor_t *a,neo_genv_t *e) {
    neo_gactor_t *c=child(a,e,0x97dd0,0x2a,true);
    if (c) {ga_sl(c,0x72,0x29dcde);ga_place_at(c,a,e,0,0);}
}
/* 096FB6 / 097240..0972A0: the legs' flames 097C46 (29DAEA) and 097D22
 * (29DC30, its +76 = the walker's), +30 = v30, program d0, +86 = 0x0800. */
static bool flames(neo_gactor_t *a,neo_genv_t *e,uint32_t v30,uint16_t prog) {
    neo_gactor_t *c=child(a,e,0x97c46,0x2a,false);
    if (c) {ga_place_from(c,a,e,0,0);ga_sl(c,0x72,0x29daea);}
    SL(0x30,v30);
    if (!program(a,e,prog)) return false;
    SW(0x86,0x800);
    c=child(a,e,0x97d22,0x2a,false);
    if (c) {ga_place_from(c,a,e,0,0);ga_sl(c,0x72,0x29dc30);}
    SB(0x91,0);
    if (c) ga_sw(c,0x76,W(0x76));
    return true;
}
/* 096728: the cannon (C): no shell in flight and 0AA956; the flash 097972
 * at (0, +82) and the shell 097A36 (+6E = 0xC000, +5E), sound 1084. */
static void cannon(neo_gactor_t *a,neo_genv_t *e) {
    if (B(0x8a)) return;
    if (!neo_vslot_shell(e->vehicle->slots,rider_p(a),(e->vehicle->dip10f001&4u)!=0)) return;
    neo_gactor_t *f=child(a,e,0x97972,0x2a,false);
    if (f) ga_place_from(f,a,e,0,(int16_t)W(0x82));
    neo_gactor_t *s=child(a,e,0x97a36,0xe,false);
    if (!s) return;
    SB(0x8a,B(0x8a)+1u);
    ga_sw(s,0x6e,0xc000);ga_place_from(s,a,e,0,0);ga_sb(s,0x5e,B(0x5e));
    sound(e,0x1084);
}
/* 0977DC: a hit takes a turret (by +5B bit 7 the right one first). */
static void lose_turret(neo_gactor_t *a,neo_genv_t *e) {
    sound(e,0x1089);
    unsigned d0=0x81,d1=0x80,d2=0x7c,d3=0x78;
    if (BIT(0x5b,7)) {d0=0x80;d1=0x81;d2=0x78;d3=0x7c;}
    if (!B(d0)) {d0=d1;d2=d3;}
    SB(d0,0);SW(d2,0x8000);
}
/* 0976C8: the common tail of the ridden states. */
static ga_result_t tail(neo_gactor_t *a,neo_genv_t *e) {
    neo_vehicle_env_t *v=e->vehicle;
    if (BIT(0x5f,6)) {SB(0x5f,B(0x5f)&~0x40u);SB(0x5f,B(0x5f)&0x7fu);SB(0x5a,0);}
    SB(0x14,B(0x1a));
    if (B(0x8e)) {
        if (e->video&1u) {SB(0x14,B(0x19));SB(0x7a,0xff);SB(0x7e,0xff);}
        SB(0x8e,B(0x8e)-1u);SB(0x5f,B(0x5f)|0x40u);
    }
    if (v->dip10f000&0x80u) SB(0x5f,B(0x5f)|0x40u);
    unsigned p=rider_p(a);
    neo_vslot_player_t pl=player(e,p);uint16_t p6e;
    if (!neo_vslot_bind(v->slots,p,a->m,node_word(e,a),&pl,e->video,0,0,&p6e)) {
        SB(0x8e,0x2a);SB(0x5f,B(0x5f)|0x40u);SL(0,0x96ba6);
    } else {
        uint8_t kind,shells,type;uint16_t health,d4;
        if (neo_vslot_ridden(v->slots,p,v->exists[p],&kind,&shells,&type,&health,&d4) && kind==3u) SB(0x8b,shells);
    }
    touched(a);
    if (!above(a,e)) SL(0,0x975c6);
    bool hit=ga_register(a,e,NULL,NULL);                                /* 0AA9E4 with 004C24 */
    bool carry=neo_vslot_damage(v->slots,p,a->m,hit);touched(a);CHECK;
    if (carry) {
        if (!BIT(0x5f,6)) lose_turret(a,e);
        SB(0x5a,0);SB(0x8e,0x2a);SB(0x5f,B(0x5f)|0x40u);
        SL(0,BIT(0x15,6)?0x9737e:0x97308);
    }
    if (!W(0x4e)) SB(0x5f,B(0x5f)|0x80u);
    if (BIT(0x5f,7) && BIT(0x15,6)) SL(0,0x973e2);
    return e->unsupported?GA_UNSUPPORTED:GA_MORE;
}
/* The ridden steps: 014594 on the ground, gravity and 0143EE in the air. */
static void ride_step(neo_gactor_t *a,neo_genv_t *e) {
    if (!BIT(0x15,6)) {SL(0x30,0);SL(0x34,L(0x34)+0xffffb000u);neo_slug_k143ee(a,e);}
    else {SL(0x62,0);neo_slug_k14594(a,e);}
}
/* 097022.. gravity: d0 = (+34 >> 5) - d (subi.l), +34 -= d0. */
static void fall(neo_gactor_t *a,uint32_t d) {SL(0x34,L(0x34)-((uint32_t)((int32_t)L(0x34)>>5)-d));}
/* The input tails of the ridden states (B, C, A+B). */
#define AB_EXIT do { if (B(0x85)&0x80u) {SL(0,0x971f8);return tail(a,e);} } while (0)
static ga_result_t walker_tick(neo_gactor_t *a,neo_genv_t *e) {
    if (!e->vehicle) STOP(L(0));
    neo_vehicle_env_t *v=e->vehicle;int r;
    for (int guard=0;guard<4;guard++) switch (L(0)) {
    case 0x9692c: {
        SB(0x93,(uint8_t)W(0x6e));
        SW(0x66,0x8810);SW(0x1e,0x8810);SL(0x50,0x966bc);
        ga_floor(a,e);CHECK;
        SW(0x4e,0x30);SB(0x78,0x13);SB(0x79,0x21);SB(0x7c,0xec);SB(0x7d,0x21);
        neo_gactor_t *t=child(a,e,0x3b4e0,0xe,false);
        if (t) {ga_sl(t,0x6e,NEO_GA_TAG(a->index)|(0x78u<<16));ga_sb(t,0x72,3);ga_sb(t,0x73,4);ga_sb(t,0x74,0);ga_sb(t,0x75,0);}
        SB(0x80,1);
        t=child(a,e,0x3b4e0,0xe,false);
        if (t) {ga_sl(t,0x6e,NEO_GA_TAG(a->index)|(0x7cu<<16));ga_sb(t,0x72,2);ga_sb(t,0x73,4);ga_sb(t,0x74,2);ga_sb(t,0x75,0);}
        SB(0x81,2);SB(0x8b,0xa);SW(0x8c,0);
        child(a,e,0x97830,0x1c,false);
        SB(0x92,1);SL(0,0x96a10);
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    }
    case 0x96a10:
        SB(0x1d,0);ga_palette(a,0x88d);ga_palette(a,2);                  /* 009F88, 009EA8 */
        SL(0x72,0x96418);
        if (!program(a,e,0)) return GA_UNSUPPORTED;
        SB(0x5f,B(0x5f)&0x7fu);
        if (above(a,e)) {
            neo_gactor_t *c=e->spawn?e->spawn(e->ctx,a,0x3523c,false):NULL;   /* 0353A4 (0, 0x28) */
            if (c) {ga_sw(c,0x6e,0);ga_sw(c,0x70,0x28);}
        }
        SL(0,0x96a60);
        /* fall through */
    case 0x96a60:
        if (!above(a,e)) {SL(0x30,0);SL(0x34,L(0x34)+0xffffb000u);ga_move(a,e);}
        else if (!BIT(0x15,6)) {SL(0x30,0);SL(0x34,L(0x34)+0xffffb000u);ga_air(a,e);}
        else {SL(0x62,0);ga_ground(a,e,NULL);}
        CHECK;
        if (step(a,e)<0) return GA_UNSUPPORTED;
    wait_tail: {
        if (ga_outside_view(a,e,0x966c8)) return GA_DELETE;            /* 0C8BEC -> 0041AC */
        if (BIT(0x5f,7)) {
            if (fade(a,e,0x28,0x53,0x97ee8)) SL(0,0x9756a);
            return e->unsupported?GA_UNSUPPORTED:GA_MORE;
        }
        if (!above(a,e)) return GA_MORE;
        int16_t rect[4];for (unsigned i=0;i<4;i++) rect[i]=(int16_t)rw(0x966d8u+2u*i);
        neo_vslot_player_t pl[2]={player(e,0),player(e,1)};
        int p=neo_vslot_select(v->slots,a->m,pl,rect,v->halted!=0,v->system!=0,rng,e);
        if (p>=0) {
            SW(0x6e,(uint16_t)p);uint16_t p6e;
            if (neo_vslot_bind(v->slots,(unsigned)p,a->m,node_word(e,a),&pl[p],e->video,0,0,&p6e)) {
                SW(0x70,p6e);
                neo_vslot_mount(v->slots,(unsigned)p,a->m,3,B(0x8b),4,0x30);
                SL(0,0x96b32);
            }
            touched(a);
        }
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    }
    case 0x96b32:
        SB(0x92,0);k1b1dc(a);SB(0x84,0);SB(0x8e,0x2a);SB(0x5f,B(0x5f)|0x40u);
        if (!program(a,e,0x18)) return GA_UNSUPPORTED;
        sound(e,0x1092);SL(0,0x96b62);
        /* fall through */
    case 0x96b62:
        ride_step(a,e);CHECK;depth(a);
        if ((r=step(a,e))<0) return GA_UNSUPPORTED;
        if (r) SL(0,0x96c22);
        return tail(a,e);
    case 0x96ba6:
        SB(0x92,1);k1b1dc(a);
        if (!program(a,e,0x1c)) return GA_UNSUPPORTED;
        SL(0,0x96bbe);
        /* fall through */
    case 0x96bbe:
        if (!above(a,e)) {SL(0x30,0);SL(0x34,L(0x34)+0xffffb000u);ga_move(a,e);}
        else ride_step(a,e);
        CHECK;depth(a);
        if ((r=step(a,e))<0) return GA_UNSUPPORTED;
        if (r) SL(0,0x96a10);
        goto wait_tail;
    case 0x96c22:
        k1b1dc(a);SB(0x84,1);
        if (!program(a,e,4)) return GA_UNSUPPORTED;
        SL(0,0x96c3a);
        /* fall through */
    case 0x96c3a:
        read_pad(a,e);
        neo_slug_k14594(a,e);CHECK;depth(a);
        if (step(a,e)<0) return GA_UNSUPPORTED;
        if (!BIT(0x15,6)) SL(0,0x96f6a);
        if (B(0x85)&8u) SL(0,0x96ccc);
        if (B(0x85)&4u) SL(0,0x96ccc);
        if (B(0x85)&2u) SL(0,0x96dc4);
        if (B(0x85)&0x20u) SL(0,0x96f86);
        AB_EXIT;
        if (B(0x85)&0x40u) {cannon(a,e);CHECK;}
        return tail(a,e);
    case 0x96ccc:
        SB(0x86,1);k1b1dc(a);SB(0x84,1);
        if (!program(a,e,0x30)) return GA_UNSUPPORTED;
        SL(0,0x96cea);
        /* fall through */
    case 0x96cea: {
        read_pad(a,e);uint32_t d7;
        if (B(0x85)&8u) {d7=0x20000;SB(0x86,1);}
        else if (B(0x85)&4u) {d7=0xfffe0000u;SB(0x86,0);}
        else {d7=0;SL(0,0x96c22);}
        SL(0x62,d7);
        neo_slug_k14594(a,e);CHECK;depth(a);
        if ((r=step(a,e))<0) return GA_UNSUPPORTED;
        if (r) {
            if (!program(a,e,0x30)) return GA_UNSUPPORTED;
            if (!(B(0x85)&0xcu)) SL(0,0x96c22);
        }
        if (!BIT(0x15,6)) SL(0,0x96f6a);
        if (B(0x85)&2u) SL(0,0x96dc4);
        if (B(0x85)&0x20u) SL(0,0x96f86);
        if (B(0x85)&0x40u) {cannon(a,e);CHECK;}
        AB_EXIT;
        return tail(a,e);
    }
    case 0x96dc4:
        SB(0x88,0);
        /* fall through */
    case 0x96dc8:
        k1b1dc(a);
        if (!program(a,e,0x20)) return GA_UNSUPPORTED;
        SL(0,0x96e02);
        /* fall through */
    case 0x96e02:
        SB(0x84,1);read_pad(a,e);
        neo_slug_k14594(a,e);CHECK;depth(a);
        if (!BIT(0x15,6)) {SL(0,0x96f6a);SB(0x84,0);}
        if ((r=step(a,e))<0) return GA_UNSUPPORTED;
        if (r && B(0x84) && (B(0x85)&0x22u)==0x22u) {                   /* 096708 */
            neo_vslot_exit(v->slots,rider_p(a),a->m,node_word(e,a),1);touched(a);
            SB(0x92,1);
        } else {
            if (!(B(0x85)&2u)) SL(0,0x96eaa);
            if (B(0x85)&0x20u) SL(0,0x96f86);
            if (B(0x85)&0x40u) {cannon(a,e);CHECK;}
        }
        AB_EXIT;
        return tail(a,e);
    case 0x96ea6:
        SB(0x88,0);
        /* fall through */
    case 0x96eaa:
        k1b1dc(a);SB(0x84,1);
        if (!program(a,e,0x24)) return GA_UNSUPPORTED;
        SL(0,0x96eea);
        /* fall through */
    case 0x96eea:
        read_pad(a,e);
        neo_slug_k14594(a,e);CHECK;depth(a);
        if ((r=step(a,e))<0) return GA_UNSUPPORTED;
        if (r) SL(0,0x96c22);
        if (!BIT(0x15,6)) SL(0,0x96f6a);
        if (B(0x85)&2u) SL(0,0x96dc8);
        if (B(0x85)&0x20u) SL(0,0x96f86);
        if (B(0x85)&0x40u) {cannon(a,e);CHECK;}
        AB_EXIT;
        return tail(a,e);
    case 0x96f6a:
        if (!program(a,e,0xc)) return GA_UNSUPPORTED;
        SL(0x30,L(0x62));SL(0,0x97028);continue;
    case 0x96f7a:
        SL(0x34,0xe4000);
        if (!flames(a,e,L(0x62),0x2c)) return GA_UNSUPPORTED;
        sound(e,0x1090);SL(0,0x97028);continue;
    case 0x96f86:
        SL(0x34,0xe4000);
        {neo_gactor_t *c=child(a,e,0x97dd0,0x2a,true);if (c) {ga_sl(c,0x72,0x29dcde);ga_place_at(c,a,e,0,0);}}
        if (!flames(a,e,L(0x62),0x2c)) return GA_UNSUPPORTED;
        sound(e,0x1090);SL(0,0x97028);
        /* fall through */
    case 0x97028: {
        read_pad(a,e);
        int32_t d7=(int32_t)L(0x30);
        if (B(0x85)&8u) {d7+=0xaaaa;if (d7>0x38000) d7=0x38000;}
        else if (B(0x85)&4u) {d7-=0xaaaa;if (d7<-0x38000) d7=-0x38000;}
        SL(0x30,(uint32_t)d7);
        pose_step(a);thrust(a,e);
        fall(a,0xffff0000u);
        neo_slug_k143ee(a,e);CHECK;
        if (BIT(0x15,6)) {SL(0,0x970fc);k1b1dc(a);read_pad(a,e);}
        if (step(a,e)<0) return GA_UNSUPPORTED;
        if (W(0x76)==0xcu && (B(0x85)&0x20u)) SL(0,0x96f7a);
        if (B(0x85)&0x40u) {cannon(a,e);CHECK;}
        return tail(a,e);
    }
    case 0x970fc:
        SB(0x84,1);
        if (!program(a,e,0x28)) return GA_UNSUPPORTED;
        dust(a,e);SL(0,0x97136);
        /* fall through */
    case 0x97136: {
        read_pad(a,e);
        uint32_t d7=(B(0x85)&8u)?0x20000u:(B(0x85)&4u)?0xfffe0000u:0;
        SL(0x62,d7);
        if (!ga_probe_forward(a,e,(int16_t)(W(0x62)<<4))) SL(0x62,0);
        CHECK;
        neo_slug_k14594(a,e);CHECK;depth(a);
        if ((r=step(a,e))<0) return GA_UNSUPPORTED;
        if (r) SL(0,0x96c22);
        if (!BIT(0x15,6)) SL(0,0x96f6a);
        if (B(0x90) && (B(0x85)&0x20u)) SL(0,0x96f86);
        if (B(0x85)&0x40u) {cannon(a,e);CHECK;}
        AB_EXIT;
        return tail(a,e);
    }
    case 0x971ee: SL(0,0x971f8);return tail(a,e);
    case 0x971f8:
        /* A+B: the rider jumps off (0AA7FC kind 1) and the walker flies up
         * (+34 = 0x1A4000) with its dust and flames. */
        neo_vslot_exit(v->slots,rider_p(a),a->m,node_word(e,a),1);touched(a);
        SB(0x92,1);SW(0x8c,0);SL(0x34,0x1a4000);
        dust(a,e);
        if (!flames(a,e,0,4)) return GA_UNSUPPORTED;
        SL(0,0x972a6);
        /* fall through */
    case 0x972a6: {
        fall(a,0xfffe0000u);
        ga_move(a,e);CHECK;
        if (fade(a,e,3,0x2e,0x97f5c)) SL(0,0x9749a);
        CHECK;
        if (step(a,e)<0) return GA_UNSUPPORTED;
        uint8_t t=ga_attack(a,e,0x963e8);CHECK;
        if (t==2u || t==3u) SL(0,0x9749a);
        if ((int16_t)W(0x40)<=0x120) SL(0,0x9749a);
        return GA_MORE;
    }
    case 0x97308:
        SB(0x84,1);
        if (!program(a,e,0x14)) return GA_UNSUPPORTED;
        SL(0,0x9731a);
        /* fall through */
    case 0x9731a:
        fall(a,0xffff0000u);
        neo_slug_k143ee(a,e);CHECK;
        if (BIT(0x15,6)) SL(0,0x9737e);
        if ((r=step(a,e))<0) return GA_UNSUPPORTED;
        if (r) {
            SL(0,0x96f6a);
            if (BIT(0x15,6)) {SL(0,0x970fc);k1b1dc(a);}
        } else if (BIT(0x15,6)) k1b1dc(a);
        return tail(a,e);
    case 0x9737e:
        k1b1dc(a);SB(0x84,1);
        if (!program(a,e,0x10)) return GA_UNSUPPORTED;
        SL(0,0x97396);
        /* fall through */
    case 0x97396:
        read_pad(a,e);
        neo_slug_k14594(a,e);CHECK;depth(a);
        if ((r=step(a,e))<0) return GA_UNSUPPORTED;
        if (r) SL(0,0x96c22);
        if (!BIT(0x15,6)) {SB(0x8e,0x2a);SB(0x5f,B(0x5f)|0x40u);SL(0,0x96f6a);}
        return tail(a,e);
    case 0x973e2: {
        /* 0973E2 (B50): wrecked on the ground with its rider: program 0x38,
         * sound 10E9, the marker 03533A (0353C2 at 0, 0x40). Then 097414
         * each update: B (+85 bit 5) ejects the rider (0AA7FC kind 1); a
         * rider no longer bound (0AA760 fails) leaves it empty (+6E = FFFF,
         * program 0x3C, +92 = 1); the death palette 097EDE ends in 09756A.
         * Before: an explicit stop here killed the node with the player
         * still bound to its slot, which froze the player for good. */
        SB(0x14,B(0x1a));k1b1dc(a);SB(0x84,1);
        if (!program(a,e,0x38)) return GA_UNSUPPORTED;
        sound(e,0x10e9);
        neo_gactor_t *m=e->spawn?e->spawn(e->ctx,a,0x3533a,false):NULL;
        if (m) {ga_sw(m,0x6e,0);ga_sw(m,0x70,0x40);}
        SL(0,0x97414);
    }   /* fall through */
    case 0x97414: {
        if ((int16_t)W(0x6e)>=0) read_pad(a,e); else SB(0x85,(uint8_t)e->vehicle->input[1][0]);
        SL(0x30,0);SL(0x34,L(0x34)+0xffffb000u);
        if ((int16_t)W(0x6e)<0) ga_air(a,e);                            /* 0143E4 */
        else {
            if (B(0x85)&0x20u) {neo_vslot_exit(v->slots,rider_p(a),a->m,node_word(e,a),1);touched(a);}
            neo_vslot_player_t pl=player(e,rider_p(a));uint16_t p6e;
            if (!neo_vslot_bind(v->slots,rider_p(a),a->m,node_word(e,a),&pl,e->video,0,0,&p6e)) {
                SW(0x6e,0xffff);
                if (!program(a,e,0x3c)) return GA_UNSUPPORTED;
                SB(0x92,1);
            }
            touched(a);
            (void)neo_slug_k143ee(a,e);
        }
        CHECK;
        if (step(a,e)<0) return GA_UNSUPPORTED;
        if (fade(a,e,0x28,0x53,0x97ee8)) SL(0,0x9756a);
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    }
    case 0x9749a:
        SW(0x78,0x8000);SB(0x80,0);SW(0x7c,0x8000);SB(0x81,0);
        {neo_gactor_t *f=e->spawn?e->spawn(e->ctx,a,0xae1dc,true):NULL;if (f) {ga_sb(f,0x70,0);ga_sw(f,0x6e,1);}}   /* 0AE220 (1, 0) */
        SL(0,0x974c0);
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    case 0x974c0: {
        if (e->kill_children) e->kill_children(e->ctx,a);              /* 004192 */
        for (unsigned i=0;i<16;i++) {                                   /* 01DBAA */
            neo_gactor_t *c=child(a,e,0x1d31c,0x2a,false);
            if (c) ga_place_from(c,a,e,0,0x18);
        }
        static const uint32_t piece[4][3]={{0xfff30000u,0x280000u,0xa},{0xb0000u,0x250000u,7},{0xa0000u,0x10000u,7},{0x80000u,0xfffc0000u,5}};
        for (unsigned i=0;i<4;i++) {                                    /* 0AD9E6 */
            neo_gactor_t *q=child(a,e,0xad9b8,0x2a,true);
            if (!q) continue;
            ga_place_at(q,a,e,0,0);
            ga_sl(q,0x30,piece[i][0]);ga_sl(q,0x34,piece[i][1]);ga_sw(q,0x4e,(uint16_t)piece[i][2]);
        }
        neo_gactor_t *b=child(a,e,0xad8a2,0x2a,true);
        if (b) ga_place_at(b,a,e,0,0x10);
        ga_attack(a,e,0x963f8);CHECK;
        goto blown;
    }
    case 0x9756a: {
        neo_vslot_exit(v->slots,rider_p(a),a->m,node_word(e,a),0);touched(a);
        ga_attack(a,e,0x96408);CHECK;
        neo_gactor_t *b=child(a,e,0x1c71e,0x2a,true);
        if (b) ga_place_at(b,a,e,0,0);
    }
    blown:
        sound(e,0x1034);
        if (!program(a,e,0x40)) return GA_UNSUPPORTED;
        SL(0,0x97638);
        ga_project(a,e);
        if (step(a,e)<0) return GA_UNSUPPORTED;
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    case 0x975c6:
        SL(0x30,0);if ((int32_t)L(0x34)>=0) SL(0x34,0);
        SL(0x62,0);SB(0x84,0);
        if (!program(a,e,0x38)) return GA_UNSUPPORTED;
        SL(0,0x975ea);
        /* fall through */
    case 0x975ea: {
        unsigned p=rider_p(a);neo_vslot_player_t pl=player(e,p);uint16_t p6e;
        if (!neo_vslot_bind(v->slots,p,a->m,node_word(e,a),&pl,e->video,0,0,&p6e)) {
            SB(0x92,1);if (!program(a,e,0x3c)) return GA_UNSUPPORTED;
            SL(0,0x9760e);
        }
        touched(a);
    }   /* fall through */
    case 0x9760e:
        SL(0x34,L(0x34)+0xffffb000u);ga_move(a,e);CHECK;
        if (step(a,e)<0) return GA_UNSUPPORTED;
        if (ga_cull(a,e,0x966c8)) SL(0,0x9756a);
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    case 0x97638:
        return GA_DELETE;                                               /* cartridge check, 004188 */
    default: STOP(L(0));
    }
    STOP(L(0));
}
/* ---- children -------------------------------------------------------------- */
/* 097830 / 097886: the pilot in the cockpit. 0966F0 with the walker's
 * +76; for the crouch entries 0x20 / 0x24 (sub-tables) the entry by the
 * walker's +89 is started after (the first 004256 is overwritten). */
static bool pilot_program(neo_gactor_t *a,neo_genv_t *e,neo_gactor_t *w,bool sub) {
    uint16_t d0=ga_w(w,0x76);SW(0x76,d0);
    uint32_t t=rl(L(0x72)+d0);
    if (sub) {uint8_t d1=ga_b(w,0x89);SB(0x89,d1);if (d0==0x20u || d0==0x24u) t=rl(t+d1);}
    if (!t) {ga_fail(e,L(0x72)+d0);return false;}
    return ga_anim_start(a,e,t,0x100);
}
static ga_result_t pilot_tick(neo_gactor_t *a,neo_genv_t *e) {
    neo_gactor_t *w=creator(a,e);
    if (!w) STOP(0x97830);
    if (L(0)==0x97830) {
        ga_place(a,e,0,0,NEO_GA_TAG(w->index));                         /* 01AD6E */
        ga_palette(a,rw(0x34330u+2u*ga_w(w,0x70)));
        SL(0x72,rl(0x9649cu+4u*ga_w(w,0x70)));
        if (!pilot_program(a,e,w,false)) return GA_UNSUPPORTED;
        SW(0x70,ga_w(w,0x70));SL(0,0x97886);
    }
    ga_project(a,e);
    SW(0x1e,ga_w(w,0x1e));
    if (ga_w(w,0x76)!=W(0x76)) {
        if (ga_w(w,0x70)!=W(0x70)) {
            SB(0x1d,0);ga_palette(a,rw(0x34330u+2u*ga_w(w,0x70)));
            SL(0x72,rl(0x9649cu+4u*ga_w(w,0x70)));SW(0x70,ga_w(w,0x70));
        }
        if (!pilot_program(a,e,w,true)) return GA_UNSUPPORTED;
    }
    SB(0x86,ga_b(w,0x86));
    if (!ga_b(w,0x92) && ga_anim_step(a,e,NULL)<0) return GA_UNSUPPORTED;
    if (W(0x76)==0x40u) return GA_DELETE;
    return e->unsupported?GA_UNSUPPORTED:GA_MORE;
}
/* 097972 / 097996: the cannon's muzzle flash. */
static ga_result_t flash_tick(neo_gactor_t *a,neo_genv_t *e) {
    int r;
    if (L(0)==0x97972) {ga_palette(a,3);if (!ga_anim_start(a,e,0x29d978,0x100)) return GA_UNSUPPORTED;SL(0,0x97996);}
    ga_project(a,e);
    if ((r=ga_anim_step(a,e,NULL))<0) return GA_UNSUPPORTED;
    return r?GA_DELETE:(e->unsupported?GA_UNSUPPORTED:GA_MORE);
}
/* 097B24: the walker gets its shell back. */
static void give_back(neo_gactor_t *a,neo_genv_t *e) {
    neo_gactor_t *w=creator(a,e);
    if (w) ga_sb(w,0x8a,(uint8_t)(ga_b(w,0x8a)-1u));
}
/* 097A36 / 097A86: the shell. */
static ga_result_t shell_tick(neo_gactor_t *a,neo_genv_t *e) {
    if (L(0)==0x97a36) {
        SB(0x15,B(0x15)|0x20u);SW(0x66,0xe000);SW(0x1e,0xe000);
        if (!ga_anim_start(a,e,0x290fc0,0x100)) return GA_UNSUPPORTED;
        ga_palette(a,0x258);
        uint32_t vx,vy;ga_polar(0x100000,B(0x6e),&vx,&vy);
        SL(0x30,vx);SL(0x34,0);SL(0,0x97a86);
        return GA_MORE;
    }
    SB(0x79,(uint8_t)(((uint16_t)((int16_t)(int8_t)B(0x6e)>>3))&0x1fu));
    SL(0x34,L(0x34)+0xffffb000u);ga_air(a,e);CHECK;                  /* 0143DC */
    if (BIT(0x15,6)) SL(0,0x97b18);
    bool boom=BIT(0x5f,0)!=0;
    if (!boom) {uint8_t t=ga_attack(a,e,0x963d8);CHECK;boom=t==2u || t==3u;}
    if (boom) SL(0,0x97b02);
    if (ga_anim_step(a,e,NULL)<0) return GA_UNSUPPORTED;
    if (ga_cull_rect(a,e,0x966d0)) {give_back(a,e);return GA_DELETE;}  /* 019504 -> 097B0C */
    return e->unsupported?GA_UNSUPPORTED:GA_MORE;
}
/* 097B02 / 097B18: the shell's ends (the SV-001 shell's burst, or the
 * bomb of type 3 01EC0E on the ground). */
static ga_result_t shell_end(neo_gactor_t *a,neo_genv_t *e) {
    if (L(0)==0x97b02) {give_back(a,e);SL(0,0x94ff8);return neo_tank_tick(a,e);}
    SB(0x1d,0);                                                         /* 009F88 */
    SB(0x88,1);SW(0x6e,0xffff);SL(0,0x1e80e);                           /* 01EC0E */
    return neo_bullet_tick(a,e);
}
/* 097DD0 / 097DF0: dust (program +72). */
static ga_result_t dust_tick(neo_gactor_t *a,neo_genv_t *e) {
    int r;
    if (L(0)==0x97dd0) {SW(0x66,0x1001);SW(0x1e,0x1001);if (!ga_anim_start(a,e,L(0x72),0x100)) return GA_UNSUPPORTED;SL(0,0x97df0);}
    ga_project(a,e);
    if ((r=ga_anim_step(a,e,call))<0) return GA_UNSUPPORTED;
    return r?GA_DELETE:(e->unsupported?GA_UNSUPPORTED:GA_MORE);
}
/* 097C46 / 097C72 and 097D22 / 097D52: the legs' flames on the walker. */
static ga_result_t flame_tick(neo_gactor_t *a,neo_genv_t *e) {
    neo_gactor_t *w=creator(a,e);int r;
    switch (L(0)) {
    case 0x97c46:
        ga_palette(a,0x204);
        if (!w) STOP(0x97c58);
        ga_place(a,e,0,0,NEO_GA_TAG(w->index));                         /* 01AD6E */
        if (!ga_anim_start(a,e,L(0x72),0x100)) return GA_UNSUPPORTED;
        SL(0,0x97c72);
        /* fall through */
    case 0x97c72:
        ga_project(a,e);
        if (!w) STOP(0x97c78);
        SW(0x1e,ga_w(w,0x1e)-1u);
        if ((r=ga_anim_step(a,e,NULL))<0) return GA_UNSUPPORTED;
        return r?GA_DELETE:(e->unsupported?GA_UNSUPPORTED:GA_MORE);
    case 0x97d22:
        ga_palette(a,0x204);SB(0x91,0);
        if (!w) STOP(0x97d38);
        ga_place(a,e,0,0,NEO_GA_TAG(w->index));
        if (!ga_anim_start(a,e,L(0x72),0x100)) return GA_UNSUPPORTED;
        SL(0,0x97d52);
        /* fall through */
    case 0x97d52:
        ga_project(a,e);
        if (!w) STOP(0x97d58);
        if (ga_b(w,0x91)==6u) {
            if (!B(0x91)) {if (!ga_anim_start(a,e,0x29dbd4,0x100)) return GA_UNSUPPORTED;SB(0x91,1);}
            else {if (!ga_anim_start(a,e,0x29dbde,0x100)) return GA_UNSUPPORTED;SB(0x91,0);}
        } else SB(0x91,0);
        SW(0x1e,ga_w(w,0x1e)-1u);
        if (ga_anim_step(a,e,NULL)<0) return GA_UNSUPPORTED;
        if (ga_w(w,0x76)!=W(0x76)) SL(0,0x97c72);
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    }
    STOP(L(0));
}
/* 0AD8A2: the burst of the blown-up walker (then 0AD88A, neo_tank.c). */
static ga_result_t burst_tick(neo_gactor_t *a,neo_genv_t *e) {
    SW(0x66,0xe0ed);SW(0x1e,0xe0ed);ga_palette(a,0x204);
    if (!ga_anim_start(a,e,0x22cf2c,0x100)) return GA_UNSUPPORTED;
    SL(0,0xad88a);return neo_tank_tick(a,e);
}
bool neo_slugnoid_routine(uint32_t r) {
    switch (r) {
    case 0x9692c: case 0x96a10: case 0x96a60: case 0x96b32: case 0x96b62: case 0x96ba6: case 0x96bbe:
    case 0x96c22: case 0x96c3a: case 0x96ccc: case 0x96cea: case 0x96dc4: case 0x96dc8: case 0x96e02:
    case 0x96ea6: case 0x96eaa: case 0x96eea: case 0x96f6a: case 0x96f7a: case 0x96f86: case 0x97028:
    case 0x970fc: case 0x97136: case 0x971ee: case 0x971f8: case 0x972a6: case 0x97308: case 0x9731a:
    case 0x9737e: case 0x97396: case 0x973e2: case 0x97414: case 0x9749a: case 0x974c0: case 0x9756a: case 0x975c6:
    case 0x975ea: case 0x9760e: case 0x97638:
    case 0x97830: case 0x97886: case 0x97972: case 0x97996: case 0x97a36: case 0x97a86: case 0x97b02: case 0x97b18:
    case 0x97c46: case 0x97c72: case 0x97d22: case 0x97d52: case 0x97dd0: case 0x97df0: case 0xad8a2: return true;
    }
    return false;
}
ga_result_t neo_slugnoid_tick(neo_gactor_t *a,neo_genv_t *e) {
    uint32_t r=L(0);
    if (r==0x97830 || r==0x97886) return pilot_tick(a,e);
    if (r==0x97972 || r==0x97996) return flash_tick(a,e);
    if (r==0x97a36 || r==0x97a86) return shell_tick(a,e);
    if (r==0x97b02 || r==0x97b18) return shell_end(a,e);
    if (r==0x97dd0 || r==0x97df0) return dust_tick(a,e);
    if (r>=0x97c46 && r<=0x97d52) return flame_tick(a,e);
    if (r==0xad8a2) return burst_tick(a,e);
    return walker_tick(a,e);
}
