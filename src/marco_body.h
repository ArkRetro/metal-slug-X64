#ifndef MSX_MARCO_BODY_H
#define MSX_MARCO_BODY_H
#include "marco_stance.h"
/* One shared actor; no copying of body/weapon fields between controllers. */
typedef enum {MARCO_BODY_MOTION,MARCO_BODY_CROUCH,MARCO_BODY_STANCE} marco_body_domain_t;
typedef struct {marco_body_domain_t domain;marco_motion_t motion;marco_stance_t stance;} marco_body_t;
typedef enum {
    MARCO_BODY_FAILED,
    MARCO_BODY_COMMON, /* Run common processing once, then finish this tick. */
    MARCO_BODY_EXTERNAL /* An unrecovered destination needs an explicit owner. */
} marco_body_result_t;
/* Dispatches recovered motion/crouch/stance-entry transitions. EXTERNAL does not mean
 * successful idle: the caller must resolve the retained mode or stop. This
 * dispatcher does not yet replace the campaign controller. */
marco_body_result_t marco_body_update(marco_body_t*,marco_animation_t*,
    const marco_motion_services_t*,const marco_crouch_services_t*,const marco_stance_services_t*);
#endif
