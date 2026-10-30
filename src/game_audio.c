#include <libdragon.h>
#include "game_audio.h"
#include "msx_audio.h"
#include "msx_sfx.h"
#include "msx_perf.h"
/* Event sources -> original request words (data/sound_map.json):
 *  - game->shots (bullet created, ROM 01B782)           -> 0x10F7
 *  - world sound list (animation 0x30, 0x10FA, 0x1027)  -> as requested
 *  - vehicle health lowered by a bullet (ROM 01BB34)     -> 0x108D
 *  - vehicle health below half / reaching zero (029AE4)  -> 0x1023
 *  - vehicle destruction burst, DESTROY frame 1 (029B14) -> 0x1033
 *  - region end: the music stops (the arcade has no region end; its driver
 *    is reset with command 03 between scenes, which silences the music). */
enum { VOLUME_PERCENT=80 };
static wav64_t sounds[MSX_SFX_COUNT];
static bool loaded[MSX_SFX_COUNT],ready,music_ready,music_on,warned,active;
static msx_sfx_state_t policy;
static unsigned shots,vehicle_health,vehicle_frame,vehicle_state;
static bool complete;
static unsigned frames;
static unsigned voice_of(unsigned channel) {return MSX_AUDIO_EFFECT_FIRST+channel;}
bool game_audio_init(void) {
    msx_sfx_reset(&policy);ready=false;
    unsigned count=0;
    for (unsigned i=0;i<MSX_SFX_COUNT;i++) {
        char path[64];snprintf(path,sizeof(path),"rom:/audio/%s.wav64",msx_sfx_defs[i].name);
        FILE *f=fopen(path,"rb");
        if (!f) {loaded[i]=false;continue;}
        fclose(f);wav64_open(&sounds[i],path);wav64_set_loop(&sounds[i],false);loaded[i]=true;count++;
    }
    /* Intro + loop body (tools/pack_game_audio.py); older packs have only
     * the whole file, looped whole. */
    music_ready=msx_audio_open(MSX_AUDIO_MUSIC,"rom:/audio/music_mission1_intro.wav64") &&
        msx_audio_open_loop(MSX_AUDIO_MUSIC,"rom:/audio/music_mission1.wav64");
    if (!music_ready) music_ready=msx_audio_open(MSX_AUDIO_MUSIC,"rom:/audio/music_mission1.wav64");
    ready=count>0;active=true;
    debugf("MSX AUDIO game sounds=%u/%u music=%u\n",count,(unsigned)MSX_SFX_COUNT,music_ready);
    return ready && count==MSX_SFX_COUNT && music_ready;
}
void game_audio_music(bool play) {
    if (!active || !music_ready) return;
    if (play) music_on=msx_audio_play(MSX_AUDIO_MUSIC,true,VOLUME_PERCENT/100.0f);
    else {msx_audio_stop(MSX_AUDIO_MUSIC);music_on=false;}
    debugf("MSX AUDIO music %s\n",music_on?"start":"stop");
}
static void request(uint16_t word) {
    int channel=msx_sfx_request(&policy,word);
    if (channel<0) return;
    int id=policy.current[channel];
    if (!loaded[id]) {
        if (!warned) debugf("MSX AUDIO missing rom:/audio/%s.wav64\n",msx_sfx_defs[id].name);
        warned=true;msx_sfx_finished(&policy,(unsigned)channel);return;
    }
    unsigned mixer=voice_of((unsigned)channel)*2;
    mixer_ch_stop(mixer);wav64_play(&sounds[id],mixer);
    float volume=VOLUME_PERCENT/100.0f;mixer_ch_set_vol(mixer,volume,volume);
}
void game_audio_sync(msx_game_t *g) {
    if (!active) return;
    for (unsigned c=0;c<MSX_SFX_CHANNELS;c++) if (policy.current[c]>=0) mixer_ch_stop(voice_of(c)*2);
    msx_sfx_reset(&policy);
    shots=g->shots;vehicle_health=g->vehicle.health;vehicle_frame=g->vehicle.frame;vehicle_state=g->vehicle.state;
    complete=g->complete;g->world.sound_count=0;
    if (!complete && !music_on) game_audio_music(true);
}
void game_audio_update(msx_game_t *g) {
    if (!active) {g->world.sound_count=0;return;}
    uint32_t started=TICKS_READ();
    // Channels whose sound ended accept any request again.
    for (unsigned c=0;c<MSX_SFX_CHANNELS;c++)
        if (policy.current[c]>=0 && !mixer_ch_playing(voice_of(c)*2)) msx_sfx_finished(&policy,c);
    if (g->shots!=shots) request(0x10f7);
    for (unsigned i=0;i<g->world.sound_count;i++) request(g->world.sounds[i]);
    g->world.sound_count=0;
    const neo_vehicle_t *v=&g->vehicle;
    if (v->health<vehicle_health) {
        bool below=v->health<v->max_health/2 && vehicle_health>=v->max_health/2;
        if (v->health) request(0x108d);
        if (below || !v->health) request(0x1023);
    }
    if (v->state>=NEO_VEHICLE_DYING && v->frame==1 && (vehicle_frame!=1 || vehicle_state<NEO_VEHICLE_DYING)) request(0x1033);
    shots=g->shots;vehicle_health=v->health;vehicle_frame=v->frame;vehicle_state=v->state;
    if (g->complete && !complete) game_audio_music(false);
    complete=g->complete;
    msx_perf_add(MSX_PERF_AUDIO,TICKS_READ()-started);
    if (++frames%300==0) {
        msx_audio_stats_t st=msx_audio_stats();
        debugf("MSX AUDIO sfx started=%u replaced=%u dropped=%u unmapped=%u world_dropped=%u music=%u ai_buffers=%lu underruns=%lu\n",
            policy.started,policy.replaced,policy.dropped,policy.unmapped,g->world.sound_dropped,
            msx_audio_playing(MSX_AUDIO_MUSIC),(unsigned long)st.buffers,(unsigned long)st.underruns);
    }
}
