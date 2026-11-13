# Misión 6 (la final): fase 1 (captura, mapa, colisión, cámara, paquete, inventario)

Rama `work-m6` (desde `ccr-09eb2f6b-8rwl8l`), árbol `/home/user/ws-m6` (el antiguo `ws-m5`, movido con
`git worktree move`: conserva `build/`, `romfs/` y el `filesystem/` de enlaces). Mismo método que la
[Misión 5](MISION5.md): nada inventado, nada ajustado; lo que no está portado se para y se cuenta. ROM, capturas y
derivados siguen fuera de git: privados en `/home/user/ws-logo/filesystem/mission6/` y `romfs/mission6.pack`.
Inventario de actores: [MISION6_ACTORES.md](MISION6_ACTORES.md).

## Flujo: tres registros de etapa y el final

`data/mission_flow_fixture.json` (`run4`): la Misión 6 (42F4 = 5) son **tres registros de etapa**; tras el tercero
vienen en la ROM un registro de tipo 4 (`01761E`, 6 bytes) y **otro registro de etapa, `017624`** (descriptor
`01708E`: lista `0BEC28`, jugadores `2D629A`, sin guion, `+C` = `0x21`, etapa 0x12, entrada `2D6290`): la huida tras
el último enemigo (en el TAS, la sección −1 de 57041 a 62172). `run4` no llega a él; no está decodificado ni en el
paquete (fase 2, con el final).

| Etapa (42F6) | Registro | Descriptor | Lista de cámara | Jugadores `+4` | Guion (tabla `0x800`) | Música `+C` | Entrada (`+0E`) |
| --- | --- | --- | --- | --- | --- | --- | --- |
| 9, la costa y el puente | `0175C6` | `016E5E` | `0BCA00` | `2D6132` | `0x824` → `2DF678..2E0038` (168 sentencias) | `0x28` | `2D6128`: clase 2 (`0C15B4`, el descenso en paracaídas) |
| 10, la fábrica | `0175E6` | `016E6E` | `0BDCE2` | `2D622E` | `0x828` → `2E003A..2E19E4` (366) | `0x27` | `2D6224`: clase 3 (andando, x 0x68) |
| 11, la arena final | `017604` | `016E7E` | `0BE99E` | `2D6282` | `0x82C` → `2E19E6..2E1A02` (4) | `0x2B` | `2D6278`: clase 3 (x 0xC0) |

En MAME (`run4`): carga de la etapa 9 en 292683..292690, bucle `0186A0` en 292737, fin de etapa `018F92` (actor
`0B6ED2`) en 304151; etapa 10: carga en 304271, bucle en 304348, fin en 322152; etapa 11: carga en 322339, bucle en
322415, y **`run4` se acaba en 358827 peleando con el último enemigo (cámara 320) sin vencerlo**. En el TAS
([TAS.md](TAS.md)): secciones 9, 10 y 11 en las actualizaciones 18679, 20251 y 23570 (fotogramas 40676, 44471,
51147), último enemigo vencido en U 26423 (57038..57041), y después el final y los créditos (abajo).

**`run4` cubre las etapas 1 y 2 enteras y la 3 sin el final; el final y los créditos solo están en el TAS.** El enlace
desde la M5 es el genérico (`neo_mission.c` elige el registro siguiente y `actor.c` carga `romfs/mission<N>.pack` por
número): `test_mission_flow` sigue igual a `run4` y con `mission6.pack` la carga funciona sin código nuevo; hoy no se
puede recorrer desde la M5 porque su jefe y su fin de misión no están portados. Menú de pruebas:
`MISSION 6 STAGE 1/2/3` (`main.c`; `tools/test_mission4_ares.py` sube ahora siete veces y
`test_mission5_ares.py` 7 − etapa).

## Capturas (MAME 0.264), en `/home/user/captures-m6/`

1. **Réplica de `run4`** desde sus estados de sección (`states4/sec09_292683`, `sec10_304271`, `sec11_322339`,
   copiados a `states/`): `tools/capture_m6.sh 9|10|11 OUT`, una ejecución por etapa con todo a la vez (sondas de
   `capture_m5.sh` más el segundo estampador `0C8A3C` y el *latch* `320000`). `tools/compare_m5_replay.py` contra
   `run4`, las 36 columnas de `trace.csv` fotograma a fotograma: **0 diferencias** en 292685..304270 y
   304273..322338; en 322341..358820 solo la columna `inputs` del fotograma 322341 (durante la carga; las otras 35
   columnas, RNG y paleta incluidos, iguales en los 36 480 fotogramas). `run4` y sus réplicas llevan las escrituras de
   referencia del piloto (supervivencia, vidas, munición y arma); no escriben nivel, guion, enemigos ni colisión.
2. **El TAS por actualización** (`tools/capture_m6_tas.sh`, `tas_updates.lua` con sus grifos de solo lectura):
   **ninguna escritura en la memoria**, 0 desincronizaciones contra `updates251.txt` en los seis tramos. Es la
   comprobación sin escrituras de RAM del mapa y la cámara.
3. **Final y créditos** (`ending/run.sh`: el TAS desde `u26400` hasta el fotograma 75 000 con sondas de solo lectura
   del depurador, `MSX_TAS_DEBUG_CMDS`, nuevo en `tas_updates.lua`): creaciones, programas, sonidos y *latch*; fotos
   cada 300 actualizaciones; estados `end26480`, `end28960`, `end34170`.

| Captura | Contenido | Queda |
| --- | --- | --- |
| estados | `sec09/10/11`, `at_294000`, `296000`, `298500`, `301000`, `303000` (etapa 1), `306000`..`321000` cada 3000 (etapa 2), `323500`, `325000`, `330000`, `340000`, `350000` (último enemigo), `end26480`, `end28960`, `end34170` (TAS: tras la muerte, antes de los créditos, fin de los créditos) | `states/mslugx/` |
| actores, mapa, cámara, sonido | `debug.log` (`ALLOC`, `FREE`, `SPAWN`, `OP`, `WAIT`, `ANIM`, `SND`, `ITEM`, `CAM`, `END`, `PATCH`, `STAMP`, `STAMP2`, `LATCH`), `cells.txt`, `events.csv` (los dos anillos, `10C7B6` y `10D83E`), `camera.csv` | `s9`, `s10`, `s11` → `data/mission6_actors.json`, `m6_map_fixture.json.gz`, `m6s<k>_camera_fixture.json.gz` |
| TAS | cámara desde los estados de sección (`tas9/10/11`); celdas y anillos desde el estado anterior (`tasmap9/10/11`: `u18060`, `u20160`, `u23460`) | → `m6tas_map_fixture.json.gz`, `m6tas<k>_camera_fixture.json.gz` |
| final | `ending/out/debug.log`, `snap_u*.png` | `ending/` |
| ares | fotos y registro de los recorridos | `ares/s1`, `s2`, `s3` |
| VRAM, paletas, RAM, capas | `run4` (cada 30 fotogramas), 292683..358827 | `/home/user/captures-fullgame/run4` |

Las trazas en bruto (`trace`, `layers`, `actorstate`, `platforms`, bancos, `wav` de la música) se borraron tras
comprobar la réplica y convertir.

## Listas de cámara (`decode_map_commands.py --address`)

