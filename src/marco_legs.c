/* Upper-body offset +72/+74 of the body programs (marco_legs.h). */
#include "marco_legs.h"
#include <string.h>
typedef struct { uint8_t wait; int8_t x,y; } entry_t;
/* 24DD18: four W1 poses, then twelve W2 poses looping to the fifth. */
static const entry_t run[16]={{1,-1,17},{1,-1,17},{1,-1,18},{1,-1,18},{2,-1,19},{2,-1,20},{2,-1,21},{2,-1,20},
    {2,-1,19},{2,-1,19},{2,-1,19},{2,-1,20},{2,-1,21},{2,-1,20},{2,-1,19},{2,-1,19}};
/* 24E4E0: up to the top (index 5 holds while airborne), then down; END. */
static const entry_t jump[12]={{2,1,24},{2,1,23},{1,1,21},{1,1,19},{1,1,18},{1,1,18},
    {1,1,19},{1,1,20},{1,1,21},{1,1,22},{1,1,23},{1,1,24}};
/* 24E858: index 3 holds while airborne; then 24DF6C (-1,15) for ever. */
static const entry_t run_jump[8]={{1,-3,15},{2,-3,12},{3,-4,12},{1,-4,12},{2,-4,12},{2,-3,12},{2,-2,11},{2,-1,11}};
static void start(marco_legs_t *l,marco_legs_program_t p) {l->program=p;l->index=0;l->wait=0;l->started=true;}
void marco_legs_reset(marco_legs_t *l) {memset(l,0,sizeof *l);l->grounded=true;l->x72=0;l->y74=17;}
void marco_legs_step(marco_legs_t *l,bool grounded,bool moving,bool crouch,bool running_jump,bool hgun) {
    /* The program of the situation (marco_visual_step's body choice). */
    if (!grounded) {
        if (l->grounded || !l->started) start(l,running_jump?MARCO_LEGS_RUN_JUMP:MARCO_LEGS_JUMP);
    } else if (crouch) {
        if (l->program!=MARCO_LEGS_CROUCH) start(l,MARCO_LEGS_CROUCH);
    } else {
        marco_legs_program_t p=moving?MARCO_LEGS_RUN:MARCO_LEGS_IDLE;
        bool same=l->program==p || (p==MARCO_LEGS_RUN && l->program==MARCO_LEGS_RUN_LOOP);
        if (!l->grounded || !same || !l->started) start(l,p);
    }
    l->grounded=grounded;
    /* The entry of this update and its wait (speed 0x100: one per update). */
    const entry_t *e=NULL;
    switch (l->program) {
    case MARCO_LEGS_IDLE: l->x72=0;l->y74=17;return;
    case MARCO_LEGS_CROUCH: l->x72=hgun?4:0;l->y74=hgun?12:6;return;
    case MARCO_LEGS_RUN_LOOP: l->x72=-1;l->y74=15;return;
    case MARCO_LEGS_RUN: e=&run[l->index];break;
    case MARCO_LEGS_JUMP: e=&jump[l->index];break;
    case MARCO_LEGS_RUN_JUMP: e=&run_jump[l->index];break;
    }
    l->x72=e->x;l->y74=e->y;
    if (++l->wait<e->wait) return;
    l->wait=0;
    switch (l->program) {
    case MARCO_LEGS_RUN: l->index=l->index==15?4:l->index+1;break;
    case MARCO_LEGS_JUMP: if (!(l->index==5 && !grounded) && l->index<11) l->index++;break;
    case MARCO_LEGS_RUN_JUMP:
        if (l->index==3 && !grounded) break;
        if (l->index==7) l->program=MARCO_LEGS_RUN_LOOP; else l->index++;
        break;
    default: break;
    }
}
/* 0C4204: rows down/level/up, forms 0..3, words (x, y up). */
static const int8_t muzzle_table[3][4][2]={
    {{0,-16},{-4,-20},{16,0},{8,-16}},{{16,10},{24,4},{24,8},{20,4}},{{0,40},{0,48},{8,32},{5,32}}};
marco_muzzle_t marco_muzzle_aim(const marco_legs_t *l,int aim92,uint8_t form,bool facing_left) {
    unsigned row=aim92>0?2u:aim92<0?0u:1u;form&=3u;
    int x=muzzle_table[row][form][0]+l->x72,y=muzzle_table[row][form][1]+l->y74;
    if (facing_left) x=-x;                                        /* 0C42A0 */
    return (marco_muzzle_t){(int16_t)x,(int16_t)y};
}
marco_muzzle_t marco_muzzle_sweep(const marco_legs_t *l,uint8_t angle,int16_t sine,int16_t cosine) {
    /* 002E7C with d0 = 0xC0000: muls.w of 0xC00 by the table words; the
     * .w arithmetic of 0C45D0..0C45D4 only touches the fractions. */
    int32_t s=(int32_t)0xc00*sine,c=(int32_t)0xc00*cosine;
    int16_t xi=(int16_t)((uint32_t)c>>16),yi=(int16_t)((uint32_t)s>>16);
    int y=yi;
    if (!(angle&0x80u)) y+=yi>>1;                                 /* 0C45EC..0C45F6 */
    return (marco_muzzle_t){(int16_t)(xi+l->x72),(int16_t)(y+l->y74+2)};
}
bool marco_legs_entry(marco_legs_program_t program,unsigned i,int8_t *x72,int8_t *y74) {
    const entry_t *t=NULL;unsigned n=1;int8_t x=0,y=17;
    switch (program) {
    case MARCO_LEGS_IDLE: break;
    case MARCO_LEGS_CROUCH: x=4;y=12;break;   /* the H's (pistol: (0,6)) */
    case MARCO_LEGS_RUN_LOOP: x=-1;y=15;break;
    case MARCO_LEGS_RUN: t=run;n=16;break;
    case MARCO_LEGS_JUMP: t=jump;n=12;break;
    case MARCO_LEGS_RUN_JUMP: t=run_jump;n=8;break;
    }
    if (i>=n) return false;
    if (t) {x=t[i].x;y=t[i].y;}
    *x72=x;*y74=y;return true;
}
