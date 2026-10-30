/* Fix-layer HUD (src/neo_hud.c) against the fix-layer VRAM of MAME dumps
 * (data/hud_fixture.json.gz, tools/extract_hud_fixture.py): each interval
 * starts from the dump at f0 (HUD rows, panel fields, blink counters, timer)
 * and replays every frame to f1 with the weapons.csv inputs of 0C071C/
 * 0C0764; the HUD rows must equal the dump at f1 word for word and the timer
 * the RAM word 1013DE (frozen captures hold 1013DE = 5A after every frame). */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../src/neo_hud.h"
#include "hud_fixture.h"

static unsigned hexbyte(const char *s) {unsigned v;sscanf(s,"%2x",&v);return v;}

int main(void) {
    static neo_hud_t h;
    unsigned bad=0,words=0,checked=0,timer_checked=0,static_ok=0;
    unsigned counts[8]={0},unknown_parity=0,resets=0,timer_unknown=0;
    for (unsigned i=0;i<HUD_INTERVALS;i++) {
        const hud_interval_t *v=&hud_intervals[i];
        neo_hud_init(&h,v->timer);
        const unsigned short *fix0=hud_words+v->words,*fix1=fix0+HUD_ROW_COUNT*40;
        for (unsigned r=0;r<HUD_ROW_COUNT;r++) for (unsigned c=0;c<40;c++) h.fix[c*32+hud_rows[r]]=fix0[r*40+c];
        for (unsigned p=0;p<2;p++) {
            const unsigned short *q=v->panel[p];neo_panel_t *n=&h.panel[p];
            n->f6e=q[0];n->f74=q[1];n->f76=q[2];n->f78=q[3];n->f7a=q[4];n->f7c=q[5];n->f7e=q[6];
            n->f84=(uint8_t)q[7];n->f85=(uint8_t)q[8];n->f86=(uint8_t)q[9];n->f89=(uint8_t)q[10];n->f8a=(uint8_t)q[11];n->f8b=(uint8_t)q[12];
            h.blink[p]=v->blink[p];
        }
        const unsigned char *f=v->flags;
        /* The game updates every second video frame, sometimes later: a
         * frame whose 10000E changed ran one update. */
        /* The panels run before the actors that change what they show (player,
         * items, enemies): an update reads the values of the previous one,
         * the weapons.csv row of the last frame that ran an update (step 0
         * is the row of f0). */
        uint16_t video=v->video;
        for (unsigned k=1;k<=v->f1-v->f0;k++) {
            const char *s=hud_steps[v->steps+k];
            uint16_t now=(uint16_t)(hexbyte(s+122)<<8|hexbyte(s+124));
            if (now==video) continue;
            if ((uint16_t)(now-video)!=1) {fprintf(stderr,"%u: 10000E %04X -> %04X\n",v->f0+k,video,now);bad++;}
            video=now;s=hud_steps[v->steps+k-1];
            uint8_t block[20],slot[36];
            for (unsigned b=0;b<20;b++) block[b]=(uint8_t)hexbyte(s+2*b);
            for (unsigned b=0;b<36;b++) slot[b]=(uint8_t)hexbyte(s+40+2*b);
            uint16_t input=(uint16_t)(hexbyte(s+112)<<8|hexbyte(s+114));
            uint8_t f15=(uint8_t)hexbyte(s+120);
            uint8_t score[4];for (unsigned b=0;b<4;b++) score[b]=(uint8_t)hexbyte(s+130+2*b);
            uint16_t rescued=(uint16_t)(hexbyte(s+138)<<8|hexbyte(s+140));
            neo_hud_input_t in={0};
            in.hud_on=f[0]!=0;in.two_players=f[1]!=0;
            in.arms=neo_hud_arms(block,slot,f[2],in.two_players,0);
            neo_hud_bombs(block,slot,input,f15,0,&in.bombs,&in.bomb_type);
            memcpy(in.score,score,4);in.rescued=rescued;
            in.video=video;in.join_mask=f[3];in.mvs=f[4]!=0;
            in.credit_text=f[6]&&(f[5]&2);
            /* 018704..01875A: a player in play (0193FE: P1 alive, +5F bit 7
             * clear; P2 never exists here), 104303 bit 7, one player
             * (10C2E7), 104300 clear, 10E9E3 clear (0C8CFC), 107003 set.
             * The RAM flags are known at f0 and f1 only: an interval where
             * one of them changes is left out of the timer check. */
            uint32_t routine=(uint32_t)hexbyte(s+142)<<24|(uint32_t)hexbyte(s+144)<<16|(uint32_t)hexbyte(s+146)<<8|hexbyte(s+148);
            uint8_t f5f=(uint8_t)hexbyte(s+150);
            in.timer_running=routine!=0x3f1e&&!(f5f&0x80)&&(f[7]&0x80)&&!f[8]&&f[3]&&!f[1]&&!f[9];
            if (slot[0]|slot[1]) {
                uint8_t kind=slot[0x21];
                in.gauge=kind==1||kind==3||kind==4||kind>=8;
                in.gauge_health=(uint16_t)(slot[0x18]<<8|slot[0x19]);in.gauge_max=(uint16_t)(slot[0x14]<<8|slot[0x15]);
            }
            neo_hud_frame(&h,&in);
            if (v->frozen) h.timer=(uint16_t)(0x5a00|(h.timer&0xff));
            /* Script spawns 01913C run after the mission actor's count-down
             * (the update shows xxFF, not xxFD): the RAM word after the
             * update, once every actor ran (the row before the next update),
             * holding xxFF while the model does not is the reset. */
            {
                unsigned e=k;
                while (e<v->f1-v->f0) {const char *n=hud_steps[v->steps+e+1];if ((uint16_t)(hexbyte(n+122)<<8|hexbyte(n+124))!=video) break;e++;}
                const char *n=hud_steps[v->steps+e];
                uint16_t t=(uint16_t)(hexbyte(n+126)<<8|hexbyte(n+128));
                if ((t&0xff)==0xff && t!=h.timer) {
                    uint8_t dip=3;for (uint8_t d=0;d<4;d++) if (!v->frozen && neo_hud_timer_start(d)==t>>8) dip=d;
                    neo_hud_timer_reset(&h,dip);resets++;
                    if (v->frozen) h.timer=(uint16_t)(0x5a00|(h.timer&0xff));
                }
            }
        }
        unsigned miss=0;
        for (unsigned r=0;r<HUD_ROW_COUNT;r++) for (unsigned c=0;c<40;c++,words++) {
            uint16_t got=h.fix[c*32+hud_rows[r]],want=fix1[r*40+c];
            if (got!=want) {
                miss++;
                unsigned region=hud_rows[r]>=28?0:c<10?1:c<18?2:c<22?3:4;counts[region]++;
                if (bad<12) fprintf(stderr,"%s %u..%u row %u col %u: %04X, dump %04X\n",hud_capture_names[v->capture],v->f0,v->f1,hud_rows[r],c,got,want);
            }
        }
        if (miss) bad++;
        checked++;
        const unsigned char *g=v->flags0;
        bool flags_change=(g[0]^f[7])&0x80||!g[1]!=!f[8]||!g[2]!=!f[9]||!g[3]!=!f[3];
        if (flags_change) timer_unknown++;
        else if (h.timer!=v->timer1) {if (bad<12) fprintf(stderr,"%s %u..%u timer %04X, RAM %04X\n",hud_capture_names[v->capture],v->f0,v->f1,h.timer,v->timer1);bad++;}
        else timer_checked++;
        if (h.unsupported) {fprintf(stderr,"explicit stop in %u..%u\n",v->f0,v->f1);bad++;}
    }
    /* The panel's first update with the HUD on draws its frame (0C966A):
     * from the baseline map and the panel fields of the first interval, the
     * whole HUD of that dump is rebuilt except the INSERT COIN blink phase. */
    {
        const hud_interval_t *v=&hud_intervals[0];
        neo_hud_init(&h,v->timer);
        for (unsigned p=0;p<2;p++) {neo_panel_t *n=&h.panel[p];n->f84=(uint8_t)v->panel[p][7];h.blink[p]=0;}
        neo_hud_input_t in={0};in.hud_on=true;{const char *s=hud_steps[v->steps];uint8_t block[20],slot[36];for (unsigned b=0;b<20;b++) block[b]=(uint8_t)hexbyte(s+2*b);for (unsigned b=0;b<36;b++) slot[b]=(uint8_t)hexbyte(s+40+2*b);in.arms=neo_hud_arms(block,slot,v->flags[2],false,0);}h.panel[0].f7a=v->panel[0][4];in.bombs=h.panel[0].f8a=(uint8_t)v->panel[0][11];in.bomb_type=1;
        memcpy(in.score,v->score,4);in.rescued=v->rescued;in.join_mask=v->flags[3];in.mvs=v->flags[4]!=0;in.credit_text=v->flags[6]&&(v->flags[5]&2);
        neo_hud_frame(&h,&in);
        const unsigned short *fix0=hud_words+v->words;
        for (unsigned r=0;r<HUD_ROW_COUNT;r++) for (unsigned c=0;c<40;c++)
            if (!(hud_rows[r]<5 && c>=26 && c<37)) {
                if (h.fix[c*32+hud_rows[r]]==fix0[r*40+c]) static_ok++;
                else {fprintf(stderr,"static frame row %u col %u: %04X, dump %04X\n",hud_rows[r],c,h.fix[c*32+hud_rows[r]],fix0[r*40+c]);bad++;}
            }
    }
    printf("HUD: %u intervals, %u fix words, %u timers; mismatching intervals %u (stars %u, score/lives/gauge %u, ARMS/BOMB %u, timer %u, P2 %u); first frame %u words; %u intervals of unknown parity; %u timer resets 01913C; %u timers left out (RAM flag changed in the interval)\n",
           checked,words,timer_checked,bad,counts[0],counts[1],counts[2],counts[3],counts[4],static_ok,unknown_parity,resets,timer_unknown);
    if (bad) return 1;
    printf("PASS: fix-layer HUD bit-exact\n");
    return 0;
}
