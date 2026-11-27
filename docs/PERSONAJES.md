# Personajes jugables: Marco, Tarma, Eri y Fio (agente C, rama `work-chars`)

Estado: **los cuatro se eligen en SOLDIER SELECT y se juegan** (ares, cartucho
normal: `tools/test_characters_ares.py`, PASS). Marco es el catálogo de
siempre, sin cambios; los otros tres salen de la ROM por las mismas tablas,
con sus programas, poses, paletas, película de elección y voz de muerte.
Lo que la ROM hace por personaje y el port aún no hace está en «Pendiente».
La pausa es la de la ROM: [PAUSA.md](PAUSA.md).

> **Aviso de tamaño (para todos los agentes).** ares solo abre el canal de
> registro (ISViewer) si la ROM mide como mucho `0x03FF0000` =
> 67 043 328 bytes (`ares/n64/cartridge/cartridge.cpp`). `ccr` medía
> 65 650 688; con los personajes mide **66 846 720**: quedan
> **196 608 bytes**. Una ROM mayor arranca, pero sin registro, y todas
> las pruebas de ares fallan por tiempo de espera. Antes de añadir datos
> (misiones 4–6) hay que liberar espacio (p. ej. `howto.wav64`, 5,8 MB en
> PCM). `select.wav64` (346 KB) ya no va al cartucho cuando está la música
> de selección: `main.c` no lo leía.
> **Hecho** ([COMPRESION.md](COMPRESION.md) §5): el PCM de las cinemáticas
> va sin pérdida (MSXL) y las películas con el LZ4 óptimo; el cartucho con
> las seis misiones mide 61 407 232 bytes (quedan 5 636 096).

## 1. Cómo lo decide la ROM

- **La elección.** El panel del jugador (`0C961C`) escribe en el nodo del
  jugador la palabra **+6E**: 0 Marco, 1 Tarma, 2 Eri, 3 Fio. En pantalla el
  orden es Marco, Eri, Tarma, Fio. Medido en MAME 0.264 solo con el mando
  (`run_marco_catalog.py --character N`, `MSX_SELECT_INPUTS`): una Derecha
  por casilla; el cursor da la vuelta por los dos lados (Izquierda desde
  Marco = Fio; cuatro Derechas = Marco).
- **Lectores de +6E** (punto de observación en `101532`,
  `MSX_CHARACTER_WATCH=1`, y barrido del código del jugador):

| Rutina | Qué indexa con +6E | En el port |
| --- | --- | --- |
| `0C0F32`/`0C0F62`/`0C0F96` (selección), `0C0FB6`/`0C0FCE` (directa) | toda tabla de programas de cuerpo y arma: fila de la forma (+93 × 0x10) + 4 × personaje | pack del personaje (§2) |
| `0C11EA`/`0C1202`/`0C121A`, `0C2DF0`… | paletas: `0BEDD0` normal (0376/0377/0004/0005), `0BEDD8`, `0BEDE0`, `0BEDE8` quemado (0056/0056/0055/0056), `0BEDF0`, `0BEDF8`, `0BEE00` momia (0070..0073), `0BEE08`, `0BEE10` transformación | normal, quemado y momia; las demás tampoco las usa Marco |
| `0C294E` | voz de muerte: 1123, 1087, 1125, 114E | `msx_character_death_voice` |
| `0C419E` desde el programa del cuchillo | fila de ataque: Marco y Tarma `2D0AF4` (clase 2), Eri y Fio `2D0B04` (clase 3; misma caja y daño 0x1E) | `gameplay.c` |
| `0C39C8` (tabla `0C38FC`) | reposo largo `0BFCD8`: tras 0x0C ciclos Marco/Tarma, 0x07 Eri/Fio (y 0x32 solo Eri/Fio en cuatro filas) | pendiente (tampoco Marco) |
| `0C2E74` → `0C4C28` | solo Fio: al morir (muerte genérica `0C2DDA`) suelta un actor `0C4C64` (programa `253322`, paleta 5) | nativo; sprites validados, fuera del cartucho (§6.2, §7) |
| `0C359A` → `0C5064` | al dejar la forma 1: el arma que cae, programa `0C4F58` (Marco/Tarma) o `0C4FD6` (Eri/Fio), paleta `0BEDD0` | pendiente (tampoco Marco: G7) |
| `0C35AE`, `0C48FA` | copia +6E a los hijos (disparos, granadas) | sin efecto en lo portado |

  `0BD21E`/`0BD26C` leen el +6E de otro actor (su propio contador), no el
  personaje. En el código del jugador no apareció ninguna diferencia de
  velocidad, salto ni armas. Los programas del corte de Eri y Fio duran más
  (34/36 actualizaciones frente a 24/27/29), pero su prefijo hasta la
  escritura de +76 es el de Marco: el golpe cae en la misma actualización.
