# Misión 5: fase 1 (captura, mapa, colisión, cámara, paquete, inventario)

Rama `work-m5` (desde `ccr-09eb2f6b-8rwl8l`), árbol `/home/user/ws-m5`. Mismo
método que la [Misión 4](MISION4.md): nada inventado, nada ajustado; lo que no
está portado se para y se cuenta. ROM, capturas y derivados siguen fuera de
git: privados en `/home/user/ws-logo/filesystem/mission5/` y
`romfs/mission5.pack`. Inventario de actores:
[MISION5_ACTORES.md](MISION5_ACTORES.md).

## Flujo: tres registros de etapa

`data/mission_flow_fixture.json` (`run4`): la Misión 5 (42F4 = 4) son **tres
registros de etapa** y el fin de misión `01759A`:

| Etapa (42F6) | Registro | Descriptor | Lista de cámara | Jugadores `+4` | Guion (tabla `0x800`) | Música `+C` | Entrada (`+0E`) |
| --- | --- | --- | --- | --- | --- | --- | --- |
| 6, la calle | `01752C` | `016E2E` | `0BB8E6` | `2D604A` | `0x818` → `2DCB3C..2DDB00` (189 sentencias) | `0x26` | `2D602C`: clase 2 (`0C15B4`, el descenso en paracaídas, como la M3) |
| 7, el metro | `017550` | `016E3E` | `0BB9BA` | `2D6088` | `0x81C` → `2DDB02..2DE832` (186) | `0x2A` | `2D6036`: clase 3 (entrada andando, x 0x68) |
| 8, la alcantarilla | `017574` | `016E4E` | `0BBA24` | `2D60F6` | `0x820` → `2DE834..2DF676` (167) | `0x2B` | `2D6040`: clase 3 |

En MAME (`run4`): carga de la etapa 6 en 247661..247668, bucle `0186A0` en
247716, fin de etapa `018F92` (actor `0BBE22`) en 252722; etapa 7: carga en
252827, bucle en 252903, fin en 274317; etapa 8: carga en 274387, bucle en
274462, **orden del jefe** en 277086 (cámara 2560: `0A0C00`, que crea el
`059F76` de la música), fin de misión `018870` en 292010/292011. En el TAS
(`docs/TAS.md`): secciones 6, 7 y 8 en las actualizaciones 12887, 13810 y
16484 (fotogramas 27901, 30005, 35614), fin en U 18101 (39492).

**`run4` ya contiene la Misión 5 entera** (y la 6): no hizo falta alargar la
partida. El flujo nativo (`neo_mission.c`, comprobado contra `run4` en
`test_mission_flow`) ya elige la M5 tras la M4 y `actor.c` carga
`romfs/mission<N>.pack` por número (`enter_mission` / `load_stage`, índice
42FA): **el enlace desde el final de la M4 está hecho y no necesita código
nuevo**; hoy no se puede recorrer porque el jefe de la M4 (`0BB028`) y su fin
de misión todavía no están portados (fase 2 de la M4). Menú de pruebas:
`MISSION 5 STAGE 1/2/3` (`main.c`; `tools/test_mission4_ares.py` sube ahora
cuatro veces para llegar a `MISSION 4`).

## Capturas (MAME 0.264)

Dos fuentes independientes, las dos al 100 % en las pruebas:

1. **Réplica de `run4`** desde sus estados de sección (`states4/sec06_247661`,
   `sec07_252827`, `sec08_274387`, copiados a `/home/user/captures-m5/states`):
   `tools/capture_m5.sh 6|7|8 OUT`, una ejecución por etapa con todo a la vez.
   `tools/compare_m5_replay.py` compara las 36 columnas de `trace.csv` con
   `run4` fotograma a fotograma: **0 diferencias** en 247663..252826,
   252829..274386 y 274389..292680 (también en la ejecución aparte del
   *latch*). Para que la réplica pulse lo mismo que `run4`, el Lua repone los
   dos contadores del piloto que el estado no guarda (`MSX_FULL_LOAD_SECTION`:
   el inicio de sección para el arma de referencia a +300 y la cuenta de
   «bloqueado»). `run4` y sus réplicas llevan las **escrituras de referencia
   del piloto** de `capture_fullgame.lua` (supervivencia, vidas, munición y
   arma del jugador); no escriben nivel, guion, enemigos ni colisión.
2. **El TAS por actualización** (`tools/capture_m5_tas.sh`, `tas_updates.lua`
   con grifos nuevos de solo lectura `MSX_TAS_CAMERA` / `MSX_TAS_CELLS`):
   **ninguna escritura en la memoria**, solo los campos de entrada; 0
   desincronizaciones contra `updates251.txt` en los tres tramos. Sirve de
   comprobación sin ninguna escritura de RAM para el mapa y la cámara.

| Captura | Guion | Contenido | Queda en `/home/user/captures-m5/` |
| --- | --- | --- | --- |
| estados | `MSX_FULL_SAVE_AT` | `sec06/07/08` y `at_249000`, `250500`, `252000` (calle), `256000`, `260000`, `264000`, `268000`, `272000` (metro), `277000` (86 fotogramas antes del jefe), `280000`, `283000`, `286000`, `289000`, `291500` (jefe y final) | `states/mslugx/` |
| actores | `capture_m5.sh` (sondas de `capture_m4_actors.sh`) | `0040BC`/`004126`, guion (op 7, esperas), programas, sonidos `0120F8`, premios, puntero de la lista de cámara, fin | `s6`, `s7`, `s8/debug.log` → `data/mission5_actors.json` |
| mapa | `MSX_FULL_CELL_TAP`, `MSX_FULL_GRID_ALL=1` | cada celda que lee `0C79FE` del anillo `10C72E +88` y **todas** las escrituras en el anillo con su PC; sellos `0C88B0`; ningún parche `0C70FC` en toda la misión | `s*/cells.txt`, `events.csv` → `data/m5_map_fixture.json.gz` |
| cámara | `MSX_FULL_CAMERA=1` | por actualización: entradas de `018D96/01903A`, bloque `10E94E..10E9DB`, objetos de capa `+0..+2B` | `s*/camera.csv` → `data/m5s<k>_camera_fixture.json.gz` |
| sonido | `MSX_M5_LATCH=1` | peticiones `0120F8` y **cada byte escrito en el *latch* `320000`** | `latch6/7/8/debug.log` |
| TAS | `capture_m5_tas.sh` | cámara desde los estados de sección (`tas6/7/8`); celdas y anillo desde el estado anterior al borrado del anillo (`tasmap6/7/8`: `u12420`, `u13740`, `u16380`) | → `data/m5tas_map_fixture.json.gz`, `m5tas<k>_camera_fixture.json.gz` |
| VRAM, paletas, RAM, capas | `run4` (cada 30 fotogramas) | 247661..292680 | `/home/user/captures-fullgame/run4` |

Las trazas en bruto (`trace`, `layers`, `actorstate`, `platforms`, bancos) se
borraron tras comprobar la réplica y convertir (57 MB en total).

## Listas de cámara (`decode_map_commands.py`)

| Lista | Órdenes |
| --- | --- |
| `0BB8E6` (calle) | gráficos `0BB598`, base (0, 0), paletas `0BB636`, punto 0x78/0x148, autoanimación 1; mapas `10C72E` ← `0BB5DE` (columna 0, fila alta 0x10) y `10D7B6` ← `0BB5F4` (0, 0xF); factores `10C72E` 0x100/0x100, **`10D7B6` 0xC0/0**; límites (0, 0, 0, 0x10); progreso 0x4C0: límites 0; 0x520: punto 0x78/0x168, modo 7; 0x578: modo 0; 0x668: retención (límite 1640) |
| `0BB9BA` (metro) | gráficos `0BB5BA`, paletas `0BB636`, punto 0x80/0x180, autoanimación 1; `10C72E` ← `0BB60A` (0, 0xF), 0x100/0x100; límites 0; retención desde la primera actualización (límite 1608) |
| `0BBA24` (alcantarilla) | gráficos `0BB5CC`, llamada `0BC550`, paletas `0BB7BE`, punto 0x78/0x1A8, autoanimación 4; `10C72E` ← `0BB620` (0, 0x10), 0x100/0x100; límites 0; progreso 0x858: modo 4; 0x898: punto 0x78/0x1C0, modo 0; 0xA00: límites (−80, −32, 0, 0) y retención (límite 2560) |

Ninguna lista carga mapas a mitad de etapa ni llama a creadores de actores
(la única llamada, `0BC550`, es de la cabeza de la alcantarilla).

Mapas (descriptor: teselas, colisión, columnas × filas de bloques de 16 px):
`0BB5DE` (`345174`, `450CEE`, 123 × 23), `0BB5F4` (`347DA8`, sin colisión,
32 × 16), `0BB60A` (`3485A8`, `453922`, 121 × 16), `0BB620` (`34A3E8`,
`455762`, 183 × 18).

Orden de dibujo (RAM de `run4`, constante en cada etapa; filas nuevas de
`draw_windows`): etapa 6: `10D7B6` 0x00, ventana B `10B95C` 0x20, `10C72E`
0x24, ventana A `10B954` 0x44; etapas 7 y 8: `10C72E` 0x00, ventana A 0x20,
sin ventana B (`FFFF`).

## Mapa y colisión desde la ROM (`tools/export_stage_rom.py`)

```sh
sh tools/build_mission5.sh          # observado (una vez), ROM, paquete
```

