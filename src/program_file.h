#ifndef PROGRAM_FILE_H
#define PROGRAM_FILE_H
#include "neo_progstore.h"
/* Console side of the programs read on demand (neo_progstore.h): the file
 * /programs.bin of the cartridge and the arena in the program image. */
const neo_anim_command_t *program_file_load(uint32_t offset,unsigned count);
void program_file_reset(void);
#endif
