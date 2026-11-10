#include "neo_region.h"
/* Mission 5 sewer gates, manual translation (ROM labels as provenance; MAME
 * run4 replay from sec08_274387, docs/MISION5.md "Verjas de la
 * alcantarilla").
 * - 0BC296 (script, camera 0): at (120, 30) of the terrain 10C72E (01AB98),
 *   +4E = rank 2D3F1E (0260C8), hurtbox 0BBAD4, +7A = 0, palette 59C,
 *   flash 59D, layers 0, program 2A91D0, the wall 0BBB28 stamped by 0C88B0.
 *   0BC30A: 004C24 (carry: +5A = 0); a hit (+5F bit 7): +7A + 1, below 3 the
 *   rank again, bit 7 cleared and the sound 1027; 01ABC8, 01AD8C, 00427A.
 *   At the third hit 0BC364: 0BBBE4 by 0C88B0 (the wall's cells to 0), the
 *   debris list 0BBD78 (01DA40), sound 102D, 019DFC, 004188.
 * - 0BC3A6 (script, +6E = 0 / 1 / 2): at (320 / 520 / 720, 30) of 10C72E,
 *   rank 2D3F80, hurtbox 0BBAC8, palette 59F, flash 59E, layers 1001,
 *   program 2A91EE. 0BC42C as 0BC30A with five hits, and every update
 *   10C700 = +3C (0BC542: the limit of the sewer soldiers' 08A654). At the
 *   fifth 0BC48E: 0BBBE4 by 0BC38E -> 0C8A3C (the six columns of 1F cells
 *   of the ROM map at the gate to 0), the debris 0BBD8A, sound 102D,
 *   019DFC, 10C700 = 200 (0BC550), 004188. */
#define L(o) ga_l(a,o)
#define W(o) ga_w(a,o)
#define B(o) ga_b(a,o)
#define SL(o,v) ga_sl(a,o,v)
#define SW(o,v) ga_sw(a,o,(uint16_t)(v))
#define SB(o,v) ga_sb(a,o,(uint8_t)(v))
static bool rom_ok(neo_genv_t *e,uint32_t at) {if (ga_rom(at)) return true;ga_fail(e,at);return false;}
/* 01DA40: the emitter 01D764 of the debris list (0040BC with D0 = 1C, 01B220
 * at (0, 0), +15 bit 2 from the caller, +86 = the list). */
