#include "marco_visual.h"
#include "marco_death.h"
#include <string.h>
/* Catalogs generated before the 24-bit command asset (src/neo_animation.h)
 * write SET/WRITE_LONG constants as 0xFFFExxxx: the field keeps the same
 * value (sign-extended 24-bit, checked by tools/convert_marco_programs.py). */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Woverflow"
#include "marco_campaign.h"
#pragma GCC diagnostic pop
/* Program choice per situation. Every role is a program the original chose
 * in the named capture condition (tools/compile_marco_campaign.py). */
static void emit(void *context,const neo_anim_draw_t *d) {
    marco_visual_t *v=context;
    neo_anim_draw_t *list=v->collect?v->weapon_draws:v->body_draws;
    unsigned *n=v->collect?&v->weapon_count:&v->body_count;
    /* Kept relative to the player's words: a finished part's last pose
     * follows the player. */
    if (*n<MARCO_VISUAL_DRAWS) {neo_anim_draw_t r=*d;r.x=(uint16_t)(r.x-v->base_x);r.y=(uint16_t)(r.y-v->base_y);list[(*n)++]=r;}
}
/* Measured event sites (effect requests such as the pistol's flash): their
 * world effects are not part of this layer; the program continues. */
static bool event(void *context,neo_anim_kind_t kind,uint32_t id,neo_anim_render_t *r) {
    (void)context;(void)id;(void)r;return kind==NEO_ANIM_EVENT;
}
/* Form 2, fat Marco (marco-f2, run_marco_catalog.py --give-fat): the same
 * controller states choose the same tables with rows +0x20 (0C0F32/0C0F62
 * offset 32). The visual keeps the form-0 role of the situation and starts
 * the program of its +0x20 row: body 0BF6D8 259D4C idle, 0BF718 259D68 run,
 * 0BF758/0BF798 259E64 jumps, 0BEE18 25D1C2 / 0BEE58 25CFFE turns, 0BEED8
 * 25D088 / 0BEF18 and 0BEF98 25D0D0 / 0BF058 25D10E / 0BF098 25D040 the
 * crouch; weapon 0BF858 259EE2 idle (also the jumps 0BFC58/0BFC98), 0BF8D8
 * 259E7C run, 0BF998 25A5EA grenade, 0BF9D8 25A724 up, 0BFD18/0BFD58/0BFD98
 * 25A8F4/25A83C/25A898 fire. The raised gun held/lowered (0BFA18 25A7FE /
 * 0BFA98 25A73A) are form-2 roles of their own. A row not captured
 * (victory, knife, hanging, H/C fire) has no program: explicit stop, the
 * part keeps its last pose (counted in `stops`). */
static int fat_role(int role) {
    static const int map[][2]={
        {MARCO_CAMPAIGN_B_IDLE,MARCO_CAMPAIGN_B2_IDLE},{MARCO_CAMPAIGN_B_RUN,MARCO_CAMPAIGN_B2_RUN},
        {MARCO_CAMPAIGN_B_JUMP,MARCO_CAMPAIGN_B2_JUMP},{MARCO_CAMPAIGN_B_RUN_JUMP,MARCO_CAMPAIGN_B2_JUMP},
        {MARCO_CAMPAIGN_B_TURN,MARCO_CAMPAIGN_B2_TURN},{MARCO_CAMPAIGN_B_TURN_RUN,MARCO_CAMPAIGN_B2_TURN_RUN},
        {MARCO_CAMPAIGN_B_CROUCH_IN,MARCO_CAMPAIGN_B2_CROUCH_IN},{MARCO_CAMPAIGN_B_CROUCH,MARCO_CAMPAIGN_B2_CROUCH},
        {MARCO_CAMPAIGN_B_CROUCH_AFTER_GRENADE,MARCO_CAMPAIGN_B2_CROUCH},{MARCO_CAMPAIGN_B_CROUCH_FIRE,MARCO_CAMPAIGN_B2_CROUCH_FIRE},
        {MARCO_CAMPAIGN_B_CROUCH_GRENADE,MARCO_CAMPAIGN_B2_CROUCH_GRENADE},
        {MARCO_CAMPAIGN_W_IDLE,MARCO_CAMPAIGN_W2_IDLE},{MARCO_CAMPAIGN_W_JUMP,MARCO_CAMPAIGN_W2_IDLE},
        {MARCO_CAMPAIGN_W_RUN_JUMP,MARCO_CAMPAIGN_W2_IDLE},{MARCO_CAMPAIGN_W_RUN,MARCO_CAMPAIGN_W2_RUN},
        {MARCO_CAMPAIGN_W_GRENADE,MARCO_CAMPAIGN_W2_GRENADE},{MARCO_CAMPAIGN_W_AIM_UP,MARCO_CAMPAIGN_W2_AIM_UP},
        {MARCO_CAMPAIGN_W_FIRE,MARCO_CAMPAIGN_W2_FIRE},{MARCO_CAMPAIGN_W_FIRE_UP,MARCO_CAMPAIGN_W2_FIRE_UP},
        {MARCO_CAMPAIGN_W_FIRE_DOWN,MARCO_CAMPAIGN_W2_FIRE_DOWN},
        {MARCO_CAMPAIGN_W2_AIM_HOLD,MARCO_CAMPAIGN_W2_AIM_HOLD},{MARCO_CAMPAIGN_W2_AIM_DOWN,MARCO_CAMPAIGN_W2_AIM_DOWN},
        {MARCO_CAMPAIGN_B2_TRANSFORM,MARCO_CAMPAIGN_B2_TRANSFORM}};
    for (unsigned i=0;i<sizeof map/sizeof map[0];i++) if (map[i][0]==role) return map[i][1];
    return -1;
}
/* The programs of the chosen character (marco_visual_use_programs); NULL:
 * Marco's, the built-in catalog. */
static const neo_anim_program_t *character_script;
static const uint16_t (*character_roles)[2];
static unsigned character_role_count;
/* B84: the weapon programs chosen with +92 = 1 (0C4100 raise, 0BFA18 /
 * 0BFA58 hold, 0C3D38 / 0C3EC4.. the fire up); the others leave +92 <= 0
 * for the turn's choice (0C2768). */
