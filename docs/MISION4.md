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

**Furgoneta `0A9F0C` (`neo_m4props.c`, cámara 1136; familias `m4van` 272
actualizaciones y `m4van2` 260, iguales a MAME).** Cuerpo sólido `0E6508`
(`004E76`), invulnerable hasta que la cámara llega a `+6E`, 9 fases de daño
(`2D1B0C[fase] · vida >> 8`, programas `0E6836`), humos `0E6546/0E6558`,
mapas de celdas `0E665A/0E6704/0E65D8` (el `0E6704` es donde aterriza el
noveno volador), al morir cuatro premios `08CDB2` del conjunto
`2D1AAC[rand]`, la explosión `0AA2C6` (fila `0E65BC`) y el chasis
`0AA1FE/0AA252`; su capa `0AA37C` sigue la fase del cuerpo (`0E6862`) y su
parpadeo (`0044F6`). `m4van` (cuerpo + capa) se corta en 152048 y `m4van2`
(cuerpo + explosión, sin la capa) llega al final: en 152049 la fila de la
explosión hace parpadear al cuerpo (`+17`) entre la actualización del cuerpo
y la de la capa, y la prueba de familia solo repone esa escritura en la
siguiente actualización del cuerpo. **`019DFC` solo pone `+69 = FF`** (no
borra `+50`; corregido en los voladores, que se borran en esa misma
actualización). No se ve en el recorrido de ares: Marco muere antes (1000).

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

### Puertas de la fase 2 (10-XI, `work-m4` `15e49c1` con `ccr` de la M5 fusionado)

`make -j4` RC 0; `make check` RC 0 (**207 PASS**, con `m4props`, `m4flyer`,
`m4flyer2`, `m4bin`, `m4urn`; tarda ≈ 40 min con la máquina compartida);
`tools/test_m1m2_ares.py` sobre un `mslugx-m1end.z64` recién hecho (`rm -rf
build/m1end`): `RESULT: PASS`; `tools/test_mission4_ares.py
mslugx-explorer.z64 --seconds 100 --jump 2.5 --fire` (menú de pruebas, `Xvfb
:67`, datos de ares copiados de `scratchpad/aresdata`): `RESULT: PASS`, hasta
la cámara 1000 con las paradas de arriba.

Segunda fusión (furgoneta `0A9F0C`, `99b81e4`): `make check` RC 0 (**209
PASS**), `make -j4`, `test_m1m2_ares.py` (m1end nuevo) PASS y el recorrido de
la M4 PASS (cámara 1000; esta vez caen los accesorios 1 y 2: máscaras de
parches `…58` y `…568`). `ws-logo` queda en el commit de `ccr` con
`romfs/mission4.pack` y `mission5.pack` rehechos (`build_packs.py 4`); **el
`mission6.pack` de otra rama apunta a trozos de los paquetes anteriores: hay
que rehacerlo tras cada cambio del de la M4**.

**Pendiente de la prueba de familia**: reponer las escrituras de un nodo de
la familia sobre otro *dentro del mismo fotograma* (hoy se reponen en la
siguiente actualización del nodo escrito): con eso `m4van` llegaría al final
con la capa `0AA37C`.

