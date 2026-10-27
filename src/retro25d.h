#ifndef MSX_RETRO25D_H
#define MSX_RETRO25D_H
#include <stdbool.h>
enum { RETRO_MAX_LIGHTS=2, RETRO_MAX_TRIANGLES=96, RETRO_FOG_BYTES=1024,
       RETRO_GROUND_SEGMENTS=8, RETRO_GROUND_ROWS=1 };
typedef struct { float x,y,z; } retro_vec3_t;
typedef struct { float x,y,z,focal; } retro_camera_t;
typedef struct { retro_vec3_t position,color; float radius,intensity; } retro_light_t;
typedef struct { float left,top,right,bottom,depth,dx,dy; } retro_plane_t;
typedef struct { float world_x,feet_y,ground_y,radius; } retro_shadow_t;
typedef struct {
    retro_camera_t camera;
    retro_light_t lights[RETRO_MAX_LIGHTS];
    retro_shadow_t shadows[2];
    unsigned light_count,shadow_count,tick;
} retro_frame_t;
typedef struct { float x,y,inverse_w; } retro_projected_t;
typedef struct { float scale,offset_x,offset_y; } retro_plane_view_t;
retro_vec3_t retro_normal(float depth_dx,float depth_dy);
bool retro_project(retro_camera_t camera,retro_vec3_t point,retro_projected_t *out);
retro_vec3_t retro_billboard_point(float asset_x,float asset_y,float depth,float focal);
bool retro_plane_projection(retro_camera_t camera,float depth,retro_plane_view_t *out);
float retro_light_energy(retro_vec3_t point,retro_vec3_t normal,const retro_light_t *light);
void retro_lighting(const retro_frame_t *frame,retro_vec3_t point,retro_vec3_t normal,float rgba[4]);
/* rgba is normalized 0..1, matching libdragon's triangle shade format. */
unsigned retro_mesh_budget(unsigned ground_segments,unsigned planes,unsigned shadows);
#endif
