#!/usr/bin/env python3
"""Weapon block 10C718..10C72B (10C72C/10C72E are not weapon fields) fixture from capture_weapons.lua runs.

The row of frame f holds the block before the logic of f, so the
events logged at f-1 explain the change from row f-1 to row f.
For every frame whose block changes (or that logs a weapon event) the
fixture keeps the block of the previous frame, the block after and the
events of the frame in log order:
  grant id       BP_0C0928 (d0 low word = player, d1 = id)
  bombs n        BP_0C0B32 (d1)
  expire         BP_0C0EE2 called from 0C339E (the 0C337A check)
  death          0C2FA4 (inferred: dead player, in-flight, id, bombs and
                 ammunition all 0)
  life           BP_0C0E86 (0C0E6E: a new life)
  fire           BP_0C438E (0C0D64 granted for the current id), and an H
                 bullet 01B6DC/01B716/01B668/01BDD2 not from 0C447C (the
                 diagonal 0C453A/0C4610, not instrumented)
  throw          BP_0C4786 (bomb consumed when bombs > 0)
  vehicle_throw  BP_0C0808 (the Camel's bomb, from the player's bombs)
The decrements nothing logs are inferred from the block and recorded as
events of their own: armed (0C0D34: the grenade's first update),
landed (0C0D48: a grenade gone), release (0C0DFC: a
projectile of the current id gone) and denied (0C0D64 consumed a round but
the active cap refused). Frames before --from are skipped (pickups2 and
vehicles2 write the block themselves until 9399).
Usage: extract_weapon_block.py RUN[:FROM] ... --output data/weapon_block_fixture.json.gz"""
import argparse,csv,gzip,json,re
from pathlib import Path
ap=argparse.ArgumentParser();ap.add_argument('runs',nargs='+');ap.add_argument('--output',required=True)
args=ap.parse_args()
out={'source':'tools/capture_weapons.lua (MAME 0.264 mslugx), /home/user/captures/weapons','runs':[]}
BP=re.compile(r'^BP_([0-9A-F]+) frame=(\d+) (\S+)(.*)$')
for spec in args.runs:
    path,_,start=spec.partition(':');start=int(start or 0);run=Path(path)
    raw=[]
    for line in open(run/'debug.log'):
        m=BP.match(line.strip())
        if not m: continue
        addr,frame,name,rest=m.group(1),int(m.group(2)),m.group(3),m.group(4)
        kv=dict(x.split('=',1) for x in rest.split() if '=' in x)
        hx=lambda k:int(kv[k],16)
        e=None
        if addr=='0C0928': e=['grant',hx('d1'),hx('d0')&0xffff]
        elif addr=='0C09C8' and raw and raw[-1][1][0]=='grant': raw[-1][2]['post']=(hx('id'),hx('ammo'));continue
        elif addr=='0C0B32': e=['bombs',hx('d1'),hx('d0')&0xffff];kv['pre']=hx('bombs')
        elif addr=='0C0EE2' and kv.get('ret')=='000C339E': e=['expire']
        elif addr=='0C0E86': e=['life']
        elif addr=='0C438E' and kv.get('a6','001014C4')=='001014C4': e=['fire'];kv['post']=(hx('ammo'),hx('shots'))
        elif addr=='0C4786' and kv['a6']=='001014C4': e=['throw'];kv['pre']=hx('bombs')
        elif addr=='0C0808': e=['vehicle_throw'];kv['pre']=hx('bombs')
        elif addr=='0C447C': e=['fire_h']
        elif addr=='00411C' and kv['ctor'] in ('0001B6DC','0001B716','0001B668','0001BDD2') and kv['creator']=='001014C4': e=['h_bullet']
        if e: raw.append([frame,e,kv])
    rows=list(csv.DictReader(open(run/'weapons.csv')))
    blk={int(r['frame']):bytes.fromhex(r['weapon_block'][:40]) for r in rows}
    ammo=lambda b:b[12]<<8|b[13]
    # The row of MAME frame f is read at one point of the frame: an event
    # of frame f can come before it (its effect is already in row f) or
    # after. The logged state of the event decides; by default after.
    def done(e,kv,b):
        k=e[0]
        if k=='fire': return ammo(b)==kv['post'][0] and (b[4]==0xff or b[2]==kv['post'][1])
        if k in ('throw','vehicle_throw'): return kv['pre']>0 and b[8]==kv['pre']-1
        if k=='bombs': return b[8]!=kv['pre']
        if k=='grant': return 'post' in kv and b[6]==kv['post'][0] and ammo(b)==kv['post'][1]
        if k=='expire': return b[6]==0 and ammo(b)==0xffff
        return False
    # The diagonal H shots 0C453A/0C4610 reserve through 0C0D64 too but are
    # not instrumented: an H bullet beyond the 0C447C ones of its frame is
    # one, early when its transition shows a round nobody took.
    hcount={}
    for frame,e,kv in raw:
        if e[0]=='fire_h': hcount[frame]=hcount.get(frame,0)+1
    ev={}
    for frame,e,kv in raw:
        if e[0]=='fire_h': continue
        if e[0]=='h_bullet':
            if hcount.get(frame,0): hcount[frame]-=1;continue
            t=frame
            if frame in blk and frame-1 in blk:
                fired=sum(1 for x in ev.get(frame,[]) if x[0]=='fire')
                if ammo(blk[frame-1])-ammo(blk[frame])<=fired: t=frame+1
            ev.setdefault(t,[]).append(['fire']);continue
        early=frame in blk and frame-1 in blk and done(e,kv,blk[frame]) and not done(e,kv,blk[frame-1])
        ev.setdefault(frame if early else frame+1,[]).append(e)
    prev=None;steps=[]
    for r in rows:
        f=int(r['frame']);blk=r['weapon_block'][:40]
        if prev is not None and f>start and int(prev['frame'])==f-1:
            es=list(ev.get(f,[]))
            if es or blk!=prev['weapon_block'][:40]:
                a=bytes.fromhex(prev['weapon_block'][:40]);b=bytes.fromhex(blk)
                fires=sum(1 for e in es if e[0]=='fire')
                if int(prev['f5f'],16)&0x80 and b[8]==0 and ammo(b)==0 and b[6]==0 and b[0]==0 and ammo(a)!=0:
                    es.append(['death'])   # 0C2FA4, the dead player gone (not instrumented)
                elif b[0]>a[0]: es+=[['armed']]*(b[0]-a[0])
                if b[0]<a[0]: es+=[['landed']]*(a[0]-b[0])
                changed=any(e[0] in ('grant','expire','life') for e in es)
                if not changed:
                    if b[2]<a[2]+ (fires if a[4]!=0xff else 0): es+=[['release']]*(a[2]+(fires if a[4]!=0xff else 0)-b[2])
                    aa=a[12]<<8|a[13];bb=b[12]<<8|b[13]
                    if aa!=0xffff and bb<aa-fires: es+=[['denied']]*(aa-fires-bb)
                steps.append({'frame':f,'before':prev['weapon_block'][:40],'after':blk,'events':es,
                    'slot':prev['slot_p1'],'f93':int(prev['f93'],16),'f5f':int(prev['f5f'],16)})
        prev=r
    out['runs'].append({'run':run.name,'from':start,'steps':steps})
    print(run.name,len(steps),'steps',sum(len(s['events']) for s in steps),'events')
data=json.dumps(out,separators=(',',':')).encode()
if args.output.endswith('.gz'): Path(args.output).write_bytes(gzip.compress(data,mtime=0))
else: Path(args.output).write_bytes(data)
