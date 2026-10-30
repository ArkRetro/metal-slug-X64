#include "neo_region.h"
#include "neo_soldier_ai.h"   /* neo_angle (ROM 002EB4) */
/* Rocket launcher soldier 05ED84 (Mission 1 camera 2688, script runner
 * 2D6FC2: one at a time while group 4 is empty), manual translation of ROM
 * 05ED84..05FD48 with its children: the aim flash 060490 (0040BC, lives +82
 * updates), the rockets 05FD8A (after the script actor, program callback
 * 05FCC0), the muzzle 0602E4 (callback 264320 in the bank), the rockets'
 * trail puffs 059C82 (059E98) and the death smoke 0897E2 (neo_armored.c).
 * ROM labels are provenance only.
 *
 * Fields: +6E..+72 spawn words (+6E: the range +70/+72 is absolute, else
 * screen relative, 01AD7C), +74 = 3 (forced by the constructor), +76/+78
 * range, +7A offset inside it (05FB26), +7C wait (rank 2D13A4), +7E decision
 * timer (0x3C) while +8A, +80 shot index, +82 aim length (rank 2D1408),
 * +84 program kept while flinching, +88 stance (FF/00: programs, hurtbox
 * 5EB46/5EB62, solid body 5EB7C/5EB6E), +89 turning, +8B vulnerable to a
 * flinch, +8C stance at the last decision, +8D flinching.
 * States: walk 05EF5C/05EF74 (speed rank 2D1372 towards +76 + +7A, push rows
 * 5EB8A/5EB9A), wait 05F0A8/05F0F6, first wait in range 05F21E/05F25C, stop
 * 05F364/05F39A, turn 05F486/05F4BC, fire 05F5F0/05F5FC/05F694 (flash), reload
 * 05F780/05F7BA (004192 deletes the flash; the program fires the rocket;
 * rank 2D13D6 shots), deaths 05F8D4 (hit type 0x0D) / 05F904 -> 05F94E
 * (sound 1021, then 102F, 16 explosions 01DBAA, smoke 0897E2). Every state
 * ends in 05FD48 (death by +5F bit 7, cull 5EBC6 -> 004188).
 * The cartridge check (2FFFE0) before the deletions is not reproduced (it
 * falls through to 004188 on genuine hardware).
 *
 * Explicit stops: spawn word +6E > 1 (trap #15 of the constructor). The
 * flinch (+8D, programs 5ED16 by 01B206) is translated but not recorded in
 * the captures (the player never hit a live launcher without killing it):
 * +84 keeps the program tag and the resume point is native (004256 with the
 * saved +20). */
