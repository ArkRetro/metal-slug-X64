#!/usr/bin/env python3
"""Build a fight fixture for the native boss (tools/test_neo_boss.c) from a
boss trace of tools/capture_mission_rest.lua (MSX_REST_BOSS_FROM).

Every update of a boss node (constructors 0D5976, 0D600C, 0D6130, 0D61F2,
0D6464, 0D6500, 0D65EE, 0D680A, 0D69DC, 0D6A84, 0D6D30, 0D6F90, 0D6FFC)
becomes one case: field image after the update (and before it whenever it
differs from the previous one), and the ordered results of the engine
services the update called, read from the RAM traffic of the update by the
program counter of the original instruction:

  rng      R 0029D2           value returned by 0029C6
  alloc    R 0040C2           free count; child node and the bytes it inherits
  pal      W 009F78           palette slot
  scene    R 01ABEA/01AC00 (projection, parent not a boss node)
           R 01AD12..01AD1E   (01ACF2 root block)
  sweep    W 014492/0144F4/0144F8/014502   kernel result committed by 0143E4
  floor    W 014726/01471E    0146E8 result
  plat     R 0140C6, W 0140F2 (new slot), W 01ABBA/014152 (position)
  platfree W 013F4E
  hurt     R 004C24 (+50 non zero), W 004BE2 (kill-all), W 004DA4 (index)
  attack   R 0049E8, R 004B96 (result)
  bounds   R 019504.. (scene bounds block of RAM 10C2F0)
  draw     W 00A078/00A080    sprite list entry
  mission  018F92 / 018F40
Writes into boss nodes by other actors (bullets, 004892..0049E0) become
hits applied before the next boss update, with the attack row (A0, read
from ROM), damage/class (D3) and the attacker's bytes."""
import argparse,gzip,json,struct,collections
from pathlib import Path
from extract_boss_programs import rom,ROOT
NATIVE_GENERIC=(0x2a18a,0x2a244,0x2a2ac,0x2a2fe,0x2a3b4,0x2a3ce)
BOSS_CTORS={0x2a3b4,0xd5976,0xd600c,0xd6130,0xd61f2,0xd6464,0xd6500,0xd65ee,0xd680a,0xd69dc,0xd6a84,0xd6d30,0xd6f90,0xd6ffc}
POOL0=0x1010a4;NODE=0xb0
STACK0,STACK1=0x10f100,0x10f400
# global bytes the boss reads (neo_boss_env_t): facing 10C606, alive mask
# 10C33C, epoch 10C33A, players' +3C/+40 words and routine longs, mission
# over 10C300, rank 10FD8B, players 100088, video frame 10000E/10800F.
ENV_BYTES=set([0x10c606,0x10c33c,0x10c33a,0x10c300,0x10fd8b,0x100088,0x10000e,0x10000f,0x10800f]+
    list(range(0x101500,0x101502))+list(range(0x101504,0x101506))+list(range(0x1015b0,0x1015b2))+list(range(0x1015b4,0x1015b6))+
    list(range(0x1014c4,0x1014c8))+list(range(0x101574,0x101578)))
def node_of(a):return POOL0+(a-POOL0)//NODE*NODE if POOL0<=a<POOL0+NODE*100 else None
def lanes(a,m,d):
    out=[]
    if m&0xff00:out.append((a,(d>>8)&255))
    if m&0x00ff:out.append((a+1,d&255))
    return out
