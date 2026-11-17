# Animaciones de daño y muerte: auditoría (agente D, rama `work-audit`)

Pregunta del usuario: ¿están bien extraídas del original las animaciones de
daño y muerte de los personajes jugables (también en sus formas) y de los
vehículos jugables? Para cada uno: rutinas 68K, estado en el port, familia o
prueba bit a bit contra MAME 0.264, sprites/paletas validados contra captura,
sonido y lo pendiente. Todo de la ROM del usuario (`mslugx.zip`) y de
capturas de MAME; desensamblado con capstone sobre `250-p1.p1`.

## 1. Cómo decide la ROM la muerte del jugador

Una sola rutina, `0C2956`, para todo golpe (`0C3702`: `+5F` bit 7; la
respiración de la momia `0C36F8`; el fin de tiempo `0C373E`; el vehículo
`0C3118`). La caída por la pantalla es `0C2D6A`. Las dos piden la voz
`0C294E[+6E]` (1123 Marco, 1087 Tarma, 1125 Eri, 114E Fio) y eligen **primero
por la forma `+93`, luego por el tipo `+5A`**:

| Rama | Condición | Programa (tabla por `+6E`) | Paletas (`009EA8`) | Movimiento / fin |
| --- | --- | --- | --- | --- |
| `0C2A36` | forma 2 (gordo) | `0C0518` (Marco `25CF70`) por `0C0FB6` | sin cambio | `+30/+34 −= >>5`, `0143EE`; al acabar el programa, fuera (`0C2F90`): sin parpadeo en el suelo |
| `0C2A06` → `0C2A36` | forma 2 y caída | ídem | ídem | antes `+34 = 0xA9000`, `+30 = ∓2` |
| `0C2C40` | forma 3 (momia), **cualquier tipo** | `0BF358` (`2501DC`) | `+19 = 0BEE08`, `+1A = 0BEDE8` | como el tipo 7 (`0C2E44`) |
| `0C2BCE` | tipo 2 | `0BF3D8` en el suelo / `0BF458` (= `0BF318`) en el aire | `0BEDD0` | `+80 = 0x100`, sin salto |
| `0C2C7A` | tipo 7 | `0BF358` | `+19 = +1A = 0BEDE8` (quemado) | `+80 = 0x80`, salto `0xA9000` |
| `0C2ACC` | tipo 8 | `0BF398` | `0BEDD8`/`0BEDD0`, `0BEDE0`, `0x51` | propio (`0C2B8A`) |
| `0C2A70` | tipo 0x12 en el suelo | `0BF418` | — | andando (`014594`) |
| `0C2CCC` | caída al agua (`03C55C`, `0191FC` = 2/3) | `0BF2D8` | `0BEDF0` | se hunde (`0C2D06`) |
| `0C2DDA` | los demás | `0BF298` suelo / `0BF318` aire | `0BEDD0` (`0BEDD8` con `10C30C`) | `+80 = 0x20`, salto `0xA9000` |

Las ramas `0C2E44`/`0C2DDA` llaman a `0C4C28` (solo Fio: el actor `0C4C64`).

## 2. Personajes jugables

