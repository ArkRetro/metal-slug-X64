#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include "../src/retro25d.h"
#include "../src/retro_events.h"
#ifdef RETRO_REAL_ASSETS
#include "retro_assets.h"
#endif
static void near(float a,float b) {assert(fabsf(a-b)<.0001f);}
typedef struct { unsigned count; float min_alpha,max_alpha; } counter_t;
static void count_triangle(void *context,const float a[6],const float b[6],const float c[6]) {
    counter_t *k=context;k->count++;
    const float *v[3]={a,b,c};
    for (unsigned i=0;i<3;i++) {
        assert(isfinite(v[i][0]) && isfinite(v[i][1]));
        for (unsigned j=2;j<6;j++) assert(v[i][j]>=0 && v[i][j]<=1);
        if (v[i][5]<k->min_alpha) k->min_alpha=v[i][5];
        if (v[i][5]>k->max_alpha) k->max_alpha=v[i][5];
    }
}
static retro_frame_t worst_frame(float camera_x) {
    // Two broad lights in front of every surface, four shadows on screen.
    retro_frame_t f={.camera={camera_x,0,0,320}};
    for (unsigned i=0;i<RETRO_MAX_LIGHTS;i++)
        assert(retro_frame_add_light(&f,(retro_light_t){{camera_x+80+160*i,140,-60},{255,200,120},600,1}));
    for (unsigned i=0;i<RETRO_MAX_SHADOWS;i++)
        assert(retro_frame_add_shadow(&f,retro_actor_shadow(camera_x+40+70*i,184,184,i==3?47:12)));
    return f;
}
static unsigned sweep(const retro_scene_t *scene,float first,float last,unsigned *max_planes) {
    unsigned worst=0;*max_planes=0;
    for (float camera=first;camera<=last;camera+=4) {
        retro_frame_t f=worst_frame(camera);counter_t k={0,1,0};
        retro_mesh_stats_t s=retro_build_mesh(&f,scene,count_triangle,&k);
        assert(k.count==s.ground+s.planes+s.shadows && k.count<=RETRO_MAX_TRIANGLES);
        assert(s.visible_planes<=RETRO_MAX_VISIBLE_PLANES && s.ground<=16 && s.shadows<=RETRO_MAX_SHADOWS*8);
        assert(s.planes<=s.visible_planes*RETRO_PLANE_TRIANGLES);
        if (k.count>worst) worst=k.count;
        if (s.visible_planes>*max_planes) *max_planes=s.visible_planes;
    }
    return worst;
}
static void test_shadows(void) {
    retro_camera_t camera={100,0,0,320};float center[6],rim[RETRO_SHADOW_SEGMENTS+1][6];
    retro_shadow_t s=retro_actor_shadow(260,184,184,10);near(s.radius,11.5f);near(s.squash,.25f);
    assert(retro_shadow_fan(camera,&s,center,rim)==RETRO_SHADOW_SEGMENTS);
    near(center[0],160);near(center[1],185);near(center[5],25/255.0f); // Feathered: dark centre,
    for (unsigned j=0;j<=RETRO_SHADOW_SEGMENTS;j++) near(rim[j][5],0);  // transparent rim.
    near(rim[0][0]-center[0],11.5f);near(rim[2][1]-center[1],11.5f*.25f);
    near(rim[4][0]-center[0],-11.5f);near(rim[0][0],rim[RETRO_SHADOW_SEGMENTS][0]);
    for (unsigned i=2;i<5;i++) near(center[i],rim[3][i]);
    retro_shadow_t jump=s;jump.feet_y=184-48;float c2[6],r2[RETRO_SHADOW_SEGMENTS+1][6];
    assert(retro_shadow_fan(camera,&jump,c2,r2)==RETRO_SHADOW_SEGMENTS);
    assert(c2[5]<center[5] && r2[0][0]-c2[0]<rim[0][0]-center[0]); // Lighter and smaller in the air.
    near(c2[1],center[1]); // Stays on the ground, not under the feet.
    jump.feet_y=184-97;assert(!retro_shadow_fan(camera,&jump,c2,r2));
    retro_shadow_t off=s;off.world_x=camera.x+320+20;assert(!retro_shadow_fan(camera,&off,c2,r2));
    off.world_x=camera.x-20;assert(!retro_shadow_fan(camera,&off,c2,r2));
    retro_shadow_t wide={260,184,184,54,.14f};assert(retro_shadow_fan(camera,&wide,c2,r2));
    near(r2[0][0]-c2[0],54);near(r2[2][1]-c2[1],54*.14f);
    retro_frame_t f={0};
    for (unsigned i=0;i<RETRO_MAX_SHADOWS;i++) assert(retro_frame_add_shadow(&f,s));
    assert(!retro_frame_add_shadow(&f,s) && f.shadow_count==RETRO_MAX_SHADOWS);
    f.shadow_count=0;s.radius=0;assert(!retro_frame_add_shadow(&f,s) && !f.shadow_count);
    near(retro_actor_shadow(0,0,0,1).radius,6);near(retro_actor_shadow(0,0,0,500).radius,64);
}
static void test_lights_and_pulses(void) {
    retro_frame_t f={0};retro_light_t weak={{0,0,-10},{255,255,255},40,.2f},strong=weak;strong.intensity=.9f;
    assert(retro_frame_add_light(&f,weak) && retro_frame_add_light(&f,weak));
    retro_light_t weaker=weak;weaker.intensity=.1f;assert(!retro_frame_add_light(&f,weaker));
    assert(retro_frame_add_light(&f,strong) && f.light_count==2);
    near(f.lights[0].intensity+f.lights[1].intensity,1.1f);
    retro_light_t dark=strong;dark.radius=0;assert(!retro_frame_add_light(&f,dark));
    retro_pulse_t p={{50,150,-12},{255,214,140},36,.55f,100,6};retro_light_t l;
    assert(!retro_pulse_light(&p,99,&l));        // Before start (e.g. after reset): expired.
    assert(retro_pulse_light(&p,100,&l));near(l.intensity,.55f);near(l.radius,27);
    float previous=l.intensity,radius=l.radius;
    for (unsigned t=101;t<106;t++) {
        assert(retro_pulse_light(&p,t,&l));assert(l.intensity<previous && l.radius>radius);
        previous=l.intensity;radius=l.radius;
    }
    assert(!retro_pulse_light(&p,106,&l));p.duration=0;assert(!retro_pulse_light(&p,100,&l));
    // Energy: a point lit by the pulse gets darker with distance and with time.
    p.duration=6;retro_vec3_t n={0,0,1};
    assert(retro_pulse_light(&p,100,&l));float e0=retro_light_energy((retro_vec3_t){50,150,0},n,&l);
    float e_far=retro_light_energy((retro_vec3_t){70,150,0},n,&l);
    assert(retro_pulse_light(&p,103,&l));float e3=retro_light_energy((retro_vec3_t){50,150,0},n,&l);
    assert(e0>e_far && e0>e3 && e3>0);
    retro_pulses_t pool={0};
    for (unsigned i=0;i<RETRO_MAX_PULSES;i++) retro_pulses_spawn(&pool,p,10+i/2);
    assert(pool.count==RETRO_MAX_PULSES);
    retro_pulse_t big={{0,0,-20},{255,142,56},150,1,0,30};retro_pulses_spawn(&pool,big,12);
    bool found=false;for (unsigned i=0;i<pool.count;i++) found|=pool.pulses[i].duration==30 && pool.pulses[i].start==12;
    assert(found && pool.count==RETRO_MAX_PULSES);
    retro_frame_t g={0};assert(retro_pulses_emit(&pool,12,&g)==3 && g.light_count==2 && pool.count==RETRO_MAX_PULSES);
    near(g.lights[0].intensity>g.lights[1].intensity?g.lights[0].intensity:g.lights[1].intensity,1); // Explosion wins a slot.
    retro_frame_t h={0};retro_pulses_emit(&pool,41,&h);assert(pool.count==1 && h.light_count==1);
    retro_frame_t k={0};retro_pulses_emit(&pool,42,&k);assert(!pool.count && !k.light_count);
}
static void test_fog_focus(void) {
    // At rest the fog is exactly the previous density; the foreground sheet never changes.
    assert(retro_fog_alpha(0,0)==18 && retro_fog_alpha(1,0)==18 && retro_fog_alpha(2,0)==12);
    assert(retro_fog_alpha(0,1)==22 && retro_fog_alpha(1,1)==20 && retro_fog_alpha(2,1)==12);
    assert(retro_fog_alpha(0,7)==22 && retro_fog_alpha(0,-3)==18);
    float focus=0;for (unsigned i=0;i<60;i++) {float next=retro_focus_step(focus,2);assert(next>=focus && next<=1);focus=next;}
    assert(focus>.99f);for (unsigned i=0;i<200;i++) focus=retro_focus_step(focus,0);assert(focus<.01f);
}
static void test_culling(void) {
    retro_plane_t p={600,100,700,180,4,0,0};
    assert(retro_plane_in_view(&p,600-320-32) && !retro_plane_in_view(&p,600-320-33));
    assert(retro_plane_in_view(&p,732) && !retro_plane_in_view(&p,733));
    // A long synthetic level (0..4000): planes anywhere in world X, sorted by left.
    static uint16_t ground[4000/8+1];for (unsigned i=0;i<=4000/8;i++) ground[i]=(uint16_t)((184-(i%16))*256);
    static retro_plane_t planes[40];
    for (unsigned i=0;i<40;i++) planes[i]=(retro_plane_t){i*100.0f,80,i*100.0f+90,178,3,.02f,.005f};
    retro_scene_t scene={ground,4000,planes,40};
    assert(retro_ground_height(&scene,-1)==192 && retro_ground_height(&scene,4000)==192);
    near(retro_ground_height(&scene,4),183.5f);
    unsigned visible,worst=sweep(&scene,0,3680,&visible);
    assert(worst<=RETRO_MAX_TRIANGLES && visible==RETRO_MAX_VISIBLE_PLANES); // Dense level hits the plane cap, not the triangle cap.
    retro_frame_t f=worst_frame(2000);counter_t k={0,1,0};
    retro_mesh_stats_t s=retro_build_mesh(&f,&scene,count_triangle,&k);
    assert(s.culled_planes>0 && s.culled_planes<40); // Far planes are skipped by the sorted early exit.
    f.light_count=0;k=(counter_t){0,1,0};s=retro_build_mesh(&f,&scene,count_triangle,&k);
    assert(!s.ground && !s.planes && s.shadows==RETRO_MAX_SHADOWS*8); // Unlit: only shadows.
    printf("measured synthetic 4000-px level: worst %u triangles, %u planes in view\n",worst,visible);
}
static void test_events(void) {
    msx_game_t g;memset(&g,0,sizeof(g));g.ticks=10;g.camera=100;
    retro_events_t e;memset(&e,0,sizeof(e));retro_frame_t f={.camera={100,0,0,320}};
    retro_events_frame(&e,&g,&f);assert(!f.light_count && !f.shadow_count && e.primed);
    g.bullets[0]=(msx_bullet_t){250,170,1,true};g.ticks=11;f=(retro_frame_t){.camera={100,0,0,320}};
    retro_events_frame(&e,&g,&f);assert(!f.light_count);
    g.bullets[0].x=256;g.ticks=12;retro_events_frame(&e,&g,&f);assert(!f.light_count); // Still flying.
    msx_game_t before=g;g.bullets[0].active=false;g.ticks=13;f=(retro_frame_t){.camera={100,0,0,320}};
    msx_game_t copy=g;retro_events_frame(&e,&g,&f);
    assert(!memcmp(&copy,&g,sizeof(g))); // Read-only observer.
    assert(f.light_count==1 && e.impacts==1);near(f.lights[0].position.x,259);near(f.lights[0].position.y,170);
    (void)before;
    // A bullet leaving the screen is not an impact.
    g.bullets[1]=(msx_bullet_t){415,170,1,true};g.ticks=14;retro_events_frame(&e,&g,&f);
    g.bullets[1].active=false;g.ticks=15;retro_events_frame(&e,&g,&f);assert(e.impacts==1);
    // Slot reused in one frame: the old bullet still produced an impact.
    g.bullets[2]=(msx_bullet_t){300,170,1,true};g.ticks=16;retro_events_frame(&e,&g,&f);
    g.bullets[2]=(msx_bullet_t){180,170,1,true};g.ticks=17;retro_events_frame(&e,&g,&f);assert(e.impacts==2);
    // Enemy bullets too, dimmer.
    g.enemy_bullets[0]=(msx_bullet_t){200,168,-1,true};g.ticks=18;retro_events_frame(&e,&g,&f);
    g.enemy_bullets[0].active=false;g.ticks=19;f=(retro_frame_t){.camera={100,0,0,320}};
    retro_events_frame(&e,&g,&f);assert(e.impacts==3 && f.light_count>=1);
    // Respawn: bullets cleared together with a death are not impacts.
    g.enemy_bullets[0]=(msx_bullet_t){200,168,-1,true};g.ticks=20;retro_events_frame(&e,&g,&f);
    memset(g.enemy_bullets,0,sizeof(g.enemy_bullets));g.deaths=1;g.ticks=21;retro_events_frame(&e,&g,&f);assert(e.impacts==3);
    // Vehicle: contact shadow under the wheels, destruction and explosion pulses.
    // Same placement as msx_game_vehicle_reference(), without the generated animation tables.
    g.vehicle=(neo_vehicle_t){.x=937*MSX_UNIT+MSX_UNIT/2,.y=64*MSX_UNIT,.health=300,.max_health=300,.facing=1,.active=true};
    g.camera=872;g.ticks=30;f=(retro_frame_t){.camera={872,0,0,320}};retro_events_frame(&e,&g,&f);
    assert(f.shadow_count==1);near(f.shadows[0].ground_y,184);near(f.shadows[0].world_x,937.5f);
    float c[6],r[RETRO_SHADOW_SEGMENTS+1][6];assert(retro_shadow_fan(f.camera,&f.shadows[0],c,r));
    assert(r[4][0]<=937.5f-872-47 && r[0][0]>=937.5f-872+47); // Covers the 94-px wheel span.
    g.vehicle.health=0;g.vehicle.state=NEO_VEHICLE_DYING;g.ticks=31;f=(retro_frame_t){.camera={872,0,0,320}};
    retro_events_frame(&e,&g,&f);assert(e.explosions==1 && f.light_count==1);
    g.vehicle_events|=NEO_VEHICLE_EXPLOSION;g.ticks=40;f=(retro_frame_t){.camera={872,0,0,320}};
    retro_events_frame(&e,&g,&f);assert(e.explosions==2);
    float peak=0;for (unsigned i=0;i<f.light_count;i++) if (f.lights[i].intensity>peak) peak=f.lights[i].intensity;near(peak,1);
    g.ticks=55;f=(retro_frame_t){.camera={872,0,0,320}};retro_events_frame(&e,&g,&f);
    assert(f.light_count==1 && f.lights[0].intensity<.3f && f.lights[0].intensity>0);
    g.ticks=70;f=(retro_frame_t){.camera={872,0,0,320}};retro_events_frame(&e,&g,&f);assert(!f.light_count);
    assert(e.explosions==2); // Flags persist in vehicle_events; no repeated explosion.
    // Restart (ticks go back): pulses are forgotten.
    g.vehicle_events=0;g.vehicle.health=300;g.ticks=50;g.bullets[3]=(msx_bullet_t){300,170,1,true};
    retro_events_frame(&e,&g,&f);g.ticks=5;g.bullets[3].active=false;f=(retro_frame_t){.camera={872,0,0,320}};
    retro_events_frame(&e,&g,&f);assert(!f.light_count && !e.pulses.count);
    // Camera scrolling raises the fog focus, which relaxes when it stops.
    for (unsigned i=0;i<30;i++) {g.camera+=2;g.ticks++;retro_events_frame(&e,&g,&f);}
    assert(f.focus>.9f);for (unsigned i=0;i<200;i++) {g.ticks++;retro_events_frame(&e,&g,&f);}assert(f.focus<.01f);
}
int main(void) {
    retro_camera_t camera={64,0,0,320};retro_projected_t q;
    const float depths[]={0,320.0f/3,320},ratios[]={1,.75,.5};
    for (unsigned i=0;i<3;i++) {
        retro_vec3_t p=retro_billboard_point(100,80,depths[i],320);
        assert(retro_project(camera,p,&q));near(q.x,100-64*ratios[i]);near(q.y,80);
        retro_plane_view_t plane;assert(retro_plane_projection(camera,depths[i],&plane));
        near(100*plane.scale+plane.offset_x,q.x);near(80*plane.scale+plane.offset_y,q.y);
    }
    assert(!retro_project(camera,(retro_vec3_t){0,0,-313},&q));
    retro_vec3_t p={200,100,0};assert(retro_project(camera,p,&q));float before=q.x;
    camera.z=32;assert(retro_project(camera,p,&q));assert(q.x<before); // Forward camera motion changes perspective.
    retro_vec3_t n=retro_normal(.25f,-.125f);
    near(n.x-.25f*n.z,0);near(n.y+.125f*n.z,0);near(n.x*n.x+n.y*n.y+n.z*n.z,1);
    retro_light_t l={{0,0,-16},{255,160,64},64,1};
    float close=retro_light_energy((retro_vec3_t){0,0,0},(retro_vec3_t){0,0,1},&l);
    float far=retro_light_energy((retro_vec3_t){30,0,0},(retro_vec3_t){0,0,1},&l);
    assert(close>far && far>0);
    l.position.z=16;near(retro_light_energy((retro_vec3_t){0,0,0},(retro_vec3_t){0,0,1},&l),0);
    l.radius=0;near(retro_light_energy(p,n,&l),0);
    retro_frame_t f={0};f.light_count=2;f.lights[0]=(retro_light_t){{0,0,-16},{255,160,64},64,1};f.lights[1]=f.lights[0];
    float rgba[4];retro_lighting(&f,(retro_vec3_t){0,0,0},n,rgba);
    for (unsigned i=0;i<4;i++) assert(rgba[i]>=0 && rgba[i]<=1);
    f.light_count=0;retro_lighting(&f,p,n,rgba);near(rgba[3],0); // Preserve baked art; skip unlit overlays.
    assert(retro_mesh_budget(RETRO_GROUND_SEGMENTS,4,2)==64 && retro_mesh_budget(RETRO_GROUND_SEGMENTS,4,2)<=RETRO_MAX_TRIANGLES);
    assert(retro_mesh_budget(RETRO_GROUND_SEGMENTS,20,2)>RETRO_MAX_TRIANGLES);
    unsigned configured=retro_mesh_budget(RETRO_GROUND_SEGMENTS,RETRO_MAX_VISIBLE_PLANES,RETRO_MAX_SHADOWS);
    assert(configured==88 && configured<=RETRO_MAX_TRIANGLES);
    test_shadows();test_lights_and_pulses();test_fog_focus();test_culling();test_events();
#ifdef RETRO_REAL_ASSETS
    {
        assert(sizeof(retro_fog)==RETRO_FOG_BYTES && retro_layer_depths[3]==0 && RETRO_PLANE_COUNT<=sizeof(retro_planes)/sizeof(*retro_planes));
        for (unsigned i=1;i<RETRO_PLANE_COUNT;i++) assert(retro_planes[i-1].left<=retro_planes[i].left);
        retro_scene_t scene={retro_ground,RETRO_GROUND_WIDTH,retro_planes,RETRO_PLANE_COUNT};
        unsigned visible,worst=sweep(&scene,0,RETRO_GROUND_WIDTH-320,&visible);
        assert(visible<=RETRO_MAX_PLANES_IN_VIEW && worst<=retro_mesh_budget(RETRO_GROUND_SEGMENTS,RETRO_MAX_PLANES_IN_VIEW,RETRO_MAX_SHADOWS));
        printf("measured authored level (%u px, %u planes): worst %u/%u triangles, %u planes in view\n",
            RETRO_GROUND_WIDTH,RETRO_PLANE_COUNT,worst,RETRO_MAX_TRIANGLES,visible);
    }
#endif
    puts("PASS: perspective/parallax, near plane, depth normals, light falloff/backfaces, normalized RDP shades and 96-triangle budget");
    puts("PASS: contact-shadow fans, light slots, pulse time decay, impact/explosion events, fog focus, plane culling and measured budgets");
}
