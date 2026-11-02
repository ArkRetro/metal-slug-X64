#!/usr/bin/env python3
"""Fixture of the parachute landing actors (src/neo_landing.c) from a family
trace of tools/capture_mission_rest.lua (MSX_REST_FAMILY_ROOTS=c4a58,c5286,
docs/MISION1_COMPLETA.md "Aterrizaje"), and its C header (--header).

From the landing (the canopy's first update that leaves 0C4AAE, the
marker's constructor 0C5286) to their deletion: per update the node, the
routine, the bytes before and after (none: deleted without 00424A) and what
the update read of the player 1014C4: +15, +1E, +3C..+4D, +70 (first value
of each byte).

The same with the mummy transformation's smoke 0C4DD8 (kind 2, from its
constructor) when the trace has it (tools/capture_fullgame.lua
MSX_FULL_FAMILY_ROOTS=c4dd8,c5286 MSX_FULL_GIVE_MUMMY, docs/MARCO_CAMPAIGN.md
"Forma 3"); the header's second field is the kind (0 marker, 1 canopy,
2 smoke)."""
import argparse,gzip,json
from pathlib import Path
P=0x1014c4
FIELDS=[0x15,0x1e,0x1f]+list(range(0x3c,0x4e))+[0x70,0x71]
def main():
    ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('trace',type=Path,nargs='?');ap.add_argument('--from-json',type=Path,help='only write the header of an extracted fixture');ap.add_argument('--output',type=Path,required=True)
    ap.add_argument('--header',type=Path);a=ap.parse_args()
    if a.from_json:
        ups=json.load(gzip.open(a.from_json))['updates'];write_header(a,ups);return
    ups=[];cur=None;frame=0;started={}
    def close(post):
        nonlocal cur
        if cur:cur['post']=post;ups.append(cur)
        cur=None
    for line in open(a.trace):
        t=line.split()
        if not t:continue
        if t[0]=='F':frame=int(t[1]);continue
        if t[0]=='U':
            close(None)
            n,r=t[1],int(t[2],16)
            if r==0x3f1e:continue
            pre=bytes.fromhex(t[3])
            # the canopy from its landing update on (0C4AAE with the player grounded), the marker from 0C5286
            if r==0xc5286:started[n]='marker'   # a node reused after the smoke's deletion
            if r==0xc4dd8:started[n]='smoke'
            cur=dict(frame=frame+1,node=n,routine=r,pre=t[3],ram={},kind=started.get(n)) if (n in started or r==0xc4aae) else None
        elif t[0]=='V':
            post=t[2]
            if cur and cur['node'] not in started:
                if int(post[:8],16)==0xc4aae:cur=None;continue   # still descending
                started[cur['node']]='canopy';cur['kind']='canopy'
            close(post)
        elif t[0]=='R' and cur:
            ad,m,d=int(t[1],16),int(t[2],16),int(t[3],16)
            for b,bm,sh in ((0,0xff00,8),(1,0xff,0)):
                o=ad+b-P
                if m&bm and o in FIELDS and str(o) not in cur['ram']:cur['ram'][str(o)]=(d>>sh)&0xff
                if m&bm and ad+b==0x10c33a and 'epoch' not in cur['ram']:cur['ram']['epoch']=(d>>sh)&0xff
            # 012022: the words RAM 10B954 / 10B95C (+15 bit 2, the front layer).
            for k,w in (('l954',0x10b954),('l95c',0x10b95c)):
                if ad==w and m==0xffff and k not in cur['ram']:cur['ram'][k]=d&0xffff
    close(None)
    ups=[u for u in ups if u['node'] in started]
    for u in ups:u['kind']=u.get('kind') or started[u['node']]
    with gzip.open(a.output,'wt') as f:json.dump(dict(source=str(a.trace),updates=ups),f,separators=(',',':'))
    print(len(ups),'updates',{k:sum(1 for u in ups if u['kind']==k) for k in ('canopy','marker','smoke')})
    write_header(a,ups)
def write_header(a,ups):
    if a.header:
        c=lambda h:'{'+','.join('0x'+h[i:i+2] for i in range(0,len(h),2))+'}'
        o=['/* %s: %d updates (tools/extract_landing_fixture.py) */'%(a.output.name,len(ups)),'static const landing_update_t landing_updates[]={']
        for u in ups:
            ram=','.join(str(u['ram'].get(str(f),-1)) for f in FIELDS)+','+str(u['ram'].get('epoch',-1))+','+str(u['ram'].get('l954',-1))+','+str(u['ram'].get('l95c',-1))
            o.append(' {%d,%d,0x%x,%s,%d,%s,{%s}},'%(u['frame'],('marker','canopy','smoke').index(u['kind']),u['routine'],c(u['pre']),1 if u['post'] else 0,c(u['post'] or '00'*0xb0),ram))
        o.append('};');o.append('static const unsigned landing_fields[]={%s};'%','.join(hex(f) for f in FIELDS))
        a.header.write_text('\n'.join(o)+'\n')
if __name__=='__main__':main()