Nada sale de un fotograma: los mapas son los de la op 0x18 de cada lista,
leídos enteros de la ROM. La captura solo sirve para comprobar la colocación
y para los colores.

| Etapa | Rejilla (celdas de 8 px) | Origen de filas | Celdas no vacías | Contra la rejilla observada | Bloques de ROM en el paquete | Contra los bloques observados |
| --- | --- | --- | --- | --- | --- | --- |
| calle | 246 × 46 | −12 | 303 | 8623 iguales, 0 distintas | `0BB5DE` 2829, `0BB5F4` 512 × 4 | 1921/1921 y 1610/1610 (910 en columnas repetidas) |
| metro | 242 × 32 | 0 | 274 | 7712 iguales, 0 distintas | `0BB60A` 1936 | 1680/1680 |
| alcantarilla | 366 × 36 | −2 | 950 | 11 371 iguales, 126 distintas (las del sello de 274388/9, vistas ya escritas), 210 de eventos | `0BB620` 3294 | 2609/2609 |

- **`0BB5F4` (fondo de la calle, capa `10D7B6`)**: 32 columnas cargadas solo
  con la op 0x18 (sin el cuadro `0C73CE`). Sus 32 columnas de sprites tienen
  el mapa entero y `0C714E` recorta toda carga posterior, así que el dibujo
  **se repite cada 512 px** (lo confirman los 910 bloques observados en
  «columnas de anillo»). El modelo lo repite hasta el último x que la capa
  enseña: límite final de la lista (op 8: 1640) × factor x (op 0x1C: 0xC0) /
  256 + 352 → 4 copias. Nada ajustado: límite y factor salen de la lista.
- **Colores**: los de los bloques observados iguales, por (mapa, número de
  paleta): ninguno con más de un juego de colores. Los bloques `40000` con
  paleta 0 (el relleno: 16 en el metro, 320 en la alcantarilla) toman la
  paleta 0 de la RAM de paletas de `run4` (`palette262620.bin`,
  `palette278100.bin`), comprobando antes que todos los números vistos
  coinciden con esa RAM.
- **Verjas de la alcantarilla**: las tres paredes de celdas `1F` (columnas
  99.., 163.., 227.. de la rejilla) **están en el mapa de la ROM**; en MAME el
  estampador `0C8A3C` escribe 0 en ellas (126 celdas en 275049, en 275434 y en
  275893; en la foto de `run4` 274980 hay una verja en la primera). El
  paquete lleva el estado de la ROM (anterior). En el recorrido de ares
  Marco las atraviesa: ver «Pendiente» 2.

## Paquete `romfs/mission5.pack`

`python3 tools/build_packs.py 5` (receta en `RECIPES`: `s1`, `s2`, `s3`,
`--rom-layers`; versión 5, trozos LZ4): 3 etapas, 272 páginas, 34 trozos, 65
paletas, 3854 imágenes, 10 107 celdas (4877 + 1936 + 3294), **531 968
bytes**; sin sección de actores todavía. En ares: etapa 1 tablas 140 KB en 9
ranuras, 15 trozos, carga en 0,19 s; etapa 2 73 KB, 6 trozos, 0,08 s; etapa 3
127 KB, 17 trozos, 0,18 s.

Reglas de capa (`rom_native_rules`, sin ajuste): `10C72E` 1/1 en las tres
etapas; `10D7B6` de la calle (factores 0xC0/0) → regla «nunca» (visible
1..0): el runtime la dibuja en el `+14/+18` de su objeto.

## Motor (cambios, mínimos)

- `actor.c`: la capa de `10D7B6` con regla «nunca» y mapa se dibuja en
  `−(+14)` y `256 − (+18)` de su objeto (`neo_layers_*`), como ya hacía la
  capa 0 con `10E83E` desde la M4; solo para paquetes de la misión ≥ 5.
- `gameplay.c`: tres filas de `draw_windows` (orden de ventanas y capas).
- `game_audio.c`: `0x26`, `0x2A`, `0x2B` en `music_name`.
- `main.c`: `MISSION 5 STAGE 1/2/3` en el menú de pruebas.
- `compile_soldier_ai_tables.py`: la tabla de apariciones `0197DC` incluye
  los desplazamientos del guion de la M5. El desplazamiento `0x2FC`
  (Slugnoid `09692C`) solo se había visto en un registro de grupo 9 (M2), que
  no lee el bit 31; el primer registro de grupo < 9 (la alcantarilla, grupo 6)
  le da su bit (ROM `80 09 69 2C`).
- `data/region_rom.json`: los tres registros de entrada del jugador
  `2D602C..2D6049` (30 bytes, lector `0183D0`/`019604`), para que
  `load_stage` lea la clase de entrada de cada etapa como en la M2 y la M3
  (la calle: clase 2, el descenso `0C15B4` desde x 0x68, y 0x100). Sin ellos
  `ga_rom` no encontraba el registro y la etapa empezaba andando. (El de la
  M4, `2D5F54`, también clase 2, no está en la imagen: es de su fase 2.)
- Sin cambios en `neo_camera.c`, `neo_train.c` ni `neo_collision.c`.

## Pruebas (`make check`)

| Prueba | Resultado |
| --- | --- |
| `test_m5_map` (réplica de `run4`) | calle: 46 127 lecturas del núcleo, **4540 de 4540 celdas distintas iguales** en `neo_collision_cell` sobre la rejilla del paquete; metro: 129 585 lecturas, **6060 de 6060** (195 por el anillo, 457 en celdas de sellos); alcantarilla: 68 346 lecturas, **7057 de 7057** (7 por el anillo, 2356 en celdas de sellos): 0 sin explicar. Registros de etapa: descriptores, listas, reglas |
| `test_m5_map` (TAS, sin escrituras) | **3264 de 3264**, **5114 de 5114** y **5730 de 5730** celdas iguales (20 504, 46 798 y 32 848 lecturas) |
| `test_m5_camera` × 3 (réplica de `run4`) | **1794**, **10 172** y **8394** actualizaciones de cámara iguales en lazo cerrado (247715..252754, 252902..274314, 274461..292008: raíz, progreso, límites, vista, órdenes) y 7176, 40 688 y 33 576 estados de capa iguales (x, y, factores, restos) |
| `test_m5_camera` × 3 (TAS) | **870**, **2602** y **1641** actualizaciones iguales (27948..29926, 30074..35535, 35683..39630) y 3480, 10 408 y 6564 estados de capa |

`tools/extract_m5_map_fixture.py` explica cada lectura con **la historia del
propio anillo** (todas sus escrituras con su PC): carga de columna
`0C72EC` con la celda de ROM de esa columna (estática); la misma con la celda
de una columna a 64 k (el anillo: un actor lejos de la vista); borrado
`0C7038` donde ningún mapa tiene celda; estampadores `0C88B0` (`0C8982`) y
`0C8A3C` (`0C8AA2`); lecturas anteriores al borrado de la etapa (anillo de la
etapa previa). Las estáticas y las vacías se comprueban todas.

## Recorrido en ares (`tools/test_mission5_ares.py`, cartucho normal)

`mslugx-explorer.z64`, menú de pruebas → `MISSION 5 STAGE n` (`Xvfb :75`
propio):

```sh
DISPLAY=:75 python3 tools/test_mission5_ares.py mslugx-explorer.z64 --stage 1 --data <aresdata> --out OUT --seconds 60 --period 4 --jump 2.5
```

`RESULT: PASS` en las tres etapas: `MSX MSELECT mission=5 stage=n`, carga con
el descriptor y la lista del flujo (`016E2E`/`0BB8E6`, `016E3E`/`0BB9BA`,
`016E4E`/`0BBA24`), y Marco (derecha + salto, sin disparar) recorre la etapa
entera: cámara 0 → 1640 con la subida final 0 → −88 px **como MAME**
(calle), 0 → 1608 (metro), 0 → 2560 (alcantarilla); 0 caídas, 0 órdenes de
cámara sin soporte, 0 celdas desconocidas. Fotos contra `run4` a la misma
cámara: la calle con sus edificios y el fondo de ciudad `0BB5F4` en su sitio
(cámara 386, 704, 1281, 1640), el túnel del metro (449, 1024, 1608) y la
alcantarilla (515, 1346, 1921, 2560): el mismo mapa y los mismos fondos.

Paradas contadas (explícitas, a portar en la fase 2): el guion se detiene en
su primer registro sin portar de cada etapa: `MSX script stopped at 2dcb42`
(`0BBE5E`, cámara 0), `2ddb5e` (`0BC1E6`) y `2de9d0` (`08346E`); dibujos sin
descriptor `last=1C3F06` y `1875FA..1876D6` (`palette_id=125`: el paracaídas
y la entrada). Sin el guion no hay paradas de cámara (las ponen sus órdenes),
por eso el recorrido llega al final de cada mapa.

## Puertas (10-XI)

`make -j4` (RC 0), `make check` (**202 PASS**, RC 0: los 184 de antes y 18 de
la M5), `tools/test_m1m2_ares.py` sobre un `mslugx-m1end.z64` recién hecho
(`rm -rf build/m1end`): `RESULT: PASS` (205 fotos); `test_mission5_ares.py`
sobre `mslugx-explorer.z64`, etapas 1, 2 y 3: `RESULT: PASS`, con
`MSX AUDIO stage music 0026` / `002A` / `002B` sonando y, en la calle,
`MSX stage entry 2D602C kind 2 (0C15B4): the parachute descent from (104,
256)` y las letras MISSION 5 START.

