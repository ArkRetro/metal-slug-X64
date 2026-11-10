# Misión 4: fase 1 (captura, mapa, colisión, cámara, paquete, inventario)

Rama `work-m4` (desde `ccr-09eb2f6b-8rwl8l`), árbol `/home/user/ws-m4`. Mismo
método que la [Misión 3](MISION3.md) con sus lecciones
([MISSION3_DEBUG.md](MISSION3_DEBUG.md)): nada inventado, nada ajustado; lo
que no está portado se para y se cuenta. ROM, capturas y derivados (mapa,
rejilla, paquete) siguen fuera de git: privados en
`/home/user/ws-logo/filesystem/mission4/` y `romfs/mission4.pack`.
Inventario de actores: [MISION4_ACTORES.md](MISION4_ACTORES.md).

## Flujo

`run4` (`data/mission_flow_fixture.json`): la Misión 4 es **un solo registro
de etapa**, `017462` (misión 3, etapa 5 de 42F6), seguido del fin de misión
`017488`. Descriptor `016E1E`: lista de cámara `0E741A`, jugadores `2D5F5E`,
guion `0x814` (sección `2DB696..2DCB3A`), música `25`, etapa `5`. En MAME:
carga en 149829..149837, bucle `0186A0` en 149885, orden del jefe `0BB028` en
157582 (progreso 0x15B0 = 5552), fin de misión `018870` en 246905.

`run4` **ya contiene la Misión 4 entera** (el piloto tarda 89 000 fotogramas
en el jefe): no hizo falta alargar la partida. `01891E` ya elegía la M4 tras
la M3 y `actor.c` carga `romfs/mission<N>.pack` por número: con el paquete, la
parada «falta mission4.pack» desaparece sola. Menú de pruebas: `MISSION 4`
(`main.c`).

## Capturas (MAME 0.264, sin escribir en la RAM)

Todas desde `states4/sec05_149829` (copiado a `/home/user/captures-m4/states`),
réplica fotograma a fotograma de `run4` (0 diferencias de jugador y cámara en
149830..160500; también al cargar un estado intermedio con un Lua nuevo:
154187..155900, 0 diferencias).

| Captura | Guion | Contenido | Queda |
| --- | --- | --- | --- |
| estados | `MSX_FULL_SAVE_AT` | `at_152088` (subida, 0x570), `at_153827` (mapa 2, 0xAD0), `at_154187` (cielo 2, 0xB80), `at_155850` (SV-001 `092A32`, nace en 155879), `at_156647` (0x10C0), `at_157560` (22 fotogramas antes del jefe), `at_158600`, `at_160000` (jefe) | `/home/user/captures-m4/states/mslugx/` |
| actores | `tools/capture_m4_actors.sh` | 0040BC/004126, guion (op 7, esperas), programas, sonidos (0120F8), premios, puntero de la lista de cámara, fin; 149829..160500 | `captures-m4/actors/debug.log` (2 MB) → `data/mission4_actors.json` |
| mapa | `tools/capture_m4_map.sh` | cada celda que lee `0C79FE` del anillo `10C72E +88`, parches `0C70FC`, sellos `0C88B0`; 149829..158600 | `captures-m4/map/cells.txt`, `debug.log` → `data/m4_map_fixture.json.gz` |
| rejilla | `MSX_FULL_GRID_ALL=1` | todas las escrituras en el anillo con su PC (solo `0C7038` borrado, `0C734E..0C735A` copia de mapa/parche y `0C8982` sello) | `captures-m4/grid/events.csv` |
| cámara | `MSX_FULL_CAMERA=1` (nuevo en `capture_fullgame.lua`) | por actualización de cámara: entradas de `018D96/01903A`, bloque `10E94E..10E9DB` y los objetos de capa `+0..+2B`; 149884..160498 | `captures-m4/camera/camera.csv` → `data/m4_camera_fixture.json.gz` |
| VRAM, paletas, RAM, capas, LSPC, latch de sonido | `run4` (cada 30 fotogramas; `layers.csv` cada fotograma) | 149829..246905 | `/home/user/captures-fullgame/run4` |

