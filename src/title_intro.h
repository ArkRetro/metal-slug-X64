#ifndef MSX_TITLE_INTRO_H
#define MSX_TITLE_INTRO_H
#include <stdbool.h>
/* Animated X64 title with original effects and authored digits; excludes arcade tutorial/selection. */
/* next: the movie that follows (prepared during the title's spare frame
 * time), or NULL. */
bool msx_title_intro(const char *next);
#endif
