#include <libdragon.h>
#include "title_intro.h"
#include "cinematic.h"
#include "msx_audio.h"
bool msx_title_intro(void) {
 if(!msx_cinematic_open("rom:/title.msxc")) return false;
 if(msx_cinematic_stats().frames!=1) {msx_cinematic_close();return false;}
 if(!msx_audio_open(MSX_AUDIO_ANNOUNCER,"rom:/title-announcement.wav64") || !msx_audio_play(MSX_AUDIO_ANNOUNCER,false,.7f)) {msx_cinematic_close();return false;}
 uint32_t began=TICKS_READ();bool previous=true,skipped=false;unsigned frames=0;
 debugf("MSX TITLE start crc=%08lx audio=%u\n",(unsigned long)msx_cinematic_pixels_crc(),msx_audio_playing(MSX_AUDIO_ANNOUNCER));
 for(;;) {
  msx_audio_service();joypad_poll();
  bool start=joypad_get_buttons(JOYPAD_PORT_1).start;
  unsigned ticks=(unsigned)((uint64_t)(uint32_t)(TICKS_READ()-began)*60/TICKS_PER_SECOND);
  skipped=ticks>=30 && start && !previous;previous=start;
  if(skipped || ticks>=300) break;
  msx_cinematic_draw(display_get());frames++;
 }
 msx_audio_stop(MSX_AUDIO_ANNOUNCER);msx_cinematic_close();
 debugf("MSX TITLE complete frames=%u skipped=%u\n",frames,skipped);return true;
}
