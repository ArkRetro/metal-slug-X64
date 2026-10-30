#!/usr/bin/env python3
"""Traverse the whole Mission 1 in ares 146, from the mission start through
the boss to the mission-complete signal (docs/MISION1_COMPLETA.md).

Launch ares separately with the reference cartridge (make
mslugx-reference.z64: the release game with an invulnerable player, so that a
scripted run cannot die; never shipped) and the keyboard bound to the N64
pad (arrows, a=A jump, b=B fire, e=R, Return=Start); pass its log. The
script walks right firing, aims up every other half second and jumps every
few seconds; in the boss arena it alternates left/right. It captures
screenshots at the milestones and checks the log: the native script runs to
its end, the camera stops and autoscroll happen, the robed swordsman starts
the boss, the boss dies and signals the mission end (018F92), no descriptor
or palette is missing, and only the documented boss path stops (turret fire
0x08FA1A)."""
from pathlib import Path
import argparse,re,subprocess,threading,time
parser=argparse.ArgumentParser()
parser.add_argument('--log',type=Path,required=True)
parser.add_argument('--output',type=Path,required=True)
args=parser.parse_args();args.output.mkdir(parents=True,exist_ok=True)
XD='/usr/bin/xdotool'
def key(action,name):subprocess.run([XD,action,name],check=True)
def snap(name):subprocess.run(['import','-window','root',str(args.output/(name+'.png'))],check=True)
def log():return args.log.read_text(errors='replace')
def wait_for(pattern,timeout=120):
    deadline=time.monotonic()+timeout
    while time.monotonic()<deadline:
        text=log()
        assert 'ASSERTION FAILED' not in text and 'MSX ERROR' not in text,text[-3000:]
        if re.search(pattern,text):return
        time.sleep(.05)
    raise AssertionError((pattern,log()[-3000:]))
stop=threading.Event();arena=threading.Event()
def pad():
    # Fire held all the time; up every other half second; jump every 4 s;
    # right, or left/right/still in 1.5 s turns once the boss is fought.
    t0=time.monotonic();state={}
    def hold(name,on):
        if state.get(name)!=on:key('keydown' if on else 'keyup',name);state[name]=on
    while not stop.is_set():
        t=time.monotonic()-t0
        hold('b',True)
        hold('Up',int(t*2)%2==1)
        hold('a',(t%4.0)<0.15)
        if arena.is_set():
            phase=int(t/1.5)%3
            hold('Left',phase==0);hold('Right',phase==1)
        else:
            hold('Left',False);hold('Right',True)
        time.sleep(.03)
    for name in list(state):hold(name,False)
worker=threading.Thread(target=pad,daemon=True)
try:
    wait_for('MSX v10 scene loaded',timeout=400);wait_for('MSX first frame ready')
    window=subprocess.check_output([XD,'search','--onlyvisible','--class','ares'],text=True).split()[-1]
    subprocess.run([XD,'windowmove',window,'0','0'],check=True)
    subprocess.run([XD,'windowfocus','--sync',window],check=True);time.sleep(.5);snap('start')
    worker.start()
    wait_for(r'MSX rebel spawned=1 ',timeout=200)
    wait_for(r'MSX camera stop=872',timeout=400)
    wait_for(r'MSX camera stop=1920',timeout=900);time.sleep(1.0);snap('village-end')
    wait_for(r'MSX region end passed',timeout=900);snap('domed-building')
    wait_for(r'MSX camera autoscroll=1',timeout=900);time.sleep(.8);snap('autoscroll')
    wait_for(r'MSX backdrop palette 0B56CA',timeout=900);snap('truck-stop')
    wait_for(r'MSX camera stop=3192',timeout=900);time.sleep(.5);snap('helicopter-stop')
    wait_for(r'MSX boss spawned',timeout=900);time.sleep(.8);snap('boss-arrives')
    wait_for(r'MSX camera stop=3376',timeout=900);arena.set()
    wait_for(r'MSX boss start signal 10C606=00',timeout=900);time.sleep(1.5);snap('boss')
    wait_for(r'MSX boss phase=\d+ hp=2\d\d\d',timeout=1800);snap('boss-damaged')
    wait_for(r'MSX mission complete signal',timeout=2400);time.sleep(.5);snap('mission-complete')
    wait_for(r'MSX mission complete camera',timeout=600);time.sleep(.3);snap('mission-end')
    stop.set();worker.join(timeout=5)
    time.sleep(6)   # one more MSX PERF/AUDIO window
    text=log()
    assert 'MSX native draw missing descriptor' not in text,re.findall(r'MSX native draw missing.*',text)
    unsupported=re.findall(r'MSX world unsupported routine=(\w+)',text)
    stopped=set(re.findall(r'MSX boss stopped node routine=(\w+) label=(\w+)',text))
    assert not unsupported,unsupported
    assert stopped<={('d6168','8fa1a'),('d6254','b05060')},stopped
    spawned=max(int(n) for n in re.findall(r'MSX rebel spawned=(\d+)',text))
    killed=max(int(n) for n in re.findall(r'MSX defeated=(\d+)',text))
    perf=[(int(a),int(b)) for a,b in re.findall(r'MSX PERF: .*? cpu=(\d+)/(\d+)',text)]
    rdp=[tuple(map(int,m)) for m in re.findall(r'MSX RDP: draws=(\d+) .*CPU=(\d+) us .*misses=(\d+)',text)]
    audio=[int(n) for n in re.findall(r'MSX AUDIO .*underruns=(\d+)',text)]
    print('PASS: whole Mission 1 traversed: rebels spawned=%d killed=%d; boss started by 0A8C46, defeated; mission complete (018F92/018F40)'%(spawned,killed))
    print('boss nodes stopped (documented): %s'%sorted(stopped))
    if perf:print('MSX PERF windows=%d cpu mean max=%d us worst frame=%d us'%(len(perf),max(p[0] for p in perf),max(p[1] for p in perf)))
    if rdp:print('RDP samples=%d draws max=%d CPU max=%d us misses max=%d'%(len(rdp),max(r[0] for r in rdp),max(r[1] for r in rdp),max(r[2] for r in rdp)))
    if audio:print('audio underruns max=%d'%max(audio))
finally:
    stop.set()
    for name in ('Right','Left','Up','a','b','e','Return'):key('keyup',name)
