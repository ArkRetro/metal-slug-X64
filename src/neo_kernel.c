#include "neo_kernel.h"
#include "neo_collision.h"
#include "neo_m6bridge.h"
#include <stddef.h>
/* Manual C translation of the original collision kernel. Register names are
 * kept where the original routines communicate through registers; 68000
 * opcodes are never decoded or interpreted. Unsigned 32-bit arithmetic gives
 * the original wrap-around; signed comparisons are made explicit. */
typedef struct {
    neo_kscene_t *s;
    uint32_t d0,d1,d2,d3,d4,d5,d6,d7;
    uint8_t a0,a1;
    uint32_t m4004; uint8_t b4008,b4009,b400a,b400b,b400d;
    uint16_t w4012; uint8_t b4014; uint32_t l4016; uint16_t w401a;
    uint8_t b4024[4]; uint32_t l4028; uint32_t l402c,l4030; uint32_t l4034,l4038,l403c,l4040;
    bool v4044; uint8_t o4044; uint32_t l4048,l404c;
    bool v4050; uint8_t o4050; uint32_t l4054,l4058,l405c,l4064; uint8_t o4060;
    uint16_t w4068; unsigned iterator;
    uint8_t mask42a2; uint16_t idx42a4; int32_t qbox[4]; /* RAM 10C2A2/10C2A4/10C292 */
    int16_t qy0,qy1;  /* 01382A: D4/D5 words (query rows with A0's +20/+18) */
    bool unsupported;
} kr_t;

/* RAM 10C016 is a long whose low word is 10C018: 013776 and the crossings
 * write the long, 013C1A/013DB2 the word; later sweeps read either. */
static void set4016(kr_t *k,uint32_t v) {k->l4016=v;k->s->w4018=(uint16_t)v;}
static void set4018(kr_t *k,uint16_t v) {k->l4016=(k->l4016&0xffff0000u)|v;k->s->w4018=v;}
static int32_t S(uint32_t v) {return v<=0x7fffffffu?(int32_t)v:(int32_t)((int64_t)v-0x100000000LL);}
static int16_t SW(uint32_t v) {v&=0xffffu;return (int16_t)(v<=0x7fffu?(int)v:(int)v-65536);}
static uint32_t SWAP(uint32_t v) {return (v<<16)|(v>>16);}
static uint32_t SETW(uint32_t reg,uint32_t word) {return (reg&0xffff0000u)|(word&0xffffu);}
static uint32_t ASR(uint32_t v,unsigned n) {return (uint32_t)(S(v)>=0?(int32_t)(v>>n):~(int32_t)(~v>>n));}
static const neo_kobj_t *obj(const kr_t *k,uint8_t id) {
    if (id>=NEO_KOBJ_DYN0 && id<NEO_KOBJ_DYN0+NEO_KDYN_COUNT) return &k->s->dyn[id-NEO_KOBJ_DYN0].pos;
    if (id==NEO_KOBJ_TRAIN) return k->s->train_on?&k->s->train:NULL;
    if (id==NEO_KOBJ_NODE) return &k->s->node;
    return id==NEO_KOBJ_TERRAIN?&k->s->terrain:id==NEO_KOBJ_ROOT?&k->s->root:NULL;
}
static bool known(uint8_t id) {return id<NEO_KOBJ_DYN0+NEO_KDYN_COUNT || id==NEO_KOBJ_TRAIN || id==NEO_KOBJ_NODE;}
/* ROM 0B8566 collision (+4 = 4340BA): a one-cell platform 0x2B on row 36
 * of every column but 384..447, which hold 0xA0 on rows 46 and 47
 * (tools/test_train_grid.py checks this against the ROM). */
uint8_t neo_kernel_train_cell(int column,int row) {
    if (column<0 || column>=784 || row<0 || row>=48) return 0;
    if (column>=384 && column<448) return row>=46?0xa0:0;
    return row==36?0x2b:0;
}
/* 0C79FE on the train's ring: slot row q = ((~y)&0x1F8)>>3 holds map row
 * (q - 24) & 63 (layer +34 = 19 all mission: the platform row 36 is in
 * slot 60 and the 0xA0 rows 46/47 in slots 6/7; MAME, every read of the
 * mission and run4 RAM 10D83E, docs/MISION3.md "Reconocimiento del mapa");
 * the column is the map column under the local x (+32 origin, neo_train.c). */
uint8_t neo_kernel_train_ring(uint16_t x,uint16_t y,int16_t origin) {
    int q=(int)(((uint16_t)~y&0x1f8u)>>3);
    int c=((int)(int16_t)x-16*(int)origin)>>3;
    c%=784;if (c<0) c+=784;
    return neo_kernel_train_cell(c,(q-24)&63);
}
static uint8_t train_cell(const neo_kscene_t *s,uint16_t x,uint16_t y) {
    if (s->bridge_ring) return neo_m6ring_cell(s->bridge_ring,s->bridge_map,x,y);   /* Mission 6: the bridge 10D7B6 */
    if (s->train_ring) return s->train_ring[((x&0x1f8u)<<3)|(((uint16_t)~y&0x1f8u)>>3)];   /* 0C79FE */
    return neo_kernel_train_ring(x,y,s->train_origin);
}
static uint16_t w4024(const kr_t *k) {return (uint16_t)((k->b4024[0]<<8)|k->b4024[1]);}
static uint16_t w4026(const kr_t *k) {return (uint16_t)((k->b4024[2]<<8)|k->b4024[3]);}
static void or4024(kr_t *k,uint16_t v) {k->b4024[0]|=(uint8_t)(v>>8);k->b4024[1]|=(uint8_t)v;}
static void or4026(kr_t *k,uint16_t v) {k->b4024[2]|=(uint8_t)(v>>8);k->b4024[3]|=(uint8_t)v;}

/* 0C79FE: terrain-object ring lookup (rows above the 256-pixel map are empty
 * in the ring). The ring holds 64 columns: the loaded window starts one
 * column before (-64-layer.x)>>3 (column loads observed in the m2x capture:
 * the slot of column c is reloaded when that start passes c+1); a column
 * outside it reads the loaded column sharing its ring slot, as the original
 * does for actors far behind or ahead of the camera. */