| Lista | Órdenes |
| --- | --- |
| `0BCA00` (etapa 1, 105 órdenes) | gráficos `0BC684`, `10C00E` ← `0BC67C`, paletas `0BC8DE`, punto 0x78/0x180, autoanimación 4. **Cuatro objetos de capa**: `10C72E` ← `0BC6BA` (columna 0, fila alta 27, tabla `0BC780`), `10D7B6` ← `0BC6E6` (88, 11, tabla `0BC8BE`; **sin teselas, solo colisión**), `10E83E` ← `0BC712` (0, 20, tabla `0BC8D2`), `10E8C6` ← `0BC6FC` (0, 2, tabla `0BC8C4`); factores 0x100/0x100 salvo `10E83E` 0x80/0x80. 18 llamadas (`0BCDBA`, `0BCDC2`, `0BCF36`, `0BCF86`, `0BD71E` en la cabeza, `0BDA8E` en el progreso 32, y después `0BD3B6`, `0BCE22`, `0BD2FA`, `0BD2E2`, `0BCF8C`, `0BD386`, `0BD3E6`, `0BCF94`, `0BD6BA`, `0BD53E`, `0BD67A`). Progreso 880..960: modo 5; 1056: **`10E83E` ← `0BC728`** (32, 21) y banderas de capa (1, 0); 1072 y 1888: op 4 (jugador en un rectángulo); 2112: modo 2; 2320: **`10E83E` ← `0BC73E`** (69, 27), factores 0x80/0x40; 2352: límites (0, −200) (el jefe intermedio); 2368: modo 7; 2448..2560: tres op 4; 2656: `10E8C6` ← `0BC76A` (186, 2; sin teselas); 2848: modo 4; 3200: modo 0; retención (límite 3632) |
| `0BDCE2` (etapa 2) | gráficos `0BDB6A`, paletas `0BDBDE`, punto 0x78/0x180, autoanimación 4; `10C72E` ← `0BDB86` (0, 16), `10E83E` ← `0BDB9C` (0, 16), factores 0x100/0x100 y 0x80/0x80; límites (0, 0, 0, 16); llamada `0BDF3A`; progreso 1792: **`0B0B72` (`10C72E`, 160, 32): el bucle del pasillo**; 3952: `0BE888`; retención (límite 4272) |
| `0BE99E` (etapa 3) | gráficos `0BE906`, paletas `0BDBDE` y `0BE98A`, punto 0x78/0x180, autoanimación 6; `10C72E` ← `0BE932` (0, 16, tabla `0BE974`), `10E83E` ← `0BE948` (0, 16); factores `10C72E` y `10D7B6` 0x100/0x100, `10E83E` 0x80/0x100; límites (0, 0, 0, 16); llamada `0BEAB2`; progreso 320: límites (−96, 0, 0, 16) y retención (límite 320) |

Mapas (descriptor: teselas, colisión, columnas × filas de bloques de 16 px): `0BC6BA` (`34DCB0`, `45902A`, 243 × 28),
`0BC6E6` (sin teselas, `45FC2A`, 60 × 5), `0BC712` (`3551B0`, 32 × 20), `0BC6FC` (`354770`, 192 × 3), `0BC728`
(`355BB0`, 32 × 20), `0BC73E` (`3565B0`, 32 × 21), `0BC76A` (sin teselas ni colisión, 32 × 3), `0BDB86` (`358220`,
`460CDA`, 160 × 17), `0BDB9C` (`35ACA0`, 32 × 15), `0BE932` (`35B420`, `46375A`, 48 × 17), `0BE948` (`35C0E0`,
30 × 14).

Orden de dibujo (RAM de `run4`, constante en cada etapa; filas nuevas de `draw_windows`): etapa 9: `10D7B6` 0x00
(nada que dibujar), `10E83E` 0x20, `10C72E` 0x40, ventana A `10B954` 0x60, **`10E8C6` 0x124 (por delante de los
sprites)**, ventana B 0x144; etapa 10: `10E83E` 0x00, `10C72E` 0x20, ventana A 0x40, sin ventana B; etapa 11:
`10E83E` 0x00, ventana B 0x20, `10D7B6` 0x80, `10C72E` 0xA0, ventana A 0xC0.

## Mapa y colisión desde la ROM (`tools/export_stage_rom.py`)

```sh
sh tools/build_mission6.sh          # observado (una vez), ROM, paquete
```

| Etapa | Rejilla (celdas de 8 px) | Celdas no vacías | Contra la rejilla observada | Bloques de ROM en el paquete | Contra los bloques observados |
| --- | --- | --- | --- | --- | --- |
| 1 | 486 × 56 | 1874 | 17 804 iguales, 0 distintas, 41 fuera (vacías) | `0BC6BA` 3449, `0BC712` 1280, `0BC6FC` 544, `0BC728` 1280, `0BC73E` 2016 | 2942/2942, 790/790, (228 de `0BC6FC` por código), 608 iguales y 300 distintos (abajo), 801/801 |
| 2 | 578 × 34 | 906 | 18 632 iguales, 0 distintas | `0BDB86` 4913 (con el bucle), `0BDB9C` 2400 | 2197 iguales y 29 distintos (los 4 parches `0C70FC`), 2075/2075 |
| 3 | 96 × 34 | 142 | 2649 iguales, 0 distintas, 10 de eventos | `0BE932` 522, `0BE948` 420 | 389/389, 402/402 |

Lo nuevo respecto a la M5, todo sacado de la ROM y comprobado con MAME:

- **El bucle del pasillo (etapa 2)**: la llamada `0B0B72` de la lista crea el actor `0B0B94` con (`10C72E`, 160, 32):
  cada actualización, si `+36 + 0x20 − +32 ≥ 160`, suma 32 al origen de columnas `+32` con `0C6F82`. La última
  columna del anillo es `+36 + 0x1F`, así que el salto llega cuando esa columna enseñaría la columna 159 del mapa: **la
  columna de bloques 159 ya enseña la 127**, y de ahí en adelante se repiten las columnas 127..158. MAME lo confirma
  celda a celda (las columnas 319, 383, 447 y 511 de la rejilla son la 255 del mapa, no la 319: con el bucle desde
  160 quedaban 4 celdas distintas; con 159, 0 de 18 632). La rejilla y el modelo repiten hasta el límite final de la
  lista (4272) más una vista.
- **Varios mapas en un mismo objeto de fondo (etapa 1)**: `10E83E` recibe `0BC712`, `0BC728` (progreso 1056) y
  `0BC73E` (2320) sin volver a la raíz. Son mapas de 32 columnas que se repiten cada 512 px y sus tramos de x se
  solapan, así que el modelo guarda la carga n-ésima en la página de x `0x2000·n` (`LOAD_PAGE`) y el runtime suma la
  página de la carga actual del objeto (`neo_layer_t.loads`, `actor.c`, solo misión ≥ 6). El tramo de cada mapa sale
  de la lista: x de la capa en el progreso de su carga y en el de la siguiente (segmentos de factor de la op 0x1C) más
  una vista.
- **`0BC728`, 300 bloques «distintos»**: son los de la primera pantalla tras el cambio (x 528..832, vistos 11 veces):
  MAME los enseña una fila (16 px) más abajo de lo que da la fila alta 21; los que entran después (x ≥ 848) coinciden.
  La lista pone en ese instante las banderas de capa (1, 0) y llama a `0BCE22` y `0BD2FA`: la capa la mueve su dueño.
  El modelo lleva la colocación de la ROM (0C741A, sin costura); el movimiento del dueño es de la fase 2.
- **Segundo objeto de terreno `10D7B6` (etapa 1)**: su mapa `0BC6E6` no tiene teselas, solo colisión (los cables del
  puente: celdas `81..8A` en diagonal). El núcleo lee de su anillo `10D83E` más que del principal (154 154 lecturas
  frente a 94 539). `extract_m6_map_fixture.py` las explica con la historia del anillo: 20 261 iguales al mapa de ROM
  en la colocación de la lista (columna 88, fila alta 11), 130 657 donde no hay celda, 170 por el anillo y **3066 de
  celdas del mapa cargadas con otro origen de columnas** (su dueño mueve `+32`; no está en la captura). El paquete
  **no** lleva esta rejilla y el núcleo nativo no tiene ese objeto: pendiente 2.
