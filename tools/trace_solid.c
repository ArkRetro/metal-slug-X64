/* Diagnostic (not built by make): the reference policy of tools/test_gameplay.c
 * without vehicle steering, tracing the player and the armoured vehicle 087E36
 * during the 2D67CA hold. Build as build/host/gameplay with this file instead
 * of tools/test_gameplay.c. */
#include <stdio.h>
#include "../src/gameplay.h"
#include "../src/mission_camera.h"
int main(void) {
    msx_game_t g;msx_game_init(&g);
    unsigned hold=0,lasthits=0;
    for (unsigned steps=0;steps<9000 && !g.complete;steps++) {
        bool held=g.world.script.base+g.world.script.pc==0x2d67cau;
        hold=held?hold+1:0;
        int dir=1;
        g.aim_up=(steps%64)>=40;
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
        msx_game_update(&g,dir,(steps%240)<4,true);
        g.invulnerable=2;
        if (held && (hold%20==1 || g.hits!=lasthits)) {
            printf("step=%u hold=%u px=%d py=%d cam=%d face=%d gnd=%d ride=%u pushes=%u hits=%u aim_up=%d |",
                steps,hold,g.x/MSX_UNIT,g.y/MSX_UNIT,g.camera,g.facing,g.grounded,g.ride,g.solid_pushes,g.hits,g.aim_up);
            for (unsigned i=0;i<NEO_WORLD_GACTORS;i++) {
                const neo_gactor_t *a=&g.world.gactors[i];uint32_t r=ga_l(a,0);
                if (!a->active || r<0x87e36u || r>0x89c00u) continue;
                printf(" veh[%u] r=%05lx x=%d y=%d f15=%02x f5f=%02x 6b=%02x vx=%ld",i,(unsigned long)r,
                    g.camera+(int16_t)ga_w(a,0x3c),(int16_t)ga_w(a,0x40),ga_b(a,0x15),ga_b(a,0x5f),ga_b(a,0x6b),(long)(int32_t)ga_l(a,0x30));
            }
            putchar('\n');lasthits=g.hits;
        }
        if (hold>600) break;
    }
    return 0;
}
