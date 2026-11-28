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
/* The body programs' commands that matter here, in the converted format of
 * tools/convert_marco_programs.py (POSE without a descriptor: the drawing is
 * marco_visual.c's): +72/+74 writes, waits, the +78 branch (FF while
 * airborne) and the loops. */
#define WW(f,v) {NEO_ANIM_WRITE_WORD,(v),(f),0,0,0,0}
#define WL(v) {NEO_ANIM_WRITE_LONG,(int32_t)(v),0x72,0,0,0,0}   /* sign-extended 24-bit (neo_animation.h) */
#define P {NEO_ANIM_POSE,0,0,0,64,0,0}
#define W(n) {NEO_ANIM_WAIT,0,0,(n),0,0,0}
#define RUNP(y,w) WW(0x74,y),P,W(w)
static const neo_anim_command_t run_cmds[]={   /* 24DD18 */
    WW(0x72,0xffff),RUNP(17,1),RUNP(17,1),RUNP(18,1),RUNP(18,1),
    RUNP(19,2),RUNP(20,2),RUNP(21,2),RUNP(20,2),RUNP(19,2),RUNP(19,2),RUNP(19,2),RUNP(20,2),RUNP(21,2),RUNP(20,2),RUNP(19,2),RUNP(19,2),
    {NEO_ANIM_JUMP,0,13,0,0,0,0},
};
static const neo_anim_command_t jump_cmds[]={  /* 24E4E0 */
    WL(0x10018),P,W(2),WL(0x10017),P,W(2),WL(0x10015),P,W(1),WL(0x10013),P,W(1),WL(0x10012),P,W(1),
    WL(0x10012),P,W(1),{NEO_ANIM_BRANCH_FIELD,0x78,16,255,0,0,0},
    WL(0x10013),P,W(1),WL(0x10014),P,W(1),WL(0x10015),P,W(1),WL(0x10016),P,W(1),WL(0x10017),P,W(1),WL(0x10018),P,W(0),
    {NEO_ANIM_FINISH,0,0,0,0,0,0},
};
static const neo_anim_command_t run_jump_cmds[]={  /* 24E858, then 24DF6C */
    WL(0xfffd000f),P,W(1),WL(0xfffd000c),P,W(2),WL(0xfffc000c),P,W(3),WL(0xfffc000c),P,W(1),
    {NEO_ANIM_BRANCH_FIELD,0x78,10,255,0,0,0},
    WL(0xfffc000c),P,W(2),WL(0xfffd000c),P,W(2),WL(0xfffe000b),P,W(2),WL(0xffff000b),P,W(2),
    WL(0xffff000f),P,W(2),P,W(2),P,W(2),P,W(2),P,W(2),P,W(2),P,W(2),P,W(2),P,W(2),P,W(2),P,W(2),P,W(2),
    {NEO_ANIM_JUMP,0,25,0,0,0,0},
};
/* Form 2 (fat Marco, 0C0F32 rows +0x20): the walk 259D68 writes the long
 * +72 = 0F/11/11/11/11/10/0F/11/11/11/11/10 (x 0), one per W2 pose, and
 * loops (MAME capture_fat_weapons.lua: +26 = 01C1, +77 the pose); the idle
 * 259D4C and the jump 259E64 write 0x10 once. */
static const neo_anim_command_t fat_run_cmds[]={   /* 259D68 */
    WL(0xf),P,W(2),WL(0x11),P,W(2),WL(0x11),P,W(2),WL(0x11),P,W(2),WL(0x11),P,W(2),WL(0x10),P,W(2),
    WL(0xf),P,W(2),WL(0x11),P,W(2),WL(0x11),P,W(2),WL(0x11),P,W(2),WL(0x11),P,W(2),WL(0x10),P,W(2),
    {NEO_ANIM_JUMP,0,0,0,0,0,0},
};
#undef WW
#undef WL
#undef P
#undef W
#undef RUNP
static const neo_anim_program_t programs[]={
    {run_cmds,sizeof run_cmds/sizeof *run_cmds},{jump_cmds,sizeof jump_cmds/sizeof *jump_cmds},
    {run_jump_cmds,sizeof run_jump_cmds/sizeof *run_jump_cmds},{fat_run_cmds,sizeof fat_run_cmds/sizeof *fat_run_cmds}};
