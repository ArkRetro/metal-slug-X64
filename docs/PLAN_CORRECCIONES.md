# Plan de corrección y optimización (26-XI)

Base: ccr `4c0a51b` (ROM 52, jugable de principio a fin). Reglas:
`CLAUDE.md` y `docs/AGENTES.md`. Tres agentes como máximo, cada uno con su
worktree, su rama y su pantalla Xvfb. El coordinador audita cada entrega,
une en ccr y monta las ROM.

## Flujo de trabajo de un fallo

Cada fallo pasa por los mismos pasos, sin saltarse ninguno:

1. **Fila en `BUG_MATRIX.md`**: síntoma, cómo reproducirlo, qué hace el
   arcade y qué hace el port.
2. **Reproducción en ares** con el cartucho de pruebas (mando por guion,
   `regression_ares.py --testpad`, o el menú de pruebas): el fallo tiene que
   verse en el registro o en una captura de pantalla.
3. **Captura de MAME 0.264** de la misma situación, por ventanas pequeñas.
4. **Causa raíz**: la rutina de la ROM que produce el comportamiento y el
   punto exacto en que el port se aparta. Sin causa no hay arreglo.
5. **Arreglo** traducido de la ROM, nunca un parche que tape el síntoma.
6. **Familia bit a bit** en `make check` que falle antes del arreglo y pase
   después.
7. **Prueba de ares** del caso, añadida a la matriz si es estable.
8. Commit y push a la rama del carril; la fila pasa a «corregido (PC)» y
   queda pendiente de confirmar en el MiSTer del usuario.

## Flujo de trabajo de optimización

Cada cambio de tamaño o de velocidad:

1. **Medida antes**: `.text`/`.data`/`.bss` por módulo (`size`, mapa del
   enlazador), caché de escena en los dos arranques (Start desde el primer
   fotograma y normal), peor fotograma y `late`/`long` de la matriz,
   tamaño del cartucho.
2. **Cambio sin pérdida**: el resultado del juego no cambia en nada.
3. **Prueba de igualdad**: `make check` entero y la matriz de ares con 0
   diferencias frente a la línea base; las familias bit a bit no se tocan.
4. **Medida después** y tabla en `RENDIMIENTO.md`.
5. No vale un cambio que gane memoria perdiendo velocidad por encima de los
   umbrales fijados (peor fotograma, vblanks perdidos), ni al revés.

## Carriles

| Carril | Árbol, rama y Xvfb | Trabajo |
| --- | --- | --- |
| Fallos de personaje y vehículo | ws-m4, `work-fix`, :71 | B82: al montar la Camel Slug con Eri, Tarma o Fio siempre sale Marco. |
| Fallos de animación | ws-chars, `work-anim`, :94 | B83: la animación de correr disparando se invierte a veces. Después, la cola de fallos abiertos. |
| Compresión y optimización | ws-m5, `work-opt`, :95 | Bajar el programa principal y recuperar margen de la caché de escena (hoy 208 ranuras, el suelo); objetivo ≥ 216 ranuras y sitio para el zoom del final. |

### Objetivos del carril de optimización, por orden

1. Inventario: tamaño de cada módulo, qué código es de una sola misión y qué
   código muerto queda (rutas `MSX_*` apagadas, herramientas de depuración
   enlazadas en la ROM de usuario).
2. Código de una sola misión a superposiciones por misión
   (`src/msx_overlay.c`), como ya se hace con los jefes.
3. Opciones de compilación por módulo (`-Os` en código frío, `-O2` en el
   bucle de dibujo y el mundo), medido en ares.
4. Datos de solo lectura que se copian a RAM sin necesidad: leerlos de la
   ROM del cartucho o comprimirlos.
5. Liberar los búferes de arranque que no se reutilizan.
6. El peor fotograma: perfil por ventanas (`MSX PERF`), los tres puntos más
   caros, y cada mejora medida.

## Cola para cuando un carril quede libre

1. Zoom del final (`wip2-work-chars`): llevarlo a la superposición
   `neo_ending` cuando haya sitio; medir los vblanks.
2. B78: comprimir el guion `data/m4boss_pad.txt` para que el cartucho de
   pruebas quepa; dos reproducciones idénticas.
3. Volumen `VOLUME_PERCENT=80` sin fuente: medirlo contra MAME.
4. B80: paletas 214, 860 y 862 de la M6, con captura.
5. Demostración `0167A0` tras el título.

## ROM 53
Se monta cuando B82 y B83 estén corregidos, la matriz en verde y la caché
con margen. Montaje: como la ROM 52 (en el árbol de integración, con los
datos privados copiados, nunca enlazados).
