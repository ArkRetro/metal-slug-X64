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
/* ---- decorations of the street and the sewer, and the street's end ----------------
 * - 0BBE5E (street, camera 0): palette 5BB, layers 0, program 2A8FB6 (+6E =
 *   0) or 2A8FC0, +78 = FFD0. 0BBEAA -> 0BBEB0: 01ABC8, 00427A; deleted
 *   once +3C < +78.
 * - 0BC4F4 (sewer): 012022, palette 0BBE00[+6E], layers FFF1, program
 *   2A921E, +7A = +6E, +78 = FEF0, then 0BBEAA. 0BC4BA: 012022, palette
 *   591, layers FFF1, program 2A921C, +78 = FFC0, then 0BBEAA.
 * - 0BBED0 (street, camera 1584): palette 5DC, layers FFF1, program 2A918A;
 *   0BBF00: 01ABC8, 00427A.
 * - 0BBE16 (street, camera 1640): +7B = 3C; 0BBE22: 01ABC8 and, at the end
 *   of the count, the stage exit 0B6ED2 at (7A0, -28) of the root with the
 *   rectangle 0BBAEC and the walk target 7A8 (neo_goarrow.c).
 * - 032B46 / 032B5A / 032B6E (sewer, camera 0): programs 2393DE / 2393EA /
 *   2393F6, 019DFC, layers 0, palette 158 with the soft-DIP BLOOD on (RAM
 *   10FD8F) or 17D; 032BC4: 01ABC8, 00427A, outside 032B3E (01945C): 004188.
 * - 0BC1E6 (metro, +6E = a group, 8 from 9 on): 012022, palette 58F, layers
 *   0, program 2A91C6. 0BC22C: while the group +6E has nobody (019DCA);
 *   else program 2A9194 (its sound 107D) and 0BC252 until the group is
 *   empty again (program 2A91C6); 01ABC8, 00427A, deleted once +3C < -30. */
