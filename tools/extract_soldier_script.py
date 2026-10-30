#!/usr/bin/env python3
"""Level-script evidence from per-frame RAM dumps (closed-loop fixture).

For every original update between the first dump and the end of the playable
region, keep the inputs the script actor (ROM 019F1A) and its runner actors
(ROM 01A438) read: camera word 10E99E, scroll longs 10E97C/10E980, player
liveness, the per-group live counts recounted by ROM 019E40 at the start of
the update (from the actor list itself), and the values returned by the ROM
0029C6 calls the script/runners made. The expected result is the state
visible before the next update: script position/timer, the runners in list
order, the group counters and the actors they created (group, variant and
constructor). Nothing is interpolated."""
import argparse,gzip,hashlib,json,re,struct
from pathlib import Path
import soldier_capture
from soldier_capture import Capture,PLAYER,rng_step,soldier,environment
SCRIPT=0x101414;
# State left by each constructor of the region's spawn table (ROM 0197DC):
# soldier 0AF17E->0AF65A, 099DCC/099E06->099E48, 0400AA->0400F2,
# 03FC5A->03FCFA, 0B6298->0B62C8, 08C37C->08C53C, 03264E->0326A8,
# 031D80->031E10.
FIRST_STATES={0xAF65A,0xAF380,0xAFCE2,0x99E48,0x400F2,0x3FCFA,0xB62C8,0x8C53C,0x326A8,0x31E10}
RUNNER=0x1A438;DEAD=0x3F1E
def tick_frames(cap,first,last):
    out=[];prev=cap.frame(first).b(0x10C33A)
    for f in range(first+1,last+1):
        e=cap.frame(f).b(0x10C33A)
        if e!=prev:out.append(f)
        prev=e
    return out
def live(frame):return [(n,rt) for n,rt in frame.actors() if rt!=DEAD]
def recount(frame):
    g=[0]*9
    for n,rt in frame.actors():
        v=frame.b(n+0x69)
        if v<9:g[v]+=1
    return g
def runners(frame):
    return [dict(node=n,record=frame.l(n+0x6E),timer72=frame.w(n+0x72),count76=frame.w(n+0x76)) for n,rt in live(frame) if rt==RUNNER]
def decayed(frame):
    """Delay grid as the constructor reads it: ROM 00469C (called by the
    mission actor 0186A0, which precedes the script) halves one 16-word row
    selected by the pointer at RAM 1068E6 at the start of every update."""
    g=list(frame.raw(0x1067E6,256));g=[g[i]<<8|g[i+1] for i in range(0,256,2)]
    row=(frame.l(0x1068E6)-0x1067E6)//32
    for i in range(16):g[row*16+i]>>=1
    return g
def construction(before,after,fcalls,node):
    """Evidence for one soldier constructor run (ROM 0AF17E, same update):
    resulting fields, RNG ring at its first call and the scene it reads."""
    words,cur=before.rng();calls=0;ring=None
    for owner,_ in fcalls:
        if owner==node:
            if ring is None:ring=(list(words),cur)
            calls+=1
        _,cur=rng_step(words,cur)
    env=environment(after);envb=environment(before)
    return dict(after=soldier(after,node),locals=list(after.raw(node+0x6E,0x26)),
        rng=dict(words=ring[0],cursor=ring[1],calls=calls),
        scene=dict(terrain=env['terrain'],epoch=env['epoch'],camera=after.w(0x10E97C),
            players=dict(p1x=envb['p1x'],p1y=envb['p1y'],p2x=envb['p2x'],p2y=envb['p2y'],
                mask=(1 if envb['p1live'] else 0)|(2 if envb['p2live'] else 0),mode88=envb['mode88'],rank=envb['rank']),
            grid=decayed(before)))
def spawn_log(path):
    """SPAWN/RUNNER lines of tools/mame/trace_soldier_campaign.lua (breakpoints
    at 01A10A/01A1E8), keyed like the RNG log by the video frame that follows."""
    out={};present=False
    for line in (Path(path)/'debug.log').read_text().splitlines():
        m=re.match(r'(SPAWN|RUNNER) f=(\d+) a6=(\w+) rec=(\w+)',line)
        if m:present=True;out.setdefault(int(m.group(2))+1,[]).append((m.group(1),int(m.group(3),16),int(m.group(4),16)))
    return out if present else None
