.DEFAULT_GOAL := all
N64_INST ?= /workspace/toolchains/package/opt/libdragon
# depth25d (default): subtle 2.5D parallax/lighting/shadows/fog; classic: original flat path.
MSX_VISUAL_MODE ?= classic
MSX_SCENARIO ?= campaign
# Arcade soft-DIP "BLOOD" (RAM 10FD8F): 1 = ON (default, uncensored), 0 = OFF (US/EU default).
MSX_BLOOD ?= 1
# depth25d extras for review builds: 1 perspective, 2 lights, 4 fog (0 = layered 2D + shadows).
MSX_DEPTH_STYLE ?= 0
export N64_INST
# Arena of the animation programs read on demand (src/program_file.c; measured, docs/MEMORY_OPTIMIZATION.md).
PROGRAM_ARENA_KB ?= 112
BUILD_DIR := build
N64_MKDFS_ROOT := romfs
# Ported from PR #2: host checks and clean must not require an installed SDK.
HOST_GOALS := check check-core check-cinematic preview-depth clean
ifeq ($(MAKECMDGOALS),)
NEED_SDK := 1
else ifneq ($(filter-out $(HOST_GOALS),$(MAKECMDGOALS)),)
NEED_SDK := 1
endif
ifdef NEED_SDK
include $(N64_INST)/include/n64.mk
N64_CFLAGS += -DPROGRAM_ARENA_KB=$(PROGRAM_ARENA_KB) -I$(BUILD_DIR) -I$(BUILD_DIR)/retro25d -DMSX_BLOOD=$(MSX_BLOOD) $(MSX_EXTRA_CFLAGS)
endif
WORLD_SRCS := src/neo_camera.c src/neo_train.c src/neo_m6bridge.c src/neo_boss.c src/neo_world.c src/neo_gactor.c src/neo_region.c src/neo_truck.c src/neo_effects.c src/neo_flyer.c src/neo_props.c src/neo_rest.c src/neo_dome.c src/neo_barrel.c src/neo_wagon.c src/neo_arab.c src/neo_prisoner.c src/neo_item.c src/neo_armored.c src/neo_secret.c src/neo_script.c src/neo_soldier_ai.c src/neo_kernel.c src/neo_contact.c src/neo_control.c src/neo_rng.c src/neo_animation.c src/neo_soldier.c src/neo_soldier_action.c src/neo_hit.c src/neo_cull.c src/neo_throw_actor.c src/neo_throw.c src/neo_target.c src/neo_blood.c src/marco_ammo.c src/neo_bullet.c src/neo_chaser.c src/neo_pweapon.c src/neo_hud.c src/marco_hgun.c src/marco_thinfire.c src/marco_death.c src/neo_swords.c src/neo_rocket.c src/neo_heli.c src/marco_legs.c src/neo_camel.c src/neo_porter.c src/neo_civil.c src/neo_vslot.c src/neo_vulcan.c src/neo_slug.c src/neo_tank.c src/neo_m3veh.c src/neo_slugnoid.c src/neo_tread.c src/neo_end.c src/neo_fade.c src/marco_mount.c src/marco_descent.c src/neo_tally.c src/neo_text.c src/neo_continue.c src/neo_landing.c src/neo_mission.c src/neo_wipe.c src/neo_goarrow.c src/neo_rebel2.c src/neo_rebel3.c src/neo_m2creature.c src/neo_gate.c src/neo_crate.c src/neo_m2props.c src/neo_m3props.c src/neo_m4props.c src/neo_m4veh.c src/neo_m4boss.c src/neo_m5props.c src/neo_m6props.c src/neo_m5enemy.c src/neo_m5train.c src/neo_m5mech.c src/neo_m5sewer.c src/neo_m3enemy.c src/neo_drone.c src/neo_m2traps.c src/neo_hostage.c src/neo_m2boss.c src/neo_m3mid.c src/neo_m3boss.c
WORLD_OBJS := $(patsubst src/%.c,$(BUILD_DIR)/src/%.o,$(WORLD_SRCS))
# RAM budget (docs/RENDIMIENTO.md): actor/boss/cutscene modules are cold
# code, compiled -Os (~78 KB less program image). The per-frame core (world,
# soldier AI, kernel, animation, contact/collision, camera, gameplay, actor)
# stays -O2. Integer code only; host checks keep -O0/-O2 (make check).
HOT_SRCS := src/neo_world.c src/neo_soldier_ai.c src/neo_kernel.c src/neo_animation.c src/neo_contact.c src/neo_control.c src/neo_rng.c src/neo_cull.c src/neo_hit.c src/neo_camera.c src/neo_soldier.c src/neo_soldier_action.c src/neo_script.c
# Boot-only code (the cinematics' loop, the options screen, the character
# choice), the depth25d path (off in the classic mode) and small per-frame
# bookkeeping (sound requests, frame counters, pause, fix-layer blit) are
# cold too: the options screen's ~7 KB came back from them (docs/OPCIONES.md; the scene
# cache stays at its floor of 208 slots).
COLD_BOOT_SRCS := src/main.c src/msx_character.c src/retro25d.c src/retro_effects.c src/retro_events.c src/diorama_building.c src/game_audio.c src/msx_perf.c src/neo_pause.c src/hud_renderer.c
COLD_OBJS := $(patsubst src/%.c,$(BUILD_DIR)/src/%.o,$(filter-out $(HOT_SRCS),$(WORLD_SRCS)) src/cinematic.c src/title_intro.c src/neogeo_logo.c src/msx_pcm.c src/msx_ramfs.c src/neo_progstore.c src/program_file.c src/msx_options.c $(COLD_BOOT_SRCS))
$(COLD_OBJS): CFLAGS += -Os
# make does not see a flag change: the -Os split is recorded in a stamp the
# kernel objects depend on, rewritten only when the cold list changes. Without
# it test ROM 21 (/home/user/ws-logo) linked ten cold objects still compiled
# -O2 from before the split (+4.5 KB, slower logo: the title missed a frame).
COLD_FLAGS := $(BUILD_DIR)/cold-objs.flags
COLD_FLAGS_RUN := $(shell mkdir -p $(BUILD_DIR) && echo '-Os $(sort $(COLD_OBJS))' | cmp -s - $(COLD_FLAGS) || echo '-Os $(sort $(COLD_OBJS))' > $(COLD_FLAGS))
$(WORLD_OBJS) $(BUILD_DIR)/src/cinematic.o $(BUILD_DIR)/src/title_intro.o $(BUILD_DIR)/src/neogeo_logo.o $(BUILD_DIR)/src/msx_pcm.o $(BUILD_DIR)/src/msx_ramfs.o $(BUILD_DIR)/src/msx_options.o $(patsubst src/%.c,$(BUILD_DIR)/src/%.o,$(COLD_BOOT_SRCS)): $(COLD_FLAGS)
HOST_CC ?= cc
HOST_CFLAGS ?= -std=c11 -Wall -Wextra -Werror
# libdragon normally propagates the toolchain from a .z64 goal. Direct object
# builds must also be MIPS; otherwise host cc silently produces x86 ELF files.
$(BUILD_DIR)/src/%.o: CC = $(N64_CC)
$(BUILD_DIR)/src/%.o: CFLAGS = $(N64_CFLAGS) -MMD
$(BUILD_DIR)/tools/test_neo_rng.o: CC = $(N64_CC)
$(BUILD_DIR)/tools/test_neo_rng.o: CFLAGS = $(N64_CFLAGS) -MMD
$(BUILD_DIR)/tools/test_neo_animation.o: CC = $(N64_CC)
$(BUILD_DIR)/tools/test_neo_animation.o: CFLAGS = $(N64_CFLAGS) -MMD
$(BUILD_DIR)/tools/test_neo_contact.o: CC = $(N64_CC)
$(BUILD_DIR)/tools/test_neo_contact.o: CFLAGS = $(N64_CFLAGS) -MMD
$(BUILD_DIR)/tools/test_neo_control.o: CC = $(N64_CC)
$(BUILD_DIR)/tools/test_neo_control.o: CFLAGS = $(N64_CFLAGS) -MMD

$(BUILD_DIR)/tools/test_neo_state.o: CC = $(N64_CC)
$(BUILD_DIR)/tools/test_neo_state.o: CFLAGS = $(N64_CFLAGS) -MMD

$(BUILD_DIR)/tools/test_neo_throw.o $(BUILD_DIR)/tools/test_neo_attack.o $(BUILD_DIR)/tools/test_neo_object.o $(BUILD_DIR)/tools/test_neo_locomotion.o: CC = $(N64_CC)
$(BUILD_DIR)/tools/test_neo_throw.o $(BUILD_DIR)/tools/test_neo_attack.o $(BUILD_DIR)/tools/test_neo_object.o $(BUILD_DIR)/tools/test_neo_locomotion.o: CFLAGS = $(N64_CFLAGS) -MMD

$(BUILD_DIR)/tools/test_msx_audio.o: CC = $(N64_CC)
$(BUILD_DIR)/tools/test_msx_audio.o: CFLAGS = $(N64_CFLAGS) -MMD
$(BUILD_DIR)/audio-regression.elf: $(BUILD_DIR)/tools/test_msx_audio.o $(BUILD_DIR)/src/msx_audio.o $(BUILD_DIR)/src/msx_pcm.o
$(BUILD_DIR)/audio-regression.dfs: N64_MKDFS_ROOT = $(BUILD_DIR)/audio-romfs
$(BUILD_DIR)/audio-regression.dfs: $(BUILD_DIR)/audio-romfs/title-reference.wav64
audio-regression.z64: N64_ROM_TITLE = "MSX AUDIO TEST"
audio-regression.z64: $(BUILD_DIR)/audio-regression.dfs
$(BUILD_DIR)/audio-romfs/title-reference.wav64:
	@echo "Run tools/prepare_audio_reference.py with a local MAME capture first."
	@exit 1

$(BUILD_DIR)/tools/test_cinematic_native.o: CC = $(N64_CC)
$(BUILD_DIR)/tools/test_cinematic_native.o: CFLAGS = $(N64_CFLAGS) -MMD
$(BUILD_DIR)/tools/test_cinematic_native.o: $(BUILD_DIR)/entry_meta.h
$(BUILD_DIR)/cinematic-regression.elf: $(BUILD_DIR)/tools/test_cinematic_native.o $(BUILD_DIR)/src/cinematic.o
$(BUILD_DIR)/cinematic-romfs/entry.msxc: filesystem/cinematics/entry.msxc
	mkdir -p $(BUILD_DIR)/cinematic-romfs
	cp $< $@
$(BUILD_DIR)/cinematic-regression.dfs: N64_MKDFS_ROOT = $(BUILD_DIR)/cinematic-romfs
$(BUILD_DIR)/cinematic-regression.dfs: $(BUILD_DIR)/cinematic-romfs/entry.msxc
cinematic-regression.z64: N64_ROM_TITLE = "MSX CINE TEST"
cinematic-regression.z64: $(BUILD_DIR)/cinematic-regression.dfs

# Compare normal and release rendering using separate objects (no stale flags).
$(BUILD_DIR)/tools/test_scene_release.o: CC = $(N64_CC)
$(BUILD_DIR)/tools/test_scene_release.o: CFLAGS = $(N64_CFLAGS) -MMD
$(BUILD_DIR)/release/%.o: src/%.c $(BUILD_DIR)/scene_meta.h $(BUILD_DIR)/retro25d/retro_assets.h
	@mkdir -p $(dir $@)
	$(N64_CC) $(N64_CFLAGS) -DNDEBUG -MMD -c $< -o $@
$(BUILD_DIR)/scene-debug.elf: $(BUILD_DIR)/tools/test_scene_release.o $(BUILD_DIR)/src/scene_renderer.o $(BUILD_DIR)/src/scene_assets.o $(BUILD_DIR)/src/msx_perf.o $(BUILD_DIR)/src/scene_cull.o $(BUILD_DIR)/src/scene_strips.o $(BUILD_DIR)/src/scene_prerender.o $(BUILD_DIR)/src/retro_effects.o $(BUILD_DIR)/src/diorama_building.o $(BUILD_DIR)/src/retro25d.o
$(BUILD_DIR)/scene-release.elf: $(BUILD_DIR)/tools/test_scene_release.o $(BUILD_DIR)/release/scene_renderer.o $(BUILD_DIR)/release/scene_assets.o $(BUILD_DIR)/release/msx_perf.o $(BUILD_DIR)/release/scene_cull.o $(BUILD_DIR)/release/scene_strips.o $(BUILD_DIR)/release/scene_prerender.o $(BUILD_DIR)/release/retro_effects.o $(BUILD_DIR)/release/diorama_building.o $(BUILD_DIR)/release/retro25d.o
$(BUILD_DIR)/scene-debug.dfs $(BUILD_DIR)/scene-release.dfs: N64_MKDFS_ROOT = $(BUILD_DIR)/scene-test-romfs
$(BUILD_DIR)/scene-test-romfs/scene.pack: romfs/scene.pack
	mkdir -p $(dir $@)
	cp $< $@
$(BUILD_DIR)/scene-debug.dfs $(BUILD_DIR)/scene-release.dfs: $(BUILD_DIR)/scene-test-romfs/scene.pack
scene-debug.z64 scene-release.z64: N64_ROM_TITLE = "MSX RELEASE TEST"
scene-debug.z64: $(BUILD_DIR)/scene-debug.dfs
scene-release.z64: $(BUILD_DIR)/scene-release.dfs

# Internal integration cartridge: not the requested complete Mission 1 demo.
$(BUILD_DIR)/mission1-romfs/scene.pack $(BUILD_DIR)/mission1-romfs/entry.msxc $(BUILD_DIR)/mission1-romfs/entry.wav64 $(BUILD_DIR)/mission1-romfs/title-motion.wav64 $(BUILD_DIR)/mission1-romfs/title.msxc &: tools/build_mission_fs.py art/audio/sixty-four.wav filesystem/cinematics/title-motion.json filesystem/cinematics/title-motion.msxc filesystem/cinematics/title-motion.wav64 filesystem/cinematics/entry.json filesystem/cinematics/entry.msxc filesystem/cinematics/entry.wav64 romfs/scene.pack $(BUILD_DIR)/vehicle-sprites.stamp $(wildcard filesystem/logo/neogeo-logo.mesh filesystem/logo/neogeo-jingle.wav64)
	python3 tools/build_mission_fs.py
$(BUILD_DIR)/entry_meta.h: tools/compile_entry_meta.py filesystem/cinematics/entry.json
	python3 tools/compile_entry_meta.py
$(BUILD_DIR)/src/mission_main.o: $(BUILD_DIR)/entry_meta.h
$(BUILD_DIR)/mission1-internal.dfs: N64_MKDFS_ROOT = $(BUILD_DIR)/mission1-romfs
$(BUILD_DIR)/mission1-internal.dfs: $(BUILD_DIR)/mission1-romfs/programs.bin
$(BUILD_DIR)/mission1-romfs/programs.bin: $(BUILD_DIR)/soldier_ai_tables.h
	@mkdir -p $(dir $@)
	cp $(BUILD_DIR)/programs.bin $@
$(BUILD_DIR)/mission1-internal.dfs: $(BUILD_DIR)/mission1-romfs/title.msxc $(BUILD_DIR)/mission1-romfs/title-motion.wav64 $(BUILD_DIR)/mission1-romfs/scene.pack $(BUILD_DIR)/mission1-romfs/entry.msxc $(BUILD_DIR)/mission1-romfs/entry.wav64
mission1-internal.z64: N64_ROM_TITLE = "MSX M1 INTERNAL"
mission1-internal.z64: $(BUILD_DIR)/mission1-internal.dfs
filesystem/cinematics/entry.json filesystem/cinematics/entry.msxc filesystem/cinematics/entry.wav64:
	@echo "Run tools/pack_cinematic.py with the local golden entry capture; see docs/INTRO_AUDIO.md"
	@exit 1

.PHONY: all clean check check-core check-cinematic preview-depth FORCE
FORCE:
all: mslugx-explorer.z64 cart-size
# Cart size cap (docs/BUG_MATRIX.md B67): the MiSTer N64 core reads only the
# first 64 MiB of the cart (ROM 49, 67 928 064 bytes: rom:/audio/sfx_1029.wav64
# not found at boot) and ares writes no log above 67 043 328 bytes; the
# release cart stays at or under the smaller of the two.
CART_MAX := 67043328
.PHONY: cart-size
cart-size: mslugx-explorer.z64
	@s=$$(stat -c %s $<); if [ $$s -gt $(CART_MAX) ]; then echo "$<: $$s bytes, over $(CART_MAX) (MiSTer 64 MiB / ares log limit)"; exit 1; fi
# X64 title and parachute entry (docs/MISSION_ENTRY.md, docs/INTRO_AUDIO.md):
# private captures; with all six files the game boots logo -> title -> entry
# -> Mission 1 (src/main.c), without them it boots logo -> Mission 1.
BOOT_CINEMATICS := $(wildcard filesystem/cinematics/title-motion.json filesystem/cinematics/title-motion.msxc filesystem/cinematics/title-motion.wav64 filesystem/cinematics/entry.json filesystem/cinematics/entry.msxc filesystem/cinematics/entry.wav64)
ifeq ($(words $(BOOT_CINEMATICS)),6)
BOOT_CINE_OBJS := $(BUILD_DIR)/src/title_intro.o $(BUILD_DIR)/src/cinematic.o
$(BUILD_DIR)/src/main.o: $(BUILD_DIR)/entry_meta.h $(BOOT_CINEMATICS)
$(BUILD_DIR)/src/main.o $(BUILD_DIR)/diag/main.o: N64_CFLAGS += -DMSX_BOOT_CINEMATICS=1
else
BOOT_CINE_OBJS :=
endif
MSLUGX_GAME_OBJS = $(BUILD_DIR)/src/main.o $(BUILD_DIR)/src/neogeo_logo.o $(BUILD_DIR)/src/marco_visual.o $(BUILD_DIR)/src/marco_animation.o $(BUILD_DIR)/src/marco_renderer.o $(BUILD_DIR)/src/actor.o $(BUILD_DIR)/src/gameplay.o $(BUILD_DIR)/src/neo_collision.o $(BUILD_DIR)/src/scene_assets.o $(BUILD_DIR)/src/msx_perf.o $(BUILD_DIR)/src/msx_diag.o $(BUILD_DIR)/src/msx_diag_core.o $(BUILD_DIR)/src/scene_cull.o $(BUILD_DIR)/src/scene_strips.o $(BUILD_DIR)/src/scene_prerender.o $(BUILD_DIR)/src/scene_renderer.o $(BUILD_DIR)/src/diorama_building.o $(BUILD_DIR)/src/retro25d.o $(BUILD_DIR)/src/retro_effects.o $(BUILD_DIR)/src/retro_events.o $(BUILD_DIR)/src/neo_vehicle.o $(BUILD_DIR)/src/vehicle_renderer.o $(BUILD_DIR)/src/boss_renderer.o $(BUILD_DIR)/src/hud_renderer.o $(BUILD_DIR)/src/neo_pause.o $(BUILD_DIR)/src/msx_character.o $(BUILD_DIR)/src/msx_audio.o $(BUILD_DIR)/src/msx_pcm.o $(BUILD_DIR)/src/msx_sfx.o $(BUILD_DIR)/src/game_audio.o $(BUILD_DIR)/src/msx_ramfs.o $(BUILD_DIR)/src/neo_progstore.o $(BUILD_DIR)/src/program_file.o $(BUILD_DIR)/src/msx_options.o $(BOOT_CINE_OBJS) $(WORLD_OBJS)
# The game objects (Marco, world, renderers) with mission_main.c in place of main.c.
$(BUILD_DIR)/mission1-internal.elf: $(BUILD_DIR)/src/title_intro.o $(BUILD_DIR)/src/mission_main.o $(BUILD_DIR)/src/cinematic.o $(filter-out $(BUILD_DIR)/src/main.o $(BUILD_DIR)/src/title_intro.o $(BUILD_DIR)/src/cinematic.o,$(MSLUGX_GAME_OBJS))
$(BUILD_DIR)/mslugx-explorer.elf: $(MSLUGX_GAME_OBJS)
# Diagnostic mode (src/msx_diag.h, docs/PORT_STATUS.md "Instrumentación"):
# the game objects again with MSX_DIAG=1 for the reference cartridges and
# mslugx-diag.z64; the release (mslugx-explorer.z64) keeps MSX_DIAG=0, where
# every hook is an empty inline. Each diag object waits for its release twin
# so the generated headers it includes exist.
DIAG_GAME_OBJS = $(patsubst $(BUILD_DIR)/src/%,$(BUILD_DIR)/diag/%,$(MSLUGX_GAME_OBJS))
DIAG_LDFLAGS := --wrap=debug_assert_func_f
$(patsubst $(BUILD_DIR)/src/%,$(BUILD_DIR)/diag/%,$(COLD_OBJS)) $(BUILD_DIR)/diag/msx_diag.o $(BUILD_DIR)/diag/msx_diag_core.o: DIAG_OPT := -Os
# MSX_REFERENCE_DIAG=0 links the reference cartridges from the release
# objects (MSX_DIAG=0): the overhead comparison of docs/RENDIMIENTO.md.
MSX_REFERENCE_DIAG ?= 1
ifeq ($(MSX_REFERENCE_DIAG),1)
REF_OBJS = $(DIAG_GAME_OBJS)
REF_DIR := diag
REF_DIAG := -DMSX_DIAG=1
REF_SUFFIX :=
else
REF_OBJS = $(MSLUGX_GAME_OBJS)
REF_DIR := src
REF_DIAG :=
REF_SUFFIX := -nodiag
endif
$(BUILD_DIR)/diag/%.o: src/%.c $(BUILD_DIR)/src/%.o
	@mkdir -p $(dir $@)
	$(N64_CC) -c $(N64_CFLAGS) $(DIAG_OPT) -MMD -DMSX_DIAG=1 -o $@ $<
