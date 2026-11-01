# Misión 2: fase 1 (flujo de misión, datos de escena, paquete, auditoría)

Rama `work-m2` (desde `ccr-09eb2f6b-8rwl8l` + `work-fullgame`). Misma regla de
siempre: lo traducido tiene evidencia de MAME; lo no recuperado se detiene y se
cuenta. ROM, capturas y todo lo derivado de ellas (mapas, rejillas, paquetes)
siguen fuera de git; herramientas y fixtures pequeños sí están.

## Herramientas de `work-fullgame`

Fusión por unión. Conflictos en `tools/neo_scene.py` y
`tools/extract_mission_map.py`: se conserva la base del primer plano leída de
`layers.csv` (escombros de la Misión 1) y se añade `--ram-layers` (rangos de
ranuras SCB de los objetos de capa, `+0x2C`) con el filtro de bancos de C-ROM.
Comprobado: `extract_mission_map.py` fusionado sobre `captures-m1rest/run8`
(5100..13740) da un `map.json` **idéntico byte a byte** al de la herramienta
anterior (el `filesystem/mission1/rest/map.json` guardado lleva además las
celdas añadidas con `merge_observed_cells.py`).

## Flujo de misión (`src/neo_mission.c`)

Traducción del actor de misión `101364` entre etapas. Palabras de RAM con
A5 = `0x108000`: `42F4` misión, `42F6` etapa, `42F8/42FA` misión e índice de
registro de `01824A`, `4303` fin de etapa (`018F40` la borra), `4314..` bits de
etapas superadas (`018234`).

| ROM | Pieza |
| --- | --- |
| `01824A` | tabla `017D44` (un puntero por misión): registro de índice `42FA` entre los de tipo 0 (el primero si la lista acaba, tipo 8) |
| `0182B6` | tipo 0 etapa; tipo 4 salto a `+4` (`0` = fin de misión); tipo 8 fin de misión `018846` |
| `0182E8` | tipo de entrada `+4`: 0 negro inmediato (`9C0A`, d3 = 0); 4 actor `3AED8` y espera de `+60`; 8 fundido de 16 pasos (`01839A`); C actor `3B2E4` |
| `0183D0` | carga de etapa: `4303` = FF (`018098`), `42F6` = descriptor `+E`, guion `019E04` (`+8`, tabla `0x000800`), mapa/cámara `0C8774` (`+0`), jugadores `019604` (`+4`), música `+C`; `+7C` = 1 y `0184B4` en la misma actualización |
| `0184BC` | tipo de salida `+6`: 0 fundido inmediato; 4 actor `3AEF0` y espera; 8 fundido de 16 pasos (`01851A`); C `01857C` + actor `3B2A2` |
| `01860A` | MISSION n START `0607DE` si `42FA` = 0 (ya nativo, `neo_end.c`), bucle de etapa `0186A0` |
| `018722`, `0182C8` | fin de etapa (`193FE` con `4303` ≥ 0): bit de `42F6`; `4303` = 0 → registro siguiente (`42FA`+1), si no `42FA` = `4303` |
| `01891E` | `42F4`+1 < 6 → `42F8` = misión, `42FA` = 0, `01829E`; si no, fin de partida `01893E` (parada) |

Misión 2 = registros `017212` (descriptor `016DDE`: mapa `0B6D2A`, guion
`0x804` = sección 1 `2D7298`, música `22`, etapa 1), `017234` (`016DEE`:
`0B7172`, `0x808`, etapa 2), `017256` (`016DFE`: `0B754E`, `0x80C`, música
`23`, etapa 3) y `017274` (tipo 4, `+4` = 0: fin de misión). Entrada/salida:
etapa 1 = 0/8 (negro y fundido de 16 pasos, como la Misión 1), etapas 2 y 3 =
4/4 (actores de transición `3AED8`/`3AEF0`, **parados**: el port sigue en la
actualización siguiente, como el recuento). La introducción de la Misión 2 es
la de la Misión 1 sin paracaídas: letras MISSION 2 START (`0607DE` con
misión 1, nativo) y Marco entra andando por la izquierda (capturas 19440..19560).

