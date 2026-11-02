#include "../src/msx_stick.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../src/gameplay.h"
#include "../src/msx_sfx.h"
#include "../src/mission_camera.h"
#include "../src/neo_collision.h"
#include "../src/scene_cull.h"
#include "../src/neo_soldier_ai.h"
#include "../src/neo_region.h"
#include "scene_meta.h"
/* Every native draw must find its descriptor and logical palette in the
 * scene pack (src/actor.c and boss_renderer.c log "native draw missing"
 * otherwise; ares traversal of ac75764: the blank pose 1C3F06 of the
 * armoured vehicle's children). Counts the misses of this update. */
static unsigned missing_draws,missing_first;
static void check_draws(const msx_game_t *g) {
    for (unsigned i=0;i<g->world.draw_count;i++) {
        const neo_world_draw_t *d=&g->world.draws[i];
        unsigned lo=0,hi=sizeof(scene_native_descriptors)/sizeof(scene_native_descriptors[0]);bool found=false;
        while (lo<hi) {unsigned mid=(lo+hi)/2;if (scene_native_descriptors[mid].tag==d->descriptor) {found=true;break;} if (scene_native_descriptors[mid].tag<d->descriptor) lo=mid+1; else hi=mid;}
        bool pal=d->kind!=NEO_WORLD_BOSS;
        for (unsigned k=0;!pal && k<sizeof(scene_native_palettes)/sizeof(scene_native_palettes[0]);k++) pal=scene_native_palettes[k].resource==d->palette;
        if (!found || !pal) {if (!missing_draws++) missing_first=d->descriptor;}
    }
}
/* Armoured vehicle 087E36 still standing (any routine of the family, +5F
 * bit 7 clear) on screen: its world X, else -1. */
static int armored_x(const msx_game_t *g) {
    for (unsigned i=0;i<NEO_WORLD_GACTORS;i++) {
        const neo_gactor_t *a=&g->world.gactors[i];
        uint32_t r=ga_l(a,0);
        if (!a->active || r<0x87e36u || r>0x88b02u || (ga_b(a,0x5f)&0x80u)) continue;
        /* Its body reaches 40 px left of +3C: at the right edge (x up to
         * 352) it is in view. */
        int x=(int16_t)ga_w(a,0x3c);
        if (x>=0 && x<352) return g->camera+x;
    }
    return -1;
}
/* Release rules (no invulnerability, deaths respawn in view): a seeded
 * "human" that walks mostly right, turns, idles, jumps and aims up at
 * random, fires all the time and aims up under the domes. Every camera
 * stop must resolve: the script reaches its end and the boss dies. No
 * node stops (the boss turret's gun fires natively since 08FA1A; before the
 * slash callback 039FA6 was ported, the slash 039B60 stopped at its
 * program's command 10 and left its swordsman waiting forever in 03975E).
 * A run depends only on its seed: `gameplay release N` runs seed N alone and
 * must print the line the whole test prints for it (make check). */
