.DEFAULT_GOAL := all
N64_INST ?= /workspace/toolchains/package/opt/libdragon
# depth25d (default): subtle 2.5D parallax/lighting/shadows/fog; classic: original flat path.
MSX_VISUAL_MODE ?= depth25d
MSX_SCENARIO ?= campaign
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
N64_CFLAGS += -I$(BUILD_DIR) -I$(BUILD_DIR)/retro25d $(MSX_EXTRA_CFLAGS)
endif
WORLD_SRCS := src/neo_world.c src/neo_script.c src/neo_soldier_ai.c src/neo_kernel.c src/neo_contact.c src/neo_control.c src/neo_rng.c src/neo_animation.c src/neo_soldier.c src/neo_soldier_action.c src/neo_hit.c src/neo_cull.c src/neo_throw_actor.c src/neo_throw.c src/neo_target.c
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
$(BUILD_DIR)/scene-debug.elf: $(BUILD_DIR)/tools/test_scene_release.o $(BUILD_DIR)/src/scene_renderer.o $(BUILD_DIR)/src/scene_assets.o $(BUILD_DIR)/src/retro_effects.o $(BUILD_DIR)/src/diorama_building.o $(BUILD_DIR)/src/retro25d.o
$(BUILD_DIR)/scene-release.elf: $(BUILD_DIR)/tools/test_scene_release.o $(BUILD_DIR)/release/scene_renderer.o $(BUILD_DIR)/release/scene_assets.o $(BUILD_DIR)/release/retro_effects.o $(BUILD_DIR)/release/diorama_building.o $(BUILD_DIR)/release/retro25d.o
$(BUILD_DIR)/scene-debug.dfs $(BUILD_DIR)/scene-release.dfs: N64_MKDFS_ROOT = $(BUILD_DIR)/scene-test-romfs
$(BUILD_DIR)/scene-test-romfs/scene.pack: romfs/scene.pack
	mkdir -p $(dir $@)
	cp $< $@
$(BUILD_DIR)/scene-debug.dfs $(BUILD_DIR)/scene-release.dfs: $(BUILD_DIR)/scene-test-romfs/scene.pack
scene-debug.z64 scene-release.z64: N64_ROM_TITLE = "MSX RELEASE TEST"
scene-debug.z64: $(BUILD_DIR)/scene-debug.dfs
scene-release.z64: $(BUILD_DIR)/scene-release.dfs

# Internal integration cartridge: not the requested complete Mission 1 demo.
$(BUILD_DIR)/mission1-romfs/scene.pack $(BUILD_DIR)/mission1-romfs/entry.msxc $(BUILD_DIR)/mission1-romfs/entry.wav64 $(BUILD_DIR)/mission1-romfs/title-motion.wav64 $(BUILD_DIR)/mission1-romfs/title.msxc &: tools/build_mission_fs.py art/audio/sixty-four.wav filesystem/cinematics/title-motion.json filesystem/cinematics/title-motion.msxc filesystem/cinematics/title-motion.wav64 filesystem/cinematics/entry.json filesystem/cinematics/entry.msxc filesystem/cinematics/entry.wav64 romfs/scene.pack $(BUILD_DIR)/vehicle-sprites.stamp
	python3 tools/build_mission_fs.py
$(BUILD_DIR)/entry_meta.h: tools/compile_entry_meta.py filesystem/cinematics/entry.json
	python3 tools/compile_entry_meta.py
$(BUILD_DIR)/src/mission_main.o: $(BUILD_DIR)/entry_meta.h
$(BUILD_DIR)/mission1-internal.dfs: N64_MKDFS_ROOT = $(BUILD_DIR)/mission1-romfs
$(BUILD_DIR)/mission1-internal.dfs: $(BUILD_DIR)/mission1-romfs/title.msxc $(BUILD_DIR)/mission1-romfs/title-motion.wav64 $(BUILD_DIR)/mission1-romfs/scene.pack $(BUILD_DIR)/mission1-romfs/entry.msxc $(BUILD_DIR)/mission1-romfs/entry.wav64
$(BUILD_DIR)/mission1-internal.elf: $(BUILD_DIR)/src/title_intro.o $(BUILD_DIR)/src/mission_main.o $(BUILD_DIR)/src/cinematic.o $(BUILD_DIR)/src/msx_audio.o $(BUILD_DIR)/src/actor.o $(BUILD_DIR)/src/gameplay.o $(BUILD_DIR)/src/neo_collision.o $(BUILD_DIR)/src/scene_assets.o $(BUILD_DIR)/src/scene_renderer.o $(BUILD_DIR)/src/diorama_building.o $(BUILD_DIR)/src/retro25d.o $(BUILD_DIR)/src/retro_effects.o $(BUILD_DIR)/src/retro_events.o $(BUILD_DIR)/src/neo_vehicle.o $(BUILD_DIR)/src/vehicle_renderer.o $(WORLD_OBJS)
mission1-internal.z64: N64_ROM_TITLE = "MSX M1 INTERNAL"
mission1-internal.z64: $(BUILD_DIR)/mission1-internal.dfs
filesystem/cinematics/entry.json filesystem/cinematics/entry.msxc filesystem/cinematics/entry.wav64:
	@echo "Run tools/pack_cinematic.py with the local golden entry capture; see docs/INTRO_AUDIO.md"
	@exit 1

