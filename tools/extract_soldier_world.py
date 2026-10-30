#!/usr/bin/env python3
"""Closed-loop evidence for the native region world (src/neo_world.c).

For every original update of a capture: the inputs the world reads (scroll
longs, epoch, video counter, player actor), the original list order of the
world's actors (script, runners, soldiers and deaths, thrown objects, death
splashes) and the complete ROM 0029C6 call log of the update with the value
each call returned (recovered by stepping the dumped ring; equality with the
next dump is asserted). Calls by actors outside the world (player, bullets,
sparks, the 099DCC/099E06 entities) are injected by the test at their
original position; calls by world actors must be made natively.

Expected results per update: RNG ring, script position, group counters and,
for every world soldier, its fields and routine-private block; for thrown
objects their position."""
import argparse,gzip,hashlib,json
from pathlib import Path
import soldier_capture
from soldier_capture import Capture,PLAYER,rng_step,soldier
SCRIPT=0x101414;DEAD=0x3F1E
# Generic actors ported natively (src/neo_region.c families), by routine
# label. Everything else is an outside actor whose RNG calls are injected.
GACTOR=set([0x2916E,0x29294,0x29326,0x2936C,0x293DC,0x29438,0x294A2,0x294E4,0x29566,0x295BC,0x29802,0x29852,0x298F4])
def gactor_routine(rt):return rt in GACTOR
def world_routine(rt):
    if gactor_routine(rt):return True
    # Soldier family, pistol death (040DB4/040DF0), death splash (040F88/040FD4),
    # runners and thrown objects. The 0400AA entity (states 0400F2..) is not
    # part of the world.
    return (rt in (0x1A422,0x1A438,0x40DB4,0x40DF0,0x40F88,0x40FD4) or 0xAE000<=rt<0xB1000 or
        0x3BC28<=rt<0x3BE8E or 0x1C8FC<=rt<0x1CA9E)
def kind_of(rt):
    if gactor_routine(rt):return 5
    if rt in (0x1A422,0x1A438):return 1
    if 0x3BC28<=rt<0x3BE8E or 0x1C8FC<=rt<0x1CA9E:return 3
    if rt in (0x40F88,0x40FD4):return 4
    return 2
def tick_frames(cap,first,last):
    out=[];prev=cap.frame(first).b(0x10C33A)
    for f in range(first+1,last+1):
        e=cap.frame(f).b(0x10C33A)
        if e!=prev:out.append(f)
        prev=e
    return out
def order(before,after):
    seq=[n for n,_ in before.actors()];later=[n for n,_ in after.actors()]
    for i,n in enumerate(later):
        if n not in seq:
            prev=later[i-1] if i else None
            seq.insert(seq.index(prev)+1 if prev in seq else len(seq),n)
    return seq
