/* The scene loader (src/scene_assets.c) built for the host and run over a
 * directory of cartridge files (scene.pack, missionN.pack): everything it
 * hands to the game is written out as text (docs/COMPRESION.md).
 *
 *   pack-loader DIR OUT
 *
 * make check runs it twice from the same sources: over the packs written
 * with the codecs of before (pack_scene.py --legacy, pack_mission.py
 * --format 4) and over the compressed ones (version 5, LZ4 chunks, actor
 * pages taken from another mission's pack), and compares the two outputs
 * byte for byte.
 *
 * An image is written with the CRC-32 of its 2048-byte page and its palette's
 * 16 colours, not the page and palette numbers: the page a part lives on is
 * the one thing a version 5 pack may renumber (actor pages), and the pixels
 * must not change. Map images also keep their page number (the map is never
 * renumbered). Every page is read through the cache: scene_page loads the
 * chunk, decodes it and checks its CRC as on the console. */
#ifndef LOADER
#define LOADER "../src/scene_assets.c"
#endif
#include LOADER
#include <inttypes.h>
void msx_perf_add(unsigned bucket,uint32_t ticks) {(void)bucket;(void)ticks;}
unsigned scene_prerender_probe(unsigned mission,unsigned index) {(void)mission;(void)index;return 0;}
bool scene_prerender_open(uint8_t *mem,unsigned palette_base) {(void)mem;(void)palette_base;return false;}
unsigned scene_prerender_layers(void) {return 0;}
void scene_prerender_close(void) {}
static FILE *out;
static unsigned frame;
static void hex(const char *name,const uint8_t *p,unsigned n) {
    fprintf(out,"%s %u crc=%08" PRIx32 "\n",name,n,n?scene_crc32(p,n):0u);
    for (unsigned i=0;i<n;i+=32) {
        for (unsigned k=i;k<n && k<i+32;k++) fprintf(out,"%02x",p[k]);
        fputc('\n',out);
    }
}
/* The page's bytes through the cache (a frame per page, so every slot but
 * the pinned ones can be recycled). */