#define B(o) ga_b(a,(o))
#define W(o) ga_w(a,(o))
#define L(o) ga_l(a,(o))
#define SB(o,v) ga_sb(a,(o),(uint8_t)(v))
#define SW(o,v) ga_sw(a,(o),(uint16_t)(v))
#define SL(o,v) ga_sl(a,(o),(uint32_t)(v))
#define BIT(o,b) ((B(o)>>(b))&1u)
#define SET(o,b) SB(o,B(o)|(1u<<(b)))
#define FACING BIT(0x15,0)
#define START(t) do { if (!ga_anim_start(a,e,(t),0x100)) return GA_UNSUPPORTED; } while (0)
#define P1_NODE 0x1014c4u
#define P2_NODE 0x101574u
static int16_t S16(uint16_t v) {return (int16_t)v;}
static uint32_t rl(uint32_t address) {const uint8_t *p=ga_rom(address);return p?(uint32_t)p[0]<<24|(uint32_t)p[1]<<16|(uint32_t)p[2]<<8|p[3]:0;}
static uint16_t rw(uint32_t address) {const uint8_t *p=ga_rom(address);return p?(uint16_t)(p[0]<<8|p[1]):0;}
static uint16_t rank(neo_gactor_t *a,neo_genv_t *e,uint32_t table) {
    const uint8_t *t=ga_rom(table);
    if (!t) {ga_fail(e,table);return 0;}
    return ga_rank_word(a,e,t);
}
static void sound(neo_genv_t *e,uint16_t id) {if (e->sound) e->sound(e->ctx,id);}
static bool rk_call(neo_gactor_t *a,neo_genv_t *e,neo_anim_kind_t kind,uint32_t routine,uint16_t parameter);
/* 00411C after the script actor, placed by 01ACF2. */
static neo_gactor_t *after_script(neo_gactor_t *a,neo_genv_t *e,uint32_t routine,int16_t dx,int16_t dy) {
    neo_gactor_t *c=e->spawn?e->spawn(e->ctx,NULL,routine,true):NULL;
    if (c) ga_place_at(c,a,e,dx,dy);
    return c;
}
/* 0143DC / 01456A by the ground flag. */
static void motion(neo_gactor_t *a,neo_genv_t *e) {
    if (!BIT(0x15,6)) {SL(0x34,L(0x34)+0xffffb000u);ga_air(a,e);}
    else {uint32_t k;ga_ground(a,e,&k);}
}
/* 004E76 with the solid box of the stance. */
static void solid(neo_gactor_t *a,neo_genv_t *e) {if (e->solid) e->solid(e->ctx,a,B(0x88)?0x5eb7cu:0x5eb6eu);}
/* 01AF9A: the nearest live player (false when none). */
static bool nearest(neo_gactor_t *a,neo_genv_t *e,uint16_t *x,uint16_t *y) {
    unsigned m=e->players.active_mask&0xffu;
    if (!m) return false;
    bool p2;
    if (m==1u) p2=false; else if (m==2u) p2=true;
    else {
        uint16_t d1=(uint16_t)(e->players.p1x-W(0x3c)),d2=(uint16_t)(e->players.p2x-W(0x3c));
        if (d1&0x8000u) d1=(uint16_t)(0u-d1);
        if (d2&0x8000u) d2=(uint16_t)(0u-d2);
        p2=S16(d2)<S16(d1);
    }
    *x=p2?e->players.p2x:e->players.p1x;*y=p2?e->players.p2y:e->players.p1y;
    return true;
}
/* 05FA5C: the range +76/+78 from the spawn words (01AD7C adds the root X). */
static void k5fa5c(neo_gactor_t *a,neo_genv_t *e) {
    if (W(0x6e)) {SW(0x76,W(0x70));SW(0x78,W(0x72));return;}
    uint16_t rx=0;neo_contact_parent_t r;
    if (ga_parent(e,0x10e968u,&r)) rx=(uint16_t)((uint32_t)r.x>>16);
    SW(0x76,W(0x70)+rx);SW(0x78,W(0x72)+rx);
}
/* 05FB06: carry when +3C lies in the range. */
static bool k5fb06(neo_gactor_t *a) {int16_t x=S16(W(0x3c));return !(x>S16(W(0x78)) || x<S16(W(0x76)));}
/* 05FB26: offset +7A inside the range, pushed 16 px away from the actor. */
static void k5fb26(neo_gactor_t *a,neo_genv_t *e) {
    uint16_t d0=(uint16_t)(W(0x78)-W(0x76));
    if (d0) d0=ga_rand_range(e,d0);
    SW(0x7a,d0);
    d0=(uint16_t)(d0+W(0x76));
    uint16_t d1=W(0x3c);
    if (S16(d0)<S16(d1)) {
        d1=(uint16_t)(d1-d0);
        if (S16(d1)<0x10) {
            uint16_t v=W(0x7a);
            if (v<0x10u) SW(0x7a,0); else SW(0x7a,v-0x10u);
        }
        return;
    }
    d0=(uint16_t)(d0-d1);
    if (S16(d0)<0x10) {
        SW(0x7a,W(0x7a)+0x10u);
        d0=(uint16_t)(W(0x7a)+W(0x76));
        if (S16(d0)>S16(W(0x78))) SW(0x7a,W(0x76)-W(0x78));
    }
}
/* 05FAD8: walk program index (facing, stance, direction). */
static uint16_t k5fad8(neo_gactor_t *a) {
    uint16_t d0=0;
    if (FACING) d0|=1u;
    if (B(0x88)) d0|=2u;
    if (!(L(0x62)&0x80000000u)) d0|=4u;
    return (uint16_t)(d0*4u);
}
/* 05FB94: speed +62 towards +76 + +7A (rank 2D1372 << 8) and its program. */
static ga_result_t k5fb94(neo_gactor_t *a,neo_genv_t *e) {
    uint32_t v=(uint32_t)rank(a,e,0x2d1372)<<8;
    SL(0x62,S16((uint16_t)(W(0x7a)+W(0x76)))<=S16(W(0x3c))?0u-v:v);
    START(rl(0x5ed26u+k5fad8(a)));
    return GA_MORE;
}
/* 05FC2C: carry = turn round (by +74: 2 compares the stance with the one of
 * the last decision, 3 the nearest player's side). */
