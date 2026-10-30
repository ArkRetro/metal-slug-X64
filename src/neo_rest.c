#include "neo_region.h"
/* Generic actors of the rest of Mission 1 (camera 1920..3520), manual
 * translation; ROM labels are provenance only.
 *
 * Robed swordsman, spawn-table entry ROM 0A8C46 (record 2D70D8, the figure in
 * the boss arena). Its gesture animation (program 2A73B4, callback 0A9EDA
 * sets +60 bit 0) toggles RAM 10C606 once (+93 = 1, +60 bit 1), which is what
 * the boss waits for in 0D5A92. Words: +6E progress to start, +70 walk
 * distance, +72 exit flag, +74 sub-script index (table ROM 00087A), +7C group
 * whose end lets it leave, +7E/+80 unused on this path.
 *
 * The sub-script it starts at the toggle (script actor 019E72 with the
 * record [ROM 00087A + 4*(+74)], 2D7214 for +74 = 2) is a lone op 3 record:
 * its first update (019F1A -> 01A02C) sets its own +5F bit 7 and the
 * swordsman's +60 bit 7 ("script finished"), then it waits (019E82) and
 * leaves with its creator (00409A). Any other op there stops explicitly.
 *
 * 0A8C62 is the same figure on the floor (0146E8, +66 = 8838) above the
 * covered wagon: its sub-script (record 2D719A, +74 = 1) creates the
 * creature 035668 and, while the wagon (group 7) lives, three swordsmen
 * 039590 (group 6). Once group +7C (6) is empty it leaves (0A8FE0 with +72
 * = 0: 0A935A re-parents to the scene root (01AD52) and walks away 0A9390).
 * Sub-scripts other than a lone op 3 run in the world's interpreter
 * (neo_genv_t.subscript).
 *
 * Not translated (explicit stop): 0A8FE0 with +72 != 0 (the boss-arena
 * figure's fight, not reached in the recordings), its death 0A93DC, and the
 * +93 = 0 branch 0A8EF8. */