static uint32_t page_crc(const scene_image_t *im) {
    scene_begin_epoch(++frame);
    const uint8_t *p=scene_page(im);
    uint32_t crc=scene_crc32(p,SCENE_PAGE_BYTES);
    scene_complete_epoch(frame);
    return crc;
}
static void image(const char *name,unsigned i,const scene_image_t *im,bool keep_page) {
    const uint16_t *pal=scene_palette(im->palette);
    fprintf(out,"%s %u page=%08" PRIx32,name,i,page_crc(im));
    if (keep_page) fprintf(out," id=%u",im->page);
    fprintf(out," at=%d,%d size=%ux%u format=%u pal=",im->x,im->y,im->width,im->height,im->format);
    for (unsigned k=0;k<16;k++) fprintf(out,"%04x",pal[k]);
    fputc('\n',out);
}
static void cells(const char *name,const scene_map_cell_t *c,unsigned n) {
    for (unsigned i=0;i<n;i++) {
        fprintf(out,"%s %u layer=%u at=%d,%d images=",name,i,c[i].layer,c[i].x,c[i].y);
        for (unsigned k=0;k<8;k++) fprintf(out,"%u%c",scene_cell_phase(&c[i],k),k<7?',':'\n');
    }
}
static void dump_scene(void) {
    fprintf(out,"scene.pack pages=%u palettes=%u\n",SCENE_PAGE_COUNT,SCENE_PALETTE_COUNT);
    for (unsigned b=0;b<SCENE_BANK_COUNT;b++) {
        const scene_bank_t *k=scene_bank(b);
        fprintf(out,"bank %u start=%u first=%u second=%u interval=%u size=%ux%u\n",b,k->start,k->first,k->second,k->interval,k->width,k->height);
    }
    for (unsigned i=0;i<sizeof(scene_images)/sizeof(*scene_images);i++) image("image",i,&scene_images[i],true);
    for (unsigned i=0;i<70;i++) if (scene_background[i].format==0) image("background",i,&scene_background[i],true);
    for (unsigned i=0;i<sizeof(scene_map_images)/sizeof(*scene_map_images);i++) image("map_image",i,&scene_map_images[i],true);
    unsigned n;const scene_map_cell_t *c=scene_map_cells(&n);cells("cell",c,n);
    c=scene_patch_cells(&n);cells("patch_cell",c,n);c=scene_collapse_cells(&n);cells("collapse_cell",c,n);
    for (unsigned i=0;i<sizeof(scene_native_descriptors)/sizeof(*scene_native_descriptors);i++)
        fprintf(out,"descriptor %06" PRIx32 " first=%u count=%u\n",scene_native_descriptors[i].tag,scene_native_descriptors[i].first,scene_native_descriptors[i].count);
    /* Every page of the pack, used by an image or not. */
    for (unsigned p=0;p<SCENE_PAGE_COUNT;p++) {
        scene_image_t im={.page=(uint16_t)p};
        fprintf(out,"page %u %08" PRIx32 "\n",p,page_crc(&im));
    }
}
static void dump_stage(unsigned mission,unsigned index) {
    const scene_stage_t *st=scene_stage();
    fprintf(out,"mission %u stage %u descriptor=%06" PRIx32 " width=%u camera_max=%u backdrop=%08" PRIx32 " grid=%ux%u ring=%u\n",
        mission,index+1,st->descriptor,st->width,st->camera_max,st->backdrop,st->columns,st->rows,st->ring!=NULL);
    unsigned grid=(unsigned)st->columns*st->rows;
    hex("grid",st->grid,grid);
    if (st->unknown) hex("unknown",st->unknown,(grid+7)/8);
    fprintf(out,"camera_rom=%06" PRIx32 " script_rom=%06" PRIx32 " players_rom=%06" PRIx32 "\n",st->camera_rom,st->script_rom,st->players_rom);
    hex("camera_list",st->camera_list,st->camera_len);hex("script",st->script,st->script_len);hex("players",st->players,st->players_len);
    for (unsigned i=0;i<4;i++) fprintf(out,"rule %u %u/%u %u/%u %d..%d\n",i,st->rules[i].x_num,st->rules[i].x_den,st->rules[i].y_num,st->rules[i].y_den,st->rules[i].visible_min,st->rules[i].visible_max);
    for (unsigned i=0;i<st->patch_count;i++) fprintf(out,"patch %u submap=%08" PRIx32 " layer_object=%08" PRIx32 "\n",i,st->patch_submap[i],st->patch_layer_object[i]);
    for (unsigned i=0;i<st->dynamic_map_count;i++) {
        fprintf(out,"dynamic_map %u rom=%06" PRIx32 "\n",i,st->dynamic_map_rom[i]);hex("dynamic_map",st->dynamic_map[i],st->dynamic_map_length[i]);
    }
    cells("cell",st->cells,st->cell_count);cells("patch_cell",st->patch_cells,st->patch_cell_count);
    fprintf(out,"cell_patterns %u\n",cell_patterns);
    for (unsigned p=0;p<cell_patterns;p++) {
        fprintf(out,"pattern %u",p);
        for (unsigned k=0;k<8;k++) fprintf(out," %d",scene_cell_patterns[p][k]);
        fputc('\n',out);
    }
    for (unsigned i=0;i<stage_images_count();i++) image("map_image",i,scene_map_image(i),true);
    /* The stage's map chunks are the ones pinned (same chunks: the map is not renumbered). */
    fprintf(out,"map_chunks %u first=%u count=%u\n",map_chunks,stage_map_first,stage_map_count);
    for (unsigned i=0;i<mission_head.palettes;i++) {
        fprintf(out,"palette %u",i);
        for (unsigned k=0;k<16;k++) fprintf(out," %04x",scene_palette(SCENE_PALETTE_COUNT+i)[k]);
        fputc('\n',out);
    }
    /* Actors: each descriptor's parts in order, by content. */
    for (unsigned i=0;i<mission_head.actors;i++) {
        const scene_native_descriptor_t *d=scene_native_descriptor(stage_actors[i].tag);
        bool own=d==&stage_actors[i];
        fprintf(out,"actor %06" PRIx32 " parts=%u own=%u\n",stage_actors[i].tag,stage_actors[i].count,own);
        for (unsigned k=0;k<stage_actors[i].count;k++) {
            image(" part",k,scene_frame(SCENE_NATIVE,stage_actors[i].first+k),false);
        }
    }
    fprintf(out,"actor_images %" PRIu32 "\n",mission_head.actor_images);
    for (unsigned i=0;i<mission_head.resources;i++) {
        int pal=scene_native_palette(stage_resources[i].resource);
        fprintf(out,"resource %04x",stage_resources[i].resource);
        for (unsigned k=0;k<16;k++) fprintf(out," %04x",scene_palette((unsigned)pal)[k]);
        fputc('\n',out);
    }
    /* 'MSXD': ROM blobs and animation programs as the world gets them. */
    for (unsigned i=0;i<mission_blobs;i++) {
        const uint8_t *r=mission_data+12u+12u*i;uint32_t rom_at=rd32(r),len=rd32(r+8);
        /* served: scene_stage_rom answers the blob's first address with these bytes (a dynamic map of the stage may come first). */
        const uint8_t *p=mission_data+rd32(r+4);
        fprintf(out,"blob %06" PRIx32 " served=%u\n",rom_at,len && scene_stage_rom(rom_at)==p);
        hex("blob",p,len);
    }
    for (unsigned i=0;i<stage_program_count;i++) {
        const neo_anim_program_t *pr=scene_stage_program(stage_programs[i].tag);
        assertf(pr==&stage_programs[i].program,"program %06" PRIx32,stage_programs[i].tag);
        fprintf(out,"program %06" PRIx32 " commands=%u\n",stage_programs[i].tag,pr->count);
        for (unsigned k=0;k<pr->count;k++) {
            const neo_anim_command_t *c=&pr->commands[k];
            fprintf(out," %u %d %u %u %u %d %d\n",(unsigned)c->kind,(int)c->asset,c->target,c->operand,c->flags,c->dx,c->dy);
        }
    }
}
int main(int argc,char **argv) {
    if (argc!=3) {fprintf(stderr,"usage: pack-loader DIR OUT\n");return 2;}
    char file[512];
    snprintf(file,sizeof(file),"%s/scene.pack",argv[1]);
    if (!host_rom_add("/scene.pack",file)) {fprintf(stderr,"%s missing\n",file);return 2;}
    bool have[7]={false};
    for (unsigned m=2;m<=6;m++) {
        char path[32];snprintf(path,sizeof(path),"/mission%u.pack",m);snprintf(file,sizeof(file),"%s%s",argv[1],path);
        have[m]=host_rom_add(path,file);
    }
    out=fopen(argv[2],"w");
    if (!out) return 2;
    scene_assets_init();
    dump_scene();
    unsigned stages=0;
    for (unsigned m=2;m<=6;m++) if (have[m]) {
        assertf(scene_mission_load(m),"mission %u pack refused",m);
        fprintf(out,"mission %u stages=%u palettes=%u actors=%" PRIu32 " resources=%" PRIu32 "\n",m,mission_head.stages,mission_head.palettes,mission_head.actors,mission_head.resources);
        unsigned n=mission_head.stages;
        for (unsigned k=0;k<n;k++) {
            assertf(scene_stage_load(k),"mission %u stage %u not loaded",m,k+1);
            dump_stage(m,k);stages++;
        }
        scene_mission_unload();
    }
    if (fclose(out)) return 2;
    printf("pack-loader %s: scene.pack and %u mission stages written (%lu KiB read from the cartridge files)\n",argv[1],stages,host_dma_bytes/1024);
    return 0;
}
