# Compresión sin pérdida y datos repetidos (Fase 2b)

Objetivo (`docs/PLAN_ATAQUE.md`, Fase 2b): hacer sitio en ROM y en RAM para las
misiones 4–6 sin tocar un solo dato. Todo lo de aquí es sin pérdida: los datos
que recibe el juego son, byte a byte, los de antes (prueba en `make check`).
Solo medidas reales; las herramientas son `tools/measure_packs.py` (tamaños y
repetidos) y `tools/test_pack_compression.py` (la prueba).

## Resumen

| | Antes | Después |
| --- | ---: | ---: |
| `scene.pack` (Misión 1) | 7.580.928 B | **3.859.008 B** |
| `mission2.pack` | 3.884.800 B | **2.126.976 B** |
| `mission3.pack` | 6.538.880 B | **2.541.440 B** |
| Los tres paquetes | 18.004.608 B | **8.527.424 B (−9,48 MB, −53 %)** |
| Cartucho `mslugx-explorer.z64` | 59.260.928 B | **49.790.976 B (−9,47 MB)** |
| Tablas de la etapa en RAM, M3 | 897.888 B, 55 ranuras | **779.680 B, 48 ranuras** |
| Ranuras de caché (`MSX CACHE slots=`, suelo 208) | 208 | 208 |

El cartucho tiene tope de 64 MB: con 59,3 MB no cabía ni una misión más
(paquete + música ≈ 5–9 MB cada una); con 49,8 MB quedan 14,2 MB.

## Formato nuevo: léelo antes de reconstruir un paquete

- **`pack_mission.py` escribe la versión 5** (`MSXM`, palabra de versión 5). El
  cargador (`src/scene_assets.c`) lee las versiones 3, 4 y 5; el de antes de
  esta rama **no** lee la 5 ni los fragmentos LZ4: paquetes y programa van
  juntos.
- **Los paquetes dependen unos de otros.** `mission3.pack` toma de
  `mission2.pack` las páginas de actores que comparten (277 fragmentos). Hay
  que construirlos en orden y, si cambia `mission2.pack`, reconstruir también
  `mission3.pack`. Un solo comando lo hace todo:

  ```
  python3 tools/build_packs.py        # todas las misiones, en orden, a romfs/
  python3 tools/build_packs.py 3      # la 3 y las siguientes
  ```

  Las recetas de cada misión (carpetas de etapa, `--loose-layers` en la M3)
  están en `RECIPES` de ese fichero: una misión nueva se añade ahí.
  `pack_mission.py` a mano sigue valiendo con los argumentos de siempre (toma
  por defecto los `missionN.pack` de número menor que haya junto a la salida).
- Si el cartucho lleva un `mission3.pack` construido contra otro
  `mission2.pack`, el cargador **rechaza la misión** con un mensaje (parada
  explícita, tamaño del paquete en la cola `MSXX`), no dibuja páginas ajenas.
  `make check` carga los paquetes de `romfs/` con el cargador real y falla si
  no cuadran; si además no son los que dan las fuentes de ahora, avisa
  (`WARNING: ... python3 tools/build_packs.py`).
- `scene.pack` y `build/scene_meta.h` los regenera `make` solo (regla de
  `tools/pack_scene.py`, que ahora depende también de `pack_store.py`,
  `lz4_blocks.py` y `lz4opt.c`). Hace falta un `cc` en el anfitrión: el
  compresor `tools/lz4opt.c` se compila al primer uso en `build/host/lz4opt`.
- **Paquetes de `/home/user/ws-logo/romfs`: no se han cambiado todavía** (ver
  «Estado»): en ese árbol hay trabajo en curso con el cargador anterior, que
  no lee la versión 5. Con esta rama fusionada, `make` regenera `scene.pack`
  solo y los `missionN.pack` de versión 4 que hay allí **siguen cargando**
  (el cargador nuevo lee 3–5); pasan al formato nuevo con
  `python3 tools/build_packs.py` (≈30 s).

### Qué cambia en el fichero

Versión 5 = versión 4 con tres cambios; cabecera, registros de etapa, celdas,
imágenes, paletas, rejillas, bloques de ROM y `MSXD` son los mismos bytes en
los mismos sitios (las herramientas que leen esas tablas no cambian):