static void front(neo_gactor_t *a,neo_genv_t *e) {                       /* 012022 */
    if (!e->front_layer) return;
    if (e->front_layer(e->ctx)) SB(0x15,B(0x15)|4u); else SB(0x15,B(0x15)&0xfbu);
}
/* 019DCA: the live count of the group +6E (RAM 10C326..; 0 from 9 on). */
static uint8_t lamp_group(neo_gactor_t *a,neo_genv_t *e) {
    uint8_t d0=(uint8_t)W(0x6e);
    return d0>=9u || !e->groups?0:e->groups[d0&0xfu];
}
static ga_result_t deco_tick(neo_gactor_t *a,neo_genv_t *e) {
    switch (L(0)) {
    case 0xbbe5e:
        ga_palette(a,0x5bb);SW(0x66,0);SW(0x1e,0);
        if (!ga_anim_start(a,e,W(0x6e)?0x2a8fc0:0x2a8fb6,0x100)) return GA_UNSUPPORTED;
        SW(0x78,0xffd0);SL(0,0xbbeb0);
        goto run;
    case 0xbc4f4: {
        front(a,e);
        const uint8_t *t=ga_rom(0xbbe00u+2u*W(0x6e));
        if (!t) {ga_fail(e,0xbbe00);return GA_UNSUPPORTED;}
        ga_palette(a,(uint16_t)(t[0]<<8|t[1]));SW(0x66,0xfff1);SW(0x1e,0xfff1);
        if (!ga_anim_start(a,e,0x2a921e,0x100)) return GA_UNSUPPORTED;
        SB(0x7a,W(0x6e));SW(0x78,0xfef0);SL(0,0xbbeb0);
        goto run;}
    case 0xbc4ba:
        front(a,e);ga_palette(a,0x591);SW(0x66,0xfff1);SW(0x1e,0xfff1);
        if (!ga_anim_start(a,e,0x2a921c,0x100)) return GA_UNSUPPORTED;
        SW(0x78,0xffc0);SL(0,0xbbeb0);
        /* fall through */
    case 0xbbeb0:
    run:
        ga_project(a,e);
        if (ga_anim_step(a,e,NULL)<0 || e->unsupported) return GA_UNSUPPORTED;
        return (int16_t)W(0x3c)<(int16_t)W(0x78)?GA_DELETE:GA_MORE;
    case 0xbbed0:
        ga_palette(a,0x5dc);SW(0x66,0xfff1);SW(0x1e,0xfff1);
        if (!ga_anim_start(a,e,0x2a918a,0x100)) return GA_UNSUPPORTED;
        SL(0,0xbbf00);
        /* fall through */
    case 0xbbf00:
        ga_project(a,e);
        if (ga_anim_step(a,e,NULL)<0 || e->unsupported) return GA_UNSUPPORTED;
        return GA_MORE;
    case 0x32b46:case 0x32b5a:case 0x32b6e:
        if (!ga_anim_start(a,e,L(0)==0x32b46u?0x2393deu:L(0)==0x32b5au?0x2393eau:0x2393f6u,0x100)) return GA_UNSUPPORTED;
        SB(0x69,0xff);                                                   /* 019DFC */
        SW(0x66,0);SW(0x1e,0);
        ga_palette(a,e->blood?0x158:0x17d);                              /* 032B94: RAM 10FD8F */
        SL(0,0x32bc4);
        /* fall through */
    case 0x32bc4:
        ga_project(a,e);
        if (ga_anim_step(a,e,NULL)<0 || e->unsupported) return GA_UNSUPPORTED;
        return ga_cull(a,e,0x32b3e)?GA_DELETE:GA_MORE;
    case 0xbc1e6:
        front(a,e);ga_palette(a,0x58f);SW(0x66,0);SW(0x1e,0);
        if (!ga_anim_start(a,e,0x2a91c6,0x100)) return GA_UNSUPPORTED;
        if (W(0x6e)>=9u) SW(0x6e,8);
        SL(0,0xbc22c);
        /* fall through */
    case 0xbc22c:
        if (!lamp_group(a,e)) goto lamp;
        if (!ga_anim_start(a,e,0x2a9194,0x100)) return GA_UNSUPPORTED;
        SL(0,0xbc252);
        /* fall through */
    case 0xbc252:
        if (!lamp_group(a,e)) {
            if (!ga_anim_start(a,e,0x2a91c6,0x100)) return GA_UNSUPPORTED;
            SL(0,0xbc22c);
        }
    lamp:
        ga_project(a,e);
        if (ga_anim_step(a,e,NULL)<0 || e->unsupported) return GA_UNSUPPORTED;
        return (int16_t)W(0x3c)<-0x30?GA_DELETE:GA_MORE;
    case 0xbbe16:
        SB(0x7b,0x3c);SL(0,0xbbe22);
        /* fall through */
    case 0xbbe22:
        ga_project(a,e);
        SB(0x7b,B(0x7b)-1u);
        if (B(0x7b)) return e->unsupported?GA_UNSUPPORTED:GA_MORE;
        SL(0x38,0x10e968u);SL(0x28,0x7a00000u);SL(0x2c,0xffd80000u);SL(0x6e,0xbbaec);SW(0x72,0x7a8);
        SL(0,0xb6ed2);return neo_goarrow_tick(a,e);                    /* jmp 0B6ED2 */
    default:
        ga_fail(e,L(0));return GA_UNSUPPORTED;
    }
}
static bool deco_routine(uint32_t r) {return r==0xbbe5eu || r==0xbc4f4u || r==0xbc4bau || r==0xbbeb0u || r==0xbbed0u || r==0xbbf00u || r==0xbbe16u || r==0xbbe22u || r==0xbc1e6u || r==0xbc22cu || r==0xbc252u || r==0x32b46u || r==0x32b5au || r==0x32b6eu || r==0x32bc4u;}
/* ---- the street's ledges 09624E and wrecked cars 050542 -----------------------------
 * - 09624E (script, +6E = 0..3, never drawn): +72 = +6E, +7A = +70, program
 *   0963B8, private palette 124, layers 0. 09628E: 01ABC8, the cell map
 *   095B54[+72] at (096356[+72], -8) (0140C4); outside 095948 (0C8BEC):
 *   004188. With the debug flag 10F000 bit 7 it draws itself (09635E: 009CCE
 *   and 00427A in a row): an explicit stop here.
 * - 050542 (script, +6E = 0..5; +70 items of kind +72 with the word +74):
 *   palette 0500FC[+6E], program 04FD26[+6E], hurtbox 050150[+6E], 0146E8,
 *   +4E = 64, +7C = 0, layers 8600 (0501D4[+6E] -> 0501F8). 0505AC: 050206
 *   (+7D / +7E: where each player stands on it, 050296: 01AEB0, 01AF34, up
 *   to 0501EC[+6E] above, the six bounds 0501A4[+6E][dy >= 2C] against the
 *   long x difference; +7C follows their mean one step per update, 5 at
 *   once from 0 when it is over 3; back at 0: program 04FD3E[+6E]); 01ABC8;
 *   004C24: a hit of kind 5 kills, another one starts 04FD56[+6E] (+5B
 *   negative) or 04FD6E[+6E]; 00427A, at its end 04FD26[+6E] again; outside
 *   050108[+6E] (01945C): 004188; the cell map 050120[+6E] lowered by +7C
 *   (050418, 0140C4).
 *   Dead (+5F bit 7 or the kind 5), 05065C: 019DFC, four wheels 05090A /
 *   05092C (the second pair mirrored), the blast 05094E, program
 *   04FD86[+6E] (its calls: 050C32 the panels 0509A8; for +6E >= 4 also
 *   013F1E, the debris list 0500AA by 01DA40 and 050384, the items 08CD58
 *   thrown to both sides). 050806: 01ABC8, the map while +6E < 4, 00427A.
 *   05082C (+6E < 4): +15 bit 5, program 04FD9E[+6E], layers 1002, +34 = F
 *   px, y + 8; 050876: 0143E4, rising: the map 050138[+6E] and -1.25 px,
 *   falling: 013F1E and -0.3125 px; landed: program 04FDB6[+6E] (calls
 *   050D12: two pieces 050B52; 05031E: the items 08CDB2 at the angle (rand
 *   & 1F) + 30 with the speed ((rand & FF) + 300) << 8), 0508DC: 01ABC8,
 *   00427A; at the end (and at once for +6E >= 4) 0508EE: 013F1E, sound
 *   1021, 009F88, jmp 01C71E.
 * - 05090A / 05092C (palette 37C, programs 22E6AE / 247DA8) and 05094E
 *   (palette 113, program 24855C, 8 px left when the car's +6E >= 3):
 *   layers 1001; 050990: 01ABC8, 00427A, deleted at the end.
 * - 0509A8 (+7F from the car): +15 bit 5, the car's +6E, palette
 *   0500FC[+6E], layers 1002 with +7F, program 04FDCE[+6E] or 04FDE6[+6E],
 *   +30 = +-C000 (twice with +7F), +34 = 5 px + (rand * C00 >> 16) << 4,
 *   +7A = 3C. 050A66: deleted when +7A runs out; +76 = +34; the first four
 *   updates 01AC38 with the gravity, then 0143DC; landed: +34 = |+76| * 3 /
 *   4, +5F bit 3; 00427A.
 * - 050B52: as 0509A8 with program 04FDFE[+6E], +30 = +-1 px, +34 = 2.25
 *   px + the same random, +7A = 1E; 050BD0: 0143DC, landed also program
 *   04FE16[+6E]. */
