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
or palette is missing, and no boss node stops. Between the region end and
the 2336 stop it jumps often to land on the Camel Slug (screenshots
camel*.png when Marco mounts it)."""
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
        assert 'ASSERTION FAILED' not in text and 'MSX ERROR' not in text and 'RSP CRASH' not in text,text[-3000:]
        if re.search(pattern,text):return
        time.sleep(.05)
    raise AssertionError((pattern,log()[-3000:]))
stop=threading.Event();arena=threading.Event();domes=threading.Event();shop=threading.Event();camel_zone=threading.Event()
shop_t=[0.0];t_start=[0.0]
def pad():
    # Fire tapped (10 presses a second: the H fires a burst per press,
    # +9B bit 4 is the press edge 10E9EC); up every other half second; jump every 4 s;
    # right, or left/right/still in 1.5 s turns once the boss is fought.
    t0=time.monotonic();t_start[0]=t0;state={}
    def hold(name,on):
        if state.get(name)!=on:key('keydown' if on else 'keyup',name);state[name]=on
    while not stop.is_set():
        t=time.monotonic()-t0
        hold('b',int(t*20)%2==0)
        hold('Up',int(t*2)%2==1)
        # Between the domes' collapse (1920) and the 2336 stop the Camel Slug
        # 03458E waits at its record (2216): jump often so that a fall lands
        # on it (0AA610 takes a falling player only) and Marco rides it.
        hold('a',(t%(0.9 if camel_zone.is_set() and not domes.is_set() else 4.0))<0.15)
        if domes.is_set():
            # Domed building (054904): sweep under the three domes firing up.
            hold('Up',True);sweep=int(t/5.0)%2
            hold('Left',sweep==0);hold('Right',sweep==1)
        elif shop.is_set():
            # Destructible shop (0B6566) at the 2336 stop: back off to its
            # left, then fire right at it.
            hold('Up',False);back=(t-shop_t[0])<1.2
            hold('Left',back);hold('Right',False)
            if not back and int(t*4)%8==0:hold('Right',True)
        elif arena.is_set():
            phase=int(t/1.5)%3
            hold('Left',phase==0);hold('Right',phase==1)
        else:
            hold('Left',False);hold('Right',True)
        time.sleep(.03)
    for name in list(state):hold(name,False)
worker=threading.Thread(target=pad,daemon=True)
def milestones():
    # Marco with the H (weapon id 0A from the prisoner's gift): a few
    # screenshots once the weapon changes, while the pad keeps firing; and
    # Marco riding the Camel Slug 03458E if the pad lands on it (0C3078).
    h=camel=False
    while not stop.is_set() and not (h and camel):
        text=log()
        if not h and re.search(r'MSX weapon id=a ',text):
            h=True
            for i in range(4):time.sleep(.7);snap('hgun%d'%i)
        if not camel and 'MSX player mounted vehicle' in text:
            camel=True
            for i in range(4):time.sleep(.25);snap('camel%d'%i)
        time.sleep(.1)
watcher=threading.Thread(target=milestones,daemon=True)
try:
    wait_for('MSX v10 scene loaded',timeout=400)
    # The scripted pad cannot survive Mission 1 with a mortal player: a
    # release cartridge dies at the truck of the 872 stop, respawns behind
    # it and misses the wreck landing (the RESULT of 2cacd0d's run).
    assert 'MSX reference build: player invulnerable' in log(),'run the reference cartridge (make mslugx-reference.z64)'
    wait_for('MSX first frame ready')
    window=subprocess.check_output([XD,'search','--onlyvisible','--class','ares'],text=True).split()[-1]
    subprocess.run([XD,'windowmove',window,'0','0'],check=True)
    subprocess.run([XD,'windowfocus','--sync',window],check=True);time.sleep(.5);snap('start')
    worker.start();watcher.start()
    wait_for(r'MSX rebel spawned=1 ',timeout=200)
    wait_for(r'MSX camera stop=872',timeout=400)
    # The truck 02916E's wreck is a ramp: standing on it lifts the camera.
    try:
        wait_for(r'MSX landed height=-3[0-9]',timeout=60);time.sleep(.4);snap('truck-wreck')
    except AssertionError:
        print('no landing on the truck wreck')
    # Just past the first camera rise (944..1008): the bottom row of the
    # foreground at x >= 1216 must be drawn (merge_observed_cells.py).
    wait_for(r'MSX camera progress=1024 ',timeout=600);snap('camera-risen')
    wait_for(r'MSX camera stop=1920',timeout=900);time.sleep(1.0);snap('village-end')
    wait_for(r'MSX domes signal 05486A',timeout=900);domes.set();time.sleep(4);snap('domes-risen')
    wait_for(r'MSX player handlers=054B18',timeout=1800);time.sleep(.6);snap('collapse')  # the tapped pistol needs 11-13 min for the three domes
    wait_for(r'MSX domed building collapsed',timeout=300);domes.clear();camel_zone.set();time.sleep(.3);snap('collapsed')
    time.sleep(1.5);snap('rubble')
    wait_for(r'MSX region end passed',timeout=900);snap('domed-building')
    wait_for(r'MSX camera autoscroll=1',timeout=900);time.sleep(.8);snap('autoscroll')
    wait_for(r'MSX backdrop palette 0B56CA',timeout=900);snap('truck-stop')
    wait_for(r'MSX camera stop=2336',timeout=900);camel_zone.clear();time.sleep(.5);snap('shop')
    shop_t[0]=time.monotonic()-t_start[0];shop.set()
    wait_for(r'MSX shop destroyed',timeout=600);time.sleep(.4);snap('shop-destroyed');shop.clear()
    time.sleep(1.5);snap('wagon')
    wait_for(r'MSX camera stop=3192',timeout=900);time.sleep(.5);snap('helicopter-stop')
    wait_for(r'MSX boss spawned',timeout=900);time.sleep(.8);snap('boss-arrives')
    wait_for(r'MSX camera stop=3376',timeout=900);arena.set()
    wait_for(r'MSX boss start signal 10C606=00',timeout=900);time.sleep(1.5);snap('boss')
    wait_for(r'MSX boss phase=\d+ hp=2\d\d\d',timeout=1800);snap('boss-damaged')
    wait_for(r'MSX mission complete signal',timeout=2400);time.sleep(.5);snap('mission-complete')
    wait_for(r'MSX mission complete camera',timeout=600);time.sleep(.3);snap('mission-end')
    wait_for(r'MSX mission end letters arrived',timeout=600);time.sleep(.2);snap('mission-complete-letters')
    stop.set();worker.join(timeout=5)
    time.sleep(6)   # one more MSX PERF/AUDIO window
    text=log()
    assert 'MSX native draw missing descriptor' not in text,re.findall(r'MSX native draw missing.*',text)
    unsupported=re.findall(r'MSX world unsupported routine=(\w+)',text)
    stopped=set(re.findall(r'MSX boss stopped node routine=(\w+) label=(\w+)',text))
    assert not unsupported,unsupported
    # The turret gun's fire 08FA1A (and with it the base's read of its gun,
    # 0D6254/0D62F2) is translated: no boss node stops.
    assert not stopped,stopped
    spawned=max(int(n) for n in re.findall(r'MSX rebel spawned=(\d+)',text))
    killed=max(int(n) for n in re.findall(r'MSX defeated=(\d+)',text))
    perf=[(int(a),int(b)) for a,b in re.findall(r'MSX PERF: .*? cpu=(\d+)/(\d+)',text)]
    rdp=[tuple(map(int,m)) for m in re.findall(r'MSX RDP: draws=(\d+) .*CPU=(\d+) us .*misses=(\d+)',text)]
    audio=[int(n) for n in re.findall(r'MSX AUDIO .*underruns=(\d+)',text)]
    print('PASS: whole Mission 1 traversed: rebels spawned=%d killed=%d; boss started by 0A8C46, defeated; mission complete (018F92/018F40)'%(spawned,killed))
    mounts=len(re.findall(r'MSX player mounted vehicle',text))
    print('boss nodes stopped: none; Camel Slug mounts=%d'%mounts)
    if perf:print('MSX PERF windows=%d cpu mean max=%d us worst frame=%d us'%(len(perf),max(p[0] for p in perf),max(p[1] for p in perf)))
    if rdp:print('RDP samples=%d draws max=%d CPU max=%d us misses max=%d'%(len(rdp),max(r[0] for r in rdp),max(r[1] for r in rdp),max(r[2] for r in rdp)))
    if audio:print('audio underruns max=%d'%max(audio))
finally:
    stop.set()
    for name in ('Right','Left','Up','a','b','e','Return'):key('keyup',name)
