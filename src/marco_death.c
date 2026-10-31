/* Player death and respawn (marco_death.h). The programs are the ones
 * 0C0F32 selects for Marco (+6E = 0; +93 has no entries of its own, the
 * 0C0F4C fallback takes +93 = 0) from bank 1, converted to native commands
 * (WAIT/POSE/BRANCH_FIELD/FINISH, the formats of
 * tools/convert_marco_programs.py). */
#include "marco_death.h"
#include <string.h>

#define P(d) {NEO_ANIM_POSE,(d),0,0,64,0,0}
#define PY(d,y) {NEO_ANIM_POSE,(d),0,0,64,0,(y)}   /* POSE with its dy byte */
#define W(n) {NEO_ANIM_WAIT,0,0,(n),0,0,0}
/* 0BF298: 25009E */
static const neo_anim_command_t ground_cmds[]={
    {NEO_ANIM_WRITE_BYTE,0xff,0x78,0,0,0,0},PY(0x18ba26,-4),W(1),P(0x18ba3e),W(1),P(0x18ba52),W(1),P(0x18ba66),
    W(1),P(0x18ba82),W(1),P(0x18ba9e),W(1),P(0x18bab8),W(1),P(0x18bacc),
    W(2),P(0x18bae0),W(3),P(0x18bb00),W(1),{NEO_ANIM_BRANCH_FIELD,0x78,19,1,0,0,0},P(0x18bb14),W(2),
    P(0x18bb28),W(2),P(0x18bb40),W(2),P(0x18bb58),W(2),P(0x18bb6c),W(1),
    P(0x18bb84),W(1),P(0x18bb9c),W(1),P(0x18bbb0),W(2),P(0x18bbc8),W(2),
    {NEO_ANIM_FINISH,0,0,0,0,0,0},
};
/* 0BF318: 25016A */
static const neo_anim_command_t air_cmds[]={
    {NEO_ANIM_WRITE_BYTE,0xff,0x78,0,0,0,0},P(0x18bd6a),W(1),P(0x18bd82),W(2),P(0x18bd9c),W(3),P(0x18bdb6),
    W(3),P(0x18bdca),W(3),P(0x18bdf2),W(1),{NEO_ANIM_BRANCH_FIELD,0x78,11,1,0,0,0},P(0x18be06),W(2),
    P(0x18be1a),W(2),P(0x18be30),W(2),P(0x18be1a),W(2),{NEO_ANIM_FINISH,0,0,0,0,0,0},
};
/* 0BF3D8: 2503D4 */
static const neo_anim_command_t kind2_ground_cmds[]={
    {NEO_ANIM_WRITE_BYTE,0xff,0x78,0,0,0,0},P(0x18bbe8),W(1),P(0x18bc00),W(1),P(0x18bc14),W(2),P(0x18bc2c),
    W(2),P(0x18bc44),W(2),P(0x18bc5c),W(2),P(0x18bc74),W(2),P(0x18bc8c),
    W(1),P(0x18bc8c),W(1),{NEO_ANIM_BRANCH_FIELD,0x78,17,1,0,0,0},P(0x18bca4),W(3),P(0x18bcbc),W(3),
    P(0x18bcd4),W(2),P(0x18bce4),W(1),P(0x18bcf4),W(1),P(0x18bd04),W(1),
    P(0x18bd14),W(1),P(0x18bd24),W(2),P(0x18bd34),W(3),P(0x18bd40),W(3),
    P(0x18bd5c),W(1),{NEO_ANIM_FINISH,0,0,0,0,0,0},
};
/* 0BF458 (kind 2 in the air) is 25016A too: air_cmds. */
/* 0BF358: 2501DC */
static const neo_anim_command_t kind7_cmds[]={
    {NEO_ANIM_WRITE_BYTE,0xff,0x78,0,0,0,0},P(0x18c9c4),W(1),P(0x18c9e8),W(1),P(0x18ca0c),W(1),P(0x18ca38),
    W(1),P(0x18ca64),W(1),P(0x18ca94),W(1),P(0x18cac4),W(1),P(0x18caf0),
    W(1),{NEO_ANIM_BRANCH_FIELD,0x78,30,0,0,0,0},P(0x18ce02),W(1),{NEO_ANIM_BRANCH_FIELD,0x78,30,0,0,0,0},P(0x18ce32),W(1),{NEO_ANIM_BRANCH_FIELD,0x78,30,0,0,0,0},
    P(0x18ce62),W(1),{NEO_ANIM_BRANCH_FIELD,0x78,30,0,0,0,0},P(0x18ce8e),W(1),{NEO_ANIM_JUMP,0,17,0,0,0,0},P(0x18cb1c),W(2),
    P(0x18cb4c),W(2),P(0x18cb7c),W(2),P(0x18cba8),W(2),P(0x18cbd4),W(2),
    P(0x18cc04),W(2),P(0x18cc34),W(2),P(0x18cc64),W(2),P(0x18cc90),W(2),
    P(0x18ccbc),W(2),P(0x18cce6),W(2),P(0x18cd10),W(2),P(0x18cd3c),W(2),
    P(0x18cd68),W(2),P(0x18cd92),W(2),P(0x18cdbc),W(2),P(0x18cde6),W(2),
    {NEO_ANIM_FINISH,0,0,0,0,0,0},
};
/* 0BF498: 24F072 */
static const neo_anim_command_t entrance_cmds[]={
    P(0x18aabc),W(1),P(0x18ab00),W(1),P(0x18ab40),W(1),P(0x18ab76),W(1),
    P(0x18aba0),W(1),P(0x18abc4),W(1),P(0x18abe4),W(1),{NEO_ANIM_WRITE_BYTE,0x1,0x76,0,0,0,0},P(0x188828),
    W(4),P(0x188838),W(1),{NEO_ANIM_FINISH,0,0,0,0,0,0},
};
#undef P
#undef PY
#undef W
#define PROGRAM(n) static const neo_anim_program_t n##_prog={n##_cmds,sizeof n##_cmds/sizeof n##_cmds[0]}
PROGRAM(ground);PROGRAM(kind2_ground);PROGRAM(kind7);
static const neo_anim_program_t air_prog={air_cmds,sizeof air_cmds/sizeof air_cmds[0]};
PROGRAM(entrance);