.PHONY: all clean check check-core check-cinematic preview-depth FORCE
FORCE:
all: mslugx-explorer.z64
$(BUILD_DIR)/mslugx-explorer.elf: $(BUILD_DIR)/src/main.o $(BUILD_DIR)/src/actor.o $(BUILD_DIR)/src/gameplay.o $(BUILD_DIR)/src/neo_collision.o $(BUILD_DIR)/src/scene_assets.o $(BUILD_DIR)/src/scene_renderer.o $(BUILD_DIR)/src/diorama_building.o $(BUILD_DIR)/src/retro25d.o $(BUILD_DIR)/src/retro_effects.o $(BUILD_DIR)/src/retro_events.o $(BUILD_DIR)/src/neo_vehicle.o $(BUILD_DIR)/src/vehicle_renderer.o $(WORLD_OBJS)
mslugx-explorer.z64: N64_ROM_TITLE = "MSX COLLISION v10"
mslugx-explorer.z64: $(BUILD_DIR)/mslugx-explorer.dfs
$(BUILD_DIR)/mslugx-explorer.dfs: romfs/scene.pack romfs/fixed.bin romfs/sprites.bin $(BUILD_DIR)/vehicle-sprites.stamp
$(BUILD_DIR)/scene_meta.h romfs/scene.pack romfs/fixed.bin romfs/sprites.bin &: tools/pack_scene.py tools/mission_tiles.py tools/export_marco.py filesystem/projectiles.json filesystem/mission1/map.json filesystem/mission1/props.json filesystem/marco.idx4 filesystem/marco-actions.idx4 filesystem/marco-motion.idx4 filesystem/soldier.idx4 filesystem/fixed.bin filesystem/sprites.bin filesystem/marco.bin filesystem/marco-actions.bin filesystem/desert.bin filesystem/soldier.bin filesystem/marco-motion.bin filesystem/soldier-native.json
	python3 tools/pack_scene.py
$(BUILD_DIR)/src/scene_assets.o: $(BUILD_DIR)/scene_meta.h
$(BUILD_DIR)/src/actor.o: $(BUILD_DIR)/visual_config.h
$(BUILD_DIR)/visual_config.h: FORCE tools/build_visual_config.py
	python3 tools/build_visual_config.py $(MSX_VISUAL_MODE) --scenario $(MSX_SCENARIO) --output $@
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
$(BUILD_DIR)/mission_collision.h: tools/compile_collision.py filesystem/mission1/collision.json
	python3 tools/compile_collision.py
filesystem/fixed.bin filesystem/sprites.bin:
	@echo 'Primero: python3 tools/import_rom.py /ruta/mslugx.zip' >&2
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
	rm -rf build intro-preview.z64 marco-upper-limits-regression.z64 marco-common-regression.z64 marco-body-regression.z64 marco-idle-regression.z64 marco-stance-regression.z64 marco-ammo-regression.z64 marco-motion-regression.z64 marco-crouch-regression.z64 marco-upper-regression.z64 marco-upper-combat-regression.z64 marco-advance-pixels.z64 marco-advance-regression.z64 marco-coupled-pixels.z64 marco-controller-regression.z64 marco-coupled-regression.z64 marco-pair-regression.z64 marco-render.z64 marco-regression.z64 mslugx-explorer.z64 rng-regression.z64 animation-regression.z64 contact-regression.z64 control-regression.z64 state-regression.z64 throw-regression.z64 attack-regression.z64 object-regression.z64 locomotion-regression.z64 kernel-regression.z64 audio-regression.z64 cinematic-regression.z64 mission1-internal.z64 scene-debug.z64 scene-release.z64
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