static bool k5fc2c(neo_gactor_t *a,neo_genv_t *e) {
    uint16_t d0=W(0x74);
    if (d0==0 || d0==1u) return false;
    if (d0==2u) return B(0x88)==B(0x8c);
    if (d0!=3u) return true;
    uint16_t px,py;
    if (!nearest(a,e,&px,&py)) return true;
    if (S16((uint16_t)(py-W(0x40)))<0x40) {
        int16_t x=S16(px);if (x<0) x=(int16_t)-x;
        int16_t d=(int16_t)(x-0x7e),d1=(int16_t)(x-0x70);
        if (d<0) d=(int16_t)-d;
        if (d1<0) d1=(int16_t)-d1;
        if (d<d1) return B(0x88)!=0;
    }
    return B(0x88)==0;
}
/* 05FBEE: with +8A the timer +7E; at zero (not turning nor flinching) turn
 * (05F486) or fire (05F5F0). */
static void k5fbee(neo_gactor_t *a,neo_genv_t *e) {
    if (!B(0x8a)) return;
    uint16_t t=W(0x7e);
    if (t) {SW(0x7e,t-1u);return;}
    SW(0x7e,0);
    if (B(0x89) || B(0x8d)) return;
    SL(0,k5fc2c(a,e)?0x5f486u:0x5f5f0u);
}
/* 05FD48: death by +5F bit 7 (05F8D4 for the hit type 0x0D) and the cull
 * rectangle 5EBC6 (01945C -> 004188). */
static ga_result_t k5fd48(neo_gactor_t *a,neo_genv_t *e) {
    if (!B(0x89) && BIT(0x5f,7)) SL(0,B(0x5a)==0x0du?0x5f8d4u:0x5f904u);
    if (ga_cull(a,e,0x5ebc6)) return GA_DELETE;
    SB(0x5a,0);
    return e->unsupported?GA_UNSUPPORTED:GA_MORE;
}
/* 05EF8E..05EFF4 (every live state): a hit (004C24) while vulnerable (+8B),
 * alive, not turning and not flinching starts the flinch program (table
 * 5ED16 by 01B206 and the stance) and keeps the running program in +84.
 * Then the hurtbox of the stance. */
static ga_result_t hit_block(neo_gactor_t *a,neo_genv_t *e) {
    static const uint8_t flinch[20]={0,0,0,0,0,0,0,0,0,1,1,0,1,1,1,0,0,0,0,0}; /* 01B1F2 */
    if (ga_register(a,e,NULL,NULL) && B(0x8b) && !BIT(0x5f,7) && !B(0x89) && !B(0x8d)) {
        SB(0x8d,0xff);
        a->resume_program=a->program;a->resume_cursor=a->cursor;a->resume_tag=a->program_tag;SL(0x84,a->program_tag);
        uint16_t d7=(B(0x5a)<20u && flinch[B(0x5a)])?4u:0u;
        if (!B(0x88)) d7|=8u;
        START(rl(0x5ed16u+d7));
    }
    SL(0x50,B(0x88)?0x5eb46u:0x5eb62u);
    return GA_MORE;
}
/* 00427A and the resume of the interrupted program (+84) when the flinch
 * program ends; *done = the program completed (carry). */
static ga_result_t step_resume(neo_gactor_t *a,neo_genv_t *e,bool *done) {
    int r=ga_anim_step(a,e,rk_call);
    if (r<0) return GA_UNSUPPORTED;
    *done=r!=0;
    if (r && B(0x8d)) {
        a->program=a->resume_program;a->program_tag=a->resume_tag;a->cursor=a->resume_cursor;
        SW(0x24,0);SW(0x26,0x100);SB(0x18,0);
        if (!a->program) {ga_fail(e,0x5f026);return GA_UNSUPPORTED;}
        SB(0x8d,0);
    }
    return GA_MORE;
}
#define HIT() do { if (hit_block(a,e)!=GA_MORE) return GA_UNSUPPORTED; } while (0)
#define STEPR(d) do { if (step_resume(a,e,&(d))!=GA_MORE) return GA_UNSUPPORTED; } while (0)
/* 05FCC0 (program callback of the reload programs): the rocket 05FD8A after
 * the script actor at the offset of the shot (5ED46/5ED56), +6E = shot,
 * +76 = stance, facing copied. */
