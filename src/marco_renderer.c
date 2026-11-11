#include <libdragon.h>
#include <stdio.h>
#include "marco_renderer.h"
#include "scene_renderer.h"
#include "marco_pose_meta.h"
#include "msx_character.h"
#include <malloc.h>
#include <string.h>
static sprite_t *parts[MARCO_PARTS];
static bool loaded;
/* Another character's catalog (marco_renderer_set_character): the pack
 * rom:/chars/cN.bin held in memory. Its poses, parts and palettes replace
 * the built-in tables; a part is either pixels of the pack or one of
 * Marco's parts in romfs/marco (the shared legs), loaded as a sprite. */
static uint8_t *pack;static msx_character_layout_t pk;
static sprite_t **pack_sprites;             /* per part: Marco's sprite when shared, else NULL */
static uint16_t pack_palettes[MARCO_PALETTES][16] __attribute__((aligned(16)));
#define be16 msx_character_be16
#define be32 msx_character_be32
static inline unsigned pose_count(void) {return pack?pk.poses:MARCO_POSES;}
static inline unsigned part_count(void) {return pack?pk.parts:MARCO_PARTS;}
static inline const uint8_t *pk_pose(unsigned i) {return pack+pk.off_poses+i*12u;}
static inline const uint8_t *pk_part(unsigned i) {return pack+pk.off_parts+i*16u;}
static inline uint32_t pose_tag(unsigned i) {return pack?be32(pk_pose(i)):marco_poses[i].source_tag;}
static inline unsigned pose_first(unsigned i) {return pack?be16(pk_pose(i)+4):marco_poses[i].first;}
static inline unsigned pose_parts(unsigned i) {return pack?be16(pk_pose(i)+6):marco_poses[i].count;}
static inline int part_x(unsigned i) {return pack?(int16_t)be16(pk_part(i)):marco_parts[i].x;}
static inline int part_y(unsigned i) {return pack?(int16_t)be16(pk_part(i)+2):marco_parts[i].y;}
static inline int part_w(unsigned i) {return pack?be16(pk_part(i)+4):marco_parts[i].width;}
static inline int part_h(unsigned i) {return pack?be16(pk_part(i)+6):marco_parts[i].height;}
static inline uint32_t part_fnv(unsigned i) {return pack?be32(pk_part(i)+8):marco_parts[i].indices_fnv;}
static inline const uint16_t *palette_words(unsigned i) {return pack?pack_palettes[i]:marco_palettes[i];}
static surface_t part_pixels(unsigned i) {
    if (!pack) return sprite_get_pixels(parts[i]);
    if (pack_sprites[i]) return sprite_get_pixels(pack_sprites[i]);
    return surface_make_linear(pack+pk.off_pixels+be32(pk_part(i)+12),FMT_CI4,(uint16_t)part_w(i),(uint16_t)part_h(i));
}
static unsigned next_part,part_bytes;static bool part_failed;   // marco_renderer_preload
static unsigned max_upload;
unsigned marco_renderer_count(void) {return pose_count();}
/* The palette profile of the burnt death (0BF358, kinds 7/0x10): the one
 * the catalog build names BURNT (build_marco_poses.py --name-palette), else
 * the first. */
