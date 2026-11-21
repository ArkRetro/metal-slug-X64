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
import argparse,os,re,subprocess,sys,time
from pathlib import Path
sys.path.insert(0,str(Path(__file__).resolve().parent));from ares_clock import EmuClock
p=argparse.ArgumentParser(description=__doc__);p.add_argument('rom');p.add_argument('--out',required=True)
p.add_argument('--seconds',type=float,default=90);p.add_argument('--period',type=float,default=3.0);p.add_argument('--progress',type=int,default=600)
p.add_argument('--jump',type=float,default=0.0);p.add_argument('--fire',action='store_true');p.add_argument('--until',type=int,default=99999)
p.add_argument('--continues',action='store_true',help='Start at every CONTINUE? panel (a player\'s choice, as tools/test_mission6_ares.py)')
p.add_argument('--boss',action='store_true',help='PASS only when the boss 02267E dies and Mission 5 starts (`MSX mission 5 stage N start`, 018F92 -> 01891E); the run stops there')
p.add_argument('--vehicle',type=int,default=0,help='B69: PASS only when the player mounts this vehicle kind (1 = the SV-001 092A32 at 4288) and a class-0 row hits it while mounted (`MSX vehicle hit (class-0 row, B69) ... mount=1` after `MSX player mounted vehicle kind=K`)')
p.add_argument('--lifebar',action='store_true',help='Z in the test menu: the optional life bar (not arcade), to walk past the sections the mortal pad dies in; needs N64 Z on the key z (R-Trigger: 0x1/0/60 in the ares settings)')
p.add_argument('--pad-script',help='B78: replay this pad script (tools/padscript.py) on the test cartridge ROM (mslugx-testpad.z64): no xdotool; '
    'the ROM is copied with the script into --out (a fresh EEPROM) and the log is cut at the end condition, so two replays give the same numbers')
p.add_argument('--record',action='store_true',help='B78: the test cartridge without a script (keys from xdotool), copied into --out (a fresh EEPROM); '
    'its log is the script (tools/padscript.py from-log) and is cut at the end condition like a replay')
p.add_argument('--ares',default='/workspace/emulator/build/rundir/bin/ares');p.add_argument('--data',default='/workspace/emulator/data146')
a=p.parse_args();out=Path(a.out);out.mkdir(parents=True,exist_ok=True)
if a.lifebar:   # N64 Z on the key z in a private copy of the ares data (tools/ares_private_data.py)
    import sys;sys.path.insert(0,str(Path(__file__).resolve().parent));from ares_private_data import private_data;a.data=private_data(a.data,out)
env=dict(os.environ,XDG_DATA_HOME=a.data,XDG_CONFIG_HOME='/workspace/emulator/config146',XDG_RUNTIME_DIR='/workspace/emulator/runtime',
    LD_LIBRARY_PATH='/workspace/emulator/root/usr/lib/x86_64-linux-gnu',VK_DRIVER_FILES='/workspace/emulator/root/usr/share/vulkan/icd.d/lvp_icd.json',LIBGL_ALWAYS_SOFTWARE='1')
XD='/usr/bin/xdotool';logp=out/'ares.log'
import ares_padtest as pt;pt_ticks=pt.ticks
if a.pad_script or a.record:
    import shutil;rom=out/'testpad.z64'
    for f in (out/'testpad.eeprom',out/'testpad.pak'):
        if f.exists():f.unlink()
    if a.pad_script:
        from padscript import apply,read_script;apply(a.rom,read_script(a.pad_script),rom)
    else:shutil.copyfile(a.rom,rom)
    a.rom=str(rom)
def cut_log():
    # The log up to the end condition's line (a replay's numbers do not depend on when the host stopped ares).
    t=logp.read_text(errors='replace');ends=[]
    if a.boss:
        m=re.search(r'MSX mission 5 stage \d+ start[^\n]*\n',t)
        if m:ends.append(m.end())
    for m in re.finditer(r' ticks=(\d+)\n',t):
        if int(m.group(1))>=a.seconds*60:ends.append(m.end());break
    if ends:logp.write_text(t[:min(ends)])
