# Entrada nativa en paracaídas y traspaso al jugador

La entrada no interactiva de la Misión 1 ya se reproduce en un ejecutable interno
N64, con audio, y entrega el control a Marco en la región parcial existente.
No es la demo solicitada del primer nivel completo. El título, tutorial y
selección completos todavía no se reproducen en este ejecutable; también faltan
encuentros, mapa, eventos, jefe, efectos de juego y música continua.
No se publica este cartucho como una nueva demo.

## Verificado en la fuente

MAME 0.276, BIOS euro, configuración y NVRAM nuevas. La única entrada automatizada
es moneda, Start y confirmación de Marco en 3620, después de terminar el tutorial.
No se parchea RAM del juego. Se captura cada fotograma y se observan las escrituras
reales de VRAM y paleta con taps de memoria. Cada escritura debe coincidir con
la palabra anterior; su reproducción debe producir el siguiente volcado completo.
La selección de sprites se conserva al inicio de cada línea.

El segmento contiene los fotogramas **3830..4059**, 230 fotogramas a la cadencia
Neo Geo de aproximadamente 59,185606 Hz. Cada uno de sus 15 662 080 píxeles activos
RGB8 se reconstruye de C-ROM/S-ROM y se compara contra una imagen independiente
de MAME, con tolerancia de una unidad por canal. Los PNG solo sirven para comparar;
no se usan como fuente de los assets. Después se cuantiza a RGBA5551.

Esta captura añade los estados reales de selección FIX y de autoanimación del
hardware. En el segmento, autoanimación está deshabilitada en el dispositivo:
se conservan los códigos que escribió el juego. El reproductor ya no depende
de inferir esa fase desde el número de frame.

El primer estado del jugador apoyado en el suelo se observa en **4060**:

| Campo observado | RAM | Valor |
| --- | --- | --- |
| X Q16.16 | `0x1014EC` | `7852402` |
| Y Q16.16, positivo hacia arriba | `0x1014F0` | `6157405` |
| Velocidad X/Y | `0x1014F4 / 0x1014F8` | `0 / 0` |
| Flags, incluye apoyo | `0x1014D9` | `0x40` |
| Etiqueta de rutina del actor | `0x1014C4` | `0x0C1FA6` |

La etiqueta es procedencia, no una dirección ejecutada en N64. No se afirma haber
traducido la lógica 68000 del paracaídas. Es una secuencia gráfica no interactiva
convertida fuera del runtime. La lógica jugable posterior sigue en C nativo.
`msx_game_begin_at()` acepta el traspaso solo si el suelo nativo coincide
exactamente con la altura capturada. Una altura incorrecta se rechaza.

## Formato y presupuesto

`entry.msxc` contiene cabecera, offsets y paquetes de cambios RGBA5551. Cada
comando salta píxeles y copia una tira literal o repite un color. Los paquetes
incluyen CRC32. El decodificador portable comprueba terminación, longitudes y
límites antes de modificar el lienzo. La rama rápida solo usa paquetes
inmutables previamente comprobados durante la carga.

El archivo mide **1 246 644 bytes**, frente a 31 324 160 bytes de fotogramas sin
comprimir. En 8 MiB se reservan hasta 2 MiB para datos codificados y hasta 2 MiB
para los fotogramas más costosos ya decodificados. En esta secuencia:

| Recurso | Bytes |
| --- | ---: |
| Datos codificados | 1 246 644 |
| 15 fotogramas preparados | 2 042 880 |
| Lienzo de trabajo | 136 192 |

Son 3 425 716 bytes más índices/metadatos, buffers de pantalla, audio y recursos
del escenario. Las cachés de la secuencia se liberan antes del juego activo.
Para este perfil se requiere Expansion Pak de 8 MiB. La ruta de streaming de
secuencias mayores conserva la validación de paquetes, pero su cadencia no está
validada por esta prueba.

Los fotogramas preparados evitan picos de trabajo durante los fundidos. El lienzo
usa memoria con caché y writeback explícito antes del acceso RDP. `rdpq_tex_blit`
trocea las transferencias conforme a TMEM; el renderer usa muestreo puntual y
sin dithering RDP. La salida VI de 320 píxeles conserva `FILTERS_RESAMPLE`,
requerido por el hardware y libdragon para esa anchura.

