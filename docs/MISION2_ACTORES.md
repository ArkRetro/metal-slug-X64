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
| `0613CA/0613D8/0613E6` momias (variante en +96; antes «rebeldes»), aliento `061E8C` (fila `E1252`, tipo `0x10`: forma 3, MARCO_CAMPAIGN.md), destello `0620AA` | `src/neo_rebel2.c` | familia `rebel2` (etapa 2, `a2`): 41 nodos, **4815 actualizaciones**, 41 borrados, 4 contactos |
| `03882A/03889C` puerta de la etapa 1, ondas `03C62A` | `src/neo_gate.c` | familia `gate` (g1): 2 nodos, **348 actualizaciones**, 2 borrados, 2 contactos, parches del anillo iguales |
| `0678B6/0678C0/0678CA/067902` y `0692B2..0692E4` soldados (variante en +9C), cuchillo `067FC0`, destello `021CB6` | `src/neo_rebel3.c` | familia `rebel3` (etapa 2, `b1`): 30 nodos, **3246 actualizaciones**, 30 borrados, 47 contactos |
| `08C154/08C15E/08C168/08C172` caja de suministros (+88 = 3..0) | `src/neo_crate.c` | familia `crate` (etapa 3, `c1`): 2 nodos, **1235 actualizaciones**, 2 borrados |
| `06851A` obstáculo y su capa `0685FE`; señal `03A892` | `src/neo_m2props.c` | familia `bags` (etapa 2, `s2`): 7 nodos, **421 actualizaciones**, 7 borrados |
| `03A6EA` puerta de la etapa 2 (orden de mapa `0B7326`), capa `03A82E`, apertura `03A776` (`0B7350`: parche `0B706C`, salida `0B6ED2`), polvo `03A464` | `src/neo_m2props.c` | familia `m2gate` (etapa 2, `g2`): 5 nodos, **1811 actualizaciones**, 2 borrados, parche del anillo igual |
| `052A5C` espera de cámara; `0678A2/0678AC` soldado (+9C = 4) | `src/neo_m2props.c`, `src/neo_rebel3.c` | familia `stage3` (`c1`): 2 nodos, **1838 actualizaciones** |
| `03670E` dron y su bomba `0369AC` | `src/neo_drone.c` | familia `drone` (`c1`): 10 nodos, **1134 actualizaciones**, 10 borrados, 22 contactos |
| `06835C` soldado con rehén: rehén `021B46..` y soldado `0683FA/0684CA` | `src/neo_hostage.c`, `src/neo_rebel3.c` | familia `hostage` (etapa 2, `h1`): 2 nodos, **186 actualizaciones**, 2 borrados |
| `09A692` caja oculta → prisionero `099E1C` (`09A842`) | `src/neo_secret.c`, `src/neo_prisoner.c` | familia `secret2` (etapa 1, `pr1`): 8 nodos, **1477 actualizaciones**, 4 borrados, 246 dibujos |
| `0B829C/0B833A` carteles (etapas 2 y 3; `0B829C` sella colisión `0C88B0`) | `src/neo_m2props.c` | familia `signs` (`s2a`): 3 nodos, **3067 actualizaciones**, el sello en el anillo igual (el banco traduce `0C88B0` sobre el anillo +88) |
| `099DE2` prisionero atado (+86 bit 0, sin `01B1DC`) | `src/neo_prisoner.c` | familia `prisoner3` (`s2a`): 561 actualizaciones |
| `099DF0` prisioneros colgados y la cuerda `09A9C4` (`011FF6`: `ga_draw` del +7A del prisionero) | `src/neo_prisoner.c`, `src/neo_gactor.c` | familia `prisoner4` (`p3`, `MSX_FULL_FAMILY_ROOTS=99df0` desde 22900): 6 nodos, **2275 actualizaciones** |
| `021A1C/021A26` mineros (etapa 2), polvo `021F96`, destello `02206E`, regalo por `08CDB2` | `src/neo_hostage.c` | familias `miner` (`s2b`, jugador quieto junto al minero): 20 nodos, **1244 actualizaciones**, 18 borrados, 1225 dibujos; `minerrun` (`s2a`, el piloto pasa corriendo): 19 nodos, 1015 actualizaciones |
| `0995EC` tesoro oculto (etapa 3), explosión `01C7AE` | `src/neo_m2traps.c`, `src/neo_effects.c` | familia `treasure` (`gem`): 1 nodo, **446 actualizaciones** (golpes, abierto, 50000 puntos, explosión) |
| `032DE2` soldado envuelto como momia, destello `021CB6`, `068E94` → soldado `067AB4` | `src/neo_m2traps.c`, `src/neo_rebel3.c` | familia `wrapped` (`wrap2`): 2 nodos, **2641 actualizaciones**, el aliento de una momia lo despierta en 36426 |
| `03A05A` respiradero de gas y sus bocanadas `03A1AE` | `src/neo_m2traps.c` | familias `vent` (`vent`): 21 nodos, **1120 actualizaciones**, 20 borrados; `ventwait` (`rope2`): constructor y espera, 546 |
| `082BCE` soldado colgado de una cuerda, cuchillo `0830FE`, cabo `083212`, salpicadura `0682F0` | `src/neo_m2traps.c` | familias `rope` (`rope`): 8 nodos, **1627 actualizaciones**, 7 cuchillos; `ropedeath` (`rope3`): 12 nodos, **1351 actualizaciones**, 2 muertes con caída y cuerpo |

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
cortaba. Ya no: la puerta `03889C` es nativa (sección siguiente) y crea la
salida como el original; las otras etapas siguen con el gancho.

## Puerta `03889C` y ondas `03C62A` (`neo_gate.c`)

Traza (estado `sec01_19391`, unos 20 s; la puerta cae en 19740):

```sh
export MSX_FULL_LOAD=sec01_19391 MSX_FULL_LOAD_FRAME=19391 MSX_FULL_STATES=/home/user/captures-fullgame/states4
MSX_FULL_FAMILY_ROOTS=3882a,ae1dc,b6ed2,b6f6a MSX_FULL_FAMILY_FROM=19393 MSX_FULL_VRAM_TRACE=1 \
MSX_FULL_FIRST_DUMP=9999999 MSX_FULL_GRID_TAP=0 MSX_FULL_LAST=19935 sh tools/run_capture_fullgame.sh /home/user/captures-m2en3/g1
python3 tools/extract_family_fixture.py /home/user/captures-m2en3/g1/boss-trace.txt --output data/gate_fixture.json.gz \
  --range 3882a-38a26 --range 3c62a-3c962 --attacks --ram 10c2f6-10c2f8 --ram 10000e-100010 --ram 10c764-10c768 --ram 10c7b6-10d7b6
```

`03882A` (guion, x 558, +6E = 1): onda `03C52C`, capa 8800, paletas
0x3CA/0x3D8, suelo `0146E8`, vida `2D39C0` (`0260C8`), caja `038A26`,
programa `23C55A`, paleta de destello 0x3CB. `03889C`: `01436A`/`0143DC`,
`00427A`, objeto dinámico `038AD8` (`0140C0`); a la izquierda de x 0x148,
recorte `038A52` (`019504`) y `004C24` (+5F bit 7 → `038994`). `038994`:
sonido 1021, `013F1E`, escombros `038A62/74/86` (`01DA40`) y, con +6E = 1
(tabla `038A5A`), `038A0C`: pantalla blanca `0AE1DC` (`0AE220`, ya nativa),
fila `038A42` y `0B6E06` (parches `0B6E4A`/`0B6E74` por `0C70FC`, marcador
`0B6F40`, salida `0B6ED2`); `0389F2` ataca nueve veces y se borra. +6E = 0
(`0389E6`) es la misma cola sin `038A0C`; otros valores paran.

