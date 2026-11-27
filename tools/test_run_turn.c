/* B84 (docs/BUG_MATRIX.md): the run's turn in the game, gameplay.c with the
 * visual layer fed back as actor.c does (marco_visual_turn_gate after each
 * player update). The inputs of MAME 0.264 marco-run-turn, pistol
 * (data/run_turn_fixture.json, segment rt0: stick and A presses per
 * update): every press must give a shot exactly when the original took one
 * (0C2746 sets the weapon state 0C381C: the presses of the turn's two
 * updates are no shot). Mission 1 from its start, invulnerable. */
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "../src/gameplay.h"
#include "../src/marco_visual.h"
#include "run_turn_fixture.h"
static msx_game_t g;static marco_visual_t v;
static void tick(int dir,bool fire) {
    msx_game_update(&g,dir,false,fire);g.invulnerable=2;g.world.sound_count=0;
    if (!(g.ticks&1)) {
        if (g.life.phase==MARCO_ALIVE && g.mount!=MSX_MOUNT_RIDE) {
            static unsigned shots;
            marco_visual_input_t mi={g.grounded,g.moving,g.crouching,g.aim_up,g.shots!=shots,g.aim_down,g.jump_moving,g.facing,0,0,
                g.world.weapon_player.form93<=3u?g.world.weapon_player.form93:0u,-1,false,false,false,false,0,false,false,false,false,false};
            shots=g.shots;marco_visual_step(&v,&mi);g.turn_gate=marco_visual_turn_gate(&v);
        } else g.turn_gate=(marco_turn_gate_t){false,false,0};
    }
}
int main(void) {
    msx_game_init(&g);marco_visual_reset(&v);
    for (unsigned s=0;s<4000 && !(g.grounded && g.life.phase==MARCO_ALIVE && !g.descent && g.ticks>400);s++) tick(0,false);
    assert(g.grounded && g.life.phase==MARCO_ALIVE);
    while (g.ticks&1) tick(0,false);
    const run_turn_segment_t *seg=&run_turn_segments[0];assert(seg->form==0 && !seg->cgun);
    unsigned n=seg->rows,presses=0,diff=0,blocked0=g.turn_blocked;
    static unsigned shot_at[1024];
    for (unsigned k=0;k<n;k++) {
        const run_turn_row_t *r=&run_turn_rows[k];int dir=r->moving?r->facing:0;
        unsigned before=g.shots;
        /* The press of the frame before the update (the plan's A on an even
         * frame, read at the odd one): the first tick of the update. */
        tick(dir,r->pressed);tick(dir,false);
        shot_at[k]=g.shots-before;
    }
    for (unsigned k=0;k<n;k++) {
        const run_turn_row_t *r=&run_turn_rows[k];if (!r->pressed) continue;
        presses++;unsigned j=k;bool shot=false;
        do {shot|=shot_at[j]!=0;j++;} while (!shot && j<n && !run_turn_rows[j].pressed);
        if (shot==(bool)r->lost && diff++<4) printf("B84 game frame %u: port shot %d, MAME lost %d\n",r->frame,shot,r->lost);
    }
    printf("B84 game: %u presses of marco-run-turn (pistol), %u dropped by the turn, %u differences\n",presses,g.turn_blocked-blocked0,diff);
    assert(!diff);
    puts("PASS: B84 a press in the run's turn is no shot in the game (gameplay.c with the visual gate, MAME marco-run-turn)");
    return 0;
}