- **Colores**: como en la M5 (por mapa y número de paleta). Tres números cambian de colores durante la etapa (22 y 16
  en la fábrica, 35 en la arena final: se toma el primero visto / el más frecuente; los cambios son de actores, fase
  2). Los números que ningún bloque visto usa salen de `palette293400/294000/295200/300000/310500/322500.bin`.

## Paquete `romfs/mission6.pack`

`python3 tools/build_packs.py 6` (receta en `RECIPES`: `s1`, `s2`, `s3`, `--rom-layers`; versión 5, trozos LZ4): 3
etapas, 784 páginas, 98 trozos, 68 paletas, 11 894 imágenes, 16 824 celdas (8569 + 7313 + 942), **1 203 392 bytes**;
sin sección de actores todavía. En ares: etapa 2, tablas 205 KB en 13 ranuras, 21 trozos, carga en 0,27 s.

Reglas de capa (`rom_native_rules`): `10C72E` 1/1 en las tres etapas; en la etapa 1 también `10D7B6` y `10E8C6`
(factores 0x100/0x100); `10E83E` → regla «nunca»: el runtime la dibuja en el `+14/+18` de su objeto.

## Motor (cambios, mínimos)

- `neo_camera.h`: `neo_camera_place_x/y` y `root_slip_x/y`. El actor `04445A` del último enemigo lleva la cámara a un
  punto: en la actualización que lo pasaría guarda el punto en `698E`/`6992` (ROM `044488`/`0444C0`) y pone a 0
  `695C`/`6960`; el objeto raíz `10E968` solo se mueve con la velocidad de la actualización, así que no sigue ese
  salto (MAME: `y` 1 024 000 → 1 048 576 con `root_y` quieto). Antes la raíz se derivaba siempre de `x`/`y`.
- `actor.c`: la página de x de la carga actual de `10E83E`. `gameplay.c`: tres filas de `draw_windows`, y con mapa en
  `10E8C6` la capa 2 del modelo es ese objeto. `game_audio.c`: `0x29`. `main.c`: `MISSION 6 STAGE 1/2/3`.
- `export_stage_rom.py`, `pack_mission.py`, `build_packs.py`, `compile_soldier_ai_tables.py` (desplazamientos del
  guion de la M6), `data/region_rom.json` (los tres registros de entrada `2D6128`, `2D6224`, `2D6278`).
- Sin cambios en `neo_camera.c`, `neo_train.c`, `neo_collision.c` ni en el formato del paquete.

## Pruebas (`make check`)

| Prueba | Resultado |
| --- | --- |
| `test_m6_map` (réplica de `run4`) | etapa 1: 94 539 lecturas del núcleo, **9525 de 9525 celdas distintas iguales** en `neo_collision_cell` sobre la rejilla del paquete; etapa 2: 117 304 lecturas, **10 286 de 10 286** (385 en celdas de los 4 sellos `0C88B0`, 4 parches `0C70FC` sin lecturas en sus celdas); etapa 3: 67 472 lecturas, **2157 de 2157** (530 por el anillo): 0 sin explicar. Nota contada: el segundo terreno `10D7B6` (arriba) |
| `test_m6_map` (TAS, sin escrituras) | **5135 de 5135**, **8668 de 8668** y **1192 de 1192** celdas iguales (28 415, 53 503 y 16 720 lecturas) |
| `test_m6_camera` × 3 (réplica de `run4`) | **5037**, **8722** y **17 837** actualizaciones de cámara iguales en lazo cerrado (292736..304198, 304347..322266, 322414..358818: raíz, progreso, límites, vista, órdenes) y 15 512, 34 888 y 53 555 estados de capa iguales; en la etapa 3, 9 escrituras de actor `0C7B48` con 4 llegadas de `04445A` y 6 de temblor aplicadas como en MAME |
| `test_m6_camera` × 3 (TAS) | **1519**, **3247** y **2875** actualizaciones iguales (40722..44387, 44540..51068, 51216..57157) y 4869, 12 988 y 8669 estados de capa |

## Recorrido en ares (`tools/test_mission6_ares.py`, cartucho normal)

`mslugx-explorer.z64`, menú de pruebas → `MISSION 6 STAGE n` (`Xvfb :76` propio; datos de ares en una copia de
`data146` con la tecla L puesta como en la de la M5):

- **Etapa 2: `RESULT: PASS`**. Carga `016E6E`/`0BDCE2`, `MSX AUDIO stage music 0027`, y Marco recorre la fábrica
  entera, cámara 0 → 4272 (el límite de la lista), con el pasillo repetido; 0 caídas, 0 órdenes de cámara sin
  soporte. Fotos contra `run4` a la misma cámara (1346, 3648, 4272): el mismo mapa y el mismo fondo.
- **Etapa 3: `RESULT: PASS`**. `016E7E`/`0BE99E`, música `002B`, cámara 0 → 320 (toda la arena); mismo mapa y cielo.
- **Etapa 1: el mapa, los fondos y la música están bien, pero Marco no puede recorrerla**: `016E5E`/`0BCA00`,
  `MSX AUDIO stage music 0028`, descenso en paracaídas (`2D6128`, clase 2), y a partir de x 160 **cae al agua**: las
  balsas y el puente colgante son actores sin portar (y el segundo terreno `10D7B6`). Con `--continues` (Start en cada
  CONTINUE?) llega a la cámara 1888 con 82 caídas y 27 continuaciones, donde la lista espera al jugador en un
  rectángulo (op 4) al que no llega. Fotos contra `run4` a la misma cámara (448, 1024, 1409, 1888): la costa, el muro,
  el bosque de `0BC728` (segunda carga de `10E83E`, en su página) y la torre coinciden. No se ha puesto ninguna ayuda.

Paradas contadas: el guion se detiene en su primer registro sin portar (`MSX script stopped at 2e03de` en la etapa 2,
`2e19f8` → `04153E` en la 3); dibujos sin descriptor `1C3F06`, `1875FA..1876D6` (paracaídas y entrada).

## Sonido y música

*Latch* `320000`: etapa 9: `06`, `00 28` en 292684..292686; etapa 10: `00 27` en 304272; etapa 11: `00 2B` en 322340,
fundido `0A 60` en 324391 y `00 2C` en 324635 (`0120F8` desde `0415CC`), fundido `0A 40` en 347135 y **`00 29`** en
348850 (`0120F8` desde `0417B8`, `042100`). **La M6 no tiene palabra de etapa propia**: `0x28` es la del jefe de la M3
(`music_mission3_boss`), `0x27` la del jefe de la M2, `0x2B` la de la alcantarilla de la M5 y `0x2C` la del jefe de la
M4: todas ya renderizadas. Nueva: `0x29` → `music_mission6_boss` (`render_sound_commands.py --labels`, 24 000
fotogramas, `pack_game_audio.py --source`): periodo 87,84 s desde 4,99 s (similitud 0,988, empalme 0,920; los
candidatos de 263,5 y 175,7 s son sus múltiplos ×3 y ×2), 189 100 + 2 372 104 bytes; en `ws-logo/filesystem/audio/`
con su entrada añadida a `manifest.json`. Las músicas del final (`0x2F`, `0x2E`, `0x31`) no están renderizadas.

