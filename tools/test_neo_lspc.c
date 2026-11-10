/* The LSPC auto-animation counter (src/neo_lspc.h, G25) against MAME 0.264:
 * from the first sample frame, every (counter, frame counter) start state
 * (the counter runs since power-on, so its phase is not in the ROM) is
 * tried; the model must then give every sampled counter (3C0006 & 7), with
 * the speed set through 002802's D0 at the frames MAME wrote REG_LSPCMODE.
 * Frames as the captures count them (frame f = f frames completed, sampled
 * at the frame-done hook): a write logged at f is made by the main loop
 * after that frame's vblank, so the first tick with the new speed is the one
 * after sample f + 1 (778/778 samples; applied at f or f + 2: 25 and 197
 * different). In the port the camera op runs in msx_game_update after the
 * update's neo_lspc_frame, the same order. */
#include <assert.h>
#include <stdio.h>
#include "neo_lspc.h"
#include "lspc_fixture.h"
int main(void) {
    unsigned total=0;
    for (unsigned r=0;r<sizeof(lspc_runs)/sizeof(lspc_runs[0]);r++) {
        unsigned best=~0u,bc=0,bf=0;
        for (unsigned c0=0;c0<8;c0++) for (unsigned f0=0;f0<256;f0++) {
            neo_lspc_t l={0};neo_lspc_set(&l,(uint16_t)lspc_runs[r].start_d0);
            if (f0>l.speed) break;
            l.counter=(uint8_t)c0;l.frame_counter=(uint8_t)f0;
            unsigned bad=0,wi=0,si=0;
            for (uint32_t f=lspc_runs[r].s[0][0];f<=lspc_runs[r].last && si<lspc_runs[r].ns;f++) {
                if (lspc_runs[r].s[si][0]==f) {if ((l.counter&7u)!=lspc_runs[r].s[si][1]) bad++;si++;}
                while (wi<lspc_runs[r].nw && lspc_runs[r].w[wi][0]+1u<=f) {if (lspc_runs[r].w[wi][0]+1u==f) neo_lspc_set(&l,(uint16_t)lspc_runs[r].w[wi][1]);wi++;}
                neo_lspc_frame(&l);
            }
            if (bad<best) {best=bad;bc=c0;bf=f0;}
        }
        printf("lspc run %u: %u samples, %u writes, start counter %u frame counter %u: %u different\n",r,lspc_runs[r].ns,lspc_runs[r].nw,bc,bf,best);
        fflush(stdout);assert(best==0);total+=lspc_runs[r].ns;
    }
    printf("PASS: LSPC auto-animation counter (002802 speeds, MAME timer) at %u sampled frames\n",total);
    return 0;
}