`0C70FC` (parte de colisión `0C72EC`): nueva llamada `map_patch` de
`neo_genv_t`. El banco escribe las celdas en el anillo +88 de la capa con el
recorte a la ventana +36/+38 (comprobado byte a byte contra la RAM); el
mundo las escribe en la rejilla de la etapa (`neo_collision_patch`, fila
superior 2·d5+1 en la rejilla de 32 filas). Las **104 celdas mutables** de
la rejilla de la etapa 1 son exactamente las de los dos parches (`0B6BEC`
7×7 en la columna 66, fila 21; `0B6C02` 7×4 en la fila 7) y las otras 204
celdas del rectángulo ya valían eso. Las teselas del parche (+0, y la capa
`10E83E`) no se dibujan aún.

`03C62A` (ondas en el agua, `03C520..03C532` con +6E = 0..3): `019240` da
la paleta por la misión (RAM 10C2F6: 0x14B en la Misión 2, 0x14C en la 5;
las misiones 1, 3 y 4 no tienen onda; la 6, `013D0A`, para). Busca agua
(clase 5, `013CF8` con A0 = el padre del creador: nuevo
`neo_kernel_point_class_in`) 2 px por encima del creador y la sigue
(`03C6EA`); programas `03C67E`/`03C7C6`/`03C7D6`/`03C84E`. El mundo pone
ahora `pickup.mission` (RAM 10C2F6) al empezar cada etapa de una misión
empaquetada, así que las granadas (`03C526`, `neo_bullet.c`) y los
prisioneros crean su onda en lugar de parar. Sin cambio en la Misión 1.

Sprites: programas `23C55A`, `23F746..23FA2C`; paletas 0x3CA, 0x3D8, 0x3CB y
0x14B en `filesystem/mission2/rest-native.json`. **Píxeles** (captura `gv`,
volcados cada 15 fotogramas 19500..19770): 18 volcados, **8591 iguales, 0
distintos**.

Juego: la puerta crea la salida y la etapa 1 de la Misión 2 termina ya sin
el gancho de referencia (`MSX_REFERENCE_STAGE_EXIT` solo actúa en las otras
etapas); `neo_world_stage_exit` desaparece.

## Puerta de la etapa 2 `03A6EA` (`neo_m2props.c`, rama `work-m2gate`)

Traza (unos 70 s; la puerta nace en 24436 y se abre en 26001):

```sh
export MSX_FULL_STATES=/home/user/captures-fullgame/states4 MSX_FULL_VRAM_TRACE=1 MSX_FULL_FIRST_DUMP=9999999 MSX_FULL_GRID_TAP=0
MSX_FULL_LOAD=sec02_19932 MSX_FULL_LOAD_FRAME=19932 MSX_FULL_FAMILY_ROOTS=3a6ea,3a892 \
  MSX_FULL_FAMILY_FROM=19933 MSX_FULL_LAST=26139 sh tools/run_capture_fullgame.sh /home/user/captures-m2gate/g2
python3 tools/extract_family_fixture.py .../g2/boss-trace.txt --output data/m2gate_fixture.json.gz \
  --range 3a464-3a8a0 --range b6ed2-b6f3f --attacks --ram 10c2f6-10c2f8 --ram 10000e-100010 --ram 10c764-10c768 \
  --ram 10c7b6-10d7b6 --ram 10c358-10c359 (más los --ram de los jugadores de `stageexit`)
```

La orden de mapa `2C` de la etapa 2 llama a `0B7326`: `0040BC` (reserva
0x1C) de `03A6EA` desde el actor de misión 101364 (+0C, +04: justo tras
él, profundidad 1, +68 = 0), +28 = 0xA70, +2C = 0x2C0. `03A6EA`: `01AB98`
con la raíz 10E968 y sus palabras +28/+2C, capa `03A82E` (`0040BC` reserva
0x1C, `01B220` (0, 0)), capa 0, paleta 0x445, +15 bit 0 a 0, programa
`23CDD0`, RAM 10C358 = 0; `03A75A`: con 10C358 ≠ 0 pasa a `03A776`
(siempre `01ABC8` y `00427A`). `03A776`: `0B7350` (parche `0B706C` por
`0C70FC`, D2..D5 = 8, 0xA, 0xA2, 0x2F; sonido 103B por `0120F8`; salida
`0B6ED2` en (0xAA0, 0x280) de la raíz con +6E = `0B7348` (−12, −1, 0x80,
0x32) y +72 = 0xAC0), polvo `03A464` (reserva 0x2A, `01B220` (−8, −64)),
+6E = +2C, +30 = 0, +34 = 4,0; `03A7BA`: `01AC38`, `00427A` hasta que +2C
sube 0x50; `03A7DE`/`03A7E8`: +70 (byte) = 0, palabra +70 + 1, con < 0x19
pasa a `03A768`; una actualización con +2C + seno `0029FC`[(+70 << 7) &
0xFF] · 0x100 para `01ABC8`/`00427A` y +2C de vuelta; `03A768` para
siempre. `03A82E`: capa 0, paleta 0x44B, +15 bit 2 a 1 y bit 0 a 0,
programa `23CDDC`; `03A86A`: `01ABC8`, `00427A`. `03A464`: paleta 0x204,
programa `22D372`; `03A488`: `01436A`, `00427A` y `004188` al acabar el
programa (comprobación anticopia no reproducida).

Prueba: familia `m2gate` (1811 actualizaciones 24436..26137, igual a MAME:
el parche en el anillo +88 de 10C72E, 10C358 leído y escrito, la salida
`0B6F24` hasta el final de la captura). En el juego `camera_step`
(`gameplay.c`) crea la puerta con el evento `0B7326`. Datos:
`region_rom.json` (`0B706C`, celdas `42C0FA`, rectángulo `0B7348`),
programas `23CDD0/23CDDC/22D372` en `soldier_programs.json`.

Sprites: paletas 0x445 y 0x44B y los marcos de los tres programas en
`filesystem/mission2/rest-native.json` (`export_rest_sprites.py ...
gv@3a464-3a8a0 --mask-auto`, captura `gv` con volcados cada 15 fotogramas
25995..26130). **Píxeles** por nodo: puerta 10 volcados, 42072 iguales, **0
distintos**; polvo 5 volcados, 1977 iguales, 0 distintos. La capa
`03A82E` usa la autoanimación del LSPC (atributo bits 2/3: el renderizador
no la implementa, se dibuja el marco del descriptor); sin comparación.

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
| 2 | `0B702A` | 13190 | 63 (columnas 336..339, filas 80..95: 0 observado, 15 en ROM): son el parche `0B706C` de `0B7350` visto después; `--patch b706c:a2:2f` les da el valor de la ROM | 20440 | **0** |
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

## Caja, obstáculo, dron (rama `work-m2en4`)

Trazas (unos 40 s y 60 s):