Evidencia: `tools/extract_mission_flow_fixture.py` (captura `run4` de
`tools/capture_fullgame.lua`) → `data/mission_flow_fixture.json`: los 59
cambios de rutina del actor de misión de la Misión 1 a la 6 y las palabras de
los 48 volcados de RAM entre dos cambios. `tools/test_neo_mission.c` (en
`make check`): **59 cambios, 48 muestras de RAM iguales** (`42F4/42F6/42F8/
42FA/4303/+6E/4314..`), 11 fines de etapa, 5 comienzos de misión, 12 paradas
(`3AED8`/`3AEF0`). Las esperas de `3AED8`/`3AEF0` duran 37/35 actualizaciones
en MAME; no se reproducen hasta portar esos actores.

En el juego: `gameplay.c` lleva el flujo desde el comienzo (registro
`01714E`); `018F40` + `018722` al empezar el fin de misión y `01891E` al
acabar el fundido. `actor.c` registra la etapa elegida (misión 2, registro
`017212`, descriptor `016DDE`, mapa `0B6D2A`, guion `0x804`) como **parada
explícita** y vuelve a la Misión 1, porque la escena aún no se carga.
`test_gameplay` lo comprueba tras el fin de misión.

## Escena de la Misión 2 (privada, `filesystem/mission2/s1..s3`)

Capturas `captures-fullgame/run4` (secciones del guion: 1 en 19391, 2 en
19932, 3 en 26140, Misión 3 en 117966) y `run4-debug` (inventario).

```sh
C=/home/user/captures-fullgame/run4
python3 tools/extract_mission_map.py $C --ram-layers --allow-backdrop-timeline --frames 19391:19931 --output filesystem/mission2/s1
python3 tools/extract_region_collision.py $C --rom mslugx.zip --descriptor-layers --layer-camera --frames 19391:19931 --output filesystem/mission2/s1/collision.json
# s2: 19932:26139, s3: 26140:117965
python3 tools/inventory_mission_rest.py mslugx.zip /home/user/captures-fullgame/run4-debug --all-sections --frames 19364:108833 --output data/mission2_inventory.json
python3 tools/pack_mission.py 2 filesystem/mission2/s1 filesystem/mission2/s2 filesystem/mission2/s3 --report build/mission2_pack_report.json
```

`--layer-camera` es obligatorio en la Misión 2: la palabra de progreso
`10E99E` vale 0 en todo el recorrido y sin él la rejilla solo cubría 40
columnas.

| Etapa | Mapa | Variantes | Fondo | Rejilla | Desconocidas |
| --- | --- | --- | --- | --- | --- |
| 1 (esfinge, río) | 1030 celdas, 640 px, cámara 0..320 | 550 | 2 valores | 80 × 32 | **0** (104 mutables) |
| 2 (fábrica) | 3946 celdas, 2728 px, sube ~530 px | 26 | 1 | 341 × 99 | 20440 |
| 3 (templo, torre) | 12929 celdas, 944 px de ancho, ~5000 px de alto | 624 | 2 | 118 × 689 | 44840 |

Las etapas 2 y 3 se desplazan en vertical (capa Y hasta −5005): sus rejillas
tienen celdas nunca observadas y **no se empaquetan** (una celda desconocida no
puede leerse como vacía). Las 550 variantes de la etapa 1 son escrituras
dinámicas del mapa (aún sin identificar) que necesitan sus eventos nativos.

## Paquete por misión (`tools/pack_mission.py`)

Un fichero autodescrito por misión (`MSXM`): tablas de etapas, celdas,
imágenes, paletas y bloques, rejillas, y las páginas CI4 en bloques de 16 KiB
(8 páginas, RLE como `pack_scene.py`). El runtime cargará las tablas de **una
etapa** a la vez y las páginas pasarán por la caché de escena, como los
actores de la Misión 1. Desde la fase 2 se escribe en `romfs/mission2.pack`
(privado) y el juego lo carga en el cambio de misión (versión 2 del formato,
ver "Fase 2").

Misión 2: 691 páginas, 87 bloques, 36 paletas, 10890 imágenes, 17905 celdas;
1,36 MiB descodificados, **1,80 MiB** el fichero (0,46 MiB de tablas).

