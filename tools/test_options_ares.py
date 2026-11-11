#!/usr/bin/env python3
"""The options screen (docs/OPCIONES.md, src/msx_options.c) in ares 146 on
the normal cartridge. The cartridge is hard-linked into --output so its
EEPROM is a new one there (ares keeps the save beside the ROM it opened).

Boot 1: Start through the title; the screen opens with the defaults of the
ROM's EU table (HERO 3, CONTINUE ON, DIFFICULTY LEVEL 4, PLAY TIME 60, DEMO
SOUND ON, PLAY MANUAL ON, BLOOD OFF). N64 B (the Neo Geo A: +1) twice on
HERO (5), Down 2 and B four times on DIFFICULTY (LEVEL 8, RAM 10FD8B = 7),
Down 3 and N64 A (the Neo Geo B: -1) on PLAY MANUAL (OFF), Start: the bytes are logged as the BIOS would write them, saved to the
EEPROM, HOW TO PLAY is skipped (016BD4 with 10FD8E = 0: SOLDIER SELECT at
once), and the game starts with 5 lives (the first spawn takes one: 4) and
rank 7. Boot 2: the EEPROM gives the same bytes back before the title.
Uses the X display in DISPLAY. Last line: RESULT: PASS/FAIL."""
import argparse,os,re,subprocess,time
from pathlib import Path
from PIL import ImageGrab
p=argparse.ArgumentParser(description=__doc__)
p.add_argument('rom',type=Path);p.add_argument('--output',type=Path,required=True)
p.add_argument('--ares',default='/workspace/emulator/build/rundir/bin/ares')
a=p.parse_args()
out=a.output.resolve();out.mkdir(parents=True,exist_ok=True)
env=dict(os.environ,XDG_DATA_HOME='/workspace/emulator/data146',XDG_CONFIG_HOME='/workspace/emulator/config146',
         XDG_RUNTIME_DIR='/workspace/emulator/runtime',LD_LIBRARY_PATH='/workspace/emulator/root/usr/lib/x86_64-linux-gnu',
         VK_DRIVER_FILES='/workspace/emulator/root/usr/share/vulkan/icd.d/lvp_icd.json',LIBGL_ALWAYS_SOFTWARE='1')
assert env.get('DISPLAY'),'DISPLAY'
XD='/usr/bin/xdotool'
rom=out/'options.z64'
for f in out.iterdir():
    if f.name.startswith('options.'):f.unlink()   # a new EEPROM each run
os.link(a.rom.resolve(),rom)
def key(action,name):subprocess.run([XD,action,name],check=True)
def tap(name):key('keydown',name);time.sleep(.15);key('keyup',name);time.sleep(.35)
DEFAULTS='10FD88=03 10FD8A=01 10FD8B=03 10FD8C=00 10FD8D=01 10FD8E=01 10FD8F=00'
CHOSEN='10FD88=05 10FD8A=01 10FD8B=07 10FD8C=00 10FD8D=01 10FD8E=00 10FD8F=00'
checks={}
def boot(n):
    logp=out/('ares-%d.log'%n)
    proc=subprocess.Popen([a.ares,'--setting','Video/Driver=OpenGL 3.2','--setting','Audio/Driver=None','--setting','General/HomebrewMode=true',
        '--system','Nintendo 64','--no-file-prompt',str(rom)],stdout=open(logp,'w'),stderr=subprocess.STDOUT,env=env)
    def log():return logp.read_text(errors='replace')
    def wait(pattern,seconds=120,start=0):
        deadline=time.monotonic()+seconds
        while time.monotonic()<deadline:
            text=log()
            assert 'ASSERTION FAILED' not in text and 'RSP CRASH' not in text and 'MSX ERROR' not in text,text[-3000:]
            m=re.search(pattern,text[start:])
            if m:return m
            assert proc.poll() is None,'ares exited'
            time.sleep(.03)
        raise AssertionError('Timeout: '+pattern+'\n'+log()[-2000:])
    return proc,log,wait
def focus():
    w=subprocess.check_output([XD,'search','--sync','--onlyvisible','--class','ares'],text=True).split()[-1]
    subprocess.run([XD,'windowmove',w,'0','0'],check=True);subprocess.run([XD,'windowfocus','--sync',w],check=True)
result='FAIL';proc=None
try:
    proc,log,wait=boot(1)
    m=wait(r'MSX OPTIONS start (.*)',120)
    checks['defaults of the EU table']=m[1].strip()==DEFAULTS
    checks['nothing saved yet']='MSX OPTIONS EEPROM empty' in log()
    wait(r'MSX TITLE start',300);time.sleep(1);focus()
    while 'MSX OPTIONS open' not in log():
        if 'MSX TITLE start' in log():tap('Return')
        time.sleep(1)
    time.sleep(.5);ImageGrab.grab(xdisplay=env['DISPLAY']).save(out/'options.png')
    tap('b');tap('b')                              # HERO 3 -> 5 (N64 B: the Neo Geo A adds one)
    tap('Down');tap('Down');[tap('b') for _ in range(4)]   # DIFFICULTY LEVEL 4 -> 8
    tap('Down');tap('Down');tap('Down');tap('a')   # PLAY MANUAL ON -> OFF (N64 A: the Neo Geo B takes one)
    time.sleep(.3);ImageGrab.grab(xdisplay=env['DISPLAY']).save(out/'options-chosen.png')
    mark=len(log());tap('Return')
    m=wait(r'MSX OPTIONS close (.*)',10,mark)
    checks['bytes written as the BIOS would']=m[1].strip()==CHOSEN
    wait(r'MSX OPTIONS saved',10,mark)
    s=wait(r'MSX CINE (\w+) start',120,mark)
    checks['PLAY MANUAL off: SOLDIER SELECT without HOW TO PLAY']=s[1]=='select'
    wait(r'MSX CINE select waiting',120,mark);time.sleep(.5);tap('Return')   # Marco
    g=wait(r'MSX GAME dips lives=(\d+) continue=(\d+) rank=(\d+) timer=(\d+) blood=(\d+)',180,mark)
    checks['game: 5 lives, one taken by the first spawn']=g[1]=='4'
    checks['game: rank 10FD8B = 7']=g[3]=='7'
    checks['game: timer 60, blood off, continue on']=(g[4],g[5],g[2])==('60','0','1')
    # ares writes the save memory every 30 s (desktop-ui program.cpp, AutoSaveMemory), not on SIGTERM.
    eep=out/'options.eeprom';deadline=time.monotonic()+75
    while time.monotonic()<deadline and not (eep.exists() and eep.read_bytes()[:4]==b'MSXO'):time.sleep(1)
    checks['EEPROM file beside the ROM']=eep.exists() and eep.read_bytes()[:4]==b'MSXO'
    proc.terminate();proc.wait(timeout=10);time.sleep(1)
    proc,log,wait=boot(2)
    m=wait(r'MSX OPTIONS start (.*)',120)
    checks['boot 2: the EEPROM gives the choice back']=m[1].strip()==CHOSEN and 'MSX OPTIONS loaded from EEPROM' in log()
    for k,v in checks.items():print(('ok   ' if v else 'FAIL ')+k)
    result='PASS' if checks and all(checks.values()) else 'FAIL'
finally:
    if proc:
        proc.terminate()
        try:proc.wait(timeout=10)
        except subprocess.TimeoutExpired:proc.kill()
    try:rom.unlink()
    except FileNotFoundError:pass
print('RESULT: %s (%s)'%(result,out))