static uint8_t cell(const neo_kscene_t *s,uint16_t x,uint16_t y) {
    if (s->ring) return s->ring[((x&0x1f8u)<<3)|(((uint16_t)~y&0x1f8u)>>3)];   /* 0C79FE */
    /* Vertical stages of the mission packs (rows > 32): the ring's window is
     * not modelled; the cell is read at the terrain-local pixel itself
     * (docs/MISION2.md "Fase 2"). */
    if (neo_collision_rows()>32u) return neo_collision_cell((int16_t)x,(int16_t)y);
    unsigned row=(unsigned)((~y)&0x1f8u)>>3; /* (~y)/8 modulo 64 */
    unsigned world_row=63u-row; if (world_row>=32u) return 0;
    int first=((int16_t)(uint16_t)(-64-(int16_t)(uint16_t)((uint32_t)s->terrain.x>>16))>>3)-1;
    int col=(int16_t)x>>3;
    int loaded=first+(int)((unsigned)(col-first)&63u);
    /* The ring is filled ahead of the scroll, but the map ends at its last
     * column: at the end of the mission (boss arena, camera 3520) the slots
     * past it still hold the columns loaded 64 earlier (MAME: the boss's
     * prisoners run on the arena floor at x 3447, column 430). */
    if (loaded>=(int)neo_collision_columns()) loaded-=64;
    return neo_collision_cell(loaded*8+((int)x&7),(int)(world_row*8u));
}
/* 012938. d0.w/d1.w are local pixels (caller swapped the Q16 words). */
static void k12938(kr_t *k) {
    if (k->a0==NEO_KOBJ_NONE) {k->d7=0;return;}
    uint32_t hi,lo;
    if (k->a0>=NEO_KOBJ_DYN0 && k->a0<NEO_KOBJ_DYN0+NEO_KDYN_COUNT) { /* 01431A */
        const uint8_t *m=k->s->dyn[k->a0-NEO_KOBJ_DYN0].map;
        if (!m) {k->unsupported=true;k->d7=0;return;}
        uint16_t d4=(uint16_t)(SW(k->d0)>>3),d5=(uint16_t)(SW(k->d1)>>3);
        d4=(uint16_t)(d4-(uint16_t)(int16_t)(int8_t)m[0]);d5=(uint16_t)(d5-(uint16_t)(int16_t)(int8_t)m[1]);
        uint16_t w=m[2],h=m[3];
        /* Outside the map 014360 returns D7=0 with D6 still holding the
         * width byte, so the common tail below raises flag 8. */
        k->d6=w;hi=0;lo=0;
        if (d4<w) {
            d5=(uint16_t)~(uint16_t)(d5-h);
            if (d5<h) {k->d6=m[4];hi=m[5];lo=m[10+(unsigned)d5*w+d4];}
        }
    } else {
    uint32_t c=k->a0==NEO_KOBJ_TRAIN?train_cell(k->s,(uint16_t)k->d0,(uint16_t)k->d1):cell(k->s,(uint16_t)k->d0,(uint16_t)k->d1);
    k->d6=c;
    uint32_t g=(c&0xe0u)>>5;
    if (k->s->group_map) g=k->s->group_map[g];
    if (g==0x0bu) {hi=0;lo=(c&15u)+16u;} else {hi=g;lo=c&15u;}
    k->d6=c&0x10u;
    }
    /* 0129A2..0129BC: group 1 with b400D clears the cell flag and shape. */
    if (hi==1u && k->b400d) {k->d6=0;lo=0;}
    k->d7=(hi<<16)|lo;
    if (k->d6&0xffu) {
        k->w4012|=8u;
        if (!k->b4008) {k->d7&=0xffff0000u;return;}
    }
    if ((k->d7&0xffffu)>=0x1eu) {k->d7&=0xffff0000u;return;}
    k->d7=SETW(k->d7,(k->d7&0xffffu)*4u);
}
/* 0129EC: discard surfaces that do not oppose the motion (d2,d3). */
static void k129ec(kr_t *k) {
    int32_t d2=S(k->d2),d3=S(k->d3),d4;bool zero=false;
    switch ((k->d7&0xffffu)>>2) {
    case 0:case 14:case 15:break;
    case 1:case 2:case 3:case 4:d4=S(k->d3<<2);zero=d2<d4;break;
    case 5:zero=d2<d3;break;
    case 6:case 7:case 8:case 9:d4=S(0u-(k->d3<<2));zero=d2>d4;break;
    case 10:zero=d3>S(0u-k->d2);break;
    case 11:case 12:case 16:zero=d3>0;break;
    case 13:case 28:zero=d3<0;break;
    case 17:case 18:case 19:case 20:d4=S(k->d3<<2);zero=d2>d4;break;
    case 21:zero=d2>d3;break;
    case 22:case 23:case 24:case 25:d4=S(0u-(k->d3<<2));zero=d2<d4;break;
    case 26:zero=d3<S(0u-k->d2);break;
    case 27:zero=d2>0;break;
    case 29:zero=d2<0;break;
    default:k->unsupported=true;zero=true;break;
    }
    if (zero) k->d7=0;
}
#define NONE 0x80000000u
/* 012AD4: horizontal distance to the shape boundary. */
static void k12ad4(kr_t *k) {
    uint32_t x=k->d0&0x7ffffu,y=k->d1&0x7ffffu,v;
    unsigned shape=(k->d7&0xffffu)>>2;
    switch (shape) {
    case 1:case 17:v=y;goto up;
    case 2:case 18:v=y-0x20000u;goto up;
    case 3:case 19:v=y-0x40000u;goto up;
    case 4:case 20:v=y-0x60000u;goto up;
    up: k->d4=x;if (v>=0x20000u) {k->d6=NONE;return;} k->d6=(v<<2)-x-1u;return;
    case 5:case 21:k->d4=x;k->d6=y-x-1u;return;
    case 6:case 22:v=y-0x60000u;goto down;
    case 7:case 23:v=y-0x40000u;goto down;
    case 8:case 24:v=y-0x20000u;goto down;
    case 9:case 25:v=y;goto down;
    down: {uint32_t t=v-0x20000u;bool borrow=v<0x20000u;
        if (!borrow) {k->d6=NONE;return;}
        k->d4=x;k->d6=~((t<<2)+x);return;}
    case 10:case 26:k->d4=x;k->d6=((~k->d1)&0x7ffffu)-x;return;
    case 15:if (S(k->d2)<0) goto c12ca8; goto c12c96;
    case 29:goto c12c96;
    case 27:goto c12ca8;
    default:k->d6=NONE;return;
    }
c12c96: if (k->b400a) {k->d6=NONE;return;} k->d6=~(k->d0&0x7ffffu);return;
c12ca8: if (k->b400a) {k->d6=NONE;return;} k->d6=(~k->d0)&0x7ffffu;return;
}
/* 012CBA: vertical distance to the shape surface. */
static void k12cba(kr_t *k) {
    uint32_t x=k->d0&0x7ffffu,y=k->d1&0x7ffffu;
    unsigned shape=(k->d7&0xffffu)>>2;
    switch (shape) {
    case 1:case 17:k->d4=y;k->d6=(x>>2)-y;return;
    case 2:case 18:k->d4=y;k->d6=(x>>2)-y+0x20000u;return;
    case 3:case 19:k->d4=y;k->d6=(x>>2)-y+0x40000u;return;
    case 4:case 20:k->d4=y;k->d6=(x>>2)-y+0x60000u;return;
    case 5:case 21:k->d4=y;k->d6=x-y;return;
    case 6:case 22:k->d4=y;k->d6=(0u-(x>>2))+0x7ffffu-y;return;
    case 7:case 23:k->d4=y;k->d6=(0u-(x>>2))+0x7ffffu-y-0x20000u;return;
    case 8:case 24:k->d4=y;k->d6=(0u-(x>>2))+0x7ffffu-y-0x40000u;return;
    case 9:case 25:k->d4=y;k->d6=(0u-(x>>2))+0x7ffffu-y-0x60000u;return;
    case 10:case 26:k->d4=y;k->d6=((~k->d0)&0x7ffffu)-y;return;
    case 11:k->d6=(~k->d1)&0x7ffffu;return;
    case 12:case 13:k->d6=~(k->d1&0x7ffffu);return;
    case 15:
        if (k->b400a) {k->d6=NONE;return;}
        if (S(k->d3)<0) {k->d6=(~k->d1)&0x7ffffu;return;}
        k->d6=~(k->d1&0x7ffffu);return;
    case 16:case 28:k->d6=((~k->d1)&0x7ffffu)-0x40000u;return;
    default:k->d6=NONE;return;
    }
}
/* 012E64: output angle word by shape. */
static const uint16_t angle_words[30]={0x4000,0x4a00,0x4a00,0x4a00,0x4a00,0x6000,0x3600,0x3600,0x3600,0x3600,
    0x2000,0x4000,0x4000,0xc000,0x4000,0x4000,0x4000,0xca00,0xca00,0xca00,0xca00,0xe000,0xb600,0xb600,0xb600,0xb600,
    0xa000,0x0000,0xc000,0x8000};
