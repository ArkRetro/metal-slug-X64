# v8: implementación del núcleo RDP, CI4 y streaming

Esta versión implementa el camino gráfico del diseño N64_ARCHITECTURE.md sobre
la zona experimental existente. No convierte aún la demo en el juego arcade
completo ni acredita rendimiento máximo en consola física.

## Implementado

- La escena usa rdpq/rspq: fondo, Marco, soldado, proyectiles, cajas y texto pasan
  por RDP. No hay bucles de blit de píxeles por CPU en actor.c. El laboratorio
  de diagnóstico FIX/C conserva intencionadamente su implementación CPU.
- Marco y soldado se empaquetan en CI4 preservando los índices Neo Geo originales
  y la asociación con las 16 entradas de su paleta. Los sidecars MSXI conservan
  los índices incluso cuando dos entradas tienen el mismo color RGBA5551.
- Fondo compuesto: 67 bloques CI4 y 3 RGBA16, sin cuantización adicional para
  forzar bloques con más de 16 colores a una paleta pequeña.
- Páginas de 2048 bytes, deduplicación y recorte transparente. Upload de la
  subregión útil, no del lienzo vacío entero. Assert de uso TMEM <=2048 bytes.
- Caché TLUT LRU de 16 slots dentro del frame; conserva orden de composición.
  Los IDs representan paletas inmutables del pack. El texto puede reemplazar
  TMEM, así que invalida explícitamente la residencia registrada.
- Point sampling, alpha compare, sin Z. Flip y rectángulos por RDP. Se mantiene
  framebuffer RGBA16: conserva el color cuantizado de la demo, no igualdad RGB8
  absoluta con MAME y la cadena de vídeo.
- scene.pack: bloques independientes de 16 KiB, RLE o raw según cuál sea menor,
  alineación de offsets a 64 bytes y CRC32 de cada bloque descomprimido.
- Cola de peticiones con prioridades, un PI DMA activo, staging ping-pong de
  16 KiB y descompresión del bloque anterior mientras empieza la siguiente DMA.
  DragonFS se convierte de dirección KSEG1 a dirección física para PI.
- Invalidación del destino DMA y writeback de páginas/paletas antes del RDP.
  Handles por ID, caché LRU y protección hasta el fence real de finalización RDP.
  El callback de rdpq_detach_cb libera la época y entrega el framebuffer a VI.
- Marco y transiciones fijados; solicitudes anticipadas de frames próximos y
  escenario. La zona actual cabe completa y se calienta antes del gameplay.
  Capacidad máxima 3 MiB con 8 MiB RDRAM, 512 KiB con 4 MiB; se asigna solo el
  tamaño necesario si el pack es menor. No se ha ejercitado evicción con este
  pack pequeño; la ampliación a otras zonas requiere medir su working set.
- Reloj entero de simulación a 59,185606 Hz separado de presentación. Animaciones
  siguen los ticks del juego. Conserva deuda temporal de catch-up y pulsaciones
  breves; pausar en el laboratorio no provoca catch-up al volver.

La prioridad de audio está prevista en el diseño, pero esta demo aún no tiene
motor de audio ni unificador de todos los consumidores PI. No se afirma que
exista ya un streaming completo de todos los recursos de Metal Slug X.

## Contenido y animación

El salto en movimiento usa una secuencia original propia, distinta del salto
vertical. La carrera con disparo usa piernas en movimiento, sin sustituirse por
el disparo estático. La selección de subida/cima/bajada depende de la velocidad
vertical, incluyendo caídas desde cajas.

Se extrajeron 30 muestras por acción combinada: salto en carrera de la captura
motion y carrera con disparo de runfire. Se verificaron 40275 píxeles visibles;
18 píxeles de Marco tapados por proyectiles se comprobaron resolviendo el sprite
superpuesto y contrastando su color con MAME. No se aceptan discrepancias con
una máscara genérica ni se usan capturas como origen del gráfico.

Hay dos cajas sólidas nuevas (x=520..560, alto 20; x=920..968, alto 24). Se puede
saltar sobre ellas, aterrizar encima y caer por el borde. Bloquean movimiento y
proyectiles según altura. Son obstáculos de prueba dibujados por RDP, no objetos
extraídos del mapa original. Se mantienen checkpoint, tres encuentros y salida.

