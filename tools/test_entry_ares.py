#!/usr/bin/env python3
"""Verify ROM-derived mission entry, native audio and player handoff in ares."""
import argparse,hashlib,json,os,re,subprocess,time
from pathlib import Path
from PIL import ImageGrab
ROOT=Path(__file__).resolve().parents[1]
p=argparse.ArgumentParser(description=__doc__);p.add_argument('--skip-title',action='store_true',help='Press Start to leave the adapted title before its timeout');p.add_argument('--region',action='store_true',help='Also traverse the current camera-1920 partial region; not a full mission test');p.add_argument('rom',type=Path);p.add_argument('--output',type=Path,required=True);a=p.parse_args()
f=json.loads((ROOT/'filesystem/cinematics/entry.json').read_text());out=a.output.resolve();assert not out.exists() or not any(out.iterdir());out.mkdir(parents=True,exist_ok=True)
env=dict(os.environ,LD_LIBRARY_PATH='/workspace/emulator/root/usr/lib/x86_64-linux-gnu');process=None;xd='/workspace/emulator/root/usr/bin/xdotool'
def key(action,value):subprocess.run([xd,action,value],env=env,check=True)
def wait(marker,seconds=60):
 deadline=time.monotonic()+seconds
 while time.monotonic()<deadline:
  log=(out/'ares.log').read_text();assert 'ASSERTION FAILED' not in log and 'RSP CRASH' not in log and 'MSX ERROR:' not in log,log[-3000:]
  if marker in log:return log
  assert process.poll() is None,'ares exited';time.sleep(.05)
 raise AssertionError('Timeout: '+marker)
