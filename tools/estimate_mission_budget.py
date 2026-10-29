#!/usr/bin/env python3
"""Scene-pack and cache budget of the whole Mission 1 map (estimate).

Merges the validated region map (filesystem/mission1/map.json) with the rest
of the mission (filesystem/mission1/rest/map.json, tools/extract_mission_map.py
on a tools/capture_mission_rest.lua capture), adds every recorded coordinate
variant (event patches such as the 0x0B6566 building), runs the unchanged
tools/pack_scene.py in a scratch tree and then the working-set rule of
tools/check_scene_budget.py up to the new camera limit. Nothing in the
repository's filesystem/, build/ or romfs/ is written. Overlapping cells of
the two maps must be identical. Not a runtime change: the scene renderer
still packs the region map only."""
import argparse,json,os,re,shutil,subprocess,sys,tempfile
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1]
sys.path.insert(0,str(ROOT/'tools'))
from check_scene_budget import table,scroll,CHUNK,WINDOW

def merge(region,rest):
    out=dict(region);palettes=list(region['palettes']);index={tuple(p):i for i,p in enumerate(palettes)}
    def pid(p):
        p=tuple(p)
        if p not in index:index[p]=len(palettes);palettes.append(list(p))
        return index[p]
    cells={(c['layer'],c['x'],c['y']):dict(c) for c in region['cells']}
    overlap=added=variants=0
    for c in rest['cells']:
        c=dict(c,palette=pid(rest['palettes'][c['palette']]));key=(c['layer'],c['x'],c['y'])
        if key in cells:
            a=cells[key]
            assert (a['code'],a['flip'],a['animation_mask'],tuple(palettes[a['palette']]))==(c['code'],c['flip'],c['animation_mask'],tuple(palettes[c['palette']])),('Overlapping cell differs between the maps',key)
            overlap+=1
        else:cells[key]=c;added+=1
    # Event variants must be resident as well: add each recorded alternative as
    # a virtual cell of the same layer (only its pages matter here).
    extra=[]
    for m in (region,rest):
        for v in m['conflicts']:
            for ch in v['choices']:
                extra.append(dict(layer=v['layer'],x=v['x'],y=v['y'],code=ch['code'],flip=ch['flip'],animation_mask=ch['animation'],palette=pid(ch['palette']),observations=ch['count']))
                variants+=1
    out.update(palettes=palettes,cells=list(cells.values())+extra,camera_max=max(region['camera_max'],rest['camera_max']),
               backdrop_values=[region['backdrop']])
    return out,dict(overlap_cells=overlap,added_cells=added,variant_cells=variants,palettes=len(palettes))

def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--region',type=Path,default=ROOT/'filesystem/mission1/map.json')
    p.add_argument('--rest',type=Path,default=ROOT/'filesystem/mission1/rest/map.json')
    p.add_argument('--switch',type=int,default=1023)
    p.add_argument('--output',type=Path)
    a=p.parse_args()
    region=json.loads(a.region.read_text());rest=json.loads(a.rest.read_text())
    merged,stats=merge(region,rest)
    reports={}
    with tempfile.TemporaryDirectory() as tmp:
        tmp=Path(tmp);(tmp/'tools').mkdir();(tmp/'filesystem/mission1').mkdir(parents=True)
        for name in ('pack_scene.py','mission_tiles.py','export_marco.py'):shutil.copy(ROOT/'tools'/name,tmp/'tools'/name)
        for f in (ROOT/'filesystem').iterdir():
            if f.is_file():os.symlink(f,tmp/'filesystem'/f.name)
        os.symlink(ROOT/'filesystem/mission1/props.json',tmp/'filesystem/mission1/props.json')
        for label,model in (('region',region),('mission',merged)):
            (tmp/'filesystem/mission1/map.json').write_text(json.dumps(model))
            run=subprocess.run([sys.executable,str(tmp/'tools/pack_scene.py')],capture_output=True,text=True,cwd=tmp)
            assert run.returncode==0,run.stderr
            pack=json.loads((tmp/'build/scene_pack_report.json').read_text())
            meta=(tmp/'build/scene_meta.h').read_text()
            reports[label]=dict(pack={k:pack[k] for k in ('pages','palettes','chunks','decoded_bytes','packed_bytes')},
                                budget=working_set(meta,model['camera_max'],a.switch))
    result=dict(source='tools/pack_scene.py (unchanged, scratch tree) + tools/check_scene_budget.py rule',merge=stats,**reports)
    if a.output:a.output.write_text(json.dumps(result,indent=1)+'\n')
    print(json.dumps(result,indent=1))

def working_set(meta,camera_max,switch):
    chunks=int(re.search(r'SCENE_CHUNK_COUNT (\d+)',meta).group(1))
    banks=table(meta,'scene_banks');images=table(meta,'scene_images')
    map_images=table(meta,'scene_map_images');cells=table(meta,'scene_map_cells_data')
    fixed=set()
    for start,first,second,*_ in banks:fixed.update(images[i][0]//8 for i in range(start,start+first+second))
    per=[]
    for c in range(camera_max+1):
        used=set()
        for layer,x,y,*phases in cells:
            source=min(layer,3)
            if (source==1 and c>=switch) or (source==2 and c<switch):continue
            if -48<=x-scroll(source,c)<352:used.update(map_images[q][0]//8 for q in phases)
        per.append(used)
    worst=(0,0)
    for c in range(camera_max+1):worst=max(worst,(len(set(fixed).union(*per[c:c+WINDOW])),c))
    caches={'4MiB':512*1024//CHUNK,'8MiB':3*1024*1024//CHUNK}
    return dict(chunks=chunks,fixed_actor_chunks=len(fixed),worst_working_set=worst[0],worst_camera=worst[1],
        cache_slots={k:min(v,chunks) for k,v in caches.items()},
        fits_without_eviction={k:chunks<=v for k,v in caches.items()},
        working_set_fits={k:worst[0]<=min(v,chunks) for k,v in caches.items()})

if __name__=='__main__':main()
