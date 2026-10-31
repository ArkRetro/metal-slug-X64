.DEFAULT_GOAL := all
N64_INST ?= /workspace/toolchains/package/opt/libdragon
# depth25d (default): subtle 2.5D parallax/lighting/shadows/fog; classic: original flat path.
MSX_VISUAL_MODE ?= depth25d
MSX_SCENARIO ?= campaign
# Arcade soft-DIP "BLOOD" (RAM 10FD8F): 1 = ON (default, uncensored), 0 = OFF (US/EU default).
MSX_BLOOD ?= 1
# depth25d extras for review builds: 1 perspective, 2 lights, 4 fog (0 = layered 2D + shadows).
MSX_DEPTH_STYLE ?= 0
export N64_INST
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
N64_CFLAGS += -I$(BUILD_DIR) -I$(BUILD_DIR)/retro25d -DMSX_BLOOD=$(MSX_BLOOD) $(MSX_EXTRA_CFLAGS)
endif
WORLD_SRCS := src/neo_camera.c src/neo_boss.c src/neo_world.c src/neo_gactor.c src/neo_region.c src/neo_truck.c src/neo_effects.c src/neo_flyer.c src/neo_props.c src/neo_rest.c src/neo_dome.c src/neo_barrel.c src/neo_wagon.c src/neo_arab.c src/neo_prisoner.c src/neo_item.c src/neo_armored.c src/neo_secret.c src/neo_script.c src/neo_soldier_ai.c src/neo_kernel.c src/neo_contact.c src/neo_control.c src/neo_rng.c src/neo_animation.c src/neo_soldier.c src/neo_soldier_action.c src/neo_hit.c src/neo_cull.c src/neo_throw_actor.c src/neo_throw.c src/neo_target.c src/neo_blood.c src/marco_ammo.c src/neo_bullet.c src/neo_chaser.c src/neo_hud.c src/marco_hgun.c src/marco_death.c src/neo_swords.c src/neo_rocket.c src/neo_heli.c src/marco_legs.c src/neo_camel.c src/neo_porter.c src/neo_civil.c src/neo_vslot.c src/neo_vulcan.c src/neo_slug.c src/neo_tank.c src/neo_tread.c src/neo_end.c src/neo_fade.c src/marco_mount.c src/marco_descent.c src/neo_tally.c src/neo_text.c src/neo_continue.c src/neo_landing.c src/neo_mission.c src/neo_wipe.c src/neo_goarrow.c
WORLD_OBJS := $(patsubst src/%.c,$(BUILD_DIR)/src/%.o,$(WORLD_SRCS))
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
$(BUILD_DIR)/audio-regression.elf: $(BUILD_DIR)/tools/test_msx_audio.o $(BUILD_DIR)/src/msx_audio.o
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
$(BUILD_DIR)/scene-debug.elf: $(BUILD_DIR)/tools/test_scene_release.o $(BUILD_DIR)/src/scene_renderer.o $(BUILD_DIR)/src/scene_assets.o $(BUILD_DIR)/src/msx_perf.o $(BUILD_DIR)/src/scene_cull.o $(BUILD_DIR)/src/retro_effects.o $(BUILD_DIR)/src/diorama_building.o $(BUILD_DIR)/src/retro25d.o
$(BUILD_DIR)/scene-release.elf: $(BUILD_DIR)/tools/test_scene_release.o $(BUILD_DIR)/release/scene_renderer.o $(BUILD_DIR)/release/scene_assets.o $(BUILD_DIR)/release/msx_perf.o $(BUILD_DIR)/release/scene_cull.o $(BUILD_DIR)/release/retro_effects.o $(BUILD_DIR)/release/diorama_building.o $(BUILD_DIR)/release/retro25d.o
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
$(BUILD_DIR)/mission1-internal.dfs: $(BUILD_DIR)/mission1-romfs/title.msxc $(BUILD_DIR)/mission1-romfs/title-motion.wav64 $(BUILD_DIR)/mission1-romfs/scene.pack $(BUILD_DIR)/mission1-romfs/entry.msxc $(BUILD_DIR)/mission1-romfs/entry.wav64
mission1-internal.z64: N64_ROM_TITLE = "MSX M1 INTERNAL"
mission1-internal.z64: $(BUILD_DIR)/mission1-internal.dfs
filesystem/cinematics/entry.json filesystem/cinematics/entry.msxc filesystem/cinematics/entry.wav64:
	@echo "Run tools/pack_cinematic.py with the local golden entry capture; see docs/INTRO_AUDIO.md"
	@exit 1

