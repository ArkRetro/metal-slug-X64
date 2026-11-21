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
/* 01DA40, then the caller's +66/+1E on the emitter (0BA03E: 1001; 0: as 01B220 left them). */
static void emitter_at(neo_gactor_t *a,neo_genv_t *e,uint32_t list,uint16_t layer) {
    neo_gactor_t *c=e->spawn_reserve?e->spawn_reserve(e->ctx,a,0x1d764,0x1c):e->spawn?e->spawn(e->ctx,a,0x1d764,false):NULL;
    if (!c) return;
    ga_place_from(c,a,e,0,0);
    if (B(0x15)&4u) ga_sb(c,0x15,(uint8_t)(ga_b(c,0x15)|4u));
    ga_sl(c,0x86,list);
    if (layer) {ga_sw(c,0x66,layer);ga_sw(c,0x1e,layer);}
}
static void emitter(neo_gactor_t *a,neo_genv_t *e,uint32_t list) {emitter_at(a,e,list,0x1001);}
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
/* ---- the bins 033526 of the street (script records 2DB6A2 / 2DB86E) ------------
 * 033526 (+6E = 0..2: the record's first word; +70 the things it holds, +72
 * the one that is the prisoner, +74 its kind, +76..+7F its words): palette
 * 0334BA[+6E], program 03351A[+6E], 87C0, hurtbox 0334CE[+6E], +4E = rank
 * 2D3EBE, +80 = 03350E[+6E] (where things come out), +84 = 2D3ED8[+6E] (its
 * eight item kinds), 0146E8, 01AD8C, y - 4. 0335C6: 01436A; 004C24 -> +88 =
 * FF; +89 = FF with a player standing inside 033506 (033766: 01B332 and
 * 01AF1A, player 1 then player 2); 00427A (the programs test +88 / +89;
 * their calls 239C44 / 239C58 / 239C6C step +8A to F / 10 / 11 and 239C80
 * with its word W: while +70, when rand * W >> 16 is 0, --+70, --+72: at 0
 * the prisoner 239D56[+74] (00411C, D0 = 2A; 01B220 at +80/+82; words +76..;
 * speed 002E7C of ((rand & FF) + 500) << 8 at the angle (rand & 1F) + 30),
 * else the item 08CD64 of kind +84[rand & 7] with random speeds; then the
 * puff 03378A at +80/+82 with +6E); 004188 outside 0334C6; alive: the cell
 * map 0333DC[+6E] (0140C0); +5F bit 7: sound 1025, 01DBAA, 013F1E, the smoke
 * 0897E2 and two pieces 033864 at (+-10, 8) after the script actor, deleted.
 * - 03378A: program 239BCA, palette 0334C0[+6E], E0ED; deleted at its end.
 * - 033864: palette 0500FC[+6E], program 04FDFE[+6E], E0E8, +8C = 3C, 0146E8;
 *   0338B8: deleted when +8C runs out; gravity -0.3125, 0143E4; landing:
 *   +34 = |+34| * 3 / 4, +5F bit 3, program 04FE16[+6E]; 00427A. */
/* 033766: player `p` exists (01AEB0), inside the rectangle 033506 (01AF3C:
 * both bounds inclusive) and on the ground (01AF1A). */
