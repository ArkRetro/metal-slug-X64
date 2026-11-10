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

- `make -j4`: RC 0. `make check`: **220 PASS**, RC 0 (los 202 de antes y 18 de la M6).
- `tools/test_m1m2_ares.py` sobre un `mslugx-m1end.z64` recién hecho (`rm -rf build/m1end`): `RESULT: PASS` (188
  fotos).
- ares, `mslugx-explorer.z64`, menú → `MISSION 6 STAGE n` (`tools/test_mission6_ares.py`): **etapas 2 y 3 PASS**
  (Marco recorre el mapa entero con sus fondos y su música); **etapa 1 no**: mapa, fondos, música y entrada bien, pero
  Marco cae al agua desde x 160 (balsas, puente y segundo terreno sin portar; arriba, «Recorrido en ares»). Es la
  parada explícita más visible de la fase 1; no se ha tapado.

`/home/user/ws-logo` tiene `romfs/mission6.pack` (`build_packs.py 6`), `filesystem/mission6/` y
`filesystem/audio/music_mission6_boss*.wav64` con su entrada en `manifest.json`; no se ha tocado nada del otro agente.

**Para seguir** (otra sesión): árbol `/home/user/ws-m6` (rama `work-m6`), `romfs/` propio, `filesystem/` de enlaces a
`ws-logo/filesystem/*` salvo `mission4/s1` (como en la M5, hasta que se fusione la fase 2 de la M4); estados MAME en
`/home/user/captures-m6/states`; Xvfb propio (`:76` aquí); datos de ares en una copia de `data146` con
`L-Bumper: 0x1/0/38;;` en `settings.bml` (sin esa tecla el menú de pruebas no se abre: «select chosen early»).

## Pendiente (fase 2, por orden)

1. **Etapa 1**: qué sostiene al jugador sobre el agua (actores de la cabeza de la lista: `0BCE2C`, `0BD18E`,
   `0BD2BA`, `0BD2CE`, `0BD822`/`0BD836`/`0BD84A`, `0BDA4E`, `0BDAB8` con su cuerpo `2D3FEE`); trazarlo con una sonda
   en el padre `+38` del jugador desde `sec09_292683`.
2. **Segundo objeto de terreno `10D7B6`**: rejilla en el paquete (hoy no cabe en el registro de etapa de 128 bytes:
   quedan 2), objeto en la lista del núcleo `01382A` (como el tren de la M3) y captura de `+32/+34` para explicar las
   3066 lecturas con el origen movido; su dueño y los sprites del puente.
3. **Capa `10E83E` movida por su dueño** en el progreso 1056 (los 300 bloques de `0BC728`) y las banderas de capa.
4. **Parches y sellos de la etapa 2**: 4 parches `0C70FC` (submapa `0BDBB2`; 304505, 304596, 304973, 309159) sin
   `patches.json` todavía, y 4 sellos `0C88B0` (313437, 313441, 314099, 314129; plantillas `0A42C4`, `0E1A2C`,
   `0E1A3A`).
5. **Actores**: 136 constructores sin portar (123 propios), 3606 creaciones ([MISION6_ACTORES.md](MISION6_ACTORES.md));
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