| Personaje / forma | Rutinas | Port | Prueba bit a bit (MAME) | Sprites / paletas | Sonido | Pendiente |
| --- | --- | --- | --- | --- | --- | --- |
| Marco normal | `0C2956` → `0C2DDA`/`0C2BCE`/`0C2C7A`, tumbado `0C2F06`/`0C2F12`, vida nueva `0C95AE`, entrada `0C1438`/`0BF498` | nativo (`marco_death.c`) | `make check` `death`: 15 muertes de `death3`/`marco-d2` (tipos 2 en el aire, 7, 9, 0xD) y, nueva, `k2d1` (tipo 2 **en el suelo**, `0BF3D8`, 35 poses): poses, velocidades, `+26`, fin, vida nueva, entrada, reaparición: **0 distintos** | catálogo (`marco_pose_meta.h`); auditoría `marco-d2` 540 fotogramas; `k2d1` PASS 136 fotogramas, 22 descriptores, 48 328 píxeles | 1123 suena | las 2 muertes de tipo 9 de `marco-d2` excluidas de la auditoría de píxeles (sprite 32 px más arriba, sin explicar) |
| Marco gordo (forma 2) | `0C2A36` (`0C2A06` tras caída), cuerpo `0C0518`/`25CF70` | **corregido (B73)**: antes hacía la muerte delgada `0BF298` | `death`: captura `fatd1` (`--give-fat --hit 400:0`), 29 poses, 42 actualizaciones, fin y vida nueva iguales | auditoría `fatd1`: PASS 62 fotogramas, 16 descriptores, 33 568 píxeles; las 14 poses en el catálogo | 1123 | la entrada por caída `0C2A06` sin captura (traducida); Tarma/Eri/Fio gordos sin captura |
| Marco momia (forma 3) | transformación `0C17A0`/`0C182A` por el tipo 0x10 (`0C36C0`); muerte `0C2C40` para todo tipo | **ruta corregida (B74)**: antes un disparo a la momia hacía la muerte normal `0BF298` | `death`: captura `mzd1` (`--give-mummy --hit 400:0`): 68 poses, 15 dibujos tumbado, velocidades, fin; tabla de paleta de `+19` = `0BEE08` medida | auditoría `mzd1`: PASS 202 fotogramas, 31 descriptores, 270 124 píxeles, con los perfiles `0x132` y `0x56` | 1123 | **paleta** (B74): la muerte de la momia usa `0x132` (`0BEE08`) en `+19` y `0x56` en `+1A` en el mismo descriptor; el catálogo tiene una paleta por pose y no tiene `0x132`: se dibuja con la quemada |
| Tarma | las mismas, columna 1 | nativo (pack `c1.bin`) | `check-characters`: captura `c1d1` (`--character 1 --hit`), 51 poses, fin, vida nueva, entrada: **0 distintos** | auditoría `c1d1`: PASS 168 fotogramas, 21 descriptores, 93 578 píxeles | `1087`: gemelo de `1123` en el controlador Z80 (§7, AUDIO_JUEGO §19), voz 2 | tipos 2/7/aire sin captura |
| Eri | columna 2 | nativo (`c2.bin`) | `c2d1`: 57 poses, 0 distintos | `c2d1`: PASS 180 fotogramas, 19 descriptores, 69 850 píxeles | 1125 suena | tipos 2/7/aire sin captura |
| Fio | columna 3 | nativo (`c3.bin`); su actor `0C4C64` **nativo (agente E)**: `0C4C28` desde `0C2E74` (tipos que llegan a `0C2E44`), hijo tras el jugador por `01ACF2` (8 hacia delante, 0x20) y `01AC94`, programa `253322` a velocidad 0, paleta 5, `+30` = la del jugador ± 1, `0C4CBA` (`marco_death_cap_*`, `neo_effects.c`) | `c3d1`: 57 poses, 0 distintos; actor: captura `c3d2` (`--child-routines c4c64,c4cba`), `make check-core` `fio_cap`: 85 actualizaciones, 169 velocidades, 85 `+26`, cuenta final `+80`/`+5F`, desaparición: **0 distintos** | `c3d1`: PASS 180 fotogramas; sprites `1A5A6A..1A5AD8` del actor sin comprobar en los exports | 114E suena | colocación inicial (`01ACF2`) no comparada; sprites del actor |
| Marco | tipo 8 `0C2ACC` → `0C2B8A` | **nativo (agente E)**: programa `0BF398` (`25032A`, poses con paleta `+19`/`+1A`), siempre el paso aéreo `0143EE`, `+34 += (−0x16 − +34) >> 6`, `+30 −= >> 6`, `+78 = 0` en el suelo, sin rampa de `+26`; con `+5B` bit 7 el giro se queda (`0C2B68` borra el bit 1, no el 0); la llamada `05A09E` del programa crea el humo `05A0B4` (programa `25339C`, paleta 0x204, sobre el bloque `+28` del jugador, `neo_effects.c`) | `death`: captura `k8d1` (`--hit 400:8`), 118 actualizaciones, 75 poses, 15 dibujos tumbado, fin y vida nueva: **0 distintos** | auditoría `k8d1`: PASS 216 fotogramas, 16 descriptores, 102 512 píxeles; **las poses `18CF86..18CFE0` no están en el catálogo** (se mantiene la anterior) | 1123 | catálogo y paletas `+1A` = `0BEDE0` (0x53) / `+1B` = 0x51; el deslizamiento por el suelo tras aterrizar es el `0143EE` compartido del port (no comparado: la prueba usa el movimiento de la captura); humo `05A0B4`: sus 10 poses en los exports, validadas a 0 px (captura `k8s1`, §8.5) |
| Marco | tipo 0x12 `0C2A70` | **nativo (agente E)**: `040F38` crea la salpicadura `040F74` para todo tipo 0x12 (`01B220` (8, 0), antes del giro); en el suelo `0BF418` (`24FF1E`), `014594` con `+62 = 0`, sin paleta ni velocidad; en el aire `0C2DDA` | `death`: captura `k12d1` (`--hit 400:12`), 76 actualizaciones, 33 poses: **0 distintos** | auditoría `k12d1`: PASS 132 fotogramas, 22 descriptores, 53 450 píxeles; **poses `18D298..18D450` fuera del catálogo** | 1123 | catálogo; 0x12 en el aire sin captura (es `0C2DDA`, ya probada) |
| Tarma / Eri / Fio | tipos 8 y 0x12 | **parada explícita** (`death_stops`, hacen `0C2DDA`): sus programas (`250DB6`/`2509AA`, …) no están en los packs de personaje | — | — | — | ampliar el formato del pack (`export_character.py`) con las filas `0BF398`/`0BF418` |
| Todos | caída al agua `0C2CCC` | **nativo para Marco (agente H, §7)**: `marco_death_start_water`/`_water_velocity`; Tarma/Eri/Fio parada explícita (programas `24F150..` fuera de los packs) | `death`: captura `w6d2` (M6 etapa 9, `--load --update-trace`): 52 velocidades, 53 poses, fin, vida nueva: 0 distintos | sin auditoría de píxeles | 1123 | paso `0143E4` no comparado; los otros tres personajes |

`marco_death_start_form` (nuevo) elige por la forma como `0C2986/0C2990`;
`marco_death_use_fat_program` recibe de `marco_visual.c` el programa de la
fila `0C0518` del personaje elegido (papel `B2_HIT_C2A06`). La muerte del
gordo deja ver su última pose en la actualización en que acaba
(`final_draw`, `fatd1`: `128606`).

### Capturas nuevas (MAME 0.264, `run_marco_catalog.py --hit AFTER:KIND`)

Escritura de captura nueva: `AFTER` fotogramas tras el primero del plan pone
`+5A = KIND` y `+5F` bit 7 en el nodo del jugador (lo que deja `004C24` en
un golpe); la ROM hace el resto (`0C3702` → `0C2956`). Plan por defecto,
`--end-frame 3000` (personajes: `--first 2350 --end-frame 3250`),
`--dump-routines c2956-c2f96`. Fixture: `tools/extract_form_death_fixture.py`
→ `data/form_death_fixture.json` (6 muertes, junto a `data/death_fixture.json`
en `build/death_fixture.h`). Brutos borrados; quedan los `audit.json` en
`/home/user/captures-audit/*`.

