/* Fix-layer text engine, manual translation of ROM 03CF9A, 03CFBA, 03D1B8,
 * 03D25A and 03DD96..03E192 (labels are provenance only). The VRAM writes
 * are `move.l d0,$3C0000` with the address in the high word and the data in
 * the low word; the long arithmetic between them (addi.l #$10010 ...) is
 * kept as 32-bit arithmetic so a carry behaves as in the original. */
#include "neo_text.h"
#include <stdio.h>
#include "neo_gactor.h"   /* ga_rom: the font tables are ROM data */

static void stop(neo_text_t *t,const char *what,unsigned value) {
    if (!t->unsupported++) fprintf(stderr,"TEXT: explicit stop %s %X\n",what,value);
}
void neo_text_word(neo_text_t *t,uint16_t address,uint16_t word) {
    if (t->on_write) t->on_write(t->ctx,address,word);
    if (address<0x7000 || address>=0x7500) {stop(t,"VRAM address outside the fix map",address);return;}
    t->hud->fix[address-0x7000]=word;t->hud->serial++;
}
static void put_long(neo_text_t *t,uint32_t d0) {neo_text_word(t,(uint16_t)(d0>>16),(uint16_t)d0);}
static uint16_t rom_word(neo_text_t *t,uint32_t address) {
    const uint8_t *p=ga_rom(address);
    if (!p) {stop(t,"font table",address);return 0;}
    return (uint16_t)(p[0]<<8|p[1]);
}
/* 03CF9A: one tile (palette d1>>8, page d1 & F, glyph d2), next column. */
static uint16_t cf9a(neo_text_t *t,uint16_t d0,uint16_t d1,uint8_t d2) {
    uint16_t w=(uint16_t)((uint16_t)((d1&0xff00u)|(uint8_t)(d1<<4))<<4);
    w=(uint16_t)((w&0xff00u)|d2);
    neo_text_word(t,d0,w);
    return (uint16_t)(d0+0x20);
}
/* 03DEDE: a 2x2 block (A, A+1, A+0x21, A+0x20), two columns. */
static uint16_t block2x2(neo_text_t *t,uint16_t a,uint16_t w) {
    uint32_t d0=(uint32_t)a<<16|w;
    put_long(t,d0);d0+=0x10010u;put_long(t,d0);d0+=0x200001u;put_long(t,d0);d0-=0x10010u;put_long(t,d0);
    return (uint16_t)((d0>>16)+0x20);
}
/* 03DFEA: the table word with the palette of d1, one column. */
static uint16_t table_glyph(neo_text_t *t,uint16_t a,uint16_t word,uint16_t d1) {
    neo_text_word(t,a,(uint16_t)(word|((uint16_t)(d1<<4)&0xf000u)));
    return (uint16_t)(a+0x20);
}
/* 03DEB4: page 'A' (and 'X' below 0x80 / A0..DF): 2x2 tiles from 0x800. */
static uint16_t page_a(neo_text_t *t,uint16_t a,uint16_t d1,uint8_t d2) {
    uint16_t w=(uint16_t)((uint16_t)((d1&0xff00u)|0x80u)<<4);
    uint8_t lo=(uint8_t)((d2&0xf0u)*2u);
    w=(uint16_t)((w&0xff00u)|lo);
    uint8_t low=(uint8_t)(d2&15u);
    if (low>=8) w=(uint16_t)(w+0x100u);
    w=(uint16_t)(w|(uint8_t)((low&7u)*2u));
    return block2x2(t,a,w);
}
uint16_t neo_text_glyph(neo_text_t *t,uint16_t d0,uint16_t d1,uint8_t d2,const uint8_t **a0) {
    uint8_t page=(uint8_t)d1;
    if (page<0x10) return cf9a(t,d0,d1,d2);
    /* 03DDA8: DE/DF prefixes (dakuten/handakuten) after the glyph. */
    uint8_t d7=**a0;
    if (d7==0xde) {
        if (d2==0xb3) {(*a0)++;d2=0xe0;}
        else if (d2>=0xb6 && d2<=0xc4) {(*a0)++;d2=(uint8_t)(d2+0x2b);}
        else if (d2>=0xca && d2<=0xce) {(*a0)++;d2=(uint8_t)((d2-0xca)*2+0xf0);}
    } else if (d7==0xdf && d2>=0xca && d2<=0xce) {(*a0)++;d2=(uint8_t)((d2-0xca)*2+0xf1);}
    switch (page) {
    case 0x30: return cf9a(t,d0,(uint16_t)((d1&0xff00u)|3u),d2);
    case 0x61: {   /* 03DE3C: two-row font from 0xA00 */
        uint16_t w=(uint16_t)((uint16_t)((d1&0xff00u)|0xa0u)<<4);
        uint8_t lo=(uint8_t)((uint8_t)((d2&0xf0u)*2u)+(d2&15u));
        uint32_t l=(uint32_t)d0<<16|(uint16_t)((w&0xff00u)|lo);
        put_long(t,l);l+=0x10010u;put_long(t,l);
        return (uint16_t)((l>>16)+0x1f);
    }
    case 0x48: case 0x4b: {   /* 03DE78: tables 03D596 / 03D796, 2x2 */
        uint32_t table=page==0x48?0x3d596u:0x3d796u;
        uint16_t w=(uint16_t)(((uint16_t)(d1<<4)&0xf000u)|rom_word(t,table+2u*d2));
        return block2x2(t,d0,w);
    }
    case 0x41: return page_a(t,d0,d1,d2);
    case 0x31: {   /* 03DF12: three rows from 0x480 */
        uint16_t w=(uint16_t)((uint16_t)((d1&0xff00u)|0x40u)<<4);
        w=(uint16_t)((w&0xff00u)|(uint8_t)(d2+0x80u));
        uint32_t l=(uint32_t)d0<<16|w;
        put_long(t,l);l+=0x10010u;put_long(t,l);l+=0x10010u;put_long(t,l);
        return (uint16_t)((l>>16)+0x1e);
    }
    case 0x68: case 0x6b: {   /* 03DF52: tables 03DB96 / 03D996 with '@' accents */
        uint32_t table=page==0x68?0x3db96u:0x3d996u;
        if (d2=='@') {
            d2=*(*a0)++;uint8_t k=0xff;
            if (d2=='.') k=0;
            else if (d2==':') k=1;
            else if (d2=='~' || d2=='-') k=2;
            else if (d2=='`') d2=*(*a0)++;
            if (k!=0xff) {             /* 03DFAC */
                d2=*(*a0)++;
                if (d2=='a') d2=(uint8_t)(k+0x80);
                else if (d2=='u') d2=(uint8_t)(k+3+0x80);
                else if (d2=='e') d2=(uint8_t)(k+6+0x80);
                else if (d2=='o') d2=(uint8_t)(k+8+0x80);
            }
        }
        return table_glyph(t,d0,rom_word(t,table+2u*d2),d1);
    }
    case 0x58:     /* 03E002: 'A' glyphs or a two-byte code of 03D512 */
        if (d2<0x80 || (d2>=0xa0 && d2<0xe0)) return page_a(t,d0,d1,d2);
        {
            uint16_t code=(uint16_t)(d2<<8|*(*a0)++);
            uint32_t p=0x3d512u;uint16_t tile,key;
            do {tile=rom_word(t,p);key=rom_word(t,p+2);p+=4;} while (key!=code && key && !t->unsupported);
            return block2x2(t,d0,(uint16_t)(tile|((uint16_t)(d1<<4)&0xf000u)));
        }
    case 0x32:     /* 03E048: '@' marks */
        if (d2=='@') {
            d2=*(*a0)++;
            if (d2=='!') return table_glyph(t,d0,0x0bea,d1);
            if (d2=='c' || d2=='C') {
                uint16_t w=(uint16_t)((uint16_t)((d1&0xff00u)|0x30u)<<4);
                uint32_t l=(uint32_t)d0<<16|(uint16_t)((w&0xff00u)|d2);
                put_long(t,l);l+=0x10000u;
                l=(l&0xffff0000u)|((l&0x0f00u)|0x0bebu);
                put_long(t,l);
                return (uint16_t)((l>>16)+0x1f);
            }
            uint16_t pair=(uint16_t)(d2<<8|**a0);
            uint32_t list=0;
            switch ((uint8_t)pair) {
            case 'a': list=0x3e194;break; case 'i': list=0x3e1ce;break; case 'u': list=0x3e1d8;break;
            case 'e': list=0x3e1f2;break; case 'o': list=0x3e214;break; case 'A': list=0x3e23e;break;
            case 'I': list=0x3e260;break; case 'U': list=0x3e26a;break; case 'E': list=0x3e274;break;
            case 'O': list=0x3e28e;break;
            }
            if (list) for (;;) {
                uint16_t key=rom_word(t,list);list+=2;
                if (!key || t->unsupported) break;
                if (key==pair) {       /* 03E154 */
                    (*a0)++;
                    uint16_t w=(uint16_t)(((uint16_t)(d1<<4)&0xf000u)|rom_word(t,list));
                    uint32_t l=(uint32_t)d0<<16|w;
                    put_long(t,l);l-=0x10000u;
                    l=(l&0xffff0000u)|(uint16_t)((l&0xf000u)|rom_word(t,list+2));
                    put_long(t,l);
                    return (uint16_t)((l>>16)+0x21);
                }
                list+=6;
            }
            /* 03E138: the low byte (the following character) on page 3. */
            uint16_t w=(uint16_t)((uint16_t)((d1&0xff00u)|0x30u)<<4);
            neo_text_word(t,d0,(uint16_t)((w&0xff00u)|(uint8_t)pair));
            return (uint16_t)(d0+0x20);
        }
        return cf9a(t,d0,(uint16_t)((d1&0xff00u)|3u),d2);
    default:
        return cf9a(t,d0,(uint16_t)((d1&0xff00u)|3u),d2);
    }
}
/* 03D1B8: escapes; returns the next glyph (0 at the end). */
static uint8_t next_glyph(neo_text_t *t,const uint8_t **a0,uint16_t *d0,uint16_t *d1,uint16_t *d3) {
    for (;;) {
        uint8_t c=*(*a0)++;
        if (c!='\\') return c;
        uint8_t e=*(*a0)++;
        if (e=='n') {*d3=(uint16_t)(*d3+1);if (*d3>=0x7500) stop(t,"TRAP #F row",*d3);*d0=*d3;}
        else if (e=='c') *d1=(uint16_t)((*d1&0xff00u)|*(*a0)++);
        else if (e=='p') *d1=(uint16_t)((*d1&0xffu)|(uint16_t)((*(*a0)++&15u)<<8));
        else if (e=='l') {
            uint16_t r=(uint16_t)(*(*a0)++<<5);r=(uint16_t)((r&0xff00u)|(uint8_t)(r+*(*a0)++));
            *d3=(uint16_t)(r+0x7000);if (*d3>=0x7500) stop(t,"TRAP #F position",*d3);*d0=*d3;
        } else return e;   /* 03D1E6: an unknown escape returns its letter */
    }
}
uint16_t neo_text_string(neo_text_t *t,uint16_t d0,uint16_t *d1,const uint8_t *a0) {
    uint16_t d3=d0;
    if (!a0) {stop(t,"string",0);return d0;}
    for (;;) {
        uint8_t c=next_glyph(t,&a0,&d0,d1,&d3);
        if (!c) return d0;
        d0=neo_text_glyph(t,d0,*d1,c,&a0);
    }
}
/* 03D056: digit glyphs, index 16 = blank. */
static const uint8_t digits[18]="0123456789ABCDEF ";
uint16_t neo_text_bcd(neo_text_t *t,uint16_t d0,uint16_t d1,const uint8_t *a0,uint8_t d3,unsigned d4) {
    for (;d4;d4--) {
        unsigned d2;
        if (d4&1) d2=*a0++&15u; else d2=*a0>>4;
        if (d2 || d4==1) d3=0xff; else if (!d3) d2=16;
        const uint8_t *table=digits;   /* A0 = 03D056 during the glyph call */
        d0=neo_text_glyph(t,d0,d1,digits[d2],&table);
    }
    return d0;
}
