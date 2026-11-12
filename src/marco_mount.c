#include "marco_mount.h"
/* Manual translation of ROM 0C3078..0C3260 and 0C3348..0C3376 (see
 * marco_mount.h); ROM labels are provenance only. */
static uint16_t rw(const uint8_t *m,unsigned o) {return (uint16_t)(m[o]<<8|m[o+1]);}
static uint32_t rl(const uint8_t *m,unsigned o) {return (uint32_t)rw(m,o)<<16|rw(m,o+2);}
static void ww(uint8_t *m,unsigned o,uint16_t v) {m[o]=(uint8_t)(v>>8);m[o+1]=(uint8_t)v;}
static void wl(uint8_t *m,unsigned o,uint32_t v) {ww(m,o,(uint16_t)(v>>16));ww(m,o+2,(uint16_t)v);}
static int32_t asr(uint32_t v,unsigned n) {return (int32_t)v>>n;}
/* 0C0F16 */
static void k0f16(uint8_t *m) {m[0x98]=0;m[0x90]=0xff;m[0x91]=0xff;wl(m,0x7c,0xc381cu);}
marco_mount_result_t marco_mount_handoff(neo_vslots_t *v,uint8_t *m,const marco_mount_in_t *in) {
    if (m[0x5f]&0x80u) return MARCO_MOUNT_COMMON;                         /* 0C3348 */
    bool was=(m[0x9a]&0x80u)!=0;
    if (neo_vslot_accept(v,in->p,in->video,in->halted)) {                 /* 0C3352 */
        m[0x9a]|=0x80u;
        if (was) return MARCO_MOUNT_COMMON;
        wl(m,0,MARCO_MOUNT_C3078);return MARCO_MOUNT_ENTER;               /* 0C3366 */
    }
    m[0x9a]&=0x7fu;                                                      /* 0C3370 */
    return was?MARCO_MOUNT_EXIT:MARCO_MOUNT_COMMON;
}
marco_mount_result_t marco_mount_ride(neo_vslots_t *v,uint8_t *m,const marco_mount_in_t *in,bool *followed,uint32_t *stop) {
    if (rl(m,0)==MARCO_MOUNT_C3078) {
        m[0x7b]=0xff;m[0x7a]=0xff;m[0x15]&=(uint8_t)~1u;wl(m,0x50,0);k0f16(m);
        wl(m,0,MARCO_MOUNT_C309C);                                        /* 0C3096 */
    }
    bool f=neo_vslot_follow(v,in->p,m);                                  /* 0C309C */
    if (followed) *followed=f;
    if (in->system || in->halted) {                                      /* 0C30A2/0C30AC */
        m[0x9a]&=0x7fu;                                                  /* 0C30C0 */
        uint8_t kind,shells,type;uint16_t health,d4;
        if (!neo_vslot_ridden(v,in->p,true,&kind,&shells,&type,&health,&d4)) {*stop=0xaaacc;return MARCO_MOUNT_STOP;}  /* TRAP #F */
        if (kind==4) {*stop=0xc315a;return MARCO_MOUNT_STOP;}
        if (!(m[0x15]&0x40u)) {marco_mount_eject_start(m);return MARCO_MOUNT_EJECT;}   /* 0C30EC */
    }
    m[0x9a]|=0x80u;                                                      /* 0C30B6/0C30E2 */
    return MARCO_MOUNT_COMMON;
}
marco_mount_result_t marco_mount_exit(neo_vslots_t *v,uint8_t *m,const marco_mount_in_t *in,uint8_t *cause,uint32_t *stop) {
    m[0x9a]&=0x7fu;                                                      /* 0C30F0 */
    uint8_t kind;neo_vslot_exit_place(v,in->p,m,&kind,cause);            /* 0AA5AC */
    switch (kind) {
    case 0:
        if (in->system) {marco_mount_eject_start(m);return MARCO_MOUNT_EJECT;}
        m[0x5a]=*cause;m[0x5f]|=0x80u;return MARCO_MOUNT_DIE;            /* 0C310E -> 0C2956 */
    case 2: *stop=0xc32d0;return MARCO_MOUNT_STOP;
    case 4: *stop=0xc315a;return MARCO_MOUNT_STOP;
    case 5: m[0x15]|=1u;                                                 /* 0C313C */
        /* fall through */
    case 3:                                                              /* 0C1F4A */
        if (m[0x93]==2u && m[0x86]>=0x20u) {*stop=0xc1e94;return MARCO_MOUNT_STOP;}
        m[0x9a]|=0x20u;m[0x7b]=0xff;m[0x7a]=0xff;ww(m,0x66,0x8834);ww(m,0x1e,0x8834);
        wl(m,0x50,0x2d0aa8u);m[0x90]=0;wl(m,0x62,0);m[0x96]=0;
        wl(m,0,0xc1fa6u);return MARCO_MOUNT_STAND;
    default: marco_mount_eject_start(m);return MARCO_MOUNT_EJECT;        /* 1 and the others: 0C3232 */
    }
}
void marco_mount_eject_start(uint8_t *m) {
    k0f16(m);wl(m,0x30,0);wl(m,0x34,0xb7000u);wl(m,0,MARCO_MOUNT_C3250);
}
uint16_t marco_mount_input(uint8_t *m,const marco_mount_in_t *in) {
    uint16_t mask=0xffffu;
    if (in->shots>=2u) mask&=0xffbfu;                                    /* 0C0FFE */
    if (!(m[0x9a]&1u) && in->shot_count<=in->shot_limit) mask&=0xffefu;  /* 0C100C */
    uint16_t d0=(uint16_t)(in->fresh&mask);
    if (d0&1u) m[0x5f]|=0x20u; else m[0x5f]&=(uint8_t)~0x20u;
    if (d0&2u) m[0x5f]|=0x10u; else m[0x5f]&=(uint8_t)~0x10u;
    m[0x9a]&=(uint8_t)~8u;
    if (!(m[0x15]&1u)) {if (d0&4u) m[0x9a]|=8u;}
    else if (d0&8u) m[0x9a]|=8u;
    m[0x9b]|=(uint8_t)(d0&m[0x9c]);
    return d0;
}
uint32_t marco_mount_eject(neo_vslots_t *v,uint8_t *m,const marco_mount_in_t *in) {
    neo_vslot_release(v,in->p);                                          /* 0AA4C8 */
    ww(m,0x84,rw(m,0x84)|0x1eu);
    uint16_t d0=marco_mount_input(m,in);
    wl(m,0x34,rl(m,0x34)+(uint32_t)asr(0xffea0000u-rl(m,0x34),5));
    if (!in->system) {
        d0=marco_mount_input(m,in);
        uint32_t d7=0;unsigned d6=5;
        if (d0&4u) {d7=0xfffc0000u;d6=2;}
        if (d0&8u) {d7=0x40000u;d6=2;}
        wl(m,0x30,rl(m,0x30)+(uint32_t)asr(d7-rl(m,0x30),d6));
    }
    if ((d0&0x50u) && (m[0x9a]&8u)) m[0x15]^=1u;                         /* 0C109C */
    return rl(m,0x34);
}
bool marco_mount_eject_end(uint8_t *m,uint32_t saved34) {
    if (!(saved34&0x80000000u)) wl(m,0x34,(m[0x5f]&2u)?0:saved34);      /* 0C10C0 */
    if (m[0x15]&0x40u) {wl(m,0,MARCO_MOUNT_C1EBE);return true;}         /* 0C31FA */
    return false;
}