/* The pose relative to the actor's words, facing right (render x/y 0,
 * attributes 0x6340: palette 0x63 of the player, as marco_visual.c). */
static void emit(void *context,const neo_anim_draw_t *draw) {
    marco_death_t *d=context;d->descriptor=draw->descriptor;d->draw=*draw;d->drawn=true;d->shown=true;
}
static bool field(void *context,unsigned f,unsigned bytes,bool write,uint32_t *value) {
    marco_death_t *d=context;(void)bytes;
    if (f==0x78) {if (write) d->f78=(uint8_t)*value; else *value=d->f78;return true;}
    if (f==0x76) return write;   /* entrance: +76 (upper body may act), not used here */
    return false;
}
static neo_anim_result_t step(marco_death_t *d) {
    neo_anim_render_t r;memset(&r,0,sizeof r);r.attributes=0x6340;
    return neo_anim_step_fields(&d->anim,&r,emit,d,256,NULL,NULL,field,d);
}
bool marco_death_start(marco_death_t *d,uint8_t kind,bool grounded,bool attacker_right,int32_t *vx,int32_t *vy,int *facing) {
    memset(d,0,sizeof *d);
    d->f78=0xff;d->phase=MARCO_DYING;
    /* +5B bit 7: facing right (0C2E66/0C2C2A), else left. */
    *facing=attacker_right?1:-1;
    if (kind==2) {
        /* 0C2BCE: on the ground +30/+34 = 0 and 0BF3D8, in the air the
         * velocities stay and 0BF318's program (0BF458); +80 = 0x100. */
        if (grounded) {*vx=0;*vy=0;neo_anim_start(&d->anim,&kind2_ground_prog,0x100);}
        else neo_anim_start(&d->anim,&air_prog,0x100);
        d->speed_target=0x100;
        return true;
    }
    bool translated=true;
    if (kind==7 || kind==0x10) {
        /* 0C2C7A/0C2C40 (their sounds differ) -> 0C2CB0: 0BF358, +80 = 0x80. */
        neo_anim_start(&d->anim,&kind7_prog,0x100);d->speed_target=0x80;d->burnt=true;
    } else {
        /* 0C2DDA (kinds 8 and 0x12 on the ground have their own deaths,
         * 0C2ACC/0C2A70: not translated, explicit stop, this one instead):
         * 0BF298 on the ground, 0BF318 in the air; +80 = 0x20. */
        translated=!(kind==8 || (kind==0x12 && grounded));
        neo_anim_start(&d->anim,grounded?&ground_prog:&air_prog,0x100);d->speed_target=0x20;
    }
    /* 0C2E44..0C2E7E: +34 = 0xA9000, +30 = -2 facing right else +2, airborne. */
    *vy=0xa9000;*vx=attacker_right?-0x20000:0x20000;
    return translated;
}
bool marco_death_draw(const marco_death_t *d,uint16_t x,uint16_t y,int facing,neo_anim_draw_t *out) {
    /* Lying, the program steps (and draws) every other update: blinking. */
    if (!d->drawn || (d->phase==MARCO_LYING && !d->shown)) return false;
    *out=d->draw;
    out->x=(uint16_t)(x+(facing<0?-(int16_t)d->draw.x:(int16_t)d->draw.x));
    out->y=(uint16_t)(y+d->draw.y);
    if (facing<0) out->attributes^=1u;
    return true;
}
void marco_death_velocity(marco_death_t *d,bool ground,int32_t *vx,int32_t *vy) {
    if (d->phase==MARCO_DYING) {
        if (ground) return;                            /* 0C2EB8: ground step, +62 = 0 */
        *vy+=((int32_t)0xffea0000-*vy)>>5;              /* 0C2E94 */
        *vx-=*vx>>5;                                    /* 0C2EA4 */
    } else if (d->phase==MARCO_LYING) {
        *vx=0;*vy+=(int32_t)0xffffb000;                 /* 0C2F20/0C2F28 */
    }
}
static bool offscreen(int16_t sx,int16_t py) {          /* 0C2F72..0C2F8A */
    return (uint16_t)sx>=0x140u || (uint16_t)(py-0xc0)>=0x200u;
}
marco_life_phase_t marco_death_after(marco_death_t *d,bool ground,int32_t *vx,int16_t screen_x,int16_t projected_y) {
    d->updates++;d->shown=false;
    if (d->phase==MARCO_DYING) {
        /* 0C2EC6: +26 moves 1/16 of the way to +80; 0C2ED4: the program. */
        int16_t sp=(int16_t)d->anim.speed;sp=(int16_t)(sp+((int16_t)(d->speed_target-sp)>>4));d->anim.speed=(uint16_t)sp;
        bool done=step(d)!=NEO_ANIM_MORE;
        if (ground) {*vx=0;d->speed_target=0x100;d->f78=0;}   /* 0C2EE4..0C2EFC */
        if (offscreen(screen_x,projected_y)) {d->phase=MARCO_GONE;d->updates=0;return d->phase;}
        if (done) {d->phase=MARCO_LYING;d->count80=0x1e;d->updates=0;}   /* 0C2EDE: 0C2F06 next update */
        return d->phase;
    }
    if (d->phase==MARCO_LYING) {
        /* 0C2F12: the count-down; at the borrow the actor ends (0C2F90). */
        if (!d->count80--) {d->phase=MARCO_GONE;d->updates=0;return d->phase;}
        if (d->count80&1) step(d);
        if (offscreen(screen_x,projected_y)) {d->phase=MARCO_GONE;d->updates=0;}
        return d->phase;
    }
    return d->phase;
}
/* Mission 1 respawn table (bank 1 2D5C22, 12-byte entries: threshold of the
 * camera progress, then the record of 0196E8: x, y, x relative to the
 * root, y relative, 0, +7 x step, +8 type). The mission's first record
 * 2D5C18 (type 2, the descent of the start) is only the first spawn's. */
