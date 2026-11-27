#!/usr/bin/env python3
"""B69: the player's vehicles take the enemies' fire on the full cartridge (ares 146).

Boots the normal cartridge (mslugx-explorer.z64), opens the test menu at
SOLDIER SELECT (L+R and A) and starts --mission (m1: MISSION 1, the Camel Slug
03458E and the SV-001 092A32; m2s3 / m5s3: MISSION 2 / 5 STAGE 3, the Slugnoid 09692C).
The pad walks right firing, with a jump every --jump seconds, until it rides a
vehicle; then it stops (no fire, no jump: the enemies come and shoot) for at
most --wait seconds, and walks on after the vehicle is lost. Optional life bar
(Z in the menu, not arcade) so the rider on foot reaches the vehicles.

Reads the log lines of actor.c: `MSX player mounted vehicle kind=K`, `MSX
vehicle hit (class-0 row, B69)`, `MSX vehicle state routine=R health=H kind=K
gauge=S/T`, `MSX vehicle flicker`, `MSX player left vehicle`. For each kind
in --expect (1 SV-001, 2 Camel, 3 Slugnoid): mounted, hit by a class-0 row
while riding, and
  - with a health (SV-001, Slugnoid: 0x30): the health drops by 0x10 a hit,
    the damage routine (SV-001 09351A/0935A8 or the M3 copy, Slugnoid
    097308/09737E), the flicker, the gauge target follows the health, and
    reported (not required): health 0, the destruction routine (SV-001 0938DA/093AFA, Slugnoid
    0973E2/09756A) and
    the rider leaves;
  - the Camel (health 0, gauge max 0): the hit kills the rider (0AA7FC
    kind 0: `left vehicle`).
--character N (B82: 0 Marco, 1 Tarma, 2 Eri, 3 Fio; SOLDIER SELECT's cursor moved
before the menu) with --rider: a kind passes once it is ridden and every `MSX
vehicle rider` line of the ride (actor.c) names that character with the ROM's
tables for it (Camel 23A58E[N], Slugnoid 09649C[N], Mission 3 vehicle 0AB40C[N],
palette 034330[N] / SV-001 climb 0954D4[N]) and no draw misses a descriptor or a
palette (`MSX native draw missing`) while riding; no hit is needed.
Last line: RESULT: PASS/FAIL. Needs DISPLAY (an Xvfb of its own)."""
import argparse,os,re,subprocess,time
import sys as _sys;from pathlib import Path as _Path;_sys.path.insert(0,str(_Path(__file__).resolve().parent));from ares_clock import EmuClock
from pathlib import Path
p=argparse.ArgumentParser(description=__doc__);p.add_argument('rom');p.add_argument('--out',required=True)
p.add_argument('--mission',default='m1',choices=('m1','m2s3','m3','m4','m5s3'))
p.add_argument('--rom-zip',default=os.environ.get('MSX_ROM_ZIP','mslugx.zip'))
p.add_argument('--character',type=int,default=0,choices=(0,1,2,3));p.add_argument('--rider',action='store_true',help='B82: check the rider of each ride instead of the damage')
p.add_argument('--expect',default='',help='comma list of vehicle kinds that must pass (default: m1 -> 2,1; m2s3/m5s3 -> 3)')
p.add_argument('--seconds',type=float,default=1500);p.add_argument('--wait',type=float,default=60,help='seconds the pad waits on a vehicle')
p.add_argument('--jump',type=float,default=2.5);p.add_argument('--up',type=float,default=5.0);p.add_argument('--period',type=float,default=4.0)
p.add_argument('--no-lifebar',dest='lifebar',action='store_false')
p.add_argument('--ride',choices=('wait','forward'),default='wait',help='while riding: wait (stand still, no fire) or forward (a 5 s cycle: 2 s right, 3 s still, the vehicle fire B in the last 0.6 s; never A: A+B is the exit) to take the vehicle to the enemies (the Slugnoid of m5s3, B69)')
import ares_padtest as pt;pt.add_args(p);pt_ticks=pt.ticks
p.add_argument('--ares',default='/workspace/emulator/build/rundir/bin/ares');p.add_argument('--data',default='/workspace/emulator/data146')
a=p.parse_args();out=Path(a.out);out.mkdir(parents=True,exist_ok=True)
if a.lifebar:   # N64 Z on the key z in a private copy of the ares data (tools/ares_private_data.py)
    import sys;sys.path.insert(0,str(Path(__file__).resolve().parent));from ares_private_data import private_data;a.data=private_data(a.data,out)
