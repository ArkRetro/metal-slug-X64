# Plan de búsqueda y resolución de fallos (11-XI)

Motivo: la regresión de la M6 (paleta 125 sin dibujar, etapa 3 parada en la
cámara 320) entró entre la ROM 47 y la 50 sin que nadie la viera, porque las
ROM 48 y 49 pasaban del límite de log de ares y se probaron sin la M6, y cada
agente solo corría las pruebas de su misión.

## 1. Detectar: matriz de regresión en cada subida

- `tools/regression_ares.py` corre en un solo cartucho normal todas las
  pruebas de ares del cartucho de juego (logo, pausa, personajes, M4, M5 ×3,
  M6 ×3) y M1→M2 sobre un `m1end`, y cuenta en cada log los dibujos ausentes,
  rutinas sin portar, piezas que faltan y programas parados.
- Compara con `data/regression_baseline.json`: una prueba que pasaba y ya no,
  o un recuento que sube, es regresión. Las bajadas se aceptan y se fijan con
  `--write-baseline` tras una pasada revisada.
- **Regla**: nadie sube a `ccr` sin la matriz en verde sobre el cartucho
  completo (todas las misiones dentro). El script rechaza un cartucho de más
  de 67 043 328 bytes; si una misión lo hace crecer, primero se busca el
  duplicado (`docs/COMPRESION.md`), nunca se prueba quitando misiones.
- `make check` sigue siendo la puerta bit a bit; la matriz es la puerta de
  conjunto.

## 2. Aislar

1. Reproducir con la prueba más corta que falle (`--only`).
2. Bisecar `ccr` entre la última ROM verde y la roja (`git bisect run` con la
   prueba concreta; reconstruir los paquetes en cada paso con `build_packs.py`).
3. Leer la rutina 68K y el código nativo implicados; una hipótesis cada vez,
   anotada en el documento de la misión antes de cambiar código.

## 3. Resolver

- Arreglo en la causa, nunca en el síntoma ni rebajando la prueba.
- Cada fallo corregido deja una prueba que lo habría detectado: familia bit a
  bit, prueba de host en `make check` (p. ej. integridad: todo programa,
  paleta y bloque que una escena referencia existe en el paquete) o una
  comprobación nueva en la matriz.
- Registro en `docs/BUG_MATRIX.md` (fila B nn: síntoma, cómo se reproduce,
  esperado, observado, causa, commit, prueba que lo cubre).

## 4. Fallos que trae el usuario (MiSTer)

Cada informe se pasa a fila de `BUG_MATRIX.md`, se reproduce en ares con el
estado o la etapa del menú de pruebas más cercanos, y entra por los pasos 2–3.
Lo que no se pueda reproducir en ares se marca «solo hardware» y se pide al
usuario una prueba concreta.

Línea base (`data/regression_baseline.json`, 11-XI): cartucho de la ROM 50 (`aeef1df` + `ebc1003`, mismos bytes) y
un `m1end` recién hecho; todo PASS salvo `m6s1` (FAIL conocido: el agua; B68). Los recuentos de dibujos ausentes de la M6
dependen del tiempo que la prueba pasa con esos actores en pantalla (90 s), no solo del cartucho.

Línea base nueva (B70, `work-chars` sobre ccr `401875e`, cartucho de 62 111 744 B con los paquetes rehechos): todo PASS, también `m6s1` (balsas; corre con `--jump 1.0 --continues`) y `m6s2` (`--fire --jump 2.5 --continues`: sus soldados ya andan con los grupos m3/m4/m5 y matan a un jugador que ni dispara ni salta); 0 dibujos ausentes en las tres etapas de la M6.

Pantalla de opciones (`work-chars`, 11-XI): prueba `options` añadida a la matriz y a la línea base; el resto de la matriz sin cambios con BLOOD OFF por defecto (tabla EU).

## 5. Abierto ahora

| Fallo | Estado |
| --- | --- |
| M6 etapa 3: paleta 125 sin dibujar, cámara parada en 320 | no era regresión (B68): 320 es el final de la arena en MAME; la paleta 125, corregida (B70) |
| M6 etapa 1: dibujos ausentes con la paleta 125 (además de la caída al agua, conocida) | corregido (B70): las balsas `0BD71E` portadas y la M6 con su `rest-native.json` y los grupos m3, m4, m5; queda el puente colgante `10D7B6` (MISION6.md «Fase 2») |
| Prueba de personajes que supera 600 s | tiempo de la prueba ampliado a 1800 s en la matriz |
| Voz de muerte de Tarma `1087` | pendiente de captura con audio |
| `veh` (B69) tras fusionar ccr `91d8ef3` en `work-chars` (menú de opciones + M6) | no era de la rama (13-XI): con la misma prueba fallaron igual (cúpulas, cámara 1920, ~90 muertes en 3049 s) `work-m4` y `work-m5`, sin el menú; el cartucho de ccr `91d8ef3` compilado aparte pasó una vez (1598 s, 43 muertes en las cúpulas). Causa hallada en `work-m4` (`326c1e1`/`6173047`): con la Z del N64 asignada, la prueba activaba la barra de vida y el mando ciego se queda en las torres de 1920; la matriz corre ahora `veh` con `--no-lifebar` (como la base). Diferencia real del menú anotada: BLOOD sale OFF (valor por defecto de la tabla EU de la ROM) donde el cartucho sin menú usaba `MSX_BLOOD` = 1 |
| `m4boss` en `work-chars` (menú de opciones, BLOOD OFF por defecto) | abierto (13-XI): con ccr `4de14ab` fusionado, `make check` y el resto de la matriz PASS, pero `m4boss` da missing_draw 3976 y 4043 (base 2061; mismo origen: descriptor 0 tras `16929E`, paletas 10/20/30/40); con la EEPROM puesta a BLOOD ON (`10FD8F` = 1, el valor de las capturas y de `MSX_BLOOD`) baja a 1694. Causa: la M4 con BLOOD OFF dibuja muertes sin exportar (las exportaciones y capturas de la M4 son con sangre). Pendiente de decidir: exportar la M4 con una captura BLOOD OFF (agente A) o correr la matriz con la EEPROM de las capturas. Sin subir a ccr hasta entonces |
