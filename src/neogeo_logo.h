#ifndef MSX_NEOGEO_LOGO_H
#define MSX_NEOGEO_LOGO_H
#include <stdbool.h>
/* Native 3D NEOGEO boot logo with the eyecatch jingle (docs/LOGO_NEOGEO.md).
 * The mesh (rom:/neogeo-logo.mesh) and jingle (rom:/neogeo-jingle.wav64) are
 * private assets built by tools/build_neogeo_logo.py; without them the logo is
 * skipped and this returns false. Needs rdpq, display and msx_audio started.
 * Start (pressed edge) skips it. Ends on black. */
bool msx_neogeo_logo(void);
/* Optional work run in each logo frame's spare time, after the frame is
 * submitted: called until it returns false; it must return false before the
 * next vertical blank (the game passes scene_assets_idle). NULL: none. */
void msx_neogeo_logo_set_idle(bool (*idle)(void));
#endif