1. **Fragmentos LZ4** (también en `scene.pack`). El registro de fragmento
   sigue siendo `u32 offset, u32 length, u32 crc32, u32 palabra`; en la
   palabra, los bits 0..7 son el códec: 0 crudo, 1 pares (repetición, byte),
   **2 un bloque LZ4**. El CRC es el de los 16 KiB descodificados, el mismo de
   antes, y se comprueba en cada carga.
2. **Fragmentos prestados.** Bits 8..15 de la palabra: número de la misión
   cuyo paquete guarda el fragmento (0: este paquete); `offset` es entonces un
   desplazamiento en ese otro paquete. Las páginas del mapa no se tocan ni
   cambian de número; las páginas de actores propias conservan su orden y las
   prestadas van al final (cambian los números de página de las partes de
   actor, no sus píxeles).
3. **Cola `MSXX`** (últimos 64 bytes): `'MSXX' u16 paquetes u16 0` y, por
   paquete del que se toman fragmentos, `u16 misión u16 0 u32 tamaño u32
   CRC-32 del paquete entero`.

`tools/pack_store.py` es el único sitio donde se codifican y descodifican los
fragmentos (lo usan `pack_scene.py`, `pack_mission.py`, `prerender_stage.py`,
`check_boss_budget.py` y `measure_packs.py`); `MissionPack` lee las versiones
3–5 y resuelve los fragmentos prestados.

## 1. Medidas (antes de tocar nada)

Paquetes de `/home/user/ws-logo/romfs` a `4ffcac7`, `tools/measure_packs.py`.

### Paquetes por sección (bytes)

| Sección | `mission2.pack` | `mission3.pack` |
| --- | ---: | ---: |
| cabecera + etapas | 432 | 176 |
| celdas (22 B) | 312.246 | 185.746 |
| imágenes (8 B) | 89.568 | 43.104 |
| paletas (32 B) | 2.880 | 6.464 |
| tabla de fragmentos (16 B) | 5.856 | 13.136 |
| actores + partes + recursos | 42.444 | 116.760 |
| rejillas de colisión | 68.416 | 36.036 |
| bloques de ROM de etapa, `MSXR`, parches | 13.280 | 169.792 |
| `MSXD` (programas y bloques de ROM) | 48.236 | 128.574 |
| **fragmentos** | **3.301.440** | **5.839.040** |
| total | 3.884.800 | 6.538.880 |

`scene.pack` son solo fragmentos (1036, 7.580.928 B); sus tablas van
compiladas en el programa (`build/scene_meta.h`, ≈344 KB de `.rodata`,
`docs/MEMORY_OPTIMIZATION.md`). Las tablas son el 3–10 % de un paquete: lo
que pesa son los fragmentos, con el códec de pares (998 de 1036, 270 de 366 y
773 de 821 fragmentos; el resto crudos).

### Sistema de ficheros del cartucho (`build/game-romfs`, antes)

| Grupo | Bytes |
| --- | ---: |
| `audio/` (118 wav64: 12 músicas 15.495.096 B, 106 efectos) | 18.917.932 |
| paquetes (`scene`, `mission2`, `mission3`) | 18.004.608 |
| wav64 de cinemáticas y logo | 11.902.004 |
| películas `.msxc` (v2, LZ4) | 5.397.906 |
| columnas prerrenderizadas `.bgp` (`m2s2`, `m3s1`) | 1.992.336 |
| `marco/`, `fixed.bin`, `vehicles/`, resto | 787.006 |

### RAM por tabla al cargar una etapa (bytes, en las ranuras reservadas)

Lo que `scene_stage_load` toma (`take`), redondeado a 16; no incluye las
columnas prerrenderizadas (`scene_prerender_probe`).

| Tabla | M2 e1 | M2 e2 | M2 e3 | M3 e1 |
| --- | ---: | ---: | ---: | ---: |
| celdas, antes (22 B por celda) | 24.976 | 88.576 | 198.704 | 185.760 |
| celdas, ahora (8 B) | 9.088 | 32.208 | 72.256 | 67.552 |
| imágenes del mapa | 19.392 | 34.464 | 80.496 | 64.656 |
| índice de recorte (orden + tramos) | 11.360 | 40.272 | 90.320 | 84.448 |
| listas de tiras | 20.480 | 20.480 | 20.480 | 20.480 |
| rejilla + copia + anillo | 5.120 | 71.616 | 68.288 | 76.192 |
| actores + partes + recursos | 42.464 | 42.464 | 42.464 | 116.784 |
| `MSXD` + tabla de programas | 49.472 | 49.472 | 49.472 | 133.840 |
| paletas + tabla de fragmentos | 8.736 | 8.736 | 8.736 | 19.600 |
| bloques de ROM de etapa | 880 | 5.264 | 7.120 | 7.680 |
| **total antes** | 182.880 | 361.344 | 566.080 | 709.440 |
| **total ahora** | **166.992** | **304.976** | **439.632** | **591.232** |

