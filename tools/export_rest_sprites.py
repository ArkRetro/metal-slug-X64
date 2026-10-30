#!/usr/bin/env python3
"""Export the sprite descriptors drawn by the native rest-of-mission actors
(programs of tools/extract_soldier_programs.py ROOTS for the robed swordsman
0A8C46: 2A7372, 2A73B4, 2A7434, 2A74E0, 2A759C) from the original P-ROM and
C-ROM, with their logical palettes (0x1B, 0x20B, 0x3BA, allocated by
0A8CA0), and verify them pixel by pixel against MAME VRAM dumps of a family
trace (MSX_REST_FAMILY_ROOTS=a8c46): the sprite-list entries the original
wrote during the actor's last update before each dump (PC 00A078/00A080)
are rasterized with the exported pens and must give the same palette slot
and pen as the VRAM sprite. Output: filesystem/rest-native.json (layout of
filesystem/boss-native.json) and docs/rest-sprites-validation.json."""
import argparse,collections,hashlib,json,struct
from pathlib import Path
from export_marco import ROOT,reconstruct
from export_soldier_sprites import program,descriptor,resources
from mission_tiles import rgba16
from extract_boss_fight import parse
ROOTS=(0x2a7372,0x2a73b4,0x2a7434,0x2a74e0,0x2a759c)
RESOURCES=(0x1b,0x20b,0x3ba)
def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('zip',type=Path);p.add_argument('capture',type=Path)
    p.add_argument('--node',type=lambda v:int(v,16),default=0x104644)
    p.add_argument('--report',type=Path,default=ROOT/'docs/rest-sprites-validation.json')
    a=p.parse_args()
    data=program(a.zip);crom=(ROOT/'filesystem/sprites.bin').read_bytes()
    progs=[q for q in json.loads((ROOT/'data/soldier_programs.json').read_text())['programs'] if q['root'] in ROOTS]
    assets=[]
    for prog in progs:
        for c in prog['commands']:
            if c['kind']=='POSE' and c['asset'] not in assets:assets.append(c['asset'])
    def columns(address):
        at=address-0x100000;groups,attr=data[at:at+2];at+=2;out=[]
        for g in range(groups):
            x,y,cols,rows=struct.unpack_from('>hhBB',data,at);at+=6+2*cols*rows
            out+=[x+16*c for c in range(cols)]
        return out
    descs={}
    for d in assets:
        try:descs[d]=descriptor(data,d,crom)
        except AssertionError:descs[d]=dict(descriptor=hex(d),x=0,y=0,width=0,height=0,pens=b'')
        descs[d]['columns']=columns(d)
    palettes={}
    for pal_file in sorted(a.capture.glob('palette*.bin')):
        frame=int(pal_file.stem[7:]);ram_file=a.capture/f'ram{frame}.bin'
        if not ram_file.exists():continue
        ram=ram_file.read_bytes();pw=struct.unpack('>4096H',pal_file.read_bytes())
        for slot,rid in resources(ram).items():
            if rid in RESOURCES:palettes.setdefault(rid,collections.Counter())[tuple(rgba16(pw[slot*16:slot*16+16]))]+=1
    pal_out={hex(k):list(c.most_common(1)[0][0]) for k,c in sorted(palettes.items())}
    # Sprite-list entries written during each update of the node.
    updates=[]
    for u in parse(a.capture/'boss-trace.txt'):
        if u['node']!=a.node:continue
        dl=[];cur=None
        for k,ad,m,d,pc in u['recs']:
            if k=='W' and pc==0xa078 and ad<0x10f000:
                if cur is None or len(cur['w'])>=6:cur=dict(w=[]);dl.append(cur)
                cur['w'].append(d)
        updates.append((u['frame'],[dict(desc=c['w'][4]<<16|c['w'][5],x=c['w'][0],y=c['w'][1],attr=c['w'][3]) for c in dl if len(c['w'])==6]))
    stats=collections.Counter();per=[]
    for vf in sorted(a.capture.glob('vram*.bin'),key=lambda f:int(f.stem[4:])):
        frame=int(vf.stem[4:])
        done=[x for x in updates if x[0]<=frame and x[1]]
        if not done:continue
        v=struct.unpack('>34304H',vf.read_bytes());pw=struct.unpack('>4096H',(a.capture/f'palette{frame}.bin').read_bytes())
        best=None
        for rank in (1,2):
            if len(done)<rank:continue
            f,dl=done[-rank]
            slots={d['attr']>>8 for d in dl}
            im,_=reconstruct(v,pw,crom,slots)
            mame=struct.unpack(f'<{320*256}H',im.info['neo_palette_indices'])
            native=[0]*(320*256)
            for d in dl:
                g=descs.get(d['desc'])
                if g is None:stats['missing_descriptor']+=1;continue
                if not g['width']:continue
                mirror=d['attr']&1;w,h=g['width'],g['height'];pens=g['pens']
                left=d['x']+(-(g['x']+w)-1 if mirror else g['x']);top=((512-d['y'])&511)+g['y']
                shown=set()
                for cx in g['columns']:
                    sx0=d['x']-cx-16-1 if mirror else d['x']+cx
                    if -8<=sx0<=311:shown.update(range(sx0,sx0+16))
                for yy in range(h):
                    sy=top+yy
                    if not 0<=sy<256:continue
                    for xx in range(w):
                        pen=pens[yy*w+(w-1-xx if mirror else xx)];sx=left+xx
                        if pen and 0<=sx<320 and sx in shown:native[sy*320+sx]=(d['attr']>>8)*16+pen
            same=sum(1 for i in range(320*256) if native[i] and native[i]==mame[i])
            diff=sum(1 for i in range(320*256) if native[i] and native[i]!=mame[i])
            if best is None or diff<best[1]:best=(same,diff,rank,f)
        stats['frames']+=1;stats['pixels_equal']+=best[0];stats['pixels_different']+=best[1];stats['rank_%d'%best[2]]+=1
        per.append(dict(frame=frame,update_frame=best[3],equal=best[0],different=best[1]))
    frames=[dict(descriptor=d['descriptor'],x=d['x'],y=d['y'],width=d['width'],height=d['height'],columns=d['columns'],pens_hex=d['pens'].hex()) for d in descs.values() if d['width']]
    out=dict(source='Original P-ROM descriptors and C-ROM tiles of the rest-of-mission programs; palettes from MAME 0.264 mslugx euro dumps',
        p2_sha256=hashlib.sha256(data).hexdigest(),frames=frames,palettes=pal_out,empty=[hex(d) for d,g in descs.items() if not g['width']],
        missing_palettes=[hex(r) for r in RESOURCES if hex(r) not in pal_out])
    (ROOT/'filesystem/rest-native.json').write_text(json.dumps(out,separators=(',',':'))+'\n')
    report=dict(descriptors=len(frames),palettes=sorted(pal_out),missing_palettes=out['missing_palettes'],stats=stats,frames=per)
    a.report.write_text(json.dumps(report,indent=1)+'\n')
    print(json.dumps(dict(descriptors=len(frames),palettes=len(pal_out),missing=out['missing_palettes'],stats=stats)))
if __name__=='__main__':main()