static bool bin_stood(neo_gactor_t *a,neo_genv_t *e,unsigned p) {
    if (!((e->players.active_mask>>p)&1u)) return false;
    const uint8_t *r=ga_rom(0x33506);
    if (!r) {ga_fail(e,0x33506);return false;}
    int16_t q[4];for (unsigned i=0;i<4;i++) q[i]=(int16_t)(r[2*i]<<8|r[2*i+1]);
    int16_t dy=(int16_t)((p?e->players.p2y:e->players.p1y)-W(0x40)),dx=(int16_t)((p?e->players.p2x:e->players.p1x)-W(0x3c));
    if (B(0x15)&2u) dy=(int16_t)-dy;
    if (dy<q[1] || dy>(int16_t)(q[1]+q[3])) return false;
    if (B(0x15)&1u) dx=(int16_t)-dx;
    if (dx<q[0] || dx>(int16_t)(q[0]+q[2])) return false;
    return (e->player_flags15[p]&0x40u)!=0;
}
static bool bin_call(neo_gactor_t *a,neo_genv_t *e,neo_anim_kind_t kind,uint32_t routine,uint16_t parameter) {
    if (kind!=NEO_ANIM_CALL) return false;
    unsigned top=routine==0x239c44u?0xf:routine==0x239c58u?0x10:routine==0x239c6cu?0x11:0;
    if (top) {SB(0x8a,B(0x8a)+1u);if ((int8_t)B(0x8a)>=(int8_t)top) SB(0x8a,0);return true;}
    if (routine!=0x239c80u) return false;
    if (!W(0x70)) return true;
    if (ga_rand_range(e,parameter)) return true;
    SW(0x70,W(0x70)-1u);SW(0x72,W(0x72)-1u);
    int16_t ox=(int16_t)W(0x80),oy=(int16_t)W(0x82);
    if (!W(0x72)) {                                                      /* 239CDA: the prisoner */
        if (W(0x74)>3u || !rom_ok(e,0x239d56u+4u*W(0x74))) return true;
        neo_gactor_t *c=e->spawn?e->spawn(e->ctx,NULL,rl(0x239d56u+4u*W(0x74)),true):NULL;
        if (c) {ga_place_from(c,a,e,ox,oy);for (unsigned o=0;o<10;o+=2) ga_sw(c,0x6eu+o,W(0x76u+o));}
        uint8_t angle=(uint8_t)((ga_rand(e)&0x1fu)+0x30u);
        uint32_t speed=(uint32_t)(int32_t)(int16_t)((ga_rand(e)&0xffu)+0x500u)<<8,vx,vy;
        ga_polar(speed,angle,&vx,&vy);
        if (c) {ga_sl(c,0x30,vx);ga_sl(c,0x34,vy);}
    } else {                                                             /* 239D66: an item */
        uint16_t k=(uint16_t)(ga_rand(e)&7u);
        if (!rom_ok(e,L(0x84)+2u*k)) return true;
        uint16_t item=rw(L(0x84)+2u*k);
        neo_gactor_t *c=NULL;
        if (item<0x5au) {                                                /* 08CD64 -> 08CDFA (D0 = 2A) */
            c=e->spawn?e->spawn(e->ctx,NULL,0x8c374,true):NULL;
            if (c) {ga_sb(c,0x8b,0);ga_sw(c,0x70,0);ga_sw(c,0x6e,item);ga_sw(c,0x72,0);ga_sb(c,0x7f,0);ga_place_at(c,a,e,ox,oy);ga_sw(c,0x72,0);}
        }
        uint16_t r=ga_rand(e);
        if (c) {ga_sw(c,0x32,r);ga_sw(c,0x30,r&3u);}
        r=ga_rand(e);
        if (c && (r&1u)) ga_sl(c,0x30,0u-ga_l(c,0x30));
        r=ga_rand(e);
        if (c) ga_sw(c,0x34,(uint16_t)((r&3u)+5u));
    }
    neo_gactor_t *c=e->spawn?e->spawn(e->ctx,NULL,0x3378a,true):NULL;    /* 00411C, D0 = 2A */
    if (c) {ga_place_at(c,a,e,ox,oy);ga_sw(c,0x6e,W(0x6e));}
    return true;
}
static ga_result_t bin_tick(neo_gactor_t *a,neo_genv_t *e) {
    int r;
    switch (L(0)) {
    case 0x33526: {
        unsigned n=W(0x6e);
        if (n>2u || !rom_ok(e,0x334bau+2u*n) || !rom_ok(e,0x2d3ed8u+4u*n) || !rom_ok(e,0x2d3ebe)) return GA_UNSUPPORTED;
        ga_palette(a,rw(0x334bau+2u*n));
        if (!ga_anim_start(a,e,rl(0x3351au+4u*n),0x100)) return GA_UNSUPPORTED;
        SW(0x66,0x87c0);SW(0x1e,0x87c0);SL(0x50,rl(0x334ceu+4u*n));
        SW(0x4e,ga_rank_word(a,e,ga_rom(0x2d3ebe)));SB(0x88,0);SB(0x89,0);
        SL(0x80,rl(0x3350eu+4u*n));SL(0x84,rl(0x2d3ed8u+4u*n));
        ga_floor(a,e);SL(0x62,0);
        if ((W(0x66)&0xf000u)==0x8000u) SW(0x1e,W(0x66)-W(0x40));        /* 01AD8C */
        SW(0x2c,W(0x2c)-4u);SL(0,0x335c6);
    }   /* fall through */
    case 0x335c6: {
        ga_stationary(a,e);
        if (ga_register(a,e,NULL,NULL)) {SB(0x5a,0);SB(0x88,0xff);}
        bool on=bin_stood(a,e,0);
        if (!on) on=bin_stood(a,e,1);
        SB(0x89,on?0xff:0);
        if (ga_anim_step(a,e,bin_call)<0 || e->unsupported) return GA_UNSUPPORTED;
        if (ga_cull(a,e,0x334c6)) return GA_DELETE;
        if (!(B(0x5f)&0x80u)) {
            if (e->dyn_set) e->dyn_set(e->ctx,a,rl(0x333dcu+4u*W(0x6e)),0,0);   /* 0140C0 */
            return e->unsupported?GA_UNSUPPORTED:GA_MORE;
        }
        if (e->sound) e->sound(e->ctx,0x1025);
        for (unsigned i=0;i<16;i++) {                                    /* 01DBAA */
            neo_gactor_t *c=e->spawn_reserve?e->spawn_reserve(e->ctx,a,0x1d31c,0x2a):e->spawn?e->spawn(e->ctx,a,0x1d31c,false):NULL;
            if (c) ga_place_from(c,a,e,0,0x18);
        }
        if (e->dyn_release) e->dyn_release(e->ctx,a);                    /* 013F1E */
        neo_gactor_t *c=e->spawn?e->spawn(e->ctx,NULL,0x897e2,true):NULL;
        if (c) ga_place_at(c,a,e,0,0);
        c=e->spawn?e->spawn(e->ctx,NULL,0x33864,true):NULL;
        if (c) {ga_place_at(c,a,e,0x10,8);ga_sl(c,0x30,0x10000);ga_sl(c,0x34,0x34000);ga_sw(c,0x6e,0);}
        c=e->spawn?e->spawn(e->ctx,NULL,0x33864,true):NULL;
        if (c) {ga_place_at(c,a,e,-0x10,8);ga_sl(c,0x30,0xfffec000u);ga_sl(c,0x34,0x38000);ga_sw(c,0x6e,0);}
        return e->unsupported?GA_UNSUPPORTED:GA_DELETE;
    }
    case 0x3378a:
        if (W(0x6e)>2u || !rom_ok(e,0x334c0u+2u*W(0x6e)) || !ga_anim_start(a,e,0x239bca,0x100)) return GA_UNSUPPORTED;
        ga_palette(a,rw(0x334c0u+2u*W(0x6e)));SW(0x66,0xe0ed);SW(0x1e,0xe0ed);SL(0,0x337c4);
        /* fall through */
    case 0x337c4:
        ga_project(a,e);
        if ((r=ga_anim_step(a,e,NULL))<0 || e->unsupported) return GA_UNSUPPORTED;
        return r?GA_DELETE:GA_MORE;
    case 0x33864:
        if (W(0x6e)>3u || !rom_ok(e,0x500fcu+2u*W(0x6e)) || !rom_ok(e,0x4fdfeu+4u*W(0x6e))) return GA_UNSUPPORTED;
        ga_palette(a,rw(0x500fcu+2u*W(0x6e)));
        if (!ga_anim_start(a,e,rl(0x4fdfeu+4u*W(0x6e)),0x100)) return GA_UNSUPPORTED;
        SW(0x66,0xe0e8);SW(0x1e,0xe0e8);SW(0x8c,0x3c);ga_floor(a,e);SL(0,0x338b8);
        /* fall through */
    case 0x338b8:
        SW(0x8c,W(0x8c)-1u);
        if (!W(0x8c)) return GA_DELETE;
        SL(0x34,L(0x34)+0xffffb000u);ga_air(a,e);
        if (B(0x15)&0x40u) {
            uint32_t v=L(0x34);
            if (v&0x80000000u) v=0u-v;
            SL(0x34,(uint32_t)((int32_t)(v*3u)>>2));SB(0x5f,B(0x5f)|8u);
            if (!rom_ok(e,0x4fe16u+4u*W(0x6e)) || !ga_anim_start(a,e,rl(0x4fe16u+4u*W(0x6e)),0x100)) return GA_UNSUPPORTED;
        }
        if (ga_anim_step(a,e,NULL)<0 || e->unsupported) return GA_UNSUPPORTED;
        return GA_MORE;
    }
    ga_fail(e,L(0));return GA_UNSUPPORTED;
}
/* ---- 036C34 (script records of the street and the climb) ------------------------
 * 036C34: 0146E8, 036BE8 (palette 26C, layers 8838 or FFF2 with +6E,
 * hurtbox 036ACC, +4E = rank 2D3A0E), program 23BDD2. 036C4E/036C58: 01ABC8,
 * 01AD8C, the cell map 036AF8 (0140C4), 00427A; a hit -> 036CA0 (23BDDC,
 * back to 036C4E at its end); +5F bit 7 -> 036CF4: 23BE08 (sound 109F,
 * hurtbox 036AE0), +4E = rank 2D3A28. 036D24 / 036D76 / 036DD4 the same
 * with the map 036B16 and the flinch 23BDF2; +5F bit 7 -> 036E12: 23BEBE
 * (sound 1036, its call 036E84: the blast 036EA8 at (0, 0)), 013F1E;
 * 036E3A to its end, 036E5A / 036E6C: +50 = 0, one more update, deleted.
 * Every state before ends in 036B4A: 004188 outside 036AC4.
 * - 036EA8: palette 26C, 22CF2C, 1001; deleted at its end. */