El framebuffer se adquiere antes de calcular qué frame corresponde presentar.
Así, la espera de VI no deja obsoleto el frame calculado. La prueba cuenta tanto
los frames distintos presentados como los omitidos; no acepta una secuencia que
solo alcance el último frame perdiendo imágenes intermedias.

El ejecutable interno mide aproximadamente 2,1 MiB. Su filesystem incluye los
recursos utilizados, sin los 48 MiB de C-ROM del laboratorio gráfico histórico.
La eliminación del laboratorio no sustituye trabajo pendiente de la misión.

## Audio y transición

El WAV64 contiene la referencia sonora desde el inicio del segmento hasta el
fotograma 4199: voz de misión y música de ese intervalo. Se corta usando tiempos
MAME medidos y se reproduce una vez mediante el mezclador RSP. No se inventa
un bucle corto para aparentar una música recuperada. El tramo termina unos
2,35 segundos después del traspaso; el bucle musical y los efectos que respondan
al juego siguen pendientes.

Se usan dos buffers AI para limitar la anticipación de la mezcla. La prueba
comprueba que el audio sigue activo al entregar el control; no es una medición
acústica de sincronización ni una prueba en MiSTer. No se simulan disparos con
un sonido grabado de una partida: el mapeo de efectos aún falta.

La secuencia no recibe los efectos del [modo 2.5D](RETRO25D.md), activo por
defecto: sus CRC no cambian. El primer frame de juego tras el traspaso
restablece modo RDP, scissor y residencia de TMEM/TLUT, y descarta pulsos de
luz previos.

Al terminar, se comprueba el CRC del último lienzo, se libera la secuencia,
se establece el estado de apoyo medido y se reinicia el reloj de simulación
para evitar que el jugador avance durante el tiempo de presentación.
Las pruebas ejercitan movimiento, salto, disparo, cámara y reinicio después
del traspaso.

## Validación

- Los 230 fotogramas reconstruidos coinciden con sus referencias MAME.
- El decodificador C host coincide con los 230 CRCs de píxeles independientes.
- En MIPS/ares coinciden los 230 CRCs, incluyendo la caché de fotogramas preparados.
- Tres framebuffers RDP completos de la región activa coinciden exactamente con
  los CRCs RGBA5551: primer frame, frame preparado más grande y último frame.
- La prueba integrada presenta los 230 frames distintos, sin omisiones, con
  audio activo al traspasar el control y controles jugables posteriores.

Los informes conservan hashes: [mission-entry-validation.json](mission-entry-validation.json),
[cinematic-native-validation.json](cinematic-native-validation.json) y
[mission-entry-evidence.json](mission-entry-evidence.json).
Los tiempos medidos son de ares, no rendimiento físico de N64/MiSTer ni prueba
del presupuesto durante la misión completa.

## Reproducir localmente

```sh
python3 tools/mame/run_intro_audio.py --rom-dir /ruta/roms \
  --output /ruta/nueva-entrada --end-frame 4200 --video-step 1 \
  --confirm-frame 3620 --script tools/mame/capture_intro_raster.lua \
  --raster-first 3820 --raster-last 4140 --shot-first 3820 --shot-last 4140
python3 tools/pack_cinematic.py /ruta/nueva-entrada --bios /ruta/roms/neogeo.zip
make check-core check check-cinematic
make cinematic-regression.z64 mission1-internal.z64
python3 tools/test_cinematic_ares.py cinematic-regression.z64 --output /ruta/nueva-prueba-pixeles
python3 tools/test_entry_ares.py mission1-internal.z64 --output /ruta/nueva-prueba-entrada
```

Los archivos generados y referencias completas quedan fuera de Git. Este objetivo
es de integración interna y no se publica hasta cumplir la entrega solicitada.

## Descenso nativo (rama `work-chute`)

Informe de MiSTer: en el arcade se controla a Marco mientras baja en
paracaídas; disparar (A) o saltar (B) corta la cuerda, Marco cae solo y la
tela se va. La lógica del descenso ya está traducida y comprobada bit a bit;
su conexión al arranque sigue pendiente (paradas abajo).