static void k5fcc0(neo_gactor_t *a,neo_genv_t *e) {
    sound(e,0x1064);
    uint32_t t=(B(0x88)?0x5ed46u:0x5ed56u)+(uint32_t)((W(0x80)&3u)*4u);
    int16_t dx=S16(rw(t)),dy=S16(rw(t+2u));
    if (FACING) dx=(int16_t)-dx;
    neo_gactor_t *c=after_script(a,e,0x5fd8a,dx,dy);
    if (!c) return;
    ga_sw(c,0x6e,W(0x80)&3u);ga_sb(c,0x76,B(0x88));
    if (FACING) ga_sb(c,0x15,(uint8_t)(ga_b(c,0x15)|1u)); else ga_sb(c,0x15,(uint8_t)(ga_b(c,0x15)&~1u));
}
/* 264320 and its seven siblings (program callbacks of the reload programs,
 * one per shot and stance): the muzzle 0602E4 at the callback's offset
 * towards the facing (common tail 26433E), +80 = shot, +88 = stance. Only
 * 264320 (shot 0 of the standing stance) allocates after the script actor
 * (00411C); the others after the launcher (0040BC). */
static const struct { uint32_t routine; int16_t dx,dy; bool after; } muzzles[]={
    {0x264320,7,0x27,true},{0x264422,0,0x20,false},{0x2644ec,-2,0x27,false},{0x2645b6,-8,0x20,false},
    {0x264680,0,0x13,false},{0x26474a,0,8,false},{0x264814,-6,0x12,false},{0x2648de,-6,8,false}};