Las trazas en bruto (bancos, `actorstate`, `trace`) se borraron tras convertir.

## Lista de cámara `0E741A` (170 órdenes, `decode_map_commands.py`)

| Progreso | Órdenes |
| --- | --- |
| 0 (cabeza) | gráficos `0E6D50`, base (0, 0), paletas `0E71C6`, autoanimación 8; mapas: `10C72E` ← `0E6D7C` (columna 0, fila alta 0xF, tabla `0E71BC`), `10D7B6` ← `0E6D92` (0, 0xF, tabla `0E71B2`), `10E83E` ← `0E6DBE` (0, 0xF) con `0C73CE` (0, 0, 0x20, 0x10); factores `10C72E` 0x100/0x100, `10D7B6` 0x100/0x100, `10E83E` 0x40/0; límites 0; punto 0x80/0x1C0; modo 0; llamada `0BA942` |
| 0x110..0x540 | llamadas `0BA964`, `0BA96A`, `0BA970`, `0BA976`, `0BA97C`, `0BA85A`, `0BA982`, `0BA912` |
| 0x570 | modo 7 (la subida) |
| 0x5B0 | op 4: jugador en el rectángulo (0x670, 0, 0x80, 0x80) de `10C72E` **y en el suelo** |
| 0x5D0..0x870 | punto 0x20/0x1C0, `0BA88A`, y `0BA8BA` con índice 0..0x15 cada 0x20 (el derrumbe: 22 parches `0C70FC`) |
| 0x9C0 | `0BA82A`, punto 0x80/0x1A0 |
| 0xAD0 | `10C72E` ← `0E6DA8` (columna 0xC8, fila alta 7, sin tabla), recarga |
| 0xB80 | `10E83E` a la raíz (op 0x44), ← `0E6DD4` (0xC2, 0xA), factores 0x80/0x80 |
| 0xBC0..0x1080 | llamadas `0BA988..0BA9FA` (20), modo 6 en 0xBE0 |
| 0x1080 | `10E83E` a la raíz, ← `0E6DEA` (0, −9) con `0C73CE` (0, 0, 0x20, 0x10), factores 0x80/0 |
| 0x10C0..0x1400 | modo 0, punto 0x80/0x200, llamadas `0BAA00..0BAA1E` |
| 0x15B0 | límites (0, 0, 0x100, 0), **`0BB028` (jefe)**, punto 0x20/0x200 |
| 0x7FF0 | límites 0; retención |

Mapas (descriptor: teselas, colisión, columnas × filas de bloques de 16 px):
`0E6D7C` (`335D48`, `444A82`, 200 × 28), `0E6D92` (las mismas teselas `335D48`,
sin colisión, 200 × 28), `0E6DA8` (`33BCC8`, `44A202`, 144 × 36), `0E6DBE`
(32 × 16), `0E6DD4` (60 × 36), `0E6DEA` (32 × 16). Submapas de los parches:
`0E6E84..0E719C` (42 parches `0C70FC` en el recorrido, 812 celdas de colisión
cambiadas) y el propio `0E6DA8` (6 copias de sus columnas 112.. a las columnas
344.. del anillo, hechas por actores desde 157264: **el suelo que se repite en
el jefe no es el anillo sin recargar, son parches**).

**Tabla de columnas (`+3E` del objeto de capa, último largo de la op 0x18)**:
pares (primera columna de bloques, palabra) hasta `FFFF`; el byte alto es la
primera fila de bloques dibujada y el bajo el número de filas. `0E71BC`
(`10C72E`): (0 → 0x0020), (0x68 → 0x1508); `0E71B2` (`10D7B6`): (0 → 0x0000),
(0x68 → 0x0015). Es decir: hasta la columna 0x68 todo el mapa lo dibuja
`10C72E`; desde ahí `10D7B6` (ranura 0x20, detrás de los sprites) dibuja las
21 filas de arriba y `10C72E` (ranura 0xA0, delante) las 8 de abajo. Evidencia:
los 1210 bloques que MAME mostró en `10D7B6` y los 1722 de `10C72E` están
todos dentro de las filas de su tabla (`export_stage_rom.py` se para si no).
La rutina que lee `+3E` no está desensamblada todavía (pendiente 4).