(ares, M3 con sus columnas: 897.888 B en 55 ranuras antes, 779.680 B en 48
ahora.)

## 2. Datos repetidos (SHA-1)

| Qué | Total | Distintos | Repetido |
| --- | ---: | ---: | --- |
| Páginas (2048 B) de `scene.pack` | 8.288 | 8.284 | 4 vacías de relleno |
| Páginas de `mission2.pack` | 2.928 | 2.924 | 4 de relleno |
| Páginas de `mission3.pack` | 6.568 | 6.558 | 10 de relleno |
| **Páginas iguales entre paquetes** | | **2.220** | **4.546.560 B descodificados**: 2.212 entre M2 y M3 (actores), 8 entre M1 y M3, 0 entre M1 y M2 |
| Fragmentos enteros iguales M2–M3 (tal como estaban ordenados) | | 13 | |
| Teselas 16×16 alineadas | 284.544 | 102.545 | 152.850 vacías; 13.093 comunes a dos paquetes |
| Paletas de los paquetes de misión | 292 | 230 | 62 comunes a M2 y M3 (1.984 B) |
| Registros de imagen / de celda | 16.584 / 22.636 | 16.582 / 22.636 | 16 B |
| Programas `MSXD` | 588 | 579 | 1.080 B; ninguno común a dos misiones |
| Bloques de ROM `MSXD` | 78 | 78 | — |
| Rejillas de colisión | 4 | 4 | — |
| wav64 (125 ficheros, 30.819.936 B) | | | ninguno idéntico a otro ni contenido en otro |

Dentro de cada paquete no hay nada que deduplicar: los empaquetadores ya
guardan una sola vez cada página y cada paleta. Lo repetido está **entre**
M2 y M3: de las 2.216 páginas de actores de la M2, 2.212 están también en la
M3 (los actores comunes de `filesystem/missionN/rest-native.json`). Las
teselas repetidas son casi todas vacías: se las lleva el compresor. Paletas,
programas y registros repetidos no llegan a 4 KB: no se tocan.

Compresión posible de los fragmentos (cada uno por separado, como los carga
la caché):

| | Pares (antes) | LZ4 voraz | LZ4 `lz4opt` (guardado, con relleno a 64) | zlib −9 (no está en libdragon) |
| --- | ---: | ---: | ---: | ---: |
| `scene.pack` | 7.549.840 | 4.225.730 | 3.859.008 | 2.837.896 |
| `mission2.pack` | 3.293.242 | 1.708.144 | 1.544.128 | 1.113.744 |
| `mission3.pack` | 5.816.150 | 3.001.533 | 2.700.992 (1.841.600 propios + 859.392 que ya están en la M2) | 1.980.676 |

## 3. Qué se ha hecho, por bytes ganados y riesgo

### 3.1 Fragmentos en LZ4 (−8,62 MB de ROM)

- Compresor: `tools/lz4opt.c`, propio, formato de bloque LZ4 estándar
  (búsqueda por cadenas de hash y programa dinámico con el coste exacto de
  cada secuencia; determinista). `tools/lz4_blocks.py` lo compila y
  **descodifica cada bloque con un descodificador en Python antes de
  aceptarlo**; un fragmento que no encoge se guarda crudo.
- Descodificador: `scene_decode_lz4` en `src/scene_codec.h`, el mismo fuente
  en la consola y en las pruebas, con todas las lecturas y escrituras
  acotadas (un bloque mal formado se rechaza; `tools/test_scene_codec.c`).
  LZ4 es el códec de libdragon (`decompress_lz4_full_fast`, el del ELF y los
  assets); aquí se usa una versión acotada en C porque la de libdragon no
  comprueba el tamaño de la salida y los fragmentos se descodifican sobre
  ranuras de la caché.
- Sin coste por fotograma: un fragmento se descodifica al cargarse, como
  antes, en el mismo sitio (`decode_active`), con el mismo CRC después. El
  DMA lee la mitad de bytes. Medido en ares (`decode_max`, la descodificación
  más lenta del recorrido): 7127 → 6627 µs en M1→M2 y 7112 → 6899 µs en la M3.

