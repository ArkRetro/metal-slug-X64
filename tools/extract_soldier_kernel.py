#!/usr/bin/env python3
"""Extract collision-kernel boundaries of Mission 1 region soldiers from MAME logs.

Input: capture directories made with tools/mame/trace_soldier_kernel.lua
(MAME 0.264, mslugx euro, print-only breakpoints). Only calls whose objects are
the terrain map 10C72E, the scene root 10E968 or none are kept; calls touching
any other collision object (the truck sequence after the region) are counted
and excluded, never approximated."""
import argparse,gzip,hashlib,json,re
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1]
OBJ={0:0,0x10C72E:1,0x10E968:2}
def fields(line):
    out={}
    for k,v in re.findall(r'(\w+)=([0-9A-Fa-f,]+)',line):
        out[k]=[int(x,16) for x in v.split(',')] if ',' in v else int(v,16)
    out['f']=int(re.search(r' f=(\d+)',line).group(1)) if ' f=' in line else None
    return out
def s32(v):return v-(1<<32) if v&0x80000000 else v
GRID_COLUMNS=json.loads((ROOT/'filesystem/mission1/collision.json').read_text())['columns']
def outside_grid(inp,out,scene):
    # Columns touched by a sweep lie between the start and the end positions
    # (plus one cell of probing). The terrain object is the world origin.
    xs=[s32(inp['d0'])>>16,(s32(inp['d0'])+s32(inp['d2']))>>16,s32(out['d0'])>>16]
    return max(xs)//8+1>=GRID_COLUMNS or min(xs)//8-1<0
def extract(capture,last_frame):
    lines=(capture/'debug.log').read_text().splitlines()
    assert 'MAME debugger version 0.264' in lines[0]
    sweeps=[];floors=[];excluded=0;beyond=0;pending=None;scene=None
    for line in lines:
        tag=line.split(' ',1)[0]
        if tag in ('KIN','FIN'):
            assert pending is None,line
            pending=(tag,fields(line));scene=None
        elif tag=='KSC':
            assert pending and scene is None;scene=fields(line)
        elif tag in ('KOUT','FOUT'):
            kind,inp=pending;out=fields(line);pending=None
            assert inp['a6']==out['a6'] and out['f']-inp['f'] in (0,1) and scene  # logic may straddle a video frame
            if inp['f']>last_frame:continue
            if inp['a0'] not in OBJ or out['a0'] not in OBJ:excluded+=1;continue
            if outside_grid(inp,out,scene):beyond+=1;continue
            assert scene['t']==scene['r'],'Terrain and scene root diverge'
            case=dict(frame=inp['f'],actor=inp['a6'],routine=inp['rt'],caller=inp['ret'],
                request=dict(x=s32(inp['d0']),y=s32(inp['d1']),dx=s32(inp['d2']),dy=s32(inp['d3']),parent=OBJ[inp['a0']],
                    mode4004=inp['mode'],b4008=inp['m8'],b4009=inp['m9'],b400a=inp['ma'],b400b=inp['mb'],b400d=inp['md']),
                scene=dict(x=s32(scene['t'][0]),y=s32(scene['t'][1]),prev_x=s32(scene['t'][2]),prev_y=s32(scene['t'][3]),w4018=scene['w4018'],w4068=scene['w4068']),
                result=dict(x=s32(out['d0']),y=s32(out['d1']),vx=s32(out['d2']),vy=s32(out['d3']),parent=OBJ[out['a0']],
                    flags=out['d6']&0xffff,kind=out['d7'],w4068=out['d4']&0xffff,d6=out['d6']))
            assert scene['gm']==0xB5332
            (sweeps if kind=='KIN' else floors).append(case)
    # A call interrupted by the scripted exit at the end frame has no exit record.
    assert pending is None or pending[1]['f']>last_frame
    return sweeps,floors,excluded,beyond
def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('captures',nargs='+',help='DIR:LAST_FRAME (last frame of the playable region)')
    p.add_argument('--output',type=Path,required=True);a=p.parse_args()
    fixed=None;report=dict(source='MAME 0.264 euro BIOS debugger breakpoints at ROM 013968/013C18 and 013DB2/01471C; print-only',captures={},sweeps=[],floors=[])
    for spec in a.captures:
        path,last=spec.rsplit(':',1);path=Path(path)
        sweeps,floors,excluded,beyond=extract(path,int(last))
        fx=(path/'program-fixed.bin').read_bytes()
        if fixed is None:fixed=fx
        assert fx==fixed
        report['captures'][path.name]=dict(last_frame=int(last),sweeps=len(sweeps),floors=len(floors),excluded_other_objects=excluded,excluded_outside_native_grid=beyond,
            debug_log_sha256=hashlib.sha256((path/'debug.log').read_bytes()).hexdigest())
        for c in sweeps+floors:c['capture']=path.name
        report['sweeps']+=sweeps;report['floors']+=floors
    report['group_map']=list(fixed[0xB5332:0xB533A]);report['group_map_rom']='0x0B5332'
    report['program_fixed_sha256']=hashlib.sha256(fixed).hexdigest()
    # Compact rows (column names in the header) keep the evidence reviewable
    # while the gzip container keeps the repository size bounded.
    sweep_cols=['capture','frame','actor','routine','caller','x','y','dx','dy','parent','mode4004','b4008','b4009','b400a','b400b','b400d',
        'scene_x','scene_y','scene_prev_x','scene_prev_y','w4018','w4068','out_x','out_y','out_vx','out_vy','out_parent','out_flags','out_kind','out_w4068']
    floor_cols=sweep_cols[:5]+['x','y','depth','parent','b4008','b400a','b400d','scene_x','scene_y','scene_prev_x','scene_prev_y','w4018','w4068','out_x','out_y','out_parent','out_kind','out_d6']
    def sweep_row(c):
        q,s,r=c['request'],c['scene'],c['result']
        return [c['capture'],c['frame'],c['actor'],c['routine'],c['caller'],q['x'],q['y'],q['dx'],q['dy'],q['parent'],q['mode4004'],q['b4008'],q['b4009'],q['b400a'],q['b400b'],q['b400d'],
            s['x'],s['y'],s['prev_x'],s['prev_y'],s['w4018'],s['w4068'],r['x'],r['y'],r['vx'],r['vy'],r['parent'],r['flags'],r['kind'],r['w4068']]
    def floor_row(c):
        q,s,r=c['request'],c['scene'],c['result']
        d6=r['d6']-(1<<32) if r['d6']&0x80000000 else r['d6']
        return [c['capture'],c['frame'],c['actor'],c['routine'],c['caller'],q['x'],q['y'],q['dy'],q['parent'],q['b4008'],q['b400a'],q['b400d'],
            s['x'],s['y'],s['prev_x'],s['prev_y'],s['w4018'],s['w4068'],r['x'],r['y'],r['parent'],r['kind'],d6]
    report['sweep_columns']=sweep_cols;report['floor_columns']=floor_cols
    report['sweeps']=[sweep_row(c) for c in report['sweeps']];report['floors']=[floor_row(c) for c in report['floors']]
    with gzip.open(a.output,'wt',compresslevel=9) as out:out.write(json.dumps(report,separators=(',',':'))+'\n')
    print({k:v for k,v in report['captures'].items()},len(report['sweeps']),'sweeps',len(report['floors']),'floors')
if __name__=='__main__':main()
