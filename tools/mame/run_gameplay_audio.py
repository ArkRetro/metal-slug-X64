#!/usr/bin/env python3
"""Run the gameplay sound-command capture (tools/mame/capture_gameplay_audio.lua).

MAME 0.264 headless: -video none -sound none still writes the mixed YM2610
output through -wavwrite (48 kHz stereo). Captures are ROM-derived and stay
outside Git."""
import argparse,hashlib,json,os,subprocess,wave
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2]
p=argparse.ArgumentParser(description=__doc__)
p.add_argument('--rom-dir',type=Path,required=True);p.add_argument('--output',type=Path,required=True)
p.add_argument('--end-frame',type=int,default=9000);p.add_argument('--confirm-frame',type=int,default=3620)
p.add_argument('--plan',default='fight');p.add_argument('--mame',default='/usr/games/mame')
a=p.parse_args()
out=a.output.resolve();assert not out.exists() or not any(out.iterdir()),'Preserve earlier captures'
out.mkdir(parents=True,exist_ok=True)
for n in ('cfg','nvram'):(out/n).mkdir()
script=ROOT/'tools/mame/capture_gameplay_audio.lua'
env=dict(os.environ,MSX_CAPTURE_DIR=str(out),MSX_CAPTURE_END_FRAME=str(a.end_frame),MSX_CONFIRM_FRAME=str(a.confirm_frame),MSX_PLAN=a.plan)
cmd=[a.mame,'mslugx','-rompath',str(a.rom_dir.resolve()),'-bios','euro','-video','none','-sound','none','-samplerate','48000',
     '-wavwrite',str(out/'reference.wav'),'-skip_gameinfo','-nothrottle','-debug','-debugger','none','-debuglog','-debugscript',str(ROOT/'tools/mame/start.cmd'),'-autoboot_delay','0','-autoboot_script',str(script),
     '-cfg_directory',str(out/'cfg'),'-nvram_directory',str(out/'nvram')]
(out/'run-command.json').write_text(json.dumps(cmd,indent=2)+'\n')
with (out/'mame.log').open('w') as log:r=subprocess.run(cmd,cwd=out,env=env,stdout=log,stderr=subprocess.STDOUT,timeout=1800)
assert r.returncode==0 and '[LUA ERROR]' not in (out/'mame.log').read_text(),'MAME/Lua capture failed'
text=(out/'debug.log').read_text();assert 'Error in command' not in text and 'LATCH ' in text and 'REQ ' in text
with wave.open(str(out/'reference.wav')) as w:
    assert w.getnchannels()==2 and w.getframerate()==48000 and w.getsampwidth()==2;frames=w.getnframes()
sha=lambda q:hashlib.sha256(q.read_bytes()).hexdigest()
report=dict(status='CAPTURED',plan=a.plan,end_frame=a.end_frame,audio_frames=frames,seconds=frames/48000,
            latch_writes=text.count('LATCH '),requests=text.count('REQ '),
            audio_sha256=sha(out/'reference.wav'),script_sha256=sha(script))
(out/'capture.json').write_text(json.dumps(report,indent=2)+'\n');print(report)