BOSS_NEAR=(-80,-20)   # player x - body x (px) where the boss cycle runs
def key(action,name):
    if not a.pad_script:subprocess.run([XD,action,name],check=True,env=env)   # a replay presses nothing: the ROM's script is the pad
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
    win=None if not a.pad_script else 'none'
    while not win:
        r=subprocess.run([XD,'search','--onlyvisible','--class','ares'],text=True,capture_output=True,env=env).stdout.split()
        if r:win=r[-1]
        time.sleep(.5)
    if not a.pad_script:subprocess.run([XD,'windowmove',win,'0','0'],env=env);subprocess.run([XD,'windowfocus',win],env=env)
    # Start through the boot movies, the test menu of SOLDIER SELECT (L+R and A) and its item, each press held until the
    # cartridge logs its effect (tools/ares_menu.py): no counted presses on the host's clock.
    import ares_menu;ares_menu.choose(key,log,proc,4,1,lifebar=a.lifebar)
    wait(r'MSX MSELECT mission=4',20)
    if a.lifebar:wait(r'MSX LIFEBAR on',5)
    m=wait(r'MSX mission 4 stage \d+ start: record (\w+) descriptor (\w+) camera list (\w+)',120)
    assert m.group(2)=='016E1E' and m.group(3)=='0E741A',m.group(0)
    wait(r'MSX stage \d+ camera x=',300)
    if not a.pad_script:subprocess.run([XD,'windowmove',win,'0','0'],env=env);subprocess.run([XD,'windowfocus',win],env=env)   # the window grows when the game starts
    t0=time.monotonic();nextgrab=0;k=0;held={};lastcam=-1;stuck=0;continued=0;seq=None
    def hold(n,on):
        if held.get(n)!=on:key('keydown' if on else 'keyup',n);held[n]=on
    # The schedule and --seconds run on the game's clock (tools/ares_clock.py), not the host's: the same pad under any machine load.
    clock=EmuClock();wall_end=time.monotonic()+3*a.seconds+600
    while time.monotonic()<wall_end:
        assert proc.poll() is None,'ares exited'
        text=log();t=clock.now(text[-4000:])
        if (pt_ticks(text)>=a.seconds*60 if (a.pad_script or a.record) else t>=a.seconds):break   # B78: a recording / replay ends on the logged update count
        assert 'ASSERTION FAILED' not in text and 'RSP CRASH' not in text,text[-3000:]
        mm=re.findall(r'MSX stage \d+ camera x=(-?\d+) y=(-?\d+)',text[-20000:])
        cam=int(mm[-1][0]) if mm else 0
        maxcam=max(maxcam,cam)
        if cam>=a.until or (a.boss and re.search(r'MSX mission 5 stage \d+ start',text)):break
        if a.continues and text.count('MSX panel continue')>continued:
            for n in list(held):hold(n,False)
            # Start until the panel takes it (`MSX panel play` again): one 0.1 s tap was less than one pad read with the
            # emulator at a third of its speed (a loaded machine), the countdown ran out and the game restarted in Mission 1.
            plays=text.count('MSX panel play');time.sleep(1.0)
            for _ in range(30):
                tap('Return',.4);time.sleep(.6);tx=log()
                if tx.count('MSX panel play')>plays or tx.count('MSX panel game over')>text.count('MSX panel game over'):break
            continued=text.count('MSX panel continue')
            continue
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
            # The pad faces the boss (B78): the body keeps itself between the screen x 0x90 and 0xE0 (022872) while the
            # camera brings the player to the left edge, under the hull's tail, where the blind cycle fired at nothing.
            # The cartridge logs the body's screen x and the player's (`MSX boss body ... x=X ... player=PX,PY`, every 30
            # updates): the cycle above only runs with the player BOSS_NEAR[0]..BOSS_NEAR[1] px from the body (where the
            # blind pad's hits landed, measured on its logs); farther left the pad walks right, farther right it walks left.
            bm=re.findall(r'MSX boss body \w+ x=(-?\d+) y=-?\d+ health=\d+ lost=\w+ phase=\d+ player=(-?\d+),',text[-6000:])
            if bm:
                dx=int(bm[-1][1])-int(bm[-1][0])
                if dx<BOSS_NEAR[0]:left=False;right=True
                elif dx>BOSS_NEAR[1]:left=True;right=False
        hold('Right',right);hold('Left',left);hold('Up',up);hold('Down',False)
        # Riding a vehicle, A and B together are the exit 093A08 (0CA1B0: pad bit 7; the vehicle is thrown away). The pad
        # used to press them together by chance (the 5 Hz fire against the jump), and then nothing could hit it mounted.
        # While it rides, a jump is a sequence on the host's clock (well over one update at any load): fire released,
        # 0.3 s, A for 0.2 s, 0.3 s, fire again.
        fire=a.fire and int(t*10)%2==0
        if text.rfind('MSX player mounted vehicle')>max(text.rfind('MSX player left vehicle'),text.rfind('MSX player death'),text.rfind('MSX respawn ')):
            w=time.monotonic()
            if jump and seq is None:seq=w
            if seq is not None:
                fire=False;jump=0.3<=w-seq<0.5
                if w-seq>=0.8:seq=None
        elif seq is not None:seq=None;jump=False
        hold('b',fire);hold('a',jump)
        hold('z',bomb)
        if t>=nextgrab:
            subprocess.run(['import','-display',os.environ['DISPLAY'],'-window','root',str(out/f'g{k:03d}_{cam}_{mm[-1][1] if mm else 0}.png')]);k+=1;nextgrab=t+a.period
        time.sleep(.03)
    for n in list(held):hold(n,False)
    if a.pad_script or a.record:
        time.sleep(1.0);proc.terminate()
        try:proc.wait(10)
        except Exception:proc.kill();proc.wait()
        cut_log()
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
    for n in ('Right','Left','Up','Down','a','b','d','e','z'):key('keyup',n)   # a key left down would be read by the next run's menu
    proc.terminate()
    try:proc.wait(5)
    except Exception:proc.kill()
print('RESULT: PASS' if ok else 'RESULT: FAIL')
