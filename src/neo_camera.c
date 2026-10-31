#include "neo_camera.h"
#include <string.h>
/* See neo_camera.h. Translated instruction by instruction; 68000 flag tests
 * are kept (bpl/bmi read the sign of the 32-bit difference, bcs/bhi/bls are
 * unsigned) because the bounds use 0x8000 sentinels and wrap-around. */
#define NEG(v) ((int32_t)(0u-(uint32_t)(v)))
static bool minus(int32_t a,int32_t b) {return (int32_t)((uint32_t)a-(uint32_t)b)<0;} /* N of a-b */
static uint16_t lw(const neo_camera_t *c,unsigned o) {
    return o+1u<c->list_size?(uint16_t)(c->list[o]<<8|c->list[o+1]):0xffffu;
}
static uint32_t ll(const neo_camera_t *c,unsigned o) {return (uint32_t)lw(c,o)<<16|lw(c,o+2);}
static void event(neo_camera_t *c,uint8_t kind,uint32_t address,uint16_t value) {
    if (c->event_count<NEO_CAMERA_EVENTS) c->events[c->event_count++]=(neo_camera_event_t){kind,address,value,neo_camera_progress(c)};
    else c->event_dropped++;
}
void neo_camera_init(neo_camera_t *c,const uint8_t *list,uint16_t size,uint32_t base) {
    memset(c,0,sizeof(*c));
    c->list=list;c->list_size=size;c->list_base=base;
    c->lock=0xffffffffu;                         /* 0C7C50 */
    c->speed_x=c->speed_y=0x000a0000;            /* mission actor +72/+76 (measured) */
}
/* ROM 0C7ECE: progress of a camera position along the current path. */
static uint32_t progress_of(const neo_camera_t *c,int32_t x,int32_t y) {
    uint32_t d7;
    switch (c->mode) {
    case 1: d7=(uint32_t)NEG((uint32_t)x-(uint32_t)c->path_x)+c->target;break;
    case 2: d7=(uint32_t)y-(uint32_t)c->path_y+c->target;break;
    case 3: d7=(uint32_t)NEG((uint32_t)y-(uint32_t)c->path_y)+c->target;break;
    default: d7=(uint32_t)x-(uint32_t)c->path_x+c->target;break;
    }
    if ((int32_t)d7<0) return 0;
    return d7<c->target?d7:c->target;
}
/* ROM 0C7F3A: path point of a progress value. */
static void point_of(neo_camera_t *c,uint32_t d7,int32_t *x,int32_t *y) {
    int32_t d=(int32_t)(d7-c->target);
    switch (c->mode) {
    case 0: *x=(int32_t)((uint32_t)d+(uint32_t)c->path_x);*y=c->path_y;return;
    case 1: *x=(int32_t)((uint32_t)NEG(d)+(uint32_t)c->path_x);*y=c->path_y;return;
    case 2: *x=c->path_x;*y=(int32_t)((uint32_t)d+(uint32_t)c->path_y);return;
    case 3: *x=c->path_x;*y=(int32_t)((uint32_t)NEG(d)+(uint32_t)c->path_y);return;
    case 4: *y=(int32_t)((uint32_t)(d>>2)+(uint32_t)c->path_y);break;
    case 5: *y=(int32_t)((uint32_t)d+(uint32_t)c->path_y);break;
    case 6: *y=(int32_t)((uint32_t)NEG(d>>2)+(uint32_t)c->path_y);break;
    case 7: *y=(int32_t)((uint32_t)NEG(d)+(uint32_t)c->path_y);break;
    default: /* trap #F */
        c->unsupported++;c->unsupported_at=0x0c8012;*x=*y=0;return;
    }
    *x=(int32_t)((uint32_t)d+(uint32_t)c->path_x);
}
/* ROM 0C7EBC. */
static void set_target(neo_camera_t *c,uint32_t d7) {
    int32_t x,y;point_of(c,d7,&x,&y);c->target=d7;c->path_x=x;c->path_y=y;
}
void neo_camera_move(neo_camera_t *c,int32_t d0,int32_t d1) {
    const int32_t d4=d0,d5=d1;
    d0=(int32_t)((uint32_t)d0+(uint32_t)c->x);d1=(int32_t)((uint32_t)d1+(uint32_t)c->y);
    uint32_t d7=progress_of(c,d0,d1);
    int32_t st[4]={(int32_t)((uint32_t)(uint16_t)c->bounds[0]<<16),(int32_t)((uint32_t)(uint16_t)c->bounds[2]<<16),
                   (int32_t)((uint32_t)(uint16_t)c->bounds[1]<<16),(int32_t)((uint32_t)(uint16_t)c->bounds[3]<<16)};
    if (c->lock!=0xffffffffu) {
        if (d7>c->lock) d7=c->lock;
        static const unsigned slot[4]={0,2,1,3};  /* 69C0->(a7), 69C2->8(a7), 69C4->4(a7), 69C6->C(a7) */
        for (unsigned i=0;i<4;i++) if ((uint16_t)c->lock_bounds[i]!=0x8000u) st[slot[i]]=(int32_t)((uint32_t)(uint16_t)c->lock_bounds[i]<<16);
    }
    if (d7>=c->progress) c->progress=d7; else d7=c->progress;
    int32_t px,py;point_of(c,d7,&px,&py);
    if ((uint16_t)((uint32_t)st[0]>>16)!=0x8000u) st[0]=(int32_t)((uint32_t)st[0]+(uint32_t)px);
    if ((uint16_t)((uint32_t)st[1]>>16)!=0x8000u) st[1]=(int32_t)((uint32_t)st[1]+(uint32_t)px);
    if ((uint16_t)((uint32_t)st[2]>>16)!=0x8000u) st[2]=(int32_t)((uint32_t)st[2]+(uint32_t)py);
    if ((uint16_t)((uint32_t)st[3]>>16)!=0x8000u) st[3]=(int32_t)((uint32_t)st[3]+(uint32_t)py);
    const int32_t none=(int32_t)0x80000000u;
    if (d4<0) {
        int32_t b=st[0];
        if (b!=none && minus(d0,b)) {d0=c->x;if (!minus(d0,b)) d0=b;}
    } else {
        int32_t b=st[1];
        if (b!=none && !minus(d0,b)) {d0=c->x;if (minus(d0,b)) d0=b;}
    }
    if (d5<0) {
        int32_t b=st[2];
        if (b!=none && minus(d1,b)) {d1=c->y;if (!minus(d1,b)) d1=b;}
    } else {
        int32_t b=st[3];
        if (b!=none && !minus(d1,b)) {d1=c->y;if (minus(d1,b)) d1=b;}
    }
    int32_t ox=c->x,oy=c->y;
    c->x=d0;c->y=d1;
    c->dx=(int32_t)((uint32_t)d0-(uint32_t)ox);c->dy=(int32_t)((uint32_t)d1-(uint32_t)oy);
    c->root_vx=NEG(c->dx);c->root_vy=NEG(c->dy);
    /* 0C7E3E..0C7EB6: visible rectangle (words). */
    int16_t cx=(int16_t)((uint32_t)c->x>>16),cy=(int16_t)((uint32_t)c->y>>16);
    for (unsigned i=0;i<4;i++) {
        int16_t v=(int16_t)((uint32_t)st[i]>>16);
        bool sentinel=(uint16_t)v==0x8000u;
        if (i==0) {if (!sentinel) {v=(int16_t)(v-cx);if (v>=0) v=0;}}
        else if (i==1) {if (!sentinel) {v=(int16_t)(v-cx);if (v<0) v=0;v=(int16_t)(v+0x140);}v=(int16_t)(v-1);}
        else if (i==2) {if (!sentinel) {v=(int16_t)(v-cy);if (v>=0) v=0;v=(int16_t)(v+0x100);}}
        else {if (!sentinel) {v=(int16_t)(v-cy);if (v<0) v=0;v=(int16_t)(v+0x200);}v=(int16_t)(v-1);}
        c->view[i]=v;
    }
}
/* ROM 0C804A + 0C8038. */
static void shake_decay(neo_camera_t *c) {
    int32_t d0=c->shake,d1=d0,d2=d1;
    d1=(int32_t)((uint32_t)d1+(uint32_t)d1);d2>>=4;d1=(int32_t)((uint32_t)d1-(uint32_t)d2);
    d0=(int32_t)((uint32_t)d0-(uint32_t)d1);
    if (d0>0x80000) d0=0x80000; else if (d0<(int32_t)0xfff80000u) d0=(int32_t)0xfff80000u;
    c->shake=d0;
    c->shake_applied=(int32_t)(((uint32_t)c->shake+0x8000u)&0xffff0000u);
}
void neo_camera_shake_start(neo_camera_t *c) {c->shake=(int32_t)(((uint32_t)c->shake&0xffffu)|(8u<<16));}
void neo_camera_shake_pulse(neo_camera_t *c) {
    uint32_t v=(uint32_t)c->shake;uint16_t hi=(uint16_t)(v>>16);
    if (!(hi&0x8000u)) {c->shake=(int32_t)((v&0xffffu)|((uint32_t)(hi|3u)<<16));return;}
    c->shake=(int32_t)((v&0xffffu)|((uint32_t)(hi&0xfffcu)<<16));
    c->shake_applied=(int32_t)(((uint32_t)c->shake+0x8000u)&0xffff0000u);
}
void neo_camera_commands(neo_camera_t *c,uint16_t *p8,uint16_t *pa) {
    for (unsigned guard=0;guard<64;guard++) {
        unsigned a=c->command;uint16_t op=lw(c,a),n=lw(c,a+2);
        if (op<0x0c) {
            if (op==0) {
                uint32_t d7=(uint32_t)lw(c,a+4)<<16;set_target(c,d7);
                if (d7>c->progress) return;
            } else if (op==8) {
                set_target(c,(uint32_t)lw(c,a+4)<<16);c->held=true;return;
            } else {
                /* 0C84BE (Mission 2's lists): wait until every present
                 * player's +3C/+40 is inside the rectangle (+8/+A relative
                 * to the layer object's +14/+18, size +C/+E). The layer is
                 * 10C72E in every list; its +14/+18 are the scene root's
                 * words (run4 RAM: -camera x and 256 - camera y). */
                int16_t lx=(int16_t)((uint32_t)NEG(c->x)>>16),ly=(int16_t)((0x01000000u-(uint32_t)c->y)>>16);
                uint16_t d2=(uint16_t)(lw(c,a+8)+(uint16_t)lx),d3=(uint16_t)(lw(c,a+10)+(uint16_t)ly),w=lw(c,a+12),h=lw(c,a+14);
                bool in[2],any=false;
                for (unsigned i=0;i<2;i++) {
                    in[i]=!c->op4_players[i].alive || ((uint16_t)(c->op4_players[i].px-d2)<w && (uint16_t)(c->op4_players[i].py-d3)<h);
                    any|=c->op4_players[i].alive;
                }
                if (!any || !in[0] || !in[1]) return;
            }
        } else switch (op) {
        case 0x0c: event(c,NEO_CAMERA_EV_PALETTE,ll(c,a+4),0);break;
        case 0x10: c->mode=lw(c,a+4);break;
        case 0x14: { /* 0C85F0 -> 0C7C70 */
            c->path_x=(int32_t)((uint32_t)lw(c,a+4)<<16);c->path_y=(int32_t)((uint32_t)lw(c,a+6)<<16);
            c->x=c->path_x;c->y=c->path_y;event(c,NEO_CAMERA_EV_REBASE_LAYER,0,0);break;
        }
        case 0x18: case 0x1c: case 0x20: case 0x24: case 0x28: case 0x44: case 0x48:
            event(c,(uint8_t)op,ll(c,a+4),0);break;
        case 0x2c: event(c,NEO_CAMERA_EV_CALL,ll(c,a+4),0);break;
        case 0x30: for (unsigned i=0;i<4;i++) c->bounds[i]=(int16_t)lw(c,a+4+2*i);break;
        case 0x34: *p8=lw(c,a+4);*pa=lw(c,a+6);break;
        case 0x38: event(c,NEO_CAMERA_EV_SOUND,0,lw(c,a+4));break;
        case 0x3c: for (unsigned i=0;i<4;i++) c->layer_flags[i]=(uint8_t)(lw(c,a+4+(i&2))>>((i&1)?0:8));event(c,NEO_CAMERA_EV_LAYER_FLAGS,0,0);break;
        case 0x40: c->auto_mode=(uint8_t)(lw(c,a+4)>>8);break;
        case 0x4c: event(c,NEO_CAMERA_EV_GRAPHICS,ll(c,a+4),0);break;
        case 0x50: break; /* RAM 10C00E long: not read by the ported code */
        case 0x54: if (((uint32_t)c->restart<<16)>=c->target) event(c,NEO_CAMERA_EV_COND_CALL,ll(c,a+4),0);break;
        case 0x58: if (((uint32_t)c->restart<<16)<c->target) event(c,NEO_CAMERA_EV_COND_CALL,ll(c,a+4),0);break;
        default: c->unsupported++;c->unsupported_at=c->list_base+a;return;
        }
        if (!n) {c->unsupported++;c->unsupported_at=c->list_base+a;return;}
        c->command=(uint16_t)(a+n);
    }
}
/* ROM 01903A with RAM 10C322 (`follow`). Returns D7 (0 = no player). */
static uint16_t player_point(const neo_camera_player_t p[2],bool follow,uint16_t p8,uint16_t pa,uint16_t out[4]) {
    uint16_t d0=p8,d1=pa,d2=d0,d3=d1,d7=0;
    if (!follow) return 0;
    if (p[0].alive) {d7=0xff00u;if (p[0].follow_x) d0=p[0].px;if (p[0].follow_y) d1=p[0].py;}
    if (p[1].alive) {d7|=0xffu;if (p[1].follow_x) d2=p[1].px;if (p[1].follow_y) d3=p[1].py;}
    if (!p[1].alive) {d2=d0;d3=d1;}
    if (!p[0].alive) {d0=d2;d1=d3;}
    out[0]=d0;out[1]=d1;out[2]=d2;out[3]=d3;
    return d7;
}
/* ROM 018E9E: smoothing toward the requested velocity. */
static int32_t smooth(int32_t d0,int32_t previous_root) {
    if (!d0) return 0;
    int32_t d2=d0,v=NEG(previous_root);
    if (d0>0) {if (v<0) v=0;} else {if (v>=0) v=0;}
    d2=(int32_t)((uint32_t)d2-(uint32_t)v);d2>>=2;
    return (int32_t)((uint32_t)v+(uint32_t)d2);
}
static int32_t clamp_speed(int32_t d0,int32_t limit) {
    if (d0>limit) return limit;
    if (d0<NEG(limit)) return NEG(limit);
    return d0;
}
void neo_camera_track(neo_camera_t *c,const neo_camera_player_t players[2],bool follow,uint16_t p8,uint16_t pa) {
    uint16_t q[4];int32_t d0=0,d1=0;
    if (player_point(players,follow,p8,pa,q)) {
        int16_t x0=(int16_t)q[0],y0=(int16_t)q[1],x1=(int16_t)q[2],y1=(int16_t)q[3];
        if (x1<x0) {int16_t t=x0;x0=x1;x1=t;}
        if (y1<y0) {int16_t t=y0;y0=y1;y1=t;}
        int16_t d4=(int16_t)((int16_t)(x0+x1)>>1);d4=(int16_t)(d4-(int16_t)p8);
        int16_t w;
        if (d4>=0) {w=(int16_t)(x0-0x2c);if (w<0) w=0; else if (w>d4) w=d4;}
        else {w=(int16_t)(x1-0x113);if (w>=0) w=0; else if (d4>w) w=d4;}
        d0=(int32_t)w*0x3000;
        d4=(int16_t)((int16_t)(y0+y1)>>1);d4=(int16_t)(d4-(int16_t)pa);
        if (d4>=0) {w=y0;if (w<0) w=0; else if (w>d4) w=d4;}
        else {w=(int16_t)(y1-0x1df);if (w>=0) w=0; else if (d4>w) w=d4;}
        d1=(int32_t)w*0x3000;
        d0=clamp_speed(d0,c->speed_x);d1=clamp_speed(d1,c->speed_y);
        d0=smooth(d0,c->root_vx);d1=smooth(d1,c->root_vy);
    }
    /* 0C81C0 */
    if (c->auto_mode&1u) d0=c->auto_vx;
    if (c->auto_mode&2u) d1=c->auto_vy;
    neo_camera_move(c,d0,d1);
    c->root_shake=c->shake_applied;
}
void neo_camera_finish(neo_camera_t *c,uint16_t *p8,uint16_t *pa) {
    c->event_count=0;
    shake_decay(c);
    neo_camera_commands(c,p8,pa);
}
void neo_camera_update(neo_camera_t *c,const neo_camera_player_t players[2],bool follow,uint16_t *p8,uint16_t *pa) {
    for (unsigned i=0;i<2;i++) {c->op4_players[i].alive=players[i].alive;c->op4_players[i].px=players[i].px;c->op4_players[i].py=players[i].py;}
    neo_camera_track(c,players,follow,*p8,*pa);
    neo_camera_finish(c,p8,pa);
}
void neo_camera_lock(neo_camera_t *c,uint16_t x,const uint16_t b[4]) {
    c->lock=(uint32_t)x<<16;for (unsigned i=0;i<4;i++) c->lock_bounds[i]=(int16_t)b[i];
}
void neo_camera_release(neo_camera_t *c) {c->lock=0xffffffffu;}
void neo_camera_autoscroll(neo_camera_t *c) {
    if (c->auto_mode) return;
    c->auto_owner=0xff;
    switch (c->mode) {
    case 1: c->auto_vx=(int32_t)0xfffd0000u;c->auto_mode=1;break;
    case 2: c->auto_vy=0x30000;c->auto_mode=2;break;
    case 3: c->auto_vy=(int32_t)0xfffd0000u;c->auto_mode=2;break;
    default: c->auto_vx=0x30000;c->auto_mode=1;break;
    }
}
void neo_camera_autoscroll_stop(neo_camera_t *c) {if (c->auto_owner) {c->auto_owner=0;c->auto_mode=0;}}
