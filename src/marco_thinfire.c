/* Thin fire states (marco_thinfire.h). */
#include "marco_thinfire.h"
#include <string.h>

/* (+76 write, END) of the form-1 programs (row +93*4+6E = 0x10), in player
 * updates from the one of 0C3CD0 (004260/00427E: a WAIT n is n updates; an
 * END right after a WAIT ends in its last update). tools/check_thin_fire.py
 * rebuilds this table from the ROM in `make check`. */
typedef struct { uint8_t t76,end; } timing_t;
static const timing_t level[16]={
    [2]={6,28},[7]={6,28},                       /* 0C3C10: 253896 */
    [3]={5,14},[8]={5,14},[12]={5,14},[13]={5,14},[14]={5,14},   /* 253A56 */
    [4]={2,12},[9]={2,12},                       /* 25391A */
};
static const timing_t up[16]={
    [2]={6,32},[7]={6,32},                       /* 0C3C50: 253962 */
    [3]={2,18},[8]={2,18},[12]={2,18},[13]={2,18},[14]={2,18},   /* 253AC6 -> 253A40 */
    [4]={2,16},[9]={2,16},                       /* 2539FA */
};
static const timing_t down[16]={
    [2]={2,12},[7]={2,12},                       /* 0C3C90: 256030 */
    [3]={2,10},[8]={2,10},[12]={2,10},[13]={2,10},[14]={2,10},   /* 255FDE */
    [4]={2,12},[9]={2,12},                       /* 256078 */
};
static const timing_t crouch[16]={
    [2]={6,28},[7]={6,28},                       /* 0C24DE: 0C0098 -> 256F26 */
    [3]={2,14},[8]={2,14},[12]={2,14},[13]={2,14},[14]={2,14},   /* 0C0398 -> 256EB6 */
    [4]={2,12},[9]={2,12},                       /* 0C0198 -> 256FAA */
};
/* END of the aim programs (tables 0BF9D8, 0BFA98, 0BFB18, 0BFB58, 0BFBD8,
 * 0BFC18, row 0x10: 25349E, 2534B4, 2534E4, 255F98, 255FAE, 255FC4). */
enum { AIM_UP=3, AIM_UP_LEVEL=3, AIM_UP_DOWN=7, AIM_DOWN=3, AIM_DOWN_LEVEL=3, AIM_DOWN_UP=7 };

bool marco_thin_program(uint8_t id,int aim,bool crouched,uint8_t *t76,uint8_t *end) {
    if (id>15) return false;
    const timing_t *t=crouched?&crouch[id]:aim>0?&up[id]:aim<0?&down[id]:&level[id];
    if (!t->end) return false;
    *t76=t->t76;*end=t->end;return true;
}
void marco_thin_reset(marco_thin_t *s) {memset(s,0,sizeof *s);s->grounded=true;}