static void k12e64(kr_t *k) {
    unsigned i=(k->d7&0xffffu)>>2;
    /* With nothing recorded the initial 0x4000 index reads beyond the table:
     * the observed word at ROM 012E6A+0x4000 is 0x0028 (imported datum). */
    if ((k->d7&0xffffu)==0x4000u) {k->d7=SETW(k->d7,0x0028u);return;}
    if (i>=30u || (k->d7&3u)) {k->unsupported=true;return;}
    k->d7=SETW(k->d7,angle_words[i]);
}
/* 013006-style rounding: ASR.L #4 then ADDX of the last bit shifted out. */
static uint32_t asr4_round(uint32_t v) {return ASR(v,4)+((v>>3)&1u);}
/* 012EE2: response of the remaining motion to the touched surface. */
static void k12ee2(kr_t *k) {
    if (!(k->d2|k->d3)) return;
    uint32_t d5=((uint32_t)w4024(k)<<16)|w4026(k);
    if ((d5&0xffffu)==0xffffu) {k->d2=k->d3=0;return;}
    d5=SWAP(d5);
    if ((d5&0xffffu)==0xffffu) {k->d2=k->d3=0;return;}
    if ((k->l4016>>16)==9u) d5=SETW(d5,0xffffu);
    unsigned shape=(k->d7&0xffffu)>>2;
    uint32_t d4;
    switch (shape) {
    case 0:case 14:return;
    case 17:case 18:case 19:case 20:
        k->w4012|=0x12u;k->w4068=0xca00;if (!k->b4024[3]) k->d3-=1u;goto c12fc0;
    case 1:case 2:case 3:case 4:
        k->w4012|=0x42u;k->w4068=0x4a00;
    c12fc0:
        if (d5) {
            d4=(k->d3<<2)-k->d2;if (!d4) return;
            uint16_t m=S(d4)>=0?0xff00u:0x00ffu;
            uint16_t w=(uint16_t)(d5&m);d5=SETW(d5,w);
            if (w) {or4026(k,w);if (w&0xffu) k->d2+=3u;k->d2=ASR(k->d2,2);k->d3=k->d2;k->d2<<=2;}
            d5=SWAP(d5);m=(uint16_t)~m;w=(uint16_t)(d5&m);d5=SETW(d5,w);
            if (w) {or4024(k,w);k->d2=k->d3<<2;}
            if (d5) return;
        }
        d4=(k->d3<<2)-k->d2;if (!d4) return;
        k->d2=(k->d2<<2)+k->d3;k->d2=asr4_round(k->d2);k->d3=k->d2;k->d2<<=2;return;
    case 21:
        k->w4012|=0x12u;k->w4068=0xe000;if (!k->b4024[3]) k->d3-=1u;goto c1304a;
    case 5:
        k->w4012|=0x42u;k->w4068=0x6000;
    c1304a:
        if (d5) {
            if (k->d3==k->d2) return;
            uint16_t m=S(k->d3-k->d2)>=0?0xff00u:0x00ffu;
            uint16_t w=(uint16_t)(d5&m);d5=SETW(d5,w);
            if (w) {or4026(k,w);k->d3=k->d2;}
            d5=SWAP(d5);m=(uint16_t)~m;w=(uint16_t)(d5&m);d5=SETW(d5,w);
            if (w) {or4024(k,w);k->d2=k->d3;}
            if (d5) return;
        }
        if (k->d3==k->d2) return;
        {uint32_t sum=k->d2+k->d3;k->d2=ASR(sum,1);if (sum&1u) k->d2=(k->d2&0xffffff00u)|((k->d2+1u)&0xffu);}
        k->d3=k->d2;return;
    case 22:case 23:case 24:case 25:
        k->w4012|=0x12u;k->w4068=0xb600;if (!k->b4024[3]) k->d3-=1u;goto c130b8;
    case 6:case 7:case 8:case 9:
        k->w4012|=0x42u;k->w4068=0x3600;
    c130b8:
        if (d5) {
            d4=(k->d3<<2)+k->d2;if (!d4) return;
            uint16_t m=S(d4)<0?0xff00u:0x00ffu;
            uint16_t w=(uint16_t)(d5&m);d5=SETW(d5,w);
            if (w) {
                if (w&0xffu) k->d2+=3u;
                d5=SETW(d5,(uint16_t)~w);or4026(k,(uint16_t)~w);
                k->d2=ASR(k->d2,2);k->d3=0u-k->d2;k->d2<<=2;
            }
            d5=SWAP(d5);w=(uint16_t)(d5&m);d5=SETW(d5,w);
            if (w) {d5=SETW(d5,(uint16_t)~w);or4024(k,(uint16_t)~w);k->d2=0u-(k->d3<<2);}
            if (d5) return;
        }
        d4=(k->d3<<2)+k->d2;if (!d4) return;
        k->d3-=k->d2<<2;k->d3=asr4_round(k->d3);k->d2=0u-(k->d3<<2);return;
    case 26:
        k->w4012|=0x12u;k->w4068=0xa000;if (!k->b4024[3]) k->d3-=1u;goto c1314a;
    case 10:
        k->w4012|=0x42u;k->w4068=0x2000;
    c1314a:
        if (d5) {
            d4=k->d2+k->d3;if (!d4) return;
            uint16_t m=S(d4)<0?0xff00u:0x00ffu;
            uint16_t w=(uint16_t)(d5&m);d5=SETW(d5,w);
            if (w) {d5=SETW(d5,(uint16_t)~w);or4026(k,(uint16_t)~w);k->d3=0u-k->d2;}
            d5=SWAP(d5);w=(uint16_t)(d5&m);d5=SETW(d5,w);
            if (w) {d5=SETW(d5,(uint16_t)~w);or4024(k,(uint16_t)~w);k->d2=0u-k->d3;}
            if (d5) return;
        }
        if (!(k->d2+k->d3)) return;
        {uint32_t diff=k->d3-k->d2;k->d3=ASR(diff,1)+(diff&1u);}
        k->d2=0u-k->d3;return;
    case 11:case 12:case 16:
        k->w4012|=0x42u;k->w4068=0x4000;k->b4024[3]=0xff;k->d3=0;return;
    case 13:case 28:
        k->w4012|=0x12u;k->w4068=0xc000;k->b4024[2]=0xff;k->d3=0;return;
    case 15:
        if (S(k->d2)>0) goto c131d6;
        if (S(k->d2)<0) goto c131ec;
        return;
    case 29:
    c131d6: k->w4012|=6u;k->w4068=0x8000;k->b4024[0]=0xff;k->d2=0;return;
    case 27:
    c131ec: k->w4012|=6u;k->w4068=0x0000;k->b4024[1]=0xff;k->d2=0;return;
    default:k->unsupported=true;return;
    }
}
/* 013204 */
static void k13204(kr_t *k) {
    if ((k->d7&0xffffu)!=0x3cu) {k12ee2(k);return;}
    if (S(k->d3)>0) {k->w4012|=0x12u;k->w4068=0xc000;k->b4024[2]=0xff;k->d3=0;}
    else if (S(k->d3)<0) {k->w4012|=0x42u;k->w4068=0x4000;k->b4024[3]=0xff;k->d3=0;}
}
/* Shared cell-crossing tails of 013214 (with optional 10C004 mask). */
static void cross_horizontal(kr_t *k) {
    k->d0=SWAP(k->d0);k->d1=SWAP(k->d1);k12938(k);
    if ((k->d7&0xffffu)==0x38u) {k->b4014=1;k->d7&=0xffff0000u;}
    else if (k->b4014==1) {
        k->b4014=2;
        if (k->m4004) {
            uint32_t w=(k->d7&0xffffu)>>2;
            if (k->m4004&(1u<<(w&31u))) {w=0;k->w4012|=0x20u;}
            k->d7=SETW(k->d7,w<<2);
        }
    }
    set4016(k,k->d7);k->d0=SWAP(k->d0);k->d1=SWAP(k->d1);k->d4=0xffffffffu;
}
static void cross_vertical(kr_t *k) {
    k->d0=SWAP(k->d0);k->d1=SWAP(k->d1);k12938(k);
    if ((k->d7&0xffffu)==0x38u) {k->b4014=1;k->d7&=0xffff0000u;}
    else k->b4014=0;
    set4016(k,k->d7);k->d0=SWAP(k->d0);k->d1=SWAP(k->d1);k->d4=0xffffffffu;
}
/* ANDI/ADD/SWAP/CMPI.W #8: true when the step leaves the current cell. */
static bool step_x(kr_t *k,uint32_t amount) {
    k->d4=SWAP((k->d0&0x7ffffu)+amount);k->d0+=amount;
    return (uint16_t)k->d4>=8u;
}
static bool step_y(kr_t *k,uint32_t amount) {
    k->d4=SWAP((k->d1&0x7ffffu)+amount);k->d1+=amount;
    return (uint16_t)k->d4>=8u;
}
/* 013214: one horizontal step. Returns with d6 = 0 or -1 as the original. */
static void k13214(kr_t *k) {
    if (!k->d2) {k->d6=0;return;}
    if (k->d7&0xffffu) {
        k12ad4(k);
        if (k->d6!=NONE && S(k->d6^k->d2)>=0) {
            if (S(k->d6)<0) k->d6+=1u;
            if (!k->d6) {
                k129ec(k);if (k->d7&0xffffu) k12ee2(k);
                k->d6=0xffffffffu;return;
            }
            uint32_t d5=k->d2;k->d2-=k->d6;d5^=k->d2;
            if (S(d5)>=0) {
                k129ec(k);if (k->d7&0xffffu) k12ee2(k);
                if (step_x(k,k->d6)) cross_horizontal(k);
                k->d6=0xffffffffu;return;
            }
            k->d2+=k->d6;
            if (step_x(k,k->d2)) cross_horizontal(k);
            k->d2=0;k->d6=0;return;
        }
    }
    /* 013348: free movement, clamped to one cell. */
    uint32_t d5=k->d2;
    if (S(d5)>0x80000) d5=0x80000u; else if (S(d5)<-0x80000) d5=(uint32_t)-0x80000;
    k->w401a=(uint16_t)k->d7;
    if (k->s->w4018==0x3cu) {
        if (step_x(k,d5)) cross_horizontal(k);
        if ((k->d7&0xffffu)==0x3cu) {k->d2-=d5;k->d6=0;return;}
    } else if (step_x(k,d5)) cross_horizontal(k);
    /* 01345A */
    if (SW(k->d4)>=0 || !(k->d7&0xffffu)) {k->d2-=d5;k->d6=0;return;}
    k12ad4(k);
    if (k->d6==NONE) {k->d2-=d5;k->d6=0;return;}
    if (S(k->d6)>=0) k->d6+=1u;
    k->d2-=d5;d5^=k->d6;
    if (S(d5)>=0) {k->d6=0;return;}
    k->d2-=k->d6;k129ec(k);
    if (!(k->d7&0xffffu)) {k->d2+=k->d6;k->w4012|=0x100u;k->d6=0;return;}
    k12ee2(k);
    if (step_x(k,k->d6)) cross_horizontal(k);
    if (SW(k->d4)<0) k->d7=SETW(k->d7,k->w401a);
    k->d6=0xffffffffu;
}
/* 01351A: one vertical step. */
static void k1351a(kr_t *k) {
    if (!k->d3) {k->d6=0;return;}
    if (k->d7&0xffffu) {
        k12cba(k);
        if (k->d6!=NONE && S(k->d6^k->d3)>=0) {
            if (S(k->d6)<0) k->d6+=1u;
            if (!k->d6) {
                k129ec(k);if (k->d7&0xffffu) k13204(k);
                k->d6=0xffffffffu;return;
            }
            uint32_t d5=k->d3;k->d3-=k->d6;d5^=k->d3;
            if (S(d5)>=0) {
                k129ec(k);if (k->d7&0xffffu) k13204(k);
                if (step_y(k,k->d6)) cross_vertical(k);
                k->d6=0xffffffffu;return;
            }
            k->d3+=k->d6;
            if (step_y(k,k->d3)) cross_vertical(k);
            k->d3=0;k->d6=0;return;
        }
    }
    uint32_t d5=k->d3;
    if (S(d5)>0x80000) d5=0x80000u; else if (S(d5)<-0x80000) d5=(uint32_t)-0x80000;
    k->w401a=(uint16_t)k->d7;
    if (k->s->w4018==0x3cu) {
        if (step_y(k,d5)) cross_vertical(k);
        if ((k->d7&0xffffu)==0x3cu) {k->d3-=d5;k->d6=0;return;}
    } else if (step_y(k,d5)) cross_vertical(k);
    /* 0136D8 */
    if (SW(k->d4)>=0 || !(k->d7&0xffffu)) {k->d3-=d5;k->d6=0;return;}
    k12cba(k);
    if (k->d6==NONE) {k->d3-=d5;k->d6=0;return;}
    if (S(k->d6)>=0) k->d6+=1u;
    k->d3-=d5;d5^=k->d6;
    if (S(d5)>=0) {k->d6=0;return;}
    k->d3-=k->d6;k129ec(k);
    if (!(k->d7&0xffffu)) {k->d3+=k->d6;k->w4012|=0x100u;k->d6=0;return;}
    k13204(k);
    if (step_y(k,k->d6)) cross_vertical(k);
    if (SW(k->d4)<0) k->d7=SETW(k->d7,k->w401a);
    k->d6=0xffffffffu;
}
/* 013776: sweep the remaining (d2,d3) through cells of object a0. */
static void k13776(kr_t *k) {
    k->d0=SWAP(k->d0);k->d1=SWAP(k->d1);k->b4014=0;k12938(k);set4016(k,k->d7);
    if ((k->d7&0xffffu)==0x38u) k->b4014=1;
    k->d0=SWAP(k->d0);k->d1=SWAP(k->d1);
    k->l4028=0;
    unsigned guard=0;
    enum {V,H,TAIL} next;
    if (k->d2) next=H; else if (k->d3) next=V; else next=TAIL;
    while (next!=TAIL) {
        if (++guard>4096u) {k->unsupported=true;return;}
        if (next==V) {
            k1351a(k);
            if (!k->d3) {next=k->d2?H:TAIL;continue;}
            uint32_t d5=k->d2;
            if (!d5) {next=V;continue;}
            if (S(d5)<0) d5=0u-d5;
            k->l4028+=d5;
            if (S(k->l4028)>=0) {next=H;continue;}
            next=(k->d6&0xffu)?H:V;
        } else {
            k13214(k);
            if (!k->d2) {next=k->d3?V:TAIL;continue;}
            uint32_t d5=k->d3;
            if (!d5) {next=H;continue;}
            if (S(d5)<0) d5=0u-d5;
            k->l4028-=d5;
            if (S(k->l4028)<0) {next=V;continue;}
            next=(k->d6&0xffu)?V:H;
        }
    }
    if (k->s->w4018==0x3cu) k->w4012|=0x200u;
}
/* 01AC4C: convert start (previous frame) / end (current frame) coordinates. */
static void k1ac4c(kr_t *k) {
    if (k->a0==k->a1) return;
    k->d2+=k->d0;k->d3+=k->d1;
    const neo_kobj_t *a=obj(k,k->a0),*b=obj(k,k->a1);
    if (a) {k->d0+=(uint32_t)a->prev_x;k->d1+=(uint32_t)a->prev_y;k->d2+=(uint32_t)a->x;k->d3+=(uint32_t)a->y;}
    if (b) {k->d0-=(uint32_t)b->prev_x;k->d1-=(uint32_t)b->prev_y;k->d2-=(uint32_t)b->x;k->d3-=(uint32_t)b->y;}
    k->d2-=k->d0;k->d3-=k->d1;
}
/* 01381C/01382A/014160/01424C: object list. Region evidence: 10C72E active,
 * 10D7B6 descriptor +4 zero, empty dynamic-platform bitmaps. */
