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
