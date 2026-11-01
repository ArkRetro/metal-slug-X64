#include "neo_region.h"
#include <string.h>
/* Mission 2 soldiers 0678B6 / 0678C0 / 0678CA / 067902 (family b) and
 * 0692B2..0692E4 (family c, which joins family b at 067A40). Manual
 * translation of ROM 0678B6..069700; ROM labels are provenance only.
 *
 * Family b: variant in +9C (0 or 8: palettes of 068F98, rank tables
 * 2D26BC.. by 0260C8), waits 0678DC until the nearest player (01AF9A, +86)
 * is within +70 / +78 (068B4C / 068B7E), or appears beside it (067908:
 * 014A02 from the player's block, clamped to the screen), then 06905A and
 * one of: the entry 0679B0 (program 268C0A, solid E1B5C) or the hidden one
 * 067A6C (021CB6 flash; its program raises +80 by 022230); turn 0679EE;
 * think 067A40 (068BB8 / 068CAE); walk 067AB4/067B42 with the walker
 * 08CCD4 (+62 = +8A by facing; 068CD4, 068ABA, 068B08); turn 067BA6;
 * throw 067E66 (program 268DCC, callback 06899A: the knife 067FC0 for
 * +9C = 0); death 067C20 (program 269024: 068F26 item by 08CDB2, 068F4A).
 * Tails 069280 (+9A, 004C24, 06914A, spark 06918E) and 0688D6 (cull E1A5E).
 * Family c 0692B2 (+9C = 0) drops from above (06932E: 06951C keeps the
 * absolute position on the new floor object, +34 by 2D29B4, program 269530
 * with callback 069548), lands (069596: the dust 0695DC after the script
 * actor), stands 0693CA and goes to 067A40; its death in the air 069448.
 * Paths without MAME evidence stop explicitly (ga_fail): 067EE4 (+6E
 * modes, edge stop), 067D06 / 067D80 / 069478 (pieces 068D32, +7A), the
 * knife of +9C = 4/8 (0689D2, 069696), 0691D2 with +7A. The cartridge
 * checks before 004188 (10F00A/10F00C, 2FFFE0) are not reproduced. */
#define B(o) ga_b(a,(o))
#define W(o) ga_w(a,(o))
#define L(o) ga_l(a,(o))
#define SB(o,v) ga_sb(a,(o),(uint8_t)(v))
#define SW(o,v) ga_sw(a,(o),(uint16_t)(v))
#define SL(o,v) ga_sl(a,(o),(uint32_t)(v))
#define BIT(o,n) ((B(o)>>(n))&1u)
#define SET(o,n) SB(o,B(o)|(1u<<(n)))
#define CLR(o,n) SB(o,B(o)&~(1u<<(n)))
#define START(t) do { if (!ga_anim_start(a,e,(t),0x100)) return GA_UNSUPPORTED; } while (0)
#define STEP(r) do { r=ga_anim_step(a,e,call); if (r<0) return GA_UNSUPPORTED; } while (0)
#define STOP(at) do { ga_fail(e,(at)); return GA_UNSUPPORTED; } while (0)
#define P1_NODE 0x1014c4u
#define P2_NODE 0x101574u
static int16_t S16(uint16_t v) {return (int16_t)v;}
static uint16_t rank(neo_gactor_t *a,neo_genv_t *e,uint32_t table) {
    const uint8_t *t=ga_rom(table);
    if (!t) {ga_fail(e,table);return 0;}
    return ga_rank_word(a,e,t);
}
static void sound(neo_genv_t *e,uint16_t id) {if (e->sound) e->sound(e->ctx,id);}
static void solid(neo_gactor_t *a,neo_genv_t *e,uint32_t d) {if (e->solid) e->solid(e->ctx,a,d);}
/* 01AF9A: the nearest live player's node (P1 with carry when none). */
static bool nearest(neo_gactor_t *a,neo_genv_t *e,uint32_t *node) {
    unsigned m=e->players.active_mask&0xffu;
    if (!m) {*node=P1_NODE;return true;}
    if (m==1u) {*node=P1_NODE;return false;}
    if (m==2u) {*node=P2_NODE;return false;}
    uint16_t d1=(uint16_t)(e->players.p1x-W(0x3c)),d2=(uint16_t)(e->players.p2x-W(0x3c));
    if (d1&0x8000u) d1=(uint16_t)(0u-d1);
    if (d2&0x8000u) d2=(uint16_t)(0u-d2);
    *node=S16(d2)<S16(d1)?P2_NODE:P1_NODE;
    return false;
}
static uint16_t px(const neo_genv_t *e,uint32_t n) {return n==P2_NODE?e->players.p2x:e->players.p1x;}
static uint16_t py(const neo_genv_t *e,uint32_t n) {return n==P2_NODE?e->players.p2y:e->players.p1y;}
/* 01AF9A, +86 = the node. */
static bool target(neo_gactor_t *a,neo_genv_t *e,uint32_t *node) {bool c=nearest(a,e,node);SL(0x86,*node);return c;}
/* 08CCD4: walk on the ground (01456A), else fall (0143DC). */
static void walker(neo_gactor_t *a,neo_genv_t *e) {
    if (BIT(0x15,6)) ga_ground(a,e,NULL); else {SL(0x34,L(0x34)+0xffffb000u);ga_air(a,e);}
}
/* 068B4C / 068B7E: carry when |player - self| (X or Y words) is within the
 * signed range `r` (0: always), on the side of its sign. */
