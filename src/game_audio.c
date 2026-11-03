#include <libdragon.h>
#include "game_audio.h"
#include "msx_audio.h"
#include "msx_sfx.h"
#include "msx_perf.h"
#include "msx_ramfs.h"
#include "scene_assets.h"
#include <malloc.h>
/* Event sources -> original request words (data/sound_map.json):
 *  - the port's pistol bullets (game->shots without the native H, C and
 *    down shots, whose constructors request their own sounds) -> 0x10F7
 *  - world sound list (animation 0x30, 0x10FA, 0x1027)  -> as requested
 *  - vehicle health lowered by a bullet (ROM 01BB34)     -> 0x108D
 *  - vehicle health below half / reaching zero (029AE4)  -> 0x1023
 *  - vehicle destruction burst, DESTROY frame 1 (029B14) -> 0x1033
 *  - mission end 0188B8 (gameplay.c): 0x0020, the mission complete jingle
 *    (bank 00: a music command, it replaces the mission music; one-shot,
 *    played on the intro voice, free during gameplay), and 0x1137, the
 *    voice (its own measured channel, msx_sfx_policy.h)
 *  - region end: the music stops (the arcade has no region end; its driver
 *    is reset with command 03 between scenes, which silences the music). */
enum { VOLUME_PERCENT=80 };
static wav64_t sounds[MSX_SFX_COUNT];
static bool loaded[MSX_SFX_COUNT],ready,music_ready,music_on,warned,active;
static msx_sfx_state_t policy;
static uint16_t music_word;
static bool open_music(uint16_t word);
static const char *music_name(uint16_t word);
static unsigned port_shots(const msx_game_t *g) {return g->shots-g->h_shots-g->c_shots-g->down_shots;}
static unsigned shots,vehicle_health,vehicle_frame,vehicle_state;
static bool complete;
static unsigned frames;
static unsigned mixer_of(unsigned channel) {return MSX_AUDIO_EFFECT_MIXER+channel;}
_Static_assert((int)MSX_SFX_CHANNELS<=(int)MSX_AUDIO_EFFECT_CHANNELS,"one mono mixer channel per driver channel");
/* The beam's sustained 10F8 (5 s window, 88 KB) is not
 * resident: the RAM budget of the scene cache (208 slots) has no room for
 * it. It plays from rom:/ like the music (docs/AUDIO_JUEGO.md, 11). */
/* With every effect resident (RAMFS_FILES 96: B34) and the death voice
 * 1123 (B30) the reference cartridge missed 9 KB of its scene cache floor
 * (3323 of 3332 KB): the mission complete voice 1137 (played at the
 * mission end, when the cache is idle) and the D's pickup voice 115A (rare)
 * play from rom:/ too (51 KB). */
/* The MISSION 2 START voice 1132 (36 KB, once per mission, while the
 * letters fly in) would leave the reference cartridges 1 slot under the
 * floor (209 slots before it): it plays from rom:/ too. */
/* B20: the truck's engine 10A5 is a long one-shot (11.58 s, 157 KB), not a
 * loop: no room in RAM, it plays from rom:/ as well. (The dome's collapse
 * 1026, 6.16 s, is rendered but has no policy entry: AUDIO_JUEGO.md 13.) */
/* B36: the pickup voices of the weapons 0E (115B) and 0F (1159), which no
 * Mission 1/2 item gives (B16), play from rom:/ too; with 1132 and 10A5 also
 * in the policy the L's 1130 (19 KB) left the M2 reference at 207 slots
 * (floor 208): it streams as well. B35: the molotov pickup voice 1156
 * (0C0A66[2], rare) streams too. */
/* B38: the Slugnoid (neo_slugnoid.c: its programs and the pilot's) left the
 * M2 reference 26 KB under the floor; with the pilot tables of the other
 * characters left out it was still 14 KB under: the vehicles' exit/death
 * 10E9 (37 KB; the SV-001's A+B exit, the Slugnoid's death with a rider)
 * streams from rom:/. */