def parse(path):
    frame=0;cur=None;pending=[];attack=None
    for line in open(path):
        t=line[0]
        if t=='F':frame=int(line[2:]);continue
        p=line.split()
        if t=='U':
            if cur is not None:
                # 004188 leaves through 00424A on another node: no V record
                cur['post']=None;yield cur
            cur=dict(frame=frame+1,node=int(p[1],16),routine=int(p[2],16),pre=bytes.fromhex(p[3]),recs=[],hits=pending);pending=[];attack=None
        elif t=='V':cur['post']=bytes.fromhex(p[2]);yield cur;cur=None
        elif t in 'RW':
            a=int(p[1],16)
            # the supervisor stack (an interrupt frame carries the PC of the
            # interrupted instruction, which would fake a service result)
            if STACK0<=a<STACK1:continue
            cur['recs'].append((t,a,int(p[2],16),int(p[3],16),int(p[4],16)))
        elif t=='B':cur['recs'].append(('B',int(p[1],16),int(p[2],16),0,0x40ec))
        elif t=='X':
            a,m,d,pc,a6=(int(x,16) for x in p[1:6]);a0,d3=(int(x,16) for x in p[6:8]) if len(p)>=8 else (0,0)
            if pc in (0x4106,0x4166,0x416a):continue    # list links
            pending.append(dict(addr=a,mask=m,data=d,pc=pc,a6=a6,a0=a0,d3=d3,frame=frame))
        elif t=='A':pending.append(dict(attacker=int(p[1],16),bytes=bytes.fromhex(p[2])))