static uint16_t rw(uint32_t at) {const uint8_t *p=ga_rom(at);return p?(uint16_t)(p[0]<<8|p[1]):0;}
static uint32_t rl(uint32_t at) {return (uint32_t)rw(at)<<16|rw(at+2u);}
static ga_result_t ledge_tick(neo_gactor_t *a,neo_genv_t *e) {
    switch (L(0)) {
    case 0x9624e: {
        uint16_t d0=W(0x6e),d1=W(0x70);
        SW(0x72,d0);SW(0x7a,d1);
        if (!ga_anim_start(a,e,0x963b8,0x100)) return GA_UNSUPPORTED;
        ga_palette(a,0x124);SW(0x66,0);SW(0x1e,0);SL(0,0x9628e);}
        /* fall through */
    case 0x9628e: {
        ga_project(a,e);
        unsigned n=W(0x72);
        if (n>3u || !rom_ok(e,0x96356u+n) || !rom_ok(e,0x95b54u+4u*n)) return GA_UNSUPPORTED;
        if (e->dyn_set) e->dyn_set(e->ctx,a,rl(0x95b54u+4u*n),(int8_t)ga_rom(0x96356u+n)[0],-8);   /* 0140C4 */
        if (ga_outside_view(a,e,0x95948)) return e->unsupported?GA_UNSUPPORTED:GA_DELETE;
        if (e->vehicle && (e->vehicle->dip10f000&0x80u)) {ga_fail(e,0x9635e);return GA_UNSUPPORTED;}   /* the debug flag, where the world gives it */
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;}
    default:
        ga_fail(e,L(0));return GA_UNSUPPORTED;
    }
}
static bool car_start(neo_gactor_t *a,neo_genv_t *e,uint32_t table) {
    uint32_t at=table+4u*W(0x6e);
    return rom_ok(e,at) && ga_anim_start(a,e,rl(at),0x100);
}
/* 050296 */
static uint8_t car_weight(neo_gactor_t *a,neo_genv_t *e,unsigned p) {
    unsigned n=W(0x6e);
    if (!((e->players.active_mask>>p)&1u)) return 0;                     /* 01AEB0 */
    if (!(e->player_flags15[p]&0x40u)) return 0;                         /* 01AF34 */
    int16_t d1=(int16_t)((p?e->players.p2y:e->players.p1y)-W(0x40));
    if (d1<0) return 0;
    if ((uint16_t)d1>rw(0x501ecu+2u*n)) return 0;
    uint32_t px=(uint32_t)(p?e->players.p2x:e->players.p1x)<<16|e->player_low[p];
    int16_t d0=(int16_t)((px-L(0x3c))>>16);
    uint32_t t=rl(0x501a4u+8u*n+(d1>=0x2c?4u:0u));
    for (unsigned i=0;i<6;i++) if (d0<(int16_t)rw(t+2u*i)) return (uint8_t)i;
    return 0;
}
/* 050418 */
static void car_map(neo_gactor_t *a,neo_genv_t *e) {
    unsigned n=W(0x6e);int16_t d1=0;uint8_t s=B(0x7c);
    if (s==1u) d1=-2;
    if (s==2u && n==5u) d1=-1;
    if (s==3u) d1=-2;
    if (s==5u) d1=-2;
    if (e->dyn_set) e->dyn_set(e->ctx,a,rl(0x50120u+4u*n),0,d1);          /* 0140C4 */
}
static neo_gactor_t *car_child(neo_gactor_t *a,neo_genv_t *e,uint32_t routine,int16_t dx,int16_t dy) {   /* 0040BC (D0 = 1C), 01B220 */
    neo_gactor_t *c=e->spawn_reserve?e->spawn_reserve(e->ctx,a,routine,0x1c):e->spawn?e->spawn(e->ctx,a,routine,false):NULL;
    if (c) ga_place_from(c,a,e,dx,dy);
    return c;
}
/* 08CDB2: the item 08C374 after the script actor (a kind from 5A on: none). */
static neo_gactor_t *car_item(neo_gactor_t *a,neo_genv_t *e,int16_t dx,int16_t dy,uint16_t kind,uint16_t word,uint8_t b8b) {
    if (kind>=0x5au) return NULL;
    neo_gactor_t *c=e->spawn?e->spawn(e->ctx,NULL,0x8c374,true):NULL;
    if (c) {ga_sb(c,0x8b,b8b);ga_sw(c,0x70,word);ga_sw(c,0x6e,kind);ga_sw(c,0x72,0);ga_sb(c,0x7f,0);ga_place_at(c,a,e,dx,dy);}
    return c;
}
static bool car_call(neo_gactor_t *a,neo_genv_t *e,neo_anim_kind_t kind,uint32_t routine,uint16_t parameter) {
    (void)parameter;
    if (kind!=NEO_ANIM_CALL) return false;
    unsigned n=W(0x6e);neo_gactor_t *c;
    switch (routine) {
    case 0x13f1e:
        if (e->dyn_release) e->dyn_release(e->ctx,a);
        return true;
    case 0x50c32:
        c=car_child(a,e,0x509a8,0x10,8);
        if (c) ga_sb(c,0x7f,0);
        if (n>=4u) return true;
        c=car_child(a,e,0x509a8,-0x10,8);
        if (c) {ga_sb(c,0x7f,0);ga_sb(c,0x15,(uint8_t)(ga_b(c,0x15)|1u));}
        if (n!=1u && n!=2u) return true;
        c=car_child(a,e,0x509a8,0x10,8);
        if (c) ga_sb(c,0x7f,1);
        c=car_child(a,e,0x509a8,-0x10,8);
        if (c) {ga_sb(c,0x7f,1);ga_sb(c,0x15,(uint8_t)(ga_b(c,0x15)|1u));}
        return true;
    case 0x50d12:
        car_child(a,e,0x50b52,0x10,0);
        c=car_child(a,e,0x50b52,-0x10,0);
        if (c) ga_sb(c,0x15,(uint8_t)(ga_b(c,0x15)|1u));
        return true;
    case 0x50d66:
        debris(a,e,0x500aa);                                             /* 01DA40 */
        return true;
    case 0x5031e:
        while (W(0x70)) {
            int16_t dx=(int16_t)(ga_rand_range(e,0x31)-0x18u);
            c=car_item(a,e,dx,8,W(0x72),W(0x74),0);
            uint8_t angle=(uint8_t)((ga_rand(e)&0x1fu)+0x30u);
            uint32_t speed=(uint32_t)(int32_t)(int16_t)((ga_rand(e)&0xffu)+0x300u)<<8,vx,vy;
            ga_polar(speed,angle,&vx,&vy);                               /* 002E7C */
            if (c) {ga_sl(c,0x30,vx);ga_sl(c,0x34,vy);}
            SW(0x70,W(0x70)-1u);
        }
        return true;
    case 0x50384:
        for (uint16_t d7=W(0x70);d7;d7--) {
            int16_t dx=(int16_t)(ga_rand_range(e,0x31)-0x18u);
            uint16_t d5=(ga_rand(e)&0x8000u)?4u:8u;
            c=car_item(a,e,dx,8,W(0x72),W(0x74),0);                      /* 08CD58 */
            if (c) ga_sw(c,0x72,d5);
            uint32_t vy=((uint32_t)(int32_t)(int16_t)ga_rand_range(e,0x400)<<8)+0x50000u;
            if (c) ga_sl(c,0x34,vy);
            uint32_t vx=(uint32_t)(int32_t)(int16_t)ga_rand_range(e,0x200)<<8;
            if (d5==8u) vx=0u-vx;
            if (c) ga_sl(c,0x30,vx);
        }
        return true;
    }
    return false;
}
static void car_bounce(neo_gactor_t *a) {
    uint32_t v=L(0x76);
    if (v&0x80000000u) v=0u-v;
    SL(0x34,(uint32_t)((int32_t)(v*3u)>>2));SB(0x5f,B(0x5f)|8u);
}
static ga_result_t car_tick(neo_gactor_t *a,neo_genv_t *e) {
    unsigned n=W(0x6e);int r;neo_gactor_t *c;
    switch (L(0)) {
    case 0x50542:
        if (n>5u || !rom_ok(e,0x500fcu+2u*n) || !rom_ok(e,0x50150u+4u*n) || !rom_ok(e,0x501a4)) return GA_UNSUPPORTED;
        ga_palette(a,rw(0x500fcu+2u*n));
        if (!car_start(a,e,0x4fd26)) return GA_UNSUPPORTED;
        SL(0x50,rl(0x50150u+4u*n));
        ga_floor(a,e);
        SW(0x4e,0x64);SB(0x7c,0);
        SW(0x66,0x8600);SW(0x1e,0x8600);                                 /* 0501D4[+6E] -> 0501F8 */
        SL(0,0x505ac);
        goto body;
    case 0x505ac: {
        uint8_t w0=car_weight(a,e,0),w1;                                 /* 050206 */
        SB(0x7d,w0);w1=car_weight(a,e,1);SB(0x7e,w1);
        uint8_t d1=(uint8_t)(w0+w1);bool back=false;
        if (w0 && w1) d1>>=1;
        if (!B(0x7c) && d1>3u) SB(0x7c,5);
        else if (B(0x7c) && !d1) {SB(0x7c,0);back=true;}
        else if (d1!=B(0x7c)) {if (d1<=B(0x7c)) SB(0x7c,B(0x7c)-1u); else SB(0x7c,B(0x7c)+1u);}
        if (back && !car_start(a,e,0x4fd3e)) return GA_UNSUPPORTED;
    }
    body:
        ga_project(a,e);
        if (!(B(0x5f)&0x80u)) {
            bool dead=false;
            if (ga_register(a,e,NULL,NULL)) {                            /* 004C24 */
                uint8_t k=B(0x5a);
                SB(0x5a,0);
                if (k==5u) dead=true;
                else if (!car_start(a,e,(B(0x5b)&0x80u)?0x4fd56u:0x4fd6eu)) return GA_UNSUPPORTED;
            }
            if (!dead) {
                if ((r=ga_anim_step(a,e,car_call))<0) return GA_UNSUPPORTED;
                if (r && !car_start(a,e,0x4fd26)) return GA_UNSUPPORTED;
                if (ga_cull(a,e,rl(0x50108u+4u*n))) return e->unsupported?GA_UNSUPPORTED:GA_DELETE;   /* 050496 */
                car_map(a,e);
                return e->unsupported?GA_UNSUPPORTED:GA_MORE;
            }
        }
        SB(0x69,0xff);                                                   /* 05065C: 019DFC */
        car_child(a,e,0x5090a,n<3u?-0x3c:-0x44,0);
        car_child(a,e,0x5092c,n<3u?-0x1e:-0x26,-10);
        SB(0x15,B(0x15)|1u);
        car_child(a,e,0x5090a,n<3u?-0x3b:-0x33,0);
        car_child(a,e,0x5092c,n<3u?-0x1d:-0x15,-10);
        SB(0x15,B(0x15)&0xfeu);
        car_child(a,e,0x5094e,0,0);
        if (!car_start(a,e,0x4fd86)) return GA_UNSUPPORTED;
        SL(0x30,0);SL(0x34,0);SB(0x7c,0);SL(0,0x50806);
        /* fall through */
    case 0x50806:
        ga_project(a,e);
        if (n<4u) car_map(a,e);
        if ((r=ga_anim_step(a,e,car_call))<0 || e->unsupported) return GA_UNSUPPORTED;
        if (r) SL(0,0x5082c);
        return GA_MORE;
    case 0x5082c:
        if (n>=4u) goto blast;
        SB(0x15,B(0x15)|0x20u);
        if (!car_start(a,e,0x4fd9e)) return GA_UNSUPPORTED;
        SW(0x66,0x1002);SW(0x1e,0x1002);SL(0x34,0xf0000);SW(0x2c,W(0x2c)+8u);SL(0,0x50876);
        /* fall through */
    case 0x50876:
        ga_air(a,e);                                                     /* 0143E4 */
        if (!(L(0x34)&0x80000000u)) {
            if (e->dyn_set) e->dyn_set(e->ctx,a,rl(0x50138u+4u*n),0,0);   /* 050478 */
            SL(0x34,L(0x34)+0xfffec000u);
        } else {
            if (e->dyn_release) e->dyn_release(e->ctx,a);                /* 013F1E */
            SL(0x34,L(0x34)+0xffffb000u);
        }
        if ((L(0x34)&0x80000000u) && (B(0x15)&0x40u)) {
            if (!car_start(a,e,0x4fdb6)) return GA_UNSUPPORTED;
            SL(0x30,0);SL(0x34,0);SL(0,0x508dc);
            ga_project(a,e);
        }
        goto last;
    case 0x508dc:
        ga_project(a,e);
    last:
        if ((r=ga_anim_step(a,e,car_call))<0 || e->unsupported) return GA_UNSUPPORTED;
        if (!r) return GA_MORE;
    blast:
        if (e->dyn_release) e->dyn_release(e->ctx,a);                    /* 0508EE: 013F1E */
        if (e->sound) e->sound(e->ctx,0x1021);
        SB(0x1d,0);                                                      /* 009F88 */
        SL(0,0x1c71e);
        return neo_effects_tick(a,e);                                    /* jmp 01C71E */
    case 0x5090a:
        ga_palette(a,0x37c);
        if (!ga_anim_start(a,e,0x22e6ae,0x100)) return GA_UNSUPPORTED;
        goto part;
    case 0x5092c:
        ga_palette(a,0x37c);
        if (!ga_anim_start(a,e,0x247da8,0x100)) return GA_UNSUPPORTED;
        goto part;
    case 0x5094e:
        ga_palette(a,0x113);
        if (!ga_anim_start(a,e,0x24855c,0x100)) return GA_UNSUPPORTED;
        c=e->creator?e->creator(e->ctx,a):NULL;
        if (!c) {ga_fail(e,0x5096c);return GA_UNSUPPORTED;}
        if (ga_w(c,0x6e)>=3u) SW(0x28,W(0x28)-8u);
    part:
        SW(0x66,0x1001);SW(0x1e,0x1001);SL(0,0x50990);
        /* fall through */
    case 0x50990:
        ga_project(a,e);
        if ((r=ga_anim_step(a,e,NULL))<0 || e->unsupported) return GA_UNSUPPORTED;
        return r?GA_DELETE:GA_MORE;
    case 0x509a8:
    case 0x50b52: {
        bool panel=L(0)==0x509a8u;
        SB(0x15,B(0x15)|0x20u);
        c=e->creator?e->creator(e->ctx,a):NULL;
        if (!c) {ga_fail(e,L(0)+6u);return GA_UNSUPPORTED;}
        SW(0x6e,ga_w(c,0x6e));n=W(0x6e);
        if (n>5u || !rom_ok(e,0x500fcu+2u*n)) return GA_UNSUPPORTED;
        ga_palette(a,rw(0x500fcu+2u*n));
        if (panel && B(0x7f)) {SW(0x66,0x1002);SW(0x1e,0x1002);}
        if (!car_start(a,e,!panel?0x4fdfeu:B(0x7f)?0x4fde6u:0x4fdceu)) return GA_UNSUPPORTED;
        uint32_t d0=panel?0xc000u:0x10000u;
        if (panel && B(0x7f)) d0+=d0;
        if (B(0x15)&1u) d0=0u-d0;
        SL(0x30,d0);
        SL(0x34,(panel?0x50000u:0x24000u)+((uint32_t)(int32_t)(int16_t)ga_rand_range(e,0xc00)<<4));
        SW(0x7a,panel?0x3c:0x1e);SL(0,panel?0x50a66u:0x50bd0u);
        if (!panel) goto piece;}
        /* fall through */
    case 0x50a66:
        SW(0x7a,W(0x7a)-1u);
        if ((int16_t)W(0x7a)<0) return GA_DELETE;
        SL(0x76,L(0x34));
        SL(0x34,L(0x34)+0xffffb000u);
        if (W(0x7a)>=0x38u) ga_move(a,e); else ga_air(a,e);              /* 01AC38 : 0143DC */
        if (B(0x15)&0x40u) car_bounce(a);
        if (ga_anim_step(a,e,NULL)<0 || e->unsupported) return GA_UNSUPPORTED;
        return GA_MORE;
    case 0x50bd0:
    piece:
        SW(0x7a,W(0x7a)-1u);
        if ((int16_t)W(0x7a)<0) return GA_DELETE;
        SL(0x76,L(0x34));
        SL(0x34,L(0x34)+0xffffb000u);ga_air(a,e);                        /* 0143DC */
        if (B(0x15)&0x40u) {
            car_bounce(a);
            if (!car_start(a,e,0x4fe16)) return GA_UNSUPPORTED;
        }
        if (ga_anim_step(a,e,NULL)<0 || e->unsupported) return GA_UNSUPPORTED;
        return GA_MORE;
    default:
        ga_fail(e,L(0));return GA_UNSUPPORTED;
    }
}
static bool car_routine(uint32_t r) {
    return r==0x9624eu || r==0x9628eu || r==0x50542u || r==0x505acu || r==0x50806u || r==0x5082cu || r==0x50876u || r==0x508dcu || r==0x5090au ||
           r==0x5092cu || r==0x5094eu || r==0x50990u || r==0x509a8u || r==0x50a66u || r==0x50b52u || r==0x50bd0u;
}
/* ---- the metro's end 0BBF12 (script, camera 1520) -----------------------------------
 * 0BBF12: at (768, 40) of 10C72E (01AB98), +4E = 0, +76 = FFFF, hurtbox
 * 0BBAE0, palette 58E, flash 58D, layers 0, the wall 0BBAF4 (0C88B0).
 * 0BBF6C: with +4E <= 0: +5F bit 7 cleared, +4E = rank 2D3F04, ++(+76) and
 * the program 0BBE06[+76] (2A906A / 2A9080 call 0BBC7E / 0BBCE0: sound 1027
 * and two puffs 01C6EE / 01C706; 0BBCD6: the debris list 0BBDAE by 01DA40;
 * 0BBC6C: the piece 0BC192); at +76 = 3, 0BBFD0: +17 = FF, the piece 0BC100
 * and the blast 01C71E at (-18, 8 / 0) (01B220), sound 102D, the debris
 * 0BBD9C with the layers FFF2, both players' input handlers = 0BBF0E (sub.w
 * d7,d7: no input, the same result as 054B18; 0CA2E0: handlers 4), +7B = 32. Else
 * 01ABC8, 00427A, 004C24. 0BC068: 01ABC8 while +7B lasts; then 009F88,
 * palette 587, program 2A909C, layers FFF1, the wall 0BBBB0 (0C88B0) and
 * the stage exit 0B6ED2 (0040BC, D0 = 0) at (780, 28) of the root with the
 * rectangle 0BBAEC and the walk target 788; 0BC0EE: +17 = FF, 01ABC8,
 * 00427A.
 * - 0BC100: +15 bit 5, palette 58E, layers 0, program 2A910C, +30 = -1.75
 *   px, +34 = 7 px. 0BC14A: +72 = +34, 0143DC; on a floor: the second time
 *   004188, else +34 = |+72| / 2 and +5F bit 3; 00427A.
 * - 0BC192: 64 px above its creator (01AD6E), palette 58E, program 2A90A6,
 *   layers 0; 0BC1CE: 01ABC8, 00427A, deleted at the end. */
