# Jefe de la Misión 2 (etapa 3, `026254`): port nativo

El jefe de la etapa 3 de la Misión 2 (registro de guion `2D9CA2`, rutina
`026254`) corre en C nativo como actor genérico: `src/neo_m2boss.c`
(traducción manual de `026254..027E8E` y de la música diferida
`059F4E/059F76/059FA8`; las etiquetas de ROM son procedencia). El mundo lo
crea por la vía genérica (`neo_region_routine`), así que la auditoría del
guion de la Misión 2 ya no lo pierde.

## Captura (por tramos)

`capture_fullgame.lua` desde `at_92700` (estado guardado por
`MSX_FULL_SAVE_AT`, ver [MISION2_ACTORES.md](MISION2_ACTORES.md)). Nuevo
`MSX_FULL_FAMILY_RANGE=<lo>-<hi>` (solo captura): la familia incluye todo
nodo despachado con una rutina del intervalo, para empezar un tramo con el
combate ya en marcha. Cada tramo reproduce desde 92700 sin trazar y traza
unos 3000 fotogramas (≈ 170 MB); se extrae su fixture, se reduce la traza a
las líneas `U/V/F/B` y las escrituras de la lista de sprites (`W` en
`00A078`, para los píxeles) y se borra el resto (disco):

```sh
export MSX_FULL_STATES=/home/user/captures-m2en5/states MSX_FULL_LOAD=at_92700 MSX_FULL_LOAD_FRAME=92700
MSX_FULL_FAMILY_ROOTS=26254 MSX_FULL_FAMILY_RANGE=26254-27e8e MSX_FULL_FAMILY_FROM=<desde> MSX_FULL_VRAM_TRACE=1 \
  MSX_FULL_FIRST_DUMP=<desde+30> MSX_FULL_PERIOD=60 MSX_FULL_GRID_TAP=0 MSX_FULL_LAST=<hasta> sh tools/run_capture_fullgame.sh <dir>
python3 tools/extract_family_fixture.py <dir>/boss-trace.txt --output data/m2bossN_fixture.json.gz \
  --range 26254-27e8e --range 59f76-59fa8 --range 1c6ee-1c796 --range 1c9e6-1c9e6 --range 1cf94-1d1b0 \
  --range 1d764-1d7b8 --range 1d31c-1d6e4 --attacks --ram 10000e-100010 --ram 10008a-10008c --ram 10f008-10f00a \
  --ram 10e99a-10e99e --ram 10e9ba-10e9bc --ram 1013d6-1013de --ram 10c354-10c356 --ram 10c2f6-10c2f8 \
  --ram 10c300-10c308 --ram 10c33c-10c33e --ram 10c608-10c650 --ram 1014c4-1014c8 --ram 101500-101506 \
  --ram 101514-101518 --ram 101548-10154a --ram 10155e-101560 --ram 101574-101578 --ram 1015b0-1015b6 \
  --ram 1015c4-1015c8 --ram 1015f8-1015fa --ram 10160e-101610 --ram 10c7b6-10d7b6 --ram 1063d8-1063da
```

El estado `at_92700` se carga con el Lua recién iniciado (contadores del
plan de entradas a cero), así que esta partida diverge de `run4`: el jefe
aparece en 93083 como allí, pero muere en **113531** (no en 117325).

| Fixture | Tramo | Actualizaciones | Contenido |
| --- | --- | ---: | --- |
| `m2boss1` | 92900..95600 | 18550 (379 nodos) | aparición, subida, apertura, primeras esperas y disparos |
| `m2boss2` | 95600..98600 | 24125 (447) | ciclo de ataques, disparos dobles `0271EC` |
| `m2boss3` | 98600..101600 | 24900 (472) | ciclo de ataques |
| `m2boss4` | 101600..104600 | 23805 (463) | ciclo de ataques |
| `m2boss5` | 104600..107600 | 24105 (522) | ciclo de ataques, fase 2 |
| `m2boss6` | 107600..110600 | 22963 (501) | ciclo de ataques |
| `m2boss7` | 110600..113600 | 24240 (551) | destello `0267FA`, columna `02701E`, muerte (113531) |
| `m2boss8` | 113600..117500 | 1295 (24) | hundimiento, `018F40`, borrado `026E7A` |

