"""Decode measured background transforms, including culled SCBs and camera shake.

The 65..104 sprite bank is used by moving enemies later in mission 1. It is
not a background layer. Source background tiles belong to C-ROM bank 0x40000.
"""
from collections import Counter
import struct

BASES=(1,33,33,105)
OBJECTS=(0xe83e,0xd7b6,0xd7b6,0xc72e)
ORIGIN_Y=(264,264,256,256)
Y_PHASE=(8,8,0,0)

def unwrap(value,period,hint):
    return value+round((hint-value)/period)*period

def columns(vram):
    x=y=rows=zx=zy=0
    for slot in range(1,381):
        c,z=vram[0x8200+slot],vram[0x8000+slot]
        if c&64:x=(x+zx+1)&511;zx=z>>8&15
        else:x=vram[0x8400+slot]>>7;y=(512-(c>>7))&511;rows=c&63;zx=z>>8&15;zy=z&255
        if rows:yield dict(slot=slot,x=x,y=y,rows=min(rows,32),zx=zx,zy=zy)

def trace_hints(trace,frame,lags=range(4)):
    """Per-layer (x, y) transforms read from a per-frame RAM trace (columns of
    tools/capture_mission_rest.lua) for `frame` and the previous frames. SCBs
    show the transform of the current or one of the previous updates."""
    out=[set() for _ in range(4)]
    names=(('sky_x','sky_y'),('mid_x','mid_y'),('mid_x','mid_y'),('fore_x','fore_y'))
    for k in lags:
        row=trace.get(frame-k)
        if row is None:continue
        for layer,(nx,ny) in enumerate(names):
            out[layer].add((-int(row[nx]),ORIGIN_Y[layer]-int(row[ny])))
    return out

LAYER_SLOTS=32

def ram_slots(ram):
    """SCB slot ranges of the background layer objects read from RAM: the
    word +0x2C of each layer object is its first SCB slot minus one (0xFFFF:
    the object draws nothing). Mission 1 gives 1, 33 and 105, the fixed
    ranges of the default classification. Returns {logical layer: first slot}
    with the logical layers of `state` (0 = 0x10E83E, 1/2 = 0x10D7B6 at
    reduced/full speed, 3 = 0x10C72E). 0x10E8C6 must be inactive."""
    fore=struct.unpack_from('>2i',ram,0xc72e+0x14)
    mid=struct.unpack_from('>2i',ram,0xd7b6+0x14)
    full_speed=fore==mid
    out={}
    for layer,address in ((0,0xe83e),(2 if full_speed else 1,0xd7b6),(3,0xc72e)):
        base=struct.unpack_from('>H',ram,address+0x2c)[0]
        if base!=0xffff:out[layer]=base+1
    assert struct.unpack_from('>H',ram,0xe8c6+0x2c)[0]==0xffff,'Fourth layer object 0x10E8C6 is drawing'
    return out

def state(vram,ram,hints=None,slots=None,banks=(4,)):
    """`hints` (optional, from trace_hints) resolves the 16 px vertical
    wrap during fast screen shakes: the unwrapped transform must then equal
    one of the traced RAM values exactly. `slots` (optional, from ram_slots)
    replaces the fixed Mission 1 slot ranges; `banks` are the C-ROM banks
    (tile code bits 16..19) that count as background tiles."""
    fore=struct.unpack_from('>2i',ram,0xc72e+0x14)
    mid=struct.unpack_from('>2i',ram,0xd7b6+0x14)
    full_speed=fore==mid
    all_columns=list(columns(vram));background=[];groups=[[] for _ in range(4)]
    for col in all_columns:
        slot=col['slot']
        if slots is not None:
            layer=next((l for l,b in slots.items() if b<=slot<b+LAYER_SLOTS),None)
            if layer is None:continue
        elif slot<=32:layer=0
        elif slot<=64:layer=2 if full_speed else 1
        elif 105<=slot<=136:layer=3
        else:continue
        if not any((vram[slot*64+row*2+1]&0xf0)>>4 in banks for row in range(col['rows'])):continue
        assert col['zx']==15 and col['zy']==255,('Background zoom',slot)
        col=dict(col,layer=layer);background.append(col)
        if col['x']<320 or col['x']>=496:groups[layer].append(col)
    offsets=[];vertical=[]
    for layer in range(4):
        address=OBJECTS[layer]
        hint_x=-struct.unpack_from('>h',ram,address+0x84)[0]
        hint_y=ORIGIN_Y[layer]-struct.unpack_from('>h',ram,address+0x86)[0]
        group=groups[layer]
        if not group:
            offsets.append(hint_x);vertical.append(hint_y);continue
        base=slots[layer] if slots is not None else BASES[layer]
        mods=Counter(((c['slot']-base)*16-c['x'])&511 for c in group)
        assert len(mods)==1,('Inconsistent background X',layer,mods)
        offset=unwrap(next(iter(mods)),512,hint_x)
        ymods=Counter((c['y']-16-Y_PHASE[layer])&15 for c in group)
        assert len(ymods)==1,('Inconsistent background Y',layer,ymods)
        y_offset=unwrap(next(iter(ymods)),16,hint_y)
        if hints is not None:
            exact={(x,y) for x,y in hints[layer] if (x-offset)%512==0 and (y-y_offset)%16==0}
            assert len(exact)==1,('Transform not in the RAM trace',layer,offset,y_offset,sorted(hints[layer]))
            offset,y_offset=next(iter(exact))
        assert abs(offset-hint_x)<=32 and abs(y_offset-hint_y)<=16,('Ambiguous frame transform',layer,offset,hint_x,y_offset,hint_y)
        if hints is None:assert abs(y_offset-hint_y)<8,('Ambiguous frame transform',layer,offset,hint_x,y_offset,hint_y)
        offsets.append(offset);vertical.append(y_offset)
    # The first foreground column can be culled. Use all surviving columns;
    # when that whole bank is empty, the new full-speed bank provides the camera.
    if groups[3]:camera=offsets[3]
    elif full_speed and groups[2]:camera=offsets[2]
    elif slots is not None and full_speed:camera=offsets[3]  # Layer positions are equal (RAM).
    else:raise ValueError('No unambiguous full-speed background camera')
    return dict(camera=camera,offsets=offsets,vertical_offsets=vertical,
                full_speed_mid=full_speed,active=[bool(g) for g in groups],columns=background)
