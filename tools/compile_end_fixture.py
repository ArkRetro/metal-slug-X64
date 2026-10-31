#!/usr/bin/env python3
"""Compile data/<name>_fixture.json.gz (tools/extract_end_fixture.py) into a C
header for tools/test_neo_end.c."""
import argparse,gzip,json
from pathlib import Path
def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('fixture',type=Path);p.add_argument('--output',type=Path,required=True)
    p.add_argument('--name',required=True);a=p.parse_args()
    d=json.load(gzip.open(a.fixture))
    ups=d['updates'];root=ups[0]['node']
    ids={root:0}
    for b in d['births']:
        if b['node'] in ids:raise SystemExit('node reused in the family: %x'%b['node'])
        ids[b['node']]=len(ids)
    def c(h):return '{'+','.join('0x%s'%h[i:i+2] for i in range(0,len(h),2))+'}'
    def word(r,k):
        if k not in r:return -1
        m,v=r[k];return v if m==0xffff else (v>>8 if m==0xff00 else v&0xff)
    o=[f'/* {a.fixture.name}: {len(ups)} updates, {len(ids)} nodes (tools/compile_end_fixture.py) */']
    o.append(f'static const int {a.name}_births[]={{{",".join(str(ids[b["node"]]) for b in d["births"])}}};')
    o.append(f'static const end_update_t {a.name}_updates[]={{')
    for u in ups:
        r=u['ram']
        rootv=','.join(str(word(r,'%x'%x)) for x in range(0x10e97c,0x10e98c,2))
        corr=word(r,'10e98c')
        o.append(f' {{{u["frame"]},{ids[u["node"]]},0x{u["routine"]:x},{c(u["pre"])},{1 if u["post"] else 0},{c(u["post"] or "00"*0xb0)},'
                 f'{word(r,"10c33a")},{word(r,"10b954")},{word(r,"10b95c")},{word(r,"10c2f4")},{{{rootv}}},{corr}}},')
    o.append('};')
    a.output.write_text('\n'.join(o)+'\n')
if __name__=='__main__':main()