Orden de dibujo (`run4` RAM, constante en toda la misión): ventana A `10B954`
= 0xC0, B `10B95C` = 0x40, `10E83E` 0, `10D7B6` 0x20, `10C72E` 0xA0: el orden
por defecto (fila añadida a `draw_windows`).

## Mapa y colisión desde la ROM (`tools/export_stage_rom.py`)

Nada sale de un fotograma: los mapas son los que carga la lista de cámara (op
0x18) y se leen enteros de la ROM. La captura solo sirve para **comprobar la
colocación** y para **los colores** (número de paleta → 16 colores).

```sh
sh tools/build_mission4.sh          # observado (una vez), ROM, paquete
```

- **Rejilla** (`collision.json`): unión de los mapas de `10C72E` en filas del
  núcleo (`0C72EC`: 4 bytes por bloque, de arriba abajo; la fila alta del mapa
  es la fila 2·T + 1): 688 × 88, **origen de filas −56** (el mapa `0E6DA8`
  baja hasta y = −448: la cámara desciende 440 px). El runtime ya no exige
  origen 0: `neo_collision_set_row_origin` (`grid_origin_y` del paquete).
  Contra la rejilla observada: 23 628 celdas iguales, 53 distintas (celdas de
  parches vistas ya cambiadas), 470 de eventos.
- **Bloques** (`map.json` = modelo de ROM; `map_observed.json` = lo visto):
  `x = 16·(C + i)`, `y = 240 − 16·(T + 1) + 16·j` (−8 en las capas 0/1 del
  modelo). Contra lo observado: `0E6DBE` 924/924 (476 en columnas repetidas),
  `0E6D92` 1210/1210, `0E6D7C` 1708/1722 (14 = puerta parcheada en x 1312),
  `0E6DD4` 915/915 (+70 fuera del mapa: restos del anillo), `0E6DEA`
  9867/9867 (+1518 restos bajo sus 16 filas), `0E6DA8` 20 086 iguales y 3672
  distintos (bloques parcheados por actores; el modelo de ROM lleva el estado
  **anterior** al parche).
- **Colores**: los de los bloques observados iguales, por (mapa, número de
  paleta): ninguno tiene más de un juego de colores; los 31 números que
  ningún bloque visto usa salen de la RAM de paletas de `run4`
  (`palette151500/154500/156000.bin`), comprobando antes que todos los números
  vistos coinciden con esa RAM. Las capturas con fundido no cuentan
  (`--skip-fade`, y ahora también las que caen a 1–2 fotogramas de una
  escritura del modo del LSPC: 158550).
- **Cielo**: `0E6DBE` y `0E6DEA` son cuadros de 32 columnas copiados una vez
  (`0C73CE`): se repiten cada 512 px; el paquete guarda una ventana de 864 px
  desde su base (0 y 4096) y el runtime envuelve.
- Pasado el final de `0E6DA8` (x ≥ 5504) el paquete lleva los bloques
  **observados** hasta x 6144 (los ponen los parches de los actores del
  jefe); la rejilla ahí está vacía, como en MAME antes del parche.

## Paquete `romfs/mission4.pack`

`python3 tools/build_packs.py` (receta de la M4 en `RECIPES`: `s1`,
`--rom-layers`; versión 5, trozos LZ4, [COMPRESION.md](COMPRESION.md); M2 y
M3 antes, porque los paquetes comparten trozos): 352 páginas, 44 trozos, 82
paletas, 4894 imágenes, 15 450 celdas, rejilla 688 × 88 (origen −56),
**793 664 bytes**; sin sección de actores todavía (los nativos compartidos
dibujan con `scene.pack`; los nuevos no existen). En ares: tablas 430 KB en
27 ranuras, 44 trozos, carga en 0,58 s.

