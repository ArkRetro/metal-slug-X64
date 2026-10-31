#include "neo_region.h"
/* The two-point tread body of the SV-001 (ROM 014A4E..0153EE): manual
 * translation; ROM labels are provenance only.
 *
 * 014AE8 creates the tread node 014A4E after the vehicle (0040BC; its +7C/
 * +80 and +74/+78 the vehicle's previous projected position). Each update
 * of the vehicle runs one of the entries 014C0C (air, 0128E8 and 004ED2),
 * 014C2E (air, 0128DA), 014C48 (ground, 0128E8, 004ED2), 014C6A (ground,
 * 0128DA): the vehicle's surface under it (013C1A) gives the mode 10C004
 * (table 014522; stick down +5F bit 4: FFFFFFFE; up, bit 5: 0); the tread
 * node takes the vehicle's motion (+30/+34 from the previous projected
 * position), its front point (+74/+78, 0x17 px ahead along the angle +6E)
 * and its rear point (+7C/+80, 0x17 px behind) are swept with 013968
 * (014B12: from the centre to the previous point, then to the new one), the
 * angle +6E follows +90 with the spring 014B90, and the vehicle is swept
 * from its previous position to the midpoint of the points (013968, A0 = 0),
 * taking the angle, the ground bit and the wall bit; 10C2AC..10C2B8 are the
 * points' corrections (0153D2 resets the node in the air). */
#define B(o) ga_b(a,(o))
#define W(o) ga_w(a,(o))
#define L(o) ga_l(a,(o))
#define SB(o,v) ga_sb(a,(o),(uint8_t)(v))
#define SW(o,v) ga_sw(a,(o),(uint16_t)(v))
#define SL(o,v) ga_sl(a,(o),(uint32_t)(v))
#define CB(o) ga_b(c,(o))
#define CW(o) ga_w(c,(o))
#define CL(o) ga_l(c,(o))
static int32_t S32(uint32_t v) {return (int32_t)v;}
static int16_t S16(uint16_t v) {return (int16_t)v;}
static uint32_t asr(uint32_t v,unsigned n) {return (uint32_t)(S32(v)>>n);}
/* 002E98: (D0 >> 8).w times cos/sin of the angle word's high byte. */
static void k2e98(uint32_t d0,uint16_t angle,uint32_t *x,uint32_t *y) {
    int16_t m=(int16_t)(uint16_t)asr(d0,8);uint8_t i=(uint8_t)(angle>>8);
    *x=(uint32_t)((int32_t)m*ga_cosine(i));*y=(uint32_t)((int32_t)m*ga_sine(i));
}
/* 002EB4: the angle of the vector (x, y) (tables 002C7C/002D7C). */
static bool k2eb4(neo_genv_t *e,int16_t x,int16_t y,uint8_t *out) {
    const uint8_t *t=ga_rom(0x2c7c);
    if (!t) {ga_fail(e,0x2c7c);return false;}
    uint16_t d0=(uint16_t)(x>>1),d1=(uint16_t)(y>>1);
    uint8_t d2=(d0&0x8000u)?0xff:0,d3=(d1&0x8000u)?0xff:0,r;
    d0=(uint16_t)((d0&0xff00u)|(uint8_t)(((uint8_t)d0^d2)-d2));
    d1=(uint16_t)((d1&0xff00u)|(uint8_t)(((uint8_t)d1^d3)-d3));
    d0&=0xffu;
    if (!d0) r=0x40;
    else {
        d1&=0xffu;
        if (!d1) r=0;
        else {
            unsigned v=(unsigned)t[d1]-(unsigned)t[d0];
            if (v>0xffu) r=t[0x100+(v&0xffu)];
            else r=(uint8_t)(0x40u-t[0x100+(uint8_t)~v]);
        }
    }
    d3^=d2;r=(uint8_t)((r^d3)-d3);r=(uint8_t)(r+(d2&0x80u));
    *out=r;return true;
}
/* 014B6E: the angle word of the vector (d0, d1) (Q16.16). */
static bool k14b6e(neo_genv_t *e,uint32_t d0,uint32_t d1,uint16_t *angle) {
    uint16_t x=(uint16_t)((d0<<3)>>16),y=(uint16_t)((d1<<3)>>16);
    if (x&0x8000u) x=(uint16_t)(x-2u);
    if (y&0x8000u) y=(uint16_t)(y-2u);
    uint8_t r;if (!k2eb4(e,(int16_t)x,(int16_t)y,&r)) return false;
    *angle=(uint16_t)(r<<8);return true;
}
/* 014B90 (tread node): the angle +6E follows +90 with the speed +70. */
static void k14b90(neo_gactor_t *c) {
    uint16_t d7=CW(0x90),d1=CW(0x70),d2=(uint16_t)S16((uint16_t)(S16(d1)>>8));
    d1=(uint16_t)(d1-d2);
    uint32_t v=CL(0x62)<<4;d2=(uint16_t)(v>>16);
    if (d2&0x8000u) d2=(uint16_t)(0u-d2);
    d2=(uint16_t)(d2+0xa0u);
    uint16_t d0=CW(0x6e);d7=(uint16_t)(d7-d0);
    if (!(d7&0x8000u)) {
        d1=(uint16_t)(d1+d2);if (d1&0x8000u) d1=0;
        if (!((uint16_t)(d1-d7)&0x8000u)) d1=d7;
    } else {
        d1=(uint16_t)(d1-d2);if (!(d1&0x8000u)) d1=0;
        if ((uint16_t)(d1-d7)&0x8000u) d1=d7;
    }
    d0=(uint16_t)(d0+d1);
    if (S16(d0)>0x2000) {d0=0x2000;d1=0;}
    else if (S16(d0)<S16(0xe000)) {d0=0xe000;d1=0;}
    ga_sw(c,0x70,d1);ga_sw(c,0x6e,d0);
}
typedef struct { uint32_t m4004; uint8_t b4008,b4009,b400a,b400b,b400d; } modes_t;
static const neo_kobj_t *kobj_pos(neo_genv_t *e,uint8_t id) {
    neo_kscene_t *s=e->kernel;
    if (id>=NEO_KOBJ_DYN0 && id<NEO_KOBJ_DYN0+NEO_KDYN_COUNT) return &s->dyn[id-NEO_KOBJ_DYN0].pos;
    return id==NEO_KOBJ_TERRAIN?&s->terrain:id==NEO_KOBJ_ROOT?&s->root:NULL;
}
/* 013968 with A0 = 0 (absolute coordinates): the end point made absolute
 * again (014B2C), its flags D6, kind D7 and object. */
