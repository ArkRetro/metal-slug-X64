#!/usr/bin/env python3
"""Compile a family fixture (tools/extract_family_fixture.py) into a C header
for tools/test_neo_family.c.

Nodes are numbered in order of first appearance; each keeps its creator
(+0C, another node of the fixture or -1) and its first routine, and the node
bytes of its first update (the allocator's fields plus what the creator
wrote before it ran). Per update: the inputs the original read (scene root
10E968 block (the layer objects hold the same position), player 1 +3C/+40, script flags 10C44E/10C44F,
layer words 10B954/10B95C, epoch 10C33A, RNG cursor and ring words) and the
writes to the watched globals as (address, byte) pairs."""
import argparse,gzip,json
from pathlib import Path
RING0,RING1=0x100016,0x100084
def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('fixture',type=Path);p.add_argument('--output',type=Path,required=True);p.add_argument('--name',required=True)
    p.add_argument('--tree',action='store_true',help='keep only the first node and its descendants')
    p.add_argument('--stop',help='FRAME,ROUTINE (hex routine): the update of that frame must stop explicitly at ROUTINE (an untranslated service); later updates are dropped')
    p.add_argument('--stamp',action='append',default=[],help='X,Y,TEMPLATE (hex): collision stamp (0C88B0) applied before the first update (an event of another family seen in the RAM dumps)')
    p.add_argument('--pickup',action='append',default=[],help='FRAME,ID,ROUNDS (hex id/rounds): the item taken at FRAME must leave player 1 with weapon ID and ROUNDS (the pickup state of neo_item.c, from a new life)')
    p.add_argument('--hit-to',help='ROUTINE (hex): an update that ends in this routine without moving (+3C..+43 unchanged: the grenade exploding on 0049E8 before its flight) answers a contact')
    p.add_argument('--hit-moving',action='store_true',help='with --hit-to: the position may change in the update (the bomb 047B24 flies before its attack row)')
    p.add_argument('--hit-from',default='',help='ROUTINES (comma hex): the deleting updates that can leave a --hit-child (default: any)')
    p.add_argument('--hit-child',help='ROUTINE (hex): a node first seen with this routine and +0C = list head 101414 (00411C) is the impact of the node deleted last, at most 3 frames before and not yet matched (its creator); a deleting update hits (0049E8 answers a contact) only when it has such a child')
    p.add_argument('--script-actor',help='node address (hex) of the script actor: nodes it created (00411C after the script actor by a family node) get the creator -2, and the native code must create them with after_script')
    p.add_argument('--script-routines',default='',help='ROUTINES (comma hex): with --script-actor, only nodes with these first routines are made by family nodes (the others the script itself creates: injected)')
    p.add_argument('--marker-seq',type=int,help='RAM 10C446 (object-list sequence, 040110) before the first update when the trace lacks its read')
    p.add_argument('--late-creator',action='store_true',help='a node first seen in the frame its creator was deleted, or the next (a program callback spawning on its FINISH update: the creator stays a placeholder, 003F1E) keeps that creator')
    a=p.parse_args();n=a.name
    script=int(a.script_actor,16) if a.script_actor else None
    d=json.load(gzip.open(a.fixture,'rt'))
    # A node deleted while deeper nodes follow it stays as a placeholder
    # (routine 003F1E, 004126): a deleting update like any other.
    for u in d['updates']:
        if u['post'] and u['post'][:8]=='00003f1e':u['post']=None
    # A node instance lives from its first update to its deleting update; a
    # later node at the same address is another instance.
    # A node deleted without a traced update (a creator's tree deletion,
    # 0041AC) shows up again with a routine its last update did not leave.
    def instances(updates):
        live={};order=[];inst=[];lastroutine={};dead=[];claimed=set();gone={}
        for u in updates:
            if u['node'] in live and lastroutine.get(u['node'])!=u['routine']:del live[u['node']]
            if u['post']:lastroutine[u['node']]=int(u['post'][:8],16)
            if u['node'] not in live:
                pre=bytes.fromhex(u['pre']);cr='%06x'%int.from_bytes(pre[0x0c:0x10],'big')
                c=live.get(cr,-1)
                if c<0 and a.late_creator and cr in gone and gone[cr][1]>=u['frame']-1:c=gone[cr][0]
                if a.hit_child and u['routine']==int(a.hit_child,16) and cr=='101414':
                    # 00411C inserts at the list head: no link names the
                    # creator, already gone (the impact's first update can
                    # come one update after the bullet's last).
                    # The nearest to where the dead node's move took it.
                    ix=int.from_bytes(pre[0x3c:0x3e],'big');iy=int.from_bytes(pre[0x40:0x42],'big')
                    cand=[x for x in dead if x[1]>=u['frame']-3 and x[0] not in claimed]
                    if cand:
                        cost=lambda x:0x4000 if x[2] is None else abs(((ix-x[2]+0x8000)&0xffff)-0x8000)+abs(((iy-x[3]+0x8000)&0xffff)-0x8000)
                        c=min(cand,key=cost)[0];claimed.add(c)
                if c<0 and script is not None and int(cr,16)==script and (not a.script_routines or u['routine'] in [int(x,16) for x in a.script_routines.split(',')]):c=-2
                live[u['node']]=len(order);order.append((u,c))
            inst.append(live[u['node']])
            if not u['post'] and (not a.hit_from or u['routine'] in [int(x,16) for x in a.hit_from.split(',')]):
                b=bytes.fromhex(u['pre']);sx=lambda o:int.from_bytes(b[o:o+2],'big')
                # Projected words after the move (y up); a constructor's
                # update has no position yet: matched last.
                dead.append((live[u['node']],u['frame'],(sx(0x3c)+sx(0x30))&0xffff,(sx(0x40)+sx(0x34))&0xffff) if sx(0x3c) else (live[u['node']],u['frame'],None,None))
            if not u['post']:gone[u['node']]=(live[u['node']],u['frame']);del live[u['node']]
        return order,inst
    if a.stop:
        sf,sr=a.stop.split(',');sf=int(sf)
        d['updates']=[u for u in d['updates'] if u['frame']<=sf]
    order,inst=instances(d['updates'])
    if a.tree:
        # Only the first node and its descendants (by creator): nodes the
        # traced family's creators outside it made belong to another test.
        kept=set()
        for i,(u,cr) in enumerate(order):
            if i==0 or cr in kept or cr==-2:kept.add(i)
        d['updates']=[u for u,i in zip(d['updates'],inst) if i in kept]
        order,inst=instances(d['updates'])
    def rd(u,ad):
        v=u['reads'].get(hex(ad))
        if v is None:return None
        mask,data=v[0],v[1]
        return (data>>8)&255 if mask==0xff00 else data&255 if mask==0xff else data
    def s32(v):return v-(1<<32) if v>=1<<31 else v
    L=['/* Generated by tools/compile_family_fixture.py from %s; do not edit. */'%a.fixture.name,'#include <stdint.h>']
    if a.stop:L.append('#define FAMILY_STOP {%d,0x%x}'%(sf,int(sr,16)))
    if a.pickup:L.append('#define FAMILY_PICKUPS {%s}'%','.join('{%d,0x%x,0x%x}'%tuple(int(x,0 if i==0 else 16) for i,x in enumerate(t.split(','))) for t in a.pickup))
    if a.hit_child:L.append('#define FAMILY_HIT_CHILD 0x%x'%int(a.hit_child,16))
    if a.hit_to:
        hits=sorted({(u['frame'],inst[k]) for k,u in enumerate(d['updates']) if u['post'] and int(u['post'][:8],16)==int(a.hit_to,16) and int(u['pre'][:8],16)!=int(a.hit_to,16) and (a.hit_moving or u['post'][2*0x3c:2*0x44]==u['pre'][2*0x3c:2*0x44])})
        L.append('#define FAMILY_HIT_UPDATES {%s}'%','.join('{%d,%d}'%h for h in hits))
    if a.marker_seq is not None:L.append('#define FAMILY_MARKER_SEQ %d'%a.marker_seq)
    if a.stamp:
        st=[tuple(int(x,16) for x in t.split(',')) for t in a.stamp]
        L.append('#define FAMILY_STAMPS {%s}'%','.join('{0x%x,0x%x,0x%x}'%t for t in st))
    L+=[
       'static const family_node_t %s_nodes[]={'%n]
    for u,cr in order:
        pre=bytes.fromhex(u['pre'])
        L.append('{%d,0x%x,%d,{%s}},'%(cr,u['routine'],u['frame'],','.join(str(x) for x in pre[:0xb0])))
    L.append('};');L.append('static const family_update_t %s_updates[]={'%n)
    last={}
    maxext=0
    for k,u in enumerate(d['updates']):
        post=bytes.fromhex(u['post']) if u['post'] else bytes(0xb0)
        # Bytes other actors (the player's shots, 0049E8) changed in the node
        # since its previous update: inputs of this update.
        ext=[]
        prev=last.get(inst[k])
        if prev is not None:
            pre0=bytes.fromhex(u['pre'])
            ext=[(o,pre0[o]) for o in range(0x10,0xb0) if pre0[o]!=prev[o] and not (0x20<=o<0x24 or 0x44<=o<0x4c)]
        if u['post']:last[inst[k]]=post
        maxext=max(maxext,len(ext))
        assert len(ext)<=16,(u['frame'],ext)
        ext+=[(0,0)]*(16-len(ext))
        pre=bytes.fromhex(u['pre'])
        root=[rd(u,x) for x in (0x10e97c,0x10e97e,0x10e980,0x10e982,0x10e984,0x10e986,0x10e988,0x10e98a)]
        rmask=0;rv=[0,0,0,0]
        for i in range(4):
            if root[2*i] is not None and root[2*i+1] is not None:rmask|=1<<i;rv[i]=s32(root[2*i]<<16|root[2*i+1])
        corr=rd(u,0x10e98c)
        # Layer object 10D7B6 (+14..+24) when the trace has its reads (older
        # fixtures: same position as the root, checked in the RAM dumps).
        ter=[rd(u,x) for x in range(0x10d7ca,0x10d7da,2)]
        tmask=0;tv=[0,0,0,0]
        for i in range(4):
            if ter[2*i] is not None and ter[2*i+1] is not None:tmask|=1<<i;tv[i]=s32(ter[2*i]<<16|ter[2*i+1])
        tcorr=rd(u,0x10d7da)
        if tcorr is not None:tmask|=16
        cur=None
        if hex(RING1) in u['reads'] and hex(RING1+2) in u['reads']:
            ptr=u['reads'][hex(RING1)][1]<<16|u['reads'][hex(RING1+2)][1];cur=(ptr-RING0)//2
        ring=[((int(k,16)-RING0)//2,v[1]) for k,v in u['reads'].items() if RING0<=int(k,16)<RING1]
        assert len(ring)<=64,u['frame']
        ring+= [(255,0)]*(64-len(ring))
        b954,b95c=rd(u,0x10b954),rd(u,0x10b95c)
        front=-1 if b954 is None or b95c is None else int((b954^0x8000)<(b95c^0x8000))
        writes=[]
        for ad,mask,data,pc in u['writes']:
            if mask&0xff00:writes.append((ad,(data>>8)&255))
            if mask&0x00ff:writes.append((ad+1,data&255))
        assert len(writes)<=12,u['frame']
        writes+=[(0,0)]*(12-len(writes))
        def opt(v):return -1 if v is None else v
        # Group live counts (RAM 10C326.., bytes) and camera progress 10E99E.
        groups=[]
        for gi in range(9):
            v=u['reads'].get(hex(0x10c326+(gi&~1)))
            if v is None or not (v[0]&(0xff00 if gi%2==0 else 0xff)):groups.append(-1)
            else:groups.append((v[1]>>8)&255 if gi%2==0 else v[1]&255)
        progress=opt(rd(u,0x10e99e))
        # Item pickup boxes of the players (RAM 10C5EC/10C5F4, x,y,w,h).
        boxes=[]
        for i in range(8):
            v=rd(u,0x10c5ec+2*i);boxes.append(-1 if v is None else v)
        # Dynamic collision objects 10C06E..10C291 as read (offset, mask, word).
        dyn=[(int(k,16)-0x10c06e,v[0],v[1]) for k,v in u['reads'].items() if 0x10c06e<=int(k,16)<0x10c292]
        assert len(dyn)<=48,(u['frame'],len(dyn))
        dyn+=[(0xffff,0,0)]*(48-len(dyn))
        # RAM 1067E2 (kill-all, 004BAC) as read: set by another family.
        kill=rd(u,0x1067e2)
        # Object list (markers, 040200/0402E6) as read: list 10C3A2 (count
        # word 10C3F2) or 10C3F4 (count 10C444), 10-byte entries x.l, y.l,
        # height.b, id.b. Entries the query did not reach stay 0.
        marks=[]
        for base in (0x10c3a2,0x10c3f4):
            cnt=rd(u,base+0x50)
            if cnt is None:continue
            assert cnt%10==0 and cnt<=80,(u['frame'],cnt)
            for i in range(cnt//10):
                wv=[(lambda v:0 if v is None else v[1]&((0xff00 if v[0]&0xff00 else 0)|(0xff if v[0]&0xff else 0)))(u['reads'].get(hex(base+10*i+2*j))) for j in range(5)]
                marks.append((s32(wv[0]<<16|wv[1]),s32(wv[2]<<16|wv[3]),wv[4]>>8,wv[4]&255))
            break
        nm=len(marks);marks+=[(0,0,0,0)]*(8-len(marks))
        L.append('{%d,%d,0x%x,%d,{%s},%d,{%d,%d,%d,%d},%d,%d,{%d,%d,%d,%d},%d,%d,%d,%d,%d,%d,%d,%d,%d,{%s},{%s},{%s},{%s},{%s},{%s},%d,%d,%d,{%s}},'%(
            u['frame'],inst[k],u['routine'],1 if u['post'] else 0,','.join(str(x) for x in post[:0xb0]),
            rmask,*rv,opt(corr),tmask,*tv,opt(tcorr),opt(rd(u,0x101500)),opt(rd(u,0x101504)),opt(rd(u,0x10c44e)),-1 if hex(0x10c44e) not in u['reads'] else u['reads'][hex(0x10c44e)][1]&255,front,opt(rd(u,0x10c33a)),
            -1 if cur is None else cur,progress,','.join(map(str,groups)),','.join('{%d,%d,%d}'%x for x in dyn),','.join('{%d,%d}'%r for r in ring),','.join('{0x%x,%d}'%w for w in writes),','.join('{%d,%d}'%x for x in ext),','.join(map(str,boxes)),opt(kill),opt(rd(u,0x10c068)),nm,','.join('{%d,%d,%d,%d}'%m for m in marks)))
    L.append('};')
    a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_text('\n'.join(L)+'\n')
    print(len(order),'node instances',len(d['updates']),'updates, at most',maxext,'external bytes per update')
if __name__=='__main__':main()
