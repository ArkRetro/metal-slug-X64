#include <assert.h>
#include <stdio.h>
#include "../src/marco_campaign_palette.h"

int main(void) {
    /* Independent source words at BEDF8, BEDD8 and BEDD0 respectively. */
    static const uint16_t source[3][4]={
        {0x0391,0x0392,0x00a0,0x00a1},
        {0x03f8,0x03f9,0x01be,0x01bf},
        {0x0376,0x0377,0x0004,0x0005}
    };
    uint16_t descriptor=0;
    for(unsigned key=0;key<3;++key) for(unsigned character=0;character<4;++character) {
        assert(marco_campaign_palette_lookup(NULL,(marco_common_palette_t)key,character,&descriptor));
        assert(descriptor==source[key][character]);
    }
    descriptor=0x1234;
    assert(!marco_campaign_palette_lookup(NULL,MCP_361C,4,&descriptor));
    assert(!marco_campaign_palette_lookup(NULL,MCP_361C,0x8000,&descriptor));
    assert(!marco_campaign_palette_lookup(NULL,MCP_361C,0xffff,&descriptor));
    assert(!marco_campaign_palette_lookup(NULL,(marco_common_palette_t)3,0,&descriptor));
    assert(!marco_campaign_palette_lookup(NULL,MCP_361C,0,NULL));
    assert(descriptor==0x1234);
    puts("PASS: 12 original common palette words and unsupported character/key bounds");
}
