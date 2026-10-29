#!/usr/bin/env python3
"""Compare captured ares ALSA PCM to the mixed title, including its voice tail."""
import argparse, hashlib, json, wave
from pathlib import Path
import numpy as np

ROOT = Path(__file__).resolve().parents[1]
p = argparse.ArgumentParser(description=__doc__)
p.add_argument('capture', type=Path)
p.add_argument('--rom', type=Path, required=True)
a = p.parse_args()
with wave.open(str(ROOT/'filesystem/cinematics/title-motion.wav')) as w:
    assert w.getframerate() == 48000
    source = np.frombuffer(w.readframes(w.getnframes()), dtype='<i2').reshape(-1, 2).astype(float).mean(axis=1)
raw = np.fromfile(a.capture/'audio.raw', dtype='<i2').reshape(-1, 2).astype(float).mean(axis=1)
x, y = raw[::4], source[::4]
n = 1 << (len(x)+len(y)-2).bit_length()
c = np.fft.irfft(np.fft.rfft(x,n)*np.fft.rfft(y[::-1],n),n)
rough = (np.argmax(c)-(len(y)-1))/12000
points = []
for t in (1,2,3,4,6,7,8):
    window = source[int(t*48000):int((t+.1)*48000):2]
    start = int((rough+t-.02)*48000)
    assert start >= 0
    segment = raw[start:start+int(.14*48000):2]
    n = 1 << (len(segment)+len(window)-2).bit_length()
    c = np.fft.irfft(np.fft.rfft(segment,n)*np.fft.rfft(window[::-1],n),n)
    offset = int(np.argmax(c))-(len(window)-1)
    if 0 <= offset <= len(segment)-len(window):
        points.append((t,(start+offset*2)/48000))
assert len(points) >= 3
rate, offset = np.polyfit(np.array(points)[:,0],np.array(points)[:,1],1)
assert .995 < rate < 1.005
samples = np.arange(0,len(source),4)
aligned = np.interp((offset+samples/48000*rate)*48000,np.arange(len(raw)),raw)
correlation = float(np.corrcoef(source[::4],aligned)[0,1])
metadata = json.loads((ROOT/'filesystem/cinematics/title-motion.json').read_text())
announcement = metadata['announcement']
tail = (samples >= announcement['start_seconds']*48000) & (samples < announcement['end_seconds']*48000)
tail_correlation = float(np.corrcoef(source[::4][tail],aligned[tail])[0,1])
assert correlation > .85 and tail_correlation > .85, (correlation,tail_correlation)
log = (a.capture/'ares.log').read_text()
assert 'MSX TITLE cadence unique=601 dropped=0 source=601' in log
assert 'MSX INTRO preview complete' in log and 'RSP CRASH' not in log
sha = lambda path: hashlib.sha256(path.read_bytes()).hexdigest()
report = dict(status='PASS_PCM_AND_SYNTHETIC_TAIL',capture='ares 146 ALSA PCM file sink',
    title_cadence='601 unique / 0 dropped',pcm_rate=48000,channels=2,
    source_pcm_correlation_after_clock_fit=correlation,synthetic_tail_correlation=tail_correlation,
    clock_rate_fit=float(rate),offset_seconds=float(offset),minimum_correlation=.85,
    announcement=announcement,pcm_sha256=sha(a.capture/'audio.raw'),rom_sha256=sha(a.rom),
    audio_sha256=metadata['audio_sha256'],limits='Emulator output comparison, not MiSTer/full-campaign stability or listening-quality approval.')
(ROOT/'docs/x64-animated-audio-validation.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps(report,indent=2))