static void release_run(unsigned seed) {
    static msx_game_t g;unsigned steps;
    unsigned rs=seed,dir_r=1;bool up=false;msx_game_init(&g);
    unsigned hold=0,hold_max=0,arena_lives=0;
    for (steps=0;steps<80000 && !g.complete;steps++) {
        /* Camera stop 1304: the script waits at 2D67CA for the armoured
         * vehicle 087E36 (group 1) and the prone soldier riding it. */
        if (g.world.script.base+g.world.script.pc==0x2d67cau) {if (++hold>hold_max) hold_max=hold;} else hold=0;
        if (steps%30==0) {rs=rs*1103515245u+12345u;unsigned r=(rs>>16)%10;dir_r=r<6?1:r<8?0:2;rs=rs*1103515245u+12345u;up=(rs>>16)%3==0;}
        int dir=dir_r==2?-1:(int)dir_r;bool a=up;
        int dome=-1;
        for (unsigned i=0;i<NEO_WORLD_GACTORS;i++) {
            const neo_gactor_t *d=&g.world.gactors[i];
            if (!d->active || ga_l(d,0x50)!=0x54730u || (ga_b(d,0x5f)&0x80u)) continue;
            int x=g.camera+(int16_t)ga_w(d,0x3c);if (dome<0 || x<dome) dome=x;
        }
        if (dome>=0) {int px=g.x/MSX_UNIT;dir=px<dome-2?1:px>dome+2?-1:0;a=true;}
        // Armoured vehicle on screen, player on the floor (Y=64; a respawn
        // can land on the ledge at x~1435 or on its platform): walk at it
        // firing level, as at the domes; its solid body (004ED2) stops the
        // walk.
        int armored=g.y>=-20*MSX_UNIT?armored_x(&g):-1;
        if (armored>=0) {dir=g.x/MSX_UNIT<armored?1:-1;a=false;}
        g.aim_up=a;rs=rs*1103515245u+12345u;
        /* Now and then a bomb; at the armoured vehicle every 40. */
        g.grenade=(rs>>8)%97==0 || (armored>=0 && steps%40<2);
        bool was_entering=g.life.phase==MARCO_ENTERING;
        unsigned progress_before=neo_camera_progress(&g.world.camera);
        /* Out of lives: the player takes the continue (0C98CC, Start). */
        g.start_edge=g.awaiting_continue;
        msx_game_update(&g,dir,(rs>>16)%50==0,steps%8<4);  /* fire tapped (H bursts per press) */
        /* A new life in the boss arena (progress 0x0D30: record 2D5C52,
         * screen x 56, from y 0x50 down to the floor) stands on the arena
         * floor, never on the boss. */
        /* The record is chosen by the progress when the new life starts
         * (0196E8, before this update's camera step): a death while the
         * camera reaches 0x0D30 (progress 0x0D2F) takes the previous record. */
        if (!was_entering && g.life.phase==MARCO_ENTERING && progress_before>=0xd30) {
            arena_lives++;
            uint16_t px,py;msx_game_projected(&g,&px,&py);
            /* px is read after this update's camera step (the record used
             * the root before it): 56..58 while the camera still scrolls
             * into the arena (the captures' deaths were all at 3520). */
            assert(g.grounded && 56*MSX_UNIT-g.y==64*MSX_UNIT && px>=56 && px<=58);
        }check_draws(&g);
        g.world.sound_count=0;
    }
    printf("release run %u: continues=%u steps=%u deaths=%u kills=%u unsupported=%u (%05lx) boss=%u items served=%u stopped=%u weapon shots=%u (native %u) grenades=%u expiries=%u score=%02x%02x%02x%02x popups=%u hold 2D67CA=%u arena lives=%u\n",seed,g.continues,steps,g.deaths,g.kills,g.world.unsupported,(unsigned long)g.world.unsupported_routine,g.world.boss_unsupported,
        g.world.pickup.taken,g.world.item_pickups,g.weapon_shots,g.world.player_shots,g.grenades,g.expiries,
        g.world.pickup.score[0][0],g.world.pickup.score[0][1],g.world.pickup.score[0][2],g.world.pickup.score[0][3],g.world.pickup.popups,hold_max,arena_lives);fflush(stdout);
    assert(!g.world.item_pickups && g.grenades>0 && !g.grenade_stops);
    assert(g.complete && g.world.script.finished && !g.world.script.halted && g.deaths>0);
    {   /* Mission end 018846: 018870 (+7C 0x1E, subq/bne), the prisoner
         * tally 0308C0 (0188B0 waits for its +60), 0188F4 (0xA0, subq/bcc)
         * and 018916 (0x20, subq/bcc) entered in the same update as the
         * previous wait ends: 30 + tally + 160 + 32 updates. */
        static msx_game_t e;e=g;unsigned updates=0,tally=0,jingle=0,voice=0;
        uint16_t rescued=g.world.ga_flags.rescued[0];
        uint8_t before[4];memcpy(before,g.world.pickup.score[0],4);
        while (e.mission_end!=MSX_END_DONE && updates<4000) {
            bool started=e.end_started;e.world.sound_count=0;msx_game_update(&e,0,false,false);
            if (started && !(e.ticks&1)) {updates++;if (e.mission_end==MSX_END_TALLY) tally++;}
            /* 0188B8: 0x0020 then 0x1137, once, in the update that creates
             * the letters (the first of the 0xA0 wait). */
            for (unsigned i=0;i<e.world.sound_count;i++) {
                uint16_t w=e.world.sounds[i];
                if (w==0x0020) {assert(!jingle && i+1<e.world.sound_count && e.world.sounds[i+1]==0x1137);jingle=updates;}
                if (w==0x1137) {assert(!voice && e.mission_end==MSX_END_LETTERS && e.end_timer==0x9f);voice=updates;}
            }
        }
        /* The 30th update ends 018870's wait and starts the tally (counted in
         * tally); 0188B0 sees its +60 in the update after the last tally
         * one and goes on to 0188B8 in that update: 30 + tally. */
        assert(jingle && jingle==voice && jingle==30+tally);
        /* The tally ran natively to its end with no explicit stop: every
         * prisoner counted (10C5FC back to 0) and 10000 each (30000 for a
         * special one) plus 100000 for ten or more added to the score. */
        assert(e.mission_end==MSX_END_DONE && !e.end_stops && !e.end.unsupported && !e.end_ram.unsupported_text);
        assert(updates==30+tally+160+32 && tally>0);
        /* 01891E: Mission 2's first stage record 017212 (descriptor 016DDE:
         * map list 0B6D2A, script entry 0x804, stage 1), entry kind 0. */
        assert(e.flow.mission==1 && e.flow.record==0x17212 && e.flow.descriptor==0x16DDE && e.flow.stage==1 && e.flow.load);
        assert(e.flow.routine==0x184B4 && neo_mission_map_list(&e.flow)==0xB6D2A && neo_mission_script_entry(&e.flow)==0x804 && e.flow.cleared[0]==1);
        assert(!e.world.ga_flags.rescued[0]);
        {
            uint32_t bonus=0;
            unsigned special=(e.end_ram.special[0][0]!=0)+(e.end_ram.special[0][1]!=0);
            for (unsigned i=0;i<rescued;i++) bonus+=i<rescued-(rescued<special?rescued:special)?10000u:30000u;
            if (rescued>=10) bonus+=100000u;
            uint8_t want[4];memcpy(want,before,4);
            uint32_t bcd=0;for (unsigned k=0,v=bonus;k<8;k++,v/=10) bcd|=(v%10u)<<(4*k);
            neo_item_add_score(want,bcd);
            assert(!memcmp(want,e.world.pickup.score[0],4));
            printf("release run %u: mission end %u prisoners (%u special), tally %u updates, 0020+1137 at update %u, score %02x%02x%02x%02x -> %02x%02x%02x%02x\n",seed,rescued,special,tally,jingle,
                before[0],before[1],before[2],before[3],e.world.pickup.score[0][0],e.world.pickup.score[0][1],e.world.pickup.score[0][2],e.world.pickup.score[0][3]);
        }
        /* Fade 9C0A (E1,E1,E1, 0x20): -248 per update, -31 at the last step. */
        for (unsigned i=0;i<3;i++) assert(neo_fade_offset(&e.fade,i)==-31 && e.fade.current[i]==(int16_t)0xe100 && e.fade.step[i]==-248);
        assert(!e.fade.count);
        /* The letters 06090A all arrive and leave (17 nodes, the root sets
         * the mission actor's +60) after the tally's nodes. */
        assert(e.end.mission60==1 && !e.end.unsupported);
        /* B24: Mission 2's boss 026254 ends its sinking with 018F40 (026E4A,
         * flags.stage_done) in the last stage. Its next record (017274, type
         * 4 with +4 = 0) is the mission end 018846 (MAME run4's mission flow
         * fixture: 0186A0 -> 018870 at frame 117325): the same sequence as
         * above, then 01891E selects Mission 3 (record 0173F6, stage bits
         * 0F). The stage step used to take it as a stage exit and wait for
         * a stage load that never came. */
        static msx_game_t m;m=e;
        for (unsigned stage=1;stage<=3;stage++) {
            for (unsigned k=0;k<200 && m.flow.state!=NEO_MISSION_PLAYING;k++) neo_mission_update(&m.flow);   /* the wipes 3AED8/3AEF0: 35 updates */
            assert(m.flow.state==NEO_MISSION_PLAYING && m.flow.mission==1 && m.flow.stage==stage);
            assert(neo_mission_ends_after_stage(&m.flow)==(stage==3));
            if (stage<3) {m.flow.end=0;neo_mission_stage_done(&m.flow);}
        }
        m.complete=false;m.end_started=false;m.mission_end=MSX_END_NONE;m.stage_step=0;m.end.count=0;
        m.world.mission_step=0;m.world.ga_flags.stage_done=1;
        unsigned calls=0,end_updates=0;
        while (m.mission_end!=MSX_END_DONE && calls<8000) {
            bool started=m.end_started;msx_game_update(&m,0,false,false);calls++;
            if (started && !(m.ticks&1)) end_updates++;
            assert(!m.stage_step);
        }
        assert(m.complete && m.mission_complete && m.mission_end==MSX_END_DONE && !m.end_stops && !m.end.unsupported);
        assert(m.flow.mission==2 && m.flow.record==0x173F6 && m.flow.load && m.flow.cleared[0]==0x0f);
        printf("release run %u: Mission 2 boss 018F40 -> mission end 018846 in %u updates -> Mission 3 record %06X\n",seed,end_updates,(unsigned)m.flow.record);
    }
    assert(!g.world.unsupported && !g.world.boss_unsupported);   /* no node stops (the turret gun's fire 08FA1A is native) */
    // The random policy (deaths, respawns on the ledge or the vehicle's
    // platform; its body blocks the walk) must get past the 1304 hold
    // (the script end is asserted above). Since the vehicle's shells
    // 088B52 hurt, its length follows the deaths there (506..2900 steps
    // depending on the host build's optimisation level, which changes
    // these runs), so only its lower bound is checked (the grenades thrown
    // at the vehicle shorten it: 150).
    assert(hold_max>=150);
    if (missing_draws) {printf("native draws missing from the scene pack: %u (first descriptor %06x)\n",missing_draws,missing_first);fflush(stdout);}
    assert(!missing_draws);
}
/* Shop 0B6566 (HP 150 from 2D3A5C; the pistol row does 10 per hit, as the
 * MAME bld1 trace's type-6 hits: 150 -> 140 -> 130 ...): the reference
 * policy until the shop's origin is at screen x < `seen`, then the player
 * stays at screen x 160 of that camera firing level (standing or
 * crouching). Returns the steps to its destruction (0B6882), ~0u if never. */
static unsigned shop_from_mid(int seen,bool crouch) {
    static msx_game_t g;msx_game_init(&g);
    int lock=-1;unsigned from=0;
    for (unsigned steps=0;steps<20000;steps++) {
        /* No jumps once the Camel Slug 03458E waits (record at camera
         * 2032): landing on it mounts Marco (0AA610) and the pistol rests. */
        int dir=1;g.aim_up=(steps%64)>=40;bool jump=(steps%240)<4 && g.camera<2032;
        int dome=-1;
        for (unsigned i=0;i<NEO_WORLD_GACTORS;i++) {
            const neo_gactor_t *a=&g.world.gactors[i];
            if (!a->active || ga_l(a,0x50)!=0x54730u || (ga_b(a,0x5f)&0x80u)) continue;
            int x=g.camera+(int16_t)ga_w(a,0x3c);if (dome<0 || x<dome) dome=x;
        }
        if (dome>=0) {int px=g.x/MSX_UNIT;dir=px<dome-2?1:px>dome+2?-1:0;g.aim_up=true;}
        for (unsigned i=0;i<NEO_WORLD_GACTORS;i++) {
            const neo_gactor_t *a=&g.world.gactors[i];
            if (a->active && ga_l(a,0)==0xb6606u && lock<0 && (int16_t)ga_w(a,0x3c)<seen) {lock=g.camera+160;from=steps;}
        }
        if (lock>=0) {int px=g.x/MSX_UNIT;dir=px<lock-4?1:px>lock+4?-1:0;if (!dir) g.facing=1;g.aim_up=false;jump=false;g.crouch=crouch && !dir;}
        msx_game_update(&g,dir,jump,steps%8<4);  /* tapped: one pistol shot per press */g.invulnerable=2;g.world.sound_count=0;
        if (g.world.ga_flags.barrel_patched) return steps-from;
    }
    return ~0u;
}
/* Camera stop 2336, after the shop 0B6566 falls: the player stands still
 * at screen x `at` firing level, turned towards the nearest counted enemy
 * (groups 0..7) on screen with a hurtbox (+50; a human does not see
 * off-screen ones, and the robed swordsman has none), else to the right; no invulnerability from there. Returns the longest wait of the
 * script at 2D6E8A (groups 0..7 empty after the wagon 033C98), or ~0u when
 * the script never passes it. The arcade holds it 197 frames (MAME pri1 and
 * fam2: 7773..7970) and 26 (trk3). */
