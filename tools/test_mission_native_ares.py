#!/usr/bin/env python3
"""Traverse the playable region with the native rebel soldiers in ares 146.

Launch ares separately (see docs/SOLDIER_WORLD.md) with the keyboard bound
to the N64 pad (arrows, a=A, b=B, e=R, Return=Start) and pass its log. The
script walks right while firing, captures the soldiers' original sprites at
fixed milestones and checks the log: soldiers spawn and die natively, no
descriptor/palette is missing, no world routine stops, the script stops only
at the end of the imported records, and the region end is reached."""
from pathlib import Path
import argparse,re,subprocess,time
parser=argparse.ArgumentParser()
parser.add_argument('--log',type=Path,required=True)
parser.add_argument('--output',type=Path,required=True)
args=parser.parse_args();args.output.mkdir(parents=True,exist_ok=True)
XD='/usr/bin/xdotool'
def key(action,name):subprocess.run([XD,action,name],check=True)
def press(name,hold=.12):key('keydown',name);time.sleep(hold);key('keyup',name)
def snap(name):subprocess.run(['import','-window','root',str(args.output/(name+'.png'))],check=True)
def log():return args.log.read_text(errors='replace')
def wait_for(pattern,timeout=120):
    deadline=time.monotonic()+timeout
    while time.monotonic()<deadline:
        text=log()
        assert 'ASSERTION FAILED' not in text and 'ERROR' not in text,text[-2000:]
        if re.search(pattern,text):return
        time.sleep(.05)
    raise AssertionError(pattern)
try:
    wait_for('MSX v10 scene loaded',timeout=400);wait_for('MSX first frame ready')
    window=subprocess.check_output([XD,'search','--onlyvisible','--class','ares'],text=True).split()[-1]
    subprocess.run([XD,'windowmove',window,'0','0'],check=True)
    subprocess.run([XD,'windowfocus','--sync',window],check=True);time.sleep(.5);snap('start')
    # Walk in until the first rebel is near, then stop and let it act.
    key('keydown','Right');wait_for(r'MSX rebel spawned=1 ');time.sleep(1.2);key('keyup','Right')
    time.sleep(.6);snap('first-rebel')
    key('keydown','b');wait_for(r'MSX defeated=1\b');time.sleep(.15);snap('first-death');key('keyup','b')
    key('keydown','Right');key('keydown','b')
    wait_for(r'MSX defeated=3\b');snap('third-death')
    wait_for(r'MSX rebel spawned=5 ');time.sleep(.6);snap('later-rebels')
    wait_for(r'MSX defeated=5\b');snap('fifth-death')
    wait_for('MSX checkpoint=640')
    wait_for('MSX script stopped at 2d6a28',timeout=240);snap('script-end')
    wait_for('MSX observed region end',timeout=240);key('keyup','Right');key('keyup','b');time.sleep(.3);snap('region-end')
    text=log()
    assert 'MSX world unsupported' not in text,'A soldier routine stopped'
    assert 'MSX native draw missing descriptor' not in text,re.findall(r'MSX native draw missing.*',text)
    spawned=max(int(n) for n in re.findall(r'MSX rebel spawned=(\d+)',text))
    killed=max(int(n) for n in re.findall(r'MSX defeated=(\d+)',text))
    rdp=[tuple(map(int,m)) for m in re.findall(r'MSX RDP: draws=(\d+) .*CPU=(\d+) us .*misses=(\d+)',text)]
    print('PASS: native rebels spawned=%d killed=%d; region end reached'%(spawned,killed))
    print('RDP samples=%d draws max=%d CPU max=%d us mean=%d us misses max=%d'%(len(rdp),max(r[0] for r in rdp),
        max(r[1] for r in rdp),sum(r[1] for r in rdp)//len(rdp),max(r[2] for r in rdp)))
finally:
    for name in ('Right','Left','a','b','e','Return'):key('keyup',name)