static bool urn_call(neo_gactor_t *a,neo_genv_t *e,neo_anim_kind_t kind,uint32_t routine,uint16_t parameter) {
    (void)parameter;
    if (kind!=NEO_ANIM_CALL || routine!=0x36e84u) return false;
    neo_gactor_t *c=e->spawn_reserve?e->spawn_reserve(e->ctx,a,0x36ea8,0x1c):e->spawn?e->spawn(e->ctx,a,0x36ea8,false):NULL;
    if (c) ga_place_from(c,a,e,0,0);
    return true;
}
/* 01ABC8, 01AD8C, the cell map (0 none), 00427A: the program's result. */
static int urn_step(neo_gactor_t *a,neo_genv_t *e,uint32_t map) {
    ga_project(a,e);
    if ((W(0x66)&0xf000u)==0x8000u) SW(0x1e,W(0x66)-W(0x40));
    if (map && e->dyn_set) e->dyn_set(e->ctx,a,map,0,0);
    return ga_anim_step(a,e,urn_call);
}
static ga_result_t urn_tick(neo_gactor_t *a,neo_genv_t *e) {
    int r;
    switch (L(0)) {
    case 0x36c34:
        ga_floor(a,e);
        if (!rom_ok(e,0x2d3a0e)) return GA_UNSUPPORTED;
        ga_palette(a,0x26c);
        if (!W(0x6e)) {SW(0x66,0x8838);SW(0x1e,0x8838);} else {SW(0x66,0xfff2);SW(0x1e,0xfff2);}
        SL(0x50,0x36acc);SW(0x4e,ga_rank_word(a,e,ga_rom(0x2d3a0e)));
        if (!ga_anim_start(a,e,0x23bdd2,0x100)) return GA_UNSUPPORTED;
        /* fall through */
    case 0x36c4e:
        SB(0x5a,0);SL(0,0x36c58);
        /* fall through */
    case 0x36c58:
        if (urn_step(a,e,0x36af8)<0) return GA_UNSUPPORTED;
        if (ga_register(a,e,NULL,NULL)) {SB(0x5a,0);SL(0,0x36ca0);}
        if (B(0x5f)&0x80u) SL(0,0x36cf4);
        break;
    case 0x36ca0:
        if (!ga_anim_start(a,e,0x23bddc,0x100)) return GA_UNSUPPORTED;
        SL(0,0x36cb6);
        /* fall through */
    case 0x36cb6:
        if ((r=urn_step(a,e,0x36af8))<0) return GA_UNSUPPORTED;
        if (r) SL(0,0x36c4e);
        if (B(0x5f)&0x80u) SL(0,0x36cf4);
        break;
    case 0x36cf4:
        if (!rom_ok(e,0x2d3a28) || !ga_anim_start(a,e,0x23be08,0x100)) return GA_UNSUPPORTED;
        SB(0x5f,B(0x5f)&0x7fu);SB(0x5a,0);SW(0x4e,ga_rank_word(a,e,ga_rom(0x2d3a28)));SL(0,0x36d24);
        /* fall through */
    case 0x36d24:
        if ((r=urn_step(a,e,0x36b16))<0) return GA_UNSUPPORTED;
        if (r) SL(0,0x36d70);
        if (ga_register(a,e,NULL,NULL)) SB(0x5a,0);
        if (B(0x5f)&0x80u) SL(0,0x36e12);
        break;
    case 0x36d70:
        SL(0,0x36d76);
        /* fall through */
    case 0x36d76:
        if (urn_step(a,e,0x36b16)<0) return GA_UNSUPPORTED;
        if (ga_register(a,e,NULL,NULL)) {SB(0x5a,0);SL(0,0x36dbe);}
        if (B(0x5f)&0x80u) SL(0,0x36e12);
        break;
    case 0x36dbe:
        if (!ga_anim_start(a,e,0x23bdf2,0x100)) return GA_UNSUPPORTED;
        SL(0,0x36dd4);
        /* fall through */
    case 0x36dd4:
        if ((r=urn_step(a,e,0x36b16))<0) return GA_UNSUPPORTED;
        if (r) SL(0,0x36d70);
        if (B(0x5f)&0x80u) SL(0,0x36e12);
        break;
    case 0x36e12:
        SB(0x5f,B(0x5f)&0x7fu);ga_project(a,e);
        if (!ga_anim_start(a,e,0x23bebe,0x100)) return GA_UNSUPPORTED;
        if (e->dyn_release) e->dyn_release(e->ctx,a);                    /* 013F1E */
        SL(0,0x36e3a);
        /* fall through */
    case 0x36e3a:
        if ((r=urn_step(a,e,0))<0) return GA_UNSUPPORTED;
        if (r) SL(0,0x36e5a);
        break;
    case 0x36e5a:
        SL(0x50,0);SB(0x5a,0);SL(0,0x36e6c);
        /* fall through */
    case 0x36e6c:
        if (urn_step(a,e,0)<0 || e->unsupported) return GA_UNSUPPORTED;
        return GA_DELETE;
    case 0x36ea8:
        ga_palette(a,0x26c);
        if (!ga_anim_start(a,e,0x22cf2c,0x100)) return GA_UNSUPPORTED;
        SW(0x66,0x1001);SW(0x1e,0x1001);SL(0,0x36ed8);
        /* fall through */
    case 0x36ed8:
        ga_project(a,e);
        if ((r=ga_anim_step(a,e,NULL))<0 || e->unsupported) return GA_UNSUPPORTED;
        return r?GA_DELETE:GA_MORE;
    default:
        ga_fail(e,L(0));return GA_UNSUPPORTED;
    }
    /* 036B4A */
    if (e->unsupported) return GA_UNSUPPORTED;
    return ga_cull(a,e,0x36ac4)?GA_DELETE:GA_MORE;
}
static bool urn_routine(uint32_t r) {
    static const uint32_t k[]={0x36c34,0x36c4e,0x36c58,0x36ca0,0x36cb6,0x36cf4,0x36d24,0x36d70,0x36d76,0x36dbe,0x36dd4,0x36e12,0x36e3a,0x36e5a,0x36e6c,0x36ea8,0x36ed8};
    for (unsigned i=0;i<sizeof k/sizeof k[0];i++) if (r==k[i]) return true;
    return false;
}
/* ---- the van 0A9F0C of the street's end (script record 2DBADE, camera 1136) ------
 * 0A9F0C: palette B5, flash 318, hurtbox 0E65CC, 8848, +4E = +72 = rank
 * 2D1B20, 2A7E1A, +70 = 0 (stage), the overlay 0AA37C (D0 = 1C), the solid
 * body 0E6508 (004E76). 0A9F96: 01436A, +4E = +72 (no damage), 01AD8C,
 * 00427A, the cell map 0E665A; once 10E99E >= +6E: +7C = FF -> 0A9FD2.
 * 0A9FD2: +74 = 2D1B0C[+70] * +72 >> 8 (the health of the next stage), the
 * program 0E6836[+70] (none: no change) and +70 + 1; +5F bit 7 -> 0AA176.
 * 0AA024: 01436A, 004C24, 01AD8C, 00427A; at the program's end with +4E <=
 * +74: sound 102C, 0A9FD2 again, the smoke lists 0E6546 (1001) / 0E6558
 * (E0ED) by the bits of 0E653A[+70]; 004188 outside 0E682E; the cell map
 * 0E6704; +5F bit 7 or +7A (the call 2A89A6 of the last program) -> 0AA176:
 * four items 08CDB2 of the set 2D1AAC[rand * 8 >> 16] (FFFF: none) at
 * (+78, 50), +78 from -30 in steps of 10 by the bits of 0E6516[rand & 1F];
 * 0AA28E: the blast 0AA2C6 (D0 = 0E; without a node the row 0E65BC at
 * once); the debris 0E656A (01DA40) -> 0AA1FE: sound 102F, 019DFC, +70 = 9
 * and its program, the solid body removed; 0AA252: 01436A, 01AD8C, 00427A,
 * the cell map 0E65D8, outside 0E682E: 013F1E and 004188.
 * - 0AA2C6: ten updates of 01ABC8 and the row 0E65BC (0049E8), then deleted.
 * - 0AA37C (overlay): 01AD6E, palette B5, flash 318, layers 0, 2A7E10;
 *   0AA3C8 until the creator's +7C; 0AA3DE: +70 = the creator's, program
 *   0E6862[+70 - 1]; 0AA412 until the creator's +70 changes; every update:
 *   01ABC8, 01AD8C, 0044F6 (the creator's flash timer), 00427A, deleted
 *   with the creator. */
