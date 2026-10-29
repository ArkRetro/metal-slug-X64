#!/usr/bin/env python3
"""Recover observed 8x8 collision cells from the arcade's scrolling RAM grid."""
import argparse,json,struct
from pathlib import Path

FOREGROUND,MIDDLE=0xc72e,0xd7b6

def collision_layer(ram):
    """Layer object whose grid the original query (PC 0xC7A1A) reads.

    A read-tap trace shows the foreground grid in use until the middle bank
    starts scrolling with it (camera 1023 in mission 1); the game then copies
    the ring into the middle object's grid and loads new columns only there.
    Equal position words select the same state that tools/neo_scene.py uses.
    """
    same=ram[FOREGROUND+0x14:FOREGROUND+0x1c]==ram[MIDDLE+0x14:MIDDLE+0x1c]
    return MIDDLE if same else FOREGROUND

def observed_cells(ram):
    """Yield only observed world cells; ring-buffer aliases are not extra terrain."""
    assert len(ram)==65536,'Expected a complete 64 KiB arcade RAM snapshot'
    camera=struct.unpack_from('>H',ram,0x2036)[0];layer=collision_layer(ram)
    for x in range(max(0,camera-8)//8*8,(camera+327)//8*8,8):
        for y in range(0,256,8):
            address=layer+0x88+((x&504)<<3)+((~y&504)>>3)
            yield camera,x//8,y//8,ram[address],address

def extract(captures,output):
    cells={};frames=0
    for path in sorted(captures.glob('ram*.bin'),key=lambda p:int(p.stem[3:])):
        ram=path.read_bytes()
        # Only inspect the observed viewport; the 64x64 RAM grid wraps at 512px.
        for camera,x,y,code,address in observed_cells(ram):
            key=(x,y)
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
