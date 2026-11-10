#include "neo_region.h"
/* Mission 4 props of the camera list 0E741A, manual translation (ROM labels
 * as provenance; MAME run4 replay from sec05_149829, docs/MISION4.md).
 * RAM 10C6C0..10C6FF (neo_ga_flags_t m4): +00..+1F one byte per prop, +20
 * (10C6E0) the lamp's signal, +22/+24 (10C6E2/10C6E4) the two collapse
 * pieces that can be hit, +26..+3F (10C6E6) one byte per collapse piece.
 * - 0BA942 (camera-list call, head): 10C6E0 = 0, 10C6E6..10C6FF = FF,
 *   10C6C0..10C6DF = 0 (neo_m4props_reset).
 * - 0BA964..0BAA1E -> 0BAA24 (D7 = 0..1F): the prop 0B9F4A after the script
 *   actor (00411C, D0 = 0E) with +6E = D7, +28/+2C = 0BAA68[D7] of the root
 *   10E968. 0BA82A / 0BA85A / 0BA88A / 0BA912: 0B99EE at (0B60, FFA8),
 *   0B9B24 at (0500, 00E0), 0B9C7A at (0730, 0028), 0B9AD6 at (06A0, FF80)
 *   of the root (D0 = 1C). 0BA8BA (word argument n): the collapse piece
 *   0B9D88 with +70 = n, +6E = 0E7A22[n], at (0E7A20[n], 0028) of the root
 *   (01AB9C) unless 0E7A20[n] < the camera x 10E98E. All in neo_world.c.
 * - 0B9F4A: +70 = its record 0E68D0 + 24 * +6E, +50 = record +2, +74/+76 =
 *   record +6/+8 (block column/row of its patches), +78..+86 an attack row
 *   around the hurtbox (x - 10, y - 10, w + 20, h + 20, 1, 1, 1, FFFF).
 *   0B9FBC: +4E = record +0, 004C1C when record +22. 0B9FD8: 01ABC8; with
 *   10C6C0[+6E] < 2, 004C24: a kill (+5F bit 7, +4E <= 0) runs 0049E8 with
 *   the row, the sound record +20, the callback record +E (the map patches
 *   0C70FC below), 01DA40 with the list record +A (its +66/+1E = 1001), the
 *   callback record +12 (0BA226: the debris 0BB186 with 0E7A9E[+6E]), the
 *   items 0BB034 of 2D5522[2 * 2D5502[+6E] + (rand & 1)] (none when
 *   negative) and, with record +16 = 0, 004188. 0BA0AA: outside record +18
 *   (01945C) +74 += 20 and the callback record +1C (none: 004188 after the
 *   cartridge check): the props 1C..1F of the boss's road copy the stage
 *   map 0E6DA8 again 0x20 columns ahead (0C70FC), +28 += 200, 10C6C0[+6E] =
 *   0 and 0BA192 waits to be inside the rectangle again (-> 0B9FBC). Prop
 *   1D waits in 0BA1B8 (rectangle 0BA210) before its copy 0BA218.
 * - The patch callbacks 0BA288..0BA7C2 (0BA81A: 0C70FC on 10C72E, D0 = D1 =
 *   0): props 0..5 (the street's front) choose each shared edge by the
 *   neighbour's byte; 6..1A, 1C, 1E one sub-map at (+74, +76); 1B and 1D
 *   two hits (10C6C0 counts; the first restores +4E); 1F the stage map
 *   0E6DA8 (5A, 1F, 4, 4). Prop 5 also sets 10C6E0 = FFFF (the lamp falls).
 * - 0BB186 / 0BB236 -> 0BB19E: D2 + 1 pieces 0BB106 after the script actor
 *   (00411C, D0 = 2A) at (D0, D1) of the caller (01ACF2) with random speeds
 *   and +6E = (rand & mask) + D3, +70 = 0BB0B6, +74 = 0BB0E6. 0BB106:
 *   palette +74[+6E], program +70[+6E], +26 = (rand & FF) + C0, layers
 *   E0ED; 0BB15C: gravity -0.625, 01AC38, 00427A, 004188 outside 0BB0FE.
 * - 0BB034 (A1 = eight records of 0x12 bytes): the item 08CD58 of kind +0
 *   (FFFF: none, the dummy node) at (+A + rand & 1F, +C + rand & 1F), +70 =
 *   +E, +72 = +10, speeds +2/+6 plus twice a random word (x: either sign).
 * - 0B99EE (the floor of section 3): program 2A8EFC, palette 0E0; 0B9A1E:
 *   01436A, 00427A, the cell map 0E79D4 (0140C0), 004188 outside 0E798C.
 * - 0B9AD6: program 2A8F3E, layers FFF1, palette 532; 0B9B06: 01ABC8,
 *   00427A, 004188 at 10E99E > 720.
 * - 0B9B24 (the lamp): program 2A8F4A, palette 54E; 0B9B54 waits for
 *   10C6E0; 0B9B6C/0B9B7A: it falls (gravity -0.3125, 0143E4) to the floor;
 *   0B9BA2/0B9BB4: two updates later 01DBAA (16 pieces 01D31C) and 004188.
 *   Every state ends in 004188 outside 0E7994.
 * - 0B9C7A (the collapse's turn keeper): 10C6E2 = 0, 10C6E4 = 1; 0B9C8E:
 *   every second update 0B9D58 moves 10C6E2 to the next piece whose byte
 *   10C6E6 is clear, and 10C6E4 = 10C6E2 + 4 the same way; 004188 outside
 *   0E799C.
 * - 0B9D88 (a collapse piece): program 2A8F56[+6E], hurtbox 0E79A4[+6E],
 *   +15 bit 2, palette 575. 0B9DE4: only the pieces 10C6E2 / 10C6E4 take
 *   004C24 (a hit: 0B9E5E); 01ABC8, its byte 10C6E6 = FF unless 10 < x <=
 *   130, 00427A, 004188 at x < -30. 0B9E5E: program 2A8F62[+6E], the
 *   debris 0BB236, sound 102E; 0B9EA4: 01ABC8, 00427A, 004188 at x < -30.
 * The cartridge checks (2FFFE0) before the deletions are not reproduced:
 * they end in 004188 on genuine hardware. */
