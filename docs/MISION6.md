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

Portado (19-XI): la cuenta, las letras de MISSION COMPLETE, la muerte `04180A..` y la huida `04677A` con todos sus
actores (familias `m6boss6` / `m6boss7`, hasta el fotograma 60000). **Siguiente** (por orden): una ventana desde
`60000` (estado **`end27900`**, actualización 27900, fotograma 59999, en `captures-m6/states/mslugx/`, guardado con
`captures-m6/fam/runP6.sh`; para cargarlo, `MSX_TAS_STATES=/home/user/captures-m6/states MSX_TAS_UPD_LOAD=end27900:27900`) para el cierre del
cuerpo `0418C2` (`018F40`) y el paso a `018870`; el registro `017624` (lista `0BEC28`) en `mission_flow`; la escena
final `0570A0`; los créditos (`0646FE..0648B6`, `064AC0`, el desfile `0A4478` / `0A454C`); el logotipo `057E00` y
`0A7FFE`; la vuelta a la sección 20. El registro de etapa
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
- **Puente colgante en ares (13-XI, hecho)**: el jugador caía a través del puente (x 2000..2100, 61 caídas). En
  MAME (`fam/P`) cada `014492` del jugador en x 1916..2128 escribe `+38` = la raíz `10E968` y su estado alterna
  `0C2692` / `0C1EBE` / `0C2888`: anda **sobre el puente** (las celdas `81..8A` de `0BC6E6`), no sobre `10C72E`.
  Causas: (1) `10C00E` de la lista `0BCA00` es `0BC67C` (op 0x50; ROM `00 01 02 03 09 05 06 07`) y el mundo usaba
  el mapa de la M1: el grupo 4 de los cables quedaba 4 en vez de 9, y `013BC0` (grupo 9 → raíz) no se cumplía;
  (2) el modelo de suelo del jugador (`neo_walk_step`) no ve el puente: aterrizaba por `platform_floor` (013DB2) y
  al paso siguiente caía. Ahora el aterrizaje es el barrido aéreo `0143EE` (`neo_kernel_air_landing`) y, mientras el
  suelo es del núcleo (`kground`), el paso en el suelo es `014594` por el núcleo (`neo_contact_prepare_ground`:
  `+62` = 0x30000, 0x28000 gordo, negado mirando a la izquierda, 0 parado o muriendo; `002E98` por `+6D`). Prueba
  `tools/test_m6_walk.c` (en `make check`; `data/m6walk_fixture.json.gz` de `tools/extract_m6walk_fixture.py` sobre
  `fam/P`): **43 de 43 pasos en el suelo del puente iguales a MAME** (`+28/+2C`, `+38` raíz, bit de suelo), con la
  cabecera del puente del fotograma siguiente (etiquetas de `capture_fullgame.lua`). ares (`--stage 1 --jump 1.0
  --continues --fire --progress 2352`): PASS, cámara 3520, 4 caídas.
- **Corredores `020166` (13-XI, hecho en anfitrión)**: el guion se paraba en `2DFAD4` (`07 0C 05 80 01 90`: el
  corredor `020166`, sin portar; también `01F836`/`01F844`, `0530F4` y `08571C` de la M4 en la etapa), así que la
  sentencia de `05310E` no llegaba y la cámara pasaba de 2352 sin retención. Traducción en `neo_m6props.c`
  (`run_tick`): `02011C` / `020166` (programa `22FD1C` / `22FE22`, +28 ± 0080 / 0040, fila de ataque +82 = `0E003E` /
  `0E001E`, +8C bit 0 en `020166`, vida por rango `2D3578` / `2D35AA`, `01B1DC`, +30 = +6E << 8, paleta 74, `004C1C`,
  el jinete `020372`), `02020C` (`01AC38`, `00427A`, mapa de celdas `0DFF52` / `0DFF68` / `0DFF7E` / `0DFFA0` por +8C y
  +15 con `0140C0`, `0049E8` → `0202B2`, `004C24` con +5F bit 7 → `0202B2`, `01945C` `0DFF12` → `02010E`), `02010E`
  (`013F1E`, `0041AC`), la muerte `0202B2` (`013F1E`, +82 del creador a 0, +66 = +1E = 1001, `009F88`, `004192`, los
  restos `01DA40` `0DFEA6`+`0DFE5E` / `0DFEB8`+`0DFE70`, la explosión `01C71E` con +28 ± 0038 / 0078, sonido 1021,
  `01C706`) y el jinete `020372` / `0203B6` (paleta 75, programa `22FEB0`, `01AD6E`, +15 bit 0 del creador, `01ABC8`,
  `01AD8C`, `00427A`). Familia **`m6run`** (`captures-m6/fam/C`, `NAME=C ROOTS=20166 FROM=294400 DUMP0=294400
  LAST=296200 sh runB2.sh`; `extract_family_fixture.py --range 2010e-2042b --attacks` y la RAM de `m6mid`; traza en
  bruto borrada): **10 nodos, 1338 actualizaciones en lazo cerrado iguales a MAME** (294458..295623), 3 borrados, 1335
  dibujos, 4 hijos borrados por `004192`. Programas en `extract_soldier_programs.py` (sin grupo); sprites
  (`export_rest_sprites.py ... captures-m6/fam/C@2010e-2042b`, raíces y paletas 74 / 75): 35 volcados, 45 156 píxeles
  iguales, 0 distintos; 12 124 descriptores, 406 paletas. Paquete de la M6: 1245 trozos (límite 1256).
- **ares tras los corredores (13-XI)**: `test_mission6_ares.py --stage 1 --jump 1.0 --continues --fire --progress 2600`:
  el guion pasa de `2DFAD4` (8 creaciones de `020166`, `01913C` en la cámara 2352) y se para ahora en `2DFB88`
  (`07 0a 00 80 04 7c`, registro sin portar); la cámara queda retenida en 2352 (x 2240) y `05310E` aún no sale.
  Siguiente: desensamblar la orden de `2DFB88` y lo que crea; después `04153E` (45 constructores, 2639 creaciones en la
  etapa 3: tarea larga) y los créditos.
- **Memoria tras fusionar ccr `250afc9` (13-XI)**: el arranque completo (logo, título, opciones, películas) dejaba 3329 KB
  para la caché (suelo 3332: «RAM budget» en `logo`; `pause`, que salta películas, sí llegaba a 208). La fuente de
  depuración del panel de diagnóstico (`FONT_BUILTIN_DEBUG_MONO` y `rdpq_text`) solo se usa con
  `msx_reference_invulnerable`: ahora la carga `msx_debug_text` de `gameplay.c` solo en los cartuchos de referencia
  (`MSX_REFERENCE_INVULNERABLE`), y el de juego no la enlaza (texto −17 KB, datos −3,3 KB): ares «MSX CACHE slots=209
  (3344 KB, floor 208)». Probado y descartado: las tablas CRC de las películas en el montón (ares: RSP CRASH en el logo).
- **Orden `2DFB88` (13-XI, agente C)**: `07 0a 00 80 04 7c` es una creación de grupo 0 (contada) con la entrada `0x47C`
  de la tabla `0197DC` = **`0530F4`**, que no era nativa (por eso el guion se paraba). `0530F4`: `st.b 444C(A5)` (10C44C =
  FF), +0 = `0530FE`; `0530FE`: con 10C44C distinto de 0 `rts`, a 0 `jmp 004188`. Es la retención del jefe: su `052F8A` pone
  10C44C a 0 al final del hundimiento y el nodo se libera (grupo 0 a 0). En `captures-m6/fam/B`: nodo 104594 con `0530FE`
  y 10C44C = FF en todos los volcados de 296858 a 302340; en 302370 10C44C = 0 y el nodo libre (`003F1E`); el grupo 0 del
  guion pasa de 1 a 0 en 302372. Traducción en `neo_m6props.c` (`m6props_tick`, `e->flags->m6_signal`). Tras ella el guion
  sigue: `2DFB92` (región 4330), `2DFB9C` / `2DFBAA` (`05310E` por el lado del jugador), espera de 120, corredores
  `0AF17E` y `2DFC60` (grupo 7 < 1) → `2DFC66` desbloqueo. Familia **`m6hold`** (`captures-m6/fam/H`, `runH.sh`: réplica
  de `run4` desde `sec09_292683`, raíz `530f4` de 296800 a 302420; `extract_family_fixture.py --range 530f4-5310d --ram
  10c44c-10c44d`; traza en bruto borrada): **1 nodo, 2716 actualizaciones en lazo cerrado iguales a MAME** (296857..302369),
  1 borrado, 2716 bytes de 10C44C comparados. El extractor guarda ahora como borrado la última actualización de la traza
  si el `004126` ya limpió su nodo (+00 escrito en 004136) aunque no venga otra `U` detrás (antes se descartaba como
  cortada).
- **ares (13-XI)**: `test_mission6_ares.py --stage 1 --jump 1.0 --turn 6 --continues --fire --progress 2600 --seconds 1500`
  (`--turn P`, nuevo: izquierda y abajo 0,5 s cada P s; sin él el jugador deja atrás los morteros `08A732` de grupo 0 tras
  los sacos y la retención `2DF9B8` de 1056 no se suelta; elección del jugador, nada del juego cambia): `2DFB88` → `0530F4`
  nativo, `2DFBAA` → `05310E` en la cámara 2352, **el jefe derrotado** (`2DFC68`, `2DFC7C` en 2353) y la cámara sigue
  hasta 3520: `RESULT: PASS` (2 continuaciones, 5 caídas). Sin `MSX script stopped` en la etapa 1. Paradas que siguen (anteriores, fuera de esta tarea): `040920` /
  `040DB4` / `040DF0` (cámara 1376..1436), `01FECE` / `0203FC` / `01F934`, `0B833A`.
- **Pendiente**: el último enemigo `04153E` (con `04445A`) y el final (créditos), sin empezar. Plan: `04153E` (`0C8CD6`,
  +86..+88 y 10C448 a 0, `04157A` por `0040BC` con +72 = creador, `01ACF2`) es el árbol de 45 constructores de la etapa 3
  (`04157A` → `042BDC`, `042E22`, `04307E`, `0431E0`, `0444E8`, `045428`; código `037404..038800`, `041500..046700`,
  `08DD2C` / `08EB18` / `08F1A0`). Una captura de familia de toda la etapa (322339..347200, 2639 creaciones) no cabe en el
  disco libre (1,6 GB): se hará por ventanas (`runB2.sh` con `FROM` / `LAST` y raíces por rama: `045428`/`045608` desde
  322504, `0431E0`/`0436B6` desde 323520, `037404` desde 324636, `04307E`/`0438A4` desde 324938), una familia por ventana.

## `040920` con prueba bit a bit (16-XI, agente C)

- Captura **`captures-m6/fam/S`** (`runS.sh`: réplica de `run4` desde `sec09_292683`, raíces
  `af1c4,af17e,8a732,9ae1e,9ae2c,9ba2a,3c16c` de 294380 a 295900; traza en bruto borrada al tener fixture). `fam/Z`
  borrada: las 128 pasadas de `040920` que contaba (nodos 102754, 102804, 104174, 102494, 102CD4) son de los rebeldes
  `0AF1C4` / `0AF17E`, del granadero `08A732` y del soldado del escudo `09AE2C`.