static neo_gactor_t *door_puff(neo_gactor_t *a,neo_genv_t *e,uint32_t routine,int16_t dx,int16_t dy) {   /* 0040BC (D0 = 2A), 01B220 */
    neo_gactor_t *c=e->spawn_reserve?e->spawn_reserve(e->ctx,a,routine,0x2a):e->spawn?e->spawn(e->ctx,a,routine,false):NULL;
    if (c) ga_place_from(c,a,e,dx,dy);
    return c;
}
static bool door_call(neo_gactor_t *a,neo_genv_t *e,neo_anim_kind_t kind,uint32_t routine,uint16_t parameter) {
    (void)parameter;
    if (kind!=NEO_ANIM_CALL) return false;
    switch (routine) {
    case 0xbbc6c:
        if (e->spawn_reserve) e->spawn_reserve(e->ctx,a,0xbc192,0x1c); else if (e->spawn) e->spawn(e->ctx,a,0xbc192,false);
        return true;
    case 0xbbc7e:
        if (e->sound) e->sound(e->ctx,0x1027);
        door_puff(a,e,0x1c6ee,-0x14,0x40);door_puff(a,e,0x1c6ee,-0x18,0x10);
        return true;
    case 0xbbcd6:
        debris(a,e,0xbbdae);                                             /* 01DA40 */
        return true;
    case 0xbbce0:
        if (e->sound) e->sound(e->ctx,0x1027);
        door_puff(a,e,0x1c706,-0x14,0x30);door_puff(a,e,0x1c706,-0x18,0);
        return true;
    }
    return false;
}
static ga_result_t door_tick(neo_gactor_t *a,neo_genv_t *e) {
    int r;neo_gactor_t *c;
    switch (L(0)) {
    case 0xbbf12:
        ga_place(a,e,0x768,0x40,0x10c72eu);
        SW(0x4e,0);SW(0x76,0xffff);SL(0x50,0xbbae0);
        ga_palette(a,0x58e);ga_flash_palette(a,0x58d);SW(0x66,0);SW(0x1e,0);
        if (e->stamp) e->stamp(e->ctx,a,0xbbaf4);                        /* 0C88B0 */
        SL(0,0xbbf6c);
        /* fall through */
    case 0xbbf6c:
        if ((int16_t)W(0x4e)<=0) {
            SB(0x5f,B(0x5f)&0x7fu);
            if (!rom_ok(e,0x2d3f04)) return GA_UNSUPPORTED;
            SW(0x4e,ga_rank_word(a,e,ga_rom(0x2d3f04)));
            SW(0x76,W(0x76)+1u);
            if (!rom_ok(e,0xbbe06u+4u*W(0x76)) || !ga_anim_start(a,e,rl(0xbbe06u+4u*W(0x76)),0x100)) return GA_UNSUPPORTED;
            if (W(0x76)>=3u) goto broken;
        }
        ga_project(a,e);
        if (ga_anim_step(a,e,door_call)<0) return GA_UNSUPPORTED;
        if (ga_register(a,e,NULL,NULL)) SB(0x5a,0);                       /* 004C24 */
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    broken:
        SB(0x17,0xff);
        door_puff(a,e,0xbc100,-0x18,8);door_puff(a,e,0x1c71e,-0x18,0);
        if (e->sound) e->sound(e->ctx,0x102d);
        c=e->spawn_reserve?e->spawn_reserve(e->ctx,a,0x1d764,0x1c):e->spawn?e->spawn(e->ctx,a,0x1d764,false):NULL;   /* 01DA40 */
        if (c) {
            ga_place_from(c,a,e,0,0);
            if (B(0x15)&4u) ga_sb(c,0x15,(uint8_t)(ga_b(c,0x15)|4u));
            ga_sl(c,0x86,0xbbd9c);ga_sw(c,0x66,0xfff2);ga_sw(c,0x1e,0xfff2);
        }
        if (e->flags) e->flags->player_handlers=4;                       /* 0CA2E0 x2 with 0BBF0E */
        SB(0x7b,0x32);SL(0,0xbc068);
        /* fall through */
    case 0xbc068:
        SB(0x7b,B(0x7b)-1u);
        if (!(B(0x7b)&0x80u)) {ga_project(a,e);return e->unsupported?GA_UNSUPPORTED:GA_MORE;}
        SB(0x1d,0);ga_palette(a,0x587);                                  /* 009F88, 009EA8 */
        if (!ga_anim_start(a,e,0x2a909c,0x100)) return GA_UNSUPPORTED;
        SW(0x66,0xfff1);SW(0x1e,0xfff1);
        if (e->stamp) e->stamp(e->ctx,a,0xbbbb0);                        /* 0C88B0 */
        c=e->spawn_reserve?e->spawn_reserve(e->ctx,a,0xb6ed2,0):e->spawn?e->spawn(e->ctx,a,0xb6ed2,false):NULL;
        if (c) {ga_sl(c,0x38,0x10e968u);ga_sl(c,0x28,0x7800000u);ga_sl(c,0x2c,0x280000u);ga_sl(c,0x6e,0xbbaec);ga_sw(c,0x72,0x788);}
        SL(0,0xbc0ee);
        /* fall through */
    case 0xbc0ee:
        SB(0x17,0xff);ga_project(a,e);
        if (ga_anim_step(a,e,door_call)<0 || e->unsupported) return GA_UNSUPPORTED;
        return GA_MORE;
    case 0xbc100:
        SB(0x15,B(0x15)|0x20u);ga_palette(a,0x58e);SW(0x66,0);SW(0x1e,0);
        if (!ga_anim_start(a,e,0x2a910c,0x100)) return GA_UNSUPPORTED;
        SL(0x30,0xfffe4000u);SL(0x34,0x70000);SB(0x7c,0);SL(0,0xbc14a);
        /* fall through */
    case 0xbc14a:
        SL(0x72,L(0x34));
        SL(0x34,L(0x34)+0xffffb000u);ga_air(a,e);                        /* 0143DC */
        if (B(0x15)&0x40u) {
            SB(0x7c,B(0x7c)+1u);
            if (B(0x7c)>=2u) return e->unsupported?GA_UNSUPPORTED:GA_DELETE;
            uint32_t v=L(0x72);
            if (v&0x80000000u) v=0u-v;
            SL(0x34,(uint32_t)((int32_t)v>>1));SB(0x5f,B(0x5f)|8u);
        }
        if (ga_anim_step(a,e,NULL)<0 || e->unsupported) return GA_UNSUPPORTED;
        return GA_MORE;
    case 0xbc192:
        c=e->creator?e->creator(e->ctx,a):NULL;
        ga_place(a,e,0,-0x40,c?NEO_GA_TAG(c->index):0);                  /* 01AD6E */
        ga_palette(a,0x58e);
        if (!ga_anim_start(a,e,0x2a90a6,0x100)) return GA_UNSUPPORTED;
        SW(0x66,0);SW(0x1e,0);SL(0,0xbc1ce);
        /* fall through */
    case 0xbc1ce:
        ga_project(a,e);
        if ((r=ga_anim_step(a,e,NULL))<0 || e->unsupported) return GA_UNSUPPORTED;
        return r?GA_DELETE:GA_MORE;
    default:
        ga_fail(e,L(0));return GA_UNSUPPORTED;
    }
}
static bool door_routine(uint32_t r) {return r==0xbbf12u || r==0xbbf6cu || r==0xbc068u || r==0xbc0eeu || r==0xbc100u || r==0xbc14au || r==0xbc192u || r==0xbc1ceu;}
bool neo_m5props_routine(uint32_t r) {return drip_routine(r) || deco_routine(r) || car_routine(r) || door_routine(r) || r==0xbc296u || r==0xbc30au || r==0xbc3a6u || r==0xbc42cu;}
ga_result_t neo_m5props_tick(neo_gactor_t *a,neo_genv_t *e) {
    uint32_t r=L(0);
    if (r==0x9624eu || r==0x9628eu) return ledge_tick(a,e);
    if (door_routine(r)) return door_tick(a,e);
    return drip_routine(r)?drip_tick(a,e):deco_routine(r)?deco_tick(a,e):car_routine(r)?car_tick(a,e):gate_tick(a,e);
}
