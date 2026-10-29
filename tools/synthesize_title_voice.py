#!/usr/bin/env python3
"""Synthesize an instruction-directed male SHOUT, retaining its natural delivery."""
import argparse,hashlib,json,time,wave
from pathlib import Path
import numpy as np
import torch
from qwen_tts import Qwen3TTSModel

ROOT=Path(__file__).resolve().parents[1]
p=argparse.ArgumentParser(description=__doc__)
p.add_argument('--model-dir',type=Path,default=ROOT.parent/'toolchains/qwen3-tts')
p.add_argument('--seed',type=int,default=64)
p.add_argument('--comparison',action='store_true',help='Also render a private neutral reference to check the delivery change')
a=p.parse_args()
torch.set_num_threads(4);torch.set_num_interop_threads(1);torch.manual_seed(a.seed)
text='SIXTY-FOUR!'
direction=(ROOT/'art/audio/sixty-four-direction.txt').read_text().strip()
print('Loading instruction-conditioned voice model',flush=True)
model=Qwen3TTSModel.from_pretrained(str(a.model_dir),device_map='cpu',dtype=torch.bfloat16,attn_implementation='sdpa',local_files_only=True)
print('Generating directed male shout',flush=True)
texts=[text];directions=[direction]
if a.comparison:
    texts.append(text);directions.append('Speak calmly and gently, in a relaxed conversational voice.')
began=time.monotonic()
with torch.inference_mode():
    wavs,rate=model.generate_custom_voice(text=texts,language=['English']*len(texts),speaker=['Ryan']*len(texts),
        instruct=directions,non_streaming_mode=True,max_new_tokens=96,temperature=.85,top_p=.95)
out=ROOT/'build/title-audio';out.mkdir(parents=True,exist_ok=True)
for index,samples in enumerate(wavs):
    name='sixty-four-shout-raw' if index==0 else 'sixty-four-neutral-reference'
    path=out/(name+'.wav')
    assert np.isfinite(samples).all()
    with wave.open(str(path),'wb') as w:
        w.setnchannels(1);w.setsampwidth(2);w.setframerate(rate);w.writeframes((np.clip(samples,-1,1)*32767).astype('<i2').tobytes())
    print(name,rate,len(samples)/rate,flush=True)
raw=out/'sixty-four-shout-raw.wav'
manifest=dict(model='Qwen/Qwen3-TTS-12Hz-1.7B-CustomVoice',speaker='Ryan (Qwen built-in male; not the earlier Piper voice)',
    text=text,instruction=direction,seed=a.seed,natural_delivery=True,seconds_to_generate=time.monotonic()-began,
    raw_sha256=hashlib.sha256(raw.read_bytes()).hexdigest())
(out/'sixty-four-shout-source.json').write_text(json.dumps(manifest,indent=2)+'\n')
