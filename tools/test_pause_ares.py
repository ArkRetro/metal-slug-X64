#!/usr/bin/env python3
"""Pause (not arcade; docs/PORT_STATUS.md "Pausa") in ares 146 on the normal
cartridge (mslugx-explorer.z64). Taps Start through the boot cinematics,
plays a few seconds (walk right), then: Start pauses (log MSX PAUSE on),
the picture stands still for --hold seconds while Right, A and B are held
(two screenshots equal, no simulation log), Start resumes (MSX PAUSE off),
the game goes on; then a second pause ended by R (MSX reset, MSX PAUSE off
(reset)). Uses the X display in DISPLAY (one shared display per agent).
Last line: RESULT: PASS/FAIL."""
import argparse,os,re,subprocess,time
from pathlib import Path
from PIL import ImageGrab,ImageChops
p=argparse.ArgumentParser(description=__doc__)
p.add_argument('rom',type=Path);p.add_argument('--output',type=Path,required=True)
p.add_argument('--ares',default='/workspace/emulator/build/rundir/bin/ares')
p.add_argument('--hold',type=float,default=3.0)
a=p.parse_args()
out=a.output.resolve();out.mkdir(parents=True,exist_ok=True)
env=dict(os.environ,XDG_DATA_HOME='/workspace/emulator/data146',XDG_CONFIG_HOME='/workspace/emulator/config146',
         XDG_RUNTIME_DIR='/workspace/emulator/runtime',LD_LIBRARY_PATH='/workspace/emulator/root/usr/lib/x86_64-linux-gnu',
         VK_DRIVER_FILES='/workspace/emulator/root/usr/share/vulkan/icd.d/lvp_icd.json',LIBGL_ALWAYS_SOFTWARE='1')
assert env.get('DISPLAY'),'DISPLAY'
XD='/usr/bin/xdotool';log_path=out/'ares.log'
def log():return log_path.read_text(errors='replace')
def key(action,name):subprocess.run([XD,action,name],check=True)
def tap(name):key('keydown',name);time.sleep(.15);key('keyup',name)
def grab(name):
    im=ImageGrab.grab(xdisplay=env['DISPLAY']);im.save(out/name)
    return im.crop((0,24,im.width,im.height*5//6))   # the picture, not ares' status bar (its VPS counter)
process=subprocess.Popen([a.ares,'--setting','Video/Driver=OpenGL 3.2','--setting','Audio/Driver=None','--setting','General/HomebrewMode=true',
    '--system','Nintendo 64','--no-file-prompt',str(a.rom.resolve())],stdout=open(log_path,'w'),stderr=subprocess.STDOUT,env=env)
def wait(pattern,seconds=120,start=0):
    deadline=time.monotonic()+seconds
    while time.monotonic()<deadline:
        text=log()
        assert 'ASSERTION FAILED' not in text and 'RSP CRASH' not in text and 'MSX ERROR' not in text,text[-3000:]
        m=re.search(pattern,text[start:])
        if m:return m
        assert process.poll() is None,'ares exited'
        time.sleep(.03)
    raise AssertionError('Timeout: '+pattern+'\n'+log()[-2000:])
result='FAIL';checks={}
try:
    focused=False
    while 'MSX first frame ready' not in log():
        assert process.poll() is None,'ares exited'
        if not focused:
            found=subprocess.run([XD,'search','--onlyvisible','--class','ares'],text=True,capture_output=True).stdout.split()
            if found:subprocess.run([XD,'windowmove',found[-1],'0','0']);subprocess.run([XD,'windowfocus',found[-1]]);focused=True
        # Start only while a movie runs: after the handoff Start is the pause.
        if focused and 'MSX mission handoff' not in log() and re.search(r'MSX (TITLE start|CINE \w+ start)',log()):tap('Return')
        time.sleep(1.5)
    window=subprocess.check_output([XD,'search','--onlyvisible','--class','ares'],text=True).split()[-1]
    subprocess.run([XD,'windowmove',window,'0','0'],check=True);subprocess.run([XD,'windowfocus','--sync',window],check=True)
    time.sleep(12)   # the descent and the landing
    assert 'MSX PAUSE' not in log(),'paused by a boot tap'
    key('keydown','Right');time.sleep(2);key('keyup','Right')
    mark=len(log());tap('Return');wait(r'MSX PAUSE on',10,mark)
    time.sleep(.5)
    key('keydown','Right');key('keydown','x');key('keydown','z')   # held while paused: no effect
    first=grab('paused-1.png');mark=len(log());time.sleep(a.hold);second=grab('paused-2.png')
    key('keyup','Right');key('keyup','x');key('keyup','z')
    during=log()[mark:]
    checks['picture still while paused']=ImageChops.difference(first.convert('RGB'),second.convert('RGB')).getbbox() is None
    checks['no simulation log while paused']=not re.search(r'MSX (rebel spawned|enemy hit|defeated|player death|script spawn)',during)
    mark=len(log());tap('Return');wait(r'MSX PAUSE off',10,mark)
    key('keydown','Right');time.sleep(2);key('keyup','Right')
    moved=grab('resumed.png')
    checks['picture moves after resume']=ImageChops.difference(second.convert('RGB'),moved.convert('RGB')).getbbox() is not None
    mark=len(log());tap('Return');wait(r'MSX PAUSE on',10,mark)
    mark=len(log());tap('e');wait(r'MSX PAUSE off \(reset\)',10,mark);wait(r'MSX reset',10,mark)
    checks['R resets from the pause']=True
    time.sleep(2)
    text=log()
    checks['no missing descriptor']='MSX native draw missing descriptor' not in text
    for k,v in checks.items():print(('ok   ' if v else 'FAIL ')+k)
    result='PASS' if all(checks.values()) else 'FAIL'
finally:
    process.terminate()
    try:process.wait(timeout=10)
    except subprocess.TimeoutExpired:process.kill()
print('RESULT: %s (%s)'%(result,out))
