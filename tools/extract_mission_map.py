#!/usr/bin/env python3
"""Recover observed mission-1 background cells and parallax from original VRAM.

This recovers the observed region, not an unseen ROM level or gameplay logic.
"""
import argparse,collections,csv,json,struct
from pathlib import Path
from neo_scene import state,trace_hints

def animation_disabled(writes,frame):
    """LSPC bit 3 as last written before the update that produced `frame`.
    A write logged with frame n happens while frame n+1 is emulated."""
    assert not any(frame-2<=f<frame for f,_ in writes),('LSPC mode written next to a snapshot',frame)
    value=[v for f,v in writes if f<frame]
    return bool(value and value[-1]&8)

def extract(roots,output,allow_backdrop_timeline=False,frames=None):
    if isinstance(roots,Path):roots=[roots]
    cells=[collections.defaultdict(collections.Counter) for _ in range(4)]
    observations=[];backdrops=set();flashes=[]
    paths=[path for root in roots for path in sorted(root.glob('vram*.bin'),key=lambda p:int(p.stem[4:]))]
    traces={};maps={};phases={};lspc_writes={};baseline={};origin_events=[];fore_bases={}
    # Cells of a layer whose map pointer differs from the first observed one
    # (the rubble map 0B53A8 that 0B58CC loads into 10C72E): kept apart, in
    # display coordinates, keyed by (layer, pointer).
    swapped=collections.defaultdict(lambda:collections.defaultdict(collections.Counter))
    for path in paths:
        root=path.parent
        if root not in maps:
            # Per-frame layer headers (layers.csv): map pointer +2E and map
            # column origin +32 (ROM 0C6F34/0C6F82) of each layer object.
            # Word +2C of 10C72E is its first SCB slot minus one.
            t=root/'layers.csv';maps[root]=None;fore_bases[root]={}
            if t.exists():
                maps[root]=collections.defaultdict(dict)
                for row in csv.DictReader(t.read_text().splitlines()):
                    h=bytes.fromhex(row['header'])
                    maps[root][int(row['frame'])][int(row['object'],16)]=(struct.unpack_from('>I',h,0x2e)[0],struct.unpack_from('>h',h,0x32)[0])
                    if int(row['object'],16)==0x10c72e:fore_bases[root][int(row['frame'])]=struct.unpack_from('>H',h,0x2c)[0]+1
        if root not in phases:
            # Hardware auto-animation counter at each snapshot (lspc.csv of
            # tools/capture_mission_rest.lua, LSPC mode register bits 2..0) and
            # every write to that register (lspc-writes.csv; bit 3 disables the
            # auto-animation, so the SCB tile codes are shown unmodified).
            t=root/'lspc.csv';w=root/'lspc-writes.csv'
            phases[root]={int(r['frame']):int(r['auto_animation']) for r in csv.DictReader(t.read_text().splitlines())} if t.exists() else None
            lspc_writes[root]=[(int(r['frame']),int(r['value'],16)) for r in csv.DictReader(w.read_text().splitlines())] if w.exists() else None
        if root not in traces:
            # tools/capture_mission_rest.lua writes a per-frame RAM trace; with it
            # every transform must equal a traced RAM value (see neo_scene.state).
            t=root/'trace.csv'
            traces[root]={int(r['frame']):r for r in csv.DictReader(t.read_text().splitlines())} if t.exists() else None
        frame=int(path.stem[4:]);ram_path=root/f'ram{frame}.bin'
        if not ram_path.exists():continue
        if frames and not frames[0]<=frame<=frames[1]:continue
        v=struct.unpack('>34304H',path.read_bytes());ram=ram_path.read_bytes()
        p=struct.unpack('>4096H',(root/f'palette{frame}.bin').read_bytes())
        trace=traces[root]
        fore_base=None
        if fore_bases[root]:
            # The SCBs may show any of the last four updates: the slot base must
            # be constant over them.
            bases={fore_bases[root][frame-k] for k in range(4) if frame-k in fore_bases[root]}
            assert len(bases)==1,('Foreground slot base changes inside the snapshot lag window',frame,bases)
            fore_base=next(iter(bases))
        scene=state(v,ram,trace_hints(trace,frame) if trace else None,fore_base);camera=scene['camera'];offsets=list(scene['offsets'])
        origins=None;layer_maps=[None]*4
        if maps[root]:
            # A layer whose map pointer is unchanged but whose column origin moved
            # (ROM 0C6F82, e.g. the map swap 0B58CC adds 2 to the sky) shows the
            # same map shifted: cells are stored in map coordinates, so the
            # observation's offset carries -16 px per origin column. The SCBs of a
            # snapshot may show any of the last four updates: the state must be
            # constant over them. A new map pointer is kept in display coordinates.
            origins=[]
            for layer,obj in enumerate((0x10e83e,0x10d7b6,0x10d7b6,0x10c72e)):
                states={maps[root][frame-k][obj] for k in range(4) if obj in maps[root].get(frame-k,{})}
                assert len(states)==1,('Layer map state changes inside the snapshot lag window',frame,hex(obj),states)
                pointer,origin=next(iter(states))
                base=baseline.setdefault((root,obj),(pointer,origin))
                if pointer!=base[0]:layer_maps[layer]=pointer
                if pointer==base[0] and origin!=base[1]:
                    offsets[layer]-=16*(origin-base[1])
                    if (str(root),layer,origin) not in {(e['captures'],e['layer'],e['origin']) for e in origin_events}:
                        origin_events.append(dict(captures=str(root),layer=layer,object=f'0x{obj:06X}',map=f'0x{pointer:06X}',first_frame=frame,origin=origin,baseline_origin=base[1],shift=-16*(origin-base[1])))
                origins.append([f'0x{pointer:06X}',origin])
        used={v[c['slot']*64+r*2+1]>>8 for c in scene['columns'] for r in range(min(c['rows'],32)) if (v[c['slot']*64+r*2+1]&0xf0)==0x40}
        if p[-1]==0x7fff and used and all(w==0x7fff for pal in used for w in p[pal*16+1:pal*16+16]):
            # Full-screen white flash (an explosion palette effect): every pen of
            # every background palette and the backdrop are white. Such a frame
            # carries no map information; it is recorded, never merged.
            flashes.append(dict(frame=frame,captures=str(root),camera=camera,palettes=len(used)));continue
        backdrops.add(p[-1])
        observations.append(dict(frame=frame,captures=str(root),camera=camera,backdrop=p[-1],
            camera_ram=struct.unpack_from('>H',ram,0x2036)[0],offsets=offsets,
            vertical_offsets=scene['vertical_offsets'],active=scene['active'],map_origins=origins,
            **({'animation_phase':phases[root][frame]} if phases[root] else {}),
            **({'auto_animation_disabled':animation_disabled(lspc_writes[root],frame)} if lspc_writes[root] is not None else {}),
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
                table=cells[layer] if layer_maps[layer] is None else swapped[(layer,layer_maps[layer])]
                table[(world_x,sy-16-scene['vertical_offsets'][layer])][(code,attr&3,mask,pal)]+=1
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
    swapped_records=[]
    for (layer,pointer),table in sorted(swapped.items()):
        for (x,y),choices in sorted(table.items()):
            assert len(choices)==1,('Swapped map cell variants',layer,hex(pointer),x,y,choices)
            (code,flip,mask,palette),count=next(iter(choices.items()))
            if palette not in palette_ids:palette_ids[palette]=len(palettes);palettes.append(palette)
            swapped_records.append(dict(layer=layer,map=f'0x{pointer:06X}',x=x,y=y,code=code,flip=flip,animation_mask=mask,palette=palette_ids[palette],observations=count))
    assert len(backdrops)==1 or allow_backdrop_timeline,'Multiple backdrops require --allow-backdrop-timeline and native event recovery before packing'
    report=dict(source='MAME mslugx euro; calibrated SCB background bank 0x40000; enemy slots excluded',backdrop=observations[0]['backdrop'],backdrop_values=sorted(backdrops),
        backdrop_policy='Observed per-frame palette values; multiple values require a recovered runtime event, not a camera threshold inferred from samples',
        camera_max=max(o['camera'] for o in observations),width=max(o['camera'] for o in observations)+320,
        selection='first observed; dynamic coordinate variants retained separately',
        layers=[dict(numerator=n,denominator=d) for n,d in ((1,2),(3,4),(1,1),(1,1))],
        palettes=palettes,cells=records,swapped_cells=swapped_records,observations=observations,conflicts=conflicts,flash_frames=flashes,map_origin_events=origin_events)
    (output/'map.json').write_text(json.dumps(report,separators=(',',':'))+'\n')
    print('Observed map:',len(records),'cells;',len(swapped_records),'swapped-map cells;',len(palettes),'palettes;',len(conflicts),'coordinate variants; camera',report['camera_max'])

if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('captures',type=Path,nargs='+');p.add_argument('--output',type=Path,default=Path('filesystem/mission1'))
    p.add_argument('--allow-backdrop-timeline',action='store_true',help='Keep per-observation backdrop values for research; runtime event mapping remains required')
    p.add_argument('--frames',type=lambda v:tuple(int(x) for x in v.split(':')),help='LO:HI inclusive snapshot frames (e.g. exclude the mission-end fade)')
    a=p.parse_args();extract(a.captures,a.output,a.allow_backdrop_timeline,a.frames)
