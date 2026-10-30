/* Heavy machine gun upper body (marco_hgun.h). The programs are the ones
 * 0C0F62 selects for Marco (+6E = 0) with +93 = 1 from the tables named
 * below; only what the states read is kept: the update of each fire
 * callback (with its angle byte for 0C453A), the update of the +76 write
 * and the update of the END command (program bytes in bank 1, read with the
 * WAIT/POSE/CALL/WRITE/END formats of tools/convert_marco_programs.py). */
#include "marco_hgun.h"
#include <string.h>

typedef struct { uint8_t calls; uint8_t at[4]; uint8_t angle[4]; bool c453a; uint8_t flag76_at; uint8_t end; } program_t;
static const program_t programs[HGUN_PROGRAMS]={
    /* 0BFDD8+10: 2534FE, 0C4372 at 0/2/4/6, +76=1 at 7, END at 12. */
    {4,{0,2,4,6},{0,0,0,0},false,7,12},
    /* 0BFE18+10: 253662 (then 253A40), +76=1 at 7, END at 16. */
    {4,{0,2,4,6},{0,0,0,0},false,7,16},
    /* 0BFE58+10: 2560C0 (then 256096), +76=FF at 7, END at 19. */
    {4,{0,2,4,6},{0,0,0,0},false,7,19},
    /* 0BFE98+10: 2537CE, 0C453A 08/18/28/38, END at 8. */
    {4,{0,2,4,6},{0x08,0x18,0x28,0x38},true,0xff,8},
    /* 0BFED8+10: 2561DC, 0C453A F8/E8/D8/C8, END at 8. */
    {4,{0,2,4,6},{0xf8,0xe8,0xd8,0xc8},true,0xff,8},
    /* 0BFF18+10: 253840, 0C453A 30/20/10, END at 6. */
    {3,{0,2,4,0},{0x30,0x20,0x10,0},true,0xff,6},
    /* 0BFF58+10: 25624E, 0C453A D0/E0/F0, END at 6. */
    {3,{0,2,4,0},{0xd0,0xe0,0xf0,0},true,0xff,6},
};

void marco_hgun_reset(marco_hgun_t *h) {memset(h,0,sizeof *h);}
void marco_hgun_throw(marco_hgun_t *h) {h->state=MARCO_HGUN_IDLE;h->aim92=0;h->request=false;}

static void start(marco_hgun_t *h,marco_hgun_state_t state,unsigned program) {
    h->state=state;h->program=(uint8_t)program;h->t=0;
    if (state==MARCO_HGUN_SWEEP) h->request=false;   /* 0C3FE2 */
}
/* 0C3FA6..0C3FD6 from the input and +92 (0C3EC4.. / 0C4052..): the sweep to
 * start, or -1 for none (fire again at the current aim). */
static int sweep_for(marco_hgun_t *h,const marco_hgun_input_t *in) {
    if (in->up) {
        if (!h->aim92) {h->aim92=1;return HGUN_FWD_UP;}
        if (h->aim92<0) {h->aim92=0;return HGUN_DOWN_FWD;}
        return -1;
    }
    if (in->down) {
        if (!h->aim92) {h->aim92=-1;return HGUN_FWD_DOWN;}
        if (h->aim92>0) {h->aim92=0;return HGUN_UP_FWD;}
        return -1;
    }
    if (h->aim92>0) {h->aim92=0;return HGUN_UP_FWD;}
    if (h->aim92<0) {h->aim92=0;return HGUN_DOWN_FWD;}
    return -1;
}
/* 0C3E9C: the fire state for the H. */
static void fire_state(marco_hgun_t *h,const marco_hgun_input_t *in) {
    h->request=false;                                  /* 0C3E9C/0C3EA2 */
    int s=sweep_for(h,in);
    if (s>=0) {start(h,MARCO_HGUN_SWEEP,(unsigned)s);return;}
    start(h,MARCO_HGUN_BURST,h->aim92>0?HGUN_FIRE_UP:h->aim92<0?HGUN_FIRE_DOWN:HGUN_FIRE_FWD);
    h->flag76=false;                                   /* 0C3F08 */
}
/* 0C3778: one update of the program; the callbacks of this update. */
static unsigned run(marco_hgun_t *h,const marco_hgun_input_t *in,marco_hgun_reserve_t reserve,void *context,
                    marco_hgun_shot_t *out,unsigned max,unsigned n,bool *done) {
    const program_t *p=&programs[h->program];
    for (unsigned i=0;i<p->calls;i++) if (p->at[i]==h->t && n<max) {
        if (!reserve(context)) continue;               /* 0C0D64 refused: no bullet, no +95 */
        uint8_t angle;
        if (p->c453a) {
            /* 0C453A: the program's angle, mirrored about 0x40 facing left; +95 counts. */
            angle=p->angle[i];if (in->facing_left) angle=(uint8_t)(0x80-angle);
            h->combo95++;out[n++]=(marco_hgun_shot_t){true,true,angle,angle};
        } else {
            /* 0C4372 -> 0C447C: 0C4244 (up 0x40, forward 0, down -0x40; facing
             * left 0x80 - d7), then 0C44A6: +95 + 1, angle + 2*((+95 mod 3) - 1). */
            angle=h->aim92>0?0x40:h->aim92<0?0xc0:0;
            if (in->facing_left) angle=(uint8_t)(0x80-angle);
            uint8_t base=angle;h->combo95++;angle=(uint8_t)(angle+2*((int)(h->combo95%3u)-1));
            out[n++]=(marco_hgun_shot_t){true,false,angle,base};
        }
    }
    if (p->flag76_at==h->t) h->flag76=true;
    *done=h->t>=p->end;
    if (!*done) h->t++;
    return n;
}
unsigned marco_hgun_step(marco_hgun_t *h,const marco_hgun_input_t *in,marco_hgun_reserve_t reserve,void *context,
                         marco_hgun_shot_t *out,unsigned max) {
    unsigned n=0;bool done;
    if (h->state==MARCO_HGUN_IDLE) {
        /* Idle (0C38B2): +92 keeps the aim of the last program (hfam 12308..
         * 12312: up held, +92 = 0 until the press starts a sweep). */
        if (!h->request) return 0;
        /* The idle state sees +9B bit 4 and sets 0C3CD0 for the next update
         * (hfam 11952: press, first bullet two updates later). */
        h->state=MARCO_HGUN_ENTER;return 0;
    } else if (h->state==MARCO_HGUN_ENTER) {
        fire_state(h,in);
    } else if (h->state==MARCO_HGUN_BURST) {
        /* 0C3F1A: before +76 the aim may change (a sweep starts now); after
         * it a new press restarts the burst (0C3F62..0C3F78). */
        if (!h->flag76) {int s=sweep_for(h,in);if (s>=0) start(h,MARCO_HGUN_SWEEP,(unsigned)s);}
        else if (h->request) fire_state(h,in);
    }
    n=run(h,in,reserve,context,out,max,n,&done);
    if (!done) return n;
    /* End of the program (0C3F7C / 0C4006): with the fire request taken the
     * next state runs in the same update (hfam: the burst after a sweep
     * fires two updates after its last diagonal); without it, idle. */
    if (!h->request) {h->state=MARCO_HGUN_IDLE;return n;}
    fire_state(h,in);
    return run(h,in,reserve,context,out,max,n,&done);
}
