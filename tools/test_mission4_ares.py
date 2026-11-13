#!/usr/bin/env python3
"""Mission 4 from the test menu in ares 146 (docs/MISION4.md, phase 1).

Boots the normal cartridge (mslugx-explorer.z64, never the reference one),
taps Start to SOLDIER SELECT, opens the test menu (L+R and A), chooses
MISSION 4 and holds right (with a jump every --jump seconds; no fire unless
--fire). Grabs a screenshot every --period seconds with the camera x in its
name. Checks from the log: the menu choice, the pack load with the flow's
descriptor 016E1E and camera list 0E741A, the camera reaching --progress,
no crash and no "not loaded" stop. Prints the counted stops of the log.
Last line: RESULT: PASS/FAIL. Needs DISPLAY (an Xvfb of its own)."""
import argparse,os,re,subprocess,time
from pathlib import Path
p=argparse.ArgumentParser(description=__doc__);p.add_argument('rom');p.add_argument('--out',required=True)
p.add_argument('--seconds',type=float,default=90);p.add_argument('--period',type=float,default=3.0);p.add_argument('--progress',type=int,default=600)
p.add_argument('--jump',type=float,default=0.0);p.add_argument('--fire',action='store_true');p.add_argument('--until',type=int,default=99999)
p.add_argument('--continues',action='store_true',help='Start at every CONTINUE? panel (a player\'s choice, as tools/test_mission6_ares.py)')
p.add_argument('--boss',action='store_true',help='PASS only when the boss 02267E dies and Mission 5 starts (`MSX mission 5 stage N start`, 018F92 -> 01891E); the run stops there')
p.add_argument('--vehicle',type=int,default=0,help='B69: PASS only when the player mounts this vehicle kind (1 = the SV-001 092A32 at 4288) and a class-0 row hits it while mounted (`MSX vehicle hit (class-0 row, B69) ... mount=1` after `MSX player mounted vehicle kind=K`)')
p.add_argument('--lifebar',action='store_true',help='Z in the test menu: the optional life bar (not arcade), to walk past the sections the mortal pad dies in; needs N64 Z on the key z (R-Trigger: 0x1/0/60 in the ares settings)')
p.add_argument('--ares',default='/workspace/emulator/build/rundir/bin/ares');p.add_argument('--data',default='/workspace/emulator/data146')
a=p.parse_args();out=Path(a.out);out.mkdir(parents=True,exist_ok=True)
if a.lifebar:   # N64 Z on the key z in a private copy of the ares data (tools/ares_private_data.py)
    import sys;sys.path.insert(0,str(Path(__file__).resolve().parent));from ares_private_data import private_data;a.data=private_data(a.data,out)
env=dict(os.environ,XDG_DATA_HOME=a.data,XDG_CONFIG_HOME='/workspace/emulator/config146',XDG_RUNTIME_DIR='/workspace/emulator/runtime',
    LD_LIBRARY_PATH='/workspace/emulator/root/usr/lib/x86_64-linux-gnu',VK_DRIVER_FILES='/workspace/emulator/root/usr/share/vulkan/icd.d/lvp_icd.json',LIBGL_ALWAYS_SOFTWARE='1')
