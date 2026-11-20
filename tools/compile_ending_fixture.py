#!/usr/bin/env python3
"""Compile data/ending_<name>_fixture.json.gz (tools/extract_ending_fixture.py)
into a C header for tools/test_neo_ending.c."""
import argparse,gzip,json
from pathlib import Path
def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('fixture',type=Path);p.add_argument('--output',type=Path,required=True)
    p.add_argument('--name',required=True);a=p.parse_args()
    d=json.load(gzip.open(a.fixture));ups=d['updates'];n=a.name
    def c(h):return '{'+','.join('0x%s'%h[i:i+2] for i in range(0,len(h),2))+'}'
    reads=[];writes=[];vram=[];rng=[]
    o=[f'/* {a.fixture.name}: {len(ups)} updates, {len(d["births"])+1} lives (tools/compile_ending_fixture.py) */']
    o.append(f'static const int {n}_births[]={{{",".join(str(b["node"]) for b in d["births"]) or "-1"}}};')
    o.append(f'static const int {n}_creators[]={{{",".join(str(b["creator"]) for b in d["births"]) or "-1"}}};')
    o.append(f'#define ENDING_NBIRTHS {len(d["births"])}')
    o.append(f'#define ENDING_NLIVES {max([u["node"] for u in ups]+[b["node"] for b in d["births"]])+1}')
    sk=[tuple(int(x,16) for x in r.split('-')) for r in d.get('skip',[])]
    o.append('#define ENDING_SKIP {%s}'%','.join('{0x%x,0x%x}'%t for t in sk+[(0x10f200,0x10f400),(0x100016,0x100088)]))
    o.append(f'#define ENDING_ROOT 0x{int(d["roots"][0],16):x}u')
    o.append(f'static const ending_update_t {n}_updates[]={{')
    for u in ups:
        rd=sorted((int(k,16),v) for k,v in u['ram'].items())
        o.append(f' {{{u["frame"]},{u["pass_"]},{u["node"]},0x{u["routine"]:x},{c(u["pre"])},{1 if u["post"] else 0},{c(u["post"] or "00"*0xb0)},'
                 f'{len(reads)},{len(rd)},{len(writes)},{len(u["writes"])},{len(vram)},{len(u["vram"])},{len(rng)},{len(u["rng"])}}},')
        reads+=rd;writes+=u['writes'];vram+=u['vram'];rng+=u['rng']
    o.append('};')
    o.append(f'static const uint32_t {n}_reads[][2]={{{",".join("{0x%x,0x%x}"%tuple(w) for w in reads) or "{0,0}"}}};')
    o.append(f'static const uint32_t {n}_writes[][2]={{{",".join("{0x%x,0x%x}"%tuple(w) for w in writes) or "{0,0}"}}};')
    o.append(f'static const uint16_t {n}_vram[][2]={{{",".join("{0x%x,0x%x}"%tuple(w) for w in vram) or "{0,0}"}}};')
    o.append(f'static const uint32_t {n}_rng[]={{{",".join(str(x) for x in rng) or "0"}}};')
    a.output.write_text('\n'.join(o)+'\n')
if __name__=='__main__':main()