static void k264320(neo_gactor_t *a,neo_genv_t *e,unsigned i) {
    int16_t dx=FACING?(int16_t)-muzzles[i].dx:muzzles[i].dx;
    neo_gactor_t *c=e->spawn?e->spawn(e->ctx,muzzles[i].after?NULL:a,0x602e4,muzzles[i].after):NULL;
    if (!c) return;
    ga_place_at(c,a,e,dx,muzzles[i].dy);
    if (FACING) ga_sb(c,0x15,(uint8_t)(ga_b(c,0x15)|1u));
    ga_sw(c,0x80,W(0x80)&3u);ga_sb(c,0x88,B(0x88));
}
static bool rk_call(neo_gactor_t *a,neo_genv_t *e,neo_anim_kind_t kind,uint32_t routine,uint16_t parameter) {
    (void)parameter;
    if (kind!=NEO_ANIM_CALL) return false;
    if (routine==0x5fcc0u) {k5fcc0(a,e);return true;}
    for (unsigned i=0;i<sizeof(muzzles)/sizeof(muzzles[0]);i++) if (muzzles[i].routine==routine) {k264320(a,e,i);return true;}
    return false;
}
static ga_result_t launcher_tick(neo_gactor_t *a,neo_genv_t *e) {
    uint32_t pc=L(0);bool done=false;
    for (unsigned guard=0;guard<64;guard++) switch (pc) {
    case 0x5ed84:
        SW(0x74,3);
        if (S16(W(0x6e))>1) {ga_fail(e,0x5ed98);return GA_UNSUPPORTED;}   /* trap #15 */
        SB(0x89,0);SB(0x8a,0xff);SW(0x7e,0x3c);SB(0x8b,0);SB(0x8d,0);SB(0x88,0xff);
        if (W(0x74)==0) SB(0x88,0);
        if (W(0x74)==1u) SB(0x88,0xff);
        SB(0x8c,B(0x88));
        SW(0x66,0x8800);SW(0x1e,0x8800);
        SW(0x4e,rank(a,e,0x2d1310));SL(0x50,0x5eb46);
        solid(a,e);
        ga_palette(a,0x9c);ga_palette(a,0x9d);ga_palette(a,0x1d);
        SB(0x14,B(0x19));
        ga_flash_palette(a,0x30b);
        ga_project(a,e);
        if (S16(W(0x3c))<0xa0) SET(0x15,0);
        ga_floor(a,e);
        pc=0x5ee94;continue;
    case 0x5ee94:
        SL(0x30,0);SL(0x34,0);ga_air(a,e);
        if (B(0x6c)!=3u) {
            ga_project(a,e);k5fa5c(a,e);
            if (k5fb06(a)) SL(0,0x5f21e); else {SB(0x8b,0);SL(0,0x5ef5c);}
            return e->unsupported?GA_UNSUPPORTED:GA_MORE;
        }
        START(B(0x88)?0x263bc6u:0x263bf4u);SB(0x8b,0xff);SL(0,0x5ef0a);
        /* fall through */
    case 0x5ef0a:
        SL(0x30,0);SL(0x34,0);ga_stationary(a,e);
        if (B(0x6c)!=3u) {pc=0x5ee94;continue;}
        STEPR(done);
        return k5fd48(a,e);
    case 0x5ef5c: {
        ga_project(a,e);k5fa5c(a,e);k5fb26(a,e);
        if (k5fb94(a,e)!=GA_MORE) return GA_UNSUPPORTED;
        SL(0,0x5ef74);
    }   /* fall through */
    case 0x5ef74: {
        motion(a,e);HIT();STEPR(done);k5fa5c(a,e);
        if (!B(0x8d)) {
            int16_t d7=S16((uint16_t)(W(0x7a)+W(0x76))),x=S16(W(0x3c));
            bool reached=(L(0x62)&0x80000000u)?d7>=x:d7<=x;
            if (reached) {SB(0x8a,0xff);SL(0,0x5f364);}
        }
        ga_attack(a,e,(L(0x62)&0x80000000u)?0x5eb8au:0x5eb9au);
        k5fbee(a,e);
        return k5fd48(a,e);
    }
    case 0x5f0a8:
        if (L(0x62)) {pc=0x5f364;continue;}
        SW(0x7c,rank(a,e,0x2d13a4));
        START(B(0x88)?0x263bc6u:0x263bf4u);SB(0x8b,0xff);SL(0,0x5f0f6);
        /* fall through */
    case 0x5f0f6:
        SL(0x62,0);motion(a,e);HIT();STEPR(done);
        if (!B(0x8d)) {
            uint16_t t=W(0x7c);
            if (!t) {
                SW(0x7c,0);SB(0x8b,0);k5fa5c(a,e);
                if (W(0x76)!=W(0x78)) SL(0,0x5ef5c);
                else SL(0,k5fb06(a)?0x5f0a8u:0x5ef5cu);
            } else SW(0x7c,t-1u);
        }
        k5fbee(a,e);
        return k5fd48(a,e);
    case 0x5f21e:
        if (L(0x62)) {pc=0x5f364;continue;}
        START(B(0x88)?0x263bc6u:0x263bf4u);SB(0x8b,0xff);SL(0,0x5f25c);
        /* fall through */
    case 0x5f25c: {
        SL(0x62,0);motion(a,e);HIT();STEPR(done);
        int16_t x=S16(W(0x3c));
        if (x>=0x30 && x<=0x120 && !B(0x8d)) {SB(0x8b,0);SB(0x8a,0xff);SL(0,0x5f0a8);}
        k5fbee(a,e);
        return k5fd48(a,e);
    }
    case 0x5f364:
        SL(0x62,0);START(B(0x88)?0x263d7au:0x263dccu);SL(0,0x5f39a);
        /* fall through */
    case 0x5f39a:
        motion(a,e);HIT();STEPR(done);
        if (done && !B(0x8d)) SL(0,0x5f0a8);
        k5fbee(a,e);
        return k5fd48(a,e);
    case 0x5f486:
        SB(0x89,0xff);START(B(0x88)?0x2640e2u:0x264072u);SL(0,0x5f4bc);
        /* fall through */
    case 0x5f4bc:
        SL(0x62,0);motion(a,e);HIT();solid(a,e);STEPR(done);
        if (done) {SB(0x88,B(0x88)?0:0xff);SB(0x89,0);solid(a,e);SL(0,0x5f0a8);}
        k5fbee(a,e);
        return k5fd48(a,e);
    case 0x5f5f0:
        SL(0x62,0);SB(0x8b,0);SW(0x80,0);
        /* fall through */
    case 0x5f5fc: {
        START(rl((B(0x88)?0x5ecd6u:0x5ece6u)+(uint32_t)((W(0x80)&3u)*4u)));
        SW(0x82,W(0x80)?1u:rank(a,e,0x2d1408));
        neo_gactor_t *c=e->spawn?e->spawn(e->ctx,a,0x60490,false):NULL;   /* 0040BC */
        if (c) {
            ga_place_at(c,a,e,0,0);
            ga_sb(c,0x88,B(0x88));ga_sw(c,0x80,W(0x80)&3u);ga_sw(c,0x82,W(0x82));
            if (FACING) ga_sb(c,0x15,(uint8_t)(ga_b(c,0x15)|1u));
        }
        SL(0,0x5f694);
    }   /* fall through */
    case 0x5f694: {
        motion(a,e);HIT();STEPR(done);
        uint16_t t=W(0x82);
        SW(0x82,t-1u);                                                  /* wraps to FFFF at zero */
        if (!t && !B(0x8d)) SL(0,0x5f780);
        return k5fd48(a,e);
    }
    case 0x5f780:
        if (e->kill_children) e->kill_children(e->ctx,a);                /* 004192 */
        START(rl((B(0x88)?0x5ecf6u:0x5ed06u)+(uint32_t)((W(0x80)&3u)*4u)));
        SL(0,0x5f7ba);
        /* fall through */
    case 0x5f7ba:
        motion(a,e);HIT();STEPR(done);
        if (done && !B(0x8d)) {
            SW(0x80,W(0x80)+1u);
            if (W(0x80)<rank(a,e,0x2d13d6)) SL(0,0x5f5fc);
            else {SB(0x8c,B(0x88));SW(0x7e,0x3c);SL(0,0x5f0a8);}
        }
        return k5fd48(a,e);
    case 0x5f8d4:
        START(B(0x88)?0x264b00u:0x264bcau);
        pc=0x5f930;continue;
    case 0x5f904:
        START(B(0x88)?0x264c94u:0x264e46u);
        /* fall through */
    case 0x5f930:
        SL(0x62,0);
        if (e->solid) e->solid(e->ctx,a,0);
        sound(e,0x1021);SL(0,0x5f94e);
        /* fall through */
    case 0x5f94e:
        motion(a,e);STEPR(done);
        if (done) {
            sound(e,0x102f);
            for (unsigned i=0;i<16;i++) ga_spawn_from(a,e,0x1d31c,0,0x18);  /* 01DBAA */
            after_script(a,e,0x897e2,0,0);
            return GA_DELETE;                                           /* 004188 */
        }
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    default: ga_fail(e,pc);return GA_UNSUPPORTED;
    }
    ga_fail(e,pc);return GA_UNSUPPORTED;
}
/* 060490: the aim flash (program by the shot and the stance, 60348/6035C),
 * deleted with a dead or gone creator or after +82 updates. */
