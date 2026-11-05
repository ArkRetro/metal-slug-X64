# Misión 3: fase 1 (flujo, escena, paquete, auditoría, recorrido)

Rama `work-m3` (desde `ccr-09eb2f6b-8rwl8l`). Misma regla que en la
[Misión 2](MISION2.md): lo traducido tiene evidencia de MAME; lo no
recuperado se detiene y se cuenta. ROM, capturas y derivados (mapa,
rejilla, paquete) siguen fuera de git.

## Flujo

`run4` (`data/mission_flow_fixture.json`): la Misión 3 es **un solo
registro de etapa**, `0173F6` (tipo 0, entrada 0, salida 8, como la etapa 1
de la Misión 2), seguido del fin de misión `017412` (tipo 4, `+4` = 0).
Descriptor `016E0E`: lista de cámara `0B8968`, jugadores `2D5E86`, guion
`0x810` (sección `2D9CDA..2DB694`), música `24`, etapa **4** (42F6). En
MAME: carga en 117975, bucle `0186A0` en 118023, fin de misión `018870` en
149172 (unos 31 000 fotogramas).

- `01891E` ya elegía la Misión 3 tras la Misión 2; `actor.c` carga
  `romfs/mission3.pack`. El índice de etapa del paquete es ahora 42FA
  (registro dentro de la misión: Misión 2 0..2, Misión 3 0), no 42F6 − 1
  (daba 3 en la Misión 3), y el descriptor del paquete debe ser el del flujo
  (si no, parada explícita).
- Fin de misión: el 018F92 (10C300) de la Misión 3 levanta
  `mission_complete` (única etapa).
- `scene_assets.c`: una tabla vacía del paquete (la Misión 3 aún no tiene
  sección de actores) no se lee (`dma_read` de 0 bytes era un assert).

## Escena (privada, `filesystem/mission3/s1`)

```sh
C=/home/user/captures-fullgame/run4
python3 tools/extract_mission_map.py $C --ram-layers --allow-backdrop-timeline --frames 117966:149171 --output filesystem/mission3/s1
python3 tools/extract_region_collision.py $C --rom mslugx.zip --descriptor-layers --layer-camera --both-layers foreground --frames 117966:149171 --output filesystem/mission3/s1/collision.json
F=filesystem/mission3/s1/collision.json
python3 tools/fill_collision_rom.py $F --rom mslugx.zip --descriptor b853a --top 45 --columns 860
python3 tools/fill_collision_rom.py $F --rom mslugx.zip --descriptor b8550 --column 520 --top 33 --outside-empty
python3 tools/rebase_collision.py $F --ring-tail 64   # "Reconocimiento del mapa"
python3 tools/export_m3_layers.py && python3 tools/export_m3_stage.py   # after extract_m3_recon.py
python3 tools/pack_mission.py 3 filesystem/mission3/s1 --loose-layers --report build/mission3_pack_report.json
python3 tools/render_m3_sections.py --report filesystem/mission3/s1/renders.json
```

**Novedades del motor que la Misión 3 usa** (es la etapa del tren):

1. **Cuarto objeto de capa `10E8C6`** activo en toda la misión (ranuras 1,
   33, 113, 145 de `10E83E/10D7B6/10E8C6/10C72E`). `neo_scene.py` lo toma
   como capa lógica 2 y `10D7B6` como capa 1 a cualquier velocidad (orden de
   dibujo = orden de ranuras); pistas de traza `obj4_x/obj4_y`. Su mapa
   (`2A6142`) solo aparece un momento (cámara ≈ 3944): superposición **no
   empaquetada**.
2. **Capas que se mueven solas**: el cielo (`10E83E`) pasa del mapa `0B857C`
   al puntero 0 en la cámara ≈ 320 (lo dibuja otro escritor, no un mapa) y
   `10D7B6` (mapa `0B8566`, **el tren**) avanza con su propio movimiento: su
   origen de columnas (+32) se mueve durante todo el recorrido y da la vuelta
   cada 4096 columnas (65536 px). Los fotogramas cuyo origen cambia dentro de
   la ventana de retraso se saltan (19). `pack_mission.py --loose-layers`
   desenrolla los desplazamientos, considera una capa activa solo con su
   primer mapa y guarda el mejor ajuste por mediana: capa 0 x 1 hasta la
   cámara 321; **capa 1 x 1 con error mediano 532 px (peor 2240)**: el
   movimiento del tren no es una regla de cámara (pendiente: su escritor).
3. **Cambios de mapa del primer plano** `0B853A → 0B8600 → 0B8550` (op 0x18
   de la lista de cámara en `0B8BB8`/`0B8C1C`): las celdas coinciden con el
   mapa principal donde ambos se vieron (0 diferencias), así que el paquete
   las une al mapa (1063 celdas).
4. **Dos rejillas de colisión**: `10C72E` (`0B853A`, luego `0B8550`) y también
   `10D7B6` (`0B8566`, el tren). La lista de objetos del núcleo (`01382A`,
   `neo_kernel.c`) solo modela `10C72E` y un `10D7B6` inactivo: la rejilla
   de la etapa es la del primer plano (`--both-layers foreground`) y **la
   colisión móvil del tren queda pendiente**.

Colisión: `0B853A` encaja en la columna 0 con la fila superior 45 (13448
celdas observadas iguales, 0 conflictos); `0B8550` en la columna 520 con la
fila 33 (6640 iguales, 0 conflictos). Las 8668 celdas restantes están fuera
de las dos rejillas de ROM (filas por encima/debajo del mapa y las columnas
480..519 de `0B8600`, que no tiene puntero de colisión) y se marcan vacías
(`--outside-empty`). La rejilla observada tenía 4046 columnas (columnas
envueltas pasado el final): se recorta a 860.

| Etapa | Mapa | Variantes | Fondos | Rejilla | Desconocidas |
| --- | --- | --- | --- | --- | --- |
| 4 (tren) | 7150 celdas, cámara 0..6496 | 2043 | 7 valores | 860 × 47 | 0 (145 mutables) |

Paquete `romfs/mission3.pack` (v3, sin actores aún): 352 páginas, 44
bloques, 46 paletas, 5558 imágenes, **0,93 MiB**.

## Auditoría del guion

`tools/audit_script_events.py build/host/script-native --script
data/mission3_script.json`: 256 registros, 44 constructores, 13 nativos;
**77 apariciones contadas que el port pierde** (en `make check`, solo informe). Las más frecuentes:

