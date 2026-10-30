#include "neo_region.h"
#include "neo_target.h"
/* Armoured vehicle of Mission 1 (camera stop 1304, spawn-table entry
 * 087E36, group 1), manual translation; ROM labels are provenance only.
 *
 * Spawn words: +6E (0: +70/+72 relative to the scene root, else screen X),
 * +70/+72 the X range where it stops (+70 <= +72, the constructor traps
 * otherwise), +74 (the wreck keeps a platform and stays), +76/+78 the item
 * dropped when destroyed (08CDB2). Constructor: health and three rank words
 * (+96 shots per burst, +98 delay between shots, +9A delay before a burst),
 * hurtbox 0E4E84, solid body 004E76 (0E4E90), moving platform 0140C4 with
 * the map 0E4FF2 (+A0) at the offset +A4/+A6 the programs set (callback
 * 089412, table 0E5104). It drives in (0880xx) attacking with the row
 * 0E4E9E, stops (0881E6), waits (08833A), aims (0888C8), fires bursts
 * (08890E: shell 0896A0, flash 088B52, smoke 088ED8 after the script actor)
 * and backs away from a player in front of it (088AD2). Shots of the types
 * of 01B1F2 make it flinch (program 2846BA, then the interrupted program
 * resumes). Destroyed (+5F bit 7, 089152): 0883E8 (or 0886EC for the hit
 * type 0x0D), 019DFC leaves its group, six pieces 08943C, debris, the
 * platform is released and the wreck map (if +74) attached; at the end of
 * the program the item drop 08917C and, with +74 = 0, the final explosion
 * 088598 (debris 0E5060, eight explosions 01DBAE, wreck 0897E2).
 *
 * The copy-protection check that precedes several deletions (writes to
 * 2FFFE0, branch to 087E30 on a failed check) is not reproduced: on the
 * original board it falls through to 004188. The resume point of the
 * flinch (+86 = +20 in the original) is kept natively; +86 holds the
 * interrupted program's tag. */
#define B(o) ga_b(a,(o))
#define W(o) ga_w(a,(o))
#define L(o) ga_l(a,(o))
#define SB(o,v) ga_sb(a,(o),(uint8_t)(v))
#define SW(o,v) ga_sw(a,(o),(uint16_t)(v))
#define SL(o,v) ga_sl(a,(o),(uint32_t)(v))
#define BIT(o,b) ((B(o)>>(b))&1u)
#define START(t) do { if (!ga_anim_start(a,e,(t),0x100)) return GA_UNSUPPORTED; } while (0)
static int16_t S16(uint16_t v) {return (int16_t)v;}
static bool armored_call(neo_gactor_t *a,neo_genv_t *e,neo_anim_kind_t kind,uint32_t routine,uint16_t parameter);
static uint16_t rank(neo_gactor_t *a,neo_genv_t *e,uint32_t table) {
    const uint8_t *t=ga_rom(table);
    if (!t) {ga_fail(e,table);return 0;}
    return ga_rank_word(a,e,t);
}
static void sound(neo_genv_t *e,uint16_t id) {if (e->sound) e->sound(e->ctx,id);}
/* 00411C after the script actor, placed by 01ACF2. */
static neo_gactor_t *after_script(neo_gactor_t *a,neo_genv_t *e,uint32_t routine,int16_t dx,int16_t dy) {
    neo_gactor_t *c=e->spawn?e->spawn(e->ctx,NULL,routine,true):NULL;
    if (c) ga_place_at(c,a,e,dx,dy);
    return c;
}
/* 0140C4 with the map +A0 at (dx,dy). */
static void platform(neo_gactor_t *a,neo_genv_t *e,bool offset) {
    uint32_t map=L(0xa0);
    if (map && e->dyn_set) e->dyn_set(e->ctx,a,map,offset?S16(W(0xa4)):0,offset?S16(W(0xa6)):0);
}
static void k1b1dc(neo_gactor_t *a) {SL(0x30,0);SL(0x34,0);SL(0x62,0);}
/* 0146A8: ground step; a non-flat contact restores the position and sets
 * +5F bit 0. */
