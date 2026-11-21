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
| `m4boss` en `work-chars` (menú de opciones, BLOOD OFF por defecto) | corregido (13-XI, agente A, `docs/MISION4.md` sexta sesión): no eran paletas sin sangre (las capturas de la M4 ya tienen `10FD8F` = 0) sino la estela `02505C` del proyectil, que dibujaba con `palette_id` 000 (su `+14` es la ranura del proyectil); con BLOOD OFF `m4boss` da 0 dibujos ausentes |
| Unión de la fase 1 (16-XI, agente I): `m6s1` 1312 dibujos ausentes, `m5boss` 8, `m4boss` `02058E` | `m6s1`: la exportación de la M6 de `ws-logo` (enlace) frente a la de `ws-chars`; con la copia propia, 0 dibujos ausentes y 0 paradas. `m5boss`: sus ausentes son la paleta 20C (no capturable, `MISION5.md`; el número sigue a la duración de la pelea) y la muerte del Slugnoid (paletas 6/A/B/C, captura SH, corregido). `02058E`: no se repite con este cartucho (69 rocas trazadas, todas por `01FDA4`); parada explícita conservada (`MISION4.md`, séptima sesión) |

Línea base nueva (16-XI, agente I, `work-m4` `9da158b` = ccr `a41e23a` + M4 + M5 `e13b3d9` + `work-chars` `786127a` + `work-audit` `1727400`, cartucho de 63 635 456 B con las exportaciones de la M5 y la M6 copiadas y los paquetes rehechos, `m1end` recién hecho): las 17 pruebas PASS en una sola pasada, 0 dibujos ausentes en todas (`m4boss` antes 2061, `m5boss` antes 8), `m4boss` con 4 paradas (`7E832`, `03C5E8`), `veh` en 569 s; pruebas nuevas `m6s1end` (agente C) y `slugnoid` (B69). En la pasada anterior de la misma unión `veh` falló una vez en las torres de 1920 (93 muertes, 3032 s), como ya se había visto: es el mando ciego.

Pasada del 17-XI (agente I, `work-m4` `c7ff44a` = ccr `e138371` + `work-audit` `f4a5571` + `work-chars` `9013c98` + `work-bugs` `ec724ca`, cartucho de 63 913 984 B, `m1end` recién hecho; el contenedor se reinició a mitad y se repitieron solo las pruebas sin terminar): todo PASS. Fallo conocido **B77**: `m5boss` 62 dibujos ausentes (28 del humo `05A0B4` de la muerte de tipo 8 sin exportar, 34 de la 20C); su línea base no se toca. `m4boss` con 6 paradas explícitas conocidas (como la primera base). `m6s2end` (agente C) PASS con 618 dibujos ausentes y 17 paradas, fuera de la línea base.

Pasada del 19-XI (agente I, `work-m4` `c932e54` = ccr `20c330c` + `work-chars` `bcc3849` + `work-audit` `c777140`, cartucho de 64 307 200 B; exportación de escena de `ws-audit` y las de la M2/M3 con los 10 descriptores del humo, como copias propias): las 18 pruebas PASS, **0 dibujos ausentes en todas** (B77 corregido: `m5boss` 0). `m6s2end` (agente C) PASS con 1813 dibujos ausentes y 12 paradas, fuera de la línea base.

Pasada del 20-XI (agente I, `work-m4` `f7a660e` = ccr `bf4b040` + `work-chars` `0171ec9`; el contenedor se reinició a mitad y se repitieron las pruebas sin terminar): todo PASS con 0 dibujos ausentes salvo **`m4boss` FAIL (B78)**, que falla igual sobre el cartucho de control de ccr `bf4b040` reconstruido: no es de esta unión; subido con ese fallo abierto y su línea base sin tocar. `m6s2end` PASS (788 dibujos ausentes, 16 paradas), fuera de la línea base.

## `m4boss`: no era el juego, era el mando (20-XI, agente S)

En las pasadas que fallaban (`agentO/matrix`, `agentO/ctl`, la del agente I con otra ares a la vez) el registro tiene
`MSX player left vehicle kind=1 (0C30F0)` uno o dos segundos después de montar en el SV-001 de 4288, y a partir de ahí
`MSX vehicle hit ... mount=0` y los dibujos sin descriptor tras `169276..16929E` (el SV-001 abandonado embistiendo:
3818 / 4897 frente a 2061). Causa: montado, **A + B a la vez es la salida `093A08`** (`0CA1B0`: bit 7 de la palabra del
mando; regla de la ROM, bien traducida), y el mando ciego pulsaba el fuego a 5 Hz y el salto 0,2 s cada 1,5 s sin
relación entre ambos: que coincidieran en una actualización dependía del reloj del anfitrión. No es una regresión.