Fusionado en `ccr` (`07fbc14`) y subido (`git push`). `/home/user/ws-logo`
está en ese commit con `romfs/mission5.pack` (`build_packs.py 5`, idéntico al
de este árbol), `filesystem/mission5/` y los `music_mission5*.wav64`; su
árbol `build/` no se ha recompilado.

Durante la fase, `make check` de este árbol no depende de los datos de la M4
que el otro agente tiene a medias en `ws-logo/filesystem/mission4`
(`patches.json` con más de 16 parches, que el `pack_mission.py` de `ccr`
todavía rechaza): `filesystem/` es aquí un directorio de enlaces a
`ws-logo/filesystem/*` salvo `mission4/s1`, regenerado con
`tools/build_mission4.sh` (paquete idéntico al de `ccr`, md5 `d547e502…`).
Al fusionar la fase 2 de la M4 se vuelve al enlace único.

**Para seguir** (otra sesión): `git worktree` `/home/user/ws-m5` (rama
`work-m5`), `romfs/` propio (copias de `ws-logo/romfs` + `mission5.pack`),
`build/marco_*`, `marco-poses.stamp` y `marco-inputs.sum` copiados de
`ws-logo/build`; estados MAME en `/home/user/captures-m5/states`; Xvfb propio
(`:75` se usó aquí; `:67` y `:92` son de otros); datos de ares en una copia
de `/workspace/emulator/data146`.

## Sonido y música

*Latch* `320000` (`latch6/7/8`): etapa 6: `06`, `00 26` en 247662..247664
(no hay `03`: el reinicio es del fin de la M4); etapa 7: `00 2A` en
252828/252829; etapa 8: `00 2B` en 274388/274389, **fundido `0A 80` en
277087 y `00 2C` en 277279** (`059F76` del jefe `0A0C00`: la misma música de
jefe `0x2C` que la M4), `0A 80` en 292069 y `00 20` en 292298 (fin de
misión). La música de cada etapa es la del descriptor `+C`; el motor ya la
pedía (`game_audio_stage_music`).

**Render** (`render_sound_commands.py --labels …`, 24 000 fotogramas cada
una, a un directorio aparte, de una en una por el disco;
`pack_game_audio.py --source`; los `wav` se borraron):

| Palabra | Etiqueta | Periodo del bucle | Desde | Similitud | Empalme | Bytes (intro + bucle) |
| --- | --- | --- | --- | --- | --- | --- |
| `0026` | `music_mission5` | 47,58 s (candidatos 237,9 y 190,3 s: sus múltiplos ×5 y ×4) | 1,85 s | 0,984 | 0,988 | 131 320 + 1 284 904 |
| `002A` | `music_mission5_stage2` | 59,52 s (297,6 y 357,1 s: ×5 y ×6) | 1,34 s | 0,985 | 0,947 | 63 532 + 1 607 392 |
| `002B` | `music_mission5_stage3` | 85,93 s (257,8 y 343,7 s: ×3 y ×4) | 0,18 s | 0,996 | 0,953 | 32 212 + 2 320 480 |

Los seis `wav64` están en `ws-logo/filesystem/audio/` y sus tres entradas se
añadieron a `manifest.json` (sin regenerarlo: el otro agente también lo
usa). La del jefe (`0x2C`) es `music_mission4_boss`, de la rama de la M4.

Peticiones `0120F8`: 27 palabras / 648 peticiones (calle), 46 / 1971
(metro), 38 / 1133 (alcantarilla). Fuera de `data/sound_map.json`: calle
`108A`×2, `1135` (voz MISSION 5 START); metro `1030`×16, `1066`×6, `1071`×5,
`107C`×101, `107D`×10, `10AA`×7, `10F0`×28; alcantarilla `1051`×7, `1052`×7,
`1097`×105, `1098`×4, `10D0`×17, `10D6`×8, `10F1`×13, `1144`×24. Sin
renderizar (pendiente 6).

## Fase 2 (11-XI, agente B, rama `work-m5`)

Capturas de familia (MAME 0.264, réplica de `run4`, `MSX_FULL_FAMILY_ROOTS`, `MSX_FULL_VRAM_TRACE=1`;
guiones `captures-m5/fam/runG.sh` y `runS.sh`, que no van a git): `fam/G` (verjas, 274388..276000), `fam/D`
(goteos, ..275600), `fam/E` (decoración de la alcantarilla, ..275100), `fam/S` (calle, 247662..252800, volcados
desde 247800, después del fundido).

**Las verjas `1F` de la alcantarilla (pendiente 2).** Lo que retenía a Marco en MAME son las celdas: `1F` es
grupo 0, forma F con el bit 4, y con 10C008 = FF (el barrido del jugador) la forma F es pared (`0129EC`/`012AD4`).
El barrido nativo ya lo hacía; el fallo era otro: **Marco empezaba la alcantarilla en el techo**. La colocación
de `msx_game_begin_stage` (el suelo más alto del borde izquierdo, una búsqueda del port) lo ponía sobre la celda
`0D` del techo (y 216), por encima de las paredes. Traducido de la ROM, **entrada de clase 3** (`0C1260` →
`0C14E6`, registros `2D6036` y `2D6040`): `0196E8` pone al jugador en (x, y) del registro respecto a la raíz,
`0C14E6` en (0, +40) absoluto y `01473A` (`0128E8` + `0146EE`: 013DB2 hacia abajo 256 px, 10C008 = FF) lo baja
al suelo; una forma 13 (techo) no para una búsqueda hacia abajo. MAME (sonda en `1014EC`, 274581): (0x68, 0x80)
de la raíz, (0, 0x180) absoluto y (0, 0x30) de `10C72E`. Nativo: `msx_game_stage_entry_walk` (`gameplay.c`)
con `neo_kernel_floor`: y 48, el mismo valor; ahora Marco se para en x 791 ante la primera verja.

**Verjas `0BC296` / `0BC3A6`** (`src/neo_m5props.c`, familia **`m5gate`: 56 nodos, 2125 actualizaciones
iguales a MAME**, 5 sellos en el anillo, 10C700 comparado): la verja 0 (x 120, 3 golpes, el muro `0BBB28` por
`0C88B0` al nacer y `0BBBE4` al morir) y las tres de `0BC3A6` (x 320/520/720, 5 golpes, `0BBBE4` por
`0BC38E` → **`0C8A3C`**, nuevo: `neo_collision_stamp_cells`, sin máscara ni ventana; callback `stamp_cells`),
restos `0BBD78`/`0BBD8A` (`01DA40`), sonidos 1027/102D, **10C700 = +3C** en cada actualización (`0BC542`, el
límite de los soldados `08A654`) y 0x200 al morir (`0BC550`; también la llamada de la cabeza de la lista
`0BBA24`, ahora consumida en `neo_world.c`).

**Goteos `08346E`** (familia **`m5drip`: 79 nodos, 3732 actualizaciones**): periodo por azar `083674`, gota
`0834F4` tras el actor de guion (`00411C`, D0 = 2A), caída `0143DC`, salpicadura `281EA2` con `01AD52`.
**Decoración** `0BC4F4`/`0BC4BA` (**`m5deco`, 415 actualizaciones**), `0BBE5E`/`0BBED0` y **el fin de la calle
`0BBE16`** → `0B6ED2` (rectángulo `0BBAEC`, destino 7A8) (**`m5street`, 1439 actualizaciones**, con las
escrituras de 10C300.. del fin de etapa).

**Guion:** la op 0A (`01A272`) apila todas las alternativas; el port tenía 16 y el corredor de los goteos
`2DE83A` tiene 27 (la M6 también): ahora 32, y si alguna lista pasa de ahí sigue siendo parada explícita.

