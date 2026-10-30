/* Host helper of tools/audit_script_events.py: for each hex spawn-table
 * constructor on the command line, prints "<hex> 1" when the native world
 * creates it (neo_world_spawn_native), "<hex> 0" when it is dropped. */
#include <stdio.h>
#include <stdlib.h>
#include "../src/neo_world.h"
int main(int argc,char **argv) {
    for (int i=1;i<argc;i++) {uint32_t c=(uint32_t)strtoul(argv[i],NULL,16);printf("%06x %d\n",(unsigned)c,neo_world_spawn_native(c)?1:0);}
    return 0;
}
