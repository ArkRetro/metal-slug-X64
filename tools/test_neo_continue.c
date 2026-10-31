/* Game over / continue of the P1 panel (src/neo_hud.c: 0C9848, 0C98CC,
 * 0C9918, 0C99E6, 0C9ACE and the mission actor's timer 0187A4) against MAME
 * captures (data/continue_fixture.json.gz, tools/extract_continue_fixture.py):
 * from the update of the game over, every update's P1 panel rows (16 columns
 * x 3 rows from fix 7043), the continue timer 1013E2 and the panel's +88 must
 * match; a taken continue must leave the score's continue digit, the lives
 * of the DIP and player mode 1. go1: no coin (countdown to GAME OVER), go2:
 * coin and start during the countdown, go3: B pressed every 40 frames (the
 * countdown speed-up below 0x780). */
#include <stdio.h>
#include <string.h>
#include "../src/neo_hud.h"
#include "continue_fixture.h"
int main(void) {
    unsigned fails=0,updates=0,words=0;
    for (unsigned c=0;c<CONT_CAPTURES;c++) {
        const cont_capture_t *cap=&cont_captures[c];
        neo_hud_t h;neo_hud_init(&h,0x36ff);
        const unsigned short *initial=&cont_words[cont_updates[cap->first].region-48];
        for (unsigned i=0;i<48;i++) h.fix[(0x7043-0x7000)+(i/3)*32+i%3]=initial[i];
        h.panel[0].f86=0xff;h.panel[0].f84=0;h.panel[0].f88=1;h.mode[0]=1;
        unsigned char score[4];memcpy(score,cap->score,4);
        for (unsigned k=0;k<cap->count;k++) {
            const cont_update_t *u=&cont_updates[cap->first+k];
            neo_hud_input_t in={0};
            in.hud_on=u->hud_on;in.join_mask=(uint8_t)cap->join;in.mvs=cap->mvs;in.credit_text=true;
            in.continue_dip=cap->dip8a!=0;in.can_continue=true;in.credit_count=(uint8_t)u->credits;
            in.start=u->start;in.speedup=u->b;in.lives_dip=3;in.no_life=k==0;
            memcpy(in.score,score,4);
            neo_hud_frame(&h,&in);updates++;
            if (h.continued) memcpy(score,h.score,4);
            const unsigned short *want=&cont_words[u->region];
            /* The continue's update goes on in 0C94D6 (join graphic at 7063,
             * not ported: explicit stop): its panel words are not compared. */
            for (unsigned i=0;i<48 && !u->start;i++) {
                unsigned short got=h.fix[(0x7043-0x7000)+(i/3)*32+i%3];words++;
                if (got!=want[i] && fails++<10) fprintf(stderr,"%s frame %u: fix %04x = %04x, MAME %04x\n",cap->name,u->frame,0x7043+(i/3)*32+i%3,got,want[i]);
            }
            if (h.continue_timer!=u->timer && fails++<10) fprintf(stderr,"%s frame %u: timer %04x, MAME %04x\n",cap->name,u->frame,h.continue_timer,u->timer);
            if (h.panel[0].f88!=u->mode && fails++<10) fprintf(stderr,"%s frame %u: +88 %u, MAME %u\n",cap->name,u->frame,h.panel[0].f88,u->mode);
            if (u->start) {
                if ((!h.continued || memcmp(score,u->score,4) || h.panel[0].f84!=u->lives || h.mode[0]!=1) && fails++<10)
                    fprintf(stderr,"%s frame %u: continue %d score %02x%02x%02x%02x lives %u\n",cap->name,u->frame,h.continued,score[0],score[1],score[2],score[3],h.panel[0].f84);
            }
        }
        unsigned last=h.panel[0].state;
        printf("%s: %u updates from frame %u, panel %s\n",cap->name,cap->count,cap->start,last==NEO_PANEL_GAME_OVER?"GAME OVER":last==NEO_PANEL_PLAY?"continued":"other");
    }
    if (fails) {fprintf(stderr,"FAIL: %u differences\n",fails);return 1;}
    printf("PASS: continue/game over, %u updates, %u panel words, timer and +88 equal to MAME\n",updates,words);
    return 0;
}