static bool field(void *context,unsigned f,unsigned bytes,bool write,uint32_t *value) {
    marco_legs_t *l=context;
    if (f==0x78 && !write) {*value=l->rising?0xffu:0u;return true;}
    if (!write) return false;
    if (f==0x72 && bytes==4) {l->x72=(int8_t)(int16_t)(*value>>16);l->y74=(int8_t)(int16_t)*value;return true;}
    if (f==0x72) {l->x72=(int8_t)(int16_t)*value;return true;}
    if (f==0x74) {l->y74=(int8_t)(int16_t)*value;return true;}
    return true;
}
/* Body speed +26: 0x1C1 for the run (C285C, marco_motion.c), else 0x100. */
static void start(marco_legs_t *l,marco_legs_program_t p) {
    l->program=p;l->started=true;l->done=false;
    if (l->fat) {if (p==MARCO_LEGS_RUN) neo_anim_start(&l->anim,&programs[3],0x1c1);return;}
    if (p==MARCO_LEGS_RUN) neo_anim_start(&l->anim,&programs[0],0x1c1);
    else if (p==MARCO_LEGS_JUMP) neo_anim_start(&l->anim,&programs[1],0x100);
    else if (p==MARCO_LEGS_RUN_JUMP) neo_anim_start(&l->anim,&programs[2],0x100);
}
void marco_legs_reset(marco_legs_t *l) {memset(l,0,sizeof *l);l->grounded=true;l->x72=0;l->y74=17;}
void marco_legs_set_fat(marco_legs_t *l,bool fat) {
    if (l->fat==fat) return;
    /* The form's tables choose new body programs (0C0F32 + 0x20). */
    l->fat=fat;l->started=false;
}
void marco_legs_step(marco_legs_t *l,bool grounded,bool moving,bool crouch,bool running_jump,bool hgun,bool rising) {
    l->rising=rising;
    /* The program of the situation (marco_visual_step's body choice). */
    if (!grounded) {
        if (l->grounded || !l->started) start(l,running_jump?MARCO_LEGS_RUN_JUMP:MARCO_LEGS_JUMP);
    } else if (crouch) {
        if (l->program!=MARCO_LEGS_CROUCH || !l->started) start(l,MARCO_LEGS_CROUCH);
    } else {
        marco_legs_program_t p=moving?MARCO_LEGS_RUN:MARCO_LEGS_IDLE;
        if (!l->grounded || l->program!=p || !l->started) start(l,p);
    }
    l->grounded=grounded;
    if (l->fat && l->program!=MARCO_LEGS_RUN) {l->x72=0;l->y74=16;return;}   /* 259D4C, 259E64; crouched: kept */
    switch (l->program) {
    case MARCO_LEGS_IDLE: l->x72=0;l->y74=17;return;          /* 24C5EA */
    case MARCO_LEGS_CROUCH: l->x72=hgun?4:0;l->y74=hgun?12:6;return;
    default: break;
    }
    if (l->done) return;
    neo_anim_render_t r;memset(&r,0,sizeof r);
    if (neo_anim_step_fields(&l->anim,&r,NULL,NULL,256,NULL,NULL,field,l)!=NEO_ANIM_MORE) l->done=true;
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
marco_muzzle_t marco_muzzle_sweep_fat(uint8_t angle,int16_t sine,int16_t cosine,bool facing_left) {
    (void)angle;   /* the caller gives the sine and cosine of it */
    /* 002E7C with d0 = 0x100000: muls.w of 0x1000; x = int / 2 + 7, y =
     * int + 8, x negated facing left (0C46B6..0C46CA); no +72/+74. */
    int32_t s=(int32_t)0x1000*sine,c=(int32_t)0x1000*cosine;
    int16_t xi=(int16_t)((uint32_t)c>>16),yi=(int16_t)((uint32_t)s>>16);
    int x=(xi>>1)+7;
    return (marco_muzzle_t){(int16_t)(facing_left?-x:x),(int16_t)(yi+8)};
}
uint8_t marco_sweep_fat_angle(uint8_t angle) {
    /* 0C469E..0C46A8 on the program's (unmirrored) word: a - 0x40 less an
     * eighth of it, + 0x40. */
    int16_t d1=(int16_t)(angle-0x40),d2=(int16_t)(d1>>3);
    return (uint8_t)(d1-d2+0x40);
}
/* 259D68 (fat walk). */
static const entry_t fat_run[12]={{2,0,15},{2,0,17},{2,0,17},{2,0,17},{2,0,17},{2,0,16},{2,0,15},{2,0,17},{2,0,17},{2,0,17},{2,0,17},{2,0,16}};
bool marco_legs_fat_entry(marco_legs_program_t program,unsigned i,int8_t *x72,int8_t *y74) {
    if (program!=MARCO_LEGS_RUN) {if (i) return false;*x72=0;*y74=16;return true;}
    if (i>=12) return false;
    *x72=fat_run[i].x;*y74=fat_run[i].y;return true;
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