```sh
python3 tools/mame/run_marco_catalog.py --rom-dir ROMS --output mzd1 --command-trace --paired-trace --give-mummy --hit 400:0 --end-frame 3000 --dump-routines c2956-c2f96
python3 tools/mame/run_marco_catalog.py ... --output fatd1 --give-fat --hit 400:0 ...
python3 tools/mame/run_marco_catalog.py ... --output c1d1 --character 1 --first 2350 --hit 400:0 --end-frame 3250 ...   # y 2, 3
python3 -I tools/extract_form_death_fixture.py --rom ROMS/mslugx.zip mzd1 fatd1 c1d1 c2d1 c3d1 --output data/form_death_fixture.json
(cd tools; python3 audit_marco_catalog.py CAP --rom mslugx.zip --output CAP/audit.json --sparse)
```

## 3. Vehículos jugables

Daño común: `0AA9E4` (de `004C24`: tipo 0x0F → salud 0, 0x0D → llena, si no
`−0x10` salvo `+5F` bit 6), ranura `10C608`, bajada `0AA7FC` (tipo 0: muere
el jinete, `0C30F0` → `0C2956`). Que el vehículo reciba la fila de clase 0
lo corrigió B69 (`test_b69` en `make check`).

| Vehículo | Rutinas de golpe / destrucción | Port | Familias (MAME, `make check`) | Sprites / paletas | Sonido | Pendiente |
| --- | --- | --- | --- | --- | --- | --- |
| SV-001 (M1, M4; `092A32`) | golpe en tierra `09351A`/`093576`, en el aire `0935A8`/`0935EC`, parpadeo `+7E` (paleta `+19`), destrucción con piloto `0938DA`/`09391E` → `093AFA` (destello `0AE1DC`, piezas `0AD9B8`, ráfaga `093BC2`, torreta `03B936`), paletas `093882` | nativo (`neo_tank.c`; B52, B59) | `tankhit` (`09351A`, `093576`), `tankkill` (`09391E`, `093AFA`), `tankair` (`0935EC`, `093576`, `09391E`, `093AFA`): iguales; ares `m4boss` 7 golpes | `rest-sprites-validation.json` (lado vehículos; diferencias atribuidas a solapes y parpadeo `+7E`, no 0) | 1034, 10E9 suenan; `1089` suena (B75, agente F); **`108A` sin política** (silencio) | paradas explícitas que no son de daño (salto, agachado, bombas, obús de tipo 5); validación de píxeles del parpadeo y la destrucción con 0 distintos |
| SV-001 (M3, copia `093C9A`) | golpes en el aire `0946FA` → `094788` → `0947CC`, destrucción `094ABA` | nativo | `m3tankair` (137396..138628, `094756`, `0947CC`, `094AFE`, `094CDA`): igual | ídem | ídem (B75) | — |
| Camel Slug (M1; `03458E`) | sube con salud 0: el primer golpe pone `+5F` bit 7 → `0351BC` (`0AA7FC(0)`: muere el jinete); el camello vuelve a esperar | nativo (`neo_slug.c`) | `slug` (camelfam 9400..10095, jinete muerto 10091): igual; ares `veh` (B69) | `slug-sprites-validation.json` (regenerado por el agente F): 38 volcados, 21 525 iguales, **0 distintos** (los 695 eran del validador antiguo, ver §6) | voz del jinete (1123 etc.) | agua `034AC0` parada explícita |
| Slugnoid (M2 E3, M5; `09692C`) | golpes en tierra `09737E`/`097396`, destrucción con jinete `0973E2` → `097414` (marcador `03533A`) → `09756A`; paleta de muerte `097EDE`/`097F52` | nativo (`neo_slugnoid.c`, B38) | `slughit` (39039/39331 golpes, 39503 destrucción) y `slugair` (agente H: golpe en el aire `097308` 39174, en tierra 39294, destrucción 39666): iguales | M2: 4047 distintos en 7 volcados (saltos, sin revalidar); M5 `slugair`: 31 volcados, 0 distintos (§7) | 1034, 10E9, `1089` (B75) | `0977DC` (torreta perdida), lista de muerte `097EDE` en espera: sin captura; pilotos de Eri, Tarma y Fio sin exportar (B69); no probado en ares bajo fuego |
| Vehículo volador de la M3 (`0AC866`) | golpe (parpadeo `+8C`, retroceso `+8E`), destrucción con jinete (paleta de muerte `+86`, piezas `0AD9B8`, destello `0AE1DC`) | nativo (`neo_m3veh.c`) | `m3veh2` (vuela hasta destruirse con su jinete): igual | MISION3_ACTORES | 1034, 1090, 10E9, `1089`; **108B sin política** | `0AE752` (índice de paleta 0xF2: trap) parada explícita |

Otros vehículos del port (`neo_m4veh.c` `0587C4`, `neo_m5tank`, trenes…) son
**enemigos**, no jugables: fuera de esta auditoría.

## 4. Fallos (BUG_MATRIX)

- **B73** muerte del gordo: corregida, `death` la prueba (`fatd1`).
- **B74** muerte de la momia: ruta corregida y probada (`mzd1`); la paleta
  `0BEE08` sigue pendiente (catálogo con una paleta por pose).
- **B75** sonidos de golpe de vehículo: `1089` renderizado, medido y con
  política (impacto, nivel 2; agente F, §6); `108A` y `108B` medidos pero sin
  política (mueven `108E` y `116E`): pendiente del controlador Z80.
- **B76** (agente E) muertes de tipo 8 y 0x12 y actor de Fio: corregidas
  para Marco (`386c5c0`) y Fio (`c79ea00`); quedan las de Tarma/Eri/Fio de
  tipos 8/0x12 y el agua.

## 5. Siguiente paso (agente E, 16-XI)

Hecho: tipos 8 y 0x12 de Marco (`386c5c0`), actor `0C4C64` de Fio
(`c79ea00`); capturas nuevas `k8d1`, `k12d1`, `c3d2` (brutos borrados,
quedan `audit.json` y `children.log` en `/home/user/captures-audit/`).
`run_marco_catalog.py --child-routines R,R` vuelca cada nodo hijo del
jugador (`children.log`). Pendiente, por orden:

1. **Catálogo de Marco** (bloquea ver bien los tipos 8/0x12 y la paleta
   `0x132` de la momia): rehacer `build_marco_poses.py` con todas las
   auditorías de `MARCO_CAMPAIGN.md` más `--extra-audit` `k8d1`, `k12d1`,
   y un perfil por ranura de paleta (`+19`/`+1A`: las poses de flags 0x10 /
   0x20 de `25032A`; `mzd1` usa `0x132` y `0x56` en el mismo descriptor).
   No se ha hecho aquí: el catálogo actual es el de `ws-logo` (10-XI) y su
   lista completa de capturas no está en este árbol. `marco_death.c` ya
   da `palettes[1..3]` = 0x63..0x65 para elegir el perfil en `actor.c`.
2. **Sonidos 1089/108A/108B y voz 1087** (B75): `1089`/`108A` no están en
   `data/sound_map.json` (llamadores `09353A`, `0935B0`, `0938DA`,
   `09471A`, `094790`, `094ABA`, `0977DC`, `0AD1F4` / `08E186`, `090A4A`,
   `09354E`, `0935C4`, `09472E`, `0947A4`); `1087` y `108B` ya tienen WAV
   en `audio-src` (sin política). Pasos: añadirlos al mapa,
   `render_sound_commands.py --labels … --output` a un directorio propio
   (`filesystem/audio-src` es un enlace a `ws-logo`: no escribir por él),
   `measure_sound_pairs.py --pairs` contra las etiquetas de la política,
   `derive_sound_policy.py` añadiendo de una en una (§17), `pack_game_audio.py`.
3. Tipos 8/0x12 de Tarma, Eri y Fio: filas `0BF398`/`0BF418` en el pack.
4. Agua `0C2CCC`: captura (ver la tabla) y traducción.
5. Camello (695 px), Slugnoid en el aire `097308`, pilotos de Tarma/Eri/Fio:
   sin empezar.

## 6. Agente F (16-XI, rama `work-audit`)

**Hecho.**

- **`1089`** (B75): en `data/sound_map.json` con sus 8 llamadores (búsqueda
  de `move.w #$1089,D0` + `jsr 0120F8` en la P-ROM: `09353A`, `0935B0`,
  `0938DA`, `09471A`, `094790`, `094ABA`, `0977DC`, `0AD1F4`; `108A`: `08E186`,
  `090A4A`, `09354E`, `0935C4`, `09472E`, `0947A4`; el código nativo ya los
  pedía). Render aislado (`render_sound_commands.py --labels sfx_1089 sfx_108a
  --output` a `/home/user/captures-audit/audio-src`): `1089` 1,537 s, `108A`
  0,644 s, `OK`. 438 pares (`measure_sound_pairs.py --pairs`, tres procesos;
  las dos con ellas mismas, entre sí, con `108B` y con las 102 etiquetas de la
  política en los dos órdenes; los de `108B` que faltaban desde G8). Añadidas
  de una en una (§17 de AUDIO_JUEGO): `1089` → impacto, nivel 2, **ningún otro
  sonido cambia**: 205 pares en `data/sound_pairs.json`, política derivada de
  nuevo. `108A` (voz 1) y `108B` (voz 1) suben `108E` (voz 1→2) y `116E`
  (voz 0→1): `108A` se descarta bajo `108F`/`1090` (nivel 0) pero corta a
  `108E` (nivel 1), sin orden total; quedan sin política, pares en
  `data/sound_pairs_b75_unmapped.json`. `sfx_1089.wav64` (VADPCM, 20 978 B)
  se lee de `rom:/` (`game_audio.c`). `filesystem/audio` de este árbol es ahora
  un directorio propio (enlaces por fichero a `ws-logo` más `sfx_1089.wav64`
  y su `manifest.json`).
- **Camello (695 px)**: no era el port ni la exportación. Con el validador
  actual (`export_rest_sprites.py`, cambios de `b6f34ac`/`e2953bf`: cada
  actualización que borra el nodo o lo saca del rango cuenta, y cada nodo
  elige su actualización) los mismos píxeles salen iguales: `camelfam`
  (`@34300-35600 @3b4e0-3b900 @1be24-1be30 @59e30-59f00`) 38 volcados, 21 525
  iguales, 0 distintos, 0 cubiertos (9531: 1648 = 1349 + 299 de antes).
  `docs/slug-sprites-validation.json` regenerado.
- **SV-001 (M1)** contra `tankfam` (`@92810-95800 @3b4e0-3bb00 @14ae8-153ee
  @1b600-1bd00 @1be24-1be30`): 36 volcados, 63 503 iguales, **429 distintos**
  en 7 volcados (10577, 10657, 10818, 10900, 11058: torreta `101784`;
  10977, 11058: 8 px de balas). Causa medida en 10577: los sprites 371/372 de
  la VRAM (tiles `8F99`/`8F9A`, paleta 0x6A, volteo H) caen sobre el destello
  de la torreta; ningún nodo trazado los dibuja (la traza de `tankfam` solo
  sigue `1014C4`, `1028B4`, `101784` y sus hijos por `00411C`/`0040BC`).
  Los píxeles propios de la torreta coinciden donde no hay ese sprite. No es
  una diferencia del port.

**Prueba en ares** (`test_vehicle_damage_ares.py --mission m1`, Xvfb `:96`):
`MSX AUDIO game sounds=98/98` (`sfx_1089` abre de `rom:/`), pero el mando
ciego no llega a los vehículos: muere tres veces por vida (`MSX player death
kind=9`) en la parada de las cúpulas (cámara 1920) hasta agotar el tiempo, y
`RESULT: FAIL`. **El cartucho de esta rama sin el cambio falla igual** (mismas
muertes de tipo 9, misma cámara): es previo a este cambio y queda abierto
(¿desde `386c5c0`/`c79ea00`, las muertes por tipo?).