SOLDIER_FIRST={0xAF65A,0xAF380,0xAFCE2,0xAFF16,0xB0396,0xAF2BA,0xAF302,0xAF7F6}
def extract(path,last,label):
    cap=Capture(path);calls=cap.rng_calls();slog=spawn_log(path)
    soldier_capture.FIXED=(Path(path)/'program-fixed.bin').read_bytes()
    files=sorted(int(p.stem[3:]) for p in Path(path).glob('ram*.bin'))
    ticks=tick_frames(cap,files[0],min(last,files[-1]))
    first=cap.frame(ticks[0]-1)
    out=dict(capture=label,first_frame=ticks[0],pc=first.l(SCRIPT+0x6E),timer72=first.w(SCRIPT+0x72),
        runners=runners(first),w4330=first.w(0x10C330),w4332=first.w(0x10C332),
        w69b8=first.w(0x10E9B8),w69ba=first.w(0x10E9BA),lock=first.l(0x10E9BC),ticks=[])
    for i,f in enumerate(ticks[:-1]):
        before,after=cap.frame(f-1),cap.frame(ticks[i+1]-1)
        rnodes={r['node'] for r in runners(before)}|{r['node'] for r in runners(after)}
        # Runner nodes created and deleted inside one update are found by
        # their creator field in the RNG log owner set as well.
        fcalls=[c for g in range(f,ticks[i+1]) for c in calls.get(g,[])]
        # The log names the caller (A6) of every ROM 0029C6 call in order; the
        # value is recovered by stepping the ring dumped before the update.
        words,cur=before.rng();mine=[]
        for node,_ in fcalls:
            v,cur=rng_step(words,cur)
            if node==SCRIPT or node in rnodes:mine.append(v)
        assert (words,cur)==after.rng(),'RNG ring not reproduced by the logged calls at %d'%f
        before_nodes={n for n,_ in before.actors()}
        spawned=[]
        if slog is not None:
            # Exact spawn list from the breakpoint log; soldier constructions
            # are matched to the new soldier nodes of each creator in order.
            fresh={}
            for n,rt in after.actors():
                if rt in SOLDIER_FIRST and not (n in before_nodes and before.l(n)!=DEAD):
                    fresh.setdefault(after.l(n+0x0C),[]).append(n)
            for kind,a6,rec in [e for g in range(f,ticks[i+1]) for e in slog.get(g,[])]:
                if kind!='SPAWN':continue
                creator_record=SCRIPT if a6==SCRIPT else before.l(a6+0x6E) if a6 in before_nodes and before.l(a6)==RUNNER else after.l(a6+0x6E)
                sp=dict(node=0,creator=a6,creator_record=creator_record,record=rec,group69=0,variant68=0)
                bank=cap.path/'program-bank.bin'
                entry=struct.unpack_from('>I',soldier_capture.FIXED,0x197DC+struct.unpack_from('>H',bank.read_bytes(),rec-0x200000+4)[0])[0]
                if entry==0xAF17E and fresh.get(a6):
                    # Each new node is inserted right after its creator, so the
                    # list holds a creator's spawns in reverse creation order.
                    n=fresh[a6].pop();sp.update(node=n,group69=after.b(n+0x69),variant68=after.b(n+0x68))
                    sp['construct']=construction(before,after,fcalls,n)
                spawned.append(sp)
        for n,rt in ([] if slog is not None else after.actors()):
            creator=after.l(n+0x0C)
            if rt==DEAD or (n in before_nodes and before.l(n)!=DEAD):continue
            if creator==SCRIPT or creator in rnodes:
                # Player projectiles and their impacts (ROM 01B000..01BFFF) are
                # linked with the script actor as creator but are not spawned
                # by the script.
                # Other actors (grenades 03BC28, prisoner items 08C374/08D330)
                # also carry the script actor as creator; only the states the
                # region's spawn-table constructors leave are script spawns.
                if rt not in FIRST_STATES:continue
                sp=dict(node=n,creator=creator,creator_record=SCRIPT if creator==SCRIPT else after.l(creator+0x6E),
                    group69=after.b(n+0x69),variant68=after.b(n+0x68))
                if 0xAE000<=rt<0xB1000:sp['construct']=construction(before,after,fcalls,n)
                spawned.append(sp)
        # The camera words are updated by an actor that precedes the script in
        # the list (ROM 0186A0 path), so the script reads this update's values.
        p1=before.l(PLAYER)!=DEAD;p2=before.l(0x101574)!=DEAD
        players=[dict(alive=before.l(n)!=DEAD,fx=before.b(n+0x7A),fy=before.b(n+0x7B),px=before.w(n+0x3C),py=before.w(n+0x40)) for n in (PLAYER,0x101574)]
        out['ticks'].append(dict(frame=f,camera=after.w(0x10E99E),scroll_x=after.l(0x10E97C),scroll_y=after.l(0x10E980),
            any_player=p1 or p2,players=players,groups=recount(before),rng=mine,
            expect=dict(pc=after.l(SCRIPT+0x6E),timer72=after.w(SCRIPT+0x72),runners=runners(after),lock=after.l(0x10E9BC),
                groups=list(after.raw(0x10C326,9)),spawned=spawned)))
    return out
def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('captures',nargs='+',help='DIR:LAST_FRAME')
    p.add_argument('--output',type=Path,required=True);a=p.parse_args()
    data=dict(source='MAME 0.264 mslugx euro; per-frame RAM dumps and print-only RNG log',captures=[])
    for spec in a.captures:
        path,last=spec.rsplit(':',1);c=extract(path,int(last),Path(path).name)
        c['debug_log_sha256']=hashlib.sha256((Path(path)/'debug.log').read_bytes()).hexdigest()
        data['captures'].append(c)
        print(c['capture'],len(c['ticks']),'updates',sum(len(t['expect']['spawned']) for t in c['ticks']),'spawns')
    with gzip.open(a.output,'wt',compresslevel=9) as f:f.write(json.dumps(data,separators=(',',':'))+'\n')
if __name__=='__main__':main()