static bool within(uint16_t r,uint16_t p,uint16_t s) {
    if (!r) return true;
    uint16_t m=(r&0x8000u)?0xffffu:0u;
    r=(uint16_t)((r^m)-m);
    uint16_t d=(uint16_t)(((uint16_t)(p-s)^m)-m);
    if (d&0x8000u) return false;
    return d<=r;
}
/* 068A50: D0 = 1 when the player is left of the actor (+6E = 2: by the sign
 * of +72); carry when there is no player. */
static bool k68a50(neo_gactor_t *a,neo_genv_t *e,uint16_t *d0) {
    if (W(0x6e)==2u) {*d0=(W(0x72)&0x8000u)?1u:0u;return false;}
    uint32_t n;bool c=nearest(a,e,&n);
    *d0=S16(px(e,n))<S16(W(0x3c))?1u:0u;
    return c;
}
/* 068ABA: carry when the player is behind the actor. */
static bool behind(neo_gactor_t *a,neo_genv_t *e) {
    if (W(0x6e)==2u) return false;
    uint32_t n;if (nearest(a,e,&n)) return false;
    SL(0x86,n);
    if (S16(px(e,n))>=S16(W(0x3c))) return !BIT(0x15,0);
    return BIT(0x15,0);
}
/* 068BB8: carry when the player in front is within +94 (+6E = 1: when the
 * count +96 is over). 068F66 compares the creator's X with itself: no
 * effect. */
