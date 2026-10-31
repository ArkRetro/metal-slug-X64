/* Fix-layer HUD (docs/ARMAS_VEHICULOS.md §11). Every function names the ROM
 * routine it translates; addresses are fix VRAM words (7000 + column*32 +
 * row). Writes outside 7000..74FF are the original's TRAP #F: counted as an
 * explicit stop and dropped. */
#include "neo_hud.h"
#include <stdio.h>
#include <string.h>

/* 03D056: digit glyphs, index 16 = blank. */
static const char hex_glyph[17] = "0123456789ABCDEF ";
/* 03D0C4: divisors of 03D068/03D0D0 by digits left. */
static const uint16_t divisor[6] = {0,1,10,100,1000,10000};
/* 0C8EB8: timer digit blocks (attribute word of the top-left tile). */
static const uint16_t timer_digit[10] = {0x5800,0x5802,0x5804,0x5806,0x5808,0x580A,0x580C,0x580E,0x5900,0x5902};

/* Strings of the panels (escapes of 03D1B8: \c page, \p palette, \n row). */
static const char s_panel_p1[] = "\\c\x0b\\p\x06\\n\xa5\xa6\xa6\xa6\xa6\xa6\xa6\xaf\\n\\c\x03\\p\x01 1UP=";  /* 0C9126 */
static const char s_panel_p2[] = "\\c\x0b\\p\x06\\n\xa5\xa6\xa6\xa6\xa6\xa6\xa6\xaf\\n\\c\x03\\p\x01 2UP=";  /* 0C9144 */
static const char s_panel_1p2[] = "\\c\x03\\p\x01 1UP=\\c\x0b\\p\x06\\n\xa5\xa6\xa6\xa6\xa6\xa6\xa6\xaf\\n";   /* 0C9162 */
static const char s_panel_2p2[] = "\\c\x03\\p\x01 2UP=\\c\x0b\\p\x06\\n\xa5\xa6\xa6\xa6\xa6\xa6\xa6\xaf\\n";   /* 0C9180 */
static const char s_box[] = "\\c\x0b\\p\x06\xa0\xa1\xa2\xa3\xbc\xbd\xbe\xbf\\n\xb0\x60\x60\x60\x60\x60\x60\xb7\\n\xc0\xc1\xc2\xc3\xc4\xc5\xc6\xc7"; /* 0C919E */
static const char s_infinite[] = "\\c\x03\x9d\x9e\x9f";                    /* 0C91C2 */
static const char s_cannon[] = "\\c\x0b\\p\x06\xb8\xb9\xba\xbb";          /* 0C91DA */
static const char s_bomb[] = "\\c\x0b\\p\x06\xbc\xbd\xbe\xbf";            /* 0C91E6 */
static const char s_other[] = "\\c\x0b\\p\x06\xb1\xb2\xb3\xb4";           /* 0C91F2 */
static const char s_insert_coin[] = "\\ca\\p\x04INSERT COIN";             /* 0C90B6 */
static const char s_please_wait[] = "\\ca\\p\x04PLEASE WAIT";             /* 0C90C8 */
static const char s_press_start[] = "\\ca\\p\x04PRESS START";             /* 0C90DA */
static const char s_game_over[] = "\\ca\\p\x04 GAME OVER ";               /* 0C90EC */
static const char s_continue[] = "\\ca\\p\x04" "CONTINUE? ";             /* 0C90FE */
/* 0C9112: continue digits (word of the top tile of the two-row font). */
static const uint16_t continue_digit[10] = {0x4a60,0x4a61,0x4a62,0x4a63,0x4a64,0x4a65,0x4a66,0x4a67,0x4a68,0x4a69};
static const char s_credit[] = "CREDIT    ";                               /* 01A85C */
static const char s_credits[] = "CREDITS   ";                              /* 01A867 */