## Módulo nativo

- **Cuerpo** `026254`: capa 8838, música 0x27 tras la espera (`059F4E`),
  `+78` = progreso (`027BC6`), suelo, `01B1DC`, `0143E4`, y fijada en 0x60,
  caja `027BFC`, vida `2C0C80` (`+7E` = mitad: fase 2, `+8F` bit 7), paletas
  0x57/0x5B/0xB2 y destello 0x59, `+6E` = `027E8E`, piezas `027E34`
  (`027A32`), programa `23502C`, sonido 1020. Sube (`026354`), se abre
  (`0263B6/026400/02644A`) y cicla por `027936` (tabla `0264AE`: espera
  `0264BE`, disparos `0269F2`, temblor `026550`, destello `0267FA` con la
  columna de explosiones `02701E`). Cola `026B78/026B82`: fila `027C14`,
  `00427A`, fase, RAM 10E9BA (0x1A0, 0x110 con `+8F` bit 4), `019388`
  (velocidades del actor de misión 1013D6/1013DA = 0xA0000), 10E99A,
  `004C24`, progreso > 0x7E00 lo mata, `+5F` bit 7 → `026C30`.
- **Muerte** `026C30..026E7A`: sonido 1033, paro de 1099/109C (`012192`),
  `018F92` (fin de misión), piezas `027E7A`, destellos blancos (`009BB0`),
  escombros `027F14/027F02` (`01DA40`) y `01DBAA`, se hunde, `018F40` con
  y < 0x60 y `026E7A` lo borra.
- **Piezas** `026F6C/026FCE` (filas `027C74` por `+87`): programa
  `027D74`, paletas `027E14[027DF4]`, colocadas sobre el cuerpo (`01AD6E`),
  lo siguen (`027698`: `+1E`, posición por `+84` del cuerpo, paleta por
  `+8C`, destello `0044F6`) hasta que cambia el `+88` del cuerpo (`0041AC`).
  Llamadas de programa: `027734` (velocidad `+26`), `01ADB2`, `027BBA`
  (escombros `027EB0`).
- **Columna** `02701E/027112`; **disparos** `02715A/0271EC` (apuntan al
  jugador con `01B04A` + `01B14A`/`01B1C0`, giran de 8 en 8, fila `027C44`,
  corte `027C64`, `027616` → `01C706`).
- **Música diferida** `059F76/059FA8`: misión (`05A054`, RAM 10C2F6),
  espera `E11AA`, orden 0x0A del sonido (`01228C`, palabra `0x0A00|n`) y
  el sonido `+72` al acabar la espera (`003B4C`: un fotograma por
  actualización en el mundo).
- `0260C8` de tipo 2 (las tablas de elección del jefe) es aleatorio: el
  byte de la fila elige un grupo de 8 palabras y `0029C6 & 0xE` la palabra
  (`ga_rank_word` devuelve el byte; se deja así para no tocar la Misión 1).
- Servicios: `neo_m2boss_env_t` en `neo_genv_t` (RAM 10E99A, 10E9BA,
  1013D6/1013DA, 10C354 y el paso de `003B4C`); el mundo lo enlaza con la
  cámara nativa (`699A`, `69BA`, `speed_x/y`) y el modo de la torre.
  Reutiliza lo nativo: `01ABC8`, `01AC38`, `0143E4`, `0146E8`, `004256`,
  `00427A`, `009EA8`, `004520`, `0049E8`, `004C24`, `01945C`, `0040BC` +
  `01B220`, escombros `01D764/01CF94..01D000/01D1B0`, `01D31C..01D6E4`,
  `01C706/01C9E6` (`neo_effects.c`), ángulo `002EB4`, seno `0029FC`.