static ga_result_t flash_tick(neo_gactor_t *a,neo_genv_t *e) {
    switch (L(0)) {
    case 0x60490:
        START(rl((B(0x88)?0x60348u:0x6035cu)+(uint32_t)((W(0x80)&3u)*4u)));
        SW(0x66,0x1001);SW(0x1e,0x1001);ga_palette(a,0x9d);sound(e,0x10a3);SL(0,0x604e8);
        /* fall through */
    case 0x604e8: {
        ga_project(a,e);
        neo_gactor_t *c=e->creator?e->creator(e->ctx,a):NULL;
        if (c && (ga_b(c,0x5f)&0x80u)) return GA_DELETE;
        if (!ga_creator_alive(a,e)) return GA_DELETE;                   /* 00409A */
        int r=ga_anim_step(a,e,NULL);
        if (r<0) return GA_UNSUPPORTED;
        uint16_t t=W(0x82);
        if (!t) return GA_DELETE;
        SW(0x82,t-1u);
        return GA_MORE;
    }
    }
    ga_fail(e,L(0));return GA_UNSUPPORTED;
}
/* 0602E4: the muzzle (program 601A0/60242 by the stance), gone with it. */
static ga_result_t muzzle_tick(neo_gactor_t *a,neo_genv_t *e) {
    switch (L(0)) {
    case 0x602e4:
        START(B(0x88)?0x601a0u:0x60242u);
        SW(0x66,0xe0f0);SW(0x1e,0xe0f0);ga_palette(a,0x9d);SL(0,0x60330);
        /* fall through */
    case 0x60330: {
        ga_project(a,e);
        int r=ga_anim_step(a,e,NULL);
        if (r<0) return GA_UNSUPPORTED;
        return r?GA_DELETE:GA_MORE;
    }
    }
    ga_fail(e,L(0));return GA_UNSUPPORTED;
}
/* 059E98: a trail puff 059C82 after the script actor at (dx,dy): it starts
 * with the rocket's speed (see below) and decelerates by a 32nd of it
 * (+6E/+72). */
