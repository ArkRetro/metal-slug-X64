#!/usr/bin/env python3
"""Compile data/tally_<name>_fixture.json.gz (tools/extract_tally_fixture.py)
into a C header for tools/test_neo_tally.c."""
import argparse,gzip,json
from pathlib import Path
def word(ram,address,full=False):
    """The byte (or with full the word) the original first read at address;
    -1 when not read."""
    if full:
        h,l=ram.get('%x'%address),ram.get('%x'%(address+1))
        return -1 if h is None or l is None else h<<8|l
    v=ram.get('%x'%address);return -1 if v is None else v
def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('fixture',type=Path);p.add_argument('--output',type=Path,required=True)
    p.add_argument('--name',required=True);a=p.parse_args()
    d=json.load(gzip.open(a.fixture))
    ups=d['updates']
    def c(h):return '{'+','.join('0x%s'%h[i:i+2] for i in range(0,len(h),2))+'}'
    rng=[];writes=[];vram=[]
    o=[f'/* {a.fixture.name}: {len(ups)} updates, {len(d["births"])+1} lives (tools/compile_tally_fixture.py) */']
    o.append(f'static const int {a.name}_births[]={{{",".join(str(b["node"]) for b in d["births"])}}};')
    o.append(f'static const int {a.name}_creators[]={{{",".join(str(b["creator"]) for b in d["births"])}}};')
    o.append(f'static const tally_update_t {a.name}_updates[]={{')
    for u in ups:
        r=u['ram']
        root=','.join(str(word(r,x,True)) for x in range(0x10e97c,0x10e98c,2))
        reads=[word(r,0x100000),word(r,0x10000e,True),word(r,0x10c33c),word(r,0x10c35d),word(r,0x10c363),
               word(r,0x10c5fc,True),word(r,0x10c5fe),word(r,0x10c5ff),word(r,0x10c306),word(r,0x10c307),word(r,0x10c33a),word(r,0x10e98c)]
        score=','.join(str(word(r,x)) for x in range(0x10c46a,0x10c46e))
        o.append(f' {{{u["frame"]},{u["pass_"]},{u["node"]},0x{u["routine"]:x},{c(u["pre"])},{1 if u["post"] else 0},{c(u["post"] or "00"*0xb0)},'
                 f'{{{",".join(map(str,reads))}}},{{{root}}},{{{score}}},{len(rng)},{len(u["rng"])},{len(writes)},{len(u["writes"])},{len(vram)},{len(u["vram"])}}},')
        rng+=u['rng'];writes+=u['writes'];vram+=u['vram']
    o.append('};')
    o.append(f'static const uint16_t {a.name}_rng[]={{{",".join(str(x) for x in rng) or "0"}}};')
    o.append(f'static const uint32_t {a.name}_writes[][3]={{{",".join("{0x%x,0x%x,0x%x}"%tuple(w) for w in writes) or "{0,0,0}"}}};')
    o.append(f'static const uint16_t {a.name}_vram[][2]={{{",".join("{0x%x,0x%x}"%tuple(w) for w in vram) or "{0,0}"}}};')
    a.output.write_text('\n'.join(o)+'\n')
if __name__=='__main__':main()