- Familia **`m6reb`** (`make check`): **61 nodos, 4668 actualizaciones en lazo cerrado iguales a MAME** (294407..295899),
  59 borrados, 1006 dibujos; contiene las 128 pasadas de `040920`. Lo que hizo falta, todo traducido de la ROM:
  - `019240` entero (`neo_gate.c`, tabla de 45 entradas `01924A`): D0 (onda) y D1 (chapoteo) con acarreo; la etapa 9
    (`01932E`) da 14E / 154 antes de la cámara 0700 y 165 / 166 después. Antes se paraba la onda `03C62A` de la M6
    (`mission > 8`); ahora solo se para fuera de la tabla.
  - `004C24` con `+50` = 0 (`ga_register`): no registra pero responde por `+5A` (`004DA6`, D2/D3 = 0). Antes devolvía
    «sin golpe» (el soldado del escudo roto `09B4F8` borra `+50` y luego mira `09CDB8`).
  - Las llamadas de programa `04061C..0406A0` (`NEO_ANIM_REPLACE_PALETTE`) en nodos genéricos: `009F88` (`+1D` = 0)
    y `009EA8` del recurso (antes solo cambiaban el recurso; `+1D` quedaba en 2).
  - La bomba de los rebeldes **`03C16C`** (`03C13E` de sus programas; `neo_m3enemy.c` `bomb_tick`, evento
    `NEO_SOLDIER_EVENT_BOMB` en `neo_world.c`): `03C16C` / `03C1D6` / `03C296`, tablas `2D3D9A` y `03C11E` /
    `03C12E` en `region_rom.json`, programa `23F39A` (`extract_soldier_programs.py`). `ga_place_at_words` = `01ACF2`
    desde las palabras de un soldado (con fracción y posición previa).
  - El módulo de soldados deja `10C00A` como el último barrido (`0128DA` 0, `0143CC` FF), para la prueba.
  - Arnés (`test_neo_family.c`, `compile_family_fixture.py`): `--m6ring` (el puente `10D7B6` y su anillo `10D83E`
    como se leyeron; `neo_kscene_t.bridge_raw`), `--group-map bc67c` (mapa de grupos de la etapa, 8 bytes de la ROM
    en `region_rom.json`), la corrección `+25` de los objetos dinámicos para los soldados (`01AC00`), la onda
    `03C52C` y la bomba de los soldados por sus eventos, y la muerte `040500..040FFF` va al módulo de soldados solo
    si el nodo nació soldado (el granadero y el del escudo mueren en su nodo genérico). Las manchas de sangre
    `040F60` / `040F88` son registros de salpicadura del mundo: fuera del rango de la familia.

- **Etapa 1 hasta su final en ares (16-XI)**: en MAME el fin de etapa `018F92` lo da la salida `0B6ED2` (304151) que crea
  la llamada **`0BD67A`** de la lista `0BCA00` (la última; `10C70B` = 1, `0040BC` prioridad 1C, (0F00, 00A8) de la raíz,
  `+6E` = `0BD536`, `+72` = 0F20). Nativa en `neo_world.c` (antes las llamadas no listadas se ignoraban; el rectángulo
  `0BD536` en `region_rom.json`). Limitaciones anotadas: la inserción por prioridad 1C no se modela (va tras el actor de
  guion, como las demás llamadas de la lista) y `10C70B` no tiene lector nativo. `test_mission6_ares.py --next` (PASS
  solo si empieza la etapa siguiente) y prueba **`m6s1end`** en `regression_ares.py` (`--stage 1 --jump 1.0 --turn 6
  --continues --fire --progress 3520 --seconds 1500 --next`): cámara 0 → 3520, jefe intermedio vencido, «stage end
  (018722) mission 6: next stage 10» y la fábrica `016E6E` empieza (2 continuaciones, 0 caídas). Con el mando ciego una
  pasada de cada tres se queda sin vidas en las balsas (cámara 508): variación del mando, no del juego.

- **Etapa 2 en ares (16-XI)**: `--stage 2 --fire --jump 2.5 --turn 6 --continues --progress 4240 --seconds 1500 --next`:
  cámara 0 → 4272 sin caídas, pero el guion se para en `2E03DE` (`07`, grupo 5 contado, constructor
  **`086712`**, sin portar, en (1304, 248)): el resto del guion (366 sentencias) no corre y la etapa no termina.
  Siguiente para la etapa 2: captura de familia de `086712` (raíz en su creación) y su traducción; luego el fin de etapa.
- **Siguiente (orden)**: (1) `086712` y el resto del guion de la etapa 2 hasta `016E7E`; (2) `04153E` por ventanas (plan de
  arriba: `045428`/`045608` desde 322504, `0431E0`/`0436B6` desde 323520, `037404` desde 324636, `04307E`/`0438A4`
  desde 324938), una familia por ventana; (3) la huida `017624` y los créditos.

## Etapa 2: el saltador `086712` (16-XI, agente G)

- Guion `2E03DE` (`07 0c 05 00 00 c8`): constructor **`086712`** (también `08671A`, +8B bit 3, y `086728`, +8B bit 1: el que
  anda hasta su +7E), 100 creaciones en la etapa (305250..320869). Código `086712..087C91` (sus hijos `087458` / `087460`
  (disparos), `0876CC` (fogonazo) y `087716` (trozos) incluidos), traducido rutina a rutina en **`src/neo_m6enemy.c`**
  (resumen en su cabecera): entrada `087838`, caída desde arriba `086868`, despertar `0867E8`, acostado `086938`, andar
  `086A1C` / `086B52` / `086C5C`, giro `086B0E`, saltos `086CD8..086EB4` (cuatro tipos por +8B bits 6/7 y el azar), disparo
  `086F4C..087008`, el andador `087066`, la cola común `08712E` / `08713E` / `087184`, las muertes `0871AA` / `0871D4` /
  `087220` / `08728C` y la comprobación del cartucho `0873C0` (no se reproduce: en el original acaba en `004188`), las
  llamadas de programa `087B84`, `087C0E`, `087C86`. Tablas en `region_rom.json` (grupo `m6`): `087C92..087E2F`,
  `2D2C68..2D301F` y los mapas de celdas de las plataformas `0BE338` / `0BE34C` (las registra `0BE460`, sin portar; los
  saltadores se suben a ellas). Programas en `extract_soldier_programs.py` (sin grupo; 20 nuevos).
- Capturas **`captures-m6/fam/G`** (`runG.sh`: réplica de `run4` desde `sec10_304271`, raíces `86712,8671a,86728` de 305200 a
  309000) y **`G2`** (`LOAD=at_312000 FROM=312000 LAST=316500 NAME=G2`); trazas en bruto borradas.
- Familias (`make check`): **`m6jump`**: 131 nodos, **11 790 actualizaciones en lazo cerrado iguales a MAME** (305250..308998),
  119 borrados, 11 671 dibujos; **`m6jump2`**: 58 nodos, **5099** (314429..316499). `--split-children` (los seis trozos de
  `087C0E` caen en dos fotogramas). No vistos en las capturas (traducidos, sin prueba): la muerte por explosión `08728C`
  (`0872FA`, `087362`, `08739C`, paleta 852) y `0869AC` / `0869E4`.
- Sprites: `export_rest_sprites.py ... G@86712-87c91 G2@86712-87c91`: 508 880 píxeles iguales, 0 distintos; 311 descriptores
  nuevos y 11 paletas (C0..C3, 188..18B, 868, 86B, 882). Sin ver: 852, 85F, 860, 86A.
- Con esos 311 descriptores el paquete daba 1291 trozos (límite 1256): resuelto con el recorte de abajo. Copia de la
  exportación M6 al día: `/home/user/captures-m6/fam/G/rest-native-m6jump.json` (privada).
- **Plataforma `0BE460`** (`neo_m6enemy.c`, `pf_*`): programa 2A9626 / 2A9650 (la que baja), paleta 508, mapas de celdas
  `0BE338` (grupo 3) / `0BE34C` por `0140C4` (0, −1). Familia **`m6plat`** (`captures-m6/fam/G3`, raíz `be460`, 305200..307500):
  3 nodos, 1445 actualizaciones iguales a MAME.
- **Jeep `0A2EFA` / `0A2F14`** (registro contado de la cámara 2672): entradas nuevas del jeep armado de la M3
  (`neo_m3enemy.c`, +96 bits 0 / 0 y 1: el sello del desguace `0C88B0`). Familia **`m6jeep`** (`captures-m6/fam/G4`,
  `LOAD=at_312000`, raíces `a2efa,a2f14,676a8,676b6`, 312000..314600; `--ram 10c72e-10d7b6` para la ventana de la capa que
  lee el sello, `--soldier a3b50`): 5 nodos, 910 actualizaciones, 2 sellos iguales a MAME.
- **Fin de la etapa 2**: la llamada `0BE888` de la lista `0BDCE2` (progreso 3952) crea `0BE8A2` (en `neo_world.c`, tras el
  actor de guion como las demás llamadas: la inserción por prioridad 1C no se modela), que borra 10C716 y espera; el
  registro contado `0BE8F6` (cámara 4240) pone 10C716 = 1 (`neo_ga_flags_t.m6_exit`); entonces `0BE8A2` crea la salida
  `0B6ED2` en (11E0, 0038) de la raíz, +6E = `0BE89A`, +72 = 1220. La plataforma `08AFA8` (registro contado de 4101) es una
  entrada nueva de la balsa `08AFB6` de la M3 (+6E = 0, +7F bits 2/3 sin poner); de paso, `08B090` guarda ahora `08B18A`
  (el `bra 08B18E` del original) en +0 como MAME. Familia **`m6end2`** (`captures-m6/fam/G5`, `LOAD=at_321000`, raíces
  `8afa8,be8a2,be8f6`, 321000..322330; el arnés lee y escribe 10C716): 5 nodos, 622 actualizaciones iguales a MAME,
  incluida la salida. En ares la balsa de la etapa 2 toma además el camino `08B144` / `08B170` (+80 ≠ 0: la plataforma que
  se hunde: programa `E539A[+6E]`, +34 = 100, `08B2DE` y `08B31A`), que `run4` no recorre: traducido sin prueba bit a bit
  (antes era una parada explícita `8b144` que dejaba la etapa sin salida).
- Pendientes vistos en ares (otras familias, sin tocar): paletas 21C (el jeep `0A2F2E` con +70 ≠ 0) y 214 (el soldado
  `0413B0`, programa 24124C) sin exportar: dibujos contados como ausentes; paradas `413d4` / `40df0` del módulo de soldados.
- Recorrido en ares (`test_mission6_ares.py --stage 2 --fire --jump 2.5 --up 1.5 --sweep 10 --continues --progress 4240
  --next`; `--up` / `--sweep`, nuevos: disparos hacia arriba y paseo a izquierda y derecha, elección del jugador): con el
  guion completo la cámara se retiene en 1104 (los saltadores sobre las plataformas), 2672 (el jeep) y 4240; el mando ciego
  necesita ~60 min y muchas continuaciones.
- Sin portar en la etapa 2 (registros no contados, el guion los salta contándolos): `032946` / `032AA4`, `0676A8` /
  `0676B6` (los sellos `0C88B0` de 3600 / 3642, con `0C899E`; tablas `0E199E..0E1A5E`), y los actores de cabeza de la lista
  `0BE02C..0BE27E` (con `0BE5E4`, `0BE592`, `0BE700`, `0BE7CC`).

## Recorte del paquete de la M6 (16-XI, agente G; decisión del coordinador: no subir el límite)

- Regla determinista (`tools/pack_mission.py`, `mission_descriptor_rule`, misiones en `TRIM` = {6}): un descriptor de la
  exportación entra en el paquete salvo que **todos** los programas de animación que lo posan (`data/soldier_programs.json`,
  órdenes POSE, también las de las tablas de poses) sean de un grupo que la misión no carga (`GROUPS[6]` = m3, m4, m5, m6;
  un programa sin grupo está en `/programs.bin` y lo cargan todas). Los descriptores que ningún programa posa (dibujados por
  código) se quedan. En la práctica salen los del grupo `m2`. La exportación `filesystem/mission6/rest-native.json` sigue
  completa (copia propia); el recorte lo hace el empaquetador.
- Prueba **`tools/check_mission_descriptors.py 6`** (en `make check`): todo descriptor que la M6 referencia (POSE de sus
  programas cargables, más los exportados que ningún programa posa) y que está exportado está en las exportaciones de la
  escena o en la tabla de actores de `romfs/mission6.pack`, y el paquete no lleva ninguno que la regla quite. Hoy: 11 911
  referenciados y exportados, 0 fuera; 1505 referenciados que no se han exportado nunca (contados: en ejecución un dibujo
  sin descriptor es una parada explícita).
