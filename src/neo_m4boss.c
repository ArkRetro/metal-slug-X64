#include "neo_region.h"
#include "neo_soldier_ai.h"   /* neo_angle (ROM 002EB4) */
/* Mission 4 boss 02267E (script order 0BB028, camera 5552): manual
 * translation of its parts, one by one; ROM labels are provenance only.
 *
 * Debris 0259F2 / 025A5E (created by the boss's parts, capture T):
 * 0259F2: palettes released (009F88), depth +66 / +1E = E0EC, palette 0xF0
 * (009EA8, D1 = 0), program 246AD2 at speed 0x100, +15 bit 2, and bit 0 (the
 * facing) when the random number's bit 0 is clear (0029C6); then 025A42
 * every update: 01ABC8, the program (004188 at its end), deleted outside the
 * rectangle E007C (024978 -> 019504). 02591C, 025B6E.. below.
 * 025A5E: the same with palette 0x276, +30 = 1000, +34 = C000 and program
 * 234D04; then 025AAA: 01AC38 (moves by +30 / +34), the program (its end
 * falls to the cartridge check 025ABA and 004188), 024978. */
#define B(o) ga_b(a,(o))
#define W(o) ga_w(a,(o))
#define L(o) ga_l(a,(o))
#define SB(o,v) ga_sb(a,(o),(uint8_t)(v))
#define SW(o,v) ga_sw(a,(o),(uint16_t)(v))
#define SL(o,v) ga_sl(a,(o),(uint32_t)(v))
#define BIT(o,b) ((B(o)>>(b))&1u)
#define SET(o,b) SB(o,B(o)|(1u<<(b)))
#define CLR(o,b) SB(o,B(o)&~(1u<<(b)))
#define S16(v) ((int16_t)(uint16_t)(v))
#define STOP(at) do { ga_fail(e,(at)); return GA_UNSUPPORTED; } while (0)
#define START(t) do { if (!ga_anim_start(a,e,(t),0x100)) return GA_UNSUPPORTED; } while (0)
#define LAYER 0x10d7b6u
#define P1_NODE 0x1014c4u
#define P2_NODE 0x101574u
static uint8_t rb(neo_genv_t *e,uint32_t at) {const uint8_t *p=ga_rom(at);if (!p) {ga_fail(e,at);return 0;}return p[0];}
static uint16_t rw(neo_genv_t *e,uint32_t at) {const uint8_t *p=ga_rom(at);if (!p) {ga_fail(e,at);return 0;}return (uint16_t)(p[0]<<8|p[1]);}
static uint32_t rl(neo_genv_t *e,uint32_t at) {return (uint32_t)rw(e,at)<<16|rw(e,at+2);}
static void sound(neo_genv_t *e,uint16_t id) {if (e->sound) e->sound(e->ctx,id);}   /* 0120F8 */
/* 0260C8 (a type-2 table takes a random number). */
static uint16_t rank(neo_gactor_t *a,neo_genv_t *e,uint32_t table) {
    const uint8_t *t=ga_rom(table);
    if (!t) {ga_fail(e,table);return 0;}
    return ga_rank_word(a,e,t);
}
static neo_gactor_t *creator(neo_gactor_t *a,neo_genv_t *e) {neo_gactor_t *c=e->creator?e->creator(e->ctx,a):NULL;if (!c) ga_fail(e,0x4000cu);return c;}
/* 0040BC with the reserve D0. */
static neo_gactor_t *child(neo_gactor_t *a,neo_genv_t *e,uint32_t routine,uint16_t reserve) {
    return e->spawn_reserve?e->spawn_reserve(e->ctx,a,routine,reserve):e->spawn?e->spawn(e->ctx,a,routine,false):NULL;
}
/* 0040BC + 01ACF2: a child at (dx, dy) from the node. */
static neo_gactor_t *child_at(neo_gactor_t *a,neo_genv_t *e,uint32_t routine,uint16_t reserve,int16_t dx,int16_t dy) {
    neo_gactor_t *c=child(a,e,routine,reserve);
    if (c) ga_place_at(c,a,e,dx,dy);
    return c;
}
/* 01AD6E: (dx, dy) relative to the creator's block. */
static void on_creator(neo_gactor_t *a,neo_genv_t *e,int16_t dx,int16_t dy) {neo_gactor_t *p=creator(a,e);ga_place(a,e,dx,dy,p?NEO_GA_TAG(p->index):0);}
/* 01DA40: the explosion list emitter 01D764 (+86 = list) at the node. */
static void k1da40(neo_gactor_t *a,neo_genv_t *e,uint32_t list) {
    neo_gactor_t *c=child(a,e,0x1d764,0x1c);
    if (!c) return;
    ga_place_from(c,a,e,0,0);
    if (B(0x15)&4u) ga_sb(c,0x15,(uint8_t)(ga_b(c,0x15)|4u));
    ga_sl(c,0x86,list);
}
/* 018F92: mission complete (as neo_m3boss.c). */
static void k18f92(neo_gactor_t *a,neo_genv_t *e) {
    neo_ga_flags_t *f=e->flags;
    if (!f) {ga_fail(e,0x18f92);return;}
    f->kill_request=0xff;f->flash_request=0xff;f->player_guard=1;
    if (e->vehicle) for (unsigned p=0;p<2;p++) {
        uint8_t *n=e->vehicle->player[p];
        if (!e->vehicle->exists[p] || !n) continue;
        n[0x84]=0xff;n[0x85]=0xff;n[0x5f]|=0x40u;n[0x9a]&=(uint8_t)~4u;
    }
    f->mission_end=0xff;f->end_kind=B(0x5e);f->player_handlers=2;
}
/* 01B04A: player 1, 2 or (both live) one at random; carry when none. */
static bool k1b04a(neo_genv_t *e,uint32_t *node) {
    unsigned d0=e->players.active_mask&0xffu;
    if (d0>=3u) d0=(ga_rand(e)&1u)+1u;
    *node=d0==2u?P2_NODE:P1_NODE;
    return d0==0;
}
/* 01B14A / 01B162: the angle to the centre of the first +50 rectangle of
 * the player node `node` (0x80/0x180 when there is none... the node is always a player here). */
static bool aim(neo_gactor_t *a,neo_genv_t *e,uint32_t node,uint8_t *angle) {
    const uint8_t *n=NULL;
    if (e->vehicle) n=node==P1_NODE?e->vehicle->player[0]:node==P2_NODE?e->vehicle->player[1]:NULL;
    if (!n) {ga_fail(e,0x1b162);return false;}
    int16_t x=(int16_t)(n[0x3c]<<8|n[0x3d]),y=(int16_t)(n[0x40]<<8|n[0x41]);
    uint32_t d=(uint32_t)n[0x50]<<24|(uint32_t)n[0x51]<<16|(uint32_t)n[0x52]<<8|n[0x53];
    if (d) {
        const uint8_t *r=ga_rom(d+2u);
        if (!r) {ga_fail(e,d);return false;}
        int16_t r0=(int16_t)(r[0]<<8|r[1]),r1=(int16_t)(r[2]<<8|r[3]),r2=(int16_t)(r[4]<<8|r[5]),r3=(int16_t)(r[6]<<8|r[7]);
        int16_t dx=(int16_t)((r2>>1)+r0),dy=(int16_t)((r3>>1)+r1);
        if (B(0x15)&1u) dx=(int16_t)-dx;
        if (B(0x15)&2u) dy=(int16_t)-dy;
        x=(int16_t)(x+dx);y=(int16_t)(y+dy);
    }
    *angle=(uint8_t)neo_angle((int16_t)(x-S16(W(0x3c))),(int16_t)(y-S16(W(0x40))));
    return true;
}
/* 03B332: speeds to cover D0/D1 in D3 updates under D2. */
static void launch(int16_t d0,int16_t d1,uint32_t d2,uint16_t d3,uint32_t *vx,uint32_t *vy) {
    uint32_t g=(d2&0x80000000u)?0u-d2:d2;
    if ((int16_t)d3<2) d3=2;
    uint16_t q=(uint16_t)(0x10000u/d3);
    uint32_t x=(uint32_t)((int32_t)d0*(int16_t)q),y=(uint32_t)((int32_t)d1*(int16_t)q);
    uint32_t t=(g&0xffffu)*d3+(((g>>16)*d3)<<16);
    t=(uint32_t)((int32_t)t>>1);
    if (d2&0x80000000u) t=0u-t;
    *vx=x;*vy=y-t;
}
static neo_m4boss_env_t *menv(neo_genv_t *e) {if (!e->m4boss || !e->m4boss->layer) {ga_fail(e,LAYER);return NULL;}return e->m4boss;}
static void lsw(uint8_t *l,unsigned o,uint16_t v) {l[o]=(uint8_t)(v>>8);l[o+1]=(uint8_t)v;}
static uint32_t ll(const uint8_t *l,unsigned o) {return (uint32_t)(l[o]<<24|l[o+1]<<16|l[o+2]<<8|l[o+3]);}
/* ---- the body 02267E ----------------------------------------------------------- */
/* 024BDA: the layer 10D7B6 becomes the boss's picture: speeds 694E/6952 0,
 * 0C6D04, 0C702A, 0C703C, at the body's +28/+2C (01AB9C, the scene root),
 * owner 3 (0C7BD0), +82 bits 1/2 off and 3 on, the map 02225E (columns
 * 0223A8; 0C6F34), its 20 x 13 blocks (0C73CE), 0C7072, 0C741A, 0C6CFC; the
 * body on the layer at (0xA8, -0xC0) (01AB98). */