**Reglas de capa de la lista de cámara** (`rom_native_rules`, sin ajuste):
`10C72E` y `10D7B6` (capa 2 del paquete: el modelo la llama 2 mientras
`10E8C6` no tiene mapa) con factores 0x100/0x100 → 1/1; `10E83E` cambia de
factores y vuelve dos veces a la raíz → regla «nunca» (visible 1..0): el
runtime la dibuja en el `+14/+18` de su objeto.

## Motor (cambios)

- `neo_train.c/.h`: **objetos de capa genéricos** `neo_layer_t` (`+0/+4`,
  factores `+26/+28`, restos `+2A/+2B`, mapa de la última op 0x18, cuadro de
  32 columnas): `neo_layers_step` (`0C81C0` → `0C6F9E` con la velocidad de la
  raíz 6970/6974, salvo la capa con su byte 6956+i ≠ 0) y `neo_layers_event`
  (op 0x14 `0C85F0` y 0x44 `0C8646` → `0C6FF2`, 0x1C `0C86C8`, 0x18 `0C8654`,
  0x48 `0C73CE`). `neo_world_camera` hace cámara → paso de capas → órdenes de
  la lista, en el orden del original.
- `neo_camera.c`: la **op 4** (`0C84BE` → `0C8524`) exige `+5F` bit 7 a 0 y
  `+15` bit 6 (en el suelo) además del rectángulo. Sin eso la lista avanzaba
  en 152244 y MAME en 152253 (al aterrizar). Afecta también a las listas de la
  M2. Cada evento lleva el desplazamiento de su orden (`at`).
- `neo_collision.c`: origen de filas de la rejilla.
- `scene_renderer.c`: `scene_render_set_layer_rise`; `actor.c`: la capa 0 con
  regla «nunca» se dibuja en `−(+14)` y `256 − (+18) + 8` del objeto `10E83E`.

## Pruebas (`make check`)

| Prueba | Resultado |
| --- | --- |
| `test_m4_map` (`data/m4_map_fixture.json.gz`, paquete privado) | 73 099 lecturas del núcleo en 149829..157581, todas explicadas por la ROM: 69 820 estáticas (**11 540 de 11 540 celdas distintas iguales** en `neo_collision_cell` sobre la rejilla del paquete), 3197 en celdas de los 42 parches `0C70FC`, 81 en sellos `0C88B0`, 1 por el anillo; registro de etapa (descriptor, lista, reglas) |
| `test_m4_camera` (`data/m4_camera_fixture.json.gz`) | **4314 de 4314 actualizaciones** de cámara iguales en lazo cerrado (149884..160498: órdenes, raíz, progreso, límites, vista) con la lista del paquete, y **15 798 estados de capa** iguales (x, y, factores, restos) con 7 órdenes de capa; 1458 estados de `10D7B6` en el jefe son de su dueño (byte 6957 = 3), no se comprueban |

## Recorrido en ares (`tools/test_mission4_ares.py`, cartucho normal)

`mslugx-explorer.z64` (nunca el de referencia), menú de pruebas → `MISSION 4`
(L+R y A en SOLDIER SELECT; `Xvfb :66` propio):

```sh
DISPLAY=:66 python3 tools/test_mission4_ares.py mslugx-explorer.z64 --data <aresdata> --out OUT --seconds 100 --period 4 --jump 2.5
```