```sh
export MSX_FULL_STATES=/home/user/captures-fullgame/states4 MSX_FULL_VRAM_TRACE=1 MSX_FULL_FIRST_DUMP=9999999 MSX_FULL_GRID_TAP=0
MSX_FULL_LOAD=sec03_26140 MSX_FULL_LOAD_FRAME=26140 MSX_FULL_FAMILY_ROOTS=8c154,8c168,52a5c,3670e,26254,678a2 \
  MSX_FULL_FAMILY_FROM=26141 MSX_FULL_LAST=29800 sh tools/run_capture_fullgame.sh /home/user/captures-m2en4/c1
MSX_FULL_LOAD=sec02_19932 MSX_FULL_LOAD_FRAME=19932 MSX_FULL_FAMILY_ROOTS=6851a,6835c,3a892 \
  MSX_FULL_FAMILY_FROM=19933 MSX_FULL_LAST=26139 sh tools/run_capture_fullgame.sh /home/user/captures-m2en4/s2
python3 tools/extract_family_fixture.py .../c1/boss-trace.txt --output data/crate_fixture.json.gz --range 8c154-8c374 --attacks --ram 10000e-100010 --ram 10c7b6-10d7b6
python3 tools/extract_family_fixture.py .../c1/boss-trace.txt --output data/drone_fixture.json.gz --range 3670e-36a64 (mismos --ram)
python3 tools/extract_family_fixture.py .../c1/boss-trace.txt --output data/stage3_fixture.json.gz --range 52a5c-52a88 --range 678a2-69700 --range 21cb6-21cee (más --ram 1014ec-1014f4 --ram 1014fc-101500)
python3 tools/extract_family_fixture.py .../s2/boss-trace.txt --output data/bags_fixture.json.gz --range 6851a-687aa --range 3a892-3a89e (mismos --ram)
```

La reproducción desde `sec03_26140` sí crea el dron (cinco, 26263..26900):
no hizo falta conducir al jugador.

`08C17C` (caja): programa `E57FA[+88]`, paleta `E55B6[+88]` (0xCF para
+88 impar, 0x3AB par), +4E = 10, capa 8820, caja de golpe `E5634`, suelo
`0146E8` si +88 < 2, objeto dinámico `E5640`. `08C1F2`: `004C24`, +5A = 0,
`01436A`+`01AD8C` (+88 ≥ 2) o el andador `08CCD4`, `00427A`, `0140C0` y
`004188` fuera del rectángulo `E562C` (`0C8BEC`). Rota (+5F bit 7):
`013F1E`, programa `E580A[+88]`, `019DFC` para +88 impar; +88 0/1 sigue en
`08C2B2` (andador) y +88 2/3 en `08C356` (`01AD8C`, `01436A`); al acabar el
programa, `004188` (comprobación anticopia de `08C2C0` no traducida). La
llamada de programa `2869A4` crea el objeto `08C374` (nativo) tras el actor
del guion con +6E/+70/+72 de la caja. **Sin evidencia**: +88 = 3 (`08C154`,
rama `08C356`) y +88 0/2: en `c1` solo se abren dos `08C168`.

`06851A`: +15 bit 0 = (+6E ≠ 0), programa `26939A`/`2693A6`, capa E06C,
paletas 0xF5 y 0xF6 (destello), caja `E1AA2`, vida `2D296A`, suelo, capa
`0685FE` (`26938E`/`269382`, `0044F6`, se borra cuando el creador pone
+60). `0685C8`: `01436A`, `00427A`, `004C24`, objeto dinámico `E1B6A`,
recorte `0688D6`. Roto: +60 = FF, `013F1E`, restos `E1BF2`/`E1C04`,
explosión `01C71E` 32 px arriba, sonido 1035; `06875A`: programa `2693B2`,
`019DFC`, fila `E1B2E`; `06878E` hasta el final del programa. `03A892`:
RAM 10C358 = 1 (nuevo `neo_ga_flags_t.signal358`, lo lee `03A75A`) y se
borra; el banco compara 10C358 en la familia `m2gate`.

`052A5C`: +6E = progreso de cámara 10E99E + 1 si +6E = 0; `052A74` se
borra cuando el progreso alcanza +6E. `0678A2/0678AC`: la variante +9C = 4
de `neo_rebel3.c` (paletas 0x17E/0x17F/0x187/0x180); en `c1` solo espera,
anda y gira (35 actualizaciones).

`03670E` (dron, +96 = 4): paletas 0x142/0x143, programa `23BA74`, capa
E0ED, vida `2D0C7C`, caja `035A84`, velocidad `2D0CAE`; gira 0x30 px más
allá del jugador más cercano (`01AF9A`), sube o baja a 0x100 + +6E
(0x18000) y, encima del jugador (|dx| ≤ 4, +74 = 0), arma +74 =
`2D0CF8`/2 o suelta la bomba (`03689C`, `0366DA`) si +78 y +77; muerte
`0368DA` (programa `23BBD0`, sonido 10F0); recorte `035A54` con `0041AC`.
Bomba `0369AC`: `0143DC`, explota (`036A0C`: paleta 0x4BB, programa
`23BCF2`, fila `035AB8` mientras +61) en el suelo o si la fila `035AC8`
toca. La tabla `2D0B9E` de `region_rom.json` se alarga hasta `2D0D40`
(los bloques deben ser contiguos: `ga_rom` no salta de bloque).

Sprites: programas `286896/286904/2868A2/286910` (caja), `26939A..2693B2`,
`269382/26938E` (obstáculo), `23BA74..23BCF2` (dron y bomba); paletas 0xCF,
0xF5, 0xF6, 0x142, 0x143, 0x4BB en `filesystem/mission2/rest-native.json`
(0x3AB y 0x17E..0x187 no aparecen en los volcados). **Píxeles** (capturas
`cv`, `sec03_26140`, volcados cada 15 fotogramas 26265..29750, y `sv`,
`sec02_19932`, 22425..25000; `export_rest_sprites.py ... cv@8c154-8c374
cv@3670e-36a64 cv@678a2-69700 sv@6851a-687aa`): caja 168 volcados, 106690
iguales, **0 distintos**; dron 50 volcados, 64761 iguales, 402 distintos en
3 volcados (26490, 26940, 26970); obstáculo 36 volcados, 119512 iguales,
1342 distintos en un volcado (22500, la rotura); el soldado `0678A2` no
sale en ningún volcado. Diferencias sin explicar.

## Soldado con rehén `06835C` (rama `work-m2en5`)

Traza y extracción (estado `sec02_19932`, unos 30 s; el soldado aparece en
20605 y el rehén se va en 20905):

```sh
export MSX_FULL_STATES=/home/user/captures-fullgame/states4 MSX_FULL_LOAD=sec02_19932 MSX_FULL_LOAD_FRAME=19932
MSX_FULL_FAMILY_ROOTS=6835c MSX_FULL_FAMILY_FROM=20590 MSX_FULL_VRAM_TRACE=1 MSX_FULL_FIRST_DUMP=20610 \
  MSX_FULL_PERIOD=15 MSX_FULL_GRID_TAP=0 MSX_FULL_LAST=20960 sh tools/run_capture_fullgame.sh /home/user/captures-m2en5/h1
python3 tools/extract_family_fixture.py .../h1/boss-trace.txt --output data/hostage_fixture.json.gz --range 678a2-69700 \
  --range 21b46-22258 --attacks --ram 10000e-100010 --ram 10c7b6-10d7b6 --ram 1014ec-1014f4 --ram 1014fc-101500
```

