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
python3 tools/extract_mission_map.py $C --ram-layers --allow-backdrop-timeline --skip-fade --frames 19391:19931 --output filesystem/mission2/s1
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

**1E003C** (resuelto en `work-m2pol`): no era la rutina de un nodo sino la
dirección que `emit_one` (`neo_effects.c`, 01DA7C) pasaba a `ga_fail`. El
emisor de escombros `01D764` del objeto `06870A` (padre `06875A`) recibe la
lista `0E1BF2`; `ga_rom` devuelve el bloque que más bytes deja tras la
dirección y el único que la cubría era el mapa `0E1B6A` (144 bytes), que
termina 8 bytes dentro de la lista: los bytes 8..17 se leían de la memoria
de otro bloque (`0E121E`), de ahí `+0E` = 001E003C (con `+0D` ≠ 0, tabla de
rutinas). El bloque `0E1BFA` de `data/region_rom.json` empieza ahora en
`0E1BF2` (listas enteras) e incluye la tabla `0E1C16` (rutinas de escombro
`01CF10..01CF34`, ya nativas). Localizado con un registro temporal en
`ga_fail` (dirección de retorno resuelta con `addr2line`), sin MAME: la
lectura es del puerto, no de la ROM.

**Recorrido en ares** (`mslugx-m2-reference.z64`, `--stage-seconds 400`):
`RESULT: PASS` — etapa 1, etapa 2 (salida de referencia 1, parada soltada 1),
etapa 3 con el piloto (80 piezas de torre, cámara hasta y 5155, progreso
7186), jefe `026254` vencido y `MSX mission complete signal (018F92)`; 0
caídas mortales, 63 rescates, 0 cuelgues del RSP. PERF (ventanas de 300
fotogramas, µs): etapa 2 cpu media mediana 6304, peor media 10699; etapa 3
cpu media mediana 21633, peor media 53384, sim media mediana 5722 (la
etapa 3 incluye el planificador de referencia, que no existe en la
versión); peores fotogramas sueltos 0,3..1,07 s en las cargas de etapa.
Subbloques de audio: 888 en todo el recorrido.

## Fase 2g: pulido de la Misión 2 (rama `work-m2pol`)

- Audio: 888 → 9 subbloques en el recorrido (Misión 1 sin cambio de
  mecanismo). Segunda llamada al servicio de audio por fotograma y llamadas
  entre las cargas de bloques de la etapa (`docs/RENDIMIENTO.md`).
- Etapa 3: cpu media de la peor ventana 53.384 → 24.727 µs (recorte vertical
  del fondo en los dos modos, torre agrupada por página). La simulación no
  cambia (solo el dibujo).
- `1E003C`: corregido (lectura fuera de un bloque de ROM, arriba).
- Rescates de la referencia: con los escombros de `06870A` ya nativos, el
  jugador de referencia podía quedar en un bucle de rescates en x 1036..1053
  al final del primer desplazamiento de la etapa 2 (punto seguro del que
  vuelve a caer; 300+ rescates y la prueba fallaba). Tras 8 rescates sin
  aterrizar se vuelve a un punto seguro anterior (hasta 4, a ≥ 32 px);
  sin ninguno, la caída mortal. Solo `MSX_REFERENCE_INVULNERABLE`.
- Pendiente: paredes 0xF/0x1F de la etapa 3 contra MAME (sin grifo todavía;
  el núcleo sí trata la forma 15 en `k12ad4`/`k12ee2`, la petición del
  jugador lleva `b400a` = 0), familia del prisionero `099E1C` (≈3,5 KB de
  código de 099E1C..09AD4C) y cargas de etapa incrementales (siguen siendo un
  fotograma con la pantalla en negro; ya no cortan el audio).

## Fase 2h: restos de la Misión 2 (rama `work-m2pol2`)

- **Prisionero `099E1C`**: `09A842` (caja oculta `09A692`) ya lo crea; es
  la entrada de `099DCC..` que salta a `099FD8` (prisionero suelto, ya
  nativo). Familia `secret2` (etapa 1, tres cajas): 8 nodos, 1477
  actualizaciones en bucle cerrado, 246 dibujos
  ([MISION2_ACTORES.md](MISION2_ACTORES.md)).
