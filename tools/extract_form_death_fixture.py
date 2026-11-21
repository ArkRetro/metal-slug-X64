#!/usr/bin/env python3
"""Deaths of a run_marco_catalog.py capture (actor*.bin of the player's node
1014C4 every frame, debug.log with MARCO_DRAW, palette*.bin of
--dump-routines) in the format of data/death_fixture.json, for the deaths
by form (docs/ANIMACIONES_DANO.md): fat (+93 = 2, 0C2A36) and mummy
(+93 = 3, 0C2C40), and the other characters' deaths (+6E).

Per update (the actor dumps of the update frames, from the one whose
routine is 0C2956 to the entrance's end 0C229C): routine, +15, +30, +34,
+3C, +40, +28, +2C, +38, +26 and the descriptor MARCO_DRAW logs one frame
later. dying_poses / lying_draws as tools/extract_death_fixture.py. The
form +93 and the character +6E of the hit update; palette_table: the ROM
table (0BEDD0..0BEE10, the column +6E) whose palette the palette RAM holds in
the slot +19 one update after the hit (0 when it is none of them).
respawn false: the progress 10E99E is not in these dumps, so the respawn x
is not compared. Capture RAM and the ROM's palettes only (no ROM bytes are
written). Usage: extract_form_death_fixture.py --rom mslugx.zip CAPTURE... --output OUT.json"""
import argparse,json,re,struct,zipfile
from pathlib import Path
DYING=('c2956','c2d6a','c2e8a','c2a48','c2b8a','c2ab2','c2d06')
LYING=('c2f06','c2f12')
TABLES=(0xbedd0,0xbedd8,0xbede0,0xbede8,0xbedf0,0xbedf8,0xbee00,0xbee08,0xbee10)
def swap(b):
    o=bytearray(len(b));o[0::2]=b[1::2];o[1::2]=b[0::2];return bytes(o)
UPD=re.compile(r'MARCO_UPDATE frame=(\d+) r=(\w+) f15=(\w+) v30=(\w+) v34=(\w+) x3c=(\w+) y40=(\w+) x28=(\w+) y2c=(\w+) p38=(\w+) f26=(\w+) f5a=(\w+) f5b=(\w+) f93=(\w+) f6e=(\w+) f19=(\w+)')
def from_updates(d,fixed,bank,w,rom_palette):
    """A capture with --update-trace (MARCO_UPDATE at 00424A, once per game
    update: Mission 6 runs an update in 1..5 frames, so the frame-end dumps
    are not used): the records are the updates; an update's pose is the
    first MARCO_DRAW of the next one (as drawn[g+1] of two-frame updates);
    MARCO_WATER (0C2D28, the surface 03C55C found) gives the update's
    surface D1 + the object's +18. Deaths from a record whose routine is
    0C2956 or 0C2D6A (the fall: 'fall') to 0C229C."""
    recs=[];draws={};surf={}
    for line in (d/'debug.log').read_text(errors='replace').splitlines():
        m=UPD.match(line)
        if m:recs.append([int(m.group(1))]+[int(x,16) for x in m.groups()[1:]]);continue
        m=re.match(r'MARCO_DRAW frame=\d+ descriptor=([0-9A-F]+)',line)
        if m:draws.setdefault(len(recs),int(m.group(1),16));continue
        m=re.match(r'MARCO_WATER frame=\d+ d1=(\w+) a0=\w+ y18=(\w+)',line)
        if m:surf[len(recs)]=(int(m.group(1),16)+int(m.group(2),16))&0xffffffff
    out=[]
    for i,r in enumerate(recs):
        if r[1] not in (0xc2956,0xc2d6a) or (i and recs[i-1][1]==r[1]):continue
        cur=dict(capture=d.name,frame=r[0],progress=0,respawn=False,kind=r[11],f5b=r[12],form=r[13],character=r[14],fall=r[1]==0xc2d6a,updates=[])
        j=i;prev=0
        while j<len(recs):
            q=recs[j];desc=draws.get(j+1,0)
            # surface: -1 none (no MARCO_WATER in that update)
            cur['updates'].append(['%x'%q[1],q[2],q[3],q[4],q[5],q[6],q[7],q[8],'%08x'%q[9],q[10],desc or prev,surf.get(j,-1)])
            prev=desc or prev
            if q[1]==0xc229c:break
            j+=1
        if cur['updates'][-1][0]!='c229c':continue
        cur['water']=len(cur['updates'])>1 and cur['updates'][1][0]=='c2d06'
        cur['dying_poses']=[draws[i+k+1] for k,u in enumerate(cur['updates']) if u[0] in DYING and (i+k+1) in draws]
        cur['lying_draws']=sum(1 for k,u in enumerate(cur['updates']) if u[0] in LYING and (i+k+1) in draws)
        # The palette in slot +19 one update after the hit (the dump after update 1's frame).
        cur['palette_table']=0;pal=d/f'palette{recs[i+1][0]+2}.bin'
        if pal.exists():
            slot=recs[i+1][15];ram=pal.read_bytes();words=[w(ram,slot*32+2*k) for k in range(16)]
            for t in TABLES:
                if rom_palette(w(fixed,t+2*cur['character']))[1:]==words[1:]:cur['palette_table']=t;break
        out.append(cur)
    return out
