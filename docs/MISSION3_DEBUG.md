# Misión 3: depuración

Punto de continuidad de la Misión 3. Se actualiza al final de cada sesión.
Referencias: `MISION3.md`, `MISION3_ACTORES.md`, `JEFE_MISION3.md`,
`PLAN_MISION3.md`, `BUG_MATRIX.md` (B18, B56–B58, B59).

## Estado (ROM 41, `ccr-09eb2f6b-8rwl8l`; lista cerrada de `work-m3gaps` abajo)

- **Carga:** tras la M2, o desde el menú de pruebas (L+R o Z+R y A en
  SOLDIER SELECT). Pack v4 con el bloque MSXD, 796 de 800 trozos.
- **Mapa y colisión:** 100 % frente a MAME en las 5 secciones (`test_m3_map`).
- **Actores:** todos los del recorrido de `run4` antes del jefe son nativos.
  Faltan las ramas al azar no vistas y `03C306` (granada, sin traza).
- **Jefe intermedio y final:** nativos (familias m3mid1..4, m3boss0..5).
  El fin de misión cae en la parada explícita "falta mission4.pack", que
  reinicia la M1.
- **Vehículos:** `093C9A` y `0AC866`. Arma E. Marco gordo y sus armas, salvo
  la bomba, el molotov y el polvo.
- **Fondo:** columnas pre-renderizadas en la capa 1 (`build/bgp/m3s1.bgp`).
  El resto va por la caché de piezas.

## Verificado

- PC: `make check` con todas las familias M3 bit-exact.
- ares: recorridos con la ROM de referencia (piloto, invulnerable) hasta el
  jefe.
- Ojo: la ROM de referencia **escondió** fallos de la ROM normal. Hay que
  probar siempre con `mslugx-explorer.z64` o con `mslugx-m3-release.z64`.

## Fallos abiertos (MiSTer, ROM 41, cartucho normal)

1. ~~**Fondo de la M3 ausente**~~ (B60, corregido en `work-m3doors`). La foto
   (`scratchpad/rom41-photos/m3-black-bg.jpg`) es el **inicio** de la M3
   (cajas, cerdos, fogata con su humo). Experimento: `mslugx-m3-release.z64`
   (juego normal desde el registro de la M3) en ares, cámara 0: mismo negro
   (`MSX prerender: layers 2, ring 94 KB`); sin `m3s1.bgp` (`missing: tile
   path`) también negro; píxeles del fondo `(8,0,0)`. MAME (vídeo 8:04,
   `run4` cámara 0): fondo con cielo, árbol, locomotora. Causa: `map.json`
   toma la primera observación de cada celda y en x 0..304 era un fotograma
   del fundido de entrada (`run4` 117990/118020): 583 celdas con paletas
   negras (`0001`). Arreglo: `extract_mission_map.py --skip-fade` y
   reempaquetado (796 trozos). Ya se veía así en ares el 4-XI (`cap-m3s1/ares7`).
2. ~~**Puertas de los vagones con palanca**~~ (B57, corregido en
   `work-m3doors`): la puerta central es `0391EA`, que solo cae con un golpe
   de tipo 13; se lo da el empujón `0AF2BA` (fila `0AE9B0`, clase 1) de los
   soldados `+76` = 7 de dentro. El port lo comprobaba contra el jugador.
   MAME `run4` 121357 (`104BC4` → `101E64`): `test_neo_m3push.c` igual. En
   ares (referencia, piloto) las dos puertas de los vagones 7-24 se abren y
   salen los soldados.
3. **Tren en el minuto 8:30 del vídeo:** en ares arranca también con el
   cartucho normal sin piloto (ver «Lista cerrada»). Lo que sí faltaba en
   esa zona era la cámara baja del puente (G1, corregido) y el cielo (G2).

## Lista cerrada de diferencias con MAME (sesión `work-m3gaps`, 7-XI)

Fuentes: MAME `run4` desde `states4/sec04_117966` con
`tools/capture_m3_actors.sh` (233 constructores, 10 871 creaciones) y
`tools/compare_m3_events.py --list/--port`; ares con
`mslugx-m3-release.z64` (juego normal, mando `--hop`, 200 s: muere en la
espera de 1520) y con `mslugx-m3-reference.z64` (piloto, 330–480 s, hasta
5980); fotos de ares por cámara contra `tools/render_m3_sections.py`
(MAME | ROM | SCB de fondo, 4 por sección); capturas MAME cortas para las
paletas (`at_119570` con el plan `to:6c0`, `at_124100`). Orden: visibilidad.

