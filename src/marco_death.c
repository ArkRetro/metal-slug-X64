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
/* 0BF398 (kind 8, 0C2ACC): 25032A. POSE flags 0x10 / 0x20: the palettes of
 * +19 / +1A (render palettes[1] / [2]); 2503B6 the callback 05A09E (the
 * child 05A0B4, an event here); 2503CE jumps to 25016A (air_cmds), copied. */
#define P1(d) {NEO_ANIM_POSE,(d),0,0,16,0,0}
#define P2(d) {NEO_ANIM_POSE,(d),0,0,32,0,0}
static const neo_anim_command_t kind8_cmds[]={
    P1(0x18cfa4),W(1),P2(0x18cfe0),W(1),P1(0x18cfa4),W(1),P2(0x18cfe0),W(1),
    P1(0x18cfa4),W(1),P2(0x18cfe0),W(1),P1(0x18cf86),W(1),P1(0x18cfa4),W(1),
    P1(0x18cf86),W(1),P2(0x18cfc2),W(1),P2(0x18cfe0),W(1),P2(0x18cfc2),W(1),
    P1(0x18cf86),W(1),P1(0x18cfc2),W(1),{NEO_ANIM_EVENT,0x5a09e,0,0,0,0,0},P1(0x18cfc2),W(2),
    {NEO_ANIM_BRANCH_FIELD,0x78,29,1,0,0,0},
    {NEO_ANIM_WRITE_BYTE,0xff,0x78,0,0,0,0},P(0x18bd6a),W(1),P(0x18bd82),W(2),P(0x18bd9c),W(3),P(0x18bdb6),
    W(3),P(0x18bdca),W(3),P(0x18bdf2),W(1),{NEO_ANIM_BRANCH_FIELD,0x78,43,1,0,0,0},P(0x18be06),W(2),
    P(0x18be1a),W(2),P(0x18be30),W(2),P(0x18be1a),W(2),{NEO_ANIM_FINISH,0,0,0,0,0,0},
};
#undef P1
#undef P2
/* 0BF418 (kind 0x12 on the ground, 0C2A70): 24FF1E. */
static const neo_anim_command_t kind12_cmds[]={
    P(0x18d298),W(1),P(0x18d2b4),W(1),P(0x18d2d0),W(2),P(0x18d2ec),W(2),
    P(0x18d308),W(2),P(0x18d326),W(2),P(0x18d342),W(2),P(0x18d35e),W(2),
    P(0x18d372),W(3),P(0x18d386),W(3),P(0x18d39a),W(2),P(0x18d3b2),W(1),
    P(0x18d3ca),W(1),P(0x18d3e2),W(1),P(0x18d3fa),W(1),P(0x18d412),W(2),
    P(0x18d42a),W(2),P(0x18d438),W(2),P(0x18d450),W(1),{NEO_ANIM_FINISH,0,0,0,0,0,0},
};
/* 0BF2D8 (the fall into water, 0C2CCC): 24ECCC, one pose and FINISH. */
static const neo_anim_command_t water_cmds[]={P(0x18d7b0),{NEO_ANIM_FINISH,0,0,0,0,0,0}};
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
PROGRAM(ground);PROGRAM(kind2_ground);PROGRAM(kind7);PROGRAM(kind8);PROGRAM(kind12);PROGRAM(water);
static const neo_anim_program_t air_prog={air_cmds,sizeof air_cmds/sizeof air_cmds[0]};
PROGRAM(entrance);
/* The chosen character's five programs (marco_death_use_programs), in the
 * order ground 0BF298, kind 2 on the ground 0BF3D8, kinds 7/0x10 0BF358, in
 * the air 0BF318, entrance 0BF498; NULL: Marco's above. */
static const neo_anim_program_t *character_programs;
void marco_death_use_programs(const neo_anim_program_t *five) {character_programs=five;}
/* +6E of the chosen character: kinds 8 and 0x12 have Marco's programs only
 * (25032A / 24FF1E, column 0 of 0BF398 / 0BF418). */
