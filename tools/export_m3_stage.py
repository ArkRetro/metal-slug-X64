#!/usr/bin/env python3
"""Mission 3 stage record, descriptor, camera list, layer rules and
sections (docs/MISION3.md, "Reconocimiento del mapa"), private output
filesystem/mission3/s1/stage.json.

* Flow record 0173F6 -> descriptor 016E0E (+0 camera list, +4 player
  records, +8 script table word, +C music/stage words, +10, +14).
* Camera list 0B8968 decoded as src/neo_camera.c walks it (op, length, the
  words; names of neo_camera.h), to its hold (op 8).
* The layer rules and grid of romfs/mission3.pack's stage record
  (tools/pack_mission.py), the sections of tools/extract_m3_recon.py."""
import argparse,gzip,json,struct,sys
from pathlib import Path
sys.path.insert(0,str(Path(__file__).resolve().parent))
from decode_level_script import Program
ROOT=Path(__file__).resolve().parents[1]
OPS={0:'wait progress',4:'wait player rectangle (0C84BE)',8:'hold',0xc:'palette (009D8C)',0x10:'camera mode',0x14:'rebase path',
     0x18:'load layer map (object, block column, top block row, map, column table)',0x1c:'layer attributes',0x20:'layer reload',0x24:'layer 24',0x28:'layer 28',
     0x2c:'call',0x30:'bounds',0x34:'player point',0x38:'sound',0x3c:'layer flags',0x40:'auto mode',0x44:'rebase layer',0x48:'patch layer',
     0x4c:'graphics (01AA28)',0x50:'cell group map',0x54:'call if restart >= target',0x58:'call if restart < target'}
def main():
    ap=argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--rom',type=Path,default=Path('/home/user/roms/mslugx.zip'))
    ap.add_argument('--pack',type=Path,default=ROOT/'romfs/mission3.pack')
    ap.add_argument('--fixture',type=Path,default=ROOT/'data/m3_map_fixture.json.gz')
    ap.add_argument('--output',type=Path,default=ROOT/'filesystem/mission3/s1/stage.json')
    a=ap.parse_args();p=Program(a.rom)
    d=0x016E0E
    desc=dict(address='016E0E',camera_list='%06X'%p.u32(d),players='%06X'%p.u32(d+4),script_word='%04X'%p.u32(d+8),
              words=['%08X'%p.u32(d+4*i) for i in range(6)])
    cam=[];at=p.u32(d)
    while True:
        op,n=p.u16(at),p.u16(at+2)
        words=['%04X'%p.u16(at+4+2*i) for i in range(max(0,(n-4)//2))] if op!=8 else ['%04X'%p.u16(at+4)]
        cam.append(dict(at='%06X'%at,op='%02X'%op,name=OPS.get(op,'?'),length=n,words=words))
        if op==8:break
        assert n and n<0x40;at+=n
    pack=a.pack.read_bytes();h=48 if struct.unpack_from('>H',pack,4)[0]>=4 else 40;st=pack[h:h+128]   # pack version 4: 48-byte header
    f=struct.unpack_from('>IHHIIIHHhhIIII',st,0)
    rules=[dict(zip(('x_num','x_den','y_num','y_den','visible_min','visible_max'),struct.unpack_from('>BBBBhh',st,80+8*i))) for i in range(4)]
    sections=json.load(gzip.open(a.fixture))['sections']
    out=dict(source='tools/export_m3_stage.py: ROM (flow record 0173F6, descriptor 016E0E, camera list) and romfs/mission3.pack',
        flow_record='0173F6',descriptor=desc,camera_list=cam,
        pack_stage=dict(width=f[1],camera_max=f[2],grid=[f[6],f[7]],origin=[f[8],f[9]],layer_rules=rules),sections=sections)
    a.output.write_text(json.dumps(out,indent=1)+'\n')
    print('camera list %d commands; layer rules %s; %d sections'%(len(cam),rules,len(sections)))
if __name__=='__main__':main()
