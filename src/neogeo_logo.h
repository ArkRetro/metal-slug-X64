#ifndef MSX_NEOGEO_LOGO_H
#define MSX_NEOGEO_LOGO_H
#include <stdbool.h>
/* Native 3D NEOGEO boot logo with the eyecatch jingle (docs/LOGO_NEOGEO.md).
 * The mesh (rom:/neogeo-logo.mesh) and jingle (rom:/neogeo-jingle.wav64) are
 * private assets built by tools/build_neogeo_logo.py; without them the logo is
 * skipped and this returns false. Needs rdpq, display and msx_audio started.
 * Start (pressed edge) skips it. Ends on black. */
bool msx_neogeo_logo(void);
#endif
