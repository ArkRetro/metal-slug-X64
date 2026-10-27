.DEFAULT_GOAL := all
N64_INST ?= /workspace/toolchains/package/opt/libdragon
export N64_INST
BUILD_DIR := build
N64_MKDFS_ROOT := romfs
include $(N64_INST)/include/n64.mk
N64_CFLAGS += -I$(BUILD_DIR)

.PHONY: all clean check
all: mslugx-explorer.z64
$(BUILD_DIR)/mslugx-explorer.elf: $(BUILD_DIR)/src/main.o $(BUILD_DIR)/src/actor.o $(BUILD_DIR)/src/gameplay.o $(BUILD_DIR)/src/neo_collision.o $(BUILD_DIR)/src/scene_assets.o $(BUILD_DIR)/src/scene_renderer.o
mslugx-explorer.z64: N64_ROM_TITLE = "MSX COLLISION v10"
mslugx-explorer.z64: $(BUILD_DIR)/mslugx-explorer.dfs
$(BUILD_DIR)/mslugx-explorer.dfs: romfs/scene.pack romfs/fixed.bin romfs/sprites.bin
$(BUILD_DIR)/scene_meta.h romfs/scene.pack romfs/fixed.bin romfs/sprites.bin &: tools/pack_scene.py tools/mission_tiles.py tools/export_marco.py filesystem/mission1/map.json filesystem/mission1/props.json filesystem/marco.idx4 filesystem/marco-actions.idx4 filesystem/marco-motion.idx4 filesystem/soldier.idx4 filesystem/fixed.bin filesystem/sprites.bin filesystem/marco.bin filesystem/marco-actions.bin filesystem/desert.bin filesystem/soldier.bin filesystem/marco-motion.bin
	python3 tools/pack_scene.py
$(BUILD_DIR)/src/scene_assets.o: $(BUILD_DIR)/scene_meta.h
$(BUILD_DIR)/src/neo_collision.o: $(BUILD_DIR)/mission_collision.h
$(BUILD_DIR)/mission_collision.h: tools/compile_collision.py filesystem/mission1/collision.json
	python3 tools/compile_collision.py
filesystem/fixed.bin filesystem/sprites.bin:
	@echo 'Primero: python3 tools/import_rom.py /ruta/mslugx.zip' >&2
	@exit 1
filesystem/marco.bin filesystem/marco-actions.bin filesystem/desert.bin filesystem/soldier.bin filesystem/marco-motion.bin:
	@echo 'Falta $@: consultar docs/MARCO.md, docs/ACTION.md y docs/ENCOUNTER.md' >&2
	@exit 1
clean:
	rm -rf build mslugx-explorer.z64
check: $(BUILD_DIR)/mission_collision.h
	mkdir -p $(BUILD_DIR)/host
	cc -std=c11 -Wall -Wextra -Werror -I$(BUILD_DIR) tools/test_gameplay.c src/gameplay.c src/neo_collision.c -o $(BUILD_DIR)/host/gameplay
	cc -std=c11 -Wall -Wextra -Werror -I$(BUILD_DIR) tools/test_neo_collision.c src/neo_collision.c -o $(BUILD_DIR)/host/collision
	cc -std=c11 -Wall -Wextra -Werror tools/test_neo_motion.c -o $(BUILD_DIR)/host/motion
	cc -std=c11 -Wall -Wextra -Werror tools/test_scene_codec.c -o $(BUILD_DIR)/host/codec
	cc -std=c11 -Wall -Wextra -Werror tools/test_scene_clock.c -o $(BUILD_DIR)/host/clock
	$(BUILD_DIR)/host/gameplay
	$(BUILD_DIR)/host/collision
	$(BUILD_DIR)/host/motion
	$(BUILD_DIR)/host/codec
	$(BUILD_DIR)/host/clock
-include $(wildcard $(BUILD_DIR)/src/*.d)
