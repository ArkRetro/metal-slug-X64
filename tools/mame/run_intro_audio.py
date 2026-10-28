#!/usr/bin/env python3
"""Capture original intro references and synchronized PCM, without gameplay patches."""
import argparse,hashlib,json,os,subprocess,wave
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2]
p=argparse.ArgumentParser(description=__doc__);p.add_argument('--rom-dir',type=Path,required=True);p.add_argument('--output',type=Path,required=True);p.add_argument('--end-frame',type=int,default=3600);p.add_argument('--confirm-frame',type=int,default=0);p.add_argument('--video-step',type=int,choices=(1,2),default=2)
a=p.parse_args();assert 60<=a.end_frame<=6000 and a.end_frame%2==0
out=a.output.resolve();assert not out.exists() or not any(out.iterdir()),'Preserve earlier captures';out.mkdir(parents=True,exist_ok=True)
for n in ('cfg','nvram'):(out/n).mkdir()
env=dict(os.environ,LD_LIBRARY_PATH='/workspace/emulator/root/usr/lib/x86_64-linux-gnu',MSX_CAPTURE_DIR=str(out),MSX_CAPTURE_END_FRAME=str(a.end_frame),MSX_CONFIRM_FRAME=str(a.confirm_frame),MSX_VIDEO_STEP=str(a.video_step))
with (out/'xvfb.log').open('w') as log:
 server=subprocess.Popen(['/workspace/emulator/root/usr/bin/Xvfb','-displayfd','1','-screen','0','1024x768x24','-ac','-nolisten','tcp'],stdout=subprocess.PIPE,stderr=log,text=True,env=env)
 try:
  number=server.stdout.readline().strip();assert number.isdigit();env['DISPLAY']=':'+number
  cmd=['/workspace/emulator/root/usr/games/mame','mslugx','-rompath',str(a.rom_dir.resolve()),'-bios','euro','-video','soft','-sound','none','-samplerate','48000','-wavwrite',str(out/'reference.wav'),'-window','-skip_gameinfo','-nothrottle','-debug','-debugger','none','-debuglog','-debugscript',str(ROOT/'tools/mame/start.cmd'),'-autoboot_delay','0','-autoboot_script',str(ROOT/'tools/mame/capture_intro_audio.lua'),'-cfg_directory',str(out/'cfg'),'-nvram_directory',str(out/'nvram')]
  (out/'options.json').write_text(json.dumps(dict(end_frame=a.end_frame,confirm_frame=a.confirm_frame,video_step=a.video_step),indent=2)+'\n')
  (out/'run-command.json').write_text(json.dumps(cmd,indent=2)+'\n')
  with (out/'mame.log').open('w') as mlog:result=subprocess.run(cmd,cwd=out,env=env,stdout=mlog,stderr=subprocess.STDOUT,timeout=480)
  assert result.returncode==0
  text=(out/'debug.log').read_text();assert 'Error in command' not in text and 'AUDIO_COMMAND' in text and 'YM_WRITE' in text
  assert len(list(out.glob('vram*.bin')))==a.end_frame//a.video_step
  with wave.open(str(out/'reference.wav')) as wav:
   frames=wav.getnframes();rate=wav.getframerate();assert wav.getnchannels()==2 and rate==48000 and wav.getsampwidth()==2
   pcm=wav.readframes(frames);assert any(pcm),'Silent capture'
  report=dict(status='CAPTURED',frames=a.end_frame,audio_frames=frames,audio_rate=rate,seconds=frames/rate,audio_sha256=hashlib.sha256((out/'reference.wav').read_bytes()).hexdigest(),script_sha256=hashlib.sha256((ROOT/'tools/mame/capture_intro_audio.lua').read_bytes()).hexdigest(),limits='Reference capture only. Intro boundaries and sound command meanings need verification.')
  (out/'capture.json').write_text(json.dumps(report,indent=2)+'\n');print(report)
 finally:server.terminate();server.wait(timeout=8)
