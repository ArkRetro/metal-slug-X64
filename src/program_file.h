#ifndef PROGRAM_FILE_H
#define PROGRAM_FILE_H
#include "neo_progstore.h"
/* Console side of the programs read on demand (neo_progstore.h): the file
 * /programs.bin of the cartridge and the arena in the program image. */
const neo_anim_program_t *program_file_load(uint32_t offset,unsigned count);
void program_file_reset(void);
/* The ROM bytes at `offset` of /romblobs.bin (tools/compile_region_rom.py), read by
 * DMA into one of eight 32-byte windows (the ROM readers take at most 4 bytes and
 * hold at most two pointers); NULL without the file or past its end. */
const uint8_t *program_file_rom(uint32_t offset);
#endif
