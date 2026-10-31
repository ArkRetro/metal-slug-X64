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
    uint16_t speed=part==MARCO_BODY && role==MARCO_CAMPAIGN_B_RUN?0x1c1:0x100;
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
        r==MARCO_CAMPAIGN_B1_CROUCH_GRENADE || r==MARCO_CAMPAIGN_B1_CROUCH_AFTER_GRENADE || r==MARCO_CAMPAIGN_B1C_CROUCH_FIRE;
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
void marco_visual_step(marco_visual_t *v,const marco_visual_input_t *in) {
    v->base_x=in->x;v->base_y=in->y;
    v->view.render.x=in->x;v->view.render.y=in->y;
    v->view.cached_x=in->x;v->view.cached_y=in->y;
    v->view.render.attributes=(uint16_t)(0x6340u|(in->facing<0?1u:0u));
    /* Crouched, the body carries the gun (no weapon thread); with the H the
     * crouching bodies are the form-1 rows (0BEED8 256BE6, 0BEF18 256C06)
     * and a burst is the body's 0BFF98 256D52 (its 0C4372 callbacks). */
    bool crouch=in->grounded && in->crouch;
    if (v->form!=in->form) {v->body_role=v->weapon_role=-1;v->body_count=v->weapon_count=0;v->weapon_done=false;}
    v->form=in->form;
    bool hgun_start=in->form==1 && in->hgun_start && in->hgun_program>=0 && in->hgun_program<7;
    /* Body. */
    if (!in->grounded) {
        if (v->grounded || v->body_role<0) {
            v->running_jump=in->running_jump;
            start(v,MARCO_BODY,v->running_jump?MARCO_CAMPAIGN_B_RUN_JUMP:MARCO_CAMPAIGN_B_JUMP);
            if (!hgun_role(v->weapon_role) || v->weapon_done) start(v,MARCO_WEAPON,weapon_rest(v,in,v->running_jump));
        }
    } else if (crouch) {
        /* A throw crouched (0C2458): body 0BF098, then the after-throw
         * crouch 0BEF98 (0C2294; it loops like the crouch 0BEF18). The C
         * crouched: 0C246A with its table C0398 (256EB6). */
        if (in->form==1) {
            if (in->thrown) start(v,MARCO_BODY,MARCO_CAMPAIGN_B1_CROUCH_GRENADE);
            else if (hgun_start) start(v,MARCO_BODY,MARCO_CAMPAIGN_B1_CROUCH_FIRE);
            else if (in->cgun && in->cgun_fire) start(v,MARCO_BODY,MARCO_CAMPAIGN_B1C_CROUCH_FIRE);
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
        else if (hgun_start && hgun_roles[in->hgun_program]>=0) start(v,MARCO_WEAPON,hgun_roles[in->hgun_program]);
        else if (in->form==1 && in->cgun && in->cgun_fire)
            start(v,MARCO_WEAPON,in->aim_up?MARCO_CAMPAIGN_W1C_FIRE_UP:in->aim_down && !in->grounded?MARCO_CAMPAIGN_W1C_FIRE_DOWN:MARCO_CAMPAIGN_W1C_FIRE);
        else if (in->form!=1 && in->fired) start(v,MARCO_WEAPON,in->aim_up?MARCO_CAMPAIGN_W_FIRE_UP:in->aim_down && !in->grounded?MARCO_CAMPAIGN_W_FIRE_DOWN:MARCO_CAMPAIGN_W_FIRE);
        else if (v->weapon_role<0 || (weapon_default(v->weapon_role) && v->weapon_role!=rest) ||
            (v->weapon_done && !weapon_default(v->weapon_role) && v->weapon_role!=rest) ||
            (v->grounded!=in->grounded && in->grounded && !hgun_role(v->weapon_role)))
            start(v,MARCO_WEAPON,rest);
    }
    step_part(v,MARCO_BODY);
    if (!crouch) step_part(v,MARCO_WEAPON);
    v->grounded=in->grounded;v->crouch=crouch;v->moving=in->moving;
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