expect=[int(k) for k in (a.expect or {'m1':'2,1','m2s3':'3','m3':'4','m4':'1','m5s3':'3'}[a.mission]).split(',')]
env=dict(os.environ,XDG_DATA_HOME=a.data,XDG_CONFIG_HOME='/workspace/emulator/config146',XDG_RUNTIME_DIR='/workspace/emulator/runtime',
    LD_LIBRARY_PATH='/workspace/emulator/root/usr/lib/x86_64-linux-gnu',VK_DRIVER_FILES='/workspace/emulator/root/usr/share/vulkan/icd.d/lvp_icd.json',LIBGL_ALWAYS_SOFTWARE='1')
XD='/usr/bin/xdotool';logp=out/'ares.log';pt.prepare(a,out)
def key(action,name):
    if not a.pad_script:subprocess.run([XD,action,name],check=True,env=env)   # a replay presses nothing (tools/ares_padtest.py)
def tap(name,t=.1):key('keydown',name);time.sleep(t);key('keyup',name)
proc=subprocess.Popen([a.ares,'--setting','Video/Driver=OpenGL 3.2','--setting','Audio/Driver=None','--setting','General/HomebrewMode=true',
    '--system','Nintendo 64','--no-file-prompt',str(Path(a.rom).resolve())],stdout=open(logp,'w'),stderr=subprocess.STDOUT,env=env)
def log():return logp.read_text(errors='replace')
def wait(pat,secs=300):
    d=time.monotonic()+secs
    while time.monotonic()<d:
        t=log();assert 'ASSERTION FAILED' not in t and 'RSP CRASH' not in t and 'MSX ERROR' not in t,t[-3000:]
        m=re.search(pat,t)
        if m:return m
        assert proc.poll() is None,'ares exited'
        time.sleep(.05)
    raise AssertionError('timeout '+pat+'\n'+log()[-2000:])