### 3.2 Páginas de actores compartidas por referencia (−0,86 MB de ROM)

No hay `common.pack`: el paquete de la misión de número mayor apunta a los
fragmentos del de número menor. Regla (`pack_mission.py`, `SHARED_MIN`): un
fragmento de otro paquete con al menos 5 de sus 8 páginas dibujadas por los
actores de esta misión no se guarda otra vez. En la M3: 277 fragmentos de la
M2 (2.212 páginas usadas de 2.216); la M3 guarda 544 fragmentos propios y su
tabla sigue teniendo 821. El mapa no participa: sus páginas, sus fragmentos
y sus números son los de antes. Las misiones 4–6 tomarán de las anteriores lo
que compartan (hasta 5 paquetes por misión).

### 3.3 Celdas de la etapa: 8 bytes en vez de 22 (RAM)

Los registros de celda (22 B) se convertían en el sitio a `scene_map_cell_t`
(8 B), así que la tabla ocupaba 22 B por celda en las ranuras de la etapa.
Ahora pasan por un bloque temporal de 16 KiB (744 registros por lectura) que
se libera al acabar: M3 −118 KB (55 → 48 ranuras), M2 etapa 3 −126 KB. Esas
ranuras vuelven a la caché de actores mientras dura la etapa.

### 3.4 Lo que no se ha hecho (y por qué)

| Qué | Ganancia medida | Por qué no |
| --- | ---: | --- |
| Comprimir las tablas de los paquetes | ≈0,8 MB de ROM (LZ4: 541→246 KB en M2, 700→276 KB en M3) | las leen por desplazamiento fijo `prerender_stage.py`, `extract_m3_recon.py`, `export_m3_stage.py` y tres pruebas en C de la M3 (ficheros del otro agente); poco frente al riesgo |
| Comprimir las columnas `.bgp` | ≈1,4 MB (zlib 1.992.336 → 575.049) | se leen por DMA asíncrono durante el juego, una columna por ranura: descomprimirlas sí costaría por fotograma |
| Tablas de la Misión 1 fuera del programa (`scene_meta.h`) | ≈344 KB de RAM permanente | la Misión 1 se dibuja directamente de esas tablas; cambio arriesgado (se permitía dejarlo) |
| Audio | 0 | 30,8 MB en wav64 sin ficheros repetidos; VADPCM/PCM, nada que quitar sin pérdida |
| aPLib / Shrinkler en los fragmentos | ≈ +1,5 MB más | descodificación varias veces más lenta en cada carga de fragmento (`docs/MEMORY_OPTIMIZATION.md`, plan 6: el tiempo de descodificación no puede subir) |

## 4. Puertas

### Prueba de identidad (`make check`)

`tools/test_pack_compression.py`:

1. Construye, de las mismas fuentes, los paquetes **de antes**
   (`pack_scene.py --legacy`, `pack_mission.py --format 4`: fragmentos en
   pares, cada página en su paquete) y los **nuevos**.
2. Compila el cargador real, `src/scene_assets.c`, para el anfitrión
   (`tools/test_pack_loader.c` con `tools/host_n64/libdragon.h`: el cartucho
   son ficheros y el DMA una copia) y lo ejecuta sobre los dos juegos de
   ficheros: caché, `scene_mission_load`, `scene_stage_load` de cada etapa.
3. Vuelca todo lo que el cargador entrega al juego y compara los dos volcados
   byte a byte (9,1 MB de texto): cada página de `scene.pack`; por etapa, el
   registro de etapa, rejilla, listas de cámara, guion y jugadores, reglas de
   capas, parches, mapas dinámicos, cada celda con sus 8 imágenes, los
   patrones de animación, cada imagen del mapa (número de página, CRC de la
   página, posición y los 16 colores), paletas, cada actor con sus partes,
   recursos de paleta, bloques de ROM y programas `MSXD` orden por orden.
   Las partes de actor se comparan por el CRC de su página y sus colores, no
   por el número de página (lo único que la versión 5 renumera).
4. Carga los paquetes de `romfs/` igual (todo fragmento da su CRC, los
   paquetes prestadores son los esperados) y avisa si no son los de las
   fuentes.

Resultado: `PASS: ... the same 9078603 bytes of tables, images and pages from
the old packs (18003776 B) and the compressed ones (8527424 B)`.

