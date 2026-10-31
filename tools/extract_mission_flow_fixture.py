#!/usr/bin/env python3
"""Mission flow evidence from a full-game capture (tools/capture_fullgame.lua).

Every routine change of the mission actor 101364 (actors.csv: its +00 resume
label) and, when a RAM dump falls between two changes, the flow words read
there: 42F4 (mission), 42F6 (stage id), 42F8/42FA (mission/record index of
1829E), 4303 (stage end) and +6E (current record of the table 017D44), with
A5 = 0x108000. tools/test_neo_mission.c replays the sequence with
src/neo_mission.c."""
import argparse,csv,json,glob,re,struct
from pathlib import Path

def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('capture',type=Path);p.add_argument('--output',type=Path,required=True)
    a=p.parse_args();cap=a.capture
    events=[dict(frame=int(r['frame']),routine=int(r['routine'],16),previous=int(r['previous'],16))
            for r in csv.DictReader(open(cap/'actors.csv')) if r['node']=='101364' and r['event']=='state']
    dumps=sorted(int(re.findall(r'\d+',Path(x).name)[0]) for x in glob.glob(str(cap/'ram*.bin')))
    for i,e in enumerate(events):
        nxt=events[i+1]['frame'] if i+1<len(events) else 1<<30
        f=next((d for d in dumps if e['frame']<d<nxt),None)
        if f is None:continue
        r=(cap/f'ram{f}.bin').read_bytes();w=lambda x:struct.unpack_from('>H',r,x-0x100000)[0]
        e['ram']=dict(frame=f,mission=w(0x10C2F4),stage=w(0x10C2F6),index=w(0x10C2F8),record_index=w(0x10C2FA),
                      end=r[0xC303],record=struct.unpack_from('>I',r,0x1364+0x6e)[0],cleared=r[0xC314:0xC31A].hex())
    a.output.write_text(json.dumps(dict(source=str(cap.name)+' actors.csv/ram*.bin (tools/capture_fullgame.lua)',events=events),indent=0)+'\n')
    print(len(events),'events',sum('ram' in e for e in events),'with RAM')
if __name__=='__main__':main()
