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
    if (!marco_animation_start(&v->anim,part,&marco_script,(uint16_t)role,0x100)) v->failures++;
    v->starts++;
}
void marco_visual_reset(marco_visual_t *v) {
    memset(v,0,sizeof(*v));marco_animation_reset(&v->anim);
    v->body_role=v->weapon_role=-1;v->grounded=true;
}
static bool crouch_role(int r) {
    return r==MARCO_CAMPAIGN_B_CROUCH_IN || r==MARCO_CAMPAIGN_B_CROUCH || r==MARCO_CAMPAIGN_B_CROUCH_FIRE;
}
static bool weapon_default(int r) {
    return r==MARCO_CAMPAIGN_W_IDLE || r==MARCO_CAMPAIGN_W_RUN || r==MARCO_CAMPAIGN_W_JUMP ||
        r==MARCO_CAMPAIGN_W_RUN_JUMP || r==MARCO_CAMPAIGN_W_AIM_UP;
}
static int weapon_rest(const marco_visual_input_t *in,bool running_jump) {
    if (!in->grounded) return running_jump?MARCO_CAMPAIGN_W_RUN_JUMP:MARCO_CAMPAIGN_W_JUMP;
    if (in->aim_up) return MARCO_CAMPAIGN_W_AIM_UP;
    return in->moving?MARCO_CAMPAIGN_W_RUN:MARCO_CAMPAIGN_W_IDLE;
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
    bool crouch=in->grounded && in->crouch;
    /* Body. */
    if (!in->grounded) {
        if (v->grounded || v->body_role<0) {
            v->running_jump=in->running_jump;
            start(v,MARCO_BODY,v->running_jump?MARCO_CAMPAIGN_B_RUN_JUMP:MARCO_CAMPAIGN_B_JUMP);
            start(v,MARCO_WEAPON,v->running_jump?MARCO_CAMPAIGN_W_RUN_JUMP:MARCO_CAMPAIGN_W_JUMP);
        }
    } else if (crouch) {
        if (in->fired) start(v,MARCO_BODY,MARCO_CAMPAIGN_B_CROUCH_FIRE);
        else if (!crouch_role(v->body_role)) start(v,MARCO_BODY,MARCO_CAMPAIGN_B_CROUCH_IN);
        else if (v->body_done) start(v,MARCO_BODY,MARCO_CAMPAIGN_B_CROUCH);
        v->weapon_role=-1;v->weapon_count=0;
    } else {
        int body=in->moving?MARCO_CAMPAIGN_B_RUN:MARCO_CAMPAIGN_B_IDLE;
        if (!v->grounded || v->body_role!=body || v->body_done) start(v,MARCO_BODY,body);
    }
    /* Weapon (none while crouched: the crouching body carries the arms). */
    if (!crouch) {
        int rest=weapon_rest(in,v->running_jump);
        if (in->fired) start(v,MARCO_WEAPON,in->aim_up?MARCO_CAMPAIGN_W_FIRE_UP:MARCO_CAMPAIGN_W_FIRE);
        else if (v->weapon_role<0 || (weapon_default(v->weapon_role) && v->weapon_role!=rest) ||
            (v->weapon_done && !weapon_default(v->weapon_role)) || (v->grounded!=in->grounded && in->grounded))
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