static bool streamed(unsigned i) {
    uint16_t w=msx_sfx_defs[i].word;
    return w==0x10f8u || w==0x1137u || w==0x115au || w==0x1132u || w==0x10a5u
        || w==0x115bu || w==0x1159u || w==0x1130u || w==0x1156u || w==0x10e9u;
}
void game_audio_preload(void) {
    if (!msx_ramfs_init()) return;
    for (unsigned i=0;i<MSX_SFX_COUNT;i++) {
        if (streamed(i)) continue;
        char name[64];snprintf(name,sizeof(name),"audio/%s.wav64",msx_sfx_defs[i].name);
        msx_ramfs_preload(name);
    }
}
bool game_audio_init(void) {
    msx_sfx_reset(&policy);ready=false;
    /* Every effect resident in RAM (Expansion Pak): each WAV64 is read whole
     * from the ROM once here and decoded from ram:/ on every play, so no play
     * streams from the cartridge or opens a file. */
    unsigned count=0;
    heap_stats_t before;sys_get_heap_stats(&before);
    bool ram=msx_ramfs_init();
    for (unsigned i=0;i<MSX_SFX_COUNT;i++) {
        char name[64],path[72];snprintf(name,sizeof(name),"audio/%s.wav64",msx_sfx_defs[i].name);
        if (streamed(i)) snprintf(path,sizeof(path),"rom:/%s",name);
        else {
            if (!ram || !msx_ramfs_load(name)) {loaded[i]=false;debugf("MSX AUDIO not resident: %s\n",name);continue;}
            snprintf(path,sizeof(path),"ram:/%s",name);
        }
        wav64_open(&sounds[i],path);wav64_set_loop(&sounds[i],false);loaded[i]=true;count++;
    }
    heap_stats_t after;sys_get_heap_stats(&after);
    debugf("MSX AUDIO sfx resident=%u bytes (%u files) heap used %d -> %d of %d, free %d\n",(unsigned)msx_ramfs_bytes(),count,
        before.used,after.used,after.total,after.total-after.used);
    music_word=0;music_ready=open_music(0x0021);
    ready=count>0;active=true;
    debugf("MSX AUDIO game sounds=%u/%u music=%u\n",count,(unsigned)MSX_SFX_COUNT,music_ready);
    return ready && count==MSX_SFX_COUNT && music_ready;
}
/* Music words of the stage descriptors (+C, requested by 018434 only when
 * it differs from the mission actor's +80: Mission 2's stage 2 keeps 0x22;
 * MAME mus0: 0x23 at 26139, no 03 between stages) -> rendered files. */
static const char *music_name(uint16_t word) {
    switch (word) {
    case 0x0021: return "music_mission1";
    case 0x0022: return "music_mission2";
    case 0x0023: return "music_mission2_stage3";
    case 0x0027: return "music_mission2_boss";
    default: return NULL;
    }
}
/* Intro + loop body (tools/pack_game_audio.py); older packs have only the
 * whole file, looped whole. */
static bool open_music(uint16_t word) {
    const char *name=music_name(word);
    if (!name) return false;
    char intro[80],body[80];
    snprintf(intro,sizeof(intro),"rom:/audio/%s_intro.wav64",name);snprintf(body,sizeof(body),"rom:/audio/%s.wav64",name);
    bool ok=msx_audio_open(MSX_AUDIO_MUSIC,intro) && msx_audio_open_loop(MSX_AUDIO_MUSIC,body);
    if (!ok) ok=msx_audio_open(MSX_AUDIO_MUSIC,body);
    if (ok) music_word=word;
    return ok;
}
/* 018434 requests the word (0120F8) and the driver plays it: whatever
 * stopped the previous music (the mission complete jingle 0020 at the end
 * of Mission 1), the new stage's music starts. */
