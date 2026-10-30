#!/usr/bin/env python3
"""Export the sprite descriptors drawn by the native boss programs
(data/boss_programs.json) from the original P-ROM and C-ROM, with the
logical palette resources the boss allocates, and verify them pixel by
pixel against MAME VRAM/palette dumps of a recorded fight.

Verification: for every dump frame of the capture, the draws the native
boss emitted for that video frame in the fight fixture (descriptor, x, y,
attributes; tools/test_neo_boss.c checks them against the original sprite
list) are rasterized from the exported pens and compared with the sprite
image rebuilt from VRAM for the boss palette slots (tools/export_marco.py
reconstruct). A boss pixel must have the same palette slot and pen as the
VRAM sprite; VRAM pixels of those slots that no native draw covers belong to
generic actors sharing the slot (shell trails, explosions) and are counted
apart. Output: filesystem/boss-native.json (same layout as
filesystem/soldier-native.json) and the verification summary."""
import argparse,gzip,hashlib,json,struct,collections
from pathlib import Path
from export_marco import ROOT,reconstruct
from export_soldier_sprites import program,descriptor,resources
from mission_tiles import rgba16
RESOURCES=(0x2ec,0x2ed,0x2ee,0x2ef,0x319,0x31a,0x31b,0x347,0x223,0x357,0x358,0x359,0x393)
def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('zip',type=Path);p.add_argument('capture',type=Path)
    p.add_argument('--fixture',type=Path,default=ROOT/'data/boss_fight_a.json.gz')
    p.add_argument('--frames',default='10230:13740:30');p.add_argument('--report',type=Path,default=ROOT/'docs/boss-sprites-validation.json')
    a=p.parse_args()
    data=program(a.zip);crom=(ROOT/'filesystem/sprites.bin').read_bytes()
    progs=json.loads((ROOT/'data/boss_programs.json').read_text())['programs']
    assets=[]
    for prog in progs:
        for c in prog['commands']:
            if c['kind']=='POSE' and c['asset'] not in assets:assets.append(c['asset'])
    def safe(d):
        try:return descriptor(data,d,crom)
        except AssertionError:   # every pen transparent: the draw shows nothing
            return dict(descriptor=hex(d),x=0,y=0,width=0,height=0,pens=b'')
    descs={d:safe(d) for d in assets}
    def columns(address):
        # 16-pixel columns of the descriptor (origin-relative x, unmirrored)
        at=address-0x100000;groups,attr=data[at:at+2];at+=2;out=[]
        for g in range(groups):
            x,y,cols,rows=struct.unpack_from('>hhBB',data,at);at+=6+2*cols*rows
            out+=[x+16*c for c in range(cols)]
        return out
    for d in assets:descs[d]['columns']=columns(d)
    # palettes: allocator table of the dumps (slot -> resource) + palette RAM
    palettes={}
    for pal_file in sorted(a.capture.glob('palette*.bin')):
        frame=int(pal_file.stem[7:]);ram_file=a.capture/f'ram{frame}.bin'
        if not ram_file.exists() or frame<10000:continue
        ram=ram_file.read_bytes();pw=struct.unpack('>4096H',pal_file.read_bytes())
        for slot,rid in resources(ram).items():
            if rid in RESOURCES:
                v=rgba16(pw[slot*16:slot*16+16])
                palettes.setdefault(rid,collections.Counter())[tuple(v)]+=1
    # a resource's colours may flash; keep the most frequent set
    pal_out={hex(k):list(c.most_common(1)[0][0]) for k,c in sorted(palettes.items())}
    fixture=json.load(gzip.open(a.fixture))
    # Draws of every boss-family update of the capture's trace, in list
    # order (the native ones are the fixture's: tools/test_neo_boss.c proves
    # them equal to the original sprite list entries). Generic actors of the
    # family (shell trails, explosions, debris, prisoners) are drawn too,
    # because some of them share the boss palette slots; only the pixels of
    # native draws are scored.
    from extract_boss_fight import parse,lanes
    native_rt=lambda r:0xd5976<=r<0xd74d4 or r in (0x2a18a,0x2a244,0x2a2ac,0x2a2fe)
    main=fixture['nodes'][0]['addr'];passes=[]
    for u in parse(a.capture/'boss-trace.txt'):
        dl=[];cur=None
        for k,ad,m,d,pc in u['recs']:
            if k=='W' and pc==0xa078:
                if cur is None or len(cur['w'])>=6:cur=dict(w=[],z=0,native=native_rt(u['routine']));dl.append(cur)
                cur['w'].append(d)
            elif k=='W' and pc==0xa080 and cur is not None:cur['z']=(cur['z']<<16|d)&0xffffffff
        out=[dict(desc=c['w'][4]<<16|c['w'][5],x=c['w'][0],y=c['w'][1],attr=c['w'][3],z=c['z']&0xffff,native=c['native']) for c in dl if len(c['w'])==6]
        if u['node']==main or not passes:passes.append([])
        passes[-1].append((u['frame'],u['node'],out,False))
    gen_descs={}
    def desc_of(t):
        if t in descs:return descs[t]
        if t not in gen_descs:
            try:gen_descs[t]=descriptor(data,t,crom);gen_descs[t]['columns']=columns(t)
            except Exception:gen_descs[t]=None
        return gen_descs[t]
    first,last,step=map(int,a.frames.split(':'))
    stats=collections.Counter();per_frame=[]
    def raster(dl,v,pw):
        slots={d['attr']>>8 for d in dl}
        im,_=reconstruct(v,pw,crom,slots)
        mame=struct.unpack(f'<{320*256}H',im.info['neo_palette_indices'])
        native=[0]*(320*256);owner=[False]*(320*256)
        for d in sorted(dl,key=lambda d:d['z']):
            g=desc_of(d['desc'])
            if g is None:stats['missing_descriptor']+=1;continue
            if not g['width']:continue
            mirror=d['attr']&1;w,h=g['width'],g['height'];pens=g['pens']
            left=d['x']+(-(g['x']+w)-1 if mirror else g['x']);top=((512-d['y'])&511)+g['y']
            slot=d['attr']>>8
            # 00A17A..00A18E: the sprite builder drops columns whose left edge
            # is outside -8..311
            shown=set()
            for cx in g['columns']:
                sx0=d['x']-cx-16 if mirror else d['x']+cx
                if mirror:sx0-=1
                if -8<=sx0<=311:shown.update(range(sx0,sx0+16))
            for yy in range(h):
                sy=top+yy
                if not 0<=sy<256:continue
                for xx in range(w):
                    pen=pens[yy*w+(w-1-xx if mirror else xx)]
                    sx=left+xx
                    if pen and 0<=sx<320 and sx in shown:native[sy*320+sx]=slot*16+pen;owner[sy*320+sx]=d['native']
        same=diff=extra=0;bad=[]
        for i in range(320*256):
            n,m=native[i],mame[i]
            if n and owner[i]:
                if n==m:same+=1
                else:
                    diff+=1
                    if len(bad)<4:bad.append((i%320,i//320,n,m))
            elif m:extra+=1
        return same,diff,extra,bad
    for frame in range(first,last,step):
        vf=a.capture/f'vram{frame}.bin'
        if not vf.exists():continue
        v=struct.unpack('>34304H',vf.read_bytes());pw=struct.unpack('>4096H',(a.capture/f'palette{frame}.bin').read_bytes())
        # The dumped VRAM holds the sprite list of the last logic pass whose
        # list was committed before the dump: the pass just before the dump or
        # the one before it (a pass may end after the vblank). Both are
        # rasterized; the one that matches is reported with its rank.
        done=[q for q in passes if q and max(x[0] for x in q)<=frame]
        best=None
        for rank in (1,2):
            if len(done)<rank:continue
            q=done[-rank]
            dl=[d for f,n,dl0,gone in q for d in dl0]
            f=max(x[0] for x in q)
            if not dl:continue
            r=raster(dl,v,pw)
            if best is None or r[1]<best[1][1]:best=(rank,r,f,dl)
        if best is None:continue
        rank,(same,diff,extra,bad),f,dl=best
        stats['pixels_equal']+=same;stats['pixels_different']+=diff;stats['vram_pixels_not_drawn_by_native_boss']+=extra;stats['frames']+=1
        stats['pass_rank_%d'%rank]+=1
        per_frame.append(dict(frame=frame,update_frame=f,pass_rank=rank,draws=len(dl),equal=same,different=diff,uncovered=extra,first_differences=bad))
    frames=[dict(descriptor=d['descriptor'],x=d['x'],y=d['y'],width=d['width'],height=d['height'],columns=d['columns'],pens_hex=d['pens'].hex()) for d in descs.values() if d['width']]
    out=dict(source='Original P-ROM descriptors and C-ROM tiles of the boss programs; palettes from MAME 0.264 mslugx euro dumps',
        p2_sha256=hashlib.sha256(data).hexdigest(),frames=frames,palettes=pal_out,empty=[hex(d) for d,g in descs.items() if not g['width']],
        missing_palettes=[hex(r) for r in RESOURCES if hex(r) not in pal_out])
    (ROOT/'filesystem/boss-native.json').write_text(json.dumps(out,separators=(',',':'))+'\n')
    tiles=set()
    for d in assets:
        at=d-0x100000;groups,attr=data[at:at+2];at+=2
        for g in range(groups):
            x,y,cols,rows=struct.unpack_from('>hhBB',data,at);at+=6
            for k in range(cols*rows):tiles.add(struct.unpack_from('>H',data,at)[0]|(attr&0xf0)<<12);at+=2
    report=dict(descriptors=len(frames),unique_tiles=len(tiles),palettes=sorted(pal_out),stats=stats,frames=per_frame)
    a.report.write_text(json.dumps(report,indent=1)+'\n')
    print(json.dumps(dict(descriptors=len(frames),unique_tiles=len(tiles),palettes=len(pal_out),missing=out['missing_palettes'],stats=stats)))
if __name__=='__main__':main()
