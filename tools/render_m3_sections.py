#!/usr/bin/env python3
"""Offline renders of every Mission 3 section from the ROM layer export
(filesystem/mission3/s1/layers_rom.json, tools/export_m3_layers.py) against
MAME (docs/MISION3.md, "Reconocimiento del mapa").

For snapshots of run4 inside each section (vram/palette/game<f>, the layer
offsets of map.json's observation of that frame):
* blocks: every background SCB block MAME had on screen (the layer objects'
  slot ranges) against the ROM map block predicted at that screen position
  (code without the auto-animation bits, flip, palette number). The train
  layer (its columns loop with +32) is matched by column identity: the
  sprite's blocks must be one column of 0B8566. The sky after progress 0x140
  has no map (another writer): counted apart, not compared.
* pixels: the ROM render against the render of MAME's background SCBs
  (304 x 224, x 8..311), and an image per frame: MAME screenshot | ROM
  render | MAME background SCBs (build/m3_renders/, private).
Output: --report JSON with the counts per section."""
import argparse,gzip,json,struct,sys
from collections import Counter,defaultdict
from functools import lru_cache
from pathlib import Path
from PIL import Image
sys.path.insert(0,str(Path(__file__).resolve().parent))
from export_marco import palette_rgb
from mission_tiles import decode
ROOT=Path(__file__).resolve().parents[1]
import os
DEBUG=os.getenv('M3_RENDER_DEBUG')=='1'
def main():
    ap=argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--layers',type=Path,default=ROOT/'filesystem/mission3/s1/layers_rom.json')
    ap.add_argument('--model',type=Path,default=ROOT/'filesystem/mission3/s1/map.json')
    ap.add_argument('--fixture',type=Path,default=ROOT/'data/m3_map_fixture.json.gz')
    ap.add_argument('--crom',type=Path,default=ROOT/'filesystem/sprites.bin')
    ap.add_argument('--per-section',type=int,default=4)
    ap.add_argument('--output',type=Path,default=ROOT/'build/m3_renders')
    ap.add_argument('--report',type=Path)
    a=ap.parse_args()
    lay=json.loads(a.layers.read_text());model=json.loads(a.model.read_text());crom=a.crom.read_bytes()
    sections=json.load(gzip.open(a.fixture))['sections']
    rom=defaultdict(dict);train_cols={};train_by_col={}
    # The sky map 0B857C (32 block columns) repeats along the layer (the
    # layer reloads its columns from the map modulo its width, 0C72EC).
    WRAP={m['layer']:16*m['block_columns'] for m in lay['maps'] if m['descriptor']=='0B857C'}
    overlay=set()
    for m in lay['maps']:
        if m['layer']==2:overlay.update((code&~anim,flip) for x,y,code,flip,anim,pal in m['cells'])
        if m.get('placement','').startswith('0C70FC'):continue
        for x,y,code,flip,anim,pal in m['cells']:
            if m['layer']==1:train_by_col.setdefault(x//16,{})[y]=(code,flip,anim,pal)
            else:rom[m['layer']][(x,y)]=(code,flip,anim,pal)
    for i,col in train_by_col.items():
        train_cols.setdefault(tuple(sorted((y,c[0]&~c[2],c[1]) for y,c in col.items())),[]).append(i)
    obs={o['frame']:o for o in model['observations']}
    @lru_cache(None)
    def tile(code,flip,palette):
        colors=[(0,0,0,0)]+[palette_rgb(w)+(255,) for w in palette[1:]]
        im=Image.new('RGBA',(16,16));im.putdata([colors[p] for p in decode(crom,code,flip)]);return im
    a.output.mkdir(parents=True,exist_ok=True)
    report=[];pal_pairs=Counter()
    for s in sections:
        frames=[f for f in sorted(obs) if s['frames'][0]<=f<=s['frames'][1]]
        pick=[frames[round(k*(len(frames)-1)/max(1,a.per_section-1))] for k in range(a.per_section)] if frames else []
        stat=Counter()
        for f in sorted(set(pick)):
            o=obs[f];cap=Path(o['captures'])
            v=struct.unpack('>34304H',(cap/f'vram{f}.bin').read_bytes());p=struct.unpack('>4096H',(cap/f'palette{f}.bin').read_bytes())
            phase=o.get('animation_phase',f&7);off=o['offsets'];vert=o['vertical_offsets'];active=o['active']
            starts={int(k):b for k,b in o['background_slots'].items()};banks=set(o.get('banks',(4,)))
            fill=palette_rgb(o.get('backdrop',0))+(255,)
            ref=Image.new('RGBA',(320,224),fill);ren=Image.new('RGBA',(320,224),fill);part={L:Image.new('RGBA',(320,224)) for L in range(4)}
            # Background SCBs per layer, in slot order (the drawing order).
            x=y=rows=0;blocks=[];refc_part={L:[] for L in range(4)};span={}
            for slot in range(1,381):
                control=v[0x8200+slot]
                if control&64:x=(x+16)&511
                else:x=v[0x8400+slot]>>7;y=(512-(control>>7))&511;rows=control&63
                layer=next((L for L,b in starts.items() if b<=slot<b+32),None)
                if layer is None or not rows:continue
                xx=x-512 if x>=496 else x
                if xx>=320 or xx<=-16:continue
                col=[]
                for row in range(min(rows,32)):
                    yy=(y+row*16)&511
                    if yy>=496:yy-=512
                    yy-=16
                    low,attr=v[slot*64+row*2:slot*64+row*2+2]
                    if not ((attr&0xf0)>>4 in banks):continue
                    mask=7 if attr&8 else 3 if attr&4 else 0
                    code=low|((attr<<12)&0xf0000)
                    col.append((yy,code,attr,mask))
                    if -16<yy<224 and active[layer] and not (layer==0 and o['map_origins'][0][0]=='0x000000'):
                        im=tile((code&~mask)|(phase&mask),attr&3,tuple(p[(attr>>8)*16:(attr>>8)*16+16]));refc_part[layer].append((im,xx,yy))
                    if -16<yy<224:ref.paste(tile((code&~mask)|(phase&mask),attr&3,tuple(p[(attr>>8)*16:(attr>>8)*16+16])),(xx,yy),tile((code&~mask)|(phase&mask),attr&3,tuple(p[(attr>>8)*16:(attr>>8)*16+16])))
                blocks.append((layer,xx,col))
                # The layer's strips: the screen blocks the hardware draws for
                # the layer (code 0 included), whatever the map holds there.
                span.setdefault(layer,set()).update((xx,yy) for yy,*_ in col)
            for layer,xx,col in blocks:
                if not active[layer]:continue
                if layer==0 and o['map_origins'][0][0]=='0x000000':
                    stat['sky_other_writer']+=sum(1 for yy,*_ in col if -16<yy<224);continue
                if layer==1:
                    key=tuple(sorted((yy-vert[1],code&~mask,attr&3) for yy,code,attr,mask in col if (code&~mask) and -16<yy<224))
                    # The column's nonzero blocks must be a 0B8566 column (subset).
                    found=None
                    # Exact on the visible rows of the strip: MAME's code (0 =
                    # empty) equals the ROM column's at that row.
                    vis_rows=[(yy-vert[1],code&~mask,attr&3) for yy,code,attr,mask in col if -16<yy<224]
                    for i,cells in train_by_col.items():
                        if all(((cells[my][0]&~cells[my][2]),cells[my][1])==(c,fl) if my in cells else c==0 for my,c,fl in vis_rows):found=i;break
                    vis=sum(1 for yy,code,*_ in col if -16<yy<224)
                    stat['train_columns']+=1;stat['train_columns_rom']+=found is not None
                    if found is None and DEBUG:
                        best=max(((sum(1 for my,c,fl in key if cells.get(my,(None,))[0] is not None and (cells[my][0]&~cells[my][2])==c and cells[my][1]==fl),i) for i,cells in train_by_col.items()))
                        cells=train_by_col[best[1]]
                        bad=[(my,c) for my,c,fl in key if not (cells.get(my,(None,))[0] is not None and (cells[my][0]&~cells[my][2])==c)]
                        where=[(my,[i for i,cc in train_by_col.items() if cc.get(my,(None,))[0] is not None and (cc[my][0]&~cc[my][2])==c][:4]) for my,c in bad]
                        print('train column',f,xx,len(key),'best',best,'bad rows',where)
                    if found is not None:
                        for yy,code,attr,mask in col:
                            my=yy-vert[1]
                            if -16<yy<224 and my in train_by_col[found] and code&~mask:
                                stat['train_blocks']+=1;rp=train_by_col[found][my][3]
                                if rp==attr>>8:stat['train_blocks_palette']+=1
                                else:pal_pairs[(rp,attr>>8,my)]+=1
                        for my,(code,flip,anim,pal) in train_by_col[found].items():
                            yy=my+vert[1]
                            if -16<yy<224 and any(yy==vy+vert[1] for vy,_,_ in vis_rows):im=tile(code|(phase&anim),flip,tuple(p[pal*16:pal*16+16]));part[1].paste(im,(xx,yy),im)
                    continue
                for yy,code,attr,mask in col:
                    if not -16<yy<224:continue
                    mx,my=xx+off[layer],yy-vert[layer]
                    r=rom[layer].get((mx%WRAP[layer] if layer in WRAP else mx,my))
                    stat['blocks_%d'%layer]+=1
                    if r and (r[0]&~r[2])==(code&~mask) and r[1]==attr&3 and r[3]==attr>>8:stat['blocks_%d_rom'%layer]+=1
                    elif not r and not (code&~mask):stat['blocks_%d_rom'%layer]+=1
                    elif layer==2 and ((code&~mask),attr&3) in overlay:stat['blocks_2_rom_patch']+=1
                    elif DEBUG:print('block',layer,f,xx,yy,mx,my,hex(code),attr,r)
            # The ROM render of layers 0, 2, 3 over the whole screen.
            order=sorted(starts,key=lambda L:starts[L])
            for L in order:
                if L==1 or not active[L] or (L==0 and o['map_origins'][0][0]=='0x000000'):continue
                cover=span.get(L,set())
                for (mx,my),(code,flip,anim,pal) in rom[L].items():
                    for rep in range(-1,40) if L in WRAP else (0,):
                        sx,sy=mx+rep*WRAP.get(L,0)-off[L],my+vert[L]
                        if -16<sx<320 and -16<sy<224 and code and (sx,sy) in cover:
                            im=tile(code|(phase&anim),flip,tuple(p[pal*16:pal*16+16]));part[L].paste(im,(sx,sy),im)
            for L in order:
                if active[L]:ren.alpha_composite(part[L])
            # Comparable reference: MAME's background SCBs of the layers the
            # ROM maps draw (not the sky of the other writer), slot order.
            refc=Image.new('RGBA',(320,224),fill)
            for L in order:
                for im,xx,yy in refc_part[L]:refc.paste(im,(xx,yy),im)
            same=sum(1 for yy in range(224) for xx in range(8,312) if ren.getpixel((xx,yy))==refc.getpixel((xx,yy)))
            stat['pixels']+=304*224;stat['pixels_equal']+=same
            if DEBUG and same<304*224:
                diff=[(xx,yy) for yy in range(224) for xx in range(8,312) if ren.getpixel((xx,yy))!=refc.getpixel((xx,yy))]
                print('pixels',f,len(diff),min(diff),max(diff),Counter((xx//16,yy//16) for xx,yy in diff).most_common(6))
                px,py=Counter((xx,yy) for xx,yy in diff).most_common(1)[0][0]
                for L in range(4):
                    for im,bx,by in refc_part[L]:
                        if bx<=px<bx+16 and by<=py<by+16:print('  ref layer',L,'block',bx,by,im.getpixel((px-bx,py-by)))
                    if part[L].getpixel((px,py))[3]:print('  ren layer',L,part[L].getpixel((px,py)))
            shot=Image.open(cap/f'game{f}.png').convert('RGBA')
            out=Image.new('RGBA',(960,224));out.paste(shot,(0,0));out.paste(ren,(320,0));out.paste(ref,(640,0))
            out.save(a.output/f'section{s["id"]}_{f}.png')
            stat['renders']+=1
        report.append(dict(section=s['id'],name=s['name'],frames=sorted(set(pick)),**stat))
        print(report[-1])
    if DEBUG:print('train palette pairs (rom, mame, model y)',pal_pairs.most_common(20))
    if a.report:a.report.write_text(json.dumps(report,indent=1)+'\n')
if __name__=='__main__':main()
