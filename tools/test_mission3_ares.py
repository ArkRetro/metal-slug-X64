#!/usr/bin/env python3
"""Walk Mission 3 in ares 146 (docs/MISION3.md). Launch ares separately with
the Mission 3 reference cartridge (make mslugx-m3-reference.z64: the
reference game, invulnerable to hits, starting at Mission 3's record
0173F6; script records of unported constructors skipped, camera stops of
the script released after 2400 steps) and pass its log. The pad of
tools/test_mission2_ares.py (right, firing, jumps; when the camera holds
still: ground fire, the downward shot, frequent jumps, left) walks the
single stage 4 for --stage-seconds. Report: how far the camera list went
(progress, camera x/y), the stage end if reached, every script record of an
unported constructor (record and constructor addresses), the world's
unsupported routines and missing draws. Last line: RESULT: PASS (the stage
loaded and the camera moved past --min-progress) or FAIL. MSX_XDOTOOL can
override the detected system/workspace xdotool executable."""
from pathlib import Path
import argparse,os,re,shutil,subprocess,threading,time
parser=argparse.ArgumentParser()
parser.add_argument('--log',type=Path,required=True)
parser.add_argument('--output',type=Path,required=True)
parser.add_argument('--stage-seconds',type=float,default=300)
parser.add_argument('--min-progress',type=int,default=1000)
parser.add_argument('--pilot',action='store_true',help='Start at the stage start: the reference route planner (msx_autopilot.h, with the platform 0B9286 as a floor) drives direction and jumps; the pad fires')
parser.add_argument('--hop',action='store_true',help='the Mission 3 pad: right, fire and repeated jumps, never left')
args=parser.parse_args()
override=os.environ.get('MSX_XDOTOOL')
if override:
    XD=shutil.which(override)
    if not XD:parser.error('MSX_XDOTOOL does not name an executable: '+override)
else:
    XD=next((p for p in ('/usr/bin/xdotool','/workspace/emulator/root/usr/bin/xdotool')
             if os.path.isfile(p) and os.access(p,os.X_OK)),None) or shutil.which('xdotool')
    if not XD:parser.error('xdotool not found; set MSX_XDOTOOL to an installed executable')
XD=str(Path(XD).resolve())
args.output.mkdir(parents=True,exist_ok=True)
def key(action,name):subprocess.run([XD,action,name],check=True)
def snap(name):subprocess.run(['import','-window','root',str(args.output/(name+'.png'))],check=True)
def log():return args.log.read_text(errors='replace')
def wait_for(pattern,timeout=120):
    deadline=time.monotonic()+timeout
    while time.monotonic()<deadline:
        text=log()
        assert 'ASSERTION FAILED' not in text and 'MSX ERROR' not in text and 'RSP CRASH' not in text,text[-3000:]
        m=re.search(pattern,text)
        if m:return m
        time.sleep(.05)
    raise AssertionError((pattern,log()[-3000:]))
stop=threading.Event()
def camera_now():
    m=re.findall(r'MSX stage \d camera x=(-?\d+) y=(-?\d+)',log()[-20000:])
    return m[-1] if m else None
def pad():
    t0=time.monotonic();state={};last=(None,t0)
    def hold(name,on):
        if state.get(name)!=on:key('keydown' if on else 'keyup',name);state[name]=on
    while not stop.is_set():
        now=time.monotonic();t=now-t0
        cam=camera_now() if int(t*4)%4==0 else last[0]
        if cam!=last[0]:last=(cam,now)
        stuck=now-last[1]
        fire=int(t*20)%2==0;right=True;left=False;down=False;jump=(t%3.0)<0.2
        if args.hop:
            # Mission 3 pad (--hop): right and fire with a jump pressed
            # again every 0.2 s, never left: the player is on the ground a
            # few frames at a time, so it does not walk off the end of the
            # ground at x 3840 before the platform 0B9286 (gap up to 56 px)
            # nor off the platform's ends while the camera stop holds it.
            jump=int(t*10)%2==0
            hold('b',fire);hold('Right',right);hold('Left',False);hold('Down',False);hold('a',jump)
            time.sleep(.03);continue
        if stuck>5:
            phase=int((stuck-5)/4)%4;u=(stuck-5)%4
            if phase==0:left=u<1.2;right=not left;jump=False
            elif phase==1:right=False;down=True;jump=(u%1.0)<0.2
            elif phase==2:jump=(u%1.0)<0.2;left=u>3.2;right=not left
            else:left=True;right=False;jump=(u%1.0)<0.2
        hold('b',fire);hold('Right',right);hold('Left',left);hold('Down',down);hold('a',jump)
        time.sleep(.03)
    for name in list(state):hold(name,False)
def boot_taps():
    focused=False
    while not stop.is_set() and 'MSX first frame ready' not in log():
        if not focused:
            found=subprocess.run([XD,'search','--onlyvisible','--class','ares'],text=True,capture_output=True).stdout.split()
            if found:subprocess.run([XD,'windowfocus',found[-1]]);focused=True
        if focused and re.search(r'MSX (TITLE start|CINE \w+ start)',log()):
            key('keydown','Return');time.sleep(.1);key('keyup','Return')
        time.sleep(1.5)