`RESULT: PASS`: `MSX MSELECT mission=4`, carga `descriptor 016E1E 15450 cells
4894 images grid 688x88, tables 430 KB in 27 slots, map 44 chunks` en 0,58 s,
`mission 4 stage 5 start: record 017462 descriptor 016E1E camera list 0E741A
script 2DB696 (5286 bytes)`, `MISSION 4 START`, y Marco (derecha + salto,
sin disparar) recorre la etapa: cámara x 0 → 5504..5952 en 100 s con la
cámara bajando 0 → −64 → −128 → −440 px **como MAME** (`fore_y` 256 → 696);
0 caídas, 0 rescates, 0 órdenes de cámara sin soporte. Fotos por cámara
contra `run4` (misma cámara): calle con el rótulo, mercado, callejón, muro
verde, pueblo de madera, ruinas con el cielo 2 (`0E6DD4`) y el cielo 3
(`0E6DEA`): el mismo mapa y los mismos fondos. Los soldados compartidos
(`0AF17E`…) salen del guion de la M4 ya nativos.

Paradas contadas en ese recorrido (explícitas, a portar en la fase 2):
`world unsupported routine=83c8a` y `2d3c16` (soldado), dibujos sin
descriptor `last=1C3F06 palette_id=125` (4 clases), `MSX AUDIO stage music
0025 missing`, y los registros del guion de constructores nuevos (saltados
con su dirección). El guion se detiene de forma explícita en su primer registro contado sin
portar: `MSX script stopped at 2dba22` (constructor `08391C`, grupo 0, cámara
≈ 928; antes, 29 registros ejecutados, 3 de ellos de `033526`/`036C34`
saltados con `native 0`). En x ≥ 5504 no hay suelo todavía (lo ponen los parches
del jefe): el recorrido de prueba acaba ahí.

## Puertas (10-XI, tras fusionar `ccr` con el paquete versión 5)

`make -j4` (RC 0), `make check` (184 PASS, RC 0, con `test_m4_map` y
`test_m4_camera`), `tools/test_m1m2_ares.py` sobre un `mslugx-m1end.z64`
recién hecho (`rm -rf build/m1end`): `RESULT: PASS`; `test_mission4_ares.py`
sobre `mslugx-explorer.z64`: `RESULT: PASS`. Fusionado en `ccr` (`68e06f0`);
`/home/user/ws-logo` está en ese commit con los paquetes v5 (`build_packs.py`;
su árbol `build/` no se ha recompilado). Sin `git push`.

**Para seguir** (otra sesión): `git worktree` `/home/user/ws-m4` (rama
`work-m4`), `filesystem` → `ws-logo/filesystem`, `build/marco_*` copiados de
`ws-logo/build`; estados MAME en `/home/user/captures-m4/states`; Xvfb propio
(`:66` se usó aquí; `:64` y `:92` son de otros). Primer paso de la fase 2: el
registro `2DBA22` (`08391C`) donde se para el guion, y los parches `0C70FC`.

## Fase 2 (10-XI, rama `work-m4`): música, accesorios de la lista y voladores

Capturas de familia (MAME 0.264, `sec05_149829`, réplica de `run4`; guiones
en el scratchpad de la sesión, el mando en `tools/run_capture_fullgame.sh`):

| Captura | Variables | Contenido |
| --- | --- | --- |
| A (`captures-m4/fam/A`, 233 MB) | `MSX_FULL_FAMILY_ROOTS` = los 44 constructores nuevos anteriores al jefe, `MSX_FULL_FAMILY_FROM=149830`, `MSX_FULL_LAST=157600`, `MSX_FULL_VRAM_TRACE=1`, volcados cada 30 | todo el recorrido de `run4`: **sirve para cualquier familia nueva** (extraer con `--range`) |
| B (`fam/B`) | raíces `83c8a,8391c`, hasta 152300 | los voladores abatidos en vuelo (el piloto de `run4`) |
| C (`fam/C`) | como B con `MSX_FULL_NOFIRE=150090-153000` (nuevo en `capture_fullgame.lua`: el plan sin A ni C), hasta 152400 | aterrizan, sueltan al jinete y se estrellan |

Extracción (todas): `extract_family_fixture.py <traza> --range lo-hi --attacks
--ram 10000e-100010 --ram 10c72e-10d7b6 --ram 10c2f6-10c2f8 --ram 10c6c0-10c702
--ram 1063d8-1063da`.

