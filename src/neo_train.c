/* Capa del tren de la Misión 3 (neo_train.h). */
#include "neo_train.h"

/* 0B8E36: pares (columna, cuenta) que 0B8E5A copia a 10C6B6. */
static const uint16_t loop_table[]={0x50,0xffff,0x78,0xffff,0xb0,0xffff,0xe0,0xffff,0x138,0xffff,0x188,0xffff,0xffff};
static int16_t s12(int v) {v&=0xfff;return (int16_t)((v&0x800)?v-4096:v);}

/* 0C748E: +36 = ((FFC0 << 16) - +14) >> 16, asr 4 (columna local de 16 px). */
static int16_t o36_word(int32_t x) {return (int16_t)((int16_t)(uint16_t)((0xffc00000u-(uint32_t)x)>>16)>>4);}
/* 0C6BDE/0C714E: columna local `local` cargada en su ranura con el origen
 * actual (columna de mapa = local - +32, 12 bits). Fuera del mapa (no menor
 * que 0B8566 +8) 0C714E no copia nada y, con el bit 3 de +82 a 0 (op 0x1C,
 * +D = 0), 0C6BDE deja la altura: la ranura sigue mostrando lo que tenía. */
static void ring_load(neo_train_t *t,int local) {
    int m=s12(local-t->origin);
    if ((uint16_t)m<NEO_TRAIN_MAP_COLUMNS) t->ring[local&31]=(int16_t)m;
}
void neo_train_reset(neo_train_t *t,int32_t x) {
    *t=(neo_train_t){0};t->x=x;t->index=-4;t->sky_x=x;t->sky_factor=0x100;
    t->o36=o36_word(x);
    for (int k=0;k<32;k++) t->ring[k]=NEO_TRAIN_EMPTY;   /* 0C702A: the ring starts blank */
    for (int k=0;k<32;k++) ring_load(t,t->o36+k);         /* 0C741A */
}
void neo_train_sky_factor(neo_train_t *t,int16_t factor) {t->sky_factor=factor;}
int32_t neo_layer_step(int32_t step,int16_t factor,uint8_t *frac) {   /* 0C6F9E */
    uint32_t d0=(uint32_t)step*2u;
    int32_t d6=(int32_t)(uint16_t)((uint16_t)d0>>1)*factor;   /* MULS.W: (lo >> 1) x +26 */
    unsigned sum=(unsigned)*frac+((uint32_t)d6&0xffu);*frac=(uint8_t)sum;   /* ADD.B +2A, X = carry */
    d6=(d6>>8)+(int32_t)(sum>>8);
    int32_t hi=(int32_t)(int16_t)(uint16_t)(d0>>16)*factor;   /* SWAP, MULS.W */
    return (int32_t)(((uint32_t)hi<<7)+(uint32_t)d6);
}
static void loop_select(neo_train_t *t,int index) {   /* 0B8E5A */
    if (index<0 || (unsigned)index/2u+1u>=sizeof loop_table/sizeof loop_table[0] || loop_table[index/2]==0xffffu) return; /* trap #F */
    t->index=(int16_t)index;t->count=loop_table[index/2+1];
}
bool neo_train_call(neo_train_t *t,uint32_t routine) {
    switch (routine) {
    case 0x0B8D34: t->on=true;t->mode=0;t->speed=0;return true;   /* 0B8EDC + 0B8CF2 + 0B8D64 */
    case 0x0B8CF2: t->mode=0;return true;
    case 0x0B8CFC: t->mode=1;return true;
    case 0x0B8D06: t->mode=2;return true;
    case 0x0B8EAC: t->index=-4;t->count=0;return true;
    case 0x0B8E50: loop_select(t,t->index+4);return true;
    case 0x0B8E7E: case 0x0B8E84: case 0x0B8E8C: case 0x0B8E94: case 0x0B8E9C: case 0x0B8EA4:
        loop_select(t,(int)((routine-0x0B8E7Eu+2u)/8u)*4);return true;
    }
    return false;
}
/* 0B8EC2: columnas de 0B8EDC (la rutina de cada una copia el cuadro). */
static const uint16_t sky_columns[]={0x50,0xb0,0x110,0x160};
static int o36_of(int32_t x) {return s12((int)((uint32_t)-(int64_t)x>>20)-4);}   /* 0C748E */
void neo_train_update(neo_train_t *t,int32_t root_dx,bool own) {neo_train_update_layers(t,root_dx,own,own);}
void neo_train_update_layers(neo_train_t *t,int32_t root_dx,bool own,bool sky_own) {
    int32_t dx=root_dx,own_dx=0;
    /* 0B8EDC (creado antes que el conductor 0B8D64: corre antes que él y que
     * el paso de la capa): 0C6F6A d4 = columna + +32; a la vista cuando
     * (+36 << 4) + 0x1F0 - (d4 << 4) >= 0 (palabras); con FFFF acaba. */
    if (t->on && t->sky<sizeof sky_columns/sizeof sky_columns[0]) {
        uint16_t d4=(uint16_t)(sky_columns[t->sky]+(uint16_t)t->origin);
        int16_t d3=(int16_t)(uint16_t)(((unsigned)o36_of(t->x)<<4)+0x1f0u-((unsigned)d4<<4));
        if (d3>=0) t->sky++;
    }
    if (t->on) {
        /* 0B8D64 (10E94E) runs after the layers: the layer adds the root
         * step of the previous update with the mode in force now. 10E9D0 =
         * 0 outside a stage start, so 10C6B8 snaps to the mode's floor. */
        int32_t d0=t->pending,d1=t->mode==1?-0x20000:t->mode==2?-0xc0000:0;
        if (t->mode==2) {int32_t q=d0>>2;d0=q*3;}
        own_dx=(int32_t)((uint32_t)d0+(uint32_t)d1);
        if (own) {t->speed=d1;dx=own_dx;}
    }
    /* 0C81C0: the sky 10E83E steps with 10E94E (bit 0 of 10E958) or the
     * root, by its factor +26 (0C6F9E), then 01AC3C adds it to +14. */
    if (t->on) t->sky_x=(int32_t)((uint32_t)t->sky_x+(uint32_t)neo_layer_step(sky_own?own_dx:root_dx,t->sky_factor,&t->sky_frac));
    t->pending=root_dx;
    t->x=(int32_t)((uint32_t)t->x+(uint32_t)dx);
    if (t->on) {
        /* 0C83BC/0C840C (the camera commands, before the driver's 0B8DD0):
         * +3A = +36, then 0C74C4 loads the columns that came in, at the
         * left (+36 went down) or at the right (+36 + 0x20 on), with the
         * origin in force; the others keep the origin they were loaded with. */
        int16_t n=o36_word(t->x);int d7=s12(t->o36-n);t->o36=n;
        if (d7>0) {for (int k=0;k<d7;k++) ring_load(t,n+k);}
        else if (d7<0) {int c=-d7,first=(int16_t)(n-c+0x20);for (int k=0;k<c;k++) ring_load(t,first+k);}
    }
    if (t->on && t->count) {   /* 0B8DD0 with +36 = (-x >> 20) - 4 (0C748E) */
        int o36=s12((int)((uint32_t)-(int64_t)t->x>>20)-4);
        if (s12((int)loop_table[t->index/2]+t->origin-0x20-o36)<0) {
            t->origin=s12(t->origin+0x20);
            if (t->count!=0xffffu) t->count--;
        }
    }
}
unsigned neo_train_sky(const neo_train_t *t) {return t->sky;}
int neo_train_column(const neo_train_t *t,int local_x) {
    int c=(local_x-16*(int)t->origin)>>3;
    c%=784;if (c<0) c+=784;   /* 0B8566: 392 pares de columnas */
    return c;
}
