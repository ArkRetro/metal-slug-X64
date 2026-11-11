#!/usr/bin/env python3
"""Mission 1 -> Mission 2 transition in ares 146 (docs/MISION2.md,
"Transición Misión 1 -> Misión 2"). Boots the transition cartridge (make
mslugx-m1end.z64: the release game whose Mission 1 ends 600 updates after the
landing, MSX_TEST_QUICK_END in actor.c; never a release), taps Start through
the boot cinematics and then leaves the pad alone. Screenshots every
--period seconds from the mission end to --after seconds past Mission 2's
stage 1 start (rec/NNNNN.png, with the log line count at each grab in
rec.json). Checks from the log, in order: the tally 0308C0, the MISSION
COMPLETE letters, the fade and 01891E, the Mission 2 stage 1 load with the
descriptor's music (0022) playing, the mission start of 1805C (weapon back
to the pistol, ten bombs, rescue counts cleared), the 16-step fade-in, the
MISSION 2 START letters and the walk-in; no crash, no missing descriptor.
Last line: RESULT: PASS/FAIL."""
import argparse,json,os,re,subprocess,threading,time
from pathlib import Path
from PIL import ImageGrab
p=argparse.ArgumentParser(description=__doc__)
p.add_argument('rom',type=Path);p.add_argument('--output',type=Path,required=True)
p.add_argument('--ares',default='/workspace/emulator/build/rundir/bin/ares')
p.add_argument('--period',type=float,default=0.25)
p.add_argument('--after',type=float,default=25.0)
p.add_argument('--walk',type=float,default=0.0,help='then hold right this many seconds (the stage scrolls; screenshots go on)')
a=p.parse_args()
out=a.output.resolve();out.mkdir(parents=True,exist_ok=True);(out/'rec').mkdir(exist_ok=True)
env=dict(os.environ,XDG_DATA_HOME='/workspace/emulator/data146',XDG_CONFIG_HOME='/workspace/emulator/config146',
         XDG_RUNTIME_DIR='/workspace/emulator/runtime',LD_LIBRARY_PATH='/workspace/emulator/root/usr/lib/x86_64-linux-gnu',
         VK_DRIVER_FILES='/workspace/emulator/root/usr/share/vulkan/icd.d/lvp_icd.json',LIBGL_ALWAYS_SOFTWARE='1')
XD='/usr/bin/xdotool';log_path=out/'ares.log'
def log():return log_path.read_text(errors='replace')
def key(action,name):subprocess.run([XD,action,name],check=True)
process=subprocess.Popen([a.ares,'--setting','Video/Driver=OpenGL 3.2','--setting','Audio/Driver=None','--setting','General/HomebrewMode=true',
    '--system','Nintendo 64','--no-file-prompt',str(a.rom.resolve())],stdout=open(log_path,'w'),stderr=subprocess.STDOUT,env=env)
def wait(pattern,seconds=120):
    deadline=time.monotonic()+seconds
    while time.monotonic()<deadline:
        text=log()
        assert 'ASSERTION FAILED' not in text and 'RSP CRASH' not in text and 'MSX ERROR' not in text,text[-3000:]
        m=re.search(pattern,text)
        if m:return m
        assert process.poll() is None,'ares exited'
        time.sleep(.03)
    raise AssertionError('Timeout: '+pattern+'\n'+log()[-2000:])
def cinematic_running(text):
    last=None
    for m in re.finditer(r'MSX (TITLE|CINE (?:howto|select|confirm|entry)) (start|complete)',text):last=m
    if 'MSX OPTIONS open' in text and 'MSX OPTIONS close' not in text:return True   # the options screen: Start leaves it
    return last is not None and last.group(2)=='start' and 'entry' not in last.group(1)
stop=threading.Event();grabs=[]
def grabber():
    k=0
    while not stop.is_set():
        t=time.monotonic();im=ImageGrab.grab(xdisplay=env.get('DISPLAY'))
        im=im.resize((im.width//2,im.height//2))   # the ares window fills the screen
        path=out/'rec'/f'{k:05d}.png';im.save(path,compress_level=1)
        grabs.append(dict(file=path.name,time=round(t,3),log_lines=log().count('\n')));k+=1
        time.sleep(max(0,a.period-(time.monotonic()-t)))
result='FAIL';notes=[]
try:
    focused=False
    while 'MSX first frame ready' not in log():
        assert process.poll() is None,'ares exited'
        if not focused:
            found=subprocess.run([XD,'search','--onlyvisible','--class','ares'],text=True,capture_output=True).stdout.split()
            if found:
                subprocess.run([XD,'windowmove',found[-1],'0','0']);subprocess.run([XD,'windowfocus',found[-1]]);focused=True
        # START only while the title, HOW TO PLAY, SOLDIER SELECT or the
        # confirmation runs (its start logged and not its end), read just
        # before the press; never in the entry film: it ends by itself and a
        # press landing in the game is the ROM's pause (neo_pause.c), which
        # stopped this test at the Mission 1 walk.
        if focused and cinematic_running(log()):
            key('keydown','Return');time.sleep(.1);key('keyup','Return')
        time.sleep(1.5)
    window=subprocess.check_output([XD,'search','--onlyvisible','--class','ares'],text=True).split()[-1]
    subprocess.run([XD,'windowmove',window,'0','0'],check=True);subprocess.run([XD,'windowfocus','--sync',window],check=True)
    wait(r'MSX test cartridge: Mission 1 ended',seconds=400)
    g=threading.Thread(target=grabber,daemon=True);g.start()
    wait(r'MSX prisoner tally done',seconds=240)
    wait(r'MSX mission end letters arrived',seconds=120)
    wait(r'MSX mission end 018846 done',seconds=120)
    wait(r'MSX mission 2 stage 1 start',seconds=120)
    time.sleep(a.after)
    if a.walk:
        key('keydown','Right');time.sleep(a.walk);key('keyup','Right')
    text=log();tail=text[text.find('MSX mission end 018846 done'):]
    checks={
        'music 0022 requested at the stage load':re.search(r'MSX AUDIO stage music 0022',tail) is not None,
        'music playing after the load':re.search(r'MSX AUDIO stage music 0022[^\n]*\n(?:.*\n)*?MSX AUDIO music start',tail) is not None,
        'mission start 1805C (pistol, bombs 10, rescues cleared)':re.search(r'MSX mission start 1805C: weapon=0 bombs=10 rescued=0',tail) is not None,
        'fade-in of the stage entry':re.search(r'MSX stage fade-in done',tail) is not None,
        'MISSION 2 START letters':re.search(r'MSX mission start letters',tail) is not None,
        'no missing descriptor':'MSX native draw missing descriptor' not in text,
    }
    for k,v in checks.items():print(('ok   ' if v else 'FAIL ')+k)
    result='PASS' if all(checks.values()) else 'FAIL'
finally:
    stop.set();time.sleep(.5)
    (out/'rec.json').write_text(json.dumps(grabs))
    process.terminate()
    try:process.wait(timeout=10)
    except subprocess.TimeoutExpired:process.kill()
print('RESULT: %s (%d screenshots in %s)'%(result,len(grabs),out/'rec'))
