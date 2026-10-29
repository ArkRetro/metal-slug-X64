# Active X64 title digits

`x64-digits-source.png` is an authored transparent numeral asset generated for
this project. It supplies only the 6 and 4, not the original title letters or
background. The original animation is reconstructed from the user's ROMs.

`tools/title_x64.py` separates and samples the digits with nearest-neighbor,
authors their falling/rebound trajectories, includes them in the original flash,
and changes their silver material to gold. The extra motion keys are authored;
they are not claimed as recovered Neo Geo game instructions. Original letters,
particles and sound effects retain their measured timeline.

Reproduce the animation from a private capture with:

```
MSX_TITLE_ONLY=1 python3 tools/mame/run_intro_audio.py \
  --rom-dir /path/to/roms --output /path/to/new-title-capture \
  --end-frame 960 --video-step 1 --raster-first 299 --raster-last 900 \
  --shot-first 300 --shot-last 900 --script tools/mame/capture_intro_raster.lua
python3 tools/pack_title_animation.py /path/to/new-title-capture \
  --bios /path/to/roms/neogeo.zip --first 300 --last 900 \
  --adapt-x64 art/title/x64-digits-source.png
make check-cinematic mission1-internal.z64 intro-preview.z64
```

Captures, reconstructed pixels and native movies remain private/ignored. The
capture checks every unmodified frame against MAME before composition and
rejects edits outside the digit/prompt/counter regions.

`metal-slug-x64.png` and `tools/pack_title_art.py` belong to the rejected static
card experiment. They are inactive. No official Nintendo font is claimed.