static unsigned stand_2336(int at) {
    static msx_game_t g;msx_game_init(&g);
    unsigned hold=0,hold_max=0;bool standing=false,passed=false;
    for (unsigned steps=0;steps<30000 && !passed;steps++) {
        uint32_t pc=g.world.script.base+g.world.script.pc;
        if (pc==0x2d6e8au) {if (++hold>hold_max) hold_max=hold;}
        else if (pc>0x2d6e8au && pc<0x2d7132u) passed=true;
        int dir=1;g.aim_up=(steps%64)>=40;bool jump=(steps%240)<4;
        int dome=-1;
        for (unsigned i=0;i<NEO_WORLD_GACTORS;i++) {
            const neo_gactor_t *a=&g.world.gactors[i];
            if (!a->active || ga_l(a,0x50)!=0x54730u || (ga_b(a,0x5f)&0x80u)) continue;
            int x=g.camera+(int16_t)ga_w(a,0x3c);if (dome<0 || x<dome) dome=x;
        }
        if (dome>=0) {int px=g.x/MSX_UNIT;dir=px<dome-2?1:px>dome+2?-1:0;g.aim_up=true;}
        for (unsigned i=0;i<NEO_WORLD_GACTORS;i++) {
            const neo_gactor_t *a=&g.world.gactors[i];
            if (!a->active || ga_l(a,0x00)!=0xb6606u) continue;
            int x=g.camera+(int16_t)ga_w(a,0x3c)-96,px=g.x/MSX_UNIT;
            if (x<g.camera+300) {dir=px<x-4?1:px>x+4?-1:0;if (!dir) g.facing=1;g.aim_up=false;}
        }
        standing|=g.camera>=2336 && g.world.ga_flags.barrel_patched;
        if (standing) {
            int px=g.x/MSX_UNIT,target=g.camera+at;dir=px<target-2?1:px>target+2?-1:0;g.aim_up=false;jump=false;
            if (!dir) {
                int best=-1;
                for (unsigned i=0;i<NEO_WORLD_GACTORS;i++) {
                    const neo_gactor_t *a=&g.world.gactors[i];int sx=(int16_t)ga_w(a,0x3c);
                    if (!a->active || ga_b(a,0x69)>7u || !ga_l(a,0x50) || sx<8 || sx>312) continue;
                    int x=g.camera+sx;if (best<0 || abs(x-px)<abs(best-px)) best=x;
                }
                g.facing=best>=0 && best<px?-1:1;
            }
        }
        msx_game_update(&g,dir,jump,steps%8<4);  /* tapped: one pistol shot per press */
        if (!standing) g.invulnerable=2;
        g.world.sound_count=0;
    }
    return passed?hold_max:~0u;
}
/* The C weapon (id 0C, neo_chaser.c): granted at the start and again
 * whenever it runs out (0C0928, as an item would), the dome policy of the
 * shop run firing tapped. Every shell, flash, trail and blast is native (no
 * stop, every descriptor in the scene pack); shells home in and explode
 * (064128 -> blast 06371E) and kill. */
static void chaser_run_id(uint8_t id) {
    static msx_game_t g;msx_game_init(&g);
    unsigned stops_before=neo_soldier_death_stops;
    unsigned blasts=0,trails=0,grants=0,rockets=0,rolls=0,beams=0,sparks=0;bool seen[NEO_WORLD_GACTORS]={false};
    for (unsigned steps=0;steps<9000;steps++) {
        if (g.world_enabled && marco_weapons_id(&g.world.weapons,0)!=id && g.life.phase==MARCO_ALIVE) {
            uint16_t sound=0;if (marco_weapons_grant(&g.world.weapons,0,id,&g.world.weapon_player,&sound)) grants++;
        }
        int dir=1;g.aim_up=(steps%64)>=40;bool jump=(steps%240)<4;
        int dome=-1;
        for (unsigned i=0;i<NEO_WORLD_GACTORS;i++) {
            const neo_gactor_t *a=&g.world.gactors[i];
            if (!a->active || ga_l(a,0x50)!=0x54730u || (ga_b(a,0x5f)&0x80u)) continue;
            int x=g.camera+(int16_t)ga_w(a,0x3c);if (dome<0 || x<dome) dome=x;
        }
        if (dome>=0) {int px=g.x/MSX_UNIT;dir=px<dome-2?1:px>dome+2?-1:0;g.aim_up=true;}
        msx_game_update(&g,dir,jump,steps%8<4);g.invulnerable=2;g.world.sound_count=0;check_draws(&g);
        for (unsigned i=0;i<NEO_WORLD_GACTORS;i++) {
            const neo_gactor_t *a=&g.world.gactors[i];uint32_t r=ga_l(a,0);
            bool blast=a->active && (r==0x63740u || r==0x63756u || r==0x86450u || r==0x86466u);
            if (a->active && (r==0x86260u || r==0x861e2u)) rolls++;
            if (a->active && (r==0x6381cu || r==0x641ecu)) rockets++;
            if (a->active && (r==0x1e28eu || r==0x1e2fcu || r==0x1e3b2u || r==0x1e41cu)) beams++;
            if (a->active && r==0x1e604u) sparks++;
            if (blast && !seen[i]) blasts++;
            seen[i]=blast;
            if (a->active && r==0x63e48u && ga_b(a,0x18)==0) trails++;
        }
    }
    printf("%s run: grants=%u shells=%u rockets=%u rolls=%u beam calls=%u beams=%u beam updates=%u spark updates=%u (native requests %u, dropped %u) blasts=%u trail updates=%u kills=%u unsupported=%u (%05lx) camera=%d missing draws=%u\n",
        id==3?"R":id==8?"R8":id==0x0d?"0D":id==2?"02":id==7?"07":id==4?"04":id==9?"09":id==6?"06":id==0x0b?"0B":"C",grants,g.c_shots,rockets,rolls,g.laser_calls,g.world.lasers,beams,sparks,g.world.player_shots,g.world.shot_dropped,blasts,trails,g.kills,g.world.unsupported,(unsigned long)g.world.unsupported_routine,g.camera,missing_draws);fflush(stdout);
    if (id==6 || id==0x0b) assert(g.laser_calls>=40 && g.world.lasers>0 && beams>0 && sparks>0 && grants>=1 && g.kills>0);
    else assert(g.c_shots>=40 && grants>=1 && g.kills>0);
    if (id==3 || id==8 || id==0x0c) assert(blasts>0 && trails>0);
    if (id==0x0d) assert(blasts>0 && rolls>0);
    /* Weapons 02/07 (rows of kind 0A/0B) and 04/09 (kind 7): a soldier
     * killed by them takes the death entries 040AF6/040A7C/040A40 of
     * 0406EA/040750/0407F4 (native): no death stop, nothing else stops. */
    if (id==2 || id==7 || id==4 || id==9) printf("  soldier death stops (kinds %08lx): %u\n",(unsigned long)neo_soldier_death_stop_kinds,neo_soldier_death_stops-stops_before);
    assert(neo_soldier_death_stops==stops_before && !g.world.unsupported);
    assert(!missing_draws && !g.weapon_stops);
    if (id==3 || id==8) assert(rockets>=40);
    assert(msx_game_c_programs(&g)==(marco_weapons_id(&g.world.weapons,0)==id));
}
/* The C (0C) and the R (3, rocket 06381C with the C's fire programs and
 * flash; MiSTer report: no rockets drawn). */
/* Weapon 0D (the ball 08606A, neo_pweapon.c: rolls, bursts and blasts)
 * fired with the same programs and flash. */
