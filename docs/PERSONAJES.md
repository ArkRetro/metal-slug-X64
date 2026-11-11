# Personajes jugables: Marco, Tarma, Eri y Fio (agente C, rama `work-chars`)

Estado: **los cuatro se eligen en SOLDIER SELECT y se juegan** (ares, cartucho
normal: `tools/test_characters_ares.py`, PASS). Marco es el catálogo de
siempre, sin cambios; los otros tres salen de la ROM por las mismas tablas,
con sus programas, poses, paletas, película de elección y voz de muerte.
Lo que la ROM hace por personaje y el port aún no hace está en «Pendiente».

> **Aviso de tamaño (para todos los agentes).** ares solo abre el canal de
> registro (ISViewer) si la ROM mide como mucho `0x03FF0000` =
> 67 043 328 bytes (`ares/n64/cartridge/cartridge.cpp`). `ccr` medía
> 65 650 688; con los personajes mide **66 846 720**: quedan
> **196 608 bytes**. Una ROM mayor arranca, pero sin registro, y todas
> las pruebas de ares fallan por tiempo de espera. Antes de añadir datos
> (misiones 4–6) hay que liberar espacio (p. ej. `howto.wav64`, 5,8 MB en
> PCM). `select.wav64` (346 KB) ya no va al cartucho cuando está la música
> de selección: `main.c` no lo leía.

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
| `0C2E74` → `0C4C28` | solo Fio: al morir (muerte genérica `0C2DDA`) suelta un actor `0C4C64` (programa `253322`, paleta 5) | pendiente |
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
  (andar, disparar, saltar, granada, agacharse, arriba), pausa y reanudar.
  Ningún descriptor fuera del catálogo, ningún programa parado, ningún
  error de pack. **PASS los cuatro.**

## 6. Pendiente

1. **Voces de muerte** 1087/1125/114E: el juego ya pide la palabra del personaje, pero
   **no suenan** (`MSX AUDIO ... unmapped`: sin sonido, no el de Marco). Hecho:
   render aislado de las tres (`render_sound_commands.py`: 0,486 s, 0,729 s
   y 0,724 s, `OK`; los WAV en `filesystem/audio-src`) y 555 pares medidos
   (`data/sound_pairs_characters.json`). Las tres salen como canal VOZ,
   nivel 2, igual que `1123`. Falta: con esos pares
   `derive_sound_policy.py` une canales de la política actual (14 → 9) y
   cambia 34 sonidos ya asignados, así que no se han fusionado; hay que
   revisar los puentes (`--bridge`) como en B30/B36 y luego
   `compile_sound_policy.py` y `pack_game_audio.py` (≈30 KB de ROM).
2. **Fio al morir** (`0C4C28` → `0C4C64`): el actor de `253322` con su
   física (`0C4CBA`: +30 −= +30/16, +34 hacia |2·+30| − 6.0, suelo `0143E4`,
   15 fines de programa → parpadeo, 30 → fuera). Falta la captura de una
   muerte de Fio con traza de familia y sus poses en el pack. No se dibuja.
3. **Reposo largo** (`0C39C8`, tabla `0C38FC`, programas `0BFCD8`): no está
   para ninguno; al portarlo, el umbral es por personaje.
4. **Arma que cae al dejar la forma 1** (`0C5064`): no está para ninguno.
5. **Muertes por personaje sin captura**: los programas son los de la ROM,
   pero no hay prueba pose a pose de las muertes de Tarma, Eri y Fio
   (`capture_weapons.lua` no elige personaje).
6. **Vehículos**: no se ha medido si algún vehículo dibuja al piloto por
   personaje (el punto de observación solo cubrió juego a pie).
7. Lo que ya faltaba a Marco falta igual: programas que el original elige y
   el port no arranca (los mismos papeles en los cuatro), el contador de
   SOLDIER SELECT y el parpadeo de P1.
8. Los registros `debug.log` de `captures-chars` están en gzip (disco):
   descomprimir antes de repetir `export_character.py --verify` o los
   fixtures.

**Siguiente paso:** las voces de muerte (punto 1) y, después, la
captura de una muerte de cada personaje (`capture_weapons.lua` con la
elección por entradas de `capture_marco_catalog.lua`) para probar sus
muertes pose a pose y traducir el actor de Fio.