Además, una sola vez y a mano: el cargador **de antes** (`2cee913`, con solo
los intercambios de orden de bytes para el anfitrión) sobre los paquetes de
antes da el mismo volcado que el cargador nuevo sobre los nuevos (md5
`8daaf27d…`), y `--format 4` / `--legacy` reproducen byte a byte los paquetes
que escribía el empaquetador anterior. Las columnas `m2s2.bgp` y `m3s1.bgp`
generadas de los paquetes nuevos son idénticas a las de los antiguos.

El cargador usa las tablas del paquete en el orden de bytes de la consola,
tal cual; para el anfitrión (little endian) `pack_native` las intercambia y en
la consola no genera código.

### ares 146

«Antes»: las ROM de `ws-logo` a `4ffcac7` con sus paquetes; «después»: esta
rama (`rm -rf build/m1end` y ROM nuevas). Xvfb propio (`:63`, `:64`).

`tools/test_m1m2_ares.py` sobre `mslugx-m1end.z64`: **PASS** antes y después
(todas las comprobaciones, 13 capturas).

| M1 → M2 (`mslugx-m1end.z64`) | Antes | Después |
| --- | ---: | ---: |
| `MSX CACHE slots=` (suelo 208) | 209, libre 473 KB | 209, libre 471 KB |
| Calentamiento antes del primer fotograma (49 fragmentos) | 288.187 µs | 274.455 µs |
| Carga de la etapa 1 de la M2 (tablas + 14 fragmentos de mapa) | 187.666 µs | 160.149 µs |
| Tablas de la etapa | 187.024 B, 12 ranuras | 170.560 B, 11 ranuras |
| Fotogramas en negro durante la carga / hueco mayor | 4 / 49.780 µs | 4 / 49.864 µs |
| Bytes leídos por PI en el recorrido (229 cargas) | 1.932.736 | 938.880 |
| `decode_max` | 7127 µs | 6627 µs |
| Fallos síncronos de caché (`sync`) | 0 | 0 |
| `heap_low` | 309 KB | 308 KB |

M3 por el menú de pruebas en `mslugx-explorer.z64` (L+R y A en SOLDIER
SELECT, MISSION 3), 150 s de juego con mando automático (derecha, disparo,
saltos): entra, carga y se juega sin aserciones ni `MSX ERROR` en los dos.

| M3 por el menú (`mslugx-explorer.z64`) | Antes | Después |
| --- | ---: | ---: |
| `MSX CACHE slots=` (suelo 208) | 208, libre 489 KB | 208, libre 487 KB |
| Carga de la etapa (tablas + 28 fragmentos de mapa) | 473.890 µs | 396.793 µs |
| Tablas de la etapa | 897.888 B, 55 ranuras | 779.680 B, 48 ranuras |
| Fotogramas en negro durante la carga / hueco mayor | 10 / 61.926 µs | 9 / 63.488 µs |
| `decode_max` | 7112 µs | 6899 µs |
| Fallos síncronos de caché (`sync`) | 0 | 0 |
| `heap_low` al final | 248 KB | 270 KB |

Los dos recorridos de la M3 no son el mismo fotograma a fotograma (el mando
va por tiempo real), así que cargas y desalojos del tramo jugado no se
comparan uno a uno (390 cargas / 2,92 MB antes, 370 / 1,33 MB después).
El programa crece 1.544 B de código y 128 B de bss (2 KB menos de montón
libre); las ranuras no cambian.

Repetidas las dos pruebas con `ccr` fusionado (`3bcb33f`, G25): M1→M2
**PASS** (209 ranuras, carga de la etapa 158.333 µs, `decode_max` 6664 µs,
`sync` 0); M3 por el menú: 48 ranuras de tablas, carga 397.272 µs,
`decode_max` 6797 µs, `sync` 0, sin aserciones. El número de ranuras de un
arranque (208 o 209) depende de cuándo se saltan las cinemáticas, no del
paquete: 208/209 antes y después.

## Estado y avisos