static void k59e98(neo_gactor_t *a,neo_genv_t *e,int16_t dx,int16_t dy) {
    neo_gactor_t *c=e->spawn?e->spawn(e->ctx,NULL,0x59c82,true):NULL;
    if (!c) return;
    int32_t vx=(int32_t)L(0x30),vy=(int32_t)L(0x34);
    ga_sl(c,0x6e,(uint32_t)(vx>>5));ga_sl(c,0x30,(uint32_t)(0-vx));
    ga_sl(c,0x72,(uint32_t)(vy>>5));ga_sl(c,0x34,(uint32_t)(0-vy));
    /* 01ACF2 after the writes: it leaves the placement delta (the rocket's
     * own motion) in +30/+34, so the puff keeps the rocket's speed. */
    ga_place_at(c,a,e,dx,dy);
}
/* 05FD8A: the rocket (speed by the stance +76, angle byte +77 from the
 * velocity (002EB4 >> 2) once in the air, pose by +77, attack row 600A0 by
 * +77, trail puff every other update (5EBD6 by +77), gravity 0xB000) until
 * the ground, a wall (+5F bit 0), a hit (+5F bit 7) or a contact: explosion
 * 01C706 in place (009F88 frees the palettes). */
static ga_result_t rocket_tick(neo_gactor_t *a,neo_genv_t *e) {
    switch (L(0)) {
    case 0x5fd8a:
        SET(0x15,5);
        if (B(0x76)) {SL(0x30,0xfffd0000u);SL(0x34,0x28000);SB(0x77,0x18);}
        else {SL(0x30,0xfffa8000u);SL(0x34,0);SB(0x77,0x20);}
        if (FACING) SL(0x30,0u-L(0x30));
        SB(0x78,0);SW(0x66,0xe0ef);SW(0x1e,0xe0ef);
        SW(0x4e,rank(a,e,0x2d143a));SL(0x50,0x5ebaa);
        ga_palette(a,0x9c);START(0x2648fa);
        SB(0x5e,B(0x5e)|0x80u);                                         /* 004C1C */
        SL(0,0x5fe28);
        /* fall through */
    case 0x5fe28: {
        ga_air(a,e);
        if (!BIT(0x15,6)) {
            int16_t d0=(int16_t)((int32_t)L(0x30)>>12),d1=(int16_t)((int32_t)L(0x34)>>12);
            SB(0x77,(uint8_t)(neo_angle(d0,d1)>>2));
        }
        int r=ga_anim_step(a,e,NULL);
        if (r<0) return GA_UNSUPPORTED;
        bool blow=BIT(0x15,6) || BIT(0x5f,0) || BIT(0x5f,7);
        if (!blow) {
            ga_register(a,e,NULL,NULL);SB(0x5a,0);
            if (ga_attack(a,e,rl(0x600a0u+(uint32_t)B(0x77)*4u))) blow=true;
            else if (ga_cull(a,e,0x5ebc6)) return GA_DELETE;
            else {
                SL(0x34,L(0x34)+0xffffb000u);
                if (!(B(0x78)&1u)) {
                    uint32_t t=0x5ebd6u+(uint32_t)((B(0x77)&0x3fu)*4u);
                    int16_t dx=S16(rw(t)),dy=S16(rw(t+2u));
                    if (!FACING) dx=(int16_t)-dx;
                    k59e98(a,e,dx,(int16_t)-dy);
                }
                SB(0x78,(B(0x78)&1u)+1u);
                return e->unsupported?GA_UNSUPPORTED:GA_MORE;
            }
        }
        /* 05FF7E */
        sound(e,0x1022);SW(0x66,0xe0ed);SW(0x1e,0xe0ed);
        SB(0x1d,0);                                                     /* 009F88 */
        SL(0,0x1c706);
        return neo_effects_tick(a,e);                                   /* jmp 01C706 */
    }
    }
    ga_fail(e,L(0));return GA_UNSUPPORTED;
}
/* 059C82: the trail puff (program 247DA8, palette 0x37C): decelerates to a
 * stop, then its program ends (004188). The creator here is the script
 * actor (00411C): its facing bit is clear. */