void game_audio_stage_music(uint16_t word) {
    if (!active || !word || word==music_word) return;
    msx_audio_stop(MSX_AUDIO_MUSIC);music_on=false;
    music_ready=open_music(word);
    debugf("MSX AUDIO stage music %04X %s\n",(unsigned)word,music_ready?"":"missing");
    if (music_ready) game_audio_music(true);
}
/* 1805C (a mission's first record): 01205E sends the driver reset 03
 * (every channel silent, the jingle cut) and 01829E sets the mission
 * actor's +80 = FFFF: the descriptor's music is requested even when it is
 * the one already playing. */
void game_audio_mission_start(void) {
    if (!active) return;
    for (unsigned c=0;c<MSX_SFX_CHANNELS;c++) if (policy.current[c]>=0) mixer_ch_stop(mixer_of(c));
    msx_sfx_reset(&policy);msx_audio_stop(MSX_AUDIO_INTRO);
    msx_audio_stop(MSX_AUDIO_MUSIC);music_on=false;music_word=0;complete=false;
}
void game_audio_music(bool play) {
    if (!active || !music_ready) return;
    if (play) music_on=msx_audio_play(MSX_AUDIO_MUSIC,true,VOLUME_PERCENT/100.0f);
    else {msx_audio_stop(MSX_AUDIO_MUSIC);music_on=false;}
    debugf("MSX AUDIO music %s\n",music_on?"start":"stop");
}
/* Logged as "MSX AUDIO voice" with the policy's verdict: the player's
 * voices, the MISSION n START voices and the long streamed effects (B20). */