static bool k68bb8(neo_gactor_t *a,neo_genv_t *e) {
    uint32_t n;
    if (nearest(a,e,&n)) return false;
    if (W(0x6e)==1u) {
        if (!W(0x96)) return true;
        SW(0x96,W(0x96)-1u);return false;
    }
    SL(0x86,n);
    uint16_t d=(uint16_t)(W(0x3c)-px(e,n));
    if (!(d&0x8000u)) return !BIT(0x15,0) && S16(d)<=S16(W(0x94));
    d=(uint16_t)(0u-d);
    return BIT(0x15,0) && S16(d)<=S16(W(0x94));
}
/* 068CAE: carry near the screen edges. */
static bool edge(neo_gactor_t *a) {return S16(W(0x3c))<=0x10 || S16(W(0x3c))>=0x128;}
/* 068CD4: carry when the ground ahead ends (014850). */
static bool k68cd4(neo_gactor_t *a,neo_genv_t *e) {
    if (ga_probe_forward(a,e,BIT(0x15,0)?0x10:-0x10)) return false;
    if (W(0x6e)==2u) SW(0x6e,1);
    return true;
}
/* 068B08: +6E 2 -> 1 once +28 left +8E by +72. */
static void k68b08(neo_gactor_t *a) {
    if (W(0x6e)!=2u) return;
    int16_t d=S16((uint16_t)(W(0x28)-W(0x8e)));
    if (!(W(0x72)&0x8000u)) {if (d>=S16(W(0x72))) SW(0x6e,1);}
    else if (d<=S16(W(0x72))) SW(0x6e,1);
}
/* 068F98: palettes by +9C. */
static void palettes(neo_gactor_t *a) {
    SB(0x1d,0);                                                        /* 009F88 */
    if (B(0x9c)==0u) {ga_palette(a,0x3fc);ga_palette(a,0x3fd);ga_palette(a,0xf2);ga_flash_palette(a,0xac);}
    if (B(0x9c)==4u) {ga_palette(a,0x17e);ga_palette(a,0x17f);ga_palette(a,0x187);ga_flash_palette(a,0x180);}
    if (B(0x9c)==8u) {ga_palette(a,0x894);ga_palette(a,0x893);ga_palette(a,0x893);ga_flash_palette(a,0x895);}
}
/* 0690EC: +96 = the patience of a +6E = 1 actor. */
static void k690ec(neo_gactor_t *a,neo_genv_t *e) {
    SW(0x96,0);
    if (W(0x6e)!=1u) return;
    if (B(0x9c)==0u) SW(0x96,rank(a,e,0x2d2844));
    if (B(0x9c)==4u) SW(0x96,rank(a,e,0x2d28a6));
    if (B(0x9c)==8u) SW(0x96,rank(a,e,0x2d2908));
}
/* 06905A: common set-up. */
static void k6905a(neo_gactor_t *a,neo_genv_t *e) {
    SW(0x66,0x8820);SW(0x1e,0x8820);
    palettes(a);
    SW(0x4e,rank(a,e,0x2d26bc));
    SL(0x8a,(uint32_t)(int32_t)S16(rank(a,e,0x2d27e2))<<8);
    SL(0x62,0);SB(0x61,0);SB(0x60,0);SB(0x81,0);SB(0x9b,0);SB(0x9a,0);
    uint16_t d0=ga_rand_range(e,0x18),d1=0;
    if (B(0x9c)==0u) d1=0x38;
    if (B(0x9c)==4u || B(0x9c)==8u) d1=0x58;
    SW(0x94,d1-d0);
    SW(0x8e,W(0x28));
    k690ec(a,e);
}
/* 08CDB2 from 068F26: the item +74 (kind < 0x5A) / +76 after the script
 * actor at (0, 8). */
static void drop_item(neo_gactor_t *a,neo_genv_t *e) {
    if (W(0x74)==0xffffu || W(0x74)>=0x5au) return;
    neo_gactor_t *c=e->spawn?e->spawn(e->ctx,NULL,0x8c374,true):NULL;
    if (c) {ga_sb(c,0x8b,0);ga_sw(c,0x70,W(0x76));ga_sw(c,0x6e,W(0x74));ga_sw(c,0x72,0);ga_sb(c,0x7f,0);ga_place_at(c,a,e,0,8);}
}
static bool call(neo_gactor_t *a,neo_genv_t *e,neo_anim_kind_t kind,uint32_t routine,uint16_t parameter) {
    (void)parameter;
    if (kind!=NEO_ANIM_CALL) return false;
    switch (routine) {
    case 0x68a40:SB(0x14,B(0x19));return true;
    case 0x68a48:SB(0x14,B(0x1a));return true;
    case 0x68bb0:SB(0x15,B(0x15)^1u);return true;
    case 0x68f26:drop_item(a,e);return true;
    case 0x68f4a:
        if (B(0x81)) {neo_gactor_t *p=e->creator?e->creator(e->ctx,a):NULL;if (p && !ga_b(p,0x80)) ga_sb(p,0x80,1);}
        return true;
    case 0x691d2:return !W(0x7a);                                      /* +7A: the bombs 0687F6 (not observed) */
    case 0x69262:return !W(0x7a) || B(0x61);                           /* +7A: 0687AA (not observed) */
    case 0x6899a:                                                      /* by +9C (table 0689AA) */
        if (B(0x9c)) return false;                                     /* 069696 / 0689D2: not observed */
        sound(e,0x1144);
        if (e->spawn) e->spawn(e->ctx,a,0x67fc0,false);
        return true;
    case 0x69548: {                                                    /* +99: the drop goes on */
        SB(0x99,0xff);
        uint32_t n;if (nearest(a,e,&n)) return true;
        uint16_t d=(uint16_t)(px(e,n)-W(0x3c));
        d^=(uint16_t)(int16_t)(int8_t)B(0x9d);
        if (!(d&0x8000u)) return true;
        if (ga_probe_vertical(a,e,-0x60)) SB(0x99,0);
        return true;
    }
    case 0x22230: {                                                    /* 021CB6: the creator's +80 = 3 */
        neo_gactor_t *p=e->creator?e->creator(e->ctx,a):NULL;
        if (p) ga_sb(p,0x80,3);
        return true;
    }
    }
    return false;
}
/* 06951C: the floor object (0146E8) with the absolute position kept
 * (01AC4C from the old +38 to the new one), then 01436A. */
