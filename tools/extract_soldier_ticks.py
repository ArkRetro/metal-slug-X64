#!/usr/bin/env python3
"""Per-update soldier evidence from per-frame RAM dumps (open-loop fixture).

For every original update of a rebel soldier (and its pistol death) inside the
playable region, keep the actor fields before and after the update, the scene
inputs it reads (terrain/root objects, epoch, players' projected positions and
hurtbox, delay grid, camera word) and the exact RNG ring at the start of its
update, reconstructed from the logged ROM 0029C6 calls of the actors that run
before it in the original list. Nothing is interpolated: a case whose inputs
cannot be established (missing dump, ambiguous order) is dropped and counted."""
import argparse,gzip,hashlib,json
from pathlib import Path
import soldier_capture
from soldier_capture import Capture,soldier,environment,rng_step,s32,PLAYER
SOLDIER=lambda rt:0xAE000<=rt<0xB1000 or rt in (0x40DB4,0x40DF0,0x40D06,0x40DC0,0x40AE0)
def entries(path):
    """ENTRY lines of tools/mame/trace_soldier_campaign.lua (MSX_SOLDIER_ENTRY=1):
    each soldier node's 0xB0 bytes at the start of its update, keyed like the
    RNG log by the video frame whose dump follows the update."""
    out={}
    for line in (Path(path)/'debug.log').read_text().splitlines():
        if not line.startswith('ENTRY'):continue
        t=line.split();f=int(t[1][2:])+1;node=int(t[2][3:],16);o=int(t[3][2:],16)
        b=out.setdefault((f,node),bytearray(0xb0))
        for i in range(4):b[o+4*i:o+4*i+4]=int(t[4+i],16).to_bytes(4,'big')
    return out
def tick_frames(cap,first,last):
    out=[];prev=cap.frame(first).b(0x10C33A)
    for f in range(first+1,last+1):
        e=cap.frame(f).b(0x10C33A)
        if e!=prev:out.append(f)
        prev=e
    return out
def order(before,after):
    """Original list order for the update: survivors keep their order; new
    nodes are inserted after their predecessor in the later list."""
    seq=[n for n,_ in before.actors()]
    later=[n for n,_ in after.actors()]
    for i,n in enumerate(later):
        if n not in seq:
            prev=later[i-1] if i else None
            seq.insert(seq.index(prev)+1 if prev in seq else len(seq),n)
    return seq