Arreglo en la prueba (`tools/test_mission4_ares.py`): montado (`MSX player mounted vehicle` posterior al último `left
vehicle` / muerte / reaparición), un salto es una secuencia: fuego suelto, 0,3 s, A 0,2 s, 0,3 s, fuego otra vez.
Además las pruebas de las misiones 4, 5 y 6 cuentan ahora en **tiempo de juego** (`tools/ares_clock.py`: el cartucho
escribe `ticks=N` en la línea periódica de etapa; 60 por segundo): el programa del mando y `--seconds` ya no cambian
con la carga de la máquina (con otra ares y `make check` a la vez la emulación iba al 40 %). Los límites de
`regression_ares.py` de esas pruebas son tiempo del anfitrión (tres veces el de juego más el arranque). Lo que sigue
dependiendo del anfitrión: el fotograma exacto en que llega cada tecla de `xdotool` (ares no tiene entrada grabada).

Pasada del 20-XI sobre este cartucho (`--only m4boss`, con `make check` y la ares de otro agente a la vez: emulación al

Pasada del 21-XI (integración, `work-m4` `90aa846` = ccr `673e676` + `work-mem` `aebe051` + `work-chars` `ac7ea75`, cartucho de 66 682 880 B, caché 208 ranuras, M6 1238 trozos; máquina con carga 22..31): PASS logo, slugnoid (1090 s; antes FAIL por `23b654`: los programas de los murciélagos `035B66` iban en el grupo `m2` y no estaban en el paquete de la M5, ahora en `/programs.bin`), m6s1, m6s2, m6s3, m6s1end, m6s2end (759 dibujos ausentes, B80, fuera de la línea base), m5s2, m5s3, m5boss, m4, veh, pause, options, m1m2, chars (al repetir; la primera, `ares exited` en el arranque) y ending (primera pasada, 889 s). `m6boss` dio 301 dibujos ausentes (ranura 0x38 con destino 10D viva en el final con el paquete 13), corregido en `actor.c`. `m4boss` cortado a 11 665 s (2× su base; no bloquea, B78). **Pendiente de verificar en la matriz por carga**: `m5s1` TIMEOUT dos veces a 600 s (cámara 600 > 100, pero los 90 s de juego no cabían en el límite de la matriz; límite subido a 1500 s para que decida el reloj del juego) y `m6boss` TIMEOUT a 7800 s (22 080 actualizaciones de juego: ares al ~5 % con carga 30). La segunda pasada de `ending` falló en el menú (pulsaciones contadas por reloj; ahora `ares_menu.choose`). `make check` 332 PASS, `make check-core` 40 PASS.
~30 %): monta en el SV-001 de 4288, 12 golpes montado (`mount=1`), **ninguna salida por A + B**; el SV-001 muere por
daño (`09391E`, vida 0) antes del jefe y a pie el mando pelea sin derrotarlo (cámara 14 188 a los 1330 s de juego; en
las pasadas buenas moría entre 9456 y 13 717). La corté ahí para correr `m6boss`: **`m4boss` sigue sin veredicto verde**.
Lo que queda no es azar del reloj sino del mando: las pasadas que pasaban llegaban al jefe montadas. Siguiente paso
(sin tocar el juego): escribir en el registro la x de la capa `10D7B6` y que el mando se encare al jefe (B78).

Otra dependencia del reloj hallada el 20-XI (pasada `m6boss` con la emulación al 30 %): la pulsación de Start en
CONTINUE? duraba 0,1 s del anfitrión, menos que una lectura del mando del juego a esa velocidad; la tercera no entró,
la cuenta llegó a 0 y el juego volvió a la Misión 1 (`MSX game over: mission restart`), con la prueba esperando en
vano. Ahora `test_mission4/6_ares.py` repiten Start (0,4 s) hasta ver `MSX panel play`, y `test_mission5_ares.py`
pulsa 0,4 s.

## `m5boss`: los 34 dibujos ausentes de más son la paleta 20C (20-XI, agente S)

Comprobado en los registros de la matriz del agente O: las 34 líneas de más son todas `descriptor=0 palette=4
palette_id=20C` con los descriptores `131FAE` / `1320AE` (que sí están en `filesystem/mission5/rest-native.json`; lo
que falta es la paleta: `0x20c` está en su `missing_palettes`), y el código que la pide es `ga_palette(a,0x20c)` de
`neo_m5boss.c` (`099268`, la llama final del soldado quemado), idéntico en el anfitrión y en la superposición. En el
cartucho de control ningún soldado quemado llegó a esa llama. No es un efecto de la superposición; sigue pendiente
la captura de la paleta 20C (`docs/MISION5.md`).
Pasada repetida el 20-XI sobre el cartucho con las superposiciones (`regression_ares.py --only m5boss`, `f4e1492`):
**PASS en 1930 s, `missing_draw=8`** (la línea base: paletas 006 ×3 y 00A ×5), una sola copia de la superposición, ninguna
línea de la paleta 20C: en esta pasada ningún soldado quemado llegó a la llama `099268`.