- **Paletas en la ROM** (`009B84`/`005978`): la paleta n son 16 palabras en
  `200000 + n × 32`; cada una, doblada **con signo**, indexa la tabla de
  colores `224000`. Comprobado contra la RAM de paleta de `marco-cat1`: 108
  de las 110 ranuras en uso son paletas de la ROM bit a bit. La segunda
  paleta de la momia (ranura 0x64) no es ninguna paleta de la ROM: sale de la
  captura de cada personaje como momia.

## 2. Datos: un pack por personaje

`tools/export_character.py --character N` → `romfs/chars/cN.bin` (privado;
copia de referencia en `/home/user/ws-logo/romfs/chars`):

- **Programas.** Cada uno de los 107 papeles de Marco
  (`build/marco_campaign.h`) es el largo de una fila de tabla en la columna
  0; el del personaje es el de la misma fila en su columna (si un programa de
  Marco está en varias filas, todas deben dar el mismo). Conversión con los
  formatos de `convert_marco_programs.py`. Un sitio de llamada cuya rutina y
  datos son los de un sitio medido de Marco es ese `EVENT`; `0C419E` con la
  fila `2D0B04` es el corte de Marco con otra fila. **0 órdenes sin
  recuperar** en los tres.
- **Muertes.** Los cinco programas de `marco_death.c` (`0BF298`, `0BF3D8`,
  `0BF358`, `0BF318`, `0BF498`), misma columna.
- **Poses.** Todo descriptor que esos programas pueden dibujar, descodificado
  de la ROM como `build_marco_poses.py` (piezas CI4 de 64×64 como máximo).
  Una pose que ya está en el catálogo de Marco (las piernas que Tarma
  comparte: 28) apunta a las piezas de `romfs/marco` en vez de llevar píxeles.
- **Paletas.** Normal, quemado y momia de la ROM; la segunda de la momia de
  la captura `cN-mz2` (`--mummy-capture`).
- **Fichero.** El pack en bloques LZ4 de 64 KiB (`tools/lz4_blocks.py`, el
  descodificador de `scene_codec.h`).

| | Órdenes | Poses | Piezas (propias) | Pack en RAM | Fichero en ROM |
| --- | --- | --- | --- | --- | --- |
| Tarma | 2102 | 688 (28 de Marco) | 713 (685) | 457 987 | 193 803 |
| Eri | 2121 | 706 | 737 | 470 721 | 200 435 |
| Fio | 2252 | 750 | 775 | 475 308 | 204 952 |

Las teselas apenas se repiten entre personajes (Eri y Fio comparten 100 de
~3900; Tarma y Eri, 2), así que un banco común no ahorra nada.

**El exportador contra Marco.** `--character 0` escribe el pack del propio
Marco (`build/chars/c0.bin`, solo pruebas): las piezas descodificadas y las
tres paletas de la ROM (y la segunda de la momia de `marco-mz2`) deben ser
las del catálogo ya compilado, y `tools/test_character.c` comprueba que sus
107 papeles son los programas compilados orden a orden y que 60 000
actualizaciones por todas las formas, más 64 muertes y entradas, dibujan lo
mismo con el pack que con el catálogo.

```sh
G=/home/user/captures-marco; C=/home/user/captures-chars
python3 tools/export_character.py --character 0 --rom ROMS/mslugx.zip --fixed $G/marco-cat1/program-fixed.bin \
  --graph $G/GRAFO/graph.json --mummy-capture $G/marco-mz2 --output build/chars/c0.bin
python3 tools/export_character.py --character 1 ... --mummy-capture $C/c1-mz2 --verify $C/c1-cat ... \
  --output romfs/chars/c1.bin --report build/chars/c1.json      # y 2, 3
cd tools; python3 extract_character_fixture.py --character 1 --report ../build/chars/c1.json \
  --captures $C --output ../data/character_fixture_c1.json; cd ..
make check-characters && make
```