- Paquete con el saltador `086712` y la plataforma `0BE460`: **1090 trozos** (límite 1256; antes del recorte 1291).

## Estado al cerrar la sesión del agente G (16-XI)

- **Hecho**: etapa 2 con todos sus registros contados nativos (`086712`, `0A2EFA`, `08AFA8`, `0BE8F6`) y su fin (`0BE888` →
  `0BE8A2` → `0B6ED2`), con pruebas bit a bit (`m6jump`, `m6jump2`, `m6plat`, `m6jeep`, `m6end2`); paquete M6 recortado por
  regla (1090 trozos) con `check_mission_descriptors.py`. Commits `412191c`, `8560a6b`, `8a13e09` en `work-chars`.
- **ares** (`m6s2end` en `regression_ares.py`, sin tocar la línea base): en una pasada de 75 min el mando ciego llegó a la
  cámara 4240, `0BE8F6` puso 10C716 y la salida `0B6ED2` se creó (manos de los jugadores cambiadas), pero el jugador no podía
  salir de la pantalla (recorte `end_x` con el máximo de cámara observado, 4240): corregido en `clamp_feet` (bajo 10C00B
  hasta el final de la rejilla). En la pasada siguiente el mando perdió todas las vidas ante los tanques de 3872: **falta
  ver la etapa 2 terminada en ares** (repetir `m6s2end`; es variación del mando, no del juego).
- **Falta** (por orden): (1) `m6s2end` en verde; (2) la etapa 3: `04153E` y sus 45 constructores (plan de ventanas de arriba:
  `045428`/`045608` desde 322504, `0431E0`/`0436B6` desde 323520, `037404` desde 324636, `04307E`/`0438A4` desde 324938);
  (3) la huida `017624`, el final y los créditos; (4) los no contados de la etapa 2 (`032946`, `0676A8` / `0676B6`, cabeza
  de lista `0BE02C..0BE27E`) y las paletas 214 / 21C / 852 / 85F / 860 / 86A.
- La exportación M6 al día (privada) está en `filesystem/mission6/rest-native.json` de `ws-chars` y en
  `/home/user/captures-m6/fam/G/rest-native-m6jump.json`: **el coordinador debe copiarla a `ws-logo`** al fusionar y rehacer
  `build_packs.py 6`.

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

## Paradas de la etapa 1 (13-XI, agente C, WIP: parado por orden del usuario)

- **Hecho, sin `make check` completo ni ares** (WIP): (1) `0B833A` (cartel, registro `2DF690`) se paraba porque la
  tabla `0B8444` y los programas de los carteles (`2A8BAC..2A8E34`) eran del grupo `m2`: ahora sin grupo
  (`region_rom.json`, `extract_soldier_programs.py`; `soldier_programs.json` regenerado: solo esos 22 programas
  cambian de grupo); familia `signs` PASS. (2) `0203FC`/`01F934`/`01FECE`: la trampilla `01F836` (registro `2DF67E`)
  hace la roca grande `01FBAC`, que no era nativa, y el estallido `01FECE`; traducidos en `neo_m6props.c`
  (`rock_tick`), programa `22FA34` añadido. Captura `captures-m6/fam/K` (`runK.sh`, raíces `1f836,1f844`,
  292684..294100); familia **`m6rock`** (`--range 1f828-2055f`, RAM de `m6mid`, `--node-fields 82`): **21 nodos, 2153
  actualizaciones en lazo cerrado iguales a MAME** (292687..294067), 2 `01FBAC` y 2 `01FECE`. `compile_family_fixture.py`
  / `test_neo_family.c`: hasta 64 lecturas de mapas de celdas por actualización (antes 48; `m6rock` lee 51).
  Sprites: `export_rest_sprites.py` con raíz `22FA34`; la exportación M6 = la anterior + los 25 descriptores de
  `22FA34` (filtrada: la salida completa traía 1347 descriptores de jefes M4/M5 que no son de la M6); 56 634 píxeles
  iguales, 48 distintos, todos en las columnas x 318..319 (borde derecho, el volcado no los muestra). Paquetes
  rehechos (`build_packs.py`): M6 1251 trozos (límite 1256). **El coordinador debe copiar
  `filesystem/mission6/rest-native.json` de `ws-chars` a `ws-logo`** al fusionar.
  (3) `040920`: traducido en `neo_m3enemy.c` (nodo genérico, `ga_to_root` nuevo en `neo_gactor.c` = `01AC4C` hacia
  `10E968`, y `01AC38`) y en `neo_soldier_ai.c` (soldado; parada explícita si el padre no es terreno, mapa o raíz).
  **Sin prueba bit a bit todavía**: captura `captures-m6/fam/Z` (`runZ.sh`, solo depurador, M6 entera): MAME pasa
  por `040920` 128 veces, todas en la etapa 1, 294644..295808, nodos 102754/102804/104174/102494/102CD4 (rebeldes
  `0AF1C4`/`0AF17E` muertos, `+00` = 040D06/040DF0). Siguiente: captura de familia con raíces `af1c4,af17e` de
  294380 a 295900 (`--soldier-nodes`, `--range af000-b0fff` y `40500-40fff`) y fixture; después `make check`, ares
  etapa 1 y matriz. `fam/Z` (193 MB, sin trazas de familia) se puede borrar cuando exista esa familia;
  `fam/K/boss-trace.txt` (41 MB) ya tiene su fixture (`data/m6rock_fixture.json.gz`).
- Falta además: `af964` (visto en ares en la cámara 1620), `04153E` (último enemigo) y los créditos (sin empezar).

## Etapa 2: lo no contado y su final en ares (16-XI, agente K)

- **`m6s2end` en verde** (ares, cartucho `078e123` con el tope de salida de `clamp_feet`): `test_mission6_ares.py --stage 2
  --fire --jump 2.5 --up 1.5 --sweep 10 --continues --progress 4240 --seconds 5000 --period 30 --next`: cámara 0 → 4272,
  `0BE8F6`, la salida `0B6ED2`, «stage end (018722) mission 6: next stage 11» y la arena `016E7E` empieza: `RESULT: PASS`
  (19 continuaciones, 0 caídas, ~65 min). Las continuaciones son infinitas (ruta AES de `0C987A`): el mando ciego no se
  queda sin ellas; solo tarda. No se ha hecho estado guardado: un estado de ares lleva el código del cartucho y no sirve
  para la ROM siguiente.
- **Máquinas de la cabeza de lista `0BDF3A`** (`neo_m6enemy.c`, `mc_*`; resumen en su comentario): las seis `0BE02C`,
  `0BE082`, `0BE0D8`, `0BE180`, `0BE1D6`, `0BE27E` en sus puntos de la raíz (llamada de lista en `neo_world.c`, tras el
  actor de guion como las demás: la prioridad 1C no se modela), su tripulación `0BE5E4` / `0BE700` / `0BE7CC` (la muerte
  `0406B6` del módulo de soldados, `neo_m3enemy_death_406b6`, nuevo), el humo `0BE592`, los restos `01DA40 0BE836` y
  `01DBAA`, los objetos `08CD58` de `2D5C12` y los parches `0C70FC` del submapa `0BDBB2` (sin celdas de colisión). Las
  llamadas de programa `0BE43A` (`0049E8 0BE360`) y las del convoy (abajo) en `k2_call`.
- **Convoy `032946` / `032AA4`** (registro `2E019C`, cámara 290): traducido con sus llamadas `23914C` (`00408C`), `23915C`
  (el objeto 30 de `08CD70`), `23918A` y las de `032AA4` (`239346..2393D4`, con su compañero `+76`). En `run4` el `032AA4`
  no llega a crearse (traducido sin prueba bit a bit).
- **Restos `0676A8` / `0676B6`** (registros `2E1238` / `2E124A`): `0C899E` (la plantilla dentro de la ventana del anillo,
  `k8c899e`) y el sello `0C88B0` con `+2C` desplazado por `+7A`; `0C8BEC` o la cámara `+70` para borrarse.
- Capturas **`captures-m6/fam/W1`** (`NAME=W1 ROOTS=be02c,be082,be0d8,be180,be1d6,be27e,32946 FROM=304272 DUMP0=304300
  LAST=312600 PERIOD=60 sh runG.sh`) y **`W2`** (`LOAD=at_312000 LOADF=312000 NAME=W2 ROOTS=676a8,676b6 FROM=313900
  DUMP0=314100 LAST=321700 PERIOD=120`); trazas en bruto borradas. Familias (`make check`): **`m6mach`**: 13 nodos, **18 030
  actualizaciones en lazo cerrado iguales a MAME** (304274..310146), 13 borrados, 6274 dibujos (las cuatro máquinas que
  `run4` destruye: `0BDDAA` 304509, `0BDDE0` 304600, `0BDE64` 304977, `0BDE9A` 309163); **`m6wreck`**: 2 nodos, **4867**
  (316707..321699), 2 sellos iguales.
- Parches de mapa: `filesystem/mission6/s2` es ahora **copia propia** (antes enlace a `ws-logo`) con `patches.json`
  (`extract_map_patches.py 6 filesystem/mission6/s2 --ram-colours --patch-frames 0:W1/vram304560.bin,1:W1/vram304620.bin,
  2:W1/vram304980.bin,3:W1/vram309180.bin`; opciones nuevas: los colores de la RAM de paleta del volcado tras cada parche,
  porque los números 16 y 22 cambian de colores en la etapa): los cuatro parches que hace `run4`, todas sus columnas vistas
  en SCB1. `0BDE16` (5, D) y `0BDEEA` (6C, D) no salen en `run4`: sin colores observados, **pendientes** (sin parche de
  dibujo; no tienen colisión).
- Programas (`extract_soldier_programs.py`, sin grupo, 22 nuevos) y sprites (`export_rest_sprites.py ... W1@be02c-be887
  W1@32946-32b3f`, luego `W2@676a8-67830`, raíces y paletas nuevas): 170 508 + 192 782 píxeles iguales, 0 distintos; la
  salida se filtró como hizo el agente C (la exportación anterior más los descriptores que posan los programas nuevos: +72;
  la salida completa trae 1347 de jefes M4/M5). Paletas nuevas: 4F2, 507, 50E, 50F, 5E2 y, vistas en `W2`, **21C, 85F, 86A**.
  Siguen sin ver: **214, 852, 860** (ni `run4` ni sus réplicas los dibujan; pendientes de una captura que los muestre).
- Paquete de la M6: 1099 trozos (límite 1256). ares (`--stage 2 --fire --jump 2.5 --continues`, 300 s): PASS, el guion
  crea `032946` nativo, las máquinas se ven, los mismos recuentos que la línea base (`missing: tile path` de `m6s2.bgp`).
- **Copiar a `ws-logo`** al fusionar: `filesystem/mission6/rest-native.json` y `filesystem/mission6/s2/patches.json`
  (y rehacer `build_packs.py 6`).

## Etapa 3: el último enemigo `04153E` (16-XI, agente M; en curso)

Traducción en `src/neo_m6boss.c` (resumen del árbol en su comentario de cabecera), rutina a rutina, por ventanas de
`run4` (`captures-m6/fam/runM.sh`: réplica desde `sec11_322339` o `at_323500`, raíz `4153e` o `RANGE=41500-46800`).

