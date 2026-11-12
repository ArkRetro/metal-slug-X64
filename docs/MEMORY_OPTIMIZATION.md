# Optimización de memoria y recursos (RAM N64 y ROM)

Solo medidas reales (ares 146, `mips64-elf-size`, `nm`, tamaños de ficheros DFS,
líneas `MSX MEM` / `MSX CINEMATIC`). Lo que es estimación se marca así.
Contexto: `docs/RENDIMIENTO.md` (presupuesto de la caché de escena, reservas,
margen) y `docs/AUDIO_JUEGO.md` (efectos residentes).

## Fase 1: auditoría (rama `work-ram`, base `f30a292`)

### RAM: imagen del programa (`mslugx-explorer.elf`)

| Sección | Base | Notas |
| --- | ---: | --- |
| texto | 826 KB | `neo_boss` 47 KB, `neo_world` 39 KB, `neo_soldier_ai` 29 KB, `gameplay` 22 KB; código propio de la misión 2 = 57 KB |
| `.rodata` | 1.18 MB | `neo_world.o` 592 KB (programas de animación: 460 KB de comandos de 16 B + 115 KB de direcciones ROM que el juego no lee), `scene_assets.o` 344 KB (`scene_meta.h` de la misión 1: imágenes 111 KB, celdas del mapa 98 KB, imágenes del mapa 55 KB, descriptores 44 KB), `neo_region.o` 96 KB, `neo_boss.o` 40 KB |
| bss | 158 KB | `game` 75 KB, `fade_copies` 16 KB, `dynamic_grid_m1` 15 KB |
| **total** | **2161 KB** | todo residente: el ELF se carga entero en RDRAM |

### RAM: montón por subsistema (cartucho de lanzamiento, 8 MiB, `MSX MEM`)

| Subsistema | KB | Vida |
| --- | ---: | --- |
| Pantalla (2 framebuffers 320×240×16), mezclador, rdpq | 452 | permanente |
| Logo NEOGEO + preparación del título durante el logo | pico 3832 usados | temporal |
| Título: película en caché 3074 + 16 fotogramas preparados 2128 | ≈5200 | temporal (se libera) |
| HOW TO PLAY: en streaming desde la ROM (4,77 MB > límite de caché 4 MB) | ≈100 | temporal |
| Select / confirm / entrada: caché 709 / 317 / 641 + preparados 3192 / 931 / 1330 | ≤3900 | temporal |
| Audio del juego: efectos residentes (`msx_ramfs`, 851.270 B) + estado | 993 | permanente |
| Caché de escena | todo lo que queda (ver RENDIMIENTO) | permanente; tablas de fase de la misión 2 en ranuras reservadas |
| `actor_init` (poses de Marco, ROM S del HUD + tira, índice del mapa, fuente) | 615 | permanente |
| Crecimiento en juego (`heap_low`) | ≤ 260 (medido antes) | reserva 320 |

No se han visto fugas: tras cada cinemática el montón vuelve a 469 KB usados
(`title done` = `entry done` = 469 KB). La caché de escena reparte sus ranuras
en los bloques que haya (fragmentación controlada).

### ROM (cartucho 33,8 MB, `build/game-romfs`)

| Fichero | Bytes | Formato | Redundancia que queda (gzip -9 / xz) |
| --- | ---: | --- | --- |
| `scene.pack` | 7.008.512 | bloques de 16 KiB con códec propio | 2,41 MB / 1,68 MB |
| `howto.wav64` | 5.817.652 | PCM 16 bit estéreo 32 kHz | estéreo real (57 % de muestras L≠R) |
| `howto.msxc` | 4.770.420 | delta+RLE RGBA5551 | 1,08 MB / 0,61 MB |
| `title.msxc` | 3.147.898 | idem | 0,64 MB / 0,49 MB |
| `audio/` (música misión 1 + 49 efectos) | 3.359.470 | VADPCM 24 kHz | — |
| `mission2.pack` | 1.970.752 | como `scene.pack` | 0,56 MB / 0,35 MB |
| `title-motion.wav64` | 1.429.560 | PCM 32 kHz estéreo | |
| resto de películas (select, entry, confirm) | 1.706.114 | delta+RLE | ≈ 4,5:1 |
| resto de audio de cinemáticas y jingle | 2.355.000 | PCM 32 kHz estéreo | |

