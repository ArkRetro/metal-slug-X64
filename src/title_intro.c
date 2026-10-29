#include <libdragon.h>
#include "title_intro.h"
#include "cinematic.h"
#include "msx_audio.h"
bool msx_title_intro(void) {
 if(!msx_cinematic_open("rom:/title.msxc")) return false;
 unsigned count=msx_cinematic_stats().frames;
 uint32_t first_crc=msx_cinematic_pixels_crc();
 if(count<=1) {msx_cinematic_close();return false;}
 if(!msx_audio_open(MSX_AUDIO_MUSIC,"rom:/title-motion.wav64")) {msx_cinematic_close();return false;}
 // Prepare the initial RDP path before starting the synchronized timeline.
 msx_cinematic_draw(display_get());
 if(!msx_audio_play(MSX_AUDIO_MUSIC,false,.8f)) {msx_cinematic_close();return false;}
 bool previous=true,skipped=false;unsigned rendered=0,unique=0,dropped=0,last=~0u;
 debugf("MSX TITLE start crc=%08lx audio=%u\n",(unsigned long)first_crc,msx_audio_playing(MSX_AUDIO_MUSIC));
 uint32_t began=TICKS_READ(),audio_max=0,previous_draw=0;
 for(;;) {
  uint32_t audio_started=TICKS_READ();msx_audio_service();uint32_t audio_ticks=TICKS_READ()-audio_started;if(audio_ticks>audio_max) audio_max=audio_ticks;joypad_poll();
  bool start=joypad_get_buttons(JOYPAD_PORT_1).start;
  uint32_t elapsed=(uint32_t)(TICKS_READ()-began);
  unsigned target=(unsigned)((uint64_t)elapsed*59185606u/((uint64_t)TICKS_PER_SECOND*1000000u));
  skipped=target>=30 && start && !previous;previous=start;
  if(skipped) break;
  surface_t *surface=display_get();
  elapsed=(uint32_t)(TICKS_READ()-began);
  target=(unsigned)((uint64_t)elapsed*59185606u/((uint64_t)TICKS_PER_SECOND*1000000u));
  msx_cinematic_stats_t state=msx_cinematic_stats();unsigned work=0;
  while(state.index<target && state.index+1<count && work++<8) {
   if(!msx_cinematic_next()) {msx_audio_stop(MSX_AUDIO_MUSIC);msx_cinematic_close();return false;}
   state=msx_cinematic_stats();
  }
  if(last!=state.index) {if(last==~0u) dropped+=state.index;else if(state.index>last+1) {dropped+=state.index-last-1;debugf("MSX TITLE miss from=%u to=%lu audio_us=%lu draw_us=%lu\n",last,(unsigned long)state.index,(unsigned long)TICKS_TO_US(audio_ticks),(unsigned long)TICKS_TO_US(previous_draw));}unique++;last=state.index;}
  uint32_t draw_started=TICKS_READ();msx_cinematic_draw(surface);previous_draw=TICKS_READ()-draw_started;rendered++;
  if(target>=count && state.index+1==count) break;
 }
 msx_cinematic_stats_t final=msx_cinematic_stats();
 debugf("MSX TITLE timing audio_max_us=%lu decode_max_us=%lu draw_max_us=%lu\n",(unsigned long)TICKS_TO_US(audio_max),(unsigned long)TICKS_TO_US(final.max_decode_ticks),(unsigned long)TICKS_TO_US(final.max_draw_ticks));
 msx_audio_stop(MSX_AUDIO_MUSIC);msx_cinematic_close();
 debugf("MSX TITLE cadence unique=%u dropped=%u source=%u\n",unique,dropped,count);
 debugf("MSX TITLE complete frames=%u skipped=%u\n",rendered,skipped);return true;
}