/* 014160: dynamic objects whose bins meet the query's box (absolute
 * coordinates: called with A0 = 0 by 013968/013DB2). */
static void k14160(kr_t *k,uint32_t d0,uint32_t d1,uint32_t d2,uint32_t d3) {
    k->idx42a4=7;k->mask42a2=0;
    {   /* 01382A's D4/D5 (D1, D3 at the list start, relative to A0) */
        const neo_kobj_t *p=obj(k,k->a0);uint32_t y0=d1,y1=d1+d3;
        if (p) {y0+=(uint32_t)p->prev_y;y1+=(uint32_t)p->y;}
        int16_t a=(int16_t)(uint16_t)(y0>>16),b=(int16_t)(uint16_t)(y1>>16);
        if (a>b) {int16_t t=a;a=b;b=t;}
        k->qy0=a;k->qy1=b;
    }
    if (!k->b4008 && !k->s->solid) return;
    d2+=d0;d3+=d1;
    if (S(d0)>S(d2)) {uint32_t t=d0;d0=d2;d2=t;}
    if (S(d1)>S(d3)) {uint32_t t=d1;d1=d3;d3=t;}
    k->qbox[0]=S(d0);k->qbox[1]=S(d1);k->qbox[2]=S(d2);k->qbox[3]=S(d3);
    int x0=(int16_t)(uint16_t)(d0>>16),y0=(int16_t)(uint16_t)(d1>>16),x1=(int16_t)(uint16_t)(d2>>16),y1=(int16_t)(uint16_t)(d3>>16);
    uint8_t m=0;
    x1=(int16_t)(uint16_t)(x1+0x80);
    if (x1>=0) {
        x0=(int16_t)(uint16_t)(x0+0x80);if (x0<0) x0=0;
        y1=(int16_t)(uint16_t)(y1-0x80);
        if (y1>=0) {
            y0=(int16_t)(uint16_t)(y0-0x80);if (y0<0) y0=0;
            if (x0<=0x23f && y0<=0x1ff) {
                if (x1>0x23f) x1=0x23f;
                if (y1>0x1ff) y1=0x1ff;
                uint8_t bx=0,by=0;
                for (int i=x0>>5;i<=x1>>5;i++) bx|=k->s->bins_x[i];
                for (int i=y0>>5;i<=y1>>5;i++) by|=k->s->bins_y[i];
                m=(uint8_t)(bx&by);
            }
        }
    }
    k->mask42a2=m;
    if (!k->b4008) k->mask42a2&=k->s->solid;
}
/* 01382A static list (013810: 10C72E, then the inactive 10D7B6), then the
 * dynamic objects 01424C, highest index first. */