Peticiones `0120F8`: 38 palabras / 922 peticiones (etapa 1), 37 / 1109 (etapa 2), 39 / 2608 (etapa 3). Fuera de
`data/sound_map.json`: etapa 1 `1036`×3, `1051`, `1052`, `1053`×7, `10AA`×2, `1136` (voz), `113C`×3, `113D`×2,
`113F`×2, `1140`, `1161`; etapa 2 `1036`×4, `10A8`×4, `10D1`×264, `116B`×31; etapa 3 `104B`×2, `10C9`, `10CC`,
`10CD`×13, `10CE`×13, `10CF`, `10D0`×29, `10D1`×372, `10D2`×49, `1141`×2, `116B`×2. Sin renderizar.

## Final y créditos (TAS, `ending/out/debug.log`; fotogramas del TAS)

| Fotograma | Qué pasa | Rutinas |
| --- | --- | --- |
| 57035..57038 | muere el último enemigo: fundido `0A A0`, fin de etapa `018F92` | `0443BE` (de `04180A`), `04445A`, `0AE1DC` |
| 57163..60000 | la huida (registro de etapa `017624`): música `0x2F` en 58698 | `04677A` → `046858`×9, `0469E8`, `046DCE`×5, `046B90`; efectos `01D764`, `01C706`, `01C71E`, `01CE08..01CE44` |
| 60038..60096 | fin de misión `018870` | la cuenta `0308C0` (`neo_tally.c`), `030B0A`, `030FB2`×6 |
| 60097..60505 | fundido `0A 80`, `00 20` (MISSION COMPLETE) | `06090A` → `060A72`×21 (`neo_end.c`) |
| 60920..62172 | la escena final: `03`, `06`, música `0x2E` | `0570A0` (de `018916`) → `0577E2`, `05785A`×2, `057862`×2, `0576B6`, `05764A`, `05731C`×2, `05752E`, `05812A` |
| 62172..72300 | **créditos**: sección 22 del guion (`2E57D4..2E598A`, `data/ending_script.json`), `03`, `06`, música `0x31` en 62770 | `0A8C62`×2, `0A4478`×54, `0A4408`, `0A454C`×43 (el desfile); **doce páginas de créditos cada 730 fotogramas** desde 62899: `0646FE`, `064726`, `06474E`, `064776`, `06479E`, `0647C6`, `0647EE`, `064816`, `06483E`, `064866`, `06488E`, `0648B6`, con `064AC0`×72 (las líneas) |
| 72301..72600 | fundido `0A A0` | `0A453A`, `01959C` (de `018B38`) |
| 72801..73037 | el logotipo de SNK | `057E00` (de `016D3C`), `0580A2` |
| 73037..73677 | la pantalla siguiente | `0A7FFE` (de `01674A`) → `0A81A4`×10, `0A83D6`×216, `0A8602` |
| 73677.. | sección 20: la demostración | (fuera de la misión) |

Ninguna de estas rutinas está portada salvo la cuenta y las letras de MISSION COMPLETE. El registro de etapa
`017624` (descriptor `01708E`, lista `0BEC28`) de la huida no se ha decodificado todavía.

## Verjas de la alcantarilla de la M5 (pendiente 2 de [MISION5.md](MISION5.md)): lo trazado

Captura `m5gate/out/debug.log` (réplica de `run4` desde `sec08_274387` con la sonda nueva en `0C8A3C`): las tres
escrituras de 126 celdas son del actor **`0BC42C`** (retorno `0BC496`), plantilla `0BBBE4`, sobre `10C72E`, con D0 =
0x64, 0xA4 y 0xE4 (las columnas de bloques 100, 164 y 228: las tres paredes `1F`) y D1 = 6, en 275049, 275434 y
275893. `0BC42C` (desensamblado): cada impacto (`+5F` bit 7) suma 1 a `+7A`, vuelve a registrar el cuerpo `2D3F80` con
`0260C8` y pide el sonido `1027`; al quinto (`0BC48E`) estampa la plantilla `0BBBE4` con `0BC38E` → `0C8A3C` (las 126
celdas a 0), suelta los restos `0BBD8A` (`01DA40`), pide `102D`, llama a `019DFC` y a `0BC550` y se borra. Falta:
traducirlo con su constructor, el estampador `0C8A3C` nativo y el paso del barrido del jugador por las celdas `1F`
(prueba bit a bit con una familia desde `at_277000` o `sec08_274387`); no se ha portado ni ajustado nada.

## Puertas (10-XI)

- `make -j4`: RC 0. `make check`: **225 PASS**, RC 0, con `ccr` (fase 2 de la M4, `f798dc1`) fusionado; antes de
  fusionar, 220 (los 202 de la M5 y 18 de la M6).
- `tools/test_m1m2_ares.py` sobre un `mslugx-m1end.z64` recién hecho (`rm -rf build/m1end`): `RESULT: PASS` (208
  fotos).
- ares, `mslugx-explorer.z64`, menú → `MISSION 6 STAGE n` (`tools/test_mission6_ares.py`): **etapas 2 y 3 PASS**
  (Marco recorre el mapa entero con sus fondos y su música); **etapa 1 no**: mapa, fondos, música y entrada bien, pero
  Marco cae al agua desde x 160 (balsas, puente y segundo terreno sin portar; arriba, «Recorrido en ares»). Es la
  parada explícita más visible de la fase 1; no se ha tapado.

**ROM 50 (11-XI, cartucho `aeef1df`, 61 472 768 B): la etapa 3 «FAIL» no era una regresión.** La cámara de la
arena `016E7E` no pasa de x 320 ni en MAME (máximo de `data/m6s3_camera_fixture.json.gz` y de `m6tas3`: 320), y la
prueba se lanzó con el `--progress` por defecto, 600, inalcanzable allí; en ROM 47 la cámara también se paró en 320
(`/home/user/captures-m6/ares/s3`). Los dibujos ausentes `1C3F06` y `1875FA..1876D6` con `palette_id=125` son los de
ROM 47 (la M6 no tiene `rest-native.json` ni grupo de actores: la paleta lógica `0x125` no está en su paquete; es la
fase 2, no se tapa): en la etapa 3 el mismo conjunto de etiquetas; en la 1, las de ROM 47 más `1580E8` y `158100`
(la misma familia `158xxx` que `158166..1581C2`, vistas porque el recorrido difiere). Ahora `test_mission6_ares.py`
toma por defecto `min(600, x máxima de la cámara en MAME)` y rechaza un `--progress` por encima. Con `r50.z64`:
etapa 3 PASS (320), etapa 2 PASS (2176 en 90 s), etapa 1 FAIL como en ROM 47 (514, 3 caídas al agua).

`/home/user/ws-logo` tiene `romfs/mission6.pack` (`build_packs.py 6`), `filesystem/mission6/` y
`filesystem/audio/music_mission6_boss*.wav64` con su entrada en `manifest.json`; no se ha tocado nada del otro agente.

**Para seguir** (otra sesión): árbol `/home/user/ws-m6` (rama `work-m6`), `romfs/` propio, `filesystem/` de enlaces a
`ws-logo/filesystem/*` (con la fase 2 de la M4 fusionada, `mission4` vuelve a ser un enlace); estados MAME en
`/home/user/captures-m6/states`; Xvfb propio (`:76` aquí); datos de ares en una copia de `data146` con
`L-Bumper: 0x1/0/38;;` en `settings.bml` (sin esa tecla el menú de pruebas no se abre: «select chosen early»).

## Fase 2 (agente C, 11-XI): las balsas, los dibujos y el puente

**Causa del agua (etapa 1).** Entre la playa (suelo de `10C72E` hasta x 176) y el muelle no hay suelo en el mapa:
el jugador pisa tres **balsas**, actores de la llamada de cabeza **`0BD71E`** de la lista `0BCA00`, que no estaba
portada (desde la ROM 47; no era regresión). Desensamblado (`neo_m6props.c` lleva el resumen):

