#!/usr/bin/env python3
"""Fixture of the parachute descent (src/marco_descent.c, src/neo_landing.c)
from family traces of tools/capture_mission_rest.lua (MSX_REST_FAMILY_ROOTS=
c15ba,c162a,c170c,c4a58,c5286, MSX_REST_INPUTS; docs/MISSION_ENTRY.md
"Descenso nativo"): ch0 without input, chA with A, C and A (cut while low),
chB with B (cut).

Player 1014C4 updates in 0C162A/0C170C: the bytes before and after, the
random word of 0029C6 (read at 0029D2), the input long 0C0FEA read
(10E9EC, at 0C102C) and the gate bytes 10C718/10C71A/10C71C. Canopy updates
(from its constructor 0C4A58 to its deletion): bytes before and after and
what it read of the player (+15, +1E, +3C..+4D, +5F, +61, +70) and of the
scene root 10E968 (+14..+25, for 01AD52)."""
import argparse,gzip,json
from pathlib import Path
P=0x1014c4;ROOT=0x10e968
PFIELDS=[0x15,0x1e,0x1f]+list(range(0x3c,0x4e))+[0x5f,0x61,0x70,0x71]
RFIELDS=list(range(0x14,0x26))
def updates(trace,name):
    out=[];cur=None;frame=0;canopy=None
    def close(post):
        nonlocal cur
        if cur is not None:cur['post']=post;out.append(cur)
        cur=None
    for line in open(trace):
        t=line.split()
        if not t:continue
        if t[0]=='F':frame=int(t[1]);continue
        if t[0]=='B' and t[2]=='1014c4' and canopy is None:canopy=t[1];continue
        if t[0]=='U':
            close(None);n,r=t[1],int(t[2],16)
            if n=='1014c4' and r in (0xc162a,0xc170c):cur=dict(capture=name,frame=frame+1,kind='player',routine=r,pre=t[3],random=-1,packed=[-1,-1],gate=[-1,-1,-1])
            elif n==canopy and r!=0x3f1e:cur=dict(capture=name,frame=frame+1,kind='canopy',routine=r,pre=t[3],ram={})
        elif t[0]=='V':close(t[2])
        elif t[0]=='R' and cur is not None:
            ad,m,d,pc=(int(x,16) for x in t[1:5])
            if cur['kind']=='player':
                if pc==0x29d2 and cur['random']<0:cur['random']=d&0xffff
                if pc==0xc102c and ad in (0x10e9ec,0x10e9ee):cur["packed"][(ad-0x10e9ec)//2]=d&0xffff
                for i,g in enumerate((0x10c718,0x10c71a,0x10c71c)):
                    if ad==g and m&0xff00 and cur['gate'][i]<0:cur['gate'][i]=(d>>8)&0xff
            else:
                for b,bm,sh in ((0,0xff00,8),(1,0xff,0)):
                    if not m&bm:continue
                    for base,fields,key in ((P,PFIELDS,'p'),(ROOT,RFIELDS,'r')):
                        o=ad+b-base
                        if o in fields and key+str(o) not in cur['ram']:cur['ram'][key+str(o)]=(d>>sh)&0xff
                    if ad+b==0x10c33a and 'epoch' not in cur['ram']:cur['ram']['epoch']=(d>>sh)&0xff
    close(None)
    return out
def main():
    ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('captures',type=Path,nargs='*')
    ap.add_argument('--from-json',type=Path);ap.add_argument('--output',type=Path,required=True);ap.add_argument('--header',type=Path);a=ap.parse_args()
    if a.from_json:ups=json.load(gzip.open(a.from_json))['updates']
    else:
        ups=[]
        for c in a.captures:ups+=updates(c/'boss-trace.txt',c.name)
        with gzip.open(a.output,'wt') as f:json.dump(dict(source=[str(c) for c in a.captures],updates=ups),f,separators=(',',':'))
        print(len(ups),'updates',{k:sum(1 for u in ups if u['kind']==k) for k in ('player','canopy')})
    if a.header:
        c=lambda h:'{'+','.join('0x'+h[i:i+2] for i in range(0,len(h),2))+'}'
        caps=sorted({u['capture'] for u in ups})
        o=['/* %s: %d updates (tools/extract_descent_fixture.py) */'%(a.output.name,len(ups)),'static const descent_update_t descent_updates[]={']
        for u in ups:
            if u['kind']=='player':extra='%d,%d,%d,{%s},{%s}'%(u['random'],u['packed'][0],u['packed'][1],','.join(map(str,u['gate'])),','.join(['-1']*(len(PFIELDS)+len(RFIELDS)+1)))
            else:extra='-1,-1,-1,{-1,-1,-1},{%s}'%','.join([str(u['ram'].get('p%d'%f,-1)) for f in PFIELDS]+[str(u['ram'].get('r%d'%f,-1)) for f in RFIELDS]+[str(u['ram'].get('epoch',-1))])
            o.append(' {%d,%d,%d,0x%x,%s,%d,%s,%s},'%(caps.index(u['capture']),u['frame'],u['kind']=='canopy',u['routine'],c(u['pre']),1 if u['post'] else 0,c(u['post'] or '00'*0xb0),extra))
        o.append('};')
        o.append('static const unsigned descent_pfields[]={%s};'%','.join(hex(f) for f in PFIELDS))
        o.append('static const unsigned descent_rfields[]={%s};'%','.join(hex(f) for f in RFIELDS))
        o.append('static const char *const descent_captures[]={%s};'%','.join('"%s"'%x for x in caps))
        a.header.write_text('\n'.join(o)+'\n')
if __name__=='__main__':main()
