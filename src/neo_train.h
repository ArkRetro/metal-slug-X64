/* Misión 3: movimiento propio de la capa del tren 10D7B6 (docs/MISION3.md).
 * Traducción del conductor 0B8D64 (velocidad propia 10C6B8/10C6BC -> RAM
 * 10E94E), de su paso de capa 0C81C0/0C6F9E (bit 0 de 10E957: la capa usa
 * 10E94E en vez de la velocidad de la raíz 10E970) y del bucle de columnas
 * 0B8DD0 (+32 avanza 0x20 columnas mientras 10C6B6 lo pida, tabla 0B8E36).
 * Las direcciones ROM son solo procedencia. */
#ifndef NEO_TRAIN_H
#define NEO_TRAIN_H
#include <stdbool.h>
#include <stdint.h>
typedef struct {
    bool on;            /* 0B8D34 creó el conductor 0B8D64 */
    uint8_t mode;       /* 10C6BC: 0 parado, 1 (-2 px), 2 (-12 px, raíz x 3/4) */
    int32_t speed;      /* 10C6B8 (Q16.16) */
    int32_t pending;    /* 10E94E de la actualización anterior */
    int32_t x;          /* capa +14 (Q16.16) */
    int16_t origin;     /* capa +32: origen de columnas (12 bits con signo) */
    int16_t index;      /* 10C6B4 (bytes en 0B8E36) */
    uint16_t count;     /* 10C6B6 (0xFFFF = sin fin) */
    uint8_t sky;        /* 0B8EDC +6E / 6: cuadros del cielo ya copiados (0..4) */
    /* Anillo de columnas de 10D7B6 (0C748E/0C74C4/0C6BDE): +36 (columna local
     * de la x de pantalla -64) y, por ranura (columna local & 31), la columna
     * del mapa 0B8566 que se cargó en ella con el origen +32 de ese momento
     * (NEO_TRAIN_EMPTY: aún no cargada). Al cambiar el origen, las columnas ya
     * cargadas no se recargan: el nuevo origen entra por el borde. */
    int16_t o36;
    int16_t ring[32];
    /* Capa del cielo 10E83E (paso de capa 0C6F9E): +14 (Q16.16), +2A (resto
     * de la fracción), +26 (factor x, op 0x1C; 0x100 = 1:1). */
    int32_t sky_x;
    uint8_t sky_frac;
    int16_t sky_factor;
} neo_train_t;
#define NEO_TRAIN_EMPTY ((int16_t)0x7fff)
#define NEO_TRAIN_MAP_COLUMNS 392   /* 0B8566 +8: columnas de bloque (16 px) */
/* x = +14 de la capa (y del cielo) en ese momento: 0B8D34 (progreso 0), las
 * dos han seguido 1:1 a la raíz con el origen 0. */
void neo_train_reset(neo_train_t *t,int32_t x);
/* Llamadas de la lista de cámara (op 0x2C/0x58) del tren; false: no es suya. */
bool neo_train_call(neo_train_t *t,uint32_t routine);
/* Una actualización: root_dx = paso de la raíz de esta actualización (10E970,
 * el desplazamiento de 10C72E), own = bit 0 de 10E957 (op 0x3C). */
void neo_train_update(neo_train_t *t,int32_t root_dx,bool own);
/* Igual, con el bit 0 de 10E958 (el cielo usa también 10E94E). */
void neo_train_update_layers(neo_train_t *t,int32_t root_dx,bool own,bool sky_own);
/* Op 0x1C de la lista de cámara sobre 10E83E: su factor x (+26). */
void neo_train_sky_factor(neo_train_t *t,int16_t factor);
/* 0C6F9E: velocidad x de una capa para el paso `step` (Q16.16) con el factor
 * +26 y el resto +2A (que actualiza). */
int32_t neo_layer_step(int32_t step,int16_t factor,uint8_t *frac);
/* Layer objects 10C72E / 10D7B6 / 10E83E / 10E8C6 as the camera list moves
 * them (docs/MISION4.md): +0/+4 position (Q16.16; +14/+18 without a parent,
 * 01ABCC), +26/+28 factors (op 0x1C, 0C86C8), +2A/+2B remainders, the map of
 * the last op 0x18 (0C8654: +2E map, +32 block column, +34 top block row).
 * 0C81C0 steps each one by the root's velocity through 0C6F9E unless its
 * flag byte 10E956+i (op 0x3C) gives it another speed; ops 0x14 / 0x44
 * (0C6FF2) put it at the root with its remainders cleared. */
typedef struct {
    int32_t x,y; int16_t fx,fy; uint8_t frac_x,frac_y;
    uint32_t map; int16_t column,top; uint16_t loads;
    /* Op 0x48 (0C73CE) with (0, 0, 0x20 columns): the map's 32 columns are
     * copied once into the object's 32 sprite columns, so the picture
     * repeats every 512 px; ring_base = the 512-px step of the layer's x
     * at that moment (the pack holds the picture from there). */
    bool ring; int32_t ring_base;
} neo_layer_t;
enum { NEO_LAYER_FORE,NEO_LAYER_MID,NEO_LAYER_SKY,NEO_LAYER_OBJ4,NEO_LAYERS };
int neo_layer_index(uint32_t object);
/* One camera-list event (kind = op, object, list offset `at` of the op in
 * `list`) at the root position (6968/696C). */
void neo_layers_event(neo_layer_t l[NEO_LAYERS],const uint8_t *list,uint8_t kind,uint32_t object,uint16_t at,int32_t root_x,int32_t root_y);
/* 0C81C0 after the camera update: the root's velocity 6970/6974 through each
 * layer's factors; a layer whose flag byte is not 0 is left to its owner. */
void neo_layers_step(neo_layer_t l[NEO_LAYERS],int32_t root_vx,int32_t root_vy,const uint8_t flags[4]);
/* 0C81C0 for one layer object as bytes (0C8240 for 10D7B6): flag byte bit
 * 7 (RAM 10E956+i, 0C7BD0): 01AC3C by its own +8/+C; otherwise the speed
 * is 694E (bit 0) or the root's 6970, 6952 (bit 1) or 6974 (own[] /
 * root_v*), through 0C6F9E (+8/+C by the factors +26/+28 and remainders
 * +2A/+2B, then 01AC3C), and 69CC (`shake`) is added to +18. 01ABCC: the
 * parent +10 (the scene root at root_x/root_y with its +25, or none),
 * +1C/+20 the previous +14/+18 once per epoch (RAM 10C33A). */
void neo_layerobj_step(uint8_t *l,uint8_t flags,const int32_t own[2],int32_t root_vx,int32_t root_vy,int32_t root_x,int32_t root_y,uint8_t root_correction,uint8_t epoch,int32_t shake);
/* +84 de una capa sin padre (0C75B2): palabra alta de +14. */
static inline int16_t neo_layer_word(int32_t x) {return (int16_t)(uint16_t)((uint32_t)x>>16);}
/* Cuadro del cielo 10E83E (0B8EDC): 0 = el mapa 0B857C de la etapa, 1..4 =
 * 0B8592 / 0B85A8 / 0B85BE / 0B85D4, copiados por 0C714E cuando la columna
 * de la tabla 0B8EC2 (0x50, 0xB0, 0x110, 0x160) más el origen +32 entra
 * por la derecha: (+36 << 4) + 0x1F0 - ((columna + +32) << 4) >= 0. */
unsigned neo_train_sky(const neo_train_t *t);
/* Columna (8 px) del mapa 0B8566 bajo el x local de la capa. */
int neo_train_column(const neo_train_t *t,int local_x);
#endif
