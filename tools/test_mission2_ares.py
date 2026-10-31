#!/usr/bin/env python3
"""Walk Mission 2 in ares 146 as far as the engine goes (docs/MISION2.md
"Fase 2"). Launch ares separately with the Mission 2 reference cartridge
(make mslugx-m2-reference.z64: the reference game, invulnerable, starting at
Mission 2's first record, with the reference stage-end hook of gameplay.c,
MSX_REFERENCE_STAGE_EXIT, in place of the stage-1 exit actor 0B6F24 that is
not ported) and pass its log. The pad walks right firing and jumps every few
seconds. Checks: the mission pack loads, stage 1 starts from its own camera
list and script, the camera reaches the list's hold (320), the hook ends
the stage and the flow's next record loads stage 2, whose camera then
follows its list (vertical modes included). Screenshots at each milestone;
the last line summarises how far it walked."""
from pathlib import Path
import argparse,re,subprocess,threading,time
parser=argparse.ArgumentParser()
parser.add_argument('--log',type=Path,required=True)
parser.add_argument('--output',type=Path,required=True)
parser.add_argument('--stage-seconds',type=float,default=150)
args=parser.parse_args();args.output.mkdir(parents=True,exist_ok=True)
XD='/usr/bin/xdotool'
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
    # Right, firing, a jump every 3 s. When the camera has not moved for 6 s
    # (a camera-list wait such as op 4's rectangle, 0C84BE, which wants the
    # player on a ledge), back off left for a second and jump more often.
    t0=time.monotonic();state={};last=(None,t0);back_until=0
    def hold(name,on):
        if state.get(name)!=on:key('keydown' if on else 'keyup',name);state[name]=on
    while not stop.is_set():
        now=time.monotonic();t=now-t0
        cam=camera_now() if int(t*4)%4==0 else last[0]
        if cam!=last[0]:last=(cam,now)
        stuck=now-last[1]>6
        if stuck and now>back_until+3:back_until=now+1.0+(int(t)%3)*0.4
        left=now<back_until
        hold('b',int(t*20)%2==0);hold('Right',not left);hold('Left',left)
        hold('a',(t%(1.2 if stuck else 3.0))<0.2)
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
def stage_report(stage):
    rows=[tuple(int(v) for v in m) for m in re.findall(r'MSX stage %d camera x=(-?\d+) y=(-?\d+) progress=(\d+) mode=(\d+) player x=(-?\d+) y=(-?\d+) grounded=(\d) unknown cells read=(\d+)'%stage,log())]
    if not rows:return 'stage %d: no camera samples'%stage
    return 'stage %d: camera x max=%d y range=%d..%d progress max=%d modes=%s player x max=%d unknown cells read=%d'%(
        stage,max(r[0] for r in rows),min(r[1] for r in rows),max(r[1] for r in rows),max(r[2] for r in rows),sorted({r[3] for r in rows}),max(r[4] for r in rows),max(r[7] for r in rows))
walked=[]
worker=threading.Thread(target=pad,daemon=True);tapper=threading.Thread(target=boot_taps,daemon=True)
try:
    tapper.start()
    wait_for('MSX v10 scene loaded',timeout=400)
    assert 'MSX reference build: player invulnerable' in log(),'run the Mission 2 reference cartridge (make mslugx-m2-reference.z64)'
    wait_for('MSX first frame ready')
    m=wait_for(r'MSX stage load mission 2 stage 1: .*',timeout=120);print(m.group(0))
    wait_for(r'MSX mission 2 stage 1 start',timeout=30)
    window=subprocess.check_output([XD,'search','--onlyvisible','--class','ares'],text=True).split()[-1]
    subprocess.run([XD,'windowmove',window,'0','0'],check=True)
    subprocess.run([XD,'windowfocus','--sync',window],check=True);time.sleep(1.0);snap('stage1-start')
    worker.start()
    wait_for(r'MSX stage 1 camera x=(1\d\d)',timeout=120);snap('stage1-middle')
    wait_for(r'MSX stage 1 camera x=320 ',timeout=240);time.sleep(.5);snap('stage1-end');walked.append(1)
    wait_for(r'MSX stage end \(018722\) mission 2: next stage 2',timeout=240)
    m=wait_for(r'MSX stage load mission 2 stage 2: .*',timeout=60);print(m.group(0))
    wait_for(r'MSX mission 2 stage 2 start',timeout=30);time.sleep(1.5);snap('stage2-start');walked.append(2)
    end=time.monotonic()+args.stage_seconds;shots=0
    while time.monotonic()<end:
        time.sleep(args.stage_seconds/6);snap('stage2-%d'%shots);shots+=1
        if re.search(r'MSX stage end \(018722\) mission 2: next stage 3',log()):break
    if re.search(r'MSX mission 2 stage 3 start',log()):
        walked.append(3);time.sleep(2);snap('stage3-start')
        time.sleep(args.stage_seconds/2);snap('stage3-later')
finally:
    stop.set()
    for name in ('Right','Left','Up','a','b','Return'):
        try:key('keyup',name)
        except Exception:pass
text=log()
for s in (1,2,3):print(stage_report(s))
print('camera list events unsupported (max):',max([int(n) for n in re.findall(r'camera events unsupported=(\d+)',text)] or [0]))
print('RESULT: Mission 2 walked through stage(s) %s'%(','.join(map(str,walked)) or 'none'))
