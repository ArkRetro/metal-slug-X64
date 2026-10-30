#!/usr/bin/env python3
"""Decode every level script of the script table (fixed ROM 0x000800, 30 long
pointers) with tools/decode_level_script.py and, given a full-game capture of
tools/capture_fullgame.lua, check that every position where the script actor
rested (+6E, script.csv) is a decoded blocking statement of its section.

`--missions` maps missions to table sections (from the capture's sections.csv;
docs/FULLGAME.md); one data/missionN_script.json is written per mission with
the decoded sections in play order. Nothing is executed; ROM bytes are only
parsed (P1 + P2 bank 1, the bank active while the scripts run)."""
import argparse,csv,json,struct
from pathlib import Path
from decode_level_script import Program,decode,walk,blocking_addresses

TABLE=0x000800;ENTRIES=30

def table(prog):
    return [prog.u32(TABLE+4*i) for i in range(ENTRIES)]

def summary(prog,base):
    s=decode(prog,base);spawns=[x for x in walk(s) if x['op']==7]
    cams=[x['camera'] for x in s if x['op']==0]
    return dict(base=f'0x{base:06X}',end=s[-1]['address'],top_level_statements=len(s),spawn_records=len(spawns),
        camera_waits=len(cams),camera_max=max(cams) if cams else None,
        stops=[dict(address=x['address'],camera=x['camera'],**x['lock']) for x in s if x['op']==0 and 'lock' in x],
        autoscroll=[x['address'] for x in walk(s) if x['op'] in (0xd,0xe)],
        constructors=sorted({x['constructor'] for x in spawns})),s

def observed(capture):
    """(frame, section, pc) of every script actor rest position, from the
    capture's script.csv (main script actor 0x101414 only) and sections.csv."""
    sections=[(int(r['frame']),int(r['section'])) for r in csv.DictReader(open(capture/'sections.csv'))]
    out=[]
    for r in csv.DictReader(open(capture/'script.csv')):
        if r['node']!='101414':continue
        f=int(r['frame']);pc=int(r['pc'],16)
        if pc==0:continue
        sec=max((s for s in sections if s[0]<=f),default=(0,-1))[1]
        out.append((f,sec,pc))
    return out

def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('zip',type=Path);p.add_argument('--capture',type=Path)
    p.add_argument('--missions',type=json.loads,help='JSON {"2":[1,2,3],...}: mission -> sections in play order')
    p.add_argument('--output-dir',type=Path,default=Path('data'));p.add_argument('--report',type=Path)
    a=p.parse_args();prog=Program(a.zip);bases=table(prog)
    decoded={};report=dict(source='Script table ROM 0x000800; grammar of ROM 019F7E/01A0A6/01A372/01A5CA (tools/decode_level_script.py)',sections=[])
    for i,b in enumerate(bases):
        if not 0x200000<=b<0x300000:
            report['sections'].append(dict(index=i,base=f'0x{b:06X}',note='outside the banked script window'));continue
        info,s=summary(prog,b);decoded[i]=s;report['sections'].append(dict(index=i,**info))
    if a.capture:
        checked=0;bad=[]
        for f,sec,pc in observed(a.capture):
            ok=sec in decoded and pc in blocking_addresses(decoded[sec])
            # A section whose base is not its first statement (Mission 1:
            # 0x2D62A8 then 0x2D6314) is still the same decoded list.
            if not ok:bad.append(dict(frame=f,section=sec,pc=f'0x{pc:06X}'))
            checked+=1
        report['observed_rest_positions']=dict(capture=str(a.capture),checked=checked,not_blocking=bad)
        assert not bad,('Observed script positions that are not decoded blocking statements',bad[:10])
    if a.missions:
        for mission,secs in a.missions.items():
            out=dict(source=report['source']+'; P1 + P2 bank 1 of the user MAME set',mission=int(mission),
                sections=[dict(index=i,base=f'0x{bases[i]:06X}',end=decoded[i][-1]['address'],statements=decoded[i]) for i in secs],
                constructors=sorted({x['constructor'] for i in secs for x in walk(decoded[i]) if x['op']==7}))
            (a.output_dir/f'mission{mission}_script.json').write_text(json.dumps(out,indent=1)+'\n')
    if a.report:a.report.write_text(json.dumps(report,indent=1)+'\n')
    for s in report['sections']:print(s['index'],s['base'],s.get('top_level_statements'),s.get('spawn_records'),s.get('camera_max'),len(s.get('stops',[])),len(s.get('constructors',[])))
    if a.capture:print('observed rest positions',report['observed_rest_positions']['checked'],'all blocking')

if __name__=='__main__':main()
