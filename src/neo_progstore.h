#ifndef NEO_PROGSTORE_H
#define NEO_PROGSTORE_H
#include <stdbool.h>
#include <stdint.h>
#include "neo_animation.h"
/* Animation programs read on demand (docs/MEMORY_OPTIMIZATION.md "Programs on
 * demand"). The console image holds one reference per program
 * (soldier_program_refs: tag, offset, count); the commands are the 10-byte
 * records of build/programs.bin (tools/compile_soldier_ai_tables.py), read
 * from the cartridge's file system the first time a program is asked for and
 * kept until neo_progstore_reset. Same values as the tables they replace
 * (tools/test_program_file.c compares every program). */
/* Reads `bytes` of the program file at `offset` into dst (16-byte aligned,
 * bytes rounded up to 2); false when the file cannot be read. */
typedef bool (*neo_progstore_read_t)(void *dst,uint32_t offset,uint32_t bytes);
/* The reader and the fixed arena the commands are kept in (16-byte aligned).
 * A program that does not fit takes its own heap block (counted). */
void neo_progstore_setup(neo_progstore_read_t read,uint8_t *arena,uint32_t arena_bytes);
/* The commands of `count` records at `offset`; NULL when they cannot be
 * read (explicit stop: the caller has no program). */
const neo_anim_command_t *neo_progstore_load(uint32_t offset,unsigned count);
/* The same with the program (commands and count) in the 16 bytes before
 * the commands, in the same store: the caller keeps one pointer a program
 * instead of the 8-byte struct (docs/MEMORY_OPTIMIZATION.md). */
const neo_anim_program_t *neo_progstore_load_program(uint32_t offset,unsigned count);
/* Forgets every program read (the caller drops its pointers first). */
void neo_progstore_reset(void);
typedef struct { unsigned programs,bytes,peak_bytes,heap_blocks,heap_bytes,failed; } neo_progstore_stats_t;
neo_progstore_stats_t neo_progstore_stats(void);
#endif
