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
import argparse,gzip,hashlib,json,struct
from pathlib import Path
import soldier_capture
from soldier_capture import Capture,PLAYER,rng_step,soldier
SCRIPT=0x101414;DEAD=0x3F1E
# Generic actors ported natively (src/neo_region.c families), by routine
# label. Everything else is an outside actor whose RNG calls are injected.
GACTOR=set([0x2916E,0x29294,0x29326,0x2936C,0x293DC,0x29438,0x294A2,0x294E4,0x29566,0x295BC,0x29802,0x29852,0x298F4,
    0x1C6EE,0x1C706,0x1C71E,0x1C736,0x1C74E,0x1C766,0x1C77E,0x1C796,0x1C9E6,0x1D764,0x1D7B8,
    0x1D86C,0x1D8A4,0x1D8D4,0x1D8E4,0x1D908,0x1D934,0x1D93C,0x1D944,0x1D958,0x1D9C6,
    0xDC97E,0xDCA54,0xDCA9C,0xDCADC,0xDCAF8,0xDCBB6,0xDCBC2,0xDD3E0,0xDD418,0xDD642,0xDD6BE,0x9503C,0xDD802,0xDD82A,
    0x403A0,0x40396,0x403C0])
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
OUTSIDE=0xFFFFFF
# Middle-bank collision layer (RAM 10D7B6): once it scrolls at full speed it
# sits exactly on the terrain layer 10C72E and the port's single merged grid
# (filesystem/mission1/collision.json) stands for both; an actor resting on it
# is compared with the terrain tag. Asserted per update, never assumed.
LAYER_B=0x10D7B6
def same_layers(frame):
    return frame.raw(0x10C72E+0x14,16)==frame.raw(LAYER_B+0x14,16)
def owned(frame):
    # Generic actors are world actors only when created by the script, a
    # runner or another world generic actor (an effect created by an outside
    # actor, e.g. a thrown object's burst, stays outside).
    acts=dict(frame.actors());world=set()
    changed=True
    while changed:
        changed=False
        for n,rt in acts.items():
            if n in world or not gactor_routine(rt):continue
            c=frame.l(n+0x0C)
            if 0x1C8FC<=rt<0x1CA9E and c==SCRIPT:continue
            if c==SCRIPT or acts.get(c) in (0x1A422,0x1A438) or c in world:world.add(n);changed=True
    def label(n,rt):
        # A thrown object's burst runs the shared effect routine 01C9E6 in
        # the thrown object's node (created by the script actor): it stays a
        # thrown object (kind 3, src/neo_throw_actor.c).
        if 0x1C8FC<=rt<0x1CA9E and frame.l(n+0x0C)==SCRIPT:return 0x3BE6C
        return OUTSIDE if gactor_routine(rt) and n not in world else rt
    return {n:label(n,rt) for n,rt in acts.items()}
