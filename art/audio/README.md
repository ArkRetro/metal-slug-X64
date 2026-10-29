# Active male sixty-four announcement

`sixty-four.wav` says **Sixty-four!** in English, using a new male base voice:
Piper `en_US-hfc_male-medium`. This replaces the rejected higher Ryan voice.
It enters at 8.60 s, after the spoken X, and its reverberation ends at 9.8679 s,
before the 10.1545 s handoff. Original effects and earlier speech are preserved.

The register is lowered by a factor of 0.88 with duration compensation.
145 Hz body EQ, 1.8 kHz presence and 6:1 compression provide a stronger delivery;
the peak is 12500 PCM16 (previously 8000). Reverb uses a deterministic diffuse
room impulse with a 380 ms T60 and 22% wet RMS. It is not a repeated-word delay.
The versioned profile and waveform hash are in `sixty-four.json`.

Source model:
https://huggingface.co/rhasspy/piper-voices/tree/main/en/en_US/hfc_male/medium

Reproduction with the local model and Piper installation:

```
PYTHONPATH=/workspace/toolchains/piperpy /workspace/toolchains/piperpy/bin/piper \
  --model /workspace/toolchains/piper-voices/en_US-hfc_male-medium.onnx \
  --input-file art/audio/sixty-four.txt \
  --output-file build/title-audio/sixty-four-male-raw.wav \
  --length-scale 0.88 --sentence-silence 0.0
python3 tools/master_title_voice.py build/title-audio/sixty-four-male-raw.wav
```

Mastering uses ffmpeg, NumPy and SciPy offline; no runtime reverb is added
to the N64 audio budget. The asset is mono PCM16/48 kHz, mixed offline into
the original stereo track.
N64 playback uses uncompressed stereo PCM16 WAV64 at 32 kHz. This is a stock
synthetic male announcer, not a clone of the arcade actor.

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
