# Arquitectura propuesta: Metal Slug X nativo en N64

Estado: especificación del objetivo. La v8 implementa el núcleo CI4/RDP, pack y
caché PI descritos en RDP_IMPLEMENTATION.md. La lógica sigue siendo provisional;
no satisface todavía el objetivo completo ni garantiza rendimiento en todo el juego.

## Corregir el modelo de origen

MAME neogeo_spr.cpp limita el generador a 381 entradas/columnas y 96 activas
por línea, no 384 por línea ni 1536 sprites independientes. Cada columna mide
16 píxeles de ancho y puede contener hasta 32 tiles de 16x16. El encadenado
horizontal agrupa columnas. Además existe la capa FIX de tiles 8x8, utilizada
para texto/HUD. La ROM no proporciona código fuente nativo para recompilar:
la lógica 68000 también debe recuperarse o traducirse.

Separar cuatro contratos: exactitud de índices/orden/zoom; color del framebuffer;
cadencia de simulación; salida VI. La conversión a RGBA5551 y la salida VI no
son automáticamente idénticas a la salida RGB de MAME.

## A. TMEM, formato y segmentación

Recomendación base: CI4, TLUT RGBA16, pen 0 transparente, nearest, sin Z.
Convertir offline las C-ROM planares a CI4 en orden de lectura del RDP.
Conservar índices, tiles, paletas y atributos; no cuantizar toda la escena a una
paleta compartida de 16/256 colores. Mantener un atlas de índices y un gestor
independiente de paletas.

Con TLUT activado, la mitad superior de TMEM está reservada a paletas:

- 2048 bytes inferiores para texels CI.
- 2048 bytes superiores: 256 entradas replicadas en cuatro bancos, 8 bytes
  físicos por entrada. Caben 16 paletas CI4 de 16 colores.
- La paleta de 16 colores ocupa 32 bytes en RDRAM, 128 físicos en TMEM.
- Ocho descriptores de tile, distintos de la cantidad de tiles almacenados.

Para CI4/RGBA16/IA8, usando b bits por texel:

    stride(W,b) = 8 * ceil(W*b/64)
    bytes(W,H,b) = stride(W,b) * H
    Hmax = floor(TMEM_texels / stride(W,b))

Los bloques y formatos también deben cumplir las reglas de carga y bancos del
RDP; usar las primitivas de libdragon para los detalles de CI4 y LoadBlock.

| Formato | TMEM para texels | Tile 16x16 | Tiles 16x16 residentes |
| --- | ---: | ---: | ---: |
| CI4 + TLUT | 2048 B | 128 B | 16 |
| RGBA16, sin TLUT | 4096 B | 512 B | 8 |
| IA8, sin TLUT | 4096 B | 256 B | 16 |

CI4 reduce por cuatro la huella de texels en RDRAM/ROM respecto a RGBA16,
pero por dos la presión efectiva de TMEM, por la reserva del TLUT. IA8 pierde
la paleta cromática arbitraria del original, por tanto no sirve como formato
general. CI8 no mejora el almacenamiento de imágenes de 16 colores.

Página natural: 64x64 CI4 = 2048 B, contiene 16 tiles. Un bloque 256x512
ocupa 64 KiB CI4 y requiere al menos 32 cargas de 2 KiB, por ejemplo 4x8
ventanas de 64x64. Una columna 16x512 se divide en dos ventanas 16x256.
Dividir también al cambiar paleta o prioridad; un rectángulo CI4 no selecciona
una paleta diferente por tile interno. Unir rectángulos solo cuando índices,
paleta, posición, zoom y orden sean compatibles.

Las paletas Neo Geo activas superan los 16 slots TLUT: gestionar una caché de
paletas con IDs y versiones. Reemplazar un slot únicamente después de sus
últimos consumidores en la cola RDP. Mantener el orden original; no ordenar
globalmente los sprites por textura/paleta. Las dependencias de oclusión mandan.

Fidelidad cromática: TLUT RGBA16 y framebuffer RGBA16 cuantizan el modelo de
resistencias y el bit dark de Neo Geo. Para igualdad RGB8 con MAME se necesita
un camino RGBA32 expandido y framebuffer de 32 bits, con mayor coste. La TLUT
N64 no admite paletas RGBA32. Comparar primero el framebuffer lógico; la VI y
la cadena de vídeo son una etapa adicional con su propia precisión.

## B. Camino RSP/RDP