XD='/usr/bin/xdotool';logp=out/'ares.log'
def key(action,name):subprocess.run([XD,action,name],check=True,env=env)
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
ok=False;cam=0;maxcam=0
try:
    win=None
    while not win:
        r=subprocess.run([XD,'search','--onlyvisible','--class','ares'],text=True,capture_output=True,env=env).stdout.split()
        if r:win=r[-1]
        time.sleep(.5)
    subprocess.run([XD,'windowmove',win,'0','0'],env=env);subprocess.run([XD,'windowfocus',win],env=env)
    # Start through the cinematics; at SOLDIER SELECT hold L+R (keys d, e) and press A (the test menu), Up seven times (MISSION 4 is the seventh item from the end: the three stages of Mission 5 and of Mission 6 follow it), A.
    end=time.monotonic()+400
    while 'MSX MSELECT mission=' not in log():
        assert proc.poll() is None and time.monotonic()<end,'no test menu\n'+log()[-1500:]
        if re.search(r'MSX CINE select (start|waiting)',log()):
            time.sleep(1.5);key('keydown','d');key('keydown','e');time.sleep(.2);tap('a');time.sleep(.2);key('keyup','e');key('keyup','d')
            if 'MSX MSELECT menu open' in log():time.sleep(.5);[(tap('Up',.2),time.sleep(.4)) for _ in range(7)];(a.lifebar and (tap('z'),time.sleep(.3)));tap('a');time.sleep(.5)
            continue
        # One Start for the title and one for HOW TO PLAY (the cinematic before SOLDIER SELECT); none after it,
        # or the press lands on SOLDIER SELECT and chooses before the menu combo.
        t=log()
        if ('MSX TITLE start' in t and 'MSX TITLE complete' not in t) or ('MSX CINE howto start' in t and 'MSX CINE howto complete' not in t) or ('MSX OPTIONS open' in t and 'MSX OPTIONS close' not in t):tap('Return')
        time.sleep(1.0)
    wait(r'MSX MSELECT mission=4',20)
    if a.lifebar:wait(r'MSX LIFEBAR on',5)
    m=wait(r'MSX mission 4 stage \d+ start: record (\w+) descriptor (\w+) camera list (\w+)',120)
    assert m.group(2)=='016E1E' and m.group(3)=='0E741A',m.group(0)
    wait(r'MSX stage \d+ camera x=',300)
    subprocess.run([XD,'windowmove',win,'0','0'],env=env);subprocess.run([XD,'windowfocus',win],env=env)   # the window grows when the game starts
    t0=time.monotonic();nextgrab=0;k=0;held={};lastcam=-1;stuck=0;continued=0
    def hold(n,on):
        if held.get(n)!=on:key('keydown' if on else 'keyup',n);held[n]=on
    while time.monotonic()-t0<a.seconds:
        assert proc.poll() is None,'ares exited'
        t=time.monotonic()-t0;text=log()
        assert 'ASSERTION FAILED' not in text and 'RSP CRASH' not in text,text[-3000:]
        mm=re.findall(r'MSX stage \d+ camera x=(-?\d+) y=(-?\d+)',text[-20000:])
        cam=int(mm[-1][0]) if mm else 0
        maxcam=max(maxcam,cam)
        if cam>=a.until or (a.boss and re.search(r'MSX mission 5 stage \d+ start',text)):break
        if a.continues and text.count('MSX panel continue')>continued:
            for n in list(held):hold(n,False)
            time.sleep(1.0);tap('Return');time.sleep(.5);continued=text.count('MSX panel continue');continue
        # A camera lock waits for an enemy group to be emptied: the car 027F54 at 1000 (ahead of Marco, who jumps on
        # its roof if he keeps walking), the armoured vehicle 087E36 at 2160 (behind him), the helicopters 085522 over him
        # at 4360 (script 2DC67A waits for group 6 < 1; they leave only when out of bombs). After 6 s without progress the
        # pad stands and fights a 12 s cycle: 0.8 s back left and fire left (a grenade at 1.5 s), 0.15 s turn right and
        # fire right (a grenade at 5.5 s), then 4 s walking on (with jumps); after 40 s stuck the cycle is 20 s, the last 8 s
        # firing upwards (Up held) while walking 4 s left and 4 s right (a flyer hovers anywhere over the street).
        # Grenades need N64 Z on the key z (--lifebar's private ares data).
        if cam!=lastcam:lastcam=cam;stuck=t
        ph=-1;up=False
        if a.fire and t-stuck>6:
            if t-stuck<46:ph=(t-stuck-6)%12
            else:ph=(t-stuck-46)%20;up=ph>=12
        fight=0<=ph<8;left=0<=ph<0.8 or 12<=ph<16;right=(4<=ph<4.15) or (not fight and not up) or ph>=16
        jump=bool(a.jump) and not fight and not up and (t%a.jump)<0.2;bomb=a.lifebar and (1.5<=ph<1.65 or 5.5<=ph<5.65)
        # The boss 02267E (camera list 0x15B0) comes from the left while the camera follows the player at 0x8000 to the
        # focus point 0x20 (MAME T): a 5 s cycle facing it, 0.15 s turning left, 3.5 s firing left (a jump at 1 s and
        # 2.6 s, a grenade at 2 s), then 1.35 s walking right to keep up with the camera.
        if a.boss and 'MSX boss layer 10D7B6' in text and 'MSX mission complete signal' not in text:
            bp=t%5;left=bp<0.15;right=bp>=3.65;up=False
            jump=1.0<=bp<1.2 or 2.6<=bp<2.8;bomb=a.lifebar and 2.0<=bp<2.15
        # Riding (B69: the SV-001 of 4288), no jump: A+B together is the vehicle's exit (0AA7FC kind 1), and the
        # pad's jumps over its fire left the SV-001 at 4360 before the helicopters' bombs hit it.
        if text.count('MSX player mounted vehicle')>text.count('MSX player left vehicle'):jump=False
        hold('Right',right);hold('Left',left);hold('Up',up);hold('Down',False)
        hold('a',jump);hold('b',a.fire and int(t*10)%2==0)
        hold('z',bomb)
        if t>=nextgrab:
            subprocess.run(['import','-display',os.environ['DISPLAY'],'-window','root',str(out/f'g{k:03d}_{cam}_{mm[-1][1] if mm else 0}.png')]);k+=1;nextgrab=t+a.period
        time.sleep(.03)
    for n in list(held):hold(n,False)
    text=log()
    stops=sorted(set(re.findall(r'(?:unsupported|explicit stop|not loaded|missing)[^\n]{0,100}',text)))
    print('camera',cam,'screenshots',k,'continues',continued);print('counted stops and missing draws in the log (%d kinds):'%len(stops))
    for s in stops[:40]:print('  ',s)
    ok=(maxcam if a.boss else cam)>=a.progress and 'not loaded' not in text   # with --boss the camera restarts at 0 in Mission 5
    if a.vehicle:
        at=text.find('MSX player mounted vehicle kind=%d '%a.vehicle)
        hit=at>=0 and re.search(r'MSX vehicle hit \(class-0 row, B69\) count=\d+ mount=1',text[at:]) is not None
        print('vehicle kind %d: mounted %s, hit while mounted %s'%(a.vehicle,at>=0,hit));ok=ok and hit
    if a.boss:
        for pat in (r'MSX boss layer 10D7B6 map=[0-9A-F]*[1-9A-F][0-9A-F]* ',r'MSX mission complete signal \(018F92\)',r'MSX mission 5 stage \d+ start'):
            m=re.search(pat,text);print('boss:',pat,'->',m.group(0) if m else 'MISSING');ok=ok and m is not None
finally:
    for n in ('Right','Left','Up','Down','a','b','d','e','z'):subprocess.run([XD,'keyup',n],env=env)   # a key left down would be read by the next run's menu
    proc.terminate()
    try:proc.wait(5)
    except Exception:proc.kill()
print('RESULT: PASS' if ok else 'RESULT: FAIL')