- **Paredes 0xF/0x1F**: grifo de MAME (`MSX_FULL_DEBUG_CMDS`, nuevo en
  `capture_fullgame.lua`) en `01448C`/`014492` con A6 = 1014C4 desde
  `sec03_26140` hasta 40000 (`captures-m2g/wq1`): `10C00A` vale 0 en las
  4653 peticiones del jugador (nadie lo escribe), así que la petición del
  port era correcta. 107 paradas (bit 2 de D6): x 624 (celdas 0x1F de la
  columna 77, filas 10..15) y en la torre x 759/791/823/887. En el aire el
  barrido termina en la raíz (`D6` = 8000000E, A0 = 10E968) y `wall_stop`
  lo descartaba por no ser el terreno; el núcleo nativo, con la misma
  petición, da lo mismo (0E, raíz). Ahora se acepta y la x vuelve al
  terreno (`01AC4C`). Paradas en el recorrido: 0 → 2.
- **Cargas de etapa**: entre lecturas y bloques, `load_idle` (actor.c)
  muestra el fotograma retenido en negro cada ~33 ms y toma el siguiente
  sin esperar al borrado (`display_try_get`; al final `display_get`), como
  el original carga tras su cortina/fundido. Etapas 1/2/3: 3/7/16
  fotogramas negros, hueco máximo 39/40/46 ms (antes un solo fotograma de
  0,14/0,29/0,64 s). La medida `worst frame` del PERF sigue contando la
  llamada entera.

### Prioridades del usuario (MiSTer)

- **Música de la Misión 2**: el original pide la música del descriptor de
  la etapa (`018412..01842E`: palabra +C, solo si cambia respecto a +80
  del actor de misión). Etapas 1/2 = `0022` (la 2 no la vuelve a pedir),
  etapa 3 = `0023`; el jefe crea `059F76`, que manda el fundido `0A..` y
  tras la espera de `0E11AA` pide `0027` (grifo MAME `mus0`/`mus1`:
  `0023` en 26139, `0027` en 93274, `0020` en 117580, `0024` (Misión 3)
  en 117964; ningún `03` entre etapas). `data/sound_map.json` tiene
  `music_mission2`, `music_mission2_stage3` y `music_mission2_boss`
  (render aislado, bucles medidos 79,33 / 90,45 / 72,26 s).
  `game_audio_stage_music` cambia la voz de música al cargar cada etapa
  (y vuelve a `0021` al reiniciar la Misión 1); las palabras de música de
  la lista de sonidos del mundo (el `0027` del jefe) también, y `0Axx`
  la para. En ares: `MSX AUDIO stage music 0022`, música activa toda la
  partida.
- **Caídas**: la referencia escribe ahora cada rescate y cada muerte por
  caída (`MSX fall rescue/death stage N: x y camera ...`). Recorridos: los
  rescates salen todos en la etapa 2, x 964..1097, con la cámara subiendo
  (y 80→168) al final del primer desplazamiento: el jugador cae desde la
  escalera de 45° (celdas de forma 5, x 984..1031) al foso de debajo. La
  rejilla coincide con MAME (`run4`: x 945..1029 → y 160..221; el modelo
  de suelo del port recorre la escalera igual, prueba de host). Las
  caídas vienen de los saltos del mando de la prueba sobre la escalera
  mientras sube la cámara, no de un hueco en la colisión; queda abierto.
  Resuelto en la Fase 2i (abajo): eran la pared x 944 sin parada y el
  aterrizaje en pendiente del modelo de planos.
- **Mapa al empezar la etapa 1**: las paletas del mapa de la etapa 1 se
  tomaron del primer fotograma observado, que era del fundido de entrada
  (468 celdas y el fondo oscuros: la esfinge salía a oscuras toda la
  etapa). `extract_mission_map.py --skip-fade` descarta los fotogramas con
  el fundido activo (RAM `10991A..10991F` ≠ 0): etapa 1 13 → 9 paletas,
  fondo 16384 → 31384; etapas 2 y 3 sin cambios. Comando de la etapa 1:
  añadir `--skip-fade` al de arriba y volver a empaquetar.


## Fase 2i: caídas de la Misión 2 (rama `work-m2fall`)

Informe del usuario (hardware real): en la Misión 2 el jugador atraviesa
suelos donde el original no. Dos causas, las dos en los barridos del
jugador (`0143EE` aire, `014594` suelo, ambos con `013968`):

