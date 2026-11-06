#!/usr/bin/env python3
"""Pre-rendered backgrounds, feasibility prototype (docs/RENDIMIENTO.md
"Fondos pre-renderizados (prototipo)").

Reads a private mission pack (romfs/missionN.pack, tools/pack_mission.py)
and, per stage and layer, composes the map cells into layer space.

  estimate PACK...           sizes per stage/layer: CI4 blocks (today's
                             texels without the page dedup), CI8, bounding
                             box, pinned map chunks today, chunk working set
                             of a screen window, streaming ring RAM.
  render PACK STAGE OUT      the streamed column file of one stage ('MSXB',
                             read by src/scene_prerender.c): every static
                             cell of every layer laid out by 16-px column,
                             CI4 texels as the pack's pages hold them, runs
                             of consecutive cells with one palette. Animated
                             cells (phase patterns) stay on the tile path.

'MSXB' layout (big endian): 'MSXB' u16 version=1 u16 stage u16 layers u16 0 u32 0
  layer[4] (16 bytes): i16 x0 (column 0's x) u16 columns u16 slot_bytes
    (largest column) u16 0 u32 table_offset u32 0
  per layer column table: u32 offset u16 bytes u16 runs (bytes 0: empty)
  column data (16-byte aligned): run[runs] (i16 y u8 n u8 0 u16 palette
    u16 0: n cells from y, 16*n texel rows), padded to 16, then the runs'
    CI4 texels (128 bytes per cell, 8-byte rows, the pages' layout).
"""
import argparse,struct,sys,zlib
from pathlib import Path
CHUNK=16384;PAGE=2048

def unrle(data):
    out=bytearray()
    for i in range(0,len(data),2):out+=bytes([data[i+1]])*data[i]
    return bytes(out)

class Pack:
    def __init__(self,path):
        self.raw=Path(path).read_bytes();b=self.raw
        assert b[:4]==b'MSXM'
        (ver,self.mission,ns,npal,nimg,nchunk,ncell,_g,_a,_ai,_r,_z,_dl)=struct.unpack_from('>HHHHIIIIIIIII',b,4)
        o=48;self.stages=[]
        for k in range(ns):
            r=struct.unpack_from('>IHHIIIHHhhIIII',b,o+128*k)
            rules=[struct.unpack_from('>BBBBhh',b,o+128*k+44+36+8*i) for i in range(4)]
            po,pc=struct.unpack_from('>IH',b,o+128*k+44+36+32)
            self.stages.append(dict(descriptor=r[0],width=r[1],camera_max=r[2],first=r[4],count=r[5],rows=r[7],
                                    image_first=r[12],image_count=r[13],rules=rules,patches=pc))
        o+=128*ns;self.cells_at=o;o+=22*ncell;self.images_at=o;o+=8*nimg;self.pal_at=o;o+=32*npal;self.chunks_at=o
        self.ncell=ncell;self.chunks=[struct.unpack_from('>4I',b,self.chunks_at+16*i) for i in range(nchunk)];self.cache={}
    def cells(self,k):
        st=self.stages[k];out=[]
        # A stage's cells run to the next stage's first (patch cells included).
        end=self.stages[k+1]['first'] if k+1<len(self.stages) else self.ncell
        for i in range(st['first'],end):
            r=struct.unpack_from('>BBhh8H',self.raw,self.cells_at+22*i)
            out.append((r[0],r[2],r[3],[p for p in r[4:12]]))
        return out
    def image(self,i):return struct.unpack_from('>4H',self.raw,self.images_at+8*i)   # page, palette, y, h
    def page(self,p):
        c=p//8
        if c not in self.cache:
            off,ln,crc,codec=self.chunks[c];d=self.raw[off:off+ln];d=unrle(d) if codec else d
            assert len(d)==CHUNK and zlib.crc32(d)==crc;self.cache[c]=d
        return self.cache[c][(p%8)*PAGE:(p%8+1)*PAGE]
    def texels(self,img):
        page,pal,y,h=self.image(img);return self.page(page)[8*y:8*(y+16)],pal

def layer_cells(pack,k):
    """Static cells per layer {layer: {(x,y): image}}, animated and patch cells counted."""
    lay={};anim={};patch=0
    for layer,x,y,ph in pack.cells(k):
        if layer&0x80:patch+=1;continue
        if len(set(ph))>1:anim[layer]=anim.get(layer,0)+1;continue
        lay.setdefault(layer,{})[(x,y)]=ph[0]
    return lay,anim,patch

def columns(pack,cells):
    """Column x -> list of runs (y, [images], palette): consecutive cells 16 px apart, one palette, <= 16 cells."""
    cols={}
    for (x,y),img in cells.items():cols.setdefault(x,[]).append((y,img))
    out={}
    for x,lst in sorted(cols.items()):
        lst.sort();runs=[]
        for y,img in lst:
            pal=pack.image(img)[1]
            if runs and runs[-1][0]+16*len(runs[-1][1])==y and runs[-1][2]==pal and len(runs[-1][1])<16:runs[-1][1].append(img)
            else:runs.append((y,[img],pal))
        out[x]=runs
    return out

def column_bytes(runs):
    head=(8*len(runs)+15)&~15
    return head+128*sum(len(r[1]) for r in runs)

