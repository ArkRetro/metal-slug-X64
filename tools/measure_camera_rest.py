#!/usr/bin/env python3
"""Measure the original camera/parallax relations on a per-frame RAM trace of
tools/capture_mission_rest.lua (trace.csv) and report how exactly each rule
holds. Quantities (RAM): progress P = 10E99E word (also 102036); displayed
foreground X = -(10C72E+84); layer words +84/+86 of 10D7B6 (middle bank),
10E83E (sky) and 10E8C6; camera position 10E98E/10E992 (Q16.16, ROM 0C7CA6);
shake 10E9CC; script camera stop 10E9BC with operands 10E9C0..C6 (ROM
0C7B0C). Nothing is fitted: every rule is a fixed formula and the report
counts the frames where it holds and lists the others."""
import argparse,collections,csv,json
from pathlib import Path

def Hc(r):return (int(r['cam_y'])+0xffff)>>16

def rules(r,prev):
    X=-int(r['fore_x']);P=int(r['camera']);H=int(r['cam_y'])>>16;S=int(r['shake'])>>16
    out={
        'middle_x == X (full-speed middle bank)':-int(r['mid_x'])==X,
        'sky_x == (X+1)//2':-int(r['sky_x'])==(X+1)//2,
        'object_10E8C6_x == X':-int(r['obj4_x'])==X,
        'middle_y == foreground_y':int(r['mid_y'])==int(r['fore_y']),
        'foreground_y == 256 - H - S':int(r['fore_y'])==256-H-S,
        'sky_y == 264 - ceil(H/2) - S':int(r['sky_y'])==264-(H+1)//2-S,
        'foreground_y == 256 - Hc - S, Hc = ceil(camera_y), previous frame':prev is not None and int(r['fore_y'])==256-Hc(prev)-(int(prev['shake'])>>16),
        'sky_y == 264 - ceil(Hc/2) - S, previous frame':prev is not None and int(r['sky_y'])==264-(Hc(prev)+1)//2-(int(prev['shake'])>>16),
        'X == ceil(camera_x of the previous frame)':prev is not None and X==(int(prev['cam_x'])+0xffff)>>16,
        'P == 102036 word':P==int(r['camera_word']),
        '|X - P| <= 3 without stop operands':None,
        'P+op1 <= X <= P+op3 with stop operands':None,
    }
    lock=int(r['lock']);params=[int(v) for v in r['lock_params'].split()]
    bounded=lock!=-1 and params[0]!=-32768 and params[2]!=-32768
    if bounded:out['P+op1 <= X <= P+op3 with stop operands']=P+params[0]<=X<=P+params[2]
    else:out['|X - P| <= 3 without stop operands']=-3<=X-P<=3
    return out

def measure(path,first,last):
    rows=[r for r in csv.DictReader(open(path)) if first-1<=int(r['frame'])<=last]
    counts=collections.defaultdict(lambda:[0,0,0]);fails=collections.defaultdict(list);prev=None
    shake_frames=0;by_frame={int(r['frame']):r for r in rows}
    for r in rows:
        f=int(r['frame'])
        if f<first:prev=r;continue
        if int(r['shake']):shake_frames+=1
        # A shake (10E9CC) in this or the two previous frames.
        shaking=any(int(by_frame[g]['shake']) for g in range(f-2,f+1) if g in by_frame)
        for k,v in rules(r,prev).items():
            if v is None:continue
            counts[k][0 if v else 1]+=1
            if not v and shaking:counts[k][2]+=1
            if not v and len(fails[k])<12:fails[k].append(f)
        prev=r
    # Script/map stops seen in the trace: (first frame, P, lock, operands).
    stops=[];last_key=None
    for r in rows:
        key=(r['lock'],r['lock_params'])
        if key!=last_key:
            lock=int(r['lock'])
            stops.append(dict(frame=int(r['frame']),progress=int(r['camera']),stop=None if lock==-1 else lock>>16,
                operands=[int(v) for v in r['lock_params'].split()]))
            last_key=key
    heights=collections.Counter(int(r['cam_y'])>>16 for r in rows)
    return dict(frames=len(rows)-1,first=first,last=last,shake_frames=shake_frames,
        rules={k:dict(exact=v[0],different=v[1],different_while_shaking=v[2],first_differences=fails[k]) for k,v in counts.items()},
        stops=stops,heights=dict(sorted(heights.items())))

if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('trace',type=Path);p.add_argument('--first',type=int,default=5300);p.add_argument('--last',type=int,default=13740)
    p.add_argument('--output',type=Path)
    a=p.parse_args();r=measure(a.trace,a.first,a.last)
    if a.output:a.output.write_text(json.dumps(r,indent=1)+'\n')
    for k,v in r['rules'].items():print(f"{v['exact']:6d} exact {v['different']:5d} different ({v['different_while_shaking']} shaking)  {k}  {v['first_differences'][:6]}")
