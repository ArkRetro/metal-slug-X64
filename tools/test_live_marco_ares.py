#!/usr/bin/env python3
"""Exercise the live recovered player in the actual campaign, not fixture replay."""
import argparse,hashlib,json,os,re,subprocess,time
from pathlib import Path
from PIL import ImageGrab
ROOT=Path(__file__).resolve().parents[1]
def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('rom',type=Path)
    p.add_argument('--output',type=Path,required=True)
    p.add_argument('--region',action='store_true',help='Traverse the existing partial region with actual trigger edges')
    a=p.parse_args()
    out=a.output.resolve();out.mkdir(parents=True,exist_ok=False)
    env=dict(os.environ,LD_LIBRARY_PATH='/workspace/emulator/root/usr/lib/x86_64-linux-gnu')
    xd='/workspace/emulator/root/usr/bin/xdotool';process=None
    def log():
        text=(out/'ares.log').read_text()
        assert not re.search(r'ASSERTION FAILED|Assertion failed|RSP CRASH|MSX ERROR:|MSX PLAYER (STOP|DRAW missing|SOUND unsupported|FOOTPRINT missing)',text),text[-3500:]
        return text
    def wait(pattern,start=0,seconds=100):
        deadline=time.monotonic()+seconds
        while time.monotonic()<deadline:
            text=log();match=re.search(pattern,text[start:])
            if match:return match
            assert process.poll() is None,'ares exited';time.sleep(.05)
        raise AssertionError('Timeout: '+pattern+'\n'+log()[-3500:])
    def key(action,name):subprocess.run([xd,action,name],env=env,check=True)
    def tap(name,hold=.16):key('keydown',name);time.sleep(hold);key('keyup',name)
    def shot():
        start=len(log());tap('b');wait(r'MSX PLAYER SOUND token=10f7',start);return start
    def screenshot(name):ImageGrab.grab(xdisplay=env['DISPLAY']).save(out/name)
    with (out/'xvfb.log').open('w') as xlog:
        server=subprocess.Popen(['/workspace/emulator/root/usr/bin/Xvfb','-displayfd','1','-screen','0','1024x768x24','-ac','-nolisten','tcp'],stdout=subprocess.PIPE,stderr=xlog,text=True,env=env)
        try:
            display=server.stdout.readline().strip();assert display.isdigit();env['DISPLAY']=':'+display
            with (out/'ares.log').open('w') as output:
                process=subprocess.Popen(['bash',str(ROOT/'tools/run_ares.sh'),str(a.rom.resolve())],stdout=output,stderr=subprocess.STDOUT,env=env)
                wait('MSX TITLE start')
                windows=subprocess.check_output([xd,'search','--onlyvisible','--class','ares'],env=env,text=True).split();assert windows
                subprocess.run([xd,'windowfocus','--sync',windows[-1]],env=env,check=True)
                time.sleep(.8);tap('Return');time.sleep(.8);tap('Return')
                wait('MSX FRONTEND tutorial start');time.sleep(1);tap('Return');time.sleep(.6);tap('Return')
                wait('MSX FRONTEND select waiting');tap('a')
                wait('MSX first frame ready');time.sleep(.2);screenshot('stand.png')
                start=len(log());key('keydown','Down');wait(r'hitbox=19x14',start);time.sleep(.35)
                screenshot('crouch.png');shot();time.sleep(.15);screenshot('crouch-fire.png')
                start=len(log());key('keyup','Down');wait(r'hitbox=16x28',start)
                key('keydown','Up');time.sleep(.5);shot();time.sleep(.15);screenshot('aim-up-fire.png');key('keyup','Up')
                key('keydown','Left');time.sleep(.5);shot();screenshot('left-fire.png');key('keyup','Left')
                start=len(log());tap('a');wait('MSX jump',start)
                key('keydown','Down');time.sleep(.2);shot();screenshot('air-down-fire.png');key('keyup','Down')
                wait('MSX landed',start)
                key('keydown','Right');time.sleep(.45);shot();screenshot('run-fire.png');key('keyup','Right')
                time.sleep(.2);log();region=None
                if a.region:
                    start=len(log());tap('e');wait('MSX reset',start)
                    key('keydown','Right');deadline=time.monotonic()+180
                    while time.monotonic()<deadline:
                        if 'MSX observed region end' in log()[start:]:break
                        tap('b',.12);time.sleep(.12)
                    else:raise AssertionError('Native region traversal timeout\n'+log()[-3500:])
                    key('keyup','Right');time.sleep(.15);screenshot('native-partial-region-end.png')
                    text=log()[start:]
                    end=re.search(r'MSX observed region end camera=(\d+) x=(\d+)',text);assert end
                    assert int(end[1])==1920 and int(end[2])==2212*65536-1 and 'MSX defeated=3' in text
                    cpu=[int(v) for v in re.findall(r'MSX RDP: .*CPU=(\d+) us',text)]
                    assert cpu and not re.search(r'misses=[1-9]',text)
                    region=dict(camera=1920,guards_defeated=3,cache_misses=0,sampled_cpu_us=cpu,
                                source_footprints=True,limits='Existing partial map and authored enemy responses, not complete Mission1/originalAI/hardware FPS.')
            text=log();states=re.findall(r'MSX PLAYER state .*',text)
            assert states and 'hitbox=19x14' in text and 'hitbox=16x28' in text
            assert len(re.findall('MSX PLAYER SOUND token=10f7',text))>=5
            sha=lambda path:hashlib.sha256(path.read_bytes()).hexdigest()
            result=dict(status='PASS',emulator='ares146',live_player=True,fixture_replay=False,
                        native_pistol_sounds=len(re.findall('MSX PLAYER SOUND token=10f7',text)),
                        native_state_changes=len(states),crouch_hitbox=[19,14],stand_hitbox=[16,28],
                        rom_sha256=sha(a.rom),log_sha256=sha(out/'ares.log'),
                        limits='Live normal Marco/pistol controls with current measured terrain and provisional enemies. Not full character/weapon/mission coverage, acoustic readback or hardware performance.')
            if region:result['partial_region']=region
            (out/'validation.json').write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result))
        finally:
            if process is not None:
                for name in ('a','b','Return','Up','Down','Left','Right'):
                    try:key('keyup',name)
                    except subprocess.CalledProcessError:pass
                process.terminate()
                try:process.wait(timeout=8)
                except subprocess.TimeoutExpired:process.kill();process.wait()
            server.terminate();server.wait(timeout=8)
if __name__=='__main__':main()