/* Weapon 8 (item kind 51, the prisoner of 2D70C4 at the camera 3224 before
 * the boss): the rocket 0641EC, bigger, with the R's fire programs. */
static void chaser_run(void) {chaser_run_id(0x0c);chaser_run_id(3);chaser_run_id(8);chaser_run_id(0x0d);}
/* Weapons 02 / 07 (the blasts 0644C4 / 0646B0) and 04 / 09 (the flames
 * 0A71F8 / 0A716E, neo_pweapon.c) with their own fire programs: native,
 * no stop, every descriptor in the scene pack, and they kill. */
/* Weapons 06 / 0B: the beam 01E22C (neo_pweapon.c) made by the steps of
 * its fire programs (0C46D6), its sparks. */
/* B25: weapons 02/07 fired facing left. 0C4434 (01AC94) copies the
 * player's +15 & 3 to the blast 0644C4/0646B0: MAME s2/s7 (the sgun2/sgun7
 * families, 23 blasts each) have +15 = 01 for each of the 12 shots facing
 * left (+6E = 80 level, 40 up, C0 down) and 00 facing right (00, 40, C0).
 * The port made every blast with +15 = 0: facing left it was drawn and hit
 * (row E1412, mirrored by +15 bit 0) towards the right. */
static void sgun_left_run(uint8_t id) {
    static msx_game_t g;msx_game_init(&g);
    unsigned checked[2][3]={{0}},bad=0;bool seen[NEO_WORLD_GACTORS]={false};
    for (unsigned steps=0;steps<4000;steps++) {
        if (g.world_enabled && marco_weapons_id(&g.world.weapons,0)!=id && g.life.phase==MARCO_ALIVE) {
            uint16_t sound=0;marco_weapons_grant(&g.world.weapons,0,id,&g.world.weapon_player,&sound);
        }
        /* Walk right, then turn round every 400 steps; aim up a quarter of
         * the time, jump with Down held a quarter of the time. */
        bool leftward=(steps/400)%2==1;unsigned u=steps%400;
        int dir=u<8?(leftward?-1:1):0;g.aim_up=u>=100 && u<200;
        bool jump=u>=300 && (u%40)<4;g.crouch=u>=300 && (u%40)>=8;
        msx_game_update(&g,dir,jump,steps%8<4);g.invulnerable=2;g.world.sound_count=0;
        for (unsigned i=0;i<NEO_WORLD_GACTORS;i++) {
            const neo_gactor_t *a=&g.world.gactors[i];uint32_t r=ga_l(a,0);
            bool blast=a->active && (r==0x64556u || r==0x645d2u || r==0x645e0u);
            if (blast && !seen[i]) {
                bool left=g.facing<0;uint8_t f15=ga_b(a,0x15)&1u,angle=ga_b(a,0x6e);
                /* 064556 toggles +15 bit 1 for the down row; bit 0 stays. */
                unsigned aim=angle==0x40?1:angle==0xc0?2:0;
                bool good=f15==(left?1u:0u) && (aim || angle==(left?0x80u:0u));
                checked[left][aim]++;if (!good) bad++;
            }
            seen[i]=blast;
        }
    }
    printf("%02X left/right: level %u/%u, up %u/%u, down %u/%u blasts checked, %u with the wrong facing\n",id,
        checked[1][0],checked[0][0],checked[1][1],checked[0][1],checked[1][2],checked[0][2],bad);fflush(stdout);
    assert(!bad && checked[1][0] && checked[0][0] && checked[1][1] && checked[0][1] && checked[1][2] && !g.world.unsupported);
}
/* B26: the sprite windows and layers of each stage in SCB slot order
 * (msx_draw_order, from MAME run4's RAM 10B954/10B95C and the layer
 * objects' +2C) and player 1's +1E (0x8834 - +40 on the ground: run4
 * 19860, +40 = 312 -> 86FC; 26040, 362 -> 86CA). Mission 1 and Mission 2
 * stage 3 keep the port's order (layers 0..2, bit-2 sprites, 3..4, the
 * others); stage 1 draws the bottom strip 10E83E (layer 0) over window A
 * and the exit marker 0B6F6A (+15 bit 2) over it; stage 2 the layer
 * 10D7B6 (1/2) over window A and the gate's frame 03A82E over it. */
static void draw_order_run(void) {
    msx_draw_order_t o=msx_draw_order(0,0);assert(o.known && o.back==0x07 && o.front==0x18 && !o.normal_mid);
    o=msx_draw_order(1,1);assert(o.known && o.back==0x1e && o.front==0x01 && o.normal_mid);
    o=msx_draw_order(1,2);assert(o.known && o.back==0x18 && o.front==0x06 && o.normal_mid);
    o=msx_draw_order(1,3);assert(o.known && o.back==0x01 && o.front==0x18 && !o.normal_mid);
    o=msx_draw_order(2,4);assert(!o.known && o.back==0x07 && o.front==0x18 && !o.normal_mid);
    static msx_game_t g;msx_game_init(&g);
    unsigned grounded=0,air=0;uint16_t last=0xffff;
    for (unsigned steps=0;steps<3000;steps++) {
        msx_game_update(&g,1,(steps%200)<4,steps%8<4);g.invulnerable=2;
        if (!g.world_enabled || (g.ticks&1)) continue;
        uint16_t px,py;msx_game_projected(&g,&px,&py);
        if (g.life.phase==MARCO_ALIVE && !g.descent && g.grounded) {assert(msx_game_depth(&g)==(uint16_t)(0x8834u-py));grounded++;}
        else if (g.life.phase==MARCO_ALIVE && !g.descent && last<0xe000u) {if (msx_game_depth(&g)!=last) fprintf(stderr,"step %u depth %04x last %04x py %u grounded %d\n",steps,msx_game_depth(&g),last,py,g.grounded);assert(msx_game_depth(&g)==last);air++;}
        last=msx_game_depth(&g);
    }
    printf("draw order: Mission 2 stage 1 back %02x front %02x, stage 2 back %02x front %02x; player +1E on the ground %u updates, kept in the air %u\n",
        msx_draw_order(1,1).back,msx_draw_order(1,1).front,msx_draw_order(1,2).back,msx_draw_order(1,2).front,grounded,air);fflush(stdout);
    assert(grounded>100 && air>10);
}
static void pweapon_run(void) {sgun_left_run(2);sgun_left_run(7);chaser_run_id(2);chaser_run_id(7);chaser_run_id(4);chaser_run_id(9);chaser_run_id(6);chaser_run_id(0x0b);}
/* The knife (MiSTer report: enemies could not be knifed): walk right with
 * the pistol tapping fire; with a rebel in reach (0C3744) a press cuts
 * (0C3B46/0C419E) instead of firing, and the cuts kill. */
static void knife_run(void) {
    static msx_game_t g;msx_game_init(&g);unsigned kills_at_cut=0,last_cuts=0;
    for (unsigned steps=0;steps<6000 && g.camera<1200;steps++) {
        msx_game_update(&g,(steps%64)<48?1:0,false,steps%6<3);g.invulnerable=2;g.world.sound_count=0;check_draws(&g);
        if (g.knife_cuts!=last_cuts) {last_cuts=g.knife_cuts;kills_at_cut=g.kills;}
    }
    printf("knife run: cuts=%u hits=%u kills=%u shots=%u camera=%d unsupported=%u\n",g.knife_cuts,g.knife_hits,g.kills,g.shots,g.camera,g.world.unsupported);fflush(stdout);
    assert(g.knife_cuts>0 && g.knife_hits>0 && !g.world.unsupported && !missing_draws);
    (void)kills_at_cut;
}
/* Form 3, the mummy (MiSTer report: a mummy's breath must turn Marco into
 * a mummy). A contact of kind 0x10 (the breath 061E8C) requests +94 = 3 and
 * resets the weapon; the transformation (19 updates, MAME marco-mz1) holds
 * the player; then 0x5A updates of protection; the mummy walks at half
 * speed (0x18000), the grant 0C0928 refuses weapons, and a second breath
 * kills (0C2956). */