- **El árbol**: el nodo de cabecera `04153E` (las órdenes en sus bytes `+6E..+72`, una por parte; `+74` / `+8A..+8D` la
  sacudida; `+7E` la velocidad de programa; `+86` bits 0 (segunda forma) y 1 (fin)), el cuerpo `04157A` (su dibujo es la
  capa `10D7B6`, como el jefe de la M4: `neo_m4boss_env_t`; velocidades `694E` / `6952` desde `+86` / `+8A` en la cola
  `041F1C`), las cinco partes `042BDC` / `0431E0` / `042E22` / `04307E` / `0444E8` (tablas de rutinas `0453A6`, orden nueva
  en `044860`, hecha en `04489E`), la apertura `045428` con sus ocho actores `045608` (línea de tiempo `045D84`, listas
  `045E02..`), el fundido `0BEA82`, las llamas `0436B6` / `043860`, los proyectiles `042F04`, los soldados voladores
  `0438A4` con `04416E` / `0441CA`, y los auxiliares `0443BE` (muertes `004BFE`), `04445A` (la cámara: `neo_camera_place_*`,
  `genv.camera_place` / `camera_xy`), `044A08` (el suelo que se rompe: parche `0C714E` de `045244` sobre `10C72E`),
  `044B32`, `0424F8` (subguion `019E72` con el registro `2C7920` a 3000 de vida), `042AEA`.
- **Ataques del cuerpo**: `042308` elige `+80` por la posición del jugador (`044FCA`) y el rango (`04463E`); los pasos de
  cada ataque son la lista `044ECC[+80]` (`04240A` / `042440` / `04246E`), traducidos en `step_run` (`042516..042A9A`);
  `042AAC` (`044F9C`) dice cuándo acaba. La segunda forma (`041634..0417FE`: `041BCE`, `041C78` con el mapa `04522E` de la
  capa, `041E38`, `04493E`, `044AE4`, `0420A8`, `0420FC`, `04215E`, `042178`, `041EC0`; después `04249E` / `0424AC`) está
  traducida; el final (`04180A..`: `041D2E`, y en `041826` el manejador `0CA438`) **para explícitamente** en `041826`, y
  la huida `04677A` en `041872`.
- **Manejadores de los jugadores** (`neo_ga_flags_t.player_handlers`, ahora por jugador con `handler_p2` /
  `split_handlers`): 6 = `0CA386` (andar hasta `10EA00`/`02` y borrarse al llegar, `0CA324`), 7 = `0455E6` y 8 = `041EBC`
  (sin mando). **Pendiente en `gameplay.c`** (el juego nativo aún no los anda: el 6 se trata como sin mando).
- Motor: `019388` como `genv.mission_speed` (`1013D6` / `1013DA`), `10C448` (`flags.m6_boss`), `01C87E` en la tabla de
  efectos simples (paleta 175), `neo_item_drop` (`08CD58` desde otro módulo). `009CCE` sobre ranuras fijas (el brillo de
  `042BDC`, la paleta 23 del fondo en `0BEA82`) se cuenta como externo: **pendiente** (no se dibuja).
- Datos de la ROM (`region_rom.json`, grupo `m6`): `04496C`, `044DC4..045428`, `045D0E..045EF4`, `046740..046774`,
  `2C4084..2C7A00`. Efectos simples nuevos `01C87E` y `01C8FC` (sin giro al azar). Sprites: `export_rest_sprites.py ... M1@41500-46800
  M2@.. M3@..` (682 775 píxeles iguales, 0 distintos) y la mezcla de `merge_rest.py` en `filesystem/mission6/rest-native.json`
  (+356 descriptores, 19 paletas; sin ver aún: D9, 169, 175, 184..186, 102, 35D, 364, 365, 36C); paquete M6 1192 trozos. Programas en
  `extract_soldier_programs.py` (sin grupo).
- **Familias** (`make check`; extracción común: `--range 41500-46800 --range bea82-beab1 --attacks --ram 1010a4-105564
  --ram 1063d8-1063da --ram 10c72e-10d83e --ram 10e94e-10e9dc --ram 10e9f8-10ea04 --ram 10c300-10c308 --ram 10000e-100010
  --ram 10c2f6-10c2f8 --ram 10c448-10c449 --ram 10c33c-10c33e --ram 10c00a-10c00d`; prueba `--m4boss --node-fields
  6e,72,86 --ext-kills`, nueva: un nodo muerto por un disparo del jugador entre sus actualizaciones ya se ve muerto por los
  que corren antes en la misma): **`m6boss1`** (`M1`, 322502..323598): 18 nodos, **8534** actualizaciones en lazo cerrado
  iguales a MAME; **`m6boss2`** (`M2`, 323500..324698): 17 nodos, **8285**; **`m6boss3`** (`M3`, 324600..325798, con los
  soldados): 29 nodos, **5176**; **`m6boss4`** (`M4`, `at_340000`, 346950..348398: el descenso de la segunda forma,
  `04493E` con el suelo `044A08` / `044B32`, `04445A`): 11 nodos, **5882**; **`m6boss5`** (`M5`, 348300..350399: la
  segunda forma `0417F0` / `0417FE`, los soldados del ataque 8 y el generador `045EF6` con los andadores `046112` /
  `04611C` y sus bombas `046510` / `0465FE`): 94 nodos, **13 831**. `--ext-kills` (nuevo, `FAMILY_KILLS`): el extractor
  anota las lecturas del `+5F` (bit 7) de otro nodo de la familia y la prueba se lo aplica antes de esa actualización (un
  soldado muerto por un disparo del jugador visto por sus disparos). El arnés lee y escribe ahora `10E9F8..10EA03`, `10C448`, `10C00B` / `10C00C` y la cámara
  `10E95A..` en la imagen de RAM cuando la familia las vigila.
- **Consola (16-XI)**: con `neo_m6boss.c` (44 KB de texto a `-Os`; probado `-fno-inline*` y accesores fuera de línea: no
  baja) el arranque en ares para en `scene_assets.c:506` («RAM budget: 3307 KB left for the scene cache (minimum 3332
  KB)»). Hasta encontrar ~25 KB, el módulo queda **apagado en el cartucho** (`MSX_M6BOSS`, `neo_region.h`: 0 en N64, 1 en
  el anfitrión; las pruebas lo tienen siempre): el guion sigue parándose en `2E19F8` → `04153E` como antes. **19-XI (`work-mem`)**: encendido en la consola: los cuatro jefes de misión son superposiciones de código que comparten una región (`src/msx_overlay.c`, `docs/RENDIMIENTO.md` «Superposiciones de código»); caché de escena 3360 KB (210 ranuras). ares
  `--stage 3`: PASS (la línea base, cámara 0 → 320). Siguiente para la consola: memoria (p. ej. código por misión cargado
  del paquete, o mover datos del montón) y entonces la prueba `m6boss` en `regression_ares.py`.
- **Sin traducir todavía** (por orden): (1) el combate de fondo `037404` (`037404..038800`: los aviones `03762A` /
  `03790C` sobre `10E83E`, sus disparos `037CEC` / `037E04`, humo `037B00`, `037FBC` con `038092` / `03848A` / `038604`,
  explosiones `03825C`; ~1000 líneas, decorado sin efecto en el juego) — en `M3` existe ya la captura de su comienzo;
  (2) el resto de la segunda forma: `08F1A0` (el proyectil de los tanques `08DD2C` con `+93` bit 5) está **traducido sin
  prueba bit a bit** (`neo_effects.c`, y `neo_m5enemy.c` ya no para en él); la ventana `M6` (`LOAD=at_350000 FROM=352700
  LAST=354000 ROOTS=8dd2c,8eb18,8f1a0`, traza en bruto guardada, 75 MB) aún no pasa: en 352726 el soldado `1023E4`
  (primer nodo, `--resume`) lee su `+72` como dirección cruda `1018E4` en vez de la etiqueta de la cabecera (algo inyecta
  los bytes grabados después de la traducción de `--node-fields`; por investigar en `test_neo_family.c`), y no se ha
  visto ningún `08F1A0` en la traza y su muerte `04180A..` (`run4` no la alcanza: hace falta una captura de familia
  del TAS, de `u26300` a `u26480`); (3) `041826` (`0CA438` / `0CA49C`) y
  `04677A`; (4) los manejadores 6..10 en `gameplay.c`; (5) sprites (exportación con `M1..M3`) y paquete; (6) prueba
  `m6boss` en ares; (7) la huida `017624` y los créditos.

### La muerte y la huida del último enemigo (17-XI, agente P)

- **Captura del TAS** (`captures-m6/fam/runP1.sh`: `tas_updates.lua` sin escrituras desde `u26340` hasta `u26708`, el
  final del registro de 0.251; `RANGE=41500-47400 ROOTS=bea82`, sin volcados; 0 desincronizaciones; 14 MB): el fin de la
  segunda forma, la muerte `04180A..` y el comienzo de la huida `04677A`.
- **`04180A..` traducido entero**: `041826` = `041DD4` (RAM `10F003` = 0: `neo_ga_flags_t.join_off`, el HUD ya no deja
  entrar al jugador 2; `0446D4` a (E0, 118); `018F92`), `041E38` y `041E9A` (`0CA35C`: los jugadores andan con `0CA438`
  a x 170 / 1D8 y miran a `10EA04` / `10EA05` = 0 / FF: manejadores 9 y 10, `walk_face`); `041872` crea la huida
  `04677A` (01ACF2 (0, 0)), espera su `+60` (`0418B0`) y en `0418C2` corre `018F40` (fin de etapa).
- **La huida `04677A`** (`es_tick`): 01ABC8, los nueve actores `046858` de la lista `0473B0` (cada uno con su línea de
  tiempo `0473FC..`: pares (señal, rutina)), su propia línea de tiempo `047394` ((señal, duración) en `+8A` / `+8C`;
  duración 0 = esperar a que un actor ponga `+8F`); al acabar, el `+60` del cuerpo. Los actores (`ea_tick`, todos
  traducidos, `046858..047256`): `04685C` (la señal), `04687A` la nave (programa `243AD6`, paleta 248, el camino
  `0472C4` con `021288`, ahora `neo_path` compartido desde `neo_heli.c` con las llamadas de la orden 8: `0469C0` /
  `0469D6`) y su pieza `0469E8`; `046964` el estallido (1035, `01DA40` `046912`, `0C8022`, `004192`); las órdenes al
  nodo de cabecera `046A48` / `046A78` / `046A8C` / `046C4C` / `046CC8` / `046D6E` (con `0446D6` sobre el cuerpo y el
  programa del cuerpo apagado por `004260`); las cinco explosiones `046A9A` → `046DCE` (`046EF2`, `1C706`); `046B40`
  (`047342`, el contador `046B90` con el sonido 1026); `046EFC` (cae por `01495C`, programas `244054[0..3]`, `2440EA`);
  los soldados que huyen `04702C..0470E0` (`0472B8`, `2442E8` / `24449E`, `244366` / `24451C`, `047238`, `244572`,
  las llamadas `047274` / `047296` / `0472A8`) y `047240` (`244630`).
- **Fuera de la traducción, a la vista**: (1) `046BAA` / `046E64` (y `037512` del combate de fondo): la sonda de la
  protección de mslugx (`10F00C` += 3EF; con acarreo, lecturas de `2FFFE0` y `10F00A`; si falla, `046774`). No se
  modela: la placa original va siempre a `004188` (borrar el nodo), que es lo traducido; `10F00A` / `10F00C` quedan
  fuera de la comparación (zona `10F000..` de `docs/TAS.md`). (2) `046CD4` llama a `0C6F34` sin mapa con D0 / D1 tal
  como los dejó la rutina anterior de la lista (registros muertos): el `+32` / `+34` de la capa `10D7B6` no se
  reproduce; el arnés toma esos 4 bytes del registro (los cuenta aparte). (3) En `046F38` un fallo de `01495C` para
  explícitamente (su D0 no está trazado).
- **Familias**: **`m6boss6`** (P1, 56794..57613): 24 nodos, **5701** actualizaciones iguales a MAME; **`m6boss7`**
  (`captures-m6/fam/P2`: `LOAD=u26400:26400 LASTU=28000 NAME=P2 sh runP1.sh`, hasta el fotograma 60000, donde para
  `tas_updates.lua`): 30 nodos, **25 017**. El arnés conoce los manejadores 9 / 10 y `10EA04` / `10EA05` (`--ram
  10ea04-10ea06`, `--range 46800-47400` además de la extracción común). En 60000 el cuerpo sigue en `0418B0` (esperando
  el `+60` de la huida); `0418C2` (`018F40`) no está en ninguna ventana todavía. Trazas crudas P1 / P2 borradas.
