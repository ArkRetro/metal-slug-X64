#!/usr/bin/env python3
"""Validate the internal native vehicle reference encounter with its own Xvfb."""
import argparse,os,subprocess,time
from pathlib import Path
from PIL import ImageGrab
ROOT=Path(__file__).resolve().parents[1]
p=argparse.ArgumentParser();p.add_argument('rom',type=Path);p.add_argument('--output',type=Path,default=Path('/workspace/emulator/vehicle-native'));args=p.parse_args()
args.output.mkdir(parents=True,exist_ok=True)
env=dict(os.environ,LD_LIBRARY_PATH='/workspace/emulator/root/usr/lib/x86_64-linux-gnu')
logpath=args.output/'ares.log';xd='/workspace/emulator/root/usr/bin/xdotool'
def key(action,name):subprocess.run([xd,action,name],check=True,env=env)
def press(name):key('keydown',name);time.sleep(.12);key('keyup',name)
def snap(name):ImageGrab.grab(xdisplay=env['DISPLAY']).save(args.output/(name+'.png'))
def wait(marker,timeout=45):
    until=time.monotonic()+timeout
    while time.monotonic()<until:
        text=logpath.read_text();assert 'ASSERTION FAILED' not in text and 'ERROR' not in text,text[-2000:]
        if marker in text:return
        time.sleep(.05)
    raise AssertionError(marker)
xlog=(args.output/'xvfb.log').open('w')
server=subprocess.Popen(['/workspace/emulator/root/usr/bin/Xvfb','-displayfd','1','-screen','0','1024x768x24','-ac','-nolisten','tcp'],stdout=subprocess.PIPE,stderr=xlog,env=env,text=True)
process=None
try:
    display=server.stdout.readline().strip();assert display.isdigit(),'Xvfb failed'
    env['DISPLAY']=':'+display
    with logpath.open('w') as log:
        process=subprocess.Popen(['bash',str(ROOT/'tools/run_ares.sh'),str(args.rom.resolve())],stdout=log,stderr=subprocess.STDOUT,env=env)
        wait('MSX first frame ready');wait('MSX vehicle reference encounter')
        windows=subprocess.check_output([xd,'search','--onlyvisible','--class','ares'],text=True,env=env).split()
        subprocess.run([xd,'windowfocus','--sync',windows[-1]],check=True,env=env);time.sleep(.5)
        snap('intact');key('keydown','b');wait('MSX vehicle health=140');snap('damaged')
        wait('MSX vehicle health=0');key('keyup','b');wait('MSX vehicle wreckage=13');wait('MSX vehicle wreck state')
        time.sleep(.2);snap('wreck')
        key('keydown','Left');wait('MSX player on wreck');key('keyup','Left');time.sleep(.1);snap('on-wreck')
        press('e');time.sleep(.3);snap('reset');wait('MSX reset')
        press('Return');time.sleep(.2);snap('lab');press('Return');time.sleep(.2);snap('return')
        assert 'misses=0' in logpath.read_text()
        print('PASS: native .sprite vehicle, pistol damage, destruction, 13-cell stamp, wreck, walk approach, restart and lab')
finally:
    if process is not None:
        for name in ('b','Left','e','Return'):
            subprocess.run([xd,'keyup',name],env=env,check=False,stdout=subprocess.DEVNULL,stderr=subprocess.DEVNULL)
        process.terminate()
        try:process.wait(timeout=8)
        except subprocess.TimeoutExpired:process.kill();process.wait()
    server.terminate();server.wait(timeout=8);xlog.close()
