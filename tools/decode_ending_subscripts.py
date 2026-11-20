#!/usr/bin/env python3
"""The credits' sub-script records into data/ending_script.json (key sub_scripts).

The two figures 0A8C62 of the credits' script (section 22, stage 0x2C) start the record [ROM 00087A + 4 * (+74)]
(019E72): +74 is 4, 5, 6, 11 or 12 for the first (pages, timers, the music and pictures) and 7..10 for the second
(pictures). Decoded statically with tools/decode_level_script.py; tools/compile_soldier_ai_tables.py takes their spawn
table entries (ROM 0197DC) from here.

usage: decode_ending_subscripts.py mslugx.zip"""
import json,sys
from pathlib import Path
sys.path.insert(0,str(Path(__file__).resolve().parent))
from decode_level_script import Program,decode
ROOT=Path(__file__).resolve().parents[1]
def main():
    if len(sys.argv)!=2:sys.exit(__doc__)
    prog=Program(sys.argv[1]);f=ROOT/'data/ending_script.json';d=json.loads(f.read_text());subs=[]
    for k in range(4,13):
        base=prog.u32(0x87a+4*k);st=decode(prog,base)
        subs.append(dict(record=k,base=f'0x{base:06X}',end=st[-1]['address'],statements=st))
    d['sub_scripts']=subs;f.write_text(json.dumps(d,indent=1)+'\n')
    print(', '.join(f"{s['record']}: {s['base']}..{s['end']} ({len(s['statements'])})" for s in subs))
if __name__=='__main__':main()