**Evidencia.** Tres capturas de familia de `capture_mission_rest.lua`
(`MSX_REST_FAMILY_ROOTS=c15ba,c162a,c170c,c4a58,c5286`, volcados cada 2
fotogramas desde 1830, nueva variable `MSX_REST_INPUTS` para pulsar durante
el descenso, 1838..2038 en esa captura):

| Captura | Entrada | Resultado |
| --- | --- | --- |
| `ch0` | ninguna | aterriza en 2039 (0C162A → 0C1FA6) |
| `chA` | A en 1858 (alto), C en 1878, A en 1918 | dispara y lanza granada colgado (bombas 10 → 9); corta en 1921 (`+40` < 448) |
| `chB` | B en 1898 | corta en 1901; cae con 0C170C y aterriza en 1935 |

En la captura de entrada (`land1`) el jugador pasa a 0C162A en 3860; la
paleta del escenario está completa desde 3860 (el oscurecimiento acaba en
3858) y el fondo no se mueve durante el descenso (filas 150..223 iguales
entre 3900 y 4100). El texto MISSION 1 START son actores de sprites (paletas
`0x6D..0x6F` cambiantes hasta 3932), no capa FIX.

**Traducido** (`src/marco_descent.c`, `src/neo_landing.c`):

- 0C15C6 (inicio), 0C162A (velocidad vertical hacia −1,0 con el término de
  `+40`, deriva aleatoria de 0029C6 con la palabra alta de D0 que deja
  `ANDI.W`, corte con B siempre y con A bajo `+40` = 0x1C0, aterrizaje),
  0C16D8 (corte: `+50`, `+90`, `+30`, `+34` = 2,0, `+61` = 2) y 0C170C
  (gravedad hacia −22,0 >> 5). El barrido 0143EE, 0C109C, los programas
  0BF818/0BF758 y el procesamiento común son del llamador.
- Vela: constructor 0C4A58 (0C4904, 01AD6E, paleta 0x38D), descenso de
  0C4AAE (`+1E` del jugador, `+28` = ∓4, 01AC38, 00427A), corte 0C4B20
  (`+5F` bit 3, 0C498C, 01AD52 a la raíz de escena con el movimiento del
  padre como velocidad, `+34` = 0) y 0C4B4A hasta el final del programa.
  El bit 3 de `+5F` oculta las poses en fases de vídeo impares (00436E):
  la vela cortada parpadea.

**Pruebas** (`tools/test_marco_descent.c`, en `make check`; fixture
`data/descent_fixture.json.gz` de `tools/extract_descent_fixture.py`):
jugador 205 actualizaciones en bucle cerrado sobre sus campos propios, 3567
bytes iguales, 2 cortes, 3 aterrizajes; vela 256 actualizaciones en bucle
cerrado desde el constructor, 35926 bytes iguales, 3 borrados.
Píxeles de la vela (exportación `export_rest_sprites.py` con raíces 0C4904 y
0C498C contra la VRAM de las tres capturas): los fotogramas visibles de la
vela cortada coinciden al 100 % (p. ej. 1098/1098, 1070/1070); los
fotogramas en que no aparece son el parpadeo de 00436E.

**Conexión al arranque.** `entry.msxc` se recorta a 3830..3859 con
`tools/truncate_cinematic.py` (los 30 primeros paquetes delta y sus CRC
auditados, sin volver a renderizar; el audio sigue entero y suena después
del traspaso). El traspaso pasa a 3860 con el estado del jugador tras su
primer 0C162A (`+28/+2C` = 7865136/16293888, `+30/+34` = 816/−483328;
`MSX_ENTRY_DESCENT` en `entry_meta.h`). `msx_game_begin_descent` crea la
vela (su constructor corre con el primer paso), y `gameplay.c` ejecuta el
descenso cada actualización: B (salto) y A (disparo) como bits 5/4 de
0C0FEA, la deriva con el RNG del mundo, el barrido aéreo del port como
0143EE/0C109C, el marcador 1P al aterrizar o al cortar. Disparo y granadas
colgado usan el procesamiento común del port. `test_gameplay`: sin entrada
aterriza a los 200 fotogramas (como MAME); con B a los 20, corta y aterriza
43 fotogramas después.

