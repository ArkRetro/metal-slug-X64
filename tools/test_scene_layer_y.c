/* Screen rows of pack layer cells (scene_layer_rise, scene_assets.h) against
 * MAME 0.264 run4 VRAM (captures-fullgame/run4/vram<frame>.bin, SCB3 rows
 * of the background columns, tools/neo_scene.py columns): for one cell of
 * each layer found once in the stage's map.json (layer, x, code), its map y,
 * the foreground's vertical offset at that frame (layer 3, the port's camera
 * height), the layer's y rule of the pack (tools/pack_mission.py
 * layer_rules) and the row MAME shows it at (SCB y - 16, the row of layer 3
 * cells at map y + offset). Layers 0/1 were drawn 8 px high before
 * (map.json reads them with Mission 1's Y_PHASE 8). */
#include <stdio.h>
#include "../src/scene_assets.h"
static const struct { unsigned layer;int map_y,height;uint8_t y_num,y_den;int row; } cells[]={
    {0,168,0,1,1,176},  /* Mission 2 stage 1, 19680: x 512 code 417E0 (the water strip) */
    {1,8,0,1,1,16},     /* 19680: x 128 code 40F85 */
    {3,16,0,1,1,16},    /* 19680: x 512 code 413F3 */
    {3,-64,66,1,1,2},   /* stage 2, 22410: x 1024 code 42079 */
    {3,0,16,1,1,16},    /* stage 3, 26670: x 512 code 42E53 */
    {3,-64,80,1,1,16},  /* 28620: x 512 code 42D67 */
    {1,-8,6,1,1,6},     /* Mission 3, 119100: x 1136 code 44755 */
    {3,96,6,1,1,102},   /* 119100: x 1024 code 44B63 */
    {1,184,3,1,1,195},  /* 119460: x 1088 code 44976 */
    {3,96,3,1,1,99},    /* 119460: x 1136 code 44B5E */
};
int main(void) {
    unsigned fails=0;
    for (unsigned i=0;i<sizeof(cells)/sizeof(cells[0]);i++) {
        scene_layer_rule_t r={1,1,cells[i].y_num,cells[i].y_den,0,0};
        int row=cells[i].map_y+scene_layer_rise(&r,cells[i].layer,cells[i].height);
        if (row!=cells[i].row) {fails++;fprintf(stderr,"layer %u map y %d height %d: row %d, MAME %d\n",cells[i].layer,cells[i].map_y,cells[i].height,row,cells[i].row);}
    }
    printf("scene layer rows: %u cells, %u differences\n",(unsigned)(sizeof(cells)/sizeof(cells[0])),fails);
    return fails!=0;
}
