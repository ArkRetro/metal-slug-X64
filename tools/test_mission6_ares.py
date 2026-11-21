#!/usr/bin/env python3
"""Mission 6 (stage --stage 1..3) from the test menu in ares 146 (docs/MISION6.md, phase 1).

Boots the normal cartridge (mslugx-explorer.z64, never the reference one),
taps Start to SOLDIER SELECT, opens the test menu (L+R and A), chooses
MISSION 6 STAGE n and holds right (with a jump every --jump seconds; no fire unless
--fire). Grabs a screenshot every --period seconds with the camera x in its
name. Checks from the log: the menu choice, the pack load with the flow's
descriptor and camera list (016E5E / 0BCA00, 016E6E / 0BDCE2, 016E7E / 0BE99E), the camera reaching --progress
(default 600, or the stage's last camera x in MAME when that is lower: the arena 016E7E stops at 320 in
data/m6s3_camera_fixture.json.gz, so 600 can never be reached there; a --progress above that x is refused),
no crash and no "not loaded" stop. Prints the counted stops of the log.
Last line: RESULT: PASS/FAIL. Needs DISPLAY (an Xvfb of its own)."""
import argparse,os,re,subprocess,sys,time
from pathlib import Path
sys.path.insert(0,str(Path(__file__).resolve().parent));from ares_clock import EmuClock
p=argparse.ArgumentParser(description=__doc__);p.add_argument('rom');p.add_argument('--out',required=True)
p.add_argument('--stage',type=int,default=1,choices=(1,2,3));p.add_argument('--seconds',type=float,default=90);p.add_argument('--period',type=float,default=3.0);p.add_argument('--progress',type=int,default=None)
p.add_argument('--jump',type=float,default=0.0);p.add_argument('--turn',type=float,default=0.0);p.add_argument('--up',type=float,default=0.0);p.add_argument('--sweep',type=float,default=0.0);p.add_argument('--continues',action='store_true');p.add_argument('--fire',action='store_true');p.add_argument('--until',type=int,default=99999)
# --next: the stage played to its end: PASS needs the next stage's record to start (stage 1 -> 016E6E, 2 -> 016E7E).
p.add_argument('--next',action='store_true')
# --boss (stage 3): PASS only when the last enemy 04153E is built natively, dies (04180A: `MSX mission complete signal (018F92)`,
# sent by 041DD4) and no routine stopped; the run ends 60 s (host time) after the signal (the escape 04677A begins).
p.add_argument('--boss',action='store_true')
p.add_argument('--lifebar',action='store_true',help='Z in the test menu: the optional life bar (not arcade); needs N64 Z on the key z (tools/ares_private_data.py)')
p.add_argument('--aimup',action='store_true',help='Up held all the time while walking (a player\'s choice: the last enemy 04153E hangs over the arena)')
p.add_argument('--bomb',type=float,default=0.0,help='a grenade (Z) every this many seconds (needs --lifebar: the key z)')
import ares_padtest as pt;pt.add_args(p)
p.add_argument('--ares',default='/workspace/emulator/build/rundir/bin/ares');p.add_argument('--data',default='/workspace/emulator/data146')
a=p.parse_args();out=Path(a.out);out.mkdir(parents=True,exist_ok=True)
if a.lifebar:
    import sys;sys.path.insert(0,str(Path(__file__).resolve().parent));from ares_private_data import private_data;a.data=private_data(a.data,out)