unsigned marco_renderer_burnt_palette(void) {
#ifdef MARCO_PALETTE_BURNT
    return MARCO_PALETTE_BURNT;
#else
    return 0;
#endif
}
unsigned marco_renderer_mummy_palette(bool second) {
#if defined(MARCO_PALETTE_MUMMY) && defined(MARCO_PALETTE_MUMMY2)
    return second?MARCO_PALETTE_MUMMY2:MARCO_PALETTE_MUMMY;
#else
    (void)second;return 0;
#endif
}
unsigned marco_renderer_palette_count(void) {return MARCO_PALETTES;}
unsigned marco_renderer_max_upload(void) {return max_upload;}
uint32_t marco_renderer_source_tag(unsigned pose) {return pose<pose_count()?pose_tag(pose):0;}
bool marco_renderer_find(uint32_t tag,unsigned *pose) {
    unsigned lo=0,hi=pose_count();
    while (lo<hi) {
        unsigned mid=lo+(hi-lo)/2;
        if (pose_tag(mid)<tag) lo=mid+1;else hi=mid;
    }
    if (!pose || lo==pose_count() || pose_tag(lo)!=tag) return false;
    *pose=lo;return true;
}
void marco_renderer_close(void) {
    for (unsigned i=0;i<MARCO_PARTS;i++) if (parts[i]) {sprite_free(parts[i]);parts[i]=NULL;}
    if (pack_sprites) for (unsigned i=0;i<pk.parts;i++) if (pack_sprites[i]) {sprite_free(pack_sprites[i]);pack_sprites[i]=NULL;}
    loaded=false;max_upload=0;next_part=part_bytes=0;part_failed=false;
}
static uint32_t part_cost;   // ticks of the slowest recent part load
static uint32_t pixels_fnv(const surface_t *pixels) {
    uint32_t fingerprint=2166136261u;const uint8_t *data=pixels->buffer;
    for (unsigned y=0;y<pixels->height;y++) for (unsigned x=0;x<pixels->width;x++)
        fingerprint=(fingerprint^((data[y*pixels->stride+x/2]>>((x&1)?0:4))&15u))*16777619u;
    return fingerprint;
}
/* A part of the pack: its own pixels (checked against the fingerprint the
 * exporter wrote) or Marco's sprite of the same geometry and pixels. */
static bool load_pack_part(unsigned i) {
    uint32_t data=be32(pk_part(i)+12);
    if (data&0x80000000u) {
        unsigned m=data&0x7fffffffu;if (m>=MARCO_PARTS) return false;
        heap_stats_t h0;sys_get_heap_stats(&h0);
        char path[48];snprintf(path,sizeof(path),"rom:/marco/p%u.sprite",m);pack_sprites[i]=sprite_load(path);
        heap_stats_t h1;sys_get_heap_stats(&h1);part_bytes+=(unsigned)(h1.used-h0.used);
        if (!pack_sprites[i] || sprite_get_format(pack_sprites[i])!=FMT_CI4 || pack_sprites[i]->width!=part_w(i) || pack_sprites[i]->height!=part_h(i)) {
            debugf("MSX character part %u: Marco's part %u not loaded\n",i,m);return false;
        }
    } else if ((uint64_t)pk.off_pixels+data+(unsigned)((part_w(i)+1)/2*part_h(i))>pk.size || (data&7u)) {
        debugf("MSX character part %u: outside the pack\n",i);return false;
    }
    surface_t pixels=part_pixels(i);
    if ((unsigned)pixels.stride*pixels.height>2048 || pixels_fnv(&pixels)!=part_fnv(i)) {debugf("MSX character part %u: pixels differ\n",i);return false;}
    return true;
}
static bool load_part(unsigned i) {
    if (pack) return load_pack_part(i);
    {
        heap_stats_t h0;sys_get_heap_stats(&h0);
        char path[48];snprintf(path,sizeof(path),"rom:/marco/p%u.sprite",i);parts[i]=sprite_load(path);
        heap_stats_t h1;sys_get_heap_stats(&h1);part_bytes+=(unsigned)(h1.used-h0.used);
        if (!parts[i]) {debugf("MSX marco part %u: not loaded\n",i);return false;}
        surface_t pixels=sprite_get_pixels(parts[i]);
        if (sprite_get_format(parts[i])!=FMT_CI4 || parts[i]->width!=marco_parts[i].width ||
            parts[i]->height!=marco_parts[i].height || (unsigned)pixels.stride*pixels.height>2048) {
            debugf("MSX marco part %u: format/size %ux%u\n",i,parts[i]->width,parts[i]->height);return false;
        }
        const uint16_t *palette=sprite_get_palette(parts[i]);
        for (unsigned pen=0;pen<16;pen++) if (palette[pen]!=marco_palettes[0][pen]) {
            debugf("MSX marco part %u: palette pen %u\n",i,pen);return false;
        }
        uint32_t fingerprint=2166136261u;const uint8_t *data=pixels.buffer;
        for (unsigned y=0;y<pixels.height;y++) for (unsigned x=0;x<pixels.width;x++)
            fingerprint=(fingerprint^((data[y*pixels.stride+x/2]>>((x&1)?0:4))&15u))*16777619u;
        if (fingerprint!=marco_parts[i].indices_fnv) {debugf("MSX marco part %u: pixels differ\n",i);return false;}
    }
    return true;
}
bool marco_renderer_preload(unsigned long deadline,int keep) {
    if (loaded || part_failed || next_part>=part_count()) return false;
    if (!part_cost) part_cost=TICKS_FROM_US(1500);
    int32_t left=(int32_t)((uint32_t)deadline-TICKS_READ());
    if (left<=(int32_t)part_cost) return false;
    heap_stats_t h;sys_get_heap_stats(&h);
    if (h.total-h.used<keep+4096) return false;
    uint32_t t0=TICKS_READ();
    if (!load_part(next_part)) {part_failed=true;return false;}
    next_part++;
    uint32_t spent=TICKS_READ()-t0;part_cost-=part_cost/16;if (spent>part_cost) part_cost=spent;
    return true;
}
/* The pack of another character and its decoded programs are part of the
 * same budget as Marco's parts (scene_assets.c, SCENE_RESERVE_GAME). */