static void mummy_run(void) {
    static msx_game_t g;msx_game_init(&g);
    unsigned s=0;
    for (;s<3000 && !(g.grounded && g.life.phase==MARCO_ALIVE && !g.descent && g.ticks>400);s++) {msx_game_update(&g,0,false,false);g.invulnerable=2;}
    assert(g.grounded && g.life.phase==MARCO_ALIVE);
    uint16_t sound=0;marco_weapons_grant(&g.world.weapons,0,0x0a,&g.world.weapon_player,&sound);
    for (unsigned k=0;k<4;k++) msx_game_update(&g,0,false,false);
    g.invulnerable=0;g.world.hit_kind=0x10;msx_game_hurt(&g,0);
    assert(g.life.phase==MARCO_ALIVE && g.mummy_breaths==1 && g.world.weapon_player.form94==3 && marco_weapons_id(&g.world.weapons,0)==0);
    int x0=g.x;
    for (unsigned k=0;k<36;k++) {msx_game_update(&g,1,false,true);g.invulnerable=0;}
    assert(g.x==x0 && g.mummy_transform && g.world.weapon_player.form93==3);   /* held still */
    for (unsigned k=0;k<2;k++) msx_game_update(&g,0,false,false);
    assert(!g.mummy_transform && g.mummy_guard==0x5a);
    g.world.hit_kind=0x10;msx_game_hurt(&g,0);assert(g.life.phase==MARCO_ALIVE);   /* protected */
    assert(!marco_weapons_grant(&g.world.weapons,0,0x0a,&g.world.weapon_player,&sound));
    x0=g.x;
    for (unsigned k=0;k<40;k++) msx_game_update(&g,1,false,false);
    int walked=g.x-x0;
    while (g.mummy_guard) msx_game_update(&g,0,false,false);
    g.invulnerable=0;g.world.hit_kind=0x10;msx_game_hurt(&g,0);
    printf("mummy: breaths=%u walked %d.%04d px in 20 updates (0x18000 each: 30) form=%u phase=%d\n",
        g.mummy_breaths,walked/65536,(walked%65536)*10000/65536,g.world.weapon_player.form93,(int)g.life.phase);fflush(stdout);
    assert(walked>=28*65536 && walked<=30*65536 && g.life.phase!=MARCO_ALIVE);
}
/* Aiming down in the air (+92 = FF): jumps with down held and fire
 * tapped, with the pistol (native 01B782 at C0), then the H and the C (both
 * at C0, the H's fan and its downward sweeps), from the mission start. */
static void air_down_run(void) {
    static msx_game_t g;msx_game_init(&g);
    unsigned h=0,c=0;
    for (unsigned steps=0;steps<3000;steps++) {
        if (steps==1000 || steps==2000) {uint16_t sound=0;marco_weapons_grant(&g.world.weapons,0,steps==1000?0x0a:0x0c,&g.world.weapon_player,&sound);}
        unsigned j=steps%120;g.crouch=j>=12 && j<100;g.aim_up=false;
        msx_game_update(&g,0,j<6,steps%8<4);g.invulnerable=2;g.world.sound_count=0;check_draws(&g);
        if (!g.grounded && g.aim_down && marco_weapons_id(&g.world.weapons,0)==0x0a) h++;
        if (!g.grounded && g.aim_down && marco_weapons_id(&g.world.weapons,0)==0x0c) c++;
    }
    printf("air down: pistol down shots=%u H shots=%u (air updates %u) C shells=%u (air updates %u) unsupported=%u missing draws=%u\n",
        g.down_shots,g.h_shots,h,g.c_shots,c,g.world.unsupported,missing_draws);fflush(stdout);
    assert(g.down_shots>0 && g.h_shots>0 && g.c_shots>0 && !g.world.unsupported && !missing_draws);
}
/* The Camel Slug ridden (MiSTer report: Marco stuck and drawn twice): the
 * C clears the way (granted until the ride), the player jumps on the Camel
 * between the domes and the 2336 stop and rides it at random (bombs, aim,
 * jumps; never A and B together), with `mode` 1 jumping off (down+B) and 2
 * ejecting (A+B) every few seconds. Never drawn on foot while the slot
 * still holds him (the Camel draws its rider 034F1A), no mounted-player
 * stop, every ride ends on foot. */
static void camel_run(int mode) {
    static msx_game_t g;msx_game_init(&g);unsigned rs=1,mounted_at=0,doubled=0;
    for (unsigned s=0;s<9000 && !g.complete;s++) {
        int cx=-1;
        for (unsigned i=0;i<NEO_WORLD_GACTORS;i++) {const neo_gactor_t *a=&g.world.gactors[i];
            if (a->active && ga_l(a,0)>=0x3458eu && ga_l(a,0)<=0x34d58u) {cx=g.camera+(int16_t)ga_w(a,0x3c);break;}}
        int dir=1;bool jump=(s%240)<4,fire=s%8<4;g.aim_up=(s%64)>=40;g.crouch=false;g.grenade=false;
        if (!g.mounts && marco_weapons_id(&g.world.weapons,0)!=0x0c && g.life.phase==MARCO_ALIVE) {uint16_t so=0;marco_weapons_grant(&g.world.weapons,0,0x0c,&g.world.weapon_player,&so);}
        int dome=-1;
        for (unsigned i=0;i<NEO_WORLD_GACTORS;i++) {const neo_gactor_t *a=&g.world.gactors[i];
            if (!a->active || ga_l(a,0x50)!=0x54730u || (ga_b(a,0x5f)&0x80u)) continue;
            int x=g.camera+(int16_t)ga_w(a,0x3c);if (dome<0 || x<dome) dome=x;}
        if (dome>=0) {int px=g.x/MSX_UNIT;dir=px<dome-2?1:px>dome+2?-1:0;g.aim_up=true;}
        if (g.mount==MSX_MOUNT_NONE && cx>=0 && g.camera>=1900 && !g.mounts) {int px=g.x/MSX_UNIT;dir=px<cx-4?1:px>cx+4?-1:0;jump=(s%40)<4;}
        if (g.mount==MSX_MOUNT_RIDE) {
            if (!mounted_at) mounted_at=s;
            unsigned t=s-mounted_at;rs=rs*1103515245u+12345u;
            dir=((rs>>16)%5)?1:-1;fire=(rs>>8)%3==0;g.grenade=(rs>>12)%23==0;g.aim_up=(rs>>24)%4==0;
            jump=(rs>>20)%17==0;if (jump) fire=false;
            if (mode==1 && t%300==299) {g.crouch=true;jump=true;fire=false;}
            if (mode==2 && t%400==399) {jump=true;fire=true;}
        }
        g.invulnerable=2;
        msx_game_update(&g,dir,jump,fire);g.world.sound_count=0;check_draws(&g);
        bool bound=g.world.vslots.slot[0].b[0]|g.world.vslots.slot[0].b[1];
        if (g.mount!=MSX_MOUNT_RIDE && bound && g.life.phase==MARCO_ALIVE) doubled++;
        /* The rider graphic 034F1A/034F96 (a Marco) only with its Camel. */
        bool camel=false,rider=false;
        for (unsigned i=0;i<NEO_WORLD_GACTORS;i++) {const neo_gactor_t *a=&g.world.gactors[i];uint32_t q=ga_l(a,0);
            if (!a->active) continue;
            camel|=q>=0x3458eu && q<=0x34d58u;rider|=q==0x34f1au || q==0x34f96u;}
        if (rider && !camel) doubled++;
    }
    printf("camel run %d: mounts=%u dismounts=%u mounted-player stops=%u drawn on foot while bound=%u mount=%u camera=%d unsupported=%u (%05lx) missing draws=%u (%06x)\n",
        mode,g.mounts,g.dismounts,g.mount_stops,doubled,g.mount,g.camera,g.world.unsupported,(unsigned long)g.world.unsupported_routine,missing_draws,missing_first);fflush(stdout);
    /* No stop: the Camel is pushed out of the rocket launcher's solid body
     * (05ED84 at camera 2688, 004ED2 native). Mode 0 never leaves on its own
     * (the 004ED2 stop used to end its ride: MiSTer report, ROM 10); modes
     * 1 and 2 end every ride on foot. */
    assert(g.mounts>=1 && !g.mount_stops && !doubled && !missing_draws);
    if (mode) assert(g.dismounts>=1 && g.mount==MSX_MOUNT_NONE);
    assert(!g.world.unsupported);
}
/* Walking left from the parachute descent (MiSTer ROM 12: "si caminas hacia
 * atras caes infinito"): hold left from the handoff, after the cut and after
 * the landing; Marco must land and stay on the floor. */