## 3. Código

- `src/msx_character.c/.h` (también en host): lee el pack, descodifica los
  programas y los entrega a `marco_visual.c` (el papel conserva su nombre
  `MARCO_CAMPAIGN_*`; al arrancar se traduce a la entrada del personaje) y a
  `marco_death.c` (cinco programas). Sin pack: Marco, las tablas compiladas.
- `src/marco_renderer.c`: `marco_renderer_set_character` cambia el catálogo
  (poses, piezas y paletas del pack; las piezas compartidas se cargan de
  `romfs/marco`). Marco sigue por su camino de siempre.
- `src/actor.c` `actor_select_character`: suelta las piezas de Marco ya
  precargadas, carga el pack y cuenta su memoria en el presupuesto
  (`marco_renderer_preloaded_bytes`). Montón del personaje: Tarma 484 KB,
  Eri 497, Fio 504. Con el juego cargado quedan libres 546/544/538 KB
  (Marco 490); mínimo en la prueba 456/454/448 KB (Marco 400).
- `src/gameplay.c`: `msx_character_request` (+6E) da la voz de muerte y la
  fila del corte.
- `src/main.c`, SOLDIER SELECT: Izquierda/Derecha mueven el cursor (da la
  vuelta). La película `select.msxc` acaba con P1 sobre Marco; el cursor en
  otra casilla es el **primer fotograma de la película de elección de ese
  personaje** (la pantalla de selección con P1 sobre él en el instante de
  la pulsación), y elegir la reproduce desde ahí. Un personaje solo se
  ofrece si el cartucho trae su película, su sonido y su pack. El contador
  de la pantalla sigue parado, como ya lo estaba con Marco.

## 4. Capturas (MAME 0.264, solo entradas para elegir)

- **Catálogos** (`/home/user/captures-chars/cN-*`, privados):
  `run_marco_catalog.py --character N` con los mismos planes que Marco
  (`--first` = el de Marco + 202: la elección en 1800 aterriza 202
  fotogramas después; el catálogo base en 2350). Las escrituras de captura
  (dar la H, la C, armas, gordo, momia, victoria) son las de Marco.
  Auditoría `audit_marco_catalog.py` (descriptores de la ROM contra la VRAM
  y los píxeles de MAME), igual que Marco; en la C se excluyen los
  fotogramas del destello `0C58D2` (lista en `excluded.txt`).
- **Elección** (`sel-pK`): `run_intro_audio.py` con `MSX_SELECT_INPUTS`
  (Derecha en 3620/3640/3660) y la confirmación en 3700;
  `pack_cinematic.py --name confirm-cN --first-frame 3700 --last-frame 3909
  --audio-last-frame 3910`: 210 fotogramas, todos iguales a la imagen de
  MAME. +6E = 2/1/3 leído tras la elección. El estado de entrega (3940:
  rutina `0C162A`, x 7865136, y 16293888, vx 816, vy −483328) es el de Marco
  (`MSX_ENTRY_*`), así que `entry.msxc` y el descenso nativo sirven igual.
- **Sonido de la elección.** Las tres pistas son iguales salvo la voz
  (0,6–2,5 s). En PCM (454 KB cada una) la ROM pasaba del límite de ares:
  van en **VADPCM** como la música del juego
  (`recompress_cinematic_audio.py` = `pack_cinematic.py --wav-compress 1`,
  128 KB cada una, con la comprobación de descodificación de los efectos:
  0 ventanas malas). Es el único dato con pérdida de esta tarea.

## 5. Pruebas

- `make check-characters` (entra en `make check` si están los packs y
  `build/chars/c0.bin`): el pack de Marco contra el catálogo compilado (§2)
  y cada personaje pose a pose contra sus capturas: los casos del fixture
  de Marco (`data/marco_throw_fixture.json`) con las mismas condiciones de
  entrada y la cadena de programas del personaje
  (`tools/extract_character_fixture.py` → `data/character_fixture_cN.json`).

