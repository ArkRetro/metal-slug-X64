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
/* The chosen character's five programs (marco_death_use_programs), in the
 * order ground 0BF298, kind 2 on the ground 0BF3D8, kinds 7/0x10 0BF358, in
 * the air 0BF318, entrance 0BF498; NULL: Marco's above. */
static const neo_anim_program_t *character_programs;
void marco_death_use_programs(const neo_anim_program_t *five) {character_programs=five;}
#define ground_prog (*(character_programs?&character_programs[0]:&ground_prog))
#define kind2_ground_prog (*(character_programs?&character_programs[1]:&kind2_ground_prog))
#define kind7_prog (*(character_programs?&character_programs[2]:&kind7_prog))
#define air_prog (*(character_programs?&character_programs[3]:&air_prog))
#define entrance_prog (*(character_programs?&character_programs[4]:&entrance_prog))

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
/* Mission 1 respawn table: the 12-byte entries of bank 1 2D5C22 as the
 * ROM holds them (threshold of the camera progress, then the record of
 * 0196E8: x, y, x relative to the root, y relative, +6, +7 x step, +8 type,
 * +9), to the 0xFFFF threshold. The mission's first record 2D5C18 (type 2,
 * the descent of the start) is only the first spawn's. Missions 2/3 read
 * their stage's entries from the mission pack (descriptor +4, 019604). */
static const uint8_t mission1_records[]={
    0x00,0x00,0x00,0x68,0x00,0x70,0x01,0x00,0x00,0x10,0x00,0x00,
    0x03,0x60,0x00,0x68,0x00,0x88,0x01,0x00,0x00,0x10,0x00,0x00,
    0x07,0x80,0x00,0xa0,0x00,0x70,0x01,0x00,0x00,0x10,0x00,0x00,
    0x07,0x88,0x00,0x68,0x00,0x70,0x01,0x00,0x00,0x10,0x00,0x00,
    0x0d,0x30,0x00,0x28,0x00,0x50,0x01,0x00,0x00,0x10,0x00,0x00,
    0xff,0xff};
static uint16_t rw(const uint8_t *p) {return (uint16_t)(p[0]<<8|p[1]);}
unsigned marco_respawn_entries(const uint8_t *table,unsigned length) {
    /* pack_mission.py stage_blobs: the entries up to the 0xFFFF threshold. */
    unsigned n=0;
    while ((n+1u)*12u+2u<=length && rw(table+12u*(n+1u))!=0xffffu) n++;
    return table && length>=14u?n+1u:0u;
}
unsigned marco_respawn_select(const uint8_t *table,unsigned entries,unsigned from,uint16_t progress) {
    /* 019658: the pointer 10C31E moves on while the progress reaches the
     * next entry's threshold (bcs: unsigned); it never moves back. */
    unsigned i=from;
    while (i+1u<entries && progress>=rw(table+12u*(i+1u))) i++;
    return i;
}
marco_respawn_t marco_respawn_entry(const uint8_t *table,unsigned index,int32_t root_x,int32_t root_y) {
    const uint8_t *r=table+12u*index+2u;   /* 10C31A: the entry's record */
    /* 0196E8: x/y words, minus the root's words (10E968/10E96C) when +4/+5
     * are set (y: +0x100 first), placed by 01AB9C at the root. */
    int16_t x=(int16_t)rw(r),y=(int16_t)rw(r+2);
    if (r[4]) x=(int16_t)(x-(int16_t)(root_x>>16));
    if (r[5]) y=(int16_t)(y+0x100-(int16_t)(root_y>>16));
    marco_respawn_t o;
    o.x=(int32_t)((uint32_t)(int16_t)x<<16)+((int32_t)(int8_t)r[7]<<16);   /* 019744: one 0143E4 step of +30 = +7 (player 1) */
    o.y=(int32_t)((uint32_t)(int16_t)y<<16);
    o.type=r[8];o.step=(int8_t)r[7];
    return o;
}
marco_respawn_t marco_respawn_point(uint16_t progress,int32_t root_x,int32_t root_y) {
    unsigned n=marco_respawn_entries(mission1_records,sizeof mission1_records);
    return marco_respawn_entry(mission1_records,marco_respawn_select(mission1_records,n,0,progress),root_x,root_y);
}
/* 014742: from the record's x (+28, Q16 relative to the root; projected
 * +3C = +28 + root_x), 5-pixel steps left while the projected x is at least
 * 0x1C, then from the record's x right while it is below 0x124, each a
 * floor search 013DB2 (A0 = 0) at the projected (x, py) down to `limit`.
 * A floor closer than the best so far (D6 > limit, signed) counts down a
 * run of 4 (reset to 4 where no floor is found); the fifth makes it the
 * best (limit = D6) and the result x +10 (left scan) or -10 (right scan).
 * Only the word of +28 moves. True with *x at the result; false: *x back
 * at the record's x. */
bool marco_respawn_search(int32_t *x,int32_t root_x,int32_t py,int32_t limit,marco_floor_fn floor,void *ctx) {
    uint16_t orig=(uint16_t)((uint32_t)*x>>16),result=0x8000u,count=4;
    uint32_t low=(uint32_t)*x&0xffffu;
    for (int side=0;side<2;side++) {
        uint16_t w=orig;count=4;
        for (unsigned guard=0;guard<0x10000u;guard++) {
            int32_t px=(int32_t)((((uint32_t)w<<16)|low)+(uint32_t)root_x);   /* 01ABC8 */
            int16_t sx=(int16_t)((uint32_t)px>>16);
            /* 014772 cmpi.w #$1C/bmi, 0147D6 cmpi.w #$124/bpl: the N flag. */
            if (side==0?(int16_t)(sx-0x1c)<0:(int16_t)(sx-0x124)>=0) break;
            int32_t d6=0;
            if (floor(ctx,px,py,limit,&d6)) {
                if (d6>limit) {
                    if (!count--) {limit=d6;result=(uint16_t)(side==0?w+10u:w-10u);}
                }
            } else count=4;
            w=(uint16_t)(side==0?w-5u:w+5u);
        }
    }
    if (result==0x8000u) {*x=(int32_t)(((uint32_t)orig<<16)|low);return false;}
    *x=(int32_t)(((uint32_t)result<<16)|low);return true;
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
