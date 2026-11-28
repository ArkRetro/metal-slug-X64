/* Mission 3 map reconnaissance (docs/MISION3.md, "Reconocimiento del
 * mapa"): the port's collision data of romfs/mission3.pack against every
 * cell the arcade's kernel read in a whole run of the mission (MAME 0.264,
 * tools/capture_fullgame.lua from states4/sec04_117966 = run4, cells.txt and
 * debug.log, tools/extract_m3_recon.py -> data/m3_map_fixture.json.gz).
 *  1. Terrain 10C72E: each read cell (column already resolved through the
 *     ring window, rows counted from the kernel's y >> 3) equals
 *     neo_collision_cell on the pack's grid, as the runtime reads it, with
 *     the 0C70FC wagon-ledge patches of 0B8FA8 (sub-map 0B85EA) made before
 *     that frame.
 *  2. Train 10D7B6: each read equals neo_kernel_train_ring (map 0B8566).
 *  3. (check-m3sweeps, M3_SWEEPS) every player-1 sweep 01448C -> 013968 ->
 *     014492 of the run replayed by the native kernel with the pack's grid,
 *     the captured scene, train and dynamic objects (their ROM maps).
 * Per section: agreement counts. Usage: m3map romfs/mission3.pack */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../src/neo_collision.h"
#include "../src/neo_kernel.h"
#include "m3_map_fixture.h"
#define N(a) (sizeof(a)/sizeof((a)[0]))
static uint8_t *grid,*grid_dynamic,*pack;static unsigned C,R;static uint32_t recon,recon_length;
static unsigned rd16(const uint8_t *p) {return (unsigned)p[0]<<8|p[1];}
static uint32_t rd32(const uint8_t *p) {return (uint32_t)p[0]<<24|(uint32_t)p[1]<<16|(uint32_t)p[2]<<8|p[3];}
static void load_pack(const char *path) {
    FILE *f=fopen(path,"rb");
    /* Private (ROM-derived) like the ROM itself (tools/test_train_grid.py). */
    if (!f) {printf("SKIP: Mission 3 map check (no %s: tools/pack_mission.py 3, docs/MISION3.md)\n",path);exit(0);}
    fseek(f,0,SEEK_END);long n=ftell(f);fseek(f,0,SEEK_SET);
    uint8_t *p=malloc((size_t)n);assert(p && fread(p,1,(size_t)n,f)==(size_t)n);fclose(f);
    assert(!memcmp(p,"MSXM",4) && rd16(p+6)==3 && rd16(p+8)==1);
    /* Version 4 adds the mission data offset and length (48-byte header). */
    unsigned version=rd16(p+4);assert(version>=3 && version<=5);   /* 5: version 4's tables (docs/COMPRESION.md) */
    const uint8_t *st=p+(version>=4?48:40);   /* the stage record (pack_mission.py) */
    assert(rd32(st)==0x016E0Eu);
    C=rd16(st+20);R=rd16(st+22);int16_t ox=(int16_t)rd16(st+24),oy=(int16_t)rd16(st+26);
    /* The runtime (neo_collision.c) reads row y >> 3 of the grid: the pack
     * must hold the kernel's rows from 0 (pack_mission.py drops the empty
     * rows below an origin). */
    if (ox || oy) {printf("FAIL: grid origin (%d, %d): the runtime reads the grid from row 0\n",ox,oy);exit(1);}
    grid=malloc(C*R);grid_dynamic=malloc(C*R);memcpy(grid,p+rd32(st+28),C*R);
    assert(!rd32(st+32));   /* no unknown cell */
    free(pack);pack=p;recon=rd32(st+118);recon_length=rd32(st+122);
}
/* The reconnaissance blob 'MSXR' of the stage (pack_mission.py recon_blob):
 * the ROM collision of the foreground maps equals the grid, the train map
 * equals neo_kernel_train_cell, the patches are the captured ones. */
