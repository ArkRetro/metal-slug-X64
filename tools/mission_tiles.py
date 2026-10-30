"""Decode original C-ROM cells; group CI4 tiles into palette-specific TMEM atlases."""
from collections import defaultdict
from export_marco import palette_rgb

def rgba16(palette):
    result=[0]
    for word in palette[1:]:
        r,g,b=palette_rgb(word)
        result.append((r>>3)<<11|(g>>3)<<6|(b>>3)<<1|1)
    return result

def decode(crom,code,flip):
    assert 0<=code<len(crom)//128
    out=[]
    for y in range(16):
        for x in range(16):
            sx=15-x if flip&1 else x;sy=15-y if flip&2 else y
            offset=code*128+(64 if sx<8 else 0)+sy*4
            out.append(sum(((crom[offset+b]>>(sx&7))&1)<<n for n,b in enumerate((0,2,1,3))))
    return out

def pack_map(model,crom,make_image):
    # Atlases per palette and map segment (0 = the region map, 1 = cells only
    # in the rest of the mission): the region keeps its original pages, so
    # its per-frame page/TLUT uploads do not change with the longer map.
    segment={}
    for c in model['cells']:
        for phase in range(c['animation_mask']+1):
            k=(c['palette'],c['code']|phase,c['flip'])
            segment[k]=min(segment.get(k,1),c.get('segment',0))
    groups=defaultdict(set)
    for (pal,code,flip),seg in segment.items():groups[(seg,pal)].add((code,flip))
    lookup={};images=[]
    for (seg,pal_id),tiles in sorted(groups.items()):
        palette=rgba16(model['palettes'][pal_id]);tiles=sorted(tiles)
        # Tall pages: 16x256 CI4 (2048 bytes, stride 8), the 16 tiles of a
        # batch stacked in code order. Neo Geo map columns usually hold
        # consecutive codes top to bottom, so vertically adjacent cells are
        # adjacent texels and the renderer draws them as one rectangle
        # (format 2). The bytes are built through the 64x64 page helper: tall
        # row ty is bytes ty*8.., i.e. 64-wide row ty//4, columns (ty%4)*16..
        for start in range(0,len(tiles),16):
            atlas=[0]*4096;batch=tiles[start:start+16]
            for slot,(code,flip) in enumerate(batch):
                pens=decode(crom,code,flip)
                for y in range(16):
                    ty=slot*16+y;row=ty//4;col=(ty%4)*16
                    atlas[row*64+col:row*64+col+16]=pens[y*16:y*16+16]
            packed=make_image([palette[p] for p in atlas],64,64,indices=atlas,source_palette=palette)
            for slot,(code,flip) in enumerate(batch):
                lookup[(pal_id,code,flip)]=len(images)
                images.append((packed[0],packed[1],0,slot*16,16,256,2))
    cells=[]
    for c in model['cells']:
        phases=[lookup[(c['palette'],c['code']|(phase&c['animation_mask']),c['flip'])] for phase in range(8)]
        cells.append((c['layer'],c['x'],c['y'],phases))
    # Background cells do not overlap. Props (virtual layer 4) MUST retain SCB
    # order: canopy/cart tiles overlap, so texture sorting would change pixels.
    cells=[c for _,c in sorted(enumerate(cells),key=lambda item:
        (item[1][0],0,0,item[0],0) if item[1][0]==4 else
        (item[1][0],images[item[1][3][0]][0],images[item[1][3][0]][1],item[1][1],item[1][2]))]
    return images,cells