static bool van_call(neo_gactor_t *a,neo_genv_t *e,neo_anim_kind_t kind,uint32_t routine,uint16_t parameter) {
    (void)e;(void)parameter;
    if (kind!=NEO_ANIM_CALL || routine!=0x2a89a6u) return false;
    SB(0x7a,0xff);return true;
}
static void van_depth(neo_gactor_t *a) {if ((W(0x66)&0xf000u)==0x8000u) SW(0x1e,W(0x66)-W(0x40));}   /* 01AD8C */
static ga_result_t van_tick(neo_gactor_t *a,neo_genv_t *e) {
    int r;neo_gactor_t *p;
    switch (L(0)) {
    case 0xa9f0c:
        if (!rom_ok(e,0x2d1b20)) return GA_UNSUPPORTED;
        ga_palette(a,0xb5);ga_flash_palette(a,0x318);SL(0x50,0xe65cc);SW(0x66,0x8848);SW(0x1e,0x8848);
        SW(0x4e,ga_rank_word(a,e,ga_rom(0x2d1b20)));
        if (!ga_anim_start(a,e,0x2a7e1a,0x100)) return GA_UNSUPPORTED;
        SW(0x70,0);SW(0x72,W(0x4e));SB(0x7a,0);SB(0x7c,0);ga_floor(a,e);SL(0x62,0);
        if (e->spawn_reserve) e->spawn_reserve(e->ctx,a,0xaa37c,0x1c); else if (e->spawn) e->spawn(e->ctx,a,0xaa37c,false);
        if (e->solid) e->solid(e->ctx,a,0xe6508);                        /* 004E76 */
        SL(0,0xa9f96);
        /* fall through */
    case 0xa9f96:
        ga_stationary(a,e);SW(0x4e,W(0x72));van_depth(a);
        if (ga_anim_step(a,e,van_call)<0) return GA_UNSUPPORTED;
        if ((int16_t)e->camera>=(int16_t)W(0x6e)) {SB(0x7c,0xff);SL(0,0xa9fd2);}
        if (e->dyn_set) e->dyn_set(e->ctx,a,0xe665a,0,0);
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    case 0xa9fd2: {
        if (W(0x70)>10u || !rom_ok(e,0x2d1b0cu+2u*W(0x70)) || !rom_ok(e,0xe6836u+4u*W(0x70))) return GA_UNSUPPORTED;
        SW(0x74,(uint16_t)(((uint32_t)rw(0x2d1b0cu+2u*W(0x70))*W(0x72))>>8));
        uint32_t prog=rl(0xe6836u+4u*W(0x70));
        if (prog) {
            if (!ga_anim_start(a,e,prog,0x100)) return GA_UNSUPPORTED;
            SW(0x70,W(0x70)+1u);
        }
        if (B(0x5f)&0x80u) goto wreck;
        SL(0,0xaa024);
    }   /* fall through */
    case 0xaa024:
        ga_stationary(a,e);ga_register(a,e,NULL,NULL);SB(0x5a,0);van_depth(a);
        if ((r=ga_anim_step(a,e,van_call))<0) return GA_UNSUPPORTED;
        if (r && (int16_t)W(0x4e)<=(int16_t)W(0x74)) {
            if (e->sound) e->sound(e->ctx,0x102c);
            SL(0,0xa9fd2);
            if (W(0x70)>11u || !rom_ok(e,0xe653au+W(0x70))) return GA_UNSUPPORTED;
            uint8_t bits=ga_rom(0xe653au+W(0x70))[0];
            if (bits&1u) emitter_at(a,e,0xe6546,0x1001);
            if (bits&2u) emitter_at(a,e,0xe6558,0xe0ed);
        }
        if (ga_cull(a,e,0xe682e)) return GA_DELETE;
        if (e->dyn_set) e->dyn_set(e->ctx,a,0xe6704,0,0);
        if (!(B(0x5f)&0x80u) && !B(0x7a)) return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    wreck: {                                                             /* 0AA176 */
        SW(0x78,0xffd0);
        if (!rom_ok(e,0xe6516)) return GA_UNSUPPORTED;
        SB(0x7b,ga_rom(0xe6516u+(ga_rand(e)&0x1fu))[0]);
        uint32_t set=0x2d1aacu+4u*ga_rand_range(e,8);
        if (!rom_ok(e,set)) return GA_UNSUPPORTED;
        set=rl(set);
        for (unsigned i=0;i<4;i++,set+=2u) {
            if (!rom_ok(e,set)) return GA_UNSUPPORTED;
            uint16_t kind=rw(set);
            if (kind&0x8000u) continue;
            uint8_t d6=B(0x7b),carry;
            do {SW(0x78,W(0x78)+0x10u);carry=d6&1u;d6>>=1;} while (!carry);
            SB(0x7b,d6);
            if (kind<0x5au) {                                            /* 08CDB2 */
                neo_gactor_t *c=e->spawn?e->spawn(e->ctx,NULL,0x8c374,true):NULL;
                if (c) {ga_sb(c,0x8b,0);ga_sw(c,0x70,0);ga_sw(c,0x6e,kind);ga_sw(c,0x72,0);ga_sb(c,0x7f,0);ga_place_at(c,a,e,(int16_t)W(0x78),0x50);}
            }
        }
        neo_gactor_t *c=e->spawn_reserve?e->spawn_reserve(e->ctx,a,0xaa2c6,0x0e):e->spawn?e->spawn(e->ctx,a,0xaa2c6,false):NULL;   /* 0AA28E */
        if (c) ga_place_from(c,a,e,0,0); else ga_attack(a,e,0xe65bc);
        emitter_at(a,e,0xe656a,0);
        SL(0,0xaa1fe);
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    }
    case 0xaa1fe: {
        if (e->sound) e->sound(e->ctx,0x102f);
        SW(0x66,0x8848);SW(0x1e,0x8848);SB(0x69,0xff);                /* 019DFC: +69 = FF only */
        SW(0x70,9);
        uint32_t prog=rl(0xe6836u+0x24u);
        if (prog) {
            if (!ga_anim_start(a,e,prog,0x100)) return GA_UNSUPPORTED;
            SW(0x70,10);
        }
        if (e->solid) e->solid(e->ctx,a,0);
        SL(0,0xaa252);
    }   /* fall through */
    case 0xaa252:
        ga_stationary(a,e);van_depth(a);
        if (ga_anim_step(a,e,van_call)<0) return GA_UNSUPPORTED;
        if (e->dyn_set) e->dyn_set(e->ctx,a,0xe65d8,0,0);
        if (ga_cull(a,e,0xe682e)) {if (e->dyn_release) e->dyn_release(e->ctx,a);return GA_DELETE;}
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    case 0xaa2c6:
        SW(0x76,0xa);SL(0,0xaa2d2);
        /* fall through */
    case 0xaa2d2:
        SW(0x76,W(0x76)-1u);
        if ((int16_t)W(0x76)<0) return GA_DELETE;
        ga_project(a,e);ga_attack(a,e,0xe65bc);
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    case 0xaa37c:
        p=e->creator?e->creator(e->ctx,a):NULL;
        ga_place(a,e,0,0,p?NEO_GA_TAG(p->index):0);                     /* 01AD6E */
        ga_palette(a,0xb5);ga_flash_palette(a,0x318);SW(0x66,0);SW(0x1e,0);
        if (!ga_anim_start(a,e,0x2a7e10,0x100)) return GA_UNSUPPORTED;
        SW(0x70,0);SL(0x62,0);SL(0,0xaa3c8);
        /* fall through */
    case 0xaa3c8:
        p=e->creator?e->creator(e->ctx,a):NULL;
        if (!p) {ga_fail(e,0xaa3c8);return GA_UNSUPPORTED;}
        if (ga_b(p,0x7c)) SL(0,0xaa3de);
        goto overlay;
    case 0xaa3de: {
        p=e->creator?e->creator(e->ctx,a):NULL;
        if (!p) {ga_fail(e,0xaa3de);return GA_UNSUPPORTED;}
        uint16_t d7=ga_w(p,0x70);SW(0x70,d7);
        uint32_t t=0xe6862u+4u*(uint16_t)(d7-1u);
        if (d7<1u || d7>10u || !rom_ok(e,t)) return GA_UNSUPPORTED;
        if (rl(t) && !ga_anim_start(a,e,rl(t),0x100)) return GA_UNSUPPORTED;
        SL(0,0xaa412);
    }   /* fall through */
    case 0xaa412:
        p=e->creator?e->creator(e->ctx,a):NULL;
        if (!p) {ga_fail(e,0xaa412);return GA_UNSUPPORTED;}
        if (ga_w(p,0x70)!=W(0x70)) SL(0,0xaa3de);
    overlay:
        ga_project(a,e);van_depth(a);
        {uint8_t d0=ga_b(p,0x17);if (d0) {if (!(d0&0x80u)) d0++;SB(0x17,d0);}}   /* 0044F6 */
        if (ga_anim_step(a,e,NULL)<0 || e->unsupported) return GA_UNSUPPORTED;
        return ga_creator_alive(a,e)?GA_MORE:GA_DELETE;
    }
    ga_fail(e,L(0));return GA_UNSUPPORTED;
}
static bool van_routine(uint32_t r) {
    static const uint32_t k[]={0xa9f0c,0xa9f96,0xa9fd2,0xaa024,0xaa1fe,0xaa252,0xaa2c6,0xaa2d2,0xaa37c,0xaa3c8,0xaa3de,0xaa412};
    for (unsigned i=0;i<sizeof k/sizeof k[0];i++) if (r==k[i]) return true;
    return false;
}
/* ---- the hatches 01F828 / 01F866 and their rocks 01FABA (family m4rock) ----
 * Entries 01F828..01F88E set +8C: bit 0 the small rock 01FABA (clear: the
 * large 01FBAC), bit 1 bursts of +76 rocks every +78 updates after +70
 * (clear: one every +70), bit 2 no hatch (and the splashes 02047A). 01F8A8:
 * 0146E8, the two halves of the hatch 01F9B8 / 01FA08 at the emitter
 * (01ACF2), +74 = 0: bit 7 (no rocks). 01F918: the stamp 0DFFC2 (0C88B0).
 * 01F934: 01ABC8, 01AD8C, 0203DE (01945C with 0DFF0A: +60 = FF, 004188),
 * then the timers and 0203FC: the rock after the emitter with its +6E, +72
 * and +8C, kept in +82, at the emitter (01ACF2); +74 rocks at most (FFFF:
 * no limit).
 * Hatch halves 01FA52: 020542 picks the program by the distance to the last
 * rock (creator's +82; thresholds 0DFE02, programs 0DFE1A / 0DFE2A), deleted
 * with the emitter (+60, 0041AC); the half with +8C bit 3 puffs 020004 when
 * its state reaches 3. Rock 01FABA: 0DFFDE, y - 0x40, vy = +6E << 8, health
 * by rank (2D34E2.. by the bits 1 and 2), row 0E000E, its shadow 01FF82;
 * 01FC9E: 01AC38, the sound 1053 once, behind the hatch until 0x20 px under
 * the emitter (+15 bit 2), dynamic map 0DFF1A / 0DFF30, 0049E8 and 004C24
 * (-> 01FECE), y >= 0x200 -> 01FDA4: with +72 set the rock and its shadow
 * leave (0041AC). The bounce 01FDE6.. and the burst 01FECE are not in the
 * capture: explicit stops. */