| | Capturas auditadas | Fotogramas / píxeles iguales en MAME | Casos | Pasos iguales | Distintos |
| --- | --- | --- | --- | --- | --- |
| Tarma | 16 | 19 938 / 14 363 638 | 42 | 852 | 0 |
| Eri | 16 | 19 936 / 13 761 246 | 41 | 867 | 0 |
| Fio | 16 | 19 932 / 13 105 036 | 41 | 867 | 0 |

  Las 16 capturas son las de Marco: catálogo base, H (`h2`, `h3`), granadas
  con la H (`g1`), C (`c1`), armas 02/04/06 (`w2`, `w4`, `w6`), cuchillo con
  pistola y con H (`k1`, `k2`), descenso (`dsc0`, `dsc1`), victoria (`v0`,
  `v1`), gordo (`f2`) y momia (`mz2`). Los casos cubren granadas (de pie, en
  carrera, en el aire, agachado), disparo de la C y de las armas 02 y 04,
  la H hacia abajo, cortes, descenso, victoria, gordo y momia. Un caso
  falta en Eri y en Fio: `h_knife_crouch` (su programa de corte agachado
  con la H, `258C16`/`258E84`, no se eligió en esa condición del plan: su
  corte anterior dura más). En cada captura, los descriptores que el
  original dibujó y no están en el pack son de programas que el port no
  arranca; las cuentas coinciden con las de Marco (p. ej. `cat`: 142 de
  174 Tarma, 142 de 172 Marco). En `c1` y `k2` se excluyen de la auditoría
  entre 4 y 16 fotogramas con un destello sobre el jugador (`excluded.txt`
  en cada captura, como los de `marco-c1`).

- `tools/test_marco_visual.c` (Marco, `make check-marco-campaign`): 852
  pasos iguales, como antes del cambio.
- ares, cartucho normal (`tools/test_characters_ares.py`): por cada casilla,
  Derecha hasta ella (`MSX SELECT place=K character=N`), Start
  (`MSX CHARACTER N chosen`), película, descenso, unos segundos de juego
  (andar, disparar, saltar, granada, agacharse, arriba), pausa y reanudar, y
  seguir andando hasta la primera muerte (su voz).
  Ningún descriptor fuera del catálogo, ningún programa parado, ningún
  error de pack. **PASS los cuatro.**

## 6. Pendiente

1. **Voz de muerte de Tarma (`1087`)**: el juego la pide, pero **no suena**
   (`MSX AUDIO voice 1087: unmapped`: silencio, no la de Marco). Las de Eri
   (`1125`) y Fio (`114E`) **ya suenan**: sus 368 pares están en
   `data/sound_pairs.json`; `derive_sound_policy.py` las deja en el canal
   VOZ, nivel 2, como la `1123` de Marco, sin cambiar ningún otro sonido
   (14 canales, mismas asignaciones); WAV64 de 10 KB cada una, leídas de
   `rom:/` (sin RAM); la prueba de ares mata al personaje y comprueba
   `MSX AUDIO voice 1125/114E: started`. La `1087` está renderizada
   (0,486 s, `filesystem/audio-src/sfx_1087.wav`) y medida (187 pares en
   `data/sound_pairs_characters.json`), pero no se comporta como las otras
   tres: 106 de sus 181 pares comunes con `1123` salen distintos, muchos
   «descarte» en los dos órdenes (contra `1084`, `108C`, `108E`..`1092`,
   `104F`, `1056`, `10F9`, `10FB`, `10FF`…), el par `1087>1123` sale
   mudo y en `1123>1087` y `108E>1087` el residuo del ajuste supera la
   mitad de la mezcla (medida poco fiable). Con ellos la política une
   canales (14 → 9) y cambia 34 sonidos ya asignados, así que no se ha
   fusionado. Falta: una captura de MAME de una muerte de Tarma con audio
   (punto 5) para comparar lo que suena con el render aislado, y leer en
   el controlador Z80 qué canal y prioridad usa `1087`, antes de darle
   canal. La tabla `0C294E` de la ROM es `1123 1087 1125 114E` (comprobada).
