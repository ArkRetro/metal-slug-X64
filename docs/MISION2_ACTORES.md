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

## Pendiente: salida de la etapa 1 `0B6ED2` (`0B6F24`)

Traducción leída, no portada (parada explícita). Creado por `2A18A` desde
el objeto `03889C` (1035C4) con +6E = rectángulo `0B6ECA` (−12, −1, 0x80,
0x32) y +72 = 0x2A0. `0B6ED2`: espera `0193FE` (un jugador existente con +5F
bit 7 a 0, o `0C8D0C` 101204 +8D); luego `018F92` (fin de misión: 4300 = FF,
4304 = +5E, manejadores `0C0538`, `0AAA88` → 4306/4307), `0C09D8`, `0C8CD6`,
10C00B = FF y `0CA340` ×2 (10EA00/10EA02 = 0x2A0, manejador `0CA3EC`: el
jugador camina a la derecha hasta x de pantalla 0x2A0, solo en el suelo).
`0B6F1E`: `01ABC8` y `01B2CE` (todos los jugadores activos dentro del
rectángulo, `01AF3C`) → `018F40` (4303 = 0) en cada actualización desde
19859. Necesita la caminata automática en el lado del jugador (gameplay) y
el banco de pruebas de familias no compara más de 12 escrituras globales
por actualización (esta escribe 10E9F8..10EA03 dos veces).

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

## Enemigos más frecuentes (no portados)

Ninguno es variante de una familia nativa:

| Constructor | Lectura |
| --- | --- |
| `0613CA/0613D8/0613E6` (×20/×39/×70) | una familia, variante en +96 bit 0/1/2 (`062114`/`062190`/`06224E`), paletas 0x1F6/0x1F7, destello 0x811, cuerpo sólido `E1262` (`004E76`) |
| `0678B6/0678C0/0678CA/067902` (×6/–/×27/×4) | una familia, variante en +9C (0 u 8), suelo `0146E8`, `01436A`, objetivo `01AF9A` |
| `0692B2..0692E4` (×9) | seis entradas, +9C = 0/4/8 |
| `03670E` (×8) | +96 = 4, paletas 0x142/0x143, programa `23BA74` |

Cada una necesita su traza de familia (las capturas desde `states4` cuestan
segundos) y una traducción completa.