static bool rock_splash(neo_gactor_t *a,neo_genv_t *e,bool up) {   /* 02047A / 0204DE */
    if (!(B(0x8c)&4u)) return true;
    if (W(0x88)) {SW(0x88,W(0x88)-1u);return true;}
    for (int16_t d7=(B(0x8c)&1u)?0x40:0x80;d7>=0;d7=(int16_t)(d7-0x10)) {
        uint32_t keep=L(0x40);
        SW(0x40,up?W(0x40)-(uint16_t)d7:W(0x40)+(uint16_t)d7);
        if (neo_gate_splash(a,e,1)) SW(0x88,0x14);                       /* 03C5CC */
        SL(0x40,keep);
        if (e->unsupported) return false;
        if (W(0x88)) break;
    }
    return true;
}
static bool rock_emit(neo_gactor_t *a,neo_genv_t *e) {                  /* 0203FC */
    uint32_t r=(B(0x8c)&1u)?0x1fabau:0x1fbacu;
    neo_gactor_t *c=e->spawn_reserve?e->spawn_reserve(e->ctx,a,r,0x1c):e->spawn?e->spawn(e->ctx,a,r,false):NULL;
    if (!c) {ga_fail(e,0x203fc);return false;}
    ga_sw(c,0x6e,W(0x6e));ga_sw(c,0x72,W(0x72));ga_sb(c,0x8c,B(0x8c));SL(0x82,NEO_GA_TAG(c->index));
    ga_place_at(c,a,e,0,0);
    if (W(0x74)) {
        SW(0x7a,W(0x7a)+1u);
        if (W(0x74)!=0xffffu && (int16_t)W(0x7a)>=(int16_t)W(0x74)) {SW(0x7a,W(0x74));SB(0x8c,B(0x8c)|0x80u);}
    }
    return true;
}
static bool hatch_state(neo_gactor_t *a,neo_genv_t *e,neo_gactor_t *p) { /* 020542 */
    uint32_t tag=ga_l(p,0x82);
    if (!tag) {
        SB(0x60,0);SW(0x86,0x7fff);
        return ga_anim_start(a,e,(B(0x8c)&8u)?0x22fbea:0x22fbf6,0x100);
    }
    neo_gactor_t *q=e->actor?e->actor(e->ctx,tag):NULL;
    if (!q || !rom_ok(e,0xdfe02)) {ga_fail(e,0x2058e);return false;}
    int16_t d1=(int16_t)(ga_w(q,0x2c)-W(0x2c));SW(0x86,d1);
    for (unsigned d2=0;d2<0x18u;d2+=4u) {
        if (d1>(int16_t)rw(0xdfe02u+d2)) continue;
        uint16_t d0=rw(0xdfe02u+d2+2u);
        if ((uint8_t)d0==B(0x60)) return true;
        SB(0x60,d0);
        return ga_anim_start(a,e,rl(((B(0x8c)&8u)?0xdfe1au:0xdfe2au)+4u*d0),0x100);
    }
    return true;
}
static ga_result_t rock_tick(neo_gactor_t *a,neo_genv_t *e) {
    neo_gactor_t *p,*c;
    switch (L(0)) {
    case 0x1f828: case 0x1f836: case 0x1f844: case 0x1f858: case 0x1f866: case 0x1f87a: case 0x1f88e: {
        static const uint8_t bits[7]={1,4,5,2,3,6,7};
        static const uint32_t entry[7]={0x1f828,0x1f836,0x1f844,0x1f858,0x1f866,0x1f87a,0x1f88e};
        for (unsigned i=0;i<7;i++) if (L(0)==entry[i]) SB(0x8c,bits[i]);
        if (!(B(0x8c)&4u)) {
            ga_floor(a,e);                                                   /* 0146E8 */
            c=e->spawn_reserve?e->spawn_reserve(e->ctx,a,0x1f9b8,0x1c):e->spawn?e->spawn(e->ctx,a,0x1f9b8,false):NULL;
            if (!c) {ga_fail(e,0x1f8c2);return GA_UNSUPPORTED;}
            ga_place_at(c,a,e,0,0);
            c=e->spawn_reserve?e->spawn_reserve(e->ctx,a,0x1fa08,0x1c):e->spawn?e->spawn(e->ctx,a,0x1fa08,false):NULL;
            if (!c) {ga_fail(e,0x1f8dc);return GA_UNSUPPORTED;}
            ga_place_at(c,a,e,0,0);
        }
        if (!W(0x74)) SB(0x8c,B(0x8c)|0x80u);
        SB(0x60,0);SW(0x7c,0);SW(0x7a,0);SW(0x80,0);SW(0x7e,0);SL(0,0x1f918);
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    }
    case 0x1f918:
        if (!(B(0x8c)&4u) && e->stamp) e->stamp(e->ctx,a,0xdffc2);          /* 0C88B0 */
        SL(0,0x1f934);
        /* fall through */
    case 0x1f934:
        ga_project(a,e);van_depth(a);
        if (ga_cull(a,e,0xdff0a)) {SB(0x60,0xff);return GA_DELETE;}         /* 0203DE: 004188 */
        if (e->unsupported) return GA_UNSUPPORTED;
        if (B(0x8c)&0x80u) return GA_MORE;
        if (!(B(0x8c)&2u)) {
            SW(0x7c,W(0x7c)-1u);
            if ((int16_t)W(0x7c)<0) {SW(0x7c,W(0x70));if (!rock_emit(a,e)) return GA_UNSUPPORTED;}
        } else if (W(0x7c)) SW(0x7c,W(0x7c)-1u);
        else if (W(0x80)) SW(0x80,W(0x80)-1u);
        else {
            if (!rock_emit(a,e)) return GA_UNSUPPORTED;
            SW(0x7e,W(0x7e)+1u);
            if ((int16_t)W(0x76)>(int16_t)W(0x7e)) SW(0x80,W(0x78));
            else {SW(0x7e,0);SW(0x7c,W(0x70));}
        }
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    case 0x1f9b8:
        SB(0x1d,0);SW(0x66,0xfff1);SW(0x1e,0xfff1);ga_palette(a,0x76);
        if (!ga_anim_start(a,e,0x22fbea,0x100)) return GA_UNSUPPORTED;
        SB(0x60,0);SB(0x8a,0);SB(0x8c,8);SW(0x86,0x7fff);SL(0,0x1fa52);
        return GA_MORE;
    case 0x1fa08:
        SB(0x1d,0);SW(0x66,0x8800);SW(0x1e,0x8800);ga_palette(a,0x76);
        if (!ga_anim_start(a,e,0x22fbf6,0x100)) return GA_UNSUPPORTED;
        SB(0x60,0);SB(0x8a,0);SB(0x8c,0);SB(0x15,B(0x15)|4u);SL(0,0x1fa52);
        return GA_MORE;
    case 0x1fa52:
        ga_project(a,e);van_depth(a);
        if (ga_anim_step(a,e,NULL)<0) return GA_UNSUPPORTED;
        p=e->creator?e->creator(e->ctx,a):NULL;
        if (!p) {ga_fail(e,0x1fa68);return GA_UNSUPPORTED;}
        if (!hatch_state(a,e,p) || e->unsupported) return GA_UNSUPPORTED;
        if (ga_b(p,0x60)) return GA_DELETE_TREE;                             /* 0041AC */
        if (B(0x8c)&8u) {
            if (B(0x60)==3u && B(0x8a)!=3u && !(B(0x8c)&0x10u)) {
                if (e->spawn_reserve) e->spawn_reserve(e->ctx,a,0x20004,0x1c); else if (e->spawn) e->spawn(e->ctx,a,0x20004,false);
            }
            SB(0x8a,B(0x60));
        }
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    case 0x1faba: {
        SB(0x1d,0);SW(0x66,0x8801);SW(0x1e,0x8801);ga_palette(a,0x74);
        if (!ga_anim_start(a,e,0x22fb34,0x100)) return GA_UNSUPPORTED;
        SL(0x50,0xdffde);SW(0x2c,W(0x2c)-0x40u);SL(0x30,0);SL(0x34,(uint32_t)W(0x6e)<<8);
        uint32_t t=(B(0x8c)&2u)?((B(0x8c)&4u)?0x2d3672u:0x2d360eu):((B(0x8c)&4u)?0x2d3546u:0x2d34e2u);
        if (!rom_ok(e,t)) return GA_UNSUPPORTED;
        SW(0x4e,ga_rank_word(a,e,ga_rom(t)));
        if (!(B(0x8c)&4u)) SB(0x15,B(0x15)|4u);
        SL(0x82,0xe000e);SB(0x5e,B(0x5e)|0x80u);                             /* 004C1C */
        if (e->spawn_reserve) e->spawn_reserve(e->ctx,a,0x1ff82,0x1c); else if (e->spawn) e->spawn(e->ctx,a,0x1ff82,false);
        SL(0,0x1fc9e);
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    }
    case 0x1fc9e:
        ga_move(a,e);                                                        /* 01AC38 */
        if ((int16_t)W(0x40)>=((B(0x8c)&1u)?0x120:0xf0) && (int16_t)W(0x3c)>0 && !B(0x8b)) {
            if (e->sound) e->sound(e->ctx,0x1053);
            SB(0x8b,0xff);
        }
        if ((B(0x8c)&1u) && (B(0x15)&4u)) {
            p=e->creator?e->creator(e->ctx,a):NULL;
            if (!p) {ga_fail(e,0x1fce4);return GA_UNSUPPORTED;}
            if ((int16_t)(ga_w(p,0x40)+0x20u)<=(int16_t)W(0x40)) SB(0x15,B(0x15)&0xfbu);
        }
        if (!rock_splash(a,e,false)) return GA_UNSUPPORTED;
        if ((int16_t)W(0x40)>=0x200) SL(0,0x1fda4);
        if (ga_anim_step(a,e,NULL)<0) return GA_UNSUPPORTED;
        if (e->dyn_set) e->dyn_set(e->ctx,a,(B(0x8c)&1u)?0xdff1a:0xdff30,0,0);   /* 0140C0 */
        if (ga_attack(a,e,L(0x82))) SL(0,0x1fece);
        if (ga_register(a,e,NULL,NULL)) {
            if (B(0x5f)&0x80u) SL(0,0x1fece); else SB(0x5a,0);
        }
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    case 0x1fda4:
        SW(0x66,0xe0ef);SW(0x1e,0xe0ef);SB(0x15,(B(0x15)&0xbfu)|0x20u);
        if (e->dyn_release) e->dyn_release(e->ctx,a);                        /* 013F1E */
        p=e->creator?e->creator(e->ctx,a):NULL;
        if (!p) {ga_fail(e,0x1fdc2);return GA_UNSUPPORTED;}
        if (ga_l(p,0x82)==NEO_GA_TAG(a->index)) ga_sl(p,0x82,0);
        if (W(0x72)) return GA_DELETE_TREE;                                  /* 0041AC */
        ga_fail(e,0x1fde6);return GA_UNSUPPORTED;                            /* the bounce: not captured */
    case 0x1ff82:
        SB(0x1d,0);SW(0x66,0x8801);SW(0x1e,0x8801);ga_palette(a,0x75);
        if (!ga_anim_start(a,e,0x22fbbc,0x100)) return GA_UNSUPPORTED;
        p=e->creator?e->creator(e->ctx,a):NULL;
        ga_place(a,e,0,0,p?NEO_GA_TAG(p->index):0);                          /* 01AD6E */
        SL(0,0x1ffc8);
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    case 0x1ffc8:
        p=e->creator?e->creator(e->ctx,a):NULL;
        if (!p) {ga_fail(e,0x1ffc8);return GA_UNSUPPORTED;}
        SB(0x15,(B(0x15)&0xfbu)|(ga_b(p,0x15)&4u));
        if (ga_b(p,0x15)&2u) SB(0x15,B(0x15)|2u);
        ga_project(a,e);
        if (ga_anim_step(a,e,NULL)<0) return GA_UNSUPPORTED;
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    case 0x20004:
        SB(0x1d,0);SW(0x66,0x8820);SW(0x1e,0x8820);ga_palette(a,0x77);
        if (!ga_anim_start(a,e,0x22fcda,0x100)) return GA_UNSUPPORTED;
        p=e->creator?e->creator(e->ctx,a):NULL;
        if (!p) {ga_fail(e,0x20042);return GA_UNSUPPORTED;}
        ga_place(a,e,0,0,NEO_GA_TAG(p->index));                              /* 01AD6E */
        ga_sb(p,0x8c,ga_b(p,0x8c)|0x10u);SL(0,0x20054);
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    case 0x20054:
        ga_project(a,e);van_depth(a);
        if (ga_anim_step(a,e,NULL)<0) return GA_UNSUPPORTED;
        p=e->creator?e->creator(e->ctx,a):NULL;
        if (!p) {ga_fail(e,0x20066);return GA_UNSUPPORTED;}
        if ((int16_t)ga_w(p,0x86)<0x40) return e->unsupported?GA_UNSUPPORTED:GA_MORE;
        ga_sb(p,0x8c,ga_b(p,0x8c)&0xefu);
        return GA_DELETE;                                                    /* 004188 */
    }
    ga_fail(e,L(0));return GA_UNSUPPORTED;
}
static bool rock_routine(uint32_t r) {
    static const uint32_t k[]={0x1f828,0x1f836,0x1f844,0x1f858,0x1f866,0x1f87a,0x1f88e,0x1f918,0x1f934,0x1f9b8,0x1fa08,0x1fa52,
        0x1faba,0x1fc9e,0x1fda4,0x1ff82,0x1ffc8,0x20004,0x20054};
    for (unsigned i=0;i<sizeof k/sizeof k[0];i++) if (r==k[i]) return true;
    return false;
}
/* ---- the jumping soldier 07E554 (family m4jump) ----
 * 07E554 (script record, 01A422): palette 379, layers 8820, 01B1DC, +4E = 1,
 * program 27F832; 07E590 each update: 01ABC8, 01AD8C, 00427A, faces the
 * nearest player (07EF24: 01AF9A, +15 bit 0 when the player is to its
 * right) and, with that player within +72 px (07EF48; +72 = 0: never),
 * keeps the player's node in +74 and goes to 07E658: program 27F884, +61 =
 * 0, +86 = +6E; 07E678: when the program ends (+61 = FF) it waits +86
 * updates, then 07E6CA: the jump (+30 = 0, +34 = A0000, +78 = 0, +7C =
 * -8000 + 800 * (+70 & 7)), program 27F8AE, the dust 07EDF0 at (0, 8);
 * 07E738: 0143E4, +30 += +78, +34 += +7C, 00427A; +34 < 0 -> 07E832 (the
 * fall: not in the capture, explicit stop), a hit (004C24) -> the piece
 * 07EC6A and 07EA6C: sound 103E, 01B1DC, program 27F932; 07EA92: 0143DC,
 * on the ground (+15 bit 6) -> 07EA1E: the dust 07EDF0, +84 = 1; 07EA48:
 * 01AC38, 00427A, deleted on the second update. Every state ends in 004188
 * outside 0E4838 (01945C).
 * 07EDF0 (the dust): layers 8820, program 22DF54, the splash palette of
 * 019240 (D1), 01DEA2, +15 bit 1 clear; 07EE2E: 01ABC8, 01AD8C, 00427A,
 * deleted when the program ends or outside 0E4848 (019504).
 * 07EC6A (the piece): palette 379, program 27FA3A, +15 bit 5, 01B1DC, +34 =
 * 70000, +7C = -A000, +30 = 2 * rand + 20000, negative when the creator's
 * +5B bit 7 is set; 07ECD2: 0143E4, +34 += +7C; on the ground: in water
 * (03C55C) the dust 07EDF0 at the surface (01AB9C), else the blast 01C706
 * (+66 = 1001, 01B220), then 004188; else 00427A and 004188 outside 0E4840. */