def estimate(paths):
    for path in paths:
        pk=Pack(path)
        for k,st in enumerate(pk.stages):
            lay,anim,patch=layer_cells(pk,k)
            chunks=set()
            for layer,x,y,ph in pk.cells(k):
                for p in ph:chunks.add(pk.image(p)[0]//8)
            # Chunks the tile path keeps for animated and patch cells, and the
            # largest set of chunks one 432-px window of a layer touches (an
            # unpinned, camera-streamed tile cache would need about this).
            resid=set();ws=0
            for layer,x,y,ph in pk.cells(k):
                if layer&0x80 or len(set(ph))>1:
                    for p in ph:resid.add(pk.image(p)[0]//8)
            # The same images repacked densely (16 per 16x256 page, 8 pages a chunk).
            rimg={p for layer,x,y,ph in pk.cells(k) if layer&0x80 or len(set(ph))>1 for p in ph}
            dense=-(-len(rimg)//128)
            bylayer={}
            for layer,x,y,ph in pk.cells(k):
                if not layer&0x80:bylayer.setdefault(layer,[]).append((x,{pk.image(p)[0]//8 for p in ph}))
            for layer,lst in bylayer.items():
                xs=sorted({x for x,_ in lst});best=0
                for x0 in xs[::4]:
                    best=max(best,len(set().union(*[c for x,c in lst if x0<=x<x0+432])))
                ws+=best
            tot_ci4=tot_ci8=ring=0;line=[]
            for layer in sorted(lay):
                cells=lay[layer];xs=[x for x,_ in cells];ys=[y for _,y in cells]
                pals={pk.image(i)[1] for i in cells.values()}
                cols=columns(pk,cells);slot=max(column_bytes(r) for r in cols.values())
                ci4=sum(column_bytes(r) for r in cols.values());ci8=256*len(cells)
                bbox=(max(xs)+16-min(xs))*(max(ys)+16-min(ys))
                tall=max(ys)+16-min(ys)>480
                # Ring: 304 px + 64 px each side + 2 columns; a tall layer
                # (vertical scroll) keeps a 2-D window of 224+128 px rows.
                ncol=(304+128)//16+2
                rb=ncol*(((224+128)//16+1)*128+32) if tall else ncol*slot
                ring+=rb;tot_ci4+=ci4;tot_ci8+=ci8
                line.append(f'L{layer}:{len(cells)}c/{len(pals)}pal/anim{anim.get(layer,0)} {xs and min(xs)}..{max(xs)+16}x{min(ys)}..{max(ys)+16} ci4={ci4//1024}K ci8={ci8//1024}K bbox8={bbox//1024}K ring={rb//1024}K')
            print(f'M{pk.mission} s{k+1} w={st["width"]} pinned={len(chunks)}ch={len(chunks)*16}K resid={len(resid)}ch dense={dense}ch window={ws}ch patchcells={patch} ROM ci4={tot_ci4//1024}K ci8={tot_ci8//1024}K ring={ring//1024}K')
            for l in line:print('  '+l)

def render(path,k,out):
    pk=Pack(path);lay,_,_=layer_cells(pk,k)
    head=bytearray(b'MSXB'+struct.pack('>HHHHI',1,k,sum(1 for l in lay if l<4),0,0))
    head+=bytes(16*4);body=bytearray();tables=bytearray();layer_tables=[]
    for layer in range(4):
        if layer not in lay:continue
        cols=columns(pk,lay[layer]);x0=min(cols);n=(max(cols)-x0)//16+1;slot=0
        table=[]
        for c in range(n):
            runs=cols.get(x0+16*c)
            if not runs:table.append((0,0,0));continue
            while len(body)%16:body.append(0)
            at=len(body);rec=bytearray()
            for y,imgs,pal in runs:rec+=struct.pack('>hBBHH',y,len(imgs),0,pal,0)
            while len(rec)%16:rec.append(0)
            for y,imgs,pal in runs:
                for i in imgs:rec+=pk.texels(i)[0]
            body+=rec;slot=max(slot,len(rec));table.append((at,len(rec),len(runs)))
        layer_tables.append((layer,x0,n,slot,table))
    o=len(head)
    for layer,x0,n,slot,table in layer_tables:
        struct.pack_into('>hHHHII',head,16+16*layer,x0,n,(slot+15)&~15,0,o+len(tables),0)
        tables+=b''.join(struct.pack('>IHH',a,b,c) for a,b,c in table)
    while (len(head)+len(tables))%16:tables.append(0)
    base=len(head)+len(tables)
    fixed=bytearray(tables)
    for layer,x0,n,slot,table in layer_tables:
        at=struct.unpack_from('>I',head,16+16*layer+8)[0]-len(head)
        for c,(a,b,r) in enumerate(table):
            if b:struct.pack_into('>I',fixed,at+8*c,base+a)
    data=bytes(head)+bytes(fixed)+bytes(body)
    Path(out).write_bytes(data)
    print(f'{out}: {len(data)} bytes, layers '+' '.join(f'L{l}:{n}col/slot{s}' for l,_,n,s,_ in layer_tables))

def main():
    ap=argparse.ArgumentParser();sub=ap.add_subparsers(dest='cmd',required=True)
    e=sub.add_parser('estimate');e.add_argument('packs',nargs='+')
    r=sub.add_parser('render');r.add_argument('pack');r.add_argument('stage',type=int);r.add_argument('out')
    a=ap.parse_args()
    if a.cmd=='estimate':estimate(a.packs)
    else:render(a.pack,a.stage-1,a.out)
if __name__=='__main__':main()
