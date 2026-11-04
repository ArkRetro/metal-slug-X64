#!/usr/bin/env python3
"""Recover observed mission-1 background cells and parallax from original VRAM.

This recovers the observed region, not an unseen ROM level or gameplay logic.
"""
import argparse,collections,hashlib,json,struct
from pathlib import Path
from neo_scene import state

def extract(roots,output,allow_backdrop_timeline=False,first_frame=None,last_frame=None):
    if isinstance(roots,Path):roots=[roots]
    if (first_frame is not None and first_frame<0) or (last_frame is not None and last_frame<0):
        raise ValueError('Frame bounds must be nonnegative')
    if first_frame is not None and last_frame is not None and first_frame>last_frame:
        raise ValueError('First frame must not exceed last frame')
    cells=[collections.defaultdict(collections.Counter) for _ in range(4)]
    observations=[];backdrops=set()
    metadata={};metadata_hashes={}
    for root in roots:
        metadata_path=root/'frames.jsonl'
        metadata[root]={}
        if metadata_path.is_file():
            raw=metadata_path.read_bytes();metadata_hashes[str(root)]=hashlib.sha256(raw).hexdigest()
            for line in raw.splitlines():
                record=json.loads(line)
                if 'animation_counter' in record and 'animation_disabled' in record:
                    frame=record['frame']
                    if frame in metadata[root]:raise ValueError(f'Duplicate frame metadata: {root}:{frame}')
                    metadata[root][frame]=record
    paths=[path for root in roots for path in sorted(root.glob('vram*.bin'),key=lambda p:int(p.stem[4:]))]
    available=len(paths)
    paths=[p for p in paths if (first_frame is None or int(p.stem[4:])>=first_frame)
        and (last_frame is None or int(p.stem[4:])<=last_frame)]
    for path in paths:
        root=path.parent
        frame=int(path.stem[4:]);ram_path=root/f'ram{frame}.bin'
        if not ram_path.exists():continue
        v=struct.unpack('>34304H',path.read_bytes());ram=ram_path.read_bytes()
        p=struct.unpack('>4096H',(root/f'palette{frame}.bin').read_bytes())
        backdrops.add(p[-1])
        scene=state(v,ram);camera=scene['camera'];offsets=scene['offsets']
        measured=metadata[root].get(frame)
        observations.append(dict(frame=frame,captures=str(root),camera=camera,backdrop=p[-1],
            animation_counter=measured['animation_counter'] if measured else None,
            animation_disabled=measured['animation_disabled'] if measured else None,
            animation_phase_source='MAME spritegen device state' if measured else 'UNRECORDED: frame number is not animation phase',
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
                code=low|((attr<<12)&0xf0000)
                mask=0 if measured and measured['animation_disabled'] else 7 if attr&8 else 3 if attr&4 else 0
                code &= ~mask
                pal=tuple(p[(attr>>8)*16:(attr>>8)*16+16])
                cells[layer][(world_x,sy-16-scene['vertical_offsets'][layer])][(code,attr&3,mask,pal)]+=1
    if not observations:raise ValueError('No RAM/VRAM observations in the requested frame range')
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
    report=dict(source='MAME mslugx euro; calibrated SCB background bank 0x40000; enemy slots excluded',
        capture_selection=dict(first_frame=first_frame,last_frame=last_frame,
            available_vram_snapshots=available,selected_vram_snapshots=len(paths),
            observed_ram_snapshots=len(observations),out_of_range_snapshots=available-len(paths),
            policy='Explicit inclusive capture-frame bounds, applied independently to each source run; no inferred mission/camera boundary'),
        animation_metadata_sha256=metadata_hashes,
        backdrop=observations[0]['backdrop'],backdrop_values=sorted(backdrops),
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
    p.add_argument('--first-frame',type=int,help='Inclusive capture frame bound; explicitly select this mission rather than inferring it from camera position')
    p.add_argument('--last-frame',type=int,help='Inclusive capture frame bound; exclude following scenes/fades using verified capture boundaries')
    a=p.parse_args();extract(a.captures,a.output,a.allow_backdrop_timeline,a.first_frame,a.last_frame)