static void stop(neo_hud_t *h,const char *what,unsigned value) {
    if (!h->unsupported++) fprintf(stderr,"HUD: explicit stop %s %X\n",what,value);
}
static void poke(neo_hud_t *h,uint16_t address,uint16_t word) {
    if (address<0x7000 || address>=0x7500) {stop(h,"fix address",address);return;} /* TRAP #F */
    h->fix[address-0x7000]=word;h->serial++;
}
/* 03CF9A: one tile at d0 (attribute d1 = palette<<8|page, glyph d2), next column. */
static uint16_t put(neo_hud_t *h,uint16_t d0,uint16_t d1,uint8_t d2) {
    poke(h,d0,(uint16_t)(((d1>>8)&15)<<12|(d1&15)<<8|d2));return (uint16_t)(d0+0x20);
}
/* 03CF12: d2 columns of d3 rows of the word d1. */
static void fill(neo_hud_t *h,uint16_t d0,uint16_t d1,unsigned d2,unsigned d3) {
    for (;d2;d2--,d0=(uint16_t)(d0+0x20)) for (unsigned r=0;r<d3;r++) poke(h,(uint16_t)(d0+r),d1);
}
/* 03CF60: a d2 x d3 block of consecutive tiles (+1 per column, +0x10 per row). */
static void block(neo_hud_t *h,uint16_t d0,uint16_t d1,unsigned d2,unsigned d3) {
    for (;d2;d2--,d0=(uint16_t)(d0+0x20),d1=(uint16_t)((d1&0xff00)|((d1+1)&0xff))) {
        uint16_t w=d1;
        for (unsigned r=0;r<d3;r++,w=(uint16_t)((w&0xff00)|((w+0x10)&0xff))) poke(h,(uint16_t)(d0+r),w);
    }
}
/* 03DD96: glyph writer of 03D25A. Pages below 10 go to 03CF9A; page 'a'
 * (0x61) is the two-row font (tile (c&F0)*2+(c&F) of page A, the next row
 * +0x10). Other pages and the DE/DF prefixes are explicit stops. */
static uint16_t put_font(neo_hud_t *h,uint16_t d0,uint16_t d1,uint8_t d2,const char **a0) {
    if ((d1&0xff)<0x10) return put(h,d0,d1,d2);
    if ((uint8_t)**a0==0xde || (uint8_t)**a0==0xdf || (d1&0xff)!=0x61) {stop(h,"glyph page",d1);return (uint16_t)(d0+0x20);}
    uint16_t w=(uint16_t)(((d1>>8)&15)<<12|0x0a00|(uint8_t)((d2&0xf0)*2+(d2&15)));
    poke(h,d0,w);poke(h,(uint16_t)(d0+1),(uint16_t)((w&0xff00)|((w+0x10)&0xff)));
    return (uint16_t)(d0+0x20);
}
/* 03D1B8: escapes; returns the next glyph (0 at the end). */
static uint8_t next_glyph(neo_hud_t *h,const char **a0,uint16_t *d0,uint16_t *d1,uint16_t *d3) {
    for (;;) {
        uint8_t c=(uint8_t)*(*a0)++;
        if (c!='\\') return c;
        uint8_t e=(uint8_t)*(*a0)++;
        if (e=='n') {*d3=(uint16_t)(*d3+1);if (*d3>=0x7500) stop(h,"fix row",*d3);*d0=*d3;}
        else if (e=='c') *d1=(uint16_t)((*d1&0xff00)|(uint8_t)*(*a0)++);
        else if (e=='p') *d1=(uint16_t)((*d1&0xff)|((uint8_t)*(*a0)++&15)<<8);
        else if (e=='l') {uint16_t r=(uint8_t)*(*a0)++;r=(uint16_t)(r<<5);r=(uint16_t)((r&0xff00)|((r+(uint8_t)*(*a0)++)&0xff));*d3=(uint16_t)(r+0x7000);*d0=*d3;}
        else return e; /* 03D1E6: an unknown escape returns its letter */
    }
}
/* 03D244 (put) and 03D25A with 03DD96 (font). */
static uint16_t text(neo_hud_t *h,uint16_t d0,uint16_t d1,const char *a0,bool font) {
    uint16_t d3=d0;
    for (;;) {
        uint8_t c=next_glyph(h,&a0,&d0,&d1,&d3);
        if (!c) return d0;
        d0=font?put_font(h,d0,d1,c,&a0):put(h,d0,d1,c);
    }
}
/* 03CFBA with 03CF9A (03D004): d4 BCD digits from a0 (odd d4: low nibble
 * first); leading zeros blank while d3 is clear. */