static void k24bda(neo_gactor_t *a,neo_genv_t *e) {
    neo_m4boss_env_t *m=menv(e);
    if (!m) return;
    uint8_t *l=m->layer;
    m->speed[0]=0;m->speed[1]=0;
    l[0x82]&=0xfeu;                                                     /* 0C6D04 */
    if (m->clear) m->clear(e->ctx);                                     /* 0C702A ring, 0C703C/0C6D04 SCB */
    neo_layerobj_place(e,l,S16(W(0x28)),S16(W(0x2c)));
    m->owner=3;                                                         /* 0C7BD0 */
    l[0x82]=(uint8_t)((l[0x82]&~6u)|8u);
    neo_layerobj_map(l,0x2225eu,0,0,0x223a8u);
    neo_layerobj_blocks(e,l,m->draw,0,0,0x14,0xd);
    for (unsigned i=0;i<32;i++) l[0x42+2*i+1]=0;                       /* 0C7072: the column table's heights */
    {   /* 0C741A: the window +36/+3A, +38/+3C and its 32 x 32 blocks */
        if (ll(l,0x2e)) {
            uint16_t d0=(uint16_t)(S16((uint16_t)((0xffc00000u-ll(l,0x14))>>16))>>4);
            uint16_t d1=(uint16_t)(S16((uint16_t)((0x800000u-ll(l,0x18))>>16))>>4);
            lsw(l,0x36,d0);lsw(l,0x3a,d0);lsw(l,0x38,d1);lsw(l,0x3c,d1);
            d1=(uint16_t)(d1+0x1f);
            uint16_t d4=d0,d5=d1;
            d0=(uint16_t)(d0-(uint16_t)(l[0x32]<<8|l[0x33]));d1=(uint16_t)(d1-(uint16_t)(l[0x34]<<8|l[0x35]));d1=(uint16_t)-d1;
            neo_layerobj_columns(e,l,d0,0x20,d4);
            if (m->draw) {int16_t d[6]={(int16_t)d0,(int16_t)d1,0x20,0x20,(int16_t)d4,(int16_t)d5};m->draw(e->ctx,ll(l,0x2e),d);}
        }
    }
    l[0x82]|=0x81u;                                                     /* 0C6CFC */
    ga_place(a,e,0xa8,-0xc0,LAYER);                                     /* 01AB98 */
}
/* 024A30: +8A from the health +4E against +70 / +6E and the parts lost. */
static void k24a30(neo_gactor_t *a) {
    if (B(0x8f)==0x7fu) {SB(0x8a,2);return;}
    int16_t d0=S16(W(0x4e));
    if (d0<=S16(W(0x70))) {SB(0x8a,2);return;}
    if (d0<=S16(W(0x6e))) {SB(0x8a,1);return;}
    unsigned d1=0;
    for (unsigned b=0;b<=7u;b++) if (!BIT(0x8f,b)) d1++;
    if (d1<=3u) SB(0x8a,1);
}
static ga_result_t body_tick(neo_gactor_t *a,neo_genv_t *e) {
    switch (L(0)) {
    case 0x2267e: {
        neo_gactor_t *c=child(a,e,0x59f76,0);                           /* 059F4E: music 2C */
        if (c) ga_sw(c,0x72,0x2c);
        k24bda(a,e);
        if (e->unsupported) return GA_UNSUPPORTED;
        ga_project(a,e);
        static const struct {uint32_t r;uint8_t n;uint16_t z;bool b8;} parts[10]={
            {0x23526,0,0x200,true},{0x23526,1,0x1f0,true},{0x23526,2,0x1e0,true},
            {0x2391e,3,0x200,true},{0x2391e,4,0x1f0,true},{0x2391e,5,0x1d0,true},{0x2391e,6,0x1e0,true},
            {0x24226,7,0x200,true},{0x246d0,8,0x100,false},{0x22cb6,9,0x100,false}};
        for (unsigned i=0;i<10;i++) {
            neo_gactor_t *p=child(a,e,parts[i].r,0xe);
            if (!p) continue;
            ga_sw(p,0x90,parts[i].n);
            if (parts[i].b8) ga_sb(p,0x8b,parts[i].n);
            ga_sw(p,0x66,parts[i].z);ga_sw(p,0x1e,parts[i].z);
        }
        SW(0x92,0);SB(0x8a,0);SB(0x8b,0xff);SB(0x8c,0);SB(0x8d,0x08);SB(0x8e,0);SW(0x9a,0);SB(0x8f,0);SB(0x61,0);SB(0x60,0);SL(0x7c,0);
        SW(0x4e,rank(a,e,0x2c2662));
        SW(0x6e,0x10cc);SW(0x70,0xbb8);
        SL(0,0x22872);
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    }
    case 0x22872: {
        ga_project(a,e);
        if (BIT(0x8d,3)) {
            SB(0x8c,2);
            if (S16(W(0x3c))>=0x40) CLR(0x8d,3);
            return GA_MORE;
        }
        k24a30(a);
        if (!B(0x8e)) {
            if (!W(0x92)) {
                uint32_t node;
                if (!k1b04a(e,&node)) {
                    SL(0x72,node);
                    uint16_t d0=0;
                    if (B(0x8a)==0) {SW(0x92,rank(a,e,0x2c26e0));d0=rank(a,e,0x2c2776);}
                    if (B(0x8a)==1) {SW(0x92,rank(a,e,0x2c2712));d0=rank(a,e,0x2c27d4);}
                    if (B(0x8a)==2) {SW(0x92,rank(a,e,0x2c26e0));d0=1;}
                    bool seven=B(0x8f)==0x7fu;
                    if (!seven && S16(W(0x3c))<=0x100 && d0 && B(0x8b)!=8u) seven=true;
                    if (seven) d0=7;
                    else if (B(0x8b)==7u) d0=8;
                    else {
                        d0=ga_rand_range(e,7);                          /* 0029F2 */
                        bool loop=BIT(0x8f,d0&7u);
                        if (!loop) {
                            int16_t d2=(int16_t)(S16(W(0x3c))+S16(rw(e,0xe056eu+4u*d0)));
                            loop=d2<0x20;
                        }
                        if (loop) {
                            bool found=false;
                            for (d0=0;d0<=6u;d0++) {
                                if (BIT(0x8f,d0)) continue;
                                int16_t d2=(int16_t)(S16(W(0x3c))+S16(rw(e,0xe056eu+4u*d0)));
                                if (d2>=0x20) {found=true;break;}
                            }
                            if (!found) d0=7;
                        }
                    }
                    SB(0x8b,d0);SB(0x8e,0xff);
                }
            } else SW(0x92,W(0x92)-1u);
        } else {
            uint8_t d0=B(0x8b);
            for (unsigned i=0;i<=6u;i++) if (d0==i && BIT(0x8f,i)) {SB(0x8e,0);SB(0x8b,0xff);}
        }
        int16_t x=S16(W(0x3c));
        if (B(0x8a)==2u) {
            if (x>0xa0) SB(0x8c,3);
            else if (x<0x70) SB(0x8c,x<0x40?2:1);
            else SB(0x8c,0);
        } else {
            if (x<=0x90) SB(0x8c,1);
            if (x<=0x40) SB(0x8c,2);
            if (x>=0xe0) SB(0x8c,0);
        }
        if (BIT(0x5f,7)) SL(0,0x22b34);
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    }
    case 0x22b34: {
        k18f92(a,e);
        neo_m4boss_env_t *m=menv(e);
        if (m) m->point69b8=0xe0;                                       /* RAM 10E9B8 */
        SL(0,0x22b46);
    }   /* fall through */
    case 0x22b46:
        if (B(0x8e)) {ga_project(a,e);return e->unsupported?GA_UNSUPPORTED:GA_MORE;}
        sound(e,0x1026);
        SB(0x8b,7);SB(0x8c,0);SB(0x8e,0xff);SB(0x8a,2);
        k1da40(a,e,0xe040e);k1da40(a,e,0xe0432);k1da40(a,e,0xe0468);
        SL(0,0x22ba2);
        /* fall through */
    case 0x22ba2:
        ga_project(a,e);
        if (!B(0x8e)) SL(0,0x22bb8);
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    case 0x22bb8:
        k1da40(a,e,0xe0420);k1da40(a,e,0xe047a);
        SW(0x94,0x30);SL(0,0x22bdc);
        /* fall through */
    case 0x22bdc:
        ga_project(a,e);
        if (!W(0x94)) {
            SB(0x61,0xff);SET(0x8d,2);
            k1da40(a,e,0xe048c);k1da40(a,e,0xe0444);k1da40(a,e,0xe049e);k1da40(a,e,0xe0456);
            SL(0,0x22c36);
        } else SW(0x94,W(0x94)-1u);
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    case 0x22c36:
        k1da40(a,e,0xe0514);k1da40(a,e,0xe0526);k1da40(a,e,0xe0538);k1da40(a,e,0xe054a);k1da40(a,e,0xe055c);
        SW(0x94,0x10);SL(0,0x22c7e);
        /* fall through */
    case 0x22c7e:
        ga_project(a,e);
        if (!W(0x94)) SL(0,0x22c9c); else SW(0x94,W(0x94)-1u);
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    case 0x22c9c:
        if (e->flags) e->flags->stage_done=1;                          /* 018F40 */
        else STOP(0x18f40);
        SB(0x8c,4);SL(0,0x22cae);
        /* fall through */
    case 0x22cae:
        ga_project(a,e);
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    }
    STOP(L(0));
}
/* ---- shared by the parts ------------------------------------------------------- */
/* 023E96: the part's turn is over (the body's +8E cleared). */
static void k23e96(neo_gactor_t *a,neo_genv_t *e) {
    neo_gactor_t *c=creator(a,e);
    if (c && ga_b(c,0x8b)==B(0x8b) && ga_b(c,0x8e)) ga_sb(c,0x8e,0);
}
static void lost(neo_gactor_t *a,neo_genv_t *e) {neo_gactor_t *c=creator(a,e);if (c) ga_sb(c,0x8f,(uint8_t)(ga_b(c,0x8f)|(1u<<(W(0x90)&7u))));}
/* 024C94: the damage since the last update passes to the body (its +5F bit 7
 * and +5E when its health runs out); 024CCA: the same, and the part keeps
 * its own health. */