**Para seguir** (otra sesión): `/home/user/ws-m4` (`work-m4`); capturas en
`/home/user/captures-m4/fam/{A,B,C}` (A sirve para todas las familias que
faltan menos el jefe); utilidades de la sesión en el scratchpad (`dis2.py`,
`dis3.py` desensamblan P1 con capstone plegando la comprobación del
cartucho; `prog.py` lista las órdenes de un programa; `exA.sh FAMILIA
RANGOS` extrae un fixture de la captura A; `fam.sh FAMILIA` compila y corre
una sola familia; `export4.sh CAPTURA@lo-hi…` exporta sprites y rehace el
paquete; `m4run.sh NOMBRE SEGUNDOS --fire` hace el recorrido en ares).
Receta por familia: tablas de ROM a `data/region_rom.json` (grupo `m4`),
programas a `extract_soldier_programs.py` (bloque «# Mission 4 …») y
`export_rest_sprites.py` (ROOTS/RESOURCES), código en `neo_m4props.c` o el
módulo de la familia original, `FAMILIES` + `FAMILY_ARGS_` en el Makefile.
Los nodos que crea el guion necesitan `--script-actor 101414
--script-routines <los que crea la familia con 00411C>`. **No editar fuentes
mientras corre `make check`.**

## Fase 2c (11-XI): estado y cómo seguir

Hecho y verificado desde la fase 2 (commits `9606f2c`, `c4dd267`, `bf7b7ad`,
`make check` RC 0 en el árbol fusionado con la M6 fase 1):

- **Soldado del jinete** (`084300`): un nodo genérico crea un soldado
  (`g_spawn` de `neo_world.c`, vista del soldado para sus palabras `+6E..`).
- **Familias completas**: `m4flyer` (captura D, con la furgoneta, hasta
  152371), `m4van` (con su capa, hasta el final), `m4bin` (la vida entera del
  prisionero).
- **`m4rock`** (`01F828`/`01F866`, rocas `01FABA`, sombras, humos: 49 nodos,
  3690 actualizaciones), **`m4jump`** (`07E554` + `07EC6A` + `07EDF0`: 18
  nodos, 426; la caída `07E832` y la granada `07EB4E` no salen en la captura:
  paradas explícitas), **`m4box`** (`09A71E`: 1990 actualizaciones).
- **Política de canales** de 9 de las 10 palabras de la M4 (medida,
  `data/sound_pairs.json` → `data/sound_policy.json`); `1071` es un bucle del
  controlador sin paquete.

Capturas nuevas (privadas, `/home/user/captures-m4/fam/`):

| Captura | Origen | Contenido |
| --- | --- | --- |
| E | `run4` desde `sec05`, raíces `99e06,de0fe,de24a`, intervalo `ddcda-de4ff`, 152700..157600 (`scratchpad/capE.sh`) | el prisionero que pelea (`0DDCDA..0DE06E`) y sus disparos `0DE0FE`/`0DE24A` |
| F | `run4`, raíces `8c374,8c37c,8cf6e`, intervalo `8c374-8d900`, 151000..155900 (`capF.sh`) | los objetos de los tipos 40/41 (animales, programas `289082`/`289CD6`) con su destello `08CF6E` |
| **T** | **TAS** (`tas_updates.lua`, sin escribir en la memoria) desde `u12000` hasta `u12890`, raíz `2267e`, intervalo `22000-26400`, volcados cada 30 (`capT.sh`) | **el jefe entero y el fin de la misión** (fotogramas 26052..27911; 0 desincronizaciones con los grifos puestos) |

`tools/tas_updates.lua` tiene ahora la traza de familia y los volcados de
`capture_fullgame.lua` (`MSX_TAS_FAMILY_ROOTS/RANGE/NODES/VRAM`,
`MSX_TAS_DUMP_PERIOD`), solo lectura.

**Helicópteros `08514A..086062` (`src/neo_heli.c`, familia `m4heli`: 51
nodos, 4774 actualizaciones iguales a MAME, 154171..157449; captura A,
`--range 8514a-86070 --ram 101500-101508`).** Tres raíces del guion:

- `08571C` (+ fogonazo `085952`): helicóptero de fondo a escala (`+12/+13`,
  `AF` si el registro trae 0), sin caja; velocidad de rango `2D5320`, al pasar
  su x objetivo espera rango `2D533A` actualizaciones (con fogonazo si `+70`)
  y da la vuelta (`FF80`/`1C0`).
- `085522` (+ fogonazo `0859E4` y bomba `085ACE`): el que ataca. Vida de rango
  `2D5132`, bombas `2D51CE`, límites de velocidad `2D514C[rango 2D515C]`;
  persigue al jugador (`085D2E`: rumbo entre 16 por las puntuaciones
  `085FB8/085FC1`, velocidad por la distancia `002F1A`, 1/16 por
  actualización, tope ±8 0000) y, tras la espera de rango `2D521C` y sin
  jugador dentro de `0E4DB4`, se para y dispara (la llamada `085DEE` del
  programa del fogonazo crea la bomba tras el actor del guion con el rumbo al
  jugador / 16). Se va (`0856CA`, `03B332`) cuando la cámara pasa su `+80` o
  se le acaban las bombas. Al morir (`085880`) suelta su objeto (`08604A` →
  `08CDB2` con `+7A/+7C`).
- `08514A` → `085294`: la escuadrilla. Un helicóptero cada 3000 / rango
  `2D50A6` actualizaciones por el camino `0E4D04/0E4D10[+8A]` (`021288`); cada
  rango `2D5024` actualizaciones uno de los vivos dispara (`+8D`); si caen
  todos, los puntos `08D1D0` con `+84`.

`2D521C` es una tabla de **tipo 2** de `0260C8` (fila de bytes, juegos de 16
bytes, una palabra al azar de 8: `02617A`): `ga_rank_word` devuelve el byte
de la fila, no la palabra (**pendiente**: corregirlo en `neo_gactor.c`; las
familias con tablas de tipo 2 llevan su propio lector, como `neo_m3boss.c` y
ahora `m4h_rank`). El entorno lleva la palabra baja de `+40` de los jugadores
(`player_ylow`, RAM `101506`); **el mundo no rellena todavía ni `player_low`
ni `player_ylow`** (valen 0: el jugador del port no tiene fracción de píxel
que darles; pendiente). Exportación de sprites: 142 entradas con zoom
saltadas (los de fondo) y 1360 píxeles distintos en 10 fotogramas de
validación (156030..157410, helicópteros bajo fuego): **sin revisar**.

**Soldado que salta `0AF1A8` (`src/neo_soldier_ai.c`; familias `m4sold`,
269 actualizaciones, y `m4sold2`, 1099, iguales a MAME).** Es la entrada de
soldado con `+81 = E0`: solo tiene las acciones 5..7 de la tabla `0AF494`,
todas `0AF99A`. Corre hacia el jugador (`23D510`, `+62` = ±4 0000, velocidad
del programa `|+62| >> 10 + 80`; con el jugador montado en un vehículo,
`0AAA88` = palabra de su ranura RAM `10C608`, hace la acción 1), a menos de
0x40 del más cercano (`01AF9A`) salta (`0AFA4C`: `23EE3C`, `+34` = 8 A000,
`+62` = ±(2 8000 + rand + rand)); al caer la fila `0AE990` encuentra al
jugador (`0AFADA`, `23EEB6`) y la `0AE9A0` le pega (`0AFAF0` → `0AFB44`);
en el suelo `0AFB88` (`23EEEA`) y vuelve a `0AF380`. **Era la parada del
guion en `2DBB50`** (cámara 1256: el primer registro contado de `0AF1A8`,
dentro de un corredor de 20 soldados): el recorrido de ares no pasaba de ahí.

Prueba: los soldados no son actores genéricos; ahora la prueba de familia
los admite como nodos (`compile_family_fixture.py --soldier-nodes`,
`soldier_tick_node` de `tools/test_neo_family.c`): los bytes del nodo pasan a
un `neo_soldier_t`, una llamada a `neo_soldier_construct` (entradas
`0AF17E..`) o `neo_soldier_tick` con la escena de la actualización (núcleo
con anillo y objetos dinámicos, RNG, rejilla de retardo `1067E6`, jugadores),
y vuelta. Se comparan los campos que el módulo modela (los de
`soldier_capture.py` y el bloque `+6E..+93`); los demás bytes toman el valor
original. La caja del jugador 1 se ofrece al soldado como en
`test_neo_soldier_ai.c` (su fase estrecha propia, sin reproducir las
respuestas grabadas). `m4sold` = captura A (seis soldados, abatidos en el
aire); `m4sold2` = captura I (`run4` desde `sec05` con
`MSX_FULL_IDLE=153480-154400`, raíz `af1a8`: el salto cae sobre el jugador
parado, `0AFADA`×2 y `0AFAF0`×24; **`0AFB44`/`0AFB4A` no salen en ninguna
captura**: traducidos de la ROM, sin comprobar). Con la captura H (pad sin
disparo) la misma prueba se para en 153822 (`0AF65A`, `+4D` 0 por 1: rutina
que ya existía; **pendiente** ver si es el arnés —época de los objetos
dinámicos— o el módulo). Extracción: `--range af000-b0fff` con el juego de
RAM de la M4 más `--ram 1067e6-1068e6`.

**Prisionero que pelea `0DDCDA..0DE06E` con sus disparos `0DE0FE` / `0DE24A`
(`src/neo_prisoner.c`; familias `m4pris`, 2211 actualizaciones, y `m4pris2`,
1321, iguales a MAME).** `099E06` con `+74 = FFFE`: al acabar el saludo
(`09A30E`) salta a `0DDCDA` con el nodo del jugador en `+76`. Lo sigue (a más
de 0x28 anda a ±4 0000, `0DDD62`; a menos de 0x18 se para), busca enemigos con
las filas `0DE494` (cualquiera en 1024 × 512: el **nodo** que `0049E8` devuelve
en A0 va a `+6E` y el programa `2AFE14` dispara, llamada `0DE3DA`) y `0DE4A4`
(tipo 5 a su lado: el puñetazo `2AFF14`, llamada `0DE3CE`, sonido `113B`),
vuelve a la pantalla (`0DE33E` / `0DE35E` → `0DDDC2`), salta donde se acaba el
suelo o el jugador está a otra altura (`0DE37E` → `0DDF5A`) y cae (`0DDF8C`).
Si el jugador muere o no está (`0DE016`: `+87`) saluda y se va (`0DE064` →
`09A318`). El disparo `0DE0FE` persigue al nodo `+6E` (`0040AC`, `002EB4`:
8 unidades de ángulo por actualización, velocidad 0x1000 · coseno / seno,
`0DE262`) 15 actualizaciones y estalla (`0DE24A` → `01C706`) con la fila
`0DE474`; con un enemigo dentro de `0DE484` la llamada crea el estallido
directamente.

Motor compartido: `neo_ga_target_t.tag` (el nodo de A0 / A1: en el mundo la
ranura del actor genérico, del soldado con su generación o del nodo de jefe;
en la prueba, la dirección del nodo) y `e->node(tag)` (los campos que otro
actor lee de ese nodo o del de un jugador `1014C4` / `101574`: `+00` = `3F1E`
si ya no existe, `+3C..+43`, `+5E`, `+5F`). El mundo guarda soldados, nodos de
jefe y actores genéricos en reservas separadas: una ranura reutilizada solo se
lee como el ocupante nuevo entre actores genéricos (en el original, una sola
reserva). Prueba: la reserva de nodos entra como RAM (`--ram 1010a4-105564
--ram-creators`: solo lecturas). `m4pris` = captura E; `m4pris2` = captura J
(`scratchpad/agA/capJ.sh`: `run4` con `MSX_FULL_IDLE=153720-155200`; el
prisionero dispara parado, `0DE18E` × 168). **Sin captura** (traducido de la
ROM, sin comprobar): el puñetazo `0DDF14` / `0DDF42` / `0DE3CE`, la despedida
`0DE064` y el borrado de `+6E` de `0DE0FE` (el objetivo en x 0x20..0x100 del
largo `+3C`).

**Animales (objetos de tipo 40..42; `src/neo_item.c`; familia `m4animal`: 54
nodos, 1554 actualizaciones iguales a MAME, 2 recogidas comprobadas con el
bloque de armas `10C718` como RAM — contador de comida `10C728`).** Movimiento
`08BAAA` = `08CCD4` (`01456A` en el suelo, `0143DC` en el aire) + `08B9D8`;
llamadas de sus programas `289082` / `289CD6` / `28A12C`: `289528` (rebote en
pared), `28953E`, `289552`, `289570` (`+62` = ∓1 4000), `2895A2` / `2895B2` /
`2895C2` (azar a `+7D`) y las doce de soltar fruta `2895D8..28969E` →
`2896B2` (`08CD58` tipo 3D o 3F a (±0x11, 7), `+30` = ±velocidad de la
entrada, `+74` = 0 / 1 / 2). Frutas: movimientos `08BB3C` / `08BB44` (tabla
`08BB24`: caída con rebote a la mitad, espera `+8D` = 15, programa `28992A` /
`289B44`). Captura F2 (como F; la traza de F ya no existía). **Sin captura**:
en `run4` y en tres réplicas con el mando parado (151250 / 151270 / 151290)
ningún animal suelta fruta (lo decide el azar en cada ciclo de paseo), así que
`2896B2`, `08BB3C` / `08BB44` y el tipo 42 están traducidos de la ROM sin
comprobar; la llamada `289A80` del programa de la fruta 3F (crea `032FB4`) no
está traducida: parada explícita. Prueba de familia: `--item-kinds` (solo los
objetos de esos tipos), `--script-kinds` (qué objetos hechos tras el actor del
guion son de la familia) y `--pickup-ram`.

**Rótulo de recogida `08C752` con sus letras `08C954` (`src/neo_item.c`;
familia `m4label`: 6 nodos, 390 actualizaciones iguales a MAME, captura M =
`capF.sh M 8c752,8c954 8c752-8cf6d 152900 153800`).** `08CD10` (D0 = jugador,
D1 = texto de `0E5782`: 0 gasolina, 1 medicina, 3 munición, tipo de bomba + 3)
lo crea tras el actor del guion; sin texto (`+72` = 0) es el rótulo de puntos
`08D330`. Con texto: cuenta `10C5B2[jugador]` en `+80`, una letra `08C954`
por byte de la lista (programa `0E565A[+76]`, o `0E56EE` con `10C2F6` = 0;
paletas `0E559E`), que espera su turno, sube `+82` actualizaciones, sigue el
rebote `0E55EC`, se para (0x96 actualizaciones o hasta que el rótulo pone
`+89` = 0) y sale parpadeando por `0E55CA`. **Sin captura**: el final de las
letras (`FF` de `0E55CA`), el borrado del rótulo (`004188`) y el del árbol con
cuatro rótulos posteriores (`0041AC`: la actualización que borra queda fuera
de la traza); el sitio de la munición `08B6C8` (D1 = 3) no existe en el port.
**No está conectado** (`MSX_PICKUP_LABEL` 0 en `neo_item.c`: sigue contado
como externo): los sprites de las letras solo están en la exportación de la
M4 y sin validar (las capturas E / M / F2 no tienen volcados de VRAM, 0
fotogramas validados); hay que exportarlos a `filesystem/rest-native.json`
(base, todas las misiones) con una captura con volcados y poner el 1.

Lo que falta, con lo que se sabe de cada cosa (nada de esto está portado: son
paradas explícitas contadas):

- **`0587C4`** (+ torreta `0592C6`, restos `059B5C`; 1160 líneas,
  `scratchpad/agA/587c4.asm`): vehículo de tierra del registro de cámara 4713
  (`+78` = 1; otras entradas `0587D8` = 3, `058806` = 2), cuerpo sólido
  `004E76` (`0586A6`), vida de rango `2D105E`, velocidad tope `2D1122`,
  estados por `+80` (`059C2C` elige programa). **La captura A no sirve**: en
  `run4` muere 24 actualizaciones después de pararse (`058DD4`), antes de
  disparar (`058E30` no sale). **Captura L hecha** (`scratchpad/agA/capL.sh`:
  `run4` con `MSX_FULL_IDLE=156880-158400`, raíces `587c4,587d8,58806,592c6,
  59b5c`, intervalo `587c4-59f76`; traza en `/home/user/captures-m4/fam/L`):
  vive 1520 actualizaciones y dispara — cuerpo `058E30` ×6, `058E48` ×362,
  `058EFC` ×10, `058F4A` ×230, `058F66` ×10, `058FF2` ×70; torreta `05934C`
  ×761, `0594B0`, `0594CE`, `0595B2` ×880; disparos `05996A`, `05999A` ×4022,
  `0599E2`, `059A3E`, `059A62`, `059A9C`. La muerte (`059048`, `0590B8`,
  `059B5C`) está en la captura A. Falta traducir (≈ 1000 líneas) y probar con
  las dos.
- **Jefe `02267E`** (captura T): el constructor crea 3 `023526`, 4 `02391E`,
  `024226`, `0246D0`, `022CB6` y la música `059F76`; después `023EB4`,
  `023FDE`, `025DC2`, `024F0E` → 64 `02505C`, `025CDE`, `025B6E`, `02591C`,
  `02530E`, `025362` → `025512`, `0257DC`, los escombros `0259F2` /
  `025A5E` de `01D764` y las explosiones `01C6EE/01C706/01C71E`,
  `01CE08..01CE44`. 95 rutinas visitadas por el TAS entre `02267E` y
  `025E18` (3063 líneas desensambladas, `scratchpad/boss4.txt`). El guion de
  la sección se cierra en la actualización 12463 (fotograma 26982) y la
  sección 6 (M5) empieza en 12887 (27901).

**Sprites de esta sesión**: `ws-m4/filesystem` es ahora
`/home/user/ws-m4-private/filesystem` (enlaces a `ws-logo/filesystem` salvo
`mission4`, copia propia: `ws-logo` no se toca). La exportación de la M4 con
el prisionero que pelea, los animales, las frutas y las letras está en
`/home/user/ws-m4-private/filesystem/mission4/rest-native.json` (10 721
descriptores; `mission4.pack` 981 trozos); **para que `ccr` la tenga hay que
copiarla a `ws-logo/filesystem/mission4/` y rehacer los paquetes
(`build_packs.py 4`)**. Sin validar contra VRAM (0 fotogramas) y con 64
paletas sin captura (216, 367, 3D2… dibujos que faltan en el recorrido).

**Capturas tras la segunda sesión** (`/home/user/captures-m4/fam/`): quedan
las trazas de **A** (todo el recorrido), **T** (jefe y fin de misión, del TAS)
y **L** (vehículo `0587C4` con el mando parado); las de D, E, G, I, J, F2, K y
M se borraron después de hacer sus fixtures (disco al 95 %); se rehacen con
`scratchpad/capE.sh`, `capF.sh`, `capI.sh` y `scratchpad/agA/capJ.sh`,
`capK.sh`, `capL.sh` (≈ 1–2 min cada una).

**Siguiente paso**: (1) `0587C4` con las capturas L y A (≈ 1000 líneas de
68K: `scratchpad/agA/587c4.asm`); (2) el jefe `02267E` (captura T, 95
rutinas, `scratchpad/boss4.txt`) y el fin de misión — **antes hay que
recuperar memoria de programa** (docs/MEMORY_OPTIMIZATION.md: queda una
ranura de caché sobre el suelo de 200); (3) exportación base de las letras
del rótulo y validación de sprites con capturas con volcados de VRAM.

### Puertas de la segunda sesión (11-XI, `work-m4` `54adba8` con `ccr` de la M5 y de los personajes fusionado)

`make -j4` RC 0; `make check` RC 0 (**251 PASS**, con `m4pris`, `m4pris2`,
`m4animal`, `m4label`); `tools/test_m1m2_ares.py` con `mslugx-m1end.z64` recién
hecho: `RESULT: PASS` al segundo intento (en el primero una pulsación de START
del propio guion de la prueba cayó ya en la partida y, con la pausa de la ROM
recién fusionada, la dejó parada: **carrera de la prueba**, no del juego);
`tools/test_mission4_ares.py mslugx-explorer.z64 --seconds 150 --jump 2.5
--fire --lifebar`: `RESULT: PASS`, `unsupported=0`, caché de escena 201
ranuras (suelo 200).

## Fase 2d (11-XI, tercera sesión): memoria, vehículo `0587C4`

**Memoria de programa** (lo primero: la caché estaba a una ranura del suelo): los 928 programas de animación sin
grupo se leen bajo demanda de `/programs.bin` ([MEMORY_OPTIMIZATION.md](MEMORY_OPTIMIZATION.md) «Programas bajo
demanda»): imagen −182 KB con el vehículo dentro, caché 212 ranuras, suelo otra vez 208.

**Vehículo de tierra `0587C4` (`src/neo_m4veh.c`; familias `m4veh` 7192 actualizaciones, `m4veh2` 218, `m4veh3`
568 y `m4veh4` 4487, todas iguales a MAME).** Registro de la cámara 4713 (`+78` = 1). Cuerpo: vida de rango
`2D105E`, caja `05868E`, cuerpo sólido `0586A6`, paleta 1C (destello 30B), la torreta `0592C6` y la onda `03C532`;
el estado `+80` elige el programa de la tabla `+7C` (`058752` cuerpo, `058776` torreta; `059C2C`): 0 avanza
(`0589D2`: `+62` crece `+82` hasta rango `2D1122` << 8), 1 frena (`058C66`), 2 espera (`058CE4`), 3 saca el cañón
(`058DD4`), 4 apunta (`058E48`), 5 fogonazo `059A3E` / `059A62`, 6 disparo `0594B0` / `0594CE` (cada rango `2D124C`
actualizaciones, rango `2D11B8` disparos, luego rango `2D11EA` de espera), 7 destruido (`059048` / `0590B8`: pieza
`059B5C`, `019DFC`, sonidos 1021 / 102F, `01DBAA`). Cada actualización `059214`: suelo o caída, borrado fuera de
`0586C2`, embestida `05871A` si `+72`, caja, programa. Torreta: sigue al cuerpo (cara, estado, profundidad + 1,
destello `0044F6`); cuando ella o el cuerpo mueren (`0593B6`) el cuerpo pasa a ser de tipo 2 (`+8F` = FF) y sale el
estallido `01C6EE`. Disparo: la llamada `059770` de su programa lo lanza (rangos `2D1154` / `2D1186`), `0595B2`
con la estela `05996A` (`0599F0`), el giro `0597DE` bajo `+7C` o sobre un jugador dentro de `0586DA`, las filas
`0586E2` / `0586F2`; al acertar, tocar suelo o con `+8F` del cuerpo: sonido 1022 y el estallido `059852`.

Capturas (`scratchpad/agA/`): **L** (mando parado, `capL.sh`), **A** (el piloto de `run4` lo destruye), **N**
(`capN.sh`, `MSX_FULL_NOFIRE`: el piloto sigue andando, el cuerpo se borra fuera de pantalla y la torreta acaba por
`0593B6`) y **N55** (`HOP=157200-158400:55 capL.sh`: opción nueva `MSX_FULL_HOP` de `capture_fullgame.lua`, el
jugador parado salta sobre el sitio y los disparos le dan: 8 estallidos `059852`). L2 = L con volcados cada 30
(`capL2.sh`) para los sprites. Las trazas de N y N55 se borraron tras hacer los fixtures; quedan A, T, L y L2.

Motor compartido: `ga_attack` admite **varias parejas (clase, daño)** en una fila de `0049E8` (`004B06`: la
embestida `05871A` lleva las clases 0 y 1; antes era una parada explícita); `neo_genv_t.attack_pair` marca las
llamadas de las parejas siguientes (la prueba de familia cuenta una sola `0049E8`).

Sprites: 136 descriptores nuevos y las paletas 1C, 1D, 1E, 30B y 37C en la exportación privada de la M4
(`/home/user/ws-m4-private/filesystem/mission4/rest-native.json`, 10 857 descriptores). **Validados contra VRAM: 50
fotogramas** (capturas L2 y A, rutinas `587c4-59f76`): 125 992 píxeles iguales, 403 distintos en 6 fotogramas de L2
(157290, 157320, 157590, 157650, 157800, 157860: entre 2 y 97 píxeles cada uno, **sin revisar**), 7182 solapados.

### Sin verificar, pendiente de captura (traducido de la ROM; ninguna captura lo ejecuta)

Lista medida con `gcov` sobre las cuatro familias del vehículo y la que ya constaba de las sesiones anteriores:

| Familia | Sin captura |
| --- | --- |
| Vehículo `0587C4` | las entradas `05879A` / `0587D8` / `058806` (tipos 0, 3 y 2) y lo que solo ellas alcanzan: `058AA8` / `058AEC`, `058B84` / `058BC2`, `058DF6` / `058E0A`, las pruebas de jugador `059EFC` de `058CE4` y `058E48`, la vuelta por `+7A`; el respingo `059C4C` (`24BA5E`) y su reanudación; las ramas mirando a la derecha (`+15` bit 0) de `0589D2` y `059770`; el mapa de celdas del resto (`058656` / `058672`, `+6E` = 1) |
| Prisionero que pelea `0DDCDA` | el puñetazo `0DDF14` / `0DDF42` / `0DE3CE`, la despedida `0DE064`, el borrado de `+6E` de `0DE0FE` |
| Animales (tipos 40..42) | soltar fruta `2896B2`, las frutas `08BB3C` / `08BB44` (3D / 3F), el tipo 42; la llamada `289A80` **no está traducida** (parada explícita) |
| Rótulo `08C752` | el final de las letras (`FF` de `0E55CA`), el borrado `004188` y el del árbol `0041AC`; el sitio de la munición `08B6C8` |
| Soldado `0AF1A8` | `0AFB44` / `0AFB4A` |
| Soldado `07E554` | la caída `07E832` y la granada `07EB4E` **no están traducidas** (paradas explícitas; `07E832` salió 4 veces en el recorrido de ares) |

Se intentó en esta sesión: el vehículo (hecho: N y N55 cubren el borrado, el fin de la torreta y el estallido).
La fruta de los animales: otras seis réplicas con el mando parado desde 151310 / 151340 / 151370 / 151400 / 151440 /
151480 (`scratchpad/agA/tryfruit.sh`: busca escrituras con PC en `2896B2..`): ninguna suelta fruta (nueve réplicas
con las tres de antes); sigue pendiente.
`MSX_FULL_HOP` + `MSX_FULL_IDLE` sirve para forzar más situaciones desde `sec05` (un jugador parado que salta).
Cuarta sesión, el prisionero que pelea (raíces `99e06,de0fe,de24a`, `scratchpad/agA/capP.sh` / `capQ.sh`): `run4` sin
disparo desde 153720 (`MSX_FULL_NOFIRE`, hasta 157600), mando parado desde 156880 con saltos desde 157200 y mando
parado desde 153720 con saltos desde 156880 (hasta 158400): en las tres el prisionero vive hasta el final disparando
(`0DDE9E`, `0DE18E`) y **ni puñetazo ni despedida**. La despedida pide que el jugador muera, y las capturas lo hacen
invulnerable (`101523` bit 6, `capture_fullgame.lua`): se probó a borrar el bit desde 153720 y no volver a ponerlo,
pero el propio juego lo pone otra vez dos fotogramas después (escritor sin identificar; hace falta un punto de
vigilancia del depurador de MAME en `101523`). Siguen pendientes.
Fruta: `run4` sin disparo desde 151000 y desde 150400 (`scratchpad/agA/tryV.sh`, raíces de la captura K): ningún
animal en la traza (los mata otro o no llegan a crearse con ese mando); en ares, en cambio, el recorrido de la prueba
sí llegó a `289A80` (una vez): el mando de la prueba desde el menú es un camino para la captura si se reproduce en
MAME. Escritores de `101522` vistos con el depurador (`wpset`, 153700..153760): `0039B0` / `0039B6` / `003A38` /
`003A3E` (copias), `004392`, `004C76` / `004C94` / `004D1A` / `004D64`, `00A046`, `01293C..0129D6`.

**No hecho**: el jefe `02267E` (captura T, 95 rutinas), el fin de misión, las celdas de las copias de `0E6DA8` del
camino del jefe y la conexión del rótulo (`MSX_PICKUP_LABEL` sigue en 0): sus letras tienen que estar en la
exportación **base** (`filesystem/rest-native.json`, de la que sale `scene.pack` y `scene_meta.h`), y esa exportación
con la lista de raíces actual metería en la base los sprites de todas las misiones; hace falta su receta (o un
filtro de raíces) y rehacer `scene.pack` y todos los paquetes, coordinado con el trabajo de compresión de
`sprites.bin`. En el recorrido de ares salieron además dos paradas sin familia: `world unsupported
routine=2058e` y `routine=1fece` (una vez cada una).

### Puertas de la tercera sesión (11-XI)

`make -j4` RC 0 (`mslugx-explorer.z64` 67 010 560 bytes: **quedan 32 KB hasta los 67 043 328** con los que ares deja
de escribir el log; el cartucho de referencia mide justo 67 043 328 y aún funciona); `make check` RC 0 (**256 PASS**,
con `m4veh`..`m4veh4` y la prueba de `programs.bin`); `tools/test_mission4_ares.py mslugx-explorer.z64 --seconds 150
--jump 2.5 --fire --lifebar`: `RESULT: PASS`, `unsupported=0`, **212 ranuras** (suelo 208), 488 KB libres,
`heap_low` 277 KB; `tools/test_m1m2_ares.py` con `mslugx-m1end.z64` recién hecho: `RESULT: PASS`, 213 ranuras,
`heap_low` 308 KB (la prueba ya no pulsa START fuera de las cinemáticas: la pulsación caía en la partida y era la
pausa de la ROM; `cinematic_running`). Misión 1 entera con `mslugx-reference.z64`: PASS.

Tras fusionar `ccr` (tercer lote de la M5, `40bbf40`): la imagen sube a 2 197 664 bytes y el cartucho a
67 207 168 (**pasa de 67 043 328: ares no escribe log**), así que las dos pruebas de ares se repitieron con
`romfs/mission6.pack` apartado (cartucho 65 994 752) y devuelto después: M4 `RESULT: PASS`, **211 ranuras**,
`heap_low` 284 KB, 0 paradas del mundo; M1 → M2 `RESULT: PASS`, 212 ranuras; `make check` RC 0 en el árbol fusionado.

Segunda fusión de `ccr` (cuarto lote de la M5: el tanque de orugas `08FF7E`, el helicóptero `047F7E`, `ga_rank_word`
de tipo 2): `ga_attack` con varias parejas (`004B06`) estaba hecho en las dos ramas; queda la de la M5 (mismo
recorrido; sin la parada tras ocho parejas) y un solo `attack_pair`. El paquete de la M5 pasa a **1037 trozos**
(el grupo de la M3 dentro): `SCENE_MISSION_CHUNKS_MAX` 1024 → 1088 (si no, el cargador rechaza el paquete). Suelo
de la caché: el de la M5, 197 (la necesidad). Puertas: `make check` RC 0 (**263 PASS**, `romfs/` rehecho con
`build_packs.py`); con `mission6.pack` apartado (cartucho 66 371 584; con él 67 567 616): M4 `RESULT: PASS`,
**210 ranuras**, `heap_low` 277 KB, una parada `routine=289a80` (los animales: la llamada sin traducir, ya en la
lista de pendientes; **ares sí la alcanza**); M1 → M2 `RESULT: PASS`, 210 ranuras.

Tercera fusión (quinto lote de la M5: el metro `051344`): `make check` RC 0 (**264 PASS**); suelo de la caché otra vez
**208** (no se baja ningún suelo): con `mission6.pack` apartado (cartucho 66 387 968) M4 `RESULT: PASS` **209 ranuras**,
`heap_low` 279 KB; M1 → M2 `RESULT: PASS`, 210 ranuras.

**Jefe `02267E`, primeras piezas (`src/neo_m4boss.c`, familia `m4bdeb` de la captura T: 135 nodos, 4069
actualizaciones iguales a MAME).** El reflejo `02591C` (paleta F8, profundidad 0200, `2348D4`, sin recorte). Escombros `0259F2` (paleta F0, programa `246AD2`, cara al azar `0029C6`; `025A42`:
`01ABC8`, programa, `024978` = fuera del rectángulo `E007C`) y `025A5E` (paleta 276, `+30` = 1000, `+34` = C000,
`234D04`; `025AAA`: `01AC38`); destellos `025B6E` / `025BE2` / `025CDE` (profundidad 1301, paleta FC, `234C06` /
`234BFC` / `234C62`, sin recorte) y `025DC2` (sobre su creador con `01AD6E`: `+38` = creador + 28; programa `+90`
de la tabla `E06DA`, `024978`). Prueba de familia: con `--ram-creators` un padre de la reserva de nodos se lee de la
imagen de RAM. **Falta**: el resto del jefe (cuerpo `02267E`, `022CB6`, `023526`, `02391E`, `024226`, `0246D0`,
`023EB4`, `023FDE`, `024F0E` / `02505C`, `02530E`..`0257DC`), sus sprites en la exportación (los de estas
piezas tampoco están: no se dibujan hasta que exista el jefe), el mapa `02225E` / la capa `10D7B6`, la cámara
automática y el fin de misión.

Tras la línea base de la matriz (`b111a4c`): `make check` RC 0 (275 PASS; un primer intento falló en la prueba de
compresión porque la exportación compartida de la M6 estaba a medio escribir por otro agente: 1194 trozos en formato 4;
al repetir, bien); matriz: todo PASS menos M6 etapa 1 (la de la línea base) y M5 etapa 1 con `unsupported` 0 → 1
(`routine=aff5e`, el soldado `0AF1A8`: **intermitente**, 1 de 4 ejecuciones con el mismo cartucho; sale también
antes de esta fusión). Tras fusionar además la máquina del metro de la M5 (`467e945`): `make check` RC 0 (**276 PASS**); la misma matriz da lo
mismo (M6 etapas 1 y 3: solo la paleta 125) y la M4 queda en **208 ranuras = el suelo** (sin margen: el próximo
módulo necesita liberar RAM). Antes, tras los personajes (`fd40fb6` + jefe): `make check` RC 0 (**275 PASS**); `regression_ares.py` con
el cartucho entero (61 554 688 bytes, con `romfs/chars` enlazado de `ws-logo`): logo, pausa, personajes, M4 (209
ranuras), M5 ×3 y M1 → M2 PASS; M6 etapas 1 y 3 FAIL por la paleta 125 (`missing descriptor … palette_id=125`, el
fallo abierto que lleva el agente C); `missing_part=1` en cada prueba de misión = `/m4s1.bgp missing` (prerender
privado que no está en este árbol). Corregido en el ejecutor: la prueba del logo informa con JSON (`"status":
"PASS"`) y salida 0, no con `RESULT:`, y contaba como FAIL.

**El vehículo no se ha visto en ares**: con el cartucho de referencia (jugador invulnerable) y el mando de la prueba
(derecha + salto + disparo, 760 s) la cámara se queda en 4360 con el jugador contra el borde derecho (x 4622..4651;
los grupos del guion pasan por 1, 2, 0) y el registro de `0587C4` está en 4713. El log da ahí `MSX camera autoscroll=0 progress=4360` (una orden de cámara del guion)
y los grupos siguen con enemigos vivos: lo probable es que el mando de la prueba no los mate (no dispara hacia
arriba a los helicópteros `085522`), pero no se ha comprobado contra `run4`; el mortal muere en 2160.
Paradas nuevas vistas en ese recorrido largo: `routine=1fece` y `routine=2058e` (una vez cada una), `7e832` (4).

**Siguiente paso**: (0) por qué la cámara se queda en 4360 en ares; (1) el jefe `02267E` con la captura T (`scratchpad/boss4.txt`): ya hay memoria (≈ 4 ranuras
sobre el suelo de 208); (2) fin de misión; (3) rótulo en la base; (4) `2058E`, `07E832`, `289A80`.

## Fase 2e (11-XI, cuarta sesión): el jefe `02267E` entero en lógica

**Familia `m4boss` (`src/neo_m4boss.c`, captura T del TAS): 227 nodos, 11 403 actualizaciones iguales a MAME en
lazo cerrado, 26151..27890 (desde que nace el jefe hasta el fin de la sección), 9526 dibujos, 206 borrados.**
Contiene la antigua `m4bdeb` (borrada). Extracción: `extract_family_fixture.py T/boss-trace.txt --range 2267e-25ec0
--attacks --ram 1010a4-105564 --ram 1063d8-1063da --ram 10c72e-10d83e --ram 10e94e-10e9dc --ram 10c300-10c308 --ram
10000e-100010 --ram 10c2f6-10c2f8`; prueba `--root-words --attacks --m4boss --ram-creators --terrain 10c72e --ring
10c72e --group-map 0`.

- **Cuerpo `02267E`**: música 2C (`059F4E`), `024BDA` (la capa `10D7B6` pasa a ser su dibujo: velocidades
  `694E/6952` a 0, `0C6D04`, `0C702A`, `0C703C`, colocada en `+28/+2C` del cuerpo, dueño 3 por `0C7BD0`, `+82`, el
  mapa `02225E` con la lista de columnas `0223A8` por `0C6F34`, sus 20 × 13 bloques `0C73CE`, `0C7072`, `0C741A`,
  `0C6CFC`; el cuerpo sobre la capa en (0xA8, −0xC0)), las diez piezas (reserva 0xE) y la elección de turno
  `022872` (`+8A` por la vida `+4E` frente a `+70`/`+6E` y las piezas perdidas `+8F`, `024A30`; `01B04A`; esperas
  y ráfagas de rango `2C26E0`/`2C2712`/`2C2776`/`2C27D4`; pieza al azar `0029F2(7)` que esté a la derecha de 0x20;
  `+8C` por la x). Muerte `022B34`: `018F92`, RAM `10E9B8` = 0xE0, sonido 1026, listas `E040E..E055C`, `+61`, `+8D`
  bit 2, `018F40`.
- **Piezas**: cañones `023526` (0..2) y `02391E` (3..6, la 3 sobre el cuadro `+9A` del cuerpo con su caja `E0098`):
  espera, carga `023EB4`, apuntado (`024AEC`, `024B1C`), boca `023FDE`, disparo (`024D06`, programa `E05E6`) con
  sus llamadas `024DBE`/`024E26`/`024E9A` (proyectil `024F0E`: `03B332` en rango `2C28E2` actualizaciones,
  paletas 357..359, estela `02505C` por `025132`, estalla con `E0364`, en el suelo o al caer el cuerpo); el daño
  pasa al cuerpo (`024C94`); retirada `0240EE` y destrucción `024164` (`01C71E`). El gran cañón `024226` (7)
  (`024CCA`: guarda su vida), ráfagas `2C29DC..2C2A40`, el apuntado `024B62` (umbrales `024BB6`), la bola
  `025362` (`02525A`: velocidad `2C2A72..2C2AD6`, ángulo `025508`, estelas `025512`, rueda `0255F4`/`025728` como
  plataforma `0140C4` con el mapa `E006A`), destello `02530E`, casquillo `0257DC` (`025786`), cajas `0258C4`. La
  pieza 8 `0246D0` (`+60`/`+61` del cuerpo; con su `+8C` a 0, `0247F2` mira su propio `+81`: A0 es aún el
  nodo + 0x20 que deja `00427A`, traza T 1027D5). Las orugas `022CB6` (9): mueven la capa (RAM `10E94E/10E952`) y
  la parchean (`0251D6`: secuencias `0223DA..02266A` de 16 bytes: submapa, espera `+94`, cuadro `+9A`, velocidad de
  `002802`, D0..D5 de `0C714E`), polvo `E04F0/E0502` cada 0x21, las órdenes 7 y 8 (`2349EE`, `234A7A`, `234AD8`,
  `234B2C`, destellos `025B4E`/`025BC2`/`025CBE`) y la caída final (`023432`: `0C8022`, la capa lanzada).
- Motor: `neo_layerobj_*` (las rutinas de capa de `neo_m3mid.c`, compartidas), `neo_m4boss_env_t` (la capa
  `10D7B6`, velocidades, dueño, `10E9B8`, `draw`/`patch`/`clear`/`anim_speed`). Prueba de familia `--m4boss`.
  Tablas de ROM (grupo `m4`): `E0076..E0817`, `02225E..02267D`, `2C2662..2C2B07` y las tablas de código
  `024BB6`, `024E0E`, `024E82`, `024EF6`, `0252C6`, `025508`, `0257B8`, `0258BA`, `0258D8`; 107 programas (grupo
  `m4`).
- **Memoria**: el módulo añade 23,6 KB de programa; se liberaron con `scene_image_t` de 12 bytes (antes 14: ancho y
  formato en bytes, `pack_scene.py` lo comprueba): la imagen queda 1808 bytes **por debajo** de la de antes del jefe
  (2 243 496 frente a 2 245 304 bytes) y las tablas de imágenes de cada misión en sus ranuras reservadas bajan
  también un 14 %.
- **`0BB028` no es el jefe**: es `019388` con 0x8000 (velocidades `+72/+76` del actor de misión, RAM
  `1013D6/1013DA`). El jefe lo crea el guion.

**Falta (en este orden)**: (1) el mundo no da todavía la capa al jefe: sin `e->m4boss` el constructor se para de
forma explícita antes de la música (`world unsupported routine=2267e`); hace falta el objeto de capa `10D7B6` en
bytes (como `m3layer` de la M3) coherente con `neo_layer_t` de la cámara, `neo_layers_step` con el dueño 3
(velocidades `694E/6952` por `0C6F9E`) y la prueba de cámara con esos 1458 estados; (2) el dibujo: los bloques de
`02225E` y de los submapas (teselas de `332468`) exportados como el `export_m3_overlay.py` de la M3 y validados contra
los volcados de VRAM de T; los sprites de todas las piezas (no están en la exportación de la M4); (3) `0BB028`
(trivial: velocidades 0x8000); (4) el fin de misión tras `018F40` (la M4 cierra con `017488`); (5) el rótulo de
recogida en la base. Sin captura: `024828` (escombro con `25EBC`), `0255F8..0256D2` (código muerto: `0255F4` salta
a `0256D4`).

**Sesión del 12-XI (agente A)**: dedicada a B69 (daño de los vehículos, `BUG_MATRIX.md`); el jefe en juego
(capa `10D7B6`, mapa `02225E`, sprites, fin de misión) sigue exactamente en la lista «Falta» de arriba, sin cambios.

## Jefe en juego (12-XI, agente A, segunda sesión)

**Hecho (lógica)**: el mundo entrega la capa al jefe y la mueve; el jefe ya no se para (`world unsupported
routine=2267e` desaparece).

- `neo_world.c`: la primera vez que corre una rutina de `neo_m4boss_routine` se crea el objeto de capa `10D7B6` en bytes
  (`w->m4layer[0x88]`) con la posición +0/+4, factores +26/+28 y restos +2A/+2B del objeto de cámara (`neo_layers_*`),
  primer sprite +2C = 0x20 y +82 = 0F (RAM de `run4` 157200/157500, antes del jefe); el entorno `neo_m4boss_env_t`
  lleva la capa, el byte 10E957 (`camera.layer_flags[1]`), 694E/6952 (`w->m4speed`), 10E9B8 (`script.w69b8`) y los
  dibujos `0C714E` (`g_m4_draw` / `g_m4_patch` / `g_m4_clear`) y `002802` (`g_m4_anim` → `neo_lspc_set` en
  `gameplay.c`). El padre `10D7B6` de `neo_world_parent` es esa capa mientras exista.
- Paso de cámara `0C8240` (`neo_layerobj_step`, `src/neo_train.c`): bit 7 del byte → `01AC3C` con su +8/+C; si no,
  694E (bit 0) o 6970, 6952 (bit 1) o 6974, `0C6F9E`, `01AC3C`, `01ABCC` contra la raíz y +18 += 69CC. Corre en
  `neo_world_camera` entre `neo_layers_step` y `neo_camera_finish` (antes del decaimiento `0C804A`), con la raíz de esa
  actualización y la época que la cabeza de lista `003B3E` (`01AB50`) ya cambió. **Prueba** `tools/test_m4layer.c`
  (`make check`, fixture `data/m4layer_fixture.json.gz` de la traza T por `tools/extract_m4layer_fixture.py`, una fila
  por actualización del cuerpo `102EE4`): lazo cerrado de 832 actualizaciones, +14 igual en las 704 que leen capa y
  raíz, +18 en 677; las 27 desde el `0C8022` de las orugas (`023432`, actualización 499) difieren en ±1..3 px = el
  69CC, que la traza no tiene (no se comparan; la sacudida es de `test_m4_camera.c`).
- `0BB028` (op 0x2C de la lista en 0x15B0): `019388` con D0 = 0x8000 → +72/+76 del actor de misión
  (`camera.speed_x/y`), ya no se cuenta como no portada.
- Terreno: en la M4 (lista `0E741A`) la etiqueta de terreno es `10C72E` (la regla `≥ 1023 → 10D7B6` es de la M1):
  ningún nodo apunta a `10D7B6` antes del jefe (RAM de `run4` 157200, 157500) y durante él solo el cuerpo y su
  pieza 3 (158400).
- Fin de misión: la M4 es una etapa (registro `017462`, después `017488` tipo 4 → 0), así que su `018F92` (`022B34` del
  cuerpo) acaba la misión como en la M3 (`gameplay.c`: `flow.mission == 3`); `01891E` pasa a la M5 por el camino de
  siempre (`actor.c`, `enter_mission`).
- Memoria: el módulo cuesta ≈ 2,4 KB; se liberan 3,7 KB dejando las referencias de los 936 programas bajo demanda en
  8 bytes (`soldier_program_refs`: etiqueta y desplazamiento de 24 bits, número de órdenes de 16;
  `compile_soldier_ai_tables.py` comprueba los rangos): `mslugx-explorer.elf` texto + datos + bss 2 250 472 B frente a
  2 251 784 antes de esta sesión (−1,3 KB).

**Medido para el dibujo (pendiente de implementar)**: en los volcados de `run4` del jefe (157620, 157800, 158400,
160200, 161400) el sprite de la columna n de la capa es +2C + 1 + n; su SCB3 = (+86 << 7) + palabra n de la tabla
de columnas +42 (byte alto = fila de inicio × 8, bajo = altura) y SCB4 = +84 << 7 + 0x800 n (`0C75EE`; altura 0 si
X ∉ [−8, 312)); la fila de mapa sr de la columna está en la casilla t = ((byte alto >> 3) + +83 + sr − +34 − 1) & 31
del SCB1 (`0C714E`: 260 de 260 bloques iguales en los cinco volcados, mapas `02225E` y `022274`); y en la captura
de pantalla de MAME la casilla t se ve en y = (512 − Y − 16 + 16 t) mod 512, x = X (SCB3/SCB4): 4186 píxeles iguales y
582 distintos (los tapados por sprites) en 158400, 4255/513 en 160200; con ±1 px, menos de la mitad. Las teselas de
los 15 mapas `02225E..02239E` están en `332468..335D47` (banco P2: desplazamiento − 0x100000), paletas 0x70 y 0x71
(iguales en los 148 volcados del jefe); 2240 de 2868 bloques tienen autoanimación de 8 imágenes, así que como
fotogramas nativos (`export_m3_overlay.py`) serían 14 566 imágenes de 16 × 16: **no caben** en las tablas de la
misión. Camino previsto: celdas del paquete de la M4 (atlas CI4 con `animation_mask`, como el mapa de la etapa) en
una página x propia para la capa `10D7B6` (como `LOAD_PAGE` de la M6), una por mapa (todas las escrituras del jefe
son mapas enteros de 20 × 13), y en `actor.c` el desplazamiento y la subida de esa capa desde +84/+86 y el mapa de
cada columna (`w->m4col_map/m4col_top`); hay que fijar antes la relación entre «fila de pantalla» de las capas del
paquete y la y de la captura (las fórmulas de `actor.c` dan 8 px de diferencia para la capa 2 en la M4 antes del
jefe).

**Puertas** (`work-m4` `d2d06db`): `make check` RC 0 (288 PASS, con `m4layer` y `program_file` de 8 bytes);
`regression_ares.py` con el cartucho completo (62 324 736 B) y un `m1end` recién hecho: todo PASS (logo, pausa,
personajes, M4, M5 ×3, M6 ×3, `veh`, M1 → M2), sin recuentos nuevos (M6 etapa 1: `unsupported` 3, la línea base 4);
`MSX CACHE slots=208 (floor 208)` en la M4.

**Falta (en este orden)**: (1) el dibujo de la capa (arriba); (2) los sprites de las piezas del jefe en la
exportación de la M4 (107 programas del grupo `m4`; validación contra VRAM de `run4` 157590..); (3) prueba de ares
que llegue al jefe y lo derrote (el mando de `test_mission4_ares.py` no pasa de la cámara 4360: «Siguiente paso
(0)» de la fase 2d) y sumarla a `regression_ares.py`; (4) el rótulo de recogida en la base; (5) del B69: el SV-001 de
forma fija en la prueba `veh` y el Slugnoid en ares (sin cambios en esta sesión).

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
   `033864`), `036C34`/`036EA8`, `0A9F0C` (+ `0AA37C`, `0AA2C6`). **Siguiente**: el soldado del
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
