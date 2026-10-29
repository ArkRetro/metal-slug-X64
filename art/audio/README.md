# Active shouted sixty-four announcement

The current take is synthesized with **Qwen3-TTS-12Hz-1.7B-CustomVoice** and its
built-in male Ryan voice (a different model/voice from the rejected Piper Ryan).
The model receives an explicit top-of-the-lungs arcade-announcer instruction in
`sixty-four-direction.txt`. It generates the whole phrase as one performance.
The earlier Kokoro split-word performance has been replaced.

The generated pitch, rhythm, attack and coarticulation are retained. There is no
post-production pitch shift, time stretching or word splicing. Mastering applies
only mild EQ/compression and an offline hall, lightly ducked behind the shouted
attack so the direct voice stays forward. Peak: 15500 PCM16; hall T60: 1.20 s.

The tag starts at 8.60 s. Its full voice/reverb tail ends at 10.5442 s; the title
holds for 60 extra ticks and hands off at 11.168 s. Start remains available.
The original Metal Slug X announcement and effects are unchanged before the tag.
`sixty-four.json` records the instruction, seed, source hash and mastering.

Model source:
https://huggingface.co/Qwen/Qwen3-TTS-12Hz-1.7B-CustomVoice

Reproduce using qwen-tts 0.1.1, transformers 4.57.3, PyTorch CPU and local weights:

```
PYTHONPATH=/workspace/toolchains/qwenttspy HF_HUB_OFFLINE=1 \
  python3 tools/synthesize_title_voice.py --comparison
python3 tools/master_title_voice.py build/title-audio/sixty-four-shout-raw.wav
```

`--comparison` also produces a private calm take with the same speaker/text so
that the performance can be compared before mastering. Model weights and raw
reference files remain outside Git. Mastering uses ffmpeg, NumPy and SciPy;
all reverb is baked offline. The N64 still plays uncompressed stereo PCM16 at
32 kHz. The final versioned voice asset is mono PCM16/48 kHz.

`tools/title_announcement.py CAPTURE` rebuilds only the title audio from its
verified private source capture and the versioned voice asset. Repeating the
command cannot stack additional tags. Full `pack_title_animation.py --adapt-x64`
builds also include the tag. The new asset does not use the old full-title TTS.

## Rejected full-title experiment (historical)

> Rejected presentation experiment. Not used by the active intro.

# Title announcement

Text: “Metal Slug! X sixty-four!” (English).
This is a generic synthetic announcement, not the original actor and not a
voice clone. Generated locally with Piper TTS 1.8.0 and its public stock
`en_US-ryan-high` voice; no original speech sample was used for synthesis.
Model source: https://huggingface.co/rhasspy/piper-voices/tree/main/en/en_US/ryan/high

Synthesis used length-scale 0.85 and sentence silence 0.12. Mastering applied
85 Hz high-pass, 7.5 kHz low-pass, a mild 210 Hz EQ lift, compression and
normalization to -16 LUFS / -2 dBTP. The WAV is mono 32 kHz, converted to WAV64
for the announcer channel. Models and synthesis dependencies remain outside Git.
