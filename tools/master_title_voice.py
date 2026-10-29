#!/usr/bin/env python3
"""Shape an emphatic male arcade call with a large, baked hall reverb."""
import argparse, hashlib, json, subprocess, wave
from pathlib import Path
import numpy as np
from scipy.signal import butter, fftconvolve, sosfilt

ROOT=Path(__file__).resolve().parents[1]
p=argparse.ArgumentParser(description=__doc__)
p.add_argument('sixty',type=Path)
p.add_argument('four',type=Path)
a=p.parse_args()
work=ROOT/'build/title-audio';work.mkdir(parents=True,exist_ok=True)
rate=48000

def word(path,name,duration,pitch):
    trimmed=work/f'{name}-trimmed.wav'
    trim=('silenceremove=start_periods=1:start_threshold=-43dB:start_silence=0.01,'
          'areverse,silenceremove=start_periods=1:start_threshold=-43dB:start_silence=0.02,areverse')
    subprocess.run(['ffmpeg','-nostdin','-hide_banner','-loglevel','error','-y','-i',str(path),
        '-af',trim,'-ar',str(rate),'-ac','1','-c:a','pcm_s16le',str(trimmed)],check=True)
    with wave.open(str(trimmed)) as source:seconds=source.getnframes()/source.getframerate()
    final=work/f'{name}-performed.wav'
    effects=(f'rubberband=tempo={seconds/duration}:pitch={pitch}:formant=preserved,'
        'highpass=f=70,lowpass=f=7600,equalizer=f=135:t=q:w=0.8:g=3,'
        'equalizer=f=2400:t=q:w=0.9:g=3,acompressor=threshold=0.08:ratio=4.5:attack=8:release=80:makeup=2')
    subprocess.run(['ffmpeg','-nostdin','-hide_banner','-loglevel','error','-y','-i',str(trimmed),
        '-af',effects,'-c:a','pcm_s16le',str(final)],check=True)
    with wave.open(str(final)) as source:data=np.frombuffer(source.readframes(source.getnframes()),dtype='<i2').astype(float)
    # Keep consonants and the complete performed word, allowing stretch rounding.
    data=np.pad(data,(0,max(0,round(duration*rate)-len(data))))
    fade=min(round(.008*rate),len(data)//4)
    data[:fade]*=np.linspace(0,1,fade);data[-fade:]*=np.linspace(1,0,fade)
    return data/max(1,np.max(abs(data)))

first=word(a.sixty,'sixty',.44,1.01)*.82
last=word(a.four,'four',.58,.96)
dry=np.concatenate((first,np.zeros(round(.095*rate)),last))
# Longer, clearly audible hall tail; no real-time convolution on N64.
rng=np.random.default_rng(64);t=np.arange(round(1.05*rate))/rate
impulse=rng.standard_normal(len(t))*np.exp(-np.log(1000)*t/1.20)
impulse[t<.035]=0
impulse=sosfilt(butter(2,[140,4800],btype='bandpass',fs=rate,output='sos'),impulse)
impulse/=np.sqrt(np.sum(impulse**2))
for seconds,gain in ((.043,.55),(.079,.43),(.127,.32),(.193,.20)):
    impulse[round(seconds*rate)]+=gain
wet=fftconvolve(dry,impulse)
wet*=np.sqrt(np.mean(dry**2))/max(1e-9,np.sqrt(np.mean(wet[:len(dry)]**2)))
result=np.pad(dry,(0,len(wet)-len(dry)))+.65*wet
result*=14500/max(abs(result));fade=round(.03*rate)
result[-fade:]*=np.linspace(1,0,fade)
assert 8.60+len(result)/rate < (601+60)/59.185606,'Do not truncate epic reverb at handoff'
out=ROOT/'art/audio/sixty-four.wav'
with wave.open(str(out),'wb') as target:
    target.setnchannels(1);target.setsampwidth(2);target.setframerate(rate);target.writeframes(result.astype('<i2').tobytes())
profile=dict(voice='Kokoro male blend: 65% am_fenrir / 35% am_onyx',synthetic=True,
    source_model='https://github.com/thewh1teagle/kokoro-onnx/releases/tag/model-files-v1.0',
    cadence=dict(sixty_seconds=.44,pause_seconds=.095,four_seconds=.58,emphasis='FOUR'),
    peak_pcm16=14500,title_hold_frames=60,
    reverb=dict(type='large diffuse hall convolution',t60_seconds=1.20,wet_rms_ratio=.65,tail_seconds=1.05),
    processing='Emphatic two-part male performance, accented FOUR, presence EQ/compression and prominent hall reverb',
    duration_seconds=len(result)/rate,asset_sha256=hashlib.sha256(out.read_bytes()).hexdigest())
(ROOT/'art/audio/sixty-four.json').write_text(json.dumps(profile,indent=2)+'\n');print(json.dumps(profile,indent=2))