static ga_result_t puff_tick(neo_gactor_t *a,neo_genv_t *e) {
    switch (L(0)) {
    case 0x59c82: {
        SB(0x78,0xff);SB(0x79,0xff);START(0x247da8);ga_palette(a,0x37c);SW(0x76,0x1e);
        ga_project(a,e);
        neo_gactor_t *c=e->creator?e->creator(e->ctx,a):NULL;
        if (c && (ga_b(c,0x15)&1u)) SET(0x15,0);
        SW(0x66,0xe0ed);SW(0x1e,0xe0ed);SL(0,0x59cda);
    }   /* fall through */
    case 0x59cda: case 0x59d00:
        if (L(0)==0x59cda) {
            if (!B(0x79)) {
                SW(0x76,W(0x76)-1u);
                if (!W(0x76)) {SET(0x5f,3);SW(0x76,4);SL(0,0x59d00);}
            }
        } else {
            SW(0x76,W(0x76)-1u);
            if (!W(0x76)) return GA_DELETE;
        }
        for (;;) {
            /* 059D0E: the speeds towards zero. */
            for (unsigned o=0;o<2;o++) {
                unsigned acc=o?0x72u:0x6eu,vel=o?0x34u:0x30u;
                int32_t d7=(int32_t)L(acc),v=(int32_t)((uint32_t)L(vel)+(uint32_t)d7);
                SL(vel,(uint32_t)v);
                if (d7>=0) {if (v>=0) {SL(acc,0);SL(vel,0);}}
                else if (v<0) {SL(acc,0);SL(vel,0);}
            }
            if (!B(0x79) && !BIT(0x5f,3) && !L(0x30) && !L(0x34)) {
                SET(0x5f,3);SW(0x76,4);SL(0,0x59d00);continue;         /* 059CEA */
            }
            break;
        }
        ga_move(a,e);
        {int r=ga_anim_step(a,e,NULL);
         if (r<0) return GA_UNSUPPORTED;
         if (r && B(0x78)) return GA_DELETE;}
        return GA_MORE;
    }
    ga_fail(e,L(0));return GA_UNSUPPORTED;
}
static bool launcher(uint32_t r) {
    static const uint32_t own[]={0x5ed84,0x5ef0a,0x5ef5c,0x5ef74,0x5f0a8,0x5f0f6,0x5f21e,0x5f25c,0x5f364,0x5f39a,
        0x5f486,0x5f4bc,0x5f5f0,0x5f5fc,0x5f694,0x5f780,0x5f7ba,0x5f8d4,0x5f904,0x5f94e};
    for (unsigned i=0;i<sizeof(own)/sizeof(own[0]);i++) if (own[i]==r) return true;
    return false;
}
bool neo_rocket_routine(uint32_t r) {
    return launcher(r) || r==0x60490 || r==0x604e8 || r==0x602e4 || r==0x60330 || r==0x5fd8a || r==0x5fe28 || r==0x59c82 || r==0x59cda || r==0x59d00;
}
ga_result_t neo_rocket_tick(neo_gactor_t *a,neo_genv_t *e) {
    uint32_t r=L(0);
    if (launcher(r)) return launcher_tick(a,e);
    if (r==0x60490 || r==0x604e8) return flash_tick(a,e);
    if (r==0x602e4 || r==0x60330) return muzzle_tick(a,e);
    if (r==0x5fd8a || r==0x5fe28) return rocket_tick(a,e);
    return puff_tick(a,e);
}
