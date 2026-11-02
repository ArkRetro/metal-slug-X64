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
5. **B11 y B12 corregidos en el PC (familias y soldados contra MAME)** (rama `work-laser`, ARMAS_VEHICULOS.md
   Paso 16): rayo `06/0B` nativo y muertes de soldado por llama y estallido.
   Falta: probar el láser en la M2 en MiSTer (la L de la sección 1), copiar
   a `/home/user/ws-logo` las entradas privadas regeneradas
   (`build/marco_campaign.h` con los papeles `W1L_*`/`B1L_*`,
   `filesystem/rest-native.json` con los descriptores del rayo; el audio
   `sfx_10f8` ya está en el directorio compartido) y fusionar en
   `ccr-09eb2f6b-8rwl8l`. Paradas que quedan de B12: el cuerpo en llamas
   `040920` (sin captura) y las entradas `040B46/040BA8/040BE6`.
6. **Siguientes fallos de la matriz por orden:** B14, B03, B13, B15, B20
   (bucles de motor 10A5/1026), pool de actores genéricos y balas.