def extract(path,last,label):
    cap=Capture(path);calls=cap.rng_calls()
    soldier_capture.FIXED=(Path(path)/'program-fixed.bin').read_bytes()
    files=sorted(int(p.stem[3:]) for p in Path(path).glob('ram*.bin'))
    ticks=tick_frames(cap,files[0],min(last,files[-1]))
    first=cap.frame(ticks[0]-1)
    words,cur=first.rng()
    out=dict(capture=label,first_frame=ticks[0],epoch=first.b(0x10C33A),rng=dict(words=words,cursor=cur),
        pc=first.l(SCRIPT+0x6E),scroll_x=first.l(0x10E97C),grid=list(first.raw(0x1067E6,256)),
        grid_cursor=(first.l(0x1068E6)-0x1067E6)//32,ticks=[])
    for i,f in enumerate(ticks[:-1]):
        before,after=cap.frame(f-1),cap.frame(ticks[i+1]-1)
        rb={n:rt for n,rt in before.actors()};ra={n:rt for n,rt in after.actors()}
        reused={n for n,rt in ra.items() if n!=SCRIPT and world_routine(rt) and rb.get(n,DEAD)!=DEAD and not world_routine(rb[n])}
        seq=order(before,after);later=[n for n,_ in after.actors()]
        # Positions are doubled so that a node reused within the update (its
        # previous occupant, e.g. a bullet, ran at its old place; the new
        # world actor was inserted after its creator) can take the odd slot
        # after the creator's position.
        def position(n):
            if n in reused:
                i=later.index(n);return 2*seq.index(later[i-1])+1
            return 2*seq.index(n)
        world=[]
        for n in seq:
            rt=rb.get(n,DEAD)
            # A node freed and reused within the update (a bullet spark
            # replaced by a death splash) is a created world actor.
            created=rt==DEAD or (not world_routine(rt) and n!=SCRIPT and world_routine(ra.get(n,DEAD)))
            if created:rt=ra.get(n,DEAD)
            if n==SCRIPT:world.append(dict(node=n,kind=0,created=False))
            elif rt!=DEAD and world_routine(rt):world.append(dict(node=n,kind=kind_of(rt),created=created))
        fcalls=[c for g in range(f,ticks[i+1]) for c in calls.get(g,[])]
        words,cur=before.rng();log=[]
        for node,_ in fcalls:
            v,cur=rng_step(words,cur);log.append(dict(node=node,value=v))
        assert (words,cur)==after.rng(),'RNG ring not reproduced at %d'%f
        bullets=[]
        for n in seq:
            if rb.get(n)!=0x1B882:continue
            bullets.append(dict(position=2*seq.index(n),body={k:before.l(n+o) for k,o in (('x',0x28),('y',0x2C),('vx',0x30),('vy',0x34),('parent',0x38),('px',0x3C),('py',0x40),('cx',0x44),('cy',0x48))},
                flags15=before.b(n+0x15),flags5f=before.b(n+0x5F),mark=before.b(n+0x4C),correction=before.b(n+0x4D),
                kind_angle=before.w(n+0x6C),f6f=before.b(n+0x6F),f5e=before.b(n+0x5E)))
        for wn in world:wn['position']=position(wn['node'])
        player=dict(px=before.w(PLAYER+0x3C),py=before.w(PLAYER+0x40),flags15=before.b(PLAYER+0x15),
            alive=before.l(PLAYER)!=DEAD,dead=bool(before.b(PLAYER+0x5F)&0xC0),follow=before.b(PLAYER+0x7A))
        if before.l(PLAYER)==DEAD and after.l(PLAYER)!=DEAD:
            # Continue: the player node is re-created and placed by an actor
            # that runs before the world actors; the position they read is not
            # recorded (the test takes the soldiers' control targets of this
            # update from the original, counted as injected).
            player=dict(px=after.w(PLAYER+0x44),py=after.w(PLAYER+0x48),flags15=after.b(PLAYER+0x15),alive=True,dead=False,follow=after.b(PLAYER+0x7A),respawn=True)
        pl=before
        world.sort(key=lambda wn:wn['position'])
        soldiers={};throws={};gactors={}
        for n,rt in ra.items():
            if rt==DEAD or not world_routine(rt) or n==SCRIPT:continue
            k=kind_of(rt)
            if k==2:soldiers['%X'%n]=dict(after=soldier(after,n),locals=list(after.raw(n+0x6E,0x26)))
            elif k==3:throws['%X'%n]=dict(x=after.l(n+0x28),y=after.l(n+0x2C),routine=rt)
            elif k==5:gactors['%X'%n]=after.raw(n,0xC0).hex()
        out['ticks'].append(dict(frame=f,scroll_x=after.l(0x10E97C),scroll_y=after.l(0x10E980),epoch=after.b(0x10C33A),
            video=before.w(0x10000E),active=after.b(0x10C33C),player=player,
            order=world,log=log,bullets=bullets,
            expect=dict(rng=dict(words=list(words),cursor=cur),pc=after.l(SCRIPT+0x6E),groups=list(after.raw(0x10C326,9)),
                grid=[after.w(0x1067E6+2*k) for k in range(128)],
                soldiers=soldiers,throws=throws,gactors=gactors)))
    return out
def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('captures',nargs='+',help='DIR:LAST_FRAME')
    p.add_argument('--output',type=Path,required=True);a=p.parse_args()
    data=dict(source='MAME 0.264 mslugx euro; per-frame RAM dumps and print-only RNG log',captures=[])
    for spec in a.captures:
        path,last=spec.rsplit(':',1);c=extract(path,int(last),Path(path).name)
        c['debug_log_sha256']=hashlib.sha256((Path(path)/'debug.log').read_bytes()).hexdigest()
        data['captures'].append(c)
        print(c['capture'],len(c['ticks']),'updates',sum(len(t['log']) for t in c['ticks']),'RNG calls',
            sum(len(t['expect']['soldiers']) for t in c['ticks']),'soldier states')
    with gzip.open(a.output,'wt',compresslevel=9) as f:f.write(json.dumps(data,separators=(',',':'))+'\n')
if __name__=='__main__':main()
