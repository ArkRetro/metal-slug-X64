# Active synthetic sixty-four tail

`sixty-four.wav` says **Sixty-four!** in English. It is appended at 8.60 s of
the reconstructed title, after the spoken X, while the original echo decays.
It ends at 9.47 s, before the 10.1545 s title handoff. The original PCM is
unchanged before and after the insertion region; no earlier effects are ducked.

The voice is Piper 1.8.0's public stock `en_US-ryan-high`, with length-scale
1.0 and no sentence silence. It is a synthetic arcade-style addition, not a
clone of the original announcer. Source model:
https://huggingface.co/rhasspy/piper-voices/tree/main/en/en_US/ryan/high

Mastering trims external silence, raises pitch by a factor of 1.09 while
preserving duration with atempo=0.917, applies 95 Hz/6.8 kHz band limits,
+3 dB at 220 Hz and +2 dB at 2.2 kHz, compression, and low-level 65/115 ms echo.
The peak is normalized to 8000 PCM16, with an 8 ms attack and 15 ms tail fade.
The versioned asset is mono PCM16/48 kHz. It is mixed into the original stereo
PCM offline; the N64 uses the resulting uncompressed stereo WAV64 at 32 kHz.

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