static void k24c94(neo_gactor_t *a,neo_genv_t *e,bool keep) {
    if (W(0x7a)!=W(0x4e)) {
        uint16_t d0=(uint16_t)(W(0x7a)-W(0x4e));
        neo_gactor_t *c=creator(a,e);
        if (c) {
            uint16_t h=(uint16_t)(ga_w(c,0x4e)-d0);ga_sw(c,0x4e,h);
            if (h&0x8000u) {ga_sw(c,0x4e,0);ga_sb(c,0x5f,(uint8_t)(ga_b(c,0x5f)|0x80u));ga_sb(c,0x5e,B(0x5e));}
        }
        if (keep) {SW(0x4e,W(0x7a));CLR(0x5f,7);}
        else SW(0x7a,W(0x4e));
    }
}
static bool k24a94(neo_gactor_t *a,neo_genv_t *e) {neo_gactor_t *c=creator(a,e);return c && ga_b(c,0x8a)==2u;}
static bool k24ab0(neo_gactor_t *a,neo_genv_t *e) {neo_gactor_t *c=creator(a,e);return c && (ga_b(c,0x5f)&0x80u);}
static bool k24acc(neo_gactor_t *a,neo_genv_t *e) {
    neo_gactor_t *c=creator(a,e),*g=c?creator(c,e):NULL;
    return g && (ga_b(g,0x5f)&0x80u);
}
/* 024AEC: +9C one step towards +9D; carry when they are equal. */
static bool k24aec(neo_gactor_t *a) {
    int8_t d0=(int8_t)B(0x9c),t=(int8_t)B(0x9d);
    if (d0==t) return true;
    SB(0x9c,d0>=t?(uint8_t)(d0-1):(uint8_t)(d0+1));
    return false;
}
/* 024B1C: +9D (0, 4, 8) from the angle to the player +72. */
static void k24b1c(neo_gactor_t *a,neo_genv_t *e) {
    uint8_t g;if (!aim(a,e,L(0x72),&g)) return;
    uint16_t d0=g;if (d0<0x80u) d0=(uint16_t)(0x100u-d0);
    SB(0x9d,d0<=0xa8u?0:d0>=0xd8u?8:4);
}
/* 024B62: 024AEC, then (+9C even) +9D from the angle to the player seen
 * 0x70 lower, by the thresholds 024BB6. */
static void k24b62(neo_gactor_t *a,neo_genv_t *e) {
    k24aec(a);
    if (BIT(0x9c,0)) return;
    uint32_t y=L(0x40);SW(0x40,W(0x40)+0x70u);
    uint8_t g;bool ok=aim(a,e,L(0x72),&g);
    SL(0x40,y);
    if (!ok) return;
    uint16_t d0=g;if (d0<0x80u) d0=(uint16_t)(0x100u-d0);
    for (unsigned o=0;o<64;o+=4) if (S16(d0)<=S16(rw(e,0x24bb6u+o))) {SB(0x9d,(uint8_t)rw(e,0x24bb8u+o));return;}
    ga_fail(e,0x24bb6);
}
/* 024D06: the aim point +76 by the angle to the player and the gun's pose. */
static void k24d06(neo_gactor_t *a,neo_genv_t *e) {
    uint8_t g;if (!aim(a,e,L(0x72),&g)) return;
    uint16_t d0=g;if (d0<0x80u) d0=(uint16_t)(0x100u-d0);
    unsigned d1=((unsigned)(B(0x9c)+1u)&0xeu)>>2;
    int16_t x=S16(W(0x3c)),d2=S16(W(0x76));
    if (d0<=0xa8u) {if (d1) d2=(int16_t)(d1==2u?x+0x28:x-0x10);}
    else if (d0>=0xd8u) {if (d1!=2u) d2=(int16_t)(!d1?x-0x28:x+0x10);}
    else if (d1!=1u) d2=(int16_t)(!d1?x-0x28:x+0x28);
    SW(0x76,d2);
}
/* The part's offset from the body (E056E) and programs by +90. */
static int16_t off_x(neo_genv_t *e,uint16_t n) {return S16(rw(e,0xe056eu+4u*n));}
static int16_t off_y(neo_genv_t *e,uint16_t n) {return S16(rw(e,0xe0570u+4u*n));}
static uint32_t prog_of(neo_genv_t *e,uint32_t table,uint16_t n) {return rl(e,table+4u*n);}
/* The end of most part updates: 004C24 (+5A cleared on a hit), killed
 * (+5F bit 7: 024164) and the body's damage, the body in its last phase
 * (024A94: 0240EE); `bit1` sets the part's +8D bit 1 on those exits. */
static void part_tail(neo_gactor_t *a,neo_genv_t *e,bool bit1,bool phase) {
    if (ga_register(a,e,NULL,NULL)) SB(0x5a,0);
    if (BIT(0x5f,7)) {k23e96(a,e);lost(a,e);if (bit1) SET(0x8d,1);SL(0,0x24164);}
    k24c94(a,e,false);
    if (phase && k24a94(a,e)) {k23e96(a,e);lost(a,e);if (bit1) SET(0x8d,1);SL(0,0x240ee);}
}
/* The program step of the parts that sit on the body's frame +9A (part 3
 * and the treads): +3C shifted by 4 * the frame (-2 or -1 at frame 9). */
static int step_shifted(neo_gactor_t *a,neo_genv_t *e,uint16_t frame,unsigned nine) {
    uint16_t d0=(uint16_t)(frame*4u);
    if (frame==9u) d0=(uint16_t)(d0-nine);
    uint32_t x=L(0x3c);SW(0x3c,W(0x3c)+d0);
    int st=ga_anim_step(a,e,NULL);
    SL(0x3c,x);
    return st;
}
static bool shell_call(neo_gactor_t *a,neo_genv_t *e,neo_anim_kind_t kind,uint32_t routine,uint16_t parameter);
static int step(neo_gactor_t *a,neo_genv_t *e) {return ga_anim_step(a,e,shell_call);}
/* ---- the guns 023526 (parts 0..2) and 02391E (parts 3..6) ----------------------- */
static ga_result_t gun_tick(neo_gactor_t *a,neo_genv_t *e) {
    neo_gactor_t *c;int st;
    bool b=L(0)>=0x2391eu;   /* 02391E */
    switch (L(0)) {
    case 0x23526: case 0x2391e:
        SB(0x1d,0);ga_palette(a,0xfb);ga_palette(a,0xfc);
        if (b) ga_palette(a,0xfc);
        ga_flash_palette(a,0x147);
        SB(0x14,B(0x19));SET(0x15,2);
        SW(0x4e,rank(a,e,b?0x2c26ae:0x2c267c));SW(0x7a,W(0x4e));
        on_creator(a,e,off_x(e,W(0x90)),off_y(e,W(0x90)));
        START(prog_of(e,0xe0596,W(0x90)));
        SB(0x8c,0);SL(0,b?0x239ee:0x235e8);
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    case 0x235e8: case 0x239ee:
    run:
        if (!(c=creator(a,e))) return GA_UNSUPPORTED;
        if (!b) {
            SL(0x50,(ga_b(c,0x8d)&8u)?0:0xe008c);
            ga_project(a,e);st=step(a,e);
        } else {
            ga_project(a,e);
            if (W(0x90)==3) {
                uint16_t f=ga_w(c,0x9a);
                SL(0x50,(ga_b(c,0x8d)&8u)?0:rl(e,0xe0098u+(uint16_t)(f*4u)));
                st=step_shifted(a,e,f,2);
            } else {SL(0x50,(ga_b(c,0x8d)&8u)?0:0xe00c0);st=step(a,e);}
        }
        if (st<0) return GA_UNSUPPORTED;
        if (ga_b(c,0x8e) && B(0x8b)==ga_b(c,0x8b)) SL(0,b?0x23ae6:0x23686);
        part_tail(a,e,false,true);
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    case 0x23686: case 0x23ae6:
        START(prog_of(e,0xe05ca,W(0x90)));
        if (!(c=creator(a,e))) return GA_UNSUPPORTED;
        SL(0x72,ga_l(c,0x72));SB(0x8a,ga_b(c,0x8a));
        if (B(0x8a)==0) {SW(0x92,rank(a,e,0x2c2832));SW(0x98,rank(a,e,0x2c287e));}
        if (B(0x8a)==1) {SW(0x92,rank(a,e,0x2c2864));SW(0x98,rank(a,e,0x2c28b0));}
        /* fall through */
    case 0x23704: case 0x23b64:
        if (!W(0x98)) {
            k23e96(a,e);SB(0x8c,0);                                     /* 0235B6 / 0239BC */
            START(prog_of(e,rl(e,0xe05beu+B(0x9c)),W(0x90)));
            SL(0,b?0x239ee:0x235e8);
            goto run;
        }
        SB(0x8d,0);
        child(a,e,0x23eb4,0x1c);
        SL(0,b?0x23b86:0x23726);
        /* fall through */
    case 0x23726: case 0x23b86:
        ga_project(a,e);
        if (b && W(0x90)==3) {
            if (!(c=creator(a,e))) return GA_UNSUPPORTED;
            uint16_t f=ga_w(c,0x9a);
            SL(0x50,rl(e,0xe0098u+(uint16_t)(f*4u)));
            st=step_shifted(a,e,f,2);
        } else st=step(a,e);
        if (st<0) return GA_UNSUPPORTED;
        if (BIT(0x8d,0)) SL(0,b?0x23c44:0x237a0);
        part_tail(a,e,true,true);
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    case 0x237a0: case 0x23c44:
        START(prog_of(e,0xe05ca,W(0x90)));
        SW(0x94,W(0x92));
        k24b1c(a,e);
        child(a,e,0x23fde,0x1c);
        SL(0,b?0x23c7e:0x237da);
        /* fall through */
    case 0x237da: case 0x23c7e: {
        if (W(0x94)) SW(0x94,W(0x94)-1u);
        ga_project(a,e);
        uint32_t x=L(0x3c);
        if (b && W(0x90)==3) {
            if (!(c=creator(a,e))) return GA_UNSUPPORTED;
            uint16_t f=ga_w(c,0x9a),d0=(uint16_t)(f*4u);
            SL(0x50,rl(e,0xe0098u+d0));
            if (f==9u) d0=(uint16_t)(d0-2u);
            SW(0x3c,W(0x3c)+d0);
        }
        st=step(a,e);
        if (st<0) return GA_UNSUPPORTED;
        if (st) {
            START(prog_of(e,0xe05ca,W(0x90)));
            if (k24aec(a)) {
                if (!W(0x94)) {SET(0x8d,1);SL(0,b?0x23dbc:0x23892);}
                else k24b1c(a,e);
            }
        }
        if (b && W(0x90)==3) SL(0x3c,x);
        part_tail(a,e,true,true);
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    }
    case 0x23892: case 0x23dbc: {
        START(prog_of(e,rl(e,0xe05e6u+B(0x9c)),W(0x90)));
        uint32_t p=L(0x72);
        const uint8_t *n=e->vehicle?(p==P1_NODE?e->vehicle->player[0]:p==P2_NODE?e->vehicle->player[1]:NULL):NULL;
        if (!n) STOP(0x238b6);
        SW(0x76,(uint16_t)(n[0x3c]<<8|n[0x3d]));SW(0x78,(uint16_t)(n[0x40]<<8|n[0x41]));
        k24d06(a,e);
        SW(0x98,W(0x98)-1u);
        SL(0,b?0x23dfe:0x238d4);
    }   /* fall through */
    case 0x238d4: case 0x23dfe:
        ga_project(a,e);
        if (b && W(0x90)==3) {
            if (!(c=creator(a,e))) return GA_UNSUPPORTED;
            uint16_t f=ga_w(c,0x9a);
            SL(0x50,rl(e,0xe0098u+(uint16_t)(f*4u)));
            st=step_shifted(a,e,f,2);
        } else st=step(a,e);
        if (st<0) return GA_UNSUPPORTED;
        if (st) SL(0,b?0x23b64:0x23704);
        part_tail(a,e,false,false);
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    }
    STOP(L(0));
}
/* 0240EE / 024164: the part retires (the body's last phase) or is
 * destroyed: the explosion 01C71E, program E069A, deleted with the body's
 * +8D bit 2. */
