#!/usr/bin/env python3
"""Smoke the freshly started v8 ROM in the retained ares/X11 environment."""
from PIL import ImageGrab
from pathlib import Path
import subprocess,time
OUT=Path('/workspace/emulator/v8-release');OUT.mkdir(exist_ok=True)
LOG=Path('/workspace/emulator/run-v8-release.log')
XD='/workspace/emulator/root/usr/bin/xdotool'
def down(key):subprocess.run([XD,'keydown',key],check=True)
def up(key):subprocess.run([XD,'keyup',key],check=True)
def snapshot(name):ImageGrab.grab().save(OUT/(name+'.png'))
def press(key):down(key);time.sleep(.09);up(key)
def wait_for(marker,timeout=30):
    deadline=time.monotonic()+timeout
    while time.monotonic()<deadline:
        text=LOG.read_text()
        assert 'ASSERTION FAILED' not in text and 'ERROR' not in text,text[-1500:]
        if marker in text:return
        time.sleep(.025)
    raise AssertionError(marker)
try:
    wait_for('MSX v8 scene loaded');snapshot('start')
    down('Right');down('a');time.sleep(.14);snapshot('running-jump');up('a');down('b')
    wait_for('MSX obstacle=0');press('a');time.sleep(.28);snapshot('platform-one')
    wait_for('MSX obstacle=1');press('a');time.sleep(.16);snapshot('platform-two')
    wait_for('MSX area clear');up('Right');up('b');time.sleep(.1);snapshot('clear')
    press('e');time.sleep(.1);down('Left');down('b');time.sleep(.2);snapshot('mirrored-run-fire');up('Left');up('b')
    press('Return');time.sleep(.15);snapshot('debug-lab');press('Return');time.sleep(.15);snapshot('back-to-rdp')
    wait_for('misses=0')
    text=LOG.read_text()
    for marker in ('MSX run-jump','MSX run-fire','MSX landed height=-20','MSX landed height=-24','MSX checkpoint=640','MSX defeated=3'):
        assert marker in text,marker
    print('PASS: RDP/PI boot, moving animations, platforms, three guards, checkpoint, exit, mirror and lab; screenshots saved')
    print('\n'.join(line for line in text.splitlines() if 'MSX pack:' in line or 'MSX RDP:' in line)[-1200:])
finally:
    for key in ('Right','Left','a','b','e','Return'):up(key)
