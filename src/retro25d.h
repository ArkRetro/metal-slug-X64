#ifndef MSX_RETRO25D_H
#define MSX_RETRO25D_H
#include <stdbool.h>
#include <stdint.h>
/* Pure C (host-testable) core of the depth25d visual mode. Nothing here reads
 * or writes sprite pixels, gameplay state or collision: it only produces
 * camera/plane transforms, vertex lighting and Gouraud triangles. */
enum { RETRO_MAX_LIGHTS=2, RETRO_MAX_TRIANGLES=96, RETRO_FOG_BYTES=1024,
       RETRO_GROUND_SEGMENTS=8, RETRO_GROUND_ROWS=1,
       RETRO_PLANE_TRIANGLES=8, RETRO_MAX_VISIBLE_PLANES=5,
       RETRO_SHADOW_SEGMENTS=8, RETRO_MAX_SHADOWS=4, RETRO_MAX_PULSES=4 };
/* Planes are culled against [camera_x-MARGIN, camera_x+320+MARGIN]. */
#define RETRO_CULL_MARGIN 32.0f
/* Authored planes stay close to the gameplay plane, so the margin is safe. */
#define RETRO_PLANE_MAX_DEPTH 24.0f
typedef struct { float x,y,z; } retro_vec3_t;
typedef struct { float x,y,z,focal; } retro_camera_t;
typedef struct { retro_vec3_t position,color; float radius,intensity; } retro_light_t;
typedef struct { float left,top,right,bottom,depth,dx,dy; } retro_plane_t;
/* World X; feet/ground Y in world screen units (Y down). radius is the
 * footprint half-width; squash is the ellipse's vertical/horizontal ratio
 * (0 selects the default 0.25). */
typedef struct { float world_x,feet_y,ground_y,radius,squash; } retro_shadow_t;
typedef struct {
    retro_camera_t camera;
    retro_light_t lights[RETRO_MAX_LIGHTS];
    retro_shadow_t shadows[RETRO_MAX_SHADOWS];
    unsigned light_count,shadow_count,tick;
    float focus; /* 0..1 camera-motion focus pull: slightly denser far fog. */
} retro_frame_t;
typedef struct { float x,y,inverse_w; } retro_projected_t;
typedef struct { float scale,offset_x,offset_y; } retro_plane_view_t;
retro_vec3_t retro_normal(float depth_dx,float depth_dy);
bool retro_project(retro_camera_t camera,retro_vec3_t point,retro_projected_t *out);
retro_vec3_t retro_billboard_point(float asset_x,float asset_y,float depth,float focal);
bool retro_plane_projection(retro_camera_t camera,float depth,retro_plane_view_t *out);
/* Layers 2..4 share gameplay coordinates (full-speed terrain and props).
 * Their texture projection must stay aligned with sprites and 2D collision. */
bool retro_layered_2d_projection(retro_camera_t camera,unsigned layer,retro_plane_view_t *out);
bool retro_background_projection(retro_camera_t camera,unsigned layer,float depth,retro_plane_view_t *out);
float retro_light_energy(retro_vec3_t point,retro_vec3_t normal,const retro_light_t *light);
void retro_lighting(const retro_frame_t *frame,retro_vec3_t point,retro_vec3_t normal,float rgba[4]);
/* rgba is normalized 0..1, matching libdragon's triangle shade format. */
unsigned retro_mesh_budget(unsigned ground_segments,unsigned planes,unsigned shadows);

/* ---- Frame builders: generic API for any actor or effect. ---- */
/* Contact shadow for a 2D actor about 2*half_width pixels wide. feet_y is
 * where its feet are now and ground_y the support below; the shadow fades
 * and shrinks while airborne. Example for a soldier at world x, on ground:
 *   retro_frame_add_shadow(&frame,retro_actor_shadow(x,feet,feet,10)); */
retro_shadow_t retro_actor_shadow(float world_x,float feet_y,float ground_y,float half_width);
/* Appends a shadow; false (nothing drawn) when full or invalid. */
bool retro_frame_add_shadow(retro_frame_t *frame,retro_shadow_t shadow);
/* Appends a light; when full, replaces the weakest one if this is stronger. */
bool retro_frame_add_light(retro_frame_t *frame,retro_light_t light);

/* ---- Short point-light pulses (impacts, explosions); ticks are game ticks. */
typedef struct { retro_vec3_t position,color; float radius,intensity; unsigned start,duration; } retro_pulse_t;
typedef struct { retro_pulse_t pulses[RETRO_MAX_PULSES]; unsigned count; } retro_pulses_t;
/* Intensity decays as (1-t)^2 while the radius opens from 75% to 100%. */
bool retro_pulse_light(const retro_pulse_t *pulse,unsigned tick,retro_light_t *out);
/* Starts a pulse at pulse.start; when full, replaces the weakest one. */
void retro_pulses_spawn(retro_pulses_t *pool,retro_pulse_t pulse,unsigned tick);
/* Drops expired pulses and offers the live ones to the frame's light slots. */
unsigned retro_pulses_emit(retro_pulses_t *pool,unsigned tick,retro_frame_t *frame);

/* ---- Fog density: far sheets thicken slightly while the camera scrolls. */
unsigned retro_fog_alpha(unsigned slice,float focus);
float retro_focus_step(float focus,float camera_dx);

/* ---- Mesh construction, shared by the N64 renderer and host tools. ---- */
typedef struct {
    const uint16_t *ground;      /* height*256 every 8 px; ground_width/8+1 entries */
    unsigned ground_width;
    const retro_plane_t *planes; /* sorted by left edge (build_retro_assets.py) */
    unsigned plane_count;
} retro_scene_t;
typedef struct { unsigned ground,planes,shadows,visible_planes,culled_planes; } retro_mesh_stats_t;
typedef void (*retro_emit_fn)(void *context,const float a[6],const float b[6],const float c[6]);
float retro_ground_height(const retro_scene_t *scene,float x);
bool retro_plane_in_view(const retro_plane_t *plane,float camera_x);
/* Vertices are {screen x,y,r,g,b,a}. Returns the triangle count (0 if culled). */
unsigned retro_shadow_fan(retro_camera_t camera,const retro_shadow_t *shadow,
    float center[6],float rim[RETRO_SHADOW_SEGMENTS+1][6]);
/* Emits at most RETRO_MAX_TRIANGLES triangles; unlit wrappers are skipped. */
retro_mesh_stats_t retro_build_mesh(const retro_frame_t *frame,const retro_scene_t *scene,
    retro_emit_fn emit,void *context);
#endif
