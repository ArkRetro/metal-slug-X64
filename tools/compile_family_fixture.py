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
    p.add_argument('--spawn-window',type=int,help='frames after the creator in which a recorded child may first appear (default 8)')
    p.add_argument('--hit-moving',action='store_true',help='with --hit-to: the position may change in the update (the bomb 047B24 flies before its attack row)')
    p.add_argument('--hit-from',default='',help='ROUTINES (comma hex): the deleting updates that can leave a --hit-child (default: any)')
    p.add_argument('--hit-child',help='ROUTINE (hex): a node first seen with this routine and +0C = list head 101414 (00411C) is the impact of the node deleted last, at most 3 frames before and not yet matched (its creator); a deleting update hits (0049E8 answers a contact) only when it has such a child')
    p.add_argument('--script-actor',help='node address (hex) of the script actor: nodes it created (00411C after the script actor by a family node) get the creator -2, and the native code must create them with after_script')
    p.add_argument('--script-routines',default='',help='ROUTINES (comma hex): with --script-actor, only nodes with these first routines are made by family nodes (the others the script itself creates: injected)')
    p.add_argument('--marker-seq',type=int,help='RAM 10C446 (object-list sequence, 040110) before the first update when the trace lacks its read')
    p.add_argument('--terrain',help='layer object of the static terrain (hex, default 10D7B6: the families past camera 1023; 10C72E in the village)')
    p.add_argument('--node-fields',help='OFFSETS (comma hex): long fields holding a node address (the porter 0327EC keeps its item and the item\'s creator in +72/+76); compared through the node correspondence (every node\'s address is emitted)')
    p.add_argument('--label-after-pickup',help='ROUTINE (hex): with --script-actor, a node of this routine made by the script actor is native only when it follows a pickup of the fixture (one per pickup, at most 2 frames later, at the item\'s projected position): the score label 08D330 of 08B944; the labels other actors make are injected')
    p.add_argument('--shared',help='ROUTINES (comma hex): nodes whose fields another family node writes between their own updates (the SV-001 writes its tread node 014A4E in 014C7E..0153D0): no external bytes are injected, the native writes are compared at the node\'s next update')
    p.add_argument('--late-creator',action='store_true',help='a node first seen in the frame its creator was deleted, or the next (a program callback spawning on its FINISH update: the creator stays a placeholder, 003F1E) keeps that creator')
    p.add_argument('--attacks',action='store_true',help='the 0049E8 answers of every update (extract_family_fixture.py --attacks): D0 and the A0/A1 targets as 01B162 read them (rectangle from data/region_rom.json); a target the update did not read gets 7FFF,7FFF and no box')
    p.add_argument('--ring',help='LAYER (hex): the kernel reads the cell ring of this layer object (+88) from the RAM bytes the original read (extract with --ram covering it)')
    p.add_argument('--train',action='store_true',help='Mission 3: the train layer 10D7B6 (+14..+20 as read) is the second terrain object of the kernel, its cells from the RAM ring 10D83E (extract with --ram 10d83e-10e83e)')
    p.add_argument('--group-map',help='ADDRESS (hex): the cell group map RAM 10C00E points to (Mission 2: 0B7000; default the Mission 1 map)')
    p.add_argument('--grid',type=Path,help='stage collision grid (collision.json of a mission stage, private): the static grid of the test instead of Mission 1 (neo_collision_set_grid)')
    p.add_argument('--resume',action='store_true',help='a trace started from a saved state: the nodes alive before it start from their recorded bytes, creator and program (as --m2boss, without its RAM)')
    p.add_argument('--m2boss',action='store_true',help='the Mission 2 boss 026254 (neo_m2boss.c): RAM 10E99A/10E9BA/1013D6/10C354/10008A/10F009 through neo_m2boss_env_t (extract with --ram covering them); nodes alive before the piece keep their creator')
    p.add_argument('--m3mid',action='store_true',help='the Mission 3 mid-boss 0A4A94 (neo_m3mid.c): its layer object 10E8C6, RAM 10E959, 1063D8 and the shake 10E9C8 from the RAM image (extract with --ram covering them); pieces resume like --m2boss')
    p.add_argument('--root-words',action='store_true',help='a single word read of the root\'s +14..+20 (01AD7C) updates it with the other word as last known')
    p.add_argument('--split-children',action='store_true',help='the children one update makes may split over two frames (the frame mark inside the list walk, as --m2boss): the first made is the last of them in the list')
    p.add_argument('--fresh-program',action='store_true',help='a constructor that steps the program (00427A) before starting one: the original steps the previous occupant\'s program (+20), the world\'s fresh node has none, so +24..+27 are not compared in a node\'s first update without a program')
    p.add_argument('--reserve',action='store_true',help='0040BC refuses a node unless more than its reserve are free: RAM 1063D8 as the update first read it (extract with --ram 1063d8-1063da); the world has no such limit')
    p.add_argument('--soldier',help='ROUTINES (comma hex): an update of one of these routines ends with JMP 0B0626 / 0B0632 (the node becomes a soldier): the native code must keep the node (the world moves it), its bytes are not compared, and the node\'s later updates and the nodes it creates from then on (the soldier\'s own code) are dropped until the node is deleted')
    p.add_argument('--external-state',default='',help='ROUTINES (comma hex): states another node of the family sets on a node between its updates (the jeep gunner 0A39D6 sets its creator to 0A35A8): an update of such a routine goes on with the same node instance')
    p.add_argument('--ram-creators',action='store_true',help='a node whose creator (+0C) is a node outside the family (the player 1014C4, a gate) reads that creator from the RAM image (extract with --ram covering the node pool)')
    p.add_argument('--previous',action='store_true',help='per update, the node before it in the list (+04 of its bytes), as a node of the fixture (-1: none)')
    a=p.parse_args();n=a.name
    ext_states=[int(x,16) for x in a.external_state.split(',')] if a.external_state else []
    shared=[int(x,16) for x in a.shared.split(',')] if a.shared else []
    script=int(a.script_actor,16) if a.script_actor else None
    used=[]
    d=json.load(gzip.open(a.fixture,'rt'))
    # A node deleted while deeper nodes follow it stays as a placeholder
    # (routine 003F1E, 004126): a deleting update like any other.
    for u in d['updates']:
        if u['post'] and u['post'][:8]=='00003f1e':u['post']=None
    # A node instance lives from its first update to its deleting update; a
    # later node at the same address is another instance.
    # A node deleted without a traced update (a creator's tree deletion,
    # 0041AC) shows up again with a routine its last update did not leave.
    pickups=[(u['frame'],u['post'][2*0x3c:2*0x44]) for u in d['updates'] if u['post'] and u['post'][:8]=='0008c66a' and u['routine']!=0x8c66a]
    def instances(updates):
        used.clear()
        live={};order=[];inst=[];lastroutine={};dead=[];claimed=set();gone={}
        for u in updates:
            if u['node'] in live and lastroutine.get(u['node'])!=u['routine'] and u['routine'] not in ext_states:del live[u['node']]
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
                if c<0 and script is not None and int(cr,16)==script and a.label_after_pickup and u['routine']==int(a.label_after_pickup,16):
                    # The score label 08D330 an item's pickup makes (08B944):
                    # one per pickup of the two frames before; the labels
                    # other actors make (08CE72/08CEEC) are injected.
                    # 01ACF2 placed it on the item: its projected words are
                    # the item's after the pickup update.
                    near=[q for q in pickups if u['frame']-2<=q[0]<=u['frame'] and q not in used and q[1]==u['pre'][2*0x3c:2*0x44]]
                    if near:used.append(near[0]);c=-2
                live[u['node']]=len(order);order.append((u,c))
            inst.append(live[u['node']])
            if not u['post'] and (not a.hit_from or u['routine'] in [int(x,16) for x in a.hit_from.split(',')]):
                b=bytes.fromhex(u['pre']);sx=lambda o:int.from_bytes(b[o:o+2],'big')
                # Projected words after the move (y up); a constructor's
                # update has no position yet: matched last.
                dead.append((live[u['node']],u['frame'],(sx(0x3c)+sx(0x30))&0xffff,(sx(0x40)+sx(0x34))&0xffff) if sx(0x3c) else (live[u['node']],u['frame'],None,None))
            if not u['post']:gone[u['node']]=(live[u['node']],u['frame']);del live[u['node']]
        return order,inst
    if a.soldier:
        sold=[int(x,16) for x in a.soldier.split(',')]
        leaving=set();alive=set();kept=[]
        for u in d['updates']:
            nd=u['node']
            if nd in leaving:
                if not u['post']:leaving.discard(nd);alive.discard(nd)
                continue
            pre=bytes.fromhex(u['pre']);cr='%06x'%int.from_bytes(pre[0x0c:0x10],'big')
            if nd not in alive and cr in leaving:
                # Made by the soldier (its ripple 03C62A): dropped with it.
                leaving.add(nd)
                if u['post']:alive.add(nd)
                continue
            if u['routine'] in sold:
                # The entry moves the record (0B05FE as a state: no V line).
                u['leave']=True;leaving.add(nd)
            if u['post']:alive.add(nd)
            else:alive.discard(nd)
            kept.append(u)
        d['updates']=kept
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
    if a.soldier:
        lv=[(u['frame'],inst[k]) for k,u in enumerate(d['updates']) if u.get('leave')]
        assert lv,'no update of --soldier in the fixture'
        L.append('#define FAMILY_LEAVE {%s}'%','.join('{%d,%d}'%x for x in lv))
    if a.pickup:L.append('#define FAMILY_PICKUPS {%s}'%','.join('{%d,0x%x,0x%x}'%tuple(int(x,0 if i==0 else 16) for i,x in enumerate(t.split(','))) for t in a.pickup))
    if a.spawn_window is not None:L.append('#define FAMILY_SPAWN_WINDOW %du'%a.spawn_window)
    if a.hit_child:L.append('#define FAMILY_HIT_CHILD 0x%x'%int(a.hit_child,16))
    if a.hit_to:
        hits=sorted({(u['frame'],inst[k]) for k,u in enumerate(d['updates']) if u['post'] and int(u['post'][:8],16)==int(a.hit_to,16) and int(u['pre'][:8],16)!=int(a.hit_to,16) and (a.hit_moving or u['post'][2*0x3c:2*0x44]==u['pre'][2*0x3c:2*0x44])})
        L.append('#define FAMILY_HIT_UPDATES {%s}'%','.join('{%d,%d}'%h for h in hits))
    if a.node_fields:
        L.append('#define FAMILY_NODE_FIELDS {%s}'%','.join('0x%x'%int(x,16) for x in a.node_fields.split(',')))
        L.append('#define FAMILY_ADDRESSES {%s}'%','.join('0x%s'%u['node'] for u,_ in order))
    if a.terrain:L.append('#define FAMILY_TERRAIN 0x%xu'%int(a.terrain,16))
    if a.ring:L.append('#define FAMILY_RING 0x%xu'%int(a.ring,16))
    if a.m2boss:L.append('#define FAMILY_M2BOSS 1')
    if a.split_children:L.append('#define FAMILY_SPLIT_CHILDREN 1')
    if a.fresh_program:L.append('#define FAMILY_FRESH_PROGRAM 1')
    if a.reserve:L.append('#define FAMILY_RESERVE 1')
    if a.m3mid:L.append('#define FAMILY_M3MID 1')
    if a.resume:L.append('#define FAMILY_RESUME 1')
    if a.train:L.append('#define FAMILY_TRAIN 1')
    if a.ram_creators:L.append('#define FAMILY_RAM_CREATORS 1')
    if a.group_map:L.append('#define FAMILY_GROUP_MAP 0x%xu'%int(a.group_map,16))
    if a.grid:
        g=json.load(open(a.grid));cells=bytes.fromhex(g['cells_hex']);assert len(cells)==g['columns']*g['rows']
        L.append('#define FAMILY_GRID_COLUMNS %du'%g['columns']);L.append('#define FAMILY_GRID_ROWS %du'%g['rows'])
        L.append('static const uint8_t family_grid[]={%s};'%','.join(str(c) for c in cells))
    if a.marker_seq is not None:L.append('#define FAMILY_MARKER_SEQ %d'%a.marker_seq)
    if a.stamp:
        st=[tuple(int(x,16) for x in t.split(',')) for t in a.stamp]
        L.append('#define FAMILY_STAMPS {%s}'%','.join('{0x%x,0x%x,0x%x}'%t for t in st))
    L+=[
       'static const family_node_t %s_nodes[]={'%n]
    for u,cr in order:
        pre=bytes.fromhex(u['pre'])
        L.append('{%d,0x%x,%d,{%s}},'%(cr,u['routine'],u['frame'],','.join(str(x) for x in pre[:0xb0])))
    L.append('};')
    # RAM the update read (first read of each byte) and wrote, in the --ram
    # intervals of the extraction (vehicle slots, input words, weapon block,
    # player nodes): inputs and compared outputs.
    rin=[];rout=[];spans=[]
    for u in d['updates']:
        i0,o0=len(rin),len(rout)
        rin+=[(int(k,16),v) for k,v in sorted(u.get('ram_in',{}).items())]
        rout+=[(ad,v) for ad,v in u.get('ram_out',[])]
        spans.append((i0,len(rin)-i0,o0,len(rout)-o0))
    if d.get('ram'):
        L.append('#define FAMILY_RAM {%s}'%','.join('{0x%s,0x%s}'%tuple(r.split('-')) for r in d['ram']))
        L.append('static const uint32_t %s_ram_in[][2]={%s};'%(n,','.join('{0x%x,%d}'%x for x in rin) or '{0,0}'))
        L.append('static const uint32_t %s_ram_out[][2]={%s};'%(n,','.join('{0x%x,%d}'%x for x in rout) or '{0,0}'))
    # --train: the train object 10D7B6 +14..+20 (x, y, previous x, y) as
    # read, per update: mask (bit i: long i read) and the four longs.
    if a.train:
        tr=[]
        for u in d['updates']:
            w=[rd(u,x) for x in range(0x10d7ca,0x10d7da,2)];m=0;v=[0,0,0,0]
            for i in range(4):
                if w[2*i] is not None and w[2*i+1] is not None:m|=1<<i;v[i]=s32(w[2*i]<<16|w[2*i+1])
            tr.append((m,*v))
        L.append('static const int32_t %s_train[][5]={%s};'%(n,','.join('{%d,%d,%d,%d,%d}'%t for t in tr)))
    # Score-label counts RAM 10C5B2/10C5B4 (08D330) as read: inputs.
    lc=[(k,rd(u,0x10c5b2),rd(u,0x10c5b4)) for k,u in enumerate(d['updates'])]
    lc=[(k,x if x is not None else -1,y if y is not None else -1) for k,x,y in lc if x is not None or y is not None]
    if lc and a.label_after_pickup:
        L.append('#define FAMILY_LABEL_COUNTS 1')
        L.append('static const int32_t %s_label_counts[][3]={%s};'%(n,','.join('{%d,%d,%d}'%t for t in lc)))
    # Writes to the watched globals (extract_family_fixture.py GLOBALS), as
    # (address, byte) pairs: per update a span of <name>_writes.
    gw=[];wspans=[]
    for u in d['updates']:
        w0=len(gw)
        for ad,mask,data,pc in u['writes']:
            if mask&0xff00:gw.append((ad,(data>>8)&255))
            if mask&0x00ff:gw.append((ad+1,data&255))
        wspans.append((w0,len(gw)-w0))
    L.append('static const uint32_t %s_writes[][2]={%s};'%(n,','.join('{0x%x,%d}'%x for x in gw) or '{0,0}'))
    if d.get('globals',1)>=2:L.append('#define FAMILY_EXIT_GLOBALS 1')
    L.append('static const family_update_t %s_updates[]={'%n)
    last={}
    maxext=0
    root_last=[None]*4
    for k,u in enumerate(d['updates']):
        post=bytes.fromhex(u['post']) if u['post'] else bytes(0xb0)
        # Bytes other actors (the player's shots, 0049E8) changed in the node
        # since its previous update: inputs of this update.
        ext=[]
        prev=last.get(inst[k])
        if prev is not None and u['routine'] not in shared:
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
            # A word read alone (01AD7C adds the root's +14/+18 words): the
            # other word as last known.
            elif a.root_words and (root[2*i] is not None or root[2*i+1] is not None) and root_last[i] is not None:
                hi=root[2*i] if root[2*i] is not None else (root_last[i]>>16)&0xffff
                lo=root[2*i+1] if root[2*i+1] is not None else root_last[i]&0xffff
                rmask|=1<<i;rv[i]=s32(hi<<16|lo)
            if rmask>>i&1:root_last[i]=rv[i]&0xffffffff
        corr=rd(u,0x10e98c)
        # Layer object 10D7B6 (+14..+24) when the trace has its reads (older
        # fixtures: same position as the root, checked in the RAM dumps).
        # --terrain 10c72e: the village object 10C72E (+14..+24) instead.
        tb=0x10c742 if a.terrain and int(a.terrain,16)==0x10c72e else 0x10d7ca
        ter=[rd(u,x) for x in range(tb,tb+0x10,2)]
        tmask=0;tv=[0,0,0,0]
        for i in range(4):
            if ter[2*i] is not None and ter[2*i+1] is not None:tmask|=1<<i;tv[i]=s32(ter[2*i]<<16|ter[2*i+1])
        tcorr=rd(u,tb+0x10)
        if tcorr is not None:tmask|=16
        cur=None
        if hex(RING1) in u['reads'] and hex(RING1+2) in u['reads']:
            ptr=u['reads'][hex(RING1)][1]<<16|u['reads'][hex(RING1+2)][1];cur=(ptr-RING0)//2
        ring=[((int(k,16)-RING0)//2,v[1]) for k,v in u['reads'].items() if RING0<=int(k,16)<RING1]
        assert len(ring)<=64,u['frame']
        ring+= [(255,0)]*(64-len(ring))
        b954,b95c=rd(u,0x10b954),rd(u,0x10b95c)
        front=-1 if b954 is None or b95c is None else int((b954^0x8000)<(b95c^0x8000))
        def opt(v):return -1 if v is None else v
        def solid_busy(u):
            # Solid-body table 106756 (16 entries of 8 bytes) as 004E76 read
            # it: bit i set for a used entry; -1 when the update read none.
            m=0;seen=False
            for i in range(16):
                for w in (0,2):
                    v=u['reads'].get(hex(0x106756+8*i+w))
                    if v is not None:
                        seen=True
                        if v[1]:m|=1<<i
            return m if seen else -1
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
        L.append('{%d,%d,0x%x,%d,{%s},%d,{%d,%d,%d,%d},%d,%d,{%d,%d,%d,%d},%d,%d,%d,%d,%d,%d,%d,%d,%d,{%s},{%s},{%s},%d,%d,{%s},{%s},%d,%d,%d,{%s},%d,%d,%d,%d,{%d,%d,%d,%d},%d,%d},'%(
            u['frame'],inst[k],u['routine'],1 if u['post'] else 0,','.join(str(x) for x in post[:0xb0]),
            rmask,*rv,opt(corr),tmask,*tv,opt(tcorr),opt(rd(u,0x101500)),opt(rd(u,0x101504)),opt(rd(u,0x10c44e)),-1 if hex(0x10c44e) not in u['reads'] else u['reads'][hex(0x10c44e)][1]&255,front,opt(rd(u,0x10c33a)),
            -1 if cur is None else cur,progress,','.join(map(str,groups)),','.join('{%d,%d,%d}'%x for x in dyn),','.join('{%d,%d}'%r for r in ring),*wspans[k],','.join('{%d,%d}'%x for x in ext),','.join(map(str,boxes)),opt(kill),opt(rd(u,0x10c068)),nm,','.join('{%d,%d,%d,%d}'%m for m in marks),*spans[k],*[opt(rd(u,0x10e9d2+2*i)) for i in range(4)],opt(rd(u,0x101502)),solid_busy(u)))
    L.append('};')
    if a.attacks:
        rom={}
        for b in json.load(open(Path(__file__).resolve().parent.parent/'data'/'region_rom.json'))['blobs']:
            base=int(b['rom'],16);data=bytes.fromhex(b['hex'])
            for i,x in enumerate(data):rom[base+i]=x
        def rect(ptr):
            if not ptr:return (0,0,0,0,0)
            w=[]
            for o in range(2,10,2):
                assert ptr+o in rom and ptr+o+1 in rom,'descriptor %x not in region_rom.json'%ptr
                v=rom[ptr+o]<<8|rom[ptr+o+1];w.append(v-0x10000 if v&0x8000 else v)
            return (1,*w)
        rows=[];spans=[]
        for u in d['updates']:
            f=len(rows)
            for typ,a0,a1 in u.get('attacks',[]):
                row=[typ]
                for node in (a0,a1):
                    tg=u.get('targets',{}).get('%x'%node) if node else None
                    lo=lambda k:(tg[k] if tg and len(tg)>k else 0)
                    if not node:row+=[0,0,0,0,0,0,0,0,0,0]
                    elif tg is None or tg[0] is None or tg[1] is None:
                        row+=[1,tg[0] if tg and tg[0] is not None else 0x7fff,tg[1] if tg and tg[1] is not None else 0x7fff,0,0,0,0,0,lo(3),lo(4)]
                    else:row+=[1,tg[0],tg[1],*rect(tg[2]),lo(3),lo(4)]
                rows.append(row)
            spans.append((f,len(rows)-f))
        L.append('#define FAMILY_ATTACKS 1')
        L.append('static const int32_t %s_attacks[][21]={%s};'%(n,','.join('{%s}'%','.join(map(str,r)) for r in rows) or '{0}'))
        L.append('static const uint16_t %s_attack_spans[][2]={%s};'%(n,','.join('{%d,%d}'%x for x in spans)))
    if any(u.get('hitw') for u in d['updates']):
        where={};rows=[]
        for k,u in enumerate(d['updates']):
            where[u['node']]=inst[k]
            for nd,off,val in u.get('hitw',[]):rows.append((k,where.get(nd,where.get(nd.zfill(6),-1)),off,val))
        L.append('#define FAMILY_HITW 1')
        L.append('static const int32_t %s_hitw[][4]={%s};'%(n,','.join('{%d,%d,%d,%d}'%r for r in rows)))
    if a.previous:
        where={};prev=[]
        for k,u in enumerate(d['updates']):
            where[u['node']]=inst[k]
            pv='%06x'%int(u['pre'][8:16],16)
            prev.append(where.get(pv,-1))
        L.append('#define FAMILY_PREVIOUS 1')
        L.append('static const int16_t %s_previous[]={%s};'%(n,','.join(map(str,prev))))
    a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_text('\n'.join(L)+'\n')
    print(len(order),'node instances',len(d['updates']),'updates, at most',maxext,'external bytes per update')
if __name__=='__main__':main()
