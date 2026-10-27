#ifndef MSX_NEO_CONTACT_H
#define MSX_NEO_CONTACT_H
#include <stdbool.h>
#include <stdint.h>
/* Native numerical fields; tags identify scene objects, never CPU addresses. */
typedef struct {
    int32_t x,y,vx,vy,scalar62,projected_x,projected_y,cached_x,cached_y;
    uint32_t parent_tag;
    uint16_t kind_angle,field66,word1e;
    uint8_t flags15,flags5f,timer5a,mark,correction;
} neo_contact_actor_t;
typedef struct {
    uint32_t tag;
    int32_t x,y,previous_x,previous_y;
    uint8_t correction;
} neo_contact_parent_t;
typedef struct {
    int32_t x,y,dx,dy;
    uint32_t parent_tag,mode;
    uint8_t mode8,mode9,modea,moded;
} neo_contact_request_t;
typedef struct {
    int32_t x,y,vx,vy;
    uint32_t parent_tag,kind_angle_word;
    uint16_t flags;
} neo_contact_result_t;
/* ROM 002E98: exact signed ASR/low-word MULS, no float trigonometry. */
void neo_contact_vector(int32_t scalar,uint8_t angle,int32_t *vx,int32_t *vy);
/* ROM 01456A..01469E: clears prior wall bits and prepares the kernel request. */
neo_contact_request_t neo_contact_prepare_ground(neo_contact_actor_t *actor);
/* ROM 0143E4..01448C: direct air velocity and independent wall-bit clearing. */
neo_contact_request_t neo_contact_prepare_air(neo_contact_actor_t *actor);
/* ROM 01ABC8 projection/history path, without a collision commit. */
void neo_contact_project(neo_contact_actor_t *actor,const neo_contact_parent_t *parent,uint8_t epoch);
/* ROM 014492..014520 plus 01ABC8/01AD8C. Parent must describe result.parent_tag. */
void neo_contact_commit(neo_contact_actor_t *actor,const neo_contact_result_t *result,
    const neo_contact_parent_t *parent,uint8_t epoch);
/* Opt-in single-plane terrain subset. Caller must establish a static level
 * context without relevant dynamic collision candidates; this API has no actor
 * broadphase input. Attached/crossing, group C0, destination
 * shapes 1..4, 6..9 or 11; observed declining-slope/flat downward clip, no stacked floors or
 * dynamic object collisions. Returns false for
 * unsupported cases; this is not the complete 013968 collision kernel. */
bool neo_contact_plane_ground(const neo_contact_actor_t *actor,
    const neo_contact_request_t *request,neo_contact_result_t *result);
bool neo_contact_ground_step(neo_contact_actor_t *actor,const neo_contact_parent_t *parent,
    uint8_t epoch,neo_contact_result_t *kernel_output,uint32_t *returned_word);
#endif
