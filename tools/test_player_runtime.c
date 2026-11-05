#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "../src/player_runtime.h"
#include "../src/neo_collision.h"

static void start(msx_game_t *game,player_runtime_t *player) {
    msx_game_init(game);game->enemy.alive=false;
    assert(player_runtime_start(player,game));
    assert(player->campaign.ticks==1 && player->campaign.draw_count);
    assert(player->campaign.draws[0].attributes>>8==0x63);
    assert(player->campaign.draws[0].z==0x86d6);
    assert(player->campaign.common.projected_y==
        (int16_t)(256+(player->world.contact.projected_y>>16)));
    assert(player->campaign.common.death_line==256);
    assert(game->player_hitbox.valid && game->player_hitbox.width==16 && game->player_hitbox.height==28);
    assert(game->player_hitbox.x==game->x/MSX_UNIT-8);
    assert(game->player_hitbox.y==248-(player->world.contact.projected_y>>16)-32);
}
static void tick(msx_game_t *game,player_runtime_t *player,msx_game_input_t input) {
    msx_game_update_controlled(game,&input,player_runtime_step,player);
    player_runtime_after_world(player,game);
    assert(!game->player_failed);
    assert(game->x==player->campaign.body.motion.player.x);
    assert(game->y==56*MSX_UNIT-player->campaign.body.motion.player.y);
}
static void live_input_and_cadence(void) {
    msx_game_t game;player_runtime_t player;start(&game,&player);
    int initial=game.x;
    tick(&game,&player,(msx_game_input_t){.direction=1});
    assert(player.campaign.ticks==1 && game.x==initial);
    tick(&game,&player,(msx_game_input_t){.direction=1});
    assert(player.campaign.ticks==2 && game.x>initial);
    for(unsigned i=0;i<38;++i) tick(&game,&player,(msx_game_input_t){.direction=1});
    assert(player.campaign.ticks==21 && game.x>initial+40*MSX_UNIT);
    assert(player.footprint_writes>1);
    assert(player.world_policy.policy==MARCO_CAMPAIGN_WORLD_POLICY_DEFAULT);
    assert(player.world_policy.words[0]==28 && player.world_policy.words[2]==264);
    assert(!player.pending_environment_effects && !player.pending_visual_edits);

    start(&game,&player);
    /* One VI of jump survives the ordinary joypad's alternate-update latch. */
    tick(&game,&player,(msx_game_input_t){.jump=true});
    tick(&game,&player,(msx_game_input_t){0});
    assert(!game.grounded && game.velocity_y<0);
    for(unsigned i=0;i<160 && !game.grounded;++i)
        tick(&game,&player,(msx_game_input_t){0});
    assert(game.grounded);
    for(unsigned i=0;i<20;++i) tick(&game,&player,(msx_game_input_t){.down=true});
    assert(game.crouched && player.campaign.body.domain==MARCO_BODY_CROUCH);
    assert(game.player_hitbox.valid && game.player_hitbox.width==19 && game.player_hitbox.height==14);
}
static void native_projectile_world_lifetime(void) {
    msx_game_t game;player_runtime_t player;start(&game,&player);
    tick(&game,&player,(msx_game_input_t){.fire=true});
    tick(&game,&player,(msx_game_input_t){.fire=true});
    assert(game.bullets[0].active && game.bullets[0].native && player.projectile_owned[0]);
    assert(game.bullets[0].born_tick==2 && game.bullets[0].vx==16*MSX_UNIT);
    int32_t born_x=game.bullets[0].world_x;
    uint16_t token;
    assert(!player_runtime_pop_sound(&player,&token));
    tick(&game,&player,(msx_game_input_t){0});
    assert(game.bullets[0].world_x==born_x);
    tick(&game,&player,(msx_game_input_t){0});
    assert(game.bullets[0].world_x==born_x+16*MSX_UNIT);
    assert(player_runtime_pop_sound(&player,&token) && token==0x10f7);
    assert(!player_runtime_pop_sound(&player,&token));
    for(unsigned i=0;i<100 && game.bullets[0].active;++i)
        tick(&game,&player,(msx_game_input_t){0});
    assert(!game.bullets[0].active && !player.projectile_owned[0]);

    /* The world can retire the child during its first actor call. Its sound
     * still belongs to the successful real allocation, before retirement. */
    start(&game,&player);
    tick(&game,&player,(msx_game_input_t){.fire=true});
    tick(&game,&player,(msx_game_input_t){.fire=true});
    tick(&game,&player,(msx_game_input_t){0});
    const msx_game_input_t neutral={0};
    msx_game_update_controlled(&game,&neutral,player_runtime_step,&player);
    game.bullets[0].active=false;
    player_runtime_after_world(&player,&game);
    assert(player_runtime_pop_sound(&player,&token) && token==0x10f7);
    assert(!player.projectile_owned[0]);
}
static void actual_allocator_rejection(void) {
    msx_game_t game;player_runtime_t player;start(&game,&player);
    for(unsigned i=0;i<MSX_BULLETS;++i)
        game.bullets[i]=(msx_bullet_t){.active=true};
    const msx_game_input_t fire={.fire=true,.pressed=MARCO_BUTTON_FIRE};
    game.ticks=2;
    assert(player_runtime_step(&player,&game,&fire));
    for(unsigned i=0;i<MSX_BULLETS;++i) assert(!player.projectile_owned[i]);
    assert(!player.campaign.body.motion.player.upper.combo95);
    assert(player.pistol.ammo.ammunition==65535);
    uint16_t token;assert(!player_runtime_pop_sound(&player,&token));
}
static void live_aim_reaches_world_projectile(void) {
    msx_game_t game;player_runtime_t player;start(&game,&player);
    for(unsigned i=0;i<12;++i) tick(&game,&player,(msx_game_input_t){.up=true});
    tick(&game,&player,(msx_game_input_t){.up=true,.fire=true});
    tick(&game,&player,(msx_game_input_t){.up=true});
    assert(game.bullets[0].active && game.bullets[0].angle==64);
    assert(!game.bullets[0].vx && game.bullets[0].vy==16*MSX_UNIT);
}
static void vertical_shots_release_native_slots(void) {
    msx_game_t game;player_runtime_t player;start(&game,&player);
    unsigned sounds=0;
    /* Repeated real input must keep allocating shots after the first eight
     * upward projectiles leave the viewport at a stationary camera. */
    for(unsigned i=0;i<1200;++i) {
        tick(&game,&player,(msx_game_input_t){.up=true,.fire=i>=12 && i%40<2});
        uint16_t token;
        while(player_runtime_pop_sound(&player,&token)) if(token==0x10f7) ++sounds;
    }
    assert(sounds>=20 && !game.camera);
    for(unsigned i=0;i<MSX_BULLETS;++i)
        assert(!game.bullets[i].active && !player.projectile_owned[i]);
}
static void authored_relocation_refreshes_native_support(void) {
    msx_game_t game;player_runtime_t player;
    assert(msx_game_begin_at(&game,128*MSX_UNIT,96*MSX_UNIT));
    game.enemy.alive=false;assert(player_runtime_start(&player,&game));
    assert((uint8_t)player.world.contact.kind_angle==0x40);
    neo_surface_t slope;
    assert(neo_surface_at(96*MSX_UNIT,88*MSX_UNIT,8*MSX_UNIT,8*MSX_UNIT,&slope));
    game.x=96*MSX_UNIT;game.y=56*MSX_UNIT-slope.height;
    player_runtime_after_world(&player,&game);
    assert(!game.player_failed);
    assert((uint8_t)player.world.contact.kind_angle==0x4a);
    assert(player.world.contact.cached_x==game.x && player.world.contact.cached_y==slope.height);
    assert(player.world.contact.mark==player.world.epoch && !player.world.contact.correction);
    assert(player.world.contact.word1e==(uint16_t)(0x8834-((uint32_t)slope.height>>16)));
    assert(player.campaign.body.motion.player.upper.input.attributes>>8==0x63);
    tick(&game,&player,(msx_game_input_t){.direction=1});
    tick(&game,&player,(msx_game_input_t){.direction=1});
    assert(game.velocity_x==190464); /* First step uses the new slope tangent. */

    /* An authored invalid grounded handoff must stop explicitly rather than
     * install coordinates with stale support and continue as if valid. */
    marco_campaign_world_t before=player.world;
    marco_crouch_t body=player.campaign.body.motion.player;
    game.x=128*MSX_UNIT;game.y=56*MSX_UNIT-97*MSX_UNIT;game.grounded=true;
    player_runtime_after_world(&player,&game);
    assert(game.player_failed && player.campaign.reason==0x013968);
    assert(!memcmp(&before,&player.world,sizeof(before)));
    assert(!memcmp(&body,&player.campaign.body.motion.player,sizeof(body)));
}
static void live_proximity_selects_actual_melee(void) {
    msx_game_t game;player_runtime_t player;msx_game_init(&game);
    game.enemy.x=game.x/MSX_UNIT+26;game.enemy.cooldown=1000;
    assert(player_runtime_start(&player,&game));
    assert(player.campaign.body.motion.player.upper.input.flags9a&1u);
    for(unsigned i=0;i<20 && !game.kills;++i)
        tick(&game,&player,(msx_game_input_t){.fire=true});
    assert(game.kills==1 && game.hits==1 && !game.enemy.alive);
    assert(player.melee_requests==1 && player.last_melee_request.damage==30);
    assert(player.last_melee_request.type==2 && player.last_melee_request.target_class==5);
    for(unsigned i=0;i<MSX_BULLETS;++i) assert(!game.bullets[i].active);
    uint16_t token;
    assert(player_runtime_pop_sound(&player,&token) && token==0x10fb);
}
static void boundary_and_unsupported_are_coherent(void) {
    msx_game_t game;player_runtime_t player;start(&game,&player);
    for(unsigned i=0;i<100;++i) tick(&game,&player,(msx_game_input_t){.direction=-1});
    assert(game.x==28*MSX_UNIT);
    assert(player.world.contact.x==game.x && !player.world.contact.vx);
    assert(player.campaign.body.motion.player.x==game.x);

    start(&game,&player);
    const msx_game_input_t grenade={.grenade=true};
    msx_game_update_controlled(&game,&grenade,player_runtime_step,&player);
    msx_game_update_controlled(&game,&grenade,player_runtime_step,&player);
    assert(game.player_failed && player.campaign.status==MARCO_CAMPAIGN_ANIMATION_EVENT);
    assert(player.campaign.reason==0x0c4786);
    unsigned ticks=game.ticks;
    msx_game_update_controlled(&game,&grenade,player_runtime_step,&player);
    assert(game.ticks==ticks && player.campaign.reason==0x0c4786);

    start(&game,&player);
    player.campaign.animation.field50_tag=0;
    player_runtime_after_world(&player,&game);
    assert(!game.player_hitbox.valid && !game.player_failed);
    player.campaign.animation.field50_tag=0xdeadbe;
    player_runtime_after_world(&player,&game);
    assert(game.player_failed && player.campaign.reason==0x004c24);
}
int main(void) {
    live_input_and_cadence();native_projectile_world_lifetime();
    actual_allocator_rejection();live_aim_reaches_world_projectile();
    vertical_shots_release_native_slots();authored_relocation_refreshes_native_support();
    live_proximity_selects_actual_melee();
    boundary_and_unsupported_are_coherent();
    puts("PASS: live native player input, cadence, projection, terrain, real projectile lifetime and explicit unsupported boundary");
    return 0;
}
