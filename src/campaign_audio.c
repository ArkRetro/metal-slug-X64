#include "campaign_audio.h"
#include "msx_audio.h"
static unsigned pistol_voice;
static uint16_t loaded_token[5];
bool campaign_audio_init(void) {
    for (unsigned i=0;i<3;i++)
        if (!msx_audio_open(MSX_AUDIO_EFFECT_FIRST+i,"rom:/effects/pistol.wav64")) return false;
    if (!msx_audio_open(MSX_AUDIO_EFFECT_FIRST+3,"rom:/effects/body-10fc.wav64") ||
        !msx_audio_open(MSX_AUDIO_EFFECT_FIRST+4,"rom:/effects/body-1146.wav64")) return false;
    pistol_voice=0;
    loaded_token[0]=loaded_token[1]=loaded_token[2]=0x10f7;
    loaded_token[3]=0x10fc;loaded_token[4]=0x1146;return true;
}
bool campaign_audio_play(uint16_t token) {
    unsigned voice;const char *path;
    switch(token) {
    case 0x10f7:voice=MSX_AUDIO_EFFECT_FIRST+pistol_voice; pistol_voice=(pistol_voice+1)%3;path="rom:/effects/pistol.wav64";break;
    case 0x10fc:voice=MSX_AUDIO_EFFECT_FIRST+3;path="rom:/effects/body-10fc.wav64";break;
    case 0x1146:voice=MSX_AUDIO_EFFECT_FIRST+4;path="rom:/effects/body-1146.wav64";break;
    case 0x10fb:voice=MSX_AUDIO_EFFECT_FIRST+3;path="rom:/effects/melee-10fb.wav64";break;
    case 0x10f9:voice=MSX_AUDIO_EFFECT_FIRST+4;path="rom:/effects/melee-10f9.wav64";break;
    case 0x1069:voice=MSX_AUDIO_EFFECT_FIRST+3;path="rom:/effects/melee-1069.wav64";break;
    default:return false;
    }
    if (loaded_token[voice-MSX_AUDIO_EFFECT_FIRST]!=token) {
        if (!msx_audio_open(voice,path)) return false;
        loaded_token[voice-MSX_AUDIO_EFFECT_FIRST]=token;
    }
    return msx_audio_play(voice,false,.8f);
}