static void k146a8(neo_gactor_t *a,neo_genv_t *e) {
    uint32_t x=L(0x28),y=L(0x2c),p=L(0x38),kind=0;
    ga_ground(a,e,&kind);
    if ((B(0x15)&64u) && (uint16_t)kind!=0x4000u) {SL(0x38,p);SL(0x2c,y);SL(0x28,x);SB(0x5f,B(0x5f)|1u);ga_project(a,e);}
}
/* 08919C: the stop range +7A..+7C (relative to the scene root when +6E = 0,
 * 01AD7C). */
static void k8919c(neo_gactor_t *a,neo_genv_t *e) {
    if (W(0x6e)) {SW(0x7a,W(0x70));SW(0x7c,W(0x72));return;}
    neo_contact_parent_t root;uint16_t rx=0;
    if (ga_parent(e,0x10e968u,&root)) rx=(uint16_t)((uint32_t)root.x>>16);
    SW(0x7a,W(0x70)+rx);SW(0x7c,W(0x72)+rx);
}
/* 089132: carry when +7A <= X <= +7C. */
static bool k89132(neo_gactor_t *a) {int16_t x=S16(W(0x3c));return !(x>S16(W(0x7c))) && !(x<S16(W(0x7a)));}
/* 0893D0: carry when a player inside the rectangle 0E4ED6 stands in front. */
static bool k893d0(neo_gactor_t *a,neo_genv_t *e) {
    const uint8_t *r=ga_rom(0xe4ed6);
    if (!r) {ga_fail(e,0xe4ed6);return false;}
    int16_t q[4];for (unsigned i=0;i<4;i++) q[i]=(int16_t)(r[2*i]<<8|r[2*i+1]);
    neo_contact_actor_t c=ga_contact(a);
    neo_target_t t=neo_target_pick(&c,&e->players,q);  /* 01AFFC */
    if (!t.valid) return false;
    if (S16(t.x)>S16(W(0x3c))) return !BIT(0x15,0);
    return BIT(0x15,0)!=0;
}
/* 0892BC: on screen and stopped (+A9), +7E counts up to +9A; then the
 * burst starts (unless flinching). */
static void k892bc(neo_gactor_t *a) {
    int16_t x=S16(W(0x3c));
    if (x<0x18 || x>0x128 || !B(0xa9)) return;
    SW(0x7e,W(0x7e)+1u);
    if (W(0x7e)<=W(0x9a)) return;
    SW(0x7e,W(0x9a));
    if (!B(0xaa)) SL(0,0x8889c);
}
/* 0892FE: hurtbox, flinch, ground step, stop range and program step (+AC:
 * the program ended). Returns -1 when the program fails. */
static int k892fe(neo_gactor_t *a,neo_genv_t *e) {
    static const uint8_t flinch[20]={0,0,0,0,0,0,0,0,0,1,1,0,1,1,1,0,0,0,0,0}; /* 01B1F2 */
    if (ga_register(a,e,NULL,NULL) && !B(0xaa)) {
        /* +86 = +20: the interrupted program, resumed below. */
        a->resume_program=a->program;a->resume_cursor=a->cursor;a->resume_tag=a->program_tag;SL(0x86,a->program_tag);
        if (B(0x5a)<20u && flinch[B(0x5a)]) {                         /* 01B206 */
            if (!ga_anim_start(a,e,0x2846ba,0x100)) return -1;
            SB(0xaa,0xff);
        }
    }
    if (!B(0xaa)) k146a8(a,e);
    else {uint32_t v=L(0x62);SL(0x62,0);k146a8(a,e);SL(0x62,v);}
    k8919c(a,e);
    int r=ga_anim_step(a,e,armored_call);
    if (r<0) return -1;
    SB(0xac,r?0xff:0);
    if (r && B(0xaa)) {                                                 /* 004256 with +86 */
        a->program=a->resume_program;a->program_tag=a->resume_tag;a->cursor=a->resume_cursor;
        SW(0x24,0);SW(0x26,0x100);SB(0x18,0);
        if (!a->program) {ga_fail(e,0x89376);return -1;}
        SB(0xaa,0);
    }
    return 0;
}
/* 089152: destroyed (+5F bit 7): 0886EC for the hit type 0x0D, else 0883E8. */
static void k89152(neo_gactor_t *a) {
    if (B(0x5f)&0x80u) SL(0,B(0x5a)==0xdu?0x886ec:0x883e8);
    SB(0x5a,0);
}
/* 089904: exhaust smoke 08982A. */
static void k89904(neo_gactor_t *a,neo_genv_t *e) {
    neo_gactor_t *c=after_script(a,e,0x8982a,BIT(0x15,0)?-10:10,0);
    if (c && BIT(0x15,0)) ga_sb(c,0x15,(uint8_t)(ga_b(c,0x15)|1u));
}
/* 088978: one shot: shell 0896A0, flash 088B52, smoke 088ED8. */
static void k88978(neo_gactor_t *a,neo_genv_t *e) {
    bool f=BIT(0x15,0)!=0;
    sound(e,0x1067);
    neo_gactor_t *c=after_script(a,e,0x896a0,f?-0x20:0x20,0x50);
    if (c && f) ga_sb(c,0x15,(uint8_t)(ga_b(c,0x15)|1u));
    c=after_script(a,e,0x88b52,f?0x22:-0x22,0x45);
    if (c && f) ga_sb(c,0x15,(uint8_t)(ga_b(c,0x15)|1u));
    c=after_script(a,e,0x88ed8,0,0x20);
    if (c && !f) ga_sb(c,0x15,(uint8_t)(ga_b(c,0x15)|1u));
}
/* 01DA40: debris emitter 01D764 with the list `list`. */
static neo_gactor_t *debris(neo_gactor_t *a,neo_genv_t *e,uint32_t list) {
    neo_gactor_t *c=ga_spawn_from(a,e,0x1d764,0,0);
    if (!c) return NULL;
    if (B(0x15)&4u) ga_sb(c,0x15,(uint8_t)(ga_b(c,0x15)|4u));
    ga_sl(c,0x86,list);
    return c;
}
/* 08869C: six pieces 08943C (random entry of the table 089554: offset and
 * speed), then the debris 0E50B2 behind the wreck. */