**Accesorios de la lista de cámara (`src/neo_m4props.c`, familia `m4props`:
160 nodos, 11 037 actualizaciones iguales a MAME, 149830..157598).** Las 48
llamadas de la lista no eran «órdenes sueltas»: crean actores.

- `0BA942` (cabeza): RAM `10C6C0..10C6FF` (`neo_ga_flags_t.m4`).
- `0BA964..0BAA1E` (32): el accesorio `0B9F4A` n.º 0..1F (registro
  `0E68D0 + 24·n`: vida, caja, columna/fila de sus parches, lista de
  escombros `01DA40`, retrollamadas +E/+12/+1C, rectángulo, sonido 1030) en
  `0BAA68[n]` de la raíz. **Los 42 parches `0C70FC` de `run4` son suyos** (la
  nota de la fase 1 sobre `0BA8BA` era errónea): fachadas de la calle 0..5
  (cada borde compartido tiene dos variantes según el vecino), 6..1A, 1C y 1E
  un submapa, 1B y 1D dos golpes; 1C..1F son el camino del jefe (copian otra
  vez columnas de `0E6DA8` 0x20 columnas más allá cada vuelta).
- `0BA85A` el farol `0B9B24` (cae cuando muere el accesorio 5), `0BA912`
  `0B9AD6`, `0BA82A` el suelo `0B99EE` (mapa dinámico `0E79D4`), `0BA88A` el
  guardián del derrumbe `0B9C7A` y `0BA8BA(n)` sus 22 piezas `0B9D88` (solo
  reciben golpes las dos de turno, `10C6E2/10C6E4`); escombros `0BB106`,
  premios `0BB034` (`08CD58`).
- `0B9D58` lee un byte más allá de su tabla: es `10C700`, el límite de la M3
  (`7FFF` tras `0BC548`; `run4` 149850). El mundo ya no lo borra entre
  misiones y el menú de pruebas lo pone al entrar en la M4 o posterior.
- `0BB028` (jefe: `019388` con 0x8000) sigue sin portar: contado en «camera
  events unsupported».
- Paquete: hasta **64 parches por etapa** (antes 16; los bits 5..7 del byte de
  capa son los bancos 1..3 de la capa 3: `pack_mission.py`,
  `scene_patch_cell_index`); la M4 lleva 48 (`extract_map_patches.py 4`,
  desde el modelo de ROM: colores por número de paleta del mapa, los números
  55 y 68 de la RAM de paletas de `run4`; 250 de 271 columnas de los parches
  de `run4` encontradas en la VRAM del volcado siguiente). Las copias de
  `0E6DA8` del camino del jefe no tienen todavía celdas en el paquete (su
  colisión sí se aplica).

**Voladores `083C8A` y `08391C` (`src/neo_m3enemy.c`, la familia del
bombardero `084588` de la M3; familias `m4flyer` 50 nodos / 906
actualizaciones y `m4flyer2` 56 / 1661, iguales a MAME).** Vuelo, aterrizaje
(`0143DC`, rebote 1060), jinete `08443E`, rotores `084098/0841A0`, salto del
jinete `0842A8`, estela `083B92` con el humo `01C82A` (`neo_effects.c`),
muertes `0843F8` (→ `0406B6`), explosión `084B9C`, premio `084F74`. El
inventario los daba por nativos (estaban dentro del intervalo de rutinas del
bombardero): eran la parada `routine=83c8a` y el registro `2DBA22` donde se
paraba el guion. Paradas que quedan en la familia:

- el soldado `0AF17E` que crea el jinete (`084300`): un nodo genérico no puede
  crear todavía un soldado; el mundo lo cuenta (`world unsupported
  routine=af17e`) y no aparece;
- `083C98` (variante con objetivo, `+99 = 1`): no vista;
- el noveno volador de `run4` (152016) aterriza sobre el mapa dinámico
  `0E6704` y su escombro lee el indicador 8 de `013968` (152060): la prueba
  de familia se corta en 152000 (el objeto es de otra familia).

