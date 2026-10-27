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
    groups=defaultdict(set)
    for c in model['cells']:
        for phase in range(c['animation_mask']+1):
            groups[c['palette']].add((c['code']|phase,c['flip']))
    lookup={};images=[]
    for pal_id,tiles in sorted(groups.items()):
        palette=rgba16(model['palettes'][pal_id]);tiles=sorted(tiles)
        for start in range(0,len(tiles),16):
            atlas=[0]*4096;batch=tiles[start:start+16]
            for slot,(code,flip) in enumerate(batch):
                pens=decode(crom,code,flip);sx=(slot%4)*16;sy=(slot//4)*16
                for y in range(16):atlas[(sy+y)*64+sx:(sy+y)*64+sx+16]=pens[y*16:y*16+16]
            packed=make_image([palette[p] for p in atlas],64,64,indices=atlas,source_palette=palette)
            for slot,(code,flip) in enumerate(batch):
                lookup[(pal_id,code,flip)]=len(images)
                images.append((packed[0],packed[1],(slot%4)*16,(slot//4)*16,64,64,0))
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