`06835C` (guion): +8A = `2D27E2` (`0260C8`) << 8 sin signo; crea el
soldado `0683FA` (`0040BC`) con +69, +8A, +86 = el propio nodo, +6E,
+70 = +72, +74, +76, +7A = +78, 0x20 px por delante (`01ACF2`); después
+72 = +70, +74 = FFFF, +62 = +8A, +7E = +69, `019DFC` y el nodo pasa a
rehén (`021B46`): capa 8820, paleta 0x1F, caja `E005E`, suelo, orientación
por +6E, programa `230984`, +81 = 2, +4E = 1, +80 = 0. `021BB2`: `01456A`,
`00427A`; con +80 = 1 (lo pone la muerte del soldado, llamada `068F4A`)
va a `021E9A` (programa `230654`) y `021ECC` (programa `23066A`, gira,
+62 = +72 << 8) y `021F02` anda (`02223C`) hasta que `0220BC` lo recorta
(`01945C` con `E004E`). `0683FA` (`neo_rebel3.c`, +9C = 0, +81 = FF):
programa `268D0C`, +94 = 0x38 − azar(0x18), +4E por `2D271E`, +62 = +8A por
+6E; `0684CA` espera (`01456A`, `00427A`) hasta `068CD4` (→ `067EE4`, no
observado) o `068C3A` (se alejó +70 de +8E, o el jugador más cercano está
delante a +94 o menos: → `067E66`) y sigue en la familia `0678..`.
`068F66` compara el creador consigo mismo y nunca escribe. Paradas
explícitas: `021C0A` (+5A = 0x10: el rehén falso que pasa a soldado por
`021CB6`, `021C70`, `068E94`; sin evidencia) y `021F22` (+5F bit 0).
`021AD0..021E78`, `022162..02222E` (rehenes `021A1C/021A26`) no se usan.

Prueba: familia `hostage` (`--node-fields 86`: +86 guarda el nodo del
rehén o el jugador): 2 nodos, **186 actualizaciones**, 2 borrados, igual a
MAME. Datos: programas `230984/230654/23066A` en
`data/soldier_programs.json`, `E004E..E0075` en `region_rom.json`.

Sprites: paleta 0x1F y programas del rehén en
`filesystem/mission2/rest-native.json`. **Píxeles** (la misma captura `h1`,
volcados cada 15 fotogramas 20610..20955): soldado 13 volcados, 9801
iguales, **0 distintos**; rehén 19 volcados, 11089 iguales, 829 distintos
en 4 volcados (20625..20670, mientras el soldado lo sujeta a su lado; sin
explicar).

## Jefe de la etapa 3 `026254`

Portado: [JEFE_MISION2.md](JEFE_MISION2.md) (`src/neo_m2boss.c`, familias
`m2boss1..m2boss8`). No nace en `c1`: es el jefe de la etapa 3. En `run4`
el registro `2D9CA2` corre entre 92475 y 93083 y el nodo pasa a `026354` en
93083 (x 480, y 1705). Se llega reproduciendo `run4` desde `sec03_26140`
(unos 19 min) con `MSX_FULL_SAVE_AT=92700` (solo captura: guarda
`at_92700`); los tramos de la traza salen de ese estado.

## Prisionero de las cajas ocultas `099E1C` (`neo_secret.c`)

Traza (estado `sec01_19391`, unos 50 s; tres cajas `09A692` en la cámara 320
de la etapa 1, salud 100/200/300, `+78` = 1):

```sh
export MSX_FULL_LOAD=sec01_19391 MSX_FULL_LOAD_FRAME=19391 MSX_FULL_STATES=/home/user/captures-fullgame/states4
MSX_FULL_FAMILY_ROOTS=9a692,99e1c,8c374 MSX_FULL_FAMILY_FROM=19393 MSX_FULL_VRAM_TRACE=1 \
MSX_FULL_FIRST_DUMP=9999999 MSX_FULL_GRID_TAP=0 MSX_FULL_LAST=22600 sh tools/run_capture_fullgame.sh /home/user/captures-m2g/pr1
python3 tools/extract_family_fixture.py /home/user/captures-m2g/pr1/boss-trace.txt --output data/secret2_fixture.json.gz \
  --range 99dcc-9a5ee --range 9a5ee-9a9c2 --range 3c62a-3c962 --attacks --ram 10c2f6-10c2f8 --ram 10000e-100010 --ram 10c7b6-10d7b6
```

`09A842` (caja con `+7E` = 0) crea `099E1C` tras sí misma (`0040BC`, reserva
0x1C; sin nodo libre escribe en el nodo falso y no pasa nada), `01B220` en
(0, 0) y suma `+84/+86` a `+28/+2C`; copia `+74/+76` (regalo) y pone su
`+6E` = `+78` de la caja. `099E1C` es otra entrada del constructor de
`099DCC..` (`01B1DC`, `09AC16`) que salta directamente a `099FD8`, el
prisionero ya suelto que pasea (`neo_prisoner.c`, ya nativo). Sprites: los
programas `29FE52..` y las paletas 0x381/0x382/0x3FA ya estaban en
`filesystem/mission2/rest-native.json` (56 marcos, 0 ausentes). Sin
comprobación de píxeles propia (son los marcos de la Misión 1).
## Armas que da la Misión 2 (rama `work-m2mummy`)

Del guion (`data/mission2_script.json`, palabra de tipo de objeto de
`08C37C/08C168` y de los prisioneros `099DF0/099E06`): tipo `04` → arma
`06` (sección 1, prisionero `2D737C`), `50` → `07`, `52` → `09` (dos), `4A`
→ `0D`, `01` → `02`, `03` → `04` (tres) y `00` (H) / `02` (R). Nativas:
`02`, `04`, `07`, `09`, `0D` (proyectiles en `src/neo_pweapon.c`, poses de
disparo y sonidos: ARMAS_VEHICULOS.md, «Paso 14»), H y R. El rayo `06`
sigue siendo una parada explícita (dispara la bala de pistola del port).
La momia (forma 3) tiene ya sus poses (MARCO_CAMPAIGN.md, «Forma 3»).

## Mineros `021A1C/021A26` (rama `work-m2events`)

Informe del usuario (hardware real): faltan los mineros de la Misión 2. Son
registros **no contados** del guion de la sección 2 (`2D7568..2D7826`:
`021A26` en las elecciones al azar de x 184 y 384, `021A1C` en x 320 y en el
borde derecho de la pantalla), que el port dejaba caer (`audit_script_events`).
Pertenecen a la misma familia de ROM que el rehén `021B46` (`021A1C..022258`,
paleta 0x1F) y se traducen en `neo_hostage.c`:

- `021A1C` (+81 = 0) / `021A26` (+81 = 1, las palabras +70/+72/+74 pasan a
  +72/+74/+76). `021A3E`: `009F88`, capa 8820, paleta 0x1F, +61 = 0, mira
  según +6E, suelo `0146E8`, una de las tres excavaciones
  `23058A/2305CC/230608` por `0029C6` & 3.
- `021AE6` (excavar; `021AD0` vuelve a `23058A`): `01436A`, `01AD8C`,
  `00427A`. Con +81 = 1, un jugador dentro de `E0056` (−0x20, −0x10, 0x40,
  0x30; `022178` → `01AFFC`) lo asusta (`021E24`); con +81 = 0, el progreso de
  cámara 10E99E ≥ +70 (con signo, +70 ≠ 0) lo hace huir (`021F4E`). La
  llamada `022162` del programa (`0029C6` & 3 = 0) pasa a la pausa `021D98`
  (`2308CE`, las mismas pruebas antes de `00427A`; al acabar, `021AD0` o la
  huida si +61).
