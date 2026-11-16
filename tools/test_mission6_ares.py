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
import argparse,os,re,subprocess,time
from pathlib import Path
p=argparse.ArgumentParser(description=__doc__);p.add_argument('rom');p.add_argument('--out',required=True)
p.add_argument('--stage',type=int,default=1,choices=(1,2,3));p.add_argument('--seconds',type=float,default=90);p.add_argument('--period',type=float,default=3.0);p.add_argument('--progress',type=int,default=None)
p.add_argument('--jump',type=float,default=0.0);p.add_argument('--turn',type=float,default=0.0);p.add_argument('--continues',action='store_true');p.add_argument('--fire',action='store_true');p.add_argument('--until',type=int,default=99999)
p.add_argument('--ares',default='/workspace/emulator/build/rundir/bin/ares');p.add_argument('--data',default='/workspace/emulator/data146')
a=p.parse_args();out=Path(a.out);out.mkdir(parents=True,exist_ok=True)
import gzip,json
# The stage's camera limit in MAME (run4 replay, tools/test_m6_camera.c): the highest camera x of its fixture.
limit=max(r['x'] for r in json.load(gzip.open(Path(__file__).resolve().parents[1]/f'data/m6s{a.stage}_camera_fixture.json.gz'))['records'])>>16
if a.progress is None:a.progress=min(600,limit)
assert a.progress<=limit,f'--progress {a.progress} is beyond the camera x {limit} that stage {a.stage} reaches in MAME'
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
ok=False;cam=0
try:
    win=None
    while not win:
        r=subprocess.run([XD,'search','--onlyvisible','--class','ares'],text=True,capture_output=True,env=env).stdout.split()
        if r:win=r[-1]
        time.sleep(.5)
    subprocess.run([XD,'windowmove',win,'0','0'],env=env);subprocess.run([XD,'windowfocus',win],env=env)
    # Start through the cinematics; at SOLDIER SELECT hold L+R (keys d, e) and press A (the test menu), Up 4 - stage times (MISSION 6 STAGE 3 is the last item), A.
    end=time.monotonic()+400
    while 'MSX MSELECT mission=' not in log():
        assert proc.poll() is None and time.monotonic()<end,'no test menu\n'+log()[-1500:]
        if re.search(r'MSX CINE select (start|waiting)',log()):
            time.sleep(1.5);key('keydown','d');key('keydown','e');time.sleep(.2);tap('a');time.sleep(.2);key('keyup','e');key('keyup','d')
            if 'MSX MSELECT menu open' in log():time.sleep(.5);[(tap('Up'),time.sleep(.3)) for _ in range(4-a.stage)];tap('a');time.sleep(.5)
            continue
        # One Start for the title and one for HOW TO PLAY (the cinematic before SOLDIER SELECT); none after it,
        # or the press lands on SOLDIER SELECT and chooses before the menu combo.
        t=log()
        if ('MSX TITLE start' in t and 'MSX TITLE complete' not in t) or ('MSX CINE howto start' in t and 'MSX CINE howto complete' not in t) or ('MSX OPTIONS open' in t and 'MSX OPTIONS close' not in t):tap('Return')
        time.sleep(1.0)
    wait(r'MSX MSELECT mission=6 stage=%d'%a.stage,20)
    m=wait(r'MSX mission 6 stage \d+ start: record (\w+) descriptor (\w+) camera list (\w+)',120)
    assert (m.group(2),m.group(3))==(('016E5E','0BCA00'),('016E6E','0BDCE2'),('016E7E','0BE99E'))[a.stage-1],m.group(0)
    wait(r'MSX stage \d+ camera x=',300)
    subprocess.run([XD,'windowmove',win,'0','0'],env=env);subprocess.run([XD,'windowfocus',win],env=env)   # the window grows when the game starts
    t0=time.monotonic();nextgrab=0;k=0;held={};continued=0
    def hold(n,on):
        if held.get(n)!=on:key('keydown' if on else 'keyup',n);held[n]=on
    while time.monotonic()-t0<a.seconds:
        assert proc.poll() is None,'ares exited'
        t=time.monotonic()-t0;text=log()
        assert 'ASSERTION FAILED' not in text and 'RSP CRASH' not in text,text[-3000:]
        mm=re.findall(r'MSX stage \d+ camera x=(-?\d+) y=(-?\d+)',text[-20000:])
        cam=int(mm[-1][0]) if mm else 0
        if cam>=a.until:break
        # --continues: Start at every CONTINUE? panel (a player's choice, nothing of the game changes): the stage 016E5E
        # has water gaps the rafts 0BD71E bridge only with well-timed jumps, which the fixed --jump period misses at times.
        if a.continues and text.count('MSX panel continue')>continued:
            for n in list(held):hold(n,False)
            time.sleep(1.0);tap('Return');time.sleep(.5);continued=text.count('MSX panel continue')
        # --turn P: Left and Down for 0.5 s every P s (a player's choice: crouched shots at the enemies left behind, e.g.
        # the group-0 mortar soldiers 08A732 behind the sandbags that hold the camera lock 2DF9B8 at 1056).
        back=bool(a.turn) and (t%a.turn)<0.5
        hold('Right',not back);hold('Left',back);hold('Down',back);hold('a',bool(a.jump) and (t%a.jump)<0.2);hold('b',a.fire and int(t*10)%2==0)
        if t>=nextgrab:
            subprocess.run(['import','-display',os.environ['DISPLAY'],'-window','root',str(out/f'g{k:03d}_{cam}_{mm[-1][1] if mm else 0}.png')]);k+=1;nextgrab=t+a.period
        time.sleep(.03)
    for n in list(held):hold(n,False)
    text=log()
    stops=sorted(set(re.findall(r'(?:unsupported|explicit stop|not loaded|missing)[^\n]{0,100}',text)))
    print('camera',cam,'screenshots',k,'continues',continued,'fall deaths',(re.findall(r'pit deaths=(\d+)',text) or ['0'])[-1]);print('counted stops and missing draws in the log (%d kinds):'%len(stops))
    for s in stops[:40]:print('  ',s)
    ok=cam>=a.progress and 'not loaded' not in text
finally:
    proc.terminate()
    try:proc.wait(5)
    except Exception:proc.kill()
print('RESULT: PASS' if ok else 'RESULT: FAIL')