static void check_recon(void) {
    if (!recon) {printf("FAIL: no map reconnaissance in the pack\n");exit(1);}
    const uint8_t *b=pack+recon;
    assert(!memcmp(b,"MSXR",4) && rd16(b+4)==1 && b[14]==24);
    unsigned maps=rd16(b+6),npatch=rd16(b+8),dyn=rd16(b+10),secs=rd16(b+12),fg=0,train=0;
    assert(secs==N(m3_sections) && npatch==N(m3_patches) && dyn>0);
    for (unsigned m=0;m<maps;m++) {
        const uint8_t *r=b+16+28*m;uint32_t desc=rd32(r+4);unsigned w=rd16(r+8),h=rd16(r+10),kind=rd16(r+18);
        int x0=(int16_t)rd16(r+12),y0=(int16_t)rd16(r+14);
        if (!(kind&2u)) continue;
        const uint8_t *cells=b+rd32(r+24);assert(rd32(r+24)+4u*w*h<=recon_length);
        for (unsigned i=0;i<w;i++) for (unsigned j=0;j<h;j++) for (unsigned k=0;k<4;k++) {
            uint8_t v=cells[(i*h+j)*4+k];
            if (desc==0x0b853au || desc==0x0b8550u) {
                int top=(240-y0)/16-1,c=2*(x0/16+(int)i)+(int)(k&1u),row=2*top+1-2*(int)j-(int)(k>>1);
                assert(neo_collision_cell(c*8,row*8)==v);fg++;
            } else if (desc==0x0b8566u) {assert(neo_kernel_train_cell(2*(int)i+(int)(k&1u),2*(int)j+(int)(k>>1))==v);train++;}
        }
    }
    for (unsigned i=0;i<npatch;i++) {
        const uint8_t *r=b+16+28*maps+20*i;
        assert(rd32(r)==m3_patches[i].frame && rd32(r+4)==m3_patches[i].submap);
        for (unsigned k=0;k<6;k++) assert(rd16(r+8+2*k)==m3_patches[i].d[k]);
    }
    assert(fg==4u*(240*16+170*10) && train==4u*392*24);
    printf("pack reconnaissance: %u maps, %u foreground ROM cells equal to the grid, %u train ROM cells equal to the kernel's, %u patches, %u dynamic maps, %u sections\n",
        maps,fg,train,npatch,dyn,secs);
}
static unsigned patched;
/* Patches of frames < f; with `same` also those of frame f (the read and
 * the patch run in the same update, either order). */