#define B(o) ga_b(a,(o))
#define W(o) ga_w(a,(o))
#define L(o) ga_l(a,(o))
#define SB(o,v) ga_sb(a,(o),(uint8_t)(v))
#define SW(o,v) ga_sw(a,(o),(uint16_t)(v))
#define SL(o,v) ga_sl(a,(o),(uint32_t)(v))
static bool rom_ok(neo_genv_t *e,uint32_t at) {if (ga_rom(at)) return true;ga_fail(e,at);return false;}
static uint32_t rl(uint32_t at) {const uint8_t *p=ga_rom(at);return p?((uint32_t)p[0]<<24|(uint32_t)p[1]<<16|(uint32_t)p[2]<<8|p[3]):0;}
static uint16_t rw(uint32_t at) {const uint8_t *p=ga_rom(at);return p?(uint16_t)(p[0]<<8|p[1]):0;}
static uint8_t *ram(neo_genv_t *e) {return e->flags?e->flags->m4:NULL;}
static uint16_t gw(const uint8_t *m,unsigned o) {return (uint16_t)(m[o]<<8|m[o+1]);}
static void sw(uint8_t *m,unsigned o,uint16_t v) {m[o]=(uint8_t)(v>>8);m[o+1]=(uint8_t)v;}
void neo_m4props_reset(neo_ga_flags_t *f) {   /* 0BA942 */
    sw(f->m4,0x20,0);
    for (unsigned i=0;i<0x1a;i++) f->m4[0x26+i]=0xff;
    for (unsigned i=0;i<0x20;i++) f->m4[i]=0;
}
/* 0BA81A / 0BA24A: 0C70FC on 10C72E. */
static void patch(neo_genv_t *e,uint32_t submap,int16_t d0,int16_t d1,int16_t d2,int16_t d3,int16_t d4,int16_t d5) {
    const int16_t d[6]={d0,d1,d2,d3,d4,d5};
    if (e->map_patch) e->map_patch(e->ctx,0x10c72eu,submap,d);
}
/* 0BB19E: `count` pieces 0BB106 at (d0, d1) of the caller. */
typedef struct { int16_t d0,d1;uint16_t count,base,vy_mask,vy_add,vx_mask,vx_add,kind_mask; } debris_t;
static void debris(neo_gactor_t *a,neo_genv_t *e,const debris_t *p) {
    for (unsigned i=0;i<p->count;i++) {
        neo_gactor_t *c=e->spawn?e->spawn(e->ctx,NULL,0xbb106,true):NULL;   /* 00411C, D0 = 2A */
        if (c) ga_place_at(c,a,e,p->d0,p->d1);                              /* 01ACF2 */
        uint16_t r=ga_rand(e);
        if (c) ga_sw(c,0x34,(uint16_t)((r&p->vy_mask)+p->vy_add));
        r=ga_rand(e);
        if (c) {ga_sw(c,0x32,r);ga_sw(c,0x30,(uint16_t)((r&p->vx_mask)+p->vx_add));}
        r=ga_rand(e);
        if (c && (r&1u)) ga_sl(c,0x30,0u-ga_l(c,0x30));
        r=ga_rand(e);
        if (c) {ga_sw(c,0x6e,(uint16_t)((r&p->kind_mask)+p->base));ga_sl(c,0x70,0xbb0b6);ga_sl(c,0x74,0xbb0e6);}
    }
}
/* 0BB034 */
static void items(neo_gactor_t *a,neo_genv_t *e,uint32_t a1) {
    for (unsigned i=0;i<8;i++,a1+=0x12) {
        if (!rom_ok(e,a1)) return;
        int16_t d1=(int16_t)((ga_rand(e)&0x1fu)+rw(a1+0xc));
        int16_t d0=(int16_t)((ga_rand(e)&0x1fu)+rw(a1+0xa));
        uint16_t kind=rw(a1);
        neo_gactor_t *c=NULL;
        if (kind<0x5au) {                                                    /* 08CD58 -> 08CDB2 */
            c=e->spawn?e->spawn(e->ctx,NULL,0x8c374,true):NULL;
            if (c) {
                ga_sb(c,0x8b,0);ga_sw(c,0x70,rw(a1+0xe));ga_sw(c,0x6e,kind);ga_sw(c,0x72,0);ga_sb(c,0x7f,0);
                ga_place_at(c,a,e,d0,d1);ga_sw(c,0x72,rw(a1+0x10));
            }
        }
        uint16_t r=ga_rand(e);
        uint32_t d2=(uint32_t)r*2u;
        if (!(r&1u)) d2=0u-d2;
        if (c) ga_sl(c,0x30,d2+rl(a1+2));
        r=ga_rand(e);
        if (c) ga_sl(c,0x34,(uint32_t)r*2u+rl(a1+6));
    }
}
/* 01DA40 with the caller's +66/+1E = 1001 on the emitter (0BA03E). */
static void emitter(neo_gactor_t *a,neo_genv_t *e,uint32_t list) {
    neo_gactor_t *c=e->spawn_reserve?e->spawn_reserve(e->ctx,a,0x1d764,0x1c):e->spawn?e->spawn(e->ctx,a,0x1d764,false):NULL;
    if (!c) return;
    ga_place_from(c,a,e,0,0);
    if (B(0x15)&4u) ga_sb(c,0x15,(uint8_t)(ga_b(c,0x15)|4u));
    ga_sl(c,0x86,list);ga_sw(c,0x66,0x1001);ga_sw(c,0x1e,0x1001);
}
/* The callbacks record +E: 0BA288..0BA7C2. */
static void destroyed(neo_gactor_t *a,neo_genv_t *e,uint8_t *m) {
    unsigned n=W(0x6e);int16_t c=(int16_t)W(0x74),r=(int16_t)W(0x76);
    /* one sub-map at (+74, +76): 0BA4F2..0BA6D2, 0BA73E, 0BA7AA */
    static const struct { uint32_t sub;uint8_t w,h; } one[0x20]={[6]={0xe6fce,8,6},[7]={0xe6fe4,7,7},[8]={0xe6ffa,6,6},[9]={0xe7010,5,5},
        [0xa]={0xe7026,7,7},[0xb]={0xe70aa,7,4},[0xc]={0xe70c0,7,5},[0xd]={0xe70d6,6,6},[0xe]={0xe6ffa,6,6},[0xf]={0xe7010,5,5},
        [0x10]={0xe70ec,7,4},[0x11]={0xe70c0,7,5},[0x12]={0xe7102,6,6},[0x13]={0xe703c,7,7},[0x14]={0xe6ffa,6,6},[0x15]={0xe7010,5,5},
        [0x16]={0xe7118,7,4},[0x17]={0xe712e,7,5},[0x18]={0xe7144,7,6},[0x19]={0xe715a,8,7},[0x1a]={0xe7052,7,4},[0x1c]={0xe7186,8,5},
        [0x1e]={0xe7094,4,9}};
    switch (n) {
    case 0:   /* 0BA288 */
        m[0]=0xff;
        patch(e,m[1]?0xe6e42:0xe6e16,0,0,3,8,0x2a,0xb);patch(e,0xe6e00,0,0,6,8,0x24,0xb);return;
    case 1:   /* 0BA2CE */
        m[1]=0xff;
        patch(e,m[0]?0xe6e42:0xe6e2c,0,0,3,8,0x2a,0xb);patch(e,m[2]?0xe6e9a:0xe6e6e,0,0,2,8,0x31,0xb);patch(e,0xe6e58,0,0,4,8,0x2d,0xb);return;
    case 2:   /* 0BA342 */
        m[2]=0xff;
        patch(e,m[1]?0xe6e9a:0xe6e84,0,0,2,8,0x31,0xb);patch(e,m[3]?0xe6ef2:0xe6ec6,0,0,3,8,0x35,0xb);patch(e,0xe6eb0,0,0,2,8,0x33,0xb);return;
    case 3:   /* 0BA3B8 */
        m[3]=0xff;
        patch(e,m[2]?0xe6ef2:0xe6edc,0,0,3,8,0x35,0xb);patch(e,m[4]?0xe6f4a:0xe6f1e,0,0,3,0xc,0x3f,0xf);patch(e,0xe6f08,0,0,7,8,0x38,0xb);return;
    case 4:   /* 0BA42E */
        m[4]=0xff;
        patch(e,m[3]?0xe6f4a:0xe6f34,0,0,3,0xc,0x3f,0xf);patch(e,m[5]?0xe6fa2:0xe6f76,0,0,3,0xc,0x4a,0xf);patch(e,0xe6f60,0,0,8,0xc,0x42,0xf);return;
    case 5:   /* 0BA4A4 */
        sw(m,0x20,0xffff);m[5]=0xff;
        patch(e,m[4]?0xe6fa2:0xe6f8c,0,0,3,0xc,0x4a,0xf);patch(e,0xe6fb8,0,0,7,0xc,0x4d,0xf);return;
    case 0x1b: case 0x1d: {   /* 0BA6EA / 0BA756 */
        bool wide=n==0x1d;
        if (!m[n]) {
            m[n]++;
            SW(0x4e,rw(L(0x70)));SB(0x5a,0);SB(0x5f,B(0x5f)&0x7fu);
            patch(e,wide?0xe707e:0xe7068,0,0,wide?0x10:7,5,c,r);
        } else if (m[n]==1u) {
            m[n]++;
            patch(e,wide?0xe719c:0xe7170,0,0,wide?0x10:8,5,c,r);
        }
        return;
    }
    case 0x1f:   /* 0BA7C2 */
        m[n]=0xff;patch(e,0xe6da8,0x5a,0x1f,4,4,c,r);return;
    default:
        if (!one[n].sub) {ga_fail(e,rl(L(0x70)+0xe));return;}
        m[n]=0xff;patch(e,one[n].sub,0,0,one[n].w,one[n].h,c,r);return;
    }
}
/* The callbacks record +1C (0BA240..0BA27C) and 0BA218: the stage map again. */
static bool restore(neo_gactor_t *a,neo_genv_t *e,uint32_t fn) {
    int16_t c=(int16_t)W(0x74),r=(int16_t)W(0x76);
    switch (fn) {
    case 0xba240: patch(e,0xe6da8,0x70,0x1e,7,5,c,r);return true;
    case 0xba264: patch(e,0xe6da8,0x78,0x1e,4,5,c,r);return true;
    case 0xba270: patch(e,0xe6da8,0x88,0x1a,4,9,c,r);return true;
    case 0xba27c: patch(e,0xe6da8,0x8c,0x1f,4,4,c,r);return true;
    case 0xba218: patch(e,0xe6da8,0x78,0x1e,0x10,5,c,r);return true;
    }
    ga_fail(e,fn);return false;
}
/* 0B9D58. The scan covers 10C6E6 + 0..1A: its last byte is RAM 10C700, the
 * high byte of the Mission 3 limit (7FFF after 0BC548, as Mission 3 leaves
 * it: MAME run4 149850). */