static void refloor(neo_gactor_t *a,neo_genv_t *e) {
    uint32_t x0=L(0x28),y0=L(0x2c),x1=L(0x30),y1=L(0x34),old=L(0x38);
    ga_floor(a,e);
    uint32_t tag=L(0x38);
    if (tag!=old) {
        neo_contact_parent_t op,np;bool ho=old && ga_parent(e,old,&op),hn=tag && ga_parent(e,tag,&np);
        x1+=x0;y1+=y0;
        if (ho) {x0+=(uint32_t)op.previous_x;y0+=(uint32_t)op.previous_y;x1+=(uint32_t)op.x;y1+=(uint32_t)op.y;}
        if (hn) {x0-=(uint32_t)np.previous_x;y0-=(uint32_t)np.previous_y;x1-=(uint32_t)np.x;y1-=(uint32_t)np.y;}
        x1-=x0;y1-=y0;
    }
    SL(0x28,x0);SL(0x2c,y0);SL(0x30,x1);SL(0x34,y1);
    ga_stationary(a,e);
}
/* 06918E: the spark 0682F0 after the script actor (+9A: one per 0x18). */
static void spark(neo_gactor_t *a,neo_genv_t *e) {
    if (B(0x9a)) return;
    SB(0x9a,0x18);
    neo_gactor_t *c=e->spawn?e->spawn(e->ctx,NULL,0x682f0u,true):NULL;
    uint16_t r=ga_rand(e);
    int16_t dx=(int16_t)((r&7u)-3u),dy=(int16_t)(((r>>8)&7u)+0x10u);
    if (c) ga_place_at(c,a,e,dx,dy);
}
/* 069582: a drop facing back turns. */
static void k69582(neo_gactor_t *a) {if (B(0x9d)&0x80u) {SB(0x9d,0u-B(0x9d));SB(0x15,B(0x15)^1u);}}
/* 069596: the landing dust 0695DC once (+9E bit 0), after the script actor. */
static void dust(neo_gactor_t *a,neo_genv_t *e) {
    if (BIT(0x9e,0)) return;
    SET(0x9e,0);
    neo_gactor_t *c=e->spawn?e->spawn(e->ctx,NULL,0x695dcu,true):NULL;
    if (!c) return;
    ga_sb(c,0x9c,B(0x9c));ga_place_from(c,a,e,0,0);
    if (B(0x9d)&0x80u) ga_sb(c,0x15,ga_b(c,0x15)^1u);
}
/* 06914A: the death by +5A. */
static bool k6914a(neo_gactor_t *a,neo_genv_t *e) {
    if (B(0x5a)==0x0au || B(0x5a)==0x0bu) {ga_fail(e,0x67d06);return false;}   /* pieces 068D32 */
    if (B(0x5a)==7u) {SB(0x14,B(0x1b));SB(0x17,0xff);}
    if (W(0x7a)) {ga_fail(e,0x67d80);return false;}
    SL(0,0x67c20);return true;
}
enum { TAIL=1, CULL, DELETE };
static ga_result_t soldier(neo_gactor_t *a,neo_genv_t *e) {
    uint32_t pc=L(0),n;int r;uint16_t d0;
    for (;;) switch (pc) {
    case 0x678a2:SB(0x9c,4);pc=0x678d0;break;
    case 0x678b6:SB(0x9c,8);pc=0x678d0;break;
    case 0x678ca:SB(0x9c,0);pc=0x678d0;break;
    case 0x678d0:ga_floor(a,e);SL(0,0x678dc);pc=0x678dc;break;
    case 0x678dc:
        ga_stationary(a,e);
        target(a,e,&n);
        if (within(W(0x70),px(e,n),W(0x3c)) && within(W(0x78),py(e,n),W(0x40))) {pc=0x6796a;break;}
        if (ga_cull(a,e,0xe1a66)) {SB(0x60,0xff);return GA_DELETE;}        /* 0688F4 */
        return GA_MORE;
    case 0x678ac:SB(0x9c,4);pc=0x67908;break;
    case 0x678c0:SB(0x9c,8);pc=0x67908;break;
    case 0x67902:SB(0x9c,0);pc=0x67908;break;
    case 0x67908: {
        ga_stationary(a,e);
        target(a,e,&n);
        uint8_t *p=e->vehicle?e->vehicle->player[n==P2_NODE]:NULL;
        if (!p) STOP(0x14a02);
        neo_gactor_t pl;memset(&pl,0,sizeof(pl));memcpy(pl.m,p,0xb0);pl.index=a->index;pl.active=true;
        ga_sweep_place(&pl,e,a,(int16_t)W(0x70),4);                    /* 014A02 from the player */
        ga_project(a,e);
        if (S16(W(0x3c))<-0x20) SW(0x28,W(0x28)+(uint16_t)(0xffe0u-W(0x3c)));
        if (S16(W(0x3c))>0x160) SW(0x28,W(0x28)+(uint16_t)(0x160u-W(0x3c)));
        ga_floor(a,e);pc=0x6796a;break;
    }
    case 0x6796a:
        k6905a(a,e);
        if (S16(W(0x3c))<=0 || S16(W(0x3c))>=0x160) {SL(0,0x67ab4);return GA_MORE;}
        SL(0,(W(0x6e)&0x80u)?0x67a6c:0x679b0);return GA_MORE;
    case 0x679b0:START(0x268c0a);solid(a,e,0xe1b5c);SL(0,0x679d2);pc=0x679d2;break;
    case 0x679d2: case 0x67a24:
        walker(a,e);STEP(r);
        if (r>0) SL(0,pc==0x679d2?0x679ee:0x67a40);
        pc=CULL;break;
    case 0x679ee:
        solid(a,e,0xe1b4e);START(0x268ca8);
        k68a50(a,e,&d0);
        if (!d0) SET(0x15,0);
        SL(0x62,0);SL(0,0x67a24);pc=0x67a24;break;
    case 0x67a40:
        SW(0x6e,W(0x6e)&0xff7fu);solid(a,e,0xe1b4e);
        if (!k68bb8(a,e)) {pc=0x67ab4;break;}
        if (edge(a)) STOP(0x67ee4);
        pc=0x67e66;break;
    case 0x67a6c:
        SW(0x6e,W(0x6e)&0xff7fu);
        {neo_gactor_t *c=e->spawn?e->spawn(e->ctx,a,0x21cb6u,false):NULL;if (c) ga_place_at(c,a,e,0,0);}
        SB(0x80,0);SL(0x62,0);SL(0,0x67a9a);pc=0x67a9a;break;
    case 0x67a9a:
        walker(a,e);
        if (B(0x80)) SL(0,0x679ee);
        pc=CULL;break;
    case 0x67ab4: {
        START(0x268d0c);SL(0x50,0xe1a96);solid(a,e,0xe1b4e);
        if (k68a50(a,e,&d0)) STOP(0x67ee4);
        target(a,e,&n);
        bool turn;
        if (W(0x6e)==2u) turn=false;
        else turn=d0?BIT(0x15,0):!BIT(0x15,0);
        if (turn) {pc=0x67ba6;break;}
        if (W(0x6e)==1u) STOP(0x67ee4);
        uint32_t v=L(0x8a);if (!BIT(0x15,0)) v=0u-v;
        SL(0x62,v);SL(0,0x67b42);pc=0x67b42;break;
    }
    case 0x67b42:
        walker(a,e);STEP(r);
        if (nearest(a,e,&n)) SL(0,0x67ee4);
        else {
            if (k68cd4(a,e)) SL(0,0x67ee4);
            if (k68bb8(a,e)) SL(0,edge(a)?0x67ee4:0x67e66);
            if (behind(a,e)) SL(0,0x67ba6);
        }
        k68b08(a);
        pc=TAIL;break;
    case 0x67ba6:
        SL(0x50,0xe1a96);solid(a,e,0xe1b4e);START(0x268fee);SL(0x62,0);SL(0,0x67bd4);pc=0x67bd4;break;
    case 0x67bd4: case 0x67e98:
        walker(a,e);STEP(r);
        if (r>0) {
            if (k68bb8(a,e)) SL(0,edge(a)?0x67ee4:0x67e66); else SL(0,0x67ab4);
            if (behind(a,e)) SL(0,0x67ba6);
        }
        pc=TAIL;break;
    case 0x67e66:
        SL(0x50,0xe1a96);solid(a,e,0xe1b4e);START(0x268dcc);SL(0x62,0);k690ec(a,e);
        SL(0,0x67e98);pc=0x67e98;break;
    case 0x67c20:
        SL(0x50,0);solid(a,e,0);START(0x269024);SB(0x61,0xff);SB(0x69,0xff);   /* 019DFC */
        SL(0x62,0);sound(e,0x1143);SL(0,0x67c62);pc=0x67c62;break;
    case 0x67c62:
        walker(a,e);STEP(r);
        if (r>0) return GA_DELETE;                                     /* 004188 */
        pc=CULL;break;
    case TAIL:                                                         /* 069280 */
        if (B(0x9a)) SB(0x9a,B(0x9a)-1u);
        if (ga_register(a,e,NULL,NULL)) {
            if (BIT(0x5f,7) && !k6914a(a,e)) return GA_UNSUPPORTED;
            SB(0x5a,0);spark(a,e);
        }
        pc=CULL;break;
    case CULL:                                                         /* 0688D6 */
        if (ga_cull(a,e,0xe1a5e)) {SB(0x60,0xff);return GA_DELETE;}
        return GA_MORE;
    /* ---- family c ---- */
    case 0x692b2:SB(0x9c,0);pc=0x6932e;break;
    case 0x692c6:SB(0x9c,4);pc=0x6932e;break;
    case 0x692da:SB(0x9c,8);pc=0x6932e;break;
    case 0x6932e:
        k6905a(a,e);SB(0x14,B(0x1a));refloor(a,e);SB(0x9e,0);          /* 069434 */
        SL(0x30,0);
        SL(0x34,(uint32_t)(int32_t)S16((uint16_t)(0u-rank(a,e,0x2d29b4)))<<8);
        START(0x269530);SL(0x50,0xe1a96);SL(0,0x6936e);pc=0x6936e;break;
    case 0x6936e:
        if (!BIT(0x15,6)) {ga_air(a,e);STEP(r);pc=0x693ec;break;}
        dust(a,e);
        START((B(0x9d)&0x80u)?0x269702:0x2696a6);
        solid(a,e,0xe1b4e);SL(0,0x693ca);pc=0x693ca;break;
    case 0x693ca:
        ga_stationary(a,e);STEP(r);
        if (r>0) SL(0,0x693e4);
        pc=0x693ec;break;
    case 0x693e4:k69582(a);pc=0x67a40;break;
    case 0x693ec:                                                      /* family c tail */
        if (B(0x9a)) SB(0x9a,B(0x9a)-1u);
        if (ga_register(a,e,NULL,NULL)) {
            if (BIT(0x5f,7)) {
                if (BIT(0x15,6)) {k69582(a);if (!k6914a(a,e)) return GA_UNSUPPORTED;}
                else {                                                 /* 069448 */
                    if (B(0x5a)==0x0au || B(0x5a)==0x0bu) STOP(0x69478);
                    if (B(0x5a)==7u) {SB(0x14,B(0x1b));SB(0x17,0xff);}
                    SL(0,0x69484);
                }
            }
            SB(0x5a,0);spark(a,e);
        }
        pc=CULL;break;
    case 0x69484:k69582(a);START(0x26975e);sound(e,0x1143);SL(0,0x694a8);pc=0x694a8;break;
    case 0x694a8:
        ga_stationary(a,e);STEP(r);
        if (r>0) SL(0,0x694c4);
        pc=CULL;break;
    case 0x694c4:dust(a,e);SL(0,0x694ce);pc=0x694ce;break;
    case 0x694ce:
        if (!BIT(0x15,6)) {SL(0x34,L(0x34)+0xffffb000u);ga_air(a,e);STEP(r);pc=CULL;break;}
        START(0x269130);SL(0,0x69500);pc=0x69500;break;
    case 0x69500:
        ga_stationary(a,e);STEP(r);
        if (r>0) return GA_DELETE;
        pc=CULL;break;
    default:ga_fail(e,pc);return GA_UNSUPPORTED;
    }
}
/* 067FC0: the knife of 06899A at its creator (01AD6E), palette 0x3FB,
 * program 268F24 (+60 = attack row of table E1A76 while not FF); gone with
 * the program or when the creator dies (+61). */
