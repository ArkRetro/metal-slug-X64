#ifndef MSX_RAMFS_H
#define MSX_RAMFS_H
#include <stdbool.h>
#include <stddef.h>
/* "ram:/" filesystem over buffers resident in RDRAM (Expansion Pak). The
 * gameplay sound effects are read whole from the ROM once at startup and
 * served from RAM to the WAV64 decoder, so a play never touches the
 * cartridge bus. Read-only; a file can be opened several times. */
bool msx_ramfs_init(void);
/* Read rom:/<path> whole into RAM and publish it as ram:/<path>. Returns the
 * bytes kept resident (0 when the ROM file is missing or memory runs out). */
size_t msx_ramfs_load(const char *path);
size_t msx_ramfs_bytes(void);
#endif
