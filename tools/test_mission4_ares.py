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
p.add_argument('--ares',default='/workspace/emulator/build/rundir/bin/ares');p.add_argument('--data',default='/workspace/emulator/data146')
a=p.parse_args();out=Path(a.out);out.mkdir(parents=True,exist_ok=True)
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
    # Start through the cinematics; at SOLDIER SELECT hold L+R (keys d, e) and press A (the test menu), Up seven times (MISSION 4 is the seventh item from the end: the three stages of Mission 5 and of Mission 6 follow it), A.
    end=time.monotonic()+400
    while 'MSX MSELECT mission=' not in log():
        assert proc.poll() is None and time.monotonic()<end,'no test menu\n'+log()[-1500:]
        if re.search(r'MSX CINE select (start|waiting)',log()):
            time.sleep(1.5);key('keydown','d');key('keydown','e');time.sleep(.2);tap('a');time.sleep(.2);key('keyup','e');key('keyup','d')
            if 'MSX MSELECT menu open' in log():time.sleep(.5);[(tap('Up'),time.sleep(.3)) for _ in range(7)];tap('a');time.sleep(.5)
            continue
        # One Start for the title and one for HOW TO PLAY (the cinematic before SOLDIER SELECT); none after it,
        # or the press lands on SOLDIER SELECT and chooses before the menu combo.
        t=log()
        if ('MSX TITLE start' in t and 'MSX TITLE complete' not in t) or ('MSX CINE howto start' in t and 'MSX CINE howto complete' not in t):tap('Return')
        time.sleep(1.0)
    wait(r'MSX MSELECT mission=4',20)
    m=wait(r'MSX mission 4 stage \d+ start: record (\w+) descriptor (\w+) camera list (\w+)',120)
    assert m.group(2)=='016E1E' and m.group(3)=='0E741A',m.group(0)
    wait(r'MSX stage \d+ camera x=',300)
    subprocess.run([XD,'windowmove',win,'0','0'],env=env);subprocess.run([XD,'windowfocus',win],env=env)   # the window grows when the game starts
    t0=time.monotonic();nextgrab=0;k=0;held={}
    def hold(n,on):
        if held.get(n)!=on:key('keydown' if on else 'keyup',n);held[n]=on
    while time.monotonic()-t0<a.seconds:
        assert proc.poll() is None,'ares exited'
        t=time.monotonic()-t0;text=log()
        assert 'ASSERTION FAILED' not in text and 'RSP CRASH' not in text,text[-3000:]
        mm=re.findall(r'MSX stage \d+ camera x=(-?\d+) y=(-?\d+)',text[-20000:])
        cam=int(mm[-1][0]) if mm else 0
        if cam>=a.until:break
        hold('Right',True);hold('a',bool(a.jump) and (t%a.jump)<0.2);hold('b',a.fire and int(t*10)%2==0)
        if t>=nextgrab:
            subprocess.run(['import','-display',os.environ['DISPLAY'],'-window','root',str(out/f'g{k:03d}_{cam}_{mm[-1][1] if mm else 0}.png')]);k+=1;nextgrab=t+a.period
        time.sleep(.03)
    for n in list(held):hold(n,False)
    text=log()
    stops=sorted(set(re.findall(r'(?:unsupported|explicit stop|not loaded|missing)[^\n]{0,100}',text)))
    print('camera',cam,'screenshots',k);print('counted stops and missing draws in the log (%d kinds):'%len(stops))
    for s in stops[:40]:print('  ',s)
    ok=cam>=a.progress and 'not loaded' not in text
finally:
    proc.terminate()
    try:proc.wait(5)
    except Exception:proc.kill()
print('RESULT: PASS' if ok else 'RESULT: FAIL')