$(BUILD_DIR)/marco_stance_fixture.h: tools/compile_marco_stance.py tools/compile_marco_upper.py data/marco_stance_fixture.json
	python3 tools/compile_marco_stance.py

$(BUILD_DIR)/marco_idle_fixture.h: tools/compile_marco_idle.py data/marco_idle_fixture.json
	python3 tools/compile_marco_idle.py

check-core: $(BUILD_DIR)/soldier_ai_tables.h $(BUILD_DIR)/throw_tables.h $(BUILD_DIR)/control_tables.h $(BUILD_DIR)/contact_tables.h $(BUILD_DIR)/mission_collision.h $(BUILD_DIR)/vehicle_wreckage.h $(BUILD_DIR)/marco_idle_fixture.h $(BUILD_DIR)/marco_stance_fixture.h $(BUILD_DIR)/marco_ammo_fixture.h $(BUILD_DIR)/marco-motion/marco_motion_fixture.h $(BUILD_DIR)/marco_crouch_fixture.h $(BUILD_DIR)/marco-crouch-turn/marco_crouch_fixture.h $(BUILD_DIR)/marco_upper_fixture.h $(BUILD_DIR)/marco-upper-combat/marco_upper_fixture.h $(BUILD_DIR)/marco-advance/marco_controller_fixture.h $(BUILD_DIR)/marco_controller_fixture.h $(BUILD_DIR)/marco_fields.h $(BUILD_DIR)/marco_selectors.h $(BUILD_DIR)/soldier_fixture.h $(BUILD_DIR)/rng_fixture.h $(BUILD_DIR)/animation_fixture.h $(BUILD_DIR)/history_fixture.h
	mkdir -p $(BUILD_DIR)/host
	$(HOST_CC) $(HOST_CFLAGS) tools/test_fixed.c -o $(BUILD_DIR)/host/fixed
	$(HOST_CC) $(HOST_CFLAGS) tools/test_sprites.c -o $(BUILD_DIR)/host/sprites
	$(HOST_CC) $(HOST_CFLAGS) tools/test_neo_motion.c -o $(BUILD_DIR)/host/motion
	$(HOST_CC) $(HOST_CFLAGS) tools/test_scene_codec.c -o $(BUILD_DIR)/host/codec
	$(HOST_CC) $(HOST_CFLAGS) tools/test_scene_clock.c -o $(BUILD_DIR)/host/clock
	$(HOST_CC) $(HOST_CFLAGS) tools/test_cinematic_codec.c -o $(BUILD_DIR)/host/cinematic-codec
	$(BUILD_DIR)/host/fixed
	$(BUILD_DIR)/host/sprites
	$(BUILD_DIR)/host/motion
	$(BUILD_DIR)/host/codec
	$(BUILD_DIR)/host/clock
	$(BUILD_DIR)/host/cinematic-codec
	python3 tools/test_neo_scene.py
	python3 tools/test_collision_audit.py
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
	$(HOST_CC) $(HOST_CFLAGS) -I$(BUILD_DIR) tools/test_marco_stance.c src/marco_stance.c src/marco_upper.c src/marco_controller.c src/marco_animation.c src/neo_animation.c -o $(BUILD_DIR)/host/marco-stance
	$(BUILD_DIR)/host/marco-stance
	$(HOST_CC) $(HOST_CFLAGS) -I$(BUILD_DIR) tools/test_marco_idle.c src/marco_stance.c src/marco_upper.c src/marco_controller.c src/marco_animation.c src/neo_animation.c -o $(BUILD_DIR)/host/marco-idle
	$(BUILD_DIR)/host/marco-idle
	$(HOST_CC) $(HOST_CFLAGS) tools/test_diorama_building.c src/diorama_building.c src/retro25d.c -lm -o $(BUILD_DIR)/host/diorama-building
	$(BUILD_DIR)/host/diorama-building
	$(HOST_CC) $(HOST_CFLAGS) tools/test_marco_common.c src/marco_common.c src/marco_upper.c src/marco_controller.c src/marco_animation.c src/neo_animation.c -o $(BUILD_DIR)/host/marco-common
	$(BUILD_DIR)/host/marco-common
