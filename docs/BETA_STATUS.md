# Complete beta work and delivery policy

The user requested no more demo cartridges before a complete-port beta. The
existing v9/v10 releases are historical prototypes. Further ROM builds and ares
runs are internal validation only; do not publish another numbered demo, ZIP or
cartridge link. The optional 2.5D shader request does not remove this constraint.

The port is not a complete beta. Original game logic, weapons, vehicles, bosses,
audio, HUD and the six-mission campaign are not complete. Three provisional
guards and REGION END are not substitutes for that coverage. Neither a graphics
test nor a Neo Geo emulator boot is evidence that the native port is finished.

The current recovery work reaches camera 1920 internally. The calibrated scene
reader handles culled columns, reuse of the middle sprite bank, and vertical
camera motion. It separates C-ROM background bank 0x40000 from moving enemy
sprites in slots 65..104. Across 122 original snapshots: 8307712 reconstructed
pixels, differences only within the 63 recorded variant coordinates, and
4953328 uncovered RGB8 pixels agreeing with MAME. This validates extraction;
the expanded region is not yet integrated into gameplay.

The collision audit of the 86 advance snapshots identifies 8950 invariant cells
and ten mutable cells at world X 1112..1184. The ten cells change from empty to
three ramp cells (shape 5) and seven flat cells (shape 11), between observations
3060 and 3120 during the vehicle/explosion sequence. A subsequent CPU-write
trace confirms the footprint is applied on destruction of the observed ground
vehicle, with zero health, at its actual world position. The complete template
is 13x3 cells. A delayed-combat run reproduced thirteen exact writes and all
4096 collision-grid bytes. A native C component now implements the observed
movement, hitboxes, damage/death animation and one-shot wreckage event, with
libdragon CI4 .sprite rendering in an internal reference encounter. Campaign
spawn/wave logic, vehicle attacks and specific destruction effects remain
pending. See `docs/NATIVE_BACKEND.md` and `docs/VEHICLE_RAMP.md`. All 4758 invariant cells overlapping the existing
149-column grid agree with it. See `docs/collision-recovery.json`.

`tools/audit_collision.py` retains independent per-run state timelines and an
explicit validity mask. Unknown/mutable cells are not interpreted as empty
terrain, and capture frame numbers are not gameplay triggers. The strict static
extractor continues rejecting conflicts. Do not select the first value or erase
its assertion to force those snapshots into a static playable level. The native
runtime still uses the previously validated region; this audit does not extend
the playable map. Camera mode/height changes are measured separately in
tools/capture_camera.lua.

An external N64 Neo Geo core was examined only as a reference, at
`mike-r129/N64-NEOGEO-mvs64` commit
`d11e3e1a823d9c8da760a5cbb4a36aace0eb6194`. It is outside the port checkout in
/workspace/dependencies/mvs64-reference. Its own compatibility claims cover
Metal Slug, not Metal Slug X. The internal PC evaluation rejects the 48 MiB
C-ROM under its original 32 MiB cap; after increasing that cap locally, it
stops on a sprite-cache assertion at frame 120. It has not been integrated or
presented as a completed native game. Its mixed component licenses also need
to be retained if code is ever adopted. No dependency ROM/BIOS is published.

A complete beta must support the original campaign through its ending, with
the original gameplay systems and working audio, and pass pertinent ares
regressions. Rendering/streaming must remain within the N64 budgets in real
gameplay. The present prototype does not meet that bar; no completion date or
full-speed hardware claim is made.