1. **Paredes sin parada (la causa de los rescates en ares).** `0128E8`
   copia antes de cada barrido del jugador 1 la caja de pantalla
   RAM `10C01C..10C023` del registro de RAM `10C2E8` (ROM `016DBE` =
   `001C 0000 0108 01E0`; MAME: las mismas palabras en todas las llamadas
   de las etapas 1 a 3). `wall_stop` barría con la caja del mundo, 0×0 salvo
   que un vehículo la hubiera escrito (`neo_slug.c`, `neo_tread.c`), y
   `0138A0` recortaba a nada todo movimiento hacia la derecha (y hacia
   arriba): la pared no se veía nunca. En la etapa 2 el jugador entraba en
   la pared x 944..983 del pasillo y 80 (el original se queda en
   x 943,99, `run4`) y caía al foso de x ≥ 984, bajo la escalera: los
   rescates en x 964..1097 con la cámara subiendo. Recorrido del piloto de
   MAME por la etapa 2 (`capture_fullgame.lua` desde `sec02_19932`, con la
   escena exacta de cada llamada): 432 paradas en pared, 431 perdidas sin la
   caja, 398 con ella (las 34 restantes y los demás desajustes caen en
   celdas que la rejilla del port no tiene igual: x 2264 y 558 y la salida
   x ≥ 2656). `neo_kernel_player_sweep` (neo_kernel.c) pone la caja de
   `016DBE` para la llamada y devuelve la del mundo; `wall_stop` la usa.
2. **Aterrizaje en pendientes.** `neo_land_step` solo aterriza en una
   superficie entre la altura anterior y la nueva *en la x nueva*: una
   superficie que sube más deprisa que la caída (la escalera de 45°
   x 976..1031 y las cuartas de pendiente de después, y 224..232) se cruza
   y el jugador cae. MAME (`capture_m2_stairs.lua`, estado `at_22370`,
   saltos con plan de mando en la escalera mientras sube la cámara): el
   original aterriza, p. ej. en x 1038,670 y 225,667 desde x 1035,290
   y 225,307 con (3,981, −2,407); el modelo de planos sigue hasta el foso.
   Ahora, si el modelo de planos no aterriza, decide el barrido
   (`neo_kernel_air_landing`: `013968` con la petición de `0143EE`,
   10C008 = FF, 10C009 = 0, la caja anterior); con suelo (D6 bit 0) del
   terreno, posición y +30/+34 del barrido. Sin suelo devuelve 10C018/
   10C068 como estaban. Contador `kernel landings` en el registro de ares.

Evidencia (MAME 0.264):

- `tools/test_m2_stairs.c` (`make check`, `data/m2_stairs_fixture.json.gz`,
  `tools/extract_m2_stairs.py`/`compile_m2_stairs_fixture.py`): 676
  llamadas del jugador bit a bit con la rejilla y el anillo del port (8
  planes de saltos y 128 llamadas del piloto en la pared x 944); las 110
  paradas en pared se pierden todas con la caja 0×0; el modelo de planos
  pierde 5 aterrizajes del original y el barrido los da exactos; los
  planes eA/eC/eD/eH atraviesan la escalera sin el arreglo y siguen las
  posiciones de MAME con él (65/65 en eA y eH; en eC/eD el original se
  desliza parado en la pendiente al final, no portado).
- Recorridos del piloto con las llamadas trazadas (escena aproximada desde
  `trace.csv`): etapa 2, 10 aterrizajes perdidos por el modelo de planos,
  9 exactos con el barrido (el otro, en la salida x 2669, es la rejilla);
  etapa 3 antes del jefe (marcos < 91000), 12 perdidos, los 12 exactos.
  Desde el jefe la torre (`0B7C38`) no está en la rejilla estática de la
  comprobación (no es una medida del juego).
- Capturas: `MSX_FULL_DEBUG_CMDS` con puntos de parada en `01448C`
  (entrada, RAM 10C004..10C00D, 10C018, 10C068, 10C01C..10C023, 10E9CC,
  10C72E +14..+20, 10E968 +14..+20) y `014492` (salida), A6 = 1014C4.