import gzip,json
# The stage's camera limit in MAME (run4 replay, tools/test_m6_camera.c): the highest camera x of its fixture.
limit=max(r['x'] for r in json.load(gzip.open(Path(__file__).resolve().parents[1]/f'data/m6s{a.stage}_camera_fixture.json.gz'))['records'])>>16
if a.progress is None:a.progress=min(600,limit)
assert a.progress<=limit,f'--progress {a.progress} is beyond the camera x {limit} that stage {a.stage} reaches in MAME'
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
    import ares_menu;ares_menu.choose(key,log,proc,6,a.stage,lifebar=a.lifebar)
    wait(r'MSX MSELECT mission=6 stage=%d'%a.stage,20)
    m=wait(r'MSX mission 6 stage \d+ start: record (\w+) descriptor (\w+) camera list (\w+)',120)
    assert (m.group(2),m.group(3))==(('016E5E','0BCA00'),('016E6E','0BDCE2'),('016E7E','0BE99E'))[a.stage-1],m.group(0)
    wait(r'MSX stage \d+ camera x=',300)
    if not a.pad_script:subprocess.run([XD,'windowmove',win,'0','0'],env=env);subprocess.run([XD,'windowfocus',win],env=env)   # the window grows when the game starts
    t0=time.monotonic();nextgrab=0;k=0;held={};continued=0;dead=0
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
        maxcam=max(maxcam,cam)
        if cam>=a.until:break
        nxt=a.next and a.stage<3 and re.search(r'MSX mission 6 stage \d+ start: record \w+ descriptor %s'%('016E6E','016E7E')[a.stage-1],text)
        if nxt:break
        if a.boss and 'MSX mission complete signal' in text:
            if not dead:dead=time.monotonic()
            if time.monotonic()-dead>60 or 'MSX mission end 018846 done' in text:break   # host time: the stage line (the game clock) stops with the mission's end
        # --continues: Start at every CONTINUE? panel (a player's choice, nothing of the game changes): the stage 016E5E
        # has water gaps the rafts 0BD71E bridge only with well-timed jumps, which the fixed --jump period misses at times.
        if a.continues and text.count('MSX panel continue')>continued:
            for n in list(held):hold(n,False)
            # Start until the panel takes it (`MSX panel play` again): one 0.1 s tap was less than one pad read with the
            # emulator at a third of its speed (a loaded machine), the countdown ran out and the game restarted in Mission 1.
            plays=text.count('MSX panel play');time.sleep(1.0)
            for _ in range(30):
                tap('Return',.4);time.sleep(.6);tx=log()
                if tx.count('MSX panel play')>plays or tx.count('MSX panel game over')>text.count('MSX panel game over'):break
            continued=text.count('MSX panel continue')
        # --turn P: Left and Down for 0.5 s every P s (a player's choice: crouched shots at the enemies left behind, e.g.
        # the group-0 mortar soldiers 08A732 behind the sandbags that hold the camera lock 2DF9B8 at 1056).
        back=bool(a.turn) and (t%a.turn)<0.5
        # --up P: Up for 0.5 s every P s (a player's choice: shots up at the jumpers 086712 hanging over the factory floor at 1104).
        up=bool(a.up) and not back and ((t+a.up/2)%a.up)<0.5
        hold('Up',up or a.aimup)
        # --sweep P: left for the first half of every P s, right for the second (a player's choice: under the camera hold
        # of 1104 the jumpers stand on the platforms all over the screen).
        left=back or (bool(a.sweep) and (t%a.sweep)<a.sweep/2)
        hold('Right',not left and not up);hold('Left',left and not up);hold('Down',back);hold('a',bool(a.jump) and (t%a.jump)<0.2);hold('b',a.fire and int(t*10)%2==0)
        if a.lifebar:hold('z',bool(a.bomb) and (t%a.bomb)<0.15)
        if t>=nextgrab:
            subprocess.run(['import','-display',os.environ['DISPLAY'],'-window','root',str(out/f'g{k:03d}_{cam}_{mm[-1][1] if mm else 0}.png')]);k+=1;nextgrab=t+a.period
        time.sleep(.03)
    for n in list(held):hold(n,False)
    pt.finish(a,proc,logp,([r'MSX mission end 018846 done'] if a.boss else [])+([r'MSX mission 6 stage \d+ start: record \w+ descriptor %s'%('016E6E','016E7E')[a.stage-1]] if a.next and a.stage<3 else []))
    text=log()
    stops=sorted(set(re.findall(r'(?:unsupported|explicit stop|not loaded|missing)[^\n]{0,100}',text)))
    print('camera',cam,'screenshots',k,'continues',continued,'fall deaths',(re.findall(r'pit deaths=(\d+)',text) or ['0'])[-1]);print('counted stops and missing draws in the log (%d kinds):'%len(stops))
    for s in stops[:40]:print('  ',s)
    # --boss: the camera glides back (04445A) and the mission ends. What the port does after Mission 6 (01891E: the ending
    # 0570A0 and the credits are not wired into the game yet: `MSX mission 7 not loaded (explicit stop)`) is outside this
    # test: printed, not judged.
    after=re.findall(r'MSX mission 7 not loaded[^\n]*',text) if a.boss else []
    if after:print('after the mission end (not judged):',after[0])
    ok=(maxcam if a.next or a.boss else cam)>=a.progress and 'not loaded' not in (text.replace(after[0],'') if after else text)
    if a.next:
        nxt=a.stage<3 and re.search(r'MSX mission 6 stage \d+ start: record \w+ descriptor %s'%('016E6E','016E7E')[a.stage-1],text)
        print('next stage started' if nxt else 'next stage not started');ok=ok and bool(nxt)
    if a.boss:
        for pat in (r'MSX script spawn record \w+ constructor 04153e native 1',r'MSX mission complete signal \(018F92\)'):
            m=re.search(pat,text);print('boss:',pat,'->',m.group(0) if m else 'MISSING');ok=ok and m is not None
        bad=re.findall(r'MSX world unsupported routine[^\n]*',text);print('boss: routine stops',len(bad),bad[:5]);ok=ok and not bad
finally:
    for n in ('Right','Left','Up','Down','a','b','d','e','z'):key('keyup',n)
    proc.terminate()
    try:proc.wait(5)
    except Exception:proc.kill()
print('RESULT: PASS' if ok else 'RESULT: FAIL')
