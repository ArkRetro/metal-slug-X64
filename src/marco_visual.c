#include "marco_visual.h"
#include <string.h>
#include "marco_campaign.h"
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
static void start(marco_visual_t *v,marco_part_t part,int role) {
    if (part==MARCO_BODY) {v->body_role=role;v->body_done=false;}
    else {v->weapon_role=role;v->weapon_done=false;}
    if (role<0) return;
    /* Body speed +26: the run starts at 0x1C1 (C285C, marco_motion.c; cat1:
     * every run update of 24DD18 has 01C1), every other program at 0x100. */
    /* The mummy's walk 0C196A: +26 = 0x80 (marco-mz2, every update of 25F3CC). */
    uint16_t speed=part==MARCO_BODY && role==MARCO_CAMPAIGN_B_RUN?0x1c1:part==MARCO_BODY && role==MARCO_CAMPAIGN_B3_WALK?0x80:0x100;
    if (!marco_animation_start(&v->anim,part,&marco_script,(uint16_t)role,speed)) v->failures++;
    v->starts++;
}
void marco_visual_reset(marco_visual_t *v) {
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
        r==MARCO_CAMPAIGN_W_RUN_JUMP || r==MARCO_CAMPAIGN_W_AIM_UP ||
        r==MARCO_CAMPAIGN_W1_IDLE || r==MARCO_CAMPAIGN_W1_RUN || r==MARCO_CAMPAIGN_W1_JUMP ||
        r==MARCO_CAMPAIGN_W1_RUN_JUMP || r==MARCO_CAMPAIGN_W1_AIM_UP || r==MARCO_CAMPAIGN_W1_AIM_HOLD ||
        r==MARCO_CAMPAIGN_W1_AIM_DOWN;
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
    if (in->form!=1) {
        if (!in->grounded) return running_jump?MARCO_CAMPAIGN_W_RUN_JUMP:MARCO_CAMPAIGN_W_JUMP;
        if (in->aim_up) return MARCO_CAMPAIGN_W_AIM_UP;
        return in->moving?MARCO_CAMPAIGN_W_RUN:MARCO_CAMPAIGN_W_IDLE;
    }
    int r=v->weapon_role;bool done=v->weapon_done;
    /* The H lowered in the air without fire (0C3A5E, table 0C413C + 0x10
     * = 0BFB98 255F8E) was replaced by a burst in the same update in
     * marco-g1 (never drawn, one pose not captured): explicit stop, the
     * jump's weapon program stays. A down program keeps its last pose. */
    if (!in->grounded && in->aim_down && !in->cgun && hgun_down(r)) return r;
    if (!in->grounded) return running_jump?MARCO_CAMPAIGN_W1_RUN_JUMP:MARCO_CAMPAIGN_W1_JUMP;
    if (in->aim_up) {
        if (r==MARCO_CAMPAIGN_W1_AIM_UP && !done) return r;
        if (r==MARCO_CAMPAIGN_W1_AIM_UP || r==MARCO_CAMPAIGN_W1_AIM_HOLD || r==MARCO_CAMPAIGN_W1_SWEEP_FWD_UP ||
            r==MARCO_CAMPAIGN_W1_BURST_UP) return MARCO_CAMPAIGN_W1_AIM_HOLD;
        return MARCO_CAMPAIGN_W1_AIM_UP;
    }
    if ((r==MARCO_CAMPAIGN_W1_AIM_DOWN && !done) || r==MARCO_CAMPAIGN_W1_AIM_UP || r==MARCO_CAMPAIGN_W1_AIM_HOLD)
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
    bool hgun_start=in->form==1 && in->hgun_start && in->hgun_program>=0 && in->hgun_program<7;
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
        v->weapon_role=-1;v->weapon_count=0;
    } else {
        int body=in->moving?MARCO_CAMPAIGN_B_RUN:MARCO_CAMPAIGN_B_IDLE;
        if (!v->grounded || v->body_role!=body || v->body_done) start(v,MARCO_BODY,body);
    }
    /* Weapon. With the H the fire states of marco_hgun.c choose the
     * program (0C3E9C/0C3F1A/0C3FA6..: its start or restart this update). */
    if (!crouch) {
        int rest=weapon_rest(v,in,v->running_jump);
        /* 0C3A5E tests the bomb (bit 6) before the fire (bit 4): a throw
         * this update starts 0BF998 (pistol 24C862, form 1 253B3A). */
        if (in->thrown) start(v,MARCO_WEAPON,in->form==1?MARCO_CAMPAIGN_W1_GRENADE:MARCO_CAMPAIGN_W_GRENADE);
        /* The knife standing: weapon table 0BF958 (form 1: +0x10). */
        else if (in->knife) start(v,MARCO_WEAPON,in->form==1?MARCO_CAMPAIGN_W1_KNIFE:MARCO_CAMPAIGN_W_KNIFE);
        else if (hgun_start && hgun_roles[in->hgun_program]>=0) start(v,MARCO_WEAPON,hgun_roles[in->hgun_program]);
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
        }
        else if (in->form!=1 && in->fired) start(v,MARCO_WEAPON,in->aim_up?MARCO_CAMPAIGN_W_FIRE_UP:in->aim_down && !in->grounded?MARCO_CAMPAIGN_W_FIRE_DOWN:MARCO_CAMPAIGN_W_FIRE);
        else if (v->weapon_role<0 || (weapon_default(v->weapon_role) && v->weapon_role!=rest) ||
            (v->weapon_done && !weapon_default(v->weapon_role) && v->weapon_role!=rest) ||
            (v->grounded!=in->grounded && in->grounded && !hgun_role(v->weapon_role)))
            start(v,MARCO_WEAPON,rest);
    }
    step_part(v,MARCO_BODY);
    if (!crouch) step_part(v,MARCO_WEAPON);
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