## Presupuesto

`tools/estimate_fullgame_budget.py` (reglas de paginado de `pack_scene.py`,
margen de 48 px, 12 bloques fijos):

| | Mapa empaquetado | Peor conjunto | 4 MiB (32) | 8 MiB (192) | Actores (CI4) |
| --- | --- | --- | --- | --- | --- |
| Misión 1 (resto) | 0,37 MiB | 26 bloques | cabe | cabe | 1,47 MiB |
| Misión 2 etapa 1 | 0,27 MiB | 30 | cabe | cabe | |
| Misión 2 etapa 2 | 0,36 MiB | 30 | cabe | cabe | |
| Misión 2 etapa 3 | 0,90 MiB | 49 | **no** | cabe | |
| Misión 2 total | 1,54 MiB | 49 | | | 2,93 MiB |

ROM: hoy 30,3 MiB (`mslugx-reference.z64`, con `scene.pack` de 6,7 MiB).
Misión 2 añade ≈ 1,8 MiB de mapa + ≈ 3 MiB de actores en CI4 ≈ 5 MiB; a ese
ritmo seis misiones quedan en ≈ 55 MiB, por debajo de 64 MiB. RAM: el mapa
se transmite por la caché de 8 MiB (peor conjunto 49 de ≈ 200 ranuras); las
tablas de una etapa ocupan como mucho 0,30 MiB (etapa 3: 12929 celdas × 24
bytes), que conviene reducir (celdas sin animación con una sola imagen)
antes de cargarlas.

## Auditoría del guion

`tools/audit_script_events.py --script data/mission2_script.json` (en `make
check`): 473 registros de aparición, 41 constructores, 7 nativos (`039590`,
`08C37C`, `099862`, `099DCC`, `099E06`, `09A67E`, `09A692`). **La etapa 1 no
pierde ninguna aparición contada.** Apariciones contadas que el port pierde:
194 (etapa 2: `0613E6` ×56, `0613D8` ×31, `0678CA` ×21, `0613CA` ×11, `0692B2`
×7, `067902` ×4, `06851A` ×3, `03A892`, `06835C`; etapa 3: `0613E6` ×14,
`0613CA` ×9, `03670E` ×8, `0613D8` ×8, `0678B6` ×6, `0678CA` ×6, `0692B2` ×2,
`08C154` ×2, `026254`, `052A5C`, `0678A2`, `08C168`). Sin contar: `01913C
021A1C 021A26 032DE2 035B66 035C68 035D12 03882A 03A05A 03A2AA 0529C0 0613CA
0613E6 0678CA 082BCE 08C154 08C168 09692C 0995EC 099DE2 099DF0 0B829C 0B833A`.

## Fase 2: motor (cargador por misión, etapas, cámara vertical)

Rama `work-m2eng`. La Misión 1 sigue compilada como antes (`scene_meta.h`,
`neo_grid`, `mission_camera.h`): `make check` y las familias no cambian.
Las demás misiones se cargan de `romfs/missionN.pack` en el cambio de
misión que elige `01891E`; sin el paquete el juego para explícitamente y
vuelve a la Misión 1, como antes.

**Paquete versión 2** (`tools/pack_mission.py`, `--rom` = el set de MAME):
registro de etapa de 128 bytes con, además de lo anterior, el color de
fondo, el rango de imágenes de la etapa, la máscara de celdas desconocidas
de la rejilla (un bit por celda), tres bloques de ROM (lista de cámara del
descriptor `+0` hasta su `op 8`; sección de guion de `+8` por la tabla
`0x000800`; registros de jugador de `+4` hasta el umbral `FFFF`) y las reglas
de capa. Las reglas se ajustan a las observaciones y la herramienta para si
no encajan (±8 px, el retraso de las instantáneas):

| Etapa | Capa 0 | Capa 1 | Capa 2 | Capa 3 |
| --- | --- | --- | --- | --- |
| 1 | x 1, visible | x 1/2, cámara ≥ 4 | x 1/2, cámara ≤ 3 | x 1 |
| 2 | — | — | x 1, y 1, cámara ≤ 1026 | x 1, y 1 |
| 3 | x 1/2, y 1/2 | — | — | x 1, y 1 |

