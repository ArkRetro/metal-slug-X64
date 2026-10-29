#!/usr/bin/env python3
"""Export exact packed title frames/audio at source cadence (not an emulator capture)."""
import argparse, hashlib, json, subprocess
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
p = argparse.ArgumentParser(description=__doc__)
p.add_argument('--output', type=Path, default=ROOT/'docs/media/metal-slug-x64-animated-intro.mp4')
a = p.parse_args()
meta = json.loads((ROOT/'filesystem/cinematics/title-motion.json').read_text())
assert meta['title'] == 'METAL SLUG X64'
frames = ROOT/'build/title-x64-frames'
assert all((frames/f'{i:04}.png').exists() for i in range(meta['first_frame'], meta['last_frame']+1))
hold=meta.get('title_hold_frames',0)
total_frames=meta['frames']+hold
a.output.parent.mkdir(parents=True, exist_ok=True)
subprocess.run(['ffmpeg', '-nostdin', '-hide_banner', '-loglevel', 'error', '-y',
    '-framerate', '59185606/1000000', '-start_number', str(meta['first_frame']),
    '-i', str(frames/'%04d.png'), '-i', str(ROOT/'filesystem/cinematics/title-motion.wav'),
    '-map', '0:v:0', '-map', '1:a:0', '-frames:v', str(total_frames),
    '-vf', f"lutrgb=r='floor(val/8)*8+floor(val/32)':g='floor(val/8)*8+floor(val/32)':b='floor(val/8)*8+floor(val/32)',pad=320:240:8:8:black,scale=960:720:flags=neighbor,setsar=1,tpad=stop_mode=clone:stop={hold}",
    '-c:v', 'libx264', '-preset', 'fast', '-threads', '2', '-crf', '16',
    '-pix_fmt', 'yuv420p', '-c:a', 'aac', '-b:a', '192k', '-movflags', '+faststart',
    str(a.output)], check=True)
report = dict(title=meta['title'], frames=total_frames,source_frames=meta['frames'],title_hold_frames=hold, rate='59185606/1000000',
    presentation='Offline preview of the authored title frames quantized to native RGB555 and expanded for video, with its source PCM. Not emulator screen capture or a hardware benchmark.',
    audio='Original title effects and original Metal Slug X announcement, followed by an emphatic male sixty-four with prominent hall reverb and an extended final hold.',
    video_sha256=hashlib.sha256(a.output.read_bytes()).hexdigest(),
    movie_sha256=meta['movie_sha256'], audio_sha256=meta['audio_sha256'])
(ROOT/'docs/x64-animated-intro-video.json').write_text(json.dumps(report,indent=2)+'\n')
print(a.output)