with (out/'xvfb.log').open('w') as xlog:
 server=subprocess.Popen(['/workspace/emulator/root/usr/bin/Xvfb','-displayfd','1','-screen','0','1024x768x24','-ac','-nolisten','tcp'],stdout=subprocess.PIPE,stderr=xlog,text=True,env=env)
 try:
  display=server.stdout.readline().strip();assert display.isdigit();env['DISPLAY']=':'+display
  with (out/'ares.log').open('w') as log:
   process=subprocess.Popen(['bash',str(ROOT/'tools/run_ares.sh'),str(a.rom.resolve())],stdout=log,stderr=subprocess.STDOUT,env=env)
   title=wait('MSX TITLE start');assert 'audio=1' in title
   title_asset=json.loads((ROOT/'filesystem/cinematics/title.json').read_text())
   assert 'MSX TITLE start crc='+title_asset['pixels_crc32'] in title
   if not a.skip_title:
    time.sleep(.5);ImageGrab.grab(xdisplay=env['DISPLAY']).save(out/'title.png')
   if a.skip_title:
    windows=subprocess.check_output([xd,'search','--onlyvisible','--class','ares'],env=env,text=True).split();assert windows
    subprocess.run([xd,'windowfocus','--sync',windows[-1]],env=env,check=True)
    time.sleep(1);key('keydown','Return');time.sleep(1.0);key('keyup','Return')
   title=wait('MSX TITLE complete');assert ('skipped=1' if a.skip_title else 'skipped=0') in title
   wait('MSX INTRO start');time.sleep(.25);ImageGrab.grab(xdisplay=env['DISPLAY']).save(out/'entry.png')
   text=wait('MSX mission handoff');match=re.search(r'MSX INTRO complete frames=(\d+) rendered=(\d+) crc=([0-9a-fA-F]+) reads=(\d+) audio=(\d+)',text);assert match
   cadence=re.search(r'MSX INTRO cadence unique=(\d+) skipped=(\d+)',text);assert cadence
   assert int(cadence[1])==f['frames'] and int(cadence[2])==0,'Entry dropped source frames'
   assert int(match[1])==f['frames'] and int(match[3],16)==f['frame_crc32'][-1] and int(match[5])==1
   assert f"MSX mission handoff x={f['handoff']['x']} y={f['handoff']['y']}" in text
   windows=subprocess.check_output([xd,'search','--onlyvisible','--class','ares'],env=env,text=True).split();assert windows
   subprocess.run([xd,'windowfocus','--sync',windows[-1]],env=env,check=True)
   key('keydown','Right');key('keydown','b');time.sleep(.25);key('keydown','a');time.sleep(.12);key('keyup','a')
   wait('MSX run-jump');wait('MSX landed');wait('MSX camera moving');wait('MSX run-fire')
   ImageGrab.grab(xdisplay=env['DISPLAY']).save(out/'playable-after-entry.png')
   region=None
   if a.region:
    text=wait('MSX observed region end',180)
    end=re.search(r'MSX observed region end camera=(\d+) x=(\d+)',text);assert end
    assert int(end[1])==1920 and int(end[2])==2212*65536-1
    for marker in ('MSX camera raised=','MSX middle bank full speed=','MSX defeated=3'):
     assert marker in text,marker
    assert 'MSX DEPTH mode enabled' in text,'Region test requires the requested 2.5D build'
    cache_samples=re.findall(r'MSX RDP: .* misses=(\d+) evictions=(\d+)',text)
    assert cache_samples and all(int(m)==0 for m,e in cache_samples),'Missing cache measurements or texture deadline missed'
    depth_samples=re.findall(r'MSX DEPTH: triangles=(\d+) limit=96 fog=0 B lights=(\d+)',text)
    assert depth_samples and all(int(t)<=96 and int(l)==0 for t,l in depth_samples),'Missing 2.5D measurements or exceeded budget'
    assert 'fog_uploads=0 planes=0' in text,'Expected shadows-only presentation'
    region=dict(camera=int(end[1]),x16=int(end[2]),cache_misses=0,depth_mode=True,presentation='layered_2d_shadows_only',depth_max_triangles=max(int(t) for t,l in depth_samples),cache_samples=len(cache_samples),limit='Three provisional guards, approximate camera and REGION END; not complete Mission 1.')
    ImageGrab.grab(xdisplay=env['DISPLAY']).save(out/'partial-region-end.png')
   key('keyup','Right');key('keyup','b')
   key('keydown','q');time.sleep(.12);key('keyup','q');wait('MSX diagnostics on');time.sleep(.1)
   ImageGrab.grab(xdisplay=env['DISPLAY']).save(out/'diagnostics-on.png')
   key('keydown','q');time.sleep(.12);key('keyup','q');wait('MSX diagnostics off');time.sleep(.1)
   ImageGrab.grab(xdisplay=env['DISPLAY']).save(out/'diagnostics-off.png')
   key('keydown','e');time.sleep(.12);key('keyup','e');wait('MSX reset')
  sha=lambda path:hashlib.sha256(path.read_bytes()).hexdigest()
  timing=re.search(r'MSX INTRO timing decode_max_us=(\d+) decode_mean_us=(\d+) draw_max_us=(\d+)',text);assert timing
  result=dict(title_presentation=title_asset.get('title','original_plate_n64_prompt'),title_auto_advance=not a.skip_title,title_start_skip=a.skip_title,status='PASS',emulator='ares 146',source_frames=f['frames'],unique_presented_frames=int(cadence[1]),skipped_frames=int(cadence[2]),rendered_frames=int(match[2]),final_pixels_crc32=match[3],movie_read_bytes=int(match[4]),audio_active_at_handoff=True,diagnostic_toggle=True,decode_max_us=int(timing[1]),decode_mean_us=int(timing[2]),draw_max_us=int(timing[3]),rom_sha256=sha(a.rom),movie_sha256=f['movie_sha256'],audio_sha256=f['audio_sha256'],log_sha256=sha(out/'ares.log'),limits='Internal noninteractive entry into the existing partial region. Full title/tutorial/select, full Mission 1, gameplay effects and continuous music remain pending. No release; no acoustic capture or hardware performance claim.')
  if a.region:result['partial_region']=region
  (out/'validation.json').write_text(json.dumps(result,indent=2)+'\n');print(result)
 finally:
  if process is not None:
   for button in ('Right','b','a','e','q','Return'):
    try:key('keyup',button)
    except subprocess.CalledProcessError:pass
   process.terminate()
   try:process.wait(timeout=8)
   except subprocess.TimeoutExpired:process.kill();process.wait()
  server.terminate();server.wait(timeout=8)