static bool voice_word(uint16_t w) {return w==0x10a5u || w==0x1026u || w==0x1123u || w==0x1124u || (w>=0x1131u && w<=0x1136u) || (w>=0x112cu && w<=0x1130u) || (w>=0x1156u && w<=0x115bu);}
static void request(uint16_t word) {
    /* 012192 (the actors' stop request, word | 0x8000): the channel playing
     * that word stops (the beam's 10F8 at its end 01E4D4). */
    if (word&0x8000u) {
        for (unsigned c=0;c<MSX_SFX_CHANNELS;c++) if (policy.current[c]>=0 && msx_sfx_defs[policy.current[c]].word==(word&0x7fffu)) {
            if (voice_word(word&0x7fffu)) debugf("MSX AUDIO voice %04X: stopped\n",(unsigned)(word&0x7fffu));
            mixer_ch_stop(mixer_of(c));msx_sfx_finished(&policy,c);
        }
        return;
    }
    /* Bank 00 music words from actors (the Mission 2 boss's 059F76: 0x27)
     * replace the music; 0x0Axx is its fade command (01228C): the music
     * stops (the fade itself is not rendered). */
    if (word>=0x0021 && word<0x0100 && music_name(word)) {music_word=0;game_audio_stage_music(word);return;}
    if ((word&0xff00u)==0x0a00u) {msx_audio_stop(MSX_AUDIO_MUSIC);music_on=false;debugf("MSX AUDIO music fade %04X\n",(unsigned)word);return;}
    if (word==0x0020) {
        msx_audio_stop(MSX_AUDIO_MUSIC);music_on=false;
        bool ok=msx_audio_open(MSX_AUDIO_INTRO,"rom:/audio/music_mission_complete.wav64") &&
            msx_audio_play(MSX_AUDIO_INTRO,false,VOLUME_PERCENT/100.0f);
        debugf("MSX AUDIO mission complete jingle %s\n",ok?"start":"missing");
        return;
    }
    /* The player's voices (death 0C294E, weapon pickups 0C08F8, bombs
     * 0C0B60) are logged with the policy's verdict (B30/B34). */
    bool voice=voice_word(word);
    int before=-1,wanted=msx_sfx_lookup(word);
    if (wanted>=0) before=policy.current[msx_sfx_defs[wanted].channel];
    int channel=msx_sfx_request(&policy,word);
    if (voice) {
        if (wanted<0) debugf("MSX AUDIO voice %04X: unmapped\n",(unsigned)word);
        else if (channel<0) debugf("MSX AUDIO voice %04X: dropped under %04X\n",(unsigned)word,(unsigned)msx_sfx_defs[before].word);
        else if (before>=0) debugf("MSX AUDIO voice %04X: started, cut %04X\n",(unsigned)word,(unsigned)msx_sfx_defs[before].word);
        else debugf("MSX AUDIO voice %04X: started\n",(unsigned)word);
    } else if (channel>=0 && before>=0 && voice_word(msx_sfx_defs[before].word))
        debugf("MSX AUDIO voice %04X: cut by %04X\n",(unsigned)msx_sfx_defs[before].word,(unsigned)word);
    if (channel<0) return;
    int id=policy.current[channel];
    if (!loaded[id]) {
        if (voice) debugf("MSX AUDIO voice %04X: not resident\n",(unsigned)word);
        if (!warned) debugf("MSX AUDIO missing rom:/audio/%s.wav64\n",msx_sfx_defs[id].name);
        warned=true;msx_sfx_finished(&policy,(unsigned)channel);return;
    }
    unsigned mixer=mixer_of((unsigned)channel);
    mixer_ch_stop(mixer);wav64_play(&sounds[id],mixer);
    float volume=VOLUME_PERCENT/100.0f;mixer_ch_set_vol(mixer,volume,volume);
}
void game_audio_sync(msx_game_t *g) {
    if (!active) return;
    for (unsigned c=0;c<MSX_SFX_CHANNELS;c++) if (policy.current[c]>=0) mixer_ch_stop(mixer_of(c));
    msx_sfx_reset(&policy);msx_audio_stop(MSX_AUDIO_INTRO);
    shots=port_shots(g);vehicle_health=g->vehicle.health;vehicle_frame=g->vehicle.frame;vehicle_state=g->vehicle.state;
    complete=g->complete;g->world.sound_count=0;
    if (!complete && !music_on) game_audio_music(true);
}
void game_audio_update(msx_game_t *g) {
    if (!active) {g->world.sound_count=0;return;}
    uint32_t started=TICKS_READ();
    // Channels whose sound ended accept any request again.
    for (unsigned c=0;c<MSX_SFX_CHANNELS;c++)
        if (policy.current[c]>=0 && !mixer_ch_playing(mixer_of(c))) msx_sfx_finished(&policy,c);
    if (port_shots(g)!=shots) request(0x10f7);
    for (unsigned i=0;i<g->world.sound_count;i++) request(g->world.sounds[i]);
    g->world.sound_count=0;
    const neo_vehicle_t *v=&g->vehicle;
    if (v->health<vehicle_health) {
        bool below=v->health<v->max_health/2 && vehicle_health>=v->max_health/2;
        if (v->health) request(0x108d);
        if (below || !v->health) request(0x1023);
    }
    if (v->state>=NEO_VEHICLE_DYING && v->frame==1 && (vehicle_frame!=1 || vehicle_state<NEO_VEHICLE_DYING)) request(0x1033);
    shots=port_shots(g);vehicle_health=v->health;vehicle_frame=v->frame;vehicle_state=v->state;
    if (g->complete && !complete) game_audio_music(false);
    complete=g->complete;
    msx_perf_add(MSX_PERF_AUDIO,TICKS_READ()-started);
    if (++frames%300==0) {
        msx_audio_stats_t st=msx_audio_stats();
        heap_stats_t h;sys_get_heap_stats(&h);
        debugf("MSX AUDIO sfx started=%u replaced=%u dropped=%u unmapped=%u world_dropped=%u music=%u ai_buffers=%lu underruns=%lu heap_free=%d resident=%u\n",
            policy.started,policy.replaced,policy.dropped,policy.unmapped,g->world.sound_dropped,
            msx_audio_playing(MSX_AUDIO_MUSIC),(unsigned long)st.buffers,(unsigned long)st.underruns,h.total-h.used,(unsigned)scene_assets_all_resident());
    }
}