static const struct { uint16_t from; int16_t x,y; uint8_t relx,rely; int8_t step; uint8_t type; } table[]={
    {0x0000,0x68,0x70,1,0,0x10,0},{0x0360,0x68,0x88,1,0,0x10,0},{0x0780,0xa0,0x70,1,0,0x10,0},
    {0x0788,0x68,0x70,1,0,0x10,0},{0x0d30,0x28,0x50,1,0,0x10,0}};
marco_respawn_t marco_respawn_point(uint16_t progress,int32_t root_x,int32_t root_y) {
    /* 019658: the entry follows while the progress reaches the next one's. */
    unsigned i=0;while (i+1<sizeof table/sizeof table[0] && progress>=table[i+1].from) i++;
    int16_t x=table[i].x,y=table[i].y;
    if (table[i].relx) x=(int16_t)(x-(int16_t)(root_x>>16));          /* 01970A: the root's word */
    if (table[i].rely) y=(int16_t)(y+0x100-(int16_t)(root_y>>16));
    marco_respawn_t r={(int32_t)((uint32_t)(uint16_t)x<<16),(int32_t)((uint32_t)(uint16_t)y<<16),table[i].type};
    r.x=(int32_t)((uint32_t)(int16_t)x<<16)+((int32_t)table[i].step<<16);   /* 019744: one 0143E4 step of +30 */
    r.y=(int32_t)((uint32_t)(int16_t)y<<16);
    return r;
}
void marco_entrance_start(marco_death_t *d) {
    memset(d,0,sizeof *d);neo_anim_start(&d->anim,&entrance_prog,0x100);d->phase=MARCO_ENTERING;
}
bool marco_entrance_step(marco_death_t *d) {
    if (d->phase!=MARCO_ENTERING) return false;
    d->updates++;d->shown=false;
    if (step(d)!=NEO_ANIM_MORE) {d->phase=MARCO_ALIVE;return false;}   /* 0C14B6: then 0C229C */
    return true;
}