**`make check`** (291 PASS, rc 0 con este cambio): no enlazaba desde `c79ea00` (`test_neo_rest` usa
`neo_effects.c`, que llama a `marco_death_cap_*`): `src/marco_death.c`
añadido a esa orden del `Makefile`.

**Hallazgo sin tocar:** `src/msx_sfx_policy.h` no es la compilación de
`data/sound_policy.json` de esta rama (la fusión `54adba8` derivó la política
con los pares de la M4 sin recompilar la cabecera: faltan `102E`, `1030`,
`1036`, `1053`, `1060`, `1066`, `10D1`, `1134`, `113B`, y `1132/1133/1137`
tienen nivel 0 en vez de 1). `1089` se ha insertado a mano tal como lo emite
`compile_sound_policy.py`, sin arrastrar esos cambios.

**Falta / siguiente paso.**

1. `108A`, `108B` y la voz de Tarma `1087`: el modelo de canales y niveles
   medido no los admite (descartes en los dos órdenes). Siguiente paso: leer
   en el controlador Z80 (`250-m1.m1`) cómo elige canal ADPCM-A y prioridad
   por orden, y sustituir el modelo por esa regla; sin eso no se les da
   política (no se rellena).
2. SV-001 a 0 px: los tiles `28F99`/`28F9A` son de la pose `1B0FE6` del
   programa `22C1B6` (vuelo de la bala `01B6DC`, la de la torreta); en
   10577 ninguna bala trazada tiene una actualización en los 2 fotogramas
   anteriores al volcado. Falta ver en `tankfam` (o una captura nueva) qué
   nodo de bala dibuja esa entrada y por qué el validador no la empareja.
3. Slugnoid: sus capturas se borraron tras extraer; la validación de
   píxeles (4047 distintos en saltos) y el golpe en el aire `097308` necesitan
   una captura nueva de la M5 (estado `u17460`, `capture_fullgame.lua`,
   `MSX_FULL_RIDE=pace`, que salta cada 75 fotogramas sin disparar).
4. Pilotos de Tarma/Eri/Fio: programas `09649C[1..3]` en
   `soldier_programs.json` (grupo `m5`), sprites sin exportar: falta una
   captura con cada personaje montado (`capture_fullgame.lua` no elige
   personaje; la elección por entradas está en `capture_marco_catalog.lua`).

## 7. Agente H (16-XI, rama `work-audit`)

**Hecho.**

- **`108A`, `108B`, `1087`** (B75, voz de Tarma): leído el controlador Z80
  (`250-m1.m1`; `AUDIO_JUEGO.md` §19, `tools/sound_driver_table.py`). Cada
  efecto tiene grupo `D` y prioridad `E` en su cabecera (`506F`); la ranura
  `50CF` corta o descarta dentro del grupo por la prioridad. La regla acierta
  5150 de los 5187 pares medidos claros. `108A`/`108B` tienen el grupo y la
  prioridad de `116E` (`07`/`0x65`) y `1087` los de `1123` (`02`/`0x01`):
  `derive_sound_policy.py --twin` les da su canal y nivel sin mover ningún
  sonido (voz 0 y voz 2); `sfx_1087.wav64` y `sfx_108a.wav64` en
  `filesystem/audio` (propio) desde `rom:/`; `test_msx_sfx` comprueba los
  gemelos y `108E` frente a `108A`/`108B`; `test_characters_ares.py` exige ya
  la voz de Tarma.
- **SV-001 contra `tankfam` (429 px)**: no es el port ni falta un nodo. El
  sprite de las 429 es la bala `01B882` del jugador (programa `22C1B6`, pose
  `1B0FE6`, paleta `0x6A`; p. ej. el nodo `1040C4` la dibuja en 10576 en x 0,
  y 0x146: SCB 371/372 en x 511 y 15), trazada pero en **otro rango** de la
  validación: `export_rest_sprites.py` validaba cada `@lo-hi` por separado, y
  una bala de la misma paleta `0x6A` sobre la torreta `03B57C` contaba como
  distinta. Ahora `DIR@lo-hi@lo-hi...` pinta la familia entera junta:
  `tankfam@92810-95800@3b4e0-3bb00@14ae8-153ee@1b600-1bd00@1be24-1be30`:
  12 volcados, 54 634 iguales, **19 distintos** (10818: 12, 11058: 7), 401
  cubiertos, 7 381 solapados. Los 19: en 10818 la tercera prueba del
  validador (una actualización por nodo) elige para la bala `1B0FE6` de
  (132, 324) la actualización sin entradas y la torreta `1C66E8` queda debajo
  (mismos tonos de `0x6A`); en 11058, 7 px en la fila 3..8 del borde superior
  (bala `1B107C` en y 510): **sin explicar** (pendiente).
- **Caída al agua `0C2CCC`** (B76): captura nueva `w6d2` (MAME 0.264,
  `run_marco_catalog.py --load /home/user/captures-m6/states/mslugx/sec09_292683.sta:292683
  --first 292700 --plan water.json --update-trace --dump-routines c2956-c2f96`:
  M6 etapa 9, andar a la derecha sin saltar; `--load` y `--update-trace`
  nuevos: el registro `MARCO_UPDATE` en `00424A` tras cada actualización,
  porque la M6 tarda 1..5 fotogramas por actualización y los volcados de fin
  de fotograma la cogen a medias; `MARCO_WATER` en `0C2D28` con la
  superficie). Dos caídas: 293020 al agua (`0C2CCC`/`0C2D06`, 66
  actualizaciones) y 293315 fuera de la pantalla sin agua debajo (`0C2DDA`).
  Port: `marco_death_start_water` / `marco_death_water_velocity` (programa
  `24ECCC`, `+19` = `0BEDF0`, `+80` = 0x1E, `+30 −= >> 4`, superficie
  `(D1 + objeto+18 − 8 − +40) >> 3` o `+34 += (−0x10 − +34) >> 5`, fin en el
  suelo o fuera de la pantalla), `marco_death_water_stage` (`0191FC`: etapas
  9, 0xB y 0), `gameplay.c` (`player_water`: `03C55C` con `013CF8`;
  `neo_kernel_object_y`). La última pose se ve en la actualización que acaba
  (`final_draw`), también en la salida por la pantalla de `0C2DDA`.
  `make check` `death`: 52 velocidades del agua, 53 + 34 poses: 0 distintos.
  Tarma/Eri/Fio: sus programas `24F150`/`24F5D4`/`24FA58` no están en los
  packs (parada explícita, `death_stops`).