static void k8869c(neo_gactor_t *a,neo_genv_t *e) {
    for (unsigned i=0;i<6;i++) {
        uint32_t entry=0x89554u+((uint32_t)(ga_rand(e)&0xfu)<<4);     /* 0886CE */
        const uint8_t *t=ga_rom(entry);
        if (!t) {ga_fail(e,entry);return;}
        int16_t dx=(int16_t)(t[0]<<8|t[1]),dy=(int16_t)(t[2]<<8|t[3]);
        if (BIT(0x15,0)) dx=(int16_t)-dx;
        neo_gactor_t *c=after_script(a,e,0x8943c,dx,dy);               /* 089654 */
        if (!c) continue;
        uint32_t vx=(uint32_t)(t[4]<<24|t[5]<<16|t[6]<<8|t[7]),vy=(uint32_t)(t[8]<<24|t[9]<<16|t[10]<<8|t[11]);
        ga_sl(c,0x30,BIT(0x15,0)?0u-vx:vx);ga_sl(c,0x34,vy);
        ga_sb(c,0x12,0x40);
    }
    neo_gactor_t *c=debris(a,e,0xe50b2);
    if (c) {ga_sw(c,0x66,0xe0ec);ga_sw(c,0x1e,0xe0ec);}
}
/* 08917C -> 08CDB2: the item +76 (kind) / +78 after the script actor. */
static void k8917c(neo_gactor_t *a,neo_genv_t *e) {
    uint16_t kind=W(0x76),d3=W(0x78);
    if (kind<0x5au) {
        neo_gactor_t *c=e->spawn?e->spawn(e->ctx,NULL,0x8c374,true):NULL;
        if (c) {
            ga_sb(c,0x8b,0);ga_sw(c,0x70,d3);ga_sw(c,0x6e,kind);ga_sw(c,0x72,0);ga_sb(c,0x7f,0);
            ga_place_at(c,a,e,-8,0x48);
        }
    }
    SW(0x76,0xffff);
}
static bool armored_call(neo_gactor_t *a,neo_genv_t *e,neo_anim_kind_t kind,uint32_t routine,uint16_t parameter) {
    if (kind!=NEO_ANIM_CALL) return false;
    if (routine==0x89412) { /* platform offset +A4/+A6 from the table 0E5104 */
        const uint8_t *t=ga_rom(0xe5104u+2u*parameter);
        if (!t) return false;
        int8_t dx=(int8_t)t[0];if (BIT(0x15,0)) dx=(int8_t)-dx;
        SW(0xa4,(uint16_t)(int16_t)dx);SW(0xa6,(uint16_t)(int16_t)(int8_t)t[1]);
        return true;
    }
    if (routine==0x8938a) { /* ten pieces 088F20 (+84 = 9..0) */
        for (int i=9;i>=0;i--) {
            neo_gactor_t *c=after_script(a,e,0x88f20,0,0);
            if (!c) continue;
            ga_sb(c,0x5b,B(0x5b));ga_sw(c,0x84,(uint16_t)i);
            if (BIT(0x15,0)) ga_sb(c,0x15,(uint8_t)(ga_b(c,0x15)|1u));
        }
        return true;
    }
    return false;
}
/* The common tail: burst timer, platform, destruction check and cull. */
static ga_result_t tail(neo_gactor_t *a,neo_genv_t *e,bool cull) {
    k892bc(a);platform(a,e,true);k89152(a);
    if (cull && ga_cull(a,e,0xe4ece)) return GA_DELETE;               /* 01945C -> 004188 */
    return GA_MORE;
}
static ga_result_t armored_tick(neo_gactor_t *a,neo_genv_t *e) {
    for (unsigned guard=0;guard<8;guard++) switch (L(0)) {
    case 0x87e36: /* constructor */
        if (W(0x72)<W(0x70)) {ga_fail(e,0x87e42);return GA_UNSUPPORTED;}  /* trap #15 */
        SW(0x4e,rank(a,e,0x2d0e40));SW(0x96,rank(a,e,0x2d0ea2));SW(0x98,rank(a,e,0x2d0f04));SW(0x9a,rank(a,e,0x2d0f66));
        SL(0x50,0xe4e84);
        if (e->solid) e->solid(e->ctx,a,0xe4e90);
        ga_palette(a,0x24);ga_flash_palette(a,0x30b);
        SW(0x7e,0);SB(0xa9,0);SB(0xaa,0);
        SW(0x66,0x8800);SW(0x1e,0x8800);
        ga_floor(a,e);
        if (S16(W(0x3c))<0xa0) SB(0x15,B(0x15)|1u);
        /* 03C532 -> 019240: RAM 10C2F6 = 0 (Mission 1) clears the carry: no 03C62A. */
        SL(0xa0,0xe4ff2);platform(a,e,false);
        SL(0,0x87f2e);
        return GA_MORE;
    case 0x87f2e:
        k1b1dc(a);ga_stationary(a,e);
        if (B(0x6c)==3u) { /* 01B284 */
            k1b1dc(a);SB(0xa9,0xff);START(0x2841b4);SB(0xaa,0);
            SL(0,0x87f9a);continue;
        }
        { neo_gactor_t *c=e->spawn?e->spawn(e->ctx,a,0x40396,false):NULL; /* 04036C */
          if (c) {ga_sw(c,0x6e,0x5c);ga_place(c,e,0x18,-8,NEO_GA_TAG(a->index));} }
        k8919c(a,e);
        if (k89132(a)) {SB(0xab,0);goto stop;}
        sound(e,0x1072);
        SL(0,0x8806e);continue;
    case 0x87f9a:
        if (k892fe(a,e)<0) return GA_UNSUPPORTED;
        if (B(0x6c)!=3u) SL(0,0x87f2e);
        return tail(a,e,true);
    case 0x8806e: { /* drive towards a random X of the stop range */
        SB(0xa8,0);SL(0x62,0);
        k146a8(a,e);k8919c(a,e);
        if (k89132(a) && W(0x7c)==W(0x7a)) {SL(0,0x8830c);continue;}
        { uint16_t d0=(uint16_t)(W(0x7c)-W(0x7a));if (d0) d0=ga_rand_range(e,d0);SW(0x92,d0);} /* 089218 */
        uint16_t d0=rank(a,e,B(0xa9)?0x2d0ffa:0x2d0fc8);
        if (S16((uint16_t)(W(0x7a)+W(0x92)))<S16(W(0x3c))) d0=(uint16_t)-d0;
        uint32_t v=(uint32_t)(int32_t)S16(d0)<<8;
        SL(0x62,v);SL(0x9c,v);
        int16_t s=S16((uint16_t)(v>>16));if (!BIT(0x15,0)) s=(int16_t)-s;
        START(s<0?0x284416:0x2842de);SB(0xaa,0);
        SL(0,0x88120);
    }   /* fall through */
    case 0x88120: {
        SL(0x62,L(0x9c));
        SB(0xa8,(B(0xa8)+1u)&7u);
        if (!B(0xa8)) k89904(a,e);
        if (k892fe(a,e)<0) return GA_UNSUPPORTED;
        int16_t d7=S16((uint16_t)(W(0x92)+W(0x7a))),x=S16(W(0x3c));
        if (L(0x62)&0x80000000u) {if (d7>=x) SL(0,0x881e6);}
        else if (d7<=x) SL(0,0x881e6);
        if ((L(0x62)&0x80000000u)?!BIT(0x15,0):BIT(0x15,0)) ga_attack(a,e,0xe4e9e);
        return tail(a,e,true);
    }
    case 0x881e6: { /* stop */
        static const uint32_t programs[4]={0x2841de,0x28425e,0x28425e,0x2841de}; /* 0881D6 */
        SB(0xa9,0xff);
        if (!L(0x62)) {SL(0,0x8830c);continue;}
        unsigned d7=(L(0x62)&0x80000000u)?0u:1u;
        if (BIT(0x15,0)) d7|=2u;
        START(programs[d7]);SB(0xaa,0);SL(0x62,0);
        SL(0,0x88232);
    }   /* fall through */
    case 0x88232:
        if (k892fe(a,e)<0) return GA_UNSUPPORTED;
        if (B(0xac) && !B(0xaa)) SL(0,0x8830c);
        return tail(a,e,true);
    case 0x8830c:
        SB(0xab,1);SW(0x84,0x1e);
    stop:
        SL(0x62,0);SB(0xa9,0xff);START(0x2841b4);SB(0xaa,0);
        SL(0,0x8833a);
        /* fall through */
    case 0x8833a: { /* wait */
        if (k892fe(a,e)<0) return GA_UNSUPPORTED;
        bool go=false;
        if (B(0xab)==0u) {int16_t x=S16(W(0x3c));go=x>=0x30 && x<=0x120;}
        else if (B(0xab)==1u) {SW(0x84,W(0x84)-1u);go=!W(0x84);}
        if (!go) return tail(a,e,false);
        k8919c(a,e);
        SL(0,k893d0(a,e)?0x88ad2:0x8806e);
        return tail(a,e,true);
    }
    case 0x883e8: case 0x886ec: { /* destroyed */
        bool d=L(0)==0x886ec;
        START(d?0x284828:0x2847de);SB(0xaa,0);
        sound(e,0x1021);
        SB(0x69,0xff);                                                  /* 019DFC */
        SB(0x17,0xff);
        k8869c(a,e);
        if (e->dyn_release) e->dyn_release(e->ctx,a);                   /* 013F1E */
        if (W(0x74)) SL(0xa0,d?(BIT(0x15,0)?0xe4fc0:0xe4f8e):(BIT(0x15,0)?0xe4f36:0xe4ede));
        else SL(0xa0,0);
        if (e->solid) e->solid(e->ctx,a,0);
        SL(0,d?0x88760:0x8845c);
    }   /* fall through */
    case 0x8845c: case 0x88760: {
        uint32_t next=L(0)==0x8845c?0x88536:0x887b2;
        ga_stationary(a,e);
        int r=ga_anim_step(a,e,armored_call);
        if (r<0) return GA_UNSUPPORTED;
        if (r) SL(0,next);
        platform(a,e,false);
        ga_attack(a,e,0xe4ebe);
        return ga_cull(a,e,0xe4ece)?GA_DELETE:GA_MORE;
    }
    case 0x88536: case 0x887b2:
        k1b1dc(a);k8917c(a,e);SW(0x94,0xf);
        SL(0,L(0)==0x88536?0x8854c:0x887c8);
        /* fall through */
    case 0x8854c: case 0x887c8: { /* wreck */
        ga_stationary(a,e);
        if (ga_anim_step(a,e,armored_call)<0) return GA_UNSUPPORTED;
        if (!W(0x74)) {
            SW(0x94,W(0x94)-1u);
            if (W(0x94)&0x8000u) {SL(0,0x88598);continue;}
        }
        platform(a,e,false);
        return ga_cull(a,e,0xe4ece)?GA_DELETE:GA_MORE;
    }
    case 0x88598: /* final explosion */
        sound(e,0x102f);
        debris(a,e,0xe5060);
        for (unsigned i=0;i<8;i++) ga_spawn_from(a,e,0x1d31c,0,0x18);  /* 01DBAE */
        if (e->dyn_release) e->dyn_release(e->ctx,a);
        after_script(a,e,0x897e2,0,0);
        return GA_DELETE;                                               /* 004188 */
    case 0x8889c: /* aim */
        SL(0x62,0);SW(0x80,0);SW(0x82,0);
        SL(0,0x888a8);
        /* fall through */
    case 0x888a8:
        SW(0x84,0xa);START(0x28454e);SB(0xaa,0);
        SL(0,0x888c8);
        /* fall through */
    case 0x888c8: {
        ga_register(a,e,NULL,NULL);k146a8(a,e);
        int r=ga_anim_step(a,e,armored_call);
        if (r<0) return GA_UNSUPPORTED;
        if (r) {SW(0x84,W(0x84)-1u);if (!W(0x84)) SL(0,0x8890e);}
        platform(a,e,true);k89152(a);SB(0x5a,0);
        return GA_MORE;
    }
    case 0x8890e: /* fire */
        SL(0x62,0);k146a8(a,e);k88978(a,e);
        START(0x2845f2);SB(0xaa,0);SW(0x80,W(0x80)+1u);
        SL(0,0x8893a);
        /* fall through */
    case 0x8893a: {
        ga_register(a,e,NULL,NULL);k146a8(a,e);
        int r=ga_anim_step(a,e,armored_call);
        if (r<0) return GA_UNSUPPORTED;
        if (r) SL(0,0x88a30);
        platform(a,e,true);k89152(a);SB(0x5a,0);
        return GA_MORE;
    }
    case 0x88a30:
        START(0x2841b4);SB(0xaa,0);
        SL(0,0x88a4a);
        /* fall through */
    case 0x88a4a: { /* between shots */
        ga_register(a,e,NULL,NULL);k146a8(a,e);
        if (ga_anim_step(a,e,armored_call)<0) return GA_UNSUPPORTED;
        SW(0x82,W(0x82)+1u);
        if (W(0x82)>=W(0x98)) {
            SW(0x82,0);
            if (W(0x80)>=W(0x96)) {SW(0x7e,0);SL(0,0x8830c);}
            else {
                SL(0,0x888a8);
                int16_t x=S16(W(0x3c));
                if (x<0x18 || x>0x128) {SW(0x7e,0);SL(0,0x8830c);}
            }
        }
        platform(a,e,true);k89152(a);SB(0x5a,0);
        return GA_MORE;
    }
    case 0x88ad2: /* back away from a player in front */
        SL(0x62,0);k146a8(a,e);k8919c(a,e);
        if (!k893d0(a,e)) {SL(0,0x8806e);continue;}
        START(0x2848aa);SB(0xaa,0);
        SL(0,0x88b02);
        /* fall through */
    case 0x88b02: {
        ga_register(a,e,NULL,NULL);k146a8(a,e);
        int r=ga_anim_step(a,e,armored_call);
        if (r<0) return GA_UNSUPPORTED;
        if (r) {k8919c(a,e);SL(0,k893d0(a,e)?0x88ad2:0x8806e);}
        platform(a,e,true);SB(0x5a,0);
        return GA_MORE;
    }
    default: ga_fail(e,L(0));return GA_UNSUPPORTED;
    }
    ga_fail(e,L(0));return GA_UNSUPPORTED;
}
static const uint32_t labels[]={0x87e36,0x87f2e,0x87f9a,0x8806e,0x88120,0x881e6,0x88232,0x8830c,0x8833a,0x883e8,0x8845c,
    0x88536,0x8854c,0x88598,0x886ec,0x88760,0x887b2,0x887c8,0x8889c,0x888a8,0x888c8,0x8890e,0x8893a,0x88a30,0x88a4a,
    0x88ad2,0x88b02};
bool neo_armored_routine(uint32_t r) {
    for (unsigned i=0;i<sizeof(labels)/sizeof(labels[0]);i++) if (labels[i]==r) return true;
    return false;
}
ga_result_t neo_armored_tick(neo_gactor_t *a,neo_genv_t *e) {return armored_tick(a,e);}
