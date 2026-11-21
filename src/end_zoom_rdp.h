#ifndef END_ZOOM_RDP_H
#define END_ZOOM_RDP_H
/* The ending's shrunk sprites as RDP texture rectangles (docs/MISION6.md "Zoom con el RDP"): no composition. Every
 * column of a list entry (neo_scb.c) is cut, per tile row, into the exact pieces of tools/export_ending_zoom.py
 * (rdp_tables): horizontal per shrink nibble and mirror (COLUMNS), vertical per L0 line sequence; each piece is one
 * TEXTURE_RECTANGLE with s0 / t0 in 10.5 and ds/dx / dt/dy in 5.10, point sampled, whose texel at pixel (i, j) is
 * (floor((32 s0 + dsdx i) / 1024), floor((32 t0 + dtdy j) / 1024)). The tiles are the descriptor's (16 x 16 CI4) in
 * pages of 16 (one 16 x 256 CI4 texture, tile k at t = 16 k). Shared by the console (end_zoom.c) and the host check
 * (tools/check_end_zoom.py --rdp, which draws the rectangles with that arithmetic). */
#include <stdbool.h>
#include <stdint.h>
#include "end_zoom_core.h"
typedef struct { const uint8_t *horizontal,*runs,*seqs,*pieces;unsigned nseq,npieces; } end_zoom_rdp_t;
/* tables: the file's bytes after the L0 (end_zoom_file_t.l0_offset + 65536) up to its end. */
bool end_zoom_rdp_parse(end_zoom_rdp_t *t,const uint8_t *tables,uint32_t size);
/* Console pixels: x 0..319, y 0..239 (raster line 8 is y 0). x1 / y1 exclusive; s0 / t0 10.5; dsdx / dtdy 5.10. */
typedef struct { int16_t x0,y0,x1,y1;uint16_t s0,t0;int16_t dsdx,dtdy;uint16_t page; } end_zoom_rect_t;
enum { END_ZOOM_RECTS=2048 };
typedef struct {
    void (*rect)(void *ctx,const end_zoom_rect_t *r);
    void *ctx;
    void *work;        /* END_ZOOM_WORK_BYTES */
    end_zoom_rect_t *sorted;unsigned sorted_max;
    const uint16_t *local_map;      /* end_zoom_rdp_local_map of the descriptor, or NULL (binary search) */
    uint16_t (*local_work)[32];     /* END_ZOOM_COLUMNS x 32, with local_map */   /* 2 x END_ZOOM_RECTS: the entry's rectangles, then sorted by page (stable) */
    unsigned rects,refits,auto_animated,missing_tiles,unfit;
} end_zoom_rdp_io_t;
/* One list entry of descriptor `index` (tile pages: the descriptor's tiles in its order, 16 a page). Its rectangles
 * are emitted grouped by page (stable): an entry's columns and tile rows do not overlap (check_end_zoom.py --rdp draws
 * them in this order). */
/* map: one u16 per descriptor word (end_zoom_descriptor_words). */
uint32_t end_zoom_descriptor_words(const end_zoom_file_t *f,int index);
void end_zoom_rdp_local_map(const end_zoom_file_t *f,int index,uint16_t *map);
bool end_zoom_rdp_entry(const end_zoom_file_t *f,const end_zoom_rdp_t *t,int index,uint16_t x,uint16_t y,uint16_t zoom,uint16_t attr,end_zoom_rdp_io_t *io);
#endif
