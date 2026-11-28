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
/* The resident bytes of ram:/<path> (read-only, kept for good), or NULL. */
const void *msx_ramfs_data(const char *path,size_t *size);
/* Background read (docs/RENDIMIENTO.md, "Cinemáticas sin parones"): queue
 * rom:/<path> to be read by asynchronous PI DMA, a chunk at a time, in the
 * spare frame time of the boot cinematics (msx_ramfs_preload_idle with the
 * deadline of the frame's spare time); msx_ramfs_load of the same path then
 * finds it resident, or finishes it. A file is only started while the heap
 * keeps `keep` bytes free after it. */
bool msx_ramfs_preload(const char *path);
/* One step: false when nothing is left or the next file does not fit yet. */
bool msx_ramfs_preload_idle(unsigned long deadline,int keep);
/* No more background reads: frees the queue (after the msx_ramfs_load of
 * every file the game uses; files can be queued again later). */
void msx_ramfs_preload_end(void);
#endif