static void k1382a(kr_t *k) {
    if (k->iterator==0) {k->iterator=1;k->a1=NEO_KOBJ_TERRAIN;return;}
    if (k->iterator==1 && k->s->train_on) {
        k->iterator=2;
        if (k->s->bridge_ring) {k->a1=NEO_KOBJ_TRAIN;return;}   /* 0BC6E6 +C = 0: 01384E returns it */
        /* 01384E..013896: (+34 - 0x11 + 1) * 16 + y; the rows 7*16 above */
        int d6=(int16_t)(uint16_t)((19-0x11+1)*16+(int16_t)(uint16_t)((uint32_t)k->s->train.y>>16));
        if (!(d6<k->qy0) && !((int16_t)(d6-7*16)>k->qy1)) {k->a1=NEO_KOBJ_TRAIN;return;}
    }
    k->iterator=2;
    uint8_t d6=k->mask42a2;int d7=k->idx42a4;
    while (d6) {
        if (d6&0x80u) {
            const neo_kdyn_t *o=&k->s->dyn[d7&7];
            bool skip=k->s->self_dyn==(uint8_t)(d7+1) || !o->map;
            for (unsigned c=0;!skip && c<3 && o->chain[c];c++) if (o->chain[c]==k->s->self_block) skip=true;
            if (!skip && !(k->qbox[2]<o->bbox[0] || k->qbox[0]>o->bbox[2] || k->qbox[3]<o->bbox[1] || k->qbox[1]>o->bbox[3])) {
                k->idx42a4=(uint16_t)(d7-1);k->mask42a2=(uint8_t)(d6<<1);
                k->a1=(uint8_t)(NEO_KOBJ_DYN0+(d7&7));return;
            }
        }
        d7--;d6=(uint8_t)(d6<<1);
    }
    k->idx42a4=(uint16_t)d7;k->mask42a2=d6;k->a1=NEO_KOBJ_NONE;
}
void neo_kernel_dyn_begin(neo_kscene_t *s) {
    for (unsigned i=0;i<18;i++) s->bins_x[i]=0;
    for (unsigned i=0;i<16;i++) s->bins_y[i]=0;
    s->solid=0;
}
/* 013F82 */
void neo_kernel_dyn_update(neo_kscene_t *s,unsigned i) {
    neo_kdyn_t *o=&s->dyn[i];const uint8_t *m=o->map;
    if (!m) return;
    uint32_t d0=(uint32_t)o->pos.prev_x,d1=(uint32_t)o->pos.prev_y,d2=(uint32_t)o->pos.x,d3=(uint32_t)o->pos.y;
    if (S(d0)>S(d2)) {uint32_t t=d0;d0=d2;d2=t;}
    if (S(d1)>S(d3)) {uint32_t t=d1;d1=d3;d3=t;}
    int16_t dx=(int16_t)((int8_t)m[0]*8),dy=(int16_t)((int8_t)m[1]*8),w=(int16_t)((int8_t)m[2]*8),h=(int16_t)(((int8_t)m[3]+1)*8);
    w=(int16_t)(w+dx);h=(int16_t)(h+dy);
    d0+=(uint32_t)(int32_t)dx<<16;d1+=(uint32_t)(int32_t)dy<<16;d2+=(uint32_t)(int32_t)w<<16;d3+=(uint32_t)(int32_t)h<<16;
    o->bbox[0]=S(d0);o->bbox[1]=S(d1);o->bbox[2]=S(d2);o->bbox[3]=S(d3);
    int x0=(int16_t)(uint16_t)(d0>>16),y0=(int16_t)(uint16_t)(d1>>16),x1=(int16_t)(uint16_t)(d2>>16),y1=(int16_t)(uint16_t)(d3>>16);
    x1=(int16_t)(uint16_t)(x1+0x80);if (x1<0) return;
    x0=(int16_t)(uint16_t)(x0+0x80);if (x0<0) x0=0;
    y1=(int16_t)(uint16_t)(y1-0x80);if (y1<0) return;
    y0=(int16_t)(uint16_t)(y0-0x80);if (y0<0) y0=0;
    if (x0>0x23f) return;
    if (x1>0x23f) x1=0x23f;
    if (y0>0x1ff) return;
    if (y1>0x1ff) y1=0x1ff;
    uint8_t bit=(uint8_t)(1u<<i);
    if (!m[4]) s->solid|=bit;
    for (int b=x0>>5;b<=x1>>5;b++) s->bins_x[b]|=bit;
    for (int b=y0>>5;b<=y1>>5;b++) s->bins_y[b]|=bit;
}
/* 0138A0: with 10C00D set and 10C00B clear, the horizontal motion is cut at
 * the screen box of the configuration (x from 10C01C, width 10C020; positions
 * relative to the sweep's object): D6 = FF and 10C024/10C025 mark the side;
 * then the vertical one (10C026/10C027) unless 10C00C is set. */