2. **Fio al morir** (`0C4C28` → `0C4C64`): el actor ya es nativo
   (`neo_effects.c`, familia `fio_cap`). Sus 12 descriptores
   `1A5A6A..1A5AD8` (programa `253322`, paleta 5) están validados contra
   MAME (captura `c3d3`, §7) y su raíz está en `export_rest_sprites.py`
   (exportación de la M1, `scene.pack`), pero **siguen sin entrar en el
   cartucho**: el export de la M1 (`filesystem/rest-native.json`) se perdió
   con el reinicio del contenedor (27-XI) y `scene.pack`/`scene_meta.h`
   salen hoy de la ROM 52. Al rehacer el export de la M1: la orden de §7.
3. **Reposo largo** (`0C39C8`, tabla `0C38FC`, programas `0BFCD8`): no está
   para ninguno; al portarlo, el umbral es por personaje.
4. **Arma que cae al dejar la forma 1** (`0C5064`): no está para ninguno.
5. **Muertes por personaje sin captura**: los programas son los de la ROM,
   pero no hay prueba pose a pose de las muertes de Tarma, Eri y Fio
   (`capture_weapons.lua` no elige personaje).
6. **Vehículos**: la Camel Slug (B82, `work-fix`) y el Slugnoid (§7) dibujan
   el piloto de cada personaje; el SV-001 de la M4 y el vehículo de la M3
   siguen sin recorrido de ares con los cuatro.
7. Lo que ya faltaba a Marco falta igual: programas que el original elige y
   el port no arranca (los mismos papeles en los cuatro), el contador de
   SOLDIER SELECT y el parpadeo de P1.
8. Los registros `debug.log` de `captures-chars` están en gzip (disco):
   descomprimir antes de repetir `export_character.py --verify` o los
   fixtures.

## 7. Pilotos del Slugnoid y actor de muerte de Fio (B82, carril de vehículos, 27-XI)

**Lo que hace la ROM.** El Slugnoid `09692C` crea su piloto `097830`/`097886`
con el andador; el piloto dibuja la tabla `09649C[+70 del andador]` con la
paleta `034330[+70]`, y `0AA760` copia en `+70` el `+6E` del jinete al
montar (antes de montar `+70` = 0: la tabla de Marco, como en MAME). Las
tablas `09649C[1..3]` (`096530`, `0965B4`, `096638`; 27 programas cada una,
`29E120..29ED1A`) posan 50 descriptores propios cada una (Tarma
`197FDC..198310`, Eri `19EC0E..19EF48`, Fio `1A60CA..1A6406`), una pieza de
64×64 como máximo cada uno.

**Causa.** Los programas de esas tablas estaban en el grupo `m5` (solo en el
pack de la M5: en la M2 el piloto paraba en `ga_anim_start`) y sus
descriptores no estaban exportados en ningún pack (en la M5, dibujos
ausentes): con los tres en las tablas de la M2 la caché quedaba bajo el
suelo de 208 ranuras.

**Arreglo: solo el piloto del personaje elegido.**
- Programas: sin grupo (`extract_soldier_programs.py`): van a
  `programs.bin`, que se lee bajo demanda; la consola solo guarda 69
  referencias (8 bytes cada una) y lee los del personaje que monta.
- Sprites: pack de misión **versión 6** (`pack_mission.py`
  `rider_descriptors`, `pack_store.py`): los descriptores que solo posa una
  tabla `09649C[c]` (c = 1..3) son las últimas imágenes de actor, un bloque
  por personaje, con el registro `MSXC` (`rider_first`, `count[3]`) tras los
  recursos. `scene_stage_load` (`scene_assets.c`) lee las imágenes comunes
  y **solo el bloque del personaje elegido** (`scene_assets_set_rider`,
  desde `actor_select_character`) y deja fuera los descriptores de los otros
  dos bloques (la tabla sigue ordenada). Línea `MSX stage N riders:
  character C, K of T actor images kept`. Solo las misiones 2 y 5 llevan
  los bloques.