| # | Diferencia | Evidencia | Estado |
| --- | --- | --- | --- |
| G1 | **Altura de la cámara**: `0B8EBC(n)` de la lista de cámara (puente 122145, hueco 123924..126262, final) no se ejecutaba; la cámara seguía a nivel y no se veían el puente, los caballetes, el río ni las balsas | MAME `trace.csv` `fore_y`: 320 en 2888..3103, 224 en 3296..3463, 256 en 3716; ares antes: 48→39 | **corregido**: `neo_camera.c` (0C8764: A0 = orden + 8; `MOVE.L (A0),6960`); `test_m3_camera_bridge` (−64 / +32 / 0 como MAME); ares: −64 en 2833..3136, +32 en 3171..3585, 0 en 3712 |
| G2 | **Cielo desde el progreso 0x140** (secciones 2–5): MAME cambia el cuadro del cielo (nubes y colinas, cañón, pueblo...) y el port pinta el color de fondo o restos del bosque | `0B8EDC` (creado por `0B8D34`, falta): con la columna del tren `10D7B6` pasando 0x50/0xB0/0x110/0x160 (tabla `0B8EC2`) carga paletas (`009D8C` `0B8948..0B8960`: recursos 0x480/0x484/0x48A/0x48E) y copia un mapa de 32×20 (`0C714E`) a la capa del cielo `10E83E`: `0B8592` (`31F2A0`), `0B85A8` (`31FCA0`), `0B85BE` (`3206A0`), `0B85D4` (`331A68`). Fotos: ares 387/1520/2888/3455 contra MAME 336/1520/2888/3463 | **en parte**: el disparo de `0B8EDC` es nativo y bit-exact (`neo_train_sky`, `test_neo_train`: los 4 cuadros con los `+32/+36` de MAME en 118892/121768/124930/144579) y los 4 mapas están en `layers_rom.json` (`export_m3_layers.py`, colores fijos de la RAM de paletas, slots 13..24). Falta dibujarlos: empaquetados como plantillas de la capa 0 suben las tablas 156 KB (835 KB) y los trozos a 820 (> 800, `SCENE_MISSION_CHUNKS_MAX`): hay que ir por columnas pre-renderizadas (`prerender_stage.py`) o ampliar el límite y medir RAM. x del cielo en `run4`: x del tren / 4 − 240, repetido cada 512 px |
| G3 | Paletas `3CF` (bomba `0286F8` del coche `027F54`) y `348` (pieza `028C76`) | ares release: 959 dibujos con la paleta de reserva en la espera de 1520; en `run4` el coche muere antes de tirar | **corregido** (export): captura `at_119570` + `to:6c0` (el jugador no dispara), paletas en `filesystem/mission3/rest-native.json`, paquete rehecho |
| G4 | Paletas `898/899` (bombarderos `0DC97E`), `87B` (torreta `03B4E0` del tanque), `4B4` (`0B91FA`) | ares piloto: 466+466 y 128 dibujos sin paleta | **corregido** (export, captura `at_124100`; las 6 paletas en `mission3.pack`); `4B4` sin uso hasta G9 |
| G5 | Soldados del campamento (`01F386/01F2D6` → `0B05FE`) desaparecen al explotar el fogón | ares (los dos cartuchos, f≈223): `world unsupported routine=b05fe`; MAME `m3camp` 118741: `+38 = 0010D7B6` (la capa del tren); el módulo de soldados no tiene padre tren | **corregido**: `neo_world.c` `convert_soldier` lo deja sin padre en su posición proyectada (como lo que aterriza en el tren); ares: sin `b05fe` ni aserción |
| G6 | Salpicadura `03C962` (balas, granadas y armas en el agua) | ahora que se ve el río: 21 paradas `3c962` en ares (granadas de `08A732`); `run4` no tiene ninguna (sin traza) | abierto: parada explícita; hace falta una captura con granadas al agua |
| G7 | Marco gordo: bomba `01C362`→`01C654` (36/71), polvo de aterrizaje `0C4E86` (88), `0C5064` (al dejar la forma, 1); molotov gordo `01E680/01E69C` (no sale en `run4`) | `compare_m3_events.py --list`: `port=missing`; `gameplay.c` parada | abierto: falta la familia (captura 124100..124700 con `ROOTS=1c362,1c654,c4e86`) |
| G8 | Sonidos sin efecto: 31 palabras | SND de la captura contra `msx_sfx_policy.h`: `1020` ×13 y `102D`/`109E` (aeronave/jeep), `10EB` ×33, `10EC` ×32, `10ED` (jefe intermedio), `1169` ×40 (arma E), `1062/1065/1078/109A/109F/10A1/10EE/10EF/1026` (jefe), `1079/107A/107B` (`0186A0`, órdenes de cámara 0B8D10..28), `1029` (puertas), `105E` (`04E990`), `108B` (`0AC866`), `109D` (`027F54`), `10BA` (`097FCC`), `10D5` (flecha), `10FC`, `1133` (voz MISSION 3 START), `1139`, `1153`, `1155`, `116D` | abierto (render aislado `render_sound_commands.py` + medida de pares) |
| G9 | `0B91FA` (adorno en (0xE00, 0x40) de la raíz, programa `2A8E40`, paleta 4B4, hasta el progreso 0xF80) | `0B91D0` de la lista (124034) | abierto: falta el programa en `soldier_programs.json` |
| G10 | Rutinas de soldado `0AF7B0` y `0AF99A` (acciones de `0AF1A8`, externo) | ares piloto: 2 + 1 paradas | abierto |
| G11 | `routine=800040` (nodo con rutina basura) en 3296 (jeep `0A2F2E` + `08AFB6`), con 16 externos `02A448` | ares piloto f=6643 | abierto (sin diagnosticar) |
| G12 | Paleta `0A7` (piloto `04EF88` de `04E990` con sangre) | ares: ~2400 dibujos; `run4` va sin sangre | abierto: captura con sangre (`capture_blood.lua`) |
| G13 | Etiquetas: letras `08C954` del rótulo de bombas (`08C752`, externo, 5), puntos `08D0D2` de los cañones del jefe intermedio (2) | `--list` | abierto |
| G14 | Fuego `01C950` del molotov `056B00` de los lanzadores (3) | `--list` | abierto |
| G15 | Jefe: orden 6 de la lanzadera (`04BA94`, misil `04C220`), bola recta `04C952` | no salen en `run4` (ramas al azar) | abierto: paradas explícitas |
| G16 | Granada `03C306` de la muerte de `04115A` | sin traza en `run4` | abierto: parada explícita |
| G17 | Superposición `10E8C6` (`0A6C8C`, 34 bloques de submapa en 3944) y 41 bloques del tren con otra paleta | `render_m3_sections.py`: 98,84 % / 99,49 % de píxeles en las secciones 4 / 3 | abierto (poco visible) |
| G18 | HUD: `continue item 019798` (parada al acabar las vidas) | ares release | abierto (común a todas las misiones) |