def extract(path,last,label,entry=False):
    cap=Capture(path);calls=cap.rng_calls();cases=[];dropped=0
    starts=entries(path) if entry else {}
    soldier_capture.FIXED=(Path(path)/'program-fixed.bin').read_bytes()
    files=sorted(int(p.stem[3:]) for p in Path(path).glob('ram*.bin'))
    ticks=tick_frames(cap,files[0],min(last,files[-1]))
    for f in ticks:
        if f-1 not in files or f+1 not in files or f+1 in ticks:continue
        # The update of a tick can overrun into the next video frame (lag):
        # the dump of the following, non-tick frame holds the complete result
        # and the RNG log continues under that frame number.
        before,after=cap.frame(f-1),cap.frame(f+1)
        seq=order(before,after);fcalls=calls.get(f,[])+calls.get(f+1,[])
        nodes_before={n:rt for n,rt in before.actors()};nodes_after={n:rt for n,rt in after.actors()}
        for node,rt in nodes_before.items():
            if not SOLDIER(rt) or rt==0xAF17E:continue
            # RNG: calls made by actors before this node in the update order.
            pos=seq.index(node);earlier=set(seq[:pos])
            mine=[c for c in fcalls if c[0]==node];prior=0
            for c in fcalls:
                if c[0]==node:break
                if c[0] in earlier or c[0] not in seq:prior+=1
                else:prior=None;break
            if prior is None:dropped+=1;continue
            words,cur=before.rng()
            for _ in range(prior):_,cur=rng_step(words,cur)
            # With the entry snapshots, the node's bytes as its update began
            # (a hit by an actor earlier in the same update included).
            start=starts.get((f,node)) or starts.get((f+1,node)) if entry else None
            if entry and start is None:dropped+=1;continue
            bnode=before
            if start is not None:
                d=bytearray(before.d);d[node-0x100000:node-0x100000+0xb0]=start;bnode=soldier_capture.Frame(bytes(d))
            env=environment(after);envb=environment(before)
            # --entry: an update while player 1 dies (its node alive, the
            # active mask RAM 10C33C clear) reads player inputs the harness
            # does not model: dropped and counted.
            if entry and envb['p1live'] and not envb['active']&1:dropped+=1;continue
            p1=dict(x=before.w(PLAYER+0x3C),y=before.w(PLAYER+0x40),flags15=before.b(PLAYER+0x15),
                descriptor=before.l(PLAYER+0x50),dead=bool(before.b(PLAYER+0x5F)&0xC0),live=before.l(PLAYER)!=0x3F1E)
            case=dict(capture=label,frame=f,node=node,before=soldier(bnode,node),
                after=soldier(after,node) if nodes_after.get(node) and SOLDIER(nodes_after[node]) else None,
                deleted=node not in nodes_after or not SOLDIER(nodes_after[node]),
                locals_before=list(bnode.raw(node+0x6E,0x26)),locals_after=list(after.raw(node+0x6E,0x26)) if node in nodes_after else None,
                rng=dict(words=words,cursor=cur,calls=len(mine)),
                scene=dict(terrain=env['terrain'],epoch=env['epoch'],camera=after.w(0x10E97C),
                    players=dict(p1x=envb['p1x'],p1y=envb['p1y'],p2x=envb['p2x'],p2y=envb['p2y'],
                        mask=(1 if envb['p1live'] else 0)|(2 if envb['p2live'] else 0),mode88=envb['mode88'],rank=envb['rank']),
                    grid=envb['grid'],grid_after=env['grid'],p1=p1,bounds=env['bounds']))
            # Player bullets (ROM 01B882) that run before/after this soldier.
            bl=[]
            for bnode,brt in nodes_before.items():
                if brt!=0x1B882:continue
                bl.append(dict(node=bnode,after_soldier=seq.index(bnode)>pos if bnode in seq else True,
                    body={k:before.l(bnode+o) for k,o in (('x',0x28),('y',0x2C),('vx',0x30),('vy',0x34),('parent',0x38),('px',0x3C),('py',0x40),('cx',0x44),('cy',0x48))},
                    flags15=before.b(bnode+0x15),flags5f=before.b(bnode+0x5F),mark=before.b(bnode+0x4C),correction=before.b(bnode+0x4D),
                    kind_angle=before.w(bnode+0x6C),f6f=before.b(bnode+0x6F),f5e=before.b(bnode+0x5E)))
            case['bullets']=bl
            # Damage by an attack the N64 port does not have (the player's knife,
            # kinds other than the pistol's 6) cannot be reproduced natively;
            # with the entry snapshot a hit before the update is in it (only
            # one after it, in the next state, is external).
            kinds={case['before']['timer5a']}|({case['after']['timer5a']} if case['after'] else set())
            if entry:kinds={case['after']['timer5a']} if case['after'] else set()
            case['external_attack']=bool(kinds-{0,6})
            if entry:case['entry']=True
            cases.append(case)
    return cases,dropped
def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('captures',nargs='+',help='DIR:LAST_FRAME')
    p.add_argument('--output',type=Path,required=True)
    p.add_argument('--entry',action='store_true',help='captures with MSX_SOLDIER_ENTRY=1: each update starts from the ENTRY bytes (cases without them are dropped)')
    p.add_argument('--only-deaths',action='store_true',help='keep only the updates that start dying (+5F bit 7 at the entry) or run a death routine (040500..040FFF)')
    a=p.parse_args()
    out=dict(source='MAME 0.264 mslugx euro; per-frame RAM dumps and print-only RNG log',captures={},cases=[])
    for spec in a.captures:
        path,last=spec.rsplit(':',1);label=Path(path).name
        cases,dropped=extract(path,int(last),label,a.entry)
        if a.only_deaths:cases=[c for c in cases if c['before']['flags5f']&0x80 or 0x40500<=c['before']['routine']<0x41000]
        out['captures'][label]=dict(last_frame=int(last),cases=len(cases),dropped_unordered=dropped,
            debug_log_sha256=hashlib.sha256((Path(path)/'debug.log').read_bytes()).hexdigest())
        out['cases']+=cases
    with gzip.open(a.output,'wt',compresslevel=9) as f:f.write(json.dumps(out,separators=(',',':'))+'\n')
    print(out['captures'])
if __name__=='__main__':main()