static bool up_role(int r) {
    return r>=0 && (r==MARCO_CAMPAIGN_W_AIM_UP || r==MARCO_CAMPAIGN_W_AIM_HOLD || r==MARCO_CAMPAIGN_W_FIRE_UP ||
        r==MARCO_CAMPAIGN_W1_AIM_UP || r==MARCO_CAMPAIGN_W1_AIM_HOLD || r==MARCO_CAMPAIGN_W1_BURST_UP || r==MARCO_CAMPAIGN_W1_SWEEP_FWD_UP ||
        r==MARCO_CAMPAIGN_W1C_FIRE_UP || r==MARCO_CAMPAIGN_W1S_FIRE_UP || r==MARCO_CAMPAIGN_W1F_FIRE_UP || r==MARCO_CAMPAIGN_W1L_FIRE_UP);
}
static void start(marco_visual_t *v,marco_part_t part,int role) {
    if (part==MARCO_BODY) {v->body_role=role;v->body_done=false;}
    else {v->weapon_role=role;v->weapon_done=false;v->m92=up_role(role)?1:0;}
    if (role<0) return;
    int program=role;
    if (v->form==2 && (program=fat_role(role))<0) {
        v->stops++;
        if (part==MARCO_BODY) v->body_done=true; else v->weapon_done=true;
        return;
    }
    /* Body speed +26: the run starts at 0x1C1 (C285C, marco_motion.c; cat1:
     * every run update of 24DD18 has 01C1), every other program at 0x100. */
    /* The mummy's walk 0C196A: +26 = 0x80 (marco-mz2, every update of 25F3CC). */
    uint16_t speed=part==MARCO_BODY && role==MARCO_CAMPAIGN_B_RUN?0x1c1:part==MARCO_BODY && role==MARCO_CAMPAIGN_B3_WALK?0x80:0x100;
    /* Another character (msx_character.h): the same role, the program of
     * its column of the same table rows. A role without one is a stop. */
    const neo_anim_program_t *script=&marco_script;
    if (character_script) {
        unsigned lo=0,hi=character_role_count;
        while (lo<hi) {unsigned mid=lo+(hi-lo)/2;if (character_roles[mid][0]<(unsigned)program) lo=mid+1;else hi=mid;}
        if (lo==character_role_count || character_roles[lo][0]!=(unsigned)program) {
            v->stops++;
            if (part==MARCO_BODY) v->body_done=true; else v->weapon_done=true;
            return;
        }
        program=character_roles[lo][1];script=character_script;
    }
    if (!marco_animation_start(&v->anim,part,script,(uint16_t)program,speed)) v->failures++;
    v->starts++;
}
/* Form 2's death (0C2A36): the body of table 0C0518, role B2_HIT_C2A06
 * (marco-f2: 25CF70 at 4423), in the chosen character's column. */
static void publish_fat_death(void) {
    unsigned program=MARCO_CAMPAIGN_B2_HIT_C2A06;
    if (!character_script) {marco_death_use_fat_program(&marco_script,(uint16_t)program);return;}
    for (unsigned i=0;i<character_role_count;i++) if (character_roles[i][0]==program) {marco_death_use_fat_program(character_script,character_roles[i][1]);return;}
    marco_death_use_fat_program(NULL,0);
}
void marco_visual_use_programs(const neo_anim_program_t *script,const uint16_t (*roles)[2],unsigned count) {
    character_script=script;character_roles=script?roles:NULL;character_role_count=script?count:0;
    publish_fat_death();
}
void marco_visual_reset(marco_visual_t *v) {
    publish_fat_death();
    memset(v,0,sizeof(*v));marco_animation_reset(&v->anim);
    v->body_role=v->weapon_role=-1;v->grounded=true;
}
static bool crouch_role(int r) {
    return r==MARCO_CAMPAIGN_B_CROUCH_IN || r==MARCO_CAMPAIGN_B_CROUCH || r==MARCO_CAMPAIGN_B_CROUCH_FIRE ||
        r==MARCO_CAMPAIGN_B_CROUCH_GRENADE || r==MARCO_CAMPAIGN_B_CROUCH_AFTER_GRENADE ||
        r==MARCO_CAMPAIGN_B1_CROUCH_IN || r==MARCO_CAMPAIGN_B1_CROUCH || r==MARCO_CAMPAIGN_B1_CROUCH_FIRE ||
        r==MARCO_CAMPAIGN_B1_CROUCH_GRENADE || r==MARCO_CAMPAIGN_B1_CROUCH_AFTER_GRENADE || r==MARCO_CAMPAIGN_B1C_CROUCH_FIRE ||
        r==MARCO_CAMPAIGN_B1S_CROUCH_FIRE || r==MARCO_CAMPAIGN_B1F_CROUCH_FIRE ||
        r==MARCO_CAMPAIGN_B_CROUCH_KNIFE || r==MARCO_CAMPAIGN_B1_CROUCH_KNIFE;
}
static bool weapon_default(int r) {
    return r==MARCO_CAMPAIGN_W_IDLE || r==MARCO_CAMPAIGN_W_RUN || r==MARCO_CAMPAIGN_W_JUMP ||
        r==MARCO_CAMPAIGN_W_RUN_JUMP || r==MARCO_CAMPAIGN_W_AIM_UP || r==MARCO_CAMPAIGN_W_AIM_HOLD ||
        r==MARCO_CAMPAIGN_W1_IDLE || r==MARCO_CAMPAIGN_W1_RUN || r==MARCO_CAMPAIGN_W1_JUMP ||
        r==MARCO_CAMPAIGN_W1_RUN_JUMP || r==MARCO_CAMPAIGN_W1_AIM_UP || r==MARCO_CAMPAIGN_W1_AIM_HOLD ||
        r==MARCO_CAMPAIGN_W1_AIM_DOWN || r==MARCO_CAMPAIGN_W2_AIM_HOLD || r==MARCO_CAMPAIGN_W2_AIM_DOWN;
}
/* B83. The weapon state 0C3A5E steps its program at 0C3B34 (0C3778,
 * 00427E); at its end (FINISH 004352: d7 = FF) it writes +91 = FF, and in
 * the next update 0C37CE, finding +91 = FF, chooses the program of the
 * body's situation +90 again: 1 run 0C387E (0BF8D8), 2 jump 0C3844
 * (0BFC58), 3 run jump 0C3830 (0BFC98), each with +92 = 0, the rows
 * weapon_rest gives. The run's weapon (24E2C8, form 1 255ABE, form 2
 * 259E7C) picks its pose by +77 and finishes at once, so the original
 * starts it every update (marco-cat1 3028..3067: 188E06..188DF6 with the
 * body's frame); kept finished, its first pose stayed with the facing of
 * the update that started it and showed reversed after a turn. The idle
 * and aim rows (0C397E with +92) are not all in the catalog: those keep
 * the rules below. */