- `0BD71E`: las balsas `0BD822` / `0BD836` / `0BD84A` en (010A / 0270 / 02F0, 0020) de la raíz `10E968` con `+74` =
  0 / 4 / 8 y tres `0BDA4E` (su dibujo, paleta 509, programa `2A954E`) en los mismos puntos.
- La balsa (sin dibujo) crea su flotador `0BD8F4` / `0BD906` / `0BD918` (`0040BC`, D0 = 1C) y en `0BD898` cuenta los
  jugadores dentro de `0BDA46` (`01B332`: (−40, 0, 80, 30)) y lleva `+2C` hacia `0BDA0C[+72]` (0020, 001C, 0018: se
  hunde un píxel por actualización con cada jugador). Se borra con `10E99E` ≥ 0340.
- El flotador (paletas 4CB / 4CC / 4CD, programa `2A953E`) se mece con la tabla `0BDA12` (actualizaciones, dy) desde
  la fase `+74`, se pone en (0, dy) de la balsa (`01AD6E`) y registra **el mapa de celdas `0BDA32`** (`0140C0`: 9 × 1
  celdas `0B` a (−5, +4) celdas): ése es el suelo sobre el agua (MAME: ranuras 5..7 de `10C06E`, caja 318..334).
- Decoraciones de la misma lista: `0BD2FA` (`0BD416`, `0BD444`, `0BD472`), `0BD386` (`0BD4A6`), `0BD3B6` / `0BD3E6`
  (`0BD4D4`) y `0BDA8E` (`0BDAB8`, rango `2D3FEE` en `+4E`, sin registro). Comparten `0BD508` / `0BD50E`.

Captura **`captures-m6/fam/R`** (réplica de `run4` desde `sec09_292683`, `MSX_FULL_FAMILY_ROOTS` con balsas,
decoraciones y los dueños de `10D7B6`, 292684..297000; guion `fam/runR.sh`). Familia **`m6raft`** (`--range
bd416-bd535 --range bd822-bdaeb --ram 1063d8-1063da --ram 10c33c-10c33e`): 16 nodos, 7528 actualizaciones en lazo
cerrado iguales a MAME. Sprites (`export_rest_sprites.py ... fam/R@bd416-bdaeb --keep filesystem/rest-native.json
--keep filesystem/mission5/rest-native.json --output filesystem/mission6/rest-native.json`): 63 volcados, 316 761
píxeles iguales, 0 distintos; las nueve paletas nuevas salen de los volcados de `R`.

**La paleta `0x125` (dibujos ausentes de las tres etapas).** La M6 no tenía `rest-native.json` ni grupos de programas
y tablas: los actores ya nativos que comparte con las misiones anteriores (`1875FA..1876D6`, `158xxx`, `1323xx`,
`1C3F06`, paleta 125) no tenían sus descriptores en el paquete, y sus rutinas paraban en tablas de los grupos m3 / m4
(`2CFD00`, `2D3A0E`, `0DFF0A`...). Ahora `filesystem/mission6/rest-native.json` (11 848 descriptores, como las
exportaciones anteriores: guarda los de las misiones 2..5) y `GROUPS[6] = ('m3', 'm4', 'm5')` (presta la M3). Paquete:
1 790 208 bytes (1195 trozos, 1031 tomados de los paquetes 2..5): `SCENE_MISSION_CHUNKS_MAX` 1088 → 1200. En ares
(ROM con todo): **0 dibujos ausentes** en las tres etapas; caché de escena 208 ranuras (el suelo); tablas de etapa
788 / 675 / 530 KB (50 / 43 / 34 ranuras). Los programas nuevos van sin grupo (`/programs.bin`) y las dos tablas nuevas
de `region_rom.json` (`0BDA0C`, `2D3FEE`, 0xD4 bytes) en la imagen.

**ares** (`test_mission6_ares.py --stage 1 --jump 1.0`): Marco salta a las balsas y llega a la cámara 1056 (PASS,
600), donde lo matan los soldados (sin disparo). Sin saltar se cae entre la playa y la primera balsa, como en el
original: la matriz corre `m6s1` con `--jump 1.0 --continues`.

**Puertas (11-XI, tras fusionar ccr `401875e`)**: `make check` RC 0 (familia `m6raft` incluida); matriz de regresión
sobre el cartucho completo (62 111 744 B, paquetes rehechos) y `m1end` recién hecho: todo PASS, línea base
actualizada (`m6s1` con `--jump 1.0 --continues`, `m6s2` con `--fire --jump 2.5 --continues`).

**Siguiente: el puente colgante (`10D7B6`)**, de la cámara 1056 en adelante (MAME: 3170 lecturas no vacías de su anillo
`10D83E` en 294407..296719, cámara 1265..). Es un segundo terreno que **se mueve solo**, como el tren de la M3:

- `0BCE2C` (cabeza de lista) = el conductor `0B8D64` del tren: `10E94E` = paso de la raíz con `10C70A` (modo 1: −2 px
  con `10C706` −0x4000 por actualización; modo 2: ¾ de la raíz y −8 px), `0BCE18` / `0BCE22` (llamada en 1056) ponen el
  modo; `0BCEB0` = el bucle `0B8DD0`: con `10C704` ≠ 0, la columna de la tabla `0BCE96` (`10C702`: pares (columna,
  cuenta) 000C/FFFF, 000C/0000, 001C/FFFF, 0010/FF00, 002C/FFFF, 0010/FFFF) más el origen `+32` entra por la derecha
  → `0C6F82` adelanta el origen; `0BCF36` / `0BCF86` / `0BCF8C` / `0BCF94` eligen la entrada.
- `0BD18E`: el balanceo `10C712..10C714` de los dibujos según lo que avanza `10D7B6` cada actualización.
- `0BD2BA` / `0BD2CE` (`0BD202` / `0BD20E`): cuando la columna `0BD2A8[+6E]` (6, 0x14) más el origen entra por la
  derecha (`0C6F6A`), crean los tramos `0BCFA4` / `0BCFC4` / `0BCFE4` / `0BD004` (programas `2A923C`, `2A92C8`, `2A9354`,
  `2A93E0`, paleta 4E1) en (`+36`+1F)·16, 0x58 de `10D7B6`, con `+6E` = `10C714`; `10C70C` / `10C70D` los bloquean.
- Núcleo: `10D7B6` en la lista estática `013810` como el tren (`NEO_KOBJ_TRAIN`), con el mapa `0BC6E6` (sin teselas;
  colisión `45FC2A`, 60 × 5 bloques; columna 88, fila alta 11) desde `region_rom.json` (como `0B8566` del tren, sin
  el registro de etapa del paquete) y su anillo con el origen de cada carga (las 3066 lecturas «con otro origen»).
  La captura `R` ya tiene a los cuatro dueños (`bce2c`, `bd18e`, `bd2ba`, `bd2ce` y sus hijos): familia y prueba del
  núcleo con las lecturas de `cells.txt` de `s9`.
- Comprobado fuera del árbol (lecturas de `R` como entrada, escrituras comparadas): la traducción de `0BCE2C` da
  `10C706` y `10E94E` iguales en las 1650 actualizaciones (modo 0: `10C706` se queda; con D1 ≤ D3 no se guarda), y la
  de `0BCEB0` los 12 saltos del origen (`+32` += `0BCE96[e+4]` + ((`+36`+1F) − (columna + `+32`)), en 12 bits;
  `10C704` − 1 salvo FFFF; `10C70C` &= `0BCE96[e+6]`). Tabla `0BCE96`: entradas de 8 bytes (columna, cuenta, avance,
  máscara): (000C, FFFF, 000C, 0000), (001C, FFFF, 0010, FF00), (002C, FFFF, 0010, FFFF), FFFF.