static ga_result_t part_end_tick(neo_gactor_t *a,neo_genv_t *e) {
    neo_gactor_t *c;int st;
    switch (L(0)) {
    case 0x240ee:
        SW(0x26,0);
        SW(0x94,(uint16_t)(ga_rand_range(e,0x20)+4u));
        SL(0x50,0);SL(0,0x24110);
        /* fall through */
    case 0x24110:
        ga_project(a,e);
        if (W(0x90)==3) {if (!(c=creator(a,e))) return GA_UNSUPPORTED;st=step_shifted(a,e,ga_w(c,0x9a),1);}
        else st=step(a,e);
        if (st<0) return GA_UNSUPPORTED;
        SW(0x94,W(0x94)-1u);
        if (!W(0x94)) SL(0,0x24164);
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    case 0x24164:
        sound(e,0x1021);
        child_at(a,e,0x1c71e,0x2a,0,0);
        SL(0x50,0);
        START(prog_of(e,0xe069a,W(0x90)));
        lost(a,e);
        SB(0x17,0xff);SL(0,0x241cc);
        /* fall through */
    case 0x241cc:
        ga_project(a,e);
        if (!(c=creator(a,e))) return GA_UNSUPPORTED;
        st=W(0x90)==3?step_shifted(a,e,ga_w(c,0x9a),1):step(a,e);
        if (st<0) return GA_UNSUPPORTED;
        if (ga_b(c,0x8d)&4u) return GA_DELETE;                          /* 004188 */
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    }
    STOP(L(0));
}
/* 023EB4: the gun's charge (program E06F2[+9C][+90]); its end or the gun's
 * +8D bit 1 sets the gun's +8D bit 0. 023FDE: the muzzle (E0752[+90]) with
 * the gun's +9C, deleted with the gun's +8D bit 1. */
static ga_result_t muzzle_tick(neo_gactor_t *a,neo_genv_t *e) {
    neo_gactor_t *c=creator(a,e);int st;
    if (!c) return GA_UNSUPPORTED;
    switch (L(0)) {
    case 0x23eb4:
        on_creator(a,e,0,0);
        SB(0x1d,0);ga_palette(a,0xfc);
        SW(0x1e,ga_w(c,0x1e)+8u);SET(0x15,2);
        START(rl(e,rl(e,0xe06f2u+ga_b(c,0x9c))+4u*ga_w(c,0x90)));
        SL(0,0x23f1a);
        /* fall through */
    case 0x23f1a:
        ga_project(a,e);
        st=step(a,e);
        if (st<0) return GA_UNSUPPORTED;
        if (st || (ga_b(c,0x8d)&2u)) {ga_sb(c,0x8d,(uint8_t)(ga_b(c,0x8d)|1u));return GA_DELETE;}
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    case 0x23fde:
        on_creator(a,e,0,0);
        SB(0x1d,0);ga_palette(a,0xfc);
        SW(0x1e,ga_w(c,0x1e)+8u);SET(0x15,2);
        START(prog_of(e,0xe0752,ga_w(c,0x90)));
        SL(0,0x2403a);
        /* fall through */
    case 0x2403a:
        SB(0x9c,ga_b(c,0x9c));
        ga_project(a,e);
        if (step(a,e)<0) return GA_UNSUPPORTED;
        if (ga_b(c,0x8d)&2u) return GA_DELETE;
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    }
    STOP(L(0));
}
/* ---- the treads 022CB6 (part 9): the layer's speed and patches -------------------- */
/* 0251D6: the next entry of the patch sequence +84 (16 bytes: sub-map,
 * wait +94, frame +9A, 002802 speed, D0..D5 of 0C714E on the layer, run
 * by the vertical interrupt through 0036A4); carry at its end (FFFFFFFF). */
