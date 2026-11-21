#!/usr/bin/env python3
"""The ending's shrunk sprites for the console (src/end_zoom.c, docs/MISION6.md "Sprites con zoom en N64").

Exports into one romfs file, from the user's ROM and BIOS only:
- the P2 descriptors the sprite list carries with a zoom word != FFFF (the results' dial 0577E2 and the title's pieces
  0A81A4): every entry of the two lists (A5 + 1948 / 294E, check_zoom_scb.pending) of every RAM dump of the captures
  given (MAME 0.264: captures-m6/fam/T1, build/captures/ZC, ZT); their words exactly as neo_scb_entry reads them;
- the C-ROM tiles those descriptors name (tile = SCB1 word | (descriptor word 0 bits 4..7) << 12), decoded from the bitplanes of
  filesystem/sprites.bin (check_lspc_shrink.pen_of) into 16 x 16 CI4, 128 bytes each;
- the L0 ROM of the BIOS (000-lo.lo), all 256 rows of 256 bytes: the zoom y byte of a flying piece takes values the
  captures do not all show, and a row missing on the console would have to be invented.

File (big-endian): 'EZM2', ndesc, ntiles, nwords, then ndesc x (P2 address, first word, word count, first tile, tile
count), the words (u16, padded to 4 bytes), the tile numbers (u32; per descriptor its own, sorted: the console reads one
descriptor's tiles in one go), the tiles (in the same order, 128 B each: 16 x 16 CI4 row by row, decoded from the C-ROM's bitplanes), the L0 (65 536 B),
then the RDP path's tables (rdp_tables: nseq, npieces; 32 x 20 B horizontal; 256 x 16 x 10 B runs; nseq x 4 B; npieces x 6 B).

  export_ending_zoom.py --rom mslugx.zip --bios neogeo.zip --crom filesystem/sprites.bin --out romfs/endzoom.bin CAPTURE..."""
import argparse,functools,struct,sys,zipfile
from pathlib import Path
sys.path.insert(0,str(Path(__file__).resolve().parent))
from check_zoom_scb import A5,load_p2,ram_l,ram_w

def descriptor_words(p2,ptr):
    a=(((ptr>>20)&0xf)-1)*0x100000+(ptr&0xfffff)
    w=lambda i:(p2[a+2*i]<<8)|p2[a+2*i+1]
    w0=w(0);i=1;tiles=[]
    for _ in range(w0>>8):
        cr=w(i+2);i+=3
        n=((cr&0xff)-1)&0x1f
        for _c in range(cr>>8):
            tiles+=[w(i+k)|((w0&0xf0)<<12) for k in range(n+1)];i+=n+1
    return [w(k) for k in range(i)],tiles