- Celdas: la fila de celda del mapa es `r` = `q` − 64 + 2·(`+34` + 1) con `q` = ((~y) & 1F8) >> 3 (en el tren de la M3,
  `+34` = 19: `q` − 24) y la columna la del anillo cargada con el origen de su momento (`neo_train_t.ring`); el mapa
  `0BC6E6` tiene `+C` = 0 (sin la poda vertical de `01384E`). La colisión está en `45FC2A` (P2 lineal: 60 × 5 bloques ×
  4 bytes, 1200 bytes) y puede ir a `region_rom.json` como el tren.
- **El jugador de `run4` nunca pisa `10D7B6`** (captura `fam/P`, réplica desde `sec09_292683` con una sonda de
  escritura en su padre `+38`, `1014FC`, hasta 297000): sus padres son la raíz `10E968` (en el aire), `10C72E` y los
  mapas dinámicos `10C0AE..10C22E` (las balsas hasta la cámara 943 y otros dos en 1050..1072); ninguna escritura de
  `10D7B6`. Las lecturas no vacías de su anillo son de barridos que no aterrizan (y de los soldados). El puente no
  bloquea el camino de `run4`: es la colisión y los dibujos del puente para los demás actores (fase 2, actores).

### Puente: el conductor nativo (12-XI)

- `src/neo_m6bridge.c`: `0BCDC2` (cabeza de la lista: `10C70C` = 0, modo 0, `10C706` = 0; los actores `0BD2CE`,
  `0BD2BA`, `0BD18E` que crea siguen sin portar), `0BCE18` / `0BCE22` (modo), `0BCF36` / `0BCF46` / `0BCF86` /
  `0BCF8C` / `0BCF94` (entrada de `0BCE96`; `0BCF46` más allá del final es el `trap #F`), `0BCE2C` (modo 0 conserva
  `10C706`; modo 1 −0x4000 hasta −2 px; modo 2 ¾ de la raíz y −0x2000 hasta −8 px) y `0BCEB0`.
- Prueba bit a bit `tools/test_m6_bridge.c` (en `make check`) con `data/m6bridge_fixture.json.gz`
  (`tools/extract_m6bridge_fixture.py` sobre `captures-m6/fam/R`): las 1650 actualizaciones de `0BCE2C` escriben lo
  mismo que MAME (`10C706`, `10E94E`, `+32`/`+34`, `10C704`, `10C70C`; 12 saltos del origen).
- Mundo: `neo_world_camera` pasa las llamadas al conductor y, tras el paso de cámara, calcula `10E94E` y el bucle
  sobre `layers[NEO_LAYER_MID]` (`+32` = `column`, `+34` = `top`, `+36` de su x). `0C81C0` (`neo_layers_step_own`)
  mueve la x de una capa con bandera exactamente 1 por `10E94E` y sus factores (`0C826C`); con otras banderas sigue
  quieta como antes. m6s1 / m6s2 en ares: PASS, sin cambios en los recuentos.
- **Orden comprobado (captura `captures-m6/fam/O`, `runO.sh`)**: réplica de `run4` desde `sec09_292683` con
  `MSX_FULL_ORDER_TAP` (nuevo en `capture_fullgame.lua`: cada lectura y escritura de `10D7B6` +0 / +14 / +32..+37,
  `10E94E`, `10C712..10C714` y el último almacenamiento de la cámara `10E9D8`, por orden de CPU y con PC) y
  `MSX_FULL_CAMERA`. En las 1650 actualizaciones de `0BCE2C` el orden es: cámara (`0C7EB6`) → `0C81C0` lee `10E94E`
  (`0C826C`, el que escribió el conductor en la actualización anterior; 1231 veces, el resto con bandera 0 toma el
  paso de la raíz) → `01AC38` escribe +0 (`01AC40`) → `01ABC8` copia +14 (`01AC08`) → `0C748E` rehace +36 (`0C74D8`)
  → `0BD1B8` lee +0 → `0BCE2C` escribe `10E94E` → `0BCEB0` lee +36 → `0BD20E` lee +32/+36 → los tramos leen `10C714`.
  Una actualización puede ocupar dos fotogramas de vídeo (292941/292942): el extractor agrupa desde cada
  almacenamiento de la cámara. Prueba `tools/test_m6_order.c` (en `make check`, `data/m6order_fixture.json.gz` de
  `tools/extract_m6order_fixture.py`): el orden, `neo_layers_step_own` con el `10E94E` anterior (`w->bridge_dx`) da el
  +0 de MAME y la fórmula de +36 del mundo da el +36 que leyó el conductor: **1650 de 1650 iguales**.
- **Actores del puente nativos** (`src/neo_m6props.c`): `0BD18E` / `0BD1B8` (balanceo `10C712..10C714`, x anterior
  `10C70E`), `0BD2BA` / `0BD2CE` → `0BD202` / `0BD20E` (columna `0BD2A8[+6E]` = 6 / 14 más el origen +32; con
  d3 = (+36 + 1F − d4) << 4 ≥ 0, el byte de bloqueo `10C70C` / `10C70D` a 0 y d3 ≠ +70 llama a `0BD09A` / `0BD114` y
  pone el bloqueo a 1), los tramos `0BCFA4` / `0BCFC4` / `0BCFE4` / `0BD004` (+66 = +1E = FFF0 / 0, programas
  `2A923C` / `2A92C8` / `2A9354` / `2A93E0`, paleta 4E1) y `0BD044` (byte +6E = `10C714`, `01ABC8`, `01945C` con
  `0BCF9C`, `00427A`, borrado con `10E99E` ≥ 08B0 o +28 < 04F00000). Entorno `neo_m6bridge_env_t` (`10D7B6` +0/+32/+36
  y RAM `10C70C..10C714`). Tablas `0BCF9C` y `0BD2A8..0BD2B9` en `region_rom.json`; programas en
  `extract_soldier_programs.py` (sin grupo).
- Familia **`m6span`** (`captures-m6/fam/R`, `--range bcfa4-bd2e1 --ram 10d7b6-10d7ee --ram 10c70c-10c715`, opción
  nueva `--m6bridge` del compilador): **31 nodos, 12 784 actualizaciones en lazo cerrado iguales a MAME**, 28 borrados,
  7806 dibujos.
- Sprites: `export_rest_sprites.py mslugx.zip captures-m6/fam/R@bd416-bdaeb captures-m6/fam/R@bcfa4-bd2e1 --keep
  filesystem/rest-native.json --keep <la exportación M6 anterior> --output filesystem/mission6/rest-native.json`: 146
  volcados, 769 266 píxeles iguales, 0 distintos; 11 860 descriptores (los 12 de los tramos nuevos) y la paleta 4E1 de
  los volcados. Paquete: 1200 trozos (el máximo). Con `--keep filesystem/mission5/rest-native.json` entraban además
  107 descriptores de la alcantarilla de la M5 (1213 trozos, +2,4 KB de cabecera) y la caché de escena bajaba de su
  suelo (`RAM budget: 3330 KB left ... (minimum 3332 KB)`, ares se para en `scene_assets.c:498`): no se conservan.
- Memoria: aun así el código nuevo (actores, mundo) dejaba 3330 KB para la caché (suelo 3332 KB). `scene_assets.c`
  guarda ahora fijado / mapa / en cola de precarga de cada trozo como bits de un byte (`chunk_bits`) en vez de tres
  `bool[CHUNKS_ALL]`: 4,4 KB menos de bss, sin cambio de lógica.
