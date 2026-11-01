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
} neo_train_t;
void neo_train_reset(neo_train_t *t,int32_t x);
/* Llamadas de la lista de cámara (op 0x2C/0x58) del tren; false: no es suya. */
bool neo_train_call(neo_train_t *t,uint32_t routine);
/* Una actualización: root_dx = paso de la raíz de esta actualización (10E970,
 * el desplazamiento de 10C72E), own = bit 0 de 10E957 (op 0x3C). */
void neo_train_update(neo_train_t *t,int32_t root_dx,bool own);
/* Desplazamiento de dibujo de la capa: -x - 16 * origen (módulo 65536). */
int32_t neo_train_scroll(const neo_train_t *t);
/* Columna (8 px) del mapa 0B8566 bajo el x local de la capa. */
int neo_train_column(const neo_train_t *t,int local_x);
#endif