static void patches_until(uint32_t f) {
    while (patched<N(m3_patches) && m3_patches[patched].frame<f) {
        for (unsigned i=0;i<m3_patches[patched].count;i++) {
            unsigned k=m3_patches[patched].first+i;int c=m3_patch_cells[k].c,r=m3_patch_cells[k].r;
            assert(c>=0 && (unsigned)c<C && r>=0 && (unsigned)r<R);
            grid[(unsigned)c*R+(unsigned)r]=m3_patch_cells[k].v;
        }
        patched++;
    }
}
static bool same_frame_patch(uint32_t f,int c,int r,uint8_t *v) {
    bool any=false;
    for (unsigned p=patched;p<N(m3_patches) && m3_patches[p].frame==f;p++)
        for (unsigned i=0;i<m3_patches[p].count;i++) {
            unsigned k=m3_patches[p].first+i;
            if (m3_patch_cells[k].c==c && m3_patch_cells[k].r==r) {*v=m3_patch_cells[k].v;any=true;}
        }
    return any;
}
static unsigned section_of(uint32_t f) {
    for (unsigned s=0;s<N(m3_sections);s++) if (f>=m3_sections[s].f0 && f<=m3_sections[s].f1) return s;
    return f<m3_sections[0].f0?0:(unsigned)N(m3_sections)-1;
}
typedef struct { uint32_t frame;unsigned i;bool last; } probe_t;
static int by_frame(const void *a,const void *b) {
    const probe_t *x=a,*y=b;return x->frame<y->frame?-1:x->frame>y->frame;
}
#if M3_SWEEPS
static uint8_t object_of(uint32_t a) {
    if (a==0x10e968u) return NEO_KOBJ_ROOT;
    if (a==0x10c72eu) return NEO_KOBJ_TERRAIN;
    if (a==0x10d7b6u) return NEO_KOBJ_TRAIN;
    if (a>=0x10c06eu && a<0x10c06eu+64u*NEO_KDYN_COUNT && !((a-0x10c06eu)&63u)) return (uint8_t)(NEO_KOBJ_DYN0+(a-0x10c06eu)/64u);
    return NEO_KOBJ_NONE;
}
static const uint8_t *map_of(uint32_t rom) {
    for (unsigned i=0;i<N(m3_maps);i++) if (m3_maps[i].rom==rom) return m3_map_bytes+m3_maps[i].at;
    return NULL;
}
#endif
int main(int argc,char **argv) {
    load_pack(argc>1?argv[1]:"romfs/mission3.pack");
    neo_collision_set_grid(grid,grid_dynamic,NULL,C,R);
    check_recon();
    unsigned ns=(unsigned)N(m3_sections);
    unsigned tok[8]={0},tn[8]={0},rok[8]={0},rn[8]={0};
    /* 1. Terrain reads, in frame order (two probes per entry). */
    unsigned np=2u*(unsigned)N(m3_terrain);probe_t *pr=malloc(np*sizeof(*pr));
    for (unsigned i=0;i<N(m3_terrain);i++) {pr[2*i]=(probe_t){m3_terrain[i].f0,i,false};pr[2*i+1]=(probe_t){m3_terrain[i].f1,i,true};}
    qsort(pr,np,sizeof(*pr),by_frame);
    unsigned bad=0;
    for (unsigned k=0;k<np;k++) {
        const probe_t *p=&pr[k];patches_until(p->frame);
        int c=m3_terrain[p->i].c,r=m3_terrain[p->i].r;uint8_t v=m3_terrain[p->i].v,alt;
        uint8_t got=neo_collision_cell(c*8,r*8);
        bool ok=got==v || (same_frame_patch(p->frame,c,r,&alt) && alt==v);
        if (p->last) {unsigned s=section_of(m3_terrain[p->i].f0);tn[s]++;tok[s]+=ok;}
        if (!ok && bad++<10) printf("terrain cell (%d, %d) frame %u: port %02X, arcade %02X\n",c,r,p->frame,got,v);
    }
    free(pr);
    /* 2. Train reads. */
    for (unsigned i=0;i<N(m3_train);i++) {
        uint8_t got=neo_kernel_train_ring((uint16_t)(8*m3_train[i].c),(uint16_t)~(uint16_t)(m3_train[i].q<<3),0);
        unsigned s=section_of(m3_train[i].f0);rn[s]++;rok[s]+=got==m3_train[i].v;
        if (got!=m3_train[i].v && bad++<20) printf("train column %d slot %d: port %02X, arcade %02X\n",m3_train[i].c,m3_train[i].q,got,m3_train[i].v);
    }
#if M3_SWEEPS
    /* 3. The player's sweeps (grid state restarts from the pack). */
    load_pack(argc>1?argv[1]:"romfs/mission3.pack");neo_collision_set_grid(grid,grid_dynamic,NULL,C,R);patched=0;
    static const uint16_t rom_box[4]={0x1c,0,0x108,0x1e0};
    unsigned sok[8]={0},sn[8]={0},dok[8]={0},dn[8]={0},shown=0;
    for (unsigned i=0;i<N(m3_sweeps);i++) {
        const __typeof__(m3_sweeps[0]) *s=&m3_sweeps[i];
        patches_until(s->frame);
        neo_kscene_t k;memset(&k,0,sizeof(k));
        k.terrain=(neo_kobj_t){(int32_t)s->scene[0],(int32_t)s->scene[1],(int32_t)s->scene[2],(int32_t)s->scene[3]};
        k.root=(neo_kobj_t){(int32_t)s->scene[4],(int32_t)s->scene[5],(int32_t)s->scene[6],(int32_t)s->scene[7]};
        k.train=(neo_kobj_t){(int32_t)s->scene[8],(int32_t)s->scene[9],(int32_t)s->scene[10],(int32_t)s->scene[11]};
        k.train_on=s->train_map==0xb8566u;k.train_origin=s->origin;
        k.b400c=(uint8_t)s->in[10];k.w4068=(uint16_t)s->in[12];k.w4018=(uint16_t)s->in[13];k.shake_y=s->shake;
        neo_kernel_dyn_begin(&k);bool dynamic=false;
        for (unsigned d=0;d<NEO_KDYN_COUNT;d++) {
            /* A released slot (map 0) keeps its position: a sweep from a
             * parent released in that update converts through it (MAME
             * frame 120775, slot 6 after 029CDC). */
            k.dyn[d].pos=(neo_kobj_t){(int32_t)s->dyn[d][1],(int32_t)s->dyn[d][2],(int32_t)s->dyn[d][3],(int32_t)s->dyn[d][4]};
            if (!s->dyn[d][0]) continue;
            k.dyn[d].map=map_of(s->dyn[d][0]);assert(k.dyn[d].map);
            neo_kernel_dyn_update(&k,d);dynamic=true;
        }
        neo_kreq_t q={(int32_t)s->in[0],(int32_t)s->in[1],(int32_t)s->in[2],(int32_t)s->in[3],object_of(s->in[4]),s->in[5],
            (uint8_t)s->in[6],(uint8_t)s->in[7],(uint8_t)s->in[8],(uint8_t)s->in[9],(uint8_t)s->in[11]};
        neo_kres_t r;bool ran;
        if (!memcmp(s->box,rom_box,sizeof(rom_box))) ran=neo_kernel_player_sweep(&k,&q,&r);
        else {memcpy(k.config,s->box,sizeof(k.config));ran=neo_kernel_sweep(&k,&q,&r);}
        bool ok=ran && (uint32_t)r.x==s->out[0] && (uint32_t)r.y==s->out[1] && (uint32_t)r.vx==s->out[2] && (uint32_t)r.vy==s->out[3] &&
            r.parent==object_of(s->out[4]) && r.w4068==(uint16_t)s->out[5] && r.flags==(uint16_t)s->out[6] && r.kind==s->out[7];
        unsigned sec=s->section;sn[sec]+=s->count;sok[sec]+=ok?s->count:0;
        if (dynamic) {dn[sec]+=s->count;dok[sec]+=ok?s->count:0;}
        if (!ok && shown++<12)
            printf("sweep frame %u (%s): %08X %08X %08X %08X parent %u flags %04X kind %08X, MAME %08X %08X %08X %08X parent %u flags %04X kind %08X\n",
                s->frame,ran?"ran":"unsupported",(unsigned)r.x,(unsigned)r.y,(unsigned)r.vx,(unsigned)r.vy,r.parent,r.flags,(unsigned)r.kind,
                s->out[0],s->out[1],s->out[2],s->out[3],object_of(s->out[4]),s->out[6]&0xffffu,s->out[7]);
    }
#endif
    unsigned all_t=0,all_tok=0,all_r=0,all_rok=0;
    for (unsigned s=0;s<ns;s++) {
        printf("section %s %-30s map %s camera %5d..%5d: terrain %u/%u, train %u/%u",m3_sections[s].id,m3_sections[s].name,m3_sections[s].map,
            m3_sections[s].cam0,m3_sections[s].cam1,tok[s],tn[s],rok[s],rn[s]);
#if M3_SWEEPS
        printf(", sweeps %u/%u (%.2f%%; with dynamic objects %u/%u)",sok[s],sn[s],sn[s]?100.0*sok[s]/sn[s]:100.0,dok[s],dn[s]);
#endif
        printf("\n");all_t+=tn[s];all_tok+=tok[s];all_r+=rn[s];all_rok+=rok[s];
    }
#if M3_SWEEPS
    for (unsigned s=0;s<ns;s++) if (sok[s]!=sn[s]) {printf("FAIL: section %s: %u sweeps differ from MAME\n",m3_sections[s].id,sn[s]-sok[s]);return 1;}
#endif
    if (all_tok!=all_t || all_rok!=all_r) {printf("FAIL: %u terrain and %u train reads differ from the arcade\n",all_t-all_tok,all_r-all_rok);return 1;}
    printf("PASS: Mission 3 collision of the pack equals the arcade's kernel reads (%u terrain cells, %u train cells, %u patches 0B85EA)%s\n",
        all_t,all_r,(unsigned)N(m3_patches),M3_SWEEPS?"; every player sweep bit-exact":"");
    return 0;
}
