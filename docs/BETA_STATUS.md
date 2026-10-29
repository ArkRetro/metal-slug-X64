Current title: animated METAL SLUG X64. The source letters, particles, reveal
flash and original audio are reconstructed; authored 6/4 digits fall into the
same reveal and change from silver to gold. Console prompt: PRESS START.
See `title-motion-validation.json` and `INTRO_AUDIO.md` for validation and scope.
The original Metal Slug X announcement is followed by a synthetic "sixty-four"
tail at 8.60 s, now using an emphatic Kokoro male performance with
prominent hall reverb and an extra final hold;
the original effects and earlier speech are preserved.
The old static-card/generic-voice proposal is rejected and inactive.
This title update does not complete the tutorial, selection or Mission 1.

Current visual direction: fixed-scale 2D scenery layers with distinct scroll
speeds and contact shadows only. Building extrusion, perspective background
projection, fog and added dynamic lighting are disabled in the active renderer.
See `RETRO25D.md` and `layered-2d-validation.json`.

# Native port status and next delivery

Current priority from the user: complete **all Marco animations** before further
level expansion. This is not complete: see [Marco animation recovery](MARCO_ANIMATIONS.md).
The observed-descriptor renderer, script primitives and paired body/weapon helper
are validated separately (2,452 whole calls, 2,574 draws and 152 effect requests
match MAME in host C and MIPS/ares);
the active campaign still uses the historical six-state visual selector.

The [native Marco input/selection/render pipeline](MARCO_CONTROLLER.md) now
matches original calls and actual RDP compositions in two captured trajectories.
The observed resource catalog has 178 descriptors. Combat verification follows
actual queue publications rather than assuming a fixed physical-frame delay.
Outer controller transitions, remaining variants and campaign integration are
still incomplete; none of the internal regression ROMs is a demo delivery.

Whole upper-controller updates now match 1,450 reference calls and 642 combat
calls; their native animation programs pass in ares. Crouch-body transitions
match another 643 blocks across two input plans; the turn/movement plan also
passes in ares with 414 actual native animation draws and 38 effect requests.
These are recorded-boundary regressions. Standing/air/common body processing,
all weapon/forms coverage and active campaign integration remain incomplete.
See [Marco controller recovery](MARCO_CONTROLLER.md).

The latest user instruction supersedes the earlier complete-campaign-beta gate:
the next requested delivery is a playable demo of the **complete first mission,
including its full intro and sound**,
validated in ares before MiSTer testing. It must include the mission's encounters,
events and boss through its actual completion. Do not present the partial scene,
a backend regression cartridge or the old REGION END as that deliverable.
The existing v9/v10 releases remain historical prototypes. Current backend test
ROMs are internal and must not be published as the requested demo.

A native ROM-streamed WAV64/RSP audio backend now passes ares tests for stereo
mixing, playback lifecycle and AI submission alongside simple RDP output. A clean
MAME reference spans the title/tutorial/selection/parachute entry with audio.
The noninteractive parachute entry is now integrated with audio in an internal
N64 target. All 230 source frames are validated and presented without omissions
in ares, followed by a measured grounded handoff and working controls in the
existing partial region. Native pixel CRCs and three actual RDP framebuffer CRCs
match. The complete title/tutorial/select presentation, gameplay sound effects
and continuous music remain pending. See `docs/MISSION_ENTRY.md` and
`docs/INTRO_AUDIO.md`. Reviewed prototype fixes from PR #2 and SDK-free host CI
are adapted in this branch; see `docs/PR2_REVIEW.md`.

The grounded attack main state now computes 20 persistent soldier updates,
40 static support queries and the transition back to idle, coupled to the
native animation and deferred child lifecycle (41 updates through cleanup).
Eight real hit candidates also validate the native narrow-phase geometry and
local impact point (one overlap); damage/filter side effects remain pending.
Host checks and the MIPS regression in ares pass. Common control boundaries
and dynamic hit-query results remain external; active campaign guards have
not yet been replaced. See `docs/SOLDIER_ATTACK.md` and
`docs/soldier-attack-state-validation.json`.

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

Reposition states AF4F6/AF572 now calculate five animation starts, six RNG calls
and 34 native movement/draw updates, including the inline pose-table helper.
A declining-plane contact bias is corrected from original 012DEE arithmetic;
the observed downward slope/flat join returns 0043 after the 01319E clip.
Host regressions, native MIPS/ares reposition tests, the coupled attack and the
existing partial gameplay traversal pass. Only selector 0 and program 23E0DA
are dynamically covered. Common/global boundaries remain external in this suite.
See `docs/SOLDIER_LOCOMOTION.md`; this remains unintegrated with campaign AI.

The partial playable region now reaches camera 1920 (previously 872). The
calibrated scene reader handles culled columns, reuse of the middle sprite
bank, and vertical camera motion. It separates C-ROM background bank 0x40000
from moving enemy sprites in slots 65..104. Across 122 original snapshots
(the 86 advance snapshots rerun on MAME 0.264): 8307712 reconstructed pixels,
differences only within the 63 recorded variant coordinates, and 4952880
uncovered RGB8 pixels agreeing with MAME. The runtime camera model switches
the middle bank to full speed at camera 1023 and raises the view by
ceil((camera-944)/4), up to 16 px; it matches the measured transforms in
67 of 122 frames. Player-following height and a later 16 px lift at camera
1920 are not recovered. The guards, REGION END and checkpoint remain
provisional; the new end is not the mission's end. See `docs/MISSION1.md` and
`docs/mission-region-validation.json`.

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
149-column grid agree with it. See `docs/collision-recovery.json`; three of
its "invariant" cells (X=1192..1208) are wreck cells first seen after the event.

`tools/audit_collision.py` retains independent per-run state timelines and an
explicit validity mask. Unknown/mutable cells are not interpreted as empty
terrain, and capture frame numbers are not gameplay triggers. A read/write-tap
trace showed that the original query switches from the foreground grid to the
middle object's grid at camera 1023; the extractors now read the active grid
(the stale foreground ring had produced 191 false cells, including ghost wreck
copies). `tools/extract_region_collision.py` exports static terrain only when
every change is explained by the recovered vehicle stamp at a unique anchor:
the 13 wreck cells keep their pre-event value and the wreck stays an event.
The runtime uses this 280-column grid; the first 149 columns are unchanged.
Unexplained conflicts are still rejected: do not select the first value or
erase an assertion to force snapshots into a static playable level.
Camera mode/height changes are measured separately in tools/capture_camera.lua.

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

## Native Marco motion/accounting increment

437 motion-controller calls (434 animation draws) now match native MIPS/ares.
The body dispatcher shares actor state between crouch and motion and preserves
same-tick branches versus next-tick landing. Stand-up/landing/idle entry
initializers now match 80 observed calls and are connected to that dispatcher.
The C1FA6 idle update, world services, melee and common processing still block
replacing the campaign controller. See `marco-stance-validation.json`. Ammunition reservation matches
134 observed pistol calls; finite-ammo/cap/release cases have static evidence
and synthetic checks, not original-game dynamic coverage. See
`MARCO_CONTROLLER.md`, `marco-motion-validation.json`, and
`marco-ammo-validation.json`. No new partial gameplay demo is released.