### Los cinco problemas mayores (por impacto)

1. **Imagen del programa 2161 KB**, de ella 1,18 MB de tablas en `.rodata`;
   cada misión nueva la hacía crecer y quitaba ranuras a la caché (fallo
   «RAM budget 3303 KB < 3328 KB», margen bajado a 160 KB).
2. **Películas de las cinemáticas: 9,6 MB de ROM** con 4–5× de redundancia
   que el códec delta+RLE no aprovecha; y en RAM, película entera en caché +
   fotogramas preparados (pico ≈5,2 MB durante el título).
3. **Efectos de sonido residentes: 851 KB** de RAM permanente (decisión del
   audio contra la contención del PI; ver AUDIO_JUEGO).
4. **Audio de las cinemáticas en PCM sin comprimir: ≈9,6 MB de ROM**
   (`howto.wav64` 5,8 MB). VADPCM tuvo distorsión; no es mono duplicado.
5. **`scene.pack` 7 MB** (una misión) con ≈3× de redundancia; cada misión
   añade un pack (≈2 MB) y su música (≈2,4 MB).

## Plan (impacto / riesgo / dificultad)

| # | Acción | Impacto | Riesgo | Estado |
| --- | --- | --- | --- | --- |
| 1 | ELF: comandos de animación de 12 B, sin direcciones ROM de soldado en N64, `-Os` en código frío, margen 256 KB | −309 KB RAM permanente | bajo (mismos valores; `make check`) | **hecho** |
| 2 | Películas MSXC v2: LZ4 por paquete, descomprimidas en la carga (mismas tramas) | −4,2 MB ROM | bajo (CRC de cada paquete en el dispositivo) | **hecho** |
| 3 | Películas: mantener la caché comprimida en RAM y descomprimir cada fotograma | −1,7 MB del pico del título, permite cachear HOW TO PLAY | medio (tiempo por fotograma; medir `late`) | pendiente |
| 4 | Efectos residentes (851 KB) | — | — | se conservan (decisión del proyecto) |
| 5 | Datos de misión siempre en `missionN.pack` (ranuras reservadas), nunca en `.rodata` | evita +100–300 KB por misión | bajo | regla (RENDIMIENTO) |
| 5b | Programas y bloques de ROM de los actores de una sola misión (grupos `m2`/`m3` de `data/soldier_programs.json` y `data/region_rom.json`) en el bloque `MSXD` del paquete (versión 4), cargados en las ranuras reservadas de cada etapa (`scene_stage_program`, `scene_stage_rom`) | M2: 141 programas, 3808 órdenes, 13 bloques 5,7 KB (`MSXD` 45,7 KB); M3: 117 programas, 2008 órdenes, 17 bloques 10,6 KB (32,3 KB); imagen −30,8 KB **con** el jefe intermedio dentro (2 061 872 → 2 031 056 B): 210 ranuras (antes 208 sin él) | bajo (mismos valores; `make check` los compila en el anfitrión) | **hecho** |
| 6 | `scene.pack` y paquetes de misión: fragmentos en LZ4, páginas de actores compartidas, celdas de etapa de 8 B (`docs/COMPRESION.md`) | −9,5 MB ROM medidos; `decode_max` 7,1 → 6,9 ms | medio (prueba de identidad en `make check`) | **hecho** |
| 7 | PCM de las cinemáticas sin pérdida (MSXL) y películas con el analizador LZ4 óptimo (`docs/COMPRESION.md` §5) | −5,77 MB de ROM (67,17 → 61,41 MB con seis misiones); programa +6,3 KB; 16,4 KB de montón solo mientras suena una pista MSXL | bajo (prueba de identidad en `make check`; mismas cadencias y CRC en ares) | **hecho** |

### Perfil de 4 MiB (no se persigue)

Decisión del proyecto: se conservan todas las funciones del Expansion Pak
(efectos residentes, preparación de cinemáticas, caché de escena grande,
precarga, logo). Con ellas no cabe en 4 MiB: solo imagen (1852 KB), pila,
pantalla (452), `actor_init` (615) y la caché mínima de 4 MiB (512) dejan
≈601 KB, menos que los efectos residentes (851 KB). No se trabaja ese perfil.

## Resultados medidos