static void debris(neo_gactor_t *a,neo_genv_t *e,uint32_t list) {
    neo_gactor_t *c=e->spawn_reserve?e->spawn_reserve(e->ctx,a,0x1d764,0x1c):e->spawn?e->spawn(e->ctx,a,0x1d764,false):NULL;
    if (!c) return;
    ga_place_from(c,a,e,0,0);
    if (B(0x15)&4u) ga_sb(c,0x15,(uint8_t)(ga_b(c,0x15)|4u));
    ga_sl(c,0x86,list);
}
/* 01ABC8, 01AD8C, 00427A */
static int gate_step(neo_gactor_t *a,neo_genv_t *e) {
    ga_project(a,e);
    if ((W(0x66)&0xf000u)==0x8000u) SW(0x1e,W(0x66)-W(0x40));
    return ga_anim_step(a,e,NULL);
}
static ga_result_t gate_tick(neo_gactor_t *a,neo_genv_t *e) {
    bool big=L(0)>=0xbc3a6u;
    uint32_t rank=big?0x2d3f80u:0x2d3f1eu;
    switch (L(0)) {
    case 0xbc296:
        ga_place(a,e,0x120,0x30,0x10c72eu);
        if (!rom_ok(e,rank)) return GA_UNSUPPORTED;
        SW(0x4e,ga_rank_word(a,e,ga_rom(rank)));SL(0x50,0xbbad4);SB(0x7a,0);
        ga_palette(a,0x59c);ga_flash_palette(a,0x59d);SW(0x66,0);SW(0x1e,0);
        if (!ga_anim_start(a,e,0x2a91d0,0x100)) return GA_UNSUPPORTED;
        if (e->stamp) e->stamp(e->ctx,a,0xbbb28);                        /* 0C88B0 */
        SL(0,0xbc30a);
        goto update;
    case 0xbc3a6:
        {   uint16_t x=0x320;
            if (W(0x6e)==1u) x=0x520;
            if (W(0x6e)==2u) x=0x720;
            ga_place(a,e,(int16_t)x,0x30,0x10c72eu);}
        if (!rom_ok(e,rank)) return GA_UNSUPPORTED;
        SW(0x4e,ga_rank_word(a,e,ga_rom(rank)));SL(0x50,0xbbac8);SB(0x7a,0);
        ga_palette(a,0x59f);ga_flash_palette(a,0x59e);SW(0x66,0x1001);SW(0x1e,0x1001);
        if (!ga_anim_start(a,e,0x2a91ee,0x100)) return GA_UNSUPPORTED;
        SL(0,0xbc42c);
        goto update;
    case 0xbc30a:case 0xbc42c:
    update:
        if (ga_register(a,e,NULL,NULL)) SB(0x5a,0);                       /* 004C24 */
        if (B(0x5f)&0x80u) {
            SB(0x7a,B(0x7a)+1u);
            if (B(0x7a)>=(big?5u:3u)) {
                if (big) {if (e->stamp_cells) e->stamp_cells(e->ctx,a,0xbbbe4);}   /* 0BC38E -> 0C8A3C */
                else if (e->stamp) e->stamp(e->ctx,a,0xbbbe4);                     /* 0C88B0 */
                debris(a,e,big?0xbbd8au:0xbbd78u);                                /* 01DA40 */
                if (e->sound) e->sound(e->ctx,0x102d);
                SB(0x69,0xff);                                                    /* 019DFC */
                if (big && e->flags) e->flags->limit700=0x200;                    /* 0BC550 */
                return e->unsupported?GA_UNSUPPORTED:GA_DELETE;                   /* 004188 */
            }
            SW(0x4e,ga_rank_word(a,e,ga_rom(rank)));SB(0x5f,B(0x5f)&0x7fu);
            if (e->sound) e->sound(e->ctx,0x1027);
        }
        if (gate_step(a,e)<0) return GA_UNSUPPORTED;
        if (big && e->flags) e->flags->limit700=W(0x3c);                          /* 0BC542 */
        break;
    default:
        ga_fail(e,L(0));return GA_UNSUPPORTED;
    }
    return e->unsupported?GA_UNSUPPORTED:GA_MORE;
}
/* ---- the sewer drips 08346E (script and runner records; 717 drops in run4) -------
 * 08346E: 083630 (+70 = +6E, +72 = +70, +6E = 0: the record's words are the
 * period and the flag); with +72: 08364C (+30 = 0, +34 = 100 px up, 0143E4)
 * and, unless it met something (+5F bit 1), +28/+2C/+38 back and 01ABC8.
 * +6E = 0; 0834B0: 01ABC8; +6E - 1 > 0 waits, else the drop 0834F4 after
 * the script actor (00411C, D0 = 2A) at its position (01ACF2) and the next
 * wait 083674: rand <= 2666: +70 * 3/2; <= 4CCC: +70 * 5/4; <= A665: +70;
 * <= CCCC: +70 / 2; else +70 / 8. Outside E4C48 (01945C): 004188.
 * - 0834F4: +28 += (rand & 7) - 3, 01ABC8, layers 1, palette AC8, program
 *   281E98, 01B1DC; 083540: 0143DC, 00427A; on a floor (+15 bit 6) ->
 *   083568; deleted when +40 < 100. 083568: program 281EA2, 01AD52 (to
 *   the root), 01B1DC; 08358A: 01ABC8, 00427A, deleted at its end. */