- `gameplay.c`: manejadores 9 (`0CA438`) y 10 (`0CA49C`) para el jugador 1; 7 (`0455E6`) y 8 (`041EBC`) son «sin mando»
  (`moveq #0,d7`), como ya estaba. **Pendiente**: cuándo vuelve `10F003` a no ser 0 (no trazado; el nativo lo borra con la
  etapa).

### El combate de fondo `037404` (19-XI, agente P)

- Traducido entero en `neo_m6boss.c` (`bf_tick`, `037404..038812`): el controlador (`+76` / `+7E`: una ranura por bando,
  etiquetas de nodo), los aviones `03762A` (persigue puntos al azar, dispara `037CEC`) y `03790C` (cruza, apunta por
  `+98` al avión enemigo y dispara `037E04`), humo `037B00`, muerte `037A86` / `037A8A`, el generador `037FBC` de las
  formaciones `038092` / `03848A` / `038604` (con sus compañeros `0381D2` / `03858A` / `038680`, los disparos `03825C`
  entre ellos y el bombardero `038702`), todo sobre la capa del cielo `10E83E` (`01AB98`, `012040`). La sonda de la
  protección (`037512` y los finales de los disparos) no se modela: la placa original borra el nodo igual.
- Un blanco se lee como el original lo lee por dirección (`bf_view`): el nodo nativo vivo o, si murió, los bytes de su
  ranura del montón (`e->node`), que pueden ser ya de otro nodo (en `M3`, 324876: el líder dispara a la ranura de su
  compañero muerto, ocupada por otro actor).
- Familia **`m6air`** (`M3`, 324636..325798): 72 nodos, **4703** actualizaciones iguales a MAME. El arnés da la capa
  `10E83E` como padre (`FAMILY_RAM`). **`m6air2`** (TAS, `captures-m6/fam/runP3.sh` con `LOAD=u24600:24600 LASTU=26420
  ROOTS=37404 NAME=P5`, sin escrituras, 53438..57029): 146 nodos, **12 152** actualizaciones, con las tres formaciones.
  El bombardero `038702..038812` no sale en ninguna captura (traducido sin prueba). Arnés: un hijo nacido en la última
  actualización de su creador guarda la dirección grabada en `+0C`; `creator_of` la resuelve a la última instancia
  de ese nodo de la familia (el humo `037B00` de un avión que sale de la pantalla).

### El proyectil `08F1A0` de los tanques (19-XI, agente P): sin captura posible

Ninguna captura lo contiene: `run4` (`M6`, 352700..354000) no lo despacha, y el TAS tampoco (`runP3.sh`, raíz `8f1a0`
sola, sin escrituras: `sec06_u12887..u23570`, `sec11_u23570..u24620` y `u24600..u26420`: 0 nodos). Queda traducido
sin prueba bit a bit (pendiente: una partida grabada que haga disparar ese proyectil al tanque `08DD2C`).

### El final tras el último enemigo (19-XI, agente Q)

- **Captura** `captures-m6/fam/Q1` (`runQ1.sh`: TAS desde `end27900`, sin escrituras, hasta el fotograma 74 400; raíces
  `570a0,a8c62,57e00,a7ffe,1959c` y `RANGE=41500-47400`, VRAM; 254 MB, borrar cuando estén todos los fixtures). Sección 22
  en U 28967 (f 62176), −1 en U 34179 (f 72602), **sección 20 (demostración tras el título) en U 34681, f 73687**.
- **Registro `017624`: inalcanzable.** Solo se llega a él con 4303 = 3 (`018730`), y en toda la ROM 4303 solo se escribe
  con FF (`01809A`) y 0 (`018F42`); el TAS nunca carga `0BEC28`. Tras la M6, `01893E..0189FA` crea `0570A0` y va a la misión
  0xC, registro 0 = **`017C54`** (descriptor `01709E`: lista `025F3E`, guion 0x858 = sección 22, etapa 0x2C, sin música):
  los créditos. Con 42F6 = 0x2C el bucle va a `018AC0` (espera el bit 7 de `101414 +5F`), `018B18`..`018BBA` (fundidos,
  `01959C`) y el núcleo `101154` crea `057E00` (`016D3C`) y `0A7FFE` (`01674A`).
- **`m6boss8`** (Q1, 60000..60917): el cierre del cuerpo `0418C2` (`018F40`) hasta el borrado general; 7024 actualizaciones
  iguales. Arnés: con `--m4boss` los globales vigilados 10C300/10C304 empiezan como se leen (como `--m2boss`).
- **`src/neo_ending.c`** (lista de `neo_end.c`): la escena de resultados y clasificación `0570A0` entera (`0577E2` el
  dial, `0576B6`, `05764A`, `05731C`, `05752E`, la entrada de nombre `05812A`, la tabla `057C82` en 10F010, con el bucle
  de 65 536 vueltas de `057CDA` y su escritura en el espejo 14F008 = 10F008) y la tabla de récords `057E00`/`0580A2`.
  RAM del original por `neo_end_ram_t rd/wr`; el reinicio de entorno `056FCC`/`057F34` es `ram->setup`.
- **Prueba** `tools/test_neo_ending.c` (`extract_ending_fixture.py`, `compile_ending_fixture.py`; `ENDINGS` en
  `make check`): nodo, RAM escrita (exacta, sin escrituras de más), VRAM en orden y azar. `scene`: 5010 actualizaciones,
  9882 palabras de VRAM, 490 bytes de RAM iguales; `ranking`: 214 y 2058. Sin comparar (contados): VRAM/RAM del reinicio y
  el A2 muerto que `03D292` guarda en +E.
- (Hecho por el agente R, abajo: páginas, temporizadores y desfile.)

### Los créditos (20-XI, agente R)

- **Quién los crea**: la sección 22 del guion crea dos espadachines `0A8C62` (los de `neo_rest.c`, fuera de la pantalla, en
  `(360,-32)` y `(360,-24)`; `random_choice` de 0029C6 entre variantes con +74 = 4..8). Cada uno, al conmutar (`0A8F1E`),
  crea el subguion `019E72` con el registro `rl(0x87A + 4·(+74))`, y **ese subguion** crea las páginas, los temporizadores,
  la música, las láminas y la orden de audio. En el mundo ya existen las dos piezas (`neo_rest.c`, `e->subscript`).
- **`src/neo_ending.c`** (traducidas y probadas): página `0646FE + 0x0A·k` (k = 0..47; tabla `0E1446`, 26 bytes: primera
  paleta y seis descriptores) con sus seis líneas `064AC0`; música `0A4408` (0x31 tras +6E actualizaciones; **RAM 10C604 =
  FF mientras un jugador en juego mantiene A, B o C: todo va 4 veces más deprisa**, `0A49B0`); temporizador `0A4478`; orden
  de audio `0A453A` (`01228C`); lámina `0A454C` (programa `2A4B16`, 63 láminas por +A8, paletas 0x845 / 0x125 desde la
  0x3E). Aritmética `04E5D0` / `04E962` (16.16 con signo) y la escala de 50 Hz `0A4996` (RAM 100002 = FF: ×0.D600).
  `009CCE(ranura, D1, nivel)` (mezcla de la ranura de paleta hacia la paleta D1 = 1 con nivel 0..0x20) sale por
  `neo_end_ram_t.slot_level` (nivel de la página en su +8C, de la lámina en su +A0): **el dibujo con ese nivel está
  pendiente** (hay que desensamblar el lector de `1068EA +C/+E` para el color exacto).
- **`neo_end.c`**: `GA_DELETE_TREE` (`0041AC`: el nodo y sus descendientes).
- **Título `0A7FFE`** (traducido entero, `neo_ending.c`): raíz (`002976` y capas `0A7D4A`: `ram->setup`), diez piezas
  `0A81A4` (tabla `0A7FD6`: vuelo 0x10, cada 0x0C; lugares `0A7FAE`; espiral `0A8278`, zoom +12/+13, escala D7, fondo +1E;
  `009CCE` de su segunda ranura hacia la paleta 0x159), sonido 0x10E1 y 24 chispas `0A83D6` por pieza (programas `0A83CA`,
  dos estelas con una sola animación; la última pieza: 0x10E5 y el +60 de la raíz), fundido a blanco, `0A8096` (`004192`
  borra los demás nodos; `0A863C` = capas `0A7D52` y textos, `ram->setup`), imagen `0A8602` (`1D5B22`, paleta 0x840),
  programa `2A7368`, 0x96 actualizaciones y el +60 de su creador. Dos datos de la captura, no deducidos: **el bit 0 de +15
  de la chispa es el indicador N con que vuelve `0029C6`** (el signo de la palabra del anillo que cambia en `0029EE`:
  `ram->rand_negative`; el fixture lo lleva en el bit 16 de cada palabra de azar) y **las mitades altas de D1/D2/D3 al
  entrar en `0A83D6` (FFFF, 0, 0 en las 216 chispas)**, que pasan a las fracciones de las estelas: lo que deja el nodo
  anterior de la lista, sin desensamblar. No reproducido: la entrada `0A8502` (título desde la demostración, con `0A8552`
  y `0A85C4`: en ninguna captura), `0CA624` / `0A872E`.
- **Pruebas** (`ENDINGS` en `make check`; `test_neo_ending.c` admite ahora **varias raíces**: toda vida que no nace de
  otra es una raíz que arranca con sus bytes grabados; los nacimientos se atan por creador; las lecturas grabadas se
  ponen antes de cada nodo, `neo_end_t.on_update`): `credits` (f 62170..64500: tres páginas, 18 líneas, música, 11
  temporizadores; 7558 actualizaciones), `parade` (f 62170..65500: 13 láminas, 3858), `creditsend` (f 71690..72700: la
  última lámina, +6E = 2, y `0A453A`; 709), `title` (f 73038..73677: entero, 5926 actualizaciones, 1728 palabras de azar,
  194 borrados). Todo igual byte a byte (nodos +10..+AF, RAM 10C604, azar). Sin comparar: los dibujos (contados), la
  RAM/VRAM de entorno de `0A7FFE` / `0A8096` (13 487 palabras). Extractor (`extract_ending_fixture.py`): lee `.gz`,
  **descarta lo que hace la interrupción de vertical `0022AC..002320` dentro de la actualización de un nodo** y agrupa las
  pasadas solo por nodo repetido (una pasada cruza líneas de fotograma y, cargada, dura tres). Órdenes:
  `--range 646fe-64b02 --range a4408-a454a --root 646fe .. --root a453a` (credits),
  `--range a454c-a4744 --root a454c` (parade), `--range a7d44-a8730 --root a7ffe` (title), con `--skip 106300-106400
  --skip 106800-107900 --skip 109900-10c000 --skip 10f000-10f010` y la ventana `--from-frame/--to-frame`.
- **Nodos**: `NEO_END_NODES` 96 en host (el título tiene 47 a la vez y crea 24 de golpe), **32 en N64** (no cabe el
  título: decidir memoria antes de engancharlo).
- **Sin captura**: la lámina oscilante `0A4698` (+6E = 1): ninguna de las 43 del TAS lo es; está traducida del listado
  y **pendiente de captura** (haría falta otra semilla de `random_choice`). Las comprobaciones de cartucho `0A4496` /
  `0A8116` no se reproducen (solo actúan si fallan).
- **Datos de ROM**: `region_rom.json` grupo nuevo **`end`** (solo host, como los `mN`; `pack_mission.py` no lo empaqueta):
  `0E1446` (0x4E0), `0E61B4`, `0A7FAE` (0x50), `0A83CA`. Programas grupo `end` en `extract_soldier_programs.py`: `2A4B16`,
  `2A7368`, `2A702C`, `2A7082`, `2A7182`, `2A7224`, `2A72C6`. En N64 el código de créditos y título no se compila (`MSX_CREDITS` lo enciende); ni datos ni código nuevos en la imagen.
