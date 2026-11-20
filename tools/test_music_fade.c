/* src/msx_fade.h against the capture's ticks (build/captures/ZF/z80tap.txt, tools/check_music_fade.py). */
#include <stdio.h>
#include <string.h>
#include "msx_fade.h"
#include "music_fade.h"
int main(int argc,char **argv) {
    FILE *f=fopen(argc>1?argv[1]:"build/captures/ZF/z80tap.txt","r");
    if (!f) {printf("test_music_fade: skipped (no capture)\n");return 0;}
    char tm[64];unsigned ad,v,fr,low=0,last=0;unsigned m6=0,m7=0,m8=0;long ticks=0,bad=0;int level=-1;
    while (fscanf(f," W %63s %x %x %u",tm,&ad,&v,&fr)==4) {
        if (ad==0xf804) {low=v;last=ad;continue;}
        if (ad==0xf805 && last!=0xf804) {level=(int)((v<<8)|((unsigned)level&0xffu));last=ad;continue;}
        last=ad;
        if (ad==0xf805) {
            unsigned hl=v<<8|low;
            if (level>=0 && msx_fade_tick((uint16_t)level,(uint16_t)(m6|m7<<8),m8!=0)!=hl) bad++;
            level=(int)hl;ticks++;continue;
        }
        if (ad==0xf806) m6=v; else if (ad==0xf807) m7=v; else if (ad==0xf808) m8=v;
    }
    fclose(f);
    /* The two steps of the capture: 0A80 (F806 0070, frame 60098) and 0AA0 (00F0, 62134 / 72303). */
    int steps=msx_fade_step(music_fade_table,0x80)==0x70 && msx_fade_step(music_fade_table,0xa0)==0xf0 && msx_fade_step(music_fade_table,0x20)==0xffff;
    printf("%s: master level, %ld timer A ticks, %ld differ; steps 0A80 / 0AA0 / 0A20 %s\n",bad||!ticks||!steps?"FAIL":"PASS",ticks,bad,steps?"equal":"differ");
    return bad||!ticks||!steps;
}
