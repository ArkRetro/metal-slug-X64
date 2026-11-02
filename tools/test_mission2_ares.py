#!/usr/bin/env python3
"""Walk Mission 2 in ares 146 (docs/MISION2.md "Fase 2d"). Launch ares
separately with the Mission 2 reference cartridge (make
mslugx-m2-reference.z64: the reference game, invulnerable to hits and, on
its last ground, to falls below the screen; starting at Mission 2's first
record; camera stops of the script released when it stops at an unported
record or after 2400 steps at the stop) and pass its log. The pad walks
right firing and jumps; when the camera holds still it tries what a player
would (ground fire, the downward shot, frequent jumps). Checks: stage 1
ends by its native exit, stage 2 with its script (06835C native, vertical
modes 4/5) ends and loads stage 3, whose tower (writer 0B7C38: collision
and drawing from the same piece types) is climbed up to the boss arena,
to the boss 026254 (native) and the mission end (018F40 after the
sinking, tally, MISSION COMPLETE, Mission 3 loaded). Screenshots at each milestone;
the stage ends, tower pieces and the perf numbers are reported and the
last line is RESULT: PASS/FAIL. Stage 3 is walked by the reference build's
route planner (src/msx_autopilot.h, docs/MISION2.md "Fase 2f"), turned on
by Start in stage 3; the pad then only fires."""
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
    # Right, firing, a jump every 3 s. The log drives the rest: when the
    # camera has not moved for 5 s (a camera stop of the script, 0C7C12's
    # lock, waiting for its group; or a camera-list wait such as op 4's
    # rectangle 0C84BE, which wants the player on a ledge), the pad cycles
    # through what a player would try: walk back and fire on the ground
    # without jumping (soldiers below a ledge), jump with Down held (the
    # downward shot), then jump often while walking right (steps of the
    # climbs), then walk and jump left for 4 s (stage 3 at the camera 336:
    # the stairs up are to the left of the ledge the walk-in reaches). Falls below the screen put the reference player back on its
    # last ground (gameplay.c, pit rescues).
    t0=time.monotonic();state={};last=(None,t0)
    def hold(name,on):
        if state.get(name)!=on:key('keydown' if on else 'keyup',name);state[name]=on
    while not stop.is_set():
        now=time.monotonic();t=now-t0
        if pilot.is_set():
            hold('Right',False);hold('Left',False);hold('Down',False);hold('a',False);hold('b',int(t*20)%2==0)
            time.sleep(.03);continue
        cam=camera_now() if int(t*4)%4==0 else last[0]
        if cam!=last[0]:last=(cam,now)
        stuck=now-last[1]
        fire=int(t*20)%2==0;right=True;left=False;down=False;jump=(t%3.0)<0.2
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
def stage_report(stage):
    rows=[tuple(int(v) for v in m) for m in re.findall(r'MSX stage %d camera x=(-?\d+) y=(-?\d+) progress=(\d+) mode=(\d+) player x=(-?\d+) y=(-?\d+) grounded=(\d) unknown cells read=(\d+)'%stage,log())]
    if not rows:return 'stage %d: no camera samples'%stage
    return 'stage %d: camera x max=%d y range=%d..%d progress max=%d modes=%s player x max=%d unknown cells read=%d'%(
        stage,max(r[0] for r in rows),min(r[1] for r in rows),max(r[1] for r in rows),max(r[2] for r in rows),sorted({r[3] for r in rows}),max(r[4] for r in rows),max(r[7] for r in rows))
