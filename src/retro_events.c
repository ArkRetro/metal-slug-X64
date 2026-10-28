#include "retro_events.h"
#include <string.h>

enum { IMPACT_TICKS=6,DEATH_TICKS=14,EXPLOSION_TICKS=30 };
retro_shadow_t retro_vehicle_shadow(const neo_vehicle_t *v) {
    // The sprite's bottom row (wheels) rests on y=248-v->y; the opaque wheel
    // span is centred on v->x within a pixel for both facings.
    float ground=248.0f-(float)v->y/MSX_UNIT;
    return (retro_shadow_t){(float)v->x/MSX_UNIT,ground,ground,54,.14f};
}
static void impact(retro_events_t *e,const msx_bullet_t *b,int camera,unsigned tick,bool enemy) {
    // Last observed position plus half a step towards where it vanished.
    float x=(float)b->x+b->direction*(enemy?2:3);
    if (x<camera+8 || x>camera+312) return; // Left the screen: not an impact.
    retro_pulse_t p={{x,(float)b->y,-12},enemy?(retro_vec3_t){255,170,96}:(retro_vec3_t){255,214,140},
        enemy?30:36,enemy?.45f:.55f,0,IMPACT_TICKS};
    retro_pulses_spawn(&e->pulses,p,tick);e->impacts++;
}
static bool same_bullet(const msx_bullet_t *before,const msx_bullet_t *now) {
    // A slot reused within one frame starts behind/at a new place: compare motion.
    if (!now->active || before->direction!=now->direction || before->y!=now->y) return false;
    int moved=(now->x-before->x)*before->direction;
    return moved>=0 && moved<=6*8;
}
static void observe_bullets(retro_events_t *e,const msx_bullet_t *before,const msx_bullet_t *now,unsigned n,
    int camera,unsigned tick,bool enemy) {
    for (unsigned i=0;i<n;i++) if (before[i].active && !same_bullet(&before[i],&now[i]))
        impact(e,&before[i],camera,tick,enemy);
}
static void snapshot(retro_events_t *e,const msx_game_t *g) {
    memcpy(e->bullets,g->bullets,sizeof(e->bullets));
    memcpy(e->enemy_bullets,g->enemy_bullets,sizeof(e->enemy_bullets));
    e->ticks=g->ticks;e->deaths=g->deaths;e->vehicle_events=g->vehicle_events;
    e->vehicle_health=g->vehicle.health;e->camera=g->camera;e->primed=true;
}
void retro_events_frame(retro_events_t *e,const msx_game_t *g,retro_frame_t *f) {
    if (!e->primed || g->ticks<e->ticks) {
        // First frame, restart or mission handoff: forget stale pulses.
        e->pulses.count=0;e->focus=0;snapshot(e,g);
    }
    const neo_vehicle_t *v=&g->vehicle;
    if (v->active) retro_frame_add_shadow(f,retro_vehicle_shadow(v));
    if (g->deaths==e->deaths) { // A respawn clears every bullet: no impacts.
        observe_bullets(e,e->bullets,g->bullets,MSX_BULLETS,e->camera,g->ticks,false);
        observe_bullets(e,e->enemy_bullets,g->enemy_bullets,MSX_ENEMY_BULLETS,e->camera,g->ticks,true);
    }
    if (v->active) {
        float x=(float)v->x/MSX_UNIT,ground=248.0f-(float)v->y/MSX_UNIT;
        unsigned fresh=g->vehicle_events&~e->vehicle_events;
        if (e->vehicle_health && !v->health) {
            retro_pulse_t p={{x,ground-30,-18},{255,150,70},96,.65f,0,DEATH_TICKS};
            retro_pulses_spawn(&e->pulses,p,g->ticks);e->explosions++;
        }
        if (fresh&NEO_VEHICLE_EXPLOSION) {
            retro_pulse_t p={{x,ground-26,-22},{255,142,56},150,1.0f,0,EXPLOSION_TICKS};
            retro_pulses_spawn(&e->pulses,p,g->ticks);e->explosions++;
        }
    }
    e->focus=retro_focus_step(e->focus,(float)(g->camera-e->camera));f->focus=e->focus;
    retro_pulses_emit(&e->pulses,g->ticks,f);
    snapshot(e,g);
}
