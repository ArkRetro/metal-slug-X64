# Siguientes pasos

1. **Cerrar B04–B07** (rama `work-m1m2flow`, árbol `/home/user/ws-world`):
   - R al llegar al jefe de la M1.
   - Transición M1 → M2.
   - Música al empezar la M2.
   - Desplazamiento del suelo al inicio de la M2.

   Prueba: con la ROM normal desde el arranque, ares jugando la M1 hasta el
   final y comparando la secuencia con una captura de MAME.
2. **Cerrar B08–B10** (rama `work-m2events`, árbol `/home/user/ws-m3`):
   - Inventario de actores de la M2 en MAME frente al port.
   - Tabla de eventos.
   - Dibujo de la forma momia.
3. **Fusionar las dos ramas en `ccr-09eb2f6b-8rwl8l`** y construir la ROM
   21 en `/home/user/ws-logo`. Copiar las entradas privadas con `cp` sin
   `-a` ni `-p`: si se conservan las fechas antiguas, `make` no recompila
   (fue la causa de B01).
   Antes de entregar:
   - Pasar todas las puertas (`PORT_STATUS.md`).
   - Comprobar `native animation loaded (562 poses)` en el registro.
   - Probar la ROM normal, no solo la de referencia.
4. **Hecho (rama `work-diag`):** modo de diagnóstico `MSX_DIAG`
   (`PORT_STATUS.md` «Instrumentación») y RAM de la caché recuperada: 218
   ranuras en la ROM normal y 217 en las de referencia (215/216 con
   `work-laser` fusionada; `RENDIMIENTO.md`). Al fusionar `work-laser`,
   resolver el conflicto de `neo_world.c` añadiendo
   `MSX_DIAG_POOL_FAIL(MSX_POOL_SHOT)` a sus peticiones de disparo. Lo
   siguiente que señalan los datos: el pool de actores genéricos (pico
   91/96 antes del jefe), las balas de pistola (8/8, rechazos) y los
   fotogramas de render con descodificación síncrona (40–45 ms en 1920 y
   3030).
5. **Siguientes fallos de la matriz por orden:** B14, B11, B12, B03, B13,
   B15.
