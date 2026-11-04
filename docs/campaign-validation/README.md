# Evidence for the internal campaign integration

| Artifact | Verified scope |
| --- | --- |
| `frontend-full.json/.log` | Title, all 3050 tutorial/Marco confirmation frames, parachute and native partial region to camera 1920; three provisional guards, HUD, camera and cache checks. |
| `frontend-skip.json/.log` | Tutorial skip, rejection of held confirm, Marco confirmation and native movement/jump/fire. Same final ROM hash as the full run. |
| `frontend-codec.json/.log` | All 3050 decoded canvas CRCs on native MIPS/ares, in a separate codec cartridge. |
| `background.json`, `background-{tiles,prerender}.log` | 128 actual RDP framebuffers and a mixed CI8/CI4 palette transition agree with the tile reference. |
| `background-rgba-reference.json` | Earlier RGBA prerender baseline, retained for bandwidth comparison. |
| `scene-pack.json`, `prerender-pack.json` | Compiled sizes, formats and independent source composition counts; one shared 226-color CI8 palette. |
| `map-phase-correction.json` | Targeted correction of 160 cells: 68 visually observed, 92 with direct SCB provenance only. |
| `animation-hardware-source.json` | MAME 0.276 hardware reference hash/lines. |
| `hud-source.json` | Original FIX art and palette comparison; authored labels display actual prototype health/kills. |
| `host-check.json` | Successful `make check` log hash; the private log remains in the cloud workspace. |

The source frontend CRC inventory is in `../frontend-source-validation.json`.
No proprietary ROMs, dumps, images, videos or PCM are included here.

This does not verify complete Mission 1, original enemy AI, other playable
characters, arcade score/lives/timer or recovered hardware event writes.
Full-map audits retain independent failures. Active audio voices do not prove
acoustic fidelity; emulator CPU/command samples do not establish hardware FPS.
The ROM is an internal integration build, not another public demo.