- Sin las fuentes de los packs (perdidas el 27-XI), los packs de la ROM 52
  se reescriben con `tools/pack_add_actors.py`: añade a los packs de la M2 y
  la M5 los fotogramas validados (páginas nuevas en fragmentos nuevos; la M5
  toma los 19 fragmentos de la M2), las paletas `377`/`4`/`5` que faltan, y
  corrige en los demás packs las referencias a los reescritos (fragmentos
  prestados, `MSXD` prestado, tamaño y CRC del remolque). El cargador del
  anfitrión sobre los packs de antes y los de después
  (`MSX_RIDERS=… build/host/pack-loader`): **idénticos** salvo las 900
  líneas de los pilotos añadidos, las paletas nuevas y las cuentas de
  imágenes; M3, M4, M6 y M13 sin cambios en lo servido.
- `SCENE_MISSION_CHUNKS_MAX` 1256 → 1272 (la M5 pasa a 1269 fragmentos).

```sh
# Capturas (MAME 0.264, estado del TAS u17460: M5 etapa 3; +6E escrito tras la carga)
MSX_FULL_SET_CHARACTER=N MSX_FULL_LOAD=u17460 MSX_FULL_LOAD_FRAME=38007 MSX_FULL_RIDE=tour \
  MSX_FULL_VEHICLE_CHEAT=0 MSX_FULL_FAMILY_ROOTS=96a60,96c3a,3b57c,97886,97830,3527e,3533a,96b32,96b62,96c22 \
  MSX_FULL_FAMILY_RANGE=966f0-97e08 MSX_FULL_FAMILY_FROM=38008 MSX_FULL_PERIOD=6 MSX_FULL_FIRST_DUMP=38008 \
  MSX_FULL_LAST=41000 MSX_FULL_STATES=TAS/sync264/states tools/run_capture_fullgame.sh slugN
# Sprites (tabla de N: 29dd22-29e11e, 29e120-29e51e, 29e51e-29e91c, 29e91c-29ed1a)
tools/export_rest_sprites.py ZIP slugN@97830-97971 --roots-from LO-HI --only --keep NADA --output slugN.json
# Familias: extract_family_fixture.py slugN/boss-trace.txt --output data/slugpN_fixture.json.gz $A (A de ARMAS_VEHICULOS.md, más --range 3533a-3539f)
# Packs
tools/pack_add_actors.py ROM52/romfs OUT --resources 377,4,5 --add 2:slug1.json,slug2.json,slug3.json --add 5:slug1.json,slug2.json,slug3.json
```

**Pruebas.**
- MAME 0.264, 4 capturas `slug0..slug3` (dos montas, 38131..39133 y
  39241..40309, el andador destruido con jinete): sprites del piloto contra
  la VRAM, 197 volcados cada una: Marco 65 202, Tarma 66 857, Eri 63 950,
  Fio 58 021 píxeles iguales, **0 distintos**.
- Familias `slugp0..slugp3` en `make check` (43 nodos, 4575
  actualizaciones 38008..40310 cada una): **iguales bit a bit** (también
  `slughit` y `slugair`). Nota: en el anfitrión todas las tablas de
  programas están compiladas, así que estas familias ya habrían pasado antes
  del arreglo; lo que falla antes y pasa después es el cargador (packs sin
  los bloques: el piloto no tiene descriptores) y la prueba de ares.
- ares (cartucho normal, `test_vehicle_damage_ares.py --mission m5s3
  --character N --rider --ride forward`): resultados en `BUG_MATRIX.md` B82.
- Caché de escena: 218 ranuras (suelo 208) en el arranque.

**Actor de muerte de Fio.** Captura `c3d3` (`run_marco_catalog.py
--character 3 --first 2352 --hit 400:0 --child-routines c4c64,c4cba
--family-routines c4c64-c4dd6 --family-frames 2750-2940`, plan de reposo de
640 fotogramas): sus 85 actualizaciones son las de `data/fio_cap_fixture.json`
(la familia `fio_cap`; el golpe cae 2 fotogramas después que en `c3d2`);
`export_rest_sprites.py ZIP c3d3@c4c64-c4dd6 --roots-from 253322-253323
--only`: 12 descriptores, 171 volcados, 11 562 píxeles iguales, **0
distintos**. Falta meterlos en el cartucho (punto 2 de §6).

**Siguiente paso:** la voz de Tarma (punto 1) y, después, la
captura de una muerte de cada personaje (`capture_weapons.lua` con la
elección por entradas de `capture_marco_catalog.lua`) para probar sus
muertes pose a pose y traducir el actor de Fio.
