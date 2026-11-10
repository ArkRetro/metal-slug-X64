/* Mission 3 stage start (B56): the camera list's head 0B8968 runs whole at
 * the stage start (0C8774) and its events reach the game with the first
 * update. MAME run4 (docs/MISION3_ACTORES.md, section 1): frame 117969 calls
 * 0B8CE8, 0B8EAC, 0B8D34 (the train's driver 0B8D64) and 0BC548 at progress
 * 0; the next calls are 0B8CFC (mode 1) and 0B8E7E at progress 0x140.
 * Before, the head's 20 events overflowed the 8-event list and the update
 * cleared them: the train never started. Private input: romfs/mission3.pack. */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../src/neo_world.h"
static unsigned rd16(const uint8_t *p) {return (unsigned)p[0]<<8|p[1];}
static uint32_t rd32(const uint8_t *p) {return (uint32_t)p[0]<<24|(uint32_t)p[1]<<16|(uint32_t)p[2]<<8|p[3];}
static neo_world_t w;
int main(int argc,char **argv) {
    FILE *f=fopen(argc>1?argv[1]:"romfs/mission3.pack","rb");
    if (!f) {printf("SKIP: Mission 3 camera start (no pack)\n");return 0;}
    fseek(f,0,SEEK_END);long n=ftell(f);fseek(f,0,SEEK_SET);
    uint8_t *p=malloc((size_t)n);assert(p && fread(p,1,(size_t)n,f)==(size_t)n);fclose(f);
    assert(!memcmp(p,"MSXM",4) && rd16(p+6)==3 && rd16(p+4)==4);
    const uint8_t *st=p+48;assert(rd32(st)==0x016E0Eu);
    uint32_t rom=rd32(st+44),off=rd32(st+48),len=rd32(st+52);assert(rom==0x0B8968u && off+len<=(uint32_t)n);
    w.stage_camera=p+off;w.stage_camera_size=(uint16_t)len;w.stage_camera_base=rom;
    neo_world_player_t pl;memset(&pl,0,sizeof(pl));pl.alive=true;pl.follow_x=pl.follow_y=0xff;pl.flags15=0x40;   /* on the ground: op 4 (0C8524) tests +15 bit 6 */pl.px=0x58;pl.py=0x100;
    neo_world_camera(&w,&pl);
    static const uint32_t want[]={0x0B8CE8,0x0B8EAC,0x0B8D34,0x0BC548};unsigned k=0;
    for (unsigned i=0;i<w.camera.event_count;i++)
        if (w.camera.events[i].kind==NEO_CAMERA_EV_CALL) {assert(k<4 && w.camera.events[i].address==want[k] && w.camera.events[i].progress==0);k++;}
    assert(k==4 && w.camera.event_dropped==0);
    printf("PASS: Mission 3 camera start: %u events, calls 0B8CE8 0B8EAC 0B8D34 0BC548 at progress 0, none dropped\n",w.camera.event_count);
    return 0;
}