- **Para el agente de la Misión 3** (árbol `ws-logo`): tras fusionar
  `ccr`, (1) `make` regenera `romfs/scene.pack` y `build/scene_meta.h`
  (LZ4); (2) ejecuta `python3 tools/build_packs.py` para pasar
  `mission2.pack` y `mission3.pack` a la versión 5 (hasta entonces cargan
  los de versión 4, y el cartucho pesa 5,7 MB más); (3) desde ahí, cada
  vez que reconstruyas un paquete usa `build_packs.py` (o reconstruye
  también los de número mayor). No se han sobrescrito los paquetes de
  `ws-logo/romfs` desde esta rama porque el programa que se compila allí
  ahora mismo no leería la versión 5 (`bad header`) y su `scene_meta.h` no
  casaría con un `scene.pack` nuevo. Los paquetes nuevos ya construidos y
  probados están en `/home/user/ws-comp/romfs`.

- `scene.pack` y `mission2.pack` de `ws-logo` **no eran los de las fuentes
  actuales** antes de este trabajo: `data/soldier_programs.json` cambió
  después de empaquetarlos (el orden de las páginas nativas de `scene.pack`
  depende de él y no está entre las dependencias de la regla de `make`; el
  `MSXD` de la M2 tenía 562 B de más). Los reconstruidos sí lo son. Queda
  pendiente decidir si `data/soldier_programs.json` debe ser dependencia de
  `romfs/scene.pack` en el `Makefile` (regeneraría el paquete, 30 s, en cada
  cambio de programas).
- Tiempos del empaquetado: `pack_scene.py` 29 s (10 s antes),
  `build_packs.py` 28 s (15 s antes). La prueba añade ≈50 s a `make check`.
- `SCENE_MISSION_CHUNKS_MAX` (832) cuenta también los fragmentos prestados:
  la M3 sigue en 821.
- **Misión 4 y siguientes**: añadir la misión a `RECIPES` de
  `tools/build_packs.py` (y a `DESCRIPTORS`/`GROUPS` de `pack_mission.py`,
  como siempre). Su paquete tomará de `mission2.pack` y `mission3.pack` los
  fragmentos de actores que comparta; la prueba de `make check` la incluye
  sola en cuanto exista `filesystem/mission4/…/map.json`.
- Las pruebas de ares de esta rama usaron los Xvfb `:63` y `:64`; el agente
  de la M4 también arrancó en `:64` mientras tanto. Conviene repartir los
  números de pantalla.

## 5. El cartucho bajo el límite del registro de ares (audio PCM y películas)

### El problema, medido

- `mslugx-explorer.z64` con los paquetes privados de las seis misiones:
  **67.174.400 B**. ares 146 solo ofrece el IS-Viewer (el registro `MSX …`
  del que dependen todas las pruebas) si el **fichero de la ROM** mide como
  mucho `0x03FF0000` = 67.043.328 B: `ares/n64/cartridge/cartridge.cpp:50`
  (`if(rom.size <= 0x03ff'0000) isviewer.ram.allocate(64_KiB)`). Con una ROM
  mayor, `ares/n64/pi/bus.hpp:48` y `:99` dejan de desviar
  `0x13FF0000–0x13FFFFFF` al IS-Viewer (lecturas: ROM; escrituras: «ROM is
  too big so ISViewer is disabled») y la sonda de libdragon
  (`src/debug.c`, `isviewer_init`, escribe y relee `0x13FF0000`) no lo
  encuentra. **Es el tamaño del fichero, no la disposición**: dejar libre
  ese rango dentro del DFS no cambia nada; no hay forma limpia de evitarlo
  salvo medir menos. (En consola el tope es otro: 64 MiB = 67.108.864 B.)
- `romfs/sprites.bin` (50.331.648 B, la C-ROM cruda) **no va en el cartucho
  de lanzamiento**: solo en `mslugx-lab.z64` (`Makefile`, regla de
  `mslugx-lab.dfs`). El juego lee `build/game-romfs`. Lo que hay ahí
  (cartucho de 67.174.400 B = 64.812.384 B de ficheros + 394.400 B de DFS +
  1.967.616 B de cabecera, ELF comprimido y símbolos):

| Grupo | Ficheros | Bytes | Formato |
| --- | ---: | ---: | --- |
| `audio/music_*.wav64` | 24 | 27.752.856 | VADPCM estéreo 24 kHz |
| paquetes (`scene` + misiones 2–6) | 6 | 12.145.024 | fragmentos LZ4 (§3) |
| wav64 de cinemáticas y logo, PCM crudo 16 bit estéreo 32 kHz | 6 | 9.755.744 | `howto` 5.817.652, `title-motion` 1.429.560, `neogeo-jingle` 878.236, `entry` 798.056, `confirm` 454.188, `select-music-head` 378.052 |
| películas `.msxc` (v2) | 8 | 5.958.238 | LZ4 por paquete, analizador voraz |
| `audio/sfx_*.wav64` | 116 | 3.637.434 | 107 VADPCM, 6 PCM crudo mono (1.290.308 B) |
| wav64 de cinemáticas en VADPCM (`select-music-loop`, `confirm-cN`) | 4 | 2.184.556 | |
| columnas `.bgp` | 2 | 1.992.336 | |
| `chars/`, `marco/` (745 partes), `vehicles/`, `fixed.bin`, resto | 787 | 1.386.196 | |