### 1. Imagen del programa (commit `38ceb91`)

| | Antes | Después |
| --- | ---: | ---: |
| texto / datos / bss (B) | 826.360 / 1.228.836 / 157.884 | 753.240 / 985.180 / 157.884 |
| total | 2161 KB | **1852 KB** |
| caché de escena (margen) | 3303 KB con 192 (falla) | **3536 KB = 221 ranuras con 256** |
| libre tras cargar el juego | — | 624 KB |

Recorrido completo de la misión 1 en ares (`mslugx-reference.z64`): PASS
antes y después de v2 (`crash=0`), mínimo de montón libre `heap_low` 369 /
367 KB, peor fotograma 39,2 / 38,0 ms (antes de la rama: 35,9 ms en
RENDIMIENTO; mismo rango que las ventanas de arranque del jefe).

### 2. Películas MSXC v2

| Película | v1 (B) | v2 (B) |
| --- | ---: | ---: |
| `howto.msxc` | 4.770.420 | 3.188.106 |
| `title.msxc` | 3.147.898 | 1.387.344 |
| `select.msxc` | 725.760 | 325.848 |
| `entry.msxc` | 656.026 | 308.814 |
| `confirm.msxc` | 324.328 | 187.794 |
| **cartucho `mslugx-explorer.z64`** | **33.832.960** | **29.835.264 (−4,0 MB)** |

Formato: cabecera y tabla de desplazamientos v1 intactas (describen la
disposición descomprimida), más una tabla de paquetes guardados; cada paquete
es un bloque LZ4 o el paquete v1 tal cual si no gana
(`tools/msxc_compress.py`, que verifica el viaje de ida y vuelta). El
reproductor (`src/cinematic.c`) descomprime cada paquete con
`decompress_lz4_full_fast` de libdragon **en el sitio**: los bytes guardados
se leen por DMA al final del hueco del paquete (en la caché, el sitio aún
libre de los paquetes siguientes más una cola de `(max>>8)+80` B; en
streaming, la cola del búfer de paquete) y se descomprimen hacia su
desplazamiento v1. Sin búfer intermedio ni tabla en RAM (las dos entradas de
la tabla se leen con el paquete): una primera versión con búfer y tabla
partía el montón y SOLDIER SELECT no podía reservar sus fotogramas
preparados. La carga va de un paquete por paso, con presupuesto de tiempo
libre (60 µs/KB hasta medirlo). El resto (CRC de cada paquete, delta,
fotogramas preparados) es el de v1: fotogramas idénticos; un error de
descompresión aborta la película por CRC. Si el montón está partido,
`select_heavy` quita los fotogramas preparados más ligeros hasta que el
bloque cabe (antes fallaba la película entera).

RAM: sin cambios (la caché guarda los bytes v1). Cadencia en ares (arranque
completo del cartucho de lanzamiento, `MSX ... cadence`):

| | v1 | v2 |
| --- | ---: | ---: |
| Logo: fotogramas tardíos | 3 | 2 |
| Título: únicos / perdidos | 601 / 0 | 601 / 0 |
| HOW TO PLAY (streaming): saltados | 42 | 35 |
| SOLDIER SELECT: saltados (preparados) | 10 (24) | 6 (18, montón partido) |
| Confirmación: saltados (preparados) | 2 (7) | 4 (13) |
| Entrada: saltados | 0 | 0 |
| Montón tras cada cinemática | 469 KB usados | 469 KB usados |

### 3. Compresión del código en ROM

El ELF ya iba comprimido (`N64_ROM_ELFCOMPRESS` 1, LZ4, por defecto de
`n64.mk`). Medido con `n64elfcompress` sobre el ELF sin símbolos (1.739.560 B):

| Nivel | ROM | Resultado |
| --- | ---: | --- |
| 0 (sin compresión) | 1.740.072 | |
| **1 (LZ4, actual)** | **905.840** | arranca (`MSX MEM display` a 479 ms) |
| 2 (aPLib) | 612.648 | **no arranca** en ares con la libdragon fijada (ni la línea del logo); se descarta |
| 3 (Shrinkler) | 546.848 | no probado (más lento que 2) |