RUNNERS=(0x1A422,0x1A438)
def exec_order(ev,first,nxt,before,after,family):
    """World actor order of one update from the execution log: positions are
    2*execution index; created = allocated earlier in the same update."""
    events=[e for g in range(first,nxt) for e in ev.get(g,[])]
    # Nodes alive before the capture's first allocation: ownership from the dump.
    for n,rt in before.actors():
        if n in family:continue
        c=before.l(n+0x0C)
        if THROW_CTORS(rt) and c==SCRIPT:family[n]='throw'
        elif 0xAE000<=rt<0xB1000 or rt in (0x40DB4,0x40DF0):family[n]='soldier'
        elif gactor_routine(rt) and (c==SCRIPT or before.l(c) in RUNNERS or family.get(c)=='gactor'):family[n]='gactor'
        else:family.setdefault(n,None)
    world=[];log_pos=[];exec_pos={};idx=0;cur=None;created=set();allocs=[];splats=[];cur_world=False;cur_rt=None
    for e in events:
        if e[0]=='ALLOC':
            _,n,rt,cr=e
            if rt==0xAF17E:family[n]='soldier'
            elif THROW_CTORS(rt) and cr==SCRIPT:family[n]='throw'
            elif gactor_routine(rt) and (cr==SCRIPT or family.get(cr)=='gactor' or family.get(cr)=='runner'):family[n]='gactor'
            elif rt in (0x40F88,):family[n]='splash'
            elif rt in RUNNERS:family[n]='runner'
            else:family[n]=None
            created.add(n);allocs.append((n,rt))
        elif e[0]=='EXEC':
            _,n,rt=e;idx+=1;pos=2*idx;cur=pos;exec_pos.setdefault(n,pos);cur_rt=rt
            cur_world=False
            if n==SCRIPT:world.append(dict(node=n,kind=0,created=False,position=pos));cur_world=True;continue
            fam=family.get(n);k=None
            if rt in RUNNERS:k=1
            elif fam=='soldier' and (0xAE000<=rt<0xB1000 or rt in (0x40DB4,0x40DF0)):k=2
            elif fam=='throw' and (THROW_CTORS(rt) or 0x3BC28<=rt<0x3BE8E):k=3
            elif rt in (0x40F88,0x40FD4):k=4
            elif fam=='gactor' and gactor_routine(rt):k=5
            if k is not None:world.append(dict(node=n,kind=k,created=n in created,position=pos));cur_world=True
        elif e[0]=='SPLAT':
            # Splats by actors outside the world, injected at their place.
            # A kill splat (00498E) of a world target by an injected attack
            # (a bullet, or the knife injection of a soldier) is made by the
            # port itself.
            bullet=cur_rt in (0x1B6DC,0x1B716,0x1B750,0x1B782,0x1B882)
            target=family.get(e[5])
            injected=e[4]==0x4992 and target in ('soldier','gactor') and bullet
            if not cur_world and not injected:splats.append(dict(position=cur if cur is not None else 0,x=e[2],y=e[3]))
        else:
            log_pos.append(cur if cur is not None else -1)
    # Thrown objects allocated this update run from the next one: listed as
    # created entries (most recent first, as 00411C leaves them) for the
    # test's node mapping.
    for n,rt in reversed(allocs):
        if family.get(n)=='throw' and n not in exec_pos:world.append(dict(node=n,kind=3,created=True,position=2*idx+2))
    return world,log_pos,exec_pos,splats
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
def exec_log(path):
    # Execution/allocation/RNG events in log order, keyed like the RNG calls
    # (the dump that follows them); None when the capture has no EXEC lines.
    import re
    ev={};seen=False
    for line in (Path(path)/'debug.log').read_text().splitlines():
        tag=line.split(' ',1)[0]
        if tag not in ('EXEC','ALLOC','RNG','SPLAT'):continue
        f=int(re.search(r' f=(\d+)',line).group(1))+1
        if tag=='EXEC':
            m=re.search(r'a6=(\w+) rt=(\w+)',line);ev.setdefault(f,[]).append(('EXEC',int(m.group(1),16),int(m.group(2),16)));seen=True
        elif tag=='ALLOC':
            m=re.search(r'a0=(\w+) rt=(\w+) cr=(\w+)',line);ev.setdefault(f,[]).append(('ALLOC',int(m.group(1),16),int(m.group(2),16),int(m.group(3),16)))
        elif tag=='SPLAT':
            m=re.search(r'a6=(\w+) x=(\w+) y=(\w+)(?: ret=(\w+) a2=(\w+))?',line)
            ev.setdefault(f,[]).append(('SPLAT',int(m.group(1),16),int(m.group(2),16),int(m.group(3),16),int(m.group(4) or '0',16),int(m.group(5) or '0',16)))
        else:
            m=re.search(r'a6=(\w+)',line);ev.setdefault(f,[]).append(('RNG',int(m.group(1),16)))
    return ev if seen else None