static bool k138a0(kr_t *k) {
    if (!k->b400d || k->b400b) return false;
    bool cut=false;
    if (k->d2) {
        if (!(k->d2&0x80000000u)) {
            uint32_t d7=(((uint32_t)(uint16_t)(k->s->config[0]+k->s->config[2]))<<16)-1u-k->d0;
            if (d7&0x80000000u) d7=0;
            if (S(k->d2)>S(d7)) {k->d2=d7;cut=true;k->b4024[0]=0xff;}
        } else {
            uint32_t d7=((uint32_t)k->s->config[0]<<16)-k->d0;
            if (!(d7&0x80000000u)) d7=0;
            if (S(k->d2)<S(d7)) {k->d2=d7;cut=true;k->b4024[1]=0xff;}
        }
    }
    /* 013906: the vertical bound unless 10C00C (y from 10C01E, height
     * 10C022, less the shake RAM 10E9CC); downwards the original compares
     * before clamping its limit (013952..01395A: only N and V decide). */
    if (!k->s->b400c && k->d3) {
        if (!(k->d3&0x80000000u)) {
            uint32_t d7=(((uint32_t)(uint16_t)(k->s->config[1]+k->s->config[3]))<<16)-k->s->shake_y-1u-k->d1;
            if (d7&0x80000000u) d7=0;
            if (S(k->d3)>S(d7)) {k->d3=d7;cut=true;k->b4024[2]=0xff;}
        } else {
            uint32_t d7=((uint32_t)k->s->config[1]<<16)-k->s->shake_y-k->d1,r=k->d3-d7;
            bool n=(r>>31)!=0,v=(((k->d3^d7)&(k->d3^r))>>31)!=0;
            if (n && !v) {k->d3=d7;cut=true;k->b4024[3]=0xff;}
        }
    }
    return cut;
}
uint32_t neo_kernel_1495c_d1(const neo_kscene_t *scene,const neo_kreq_t *q,const neo_kres_t *r) {
    if (!(r->flags&1u)) return (uint32_t)r->y;                          /* 0149A8 -> 0149C4 */
    kr_t k={0};k.s=(neo_kscene_t *)scene;
    const neo_kobj_t *a=obj(&k,q->parent),*b=obj(&k,r->parent);
    uint32_t d7=(uint32_t)q->y+(a?(uint32_t)a->y:0u);                   /* 01498C..014996 */
    uint32_t d1=(uint32_t)r->y+(b?(uint32_t)b->y:0u);                   /* 0149AC..0149B4 */
    return SWAP(d1-d7);                                                  /* 0149B8, 0149BA */
}
bool neo_kernel_sweep(neo_kscene_t *scene,const neo_kreq_t *q,neo_kres_t *out) {
    kr_t k={0};k.s=scene;
    if (!known(q->parent)) return false;
    k.d0=(uint32_t)q->x;k.d1=(uint32_t)q->y;k.d2=(uint32_t)q->dx;k.d3=(uint32_t)q->dy;k.a0=q->parent;
    k.w4068=scene->w4068;
    k.m4004=q->mode4004;k.b4008=q->b4008;k.b4009=q->b4009;k.b400a=q->b400a;k.b400b=q->b400b;k.b400d=q->b400d;
    /* 013968 */
    if (S(k.d2)>0xffffff) k.d2=0xffffffu;
    if (S(k.d3)>0xffffff) k.d3=0xffffffu;
    if (S(k.d2)<S(0xff000000u)) k.d2=0xff000000u;
    if (S(k.d3)<S(0xff000000u)) k.d3=0xff000000u;
    k.v4044=false;k.v4050=false;k.o4060=NEO_KOBJ_NONE;k.l405c=0x80000000u;k.l4064=0x4000;
    unsigned restarts=0;
restart:
    if (++restarts>64u) return false;
    k.a1=NEO_KOBJ_NONE;k1ac4c(&k);k.a0=k.a1;
    k.l402c=k.d0;k.l4030=k.d1;
    if (k138a0(&k)) {                                               /* 0139D2..013A32 */
        if (w4024(&k)==0xffffu || w4026(&k)==0xffffu) {k.w4012|=0x80u;goto finish;}
        if (k.v4044 && k.a0==k.o4044) {
            if (k.d2==k.l4048 && k.d3==k.l404c) {k.w4012|=0x80u;goto finish;}
            k.l4048=k.d2;k.l404c=k.d3;
        } else if (!k.v4044) {k.v4044=true;k.o4044=k.a0;k.l4048=k.d2;k.l404c=k.d3;}
        k.v4050=false;
    }
    k.iterator=0;k14160(&k,k.d0,k.d1,k.d2,k.d3);k1382a(&k);
    while (k.a1!=NEO_KOBJ_NONE) {
        k1ac4c(&k);k.a0=k.a1;
        if (k.v4050 && k.a0==k.o4050) {k1382a(&k);continue;}
        k.l4034=k.d0;k.l4038=k.d1;k.l403c=k.d2;k.l4040=k.d3;
        k13776(&k);
        if (k.unsupported) return false;
        bool skip_probe=false;
        if (k.b4009) {k.d2=0;k.d3=0xffffffffu;if (k.w4012&0x20u) skip_probe=true;}
        else {k.d2=k.l403c;k.d3=k.l4040-1u;}
        bool record=false;
        if (!skip_probe) {
            k.d7=k.l4016;
            if (k.d7&0xffffu) {
                k129ec(&k);k12cba(&k);
                if (k.d6>0x80000000u) record=true;
            }
        }
        if (!record) {
            k.d0=SWAP(k.d0);k.d1=SWAP(k.d1);k.d1=SETW(k.d1,(k.d1-8u)&0xffffu);k12938(&k);
            k.d1=SETW(k.d1,(k.d1+8u)&0xffffu);k.d1=SWAP(k.d1);k.d0=SWAP(k.d0);
            if ((k.d7&0xffffu) && !(scene->w4018==0x3cu && (k.d7&0xffffu)==0x3cu)) {
                k129ec(&k);k12cba(&k);
                if (k.d6!=NONE) {k.d6-=0x80000u;record=true;}
            }
        }
        if (k.unsupported) return false;
        if (record && S(k.d6)>=S(k.l405c)) {k.l405c=k.d6;k.l4064=k.d7;k.o4060=k.a0;}
        k.d2=k.d0;k.d3=k.d1;k.d0=k.l4034;k.d1=k.l4038;k.d2-=k.d0;k.d3-=k.d1;
        if (k.d2==k.l403c && k.d3==k.l4040) {k1382a(&k);continue;}
        if (w4024(&k)==0xffffu || w4026(&k)==0xffffu) {k.w4012|=0x80u;goto finish;}
        if (k.v4044 && k.a0==k.o4044) {
            if (k.d2==k.l4048 && k.d3==k.l404c) {k.w4012|=0x80u;goto finish;}
            k.l4048=k.d2;k.l404c=k.d3;
        } else if (!k.v4044) {k.v4044=true;k.o4044=k.a0;k.l4048=k.d2;k.l404c=k.d3;}
        k.v4050=true;k.o4050=k.a0;k.l4054=k.l4064;k.l4058=k.l405c;
        goto restart;
    }
finish:
    k.d0+=k.d2;k.d1+=k.d3;
    k.d6=k.l405c;k.a1=k.o4060;k.d7=k.l4064;k.d5=SETW(k.d5,k.d7);k12e64(&k);
    if (k.unsupported) return false;
    k.d7=SWAP(k.d7);
    bool root=false;
    if (k.d6!=0xffffffffu) {
        if (!k.b4009 || k.d6==0x80000000u) root=true;
        else {k.d6+=1u;k.d1+=k.d6;}
    }
    if (!root) {k.w4012|=1u;if ((k.d7&0xffffu)==9u) root=true;}
    if (root) k.a1=NEO_KOBJ_ROOT;
    k.d7=SWAP(k.d7);
    k.d2=k.d0;k.d3=k.d1;
    const neo_kobj_t *a=obj(&k,k.a0);
    if (a) {k.d2+=(uint32_t)a->x;k.d3+=(uint32_t)a->y;}
    k.d0=k.l402c;k.d1=k.l4030;k.a0=k.a1;a=obj(&k,k.a0);
    if (a) {k.d0-=(uint32_t)a->prev_x;k.d1-=(uint32_t)a->prev_y;k.d2-=(uint32_t)a->x;k.d3-=(uint32_t)a->y;}
    k.d2-=k.d0;k.d3-=k.d1;k.d0+=k.d2;k.d1+=k.d3;
    scene->w4068=k.w4068;
    *out=(neo_kres_t){S(k.d0),S(k.d1),S(k.d2),S(k.d3),k.a0,k.w4012,k.d7,k.w4068};
    return true;
}
bool neo_kernel_point_group(const neo_kscene_t *scene,int32_t x,int32_t y,uint16_t group,bool *unknown) {
    for (unsigned i=0;i<NEO_KDYN_COUNT;i++) if (scene->dyn[i].map && scene->dyn[i].map[5]==group) *unknown=true;
    kr_t k={0};k.s=(neo_kscene_t *)scene;k.a0=NEO_KOBJ_TERRAIN;
    const neo_kobj_t *a=obj(&k,k.a0);
    k.d0=(uint32_t)x;k.d1=(uint32_t)y;
    if (a) {k.d0-=(uint32_t)a->x;k.d1-=(uint32_t)a->y;}
    k.d0=SWAP(k.d0);k.d1=SWAP(k.d1);k12938(&k);
    return (k.d7>>16)==group;
}
uint32_t neo_kernel_cell_class(const neo_kscene_t *scene,uint8_t parent,uint16_t x,uint16_t y,
    uint8_t b4008,uint8_t b400d,uint16_t *flags,uint32_t d7_in) {
    kr_t k={0};k.s=(neo_kscene_t *)scene;k.a0=parent;k.d0=x;k.d1=y;k.b4008=b4008;k.b400d=b400d;k.d7=d7_in;
    k12938(&k);if (flags) *flags=k.w4012;return k.d7;
}
/* 013DB2: search downwards (depth is the negative Q16.16 range in D3). */
bool neo_kernel_floor(neo_kscene_t *scene,int32_t x,int32_t y,uint8_t parent,int32_t depth,
    uint8_t b4008,uint8_t b400a,uint8_t b400d,neo_kfloor_t *out) {
    kr_t k={0};k.s=scene;k.b4008=b4008;k.b400a=b400a;k.b400d=b400d;
    if (!known(parent)) return false;
    k.d0=(uint32_t)x;k.d1=(uint32_t)y;k.a0=parent;
    uint32_t save0=k.d0,save1=k.d1;uint8_t save_a0=k.a0;   /* 4034/4038/403C */
    k.l405c=(uint32_t)depth;k.l4064=0x4000;k.w4012=0;
    const neo_kobj_t *a=obj(&k,k.a0);
    if (a) {k.d0+=(uint32_t)a->x;k.d1+=(uint32_t)a->y;}
    k.a0=NEO_KOBJ_NONE;k.l402c=k.d0;k.l4030=k.d1;k.d2=0;
    k.iterator=0;k14160(&k,k.d0,k.d1,0,(uint32_t)depth);k1382a(&k);
    unsigned guard=0;
    while (k.a1!=NEO_KOBJ_NONE) {
        k.a0=k.a1;a=obj(&k,k.a0);
        k.d0-=(uint32_t)a->x;k.d1-=(uint32_t)a->y;
        set4016(&k,0);uint32_t acc=0;k.d3=0xfff80000u;
        k.d0=SWAP(k.d0);k.d1=SWAP(k.d1);k12938(&k);set4016(&k,k.d7);k.d0=SWAP(k.d0);k.d1=SWAP(k.d1);
        k129ec(&k);k12cba(&k);
        bool hit=k.d6>0x80000000u;
        if (!hit) {k.d1+=k.d3;acc+=k.d3;}
        while (!hit) {
            if (++guard>4096u || k.unsupported) return false;
            /* 013E3A */
            k.d0=SWAP(k.d0);k.d1=SWAP(k.d1);k12938(&k);k.d0=SWAP(k.d0);k.d1=SWAP(k.d1);
            k129ec(&k);
            if (!(scene->w4018==0x3cu && (k.d7&0xffffu)==0x3cu)) {
                set4018(&k,(uint16_t)k.d7);
                if (k.d7&0xffffu) {
                    k12cba(&k);
                    if (k.d6!=NONE) {hit=true;break;}
                }
            }
            /* 013E98 */
            k.d1+=k.d3;acc+=k.d3;
            if (S(acc)<S(k.l405c)) break;
        }
        if (k.unsupported) return false;
        if (hit) { /* 013E72 */
            k.d1+=k.d6;k.d6+=acc;
            if (k.d6>k.l405c) {
                k.l405c=k.d6;k.l4064=k.d7;k.d1+=1u;
                save0=k.d0;save1=k.d1;save_a0=k.a0;k.w4012|=1u;
            }
        }
        k.d0=k.l402c;k.d1=k.l4030;k.a0=NEO_KOBJ_NONE;k.d3=k.l405c;k1382a(&k);
    }
    /* 013EC0 */
    k.d0=save0;k.d1=save1;k.a0=save_a0;k.d7=k.l4064;k.d5=SETW(k.d5,k.d7);k12e64(&k);
    if (k.unsupported) return false;
    k.d7=SWAP(k.d7);
    if ((k.d7&0xffffu)==9u) {
        a=obj(&k,k.a0);if (a) {k.d0+=(uint32_t)a->x;k.d1+=(uint32_t)a->y;}
        k.a0=NEO_KOBJ_ROOT;a=obj(&k,k.a0);k.d0-=(uint32_t)a->x;k.d1-=(uint32_t)a->y;
    }
    k.d7=SWAP(k.d7);
    *out=(neo_kfloor_t){S(k.d0),S(k.d1),k.a0,k.d7,S(k.l405c+1u),(k.w4012&1u)!=0};
    return true;
}
/* 013CF8 (01381C, 01382A; A0 = none): the first collision object whose
 * cell under the absolute point (x,y) has class `cls` (012938, D7 high
 * word). The original keeps B4008 from the previous query: the caller
 * passes it. */