## Tamaños del conjunto actual

188 frames, 157 páginas, 50 paletas y 20 bloques.

- Fuentes RGBA de la escena: 1912896 bytes.
- Páginas decodificadas y acolchadas: 327680 bytes (320 KiB).
- Pack comprimido: 122944 bytes (aproximadamente 120 KiB).
- BSS de la aplicación: alrededor de 145 KiB; caché/staging son asignaciones
  dinámicas adicionales, no memoria contabilizada dentro de ese BSS.

La ROM conserva las ROM C crudas para el laboratorio, por lo que su tamaño
principal sigue rondando 48 MiB. No confundir el tamaño del pack del escenario
con el tamaño total de la ROM o de los gráficos del juego.

## Construcción y regeneración

`make -j4` genera scene.pack y build/scene_meta.h automáticamente a partir de
los assets intermedios retenidos y sus sidecars .idx4. romfs/ contiene únicamente
el pack y los datos del laboratorio, evitando incorporar los grandes bancos
RGBA y PNG de trabajo al cartucho.

Si faltan los intermedios, ejecutar desde el checkout:

```sh
python3 tools/export_marco.py /workspace/research/captures
python3 tools/export_marco.py /workspace/research/actions --actions
python3 tools/export_marco.py /workspace/research/motion --motion \
  --secondary-captures /workspace/research/runfire
python3 tools/export_enemy.py /workspace/research/captures
python3 tools/export_scene.py /workspace/research/captures
make -j4
```

Las capturas nuevas se generan con tools/capture_motion.lua (salto en carrera)
y tools/capture_runfire.lua (disparo en carrera), usando el mismo comando y BIOS
MAME de docs/MARCO.md y MSX_CAPTURE_DIR en directorios separados. Las capturas
originales dependen de ROM/BIOS del usuario y no están versionadas.

## Comprobación

El empaquetador verifica índices y colores tras codificar cada imagen CI4, y
cada bloque comprimido se decodifica y compara antes de escribirse. El runtime
comprueba codec, límites y CRC antes de entregar páginas al RDP.

Pruebas del host:

```sh
cc -std=c11 -Wall -Wextra -Werror -Ibuild tools/test_gameplay.c src/gameplay.c src/neo_collision.c src/neo_vehicle.c -o /tmp/msx-game
/tmp/msx-game
cc -std=c11 -Wall -Wextra -Werror tools/test_scene_codec.c -o /tmp/msx-codec
/tmp/msx-codec
cc -std=c11 -Wall -Wextra -Werror tools/test_scene_clock.c -o /tmp/msx-clock
/tmp/msx-clock
```

En el entorno retenido, iniciar ares con tools/run_ares.sh redirigiendo la salida
a /workspace/emulator/run-v8-release.log. Con ares recién iniciado:

```sh
DISPLAY=:99 LD_LIBRARY_PATH=/workspace/emulator/root/usr/lib/x86_64-linux-gnu \
python3 tools/test_rdp_ares.py
```

El test usa controles normales y capturas para salto en carrera, cajas, combate,
checkpoint, salida, flip, reinicio y retorno desde el laboratorio. No hace
falta repetir todas las pruebas históricas de FIX/C por cada cambio.

Instrumentación MSX RDP: draws, uploads, cambios TLUT, bytes de TMEM, tiempo CPU
hasta el envío y contadores PI/caché. No mide tiempo RDP aislado ni constituye
un benchmark de N64 física. La cadencia de ares en un servidor con lavapipe
está limitada por el emulador y su GPU por software.

## Pendiente para el objetivo completo

Mapa y colisiones originales; lógica y enemigos completos del 68000; audio;
zoom por patrones Neo Geo, efectos raster y límite por scanline; perfil de color
RGB8 exacto; streaming entre zonas mayores y pruebas de peor caso con jefes y
explosiones. La implementación presente no debe anunciarse como pixel-perfect
ni libre de ralentizaciones en todo el juego.
