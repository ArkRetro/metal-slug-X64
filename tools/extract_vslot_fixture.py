#!/usr/bin/env python3
"""Vehicle-slot fixture (docs/ARMAS_VEHICULOS.md §7) from the weapons family
traces (camelfam, tankfam: tools/capture_weapons.lua, boss-trace.txt).

Every call into the slot services 0AA458..0AABF2 made during a traced update
(the vehicles 03458E/092A32, the vulcan turret 03B57C and the player 1014C4)
is kept in order: frame, node, the node's routine, the service (by the entry
range of its first access), the caller (the return address the JSR pushed)
and every read and write the service made (address, mask, data, pc).
The accesses of the helpers a service calls (0040AC, 018F16/018F24, 019424,
0C0556, 0029C6) are kept with it; other subroutines (0143EE, 01ABC8, 004C24)
are not slot code: their accesses are dropped and the service's accesses
after them continue the same call. The update's node bytes before it ran
(`pre`) give the callers' arguments (the SV-001's shells +7B, type +7C)."""
import argparse,gzip,json,re,zipfile
from pathlib import Path
SERVICES=[(0xaa458,'accept'),(0xaa4c8,'release'),(0xaa4f6,'reset_all'),(0xaa50c,'reset'),(0xaa550,'follow'),(0xaa5ac,'exit_read'),
 (0xaa606,'select_a'),(0xaa610,'select'),(0xaa760,'bind'),(0xaa7fc,'exit'),(0xaa83a,'mount'),(0xaa898,'bombs'),(0xaa956,'shell'),
 (0xaa994,'damage_flag'),(0xaa9e4,'damage'),(0xaaa88,'ridden'),(0xaaae0,'gauge'),(0xaab66,'which'),(0xaab84,'rider'),(0xaabdc,'angle'),(0xaabf4,None)]
def service(pc):
    for (lo,name),(hi,_) in zip(SERVICES,SERVICES[1:]):
        if lo<=pc<hi:return name
    return None
HELPERS=[(0xc0556,0xc05b8),(0x18f16,0x18f40),(0x19424,0x19432),(0x40ac,0x40bc),(0x29c6,0x29f2)]
def helper(pc):return any(lo<=pc<hi for lo,hi in HELPERS)
def site_args(p1,site):
    """D1..D4 the caller set before the JSR at `site`: immediates, or a byte
    field of the caller's node (+7B/+7C), from the instructions before it."""
    import capstone
    md=capstone.Cs(capstone.CS_ARCH_M68K,capstone.CS_MODE_BIG_ENDIAN|capstone.CS_MODE_M68K_000)
    for back in range(24,0,-2):
        ins=list(md.disasm(p1[site-back:site+6],site-back))
        if ins and ins[-1].address==site:break
    args={}
    for i in ins[:-1]:
        m=re.match(r'(?:moveq|move\.[bw]) #\$(-?[0-9a-f]+), d([1-4])$','%s %s'%(i.mnemonic,i.op_str))
        if m:args['d'+m.group(2)]=int(m.group(1),16);continue
        m=re.match(r'move\.b \$([0-9a-f]+)\(a6\), d([1-4])$','%s %s'%(i.mnemonic,i.op_str))
        if m:args['d'+m.group(2)]='+'+m.group(1)
    return args
def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('trace',nargs='+',type=Path,help='LABEL=boss-trace.txt');p.add_argument('--output',type=Path,required=True)
    p.add_argument('--routing',action='append',default=[],type=Path,help='weapons.csv of a run: stable input samples for 0CA1B8')
    p.add_argument('--rom',type=Path,default=Path('/home/user/roms/mslugx.zip'))
    a=p.parse_args();out=[]
    raw=zipfile.ZipFile(a.rom).read('250-p1.p1');p1=bytearray(len(raw));p1[0::2]=raw[1::2];p1[1::2]=raw[0::2]
    for spec in a.trace:
        label,path=str(spec).split('=',1)
        frame=0;node=None;routine=None;pre=None;cur=None;last=None;open_=False;site=None
        for line in open(path):
            t=line.split()
            if not t:continue
            if t[0]=='F':frame=int(t[1]);continue
            if t[0]=='U':node=t[1];routine=int(t[2],16);pre=t[3];cur=None;last=None;open_=False;continue
            if t[0]=='V':cur=None;last=None;open_=False;continue
            if t[0] not in 'RW' or node is None:continue
            ad,mask,data,pc=int(t[1],16),int(t[2],16),int(t[3],16),int(t[4],16)
            s=service(pc)
            if t[0]=='W' and 0x10f002<=ad<0x10f300 and s is None and not helper(pc):site=pc
            if s is None:
                if helper(pc) and open_:cur['acc'].append([t[0],ad,mask,data,pc])
                else:open_=False
                continue
            # A service's accesses after an outside subroutine continue the
            # same call when execution moved forward.
            if cur is not None and cur['service']==s and (open_ or pc>cur['acc'][-1][4]):pass
            else:
                cur=dict(source=label,frame=frame,node=node,routine=routine,pre=pre,service=s,site=site,args=site_args(p1,site) if site else {},acc=[]);out.append(cur)
            open_=True
            cur['acc'].append([t[0],ad,mask,data,pc])
        print(label,'calls',len([c for c in out if c['source']==label]))
    # 0CA1B8 samples: MAME P1 inputs unchanged over the six frames before
    # (the game samples them on its own update, a few frames later) and the
    # held word 10E9EC+2 unchanged over two frames, nonzero (zero while the
    # input is locked by the script).
    import csv
    routing=[]
    for path in a.routing:
        rows=list(csv.DictReader(open(path)))
        for i in range(8,len(rows)):
            L={rows[j]['inputs'] for j in range(i-6,i)};W={int(rows[j]['input_word'],16)&0xffff for j in range(i-1,i+1)}
            if len(L)==1 and len(W)==1 and min(W):routing.append([str(path.parent.name),int(rows[i]['frame']),L.pop(),W.pop()])
        # A+B pressed together (bit 7): the first update that saw it, with
        # the inputs of the last frame before it that held both.
        for i,r in enumerate(rows):
            if int(r['input_word'],16)&0x80 and not int(rows[i-1]['input_word'],16)&0x80:
                src=[x for x in rows[i-4:i+1] if 'A' in x['inputs'] and 'B' in x['inputs']][-1]
                routing.append([str(path.parent.name),int(r['frame']),src['inputs'],int(r['input_word'],16)&0xffff])
    with gzip.open(a.output,'wt') as f:json.dump(dict(routing=routing,source='MAME 0.264 mslugx euro weapons family traces '+' '.join(map(str,a.trace)),calls=out),f,separators=(',',':'))
    import collections
    print(collections.Counter(c['service'] for c in out))
if __name__=='__main__':main()