bool neo_kernel_point_class(neo_kscene_t *scene,int32_t x,int32_t y,uint16_t cls,uint8_t b4008,
    int32_t *ox,int32_t *oy,uint8_t *object) {
    return neo_kernel_point_class_in(scene,NEO_KOBJ_NONE,x,y,cls,b4008,ox,oy,object);
}
/* 013CF8 with A0 = `parent` (the point relative to that object). */
bool neo_kernel_point_class_in(neo_kscene_t *scene,uint8_t parent,int32_t x,int32_t y,uint16_t cls,uint8_t b4008,
    int32_t *ox,int32_t *oy,uint8_t *object) {
    kr_t k={0};k.s=scene;k.b4008=b4008;
    k.d0=(uint32_t)x;k.d1=(uint32_t)y;k.a0=parent;
    k.iterator=0;k14160(&k,k.d0,k.d1,0,0);k1382a(&k);
    while (k.a1!=NEO_KOBJ_NONE) {
        const neo_kobj_t *a=obj(&k,k.a0),*b=obj(&k,k.a1);
        if (!b) return false;
        if (a) {k.d0+=(uint32_t)a->x;k.d1+=(uint32_t)a->y;}
        k.d0-=(uint32_t)b->x;k.d1-=(uint32_t)b->y;k.a0=k.a1;
        k.d0=SWAP(k.d0);k.d1=SWAP(k.d1);k12938(&k);k.d1=SWAP(k.d1);k.d0=SWAP(k.d0);
        if (k.unsupported) return false;
        if ((uint16_t)(k.d7>>16)==cls) {if (ox) *ox=S(k.d0);if (oy) *oy=S(k.d1);if (object) *object=k.a0;return true;}
        k1382a(&k);
    }
    return false;
}
bool neo_kernel_point_solid(neo_kscene_t *scene,uint8_t parent,int32_t x,int32_t y,uint8_t b4008) {
    kr_t k={0};k.s=scene;k.b4008=b4008;
    k.d0=(uint32_t)x;k.d1=(uint32_t)y;k.a0=parent;
    k.iterator=0;k14160(&k,k.d0,k.d1,0,0);k1382a(&k);
    while (k.a1!=NEO_KOBJ_NONE) {
        const neo_kobj_t *a=obj(&k,k.a0),*b=obj(&k,k.a1);
        if (!b) return false;
        if (a) {k.d0+=(uint32_t)a->x;k.d1+=(uint32_t)a->y;}
        k.d0-=(uint32_t)b->x;k.d1-=(uint32_t)b->y;k.a0=k.a1;
        k.d0=SWAP(k.d0);k.d1=SWAP(k.d1);k12938(&k);k.d1=SWAP(k.d1);k.d0=SWAP(k.d0);
        if (k.unsupported) return false;
        if (k.d7&0xffffu) return true;
        k1382a(&k);
    }
    return false;
}
/* 013C1A (01381C, 01382A): the surface under the point (x,y) of `parent`
 * among the collision objects: per object the cell at the point (012938)
 * and, without a surface there, the one 8 px above; the highest surface
 * height wins (D6, 10C05C), its D7 word (10C064) is returned through 012E64
 * (D5 = the word before the table, the caller's index). */