(y = subida de la cámara: `256 − y` del objeto raíz `10E968`, comprobado en
los volcados de RAM de `run4`: la capa 3 sube 536 px en la etapa 2.) Los
umbrales de visibilidad son el punto medio entre observaciones (la capa 2 de
la etapa 2 se apaga entre las cámaras 1008 y 1044): aproximación hasta
encontrar el evento que la apaga. Las rejillas con celdas nunca observadas
se empaquetan con su máscara: una celda desconocida se lee vacía y se cuenta
(`neo_collision_unknown_reads`, parada explícita de cobertura).

**Cargador** (`scene_assets.c`, `scene_mission_load`/`scene_stage_load`):
las tablas de una etapa se leen en ranuras contiguas de la caché de escena
sacadas del LRU (`reserved`), así el presupuesto de RAM no cambia (la caché
sigue teniendo sus ranuras, ≥ 208); los bloques de la misión son los ids
`SCENE_CHUNK_COUNT..` (páginas `SCENE_CHUNK_COUNT·8 + n`, paletas
`SCENE_PALETTE_COUNT + n`) y pasan por la misma caché; el mapa de la etapa
se fija y se carga con la pantalla en negro. Medido en ares:

| Etapa | Tablas | Ranuras | Mapa | Carga |
| --- | --- | --- | --- | --- |
| 1 | 55 KB | 4 | 12 bloques | 125 ms |
| 2 | 224 KB | 15 | 24 bloques | 272 ms |
| 3 | ≈ 640 KB (estimado) | ≈ 40 | 49 | — |

Al volver a la Misión 1 (fin de partida, R) se liberan las ranuras y el mapa
de la Misión 1 se fija otra vez. El índice de recorte del renderizador
(`scene_cull`) se construye en la memoria de la etapa
(`scene_cell_index_build_into`).

**Mundo y juego**: `neo_world_init_stage` pone el guion de la sección (`0x804`
→ `2D7298`, etc.) y la lista de cámara completa desde su primer comando
(`0C8774` con `69D0` = 0: el punto de `op 34` da `69B8/69BA`); el anillo del
RNG, las armas, la puntuación y los rescates pasan de una etapa a otra.
`msx_game_begin_stage` conserva el HUD (reinicia el tiempo a 60) y el flujo,
coloca a Marco en el suelo más alto del borde izquierdo (captura `run4`:
y 64, 184 y 48 en las tres etapas) y lo hace entrar andando hasta la x del
registro de `019604` (`+2` = `0x68`). Fin de etapa: `msx_game_stage_end`
(`018F40` + `018722`) y una actualización del flujo por actualización del
juego hasta su `0183D0`, donde `actor.c` carga la etapa siguiente.

**Cámara vertical**: la cámara nativa ya llevaba x/y (modos 0..7, raíz
`10E968`); falta en la Misión 1 solo el `op 4` (`0C84BE`), que ahora espera a
que cada jugador presente esté (`+3C/+40`) dentro del rectángulo relativo a
`+14/+18` de la capa `10C72E` (= palabras de la raíz, volcados de `run4`). El
renderizador aplica las reglas de capa en x e y; la colisión admite rejillas
de cualquier altura (`neo_collision_set_grid`), y el anillo de `neo_kernel`
lee la celda directamente cuando la rejilla tiene más de 32 filas (la
ventana vertical del anillo no está modelada).

**Gancho de referencia** (solo `mslugx-m2-reference.z64`, nunca una versión):
`MSX_REFERENCE_START_MISSION=2` salta al primer registro de la Misión 2 en
el primer fotograma y `MSX_REFERENCE_STAGE_EXIT` termina la etapa cuando la
cámara llega al final de su lista y el jugador al borde derecho, salvo en la
etapa 1: allí la puerta nativa `03889C` crea `0B6ED2` y el fin lo da su
`018F40`.

