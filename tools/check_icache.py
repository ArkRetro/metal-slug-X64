#!/usr/bin/env python3
"""Instruction cache check of the boot logo's per-vertex path (after the link).

The VR4300 instruction cache is 16 KB, direct mapped, 32-byte lines: two
pieces of code whose addresses agree modulo 16 KB evict each other. The logo
(src/neogeo_logo.c) runs project, metal, gradient, draw_cap, draw_sides and libdragon's
rdpq_triangle_rsp, which calls libm's floor and floorf, for every vertex.
With floorf at the start of the program (libm's plain .text) and libdragon
after the game code, the game's size decided whether they shared lines: after
the ccr merge of 12-XI (ROM 2cb2aa0) floorf and rdpq_triangle_rsp shared 9,
the logo took 12 ms of CPU a frame instead of 7 and 211 of its frames were
late (docs/RENDIMIENTO.md, "Caché de instrucciones"). Now the logo and
floor/floorf go after all the code (keep.text), at a fixed distance from
libdragon; this check fails the build if the path shares a line anyway, or
if the per-vertex functions call code outside it.

Usage: check_icache.py ELF MAP"""
import bisect,re,subprocess,sys
from pathlib import Path
B=Path(__import__('os').environ.get('N64_INST','/home/user/toolchains/package/opt/libdragon'))/'bin'/'mips64-elf-'
LINE,SETS=32,512
# (name, object) of the path; None: any object (libdragon / libm).
PATH=[('project','neogeo_logo.o'),('metal','neogeo_logo.o'),('gradient','neogeo_logo.o'),
      ('draw_cap','neogeo_logo.o'),('draw_sides','neogeo_logo.o'),('msx_neogeo_logo','neogeo_logo.o'),
      ('rdpq_triangle',None),('rdpq_triangle_rsp',None),('floor',None),('floorf',None)]
# Callees allowed outside the path: the rspq buffer switch (once a buffer)
# and the zeroing of draw_sides' vertices at its entry (once a frame;
# n64.mk builds with -ftrivial-auto-var-init=pattern).
OUTSIDE={'rspq_next_buffer','memset'}
def main(elf,mapf):
    fn=[]
    for l in subprocess.check_output([str(B)+'nm','-S','-n',elf],text=True).splitlines():
        p=l.split()
        if len(p)==4 and p[2] in 'tTwW' and int(p[1],16):fn.append((int(p[0],16),int(p[1],16),p[3]))
    fn.sort();starts=[f[0] for f in fn]
    secs=[];text=Path(mapf).read_text().splitlines()
    for k,l in enumerate(text):
        m=re.match(r'^ ((?:\.text|keep\.text)\S*)(?:\s+0x(8[0-9a-f]+)\s+0x([0-9a-f]+)\s+(\S+))?$',l)
        if not m:continue
        if not m[2]:
            m2=re.match(r'^\s+0x(8[0-9a-f]+)\s+0x([0-9a-f]+)\s+(\S+)$',text[k+1]) if k+1<len(text) else None
            if m2:secs.append((int(m2[1],16),int(m2[2],16),m2[3]))
        else:secs.append((int(m[2],16),int(m[3],16),m[4]))
    secs.sort();ss=[s[0] for s in secs]
    def obj(a):
        j=bisect.bisect_right(ss,a)-1
        return re.sub(r'.*/','',secs[j][2]).split('(')[-1].rstrip(')') if j>=0 and a<secs[j][0]+secs[j][1] else '?'
    def at(a):
        j=bisect.bisect_right(starts,a)-1
        return fn[j] if j>=0 and a<fn[j][0]+fn[j][1] else None
    path=[];errors=[]
    for name,o in PATH:
        hits=[f for f in fn if f[2]==name and (o is None or obj(f[0])==o)]
        if o is None:hits=[f for f in hits if obj(f[0])!='neogeo_logo.o']
        if not hits and o=='neogeo_logo.o' and name!='msx_neogeo_logo':continue   # inlined in its caller
        if len(hits)!=1:errors.append('%s (%s): %d definitions'%(name,o or 'library',len(hits)));continue
        path.append(hits[0])
    if errors:print('\n'.join(errors));return 1
    names={f[2] for f in path}
    # Calls out of the per-vertex functions (msx_neogeo_logo's own calls are per frame).
    dis=subprocess.check_output([str(B)+'objdump','-d','--no-show-raw-insn','-j','.text',elf],text=True)
    for a,s,n in path:
        per_frame=n=='msx_neogeo_logo'
        for m in re.finditer(r'^\s*([0-9a-f]+):\t(?:jal|j|b\w*)\s+(?:\S+,)?([0-9a-f]{8}) <',dis,re.M):
            src=int(m[1],16)
            if not a<=src<a+s:continue
            t=at(int(m[2],16))
            if per_frame:continue   # msx_neogeo_logo's own calls are per frame
            if t and t[0]!=a and t[2] not in names and t[2] not in OUTSIDE:
                errors.append('%s calls %s at %08x, outside the checked path'%(n,t[2],t[0]))
    owner={}
    for a,s,n in path:
        for line in range(a&~(LINE-1),a+s,LINE):
            idx=(line//LINE)%SETS
            prev=owner.get(idx)
            if prev and prev[0]!=line:
                errors.append('line %3d: %s %08x and %s %08x'%(idx,prev[1],prev[0],n,line))
            else:owner[idx]=(line,n)
    for a,s,n in path:print('  %08x %5x %-18s set %3d..%3d'%(a,s,n,(a//LINE)%SETS,((a+s-1)//LINE)%SETS))
    if errors:
        print('%s: the logo per-vertex path shares instruction cache lines:'%elf);print('\n'.join(errors[:40]));return 1
    print('%s: logo per-vertex path, %d functions, no shared instruction cache line'%(elf,len(path)));return 0
if __name__=='__main__':
    if len(sys.argv)!=3:sys.exit(__doc__)
    sys.exit(main(*sys.argv[1:]))