- **Traza Q1**: comprimida en `captures-m6/fam/Q1/boss-trace.txt.gz` (29 MB; la cruda de 254 MB, borrada). Aún sirve
  para: fixture de familia de los espadachines `0A8C62` y su subguion `019E82` en los créditos (no hecho). **No tiene
  volcados de VRAM** (`PERIOD=0`): la validación de sprites a 0 px necesita repetir `runQ1.sh` con
  `MSX_TAS_DUMP_PERIOD`.
- (La lista de pendientes del agente R queda sustituida por la sección siguiente.)

### El final en el juego y en la consola (20-XI, agente T)

Rama `work-chars` con `origin/work-mem` fusionada (superposiciones de código; el último enemigo vuelve a estar encendido
en la consola).

**Captura `captures-m6/fam/T1`** (`runT1.sh`: como Q1, del estado `end27900` al fotograma 74 400, sin escrituras, 0
desincronizaciones; además el actor de misión `101364`, el de núcleo `101154` y el de guion `101414` en la familia,
volcados `vram/ram/palette/png` cada 30 fotogramas (482) y la RAM de trabajo de las actualizaciones 28966..28968 en
`T1/upd/`). La traza va comprimida (`boss-trace.txt.gz`, 74 MB; `extract_boss_fight.parse` la lee así). `Q1` (su
`.gz`) borrada: todo lo que daba sale de T1.

**Cronología de los dos actores** (actualizaciones del actor de misión desde `end27900`; del listado y de T1):

| Act. | Rutina | Qué hace |
| --- | --- | --- |
| 441 | `018916` → `01893E..0189E8` | fin del fundido del fin de misión: 42F4 = 6 ≥ 6; `0CAE6C`, 430E = FF, 7003 = 0, `0027C4` sobre 10FDB6/7, `004192` (todo borrado), `016502`, `015418`, `01AA28(0)` y `0570A0`; espera su `+60` en `0189F2` |
| 1067 | `0189FA` → `0182A4` | misión 0xC, registro 0 = `017C54` (tipo 0, entrada y salida de clase 0, descriptor `01709E`, entrada `+0E` negativa: sin jugadores); `0183D0` carga la etapa 0x2C. **`0180AA` → `08BF5C` saca una palabra de azar** (10C5EA = r & 7) antes de que el guion haga sus dos elecciones (RAM 100060, luego 100062 / 100064) |
| 1068..6248 | `018AC6` | espera el bit 7 de `+5F` del guion (su op 3); 5180 actualizaciones |
| 6248 | `018B18` | `0C8EAA`, fundido a negro en 0x1E pasos, `+7C` = 0x1E |
| 6278 | `018B40` | `004192`, guion borrado, `016502`, `03D4EE`, capas `017EB0`, lista `017EE6`, fundido de vuelta en 0x1E, **`01959C`** (etapa 0x2C: escribe «PEACE FOREVER!», cadena `019582`, un glifo por actualización tras 16), `+7C` = 0x64 |
| 6378 | `018BC2` | `0C8EAA`; la sonda de protección (10F00C += 3EF) y `004188`: el actor de misión se borra |
| 6379 (núcleo) | `016D18` | `0040AC`: el de misión ya no está → `016D80`, `0CAC24` (la tarjeta de memoria por la BIOS `C00468`: `+60` = FF al instante en la captura), `016502`, `012244`, **`057E00`**; espera en `016D72` |
| 6486 | `016D72` | `+60` → `C00444` (la BIOS), que vuelve por `0166AA` |
| 6487 / 6488 | `0166AA` / `0166C8` | una actualización de espera (`009BAA`), luego `03D4AC`, `002976(0)`, `017FD0`, `016502`, `016D80` y **`0A7FFE`**; espera en `01677A` |
| 6775 | `01677A` → `0167A0` | la demostración (sección 20) |

**Hecho y probado en el anfitrión** (`make check`):
- `neo_mission.c`: `01891E` con 42F4 ≥ 6 deja `NEO_MISSION_GAME_END` (ya no es una parada); `neo_mission_start(0xC)`
  da el registro `017C54` (blobs `017D74`, `017C54`); un registro con `+0E` negativo no crea letras ni flecha ni
  llama a `015422` (`01862E`); con la etapa 0x2C el bucle es `018AC6`. `test_neo_mission` lo comprueba con los valores
  de T1 (42F4 = 6, 42F6 = 0x2C, 42F8 = 0xC, 42FA = 0; `0184B4` y a la siguiente `018AC6`).
