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
actores de la Misión 1. Mientras no exista el cargador se escribe en `build/`
(no en `romfs/`), así la ROM no cambia.

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

## Pendiente (fase 2)

1. Cargador por misión en `scene_assets.c`: tablas de la etapa desde
   `missionN.pack`, páginas por la caché; reglas de capas por etapa en lugar
   de `mission_camera.h` (Misión 1).
2. `neo_collision` con rejilla y columnas por etapa (la etapa 1 ya está
   completa); `neo_world_init` con guion (sección 1 `2D7298..2D7516`, blob de
   ROM), lista de cámara `0B6D2A` (espera de progreso en `0B6DF2`) y
   jugadores `019604` (`+4` = `2D5C6A`).
3. Salida de la etapa 1: el actor de mapa `0B6ED2`/`0B6F24` (rectángulo
   `01B2CE` → `018F40`, caminata automática `0CA340`): pendiente. Hechos en
   `work-m2act` ([MISION2_ACTORES.md](MISION2_ACTORES.md)): transiciones
   `3AED8`/`3AEF0` nativas (`neo_wipe.c`, sin paradas en `test_neo_mission`),
   flecha GO `05287C` y marcador `0B6F6A` (`neo_goarrow.c`).
4. Etapas 2 y 3: cámara/render verticales. Colisión: etapa 2 completa desde
   la ROM (`fill_collision_rom.py`, 0 desconocidas); etapa 3 completa hasta
   la fila 207, quedan 37037 celdas fuera de la vista de la subida.
5. Recorrido de la Misión 2 en ares (`tools/test_mission_native_ares.py` con
   la etapa 1) cuando exista el cargador.