### Opciones medidas

| Opción | Ganancia medida | Coste en ejecución | Decisión |
| --- | ---: | --- | --- |
| **Películas con el analizador óptimo `tools/lz4opt.c`** (el de los fragmentos) en vez del voraz de `msxc_compress.py` | 5.958.238 → 4.384.712 B (**−1.573.526**) | ninguno: mismos bloques LZ4, mismo reproductor, menos bytes por DMA | **hecha** |
| **PCM crudo de las cinemáticas sin pérdida (MSXL)**: predictores fijos + Rice, por bloques de 1024 muestras | 9.755.744 → 5.563.166 B (**−4.192.578**) | ≈2,0 ms de descodificación por bloque (32 ms de audio), en el tiempo libre de la cinemática; nada durante el juego | **hecha** |
| MSXL con predictor lineal (LPC ≤ 8, 12 bits) además de los fijos | 5.413.048 B (147.358 B menos que con solo fijos) | 8 multiplicaciones por muestra: medido 2,0–4,1 ms por bloque en vez de 1,6–2,1 | no: poco por mucho |
| FLAC `-compression_level 12` (referencia, ffmpeg) | `howto` 3.194.623 B (MSXL 3.351.290), `title-motion` 558.822 (619.238) | bloques de 4096 y LPC de orden 12 | referencia |
| Los 6 efectos en PCM crudo (`sfx_1053/109d/109e/10a5/10d5/1169`) como MSXL | 1.290.308 → 852.192 B (−438.116) | 5 de los 6 se leen de la ROM **durante el juego** (`streamed` en `game_audio.c`): la descodificación caería dentro del fotograma; el sexto (66 KB) es residente | no (pendiente si hiciera falta: decodificar adelantado como aquí) |
| Quitar la tabla de símbolos (`*.elf.sym`) del cartucho | 776.298 B | ninguno; se pierden los nombres en los volcados de pila de una excepción | no: depuración de los otros agentes |
| Cabecera de fichero del DFS (256 B por fichero + relleno a 256) | 394.400 B en 954 ficheros (745 son `marco/pN.sprite`) | habría que empaquetar las partes de Marco en un fichero y cambiar su carga | no |
| Partes de Marco comprimidas (`mkasset -c 1`, `asset_load` las abre igual) | 563.304 → ≈300.000 B (zlib −9) | el margen de la descompresión en sitio de libdragon (`(n>>8)+32` B por fichero) × 745 ≈ 30 KB de montón permanente = 2 ranuras de la caché | no |
| Música VADPCM | xz −9: ≈ −8 % (2,2 MB) | es estéreo real (tramas L = R: 0–3 %, 27 % en una intro corta); haría falta un descodificador de entropía en cada lectura | no |
| Tablas de los paquetes, columnas `.bgp` | ≈ 0,8 MB y 1,4 MB | ver §3.4 | no |
| Deduplicar o comprimir `sprites.bin` | 0 | no está en el cartucho | — |

### Resultado

| | Antes | Después |
| --- | ---: | ---: |
| wav64 PCM de las cinemáticas (6) | 9.755.744 B | **5.563.166 B** |
| películas `.msxc` (8) | 5.958.238 B | **4.384.712 B** |
| `mslugx-explorer.z64`, seis misiones | 67.174.400 B | **61.407.232 B (−5.767.168)** |
| Margen bajo el límite de ares (67.043.328 B) | −131.072 B | **+5.636.096 B** |
| Programa (`text` / `bss`) | 1.169.144 / 203.100 B | 1.175.288 / 203.228 B (+6,3 KB) |

### MSXL: formato y lectura

