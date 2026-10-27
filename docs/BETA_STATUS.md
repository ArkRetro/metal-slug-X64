# Native port status and next delivery

The latest user instruction supersedes the earlier complete-campaign-beta gate:
the next requested delivery is a playable demo of the **complete first mission**,
validated in ares before MiSTer testing. It must include the mission's encounters,
events and boss through its actual completion. Do not present the partial scene,
a backend regression cartridge or the old REGION END as that deliverable.
The existing v9/v10 releases remain historical prototypes. Current backend test
ROMs are internal and must not be published as the requested demo.

The port is not a complete beta. Original game logic, weapons, vehicles, bosses,
audio, HUD and the six-mission campaign are not complete. Three provisional
guards and REGION END are not substitutes for that coverage. Neither a graphics
test nor a Neo Geo emulator boot is evidence that the native port is finished.

The first mission-1 soldier is now correlated with RAM slot 0x101F14 in a
specific clean euro-BIOS capture. Actual debugger watchpoints, breakpoints and
an instruction trace establish the 0x0AF17E -> 0x0AF65A -> 0x0AF6C0 chain.
Ghidra was run on the captured Neo Geo map, with the physical data-ROM bank in
a separate overlay. The report documents its MC68020-superset decoder limit;
executed instructions are cross-checked against MAME's M68000 trace. A small
initializer arithmetic fragment matches captured inputs/outputs. Follow-up
work ports the original 55-word RNG and its seeding, tested against four seed
calls and 276 consecutive complete states, plus two soldier initializations,
26 decision prefixes and 17 damping updates. These fixtures pass both host C
and an internal native MIPS cartridge in ares. A build-rule fix ensures direct
object goals also use the MIPS compiler; the earlier direct-object command
alone was insufficient evidence of MIPS compilation. Full native RNG call
scheduling, state semantics, health and pool bounds remain incomplete. The
active authored guard AI has not been replaced. See `docs/SOLDIER_RNG.md`,
`docs/FIRST_SOLDIER.md` and `docs/first-soldier-evidence.json`.

The observed animation-helper subset is now native C as well. Five converted
programs replay six starts and 54 updates with persistent cursor/timer state;
all 54 descriptor/position/attribute outputs, two end signals and the observed
orientation callback match MAME. Only the two previously measured initializer
speed writes are external. Host and native MIPS replay tests pass in ares.
Unrecovered callbacks fail explicitly, and complete movement/contact, impacts,
other animation commands and campaign soldier rendering remain unintegrated.
See `docs/SOLDIER_ANIMATION.md` and `docs/soldier-animation-validation.json`.

The observed ground-motion path now has native vector preparation, a static
single-plane terrain subset, actor/contact updates and parent-relative
presentation/cache handling. All 54 measured kernel results and actor updates
match. A persistent motion-to-animation test produces all 54 original draw
records using the computed coordinates; host and MIPS/ares pass. Scalar/sensing
inputs and level/camera state are still supplied at the unrecovered controller
boundary. This is not the complete 013968 solver or a closed-loop soldier AI.
See `docs/SOLDIER_CONTACT.md` and `docs/soldier-contact-validation.json`.

The common soldier control block is now native C and passes 55 persistent cycles,
feeding 54 native movements and animation draws without injecting flags5f from
the capture. RNG condition flags follow the final XOR word, whose sign differs
from the returned random word in 128 of 276 samples. Host and MIPS/ares pass.
Two forward query results, the inter-actor RNG schedule, main-state scalar
choices and scene inputs remain external. Only the captured one-player profile
is dynamically verified. The provisional gameplay guards remain unchanged.
See `docs/SOLDIER_CONTROL.md` and `docs/soldier-control-validation.json`.

The observed grounded main-state subset now calculates the routine transitions,
scalar62 and animation starts/speeds rather than replaying them as stimuli.
All 54 integrated state cycles, six starts, two RNG-based speed calculations
and 54 movement/draw outputs match MAME. A separate near-player capture verifies
84 weighted-action queries, including the first nonzero choice of action 1
(ROM AF918). The follow-up attack animation now emits native palette/spawn effects, with
20 original soldier draws and 41 spawned-actor draws through burst and removal.
Its trajectory, palette cycle, phase transitions and resource lifetime match
MAME in native host/MIPS tests. Scene collision answers remain external; the
full soldier state machine and active gameplay integration are not complete.
Scene region/object queries, forward probes and inter-actor RNG scheduling
remain external. See `docs/SOLDIER_STATE.md`, `docs/soldier-state-validation.json` and
`docs/SOLDIER_ATTACK.md` with its three validation reports.

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