- `021E24` (`2306EE`) y `021E5C`: +61 = FF, `23072C`, cuya llamada `0221C2`
  regala el objeto +74 (vida +76) por `08CDB2` 0x18 px por delante (+12/+13
  = 4, +34 = 0x90000) y crea el destello `02206E` allí; después la pausa.
- `021F4E`: `2306AC`, +62 = +72 << 8 (mira a la derecha si es positiva),
  `021F84` anda con `01456A`.
- `022196` (llamada de las excavaciones): polvo `021F96` (capa 8820,
  paleta 0x204, `22E504`) 0x18 px por delante. `02206E`: capa 1001, paleta
  0xAD, `22DF54`; **sigue sin `rts` hasta `02209E`** (su primer paso es en la
  misma actualización). Los dos siguen a la raíz (`01ABC8`, `01AD8C`) y se
  borran al acabar el programa. Cola de todos: `0220BC` (`01945C` con
  `E004E`).

Trazas (MAME 0.264, `sec02_19932`):

```sh
export MSX_FULL_STATES=/home/user/captures-fullgame/states4 MSX_FULL_LOAD=sec02_19932 MSX_FULL_LOAD_FRAME=19932 MSX_FULL_FAMILY_FROM=19933 MSX_FULL_VRAM_TRACE=1 MSX_FULL_GRID_TAP=0
MSX_FULL_FAMILY_ROOTS=35b66,35c68,35d12,21a1c,21a26,b829c,b833a,99de2,529c0 MSX_FULL_FIRST_DUMP=19950 MSX_FULL_PERIOD=15 MSX_FULL_LAST=23300 sh tools/run_capture_fullgame.sh /home/user/captures-m2ev/s2a
MSX_FULL_FAMILY_ROOTS=21a1c,21a26 MSX_FULL_FIRST_DUMP=20150 MSX_FULL_PERIOD=15 MSX_FULL_IDLE=20158-20700 MSX_FULL_LAST=20750 sh tools/run_capture_fullgame.sh /home/user/captures-m2ev/s2b
python3 tools/extract_family_fixture.py .../s2b/boss-trace.txt --output data/miner_fixture.json.gz --range 21a1c-22260 --attacks --ram 10000e-100010 --ram 10c7b6-10d7b6 --ram 1014ec-1014f4 --ram 1014fc-101500
# minerrun: lo mismo sobre s2a
```

En `s2b` el jugador se queda quieto (x ≈ 170) junto al minero `021A26` de
x 184: susto, regalo (el objeto de su palabra +74), destello, pausa
y huida; en `s2a` el piloto pasa corriendo (excavar, huir, polvo). Familias
`miner` y `minerrun` iguales en bucle cerrado. Programas en
`data/soldier_programs.json` (`extract_soldier_programs.py`, raíces de los
mineros; el banco es el mismo de `marco-mz2`).

Sprites: `export_rest_sprites.py /home/user/roms/mslugx.zip
/home/user/captures-m2ev/s2b@21a1c-22260 --mask-auto --output
filesystem/mission2/rest-native.json --keep <export anterior>` (raíces y
paleta 0xAD añadidas; 0x1F y 0x204 ya estaban): **40 volcados, 55175
píxeles iguales, 0 distintos**. `pack_mission.py 2 ...` vuelve a empaquetar
`romfs/mission2.pack` (3,19 MB, 980 actores).

### Carteles, prisioneros `099DE2`/`099DF0` (misma rama)

Sprites (`export_rest_sprites.py`, con el exportador tomando también los
descriptores que `29FB62` escribe en +7A): carteles `s2a@b829c-b8444`, 154
volcados, 590018 píxeles iguales y 817 distintos en un solo volcado tomado
en el fotograma de la actualización (21480; los demás 0); prisioneros
colgados `p3@99dcc-9aa9e`, 78 volcados, 105991 iguales, **0 distintos**. La
paleta 0x4B8 (carteles de la etapa 3, +70 = 0) no está en ningún volcado
(parada de paleta: se dibujan con la de reserva).

**Murciélagos** `035B66/035C68` (`neo_drone.c`, el mismo módulo de ROM
`035A40..0366F0` que el dron): constructor común `0362EE` (paleta 0x94, o
0xB7 y +97 = 1 para el murciélago de puntos con +6E ≠ 0: las bandadas de
la etapa 3 (registro `2D923C`, cámara 2128) lo usan, pero no se ejecutaron
en `run4`/`run4-debug`; sin captura es parada explícita, `0364D2`, que el
recorrido de ares registra una vez), la bandada `035B66` (`03635E`: +70
murciélagos `035BF8` con +86 distintos de 0..4, velocidad inicial
`035ADC[+86]` y +30 al azar), el murciélago con recorrido `035C68` (posado
`23B838` con estiramientos `23B88E/23B912`, despega por +92 o por el
progreso de cámara +90, vuela a uno de sus tres puntos a 1,0 px con
`002EB4` ± 0x18 y `002E7C`, se vuelve a posar a 3 px; +60: susto
`036254`), la muerte `036068` (sonido 10F0, quemado con +9F = 7), la caída
`03610E` y el gateo `03618A`/`03663E` hasta 0x50 actualizaciones. Familia
`bats` (`s2a`, `--range 35a40-366f0`): **25 nodos, 2181 actualizaciones
iguales**, 25 borrados. Sprites (`s2a@35a40-366f0`): 75 volcados, 18611
píxeles iguales y 538 distintos en 4 volcados (uno tomado en el fotograma
de la actualización; 20490, 20820 y 20865 con explosiones encima, sin
explicar). Paradas explícitas: +96 = 1/3 (el nido `035D12` de la etapa 3,
`035EEC`, `035C76/035D04`, `035E0A`, `0361EC`), sin traza todavía.

## Premios de la Misión 2 (B27, rama `work-m2fix2`)

Informe de MiSTer (ROM 22): faltan premios en la Misión 2. Inventario de
cada objeto (`08C374` soltado / `08C37C` del guion; tipo `+6E`, registro
`0E581A + 0x18·tipo`: `+10` recogida, `+14` movimiento) en una captura
completa de MAME 0.264 de las tres etapas y el jefe (`capture_fullgame.lua`
con el piloto de la captura, invulnerable; traza de familia de los objetos y
de sus etiquetas):

```sh
export MSX_FULL_STATES=/home/user/captures-fullgame/states4 MSX_FULL_FAMILY_ROOTS=8c374,8c37c,32de2,32fb4,36a64 MSX_FULL_FAMILY_RANGE=8c374-8d900 MSX_FULL_FIRST_DUMP=9999999 MSX_FULL_GRID_TAP=0
MSX_FULL_LOAD=sec01_19391 MSX_FULL_LOAD_FRAME=19391 MSX_FULL_FAMILY_FROM=19392 MSX_FULL_LAST=19932  sh tools/run_capture_fullgame.sh CAP/inv1
MSX_FULL_LOAD=sec02_19932 MSX_FULL_LOAD_FRAME=19932 MSX_FULL_FAMILY_FROM=19933 MSX_FULL_LAST=26140  sh tools/run_capture_fullgame.sh CAP/inv2
MSX_FULL_LOAD=sec03_26140 MSX_FULL_LOAD_FRAME=26140 MSX_FULL_FAMILY_FROM=26141 MSX_FULL_LAST=118000 sh tools/run_capture_fullgame.sh CAP/inv3   # 26 min, hasta la Misión 3
```