| Constructor | Contadas | | Constructor | Contadas |
| --- | --- | --- | --- | --- |
| `097FCC` | 20 | | `0A2F2E` | 5 |
| `09BA2A` | 9 | | `05D916` | 4 |
| `02B202` | 6 | | `05C53C`, `04E990` | 3 |
| `08A732` | 6 | | `0391EA`, `0569D0`, `084588` | 2 |
| `08DA12` | 5 | | `09AE2C 0AF1B6 027F54 09AE1E 05D234 05D226 08AFB6 0B9400` | 1 |

Ninguno comparte cuerpo con una familia nativa (`097FCC` → `0120F8` +
`0985A0`; `09BA2A` → `09CB70`; `02B202` → `02BDA6`; `08A732` y `08DA12` con
sus tablas propias; `0A2F2E` → `0A3DB0`): **no se ha portado ninguna
familia de la Misión 3 en esta fase** y `filesystem/mission3/rest-native.json`
no existe todavía (el paquete se construye sin sección de actores). Sin
contar: `01913C 01F1D4 0214EA 03EE8A 0569B4 0569D0 0569E6 093C9A 099E1C
09AE1E 0AC866`.

Tabla de spawn: `compile_soldier_ai_tables.py` añade los desplazamientos del
guion de la Misión 3 (sin ellos el guion paraba en su primer registro,
`2D9CEE`).

## Recorrido (`tools/test_mission3_ares.py`, `mslugx-m3-reference.z64`)

Cartucho de referencia (nunca una versión): `MSX_REFERENCE_START_MISSION=3`
salta al registro `0173F6`; mismos ganchos que el de la Misión 2 (jugador
invulnerable, registros de constructores no portados saltados y registrados
con su dirección, paradas de cámara del guion soltadas). Mando: el de la
prueba de la Misión 2 sin piloto.

Resultado (300 s, `RESULT: PASS`, umbral de progreso 1000): la etapa carga
en 506 ms (tablas 364 KB en 23 ranuras, mapa 44 bloques), Marco entra
andando y la cámara (modo 0) llega a **x 1901 (progreso 1900 de 6496)**,
0 celdas desconocidas leídas, 0 eventos de cámara sin soporte. Ahí para: el
jugador queda entre los vagones (x 1929..2012), sin subir a la rampa de
x 2000..2072. 18 registros saltados (constructor, registro):
`09AE2C 2D9F38`, `09BA2A 2D9FA0/2D9FB8/2DA68A/2DA6E2`, `08DA12
2DA030/2DA11A/2DA23E`, `02B202 2DA18A/2DA274/2DA3DE/2DA428`, `027F54
2DA4A6`, `05D916 2DA646`, `0391EA 2DA6B4/2DA800`, `08A732 2DA6FA`,
`09AE1E 2DA790`. Rutina sin soporte del mundo: `0AF17E`. 54 dibujos sin
descriptor (`descriptor` 10 y 6). Rendimiento: peor media de ventana 16,8
ms de CPU.

## Presupuesto

ROM: 31 948 800 B (30,5 MiB) con `mission2.pack` (3,1 MB) y `mission3.pack`
(0,93 MB); la Misión 3 no añade datos al programa salvo ≈ 1 KB de la tabla
de spawn (`mslugx-explorer.elf`: texto 1 724 844 B, datos 19 328 B, bss
160 788 B, total 1 904 960 B). RAM: tablas de la etapa 364 KB en 23 ranuras de la caché (219
ranuras, 3504 KB); el presupuesto fijo no cambia.

## Fase 2: el tren (rama `work-m3b`)

**Capa `10D7B6` nativa** (`src/neo_train.c`). El escritor de su `+14` es
`0C81C0`/`0C6F9E` (paso de capa: bit 0 de `10E956+i` → la capa suma su
velocidad propia `10E94E` en vez de la de la raíz `10E970`; op 0x3C de la
lista de cámara en el progreso 0x140: `10E957 = 10E958 = 1`). `10E94E` lo
escribe el conductor `0B8D64` (creado por `0B8D34` desde la lista de
cámara): raíz (× 3/4 en el modo 2) + `10C6B8`, que con `10E9D0 = 0` salta al
suelo del modo (`0B8CFC` modo 1: −2 px; `0B8D06` modo 2: −12 px). El
conductor corre tras las capas: la capa usa la raíz de la actualización
anterior con el modo vigente. El origen de columnas `+32` lo mueve `0B8DD0`:
mientras `10C6B6 ≠ 0`, si `tabla[10C6B4] + 32 − 0x20 < +36` (con `+36 =
(−x >> 20) − 4`, `0C748E`) suma 0x20 (el tren repite 32 columnas antes de la
columna de la tabla `0B8E36`: 0x50, 0x78, 0xB0, 0xE0, 0x138, 0x188; las
elige `0B8E7E..0B8EA4`). `test_neo_train` (`data/train_fixture.json.gz`,
`tools/extract_train_fixture.py` desde run4): **14252 actualizaciones, `x`
y `+32` exactos**. El desplazamiento observado de la capa es `−x − 16·(+32)`
con un paso de retraso (553 de 614 observaciones exactas, el resto es el
retraso de la instantánea): el renderizador dibuja la capa 1 así en vez de
la regla ajustada (deriva mediana anterior 532 px). Las celdas del paquete
siguen siendo las primeras vistas por posición (módulo 65536).

**Colisión** (`neo_kernel.c`): `01382A` recorre `10D7B6` tras `10C72E` con
la ventana vertical de `013856..013896` (`(+34 − 0x11 + 1)·16 + y`, 7 filas
de 16 px). La rejilla `0B8566` (784 × 48) es una plataforma `0x2B` en la
fila 36 de cada columna salvo 384..447 (`0xA0` en las filas 46/47):
`neo_kernel_train_cell`, comparada con la ROM por `tools/test_train_grid.py`
(`make check MSX_ROM_ZIP=...`). En el anillo la fila de mapa r ocupa la fila
r + 20 (RAM `10D83E` de run4). El jugador puede aterrizar en ella pero no
viaja con el tren (en run4 el jugador tiene siempre `10C72E` como padre en
este tramo: x 1338..3179 con `+38 = 10C72E`).