static bool weapon_reselected(int r) {
    return r==MARCO_CAMPAIGN_W_RUN || r==MARCO_CAMPAIGN_W_JUMP || r==MARCO_CAMPAIGN_W_RUN_JUMP ||
        r==MARCO_CAMPAIGN_W1_RUN || r==MARCO_CAMPAIGN_W1_JUMP || r==MARCO_CAMPAIGN_W1_RUN_JUMP;
}
/* H programs (marco_hgun_program_t order): tables 0BFDD8..0BFF58 + 0x10.
 * Down in the air (marco-g1): burst 0BFE58 2560C0 and sweep forward->down
 * 0BFED8 2561DC. The sweep down->forward 25624E did not occur in any
 * capture: not in the catalog, never started (explicit stop: its pose stays
 * the previous one). */
static const int hgun_roles[7]={MARCO_CAMPAIGN_W1_BURST,MARCO_CAMPAIGN_W1_BURST_UP,MARCO_CAMPAIGN_W1_BURST_DOWN,
    MARCO_CAMPAIGN_W1_SWEEP_FWD_UP,MARCO_CAMPAIGN_W1_SWEEP_FWD_DOWN,MARCO_CAMPAIGN_W1_SWEEP_UP_FWD,-1};
/* Down in the air with the H: its down programs finished hold their last
 * pose (the settle 0C38B2 of aim FF, table 38DA, was not captured). */
static bool hgun_down(int r) {
    return r==MARCO_CAMPAIGN_W1_BURST_DOWN || r==MARCO_CAMPAIGN_W1_SWEEP_FWD_DOWN;
}
static bool hgun_role(int r) {
    for (unsigned i=0;i<7;i++) if (r>=0 && hgun_roles[i]==r) return true;
    return false;
}
/* The weapon program at rest. Form 1 (the H, rows +0x10; marco-hgun
 * capture): idle 0BF858 253402, run 0BF8D8 255ABE, jump 0BFC58 255CD6, run
 * jump 0BFC98 255E96; up pressed at rest raises the gun 0BF9D8 25349E,
 * then holds it 0BFA18 253450 (also after an upward sweep), and released
 * lowers it 0BFA98 2534B4 before the idle program. */
