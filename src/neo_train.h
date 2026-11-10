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