.PHONY: all clean check check-core check-cinematic preview-depth FORCE
FORCE:
all: mslugx-explorer.z64
# X64 title and parachute entry (docs/MISSION_ENTRY.md, docs/INTRO_AUDIO.md):
# private captures; with all six files the game boots logo -> title -> entry
# -> Mission 1 (src/main.c), without them it boots logo -> Mission 1.
BOOT_CINEMATICS := $(wildcard filesystem/cinematics/title-motion.json filesystem/cinematics/title-motion.msxc filesystem/cinematics/title-motion.wav64 filesystem/cinematics/entry.json filesystem/cinematics/entry.msxc filesystem/cinematics/entry.wav64)
ifeq ($(words $(BOOT_CINEMATICS)),6)
BOOT_CINE_OBJS := $(BUILD_DIR)/src/title_intro.o $(BUILD_DIR)/src/cinematic.o
$(BUILD_DIR)/src/main.o: $(BUILD_DIR)/entry_meta.h $(BOOT_CINEMATICS)
$(BUILD_DIR)/src/main.o: N64_CFLAGS += -DMSX_BOOT_CINEMATICS=1
else
BOOT_CINE_OBJS :=
endif
MSLUGX_GAME_OBJS = $(BUILD_DIR)/src/main.o $(BUILD_DIR)/src/neogeo_logo.o $(BUILD_DIR)/src/marco_visual.o $(BUILD_DIR)/src/marco_animation.o $(BUILD_DIR)/src/marco_renderer.o $(BUILD_DIR)/src/actor.o $(BUILD_DIR)/src/gameplay.o $(BUILD_DIR)/src/neo_collision.o $(BUILD_DIR)/src/scene_assets.o $(BUILD_DIR)/src/msx_perf.o $(BUILD_DIR)/src/scene_cull.o $(BUILD_DIR)/src/scene_renderer.o $(BUILD_DIR)/src/diorama_building.o $(BUILD_DIR)/src/retro25d.o $(BUILD_DIR)/src/retro_effects.o $(BUILD_DIR)/src/retro_events.o $(BUILD_DIR)/src/neo_vehicle.o $(BUILD_DIR)/src/vehicle_renderer.o $(BUILD_DIR)/src/boss_renderer.o $(BUILD_DIR)/src/hud_renderer.o $(BUILD_DIR)/src/msx_audio.o $(BUILD_DIR)/src/msx_sfx.o $(BUILD_DIR)/src/game_audio.o $(BUILD_DIR)/src/msx_ramfs.o $(BOOT_CINE_OBJS) $(WORLD_OBJS)
# The game objects (Marco, world, renderers) with mission_main.c in place of main.c.
$(BUILD_DIR)/mission1-internal.elf: $(BUILD_DIR)/src/title_intro.o $(BUILD_DIR)/src/mission_main.o $(BUILD_DIR)/src/cinematic.o $(filter-out $(BUILD_DIR)/src/main.o $(BUILD_DIR)/src/title_intro.o $(BUILD_DIR)/src/cinematic.o,$(MSLUGX_GAME_OBJS))
$(BUILD_DIR)/mslugx-explorer.elf: $(MSLUGX_GAME_OBJS)
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
BOOT_ATTRACT := $(wildcard $(foreach n,howto select confirm,filesystem/cinematics/$(n).msxc filesystem/cinematics/$(n).wav64))
# The fix-layer HUD (src/hud_renderer.c) draws the S-ROM tiles with the fix
# palettes of tools/build_hud_assets.py (docs/ARMAS_VEHICULOS.md §11).
$(BUILD_DIR)/game-romfs.stamp: romfs/scene.pack $(BUILD_DIR)/vehicle-sprites.stamp $(BUILD_DIR)/marco-poses.stamp $(GAME_AUDIO) $(BOOT_LOGO) $(BOOT_CINEMATICS) $(BOOT_ATTRACT) filesystem/fixed.bin filesystem/hud-palette.bin
	rm -rf $(BUILD_DIR)/game-romfs && mkdir -p $(BUILD_DIR)/game-romfs
	cp romfs/scene.pack $(BUILD_DIR)/game-romfs/ && cp -r romfs/vehicles romfs/marco $(BUILD_DIR)/game-romfs/
	cp filesystem/fixed.bin filesystem/hud-palette.bin $(BUILD_DIR)/game-romfs/
	$(if $(GAME_AUDIO),mkdir -p $(BUILD_DIR)/game-romfs/audio && cp $(GAME_AUDIO) $(BUILD_DIR)/game-romfs/audio/)
	$(if $(BOOT_LOGO),cp $(BOOT_LOGO) $(BUILD_DIR)/game-romfs/)
	$(if $(BOOT_CINE_OBJS),cp filesystem/cinematics/title-motion.msxc $(BUILD_DIR)/game-romfs/title.msxc && cp filesystem/cinematics/title-motion.wav64 filesystem/cinematics/entry.msxc filesystem/cinematics/entry.wav64 $(BUILD_DIR)/game-romfs/)
	$(if $(and $(BOOT_CINE_OBJS),$(BOOT_ATTRACT)),cp $(BOOT_ATTRACT) $(BUILD_DIR)/game-romfs/)
	touch $@
