# Recorded prototype footage

`metal-slug-x-n64-25d.mp4` is a 44.57-second screen recording of the existing
partial-region N64 prototype in ares 146 with depth25d enabled. Captured at
30 video frames/second in real time, without audio, on 2026-10-29.
The visible emulator speed is host performance, not a hardware benchmark.

ROM SHA-256: `5746bb70fda591048a72b65af166d202a5e5651a8c6e457281cf9e30ac508d62`.
The log confirms `MSX DEPTH mode enabled`, camera movement, and the partial
region ending at camera 1920. This is not the complete first mission and does
not demonstrate the newly recovered Marco backend integrated into campaign.
No new playable ROM is released with this video.

`metal-slug-x-n64-25d-strong.mp4` shows the increased distant-background
separation (depths 960/320; scroll ratios 1/4 and 1/2). Recorded in ares at
20 capture frames/second, 44.6 seconds, no audio; encoded again offline for
size. Collidable buildings remain in the action plane. ROM SHA-256:
`db5ca6b362b290b033ccc89e13b0621eecea81f5e3b60ab934c8da6adcc7dd54`.
A first capture attempt hit an RSP timeout; the retained recording completed
with a lower-load encoder and no RSP crash in its log. This does not establish
that the timing-dependent failure is fixed. The separate region regression
also passed. No new gameplay ROM is published.

`metal-slug-x-building-volume.mp4` demonstrates the first authored side-wall
cutout on the small building around world X=704..832 (roughly 8–12 seconds).
Front artwork and collision remain fixed; two untextured receding walls provide
perspective-dependent thickness. This is an initial single-building volume
sample, not completed building modeling. Recorded in ares at 20 capture fps,
without audio. Full host checks and a cutout/projection/budget sweep pass.

`metal-slug-x-building-textured.mp4` replaces the single-building side-wall
color pass with perspective-correct point-sampled tiles from the facade edge.
Look around 8–12 seconds; recording is 44.6 seconds, ares, no audio. The side
reuses the edge's materials and decorative features; no new authored side-wall
art is supplied. Roofs and other buildings remain pending.


Visual review rejected both building-volume recordings: silhouette mismatch
and distorted facade details. Building extrusion is now disabled in the active
renderer. These files remain historical evidence, not approved visual results.

`metal-slug-x-layered-2d.mp4` is the current requested direction: fixed 2D
scales (far 1.08, middle 1.04, action 1), scroll ratios .04/.45/1 and contact
shadows only. No perspective background projection, building extrusion, fog
or added lighting. Captured in ares at 20 video fps, 44.6 seconds, no audio.
Terrain and collidable props retain their original action-plane coordinates.

`metal-slug-x64-intro-sound.mp4` is an **edited intro preview**, not a real-time
performance capture or complete game demo. It shows the authored METAL SLUG X64
title, the generic synthetic announcement (“Metal Slug! X sixty-four!”), and
the existing native parachute movie. Audio was captured from ares through an
ALSA file sink; the still title and recorded movie segment are aligned to the
native presentation timing rather than the slower screen-capture clock.
The preview cartridge deliberately holds the final cinematic frame and does
not enter gameplay. A separate standard region test passed; ALSA capture runs
that entered gameplay hit an RSP timeout which remains unresolved.
The parachute footage still contains original arcade HUD/prompts; full N64 HUD
adaptation and the complete mission are pending.


The user rejected `metal-slug-x64-intro-sound.mp4`: a static generated card and
generic voice do not reproduce the original letter animation or sound effects.
It is historical evidence only, not the approved intro. The active work now
recovers the original animation and synchronized sound before making X64 edits.

## Animated X64 title

`metal-slug-x64-animated-intro.mp4` previews the exact 601 composed frames used
by the N64 title movie, quantized to native RGB555, with its synchronized source PCM, at 59.185606 fps.
It is an offline asset preview, not an emulator screen recording or hardware
performance measurement. The native movie is separately tested in ares; see
`../title-motion-validation.json`. No captured frames are interpolated.

Original letters, particles, metallic impacts and reveal are retained; 6/4
numerals fall into the reveal, turn gold, and the prompt reads PRESS START.
The original announcement says Metal Slug X, followed by an arcade-style
synthetic "sixty-four" at 8.60 s. The current revision uses a lower male
Kokoro performance with a deliberate beat, accented FOUR and prominent hall
reverb. A 60-frame final hold lets the reverb decay (661 video frames total). The rejected static/generic-voice video remains historical only.
