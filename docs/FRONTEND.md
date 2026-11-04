# Original tutorial and Marco confirmation

The native front end follows the adapted X64 title with three ROM-derived
movies: the HOW TO PLAY tutorial, original selection reveal held on Marco,
and the original Marco confirmation/transition. Then the existing parachute
movie hands over to native gameplay. `msx_frontend_after_title()` owns this
part of the flow; title and mission entry remain caller-owned.

This is **Marco-only confirmation**, not a completed four-character selection
controller. Direction input cannot select a character whose gameplay is absent.
A fresh A/START press confirms Marco after the reveal; held input from the
previous screen cannot confirm him. A/START can skip the tutorial after its
first 30 source ticks. The selection footer states the currently available
character and the native B-fire/A-jump mapping. It replaces the arcade credit
strip during tutorial/selection, inside the VI safe area, using the original
8-pixel FIX font. Source canvas CRCs refer to the unmodified reconstruction;
the native overlay is a deliberate presentation adaptation.

The tutorial preserves the original arcade demonstration and its original
A/B/C/D labels, including actions that remain absent from the partial campaign.
It is an original instructional sequence, not evidence that those actions have
already been integrated. During a long wait for confirmation, the finite
selection audio finishes naturally; no invented music loop is used.

## Original evidence

Fresh MAME 0.276 input-only capture with euro BIOS:
`/workspace/research/frontend-every-frame-190`. Coin/start/Marco confirmation
are logged; game RAM is not patched. Video, both physical palette banks,
auto-animation state, raster writes and stereo PCM are captured for every frame.
The source slices below are presentation edits, not recovered function entry
points or gameplay event triggers.

| Slice | Original capture frames | Count |
| --- | --- | --- |
| tutorial | 780–3459 | 2680 |
| selection reveal | 3460–3619 | 160 |
| Marco confirmation | 3620–3829 | 210 |

All 3050 frames were reconstructed independently from C-ROM/S-ROM, VRAM,
palettes and scanline writes, then compared with MAME's PNGs at RGB tolerance
at most one unit (207,692,800 active pixels). PNGs serve only as independent
references; their pixels do not become the movies. Quantization to RGBA5551
and delta encoding follow the comparison. Source frame 3830 from this capture
also matches the existing parachute movie's first pixel CRC `342b3c8e`, verifying
the join between captures.

[Source checksums and per-frame CRCs](frontend-source-validation.json).
The offline renderer now caches parsed sprite-column descriptors only while
sharing the exact same immutable VRAM tuple. Scanline latch/palette rules are
unchanged, and each output still passes the independent image comparison.

## Bounded native playback cache

The tutorial is 4760874 encoded bytes. Streaming it caused 28 source frames
to be skipped in the first ares run; one contiguous allocation also failed
after the title despite sufficient aggregate free memory. The 4–5 MiB movie
path now retains separately allocated, validated encoded packets and prepares
the largest transitions as separate full canvases, dropping their redundant
encoded packets. Smaller movies keep the existing contiguous path.

The retained-data cap is 6 MiB minus a 256 KiB reserve. The observed tutorial
uses 3741174 encoded bytes and 16 prepared canvases (2179072 bytes); total
heap use after preparation is about 6.57 MB of 7.93 MB available. Allocations
are released at each segment boundary before campaign resources are loaded.
This is an 8 MiB path; it makes no 4 MiB playback claim.

`frontend-codec-regression.z64` verifies all 3050 decoded canvas CRCs as native
MIPS in ares, including transitions into and out of prepared canvases. The
separate timed flow test verifies presentation cadence, audio voice state,
confirmation edges and handoff. Neither test is an acoustic capture.

## Build and test tools

```sh
python3 tools/mame/run_intro_audio.py --rom-dir /path/to/roms \
  --output /path/to/new-capture --end-frame 3840 --confirm-frame 3620 \
  --video-step 1 --raster-first 770 --raster-last 3839 \
  --shot-first 770 --shot-last 3839 --script tools/mame/capture_intro_raster.lua
python3 tools/pack_frontend.py /path/to/new-capture --bios /path/to/roms/neogeo.zip
python3 tools/compile_frontend_meta.py
python3 tools/test_frontend_ares.py campaign-internal.z64 --output /tmp/frontend-check
```

Assets are local under `filesystem/cinematics/frontend/`; stage their six
`.msxc`/`.wav64` files under `rom:/frontend/`. The original proprietary graphics,
PCM and capture dumps are not source-controlled. The native test checks the
presentation cadence, pixel CRC boundaries, active audio state, confirmation
gate and subsequent gameplay handoff. An active audio voice is not an acoustic
recording, and ares timing is not hardware performance verification.