check: check-core $(BUILD_DIR)/retro25d/retro_assets.h $(BUILD_DIR)/scene_meta.h $(BUILD_DIR)/soldier_tick_fixture.h $(BUILD_DIR)/soldier_script_fixture.h $(BUILD_DIR)/soldier_world_fixture.h $(BUILD_DIR)/kernel_fixture.h $(BUILD_DIR)/attack_state_fixture.h $(BUILD_DIR)/locomotion_fixture.h $(BUILD_DIR)/object_fixture.h $(BUILD_DIR)/throw_fixture.h $(BUILD_DIR)/attack_fixture.h $(BUILD_DIR)/cull_fixture.h $(BUILD_DIR)/action_fixture.h $(BUILD_DIR)/state_fixture.h $(BUILD_DIR)/mission_collision.h $(BUILD_DIR)/vehicle_anim.h $(BUILD_DIR)/vehicle_wreckage.h $(BUILD_DIR)/soldier_fixture.h $(BUILD_DIR)/rng_fixture.h $(BUILD_DIR)/animation_fixture.h $(BUILD_DIR)/contact_fixture.h $(BUILD_DIR)/contact_tables.h $(BUILD_DIR)/control_fixture.h $(BUILD_DIR)/control_tables.h
	mkdir -p $(BUILD_DIR)/host
	$(HOST_CC) $(HOST_CFLAGS) -I$(BUILD_DIR) tools/test_gameplay.c src/gameplay.c src/neo_collision.c src/neo_vehicle.c $(WORLD_SRCS) -o $(BUILD_DIR)/host/gameplay
	$(HOST_CC) $(HOST_CFLAGS) -I$(BUILD_DIR) tools/test_neo_collision.c src/neo_collision.c -o $(BUILD_DIR)/host/collision
	$(BUILD_DIR)/host/gameplay
	python3 tools/check_scene_budget.py
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
	$(HOST_CC) $(HOST_CFLAGS) -I$(BUILD_DIR) tools/test_neo_soldier_ai.c src/neo_soldier_ai.c src/neo_kernel.c src/neo_collision.c src/neo_contact.c src/neo_control.c src/neo_rng.c src/neo_animation.c src/neo_soldier.c src/neo_soldier_action.c src/neo_hit.c src/neo_cull.c -o $(BUILD_DIR)/host/soldier_ai
	$(BUILD_DIR)/host/soldier_ai
	$(HOST_CC) $(HOST_CFLAGS) -I$(BUILD_DIR) tools/test_neo_script.c src/neo_script.c src/neo_soldier_ai.c src/neo_kernel.c src/neo_collision.c src/neo_contact.c src/neo_control.c src/neo_rng.c src/neo_animation.c src/neo_soldier.c src/neo_soldier_action.c src/neo_hit.c src/neo_cull.c -o $(BUILD_DIR)/host/script
	$(BUILD_DIR)/host/script
	$(HOST_CC) $(HOST_CFLAGS) -I$(BUILD_DIR) tools/test_neo_world.c $(WORLD_SRCS) src/neo_collision.c -o $(BUILD_DIR)/host/world
	$(BUILD_DIR)/host/world
	$(HOST_CC) $(HOST_CFLAGS) -DRETRO_REAL_ASSETS -I$(BUILD_DIR)/retro25d tools/test_retro25d.c src/retro25d.c src/retro_events.c -lm -o $(BUILD_DIR)/host/retro25d
	$(BUILD_DIR)/host/retro25d
-include $(wildcard $(BUILD_DIR)/src/*.d $(BUILD_DIR)/tools/*.d $(BUILD_DIR)/release/*.d)

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
$(BUILD_DIR)/intro-preview.elf: $(BUILD_DIR)/src/title_intro.o $(BUILD_DIR)/tools/intro_preview.o $(BUILD_DIR)/src/cinematic.o $(BUILD_DIR)/src/msx_audio.o $(BUILD_DIR)/src/actor.o $(BUILD_DIR)/src/gameplay.o $(BUILD_DIR)/src/neo_collision.o $(BUILD_DIR)/src/scene_assets.o $(BUILD_DIR)/src/scene_renderer.o $(BUILD_DIR)/src/diorama_building.o $(BUILD_DIR)/src/retro25d.o $(BUILD_DIR)/src/retro_effects.o $(BUILD_DIR)/src/retro_events.o $(BUILD_DIR)/src/neo_vehicle.o $(BUILD_DIR)/src/vehicle_renderer.o
$(BUILD_DIR)/intro-preview.dfs: $(BUILD_DIR)/mission1-internal.dfs
	cp $< $@
intro-preview.z64: N64_ROM_TITLE = "METAL SLUG X64"
intro-preview.z64: $(BUILD_DIR)/intro-preview.dfs

filesystem/cinematics/title-motion.msxc filesystem/cinematics/title-motion.wav64:
	@echo "Recover full title motion with tools/pack_title_animation.py; see docs/INTRO_AUDIO.md"
	@exit 1