**Recorrido** (`tools/test_mission2_ares.py`, ares, cartucho
`mslugx-m2-reference.z64`): la etapa 1 carga en 125 ms, Marco entra andando,
la cámara sigue su lista hasta 320 (progreso 320, modo 0) sin leer ninguna
celda desconocida; el gancho termina la etapa y el flujo (`017234`, salida 8,
entrada 4) carga la etapa 2, que se dibuja con sus reglas y avanza hasta la
espera `op 4` de la cámara 752 (rectángulo x 928..1247, y 160..295). Ahí
para: el jugador del port no tiene el contacto lateral con paredes (`+15`
bits 3/4, que `014594` pone a cero la velocidad), cruza el bloque de x 944
por debajo y cae al foso de x 992, así que nunca sube al escalón que pide el
`op 4`; la subida vertical (modos 4/5) y la etapa 3 no se alcanzan en ares.
Rendimiento en la etapa 2: peor fotograma 5,5 ms de CPU, 0 fallos síncronos.
Pendiente de esta parte: el contacto lateral del jugador, la ventana vertical del anillo de
colisión, el evento real de visibilidad de capas, las 550 variantes
dinámicas de la etapa 1 (zonas negras: el agua del río), la caída al vacío
(sin muerte por caída en el port) y reducir las tablas de la etapa 3.

## Pendiente (fase 2, resto)

1. Cargador por misión en `scene_assets.c`: tablas de la etapa desde
   `missionN.pack`, páginas por la caché; reglas de capas por etapa en lugar
   de `mission_camera.h` (Misión 1).
2. `neo_collision` con rejilla y columnas por etapa (la etapa 1 ya está
   completa); `neo_world_init` con guion (sección 1 `2D7298..2D7516`, blob de
   ROM), lista de cámara `0B6D2A` (espera de progreso en `0B6DF2`) y
   jugadores `019604` (`+4` = `2D5C6A`).
3. Salida de la etapa 1: el actor de mapa `0B6ED2`/`0B6F24` (rectángulo
   `01B2CE` → `018F40`, caminata automática `0CA340`) es nativo
   (`neo_goarrow.c`, familia `stageexit`) y lo crea la puerta nativa
   `03889C` (`neo_gate.c`, familia `gate`, con los parches `0C70FC`): la
   etapa 1 termina sin gancho. Hechos en
   `work-m2act` ([MISION2_ACTORES.md](MISION2_ACTORES.md)): transiciones
   `3AED8`/`3AEF0` nativas (`neo_wipe.c`, sin paradas en `test_neo_mission`),
   flecha GO `05287C` y marcador `0B6F6A` (`neo_goarrow.c`).
4. Etapas 2 y 3: cámara/render verticales. Colisión: etapa 2 completa desde
   la ROM (`fill_collision_rom.py`, 0 desconocidas); etapa 3 completa hasta
   la fila 207, quedan 37037 celdas fuera de la vista de la subida.
5. Recorrido de la Misión 2 en ares (`tools/test_mission_native_ares.py` con
   la etapa 1) cuando exista el cargador.

## Fase 2b: contacto lateral, caída, anillo vertical, torre (rama `work-m2eng2`)

**Contacto lateral del jugador** (`gameplay.c`, `wall_stop`): los barridos
`0143EE`/`014594` llaman a `013968`, cuya respuesta de superficie
`0131D6`/`0131EC` (D6 bit 2) anula d2 y `014492` lo guarda en `+30`. El
jugador del port conserva su modelo de suelo y toma del núcleo nativo solo
esa parada (A0 = terreno, 10C008 = FF, 10C009 = FF en el suelo, 10C00D = FF):
en el suelo la dirección pasa a 0 y x queda en la del barrido; en el aire
`+30` = 0. La Misión 1 no cambia (`make check`, `test_gameplay`, recorrido).
En ares la etapa 2 ya no atraviesa el bloque de x 944: el `op 4` de la
cámara 752 se cumple y la cámara llega a 1567 (y 283, modos 0/4/5).

**Muerte por caída** (`0C3712`, proceso común `0C32F4`): `+40` (y proyectada
de los pies) < RAM 10C72C (0x100, puesto por la vida nueva `0C0E42`: el
borde inferior de la pantalla) → `0C2D6A` (`+5B` = x proyectada ≥ 0xA0) y la
muerte genérica `0C2DDA` con `+5A` = 0. No depende de la invulnerabilidad
del cartucho de referencia (solo golpes). Contador `pit_deaths`.

