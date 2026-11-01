# Misión 2, fase 2: actores y datos (rama `work-m2act`)

Lado de actores y datos de la fase 2 ([MISION2.md](MISION2.md)). El motor
(cargador por misión, `neo_world_init`, desplazamiento vertical) va en
`work-m2eng`. Misma regla: lo traducido tiene evidencia de MAME; lo demás
para y se cuenta.

## Trazas de familia desde la captura completa

`tools/capture_fullgame.lua` acepta ahora `MSX_FULL_FAMILY_ROOTS` (lista
hex de constructores), `MSX_FULL_FAMILY_FROM` (fotograma) y
`MSX_FULL_VRAM_TRACE=1`: escribe `boss-trace.txt` con el formato de
`capture_mission_rest.lua` (U/V/R/W/X/A/B/F/Z), así que
`extract_family_fixture.py` y `compile_family_fixture.py` sirven sin
cambios. `MSX_FULL_IDLE=lo-hi` (solo captura) suelta todos los mandos.
Se arranca desde los estados de `captures-fullgame/states4`:

```sh
export MSX_FULL_LOAD=sec01_19391 MSX_FULL_LOAD_FRAME=19391 MSX_FULL_STATES=/home/user/captures-fullgame/states4
MSX_FULL_FAMILY_ROOTS=3aed8,3aef0,5287c,b6ed2,b6f6a MSX_FULL_FAMILY_FROM=19393 MSX_FULL_VRAM_TRACE=1 \
MSX_FULL_FIRST_DUMP=9999999 MSX_FULL_GRID_TAP=0 MSX_FULL_LAST=21000 sh tools/run_capture_fullgame.sh /home/user/captures-m2act/tr1
# tr2: MSX_FULL_FAMILY_ROOTS=5287c MSX_FULL_IDLE=19430-20400 MSX_FULL_LAST=20400
```

La reproducción desde el estado coincide con `run4` (3AED8 en 19862,
sección 2 en 19932). Unos 20 s por ejecución.

## Nativo

| Actor | Fichero | Prueba |
| --- | --- | --- |
| `3AED8` entrada / `3AEF0` salida (tipo 4) | `src/neo_wipe.c`, llamado por `neo_mission.c` | `test_neo_wipe`: 70 actualizaciones, +72 y **22800 escrituras del mapa fijo** iguales; `test_neo_mission`: 12 esperas de 35 actualizaciones, 0 paradas |
| `05287C` flecha GO (todas las etapas) | `src/neo_goarrow.c` | familia `goarrow` (tr2, jugador quieto): 486 actualizaciones, 176 con la flecha |
| `0B6F6A` marcador de salida de la etapa 1 | `src/neo_goarrow.c` | familia `exitmark` (tr1): 84 actualizaciones |
| `0613CA/0613D8/0613E6` rebeldes (variante en +96), disparo `061E8C`, destello `0620AA` | `src/neo_rebel2.c` | familia `rebel2` (etapa 2, `a2`): 41 nodos, **4815 actualizaciones**, 41 borrados, 4 contactos |
| `0678B6/0678C0/0678CA/067902` y `0692B2..0692E4` soldados (variante en +9C), cuchillo `067FC0`, destello `021CB6` | `src/neo_rebel3.c` | familia `rebel3` (etapa 2, `b1`): 30 nodos, **3246 actualizaciones**, 30 borrados, 47 contactos |

`3AED8`/`3AEF0`: cursor +72 de −30 a 0x46 en pasos de 3; cada actualización
rellena las columnas desde el cursor (filas 1..30, VRAM `7000+32c+f`) con la
tabla `E0818`/`E0856`; al llegar pone +60 = 1 en el creador y se borra
(`004188`). La comprobación anticopia de `03AF20` (`10F00A/10F00C`,
`2FFFE0`) no se traduce. El mapa fijo queda en `neo_wipe_t.fix` (40×32):
**el renderizador aún no lo dibuja** (fuera de este alcance). Las 37/35
"actualizaciones" de MAME son pares de fotogramas que incluyen el retraso de
la carga de etapa; en actualizaciones del actor de misión la espera es 35.

`05287C`: anclada en (108, A0) de la raíz 10E968, paleta 0x125, programa en
reposo `24953C`; si el progreso de cámara 10E99E está quieto 0xD2
actualizaciones, sin parada de cámara alcanzada (`0C7B26`, 10E9BC), sin
grupos vivos (`019DE4`), sin pausa (10C300/10C301) y con un jugador
(`01AEB0`), toca dos veces la flecha de la tabla `05285C` según RAM 10C44A
y espera 60 actualizaciones. Campos nuevos de `neo_genv_t`: `camera_low`
(10E9A0), `camera_stop` (10E9BC; el mundo debe rellenarlo, 0 deja la flecha
en reposo) y `arrow` (10C44A). Falta que el mundo la cree en `01860A` y que
exista `0529C0` (dirección de la flecha, 10C44A).

