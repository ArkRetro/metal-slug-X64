#!/usr/bin/env python3
"""Why four VRAM dumps of the credits differ from the native sprites (docs/MISION6.md "Los cuatro volcados").

tools/export_rest_sprites.py --ending compares, at every dump of the capture, the sprite-list entries the credits'
nodes (routines 0646FE..0A4744) wrote in their last update with the dump's VRAM; four dumps (63630, 65820, 68010,
70200) had different pixels. The explanation tested here with the capture alone: the sprite list a pass builds
reaches VRAM at the next vertical blank, so the entries of a node whose FIRST update is in the pass in progress at
a dump are not in that dump's VRAM yet.

For every dump in --frames and in --control: the nodes of the range whose first update that writes sprite-list
entries (W records of 00A078) falls within --pass frames before the dump, the palette slots of those entries
(attribute high byte; every line of a page has a slot of its own), and how many tile words of the hardware sprites
that are on carry those slots in this dump and in the next one (30 frames later). A control dump can have a birth too
(a picture 0A454C born outside the screen): its slot is not in VRAM either, and no pixel differs."""
import argparse,gzip,json,struct
from pathlib import Path
def slot_tiles(capture,f,slots):
    v=struct.unpack('>34304H',(capture/f'vram{f}.bin').read_bytes());n=0;rows=0
    for s in range(1,381):
        c3=v[0x8200+s]
        if not c3&64:rows=c3&63
        for r in range(min(rows,32)):
            if v[s*64+2*r] and v[s*64+2*r+1]>>8 in slots:n+=1
    return n
def main():
    ap=argparse.ArgumentParser(description=__doc__,formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('capture',type=Path);ap.add_argument('--frames',default='63630,65820,68010,70200')
    ap.add_argument('--control',default='63600,63660,65790,65850,67980,68040,70170,70230,64500,66900,69000,71400')
    ap.add_argument('--pass',dest='span',type=int,default=4);ap.add_argument('--report',type=Path)
    a=ap.parse_args()
    targets=[int(x) for x in a.frames.split(',')];want=sorted(targets+[int(x) for x in a.control.split(',')])
    lo,hi=0x646fe,0xa4744
    first={};alive={};frame=0;cur=None   # node -> (frame of its first update with entries, routine, slots)
    trace=a.capture/'boss-trace.txt'
    for line in (open(trace) if trace.exists() else gzip.open(str(trace)+'.gz','rt')):
        t=line[0]
        if t=='F':frame=int(line[2:]);continue
        if t=='U':
            p=line.split();node=int(p[1],16);routine=int(p[2],16);cur=None
            if lo<=routine<=hi:
                # A new life in the slot: the line's or the picture's first routine, or no update for 8 frames (a
                # node deleted by its page, 0041AC, has no update of its own for it).
                if node not in alive or routine in (0x64ac0,0xa454c) or frame-alive[node]['seen']>8:alive[node]=dict(frame=None,routine=routine,slots=set(),w=0)
                cur=alive[node];cur['seen']=frame
            else:alive.pop(node,None)   # the slot holds another actor now
        elif t=='W' and cur is not None:
            p=line.split()
            if p[4]=='a078' and int(p[1],16)<0x10f000:
                if cur['frame'] is None:cur['frame']=frame+1;first.setdefault(frame+1,[]).append(cur)
                if cur['frame']==frame+1:
                    if cur['w']%6==3:cur['slots'].add(int(p[3],16)>>8)
                    if cur['w']<2:cur.setdefault('xy',[]).append(int(p[3],16))
                    cur['w']+=1
        elif t=='V' and cur is not None and line.split()[2].startswith('00003f1e'):
            for k,v in list(alive.items()):
                if v is cur:del alive[k]
    rep=[]
    for f in want:
        born=[n for fr in range(f-a.span+1,f+1) for n in first.get(fr,[])]
        slots=set().union(*[n['slots'] for n in born]) if born else set()
        now=slot_tiles(a.capture,f,slots) if slots else 0;later=slot_tiles(a.capture,f+30,slots) if slots else 0
        rep.append(dict(frame=f,kind='different in the export' if f in targets else 'control',born=[dict(routine='%06X'%n['routine'],first_frame=n['frame'],slots=sorted(n['slots']),first_entry_xy=n.get('xy')) for n in born],
                        tile_words_with_their_slots=now,in_the_next_dump=later))
        print('%s %d: %d nodes with their first entries in the last %d frames (routines %s, slots %s); tile words with those slots in this dump: %d, in the next: %d'%(
            'DIFFERENT' if f in targets else 'control  ',f,len(born),a.span,sorted({'%06X'%n['routine'] for n in born}),['%02X'%s for s in sorted(slots)],now,later))
    # Every dump with a birth in its pass has none of the newborn's slots in VRAM, and the dumps that differ are among
    # them (a newborn whose first entry lies outside the screen, a picture 0A454C coming in, changes no pixel).
    ok=all(r['tile_words_with_their_slots']==0 and r['in_the_next_dump']>0 for r in rep if r['born']) and all(r['born'] for r in rep if r['frame'] in targets)
    print('RESULT: %s'%('every dump that differs is the dump of a birth, and no newborn entry is in the VRAM of its dump (it is in the next)' if ok else 'NOT explained'))
    if a.report:a.report.write_text(json.dumps(rep,indent=1)+'\n')
    raise SystemExit(0 if ok else 1)
if __name__=='__main__':main()
