# Núcleo de colisión nativo para los soldados de la región jugable

Los informes anteriores ([SOLDIER_CONTACT.md](SOLDIER_CONTACT.md),
[SOLDIER_LOCOMOTION.md](SOLDIER_LOCOMOTION.md)) usaban subconjuntos semánticos
del kernel genérico `0x013968`: suelo de un solo plano, vuelo sin contacto y
las respuestas de las consultas delanteras introducidas desde la captura. Este
paso traduce a C el propio kernel para el contexto de objetos observado en la
región jugable de la Misión 1. No se interpreta código 68000 durante el juego.

## Traducción

`src/neo_kernel.c` traduce manualmente estas rutinas, conservando el orden de
operaciones, la aritmética de palabra/doble palabra, los acarreos y las
banderas que el original deja en RAM de trabajo:

| ROM | Función |
| --- | --- |
| `0x013968..0x013C18` | Barrido completo: recorte de desplazamiento, recorrido de objetos, apoyo inferior, reintento por objeto y salida (posición, velocidad, padre, banderas, ángulo). |
| `0x013776`, `0x013214`, `0x01351A` | Avance por celdas horizontal y vertical, con límite de una celda por paso y alternancia ponderada por `+0x4028`. |
| `0x012938` | Clasificación de celda: grupo por la tabla de `0x10C00E` (ROM `0x0B5332`), forma, grupo `0x0B` como formas 16..29 y bandera `0x10`. |
| `0x0129EC`, `0x012AD4`, `0x012CBA` | Tablas por forma: filtro por sentido del movimiento, distancia horizontal y vertical. |
| `0x012EE2`, `0x013204` | Respuesta a la superficie: proyección sobre pendientes con redondeo `ASR #4`+`ADDX`, bloqueos `+0x4024..+0x4027`. |
| `0x012E64` | Palabra de ángulo. Sin contacto, el índice inicial `0x4000` lee la palabra de ROM `0x016E6A` (`0x0028`), dato importado. |
| `0x013DB2` | Búsqueda de suelo hacia abajo usada al crear soldados (`0x0146E8`). |
| `0x01AC4C` | Cambio de marco entre objetos, con posición anterior para el origen y actual para el destino. |

Dos palabras de trabajo **persisten entre llamadas** y forman parte del estado
de escena nativo: `0x10C018` (escrita por `0x013DB2`/`0x013C1A` y leída por
los barridos) y `0x10C068` (escrita por la respuesta de superficie y devuelta
en `D4.w` por cada barrido). No se reinician por llamada porque el original no
lo hace.

La celda se obtiene de la cuadrícula nativa (`neo_collision_cell`). Se comprobó
que el anillo de 64×64 celdas del objeto `0x10C72E` coincide con ella en la
ventana cargada de todas las capturas (filas por encima del mapa de 256 px
vacías); las únicas diferencias están en columnas ≥149, fuera del mapa nativo.

## Contexto de objetos verificado

En los fotogramas de la región, la lista de objetos de colisión contiene el
mapa `0x10C72E` (descriptor ROM `0x0B533A`, sin control de rango porque
`+0x0C` es cero), el segundo mapa `0x10D7B6` inactivo (descriptor con `+4`
cero) y ningún objeto dinámico: los mapas de bits `0x10C26E..0x10C291` son
cero en todas las capturas. El padre "raíz" `0x10E968` tiene las mismas
posiciones que el mapa. Un barrido que toque otro objeto (el camión posterior
al bloqueo de cámara) se excluye del fixture y se cuenta; el código nativo
devuelve `false` ante objetos desconocidos o el modo de límites del jugador
(`+0x400D`), en vez de improvisar.

## Evidencia

Cuatro capturas nuevas con MAME 0.264 (`mslugx`, BIOS euro), solo entradas de
mando y breakpoints que imprimen: `walk3` y `fire3` (cronología del primer
soldado: A en 1450, derecha desde 2280; `fire3` dispara alternando desde 2190)
y `entry-walk3`/`entry-fire3` (cronología de la entrada en paracaídas: moneda,
Start y confirmación en 3620, derecha desde el traspaso de 4062).
`tools/mame/trace_soldier_kernel.lua` registra registros y modo en
`0x013968`/`0x013DB2`, la salida en `0x013C18`/`0x01471C`, las posiciones de
los objetos y las dos palabras persistentes, solo para actores cuya rutina
pertenece al soldado (`0x0AE000..0x0B1000`) o a sus muertes (`0x040D00..0x041000`).

`tools/extract_soldier_kernel.py` genera `data/soldier_kernel_fixture.json.gz`
(columnas con nombre, comprimido). Se excluyen 28 llamadas que tocan columnas
fuera del mapa nativo de 149 columnas y los fotogramas del camión.

**Resultado:** `tools/test_neo_kernel.c` reproduce exactamente **6979 barridos**
(4383 con contacto) y **24 búsquedas de suelo**: posición, velocidad, padre,
banderas `D6.w`, palabra de grupo/ángulo `D7` y `D4.w`. Incluye pasos de suelo,
consultas delanteras `0x0148C2`/`0x014850`, caídas y saltos de soldados,
caídas de muerte y los anclajes de creación.

```sh
MSX_CAPTURE_DIR=/ruta/walk3 MSX_CAPTURE_END_FRAME=3400 \
  mame mslugx -rompath /ruta/roms -bios euro -video none -sound none \
  -skip_gameinfo -nothrottle -autoboot_delay 0 \
  -autoboot_script tools/mame/trace_soldier_kernel.lua \
  -cfg_directory /ruta/cfg -nvram_directory /ruta/nvram -debug -debugger none -debuglog
# fire3: MSX_FIRE_FRAME=2190; entry-*: MSX_CONFIRM_FRAME=3620 MSX_RAM_FIRST=4000
#        MSX_RIGHT_FRAME=4062 MSX_CAPTURE_END_FRAME=5400 (y MSX_FIRE_FRAME=4062)
python3 tools/extract_soldier_kernel.py /ruta/walk3:3320 /ruta/fire3:2880 \
  /ruta/entry-walk3:4970 /ruta/entry-fire3:4660 --output data/soldier_kernel_fixture.json.gz
make check
make kernel-regression.z64
```

Las capturas (RAM por fotograma, registros completos) quedan locales en
`/home/user/captures-soldiers`; en Git solo hay scripts, el fixture y sus hashes.

## Límites

No se cubren objetos dinámicos (plataformas, vehículos registrados), el segundo
mapa activo, la rama de límites del jugador `0x0138A0`, ni la búsqueda
`0x013C1A`. Las formas cuyo recorrido no aparece en las capturas están traducidas
desde las tablas, sin cobertura dinámica propia. Fuera de las 149 columnas
nativas el resultado depende de la extensión del mapa en curso.
