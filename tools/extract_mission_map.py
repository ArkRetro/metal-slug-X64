#!/usr/bin/env python3
"""Recover observed mission-1 background cells and parallax from original VRAM.

This recovers the observed region, not an unseen ROM level or gameplay logic.
"""
import argparse,collections,csv,json,struct
from pathlib import Path
from neo_scene import state,trace_hints,ram_slots,fourth_layer

def animation_disabled(writes,frame):
    """LSPC bit 3 as last written before the update that produced `frame`.
    A write logged with frame n happens while frame n+1 is emulated."""
    assert not any(frame-2<=f<frame for f,_ in writes),('LSPC mode written next to a snapshot',frame)
    value=[v for f,v in writes if f<frame]
    return bool(value and value[-1]&8)

def extract(roots,output,allow_backdrop_timeline=False,frames=None,ram_layers=False,banks=(4,),skip_fade=False):
    faded=[]
    if isinstance(roots,Path):roots=[roots]
    cells=[collections.defaultdict(collections.Counter) for _ in range(4)]
    observations=[];backdrops=set();flashes=[]
    paths=[path for root in roots for path in sorted(root.glob('vram*.bin'),key=lambda p:int(p.stem[4:]))]
    traces={};maps={};phases={};lspc_writes={};baseline={};origin_events=[];fore_bases={};slot_changes=[];slot_headers={};lag_skips=[]
    # Cells of a layer whose map pointer differs from the first observed one
    # (the rubble map 0B53A8 that 0B58CC loads into 10C72E): kept apart, in
    # display coordinates, keyed by (layer, pointer).
    swapped=collections.defaultdict(lambda:collections.defaultdict(collections.Counter))
    def maps_slots(root,frame):
        # {logical layer: first slot} from the layer headers of `frame`.
        if root not in slot_headers:
            slot_headers[root]=collections.defaultdict(dict)
            for row in csv.DictReader((root/'layers.csv').read_text().splitlines()):
                if len(row['header'] or '')!=0x110:continue
                h=bytes.fromhex(row['header']);slot_headers[root][int(row['frame'])][int(row['object'],16)]=h
        hs=slot_headers[root].get(frame)
        if not hs:return {}
        full=hs[0x10c72e][0x14:0x1c]==hs[0x10d7b6][0x14:0x1c]
        out={};fourth=0x10e8c6 in hs and struct.unpack_from('>H',hs[0x10e8c6],0x2c)[0]!=0xffff
        for layer,obj in ((0,0x10e83e),(1 if fourth else 2 if full else 1,0x10d7b6),(2,0x10e8c6),(3,0x10c72e)):
            if obj not in hs:continue
            b=struct.unpack_from('>H',hs[obj],0x2c)[0]
            if b!=0xffff:out[layer]=b+1
        return out
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
                    if len(row['header'] or '')!=0x110:continue  # Unterminated last line of a capture in progress.
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
        # --skip-fade: a palette fade (neo_fade.c, RAM 10991A..10991F: the
        # three channel offsets) is running, the pens are not the map's.
        if skip_fade and any(ram[0x991a:0x9920]):faded.append(frame);continue
        # A snapshot one or two frames after an LSPC mode write is ambiguous
        # (animation_disabled): left out with the fade frames (Mission 4: 158550).
        if skip_fade and lspc_writes.get(root) is not None and any(frame-2<=f<frame for f,_ in lspc_writes[root]):faded.append(frame);continue
        p=struct.unpack('>4096H',(root/f'palette{frame}.bin').read_bytes())
        trace=traces[root]
        fore_base=None
        if fore_bases[root]:
            # The SCBs may show any of the last four updates: the slot base must
            # be constant over them.
            bases={fore_bases[root][frame-k] for k in range(4) if frame-k in fore_bases[root]}
            assert len(bases)==1,('Foreground slot base changes inside the snapshot lag window',frame,bases)
            fore_base=next(iter(bases))
        slots=None
        if ram_layers:
            # Slot ranges of the layer objects (+0x2C) must be constant over
            # the SCB lag window (layers.csv), else the frame is not used.
            slots=ram_slots(ram)
            window={tuple(sorted(maps_slots(root,frame-k).items())) for k in range(4)} if (root/'layers.csv').exists() else {tuple(sorted(slots.items()))}
            if len(window)!=1 or next(iter(window))!=tuple(sorted(slots.items())):
                slot_changes.append(dict(frame=frame,captures=str(root)));continue
        scene=state(v,ram,trace_hints(trace,frame,fourth=slots is not None and fourth_layer(ram)) if trace else None,fore_base=fore_base if slots is None else None,slots=slots,banks=banks);camera=scene['camera'];offsets=list(scene['offsets'])
        origins=None;layer_maps=[None]*4
        if maps[root]:
            # A layer whose map pointer is unchanged but whose column origin moved
            # (ROM 0C6F82, e.g. the map swap 0B58CC adds 2 to the sky) shows the
            # same map shifted: cells are stored in map coordinates, so the
            # observation's offset carries -16 px per origin column. The SCBs of a
            # snapshot may show any of the last four updates: the state must be
            # constant over them. A new map pointer is kept in display coordinates.
            origins=[];objs=(0x10e83e,0x10d7b6,0x10e8c6 if slots is not None and fourth_layer(ram) else 0x10d7b6,0x10c72e)
            if ram_layers and any(len({maps[root][frame-k][obj] for k in range(4) if obj in maps[root].get(frame-k,{})})!=1 for obj in objs):
                # --ram-layers (Mission 3: the column origins move while the
                # layers scroll): such a frame is skipped and counted.
                lag_skips.append(frame);continue
            for layer,obj in enumerate(objs):
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
        used={v[c['slot']*64+r*2+1]>>8 for c in scene['columns'] for r in range(min(c['rows'],32)) if (v[c['slot']*64+r*2+1]&0xf0)>>4 in banks}
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
            full_speed_mid=scene['full_speed_mid'],**({'background_slots':{str(k):v for k,v in slots.items()},'banks':list(banks)} if ram_layers else {}),
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
                if (attr&0xf0)>>4 not in banks:continue
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
            # Mission 3 swaps maps whose cells change (variants are kept like
            # the main map's: first observed, the rest in the conflicts).
            assert len(choices)==1 or ram_layers,('Swapped map cell variants',layer,hex(pointer),x,y,choices)
            if len(choices)>1:conflicts.append(dict(layer=layer,map=f'0x{pointer:06X}',x=x,y=y,variants=len(choices)))
            (code,flip,mask,palette),count=next(iter(choices.items()))
            if palette not in palette_ids:palette_ids[palette]=len(palettes);palettes.append(palette)
            swapped_records.append(dict(layer=layer,map=f'0x{pointer:06X}',x=x,y=y,code=code,flip=flip,animation_mask=mask,palette=palette_ids[palette],observations=count))
    assert len(backdrops)==1 or allow_backdrop_timeline,'Multiple backdrops require --allow-backdrop-timeline and native event recovery before packing'
    report=dict(source='MAME mslugx euro; calibrated SCB background bank 0x40000; enemy slots excluded',backdrop=observations[0]['backdrop'],backdrop_values=sorted(backdrops),
        backdrop_policy='Observed per-frame palette values; multiple values require a recovered runtime event, not a camera threshold inferred from samples',
        camera_max=max(o['camera'] for o in observations),width=max(o['camera'] for o in observations)+320,
        selection='first observed; dynamic coordinate variants retained separately',
        layers=[dict(numerator=n,denominator=d) for n,d in ((1,2),(3,4),(1,1),(1,1))],
        **({'ram_layers':True,'slot_change_frames':slot_changes,'origin_lag_skips':len(lag_skips)} if ram_layers else {}),
        palettes=palettes,cells=records,swapped_cells=swapped_records,observations=observations,conflicts=conflicts,flash_frames=flashes,map_origin_events=origin_events)
    (output/'map.json').write_text(json.dumps(report,separators=(',',':'))+'\n')
    if skip_fade:print('Frames left out (palette fade):',len(faded),faded[:3],faded[-3:])
    print('Observed map:',len(records),'cells;',len(swapped_records),'swapped-map cells;',len(palettes),'palettes;',len(conflicts),'coordinate variants; camera',report['camera_max'])

if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('captures',type=Path,nargs='+');p.add_argument('--output',type=Path,default=Path('filesystem/mission1'))
    p.add_argument('--allow-backdrop-timeline',action='store_true',help='Keep per-observation backdrop values for research; runtime event mapping remains required')
    p.add_argument('--frames',type=lambda v:tuple(int(x) for x in v.split(':')),help='LO:HI inclusive snapshot frames (e.g. exclude the mission-end fade)')
    p.add_argument('--ram-layers',action='store_true',help='Background SCB slot ranges from the layer objects (+0x2C) instead of the fixed Mission 1 ranges')
    p.add_argument('--banks',type=lambda v:tuple(int(x,0) for x in v.split(',')),default=(4,),help='C-ROM banks (code bits 16..19) of background tiles')
    p.add_argument('--skip-fade',action='store_true',help='Leave out frames with a palette fade running (RAM 10991A..10991F nonzero): their pens are faded, and the first observed state would keep them')
    a=p.parse_args();extract(a.captures,a.output,a.allow_backdrop_timeline,a.frames,a.ram_layers,a.banks,a.skip_fade)