static bool sweep_abs(neo_gactor_t *self,neo_genv_t *e,const modes_t *m,uint32_t x,uint32_t y,uint32_t dx,uint32_t dy,
    uint32_t *ox,uint32_t *oy,neo_kres_t *r) {
    ga_self(self,e);
    neo_kreq_t q={S32(x),S32(y),S32(dx),S32(dy),NEO_KOBJ_NONE,m->m4004,m->b4008,m->b4009,m->b400a,m->b400b,m->b400d};
    if (!neo_kernel_sweep(e->kernel,&q,r)) {ga_fail(e,0x13968);return false;}
    *ox=(uint32_t)r->x;*oy=(uint32_t)r->y;
    const neo_kobj_t *o=kobj_pos(e,r->parent);
    if (o) {*ox+=(uint32_t)o->x;*oy+=(uint32_t)o->y;}
    return true;
}
/* 014B12 (tread node): from its centre to the previous point (d2,d3), then
 * to the new point (d0,d1); D7 angle word - 0x4000. */
static bool k14b12(neo_gactor_t *c,neo_genv_t *e,const modes_t *m,uint32_t d0,uint32_t d1,uint32_t d2,uint32_t d3,
    uint32_t *x,uint32_t *y,uint16_t *d6,uint32_t *d7) {
    uint32_t cx=CL(0x3c),cy=CL(0x40),px,py;neo_kres_t r;
    if (!sweep_abs(c,e,m,cx,cy,d2-cx,d3-cy,&px,&py,&r)) return false;
    uint32_t tx=d0+cx-px,ty=d1+cy-py;
    if (!sweep_abs(c,e,m,px,py,tx,ty,x,y,&r)) return false;
    *d6=r.flags;*d7=(r.kind&0xffff0000u)|(uint16_t)(r.kind-0x4000u);
    return true;
}
/* 0128E8 / 0128DA: RAM 10C00D (and the screen box of the rider's record,
 * neo_vslot) for the sweeps; 10C00A cleared. */