# Reference traversal cartridge (tools/test_mission_native_ares.py): the same
# game with an invulnerable player (MSX_REFERENCE_INVULNERABLE); never the release.
$(BUILD_DIR)/reference/gameplay.o: src/gameplay.c
	@mkdir -p $(dir $@)
	$(N64_CC) -c $(N64_CFLAGS) -MMD -DMSX_REFERENCE_INVULNERABLE=1 -o $@ $<
$(BUILD_DIR)/mslugx-reference.elf: $(BUILD_DIR)/reference/gameplay.o $(filter-out $(BUILD_DIR)/src/gameplay.o,$(MSLUGX_GAME_OBJS))
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
$(BUILD_DIR)/mslugx-lab.dfs: romfs/scene.pack romfs/fixed.bin romfs/sprites.bin $(BUILD_DIR)/vehicle-sprites.stamp
$(BUILD_DIR)/scene_meta.h $(BUILD_DIR)/boss_columns.h romfs/scene.pack romfs/fixed.bin romfs/sprites.bin &: tools/pack_scene.py tools/mission_tiles.py tools/export_marco.py filesystem/projectiles.json filesystem/mission1/map.json filesystem/mission1/props.json filesystem/marco.idx4 filesystem/marco-actions.idx4 filesystem/marco-motion.idx4 filesystem/soldier.idx4 filesystem/fixed.bin filesystem/sprites.bin filesystem/marco.bin filesystem/marco-actions.bin filesystem/desert.bin filesystem/soldier.bin filesystem/marco-motion.bin filesystem/soldier-native.json filesystem/boss-native.json data/boss_programs.json data/mission1_rest_inventory.json $(wildcard filesystem/mission1/rest/map.json filesystem/rest-native.json filesystem/blood-native.json)
	python3 tools/pack_scene.py
$(BUILD_DIR)/src/scene_assets.o: $(BUILD_DIR)/scene_meta.h
$(BUILD_DIR)/src/boss_renderer.o: $(BUILD_DIR)/scene_meta.h
$(BUILD_DIR)/src/neo_world.o $(BUILD_DIR)/src/neo_boss.o: $(BUILD_DIR)/boss_tables.h $(BUILD_DIR)/soldier_ai_tables.h $(BUILD_DIR)/region_rom.h
$(BUILD_DIR)/src/actor.o $(BUILD_DIR)/src/scene_renderer.o: $(BUILD_DIR)/visual_config.h
$(BUILD_DIR)/visual_config.h: FORCE tools/build_visual_config.py
	python3 tools/build_visual_config.py $(MSX_VISUAL_MODE) --scenario $(MSX_SCENARIO) --depth-style $(MSX_DEPTH_STYLE) --output $@
