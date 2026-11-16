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
| Tarma | las mismas, columna 1 | nativo (pack `c1.bin`) | `check-characters`: captura `c1d1` (`--character 1 --hit`), 51 poses, fin, vida nueva, entrada: **0 distintos** | auditoría `c1d1`: PASS 168 fotogramas, 21 descriptores, 93 578 píxeles | **1087 sin mapear** (silencio; FALLOS §5, PERSONAJES §6.1) | voz 1087 (necesita captura con audio); tipos 2/7/aire sin captura |
| Eri | columna 2 | nativo (`c2.bin`) | `c2d1`: 57 poses, 0 distintos | `c2d1`: PASS 180 fotogramas, 19 descriptores, 69 850 píxeles | 1125 suena | tipos 2/7/aire sin captura |
| Fio | columna 3 | nativo (`c3.bin`) salvo `0C4C28` | `c3d1`: 57 poses, 0 distintos | `c3d1`: PASS 180 fotogramas, 19 descriptores, 73 232 píxeles | 114E suena | el actor `0C4C64` (programa `253322`) **sin portar** (no se dibuja); `c3d1` es su primera muerte capturada (VRAM y trazas borradas tras el fixture: repetir con `--dump-routines` ampliado para traducirlo) |
| Todos | tipo 8 `0C2ACC`, tipo 0x12 `0C2A70`, agua `0C2CCC` | **parada explícita** (se cuenta en `death_stops` y se hace `0C2DDA`) | — | — | — | sin captura; no se ha localizado qué fila de ataque da 8 o 0x12 |

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
| SV-001 (M1, M4; `092A32`) | golpe en tierra `09351A`/`093576`, en el aire `0935A8`/`0935EC`, parpadeo `+7E` (paleta `+19`), destrucción con piloto `0938DA`/`09391E` → `093AFA` (destello `0AE1DC`, piezas `0AD9B8`, ráfaga `093BC2`, torreta `03B936`), paletas `093882` | nativo (`neo_tank.c`; B52, B59) | `tankhit` (`09351A`, `093576`), `tankkill` (`09391E`, `093AFA`), `tankair` (`0935EC`, `093576`, `09391E`, `093AFA`): iguales; ares `m4boss` 7 golpes | `rest-sprites-validation.json` (lado vehículos; diferencias atribuidas a solapes y parpadeo `+7E`, no 0) | 1034, 10E9 suenan; **1089/108A (retroceso) sin renderizar** → silencio (B75) | paradas explícitas que no son de daño (salto, agachado, bombas, obús de tipo 5); validación de píxeles del parpadeo y la destrucción con 0 distintos |
| SV-001 (M3, copia `093C9A`) | golpes en el aire `0946FA` → `094788` → `0947CC`, destrucción `094ABA` | nativo | `m3tankair` (137396..138628, `094756`, `0947CC`, `094AFE`, `094CDA`): igual | ídem | ídem (B75) | — |
| Camel Slug (M1; `03458E`) | sube con salud 0: el primer golpe pone `+5F` bit 7 → `0351BC` (`0AA7FC(0)`: muere el jinete); el camello vuelve a esperar | nativo (`neo_slug.c`) | `slug` (camelfam 9400..10095, jinete muerto 10091): igual; ares `veh` (B69) | `slug-sprites-validation.json`: 24 volcados, 21 206 iguales, **695 distintos** (subida 9531: 299; muerte 10092: 324; balas) | voz del jinete (1123 etc.) | volcado de la muerte 10092 no a 0 (asincronía volcado/actualización, sin medir pose a pose); agua `034AC0` parada explícita |
| Slugnoid (M2 E3, M5; `09692C`) | golpes en tierra `09737E`/`097396`, destrucción con jinete `0973E2` → `097414` (marcador `03533A`) → `09756A`; paleta de muerte `097EDE`/`097F52` | nativo (`neo_slugnoid.c`, B38) | `slughit` (39039/39331 golpes, 39503 destrucción): igual | `docs/ARMAS_VEHICULOS.md` paso 17: 4047 distintos en 7 volcados (saltos) | 1034, 10E9; **1089** (B75) | golpe en el aire `097308`, `0977DC` (torreta perdida), lista de muerte `097EDE` en espera: sin captura; pilotos de Eri, Tarma y Fio sin exportar (B69); no probado en ares bajo fuego |
| Vehículo volador de la M3 (`0AC866`) | golpe (parpadeo `+8C`, retroceso `+8E`), destrucción con jinete (paleta de muerte `+86`, piezas `0AD9B8`, destello `0AE1DC`) | nativo (`neo_m3veh.c`) | `m3veh2` (vuela hasta destruirse con su jinete): igual | MISION3_ACTORES | 1034, 1090, 10E9; **108B sin política** | `0AE752` (índice de paleta 0xF2: trap) parada explícita |

Otros vehículos del port (`neo_m4veh.c` `0587C4`, `neo_m5tank`, trenes…) son
**enemigos**, no jugables: fuera de esta auditoría.

## 4. Fallos (BUG_MATRIX)

- **B73** muerte del gordo: corregida, `death` la prueba (`fatd1`).
- **B74** muerte de la momia: ruta corregida y probada (`mzd1`); la paleta
  `0BEE08` sigue pendiente (catálogo con una paleta por pose).
- **B75** sonidos de golpe de vehículo 1089/108A/108B sin renderizar ni
  política: pendiente (medir pares como en AUDIO_JUEGO.md §17).

## 5. Siguiente paso

1. Paleta de la muerte de la momia: piezas del catálogo por byte de paleta
   (`build_marco_poses.py`) o un perfil compuesto; el perfil `0x132` de
   `mzd1` (repetir la captura, ~26 MB).
2. Voz 1087 y sonidos 1089/108A/108B: captura con audio
   (`run_gameplay_audio.py`), render y pares.
3. Fio `0C4C64`: repetir `c3d1` guardando la VRAM del actor y traducirlo.
4. Muertes de tipo 8, 0x12 y agua: capturas con `--hit 400:8` / `400:12`
   y el agua de la M6, y su traducción.

## 6. Pruebas de esta rama

`death` (Marco: 18 muertes, 447 poses), `check-characters` (Tarma, Eri, Fio:
una muerte cada uno, 0 distintos), `gameplay` y `marco_visual` en el PC:
PASS; objetos N64 de los ficheros tocados compilan sin avisos. **No** se ha
corrido `make check` completo ni la matriz de ares: las cabeceras de familias
de `build/` ocupan ~1,6 GB y el disco quedó a 0 (quedaban 1,8 GB); por eso la
rama no se ha fusionado en `ccr`.
