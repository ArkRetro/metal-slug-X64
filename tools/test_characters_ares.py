#!/usr/bin/env python3
"""The four characters in ares 146 on the normal cartridge
(mslugx-explorer.z64; docs/PERSONAJES.md). One boot per place of SOLDIER
SELECT (0 Marco, 1 Eri, 2 Tarma, 3 Fio): Start through the boot movies up to
the wait of SOLDIER SELECT, Right once per place (log MSX SELECT place=K
character=N), Start (MSX CHARACTER N chosen for the three packs), the choice
movie and the descent, then a few seconds of play (walk, fire, jump,
grenade, crouch), a pause and resume (MSX PAUSE on/off) and a walk on to the
first death (its voice 0C294E[+6E]: MSX AUDIO voice). Checks from the
log: the chosen character, no descriptor outside its catalog, no stopped
program, no part or pack error, no crash. Screenshots per place:
select.png (the cursor), play.png, paused.png. Uses the X display in DISPLAY.
Last line: RESULT: PASS/FAIL."""
import argparse,os,re,subprocess,sys,time
from pathlib import Path
from PIL import ImageGrab
p=argparse.ArgumentParser(description=__doc__)
p.add_argument('rom',type=Path);p.add_argument('--output',type=Path,required=True)
p.add_argument('--ares',default='/workspace/emulator/build/rundir/bin/ares')
p.add_argument('--places',default='0,1,2,3')
p.add_argument('--run-fire-turn',action='store_true',help='B83, with mslugx-diag.z64 (MSX_DIAG=1): after the landing, run and fire, '
    'keep running with the fire over, turn, crouch and fire, with the pistol; no update may draw the parts with two facings '
    '(MSX DIAG marco facing mixed)')
a=p.parse_args()
out=a.output.resolve();out.mkdir(parents=True,exist_ok=True)
env=dict(os.environ,XDG_DATA_HOME='/workspace/emulator/data146',XDG_CONFIG_HOME='/workspace/emulator/config146',
         XDG_RUNTIME_DIR='/workspace/emulator/runtime',LD_LIBRARY_PATH='/workspace/emulator/root/usr/lib/x86_64-linux-gnu',
         VK_DRIVER_FILES='/workspace/emulator/root/usr/share/vulkan/icd.d/lvp_icd.json',LIBGL_ALWAYS_SOFTWARE='1')
