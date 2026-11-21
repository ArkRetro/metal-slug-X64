#!/usr/bin/env python3
"""Mission 5 (stage --stage 1..3) from the test menu in ares 146 (docs/MISION5.md, phase 1).

Boots the normal cartridge (mslugx-explorer.z64, never the reference one),
taps Start to SOLDIER SELECT, opens the test menu (L+R and A), chooses
MISSION 5 STAGE n and holds right (with a jump every --jump seconds; no fire unless
--fire). Grabs a screenshot every --period seconds with the camera x in its
name. Checks from the log: the menu choice, the pack load with the flow's
descriptor and camera list (016E2E / 0BB8E6, 016E3E / 0BB9BA, 016E4E / 0BBA24), the camera reaching --progress,
no crash and no "not loaded" stop. Prints the counted stops of the log.
Last line: RESULT: PASS/FAIL. Needs DISPLAY (an Xvfb of its own)."""
import argparse,os,re,subprocess,time
import sys as _sys;from pathlib import Path as _Path;_sys.path.insert(0,str(_Path(__file__).resolve().parent));from ares_clock import EmuClock
from pathlib import Path
p=argparse.ArgumentParser(description=__doc__);p.add_argument('rom');p.add_argument('--out',required=True)
p.add_argument('--stage',type=int,default=1,choices=(1,2,3));p.add_argument('--seconds',type=float,default=90);p.add_argument('--period',type=float,default=3.0);p.add_argument('--progress',type=int,default=600)
p.add_argument('--jump',type=float,default=0.0);p.add_argument('--fire',action='store_true');p.add_argument('--until',type=int,default=99999)
p.add_argument('--back',type=float,default=0,help='every BACK seconds, hold left for 1.5 s (0: never)')
p.add_argument('--up',type=float,default=0,help='every UP seconds, hold up for 1.5 s (the pad fires upward: the helicopters over a camera stop; 0: never)')
p.add_argument('--lifebar',action='store_true',help='Z in the test menu: the optional life bar (not arcade), to walk past the sections the mortal pad dies in; needs N64 Z on the key z (R-Trigger: 0x1/0/60 in the ares settings)')
p.add_argument('--down',action='store_true',help='hold down outside the --back / --up turns: the pistol fires down in the jumps (the boss 0A0C00 below the ledge of the arena, camera 2560)')
p.add_argument('--bomb',action='store_true',help='a bomb (N64 Z, key z) at the start of each --back turn: the tank on treads 08FF7E below the stairs of the sewer\'s camera 2064 is out of the pistol\'s line')
p.add_argument('--boss',action='store_true',help='stage 3: PASS needs the boss 0A0C00 beaten (the log\'s mission complete signal 018F92) and the next mission chosen by 01891E (mission end done -> mission 6); stops there')
p.add_argument('--continue',dest='cont',action='store_true',help='tap Start at the CONTINUE? panel (the arcade continue, MSX panel continue): the pad keeps going after its lives')
import ares_padtest as pt;pt.add_args(p)
p.add_argument('--ares',default='/workspace/emulator/build/rundir/bin/ares');p.add_argument('--data',default='/workspace/emulator/data146')
a=p.parse_args();out=Path(a.out);out.mkdir(parents=True,exist_ok=True)
if a.lifebar or a.bomb:   # N64 Z on the key z in a private copy of the ares data (tools/ares_private_data.py)
    import sys;sys.path.insert(0,str(Path(__file__).resolve().parent));from ares_private_data import private_data;a.data=private_data(a.data,out)
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
    import ares_menu;ares_menu.choose(key,log,proc,5,a.stage,lifebar=a.lifebar)
    wait(r'MSX MSELECT mission=5 stage=%d'%a.stage,20)
    m=wait(r'MSX mission 5 stage \d+ start: record (\w+) descriptor (\w+) camera list (\w+)',120)
    assert (m.group(2),m.group(3))==(('016E2E','0BB8E6'),('016E3E','0BB9BA'),('016E4E','0BBA24'))[a.stage-1],m.group(0)
    wait(r'MSX stage \d+ camera x=',300)
    if not a.pad_script:subprocess.run([XD,'windowmove',win,'0','0'],env=env);subprocess.run([XD,'windowfocus',win],env=env)   # the window grows when the game starts
    t0=time.monotonic();nextgrab=0;k=0;held={};next_start=0
    def hold(n,on):
        if held.get(n)!=on:key('keydown' if on else 'keyup',n);held[n]=on
    # The schedule and --seconds run on the game's clock (tools/ares_clock.py), not the host's: the same pad under any machine load.
    clock=EmuClock();wall_end=time.monotonic()+3*a.seconds+600
    while time.monotonic()<wall_end:
        assert proc.poll() is None,'ares exited'
        text=log();t=clock.now(text[-4000:])
        if t>=a.seconds:break
        assert 'ASSERTION FAILED' not in text and 'RSP CRASH' not in text,text[-3000:]
        mm=re.findall(r'MSX stage \d+ camera x=(-?\d+) y=(-?\d+)',text[-20000:])
        cam=int(mm[-1][0]) if mm else 0
        maxcam=max(maxcam,cam) if mm and re.search(r'MSX stage [678] camera',text[-20000:]) else maxcam
        if cam>=a.until:break
        # --back T: every T seconds the pad faces left for 1.5 s (an enemy behind the walker at a camera stop, the tank
        # 08FF7E at the screen's edge, is never hit by a pad that only holds right).
        back=bool(a.back) and (t%a.back)>a.back-1.5
        up=bool(a.up) and (t%a.up)>a.up-1.5
        hold('Right',not back and not up);hold('Left',back);hold('Up',up);hold('Down',a.down and not back and not up);hold('a',bool(a.jump) and (t%a.jump)<0.2);hold('b',a.fire and int(t*10)%2==0)
        hold('z',a.bomb and back and (t%a.back)<a.back-1.3)
        if a.boss and re.search(r'MSX mission end 018846 done[^\n]*-> mission 6',text):break
        # --continue: the panel's last state is "continue" (CONTINUE? 9..0): Start, as an arcade player would.
        if a.cont:
            pm=re.findall(r'MSX panel (play|continue|game over)',text[-20000:])
            if pm and pm[-1]=='continue' and t>=next_start:tap('Return',.4);next_start=t+1.0   # 0.4 s: longer than one pad read with the emulator at a third of its speed
        if t>=nextgrab:
            subprocess.run(['import','-display',os.environ['DISPLAY'],'-window','root',str(out/f'g{k:03d}_{cam}_{mm[-1][1] if mm else 0}.png')]);k+=1;nextgrab=t+a.period
        time.sleep(.03)
    for n in list(held):hold(n,False)
    pt.finish(a,proc,logp,[r'MSX mission end 018846 done'] if a.boss else [])
    text=log()
    stops=sorted(set(re.findall(r'(?:unsupported|explicit stop|not loaded|missing)[^\n]{0,100}',text)))
    print('camera',cam,'screenshots',k);print('counted stops and missing draws in the log (%d kinds):'%len(stops))
    for s in stops[:40]:print('  ',s)
    ok=cam>=a.progress and 'not loaded' not in text
    if a.boss:
        won='MSX mission complete signal (018F92)' in text;nxt=re.search(r'MSX mission end 018846 done[^\n]*',text)
        print('boss beaten (018F92):',won,'| next mission:',nxt.group(0) if nxt else None)
        ok=maxcam>=a.progress and 'not loaded' not in text and won and bool(nxt) and '-> mission 6' in nxt.group(0)   # the camera of the next mission follows
finally:
    proc.terminate()
    try:proc.wait(5)
    except Exception:proc.kill()
print('RESULT: PASS' if ok else 'RESULT: FAIL')