static ga_result_t knife(neo_gactor_t *a,neo_genv_t *e) {
    neo_gactor_t *p=e->creator?e->creator(e->ctx,a):NULL;
    if (L(0)==0x67fc0u) {
        SB(0x1d,0);SW(0x66,0xe0ef);SW(0x1e,0xe0ef);ga_palette(a,0x3fb);
        START(0x268f24);
        ga_place(a,e,0,0,p?NEO_GA_TAG(p->index):0);
        SB(0x60,0xff);
        if (p && (ga_b(p,0x15)&1u)) SET(0x15,0);
        SL(0,0x68020);return GA_MORE;
    }
    int r;
    ga_stationary(a,e);STEP(r);
    if (r>0) return GA_DELETE;
    if (B(0x60)!=0xffu) {
        const uint8_t *t=ga_rom(0xe1a76u+4u*B(0x60));
        if (!t) STOP(0xe1a76);
        ga_attack(a,e,(uint32_t)t[0]<<24|(uint32_t)t[1]<<16|(uint32_t)t[2]<<8|t[3]);
    }
    if (p && ga_b(p,0x61)) return GA_DELETE;
    return GA_MORE;
}
/* 021CB6: the flash of the hidden entry 067A6C (palette 0xB0, program
 * 2309D2: callback 022230 raises the creator's +80). */