Objetos que el original creó (110; «cogidos» por el piloto, que no busca
premios) y su estado en el port:

| Tipo | Movimiento | Recogida | Etapa 1 (creados/cogidos) | Etapa 2 | Etapa 3 + jefe | Port antes | Port ahora |
| --- | --- | --- | --- | --- | --- | --- | --- |
| `00` | `08B9B2` | arma 5 | 0/0 | 0/0 | 9/5 | nativo | nativo |
| `01` | `08B9B2` | arma 2 | 0/0 | 0/0 | 2/2 | nativo | nativo |
| `02` | `08B9B2` | arma 3 | 0/0 | 0/0 | 4/4 | nativo | nativo |
| `03` | `08B9B2` | arma 4 | 0/0 | 0/0 | 2/2 | nativo | nativo |
| `04` | `08B9B2` | arma 6 | 1/0 | 0/0 | 0/0 | nativo | nativo |
| `06` | `08B9B2` | bombas | 0/0 | 0/0 | 2/1 | nativo | nativo |
| `09` | `08B9B2` | 08B5FA | 0/0 | 1/1 | 0/0 | parada en la recogida | parada en la recogida `08B5FA` |
| `11` | `08B9B2` | puntos/comida | 0/0 | 0/0 | 1/0 | nativo | nativo |
| `18` | `08B9B2` | puntos/comida | 0/0 | 0/0 | 1/0 | nativo | nativo |
| `1C` | `08B9B2` | puntos/comida | 0/0 | 0/0 | 1/0 | nativo | nativo |
| `1E` | `08B9B2` | puntos/comida | 0/0 | 0/0 | 3/0 | nativo | nativo |
| `22` | `08B9B2` | puntos/comida | 1/1 | 0/0 | 0/0 | nativo | nativo |
| `24` | `08BA38` | puntos/comida | 2/2 | 0/0 | 0/0 | nativo | nativo |
| `29` | `08B9B2` | puntos/comida | 0/0 | 3/2 | 8/6 | nativo | nativo |
| `2A` | `08B9B2` | puntos/comida | 0/0 | 4/2 | 5/3 | nativo | nativo |
| `2B` | `08B9B2` | puntos/comida | 2/0 | 3/2 | 1/1 | nativo | nativo |
| `2C` | `08B9B2` | puntos/comida | 0/0 | 1/1 | 0/0 | nativo | nativo |
| `2F` | `08B9B2` | puntos/comida | 0/0 | 1/1 | 4/2 | nativo | nativo |
| `32` | `08B9B2` | medicina | 0/0 | 4/3 | 5/3 | nativo | nativo |
| `43` | `08BAB2` | puntos/comida | 0/0 | 0/0 | 1/0 | nativo | nativo |
| `48` | `08B9B2` | puntos/comida | 0/0 | 0/0 | 1/1 | nativo | nativo |
| `4A` | `08B9B2` | arma D | 0/0 | 1/0 | 0/0 | nativo | nativo |
| `50` | `08B9B2` | arma 7 | 0/0 | 1/0 | 0/0 | nativo | nativo |
| `52` | `08B9B2` | arma 9 | 0/0 | 1/1 | 0/0 | nativo | nativo |
| `54` | `08B9B2` | puntos/comida | 0/0 | 1/1 | 3/1 | nativo | nativo |
| `56` | `08BD32` | puntos/comida | 0/0 | 0/0 | 30/14 | externo (no se crea) | nativo (familia `coin`) |

Fuentes de premios del guion de la Misión 2 (`data/mission2_script.json`,
palabra del tipo de cada constructor; los registros al azar cuentan todos)
y los tipos que el port no traduce:

| Fuente (constructor) | Tipos de objeto en sus registros (veces) | Sin traducir |
| --- | --- | --- |
| `021A26` | `11`×4 `12`×2 `29`×1 `2A`×1 `2B`×1 | — |
| `032DE2` | `2A`×1 `2D`×1 | — |
| `0613CA` | `2D`×1 `32`×1 `40`×1 `54`×7 | `40` (movimiento `08BAAA`) |
| `0613D8` | `28`×2 `32`×2 `54`×9 | — |
| `0613E6` | `1E`×1 `28`×3 `40`×6 `54`×20 | `40` (movimiento `08BAAA`) |
| `0678CA` | `06`×1 `29`×2 `2A`×2 `2B`×1 `32`×1 | — |
| `067902` | `2A`×1 `2B`×1 | — |
| `0692B2` | `06`×1 `29`×3 `2A`×2 `2B`×2 | — |
| `08C154` | `2B`×1 `2D`×1 `2F`×1 `31`×1 `48`×1 | — |
| `08C168` | `00`×3 `01`×1 `03`×3 `18`×1 `1C`×1 `29`×2 `2A`×1 `2F`×2 `32`×3 | — |
| `08C37C` | `00`×1 `02`×1 `1C`×1 `22`×1 `23`×1 `29`×2 `2A`×2 `2B`×2 `2F`×5 `32`×2 `48`×2 `50`×1 `52`×1 `54`×1 `56`×47 | — |
| `099DE2` | `11`×1 `2F`×1 `35`×1 `36`×1 | — |
| `099DF0` | `09`×1 `32`×1 `52`×1 | `09` (recogida `08B5FA`) |
| `099E06` | `04`×1 `32`×2 `4A`×1 | — |
| `09A67E` | `14`×1 `16`×1 `17`×1 `26`×1 `27`×1 `29`×7 `2A`×11 `2B`×9 `2C`×4 `2D`×4 `2E`×3 `31`×2 `3F`×1 `55`×1 `59`×2 | `3F` (movimiento `08BB44`), `59` (movimiento `08BEA2`) |

**Causa.** El constructor de `neo_item.c` devolvía al mundo como externo
(contado, no creado) todo objeto cuyo movimiento no estaba traducido. En la
Misión 2 eso era el tipo `56` (`08BD32`): las monedas que el guion pone en
la etapa 3 (47 registros `08C37C`, `2D8EEC..2D9B60`; el piloto vio 30 y
cogió 14) no aparecían. Y el tipo `28` (`08BD5A`), que las momias
`0613D8/0613E6` sueltan (su `+90`), solo existía como conversión del tipo
`37` de la Misión 1.

**Arreglo.** `08BD32` (01ABC8 en su sitio y el crecimiento de `08B9D8`) y la
construcción directa del tipo `28` (mismo registro y movimiento `08BD5A` que
ya probaba la familia `food`). `neo_pickup_env_t` cuenta por tipo los
objetos creados, los externos y los cogidos, y la ROM escribe `MSX item kind
NN created=.. external=.. taken=..` cuando cambian (comparación con MAME en
ares).

**Prueba:** familia `coin` (ventana 55420..61300 de `inv3`, recortada de la
traza): seis monedas, su recogida con combo (`0E5556`) y sus etiquetas
`08D330`, más el objeto `2F` de `2D8FEA` en el suelo: **13 nodos, 15523
actualizaciones en bucle cerrado, 2171540 bytes iguales, 6 recogidas**. El
banco de familias cuenta ahora el contador de recogidas `10C590` por
actualización del juego (`08BFD6`), no por nodo: con varios objetos vivos el
combo de dos monedas cogidas a 4 actualizaciones salía como dos recogidas
sueltas. Píxeles (captura `coinv`, `sec03_26140`, volcados cada 15
fotogramas 55430..56190, `export_rest_sprites.py ... coinv@8c374-8d900`):
**51 volcados, 28193 píxeles iguales, 0 distintos**.