Comprimir el código solo ahorra ROM: se descomprime entero en RDRAM al
arrancar. Las palancas de RAM son las de la sección 1 (`-Os` en código frío,
tablas fuera del ELF) y, si un día hacen falta, módulos por misión (DSO de
libdragon, comprimidos en ROM con `N64_ROM_DSOCOMPRESS`, cargados al cambiar
de misión): hoy el código propio de una misión son ≈57 KB (≈52 KB con `-Os`)
y la holgura medida (13 ranuras, 208 KB con margen 256) cubre ≈4 misiones así.

## Suelo de la caché de escena: 200, no 208 (medido el 11-XI, `work-m4`)

Pregunta: ¿puede volver `SCENE_EXPANDED_MIN_SLOTS` de 200 a 208? **No con el
programa de hoy**: la caché recibe 205 ranuras en todas las misiones, así que
un suelo de 208 pararía el arranque (`RAM budget: 205 scene slots allocated
(minimum 208)`). Se deja en 200.

Medidas (ares 146, 8 MB, cartucho normal `mslugx-explorer.z64` de `b9e8ca2`;
M1 por el camino normal con `mslugx-m1end.z64`, el resto por el menú de
pruebas; `MSX CACHE slots=` al cargar y el mínimo de `heap_low=` de las líneas
`MSX RDP` del paseo):

| Misión | Ranuras | Libre tras cargar | `heap_low` mínimo | Paseo |
| --- | ---: | ---: | ---: | --- |
| M1 (→ M2, `test_m1m2_ares.py`) | 205 | 483 KB | 312 KB | fin de la M1 y 20 s de la M2 |
| M2 etapa 1 | 205 | 483 KB | 320 KB | 25 s |
| M3 | 205 | 483 KB | pendiente (la pulsación del menú se perdió; se repite) | — |
| M4 | 205 | 483 KB | 280 KB | 300 s, cámara 0..1256 con disparo |
| M5 etapa 1 | 205 | 483 KB | 393 KB | 25 s |
| M6 etapa 1 | 205 | 483 KB | 320 KB | 25 s |

Las ranuras son las mismas en todas porque la caché se dimensiona **una vez**,
al acabar el arranque (`scene_assets_begin`): con el audio cargado quedan
3893 KB libres de un montón de 5880 KB, se apartan la reserva de juego y de
crecimiento (`SCENE_RESERVE_GAME_KB` 672 menos lo precargado +
`SCENE_RESERVE_RUNTIME_KB` 320) y el margen (`SCENE_HEAP_MARGIN_KB` 256), y lo
que queda se divide por el coste de una ranura (16 384 + 12 bytes): 205.
Después de cargar quedan 483 KB libres (presupuesto 576).

Por qué no llega a 208:

- Harían falta 3 ranuras más = **49 KB** más de montón libre en ese punto
  (3941 KB en vez de 3893).
- El montón es lo que queda de los 8 MB tras el programa (`text + data + bss`
  del ELF: 2 300 KB hoy). Cuando se fijó el suelo de 208 (`906e3f1`) el montón
  era mayor; desde entonces el programa ha crecido con el código y las tablas
  enlazadas de las misiones 4, 5 y 6 (accesorios de la M4, voladores,
  helicópteros, cámaras y capas genéricas, menús de prueba): cada 16,4 KB de
  programa es una ranura menos. Solo las familias de esta sesión (helicópteros
  de la M4, soldado `0AF1A8`, piezas, destello) bajaron el montón de 5891 a
  5880 KB.
- Tampoco se puede sacar de la reserva sin medir más: con 3 ranuras más
  quedarían 434 KB libres tras cargar, por debajo del mínimo que comprueba
  `scene_assets_check_budget` (320 + 128 = 448 KB), y el `heap_low` de la M4
  (280 KB) bajaría a 231 KB. Los jefes de las misiones 4 a 6 y sus cargas de
  música no están medidos todavía (no existen en el port): no hay dato para
  recortar el margen.

Qué lo devolvería (ninguna hecha aquí): `-Os` en los módulos fríos de las
misiones (≈ 5 KB por misión, sección 3), sacar del ELF las tablas que aún van
enlazadas, o módulos por misión (DSO) cargados al cambiar de misión: con los
de las misiones 2 a 6 fuera del programa residente se recuperan bastante más
de 49 KB. El suelo debe revisarse cuando exista el jefe de la M4 (su
`heap_low` con la música `0x2C`).

