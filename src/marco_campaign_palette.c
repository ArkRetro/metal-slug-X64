#include "marco_campaign_palette.h"

bool marco_campaign_palette_lookup(void *context,marco_common_palette_t key,
    uint16_t character,uint16_t *descriptor) {
    (void)context;
    if(!descriptor || character>=4) return false;
    /* PC-relative LEA destinations, followed by C3620..C3626's word index. */
    static const uint16_t table_35fc[4]={0x0391,0x0392,0x00a0,0x00a1}; /* BEDF8 */
    static const uint16_t table_3614[4]={0x03f8,0x03f9,0x01be,0x01bf}; /* BEDD8 */
    static const uint16_t table_361c[4]={0x0376,0x0377,0x0004,0x0005}; /* BEDD0 */
    const uint16_t *table;
    switch(key) {
    case MCP_35FC:table=table_35fc;break;
    case MCP_3614:table=table_3614;break;
    case MCP_361C:table=table_361c;break;
    default:return false;
    }
    *descriptor=table[character];return true;
}
