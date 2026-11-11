#!/usr/bin/env python3
"""The original's pause 015520 (docs/PAUSA.md) in ares 146 on the normal
cartridge (mslugx-explorer.z64). Taps Start through the boot cinematics,
plays a few seconds (walk right), then: Start (the SELECT of the Neo Geo
pad) pauses (log MSX PAUSE on); while Right, A and B are held for --hold
seconds the picture is darker than the play (the fade 009C0A to -8) and
stands still but for the cells of the blinking PAUSE (fix 725A: column 18,
row 26), which is seen both shown and hidden, with no simulation log;
Start resumes (MSX PAUSE off), the picture is as bright as before and the
game goes on; then a second pause ended by R (MSX reset, MSX PAUSE off
(reset)). Uses the X display in DISPLAY (one shared display per agent).
Last line: RESULT: PASS/FAIL."""
import argparse,os,re,subprocess,time
from pathlib import Path
from PIL import ImageGrab,ImageChops,ImageStat
p=argparse.ArgumentParser(description=__doc__)
p.add_argument('rom',type=Path);p.add_argument('--output',type=Path,required=True)
p.add_argument('--ares',default='/workspace/emulator/build/rundir/bin/ares')
p.add_argument('--hold',type=float,default=6.0)
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
    return im.crop((0,24,im.width,im.height-70))   # the picture, not ares' status bar (its VPS counter)
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
    key('keydown','Right');time.sleep(2);key('keyup','Right');time.sleep(.5)
    def bright(im):return sum(ImageStat.Stat(im.convert('RGB')).mean)
    play=grab('play.png')
    mark=len(log());tap('Return');wait(r'MSX PAUSE on',10,mark)
    time.sleep(1.5)   # the four turns of the fade
    key('keydown','Right');key('keydown','x');key('keydown','z')   # held while paused: no effect
    mark=len(log());shots=[]
    for k in range(int(a.hold/.25)):shots.append(grab('paused-%02d.png'%k));time.sleep(.25)
    key('keyup','Right');key('keyup','x');key('keyup','z')
    during=log()[mark:]
    w,h=shots[0].size;boxes=[ImageChops.difference(shots[0].convert('RGB'),x.convert('RGB')).getbbox() for x in shots[1:]]
    # The only cells that change: PAUSE at fix column 18..22, row 26 (lower middle of the picture).
    checks['picture still while paused but for PAUSE']=all(b is None or (b[0]>=w*.40 and b[2]<=w*.62 and b[1]>=h*.85 and b[3]<=h*.99) for b in boxes)
    checks['PAUSE blinks (shown and hidden)']=any(b is not None for b in boxes)
    checks['paused picture darker (fade to -8)']=bright(shots[-1])<bright(play)*.9
    checks['no simulation log while paused']=not re.search(r'MSX (rebel spawned|enemy hit|defeated|player death|script spawn)',during)
    second=shots[-1]
    mark=len(log());tap('Return');wait(r'MSX PAUSE off',10,mark)
    key('keydown','Right');time.sleep(2);key('keyup','Right')
    moved=grab('resumed.png')
    checks['picture moves after resume']=ImageChops.difference(second.convert('RGB'),moved.convert('RGB')).getbbox() is not None
    checks['bright again after resume']=bright(moved)>bright(second)*1.05
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
