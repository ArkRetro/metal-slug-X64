#!/usr/bin/env python3
"""Capture original RAM-code lifetimes without game-state injection."""
import argparse
import hashlib
import json
import os
import subprocess
import zipfile
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
parser = argparse.ArgumentParser(description=__doc__)
parser.add_argument('--rom-dir', type=Path, required=True)
parser.add_argument('--output', type=Path, required=True)
parser.add_argument('--end-frame', type=int, default=2460)
parser.add_argument('--bios', choices=('euro', 'japan', 'us'), default='euro')
args = parser.parse_args()
assert 1 <= args.end_frame <= 6000
out = args.output.resolve()
out.mkdir(parents=True, exist_ok=False)
for name in ('cfg', 'nvram', 'ram'):
    (out / name).mkdir()
manifest = json.loads((ROOT / 'tools/rom_manifest.json').read_text())
with zipfile.ZipFile(args.rom_dir / 'mslugx.zip') as archive:
    for name in ('250-p1.p1', '250-p2.ep1', '250-m1.m1'):
        assert hashlib.sha1(archive.read(name)).hexdigest() == manifest[name]['sha1']
script = ROOT / 'tools/mame/trace_program_contexts.lua'
(out / 'capture.lua').write_bytes(script.read_bytes())
executed_script = out / 'capture.lua'
env = dict(os.environ, MSX_CAPTURE_DIR=str(out), MSX_CAPTURE_END_FRAME=str(args.end_frame),
           LD_LIBRARY_PATH='/workspace/emulator/root/usr/lib/x86_64-linux-gnu')
with (out / 'xvfb.log').open('w') as xlog:
    server = subprocess.Popen(['/workspace/emulator/root/usr/bin/Xvfb', '-displayfd', '1', '-screen', '0', '1024x768x24', '-ac', '-nolisten', 'tcp'], stdout=subprocess.PIPE, stderr=xlog, text=True, env=env)
    try:
        number = server.stdout.readline().strip()
        assert number.isdigit()
        env['DISPLAY'] = ':' + number
        command = ['/workspace/emulator/root/usr/games/mame', 'mslugx', '-rompath', str(args.rom_dir.resolve()), '-bios', args.bios, '-video', 'soft', '-sound', 'none', '-window', '-skip_gameinfo', '-nothrottle', '-debug', '-debugger', 'none', '-debuglog', '-debugscript', str(ROOT / 'tools/mame/start.cmd'), '-autoboot_delay', '0', '-autoboot_script', str(executed_script), '-cfg_directory', str(out / 'cfg'), '-nvram_directory', str(out / 'nvram')]
        (out / 'run-command.json').write_text(json.dumps(command, indent=2) + '\n')
        with (out / 'mame.log').open('w') as log:
            subprocess.run(command, cwd=out, env=env, stdout=log, stderr=subprocess.STDOUT, check=True, timeout=300)
    finally:
        server.terminate()
        server.wait(timeout=8)
assert 'LUA ERROR' not in (out / 'mame.log').read_text()
assert 'Error in command' not in (out / 'debug.log').read_text()
assert json.loads((out / 'finished.json').read_text())['frames'] == args.end_frame
assert len(list((out / 'ram').glob('*.bin'))) == args.end_frame
report = dict(status='ORIGINAL_INPUT_ONLY_CONTEXT_CAPTURE', frames=args.end_frame,
              bios=args.bios,
              script_sha256=hashlib.sha256(executed_script.read_bytes()).hexdigest(),
              debugger_log_sha256=hashlib.sha256((out / 'debug.log').read_bytes()).hexdigest(),
              inputs_sha256=hashlib.sha256((out / 'inputs.jsonl').read_bytes()).hexdigest(),
              limits='Fresh MVS BIOS and scripted inputs; no emulated RAM/register patches. Breakpoint site and reported PC are distinct (prefetch). Observation window and VI-end snapshots do not cover every execution path or intra-frame RAM version. Entry files retain the last entry image; traces are capped at one partial VI after each first-copy breakpoint.')
(out / 'capture.json').write_text(json.dumps(report, indent=2) + '\n')
print(json.dumps(report, indent=2))
