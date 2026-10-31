#!/usr/bin/env python3
"""Per-mission scene pack (build/missionN.pack; romfs/ once the runtime
loads it), docs/MISION2.md.

One self-describing file per mission so the cartridge holds every mission
while RDRAM holds only the current one: the header, stage table, cell,
image and palette tables are read into RAM at the mission start (a few tens
of KiB); the CI4 map pages stay in the ROM in 16 KiB chunks (8 pages,
RLE-packed when smaller, tools/pack_scene.py codec) and stream through the
scene cache like Mission 1's actor chunks. Actor graphics of the mission
(per-mission inventory) are a later section of the same file.

Input per stage: filesystem/missionN/sK/map.json (tools/extract_mission_map.py
--ram-layers) and collision.json (tools/extract_region_collision.py
--descriptor-layers). Coordinate variants (dynamic map writes, still needing
their native events) are counted, not packed.

Layout (big endian, every table 4-byte aligned):
  'MSXM' u16 version=1 u16 mission u16 stages u16 palettes u32 images u32 chunks
  u32 cells_total u32 grid_bytes
  stage[stages]:  u32 descriptor u16 width u16 camera_max u16 backdrop u16 layers
                  u32 first_cell u32 cell_count u16 grid_columns u16 grid_rows
                  i16 grid_origin_x i16 grid_origin_y u32 grid_offset
  cell[]:    u8 layer u8 0 i16 x i16 y u16 image[8]        (24 bytes)
  image[]:   u16 page u16 palette u16 y u16 h              (8 bytes)
  palette[]: u16 rgba5551[16]
  chunk[]:   u32 offset u32 length u32 crc32 u32 codec
  grids (u8 cells, column major; 0 x 0 when the export has unobserved
  cells), then the chunks (64-byte aligned). The runtime loads the tables of
  one stage at a time (stage table offsets)."""
import argparse,hashlib,json,struct,zlib
from pathlib import Path
from mission_tiles import pack_map
from pack_scene import rle,unrle,PAGE
ROOT=Path(__file__).resolve().parents[1]
DESCRIPTORS={2:(0x016DDE,0x016DEE,0x016DFE)}

def build(mission,stage_dirs,crom):
    pages=[];page_ids={};palettes=[];pal_ids={}
    def image(pixels,w,h,dx=0,dy=0,force_actor=False,indices=None,source_palette=None):
        assert indices is not None and w==64 and h==64
        key=tuple(source_palette)+(0,)*(16-len(source_palette))
        if key not in pal_ids:pal_ids[key]=len(palettes);palettes.append(key)
        data=bytearray(PAGE)
        for y in range(64):
            for x in range(64):data[y*32+x//2]|=indices[y*64+x]<<(0 if x&1 else 4)
        data=bytes(data)
        if data not in page_ids:page_ids[data]=len(pages);pages.append(data)
        return (page_ids[data],pal_ids[key],dx,dy,w,h,0)
    stages=[];cells=[];images=[];grids=bytearray();report=[]
    for k,d in enumerate(stage_dirs):
        model=json.loads((d/'map.json').read_text())
        base=len(images)
        st_images,st_cells=pack_map(model,crom,image)
        images.extend((pg,pal,y,h) for pg,pal,_,y,_,h,_ in st_images)
        first=len(cells);cells.extend((layer,x,y,[base+i for i in phases]) for layer,x,y,phases in st_cells)
        col=json.loads((d/'collision.json').read_text())
        grid=bytes.fromhex(col['cells_hex']);assert len(grid)==col['columns']*col['rows']
        # A grid with unobserved cells is not packed (0 x 0): the runtime
        # must not read unknown terrain as empty (docs/MISION2.md).
        usable=not col.get('unknown_cells')
        if not usable:grid=b''
        while len(grids)%4:grids.append(0)
        offset=len(grids);grids.extend(grid)
        backdrops=sorted({o.get('backdrop') for o in model['observations'] if o.get('backdrop') is not None})
        stages.append((DESCRIPTORS.get(mission,(0,)*len(stage_dirs))[k],model['width'],model['camera_max'],backdrops[0] if backdrops else 0,
                       len(model['layers']),first,len(st_cells),col['columns'] if usable else 0,col['rows'] if usable else 0,col['origin_cells'][0],col['origin_cells'][1],offset))
        report.append(dict(stage=d.name,cells=len(st_cells),images=len(st_images),coordinate_variants=len(model['conflicts']),
                           backdrops=backdrops,grid=[col['columns'],col['rows']],grid_packed=usable,grid_unknown=col.get('unknown_cells'),grid_mutable=len(col.get('mutable_cells',[]))))
    real_pages=len(pages)
    while len(pages)%8:pages.append(bytes(PAGE))
    chunks=[];body=bytearray()
    for start in range(0,len(pages),8):
        decoded=b''.join(pages[start:start+8]);packed=rle(decoded)
        if len(packed)<len(decoded):codec=1;assert unrle(packed)==decoded
        else:codec=0;packed=decoded
        chunks.append([len(body),len(packed),zlib.crc32(decoded),codec])
        body.extend(packed);body.extend(bytes((-len(body))%64))
    head=bytearray(b'MSXM')+struct.pack('>HHHHIIII',1,mission,len(stages),len(palettes),len(images),len(chunks),len(cells),len(grids))
    for s in stages:head+=struct.pack('>IHHHHIIHHhhI',*s)
    for layer,x,y,ph in cells:head+=struct.pack('>BBhh8H',layer,0,x,y,*ph)
    for im in images:head+=struct.pack('>4H',*im)
    for p in palettes:head+=struct.pack('>16H',*p)
    table_at=len(head);head+=bytes(16*len(chunks))
    head+=grids;head+=bytes((-len(head))%64)
    for i,c in enumerate(chunks):c[0]+=len(head);struct.pack_into('>4I',head,table_at+16*i,*c)
    blob=bytes(head)+bytes(body)
    return blob,dict(mission=mission,stages=report,pages=real_pages,chunks=len(chunks),palettes=len(palettes),images=len(images),cells=len(cells),
                     header_bytes=len(head),decoded_bytes=len(pages)*PAGE,packed_bytes=len(blob),sha256=hashlib.sha256(blob).hexdigest())

def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('mission',type=int);p.add_argument('stages',type=Path,nargs='+',help='stage directories in play order')
    p.add_argument('--crom',type=Path,default=ROOT/'filesystem/sprites.bin');p.add_argument('--output',type=Path)
    p.add_argument('--report',type=Path)
    a=p.parse_args();blob,report=build(a.mission,a.stages,a.crom.read_bytes())
    # build/ until the runtime loader exists (docs/MISION2.md); then romfs/.
    out=a.output or ROOT/f'build/mission{a.mission}.pack';out.parent.mkdir(exist_ok=True);out.write_bytes(blob)
    if a.report:a.report.write_text(json.dumps(report,indent=1)+'\n')
    print(json.dumps({k:v for k,v in report.items() if k!='stages'}))
    for s in report['stages']:print(' ',s)
if __name__=='__main__':main()