static bool k251d6(neo_gactor_t *a,neo_genv_t *e) {
    uint32_t at=L(0x84)+(uint32_t)W(0x88)*16u;
    uint32_t sub=rl(e,at);
    if (sub==0xffffffffu) return true;
    int16_t d[6];for (unsigned i=0;i<6;i++) d[i]=rb(e,at+10u+i);
    SW(0x94,rw(e,at+4));SW(0x9a,rw(e,at+6));
    neo_m4boss_env_t *m=menv(e);
    if (m && m->patch) m->patch(e->ctx,sub,d);
    if (m && m->anim_speed) m->anim_speed(e->ctx,rw(e,at+8));
    SW(0x88,W(0x88)+1u);
    return false;
}
static bool k234de(neo_gactor_t *a,neo_genv_t *e) {
    if (!W(0x94)) return k251d6(a,e);
    SW(0x94,W(0x94)-1u);return false;
}
/* 0234F6: every 0x21 updates the dust lists E04F0 / E0502. */
static void k234f6(neo_gactor_t *a,neo_genv_t *e) {
    if (!W(0x96)) {SW(0x96,0x20);k1da40(a,e,0xe04f0);k1da40(a,e,0xe0502);}
    else SW(0x96,W(0x96)-1u);
}
static void sequence(neo_gactor_t *a,uint32_t table) {SL(0x84,table);SW(0x88,0);SW(0x94,0);}
static void set_speed(neo_genv_t *e,uint32_t x,uint32_t y) {neo_m4boss_env_t *m=menv(e);if (m) {m->speed[0]=(int32_t)x;m->speed[1]=(int32_t)y;}}
/* The body's frame: its +9A from the treads'. */
static void frame_to_body(neo_gactor_t *a,neo_gactor_t *c) {ga_sw(c,0x9a,W(0x9a));}
static ga_result_t tread_tick(neo_gactor_t *a,neo_genv_t *e) {
    neo_gactor_t *c;int st;
    switch (L(0)) {
    case 0x22cb6:
        SB(0x1d,0);ga_palette(a,0xfb);ga_palette(a,0xfc);
        SB(0x14,B(0x19));SET(0x15,2);
        on_creator(a,e,off_x(e,W(0x90)),off_y(e,W(0x90)));
        SW(0x9a,0);SL(0,0x22d08);
        /* fall through */
    case 0x22d08:
        START(prog_of(e,0xe0596,W(0x90)));
        if (!(c=creator(a,e))) return GA_UNSUPPORTED;
        sequence(a,(ga_b(c,0x8d)&2u)?0x225a6:0x224e2);
        ga_sb(c,0x60,0);
        set_speed(e,0,0);
        SL(0,0x22d6c);
        /* fall through */
    case 0x22d6c:
        ga_project(a,e);
        if (step(a,e)<0) return GA_UNSUPPORTED;
        k234de(a,e);
        if (!(c=creator(a,e))) return GA_UNSUPPORTED;
        frame_to_body(a,c);
        if (ga_b(c,0x8c)==1u || ga_b(c,0x8c)==2u) SL(0,0x22e38);
        if (ga_b(c,0x8c)==3u) SL(0,0x2300a);
        if (ga_b(c,0x8b)==7u && ga_b(c,0x8e)) {
            if (!(ga_b(c,0x8d)&2u)) SL(0,0x231a6);
            else if (ga_b(c,0x8e)!=0x7fu) SL(0,0x22d08);
        }
        if (ga_b(c,0x8b)==8u && ga_b(c,0x8e)) SL(0,(ga_b(c,0x8d)&1u)?0x22d08:0x232ec);
        if ((ga_b(c,0x5f)&0x80u) && (ga_b(c,0x8d)&4u)) SL(0,0x23432);
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    case 0x22e38: case 0x2300a: {
        bool back=L(0)==0x2300au;
        sound(e,0x1071);
        if (!(c=creator(a,e))) return GA_UNSUPPORTED;
        sequence(a,(ga_b(c,0x8d)&2u)?(back?0x22612:0x225ba):(back?0x2254e:0x224f6));
        SL(0x7c,0x1000);SL(0x80,0);SL(0x30,0);SL(0x34,0);SW(0x94,0);SW(0x96,0);
        SL(0,back?0x23068:0x22e96);
        if (back) goto back_run;
    }   /* fall through */
    case 0x22e96:
        ga_project(a,e);
        if (step(a,e)<0) return GA_UNSUPPORTED;
        k234de(a,e);
        if (!(c=creator(a,e))) return GA_UNSUPPORTED;
        frame_to_body(a,c);
        if (ga_b(c,0x8c)==2u) {
            SL(0x30,L(0x30)+L(0x7c));
            if ((int32_t)L(0x30)>0x28000) SL(0x30,0x28000);
        } else if (L(0x30)!=0x18000u) {
            uint32_t d0=L(0x7c);if ((int32_t)L(0x30)>0x18000) d0=0u-d0;
            SL(0x30,L(0x30)+d0);
        }
        set_speed(e,L(0x30),L(0x34));
        if (ga_b(c,0x8c)!=1u && ga_b(c,0x8c)!=2u) SL(0,0x22f70);
        if (ga_b(c,0x8b)==7u && ga_b(c,0x8e) && ga_b(c,0x8e)!=0x7fu && !(ga_b(c,0x8d)&2u)) SL(0,0x22f70);
        if (ga_b(c,0x8b)==8u && ga_b(c,0x8e)) SL(0,0x22f70);
        k234f6(a,e);
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    case 0x23068:
    back_run:
        ga_project(a,e);
        if (step(a,e)<0) return GA_UNSUPPORTED;
        k234de(a,e);
        if (!(c=creator(a,e))) return GA_UNSUPPORTED;
        frame_to_body(a,c);
        if (L(0x30)!=0x18000u) {
            uint32_t d0=L(0x7c);if ((int32_t)L(0x30)>0x18000) d0=0u-d0;
            SL(0x30,L(0x30)+d0);
        }
        set_speed(e,0u-L(0x30),L(0x34));
        if (ga_b(c,0x8c)!=3u) SL(0,0x23108);
        if (ga_b(c,0x8b)==7u && ga_b(c,0x8e) && !(ga_b(c,0x8d)&2u)) SL(0,0x23108);
        if (ga_b(c,0x8b)==8u && ga_b(c,0x8e)) SL(0,0x23108);
        k234f6(a,e);
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    case 0x22f70: case 0x23108: {
        bool back=L(0)==0x23108u;
        if (!(c=creator(a,e))) return GA_UNSUPPORTED;
        sequence(a,(ga_b(c,0x8d)&2u)?(back?0x22636:0x225de):(back?0x22572:0x2251a));
        SL(0x7c,0xc00);SL(0x80,0);SW(0x94,0);SW(0x96,0);
        SL(0,back?0x23154:0x22fbc);
    }   /* fall through */
    case 0x22fbc: case 0x23154: {
        bool back=L(0)==0x23154u;
        ga_project(a,e);
        if (step(a,e)<0) return GA_UNSUPPORTED;
        if (k234de(a,e)) SL(0,0x22d08);
        if (!(c=creator(a,e))) return GA_UNSUPPORTED;
        frame_to_body(a,c);
        SL(0x30,L(0x30)-L(0x7c));
        if ((int32_t)L(0x30)<0) SL(0x30,0);
        set_speed(e,back?0u-L(0x30):L(0x30),L(0x34));
        k234f6(a,e);
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    }
    /* The order 7 (the gun 024226 fires) and 8 (part 8): the frames 2349EE
     * and the patch sequences 0223DA / 02245E with their flashes. */
    case 0x231a6:
        if (!(c=creator(a,e))) return GA_UNSUPPORTED;
        ga_sb(c,0x60,0xff);
        START(0x2349ee);SL(0,0x231c6);
        /* fall through */
    case 0x231c6:
        ga_project(a,e);
        if (!(c=creator(a,e))) return GA_UNSUPPORTED;
        frame_to_body(a,c);
        st=step_shifted(a,e,W(0x9a),2);
        if (st<0) return GA_UNSUPPORTED;
        if (st) SL(0,0x23208);
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    case 0x23208:
        sound(e,0x1074);
        START(0x234a7a);
        sequence(a,0x223da);
        child_at(a,e,0x25b6e,0x2a,-0x48,-0x30);                        /* 025B4E */
        child_at(a,e,0x25cde,0x2a,-0x48,-0x30);                        /* 025CBE */
        SL(0,0x23240);
        /* fall through */
    case 0x23240: case 0x23378:
        ga_project(a,e);
        if (k234de(a,e)) SL(0,L(0)==0x23378u?0x233be:0x23286);
        if (!(c=creator(a,e))) return GA_UNSUPPORTED;
        frame_to_body(a,c);
        if (step_shifted(a,e,ga_w(c,0x9a),2)<0) return GA_UNSUPPORTED;
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    case 0x23286: case 0x233be:
        if (L(0)==0x233beu) sound(e,0x1075);
        START(0x234b2c);
        SL(0,L(0)==0x233beu?0x233de:0x2329c);
        /* fall through */
    case 0x2329c: case 0x233de: {
        ga_project(a,e);
        if (!(c=creator(a,e))) return GA_UNSUPPORTED;
        frame_to_body(a,c);
        st=step_shifted(a,e,ga_w(c,0x9a),2);
        if (st<0) return GA_UNSUPPORTED;
        if (st) {
            ga_sb(c,0x60,0);
            if (L(0)==0x2329cu) ga_sb(c,0x8d,(uint8_t)(ga_b(c,0x8d)|2u));
            else {ga_sb(c,0x8d,(uint8_t)(ga_b(c,0x8d)&~2u));ga_sb(c,0x8e,0);}
            SL(0,0x22d08);
        }
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    }
    case 0x232ec:
        START(0x2349ee);
        if (!(c=creator(a,e))) return GA_UNSUPPORTED;
        ga_sb(c,0x60,0xff);SL(0,0x2330c);
        /* fall through */
    case 0x2330c:
        ga_project(a,e);
        if (!(c=creator(a,e))) return GA_UNSUPPORTED;
        frame_to_body(a,c);
        st=step_shifted(a,e,ga_w(c,0x9a),2);
        if (st<0) return GA_UNSUPPORTED;
        if (st) SL(0,0x2334e);
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    case 0x2334e:
        START(0x234ad8);
        sequence(a,0x2245e);
        child_at(a,e,0x25be2,0x2a,-0x48,-0x30);                        /* 025BC2 */
        SL(0,0x23378);
        goto seq_run;
    /* The body destroyed (+5F bit 7 and +8D bit 2): sound 1033, the
     * sequence 02266A, the layer thrown up and back (0C8022). */
    case 0x23432:
        sound(e,0x1033);
        START(prog_of(e,0xe0596,W(0x90)));
        sequence(a,0x2266a);
        set_speed(e,0xfffe0000u,0xffffe000u);
        SW(0x98,1);
        if (e->shake_pulse) e->shake_pulse(e->ctx); else STOP(0xc8022);
        SL(0,0x2348a);
        /* fall through */
    case 0x2348a: {
        ga_project(a,e);
        k234de(a,e);
        neo_m4boss_env_t *m=menv(e);
        if (m) m->speed[0]=-m->speed[0];
        if (!(c=creator(a,e))) return GA_UNSUPPORTED;
        frame_to_body(a,c);
        if (step_shifted(a,e,ga_w(c,0x9a),2)<0) return GA_UNSUPPORTED;
        if (S16(W(0x40))<0x90) {SW(0x40,0x90);if (m) m->speed[1]=0;}
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    }
    }
    STOP(L(0));
seq_run:
    ga_project(a,e);
    if (k234de(a,e)) SL(0,0x233be);
    if (!(c=creator(a,e))) return GA_UNSUPPORTED;
    frame_to_body(a,c);
    if (step_shifted(a,e,ga_w(c,0x9a),2)<0) return GA_UNSUPPORTED;
    return e->unsupported?GA_UNSUPPORTED:GA_MORE;
}
/* ---- the cannon 024226 (part 7) ---------------------------------------------------- */
static ga_result_t cannon_tick(neo_gactor_t *a,neo_genv_t *e) {
    neo_gactor_t *c;int st;
    switch (L(0)) {
    case 0x24226:
        SB(0x1d,0);ga_palette(a,0xf3);ga_flash_palette(a,0x146);
        SET(0x15,2);
        SW(0x4e,0x3000);SW(0x7a,W(0x4e));
        on_creator(a,e,off_x(e,W(0x90)),off_y(e,W(0x90)));
        goto rest;
    case 0x24274:
    again:
        if (!(c=creator(a,e))) return GA_UNSUPPORTED;
        ga_sb(c,0x8e,0);
        /* fall through */
    case 0x2427c:
    rest:
        if (!(c=creator(a,e))) return GA_UNSUPPORTED;
        if (!(ga_b(c,0x8d)&1u)) START(prog_of(e,0xe0596,W(0x90)));
        else START(0x2341e4);
        SB(0x8c,0);SW(0x26,0x100);SL(0x50,0);SL(0,0x242d2);
        /* fall through */
    case 0x242d2:
        ga_project(a,e);
        if (step(a,e)<0) return GA_UNSUPPORTED;
        if (!(c=creator(a,e))) return GA_UNSUPPORTED;
        if (ga_b(c,0x8e)) {
            if (B(0x8b)==ga_b(c,0x8b)) {
                if (ga_b(c,0x8d)&1u) SL(0,0x243e2);
                else if (ga_b(c,0x8d)&2u) SL(0,0x24358);
            }
            if (ga_b(c,0x8b)==8u && (ga_b(c,0x8d)&1u)) SL(0,0x2458c);
        }
        goto tail;
    case 0x24358:
        START(0x234554);
        if (!(c=creator(a,e))) return GA_UNSUPPORTED;
        SB(0x8a,ga_b(c,0x8a));
        SW(0x26,B(0x8a)==2u?0x200:0x100);
        SB(0x9c,0);SL(0,0x24396);
        /* fall through */
    case 0x24396:
        ga_project(a,e);
        if ((st=step(a,e))<0) return GA_UNSUPPORTED;
        if (st) SL(0,k24ab0(a,e)?0x24678:0x243e2);
        goto tail;
    case 0x243e2:
        SW(0x26,0x100);
        if (!(c=creator(a,e))) return GA_UNSUPPORTED;
        ga_sb(c,0x8d,(uint8_t)(ga_b(c,0x8d)|1u));
        SB(0x8a,ga_b(c,0x8a));
        SW(0x98,rank(a,e,ga_b(c,0x8a)==0?0x2c29dc:ga_b(c,0x8a)==1?0x2c2a0e:0x2c2a40));
        /* fall through */
    case 0x24444:
        if (!W(0x98)) goto again;
        START(0x2341e4);
        if (!(c=creator(a,e))) return GA_UNSUPPORTED;
        SL(0x72,ga_l(c,0x72));
        SW(0x92,rank(a,e,ga_b(c,0x8a)==0?0x2c2946:ga_b(c,0x8a)==1?0x2c2978:0x2c29aa));
        SL(0,0x244b8);
        /* fall through */
    case 0x244b8:
        ga_project(a,e);
        if ((st=step(a,e))<0) return GA_UNSUPPORTED;
        if (st) {START(0x2341e4);k24b62(a,e);}
        if (!W(0x92)) {
            if (!BIT(0x9c,0)) {
                if (S16(W(0x3c))>=0x30) SL(0,0x24548);
                else {if (!(c=creator(a,e))) return GA_UNSUPPORTED;ga_sb(c,0x8e,0x7f);}
            }
        } else SW(0x92,W(0x92)-1u);
        if (ga_register(a,e,NULL,NULL)) SB(0x5a,0);
        k24c94(a,e,true);
        if (k24ab0(a,e)) {SL(0x50,0);if (!BIT(0x9c,0)) SL(0,0x2458c);}
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    case 0x24548:
        START(0x23423a);SL(0,0x2455e);
        /* fall through */
    case 0x2455e:
        ga_project(a,e);
        if ((st=step(a,e))<0) return GA_UNSUPPORTED;
        if (st) {SW(0x98,W(0x98)-1u);SL(0,0x24444);}
        if (ga_register(a,e,NULL,NULL)) SB(0x5a,0);
        k24c94(a,e,true);
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    case 0x2458c:
        START(0x2341e4);SB(0x9d,0);SL(0,0x245a6);
        /* fall through */
    case 0x245a6:
        ga_project(a,e);
        if ((st=step(a,e))<0) return GA_UNSUPPORTED;
        if (st) {
            START(0x2341e4);
            if (k24aec(a)) SL(0,k24ab0(a,e)?0x24678:0x2460a);
        }
        goto tail;
    case 0x2460a:
        START(0x2346aa);
        if (!(c=creator(a,e))) return GA_UNSUPPORTED;
        SB(0x8a,ga_b(c,0x8a));
        SW(0x26,B(0x8a)==2u?0x200:0x100);
        SL(0,0x24644);
        /* fall through */
    case 0x24644:
        ga_project(a,e);
        if ((st=step(a,e))<0) return GA_UNSUPPORTED;
        if (st) {if (!(c=creator(a,e))) return GA_UNSUPPORTED;ga_sb(c,0x8d,(uint8_t)(ga_b(c,0x8d)&~1u));SL(0,0x2427c);}
        if (ga_register(a,e,NULL,NULL)) SB(0x5a,0);
        k24c94(a,e,true);
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    case 0x24678:
        SL(0,0x2467e);
        /* fall through */
    case 0x2467e:
        SL(0x50,0);
        ga_project(a,e);
        if (step(a,e)<0) return GA_UNSUPPORTED;
        if (!(c=creator(a,e))) return GA_UNSUPPORTED;
        ga_sb(c,0x8e,0);
        if (ga_b(c,0x8d)&4u) SL(0,0x246ac);
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    case 0x246ac:
        START(0x234912);SL(0,0x246c2);
        /* fall through */
    case 0x246c2:
        ga_project(a,e);
        if (step(a,e)<0) return GA_UNSUPPORTED;
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    }
    STOP(L(0));
tail:
    if (ga_register(a,e,NULL,NULL)) SB(0x5a,0);
    k24c94(a,e,true);
    if (k24ab0(a,e)) SL(0x50,0);
    return e->unsupported?GA_UNSUPPORTED:GA_MORE;
}
/* ---- part 8 0246D0: the body's +60 / +61 ------------------------------------------- */
static ga_result_t hatch_tick(neo_gactor_t *a,neo_genv_t *e) {
    neo_gactor_t *c;
    switch (L(0)) {
    case 0x246d0:
        SB(0x1d,0);ga_palette(a,0xfb);SET(0x15,2);
        on_creator(a,e,off_x(e,W(0x90)),off_y(e,W(0x90)));
        /* fall through */
    case 0x24704:
        START(prog_of(e,0xe0596,W(0x90)));SL(0,0x24726);
        /* fall through */
    case 0x24726:
        ga_project(a,e);
        if (step(a,e)<0) return GA_UNSUPPORTED;
        if (!(c=creator(a,e))) return GA_UNSUPPORTED;
        if (ga_b(c,0x60)) SL(0,0x247dc);
        if (ga_b(c,0x61)) return GA_DELETE;                             /* cartridge check -> 004188 */
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    case 0x247dc:
        START(0x234930);SL(0,0x247f2);
        /* fall through */
    case 0x247f2:
        ga_project(a,e);
        if (step(a,e)<0) return GA_UNSUPPORTED;
        /* With +8C clear A0 is still 00427A's: the node + 0x20 (its movem at
         * 00440C), so the test reads the part's own +81 (trace T: 1027D5). */
        if (!B(0x8c)) {if (B(0x81)) return GA_DELETE;return e->unsupported?GA_UNSUPPORTED:GA_MORE;}
        if (!(c=creator(a,e))) return GA_UNSUPPORTED;
        if (!ga_b(c,0x60)) SL(0,0x24704);
        if (ga_b(c,0x61)) return GA_DELETE;
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    }
    STOP(L(0));
}
/* ---- shots ------------------------------------------------------------------------- */
/* 024DBE / 024E26 / 024E9A (calls of the guns' programs): sound 1066 and the
 * shell 024F0E at the table's offset for +9C, with +72, +76 (+ the table's
 * step) and +78. */