**Descriptor que faltaba**: los 59 dibujos sin descriptor del recorrido eran
todos `18653C` (de `filesystem/mission2/rest-native.json`; `actor.c` registra
ahora la última etiqueta que falta). `filesystem/mission3/rest-native.json`
(privado) lleva ese marco y las paletas de la Misión 2; el paquete
`mission3.pack` tiene ya sección de actores (1 descriptor, 0,95 MB).

**Recorrido** (`mslugx-m3-reference.z64`, 300 s): igual que en la fase 1,
cámara 1901, jugador hasta x 2017. El bloqueo no es el tren: en x 1930..2000
el jugador **cae** (y hasta −93, 147 rescates) a través del suelo del vagón,
celdas `0x31..0x3B` del grupo 1 en las filas 17/18 de `10C72E`. El barrido del
jugador (`0128E8` pone `10C00D` siempre) borra el grupo 1 en `0129A2`, y la
Misión 3 no tiene tabla de grupos (`10C00E = 0` en la RAM de run4), así que
`player_surface` (`neo_collision.c`) las ignora igual que el original. En
run4, sin embargo, el jugador pasa por encima (x 1953, `+38 = 10C72E`): falta
identificar sobre qué celda se apoya (la conversión de y entre la RAM y la
rejilla del paquete no está cerrada; con y local = y + `10C72E +18` − 256 da
la fila 17).

## Reconocimiento del mapa (rama `work-m3map`)

Encargo: el mapa entero de la Misión 3 guardado (no jugable todavía): capas,
colisión, plataformas dinámicas, listas de cámara, reglas de capa,
descriptores, y los puntos de suelo iguales al arcade. Sin trabajo de
actores ni de jugabilidad.

**Captura** (MAME 0.264, desde `states4/sec04_117966`; la reproducción es
`run4` fotograma a fotograma: 0 diferencias de posición del jugador en
117967..149300). `capture_fullgame.lua` gana `MSX_FULL_CELL_TAP` (cada celda
que lee `0C79FE`, PC `0C7A18`, de los anillos `10C72E`/`10D7B6 +88`, con x/y
locales, agrupadas en `cells.txt`) y `MSX_FULL_NO_PROBES` (solo las órdenes
propias del depurador):

```sh
sh tools/capture_m3_map.sh OUT          # 117966..149300; breakpoints in the script
python3 tools/extract_m3_recon.py OUT --report filesystem/mission3/s1/recon.json --dynamic-output filesystem/mission3/s1/dynamic.json
```

Resultado: 187 153 lecturas distintas del anillo del terreno, 225 547 del
tren, 14 540 barridos del jugador (01448C → 014492), 28 parches `0C70FC` y 0
sellos `0C88B0`. La captura se borró tras extraer (3 min de MAME).

**Secciones** (lista de cámara `0B8968`; la sección empieza cuando el
puntero de la lista pasa la orden indicada):

| Sección | Cámara | Fotogramas | Mapa del primer plano |
| --- | --- | --- | --- |
| 1 salida | 0..320 | 117966..118732 | `0B853A` |
| 2 vagones | 318..3391 | 118733..124728 (`0B8A50`, progreso 0x140) | `0B853A` |
| 3 hueco | 3391..3709 | 124729..126288 (`0B8BB8`, 0xD40) | `0B8600` (sin mosaicos ni colisión: FFFFFFFF) |
| 4 tramo final | 3709..6304 | 126289..144371 (`0B8C1C`, 0xE80) | `0B8550` |
| 5 jefe | 6304..6496 | 144372..149300 (`0B8CAC`, 0x18A0) | `0B8550` |

**Lo que estaba mal (datos) y se corrige:**

1. **Rejilla desplazada 8 filas (64 px).** La rejilla privada tenía el
   origen −8 (la ventana observada empezaba 64 px bajo la fila 0 del
   núcleo) y el paquete lo guardaba, pero el runtime (`neo_collision.c`,
   `cell()` de `neo_kernel.c`) lee la fila `y >> 3` y no mira el origen:
   todo suelo de la Misión 3 quedaba 64 px fuera de su sitio. Con el
   origen aplicado, las lecturas del arcade coinciden con la rejilla.
   `tools/rebase_collision.py` quita las 8 filas (vacías, comprobado) y
   `pack_mission.py` se niega a empaquetar un origen distinto de (0, 0).
   **Este era el problema del suelo de grupo 1 de los vagones** (fase 2):
   el jugador del port pisaba las filas 17/18 del paquete, que son las
   filas 9/10 del núcleo; en el arcade no pisa nunca una celda de grupo 1
   (de los 14 540 barridos, ninguno acaba en grupo 1; 17 en grupo 2 y 8
   en grupo 7). La repisa por la que sube (`0x0B`, fila 16 del núcleo) es un
   parche de `0B8FA8` (submapa `0B85EA`), dato dinámico que ahora se guarda
   (abajo); crearla en el juego es trabajo de actores (rama `work-m3c`, sin
   fusionar).
2. **Columnas más allá del mapa.** `0B8550` acaba en la columna 859; el
   hueco del anillo de las columnas 860.. conserva la columna c − 64
   (`0C714E` no recarga), y en el jefe el núcleo lee suelo `0x0B` en
   860..862 (celdas de 796..798). `rebase_collision.py --ring-tail 64`
   añade esas 64 columnas (rejilla 924 × 39).
3. **Filas del anillo del tren.** La ranura `q = ((~y) & 0x1F8) >> 3` del
   anillo de `10D7B6` guarda la fila de mapa `(q − 24) & 63`, no `q − 20`:
   la plataforma `0x2B` (fila 36) está en la ranura 60 y el `0xA0` de las
   filas 46/47 en las ranuras 6/7 (todas las lecturas de la misión y la RAM
   `10D83E` de `run4`, `+34 = 19` siempre). `neo_kernel_train_ring`
   (`neo_kernel.c`) lo corrige (antes: 32 px más abajo).

**Modelo del anillo comprobado** (`extract_m3_recon.py` se detiene si una
lectura no se explica): terreno, la ranura de la columna c guarda la
columna `first + ((c − first) & 63)`, `first = (−64 − X) >> 3` (X = `+14`
entero de la capa; no `− 1` como en la Misión 1), fila de núcleo `y >> 3`;
tren, la columna `((x − 16·(+32)) >> 3) mod 784`, y en el borde izquierdo
de la pantalla la ranura ya tiene la columna 64 más adelante (176
lecturas). Las 187 153 + 225 547 lecturas quedan explicadas.