Usar libdragon rdpq/rspq, no Fast3D. El objetivo son TextureRectangle y cargas
de textura, sin transformaciones 3D ni Z. rdpq encola comandos a través de un
overlay RSP y aplica fixups; RSP no es un blitter de píxeles ni amplía TMEM.
S2DEX es una alternativa para una base compatible con ese SDK; no hace falta
recrear su API encima de libdragon. Un microcódigo propio se justifica solo
si el perfil muestra que la expansión de metasprites domina el tiempo.

CPU: lógica, cámara, eventos, selección de frames, visibilidad y solicitudes
de residencia. Cola compacta de draw items: índice de página, subrectángulo,
paleta/version, posición, flip, zoom, prioridad. RSP: envío/expansión de comandos
y audio bajo planificación compartida. RDP: cargas TMEM y rasterizado.

Modo inicial 1-cycle, point sampling, alpha compare, sin Z ni filtrado 3-point.
COPY se limita a blits 1:1 compatibles; no cubre por sí solo todos los zoom.
El zoom Neo Geo usa patrones de selección/descarte, no necesariamente el mismo
muestreo afín que TextureRectangle. Para exactitud, reproducir esos patrones
con bandas/runs o variantes de zoom precalculadas. Evitar costuras por redondeos
distintos en bordes compartidos y semánticas inclusivas de rectángulos.

No confiar en un único snapshot de VRAM para toda escena: cambios de paleta,
registros y efectos raster pueden exigir bandas de scanlines. Reproducir orden
de columnas, capa FIX, recorte y límite de 96 columnas por línea si el original
alcanza overflow. El objetivo es reproducir también lo que el hardware omite.

Optimización de RDRAM:

1. Preconvertir y deduplicar tiles; almacenar páginas de 2 KiB contiguas, alineadas
   a 64 bytes. Metadatos en estructuras compactas, sin perseguir punteros por tile.
2. Conservar páginas calientes y paquetes de animación entre frames. No copiar
   todos los frames visibles al cambiar solo el índice de animación.
3. Agrupar cargas contiguas: LoadBlock para bloques lineales válidos, LoadTile
   para ventanas con stride. No imponer que uno sea siempre más rápido.
4. Reducir rectángulos/texels transparentes y fusionar runs compatibles sin
   cambiar el orden de composición. No pasar un canvas vacío gigante al RDP.
5. Usar sincronización de rdpq para LoadSync, TileSync y PipeSync; evitar una
   espera de CPU o FullSync por sprite. Finalizar/reusar buffers por fences.
6. Una caché lógica de dos regiones de 1 KiB dentro de la mitad CI permite
   preparar/reusar grupos pequeños. No proporciona carga y dibujo simultáneos:
   las cargas TMEM y el rasterizado comparten el RDP y su orden de comandos.

Una página TMEM no se sobrescribe hasta terminar sus draws. Una página RDRAM
no se libera hasta que el RDP termina de cargarla. Un único syncpoint RSP no
demuestra necesariamente finalización del trabajo RDP.

El techo geométrico del origen es 96*16*224 = 344064 muestras de sprite por
frame, antes de sumar FIX, fondo, zoom/recorte y costes de estado. No equivale
a rendimiento garantizado: importa el overdraw, alpha, cargas y arbitraje de
RDRAM con VI, PI, CPU y audio.

## C. Streaming con 8 MiB

Hay dos cachés distintas: ROM->RDRAM y RDRAM->TMEM. La primera mantiene varios
frames y zonas; la segunda solo el working set del grupo de draws actual.
El RDP no toma texturas directamente del cartucho.

Presupuesto inicial, sujeto a linker y medición:

| Uso | MiB |
| --- | ---: |
| Código y estado del juego | 1,00 |
| Framebuffers y colas RSP/RDP | 0,75 |
| Assets fijados: Marco, HUD, efectos comunes | 0,75 |
| Caché CI4 de tiles/animaciones/escenario | 3,00 |
| Entrada comprimida y staging DMA | 0,50 |
| Audio y colas de muestras | 0,50 |
| Scratch, pilas y heap transitorio | 0,75 |
| Reserva y fragmentación | 0,75 |
| Total | 8,00 |

Dos framebuffers 336x240x32bpp ocupan 630 KiB; dos 320x240x16bpp, 300 KiB.
No asignar Z. El perfil exacto de VI/resolución se valida aparte; a 320 píxeles,
libdragon no permite FILTERS_DISABLED. Usar márgenes/VI apropiados, no eliminar
el assert. La salida lógica objetivo sigue siendo la región Neo Geo de 320x224.

