#ifndef NEO_HUD_H
#define NEO_HUD_H
/* The fix-layer HUD of the original (docs/ARMAS_VEHICULOS.md §11): a model of
 * the Neo Geo fix map (VRAM 7000..74FF, 40 columns of 32 rows, word =
 * palette<<12 | S-ROM tile) written by native translations of the game's own
 * routines: the player panels 0C92C6 (play state 0C9650..0C983E, gauge
 * 0C9B94, prisoner stars 0C9D08, the idle panel's blinking text 0C9380), the
 * timer digits 0C8FDC of the mission actor 101364 (+7A; count-down 0186FA..
 * 0187A2, reset by the script spawn 01913C) and the credit text 01A7F4.
 * Their text services 03CF12/03CF60/03CF9A/03D004/03D050/03D068/03D0D0/
 * 03D244/03D25A(03DD96) write the map as the original writes VRAM.
 * Pure C: host tests replay it against fix-layer VRAM dumps of MAME. */
#include <stdbool.h>
#include <stdint.h>

#define NEO_FIX_COLUMNS 40
#define NEO_FIX_ROWS 32
#define NEO_FIX_WORDS (NEO_FIX_COLUMNS*NEO_FIX_ROWS)

/* Fields of a player panel actor (101204 P1 / 1012B4 P2) the HUD uses. */
typedef struct {
    uint16_t f6e;              /* player index */
    uint16_t f74,f76,f78;      /* fix addresses (0C92C6) */
    uint16_t f7a;              /* last ARMS value */
    uint16_t f7c,f7e;          /* gauge: shown / target (0C9B94) */
    uint8_t f84;               /* lives left (signed: <0 = infinite symbol) */
    uint8_t f85;               /* credits available (0C938A) */
    uint8_t f86;               /* HUD flag of the previous update (10E9E1 copy) */
    uint8_t f89;               /* bit 0 digit blink, bit 2 BOMB label, bit 3 gauge falling */
    uint8_t f8a,f8b;           /* last BOMB value, last star count */
    uint8_t f88;               /* player mode copy (27C4 result) */
    uint8_t f8c;               /* credits seen by the continue (1A872) */
    uint8_t f8d;               /* cleared by 0C9B30 */
    uint16_t f80;              /* GAME OVER delay word (0C9AC2) */
    uint8_t state;             /* NEO_PANEL_*: which routine (a6) runs */
} neo_panel_t;
enum { NEO_PANEL_PLAY, NEO_PANEL_CONTINUE, NEO_PANEL_GAME_OVER, NEO_PANEL_STOPPED };

typedef struct {
    uint16_t fix[NEO_FIX_WORDS];  /* index column*32+row = VRAM address - 7000 */
    neo_panel_t panel[2];
    uint8_t blink[2];             /* RAM 10E9DA+p (0C9228) */
    uint16_t timer;               /* RAM 1013DE (mission actor 101364 +7A) */
    uint32_t serial;              /* incremented by every map write */
    uint16_t sound;               /* last sound request (0x10D7 timer beep), 0 none */
    bool time_over;               /* 0C0A2C reached (the caller kills the player) until 019124 */
    unsigned unsupported;         /* explicit stops (unsupported escapes or glyphs) */
    uint16_t continue_timer;      /* RAM 1013E2 (mission actor +7E), 19008/1901A/19022 */
    uint8_t mode[2];              /* BIOS player mode 10FDB6+p (0 none, 1 play, 2 continue, 3 over) */
    uint16_t continues[2];        /* RAM 10E9E4+2p (0C8CB2) */
    bool continued;               /* set by the update in which 0C98CC took the continue */
    uint8_t score[4];             /* copy of RAM 10C46A: the continue's digit 0C98DE is
                                   * written here and copied back by the caller */
} neo_hud_t;

