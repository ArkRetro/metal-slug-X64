#!/usr/bin/env python3
"""Recover observed 8x8 collision cells from the arcade's scrolling RAM grid."""
import argparse,json,struct
from pathlib import Path

def extract(captures,output):
    cells={};frames=0
    for path in sorted(captures.glob('ram*.bin'),key=lambda p:int(p.stem[3:])):
        ram=path.read_bytes();camera=struct.unpack_from('>H',ram,0x2036)[0]
        # Only inspect the observed viewport; the 64x64 RAM grid wraps at 512px.
        for x in range(max(0,camera-8)//8*8,(camera+327)//8*8,8):
            for y in range(0,256,8):
                code=ram[0xc72e+0x88+((x&504)<<3)+((~y&504)>>3)]
                key=(x//8,y//8)
                assert key not in cells or cells[key]==code,('Dynamic collision cell',path,key)
                cells[key]=code
        frames+=1
    width=max(x for x,y in cells)+1
    assert all((x,y) in cells for x in range(width) for y in range(32))
    data=bytes(cells[x,y] for x in range(width) for y in range(32))
    output.parent.mkdir(parents=True,exist_ok=True)
    report=dict(source='MAME mslugx euro, RAM 0x10C72E + 0x88, observed viewports',
                columns=width,rows=32,frames=frames,cells_hex=data.hex())
    output.write_text(json.dumps(report,separators=(',',':'))+'\n')
    print('Original collision grid:',width,'x32;',sum(bool(c) for c in data),'nonempty cells;',frames,'snapshots')

if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('captures',type=Path)
    p.add_argument('--output',type=Path,default=Path('filesystem/mission1/collision.json'))
    a=p.parse_args();extract(a.captures,a.output)
