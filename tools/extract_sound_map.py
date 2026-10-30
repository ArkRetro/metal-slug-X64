#!/usr/bin/env python3
"""Summarise the 68000 sound requests of gameplay captures.

Reads debug.log of tools/mame/run_gameplay_audio.py captures. Every REQ line
is one byte queued by ROM 01209C into the ring 0x10BF68. Request words go
through ROM 0120F8: words >= 0x2000 or with a low byte < 0x20 are dropped;
otherwise the high byte (a driver mode/bank selector < 0x20, only queued when
it differs from the last one, ROM 0120A4) and the low byte follow. Direct
calls to 01209C queue a single byte. The caller is the return address on the
stack: for the low byte of a word request it is the long at SP; the high-byte
line (return address 01218A) is folded into the next one.

Output: per request word, the callers, counts and frames of every capture,
plus the LATCH bytes the 68000 actually wrote (ROM 01221A, one per frame).
The result carries numbers only (no audio, no ROM bytes)."""
import argparse,collections,json,re
from pathlib import Path
REQ=re.compile(r'REQ frame=(\d+) byte=(\w+) d0=(\w+) s0=(\w+) s4=(\w+) s6=(\w+)')
LATCH=re.compile(r'LATCH frame=(\d+) pc=(\w+) value=(\w+)')
def parse(path):
    reqs=[];latch=[]
    for line in Path(path).open(errors='replace'):
        m=REQ.match(line)
        if m:
            frame,byte,d0,s0=int(m[1]),int(m[2],16),int(m[3],16),int(m[4],16)
            if s0==0x1218a:continue          # high byte of the next request
            reqs.append(dict(frame=frame,word=d0,caller=s0,byte=byte))
            continue
        m=LATCH.match(line)
        if m:latch.append(dict(frame=int(m[1]),pc=int(m[2],16),value=int(m[3],16)))
    return reqs,latch
def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('captures',nargs='+',help='NAME=DIR (capture directory with debug.log)')
    p.add_argument('--first-frame',type=int,default=3800,help='ignore the attract/tutorial before the mission')
    p.add_argument('--output',type=Path,required=True);a=p.parse_args()
    table=collections.defaultdict(lambda:dict(callers=collections.Counter(),frames={}))
    sources={}
    for item in a.captures:
        name,_,directory=item.partition('=');reqs,latch=parse(Path(directory)/'debug.log')
        sources[name]=dict(requests=len(reqs),latch_writes=len(latch))
        for r in reqs:
            if r['frame']<a.first_frame:continue
            e=table['%04X'%r['word']];e['callers']['%06X'%r['caller']]+=1
            e['frames'].setdefault(name,[]).append(r['frame'])
    out=dict(source='MAME 0.264 mslugx euro; REQ/LATCH debugger watchpoints (tools/mame/capture_gameplay_audio.lua)',
             first_frame=a.first_frame,captures=sources,
             requests={k:dict(callers=dict(v['callers']),count=sum(v['callers'].values()),frames=v['frames'])
                       for k,v in sorted(table.items())})
    a.output.write_text(json.dumps(out,indent=1)+'\n')
    for k,v in out['requests'].items():print(k,v['count'],v['callers'])
if __name__=='__main__':main()