Sprites: `export_rest_sprites.py --output filesystem/mission2/rest-native.json`
(privado; incluye los de la Misión 1 con `--keep`). Paletas 0x125 y 0x461
exportadas; el marco del marcador (`1CDEEE`, 64×141) y los de la flecha.
**Comprobación de píxeles incompleta**: el reposo de la flecha es un marco
vacío, los marcos GO usan la autoanimación del LSPC (atributo 8804, el
exportador y el renderizador no la implementan: 36 entradas saltadas) y el
marcador está fuera de la pantalla en los volcados.

## Rebeldes `0613CA/0613D8/0613E6` (`neo_rebel2.c`)

Traza (estado `sec02_19932`, unos 100 s):

```sh
export MSX_FULL_LOAD=sec02_19932 MSX_FULL_LOAD_FRAME=19932 MSX_FULL_STATES=/home/user/captures-fullgame/states4
MSX_FULL_FAMILY_ROOTS=613ca,613d8,613e6 MSX_FULL_FAMILY_FROM=22600 MSX_FULL_VRAM_TRACE=1 \
MSX_FULL_FIRST_DUMP=9999999 MSX_FULL_GRID_TAP=0 MSX_FULL_LAST=26139 sh tools/run_capture_fullgame.sh /home/user/captures-m2en/a2
python3 tools/extract_family_fixture.py /home/user/captures-m2en/a2/boss-trace.txt --output data/rebel2_fixture.json.gz \
  --range 613ca-62113 --attacks --ram 10000e-100010 --ram 10c7b6-10d7b6
```

Traducción de `0613CA..062634`: constructores `062114/062190/06224E`
(palabras del guion a +76/+90/+92/+9B/+9C, rango `2D4A60..` por `0260C8`,
+8A/+8C con ruido del RNG), reposo `06173C`, jugador más cercano en +6E
(`01AF9A`/`01AEDA`, comparaciones largas de X: nuevo `player_low` de
`neo_genv_t`, RAM 101502), andar hacia/desde el jugador (`0615A6`/`061670`),
giro `061808`, disparo `061AE4` (llamada `062422`: tiro `061E8C`), saltos de
la variante 2 por la tabla `2D4CCC` (`06243E`/`06247E`), destello previo
`0620AA` (+9C), caída `061B70` (+9B), cola común `061BE8` (+84, recorte
`E121E`, +9D, chispa `0682F0` tras el actor del guion, muerte por +5A:
`061C6E`, `061D46`, `061E2E` con el objeto +90/+92 de `08CDB2` desde la
llamada `0624C4`). Paradas explícitas: `061E16` (+5A = 0x0A, piezas
`068134`, no aparece en la traza) y la comprobación anticopia de `06201C`
(10F00A/10F00C, 2FFFE0) antes de `004188`.

Banco de familias (para las etapas verticales): `--ring 10c72e` hace que el
núcleo lea las celdas del anillo 64×64 de la capa (`0C79FE`, +88) con los
bytes que leyó el original (`neo_kscene_t.ring`; NULL en el juego, sin
cambio en la Misión 1), `--group-map b7000` (RAM 10C00E de la Misión 2:
`0B7000`, entrada 6 = 0x0A), mapa dinámico `E1B6A` en `region_rom.json`,
y la tabla de cuerpos sólidos `106756` leída por `004E76` (+6B = primera
entrada libre). `--grid` (rejilla de etapa privada) queda disponible.

Sprites: raíces `265C8A..266A38` y paletas 0x1F6, 0x1F7, 0x811, 0x84A y
0xB0 en `filesystem/mission2/rest-native.json` (paletas de los volcados de
`run4` 22650..26100). **Píxeles** (captura `av`, volcados cada 15
fotogramas 22650..23490 con la traza): 55 volcados, 89228 iguales, 4373
distintos concentrados en 7 volcados: tres tomados en el fotograma de la
actualización y los de 23100..23130, con los destellos `0620AA` en pantalla
(sin explicar del todo).

## Salida de la etapa 1 `0B6ED2` (`0B6F24`, `neo_goarrow.c`)