**Salida de la etapa 2.** Con las paradas en pared el jugador ya no
atraviesa la puerta x 2656..2719 (y 640..767, celdas 0xF) y el gancho de
salida de la referencia no se alcanzaba. Ahora es nativa (rama
`work-m2gate`, `neo_m2props.c`, docs/MISION2_ACTORES.md "Puerta de la
etapa 2"): la orden de mapa `2C` `0B7326` crea la puerta `03A6EA` tras el
actor de misión (`neo_world_mission_actor`, cabeza de la lista); `03A75A`
espera RAM `10C358` (la pone `03A892`) y `03A776` llama a `0B7350`: parche
`0B706C` por `0C70FC` en el bloque (0xA2, 0x2F) (fila superior 95 contada
desde abajo; `g_map_patch` ya no exige la rejilla de 32 filas y las celdas
dentro de la ventana del anillo se copian a él), sonido 103B y la salida
`0B6ED2` (rectángulo `0B7348`, +72 = 0xAC0). La puerta sube 0x50 px y se
queda. El gancho de la referencia ya no actúa en ninguna etapa de la Misión
2. Las 63 celdas «en conflicto» de la rejilla privada de la etapa 2
(columnas 336..339, filas 80..95) eran exactamente las del parche vistas
después de él: `fill_collision_rom.py --patch b706c:a2:2f` les pone el
valor de la ROM (antes del parche; 0 conflictos):

```sh
python3 tools/fill_collision_rom.py filesystem/mission2/s2/collision.json --rom mslugx.zip --descriptor b702a --top 101 --patch b706c:a2:2f
```

**Caída en x 2286** (MAME, captura con puntos de parada en `01448C` y
`014492`, A6 = 1014C4, marcos 24196..24245 del piloto desde
`sec02_19932`): la parada del original en x 2263,99998 (fotograma 24207,
D0 = 08D7FFFF, D6 = 4F) sale con **A0 = 10C72E** (el terreno), no con el
objeto dinámico. La rejilla estática no tiene pared en x 2264 (columna
283: solo la pendiente de las filas 68..71), así que la pared es la del
objeto dinámico de la ranura 7 (`0E1B6A`, el obstáculo `06851A`) y el
barrido de 013968 no cambia A0 en una parada lateral: `wall_stop` ya acepta
ese resultado (padre terreno) y no se toca. En ares la caída sigue siendo
intermitente (recorrido A: 0 rescates en la etapa 2; recorrido B: 1, en x
2281 desde x 2261): sin medir si el obstáculo del port existe aún cuando
llega el jugador (en MAME el piloto lo rompe en el fotograma ~24216 y sigue
a x 2266,9) o si es el aterrizaje en la pendiente. Parada explícita.

**Recorrido en ares** (rama `work-m2gate`, `mslugx-m2-reference.z64`,
`--stage-seconds 800`, dos recorridos): `RESULT: PASS` en los dos; la
etapa 2 termina por su salida nativa (`exit hooks=0`), 0 caídas mortales;
rescates: A 0 en la etapa 2 y 2 en la 3 (x 348), B 1 en la etapa 2 (x
2281) y 4 en la 3 (x 348 y 611; la etapa 3 no cambia en esta rama). Explícito: el sonido 103B
de `0B7350` no está en `data/sound_map.json` (petición sin render) y las
teselas del parche (+0 del sub-mapa, como en la etapa 1) no se dibujan.

**Recorrido en ares** (`mslugx-m2-reference.z64`, `--stage-seconds 800`;
con 400 la etapa 2 no llega a soltar la parada 2368 con otros dos ares en
la máquina): `RESULT: PASS`, 0 caídas mortales, 2 rescates (antes 63;
con solo el aterrizaje, 15 en la pared x 944 y la puerta), 5 aterrizajes
por el barrido. Quedan, sin cerrar: etapa 2 x 2286 (y 467, desde x 2261
y 557): en MAME el jugador para en x 2263,99 contra el objeto de colisión
dinámico de la ranura 7 (mapa `0E1B6A`, el de los escombros de `06870A`;
la rejilla del anillo es igual a la del port), que `wall_stop` no toma
(solo paredes del terreno); etapa 3 x 348 y 888 (desde x 259 y 928), sin
captura todavía.

Desviaciones vistas y no tocadas (sin prueba de la rutina): el salto sin
dirección desde la carrera deja +30 = 0 en el original (plan eE; el port
conserva la velocidad), y el jugador parado en la cuarta de pendiente
y 226 avanza ~1 px por actualización en el original (eC/eD).

## Fase 3: actores secundarios y cadena de eventos (rama `work-m2events`)

Informe del usuario (hardware real, ROM 20): faltan actores secundarios de
la Misión 2 (mineros, arqueólogos…) y la cadena de eventos no sigue a la
recreativa.

### Inventario: lo que crea el guion de la recreativa y lo que crea el port

Arcade: `data/mission2_inventory.json` (`run4-debug`, sonda SPAWN `01A132`:
298 registros de aparición ejecutados de las etapas 1 a 3 y el jefe) y las
capturas de familia de esta rama (`captures-m2ev/s2a`, `s2b`). Port: el
auditor (`tools/audit_script_events.py`, `script_native.c`) y el registro de
ares (`MSX script spawn record … native N`, abajo). «Contado»: el registro
lleva su grupo (< 9) y retiene las esperas del guion.

| Constructor | Etapas | Registros | Ejecutados (arcade) | Contado | Port | Qué es |
| --- | --- | --- | --- | --- | --- | --- |
| `03882A` | 1 | 1 | 1 | no | sí | puerta de la etapa 1 |
| `039590` | 1 | 8 | 5 | sí | sí | espadachines |
| `08C37C` | 1,2,3 | 70 | 64 | no | sí | objetos |
| `099E06` | 1,2 | 4 | 4 | no | sí | prisionero en la silla |
| `09A67E` / `09A692` | 1,2,3 | 55 / 4 | 25 / 1 | sí / no | sí | cajas ocultas |
| `01913C` | 2,3 | 6 | 6 | no | sí (ya lo aplicaba; ahora contado como nativo) | reinicio del tiempo |
| **`021A1C` / `021A26`** | 2 | 8 / 9 | 8 / 2 | no | **sí (nuevo)** | **mineros** (excavan, se asustan y regalan un objeto, huyen) |
| **`035B66` / `035C68`** / `035D12` | 2,3 | 8 / 4 / 3 | 5 / 4 / 1 | no | **sí (nuevo)** / **sí (nuevo)** / no (parada) | murciélagos: bandada, murciélago con recorrido; el nido de la etapa 3 sigue parado |
| `03A892` | 2 | 1 | 1 | sí | sí | señal de la puerta de la etapa 2 |
| **`0529C0`** | 2,3 | 21 | 21 | no | **sí (nuevo)** | dirección de la flecha GO (10C44A) |
| `0613CA/D8/E6` | 2,3 | 137 | 41 | sí | sí | momias |
| `0678CA` `067902` `0692B2` `0678A2` `0678B6` | 2,3 | 48 | 35 | sí | sí | soldados |
| `06835C` | 2 | 1 | 1 | sí | sí | soldado con rehén |
| `06851A` | 2 | 3 | 3 | sí | sí | sacos |
| **`099DE2`** | 2 | 4 | 1 | no | **sí (nuevo)** | prisionero atado (variante +86 bit 0) |
| **`099DF0`** | 2 | 3 | 3 | no | **sí (nuevo)** | prisioneros colgados de una cuerda (trozos `09A9C4`, dibujo directo `011FF6`) |
| **`0B829C` / `0B833A`** | 2,3 | 1 / 24 | 1 / 24 | no | **sí (nuevo)** | carteles de madera (el primero con sello de colisión) |
| `026254` | 3 | 1 | 1 | sí | sí | jefe |
| `032DE2` | 3 | 2 | 1 | no | no (parada) | sin identificar: paleta 0x105, programa `239492`, andador `08CCD4`, sigue en `067AB4` (soldado) |
| `03670E` | 3 | 8 | 7 | sí | sí | dron |
| `03A05A` / `03A2AA` | 3 | 1 / 1 | 1 / 1 | no | no (parada) | sin identificar: espera de cámara +70 y sonido 10EE / hijos `03A344`, `03A58A` |
| `052A5C` | 3 | 1 | 1 | sí | sí | espera de cámara |
| `082BCE` | 3 | 6 | 6 | no | no (parada) | sin identificar: busca al jugador (`01AF9A`), `0832A6`.. |
| `08C154` / `08C168` | 3 | 7 / 18 | 3 / 16 | sí | sí | cajas de suministros |
| `09692C` | 3 | 2 | 1 | no | no (parada) | sin identificar: crea `03B4E0` |
| `0995EC` | 3 | 1 | 1 | no | no (parada) | objeto golpeable (caja `099838`, vida +72) |
| `099862` / `099DCC` | 3 | 1 / 1 | 1 / 1 | no | sí | prisioneros |

Apariciones que el port pierde: de 17 constructores no contados a **7**
(`032DE2 035D12 03A05A 03A2AA 082BCE 09692C 0995EC`, todos de la etapa 3);
contadas perdidas: 0. Todas son paradas explícitas: el mundo cuenta el
constructor (`external_spawns`) y el registro de ares dice `native 0`.

### Cadena de eventos: recreativa contra port

`tools/compare_m2_events.py LOG` compara los registros del guion que ejecutó
la recreativa (`run4-debug`, en orden) con los que ejecuta el port (líneas
`MSX script spawn record R constructor C native N camera P` que `actor.c`
escribe para cada registro que pasa por `script_spawn` de `neo_world.c`).
Las alternativas de una elección al azar (op 10) cuentan como un evento (el
RNG elige otra). Recorrido de ares de esta rama (`mslugx-m2-reference.z64`,
`test_mission2_ares.py --stage-seconds 800`, PASS):

| Etapa | Registros (arcade) | Registros (port) | Mismo orden (LCS) | Solo arcade | Solo port |
| --- | --- | --- | --- | --- | --- |
| 1 | 10 | 13 | **10** (todos los del arcade) | — | `2D74D4` `2D74EA` `2D7500` (cajas `09A692` de la cámara 320: la puerta `03882A` del arcade se rompió antes y `018F92` borró el guion) |
| 2 | 80 | 83 | **78** | — | `2D7B58` (`if` grupo 1 = 1), `2D7FA2` (`if` grupo 2 < 1), `2D8120` (`if` grupo 7 < 1) |
| 3 | 208 | 149 | **137** | 52 registros dentro de `if` de grupo vacío (`2D8EE4` g2, `2D9110` g4, `2D9358` g8: los 14 objetos de la caja `09A67E` rota, `2D997E` g8, `2D9B12` g2), `2D97EE` | `2D9294` (elección al azar), `2D98F2` `2D9904` (`if` 0B: posición del jugador); `2D979C` 8/5, `2D9CB8` 8/1 |

(Recorrido D, el último: `PASS`, 0 caídas mortales, 2 rescates; un
recorrido anterior con cuatro ares en la máquina se quedó sin tiempo en la
parada 2368 de la etapa 2 sin cambio de lógica.)

Todas las diferencias restantes son **del jugador**, no de la lógica: los
`if` del guion leen los recuentos de grupo (enemigos o cajas destruidos a
tiempo) o la posición del jugador (`01903A`), y el piloto de la prueba no
juega como el de la captura; el intérprete (`neo_script.c`) es el de la
Misión 1, ya bit a bit (`test_neo_script`). En el mismo orden quedan todos
los eventos que ejecutaron los dos lados.

Lo que la cadena no tenía y ahora tiene (además de los actores de arriba):

| Evento (ROM) | Recreativa | Port antes | Port ahora |
| --- | --- | --- | --- |
| MISSION 2 START `0607DE` (`01860A`, 42FA = 0) | letras en la etapa 1 | nunca (solo la Misión 1 por el paracaídas) | `msx_game_stage_actors` al entrar en `0186A0`, con la misión del flujo (RAM 10C2F4 = 42F4) |
| voz `607D2[misión]` (0x1132) | sí | no | **parada**: sin render en `sound_map.json` (`start_voices`) |
| flecha GO `05287C` (`01860A`, cada etapa; `run4`: 19429, 20009, 26216) | sí | nunca creada | creada en cada etapa de una misión empaquetada; entorno 10E9A0/10E9BC/10C44A del mundo (antes 0: siempre en reposo) |
| dirección de la flecha `0529C0` (21 registros) | 10C44A | perdida | `neo_world.c` |
| tiempo `01913C` | reinicio | ya se aplicaba (el auditor lo contaba como perdido) | igual; nativo para el auditor |
| actores de las secciones 2/3 no contados | creados | 17 constructores perdidos | 7 perdidos, todos de la etapa 3 (paradas, arriba) |

Sin cambios (ya nativos y comprobados en fases anteriores): cortinas
`3AED8/3AEF0`, puertas `03882A`/`03A6EA` y salidas `0B6ED2`, esperas y
bloqueos de cámara (`op 4`, `052A5C`), torre `0B7C38`, jefe `026254` y su
música `059F76`, música por etapa, fin de misión `018F92`/`018F40`. La
flecha GO de la **Misión 1** sigue sin crearse: sus sprites no están en el
paquete de la Misión 1 (parada).