**Anillo vertical** (`neo_collision_ring`, sobre el gancho `ring` de
`neo_kscene_t` de `work-m2en`): 64×64 celdas en la memoria de la etapa
(4 KB más en sus ranuras, no en el presupuesto fijo). `0C73CE`/`0C74C4`:
`+36` = (−64 − x de la capa) >> 4 y `+38` = (0x80 − y de la capa) >> 4, 32
bloques de 2×2 desde cada uno; `0C72EC` pone la celda (c, fila r desde
abajo) en `(c & 63)·64 + 63 − (r & 63)` (la fila superior es `2·+34+1`, la
última de la rejilla: etapa 3 `+34` = 135, 272 filas). Las filas fuera del
mapa no se escriben (`0C714E` recorta) y conservan lo que había. Los sellos
`0C88B0` escriben también en el anillo. En las etapas verticales
`neo_collision_cell` lee el anillo (el jugador, los soldados y el núcleo ven
lo mismo); la Misión 1 (32 filas) sigue con el modelo de columnas. Mapa de
grupos de celdas 10C00E: op 0x50 de la lista de cámara (`0B7000` en la
Misión 2: grupo 6 → 0x0A; Misión 1 sin cambio).

**Torre de la etapa 3** (escritor encontrado con un grifo de escritura en
MAME sobre todo el anillo, `captures-m2g/t2`, 26140..117960: aparte del
cargador `0C72EC` nadie escribe el anillo). Las "plataformas desde la fila
208" no eran filas observadas sino el anillo de una capa que sube hasta
y −5005 con `+34` = 135: el mapa de colisión de la etapa 3 es la rejilla de
la ROM, 118 × 272 sin desconocidas (`fill_collision_rom.py --rom-rows`), y
las plataformas las copia el actor `0B7C38` (creado por `0B7C20`, llamada
de la lista de cámara `0B7996`; contador 10C6B2 a 0 por `0B7B00`): mientras
(10E992 >> 4) + 0x13 − 0x66 − 10C6B2 ≥ 0, un tipo de pieza de `0261A4`
(secuencia en 10C350: 2C1374 primero; luego una de cuatro por `0029C6` & 3,
tabla `027E8E` con 10C354 = 0 o `027E7E`; banco 1 de P2) va a `0B7B1C`
(tipo ≥ 0xD → 0xB): 24 × 3 bloques del mapa `0B740A` en (0x10, 3·tipo) a
los bloques de ventana (0x10, 0x66 + 10C6B2) por `0C70FC` (recorte a la
ventana) y 10C6B2 += 3. Comprobado con el grifo: las filas del anillo
escritas en la subida son piezas cada 6 filas. Nativo en `tower_step`
(`gameplay.c`) y `neo_collision_ring_copy`. **Pendiente**: las escrituras
de 10C354 de otros actores (−1 y 2 en `run4`) no se traducen, y los
gráficos de la torre siguen siendo los grabados de `run4` (las piezas
gráficas las copia la misma `0C714E` al mapa de sprites, que el
renderizador no reproduce): con otra secuencia aleatoria la colisión y el
dibujo no coinciden.

**Tablas de la etapa 3**: con la rejilla de 272 filas, 12929 celdas, 6654
imágenes y el anillo, ≈ 34 ranuras (antes ≈ 40); la etapa 2 rellenada desde
la ROM (0 desconocidas). El presupuesto fijo de RAM de `ccr` quedaba por
debajo del mínimo (3307 KB < 3328 KB, el juego no arrancaba): `first_camera`
de `scene_assets_begin` usa ahora el búfer de staging libre (−4 KB de BSS).

**Umbrales de capas**: la capa 2 de la etapa 2 (objeto 10D7B6) no se apaga:
en el volcado 22620 (cámara 1048) su velocidad `+8`/`+C` pasa de 0 a
−4,7 px (la capa sale de la pantalla); 22590 (cámara 1008) aún quieta. El
autor de esa velocidad no se ha encontrado (las escrituras de 10D7B6..C5
son todas de la actualización de capas `0C6FC4`/`01AC44`): el umbral 1026
sigue siendo aproximado.

