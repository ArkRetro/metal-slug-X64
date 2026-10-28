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

# Internal integration cartridge: not the requested complete Mission 1 demo.
$(BUILD_DIR)/mission1-romfs/scene.pack $(BUILD_DIR)/mission1-romfs/entry.msxc $(BUILD_DIR)/mission1-romfs/entry.wav64 &: tools/build_mission_fs.py filesystem/cinematics/entry.json filesystem/cinematics/entry.msxc filesystem/cinematics/entry.wav64 romfs/scene.pack $(BUILD_DIR)/vehicle-sprites.stamp
	python3 tools/build_mission_fs.py
$(BUILD_DIR)/entry_meta.h: tools/compile_entry_meta.py filesystem/cinematics/entry.json
	python3 tools/compile_entry_meta.py
$(BUILD_DIR)/src/mission_main.o: $(BUILD_DIR)/entry_meta.h
$(BUILD_DIR)/mission1-internal.dfs: N64_MKDFS_ROOT = $(BUILD_DIR)/mission1-romfs
$(BUILD_DIR)/mission1-internal.dfs: $(BUILD_DIR)/mission1-romfs/scene.pack $(BUILD_DIR)/mission1-romfs/entry.msxc $(BUILD_DIR)/mission1-romfs/entry.wav64
$(BUILD_DIR)/mission1-internal.elf: $(BUILD_DIR)/src/mission_main.o $(BUILD_DIR)/src/cinematic.o $(BUILD_DIR)/src/msx_audio.o $(BUILD_DIR)/src/actor.o $(BUILD_DIR)/src/gameplay.o $(BUILD_DIR)/src/neo_collision.o $(BUILD_DIR)/src/scene_assets.o $(BUILD_DIR)/src/scene_renderer.o $(BUILD_DIR)/src/retro25d.o $(BUILD_DIR)/src/retro_effects.o $(BUILD_DIR)/src/retro_events.o $(BUILD_DIR)/src/neo_vehicle.o $(BUILD_DIR)/src/vehicle_renderer.o
mission1-internal.z64: N64_ROM_TITLE = "MSX M1 INTERNAL"
mission1-internal.z64: $(BUILD_DIR)/mission1-internal.dfs
filesystem/cinematics/entry.json filesystem/cinematics/entry.msxc filesystem/cinematics/entry.wav64:
	@echo "Run tools/pack_cinematic.py with the local golden entry capture; see docs/INTRO_AUDIO.md"
	@exit 1

.PHONY: all clean check check-core check-cinematic preview-depth FORCE
FORCE:
all: mslugx-explorer.z64
$(BUILD_DIR)/mslugx-explorer.elf: $(BUILD_DIR)/src/main.o $(BUILD_DIR)/src/actor.o $(BUILD_DIR)/src/gameplay.o $(BUILD_DIR)/src/neo_collision.o $(BUILD_DIR)/src/scene_assets.o $(BUILD_DIR)/src/scene_renderer.o $(BUILD_DIR)/src/retro25d.o $(BUILD_DIR)/src/retro_effects.o $(BUILD_DIR)/src/retro_events.o $(BUILD_DIR)/src/neo_vehicle.o $(BUILD_DIR)/src/vehicle_renderer.o
mslugx-explorer.z64: N64_ROM_TITLE = "MSX COLLISION v10"
mslugx-explorer.z64: $(BUILD_DIR)/mslugx-explorer.dfs
$(BUILD_DIR)/mslugx-explorer.dfs: romfs/scene.pack romfs/fixed.bin romfs/sprites.bin $(BUILD_DIR)/vehicle-sprites.stamp
$(BUILD_DIR)/scene_meta.h romfs/scene.pack romfs/fixed.bin romfs/sprites.bin &: tools/pack_scene.py tools/mission_tiles.py tools/export_marco.py filesystem/projectiles.json filesystem/mission1/map.json filesystem/mission1/props.json filesystem/marco.idx4 filesystem/marco-actions.idx4 filesystem/marco-motion.idx4 filesystem/soldier.idx4 filesystem/fixed.bin filesystem/sprites.bin filesystem/marco.bin filesystem/marco-actions.bin filesystem/desert.bin filesystem/soldier.bin filesystem/marco-motion.bin
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
	rm -rf build mslugx-explorer.z64 rng-regression.z64 animation-regression.z64 contact-regression.z64 control-regression.z64 state-regression.z64 throw-regression.z64 attack-regression.z64 object-regression.z64 locomotion-regression.z64 audio-regression.z64 cinematic-regression.z64 mission1-internal.z64