**Prueba** (`tools/test_m3_map.c`): en `make check` con
`data/m3_map_fixture.json.gz` (133 KB: 18 824 lecturas del terreno
deduplicadas, 7 846 del tren, los 28 parches) contra `romfs/mission3.pack`
(`neo_collision_cell` y `neo_kernel_train_ring`, como el runtime), y la
sección `MSXR` del paquete (abajo); `make check-m3sweeps` además reproduce
los 14 540 barridos con el núcleo nativo, la rejilla del paquete, el tren y
los objetos dinámicos capturados (fixture privado
`filesystem/mission3/m3_sweeps.json.gz`, 0,7 MB).

| Sección | Columnas | Celdas ≠ 0 | Terreno (antes → ahora) | Tren (antes → ahora) | Barridos (antes → ahora) | Sobre objetos dinámicos |
| --- | --- | --- | --- | --- | --- | --- |
| 1 salida | 0..79 | 80 | 1472/1603 → **1603/1603** | 118/176 → **176/176** | 57,2 % → **346/346** | 0 |
| 2 vagones | 39..462 | 1102 | 6559/7914 → **7914/7914** | 1405/1632 → **1632/1632** | 52,4 % → **2285/2285** | 61 (`029CDC`) |
| 3 hueco | 423..502 | 137 | 437/481 → **481/481** | — | 35,8 % → **777/777** | 0 |
| 4 tramo final | 463..827 | 1232 | 6483/7594 → **7594/7594** | 5720/6016 → **6016/6016** | 87,6 % → **8668/8668** | 2363 (`0E6896`) |
| 5 jefe | 788..923 | 128 | 1142/1232 → **1232/1232** | 19/22 → **22/22** | 42,6 % → **2464/2464** | 98 (`0E0D00`) |

(Terreno y tren: lecturas distintas; «antes» = paquete y núcleo de
`0f13394` con los mismos parches. Barridos: con su multiplicidad.)

**Datos exportados** (privados, `filesystem/mission3/s1/` y
`romfs/mission3.pack`):

- `collision.json`: rejilla 924 × 39, origen 0, sin celdas desconocidas,
  145 mutables (las de los parches).
- `layers_rom.json` (`tools/export_m3_layers.py`): los mapas completos de
  ROM de cada objeto de capa, con colisión 2 × 2 por bloque y los colores
  de la RAM de paletas de `run4` por sección: primer plano `0B853A`
  (240 × 16 bloques, x 0), `0B8600` (vacío), `0B8550` (170 × 10, x 4160);
  tren `0B8566` (392 × 24); cielo `0B857C` (32 × 20, se repite cada 512 px;
  desde el progreso 0x140 el mapa es 0 y lo dibuja otro escritor);
  superposición `2A6142` (19 × 10) de `10E8C6` y los submapas `2A6158`,
  `2A616E`, `2A6184`, `2A619A` que el actor `0A6C8C` copia con `0C70FC`
  (listas en P2 `2A620E..2A62FA`). Colocación comprobada contra MAME: 1569
  + 948 bloques del primer plano, 448 del cielo y 115 de la superposición
  iguales; 0 distintos.
- `dynamic.json`: el movimiento del tren (`+14`, `+32` en cada cambio,
  14 317 puntos; nativo en `neo_train.c`), los 19 usos de objetos dinámicos
  (14 mapas de ROM: `0E5640` ×5 al inicio, hijos del tren y del terreno;
  restos `029C9A/029CDC/029D1E`; `0E112E` ×2; `0E53D4/0E53C2`; `0E6896`,
  la plataforma de 126245..141259; `0AAC94`, `0E4FF2`, `02C5BA/02C5C8`;
  jefe `0E0D00`/`0E0E36`) con su recorrido y los 28 parches (6 repisas de
  `0B8FA8`, submapa `0B85EA`).
- `stage.json` (`tools/export_m3_stage.py`): registro `0173F6`, descriptor
  `016E0E`, la lista de cámara descodificada (99 órdenes), las reglas de
  capa y la rejilla del paquete, las secciones.
- `romfs/mission3.pack`: el registro de etapa gana `recon_offset/length`
  (bytes libres del registro, `scene_assets.c` no los lee aún): bloque
  `MSXR` de 145 KB con los mapas de ROM (palabras de mosaico y colisión),
  parches, mapas dinámicos y secciones con sus paletas
  (`pack_mission.py recon_blob`). `test_m3_map` comprueba que la colisión
  de ROM del primer plano es la rejilla, la del tren `neo_kernel_train_cell`
  y los parches los capturados.

**Dibujos fuera de línea** (`tools/render_m3_sections.py`, 4 instantáneas
de `run4` por sección, imágenes en `build/m3_renders/`: MAME | ROM | SCB de
fondo de MAME): cada bloque de fondo de MAME contra el bloque de ROM en esa
posición.

| Sección | Primer plano | Tren (columnas) | Cielo | Superposición | Píxeles |
| --- | --- | --- | --- | --- | --- |
| 1 | 254/254 | 80/80 | 1120/1120 | — | 100 % |
| 2 | 508/508 | 80/80 | otro escritor (1060) | — | 100 % |
| 3 | 1011/1011 | 80/80 | otro escritor (1140) | — | 99,49 % |
| 4 | 355/355 | 80/80 | otro escritor (1160) | 81 en su sitio + 34 de los submapas | 98,84 % |
| 5 | 260/260 | 80/80 | otro escritor (1140) | — | 100 % |

Píxeles: el dibujo de ROM contra los SCB de fondo de MAME de las mismas
capas (sin el cielo del otro escritor). Lo que falta: en la sección 4 los
34 bloques de los submapas que el actor coloca; en las secciones 2 a 5, 41
de 1907 bloques del tren tienen el mismo código con otra paleta (filas de
otra columna del anillo del tren; solo cambian píxeles en la 3 y la 4).

**Límites** (sin cerrar, sin adivinar):

- El cielo desde el progreso 0x140 (mapa 0): otro escritor (probablemente
  `0B8EDC`, ver «Pendiente»); en el paquete siguen las celdas observadas.
- La superposición de `10E8C6`: los mapas están, la secuencia de parches
  del actor `0A6C8C` (y su movimiento vertical) no.
