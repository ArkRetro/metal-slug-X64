#include <assert.h>
#include <inttypes.h>
#include <stdio.h>
#include "../src/neo_motion.h"
int main(void) {
    FILE *f=fopen("data/original-air-trace.csv","r");assert(f);
    char header[100];assert(fgets(header,sizeof(header),f));
    unsigned frame,checks=0;uint32_t x,y,vx,vy;
    assert(fscanf(f,"%u,%"SCNu32",%"SCNu32",%"SCNu32",%"SCNu32" ",&frame,&x,&y,&vx,&vy)==5);
    neo_air_t p={(int32_t)x,(int32_t)y,(int32_t)vx,NEO_JUMP_VY};
    while (fscanf(f,"%u,%"SCNu32",%"SCNu32",%"SCNu32",%"SCNu32" ",&frame,&x,&y,&vx,&vy)==5) {
        neo_air_step(&p,1);
        assert(p.x==(int32_t)x && p.y==(int32_t)y && p.vx==(int32_t)vx && p.vy==(int32_t)vy);checks++;
    }
    assert(checks==22);fclose(f);
    assert(neo_asr(-33,5)==-2 && neo_asr(-32,5)==-1 && neo_asr(33,5)==1);
    p=(neo_air_t){0,0,-3*NEO_UNIT,NEO_JUMP_VY};neo_air_step(&p,-1);
    assert(p.vx== -3*NEO_UNIT-NEO_UNIT/4);
    puts("PASS: arcade air trajectory, 22 full Q16.16 states match; signed shift and left acceleration");
}
