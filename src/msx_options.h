#ifndef MSX_OPTIONS_H
#define MSX_OPTIONS_H
#include <stdbool.h>
/* Options screen before SOLDIER SELECT (docs/OPCIONES.md; an N64 addition,
 * not arcade): the game's soft-DIP settings of RAM 10FD88..10FD8F as the
 * MVS BIOS offers them, with the names, options and defaults of the ROM's
 * own table (header of 250-p1.p1, EU 0x316E, in rom:/softdip.bin from
 * tools/extract_softdip.py), drawn with the ROM's fix font (page 3,
 * palette 0, as PAUSE). The choice goes to neo_softdip, as the BIOS writes
 * those bytes, and to the cartridge EEPROM when there is one. */

/* Reads the table and the saved choice into neo_softdip (defaults of the
 * table when nothing valid is saved). False without the table: no screen,
 * neo_softdip keeps the values of the captures. */
bool msx_options_init(void);
/* The screen until Start; idle() runs in each frame's spare time. */
void msx_options_menu(void (*idle)(void));
#endif
