# v9: escenario original observado y movimiento aéreo

La v9 sustituye el fondo de 304 píxeles repetido por las capas recuperadas de
la primera misión hasta el pueblo. No es aún una misión completa ni lógica 1:1.

## Datos recuperados e implementación

Se leyeron SCB/VRAM, paletas y RAM del juego aportado por el usuario en MAME
0.276, BIOS euro. Los gráficos se decodifican de C-ROM; las capturas PNG se usan
para comprobarlos, no como texturas del port. El render nativo usa CI4, índices
originales y paletas cuantizadas a RGBA5551, con pen 0 transparente.

1402 celdas de 16x16, 22 paletas del mapa, cámara observada 0..872. Las franjas
de SCB 1..32, 33..64 y 105..136 corresponden a tres capas activas. La franja
65..104 no aportó celdas visibles en esta captura. Con cámara C, los desplazamientos
son `(C+1)/2`, `(3*C+2)/4`, C y C, mediante división entera. La fase de
autoanimación conserva las máscaras originales de 4 u 8 frames. El color de
fondo procede de la entrada 4095 de la paleta: evita huecos negros entre capas.
Se conserva la ventana activa de 304x224, situada en x=8..311, y=8..231 de N64.

Los tiles se agrupan por paleta en atlas CI4 de 64x64 (2048 bytes), 16 tiles
por atlas. Las celdas de una capa no se superponen y se ordenan por página para
reducir uploads. Las capas conservan su orden. Cada rectángulo toma una subregión
16x16 del atlas; el TLUT utiliza la mitad superior de TMEM. Se solicita un margen
de 48 píxeles y la siguiente fase animada antes de dibujar.

El conjunto completo, incluyendo actores y los recursos anteriores, ocupa
230 páginas, 72 paletas y 29 chunks: 475136 bytes decodificados, 272384 bytes
comprimidos. Cabe en las cachés configuradas para 4 y 8 MiB. La prueba de esta
zona no ejercita evicción ni demuestra streaming del juego entero.

## Movimiento

La escritura de RAM original identifica posición X/Y en 0x1014EC/0x1014F0 y
velocidad X/Y en 0x1014F4/0x1014F8. Los valores son Q16.16; Y crece hacia arriba.
La rutina aérea observada se ejecuta cada dos ticks de vídeo, aproximadamente
29,592803 actualizaciones/s. No se extrapola esta frecuencia a todas las
entidades del arcade.

El salto inicializa Vy a 0xA9000 (10,5625). En cada actualización:

```
Vy += ASR((-22 << 16) - Vy, 5)
Vx += ASR(target - Vx, input_horizontal ? 2 : 5)
X += Vx
Y += Vy
```

`target` es -4, 0 o +4 en Q16.16. `ASR` redondea los negativos hacia menos
infinito; no equivale a una división C que trunca a cero. La implementación
portable está en `src/neo_motion.h`. La ecuación procede del comportamiento
observado y la rutina de la ROM: inicialización 0xC2684, actualización vertical
0xC2696..0xC26A2 y horizontal 0xC26AA..0xC26DA. No se distribuye el programa
68000 extraído ni se requiere BIOS Neo Geo en el ejecutable N64.

La prueba compara X, Y, Vx y Vy, bit por bit, con 22 estados originales de
frames 2302..2344, partiendo del frame 2300. No prueba colisiones ni todas las
acciones. Las pulsaciones breves se retienen hasta el siguiente paso de jugador.
Las animaciones siguen el tiempo transcurrido desde el salto.

El suelo usa 352 muestras de la marcha original, interpoladas entre X. La
posición inicial es la capturada (X≈120,7299, Y≈94,1825); el pueblo tiene Y=64.
La conversión a pies N64 es `248-Y`. Esta tabla **no sustituye todavía al
resolvedor original de colisiones**. La marcha usa 3 píxeles por paso; el arcade
reduce a 2,90625 en ciertas pendientes, comportamiento todavía pendiente.
También siguen pendientes los pivotes y la selección exacta de todas las poses.
La cámara nativa avanza sin retroceder y queda limitada al tramo recuperado.

## Variantes del escenario y límites

63 coordenadas cambian de tile/paleta durante las capturas. El extractor conserva
la primera variante y registra las demás, en lugar de elegirlas por mayoría.
Esas transiciones dinámicas aún no se reproducen en el juego N64. La validación
informa de sus diferencias de píxeles por separado.

Se retiraron las cajas y banderas de prueba. Los tres guardias, proyectiles,
daño, checkpoint y reaparición siguen siendo lógica provisional. `REGION END`
marca el límite observado; no representa el final original de la misión.
Faltan el encuentro que bloquea el avance en el arcade, el resto del mapa,
colisiones originales, HUD, audio, granadas, vehículos, enemigos y jefes.

Los overrides de invulnerabilidad y tiempo de los scripts MAME se usan solo
para obtener referencias; no se incorporan a la ROM N64 ni se toman como
evidencia de comportamiento original del combate.

## Regeneración y pruebas

Con los assets anteriores de docs/RDP_IMPLEMENTATION.md y Pillow disponibles:

```sh
python3 tools/extract_mission_map.py /workspace/research/mission1
python3 tools/validate_mission_map.py /workspace/research/mission1
make check
make -j4
```

Las capturas se generan con tools/capture_mission.lua y tools/capture_ground.lua
en MAME, definiendo MSX_CAPTURE_DIR; requieren las ROM locales del usuario.
`data/mission1-ground.csv` y `data/original-air-trace.csv` contienen únicamente
coordenadas/velocidades medidas. No borrar los assets ignorados de filesystem/.

La validación de 36 frames comprobó 2451456 píxeles reconstruidos: 17227 difieren
dentro de coordenadas con variantes registradas; ninguna diferencia fuera de
esas coordenadas. Además coinciden 1741513 píxeles visibles de MAME, excluyendo
sprites superpuestos y píxeles FIX; tolerancia RGB por canal <=1. El port N64
cuantiza después a RGB5551, de modo que esa cifra valida la extracción RGB8,
no igualdad entre fotografías o salidas de vídeo N64 y MAME.

Para ares 146 y el entorno X11 retenido:

```sh
bash tools/run_ares.sh > /workspace/emulator/run-v9-release.log 2>&1
# Desde otra terminal, con ares recién iniciado:
DISPLAY=:99 LD_LIBRARY_PATH=/workspace/emulator/root/usr/lib/x86_64-linux-gnu \
  python3 tools/test_mission_ares.py
```

Se recorrió la ROM: arranque, salto en carrera, dunas, pueblo, combate provisional,
límite de región, reinicio, salto a izquierda y laboratorio/retorno. Sin asserts;
29 lecturas PI, cero misses después del precalentamiento y cero evicciones.
Muestras del log: 398..458 draws, 28..44 uploads de textura/frame; 56..89 KB a
TMEM y envío CPU de unos 8,6..10,2 ms en ares. No es una medición de rendimiento
en N64 física. Capturas: /workspace/emulator/v9-release/.

MiSTer v9 y consola N64 pendientes de validar.