**Brutos borrados**: de `slugair` (traza de familia) y `w6d2` (volcados)
quedan en `/home/user/captures-audit/` los informes `slugair-sprites.json`,
`tankfam-union-sprites.json`, `w6d2/debug.log.gz` y los fixtures en `data/`.
**ares** (`:96`): `test_characters_ares.py --places 2`: Tarma, `death voice
1087 plays`, PASS; `test_mission6_ares.py --stage 1` (sin saltar): tres caídas
al agua (`MSX fall death stage 9`; la etapa falla como antes por no saltar,
B68); la línea nueva `MSX fall into water 0C2CCC` marca la rama del agua.

**Pendiente / siguiente paso.**

1. Política entera por la regla del Z80 (AUDIO_JUEGO §19, pendiente).
2. Los 19 px de `tankfam` (arriba).
3. Agua: sin auditoría de píxeles de `w6d2` (la M6 no está en el catálogo de
   `audit_marco_catalog.py`); el paso `0143E4` de la caída usa el paso aéreo
   `0143EE` del port (difieren en `10C00D` y en `004ED2`): no comparado (la
   prueba usa el movimiento de la captura).
4. Hecho: **Slugnoid golpeado en el aire** (captura `slugair`, MAME 0.264,
   `capture_fullgame.lua` desde `u17460` con `MSX_FULL_RIDE=pace`,
   `VEHICLE_CHEAT=0`, raíces de `slughit`, `FROM=38008`, traza cortada en
   39840): golpe en el aire `097308` → `09731A` en 39174, en tierra `09737E`
   en 39294, destrucción con jinete `0973E2` en 39666, `09756A`. Familia nueva
   `slugair` en `make check` (81 nodos, 4161 actualizaciones 38008..39832):
   **igual bit a bit** (el nativo ya lo tenía). Sprites (`export_rest_sprites.py`
   con todos los rangos de la familia juntos): 31 volcados, 40 206 iguales,
   **0 distintos** (56 entradas sin descriptor exportado, no pintadas: las de
   fuera de las raíces). Los 4047 de la M2 (`ARMAS_VEHICULOS.md`, saltos con
   balas vulcan `01B6A0` delante) no se han revalidado: su captura se borró.
5. **Piloto Tarma del Slugnoid**: el TAS juega con Tarma (`+6E` = 1 en la RAM
   de `slugair`), así que `slugair` es ya la captura de Tarma pilotando: las 56
   entradas sin descriptor eran sus poses `197FDC..19822C` (programas
   `29E122..`, tabla `09649C[1]`, grupo `m5`). `export_rest_sprites.py
   --roots-from 29e100-29f000` (nuevo) las exporta y valida: 31 volcados,
   42 481 iguales, **0 distintos**, 0 sin descriptor. No entran en la
   exportación por defecto ni en el cartucho (presupuesto de RAM de la M2/M5,
   `ARMAS_VEHICULOS.md`). **Pendiente**: Eri y Fio (`09649C[2]`/`[3]`) y los
   pilotos del SV-001 (`0954D4`): hace falta una partida con ellas;
   `capture_marco_catalog.lua` elige personaje en SOLDIER SELECT
   (`MSX_CHARACTER`, como la ROM: el cursor al confirmar fija `+6E`), pero sus
   planes son experimentos cortos al principio de la M1; `capture_fullgame.lua` y
   `capture_weapons.lua` (planes que llegan a los vehículos) no eligen
   personaje: siguiente paso, pasarles la elección de `capture_marco_catalog.lua`.

## 8. Agente J (16-XI, rama `work-audit`)

### 8.1 Catálogo de Marco (tipos 8, 0x12, agua y paletas de la muerte)

**Catálogo nuevo, para copiar a `ws-logo`:**
`/home/user/captures-audit/j/catalog/` (`build/marco_pose_meta.h`,
`build/marco_pose_assets.json`, `build/marco-poses.stamp` y
`romfs/marco/p0..p768.sprite`; los `.sprite` sustituyen a todos los de
`ws-logo/romfs/marco`: la numeración de las piezas cambia). **Con él hay que
copiar también los packs** `ws-audit/romfs/chars/c1..c3.bin` (versión 2, 11
paletas: `msx_character_layout` exige `palettes == MARCO_PALETTES`) y el
código de esta rama (`marco_death.c`, `marco_renderer.c`, `msx_character.*`).

- **744 poses, 769 piezas CI4, 582 336 bytes, 11 perfiles** (antes 720 /
  745 / 563 304 / 7). `build_marco_poses.py` con las 17 auditorías de
  `captures-marco` de siempre más `j/k8d1`, `j/k12d1`, `j/mzd1`, `j/w6d2`
  (`--out-dir`, nuevo: escribe `build/` y `romfs/marco` en otro árbol).
  Nombres nuevos: `DEATH8` (7, `0BEDE0`: Marco 0x53), `DEATH8B` (8, 0x51),
  `MUMMYDEATH` (9, `0BEE08`: 0x132), `WATER` (10, `0BEDF0`: 0x136); cada uno
  es la paleta de la ROM de la columna de Marco, palabra a palabra.