def linear(c):
    """A C-ROM tile (check_lspc_shrink.pen_of's layout) as 16 x 16 CI4, row by row, the left pixel in the high nibble."""
    out=bytearray(128)
    for yy in range(16):
        for xx in range(16):
            off=(64 if xx<8 else 0)+yy*4
            pen=sum(((c[off+b]>>(xx%8))&1)<<bit for bit,b in enumerate((0,2,1,3)))
            out[yy*8+xx//2]|=pen<<4 if xx%2==0 else pen
    return bytes(out)

COLUMNS=[
 (8,),(4,8),(4,8,12),(2,4,8,12),(2,4,8,12,14),(2,4,6,8,12,14),(2,4,6,8,10,12,14),(0,2,4,6,8,10,12,14),
 (0,2,4,6,8,9,10,12,14),(0,2,3,4,6,8,9,10,12,14),(0,2,3,4,6,8,9,10,12,14,15),(0,2,3,4,6,7,8,9,10,12,14,15),
 (0,2,3,4,6,7,8,9,10,12,13,14,15),(0,1,2,3,4,6,7,8,9,10,12,13,14,15),(0,1,2,3,4,6,7,8,9,10,11,12,13,14,15),
 tuple(range(16))]   # check_lspc_shrink.COLUMNS (derived from the T1 screenshots)

@functools.lru_cache(maxsize=None)
def fit(seq):
    """(S, d) of an RDP texture rectangle (S: s0 / t0 in 10.5, d: ds/dx / dt/dy in 5.10, point sampling) whose texel
    at step i is floor((32 S + d i) / 1024) = seq[i] for every i; None when none exists (exhaustive over d)."""
    if len(seq)==1:return (seq[0]*32,0)
    k=len(seq)-1;a=seq[-1]-seq[0]
    # nearest to the mean step first (an exact 1:1 run gets d = 1024, so pieces of consecutive tiles continue)
    c0=round(a*1024/k)
    for d in sorted(range(max(-16*1024,((a-1)*1024)//k-2),min(16*1024-1,((a+1)*1024)//k+3)),key=lambda d:(abs(d-c0),d)):
        lo=-10**9;hi=10**9
        for i,c in enumerate(seq):
            lo=max(lo,c*1024-d*i);hi=min(hi,c*1024+1023-d*i)
            if lo>hi:break
        else:
            S=-(-lo//32)
            if 32*S<=hi:return (S,d)
    return None

def pieces(seq):
    """The fewest exact rectangles for seq: [(offset, length, S, d)]."""
    seq=tuple(seq);best={len(seq):(0,None)}
    for k in range(len(seq)-1,-1,-1):
        best[k]=min(((1+best[j][0],j) for j in range(k+1,len(seq)+1) if fit(seq[k:j])),key=lambda b:b[0])
    out=[];k=0
    while k<len(seq):
        j=best[k][1];S,d=fit(seq[k:j]);out.append((k,j-k,S,d));k=j
    return out

def rdp_tables(l0):
    """The RDP path's tables (src/end_zoom_rdp.c): horizontal pieces per shrink nibble and mirror; per zoom y and tile
    row (L0 high nibble, lines 0..255 of the sprite) its first line and line count and the pieces of its 4 line sequences
    (as read; reversed: lines 256..511, the L0 mirrored; tile flipped vertically; both)."""
    out=bytearray()
    for zx in range(16):
        for hf in (0,1):
            p=pieces([15-c if hf else c for c in COLUMNS[zx]]);assert len(p)<=3
            e=struct.pack('>B',len(p))+b''.join(struct.pack('>BBhh',*q) for q in p)
            out+=e+b'\0'*(20-len(e))
    seq_id={};seqs=[]
    runs=bytearray()
    for zy in range(256):
        rows={};start={}
        for zl in range(256):
            b=l0[(zy<<8)|zl];t=b>>4
            if t in rows:assert start[t]+len(rows[t])==zl,'a tile row not contiguous in the L0'
            else:start[t]=zl;rows[t]=[]
            rows[t].append(b&15)
        for t in range(16):
            if t not in rows:runs+=struct.pack('>BB4H',0,0,0,0,0,0);continue
            v=rows[t];ids=[]
            for q in (v,v[::-1],[15-x for x in v],[15-x for x in v[::-1]]):
                q=tuple(q)
                if q not in seq_id:seq_id[q]=len(seqs);seqs.append(q)
                ids.append(seq_id[q])
            runs+=struct.pack('>BB4H',start[t],len(v),*ids)   # len 1..16 (0: no such tile row)
    ptab=bytearray();stab=bytearray()
    for q in seqs:
        p=pieces(q);stab+=struct.pack('>HBB',len(ptab)//6,len(p),0)
        for e in p:ptab+=struct.pack('>BBhh',*e)
    head=struct.pack('>II',len(seqs),len(ptab)//6)
    return head+out+runs+stab+ptab,len(seqs),len(ptab)//6

def main():
    ap=argparse.ArgumentParser();ap.add_argument('captures',nargs='+',type=Path)
    ap.add_argument('--rom',type=Path,required=True);ap.add_argument('--bios',type=Path,required=True)
    ap.add_argument('--crom',type=Path,required=True);ap.add_argument('--out',type=Path,required=True)
    a=ap.parse_args()
    p2=load_p2(a.rom);l0=zipfile.ZipFile(a.bios).read('000-lo.lo')[:65536]   # (zoom y << 8) | line: the rows check_lspc_shrink reads
    crom=a.crom.read_bytes()
    descs=set();dumps=0
    for c in a.captures:
        for f in sorted(c.glob('ram*.bin')):
            r=f.read_bytes();dumps+=1
            for fwd in (False,True):
                # both lists of the dump (the one 11D04 wrote and the one 11E60 shows)
                base=A5+(0x1948 if fwd else 0x294e);end=ram_l(r,base+0x400)
                if not base<=end<=base+0x400:continue
                for p in range(base,end,4):
                    e=base+ram_w(r,p)
                    if ram_w(r,e+4)!=0xffff:descs.add(ram_l(r,e+8))
    table=[];words=[];keys=[]
    for d in sorted(descs):
        w,t=descriptor_words(p2,d);t=sorted(set(t))
        table.append((d,len(words),len(w),len(keys),len(t)));words+=w;keys+=t
    out=bytearray(b'EZM3'+struct.pack('>III',len(table),len(keys),len(words)))
    for e in table:out+=struct.pack('>5I',*e)
    out+=struct.pack('>%dH'%len(words),*words)
    if len(words)&1:out+=b'\0\0'   # the words padded to 4 bytes
    out+=struct.pack('>%dI'%len(keys),*keys)
    for t in keys:out+=linear(crom[t*128:t*128+128])
    out+=l0
    tables,nseq,npieces=rdp_tables(l0);out+=tables
    a.out.write_bytes(out)
    print('export_ending_zoom: %d sequences, %d pieces; '%(nseq,npieces)+'%d dumps, %d descriptors, %d words, %d tiles (%d different), %d bytes -> %s'%(dumps,len(table),len(words),len(keys),len(set(keys)),len(out),a.out))
if __name__=='__main__':main()