walked=[]
worker=threading.Thread(target=pad,daemon=True);tapper=threading.Thread(target=boot_taps,daemon=True)
try:
    tapper.start()
    wait_for('MSX v10 scene loaded',timeout=400)
    assert 'MSX reference build: player invulnerable' in log(),'run the Mission 3 reference cartridge (make mslugx-m3-reference.z64)'
    wait_for('MSX first frame ready')
    m=wait_for(r'MSX stage load mission 3 stage 1: .*',timeout=120);print(m.group(0))
    wait_for(r'MSX mission 3 stage 4 start',timeout=30)
    window=subprocess.check_output([XD,'search','--onlyvisible','--class','ares'],text=True).split()[-1]
    subprocess.run([XD,'windowmove',window,'0','0'],check=True)
    subprocess.run([XD,'windowfocus','--sync',window],check=True);time.sleep(1.0);snap('stage4-start');walked.append('stage 4 loaded')
    worker.start()
    if args.pilot:
        time.sleep(1.0);key('keydown','Return');time.sleep(.1);key('keyup','Return')
        wait_for('MSX autopilot on',timeout=10);walked.append('autopilot on')
    end=time.monotonic()+args.stage_seconds;shots=0
    while time.monotonic()<end:
        time.sleep(2)
        if args.pilot:
            # A second Start edge (seen once in ares) turns the planner off:
            # press again (counted).
            states=re.findall(r'MSX autopilot (on|off)',log())
            if states and states[-1]=='off':
                key('keydown','Return');time.sleep(.1);key('keyup','Return');walked.append('autopilot on again')
        if time.monotonic()>end-args.stage_seconds+(shots+1)*args.stage_seconds/8:snap('stage4-%d'%shots);shots+=1
        if re.search(r'MSX (mission complete signal \(018F92\)|stage end \(018722\) mission 3|mission end 018846 done)',log()):walked.append('stage end');break
        if re.search(r'MSX game over|Mission 1 restarts',log()):walked.append('game over');break
finally:
    stop.set()
    for name in ('Right','Left','Down','a','b','Return'):
        try:key('keyup',name)
        except Exception:pass
text=log();text=text[text.find('MSX mission 3 stage 4 start'):]
rows=[tuple(int(v) for v in m) for m in re.findall(r'MSX stage \d+ camera x=(-?\d+) y=(-?\d+) progress=(\d+) mode=(\d+) player x=(-?\d+) y=(-?\d+)',text)]
progress=max([r[2] for r in rows] or [0])
if rows:print('stage 4: camera x max=%d y range=%d..%d progress max=%d modes=%s player x max=%d'%(max(r[0] for r in rows),min(r[1] for r in rows),max(r[1] for r in rows),progress,sorted({r[3] for r in rows}),max(r[4] for r in rows)))
# Per section of the camera list 0B8968 (docs/MISION3.md "Reconocimiento
# del mapa"): progress bounds, lines seen, player x range, rescues and fall
# deaths added while in it (the periodic stage line's counters).
SECTIONS=[('1 salida',0,320),('2 vagones',320,3392),('3 hueco',3392,3712),('4 tramo final',3712,6304),('5 jefe',6304,1<<30)]
full=[tuple(int(v) for v in m) for m in re.findall(r'MSX stage \d+ camera x=(-?\d+) y=(-?\d+) progress=(\d+) mode=\d+ player x=(-?\d+) y=(-?\d+) .*?pit deaths=(\d+) rescues=(\d+)',text)]
prev=None;reached=[]
for name,lo,hi in SECTIONS:
    rs=[r for r in full if lo<=r[2]<hi]
    if not rs:print('section %s: not reached'%name);continue
    reached.append(name)
    i0=full.index(rs[0]);base=full[i0-1] if i0 else rs[0]
    print('section %s: %d lines, progress %d..%d, player x %d..%d, rescues %d, fall deaths %d'%(name,len(rs),min(r[2] for r in rs),max(r[2] for r in rs),
        min(r[3] for r in rs),max(r[3] for r in rs),rs[-1][6]-base[6],rs[-1][5]-base[5]))
if full:print('total rescues %d, fall deaths %d; boss arena (section 5) %s'%(full[-1][6],full[-1][5],'reached' if '5 jefe' in reached else 'not reached'))
for m in sorted(set(re.findall(r'MSX stage entry \w+ kind 2 .*',text))):print(m)
print('unknown cells read max:',max([int(n) for n in re.findall(r'unknown cells read=(\d+)',text)] or [0]))
print('camera list events unsupported (max):',max([int(n) for n in re.findall(r'camera events unsupported=(\d+)',text)] or [0]))
skips=sorted(set(re.findall(r'unported constructor skipped \(count=\d+\) record (\w+) constructor (\w+)',text)))
print('script records of unported constructors skipped: %d'%len(skips))
for rec,con in skips:print('  record %s constructor %s'%(rec,con))
for m in sorted(set(re.findall(r'MSX script stopped at (\w+)',text))):print('script stopped at',m)
print('world unsupported routines:',' '.join(sorted(set(re.findall(r'MSX world unsupported routine=(\w+)',text)))) or 'none')
print('native draws missing:',len(re.findall(r'MSX native draw missing',text)))
perf=[(int(a),int(b)) for a,b in re.findall(r'MSX PERF: .*? cpu=(\d+)/(\d+)',text)]
if perf:print('MSX PERF windows=%d cpu mean max=%d us worst frame=%d us'%(len(perf),max(p[0] for p in perf),max(p[1] for p in perf)))
print('RESULT: %s Mission 3 walked: %s (progress %d)'%('PASS' if walked and progress>=args.min_progress else 'FAIL',', '.join(walked) or 'none',progress))