- Mundo: `0BCDC2` crea en la misma actualización (antes del bucle de actores, tras el actor de guion; en MAME tras el
  actor de misión `101364`, que corre antes) `0BD2CE`, `0BD2BA` y `0BD18E`: la lista queda `0BD18E`, `0BD2BA`,
  `0BD2CE` y el conductor nativo corre antes (con la cámara); ninguno lee lo que escribe el otro (comprobado con `O`).
  `neo_world_parent(10D7B6)` con el puente activo da la capa media (x/y y la posición previa al paso, `01ABC8`).
### Puente: el objeto de núcleo `10D7B6` (12-XI)

- **Lista estática `013810`** (`01382A`): `10C72E`, luego `10D7B6` si su `+2E` tiene mapa con `+4` ≠ 0; con `+C` = 0
  (`0BC6E6`) `01384E` lo devuelve sin la prueba vertical (la del tren usa `+10`/`+14`). Solo en la etapa 1: `s10` / `s11`
  no tienen ninguna lectura de `10D7B6`.
- **Mapa `0BC6E6`**: `+0` = FFFFFFFF (sin teselas: `0C714E` va a `0C7272`), `+4` = `45FC2A` (P2 lineal, banco 3), 60 × 5
  bloques, `+C` = 0; 56 celdas no vacías (formas 81..8A, grupo 4). Ambos en `region_rom.json` con grupo nuevo **`m6`**
  (los sirve el `MSXD` del paquete de la M6, no la RDRAM; también `0BDA0C`, `2D3FEE`, `0BCF9C`, `0BD2A8`).
- **Anillo** (`neo_m6ring_*` en `src/neo_m6bridge.c`): `0C7038` lo borra al empezar la etapa; `0C741A` (op 0x18) carga la
  ventana; `0C74C4` (desde `0C748E`, en cada pasada de cámara) carga las columnas que entran (`+3A` − `+36`) y, con el bit
  2 de `+82` (siempre: `0C6CD4` pone `+82` = 6), las filas que entran (`+3C` − `+38`), por `0C714E` (recorte al mapa) y
  `0C72EC` (bloque → (2·s, q), (2·s+1, q), (2·s, q+1), (2·s+1, q+1), q = 2·(~d5 & 31) + 2k). **`0C6F82`** (el avance del
  origen de `0BCEB0`) salta a `0C748E` en la misma actualización: `+3A`/`+3C` aún son los de la pasada anterior (`0C7480`
  los copia en la siguiente), así que lo que entró se carga **otra vez con el origen nuevo** (los 7 fotogramas de avance
  de 16 columnas lo muestran). Por ranura y fila de bloque del mapa: la columna del mapa escrita y su fila en el anillo.
- Núcleo: `neo_kscene_t.bridge_ring` / `bridge_map`; `10D7B6` sigue siendo `NEO_KOBJ_TRAIN` (posición: `+14/+18` de la capa
  media y la previa al paso). Mundo: borrado y carga en la cabeza, pasada tras los comandos de cámara, segunda pasada si
  el bucle movió el origen, `0C7480` al final.
- Captura **`captures-m6/fam/L`** (`runL.sh`: réplica de `run4` desde `sec09_292683` hasta 304270, `layers.csv` de cada
  fotograma) con `captures-m6/s9` (escrituras del anillo y `cells.txt`): `tools/extract_m6ring_fixture.py` →
  `data/m6ring_fixture.json.gz`. Etiquetas: `capture_fullgame.lua` cuenta el fotograma en `frame_done`, así que lo que
  un tap ve en el fotograma N es el estado que la instantánea etiqueta N + 1. Prueba **`tools/test_m6_ring.c`** (en
  `make check`): **671 de 671 fotogramas de carga, 14 240 escrituras del anillo iguales a MAME en orden, y 277 002 de
  277 002 comprobaciones de lectura del núcleo (223 167 lecturas) iguales**.
- Memoria: con los blobs en la RDRAM la caché quedaba en 3331 KB (suelo 3332); con el grupo `m6`, 208 ranuras (3328 KB).
  Paquete de la M6: 1200 trozos, `MSXD` 86 blobs.
- **Siguiente**: jefes de la M6 y final (abajo).

## Jefe intermedio `05310E` (12-XI, agente C)

- Captura **`captures-m6/fam/B`** (`runB.sh`: réplica de `run4` desde `sec09_292683`, raíz de familia `5310e` desde
  296800, volcados desde 296850, hasta 303000). Nace en 296857 (guion, cámara 2352), muere en 301809 y se hunde en el agua
  hasta 302369.
- Traducción en `src/neo_m6props.c` (`mid_*`), rutina a rutina: `05310E` (vida rango `2CFF8A`, umbral `+76`/`+78` = vida/5,
  `+54` = `2D01BA`, paleta 38E, las cuatro partes `0539E0` / `0539EE` / `0539C4` / `0539D2`, lado `+8A` por `+28` < 08C0),
  la entrada `0531EC` / `053214` / `053254` / `053280`, el bucle de decisión `0532F6` (en su orden: sin blanco `0534A8`, de
  espaldas `0533A2`, cuerpo a cuerpo `052C78` → `05345E`, agarre `052CDA` → `05344A`, patada `052E06` → `053698`, disparo
  `052D82` → `0533EE` (voz `052B56`), borde `052D0A` → `0533E2`, saltos `052D36` → `0535AE` y `052E88` → `053658`, partes
  `052ED4` → `0533E2`, en el aire → `053520`), los saltos (`053520` / `05358A` / `0535AE` / `053658`, `05362A`), la caída
  `052B7A`, el blanco `052BDA` (`01AEDA`, `01AFFC` con `052B3E`, `01AF9A`), la cola común `052F04` (`004C24` o `+92`,
  chispa `040ED0` y tinte `053DCC` al herir, muerte por `+5F` bit 7, recorte `019504` con `052B46` → `052F8A`), la muerte
  `0536F4` (`040F04`, `05390C`, en el aire `0537AE` / `0537D0`), `053748`, el final `0537F4` / `05385C` / `0538AE` /
  `0538C8` (`0C8CD6`, `10C00B` = 0, `10C00C` = FF, `0CA2E0` con `0537E4` ×2, cámara `10E95C` / `10E960`, `0C09DA` con F,
  `+7E` = `10E992`) y `052F8A` (`019DFC`, `0CA2F2` ×2, `0C8CE6`, `0C7B48(0)`, `10C44C` = 0); las llamadas de programa
  `05304A` / `05305C` (polvo `053A48` / `053A5C`), `05306E` / `0530AA` (cuchillo `053CB2`, sonido 10F4), `052E3E` (granada
  `053B38`), `053DBC` (fila `052AE6`), `053E20` (objeto 49 de `08CDB2`); el hundimiento `05439E` (`053F0A` / `053E40`,
  siete `0540EA`, `0542B0`, `0541F2` con `03C5C6`; `012040` es el inverso de `012022`) y la paleta de chapoteo de la etapa
  9 en `019240` (`01932E`: 154 antes de la cámara 0700, 166 después).
- **Familia `m6mid`** (`make check`; `--range 52a00-546ef --attacks` y la RAM de los jugadores `+2C`, el anillo del
  terreno, `10C44C`, `10E992`, `10C2F6`): **56 nodos, 15 554 actualizaciones en lazo cerrado iguales a MAME** (296857..302369),
  55 borrados, 4171 dibujos. Arnés: `camera_push` (`10E95C`), `camera_y` (`10E992`), `10C44C` y el manejador `0537E4`.