**Segunda medida (11-XI, tras el prisionero que pelea y los animales, `2b9dca8`)**: la caché baja a **202
ranuras** (`MSX CACHE slots=202 (3232 KB, floor 200) free=489 KB`, M4 por el menú de pruebas, `heap_low` mínimo
285 KB en 300 s hasta la cámara 2160): el programa creció ≈ 45 KB con la fusión de la M5 y el código nuevo
(ELF 2 344 936 bytes). Quedan **2 ranuras (≈ 33 KB de programa) hasta el suelo de 200**: el jefe de la M4
(≈ 3000 líneas de 68K) no cabe sin recuperar memoria antes (`-Os` en los módulos de misión, tablas fuera del
ELF o módulos por misión). El paquete de la M4 tiene ahora 976 trozos (límite 1024).

**Tercera medida (11-XI, `54adba8`: con `ccr` de los personajes y la pausa fusionado)**: **201 ranuras**
(`MSX CACHE slots=201 (3216 KB, floor 200) free=486 KB`, ELF 2 362 984 bytes; M4 por el menú, `heap_low` 287 KB).
Queda **una** ranura (≈ 16 KB de programa) antes de que el arranque se pare por el suelo.

## Programas bajo demanda (11-XI, `work-m4`): la caché vuelve a 208 con margen

Medida de partida (`mslugx-explorer.elf` de `54adba8`, `mips64-elf-nm -S`): texto 1185 KB, datos de solo lectura
881 KB, bss 204 KB. De los datos, **308 KB eran los 928 programas de animación sin grupo** (`soldier_program_N`, 32 460
órdenes de 10 bytes) más 15 KB de su tabla; después `scene_images` 122 KB, los bloques de ROM sin grupo 99 KB (174;
el mayor, las tablas de poses del SV-001 `291CEA`, 44 KB), `scene_map_images` 55 KB, `scene_native_descriptors` y
`marco_poses` 49 KB cada uno.

Cambio (sin tocar valores ni lógica): los programas sin grupo salen del ELF y van al sistema de ficheros del cartucho.

- `tools/compile_soldier_ai_tables.py` escribe `build/programs.bin` (`MSXP`, versión 1; las órdenes en los registros
  de 10 bytes de `MSXD`, las colas compartidas igual que antes) y, en la imagen, una referencia por programa
  (`soldier_program_refs`: etiqueta, desplazamiento, número de órdenes; 12 bytes, en orden de etiqueta). El Makefile
  copia el fichero a `build/game-romfs` (todos los cartuchos del juego).
- `src/neo_progstore.c` (portable): lee un programa la primera vez que se pide, lo convierte en el sitio y lo guarda
  en una arena fija; lo que no cabe toma un bloque del montón (contado en el log). `src/program_file.c` (consola):
  `/programs.bin` por DMA (`dma_read`, que espera a la transferencia en curso) y la arena en bss
  (`PROGRAM_ARENA_KB`, 112), así la caché de escena se dimensiona con ella ya descontada.
- `neo_world.c` `program()`: en consola, búsqueda binaria en las referencias y lectura; en el anfitrión, las tablas
  de siempre (las pruebas no cambian).
- **Vida de los punteros**: todo puntero a un programa vive dentro de `msx_game_t` (mundo, sprites de CONTINUE,
  aterrizaje, fin de misión); `msx_game_init` lo borra entero, así que ahí se olvidan los programas leídos
  (`neo_world_programs_reset`): al cargar cada etapa y al volver a la Misión 1. Lo que se conserva entre etapas
  (HUD, flujo, RNG, armas, premios, banderas, fundido) no lleva punteros a programas (revisado tipo a tipo).
- Prueba (`make check`): `tools/test_program_file.c` lee los 928 programas por el almacén (arena de 64 KB: 675 en
  bloques del montón; dos pasadas con borrado) y compara cada orden con la tabla: iguales.

Medidas (ares 146, 8 MB):

| | Antes (`54adba8`) | Después |
| --- | ---: | ---: |
| `mslugx-explorer.elf` texto + datos + bss | 2 362 984 B | **2 180 920 B** (con el vehículo `0587C4` dentro) |
| Caché de escena (M4 por el menú) | 201 ranuras | **212 ranuras** (213 por el arranque normal; suelo otra vez 208) |
| Libre tras cargar | 486 KB | 488 KB (`heap_low` 277 KB en 150 s de la M4, 308 KB en el fin de la M1 y la carga de la M2) |