Registros del guion por sección (`--port`): release (hasta 1520) sección 1
28/28 en el mismo orden; piloto: 1 28/28, 2 106/112 (difieren solo ramas
`si`/azar), 3 1/1, 4 48/55 (no llegó al jefe en 330 s). Sin registros de
constructores sin portar saltados; `camera events unsupported` 0.

**Tren del minuto 8:30**: en el cartucho normal (`--hop`, sin piloto ni
invulnerable) arranca: el grupo 0..7 y el soldado del escudo `09AE2C`
mueren, la cámara se suelta en 312 y el tren pasa a modo 1 en 320 (`train
on=1 mode=1 own=1`). No hay diferencia en ares: falta verlo en MiSTer.

## Hipótesis

| Hipótesis | Evidencia | Estado |
| --- | --- | --- |
| H1: el anillo pre-renderizado deja columnas sin dibujar en el hardware | Sin `m3s1.bgp` el camino de piezas da el mismo negro | descartada |
| H2: la sección de la foto usa capas o reglas que el pack no tiene | La foto es la cámara 0, con sus capas en el pack; solo las paletas eran negras | descartada |
| H3: falta de ranuras de caché en ese tramo | Tablas y mapa cargados (774 KB en 49 ranuras, 31 trozos); el dibujo existe, con paletas negras | descartada |
| H4: paletas de las celdas tomadas de un fotograma del fundido | 583 celdas de x 0..304 con paletas `0001`; con `--skip-fade` el fondo es el de MAME | **confirmada, corregida (B60)** |

## Siguiente experimento (uno solo)

Cartucho normal en MiSTer (ROM 42): el puente de 2888 (la cámara baja 64 px:
caballetes, río y balsas, G1), los soldados del fogón (G5) y el tren de la
salida. Después, en el PC, G2 (cielo) por columnas pre-renderizadas.

## Archivos tocados por la M3

`src/neo_m3props.c`, `neo_m3enemy.c`, `neo_m3mid.c`, `neo_m3veh.c`,
`neo_m3boss.c`, `neo_train.c`, `scene_prerender.c`, `scene_assets.h`,
`tools/pack_mission.py`, `tools/prerender_stage.py`; B60: `filesystem/mission3/s1/map.json` y
`romfs/mission3.pack` (privados); B57: `src/neo_soldier_ai.c/.h`, `src/neo_world.c/.h`,
`tools/test_neo_m3push.c`, `Makefile`; `work-m3gaps`: `src/neo_camera.c` (G1),
`src/neo_world.c` (G5), `src/neo_train.c/.h` (G2, disparo),
`tools/test_m3_camera_bridge.c`, `tools/test_neo_train.c`,
`tools/export_rest_sprites.py` (paletas), `tools/export_m3_layers.py` y
`tools/render_m3_sections.py` (cuadros del cielo); privados:
`filesystem/mission3/rest-native.json`, `filesystem/mission3/s1/layers_rom.json`,
`romfs/mission3.pack`.