Creada por `0B6E06` (parches de mapa `0C70FC` y marcador `0B6F40`) cuando
se rompe la puerta `03889C` (1035C4, tabla `038A5A` por +6E; **no
portada**): raíz, (0x280, 0x38), +6E = rectángulo `0B6ECA` (−12, −1, 0x80,
0x32), +72 = 0x2A0. `0B6ED2`: con un jugador en juego (`0193FE`; `0C8D0C`,
sin jugador vivo, es parada explícita) llama a `018F92` (`004BFE`,
`004C06`, borra el actor del guion 101414 con `0041D0`: el mundo deja de
ejecutar el guion; `0C09D8`: +84 = FFFF, +5F bit 6, +9A bit 2 de cada
jugador; 10C300 = FF, 10C304 = +5E, manejadores `0C0538`, 10C306/10C307
por `0AAA88`: con vehículo es parada), `0C09D8`, `0C8CD6`, 10C00B = FF y
`0CA340` ×2 (10EA00/10EA02 = +72, manejador `0CA3EC`). `0B6F24`: `01ABC8`
y, si todos los jugadores en juego (10C33C) están dentro del rectángulo
(`01B2CE`/`01AF3C`), `018F40` (10C303 = 0). Campos nuevos de
`neo_ga_flags_t`: `walk_to`, `mission_end`, `end_kind`, `scroll_lock`,
`stage_done`, `player_guard`; `player_handlers` vale 2 (`0C0538`) o 3
(`0CA3EC`).

Familia `stageexit` (tr1, `--range b6ed2-b6f3f`, más los bytes de los
jugadores que leen `0193C0`/`0C09D8` con `--ram`): 84 actualizaciones
(19741..19929), con el disparo y `018F40` desde 19859. El banco compara ya
cualquier número de escrituras globales por actualización (`<familia>_writes`)
y, con `globals = 2` en la extracción, 1067E5, 10EA00..10EA03,
10C300..10C307 y 10C00A/10C00B (10C303 empieza en FF).

Juego (`gameplay.c`): el manejador 3 sustituye la entrada del jugador
(en el suelo, hacia la X de pantalla `walk_to`; sin botones) y
`stage_done` termina la etapa (`msx_game_stage_end`). En la etapa 1 de la
Misión 2 el gancho de referencia `MSX_REFERENCE_STAGE_EXIT` ya no termina
la etapa: solo inserta el `0B6ED2` nativo (`neo_world_stage_exit`, tras la
cabeza de la lista en lugar de tras la puerta) en el punto donde antes
cortaba; caminata, rectángulo y fin son los del original. Las otras etapas
siguen con el gancho. Falta portar la puerta `03889C` (y `0C70FC`,
`0AE220`) para quitar el gancho del todo; sin ejecución en ares en esta
rama (otro ares ocupado).

## Colisión: rejillas desde la ROM

`0C72EC` copia las celdas de colisión del descriptor de mapa de la capa
(+2E): +4 puntero (P2 lineal, banco (A>>20)−1 por 2FFFF0), +8 pares de
columnas, +A bloques por par, 4 bytes por bloque 2×2. `tools/fill_collision_rom.py`
comprueba la correspondencia con **todas** las celdas observadas y rellena
solo las desconocidas:

```sh
python3 tools/fill_collision_rom.py filesystem/mission2/s2/collision.json --rom mslugx.zip --descriptor b702a --top 101 --allow-conflicts 64
python3 tools/fill_collision_rom.py filesystem/mission2/s3/collision.json --rom mslugx.zip --descriptor b740a --top 271 --rows 0:207
```

| Etapa | Descriptor | Iguales | Conflictos | Rellenadas | Desconocidas |
| --- | --- | --- | --- | --- | --- |
| 1 | `0B6BAA` | 2456 | 0 | 0 | 0 |
| 2 | `0B702A` | 13190 | 63 (columnas 336..339, filas 80..95: 0 observado, 15 en ROM; escritura sin identificar, se conserva lo observado) | 20440 | **0** |
| 3 | `0B740A` | 16718 (filas 0..207) | 0 | 7803 | 37037 |

`pack_mission.py` ya empaqueta las rejillas de las etapas 1 y 2. En la etapa
3, desde la fila 208 (la subida de la torre) las celdas observadas no son
filas de la ROM: las plataformas se escriben en marcha (sin `0C88B0`) y la
cámara queda fija en las columnas 39..79; las 37037 celdas desconocidas
están todas fuera de esas columnas. Hace falta identificar al escritor de
las plataformas de la subida.

## Soldados `0678B6..` y `0692B2..` (`neo_rebel3.c`)

Traza (estado `sec02_19932`, unos 100 s; `b1`, 3246 actualizaciones) y
extracción:

```sh
export MSX_FULL_LOAD=sec02_19932 MSX_FULL_LOAD_FRAME=19932 MSX_FULL_STATES=/home/user/captures-fullgame/states4
MSX_FULL_FAMILY_ROOTS=678b6,678c0,678ca,67902,692b2,692bc,692c6,692d0,692da,692e4 MSX_FULL_FAMILY_FROM=20500 \
MSX_FULL_VRAM_TRACE=1 MSX_FULL_FIRST_DUMP=9999999 MSX_FULL_GRID_TAP=0 MSX_FULL_LAST=26139 sh tools/run_capture_fullgame.sh /home/user/captures-m2en/b1
python3 tools/extract_family_fixture.py /home/user/captures-m2en/b1/boss-trace.txt --output data/rebel3_fixture.json.gz \
  --range 678b6-69700 --range 21cb6-21cee --attacks --ram 10000e-100010 --ram 10c7b6-10d7b6 --ram 1014ec-1014f4 --ram 1014fc-101500
```

Familia `rebel3` (las dos juntas: la `c` entra en la `b` por `067A40`):
30 nodos, **3246 actualizaciones**, 30 borrados, 47 contactos del jugador.
`extract_family_fixture.py` funde ahora también las lecturas por bytes de
los objetos dinámicos (`10C06E..10C291`): `014160` lee los bytes de los
cubos `10C26E/10C280` por separado y solo se guardaba el primero.

Traducción de `0678B6..069700`: variante +9C (paletas `068F98`, rangos
`2D26BC..2D29B4` por `0260C8`); espera `0678DC` hasta tener al jugador más
cercano (`01AF9A`, +86) dentro de +70/+78 (`068B4C`/`068B7E`), o aparece
junto a él (`067908`: `014A02` desde el bloque del jugador, que el banco lee
de 1014EC..1014FF; `neo_vehicle_env_t.player`); `06905A`; entrada `0679B0`
(sólido `E1B5C`) u oculta `067A6C` (destello `021CB6`, cuyo programa sube
+80 del creador con `022230`); giro `0679EE`; decisión `067A40` (`068BB8`,
`068CAE`); andar `067AB4`/`067B42` con el andador `08CCD4` (`068CD4`,
`068ABA`, `068B08`); giro `067BA6`; cuchillada `067E66` (programa `268DCC`,
llamada `06899A`: cuchillo `067FC0` con la fila de ataque de `E1A76` por
+60); muerte `067C20` (programa `269024`: objeto `068F26` por `08CDB2`,
`068F4A`, `069262`). Colas `069280` (+9A, `004C24`, `06914A`, chispa
`06918E`) y `0688D6` (recorte `E1A5E`). Familia `c` (`0692B2`, +9C = 0): cae
(`06932E`: `06951C` conserva la posición absoluta sobre el nuevo objeto de
suelo, +34 por `2D29B4`, programa `269530` con `069548`), aterriza (`069596`:
polvo `0695DC` tras el actor del guion), `0693CA` y va a `067A40`; muerte en
el aire `069448`/`069484..069500`.

Paradas explícitas (sin evidencia en la traza): `067EE4` (+6E y borde de
pantalla), piezas `067D06`/`069478` (+5A = 0x0A/0x0B, `068D32`), `067D80` y
las bombas `0687F6` de `0691D2`/`069262` (+7A), los cuchillos de +9C = 4/8
(`069696`, `0689D2`), `067F12`. No se reproduce la comprobación anticopia
antes de `004188` (10F00A/10F00C, 2FFFE0).

Sprites: raíces `268C0A..26985A` y `2309D2`, paletas 0x3FC, 0x3FD, 0xF2,
0xAC, 0x3FB (y 0xB0) en `filesystem/mission2/rest-native.json`.
**Píxeles** (captura `bv`, volcados cada 15 fotogramas 20910..21900 con la
traza; `export_rest_sprites.py ... bv@678b6-69700 bv@21cb6-21cee`): 30
volcados, 37947 iguales, 1056 distintos en 5 volcados (21075, 21120..21150,
21270: muertes y cuchillos superpuestos, sin explicar).

## Enemigos más frecuentes (no portados)

`0613CA..`, `0678B6..` y `0692B2..` ya son nativos (arriba). Los demás no son variante de una familia nativa:

| Constructor | Lectura |
| --- | --- |
| `03670E` (×8) | +96 = 4, paletas 0x142/0x143, programa `23BA74` |

Cada una necesita su traza de familia (las capturas desde `states4` cuestan
segundos) y una traducción completa.