Uso de la arena: **Misión 1 entera** (`mslugx-reference.z64`, `test_mission_native_ares.py`, PASS: jefe, fin de
misión y recuento) 237 programas, pico entre 96 y 104 KB, `heap_low` 273 KB, peor fotograma 42,3 ms; **Misión 4**
hasta la cámara 2160: 120 programas, 42 560 bytes; al volver a la M1 tras el *game over* el log da `MSX PROGRAMS
reset` y la M1 vuelve a leer los suyos. Tiempo de lectura: 13,9 ms en total para 120 programas (media 0,12 ms), la
más larga 0,82 ms. La arena de 112 KB deja ≥ 8 KB sobre el pico medido de la M1; lo que no quepa cae al montón
(línea `heap blocks` del log), nunca falla.

**Lo que no se hizo** (medido, pendiente): los tres bloques de ROM grandes sin grupo (`291CEA` 44 KB y `2B0386`
7,5 KB del SV-001, `02CAD6` 7,5 KB de nombres del recuento) con el mismo mecanismo darían 59 KB menos de imagen, pero
en la M1 (donde se usan todos) habría que subir la arena casi lo mismo: ganancia neta ≈ 25 KB; los datos de escena
propios de la M1 que siguen enlazados (`scene_map_images` 55 KB, `scene_map_cells_data` 36 KB, `neo_grid` 15 KB,
`dynamic_grid_m1` 15 KB de bss: 121 KB = 7 ranuras en las misiones 2 a 6) pedirían cargar la M1 como las demás
(tablas en ranuras reservadas). `PROGRAM_ARENA_KB` y el suelo se revisan cuando exista el jefe de la M4 (≈ 3 ranuras
de código por las 3000 líneas de 68K, como `neo_m3boss.o`, 30 KB).

`SCENE_MISSION_CHUNKS_MAX` pasó de 896 a 1024 (la M4 tiene 895 trozos y el
paquete de formato 4 de la M5 que construye `make check` 935): son 128
entradas más en las tablas por trozo de `scene_assets.c`; las ranuras medidas
después siguen en la línea `MSX CACHE` de las puertas (docs/MISION4.md).

### Proyección de ROM con seis misiones

Cartucho actual 29,8 MB. Cada misión añade su pack (`mission2.pack` 1,97 MB)
y su música (la de la misión 1, 2,36 MB, VADPCM 24 kHz): ≈4,3 MB × 4 misiones
≈ 17 MB → **≈47 MB < 64 MB**. Las películas de
`filesystem/cinematics` siguen en v1; se convierten al construir
`build/game-romfs`.

## Imagen de 12 bytes (11-XI, `work-m4`): sitio para el jefe de la M4

`scene_image_t` pasa de 14 a 12 bytes (`{page, palette, x, y, height, width, format}`: ancho y formato en un byte;
medido: ancho ≤ 64 y formato 0 / 2 en `scene_images` (8710), `scene_background` y `scene_map_images` (3946), las
piezas de actor de los paquetes de misión ≤ 64 × 64 y las tiras de mapa 16 × 256; `pack_scene.py` lo comprueba y el
cargador de paquetes se para si un ancho no cabe). Ahorro: 25,3 KB de imagen (las tablas base) y un 14 % de las
tablas de imágenes de cada etapa en sus ranuras reservadas. Con el jefe de la M4 dentro (`neo_m4boss.o` 24,7 KB)
la imagen queda en 2 243 496 bytes, 1808 menos que antes del jefe: la caché no pierde ninguna ranura.

## Para continuar (nueva sesión)

Rama `work-ram`. Puertas: `make check`, `make check-core`, `make -j4`,
`make mslugx-reference.z64 mslugx-m2-reference.z64`, arranque en ares
(`tools/test_logo_ares.py`; su regex `MSX mission handoff x=` está desfasada
frente a la línea `MSX mission handoff (descent)` del juego) y recorrido de la
misión 1 (`tools/test_mission_native_ares.py`). Siguiente paso: plan #3
(caché de películas comprimida en RAM, descompresión por fotograma).