$(BUILD_DIR)/src/neo_collision.o: $(BUILD_DIR)/mission_collision.h $(BUILD_DIR)/vehicle_wreckage.h
$(BUILD_DIR)/src/neo_vehicle.o: $(BUILD_DIR)/vehicle_anim.h
$(BUILD_DIR)/vehicle_anim.h $(BUILD_DIR)/vehicle_wreckage.h &: tools/compile_vehicle.py filesystem/vehicle.json data/vehicle_wreckage.json
	python3 tools/compile_vehicle.py
$(BUILD_DIR)/vehicle-sprites.stamp: tools/build_vehicle_sprites.py filesystem/vehicle.json
	python3 tools/build_vehicle_sprites.py --mksprite $(N64_INST)/bin/mksprite
$(BUILD_DIR)/soldier_fixture.h: tools/compile_soldier_fixture.py data/first_soldier_init_fixture.json
	python3 tools/compile_soldier_fixture.py
$(BUILD_DIR)/rng_fixture.h: tools/compile_rng_fixture.py data/soldier_rng_fixture.json
	python3 tools/compile_rng_fixture.py
$(BUILD_DIR)/tools/test_neo_rng.o: $(BUILD_DIR)/rng_fixture.h
# Private graphics conversion is an explicit offline step; never needs ROMs in CI.
$(BUILD_DIR)/marco_pose_meta.h $(BUILD_DIR)/marco-poses.stamp:
	@echo "Run tools/build_marco_poses.py with an audited local capture first."
	@exit 1
$(BUILD_DIR)/src/marco_renderer.o $(BUILD_DIR)/tools/test_marco_render.o: $(BUILD_DIR)/marco_pose_meta.h
# Campaign Marco programs (docs/MARCO_CAMPAIGN.md): also an offline step from
# the audited catalog capture (tools/compile_marco_campaign.py).
$(BUILD_DIR)/marco_campaign.h:
	@echo "Run tools/compile_marco_campaign.py with the audited catalog capture first."
	@exit 1
$(BUILD_DIR)/src/marco_visual.o: $(BUILD_DIR)/marco_campaign.h
$(BUILD_DIR)/tools/test_marco_render.o: CC = $(N64_CC)
$(BUILD_DIR)/tools/test_marco_render.o: CFLAGS = $(N64_CFLAGS) -MMD
$(BUILD_DIR)/marco-render.elf: $(BUILD_DIR)/tools/test_marco_render.o $(BUILD_DIR)/src/marco_renderer.o
$(BUILD_DIR)/marco-render.dfs: N64_MKDFS_ROOT = $(BUILD_DIR)/marco-render-romfs
$(BUILD_DIR)/marco-render-romfs/.stamp: $(BUILD_DIR)/marco-poses.stamp
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
$(BUILD_DIR)/soldier_ai_tables.h: tools/compile_soldier_ai_tables.py data/soldier_ai_tables.json data/soldier_programs.json
	python3 tools/compile_soldier_ai_tables.py
$(BUILD_DIR)/soldier_tick_fixture.h: tools/compile_soldier_ticks.py data/soldier_tick_fixture.json.gz
	python3 tools/compile_soldier_ticks.py
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
FAMILIES := dome barrel truck swordgen arab creature prisoner hitem citem hbullet pbullet grenade chaser armored secret swords prisoner2 bossprisoner bossprisonerb food rocket heli camel decor awning porter slug civil blast debris flash label tank tankjump tankcrouch tankpush rlaunch goarrow exitmark
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
FAMILY_ARGS_rlaunch := --script-actor 101414 --script-routines 6371e --attacks --previous --spawn-window 2
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
	rm -rf build mslugx-camel-shot.z64 intro-preview.z64 marco-upper-limits-regression.z64 marco-common-regression.z64 marco-body-regression.z64 marco-idle-regression.z64 marco-stance-regression.z64 marco-ammo-regression.z64 marco-motion-regression.z64 marco-crouch-regression.z64 marco-upper-regression.z64 marco-upper-combat-regression.z64 marco-advance-pixels.z64 marco-advance-regression.z64 marco-coupled-pixels.z64 marco-controller-regression.z64 marco-coupled-regression.z64 marco-pair-regression.z64 marco-render.z64 marco-regression.z64 mslugx-explorer.z64 mslugx-lab.z64 rng-regression.z64 animation-regression.z64 contact-regression.z64 control-regression.z64 state-regression.z64 throw-regression.z64 attack-regression.z64 object-regression.z64 locomotion-regression.z64 kernel-regression.z64 audio-regression.z64 cinematic-regression.z64 mission1-internal.z64 scene-debug.z64 scene-release.z64
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
# Player death/respawn (docs/JEFE_MISION1.md): tools/extract_death_fixture.py.
$(BUILD_DIR)/death_fixture.h: tools/compile_death_fixture.py data/death_fixture.json
	python3 tools/compile_death_fixture.py
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