static uint16_t bcd(neo_hud_t *h,uint16_t d0,uint16_t d1,const uint8_t *a0,uint8_t d3,unsigned d4) {
    for (;d4;d4--) {
        unsigned d2;
        if (d4&1) d2=*a0++&15; else d2=*a0>>4;
        if (d2 || d4==1) d3=0xff; else if (!d3) d2=16;
        d0=put(h,d0,d1,(uint8_t)hex_glyph[d2]);
    }
    return d0;
}
/* 03D00A with 03CF9A (03D050): as 03D004 but leading zeros are skipped. */
static uint16_t bcd_skip(neo_hud_t *h,uint16_t d0,uint16_t d1,const uint8_t *a0,unsigned d4) {
    uint8_t d3=0;
    for (;d4;d4--) {
        unsigned d2;
        if (d4&1) d2=*a0++&15; else d2=*a0>>4;
        if (d2 || d4==1) d3=0xff;
        if (d3) d0=put(h,d0,d1,(uint8_t)hex_glyph[d2]);
    }
    return d0;
}
static unsigned digit(uint16_t d7,unsigned d4) {
    if (d4>=6) return 0;
    return (unsigned)(d7/divisor[d4])%10u;
}
/* 03D068: d4 decimal digits of d7, leading zeros blank while d3 is clear. */
static uint16_t number(neo_hud_t *h,uint16_t d0,uint16_t d1,uint16_t d7,uint8_t d3,unsigned d4) {
    for (;d4;d4--) {
        if (d4==1) d3=0xff;
        unsigned d2=digit(d7,d4);
        if (d2) d3=0xff; else if (!d3) d2=16;
        d0=put(h,d0,d1,(uint8_t)hex_glyph[d2]);
    }
    return d0;
}
/* 03D0D0: as 03D068 but leading zeros are skipped. */
static uint16_t number_skip(neo_hud_t *h,uint16_t d0,uint16_t d1,uint16_t d7,uint8_t d3,unsigned d4) {
    for (;d4;d4--) {
        if (d4==1) d3=0xff;
        unsigned d2=digit(d7,d4);
        if (d2) d3=0xff;
        if (d3) d0=put(h,d0,d1,(uint8_t)hex_glyph[d2]);
    }
    return d0;
}

