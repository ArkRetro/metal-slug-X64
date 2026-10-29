"""Exact model of the original collision stamp routine ROM 0C88B0.

An actor (A6) whose parent (+38) is the foreground (0x10C72E) or middle
(0x10D7B6) layer object writes a template into that layer's 64x64 ring of
8x8 cells, at its integer position (+28/+2C words). Template (A1):
  +0 dx.b, +1 dy.b (signed cells), +2 width.b, +3 height.b,
  +4 material flag.b (nonzero: set bit 4), +5 unused here,
  +6 protection mask.l (an existing nonzero shape s survives unless bit s
     is set), +A width*height shape nibbles, row-major, top row first.
Columns outside [(-64-layer.x)>>3, +64) and rows outside the vertical ring
window are skipped (0C891C/0C892E); bits 5..7 of the old byte are kept
(0C8976). The routine kicks the watchdog per column (0C8912); that write has
no gameplay effect and is not modelled.
tools/vehicle_wreckage.py remains the verified model of the vehicle wreck;
test_collision_stamp checks that this generic routine reproduces it from the
ROM template 0x029BF4."""
import struct

def template(program,address):
    """Read a template from the fixed program ROM block (byte-swapped P1)."""
    dx,dy,w,h,flag=struct.unpack_from('>bbBBB',program,address)
    mask=struct.unpack_from('>I',program,address+6)[0]
    cells=list(program[address+10:address+10+w*h])
    assert all(c<16 for c in cells),('Template cell >= 0x10 traps in 0C8964',hex(address))
    return dict(address=address,dx=dx,dy=dy,width=w,height=h,material=bool(flag),mask=mask,cells=cells)

def stamp(ram,layer,t,x,y):
    """Apply template `t` for an actor at integer position (x, y) whose parent
    is `layer` (0x10C72E or 0x10D7B6). Returns (new RAM, ordered writes)."""
    assert len(ram)==65536 and layer in (0x10c72e,0x10d7b6)
    result=bytearray(ram);writes=[];base=layer-0x100000
    s16=lambda v:((v+0x8000)&0xffff)-0x8000
    lx=struct.unpack_from('>h',ram,base+0x14)[0];ly=struct.unpack_from('>h',ram,base+0x18)[0]
    d2=s16(-64-lx)>>3;d3=s16(128-ly)>>3
    d0=(s16(x)>>3)+t['dx'];d1=(s16(y)>>3)+t['dy']
    d1=-(d1+t['height']);d3=-(d3+0x3f)
    for col in range(t['width']):
        cx=d0+col
        if (cx-d2)&0xffff>=64:continue
        for row in range(t['height']):
            ry=d1+row
            if (ry-d3)&0xffff>=64:continue
            address=base+0x88+((cx&63)<<6)+(ry&63)
            old=result[address];shape=old&15
            if shape and not (t['mask']>>shape)&1:continue
            new=((t['cells'][row*t['width']+col]|(16 if t['material'] else 0))+(old&0xe0))&255
            result[address]=new
            writes.append(dict(address=0x100000+address,old=old,new=new,world_x=cx*8,world_y=(-ry-1)*8))
    return result,writes