static void fall_run(unsigned cut_at,unsigned left_from,bool print) {
    static msx_game_t g;msx_game_init(&g);
    msx_game_begin_descent(&g,7865136,16293888,816,-483328);
    int deepest=g.y;unsigned landed=0;
    for (unsigned t=0;t<1200;t++) {
        bool cut=cut_at && t==cut_at;
        msx_game_update(&g,t>=left_from?-1:0,cut,false);
        if (g.y>deepest) deepest=g.y;
        if (!landed && !g.descent && g.grounded) landed=t;
        if (print && (t%40==0 || t<4)) printf("t=%u x=%d y=%d cam=%d descent=%u gr=%d\n",t,g.x>>16,g.y>>16,g.camera,g.descent,g.grounded);
    }
    printf("fall cut=%u left=%u: landed %u x=%d y=%d deepest=%d camera=%d\n",cut_at,left_from,landed,g.x>>16,g.y>>16,deepest>>16,g.camera);
    assert(landed && g.grounded && !g.descent && g.y==msx_ground_offset(g.x) && deepest<=64*MSX_UNIT);
    assert(g.x>=(g.camera+28)*MSX_UNIT);
    if (left_from<1200) assert(g.x==(g.camera+28)*MSX_UNIT);   /* MAME: x 28.0, feet on the floor */
}
int main(int argc,char **argv) {
    if (argc>1 && !strcmp(argv[1],"fall")) {fall_run(argc>2?(unsigned)atoi(argv[2]):0,argc>3?(unsigned)atoi(argv[3]):0,argc>4);return 0;}
    if (argc>1 && !strcmp(argv[1],"camel")) {for (int m=0;m<3;m++) camel_run(m);return 0;}
    if (argc>1 && !strcmp(argv[1],"airdown")) {air_down_run();return 0;}
    if (argc>1 && !strcmp(argv[1],"chaser")) {chaser_run();return 0;}
    if (argc>1 && !strcmp(argv[1],"pweapon")) {pweapon_run();return 0;}
    if (argc>1 && !strcmp(argv[1],"knife")) {knife_run();return 0;}
    if (argc>1 && !strcmp(argv[1],"mummy")) {mummy_run();return 0;}
    if (argc>2 && !strcmp(argv[1],"release")) {for (int i=2;i<argc;i++) release_run((unsigned)atoi(argv[i]));return 0;}
    {   /* The stick as an 8-way lever (msx_stick.h): 45-degree sectors. */
        int h,v;
        msx_stick_lever(85,26,&h,&v);assert(h==1 && v==0);     /* 17 degrees: right only (was up-right) */
        msx_stick_lever(60,60,&h,&v);assert(h==1 && v==1);     /* 45: up-right */
        msx_stick_lever(30,80,&h,&v);assert(h==0 && v==1);     /* 69: up only */
        msx_stick_lever(-70,-40,&h,&v);assert(h==-1 && v==-1); /* 30: down-left */
        msx_stick_lever(20,10,&h,&v);assert(h==0 && v==0);     /* dead zone */
    }
    msx_game_t g;msx_game_init(&g);
    int start=g.x;
    msx_game_update(&g,1,true,false);
    assert(g.x==start); // Short input edges survive the half-rate player update.
    msx_game_update(&g,1,false,false);
    assert(!g.grounded && g.x>start && g.velocity_y<0);
    assert(msx_game_animation(&g)==MSX_RUN_JUMP);
    for (unsigned i=0;i<180;i++) msx_game_update(&g,0,false,false);
    assert(g.grounded && g.y==msx_ground_offset(g.x));
    msx_game_init(&g);msx_game_update(&g,1,false,true);
    assert(msx_game_animation(&g)==MSX_RUN_FIRE);
    assert(msx_ground_offset(700*MSX_UNIT)== -8*MSX_UNIT); // Original village floor Y=64.
    assert(msx_ground_offset(g.x)<-37*MSX_UNIT); // Initial dune is above the village.
    msx_game_t roof;msx_game_init(&roof);roof.world_enabled=false;
    roof.x=600*MSX_UNIT;roof.y=msx_ground_offset(roof.x);
    for (unsigned i=0;i<100;i++) msx_game_update(&roof,0,i<2,false);
    assert(roof.grounded && roof.y== -40*MSX_UNIT); // First roof: original world Y=96.
    bool fell=false;
    for (unsigned i=0;i<160;i++) {msx_game_update(&roof,1,false,false);fell|=!roof.grounded;}
    assert(fell && roof.grounded && roof.y== -8*MSX_UNIT); // Walk off to lower ground.
    g.world_enabled=false;
    // Whole mission without the native world: the reference camera follows
    // the player up to the original command list's hold (3520); the player's
    // right limit keeps camera+292-1/65536.
    int highest=0;
    for (unsigned i=0;i<3200;i++) {
        msx_game_update(&g,1,false,false);
        assert(g.camera>=highest && g.camera<=MSX_CAMERA_MAX && g.x<=MSX_END_X);highest=g.camera;
        if (g.x>1200*MSX_UNIT && g.x<2100*MSX_UNIT) assert(g.grounded && g.y==-8*MSX_UNIT); // Walkable Y=64 floor.
    }
    assert(g.camera==MSX_CAMERA_MAX && g.x==(MSX_CAMERA_MAX+292)*MSX_UNIT-1);
    int camera=g.camera;
    for (unsigned i=0;i<100;i++) msx_game_update(&g,-1,false,false);
    assert(g.x<(MSX_CAMERA_MAX+292)*MSX_UNIT-1 && g.camera==camera); // Camera does not scroll backwards.
    for (unsigned i=0;i<1000;i++) msx_game_update(&g,-1,false,false);
    assert(g.x==(g.camera+28)*MSX_UNIT); // Player remains inside the current camera.
    // Whole Mission 1 with the native world and camera (reference run: walk
    // right firing, up-shots every other half second, jump every 4 s; the
    // test keeps the player alive). The script runs to its op 3 end
    // (2D7132) with the region's soldiers, truck, bomber, the robed swordsman
    // 0A8C46 and the boss native; the other rest-of-mission constructors are
    // counted as external. The robed swordsman's gesture toggles RAM 10C606,
    // the boss starts, dies and signals the mission end (018F92, 018F40).
    msx_game_init(&g);
    unsigned steps=0;
    unsigned voices=0,mapped=0,requests=0,pending=0;
    bool region_end_seen=false;int max_height=0;unsigned max_progress=0,hold=0,hold_max=0,weapon_seen=0;
    for (;steps<60000 && !g.complete;steps++) {
        if (g.world.script.base+g.world.script.pc==0x2d67cau) {if (++hold>hold_max) hold_max=hold;} else hold=0;
        int dir=1;
        if (g.world.boss_active) dir=(steps/90)%3==0?-1:(steps/90)%3==1?1:0;
        g.aim_up=(steps%64)>=40;
        // Domed building (054904): stand under the leftmost dome that can be
        // hit (hurtbox 054730) and fire upwards.
        int dome=-1;
        for (unsigned i=0;i<NEO_WORLD_GACTORS;i++) {
            const neo_gactor_t *a=&g.world.gactors[i];
            if (!a->active || ga_l(a,0x50)!=0x54730u || (ga_b(a,0x5f)&0x80u)) continue;
            int x=g.camera+(int16_t)ga_w(a,0x3c);
            if (dome<0 || x<dome) dome=x;
        }
        if (dome>=0) {int px=g.x/MSX_UNIT;dir=px<dome-2?1:px>dome+2?-1:0;g.aim_up=true;}
        // Destructible shop (0B6566): fire at it from 96 px to its left.
        for (unsigned i=0;i<NEO_WORLD_GACTORS;i++) {
            const neo_gactor_t *a=&g.world.gactors[i];
            if (!a->active || ga_l(a,0x00)!=0xb6606u) continue;
            int x=g.camera+(int16_t)ga_w(a,0x3c)-96,px=g.x/MSX_UNIT;
            if (x<g.camera+300) {dir=px<x-4?1:px>x+4?-1:0;if (!dir) g.facing=1;g.aim_up=false;}
        }
        /* Bombs at the armoured vehicle of the 1304 stop, the domes and the
         * shop (every 40 updates while they stand). */
        bool target=armored_x(&g)>=0 || dome>=0;
        for (unsigned i=0;i<NEO_WORLD_GACTORS;i++) if (g.world.gactors[i].active && ga_l(&g.world.gactors[i],0)==0xb6606u) target=true;
        g.grenade=target && (steps%40)<2;
        weapon_seen|=1u<<marco_weapons_id(&g.world.weapons,0);
        msx_game_update(&g,dir,(steps%240)<4,steps%8<4);check_draws(&g);
        g.invulnerable=2;
        region_end_seen|=g.region_end;
        if (g.camera<2336 && g.camera_height>max_height) max_height=g.camera_height;
        if (neo_camera_progress(&g.world.camera)>max_progress) max_progress=neo_camera_progress(&g.world.camera);
        // Sound requests of the native world: every one has a packed render.
        for (unsigned i=0;i<g.world.sound_count;i++) {
            int id=msx_sfx_lookup(g.world.sounds[i]);requests++;
            if (id>=0) {mapped++;voices+=msx_sfx_defs[id].channel==MSX_SFX_CH_VOICE;}
            // Not rendered yet (docs/AUDIO_JUEGO.md): vehicle engine loop 10A5,
            // 10E8 of the generic actors and the boss's own requests.
            else pending++;
        }
        g.world.sound_count=0;
    }
    printf("mission: steps=%u spawned=%u kills=%u ext=%u unsupported=%u (%05lx) boss_unsupported=%u (%05lx) boss_hits=%u boss_effects_ext=%u hits=%u mapped=%u/%u progress=%u height=%d collapsed=%u killed_all=%u shop=%u rides=%u hold 2D67CA=%u\n",
        steps,g.world.spawned,g.kills,g.world.external_spawns,g.world.unsupported,(unsigned long)g.world.unsupported_routine,g.world.boss_unsupported,(unsigned long)g.world.boss_unsupported_label,
        g.world.boss_hits,g.world.boss_external_effects,g.world.player_hits,mapped,requests,max_progress,max_height,g.world.ga_flags.collapsed,g.world.killed_all,g.world.ga_flags.barrel_patched,g.rides,hold_max);
    printf("weapons: items served=%u stopped=%u (%05lx) weapons seen=%04x shots with them=%u native=%u stops=%u (last %05lx) expiries=%u grenades=%u bombs=%u score=%02x%02x%02x%02x\n",
        g.world.pickup.taken,g.world.item_pickups,(unsigned long)g.world.item_pickup_routine,weapon_seen,g.weapon_shots,g.world.player_shots,g.weapon_stops,(unsigned long)g.weapon_stop_constructor,
        g.expiries,g.grenades,marco_weapons_bomb_count(&g.world.weapons,0),g.world.pickup.score[0][0],g.world.pickup.score[0][1],g.world.pickup.score[0][2],g.world.pickup.score[0][3]);
    fflush(stdout);
    /* Every item the policy walks into is served (weapon, bombs, points):
     * no pickup stops. The H (id 0A) is taken and used up, then the pistol
     * comes back (0C337A); the first life's 10 bombs are thrown. */
    assert(g.world.pickup.taken>0 && !g.world.item_pickups);
    assert((weapon_seen&(1u<<0xa)) && g.weapon_shots>0 && g.expiries>0);
    /* The H fires native bullets 01B716 (neo_bullet.c): none is a stop. */
    /* Every H shot is native; the other weapons the prisoners give (their
     * shots are not ported) stop explicitly at their constructor, counted. */
    assert(g.world.player_shots==g.h_shots+g.grenades+2u*g.c_shots+g.down_shots && !g.world.shot_dropped);
    assert(g.weapon_stops==g.weapon_shots-g.h_shots-g.c_shots && g.weapon_stop_constructor!=0x1b6dc && g.weapon_stop_constructor!=0x1b716);
    printf("H bullets that hit: %u, diagonal H bullets (0C453A sweeps): %u, grenade blast rows that hit: %u\n",g.world.pickup.bullet_hits,g.hgun_diagonal,g.world.pickup.blast_hits);fflush(stdout);
    /* Aiming up and back during the bursts sweeps the H (marco_hgun.c). */
    assert(g.hgun_diagonal>0);
    assert(g.world.pickup.bullet_hits>0 && g.world.pickup.blast_hits>0);
    /* Native grenades 01BF46/01BF10 (neo_bullet.c): every one lands. */
    assert(g.grenades>=10 && !g.grenade_stops && !g.world.weapons.b[0]);
    assert(requests && voices && !g.world.sound_dropped);
    if (missing_draws) {printf("native draws missing from the scene pack: %u (first descriptor %06x)\n",missing_draws,missing_first);fflush(stdout);}
    assert(!missing_draws);
    assert(g.shots>=g.hits);
    assert(g.complete && g.mission_complete && g.world.mission_step==2 && steps<60000);
    // The player stands on the boss's dynamic platforms (0140C0 objects,
    // floor search 013DB2) and is carried by them.
    assert(g.rides>0);
    // Camera stop 1304: the arcade holds 2D67CA for 385 frames (MAME,
    // docs/MISION1_ACTORES.md) until the armoured vehicle 087E36 that
    // brings the prone soldier in is destroyed. The pistol-only policy,
    // walking right into it (its solid body 004E76/0E4E90 stops the player
    // through the push-out 004ED2), needed 668 (~2000 while the vehicle was
    // external); with the native grenades thrown at it the stop held 194,
    // and 146 since the region's prisoners and their items are native (the
    // rescues change the walk), under the arcade's 385.
    assert(g.solid_pushes>0 && !g.world.solid_unsupported);
    assert(hold_max>=120 && hold_max<385);   /* 194 with the held-fire pistol; 134 with one shot per press (hold2 semantics) */
    assert(region_end_seen && g.world.script.finished && !g.world.script.halted);
    // The script camera stops at 3376 (boss arena); the player's view then
    // follows the oscillating boss-fight policy, so where it stands when the
    // boss's end sequence completes depends on the fight's timing.
    assert(max_progress==3376 && g.camera>=3376 && g.camera<=MSX_CAMERA_MAX);
    assert(g.world.ga_flags.collapsed && max_height>=30 && max_height<=32 && g.world.ga_flags.barrel_patched); // domed building down: camera rose 15..16 from 16 (31 with one pistol shot per press; 30 once the village porter and awning are native and change the scripted walk) (14 before the region prisoners were native; measured before the wagon stop: standing on the wreck or the boss lifts the view)
    draw_order_run();
    chaser_run();
    pweapon_run();
    knife_run();
    mummy_run();
    air_down_run();
    for (int m=0;m<3;m++) camel_run(m);
    for (unsigned seed=1;seed<=4;seed++) release_run(seed);
    // Camera stop 2336 (user report: Marco still mid-screen firing level,
    // the shop down, the wagon gone, the script waiting at 2D6E8A). The
    // group 6 swordsmen 039590 and the robed swordsman 0A8C62 (group 0, who
    // leaves once group 6 is empty and its sub-script 019E72 finished) end
    // the hold within three times the arcade's 197 frames wherever the
    // player stands, left or right of the ruin's column x = 2576 (cell 0x10,
    // the same in the MAME RAM dumps from 7500 on), when it turns to the
    // enemies it can see.
    // The shop from mid-screen (user report: Marco ~150 px left of it firing
    // level): as soon as it appears (origin at screen 400, camera ~2243) or
    // with the camera at the 2336 stop, standing or crouching, 15 pistol
    // hits break it.
    for (int seen=400;seen>=300;seen-=100) for (int c=0;c<2;c++) {
        unsigned n=shop_from_mid(seen,c);
        printf("shop from screen 160 (seen at %d, crouch %d): destroyed after %u steps\n",seen,c,n);
        assert(n<400);
    }
    for (int at=160;at<=280;at+=60) {
        unsigned h=stand_2336(at);
        printf("2336 stand at x=%d: hold 2D6E8A=%u\n",at,h);
        assert(h<3*197);
    }
    /* One hit kills (0C2956): the death flies away from the attacker
     * (0C2E44: +34 = 0xA9000, +30 = -2 with the attacker on the right),
     * lands, lies 31 updates, and the new life (lab build without the native
     * world: the port's checkpoint) enters invulnerable with no bullets. */
    msx_game_init(&g);g.x=640*MSX_UNIT;g.y=msx_ground_offset(g.x);g.checkpoint=640;
    g.invulnerable=0;g.world_enabled=false;
    g.bullets[0]=(msx_bullet_t){g.x/MSX_UNIT,168+g.y/MSX_UNIT,1,true,false,0};
    msx_game_hurt(&g,g.x/MSX_UNIT+12);
    assert(g.deaths==1 && g.life.phase==MARCO_DYING && g.facing==1 && g.velocity_x==-0x20000 && g.velocity_y==-0xa9000 && !g.grounded);
    unsigned landed=0,updates=0;
    for (;updates<400 && g.life.phase!=MARCO_ENTERING;updates++) {msx_game_update(&g,1,true,true);if (!landed && g.grounded) landed=updates;}
    assert(landed && g.life.phase==MARCO_ENTERING && g.x==640*MSX_UNIT && g.invulnerable && g.camera==496 && g.deaths==1);
    for (unsigned i=0;i<MSX_BULLETS;i++) assert(!g.bullets[i].active);
    for (unsigned i=0;i<40 && g.life.phase==MARCO_ENTERING;i++) msx_game_update(&g,0,false,false);
    assert(g.life.phase==MARCO_ALIVE && g.invulnerable);
    msx_game_init(&g);g.x=200*MSX_UNIT;g.y=msx_ground_offset(g.x);g.camera=56;g.world_enabled=false;
    msx_game_hurt(&g,212);
    for (unsigned i=0;i<100 && g.life.phase==MARCO_DYING;i++) msx_game_update(&g,0,false,false);
    assert(g.life.phase==MARCO_LYING && g.grounded && g.y==msx_ground_offset(g.x) && g.x<200*MSX_UNIT); // Lands, feet on the slope.
    msx_game_init(&g);
    assert(msx_game_begin_at(&g,7852402,6157405));
    assert(g.x==7852402 && g.y==56*MSX_UNIT-6157405 && g.grounded && !g.velocity_x && !g.velocity_y);
    msx_game_t unchanged=g;
    assert(!msx_game_begin_at(&g,7852402,6157406));
    assert(g.x==unchanged.x && g.y==unchanged.y);
    // Parachute descent from the 3860 handoff (marco_descent.c): no input
    // lands after about 100 updates (MAME 200 frames), B cuts and falls sooner.
    {
        unsigned hang=0,cut=0,most=0,letter_ticks=0;
        msx_game_begin_descent(&g,7865136,16293888,816,-483328);g.world_enabled=false;
        assert(g.descent==MSX_DESCENT_HANG && !g.grounded);
        for (;hang<600 && g.descent;hang++) {
            msx_game_update(&g,0,false,false);
            if (g.end.draw_count>most) most=g.end.draw_count;
            if (g.start_letters) letter_ticks=hang+1;
        }
        assert(!g.descent && g.grounded && g.world.player_node[0][0x61]==1 && hang>=190 && hang<=210);
        assert(g.y==msx_ground_offset(g.x));
        // MISSION 1 START 0607DE (neo_end.c): the letters arrive, blink and
        // leave during the descent without a stop (MAME ms2: 71 updates).
        assert(!g.start_letters && !g.start_stops && !g.end.count && most>=13);
        printf("MISSION 1 START: %u draws at most, gone after %u ticks\n",most,letter_ticks);
        msx_game_begin_descent(&g,7865136,16293888,816,-483328);g.world_enabled=false;
        for (unsigned i=0;i<20;i++) msx_game_update(&g,0,false,false);
        msx_game_update(&g,0,true,false);
        for (;cut<600 && g.descent;cut++) msx_game_update(&g,0,false,false);
        assert(g.descent_cuts==1 && g.grounded && g.world.player_node[0][0x61]==2 && cut+21<hang);
        printf("descent: landed after %u ticks, cut at 20 and landed %u ticks later\n",hang,cut);
        // Left held from the handoff, after the cut and after the landing:
        // stopped at camera+28 on the floor, never through it (MiSTer ROM 12).
        fall_run(0,0,false);fall_run(0,300,false);fall_run(20,0,false);fall_run(20,100,false);fall_run(60,0,false);
        msx_game_init(&g);assert(msx_game_begin_at(&g,7852402,6157405));
        for (unsigned t=0;t<400;t++) msx_game_update(&g,-1,false,false);
        assert(g.grounded && g.x==28*MSX_UNIT && g.y==msx_ground_offset(g.x));
        // Every camera stop of the course: left against the edge stays on the floor.
        for (int c=0;c<=MSX_CAMERA_MAX;c+=16) {
            msx_game_init(&g);g.world_enabled=false;g.camera=c;g.x=(c+60)*MSX_UNIT;g.y=msx_ground_offset(g.x);
            if (!g.y && !msx_ground_offset(g.x+MSX_UNIT)) continue;
            for (unsigned t=0;t<120;t++) msx_game_update(&g,-1,false,false);
            if (!(g.grounded && g.x==(c+28)*MSX_UNIT)) printf("left edge camera %d: x=%d y=%d grounded=%d\n",c,g.x>>16,g.y>>16,g.grounded);
            assert(g.y<=64*MSX_UNIT);
        }
    }
    // Extended terrain: floor beyond the old end, and a measured upper roof.
    assert(msx_ground_offset(1192*MSX_UNIT)== -8*MSX_UNIT && msx_ground_offset(2200*MSX_UNIT)== -8*MSX_UNIT);
    msx_game_init(&g);g.world_enabled=false;g.camera=1200;g.x=1350*MSX_UNIT;g.y=56*MSX_UNIT-170*MSX_UNIT;g.grounded=false;
    for (unsigned i=0;i<120 && !g.grounded;i++) msx_game_update(&g,0,false,false);
    assert(g.grounded && g.y==56*MSX_UNIT-144*MSX_UNIT); // Lands on the Y=144 roof, not the floor below.
    msx_game_init(&g);g.world_enabled=false;g.camera=1800-144;g.x=1800*MSX_UNIT;g.y=msx_ground_offset(g.x);
    msx_game_update(&g,0,true,false);
    for (unsigned i=0;i<200 && (g.ticks<4 || !g.grounded);i++) msx_game_update(&g,0,false,false);
    assert(g.grounded && g.y==-8*MSX_UNIT && g.camera==1800-144 && g.air_ticks>0);
    // Vertical camera: scroll-driven rise 944..1008; middle bank switches at 1023.
    assert(msx_camera_height(872)==0 && msx_camera_height(944)==0 && msx_camera_height(947)==1);
    assert(msx_camera_height(965)==6 && msx_camera_height(1008)==16 && msx_camera_height(MSX_CAMERA_MAX)==16);
    assert(msx_layer_visible(1,1022) && !msx_layer_visible(2,1022) && !msx_layer_visible(1,1023) && msx_layer_visible(2,1023));
    // The observed vehicle wreck at X=1174 is complete now: 13 cells, as in the arcade trace.
    neo_collision_reset();
    assert(neo_collision_vehicle_wreckage(1174*MSX_UNIT,64*MSX_UNIT,1)==13);
    assert(neo_collision_cell(1196,80)==10 && neo_collision_cell(1208,64)==10); // Cells beyond the old 1192 limit.
    neo_collision_reset();
    assert(!neo_collision_cell(1196,80) && !neo_collision_cell(1208,64)); // Event-only: never static terrain.
    // A soldier's knife (ROM 0AEF14) reaches the player standing still at
    // the first spawn: the native world reports the hit and the player loses
    // health. Re-measured with the ported soldiers.
    msx_game_init(&g);
    unsigned hit_at=0;
    for (unsigned i=0;i<1200 && !hit_at;i++) {msx_game_update(&g,i<170?1:0,false,false);if (g.world.player_hits) hit_at=i;}
    assert(hit_at && g.deaths==1 && g.life.phase==MARCO_DYING && g.world.player_hits==1);
    puts("PASS: half-rate input, air landing, dune/village support, reference camera to 3520, whole Mission 1 with native camera, script, boss and mission end, extended terrain, knife hit and respawn");
}
