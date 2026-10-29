#!/usr/bin/env python3
"""Recover observed mission-1 background cells and parallax from original VRAM.

This recovers the observed region, not an unseen ROM level or gameplay logic.
"""
import argparse,collections,json,struct
from pathlib import Path
from neo_scene import state

def extract(roots,output,allow_backdrop_timeline=False):
    if isinstance(roots,Path):roots=[roots]
    cells=[collections.defaultdict(collections.Counter) for _ in range(4)]
    observations=[];backdrops=set()
    paths=[path for root in roots for path in sorted(root.glob('vram*.bin'),key=lambda p:int(p.stem[4:]))]
    for path in paths:
        root=path.parent
        frame=int(path.stem[4:]);ram_path=root/f'ram{frame}.bin'
        if not ram_path.exists():continue
        v=struct.unpack('>34304H',path.read_bytes());ram=ram_path.read_bytes()
        p=struct.unpack('>4096H',(root/f'palette{frame}.bin').read_bytes())
        backdrops.add(p[-1])
        scene=state(v,ram);camera=scene['camera'];offsets=scene['offsets']
        observations.append(dict(frame=frame,captures=str(root),camera=camera,backdrop=p[-1],
            camera_ram=struct.unpack_from('>H',ram,0x2036)[0],offsets=offsets,
            vertical_offsets=scene['vertical_offsets'],active=scene['active'],
            full_speed_mid=scene['full_speed_mid'],
            player_x16=struct.unpack_from('>I',ram,0x14ec)[0],player_y16=struct.unpack_from('>I',ram,0x14f0)[0]))
        for col in scene['columns']:
            slot,x,y,rows,layer=(col[k] for k in ('slot','x','y','rows','layer'))
            sx=x-512 if x>=496 else x
            if sx>=320:continue
            world_x=sx+offsets[layer]
            assert world_x%16==0,(frame,slot,world_x,layer)
            for row in range(min(rows,32)):
                sy=(y+row*16)&511
                if sy>=496:sy-=512
                if sy+16<=16 or sy>=240:continue
                low,attr=v[slot*64+row*2:slot*64+row*2+2]
                if (attr&0xf0)!=0x40:continue
                code=low|((attr<<12)&0xf0000);mask=7 if attr&8 else 3 if attr&4 else 0
                code &= ~mask
                pal=tuple(p[(attr>>8)*16:(attr>>8)*16+16])
                cells[layer][(world_x,sy-16-scene['vertical_offsets'][layer])][(code,attr&3,mask,pal)]+=1
    output.mkdir(parents=True,exist_ok=True)
    palette_ids={};palettes=[];records=[];conflicts=[]
    for layer,table in enumerate(cells):
        for (x,y),choices in sorted(table.items()):
            # Keep the first observed state. Later sand/decor changes are retained
            # below, rather than silently replacing the untouched terrain by a vote.
            selected=next(iter(choices));count=choices[selected]
            if len(choices)>1:conflicts.append(dict(layer=layer,x=x,y=y,variants=len(choices),choices=[dict(code=k[0],flip=k[1],animation=k[2],palette=list(k[3]),count=n) for k,n in choices.most_common()] ))
            code,flip,mask,palette=selected
            if palette not in palette_ids:palette_ids[palette]=len(palettes);palettes.append(palette)
            records.append(dict(layer=layer,x=x,y=y,code=code,flip=flip,animation_mask=mask,palette=palette_ids[palette],observations=count))
    assert len(backdrops)==1 or allow_backdrop_timeline,'Multiple backdrops require --allow-backdrop-timeline and native event recovery before packing'
    report=dict(source='MAME mslugx euro; calibrated SCB background bank 0x40000; enemy slots excluded',backdrop=observations[0]['backdrop'],backdrop_values=sorted(backdrops),
        backdrop_policy='Observed per-frame palette values; multiple values require a recovered runtime event, not a camera threshold inferred from samples',
        camera_max=max(o['camera'] for o in observations),width=max(o['camera'] for o in observations)+320,
        selection='first observed; dynamic coordinate variants retained separately',
        layers=[dict(numerator=n,denominator=d) for n,d in ((1,2),(3,4),(1,1),(1,1))],
        palettes=palettes,cells=records,observations=observations,conflicts=conflicts)
    (output/'map.json').write_text(json.dumps(report,separators=(',',':'))+'\n')
    print('Observed map:',len(records),'cells;',len(palettes),'palettes;',len(conflicts),'coordinate variants; camera',report['camera_max'])

if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('captures',type=Path,nargs='+');p.add_argument('--output',type=Path,default=Path('filesystem/mission1'))
    p.add_argument('--allow-backdrop-timeline',action='store_true',help='Keep per-observation backdrop values for research; runtime event mapping remains required')
    a=p.parse_args();extract(a.captures,a.output,a.allow_backdrop_timeline)
