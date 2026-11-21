#!/usr/bin/env python3
"""The game's ending in ares 146, from the test menu's ENDING (docs/MISION6.md "El final en la consola").

ENDING starts the game's end at 01893E, as the mission end of Mission 6 leaves it: the results 0570A0, mission 0xC
(the credits' stage 0x2C, record 017C54: the script's section 22, its two figures 0A8C62 and their sub-scripts with the
pages, the pictures and the music), the last screen with the line 01959C, the ranking 057E00 and the title 0A7FFE.
No button is pressed after the menu (a button held makes the credits run four times faster, RAM 10C604).

PASS needs every step in the cartridge's log, with the numbers of game updates between them that the TAS has (the
mission actor's and the core actor's updates of the capture captures-m6/fam/T1; the cartridge logs the update of each
step, so the check does not depend on the emulator's speed):
  018AC6 (the stage loop, phase 0) -> 018B18 (the script's end)      5181 updates
  018B18 -> 018B40 (the fade to black; the last screen)                30
  018B40 -> 018BC2 (the mission actor deletes itself)                 100
  -> 016D18 (the core actor: the ranking 057E00)                        1
  057E00 -> its end (016D72)                                          107
  -> 0166AA, -> 0166C8 (the title 0A7FFE)                            1, 1
  0A7FFE -> its end (01677A)                                          287
and no explicit stop, no access outside the ending's RAM, no missing descriptor or palette, no assertion.

  python3 tools/test_ending_ares.py mslugx-explorer.z64 --out OUT [--seconds 2400]"""
import argparse,os,re,subprocess,sys,time
from pathlib import Path
p=argparse.ArgumentParser(description=__doc__,formatter_class=argparse.RawDescriptionHelpFormatter)
p.add_argument('rom');p.add_argument('--out',required=True);p.add_argument('--seconds',type=float,default=2400);p.add_argument('--period',type=float,default=20.0);p.add_argument('--period-late',type=float,default=3.0)
p.add_argument('--ares',default='/workspace/emulator/build/rundir/bin/ares');p.add_argument('--data',default='/workspace/emulator/data146')
a=p.parse_args();out=Path(a.out);out.mkdir(parents=True,exist_ok=True)
env=dict(os.environ,XDG_DATA_HOME=a.data,XDG_CONFIG_HOME='/workspace/emulator/config146',XDG_RUNTIME_DIR='/workspace/emulator/runtime',
    LD_LIBRARY_PATH='/workspace/emulator/root/usr/lib/x86_64-linux-gnu',VK_DRIVER_FILES='/workspace/emulator/root/usr/share/vulkan/icd.d/lvp_icd.json',LIBGL_ALWAYS_SOFTWARE='1')
XD='/usr/bin/xdotool';logp=out/'ares.log'
def key(action,name):subprocess.run([XD,action,name],check=True,env=env)
def tap(name,t=.1):key('keydown',name);time.sleep(t);key('keyup',name)
proc=subprocess.Popen([a.ares,'--setting','Video/Driver=OpenGL 3.2','--setting','Audio/Driver=None','--setting','General/HomebrewMode=true',
    '--system','Nintendo 64','--no-file-prompt',str(Path(a.rom).resolve())],stdout=open(logp,'w'),stderr=subprocess.STDOUT,env=env)
def log():return logp.read_text(errors='replace')
# The TAS's updates between the steps (phases 0..8 of gameplay.c credits_update).
WANT=(5181,30,100,1,107,1,1,287)
ok=False
try:
    win=None
    while not win:
        r=subprocess.run([XD,'search','--onlyvisible','--class','ares'],text=True,capture_output=True,env=env).stdout.split()
        if r:win=r[-1]
        time.sleep(.5)
    subprocess.run([XD,'windowmove',win,'0','0'],env=env);subprocess.run([XD,'windowfocus',win],env=env)
    # Start through the cinematics and the test menu's ENDING item, every press checked on the cartridge's log
    # (tools/ares_menu.py: a press counted on the host's clock is lost at a loaded machine's speed).
    sys.path.insert(0,str(Path(__file__).resolve().parent))
    import ares_menu;ares_menu.choose(key,log,proc,13,1)
    assert 'MSX MSELECT mission=13' in log(),'the menu chose another item\n'+log()[-800:]
    t0=time.monotonic();nextgrab=0;k=0
    while time.monotonic()-t0<a.seconds:
        assert proc.poll() is None,'ares exited'
        text=log()
        assert 'ASSERTION FAILED' not in text and 'RSP CRASH' not in text and 'MSX ERROR' not in text,text[-3000:]
        if 'MSX ENDING done' in text:break
        t=time.monotonic()-t0
        if t>=nextgrab:
            if k==1:subprocess.run([XD,'windowmove',win,'0','0'],env=env)
            ph=re.findall(r'MSX ENDING credits phase (\d+)',text);sc=re.findall(r'MSX ENDING scene (\d+)',text)
            subprocess.run(['import','-display',os.environ['DISPLAY'],'-window','root',str(out/f"g{k:03d}_s{sc[-1] if sc else 0}_p{ph[-1] if ph else 'x'}.png")]);k+=1
            # After the credits (the last screen, the ranking, the title: 496 updates in all) a picture every --period-late seconds.
            nextgrab=t+(a.period_late if ph and int(ph[-1])>=1 else a.period)
        time.sleep(.2)
    text=log()
    steps={int(m.group(1)):int(m.group(2)) for m in re.finditer(r'MSX ENDING credits phase (\d+) update=(\d+)',text)}   # the last line of a phase: its update
    scenes=[int(x) for x in re.findall(r'MSX ENDING scene (\d+)',text)]
    got=tuple(steps[i+1]-steps[i] for i in range(8)) if all(i in steps for i in range(9)) else None
    done=re.search(r'MSX ENDING done[^\n]*stops=(\d+) stray=(\d+)',text)
    missing=re.findall(r'MSX native draw missing[^\n]*',text[text.find('MSX ENDING results'):]) if 'MSX ENDING results' in text else []
    stops=sorted(set(re.findall(r'(?:unsupported routine|script stopped|not loaded|pack 13 not)[^\n]{0,100}',text[text.find('MSX MSELECT enter the ending'):])))
    ovl=re.findall(r'MSX OVL (\d+) loaded',text)
    print('results started:','MSX ENDING results' in text,'| scenes',scenes,'| steps',steps)
    print('updates between the steps:',got,'TAS',WANT)
    print('end:',done.group(0)[:160] if done else None)
    print('missing draws: %d lines%s'%(len(missing),(' (last: '+missing[-1][:140]+')') if missing else ''))
    print('stops:',stops[:10]);print('overlay loads:',ovl,'screenshots',k)
    ok=bool(done) and got==WANT and int(done.group(1))==0 and int(done.group(2))==0 and scenes[:6]==[1,2,3,4,5,6] and not missing and not stops
finally:
    for n in ('d','e','a','b','Down','Return'):subprocess.run([XD,'keyup',n],env=env)
    proc.terminate()
    try:proc.wait(10)
    except subprocess.TimeoutExpired:proc.kill()
print('RESULT:','PASS' if ok else 'FAIL');sys.exit(0 if ok else 1)