walked=[]
pilot=threading.Event()
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
    # B26: the exit 0B6ED2 walks the player into the doorway (handlers
    # 0CA3EC): the marker 0B6F6A (+15 bit 2, window B) over him.
    wait_for(r'MSX player handlers=054B18',timeout=240)
    for k in range(4):time.sleep(.4);snap('stage1-door-%d'%k)
    wait_for(r'MSX stage end \(018722\) mission 2: next stage 2',timeout=240)
    m=wait_for(r'MSX stage load mission 2 stage 2: .*',timeout=60);print(m.group(0))
    wait_for(r'MSX mission 2 stage 2 start',timeout=30);time.sleep(1.5);snap('stage2-start');walked.append(2)
    end=time.monotonic()+args.stage_seconds;shots=0
    while time.monotonic()<end:
        time.sleep(2);
        if time.monotonic()>end-args.stage_seconds+(shots+1)*args.stage_seconds/6:snap('stage2-%d'%shots);shots+=1
        if re.search(r'MSX stage end \(018722\) mission 2: next stage 3',log()):break
        # B26: the gate 03A6EA open, the player walked through its frame 03A82E.
        if 'door2' not in walked and 'MSX player handlers=054B18' in log()[log().find('MSX mission 2 stage 2 start'):]:
            walked.append('door2')
            for k in range(4):time.sleep(.4);snap('stage2-door-%d'%k)
    wait_for(r'MSX stage end \(018722\) mission 2: next stage 3',timeout=5);walked.append('2 done')
    m=wait_for(r'MSX stage load mission 2 stage 3: .*',timeout=60);print(m.group(0))
    wait_for(r'MSX mission 2 stage 3 start',timeout=30);time.sleep(2);snap('stage3-start');walked.append(3)
    # The route planner: Start toggles it in stage 3.
    pilot.set();time.sleep(.3)
    for attempt in range(3):
        key('keydown','Return');time.sleep(.2);key('keyup','Return')
        try:wait_for(r'MSX autopilot on',timeout=5);break
        except AssertionError:time.sleep(1)
    walked.append('autopilot')
    # The tower climb (writer 0B7C38) up to the boss arena, the boss 026254
    # (native, script record 2D9CA2) and the mission end.
    end=time.monotonic()+3*args.stage_seconds;shots=0;done=None
    def stage3():return log()[log().find('MSX mission 2 stage 3 start'):]
    while time.monotonic()<end and not done:
        time.sleep(2)
        if time.monotonic()>end-3*args.stage_seconds+(shots+1)*args.stage_seconds/3:snap('stage3-%d'%shots);shots+=1
        if 'boss spawned' not in ' '.join(map(str,walked)) and re.search(r'MSX boss spawned',stage3()):snap('stage3-boss');walked.append('boss spawned')
        done=re.search(r'MSX (mission complete signal \(018F92\)|mission end 018846 done)',stage3())
        stopped=re.search(r'MSX boss stopped node routine=(\w+) label=(\w+)',stage3())
        assert not stopped,stopped.group(0)
    assert done,'stage 3: no mission complete within %d s'%(3*args.stage_seconds)
    time.sleep(1);snap('mission-complete');walked.append('mission complete')
    # B24: the boss sinks and ends the stage (018F40 at 026E4A); its next
    # record is the mission end 018846: the tally, MISSION COMPLETE, the
    # fade and 01891E's Mission 3 (no softlock after the boss).
    wait_for(r'MSX mission complete camera=',timeout=120);walked.append('018F40')
    wait_for(r'MSX prisoner tally done',timeout=120)
    wait_for(r'MSX mission end letters arrived',timeout=60);snap('mission-complete-letters')
    wait_for(r'MSX mission end 018846 done',timeout=60)
    wait_for(r'MSX mission 3 stage \d+ start',timeout=60);time.sleep(2);snap('mission3-start');walked.append('mission 3')
finally:
    stop.set()
    for name in ('Right','Left','Up','a','b','Return'):
        try:key('keyup',name)
        except Exception:pass
text=log()
for s in (1,2,3):print(stage_report(s))
print('camera list events unsupported (max):',max([int(n) for n in re.findall(r'camera events unsupported=(\d+)',text)] or [0]))
pieces=re.findall(r'MSX tower piece (\d+) type (\d+)',text)
print('stage 3 tower: %d pieces (types %s); drawing and collision use the same types (scene_render_set_tower)'%(len(pieces),''.join('%x'%int(t) for _,t in pieces)))
ends=re.findall(r'MSX stage end \(018722\) mission 2: next stage (\d) \(exit hooks=(\d+), script lock releases=(\d+)\)',text)
print('stage ends:',ends)
pits=[tuple(map(int,m)) for m in re.findall(r'pit deaths=(\d+) rescues=(\d+)',text)]
if pits:print('pit deaths max=%d, reference rescues max=%d'%(max(p[0] for p in pits),max(p[1] for p in pits)))
lands=[int(n) for n in re.findall(r'kernel landings=(\d+)',text)]
if lands:print('air landings by the sweep 013968 where the plane test missed: %d'%max(lands))
falls=re.findall(r'MSX fall (?:rescue \d+|death) stage \d+: x=-?\d+ y=-?\d+',text)
print('fall log lines: %d%s'%(len(falls),(' (first: %s)'%falls[0]) if falls else ''))
perf=[(int(a),int(b)) for a,b in re.findall(r'MSX PERF: .*? cpu=(\d+)/(\d+)',text)]
rdp=[tuple(map(int,m)) for m in re.findall(r'MSX RDP: draws=(\d+) .*CPU=(\d+) us .*misses=(\d+)',text)]
audio=[int(n) for n in re.findall(r'MSX AUDIO .*underruns=(\d+)',text)]
if perf:print('MSX PERF windows=%d cpu mean max=%d us worst frame=%d us'%(len(perf),max(p[0] for p in perf),max(p[1] for p in perf)))
if rdp:print('RDP samples=%d draws max=%d CPU max=%d us misses max=%d'%(len(rdp),max(r[0] for r in rdp),max(r[1] for r in rdp),max(r[2] for r in rdp)))
if audio:print('audio underruns max=%d'%max(audio))
complete=[s for s in ('2','3') if any(e[0]==s for e in ends)]
print('script records of unported constructors skipped (reference):',max([int(n) for n in re.findall(r'unported constructor skipped \(count=(\d+)\)',text)] or [0]))
print('RESULT: %s Mission 2 walked: %s'%('PASS' if complete==['2','3'] and 'mission complete' in walked and 'mission 3' in walked and pieces else 'FAIL',', '.join(map(str,walked)) or 'none'))
