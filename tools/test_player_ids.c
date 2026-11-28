/* B82: player 1's node record of the port (marco_mount_record_ids) for each
 * character against MAME 0.264 (data/player_ids_fixture.txt: RAM
 * 1014C4+6E..+71), and what the vehicle binding 0AA760 hands the vehicle
 * (neo_vslot_bind: +6E -> the Camel's +70), which the Camel's rider graphic
 * 034F1A turns into its program table 23A58E[+70] (MAME rider +70 / +72). */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../src/marco_mount.h"
#include "../src/neo_vslot.h"
int main(int argc,char **argv) {
    FILE *f=fopen(argc>1?argv[1]:"data/player_ids_fixture.txt","r");
    if (!f) {perror("fixture");return 1;}
    /* 23A58E: the four rider tables (P2 bank, as in the ROM). */
    static const uint32_t rider_table[4]={0x034388,0x0343a8,0x0343c8,0x0343e8};
    char line[256];unsigned n=0,bad=0;
    while (fgets(line,sizeof(line),f)) {
        unsigned c,rec,r70,r72;
        if (line[0]=='#' || sscanf(line,"%u %x %x %x",&c,&rec,&r70,&r72)!=4) continue;
        uint8_t m[0xc0];memset(m,0xaa,sizeof(m));m[0x5f]=0;
        marco_mount_record_ids(m,c);
        uint32_t got=(uint32_t)m[0x6e]<<24|(uint32_t)m[0x6f]<<16|(uint32_t)m[0x70]<<8|m[0x71];
        neo_vslots_t v;memset(&v,0,sizeof(v));
        uint8_t veh[0xc0];memset(veh,0,sizeof(veh));
        neo_vslot_player_t pl;memset(&pl,0,sizeof(pl));pl.exists=true;pl.m=m;
        uint16_t p6e=0xffff;
        bool bound=neo_vslot_bind(&v,0,veh,0x8001,&pl,0,0,0x18,&p6e);
        bool ok=got==rec && bound && p6e==r70 && p6e<4 && rider_table[p6e]==r72;
        printf("character %u: record %08x (MAME %08x) bound %d +70 %04x (MAME %04x) table %06x (MAME %06x) %s\n",c,(unsigned)got,rec,bound,p6e,r70,p6e<4?(unsigned)rider_table[p6e]:0u,r72,ok?"ok":"DIFFERENT");
        n++;bad+=!ok;
    }
    fclose(f);
    if (n!=4 || bad) {printf("FAIL: player ids %u/%u different\n",bad,n);return 1;}
    printf("PASS: player ids, 4 characters equal to MAME\n");return 0;
}