static int weapon_rest(const marco_visual_t *v,const marco_visual_input_t *in,bool running_jump) {
    if (in->form==2 && in->grounded) {
        /* Fat: up raises 0BF9D8, then holds 0BFA18; released lowers 0BFA98
         * (marco-f2 aim_up_right, walk_right). */
        int r=v->weapon_role;bool done=v->weapon_done;
        if (in->aim_up) {
            if (r==MARCO_CAMPAIGN_W_AIM_UP && !done) return r;
            if (r==MARCO_CAMPAIGN_W_AIM_UP || r==MARCO_CAMPAIGN_W2_AIM_HOLD || r==MARCO_CAMPAIGN_W_FIRE_UP) return MARCO_CAMPAIGN_W2_AIM_HOLD;
            return MARCO_CAMPAIGN_W_AIM_UP;
        }
        if ((r==MARCO_CAMPAIGN_W2_AIM_DOWN && !done) || r==MARCO_CAMPAIGN_W_AIM_UP || r==MARCO_CAMPAIGN_W2_AIM_HOLD)
            return MARCO_CAMPAIGN_W2_AIM_DOWN;
        return in->moving?MARCO_CAMPAIGN_W_RUN:MARCO_CAMPAIGN_W_IDLE;
    }
    if (in->form!=1) {
        if (!in->grounded) return running_jump?MARCO_CAMPAIGN_W_RUN_JUMP:MARCO_CAMPAIGN_W_JUMP;
        /* B84 (MAME marco-rt0): the raise 0BF9D8 ends (0C4194) in the hold
         * 0BFA18 24C65C (0C397E with +92 > 0), which the run's choice
         * 0C387E keeps (0BFA58, the same program) after a turn; released,
         * 0C4112 lowers the gun (0BFA98 24C6C0) before the rest. */
        int r=v->weapon_role;bool done=v->weapon_done,up=r<0 && v->m92>0;
        if (in->aim_up) {
            if (r==MARCO_CAMPAIGN_W_AIM_UP && !done) return r;
            if (r==MARCO_CAMPAIGN_W_AIM_UP || r==MARCO_CAMPAIGN_W_AIM_HOLD || r==MARCO_CAMPAIGN_W_FIRE_UP || up) return MARCO_CAMPAIGN_W_AIM_HOLD;
            return MARCO_CAMPAIGN_W_AIM_UP;
        }
        if ((r==MARCO_CAMPAIGN_W_AIM_DOWN && !done) || r==MARCO_CAMPAIGN_W_AIM_UP || r==MARCO_CAMPAIGN_W_AIM_HOLD || up)
            return MARCO_CAMPAIGN_W_AIM_DOWN;
        return in->moving?MARCO_CAMPAIGN_W_RUN:MARCO_CAMPAIGN_W_IDLE;
    }
    int r=v->weapon_role;bool done=v->weapon_done;
    /* The H lowered in the air without fire (0C3A5E, table 0C413C + 0x10
     * = 0BFB98 255F8E) was replaced by a burst in the same update in
     * marco-g1 (never drawn, one pose not captured): explicit stop, the
     * jump's weapon program stays. A down program keeps its last pose. */
    if (!in->grounded && in->aim_down && !in->cgun && hgun_down(r)) return r;
    if (!in->grounded) return running_jump?MARCO_CAMPAIGN_W1_RUN_JUMP:MARCO_CAMPAIGN_W1_JUMP;
    const bool up=r<0 && v->m92>0;   /* after a turn with +92 > 0 (marco-rt1: 0BFA58 253450) */
    if (in->aim_up) {
        if (r==MARCO_CAMPAIGN_W1_AIM_UP && !done) return r;
        /* A fire up's end (0C38B2 with +92 > 0: 0BFA18; MAME marco-rtc
         * 2435, the C's 253AC6 then 253450). */
        if (up_role(r) || up) return MARCO_CAMPAIGN_W1_AIM_HOLD;
        return MARCO_CAMPAIGN_W1_AIM_UP;
    }
    if ((r==MARCO_CAMPAIGN_W1_AIM_DOWN && !done) || r==MARCO_CAMPAIGN_W1_AIM_UP || r==MARCO_CAMPAIGN_W1_AIM_HOLD || up)
        return MARCO_CAMPAIGN_W1_AIM_DOWN;
    return in->moving?MARCO_CAMPAIGN_W1_RUN:MARCO_CAMPAIGN_W1_IDLE;
}
static void step_part(marco_visual_t *v,marco_part_t part) {
    bool *done=part==MARCO_BODY?&v->body_done:&v->weapon_done;
    int role=part==MARCO_BODY?v->body_role:v->weapon_role;
    if (role<0 || *done) return;   /* finished: its last draws stay listed */
    v->collect=part==MARCO_WEAPON;
    if (part==MARCO_BODY) v->body_count=0; else v->weapon_count=0;
    neo_anim_result_t r=marco_animation_step(&v->anim,part,&v->view,emit,v,256,event,NULL);
    if (r==NEO_ANIM_COMPLETE) *done=true;
    else if (r!=NEO_ANIM_MORE) {
        /* An unrecovered callback stops the program (never a silent no-op):
         * the pose is dropped and the situation's program restarts next. */
        v->failures++;v->anim.failed=false;*done=true;
        if (part==MARCO_BODY) {v->body_role=-1;v->body_count=0;} else {v->weapon_role=-1;v->weapon_count=0;}
    }
}
/* Form 3, the mummy (marco-mz2, run_marco_catalog.py --give-mummy). The
 * weapon part takes rows +0x30 of the weapon tables: idle 0BF858 25F520
 * (also the jump tables 0BFC58/0BFC98), walk 0BF8D8 25F562, fire 0BFD18
 * 25F5CA / up 0BFD58 25F89E / down in the air 0BFD98 25F932, grenade 0BF998
 * 25F626, up raises 0BF9D8 25F74C, holds 0BFA18 25F8F0 and released lowers
 * 0BFA98 25F7AC. The gun raised/lowered in the air (0BFB58 25F794 / 0BFB98
 * 25F7A0) is in the catalog but not selected (its condition, 0C3A5E, is not
 * translated). The body is the mummy controller's own (0C18B4..0C1D6C,
 * tables C0408.. through 0C0FB6, character 0):
 *  - the transformation 0C17A0..0C182A: body 0BF258 24EDE0 alone (0C0F16
 *    stops the weapon; MAME: 1887E0, 1887CC for 18 updates, no other part);
 *  - idle 0C18B4: C0408 25F3B4, restarted every update (+90 = 0);
 *  - walk 0C196A: C0418 25F3CC at speed 0x80 (+26), +90 = 1;
 *  - a facing change on the ground 0C1924 (+9A bit 3): 0C0F16 and the turn
 *    C0448 260B1C (C0458 with +92 set: not captured), the weapon hidden until
 *    the walk restarts; with +98 = 2 (the fire state) only the facing flips;
 *  - the takeoff 0C19E4: C0428 25F486 (its takeoff and air poses), the
 *    weapon running on; the landing 0C1AC8: C0478 25F4EE (three updates);
 *  - down 0C1B14: 0C0F16, crouching C0488 260B5E, then crouched C0498
 *    260BE6 (0C1B66), a shot C04B8 260C8A (0C1CCC, back to 0C1B66 at its
 *    end); released, the stand-up C04D8 260BA2 (0C1882; a shot or a bomb goes
 *    straight to 0C18B4). Crouched there is no weapon part (+90 = FF).
 * Not captured, no program (the body stays, counted in `stops`): walking off
 * a ledge C0438 25F4BC (0C1A48), the turn with +92 set C0458, crawling C04A8
 * and its turn C0468 (the port does not crawl), the crouched throw C04C8. */
