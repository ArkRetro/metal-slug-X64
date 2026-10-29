"""Recovered collision-stamp model for the observed ground vehicle variant.

This models the original scrolling RAM grid for verification. It is not yet a
native vehicle controller, soldier-wave trigger, or runtime frame schedule.
"""
import json
from pathlib import Path
import struct

ROOT=Path(__file__).resolve().parents[1]
FOOTPRINT=json.loads((ROOT/'data/vehicle_wreckage.json').read_text())


def stamp_destroyed_vehicle(ram,x16,y16,health,flags):
    assert len(ram)==65536,'Expected complete arcade RAM'
    result=bytearray(ram);writes=[]
    # Observed eligible wreck: zero health, enabled footprint, not flag-5 mode.
    # Soldier/wave requirements belong to the unimplemented vehicle controller.
    if health!=0 or not flags&1 or flags&32:
        return result,writes
    origin_x=struct.unpack_from('>h',ram,0xc72e+0x14)[0]
    origin_y=struct.unpack_from('>h',ram,0xc72e+0x18)[0]
    min_x=(-64-origin_x)>>3
    min_ring_y=-(((128-origin_y)>>3)+63)
    width,height=FOOTPRINT['width'],FOOTPRINT['height']
    base_x=(x16>>19)+FOOTPRINT['anchor_x_cells']
    base_y=(y16>>19)+FOOTPRINT['anchor_y_cells']
    material=16 if flags&2 else 0
    for col in range(width):
        cell_x=base_x+col
        if ((cell_x-min_x)&0xffff)>=64:
            continue
        for row in range(height):
            cell_y=base_y+height-1-row
            ring_y=~cell_y
            if ((ring_y-min_ring_y)&0xffff)>=64:
                continue
            address=0xc7b6+(cell_x&63)*64+(ring_y&63)
            old=result[address]
            if old&15:
                continue # This footprint's replacement mask protects solid cells.
            new=(old&0xe0)|material|FOOTPRINT['rows_high_to_low'][row][col]
            if new!=old:
                result[address]=new
                writes.append(dict(address=0x100000+address,old=old,new=new,
                    world_x=cell_x*8,world_y=cell_y*8))
    return result,writes
