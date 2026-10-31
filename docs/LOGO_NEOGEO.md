# Logo NEOGEO en 3D al arrancar

Al encender el cartucho aparece el logotipo **NEOGEO** en 3D girando, al estilo
del arranque de *Mario Kart 64* (donde gira el logotipo de Nintendo), con la
sintonía del *eyecatch* de la BIOS Neo Geo. Después sigue lo que ya había:

- en el cartucho de juego (`mslugx-explorer.z64`, `mslugx-reference.z64`,
  las ROM de prueba), la presentación animada METAL SLUG X64, la entrada en
  paracaídas y la Misión 1 cuando el cartucho lleva las cinemáticas privadas
  (`filesystem/cinematics/title-motion.*`, `entry.*`; `src/main.c`); sin
  ellas, directamente la Misión 1 (con la caché de escena calentándose en el
  tiempo libre del logo, RENDIMIENTO.md);
- en `mission1-internal.z64` (`mission_main.c`, mismo orden; vuelve a enlazar
  con los objetos del juego), igual que el cartucho de juego.

Start salta cada etapa (logo, título y entrada). `tools/test_logo_ares.py`
comprueba el orden entero, sin fotogramas tardíos ni perdidos, con
`--skip logo,title,entry` para elegir qué etapas se saltan.

Start (flanco de pulsación, como en el título) salta el logo; un Start
mantenido desde el encendido no cuenta. Si los recursos privados del logo no
están en la ROM, el arranque es el de antes.

## Qué es nativo y qué se ha tomado de dónde

- **Nada de Mario Kart 64** entra en el repositorio ni en la ROM: ni código,
  ni listas de visualización, ni modelos, texturas o audio. Se usó solo como
  referencia visual para medir tiempos y estilo (tabla de abajo).
- **Las letras** son las del eyecatch real: los tiles del logotipo NEOGEO que
  la BIOS dibuja con el banco de sprites que declara el cartucho (byte `0x115`
  de la cabecera = 1, tiles `0x100..0x139` de la C-ROM), colocados exactamente
  como los coloca la BIOS (15 columnas de sprites de 4 tiles, leídas de SCB1..SCB4
  en un volcado de VRAM). La herramienta compara la reconstrucción con la
  captura de pantalla de MAME del mismo fotograma (diferencia RGB máxima 7/255,
  la conversión de color de 6 bits) antes de seguir.
- **La malla** se genera en compilación con `tools/build_neogeo_logo.py`: los
  pinceles 1..4 de la paleta del eyecatch (`0xFFF`, `0xDDD`, `0xAAA`, `0x555`,
  el antialiasing del original) se convierten en cobertura; *marching squares*
  al 50 % da el contorno de cada letra (con los huecos de las O), se simplifica
  (Douglas-Peucker, 0,45 px) y se triangula por *ear clipping* con puentes a los
  huecos. Se extruye 14 px (35 % de la altura de las letras): tapas delantera y
  trasera y paredes laterales con normales suavizadas en curvas (< 35°) y duras
  en las esquinas. La herramienta exige que la malla expandida sea una
  2-variedad cerrada con todas las caras orientadas hacia fuera.
  Resultado actual: 8 contornos, 229 puntos, 221 triángulos por tapa,
  229 aristas laterales (900 triángulos en total).
- **El render es nativo** (`src/neogeo_logo.c`, libdragon fijado del repo,
  sin dependencias nuevas): la CPU proyecta y calcula la luz por vértice y el
  overlay RSP de `rdpq` monta los triángulos Gouraud con z-buffer
  (`rdpq_triangle` con `TRIFMT_ZBUF_SHADE`) que dibuja el RDP, a 320×240 con
  antialiasing. Metal dorado: ambiente + difusa de una luz arriba-izquierda,
  término de entorno cielo/suelo con el rayo reflejado y un brillo Blinn blanco
  (el destello al quedar de frente); degradado vertical en las letras. La estela
  de movimiento es realimentación de fotograma: el fotograma anterior se dibuja
  oscurecido bajo el nuevo (`rdpq_tex_blit`), igual que el efecto del original.
  Una tapa se dibuja o no entera y cada pared lateral se descarta con una
  multiplicación contra una constante guardada por arista; así solo se proyecta
  y se ilumina lo visible, recorriendo la memoria en orden.
- **La sintonía** es la música del eyecatch tal como la toca el propio driver
  de sonido del cartucho cuando la BIOS se la pide (comando `$02`, *play
  eye-catch music*), capturada con MAME desde el encendido. No se sintetiza ni
  se adivina nada.

## Por qué hace falta una captura nueva para la sintonía

La captura de referencia de `INTRO_AUDIO.md` (BIOS euro) no la contiene: el PCM
es silencio exacto hasta el fotograma ~330, donde empieza el título del juego.
La cabecera de Metal Slug X tiene el byte `0x114` (*eyecatch flag*) a 1, "lo
hace el juego", y el juego no lo hace: la BIOS se lo salta. Comprobado también
en pantalla (autotest y directamente el título).