**Borde izquierdo (fallo de ROM 12 en MiSTer: «si caminas hacia atrás caes
infinito»).** La pantalla para al jugador en `cámara+28`; el port recortaba
solo la x y dejaba los pies a la altura de la x sin recortar, que en la
rampa de la salida (más baja a la izquierda) queda dentro del suelo: el
siguiente paso de suelo no encontraba suelo y caía sin fin (también desde el
aterrizaje antiguo de 4060). MAME (`capture_mission_rest.lua`,
`MSX_REST_INPUTS=2040:L:1`): x se queda en 28,0 con la cámara en 0 y los pies
sobre el suelo en esa x (y = 71). `clamp_feet` (`gameplay.c`) recorta y, si
estaba apoyado, recoloca los pies sobre el suelo en la x recortada (paso
normal, descenso y expulsión de vehículo). `test_gameplay`: izquierda desde
el traspaso, tras el corte y tras el aterrizaje, y en cada parada de cámara.

**Paradas explícitas.**

- La deriva usa el RNG del port, no la secuencia exacta de la partida.

### MISSION 1 START (`0607DE`, `src/neo_end.c`, rama `work-rom12`)

El actor de misión lo crea en `018646` (`0040BC`, D0 = 0x0E) justo antes del
traspaso; su primera actualización es la del constructor de la vela (1838 en
`ms2`). `0607DE`: misiones ≥ 6 lo borran (la comprobación de cartucho
`0607EA` no se reproduce); `060878` pide la voz `607D2[misión]` (`0120F8`,
0x1131 en la Misión 1; en el port suena con el audio de la entrada);
`060888` es la raíz de `06090A` con `+7D` = 0: cifra `060B06` (`060DC4`) y
las letras de `605AA` (misión 5: `60614`); `0608E0` espera 0x0E llegadas
(0x12) y sigue por `06099E` como MISSION COMPLETE (`+7D` = 0: velocidad
`060E7A` con los topes y `+7A` = 0x30). `gameplay.c` la crea con
`msx_game_begin_descent` en la lista `end` y la actualiza cada paso hasta
que se borra la última letra; se dibuja con las letras de fin de misión.

Evidencia: captura de familia `ms2` (`MSX_REST_BOSS_FROM=1780
MSX_REST_FAMILY_ROOTS=607de MSX_REST_FIRST_DUMP=1780 MSX_REST_PERIOD=2
MSX_REST_LAST=2110`). `tools/extract_end_fixture.py ... --range 607de-60f42`
→ `data/start_letters_fixture.json.gz`; `tools/test_neo_end.c` (en `make
check`, `start_letters`): **1038 actualizaciones en bucle cerrado (71
pasadas, 15 nodos), 145550 bytes iguales, 13 borrados**, `+60` del actor de
misión puesto. Sprites (`export_rest_sprites.py ... ms2@607de-60f42`) contra
la VRAM: **72 volcados, 165793 píxeles iguales, 1550 distintos** (1838: la
primera actualización, aún no en la VRAM; 1972/1976: cambio de programa al
salir). `test_gameplay`: 14 sprites como máximo, desaparecen a los 143
fotogramas del traspaso, sin paradas.

### Poses del descenso (catálogo de Marco, rama `work-rom12`)

`run_marco_catalog.py --first 1830` (los bloques de programa se vuelcan en
2102, tras el aterrizaje: antes cambian las palabras de protección
`2FFFE0..`) con `tools/mame/plans/marco-descent*.json`: `marco-dsc0` (sin
entrada), `marco-dsc1` (B en 1898) y `marco-dsc2` (A en 1858, C en 1878, A
en 1918). Colgado: cuerpo `0BF818` → `24E5AA` (nuevo, `B_HANG`), arma la del
salto `0BFC58` → `24E5BC`; el corte elige el cuerpo de salto `0BF758` →
`24E4E0`; colgado, A y C eligen los programas de disparo y granada de
siempre (`24C70A`, `24C862`). `marco_visual.c` toma `hanging`
(`gameplay.c` en `MSX_DESCENT_HANG`). `marco-dsc2` queda como evidencia
de elecciones: su bloque de 2102 aún lleva la comprobación de cartucho de
las letras (`0609D4`), así que no entra en el grafo. Catálogo 402 → 411
poses, las anteriores idénticas; `make check-marco-campaign`: casos
`d_hang` (100 pasos) y `d_cut` (18), **411 pasos iguales, 0 distintos**.