static ga_result_t flash(neo_gactor_t *a,neo_genv_t *e) {
    if (L(0)==0x21cb6u) {
        SB(0x1d,0);SW(0x66,0xdfec);SW(0x1e,0xdfec);ga_palette(a,0xb0);
        START(0x2309d2);SL(0,0x21cee);return GA_MORE;
    }
    int r;
    ga_project(a,e);
    if ((W(0x66)&0xf000u)==0x8000u) SW(0x1e,W(0x66)-W(0x40));          /* 01AD8C */
    STEP(r);
    return r>0?GA_DELETE:GA_MORE;
}
static const uint32_t STATES[]={0x678a2,0x678ac,0x678b6,0x678c0,0x678ca,0x67902,0x678dc,0x679b0,0x679d2,0x679ee,0x67a24,0x67a40,0x67a6c,0x67a9a,
    0x67ab4,0x67b42,0x67ba6,0x67bd4,0x67c20,0x67c62,0x67e66,0x67e98,0x692b2,0x692c6,0x692da,0x6936e,0x693ca,0x693e4,
    0x69484,0x694a8,0x694c4,0x694ce,0x69500,0x67fc0,0x68020,0x21cb6,0x21cee};
bool neo_rebel3_routine(uint32_t r) {
    for (unsigned i=0;i<sizeof(STATES)/sizeof(STATES[0]);i++) if (STATES[i]==r) return true;
    return false;
}
ga_result_t neo_rebel3_tick(neo_gactor_t *a,neo_genv_t *e) {
    uint32_t r=ga_l(a,0);
    if (r==0x67fc0u || r==0x68020u) return knife(a,e);
    if (r==0x21cb6u || r==0x21ceeu) return flash(a,e);
    return soldier(a,e);
}