# Damage routines of the ROM (neo_tank.c, neo_slugnoid.c): the reaction on the ground / in the air and the destruction.
REACT={3:(0x97308,0x9731a,0x9737e,0x97396),1:(0x9351a,0x93576,0x935a8,0x935ec,0x9351a+0x11e0,0x93576+0x11e0,0x935a8+0x11e0,0x935ec+0x11e0)}
DESTROY={3:(0x973e2,0x97414,0x9756a),1:(0x938da,0x9391e,0x93afa,0x938da+0x11e0,0x9391e+0x11e0,0x93afa+0x11e0)}
done=set();ok=False
try:
    win=None if not a.pad_script else 'none'
    while not win:
        r=subprocess.run([XD,'search','--onlyvisible','--class','ares'],text=True,capture_output=True,env=env).stdout.split()
        if r:win=r[-1]
        time.sleep(.5)
    if not a.pad_script:subprocess.run([XD,'windowmove',win,'0','0'],env=env);subprocess.run([XD,'windowfocus',win],env=env)
    # Start through the boot movies, the test menu of SOLDIER SELECT (L+R and A) and its item, each press held until the
    # cartridge logs its effect (tools/ares_menu.py): no counted presses on the host's clock.
    import ares_menu;ares_menu.choose(key,log,proc,*{'m1':(1,1),'m2s3':(2,3),'m3':(3,1),'m4':(4,1),'m5s3':(5,3)}[a.mission],lifebar=a.lifebar,place={0:0,1:2,2:1,3:3}[a.character])
    if a.character:wait(r'MSX CHARACTER %d chosen'%a.character,120)
    if a.lifebar:wait(r'MSX LIFEBAR on',60)
    wait(r'MSX first frame ready' if a.mission=='m1' else r'MSX stage \d+ camera x=',300)
    if not a.pad_script:subprocess.run([XD,'windowmove',win,'0','0'],env=env);subprocess.run([XD,'windowfocus',win],env=env)
    t0=time.monotonic();nextgrab=0;k=0;held={};ride_since=None;next_start=0;last_prog=None;prog_at=0;seen_wait=[0,-9.0];seek_dx=[None,0.0];seek_off=[0.0]
    def hold(n,on):
        if held.get(n)!=on:key('keydown' if on else 'keyup',n);held[n]=on
    # The schedule and --seconds run on the game's clock (tools/ares_clock.py), not the host's: the same pad under any machine load.
    clock=EmuClock();wall_end=time.monotonic()+(3*a.seconds+600 if not (a.pad_script or a.record) else 10**9)   # B78: a recording / replay ends on the game's count only (the caller's timeout bounds it)
    while time.monotonic()<wall_end and not set(expect)<=done:
        assert proc.poll() is None,'ares exited'
        text=log();t=clock.now(text[-4000:])
        if (pt_ticks(text)>=a.seconds*60 if (a.pad_script or a.record) else t>=a.seconds):break   # B78: a recording / replay ends on the logged update count
        assert 'ASSERTION FAILED' not in text and 'RSP CRASH' not in text,text[-3000:]
        mounts=len(re.findall(r'MSX player mounted vehicle',text));lefts=len(re.findall(r'MSX player left vehicle',text))
        riding=mounts>lefts
        if riding and ride_since is None:ride_since=t
        if not riding:ride_since=None
        waiting=riding and t-ride_since<a.wait
        prog=re.findall(r'MSX (?:stage \d+ camera x=|camera progress=)(-?\d+)',text[-20000:])
        if prog and prog[-1]!=last_prog:last_prog=prog[-1];prog_at=t
        # A camera stop (the Mission 1 towers, a tank, a helicopter): a 9 s cycle, 3 s left aiming up, 2 s
        # aiming up, 4 s right firing ahead.
        stuck=not riding and t-prog_at>12;c9=t%9.0
        up=not riding and ((stuck and c9<5.0) or (not stuck and bool(a.up) and (t%a.up)>a.up-1.2))   # on foot: aim up a while
        left=stuck and c9<3.0
        # A vehicle waiting for its rider (actor.c `MSX vehicle waiting`, twice a second): walk to it and hop on.
        # (a kind already done or not expected is left alone: 0345E0 Camel 2, 092B0E/093D76 SV-001 1, 096BA6 Slugnoid 3)
        nw=text.count('MSX vehicle waiting');vw=re.findall(r'MSX vehicle waiting routine=(\w+) dx=(-?\d+) dy=(-?\d+)',text[text.rfind('MSX vehicle waiting'):]) if nw else []
        if nw!=seen_wait[0]:seen_wait[:]=[nw,t]
        wkind={'0345e0':2,'092b0e':1,'093d76':1,'096ba6':3}.get(vw[-1][0]) if vw else None
        seek=not riding and vw and t-seen_wait[1]<1.5 and wkind in expect and wkind not in done
        if vw:vw=[(x,y) for _,x,y in vw]
        # A vehicle the pad cannot reach (dx unchanged 8 s: behind the camera's left edge) is left for 20 s.
        if seek and vw[-1]!=seek_dx[0]:seek_dx[:]=[vw[-1],t]
        if seek and t-seek_dx[1]>8:seek_off[0]=t+20;seek_dx[1]=t+20
        if t<seek_off[0]:seek=False
        fwd=waiting and a.ride=='forward';c5=(t-ride_since)%5.0 if fwd else 0.0
        if seek:
            dx=int(vw[-1][0]);up=False;left=dx<-6
            hold('Up',False);hold('Left',left);hold('Right',dx>6);hold('a',abs(dx)<48 and (t%0.6)<0.15)
        else:
            hold('Up',up);hold('Left',left);hold('Right',(fwd and c5<2.0) or (not waiting and not left and (stuck or not up)));hold('a',not riding and bool(a.jump) and (t%a.jump)<0.2)
        hold('b',(not waiting and not seek and int(t*10)%2==0) or (not seek and fwd and c5>=4.4 and int(t*10)%2==0))   # no fire while hopping on: A+B is the exit (0AA7FC kind 1)
        if a.rider and riding:   # B82: a kind already checked is left (A+B, 0AA7FC kind 1) to reach the next vehicle
            km=re.findall(r'MSX player mounted vehicle kind=(\d+)',text)
            if km and int(km[-1]) in done:hold('a',int(t*4)%2==0);hold('b',int(t*4)%2==0)
        pm=re.findall(r'MSX panel (play|continue|game over)',text[-20000:])
        if pm and pm[-1]=='continue' and t>=next_start:tap('Return');next_start=t+1.0
        if t>=nextgrab:
            mm=re.findall(r'MSX (?:stage \d+ camera x=|camera progress=\d+ x=)(-?\d+)',text[-20000:])
            subprocess.run(['import','-display',env['DISPLAY'],'-window','root',str(out/f'g{k:03d}_{mm[-1] if mm else 0}.png')]);k+=1;nextgrab=t+a.period
        # a kind is done once its rides show everything (checked below on the whole log)
        # (the `left` line prints the slot's +1D: the kind of the last mount is the one left)
        # (done once a ride of that kind took a class-0 hit; a ride left without one is tried again)
        if a.rider and riding:   # B82: done once the ride shows its rider for 3 s
            at=text.rfind('MSX player mounted vehicle kind=')
            km=re.match(r'MSX player mounted vehicle kind=(\d+)',text[at:])
            if km and ride_since is not None and t-ride_since>3 and re.search(r'MSX vehicle rider routine=',text[text.rfind('MSX player left vehicle',0,at)+1:]):done.add(int(km.group(1)))
        if lefts and not riding:
            at=text.rfind('MSX player mounted vehicle kind=')
            km=re.match(r'MSX player mounted vehicle kind=(\d+)',text[at:])
            if km and 'MSX vehicle hit (class-0 row' in text[at:]:done.add(int(km.group(1)))
        time.sleep(.03)
    for n in list(held):hold(n,False)
    pt.finish(a,proc,logp,[])
    text=log()
    # Split the log into rides: from each `mounted kind=K` to the next `left vehicle`.
    lines=text.splitlines();rides=[];cur=None
    for ln in lines:
        m=re.search(r'MSX player mounted vehicle kind=(\d+)',ln)
        if m:cur={'kind':int(m.group(1)),'lines':[]};rides.append(cur);continue
        if cur is not None:
            cur['lines'].append(ln)
            if 'MSX player left vehicle' in ln:cur=None
    verdict={}
    if a.rider:
        # B82: the riders' tables and palettes of the ROM for the character (P1 + P2 bank 1, as the port reads them).
        import zipfile
        z=zipfile.ZipFile(a.rom_zip);sw=lambda b:bytes(b[i^1] for i in range(len(b)))
        mem=sw(z.read('250-p1.p1'))+bytes(0x100000)+sw(z.read('250-p2.ep1'))[0x100000:0x200000]   # 000000 P1, 200000 the bank
        rl=lambda x:int.from_bytes(mem[x:x+4],'big');rw=lambda x:int.from_bytes(mem[x:x+2],'big');N=a.character
        want={0x34f96:(rl(0x23a58e+4*N),rw(0x34330+2*N)),0x97886:(rl(0x9649c+4*N),rw(0x34330+2*N)),0xad2fc:(rl(0xab40c+4*N),rw(0x34330+2*N)),0x95580:(0,rw(0x954d4+2*N))}
        kind_riders={2:(0x34f96,),1:(0x95580,),3:(0x97886,),4:(0xad2fc,)}
        # Each ride with the lines since the previous ride ended (the SV-001's climb 095580 comes before its mount line).
        segs=[];start=0;cur=None
        for i,ln in enumerate(lines):
            m=re.search(r'MSX player mounted vehicle kind=(\d+)',ln)
            if m:cur={'kind':int(m.group(1)),'lines':lines[start:i+1]};segs.append(cur);continue
            if cur is not None:
                cur['lines'].append(ln)
                if 'MSX player left vehicle' in ln:cur=None;start=i+1
        for kind in expect:
            best=None
            for r in (r for r in segs if r['kind']==kind):
                rid=[(int(m.group(1),16),int(m.group(2),16),int(m.group(3)),int(m.group(4),16),int(m.group(5))) for m in
                    (re.search(r'MSX vehicle rider routine=(\w+) table=(\w+) character=(-?\d+) palette=(\w+) player6e=(\d+)',l) for l in r['lines']) if m]
                rid=[x for x in rid if x[0] in kind_riders[kind]]
                # (while riding: the lines after the rider's death are the death's, not the vehicle's)
                L=r['lines'];dead=next((i for i,l in enumerate(L) if 'MSX player death' in l),len(L))
                miss=[l for l in L[:dead] if 'MSX native draw missing' in l]
                good=[x for x in rid if x[4]==N and x[3]==want[x[0]][1] and (x[0]==0x95580 or (x[2]==N and x[1]==want[x[0]][0]))]
                res={'riders':sorted(set('%06x/%06x/%d/%03X'%x[:4] for x in rid)),'want':'%06x/%03X'%want[kind_riders[kind][0]],'missing_draws':len(miss),'ok':bool(rid) and len(good)==len(rid) and not miss}
                if miss:res['missing']=miss[:3]
                if best is None or (res['ok'] and not best['ok']):best=res
            verdict[kind]=best;print('kind',kind,'character',N,best)
    for kind in ([] if a.rider else expect):
        best=None
        for r in (r for r in rides if r['kind']==kind):
            L=r['lines'];st=[(int(m.group(1),16),int(m.group(2)),int(m.group(3)),int(m.group(4))) for m in
                (re.search(r'MSX vehicle state routine=(\w+) health=(\d+) kind=\d+ gauge=(\d+)/(\d+)',l) for l in L) if m]
            hit=any('MSX vehicle hit (class-0 row, B69)' in l for l in L)
            left=any('MSX player left vehicle' in l for l in L)
            hs=[s[1] for s in st];drops=[(x,y) for x,y in zip(hs,hs[1:]) if y<x]
            res={'hit':hit,'left':left,'drops':drops}
            if kind==2:   # Camel: health 0, any hit kills the rider (0AA7FC kind 0)
                res['ok']=hit and left
            else:
                res['steps']=all(x-y==0x10 or y==0 for x,y in drops) and bool(drops)
                res['react']=kind not in REACT or any(s[0] in REACT[kind] for s in st)
                res['flicker']=any('MSX vehicle flicker' in l for l in L)
                res['gauge']=any(s[3]==s[1]<<8 for s in st if s[1]<0x30)   # 0C9B94: the target +7E is the health << 8
                res['destroyed']=bool(hs) and hs[-1]==0 and left and (kind not in DESTROY or any(s[0] in DESTROY[kind] for s in st))
                res['ok']=hit and res['steps'] and res['react'] and res['flicker'] and res['gauge']
            if best is None or (res['ok'] and not best['ok']) or (res.get('destroyed') and not best.get('destroyed')):best=res
        verdict[kind]=best
        print('kind',kind,best)
    stops=sorted(set(re.findall(r'(?:unsupported|explicit stop|stopped explicitly|not loaded|missing)[^\n]{0,100}',text)))
    print('screenshots',k,'rides',[(r['kind'],len(r['lines'])) for r in rides]);print('counted stops and missing draws in the log (%d kinds):'%len(stops))
    for s in stops[:40]:print('  ',s)
    ok=all(v and v['ok'] for v in verdict.values())
finally:
    proc.terminate()
    try:proc.wait(5)
    except Exception:proc.kill()
print('RESULT: PASS' if ok else 'RESULT: FAIL')
