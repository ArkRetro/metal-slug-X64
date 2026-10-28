# v9: escenario original observado y movimiento aéreo

> Actualización: el tramo jugable llega ahora a la cámara 1920 (antes 872).
> Véase [la ampliación](#ampliación-hasta-la-cámara-1920) al final.

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

## Ampliación hasta la cámara 1920

El tramo jugable termina ahora en la cámara 1920: `MSX_WORLD_WIDTH` 2240 y
límite derecho 2212−1/65536 (cámara+292, la misma fracción medida en 1164).
Los tres guardias, el checkpoint y `REGION END` siguen siendo provisionales;
solo se ha movido el final. No es el final de la misión original.

Datos: 36 capturas del tramo anterior (MAME 0.276) y 86 de
`tools/capture_advance.lua`, repetidas con MAME 0.264 (fotogramas 2100..7200).
La posición del jugador coincide byte a byte en las tres ejecuciones con esa
entrada. El extractor obtiene 2719 celdas, 48 paletas, 63 coordenadas con
variantes y cámara máxima 1920.

`validate_mission_map.py` comprueba los 122 fotogramas: 8307712 píxeles
reconstruidos, 24236 diferencias dentro de variantes registradas, ninguna fuera,
y 4952880 píxeles descubiertos de MAME con tolerancia RGB ≤1. La cifra anterior
(4953328) procedía de la captura con 0.276; mapa y variantes coinciden.
Resumen en [mission-region-validation.json](mission-region-validation.json).

### Cámara

`src/mission_camera.h` contiene el modelo nativo:

- Banco medio: a 3/4 hasta la cámara 1022; desde 1023 se recarga a velocidad
  completa (capa 2). Medido en dos trazas con entradas distintas.
- Altura: `ceil((C-944)/4)`, limitada a 0..16, para fondo y actores; el cielo usa
  la mitad y el banco a 3/4, tres cuartos. Coincide con los nueve cambios de la
  traza con saltos (`tools/capture_camera.lua`, `MSX_CAMERA_LAST=7200`).

No verificado o no reproducido: sin saltos (`MSX_CAMERA_JUMP=0`) la altura se
detiene en 14, no en 16; el original sigue también al jugador (subida temporal
a 23 en la cámara 1550..1607) y eleva otros 16 px en la cámara 1920 por un
evento no recuperado. `tools/compare_camera_model.py` da 67/122 fotogramas
idénticos; 10 difieren 1 px en la capa a 3/4 (aproximación ya existente) y 45
por esos casos de altura. Las capturas de esos 45 validan la extracción, no
el modelo de cámara.

### Presupuesto de streaming

Paquete completo: 296 páginas, 101 paletas, 37 chunks; 606208 bytes
decodificados y 388352 comprimidos (antes 235/75/30, 491520 y 272832).
Con 8 MiB la caché de 3 MiB lo contiene entero. Con 4 MiB (512 KiB, 32 chunks)
ya no: el arranque carga la vista inicial al final para que el LRU la conserve,
y el resto entra por prefetch. `tools/check_scene_budget.py`, dentro de
`make check`, calcula 25 chunks como peor conjunto de trabajo (cámara 1056),
con los bancos de Marco fijados, los ocho fotogramas de animación y una
ventana de 16 px de cámara. Falta medirlo en ares y en hardware de 4 MiB.

### Regeneración

```sh
MSX_CAPTURE_DIR=/ruta/advance mame mslugx ... -autoboot_script tools/capture_advance.lua
python3 tools/extract_mission_map.py /ruta/mission1 /ruta/advance
python3 tools/validate_mission_map.py /ruta/mission1
python3 tools/extract_region_collision.py /ruta/mission1 /ruta/advance \
  --expect-prefix /ruta/collision-149.json
make check && make -j4
```

Las pruebas host recorren hasta 2212, comprueban cámara monótona y acotada,
suelo Y=64, aterrizaje sobre el tejado Y=144 y la altura de cámara. La
integración interna de entrada con audio y recorrido hasta cámara 1920 pasa en ares
(ver `docs/PR2_REVIEW.md`). La prueba en MiSTer y en 4 MiB sigue pendiente.