static void next_piece(uint8_t *m,unsigned at,uint8_t b700) {
    uint16_t v=(uint16_t)(gw(m,at)+1u);
    if (v>0x1au) v=0;
    uint16_t d7=v;
    for (unsigned d6=0x1a;d6;d6--) {
        if (!(d7<0x1au?m[0x26+d7]:b700)) break;
        if (++d7>0x1au) d7=0;
    }
    sw(m,at,d7);
}
static ga_result_t m4props_tick(neo_gactor_t *a,neo_genv_t *e) {
    uint8_t *m=ram(e);
    if (!m) {ga_fail(e,L(0));return GA_UNSUPPORTED;}
    switch (L(0)) {
    /* ---- 0B9F4A: a prop ---- */
    case 0xb9f4a: {
        uint32_t rec=0xe68d0u+0x24u*W(0x6e);
        if (W(0x6e)>=0x20u || !rom_ok(e,rec)) return GA_UNSUPPORTED;
        SL(0x70,rec);SL(0x50,rl(rec+2));SW(0x74,rw(rec+6));SW(0x76,rw(rec+8));
        uint32_t h=L(0x50);
        if (!rom_ok(e,h)) return GA_UNSUPPORTED;
        SW(0x78,rw(h+2)-0x10u);SW(0x7a,rw(h+4)-0x10u);SW(0x7c,rw(h+6)+0x20u);SW(0x7e,rw(h+8)+0x20u);
        SW(0x80,1);SW(0x82,1);SW(0x84,1);SW(0x86,0xffff);
    }   /* fall through */
    case 0xb9fbc:
    rearm: {
        uint32_t rec=L(0x70);
        SW(0x4e,rw(rec));
        if (rw(rec+0x22)) SB(0x5e,B(0x5e)|0x80u);                       /* 004C1C */
        SL(0,0xb9fd8);
    }   /* fall through */
    case 0xb9fd8: {
        uint32_t rec=L(0x70);unsigned n=W(0x6e);
        ga_project(a,e);
        if (m[n]<2u && ga_register(a,e,NULL,NULL)) {
            SB(0x5a,0);
            if ((B(0x5f)&0x80u) && (int16_t)W(0x4e)<=0) {
                int16_t row[7];for (unsigned i=0;i<7;i++) row[i]=(int16_t)W(0x78+2*i);
                if (!e->attack) {ga_fail(e,0x49e8);return GA_UNSUPPORTED;}
                e->attack(e->ctx,a,row);                                 /* 0049E8 with the row at +78 */
                if (e->sound) e->sound(e->ctx,rw(rec+0x20));
                destroyed(a,e,m);                                        /* record +E */
                emitter(a,e,rl(rec+0xa));                                /* 01DA40 */
                uint32_t fn=rl(rec+0x12);
                if (fn==0xba226u) {                                      /* 0BB186 with 0E7A9E[+6E] */
                    uint32_t t=0xe7a9eu+8u*n;
                    if (!rom_ok(e,t)) return GA_UNSUPPORTED;
                    const debris_t p={(int16_t)rw(t),(int16_t)rw(t+2),(uint16_t)(rw(t+4)+1u),rw(t+6),0xf,1,7,0,3};
                    debris(a,e,&p);
                } else if (fn) {ga_fail(e,fn);return GA_UNSUPPORTED;}
                if (!rom_ok(e,0x2d5502u+n)) return GA_UNSUPPORTED;
                int8_t set=(int8_t)ga_rom(0x2d5502u+n)[0];
                if (set>=0) {
                    uint16_t k=(uint16_t)((ga_rand(e)&1u)+2u*(uint8_t)set);
                    if (!rom_ok(e,0x2d5522u+4u*k)) return GA_UNSUPPORTED;
                    items(a,e,rl(0x2d5522u+4u*k));
                }
                if (e->unsupported) return GA_UNSUPPORTED;
                if (!rw(rec+0x16)) return GA_DELETE;
            }
        }
        /* 0BA0AA */
        if (!ga_cull(a,e,rl(rec+0x18))) return e->unsupported?GA_UNSUPPORTED:GA_MORE;
        SW(0x74,W(0x74)+0x20u);
        uint32_t fn=rl(rec+0x1c);
        if (!fn) return e->unsupported?GA_UNSUPPORTED:GA_DELETE;
        if (!restore(a,e,fn)) return GA_UNSUPPORTED;
        if (n==0x1du) {SL(0,0xba1b8);goto wait_1d;}
        SW(0x28,W(0x28)+0x200u);SB(0x5a,0);SB(0x5f,B(0x5f)&0x7fu);m[n]=0;SL(0,0xba192);
    }   /* fall through */
    case 0xba192:
        ga_project(a,e);
        if (ga_cull(a,e,rl(L(0x70)+0x18))) return e->unsupported?GA_UNSUPPORTED:GA_MORE;
        goto rearm;
    case 0xba1b8:
    wait_1d:
        ga_project(a,e);
        if (!ga_cull(a,e,0xba210)) return e->unsupported?GA_UNSUPPORTED:GA_MORE;
        if (!restore(a,e,0xba218)) return GA_UNSUPPORTED;
        SW(0x28,W(0x28)+0x200u);SB(0x5a,0);SB(0x5f,B(0x5f)&0x7fu);m[W(0x6e)&0x1fu]=0;SL(0,0xba1f4);
        /* fall through */
    case 0xba1f4:
        ga_project(a,e);
        if (ga_cull(a,e,0xba210)) return e->unsupported?GA_UNSUPPORTED:GA_MORE;
        goto rearm;
    /* ---- 0BB106: a piece of debris ---- */
    case 0xbb106: {
        uint32_t pt=L(0x74)+2u*W(0x6e),gt=L(0x70)+4u*W(0x6e);
        if (!rom_ok(e,pt) || !rom_ok(e,gt)) return GA_UNSUPPORTED;
        ga_palette(a,rw(pt));
        if (!ga_anim_start(a,e,rl(gt),0x100)) return GA_UNSUPPORTED;
        SW(0x26,(ga_rand(e)&0xffu)+0xc0u);SW(0x66,0xe0ed);SW(0x1e,0xe0ed);SL(0,0xbb15c);
    }   /* fall through */
    case 0xbb15c:
        SL(0x34,L(0x34)+0xffff6000u);ga_move(a,e);
        if (ga_anim_step(a,e,NULL)<0 || e->unsupported) return GA_UNSUPPORTED;
        return ga_cull(a,e,0xbb0fe)?GA_DELETE:GA_MORE;
    /* ---- 0B99EE: the floor piece of section 3 ---- */
    case 0xb99ee:
        if (!ga_anim_start(a,e,0x2a8efc,0x100)) return GA_UNSUPPORTED;
        SW(0x66,0);SW(0x1e,0);ga_palette(a,0xe0);SL(0,0xb9a1e);
        /* fall through */
    case 0xb9a1e:
        ga_stationary(a,e);
        if (ga_anim_step(a,e,NULL)<0 || e->unsupported) return GA_UNSUPPORTED;
        if (ga_cull(a,e,0xe798c)) return GA_DELETE;
        if (e->dyn_set) e->dyn_set(e->ctx,a,0xe79d4,0,0);                /* 0140C0 */
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    /* ---- 0B9AD6 ---- */
    case 0xb9ad6:
        if (!ga_anim_start(a,e,0x2a8f3e,0x100)) return GA_UNSUPPORTED;
        SW(0x66,0xfff1);SW(0x1e,0xfff1);ga_palette(a,0x532);SL(0,0xb9b06);
        /* fall through */
    case 0xb9b06:
        ga_project(a,e);
        if (ga_anim_step(a,e,NULL)<0 || e->unsupported) return GA_UNSUPPORTED;
        return e->camera>0x720u?GA_DELETE:GA_MORE;
    /* ---- 0B9B24: the lamp ---- */
    case 0xb9b24:
        if (!ga_anim_start(a,e,0x2a8f4a,0x100)) return GA_UNSUPPORTED;
        SW(0x66,0);SW(0x1e,0);ga_palette(a,0x54e);SL(0,0xb9b54);
        /* fall through */
    case 0xb9b54:
        if (gw(m,0x20)) SL(0,0xb9b6c);
        ga_project(a,e);
        goto lamp_tail;
    case 0xb9b6c:
        SL(0x30,0);SL(0x34,0);SL(0,0xb9b7a);
        /* fall through */
    case 0xb9b7a:
        SL(0x34,L(0x34)+0xffffb000u);ga_air(a,e);
        if (B(0x15)&0x40u) SL(0,0xb9ba2);
        if (ga_anim_step(a,e,NULL)<0) return GA_UNSUPPORTED;
        goto lamp_tail;
    case 0xb9ba2:
        SW(0x34,2);SB(0x13,0xc0);SL(0,0xb9bb4);
        /* fall through */
    case 0xb9bb4:
        ga_project(a,e);
        SW(0x34,W(0x34)-1u);
        if (!W(0x34)) {
            for (unsigned i=0;i<16;i++) {                                /* 01DBAA */
                neo_gactor_t *c=e->spawn_reserve?e->spawn_reserve(e->ctx,a,0x1d31c,0x2a):e->spawn?e->spawn(e->ctx,a,0x1d31c,false):NULL;
                if (c) ga_place_from(c,a,e,0,0x18);
            }
            if (ga_anim_step(a,e,NULL)<0 || e->unsupported) return GA_UNSUPPORTED;
            return GA_DELETE;
        }
        if (ga_anim_step(a,e,NULL)<0) return GA_UNSUPPORTED;
    lamp_tail:
        if (e->unsupported) return GA_UNSUPPORTED;
        return ga_cull(a,e,0xe7994)?GA_DELETE:GA_MORE;
    /* ---- 0B9C7A: the collapse's turn keeper ---- */
    case 0xb9c7a:
        sw(m,0x22,0);sw(m,0x24,1);SB(0x6e,0);SL(0,0xb9c8e);
        /* fall through */
    case 0xb9c8e:
        ga_project(a,e);
        if (B(0x6e)&1u) {
            uint8_t b700=(uint8_t)(e->flags->limit700>>8);
            next_piece(m,0x22,b700);
            sw(m,0x24,(uint16_t)(gw(m,0x22)+4u));next_piece(m,0x24,b700);
        }
        SB(0x6e,B(0x6e)+1u);
        if (e->unsupported) return GA_UNSUPPORTED;
        return ga_cull(a,e,0xe799c)?GA_DELETE:GA_MORE;
    /* ---- 0B9D88: a collapse piece ---- */
    case 0xb9d88: {
        uint32_t k=4u*W(0x6e);
        if (W(0x6e)>2u || !rom_ok(e,0x2a8f56u+k) || !rom_ok(e,0xe79a4u+k)) return GA_UNSUPPORTED;
        if (!ga_anim_start(a,e,rl(0x2a8f56u+k),0x100)) return GA_UNSUPPORTED;
        SL(0x50,rl(0xe79a4u+k));SW(0x66,0);SW(0x1e,0);SB(0x15,B(0x15)|4u);ga_palette(a,0x575);
        SW(0x4e,0);SL(0,0xb9de4);
    }   /* fall through */
    case 0xb9de4: {
        uint16_t d7=W(0x70);
        if (d7>=0x1au) {ga_fail(e,0xb9e22);return GA_UNSUPPORTED;}
        if (d7==gw(m,0x22) || d7==gw(m,0x24)) {
            if (ga_register(a,e,NULL,NULL)) {                            /* 0B9E5E */
                SB(0x5a,0);
                if (!rom_ok(e,0x2a8f62u+4u*W(0x6e)) || !ga_anim_start(a,e,rl(0x2a8f62u+4u*W(0x6e)),0x100)) return GA_UNSUPPORTED;
                m[0x26+d7]=0xff;
                ga_project(a,e);
                static const debris_t p={0,0,10,0xa,7,3,7,3,1};           /* 0BB236 */
                debris(a,e,&p);
                if (e->sound) e->sound(e->ctx,0x102e);
                SL(0,0xb9ea4);
                goto broken;
            }
        } else {SB(0x5f,B(0x5f)&0x7fu);SW(0x4e,0);}
        ga_project(a,e);
        m[0x26+d7]=0xff;
        int16_t x=(int16_t)W(0x3c);
        if (x>0x10) m[0x26+d7]=x>0x130?0xff:0;
        if (ga_anim_step(a,e,NULL)<0 || e->unsupported) return GA_UNSUPPORTED;
        if ((int16_t)W(0x3c)<-0x30) {m[0x26+d7]=0xff;return GA_DELETE;}
        return GA_MORE;
    }
    case 0xb9ea4:
    broken:
        ga_project(a,e);
        if (ga_anim_step(a,e,NULL)<0 || e->unsupported) return GA_UNSUPPORTED;
        return (int16_t)W(0x3c)<-0x30?GA_DELETE:GA_MORE;
    }
    ga_fail(e,L(0));return GA_UNSUPPORTED;
}
bool neo_m4props_routine(uint32_t r) {
    static const uint32_t k[]={0xb9f4a,0xb9fbc,0xb9fd8,0xba192,0xba1b8,0xba1f4,0xbb106,0xbb15c,0xb99ee,0xb9a1e,0xb9ad6,0xb9b06,
        0xb9b24,0xb9b54,0xb9b6c,0xb9b7a,0xb9ba2,0xb9bb4,0xb9c7a,0xb9c8e,0xb9d88,0xb9de4,0xb9ea4};
    for (unsigned i=0;i<sizeof k/sizeof k[0];i++) if (r==k[i]) return true;
    return false;
}
ga_result_t neo_m4props_tick(neo_gactor_t *a,neo_genv_t *e) {return m4props_tick(a,e);}
