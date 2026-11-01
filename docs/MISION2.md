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
check`): 473 registros de aparición, 41 constructores, 24 nativos. **Apariciones
contadas que el port pierde: 0**: el jefe de la etapa 3 `026254` ya es
nativo ([JEFE_MISION2.md](JEFE_MISION2.md)), como `06835C` (soldado con
rehén). Sin contar: `01913C 021A1C 021A26 032DE2 035B66 035C68
035D12 03A05A 03A2AA 0529C0 082BCE 09692C 0995EC 099DE2 099DF0 0B829C 0B833A`.

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

## Fase 2d: torre dibujada, caídas y recorrido completo (rama `work-m2eng4`)

**Gráficos de la torre** (`tools/pack_mission.py`, `scene_renderer.c`
`scene_render_set_tower`): el descriptor `0B740A` lleva en `+0` el mapa de
sprites (u16 tile, u16 atributo por bloque de 16 px, columnas de 0x88
bloques; tile | (atributo >> 4 & 0xF) << 16, paleta = atributo >> 8, giro
= bits 0..1, autoanimación de 8 fotogramas = bit 3), y `0C714E` copia sus
bloques del mismo rectángulo que la colisión. Comprobado contra `run4`:
las celdas estáticas de la capa 3 son ese mapa (bloque b en y = 16·b −
1936) y las 82 piezas grabadas de la subida son plantillas exactas: la
pieza k (10C6B2 = 3k, bloque de ventana 0x66 + 3k) tiene su fila superior
en y = −1408 − 48k, columnas 0x10..0x27 (la primera, tipo 0xB, es la de
`2C1374`). El paquete quita las 4880 celdas grabadas de la subida (capa 3,
y < −1360, x 256..639) y añade las 13 plantillas (936 celdas) en la capa 3
en x ≥ 0x4000 (nunca en pantalla). `tower_step` guarda el tipo de cada
pieza (`neo_world_t.tower.types`, 128 piezas) y el renderizador dibuja la
pieza k desde la plantilla de su tipo (también en el modo de profundidad;
la clave de los bloques grabados del fondo incluye el número de piezas):
colisión y dibujo salen del mismo tipo, con cualquier secuencia aleatoria.
Línea de registro `MSX tower piece k type t`.

**Escrituras de 10C354**: aparte de `026190`/`0261A4` (la torre) solo las
hacen `026230` (0 → −1), `026244` (borrar) y `02624C` (leer), llamadas por
el cuerpo del jefe `026254` (`027936`); el jefe nativo (`neo_m2boss.c`) las
traduce sobre `neo_world_t.tower.mode`.

**Cartucho de referencia** (nunca el de publicación):
- Caídas: con el jugador en el suelo y los pies proyectados ≥ 0x120 se
  guarda la posición; si cae bajo la pantalla (`0C3712`) vuelve a ella en
  lugar de morir (`pit_rescues`; la línea de cámara da `pit deaths` y
  `rescues`).
- Paradas de cámara del guion retenidas 2400 pasos con la cámara en ellas
  se sueltan (`lock_timeouts`): en la etapa 2 los soldados `067E98`/`068020`
  de `06835C` quedan en el borde derecho, bajo el jugador, y el mando no
  los mata.
- Registros del guion de constructores no portados contados en su grupo:
  se saltan en lugar de parar el guion (`neo_script_env_t.skip_unported`,
  `MSX reference: script record ... skipped`): la etapa 3 paraba en
  `2D8846` (`03670E`, al principio) y no llegaba al jefe (`2D9CA2`).

**Prueba** (`tools/test_mission2_ares.py`): el mando sigue el registro
(cámara quieta 5 s: fuego en el suelo sin saltar, disparo hacia abajo,
saltos frecuentes); exige el fin de las etapas 1 y 2, la carga de la etapa
3, piezas de torre y el fin de misión; informa de las piezas, caídas,
registros saltados y los números de rendimiento (PERF, RDP, audio) como la
de la Misión 1.

**Presupuesto de RAM**: con el jefe `026254` fusionado (+36 KB de código)
el arranque fallaba otra vez (3303 KB < 3328 KB): margen del montón
192 → 160 KB.

## Fase 2e: sprites de la Misión 2, corredores agrupados y mando (rama `work-m2eng5`)

**Sprites de los actores** (`tools/pack_mission.py`, versión 3 del paquete):
ningún paso leía `filesystem/mission2/rest-native.json`, así que enemigos,
cajas y jefe de la Misión 2 no se dibujaban (`MSX native draw missing`). El
paquete de misión añade una sección de actores: los descriptores que el
paquete de escena no tiene (906, con 1455 partes ≤ 64×64 en 1348 páginas
propias, a partir de un fragmento nuevo) y los 36 recursos de paleta que le
faltan; los descriptores y paletas comunes deben ser idénticos (lo comprueba
la herramienta). Cabecera: `u32 actors u32 actor_images u32 resources`;
tablas tras la de fragmentos: `actor[]` (u32 descriptor, u16 primera parte,
u16 partes), `actor_image[]` (u16 página, u16 paleta, i16 x, i16 y, u16 w,
u16 h), `resource[]` (u16 recurso, u16 paleta). `scene_stage_load` las lee
con las tablas de la etapa en las ranuras reservadas (≈ 31 KB: etapa 1 de 4
a 6 ranuras, etapa 2 de 15 a 16), `scene_native_descriptor` y
`scene_native_palette` buscan después en ellas y `scene_frame(SCENE_NATIVE,…)`
sigue tras las partes del banco. La Misión 1 no cambia (su paquete y su
`scene_meta.h` son los mismos). Paquete: 87 → 256 fragmentos
(`SCENE_MISSION_CHUNKS_MAX` 128 → 320, ≈ 2,5 KB de BSS en las tablas por
fragmento), 1,9 → 3,1 MB de ROM. En ares: 0 dibujos sin descriptor en las
tres etapas (soldados, momias, rehenes, drones y jefe dibujados).

**Corredor agrupado** (op 11, `01A210`): crea un corredor `01A422` cuyo
registro es el op 8 de `+4` (su cuerpo es la sentencia siguiente), con
`+69` = la palabra de `+2` si es < 9 (`10C326` contado en el momento) y
salta la sentencia. `neo_script.c` lo ejecuta (`group69` del corredor) y
`neo_world.c` cuenta los corredores agrupados vivos en el recuento de
grupos (`019E40`). La etapa 3 ya no para en `2D8846`: los cinco corredores
crean el dron `03670E` (nativo, `neo_drone.c`) sin el salto de registros de
la referencia; la etapa 2 usa dos (`2D8368`, `2D8418`).

**Jugador en el borde derecho** (paradas 616/752 de la etapa 2, 336 de la
etapa 3): reproducido en el anfitrión (etapas del paquete, andar a la
derecha y luego a la izquierda). `msx_game_begin_stage` buscaba el suelo
más alto del borde izquierdo con `neo_surface_at`, que ya leía el anillo de
64 filas del núcleo (cargado por `neo_world_init_stage`); el anillo se
repite cada 64 filas y la búsqueda desde arriba encontraba la copia del
suelo de salida 64k filas más arriba (etapa 2: fila 86 por 22; etapa 3:
fila 261 por 5). El jugador vivía fuera de la ventana del anillo (filas
−16..47 al empezar), sobre celdas de otra fila: suelos y paredes fantasma
(`wall_stop` lo paraba en x 624 de la etapa 3 con la izquierda pulsada; 172
celdas distintas de la rejilla alrededor del jugador en la parada 616 de la
etapa 2). La búsqueda lee ahora la rejilla (`neo_collision_ring_bypass`): y
de salida 184 y 48 en las etapas 2 y 3, los valores de `run4`, y 0 celdas
distintas alrededor del jugador en todo el recorrido del anfitrión. En la
etapa 3 la pared de x 616 (6 celdas, forma 0xF) es real: se sube saltando
a la izquierda. Su parte de arriba es suelo para el barrido `013968` (el
núcleo devuelve el aterrizaje en y 128, `D6` = 0x4B) pero no para el modelo
de suelo del puerto (formas 1..13), así que el jugador la cruzaba por la
esquina, quedaba bajo la plataforma y caía a través de la rampa (rescates
de caída). `neo_collision.c` toma la parte de arriba de una celda de forma
14/15 con la celda de encima vacía como suelo plano; la rejilla de la
Misión 1 no tiene ninguna. Con ello la cámara (modo 1: el progreso crece al
ir a la izquierda) pasa de 336 a 384 en el anfitrión. El mando de la prueba
añade una cuarta fase de 4 s andando y saltando a la izquierda.

**Estado del recorrido en ares** (`mslugx-m2-reference.z64`, `--stage-seconds
400`): etapas 1 y 2 completas sin soltar ninguna parada de cámara del guion
(`script lock releases=0`; antes 3) y con los actores dibujados; la etapa 3
carga en y 48 (la de `run4`), el guion pasa los corredores agrupados sin
parar y el mando no llega a la torre: la lista de cámara `0B754E` espera en
el `op 4` de `0B7656` (tras el progreso 0x180) a que el jugador esté en el
rectángulo x 256..319, y (desde abajo) 176..351 de la capa, es decir, en la
plataforma de la fila 23 (y 192, x 248..311) que se alcanza desde el suelo
de la fila 17 (y 144, x 328..375, escalera superior desde la plataforma de
la fila 27) saltando la pared de x 312. Ni el mando de la prueba en ares
(progreso 336) ni su simulación en el anfitrión (progreso 384, 30000
actualizaciones) suben a esa plataforma: el jefe y el fin de misión siguen
sin recorrerse. Pendiente: un mando que siga esa ruta (o un registro de
entradas de MAME de la etapa 3).

## Fase 2f: ruta de la etapa 3, torre, jefe y fin de misión (rama `work-m2eng6`)

**Ruta comprobada en el anfitrión** (simulación de la etapa 3 del paquete con
el jugador de referencia): la plataforma del `op 4` de `0B7656` (x 256..319,
y 176..351) se alcanza por la escalera de la fila 27 (x 600..631), a la que
se llega saltando a la izquierda desde el suelo de la fila 21 (x ≥ 648, y
176) por debajo de la plataforma (las celdas `B` son de un solo sentido) y
aterrizando en la rampa `5`; de la fila 17 se sube saltando la pared de x
312. Desde ahí la subida sigue a la derecha (filas 29..45), a la izquierda
(51..69), la escalera de x 600..940 (filas 79..103: el hueco de 64 px de la
fila 79 se cruza con un salto corriendo) y la torre. Ruta posible con la
colisión nativa; las correcciones que necesitó:

- `neo_collision.c`: las búsquedas de suelo (`neo_surface_at`,
  `neo_walk_step`, `neo_land_step`, `neo_surface_unique`) recorrían las 272
  filas del mapa; con el anillo recorren sus 64 filas de ventana. La torre
  sube por encima del mapa (y ≥ 2176): allí ninguna fila daba suelo y el
  jugador atravesaba las piezas (`0B7C38`) en las que estaba.
- `gameplay.c`, suelo: un empuje lateral de un sólido (soldado `0E1B4E`) en
  una pendiente movía x sin y y la marcha con velocidad 0 no encontraba el
  suelo a una unidad (x 536 de la fila 35): ahora los pies siguen la
  superficie tras el empuje.
- Salida de referencia (`MSX_REFERENCE_STAGE_EXIT`): el borde derecho con la
  cámara en su máximo (x 624) terminaba la etapa 3 en la escalera de x 904;
  la etapa 3 termina con su jefe (en `run4` el jugador pasa por x 915).
- Caídas del jugador de referencia: si la posición segura ya está bajo la
  pantalla (la cámara sube en los modos 2/4/5) muere y renace en vez de un
  bucle de rescates; el punto de `019658` (tabla de la Misión 1) puede quedar
  bajo la pantalla de otra misión (torre: y 65 con la cámara en y 2200) y
  entonces la vida nueva cae desde la parte alta de la pantalla.
- Paradas de cámara del guion (`lock_timeouts`): se comparaban con la x de la
  cámara; con la cámara nativa se comparan con el progreso de la lista
  (en la etapa 3 la parada 2376 no se soltaba nunca con la cámara en x 361).

Las paredes de forma 0xF/0x1F de la etapa 3 no frenan al jugador en el
barrido nativo (`wall_stops` = 0 en todo el recorrido; el núcleo no las
devuelve con el bit 4 de `D6`): la ruta no depende de ello, pero queda por
contrastar con MAME.

**Piloto de la prueba** (`src/msx_autopilot.h`, solo en la referencia): un
mando a ciegas no sube la torre (piezas aleatorias). Con Start en la etapa 3
(`MSX autopilot on`; ares no entrega Izquierda y Derecha a la vez) un planificador lee los
suelos del jugador (`neo_player_surface`, anillo incluido) en 80 columnas
alrededor, los enlaza con pasos y saltos (alcance del salto del puerto: 50
px de subida con ≤ 40 px de avance, 64 px de hueco a la misma altura) y
camina hasta el salto del camino hacia el objetivo de la lista de cámara: el
rectángulo del siguiente `op 4` (leído por delante de las esperas de
progreso, para no llevar la cámara de modo 1 más allá de la escalera), si
no la dirección del modo. Replanifica cada 16 pasos; bloqueado salta en
vertical; con el piloto los cuerpos de los soldados no lo empujan; con el
jefe `026254` presente pulsa Abajo en el aire (disparo hacia abajo) sin
dejar de subir (la cámara sube con el jefe). En el anfitrión: fin de misión
(`018F92`) en 10700..12200 actualizaciones en 6 variantes del disparo.

**Rutinas sin portar**: `067EE4`/`067F12` (soldado parado en un borde,
programa `268CC4`, raíz nueva de `extract_soldier_programs.py`) traducidas
de la ROM; su cuchillo de `+9C` = 4/8 (`0689D2`, `069696`) sigue sin portar
(`6899A`). `09A842` crea el prisionero `099E1C` (familia sin traducir).
`067D80` (piezas `068D32`, `+7A`) sigue siendo una parada explícita.

**Fin de misión**: `018F92` del jefe (`026C30`) pone 10C300 = FF y los
manejadores `0C0538`, pero `mission_complete` solo lo levantaba el jefe de
la Misión 1 (`mission_step`): ahora también 10C300 en la etapa 3 de la Misión 2 (las puertas
`0B6ED2` de las etapas 1 y 2 también lo ponen; el
registro `MSX mission complete signal (018F92)`); `018F40` cierra después
la etapa (en el anfitrión, 470 actualizaciones más tarde). El piloto no
planifica con los manejadores del jugador cambiados (entrada ignorada).

**1E003C**: aparece en la etapa 2 (`MSX world unsupported routine=1e003c`)
justo al terminar cada desplazamiento automático (progreso 1008, 1568,
2048): la rutina de un nodo es una dirección del espejo de la RAM de
trabajo (0x1E003C = 0x10003C + 0xE0000), no de la ROM; el actor se borra con
sus descendientes y el recorrido sigue. Su escritor no está localizado
(candidatos: el actor que posee el desplazamiento, `+5B` de 0C7C12).
Pendiente con el grifo de MAME.