- Las celdas de mapa empaquetadas para dibujar (tabla de celdas) son las de
  antes (observadas; tren por posición); los mapas completos están en
  `MSXR` y `layers_rom.json`, sin uso en el runtime.
- La comprobación cubre lo que el piloto recorrió (las celdas que leyó el
  núcleo para cualquier actor); las celdas nunca leídas se comprueban solo
  contra la ROM (colisión de `0B853A`/`0B8550` igual a la rejilla).
- Los objetos dinámicos y las repisas necesitan sus actores nativos
  (`0B8FA8` y la caja `039382` están en `work-m3c`, sin fusionar).

## Paso 1: la M3 de principio a fin sin enemigos (rama `work-m3step1`)

Plan: [PLAN_MISION3.md](PLAN_MISION3.md), paso 1. Suelos que ponen los
actores, carga con el paquete nuevo y recorrido en ares.

**Fusión de `work-m3c`** (cajas `039382/03938A`, estampador `0B8FA8`): sin
su desfase de filas (`origin_rows`, `scene_stage_t.origin_y` fuera; la
rejilla empieza en la fila 0). `0C70FC` recorta a la ventana del anillo
(`neo_collision_window_clip`) y luego al submapa como en la M2
(`g_map_patch`); `data/m3ledge_fixture.json` va en filas del núcleo (fila
= 63 − fila del anillo): `test_neo_m3props`, 5 copias, 320 celdas iguales.

**Nativos nuevos** (`src/neo_m3props.c`, comentario de cabecera con cada
dirección):

- Vagones `038D1E/038D26` (llamadas `0B9148/0B916A`, (09D0/0AB0, 0048)):
  puertas `038B06` (al romperse: sonido 1029, bit del creador `+74`, el
  estampador `039562[+6E]`, caída con rebote), blancos `038F38` (al
  romperse: programa al azar y dos piezas `0390A6`), y `038D26` mantiene
  `10C700 = +3C − 58` (`0BC542`; lo lee `08A654` de `08A732`) hasta que se
  rompe una puerta (`0BC548`: 7FFF).
- Estampadores `0B90BC/0B90FA/0B9106/0B9112` (`+78` = 95/9E/A3/AC, `+7A` =
  8, programas `0B9040/0B9064`) sobre `0B8FA8`.
- Plataforma `0B9286` (llamada `0B925C`, (0FA0, 0030) de la raíz): va y
  viene entre x F60 y FA0 hasta que la cámara llega a F10, se queda en FA0
  (`10C6BD = 1`), mapa dinámico `0E6896` (`0140C0`), se borra en la cámara
  10F0; su pieza `0B93B4` (01AD6E a +C0) y el registro del guion `0B9400`
  (se borra con `10C6BD ≠ 0`). `0B9748` (puntos al saltar a la plataforma:
  lee `+15` del jugador, que el entorno de los actores no lleva) queda fuera:
  se crea como externo y se cuenta.

**Familias contra MAME** (capturas desde `states4/sec04_117966`, borradas
tras extraer):

```sh
export MSX_FULL_STATES=/home/user/captures-fullgame/states4 MSX_FULL_LOAD=sec04_117966 MSX_FULL_LOAD_FRAME=117966 MSX_FULL_VRAM_TRACE=1 MSX_FULL_GRID_TAP=0
MSX_FULL_FIRST_DUMP=120900 MSX_FULL_PERIOD=15 MSX_FULL_FAMILY_ROOTS=39382,3938a,38d1e,38d26 MSX_FULL_FAMILY_FROM=120900 MSX_FULL_LAST=122400 sh tools/run_capture_fullgame.sh W
MSX_FULL_FIRST_DUMP=126200 MSX_FULL_PERIOD=60 MSX_FULL_FAMILY_ROOTS=b9286,b9400 MSX_FULL_FAMILY_FROM=126200 MSX_FULL_LAST=141300 sh tools/run_capture_fullgame.sh P
R='--attacks --ram 10000e-100010 --ram 10c7b6-10d7b6 --ram 10d83e-10e83e'
python3 tools/extract_family_fixture.py W/boss-trace.txt --output data/m3wagon_fixture.json.gz --range 38b06-391e8 --range 39382-39470 --range b8fa8-b9148 $R --ram 10c700-10c702
python3 tools/extract_family_fixture.py P/boss-trace.txt --output data/m3platform_fixture.json.gz --range b9286-b9418 $R --ram 10c6bc-10c6be --ram 10e9d0-10e9d2
python3 tools/export_rest_sprites.py ZIP W@38b06-39470 P@b9286-b9418 --mask-auto --output filesystem/mission3/rest-native.json --keep <export anterior de la M3> --keep filesystem/mission2/rest-native.json --keep filesystem/rest-native.json
python3 tools/pack_mission.py 3 filesystem/mission3/s1 --loose-layers --report build/mission3_pack_report.json
```

| Familia | Nodos | Actualizaciones | Fotogramas | Resultado |
| --- | --- | --- | --- | --- |
| `m3wagon` (cajas, vagones, puertas, blancos, piezas, 6 estampadores) | 39 | 3515 | 120945..122187 | iguales a MAME, con los anillos de `10C72E` (parches) y del tren y `10C700` |
| `m3platform` (`0B9286`, `0B93B4`, `0B9400`) | 3 | 14506 | 126244..141260 | iguales, con `10C6BD` y `10E9D0` |

Para eso las pruebas de familia ganan `--train` (el tren `10D7B6` como
segundo objeto de terreno del núcleo, `+14..+20` leídos y su anillo RAM
`10D83E`: `neo_kscene_t.train_ring`) y `--group-map 0` (`10C00E = 0`:
`012970` no traduce el grupo; el mundo hace lo mismo en la lista `0B8968`).
Los actores genéricos ven `10D7B6` como el tren (`NEO_KOBJ_TRAIN`) cuando el
núcleo lo lleva, y en la M3 su terreno es siempre `10C72E`. Sprites:
`export_rest_sprites.py` con las raíces y paletas nuevas (523 107 píxeles
iguales, 59 distintos en 96 volcados; 44 volcados sin validar porque otra
figura con la misma paleta se autoanima); el export lleva ya los sprites y
paletas de la M1 (`check_mission_sprites.py` en `make check`, como el de la
M2) y `mission3.pack` las 6849 descripciones (3,0 MB, 317 trozos de 432).