def main():
    ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('trace',type=Path);ap.add_argument('zip',type=Path)
    ap.add_argument('--name',required=True);ap.add_argument('--output',type=Path)
    a=ap.parse_args()
    p1,p2=rom(a.zip);bank=p2[0x100000:0x200000]
    def romw(addr):
        if addr<0x100000:return struct.unpack_from('>H',p1,addr)[0]
        return struct.unpack_from('>H',bank,addr-0x200000)[0]
    ups=list(parse(a.trace))
    # node identity: lifetimes split at births (B) and constructor dispatches
    ctor={};first_pre={}
    life={}            # address -> current life id
    lives=[]           # life id -> dict
    written_by_creator={};born=[];stale={}
    for u in ups:
        n=u['node']
        if n not in life or (u['routine']==0xd5976 and lives[life[n]]['ctor'] is not None):
            lid=len(lives);lives.append(dict(addr=n,creator=None,ctor=None,first=None));life[n]=lid;written_by_creator[lid]=set()
        L=lives[life[n]]
        if L['ctor'] is None:L['ctor']=u['routine'];L['first']=u['pre']
        for r in u['recs']:
            if r[0]=='B':
                lid=len(lives);lives.append(dict(addr=r[1],creator=life.get(u['node']),ctor=None,first=None));life[r[1]]=lid
                written_by_creator[lid]=set();born.append(lid)
            elif r[0] in 'RW':
                tn=node_of(r[1])
                # bytes of a node not yet dispatched: what any family update
                # read there before writing it is what the node inherited
                if r[0]=='W' and r[4]==0x410e and tn is not None and tn in life and lives[life[tn]]['ctor'] is None:
                    # 00410E stores the constructor: known even if the node
                    # is deleted before its first update
                    L2=lives[life[tn]];L2['ctor0']=((L2.get('ctor0',0)<<16)|r[3])&0xffffffff
                if tn is not None and tn!=n and tn in life and lives[life[tn]]['first'] is None:
                    st=stale.setdefault(life[tn],{})
                    for b,v in lanes(r[1],r[2],r[3]):
                        if r[0]=='W':written_by_creator[life[tn]].add(b-tn)
                        elif b-tn not in written_by_creator[life[tn]]:st.setdefault(b-tn,v)
        u['life']=life[n]
    for L in lives:
        if L['ctor'] is None and 'ctor0' in L:L['ctor']=L['ctor0']
    native={i for i,L in enumerate(lives) if L['ctor'] in BOSS_CTORS}
    ids={lid:k for k,lid in enumerate(sorted(native))}
    def image_base(lid):
        L=lives[lid]
        b=bytearray(L['first']) if L['first'] is not None else bytearray(NODE)
        if L['first'] is None:
            for o,v in stale.get(lid,{}).items():b[o]=v
        for o in written_by_creator.get(lid,()):b[o]=stale.get(lid,{}).get(o,0xa5)
        for o in range(0,0x10):b[o]=0
        return b.hex()
    addr_life={};nb=[0];last_rec={};handed=set()
    out=[];counts=collections.Counter();routines=collections.Counter()
    prev_post={}
    live_at={}   # address -> life id while processing (for pointer mapping)
    for u in ups:
        n=u['node']
        # hits pending since the previous boss update
        hits=[];hl0=u['hits']
        for j,h in enumerate(hl0):
            if 'attacker' in h:continue
            tn=node_of(h['addr'])
            if tn is None:continue
            # the attacker's bytes are dumped right after its first write
            att=next((x for x in hl0[j:]+hl0[:j][::-1] if 'attacker' in x and x['attacker']==h['a6']),None)
            hits.append((h,att))
        lid=u['life'];live_at[n]=lid
        # descendants of a handed-off node belong to the world from then on
        anc=lives[lid]['creator'];gone=False
        while anc is not None:
            if anc in handed:gone=True;break
            anc=lives[anc]['creator']
        if lid in native and gone:
            for r in u['recs']:
                if r[0]=='B':live_at[r[1]]=born[nb[0]];nb[0]+=1
            continue
        if u['routine']==0x3f1e:
            # a deleted node still linked before deeper nodes (003F1E)
            for r in u['recs']:
                if r[0]=='B':live_at[r[1]]=born[nb[0]];nb[0]+=1
            continue
        if lid in native and not (0xd5976<=u['routine']<0xd74d4 or u['routine'] in NATIVE_GENERIC):
            # a boss node continued by generic code after a hand-off
            if out and lid in last_rec and not last_rec[lid].get('handoff'):
                r0=last_rec[lid];img=bytes.fromhex(r0['post'])
                prog=int.from_bytes(img[0x20:0x24],'big')
                handed.add(lid)
                r0['handoff']=0x8eebc if u['routine']==0x8eebc else 0x1c71e if 0<=prog-0x22cf2c<0x40 else 0x1c706 if 0<=prog-0x22ce6c<0x40 else -1
            for r in u['recs']:
                if r[0]=='B':live_at[r[1]]=born[nb[0]];nb[0]+=1
            continue
        if lid not in native:
            # generic family actors are the world's: drop (keep birth order)
            for r in u['recs']:
                if r[0]=='B':live_at[r[1]]=born[nb[0]];nb[0]+=1
            continue
        ev=[];recs=u['recs'];i=0;last_attack=None
        scene_reads={};acf=[0]
        def life_id(addr):
            l=live_at.get(addr);return ids.get(l,-1) if l is not None else -1
        while i<len(recs):
            k,ad,m,d,pc=recs[i]
            if k=='R' and pc==0x29d2:ev.append(['rng',d])
            elif k=='R' and pc==0x40c2:ev.append(['alloc',d,None,None,None])
            elif k=='B':
                al=[e for e in ev if e[0]=='alloc'][-1]
                cl=born[nb[0]];nb[0]+=1
                assert lives[cl]['addr']==ad,('birth order',hex(ad),hex(lives[cl]['addr']))
                al[2]=cl;al[3]=ids.get(cl,-1);al[4]=image_base(cl);al.append(lives[cl]['first'].hex() if lives[cl]['first'] is not None else None)
                live_at[ad]=cl
            elif k=='W' and pc==0x9f78:ev.append(['pal',[v for _,v in lanes(ad,m,d)][0]])
            elif k=='R' and pc in (0x1abea,0x1ac00):
                scene_reads.setdefault('a1',ad-0x14 if pc==0x1abea and 'a1' not in scene_reads else scene_reads.get('a1'))
                scene_reads.setdefault('w',[]).append((ad,m,d,pc))
            elif k=='W' and pc==0x1ac08:
                tgt=node_of(ad);sr=scene_reads;scene_reads={}
                if sr and tgt is not None:
                    a1=sr['a1']
                    if node_of(a1) is not None and (a1-POOL0)%NODE==0x28:pass   # boss/node parent
                    else:
                        words=[x for x in sr['w'] if x[3]==0x1abea];corr=[x for x in sr['w'] if x[3]==0x1ac00]
                        x=(words[0][2]<<16|words[1][2]) if len(words)>=2 else 0;y=(words[2][2]<<16|words[3][2]) if len(words)>=4 else 0
                        c=[v for _,v in lanes(corr[0][0],corr[0][1],corr[0][2])][0] if corr else 0
                        ev.append(['scene',a1,x,y,None,None,c])
                i+=1;continue
            elif k=='R' and pc in (0x1ad12,0x1ad16,0x1ad1a,0x1ad1e):
                if pc==0x1ad12 and acf[0]%8==0:ev.append(['scene',ad-0x14,0,0,0,0,0])
                acf[0]+=1
                e=[e for e in ev if e[0]=='scene'][-1]
                slot={0x1ad12:2,0x1ad16:3,0x1ad1a:4,0x1ad1e:5}[pc]
                e[slot]=((e[slot] or 0)<<16|d)&0xffffffff
            elif k=='W' and pc==0x14492:
                if not ev or ev[-1][0]!='sweep':ev.append(['sweep',[],0,0,0])
                ev[-1][1].append(d)
            elif k=='W' and pc==0x144f4:ev[-1][2]=[v for _,v in lanes(ad,m,d)][0]
            elif k=='W' and pc==0x144f8:ev[-1][3]=[v for _,v in lanes(ad,m,d)][0]
            elif k=='W' and pc==0x14502:ev[-1][4]=d
            elif k=='W' and pc==0x1471e:ev.append(['floor',[],d])
            elif k=='W' and pc==0x14726:ev[-1][1].append(d)
            elif k=='R' and pc==0x140c6:ev.append(['plat',None,[]])
            elif k=='W' and pc==0x140f2:ev[-1][1]=[v for _,v in lanes(ad,m,d)][0]
            elif k=='W' and pc in (0x1abba,0x14152) and ev and ev[-1][0]=='plat':ev[-1][2].append(d)
            elif k=='W' and pc==0x13f4e:ev.append(['platfree'])
            elif k=='R' and pc==0x4c24:
                if ad==n+0x50:ev.append(['hurtr',d,None,None,0])
                elif ad==n+0x52:
                    e=ev[-1];v=e[1]<<16|d
                    if v:e[0]='hurt'
                    else:ev.pop()
            elif k=='W' and pc==0x4be2 and ev and ev[-1][0]=='hurt':ev[-1][4]=1
            elif k=='W' and pc==0x4da4 and ev and ev[-1][0]=='hurt':
                (b,v),=lanes(ad,m,d);ev[-1][2]=b-n;ev[-1][3]=v
            elif k=='R' and pc==0x49e8:ev.append(['attack',0]);last_attack=ev[-1]
            elif k=='R' and pc==0x4b96 and last_attack is not None:last_attack[1]=d
            elif k=='R' and pc==0x19504:
                if ad==0x10c2f0:ev.append(['bounds',d<<16,0,0,0,0x8000,0x8000,0x8000,0x8000])
                else:ev[-1][1]|=d
            elif k=='R' and pc==0x19508:ev[-1][2]=(ev[-1][2]<<16|d)&0xffffffff
            elif k=='R' and pc in (0x19518,0x1951c):
                j=3 if pc==0x19518 else 4;ev[-1][j]=(ev[-1][j]<<16|d)&0xffffffff
            elif k=='R' and pc in (0x1954c,0x19554,0x1956e,0x19576):
                ev[-1][{0x1954c:5,0x1956e:6,0x19554:7,0x19576:8}[pc]]=d
            elif k=='W' and pc==0xa078 and ad<0x10f000:   # an interrupt frame push has the same PC
                if not ev or ev[-1][0]!='draw' or len(ev[-1][1])>=6:ev.append(['draw',[],0])
                ev[-1][1].append(d)
            elif k=='W' and pc==0xa080:
                e=[e for e in ev if e[0]=='draw'][-1];e[2]=(e[2]<<16|d)&0xffffffff
            elif k in 'RW' and 0x18f92<=pc<0x18fd0:
                if not ev or ev[-1]!=['mission',1]:ev.append(['mission',1])
            elif k in 'RW' and 0x18f40<=pc<0x18f68:
                if not ev or ev[-1]!=['mission',2]:ev.append(['mission',2])
            i+=1
        envr={}
        for k,ad,m,d,pc in recs:
            if k!='R':continue
            for b,v in lanes(ad,m,d):
                if b in ENV_BYTES:envr[b]=v
        for e in ev:
            if e[0]=='bounds':
                # RAM 10C2F0 points at a ROM block: parent object (long), then
                # left/top/right/bottom words at +4/+6/+8/+A
                blk=e[1];e[2]=struct.unpack_from('>I',p1,blk)[0] if blk<0x100000 else e[2]
                if blk<0x100000:e[5],e[6],e[7],e[8]=struct.unpack_from('>HHHH',p1,blk+4)
        for e in ev:counts[e[0]]+=1
        routines[u['routine']]+=1
        hl=[]
        for h,att in hits:
            tn=node_of(h['addr'])
            tl=live_at.get(tn)
            if tl not in native:continue
            g=hl[-1] if hl else None
            if not g or g['node']!=ids[tl] or g['a6']!=h['a6'] or h['pc']<=g['last']:
                g=dict(node=ids[tl],a6=h['a6'],a0=h['a0'],d3=h['d3'],last=0,writes=[],
                    attacker=att['bytes'].hex() if att else None,kind=romw(h['a0']+8) if h['a0'] else None)
                hl.append(g)
            g['last']=h['pc']
            for b,v in lanes(h['addr'],h['mask'],h['data']):g['writes'].append([b-tn,v,h['pc']])
        pre=u['pre'];post=u['post']
        if post is None:post=bytes([0,0,0x3f,0x1e])+pre[4:]   # deleted in this update
        rec=dict(f=u['frame'],n=ids[lid],r=u['routine'],post=post.hex(),ev=ev,hits=hl,env={'%x'%k:v for k,v in sorted(envr.items())})
        if prev_post.get(lid)!=pre:rec['pre']=pre.hex()
        prev_post[lid]=post
        nrt=int.from_bytes(post[0:4],'big')
        if nrt!=0x3f1e and not (0xd5976<=nrt<0xd74d4 or nrt in NATIVE_GENERIC):
            # the update jumped into generic code: hand-off (label from the
            # program it started, or the routine it set)
            prog=int.from_bytes(post[0x20:0x24],'big')
            rec['handoff']=0x8eebc if nrt==0x8eebc else 0x1c71e if 0<=prog-0x22cf2c<0x40 else 0x1c706 if 0<=prog-0x22ce6c<0x40 else -1
            handed.add(lid)
        out.append(rec);last_rec[lid]=rec
    nodes=[dict(id=ids[l],addr=lives[l]['addr'],ctor=lives[l]['ctor'],creator=ids.get(lives[l]['creator'],-1)) for l in sorted(native)]
    # main boss spawn image
    data=dict(name=a.name,source='MAME 0.264 mslugx euro, tools/capture_mission_rest.lua MSX_REST_BOSS_FROM boss trace',
        nodes=nodes,updates=out,event_counts=counts,routines={'%06x'%k:v for k,v in sorted(routines.items())})
    o=a.output or ROOT/f'data/boss_fight_{a.name}.json.gz'
    with gzip.open(o,'wt',compresslevel=9) as f:f.write(json.dumps(data,separators=(',',':'))+'\n')
    print(len(out),'updates',len(nodes),'nodes',dict(counts),len(routines),'routines')
if __name__=='__main__':main()