**Paradas explícitas** (registradas con `ga_fail`, como los nodos del jefe
de la Misión 1): `0273AA` y `027440` (disparo con un jugador montado en un
vehículo, `0AAA88` ≠ 0), `026BBC`/`059F76` sin `neo_m2boss_env_t`,
`026FCE` sin creador, `026118` (fila de rango vacía) y cualquier callback
de programa no traducido. No se portan el chequeo de la protección
(`026E96..026F1A`, `059FC6..05A04A`: RAM 10F00A/10F00C) ni las rutas de
`02618A` a las que solo lleva ese chequeo.

## Verificación

`make check` (objetivo de familias): `m2boss1..m2boss8` con
`--m2boss --late-creator --attacks --terrain 10c72e --ring 10c72e
--group-map b7000`. Bucle cerrado: cada nodo nace cuando el código nativo
lo crea (piezas, disparos, columna, escombros, explosiones, música); los
nodos vivos al empezar un tramo arrancan de sus bytes grabados con su
creador (`+0C`) y su programa (`+20`, dirección de ROM del siguiente
comando → programa y cursor nativos). Se comparan los nodos `+10..+AF`, las
respuestas de `0049E8`, los globales vigilados (destello 109944, fin de
misión, temblor) y la RAM de los `--ram` (10E9BA, 1013D6/1013DA, 10C354…;
los contadores 10F008/10F009 de la interrupción vertical no). El
extractor descarta la actualización abierta al final de la traza (cortada
por el último fotograma, no un borrado).

**Resultado: las 8 familias pasan**: 163983 actualizaciones en bucle
cerrado, 3359 nodos, todos creados por el código nativo salvo los vivos al
empezar cada tramo (de 2 a 9 por tramo), 0 hijos fuera de la familia.
Detalles de la prueba (`test_neo_family.c`, solo con `FAMILY_M2BOSS`):

- `0040BC` con reserva: `spawn_reserve` niega el nodo si los libres (RAM
  1063D8 tal como la leyó la actualización, uno menos por nodo creado) no
  superan la reserva (`0x1C` piezas, disparos, columna, escombros; `0xE`
  las piezas con la fila `[7]` ≠ 0; `0x2A` `01DBAA`; 0 la música). Sin
  esto el tramo 3 crea piezas que el original no pudo crear.
- Hijos de una actualización repartidos en dos fotogramas (la marca de
  fotograma cae en mitad del recorrido de la lista): se enlaza el primero
  creado con el último de ellos en la lista.
- Datos de ROM nuevos (`region_rom.json`): `027BFC..027F30`, `027926`,
  `2C0C80..2C137F`, `E11AA`, y las cajas del jugador `2D0AA8/2D0AC0`
  (`01B162`, puntería de los disparos en fase 2). Programas nuevos en
  `soldier_programs.json` (raíces de `extract_soldier_programs.py`).

## Píxeles

`export_rest_sprites.py` con las raíces y paletas del jefe (programas
`23502C..235762`, paletas 0x57..0x61, 0xB1..0xB3, 0x223) en
`filesystem/mission2/rest-native.json` (privado, `--keep` del anterior) y
la nueva opción `--mask-auto`: las entradas autoanimadas del LSPC
(atributo bits 2/3, el exportador no las rasteriza) ya no descartan el
volcado entero, solo su área. Tramos `p1`, `p4`, `p7`, `p8` (volcados cada
60 fotogramas):

```sh
python3 tools/export_rest_sprites.py mslugx.zip <dir>/p1@26254-27e8e <dir>/p4@26254-27e8e <dir>/p7@26254-27e8e \
  <dir>/p8@26254-27e8e --mask-auto --output filesystem/mission2/rest-native.json --keep <export anterior>
```

**149 volcados, 1974572 píxeles iguales, 2391 distintos en 8 volcados**
(95580: 280; 102000, 103560, 104580: 9/10/63; 112200: 1846, 112380,
112440, 112500: 128/1/54, durante el destello y la columna de
explosiones; sin explicar uno por uno). 1260105 píxeles con dos piezas
del jefe superpuestas no se comparan (el orden del hardware decide) y 299
entradas autoanimadas quedan fuera.

## Pendiente

- La reaparición del jugador y el resto de la etapa tras `018F40` siguen
  el flujo de misión existente; la integración en partida (cámara de la
  arena, colisión del jugador con el cuerpo) no se ha jugado entera.