# Release-like diagnostic cartridge: the release game with MSX_DIAG=1.
$(BUILD_DIR)/mslugx-diag.elf: $(DIAG_GAME_OBJS)
$(BUILD_DIR)/mslugx-diag.elf: LDFLAGS += $(DIAG_LDFLAGS)
$(BUILD_DIR)/mslugx-reference.elf $(BUILD_DIR)/mslugx-m2-reference.elf $(BUILD_DIR)/mslugx-m3-reference.elf: LDFLAGS += $(if $(REF_DIAG),$(DIAG_LDFLAGS))
mslugx-diag.z64: N64_ROM_TITLE = "MSX DIAG"
mslugx-diag.z64: $(BUILD_DIR)/mslugx-diag.dfs
$(BUILD_DIR)/mslugx-diag.dfs: N64_MKDFS_ROOT = $(BUILD_DIR)/game-romfs
$(BUILD_DIR)/mslugx-diag.dfs: $(BUILD_DIR)/game-romfs.stamp
mslugx-explorer.z64: N64_ROM_TITLE = "MSX COLLISION v10"
mslugx-explorer.z64: $(BUILD_DIR)/mslugx-explorer.dfs
# The game cartridge carries only what the game reads; the raw FIX/C-ROM copies
# (fixed.bin, sprites.bin, 48 MiB) go only into the diagnostic lab cartridge.
$(BUILD_DIR)/mslugx-explorer.dfs: N64_MKDFS_ROOT = $(BUILD_DIR)/game-romfs
$(BUILD_DIR)/mslugx-explorer.dfs: $(BUILD_DIR)/game-romfs.stamp
# Gameplay audio (docs/AUDIO_JUEGO.md) is rendered locally from the user's ROM
# into filesystem/audio; without it the game runs silently.
GAME_AUDIO := $(wildcard filesystem/audio/*.wav64)
# 3D NEOGEO boot logo and jingle (docs/LOGO_NEOGEO.md): private assets from
# tools/build_neogeo_logo.py; without them the game boots straight in.
BOOT_LOGO := $(wildcard filesystem/logo/neogeo-logo.mesh filesystem/logo/neogeo-jingle.wav64)
# HOW TO PLAY, SOLDIER SELECT and the choice between the title and the entry
# (tools/pack_cinematic.py --name howto/select/confirm, docs/MISSION_ENTRY.md).
# The select music's continuation (tools/pack_select_music.py, B44) rides along.
BOOT_ATTRACT := $(wildcard $(foreach n,howto select confirm,filesystem/cinematics/$(n).msxc filesystem/cinematics/$(n).wav64) filesystem/cinematics/select-music-head.wav64 filesystem/cinematics/select-music-loop.wav64)
# select.wav64 is played only without the select music (main.c): with the
# music in the cartridge it would be 346 KB nobody reads.
ifneq ($(and $(wildcard filesystem/cinematics/select-music-head.wav64),$(wildcard filesystem/cinematics/select-music-loop.wav64)),)
BOOT_ATTRACT := $(filter-out filesystem/cinematics/select.wav64,$(BOOT_ATTRACT))
endif
# Tarma, Eri and Fio (docs/PERSONAJES.md; private, optional): each
# character's choice movie with its voice (tools/pack_cinematic.py --name
# confirm-cN --wav-compress 1) and its pack (tools/export_character.py).
# main.c offers a character only when the cartridge has all of its files.
BOOT_ATTRACT += $(wildcard $(foreach n,1 2 3,filesystem/cinematics/confirm-c$(n).msxc filesystem/cinematics/confirm-c$(n).wav64))
CHARACTER_PACKS := $(wildcard $(foreach n,1 2 3,romfs/chars/c$(n).bin))
# The options screen (src/msx_options.c, docs/OPCIONES.md): the ROM's
# soft-DIP table from tools/extract_softdip.py (private); without it no
# screen and the settings of the captures. The choice is kept in the
# cartridge EEPROM (4 Kbit, declared in the ROM header).
SOFTDIP := $(wildcard filesystem/softdip.bin)
N64_ROM_SAVETYPE = eeprom4k
# The fix-layer HUD (src/hud_renderer.c) draws the S-ROM tiles with the fix
# palettes of tools/build_hud_assets.py (docs/ARMAS_VEHICULOS.md §11).
# Mission packs (tools/pack_mission.py, docs/MISION2.md): private, loaded at
# the mission switch; without them the game stops there and restarts Mission 1.
MISSION_PACKS := $(wildcard romfs/mission[2-6].pack)
# Pre-rendered stage backgrounds (docs/RENDIMIENTO.md "Fondos
# pre-renderizados"): generated from the private packs for Mission 2 stage 2
# and Mission 3, only the stages and layers whose columns save RAM
# (tools/prerender_stage.py build); none without the packs.
PRERENDER_SPECS := romfs/mission2.pack:2 romfs/mission3.pack:all
MISSION_BGP := $(if $(MISSION_PACKS),$(BUILD_DIR)/bgp.stamp)
$(BUILD_DIR)/bgp.stamp: tools/prerender_stage.py $(MISSION_PACKS)
	@mkdir -p $(BUILD_DIR)
	python3 -I tools/prerender_stage.py build $(BUILD_DIR)/bgp $(PRERENDER_SPECS)
	touch $@
# The private Marco inputs are compared by content, not by time
# (tools/marco_inputs_sum.sh): a catalog copied with its old time (cp -p)
# left marco_renderer.o compiled against the previous catalog in test ROM 18.
MARCO_INPUTS_SUM := $(BUILD_DIR)/marco-inputs.sum
MARCO_INPUTS_SUM_RUN := $(shell sh tools/marco_inputs_sum.sh $(BUILD_DIR))
$(MARCO_INPUTS_SUM): ;
# MSXL packer (lossless 16-bit PCM, docs/COMPRESION.md): the raw WAV64s of
# the cinematics go into the cartridge packed; src/msx_audio.c plays both.
# make MSX_PCM_STATS=1: the "MSX PCM" measurement lines (after touching src/msx_pcm.h).
ifdef MSX_PCM_STATS
$(BUILD_DIR)/src/msx_pcm.o $(BUILD_DIR)/src/msx_audio.o: N64_CFLAGS += -DMSX_PCM_STATS=$(MSX_PCM_STATS)
endif
MSXL_RAW := neogeo-jingle.wav64
$(BUILD_DIR)/host/msxl: tools/msxl.c tools/msxl_encode.h src/msx_pcm_codec.h
	@mkdir -p $(BUILD_DIR)/host
	$(HOST_CC) $(HOST_CFLAGS) -O2 tools/msxl.c -lm -o $@
$(BUILD_DIR)/game-romfs.stamp: $(BUILD_DIR)/host/msxl $(BUILD_DIR)/soldier_ai_tables.h $(CHARACTER_PACKS) tools/msxc_compress.py romfs/scene.pack $(BUILD_DIR)/vehicle-sprites.stamp $(BUILD_DIR)/marco-poses.stamp $(MARCO_INPUTS_SUM) tools/check_marco_romfs.py $(GAME_AUDIO) $(BOOT_LOGO) $(BOOT_CINEMATICS) $(BOOT_ATTRACT) filesystem/fixed.bin filesystem/hud-palette.bin $(SOFTDIP) $(MISSION_PACKS) $(MISSION_BGP)
	# The sprites must be the compiled catalog's (else the console falls back to six states).
	python3 -I tools/check_marco_romfs.py $(BUILD_DIR)/marco_pose_meta.h romfs/marco
	rm -rf $(BUILD_DIR)/game-romfs && mkdir -p $(BUILD_DIR)/game-romfs
	cp romfs/scene.pack $(BUILD_DIR)/game-romfs/ && cp -r romfs/vehicles romfs/marco $(BUILD_DIR)/game-romfs/
	$(if $(MISSION_PACKS),cp $(MISSION_PACKS) $(BUILD_DIR)/game-romfs/)
	$(if $(CHARACTER_PACKS),mkdir -p $(BUILD_DIR)/game-romfs/chars && cp $(CHARACTER_PACKS) $(BUILD_DIR)/game-romfs/chars/)
	$(if $(MISSION_BGP),for f in $(BUILD_DIR)/bgp/m*s*.bgp; do [ -f "$$f" ] && cp "$$f" $(BUILD_DIR)/game-romfs/; done; true)
	cp filesystem/fixed.bin filesystem/hud-palette.bin $(BUILD_DIR)/game-romfs/
	$(if $(SOFTDIP),cp $(SOFTDIP) $(BUILD_DIR)/game-romfs/)
	cp $(BUILD_DIR)/programs.bin $(BUILD_DIR)/game-romfs/
	$(if $(GAME_AUDIO),mkdir -p $(BUILD_DIR)/game-romfs/audio && cp $(GAME_AUDIO) $(BUILD_DIR)/game-romfs/audio/)
	$(if $(BOOT_LOGO),cp $(BOOT_LOGO) $(BUILD_DIR)/game-romfs/)
	$(if $(BOOT_CINE_OBJS),cp filesystem/cinematics/title-motion.msxc $(BUILD_DIR)/game-romfs/title.msxc && cp filesystem/cinematics/title-motion.wav64 filesystem/cinematics/entry.msxc filesystem/cinematics/entry.wav64 $(BUILD_DIR)/game-romfs/)
	$(if $(and $(BOOT_CINE_OBJS),$(BOOT_ATTRACT)),cp $(BOOT_ATTRACT) $(BUILD_DIR)/game-romfs/)
	# Movies as MSXC v2 (LZ4 per packet, same frames; docs/MEMORY_OPTIMIZATION.md).
	for f in $(BUILD_DIR)/game-romfs/*.msxc; do [ -f "$$f" ] || continue; python3 -I tools/msxc_compress.py $$f $$f.v2 >/dev/null && mv $$f.v2 $$f || exit 1; done
	# Raw 16-bit WAV64s of the cinematics as MSXL (same samples; the packer
	# decodes each file before writing it and leaves the others as they are).
	# The NEOGEO logo's jingle stays raw (MSXL_RAW): the logo's frame takes
	# 9-12 ms of CPU and decoding its blocks in the spare time made 16 of
	# its frames late (docs/COMPRESION.md, Duplicados).
	for f in $(BUILD_DIR)/game-romfs/*.wav64; do [ -f "$$f" ] || continue; case $$(basename $$f) in $(MSXL_RAW)) continue;; esac; $(BUILD_DIR)/host/msxl pack $$f $$f.msxl >/dev/null || exit 1; if [ -f $$f.msxl ]; then mv $$f.msxl $$f; fi; done
	touch $@
# Reference traversal cartridge (tools/test_mission_native_ares.py): the same
# game with an invulnerable player (MSX_REFERENCE_INVULNERABLE); never the release.
$(BUILD_DIR)/reference$(REF_SUFFIX)/gameplay.o: src/gameplay.c
	@mkdir -p $(dir $@)
	$(N64_CC) -c $(N64_CFLAGS) -MMD -DMSX_REFERENCE_INVULNERABLE=1 $(REF_DIAG) -o $@ $<
$(BUILD_DIR)/mslugx-reference.elf: $(BUILD_DIR)/reference$(REF_SUFFIX)/gameplay.o $(filter-out $(BUILD_DIR)/$(REF_DIR)/gameplay.o,$(REF_OBJS))
# Screenshot cartridge (never a release): the reference game with the player
# dropped once above the waiting Camel Slug (MSX_DEBUG_CAMEL_DROP).
$(BUILD_DIR)/camelshot/gameplay.o: src/gameplay.c
	@mkdir -p $(dir $@)
	$(N64_CC) -c $(N64_CFLAGS) -MMD -DMSX_REFERENCE_INVULNERABLE=1 -DMSX_DEBUG_CAMEL_DROP=1 -o $@ $<
$(BUILD_DIR)/mslugx-camel-shot.elf: $(BUILD_DIR)/camelshot/gameplay.o $(filter-out $(BUILD_DIR)/src/gameplay.o,$(MSLUGX_GAME_OBJS))
mslugx-camel-shot.z64: N64_ROM_TITLE = "MSX CAMEL SHOT"
mslugx-camel-shot.z64: $(BUILD_DIR)/mslugx-camel-shot.dfs
$(BUILD_DIR)/mslugx-camel-shot.dfs: N64_MKDFS_ROOT = $(BUILD_DIR)/game-romfs
$(BUILD_DIR)/mslugx-camel-shot.dfs: $(BUILD_DIR)/game-romfs.stamp
# Mission 2 reference cartridge (tools/test_mission2_ares.py, never a
# release): the reference game starting at Mission 2's first record, with
# the stage-end hook (MSX_REFERENCE_STAGE_EXIT, gameplay.c).
$(BUILD_DIR)/m2ref$(REF_SUFFIX)/actor.o: src/actor.c
	@mkdir -p $(dir $@)
	$(N64_CC) -c $(N64_CFLAGS) -MMD -DMSX_REFERENCE_START_MISSION=2 $(REF_DIAG) -o $@ $<
$(BUILD_DIR)/mslugx-m2-reference.elf: $(BUILD_DIR)/reference$(REF_SUFFIX)/gameplay.o $(BUILD_DIR)/m2ref$(REF_SUFFIX)/actor.o $(filter-out $(BUILD_DIR)/$(REF_DIR)/gameplay.o $(BUILD_DIR)/$(REF_DIR)/actor.o,$(REF_OBJS))
mslugx-m2-reference.z64: N64_ROM_TITLE = "MSX M2 REFERENCE"
mslugx-m2-reference.z64: $(BUILD_DIR)/mslugx-m2-reference.dfs
$(BUILD_DIR)/mslugx-m2-reference.dfs: N64_MKDFS_ROOT = $(BUILD_DIR)/game-romfs
$(BUILD_DIR)/mslugx-m2-reference.dfs: $(BUILD_DIR)/game-romfs.stamp
# Transition cartridge (never a release): the release game ending Mission 1
# 600 steps after the landing (MSX_TEST_QUICK_END, actor.c), for the mission
# end, the tally, the fade and the first stage of Mission 2 in ares.
$(BUILD_DIR)/m1end/actor.o: src/actor.c
	@mkdir -p $(dir $@)
	$(N64_CC) -c $(N64_CFLAGS) -MMD -DMSX_TEST_QUICK_END=600 -o $@ $<
$(BUILD_DIR)/mslugx-m1end.elf: $(BUILD_DIR)/m1end/actor.o $(filter-out $(BUILD_DIR)/src/actor.o,$(MSLUGX_GAME_OBJS))
mslugx-m1end.z64: N64_ROM_TITLE = "MSX M1 END TEST"
mslugx-m1end.z64: $(BUILD_DIR)/mslugx-m1end.dfs
$(BUILD_DIR)/mslugx-m1end.dfs: N64_MKDFS_ROOT = $(BUILD_DIR)/game-romfs
$(BUILD_DIR)/mslugx-m1end.dfs: $(BUILD_DIR)/game-romfs.stamp
# Mission 3 reference cartridge (tools/test_mission3_ares.py, never a
# release): the same hooks starting at Mission 3's record 0173F6.
$(BUILD_DIR)/m3ref$(REF_SUFFIX)/actor.o: src/actor.c
	@mkdir -p $(dir $@)
	$(N64_CC) -c $(N64_CFLAGS) -MMD -DMSX_REFERENCE_START_MISSION=3 $(REF_DIAG) -o $@ $<
$(BUILD_DIR)/mslugx-m3-reference.elf: $(BUILD_DIR)/reference$(REF_SUFFIX)/gameplay.o $(BUILD_DIR)/m3ref$(REF_SUFFIX)/actor.o $(filter-out $(BUILD_DIR)/$(REF_DIR)/gameplay.o $(BUILD_DIR)/$(REF_DIR)/actor.o,$(REF_OBJS))
mslugx-m3-reference.z64: N64_ROM_TITLE = "MSX M3 REFERENCE"
mslugx-m3-reference.z64: $(BUILD_DIR)/mslugx-m3-reference.dfs
$(BUILD_DIR)/mslugx-m3-reference.dfs: N64_MKDFS_ROOT = $(BUILD_DIR)/game-romfs
$(BUILD_DIR)/mslugx-m3-reference.dfs: $(BUILD_DIR)/game-romfs.stamp
# Release-like Mission 3 cartridge (never a release): the release game
# (mortal player, no script skips, no lock releases, no autopilot) starting
# at Mission 3's record 0173F6, for hand-like play in ares (B56-B58).
$(BUILD_DIR)/m3rel/actor.o: src/actor.c
	@mkdir -p $(dir $@)
	$(N64_CC) -c $(N64_CFLAGS) -MMD -DMSX_REFERENCE_START_MISSION=3 -o $@ $<
$(BUILD_DIR)/mslugx-m3-release.elf: $(BUILD_DIR)/m3rel/actor.o $(filter-out $(BUILD_DIR)/src/actor.o,$(MSLUGX_GAME_OBJS))
mslugx-m3-release.z64: N64_ROM_TITLE = "MSX M3 RELEASE TEST"
mslugx-m3-release.z64: $(BUILD_DIR)/mslugx-m3-release.dfs
$(BUILD_DIR)/mslugx-m3-release.dfs: N64_MKDFS_ROOT = $(BUILD_DIR)/game-romfs
$(BUILD_DIR)/mslugx-m3-release.dfs: $(BUILD_DIR)/game-romfs.stamp
mslugx-reference.z64: N64_ROM_TITLE = "MSX M1 REFERENCE"
mslugx-reference.z64: $(BUILD_DIR)/mslugx-reference.dfs
$(BUILD_DIR)/mslugx-reference.dfs: N64_MKDFS_ROOT = $(BUILD_DIR)/game-romfs
$(BUILD_DIR)/mslugx-reference.dfs: $(BUILD_DIR)/game-romfs.stamp
$(BUILD_DIR)/src/main-lab.o: src/main.c
	@mkdir -p $(dir $@)
	$(N64_CC) -c $(N64_CFLAGS) -MMD -DMSX_GRAPHICS_LAB=1 -o $@ $<
$(BUILD_DIR)/mslugx-lab.elf: $(BUILD_DIR)/src/main-lab.o $(filter-out $(BUILD_DIR)/src/main.o,$(MSLUGX_GAME_OBJS))
mslugx-lab.z64: N64_ROM_TITLE = "MSX GRAPHICS LAB"
mslugx-lab.z64: $(BUILD_DIR)/mslugx-lab.dfs
$(BUILD_DIR)/mslugx-lab.dfs: romfs/scene.pack filesystem/fixed.bin filesystem/sprites.bin $(BUILD_DIR)/vehicle-sprites.stamp
$(BUILD_DIR)/scene_meta.h $(BUILD_DIR)/boss_columns.h romfs/scene.pack &: tools/pack_scene.py tools/pack_store.py tools/lz4_blocks.py tools/lz4opt.c tools/mission_tiles.py tools/export_marco.py filesystem/projectiles.json filesystem/mission1/map.json filesystem/mission1/props.json filesystem/marco.idx4 filesystem/marco-actions.idx4 filesystem/marco-motion.idx4 filesystem/soldier.idx4 filesystem/fixed.bin filesystem/sprites.bin filesystem/marco.bin filesystem/marco-actions.bin filesystem/desert.bin filesystem/soldier.bin filesystem/marco-motion.bin filesystem/soldier-native.json filesystem/boss-native.json data/boss_programs.json data/soldier_programs.json data/mission1_rest_inventory.json $(wildcard filesystem/mission1/rest/map.json filesystem/rest-native.json filesystem/blood-native.json)
	python3 tools/pack_scene.py
$(BUILD_DIR)/src/scene_assets.o: $(BUILD_DIR)/scene_meta.h
$(BUILD_DIR)/src/boss_renderer.o: $(BUILD_DIR)/scene_meta.h
$(BUILD_DIR)/src/neo_world.o $(BUILD_DIR)/src/neo_boss.o: $(BUILD_DIR)/boss_tables.h $(BUILD_DIR)/soldier_ai_tables.h $(BUILD_DIR)/region_rom.h
$(BUILD_DIR)/src/actor.o $(BUILD_DIR)/m1end/actor.o $(BUILD_DIR)/m2ref$(REF_SUFFIX)/actor.o $(BUILD_DIR)/m3ref$(REF_SUFFIX)/actor.o $(BUILD_DIR)/src/scene_renderer.o: $(BUILD_DIR)/visual_config.h
$(BUILD_DIR)/visual_config.h: FORCE tools/build_visual_config.py
	python3 tools/build_visual_config.py $(MSX_VISUAL_MODE) --scenario $(MSX_SCENARIO) --depth-style $(MSX_DEPTH_STYLE) --output $@
$(BUILD_DIR)/src/neo_collision.o: $(BUILD_DIR)/mission_collision.h $(BUILD_DIR)/vehicle_wreckage.h
$(BUILD_DIR)/src/neo_vehicle.o: $(BUILD_DIR)/vehicle_anim.h
$(BUILD_DIR)/vehicle_anim.h $(BUILD_DIR)/vehicle_wreckage.h &: tools/compile_vehicle.py filesystem/vehicle.json data/vehicle_wreckage.json
	python3 tools/compile_vehicle.py
$(BUILD_DIR)/vehicle-sprites.stamp: tools/build_vehicle_sprites.py filesystem/vehicle.json
	python3 tools/build_vehicle_sprites.py --mksprite $(N64_INST)/bin/mksprite
# The original's pause 015520 against its MAME capture (docs/PAUSA.md).
$(BUILD_DIR)/pause_fixture.h: tools/pause_fixture.py data/pause_fixture.json
	python3 tools/pause_fixture.py --json data/pause_fixture.json --header $@
check-pause: $(BUILD_DIR)/pause_fixture.h
	mkdir -p $(BUILD_DIR)/host
	$(HOST_CC) $(HOST_CFLAGS) -I$(BUILD_DIR) tools/test_gameplay.c src/gameplay.c src/neo_pause.c src/msx_sfx.c src/neo_collision.c src/neo_vehicle.c $(WORLD_SRCS) -o $(BUILD_DIR)/host/gameplay
	$(BUILD_DIR)/host/gameplay pause
$(BUILD_DIR)/soldier_fixture.h: tools/compile_soldier_fixture.py data/first_soldier_init_fixture.json
	python3 tools/compile_soldier_fixture.py
$(BUILD_DIR)/rng_fixture.h: tools/compile_rng_fixture.py data/soldier_rng_fixture.json
	python3 tools/compile_rng_fixture.py
$(BUILD_DIR)/tools/test_neo_rng.o: $(BUILD_DIR)/rng_fixture.h
# Private graphics conversion is an explicit offline step; never needs ROMs in CI.
$(BUILD_DIR)/marco_pose_meta.h $(BUILD_DIR)/marco-poses.stamp:
	@echo "Run tools/build_marco_poses.py with an audited local capture first."
	@exit 1
$(BUILD_DIR)/src/marco_renderer.o $(BUILD_DIR)/tools/test_marco_render.o: $(BUILD_DIR)/marco_pose_meta.h $(MARCO_INPUTS_SUM)
# Campaign Marco programs (docs/MARCO_CAMPAIGN.md): also an offline step from
# the audited catalog capture (tools/compile_marco_campaign.py).
$(BUILD_DIR)/marco_campaign.h:
	@echo "Run tools/compile_marco_campaign.py with the audited catalog capture first."
	@exit 1
$(BUILD_DIR)/src/marco_visual.o: $(BUILD_DIR)/marco_campaign.h $(MARCO_INPUTS_SUM)
$(BUILD_DIR)/tools/test_marco_render.o: CC = $(N64_CC)
$(BUILD_DIR)/tools/test_marco_render.o: CFLAGS = $(N64_CFLAGS) -MMD
$(BUILD_DIR)/marco-render.elf: $(BUILD_DIR)/tools/test_marco_render.o $(BUILD_DIR)/src/marco_renderer.o
$(BUILD_DIR)/marco-render.dfs: N64_MKDFS_ROOT = $(BUILD_DIR)/marco-render-romfs
$(BUILD_DIR)/marco-render-romfs/.stamp: $(BUILD_DIR)/marco-poses.stamp $(MARCO_INPUTS_SUM)
	mkdir -p $(BUILD_DIR)/marco-render-romfs
	cp -r romfs/marco $(BUILD_DIR)/marco-render-romfs/
	touch $@
$(BUILD_DIR)/marco-render.dfs: $(BUILD_DIR)/marco-render-romfs/.stamp
marco-render.z64: N64_ROM_TITLE = "MSX MARCO PIXELS"
marco-render.z64: $(BUILD_DIR)/marco-render.dfs

$(BUILD_DIR)/tools/test_marco_fields.o: CC = $(N64_CC)
$(BUILD_DIR)/tools/test_marco_fields.o: CFLAGS = $(N64_CFLAGS) -MMD
$(BUILD_DIR)/tools/test_marco_fields.o: $(BUILD_DIR)/marco_fields.h $(BUILD_DIR)/marco_selectors.h
$(BUILD_DIR)/marco-regression.elf: $(BUILD_DIR)/tools/test_marco_fields.o $(BUILD_DIR)/src/neo_animation.o
marco-regression.z64: N64_ROM_TITLE = "MSX MARCO TEST"

$(BUILD_DIR)/rng-regression.elf: $(BUILD_DIR)/tools/test_neo_rng.o $(BUILD_DIR)/src/neo_rng.o $(BUILD_DIR)/src/neo_soldier.o
rng-regression.z64: N64_ROM_TITLE = "MSX RNG TEST"
$(BUILD_DIR)/animation_fixture.h: tools/compile_animation_fixture.py data/soldier_animation_fixture.json
	python3 tools/compile_animation_fixture.py
$(BUILD_DIR)/tools/test_neo_animation.o: $(BUILD_DIR)/animation_fixture.h
$(BUILD_DIR)/animation-regression.elf: $(BUILD_DIR)/tools/test_neo_animation.o $(BUILD_DIR)/src/neo_animation.o
animation-regression.z64: N64_ROM_TITLE = "MSX ANIMATION TEST"
$(BUILD_DIR)/contact_fixture.h $(BUILD_DIR)/contact_tables.h &: tools/compile_contact_fixture.py data/soldier_contact_fixture.json data/soldier_animation_fixture.json
	python3 tools/compile_contact_fixture.py
$(BUILD_DIR)/src/neo_contact.o: $(BUILD_DIR)/contact_tables.h
$(BUILD_DIR)/tools/test_neo_contact.o: $(BUILD_DIR)/contact_fixture.h $(BUILD_DIR)/animation_fixture.h
$(BUILD_DIR)/contact-regression.elf: $(BUILD_DIR)/tools/test_neo_contact.o $(BUILD_DIR)/src/neo_contact.o $(BUILD_DIR)/src/neo_collision.o $(BUILD_DIR)/src/neo_animation.o
contact-regression.z64: N64_ROM_TITLE = "MSX CONTACT TEST"
$(BUILD_DIR)/control_fixture.h $(BUILD_DIR)/control_tables.h &: tools/compile_control_fixture.py data/soldier_control_fixture.json data/soldier_contact_fixture.json data/soldier_animation_fixture.json
	python3 tools/compile_control_fixture.py
$(BUILD_DIR)/src/neo_control.o: $(BUILD_DIR)/control_tables.h
$(BUILD_DIR)/tools/test_neo_control.o: $(BUILD_DIR)/control_fixture.h $(BUILD_DIR)/contact_fixture.h $(BUILD_DIR)/animation_fixture.h
$(BUILD_DIR)/control-regression.elf: $(BUILD_DIR)/tools/test_neo_control.o $(BUILD_DIR)/src/neo_control.o $(BUILD_DIR)/src/neo_rng.o $(BUILD_DIR)/src/neo_contact.o $(BUILD_DIR)/src/neo_collision.o $(BUILD_DIR)/src/neo_animation.o
control-regression.z64: N64_ROM_TITLE = "MSX CONTROL TEST"
$(BUILD_DIR)/state_fixture.h $(BUILD_DIR)/state_tables.h &: tools/compile_state_fixture.py data/soldier_state_fixture.json data/soldier_control_fixture.json data/soldier_animation_fixture.json
	python3 tools/compile_state_fixture.py
$(BUILD_DIR)/action_fixture.h: tools/compile_action_fixture.py data/soldier_action_fixture.json
	python3 tools/compile_action_fixture.py
$(BUILD_DIR)/cull_fixture.h: tools/compile_cull_fixture.py data/soldier_cull_fixture.json data/soldier_control_fixture.json
	python3 tools/compile_cull_fixture.py
$(BUILD_DIR)/src/neo_soldier_state.o $(BUILD_DIR)/src/neo_soldier_action.o: $(BUILD_DIR)/state_tables.h
$(BUILD_DIR)/tools/test_neo_state.o: $(BUILD_DIR)/cull_fixture.h $(BUILD_DIR)/action_fixture.h tools/test_neo_control.c $(BUILD_DIR)/state_fixture.h $(BUILD_DIR)/control_fixture.h $(BUILD_DIR)/contact_fixture.h $(BUILD_DIR)/animation_fixture.h
$(BUILD_DIR)/state-regression.elf: $(BUILD_DIR)/src/neo_cull.o $(BUILD_DIR)/tools/test_neo_state.o $(BUILD_DIR)/src/neo_soldier_state.o $(BUILD_DIR)/src/neo_soldier_action.o $(BUILD_DIR)/src/neo_soldier.o $(BUILD_DIR)/src/neo_control.o $(BUILD_DIR)/src/neo_rng.o $(BUILD_DIR)/src/neo_contact.o $(BUILD_DIR)/src/neo_collision.o $(BUILD_DIR)/src/neo_animation.o
state-regression.z64: N64_ROM_TITLE = "MSX STATE TEST"
$(BUILD_DIR)/throw_fixture.h $(BUILD_DIR)/throw_tables.h &: tools/compile_throw_fixture.py data/soldier_throw_fixture.json
	python3 tools/compile_throw_fixture.py
$(BUILD_DIR)/object_fixture.h: tools/compile_object_fixture.py data/soldier_object_fixture.json
	python3 tools/compile_object_fixture.py
$(BUILD_DIR)/attack_fixture.h: tools/compile_attack_fixture.py data/soldier_attack_fixture.json data/soldier_throw_fixture.json
	python3 tools/compile_attack_fixture.py
$(BUILD_DIR)/src/neo_throw.o: $(BUILD_DIR)/throw_tables.h
$(BUILD_DIR)/tools/test_neo_throw.o: $(BUILD_DIR)/throw_fixture.h
$(BUILD_DIR)/tools/test_neo_object.o: $(BUILD_DIR)/throw_fixture.h $(BUILD_DIR)/object_fixture.h
$(BUILD_DIR)/attack_state_fixture.h: tools/compile_attack_state_fixture.py data/soldier_attack_state_fixture.json data/soldier_hit_geometry_fixture.json
	python3 tools/compile_attack_state_fixture.py
$(BUILD_DIR)/tools/test_neo_attack.o: $(BUILD_DIR)/attack_state_fixture.h $(BUILD_DIR)/throw_fixture.h $(BUILD_DIR)/attack_fixture.h $(BUILD_DIR)/object_fixture.h
$(BUILD_DIR)/throw-regression.elf: $(BUILD_DIR)/tools/test_neo_throw.o $(BUILD_DIR)/src/neo_throw.o $(BUILD_DIR)/src/neo_target.o $(BUILD_DIR)/src/neo_contact.o $(BUILD_DIR)/src/neo_collision.o
throw-regression.z64: N64_ROM_TITLE = "MSX THROW TEST"
$(BUILD_DIR)/attack-regression.elf: $(BUILD_DIR)/src/neo_hit.o $(BUILD_DIR)/src/neo_soldier_state.o $(BUILD_DIR)/src/neo_soldier.o $(BUILD_DIR)/src/neo_soldier_action.o $(BUILD_DIR)/src/neo_rng.o $(BUILD_DIR)/src/neo_throw_actor.o $(BUILD_DIR)/src/neo_cull.o $(BUILD_DIR)/tools/test_neo_attack.o $(BUILD_DIR)/src/neo_animation.o $(BUILD_DIR)/src/neo_soldier_effects.o $(BUILD_DIR)/src/neo_throw.o $(BUILD_DIR)/src/neo_target.o $(BUILD_DIR)/src/neo_contact.o $(BUILD_DIR)/src/neo_collision.o
attack-regression.z64: N64_ROM_TITLE = "MSX ATTACK TEST"
$(BUILD_DIR)/object-regression.elf: $(BUILD_DIR)/tools/test_neo_object.o $(BUILD_DIR)/src/neo_animation.o $(BUILD_DIR)/src/neo_throw_actor.o $(BUILD_DIR)/src/neo_throw.o $(BUILD_DIR)/src/neo_target.o $(BUILD_DIR)/src/neo_contact.o $(BUILD_DIR)/src/neo_collision.o $(BUILD_DIR)/src/neo_cull.o
object-regression.z64: N64_ROM_TITLE = "MSX OBJECT TEST"
$(BUILD_DIR)/locomotion_fixture.h: tools/compile_locomotion_fixture.py data/soldier_locomotion_fixture.json
	python3 tools/compile_locomotion_fixture.py
$(BUILD_DIR)/tools/test_neo_locomotion.o: $(BUILD_DIR)/locomotion_fixture.h
$(BUILD_DIR)/locomotion-regression.elf: $(BUILD_DIR)/tools/test_neo_locomotion.o $(BUILD_DIR)/src/neo_soldier_state.o $(BUILD_DIR)/src/neo_soldier.o $(BUILD_DIR)/src/neo_soldier_action.o $(BUILD_DIR)/src/neo_rng.o $(BUILD_DIR)/src/neo_contact.o $(BUILD_DIR)/src/neo_collision.o $(BUILD_DIR)/src/neo_animation.o
locomotion-regression.z64: N64_ROM_TITLE = "MSX LOCO TEST"
$(BUILD_DIR)/soldier_ai_tables.h: tools/compile_soldier_ai_tables.py data/soldier_ai_tables.json data/soldier_programs.json data/mission2_script.json data/mission3_script.json data/mission4_script.json data/mission5_script.json data/mission6_script.json
	python3 tools/compile_soldier_ai_tables.py
$(BUILD_DIR)/soldier_tick_fixture.h: tools/compile_soldier_ticks.py data/soldier_tick_fixture.json.gz
	python3 tools/compile_soldier_ticks.py
# Soldier deaths by rows of kinds 7 and 0A (the flame of weapon 04 and the
# blast of weapon 02): trace_soldier_campaign.lua MSX_ASSIST_WEAPON=4 / 2
# MSX_SOLDIER_ENTRY=1 (b12w4 / b12w2, frames 2100..3270), extract_soldier_ticks.py
# --entry --only-deaths: every update of a dying soldier from its entry bytes.
$(BUILD_DIR)/soldier_death_tick_fixture.h: tools/compile_soldier_ticks.py data/soldier_death_tick_fixture.json.gz
	python3 tools/compile_soldier_ticks.py data/soldier_death_tick_fixture.json.gz $@
$(BUILD_DIR)/src/neo_soldier_ai.o: $(BUILD_DIR)/soldier_ai_tables.h
$(BUILD_DIR)/src/neo_script.o: $(BUILD_DIR)/soldier_ai_tables.h
$(BUILD_DIR)/src/neo_world.o: $(BUILD_DIR)/soldier_ai_tables.h
# Stage transition actors 3AED8/3AEF0 (src/neo_wipe.c) against the Mission 2
# stage 1 -> 2 family trace (docs/MISION2.md).
$(BUILD_DIR)/wipe_fixture.h: tools/extract_wipe_fixture.py data/wipe_fixture.json.gz
	@mkdir -p $(BUILD_DIR)
	python3 tools/extract_wipe_fixture.py --from-json data/wipe_fixture.json.gz --output data/wipe_fixture.json.gz --header $@
$(BUILD_DIR)/mission_flow_fixture.h: tools/compile_mission_flow_fixture.py data/mission_flow_fixture.json
	python3 tools/compile_mission_flow_fixture.py
$(BUILD_DIR)/region_rom.h: tools/compile_region_rom.py data/region_rom.json
	python3 tools/compile_region_rom.py
# LSPC auto-animation (G25): REG_LSPCMODE writes and counter samples of run4/spr.
$(BUILD_DIR)/lspc_fixture.h: data/lspc_fixture.json tools/compile_lspc_fixture.py
	@mkdir -p $(BUILD_DIR)
	python3 tools/compile_lspc_fixture.py $< $@
$(BUILD_DIR)/m3ledge_fixture.h: tools/compile_m3ledge_fixture.py data/m3ledge_fixture.json
	python3 tools/compile_m3ledge_fixture.py
$(BUILD_DIR)/train_fixture.h: tools/compile_train_fixture.py data/train_fixture.json.gz
	@mkdir -p $(BUILD_DIR)
	python3 tools/compile_train_fixture.py data/train_fixture.json.gz $@
$(BUILD_DIR)/camera_fixture.h: tools/compile_camera_fixture.py data/camera_fixture.json.gz
	python3 tools/compile_camera_fixture.py
$(BUILD_DIR)/robe_fixture.h: tools/compile_robe_fixture.py data/robe_fixture.json.gz
	python3 tools/compile_robe_fixture.py
# Mission-end actors (src/neo_end.c) against the runK family trace
# (tools/extract_end_fixture.py): the MISSION COMPLETE letters 06090A.
$(BUILD_DIR)/end_letters_fixture.h: tools/compile_end_fixture.py data/end_letters_fixture.json.gz
	@mkdir -p $(BUILD_DIR)
	python3 tools/compile_end_fixture.py data/end_letters_fixture.json.gz --output $@ --name end_letters
# MISSION 1 START 0607DE (same family) against the ms2 family trace.
$(BUILD_DIR)/start_letters_fixture.h: tools/compile_end_fixture.py data/start_letters_fixture.json.gz
	@mkdir -p $(BUILD_DIR)
	python3 tools/compile_end_fixture.py data/start_letters_fixture.json.gz --output $@ --name start_letters
# Prisoner tally 0308C0 (src/neo_tally.c, text engine src/neo_text.c)
# against the family traces with VRAM writes of tal1 (counted without the
# skip), talK (A/B skip), tal0 (no prisoner) and tal2 (two special
# prisoners) (tools/extract_tally_fixture.py, docs/FIN_PARTIDA.md).
TALLIES := tal1 talK tal0 tal2
# The mission actor's CONTINUE? sprites (src/neo_continue.c) against the
# sprite-list entries of the go2 game-over capture.
# Parachute landing actors (src/neo_landing.c) against the land2 family trace.
$(BUILD_DIR)/landing_fixture.h: tools/extract_landing_fixture.py data/landing_fixture.json.gz
	@mkdir -p $(BUILD_DIR)
	python3 tools/extract_landing_fixture.py --from-json data/landing_fixture.json.gz --output data/landing_fixture.json.gz --header $@
# The mummy transformation's smoke 0C4DD8 and marker 0C5286 (src/neo_landing.c)
# against the mz4 family trace (capture_fullgame.lua MSX_FULL_GIVE_MUMMY).
$(BUILD_DIR)/smoke_fixture.h: tools/extract_landing_fixture.py data/smoke_fixture.json.gz
	@mkdir -p $(BUILD_DIR)
	python3 tools/extract_landing_fixture.py --from-json data/smoke_fixture.json.gz --output data/smoke_fixture.json.gz --header $@
# Parachute descent (src/marco_descent.c, canopy of src/neo_landing.c)
# against the ch0/chA/chB family traces.
$(BUILD_DIR)/descent_fixture.h: tools/extract_descent_fixture.py data/descent_fixture.json.gz
	@mkdir -p $(BUILD_DIR)
	python3 tools/extract_descent_fixture.py --from-json data/descent_fixture.json.gz --output data/descent_fixture.json.gz --header $@
$(BUILD_DIR)/continue_sprites_fixture.h: tools/compile_continue_sprites.py data/continue_sprites_fixture.json.gz
	@mkdir -p $(BUILD_DIR)
	python3 tools/compile_continue_sprites.py data/continue_sprites_fixture.json.gz --output $@
TALLY_HEADERS := $(patsubst %,$(BUILD_DIR)/tally_%_fixture.h,$(TALLIES))
$(BUILD_DIR)/tally_%_fixture.h: tools/compile_tally_fixture.py data/tally_%_fixture.json.gz
	@mkdir -p $(BUILD_DIR)
	python3 tools/compile_tally_fixture.py data/tally_$*_fixture.json.gz --output $@ --name tally_$*
# Generic-actor families checked against a MAME family trace
# (tools/extract_family_fixture.py -> data/<family>_fixture.json.gz).
# FAMILY_ARGS_<family>: extra options of the compiler (--tree: the first
# node and its descendants only).
FAMILIES := dome barrel figure tankhit tankkill tankair truck swordgen arab creature prisoner hitem citem hbullet pbullet grenade chaser armored secret swords prisoner2 bossprisoner bossprisonerb food rocket heli camel decor awning porter slug civil blast debris flash label tank tankjump tankcrouch tankpush rlaunch dball sgun2 sgun7 flame4 flame9 goarrow exitmark rebel2 rebel3 stageexit gate secret2 crate bags m2gate stage3 drone hostage miner minerrun signs prisoner3 prisoner4 bats rlaunch8 laser6 laserb coin treasure vent ventwait wrapped rope ropedeath molotov boulder robeflag slugidle slugfall slugtour sluglaunch m2boss1 m2boss8 m3wagon m3platform m3grenade m3grenade4 m3shield m3shield4 m3window m3mid1 m3mid2 m3mid3 m3mid4 m3para m3pod m3gun m3gun4 m3thrower m3bomber m3drum m3truck m3raft m3car m3jeep m3bird m3sack m3camp m3bonus m3tank m3tankair m3veh m3veh2 m3wepe m3fat m3fat2 m3bomb m3molotov m3splash m3boss0 m3boss1 m3boss3 m3boss2 m3boss4 m3boss5 m3boss6 m2creature m2ripple m4props m4flyer m4flyer2 m4bin m4urn m4van m4van2 m4rock m4jump m4box m4heli m4spark m4truck m4sold m4sold2 m4pris m4pris2 m4animal m4label m5gate m5drip m5deco m5street m5ledge m5car m5signal m5door m5stain m5jet m5tank m5heli m5tread m5tread2 m5tread3 m5heli2 m5heli3 m4veh m4veh2 m4veh3 m4veh4 m5train m5mech m6raft m6span m6mid m5pow m5sewer m4boss
# m4props = the Mission 4 props of the camera list 0E741A (neo_m4props.c):
# the destructible fronts 0B9F4A (32 records 0E68D0, map patches 0C70FC into
# the ring of 10C72E), their debris 0BB106, the lamp 0B9B24, the decorations
# 0B9AD6 / 0B99EE, the collapse keeper 0B9C7A and its pieces 0B9D88; capture
# A from captures-m4/states sec05_149829 (family 149830..157600,
# docs/MISION4.md "Fase 2"). RAM 10C6C0..10C6FF through neo_ga_flags_t m4.
FAMILY_ARGS_m4props := --attacks --terrain 10c72e --ring 10c72e --group-map 0 --root-words --reserve --script-actor 101414 --script-routines bb106
# m4flyer / m4flyer2 = the Mission 4 flyers 083C8A and 08391C (neo_m3enemy.c,
# the family of the Mission 3 bomber): m4flyer = capture D (run4's own pad
# from sec05_149829, roots 83c8a,8391c,a9f0c,aa37c,aa2c6 up to 152400: nine
# 083C8A and the 08391C shot down in flight, together with the van 0A9F0C:
# the ninth flyer lands on the van's dynamic map 0E6704; extracted with
# --range 8391c-84c98 --range a9f0c-aa458 --hit-writes),
# m4flyer2 = capture C (MSX_FULL_NOFIRE=150090-153000: they land, drop the
# rider 0842A8 and crash; the trail 083B92 of 08391C). The rider's soldier
# 0AF17E is outside the family.
FAMILY_ARGS_m4flyer := --attacks --terrain 10c72e --ring 10c72e --group-map 0 --root-words --reserve --script-actor 101414 --script-routines 8c374 --late-creator --split-children
FAMILY_ARGS_m4flyer2 := --attacks --terrain 10c72e --ring 10c72e --group-map 0 --root-words --reserve --script-actor 101414 --script-routines 8c374 --late-creator --split-children --node-fields 7a
# m4bin = the Mission 4 bins 033526 (script records 2DB6A2 / 2DB86E; neo_m4props.c):
# hits, the items and the prisoner 099DD6 thrown out by the program call
# 239C80 with its whole life (neo_prisoner.c; the other prisoners of capture
# A come along), the puffs 03378A, the pieces 033864; capture A, --range
# 33526-33920 --range 99dcc-9a71d --range 9a97c-9aa9e (the ripples 03C62A
# stay outside: the soldiers' are in the same capture).
FAMILY_ARGS_m4bin := --attacks --terrain 10c72e --ring 10c72e --group-map 0 --root-words --reserve --script-actor 101414 --script-routines 3378a,33864,99dd6
# m4urn = the Mission 4 object 036C34 (five script records; neo_m4props.c): two
# stages with their flinches, the end 23BEBE and the blast 036EA8; capture A.
FAMILY_ARGS_m4urn := --attacks --terrain 10c72e --ring 10c72e --group-map 0 --root-words --reserve --script-actor 101414 --script-routines 36ea8
# m4van = the Mission 4 van 0A9F0C (script record 2DBADE; neo_m4props.c): its
# damage stages, overlay 0AA37C, items, the blast 0AA2C6 and the wreck;
# capture A.
FAMILY_ARGS_m4van := --attacks --terrain 10c72e --ring 10c72e --group-map 0 --root-words --reserve --script-actor 101414 --script-routines 8c374
# m5gate = the Mission 5 sewer gates 0BC296 / 0BC3A6 (script records, neo_m5props.c):
# hits, the wall stamps 0C88B0 / 0C8A3C into the ring of 10C72E, the debris
# lists 0BBD78 / 0BBD8A (01DA40) and 10C700; captures-m5/fam/G (run4 replay
# from sec08_274387, 274388..276000).
FAMILY_ARGS_m5gate := --attacks --terrain 10c72e --ring 10c72e --group-map 0 --root-words --reserve
# m5drip = the Mission 5 sewer drips 08346E (script and runner records) and their drops
# 0834F4 made after the script actor (neo_m5props.c); captures-m5/fam/D (run4 replay
# from sec08_274387, 274388..275600).
FAMILY_ARGS_m5drip := --attacks --terrain 10c72e --ring 10c72e --group-map 0 --root-words --script-actor 101414 --script-routines 834f4
# m5ledge = the Mission 5 street ledges 09624E (script records; cell maps 095B54 by 0140C4, never drawn);
# m5car = the wrecked cars 050542 with their wheels 05090A / 05092C, blast 05094E, panels 0509A8, pieces
# 050B52 and the items 08C374 of their programs' calls (neo_m5props.c); captures-m5/fam/T (run4 replay
# from sec06_247661, 247662..252800).
# m5signal = the Mission 5 metro signals 0BC1E6 (script records; lit while their group has somebody, 019DCA);
# captures-m5/fam/M (run4 replay from sec07_252827, 252828..274330).
FAMILY_ARGS_m5signal := --attacks --terrain 10c72e --ring 10c72e --group-map 0 --root-words --reserve
# m5door = the Mission 5 metro's end 0BBF12 (three stages, walls 0BBAF4 / 0BBBB0 by 0C88B0, the pieces 0BC100 /
# 0BC192, the stage exit 0B6ED2 as its child); captures-m5/fam/M.
FAMILY_ARGS_m5door := --attacks --terrain 10c72e --ring 10c72e --group-map 0 --root-words --reserve
# m5stain = the Mission 5 sewer stains 032B46 / 032B5A / 032B6E; m5jet = the jets 0836E8 and their puffs 083820
# (neo_m5props.c); captures-m5/fam/W (run4 replay from sec08_274387, 274388..277080).
FAMILY_ARGS_m5stain := --attacks --terrain 10c72e --ring 10c72e --group-map 0 --root-words --reserve
FAMILY_ARGS_m5jet := --attacks --terrain 10c72e --ring 10c72e --group-map 0 --root-words --reserve
# m5tank = the Mission 5 street tank 08DCB2 with its turret 08EB18, flashes 08EFFA and shells 08F058 (neo_m5enemy.c);
# captures-m5/fam/K (run4 replay from sec06_247661 without fire in 248100..249600: it drives, turns, aims, fires
# and is destroyed at 249798).
FAMILY_ARGS_m5tank := --attacks --terrain 10c72e --ring 10c72e --group-map 0 --root-words --reserve --shared 8dcb2
# m5heli = the Mission 5 lone helicopter 0474B6 (neo_heli.c: its rotor 047874, droppers 0479B8, bombs 047AFC and its
# death 048B06); captures-m5/fam/T (the first helicopter of the street and its descendants).
FAMILY_ARGS_m5heli := --script-actor 101414 --script-routines 47de0 --late-creator --hit-to 47c48 --hit-moving --attacks --terrain 10c72e --ring 10c72e --group-map 0 --root-words --reserve
# m5tread = the Mission 5 tank on treads 08FF7E (street record 2DCFA4) with its tread node 014A4E, its turret 090A62, flashes
# 091290 and shells 0912EE (neo_m5enemy.c); captures-m5/fam/T (run4 replay from sec06_247661): the two tanks of the street.
FAMILY_ARGS_m5tread := --node-fields 7a --attacks --terrain 10c72e --ring 10c72e --group-map 0 --root-words --reserve --shared 14a4e
# m5tread2 = the same tank in the sewer (records 2DF502..): captures-m5/fam/N (run4 replay from sec08_274387, roots 8ff7e,
# 276150..277080: the four tanks before the boss, on the sewer's slopes).
FAMILY_ARGS_m5tread2 := $(FAMILY_ARGS_m5tread)
# m5tread3 = the same tanks left alive: captures-m5/fam/N2 (the same replay without fire in 276190..278600: they drive over
# the sewer's slopes, tilt, aim and fire 18 shells).
FAMILY_ARGS_m5tread3 := $(FAMILY_ARGS_m5tread)
# m5heli2 = the other lone helicopter 047F7E (neo_heli.c: its droppers 047A12 and bombs 047B10) with the 0474B6 that
# fly beside it; captures-m5/fam/H (run4 replay from sec07_252827, roots 474b6,47f7e,479b8,47a12,47afc,47b10, the metro
# from 263800 to 269500: four 047F7E and two 0474B6 with all their bombs and deaths).
FAMILY_ARGS_m5heli2 := --node-fields 7c $(FAMILY_ARGS_m5heli)
# m5heli3 = the leave 04783A / 047856 of the helicopter 047F7E of record 2DE3DA (+72 = 30): captures-m5/fam/L (the replay
# of H without fire in 267800..276500), only its frames 274700..274900 (the nodes alive at the cut start from their
# recorded bytes).
FAMILY_ARGS_m5heli3 := --resume $(FAMILY_ARGS_m5heli2)
# m5train = the Mission 5 metro train 051344 (record 2DDF60) with its wheels, bogies, panels, sparks, smoke and pieces
# (neo_m5train.c); captures-m5/fam/R (run4 replay from sec07_252827, 262380..263700: the first train, destroyed at 263176).
FAMILY_ARGS_m5train := --script-actor 101414 --script-routines 1c706,1c6ee,8c374 --attacks --terrain 10c72e --ring 10c72e --group-map 0 --root-words --reserve
# m5mech = the Mission 5 metro machine 0D3B36 (camera 992) with its arms, pilot, shells, muzzles, pieces, grenade, helmet
# and sparks (neo_m5mech.c); captures-m5/fam/Q (run4 replay from sec07_252827, 269300..271500: born at 269327, destroyed
# at 270999, gone at 271119).
# m6raft = the Mission 6 stage 1 rafts 0BD822 / 0BD836 / 0BD84A with their floats 0BD8F4 / 0BD906 / 0BD918 (the cell
# map 0BDA32 by 0140C0: the floor over the water), the raft pictures 0BDA4E and the decorations 0BD416..0BD4D4 / 0BDAB8
# of the camera list 0BCA00 (neo_m6props.c); captures-m6/fam/R (run4 replay from sec09_292683, 292684..297000),
# --range bd416-bd535 --range bd822-bdaeb --ram 1063d8-1063da --ram 10c33c-10c33e.
FAMILY_ARGS_m6raft := --terrain 10c72e --ring 10c72e --group-map 0 --root-words --reserve
# m6span = the Mission 6 stage 1 hanging bridge's actors (neo_m6props.c): the swing 0BD18E (10C712..10C714), the span
# makers 0BD2BA / 0BD2CE (lock bytes 10C70C / 10C70D) and the spans 0BCFA4 / 0BCFC4 / 0BCFE4 / 0BD004 (palette 4E1) on
# 10D7B6; captures-m6/fam/R, --range bcfa4-bd2e1 --ram 10d7b6-10d7ee --ram 10c70c-10c715 --ram 1063d8-1063da
# --ram 10c33c-10c33e.
FAMILY_ARGS_m6span := --terrain 10c72e --ring 10c72e --group-map 0 --root-words --reserve --m6bridge
# m6mid = the Mission 6 stage 1 mid-boss 05310E (camera 2352; neo_m6props.c): its four hit parts 0539C4..0539EE, the
# dust 053A48 / 053A5C, the death fade 05390C and the fall into the water 05439E with its splashes 053F0A / 053E40 /
# 0540EA / 0541F2 and 0542B0; captures-m6/fam/B (run4 replay from sec09_292683, 296800..303000; runB.sh), --range
# 52a00-546ef --attacks --ram 10c44c-10c44d --ram 10e992-10e994 --ram 1063d8-1063da --ram 10c33c-10c33e --ram 1014f0-1014f2
# --ram 1015a0-1015a2 (the players' +2C) --ram 10c7b6-10d7b6 (the terrain ring) --ram 10c2f6-10c2f8 (the stage, 019240).
FAMILY_ARGS_m6mid := --attacks --terrain 10c72e --ring 10c72e --group-map 0 --root-words --reserve
FAMILY_ARGS_m5mech := --attacks --terrain 10c72e --ring 10c72e --group-map 0 --root-words --reserve
# m5pow = the Mission 5 sewer crank 095C98 and its child 095F8C (neo_m5sewer.c; the item 08C374 it makes is outside the
# family); m5sewer = the sewer soldiers 089948 / 08995A / 08996A and their guns 08A264. captures-m5/fam/X (run4 replay from
# sec08_274387, 274388..276400); m5pow with the vehicle slots, input words and player nodes (0AA606 / 0C069E).
FAMILY_ARGS_m5pow := --attacks --terrain 10c72e --ring 10c72e --group-map 0 --root-words --reserve --item-kinds 3,6,52,53 --pickup-ram
FAMILY_ARGS_m5sewer := --attacks --terrain 10c72e --ring 10c72e --group-map 0 --root-words --reserve --tree-deletes 89948-8a264
FAMILY_ARGS_m5ledge := --attacks --terrain 10c72e --ring 10c72e --group-map 0 --root-words --reserve
FAMILY_ARGS_m5car := --attacks --terrain 10c72e --ring 10c72e --group-map 0 --root-words --reserve --fresh-flip --split-children --script-actor 101414 --script-routines 8c374
# m5deco = the Mission 5 sewer decorations 0BC4F4 (script records of the sewer's start,
# neo_m5props.c); captures-m5/fam/E (run4 replay from sec08_274387, to 275100).
FAMILY_ARGS_m5deco := --terrain 10c72e --group-map 0 --root-words
# m5street = the Mission 5 street's decorations 0BBE5E / 0BBED0 and its end 0BBE16 (the
# stage exit 0B6ED2 after 60 updates; neo_m5props.c, neo_goarrow.c); captures-m5/fam/S
# (run4 replay from sec06_247661, to 252800).
FAMILY_ARGS_m5street := --terrain 10c72e --group-map 0 --root-words
# m4van2 = the whole life of the van without its overlay (routines 0A9F0C..
# 0AA2DF only: the wreck 0AA176, the blast 0AA2C6, 0AA1FE / 0AA252). m4van
# (van and overlay, --range a9f0c-aa458 --hit-writes) runs to the end too: in
# 152049 the blast's 0049E8 flashes the van (+17) between the van's update
# and the overlay's 0044F6; extract_family_fixture.py --hit-writes records
# what 0049E8 wrote into the other nodes of the family and the test applies
# it before the next update (FAMILY_HITW).
FAMILY_ARGS_m4van2 := --attacks --terrain 10c72e --ring 10c72e --group-map 0 --root-words --reserve --script-actor 101414 --script-routines 8c374
# m4rock = the Mission 4 hatches 01F828 / 01F866 (four script records; neo_m4props.c):
# the emitter, the two halves of the hatch 01F9B8 / 01FA08 with their states
# by the distance to the last rock (emitter +82), the puffs 020004, fifteen
# rocks 01FABA with their shadows 01FF82 down to y 0x200; capture A, --range
# 1f828-2055f.
FAMILY_ARGS_m4rock := --attacks --terrain 10c72e --ring 10c72e --group-map 0 --root-words --reserve --node-fields 82
# m4jump = the Mission 4 jumping soldier 07E554 (four script records, 01A422;
# neo_m4props.c): it waits for the player within +72, jumps out, is shot in
# the air (07EA6C: the piece 07EC6A, the blast 01C706 or the dust 07EDF0 on
# water) and its dust 07EDF0; capture A, --range 7e554-7f000.
FAMILY_ARGS_m4jump := --attacks --terrain 10c72e --ring 10c72e --group-map 0 --root-words --reserve
# m4box = the Mission 4 hidden prisoner boxes 09A71E (three script records,
# neo_secret.c: 09A688's hits with the prisoner 099E1C, +7E = 0); in run4 they
# are not opened; capture A, --range 9a5ee-9a97b.
FAMILY_ARGS_m4box := --attacks --terrain 10c72e --ring 10c72e --group-map 0 --root-words --reserve
# m4heli = the Mission 4 helicopters (neo_heli.c): the background ones 08571C
# with their flash 085952, the attackers 085522 with the flash 0859E4 and the
# bomb 085ACE, the squadrons 08514A of 085294; capture A, --range
# 8514a-86070 --ram 101500-101508 (the player's +3C / +40 longs).
FAMILY_ARGS_m4heli := --attacks --terrain 10c72e --ring 10c72e --group-map 0 --root-words --reserve --script-actor 101414 --script-routines 85ace,8c374,8d0d2
# m4spark = the sparkle 08CF6E of the animals (item kinds 40..42, neo_item.c;
# 48 of them in capture A, --range 8cf6e-8d08d; made by the items, outside the family).
FAMILY_ARGS_m4spark := --terrain 10c72e --ring 10c72e --group-map 0 --root-words --reserve
# m4veh = the Mission 4 ground vehicle 0587C4 (neo_m4veh.c) with its turret
# 0592C6, muzzle flashes, shots 0594B0 / 0594CE and their trails; capture L
# (run4 with the pad idle from its creation: it lives and fires its bursts).
# m4veh2 = the same in capture A (run4's pilot destroys it: 059048 / 0590B8,
# the turret's end 0593B6, the piece 059B5C).
# m4veh3 = capture N (run4's pad without fire, MSX_FULL_NOFIRE: the pilot walks
# on, the body is deleted outside 0586C2 and the turret ends by 0593B6 with
# its creator gone). m4veh4 = capture N55 (as L with MSX_FULL_HOP: the idle
# player jumps into the shots; their end 0596DE and the blast 059852).
# m4boss = the whole Mission 4 boss 02267E (neo_m4boss.c: body, guns, cannon, treads, shells, balls, debris) in capture T.
FAMILY_ARGS_m4boss := --root-words --attacks --m4boss --ram-creators --terrain 10c72e --ring 10c72e --group-map 0
FAMILY_ARGS_m4veh := --attacks --terrain 10c72e --ring 10c72e --group-map 0 --root-words --reserve --script-actor 101414 --script-routines 59b5c,59852,1c6ee
FAMILY_ARGS_m4veh2 := --attacks --terrain 10c72e --ring 10c72e --group-map 0 --root-words --reserve --script-actor 101414 --script-routines 59b5c,59852,1c6ee
FAMILY_ARGS_m4veh3 := --attacks --terrain 10c72e --ring 10c72e --group-map 0 --root-words --reserve --script-actor 101414 --script-routines 59b5c,59852,1c6ee
FAMILY_ARGS_m4veh4 := --attacks --terrain 10c72e --ring 10c72e --group-map 0 --root-words --reserve --script-actor 101414 --script-routines 59b5c,59852,1c6ee
# m4truck = the Mission 4 truck 02916E (neo_truck.c, record of camera 4929)
# with its body 029802 and the pieces 02A448 (02A71E with the reserve 2A:
# two of sixteen; 01D5AA); capture G (run4's pad from sec05, roots
# 2916e,2a448, --range 2916e-2aa00, 156900..157600).
FAMILY_ARGS_m4truck := --attacks --terrain 10c72e --ring 10c72e --group-map 0 --root-words --reserve
# m4sold / m4sold2 = the leaping soldiers 0AF1A8 (+81 = E0: the actions
# 0AF99A of neo_soldier_ai.c) as nodes of a family trace (--soldier-nodes:
# neo_soldier_construct / neo_soldier_tick on the node's bytes, the fields
# the soldier module models): m4sold = capture A (six of them, 153471..155461,
# shot in the air), m4sold2 = capture I (run4's pad from sec05 with
# MSX_FULL_IDLE=153480-154400, roots af1a8: the leap reaches the idle
# player, 0AFADA / 0AFAF0); --range af000-b0fff --ram 1067e6-1068e6 (the
# delay grid) besides the Mission 4 set.
FAMILY_ARGS_m4sold := --attacks --terrain 10c72e --ring 10c72e --group-map 0 --root-words --reserve --soldier-nodes
# m4pris = the fighting prisoner 0DDCDA.. (099E06 with +74 = FFFE, neo_prisoner.c) with its shots 0DE0FE and
# blasts 0DE24A; capture E (run4's pad from sec05, roots 99e06,de0fe,de24a, range ddcda-de4ff,
# 152700..157600): --range 99e06-9a97b --range ddcda-de4d8 and the node pool as RAM (--ram 1010a4-105564:
# the player's node +76 and the enemy's +6E are read there, e->node).
FAMILY_ARGS_m4pris := --attacks --terrain 10c72e --ring 10c72e --group-map 0 --root-words --reserve --script-actor 101414 --script-routines 8c374 --ram-creators
# m4pris2 = the same from capture J (the pad idle from 153720: the prisoner stands and shoots, its shots home
# on live soldiers, 0DE18E 168 updates).
FAMILY_ARGS_m4pris2 := --attacks --terrain 10c72e --ring 10c72e --group-map 0 --root-words --reserve --script-actor 101414 --script-routines 8c374 --ram-creators
# m4animal = the animals of Mission 4 (item kinds 40..42, motion 08BAAA, neo_item.c) with the fruits they drop
# (kinds 3D / 3F, motions 08BB3C / 08BB44, 2896B2) and their sparkles 08CF6E; capture F (run4's pad from
# sec05, roots 8c374,8c37c,8cf6e, range 8c374-8d900, 151000..155900).
FAMILY_ARGS_m4animal := --terrain 10c72e --ring 10c72e --group-map 0 --root-words --reserve --script-actor 101414 --script-routines 8c374,8c37c,8cf6e --script-kinds 3d,3f --item-kinds 3d,3f,40,41,42 --pickup-ram
# m4label = the pickup label 08C752 (08CD10) with its characters 08C954 (neo_item.c): five labels of capture M
# (run4's pad from sec05, roots 8c752,8c954, range 8c752-8cf6d, 152900..153800); the items that make them are
# outside the family (injected), RAM 10C5B2 / 10C2F6 as read.
FAMILY_ARGS_m4label := --terrain 10c72e --ring 10c72e --group-map 0 --root-words --reserve --label-counts
FAMILY_ARGS_m4sold2 := --attacks --terrain 10c72e --ring 10c72e --group-map 0 --root-words --reserve --soldier-nodes
# m3wagon = the Mission 3 wagon boxes 039382/03938A and wagons 038D1E/038D26
# (doors 038B06, targets 038F38, pieces 0390A6) with their ledge stampers
# 0B90BC..0B9140 -> 0B8FA8 (0C70FC into the ring of 10C72E), neo_m3props.c;
# wagon from states4/sec04_117966 (family 120900..122400, docs/MISION3.md
# "Paso 1"). No group map (RAM 10C00E = 0).
FAMILY_ARGS_m3wagon := --attacks --terrain 10c72e --ring 10c72e --group-map 0 --train
# m3platform = the Mission 3 platform 0B9286 over the gap (camera-list call
# 0B925C, dynamic map 0E6896 through 0140C0), its piece 0B93B4 and the
# script record 0B9400 (10C6BD), neo_m3props.c; platform from
# states4/sec04_117966 (family 126200..141300). Its bonus 0B9748 is outside.
FAMILY_ARGS_m3platform := --attacks --terrain 10c72e --ring 10c72e --group-map 0 --train
# m3grenade = the Mission 3 grenadier 08A732 (script record 2DA6FA, camera
# 1712), its helmet 08AA6E and its death through the soldier's 0406B6 on the
# generic node (neo_m3enemy.c); capture A from states4/sec04_117966 (family
# 118600..123850, docs/MISION3.md "Paso 2"), the grenadier's tree only.
FAMILY_ARGS_m3grenade := --attacks --terrain 10c72e --ring 10c72e --group-map 0 --train
# m3grenade4 = the four grenadiers 08A732 of section 4 (records 2DB2xx, camera
# 5508: throws, shells 08AC54, flashes 08AB70, smoke 08AE70), capture B from
# states4/sec04_117966 (family 141000..144300).
FAMILY_ARGS_m3grenade4 := --attacks --terrain 10c72e --ring 10c72e --group-map 0 --train
# m3shield = the Mission 3 shield soldiers 09AE2C / 09AE1E / 09BA2A (records of
# sections 1 and 2), their shield 09C46C (+72), overlay 09C6B0, shots 09C744..
# and flashes 09CAE8.., and the death through 0406B6 (neo_m3enemy.c); capture
# A (family 118600..123850), the soldiers' trees only.
FAMILY_ARGS_m3shield := --attacks --terrain 10c72e --ring 10c72e --group-map 0 --train --node-fields 72 --root-words
# m3shield4 = the three shield soldiers 09BA2A of section 4, capture B.
FAMILY_ARGS_m3shield4 := --attacks --terrain 10c72e --ring 10c72e --group-map 0 --train --node-fields 72 --root-words
# m3window = the Mission 3 window soldiers 08DA12 of section 2 (jump out,
# land, the delay grid 1067E6 through 00510C -> 0B05FE: the node becomes a
# soldier, its soldier updates left out), capture A.
FAMILY_ARGS_m3window := --attacks --terrain 10c72e --ring 10c72e --group-map 0 --train
# m3para = the Mission 3 soldiers 05D916 (parachutists: canopy 05DDA4, shots
# 05DF3E/05DFC2, landing -> 05C5B4), 05D226 and 05C53C of section 2 with their
# helmets 05DD2A and deaths (0406B6), capture A.
FAMILY_ARGS_m3para := --attacks --terrain 10c72e --ring 10c72e --group-map 0 --train --root-words
# m3pod = the Mission 3 flying machines 04E990 of section 2 (pilot 04EF88, floor
# 04EE88 with the dynamic map 0E112E, missiles 04F44E/04F464 on the path 0E1168,
# bombs 04F358, smoke 04F720, pieces 04EC8C, explosions), capture A.
FAMILY_ARGS_m3pod := --attacks --terrain 10c72e --ring 10c72e --group-map 0 --train --root-words --split-children
# m3mid<n> = the Mission 3 mid-boss aircraft 0A4A94 (neo_m3mid.c) in pieces
# of one fight (tools/capture_m3_boss.sh mid, run4 from states4/sec04_117966,
# MSX_FULL_FAMILY_RANGE=a4a94-a6ce9): body and its layer object 10E8C6, side
# pieces, guns, parts, debris, engines, hatch, bay, soldiers, bombs, death
# (docs/JEFE_MISION3.md, "Jefe intermedio").
M3MID_ARGS := --m3mid --late-creator --attacks --terrain 10c72e --ring 10c72e --group-map 0 --train --node-fields 6e --script-actor 101414
FAMILY_ARGS_m3mid1 := $(M3MID_ARGS)
FAMILY_ARGS_m3mid2 := $(M3MID_ARGS)
FAMILY_ARGS_m3mid3 := $(M3MID_ARGS)
FAMILY_ARGS_m3mid4 := $(M3MID_ARGS)
# m3boss<n> = the Mission 3 final boss 04A2C2 (neo_m3boss.c) in pieces of
# one fight (tools/capture_m3_boss.sh boss from at_144400, family range
# 4a2c2-4e577; docs/JEFE_MISION3.md): m3boss0 = 144410..144886 (arrival,
# stop, footsteps, crouch; the pieces waiting), m3boss1 = 144410..145600
# (the flamethrower, order 0: flames 04B79C/04B87E, fireballs 04CAB4 with
# their landing on 10C72E by 0143E4 in 145025, trails, puddles 04CCE4, the
# mortar; extracted with --ram 10c72e-10d7b6 too: the ring the sweep reads), m3boss3 = 146600..147600 of the full trace spr (the missile pod,
# order 2: missiles 04C194; the flamethrower at the player, order 1).
# m3boss2 = 146200..146560 of spr (the hatch, order 7: shots 04C7B6/04C840),
# m3boss4 = 145160..145600 of spr (the mortar, order 5: 4 volleys of bombs
# 04D1DA), m3boss5 = 148541..149200 of spr (the death 04A9D4, 018F92, the
# drag, 018F40 and the deletion at 149169; extracted with --range
# 1d86c-1d93b --range 1d93d-1da3f: the debris 01D3D2 becomes smoke 01D93C).
M3BOSS_ARGS := --m2boss --late-creator --attacks --terrain 10c72e --ring 10c72e --group-map 0 --train --node-fields 6e
FAMILY_ARGS_m3boss0 := $(M3BOSS_ARGS)
FAMILY_ARGS_m3boss1 := $(M3BOSS_ARGS)
FAMILY_ARGS_m3boss3 := $(M3BOSS_ARGS) --resume
FAMILY_ARGS_m3boss5 := $(M3BOSS_ARGS) --resume
FAMILY_ARGS_m3boss4 := $(M3BOSS_ARGS) --resume
FAMILY_ARGS_m3boss2 := $(M3BOSS_ARGS) --resume
# m3boss6 = the boss with a rider in the SV-001 093C9A (G15; capture_m3_boss.sh ride,
# 140360..142500 from prog_140352): the vehicle tables of 04DA38, order 6 of the pod
# (04BAB2 aim, 04BB88 open, sweeps 04BCA2..04BD32 with the missiles 04C220, 04BF66).
FAMILY_ARGS_m3boss6 := $(M3BOSS_ARGS)
# m3gun = the Mission 3 gunships 097FCC of section 2 (rotor 098886, hurtbox
# 098E4A, exhaust 01D86C, death debris), capture A; m3gun4 = the three ships
# of section 4 (bombs 09879C, missiles 098974 after the script actor, smoke,
# pieces 098C0C/098CB6/098E18), capture B.
M3GUN_ARGS := --attacks --terrain 10c72e --ring 10c72e --group-map 0 --train --root-words --fresh-program --reserve --script-actor 101414 --script-routines 98cb6,98e18,98974
FAMILY_ARGS_m3gun := $(M3GUN_ARGS)
FAMILY_ARGS_m3gun4 := $(M3GUN_ARGS)
# m3thrower = the Mission 3 molotov throwers 0569B4/0569D0/0569E6 of section 4
# (hurtbox 056E5A, shield 056D76, molotovs 056B00, helmets), capture B.
FAMILY_ARGS_m3thrower := --attacks --terrain 10c72e --ring 10c72e --group-map 0 --train --root-words --reserve
# m3bomber = the Mission 3 flying bomber 084588 of section 4 (rotor 083B30, bomb
# 08471A with its trail, helmet 084948, debris 08497E), capture B; its creator
# is a script runner (injected root).
FAMILY_ARGS_m3bomber := --attacks --terrain 10c72e --ring 10c72e --group-map 0 --train --root-words --reserve
# m3drum = the Mission 3 oil drums 0391EA of section 2 (hit kind 13 -> fall,
# four smokes 08AE76), capture A (second capture, captures-m3s2c).
FAMILY_ARGS_m3drum := --attacks --terrain 10c72e --ring 10c72e --group-map 0 --train --root-words --reserve
# m3truck = the Mission 3 armed trucks 02B202 of section 2 (gunner 02B61C,
# tyres 02BBBA), capture A (captures-m3s2c).
FAMILY_ARGS_m3truck := --attacks --terrain 10c72e --ring 10c72e --group-map 0 --train --root-words --reserve
# m3raft = the Mission 3 floating platform 08AFB6 of section 2 with its floor
# 08B3E6 (dynamic map), capture A (captures-m3s2c).
FAMILY_ARGS_m3raft := --attacks --terrain 10c72e --ring 10c72e --group-map 0 --train --root-words --reserve
# m3car = the Mission 3 rider car 027F54 of section 2 (idle at camera 1264, its
# flinch, death and the rider 0287FA, who becomes a soldier at 028948: the
# soldier's own updates and children are another test's), capture A
# (captures-m3s2c).
# m3tank = the Mission 3 SV-001 093C9A (docs/PLAN_MISION3.md step 5: a separate
# copy of the SV-001 code, neo_tank.c v3), its tread 014A4E, turret 03B4E0,
# marker 03523C, shells, flashes, entries, dust and burst 094E2A, with the
# rider (node 1014C0) from the vehicle slots: capture from at_132300 with
# MSX_FULL_RIDE=tank (family 137300..139700: born 137396, mounted 137578,
# driven both ways with the vulcan, aimed, jumps, cannon, crouch, A+B out
# 138479, blast 138546).
FAMILY_ARGS_m3tank := --script-actor 101414 --script-routines 1b6a0 --node-fields 6e,8c --shared 14a4e --attacks --terrain 10c72e --ring 10c72e --group-map 0 --train
# m3tankair = B59 on the Mission 3 copy 093C9A (MAME b59m3: at_132300, ride plan
# 'dmg', VEHICLE_CHEAT=0, FAMILY_ROOTS=93c9a,ae1dc,ad9b8, FROM=137300, LAST=138700):
# hits in the air 0946FA -> 094788 -> 0947CC from behind (137880) and the front
# (138123), the destruction 094ABA at 138361; the solid bodies' nodes as --ram.
FAMILY_ARGS_m3tankair := $(FAMILY_ARGS_m3tank)
# m3veh = the Mission 3 vehicle 0AC866 (neo_m3veh.c), its pilot, wings,
# flames, bombs and trails, the turret 03B4E0, the markers 03523C/03533A
# and the rider (node 1014C4) from the vehicle slots: capture from at_132300
# with MSX_FULL_RIDE=tank MSX_FULL_RIDE_RANGE=ac866-ad2ab
# MSX_FULL_RIDE_SCREEN=8 (family 137400..140000: born 137446, boarded
# 137599, flown, bombs, landed, out 138494, the empty seat's end).
FAMILY_ARGS_m3veh := --script-actor 101414 --script-routines 1b6a0,ad856,ad9b8,ad8a2,ae1dc,ada16,ad5d4,ad58c,1c71e,59c82 --node-fields 6e --attacks --terrain 10c72e --ring 10c72e --group-map 0 --train
# m3veh2 = the same vehicle with MSX_FULL_RIDE=flyer (family 137400..139600:
# boarded 137599, bombs, landed (0ACD76), down+B out 137931 (0ACE44),
# boarded again 138045, flown until it is destroyed with its rider).
FAMILY_ARGS_m3veh2 := $(FAMILY_ARGS_m3veh)
# m3wepe = the player's weapon E 0DE4DA (neo_pweapon.c, table 0C42AA id 0E)
# and its burst 0DE8EE: from at_132300 with the usual plan (family
# 136400..137400, the player holds weapon E in section 4).
FAMILY_ARGS_m3wepe := --script-actor 101414 --script-routines de8ee,de92e --attacks --terrain 10c72e --ring 10c72e --group-map 0 --train
# m3fat = the weapons with +93 = 2 (fat Marco, table 0C42EA / 0C476E):
# the R's rocket 063AEE (063BC0/063D60, neo_chaser.c), its trails 063D9E,
# the grenade 01BEDA (neo_bullet.c) and its ripples 03C62A: from
# sec04_117966 with the usual plan, family 119100..120400 (ROOTS
# 63aee,63f10,1beda,1c362,c4e86,c5064,1b62c,1b668,1bdd2, node 1014C4;
# extract_family_fixture.py --range 63600-64200 --range 1beda-1c200
# --range 3c600-3c700, the m3wepe --ram list).
FAMILY_ARGS_m3fat := --reserve --script-actor 101414 --script-routines 6371e --attacks --previous --hit-to 1c152 --terrain 10c72e --ring 10c72e --group-map 0 --train
# m3fat2 = the same weapons later in the capture (family 122550..123300):
# the C 063F10 (neo_chaser.c, 063FDC as 063F32), R rockets and grenades
# (--range 63600-64200 --range 1beda-1c200: the ripples 03C62A stay outside
# the family, their water probe 013CF8 under the train is not modelled).
FAMILY_ARGS_m3fat2 := $(FAMILY_ARGS_m3fat)
# m3bomb = the fat bomb type 3 01C362 (neo_bullet.c, table 0C476E), its pieces
# 01C654 and ripples 03C62A, and the fat landing dust 0C4E86 (0C1ED6): from
# sec04_117966 with the usual plan (run4), family 124100..124700 (ROOTS
# 1c362,1c654,c4e86,c5064, node 1014C4; extract_family_fixture.py --range
# 1c300-1c700 --range c4e00-c5100 --range 3c600-3c700, the m3fat --ram list
# and 1014e2-1014e4: the player's +1E the dust reads).
FAMILY_ARGS_m3bomb := --reserve --script-actor 101414 --script-routines 1c654 --attacks --terrain 10c72e --ring 10c72e --group-map 0 --train
# m3molotov = the fat molotov 01E680 (bomb type 2 with +93 = 2, table 0C476E;
# neo_bullet.c), its fire 01E80E/01E894 and the type-3 flames 01EA7A..01EB72:
# from sec04_117966 with the usual plan and MSX_FULL_GIVE_BOMB=124740:2 (the
# fat player gets bomb type 2, as an item of kind 09), family 124750..125350
# (ROOTS 1e680,1e69c, node 1014C4; --range 1e4f0-1ec00 --range 1c7ea-1c7f0
# --range 1c9d2-1ca96, the m3fat --ram list with 10c718-10c730).
FAMILY_ARGS_m3molotov := --reserve --attacks --terrain 10c72e --ring 10c72e --group-map 0 --train
# m3splash = the splash 03C962 (neo_gate.c) of the player's pistol shots
# 01B782 fired down into the river from the bridge (01B8AE -> 03C5C6): from
# the state at_122200 (run4 replayed from sec04_117966, MSX_FULL_SAVE_AT)
# with MSX_FULL_PLAN=dive:bb2, MSX_FULL_GIVE_FORM=122505:0 (thin again) and
# MSX_FULL_GIVE_WEAPON=122510:0 (the pistol), family 122500..123100 (ROOTS
# 3c962,c5064,c1d82, RANGE 1b600-1bd00; --range 1b600-1bd00 --range
# 3c900-3ca80, the m3fat --ram list).
FAMILY_ARGS_m3splash := --reserve --script-actor 101414 --script-routines 3c962 --attacks --terrain 10c72e --ring 10c72e --group-map 0 --train
FAMILY_ARGS_m3car := --attacks --terrain 10c72e --ring 10c72e --group-map 0 --train --root-words --reserve --soldier 28948
# m3jeep = the Mission 3 armed jeep 0A2F2E of section 2 (idle at camera 3296: bursts
# of shells 0A3C24, the gunner 0A38F4 killed, the death with the rider 0A3A54 of
# the turret 0A3A0A; the children by +76 / +7A), capture A (captures-m3s2c).
FAMILY_ARGS_m3jeep := --attacks --terrain 10c72e --ring 10c72e --group-map 0 --train --root-words --reserve --node-fields 76,7a --external-state a35a8,a3a54,a3ba4
# m3bird = two Mission 3 birds 03EE8A of section 2 (idle, hops, flights), m3sack = the
# sandbags 0214EA at 1264 (pieces 0218C4, the item), m3camp = the camp 01F1D4 at
# 1264 (two soldiers, the stove 01F562 and its pot 01F4DA; a soldier leaves by the
# state 0B05FE), capture A (captures-m3s2c).
FAMILY_ARGS_m3bird := --attacks --terrain 10c72e --ring 10c72e --group-map 0 --train --root-words --reserve
FAMILY_ARGS_m3sack := --attacks --terrain 10c72e --ring 10c72e --group-map 0 --train --root-words --reserve
FAMILY_ARGS_m3camp := --attacks --terrain 10c72e --ring 10c72e --group-map 0 --train --root-words --reserve --soldier b05fe
# m3bonus = the bonus sensor 0B9748 of the pit platform 0B9286 (neo_m3props.c), capture
# D (captures-m3s2d): the players' +15 from RAM 1014D9 / 101589 (--ram).
FAMILY_ARGS_m3bonus := --attacks --terrain 10c72e --ring 10c72e --group-map 0 --train --root-words --reserve
# robeflag = the boss-arena robed swordsman 0A8C46 surrendering after the
# Mission 1 boss (0A8FE0 +72 != 0 -> 0A9010, white flag 2A74E0, hurtbox
# 0E64EC) until the mission change (m1end/flag3 from states3 sec00_2100,
# family 19200..20200; neo_rest.c, JEFE_MISION1.md B46).
# The whole Mission 2 boss fight (8 pieces, ~30 min): `make check-m2boss`
# (or FULL_M2BOSS=1 make check). make check runs the first and last piece.
M2BOSS_ALL := m2boss1 m2boss2 m2boss3 m2boss4 m2boss5 m2boss6 m2boss7 m2boss8
ifeq ($(FULL_M2BOSS),1)
FAMILIES := $(filter-out $(M2BOSS_ALL),$(FAMILIES)) $(M2BOSS_ALL)
endif
# The boss's three prisoners 099DCC (hatch 0D7402 in phase 2) from the two
# boss fights (captures-boss runJ 11957..12644, runDbg2 11249..11902): they
# fall from y 96 through the body platform 0E94AC, land on the arena floor
# (layer 10D7B6), are freed, rescued and run off left (view 10E9D2 = -88).
# The camel riders 09F682 (camel2: hold-fire 6950..7950 at camera 2184..2336),
# their riders, heads, shots, pieces and the dropped sword 09EE34; the rider
# dies as a swordsman (09E920/09EAB8, neo_swords.c). A shot A0034 that hits
# the player (0049E8) explodes in place (A00E8) after its move: --hit-to with
# --hit-moving.
FAMILY_ARGS_camel := --hit-to a00e8 --hit-moving
# The village decoration 0B6298 (unc1: camera 336 -> 968, cull 0B6948) and the
# market stall's awning 0400AA (camera 192, its collision stamp 0E0932; the
# village terrain object 10C72E).
FAMILY_ARGS_awning := --terrain 10c72e
# The porter 03264E (unc1, camera 440): its walker 0327EC hands over an item
# 08C374 kind 0x30 (taken at 2836: points) and keeps it in +72/+76.
# The Camel Slug 03458E (camelfam 9400..10095: waits, mounted at 9530, rides,
# fires the vulcan and bombs, the rider killed at 10091) with its turret
# 03B4E0, rider graphic 034F1A, effects 034FD2, marker 03523C, dust 059E30
# and the bullets 01BE24 (00411C after the script actor) with their impacts;
# the slots, input words, weapon block and player nodes it reads and writes
# (--ram of the extraction) are compared byte for byte.
FAMILY_ARGS_slug := --script-actor 101414 --script-routines 1be24,59e30 --node-fields 6e
FAMILY_ARGS_tank := --script-actor 101414 --script-routines 1b6a0 --node-fields 6e,8c --shared 14a4e --attacks --stamp a480000,400000,b69f8 --stamp a480000,400000,b6a2c --stamp a480000,400000,b6a12 --stamp a480000,400000,b6a42
# tankhit / tankkill = the SV-001 hit on the ground (09351A -> 093576) and destroyed
# with its rider (0938DA -> 09391E -> 093AFA), B52: MAME at_9000 (b51b states),
# MSX_FULL_RIDE=sit, VEHICLE_CHEAT=0, FAMILY_FROM=9100 (the hit of 11351; tankkill
# with MSX_FULL_VEHICLE_HEALTH=11340:1), ranges and RAM of the tank family.
FAMILY_ARGS_tankhit := $(FAMILY_ARGS_tank)
FAMILY_ARGS_tankkill := $(FAMILY_ARGS_tank)
# tankair = B59: a full damage sequence while riding (MAME b59a: at_9000, ride plan
# 'dmg' with MSX_FULL_RIDE_SCREEN=8, VEHICLE_CHEAT=0, FAMILY_ROOTS=92a32,ae1dc,ad9b8,
# FROM=9100, LAST=10900): two rocket hits in the air (0935A8 -> 0935EC -> 093576,
# 9718 and 10149) and the third on the ground with health 0 (0938DA, 093AFA at 10739).
FAMILY_ARGS_tankair := $(FAMILY_ARGS_tank)
# figure = the burning figures 08732C of the destroyed shop 0B6566 (B51, MAME b51f:
# sec00_2100, FAMILY_ROOTS=8732c FROM=8600, the shop falls at 8660): they land
# on the ground the shop raised and cleared (its roof 0B680E stamps).
FAMILY_ARGS_figure := --stamp a480000,400000,b69f8 --stamp a480000,400000,b6a2c --stamp a480000,400000,b6a12 --stamp a480000,400000,b6a42
FAMILY_ARGS_tankpush := --script-actor 101414 --script-routines 1b6a0 --node-fields 6e,8c --shared 14a4e --attacks --stamp a480000,400000,b69f8 --stamp a480000,400000,b6a2c --stamp a480000,400000,b6a12 --stamp a480000,400000,b6a42
FAMILY_ARGS_tankcrouch := --script-actor 101414 --script-routines 1b6a0,1bf10 --node-fields 6e,8c --shared 14a4e --attacks --stamp a480000,400000,b69f8 --stamp a480000,400000,b6a2c --stamp a480000,400000,b6a12 --stamp a480000,400000,b6a42
FAMILY_ARGS_tankjump := --script-actor 101414 --script-routines 1b6a0 --node-fields 6e,8c --shared 14a4e --attacks --stamp a480000,400000,b69f8 --stamp a480000,400000,b6a2c --stamp a480000,400000,b6a12 --stamp a480000,400000,b6a42
# The village civilians 031D92 (civ1: camera 1984, one figure 031F52 that
# stands, is hit at 7064 and flees with the scream 104F); the figures keep
# the leader in +7A.
FAMILY_ARGS_civil := --node-fields 7a
# Explosion effects (src/neo_effects.c): fragments 01D31C.. and debris
# 01CE08.. of fam2 (wagon, rocket launcher, helicopters; 8414..10599), the
# shop's debris and puffs 01D488 of bld1 (cut before 7312: the shop's
# collision stamps are not replayed) and its flashes 01E058 (bld1).
FAMILY_ARGS_porter := --terrain 10c72e --node-fields 72,76 --pickup 2836,0,ffff
# The rocket launcher 05ED84 (rl2: hold-fire capture at camera 2688), its aim
# flash 060490 (0040BC) and, after the script actor (00411C), the rockets
# 05FD8A, muzzles 0602E4, trail puffs 059C82 and the death smoke 0897E2.
FAMILY_ARGS_rocket := --script-actor 101414
# The helicopter formations 04857C (heli2: both script records through the
# capture-only ROM patch, hold-fire 8600..9900), their leaders, helicopters,
# rotors, droppers and bombs; the shells 047DE0 after the script actor
# (00411C). A bomb is created by the dropper's callback on the update whose
# program ends (the dropper stays a placeholder): --late-creator.
# A bomb that hits a player (0049E8) explodes in place (047C48) without
# being deleted: --hit-to with --hit-moving.
FAMILY_ARGS_heli := --script-actor 101414 --script-routines 47de0 --late-creator --hit-to 47c48 --hit-moving
# Mission 2, capture_fullgame.lua from the stage 1 state (sec01_19391):
# goarrow = the GO arrow 05287C with an idle player (tr2: MSX_FULL_IDLE=
# 19430-20400, 176 updates of the arrow); the camera stop 10E9BC, progress
# 10E99E..10E9A1, halt bytes 10C300/10C301 and the arrow direction 10C44A
# as read (--ram). exitmark = the stage-1 exit marker 0B6F6A (tr1).
# rebel2 = the Mission 2 rebels 0613CA/0613D8/0613E6 (neo_rebel2.c) of
# stage 2 (capture_fullgame.lua from sec02_19932, frames 22600..26139), on
# the stage-2 grid (private, docs/MISION2_ACTORES.md).
FAMILY_ARGS_rebel2 := --attacks --terrain 10c72e --ring 10c72e --group-map b7000
# stageexit = the stage-1 exit 0B6ED2/0B6F24 (neo_goarrow.c, tr1 from
# sec01_19391: 1 trigger update, 83 rectangle tests, 018F40 in the last).
# rebel3 = the Mission 2 soldiers 0678B6.. and 0692B2.. (neo_rebel3.c) of
# stage 2 (capture_fullgame.lua from sec02_19932, frames 20500..26139).
FAMILY_ARGS_rebel3 := --attacks --terrain 10c72e --ring 10c72e --group-map b7000
# gate = the Mission 2 stage-1 gate 03882A and its water ripple 03C62A
# (neo_gate.c, g1 from sec01_19391: shot open at 19740, 0B6E06).
FAMILY_ARGS_gate := --attacks --terrain 10c72e --ring 10c72e
# m2ripple = the water ripples 03C62A (neo_gate.c) of the Mission 2 stage-1
# river, with the creators the port missed: the player 1014C4 (0C124C, at
# the stage start; it outlives the death at 19621) and the swordsmen 039590
# (039E5A, neo_arab.c), plus the gate's and the prisoner's (capture_fullgame.lua
# from sec01_19391, MSX_FULL_PLAN=hop, ROOTS 3c62a,39590,395b6, family
# 19392..20400; extract_family_fixture.py --range 3c62a-3c961 --range
# 39590-3a0ff --range a9400-a98ff --range 35600-35a00 --attacks, the gate's
# --ram list and --ram 1010a4-104924: the creators outside the family are
# read from the node pool, --ram-creators).
FAMILY_ARGS_m2ripple := --attacks --terrain 10c72e --ring 10c72e --ram-creators
# secret2 = the Mission 2 stage-1 hidden boxes 09A692 (camera 320) and the
# prisoners 099E1C they release (09A842; neo_secret.c, neo_prisoner.c), pr1
# from sec01_19391 (19393..22600: three boxes shot open, three prisoners).
FAMILY_ARGS_secret2 := --attacks --terrain 10c72e --ring 10c72e
# crate = the Mission 2 stage-3 supply crate 08C168 (neo_crate.c, c1 from
# sec03_26140: two crates, opened at 28452 and 29708).
FAMILY_ARGS_crate := --attacks --terrain 10c72e --ring 10c72e
# bags = the Mission 2 stage-2 prop 06851A with its overlay 0685FE and the
# signal 03A892 (neo_m2props.c, s2 from sec02_19932, frames 19933..26139).
FAMILY_ARGS_bags := --attacks --terrain 10c72e --ring 10c72e --group-map b7000
# m2gate = the Mission 2 stage-2 gate 03A6EA (map command 0B7326), its
# overlay 03A82E, the opening 03A776 (0B7350: map patch 0B706C, exit
# 0B6ED2/0B6F24), the dust 03A464 and the signal 03A892 (neo_m2props.c,
# g2 from sec02_19932, frames 19933..26139: opened at 26001).
FAMILY_ARGS_m2gate := --attacks --terrain 10c72e --ring 10c72e --group-map b7000
# stage3 = the Mission 2 stage-3 camera wait 052A5C (neo_m2props.c) and the
# soldier 0678A2 (+9C = 4, neo_rebel3.c), c1 from sec03_26140.
FAMILY_ARGS_stage3 := --attacks --terrain 10c72e --ring 10c72e --group-map b7000
# drone = the Mission 2 stage-3 drone 03670E and its bomb 0369AC
# (neo_drone.c), c1 from sec03_26140: five drones.
FAMILY_ARGS_drone := --attacks --terrain 10c72e --ring 10c72e --group-map b7000
# m2creature = the Mission 2 stage-3 creature 080170 (neo_m2creature.c) thrown
# by the soldiers 0678B6 of +9C = 8 (callback 06899A -> 0689D2 at 067E98),
# its egg 0808AA and pieces 0805D4/080634/08070C: at_38090 with ROOTS
# 80170,808e0,67e98 (frames 38091..38700, B54, docs/MISION2_ACTORES.md).
FAMILY_ARGS_m2creature := --attacks --terrain 10c72e --ring 10c72e --group-map b7000 --resume
# hostage = the Mission 2 stage-2 soldier 06835C holding a hostage: the
# node becomes the hostage 021B46.. and its holder is 0683FA (neo_hostage.c,
# neo_rebel3.c), h1 from sec02_19932, frames 20590..20960.
FAMILY_ARGS_hostage := --node-fields 86 --attacks --terrain 10c72e --ring 10c72e --group-map b7000
# miner = the Mission 2 stage-2 miners 021A1C/021A26 (script records of
# section 2, uncounted), their dust 021F96 and sparkle 02206E, the item gift
# (neo_hostage.c): s2b from sec02_19932 with the player idle next to the
# miner of 2D7598 (MSX_FULL_IDLE=20158-20700); minerrun: s2a, the pilot
# running through the stage start (frames 19933..22800).
FAMILY_ARGS_miner := --attacks --terrain 10c72e --ring 10c72e --group-map b7000
FAMILY_ARGS_minerrun := --attacks --terrain 10c72e --ring 10c72e --group-map b7000
# signs = the Mission 2 stage-2 signs 0B829C (collision stamp) and 0B833A
# (wiggles with a player near), neo_m2props.c, s2a; prisoner3 = the tied
# prisoner 099DE2 (no 01B1DC, +86 bit 0) of the stage-2 random record
# 2D778E.., neo_prisoner.c, s2a.
FAMILY_ARGS_signs := --attacks --terrain 10c72e --ring 10c72e --group-map b7000
FAMILY_ARGS_prisoner3 := --attacks --terrain 10c72e --ring 10c72e --group-map b7000
# prisoner4 = the stage-2 prisoners hung from a rope 099DF0 and the rope
# pieces 09A9C4 (011FF6 draws the prisoner's +7A), neo_prisoner.c, p3 from
# sec02_19932 (MSX_FULL_FAMILY_ROOTS=99df0, frames 22900..24400).
FAMILY_ARGS_prisoner4 := --attacks --terrain 10c72e --ring 10c72e --group-map b7000
# bats = the Mission 2 stage-2 bats 035B66 (swarm of 035BF8) and 035C68
# (waypoints), their death, fall and crawl (neo_drone.c), s2a.
FAMILY_ARGS_bats := --attacks --terrain 10c72e --ring 10c72e --group-map b7000
# treasure = the Mission 2 stage-3 hidden treasure 0995EC (neo_m2traps.c):
# hit, opened, taken by the player (50000 points, 08CECC) and turned into
# the explosion 01C7AE; gem from sec03_26140 (frames 26141..27500).
FAMILY_ARGS_treasure := --attacks --terrain 10c72e --ring 10c72e --group-map b7000
# vent = the Mission 2 stage-3 gas vent 03A05A and its puffs 03A1AE (the
# mummies' breath, kind 0x10; neo_m2traps.c), vent from at_57300 (frames
# 57301..58700).
FAMILY_ARGS_vent := --attacks --terrain 10c72e --ring 10c72e --group-map b7000
# wrapped = the Mission 2 stage-3 soldier wrapped as a mummy 032DE2: lying
# (any hit restores it) until the mummies' breath (+5A = 0x10) wakes it, the
# flash 021CB6, 068E94 and the soldier 067AB4 (neo_m2traps.c, neo_rebel3.c);
# wrap2 from sec03_26140 (family frames 31481..37200).
FAMILY_ARGS_wrapped := --attacks --terrain 10c72e --ring 10c72e --group-map b7000
# rope = the Mission 2 stage-3 soldier hanging from a rope 082BCE and its
# knives 0830FE (neo_m2traps.c): rope from at_37120 (frames 37121..40000,
# the player kept below it: descent, waits, turns, seven knives).
FAMILY_ARGS_rope := --attacks --terrain 10c72e --ring 10c72e --group-map b7000
# ropedeath = two rope soldiers 082BCE (2D8F8A/2D8F9C) killed by the pilot:
# 082EAA, the rope end 083212, the fall 082F1A and the body 082F48 (rope3
# from sec03_26140, family frames 53681..55600).
FAMILY_ARGS_ropedeath := --attacks --terrain 10c72e --ring 10c72e --group-map b7000
# ventwait = the gas vent's constructor 03A05A (rank words 2D3A76/2D3AD8)
# and its wait below the camera limit (rope2 from sec03_26140, frames
# 37121..39200).
FAMILY_ARGS_ventwait := --attacks --terrain 10c72e --ring 10c72e --group-map b7000
# boulder = the Mission 2 stage-3 falling boulder 03A2AA (record 2D8D82,
# camera 952): the boulder 03A344, dust 03A464, slab 03A528 and the map
# patches 0B7A2C (0C70FC with source offsets D0/D1) into the ring of
# 10C72E (neo_m2props.c; boulder from sec03_26140, family 37121..39300).
FAMILY_ARGS_boulder := --attacks --terrain 10c72e --ring 10c72e --group-map b7000
# m2boss<n> = the Mission 2 stage-3 boss 026254 (neo_m2boss.c) in pieces of
# one fight (capture_fullgame.lua from at_92700, MSX_FULL_FAMILY_RANGE=
# 26254-27e8e): body, parts, shots, the column, the music delay 059F76 and
# the debris of its family (docs/JEFE_MISION2.md).
M2BOSS_ARGS := --m2boss --late-creator --attacks --terrain 10c72e --ring 10c72e --group-map b7000
FAMILY_ARGS_m2boss1 := $(M2BOSS_ARGS)
FAMILY_ARGS_m2boss2 := $(M2BOSS_ARGS)
FAMILY_ARGS_m2boss3 := $(M2BOSS_ARGS)
FAMILY_ARGS_m2boss4 := $(M2BOSS_ARGS)
FAMILY_ARGS_m2boss5 := $(M2BOSS_ARGS)
FAMILY_ARGS_m2boss6 := $(M2BOSS_ARGS)
FAMILY_ARGS_m2boss7 := $(M2BOSS_ARGS)
FAMILY_ARGS_m2boss8 := $(M2BOSS_ARGS)
FAMILY_ARGS_truck := --tree
FAMILY_ARGS_swordgen := --tree
# The shop 0B6566 fell before the prisoner (pri1 RAM dumps 7200 -> 7500: its
# raise and clear stamps by its roof 0B680E at +28/+2C = 0A48/0040, barrel fixture).
FAMILY_ARGS_prisoner := --stamp a480000,400000,b69f8 --stamp a480000,400000,b6a2c --stamp a480000,400000,b6a12 --stamp a480000,400000,b6a42
# The H item is taken at 8081 (08B59E: weapon 0A, 200 rounds), then two
# updates of 08C66A.
FAMILY_ARGS_hitem := --pickup 8081,a,c8
# The C item (4B) taken at 8370 in the weapons capture cfam (08B58C: weapon
# 0C, 40 rounds), its sparkle 08C68E/08C6B2 and two updates of 08C66A.
FAMILY_ARGS_citem := --pickup 8370,c,28
# The player's bullets (neo_bullet.c) of the weapons captures: hfam (200 H
# bullets 01B716, 64 impacts 01BC2E) and cfam before the C (36 pistol
# bullets 01B782). The player (outside the family) creates them; a bullet
# deleted with an impact hit (0049E8 contact), one without left the view.
BULLET_ARGS := --hit-child 1bc2e --hit-from 1b6dc,1b716,1b750,1b782,1b882,1bbe2
FAMILY_ARGS_hbullet := $(BULLET_ARGS)
FAMILY_ARGS_pbullet := $(BULLET_ARGS)
# The C weapon of cfam (neo_chaser.c, extract_family_fixture.py --range
# 63600-64300 --range c5800-c5a00 --ram 10c718-10c72c --attacks): 40 shells
# 063F32 and their flight 063FDC with the 0049E8 answers (D0, the A0/A1
# targets 01B162 read), 32 explosions 064128 and blasts 06371E (after the
# list head, 00411C), 286 trail puffs 063D9E pulled towards the node before
# them (+04), 40 muzzle flashes 0C58D2, and the weapon block (0C0DFC).
FAMILY_ARGS_chaser := --script-actor 101414 --script-routines 6371e --attacks --previous
# Weapon 0D (neo_pweapon.c; capture d1: capture_weapons.lua PLAN=air with
# ASSIST_ID=d, FAMILY 2150..3140 ROOTS=8606a,c58d2,8651c; extract_family_fixture.py
# --range 86000-86700 --range c5800-c5a00 --ram 10c718-10c72c --attacks):
# 53 balls (level, up, down; rolls and bursts), their glows 08651C and the
# blasts 08642E made after the list head by the explosion programs.
FAMILY_ARGS_dball := --script-actor 101414 --script-routines 8642e --attacks --terrain 10c72e
# Weapons 02 / 07 (neo_pweapon.c; captures s2 / s7 as d1 with ASSIST_ID=2 / 7,
# ROOTS=644c4,646b0; --range 64400-64700): 23 blasts each.
FAMILY_ARGS_sgun2 := --attacks --terrain 10c72e
FAMILY_ARGS_sgun7 := $(FAMILY_ARGS_sgun2)
# Weapons 04 / 09 (captures f4 / f9, ROOTS=a71f8,a716e; --range a7100-a7800):
# the flames, their first flame and puffs (pulled by the node before them).
FAMILY_ARGS_flame4 := --attacks --previous --terrain 10c72e
FAMILY_ARGS_flame9 := $(FAMILY_ARGS_flame4)
FAMILY_ARGS_rlaunch := --script-actor 101414 --script-routines 6371e --attacks --previous --spawn-window 2
# Weapon 8 (item kind 51; capture r8: capture_weapons.lua PLAN=air ASSIST_ID=8,
# FAMILY 2150..3140 ROOTS=641ec,c58d2,63d9e; extract_family_fixture.py --range
# 63600-64700 --range c5800-c5a00 --ram 10c718-10c72c --attacks): 42 rockets
# 0641EC (bigger frames, rows 063640/063650), trails, flashes, explosions;
# the capture is in the village (camera 0): terrain object 10C72E.
FAMILY_ARGS_rlaunch8 := $(FAMILY_ARGS_rlaunch) --terrain 10c72e
# Weapons 06 / 0B (neo_pweapon.c; captures w6 / wb: capture_weapons.lua
# PLAN=air ASSIST_ID=6 / B, FAMILY 2150..3140 ROOTS=1e22c,c58d2;
# extract_family_fixture.py --range 1e100-1f100 --ram 10c340-10c350 --ram
# 1014c4-101574 --ram 10000e-100010 --ram 10c718-10c730 --attacks): 9 beams
# 01E22C (several draws per update, the beam block 10C340 and the player's
# node as read; the writes compared), 65 sparks; village terrain 10C72E.
FAMILY_ARGS_laser6 := --attacks --terrain 10c72e
FAMILY_ARGS_laserb := $(FAMILY_ARGS_laser6)
# molotov = the bomb type 2 (0C4756: 01E648/01E664, flight 01E738, the
# hit in flight 01E7B0 with 01C7EA, the fire 01E80E/01E894 and its flames
# 01E9B6..01EA3A; neo_bullet.c), m from sec02_19932, frames 23700..26139.
FAMILY_ARGS_molotov := --attacks --terrain 10c72e --ring 10c72e --group-map b7000
# The Slugnoid 09692C (neo_slugnoid.c, B38): captures idle / fall / tour
# (tools/capture_fullgame.lua MSX_FULL_RIDE, docs/ARMAS_VEHICULOS.md).
SLUGNOID_ARGS := --script-actor 101414 --script-routines 1b6a0,97dd0,ad9b8,ad8a2,1c71e --node-fields 6e --attacks --terrain 10c72e --ring 10c72e --group-map b7000
FAMILY_ARGS_slugidle := $(SLUGNOID_ARGS)
FAMILY_ARGS_slugfall := $(SLUGNOID_ARGS) --resume
FAMILY_ARGS_slugtour := $(SLUGNOID_ARGS) --resume
FAMILY_ARGS_sluglaunch := $(SLUGNOID_ARGS) --resume
# The 10 grenades 01BF46 of cfam (bounces, three explosions 01C152/01C19A);
# the one that explodes on 0049E8 before its flight is the contact.
FAMILY_ARGS_grenade := --hit-to 1c152
# The armoured vehicle's marker child 040396 reads RAM 10C446 = 2 (its
# +70 = 3 in veh1); the trace keeps no read of that byte.
FAMILY_ARGS_armored := --tree --marker-seq 2 --script-actor 101414
# The hidden boxes 09A67E.. of the dome ruin and the items they release after
# the collapse (food1: kinds 58, 37, 54, 38, 2E); the script actor makes the
# boxes, the boxes the items (00411C). Three food pickups (points: the weapon
# stays). Cut before 7339 (fixture source): the post-collapse collision of the
# rubble map 0B53A8 is not in the port's grid.
FAMILY_ARGS_food := --script-actor 101414 --script-routines 8c374,8c37c --pickup 7119,0,ffff --pickup 7137,0,ffff --pickup 7139,0,ffff
# Score labels 08D330 of the item pickups and the item kind 55 (motion
# 08BCCC) of gift3 (capture-only ROM patch: the prisoner 2D6ED0 gives kind
# 55): a label an item's pickup makes is native (--label-after-pickup: one
# per pickup, at the item's projected position), the labels other actors
# make (08CE72/08CEEC) are injected; RAM 10C5B2/10C5B4 as read.
FAMILY_ARGS_label := --script-actor 101414 --script-routines 1 --label-after-pickup 8d330 --pickup 7848,0,ffff --pickup 7848,0,ffff --pickup 8017,0,ffff --pickup 8081,a,c8
# The Mission 2 stage-3 coins (kind 56, motion 08BD32: 01ABC8 and the
# growth of 08B9D8; inv3 window 55420..61300 of capture_fullgame.lua from
# sec03_26140, MSX_FULL_FAMILY_ROOTS=8c374,8c37c MSX_FULL_FAMILY_RANGE=
# 8c374-8d900): six coins of the script (08C37C, 2D8EEC..2D8F32), taken in
# 58215..60873 (points only: the weapon block, never read by these
# updates, stays as the fixture leaves it), their score labels 08D330,
# and the 2F item 2D8FEA left on the ground.
FAMILY_ARGS_coin := --terrain 10c72e --ring 10c72e --script-actor 101414 --script-routines 1 --label-after-pickup 8d330 --pickup 58215,0,0 --pickup 58835,0,0 --pickup 60153,0,0 --pickup 60161,0,0 --pickup 60867,0,0 --pickup 60873,0,0
FAMILY_HEADERS := $(patsubst %,$(BUILD_DIR)/%_fixture.h,$(FAMILIES))
$(FAMILY_HEADERS): $(BUILD_DIR)/%_fixture.h: data/%_fixture.json.gz tools/compile_family_fixture.py
	python3 tools/compile_family_fixture.py $< --output $@ --name $* $(FAMILY_ARGS_$*)
$(BUILD_DIR)/src/neo_region.o: $(BUILD_DIR)/region_rom.h
$(BUILD_DIR)/soldier_world_fixture.h: tools/compile_soldier_world.py data/soldier_world_fixture.json.gz
	python3 tools/compile_soldier_world.py
$(BUILD_DIR)/soldier_script_fixture.h: tools/compile_soldier_script.py data/soldier_script_fixture.json.gz
	python3 tools/compile_soldier_script.py
$(BUILD_DIR)/kernel_fixture.h: tools/compile_kernel_fixture.py data/soldier_kernel_fixture.json.gz
	python3 tools/compile_kernel_fixture.py
$(BUILD_DIR)/m2_stairs_fixture.h: tools/compile_m2_stairs_fixture.py data/m2_stairs_fixture.json.gz
	python3 tools/compile_m2_stairs_fixture.py data/m2_stairs_fixture.json.gz $@
# Mission 3 map reconnaissance (docs/MISION3.md "Reconocimiento del mapa"):
# the arcade's kernel cell reads of the whole mission against romfs/mission3.pack.
$(BUILD_DIR)/m3_map_fixture.h: tools/compile_m3_map_fixture.py data/m3_map_fixture.json.gz
	python3 tools/compile_m3_map_fixture.py data/m3_map_fixture.json.gz $@
# check-m3sweeps: also every player sweep of the run (private fixture
# filesystem/mission3/m3_sweeps.json.gz, tools/extract_m3_recon.py).
$(BUILD_DIR)/m3sweeps/m3_map_fixture.h: tools/compile_m3_map_fixture.py data/m3_map_fixture.json.gz filesystem/mission3/m3_sweeps.json.gz
	@mkdir -p $(dir $@)
	python3 tools/compile_m3_map_fixture.py data/m3_map_fixture.json.gz $@ --sweeps filesystem/mission3/m3_sweeps.json.gz
.PHONY: check-m3sweeps
check-m3sweeps: $(BUILD_DIR)/m3sweeps/m3_map_fixture.h $(BUILD_DIR)/mission_collision.h
	@mkdir -p $(BUILD_DIR)/host
	$(HOST_CC) $(HOST_CFLAGS) -I$(BUILD_DIR)/m3sweeps -I$(BUILD_DIR) tools/test_m3_map.c src/neo_kernel.c src/neo_collision.c -o $(BUILD_DIR)/host/m3sweeps
	$(BUILD_DIR)/host/m3sweeps romfs/mission3.pack
$(BUILD_DIR)/tools/test_neo_kernel.o: CC = $(N64_CC)
$(BUILD_DIR)/tools/test_neo_kernel.o: CFLAGS = $(N64_CFLAGS) -MMD
$(BUILD_DIR)/tools/test_neo_kernel.o: $(BUILD_DIR)/kernel_fixture.h
$(BUILD_DIR)/kernel-regression.elf: $(BUILD_DIR)/tools/test_neo_kernel.o $(BUILD_DIR)/src/neo_kernel.o $(BUILD_DIR)/src/neo_collision.o
kernel-regression.z64: N64_ROM_TITLE = "MSX KERNEL TEST"
$(BUILD_DIR)/src/retro_effects.o: $(BUILD_DIR)/retro25d/retro_assets.h
$(BUILD_DIR)/retro25d/retro_assets.h: tools/build_retro_assets.py tools/retro_geometry.py data/retro_surfaces.json filesystem/mission1/collision.json
	python3 tools/build_retro_assets.py
$(BUILD_DIR)/mission_collision.h: tools/compile_collision.py filesystem/mission1/collision.json $(wildcard filesystem/mission1/rest/collision.json)
	python3 tools/compile_collision.py
filesystem/fixed.bin filesystem/sprites.bin:
	@echo 'Primero: python3 tools/import_rom.py /ruta/mslugx.zip' >&2
	@exit 1
filesystem/hud-palette.bin:
	@echo 'Falta la paleta del HUD: python3 tools/build_hud_assets.py CAPTURA/palette<f>.bin...; consultar docs/ARMAS_VEHICULOS.md' >&2
	@exit 1
filesystem/projectiles.json:
	@echo 'Faltan los proyectiles: python3 tools/export_projectiles.py /ruta/capturas-proyectiles; consultar docs/PROJECTILES.md' >&2
	@exit 1
filesystem/vehicle.json:
	@echo 'Falta el blindado: consultar docs/NATIVE_BACKEND.md y tools/export_vehicle.py' >&2
	@exit 1
filesystem/marco.bin filesystem/marco-actions.bin filesystem/desert.bin filesystem/soldier.bin filesystem/marco-motion.bin:
	@echo 'Falta $@: consultar docs/MARCO.md, docs/ACTION.md y docs/ENCOUNTER.md' >&2
	@exit 1
clean:
	rm -rf build mslugx-camel-shot.z64 intro-preview.z64 marco-upper-limits-regression.z64 marco-common-regression.z64 marco-body-regression.z64 marco-idle-regression.z64 marco-stance-regression.z64 marco-ammo-regression.z64 marco-motion-regression.z64 marco-crouch-regression.z64 marco-upper-regression.z64 marco-upper-combat-regression.z64 marco-advance-pixels.z64 marco-advance-regression.z64 marco-coupled-pixels.z64 marco-controller-regression.z64 marco-coupled-regression.z64 marco-pair-regression.z64 marco-render.z64 marco-regression.z64 mslugx-explorer.z64 mslugx-m3-release.z64 mslugx-lab.z64 rng-regression.z64 animation-regression.z64 contact-regression.z64 control-regression.z64 state-regression.z64 throw-regression.z64 attack-regression.z64 object-regression.z64 locomotion-regression.z64 kernel-regression.z64 audio-regression.z64 cinematic-regression.z64 mission1-internal.z64 scene-debug.z64 scene-release.z64
# Offline effect previews (proxy level, no original art) from the real mesh code.
preview-depth: $(BUILD_DIR)/retro25d/retro_assets.h
	python3 tools/export_effect_previews.py
$(BUILD_DIR)/src/title_intro.o: $(BUILD_DIR)/title_motion_meta.h
$(BUILD_DIR)/title_motion_meta.h: tools/compile_cinematic_crcs.py filesystem/cinematics/title-motion.json
	python3 tools/compile_cinematic_crcs.py filesystem/cinematics/title-motion.json $@

check-cinematic: $(BUILD_DIR)/entry_meta.h $(BUILD_DIR)/title_motion_meta.h
	mkdir -p $(BUILD_DIR)/host
	$(HOST_CC) $(HOST_CFLAGS) -I$(BUILD_DIR) tools/test_cinematic_pack.c -o $(BUILD_DIR)/host/cinematic-pack
	$(BUILD_DIR)/host/cinematic-pack filesystem/cinematics/entry.msxc
	$(HOST_CC) $(HOST_CFLAGS) -I$(BUILD_DIR) -DMSX_CINE_META_HEADER='"title_motion_meta.h"' tools/test_cinematic_pack.c -o $(BUILD_DIR)/host/title-motion-pack
	$(BUILD_DIR)/host/title-motion-pack filesystem/cinematics/title-motion.msxc
	$(HOST_CC) $(HOST_CFLAGS) tools/test_cinematic_stream.c -o $(BUILD_DIR)/host/cinematic-stream
	python3 tools/msxc_compress.py filesystem/cinematics/entry.msxc $(BUILD_DIR)/host/entry-v2.msxc
	$(BUILD_DIR)/host/cinematic-stream filesystem/cinematics/entry.msxc $(BUILD_DIR)/host/entry-v2.msxc
	python3 tools/test_title_palette_banks.py
	python3 tools/test_title_x64.py
$(BUILD_DIR)/blood_fixture.h: tools/compile_blood_fixture.py data/blood_fixture.json
	python3 tools/compile_blood_fixture.py

$(BUILD_DIR)/marco_selectors.h: tools/compile_marco_selectors.py data/marco_selector_fixture.json
	python3 tools/compile_marco_selectors.py

$(BUILD_DIR)/marco_fields.h: tools/compile_marco_fields.py data/marco_fields_fixture.json
	python3 tools/compile_marco_fields.py

$(BUILD_DIR)/history_fixture.h: tools/compile_history_fixture.py data/soldier_history_fixture.json
	python3 tools/compile_history_fixture.py

$(BUILD_DIR)/marco_controller_fixture.h: tools/compile_marco_controller.py data/marco_controller_fixture.json
	python3 tools/compile_marco_controller.py

$(BUILD_DIR)/marco-advance/marco_controller_fixture.h: tools/compile_marco_controller.py data/marco_advance_controller_fixture.json
	python3 tools/compile_marco_controller.py --fixture data/marco_advance_controller_fixture.json --output $@

$(BUILD_DIR)/marco_upper_fixture.h: tools/compile_marco_upper.py data/marco_upper_fixture.json
	python3 tools/compile_marco_upper.py
$(BUILD_DIR)/marco-upper-combat/marco_upper_fixture.h: tools/compile_marco_upper.py data/marco_upper_combat_fixture.json
	python3 tools/compile_marco_upper.py --fixture data/marco_upper_combat_fixture.json --output $@

$(BUILD_DIR)/marco_crouch_fixture.h: tools/compile_marco_crouch.py tools/compile_marco_upper.py data/marco_crouch_fixture.json
	python3 tools/compile_marco_crouch.py
$(BUILD_DIR)/marco-crouch-turn/marco_crouch_fixture.h: tools/compile_marco_crouch.py tools/compile_marco_upper.py data/marco_crouch_turn_fixture.json
	python3 tools/compile_marco_crouch.py --fixture data/marco_crouch_turn_fixture.json --output $@

$(BUILD_DIR)/marco-motion/marco_motion_fixture.h: tools/compile_marco_motion.py tools/compile_marco_crouch.py tools/compile_marco_upper.py data/marco_motion_fixture.json
	python3 tools/compile_marco_motion.py

$(BUILD_DIR)/marco_ammo_fixture.h: tools/compile_marco_ammo.py data/marco_ammo_fixture.json
	python3 tools/compile_marco_ammo.py
# Body programs' +72/+74 (src/marco_legs.c): tools/extract_legs_fixture.py.
$(BUILD_DIR)/legs_fixture.h: tools/compile_legs_fixture.py data/legs_fixture.json
	python3 tools/compile_legs_fixture.py
# H muzzles (docs/ARMAS_VEHICULOS.md §10): tools/extract_hmuzzle_fixture.py.
$(BUILD_DIR)/hmuzzle_fixture.h: tools/compile_hmuzzle_fixture.py data/hmuzzle_fixture.json
	python3 tools/compile_hmuzzle_fixture.py
# Fat Marco's shots (docs/MISSION3_DEBUG.md G21): tools/extract_fat_muzzle_fixture.py.
$(BUILD_DIR)/fatmuzzle_fixture.h: tools/extract_fat_muzzle_fixture.py data/fatmuzzle_fixture.json
	python3 tools/extract_fat_muzzle_fixture.py --header data/fatmuzzle_fixture.json $@
# Player death/respawn (docs/JEFE_MISION1.md): tools/extract_death_fixture.py.
$(BUILD_DIR)/death_fixture.h: tools/compile_death_fixture.py data/death_fixture.json
	python3 tools/compile_death_fixture.py
# Respawn at the Mission 2 boss (B33, docs/JEFE_MISION2.md): tools/extract_respawn_fixture.py.
$(BUILD_DIR)/respawn_fixture.h: tools/compile_respawn_fixture.py data/respawn_fixture.json
	python3 tools/compile_respawn_fixture.py
# H aim (docs/ARMAS_VEHICULOS.md §10): tools/extract_hgun_fixture.py.
$(BUILD_DIR)/hgun_fixture.h: tools/compile_hgun_fixture.py data/hgun_fixture.json data/hgun_replay_fixture.json
	python3 tools/compile_hgun_fixture.py
# Fix-layer HUD (docs/ARMAS_VEHICULOS.md §11): tools/extract_hud_fixture.py.
$(BUILD_DIR)/hud_fixture.h: tools/compile_hud_fixture.py data/hud_fixture.json.gz
	python3 tools/compile_hud_fixture.py
# Game over / continue (docs/MISION1_COMPLETA.md): tools/extract_continue_fixture.py.
$(BUILD_DIR)/continue_fixture.h: tools/compile_continue_fixture.py data/continue_fixture.json.gz
	python3 tools/compile_continue_fixture.py
# Weapon block 10C718 (docs/ARMAS_VEHICULOS.md): tools/extract_weapon_block.py.
$(BUILD_DIR)/weapon_block_fixture.h: tools/compile_weapon_block.py data/weapon_block_fixture.json.gz
	python3 tools/compile_weapon_block.py

$(BUILD_DIR)/marco_stance_fixture.h: tools/compile_marco_stance.py tools/compile_marco_upper.py data/marco_stance_fixture.json
	python3 tools/compile_marco_stance.py

# Vehicle slots (docs/ARMAS_VEHICULOS.md §7): every slot service call of camelfam/tankfam.
$(BUILD_DIR)/vslot_fixture.h: tools/compile_vslot_fixture.py data/vslot_fixture.json.gz data/region_rom.json
	python3 tools/compile_vslot_fixture.py
# Vulcan turret control 03B57C: every turret update of camelfam/tankfam.
$(BUILD_DIR)/mount_fixture.h: tools/compile_mount_fixture.py data/mount_fixture.json.gz
	python3 tools/compile_mount_fixture.py

$(BUILD_DIR)/vulcan_fixture.h: tools/compile_vulcan_fixture.py data/vulcan_fixture.json.gz data/region_rom.json
	python3 tools/compile_vulcan_fixture.py
$(BUILD_DIR)/marco_idle_fixture.h: tools/compile_marco_idle.py data/marco_idle_fixture.json
	python3 tools/compile_marco_idle.py

check-core: $(BUILD_DIR)/continue_fixture.h $(BUILD_DIR)/legs_fixture.h $(BUILD_DIR)/death_fixture.h $(BUILD_DIR)/respawn_fixture.h $(BUILD_DIR)/hgun_fixture.h $(BUILD_DIR)/hud_fixture.h $(BUILD_DIR)/marco_idle_fixture.h $(BUILD_DIR)/marco_stance_fixture.h $(BUILD_DIR)/marco_ammo_fixture.h $(BUILD_DIR)/weapon_block_fixture.h $(BUILD_DIR)/marco-motion/marco_motion_fixture.h $(BUILD_DIR)/marco_crouch_fixture.h $(BUILD_DIR)/marco-crouch-turn/marco_crouch_fixture.h $(BUILD_DIR)/marco_upper_fixture.h $(BUILD_DIR)/marco-upper-combat/marco_upper_fixture.h $(BUILD_DIR)/marco-advance/marco_controller_fixture.h $(BUILD_DIR)/marco_controller_fixture.h $(BUILD_DIR)/marco_fields.h $(BUILD_DIR)/marco_selectors.h $(BUILD_DIR)/soldier_fixture.h $(BUILD_DIR)/rng_fixture.h $(BUILD_DIR)/animation_fixture.h $(BUILD_DIR)/history_fixture.h $(BUILD_DIR)/vslot_fixture.h $(BUILD_DIR)/vulcan_fixture.h $(BUILD_DIR)/mount_fixture.h
	mkdir -p $(BUILD_DIR)/host
	$(HOST_CC) $(HOST_CFLAGS) tools/test_fixed.c -o $(BUILD_DIR)/host/fixed
	$(HOST_CC) $(HOST_CFLAGS) tools/test_sprites.c -o $(BUILD_DIR)/host/sprites
	$(HOST_CC) $(HOST_CFLAGS) tools/test_neo_motion.c -o $(BUILD_DIR)/host/motion
	$(HOST_CC) $(HOST_CFLAGS) tools/test_scene_codec.c -o $(BUILD_DIR)/host/codec
	$(HOST_CC) $(HOST_CFLAGS) tools/test_scene_clock.c -o $(BUILD_DIR)/host/clock
	$(HOST_CC) $(HOST_CFLAGS) tools/test_cinematic_codec.c -o $(BUILD_DIR)/host/cinematic-codec
	$(HOST_CC) $(HOST_CFLAGS) tools/test_msx_sfx.c src/msx_sfx.c -o $(BUILD_DIR)/host/msx-sfx
	$(HOST_CC) $(HOST_CFLAGS) -DMSX_DIAG=1 tools/test_msx_diag.c src/msx_diag_core.c -o $(BUILD_DIR)/host/msx-diag
	$(BUILD_DIR)/host/msx-diag 2>/dev/null
	$(BUILD_DIR)/host/msx-sfx
	$(BUILD_DIR)/host/fixed
	$(BUILD_DIR)/host/sprites
	$(BUILD_DIR)/host/motion
	$(BUILD_DIR)/host/codec
	$(BUILD_DIR)/host/clock
	$(BUILD_DIR)/host/cinematic-codec
	python3 tools/test_neo_scene.py
	python3 tools/test_collision_audit.py
	python3 tools/test_mission_rest.py
	python3 tools/test_neo_queue_publication.py
	python3 tools/test_vehicle_wreckage.py
	$(HOST_CC) $(HOST_CFLAGS) -I$(BUILD_DIR) tools/test_neo_soldier.c src/neo_soldier.c src/neo_rng.c -o $(BUILD_DIR)/host/soldier
	$(BUILD_DIR)/host/soldier
	$(HOST_CC) $(HOST_CFLAGS) -I$(BUILD_DIR) tools/test_neo_rng.c src/neo_soldier.c src/neo_rng.c -o $(BUILD_DIR)/host/rng
	$(BUILD_DIR)/host/rng
	$(HOST_CC) $(HOST_CFLAGS) -I$(BUILD_DIR) tools/test_neo_animation.c src/neo_animation.c -o $(BUILD_DIR)/host/animation
	$(BUILD_DIR)/host/animation
	$(HOST_CC) $(HOST_CFLAGS) tools/test_retro25d.c src/retro25d.c src/retro_events.c -lm -o $(BUILD_DIR)/host/retro25d
	$(BUILD_DIR)/host/retro25d
	$(HOST_CC) $(HOST_CFLAGS) -I$(BUILD_DIR) tools/test_neo_history.c -o $(BUILD_DIR)/host/history
	$(BUILD_DIR)/host/history
	$(HOST_CC) $(HOST_CFLAGS) -I$(BUILD_DIR) tools/test_marco_fields.c src/neo_animation.c -o $(BUILD_DIR)/host/marco-fields
	$(BUILD_DIR)/host/marco-fields
	$(HOST_CC) $(HOST_CFLAGS) tools/test_marco_animation.c src/marco_animation.c src/neo_animation.c -o $(BUILD_DIR)/host/marco-animation
	$(BUILD_DIR)/host/marco-animation
	$(HOST_CC) $(HOST_CFLAGS) -I$(BUILD_DIR) tools/test_marco_controller.c src/marco_controller.c src/marco_animation.c src/neo_animation.c -o $(BUILD_DIR)/host/marco-controller
	$(BUILD_DIR)/host/marco-controller
	$(HOST_CC) $(HOST_CFLAGS) -I$(BUILD_DIR)/marco-advance tools/test_marco_controller.c src/marco_controller.c src/marco_animation.c src/neo_animation.c -o $(BUILD_DIR)/host/marco-advance-controller
	$(BUILD_DIR)/host/marco-advance-controller
	$(HOST_CC) $(HOST_CFLAGS) -I$(BUILD_DIR) tools/test_marco_upper.c src/marco_upper.c src/marco_controller.c src/marco_animation.c src/neo_animation.c -o $(BUILD_DIR)/host/marco-upper
	$(BUILD_DIR)/host/marco-upper
	$(HOST_CC) $(HOST_CFLAGS) -I$(BUILD_DIR)/marco-upper-combat tools/test_marco_upper.c src/marco_upper.c src/marco_controller.c src/marco_animation.c src/neo_animation.c -o $(BUILD_DIR)/host/marco-upper-combat-core
	$(BUILD_DIR)/host/marco-upper-combat-core
	$(HOST_CC) $(HOST_CFLAGS) tools/test_marco_upper_limits.c src/marco_crouch.c src/marco_upper.c src/marco_controller.c src/marco_animation.c src/neo_animation.c -o $(BUILD_DIR)/host/marco-upper-limits
	$(BUILD_DIR)/host/marco-upper-limits
	$(HOST_CC) $(HOST_CFLAGS) -I$(BUILD_DIR) tools/test_marco_crouch.c src/marco_crouch.c src/marco_upper.c src/marco_controller.c src/marco_animation.c src/neo_animation.c -o $(BUILD_DIR)/host/marco-crouch
	$(BUILD_DIR)/host/marco-crouch
	$(HOST_CC) $(HOST_CFLAGS) -I$(BUILD_DIR)/marco-crouch-turn tools/test_marco_crouch.c src/marco_crouch.c src/marco_upper.c src/marco_controller.c src/marco_animation.c src/neo_animation.c -o $(BUILD_DIR)/host/marco-crouch-turn
	$(BUILD_DIR)/host/marco-crouch-turn
	$(HOST_CC) $(HOST_CFLAGS) -I$(BUILD_DIR)/marco-motion tools/test_marco_motion.c src/marco_motion.c src/marco_crouch.c src/marco_upper.c src/marco_controller.c src/marco_animation.c src/neo_animation.c -o $(BUILD_DIR)/host/marco-motion
	$(BUILD_DIR)/host/marco-motion
	$(HOST_CC) $(HOST_CFLAGS) tools/test_marco_body.c src/marco_body.c src/marco_stance.c src/marco_motion.c src/marco_crouch.c src/marco_upper.c src/marco_controller.c src/marco_animation.c src/neo_animation.c -o $(BUILD_DIR)/host/marco-body
	$(BUILD_DIR)/host/marco-body
	$(HOST_CC) $(HOST_CFLAGS) -I$(BUILD_DIR) tools/test_marco_ammo.c src/marco_ammo.c -o $(BUILD_DIR)/host/marco-ammo
	$(BUILD_DIR)/host/marco-ammo
	$(HOST_CC) $(HOST_CFLAGS) -I$(BUILD_DIR) tools/test_marco_weapons.c src/marco_ammo.c -o $(BUILD_DIR)/host/marco-weapons
	$(BUILD_DIR)/host/marco-weapons
	$(HOST_CC) $(HOST_CFLAGS) -I$(BUILD_DIR) tools/test_neo_hud.c src/neo_hud.c src/neo_vslot.c -o $(BUILD_DIR)/host/hud
	$(HOST_CC) $(HOST_CFLAGS) -I$(BUILD_DIR) tools/test_neo_continue.c src/neo_hud.c -o $(BUILD_DIR)/host/continue
	$(BUILD_DIR)/host/continue
	$(HOST_CC) $(HOST_CFLAGS) -I$(BUILD_DIR) tools/test_neo_vslot.c src/neo_vslot.c -o $(BUILD_DIR)/host/vslot
	$(BUILD_DIR)/host/vslot
	$(HOST_CC) $(HOST_CFLAGS) -I$(BUILD_DIR) tools/test_neo_vulcan.c src/neo_vulcan.c -o $(BUILD_DIR)/host/vulcan
	$(BUILD_DIR)/host/vulcan
	$(HOST_CC) $(HOST_CFLAGS) -I$(BUILD_DIR) tools/test_marco_mount.c src/marco_mount.c src/neo_vslot.c -o $(BUILD_DIR)/host/mount
	$(BUILD_DIR)/host/mount
	$(BUILD_DIR)/host/hud
	$(HOST_CC) $(HOST_CFLAGS) -I$(BUILD_DIR) tools/test_marco_hgun.c src/marco_hgun.c -o $(BUILD_DIR)/host/hgun
	$(BUILD_DIR)/host/hgun
	$(HOST_CC) $(HOST_CFLAGS) -I$(BUILD_DIR) tools/test_marco_death.c src/marco_death.c src/neo_animation.c -o $(BUILD_DIR)/host/death
	$(BUILD_DIR)/host/death
	$(HOST_CC) $(HOST_CFLAGS) -I$(BUILD_DIR) tools/test_marco_legs.c src/marco_legs.c src/neo_animation.c -o $(BUILD_DIR)/host/legs
	$(BUILD_DIR)/host/legs
	$(HOST_CC) $(HOST_CFLAGS) -I$(BUILD_DIR) tools/test_marco_stance.c src/marco_stance.c src/marco_upper.c src/marco_controller.c src/marco_animation.c src/neo_animation.c -o $(BUILD_DIR)/host/marco-stance
	$(BUILD_DIR)/host/marco-stance
	$(HOST_CC) $(HOST_CFLAGS) -I$(BUILD_DIR) tools/test_marco_idle.c src/marco_stance.c src/marco_upper.c src/marco_controller.c src/marco_animation.c src/neo_animation.c -o $(BUILD_DIR)/host/marco-idle
	$(BUILD_DIR)/host/marco-idle
	$(HOST_CC) $(HOST_CFLAGS) tools/test_diorama_building.c src/diorama_building.c src/retro25d.c -lm -o $(BUILD_DIR)/host/diorama-building
	$(BUILD_DIR)/host/diorama-building
	$(HOST_CC) $(HOST_CFLAGS) tools/test_marco_common.c src/marco_common.c src/marco_upper.c src/marco_controller.c src/marco_animation.c src/neo_animation.c -o $(BUILD_DIR)/host/marco-common
	$(BUILD_DIR)/host/marco-common
# Mission 1 boss (docs/JEFE_MISION1.md): ROM-derived tables and fight fixtures.
$(BUILD_DIR)/boss_tables.h: tools/compile_boss_tables.py data/boss_tables.json data/boss_programs.json
	python3 tools/compile_boss_tables.py
$(BUILD_DIR)/boss_fixture.h: tools/compile_boss_fixture.py $(wildcard data/boss_fight_*.json.gz)
	python3 tools/compile_boss_fixture.py
$(BUILD_DIR)/src/neo_boss.o: $(BUILD_DIR)/boss_tables.h
BOSS_SRCS_N64 := src/neo_boss.c src/neo_animation.c src/neo_contact.c src/neo_cull.c src/neo_collision.c src/neo_soldier_ai.c src/neo_blood.c src/neo_kernel.c src/neo_control.c src/neo_rng.c src/neo_soldier.c src/neo_soldier_action.c src/neo_hit.c src/neo_target.c
# Internal MIPS regression: the fight fixtures replayed by neo_boss in ares
# (tools/test_boss_ares.py); the streams are DFS files, not headers.
$(BUILD_DIR)/boss_fight_names.h $(BUILD_DIR)/boss-romfs/boss-a.bin $(BUILD_DIR)/boss-romfs/boss-b.bin: $(BUILD_DIR)/boss_fixture.h
$(BUILD_DIR)/tools/test_neo_boss.o: CFLAGS = $(N64_CFLAGS) -MMD
$(BUILD_DIR)/tools/test_neo_boss.o: $(BUILD_DIR)/boss_fight_names.h $(BUILD_DIR)/boss_tables.h
$(BUILD_DIR)/boss-regression.elf: $(BUILD_DIR)/tools/test_neo_boss.o $(patsubst src/%.c,$(BUILD_DIR)/src/%.o,$(BOSS_SRCS_N64))
$(BUILD_DIR)/boss-regression.dfs: N64_MKDFS_ROOT = $(BUILD_DIR)/boss-romfs
$(BUILD_DIR)/boss-regression.dfs: $(BUILD_DIR)/boss-romfs/boss-a.bin $(BUILD_DIR)/boss-romfs/boss-b.bin
boss-regression.z64: N64_ROM_TITLE = "MSX BOSS TEST"
boss-regression.z64: $(BUILD_DIR)/boss-regression.dfs
BOSS_SRCS := src/neo_boss.c src/neo_animation.c src/neo_contact.c src/neo_cull.c src/neo_collision.c src/neo_soldier_ai.c src/neo_blood.c src/neo_kernel.c src/neo_control.c src/neo_rng.c src/neo_soldier.c src/neo_soldier_action.c src/neo_hit.c src/neo_target.c
check-boss: $(BUILD_DIR)/region_rom.h $(BUILD_DIR)/boss_tables.h $(BUILD_DIR)/boss_fixture.h $(BUILD_DIR)/scene_meta.h $(BUILD_DIR)/soldier_ai_tables.h $(BUILD_DIR)/contact_tables.h $(BUILD_DIR)/control_tables.h $(BUILD_DIR)/state_tables.h $(BUILD_DIR)/mission_collision.h $(BUILD_DIR)/vehicle_wreckage.h
	mkdir -p $(BUILD_DIR)/host
	$(HOST_CC) $(HOST_CFLAGS) -I$(BUILD_DIR) tools/test_neo_boss.c $(BOSS_SRCS) -o $(BUILD_DIR)/host/boss
	$(BUILD_DIR)/host/boss
	python3 tools/check_boss_budget.py
check: $(MISSION_BGP) $(BUILD_DIR)/pause_fixture.h $(if $(and $(CHARACTER_PACKS),$(wildcard $(BUILD_DIR)/chars/c0.bin)),check-characters) check-boss $(BUILD_DIR)/lspc_fixture.h $(BUILD_DIR)/m3ledge_fixture.h $(BUILD_DIR)/train_fixture.h $(BUILD_DIR)/mission_flow_fixture.h $(BUILD_DIR)/wipe_fixture.h $(BUILD_DIR)/end_letters_fixture.h $(BUILD_DIR)/start_letters_fixture.h $(TALLY_HEADERS) $(BUILD_DIR)/continue_sprites_fixture.h $(BUILD_DIR)/landing_fixture.h $(BUILD_DIR)/smoke_fixture.h $(BUILD_DIR)/descent_fixture.h $(BUILD_DIR)/hmuzzle_fixture.h $(BUILD_DIR)/fatmuzzle_fixture.h $(BUILD_DIR)/blood_fixture.h $(BUILD_DIR)/camera_fixture.h $(BUILD_DIR)/robe_fixture.h $(FAMILY_HEADERS) $(BUILD_DIR)/soldier_ai_tables.h $(BUILD_DIR)/region_rom.h $(BUILD_DIR)/throw_tables.h $(BUILD_DIR)/retro25d/retro_assets.h $(BUILD_DIR)/scene_meta.h $(BUILD_DIR)/soldier_tick_fixture.h $(BUILD_DIR)/soldier_death_tick_fixture.h $(BUILD_DIR)/soldier_script_fixture.h $(BUILD_DIR)/soldier_world_fixture.h $(BUILD_DIR)/kernel_fixture.h $(BUILD_DIR)/m2_stairs_fixture.h $(BUILD_DIR)/m3_map_fixture.h $(BUILD_DIR)/attack_state_fixture.h $(BUILD_DIR)/locomotion_fixture.h $(BUILD_DIR)/object_fixture.h $(BUILD_DIR)/throw_fixture.h $(BUILD_DIR)/attack_fixture.h $(BUILD_DIR)/cull_fixture.h $(BUILD_DIR)/action_fixture.h $(BUILD_DIR)/state_fixture.h $(BUILD_DIR)/mission_collision.h $(BUILD_DIR)/vehicle_anim.h $(BUILD_DIR)/vehicle_wreckage.h $(BUILD_DIR)/soldier_fixture.h $(BUILD_DIR)/rng_fixture.h $(BUILD_DIR)/animation_fixture.h $(BUILD_DIR)/contact_fixture.h $(BUILD_DIR)/contact_tables.h $(BUILD_DIR)/control_fixture.h $(BUILD_DIR)/control_tables.h
	sh tools/test_marco_inputs_sum.sh
	# B43: every VADPCM pack decodes to its source (synthetic always; private packs when present).
	python3 tools/test_wav64_vadpcm.py
	# The Mission 2 export (private) carries the Mission 1 sprites: the
	# player's weapons and effects (BUG_MATRIX B29: the beam of the laser).
	# Mission 3's export (step 1, docs/MISION3.md) carries them too.
	for m in $(wildcard filesystem/mission2/rest-native.json filesystem/mission3/rest-native.json); do python3 tools/check_mission_sprites.py filesystem/rest-native.json $$m || exit 1; done
	mkdir -p $(BUILD_DIR)/host
	$(HOST_CC) $(HOST_CFLAGS) -I$(BUILD_DIR) tools/test_gameplay.c src/gameplay.c src/neo_pause.c src/msx_sfx.c src/neo_collision.c src/neo_vehicle.c $(WORLD_SRCS) -o $(BUILD_DIR)/host/gameplay
	$(HOST_CC) $(HOST_CFLAGS) -I$(BUILD_DIR) tools/test_neo_collision.c src/neo_collision.c -o $(BUILD_DIR)/host/collision
	$(HOST_CC) $(HOST_CFLAGS) -I$(BUILD_DIR) tools/test_neo_mission.c $(WORLD_SRCS) src/neo_collision.c -o $(BUILD_DIR)/host/mission
	$(BUILD_DIR)/host/mission
	$(HOST_CC) $(HOST_CFLAGS) -I$(BUILD_DIR) tools/test_neo_train.c src/neo_train.c -o $(BUILD_DIR)/host/train
	$(BUILD_DIR)/host/train
	python3 tools/test_train_grid.py $(or $(MSX_ROM_ZIP),mslugx.zip)
	$(HOST_CC) $(HOST_CFLAGS) -I$(BUILD_DIR) -Isrc tools/test_neo_lspc.c -o $(BUILD_DIR)/host/lspc
	$(BUILD_DIR)/host/lspc
	$(HOST_CC) $(HOST_CFLAGS) -I$(BUILD_DIR) tools/test_neo_wipe.c src/neo_wipe.c $(filter-out src/neo_wipe.c,$(WORLD_SRCS)) src/neo_collision.c -o $(BUILD_DIR)/host/wipe
	$(BUILD_DIR)/host/wipe
	$(HOST_CC) $(HOST_CFLAGS) -I$(BUILD_DIR) tools/test_marco_muzzle.c $(WORLD_SRCS) src/neo_collision.c -o $(BUILD_DIR)/host/muzzle
	$(BUILD_DIR)/host/muzzle
	$(HOST_CC) $(HOST_CFLAGS) -I$(BUILD_DIR) tools/test_fat_muzzle.c $(WORLD_SRCS) src/neo_collision.c -o $(BUILD_DIR)/host/fatmuzzle
	$(BUILD_DIR)/host/fatmuzzle
	$(BUILD_DIR)/host/gameplay > $(BUILD_DIR)/host/gameplay.out; s=$$?; cat $(BUILD_DIR)/host/gameplay.out; exit $$s
	# Determinism: the same test built at -O2 prints the same results, and
	# every release seed run alone (in another order) prints its line again.
	$(HOST_CC) $(HOST_CFLAGS) -O2 -I$(BUILD_DIR) tools/test_gameplay.c src/gameplay.c src/neo_pause.c src/msx_sfx.c src/neo_collision.c src/neo_vehicle.c $(WORLD_SRCS) -o $(BUILD_DIR)/host/gameplay-O2
	$(BUILD_DIR)/host/gameplay-O2 > $(BUILD_DIR)/host/gameplay-O2.out
	diff $(BUILD_DIR)/host/gameplay.out $(BUILD_DIR)/host/gameplay-O2.out
	$(BUILD_DIR)/host/gameplay-O2 release 4 3 2 1 | sort > $(BUILD_DIR)/host/gameplay-seeds.out
	grep '^release run' $(BUILD_DIR)/host/gameplay.out | sort | diff - $(BUILD_DIR)/host/gameplay-seeds.out
	@echo "PASS: gameplay determinism (-O2 output identical, each release seed alone identical)"
	# Diagnostic mode (src/msx_diag.h): the same test with MSX_DIAG=1 (pool
	# hooks and list checks compiled in) prints the same results.
	$(HOST_CC) $(HOST_CFLAGS) -O2 -DMSX_DIAG=1 -I$(BUILD_DIR) tools/test_gameplay.c src/gameplay.c src/neo_pause.c src/msx_sfx.c src/neo_collision.c src/neo_vehicle.c src/msx_diag_core.c $(WORLD_SRCS) -o $(BUILD_DIR)/host/gameplay-diag
	$(BUILD_DIR)/host/gameplay-diag > $(BUILD_DIR)/host/gameplay-diag.out 2> $(BUILD_DIR)/host/gameplay-diag.err
	diff $(BUILD_DIR)/host/gameplay.out $(BUILD_DIR)/host/gameplay-diag.out
	@echo "PASS: MSX_DIAG=1 gameplay output identical (diagnostic hooks only read the simulation)"
	$(HOST_CC) $(HOST_CFLAGS) -I$(BUILD_DIR) tools/script_native.c $(WORLD_SRCS) src/neo_collision.c -o $(BUILD_DIR)/host/script-native
	python3 tools/audit_script_events.py $(BUILD_DIR)/host/script-native
	python3 tools/audit_script_events.py $(BUILD_DIR)/host/script-native --script data/mission2_script.json
	python3 tools/audit_script_events.py $(BUILD_DIR)/host/script-native --script data/mission3_script.json
	python3 tools/audit_script_events.py $(BUILD_DIR)/host/script-native --script data/mission4_script.json
	python3 tools/audit_script_events.py $(BUILD_DIR)/host/script-native --script data/mission5_script.json
	python3 tools/audit_script_events.py $(BUILD_DIR)/host/script-native --script data/mission6_script.json
	python3 tools/check_scene_budget.py
	$(HOST_CC) $(HOST_CFLAGS) -I$(BUILD_DIR) tools/test_scene_cull.c src/scene_cull.c src/retro25d.c -lm -o $(BUILD_DIR)/host/scene-cull
	$(BUILD_DIR)/host/scene-cull
	$(HOST_CC) $(HOST_CFLAGS) tools/test_scene_layer_y.c -o $(BUILD_DIR)/host/scene-layer-y
	$(BUILD_DIR)/host/scene-layer-y
	$(HOST_CC) $(HOST_CFLAGS) tools/test_marco_thinfire.c src/marco_thinfire.c -o $(BUILD_DIR)/host/marco-thinfire
	$(BUILD_DIR)/host/marco-thinfire data/thinfire_fixture.txt
	python3 tools/check_thin_fire.py
	$(HOST_CC) $(HOST_CFLAGS) -I$(BUILD_DIR) tools/test_scene_strips.c src/scene_strips.c src/scene_cull.c -lm -o $(BUILD_DIR)/host/scene-strips
	$(BUILD_DIR)/host/scene-strips
	$(HOST_CC) $(HOST_CFLAGS) -I$(BUILD_DIR) tools/test_scene_pack.c -o $(BUILD_DIR)/host/scene-pack
	$(BUILD_DIR)/host/scene-pack romfs/scene.pack
	HOST_CC="$(HOST_CC)" python3 tools/test_pack_compression.py
	# MSXL (docs/COMPRESION.md): every sample back, synthetic always, the
	# private raw WAV64s and the cartridge's packed files when present.
	$(HOST_CC) $(HOST_CFLAGS) -O2 -DMSX_PCM_STATS=1 -Itools/host_msxl tools/test_msxl.c src/msx_pcm.c -lm -o $(BUILD_DIR)/host/test-msxl
	$(BUILD_DIR)/host/test-msxl $(BUILD_DIR)/host/msxl-work.tmp $(wildcard filesystem/cinematics/*.wav64 filesystem/logo/*.wav64 filesystem/audio/*.wav64) --packed $$(for f in $(BUILD_DIR)/game-romfs/*.wav64; do for s in filesystem/cinematics filesystem/logo; do if [ -f "$$f" ] && [ -f $$s/$$(basename $$f) ]; then echo $$f $$s/$$(basename $$f); fi; done; done)
	# Pre-rendered columns equal byte for byte stored once (docs/COMPRESION.md, Duplicados).
	python3 tools/test_bgp_share.py $(if $(MISSION_PACKS),$(PRERENDER_SPECS))
	# The cartridge's movies (optimal LZ4 parser, docs/COMPRESION.md §5)
	# against their v1 sources through the console's in-place decoder.
	$(HOST_CC) $(HOST_CFLAGS) -O2 tools/test_cinematic_stream.c -o $(BUILD_DIR)/host/cinematic-stream
	for f in $(BUILD_DIR)/game-romfs/*.msxc; do [ -f "$$f" ] || continue; b=$$(basename $$f); s=filesystem/cinematics/$$b; [ $$b = title.msxc ] && s=filesystem/cinematics/title-motion.msxc; $(BUILD_DIR)/host/cinematic-stream $$s $$f || exit 1; done
	python3 tools/test_marco_sprite_edges.py
	$(BUILD_DIR)/host/collision
	$(HOST_CC) $(HOST_CFLAGS) -I$(BUILD_DIR) tools/test_neo_vehicle.c src/neo_vehicle.c src/neo_collision.c src/gameplay.c src/msx_sfx.c $(WORLD_SRCS) -o $(BUILD_DIR)/host/vehicle
	$(BUILD_DIR)/host/vehicle
	$(HOST_CC) $(HOST_CFLAGS) -I$(BUILD_DIR) tools/test_neo_contact.c src/neo_contact.c src/neo_collision.c src/neo_animation.c -o $(BUILD_DIR)/host/contact
	$(BUILD_DIR)/host/contact
	$(HOST_CC) $(HOST_CFLAGS) -I$(BUILD_DIR) tools/test_neo_control.c src/neo_control.c src/neo_rng.c src/neo_contact.c src/neo_collision.c src/neo_animation.c -o $(BUILD_DIR)/host/control
	$(BUILD_DIR)/host/control
	$(HOST_CC) $(HOST_CFLAGS) -I$(BUILD_DIR) tools/test_neo_state.c src/neo_cull.c src/neo_soldier_state.c src/neo_soldier_action.c src/neo_soldier.c src/neo_control.c src/neo_rng.c src/neo_contact.c src/neo_collision.c src/neo_animation.c -o $(BUILD_DIR)/host/state
	$(BUILD_DIR)/host/state
	$(HOST_CC) $(HOST_CFLAGS) -I$(BUILD_DIR) tools/test_neo_throw.c src/neo_throw.c src/neo_target.c src/neo_contact.c src/neo_collision.c -o $(BUILD_DIR)/host/throw
	$(BUILD_DIR)/host/throw
	$(HOST_CC) $(HOST_CFLAGS) -I$(BUILD_DIR) tools/test_neo_attack.c src/neo_hit.c src/neo_soldier_state.c src/neo_soldier.c src/neo_soldier_action.c src/neo_rng.c src/neo_throw_actor.c src/neo_cull.c src/neo_animation.c src/neo_soldier_effects.c src/neo_throw.c src/neo_target.c src/neo_contact.c src/neo_collision.c -o $(BUILD_DIR)/host/attack
	$(BUILD_DIR)/host/attack
	$(HOST_CC) $(HOST_CFLAGS) -I$(BUILD_DIR) tools/test_neo_object.c src/neo_throw_actor.c src/neo_animation.c src/neo_throw.c src/neo_target.c src/neo_contact.c src/neo_collision.c src/neo_cull.c -o $(BUILD_DIR)/host/object
	$(BUILD_DIR)/host/object
	$(HOST_CC) $(HOST_CFLAGS) -I$(BUILD_DIR) tools/test_neo_locomotion.c src/neo_soldier_state.c src/neo_soldier.c src/neo_soldier_action.c src/neo_rng.c src/neo_contact.c src/neo_collision.c src/neo_animation.c -o $(BUILD_DIR)/host/locomotion
	$(BUILD_DIR)/host/locomotion
	$(HOST_CC) $(HOST_CFLAGS) -I$(BUILD_DIR) tools/test_neo_kernel.c src/neo_kernel.c src/neo_collision.c -o $(BUILD_DIR)/host/kernel
	$(BUILD_DIR)/host/kernel
	$(HOST_CC) $(HOST_CFLAGS) -I$(BUILD_DIR) tools/test_m2_stairs.c src/neo_kernel.c src/neo_collision.c -o $(BUILD_DIR)/host/m2stairs
	$(BUILD_DIR)/host/m2stairs
	$(HOST_CC) $(HOST_CFLAGS) -I$(BUILD_DIR) tools/test_m3_map.c src/neo_kernel.c src/neo_collision.c -o $(BUILD_DIR)/host/m3map
	$(BUILD_DIR)/host/m3map romfs/mission3.pack
	$(HOST_CC) $(HOST_CFLAGS) -I$(BUILD_DIR) tools/test_m3_camera_start.c src/neo_collision.c src/msx_sfx.c $(WORLD_SRCS) -o $(BUILD_DIR)/host/m3camstart
	$(BUILD_DIR)/host/m3camstart romfs/mission3.pack
	$(HOST_CC) $(HOST_CFLAGS) -I$(BUILD_DIR) tools/test_m3_camera_bridge.c src/neo_collision.c src/msx_sfx.c $(WORLD_SRCS) -o $(BUILD_DIR)/host/m3cambridge
	$(BUILD_DIR)/host/m3cambridge romfs/mission3.pack
	python3 tools/compile_m4_map_fixture.py data/m4_map_fixture.json.gz $(BUILD_DIR)/m4_map_fixture.h
	$(HOST_CC) $(HOST_CFLAGS) -I$(BUILD_DIR) tools/test_m4_map.c src/neo_collision.c -o $(BUILD_DIR)/host/m4map
	$(BUILD_DIR)/host/m4map romfs/mission4.pack
	python3 tools/compile_camera_fixture.py data/m4_camera_fixture.json.gz $(BUILD_DIR)/m4_camera_fixture.h
	$(HOST_CC) $(HOST_CFLAGS) -I$(BUILD_DIR) tools/test_m4_camera.c src/neo_camera.c src/neo_train.c -o $(BUILD_DIR)/host/m4camera
	$(BUILD_DIR)/host/m4camera romfs/mission4.pack
	python3 tools/compile_m5_map_fixture.py data/m5_map_fixture.json.gz $(BUILD_DIR)/m5_map_fixture.h
	$(HOST_CC) $(HOST_CFLAGS) -I$(BUILD_DIR) tools/test_m5_map.c src/neo_collision.c -o $(BUILD_DIR)/host/m5map
	$(BUILD_DIR)/host/m5map romfs/mission5.pack
	mkdir -p $(BUILD_DIR)/m5tas
	python3 tools/compile_m5_map_fixture.py data/m5tas_map_fixture.json.gz $(BUILD_DIR)/m5tas/m5_map_fixture.h
	$(HOST_CC) $(HOST_CFLAGS) -I$(BUILD_DIR)/m5tas -I$(BUILD_DIR) '-DM5_SOURCE="TAS, no memory writes"' tools/test_m5_map.c src/neo_collision.c -o $(BUILD_DIR)/host/m5tasmap
	$(BUILD_DIR)/host/m5tasmap romfs/mission5.pack
	mkdir -p $(BUILD_DIR)/m5s1 $(BUILD_DIR)/m5tas1
	python3 tools/compile_camera_fixture.py data/m5s1_camera_fixture.json.gz $(BUILD_DIR)/m5s1/m5_camera_fixture.h
	$(HOST_CC) $(HOST_CFLAGS) -I$(BUILD_DIR)/m5s1 -I$(BUILD_DIR) -DM5_STAGE=0 tools/test_m5_camera.c src/neo_camera.c src/neo_train.c -o $(BUILD_DIR)/host/m5camera1
	$(BUILD_DIR)/host/m5camera1 romfs/mission5.pack
	python3 tools/compile_camera_fixture.py data/m5tas1_camera_fixture.json.gz $(BUILD_DIR)/m5tas1/m5_camera_fixture.h
	$(HOST_CC) $(HOST_CFLAGS) -I$(BUILD_DIR)/m5tas1 -I$(BUILD_DIR) -DM5_STAGE=0 '-DM5_SOURCE="TAS, no memory writes"' tools/test_m5_camera.c src/neo_camera.c src/neo_train.c -o $(BUILD_DIR)/host/m5tascamera1
	$(BUILD_DIR)/host/m5tascamera1 romfs/mission5.pack
	mkdir -p $(BUILD_DIR)/m5s2 $(BUILD_DIR)/m5tas2
	python3 tools/compile_camera_fixture.py data/m5s2_camera_fixture.json.gz $(BUILD_DIR)/m5s2/m5_camera_fixture.h
	$(HOST_CC) $(HOST_CFLAGS) -I$(BUILD_DIR)/m5s2 -I$(BUILD_DIR) -DM5_STAGE=1 tools/test_m5_camera.c src/neo_camera.c src/neo_train.c -o $(BUILD_DIR)/host/m5camera2
	$(BUILD_DIR)/host/m5camera2 romfs/mission5.pack
	python3 tools/compile_camera_fixture.py data/m5tas2_camera_fixture.json.gz $(BUILD_DIR)/m5tas2/m5_camera_fixture.h
	$(HOST_CC) $(HOST_CFLAGS) -I$(BUILD_DIR)/m5tas2 -I$(BUILD_DIR) -DM5_STAGE=1 '-DM5_SOURCE="TAS, no memory writes"' tools/test_m5_camera.c src/neo_camera.c src/neo_train.c -o $(BUILD_DIR)/host/m5tascamera2
	$(BUILD_DIR)/host/m5tascamera2 romfs/mission5.pack
	mkdir -p $(BUILD_DIR)/m5s3 $(BUILD_DIR)/m5tas3
	python3 tools/compile_camera_fixture.py data/m5s3_camera_fixture.json.gz $(BUILD_DIR)/m5s3/m5_camera_fixture.h
	$(HOST_CC) $(HOST_CFLAGS) -I$(BUILD_DIR)/m5s3 -I$(BUILD_DIR) -DM5_STAGE=2 tools/test_m5_camera.c src/neo_camera.c src/neo_train.c -o $(BUILD_DIR)/host/m5camera3
	$(BUILD_DIR)/host/m5camera3 romfs/mission5.pack
	python3 tools/compile_camera_fixture.py data/m5tas3_camera_fixture.json.gz $(BUILD_DIR)/m5tas3/m5_camera_fixture.h
	$(HOST_CC) $(HOST_CFLAGS) -I$(BUILD_DIR)/m5tas3 -I$(BUILD_DIR) -DM5_STAGE=2 '-DM5_SOURCE="TAS, no memory writes"' tools/test_m5_camera.c src/neo_camera.c src/neo_train.c -o $(BUILD_DIR)/host/m5tascamera3
	$(BUILD_DIR)/host/m5tascamera3 romfs/mission5.pack
	python3 tools/compile_m6_map_fixture.py data/m6_map_fixture.json.gz $(BUILD_DIR)/m6_map_fixture.h
	$(HOST_CC) $(HOST_CFLAGS) -I$(BUILD_DIR) tools/test_m6_map.c src/neo_collision.c -o $(BUILD_DIR)/host/m6map
	$(BUILD_DIR)/host/m6map romfs/mission6.pack
	mkdir -p $(BUILD_DIR)/m6tas
	python3 tools/compile_m6_map_fixture.py data/m6tas_map_fixture.json.gz $(BUILD_DIR)/m6tas/m6_map_fixture.h
	$(HOST_CC) $(HOST_CFLAGS) -I$(BUILD_DIR)/m6tas -I$(BUILD_DIR) '-DM6_SOURCE="TAS, no memory writes"' tools/test_m6_map.c src/neo_collision.c -o $(BUILD_DIR)/host/m6tasmap
	$(BUILD_DIR)/host/m6tasmap romfs/mission6.pack
	mkdir -p $(BUILD_DIR)/m6s1 $(BUILD_DIR)/m6tas1
	python3 tools/compile_camera_fixture.py data/m6s1_camera_fixture.json.gz $(BUILD_DIR)/m6s1/m6_camera_fixture.h
	$(HOST_CC) $(HOST_CFLAGS) -I$(BUILD_DIR)/m6s1 -I$(BUILD_DIR) -DM6_STAGE=0 tools/test_m6_camera.c src/neo_camera.c src/neo_train.c -o $(BUILD_DIR)/host/m6camera1
	$(BUILD_DIR)/host/m6camera1 romfs/mission6.pack
	python3 tools/compile_camera_fixture.py data/m6tas1_camera_fixture.json.gz $(BUILD_DIR)/m6tas1/m6_camera_fixture.h
	$(HOST_CC) $(HOST_CFLAGS) -I$(BUILD_DIR)/m6tas1 -I$(BUILD_DIR) -DM6_STAGE=0 '-DM6_SOURCE="TAS, no memory writes"' tools/test_m6_camera.c src/neo_camera.c src/neo_train.c -o $(BUILD_DIR)/host/m6tascamera1
	$(BUILD_DIR)/host/m6tascamera1 romfs/mission6.pack
	mkdir -p $(BUILD_DIR)/m6s2 $(BUILD_DIR)/m6tas2
	python3 tools/compile_camera_fixture.py data/m6s2_camera_fixture.json.gz $(BUILD_DIR)/m6s2/m6_camera_fixture.h
	$(HOST_CC) $(HOST_CFLAGS) -I$(BUILD_DIR)/m6s2 -I$(BUILD_DIR) -DM6_STAGE=1 tools/test_m6_camera.c src/neo_camera.c src/neo_train.c -o $(BUILD_DIR)/host/m6camera2
	$(BUILD_DIR)/host/m6camera2 romfs/mission6.pack
	python3 tools/compile_camera_fixture.py data/m6tas2_camera_fixture.json.gz $(BUILD_DIR)/m6tas2/m6_camera_fixture.h
	$(HOST_CC) $(HOST_CFLAGS) -I$(BUILD_DIR)/m6tas2 -I$(BUILD_DIR) -DM6_STAGE=1 '-DM6_SOURCE="TAS, no memory writes"' tools/test_m6_camera.c src/neo_camera.c src/neo_train.c -o $(BUILD_DIR)/host/m6tascamera2
	$(BUILD_DIR)/host/m6tascamera2 romfs/mission6.pack
	mkdir -p $(BUILD_DIR)/m6s3 $(BUILD_DIR)/m6tas3
	python3 tools/compile_camera_fixture.py data/m6s3_camera_fixture.json.gz $(BUILD_DIR)/m6s3/m6_camera_fixture.h
	$(HOST_CC) $(HOST_CFLAGS) -I$(BUILD_DIR)/m6s3 -I$(BUILD_DIR) -DM6_STAGE=2 tools/test_m6_camera.c src/neo_camera.c src/neo_train.c -o $(BUILD_DIR)/host/m6camera3
	$(BUILD_DIR)/host/m6camera3 romfs/mission6.pack
	python3 tools/compile_camera_fixture.py data/m6tas3_camera_fixture.json.gz $(BUILD_DIR)/m6tas3/m6_camera_fixture.h
	$(HOST_CC) $(HOST_CFLAGS) -I$(BUILD_DIR)/m6tas3 -I$(BUILD_DIR) -DM6_STAGE=2 '-DM6_SOURCE="TAS, no memory writes"' tools/test_m6_camera.c src/neo_camera.c src/neo_train.c -o $(BUILD_DIR)/host/m6tascamera3
	$(BUILD_DIR)/host/m6tascamera3 romfs/mission6.pack
	# B14: the ground sweep's wall stop at the slot-7 object of Mission 2 stage 2 (MAME m2wall).
	$(HOST_CC) $(HOST_CFLAGS) -I$(BUILD_DIR) tools/test_m2_wall.c src/neo_collision.c src/msx_sfx.c $(WORLD_SRCS) -o $(BUILD_DIR)/host/m2wall
	$(BUILD_DIR)/host/m2wall
	$(HOST_CC) $(HOST_CFLAGS) -I$(BUILD_DIR) tools/test_neo_soldier_ai.c src/neo_soldier_ai.c src/neo_blood.c src/neo_target.c src/neo_kernel.c src/neo_collision.c src/neo_contact.c src/neo_control.c src/neo_rng.c src/neo_animation.c src/neo_soldier.c src/neo_soldier_action.c src/neo_hit.c src/neo_cull.c -o $(BUILD_DIR)/host/soldier_ai
	$(BUILD_DIR)/host/soldier_ai
	$(HOST_CC) $(HOST_CFLAGS) -I$(BUILD_DIR) -DTICK_FIXTURE_HEADER='"soldier_death_tick_fixture.h"' -DTICK_MAX_EXTERNAL=0 tools/test_neo_soldier_ai.c src/neo_soldier_ai.c src/neo_blood.c src/neo_target.c src/neo_kernel.c src/neo_collision.c src/neo_contact.c src/neo_control.c src/neo_rng.c src/neo_animation.c src/neo_soldier.c src/neo_soldier_action.c src/neo_hit.c src/neo_cull.c -o $(BUILD_DIR)/host/soldier_death
	$(BUILD_DIR)/host/soldier_death
	$(HOST_CC) $(HOST_CFLAGS) -I$(BUILD_DIR) tools/test_neo_script.c src/neo_script.c src/neo_soldier_ai.c src/neo_blood.c src/neo_target.c src/neo_kernel.c src/neo_collision.c src/neo_contact.c src/neo_control.c src/neo_rng.c src/neo_animation.c src/neo_soldier.c src/neo_soldier_action.c src/neo_hit.c src/neo_cull.c -o $(BUILD_DIR)/host/script
	$(BUILD_DIR)/host/script
	$(HOST_CC) $(HOST_CFLAGS) -I$(BUILD_DIR) tools/test_neo_world.c $(WORLD_SRCS) src/neo_collision.c -o $(BUILD_DIR)/host/world
	$(BUILD_DIR)/host/world
	$(HOST_CC) $(HOST_CFLAGS) -I$(BUILD_DIR) tools/test_b69.c $(WORLD_SRCS) src/neo_collision.c -o $(BUILD_DIR)/host/b69
	$(BUILD_DIR)/host/b69
	python3 tools/compile_m6bridge_fixture.py
	$(HOST_CC) $(HOST_CFLAGS) -I$(BUILD_DIR) -Isrc tools/test_m6_bridge.c src/neo_m6bridge.c -o $(BUILD_DIR)/host/m6bridge
	$(BUILD_DIR)/host/m6bridge
	$(HOST_CC) $(HOST_CFLAGS) -I$(BUILD_DIR) -Isrc tools/test_m6_order.c src/neo_train.c -o $(BUILD_DIR)/host/m6order
	$(BUILD_DIR)/host/m6order
	# The bridge 10D7B6 in the collision kernel: its ring (map 0BC6E6, collision 45FC2A) against MAME (captures-m6/fam/L + s9).
	$(HOST_CC) $(HOST_CFLAGS) -I$(BUILD_DIR) -Isrc tools/test_m6_ring.c src/neo_m6bridge.c -o $(BUILD_DIR)/host/m6ring
	$(BUILD_DIR)/host/m6ring
	$(HOST_CC) $(HOST_CFLAGS) -I$(BUILD_DIR) -DMSX_BLOOD=$(MSX_BLOOD) tools/test_neo_blood.c $(WORLD_SRCS) src/neo_collision.c -o $(BUILD_DIR)/host/blood
	$(BUILD_DIR)/host/blood
	$(HOST_CC) $(HOST_CFLAGS) -I$(BUILD_DIR) tools/test_neo_m3props.c $(WORLD_SRCS) src/neo_collision.c -o $(BUILD_DIR)/host/m3props
	$(BUILD_DIR)/host/m3props
	$(HOST_CC) $(HOST_CFLAGS) -I$(BUILD_DIR) tools/test_neo_m3push.c $(WORLD_SRCS) src/neo_collision.c -o $(BUILD_DIR)/host/m3push
	$(BUILD_DIR)/host/m3push
	$(HOST_CC) $(HOST_CFLAGS) -I$(BUILD_DIR) tools/test_neo_camera.c src/neo_camera.c -o $(BUILD_DIR)/host/camera
	$(BUILD_DIR)/host/camera
	$(HOST_CC) $(HOST_CFLAGS) -I$(BUILD_DIR) tools/test_neo_rest.c src/neo_rest.c src/neo_dome.c src/neo_barrel.c src/neo_wagon.c src/neo_arab.c src/neo_prisoner.c src/neo_item.c src/neo_armored.c src/neo_secret.c src/neo_swords.c src/neo_rocket.c src/neo_heli.c src/neo_camel.c src/neo_porter.c src/neo_civil.c src/neo_slug.c src/neo_tank.c src/neo_m3veh.c src/neo_slugnoid.c src/neo_tread.c src/neo_vulcan.c src/neo_vslot.c src/neo_boss.c src/neo_gactor.c src/neo_region.c src/neo_goarrow.c src/neo_rebel2.c src/neo_rebel3.c src/neo_m2creature.c src/neo_gate.c src/neo_crate.c src/neo_m2props.c src/neo_m3props.c src/neo_m4props.c src/neo_m4veh.c src/neo_m4boss.c src/neo_m5props.c src/neo_m6props.c src/neo_m5enemy.c src/neo_m5train.c src/neo_m5mech.c src/neo_m5sewer.c src/neo_m3enemy.c src/neo_drone.c src/neo_m2traps.c src/neo_hostage.c src/neo_m2boss.c src/neo_m3mid.c src/neo_m3boss.c src/neo_truck.c src/neo_effects.c src/neo_flyer.c src/neo_props.c src/neo_kernel.c src/neo_contact.c src/neo_control.c src/neo_rng.c src/neo_animation.c src/neo_cull.c src/neo_collision.c src/neo_soldier_ai.c src/neo_blood.c src/neo_soldier.c src/neo_soldier_action.c src/neo_hit.c src/neo_target.c src/marco_ammo.c src/neo_bullet.c src/neo_chaser.c src/neo_pweapon.c -o $(BUILD_DIR)/host/rest
	$(BUILD_DIR)/host/rest
	$(HOST_CC) $(HOST_CFLAGS) -I$(BUILD_DIR) -DEND_HEADER='"end_letters_fixture.h"' -DEND_NAME=end_letters tools/test_neo_end.c $(filter-out src/neo_world.c src/neo_script.c src/neo_camera.c,$(WORLD_SRCS)) src/neo_collision.c -o $(BUILD_DIR)/host/end_letters
	$(BUILD_DIR)/host/end_letters
	$(HOST_CC) $(HOST_CFLAGS) -I$(BUILD_DIR) -DEND_HEADER='"start_letters_fixture.h"' -DEND_NAME=start_letters tools/test_neo_end.c $(filter-out src/neo_world.c src/neo_script.c src/neo_camera.c,$(WORLD_SRCS)) src/neo_collision.c -o $(BUILD_DIR)/host/start_letters
	$(BUILD_DIR)/host/start_letters
	$(HOST_CC) $(HOST_CFLAGS) -I$(BUILD_DIR) tools/test_neo_continue_sprites.c $(filter-out src/neo_world.c src/neo_script.c src/neo_camera.c,$(WORLD_SRCS)) src/neo_collision.c -o $(BUILD_DIR)/host/continue_sprites
	$(BUILD_DIR)/host/continue_sprites
	$(HOST_CC) $(HOST_CFLAGS) -I$(BUILD_DIR) tools/test_program_file.c src/neo_progstore.c -o $(BUILD_DIR)/host/program_file
	$(BUILD_DIR)/host/program_file $(BUILD_DIR)/programs.bin
	$(HOST_CC) $(HOST_CFLAGS) -I$(BUILD_DIR) tools/test_neo_landing.c $(filter-out src/neo_world.c src/neo_script.c src/neo_camera.c,$(WORLD_SRCS)) src/neo_collision.c -o $(BUILD_DIR)/host/landing
	$(BUILD_DIR)/host/landing
	$(HOST_CC) $(HOST_CFLAGS) -I$(BUILD_DIR) tools/test_neo_smoke.c $(filter-out src/neo_world.c src/neo_script.c src/neo_camera.c,$(WORLD_SRCS)) src/neo_collision.c -o $(BUILD_DIR)/host/smoke
	$(BUILD_DIR)/host/smoke
	$(HOST_CC) $(HOST_CFLAGS) -I$(BUILD_DIR) tools/test_marco_descent.c src/marco_controller.c src/marco_animation.c $(filter-out src/neo_world.c src/neo_script.c src/neo_camera.c,$(WORLD_SRCS)) src/neo_collision.c -o $(BUILD_DIR)/host/descent
	$(BUILD_DIR)/host/descent
	for t in $(TALLIES); do $(HOST_CC) $(HOST_CFLAGS) -I$(BUILD_DIR) -DTALLY_HEADER="\"tally_$${t}_fixture.h\"" -DTALLY_NAME=tally_$$t tools/test_neo_tally.c $(filter-out src/neo_world.c src/neo_script.c src/neo_camera.c,$(WORLD_SRCS)) src/neo_collision.c -o $(BUILD_DIR)/host/tally_$$t && $(BUILD_DIR)/host/tally_$$t || exit 1; done
	$(HOST_CC) $(HOST_CFLAGS) -I$(BUILD_DIR) tools/test_neo_fuel.c $(filter-out src/neo_world.c src/neo_script.c src/neo_camera.c,$(WORLD_SRCS)) src/neo_collision.c -o $(BUILD_DIR)/host/fuel
	$(BUILD_DIR)/host/fuel
	for f in $(FAMILIES); do $(HOST_CC) $(HOST_CFLAGS) -I$(BUILD_DIR) -DFAMILY_HEADER="\"$${f}_fixture.h\"" -DFAMILY_NAME=$$f tools/test_neo_family.c $(filter-out src/neo_world.c src/neo_script.c src/neo_camera.c,$(WORLD_SRCS)) src/neo_collision.c -o $(BUILD_DIR)/host/family_$$f && $(BUILD_DIR)/host/family_$$f && rm -f $(BUILD_DIR)/host/family_$$f || exit 1; done
	$(HOST_CC) $(HOST_CFLAGS) -DRETRO_REAL_ASSETS -I$(BUILD_DIR)/retro25d tools/test_retro25d.c src/retro25d.c src/retro_events.c -lm -o $(BUILD_DIR)/host/retro25d
	$(BUILD_DIR)/host/retro25d
-include $(wildcard $(BUILD_DIR)/src/*.d $(BUILD_DIR)/diag/*.d $(BUILD_DIR)/tools/*.d $(BUILD_DIR)/release/*.d $(BUILD_DIR)/reference*/*.d $(BUILD_DIR)/m2ref*/*.d $(BUILD_DIR)/m3ref*/*.d $(BUILD_DIR)/m3rel/*.d)

# Internal whole-helper regression. Headers derive from private ROM journals.
$(BUILD_DIR)/tools/test_marco_steps.o: CC = $(N64_CC)
$(BUILD_DIR)/tools/test_marco_steps.o: CFLAGS = $(N64_CFLAGS) -MMD -Isrc
$(BUILD_DIR)/tools/test_marco_steps.o: $(BUILD_DIR)/marco_script.h $(BUILD_DIR)/marco_steps.h
$(BUILD_DIR)/marco-pair-regression.elf: $(BUILD_DIR)/tools/test_marco_steps.o $(BUILD_DIR)/src/marco_animation.o $(BUILD_DIR)/src/neo_animation.o
marco-pair-regression.z64: N64_ROM_TITLE = "MSX MARCO PAIR"
$(BUILD_DIR)/marco_script.h $(BUILD_DIR)/marco_steps.h:
	@echo "Compile private Marco scripts/helper journals before this internal target."
	@exit 1

$(BUILD_DIR)/tools/test_marco_controller.o: CC = $(N64_CC)
$(BUILD_DIR)/tools/test_marco_controller.o: CFLAGS = $(N64_CFLAGS) -MMD -Isrc -DMARCO_PRIVATE_CONTROLLER_FIXTURE
$(BUILD_DIR)/tools/test_marco_controller.o: $(BUILD_DIR)/marco_controller_private_fixture.h $(BUILD_DIR)/marco_script.h
$(BUILD_DIR)/marco-controller-regression.elf: $(BUILD_DIR)/tools/test_marco_controller.o $(BUILD_DIR)/src/marco_controller.o $(BUILD_DIR)/src/marco_animation.o $(BUILD_DIR)/src/neo_animation.o
marco-controller-regression.z64: N64_ROM_TITLE = "MSX MARCO CONTROL"
$(BUILD_DIR)/marco_controller_private_fixture.h:
	@echo "Run compile_marco_controller.py --graph PRIVATE_GRAPH --output build/marco_controller_private_fixture.h"
	@exit 1

$(BUILD_DIR)/tools/test_marco_coupled.o: tools/test_marco_steps.c $(BUILD_DIR)/marco_controller_private_fixture.h $(BUILD_DIR)/marco_script.h $(BUILD_DIR)/marco_steps.h $(BUILD_DIR)/marco_controller_timeline.h
	@mkdir -p $(dir $@)
	$(N64_CC) $(N64_CFLAGS) -MMD -Isrc -DMARCO_COUPLED_CONTROLLER -c $< -o $@
$(BUILD_DIR)/marco-coupled-regression.elf: $(BUILD_DIR)/tools/test_marco_coupled.o $(BUILD_DIR)/src/marco_controller.o $(BUILD_DIR)/src/marco_animation.o $(BUILD_DIR)/src/neo_animation.o
marco-coupled-regression.z64: N64_ROM_TITLE = "MSX MARCO COUPLED"
$(BUILD_DIR)/marco_controller_timeline.h:
	@echo "Compile the private combined journal with compile_marco_timeline.py"
	@exit 1

$(BUILD_DIR)/tools/test_marco_coupled_pixels.o: tools/test_marco_steps.c $(BUILD_DIR)/marco_controller_private_fixture.h $(BUILD_DIR)/marco_script.h $(BUILD_DIR)/marco_steps.h $(BUILD_DIR)/marco_controller_timeline.h $(BUILD_DIR)/marco_pixel_sequence.h
	@mkdir -p $(dir $@)
	$(N64_CC) $(N64_CFLAGS) -MMD -Isrc -DMARCO_COUPLED_CONTROLLER -DMARCO_RENDER_COUPLED -c $< -o $@
$(BUILD_DIR)/marco-coupled-pixels.elf: $(BUILD_DIR)/tools/test_marco_coupled_pixels.o $(BUILD_DIR)/src/marco_controller.o $(BUILD_DIR)/src/marco_animation.o $(BUILD_DIR)/src/neo_animation.o $(BUILD_DIR)/src/marco_renderer.o
$(BUILD_DIR)/marco-coupled-pixels.dfs: N64_MKDFS_ROOT = romfs
marco-coupled-pixels.z64: N64_ROM_TITLE = "MSX MARCO PIPELINE"
marco-coupled-pixels.z64: $(BUILD_DIR)/marco-coupled-pixels.dfs
$(BUILD_DIR)/marco_pixel_sequence.h:
	@echo "Compile audited SCB references with compile_marco_pixel_sequence.py"
	@exit 1

$(BUILD_DIR)/tools/test_marco_advance.o: tools/test_marco_steps.c $(BUILD_DIR)/marco-advance/marco_script.h $(BUILD_DIR)/marco-advance/marco_steps.h
	@mkdir -p $(dir $@)
	$(N64_CC) -I$(BUILD_DIR)/marco-advance $(N64_CFLAGS) -MMD -Isrc -c $< -o $@
$(BUILD_DIR)/marco-advance-regression.elf: $(BUILD_DIR)/tools/test_marco_advance.o $(BUILD_DIR)/src/marco_animation.o $(BUILD_DIR)/src/neo_animation.o
marco-advance-regression.z64: N64_ROM_TITLE = "MSX MARCO ADVANCE"
$(BUILD_DIR)/marco-advance/marco_script.h $(BUILD_DIR)/marco-advance/marco_steps.h:
	@echo "Compile the private advance helper journal in build/marco-advance first."
	@exit 1

$(BUILD_DIR)/tools/test_marco_advance_pixels.o: tools/test_marco_steps.c $(BUILD_DIR)/marco-advance/marco_controller_private_fixture.h $(BUILD_DIR)/marco-advance/marco_script.h $(BUILD_DIR)/marco-advance/marco_steps.h $(BUILD_DIR)/marco-advance/marco_controller_timeline.h $(BUILD_DIR)/marco-advance/marco_pixel_sequence.h
	@mkdir -p $(dir $@)
	$(N64_CC) -I$(BUILD_DIR)/marco-advance $(N64_CFLAGS) -MMD -Isrc -DMARCO_COUPLED_CONTROLLER -DMARCO_RENDER_COUPLED -DMARCO_PUBLICATION_QUEUE -c $< -o $@
$(BUILD_DIR)/marco-advance-pixels.elf: $(BUILD_DIR)/tools/test_marco_advance_pixels.o $(BUILD_DIR)/src/marco_controller.o $(BUILD_DIR)/src/marco_animation.o $(BUILD_DIR)/src/neo_animation.o $(BUILD_DIR)/src/marco_renderer.o
$(BUILD_DIR)/marco-advance-pixels.dfs: N64_MKDFS_ROOT = romfs
marco-advance-pixels.z64: N64_ROM_TITLE = "MSX MARCO COMBAT"
marco-advance-pixels.z64: $(BUILD_DIR)/marco-advance-pixels.dfs

$(BUILD_DIR)/tools/test_marco_upper_native.o: tools/test_marco_upper.c $(BUILD_DIR)/marco_upper_fixture.h $(BUILD_DIR)/marco-upper-native/marco_upper_native.h $(BUILD_DIR)/marco-upper-native/marco_script.h
	@mkdir -p $(dir $@)
	$(N64_CC) -I$(BUILD_DIR)/marco-upper-native $(N64_CFLAGS) -MMD -Isrc -DMARCO_UPPER_NATIVE_ANIMATION -c $< -o $@
$(BUILD_DIR)/marco-upper-regression.elf: $(BUILD_DIR)/tools/test_marco_upper_native.o $(BUILD_DIR)/src/marco_upper.o $(BUILD_DIR)/src/marco_controller.o $(BUILD_DIR)/src/marco_animation.o $(BUILD_DIR)/src/neo_animation.o
marco-upper-regression.z64: N64_ROM_TITLE = "MSX MARCO UPPER"
$(BUILD_DIR)/tools/test_marco_upper_combat.o: tools/test_marco_upper.c $(BUILD_DIR)/marco-upper-combat/marco_upper_fixture.h $(BUILD_DIR)/marco-upper-combat/marco_upper_native.h $(BUILD_DIR)/marco-upper-combat/marco_script.h
	@mkdir -p $(dir $@)
	$(N64_CC) -I$(BUILD_DIR)/marco-upper-combat $(N64_CFLAGS) -MMD -Isrc -DMARCO_UPPER_NATIVE_ANIMATION -c $< -o $@
$(BUILD_DIR)/marco-upper-combat-regression.elf: $(BUILD_DIR)/tools/test_marco_upper_combat.o $(BUILD_DIR)/src/marco_upper.o $(BUILD_DIR)/src/marco_controller.o $(BUILD_DIR)/src/marco_animation.o $(BUILD_DIR)/src/neo_animation.o
marco-upper-combat-regression.z64: N64_ROM_TITLE = "MSX UPPER COMBAT"

$(BUILD_DIR)/tools/test_marco_crouch_native.o: tools/test_marco_crouch.c src/marco_crouch.h $(BUILD_DIR)/marco-crouch-turn/marco_crouch_fixture.h $(BUILD_DIR)/marco-crouch-turn/marco_crouch_native.h
	@mkdir -p $(dir $@)
	$(N64_CC) -I$(BUILD_DIR)/marco-crouch-turn $(N64_CFLAGS) -MMD -Isrc -DMARCO_CROUCH_NATIVE_ANIMATION -c $< -o $@
$(BUILD_DIR)/marco-crouch-regression.elf: $(BUILD_DIR)/tools/test_marco_crouch_native.o $(BUILD_DIR)/src/marco_crouch.o $(BUILD_DIR)/src/marco_upper.o $(BUILD_DIR)/src/marco_controller.o $(BUILD_DIR)/src/marco_animation.o $(BUILD_DIR)/src/neo_animation.o
marco-crouch-regression.z64: N64_ROM_TITLE = "MSX MARCO CROUCH"

$(BUILD_DIR)/tools/test_marco_motion_native.o: tools/test_marco_motion.c src/marco_motion.h $(BUILD_DIR)/marco-motion/marco_motion_fixture.h $(BUILD_DIR)/marco-motion/marco_motion_native.h $(BUILD_DIR)/marco-motion/marco_script.h
	@mkdir -p $(dir $@)
	$(N64_CC) -I$(BUILD_DIR)/marco-motion $(N64_CFLAGS) -MMD -Isrc -DMARCO_MOTION_NATIVE_ANIMATION -c $< -o $@
$(BUILD_DIR)/marco-motion-regression.elf: $(BUILD_DIR)/tools/test_marco_motion_native.o $(BUILD_DIR)/src/marco_motion.o $(BUILD_DIR)/src/marco_crouch.o $(BUILD_DIR)/src/marco_upper.o $(BUILD_DIR)/src/marco_controller.o $(BUILD_DIR)/src/marco_animation.o $(BUILD_DIR)/src/neo_animation.o
marco-motion-regression.z64: N64_ROM_TITLE = "MSX MARCO MOTION"

$(BUILD_DIR)/tools/test_marco_ammo.o: tools/test_marco_ammo.c $(BUILD_DIR)/marco_ammo_fixture.h
	@mkdir -p $(dir $@)
	$(N64_CC) $(N64_CFLAGS) -MMD -Isrc -c $< -o $@
$(BUILD_DIR)/marco-ammo-regression.elf: $(BUILD_DIR)/tools/test_marco_ammo.o $(BUILD_DIR)/src/marco_ammo.o
marco-ammo-regression.z64: N64_ROM_TITLE = "MSX MARCO AMMO"

$(BUILD_DIR)/tools/test_marco_stance.o: tools/test_marco_stance.c $(BUILD_DIR)/marco_stance_fixture.h
	@mkdir -p $(dir $@)
	$(N64_CC) $(N64_CFLAGS) -MMD -Isrc -c $< -o $@
$(BUILD_DIR)/marco-stance-regression.elf: $(BUILD_DIR)/tools/test_marco_stance.o $(BUILD_DIR)/src/marco_stance.o $(BUILD_DIR)/src/marco_upper.o $(BUILD_DIR)/src/marco_controller.o $(BUILD_DIR)/src/marco_animation.o $(BUILD_DIR)/src/neo_animation.o
marco-stance-regression.z64: N64_ROM_TITLE = "MSX MARCO STANCE"

$(BUILD_DIR)/tools/test_marco_idle.o: tools/test_marco_idle.c $(BUILD_DIR)/marco_idle_fixture.h
	@mkdir -p $(dir $@)
	$(N64_CC) $(N64_CFLAGS) -MMD -Isrc -c $< -o $@
$(BUILD_DIR)/marco-idle-regression.elf: $(BUILD_DIR)/tools/test_marco_idle.o $(BUILD_DIR)/src/marco_stance.o $(BUILD_DIR)/src/marco_upper.o $(BUILD_DIR)/src/marco_controller.o $(BUILD_DIR)/src/marco_animation.o $(BUILD_DIR)/src/neo_animation.o
marco-idle-regression.z64: N64_ROM_TITLE = "MSX MARCO IDLE"

$(BUILD_DIR)/tools/test_marco_body.o: tools/test_marco_body.c src/marco_body.h
	@mkdir -p $(dir $@)
	$(N64_CC) $(N64_CFLAGS) -MMD -Isrc -c $< -o $@
$(BUILD_DIR)/marco-body-regression.elf: $(BUILD_DIR)/tools/test_marco_body.o $(BUILD_DIR)/src/marco_body.o $(BUILD_DIR)/src/marco_stance.o $(BUILD_DIR)/src/marco_motion.o $(BUILD_DIR)/src/marco_crouch.o $(BUILD_DIR)/src/marco_upper.o $(BUILD_DIR)/src/marco_controller.o $(BUILD_DIR)/src/marco_animation.o $(BUILD_DIR)/src/neo_animation.o
marco-body-regression.z64: N64_ROM_TITLE = "MSX MARCO BODY"

$(BUILD_DIR)/tools/test_marco_common.o: tools/test_marco_common.c src/marco_common.h
	@mkdir -p $(dir $@)
	$(N64_CC) $(N64_CFLAGS) -MMD -Isrc -c $< -o $@
$(BUILD_DIR)/marco-common-regression.elf: $(BUILD_DIR)/tools/test_marco_common.o $(BUILD_DIR)/src/marco_common.o $(BUILD_DIR)/src/marco_upper.o $(BUILD_DIR)/src/marco_controller.o $(BUILD_DIR)/src/marco_animation.o $(BUILD_DIR)/src/neo_animation.o
marco-common-regression.z64: N64_ROM_TITLE = "MSX MARCO COMMON"

$(BUILD_DIR)/tools/test_marco_upper_limits.o: tools/test_marco_upper_limits.c src/marco_upper.h
	@mkdir -p $(dir $@)
	$(N64_CC) $(N64_CFLAGS) -MMD -Isrc -c $< -o $@
$(BUILD_DIR)/marco-upper-limits-regression.elf: $(BUILD_DIR)/tools/test_marco_upper_limits.o $(BUILD_DIR)/src/marco_crouch.o $(BUILD_DIR)/src/marco_upper.o $(BUILD_DIR)/src/marco_controller.o $(BUILD_DIR)/src/marco_animation.o $(BUILD_DIR)/src/neo_animation.o
marco-upper-limits-regression.z64: N64_ROM_TITLE = "MSX UPPER FORM"

filesystem/cinematics/title.msxc: tools/pack_title_art.py art/title/metal-slug-x64.png
	python3 tools/pack_title_art.py art/title/metal-slug-x64.png

filesystem/cinematics/title-announcement.wav64: art/audio/title-announcement.wav
	@mkdir -p filesystem/cinematics
	$(N64_INST)/bin/audioconv64 --wav-resample 32000 --wav-compress 1 --wav-loop false -o filesystem/cinematics $<

$(BUILD_DIR)/tools/intro_preview.o: src/mission_main.c $(BUILD_DIR)/entry_meta.h
	@mkdir -p $(dir $@)
	$(N64_CC) $(N64_CFLAGS) -MMD -Isrc -DMSX_INTRO_PREVIEW -c $< -o $@
$(BUILD_DIR)/intro-preview.elf: $(BUILD_DIR)/src/title_intro.o $(BUILD_DIR)/tools/intro_preview.o $(BUILD_DIR)/src/cinematic.o $(filter-out $(BUILD_DIR)/src/main.o $(BUILD_DIR)/src/title_intro.o $(BUILD_DIR)/src/cinematic.o,$(MSLUGX_GAME_OBJS))
$(BUILD_DIR)/intro-preview.dfs: $(BUILD_DIR)/mission1-internal.dfs
	cp $< $@
intro-preview.z64: N64_ROM_TITLE = "METAL SLUG X64"
intro-preview.z64: $(BUILD_DIR)/intro-preview.dfs

filesystem/cinematics/title-motion.msxc filesystem/cinematics/title-motion.wav64:
	@echo "Recover full title motion with tools/pack_title_animation.py; see docs/INTRO_AUDIO.md"
	@exit 1
# Campaign Marco with the private program table and pose catalog.
# Throw/C/H-down poses per step (tools/extract_throw_fixture.py, data/marco_throw_fixture.json).
$(BUILD_DIR)/marco_throw_fixture.h: tools/extract_throw_fixture.py data/marco_throw_fixture.json
	python3 tools/extract_throw_fixture.py --from-json data/marco_throw_fixture.json --header $@
$(BUILD_DIR)/marco_mummy_fixture.h: tools/extract_mummy_fixture.py data/marco_mummy_fixture.json
	python3 tools/extract_mummy_fixture.py --from-json data/marco_mummy_fixture.json --header $@
check-marco-campaign: $(BUILD_DIR)/marco_campaign.h $(BUILD_DIR)/marco_pose_meta.h $(BUILD_DIR)/legs_fixture.h $(BUILD_DIR)/marco_throw_fixture.h $(BUILD_DIR)/marco_mummy_fixture.h
	@mkdir -p $(BUILD_DIR)/host
	$(HOST_CC) $(HOST_CFLAGS) -I$(BUILD_DIR) -Isrc tools/test_marco_visual.c src/marco_visual.c src/marco_animation.c src/neo_animation.c src/marco_hgun.c src/marco_legs.c src/marco_death.c -o $(BUILD_DIR)/host/marco_visual
	$(BUILD_DIR)/host/marco_visual
	python3 -I tools/check_marco_romfs.py $(BUILD_DIR)/marco_pose_meta.h romfs/marco
# Playable characters (docs/PERSONAJES.md): private packs of the ROM
# (tools/export_character.py -> romfs/chars/cN.bin, Marco's own pack
# build/chars/c0.bin for the comparison with the built-in catalog) against
# each character's MAME 0.264 captures (data/character_fixture_cN.json).
$(BUILD_DIR)/character_fixture_c%.h: tools/extract_character_fixture.py data/character_fixture_c%.json
	python3 tools/extract_character_fixture.py --from-json data/character_fixture_c$*.json --header $@
.PHONY: check-characters check-pause
check-characters: $(BUILD_DIR)/marco_campaign.h $(BUILD_DIR)/character_fixture_c1.h $(BUILD_DIR)/character_fixture_c2.h $(BUILD_DIR)/character_fixture_c3.h
	@mkdir -p $(BUILD_DIR)/host $(BUILD_DIR)/chars
	cp romfs/chars/c1.bin romfs/chars/c2.bin romfs/chars/c3.bin $(BUILD_DIR)/chars/
	$(HOST_CC) $(HOST_CFLAGS) -I$(BUILD_DIR) -Isrc -Itools tools/test_character.c src/msx_character.c src/marco_visual.c src/marco_animation.c src/neo_animation.c src/marco_hgun.c src/marco_legs.c src/marco_death.c -o $(BUILD_DIR)/host/test_character
	$(BUILD_DIR)/host/test_character $(BUILD_DIR)/chars
.PHONY: check-m2boss
check-m2boss:
	$(MAKE) FULL_M2BOSS=1 check