**Carga** (`actor.c`, `scene_assets.c`): el bloque `MSXR` del paquete se lee:
sus 14 mapas dinámicos (los bytes de la ROM) se sirven a `ga_rom`
(`scene_stage_rom`, `ga_rom_set_extra`). Entrada: el registro de etapa +0E
es el registro de entrada de `019604` (A0); el de la M3 (`2D5E7C`) es de
tipo 2 (`0C15B4`, paracaídas desde y 0x100): el jugador empieza en x 0x48
sobre el suelo **sin el paracaídas (parada explícita registrada)**; la M2
(tipos 6/3, x 0x68) sigue con su entrada a pie. El límite derecho del
jugador usaba el ancho de la M1 (3840): ahora el de la etapa (cámara máxima
+ 320). RAM: el cartucho de referencia quedaba en 207 ranuras (mínimo 208):
`10A7` (transformación del jefe de la M1, 52 KB, una vez) se toca desde
`rom:/`.

**Recorrido** (`mslugx-m3-reference.z64`, `tools/test_mission3_ares.py
--hop --pilot`: Start enciende en la M3 el planificador de rutas de
referencia, `msx_autopilot.h`, que ahora ve también la plataforma `0B9286`
como suelo donde está; el mando dispara; resumen por sección):

| Sección | Progreso | Jugador x | Rescates | Muertes por caída |
| --- | --- | --- | --- | --- |
| 1 salida | 0..256 | 72..389 | 0 | 0 |
| 2 vagones | 320..3391 | 485..3527 | 0 | 0 |
| 3 hueco | 3456..3650 | 3590..3804 | 0 | 0 |
| 4 tramo final | 3712..6272 | 3807..6406 | 0 | 0 |
| 5 jefe | 6336..6465 | 6472..6601 | 0 | 0 |

(360 s, dos pasadas con 0 rescates; llega a la arena del jefe y la etapa
acaba al final de la lista de cámara por el gancho de referencia. En ares
llega a veces un segundo flanco de Start que apaga el planificador: la
prueba lo vuelve a encender y lo anota, «autopilot on again».) Con el mando ciego `--hop` (derecha y
salto cada 0,2 s, sin planificador) una de tres pasadas no tuvo rescates;
las otras, uno: el salto al hueco cuando la plataforma está lejos (hasta
88 px entre el suelo, que acaba en x 3808, y su borde) y una bajada por su
borde derecho (x 4263) que llega a la tierra de x 4272 ya 5 px por debajo
(pendiente: comprobar en MAME el paso plataforma → suelo). Paradas
explícitas del recorrido: la entrada en paracaídas, `0B9748`, los
registros de constructores no portados (enemigos, saltados y registrados),
`2869D6` y `0AF17E` como rutinas sin soporte del mundo, el guion parado en
`2DB148` (op 0x15) y 2493 dibujos sin descriptor (`descriptor=0 palette=2`).

## Paso 2: enemigos del camino (rama `work-m3step2`, en curso)

Plan: [PLAN_MISION3.md](PLAN_MISION3.md), pasos 2 y 6. Módulo nuevo
`src/neo_m3enemy.c` (traducción manual, cabecera con cada dirección):

- **Muerte de soldado sobre un nodo genérico** (`0406B6`/`0407B6`): la que
  usan casi todos los enemigos de la M3 (`jmp 0406B6`). Mismas entradas que
  `neo_soldier_ai.c` (`040DC0`, `040DD8`, `040A10`, `040A40`, `040A7C`,
  `040AF6`, `040B46`, `040BA8`, `040C8E`, `040D66`) y sus ticks `040DF0`,
  `040D06`, `040DB4`, `040AE0`, las salpicaduras `040F88/040F60/040F74`
  como actores genéricos y las llamadas de programa `040948/040956/04095E`.
  Paradas explícitas: `040920` (muerte en el suelo con `+74` en el aire) y
  `040BE6` (tabla de agachado: lee el byte alto de D1).
- **Granadero `08A732`**: casco `08AA6E`, lanzamiento (llamada de programa
  `08A9A4`), destello `08AB70`, obús `08AC54` (`03B332`, fila `08AF82`,
  explosión `01C706`), humo `08AE70`. Parada explícita: la salpicadura de
  agua `03C962` de `03C5C6` (no vista en la M3).
- **Soldados del escudo `09AE2C`/`09AE1E`/`09BA2A`** (y las variantes
  `09BA40/09BA68/09BA8A`): todos los estados de `09AE1E..09D62A` (andar,
  agacharse, disparar, levantar el escudo con su mapa `0140C0`, saltar,
  retroceder), el escudo `09C46C` (`+72`, poses `09D804`, el cuerpo sólido
  `004E76`), su capa `09C6B0`, los disparos `09C744/09C76C/09C780`, los
  destellos `09CAE8..` y el portador de objeto (`+90` bit 6, `08CDB2`).
  Paradas explícitas: `0B0626` / `04611C` (al romperse el escudo el nodo se
  convierte en soldado normal: falta pasar un nodo genérico al soldado
  nativo) y `044930` (`+90` bit 4, variante `09BA40`, no vista).

Arreglos compartidos: `0260C8` de tipo 2 (lista de ocho palabras al azar:
`neo_m3enemy.c`, `rank`), las llamadas de programa que escriben `+50`
(`neo_gactor.c`), `0143CC` (RAM `10C00A`, `ga_air_400a`) y `01495C` con D6
(`ga_probe_vertical_d6`). Pruebas de familia: `--root-words` (`01AD7C` lee
solo la palabra alta de la raíz), punteros de nodo con bytes pisados
(`--node-fields 72`) y la instancia viva primero al traducir un nodo.

**Familias contra MAME** (capturas A y B desde `states4/sec04_117966`,
borradas tras extraer; árbol de cada familia con un filtro de nodos):

