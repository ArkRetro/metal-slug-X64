#ifndef END_ZOOM_CORE_H
#define END_ZOOM_CORE_H
/* The ending's shrunk sprites drawn as the LSPC draws them (docs/MISION6.md "Sprites con zoom en N64"): one sprite-list
 * entry through the 68K's column writers (neo_scb.c) into SCB1..SCB4 words, then every column line by line with the
 * L0 ROM of the BIOS (vertical) and COLUMNS (horizontal, tools/check_lspc_shrink.py), cut into strips: the lines of
 * one column that take the same tile row. A strip is drawn 1:1. Shared by the console (end_zoom.c) and the host check
 * (tools/test_end_zoom.c). */
#include <stdbool.h>
#include <stdint.h>
/* romfs/endzoom.bin (tools/export_ending_zoom.py), big-endian. */
typedef struct {
    unsigned descriptors,tiles;
    const uint8_t *table;        /* descriptors x (P2 address, first word, word count, first tile, tile count) */
    const uint8_t *words;        /* u16 */
    const uint8_t *tile_keys;    /* tiles x u32, sorted per descriptor */
    uint32_t tiles_offset,l0_offset;   /* in the file */
} end_zoom_file_t;
/* header: the file's first 16 bytes ('EZM2', descriptors, tiles, words); rest: the bytes after them up to the tiles (table, words, tile numbers). */
bool end_zoom_parse(end_zoom_file_t *f,const uint8_t *header,const uint8_t *rest,uint32_t rest_size);
uint32_t end_zoom_rest_size(const uint8_t *header);
int end_zoom_find_descriptor(const end_zoom_file_t *f,uint32_t descriptor);   /* index or -1 */
/* Descriptor `index`'s tiles: [*first, *first + *count) of the file's tiles; tile `tile` among them, or -1. */
void end_zoom_descriptor_tiles(const end_zoom_file_t *f,int index,uint32_t *first,uint32_t *count);
int end_zoom_find_tile(const end_zoom_file_t *f,int index,uint32_t tile);
/* A strip: tile, SCB1 flips (bit 0 h, bit 1 v), shrink nibble zx (zx + 1 pixels wide), n lines (1..16) and the
 * tile line of each. */
typedef struct { uint32_t tile;uint8_t flips,zx,n;uint8_t line[16]; } end_zoom_strip_t;
typedef struct {
    const uint8_t *(*l0_row)(void *ctx,unsigned zoom_y);   /* 256 bytes of the L0 ROM: (zoom_y << 8) .. */
    /* raster x, raster line of the strip's first line */
    void (*strip)(void *ctx,const end_zoom_strip_t *s,int x,int line);
    void (*column_end)(void *ctx);   /* after the strips of one column (NULL: none) */
    void *ctx;
    unsigned auto_animated;   /* tiles with SCB1 bits 2 / 3 (not drawn: the LSPC's counter is not modelled) */
    void *work;               /* END_ZOOM_WORK_BYTES, 2-byte aligned (the entry's SCB words; the caller's, not bss) */
} end_zoom_io_t;
/* One list entry (00A03E: x, y, zoom x << 8 | zoom y, attribute bits 0..1) of descriptor `index`, raster lines
 * first..last. Returns false when the entry has more columns than the core keeps (END_ZOOM_COLUMNS). */
enum { END_ZOOM_COLUMNS=96,END_ZOOM_WORK_BYTES=END_ZOOM_COLUMNS*134 };
bool end_zoom_entry(const end_zoom_file_t *f,int index,uint16_t x,uint16_t y,uint16_t zoom,uint16_t attr,int first,int last,end_zoom_io_t *io);
/* The CI4 texels of a strip, 16 pixels (8 bytes) per line, from the tile's 128 bytes (16 x 16 CI4 in the file). */
void end_zoom_compose(const end_zoom_strip_t *s,const uint8_t *tile128,uint8_t *out);
#endif