/* 0C3CD0 (with the 0C3D9A of the same update) or the crouched 0C246A. */
static bool start(marco_thin_t *s,const marco_thin_in_t *in,bool crouched,marco_thin_out_t *out) {
    int8_t aim=crouched?0:in->up?1:in->down?-1:0;
    uint8_t t76,end;
    if (!marco_thin_program(in->id,aim,crouched,&t76,&end)) return false;
    s->latched=false;s->action98=2;
    if (!crouched) {s->aim92=aim;s->rest_bit5=false;}              /* 0C3CDC */
    s->step=0;s->t76=t76;s->end=end;s->wrote76=t76==0;
    s->state=crouched?THIN_CROUCH_ATTACK:THIN_WAIT;
    out->fire=true;out->aim=aim;out->crouched=crouched;
    return true;
}
/* 0C1EFA: kept with +92 > 0 or +98 != 0 (0C1F4A), else 0C1F24. */
static void drop_unless_busy(marco_thin_t *s) {
    if (s->aim92>0 || s->action98) {s->rest_bit5=true;return;}   /* 0C1F4A: +9A bit 5, +90 = 0 */
    s->aim92=0;s->state=THIN_DROPPED;s->action98=0;s->rest_bit5=false;   /* 0C1F24, 0C1F32 */
}
/* The rest 0C3A5E: fire, else the aim rules. */
static void rest(marco_thin_t *s,const marco_thin_in_t *in,bool fresh,marco_thin_out_t *out) {
    if (fresh && start(s,in,false,out)) return;
    uint8_t end=0;int8_t a=s->aim92;
    if (!s->aim92) {
        if (in->grounded) {if (in->up) {a=1;end=AIM_UP;}}               /* 0C4100 */
        else if (in->down) {a=-1;end=AIM_DOWN;}                          /* 0C4136 */
    } else if (s->aim92>0) {
        if (in->down) {a=-1;end=AIM_UP_DOWN;}                            /* 0C4124 */
        else if (!in->up) {a=0;end=AIM_UP_LEVEL;}                        /* 0C4112 */
    } else {
        if (in->up) {a=1;end=AIM_DOWN_UP;}                               /* 0C415A */
        else if (!in->down) {a=0;end=AIM_DOWN_LEVEL;}                    /* 0C4148 */
    }
    if (!end) return;
    /* 0C4168: +98 = 1, 0C4176 in the same update (its first step). */
    s->aim92=a;s->action98=1;s->state=THIN_AIM;s->step=0;s->end=end;
}
marco_thin_out_t marco_thin_step(marco_thin_t *s,const marco_thin_in_t *in) {
    marco_thin_out_t out={false,0,false};
    /* 0C0FEA: the press, unless the cap is reached (0C100C). */
    bool fresh=in->fresh && (in->knife || in->cap>in->live);
    /* The body first (0C32F4 calls +7C at its end). */
    int set90=-1;   /* the +90 the body wrote this update */
    if (in->crouch!=s->crouched) {
        s->crouched=in->crouch;s->walking=false;
        s->state=THIN_DROPPED;s->action98=0;s->latched=false;   /* 0C2214 / 0C1E34 -> 0C1F24 */
        if (!in->crouch) {s->aim92=0;s->rest_bit5=false;}         /* 0C1F24, 0C1F32 */
    } else if (!in->crouch && in->landed) {drop_unless_busy(s);}  /* 0C1EBE */
    if (in->crouch) {
        /* The crouched body: 0C2236/0C22B6/0C23F2 fire on a fresh press;
         * 0C2588 latches, steps the program, then tests +76. */
        if (s->state==THIN_CROUCH_NEXT) start(s,in,true,&out);
        else if (s->state==THIN_CROUCH_ATTACK) {
            if (fresh) s->latched=true;
            s->step++;
        } else if (fresh) start(s,in,true,&out);
        if (s->state==THIN_CROUCH_ATTACK) {
            if (s->step==s->t76) s->wrote76=true;
            bool next=s->wrote76 && s->latched;
            if (s->step>=s->end) s->state=THIN_DROPPED;
            if (next) s->state=THIN_CROUCH_NEXT;
        }
        s->grounded=in->grounded;
        return out;
    }
    if (!in->crouch) {
        if (in->jumped) {set90=2;s->walking=false;s->turn=0;}          /* 0C2642/0C2654: +90 = 3/2 */
        else if (in->grounded && s->grounded) {
            /* A ground routine runs this update (the update that finds the
             * ground still runs the air one: 0C26FC sets 0C1EBE). */
            bool stand=false;
            if (in->landed) stand=true;                                    /* 0C1F4A/0C1F24 -> 0C1FA6 */
            if (s->turn) {
                /* 0C27A8: the turn program (0BEE58/0BEE98 row 0x10: END in
                 * its second update), then 0C285C in the next. */
                if (!--s->turn) {s->walking=true;s->walk80=0x3f>>1;set90=1;}
            } else if (in->walking) {
                if (!s->walking) {s->walking=true;s->walk80=0x3f;set90=1;}  /* 0C285C: +90 = 1 */
                s->walk80>>=1;                                             /* 0C28A2 */
                if (in->turned) {
                    if (s->action98==2) {s->walk80=0x3f>>1;set90=1;}   /* 0C2764 -> 0C285C */
                    else if (s->action98!=1) {                             /* 0C2746..0C2780 */
                        s->state=THIN_DROPPED;s->walking=false;s->turn=2;
                    }
                }
            } else if (s->walking) {
                s->walking=false;s->walk80>>=1;                            /* 0C28A2 */
                if (s->walk80) {s->rest_bit5=true;set90=0;}            /* 0C28E6 -> 0C1F4A */
                else {drop_unless_busy(s);if (s->state!=THIN_DROPPED) set90=0;}   /* 0C1EFA */
                stand=true;
            } else stand=true;
            /* 0C1FA6: a fresh fire, up held or a grenade with +9A bit 5
             * clear -> 0C1F4A (+9A bit 5, +90 = 0). */
            if (stand && !s->walking && !s->turn && !s->rest_bit5 && (fresh || in->up)) {s->rest_bit5=true;set90=0;}
        }
    }
    s->grounded=in->grounded;
    /* The upper body (+7C). */
    switch (s->state) {
    case THIN_DROPPED:
        if (set90<0) break;
        /* 0C37CE: 0C397E (+90 = 0: +9A bit 5), 0C387E (1: +92 <= 0 -> 0),
         * 0C3844/0C3830 (2/3: +92 > 0 -> 0); +98 = 0; then 0C3A5E. */
        if (!set90) s->rest_bit5=true;
        else if (set90==1) {if (s->aim92<=0) s->aim92=0;}
        else if (s->aim92>0) s->aim92=0;
        s->state=THIN_REST;s->action98=0;
        rest(s,in,fresh,&out);
        break;
    case THIN_SETTLE:
        if (s->settle_pose0) s->rest_bit5=true;                                /* 0C397E (0C38B2 leaves it) */
        s->state=THIN_REST;s->action98=0;
        rest(s,in,fresh,&out);
        break;
    case THIN_REST:
        rest(s,in,fresh,&out);
        break;
    case THIN_AIM:
        if (fresh && start(s,in,false,&out)) break;                            /* 0C4182 */
        if (++s->step>=s->end) {s->state=THIN_SETTLE;s->settle_pose0=true;}    /* 0C4194 */
        return out;
    case THIN_NEXT:
        start(s,in,false,&out);
        break;
    case THIN_WAIT:
        if (fresh) s->latched=true;
        if (s->wrote76) {s->action98=0;if (s->latched) s->state=THIN_NEXT;}   /* 0C3DA6 */
        s->step++;
        break;
    default: break;
    }
    if (s->state==THIN_WAIT || s->state==THIN_NEXT) {
        if (s->step==s->t76) s->wrote76=true;
        if (s->step>=s->end && !out.fire) {s->state=THIN_SETTLE;s->settle_pose0=false;}   /* 0C3DF8 */
    } else if (s->state==THIN_AIM && s->step>=s->end) {s->state=THIN_SETTLE;s->settle_pose0=true;}
    return out;
}