Datos: grupo **`m5`** en `region_rom.json` (tablas de rango `2D3F1E`, bloque `0BBAC8..0BBE00`, `0E4C48`,
`0BBAEC`, `0BBE00`), programas (`extract_soldier_programs.py`, bloque «# Mission 5 sewer gates») y
`pack_mission.py` `GROUPS[5]`. Sprites y paletas (`export_rest_sprites.py`, 59C..59F, AC8, 5BB, 5DC, 591, 592,
596) en `filesystem/mission5/rest-native.json`: 200 fotos, **0 píxeles distintos**; el paquete lleva 4719
descriptores de actores. La paleta 0x208 de la captura de la calle sale con otros colores que la del paquete
de escena: se deja la de la escena (solo los recursos de la M5 salen de sus capturas).

Recorrido en ares (alcantarilla, disparando): la verja 0 cae, la cámara se suelta (grupo 7 = 0), los goteos
caen y el guion llega a `2DEA8C` (`08996A`, el soldado de la alcantarilla), primera parada que queda.

### Fase 2, segunda tanda (11-XI, agente B)

Capturas de familia nuevas (réplica de `run4`, `MSX_FULL_FAMILY_ROOTS`, `MSX_FULL_VRAM_TRACE=1`, en
`/home/user/captures-m5/fam/`; `S`, `G`, `D` y `E` siguen allí): **`T`** (calle, 247662..252800, raíces `09624E`,
`050542`…, `08DCB2`, `08EB18`, `08FF7E`…, `0474B6`, `0479B8`, `047AFC`, `0AF1A8`, `047F7E`, `047A12`, `047B10`:
sirve para todas las familias de la calle que faltan), **`M`** (metro entero, 252828..274330, raíces `0BC1E6`,
`0BBF12`, `0BC192`, `0BC100`), **`W`** (alcantarilla hasta el jefe, 274388..277080, raíces `032B46/5A/6E`,
`095C98`, `095F8C`, `0836E8`, `083820`, `08996A`, `089948`, `08995A`, `08A264`: sirve para los soldados de la
alcantarilla y el aparato `095C98`, sin portar) y **`B8`** (inicio de la alcantarilla con BLOOD ON,
`MSX_FULL_BLOOD=1`, nuevo en `capture_fullgame.lua` como `capture_blood.lua`: solo para la paleta `158`).
Extracción: `extract_family_fixture.py … --attacks --ram 10000e-100010 --ram 10c72e-10d7b6 --ram 10c2f6-10c2f8
--ram 10c6c0-10c702 --ram 1063d8-1063da` (y `--ram 1014d8-1014da --ram 101588-10158a` para los coches: los `+15`
de los jugadores, por palabra).

| Familia | Rutinas | Resultado contra MAME |
| --- | --- | --- |
| `m5ledge` | `09624E` (calle; repisas invisibles: mapa de celdas `095B54[+72]` por `0140C4`, fuera de `095948` se borra) | 4 nodos, 1072 actualizaciones iguales |
| `m5car` | `050542` (coches destrozados, +6E 0..5: suspensión `050206` con los dos jugadores, mapas `050120/050138`, golpes, muerte `05065C`, salto, premios `05031E`/`050384`), ruedas `05090A/05092C`, explosión `05094E`, chapas `0509A8`, piezas `050B52` | 65 nodos, 2939 actualizaciones iguales (9 coches, 7 destruidos) |
| `m5signal` | `0BC1E6` (metro; señal encendida mientras su grupo tiene a alguien, `019DCA`; sonido 107D de su programa) | 7 nodos, 21 435 actualizaciones iguales |
| `m5door` | `0BBF12` (fin del metro: tres fases de daño, muros `0BBAF4/0BBBB0` por `0C88B0`, piezas `0BC100/0BC192`, manejadores de los jugadores `0BBF0E`, la salida `0B6ED2` como hija en (780, 28), destino 788) | 3 nodos, 221 actualizaciones iguales, 2 sellos |
| `m5stain` | `032B46/032B5A/032B6E` (alcantarilla; paleta 158 con BLOOD, 17D sin él) | 3 nodos, 606 actualizaciones iguales |
| `m5jet` | `0836E8` (chorros; **`0838DA` lee `move.w $70.w`: la palabra de ROM `000070`, no `+70`**, traducido tal cual) y sus bocanadas `083820` | 64 nodos, 2053 actualizaciones iguales |

Motor y pruebas: `player_handlers` 4 = `0BBF0E` (`sub.w d7,d7`: sin entrada, como `054B18`); prueba de familia
`--fresh-flip` (un creador que pone el bit de giro del hijo después de `01B220`: el nodo recién dado lo lleva a 0)
y cuentas de ranura dinámica que se agotan cuando la familia no tuvo ningún nodo vivo durante más de 8
fotogramas. `09624E` con el indicador de depuración `10F000` bit 7 (su dibujo `09635E`, con `009CCE`) es parada
explícita.

**Paquete** (`export_rest_sprites.py` con `T@50542-50d70`, `M@bbf12-bc296`, `W@32b46-32c6c`, `W@836de-83916`,
`G@1ce50-1ce98`, `M@1cdc0-1cf90` y `--palette-capture B8`; `build_packs.py 5`): 5106 descriptores de actores, 263
paletas (nuevas: 211, E1..E5, 37C, 113, 58F, 58E, 58D, 587, 17D, 158, 851 y **167/168**, las de las piezas
`01CE50..01CE98` de las verjas, que el recorrido de la alcantarilla dibujaba sin paleta: 85 dibujos ausentes antes,
0 ahora), **935 trozos** (819 prestados de los paquetes de las misiones 2 a 4): `SCENE_MISSION_CHUNKS_MAX` 896 →
960 (con 896 el paquete se rechazaba: `bad header`). La 869 (variante `0836DE`, que la M5 no crea) no está en
ninguna captura y no se exporta.

**Validación de píxeles sin cerrar**: 521 fotos, 1 108 784 píxeles iguales y **21 157 distintos en 28 fotos**
(captura `T` 23 de 42, `M` 1 de 447, `W` 4 de 32; las familias de la primera tanda dieron 0). Los descriptores salen
de la ROM por la misma vía que los demás; no se ha mirado todavía qué píxeles son (hipótesis sin comprobar: sprites
de fuera de la familia con el mismo número de paleta encima, p. ej. los premios y las explosiones `01C71E` en que se
convierten los coches). **Pendiente**: volcar la diferencia de una foto (`T` 247830) y explicarla.

**Recorrido en ares** (`test_mission5_ares.py`, `mslugx-explorer.z64`, `--fire --jump 2.5`): las tres etapas
`RESULT: PASS`. Calle (`--progress 100`): los coches se destrozan y sueltan sus premios; paradas contadas:
`MSX script stopped at 2dcf1a`, `world unsupported routine=22c0a6` ×28, `2cfd00` ×5, `2d3c16` ×2, `e53ba` ×1
(acciones de soldados ya nativos que la M5 usa y no están portadas) y **7 dibujos sin paleta `892`** (descriptores
`1C6086..1C671C`: los premios de clase 0x55 de los coches, `08C374`; falta exportarlos: sus nodos no cuelgan de
la familia en la captura `T`). Metro (`--progress 40`): parada en `2ddbbe`, 0 dibujos ausentes. Alcantarilla
(`--progress 100`): parada en `2dea8c`, registros `095C98` saltados (2), 0 dibujos ausentes.

Guion tras esta tanda: la calle pasa de `2DCBBA` (`050542`) y se para en **`2DCF1A` (`08DCB2`)**; el metro pasa
de `2DDB5E` y se para en **`2DDBBE` (`0474B6`, el helicóptero suelto)**; la alcantarilla se para en **`2DEA8C`
(`08996A`)**.

**Lo siguiente, por orden de guion** (desensamblado ya mirado; utilidades en el scratchpad: `m5/d474b6.txt`,
`m5/d47f7e.txt`, `m5/dheli_srv.txt`, `ex5.sh FAMILIA CAPTURA RANGOS`, `famd.sh FAMILIA`):
1. `08DCB2` + `08EB18` (calle, 1 creación, ~5,4 KB de código propio `08DCB2..08F1E0`).
2. **El helicóptero suelto `0474B6` / `047F7E`** (calle y metro, 681 creaciones con sus lanzadores `0479B8/047A12` y
   bombas `047AFC/047B10`): está en el módulo de `neo_heli.c` (comparte `0499A4`, `049A92`, `049E46/74`, `049EBE`,
   la muerte `048B06..`, el rotor `047874`, el lanzador `047AAA..` y la bomba `047B34..`); faltan los servicios
   `04900C`, `049324`, `04936C` (+ `049CB2`, `01B12E`), `049A1E`, `049B20`, `049E9E`, `049ECE`, `049F9E`, `01B04A`,
   `01AF9A`, los estados `0474B6..047872` y `047F7E..04850E`, y las llamadas `04A1AE/04A208` (hoy parada
   explícita en `heli_call`). Guarda en `+7C` el nodo del jugador objetivo.
3. `0AF1A8` (del inventario de la M4), `051344` (trenes), `0D3B36`.
4. Alcantarilla: soldados `08996A/089948/08995A` (código propio `089948..08A6xx`, hijo `08A264`), el aparato
   montable `095C98/095F8C` (servicios de vehículo `0AA606/0AA83A/0AA7FC`), `08FF7E`, el jefe `0A0C00`.

**Puertas de la segunda tanda (11-XI, `work-m5`)**: `make -j4` RC 0; `make check` RC 0 (**236 PASS**: los 230 de
la primera tanda de la fase 2 y `m5ledge`, `m5car`, `m5signal`, `m5door`, `m5stain`, `m5jet`);
`tools/test_m1m2_ares.py` sobre un `mslugx-m1end.z64` recién hecho (`rm -rf build/m1end`): `RESULT: PASS` (354
fotos); `test_mission5_ares.py` etapas 1, 2 y 3: `RESULT: PASS` (arriba). Un primer `make check` se cortó por
disco lleno (`No space left on device`, árbol compartido al 100 %); se repitió entero. Fusionado en `ccr`.
`/home/user/ws-logo/romfs` **no** se ha tocado: `mission5.pack` toma trozos de los paquetes de las misiones 2 a 4,
así que hay que rehacerlo allí (`python3 tools/build_packs.py 5`, que rehace también el de la M6) cuando el agente
de la M4 deje el suyo; `filesystem/mission5/rest-native.json` (compartido) ya lleva la exportación nueva.

### Fase 2, tercera tanda (11-XI, agente B tras el reinicio del contenedor)

**El tanque de la calle `08DCB2` y su torreta `08EB18`** (`src/neo_m5enemy.c`, nuevo; registro `2DCF1A`, cámara 408). En
`run4` (captura `T`) el jugador lo destruye en 140 fotogramas y solo se ven 12 de sus estados, así que se hizo una
captura propia: **`K`** (`fam/K`, réplica de `run4` desde `sec06_247661` con `MSX_FULL_NOFIRE=248100-249600`, raíces
`8dcb2,8eb18`, 248100..249900; guion en el scratchpad, `b/capK.sh`): el tanque espera, arranca, avanza, gira en los
bordes (30 / 110), apunta, dispara ocho proyectiles en ráfagas y muere en 249798. Familia **`m5tank`: 18 nodos, 1971
actualizaciones iguales a MAME** (tanque, torreta, fogonazos `08EFFA`, proyectiles `08F058`; 274 820 bytes, 5 contactos
con el jugador). Traducido: `08DCB2..08E19C` (todos los estados del registro, también `08DDA2` con `+95`), la torreta
`08EB18..08EF3C` con sus tres tablas de órdenes `08FEF0/08FF14/08FF38` (`jmp (a1)` en la misma actualización), el
proyectil `08F058/08F1A0`, y los servicios `08F1EE..08FBC6`. Las otras entradas del módulo (`08DCF4`, `08DD08`,
`08DD16`, `08DD24`, `08DD2C` → `08E19E..08EB16`) no las crea ningún registro de la M5 y no están traducidas (parada
explícita). `08EF46` es la comprobación del cartucho (`2FFFE0`): como en `neo_armored.c`, acaba en `004188`.

- **`0260C8` de tipo 2** (`2CED8E`, las actualizaciones antes de disparar): el byte de la fila elige un grupo de 8
  palabras y `0029C6` una de ellas. `ga_rank_word` (motor compartido) solo devuelve el byte; el tanque lo completa en
  su propio `rank` (sin tocar el motor).
- Datos: programas `28AEF0..28C32C` y `22D264` (`extract_soldier_programs.py`), `region_rom.json` (`2CECCA`, `2CED2C`,
  `2CED8E`, `2CEE00`, `2CEE1A`, `2CEE34`, `08FC0A..08FF77`, `28B77C`); paletas `22B` (tanque), `306`, `208`, `373` y
  **`892`** (los premios de clase 0x55 de los coches: sale de los volcados de `T`; los 7 dibujos sin paleta del
  recorrido ya no faltan). La `347` (variante `+89 = 1`) no la crea la M5 y no se exporta.
- En ares (calle, `--fire`): el tanque nace nativo en la cámara 409, con su dibujo y su paleta, es sólido (Marco se sube
  encima, `004E76`) y dispara. El caminante de la prueba se queda encima disparando por encima de él, no lo destruye y
  el guion espera en la cámara 600 (`2DCF6E`, condición): **la siguiente parada del guion de la calle es `2DCF7C`
  (`08FF7E`)**, todavía sin ver en el recorrido.

**Los 21 157 píxeles distintos de la validación de sprites: explicados, quedan 268 en 13 de 756 fotos.** No eran
descriptores mal exportados sino tres defectos del validador (`export_rest_sprites.py`, `validate`):

1. Un nodo borrado, o que sale del intervalo de rutinas (un coche que pasa a ser la explosión `01C71E`), dejaba de
   contar actualizaciones: «la actualización anterior» del segundo intento era para él una más vieja que para los demás.
   Ahora toda actualización de un nodo de la familia cuenta (sin entradas si borra o está fuera del intervalo).
2. El juego va más lento que la pantalla (una actualización del juego dura 2 a 4 fotogramas) y el volcado puede caer
   en medio del recorrido de la lista: unos nodos enseñan su última actualización y otros la anterior, y un nodo recién
   creado o recién convertido no tiene entradas anteriores. Tercer intento: por nodo, la de las dos que está en
   pantalla.
3. Un sprite de fuera de la familia dibujado con un número de paleta que también usa la familia (el fuego `01C9E6` y la
   explosión `01C706` de un proyectil comparten recurso) tapa el de la familia: se cuenta aparte (`pixels_covered`,
   13 590), no como diferencia.

Con las siete capturas (`K`, `T`, `M`, `W`, `G`): **1 215 989 píxeles iguales, 268 distintos** (antes 21 157), 30 966
solapados entre sprites de la familia. Los 268 (calle 248070..248130, 249450..249570, 250770/250800; alcantarilla
275220, 275670, 275700) son todos píxeles donde el volcado tiene **otro lápiz del mismo número de paleta** (ninguno
vacío): otro sprite con la misma paleta encima (en 248070, dos ruedas `37C`); no se ha identificado cuál en cada foto.

**El helicóptero suelto `0474B6`** (`neo_heli.c`, junto a la formación de la M1 con la que comparte rotor `047874`,
bomba `047B34..`, muerte `048B06..` y los servicios `0499A4`, `049A92`, `049E46/74`). Traducidos sus 16 estados
`0474B6..047872`, los servicios `049324`, `04936C` (con `049CB2` y `01B12E`: el ángulo de bombardeo `+80` de la tabla
`0E0962[+70]` y el x que lo da), `049A1E`, `049B20` (la inclinación `+96` en nueve pasos, con el acarreo de la última
comparación o suma del original), `049E9E`, `049ECE`, `049F9E`, `01B04A`, `04900C/049166`, el lanzador `0479B8`, la bomba
`047AFC` (rango `2D18F0`) y el callback `04A1AE`. Familia **`m5heli`: 46 nodos, 2037 actualizaciones iguales a MAME**
(captura `T` cortada en 250760, antes de que nazca el `047F7E`: los dos primeros helicópteros de la calle con sus
rotores, 19 lanzadores, 19 bombas y sus dos muertes, 249523..250531). **Sin cubrir por la captura** (traducido, no
probado): la marcha `04783A/047856` (`049ECE`, cuando se agota `+86`) y las ramas de `049CB2` con vecinos marcados.
Datos: `2D146C`, `0E0C32` (16 bytes, separados del bloque `m3` que empezaba ahí), programas `244792..2447BC`.

**Lo siguiente** (sin hacer en esta tanda): `08FF7E` (calle, cámara 600: `08FF7E..0913xx`, con `090A62`, `091290`,
`0912EE`); el otro helicóptero suelto `047F7E` (calle 1208 y metro: estados `047F7E..0484FE`, servicios `04940C`
(1 KB, lógica de dos jugadores) y `049862`, lanzador `047A12`, bomba `047B10`, callback `04A208`; la captura `T` solo
tiene sus estados hasta `048254`; el bombardeo `04828E..048504` solo ocurre en el metro y la captura `M` no lleva esas
raíces: hay que repetirla con `474b6,47f7e,479b8,47a12,47afc,47b10`); después `0AF1A8`, trenes `051344`, `0D3B36`,
los soldados de la alcantarilla `08996A/089948/08995A`, `095C98`, el jefe `0A0C00` y las palabras de sonido.

**Puertas de la tercera tanda (11-XI, `work-m5` con `ccr` `0cc0d53` fusionado: personajes y pausa, fase 2 de la M4)**:
`make -j4` RC 0; `make check` RC 0 (**253 PASS**, con `m5tank` y `m5heli`; lanzado con `HOST_CFLAGS=... -pipe` porque el
disco compartido no daba para los temporales del compilador); `test_mission5_ares.py` sobre `mslugx-explorer.z64`,
etapas 1, 2 y 3 (`--fire`): `RESULT: PASS`, 0 dibujos sin descriptor. Calle: el tanque nativo en la cámara 409; cuando
el caminante lo destruye el guion sigue y se para en **`2DCFA4` (`08FF7E`)**. Metro: el guion pasa de `2DDBBE`, el
helicóptero `0474B6` nace nativo en la cámara 80 y bombardea (el caminante muere 3 veces y no pasa de ahí en 80 s; la
siguiente parada del guion del metro no se ha visto). Alcantarilla: parada en `2DEA8C` (sin cambios).
`tools/test_m1m2_ares.py` no se ha repetido en esta tanda (no estaba en las puertas pedidas).

Notas para el coordinador: (1) `romfs/scene.pack` depende ahora de `data/soldier_programs.json` (sus imágenes se
empaquetan en el orden de los programas; sin esa dependencia `test_pack_compression` fallaba al añadir programas): el
primer `make` de cada árbol lo rehace, y hay que rehacer los paquetes de misión (`python3 tools/build_packs.py`) en
`ws-logo/romfs`. (2) Durante un minuto `ccr` tuvo la fusión `99c4476` hecha con el árbol de `work-m5` solo (sin los 19
commits de la M4 que acababan de entrar); `8d75f1a` restauró el árbol de `0cc0d53` y la fusión buena va encima.
(3) El disco compartido llegó al 100 % (1,4 MB libres) dos veces; el clasificador de permisos denegó borrar los CSV
de las capturas `fam/*` del agente anterior (`layers.csv`, `actorstate.csv`, `platforms.csv`, unos 80 MB) y comprimir
sus trazas: siguen ahí. La captura propia `K` se borró tras hacer la familia y la exportación (se rehace con
`b/capK.sh`: `MSX_FULL_NOFIRE=248100-249600`, raíces `8dcb2,8eb18`, desde `sec06_247661`).

### Fase 2, cuarta tanda (11-XI, agente B)

**El tanque de orugas `08FF7E` con su torreta `090A62`** (`src/neo_m5enemy.c`; calle: registros `2DCF7C..2DCFFE`, cámara
600; alcantarilla: `2DF502..2DF5A2`, cámara 2064). Traducidos todos sus estados `08FF7E..09097E`, la torreta
`090A62..09127A` con su tabla de órdenes `0924B0` (`jmp (a1)` en la misma actualización), el fogonazo `091290`, el
proyectil `0912EE` y los servicios `0913DE..091CC6`. `0907FC` (y con él la orden 7 de la torreta, `0910A6`) no lo
referencia nada en la ROM; `08FF8C` (la entrada sin `+96` bit 0) no está en ningún registro: los dos son parada
explícita (`0910A6` está traducido, sin captura). `090986` es la comprobación del cartucho, como `08EF46`.

- Va sobre el **nodo de orugas `014A4E`** del SV-001 (`014AE8`, `014C6A` en el suelo y `014C2E` en el aire): el
  servicio recibe el nodo en A0 (el SV-001 lo guarda en `+8C`, este tanque en `+7A`): `neo_tread_step_node`. La
  pendiente (`091A72`, de 0 a 0x10) elige la pose (`0919F4`: 0, 8 o 0x10; entre dos, el estado «inclinándose»
  `090698` / `090772`) y con ella el programa, la caja de daño y el cuerpo sólido de cada estado.
- `091638` lee la x del objetivo por A0 **después** de `002EB4`, que deja A0 en su tabla `002D7C` salvo cuando una
  de las dos componentes partidas por dos tiene el byte bajo a cero: lo que lee casi siempre es la palabra de ROM
  `002DB8`. Traducido tal cual (la familia sin fuego, con 18 disparos, lo comprueba).
- Sus filas de ataque `091D08..` llevan **dos pares (clase, daño)** (el jugador y los cuerpos de clase 1): `ga_attack`
  (motor compartido) recorre ahora todos los pares de la fila (`004B06`), una llamada al `attack` del mundo por par
  (`attack_pair`); antes era parada explícita «no observado».
- Capturas propias (réplica de `run4` desde `sec08_274387`, raíz `8ff7e`): **`N`** (276150..277080, los cuatro tanques
  de la alcantarilla) y **`N2`** (la misma sin fuego en 276190..278600: recorren las pendientes, se inclinan, apuntan
  y disparan 18 proyectiles). Borradas tras hacer las familias y la exportación; se rehacen con `b/capK.sh`
  (`LOADN=sec08_274387 LOADF=274387 BLOCKED=20 ROOTS=8ff7e FROM=276150 LAST=277080 NOFIRE=1-2`, y
  `LAST=278600 NOFIRE=276190-278600`).

| Familia | Captura | Resultado contra MAME |
| --- | --- | --- |
| `m5tread` | `T` (calle) | 10 nodos, 1894 actualizaciones iguales |
| `m5tread2` | `N` (alcantarilla) | 12 nodos, 1705 actualizaciones iguales |
| `m5tread3` | `N2` (alcantarilla sin fuego) | 42 nodos, 7259 actualizaciones iguales (pendientes, 18 disparos, 621 contactos) |
| `m5heli2` | `H` (metro) | 542 nodos, 27 649 actualizaciones iguales |
| `m5heli3` | `L` (metro sin fuego, 274700..274900) | 20 nodos, 705 actualizaciones iguales: la marcha `04783A` / `047856` y su borrado |

**El otro helicóptero suelto `047F7E`** (`neo_heli.c`, junto al `0474B6`): sus 22 estados `047F7E..04850E`, los
servicios `04940C` (la x junto a los jugadores que tiene debajo, con la lógica de dos jugadores) y `049862` (el lado
de la pantalla contrario), el lanzador `047A12` (programa `244E6E`), la bomba `047B10` (rango `2D1952`) y el callback
`04A208`. Captura **`H`** (réplica de `run4` desde `sec07_252827`, raíces `474b6,47f7e,479b8,47a12,47afc,47b10`, traza
263800..269500: los cuatro `047F7E` del metro y dos `0474B6` con todas sus bombas y muertes). **La marcha `04783A` /
`047856`** (pendiente 5): el único registro con estancia finita es `2DE3DA` (`+72` = 30: 960 llamadas a `049ECE`);
captura **`L`** (la misma réplica sin fuego en 267800..276500): el helicóptero `1018E4` se va en 274818 y sale por
arriba en 274848. De `L` se guarda solo el tramo 274700..274900 (familia `m5heli3`, `--resume`), sin los descendientes de los dos
helicópteros nacidos antes de la traza (`102AC4` y `104174`, nodos sin seguir: sus lanzadores no se pueden crear en
nativo). `H` y `L` borradas (se rehacen con `b/capK.sh`: `LOADN=sec07_252827 LOADF=252827 BLOCKED=20
ROOTS=474b6,47f7e,479b8,47a12,47afc,47b10 FROM=263800 LAST=269500 NOFIRE=1-2`; `L`: `FROM=267800 LAST=276500
NOFIRE=267800-276500`).

**Utilidades de las familias** (`tools/`): `extract_family_fixture.py --births` (un nodo creado dentro de una
actualización de la familia y usado en esa misma actualización, el nodo de orugas, nace con los bytes que tenía al
nacer —los que el resto de esa actualización leyó antes de escribir— y no con los de su primera actualización propia);
`test_neo_family.c`: una dirección de nodo reutilizada se resuelve a la instancia viva más reciente (y, muerta, a una
que ya haya vivido), y un campo de nodo (`--node-fields`) que la actualización no toca se compara con el campo nativo
de antes (una dirección caducada, el rotor borrado antes que su helicóptero, puede ser ya de otro nodo).

**`ga_rank_word` con tablas de tipo 2** (pendiente 5): hecho en el motor (`neo_gactor.c`): el byte de la fila elige un
grupo de 8 palabras tras las filas (12 bytes por bit de la máscara) y `0029C6 & E` una de ellas (`02614A..026184`). Lo
prueban `m5tank` y las tres `m5tread` (`2CED8E`), que ya no llevan el apaño local. Los otros módulos con su propia
copia (`neo_m3enemy.c`, `neo_m3mid.c`, `neo_m3boss.c`, `neo_m2boss.c`, `m4h_rank` de `neo_heli.c`) siguen con ella:
hacen lo mismo y son de otros agentes.

**RAM: el suelo de la caché de escena.** Con el código del tanque (15 KB) la caché se queda en 199 ranuras (3184 KB)
y `SCENE_EXPANDED_MIN_SLOTS` era 200: el cartucho paraba al arrancar (`RAM budget: 3155 KB left for the scene cache
(minimum 3203 KB)`). Dos cambios: los programas del tanque van al paquete de la misión (grupo `m5`: 85 programas,
5006 órdenes, 57 KB de datos de misión) y el suelo baja a **197**, que es lo que el propio comentario da como
necesidad de los tres juegos fijados (49 + 65 + 83). **Ya no queda margen**: el siguiente módulo de la M5 (trenes,
soldados de la alcantarilla, jefe: unos 40 KB de código) no cabe sin liberar RAM (código por misión, o tablas fuera de
la imagen). Decisión del coordinador.

**Datos de la M3 en el paquete de la M5.** El recorrido de la calle contaba `world unsupported routine=22c0a6`
(×270: el programa del disparo `01B62C` de un arma que sueltan los coches), `2cfd00`, `2d3c16` y `e53ba`: programas
y tablas del grupo `m3` (los soldados `05C53C` y sus armas, que la M5 también usa) que solo iban en los paquetes de
la M3 y la M4. `pack_mission.py`: `GROUPS[5] = ('m3', 'm5')`, como la M4.

**Prueba de ares** (`tools/test_mission5_ares.py`): `--lifebar` (la barra de vida opcional, Z en el menú de pruebas;
en ares Z es `R-Trigger: 0x1/0/60`) y `--back T` (cada T segundos el mando mira 1,5 s a la izquierda: en una parada
de cámara el tanque queda detrás del caminante y un mando que solo va a la derecha no lo toca nunca).

**Recorrido en ares** (`mslugx-explorer.z64`, `--fire --jump 2.5 --lifebar`, calle y metro con `--back`): las tres
etapas `RESULT: PASS`, caché de escena 199 ranuras (suelo 197), `heap_low` 281 KB en la calle. Calle: los dos tanques
de orugas nacen nativos en la cámara 600 (`2DCFA4`, `2DCFFE`), el caminante los destruye y llega a la **cámara 880**
(el helicóptero `0474B6` de `2DD29E` y los soldados `0AF1A8`, ya nativos por la fusión de la M4): el mando no dispara
hacia arriba y no pasa de ahí; ninguna parada del guion en el log. Queda contada una parada nueva, **`world
unsupported routine=800040` ×3** (la primera en la cámara 289, antes de los tanques: un actor del grupo `m3` que
ahora sí tiene sus datos y llega más lejos; `800040` es la dirección que pidió, sin identificar: falta el nodo en el
diagnóstico). Metro: el helicóptero de la cámara 80 retiene al caminante (0 paradas). Alcantarilla: parada en
`2DEA8C` (`08996A`) y los dos registros `095C98` saltados, sin cambios.

**El cartucho mide ya 67 043 328 bytes**, el límite a partir del cual ares no escribe log: con el próximo recurso
habrá que probar con `romfs/mission6.pack` apartado. `mission5.pack`: 1001 trozos de 1024
(`SCENE_MISSION_CHUNKS_MAX`), 186 KB de datos de misión (523 programas con el grupo `m3`).

**Lo siguiente, por orden de guion**: metro `2DDF60` (`051344`, los trenes: 12 constructores, `050D70..0528xx`, con el
mapa dinámico `0140C4` y los premios `08CDB2`) y `0D3B36`; alcantarilla `2DEA8C` (`08996A` / `089948` / `08995A`:
`089948..08A6xx`, desensamblado mirado: tres entradas de un mismo soldado —suelo, `089D92` y techo con `+15` bit 1—,
hijo `08A264`) y `095C98`; el jefe `0A0C00`. Antes hay que resolver la RAM (arriba). La calle no tiene más
constructores sin portar hasta su fin `0BBE16`; falta verla entera en ares (un mando que dispare hacia arriba).

### Fase 2, quinta tanda (11-XI, agente B tras el corte)

**El tren del metro `051344`** (`src/neo_m5train.c`, nuevo; registro `2DDF60`, cámara 528). Traducidos el tren y todos
sus hijos: las ruedas `0515CE` (programa `050EE8[etapa]`, paletas por el `+9F` del tren), los bogies `051688` /
`051690`, los paneles `051772` (`24922C[+96]`), las chispas `051DF2` → `051D70` (lista `050F40`, el tren baja 8 px
una actualización) y luego las explosiones `01C71E` de `050F48`, el humo `051C00` (lista `050F50`), las piezas
`051FF4` / `051FFE` (`0525EE`: siete, programa `+82`) y `052128`, el vaivén `0527A2` (`052840`), y las llamadas de sus
programas (`0517D0`, `0518C4`, `051A1E`, `051B20`, `0521B4`, `0521D0`, `0521F2`, `0522E4`, `0523D6`, `0524C8`,
`0524E0`, `0525C6`, `052840`, `051032`, `051082`, `0510D4`, `013F1E`). Salud `+4E` = rango `2D388E`, cinco etapas de
daño (`+8A` umbral, `+8C` = salud / 5, programas `050ECC`), empuje del impacto por el tipo de arma `+9C` (`051220`,
rangos `2D390A..2D39A6`), mapa de celdas `050D82` en (−78, 0) por `0140C4` mientras la etapa es < 5, salto final
(`0521D0`: etapa 6, `+30` = FFFEC000, `+34` = 6C000) y destrucción (`0514EA`: `009BB0`, sonido 1034, piezas,
escombros `050F62`, premio `08CDB2` en (−20, 58)). Las comprobaciones de cartucho (`10F00C` + 3EF) acaban en
`004188`, como en `neo_m5enemy.c`.

- Captura propia **`R`** (réplica de `run4` desde `sec07_252827`, raíces `51344,515ce,51688,51690,51772,527a2,52128,
  51d70,51df2,51c00,51ff4,51ffe`, traza 262380..263700, volcados cada 30): el primer tren, destruido en 263176. Familia
  **`m5train`**: 37 nodos, 2261 actualizaciones iguales a MAME (`--script-actor 101414 --script-routines
  1c706,1c6ee,8c374`). Se rehace con `b/capK.sh` (`NAME=R LOADN=sec07_252827 LOADF=252827 BLOCKED=20 ROOTS=… FROM=262380
  DUMP0=262380 LAST=263700 NOFIRE=1-2`).
- Programas (31 raíces, grupo `m5`), descriptores (`export_rest_sprites.py` con `R@51344-52856`: 163 nuevos, paletas
  10F, 110, 111, 112, 115, 116 y 1D) y tablas de ROM (`region_rom.json`: `2D388E`, `050D76`, `051126`, `24922C`).
  Validación de `R`: 156 567 píxeles iguales y **70 distintos en 2 de 27 fotos**, sin cerrar: 263040 (20 px del
  descriptor `1D04C4` del tren, donde MAME tiene el pen 3 de su paleta: otro sprite de la misma paleta encima, sin
  identificar) y 263190 (50 px de `1D23E8` en x = 0: pens desplazados, el borde izquierdo de la pantalla; probable
  defecto del validador con columnas en x < 0, sin comprobar).
- El paquete de la M5 tiene ahora **1037 trozos**: `SCENE_MISSION_CHUNKS_MAX` 1024 → 1088 (con 1024 el paquete se
  rechaza). Caché de escena en ares: 198 ranuras (3168 KB, suelo 197); `neo_m5train.o` 9,3 KB.
- ares: el cartucho mide 67 239 936 bytes (más que el límite de log): las pruebas se hicieron con
  `mission6.pack` fuera de `build/game-romfs`. El mando automático no llega a la cámara 528: el helicóptero
  `0474B6` de la cámara 80 lo mata (incluso con `--lifebar --up 4 --back 9`, opción `--up` nueva: arriba 1,5 s cada
  T s); el tren queda probado solo por la familia.

**G11 (`routine=800040`) corregido** (también en la M3): el diagnóstico de parada del mundo lleva ahora la rutina del
nodo (`soldier line=… (rutina)`) y la de su padre (`detail`): era el suelo de la balsa `08B43E` (padre `08B090`) en la
cámara 289 de la calle. `08B52A` indexa `0E53E6` con el byte `(+7E del creador × 4) & FF`; `neo_m3enemy.c` lo volvía a
multiplicar por 4 y con `+7E` = 1 tomaba la palabra `00800040` de la tabla como mapa. Calle en ares sin paradas.

### Fase 2, sexta tanda (11-XI, agente B)

**La máquina del metro `0D3B36`** (`src/neo_m5mech.c`, nuevo; registro de la cámara 992). Traducidas las dos entradas
de la tabla `0197DC` (`0D3B36` y `0D3C5A`; `0D3B4C` / `0D3C76` son del actor de `0D39xx`, fuera de la misión: parada
explícita) y sus 13 hijos: el cuerpo (marcha `0D3E48` sobre los mapas de celdas `0D559A` / `0D55FE`, giro `0D3F0A`,
la IA `0D50EC` que manda órdenes a los hijos por `+7E..+82`: `0D512E`, `0D5298`, `0D5396`; muerte `0D40A6` /
`0D4116`), los brazos `0D42A4` (órdenes `0E8FBC`, posición por el cuadro `+7D` del cuerpo, `0D5506`), el piloto
`0D447C` (órdenes `0E8FD8`, puntería `0D545C`, impacto `0D45EE`, muerte `0D470E` = `0405EA` + la muerte de soldado
`040846` de `neo_m3enemy.c`, exportada como `neo_m3enemy_death_40846`), los proyectiles `0D4762` / `0D4776`
(`2CA75E` / `2CA7BE`, `01B0F6`, ángulo `002EB4`), el fogonazo `0D498C`, las piezas `0D4A1E` / `0D4A82` / `0D4ABA` /
`0D4C26`, la granada `0D4D84`, el casco `0D4F1C`, las chispas `0D5018` y las llamadas de programa (`0D5662`,
`0D56D2`, `0D5722`, `0D5740`, `0D57D0`, `0D585C`, `0D58D4`, `0D5908`, `0D593C`, `029034`). Código muerto sin
referencias en la ROM: `0D3F00`, `0D4034`, `0D404C`, `0D4EF0`, `0D4FAA`.

- Familia **`m5mech`** (`data/m5mech_fixture.json.gz`, de la captura `Q`, que se ha borrado): **51 nodos, 6314
  actualizaciones iguales a MAME** (269327..271119). Dos detalles: las sumas del creador sobre un hijo recién creado
  (`addi.w #1E, +72`, `addi.w #10, +98`) se escriben como el valor, porque `003F24` deja a 0 `+6E..+AF` de todo nodo
  libre; y el piloto muere sobre el bloque de posición del cuerpo (`+38` = `10419C`): `01495C` hace el barrido con un
  bloque de actor como padre.
- **Motor compartido** (mínimo): `neo_kernel` acepta `NEO_KOBJ_NODE`, un bloque de posición de actor como padre de la
  consulta (sus `+14/+18/+1C/+20` = `+3C/+40/+44/+48` del actor, rellenados por `neo_gactor.c` en cada consulta);
  antes esos barridos paraban con `1495c`. Ninguna otra familia pasaba por ahí (todas siguen iguales en `make check`).
- Programas (45 raíces, grupo `m5`), descriptores (`export_rest_sprites.py` con `Q@d3b36-d5970`) y tablas de ROM
  (`region_rom.json`: `2C9D84` (rangos y filas de velocidad), `0E8CC6` (cajas, filas de ataque, rectángulos, órdenes,
  programas), `0D559A` (mapas de celdas)). Validación de `Q`: 48 fotos, **208 px distintos en 2** (270150: 114,
  270180: 94), sin cerrar: el cuerpo y los brazos con el atributo de paleta del destello `+17` (BA00 / 9900);
  probable límite del validador con el destello compartido `0044F6`, sin comprobar.

- **Paletas**: el tren (10F, 110, 111, 112, 115, 116) no tenía sus paletas en el paquete (la quinta tanda puso sus
  raíces en `ROOTS` pero no sus recursos en `RESOURCES`: en ares 224 avisos `missing ... palette_id=110`). Como `R` y
  `Q` ya no existen, las paletas salen de la RAM de paletas de `run4` en el metro (`--palette-capture` de sus fotos
  262380..271500): tren y máquina completos salvo **239** (la granada `0D4D84`, sin foto de `run4` con ella viva:
  pendiente de captura) y 25B / 25C / 25D (la variante `+7B` / `0D3C5A`, que la M5 no crea). En ares el tren se ve ya
  con sus colores (cámara 528) y ningún aviso de paleta.
- **ares, metro** (tarea 2): el mando pasa ahora el helicóptero de la cámara 80 con `--up 1.8` (arriba 1,5 s de cada
  1,8: dispara hacia arriba casi siempre) y `--continue` (nuevo: Start en el panel CONTINUE? del juego, como un
  jugador; no cambia nada del juego). `--fire --jump 2.5 --up 1.8 --continue`: cámara 688 en 420 s, el tren de la
  cámara 528 visto destruido; en 900 s llega a la **cámara 809** y para con `world unsupported routine=23b636
  (35bf8)`: los murciélagos `035B66` (registro `2DDF4C`) usan los programas `23B636..` del grupo `m2`, que no van en el
  paquete de la M5 (`GROUPS[5] = ('m3', 'm5')`). Se resuelve con `/programs.bin` del agente A (todos los programas
  fuera de los paquetes); hasta entonces la máquina de la cámara 992 no se ve en ares (solo la familia).
- **Caché de escena** (tarea 5): con `neo_m5mech.o` (15 KB de código) la caché queda en **197 ranuras = el suelo**.
  El suelo 197 es la suma de los tres juegos que se fijan (bancos y mapa 49 + jefe 65 + tres programas de soldado 83,
  comentario de `SCENE_EXPANDED_MIN_SLOTS` en `scene_assets.c`); era 200 («con algo de margen») y se bajó a 197 en la
  tercera tanda (tanque `08FF7E`). La caché se dimensiona tras cargar el audio con el montón libre que deja la imagen
  de consola (código y datos: 1,26 MB de texto, 0,96 MB de datos): lo que la achica es el **código** de los módulos de
  la M5 (tanque 15 KB, tren 9 KB, máquina 15 KB), no el paquete de la M5, cuyos trozos se leen bajo demanda en la
  caché (1066 trozos, dentro de 1088). Las tablas nuevas (`region_rom.json`, 3,8 KB) no duplican nada (comprobado
  contra los bloques existentes). Con el suelo a 208 de A hacen falta 11 ranuras (176 KB) que tiene que liberar
  `/programs.bin`; si no basta, el siguiente paso es sacar código o tablas de la imagen, no bajar el suelo.

**Puertas de la sexta tanda** (tras fusionar ccr con `/programs.bin` de A; cartucho sin `mission6.pack`): `make check`
y `make -j4` bien; ares `--fire --jump 2.5`: calle cámara 600 PASS, metro (`--up 1.8 --continue`, 420 s) cámara 688
PASS (única parada contada: los murciélagos `23b636`), alcantarilla cámara 673 PASS; `test_m1m2_ares.py` con
`mslugx-m1end.z64` recién hecho PASS. Caché de escena: **208 ranuras** en la calle y el metro, 209 en la alcantarilla
(suelo 208): `/programs.bin` devolvió las ranuras, pero el margen sigue siendo 0..1. `/programs.bin` lleva solo los
programas sin grupo; los de la M5 van en su paquete (grupo `m5`) y los murciélagos (`m2`) siguen fuera de él.

**Matriz de regresión** (`tools/regression_ares.py`, cartucho completo de 60,9 MB con los cinco paquetes rehechos tras
fusionar la compresión de C, `mslugx-m1end.z64` recién hecho; sin línea base todavía): logo PASS (con el arreglo del
propio script: `test_logo_ares.py` da su veredicto en el JSON `"status": "PASS"`, no en `RESULT:`), pausa PASS, M4
PASS, M5 ×3 PASS (0 dibujos perdidos, 0 rutinas sin portar), m1m2 PASS; M6 etapas 1 y 3 FAIL por la paleta 125 (el
fallo abierto que lleva C; la etapa 1 cuenta también 2CFD00 / 2D3A0E / 0DFF0A), M6 etapa 2 PASS con los mismos
avisos; personajes FAIL (`Timeout: MSX SELECT place=` en Eri, Tarma y Fio). **Causa**: no era el código: `ws-m5` no tenía
`romfs/chars` (los paquetes de personaje, datos privados) y el juego decía `MSX SELECT place=1 not in this cartridge`.
Arreglo: `romfs/chars` es un enlace al almacén `/home/user/ws-logo/romfs/chars` (idéntico al de `ws-chars`). Con
él, matriz completa (61 505 536 bytes): logo, pausa, **personajes** (4/4), M4, M5 ×3, M6 etapa 2 y m1m2 PASS; M6
etapas 1 (cámara 515, tres caídas, paleta 125) y 3 FAIL. La etapa 1 da lo mismo sin el cambio `NEO_KOBJ_NODE` del
núcleo (probado: misma cámara 515 y mismas cuentas), y `neo_m5mech.c` solo corre en la M5.

**Inventario**: arreglado (pendiente 1) y regenerado: **40 constructores sin portar** (54 antes de esta tanda, menos los 14 de `0D3B36`).

**Siguiente paso (alcantarilla)**: captura propia **`X`** hecha y **guardada** en `captures-m5/fam/X` (réplica de
`run4` desde `sec08_274387`, raíces `95c98,95f8c,96140,8996a,89948,8995a,8a264`, traza 274388..276400, volcados cada
30; hace falta para los descriptores de la exportación; se borra tras exportar). Fixtures ya extraídos (sin familia
en el Makefile hasta que haya código): `data/m5sewer_fixture.json.gz` (`--range 89948-8a700`: los soldados 08996A /
089948 / 08995A y su hijo 08A264, 9185 actualizaciones) y `data/m5pow_fixture.json.gz` (`--range 95c98-96200`: el
prisionero 095C98 / 095F8C con el premio 08C374, 1056). 095C98 usa el sistema de huecos `0AA606` / `0AA83A` /
`0AA7FC` / `0CA2CE` (en parte ya en `neo_vslot` / `neo_slug.c`) y `0C05EC` (sin portar). Desensamblado:
`scratchpad/m5/d95c98.txt`. Antes de ellos, por cámara: los escombros `01CDF0` (cámara 992, de la lista `E8F02` de la
máquina) y `01CDxx` (1545..1602), `0DEA44` / `0DEC14` (del jugador, cámara 1280).

## Pendiente (fase 2, por orden)

1. **Actores nuevos**: tras la sexta tanda quedan **40 constructores sin portar** (los 54 de la quinta menos los 14 de
   `0D3B36`); el inventario ([MISION5_ACTORES.md](MISION5_ACTORES.md)) se regenera con
   `tools/build_mission5_actors_doc.sh` (regenerado en la sexta tanda: 169 nativos, 13 en otro módulo, **40 sin
   portar**. Los «208 sin portar» salían al lanzar el script desde otro árbol o sin `build/host/script-native`:
   `compare_m3_events.py` tomaba entonces todo por «missing» en silencio. Ahora el script entra en su propio árbol
   (`cd "$(dirname "$0")/.."`, también el de la M6) y `compare_m3_events.py` se para si falta el ayudante o es más
   viejo que algún `src/*.c`). Paradas del guion:
   alcantarilla `2DEA8C` (`08996A`). El tren de la cámara 528 y los siguientes del metro no se han visto en ares (el
   mando automático muere ante el helicóptero de la cámara 80).
2. ~~**Estampadores** y verjas `1F`~~: hechos en la fase 2 (arriba): entrada de clase 3, `0BC296`/`0BC3A6`,
   `0C8A3C` nativo. Quedan los dos sellos `0C88B0` del metro (sus actores).
3. ~~Trenes del metro~~ (hecho en la quinta tanda) y **el jefe**
   `0A0C00` (22 constructores, cámara 2560, música `0x2C` por `059F76`):
   estados `at_277000` (antes) y `at_280000..291500`.
4. **Vehículos**: SV-001 `092A32` en la calle (registro `2DCE10`, cámara 280)
   y Slugnoid `09692C` ×3 en la alcantarilla (`2DECEA`…, cámara 557): los dos
   ya son nativos; falta llegar a sus registros (el guion se para antes).
5. Llamada `0BC550` de la cabeza de la lista de la alcantarilla (hoy evento
   sin consumidor) y las paletas que cambien durante el jefe.
6. **Efectos**: las 17 palabras sin política y la voz `1135`. Sin hacer: `docs/AUDIO_JUEGO.md` §17 pide medir en
   MAME los pares de cada palabra consigo misma, entre ellas y contra las etiquetas de la política (miles de
   renders) y añadirlas de una en una; no cabía en el disco compartido de esta sesión (< 0,7 GB libres).
7. La tabla de columnas `+3E` (pendiente 4 de la M4) no se usa en la M5
   (ninguna op 0x18 lleva tabla).

## Archivos

`tools/capture_m5.sh`, `capture_m5_tas.sh`, `compare_m5_replay.py`,
`build_mission5.sh`, `extract_m5_map_fixture.py`, `compile_m5_map_fixture.py`,
`test_m5_map.c`, `test_m5_camera.c`, `test_mission5_ares.py`,
`report_mission_actors.py`, `capture_fullgame.lua` (`MSX_FULL_LOAD_SECTION`),
`tas_updates.lua` (`MSX_TAS_CAMERA`, `MSX_TAS_CELLS`), `export_stage_rom.py`
(mapas de 32 columnas que se repiten), `pack_mission.py`, `build_packs.py`,
`compile_soldier_ai_tables.py`; `src/actor.c`, `gameplay.c`, `game_audio.c`,
`main.c`; `data/mission5_actors.json`, `m5_map_fixture.json.gz`,
`m5tas_map_fixture.json.gz`, `m5s{1,2,3}_camera_fixture.json.gz`,
`m5tas{1,2,3}_camera_fixture.json.gz`, `sound_map.json`. Privados:
`filesystem/mission5/s{1,2,3}/{map.json,map_observed.json,collision.json,collision_observed.json,rom_report.json}`,
`romfs/mission5.pack`, `filesystem/audio/music_mission5*.wav64`.