- Sprites: `export_rest_sprites.py ... captures-m6/fam/B@52a00-546ef`: 11 975 descriptores (+115), 372 paletas; 849 971
  píxeles iguales, 0 distintos. Programas en `extract_soldier_programs.py` (sin grupo). Paquete de la M6: 1224 trozos
  (`SCENE_MISSION_CHUNKS_MAX` 1200 → 1224). Tablas `052A8E`, `053B30`, `053C9E`, `2CFF8A..2D01E1` en el grupo `m6`.
- Memoria: el código del jefe (~13 KB) dejaba la caché en 3318 KB (suelo 3332). La copia mutable de la rejilla de la M1
  (`dynamic_grid_m1`, 15 KB de bss) sale del montón en su primera escritura (`neo_collision.c`; las misiones de paquete
  pasan la suya): de nuevo 208 ranuras (3328 KB) con la M6 entera.
- **`009CCE` (13-XI)**: la ranura de paleta (1068EA) guarda +A (su recurso), +C (destino) y +E (peso); el vblank `008402`
  la rehace: +A = +C o peso 0 → +A (`005978`), peso ≥ 31 → +C, si no `005EA0` mezcla canal a canal (base +A, byte con
  signo de `0051D8` + 64·peso + (C − A), en palabras de 16 bits del 68000; el canal alto lleva el bit 15) y convierte por
  la tabla `224000`. El tinte de daño `053DCC` es peso 0x20 (la paleta 3BF / 3C0 / 3C1 / 3C2 entera, según `+86` = 7 / E /
  15 / 1C; con otro `+86` D1 sería el del llamador `052F5E`, no visto en `B`: se cuenta como externa 0x9CCE) y el fundido
  `05390C` mezcla 38E hacia 3C2 con `+7C` = 1F..0 en la ranura del jefe (`+C` → su `+14`; ranura privada, bit 0 de +F).
  `tools/palette_mix.py` traduce `005EA0` / `005978`; `--verify` sobre `captures-m6/fam/B` y **`B3`** (`runB2.sh` con
  `DUMP0=301760 LAST=301845 MSX_FULL_PERIOD=1`: el fundido fotograma a fotograma): **300 de 300 ranuras con destino iguales a
  MAME** (170 emparejadas con la RAM de la actualización anterior: el volcado de paleta del fotograma f muestra lo que dejó
  la de f − 1; 3 sin pareja no comparables). Prueba `palette_mix.py --check data/palmix_fixture.json.gz` en `make check`
  (31 estados, sha256 de los colores). Exportación: `palette_mix.py mslugx.zip --into filesystem/mission6/rest-native.json
  --plain 3bf,3c0,3c1,3c2 --mix 38e:3c2` (34 paletas: las cuatro y las 30 mezclas como recursos `4000 | par << 5 | peso`,
  `src/neo_palmix.h`). Nativo: `e->palette_mix` → `neo_world_t.mixes` (dueño + asa `+14`; se libera con el nodo) y la
  emisión de dibujos cambia el recurso (`mix_missing` cuenta un par sin exportar).
- **`0C09DA`** (13-XI): `gameplay.c` lo aplica tras el mundo: con el jugador vivo y su +84 (aquí `mummy_guard`, o la cuenta
  de entrada `invulnerable` / 2) no mayor que D0, +84 = D0 y sin parpadeo; `guard_min` se borra cada actualización.
  `player_guard` (0C09D8, jefes de M2..M4) sigue sin aplicar (no es de esta tarea: +84 = FFFF duraría a la etapa siguiente).
- **Balas** (13-XI): familia **`m6shot`** (`captures-m6/fam/B2`, `runB2.sh` con raíces `5310e,53b38,53cb2` hasta 301700;
  inyectadas tras el actor de guion): **84 nodos, 2171 actualizaciones en lazo cerrado iguales a MAME** (297440..301601),
  84 borrados, 2188 dibujos, 21 contactos con el jugador.
- Memoria: el código nuevo dejaba 3331 KB (suelo 3332); `scene_prerender.c`, `vehicle_renderer.c` y `neo_vehicle.c`
  pasan a `-Os` (`COLD_BOOT_SRCS`, −1,1 KB de texto): ares de nuevo «MSX CACHE slots=208 (3328 KB, floor 208)».
- **Pendiente: ares no llega a la cámara 2352** (13-XI, `test_mission6_ares.py --stage 1 --jump 1.0 --continues --fire
  --progress 2352 --seconds 420`): la cámara se queda en **1888**; el jugador cae a través del puente colgante (x 2000..2100,
  61 muertes por caída, 20 continuaciones). En MAME `run4` cruza ese tramo sobre `10C72E` (nunca sobre `10D7B6`, captura
  `fam/P`): falta comparar el suelo del jugador nativo (rejilla del paquete / anillo de `10C72E`) con las lecturas de `P`
  en x 1888..2352. Sin captura nueva no se toca.
- **Pendiente**: el último enemigo `04153E` (con `04445A`) y el final (créditos), sin empezar en esta sesión.

## Pendiente (fase 2, por orden)

1. **Etapa 1**: hecho (balsas, conductor, actores del puente y el objeto de núcleo `10D7B6` con su anillo; arriba).
2. ~~Segundo objeto de terreno `10D7B6`~~: hecho (anillo del ROM, no rejilla del paquete; las 3066 lecturas «con otro
   origen» son celdas cargadas con el origen de su momento, que el modelo lleva por ranura, `test_m6_ring`). `test_m6_map` sigue contándolas como nota.
3. **Capa `10E83E` movida por su dueño** en el progreso 1056 (los 300 bloques de `0BC728`) y las banderas de capa.
4. **Parches y sellos de la etapa 2**: 4 parches `0C70FC` (submapa `0BDBB2`; 304505, 304596, 304973, 309159) sin
   `patches.json` todavía, y 4 sellos `0C88B0` (313437, 313441, 314099, 314129; plantillas `0A42C4`, `0E1A2C`,
   `0E1A3A`).
5. **Actores**: 134 constructores sin portar (123 propios), 3582 creaciones ([MISION6_ACTORES.md](MISION6_ACTORES.md));
   el jefe intermedio `05310E`, y la familia `04153E` del último enemigo con `04445A` (`neo_camera_place_*`).
6. **Final y créditos** (tabla de arriba) y sus tres músicas; el registro de etapa `017624` (la huida): captura, mapa y paquete.
7. Paletas que cambian (22 y 16 en la fábrica, 35 en la arena) y las 30 palabras de efectos sin política.
8. La tabla de columnas `+3E` sigue sin desensamblar (pendiente 4 de la M4): en la M6 la llevan cinco cargas
   (`0BC780`, `0BC8BE`, `0BC8C4`, `0BC8D2`, `0BE974`); todos los bloques observados caen dentro de sus filas.

## Archivos

`tools/capture_m6.sh`, `capture_m6_tas.sh`, `build_mission6.sh`, `extract_m6_map_fixture.py`,
`compile_m6_map_fixture.py`, `test_m6_map.c`, `test_m6_camera.c`, `test_mission6_ares.py`,
`build_mission6_actors_doc.sh`, `export_stage_rom.py`, `tas_updates.lua`; `src/neo_camera.h`, `actor.c`,
`gameplay.c`, `game_audio.c`, `main.c`; `data/mission6_actors.json`, `m6_map_fixture.json.gz`,
`m6tas_map_fixture.json.gz`, `m6s{1,2,3}_camera_fixture.json.gz`, `m6tas{1,2,3}_camera_fixture.json.gz`,
`sound_map.json`, `region_rom.json`. Privados:
`filesystem/mission6/s{1,2,3}/{map.json,map_observed.json,collision.json,collision_observed.json,rom_report.json}`,
`romfs/mission6.pack`, `filesystem/audio/music_mission6_boss*.wav64`.