**Recorrido en ares** (`mslugx-m2-reference.z64`): etapa 1 completa; etapa
2 hasta la cámara 864..867 (y 64..67, modo 5), donde el RSP/RDP se colgaba.
Causa y corrección en la fase 2c.

## Fase 2c: cuelgue de la etapa 2, tabla de spawn, salida de la etapa 1 (rama `work-m2eng3`)

**Cuelgue** (20 `MSX rspq kick` y el diagnóstico de `display_get`): no era
el renderizador vertical sino los bloques grabados del fondo
(`scene_renderer.c`). Tras ejecutar un bloque, el RSP sigue escribiendo los
comandos RDP siguientes en el último búfer estático del bloque; los bloques
retirados se liberaban en cuanto su fotograma terminaba y el RSP escribía en
montón libre. En la etapa 2 el fondo se grababa y retiraba cada dos
fotogramas (el juego actualiza uno de cada dos), así que la corrupción
llegaba siempre (en la cámara 864 o 1138 según la ejecución). Bisección:
sin bloques la etapa 2 se completa y la etapa 3 sube hasta la cámara 2408
(y 536); con un detector (`mallinfo` en cada grabación) el montón aparece
corrupto ya en la etapa 1. Corrección y medidas en
[RENDIMIENTO.md](RENDIMIENTO.md) ("Bloques del fondo y montón"): bloque
permanente `bg_tail` tras cada repetición, grabación tras 3 fotogramas
iguales y con ≥ 192 KB libres. Con la grabación antigua más el detector la
etapa 2 llega a la cámara 1598 sin corrupción.

**Presupuesto de RAM**: tras la fusión de `work-m2eng2` el arranque volvía a
fallar (3295 KB < 3328 KB): margen del montón 256 → 192 KB (209 ranuras).

**Tabla de spawn**: `mission1_spawn_table` (ROM `0197DC`) solo tenía las
entradas de la Misión 1, así que el guion de la etapa 1 paraba en
`2D736A` (la puerta `03882A`, desplazamiento 0x240) y la etapa no terminaba
sin gancho. `compile_soldier_ai_tables.py` añade los desplazamientos del
guion decodificado de la Misión 2 (`data/mission2_script.json`; el bit 31
sale de `group_counted` para los grupos < 9, el único caso en que se lee).
Un registro contado en su grupo (< 9) de un constructor que el puerto no
crea para el guion (`neo_script_env_t.native` = `neo_world_spawn_native`):
su grupo nunca se vaciaría y las esperas lo retendrían. Primera parada: la
etapa 2 en `2D7878` (`06835C`, grupo 0, cámara 616).

**Salida de la etapa 1**: con 10C00B = FF (`0B6EF4`) los barridos
`0143AC`/`014574` no aplican el recuadro de pantalla, así que la caminata
`0CA3EC` hacia la x de pantalla 0x2A0 sale por la derecha y entra en el
rectángulo de `0B6F24`; el jugador del puerto se recortaba al borde
(x 611) y la etapa no terminaba. `clamp_feet` no recorta con 10C00B puesto.
`msx_game_begin_stage` conserva de `neo_ga_flags_t` solo los rescates
(manejadores, destino de la caminata, 10C00B y peticiones de fin eran de la
etapa anterior).

**Gancho de referencia**: con el guion parado en un actor no portado, su
bloqueo de cámara (`2D7864`, 616) retendría la etapa: el cartucho de
referencia lo suelta (`0C7C12`, contador `stage_lock_releases` en la línea
de fin de etapa) para recorrer el motor; los actores del guion faltan.

**Recorrido en ares**: etapa 1 completa sin gancho (puerta nativa,
caminata, `018F40`); etapa 2 con el guion hasta `2D7878` y la cámara hasta
1585..1651 (y 285..298, modos 0/4/5) sin cuelgue en tres ejecuciones; ahí el jugador cae en la
subida (tres muertes por caída, fin de partida, vuelve a la Misión 1). La
etapa 3 (subida de la torre, escritor `0B7C38`) solo se ha recorrido con el
guion de la etapa 2 desactivado (diagnóstico, cámara 2408, y 536).