static bool mummy_default(int r) {
    return r==MARCO_CAMPAIGN_W3_IDLE || r==MARCO_CAMPAIGN_W3_WALK || r==MARCO_CAMPAIGN_W3_AIM_UP ||
        r==MARCO_CAMPAIGN_W3_AIM_HOLD || r==MARCO_CAMPAIGN_W3_AIM_DOWN;
}
static bool mummy_crouch(int r) {
    return r==MARCO_CAMPAIGN_B3_CROUCH_IN || r==MARCO_CAMPAIGN_B3_CROUCH || r==MARCO_CAMPAIGN_B3_CROUCH_FIRE;
}
static bool mummy_firing(const marco_visual_t *v) {
    int r=v->weapon_role;
    return !v->weapon_done && (r==MARCO_CAMPAIGN_W3_FIRE || r==MARCO_CAMPAIGN_W3_FIRE_UP || r==MARCO_CAMPAIGN_W3_FIRE_DOWN);
}
static void mummy_step(marco_visual_t *v,const marco_visual_input_t *in) {
    int b=v->body_role;bool bdone=v->body_done;bool weapon=true;
    const bool turned=v->facing && in->facing!=v->facing;
    if (in->transforming) {
        if (b!=MARCO_CAMPAIGN_B3_TRANSFORM) start(v,MARCO_BODY,MARCO_CAMPAIGN_B3_TRANSFORM);
        weapon=false;
    } else if (in->takeoff || !in->grounded) {
        if (b!=MARCO_CAMPAIGN_B3_JUMP) {
            if (in->takeoff) start(v,MARCO_BODY,MARCO_CAMPAIGN_B3_JUMP);
            else if (v->grounded) v->stops++;   /* off a ledge: C0438 not captured */
        }
    } else if (b==MARCO_CAMPAIGN_B3_CROUCH_FIRE && v->crouch && !in->crouch) {
        /* 0C1CF8: down released, the shot's update runs (0C1D48 sets
         * 0C1882 for the next one). */
        weapon=false;
    } else if (in->crouch) {
        weapon=false;
        if (!mummy_crouch(b)) start(v,MARCO_BODY,MARCO_CAMPAIGN_B3_CROUCH_IN);
        else if (in->fired) start(v,MARCO_BODY,MARCO_CAMPAIGN_B3_CROUCH_FIRE);
        else if (bdone && b!=MARCO_CAMPAIGN_B3_CROUCH) start(v,MARCO_BODY,MARCO_CAMPAIGN_B3_CROUCH);
        if (turned) v->stops++;   /* the crawl's turn C0468 not captured */
    } else {
        bool shot=in->fired || in->thrown;
        if (mummy_crouch(b)) {
            if (shot) start(v,MARCO_BODY,MARCO_CAMPAIGN_B3_IDLE);
            else {start(v,MARCO_BODY,MARCO_CAMPAIGN_B3_STAND);weapon=false;}
            b=v->body_role;bdone=false;
        }
        if (b==MARCO_CAMPAIGN_B3_STAND && !bdone && !in->moving && !shot && !turned) weapon=false;
        else if (b==MARCO_CAMPAIGN_B3_JUMP || (b==MARCO_CAMPAIGN_B3_LAND && !bdone && !in->moving)) {
            /* 0C1A62 lands at the end of its update (the air pose drawn);
             * 0C1AC8 starts the landing in the next one. */
            if (b==MARCO_CAMPAIGN_B3_JUMP && v->grounded) start(v,MARCO_BODY,MARCO_CAMPAIGN_B3_LAND);
        } else if (turned && mummy_firing(v)) {
            /* +98 = 2: 0C1930 goes to 0C196A, the walk restarted (then
             * 0C1984 idles without a direction). */
            start(v,MARCO_BODY,in->moving?MARCO_CAMPAIGN_B3_WALK:MARCO_CAMPAIGN_B3_IDLE);
        } else if (turned) {
            if (in->aim_up) {v->stops++;start(v,MARCO_BODY,in->moving?MARCO_CAMPAIGN_B3_WALK:MARCO_CAMPAIGN_B3_IDLE);}
            else {start(v,MARCO_BODY,MARCO_CAMPAIGN_B3_TURN);weapon=false;}
        } else if (b==MARCO_CAMPAIGN_B3_TURN && !bdone) weapon=false;
        else if (in->moving) {if (b!=MARCO_CAMPAIGN_B3_WALK) start(v,MARCO_BODY,MARCO_CAMPAIGN_B3_WALK);}
        else if (b!=MARCO_CAMPAIGN_B3_IDLE || bdone) start(v,MARCO_BODY,MARCO_CAMPAIGN_B3_IDLE);
    }
    /* +90: 0C18B4 0, 0C196A 1, 0C19E4 3 (the air and the landing keep it),
     * 0C0F16 FF. A new value makes 0C3A50 choose the weapon's program again
     * (marco-mz2: 25F520 at 3623 after the landing, 2781 after the stand-up). */
    uint8_t m90=!weapon?0xffu:v->body_role==MARCO_CAMPAIGN_B3_JUMP || v->body_role==MARCO_CAMPAIGN_B3_LAND?3u:
        v->body_role==MARCO_CAMPAIGN_B3_WALK?1u:0u;
    bool renew=m90!=v->m90;v->m90=m90;
    if (!weapon) {v->weapon_role=-1;v->weapon_count=0;v->weapon_done=false;}
    else {
        if (renew && (v->weapon_done || mummy_default(v->weapon_role))) v->weapon_role=-1;
        int r=v->weapon_role;bool done=v->weapon_done;
        if (in->thrown) start(v,MARCO_WEAPON,MARCO_CAMPAIGN_W3_GRENADE);
        else if (in->fired) start(v,MARCO_WEAPON,in->aim_up?MARCO_CAMPAIGN_W3_FIRE_UP:in->aim_down && (!in->grounded || in->takeoff)?MARCO_CAMPAIGN_W3_FIRE_DOWN:MARCO_CAMPAIGN_W3_FIRE);
        else {
            int rest;
            if (in->aim_up) {
                if (r==MARCO_CAMPAIGN_W3_AIM_UP && !done) rest=r;
                else if (r==MARCO_CAMPAIGN_W3_AIM_UP || r==MARCO_CAMPAIGN_W3_AIM_HOLD || r==MARCO_CAMPAIGN_W3_FIRE_UP) rest=MARCO_CAMPAIGN_W3_AIM_HOLD;
                else rest=MARCO_CAMPAIGN_W3_AIM_UP;
            } else if ((r==MARCO_CAMPAIGN_W3_AIM_DOWN && !done) || r==MARCO_CAMPAIGN_W3_AIM_UP || r==MARCO_CAMPAIGN_W3_AIM_HOLD ||
                (r==MARCO_CAMPAIGN_W3_FIRE_UP && done)) rest=MARCO_CAMPAIGN_W3_AIM_DOWN;   /* marco-mz2 3555: hold 25F8F0, then down 25F7AC */
            else rest=in->moving && in->grounded?MARCO_CAMPAIGN_W3_WALK:MARCO_CAMPAIGN_W3_IDLE;
            /* The walk's weapon 25F562 picks its pose by +77 (written by the
             * body 25F3CC) and finishes: 0C3A50 chooses it again every
             * update (marco-mz2). */
            if (r<0 || (mummy_default(r) && r!=rest) || (done && !mummy_default(r) && r!=rest) ||
                (done && r==MARCO_CAMPAIGN_W3_WALK && rest==r)) start(v,MARCO_WEAPON,rest);
        }
    }
    step_part(v,MARCO_BODY);
    if (weapon) step_part(v,MARCO_WEAPON);
    v->grounded=in->grounded && !in->takeoff;v->crouch=in->crouch;v->moving=in->moving;v->facing=in->facing;
}
/* B84: the run's turn 0C2746 (tables 0BEE58, +92 > 0 0BEE98) and the
 * stop 0C1EFA (0BEE18), rows of forms 0 and 1. */
