/* The body programs' +72/+74 (src/marco_legs.c) update by update against
 * the Marco catalog capture marco-cat1 (data/legs_fixture.json,
 * tools/extract_legs_fixture.py: MARCO_STEP_OUT of the body state): the run
 * 24DD18 at the body speed 0x1C1 (walk, run cycles both ways), the jump
 * 24E4E0 and the running jump 24E858 (holding while +78 = FF, the capture's)
 * until the
 * landing. Bit-exact words. */
#include <stdio.h>
#include "../src/marco_legs.h"
#include "legs_fixture.h"
int main(void) {
    unsigned ok=0,bad=0;
    for (unsigned i=0;i<LEGS_CASES;i++) {
        const legs_case_t *c=&legs_cases[i];const legs_step_t *s=&legs_steps[c->first];
        marco_legs_t l;marco_legs_reset(&l);
        /* The program running before the case: idle on the ground. */
        marco_legs_step(&l,true,false,false,false,false,false);
        unsigned k=0;
        /* The run cycle to the left starts with the turn 24EF0E (no
         * +72/+74 writes; marco_visual.c does not play it): from 24DD18. */
        while (k<c->count && (s[k].cursor<0x24dd18 || s[k].cursor>0x24de3c) && !c->jump) k++;
        bool landed=false;
        for (;k<c->count;k++) {
            bool ground=c->jump?false:s[k].ground!=0;
            if (c->jump && k && s[k].ground && !s[k-1].ground) landed=true;
            if (landed) break;
            marco_legs_step(&l,ground,c->moving!=0,false,c->moving && c->jump,false,s[k].f78==0xff);
            int16_t x=(int16_t)(s[k].f72>>16),y=(int16_t)s[k].f72;
            if (l.x72!=x || l.y74!=y) {if (bad<20) fprintf(stderr,"%s step %u (frame %u): +72/+74 %d,%d, capture %d,%d\n",c->name,k,s[k].frame,l.x72,l.y74,x,y);bad++;}
            else ok++;
        }
    }
    printf("legs: %u body steps equal, %u different\n",ok,bad);
    if (bad) return 1;
    puts("PASS: +72/+74 of the run (speed 0x1C1), jump and running jump programs, update by update");
    return 0;
}
