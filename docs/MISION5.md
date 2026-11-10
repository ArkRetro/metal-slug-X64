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

## Pendiente (fase 2, por orden)

1. **Actores nuevos**: 103 constructores sin portar (102 propios), 2583
   creaciones ([MISION5_ACTORES.md](MISION5_ACTORES.md)); 27 de los 49
   constructores del guion; 141 registros del guion que `run4` no ejecutó
   necesitan captura propia. Primer paso: los registros donde se para el
   guion (`2DCB42` → `0BBE5E`, `2DDB5E` → `0BC1E6`, `2DE9D0` → `08346E`).
2. **Estampadores**: `0C88B0` (2 sellos en el metro, 2 en la alcantarilla) y
   `0C8A3C` (tres escrituras de 126 celdas sobre las paredes `1F`; su actor
   no está capturado: la sonda estaba solo en `0C88B0`; hay que poner otra en
   `0C8A3C`). En ares Marco cruza hoy las paredes `1F`: hay que desensamblar
   qué consulta del núcleo las trata como pared (o si lo que para al jugador
   es el cuerpo del actor) antes de tocar nada; no se ha ajustado.
3. **Trenes del metro** (familia `051344`, 12 constructores) y **el jefe**
   `0A0C00` (22 constructores, cámara 2560, música `0x2C` por `059F76`):
   estados `at_277000` (antes) y `at_280000..291500`.
4. **Vehículos**: SV-001 `092A32` en la calle (registro `2DCE10`, cámara 280)
   y Slugnoid `09692C` ×3 en la alcantarilla (`2DECEA`…, cámara 557): los dos
   ya son nativos; falta llegar a sus registros (el guion se para antes).
5. Llamada `0BC550` de la cabeza de la lista de la alcantarilla (hoy evento
   sin consumidor) y las paletas que cambien durante el jefe.
6. **Efectos**: las 17 palabras sin política y la voz `1135`.
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
