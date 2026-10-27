#!/usr/bin/env python3
"""Traverse the observed region using the actual N64 ROM in ares."""
from PIL import ImageGrab
from pathlib import Path
import argparse,subprocess,time
parser=argparse.ArgumentParser();parser.add_argument('--version',type=int,default=10)
parser.add_argument('--variant',default='release',choices=('release','classic','retro25d'))
parser.add_argument('--projectiles',action='store_true',help='Check original projectile sprites in both directions and enemy fire')
args=parser.parse_args();VERSION=args.version;variant=args.variant
OUT=Path(f'/workspace/emulator/v{VERSION}-{variant}');OUT.mkdir(exist_ok=True)
LOG=Path(f'/workspace/emulator/run-v{VERSION}-{variant}.log')
XD='/workspace/emulator/root/usr/bin/xdotool'
def key(action,key):subprocess.run([XD,action,key],check=True)
def press(button):key('keydown',button);time.sleep(.12);key('keyup',button)
def snap(name):ImageGrab.grab().save(OUT/(name+'.png'))
def wait_for(marker,timeout=75):
    deadline=time.monotonic()+timeout
    while time.monotonic()<deadline:
        text=LOG.read_text()
        assert 'ASSERTION FAILED' not in text and 'ERROR' not in text,text[-2000:]
        if marker in text:return
        time.sleep(.05)
    raise AssertionError(marker)
try:
    wait_for(f'MSX v{VERSION} scene loaded')
    if variant!='release':wait_for('MSX first frame ready')
    # First-frame shader compilation can delay the host event loop at startup.
    # Focus the emulator only after its rendering window is ready.
    windows=subprocess.check_output([XD,'search','--onlyvisible','--class','ares'],text=True).split()
    assert windows,'No visible ares window'
    subprocess.run([XD,'windowfocus','--sync',windows[-1]],check=True)
    time.sleep(.5);snap('start')
    if variant=='retro25d':wait_for('MSX DEPTH mode enabled')
    else:assert 'MSX DEPTH mode enabled' not in LOG.read_text()
    if args.projectiles:
        key('keydown','b');time.sleep(.45);snap('projectiles-right')
        key('keydown','Left');time.sleep(.15);key('keyup','Left')
        time.sleep(.15);snap('projectiles-left');key('keyup','b')
        key('keydown','Right');time.sleep(2);key('keyup','Right')
        wait_for('MSX projectile sprite owner=1 direction=-1 size=10x6')
        time.sleep(.25) # Allow the submitted shot to reach the displayed framebuffer.
        snap('projectiles-enemy');press('e');time.sleep(.2)
    key('keydown','Right');press('a');wait_for('MSX run-jump');snap('moving-jump')
    key('keydown','b');wait_for('MSX landed');snap('dunes')
    wait_for('MSX defeated=1');snap('first-guard')
    if VERSION>=10:
        wait_for('MSX first roof');key('keyup','Right');time.sleep(.2)
        press('a');time.sleep(.35);snap('roof-jump')
        wait_for('MSX landed height=-40');time.sleep(.15);snap('on-roof');key('keydown','Right')
    wait_for('MSX checkpoint=640');press('a');snap('village-jump')
    wait_for('MSX observed region end');key('keyup','Right');key('keyup','b');snap('region-end')
    press('e');time.sleep(.2);snap('reset');key('keydown','Left');press('a');snap('left-jump');key('keyup','Left')
    time.sleep(1.5);press('Return');time.sleep(.2);snap('lab');press('Return');time.sleep(.2);snap('return')
    wait_for('misses=0')
    text=LOG.read_text()
    for marker in ('MSX run-fire','MSX camera moving','MSX defeated=3','MSX reset'):
        assert marker in text,marker
    if args.projectiles:
        for owner,direction in ((0,1),(0,-1),(1,-1)):
            assert f'MSX projectile sprite owner={owner} direction={direction} size=10x6' in text
        print('PASS: original 10x6 projectile sprites, right/left mirroring and enemy shot rendering')
    if variant=='retro25d':
        import re
        assert 'MSX DEPTH light response active' in text
        samples=re.findall(r'MSX DEPTH: triangles=(\d+) limit=96 fog=1024 B lights=(\d+)',text)
        assert samples and all(int(n)<=96 and int(l)<=2 for n,l in samples)
    print('PASS: original map, slope motion, roof landing, three provisional encounters, region bounds, restart and lab')
    print('\n'.join(line for line in text.splitlines() if 'MSX pack:' in line or 'MSX RDP:' in line)[-1600:])
finally:
    for button in ('Right','Left','a','b','e','Return'):key('keyup',button)
