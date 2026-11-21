#!/usr/bin/env python3
"""Camera closed-loop fixture from a MAME capture made with
MSX_REST_CAMERA=1 tools/run_capture_rest.sh (camera.csv: one line per camera
update, written by a tap on the last store of ROM 0C7CA6).

Each record keeps the inputs ROM 018D96/01903A read (player 1 projected words
and follow bytes, RAM 10C322) and the camera/map block RAM 10E94E..10E9DB
after the camera update. The fixture covers the Mission 1 mission actor
(routine 0186A0) from its first update to the mission end."""
import argparse,csv,gzip,json
from pathlib import Path
FIELDS={ # name: (offset in the block from RAM 10E94E, bytes, signed)
 'layer_flags':(0x08,4,False),'auto_mode':(0x0c,1,False),'auto_owner':(0x0d,1,False),'auto_vx':(0x0e,4,True),'auto_vy':(0x12,4,True),
 'command':(0x16,4,False),'root_vx':(0x22,4,True),'root_vy':(0x26,4,True),'root_x':(0x2e,4,True),'root_y':(0x32,4,True),
 'x':(0x40,4,True),'y':(0x44,4,True),'dx':(0x48,4,True),'dy':(0x4c,4,True),'progress':(0x50,4,False),'target':(0x54,4,False),
 'path_x':(0x58,4,True),'path_y':(0x5c,4,True),'bounds':(0x60,8,False),'mode':(0x68,2,False),'p69b8':(0x6a,2,False),'p69ba':(0x6c,2,False),
 'lock':(0x6e,4,False),'lock_bounds':(0x72,8,False),'shake':(0x7a,4,True),'shake_applied':(0x7e,4,True),'restart':(0x82,2,False),'view':(0x84,8,False)}
def field(b,name):
    o,n,s=FIELDS[name]
    if name in('bounds','lock_bounds','view'):return [int.from_bytes(b[o+2*i:o+2*i+2],'big',signed=True) for i in range(4)]
    return int.from_bytes(b[o:o+n],'big',signed=s)
def player(text):
    routine,words,follow,flags=text.split(':')
    w=bytes.fromhex(words);routine=int(routine,16)
    return [1 if routine not in (0x3f1e,0x55555555,0xaaaaaaaa) and routine<0x1000000 else 0,int(follow[:2],16),int(follow[2:],16),
            int.from_bytes(w[0:2],'big'),int.from_bytes(w[4:6],'big'),int(flags[:2],16),int(flags[2:],16)]   # ..., +5F, +15
def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('capture',type=Path);p.add_argument('--output',type=Path,required=True)
    p.add_argument('--list-base',type=lambda v:int(v,16),default=0x0B57BE,help='camera list of the stage (Mission 4: e741a, capture of tools/capture_fullgame.lua MSX_FULL_CAMERA=1)')
    p.add_argument('--list-size',type=lambda v:int(v,16),default=0x200)
    a=p.parse_args();records=[];list_base=a.list_base
    for r in csv.DictReader(open(a.capture/'camera.csv')):
        mission=r['mission'].split(':')
        if int(mission[0],16)!=0x0186a0:
            if records:break
            continue
        assert mission[1]=='000a0000000a0000' or 'layers' in r,'mission actor +72/+76'
        flags=bytes.fromhex(r['flags']);assert not flags[1]&0x18,'10F000 bits 3/4 (debug camera) set'
        b=bytearray(bytes.fromhex(r['block']));b[0x8a:0x8c]=int(r['d8'],16).to_bytes(2,'big')
        rec=dict(frame=int(r['frame']),players=[player(r['p1']),player(r['p2'])],follow=1 if flags[0] else 0)
        for k in FIELDS:rec[k]=field(b,k)
        rec['command']-=list_base;assert 0<=rec['command']<a.list_size,hex(rec['command'])
        if r.get('layers'):
            rec['speed']=[int(mission[1][:8],16),int(mission[1][8:],16)]   # mission actor +72/+76 (Mission 4: 0x8000 from the boss call)
            # Layer objects 10C72E / 10D7B6 / 10E83E / 10E8C6: +0 x, +4 y, +26 / +28 factors, +2A / +2B remainders.
            rec['layers']=[[int.from_bytes(h[0:4],'big',signed=True),int.from_bytes(h[4:8],'big',signed=True),int.from_bytes(h[0x26:0x28],'big',signed=True),
                            int.from_bytes(h[0x28:0x2a],'big',signed=True),h[0x2a],h[0x2b]] for h in map(bytes.fromhex,r['layers'].split(':'))]
        records.append(rec)
    out=dict(source=f'MAME 0.264 mslugx euro, tools/capture_mission_rest.lua MSX_REST_CAMERA=1 or tools/capture_fullgame.lua MSX_FULL_CAMERA=1 ({a.capture.name})',list_base=hex(list_base),records=records)
    with gzip.open(a.output,'wt') as f:json.dump(out,f,separators=(',',':'))
    print(len(records),'camera updates',records[0]['frame'],'..',records[-1]['frame'])
if __name__=='__main__':main()