static void config(neo_gactor_t *a,neo_genv_t *e) {
    uint16_t node=e->node_word?e->node_word(e->ctx,a):(uint16_t)(0x8000u|(uint16_t)a->index);
    int p=e->vehicle?neo_vslot_which(e->vehicle->slots,node):-1;
    uint32_t rec=p<0?NEO_VSLOT_RECORD:e->vehicle->slots->record[p];
    for (unsigned i=0;i<4;i++) {const uint8_t *q=ga_rom(rec+2u*i);e->kernel->config[i]=q?(uint16_t)(q[0]<<8|q[1]):0;}
}
/* 004ED2 (after 0128E8, 10C004 = 0, 10C009 by the entry): the push out
 * of the solid bodies (ga_push_solids). */
static bool k4ed2(neo_gactor_t *a,neo_genv_t *e,uint8_t b4009) {return ga_push_solids(a,e,b4009,NULL);}
neo_gactor_t *neo_tread_spawn(neo_gactor_t *a,neo_genv_t *e) {   /* 014AE8 */
    ga_project(a,e);
    neo_gactor_t *c=e->spawn?e->spawn(e->ctx,a,0x14a4e,false):NULL;
    if (!c) return NULL;
    ga_sl(c,0x7c,L(0x44));ga_sl(c,0x80,L(0x48));ga_sl(c,0x74,L(0x44));ga_sl(c,0x78,L(0x48));
    return c;
}
void neo_tread_air_reset(neo_gactor_t *c,uint16_t d0) {     /* 0153D2 */
    ga_sw(c,0x90,d0);ga_sb(c,0x92,0);ga_sb(c,0x93,0);ga_sb(c,0x94,0);ga_sw(c,0x8a,0);
}
/* entry: 0 014C0C, 1 014C2E, 2 014C48, 3 014C6A. Returns the angle word D0
 * (the callers' +84) or -1 on an explicit stop. */