/* 0C92C6: panel fields for player p. */
static void panel_init(neo_panel_t *q,unsigned p) {
    memset(q,0,sizeof *q);q->f6e=(uint16_t)p;
    if (!p) {q->f74=0x7043;q->f76=0x7043;q->f78=0x7143;}
    else {q->f74=0x72c3;q->f76=0x73c3;q->f78=0x72c3;}
}
void neo_hud_init(neo_hud_t *h,uint16_t timer) {
    memset(h,0,sizeof *h);
    for (unsigned c=0;c<NEO_FIX_COLUMNS;c++) for (unsigned r=0;r<NEO_FIX_ROWS;r++)
        h->fix[c*32+r]=(c&&c<39&&r&&r<31)?0x00ff:0x0000;
    panel_init(&h->panel[0],0);panel_init(&h->panel[1],1);
    h->timer=timer;
    h->mode[0]=1;h->panel[0].f88=1;  /* single player game: BIOS 10FDB6 = 1, 10FDB7 = 0 (captures) */
}
/* 0C9254: clears the panel's three rows (16 columns of 0320). */
static void panel_clear(neo_hud_t *h,neo_panel_t *q,bool hud_on) {
    if (!hud_on) return;
    q->f86=0;fill(h,q->f74,0x0320,16,3);
}
/* 0C9B94: the vehicle gauge. */
static void gauge(neo_hud_t *h,neo_panel_t *q,const neo_hud_input_t *in) {
    if (!in->gauge) {
        if (!q->f7c) return;
        q->f7c=0;q->f7e=0;
    } else {
        uint16_t d7;
        if (!in->gauge_max) d7=0;
        else if (in->gauge_health>=in->gauge_max) d7=0x3000;
        else d7=(uint16_t)((uint32_t)in->gauge_health*0x3000u/in->gauge_max);
        uint16_t old=q->f7e;q->f7e=d7;
        if (d7!=old) {if ((int16_t)(d7-old)<0) q->f89|=8; else q->f89&=(uint8_t)~8;}
        if (q->f89&8) {
            int16_t d0=(int16_t)(q->f7e-q->f7c);
            if (d0) {
                int16_t d1=(int16_t)(d0>>1);uint16_t x=(uint16_t)(d0&1);
                d0=(int16_t)(d0+d1+x);x=(uint16_t)(d1&1);d1=(int16_t)(d1>>1);
                d0=(int16_t)(d0+d1+x);q->f7c=(uint16_t)(q->f7c+d0);
            }
        } else {
            int16_t d0=(int16_t)(q->f7e-q->f7c);
            if (!d0) {if (q->f7e>=0x1800) return;}
            else {
                if (d0>0x200) d0=0x200; else if (d0<-0x200) d0=-0x200;
                q->f7c=(uint16_t)(q->f7c+d0);
            }
        }
    }
    uint16_t a=(uint16_t)(q->f76+1),attr=0x6b00;
    if (q->f7e && q->f7e<0x1800 && !(in->video&7)) attr=0x7b00;
    int16_t d1=(int16_t)q->f7c;if (d1<0) d1=0;
    poke(h,a,attr|0xa5);a=(uint16_t)(a+0x20);
    unsigned d2=6;
    for (;;) {
        d1=(int16_t)(d1-0x800);
        if (d1>=0) {poke(h,a,attr|0xae);a=(uint16_t)(a+0x20);if (!--d2) break;continue;}
        poke(h,a,(uint16_t)(attr|(uint8_t)(0xae + (int8_t)(d1>>8))));a=(uint16_t)(a+0x20);
        if (!--d2) break;
        while (d2) {poke(h,a,attr|0xa6);a=(uint16_t)(a+0x20);d2--;}
        break;
    }
    poke(h,a,attr|0xaf);
}
/* 0C9D08: prisoner stars, 14 at most. */
static void stars(neo_hud_t *h,neo_panel_t *q,uint16_t rescued) {
    uint16_t d7=rescued,d6=14;
    if (d7>=d6) d7=d6;
    if ((uint8_t)d7==q->f8b) return;
    q->f8b=(uint8_t)d7;d6=(uint16_t)(d6-d7);
    uint16_t a=q->f6e?0x749c:0x707c;int step=q->f6e?-0x20:0x20;
    for (;d7;d7--,a=(uint16_t)(a+step)) poke(h,a,0x0310);
    for (;d6;d6--,a=(uint16_t)(a+step)) poke(h,a,0x0320);
}
void neo_hud_panel_play(neo_hud_t *h,unsigned p,const neo_hud_input_t *in) {
    neo_panel_t *q=&h->panel[p];
    if (!in->hud_on) {q->f86=0;return;}            /* 0C965E -> 0C983E */
    if (!q->f86) {                                   /* 0C966A */
        panel_clear(h,q,in->hud_on);
        static const char *const panels[4]={s_panel_p1,s_panel_1p2,s_panel_p2,s_panel_2p2}; /* 0C92B6 */
        unsigned i=(in->two_players?1u:0u)|(q->f6e?2u:0u);
        text(h,q->f76,0,panels[i],false);
        text(h,q->f78,0,s_box,false);
        q->f89|=4;q->f8b=0xff;q->f7c=0;
    }
    /* 0C96BE: lives. */
    uint16_t d0=(uint16_t)(q->f76+0xa0+(in->two_players?0:2));
    if ((int8_t)q->f84<0) text(h,d0,0x103,s_infinite,false);
    else number_skip(h,d0,0x103,q->f84,0,2);
    /* 0C96FC: score. */
    if (!in->two_players) bcd(h,(uint16_t)(q->f76+(q->f6e?0:0x20)),3,in->score,(uint8_t)(in->score[0]&0xf0),7);
    /* 0C9738: ARMS. */
    if (in->arms==0xffff) text(h,(uint16_t)(q->f78+0x21),0x200,s_infinite,false);
    else {
        uint16_t d1=0x203;
        if (in->arms!=q->f7a) {q->f7a=in->arms;q->f89^=1;if (q->f89&1) d1=0x303;}
        else q->f89&=(uint8_t)~1;
        number(h,(uint16_t)(q->f78+0x21),d1,in->arms,0,3);
    }
    /* 0C979E: BOMB label and count. */
    if (in->bomb_type<4) {
        if (!(q->f89&4)) {q->f89|=4;text(h,(uint16_t)(q->f78+0x80),0,s_bomb,false);}
    } else if (q->f89&4) {
        q->f89&=(uint8_t)~4;text(h,(uint16_t)(q->f78+0x80),0,in->bomb_type==6?s_other:s_cannon,false);
    }
    {
        uint16_t d1=0x203;
        if (in->bombs!=q->f8a) {q->f8a=in->bombs;q->f89^=1;if (q->f89&1) d1=0x303;}
        else q->f89&=(uint8_t)~1;
        number(h,(uint16_t)(q->f78+0xa1),d1,in->bombs,0,2);
    }
    gauge(h,q,in);                                   /* 0C9836 */
    stars(h,q,in->rescued);                          /* 0C983A */
    q->f86=0xff;                                     /* 0C983E */
}
/* 0C9228: the panel's blink counter: 1 show, -1 clear, 0 nothing. */
static int blink(neo_hud_t *h,unsigned p) {
    uint8_t d0=h->blink[p]++;d0&=0x3f;
    return !d0?1:d0==0x30?-1:0;
}
/* 0C927A: clears the panel's text (11 columns of two rows). */
static void text_clear(neo_hud_t *h,neo_panel_t *q,bool hud_on) {
    if (!hud_on) return;
    q->f86=0;fill(h,(uint16_t)(q->f74+(q->f6e?0x80:0x20)),0x0320,11,2);
}
void neo_hud_panel_idle(neo_hud_t *h,unsigned p,const neo_hud_input_t *in) {
    neo_panel_t *q=&h->panel[p];
    if (in->hud_on) {
        if (!(in->join_mask>>p&1)) {                 /* 0C9378 -> 0C9B38 */
            if (in->two_players) return;
            q->f86=0xff;
            int r=blink(h,p);
            if (r>0) text(h,(uint16_t)(q->f74+(q->f6e?0x80:0x20)),0,s_please_wait,true);
            else if (r<0) text_clear(h,q,true);
            return;
        }
        uint8_t c=in->credits_available?0xff:0;      /* 0C9380 */
        if (c!=q->f85) {q->f85=c;h->blink[p]=0;}
        int r=blink(h,p);
        if (r>0) {
            const char *s=q->f85?s_press_start:in->mvs?s_insert_coin:s_game_over;
            text(h,(uint16_t)(q->f74+(q->f6e?0x80:0x20)),0,s,true);
        } else if (r<0) text_clear(h,q,true);
    }
    q->f86=in->hud_on?0xff:0;                        /* 0C93FC */
}
void neo_hud_timer_draw(neo_hud_t *h,bool hud_on,uint16_t d0) {
    if (!hud_on) return;
    if (d0>=0x6400) {block(h,0x7243,0x5904,4,2);return;}
    if (d0<0xa00 && (d0&0xc)==0xc) {fill(h,0x7243,0x00ff,4,2);return;}
    unsigned s=d0>>8;
    block(h,0x7243,timer_digit[s/10],2,2);
    block(h,0x7283,timer_digit[s%10],2,2);
}
void neo_hud_timer_step(neo_hud_t *h,const neo_hud_input_t *in) {
    h->sound=0;
    neo_hud_timer_draw(h,in->hud_on,h->timer);        /* 0186FA */
    if (!in->timer_running || h->timer==0xffff || h->time_over) return;
    uint16_t d0=(uint16_t)(h->timer-2);
    if (h->timer<=2) {                                 /* 01876C */
        h->timer=0;neo_hud_timer_draw(h,in->hud_on,0);
        h->time_over=true;  /* 0C0A2C: the caller kills the player (gameplay.c) */
        return;
    }
    h->timer=d0;
    if (d0<0xa00 && (uint8_t)(d0<<1)+4>0xff) h->sound=0x10d7; /* 018792 */
}
void neo_hud_credit(neo_hud_t *h,const neo_hud_input_t *in) {
    if (in->two_players || !in->credit_text) return;
    /* 01A7F4 with RAM 100003 clear: one counter at 737D. */
    uint16_t d0=text(h,0x737d,3,in->credits>=2?s_credits:s_credit,false);
    uint8_t c=in->credits;
    bcd_skip(h,(uint16_t)(d0-0x40),3,&c,2);
}
uint8_t neo_hud_timer_start(uint8_t dip) {
    static const uint8_t start[8]={0x3c,0x46,0x50,0x5a,0x5a,0x5a,0x5a,0x5a}; /* 00357E */
    return start[dip&7];
}
void neo_hud_timer_reset(neo_hud_t *h,uint8_t dip) {
    uint8_t d0=neo_hud_timer_start(dip);
    if (d0==0xff) return;
    h->timer=(uint16_t)(d0<<8|0xff);h->time_over=false;
}
bool neo_hud_life_lost(neo_hud_t *h,unsigned p) {
    neo_panel_t *q=&h->panel[p];
    if ((int8_t)q->f84<0) return true;
    if (!q->f84) return false;                       /* 0C95E6: game over 0C9848 */
    q->f84--;return true;
}
/* 27C4: player mode transition table 27B4 (row = mode, column = request). */
static uint8_t mode_request(uint8_t *mode,uint8_t request) {
    static const uint8_t table[16]={0,1,0,0, 1,1,2,3, 0,1,2,3, 0,3,3,3};
    return *mode=table[(*mode&3)*4+request];
}
/* 19008 / 1901A. */
static void continue_start(neo_hud_t *h,const neo_hud_input_t *in) { if (in->continue_dip) h->continue_timer=0x9ff; }
static uint16_t panel_text_address(const neo_panel_t *q) { return (uint16_t)(q->f74+(q->f6e?0x80:0x20)); }
/* 0C9ACE (entered at 0C9AC8 or from 0C9ABC). */
static void game_over_body(neo_hud_t *h,neo_panel_t *q,const neo_hud_input_t *in) {
    if (q->f80>>8) {stop(h,"game over delay 0C9AD6",q->f80);q->state=NEO_PANEL_STOPPED;return;}
    if (h->mode[0]==1 || h->mode[1]==1) {stop(h,"rejoin 0C9312",q->f6e);q->state=NEO_PANEL_STOPPED;return;}
    q->f86=in->hud_on?0xff:0;                        /* 0C9AF6 */
    if (in->hud_on) text(h,panel_text_address(q),0,s_game_over,true);
    q->f86=in->hud_on?0xff:0;q->f8d=0;               /* 0C9B2A, 0C9B30 */
}
/* 0C99E6: the player's game over. */
static void game_over_enter(neo_hud_t *h,neo_panel_t *q,const neo_hud_input_t *in) {
    uint8_t r=mode_request(&h->mode[q->f6e&1],3);
    if (r==0) {stop(h,"panel 0C9312",q->f6e);q->state=NEO_PANEL_STOPPED;return;}
    if (r==1) {stop(h,"new game 0C9406",q->f6e);q->state=NEO_PANEL_STOPPED;return;}
    if (r==2) {neo_hud_game_over(h,q->f6e,in);return;}
    panel_clear(h,q,in->hud_on);                     /* 0C9A0E */
    /* 0C9A12..0C9A3E: the ranking check 057B44 (name entry actor 058132 when
     * the score qualifies) is not ported: explicit stop, the path of a score
     * that does not qualify (the capture's) goes on. */
    if (!in->two_players) stop(h,"ranking 057B44",q->f6e);
    q->f88=3;q->f80=0x1e;q->state=NEO_PANEL_GAME_OVER; /* 0C9ABC..0C9AC8 */
    game_over_body(h,q,in);
}
/* 0C9918: one update of the continue countdown. */
static void continue_body(neo_hud_t *h,neo_panel_t *q,const neo_hud_input_t *in) {
    uint8_t r=mode_request(&h->mode[q->f6e&1],2);
    if (r==0) {stop(h,"panel 0C9312",q->f6e);q->state=NEO_PANEL_STOPPED;return;}
    if (r==3) {game_over_enter(h,q,in);return;}
    q->f88=r;
    if (in->credit_count>q->f8c) {q->f8c=in->credit_count;continue_start(h,in);} /* 0C993E */
    if (!h->continue_timer) {game_over_enter(h,q,in);return;}                    /* 0C9950 */
    if (!(in->join_mask>>(q->f6e&1)&1)) {stop(h,"panel 0C9B38",q->f6e);q->state=NEO_PANEL_STOPPED;return;}
    if (in->hud_on) {
        if (!q->f86) text(h,panel_text_address(q),0,s_continue,true);            /* 0C9970 */
        unsigned d=h->continue_timer>>8;
        if (d>9) {stop(h,"continue digit",d);d=9;}
        block(h,(uint16_t)(panel_text_address(q)+0x140),continue_digit[d],1,2);   /* 0C99A2 */
    }
    q->f86=in->hud_on?0xff:0;q->f8d=0;               /* 0C99DC, 0C9B30 */
}
void neo_hud_game_over(neo_hud_t *h,unsigned p,const neo_hud_input_t *in) {
    neo_panel_t *q=&h->panel[p&1];
    if (!in->continue_dip || in->two_players) {game_over_enter(h,q,in);return;}
    q->f8c=in->credit_count;                         /* 0C985E */
    if (!in->can_continue && in->mvs) {              /* 0C987A: no credit */
        if (in->can_continue) continue_start(h,in); else h->continue_timer=0;
        game_over_enter(h,q,in);return;
    }
    /* 0C98A2: 019798 creates the CONTINUE? actor (008CD58, the big sprite text
     * and digit) at the respawn point 0196E8: not ported, explicit stop; the
     * fix-layer text of this panel is. */
    stop(h,"continue actor 019798",p);
    panel_clear(h,q,in->hud_on);
    continue_start(h,in);
    q->state=NEO_PANEL_CONTINUE;
    continue_body(h,q,in);
}
/* 0C98CC: the continue panel's update. */
static void continue_update(neo_hud_t *h,neo_panel_t *q,const neo_hud_input_t *in) {
    if (in->start) h->mode[q->f6e&1]=1;               /* the BIOS start of the player */
    if (h->mode[q->f6e&1]!=1) {continue_body(h,q,in);return;}
    /* 0C98DE: the continue: the score's last digit counts continues (max 9). */
    uint8_t *last=&h->score[3];
    uint8_t d1=(uint8_t)((*last&15)+1);if (d1>=10) d1=9;
    *last=(uint8_t)((*last&0xf0)|d1);
    h->continues[q->f6e&1]++;                         /* 0C8CB2 */
    /* 0C9420: c9068 (attract counters), the panel cleared, the lives of the
     * DIP (1 when 0), the blink counter, player mode 1. The new player actor
     * (0C94E6..) is the caller's new life. */
    panel_clear(h,q,in->hud_on);
    q->f84=in->lives_dip?in->lives_dip:1;
    h->blink[q->f6e&1]=0;q->f88=1;
    /* 0C94D6..0C94FC: the join graphic (c9216, fix 7063..) and the new player
     * actor 0C9F2C: the graphic is not ported (explicit stop); the port's
     * play panel redraws in the next update and the caller makes the life. */
    stop(h,"continue join 0C94D6",q->f6e);
    q->state=NEO_PANEL_PLAY;h->continued=true;
}
void neo_hud_continue_step(neo_hud_t *h,const neo_hud_input_t *in) {
    uint16_t t=h->continue_timer;
    if (!t) return;                                   /* 0187A4 */
    uint16_t d0;
    if (t<0x780 && in->speedup && h->mode[0]==2) {     /* 0187B0: 190F0 bit 5 */
        d0=(uint16_t)((t&0xff00)|0x80);d0=d0>=0x100?(uint16_t)(d0-0x100):0;
    } else d0=t>=5?(uint16_t)(t-5):0;
    h->continue_timer=d0;
    if (in->continue_dip && d0 && h->mode[0]!=1 && h->mode[1]!=1 && in->join_mask && (uint8_t)(t>>8)!=(uint8_t)(d0>>8))
        h->sound=0x10d4;                              /* 0187F0..018836 (10E9E3 clear) */
}
void neo_hud_frame(neo_hud_t *h,const neo_hud_input_t *in) {
    h->continued=false;
    memcpy(h->score,in->score,4);
    neo_panel_t *q=&h->panel[0];
    if (in->no_life && q->state==NEO_PANEL_PLAY) neo_hud_game_over(h,0,in);
    else if (q->state==NEO_PANEL_CONTINUE) continue_update(h,q,in);
    else if (q->state==NEO_PANEL_GAME_OVER) game_over_body(h,q,in);
    else if (q->state==NEO_PANEL_PLAY) neo_hud_panel_play(h,0,in);
    neo_hud_panel_idle(h,1,in);
    neo_hud_credit(h,in);
    neo_hud_timer_step(h,in);
    uint16_t beep=h->sound;neo_hud_continue_step(h,in);if (!h->sound) h->sound=beep;
}

/* 0AAA88: kind of the vehicle the player rides (0 = none). */
static uint8_t ridden(const uint8_t *slot) {
    return slot && (slot[0]|slot[1])?slot[0x21]:0;
}
uint16_t neo_hud_arms(const uint8_t *block,const uint8_t *slot,uint8_t dip10f001,bool two_players,unsigned p) {
    uint8_t k=ridden(slot);
    if (k && k!=5) return 0xffff;
    if (dip10f001&8 || two_players) return 0xffff;
    return (uint16_t)(block[12+2*p]<<8|block[13+2*p]);
}
void neo_hud_bombs(const uint8_t *block,const uint8_t *slot,uint16_t input_word,uint8_t flags15,unsigned p,uint8_t *count,uint8_t *type) {
    uint8_t k=ridden(slot);bool own;
    if (!k) own=true;
    else if (k==1 || k==8) own=(input_word&2) && (flags15&0x40);
    else own=k==2||k==5||k==6||k==7;
    if (own) {*count=block[8+(p&1)];*type=block[10+(p&1)];}
    else {*count=slot[0x1f];*type=slot[0x20];}
}