- **Las anteriores, idénticas**: el mismo comando sin las cuatro auditorías
  nuevas (`j/cat-old`) da el catálogo de `ws-logo` (paletas, piezas, poses,
  anclas y CRC iguales; los 745 `.sprite` iguales byte a byte), y
  `tools/check_marco_catalog.py j/catalog --base j/cat-old`: 720 poses y 745
  piezas idénticas, las 7 paletas en su índice.
- **Las nuevas a 0 px**: 24 poses (`18CF86..18CFE0`, `18D298..18D450`,
  `18D7B0`). `check_marco_catalog.py` (nuevo) comprueba que las piezas del
  catálogo son la descodificación de la ROM de cada descriptor que las
  auditorías verificaron (k8d1 216 fotogramas, 102 512 px; k12d1 132,
  53 450; mzd1 202, 270 124; w6d2 167, 91 814; 0 distintos) y que cada
  banco con que MAME lo dibuja es un perfil del catálogo: `18CF86/18CFA4`
  `DEATH8`, `18CFC2` `DEATH8`+`DEATH8B`, `18CFE0` `DEATH8B`, la momia
  `MUMMYDEATH` y `18CDE6` `BURNT`, el agua `WATER`: PASS.
- **Capturas repetidas** (MAME 0.264, las órdenes del §2/§7, en
  `/home/user/captures-audit/j/`): `mzd1`, `k8d1`, `k12d1` dan auditorías
  iguales campo a campo a las de antes (el `debug.log` de k8d1/k12d1 solo
  difiere en la ruta); `w6d2` (con `--command-trace --paired-trace`) se
  audita por primera vez: `audit_marco_catalog.py --draw-palettes` (nuevo:
  en la M6 el banco 0x63 es de otro actor) y la oclusión por un sprite con
  autoanimación (el agua, atributo bit 3): `top_sprite_pixel(..., auto)` y
  un solo contador LSPC por fotograma que explique todas sus oclusiones
  (113 959 píxeles, 151 fotogramas); excluidos 293188.. (la reaparición:
  un sprite encogido, `zx` 11, encima; sus poses ya estaban).
- **Port**: la muerte usaba `palettes[1..3]` = 0x63..0x65 (desplazado: en
  `0043EA` la ranura es `+19 + (flags >> 4)`; MAME k8d1: los `flags` 0x10/0x20
  de `25032A` dibujan con los bancos de `+1A`/`+1B`); ahora `palettes[0..2]`.
  `2501DC` dibuja `18CDE6` con `flags` 0x10 (`+1A`, quemado en mzd1): estaba
  traducido con 0x40. `actor.c` elige el perfil de cada dibujo por su ranura y
  la tabla que la muerte puso en ella (`palette_table`, `palette_slot1a`,
  `palette_slot1b` nuevo; `marco_renderer_death_palette`): la momia muere con
  0x132/quemado (**B74 corregido**), el tipo 8 con 0x53/0x51 y el agua con su
  paleta (antes la normal).

### 8.2 Tarma, Eri y Fio: tipos 8, 0x12 y agua (B76)

- Pack versión 2 (`export_character.py`): ocho muertes (`DEATH_TABLES` +
  `0BF398`, `0BF418`, `0BF2D8`, columna +6E); la llamada `05A09E` de `25032A`
  (medida en k8d1) es un `EVENT` que conserva la rutina; un programa que salta
  por debajo de su entrada (`25032A` → `25016A`) se ordena desde la entrada
  (comprobado que nada cae por el corte); paletas `DEATH8`, `DEATH8B`,
  `MUMMYDEATH`, `WATER` de la ROM por columna. `marco_death.c` toma los
  programas 5..7 del pack: **sin paradas explícitas** (`death_stops`) en
  esos tipos.
- Capturas nuevas (MAME 0.264, `--character N --first 2350 --hit 400:8|12
  --end-frame 3250 --dump-routines c2956-c2f96`): `j/c{1,2,3}k8`,
  `j/c{1,2,3}k12`; auditoría de píxeles PASS (c1k8 216 fotogramas, 105 990
  px; c1k12 132, 54 184; c2k8 240, 83 568; c2k12 144, 60 622; c3k8 240,
  91 068; c3k12 144, 58 842). `export_character.py --verify`: todos los
  descriptores de la muerte en el pack (falta solo el de antes del golpe,
  `194B70`/`19DDA2`/`1A5982`, que ya faltaba en `cNd1`). Fixture:
  `data/form_death_fixture.json` + 6 muertes (31 en `death_fixture.h`; las de
  `mzd1`, `k8d1`, `k12d1`, `w6d2` re-extraídas de las capturas nuevas salen
  iguales). `make check-characters`: Tarma, Eri y Fio, 3 muertes cada uno
  contra MAME (d1, k8, k12), poses, velocidades y fin: **0 distintos**; el pack
  de Marco (`c0`) igual al catálogo interno también en las muertes 8/0x12 y
  el agua (casos nuevos en `test_character.c`).
- **Pendiente**: agua de Tarma/Eri/Fio sin captura (el estado de la M6 es
  de Marco; los programas `24F150/24F5D4/24FA58` vienen de la ROM y su forma,
  una pose y fin, es la de Marco).

### 8.3 Pilotos de Eri y Fio en el SV-001

- **Cómo fija la ROM el personaje**: en SOLDIER SELECT el cursor de P1 (orden
  en pantalla Marco, Eri, Tarma, Fio) se confirma con A y el panel `0C961C`
  escribe `+6E` (`msx_character.h`). `capture_weapons.lua`
  (`MSX_WEAPONS_CHARACTER`) y `capture_fullgame.lua` (`MSX_FULL_CHARACTER`)
  eligen ahora igual que `capture_marco_catalog.lua`: Derecha una vez por
  casilla desde 1700 y A en 1800, en vez de A mantenido 1550..1700 (solo
  entradas de mando; ninguna escritura de RAM).