static void shell(neo_gactor_t *a,neo_genv_t *e,uint32_t table,bool step76) {
    sound(e,0x1066);
    neo_gactor_t *c=child(a,e,0x24f0e,0x1c);
    if (!c) return;
    unsigned o=(unsigned)B(0x9c)*2u;
    ga_sb(c,0x9c,(uint8_t)rw(e,table+o+6u));
    int16_t d2=step76?S16(rw(e,table+o+4u)):0;
    ga_place_at(c,a,e,S16(rw(e,table+o)),S16(rw(e,table+o+2u)));
    ga_sl(c,0x72,L(0x72));ga_sw(c,0x76,(uint16_t)(W(0x76)+(uint16_t)d2));ga_sw(c,0x78,W(0x78));
}
/* 02525A: sound 1065, the ball 025362 and its flash 02530E at the offsets
 * 0252EA / 0252C6 for +9C. */
static void cannon_fire(neo_gactor_t *a,neo_genv_t *e) {
    sound(e,0x1065);
    unsigned o=(unsigned)B(0x9c)*4u;
    neo_gactor_t *c=child_at(a,e,0x25362,0x1c,S16(rw(e,0x252eau+o)),S16(rw(e,0x252ecu+o)));
    if (c) ga_sb(c,0x9c,B(0x9c));
    c=child(a,e,0x2530e,0x2a);
    if (c) {ga_sb(c,0x9c,B(0x9c));ga_place_at(c,a,e,S16(rw(e,0x252c6u+o)),S16(rw(e,0x252c8u+o)));}
}
static bool shell_call(neo_gactor_t *a,neo_genv_t *e,neo_anim_kind_t kind,uint32_t routine,uint16_t parameter) {
    (void)parameter;
    if (kind!=NEO_ANIM_CALL) return false;
    switch (routine) {
    case 0x24dbe: shell(a,e,0x24e0e,false);return true;
    case 0x24e26: shell(a,e,0x24e82,true);return true;
    case 0x24e9a: shell(a,e,0x24ef6,true);return true;
    case 0x2525a: cannon_fire(a,e);return true;
    case 0x25252: SET(0x15,0);return true;
    case 0x25132: {                                                    /* the trail 02505C */
        neo_gactor_t *c=child(a,e,0x2505c,0x1c);
        if (!c) return true;
        ga_place_from(c,a,e,0,0);
        ga_sw(c,0x1e,(uint16_t)(W(0x1e)-1u));
        return true;
    }
    case 0x25786: {                                                    /* the casing 0257DC */
        unsigned o=(unsigned)B(0x9c)*4u;
        neo_gactor_t *c=child(a,e,0x257dc,0x1c);
        if (!c) return true;
        ga_sb(c,0x9c,B(0x9c));
        ga_place_at(c,a,e,S16(rw(e,0x257b8u+o)),S16(rw(e,0x257bau+o)));
        return true;
    }
    case 0x258c4: SL(0x50,rl(e,0x258d8u+4u*B(0x9c)));return true;      /* the cannon's hurtbox by +9C */
    case 0x258fc: child_at(a,e,0x2591c,0x2a,0,0);return true;          /* the reflection 02591C */
    case 0x25b4e: child_at(a,e,0x25b6e,0x2a,-0x48,-0x30);return true;
    case 0x25bc2: child_at(a,e,0x25be2,0x2a,-0x48,-0x30);return true;
    case 0x25cbe: child_at(a,e,0x25cde,0x2a,-0x48,-0x30);return true;
    case 0x25d32: case 0x25d4a: case 0x25d62: case 0x25d7a: case 0x25d92: case 0x25daa: {
        neo_gactor_t *c=child(a,e,0x25dc2,0x2a);
        if (c) ga_sw(c,0x90,(uint16_t)((routine-0x25d32u)/0x18u));
        return true;
    }
    }
    return false;
}
static ga_result_t shot_tick(neo_gactor_t *a,neo_genv_t *e) {
    int st;
    switch (L(0)) {
    /* 024F0E: the shell, thrown at +76/+78 in rank 2C28E2 updates under
     * -0x2000 (03B332), palettes 357..359 cycled, 0143E4; it bursts on a
     * player (row E0364), on the ground (+15 bit 7: 022CB60) or when the
     * body falls (024ACC); culled by 0248D8. */
    case 0x24f0e: {
        SB(0x1d,0);
        SW(0x66,0xe0ef);SW(0x1e,0xe0ef);SET(0x15,5);
        ga_project(a,e);
        uint16_t d3=rank(a,e,0x2c28e2);
        uint32_t vx,vy;
        launch((int16_t)(W(0x76)-W(0x3c)),(int16_t)(W(0x78)-W(0x40)),0xffffe000u,d3,&vx,&vy);
        SL(0x30,vx);SL(0x34,vy);SL(0x80,0xffffe000u);
        SW(0x98,0);
        ga_palette(a,0x357);ga_palette(a,0x358);ga_palette(a,0x359);SB(0x14,B(0x19));   /* 025160 */
        START(0x234bb2);SL(0,0x24f80);
    }   /* fall through */
    case 0x24f80:
        if (W(0x98)==0) SB(0x14,B(0x19));                               /* 025192 */
        if (W(0x98)==1) SB(0x14,B(0x1a));
        if (W(0x98)==2) SB(0x14,B(0x1b));
        SW(0x98,W(0x98)+1u);
        if (S16(W(0x98))>2) SW(0x98,0);
        SL(0x34,L(0x34)+L(0x80));
        ga_air(a,e);
        if (step(a,e)<0) return GA_UNSUPPORTED;
        if (ga_attack(a,e,0xe0364)) SL(0,0x2503a);
        if (BIT(0x15,7)) SL(0,0x24fd6);
        if (k24acc(a,e)) SL(0,0x2503a);
        if (ga_cull(a,e,0xe007c)) return GA_DELETE;                     /* 0248D8 */
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    case 0x24fd6:
        if (e->kill_children) e->kill_children(e->ctx,a); else STOP(0x4192);
        SB(0x1d,0);ga_palette(a,0x378);
        SW(0x66,0x1001);SW(0x1e,0x1001);
        START(0x22cb60);
        SW(0x94,0x64);sound(e,0x1022);SL(0,0x25022);
        /* fall through */
    case 0x25022:
        ga_project(a,e);
        if ((st=step(a,e))<0) return GA_UNSUPPORTED;
        if (st) return GA_DELETE;
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    case 0x2503a:
        if (e->kill_children) e->kill_children(e->ctx,a); else STOP(0x4192);
        SW(0x66,0x1001);SW(0x1e,0x1001);
        sound(e,0x1022);
        SL(0,0x1c706);return neo_effects_tick(a,e);
    /* 02505C: the shell's trail (program 234BC8), its creator's +14. */
    case 0x2505c:
        SB(0x1d,0);SW(0x66,0xe0df);SW(0x1e,0xe0df);
        START(0x234bc8);SW(0x98,0);SL(0,0x25088);
        /* fall through */
    case 0x25088: {
        neo_gactor_t *c=creator(a,e);
        if (!c) return GA_UNSUPPORTED;
        SB(0x14,ga_b(c,0x14));
        ga_project(a,e);
        if ((st=step(a,e))<0) return GA_UNSUPPORTED;
        if (st) return GA_DELETE;
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    }
    /* 02530E: the cannon's flash (palette 141, program 22CE6C). */
    case 0x2530e:
        SW(0x66,0xe0ef);SW(0x1e,0xe0ef);SB(0x1d,0);SET(0x15,2);
        ga_palette(a,0x141);
        START(0x22ce6c);SL(0,0x2534a);
        /* fall through */
    case 0x2534a:
        ga_project(a,e);
        if ((st=step(a,e))<0) return GA_UNSUPPORTED;
        if (st) return GA_DELETE;
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    /* 025362: the cannon ball (palettes F4 / 156, program E06B6[+9C]), at
     * rank 2C2A72 / 2C2AA4 / 2C2AD6 speed by the body's +8A, angle 025508[+9C];
     * a trail 025512 every 0x14 of vertical travel; it bursts on a player
     * (E0384[+9C]) or when the body falls, rolls on the ground (0255F4). */
    case 0x25362: {
        SB(0x1d,0);ga_palette(a,0xf4);ga_palette(a,0x156);
        SW(0x66,0xe0ef);SW(0x1e,0xe0ef);SET(0x15,5);
        START(rl(e,0xe06b6u+4u*B(0x9c)));
        uint8_t angle=rb(e,0x25508u+B(0x9c));
        neo_gactor_t *c=creator(a,e);
        if (!c) return GA_UNSUPPORTED;
        uint16_t sp=rank(a,e,ga_b(c,0x8a)==0?0x2c2a72:ga_b(c,0x8a)==1?0x2c2aa4:0x2c2ad6);
        uint32_t vx,vy;ga_polar((uint32_t)((int32_t)(int16_t)sp<<8),angle,&vx,&vy);
        SL(0x30,vx);SL(0x34,vy);
        SW(0x98,0);SL(0x80,0);SL(0,0x2542a);
    }   /* fall through */
    case 0x2542a: {
        uint32_t d0=L(0x34);if (d0&0x80000000u) d0=0u-d0;
        SL(0x80,L(0x80)+d0);
        if ((int32_t)L(0x80)>=0x140000) {SL(0x80,L(0x80)-0x140000u);child_at(a,e,0x25512,0x2a,0,-4);}
        SW(0x90,0xfed);
        ga_air(a,e);
        if (step(a,e)<0) return GA_UNSUPPORTED;
        SW(0x98,W(0x98)+1u);
        SB(0x14,(W(0x98)&1u)?B(0x1a):B(0x19));
        if (ga_attack(a,e,rl(e,0xe0384u+4u*B(0x9c)))) {SW(0x66,0x1001);SW(0x1e,0x1001);sound(e,0x1025);SL(0,0x1c71e);}
        if (BIT(0x15,6)) SL(0,0x255f4);
        if (k24acc(a,e)) {SW(0x66,0x1001);SW(0x1e,0x1001);SL(0,0x1c71e);}
        if (ga_cull(a,e,0xe007c)) return GA_DELETE;                     /* 0248D8 */
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    }
    /* 025512: the ball's trail (palette 276, program 234E0A), depth from the
     * ball's +90 (which it counts down), 01AD8C. */
    case 0x25512:
        SB(0x1d,0);ga_palette(a,0x276);
        START(0x234e0a);SL(0,0x2553c);
        /* fall through */
    case 0x2553c: {
        neo_gactor_t *c=creator(a,e);
        if (!c) return GA_UNSUPPORTED;
        SW(0x1e,ga_w(c,0x90));ga_sw(c,0x90,(uint16_t)(ga_w(c,0x90)-1u));
        ga_project(a,e);
        if ((W(0x66)&0xf000u)==0x8000u) SW(0x1e,W(0x66)-W(0x40));      /* 01AD8C */
        if ((st=step(a,e))<0) return GA_UNSUPPORTED;
        if (st) return GA_DELETE;
        if (ga_cull_rect(a,e,0xe007c)) return GA_DELETE;                /* 024978 */
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    }
    /* 0255F4 -> 0256D4: the ball on the ground (palette 113, program
     * 24855C) is a platform (0140C4, map E006A at +30/+34) rising by +80
     * while +80 >= 0 (-0x14000 per update), released (013F1E) at the end. */
    case 0x255f4:
        SB(0x1d,0);ga_palette(a,0x113);
        START(0x24855c);
        SW(0x66,0x1001);SW(0x1e,0x1001);SW(0x9a,0xfff0);
        SL(0x30,0);SL(0x34,0xfff00000u);SL(0x80,0xf0000);SL(0,0x25728);
        /* fall through */
    case 0x25728:
        ga_project(a,e);
        if ((W(0x66)&0xf000u)==0x8000u) SW(0x1e,W(0x66)-W(0x40));      /* 01AD8C */
        if ((st=step(a,e))<0) return GA_UNSUPPORTED;
        if (st) {if (e->dyn_release) e->dyn_release(e->ctx,a);return GA_DELETE;}
        if ((int32_t)L(0x80)<0) {if (e->dyn_release) e->dyn_release(e->ctx,a);}
        else {
            if (e->dyn_set) e->dyn_set(e->ctx,a,0xe006a,S16(W(0x30)),S16(W(0x34))); else STOP(0x140c4);
            SL(0x34,L(0x34)+L(0x80));SL(0x80,L(0x80)+0xfffec000u);
        }
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    /* 0257DC: the casing (palette F4, program 234840) thrown at 0x10000 by
     * 0258BA[+9C] (facing left up to +9C 4), 0143DC until it lands, then
     * one bounce (+5F bit 3) with -0x53A0 per update (01AC38); 0248D8. */
    case 0x257dc: {
        SB(0x1d,0);ga_palette(a,0xf4);
        SW(0x66,0xe0ef);SW(0x1e,0xe0ef);
        START(0x234840);
        SET(0x15,5);SET(0x15,2);SB(0x61,0);
        if ((int8_t)B(0x9c)<=4) SET(0x15,0);
        uint32_t vx,vy;ga_polar(0x10000,rb(e,0x258bau+B(0x9c)),&vx,&vy);
        SL(0x30,vx);SL(0x34,vy);SL(0x80,L(0x34));SL(0,0x2585c);
    }   /* fall through */
    case 0x2585c:
        if (!B(0x61)) {
            SL(0x34,L(0x34)+0xffffb000u);ga_air(a,e);                   /* 0143DC */
            if (BIT(0x15,6)) {
                CLR(0x15,6);SET(0x5f,3);SB(0x61,0xff);
                SL(0x34,(uint32_t)((int32_t)(0u-L(0x80))>>2));
            } else SL(0x80,L(0x34));
        } else {SL(0x34,L(0x34)+0xffffac60u);ga_move(a,e);}
        if (step(a,e)<0) return GA_UNSUPPORTED;
        if (ga_cull(a,e,0xe007c)) return GA_DELETE;                     /* 0248D8 */
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    }
    STOP(L(0));
}
static ga_result_t parts_tick(neo_gactor_t *a,neo_genv_t *e) {
    uint32_t r=L(0);
    if (r<0x22cb6u) return body_tick(a,e);
    if (r<0x23526u) return tread_tick(a,e);
    if (r<0x23eb4u) return gun_tick(a,e);
    if (r<0x240eeu) return muzzle_tick(a,e);
    if (r<0x24226u) return part_end_tick(a,e);
    if (r<0x246d0u) return cannon_tick(a,e);
    if (r<0x24828u) return hatch_tick(a,e);
    if (r>=0x24f0eu) return shot_tick(a,e);
    STOP(r);
}
bool neo_m4boss_routine(uint32_t r) {return r>=0x2267eu && r<0x25ec0u;}
/* 025B6E / 025BE2 / 025CDE / 025DC2: palettes released, depth 1301,
 * palette 0xFC, +15 bit 2, the program at speed 0x100. */