THROW_CTORS=lambda rt:0x3BC28<=rt<0x3BE8E or 0x1C8FC<=rt<0x1CA9E
def extract(path,last,label):
    cap=Capture(path);calls=cap.rng_calls();ev=exec_log(path)
    # Node ownership from the allocation log: 'gactor' (world generic actor),
    # 'throw', 'soldier' or None (outside).
    family={}
    soldier_capture.FIXED=(Path(path)/'program-fixed.bin').read_bytes()
    files=sorted(int(p.stem[3:]) for p in Path(path).glob('ram*.bin'))
    ticks=tick_frames(cap,files[0],min(last,files[-1]))
    first=cap.frame(ticks[0]-1)
    words,cur=first.rng()
    out=dict(capture=label,first_frame=ticks[0],epoch=first.b(0x10C33A),rng=dict(words=words,cursor=cur),
        pc=first.l(SCRIPT+0x6E),scroll_x=first.l(0x10E97C),grid=list(first.raw(0x1067E6,256)),
        grid_cursor=(first.l(0x1068E6)-0x1067E6)//32,marker_seq=first.b(0x10C446),ticks=[])
    for i,f in enumerate(ticks[:-1]):
        before,after=cap.frame(f-1),cap.frame(ticks[i+1]-1)
        rb=owned(before);ra=owned(after)
        reused={n for n,rt in ra.items() if n!=SCRIPT and world_routine(rt) and rb.get(n,DEAD)!=DEAD and not world_routine(rb[n])}
        # Generic actor node freed and taken again within the update (the
        # creator changes): its old occupant ran at its place, the new one
        # was inserted after its creator.
        regen={n for n,rt in ra.items() if n!=SCRIPT and gactor_routine(rt) and gactor_routine(rb.get(n,DEAD)) and before.l(n+0x0C)!=after.l(n+0x0C)}
        reused|=regen
        # A dead placeholder node (003F1E kept while deeper nodes follow it)
        # taken again within the update is a created actor after its creator.
        reused|={n for n,rt in ra.items() if n!=SCRIPT and world_routine(rt) and n in rb and rb[n]==DEAD}
        seq=order(before,after);later=[n for n,_ in after.actors()];later_all=later
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
            if n in regen:
                world.append(dict(node=n,kind=kind_of(rb[n]),created=False,position=2*seq.index(n)))
                world.append(dict(node=n,kind=kind_of(ra[n]),created=True))
                continue
            if n==SCRIPT:world.append(dict(node=n,kind=0,created=False))
            elif rt!=DEAD and world_routine(rt):world.append(dict(node=n,kind=kind_of(rt),created=created))
        fcalls=[c for g in range(f,ticks[i+1]) for c in calls.get(g,[])]
        words,cur=before.rng();log=[]
        for node,_ in fcalls:
            v,cur=rng_step(words,cur);log.append(dict(node=node,value=v))
        if ev is not None:
            world,log_pos,exec_pos,splats=exec_order(ev,f,ticks[i+1],before,after,family)
            assert len(log_pos)==len(log),'RNG log mismatch at %d'%f
            for e,pos in zip(log,log_pos):e['position']=pos
        assert (words,cur)==after.rng(),'RNG ring not reproduced at %d'%f
        bullets=[]
        def bpos(n):
            if ev is None:return 2*seq.index(n)
            return exec_pos.get(n)
        def s16(v):return v-65536 if v&0x8000 else v
        for n in seq:
            if rb.get(n) in (0x1B6DC,0x1B716,0x1B750,0x1B782):
                # Bullet constructor (01B7BA..01B83E) then its first 01B882
                # update: velocity from +72 (0x100000) and the angle +6E
                # (002E7C with the sine table ROM 0029FC), +6F row index.
                fx=soldier_capture.FIXED;ang=before.b(n+0x6E)
                sin=lambda i:s16(struct.unpack_from('>H',fx,0x29FC+2*(i&255))[0])
                cos=lambda i:s16(struct.unpack_from('>H',fx,0x29FC+0x80+2*(i&255))[0])
                vx=(0x1000*cos(ang))&0xffffffff;vy=(0x1000*sin(ang))&0xffffffff
                f6f=(((ang+15)&255 if ang&0x20 else ang)>>4)&15
                bullets.append(dict(position=bpos(n),body={k:before.l(n+o) for k,o in (('x',0x28),('y',0x2C),('parent',0x38),('px',0x3C),('py',0x40),('cx',0x44),('cy',0x48))}|dict(vx=vx,vy=vy),
                    flags15=(before.b(n+0x15)|0x20)&0xFE,flags5f=before.b(n+0x5F),mark=before.b(n+0x4C),correction=before.b(n+0x4D),
                    kind_angle=before.w(n+0x6C),f6f=f6f,f5e=before.b(n+0x5E)))
                continue
            if rb.get(n)!=0x1B882:continue
            if bpos(n) is None:continue
            bullets.append(dict(position=bpos(n),body={k:before.l(n+o) for k,o in (('x',0x28),('y',0x2C),('vx',0x30),('vy',0x34),('parent',0x38),('px',0x3C),('py',0x40),('cx',0x44),('cy',0x48))},
                flags15=before.b(n+0x15),flags5f=before.b(n+0x5F),mark=before.b(n+0x4C),correction=before.b(n+0x4D),
                kind_angle=before.w(n+0x6C),f6f=before.b(n+0x6F),f5e=before.b(n+0x5E)))
        for wn in world:
            if 'position' not in wn:wn['position']=position(wn['node'])
        bullets.sort(key=lambda b:b['position'])
        # Transient actors: created by a world generic actor and deleted within
        # the same update (no routine left in the dump). Optional in the test:
        # matched only when the port creates one at that place.
        wnodes={wn['node'] for wn in world if wn['kind']==5}
        for n in (range(0x1010A4,0x1063D4,0xB0) if ev is None else ()):
            if (n in ra and ra[n]!=DEAD) or (n in rb and rb[n]!=DEAD):continue
            if before.raw(n,0xB0)==after.raw(n,0xB0) or after.l(n)!=DEAD:continue
            if after.l(n+0x0C) in wnodes:
                cpos=[wn['position'] for wn in world if wn['node']==after.l(n+0x0C)][0]
                world.append(dict(node=n,kind=5,created=True,transient=True,position=cpos+1))
        player=dict(px=before.w(PLAYER+0x3C),py=before.w(PLAYER+0x40),flags15=before.b(PLAYER+0x15),
            alive=before.l(PLAYER)!=DEAD,dead=bool(before.b(PLAYER+0x5F)&0xC0),follow=before.b(PLAYER+0x7A),
            unregistered=before.l(PLAYER+0x50)==0)
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
            if ev is not None and gactor_routine(rt) and family.get(n)!='gactor':continue
            k=kind_of(rt)
            if k==2:
                sv=soldier(after,n)
                if sv['parent']==LAYER_B and same_layers(after):sv['parent']=0x10C72E
                soldiers['%X'%n]=dict(after=sv,locals=list(after.raw(n+0x6E,0x26)))
            elif k==3:throws['%X'%n]=dict(x=after.l(n+0x28),y=after.l(n+0x2C),routine=rt)
            elif k==5:
                rec=bytearray(after.raw(n,0xC0))
                if int.from_bytes(rec[0x38:0x3C],'big')==LAYER_B and same_layers(after):rec[0x38:0x3C]=(0x10C72E).to_bytes(4,'big')
                gactors['%X'%n]=rec.hex()
        out['ticks'].append(dict(frame=f,scroll_x=after.l(0x10E97C),scroll_y=after.l(0x10E980),epoch=after.b(0x10C33A),
            video=before.w(0x10000E),active=after.b(0x10C33C),player=player,view=[after.w(0x10E9D2+2*k) for k in range(4)],
            order=world,log=log,bullets=bullets,splats=splats if ev is not None else None,
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