static neo_gactor_t *jump_kid(neo_gactor_t *a,neo_genv_t *e,uint32_t r) {   /* 0040BC, D0 = 1C */
    neo_gactor_t *c=e->spawn_reserve?e->spawn_reserve(e->ctx,a,r,0x1c):e->spawn?e->spawn(e->ctx,a,r,false):NULL;
    if (!c) ga_fail(e,r);
    return c;
}
/* 01AF9A: the nearest live player's node and +3C; carry (true) when none (player 1). */
static bool jump_nearest(neo_gactor_t *a,neo_genv_t *e,uint32_t *node,uint16_t *x) {
    unsigned m=e->players.active_mask&0xffu;
    *node=0x1014c4u;*x=e->players.p1x;
    if (!m) return true;
    if (m==1u) return false;
    if (m==2u) {*node=0x101574u;*x=e->players.p2x;return false;}
    int16_t d1=(int16_t)(e->players.p1x-W(0x3c)),d2=(int16_t)(e->players.p2x-W(0x3c));
    if (d1<0) d1=(int16_t)-d1;
    if (d2<0) d2=(int16_t)-d2;
    if (d2<d1) {*node=0x101574u;*x=e->players.p2x;}
    return false;
}
static void jump_face(neo_gactor_t *a,neo_genv_t *e) {   /* 07EF24 */
    uint32_t n;uint16_t x;jump_nearest(a,e,&n,&x);
    if ((int16_t)(W(0x3c)-x)<0) SB(0x15,B(0x15)|1u); else SB(0x15,B(0x15)&0xfeu);
}
static bool jump_near(neo_gactor_t *a,neo_genv_t *e,uint32_t *node) {   /* 07EF48: carry */
    uint16_t x;
    if (jump_nearest(a,e,node,&x)) return false;
    int16_t d0=(int16_t)(W(0x3c)-x);
    if (d0<0) d0=(int16_t)-d0;
    int16_t d1=(int16_t)W(0x72);
    return d1 && d1>=d0;
}
static ga_result_t jump_cull(neo_gactor_t *a,neo_genv_t *e) {
    if (ga_cull(a,e,0xe4838)) return GA_DELETE;   /* 01945C, 004188 */
    return e->unsupported?GA_UNSUPPORTED:GA_MORE;
}
static ga_result_t jump_tick(neo_gactor_t *a,neo_genv_t *e) {
    neo_gactor_t *c,*p;int r;uint32_t n;
    switch (L(0)) {
    case 0x7e554:
        ga_palette(a,0x379);SW(0x66,0x8820);SW(0x1e,0x8820);
        SL(0x30,0);SL(0x34,0);SL(0x62,0);SW(0x4e,1);                          /* 01B1DC */
        if (!ga_anim_start(a,e,0x27f832,0x100)) return GA_UNSUPPORTED;
        SL(0,0x7e590);
        /* fall through */
    case 0x7e590:
        ga_project(a,e);van_depth(a);
        if (ga_anim_step(a,e,NULL)<0) return GA_UNSUPPORTED;
        jump_face(a,e);
        if (jump_near(a,e,&n)) {SL(0x74,n);SL(0,0x7e658);}
        return jump_cull(a,e);
    case 0x7e658:
        if (!ga_anim_start(a,e,0x27f884,0x100)) return GA_UNSUPPORTED;
        SB(0x61,0);SW(0x86,W(0x6e));SL(0,0x7e678);
        /* fall through */
    case 0x7e678:
        ga_project(a,e);van_depth(a);
        r=ga_anim_step(a,e,NULL);
        if (r<0) return GA_UNSUPPORTED;
        if (r) SB(0x61,0xff);
        if (B(0x61)) {
            if (!W(0x86)) SL(0,0x7e6ca); else SW(0x86,W(0x86)-1u);
        }
        return jump_cull(a,e);
    case 0x7e6ca: {
        SL(0x30,0);SL(0x34,0xa0000);SL(0x78,0);
        uint32_t vy=0xffff8000u;
        for (uint16_t d0=W(0x70)&7u;d0;d0--) vy+=0x800u;
        SL(0x7c,vy);
        if (!ga_anim_start(a,e,0x27f8ae,0x100)) return GA_UNSUPPORTED;
        if (!(c=jump_kid(a,e,0x7edf0))) return GA_UNSUPPORTED;
        ga_place_at(c,a,e,0,8);
        SL(0,0x7e738);
    }
        /* fall through */
    case 0x7e738:
        ga_air(a,e);SL(0x30,L(0x30)+L(0x78));SL(0x34,L(0x34)+L(0x7c));
        if (ga_anim_step(a,e,NULL)<0) return GA_UNSUPPORTED;
        if ((int32_t)L(0x34)<0) SL(0,0x7e832);
        if (ga_register(a,e,NULL,NULL)) {
            if (!(c=jump_kid(a,e,0x7ec6a))) return GA_UNSUPPORTED;
            ga_place_at(c,a,e,0,0);
            SL(0,0x7ea6c);
        }
        return jump_cull(a,e);
    case 0x7ea6c:
        if (e->sound) e->sound(e->ctx,0x103e);
        SL(0x30,0);SL(0x34,0);SL(0x62,0);                                     /* 01B1DC */
        if (!ga_anim_start(a,e,0x27f932,0x100)) return GA_UNSUPPORTED;
        SL(0,0x7ea92);
        /* fall through */
    case 0x7ea92:
        SL(0x34,L(0x34)+0xffffb000u);ga_air(a,e);                             /* 0143DC */
        if (B(0x15)&0x40u) SL(0,0x7ea1e);
        if (ga_anim_step(a,e,NULL)<0) return GA_UNSUPPORTED;
        return jump_cull(a,e);
    case 0x7ea1e:
        if (!(c=jump_kid(a,e,0x7edf0))) return GA_UNSUPPORTED;
        ga_place_at(c,a,e,0,0);
        SW(0x84,1);SL(0,0x7ea48);
        /* fall through */
    case 0x7ea48:
        ga_move(a,e);
        if (ga_anim_step(a,e,NULL)<0) return GA_UNSUPPORTED;
        if (!W(0x84)) return GA_DELETE;
        SW(0x84,W(0x84)-1u);
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    case 0x7edf0: {
        SW(0x66,0x8820);SW(0x1e,0x8820);
        if (!ga_anim_start(a,e,0x22df54,0x100)) return GA_UNSUPPORTED;
        uint16_t pal=neo_gate_splash_palette(e);                              /* 019240 D1 */
        if (!pal) {ga_fail(e,0x19240);return GA_UNSUPPORTED;}
        ga_palette(a,pal);
        if (!(ga_rand(e)&2u)) SB(0x15,B(0x15)^1u);                            /* 01DEA2 */
        SB(0x15,B(0x15)&0xfdu);SL(0,0x7ee2e);
    }
        /* fall through */
    case 0x7ee2e:
        ga_project(a,e);van_depth(a);
        r=ga_anim_step(a,e,NULL);
        if (r<0) return GA_UNSUPPORTED;
        if (r) return GA_DELETE;
        if (ga_cull_rect(a,e,0xe4848)) return GA_DELETE;                      /* 019504 */
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    case 0x7ec6a: {
        ga_palette(a,0x379);
        if (!ga_anim_start(a,e,0x27fa3a,0x100)) return GA_UNSUPPORTED;
        SB(0x15,B(0x15)|0x20u);SL(0x30,0);SL(0x34,0);SL(0x62,0);              /* 01B1DC */
        SL(0x34,0x70000);SL(0x7c,0xffff6000u);
        uint32_t d0=((uint32_t)ga_rand(e)&0xffffu)*2u+0x20000u;
        p=e->creator?e->creator(e->ctx,a):NULL;
        if (!p) {ga_fail(e,0x7ecb8);return GA_UNSUPPORTED;}
        if (ga_b(p,0x5b)&0x80u) d0=0u-d0;
        SL(0x30,d0);SL(0,0x7ecd2);
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    }
    case 0x7ecd2:
        ga_air(a,e);SL(0x34,L(0x34)+L(0x7c));
        if (B(0x15)&0x40u) {
            int32_t x,y;uint32_t t;
            if (neo_gate_surface(a,e,&x,&y,&t)) {                             /* 03C55C */
                if (!(c=jump_kid(a,e,0x7edf0))) return GA_UNSUPPORTED;
                ga_sl(c,0x28,(uint32_t)x&0xffff0000u);ga_sl(c,0x2c,(uint32_t)y&0xffff0000u);ga_sl(c,0x38,t);ga_sw(c,0x4c,0);
                ga_project(c,e);                                              /* 01AB9C */
            } else {
                if (!(c=jump_kid(a,e,0x1c706))) return GA_UNSUPPORTED;
                ga_sw(c,0x66,0x1001);ga_sw(c,0x1e,0x1001);
                ga_place_from(c,a,e,0,0);                                     /* 01B220 */
            }
            return e->unsupported?GA_UNSUPPORTED:GA_DELETE;
        }
        if (ga_anim_step(a,e,NULL)<0) return GA_UNSUPPORTED;
        if (ga_cull(a,e,0xe4840)) return GA_DELETE;
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    }
    ga_fail(e,L(0));return GA_UNSUPPORTED;   /* 07E832, 07E902, 07EB4E: not in the capture */
}
static bool jump_routine(uint32_t r) {
    static const uint32_t k[]={0x7e554,0x7e590,0x7e658,0x7e678,0x7e6ca,0x7e738,0x7e832,0x7e856,0x7e902,0x7e91c,0x7ea1e,0x7ea48,0x7ea6c,0x7ea92,
        0x7eb4e,0x7ebcc,0x7ebee,0x7ec6a,0x7ecd2,0x7edf0,0x7ee2e};
    for (unsigned i=0;i<sizeof k/sizeof k[0];i++) if (r==k[i]) return true;
    return false;
}
static bool bin_routine(uint32_t r) {return r==0x33526u || r==0x335c6u || r==0x3378au || r==0x337c4u || r==0x33864u || r==0x338b8u;}
bool neo_m4props_routine(uint32_t r) {
    if (bin_routine(r) || urn_routine(r) || van_routine(r) || rock_routine(r) || jump_routine(r)) return true;
    static const uint32_t k[]={0xb9f4a,0xb9fbc,0xb9fd8,0xba192,0xba1b8,0xba1f4,0xbb106,0xbb15c,0xb99ee,0xb9a1e,0xb9ad6,0xb9b06,
        0xb9b24,0xb9b54,0xb9b6c,0xb9b7a,0xb9ba2,0xb9bb4,0xb9c7a,0xb9c8e,0xb9d88,0xb9de4,0xb9ea4};
    for (unsigned i=0;i<sizeof k/sizeof k[0];i++) if (r==k[i]) return true;
    return false;
}
ga_result_t neo_m4props_tick(neo_gactor_t *a,neo_genv_t *e) {return jump_routine(L(0))?jump_tick(a,e):rock_routine(L(0))?rock_tick(a,e):bin_routine(L(0))?bin_tick(a,e):urn_routine(L(0))?urn_tick(a,e):van_routine(L(0))?van_tick(a,e):m4props_tick(a,e);}