**Cubos `033526` (`neo_m4props.c`, familia `m4bin`: 7 nodos, 304
actualizaciones iguales, 149833..150481).** Registros `2DB6A2` (variante 1:
10 cosas, la 2.ª es el prisionero de tipo 0 con sus palabras) y `2DB86E`
(variante 0). Golpes (`+88`), jugador encima (`033766`: `01B332` + `01AF1A`;
**`01AF3C` incluye los dos bordes**), las llamadas de sus programas
(`239C44/58/6C` y `239C80`: premios `08CD64` de `2D3EE4[rand & 7]`, el
prisionero `239D56[+74]` = `099DD6` con `002E7C`, la nube `03378A`), piezas
`033864`, mapa de celdas `0333DC`. `099DD6` (`09AD1C` en vez de `01B1DC`) en
`neo_prisoner.c`. Paletas sin captura y sin uso en la M4: `13E/140` (variante
2) y `E1..E3` (piezas 1..3).

**Objeto `036C34` (`neo_m4props.c`, familia `m4urn`: 7 nodos, 904
actualizaciones iguales, 150322..156728; 5 registros del guion).** Dos fases
con sus respingos (`23BDDC`, `23BDF2`), mapas de celdas `036AF8/036B16`
(`0140C4`), `+4E` de `2D3A0E` y `2D3A28`, final `23BEBE` con su llamada
`036E84` (explosión `036EA8`, paleta 26C).

**`019240` y RAM `10C2F6`**: la palabra de misión es la `+0E` del descriptor
de etapa (M2 1, M3 4, **M4 5**; volcado de RAM 150000). El port le daba 3 a
la M4. `019240` con 5 da la onda `14A` y la salpicadura `150` (`neo_gate.c`;
con 8, `14D/153`; de 9 en adelante `013D1A`).

**Límites tocados**: `SCENE_EXPANDED_MIN_SLOTS` 208 → 200 (el código nuevo se
llevó la ranura 208; los tres conjuntos fijados necesitan 197),
`SCENE_MISSION_CHUNKS_MAX` 832 → 896 (la M4 tiene 834 trozos: su exportación
lleva también los actores de la M2 y la M3; **pendiente**: exportar solo lo
que la M4 usa), `GROUPS[4] = ('m3', 'm4')` (la M4 necesita los datos de ROM
de los enemigos compartidos con la M3: +152 KB de tablas).

**Recorrido en ares tras la fase 2** (`mslugx-explorer.z64`, menú, derecha +
salto + disparo, mortal): música 0x25 sonando, los voladores entran y mueren,
el guion pasa de `2DBA22`; Marco muere en la cámara ≈ 1000 (coche `027F54` y
soldados) y la partida vuelve a la M1 (sin continuaciones). Se ve el primer
parche (`MSX map patches drawn mask=…580`: el accesorio 2, como en `run4`).
Paradas contadas hasta ahí (cámara 0..1000): **solo** `af17e` del jinete (10)
y la voz `1134` sin política; ningún registro del guion saltado.

## Sonido y música

Latch (0120F8) en 149829..160500: 56 palabras, 1178 peticiones; 12 fuera de
`data/sound_map.json`: `102E`×17, `1030`×33, `1036`×2, `1053`×10, `1060`,
`1066`×43, `1071`×5, `10D1`×7, `1134` (voz MISSION 4 START), `113B`×33,
`002C` (música del jefe, 157772, `059FBA` del actor `059F76`) y `0000`×3. La
música de la etapa es la del descriptor `+C` = **0x25**.