#define B(o) ga_b(a,(o))
#define W(o) ga_w(a,(o))
#define L(o) ga_l(a,(o))
#define SB(o,v) ga_sb(a,(o),(uint8_t)(v))
#define SW(o,v) ga_sw(a,(o),(uint16_t)(v))
#define SL(o,v) ga_sl(a,(o),(uint32_t)(v))
#define SET(o,b) SB(o,B(o)|(1u<<(b)))
#define CLR(o,b) SB(o,B(o)&~(1u<<(b)))
#define BIT(o,b) ((B(o)>>(b))&1u)
#define START(t) do { if (!ga_anim_start(a,e,(t),0x100)) return GA_UNSUPPORTED; } while (0)
static int16_t S16(uint16_t v) {return (int16_t)v;}
static bool robe_call(neo_gactor_t *a,neo_genv_t *e,neo_anim_kind_t kind,uint32_t routine,uint16_t parameter) {
    (void)e;(void)parameter;
    if (kind!=NEO_ANIM_CALL) return false;
    switch (routine) {
    case 0xa9eda: SET(0x60,0);return true;
    case 0xa9ee2: CLR(0x60,0);return true;
    case 0xa9eea: SET(0x60,2);return true;
    case 0xa9ef2: CLR(0x60,2);return true;
    default: return false;
    }
}
/* 0A9AF6: carry when +6E is 0 or the camera progress reached it. */
static bool k9af6(neo_gactor_t *a,neo_genv_t *e) {return !W(0x6e) || !(S16(W(0x6e))>S16(e->camera));}
/* 0A9B42: carry when +3C + +70 <= +82. */
static bool k9b42(neo_gactor_t *a) {return !(S16((uint16_t)(W(0x3c)+W(0x70)))>S16(W(0x82)));}
/* 019DCA: live count of group d0 (RAM 10C326..), 0 for d0 >= 9. */
static uint8_t group(neo_genv_t *e,uint16_t g) {return (g&0xffu)<9u && e->groups?e->groups[g&0xfu]:0;}
/* 0A9A56: removal outside the 01945C rectangle 0E64D8 (0x004188). */
static ga_result_t k9a56(neo_gactor_t *a,neo_genv_t *e) {return ga_cull(a,e,0xe64d8)?GA_DELETE:GA_MORE;}
/* The +95 motion choice of every state: 1ABC8 projection or 1436A. */
static void hold(neo_gactor_t *a,neo_genv_t *e) {if (B(0x95)) ga_project(a,e); else ga_stationary(a,e);}
static int step(neo_gactor_t *a,neo_genv_t *e) {return ga_anim_step(a,e,robe_call);}
static void toggle606(neo_genv_t *e) {if (e->flag606) *e->flag606^=0xffu;}
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
static ga_result_t robe(neo_gactor_t *a,neo_genv_t *e) {
    for (;;) switch (L(0)) {
    case 0xa8c46:
        SW(0x66,0x800);SW(0x1e,0x800);SB(0x93,1);SB(0x95,0xff);
        SL(0,0xa8ca0);continue;   /* bra 0A8CA0 */
    case 0xa8c62:
        ga_floor(a,e);SW(0x66,0x8838);SW(0x1e,0x8838);SB(0x93,1);SB(0x95,0xff);
        SL(0,0xa8ca0);continue;   /* bra 0A8CA0 */
    case 0xa8ca0: {
        SB(0x1d,0);SL(0x50,0);                       /* 009F88, no hurtbox */
        ga_palette(a,0x1b);ga_palette(a,0x20b);ga_palette(a,0x3ba);
        SB(0x14,B(0x19));
        static const uint32_t tables[3]={0x2d2fc0,0x2d2fda,0x2d2ff4};
        static const uint8_t field[3]={0x4e,0x86,0x84};
        for (unsigned i=0;i<3;i++) {
            const uint8_t *t=ga_rom(tables[i]);
            if (!t) {ga_fail(e,tables[i]);return GA_UNSUPPORTED;}
            SW(field[i],ga_rank_word(a,e,t));
        }
        SB(0x61,0);SB(0x60,0);SB(0x94,0);
        SL(0,0xa8d22);return GA_MORE;
    }
    case 0xa8d22: START(0x2a7372);SL(0,0xa8d38);continue;
    case 0xa8d38:
        hold(a,e);
        if (step(a,e)<0) return GA_UNSUPPORTED;
        if (k9af6(a,e)) SL(0,0xa8d6a);
        return k9a56(a,e);
    case 0xa8d6a:
        SL(0x50,0);
        if (!W(0x70)) {SL(0,0xa8dec);continue;}
        SB(0x15,B(0x15)^1u);START(0x2a759c);
        SW(0x82,W(0x3c));
        SL(0x62,0u-((uint32_t)W(0x86)<<8));
        SL(0,0xa8daa);continue;
    case 0xa8daa:
        if (B(0x95)) {SL(0x30,L(0x62));SL(0x34,0);ga_move(a,e);}
        else {uint32_t kind;ga_ground(a,e,&kind);}
        if (step(a,e)<0) return GA_UNSUPPORTED;
        if (k9b42(a)) {SB(0x15,B(0x15)^1u);SL(0,0xa8dec);}
        return k9a56(a,e);
    case 0xa8dec: SW(0x88,0x0f);SL(0,0xa8dfc);continue;
    case 0xa8df6: SW(0x88,0x32);SL(0,0xa8dfc);continue;
    case 0xa8dfc: SL(0x50,0);START(0x2a7372);SL(0,0xa8e1a);continue;
    case 0xa8e1a:
        hold(a,e);
        if (step(a,e)<0) return GA_UNSUPPORTED;
        SW(0x88,W(0x88)-1u);
        if (!W(0x88)) SL(0,0xa8e8e);
        if (!B(0x93)) {if (!group(e,W(0x7c))) SL(0,0xa8fe0);}
        else if (BIT(0x60,7) && !group(e,W(0x7c))) SL(0,0xa8fe0);
        return k9a56(a,e);
    case 0xa8e8e: {
        SL(0x50,0);START(0x2a73b4);
        /* 01AF9A: the nearest live player (one player here: player 1). */
        uint16_t px=e->players.p1x;
        if (S16(W(0x3c))<S16(px)) SET(0x15,0); else CLR(0x15,0);
        SL(0,0xa8ece);continue;
    }
    case 0xa8ece: {
        hold(a,e);
        if (BIT(0x60,0)) {
            if (!B(0x93)) {toggle606(e);CLR(0x60,0);}
            else if (!BIT(0x60,1)) {
                toggle606(e);SET(0x60,1);
                /* 0A8F1E: sub-script 019E72 after this actor (0040BC),
                 * record [00087A + 4*(+74)], +2C word = 0x100. */
                if (W(0x74)!=0xffffu) {
                    const uint8_t *t=ga_rom(0x87au+4u*W(0x74));
                    if (!t) {ga_fail(e,0x87au+4u*W(0x74));return GA_UNSUPPORTED;}
                    neo_gactor_t *s=e->spawn?e->spawn(e->ctx,a,0x19e72,false):NULL;
                    if (s) {ga_sl(s,0x6e,(uint32_t)((uint32_t)t[0]<<24|t[1]<<16|t[2]<<8|t[3]));ga_sw(s,0x2c,0x100);}
                }
            }
        }
        int r=step(a,e);
        if (r<0) return GA_UNSUPPORTED;
        if (r>0) {
            if (!B(0x93)) SL(0,group(e,W(0x7c))?0xa8df6:0xa8fe0);
            else if (!BIT(0x60,7)) SL(0,0xa8df6);
            else SL(0,group(e,W(0x7c))?0xa8df6:0xa8fe0);
        }
        return k9a56(a,e);
    }
    case 0xa8fe0:
        SL(0x50,0);
        if (W(0x72)) {ga_fail(e,0xa8ff0);return GA_UNSUPPORTED;}   /* fight path: not reached */
        SL(0,0xa935a);continue;
    case 0xa935a:
        SL(0x50,0);START(0x2a759c);to_root(a,e);
        SL(0x62,(uint32_t)W(0x86)<<8);CLR(0x15,0);
        SL(0,0xa9390);continue;
    case 0xa9390:
        if (!B(0x95)) {uint32_t kind;ga_ground(a,e,&kind);}
        else {SL(0x30,L(0x62));SL(0x34,0);ga_move(a,e);}
        if (step(a,e)<0) return GA_UNSUPPORTED;
        if (B(0x5f)&0x80u) SL(0,0xa93dc);
        return k9a56(a,e);
    default: /* 0A93DC death and the rest: not translated */
        ga_fail(e,L(0));return GA_UNSUPPORTED;
    }
}
/* Script actor 019E72/019E82 running a record that is a lone op 3 (the
 * swordsman's sub-script): 01A02C on every update while its creator lives. */
