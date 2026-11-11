/* Playable characters: see msx_character.h. */
#include "msx_character.h"
#include <stdlib.h>
#include <string.h>
#include "marco_visual.h"
#include "marco_death.h"
#include "scene_codec.h"
#define be16 msx_character_be16
#define be32 msx_character_be32
static unsigned current;
uint32_t msx_character_file_size(const uint8_t *f,uint32_t n) {
    if (!f || n<20 || memcmp(f,"MSXZ",4) || be16(f+4)!=1) return 0;
    uint32_t raw=be32(f+8),block=be32(f+12),blocks=be32(f+16);
    if (!raw || block!=65536u || blocks!=(raw+block-1)/block || (uint64_t)20+blocks*4u>n) return 0;
    return raw;
}
bool msx_character_unpack(const uint8_t *f,uint32_t n,uint8_t *out,uint32_t out_size) {
    uint32_t raw=msx_character_file_size(f,n);
    if (!raw || raw!=out_size) return false;
    uint32_t block=be32(f+12),blocks=be32(f+16);uint64_t at=20+(uint64_t)blocks*4u;
    for (uint32_t i=0;i<blocks;i++) {
        uint32_t stored=be32(f+20+i*4u),length=stored&0x7fffffffu,want=raw-i*block<block?raw-i*block:block;
        if (at+length>n) return false;
        if (stored&0x80000000u) {if (length!=want) return false;memcpy(out+i*block,f+at,length);}
        else if (want==block) {if (!scene_decode_lz4(out+i*block,block,f+at,length)) return false;}
        else {
            /* The last block was packed padded with zeros to a whole one. */
            uint8_t *whole=malloc(block);bool ok=whole && scene_decode_lz4(whole,block,f+at,length);
            if (ok) memcpy(out+i*block,whole,want);
            free(whole);if (!ok) return false;
        }
        at+=length;
    }
    return at==n;
}
bool msx_character_layout(const uint8_t *b,uint32_t size,msx_character_layout_t *o) {
    if (!b || size<64 || memcmp(b,"MSXP",4) || be16(b+4)!=1) return false;
    memset(o,0,sizeof *o);o->blob=b;o->size=size;
    o->character=be16(b+6);o->commands=be16(b+8);o->roles=be16(b+10);o->poses=be16(b+12);o->parts=be16(b+14);
    o->palettes=be16(b+16);unsigned deaths=be16(b+18);o->death_commands=be16(b+20);
    o->off_commands=be32(b+24);o->off_roles=be32(b+28);o->off_death=be32(b+32);o->off_poses=be32(b+36);
    o->off_parts=be32(b+40);o->off_palettes=be32(b+44);o->off_pixels=be32(b+48);
    if (be32(b+52)!=size || o->character>=MSX_CHARACTERS || deaths!=MSX_CHARACTER_DEATHS || !o->commands || !o->roles) return false;
    return (uint64_t)o->off_commands+o->commands*10u<=size && (uint64_t)o->off_roles+o->roles*4u<=size &&
        (uint64_t)o->off_death+deaths*4u+o->death_commands*10u<=size && (uint64_t)o->off_poses+o->poses*12u<=size &&
        (uint64_t)o->off_parts+o->parts*16u<=size && (uint64_t)o->off_palettes+o->palettes*32u<=size && o->off_pixels<=size;
}
/* One command of the file: kind u8, asset s24, target u16, operand u8,
 * flags u8, dx s8, dy s8 (the fields of neo_anim_command_t). */
static void command(neo_anim_command_t *c,const uint8_t *s) {
    int32_t asset=(int32_t)((uint32_t)s[1]<<16|(uint32_t)s[2]<<8|s[3]);
    if (asset&0x800000) asset-=0x1000000;
    c->kind=(neo_anim_kind_t)s[0];c->asset=asset;c->target=be16(s+4);c->operand=s[6];c->flags=s[7];c->dx=(int8_t)s[8];c->dy=(int8_t)s[9];
}
bool msx_character_open(msx_character_pack_t *p,const uint8_t *blob,uint32_t size) {
    memset(p,0,sizeof *p);
    if (!msx_character_layout(blob,size,&p->layout)) return false;
    const msx_character_layout_t *l=&p->layout;
    p->commands=malloc(l->commands*sizeof *p->commands);p->roles=malloc(l->roles*sizeof *p->roles);
    p->death_commands=malloc((l->death_commands?l->death_commands:1u)*sizeof *p->death_commands);
    if (!p->commands || !p->roles || !p->death_commands) {msx_character_close(p);return false;}
    for (unsigned i=0;i<l->commands;i++) command(&p->commands[i],blob+l->off_commands+i*10u);
    p->script=(neo_anim_program_t){p->commands,(uint16_t)l->commands};
    for (unsigned i=0;i<l->roles;i++) {
        p->roles[i][0]=be16(blob+l->off_roles+i*4u);p->roles[i][1]=be16(blob+l->off_roles+i*4u+2);
        if (p->roles[i][1]>=l->commands || (i && p->roles[i][0]<=p->roles[i-1][0])) {msx_character_close(p);return false;}
    }
    const uint8_t *d=blob+l->off_death;
    for (unsigned i=0;i<l->death_commands;i++) command(&p->death_commands[i],d+MSX_CHARACTER_DEATHS*4u+i*10u);
    for (unsigned i=0;i<MSX_CHARACTER_DEATHS;i++) {
        unsigned first=be16(d+i*4u),count=be16(d+i*4u+2);
        if (!count || first+count>l->death_commands) {msx_character_close(p);return false;}
        p->death[i]=(neo_anim_program_t){p->death_commands+first,(uint16_t)count};
    }
    return true;
}
void msx_character_close(msx_character_pack_t *p) {
    free(p->commands);free(p->roles);free(p->death_commands);memset(p,0,sizeof *p);
}
void msx_character_use(const msx_character_pack_t *p) {
    if (p && p->commands) {
        current=p->layout.character;
        marco_visual_use_programs(&p->script,(const uint16_t (*)[2])p->roles,p->layout.roles);marco_death_use_programs(p->death);
    } else {current=MSX_CHARACTER_MARCO;marco_visual_use_programs(NULL,NULL,0);marco_death_use_programs(NULL);}
}
unsigned msx_character_current(void) {return current;}
unsigned msx_character_of_place(unsigned place) {
    static const uint8_t order[MSX_CHARACTERS]={MSX_CHARACTER_MARCO,MSX_CHARACTER_ERI,MSX_CHARACTER_TARMA,MSX_CHARACTER_FIO};
    return order[place&3u];
}
