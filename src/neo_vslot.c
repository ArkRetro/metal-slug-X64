#include "neo_vslot.h"
/* Manual translation of the vehicle-slot services ROM 0AA458..0AABF2
 * (docs/ARMAS_VEHICULOS.md §7); ROM labels are provenance. */
static uint16_t rw(const uint8_t *m,unsigned o) {return (uint16_t)(m[o]<<8|m[o+1]);}
static uint32_t rl(const uint8_t *m,unsigned o) {return (uint32_t)rw(m,o)<<16|rw(m,o+2);}
static void ww(uint8_t *m,unsigned o,uint16_t v) {m[o]=(uint8_t)(v>>8);m[o+1]=(uint8_t)v;}
static void wl(uint8_t *m,unsigned o,uint32_t v) {ww(m,o,(uint16_t)(v>>16));ww(m,o+2,(uint16_t)v);}
void neo_vslot_reset(neo_vslot_t *s) {
    uint8_t *b=s->b;
    ww(b,0,0);b[0x1c]=0;b[0x21]=0;b[0x1f]=0;b[0x20]=4;ww(b,0x14,0);ww(b,0x18,0);ww(b,0x16,0);
}
void neo_vslot_reset_all(neo_vslots_t *v) {neo_vslot_reset(&v->slot[0]);neo_vslot_reset(&v->slot[1]);}
bool neo_vslot_accept(neo_vslots_t *v,unsigned p,uint16_t video,bool halted) {
    uint8_t *b=v->slot[p&1].b;
    if (!rw(b,0)) b[0x1c]=(uint8_t)(b[0x1c]>>1);                        /* 0AA47A */
    else {
        if (b[0x1c] && rw(b,0x12)==video && !halted) {b[0x1c]=0;return true;} /* 0AA4A6 */
        ww(b,0,0);b[0x1c]=1;                                              /* 0AA4B2 */
    }
    v->record[p&1]=NEO_VSLOT_RECORD;return false;                          /* 0AA4BC */
}
void neo_vslot_release(neo_vslots_t *v,unsigned p) {
    uint8_t *b=v->slot[p&1].b;ww(b,0,0);b[0x1c]=2;v->record[p&1]=NEO_VSLOT_RECORD;
}
bool neo_vslot_follow(neo_vslots_t *v,unsigned p,uint8_t *m) {
    const uint8_t *b=v->slot[p&1].b;
    if (!rw(b,0)) return false;
    m[0x15]=(uint8_t)((m[0x15]&~0x40u)|(b[0x22]&0x40u));                 /* 0AA56A */
    wl(m,0x28,((uint32_t)rw(b,2)<<16)+rl(b,0x0a));
    wl(m,0x2c,((uint32_t)rw(b,4)<<16)+rl(b,0x0e));
    wl(m,0x30,0);wl(m,0x34,0);wl(m,0x38,rl(b,6));                         /* 0AA59E movem */
    return true;
}
void neo_vslot_exit_place(neo_vslots_t *v,unsigned p,uint8_t *m,uint8_t *kind,uint8_t *cause) {
    const uint8_t *b=v->slot[p&1].b;
    wl(m,0x38,rl(b,6));wl(m,0x28,rl(b,0x0a));wl(m,0x2c,rl(b,0x0e));
    wl(m,0x30,(uint32_t)rw(b,2)<<16);wl(m,0x34,(uint32_t)rw(b,4)<<16);
    *kind=b[0x1d];*cause=b[0x1e];
}
/* 0C0556: d0 = 0 when player p may mount (else FF). */
static bool may_mount(const neo_vslot_player_t *pl,bool system) {
    if (system || !pl->exists) return false;
    const uint8_t *m=pl->m;
    if (m[0x5f]&0x80u) return false;
    if (m[0x93]==3) return false;
    if (!(m[0x34]&0x80u)) return false;                                   /* tst.l +34; bpl */
    if (m[0x15]&0x40u) return false;
    return true;
}
/* 0AA6B6 (with 0C0556): carry when player p mounts. */
static bool candidate(const neo_vslots_t *v,unsigned p,const uint8_t *veh,const neo_vslot_player_t *pl,const int16_t rect[4],bool system) {
    if (!may_mount(pl,system)) return false;
    const uint8_t *b=v->slot[p].b;
    if (rw(b,0) || b[0x1c]) return false;
    const uint8_t *m=pl->m;
    uint16_t d0=(uint16_t)(rw(m,0x3c)-rw(veh,0x3c)),d1=(uint16_t)(rw(m,0x44)-rw(veh,0x44));
    if (veh[0x15]&1u) {d0=(uint16_t)-d0;d1=(uint16_t)-d1;}
    d0=(uint16_t)(d0-(uint16_t)rect[0]);d1=(uint16_t)(d1-(uint16_t)rect[0]);
    uint16_t w=(uint16_t)rect[2];
    if (d0>=w) {                                                         /* 0AA71C bcs not taken */
        d0=(uint16_t)(d0-w);d1=(uint16_t)(d1-w);
        if (!((d0^d1)&0x8000u)) return false;
    }
    d0=(uint16_t)(rw(m,0x40)-rw(veh,0x40));d1=(uint16_t)(rw(m,0x48)-rw(veh,0x48));
    d0=(uint16_t)(d0-(uint16_t)rect[1]);d1=(uint16_t)(d1-(uint16_t)rect[1]);
    uint16_t h=(uint16_t)rect[3];
    if (d0>=h) {
        d0=(uint16_t)(d0-h);d1=(uint16_t)(d1-h);
        if (!((d0^d1)&0x8000u)) return false;
    }
    return true;
}
int neo_vslot_select(const neo_vslots_t *v,const uint8_t *veh,const neo_vslot_player_t players[2],
    const int16_t rect[4],bool halted,bool system,uint16_t (*rng)(void *),void *ctx) {
    if (halted) return -1;                                               /* 018F24 */
    bool c0=candidate(v,0,veh,&players[0],rect,system),c1=candidate(v,1,veh,&players[1],rect,system);
    if (!c0 && !c1) return -1;
    if (!c0) return 1;
    if (!c1) return 0;
    uint16_t x=rw(veh,0x28);                                             /* 0AA670 */
    uint16_t d0=(uint16_t)(rw(players[0].m,0x28)-x),d1=(uint16_t)(rw(players[1].m,0x28)-x);
    if (d0&0x8000u) d0=(uint16_t)-d0;
    if (d1&0x8000u) d1=(uint16_t)-d1;
    if (d0==d1) return rng(ctx)&1;                                       /* 0029C6 */
    return d1<d0?1:0;
}
bool neo_vslot_bind(neo_vslots_t *v,unsigned p,uint8_t *veh,uint16_t node,const neo_vslot_player_t *pl,
    uint16_t video,int16_t d2,int16_t d3,uint16_t *player6e) {
    uint8_t *b=v->slot[p&1].b;
    *player6e=0;
    if (!pl->exists || (pl->m[0x5f]&0x80u)) return false;
    if (rw(b,0) && rw(b,0)!=node) return false;
    if (b[0x1c]) return false;
    ww(b,0,node);b[0x1c]=0xff;ww(b,0x12,video);ww(b,2,(uint16_t)d2);ww(b,4,(uint16_t)d3);
    wl(b,6,rl(veh,0x38));wl(b,0x0a,rl(veh,0x28));wl(b,0x0e,rl(veh,0x2c));
    if (veh[0x5a]) b[0x1e]=veh[0x5a];
    veh[0x5e]=pl->m[0x5e];
    b[0x1d]=1;b[0x22]=veh[0x15];
    *player6e=rw(pl->m,0x6e);
    return true;
}
void neo_vslot_exit(neo_vslots_t *v,unsigned p,const uint8_t *veh,uint16_t node,uint8_t kind) {
    /* 0AA80E names player 1's node for P2 too (A0 is not used after). */
    uint8_t *b=v->slot[p&1].b;
    if (rw(b,0)!=node) return;
    ww(b,0,0);b[0x1c]=2;b[0x1d]=kind;
    if (veh[0x5a]) b[0x1e]=veh[0x5a];
}
void neo_vslot_mount(neo_vslots_t *v,unsigned p,const uint8_t *veh,uint8_t kind,uint8_t shells,uint8_t type,uint16_t d4) {
    uint8_t *b=v->slot[p&1].b;
    ww(b,0x18,rw(veh,0x4e));ww(b,0x16,0);
    if (shells>=0x64u) shells=0x63;
    b[0x21]=kind;b[0x1f]=shells;b[0x20]=type;ww(b,0x14,d4);
    v->record[p&1]=kind==4?NEO_VSLOT_RECORD_KIND4:NEO_VSLOT_RECORD;
}
bool neo_vslot_shell(neo_vslots_t *v,unsigned p,bool unlimited) {
    if (unlimited) return true;
    uint8_t *b=v->slot[p&1].b;
    if (!b[0x1f]) return false;
    b[0x1f]--;return true;
}
static void add_pending(uint8_t *b) {ww(b,0x18,(uint16_t)(rw(b,0x18)+rw(b,0x16)));ww(b,0x16,0);}
void neo_vslot_damage_flag(neo_vslots_t *v,unsigned p,uint8_t *veh) {
    uint8_t *b=v->slot[p&1].b;
    bool was=veh[0x5f]&0x80u;veh[0x5f]&=0x7f;
    if (was && !(veh[0x5f]&0x40u)) ww(b,0x16,0);
    add_pending(b);
    ww(veh,0x4e,rw(b,0x18));
    if (!rw(b,0x18)) veh[0x5f]|=0x80u;
}
bool neo_vslot_damage(neo_vslots_t *v,unsigned p,uint8_t *veh,bool hit) {
    uint8_t *b=v->slot[p&1].b;
    bool skip_add=false;
    if (hit) {
        if (veh[0x5a]==0x0f) {ww(b,0x18,0);veh[0x5f]|=0x80u;skip_add=true;}         /* 0AAA0E */
        else if (veh[0x5a]==0x0d) ww(b,0x18,rw(veh,0x4e));                         /* 0AAA28 */
        else {
            veh[0x5f]&=0x7f;                                                       /* 0AAA32 */
            if (!(veh[0x5f]&0x40u)) {
                uint16_t h=rw(b,0x18);
                if (h>0x10u) ww(b,0x18,(uint16_t)(h-0x10u));                       /* bhi */
                else {ww(b,0x18,0);veh[0x5f]|=0x80u;skip_add=true;}
            }
        }
    }
    if (!skip_add) add_pending(b);                                                 /* 0AAA62 */
    ww(veh,0x4e,rw(b,0x18));                                                       /* 0AAA74 */
    return veh[0x5a]!=0;
}
int neo_vslot_which(const neo_vslots_t *v,uint16_t node) {
    if (rw(v->slot[0].b,0)==node) return 0;
    if (rw(v->slot[1].b,0)==node) return 1;
    return -1;
}
int neo_vslot_rider(const neo_vslots_t *v,uint16_t node,const neo_vslot_player_t players[2]) {
    for (unsigned p=0;p<2;p++) {
        const uint8_t *b=v->slot[p].b;
        if (players[p].exists && rw(b,0)==node && b[0x1c]) return (int)p;
    }
    return -1;
}
void neo_vslot_angle(neo_vslots_t *v,unsigned p,uint16_t angle) {ww(v->slot[p&1].b,0x1a,angle);}
bool neo_vslot_ridden(const neo_vslots_t *v,unsigned p,bool exists,uint8_t *kind,uint8_t *shells,uint8_t *type,uint16_t *health,uint16_t *d4) {
    const uint8_t *b=v->slot[p&1].b;
    *kind=0;*shells=0;*type=0;*health=0;*d4=0;
    if (!exists || !rw(b,0)) return true;
    if (!b[0x21]) return false;                                          /* 0AAACC TRAP #F */
    *kind=b[0x21];*shells=b[0x1f];*type=b[0x20];*health=rw(b,0x18);*d4=rw(b,0x14);
    return true;
}
const uint8_t neo_vslot_masks[4]={0x10,0x20,0x40,0x30};             /* 0CA1B0 */
uint8_t neo_vslot_route(uint8_t held,uint8_t pressed,const uint8_t masks[4],uint8_t *fresh) {
    uint8_t d7=(uint8_t)(held&0x0fu),d6=d7;
    for (unsigned i=0;i<4;i++) {
        uint8_t d4=(uint8_t)(held&masks[i]);
        if (d4!=masks[i]) continue;
        d7|=(uint8_t)(0x10u<<i);
        if (d4&pressed) d6|=(uint8_t)(0x10u<<i);
    }
    if ((masks[2]&masks[1]) && (d7&0x40u)) {d7&=(uint8_t)~0x20u;d6&=(uint8_t)~0x20u;}
    *fresh=d6;return d7;
}
bool neo_vslot_gauge(uint8_t *slot,uint16_t d1,uint16_t *health,uint16_t *max) {
    *health=0;*max=0;
    if (!slot || !(slot[0]|slot[1])) return false;                      /* 0AAAF2 */
    uint8_t k=slot[0x21];
    *health=(uint16_t)(slot[0x18]<<8|slot[0x19]);*max=(uint16_t)(slot[0x14]<<8|slot[0x15]);
    if (!(k==1||k==3||k==4||k==8||k==9||k==0xa||k==0xb)) return false;  /* 0AAB34 */
    if (d1) {                                                           /* 0AAB42 */
        if (*health>=*max) return false;                                /* 0AAB52 bcc */
        uint16_t v=(uint16_t)((slot[0x16]<<8|slot[0x17])+d1);slot[0x16]=(uint8_t)(v>>8);slot[0x17]=(uint8_t)v;
    }
    return true;
}