**Ares** (`mslugx-m2-reference.z64`, `test_mission2_ares.py --stage-seconds
800`: PASS; últimas líneas `MSX item kind` creados/externos/cogidos): `00`
7/0/3, `01` 2/0/1, `03` 2/0/1, `04` 1/0/0, `06` 1/0/0, `18` 1/0/0, `1C`
2/0/2, `22` 1/0/1, `24` 3/0/2, `29` 7/0/4, `2A` 10/0/3, `2B` 5/0/3, `2C`
1/0/0, `2F` 1/0/0, `32` 7/0/6, `35` 1/0/0, `36` 3/0/0, `40` **0/2/0**
(parada explícita, abajo), `44` 1/0/0, `46` 1/0/0, `48` 1/0/0, `4A` 1/0/0,
`50` 1/0/1, `52` 1/0/1, `54` 6/0/4. El piloto del port no pasa por los
registros de las monedas (ramas del guion de la etapa 3 distintas de las de
MAME): el tipo `56` lo prueba la familia `coin`.

**Paradas explícitas que quedan** (sin captura: el piloto no los produce;
el objeto se devuelve como externo y la línea `MSX item kind` lo cuenta):

- `40` (`08BAAA`, el andador `08CCD4`, de las momias `0613CA/0613E6`): su
  programa `289082` y los de `41/42` llaman a `289528..2896B2`, que sueltan
  objetos `3D/3F` (`08CD58`); `3D` (`08BB3C`, programa `28992A`) crea el
  actor `032FB4` (`289A80`), sin portar.
- `3F` (`08BB44`, caja oculta `2D7604` de la etapa 2, elección al azar) y
  `59` (`08BEA2`: crea `036A64`, no es un premio; cajas `2D92EE/2D9308`).
- `09` (prisionero colgado `2D8072`, etapa 2): la recogida `08B5FA` da el
  tipo de bomba 2 (`0C0A7C`, `10C722+p`) y la etiqueta `08CD10`; el port no
  tiene bombas de otro tipo (sus lanzamientos y el HUD): parada de la
  recogida (`ga_fail 08B5FA`, `MSX item pickup stopped explicitly`).
- `032DE2` (etapa 3, `2D8C68/2D8C76`, regalo `2D`/`2A` en `+74`): **nativo
  desde `work-m2fix4`** (B31, familia `wrapped`): el aliento de una momia lo
  convierte en el soldado `067AB4`, que suelta su regalo al morir.

## Etapa 3: actores de las fotos de la ROM 23 (B31, rama `work-m2fix4`)

Informe de MiSTer (ROM 23): faltan actores en la etapa 3; seis fotos de las
posiciones. Cada foto se situó por el fondo contra las capturas de `run4`
(`game<f>.png` cada 30 fotogramas, progreso de cámara 10E99E y `cam_x/cam_y`
de `trace.csv`), y los registros del guion de la etapa 3 de esa zona
(`data/mission2_script.json`) contra los nodos de `actors.csv`:

| Foto | Zona (progreso, cámara) | Lo que el arcade crea allí | Estado |
| --- | --- | --- | --- |
| `6af4` (tres carteles, panel del ojo) | 336–377, y 16 | tesoro oculto `0995EC` (2D89EE, cámara 272, (624, 88)): se rompe a tiros y da 50000 puntos al tocarlo | nativo |
| `9feb` (pilar con cartel) | 784–944, y 336 | soldado envuelto `032DE2` (2D8C68/2D8C76, (280, 424)) junto al pilar; soldado de la cuerda `082BCE` (2D8D6A, cámara 944, (312, 512)) | nativos |
| `0bdb`, `692f` (cofre arriba, agujeros con calaveras) | 1640–1834, y 736 | respiradero `03A05A` (2D8E58, (466, 775), mientras el progreso < 2144): columnas de gas morado (tipo 0x10, momia); soldados de la cuerda `082BCE` (2D8E12, 2D8F8A/2D8F9C) | nativos |
| `ccc7` (murciélagos sobre un cartel, camino que sube) | ~2080–2128 | el nido `035D12` y la bandada de puntos `035B66` (+6E = 1) de las cajas `08C154` vacías (2D922A/2D923C, 2D9296/2D92A8): rama al azar que ni `run4` ni estas capturas ejecutan | parada explícita (`0364D2`, `035FD0`) |
| `fd11` (jefe, CONTINUE) | 3391–3888 | el jefe (nativo) y, en la torre, `09692C` (2D9BEA/2D9BF6, elección al azar): el Slugnoid (dos torretas `03B4E0`, servicios de ranura `0AA610/0AA760`) | nativo (`neo_slugnoid.c`, ARMAS_VEHICULOS.md paso 17) |

Capturas (MAME 0.264, `capture_fullgame.lua`; `MSX_FULL_SAVE_AT` acepta
ahora una lista de fotogramas). La reproducción desde un estado intermedio
pierde el estado Lua del piloto y diverge de `run4`: las trazas que
necesitan la misma partida (el aliento de la momia sobre `032DE2`, las
muertes de `082BCE`) se toman desde `sec03_26140` con `MSX_FULL_FAMILY_FROM`
tarde:

```sh
export MSX_FULL_STATES=<estados> MSX_FULL_VRAM_TRACE=1 MSX_FULL_PERIOD=15 MSX_FULL_GRID_TAP=0
# estados intermedios (sin traza): MSX_FULL_SAVE_AT=26470,31480,37120,37960,38090,53680,57300
MSX_FULL_LOAD=sec03_26140 MSX_FULL_LOAD_FRAME=26140 MSX_FULL_FIRST_DUMP=26480 MSX_FULL_FAMILY_ROOTS=995ec MSX_FULL_FAMILY_FROM=26141 MSX_FULL_LAST=27500 ... gem
MSX_FULL_LOAD=sec03_26140 MSX_FULL_LOAD_FRAME=26140 MSX_FULL_FIRST_DUMP=36000 MSX_FULL_FAMILY_ROOTS=32de2 MSX_FULL_FAMILY_FROM=31481 MSX_FULL_LAST=37200 ... wrap2
MSX_FULL_LOAD=at_57300 MSX_FULL_LOAD_FRAME=57300 MSX_FULL_FIRST_DUMP=57305 MSX_FULL_FAMILY_ROOTS=3a05a,3a09c MSX_FULL_FAMILY_FROM=57301 MSX_FULL_LAST=58700 ... vent
MSX_FULL_LOAD=at_37120 MSX_FULL_LOAD_FRAME=37120 MSX_FULL_FIRST_DUMP=37125 MSX_FULL_FAMILY_ROOTS=82bce,3a2aa,3a05a MSX_FULL_FAMILY_FROM=37121 MSX_FULL_LAST=40000 ... rope
MSX_FULL_LOAD=sec03_26140 MSX_FULL_LOAD_FRAME=26140 (mismas raíces) MSX_FULL_FAMILY_FROM=37121 MSX_FULL_LAST=39200 ... rope2
MSX_FULL_LOAD=sec03_26140 MSX_FULL_LOAD_FRAME=26140 MSX_FULL_FIRST_DUMP=53690 MSX_FULL_FAMILY_ROOTS=82bce MSX_FULL_FAMILY_FROM=53681 MSX_FULL_LAST=55600 ... rope3
python3 tools/extract_family_fixture.py gem/boss-trace.txt --output data/treasure_fixture.json.gz --range 995ec-99860 --range 1c6e8-1ca9a --range 8d330-8d900 --attacks --ram 10000e-100010 --ram 10c7b6-10d7b6 --ram 1014ec-1014f4 --ram 1014fc-101500
python3 tools/extract_family_fixture.py wrap2/boss-trace.txt --output data/wrapped_fixture.json.gz --range 32de2-32fb4 --range 678a2-69700 --range 21cb6-21cee (mismos --ram)
python3 tools/extract_family_fixture.py vent/boss-trace.txt --output data/vent_fixture.json.gz --range 3a05a-3a2aa (mismos --ram)
python3 tools/extract_family_fixture.py rope2/boss-trace.txt --output data/ventwait_fixture.json.gz --range 3a05a-3a1ae (mismos --ram)
python3 tools/extract_family_fixture.py rope/boss-trace.txt --output data/rope_fixture.json.gz --range 82bce-83300 --attacks --ram 10000e-100010 --ram 10c7b6-10d7b6 --ram 1014c4-101624
python3 tools/extract_family_fixture.py rope3/boss-trace.txt --output data/ropedeath_fixture.json.gz --range 82bce-83300 (mismos --ram que rope)
```

