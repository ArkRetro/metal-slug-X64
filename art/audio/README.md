# Active epic male sixty-four announcement

The current tag uses **Kokoro ONNX**, with a blend of 65% `am_fenrir` and 35%
`am_onyx`. It replaces the rejected Piper performances. The delivery is authored
as “Sixty … FOUR!”: 440 ms for sixty, a 95 ms beat, then an accented 580 ms four.
Each word is synthesized separately and shaped with formant-preserving stretch,
a small rise/fall in pitch, presence/body EQ and compression.

The diffuse hall is deliberately prominent: T60 1.20 s, 65% wet RMS and a
1.05-second impulse. The waveform peak is 14500 PCM16. Convolution is baked into
the asset offline; N64 still plays a single uncompressed stereo PCM16 stream.

The tag begins at 8.60 s and the full hall tail ends at 10.765 s. The title now
holds its final frame for 60 additional source ticks (about 1.014 s), ending at
11.168 s. The original letters and effects are preserved; the extra hold lets
the reverb breathe before the mission handoff. Start can still skip the title.
`sixty-four.json` records the voice, cadence, reverb and waveform hash.

Synthesis model and voice-bank source:
https://github.com/thewh1teagle/kokoro-onnx/releases/tag/model-files-v1.0

Reproduce with the downloaded Kokoro model and kokoro-onnx Python environment:

```
PYTHONPATH=/workspace/toolchains/kokoropy python3 tools/synthesize_title_voice.py
python3 tools/master_title_voice.py \
  build/title-audio/sixty-epic-raw.wav build/title-audio/four-epic-raw.wav
```

Mastering uses ffmpeg with rubberband, NumPy and SciPy. Model files stay outside
Git. The voice asset is mono PCM16/48 kHz; the composite title WAV64 is stereo
PCM16/32 kHz. This is a synthetic announcer, not an original actor voice clone.

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
