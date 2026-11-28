/* Thin fire states (src/marco_thinfire.c) against MAME 0.264
 * (data/thinfire_fixture.txt, tools/extract_thinfire_fixture.py from
 * tools/capture_fat_weapons.lua with MSX_FAT_THIN=1 MSX_FAT_LOAD=sec01_19391
 * MSX_FAT_IDS=3,2,4,6,c): every player update of the R, S, F and C segments
 * must start a fire program exactly when MAME allocates its projectile. The
 * beam (ids 6/0B, 0C3E02) is another state: its updates restart the states. */
#include <stdio.h>
#include <string.h>
#include "../src/marco_thinfire.h"
int main(int argc,char **argv) {
    FILE *f=fopen(argc>1?argv[1]:"data/thinfire_fixture.txt","r");
    if (!f) {perror("fixture");return 1;}
    char line[256];marco_thin_t s;marco_thin_reset(&s);
    unsigned rows=0,shots=0,bad=0,fires=0;
    while (fgets(line,sizeof line,f)) {
        if (line[0]=='#') continue;
        unsigned fr,id,fresh,up,down,g,crouch,landed,turned,walking,jumped,knife,live,cap,fire;
        if (sscanf(line,"%u %u %u %u %u %u %u %u %u %u %u %u %u %u %u",&fr,&id,&fresh,&up,&down,&g,&crouch,&landed,&turned,&walking,&jumped,&knife,&live,&cap,&fire)!=15) {fprintf(stderr,"bad row: %s",line);return 1;}
        if (id==6 || id==11) {marco_thin_reset(&s);s.grounded=g;continue;}
        marco_thin_in_t in={(uint8_t)id,fresh!=0,up!=0,down!=0,g!=0,crouch!=0,landed!=0,turned!=0,walking!=0,jumped!=0,knife!=0,(uint8_t)live,(uint8_t)cap};
        marco_thin_out_t o=marco_thin_step(&s,&in);
        rows++;shots+=fire;fires+=o.fire;
        if (o.fire!=(fire!=0)) {bad++;fprintf(stderr,"frame %u id %X: port %s, MAME %s (state %d)\n",fr,id,o.fire?"fires":"-",fire?"fires":"-",(int)s.state);}
    }
    fclose(f);
    printf("thin fire states: %u updates, %u MAME shots, %u port shots, %u differences\n",rows,shots,fires,bad);
    return bad!=0;
}
