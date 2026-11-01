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
| 6 | `scene.pack`: códec con más ratio solo si el tiempo de descodificación (≤ 7,3 ms) no sube | hasta −3 MB ROM (estimación) | alto (picos de `asset`) | no ahora |

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

### Proyección de ROM con seis misiones

Cartucho actual 29,8 MB. Cada misión añade su pack (`mission2.pack` 1,97 MB)
y su música (la de la misión 1, 2,36 MB, VADPCM 24 kHz): ≈4,3 MB × 4 misiones
≈ 17 MB → **≈47 MB < 64 MB**. Las películas de
`filesystem/cinematics` siguen en v1; se convierten al construir
`build/game-romfs`.

## Para continuar (nueva sesión)

Rama `work-ram`. Puertas: `make check`, `make check-core`, `make -j4`,
`make mslugx-reference.z64 mslugx-m2-reference.z64`, arranque en ares
(`tools/test_logo_ares.py`; su regex `MSX mission handoff x=` está desfasada
frente a la línea `MSX mission handoff (descent)` del juego) y recorrido de la
misión 1 (`tools/test_mission_native_ares.py`). Siguiente paso: plan #3
(caché de películas comprimida en RAM, descompresión por fotograma).
