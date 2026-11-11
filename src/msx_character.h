#ifndef MSX_CHARACTER_H
#define MSX_CHARACTER_H
/* The playable characters (docs/PERSONAJES.md). The original keeps the
 * choice of SOLDIER SELECT in the player's word +6E (0 Marco, 1 Tarma,
 * 2 Eri, 3 Fio; the panel 0C961C writes it) and indexes with it every
 * 4-character table of the player: body and weapon programs (0C0F32,
 * 0C0F62, 0C0FB6, 0C0FCE: row of the form + 4 * character), palettes
 * (0BEDD0..), the death voice (0C294E).
 *
 * Marco is the built-in catalog (build/marco_campaign.h,
 * build/marco_pose_meta.h, romfs/marco). The other three come from a pack
 * in the romfs (rom:/chars/cN.bin, tools/export_character.py), read when
 * the character is chosen: the same roles with the programs of the
 * character's column, its death programs, its poses and its palettes.
 * Host-compilable (make check: tools/test_character.c). */
#include <stdbool.h>
#include <stdint.h>
#include "neo_animation.h"
enum { MSX_CHARACTER_MARCO,MSX_CHARACTER_TARMA,MSX_CHARACTER_ERI,MSX_CHARACTER_FIO,MSX_CHARACTERS };
enum { MSX_CHARACTER_DEATHS=5 };
/* Big-endian sections of the pack, as the file has them. */
typedef struct {
    const uint8_t *blob;uint32_t size;
    unsigned character,commands,roles,poses,parts,palettes,death_commands;
    uint32_t off_commands,off_roles,off_death,off_poses,off_parts,off_palettes,off_pixels;
} msx_character_layout_t;
typedef struct {
    msx_character_layout_t layout;
    neo_anim_command_t *commands;          /* decoded: the role programs */
    neo_anim_program_t script;
    uint16_t (*roles)[2];                  /* (Marco's entry, this character's entry), sorted */
    neo_anim_command_t *death_commands;
    neo_anim_program_t death[MSX_CHARACTER_DEATHS];
} msx_character_pack_t;
/* The file rom:/chars/cN.bin holds the pack in LZ4 blocks ('MSXZ';
 * tools/export_character.py, the block decoder of scene_codec.h).
 * msx_character_file_size: the pack's size, 0 when the file is not one.
 * msx_character_unpack: the pack into `out` (that many bytes). */
uint32_t msx_character_file_size(const uint8_t *file,uint32_t file_size);
bool msx_character_unpack(const uint8_t *file,uint32_t file_size,uint8_t *out,uint32_t out_size);
/* Checks the header and the section bounds of a pack held in memory. */
bool msx_character_layout(const uint8_t *blob,uint32_t size,msx_character_layout_t *out);
/* Decodes the programs of a pack (the blob must outlive it only for the
 * renderer's sections). False: not a pack of version 1 or out of memory. */
bool msx_character_open(msx_character_pack_t *p,const uint8_t *blob,uint32_t size);
void msx_character_close(msx_character_pack_t *p);
/* The character of the next game: its programs go to marco_visual.c and
 * marco_death.c. NULL: Marco. */
void msx_character_use(const msx_character_pack_t *p);
unsigned msx_character_current(void);
/* 0C294E[+6E]: the death voice requested by 0C2956/0C2D6A. */
static inline uint16_t msx_character_death_voice(unsigned character) {
    static const uint16_t voice[MSX_CHARACTERS]={0x1123,0x1087,0x1125,0x114e};
    return voice[character<MSX_CHARACTERS?character:0];
}
/* Screen order of SOLDIER SELECT (MAME 0.264, inputs only): place 0..3 =
 * Marco, Eri, Tarma, Fio -> +6E 0, 2, 1, 3. */
unsigned msx_character_of_place(unsigned place);
static inline uint16_t msx_character_be16(const uint8_t *p) {return (uint16_t)(p[0]<<8|p[1]);}
static inline uint32_t msx_character_be32(const uint8_t *p) {return (uint32_t)p[0]<<24|(uint32_t)p[1]<<16|(uint32_t)p[2]<<8|p[3];}
#endif
