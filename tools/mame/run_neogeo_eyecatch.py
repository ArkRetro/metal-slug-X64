#!/usr/bin/env python3
"""Capture the Neo Geo BIOS eyecatch (logo + jingle) with MAME, from power-on.

Runs tools/mame/capture_neogeo_eyecatch.lua: the user's euro BIOS and Metal Slug X
cartridge, with only the cartridge header's eyecatch flag cleared in emulation
(docs/LOGO_NEOGEO.md). Writes reference.wav (48 kHz stereo, -wavwrite),
frames.jsonl (emulated time of every frame), the sound commands written to the
Z80 latch (debug.log), screenshots, and VRAM/palette dumps of the flat logo.
The capture is ROM-derived and stays outside Git."""
import argparse,hashlib,json,os,shutil,subprocess,wave
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2]
p=argparse.ArgumentParser(description=__doc__)
p.add_argument('--rom-dir',type=Path,required=True);p.add_argument('--output',type=Path,required=True)
p.add_argument('--end-frame',type=int,default=900);p.add_argument('--dump-frames',default='400,450')
p.add_argument('--mame',default=shutil.which('mame') or '/usr/games/mame')
a=p.parse_args()
out=a.output.resolve();assert not out.exists() or not any(out.iterdir()),'Preserve earlier captures'
out.mkdir(parents=True,exist_ok=True)
for n in ('cfg','nvram','snap'):(out/n).mkdir()
script=ROOT/'tools/mame/capture_neogeo_eyecatch.lua'
env=dict(os.environ,MSX_CAPTURE_DIR=str(out),MSX_CAPTURE_END_FRAME=str(a.end_frame),MSX_DUMP_FRAMES=a.dump_frames)
cmd=[a.mame,'mslugx','-rompath',str(a.rom_dir.resolve()),'-bios','euro','-video','none','-sound','none','-samplerate','48000',
     '-wavwrite',str(out/'reference.wav'),'-skip_gameinfo','-nothrottle','-debug','-debugger','none','-debuglog','-debugscript',str(ROOT/'tools/mame/start.cmd'),
     '-autoboot_delay','0','-autoboot_script',str(script),'-snapshot_directory',str(out/'snap'),
     '-cfg_directory',str(out/'cfg'),'-nvram_directory',str(out/'nvram')]
(out/'run-command.json').write_text(json.dumps(cmd,indent=2)+'\n')
with (out/'mame.log').open('w') as log:r=subprocess.run(cmd,cwd=out,env=env,stdout=log,stderr=subprocess.STDOUT,timeout=900)
assert r.returncode==0 and '[LUA ERROR]' not in (out/'mame.log').read_text(),'MAME/Lua capture failed'
text=(out/'debug.log').read_text();assert 'Error in command' not in text and 'AUDIO_COMMAND' in text
head=json.loads((out/'frames.jsonl').read_text().splitlines()[0]);assert head['header_word_0x114_before']>>8==1 and head['after']>>8==0,head
with wave.open(str(out/'reference.wav')) as w:
    assert w.getnchannels()==2 and w.getframerate()==48000 and w.getsampwidth()==2;frames=w.getnframes()
sha=lambda q:hashlib.sha256(q.read_bytes()).hexdigest()
version=subprocess.run([a.mame,'-version'],capture_output=True,text=True).stdout.strip()
report=dict(status='CAPTURED',mame=version,bios='euro',end_frame=a.end_frame,audio_frames=frames,seconds=frames/48000,
            audio_commands=[l for l in text.splitlines() if l.startswith('AUDIO_COMMAND')],
            audio_sha256=sha(out/'reference.wav'),script_sha256=sha(script),
            limits='Reference capture. Header byte 0x114 cleared in emulation only, so the BIOS runs its eyecatch.')
(out/'capture.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report,indent=1)[:1500])
