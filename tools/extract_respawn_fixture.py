#!/usr/bin/env python3
"""MAME respawn capture -> data/respawn_fixture.json (B33, docs/JEFE_MISION2.md).

Input: a tools/capture_fullgame.lua run with MSX_FULL_DEBUG=1 and the
breakpoints of docs/JEFE_MISION2.md ("Reaparición"): RSPA at 019758 (the
record placed by 0196E8, after its 0143E4 step), RSPL/RSPF and RSPR/RSPG
around each 013DB2 call of 014742's left and right scans (inputs, carry,
D6), RSPE at its end, RSPD after 0196E8 (0C1266: D7 = entrance type) and
its trace.csv (camera progress 10E99E). The stage's respawn entries
(descriptor +4) are read from the ROM. Numbers only."""
import argparse,csv,json,re,sys
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1]
sys.path.insert(0,str(ROOT/'tools'))
from decode_level_script import Program
def fields(line):return {k:v for k,v in re.findall(r'(\w+)=([0-9A-Fa-f]+)',line)}
def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('capture',type=Path);p.add_argument('--descriptor',default='16DFE')
    p.add_argument('--rom',type=Path,default=Path('/home/user/roms/mslugx.zip'))
    p.add_argument('--output',type=Path,default=ROOT/'data/respawn_fixture.json')
    a=p.parse_args()
    rom=Program(a.rom);desc=int(a.descriptor,16);table=rom.u32(desc+4);t=table
    while rom.u16(t+12)!=0xffff:t+=12
    blob=rom.read(table,t+14-table)
    progress={}
    with open(a.capture/'trace.csv') as f:
        r=csv.reader(f);h=next(r);i=h.index('camera_word')
        for row in r:progress[int(row[0])]=int(row[i])
    cases=[];cur=None
    for line in open(a.capture/'debug.log'):
        if not line.startswith('RSP'):continue
        tag=line[:4];v=fields(line)
        if tag=='RSPA':
            cur=dict(frame=int(v['frame']),progress=progress[int(v['frame'])],record=int(v['rec'],16),x=int(v['x'],16),y=int(v['y'],16),
                     px=int(v['px'],16),py=int(v['py'],16),root_x=int(v['rx'],16),root_y=int(v['ry'],16),queries=[]);cases.append(cur)
        elif cur is None:continue
        elif tag in('RSPL','RSPR'):cur['queries'].append([0 if tag=='RSPL' else 1,int(v['x'],16),int(v['px'],16),int(v['py'],16),int(v['lim'],16)])
        elif tag in('RSPF','RSPG'):cur['queries'][-1]+= [int(v['c'],16),int(v['d6'],16)]
        elif tag=='RSPE':cur['search_x']=int(v['x'],16);cur['result']=int(v['res'],16)
        elif tag=='RSPD':cur['type']=int(v['d7'],16);cur['placed_x']=int(v['x'],16);cur=None
    cases=[c for c in cases if 'type' in c]
    out=dict(source=f'{a.capture.name}: capture_fullgame.lua from at_92700 (Mission 2 boss tower), MSX_FULL_DEBUG breakpoints',
             descriptor=f'0x{desc:06X}',table=f'0x{table:06X}',table_hex=blob.hex(),cases=cases)
    a.output.write_text(json.dumps(out,indent=0)+'\n')
    print(len(cases),'respawns',sum(len(c['queries']) for c in cases),'floor searches ->',a.output)
if __name__=='__main__':main()
