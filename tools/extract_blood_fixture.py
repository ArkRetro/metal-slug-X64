#!/usr/bin/env python3
"""BLOOD ON evidence from a tools/capture_blood.lua capture (MAME 0.264,
mslugx euro, RAM 10FD8F forced to 1, MSX_BLOOD_PALREQ=1).

For every jsr 01B2BE call site (blood.csv) it lists the reading actors'
routines and the logical palette resources requested by the 009EA8 call of
the ON branch (debug.log PAL lines whose caller lies between the call site
and the end of its ON branch). For 0A9436 (no 009EA8 call: +14 takes the
slot +1A instead of +1B) it reads the resource of +14 in the RAM dumps of the
actors in routine 0A946C. Writes data/blood_fixture.json (addresses and
resource ids only, no ROM data).

Usage: tools/extract_blood_fixture.py CAPTURE_DIR"""
import argparse,collections,json,re,struct
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1]
# Call site -> (end of the ON branch, OFF palette of the euro recordings).
# The ON branch follows the bcs; its 009EA8 call is at site+0x12.
SITES={0x405f0:0x22f,0x40f98:0x3ba,0xa95f2:0x19,0xa969a:0x1a,0xa9742:0x1a,0xa9876:0x19,0x1c996:0x3ba,0x9efe4:0xb8,0x9f598:0xb8}
# Sites whose ON branch calls 009EA8 elsewhere than site+0x12: the 1C97A
# splash at 01C9A8; the swordsmen 09D846.. (09EFE4) at 09EFFE or 09F010 by
# the sign of +8C (the same resource on both). OFF: the immediate operand of
# the carry-set branch (01C9B2 0x3BA, 09F01A 0x0B8).
ON_CALLS={0x9efe4:(0x9effe,0x9f010)}
ARAB_DEATH=0xa9436
def resources(ram):
    out={}
    for i in range(256):
        rid=struct.unpack('>H',ram[0x68EA+16*i+10:0x68EA+16*i+12])[0]
        if rid not in (0,0xFFFF):out[i]=rid
    return out
def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('capture',type=Path)
    p.add_argument('--output',type=Path,default=ROOT/'data/blood_fixture.json')
    a=p.parse_args();cap=a.capture
    reads=collections.defaultdict(collections.Counter)
    for row in (cap/'blood.csv').read_text().splitlines()[1:]:
        frame,pc,caller,value,actor,routine=row.split(',')
        if pc=='01b2c4':reads[int(caller,16)][int(routine,16)]+=1
        assert value=='1',row
    pal=collections.defaultdict(collections.Counter)
    for m in re.finditer(r'PAL frame=(\w+) caller=(\w+) d0=(\w+) a6=(\w+) routine=(\w+)',(cap/'debug.log').read_text()):
        pal[int(m.group(2),16)][int(m.group(3),16)]+=1
    sites=[]
    for site,off in sorted(SITES.items()):
        on=collections.Counter()
        for c in ON_CALLS.get(site,(site+0x12,)):on+=pal.get(c,collections.Counter())
        assert len(on)<=1,(hex(site),on)
        sites.append(dict(site=hex(site),routines={hex(r):n for r,n in sorted(reads[site].items())},
            on=hex(next(iter(on))) if on else None,on_requests=sum(on.values()),off=hex(off)))
    seen=collections.Counter()
    for f in sorted(cap.glob('ram*.bin')):
        ram=f.read_bytes();r=resources(ram)
        for n in range(0x10a4,0x10a4+0xb0*100,0xb0):
            if struct.unpack('>I',ram[n:n+4])[0]==0xa946c:seen[r.get(ram[n+0x14],0)]+=1
    assert len(seen)==1,seen
    sites.append(dict(site=hex(ARAB_DEATH),routines={hex(r):n for r,n in sorted(reads[ARAB_DEATH].items())},
        on=hex(next(iter(seen))),on_requests=sum(seen.values()),off='0x3ba',note='resource of +14 in 0A946C (RAM dumps)'))
    other={hex(s):{hex(r):n for r,n in sorted(c.items())} for s,c in sorted(reads.items()) if s not in SITES and s!=ARAB_DEATH and s<0x100000}
    out=dict(source='MAME 0.264 mslugx euro, tools/capture_blood.lua (RAM 10FD8F = 1), Mission 1 complete',sites=sites,
        untranslated_sites_read=other)
    a.output.write_text(json.dumps(out,indent=1)+'\n');print(json.dumps(out,indent=1))
if __name__=='__main__':main()