static ga_result_t subscript(neo_gactor_t *a,neo_genv_t *e) {
    if (L(0)==0x19e72) {SW(0x72,0);CLR(0x5f,7);SL(0,0x19e82);}
    if (!ga_creator_alive(a,e)) return GA_DELETE;          /* 00409A, 004188 */
    if (!L(0x6e) || BIT(0x5f,7)) return GA_MORE;
    const uint8_t *r=ga_rom(L(0x6e));
    if (!r) {ga_fail(e,L(0x6e));return GA_UNSUPPORTED;}
    if (r[0]!=3u) {
        /* Other records: the level-script interpreter of the world. */
        if (!e->subscript) {ga_fail(e,L(0x6e));return GA_UNSUPPORTED;}
        if (!e->subscript(e->ctx,a,L(0x6e))) return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    }
    SET(0x5f,7);
    neo_gactor_t *c=e->creator?e->creator(e->ctx,a):NULL;
    if (c) ga_sb(c,0x60,(uint8_t)(ga_b(c,0x60)|0x80u));
    return GA_MORE;
}
bool neo_rest_routine(uint32_t r) {
    if (r==0x19e72 || r==0x19e82) return true;
    return r==0xa8c46 || r==0xa8c62 || r==0xa8ca0 || r==0xa8d22 || r==0xa8d38 || r==0xa8d6a || r==0xa8daa || r==0xa8dec ||
        r==0xa8df6 || r==0xa8dfc || r==0xa8e1a || r==0xa8e8e || r==0xa8ece || r==0xa8fe0 || r==0xa935a || r==0xa9390;
}
ga_result_t neo_rest_tick(neo_gactor_t *a,neo_genv_t *e) {
    if (L(0)==0x19e72 || L(0)==0x19e82) return subscript(a,e);
    return robe(a,e);
}