Formato de ROM: directorio de assets, páginas CI4 de 2 KiB y paquetes
independientemente descomprimibles de 16-64 KiB. Agrupar por zona y afinidad de
animación: idle/run/attack/hit/death de un arquetipo, no una copia enorme por
instancia. Deduplicar índices independientemente de paletas. Mantener offsets
de ROM, tamaño comprimido/descomprimido, dependencias y límites de memoria.
Objetivo habitual de compatibilidad: ROM <=64 MiB; verificar flashcart y PI.
No incorporar BIOS ni ROM 68000 al paquete de un port completamente nativo.

Estados: ABSENT -> REQUESTED -> DMA_IN_FLIGHT -> COMPRESSED_READY -> DECODING
-> READY -> PINNED -> EVICTABLE. Evicción solo sin referencias y con fences
cumplidas; introducir generaciones para no reutilizar handles obsoletos.

Prefetch por eventos y horizonte de cámara: fijar Marco y sus transiciones;
cargar arquetipos antes del punto de spawn; anticipar explosiones/vehículos
antes de su evento; mantener anterior, actual y siguientes estados, no solo el
frame siguiente. Explosiones críticas deben estar residentes antes de disparar.
Limitar por igual el DMA y el tiempo de descompresión, con cuota por frame.

Prioridades: audio con deadline, acciones inminentes de Marco, enemigos activos,
eventos próximos, entrada de escenario, precarga especulativa. Usar una cola PI
central y buffers ping-pong; no iniciar DMAs competidores desde cada actor.
PI puede solaparse con CPU/RCP, pero consume RDRAM y no es ancho de banda gratis.

Predicción:

    anticipación >= tiempo_DMA + tiempo_decode + espera_cola + margen
    tiempo_DMA = bytes / ancho_de_banda_efectivo_medido

Ejemplo ilustrativo, no benchmark: a 8 MiB/s, 32 KiB tardan 3,9 ms y 256 KiB
31,25 ms. No se puede esperar al spawn para pedir 256 KiB y mantener un frame
de ~16,9 ms. Un horizonte de 3-6 ticks y precarga de zona reduce ese riesgo,
pero debe adaptarse al tamaño real y a las dependencias de la siguiente acción.

DMA: respetar alineaciones de PI/libdragon; páginas de 64 bytes y tamaños
acolchados simplifican el contrato. Caché D de CPU: líneas de 16 bytes, invalidar
destinos de DMA y hacer writeback de datos generados por CPU antes de consumirlos
RSP/RDP; no invalidar líneas compartidas con datos sucios ajenos. Fences antes
de reutilizar staging, páginas o framebuffers. Preferir arenas fijas/pools.

Un fallo de caché crítico no se resuelve cambiando a otro frame, omitiendo una
explosión o frenando la simulación silenciosamente: eso rompe el objetivo.
En desarrollo debe dejar evidencia de deadline incumplido; se corrige el pack,
la predicción o el tamaño del working set. Cargas de zona fuera de gameplay
pueden cubrir cambios de conjunto que no quepan durante un combate.

## Cadencia y criterio de aceptación

Neo Geo ronda 59,1856 Hz, distinto de NTSC N64 ~59,94 Hz y PAL 50 Hz. Simulación
con acumulador de tiempo a la cadencia original; salida desacoplada. NTSC repite
alguna presentación, aproximadamente cada 1,3 s, si conserva esa velocidad.
Cadencia idéntica exige evaluar una configuración VI cercana al origen y su
compatibilidad, no acelerar el juego ni introducir una caída de rendimiento.

Medir máximos, no solo promedios: CPU, RSP/audio, RDP, texels, comandos, cambios
TLUT, bytes PI, latencia de cargas, misses y memoria. CPU/RSP/RDP se solapan;
no sumar sus duraciones como si fueran secuenciales. Admitir el objetivo solo
tras escenas de peor caso, jefes/explosiones y pruebas en N64 física; ares y
MiSTer aportan evidencias adicionales, no una garantía temporal de consola.

Ruta de implementación: mantener gameplay y reemplazar blits CPU por CI4/rdpq;
añadir gestor de paletas/páginas; compactar los assets RGBA de la demo; construir
pack de ROM y cola PI; recuperar mapa, lógica y efectos originales; validar
índices, orden, zoom, color y cadencia con trazas equivalentes de MAME.

## Referencias contrastadas

- MAME: https://github.com/mamedev/mame/blob/master/src/mame/snk/neogeo_spr.cpp
- Paleta: https://github.com/mamedev/mame/blob/master/src/mame/snk/neogeo_v.cpp
- libdragon rdpq: https://github.com/DragonMinded/libdragon/blob/trunk/include/rdpq.h
- Cargas/texturas: https://github.com/DragonMinded/libdragon/blob/trunk/include/rdpq_tex.h
- Modos/TLUT: https://github.com/DragonMinded/libdragon/blob/trunk/include/rdpq_mode.h