static unsigned pack_bytes;
void marco_renderer_character_heap(unsigned bytes) {pack_bytes=bytes;}
unsigned marco_renderer_preloaded_bytes(void) {return part_bytes+pack_bytes;}
bool marco_renderer_load(void) {
    if (loaded) return true;
    if (part_failed) {marco_renderer_close();return false;}
    unsigned before=next_part;
    for (;next_part<part_count();next_part++) if (!load_part(next_part)) {marco_renderer_close();return false;}
    if (before) debugf("MSX marco parts: %u of %u loaded before the handoff\n",before,part_count());
    if (pack) {data_cache_hit_writeback(pack,pk.size);data_cache_hit_writeback(pack_palettes,sizeof(pack_palettes));}
    data_cache_hit_writeback((void*)marco_palettes,sizeof(marco_palettes));
    loaded=true;return true;
}
static bool draw_pose(unsigned pose,int x,int y,bool mirror,bool flip_y,unsigned palette,bool wrapped,int viewport_x,int viewport_y) {
    if (!loaded || pose>=pose_count() || palette>=MARCO_PALETTES) return false;
    rdpq_mode_push();rdpq_set_mode_standard();rdpq_mode_filter(FILTER_POINT);
    rdpq_mode_dithering(DITHER_NONE_NONE);rdpq_mode_alphacompare(1);
    rdpq_mode_tlut(TLUT_RGBA16);
    rdpq_tex_upload_tlut((uint16_t*)scene_render_fade_palette(palette_words(palette)),0,16);
    for (unsigned p=0;p<pose_parts(pose);p++) {
        unsigned i=pose_first(pose)+p;
        int width=part_w(i),height=part_h(i);
        int left=x+(mirror?-part_x(i)-width-1:part_x(i));
        int top=y+(flip_y?-part_y(i)-height-1:part_y(i));
        if (wrapped) {left=(int)((unsigned)left&511u);top=(int)((unsigned)top&511u);}
        unsigned copies=wrapped?2:1;bool uploaded=false;
        for (unsigned wy=0;wy<copies;wy++) for (unsigned wx=0;wx<copies;wx++) {
            int xx=left+viewport_x-(int)wx*512,yy=top+viewport_y-(int)wy*512;
            if (xx>=320 || xx+width<=0 || yy>=240 || yy+height<=0) continue;
            if (!uploaded) {
                surface_t pixels=part_pixels(i);
                int bytes=rdpq_tex_upload(TILE0,&pixels,NULL);
                if (bytes<0 || bytes>2048) {rdpq_mode_pop();return false;}
                if ((unsigned)bytes>max_upload) max_upload=(unsigned)bytes;
                uploaded=true;
            }
            rdpq_texture_rectangle_scaled(TILE0,xx,yy,xx+width,yy+height,
                mirror?width-1:0,flip_y?height-1:0,mirror?-1:width,flip_y?-1:height);
        }
    }
    rdpq_mode_pop();return true;
}
bool marco_renderer_draw_palette(unsigned pose,int x,int y,bool mirror,unsigned palette) {
    return draw_pose(pose,x,y,mirror,false,palette,false,0,0);
}
bool marco_renderer_draw(unsigned pose,int x,int y,bool mirror) {
    return marco_renderer_draw_palette(pose,x,y,mirror,0);
}
bool marco_renderer_draw_native(const neo_anim_draw_t *draw,int viewport_x,int viewport_y,unsigned palette) {
    unsigned pose;
    if (!draw || !marco_renderer_find(draw->descriptor,&pose)) return false;
    return draw_pose(pose,draw->x&511u,(0u-draw->y)&511u,
        (draw->attributes&1u)!=0,(draw->attributes&2u)!=0,palette,true,viewport_x,viewport_y);
}
/* The catalog of the chosen character: 0 Marco (built in), 1..3 the pack
 * rom:/chars/cN.bin read whole into memory. The parts loaded so far are
 * dropped; marco_renderer_preload/load then load the new ones. */
