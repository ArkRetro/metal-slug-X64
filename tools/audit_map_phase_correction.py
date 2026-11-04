#!/usr/bin/env python3
"""Audit only a disabled-autoanimation correction, without declaring a full map valid.

SCB/zoom/FIX data determines pixel provenance. Screenshots only compare the
selected visible source pixels; they never select phase, coverage or assets.
"""
import argparse
import hashlib
import json
from pathlib import Path
import struct
import zipfile

import numpy as np
from PIL import Image

from export_marco import palette_rgb
from neo_scene import state
from render_intro_reference import Renderer


def corrected_cells(before, after):
    def index(model):
        return {(c['layer'],c['x'],c['y']):(c['code'],c['flip'],c['animation_mask'],tuple(model['palettes'][c['palette']])) for c in model['cells']}
    old,new=index(before),index(after)
    if old.keys()!=new.keys():raise ValueError('Correction must not add/remove cells')
    changed={key for key in old if old[key]!=new[key]}
    for key in changed:
        a,b=old[key],new[key]
        if not(a[2]==3 and b[2]==0 and a[:2]==b[:2] and a[3]==b[3]):
            raise ValueError(f'Unsupported non-mask correction: {key}')
    return changed


def pixel_origins(vram, renderer):
    """Last opaque sprite tile per pixel, using the original zoom ROM and limit."""
    columns=[];x=y=rows=zx=zy=0
    for slot in range(381):
        control,z=vram[0x8200+slot],vram[0x8000+slot]
        if control&64:x=(x+zx+1)&511;zx=z>>8&15
        else:x=vram[0x8400+slot]>>7;y=512-(control>>7);rows=control&63;zx=z>>8&15;zy=z&255
        columns.append((slot,x,y,rows,zx,zy))
    origin=np.zeros((224,320),dtype=np.int32)
    for sy in range(16,240):
        active=[c for c in columns if c[3] and (c[3]>=32 or ((sy-c[2])&511)<c[3]*16)][:96]
        for slot,x,y,rows,zx,zy in active:
            if 320<=x<=496:continue
            line=(sy-y)&511;zline=line&255;invert=bool(line&256)
            if invert:zline^=255
            if rows>32:
                zline%=((zy+1)*2)
                if zline>zy:zline=(zy+1)*2-1-zline;invert=not invert
            lookup=renderer.zoom[zy*256+zline];ty=lookup&15;row=lookup>>4
            if invert:ty^=15;row^=31
            low,attr=vram[slot*64+row*2:slot*64+row*2+2]
            # This audit is explicitly limited to measured disabled animation.
            code=low|((attr<<12)&0xf0000)
            if attr&2:ty^=15
            xs=renderer.xsamples[zx];xs=[15-i for i in xs] if attr&1 else xs
            pens=renderer.tile(code)[ty,xs]
            indices=(np.arange(len(xs))+x)&511;valid=(indices<320)&(pens!=0)
            origin[sy-16,indices[valid]]=slot*32+row+1
    for sy in range(16,240):
        for x in range(40):
            word=vram[0x7000+x*32+sy//8]
            pens=renderer.character(word&4095)[sy&7]
            target=origin[sy-16,x*8:x*8+8];target[pens!=0]=-1
    return origin


def audit(old_path,new_path,crom_path,fixed_path,bios_path):
    old=json.loads(old_path.read_text());new=json.loads(new_path.read_text())
    changed=corrected_cells(old,new)
    target={(c['layer'],c['x'],c['y']):c for c in new['cells']}
    if not changed:raise ValueError('No mask corrections to audit')
    if not all(o.get('animation_disabled') is True for o in new['observations']):
        raise ValueError('All candidate observations must measure disabled autoanimation')
    with zipfile.ZipFile(bios_path) as bios:zoom=bios.read('000-lo.lo')
    renderer=Renderer(crom_path.read_bytes(),fixed_path.read_bytes(),zoom)
    checked=0;visible=set();opaque_source=set();failures=[]
    for observation in new['observations']:
        root=Path(observation['captures']);frame=observation['frame']
        v=struct.unpack('>34304H',(root/f'vram{frame}.bin').read_bytes())
        p=struct.unpack('>4096H',(root/f'palette{frame}.bin').read_bytes())
        scene=state(v,(root/f'ram{frame}.bin').read_bytes())
        origin=pixel_origins(v,renderer)
        screenshot=np.asarray(Image.open(root/f'game{frame}.png').convert('RGB'))
        for column in scene['columns']:
            slot=column['slot'];layer=column['layer']
            x=column['x']-512 if column['x']>=496 else column['x']
            if x>=320:continue
            world_x=x+scene['offsets'][layer]
            for row in range(column['rows']):
                y=(column['y']+row*16)&511
                if y>=496:y-=512
                y-=16
                key=(layer,world_x,y-scene['vertical_offsets'][layer])
                if key not in changed:continue
                low,attr=v[slot*64+row*2:slot*64+row*2+2]
                if (attr&0xf0)!=0x40:continue
                code=low|((attr<<12)&0xf0000)
                cell=target[key]
                if code!=cell['code'] or (attr&3)!=cell['flip'] or tuple(p[(attr>>8)*16:(attr>>8)*16+16])!=tuple(new['palettes'][cell['palette']]):
                    continue # A captured dynamic variant cannot validate another variant.
                tile=renderer.tile(code)
                if attr&1:tile=tile[:,::-1]
                if attr&2:tile=tile[::-1,:]
                colors=np.asarray([palette_rgb(w) for w in p[(attr>>8)*16:(attr>>8)*16+16]])
                for ty in range(16):
                    sy=y+ty
                    if not 0<=sy<224:continue
                    for tx in range(16):
                        sx=x+tx
                        if not 8<=sx<312 or not tile[ty,tx]:continue
                        opaque_source.add(key)
                        if origin[sy,sx]!=slot*32+row+1:continue
                        expected=colors[tile[ty,tx]]
                        if int(np.max(np.abs(expected.astype(int)-screenshot[sy,sx].astype(int))))>1:
                            if len(failures)<20:failures.append(dict(frame=frame,x=sx,y=sy,key=key,expected=expected.tolist(),actual=screenshot[sy,sx].tolist()))
                        checked+=1;visible.add(key)
    sha=lambda path:hashlib.sha256(path.read_bytes()).hexdigest()
    return dict(status='TARGETED_CORRECTION_PASS' if checked and not failures else 'FAIL',
        corrected_cells=len(changed),visually_observed_corrected_cells=len(visible),
        source_opaque_corrected_cells=len(opaque_source),visible_source_pixels=checked,
        covered_or_unseen_cells=len(changed-visible),failures=failures,
        sampled_frames=len(new['observations']),first_frame=min(o['frame'] for o in new['observations']),last_frame=max(o['frame'] for o in new['observations']),
        old_map_sha256=sha(old_path),candidate_map_sha256=sha(new_path),
        crom_sha256=sha(crom_path),fixed_sha256=sha(fixed_path),zoom_sha256=hashlib.sha256(zoom).hexdigest(),
        animation_metadata_sha256=new['animation_metadata_sha256'],
        limits='Targeted source-art check only. Uses measured disabled animation and original source alpha/zoom/FIX for visibility. Does not waive any independent full-map audit failure or validate all map/actors/events. Covered/unseen cells have direct SCB/metadata provenance, not independent visible-pixel coverage.')


def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--old',type=Path,required=True);parser.add_argument('--candidate',type=Path,required=True)
    parser.add_argument('--crom',type=Path,default=Path('filesystem/sprites.bin'))
    parser.add_argument('--fixed',type=Path,default=Path('filesystem/fixed.bin'))
    parser.add_argument('--bios',type=Path,required=True);parser.add_argument('--output',type=Path,required=True)
    args=parser.parse_args();report=audit(args.old,args.candidate,args.crom,args.fixed,args.bios)
    args.output.parent.mkdir(parents=True,exist_ok=True);args.output.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
    if report['status']=='FAIL':raise SystemExit(1)


if __name__=='__main__':main()
