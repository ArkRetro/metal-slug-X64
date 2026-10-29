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