bool neo_kernel_probe(neo_kscene_t *scene,int32_t x,int32_t y,uint8_t parent,uint8_t b4008,uint8_t b400a,uint8_t b400d,
    uint16_t *d5,uint32_t *d6,uint32_t *d7,uint8_t *object) {
    kr_t k={0};k.s=scene;k.b4008=b4008;k.b400a=b400a;k.b400d=b400d;
    if (!known(parent)) return false;
    k.d0=(uint32_t)x;k.d1=(uint32_t)y;k.a0=parent;
    k.d7=0;uint8_t o4060=NEO_KOBJ_NONE;k.l405c=0x80000000u;k.l4064=0x4000;
    k.iterator=0;k14160(&k,k.d0,k.d1,0,0xfff80000u);k1382a(&k);
    unsigned guard=0;
    while (k.a1!=NEO_KOBJ_NONE) {
        if (++guard>32u) return false;
        const neo_kobj_t *a=obj(&k,k.a0),*b=obj(&k,k.a1);
        if (a) {k.d0+=(uint32_t)a->x;k.d1+=(uint32_t)a->y;}
        if (b) {k.d0-=(uint32_t)b->x;k.d1-=(uint32_t)b->y;}
        k.a0=k.a1;
        k.d0=SWAP(k.d0);k.d1=SWAP(k.d1);k12938(&k);k.d0=SWAP(k.d0);k.d1=SWAP(k.d1);
        set4018(&k,(uint16_t)k.d7);
        bool record=false;
        if (k.d7&0xffffu) {k129ec(&k);k12cba(&k);if (k.d6>0x80000000u) record=true;}
        if (!record) {
            k.d0=SWAP(k.d0);k.d1=SWAP(k.d1);k.d1=SETW(k.d1,(k.d1-8u)&0xffffu);k12938(&k);
            k.d1=SETW(k.d1,(k.d1+8u)&0xffffu);k.d1=SWAP(k.d1);k.d0=SWAP(k.d0);
            if ((k.d7&0xffffu) && !(scene->w4018==0x3cu && (k.d7&0xffffu)==0x3cu)) {
                k129ec(&k);k12cba(&k);
                if (k.d6!=NONE) {k.d6-=0x80000u;record=true;}
            }
        }
        if (k.unsupported) return false;
        if (record && S(k.d6)>S(k.l405c)) {k.l405c=k.d6;k.l4064=k.d7;o4060=k.a0;}
        k1382a(&k);
    }
    k.d6=k.l405c;k.d7=k.l4064;k.d5=SETW(k.d5,k.d7);k12e64(&k);
    if (k.unsupported) return false;
    *d5=(uint16_t)k.d5;*d6=k.d6;*d7=k.d7;*object=o4060;
    return true;
}
/* 0128E8 for player 1 (A6 = 1014C4), before each of its sweeps 0143EE
 * (air) and 014594 (ground): 10C00A = 0, 10C00D = FF and the screen box
 * RAM 10C01C..10C023 copied from the record at RAM 10C2E8, ROM 016DBE =
 * 001C 0000 0108 01E0 (MAME: the same words at every call of player 1 in
 * Mission 2, stages 1 to 3). Without it 0138A0 clips every move to the
 * right (and up) to nothing at the box of another object, or at 0 x 0, and
 * the sweep never meets a wall there. The box is the call's own: the
 * world's (neo_slug.c, neo_tread.c) is put back. */
bool neo_kernel_player_sweep(neo_kscene_t *scene,const neo_kreq_t *request,neo_kres_t *result) {
    static const uint16_t box[4]={0x001c,0x0000,0x0108,0x01e0};   /* ROM 016DBE */
    uint16_t config[4];
    for (unsigned i=0;i<4u;i++) {config[i]=scene->config[i];scene->config[i]=box[i];}
    bool ok=neo_kernel_sweep(scene,request,result);
    for (unsigned i=0;i<4u;i++) scene->config[i]=config[i];
    return ok;
}
/* Player 1's air sweep 0143EE: 013968 from +28..+34 with 10C004 = 0,
 * 10C008 = FF (+15 bit 5 clear) and 10C009 = 0. On a floor (D6 bit 0,
 * 0144B8: +15 bit 6) of the terrain or the root, *out gets the swept
 * position and +30/+34 (014492) relative to the terrain and true is
 * returned; otherwise the persistent words 10C018/10C068 are put back, so
 * the caller's own sweeps see them unchanged. */
bool neo_kernel_air_landing(neo_kscene_t *scene,int32_t x,int32_t y,int32_t vx,int32_t vy,neo_kland_t *out) {
    uint16_t w4018=scene->w4018,w4068=scene->w4068;
    uint8_t self_dyn=scene->self_dyn;uint32_t self_block=scene->self_block;
    scene->self_dyn=0;scene->self_block=0;
    neo_kreq_t q={x,y,vx,vy,NEO_KOBJ_TERRAIN,0,0xff,0,0,0,0xff};
    neo_kres_t r;
    bool ok=neo_kernel_player_sweep(scene,&q,&r);
    scene->self_dyn=self_dyn;scene->self_block=self_block;
    if (ok && (r.flags&1u) && (r.parent==NEO_KOBJ_TERRAIN || r.parent==NEO_KOBJ_ROOT)) {
        if (r.parent==NEO_KOBJ_ROOT) {   /* 01AC4C: back to the terrain */
            r.x=(int32_t)((uint32_t)r.x+(uint32_t)scene->root.x-(uint32_t)scene->terrain.x);
            r.y=(int32_t)((uint32_t)r.y+(uint32_t)scene->root.y-(uint32_t)scene->terrain.y);
        }
        *out=(neo_kland_t){r.x,r.y,r.vx,r.vy,r.flags,r.kind};
        return true;
    }
    scene->w4018=w4018;scene->w4068=w4068;
    return false;
}