/* Inputs of one update: RAM the routines read. */
typedef struct {
    bool hud_on;                /* RAM 10E9E1 */
    bool two_players;           /* RAM 10C2E7 != 0 (never in the port) */
    uint16_t arms;              /* 0C071C (FFFF: infinite) */
    uint8_t bombs,bomb_type;    /* 0C0764 D0/D1 */
    uint8_t score[4];           /* RAM 10C46A (BCD) */
    uint16_t rescued;           /* RAM 10C5FC */
    bool gauge;                 /* 0AAAE0 carry (riding a vehicle with a gauge) */
    uint16_t gauge_health,gauge_max; /* 0AAAE0 D7/D6 */
    uint16_t video;             /* RAM 10000E frame counter */
    uint8_t join_mask;          /* RAM 10F003 (0C9378) */
    bool mvs;                   /* RAM 10FD82 != 0 (0C93C2) */
    bool credits_available;     /* 01A8AA (0C9384) */
    uint8_t credits;            /* BCD credits (01A7F4) */
    bool credit_text;           /* RAM 10C2E6 and 10FD91 bit 1 (016A62/00354E) */
    bool timer_running;         /* 018704..01875A: a player in play, 4303 bit 7,
                                 * 10E9E3 clear, 10F003 nonzero */
    /* Game over / continue (0C9848, 0C9918, 0187A4): */
    bool continue_dip;          /* RAM 10FD8A (game DIP: continue allowed) */
    bool can_continue;          /* 1A8AA for the player (BIOS CREDIT_CHECK on MVS) */
    uint8_t credit_count;       /* 1A872: credits of the player's coin slot */
    bool start;                 /* the BIOS took a start: player mode 10FDB6+p = 1 */
    bool speedup;               /* 190F0 D1 bit 5 (new press of B) while the mode is 2 */
    uint8_t lives_dip;          /* RAM 10FD88 (0C942C) */
    bool no_life;               /* 0C95F6: this update's new life found no life left */
} neo_hud_input_t;

/* 0C071C: the ARMS value of player p: FFFF when riding a vehicle of a kind
 * other than 0 and 5 (0AAA88 on the slot 10C608+0x24p: word +00, kind +21),
 * with 10F001 bit 3 (cheat) or two players; else the rounds 10C724+2p.
 * block = RAM 10C718.., slot = NULL when the player does not exist. */
uint16_t neo_hud_arms(const uint8_t *block,const uint8_t *slot,uint8_t dip10f001,bool two_players,unsigned p);
/* 0C0764: BOMB count and type: the player's (10C720+p, 10C722+p) or, riding
 * a kind 1/8 vehicle (unless down is held, bit 1 of 10E9EC+1, with the player
 * on the ground, +15 bit 6) or a kind other than 2/5/6/7, the slot's +1F/+20. */
void neo_hud_bombs(const uint8_t *block,const uint8_t *slot,uint16_t input_word,uint8_t flags15,unsigned p,uint8_t *count,uint8_t *type);
/* The fix map as the mission starts (observed: 00FF in the visible frame,
 * 0000 on the border) and the panels of 0C92C6 for both players. */
void neo_hud_init(neo_hud_t *h,uint16_t timer);
/* One original frame: the P1 panel in play (0C9650), the P2 panel idle
 * (0C9340 with 10F003 bit 1: 0C9380), the credit text (016A52) and the
 * mission actor's timer (0186FA). */
void neo_hud_frame(neo_hud_t *h,const neo_hud_input_t *in);
/* 01913C: the timer back to its start value 003586 (DIP index 10FD8C). */
void neo_hud_timer_reset(neo_hud_t *h,uint8_t dip);
/* 003586: start value of the timer for the DIP index (0x3C,0x46,0x50,0x5A..). */
uint8_t neo_hud_timer_start(uint8_t dip);
/* The pieces, for the replay tests. */
void neo_hud_panel_play(neo_hud_t *h,unsigned p,const neo_hud_input_t *in); /* 0C9650 */
void neo_hud_panel_idle(neo_hud_t *h,unsigned p,const neo_hud_input_t *in); /* 0C9380 */
void neo_hud_timer_draw(neo_hud_t *h,bool hud_on,uint16_t value);        /* 0C8FDC */
void neo_hud_timer_step(neo_hud_t *h,const neo_hud_input_t *in);          /* 0186FA */
void neo_hud_credit(neo_hud_t *h,const neo_hud_input_t *in);              /* 01A7F4 */
/* 0C2FA4 path of the panel (0C95E6): one life less; false when none was left
 * (game over 0C9848: explicit stop in the port). */
bool neo_hud_life_lost(neo_hud_t *h,unsigned p);
/* 0C9848: the panel's game over (no lives left). Continue countdown
 * (0C98CC/0C9918, timer 19008) or GAME OVER (0C99E6 -> 0C9ACE). */
void neo_hud_game_over(neo_hud_t *h,unsigned p,const neo_hud_input_t *in);
/* 0187A4..0187EC: the mission actor's continue timer step (after 0186FA). */
void neo_hud_continue_step(neo_hud_t *h,const neo_hud_input_t *in);
#endif
