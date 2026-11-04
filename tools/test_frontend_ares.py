#!/usr/bin/env python3
"""Verify native tutorial, Marco-confirm gate, confirmation and mission handoff."""
import argparse
import hashlib
import json
import os
import re
import subprocess
import time
from pathlib import Path
from PIL import ImageGrab
ROOT=Path(__file__).resolve().parents[1]


def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('rom',type=Path)
    parser.add_argument('--output',type=Path,required=True)
    parser.add_argument('--skip-tutorial',action='store_true')
    parser.add_argument('--region',action='store_true',help='Traverse the existing partial region after the full presentation; not complete Mission 1')
    args=parser.parse_args()
    assets=json.loads((ROOT/'filesystem/cinematics/frontend/frontend.json').read_text())
    fixture={s['name']:s for s in assets['segments']}
    entry=json.loads((ROOT/'filesystem/cinematics/entry.json').read_text())
    out=args.output.resolve();assert not out.exists() or not any(out.iterdir());out.mkdir(parents=True,exist_ok=True)
    env=dict(os.environ,LD_LIBRARY_PATH='/workspace/emulator/root/usr/lib/x86_64-linux-gnu')
    xdotool='/workspace/emulator/root/usr/bin/xdotool';process=None
    def key(action,value):
        subprocess.run([xdotool,action,value],env=env,check=True)
    def wait(marker,seconds=90):
        deadline=time.monotonic()+seconds
        while time.monotonic()<deadline:
            text=(out/'ares.log').read_text()
            assert all(s not in text for s in ('ASSERTION FAILED','Assertion failed','RSP CRASH','MSX ERROR:')),text[-3000:]
            if marker in text:return text
            assert process.poll() is None,'ares exited'
            time.sleep(.05)
        raise AssertionError('Timeout: '+marker)
    def screenshot(name):
        ImageGrab.grab(xdisplay=env['DISPLAY']).save(out/name)
    with (out/'xvfb.log').open('w') as xlog:
        server=subprocess.Popen(['/workspace/emulator/root/usr/bin/Xvfb','-displayfd','1','-screen','0','1024x768x24','-ac','-nolisten','tcp'],stdout=subprocess.PIPE,stderr=xlog,text=True,env=env)
        try:
            number=server.stdout.readline().strip();assert number.isdigit();env['DISPLAY']=':'+number
            with (out/'ares.log').open('w') as log:
                process=subprocess.Popen(['bash',str(ROOT/'tools/run_ares.sh'),str(args.rom.resolve())],stdout=log,stderr=subprocess.STDOUT,env=env)
                wait('MSX TITLE complete')
                wait('MSX FRONTEND tutorial start')
                windows=subprocess.check_output([xdotool,'search','--onlyvisible','--class','ares'],env=env,text=True).split();assert windows
                subprocess.run([xdotool,'windowfocus','--sync',windows[-1]],env=env,check=True)
                time.sleep(1);screenshot('how-to-play.png')
                if args.skip_tutorial:
                    key('keydown','Return');time.sleep(.25);key('keyup','Return')
                wait('MSX FRONTEND tutorial complete',180)
                # A held before the selection gate must not confirm a character.
                key('keydown','a')
                wait('MSX FRONTEND select waiting character=MARCO')
                time.sleep(.25)
                text=(out/'ares.log').read_text()
                assert 'MSX FRONTEND select confirmed' not in text,'Held input bypassed confirmation gate'
                key('keyup','a');time.sleep(.15)
                key('keydown','Right');time.sleep(.2);key('keyup','Right')
                assert 'MSX FRONTEND select confirmed' not in (out/'ares.log').read_text()
                screenshot('soldier-select-marco.png')
                key('keydown','a');time.sleep(.2);key('keyup','a')
                wait('MSX FRONTEND select confirmed character=MARCO')
                wait('MSX FRONTEND select-confirm complete')
                text=wait('MSX mission handoff')
                wait('MSX first frame ready');time.sleep(.15)
                screenshot('playable-handoff.png')
                assert f"MSX mission handoff x={entry['handoff']['x']} y={entry['handoff']['y']}" in text
                # Verify actual handoff accepts native movement/jump/fire inputs.
                key('keydown','Right');key('keydown','b');key('keydown','a');time.sleep(.6);key('keyup','a')
                text=wait('MSX run-fire');wait('MSX landed')
                region=None
                if args.region:
                    text=wait('MSX observed region end',180)
                    end=re.search(r'MSX observed region end camera=(\d+) x=(\d+)',text);assert end
                    assert int(end[1])==1920 and int(end[2])==2212*65536-1
                    assert all(marker in text for marker in ('MSX defeated=3','MSX camera raised=','MSX middle bank full speed='))
                    cache=re.findall(r'MSX RDP: .* CPU=(\d+) us PI=.* misses=(\d+) evictions=(\d+)',text)
                    assert cache and all(int(m)==0 for cpu,m,e in cache),'Texture deadline miss or no cache samples'
                    depth=re.findall(r'MSX DEPTH: triangles=(\d+) limit=96 fog=0 B lights=(\d+)',text)
                    assert depth and all(int(t)<=96 and int(l)==0 for t,l in depth)
                    region=dict(camera=1920,x16=int(end[2]),guards_defeated=3,cache_misses=0,
                                sampled_cpu_us=[int(cpu) for cpu,m,e in cache],limits='Existing three provisional guards and camera-1920 limit; not full Mission 1, original AI or hardware performance.')
                    time.sleep(.1);screenshot('partial-region-end.png')
                key('keyup','Right');key('keyup','b')
            results=[]
            for name,source in fixture.items():
                start=re.search(r'MSX FRONTEND '+re.escape(name)+r' start frames=(\d+) crc=([0-9a-f]+) audio=(\d+)',text)
                end=re.search(r'MSX FRONTEND '+re.escape(name)+r' complete rendered=(\d+) unique=(\d+) dropped=(\d+) skipped=(\d+) crc=([0-9a-f]+)',text)
                assert start and end,(name,'missing timeline log')
                assert int(start[1])==source['frames'] and int(start[2],16)==source['frame_crc32'][0] and int(start[3])==1
                skipped=bool(int(end[4]));assert skipped==(args.skip_tutorial and name=='tutorial')
                if not skipped:
                    assert int(end[5],16)==source['frame_crc32'][-1]
                    assert int(end[2])==source['frames'] and int(end[3])==0,(name,'presentation dropped source frames',end.groups())
                results.append(dict(name=name,source_frames=source['frames'],presented=int(end[2]),dropped=int(end[3]),skipped=skipped,first_crc32=start[2],last_crc32=end[5],audio_active=True))
            sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
            result=dict(status='PASS',emulator='ares 146',segments=results,held_confirm_rejected=True,marco_confirm=True,native_handoff=True,rom_sha256=sha(args.rom),log_sha256=sha(out/'ares.log'),frontend_provenance=assets['provenance'],limits='Tutorial and Marco-only selection/confirmation; no other playable characters, complete campaign, acoustic recording or hardware performance claim.')
            if region:result['partial_region']=region
            (out/'validation.json').write_text(json.dumps(result,indent=2)+'\n');print(result)
        finally:
            if process is not None:
                for button in ('Right','a','b','Return'):
                    try:key('keyup',button)
                    except subprocess.CalledProcessError:pass
                process.terminate()
                try:process.wait(timeout=8)
                except subprocess.TimeoutExpired:process.kill();process.wait()
            server.terminate();server.wait(timeout=8)


if __name__=='__main__':
    main()