int32_t neo_tread_step(neo_gactor_t *a,neo_genv_t *e,unsigned entry) {
    neo_gactor_t *c=e->actor?e->actor(e->ctx,L(0x8c)):NULL;
    if (!c) {ga_fail(e,0x14c0c);return -1;}
    modes_t m={0,0,0,0,0,0};
    bool ground=entry>=2;
    if (entry==0 || entry==2) {
        config(a,e);m.b400d=0xff;m.b400a=0;m.m4004=0;m.b4009=ground?0xff:0;
        if (!k4ed2(a,e,m.b4009)) return -1;
    } else {m.b400d=0;m.b400a=0;}
    if (!ground) SL(0x62,L(0x30));
    uint8_t g42aa=ground?0xff:0;
    uint32_t saved62=L(0x62);
    if (B(0x15)&8u) {SB(0x15,B(0x15)&~8u);if (!(L(0x62)&0x80000000u)) SL(0x62,0);}
    if (B(0x15)&16u) {SB(0x15,B(0x15)&~16u);if (L(0x62)&0x80000000u) SL(0x62,0);}
    uint32_t c42ac=0,c42b0=0,c42b4=0,c42b8=0;
    m.m4004=0;m.b4008=0xff;
    {   /* 013C1A at the vehicle */
        uint8_t parent=ga_kobj(L(0x38)),object;uint16_t d5;uint32_t d6,d7;
        if (parent==0xffu) {ga_fail(e,0x13c1a);return -1;}
        ga_self(a,e);
        if (!neo_kernel_probe(e->kernel,S32(L(0x28)),S32(L(0x2c)),parent,m.b4008,m.b400a,m.b400d,&d5,&d6,&d7,&object)) {ga_fail(e,0x13c1a);return -1;}
        if (B(0x5f)&16u) m.m4004=0xfffffffeu;
        else if (!(B(0x5f)&32u)) {
            const uint8_t *t=ga_rom(0x14522u+(uint32_t)(int32_t)(int16_t)d5);
            if (!t) {ga_fail(e,0x14522u+(uint32_t)(int32_t)(int16_t)d5);return -1;}
            m.m4004=(uint32_t)t[0]<<24|(uint32_t)t[1]<<16|(uint32_t)t[2]<<8|t[3];
        }
    }
    ga_project(a,e);
    /* 014D14: the tread node (A6), the vehicle in A4 */
    ga_sl(c,0x38,0);ga_sl(c,0x28,L(0x44));ga_sl(c,0x2c,L(0x48));
    uint32_t d2=g42aa?0:L(0x62),d3=g42aa?0:L(0x34);
    d2=d2-L(0x44)+L(0x3c);d3=d3-L(0x48)+L(0x40);
    ga_sl(c,0x30,d2);ga_sl(c,0x34,d3);ga_project(c,e);
    ga_sb(c,0x15,B(0x15));ga_sb(c,0x5f,B(0x5f));ga_sl(c,0x62,L(0x62));
    if (CW(0x8a)) {uint32_t d1=CL(0x8c)+0x8000u;ga_sl(c,0x8c,d1);ga_sl(c,0x34,CL(0x34)-d1);}
    uint16_t sel=(uint16_t)(((CB(0x93)&5u)?0xff00u:0)|((CB(0x92)&5u)?0x00ffu:0));
    uint32_t x,y;
    if (!sel) {
        ga_sw(c,0x72,CW(0x88));
        if (CB(0x94)&5u) goto k15068;
        k14b90(c);goto k15076;
    }
    if (sel==0xffffu) {
        uint16_t ang;if (!k14b6e(e,CL(0x74)-CL(0x7c),CL(0x78)-CL(0x80),&ang)) return -1;
        ga_sw(c,0x90,ang);
        uint16_t d0=(uint16_t)(CW(0x84)-CW(0x86));
        if (!((uint16_t)(d0-0x1000u)&0x8000u) && CL(0x62)) ga_sw(c,0x90,(CL(0x62)&0x80000000u)?CW(0x86):CW(0x84));
        k14b90(c);ga_sw(c,0x72,CW(0x6e));
        uint16_t d1=(uint16_t)(CW(0x84)-CW(0x86));
        if (d1&0x8000u) {
            k2e98(0x170000u,CW(0x6e),&x,&y);
            x+=CL(0x7c);y+=CL(0x80);y-=CL(0x40);x-=CL(0x3c);c42b4+=x;c42b8+=y;
            k2e98(0xffe90000u,CW(0x6e),&x,&y);
            x+=CL(0x74);y+=CL(0x78);x-=CL(0x3c);y-=CL(0x40);c42b4+=x;c42b8+=y;
        } else {
            d1=(uint16_t)(S16(d1)>>1);d1=(uint16_t)(d1+CW(0x86)-0x4000u);
            k2e98(0x10000u,d1,&x,&y);
            c42ac+=x;c42b0+=y;c42b4+=x;c42b8+=y;
        }
        goto k15076;
    }
    if (sel==0x00ffu) {
        ga_sw(c,0x72,CW(0x84));
        if (S32(CL(0x62))>0) {
            if (CB(0x94)&4u) goto k15068;
            ga_sw(c,0x90,CW(0x84));
        } else {
            ga_sw(c,0x90,0x2000);
            if (CW(0x6e)==0x2000u) {ga_sl(c,0x74,CL(0x3c));ga_sl(c,0x78,CL(0x40));ga_sw(c,0x34,CW(0x34)-1u);goto k15076;}
        }
        if (CB(0x94)&5u) {
            if (!((uint16_t)(CW(0x88)-CW(0x84))&0x8000u)) goto k15068;
            uint16_t ang;if (!k14b6e(e,CL(0x74)-CL(0x3c),CL(0x78)-CL(0x40),&ang)) return -1;
            ga_sw(c,0x90,ang);k14b90(c);
            k2e98(0xffe90000u,CW(0x6e),&x,&y);
            x+=CL(0x74);y+=CL(0x78);x-=CL(0x3c);y-=CL(0x40);c42ac+=x;c42b0+=y;
        } else {
            k14b90(c);
            k2e98(0x170000u,CW(0x6e),&x,&y);
            x-=CL(0x74);y-=CL(0x78);x+=CL(0x3c);y+=CL(0x40);
            ga_sl(c,0x30,CL(0x30)-x);ga_sl(c,0x34,CL(0x34)-y);
        }
        goto k15076;
    }
    if (sel==0xff00u) {
        ga_sw(c,0x72,CW(0x86));
        if (!(CL(0x62)&0x80000000u)) {
            ga_sw(c,0x90,0xe000);
            if (CW(0x6e)==0xe000u) {ga_sl(c,0x7c,CL(0x3c));ga_sl(c,0x80,CL(0x40));ga_sw(c,0x34,CW(0x34)-1u);goto k15076;}
        } else {
            if (CB(0x94)&4u) goto k15068;
            ga_sw(c,0x90,CW(0x86));
        }
        if (CB(0x94)&5u) {
            if (!((uint16_t)(CW(0x86)-CW(0x88))&0x8000u)) goto k15068;
            uint16_t ang;if (!k14b6e(e,CL(0x3c)-CL(0x7c),CL(0x40)-CL(0x80),&ang)) return -1;
            ga_sw(c,0x90,ang);k14b90(c);
            k2e98(0x170000u,CW(0x6e),&x,&y);
            x+=CL(0x7c);y+=CL(0x80);x-=CL(0x3c);y-=CL(0x40);c42b4+=x;c42b8+=y;
        } else {
            k14b90(c);
            k2e98(0xffe90000u,CW(0x6e),&x,&y);
            x-=CL(0x7c);y-=CL(0x80);x+=CL(0x3c);y+=CL(0x40);
            ga_sl(c,0x30,CL(0x30)-x);ga_sl(c,0x34,CL(0x34)-y);
        }
        goto k15076;
    }
k15068:
    ga_sw(c,0x90,CW(0x88));k14b90(c);
k15076:
    if (g42aa) {
        k2e98(CL(0x62),CW(0x72),&x,&y);
        ga_sl(c,0x30,CL(0x30)+x);ga_sl(c,0x34,CL(0x34)+y);
        if (CW(0x8a) && CL(0x62)) {
            uint32_t h=asr(x,1);
            if (CL(0x62)&0x80000000u) c42b8+=h; else c42b0-=h;
        }
    }
    {
        modes_t pm=m;pm.b4009=0;pm.b400b=0xff;
        uint16_t d6;uint32_t d7;
        /* front point */
        modes_t fm=pm;
        if (S32(CL(0x62))>0) {
            if ((CB(0x94)|CB(0x92))&0x20u) {ga_sl(c,0x74,CL(0x3c));ga_sl(c,0x78,CL(0x40));}
        } else fm.m4004=0xfffffffeu;
        k2e98(0x170000u,CW(0x6e),&x,&y);
        x+=c42ac+CL(0x30);y+=c42b0+CL(0x34);
        uint32_t px,py;
        if (!k14b12(c,e,&fm,x,y,CL(0x74),CL(0x78),&px,&py,&d6,&d7)) return -1;
        ga_sl(c,0x74,px);ga_sl(c,0x78,py);ga_sb(c,0x92,(uint8_t)d6);
        if ((d6&4u) && CL(0x62)) d7=(d7&0xffff0000u)|((CL(0x62)&0x80000000u)?0xe000u:0x2000u);
        ga_sw(c,0x84,(uint16_t)d7);
        /* rear point */
        modes_t rm=pm;
        if (!(CL(0x62)&0x80000000u)) rm.m4004=0xfffffffeu;
        else if ((CB(0x94)|CB(0x93))&0x20u) {ga_sl(c,0x7c,CL(0x3c));ga_sl(c,0x80,CL(0x40));}
        k2e98(0xffe90000u,CW(0x6e),&x,&y);
        x+=c42b4+CL(0x30);y+=c42b8+CL(0x34);
        if (!k14b12(c,e,&rm,x,y,CL(0x7c),CL(0x80),&px,&py,&d6,&d7)) return -1;
        ga_sl(c,0x7c,px);ga_sl(c,0x80,py);ga_sb(c,0x93,(uint8_t)d6);
        if ((d6&4u) && CL(0x62)) d7=(d7&0xffff0000u)|((CL(0x62)&0x80000000u)?0xe000u:0x2000u);
        ga_sw(c,0x86,(uint16_t)d7);
    }
    if (!(B(0x15)&0x40u)) {
        uint16_t d7=(uint16_t)asr(CL(0x34),7);
        if (CB(0x92)&5u) ga_sw(c,0x70,CW(0x70)-d7);
        if (CB(0x93)&5u) ga_sw(c,0x70,CW(0x70)+d7);
    }
    {   /* 015220: the vehicle to the midpoint (A6 the vehicle again) */
        uint32_t mx=asr(CL(0x74)+CL(0x7c),1),my=asr(CL(0x78)+CL(0x80),1);
        SB(0x15,B(0x15)&~0xc0u);SB(0x5f,B(0x5f)&~7u);
        modes_t cm=m;cm.b4009=0;cm.b400b=0;
        if ((CB(0x92)|CB(0x93))&0x20u) cm.b4009=g42aa;
        uint32_t d0=L(0x44),d1=L(0x48);
        ga_self(a,e);
        neo_kreq_t q={S32(d0),S32(d1),S32(mx-d0),S32(my-d1),NEO_KOBJ_NONE,cm.m4004,cm.b4008,cm.b4009,cm.b400a,cm.b400b,cm.b400d};
        neo_kres_t r;
        if (!neo_kernel_sweep(e->kernel,&q,&r)) {ga_fail(e,0x13968);return -1;}
        uint8_t d6=(uint8_t)r.flags;
        if (d6&0x80u) {SB(0x5f,B(0x5f)|0x80u);SB(0x5a,0x0f);}
        SL(0x28,(uint32_t)r.x);SL(0x2c,(uint32_t)r.y);
        if (g42aa) {SL(0x30,(uint32_t)r.vx);SL(0x34,(uint32_t)r.vy);}
        SL(0x38,ga_ktag(r.parent));
        ga_sb(c,0x94,d6);
        uint32_t d7=(r.kind&0xffff0000u)|(uint16_t)(r.kind-0x4000u);
        if (L(0x62)) {
            if (!(L(0x62)&0x80000000u)) {
                if (d6&0x20u) ga_sb(c,0x92,CB(0x92)&~1u);
                if (d6&4u) {d7=(d7&0xffff0000u)|0x2000u;d6|=1u;}
            } else {
                if (d6&0x20u) ga_sb(c,0x93,CB(0x93)&~1u);
                if (d6&4u) {d7=(d7&0xffff0000u)|0xe000u;d6|=1u;}
            }
        }
        ga_sw(c,0x88,(uint16_t)d7);
        SW(0x6c,(uint16_t)(d7>>8));
        uint8_t f=(uint8_t)(CB(0x93)|CB(0x92));
        if ((d6|f)&1u) {SB(0x15,B(0x15)|0x40u);ga_sl(c,0x8c,0);ga_sw(c,0x8a,0);}
        else if (!g42aa) ga_sw(c,0x8a,0);
        else if (!CW(0x8a)) {ga_sw(c,0x8a,5);SB(0x15,B(0x15)|0x40u);}
        else {uint16_t n=(uint16_t)(CW(0x8a)-1u);ga_sw(c,0x8a,n);if (n) SB(0x15,B(0x15)|0x40u);}
        if (f&2u) SB(0x15,B(0x15)|0x80u);
        if (L(0x62)) {
            if (!(L(0x62)&0x80000000u)) {if (CB(0x92)&4u) SB(0x5f,B(0x5f)|1u);}
            else if (CB(0x93)&4u) SB(0x5f,B(0x5f)|1u);
        }
        uint16_t angle=CW(0x6e);
        ga_project(a,e);
        if ((W(0x66)&0xf000u)==0x8000u) SW(0x1e,W(0x66)-W(0x40));     /* 01AD8C */
        SL(0x62,saved62);
        return angle;
    }
}
/* 014A4E: the tread node lives while its vehicle does (00409A). */
ga_result_t neo_tread_tick(neo_gactor_t *a,neo_genv_t *e) {
    return ga_creator_alive(a,e)?GA_MORE:GA_DELETE;
}
