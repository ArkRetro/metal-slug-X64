#include <assert.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include "../src/neo_collision.h"
int main(void) {
    neo_surface_t s;
    assert(!neo_surface_at(-1,64*NEO_UNIT,NEO_UNIT,NEO_UNIT,&s));
    assert(!neo_surface_at(1192*NEO_UNIT,64*NEO_UNIT,NEO_UNIT,NEO_UNIT,&s));
    assert(neo_collision_cell(-1,64)==0 && neo_collision_cell(0,-1)==0);
    assert(neo_surface_at(600*NEO_UNIT,64*NEO_UNIT,1,1,&s) && s.height==64*NEO_UNIT);
    assert(neo_surface_at(600*NEO_UNIT,96*NEO_UNIT,1,1,&s) && s.height==96*NEO_UNIT);
    neo_air_t fall={600*NEO_UNIT,90*NEO_UNIT,0,-10*NEO_UNIT};
    assert(neo_land_step(&fall,100*NEO_UNIT) && fall.y==96*NEO_UNIT);
    fall=(neo_air_t){600*NEO_UNIT,70*NEO_UNIT,0,10*NEO_UNIT};
    assert(!neo_land_step(&fall,60*NEO_UNIT)); // Do not snap upward to an overhead surface.
    fall=(neo_air_t){600*NEO_UNIT,60*NEO_UNIT,0,-10*NEO_UNIT};
    assert(neo_land_step(&fall,70*NEO_UNIT) && fall.y==64*NEO_UNIT);
    FILE *f=fopen("data/original-walk-trace.csv","r");assert(f);
    char header[100];assert(fgets(header,sizeof(header),f));
    unsigned frame,checks=0;uint32_t x,y,vx,vy;
    assert(fscanf(f,"%u,%"SCNu32",%"SCNu32",%"SCNu32",%"SCNu32" ",&frame,&x,&y,&vx,&vy)==5);
    neo_air_t p={(int32_t)x,(int32_t)y,0,0};
    while (fscanf(f,"%u,%"SCNu32",%"SCNu32",%"SCNu32",%"SCNu32" ",&frame,&x,&y,&vx,&vy)==5) {
        assert(neo_walk_step(&p,1));
        // Recovered planes match the original trajectory to <=2 Q16 fractional
        // units; the arcade's complete corner/edge resolver is not ported yet.
        assert(abs(p.x-(int32_t)x)<=2 && abs(p.y-(int32_t)y)<=1);
        assert(abs(p.vx-(int32_t)vx)<=2 && abs(p.vy-(int32_t)vy)<=1);checks++;
    }
    fclose(f);assert(checks==350);
    f=fopen("data/original-roof-trace.csv","r");assert(f);
    assert(fgets(header,sizeof(header),f));
    assert(fscanf(f,"%u,%"SCNu32",%"SCNu32",%"SCNu32",%"SCNu32" ",&frame,&x,&y,&vx,&vy)==5);
    neo_air_t roof={(int32_t)x,(int32_t)y,0,NEO_JUMP_VY};
    bool landed=false;unsigned roof_checks=0;
    while (fscanf(f,"%u,%"SCNu32",%"SCNu32",%"SCNu32",%"SCNu32" ",&frame,&x,&y,&vx,&vy)==5) {
        if (landed) assert(neo_walk_step(&roof,0));
        else {
            int32_t previous=roof.y;neo_air_step(&roof,0);
            landed=neo_land_step(&roof,previous);
        }
        assert(roof.x==(int32_t)x && roof.y==(int32_t)y && roof.vx==(int32_t)vx && roof.vy==(int32_t)vy);
        roof_checks++;
    }
    fclose(f);assert(roof_checks==24 && landed && roof.y==96*NEO_UNIT);
    neo_air_t reverse=p;
    assert(neo_walk_step(&reverse,-1) && reverse.x<p.x);
    int32_t slope_x,slope_y;
    neo_ground_velocity(1,1,&slope_x,&slope_y);assert(slope_x==190464 && slope_y==47616);
    puts("PASS: 350 walk states within 2/65536px; 24 original roof-jump/landing states bit-exact; stacked surfaces and bounds");
}