static void to_root(neo_gactor_t *a,neo_genv_t *e) {                    /* 01AD52 */
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
static void k1b1dc(neo_gactor_t *a) {SL(0x30,0);SL(0x34,0);SL(0x62,0);}
static ga_result_t drip_tick(neo_gactor_t *a,neo_genv_t *e) {
    int r;
    switch (L(0)) {
    case 0x8346e: {
        uint16_t d0=W(0x6e),d1=W(0x70);SW(0x6e,0);SW(0x70,0);SW(0x70,d0);SW(0x72,d1);   /* 083630 */
        if (W(0x72)) {
            uint32_t x=L(0x28),y=L(0x2c),p=L(0x38);
            SL(0x30,0);SL(0x34,0x1000000);ga_air(a,e);                     /* 08364C */
            if (!(B(0x5f)&2u)) {SL(0x28,x);SL(0x2c,y);SL(0x38,p);ga_project(a,e);}
        }
        SW(0x6e,0);SL(0,0x834b0);}
        /* fall through */
    case 0x834b0:
        ga_project(a,e);
        SW(0x6e,W(0x6e)-1u);
        if ((int16_t)W(0x6e)<=0) {
            neo_gactor_t *c=e->spawn?e->spawn(e->ctx,NULL,0x834f4,true):NULL;   /* 00411C, D0 = 2A */
            if (c) ga_place_at(c,a,e,0,0);                                 /* 01ACF2 */
            uint16_t q=ga_rand(e),v=W(0x70);                               /* 083674 */
            if (q<=0x2666u) v=(uint16_t)(((int16_t)v>>1)+v);
            else if (q<=0x4cccu) v=(uint16_t)(((int16_t)v>>2)+v);
            else if (q<=0xa665u) {}
            else if (q<=0xccccu) v=(uint16_t)((int16_t)v>>1);
            else v=(uint16_t)((int16_t)v>>3);
            SW(0x6e,v);
        }
        if (e->unsupported) return GA_UNSUPPORTED;
        return ga_cull(a,e,0xe4c48)?GA_DELETE:GA_MORE;
    case 0x834f4:
        SW(0x28,W(0x28)+(uint16_t)((ga_rand(e)&7u)-3u));ga_project(a,e);
        SW(0x66,1);SW(0x1e,1);ga_palette(a,0xac8);
        if (!ga_anim_start(a,e,0x281e98,0x100)) return GA_UNSUPPORTED;
        k1b1dc(a);SL(0,0x83540);
        /* fall through */
    case 0x83540:
        SL(0x34,L(0x34)+0xffffb000u);ga_air(a,e);                          /* 0143DC */
        if (ga_anim_step(a,e,NULL)<0) return GA_UNSUPPORTED;
        if (B(0x15)&0x40u) SL(0,0x83568);
        if (e->unsupported) return GA_UNSUPPORTED;
        return (int16_t)W(0x40)<0x100?GA_DELETE:GA_MORE;
    case 0x83568:
        if (!ga_anim_start(a,e,0x281ea2,0x100)) return GA_UNSUPPORTED;
        to_root(a,e);k1b1dc(a);SL(0,0x8358a);
        /* fall through */
    case 0x8358a:
        ga_project(a,e);
        if ((r=ga_anim_step(a,e,NULL))<0 || e->unsupported) return GA_UNSUPPORTED;
        return r?GA_DELETE:GA_MORE;
    default:
        ga_fail(e,L(0));return GA_UNSUPPORTED;
    }
}
static bool drip_routine(uint32_t r) {return r==0x8346eu || r==0x834b0u || r==0x834f4u || r==0x83540u || r==0x83568u || r==0x8358au;}
bool neo_m5props_routine(uint32_t r) {return drip_routine(r) || r==0xbc296u || r==0xbc30au || r==0xbc3a6u || r==0xbc42cu;}
ga_result_t neo_m5props_tick(neo_gactor_t *a,neo_genv_t *e) {return drip_routine(L(0))?drip_tick(a,e):gate_tick(a,e);}
