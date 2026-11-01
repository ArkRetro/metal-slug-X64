/* Capa del tren de la Misión 3 (neo_train.h). */
#include "neo_train.h"

/* 0B8E36: pares (columna, cuenta) que 0B8E5A copia a 10C6B6. */
static const uint16_t loop_table[]={0x50,0xffff,0x78,0xffff,0xb0,0xffff,0xe0,0xffff,0x138,0xffff,0x188,0xffff,0xffff};
static int16_t s12(int v) {v&=0xfff;return (int16_t)((v&0x800)?v-4096:v);}

void neo_train_reset(neo_train_t *t,int32_t x) {
    *t=(neo_train_t){0};t->x=x;t->index=-4;
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
void neo_train_update(neo_train_t *t,int32_t root_dx,bool own) {
    int32_t dx=root_dx;
    if (own && t->on) {
        /* 0B8D64 (10E94E) runs after the layers: the layer adds the root
         * step of the previous update with the mode in force now. 10E9D0 =
         * 0 outside a stage start, so 10C6B8 snaps to the mode's floor. */
        int32_t d0=t->pending,d1=t->mode==1?-0x20000:t->mode==2?-0xc0000:0;
        if (t->mode==2) {int32_t q=d0>>2;d0=q*3;}
        t->speed=d1;dx=(int32_t)((uint32_t)d0+(uint32_t)d1);
    }
    t->pending=root_dx;
    t->x=(int32_t)((uint32_t)t->x+(uint32_t)dx);
    if (t->on && t->count) {   /* 0B8DD0 with +36 = (-x >> 20) - 4 (0C748E) */
        int o36=s12((int)((uint32_t)-(int64_t)t->x>>20)-4);
        if (s12((int)loop_table[t->index/2]+t->origin-0x20-o36)<0) {
            t->origin=s12(t->origin+0x20);
            if (t->count!=0xffffu) t->count--;
        }
    }
}
int32_t neo_train_scroll(const neo_train_t *t) {
    return (int32_t)(int16_t)(uint16_t)(-(t->x>>16)-16*(int)t->origin);
}
int neo_train_column(const neo_train_t *t,int local_x) {
    int c=(local_x-16*(int)t->origin)>>3;
    c%=784;if (c<0) c+=784;   /* 0B8566: 392 pares de columnas */
    return c;
}