static unsigned character_column;
void marco_death_use_character(unsigned column) {character_column=column;}
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
/* 2503B6: the callback 05A09E (0040BC: the child 05A0B4, made by the caller). */
static bool event(void *context,neo_anim_kind_t kind,uint32_t asset,neo_anim_render_t *r) {
    marco_death_t *d=context;(void)r;
    if (kind!=NEO_ANIM_EVENT || asset!=0x5a09eu) return false;
    d->child_5a0b4=true;return true;
}
static neo_anim_result_t step(marco_death_t *d) {
    /* palettes[1..3]: the slots +19..+1B (0043D4: POSE flags >> 4), named
     * by the player's attribute 0x63 and the two next ones. */
    neo_anim_render_t r;memset(&r,0,sizeof r);r.attributes=0x6340;r.palettes[1]=0x63;r.palettes[2]=0x64;r.palettes[3]=0x65;
    return neo_anim_step_fields(&d->anim,&r,emit,d,256,event,d,field,d);
}
bool marco_death_start(marco_death_t *d,uint8_t kind,bool grounded,bool attacker_right,int32_t *vx,int32_t *vy,int *facing) {
    memset(d,0,sizeof *d);
    d->f78=0xff;d->phase=MARCO_DYING;
    const int before=*facing;
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
    if (kind==8 && !character_column) {
        /* 0C2ACC: 009F88; +19 = 0BEDD0 (0BEDD8 with 10C30C), +1A = 0BEDE0,
         * +1B = 0x51 (009EA8); 0BF398; +5B bit 7 clear: +15 bit 0 set (left),
         * +30 = 2; set: 0C2B68 clears bit 1, not bit 0 (the facing stays),
         * +30 = -2; +34 = 0xA9000, +78 = FF. +15 bit 6 and +80/+26 untouched. */
        *facing=attacker_right?(before<0?-1:1):-1;
        *vx=attacker_right?-0x20000:0x20000;*vy=0xa9000;
        neo_anim_start(&d->anim,&kind8_prog,0x100);d->own=8;d->keep_ground=true;d->palette_slot1a=0xbede0u;
        return true;
    }
    if (kind==0x12 && grounded && !character_column) {
        /* 0C2A70: 040F38 (the splash 040F74, made by the caller), then on
         * the ground (+15 bit 6) the facing by +5B, 0BF418, +62 = 0; no
         * palette request and no velocity write. */
        neo_anim_start(&d->anim,&kind12_prog,0x100);d->own=0x12;d->keep_ground=true;
        return true;
    }
    bool translated=true;
    if (kind==7 || kind==0x10) {
        /* 0C2C7A (+19/+1A = 0BEDE8, the burnt palette) / 0C2C40 (+19 =
         * 0BEE08, +1A = 0BEDE8) -> 0C2CB0: 0BF358, +80 = 0x80. Kind 0x10
         * only reaches 0C2956 in form 3 (0C36CE), which takes 0C2C40. */
        neo_anim_start(&d->anim,&kind7_prog,0x100);d->speed_target=0x80;d->burnt=true;d->palette_table=kind==7?0xbede8u:0xbee08u;
    } else {
        /* 0C2DDA (kinds 8 and 0x12 on the ground of Tarma, Eri and Fio have
         * their own deaths, 0C2ACC/0C2A70, programs not in their packs:
         * explicit stop, this one instead):
         * 0BF298 on the ground, 0BF318 in the air; +80 = 0x20. */
        translated=!character_column || !(kind==8 || (kind==0x12 && grounded));
        neo_anim_start(&d->anim,grounded?&ground_prog:&air_prog,0x100);d->speed_target=0x20;
    }
    d->c4c28=true;                                     /* 0C2E74 */
    /* 0C2E44..0C2E7E: +34 = 0xA9000, +30 = -2 facing right else +2, airborne. */
    *vy=0xa9000;*vx=attacker_right?-0x20000:0x20000;
    return translated;
}
bool marco_death_water_stage(uint16_t stage) {return stage==9u || stage==0xbu || stage==0u;}   /* 0191FC: 2, 2, 3 */
bool marco_death_start_water(marco_death_t *d,int32_t *vy) {
    memset(d,0,sizeof *d);
    d->f78=0xff;d->phase=MARCO_DYING;d->water=true;d->water80=0x1e;
    d->palette_table=0xbedf0u;                          /* 0C2CD2..0C2CE4 */
    neo_anim_start(&d->anim,&water_prog,0x100);         /* 0C0F32 (0C0F56: speed 0x100) */
    *vy=0;                                              /* 0C2CF6 */
    return !character_column;
}
void marco_death_water_velocity(marco_death_t *d,int32_t *vx,int32_t *vy,int32_t y40,marco_surface_fn surface,void *ctx) {
    *vx-=*vx>>4;                                        /* 0C2D06..0C2D0C */
    int32_t y;
    if (d->water80) {
        d->water80--;                                   /* 0C2D18 */
        if (surface && surface(ctx,&y)) {               /* 0C2D1E: 03C55C */
            *vy=(int32_t)((uint32_t)y-0x80000u-(uint32_t)y40)>>3;   /* 0C2D28..0C2D38 */
            return;
        }
    }
    *vy+=(int32_t)((uint32_t)0xfff00000u-(uint32_t)*vy)>>5;          /* 0C2D40..0C2D4C */
}
static const neo_anim_program_t *fat_script;static uint16_t fat_cursor;
void marco_death_use_fat_program(const neo_anim_program_t *script,uint16_t entry) {fat_script=script;fat_cursor=entry;}
bool marco_death_start_form(marco_death_t *d,uint8_t kind,uint8_t form93,bool fall,bool grounded,bool attacker_right,
    int32_t *vx,int32_t *vy,int *facing) {
    if (form93==3) {
        /* 0C2986/0C2DD0: 0C2C40 whatever the kind (MAME audit mzd1: a hit
         * of kind 0 on the mummy runs 2501DC with +19 = 0x132, +1A = 0x56). */
        marco_death_start(d,0x10,grounded,attacker_right,vx,vy,facing);
        return true;
    }
    if (form93!=2) return marco_death_start(d,kind,grounded,attacker_right,vx,vy,facing);
    memset(d,0,sizeof *d);
    d->phase=MARCO_DYING;d->fat=true;d->f78=0xff;
    if (fall) {
        /* 0C2A06: +34 = 0xA9000; +5B bit 7 clear: facing right, +30 = 2. */
        *vy=0xa9000;*vx=attacker_right?-0x20000:0x20000;*facing=attacker_right?-1:1;
    }
    /* 0C2A36: the body of 0C0518 (0C0FB6, +6E's column), 0C0F16; the
     * velocities and the facing stay. */
    if (!fat_script || fat_cursor>=fat_script->count) {
        neo_anim_start(&d->anim,grounded?&ground_prog:&air_prog,0x100);d->fat=false;d->speed_target=0x20;
        return false;
    }
    neo_anim_start(&d->anim,fat_script,0x100);d->anim.cursor=fat_cursor;
    return true;
}
bool marco_death_draw(const marco_death_t *d,uint16_t x,uint16_t y,int facing,neo_anim_draw_t *out) {
    /* Lying, the program steps (and draws) every other update: blinking. */
    if (!d->drawn || (d->phase==MARCO_LYING && !d->shown) || (d->phase==MARCO_GONE && !d->final_draw)) return false;
    *out=d->draw;
    out->x=(uint16_t)(x+(facing<0?-(int16_t)d->draw.x:(int16_t)d->draw.x));
    out->y=(uint16_t)(y+d->draw.y);
    if (facing<0) out->attributes^=1u;
    return true;
}
void marco_death_cap_start(int32_t creator_vx,int32_t creator_vy,int32_t *vx,int32_t *vy) {
    *vx=(int32_t)((uint32_t)creator_vx+(creator_vx>=0?0x10000u:0xffff0000u));   /* 0C4C94..0C4CAA */
    *vy=creator_vy;                                                             /* 0C4CAE */
}
void marco_death_cap_velocity(int32_t *vx,int32_t *vy,uint16_t *speed,bool ground) {
    *vx-=*vx>>4;                                                                /* 0C4CBA */
    int32_t d0=(int32_t)((uint32_t)*vx+(uint32_t)*vx);if (d0<0) d0=(int32_t)(0u-(uint32_t)d0);
    d0=(int32_t)((uint32_t)d0-0x60000u-(uint32_t)*vy);*vy+=d0>>4;              /* 0C4CC4..0C4CDC */
    uint16_t target=0x80;
    if (ground) {*vy=0;target=0x400;}                                           /* 0C4CE0..0C4CFA */
    *speed=(uint16_t)(*speed+(uint16_t)((int16_t)(target-*speed)>>3));         /* 0C4CFE..0C4D04 */
}
bool marco_death_cap_after(bool program_done,uint16_t *count80,uint8_t *f5f) {
    if (!program_done) return false;                                            /* 0C4D14 */
    ++*count80;
    if (*count80>=0xf) *f5f|=8u;                                                /* 0C4D22: +5F bit 3 */
    return *count80>=0x1e;                                                      /* 0C4D30: 004188 */
}
void marco_death_velocity(marco_death_t *d,bool ground,int32_t *vx,int32_t *vy) {
    if (d->phase==MARCO_DYING && d->fat) {
        *vx-=*vx>>5;*vy-=*vy>>5;                        /* 0C2A48..0C2A58, then 0143EE */
        return;
    }
    if (d->phase==MARCO_DYING && d->own==8) {
        *vy+=((int32_t)0xffea0000-*vy)>>6;              /* 0C2B8A..0C2BA0, then 0143EE */
        *vx-=*vx>>6;(void)ground;
        return;
    }
    if (d->phase==MARCO_DYING && d->own==0x12) return; /* 0C2AB2: 014594 with +62 = 0 */
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
    if (d->phase==MARCO_DYING && d->fat) {
        /* 0C2A62: the program at +26 (unchanged); at its end 0C2F90, the
         * actor's end (no lying count-down); 0C2F72 off the screen too. */
        bool done=step(d)!=NEO_ANIM_MORE;(void)ground;(void)vx;
        if (done || offscreen(screen_x,projected_y)) {d->phase=MARCO_GONE;d->updates=0;d->final_draw=done;}   /* fatd1: 128606 shows at the end update */
        return d->phase;
    }
    if (d->phase==MARCO_DYING && d->water) {
        /* 0C2D50: the program (its end ignored); 0C2D5C: on the ground
         * (+15 bit 6) 0C2F90, the actor's end; else 0C2F4A..0C2F72: off the
         * screen, the end too. No +26 ramp, no lying. */
        step(d);(void)vx;
        if (ground || offscreen(screen_x,projected_y)) {d->phase=MARCO_GONE;d->updates=0;d->final_draw=d->shown;}   /* w6d2: its pose shows */
        return d->phase;
    }
    if (d->phase==MARCO_DYING && d->own) {
        /* 0C2BAA / 0C2AB8: the program (no +26 ramp), its end 0C2F06; kind 8:
         * +78 = 0 on the ground (0C2BBA); 0C2F72 off the screen. */
        bool done=step(d)!=NEO_ANIM_MORE;(void)vx;
        if (d->own==8 && ground) d->f78=0;
        if (offscreen(screen_x,projected_y)) {d->phase=MARCO_GONE;d->updates=0;return d->phase;}
        if (done) {d->phase=MARCO_LYING;d->count80=0x1e;d->updates=0;}
        return d->phase;
    }
    if (d->phase==MARCO_DYING) {
        /* 0C2EC6: +26 moves 1/16 of the way to +80; 0C2ED4: the program. */
        int16_t sp=(int16_t)d->anim.speed;sp=(int16_t)(sp+((int16_t)(d->speed_target-sp)>>4));d->anim.speed=(uint16_t)sp;
        bool done=step(d)!=NEO_ANIM_MORE;
        if (ground) {*vx=0;d->speed_target=0x100;d->f78=0;}   /* 0C2EE4..0C2EFC */
        /* 0C2F72 after the program: the pose of this update shows (w6d2 293315). */
        if (offscreen(screen_x,projected_y)) {d->phase=MARCO_GONE;d->updates=0;d->final_draw=d->shown;return d->phase;}
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
