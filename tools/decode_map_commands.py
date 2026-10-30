#!/usr/bin/env python3
"""Decode the camera/map command list of a mission (fixed program ROM) and
check the rest-of-Mission-1 map events against the ROM.

Interpreter ROM 0C83E6 (pointer RAM 10E964): records [op.w][length.w][...];
ops 0, 4, 8 block (ROM 0C84A6 wait for progress >= word, 0C84BE wait for a
player inside a rectangle, 0C855C hold with a final progress limit); the
others run immediately (dispatch table ROM 0C844A). Names describe the
handler, not a guessed purpose.

`--map` additionally checks every recorded coordinate variant of a map
extracted by tools/extract_mission_map.py against the destroyed-building
sub-map that ROM 0B6882 copies with 0C70FC (descriptor 0B6AD2, source
column 3, 8x12 cells, destination map column 0x9F, row 0x0B, layers
10D7B6/10E8C6): the variant observed later must be that ROM cell."""
import argparse,json,struct
from pathlib import Path
from extract_region_collision import program_rom
import zipfile

OPS={0x00:'wait_progress',0x04:'wait_player_in_rect',0x08:'hold_progress_limit',0x0c:'call_009D8C_palette',
     0x10:'set_10E9B6',0x14:'rebase_all_layers',0x18:'load_layer_map',0x1c:'layer_attributes',0x20:'layer_reload_0C741A',
     0x24:'layer_0C6CFC',0x28:'layer_0C6D04',0x2c:'call',0x30:'set_camera_bounds_10E9AE',0x34:'set_10E9B8',
     0x38:'call_002802',0x3c:'set_10E956_layer_flags',0x40:'set_10E95A',0x44:'rebase_layer',0x48:'patch_layer_0C73CE',
     0x4c:'call_01AA28',0x50:'set_10C00E',0x54:'call_if_10E9D0_ge_10E9A2',0x58:'call_if_10E9D0_lt_10E9A2'}

def commands(prog,address):
    out=[]
    while True:
        op,n=struct.unpack_from('>HH',prog,address)
        assert op in OPS,('Unknown map command',hex(address),op)
        # Op 8 (0C855C) never advances: its record has length 0 and the
        # progress limit at +4.
        words=list(struct.unpack_from(f'>{max(0,(n-4)//2)}H',prog,address+4)) if n>4 else [struct.unpack_from('>H',prog,address+4)[0]] if op==0x08 else []
        out.append(dict(address=f'0x{address:06X}',op=op,name=OPS[op],length=n,words=words))
        if op==0x08:break
        address+=n
    return out

def check_building_patch(zip_path,model):
    with zipfile.ZipFile(zip_path) as z:raw=z.read('250-p2.ep1')
    p2=bytearray(raw);p2[0::2]=raw[1::2];p2[1::2]=raw[0::2]
    prog=program_rom(zip_path)
    pointer,=struct.unpack_from('>I',prog,0x0B6AD2);height,=struct.unpack_from('>H',prog,0x0B6AD2+10)
    # 0C71CC..0C71EA: the high word rotated right by 4 gives bank+1 in its
    # low byte (written to 0x2FFFF0); the CPU address is rebuilt with low byte
    # 2 and rotated back, inside the window 0x200000..0x2FFFFF.
    w=pointer>>16;r=((w>>4)|(w<<12))&0xffff;bank=(r&0xff)-1
    r=(r&0xff00)|2;address=((((r<<4)|(r>>12))&0xffff)<<16)|(pointer&0xffff)
    assert 0x200000<=address<0x300000;base=bank*0x100000+address-0x200000
    first={(c['layer'],c['x'],c['y']):c['code'] for c in model['cells']}
    checked=0
    for c in model['conflicts']:
        col=c['x']//16-0x9f+3;row=c['y']//16-3   # Destination column 0x9F <- source column 3; map y 48 is the first row.
        assert c['layer']==2 and 3<=col<11 and 0<=row<12,('Variant outside the destroyed-building patch',c['layer'],c['x'],c['y'])
        tile,attr=struct.unpack_from('>HH',p2,base+(col*height+row)*4)
        code=tile|(attr<<12)&0xf0000
        assert any(ch['code']==code for ch in c['choices']),('Variant without the ROM patch tile',c['x'],c['y'],hex(code))
        checked+=1
    return dict(sub_map=f'0x{pointer:08X}',bank=bank,cpu_address=f'0x{address:06X}',variant_cells=checked)

if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('zip',type=Path);p.add_argument('--address',type=lambda v:int(v,0),default=0x0B57BE)
    p.add_argument('--map',type=Path);p.add_argument('--output',type=Path)
    a=p.parse_args();prog=program_rom(a.zip)
    report=dict(source='Fixed program ROM of the user MAME set; interpreter ROM 0C83E6',list=f'0x{a.address:06X}',commands=commands(prog,a.address))
    if a.map:report['building_patch']=check_building_patch(a.zip,json.loads(a.map.read_text()))
    if a.output:a.output.write_text(json.dumps(report,indent=1)+'\n')
    for c in report['commands']:print(c['address'],c['name'],c['words'])
    if a.map:print(report['building_patch'])
