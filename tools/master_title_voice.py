#!/usr/bin/env python3
"""Master the male synthetic title tag with body, compression and room reverb."""
import argparse, hashlib, json, subprocess, wave
from pathlib import Path
import numpy as np
from scipy.signal import butter, fftconvolve, sosfilt

ROOT = Path(__file__).resolve().parents[1]
p = argparse.ArgumentParser(description=__doc__)
p.add_argument('raw', type=Path)
a = p.parse_args()
work = ROOT/'build/title-audio'; work.mkdir(parents=True,exist_ok=True)
with wave.open(str(a.raw)) as source:
    source_rate = source.getframerate()
filters = (
    'silenceremove=start_periods=1:start_threshold=-45dB:start_silence=0.008,'
    'areverse,silenceremove=start_periods=1:start_threshold=-45dB:start_silence=0.015,areverse,'
    f'asetrate={source_rate*.88},aresample=48000,atempo={1/.88},'
    'highpass=f=75,lowpass=f=7000,'
    'equalizer=f=145:t=q:w=0.8:g=4,equalizer=f=1800:t=q:w=0.9:g=3,'
    'acompressor=threshold=0.06:ratio=6:attack=2:release=70:makeup=2.2'
)
subprocess.run(['ffmpeg','-nostdin','-hide_banner','-loglevel','error','-y',
    '-i',str(a.raw),'-af',filters,'-ar','48000','-ac','1','-c:a','pcm_s16le',
    str(work/'sixty-four-male-dry.wav')],check=True)
with wave.open(str(work/'sixty-four-male-dry.wav')) as source:
    dry=np.frombuffer(source.readframes(source.getnframes()),dtype='<i2').astype(float)
rate=48000
# Deterministic diffuse room, not a repeated-word delay. T60 is 380 ms.
rng=np.random.default_rng(64)
t=np.arange(round(.40*rate))/rate
impulse=rng.standard_normal(len(t))*np.exp(-np.log(1000)*t/.38)
impulse[t<.022]=0
impulse=sosfilt(butter(2,[180,3400],btype='bandpass',fs=rate,output='sos'),impulse)
impulse/=np.sqrt(np.sum(impulse**2))
for seconds,gain in ((.023,.60),(.041,.45),(.067,.30)):
    impulse[round(seconds*rate)]+=gain
wet=fftconvolve(dry,impulse)
wet*=np.sqrt(np.mean(dry**2))/max(1,np.sqrt(np.mean(wet[:len(dry)]**2)))
result=np.pad(dry,(0,len(wet)-len(dry)))+.22*wet
result*=12500/max(abs(result))
attack=round(.005*rate);fade=round(.02*rate)
result[:attack]*=np.linspace(0,1,attack);result[-fade:]*=np.linspace(1,0,fade)
assert len(result)/rate < 1.5545,'Voice/reverb must finish before title handoff'
out=ROOT/'art/audio/sixty-four.wav'
with wave.open(str(out),'wb') as target:
    target.setnchannels(1);target.setsampwidth(2);target.setframerate(rate);target.writeframes(result.astype('<i2').tobytes())
profile=dict(voice='Piper stock en_US-hfc_male-medium; male announcer',synthetic=True,
    source_model='https://huggingface.co/rhasspy/piper-voices/tree/main/en/en_US/hfc_male/medium',
    synthesis_length_scale=.88,pitch_ratio=.88,peak_pcm16=12500,
    reverb=dict(type='diffuse room convolution',t60_seconds=.38,wet_rms_ratio=.22),
    processing='Lower register, 145 Hz body EQ, 1.8 kHz presence, 6:1 compression, diffuse room reverb',
    duration_seconds=len(result)/rate,asset_sha256=hashlib.sha256(out.read_bytes()).hexdigest())
(ROOT/'art/audio/sixty-four.json').write_text(json.dumps(profile,indent=2)+'\n')
print(json.dumps(profile,indent=2))