static bool flash_start(neo_gactor_t *a,neo_genv_t *e,uint32_t program,uint32_t next) {
    SB(0x1d,0);                                                         /* 009F88 */
    SW(0x66,0x1301);SW(0x1e,0x1301);ga_palette(a,0xfc);SB(0x15,B(0x15)|0x04u);
    if (!ga_anim_start(a,e,program,0x100)) return false;
    SL(0,next);return true;
}
ga_result_t neo_m4boss_tick(neo_gactor_t *a,neo_genv_t *e) {
    if (L(0)<0x2591cu) return parts_tick(a,e);
    switch (L(0)) {
    case 0x259f2:
        SB(0x1d,0);                                                     /* 009F88 */
        SW(0x66,0xe0ec);SW(0x1e,0xe0ec);
        ga_palette(a,0xf0);
        if (!ga_anim_start(a,e,0x246ad2,0x100)) return GA_UNSUPPORTED;
        SB(0x15,B(0x15)|0x04u);
        if (!(ga_rand(e)&1u)) SB(0x15,B(0x15)|0x01u);
        SL(0,0x25a42);
        /* fall through */
    case 0x25a42: {
        ga_project(a,e);                                                /* 01ABC8 */
        int st=ga_anim_step(a,e,NULL);                                  /* 00427A */
        if (st<0) return GA_UNSUPPORTED;
        if (st) return GA_DELETE;                                       /* 004188 */
        break;
    }
    case 0x25a5e:
        SB(0x1d,0);                                                     /* 009F88 */
        SW(0x66,0xe0ec);SW(0x1e,0xe0ec);
        ga_palette(a,0x276);
        SL(0x30,0x1000);SL(0x34,0xc000);
        SB(0x15,B(0x15)|0x04u);
        if (!ga_anim_start(a,e,0x234d04,0x100)) return GA_UNSUPPORTED;
        SL(0,0x25aaa);
        /* fall through */
    case 0x25aaa: {
        ga_move(a,e);                                                   /* 01AC38 */
        int st=ga_anim_step(a,e,NULL);
        if (st<0) return GA_UNSUPPORTED;
        if (st) return GA_DELETE;                                       /* 025ABA -> 004188 */
        break;
    }
    /* The flashes made by the boss's program calls (025B4E / 025BC2 / 025CBE
     * at -48, -30 through 01ACF2; 025D32..025DAA with +90 = 0..5): 01ABC8
     * and the program to its end (004188), no cull, except 025DC2: placed
     * on its creator (01AD6E), program +90 of the table E06DA, 024978. */
    case 0x25b6e: if (!flash_start(a,e,0x234c06,0x25baa)) return GA_UNSUPPORTED; /* fall through */
    case 0x25baa: goto flash;
    case 0x25be2: if (!flash_start(a,e,0x234bfc,0x25c1e)) return GA_UNSUPPORTED; /* fall through */
    case 0x25c1e: goto flash;                                           /* its end: the cartridge check 025C2E -> 004188 */
    case 0x25cde: if (!flash_start(a,e,0x234c62,0x25d1a)) return GA_UNSUPPORTED; /* fall through */
    case 0x25d1a:
    flash: {
        ga_project(a,e);
        int st=ga_anim_step(a,e,NULL);
        if (st<0) return GA_UNSUPPORTED;
        if (st) return GA_DELETE;
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    }
    /* 02591C: palette 0xF8, depth 0200, program 2348D4; 025952: 01ABC8 and
     * the program to its end (the cartridge check 025962 -> 004188). */
    case 0x2591c:
        SB(0x1d,0);ga_palette(a,0xf8);                                  /* 009F88, 009EA8 */
        SW(0x66,0x200);SW(0x1e,0x200);
        if (!ga_anim_start(a,e,0x2348d4,0x100)) return GA_UNSUPPORTED;
        SL(0,0x25952);
        /* fall through */
    case 0x25952: goto flash;
    case 0x25dc2: {
        SB(0x1d,0);ga_palette(a,0xfc);                                  /* 009F88, 009EA8 */
        SW(0x66,0x1301);SW(0x1e,0x1301);SB(0x15,B(0x15)|0x04u);
        neo_gactor_t *p=e->creator?e->creator(e->ctx,a):NULL;
        ga_place(a,e,0,0,p?(p->index>=0?NEO_GA_TAG(p->index):L(0x0c)+0x28u):0);   /* 01AD6E: +38 = A0 + 28 (a creator the family test reads from RAM: its address) */
        const uint8_t *t=ga_rom(0xe06dau+(uint32_t)(int32_t)(int16_t)(uint16_t)(W(0x90)<<2));   /* (a1, d0.w) */
        if (!t) {ga_fail(e,0xe06da);return GA_UNSUPPORTED;}
        if (!ga_anim_start(a,e,(uint32_t)t[0]<<24|(uint32_t)t[1]<<16|(uint32_t)t[2]<<8|t[3],0x100)) return GA_UNSUPPORTED;
        SL(0,0x25e18);
    }   /* fall through */
    case 0x25e18: {
        ga_project(a,e);
        int st=ga_anim_step(a,e,NULL);
        if (st<0) return GA_UNSUPPORTED;
        if (st) return GA_DELETE;                                       /* 025E28 -> 004188 */
        break;
    }
    default:
        ga_fail(e,L(0));return GA_UNSUPPORTED;
    }
    if (ga_cull_rect(a,e,0xe007c)) return GA_DELETE;                    /* 024978 */
    return e->unsupported?GA_UNSUPPORTED:GA_MORE;
}