static unsigned character;
unsigned marco_renderer_character(void) {return character;}
const uint8_t *marco_renderer_character_pack(uint32_t *size) {if (size) *size=pack?pk.size:0;return pack;}
bool marco_renderer_set_character(unsigned c) {
    if (c==character) return true;
    marco_renderer_close();
    if (pack) {free(pack_sprites);pack_sprites=NULL;free(pack);pack=NULL;}
    character=0;pack_bytes=0;
    if (!c) return true;
    char path[32];snprintf(path,sizeof(path),"rom:/chars/c%u.bin",c);
    FILE *f=fopen(path,"rb");if (!f) {debugf("MSX character %u: %s missing\n",c,path);return false;}
    fseek(f,0,SEEK_END);long size=ftell(f);fseek(f,0,SEEK_SET);
    uint8_t *file=size>0?malloc((size_t)size):NULL;
    bool ok=file && fread(file,1,(size_t)size,f)==(size_t)size;fclose(f);
    uint32_t raw=ok?msx_character_file_size(file,(uint32_t)size):0;
    uint8_t *blob=raw?memalign(16,((size_t)raw+15u)&~(size_t)15u):NULL;
    ok=blob && msx_character_unpack(file,(uint32_t)size,blob,raw);free(file);size=(long)raw;
    msx_character_layout_t l;
    if (!ok || !msx_character_layout(blob,(uint32_t)size,&l) || l.character!=c || l.palettes!=MARCO_PALETTES || (l.off_pixels&7u)) {
        debugf("MSX character %u: bad pack\n",c);free(blob);return false;
    }
    sprite_t **sprites=calloc(l.parts?l.parts:1u,sizeof *sprites);
    if (!sprites) {free(blob);return false;}
    for (unsigned i=0;i<l.poses;i++) {   /* poses sorted by tag, parts inside the table */
        const uint8_t *q=blob+l.off_poses+i*12u;
        if ((i && be32(q)<=be32(q-12)) || (unsigned)be16(q+4)+be16(q+6)>l.parts) {debugf("MSX character %u: bad pose table\n",c);free(sprites);free(blob);return false;}
    }
    for (unsigned i=0;i<MARCO_PALETTES;i++) for (unsigned k=0;k<16;k++) pack_palettes[i][k]=be16(blob+l.off_palettes+i*32u+k*2u);
    pack=blob;pk=l;pack_sprites=sprites;character=c;
    debugf("MSX character %u: pack %ld bytes, %u poses, %u parts\n",c,size,l.poses,l.parts);
    return true;
}