**Fase 2**: `render_sound_commands.py --labels music_mission4
music_mission4_boss sfx_102e ... sfx_113b` (24 000 fotogramas por música) y
`pack_game_audio.py --source <render> --period music_mission4=86.67 --period
music_mission4_boss=46.96`. El buscador de bucles elegía 370,5 s / 328,7 s
(no son múltiplos del periodo: la melodía repite una frase dentro del bucle);
`--period` toma el periodo medido en todo el render y exige que se mantenga
en cada ventana de 20 s: **0x25** 86,67 s desde 1,48 s (similitud 0,98–0,99 en
las 16 ventanas, empalme 0,952), **0x2C** 46,95 s tras una introducción de
15 s (0,98 desde el segundo 15; empalme 0,904). `music_mission4*.wav64`:
2 340 532 + 202 312 y 1 268 020 + 445 852 bytes; `game_audio.c` `music_name`
0x25 / 0x2C (el 0x2C llegará por `059F76`, ya nativo, cuando exista el jefe).
Efectos: las 10 palabras están en `data/sound_map.json` y renderizadas
(`sfx_1053` en PCM16; `1071` es un bucle del controlador: sin paquete).
**Pendiente: su política de canales** (`measure_sound_pairs.py` +
`derive_sound_policy.py`, una a una como en la M3): hoy suenan `unmapped`
(`1134`, la voz MISSION 4 START; `1030` y `102E` de los accesorios).

## Pendiente (fase 2, por orden)

1. ~~Parches del mapa~~ y 2. ~~llamadas de la lista~~: hechos en la fase 2
   (arriba). Quedan los 4 sellos `0C88B0` (de otros actores) y las celdas de
   las copias de `0E6DA8` del camino del jefe.
3. **Actores nuevos**, en orden de guion ([MISION4_ACTORES.md](MISION4_ACTORES.md)):
   hechos `0B9F4A`…, `083C8A`, `08391C`, `033526` (+ `03378A`, `099DD6`,
   `033864`), `036C34`/`036EA8`. **Siguiente**: el soldado del
   jinete (`0AF17E` desde un nodo genérico), `0A9F0C` (1136), `01F828`…
4. **Tabla de columnas `+3E`**: desensamblar su lector (cerca de `0C748E/0C74C4`)
   y dejar de depender de la comprobación por observación.
5. **Jefe** `0BB028` (familia `02267E`, mapa `02225E` en `10D7B6`, cámara
   automática 5552 → 27891, suelo por parches de `0E6DA8`): estados
   `at_157560/158600/160000`. El paquete acaba en x 6144 (`camera_max` 5808).
6. **Música** 0x25 y 0x2C y las 10 palabras de efectos.
7. Capa `10D7B6` en el jefe y paletas que cambien en el jefe (no vistas antes
   de 157582).
8. `neo_collision_ring_copy` indexa el mapa desde la fila alta de la rejilla:
   con dos mapas de distinta altura (M4) los parches deben usar su propia
   colocación (`neo_collision_patch` con columna y fila del núcleo).

## Archivos

`tools/export_stage_rom.py`, `extract_m4_map_fixture.py`,
`compile_m4_map_fixture.py`, `test_m4_map.c`, `test_m4_camera.c`,
`test_mission4_ares.py`, `build_mission4.sh`, `capture_m4_actors.sh`,
`capture_m4_map.sh`, `capture_fullgame.lua` (`MSX_FULL_CAMERA`),
`extract_camera_fixture.py`, `compile_camera_fixture.py`, `pack_mission.py`
(`--rom-layers`), `extract_mission_map.py`, `compare_m3_events.py`
(`--sections`), `compile_soldier_ai_tables.py`; `src/neo_train.c/.h`,
`neo_camera.c/.h`, `neo_collision.c/.h`, `neo_world.c/.h`, `scene_assets.c/.h`,
`scene_renderer.c/.h`, `actor.c`, `gameplay.c`, `main.c`; `data/mission4_script.json`,
`mission4_actors.json`, `m4_map_fixture.json.gz`, `m4_camera_fixture.json.gz`.
Privados: `filesystem/mission4/s1/{map.json,map_observed.json,collision.json,collision_observed.json,rom_report.json}`,
`romfs/mission4.pack`.