- `tools/msxl.c` (codificador, `tools/msxl_encode.h`) convierte un WAV64
  crudo de 16 bits (1 o 2 canales, muestras desde el byte 24 hasta el final)
  y **no escribe nada si al descodificarlo no salen los bytes del origen**;
  cualquier otro fichero (VADPCM) se queda como está. Lo aplica la regla de
  `build/game-romfs.stamp` a los `*.wav64` de la raíz del cartucho (las
  cinemáticas y el logo; `audio/` no). El formato completo está en la
  cabecera de `src/msx_pcm_codec.h`: bloques independientes de 1024
  muestras; por bloque, 1 bit de modo estéreo (izquierda/derecha o
  medio/lado) y, por canal, el predictor fijo de orden 0–4 que menos ocupa
  (las diferencias de Shorten/FLAC, sin multiplicar) con sus residuos en
  código de Rice (hasta 8 particiones con su parámetro), o las muestras
  crudas si nada gana; índice de bloques al final para los saltos. De 2385
  bloques del cartucho: 2273 en medio/lado; predictor de orden 1 en el 81 %
  de los primeros canales, orden 2 en el 13 %.
- `src/msx_pcm_codec.h` es el descodificador, el mismo fuente en la consola
  y en las herramientas y pruebas, con lecturas y escrituras acotadas (un
  bloque dañado se rechaza: silencio y `MSX ERROR pcm`).
- `src/msx_pcm.c` es una forma de onda del mezclador (`waveform_t`) que
  entrega **las mismas muestras en las mismas posiciones** que el WAV64
  crudo; `src/msx_audio.c` la usa cuando el fichero empieza por `MSXL`
  (si no, `wav64_open` como siempre: los cartuchos de prueba con WAV64
  crudos siguen valiendo).
- Coste y dónde cae: antes, cada bloque de 1024 muestras eran 4096 B leídos
  por DMA dentro de la mezcla (≈1,5 ms en ares con los tiempos de PI del
  cartucho); ahora son ≈2,3 KB leídos y ≈2,0 ms de descodificación. Para
  que eso no entre en el fotograma, `msx_audio_prefetch` (primera llamada
  del tiempo libre de cada fotograma de cinemática: `spare_time` y
  `logo_idle` en `main.c`, `title_intro.c`) descodifica por adelantado
  hasta dos bloques después del que suena, solo si quedan 4 ms antes del
  siguiente blanco; la mezcla entonces solo copia. Medido: de 215 / 350 /
  1421 bloques (logo / título / HOW TO PLAY), todos menos los 2–3 primeros
  de cada pista van adelantados.
- Memoria: 16,4 KB de montón (tres bloques descodificados + el leído)
  mientras suena una pista MSXL; se sueltan en `msx_audio_stop` (antes de
  que el juego mida el montón para la caché) y al cerrar. El lector (≈200 B)
  vive en el montón mientras el fichero está abierto.

### Pruebas

- `make check` → `tools/test_msxl.c` (compila `src/msx_pcm.c`, el lector de
  la consola, para el anfitrión con `tools/host_msxl/libdragon.h`):
  1. 308 señales sintéticas (silencio, extremos ±32768 en cada muestra y
     opuestos entre canales, ruido, tonos, longitudes 1…5121, mono y
     estéreo): siempre.
  2. Todos los WAV64 crudos privados (`filesystem/cinematics`, `logo`,
     `audio`): empaquetados y comparados **enteros**.
  3. Los ficheros MSXL que lleva `build/game-romfs` contra sus orígenes.
  Cada fichero se descodifica dos veces: bloque a bloque por el índice y
  con el lector de la consola haciendo las lecturas del mezclador (en
  orden con longitudes 1…3000 y bloques adelantados, 300 saltos, tras
  soltar los búferes). 6000 bloques dañados: rechazados o descodificados
  sin escribir fuera de los búferes.
  Resultado: `sources: 13 raw 16-bit WAV64 … 11392104 -> 6597604 bytes,
  every sample back`, `cartridge: 6 MSXL files, 9755744 -> 5563166 bytes,
  each one decodes to its source`, `PASS: MSXL`.
- Películas: `msxc_compress.py` descodifica cada paquete antes de aceptarlo
  (dos descodificadores: el suyo y el de `lz4_blocks.py`) y el reproductor
  sigue comprobando el CRC de cada paquete en la consola. `lz4opt.c` acepta
  ahora bloques de hasta 1 MiB y de longitudes distintas (los paquetes de
  una película); para los fragmentos de 16 KiB produce los mismos bytes
  (`scene.pack` reconstruido es idéntico al anterior, `cmp`).