Traducción (`src/neo_m2traps.c`; detalle de rutinas en su cabecera):

- `0995EC`: el tesoro oculto. Cada golpe (`004C24`) reinicia el programa
  `29F586`; al acabar, `29F5D8` (abierto) espera a que un jugador en juego
  (primero el 2) entre en el rectángulo `099830` (`01AF3C`): `08CECC` suma
  los puntos de `099858` (+6E = 1: 50000 en BCD) con `057948`, sonido 1121
  y la etiqueta `08D330`; después `29F602`, la fila `099844`, sonido 1022 y
  el nodo se convierte en la explosión `01C7AE` (paleta 0x26C, nueva entrada
  de la tabla de `neo_effects.c`). El recorte `099828` pasa a `09972E`
  (comprobación anticopia y `004188`) en la actualización siguiente.
- `032DE2`: no puede morir (cada golpe deja +4E = 0x64 y borra +5F bit 7);
  solo un contacto de tipo 0x10 (el aliento de las momias) lo despierta:
  destello `021CB6`, cuya llamada `022230` sube su +80 a 3, y entonces
  `068E94` lo convierte en el soldado `067AB4` de `neo_rebel3.c` con su
  regalo en +74/+76 (los `2D`/`2A` de B27). El programa `239492` llama a
  `239604` (+7E = FF una vez de cada cuatro).
- `03A05A`: cada +74 actualizaciones (`2D3A76`), en el rectángulo `03A900`,
  dos bocanadas `03A1AE` a (−20, −5) y (20, 5) o, cada +7A-ésima vez
  (`2D3AD8`), una en el centro; sonido 10EE. La bocanada (programa
  `23CABA[+72]`, paleta 0x4BB) ataca con la fila `3A8A0[+72][+78]` (tipo
  0x10) mientras su programa deja +78 ≥ 0. Se borra (`0041AC`) cuando el
  progreso de cámara llega a +70.
- `082BCE`: baja por la cuerda (`2818E4`, con caja y cuerpo sólido por la
  llamada `0833A8`), cuelga con colisión dinámica `E4C2E`, se gira hacia el
  jugador y lanza cuchillos (`083350` → `0830FE`, fila `E1A76[+60]`) cada
  `2D53B6` actualizaciones; cada golpe crea la salpicadura `0682F0` (como
  mucho una cada 0x18). Muerto: `082EAA` (sonido 1143), la llamada `083380`
  suelta el cabo `083212` y lo deja caer a −3,0 hasta el suelo (`082F1A`),
  cuerpo `281D66`. La muerte quemada (+5A = 0x0A/0x0B: `083018`, `068D32`)
  no sale en ninguna captura: **parada explícita** (`ga_fail 83018`).
- Banco de familias: `dyn_set` toma una ranura de colisión dinámica ocupada
  por un actor de fuera de la familia como ocupada (la RAM 10C06E.. tal como
  la leyó el original), igual que `0140D2`.

**Píxeles** (`export_rest_sprites.py ... gem@995ec-99860 gem@1c7ae-1ca9a
wrap2@32de2-32fb4 vent@3a05a-3a2aa rope@82bce-83300 rope3@82bce-83300
--mask-auto`): tesoro 62 volcados, 53610 iguales, 0 distintos; soldado
envuelto 31, 17658/0; respiradero 32, 4721/0; cuerda 10, 10038/0 y 103,
142458 iguales y **70 distintos en un volcado** (54495, tomado 3 fotogramas
después de la actualización; sin explicar). Paletas nuevas 0x104, 0x105,
0x26C, 0x16A. `romfs/mission2.pack` reempaquetado (3,47 MB).

**Ares** (`9e2b3cb`, `mslugx-m2-reference.z64`, `test_mission2_ares.py
--stage-seconds 800`: PASS hasta la Misión 3, crash=0): los registros
`2D89EE` (`0995EC`), `2D8C76` (`032DE2`), `2D8D6A`/`2D8E12`/`2D8F8A`/`2D8F9C`
(`082BCE`) y `2D8E58` (`03A05A`) se crean nativos (`MSX script spawn ...
native 1`); `03A2AA` sigue externo (`native 0`) y `09692C` se cuenta como
externo (5). Recorrido de la Misión 1: PASS, crash=0, 1 fallo de audio.

**Lo que queda de la etapa 3** (sin traducir, contado como externo o como
parada):

- `03A2AA` (2D8D82, cámara 952): la roca que cae, **nativa** en
  `work-m2fix7` (`neo_m2props.c`, cabecera): `03A344` cae (programa
  `23CD24`, fila `03A8E4` mientras cae), `03A3A2` sonido 1035, temblor
  `0C801A`, polvo `03A464`, escombros `01DA40` (`3A918/3A92A`), losa
  `03A528`, el creador +74 = 1, parches `0B7A2C` y la roca sigue cayendo
  hasta salir del rectángulo `03A908`; luego el creador suelta piedras
  `03A58A` cada +6E (`2D3B3A`). Captura (borrada tras extraer):
  `MSX_FULL_LOAD=sec03_26140 MSX_FULL_STATES=/home/user/captures-b31/states
  MSX_FULL_VRAM_TRACE=1 MSX_FULL_PERIOD=15 MSX_FULL_FIRST_DUMP=37900
  MSX_FULL_FAMILY_ROOTS=3a2aa MSX_FULL_FAMILY_FROM=37121 MSX_FULL_LAST=39300`;
  `extract_family_fixture.py ... --range 3a2aa-3a6ea --attacks` (mismos
  --ram que `gem`): familia `boulder`, 4 nodos, 283 actualizaciones
  (37979..38223), anillo de 10C72E igual. Las piedras `03A58A` no nacen en
  esa ventana: su rama no tiene captura propia. Píxeles
  (`export_rest_sprites.py ... boulder@3a2aa-3a6ea --mask-auto`): 13
  volcados, 18826 iguales, 61 distintos en uno (38115, dos fotogramas tras
  la actualización; sin explicar, como el de `rope`).
- ~~`09692C`~~: el Slugnoid, nativo (`neo_slugnoid.c`, B38).
- El nido `035D12` y el murciélago de puntos (`0364D2`): ramas al azar de
  las cajas `08C154` sin captura.
- `083018`/`068D32`: muerte quemada del soldado de la cuerda.