static int turn_role(const marco_visual_t *v,const marco_visual_input_t *in) {
    if (in->form==1) return v->m92>0?MARCO_CAMPAIGN_B1_TURN_RUN_UP:MARCO_CAMPAIGN_B1_TURN_RUN;
    return v->m92>0?MARCO_CAMPAIGN_B_TURN_RUN_UP:MARCO_CAMPAIGN_B_TURN_RUN;
}
static bool turn_role_of(int r) {
    return r>=0 && (r==MARCO_CAMPAIGN_B_TURN_RUN || r==MARCO_CAMPAIGN_B_TURN_RUN_UP || r==MARCO_CAMPAIGN_B1_TURN_RUN || r==MARCO_CAMPAIGN_B1_TURN_RUN_UP);
}
static bool stop_role_of(int r) {return r>=0 && (r==MARCO_CAMPAIGN_B_TURN || r==MARCO_CAMPAIGN_B1_TURN);}
marco_turn_gate_t marco_visual_turn_gate(const marco_visual_t *v) {
    marco_turn_gate_t t={false,false,v->facing};
    if (v->form>1) return t;
    /* The run 0C2888, and the idle and the stop 0C1FA6, whose stick starts
     * the run 0C285C, which goes on to 0C28BE in the same update (MAME
     * marco-rt0 2541: 0BF718 then 0BEE58). */
    const int b=v->body_role;
    t.ready=v->grounded && !v->crouch && (b==MARCO_CAMPAIGN_B_RUN || b==MARCO_CAMPAIGN_B_IDLE || stop_role_of(b)) && v->m98!=2 && v->facing!=0;
    t.hold=turn_role_of(v->body_role) && !v->body_done;
    return t;
}
void marco_visual_step(marco_visual_t *v,const marco_visual_input_t *in) {
    v->base_x=in->x;v->base_y=in->y;
    v->view.render.x=in->x;v->view.render.y=in->y;
    v->view.cached_x=in->x;v->view.cached_y=in->y;
    v->view.render.attributes=(uint16_t)(0x6340u|(in->facing<0?1u:0u));
    /* The player's palette slots +19..+1C (marco-mz2 actor dumps: 63 64 65
     * 00): a pose that selects one (0043F6) draws with its high byte. */
    v->view.render.palettes[0]=0x63;v->view.render.palettes[1]=0x64;v->view.render.palettes[2]=0x65;v->view.render.palettes[3]=0;
    /* Crouched, the body carries the gun (no weapon thread); with the H the
     * crouching bodies are the form-1 rows (0BEED8 256BE6, 0BEF18 256C06)
     * and a burst is the body's 0BFF98 256D52 (its 0C4372 callbacks). */
    bool crouch=in->grounded && in->crouch;
    if (v->form!=in->form) {v->body_role=v->weapon_role=-1;v->body_count=v->weapon_count=0;v->weapon_done=false;}
    v->form=in->form;
    if (in->form==3) {mummy_step(v,in);return;}
    /* The fat transformation 0C1D70: body C04F8 25CDEA alone (0C0FB6 /
     * 0C0F16), +84 = 0x5A, until 0C1DA8 ends it (marco-f2 2103). */
    if (in->form==2 && in->transforming) {
        if (v->body_role!=MARCO_CAMPAIGN_B2_TRANSFORM) start(v,MARCO_BODY,MARCO_CAMPAIGN_B2_TRANSFORM);
        v->weapon_role=-1;v->weapon_count=0;v->weapon_done=false;
        step_part(v,MARCO_BODY);
        v->grounded=in->grounded;v->crouch=false;v->moving=in->moving;v->facing=in->facing;
        return;
    }
    /* The mission end (0C2016..0C2046): standing idle with RAM 10C301 set
     * and 10C304 >> 2 & +5E, the idle sets the routine 0C1E84, which in the
     * next update starts the victory table 0BF658 (body; 0C0F16: no weapon
     * thread) and goes back to the idle 0C1FA6, whose check is then off
     * (+9A bit 5 cleared by 0C1F32): the program runs on to the end of the
     * mission (MAME runK 13701..14456, run4 18648, 117325). */
    {
        const int role=in->form==1?MARCO_CAMPAIGN_B1_VICTORY:MARCO_CAMPAIGN_B_VICTORY;
        if (v->body_role==role && in->victory) {
            v->weapon_role=-1;v->weapon_count=0;v->weapon_done=false;
            step_part(v,MARCO_BODY);
            v->grounded=in->grounded;v->crouch=false;v->moving=in->moving;v->facing=in->facing;
            return;
        }
        if (in->victory && in->grounded && !in->crouch && !in->moving && !in->fired && !in->thrown) {
            if (v->victory_armed) {
                v->victory_armed=false;start(v,MARCO_BODY,role);
                v->weapon_role=-1;v->weapon_count=0;v->weapon_done=false;
                step_part(v,MARCO_BODY);
                v->grounded=in->grounded;v->crouch=false;v->moving=false;v->facing=in->facing;
                return;
            }
            v->victory_armed=true;
        } else v->victory_armed=false;
    }
    bool hgun_start=in->form==1 && in->hgun_start && in->hgun_program>=0 && in->hgun_program<7;
    /* The turn as the fire saw it (gameplay.c reads the same gate). */
    const marco_turn_gate_t gate=marco_visual_turn_gate(v);
    if (v->body_role==MARCO_CAMPAIGN_B_RUN) v->m80>>=1;   /* 0C28A2 */
    /* 0C2006: a fire or bomb press (the shot or throw of this update). */
    const bool pressed=in->fired || in->thrown || in->knife || hgun_start || (in->cgun && in->cgun_fire);
    /* Body. */
    if (!in->grounded && in->hanging) {
        /* Hanging from the canopy (0C15C6: body 0BF818 24E5AA, weapon the
         * jump's 0BFC58 24E5BC; marco-dsc0). */
        if (v->body_role!=MARCO_CAMPAIGN_B_HANG) {
            start(v,MARCO_BODY,MARCO_CAMPAIGN_B_HANG);v->running_jump=false;
            if (!hgun_role(v->weapon_role) || v->weapon_done) start(v,MARCO_WEAPON,weapon_rest(v,in,false));
        }
    } else if (!in->grounded) {
        /* The cut (0C16D8) starts the fall with the jump body 0BF758
         * 24E4E0 (marco-dsc1/dsc2); the weapon program stays. */
        if (v->body_role==MARCO_CAMPAIGN_B_HANG) {v->running_jump=false;start(v,MARCO_BODY,MARCO_CAMPAIGN_B_JUMP);}
        else if (v->grounded || v->body_role<0) {
            v->running_jump=in->running_jump;
            start(v,MARCO_BODY,v->running_jump?MARCO_CAMPAIGN_B_RUN_JUMP:MARCO_CAMPAIGN_B_JUMP);
            if (!hgun_role(v->weapon_role) || v->weapon_done) start(v,MARCO_WEAPON,weapon_rest(v,in,v->running_jump));
        }
    } else if (crouch) {
        /* A throw crouched (0C2458): body 0BF098, then the after-throw
         * crouch 0BEF98 (0C2294; it loops like the crouch 0BEF18). The C
         * crouched: 0C246A with its table C0398 (256EB6). */
        /* The knife crouched (0C3B46): body table 0BF118 (marco-k1/k2),
         * then the crouch. */
        if (in->knife) start(v,MARCO_BODY,in->form==1?MARCO_CAMPAIGN_B1_CROUCH_KNIFE:MARCO_CAMPAIGN_B_CROUCH_KNIFE);
        else if (in->form==1) {
            if (in->thrown) start(v,MARCO_BODY,MARCO_CAMPAIGN_B1_CROUCH_GRENADE);
            else if (hgun_start) start(v,MARCO_BODY,MARCO_CAMPAIGN_B1_CROUCH_FIRE);
            else if (in->cgun && in->cgun_fire) start(v,MARCO_BODY,in->cgroup==2?MARCO_CAMPAIGN_B1S_CROUCH_FIRE:in->cgroup==3?MARCO_CAMPAIGN_B1F_CROUCH_FIRE:in->cgroup==4?MARCO_CAMPAIGN_B1L_CROUCH_FIRE:MARCO_CAMPAIGN_B1C_CROUCH_FIRE);
            else if (!crouch_role(v->body_role)) start(v,MARCO_BODY,MARCO_CAMPAIGN_B1_CROUCH_IN);
            else if (v->body_done) start(v,MARCO_BODY,v->body_role==MARCO_CAMPAIGN_B1_CROUCH_GRENADE?MARCO_CAMPAIGN_B1_CROUCH_AFTER_GRENADE:MARCO_CAMPAIGN_B1_CROUCH);
        } else {
            if (in->thrown) start(v,MARCO_BODY,MARCO_CAMPAIGN_B_CROUCH_GRENADE);
            else if (in->fired) start(v,MARCO_BODY,MARCO_CAMPAIGN_B_CROUCH_FIRE);
            else if (!crouch_role(v->body_role)) start(v,MARCO_BODY,MARCO_CAMPAIGN_B_CROUCH_IN);
            else if (v->body_done) start(v,MARCO_BODY,v->body_role==MARCO_CAMPAIGN_B_CROUCH_GRENADE?MARCO_CAMPAIGN_B_CROUCH_AFTER_GRENADE:MARCO_CAMPAIGN_B_CROUCH);
        }
        v->weapon_role=-1;v->weapon_count=0;v->m98=0;   /* 0C0F16 */
    } else if (marco_turn_gate_blocks(&gate,in->grounded,crouch,in->moving,in->facing) && !gate.hold) {
        /* The run's turn (0C28BE: +9A bit 3; 0C28C8: with +98 = 2 only the
         * facing flips, 0C28D2). 0C2746: the facing flips, body 0BEE58
         * (+92 > 0: 0BEE98), rows by +93 (form 1 +0x10: 256FF2 / 257008),
         * its first pose mirrored back (MAME marco-run-fire-turn 188958 /0
         * then /1), 0C0F16 (+98 = 0, +90 = +91 = FF: no weapon, the shot
         * not taken), state 0C27A8 until the program's end, then 0C285C
         * restarts the run (+77 = 0, +80 = 0x3F, 0BF718) and its weapon.
         * (+98 = 1, the gun raised or lowered (0C4168), takes 0C27E8 and
         * 0BF7D8: not captured, docs/BUG_MATRIX.md B84.) */
        start(v,MARCO_BODY,turn_role(v,in));v->m98=0;
    } else if (turn_role_of(v->body_role) && !v->body_done) {
        /* 0C27A8 steps the turn (00427A); 0C285C from the next update. */
    } else if (stop_role_of(v->body_role) && !v->body_done && !in->moving && !pressed && !in->aim_up) {
        /* 0C1FA6 with +9A bit 5 clear: up, fire or bomb (0x51) take the
         * idle 0C1F4A, the stick the run 0C285C; else 0C2144 steps the
         * stop and its end takes the idle (next update). */
    } else if (in->form<=1 && v->grounded && v->body_role==MARCO_CAMPAIGN_B_RUN && !in->moving && !v->m80 && v->m92<=0 && !v->m98 && !pressed && !in->aim_up) {
        /* 0C28E2: the stick released after the run's +80 is 0 (six run
         * updates): 0C1EFA with +92 <= 0 and +98 = 0 starts the stop
         * 0BEE18 (24EF50, form 1 257034) with no weapon (0C0F16), then
         * 0C1FA6 in the same update: a fire or bomb press or up held there
         * (0x51) goes on to the idle 0C1F4A at once (and its weapon fires
         * in this update), so those keep the idle. */
        start(v,MARCO_BODY,in->form==1?MARCO_CAMPAIGN_B1_TURN:MARCO_CAMPAIGN_B_TURN);v->m98=0;
    } else {
        int body=in->moving?MARCO_CAMPAIGN_B_RUN:MARCO_CAMPAIGN_B_IDLE;
        if (!v->grounded || v->body_role!=body || v->body_done) {
            if (body==MARCO_CAMPAIGN_B_RUN) {v->anim.fields.frame77=0;v->m80=0x3f>>1;}   /* 0C2864, 0C287C, 0C28A2 */
            start(v,MARCO_BODY,body);
        }
    }
    const bool turning=turn_role_of(v->body_role) || stop_role_of(v->body_role);   /* 0C0F16: no weapon thread */
    /* Weapon. With the H the fire states of marco_hgun.c choose the
     * program (0C3E9C/0C3F1A/0C3FA6..: its start or restart this update). */
    /* 0C3D9A: +76 written by the fire program ends +98 = 2 (0C3DA6). */
    /* (The H's states 0C3F1A.. keep +98 = 2 until the weapon is chosen
     * again, 0C38B2: MAME marco-rt1 2149..2263.) */
    if (v->m98==2 && v->anim.fields.field76 && !hgun_role(v->weapon_role)) v->m98=0;
    if (turning) {v->weapon_role=-1;v->weapon_count=0;v->weapon_done=false;}
    else if (!crouch) {
        int rest=weapon_rest(v,in,v->running_jump);
        /* 0C3A5E tests the bomb (bit 6) before the fire (bit 4): a throw
         * this update starts 0BF998 (pistol 24C862, form 1 253B3A). */
        if (in->thrown) {
            start(v,MARCO_WEAPON,in->form==1?MARCO_CAMPAIGN_W1_GRENADE:MARCO_CAMPAIGN_W_GRENADE);
            v->anim.fields.field76=0;v->m98=2;   /* 0C3BD2, 0C3BFE..0C3D74 */
        }
        /* The knife standing: weapon table 0BF958 (form 1: +0x10). */
        else if (in->knife) start(v,MARCO_WEAPON,in->form==1?MARCO_CAMPAIGN_W1_KNIFE:MARCO_CAMPAIGN_W_KNIFE);
        else if (hgun_start && hgun_roles[in->hgun_program]>=0) {start(v,MARCO_WEAPON,hgun_roles[in->hgun_program]);v->m98=2;}   /* 0C3EB2 */
        else if (in->form==1 && in->cgun && in->cgun_fire)
        {
            /* The weapon's tables 0C3C10/0C3C50/0C3C90 (+0x10): the C's
             * C02D8.., ids 02/07 0BFFD8.., ids 04/09 0C00D8.., ids 06/0B
             * the beam's 0C01D8... */
            static const int fire[5][3]={
                {MARCO_CAMPAIGN_W1C_FIRE,MARCO_CAMPAIGN_W1C_FIRE_UP,MARCO_CAMPAIGN_W1C_FIRE_DOWN},
                {MARCO_CAMPAIGN_W1C_FIRE,MARCO_CAMPAIGN_W1C_FIRE_UP,MARCO_CAMPAIGN_W1C_FIRE_DOWN},
                {MARCO_CAMPAIGN_W1S_FIRE,MARCO_CAMPAIGN_W1S_FIRE_UP,MARCO_CAMPAIGN_W1S_FIRE_DOWN},
                {MARCO_CAMPAIGN_W1F_FIRE,MARCO_CAMPAIGN_W1F_FIRE_UP,MARCO_CAMPAIGN_W1F_FIRE_DOWN},
                {MARCO_CAMPAIGN_W1L_FIRE,MARCO_CAMPAIGN_W1L_FIRE_UP,MARCO_CAMPAIGN_W1L_FIRE_DOWN}};
            const int *f=fire[in->cgroup<5u?in->cgroup:0u];
            start(v,MARCO_WEAPON,in->aim_up?f[1]:in->aim_down && !in->grounded?f[2]:f[0]);
            v->anim.fields.field76=0;v->m98=2;   /* 0C3CE2, 0C3D8C (the beam 0C3E18, +76 at its end) */
        }
        else if (in->form!=1 && in->fired) {
            start(v,MARCO_WEAPON,in->aim_up?MARCO_CAMPAIGN_W_FIRE_UP:in->aim_down && !in->grounded?MARCO_CAMPAIGN_W_FIRE_DOWN:MARCO_CAMPAIGN_W_FIRE);
            v->anim.fields.field76=0;v->m98=2;   /* 0C3D8C, 0C3CE2 */
        }
        else if (v->weapon_role<0 || (weapon_default(v->weapon_role) && v->weapon_role!=rest) ||
            (v->weapon_done && !weapon_default(v->weapon_role) && v->weapon_role!=rest) ||
            (v->weapon_done && v->weapon_role==rest && weapon_reselected(rest)) ||
            (v->grounded!=in->grounded && in->grounded && !hgun_role(v->weapon_role)))
            {start(v,MARCO_WEAPON,rest);v->m98=0;}   /* 0C381C..0C397E: +98 = 0 */
    }
    step_part(v,MARCO_BODY);
    if (!crouch && !turning) step_part(v,MARCO_WEAPON);
    v->grounded=in->grounded;v->crouch=crouch;v->moving=in->moving;v->facing=in->facing;
}
unsigned marco_visual_draws(const marco_visual_t *v,neo_anim_draw_t *out,unsigned max) {
    unsigned n=0;
    for (unsigned part=0;part<2;part++) {
        const neo_anim_draw_t *list=part?v->weapon_draws:v->body_draws;
        unsigned count=part?(v->weapon_role>=0?v->weapon_count:0):v->body_count;
        for (unsigned i=0;i<count && n<max;i++) {
            out[n]=list[i];out[n].x=(uint16_t)(out[n].x+v->base_x);out[n].y=(uint16_t)(out[n].y+v->base_y);n++;
        }
    }
    return n;
}
