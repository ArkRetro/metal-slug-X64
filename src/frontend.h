#ifndef MSX_FRONTEND_H
#define MSX_FRONTEND_H
#include <stdbool.h>
/* Original tutorial, selection reveal and Marco confirmation, following title.
 * Only Marco is supported by the current campaign. No other character input is
 * accepted. This does not implement the original four-character controller.
 * A/START may skip the tutorial; a fresh A/START edge confirms Marco at select.
 */
bool msx_frontend_after_title(void);
#endif