# Offline effect previews (proxy level, no original art) from the real mesh code.
preview-depth: $(BUILD_DIR)/retro25d/retro_assets.h
	python3 tools/export_effect_previews.py
check-cinematic: $(BUILD_DIR)/entry_meta.h
	mkdir -p $(BUILD_DIR)/host
	$(HOST_CC) $(HOST_CFLAGS) -I$(BUILD_DIR) tools/test_cinematic_pack.c -o $(BUILD_DIR)/host/cinematic-pack
	$(BUILD_DIR)/host/cinematic-pack filesystem/cinematics/entry.msxc
check-core:
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
	$(HOST_CC) $(HOST_CFLAGS) tools/test_retro25d.c src/retro25d.c src/retro_events.c -lm -o $(BUILD_DIR)/host/retro25d-core
	$(BUILD_DIR)/host/retro25d-core
check: check-core $(BUILD_DIR)/retro25d/retro_assets.h $(BUILD_DIR)/scene_meta.h $(BUILD_DIR)/attack_state_fixture.h $(BUILD_DIR)/locomotion_fixture.h $(BUILD_DIR)/object_fixture.h $(BUILD_DIR)/throw_fixture.h $(BUILD_DIR)/attack_fixture.h $(BUILD_DIR)/cull_fixture.h $(BUILD_DIR)/action_fixture.h $(BUILD_DIR)/state_fixture.h $(BUILD_DIR)/mission_collision.h $(BUILD_DIR)/vehicle_anim.h $(BUILD_DIR)/vehicle_wreckage.h $(BUILD_DIR)/soldier_fixture.h $(BUILD_DIR)/rng_fixture.h $(BUILD_DIR)/animation_fixture.h $(BUILD_DIR)/contact_fixture.h $(BUILD_DIR)/contact_tables.h $(BUILD_DIR)/control_fixture.h $(BUILD_DIR)/control_tables.h
	mkdir -p $(BUILD_DIR)/host
	$(HOST_CC) $(HOST_CFLAGS) -I$(BUILD_DIR) tools/test_gameplay.c src/gameplay.c src/neo_collision.c src/neo_vehicle.c -o $(BUILD_DIR)/host/gameplay
	$(HOST_CC) $(HOST_CFLAGS) -I$(BUILD_DIR) tools/test_neo_collision.c src/neo_collision.c -o $(BUILD_DIR)/host/collision
	$(HOST_CC) $(HOST_CFLAGS) tools/test_neo_motion.c -o $(BUILD_DIR)/host/motion
	$(HOST_CC) $(HOST_CFLAGS) tools/test_scene_codec.c -o $(BUILD_DIR)/host/codec
	$(HOST_CC) $(HOST_CFLAGS) tools/test_scene_clock.c -o $(BUILD_DIR)/host/clock
	$(BUILD_DIR)/host/gameplay
	$(BUILD_DIR)/host/collision
	$(BUILD_DIR)/host/motion
	$(BUILD_DIR)/host/codec
	$(BUILD_DIR)/host/clock
	python3 tools/test_neo_scene.py
	python3 tools/test_collision_audit.py
	python3 tools/check_scene_budget.py
	python3 tools/test_vehicle_wreckage.py
	$(HOST_CC) $(HOST_CFLAGS) -I$(BUILD_DIR) tools/test_neo_vehicle.c src/neo_vehicle.c src/neo_collision.c src/gameplay.c -o $(BUILD_DIR)/host/vehicle
	$(BUILD_DIR)/host/vehicle
	$(HOST_CC) $(HOST_CFLAGS) -I$(BUILD_DIR) tools/test_neo_soldier.c src/neo_soldier.c src/neo_rng.c -o $(BUILD_DIR)/host/soldier
	$(BUILD_DIR)/host/soldier
	$(HOST_CC) $(HOST_CFLAGS) -I$(BUILD_DIR) tools/test_neo_rng.c src/neo_soldier.c src/neo_rng.c -o $(BUILD_DIR)/host/rng
	$(BUILD_DIR)/host/rng
	$(HOST_CC) $(HOST_CFLAGS) -I$(BUILD_DIR) tools/test_neo_animation.c src/neo_animation.c -o $(BUILD_DIR)/host/animation
	$(BUILD_DIR)/host/animation
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
	$(HOST_CC) $(HOST_CFLAGS) -DRETRO_REAL_ASSETS -I$(BUILD_DIR)/retro25d tools/test_retro25d.c src/retro25d.c src/retro_events.c -lm -o $(BUILD_DIR)/host/retro25d
	$(BUILD_DIR)/host/retro25d
-include $(wildcard $(BUILD_DIR)/src/*.d $(BUILD_DIR)/tools/*.d)