```sh
export MSX_FULL_STATES=/home/user/captures-fullgame/states4 MSX_FULL_LOAD=sec04_117966 MSX_FULL_LOAD_FRAME=117966 MSX_FULL_VRAM_TRACE=1 MSX_FULL_GRID_TAP=0
MSX_FULL_FIRST_DUMP=118600 MSX_FULL_PERIOD=30 MSX_FULL_FAMILY_ROOTS=9ae1e,9ae2c,9ba2a,8a732,8da12,5d916,5c53c,5d226,4e990,97fcc MSX_FULL_FAMILY_FROM=118600 MSX_FULL_LAST=123850 sh tools/run_capture_fullgame.sh A
MSX_FULL_FIRST_DUMP=141000 MSX_FULL_PERIOD=30 MSX_FULL_FAMILY_ROOTS=8a732,569b4,569d0,569e6,97fcc,9ba2a,84588,9ae1e,9ae2c MSX_FULL_FAMILY_FROM=141000 MSX_FULL_LAST=144300 sh tools/run_capture_fullgame.sh B
R='--attacks --ram 10000e-100010 --ram 10c7b6-10d7b6 --ram 10d83e-10e83e --ram 10c2f6-10c2f8'
python3 tools/extract_family_fixture.py A/shield-trace.txt --output data/m3shield_fixture.json.gz --range 3f1f-ffffff $R   # y gren-trace, B
python3 tools/export_rest_sprites.py ZIP A@8a5d4-8afb6 A@9ae1e-9d630 B@8a5d4-8afb6 B@9ae1e-9d630 --mask-auto --output filesystem/mission3/rest-native.json --keep <export anterior> --keep filesystem/mission2/rest-native.json --keep filesystem/rest-native.json
python3 tools/pack_mission.py 3 filesystem/mission3/s1 --loose-layers --report build/mission3_pack_report.json
```

| Familia | Nodos | Actualizaciones | Fotogramas | Resultado |
| --- | --- | --- | --- | --- |
| `m3grenade` (granadero de la sección 2, muerte `040C8E`→`040D06`) | 2 | 213 | 121045..121615 | iguales a MAME |
| `m3grenade4` (los 4 granaderos de la sección 4: lanzamientos, obuses, humo) | 23 | 996 | 141202..142782 | iguales |
| `m3shield` (`09AE2C`, `09AE1E` ×2, `09BA2A` ×5, escudos, muertes) | 22 | 1938 | 118652..122134 | iguales |
| `m3shield4` (`09BA2A` ×3 de la sección 4) | 9 | 1033 | 141398..142343 | iguales |

Sprites: 7068 descripciones (+219), 56 423 píxeles iguales y 1058 distintos
en 89 volcados (los distintos, volcados con la actualización del nodo
anterior al volcado o con la figura ya cambiada); paletas sin captura:
`0x3C7/0x171/0x39F/0x3A3` (el soldado del escudo portador de objeto, `+90`
bit 6, no salió en las capturas). `mission3.pack` 3,18 MB, 342 trozos.

Recorrido en ares (`mslugx-m3-reference.z64`, `test_mission3_ares.py --hop
--pilot`, 300 s): PASS hasta la arena del jefe, 0 rescates; ranuras de caché
208 (mínimo 208). Paradas explícitas por sección (registros de constructores
no portados; antes del paso 2 entre paréntesis): salida 10 (11), vagones 17
(25), hueco 0 (0), tramo final 10 (17), jefe 0 (0); los nativos nuevos se
crean (`09BA2A` ×8, `08A732` ×5, `09AE1E` ×2, `09AE2C`). Rutinas sin soporte
del mundo: `0AF17E` ×11 y `2869D6` ×4 (como antes) y `040DB4` ×1 (muerte de
un soldado nativo de `neo_soldier_ai.c` con un tipo de golpe sin traducir
allí); 1827 dibujos sin descriptor (antes 2493).

**Segunda tanda** (misma rama):

- **Nodo genérico → soldier nativo** (`0B05FE`/`0B0626`/`0B0632`): el actor
  pide `become_soldier` y el mundo mueve el registro (`+00..+6D` con nombre,
  `+6E..+93` locales, programa y cursor) a un soldado libre,
  `neo_soldier_become` corre `0B05DC` y la entrada (`+7C` bit 7, `0B0750` /
  `0B063E`) y el soldado sigue en la misma actualización. Lo usan el escudo
  roto (`09B500`) y `08DA12`. Sin prueba de familia del tramo de soldado (el
  soldado nativo tiene sus propias pruebas); la familia llega hasta la
  conversión (la palabra de rutina `0B05FE` comparada).
- **Soldados de ventana `08DA12`/`08DA34`**: salto desde la ventana,
  aterrizaje, la rejilla de retardo `1067E6` (`00510C`) que los manda al
  soldado normal, disparo `08DBE8`. Paradas explícitas: la granada de los
  programas `23D976/28ABD4/23E82E` (`03BB8E` desde un nodo genérico) y
  `08DBC4` con `+90 = 0` (compara `+90` como palabra con el byte alto de D0
  tras `00427A`). Familia `m3window` (4 nodos, 237 actualizaciones) igual a
  MAME; la prueba de familia lee la rejilla `1067E6` (`--ram 1067e6-1068e6`).
- **Ondas `03C62A`**: RAM `10C2F6` vale 4 en la M3 (`gameplay.c`), la paleta
  `0x14C` (`019240` → `01930A`) entra en el export.
- **Guion**: las condiciones `0x14`/`0x15` (`01A7A2`/`01A7C8`: algún / ningún
  jugador montado, `0AAA88`) que paraban el guion en `2DB148`.
- **`neo_soldier_ai.c`**: las entradas de muerte `040B46` (tipos 3/4) y
  `040BA8` (tipo 5 en el suelo) que paraban en `040DB4`.

Recorrido en ares tras la segunda tanda (300 s): PASS, la lista de cámara
llega a 6496 (antes 6465: el guion ya no para en `2DB148` y crea los
registros del tramo final y el del jefe), 208 ranuras. Paradas explícitas
por sección: salida 10, vagones 12 (antes 18), hueco 0, tramo final 20 (los
registros que el guion parado no alcanzaba), jefe 1 (`04A2C2`, de la otra
rama). Rutinas sin soporte del mundo: `0AF17E` ×13, `2869D6` ×4, `08DBC8`
×1 (parada documentada de `08DA12`); `040DB4` desaparece. 1283 dibujos sin
descriptor (antes 1827).

**Tercera tanda** (misma rama):