def main():
    p=argparse.ArgumentParser();p.add_argument('--rom',type=Path,required=True);p.add_argument('--output',type=Path,required=True)
    p.add_argument('captures',type=Path,nargs='+');a=p.parse_args()
    z=zipfile.ZipFile(a.rom);fixed=swap(z.read('250-p1.p1'));bank=swap(z.read('250-p2.ep1'))[0x100000:0x200000]
    w=lambda m,o:m[o]<<8|m[o+1]
    def rom_palette(n):   # 009B84/005978: 16 words at 200000 + 32n, each the doubled index into 224000
        return [0]+[w(bank,0x24000+((2*w(bank,n*32+2*k)+0x8000)&0xffff)-0x8000) for k in range(1,16)]
    deaths=[]
    for d in a.captures:
        if 'MARCO_UPDATE' in (d/'debug.log').read_text(errors='replace'):deaths+=from_updates(d,fixed,bank,w,rom_palette);continue
        drawn={}
        for f,desc in re.findall(r'MARCO_DRAW frame=(\d+) descriptor=([0-9A-F]+)',(d/'debug.log').read_text(errors='replace')):drawn.setdefault(int(f),int(desc,16))
        frames=sorted(int(m.group(1)) for m in (re.match(r'actor(\d+)\.bin$',x.name) for x in d.iterdir()) if m)
        node={f:(d/f'actor{f}.bin').read_bytes() for f in frames}
        L=lambda b,o:int.from_bytes(b[o:o+4],'big')
        rt=lambda f:'%x'%L(node[f],0)
        for i,f in enumerate(frames):
            if rt(f)!='c2956' or (i and rt(frames[i-1])=='c2956'):continue
            b=node[f];cur=dict(capture=d.name,frame=f,progress=0,respawn=False,kind=b[0x5a],f5b=b[0x5b],form=b[0x93],character=w(b,0x6e),updates=[])
            g=f;prev=0
            while g in node:
                n=node[g];desc=drawn.get(g+1,0)
                cur['updates'].append(['%x'%L(n,0),n[0x15],L(n,0x30),L(n,0x34),L(n,0x3c),L(n,0x40),L(n,0x28),L(n,0x2c),'%08x'%L(n,0x38),w(n,0x26),desc or prev])
                prev=desc or prev
                if rt(g)=='c229c':break
                g+=2
            if cur['updates'][-1][0]!='c229c':continue
            cur['dying_poses']=[drawn[f+2*k+1] for k,u in enumerate(cur['updates']) if u[0] in DYING and (f+2*k+1) in drawn]
            cur['lying_draws']=sum(1 for k,u in enumerate(cur['updates']) if u[0] in LYING and (f+2*k+1) in drawn)
            # The palette in slot +19 one update after the hit, by its ROM table.
            pal=d/f'palette{f+3}.bin';cur['palette_table']=0
            if pal.exists():
                slot=node[f+2][0x19];ram=pal.read_bytes();words=[w(ram,slot*32+2*k) for k in range(16)]
                for t in TABLES:
                    if rom_palette(w(fixed,t+2*cur['character']))[1:]==words[1:]:cur['palette_table']=t;break
            deaths.append(cur)
    json.dump(dict(source=' + '.join(c.name for c in a.captures)+' (run_marco_catalog.py --hit / --load --update-trace)',deaths=deaths),open(a.output,'w'),separators=(',',':'))
    for x in deaths:print(x['capture'],x['frame'],'kind',hex(x['kind']),'form',x['form'],'character',x['character'],'updates',len(x['updates']),'poses',len(x['dying_poses']),'lying',x['lying_draws'],'palette',hex(x['palette_table']))
if __name__=='__main__':main()