- El hijo de subida `0954DC/095580` dibuja el programa `0954A4/B4/C4[+6E]`
  (Eri `292732/292770/25E536`, Fio `2927AE/2927EC/25EECC`) con la paleta
  `0954D4[+6E]` (Eri 0x4, Fio 0x5, las que faltaban en la exportación del
  SV-001). Capturas MAME 0.264 (plan `vehicles`, `ASSIST_CAMERA=2000`,
  familia `92a32`): Eri sube en 11450 (`+6E` = 2), Fio en 9781 (`+6E` = 3).
  `export_rest_sprites.py "CAP@954dc-95640" --roots-from 292600-292800`:
  **Fio 13 volcados (9775.., uno por fotograma), 11 120 píxeles iguales, 0
  distintos** (`/home/user/captures-audit/j/fio-tank-pilot-sprites.json`);
  **Eri 3 volcados (PERIOD 4), 2 345 iguales, 0 distintos** (una ejecución;
  su informe se sobrescribió al repetirla, ver abajo).
- **Hallazgo**: el plan `vehicles` con otro personaje no es reproducible: con
  los mismos `MSX_WEAPONS_*` Eri sube en 11450 en dos ejecuciones y en otras
  tres no llega al tanque antes de 11500 (cámara 3192: lo pasa de largo).
  Sin explicar (¿carga de la máquina? `make check` corría a la vez); por eso la
  captura de Eri a cada fotograma queda **pendiente**.
- La familia entera del tanque con Eri (12 912 descriptores, 240 volcados):
  1 302 350 iguales y 559 distintos en 26 volcados, sin atribuir (no son del
  piloto: su nodo da 0); **pendiente**, como los 19 de Marco (§7).
- **Pendiente**: los pilotos del Slugnoid de Eri y Fio (`09649C[2]/[3]`):
  hace falta una partida hasta la M2/M5 con ellas
  (`capture_fullgame.lua` con `MSX_FULL_CHARACTER`, ya posible, pero la
  partida desde el arranque hasta el Slugnoid no se ha hecho aquí).

### 8.4 Estado al terminar (agente J)

- `make check` (292 PASS, rc 0) y `make -j2` con todo lo anterior; ares
  (`:96`) `test_characters_ares.py mslugx-explorer.z64`: los cuatro personajes
  con los packs v2 y el catálogo nuevo, voces de muerte 1123/1125/1087/114E,
  **RESULT: PASS**.
- Datos en `/home/user/captures-audit/j/`: `catalog/` (para `ws-logo`, §8.1),
  las auditorías y `debug.log`/`palette*.bin` de `mzd1`, `k8d1`, `k12d1`,
  `w6d2`, `c{1,2,3}k{8,12}` (volcados de VRAM, pantallas y nodos borrados;
  `check_marco_catalog.py` necesita además `catalog/build/marco-png`, que se
  rehace con la orden de §8.1), los informes de los pilotos.
- Siguiente paso: copiar `catalog/` y `romfs/chars/c1..c3.bin` a `ws-logo`
  junto con la fusión de esta rama; agua de Tarma/Eri/Fio; Eri a cada
  fotograma en el SV-001 y la reproducibilidad del plan `vehicles`; pilotos
  del Slugnoid de Eri/Fio; reparto de canales del chip `4422`.

### 8.5 Humo `05A0B4` de la muerte de tipo 8 (agente N)

- La matriz de integración (m5boss) contaba 28 dibujos ausentes del humo:
  el port lo crea (`neo_effects.c`, programa `25339C`, paleta 0x204) pero
  sus descriptores `1B02D6..1B038E` (10 poses) no estaban en ningún export.
- Captura nueva (MAME 0.264): `run_marco_catalog.py --rom-dir ROMS --output
  k8s1 --hit 400:8 --end-frame 3000 --dump-routines c2956-c2f96
  --family-routines 5a0b4-5a0ff --family-frames 2400-3000 --child-routines
  5a0b4,5a0e2`. `--family-routines` (nuevo, `capture_marco_catalog.lua`):
  `boss-trace.txt` con el formato de `capture_weapons.lua` (U/V alrededor de
  `004246..00424A` de cada nodo cuya rutina está en el rango, sus W) para
  `export_rest_sprites.py DIR@lo-hi`. El humo: nodo `101F14`, 14
  actualizaciones (2532..2556), una pose por actualización.
- `export_rest_sprites.py` raíz `0x25339C` en `ROOTS`; `--only` (nuevo): solo
  los programas de `--roots-from`, sobre el export de `--keep` (que conserva
  ahora también su `empty` y, con `--only`, su orden), sin las raíces que
  otras ramas añadieron desde que se hizo. Orden por export (N = 2..6):
  `export_rest_sprites.py ROMS/mslugx.zip k8s1@5a0b4-5a0ff --roots-from
  25339c-25339d --only --keep-palettes --keep VIEJO --output NUEVO`.
- Validación: 27 volcados, **9 962 píxeles iguales, 0 distintos**; las 10
  poses vistas (de 503 a 78 px; `/home/user/captures-audit/j/k8s1-sprites.json`,
  brutos borrados, quedan `boss-trace.txt` y `children.log`).
- Dónde: el humo es del jugador y puede salir en cualquier misión, así que va
  una vez en el export de escena `filesystem/rest-native.json` (residente en
  todas: `scene_native_descriptor`), `scene.pack` 1036 → 1038 trozos (16
  páginas). Los exports M2..M6 lo llevan también (regla de
  `check_mission_sprites.py`), pero `pack_mission.py` no guarda lo que ya
  está en la escena: los paquetes de misión no cambian (M2..M4 rehechos
  idénticos byte a byte; M5 1250 y M6 1195 trozos, iguales; su bloque
  `MSXD` rehecho crece 16 606 B por los programas de este árbol, sin
  relación con el humo).

