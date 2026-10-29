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

def state(vram,ram):
    fore=struct.unpack_from('>2i',ram,0xc72e+0x14)
    mid=struct.unpack_from('>2i',ram,0xd7b6+0x14)
    full_speed=fore==mid
    all_columns=list(columns(vram));background=[];groups=[[] for _ in range(4)]
    for col in all_columns:
        slot=col['slot']
        if slot<=32:layer=0
        elif slot<=64:layer=2 if full_speed else 1
        elif 105<=slot<=136:layer=3
        else:continue
        if not any((vram[slot*64+row*2+1]&0xf0)==0x40 for row in range(col['rows'])):continue
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
        mods=Counter(((c['slot']-BASES[layer])*16-c['x'])&511 for c in group)
        assert len(mods)==1,('Inconsistent background X',layer,mods)
        offset=unwrap(next(iter(mods)),512,hint_x)
        ymods=Counter((c['y']-16-Y_PHASE[layer])&15 for c in group)
        assert len(ymods)==1,('Inconsistent background Y',layer,ymods)
        y_offset=unwrap(next(iter(ymods)),16,hint_y)
        assert abs(offset-hint_x)<=32 and abs(y_offset-hint_y)<8,('Ambiguous frame transform',layer,offset,hint_x,y_offset,hint_y)
        offsets.append(offset);vertical.append(y_offset)
    # The first foreground column can be culled. Use all surviving columns;
    # when that whole bank is empty, the new full-speed bank provides the camera.
    if groups[3]:camera=offsets[3]
    elif full_speed and groups[2]:camera=offsets[2]
    else:raise ValueError('No unambiguous full-speed background camera')
    return dict(camera=camera,offsets=offsets,vertical_offsets=vertical,
                full_speed_mid=full_speed,active=[bool(g) for g in groups],columns=background)