assert env.get('DISPLAY'),'DISPLAY'
XD='/usr/bin/xdotool';CHARACTER=(0,2,1,3);NAME=('Marco','Tarma','Eri','Fio')
def key(action,name):subprocess.run([XD,action,name],check=True)
# The play counts game time (tools/ares_clock.py: the cartridge's update count; Mission 1 prints `MSX clock ticks=N`) and
# the presses the cartridge logs are held until their line (tools/ares_menu.py): the same test at any machine load.
sys.path.insert(0,str(Path(__file__).resolve().parent));from ares_clock import EmuClock,game_sleep;import ares_menu
def tap(name,hold=.3):key('keydown',name);time.sleep(hold);key('keyup',name)
results={}
for place in [int(x) for x in a.places.split(',')]:
    who=CHARACTER[place];d=out/('place%d-%s'%(place,NAME[who].lower()));d.mkdir(exist_ok=True);log_path=d/'ares.log'
    def log():return log_path.read_text(errors='replace')
    def grab(name):ImageGrab.grab(xdisplay=env['DISPLAY']).save(d/name)
    process=subprocess.Popen([a.ares,'--setting','Video/Driver=OpenGL 3.2','--setting','Audio/Driver=None','--setting','General/HomebrewMode=true',
        '--system','Nintendo 64','--no-file-prompt',str(a.rom.resolve())],stdout=open(log_path,'w'),stderr=subprocess.STDOUT,env=env)
    def wait(pattern,seconds=120,start=0):
        deadline=time.monotonic()+4*seconds   # host time: four times the game's, for a loaded machine
        while time.monotonic()<deadline:
            text=log()
            assert 'ASSERTION FAILED' not in text and 'RSP CRASH' not in text and 'MSX ERROR' not in text,text[-3000:]
            m=re.search(pattern,text[start:])
            if m:return m
            assert process.poll() is None,'ares exited'
            time.sleep(.03)
        raise AssertionError('Timeout: '+pattern+'\n'+log()[-2000:])
    checks={}
    try:
        focused=False
        while 'MSX CINE select waiting' not in log():
            assert process.poll() is None,'ares exited'
            if not focused:
                found=subprocess.run([XD,'search','--onlyvisible','--class','ares'],text=True,capture_output=True).stdout.split()
                if found:subprocess.run([XD,'windowmove',found[-1],'0','0']);subprocess.run([XD,'windowfocus',found[-1]]);focused=True
            # Start only while the title or HOW TO PLAY runs: SOLDIER SELECT plays to its wait.
            t=log()
            if focused and (('MSX TITLE start' in t and 'MSX TITLE complete' not in t) or ('MSX CINE howto start' in t and 'MSX CINE howto complete' not in t) or ('MSX OPTIONS open' in t and 'MSX OPTIONS close' not in t)):tap('Return')
            time.sleep(1.0)
        window=subprocess.check_output([XD,'search','--onlyvisible','--class','ares'],text=True).split()[-1]
        subprocess.run([XD,'windowmove',window,'0','0'],check=True);subprocess.run([XD,'windowfocus','--sync',window],check=True)
        time.sleep(1)
        for k in range(1,place+1):
            # (ares runs at about half speed here: a press shorter than the movie's open, ~0.2 s, would not be seen released)
            time.sleep(1.2);mark=len(log());ares_menu.held(key,'Right',log,r'MSX SELECT place=\d+ character=\d+',15);m=wait(r'MSX SELECT place=(\d+) character=(\d+)',15,mark)
            assert int(m[1])==k,(k,m[0])
        time.sleep(1);grab('select.png')
        if place:checks['cursor on place %d = %s'%(place,NAME[who])]=bool(re.search(r'MSX SELECT place=%d character=%d\n'%(place,who),log()))
        mark=len(log());ares_menu.held(key,'Return',log,r'MSX CINE select chosen',15);wait(r'MSX CINE select chosen',15,mark)
        if who:wait(r'MSX CHARACTER %d chosen'%who,30,mark)
        m=wait(r'MSX CINE confirm setup',30,mark)
        wait(r'MSX mission handoff',90,mark);wait(r'MSX first frame ready',90,mark)
        checks['character %d in the game'%who]=(('MSX CHARACTER %d chosen'%who) in log()) if who else 'MSX CHARACTER' not in log()
        clock=EmuClock()
        def gs(seconds):game_sleep(clock,log,seconds)
        def gtap(name,hold=.15):key('keydown',name);gs(hold);key('keyup',name)
        gs(11)   # the descent and the landing
        assert 'MSX PAUSE' not in log(),'paused before the play'
        if a.run_fire_turn:
            mark=len(log())
            for side,other in (('Right','Left'),('Left','Right'),('Right','Left')):
                key('keydown',side);gs(.4)
                for _ in range(3):gtap('x');gs(.25)
                gs(1.2);key('keyup',side);key('keydown',other);gs(.8)       # the fire over, running: the turn
                gtap('x');gs(.15);key('keyup',other);key('keydown',side);gs(.6);key('keyup',side)   # the turn with the shot on
            key('keydown','Down');gs(.3);gtap('x');gs(.2);key('keydown','Left');gs(.3);key('keyup','Left');gtap('x');gs(.3);key('keyup','Down')
            checks['parts with one facing (run, fire, turn)']='MSX DIAG marco facing mixed' not in log()[mark:]
        key('keydown','Right');gs(1.5);gtap('x');gs(.3);gtap('x');gs(.5);gtap('z');gs(.8);key('keyup','Right')
        gs(.3);gtap('x');gs(.5);key('keydown','Down');gs(.6);gtap('x');gs(.6);key('keyup','Down')
        key('keydown','Up');gs(.4);gtap('x');gs(.5);key('keyup','Up');gs(.3)
        grab('play.png')
        # Start pauses only while player 1's panel runs its play routine (neo_pause.h: the SELECT of 015520):
        # a press during a death or the new life is not a pause, so it is tried again.
        mark=len(log())
        for attempt in range(5):
            ares_menu.held(key,'Return',log,r'MSX PAUSE on',6)
            try:wait(r'MSX PAUSE on',1,mark);break
            except AssertionError:
                if attempt==4 or 'Timeout' not in str(sys.exc_info()[1]):raise
        time.sleep(.6/max(clock.rate,.1));grab('paused.png')   # paused: the game's clock stands still
        mark=len(log());ares_menu.held(key,'Return',log,r'MSX PAUSE off',20);wait(r'MSX PAUSE off',10,mark)
        key('keydown','Left');gs(1.2);gtap('x');key('keyup','Left');gs(.5)
        checks['pause and resume']=True
        # The death voice 0C294E[+6E]: walk on without firing until the first death.
        VOICE=('1123','1087','1125','114E')[who];mark=len(log());key('keydown','Right')
        try:wait(r'MSX player death',150,mark)
        finally:key('keyup','Right')
        m=wait(r'MSX AUDIO voice %s: ([a-z ,0-9A-F]+)'%VOICE,10,mark)
        checks['death voice %s plays'%VOICE]=m[1].startswith('started')   # Tarma's 1087: driver twin of 1123 (AUDIO_JUEGO.md 19)
        text=log()
        checks['every pose in the catalog']='descriptor outside the catalog' not in text and 'MSX native draw missing descriptor' not in text
        checks['no stopped program']='MSX marco program stopped' not in text
        checks['no pack or part error']=not re.search(r'MSX character (part|\d+: (bad|.*missing))|MSX marco part \d',text)
    except AssertionError as e:
        checks['run']=False;print('place',place,str(e)[:1500])
    finally:
        process.terminate()
        try:process.wait(timeout=10)
        except subprocess.TimeoutExpired:process.kill()
        time.sleep(1)
    for k,v in checks.items():print(('ok   ' if v else 'FAIL ')+NAME[who]+': '+k)
    results[place]=bool(checks) and all(checks.values())
print('RESULT: %s (%s)'%('PASS' if results and all(results.values()) else 'FAIL',out))