`tools/mame/run_neogeo_eyecatch.py` arranca MAME con la BIOS euro y el
cartucho del usuario y, **solo en la región emulada**, pone ese byte a 0 antes
de que la BIOS lo lea. Nada más se modifica ni se pulsa. La BIOS ejecuta
entonces su eyecatch real (letras NEOGEO girando sobre blanco, MAX 330 MEGA /
PRO-GEAR SPEC, SNK) y en el fotograma 290 escribe `$02` en el latch de sonido
(`pc=00C1802E`, código de BIOS). El driver Z80 del cartucho toca la sintonía
del fotograma 290 al ~705 (6,86 s); en el 750 la BIOS ya pasa al juego. La
herramienta de construcción recorta desde el inicio del fotograma del comando
hasta el final de la cola (silencio de más de 0,25 s) y aplica ganancia ×4
(en la captura suena muy por debajo de la música del juego; pico resultante
10484/32767, sin recorte). Se guarda como WAV64 PCM16 estéreo a 32 kHz sin
compresión, como el título.

Captura usada: MAME 0.264 (el `/usr/games/mame` de este entorno; el 0.276 que
citan otros documentos ya no está instalado), 900 fotogramas.

## Medidas del arranque de Mario Kart 64 (referencia)

ROM aportada por el usuario (*Mario Kart 64 Spanish v1.1*, NTSC), ejecutada en
ares 146 sin cabeza (Xvfb). El binario compartido de ares de este entorno muere
con `SIGILL` en ese juego (se compiló con `-march=native` en otra CPU: usa
`vmovw`, AVX512-FP16); se usó una compilación del mismo código fuente
(`/home/user/ares-src`, `ARES_BUILD_LOCAL=OFF`) solo para medir. Como la
emulación iba a 22–37 VI/s, los tiempos se dan en **VI emulados** (1 VI =
1/59,826 s), integrando el contador VPS de la barra de estado de ares en cada
captura; precisión aproximada ±5 %. Dos pasadas independientes coinciden.
Las capturas se borraron después de medir.

| Momento (VI desde que aparece el logo) | Observación |
| --- | --- |
| 0 | Aparece ya grande, girado unos −55° y casi a oscuras |
| 0–45 | La luz sube hasta plena intensidad; a +46 queda de frente (destello blanco del brillo) |
| +112 | De canto (90°) |
| ~+165 | De espaldas (180°, texto en espejo) |
| +210 / +238 | 270° / 360° (de frente otra vez) |
| +260, +282, +299, +319 | 450°, 540°, 630°, 720° |
| +331, +346, +353, +364 | 810°, 900°, 990°, 1080° |
| ~+180 en adelante | Estelas de movimiento (copias anteriores del logo, cada vez más tenues) |
| +369 (366–372) | Empieza el fundido a blanco, girando a ~10°/VI |
| +369 → +439 (~70 VI) | Fundido a blanco completo |
| ~64 VI | Blanco mantenido |
| ~35 VI | Fundido del blanco a la pantalla de título |

- Eje de giro: vertical (yaw), sin cabeceo ni balanceo; la cámara mira de frente.
- Curva de velocidad: ~1,3°/VI al principio, 1,7 a 2,0°/VI hacia los 180–270°,
  luego acelera hasta ~10°/VI justo antes del blanco (unas 3,4 vueltas en
  ~6,2 s).
- Encuadre: de frente el logotipo ocupa ~97 % del ancho y ~34 % del alto de la
  imagen; entre el primer y el segundo frente crece ~10 % (acercamiento lento
  de cámara). Perspectiva marcada: de canto el extremo cercano se ve mucho más
  alto que el lejano.
- Estilo: metal dorado con sombreado Gouraud y brillo especular; fondo negro.
- Final: fundido a blanco, blanco sostenido y fundido del blanco al título.

## Cómo se ha adaptado

`src/neogeo_logo.c` reproduce esa curva (tabla de velocidades por tramos
integrada en una tabla de ángulo por VI, desde −55°), la subida de luz en los
primeros 60 VI, el acercamiento del 8 % durante el giro, el encuadre de ~90 %
del ancho de frente, las estelas desde +170 VI, el fundido a blanco de +373 a
+443, el blanco hasta +509 y un fundido de blanco a **negro** hasta +545 (9,1 s
en total). Termina en negro y no en un fundido sobre el título porque la
presentación X64 debe quedar sin cambios: empieza desde negro.
La sintonía dura 6,86 s y termina durante el fundido a blanco, como la de
Nintendo en MK64. Todo se rige por el reloj (`TICKS_READ`), no por el número
de fotogramas: si se perdiera alguno, el logo no se retrasa respecto al audio.