- `gameplay.c` («The game's ending»): `game_end_start` (`01893E`), `MSX_END_RESULTS` (espera de `0189F2` y `0189FA`),
  `credits_update` (las fases 0..8 de la tabla; el `016502` de la salida de clase 0, `0184D8`), `ending_actors`. La RAM
  del original para los actores del final son 452 bytes (`end_mem`: 10C340..10C47F, 10F000..10F05F, 10FDB0..10FDBF,
  10E9E0..10E9EF, 100000, 100002, 10C2E6, 10C604); un acceso fuera se cuenta (`end_mem_stray`). La tabla de récords
  10F010 arranca con su contenido de fábrica, **ROM `0579D6`** (0x50 bytes, lo que lee el TAS); prisioneros 10E9E8 =
  `ga_flags.tally` (`0C8CC2`), continuaciones 10E9E4 = las del port.
- **Los créditos corren en el mundo nativo**: `neo_region_tick` despacha `0646FE..064AE0` y `0A4408..0A4724`
  (`neo_ending_world_routine`) con la RAM del final en `e->end_ram`; el guion de la sección 22 crea las dos figuras
  `0A8C62`, cada una su subguion `019E72` (registros 4..12 de la tabla `00087A`: decodificados en
  `data/ending_script.json` con `decode_ending_subscripts.py`, de ahí salen las entradas de la tabla de creación
  `0197DC`; el intérprete ya no corta un subguion a 0x80 bytes). **`tools/test_neo_credits.c`** (familia de las `0A8C62`
  con sus subguiones y toda su descendencia; `extract_credits_world.py` sobre T1, `data/credits_world_fixture.json.gz`):
  el mundo arranca la etapa como el juego, con el anillo de azar de la actualización 28966, y **las 5183 pasadas de la
  etapa dan lo mismo que MAME: 65 060 actualizaciones con los bytes +10..+AF iguales antes y después, 187 nacimientos
  atados a las vidas de MAME (4290 bytes escritos al nacer iguales), 48 243 dibujos, y el guion acaba (op 3) en la
  pasada en que el actor de misión sale de `018AC6`**. No se comparan los bytes de los nodos `019E72` / `019E82` (su
  estado vive en el intérprete nativo): sí su sitio en la lista y sus nacimientos.
- `neo_ending.c`: `01959C` traducida (fixture `peace`: 30 actualizaciones, 56 palabras de VRAM iguales). Su `tick`
  recibe la lista y la RAM por el entorno (`e->end_list`, `e->end_ram`).
- **`009CCE` en el dibujo**: no hacía falta desensamblar un lector nuevo: es la mezcla `005EA0` del vblank, que ya
  estaba en `tools/palette_mix.py`. Como los créditos mezclan 288 paletas de página y las de las láminas hacia la
  paleta 1 con 31 pesos, la consola la calcula (`src/neo_palmix_run.h`, `neo_palmix_mix`) con las palabras de ROM de
  las dos paletas (recurso `8000 | n` del paquete) y la tabla `0051D8`. `tools/test_palmix_run.py` (con la ROM): la
  tabla de conversión `224000` es la identidad de los tres canales para las 32 768 palabras (rojo 10..14, verde 5..9,
  azul 0..4; nunca el bit oscuro), `neo_palmix_dac` es la escalera de resistencias de las exportaciones, y **8730
  casos (291 pares × 30 pesos) iguales a `Mixer.mix`**. `palette_mix.py --verify T1`: **3384 ranuras con destino
  iguales a la RAM de paleta de MAME durante el final, 0 distintas** (695 sin pareja, no comparables). Los dibujos del
  mundo llevan peso y destino (`neo_world_draw_t.mix_weight / mix_target`, 24 ranuras `mixes`), los de la lista del
  final también (`neo_end_slot_level`, las piezas del título hacia 0x159).

**Consola**:
- **Superposición**: `neo_ending` es la imagen 4 (20 528 B); el programa principal solo guarda sus pruebas de rutina.
  `neo_m6boss` con la muerte, la huida y el combate de fondo son **62 240 B** (era 43 824): `MSX_OVERLAY_BYTES` =
  62 240. Imagen del programa 2 232 224 B (`text` 1 315 176, `bss` 350 124); **ares: caché de escena 3344 KB, 209
  ranuras (suelo 3332 KB / 208)**: queda una ranura de margen.
- **Los 96 nodos del título**: la lista del final tiene 32 nodos propios (letras, cuenta, resultados, ranking) y puede
  correr sobre nodos prestados (`neo_end_lend`): el título `0A7FFE` usa los 96 actores genéricos del mundo, parados
  desde `018B40`. Sin memoria nueva.
- **Paquete propio `romfs/mission13.pack`** (misión 0xC; privado, no va en git): etapa `01709E` exportada de la ROM
  (`export_stage_rom.py 1709e`: rejilla 344 × 102, 645 celdas, **1280 celdas observadas iguales, 0 distintas**; el mapa
  `025F22` no tiene ninguna tesela: fondo negro), guion y datos del grupo `end` (8 blobs, 7 programas) y los sprites de
  `export_rest_sprites.py --ending`: 433 descriptores (las 288 capas de las 48 páginas de `0E1446`, 63 láminas de
  `2A4B16`, título, resultados) y 590 paletas (295 de la ROM y sus 295 en palabras de ROM). **1 849 536 B, 420 trozos**
  (límite 1256). Cartucho: **66 158 592 B** (tope 67 043 328: quedan 884 736). Receta: `tools/build_packs.py 13`.
  Las cuatro variantes de página (0/4/8.., 1/5/9.., 2/6.., 3/7..: los subguiones 7..10) van todas.
- **Validación de sprites contra T1** (`T1@646fe-a4744`, 374 volcados): **10 768 156 píxeles iguales; distintos solo
  en 4 volcados (63630, 65820, 68010, 70200), el fotograma en que nace una página: su primera lista de sprites aún no
  está en la VRAM de ese volcado**. Resultados, ranking y título (`T1@56fcc-585fa`, `T1@a7ffe-a8622`, 467 volcados):
  **212 918 píxeles iguales, 0 distintos**; sus 190 entradas con zoom (el dial `0577E2`, las piezas `0A81A4` en vuelo) y
  20 autoanimadas no las pinta el rasterizador de la herramienta: **sin validar a píxel** (pendiente).
- La tabla `0051D8` va **solo en el paquete** (`"pack_only"` en `region_rom.json`: `compile_region_rom.py` no la pone
  en ninguna imagen de programa). Con ella en las imágenes del anfitrión la familia `m3jeep` dejaba de pasar: un nodo
  del jeep `0A2F2E` guarda en `+50` el valor `0053C2`, que cae dentro de esa tabla, y el arnés lo tomaba por un
  descriptor de golpe (sin el blob se lee «sin descriptor», que es lo que da MAME).
- `actor.c`: el paquete 13 se carga al empezar los resultados (pantalla en negro de `018954`), sin jugador ni HUD
  mientras dura el final (`msx_game_ending`), mezclas por fotograma (`scene_render_mixed_palette`, 24 por fotograma),
  y al acabar el título vuelve a empezar la partida (`0167A0`, la demostración, no está portada).
- **Menú de pruebas: `ENDING`** (segundo elemento; los demás no se mueven): arranca en `01893E`.
  **`tools/test_ending_ares.py`**: recorre resultados → créditos → última pantalla → ranking → título sin tocar el
  mando y exige las cuentas de actualizaciones del TAS entre pasos (5181, 30, 100, 1, 107, 1, 1, 287), que el
  cartucho escribe en su registro (no depende de la velocidad del emulador), sin paradas ni dibujos sin descriptor.
  Está en `tools/regression_ares.py` como `ending` (unos 25 minutos de reloj aquí). **Resultado (20-XI, cartucho de
  66 158 592 B): PASS** — pasos (5181, 30, 100, 1, 107, 1, 1, 287) = TAS, pantallas 1..6 en orden, 0 paradas, 0 accesos
  fuera de la RAM del final, 0 dibujos sin descriptor ni paleta, la superposición 4 cargada una vez (20 528 B en
  4,0 ms), el título sobre los nodos prestados, sin aserciones. (La primera pasada daba 5183: el guion hacía su
  primera pasada de lista una actualización tarde; ahora la hace en la carga de la etapa, como `0183D0` en la pasada de
  `0189FA`.)

**Estado de las puertas (20-XI, `ab79392`)**: `make check` verde (328 PASS, 0 FAIL; las dos pruebas de mezcla de
paleta se saltan sin `MSX_ROM_ZIP` y pasan a mano con la ROM), `make -j2` verde, `test_ending_ares.py` PASS dos veces
con los mismos números de actualización (1, 5182, 5212, 5312, 5313, 5420, 5421, 5422, 5709) y `test_mission6_ares.py
--stage 3` PASS (caché 3344 KB; superposición 3 de 62 240 B en 12 ms). La matriz completa de `regression_ares.py` no
se ha pasado en esta sesión.

**Pendiente (por orden)**:
1. **Fondos de teselas** de los resultados (`056EF4`), la última pantalla (`017EB0` / `017EE6`), el ranking y el
   título (`0A7D4A`, `0A7D52`): son tablas `01AA28` / listas `0C81A6` con mapas propios; hoy esas pantallas salen con
   sus sprites y su texto sobre negro. Camino: etapas adicionales del paquete 13 con `export_stage_rom.py` (hay que
   darle la lista directamente; hoy parte de un descriptor) y que `ending_setup` pida la etapa a `actor.c`.
   Con ellos van los textos que escriben esos reinicios de entorno y no las rutinas traducidas: en el título,
   `0A863C` (`03E2A8`, `0C81A6`, `03CF60`: el © y «INSERT COIN») y `017FD0`. Referencia: `T1/game72690.png` (la arena
   con «GAME OVER» y «PEACE FOREVER!»), `game72900.png` (ranking sobre el plano), `game73110.png` / `game73380.png`.
2. **Paletas de la capa fija**: comprobadas con los volcados de T1. Los textos de resultados, última pantalla y
   ranking usan las ranuras 0 y 4, **iguales a `hud-palette.bin`** (en los créditos, la última pantalla y el ranking
   las 16 ranuras lo son; en los resultados cambia la 10, sin uso en el texto). Los del título usan la 6 y la 7, que
   allí son otras (cambian las 5..10): van con sus textos, pendiente 1.
3. **Sprites con zoom de resultados y título a 0 px** (el encogido del port es una aproximación del de la LSPC, como en el resto del juego) y **la lámina oscilante `0A4698`**: la variante de láminas la
   elige `0029C6` al empezar la etapa (alternativas 7..10 del segundo `random_choice`); el TAS da una sin láminas
   oscilantes y, sin escrituras en memoria, cambiarla exige otra partida que llegue a los créditos con otro número de
   llamadas al azar. No capturada.
4. **La palabra de azar de `08BF5C`** en todas las etapas (hoy solo en la 0x2C): cambia el azar de las misiones en la
   consola respecto a hoy (habría que rehacer la línea base de `regression_ares.py`); 10C5EA no está modelada.
5. **Música**: 0x2E (resultados) y 0x31 (créditos) se piden por `0120F8` como las demás; la orden `0A453A` (`01228C`)
   queda en `ending_audio_command` sin consumidor. Sin comprobar que el paquete de audio las tenga.
6. **Cadena desde el jefe en ares**: `ENDING` empieza en `01893E`. Jefe → huida → cuenta están en las familias
   `m6boss6..8` y el último enemigo corre ya en la consola, pero no hay prueba de ares que lo venza (mando ciego).
7. `0CAC24` (tarjeta de memoria) y `016572` (línea de créditos con 10FD91 bit 0) no se reproducen.
8. **Entrada de nombre `05812A`** (cuando la puntuación entra en la tabla): el final recibe A, B y C (10C35C / 10C35D)
   pero no la palanca (bits 2 / 3 de arriba / abajo: `msx_game_update` no los pasa): la letra no se puede cambiar en
   la consola; acaba con A o por tiempo, como en el original. `ENDING` (puntuación 0) no pasa por ella.

### Fondos, textos y entradas del final (20-XI, agente U)

**Qué pantallas tienen mapa** (del listado; `tools/export_ending_layers.py` lo lee de la ROM):

| Pantalla | Reinicio | Tabla `01AA28` | Lista `0C81A6` | Capas |
| --- | --- | --- | --- | --- |
| Resultados `0570A0` | `056FCC` | `056EF4`: ninguna capa | no hay | el plano es el dial `0577E2` (sprites con zoom) |
| Última pantalla | `018B40` | `017EB0`: `10C72E` (sprites 1..32) | `017EE6`: paletas `017ED8` (ranuras 10/11 ← 126/127), mapa `017EC2` (20 × 16 bloques, P2 `300000`) | 1 |
| Ranking `057E00` | `057F34` | `0579AA`: ninguna capa | `0579BA` (solo `01AA28`, `0x14`, `002802`) | el plano es su sprite (paleta E1) |
| Título, piezas `0A7FFE` | `002976`, `01AA28(0A7D4A)` | ninguna capa | no hay | negro |
| Título, logotipo `0A8096` | `0A863C` | `0A7D52`: ventana de 40 sprites (1..40), `10E83E` (41..), `10D7B6` (73..), `10C72E` (105..) | `0A7F02` con RAM 100001 ≠ 0 (`0A7E5E` si es 0): paletas `0A7D7E`, mapas `0A7E24` / `0A7DF8` / `0A7E0E` | 3 |

- RAM 100001 es el país de la BIOS (`00240E`: `10FD83`; 2 en todas las capturas). Con 0 la lista `0A7E5E` muestra las
  filas 13 y 14 del mapa de `10E83E` (60 bloques más) y no escribe el texto; con ≠ 0 la lista `0A7F02` copia solo 20 × 13
  bloques (`0C73CE`) y `0A863C` escribe la línea `0A7E3A` (`03E2A8`) y el bloque de 6 × 3 teselas `6E5A` en `709A`
  (`03CF60`). El port es la placa europea de las capturas: `0A7F02`.
- **Exportación**: las celdas van en la etapa de los créditos del paquete 13 (su mapa propio `025F22` no tiene
  teselas) como **páginas de x**: última pantalla en `0x2000`, logotipo en `0x4000`; las cuatro reglas de capa del
  paquete 13 son «nunca» y `actor.c` enseña la página con el desplazamiento propio de la capa (las capas no se mueven:
  op `0x14` con 0,0 y factores 0). 675 celdas, 572 imágenes, 7 paletas (de la ROM, `palette_mix.Mixer.plain`).
  Paquete 13: **1 902 912 B, 425 trozos** (era 1 849 536 / 420).
- **Validación contra T1** (`build/ending_layers_report.json`): última pantalla, 6 volcados (72630..72780): **1920
  palabras de bloque y 430 080 píxeles iguales, 0 distintos**; colores de ROM iguales a la RAM de paleta en 5 (72630 está
  en el fundido). Logotipo, 10 volcados (73380..73650): **9000 palabras y 569 050 píxeles iguales, 0 distintos**; colores
  iguales en 9 (73380 en el fundido desde blanco), también los de las ranuras fijas 6 / 7 (paletas 174 / 890).
- **Orden de dibujo del logotipo**: la tabla `0A7D52` reserva primero la ventana de las entradas con `+15` bit 2 (RAM
  10B95C = 0), luego las capas y encima la otra ventana (10B954 = 0x88). `actor.c`: fondo, entradas con bit 2 (la
  lámina `0A8602`), las tres capas, el resto (el programa `2A7368`).
- **Textos del título**: escritos por el nodo `0A8096` (`neo_ending.c`), no por el entorno. El fixture `title` compara
  ahora las palabras de la capa fija de un reinicio: **43 palabras iguales en orden** (25 de la línea, 18 del bloque).
  Las ranuras fijas 6 y 7 siguen el fundido (T1 73380: blanqueadas con las de las capas; las 0..4 no):
  `hud_renderer_slot`. «INSERT COIN» y «CREDIT 0» son la línea de monedas de la MVS (`016572` / `016648`): el port
  no tiene línea de monedas; no se escriben.
- **En ares** (`test_ending_ares.py`, capturas cada 3 s tras los créditos): última pantalla con la arena y el casco,
  logotipo con sus tres capas sobre la lámina y los textos.

**Entrada de nombre `05812A`**: en la ROM la letra se cambia con **derecha / izquierda** (bits 3 / 2 del byte de
repetición 10C35E, `058554`), no con arriba / abajo. El bloque de puerto del jugador 1 (`03CC10..03CC54`, una vez por
actualización en el bucle principal `003A5E`: anterior, mantenido, nuevas pulsaciones, repetición con la cuenta de
10C380 / 10C381 = 0x20 / 4 que pone `03CAB4`) está en `neo_end_port_block` y el final lo recibe entero (palanca
incluida) en cada actualización (`gameplay.c end_inputs`). **Captura `captures-m6/fam/NE1`** (`runNE1.sh`: desde
`end27900` con entradas en las actualizaciones 28600..28835, `updates_ne1.txt`; nombre «DAB»): familia `name` (4560
actualizaciones, 8604 palabras de VRAM, 506 bytes de RAM iguales) y `end_ports` (**249 actualizaciones de la entrada de
nombre: nuevas pulsaciones, byte de repetición y cuenta iguales a MAME**, 16 pulsaciones, 8 repeticiones).

**Audio**: `01228C` (fundido `0A xx` del controlador) de `0571BA` y `0A453A` sale ya como palabra `0Axx` (para la música,
como los jefes). **Las músicas 0x2E (resultados) y 0x31 (créditos) no están en el paquete de audio.** Renderizadas
por el procedimiento de `AUDIO_JUEGO.md` (`data/sound_map.json`: `music_results`, `music_credits`) y empaquetadas en
un directorio aparte: 0x2E bucle de 48,10 s (similitud 0,927) = 1 299 088 B + introducción 162 100 B; 0x31 de una
vez, 163,31 s = 4 409 812 B. **Total 5 871 000 B; al cartucho le quedan 786 432 B: no caben.** Sin empaquetar
(decisión del coordinador: subir el tope o quitar otra cosa).

**Los cuatro volcados (63630, 65820, 68010, 70200)**: demostrado con la captura sola
(`tools/check_credits_birth_dumps.py`, `build/credits_birth_dumps_report.json`). En cada uno hay **6 nodos `064AC0` (las
seis líneas de una página) cuya primera entrada de lista de sprites se escribe en la pasada en curso**, y **ninguna
palabra de tesela de los sprites encendidos de ese volcado lleva sus ranuras de paleta** (0 de 0; en el volcado
siguiente, 541, 586, 790 y 676): la lista que construye una pasada llega a la VRAM en el vblank siguiente. De los 12
volcados de control, 11 no tienen nacimientos; el otro (70170) tiene una lámina `0A454C` recién nacida fuera de la
pantalla (primera entrada en y = 260 → fila 252), tampoco en VRAM y sin píxeles que comparar. No hay nada que corregir
en el port: es el fotograma de retraso del volcado.

**Tras el título** (del listado, `0166F8..0169A0`): el actor de núcleo, con `10FDAE` = 2, hace título `0A7FFE` →
`0167A0` (demostración: `10F003` = 3, `017F2C` con el registro `01678C + 10·(10F00E & 1)`, jugadores `0C10D8`,
reproducción de entradas grabadas `03CAD2` / tabla `03CF06`, 900 actualizaciones como mucho) → ranking `057E00` →
`C00444` (la BIOS, que vuelve a `0166AA`). En cada espera `0168DA` llama a `016648` (la línea de monedas) y, con
RAM 100088 = 2 (inicio pulsado), va a `016B9A` en MVS o, en casa (`10FD82` = 0), a `01691E`: el menú casero `066A1C`.
**Pendiente**: el port no tiene ni la demostración (hace falta el reproductor de entradas `03CCB2..03CE8E` y sus dos
grabaciones) ni el menú `066A1C`, y sus películas de arranque (título, instrucciones, selección) solo se reproducen
antes de reservar la memoria del juego; por eso al acabar el título sigue empezando la partida (`reset_game`), igual
que tras un GAME OVER.

**Ranking**: su plano (nodo `0580A2`, descriptor `1D932C`) se lista en y = `FF80`; la LSPC toma la palabra módulo 512
líneas (como `0x180`; T1, VRAM 72900: sprites en las líneas 8 / 48). `draw_end` tomaba la palabra con signo y lo
dibujaba 512 líneas fuera: corregido (`actor.c`), el plano sale tras los nombres.

**Sprites con zoom: el encogido de la LSPC, validado en el anfitrión (paso 1 de 2).** `tools/check_lspc_shrink.py`
dibuja cada sprite de un volcado de VRAM como la LSPC y lo compara con la captura de pantalla de MAME del mismo
fotograma (`game<n>.png`, líneas de raster 16..239).
- **Vertical**: la ROM L0 de la BIOS (`000-lo.lo` de `neogeo.zip`), byte `(zoom_y << 8) | línea` = tesela (nibble
  alto) y línea de la tesela; con el bit 8 de la línea, espejo (línea ^ 0xFF, tesela ^ 0x1F, línea ^ 0xF); con tamaño
  ≥ 0x21 la altura encogida se repite plegándose; una línea cuya tesela es ≥ el tamaño no se dibuja.
- **Horizontal**: qué columnas de las 16 de una tesela quedan con cada nibble de SCB2 (el sprite encadenado va en
  x + nibble + 1). No está en ninguna ROM (es lógica del chip) y no hay fuente de MAME en disco: **la tabla sale de
  las capturas** (`--derive`). Para cada nibble 0..14 y cada columna de salida se descartan las columnas de origen que
  refuta algún píxel de la captura (pen opaco de otro color), usando solo píxeles cuyo sprite más alto por caja
  (x .. x + nibble, líneas de la L0: sin suponer tabla) es ese, y fuera de la capa fija. Con los volcados de
  resultados de T1 (60000..62580) **cada (nibble, columna) queda con una sola columna**, y cada nibble 0..14 queda
  determinado también con un solo volcado (60930: 0, 1, 14; 61290: 2, 3; 61020: 4, 5; 60960: 6, 7, 13; 61260: 8, 12;
  61620: 9, 10, 11). El nibble 15 es el sprite entero (los sprites sin zoom de la comparación). La tabla escrita en
  `COLUMNS` tiene que ser igual a la derivada (`--derive` falla si no). Ningún nibble queda sin captura.
- **Comparación**: T1, los 57 volcados con sprites con zoom y captura (resultados y título): **347 427 píxeles de
  sprites con zoom iguales y 7 distintos**; están los 15 nibbles, todos con 0 distintos salvo el 6, que tiene 7. De los
  sprites con zoom vertical, 24 810 son iguales y 7 distintos. Los 7, y 2 469 píxeles de sprites sin zoom, caen en los
  volcados 73080..73350 del fundido del título. **Captura TF** (`tools/tas_updates.lua` con
  `MSX_TAS_DUMP_PERIOD=1 MSX_TAS_DUMP_FRAMES=73050-73400`, desde `end27900` como `runT1.sh`, sin familia: VRAM, RAM,
  paleta e imagen en cada fotograma; en los fotogramas comunes, VRAM y paleta iguales byte a byte a T1; guardada en
  `scratchpad/agV/TF`, 351 fotogramas). Con ella **quedan descartadas** la paleta de otro fotograma (paleta f−1 y f−2
  contra la imagen f: 73080, 73260 y 73350 igual o peor) y la VRAM de otro fotograma (f−1 y f+1: peor; de los 1448 píxeles
  distintos de 73260, ninguno coincide con la VRAM f−1). Lo que sí se ve: en 73260 los distintos son de la cadena de
  sprites sin zoom 345..348 (y 358 / 359), paleta 16, tamaño 5, sin autoanimación, en las líneas 36..64. Su posición es
  la correcta (desplazarla ±16 líneas / ±4 px solo empeora), y en el ~40 % de sus píxeles opacos la captura muestra otro
  pen de la misma fila de paleta (4 / 5 en vez de 1). Los datos de C-ROM de esas teselas (0x3055B..) son los del zip
  (`250-c3/c4`). Descartado también (20-XI, con TF): **el límite de 96 sprites por línea** (en las líneas 36 / 50 / 64
  hay 15 / 17 / 26 sprites cuya altura las cubre, y 345..348 son del 2.º al 8.º); **el orden de pintado** (en esos
  píxeles solo hay un sprite opaco en la VRAM, el de la cadena; ninguna otra tesela de la cadena, de −40 a +40, da
  los colores de la captura); **el otro banco de paleta** (`MSX_TAS_DUMP_PALRAM=1` vuelca los dos bancos de
  `m_paletteram`: el banco 0 es el volcado y da los mismos 1448 distintos; el banco 1 no da casi ninguno igual). El
  port no modela el límite de 96 sprites por línea: su renderizador dibuja todas las partes. Y **los bits altos de la
  tesela** (las teselas de 345..348 con ±0x10000, ±0x20000, ±0x40000, con los bits 4..7 del atributo y sin ellos:
  ninguna da 0 distintos; la mejor es la decodificación actual, p. ej. 348: 728 iguales / 355 distintos frente a 119 /
  1161 la mejor otra). **Pendiente, cerrado por ahora** (1448 px de un fotograma del fundido): la causa no está
  encontrada. TF está borrada; para rehacerla: `MSX_TAS_SCRIPT=tas_updates.lua
  MSX_TAS_UPD_PLAY=/home/user/captures-tas/updates251.txt MSX_TAS_STATES=/home/user/captures-m6/states
  MSX_TAS_UPD_LOAD=end27900:27900 MSX_TAS_CAPTURE_FROM=27900 MSX_TAS_UPD_LAST=35100 MSX_TAS_LAST=73410
  MSX_TAS_DUMP_PERIOD=1 MSX_TAS_DUMP_FRAMES=73050-73400 [MSX_TAS_DUMP_PALRAM=1] sh tools/run_tas_play.sh OUT` (unos 4
  minutos, 67 MB). No es el encogido: esos sprites no tienen zoom, y los
  únicos 7 px con zoom (73350) caen en la misma zona y fotogramas.
- Se excluyen las teselas fijas distintas de 0x0FF: la 0 no es transparente, porque las columnas 0 y 39, todas de
  tesela 0, salen negras sobre los sprites. También se excluyen los sprites autoanimados. Informe:
  `build/lspc_shrink_report.json`.
- **`make check`** (versión rápida, unos 15 s): la tabla derivada de nuevo de los seis volcados de arriba, y 61500 y
  73200 (con zoom vertical) comparados con 0 distintos. Se salta sin `neogeo.zip` (`MSX_BIOS_ZIP`) o sin la captura.

**Escritura de la lista de sprites en la VRAM (zoom incluido), traducida y comprobada palabra a palabra (parte A).**
La lista que llena `00A03E` (entrada: +0 x, +2 y, +4 zoom x / zoom y = +12 / +13 del actor, +6 paleta << 8 |
atributo & 7, +8 descriptor de P2, banco = bits 20..23 − 1) la ordena `0113EA` (radix estable por la palabra +2 del
índice) y la escriben dos pasadas: **`011D04`** al final de la pasada del juego (SCB1/2/4 de los sprites que la pantalla
no enseña: hacia delante desde el principio de la ventana `3954` / `395C` hasta su +6, o hacia atrás desde el final
hasta su +4; luego invierte 1946) y **`011E60`** en el vblank (SCB3 de cada columna y SCB1/2/4 de los solapados;
después `0103A6` / `010BC8` esconden el resto de la ventana con SCB3 = 8000). Cada entrada va a uno de 32 escritores
de columnas (`00A086` + 4·((zoom ≠ FFFF) << 2 | atributo & 3), +0x20 en el vblank, +0x40 hacia atrás). Con zoom
(`00AB7A` / `00C3EE` / …): por columna, acumulador `d4 = (d4 & FF) + 16·(zx+1) − 256` (inicio: byte bajo de
x_desc·(zx+1)); negativo = columna sin anchura (no se escribe); si no, **SCB2 = (d4 & FF00) | zoom y** (nibble = anchura
− 1), **SCB4 = x << 7** con x = x_entrada + ⌊x_desc·(zx+1)/256⌋ más las anchuras anteriores (con espejo: x_desc = ~x_desc
y la anchura se resta antes), **SCB3 = (y_entrada + ⌊y_desc·(zy+1)/256⌋) << 7 | ⌈filas·(zy+1)/256⌉**, y SCB1 con las
filas del descriptor y ceros en el resto de las 32. Columnas con x fuera de −8..311 no se escriben.
`tools/check_zoom_scb.py` es la traducción (las 32 variantes como una función con sus parámetros) y la familia: de
cada volcado justo tras `011D04` al siguiente de la otra lista, VRAM(f) + `011E60`(lista de f) + `011D04`(lista de g)
= VRAM(g) en SCB1..SCB4 de los sprites de las ventanas, y las +4 / +6 de la RAM. **Capturas ZC** (resultados,
fotogramas 61490..61530) y **ZT** (título, 73150..73410), volcados de cada fotograma: receta de TF de arriba con
`MSX_TAS_DUMP_FRAMES=61490-61530` (`MSX_TAS_LAST=73410`) y `73150-73410` (`MSX_TAS_LAST=73420`); guardadas (RAM y VRAM)
en `build/captures/ZC` y `ZT` de este árbol. **Resultado: 121 pares, 2 958 452 palabras iguales, 0 distintas**; 239
entradas con zoom (80 sin espejo y 40 con espejo horizontal en el dial; 159 sin espejo en el título) y 14 957 sin zoom
de las cuatro variantes. Variantes con zoom y espejo vertical (6 / 7): **sin captura** (no salen en ZC ni ZT;
traducidas igual que las demás, sin comprobar). En `make check` (se salta sin las capturas o la ROM).

**Paso 2, pendiente**: el port (`actor.c`, `scene_render_sprite_scaled`) sigue escalando cada parte con un
rectángulo del RDP; falta dibujar sprite a sprite de 16 px con estas columnas y líneas.

**Otros**: `romfs/scene.pack` de este árbol estaba atrasado respecto a sus fuentes (8285 páginas; las fuentes dan
8301, como ws-logo y ws-m4) y `make check` fallaba en la prueba de compresión; rehecho con `tools/pack_scene.py` y
`build_packs.py`.

**Números (20-XI, agente U)**: cartucho **66 256 896 B** (tope 67 043 328: quedan 786 432); programa `text`
1 319 592, `bss` 354 196; superposición `neo_ending` 20 752 B (región 62 240); caché de escena en ares 3344 KB, 209
ranuras (sin cambio). Receta del paquete 13: `export_stage_rom.py 1709e …` (como antes), `export_rest_sprites.py
--ending`, **`export_ending_layers.py --map filesystem/mission13/s1/map.json --capture captures-m6/fam/T1 --rest
filesystem/mission13/rest-native.json`**, `build_packs.py 13`.