check-core: $(BUILD_DIR)/continue_fixture.h $(BUILD_DIR)/legs_fixture.h $(BUILD_DIR)/death_fixture.h $(BUILD_DIR)/hgun_fixture.h $(BUILD_DIR)/hud_fixture.h $(BUILD_DIR)/marco_idle_fixture.h $(BUILD_DIR)/marco_stance_fixture.h $(BUILD_DIR)/marco_ammo_fixture.h $(BUILD_DIR)/weapon_block_fixture.h $(BUILD_DIR)/marco-motion/marco_motion_fixture.h $(BUILD_DIR)/marco_crouch_fixture.h $(BUILD_DIR)/marco-crouch-turn/marco_crouch_fixture.h $(BUILD_DIR)/marco_upper_fixture.h $(BUILD_DIR)/marco-upper-combat/marco_upper_fixture.h $(BUILD_DIR)/marco-advance/marco_controller_fixture.h $(BUILD_DIR)/marco_controller_fixture.h $(BUILD_DIR)/marco_fields.h $(BUILD_DIR)/marco_selectors.h $(BUILD_DIR)/soldier_fixture.h $(BUILD_DIR)/rng_fixture.h $(BUILD_DIR)/animation_fixture.h $(BUILD_DIR)/history_fixture.h $(BUILD_DIR)/vslot_fixture.h $(BUILD_DIR)/vulcan_fixture.h $(BUILD_DIR)/mount_fixture.h
	mkdir -p $(BUILD_DIR)/host
	$(HOST_CC) $(HOST_CFLAGS) tools/test_fixed.c -o $(BUILD_DIR)/host/fixed
	$(HOST_CC) $(HOST_CFLAGS) tools/test_sprites.c -o $(BUILD_DIR)/host/sprites
	$(HOST_CC) $(HOST_CFLAGS) tools/test_neo_motion.c -o $(BUILD_DIR)/host/motion
	$(HOST_CC) $(HOST_CFLAGS) tools/test_scene_codec.c -o $(BUILD_DIR)/host/codec
	$(HOST_CC) $(HOST_CFLAGS) tools/test_scene_clock.c -o $(BUILD_DIR)/host/clock
	$(HOST_CC) $(HOST_CFLAGS) tools/test_cinematic_codec.c -o $(BUILD_DIR)/host/cinematic-codec
	$(HOST_CC) $(HOST_CFLAGS) tools/test_msx_sfx.c src/msx_sfx.c -o $(BUILD_DIR)/host/msx-sfx
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
	$(HOST_CC) $(HOST_CFLAGS) -I$(BUILD_DIR) tools/test_neo_hud.c src/neo_hud.c -o $(BUILD_DIR)/host/hud
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
check: check-boss $(BUILD_DIR)/mission_flow_fixture.h $(BUILD_DIR)/wipe_fixture.h $(BUILD_DIR)/end_letters_fixture.h $(BUILD_DIR)/start_letters_fixture.h $(TALLY_HEADERS) $(BUILD_DIR)/continue_sprites_fixture.h $(BUILD_DIR)/landing_fixture.h $(BUILD_DIR)/descent_fixture.h $(BUILD_DIR)/hmuzzle_fixture.h $(BUILD_DIR)/blood_fixture.h $(BUILD_DIR)/camera_fixture.h $(BUILD_DIR)/robe_fixture.h $(FAMILY_HEADERS) $(BUILD_DIR)/soldier_ai_tables.h $(BUILD_DIR)/region_rom.h $(BUILD_DIR)/throw_tables.h $(BUILD_DIR)/retro25d/retro_assets.h $(BUILD_DIR)/scene_meta.h $(BUILD_DIR)/soldier_tick_fixture.h $(BUILD_DIR)/soldier_script_fixture.h $(BUILD_DIR)/soldier_world_fixture.h $(BUILD_DIR)/kernel_fixture.h $(BUILD_DIR)/attack_state_fixture.h $(BUILD_DIR)/locomotion_fixture.h $(BUILD_DIR)/object_fixture.h $(BUILD_DIR)/throw_fixture.h $(BUILD_DIR)/attack_fixture.h $(BUILD_DIR)/cull_fixture.h $(BUILD_DIR)/action_fixture.h $(BUILD_DIR)/state_fixture.h $(BUILD_DIR)/mission_collision.h $(BUILD_DIR)/vehicle_anim.h $(BUILD_DIR)/vehicle_wreckage.h $(BUILD_DIR)/soldier_fixture.h $(BUILD_DIR)/rng_fixture.h $(BUILD_DIR)/animation_fixture.h $(BUILD_DIR)/contact_fixture.h $(BUILD_DIR)/contact_tables.h $(BUILD_DIR)/control_fixture.h $(BUILD_DIR)/control_tables.h
	mkdir -p $(BUILD_DIR)/host
	$(HOST_CC) $(HOST_CFLAGS) -I$(BUILD_DIR) tools/test_gameplay.c src/gameplay.c src/msx_sfx.c src/neo_collision.c src/neo_vehicle.c $(WORLD_SRCS) -o $(BUILD_DIR)/host/gameplay
	$(HOST_CC) $(HOST_CFLAGS) -I$(BUILD_DIR) tools/test_neo_collision.c src/neo_collision.c -o $(BUILD_DIR)/host/collision
	$(HOST_CC) $(HOST_CFLAGS) -I$(BUILD_DIR) tools/test_neo_mission.c $(WORLD_SRCS) src/neo_collision.c -o $(BUILD_DIR)/host/mission
	$(BUILD_DIR)/host/mission
	$(HOST_CC) $(HOST_CFLAGS) -I$(BUILD_DIR) tools/test_neo_wipe.c src/neo_wipe.c $(filter-out src/neo_wipe.c,$(WORLD_SRCS)) src/neo_collision.c -o $(BUILD_DIR)/host/wipe
	$(BUILD_DIR)/host/wipe
	$(HOST_CC) $(HOST_CFLAGS) -I$(BUILD_DIR) tools/test_marco_muzzle.c $(WORLD_SRCS) src/neo_collision.c -o $(BUILD_DIR)/host/muzzle
	$(BUILD_DIR)/host/muzzle
	$(BUILD_DIR)/host/gameplay > $(BUILD_DIR)/host/gameplay.out; s=$$?; cat $(BUILD_DIR)/host/gameplay.out; exit $$s
	# Determinism: the same test built at -O2 prints the same results, and
	# every release seed run alone (in another order) prints its line again.
	$(HOST_CC) $(HOST_CFLAGS) -O2 -I$(BUILD_DIR) tools/test_gameplay.c src/gameplay.c src/msx_sfx.c src/neo_collision.c src/neo_vehicle.c $(WORLD_SRCS) -o $(BUILD_DIR)/host/gameplay-O2
	$(BUILD_DIR)/host/gameplay-O2 > $(BUILD_DIR)/host/gameplay-O2.out
	diff $(BUILD_DIR)/host/gameplay.out $(BUILD_DIR)/host/gameplay-O2.out
	$(BUILD_DIR)/host/gameplay-O2 release 4 3 2 1 | sort > $(BUILD_DIR)/host/gameplay-seeds.out
	grep '^release run' $(BUILD_DIR)/host/gameplay.out | sort | diff - $(BUILD_DIR)/host/gameplay-seeds.out
	@echo "PASS: gameplay determinism (-O2 output identical, each release seed alone identical)"
	$(HOST_CC) $(HOST_CFLAGS) -I$(BUILD_DIR) tools/script_native.c $(WORLD_SRCS) src/neo_collision.c -o $(BUILD_DIR)/host/script-native
	python3 tools/audit_script_events.py $(BUILD_DIR)/host/script-native
	python3 tools/audit_script_events.py $(BUILD_DIR)/host/script-native --script data/mission2_script.json
	python3 tools/check_scene_budget.py
	$(HOST_CC) $(HOST_CFLAGS) -I$(BUILD_DIR) tools/test_scene_cull.c src/scene_cull.c src/retro25d.c -lm -o $(BUILD_DIR)/host/scene-cull
	$(BUILD_DIR)/host/scene-cull
	$(HOST_CC) $(HOST_CFLAGS) -I$(BUILD_DIR) tools/test_scene_pack.c -o $(BUILD_DIR)/host/scene-pack
	$(BUILD_DIR)/host/scene-pack romfs/scene.pack
	python3 tools/test_marco_sprite_edges.py
	$(BUILD_DIR)/host/collision
	$(HOST_CC) $(HOST_CFLAGS) -I$(BUILD_DIR) tools/test_neo_vehicle.c src/neo_vehicle.c src/neo_collision.c src/gameplay.c $(WORLD_SRCS) -o $(BUILD_DIR)/host/vehicle
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
	$(HOST_CC) $(HOST_CFLAGS) -I$(BUILD_DIR) tools/test_neo_soldier_ai.c src/neo_soldier_ai.c src/neo_blood.c src/neo_target.c src/neo_kernel.c src/neo_collision.c src/neo_contact.c src/neo_control.c src/neo_rng.c src/neo_animation.c src/neo_soldier.c src/neo_soldier_action.c src/neo_hit.c src/neo_cull.c -o $(BUILD_DIR)/host/soldier_ai
	$(BUILD_DIR)/host/soldier_ai
	$(HOST_CC) $(HOST_CFLAGS) -I$(BUILD_DIR) tools/test_neo_script.c src/neo_script.c src/neo_soldier_ai.c src/neo_blood.c src/neo_target.c src/neo_kernel.c src/neo_collision.c src/neo_contact.c src/neo_control.c src/neo_rng.c src/neo_animation.c src/neo_soldier.c src/neo_soldier_action.c src/neo_hit.c src/neo_cull.c -o $(BUILD_DIR)/host/script
	$(BUILD_DIR)/host/script
	$(HOST_CC) $(HOST_CFLAGS) -I$(BUILD_DIR) tools/test_neo_world.c $(WORLD_SRCS) src/neo_collision.c -o $(BUILD_DIR)/host/world
	$(BUILD_DIR)/host/world
	$(HOST_CC) $(HOST_CFLAGS) -I$(BUILD_DIR) -DMSX_BLOOD=$(MSX_BLOOD) tools/test_neo_blood.c $(WORLD_SRCS) src/neo_collision.c -o $(BUILD_DIR)/host/blood
	$(BUILD_DIR)/host/blood
	$(HOST_CC) $(HOST_CFLAGS) -I$(BUILD_DIR) tools/test_neo_camera.c src/neo_camera.c -o $(BUILD_DIR)/host/camera
	$(BUILD_DIR)/host/camera
	$(HOST_CC) $(HOST_CFLAGS) -I$(BUILD_DIR) tools/test_neo_rest.c src/neo_rest.c src/neo_dome.c src/neo_barrel.c src/neo_wagon.c src/neo_arab.c src/neo_prisoner.c src/neo_item.c src/neo_armored.c src/neo_secret.c src/neo_swords.c src/neo_rocket.c src/neo_heli.c src/neo_camel.c src/neo_porter.c src/neo_civil.c src/neo_slug.c src/neo_tank.c src/neo_tread.c src/neo_vulcan.c src/neo_vslot.c src/neo_boss.c src/neo_gactor.c src/neo_region.c src/neo_goarrow.c src/neo_truck.c src/neo_effects.c src/neo_flyer.c src/neo_props.c src/neo_kernel.c src/neo_contact.c src/neo_control.c src/neo_rng.c src/neo_animation.c src/neo_cull.c src/neo_collision.c src/neo_soldier_ai.c src/neo_blood.c src/neo_soldier.c src/neo_soldier_action.c src/neo_hit.c src/neo_target.c src/marco_ammo.c src/neo_bullet.c src/neo_chaser.c -o $(BUILD_DIR)/host/rest
	$(BUILD_DIR)/host/rest
	$(HOST_CC) $(HOST_CFLAGS) -I$(BUILD_DIR) -DEND_HEADER='"end_letters_fixture.h"' -DEND_NAME=end_letters tools/test_neo_end.c $(filter-out src/neo_world.c src/neo_script.c src/neo_camera.c,$(WORLD_SRCS)) src/neo_collision.c -o $(BUILD_DIR)/host/end_letters
	$(BUILD_DIR)/host/end_letters
	$(HOST_CC) $(HOST_CFLAGS) -I$(BUILD_DIR) -DEND_HEADER='"start_letters_fixture.h"' -DEND_NAME=start_letters tools/test_neo_end.c $(filter-out src/neo_world.c src/neo_script.c src/neo_camera.c,$(WORLD_SRCS)) src/neo_collision.c -o $(BUILD_DIR)/host/start_letters
	$(BUILD_DIR)/host/start_letters
	$(HOST_CC) $(HOST_CFLAGS) -I$(BUILD_DIR) tools/test_neo_continue_sprites.c $(filter-out src/neo_world.c src/neo_script.c src/neo_camera.c,$(WORLD_SRCS)) src/neo_collision.c -o $(BUILD_DIR)/host/continue_sprites
	$(BUILD_DIR)/host/continue_sprites
	$(HOST_CC) $(HOST_CFLAGS) -I$(BUILD_DIR) tools/test_neo_landing.c $(filter-out src/neo_world.c src/neo_script.c src/neo_camera.c,$(WORLD_SRCS)) src/neo_collision.c -o $(BUILD_DIR)/host/landing
	$(BUILD_DIR)/host/landing
	$(HOST_CC) $(HOST_CFLAGS) -I$(BUILD_DIR) tools/test_marco_descent.c src/marco_controller.c src/marco_animation.c $(filter-out src/neo_world.c src/neo_script.c src/neo_camera.c,$(WORLD_SRCS)) src/neo_collision.c -o $(BUILD_DIR)/host/descent
	$(BUILD_DIR)/host/descent
	for t in $(TALLIES); do $(HOST_CC) $(HOST_CFLAGS) -I$(BUILD_DIR) -DTALLY_HEADER="\"tally_$${t}_fixture.h\"" -DTALLY_NAME=tally_$$t tools/test_neo_tally.c $(filter-out src/neo_world.c src/neo_script.c src/neo_camera.c,$(WORLD_SRCS)) src/neo_collision.c -o $(BUILD_DIR)/host/tally_$$t && $(BUILD_DIR)/host/tally_$$t || exit 1; done
	for f in $(FAMILIES); do $(HOST_CC) $(HOST_CFLAGS) -I$(BUILD_DIR) -DFAMILY_HEADER="\"$${f}_fixture.h\"" -DFAMILY_NAME=$$f tools/test_neo_family.c $(filter-out src/neo_world.c src/neo_script.c src/neo_camera.c,$(WORLD_SRCS)) src/neo_collision.c -o $(BUILD_DIR)/host/family_$$f && $(BUILD_DIR)/host/family_$$f && rm -f $(BUILD_DIR)/host/family_$$f || exit 1; done
	$(HOST_CC) $(HOST_CFLAGS) -DRETRO_REAL_ASSETS -I$(BUILD_DIR)/retro25d tools/test_retro25d.c src/retro25d.c src/retro_events.c -lm -o $(BUILD_DIR)/host/retro25d
	$(BUILD_DIR)/host/retro25d
-include $(wildcard $(BUILD_DIR)/src/*.d $(BUILD_DIR)/tools/*.d $(BUILD_DIR)/release/*.d $(BUILD_DIR)/reference/*.d)

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
check-marco-campaign: $(BUILD_DIR)/marco_campaign.h $(BUILD_DIR)/marco_pose_meta.h $(BUILD_DIR)/legs_fixture.h $(BUILD_DIR)/marco_throw_fixture.h
	@mkdir -p $(BUILD_DIR)/host
	$(HOST_CC) $(HOST_CFLAGS) -I$(BUILD_DIR) -Isrc tools/test_marco_visual.c src/marco_visual.c src/marco_animation.c src/neo_animation.c src/marco_hgun.c src/marco_legs.c src/marco_death.c -o $(BUILD_DIR)/host/marco_visual
	$(BUILD_DIR)/host/marco_visual