- **Soldados `05D916` / `05D226` / `05C53C`** (`05C53C..05E918`, un solo
  registro para tres comportamientos): el paracaidista `05D916/05D924` (vaivén
  `05E19C`, puntería `05E388`, disparo `05E7D8` con las tablas `05EA38`, la
  vela `05DDA4` que sigue el `+86` del creador y le pasa los golpes, aterriza
  en `05C5B4`); el soldado de a pie `05C53C..05C5A0` (todos los estados
  `05C694..05D200`: andar, correr, girar, saltar, caer, apuntar con las tablas
  `05E9B8/05E9F8` y el disparo `05E800`, disparo agachado si el blanco se
  agacha, `01AE00`); `05D226/05D234` (anda hasta `+80`, salta o se para,
  dispara y en `05D732` lanza el casco y pasa a soldado nativo, `JMP 0B0632`);
  `05D76A` (saltos de `2CFEAA`); casco `05DD2A`, destello `05DF3E`, balas
  `05DF9A/05DFC2` (`05E03E`, explosión `01C706`); muertes `05DB18/05DB92/05DC32`
  (`JSR 0406B6/0407CA` y la rutina que deja en la misma actualización).
  Paradas explícitas: la salpicadura `03C962`, `01AE00` sin los nodos de los
  jugadores (`e->vehicle`), `0AAA88` con ranura de tipo 0.
- **Máquina voladora `04E990`** (`04E990..04FCA0`): piloto `04EF88` (paleta
  `A7/B9` por `01B2BE`; su golpe pone el `+8E` bit 0 del creador), suelo
  `04EE88` con el mapa dinámico `0E112E` (el suelo de `05C53C` en `2DAB20`),
  piezas `04EE18/04F2DC`, bombas `04F358`, misiles `04F44E/04F464` por el
  camino `0E1168` (`021288` con sus llamadas `04F97A..04F9B6`) y luego
  guiados (`04F9FA`, `002E7C`), humo `04F720`, la caída `04EB36..04EC78`
  (explosiones `01C706/01C71E`, seis piezas `04EC8C`, fila `0E111E`).
  `04F6F2` (golpe al creador de un misil guiado) no salta: corre `04FA8E`
  igual (sin `BRA`, al contrario que `04F5EA`).
- Prueba de familia: `--split-children` (`compile_family_fixture.py`): los
  hijos de una actualización repartidos en dos fotogramas (la marca de
  fotograma dentro del recorrido de la lista, como `--m2boss`).

| Familia | Nodos | Actualizaciones | Fotogramas | Resultado |
| --- | --- | --- | --- | --- |
| `m3para` (`05D916` ×4 con velas y disparos, `05D226`, `05C53C`, cascos, muertes) | 20 | 1167 | 120554..122417 | iguales a MAME |
| `m3pod` (`04E990` ×3: piloto, suelo, misiles, bombas, humo, piezas) | 323 | 11 270 | 122232..123832 | iguales |

Sprites: 7329 descripciones (+87 de `05C53C..05E918` y `04E990..04FFFF` en la
captura A); paleta sin captura `0xA7` (el piloto con sangre: la captura es sin
sangre, `0xB9`). Capturas A y B borradas tras extraer.

Recorrido en ares tras la tercera tanda (`mslugx-m3-reference.z64`, 300 s):
PASS, 0 rescates, 208 ranuras (3328 KB). El código nuevo dejaba la caché
44 KB bajo el mínimo (3288 de 3332 KB, aserción en `scene_assets.c`): la
explosión de muerte de los jefes `1032` (60 KB, una vez por jefe) suena desde
`rom:/` (`game_audio.c`). Con los enemigos nuevos disparando, el piloto ciego
(`--hop --pilot`) ya no llega a la arena del jefe en 300 s (progreso 5888;
en otras dos pasadas, de 420 s y de 300 s tras fusionar `ccr` con el paso 3,
4379: la cámara se queda en el bloqueo de 4380 del tramo final, con
`099DCC/04115A/0DC97E` vivos). Registros
saltados (constructores no portados) por sección: salida 0, vagones 10
(`02B202` ×4, `027F54`, `0391EA` ×2, `097FCC` ×2, `08AFB6`), hueco 0, tramo
final 7 (`0A4A94`, `0569D0` ×2, `02B202`, `084588`, `0A2F2E`, `097FCC`), jefe
no alcanzado. Rutinas sin soporte del mundo: `2869D6`, `0AF17E`, `08DBC8`
(paradas ya documentadas) y `0413D4` ×1 (nueva: un soldado nativo de
`neo_soldier_ai.c` en `0413B0..0413D4`, probablemente el que deja `05D226`
al pasar a soldado). Dibujos sin paleta: 4420 (la paleta `0xA7` del piloto
de `04E990` con sangre, sin captura).

Pendiente del paso 2 (en orden): `097FCC`, `0569xx`, `084588` (trazas en
`/home/user/captures-m3s2` A y B, sin borrar todavía), `02B202`, `027F54`,
`0391EA`, `08AFB6/0A2F2E`, `03EE8A`, `0214EA`, `01F1D4`, las paradas del
mundo `0AF17E` y `2869D6`, `08DBC8`, `0413D4`, `0B9748`, la entrada en
paracaídas, premios y ramas al azar.

## Pendiente

1. Recorrido: hecho hasta la arena del jefe (paso 1); falta la entrada en
   paracaídas (tipo 2), `0B9748`, el paso plataforma → suelo de x 4272, los
   enemigos (paso 2) y el jefe con el fin de misión (paso 4).
2. Capas: el cielo (`10E83E`) usa también `10E94E` desde el progreso 0x140
   (velocidad 0x40) pero su mapa pasa a 0; su escritor (probablemente el actor
   `0B8EDC` de `0B8D34`, tabla `0B8EC2`) y la superposición `2A6142` de
   `10E8C6`; los 7 valores de fondo. Las celdas empaquetadas del tren son las
   primeras vistas por posición; el bucle de columnas no se refleja en ellas.
3. Inventario completo de eventos y actores (233 constructores, plan por
   prioridades): [MISION3_ACTORES.md](MISION3_ACTORES.md). Familias nativas, por frecuencia: `097FCC` (sonido 10BA, `0985A0`, hijo
   `098E4A`), `09BA2A`, `02B202`, `08A732`, `08DA12`, `0A2F2E`, `05D916`, y
   las rutinas `0AF17E` (constructor de soldado) y `09A842` (trazas con
   `MSX_FULL_FAMILY_ROOTS` desde `states4/sec04_117966`, como
   [MISION2_ACTORES.md](MISION2_ACTORES.md)).