En el cartucho de juego el logo va antes de cargar la escena y de arrancar la
música de la misión (antes, `game_audio_init` arrancaba la música durante la
carga). La carga del juego viene después del logo, así que tras el negro final
hay unos segundos en negro hasta la misión (en ares ~3 s).

## Rendimiento (ares 146)

`tools/test_logo_ares.py` mide en tiempo emulado: 546 fotogramas en 9,15 s,
**ningún fotograma tarde** (intervalo máximo entre fotogramas 16 725 µs, es
decir, cada VI), CPU por fotograma media 6,7 ms y máxima 9,3 ms, hasta 471
triángulos por fotograma. El RDP termina dentro del VI (no hay ningún intervalo
de 33 ms). No es una medición en MiSTer ni en hardware; la CPU emulada de ares
es aproximada, aunque la holgura es grande. Una primera versión que iluminaba
toda la malla indexada tardaba 20 ms de CPU por fotograma (fallos de caché
en accesos aleatorios a 1398 vértices) y perdía uno de cada dos VI: se rehízo
con el formato por tapas/aristas descrito arriba.

## Generar los recursos (privados, fuera de Git)

```sh
export N64_INST=/ruta/libdragon     # audioconv64
python3 tools/mame/run_neogeo_eyecatch.py --rom-dir /ruta/roms --output /ruta/captura-eyecatch
python3 tools/build_neogeo_logo.py /ruta/captura-eyecatch --rom-dir /ruta/roms
make -j4 && make mslugx-reference.z64      # el logo entra en la ROM si existen los recursos
python3 tools/test_logo_ares.py mslugx-explorer.z64 --output /ruta/prueba-logo
python3 tools/test_logo_ares.py mslugx-explorer.z64 --skip --output /ruta/prueba-logo-start
```

`build_neogeo_logo.py` necesita `filesystem/sprites.bin` (`tools/import_rom.py`)
y `mslugx.zip`. Escribe `filesystem/logo/neogeo-logo.mesh`,
`filesystem/logo/neogeo-jingle.wav64`, `filesystem/logo/neogeo-logo.json`
(procedencia, medidas y SHA-256) y vistas previas en `build/logo-preview/`.
`make` copia los dos primeros en `game-romfs` (y `build_mission_fs.py` en el
de `mission1-internal`) solo si existen.

`test_logo_ares.py --record video.mp4` además graba un vídeo de revisión: la
salida ALSA de ares va a un fichero (sus bytes son el reloj emulado, porque
ares aquí va más lento que el tiempo real), las capturas de pantalla se
sitúan con ese reloj y ffmpeg las une al audio a velocidad real.

Formato de `neogeo-logo.mesh` (big-endian, versión 2): cabecera `NGLM`,
versión, número de puntos de tapa, de aristas y de triángulos de tapa, ancho,
alto y fondo; puntos de tapa (x, y); aristas (a, b, normales suavizadas en a y
b, normal plana x y la constante de visibilidad −n·punto medio); triángulos de
tapa (3 × u16).

## Vídeo de revisión

`downloads/logo-preview/arranque-neogeo.mp4` (fuera de Git, 20,8 s, H.264/AAC,
960×720) es un **montaje**: los primeros 9,6 s son la grabación de ares de
`mslugx-reference.z64` con su audio real (sumidero ALSA, recolocada con el
reloj emulado, como se describe arriba); el resto es la vista previa ya
existente del título X64 (`docs/media/metal-slug-x64-animated-intro.mp4`,
generada fuera de línea), pegada a continuación porque `mission1-internal`
no se puede construir aquí. No es una grabación continua de una sola ROM.

## Pendiente / límites

- Las cinemáticas privadas (título X64 y entrada, regeneradas con MAME 0.264)
  ya están en el cartucho de juego: logo → título → entrada → Misión 1,
  probado en ares (RENDIMIENTO.md, «Orden de arranque»).
- Sin prueba en MiSTer.
Evidencia numérica: [logo-neogeo-validation.json](logo-neogeo-validation.json).

## Carga del título durante el logo (ROM 10c)

En MiSTer se veían unos 3 s de negro entre el blanco final del logo y el
título: `msx_cinematic_open` leía de golpe los 3,4 MB del título, validaba sus
601 paquetes (CRC y estructura) y aplicaba todos para preparar los fotogramas
pesados. Ahora `msx_cinematic_prepare` arranca ese trabajo antes del logo y
`msx_cinematic_prepare_idle` (gancho de tiempo libre del logo) lo avanza por
pasos acotados: DMA de 16 KB, CRC en trozos de 8 KB, un paquete por paso al
aplicar; cada paso solo empieza si cabe antes del siguiente blanco vertical
(margen 1,5 ms, estimación media por KB). `msx_cinematic_open` termina lo que
falte. Medido en ares: negro tras el logo 2,2 s → 0,55 s (queda la aplicación
de los paquetes 346..600); el logo pierde 2 fotogramas de 544.
