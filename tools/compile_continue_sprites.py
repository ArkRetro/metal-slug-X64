#!/usr/bin/env python3
"""Compile data/continue_sprites_fixture.json.gz into a C header for
tools/test_neo_continue_sprites.c."""
import argparse,gzip,json
from pathlib import Path
def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('fixture',type=Path);p.add_argument('--output',type=Path,required=True)
    a=p.parse_args();d=json.load(gzip.open(a.fixture))
    o=[f'/* {a.fixture.name}: {len(d["updates"])} updates (tools/compile_continue_sprites.py) */',
       'static const continue_update_t continue_updates[]={']
    for u in d['updates']:
        es=','.join('{%d,%d,%d,%d,0x%x}'%tuple(e) for e in u['entries'])
        o.append(f' {{{u["frame"]},0x{u["counter"]:x},{u["front"]},{len(u["entries"])},{{{es}}}}},')
    o.append('};')
    a.output.write_text('\n'.join(o)+'\n')
if __name__=='__main__':main()
