# Armas y vehículos del jugador (evidencia para el port)

Estado: **investigación**, sin código de juego. Todo lo que sigue sale del
desensamblado de la ROM P1 (`250-p1.p1`, direcciones 68000 tal cual) y de
trazas de MAME 0.264 (`mslugx`, BIOS `euro`) hechas con
`tools/capture_weapons.lua`. Lo que no está respaldado por ambas cosas se marca
**desconocido** o **no observado**. Las direcciones son procedencia, no
punteros para el N64.

Convenciones: `A5 = 0x108000` (fijado en `0x0040BC`), así que `$4718(a5)` es
la RAM `0x10C718` y `-$6B3C(a5)` / `-$6A8C(a5)` son los actores de jugador
`0x1014C4` (P1) / `0x101574` (P2). "p" es el índice de jugador (actor `+70`):
los campos por jugador de un byte están en `base+p`, los de palabra en
`base+2p`.

## Capturas

`tools/run_capture_weapons.sh SALIDA` (MAME con `-debug`; los `printf` de los
puntos de ruptura van a `SALIDA/debug.log` como `BP_<dirección> frame=N ...`).
Las salidas están en `/home/user/captures/weapons/` (fuera de Git: contienen
RAM, VRAM y paletas de la ROM).

Escrituras de RAM, solo de captura: tiempo `1013DE`, bit de invulnerabilidad
del P1 `101523 |= 0x40`, `101549`, `101288` (las mismas que
`capture_mission_rest.lua`). **Nada escribe el bloque de armas, las bombas ni
las ranuras de vehículo**, salvo la ayuda de progresión opcional
`MSX_WEAPONS_ASSIST_CAMERA` (ver abajo).

| Ejecución | Variables | Fotogramas | Contenido |
| --- | --- | --- | --- |
| `pickups` | `PLAN=pickups LAST=12000` (sin ayuda) | 2100..12000 | Solo pistola. Granadas 2943..3483 (10→0), objeto bombas 7570 (0→10), granadas 7803..8402 (10→0), objeto C (`0x4B`) 8370, fuego C 8376..9038, fin de munición 9050. La parada de las cúpulas (cámara 1920) no termina antes de 12000. |
| `pickups2` | `PLAN=pickups ASSIST_CAMERA=2000 LAST=14500` | 2100..14500 | Ayuda hasta 9399 (cámara 2003). Camel 9530..9593 (salida A+B 9592). SV-001 11192..11255 (salida A+B 11254). H (`0x4F`) 11199, 200 balas H 11329..12386, vuelta a la pistola 12386. |
| `vehicles2` | `PLAN=vehicles ASSIST_CAMERA=2000 LAST=14500` | 2100..14500 | Camel 9530..10091: 8 bombas 9532.., sin bombas 10 veces `0x034FD2`, golpe a la Camel 10091 → muerte del jinete 10092, reaparición 10194..10196. SV-001 10375..11099: 9 obuses 10416..11057, vulcan, salida A+B 11099. H tomada dentro del tanque 10926, fuego H 11173..12211, pistola 12211. |
| `camelfam` | como `vehicles2`, `LAST=10100 FAMILY_FROM=9400 FAMILY_TO=10095 FAMILY_ROOTS=3458e ALLOC_ALL=9525-10095` | 9400..10095 | Traza de familia (formato de `capture_mission_rest.lua`) de la Camel y del jugador. |
| `tankfam` | como `vehicles2`, `LAST=11120 FAMILY_FROM=10360 FAMILY_TO=11110 FAMILY_NODES=1014c4,1028b4,101784 ALLOC_ALL=10370-11110` | 10360..11110 | Traza de familia del jugador, el SV-001 (nodo `1028B4`, creado antes de la ventana) y su torreta vulcan (`101784`). |
| `hfam` | como `pickups2`, `LAST=12420 FAMILY_FROM=11190 FAMILY_TO=12410 FAMILY_ROOTS=8c37c,8c374` | 11190..12410 | Traza de familia del jugador, sus balas H y los objetos. |
| `cfam` | como `pickups`, `LAST=9070 FAMILY_FROM=7795 FAMILY_TO=9060 FAMILY_ROOTS=8c37c,8c374` | 7795..9060 | Traza de familia del jugador: 10 granadas, objeto C, 40 disparos C hasta la vuelta a la pistola. |

La Camel Slug y el SV-001 están en la propia Misión 1 (cámara 2029..2336):
no hace falta selección de nivel ni arranque por RAM. Con solo la pistola la
partida se queda en la parada de las cúpulas (cámara 1920) más de 3000
fotogramas, por eso `pickups2`/`vehicles2` usan la ayuda de progresión: hasta
la cámara 2000 escriben arma 3, munición y bombas (como
`capture_mission_rest.lua`); en ese momento escriben munición 0 y bombas 10
una vez y dejan de escribir. La vuelta a la pistola de 9399 es la del propio
juego. Las cifras de bombas anteriores a 9399 en esas dos ejecuciones no
son del juego.

Cada ejecución escribe `weapons.csv` (cada fotograma: entradas, palabra de
mando `10E9EC`, campos del actor P1, bloque `10C718..10C72F`, ranura
`10C608..10C62B`, puntero `10C2E8`), `trace.csv`, `actors.csv` (con `+6E`:
tipo de objeto), `actorstate.csv`, y volcados `ram|vram|palette|game<f>`
cada 30 fotogramas desde `MSX_WEAPONS_FIRST_DUMP` y en cada cambio de arma,
bombas, tipo de bomba o ranura. Las trazas de familia se comprobaron con
`tools/extract_family_fixture.py` (ver el plan). `pickups` se hizo antes de
añadir los puntos de ruptura de comida, Camel y SV-001; las demás, con la
versión final del script.

Nota: la traza de familia de `capture_mission_rest.lua` toma el creador de
`A6` en `0x0040EC`; `0x00411C` llama allí con `A6 = 0x101414` (cabeza de la
lista), así que las balas creadas con `0x00411C` no entraban en la familia.
`capture_weapons.lua` usa el `A6` que `0x00411C` guardó en la pila
(`(SP+8)` cuando `(SP+4) = 0x004122`). El nodo conserva `+0C = 101414`.

## 1. Estado de armas en RAM: bloque `0x10C718`

Inicialización de partida `0x0C0E20` (palabras `+0..+8` del bloque a 0 salvo
`+4 = FFFF`) y de cada vida
`0x0C0E6E` (bombas 10 en `0x0C0E80`, tipo de bomba 1, pistola con
`0x0C0EE2`). Campos (P1 = `+0`, P2 = `+1` / `+2`):

| RAM (P1) | Tamaño | Significado | Evidencia |
| --- | --- | --- | --- |
| `10C718+p` | byte | granadas en vuelo: `+1` en `0x0C0D34` (constructor de granada), `−1` en `0x0C0D48`; `0x0C07F8` compara con 2 | `weapons.csv`: 0→1 en cada lanzamiento (2946, 3006…), 2 en 8405 |
| `10C71A+p` | byte | proyectiles activos del arma actual: `+1` en `0x0C0D64`, `−1` en `0x0C0DFC` (lo llaman las balas de
algunas armas al acabar: `0x0639D8…`, `0x08630C`, `0x0A73CE`); a 0 al cambiar de arma | `fire ... shots=` 1..3 con C |
| `10C71C+p` | byte | máximo de proyectiles activos, tabla `0x0C08A8[id]` (`FF` = sin límite) | 03 con C, FF con H y pistola |
| `10C71E+p` | byte | **id de arma** (0 = pistola) | 0C en 8370, 0A en 11199, 00 en 9050/12386 |
| `10C720+p` | byte | **bombas** (máx. 99) | 0A→00 en 2943..3483 |
| `10C722+p` | byte | tipo de bomba (1 = granada normal) | siempre 01 en Misión 1 |
| `10C724+2p` | palabra | **munición** (`FFFF` = infinita) | 0028 al tomar C, 00C8 al tomar H |
| `10C728+2p` | palabra | contador de comida (`+0x100` por objeto de comida, `0x0C0C24`) | 0 en Misión 1 (ver §2) |

`10C72C` (palabra, `0x0C0D24/0x0C0D2C`, comparada con la Y del jugador en
`0x0C3716`) no es de armas. `10C72E` es ya el objeto de capa.

Campos del actor jugador usados por este sistema (observados en
`weapons.csv`): `+93` forma del cuerpo superior (0 pistola, 1 arma pesada;
2 y 3 existen en el código, no observadas), `+94` forma pedida (`FF` =
ninguna; `0x0C096E` la pide al dar un arma, el controlador la copia a `+93`),
`+95` contador de disparos (`0x0C4430`), `+90` estado de piernas (01 correr,
03 en el aire, `FF` quieto o montado: valores observados), `+9A` bit 7
"montado".

## 2. Recogida de objetos (`0x08B568..0x08B9B0`)

El registro de objeto `0x0E581A + 0x18·tipo` (ver `MISION1_ACTORES.md`) da en
`+10` la rutina de recogida. Se llama con `A6` = objeto, `A0` = registro,
`D0` = jugador.

- **Armas** `0x08B568..0x08B5BA`: cada entrada es `moveq #id,d1` + salto a
  `0x08B5BC` → `0x0C0928`. Tabla tipo → id: `00→5, 01→2, 02→3, 03→4, 04→6,
  4A→D, 4B→C, 4C→F, 4D→E, 4F→A, 50→7, 51→8, 52→9, 53→B`. Si se acepta
  (carry): ventana de puntos `0x08B896` + `0x08B850` y sonido `0x1121`.
- **Bombas** tipo `06` → `0x08B6D0` → `0x0C0B32` con `D1 = 10`.
- **Tipo de bomba** tipos `08,09,0A,0B,4E` → `0x08B5F4..0x08B60C` (`D1` =
  1,2,4,5,3) → `0x0C0A7C`; no aparecen en Misión 1.
- **Puntos / comida** (todos los demás) → `0x08B7FC`: sonido `0x1121`; si el
  byte 3 del registro es ≠ 0 y el objeto no tiene `+7A`, `0x0C0BF4` (comida:
  `+0x100` en `10C728+2p` y `+9A` bit 4 solo si la misión `$42F6(a5)` es 4, 5,
  6, 7, 9 o 10, `0x0C0BAE`; con `≥ 0x780` pide la forma 2). Luego puntos.
- **Puntos** `0x08B896`: valor = palabra larga `+0C` del registro (BCD; el
  rótulo `0x08D330` se crea en la lista `0x101414` con ese valor) o, si el
  byte 0 del registro es ≠ 0 y el anterior fue hace ≤ 15 fotogramas
  (`10C594+4p` contra `10C590`), la tabla de combo `0x0E5556`.
  `0x08B850` suma con `0x057948` (`D0 = jugador+1`, `A0 = 10C5A0+4p`).

Los tipos de la Misión 1 (registro `0x0E581A`; puntos en BCD):

| Tipo | Recogida | Efecto | Observado |
| --- | --- | --- | --- |
| `06` | `0x08B6D0` | +10 bombas (`0x0C0B32`, tope 99) y 1000 puntos | `pickups` 7570: `give_bombs d1=A bombs=0`, bombas 0→10 |
| `18`, `1C`, `29`, `2A`, `2B`, `2F` | `0x08B7FC` | puntos 100 / 1000 / 100 / 100 / 100 / 1000 | `2A` y `2B` tomados en `pickups2`/`vehicles2` (`score_popup value=100`); el resto solo estático |
| `4B` | `0x08B58C` | arma `0x0C` (C, Enemy Chaser: icono "C" en `pickups/game8340.png`) | `pickups` 8370 y `pickups2` 5875 |
| `4F` | `0x08B59E` | arma `0x0A` (H) | `pickups2` 11199, `vehicles2` 10926 |
| `51` | `0x08B5B0` | arma `0x08` (lanzacohetes, Paso 15) | prisionero `2D70C4` (cámara 3224, antes del jefe) |

También se tomaron tipos de puntos/comida `1A, 24, 30, 35, 36, 58` (comida
sin efecto en Misión 1: `food_item ... food=0`, `10C728` sigue a 0).

### Concesión de arma `0x0C0928`

Entrada `D0` jugador, `D1` id. Rechaza (carry a 0) si el jugador no existe,
si `+93 = 3` o si `id ≥ 0x10`. Si no: `+95 = 0`, `10C71E+p = id`,
`+94 = 0x0C0918[id]` (0 para ids 0–1, 1 para el resto),
`10C71C+p = 0x0C08A8[id]`. Si el id es distinto del anterior, `10C71A+p = 0`
y munición `= 0x0C08B8[id]`; si es el mismo, munición `+= 0x0C08D8[id]` con
tope 999. Sonido `0x0C08F8[id]`.

| id | munición | +misma | máx. activos | sonido | constructor (`0x0C42AA`) | Identificación |
| --- | --- | --- | --- | --- | --- | --- |
| 0 | FFFF | 0 | FF | — | `0x01B782` | pistola (observado) |
| 1 | 8 | 30 | 4 | — | `0x0C5508` | desconocido (forma 0) |
| 2 | 30 | 10 | 1 | 112F | `0x0644C4` | desconocido |
| 3 | 30 | 20 | 2 | 112E | `0x06381C` + `0x0C58D2` | R (cohetes, `pickups2/game7020.png`, ayuda) |
| 4 | 30 | 15 | 1 | 112C | `0x0A71F8` | desconocido |
| 5 | 200 | 150 | FF | 112D | especial `0x01B6DC` | H (tipo `00`, no en M1) |
| 6 | 200 | 150 | FF | 1130 | rayo `0x0C46D6` | desconocido (rayo `0x01E19E`) |
| 7 | 15 | 10 | 1 | 112F | `0x0646B0` | variante de 2 (no observada) |
| 8 | 30 | 20 | 2 | 112E | `0x0641EC` + `0x0C58D2` | lanzacohetes (tipo `51`: el prisionero `2D70C4` antes del jefe; Paso 15) |
| 9 | 30 | 15 | 1 | 112C | `0x0A716E` | variante de 4 (no observada) |
| A | 200 | 150 | FF | 112D | especial `0x01B716` | H (tipo `4F`, observada) |
| B | 200 | 150 | FF | 1130 | rayo | variante de 6 (no observada) |
| C | 40 | 20 | 3 | 1158 | `0x063F32` + `0x0C58D2` | C (observada) |
| D | 30 | 10 | 2 | 115A | `0x08606A` | desconocido |
| E | 30 | 10 | 3 | 115B | `0x0DE4DA` | desconocido |
| F | 20 | 5 | 2 | 1159 | `0x0DEA20` | desconocido |

Los nombres F/S/L y el significado de las variantes 7..B **no** se han
verificado; solo hay igualdad de sonido, tablas y código.

## 3. Disparo del jugador

Los disparos no se llaman desde código 68000: son **retrollamadas de los
programas de animación de Marco** en el banco 1 (palabras largas
`000C436A` en `0x25A074…`, `000C4372` en `0x24C70C…`, `000C453A` en
`0x2537D0…`, `000C4610` en `0x25A45C…`, `000C46D6` en `0x253596…`,
`000C4786` en `0x24C864…`, `000C48C0` en `0x25F6DE…`).

- `0x0C4372` (tabla `0x0C42AA`) / `0x0C436A` (tabla `0x0C42EA`) →
  `0x0C4376`: reserva `0x0C0D64` (ya traducida en `src/marco_ammo.c`): si el
  arma pedida es la actual, resta 1 a la munición salvo `FFFF` (o si
  `10F001` bit 3 / `10C2E7 ≠ 0`), y concede si `10C71A < 10C71C`. Con
  permiso: destello opcional de `0x0C432A[id]` (`0x0C58D2` para 3, 8, C,
  D, E, F) y luego id 5 → `0x01B6DC`, id A → `0x01B716` (`+93 = 2`:
  `0x01B668` / `0x01BDD2`), con `+95` incrementado dos veces y ángulo
  `+6E += 2·((+95 mod 3)−1)` (abanico de la H, `0x0C44A6`; `×3` con
  `+93 = 2`, `0x0C4506`); el resto, constructor de la tabla, `+95 += 1`,
  `+6E` = dirección (`0x0C4244`), `+70` = jugador. Posición `0x014A02` y
  velocidad heredada del soporte (`0x0C4834..0x0C4874`). Sin permiso,
  `0x0C0DFC`.
- `0x0C453A` / `0x0C4610`: H en diagonal (velocidad `0xC0000` / `0x100000`).
- `0x0C46D6`: ids 6/B, rayo `0x01E19E` → `0x01E166`.

Observado (`pickups2`, 2100..14500): 840 llamadas con `C42AA` (ninguna con
`C42EA`, que queda **no observada**); pistola `0x01B782` ×238; cohetes
`0x06381C` + `0x0C58D2` ×425 (ayuda); H `0x01B716` ×200 entre 11329 y 12386
(177 pasan por `0x0C447C`; las otras 23 no: `0x0C453A`/`0x0C4610` no
estaban instrumentadas); C `0x063F32` + `0x0C58D2`
(`pickups` 8376..9038, 40 disparos).

Constructores y programas (programa de animación y paleta tal como los
cargan `0x004256` / `0x009EA8`):

| Constructor | Programa | Paleta | Uso |
| --- | --- | --- | --- |
| `0x01B782` / `0x01B62C` | `0x22C12E` / `0x22C0A6` | `0x202` | pistola (común `0x01B7BA`: velocidad `+72`, ángulo `+6E`, ataque por dirección tabla `0x01B842` vía `0x0049E8`, rutina `0x01B882`) |
| `0x01B6DC` / `0x01B716` | `0x22C1B6` / `0x22C3BE` | `0x202` | H (ids 5 / A) |
| `0x01B668` / `0x01BDD2` | `0x22C000` / `0x22C4C6` | `0x202` / `0x874` | H con `+93 = 2` (no observado) |
| `0x06381C` | `0x2683F2` | `0x37C` | cohete (id 3) |
| `0x063F32` | `0x267F92` | `0x202` | C (id C), rutina `0x063FDC` |
| `0x0C58D2` | `0x0C586C` | `0x37C` | destello de boca (ids 3, 8, C–F) |
| `0x01BC2E` | `+7A` del creador | `0x202` | impacto creado por `0x01B882` (observado) |

## 4. Granadas

`0x0C4786` (programa de lanzar): resta una bomba de `10C720+p` (salvo
`10F001` bit 2); **sin bombas no lanza nada** (`pickups`: 121 llamadas con
`bombs=0` sin creación; 20 granadas creadas en total). Con tipo `10C722+p` = 1..3 toma la tabla
`0x0C4756` (`0x0C476E` si `+93 = 2`), dos constructores por tipo, el segundo
si `+90 = FF`: tipo 1 → `0x01BF46` / `0x01BF10`. Hijo en la lista
`0x101414`, `+6E` = jugador, salida a `0x0C482E`.

`0x01BF46` / `0x01BF10`: paleta `0x27C`, programa `0x22C950`, velocidad X
`±0x90000` sumada a la del jugador, Y `0x85B6A` / `0x6448F`, gravedad
`+84 = FFF053A0`, giro `+74 = ±0x320`, `0x0C0D34` (granadas en vuelo +1),
rutina `0x01BFD0` con ataque `0x01B5D2`. `0x0C48C0` (otra variante,
constructor `0x01BE56`) no se observó.

Observado: `pickups` 2943..3483 y 7803..8402, una granada `0x01BF46` cada
60 fotogramas (10→0 dos veces); `pickups2` 107 creaciones `0x01BF46`.

## 5. Fin del arma

En el procesamiento común del jugador `0x0C32F4`, `0x0C337A`: si la
munición `10C724+2p` es 0 (o el jugador está muerto, `+5F` bit 7), llama a
`0x0C0EE2` (id 0, `10C71A = 0`, máximo y munición de la tabla: `FF`,
`FFFF`) y, si `+93 = 1`, pide la forma 0 (`+94 = 0`). Observado:
`pistol_reset ... ret=000C339E` en `pickups` 9050 (C) y `pickups2` 12386 /
`vehicles2` 12211 (H), con `+93` 1 → 0 al fotograma siguiente.
`0x0C0EE2` también se llama al iniciar una vida (`ret=000C0E8A`, 10195).

## 6. HUD: ARMS y BOMB

El panel del jugador (`0x0C9650..0x0C9844`, actor con `+6E` = jugador y
`+78` = dirección en la capa fija):

- ARMS: `0x0C071C` devuelve la munición `10C724+2p`, o `FFFF` si el jugador
  monta un vehículo de tipo ≠ 0 y ≠ 5 (`0x0AAA88`), o con `10F001` bit 3 o
  `10C2E7 ≠ 0`. `FFFF` → símbolo infinito `0x0C91C2`; si no, 3 cifras con
  `0x03D068` en `+78+0x21`, parpadeo al cambiar (`+7A`, `+89` bit 0).
- BOMB: `0x0C0764` devuelve bombas y tipo del jugador, o, montado en un
  vehículo de tipo 1 u 8 (salvo abajo pulsado en el suelo) y de tipos
  distintos de 2, 5, 6, 7, los del vehículo (`+1F` munición, `+20` tipo).
  Rótulo: tipo < 4 "BOMB" (`0x0C91E6`), tipo 6 `0x0C91F2` (no observado), si
  no `0x0C91DA` ("CANNON"). 2 cifras en `+78+0xA1`.

Observado: `pickups/game8400.png` "ARMS 38 BOMB 1" (C); `vehicles2/game9600.png`
"ARMS ∞ BOMB 6" (Camel: tipo 2, bombas del jugador);
`vehicles2/game10560.png` "ARMS ∞ CANNON 8" (SV-001). El volcado de 12387
aún muestra "1" (el panel se actualiza después del cambio de arma).

## 7. Vehículos del jugador

### Ranura `10C608` (P1) / `10C62C` (P2), 0x24 bytes

Inicialización `0x0AA4F6` / `0x0AA50C`. Campos (escritores):

| Off. | Tam. | Significado |
| --- | --- | --- |
| +00 | pal. | palabra baja del nodo del vehículo (0 = libre) — `0x0AA7A4` |
| +02/+04 | pal. | asiento relativo X/Y (`D2/D3` de `0x0AA760`: Camel 0/`0x18`, SV-001 0/0) |
| +06 | larga | `+38` (soporte) del vehículo |
| +0A/+0E | larga | posición `+28/+2C` del vehículo |
| +12 | pal. | sello de fotograma `10000E` de la última atadura |
| +14 | pal. | `D4` de `0x0AA83A` (SV-001 `0x30`) |
| +16 | pal. | daño pendiente |
| +18 | pal. | salud del vehículo (copia de su `+4E`) |
| +1A | pal. | ángulo de la torreta vulcan (`0x0AABDC`, desde `0x03B5EE`) |
| +1C | byte | `FF` atado este fotograma, 0 montado, 2 bajada pedida, 1 recién liberado |
| +1D | byte | tipo de salida (`D1` de `0x0AA7FC`) |
| +1E | byte | causa (`+5A` del vehículo) |
| +1F | byte | munición del vehículo (obuses), tope 99 |
| +20 | byte | tipo de bomba del vehículo |
| +21 | byte | tipo de vehículo (Camel 2, SV-001 1) |
| +22 | byte | `+15` del vehículo (bit 6 = en el suelo) |

`10C2E8` (P1) / `10C2EC` (P2): puntero a un registro de 8 bytes que
`0x0128F4` copia a `10C01C` para los actores del jugador; `0x016DBE` por
defecto, `0x016DC6` para el tipo de vehículo 4 (`0x0AA87E`). **Uso no
verificado**; en Misión 1 solo se vio `0x016DBE`. No es el HUD.

### Selección, atadura y salida

- `0x0AA610` (con la prueba `0x0C0556`): cada jugador vivo, sin `+93 = 3`,
  **cayendo** (`+34 < 0`) y sin suelo (`+15` bit 6 a 0), cuya posición está
  dentro del rectángulo de `A0` respecto al vehículo (`0x0AA6B6`); si ambos,
  el más cercano en X, empate al azar (`0x0029C6`). `0x0AA606` usa
  `0x0C069E` (en el suelo, mirando al vehículo y con **solo A** como
  pulsación nueva; lo usa `0x095DC4`, no observado).
- `0x0AA760` (atadura, **cada actualización** del vehículo): escribe nodo,
  `+1C = FF`, sello, asiento, soporte y posición; devuelve `+6E` del
  jugador. El jugador, en `0x0C3352` → `0x0AA458`, acepta si `+1C ≠ 0` y el
  sello es el del fotograma (pone `+1C = 0`); si no, libera la ranura
  (`+1C = 1`, `10C2E8 = 0x016DBE`). `weapons.csv` muestra `+1C` alternando
  `FF/00` mientras se monta.
- `0x0AA83A` (al subir): `+18` = `+4E` del vehículo, `+21` tipo, `+1F`
  munición, `+20` tipo de bomba, `+14`. Observado: Camel `kind=2 ammo=0
  type=0 health=0` (9530), SV-001 `kind=1 ammo=A type=4 health=30` (10375).
- Jugador montado: `0x0C3078` (`+7A/+7B = FF`) → `0x0C309C`, que sigue la
  ranura con `0x0AA550` (posición = vehículo + asiento) y ejecuta el
  procesamiento común `0x0C32F4`.
- `0x0AA7FC` (bajar): `+00 = 0`, `+1C = 2`, `+1D = D1`, `+1E = +5A`. El
  jugador lo lee en `0x0C30F0` (`0x0AA5AC`): tipo 0 → **muerte** (`+5A` =
  causa, `+5F` bit 7, `0x0C2956`); 1 → salto de expulsión `0x0C3232`
  (`0x0C3250`); 2 → `0x0C32D0`; 3 → `0x0C1F4A`; 4 → `0x0C315A`; 5 → girado y
  `0x0C1F4A`. Observado: tipo 1 en 9592/11254/11099 y tipo 0 en 10091.

### Mando `0x0CA2CE`

Devuelve `A0 = 10E9EC` (P1) / `10E9F2` (P2): `+0` pulsaciones nuevas, `+2`
mantenidas (`0x0CA21E..0x0CA2CC`). Bits 0–3 palanca, bits 4–7 por la tabla
de máscaras `0x0CA1B0 = 10 20 40 30`: **bit 4 A, bit 5 B, bit 6 C, bit 7
A+B a la vez**. C mantenido anula B. `10E9F8/10E9FC` pueden sustituir la
lectura (guiones).

### Daño `0x0AA9E4` y destrucción

`0x0AA9E4` (llamado por los vehículos): si `0x004C24` detecta un impacto,
causa `+5A = 0x0F` → salud 0 y `+5F` bit 7; `0x0D` → salud = `+4E`; en otro
caso, salvo `+5F` bit 6, salud `−= 0x10` y a ≤ 0, `+5F` bit 7. Después suma
`+16` y copia a `+4E`.

### Camel Slug `0x03458E`

Constructor: torreta vulcan `0x03B4E0` con `+6E = &camel+78`, `+72 = 5`.
Espera `0x0345A0/0x0345E0` (`+4E = 0`) → `0x0AA610` (rect. `0x034320`) →
`0x0AA760(0, 0x18)` → `0x0AA83A(tipo 2, 0, 0, 0)` → `0x034698` (sonido
`0x116E`, jinete gráfico `0x034F1A`). Estados montada `0x03478C`,
`0x03479C`, `0x0346EA`, `0x0348D8`, `0x0348F0` (observados). Mando:
abajo → `0x034786`, derecha/izquierda → velocidad `±0x38000`, B → salto
`0x0349BE`, C → `0x035100`: si hay bombas y menos de 2 en vuelo, `0x0C0808`
(granada del jugador con su `+90 = FF` → `0x01BF10`), si no, `0x034FD2`
(programa de `0x23A59E`, sin efecto conocido); A+B → `0x034BA0` (expulsión,
`0x0AA7FC(1)`, programa `0x23A0C6`); abajo+B en el suelo → `0x0351E6`
(`0x0AA7FC(1)`); `+5F` bit 7 → `0x0351BC` (`0x0AA7FC(0)`: muere el jinete).
Al correr crea `0x059E30` vía `0x059EE0` (velocidad = −velocidad de la
Camel, programa `0x24C49C`, paleta `0x170`; probablemente polvo, no
verificado).

Observado (`vehicles2`): subida 9530; 8 granadas `0x01BF10` (bombas 8→1);
10 veces `0x034FD2`; 83 balas vulcan `0x01BE24` (`camelfam`); 10091
`camel_killed cause=2` con salud 0 (la Camel sube con `+4E = 0`, así que
cualquier impacto la "mata"), jinete muerto 10092, Camel de vuelta a
`0x0345E0`. `pickups2`: salida A+B en 9592.

### SV-001 `0x092A32`

Constructor: salud `+4E = 0x30`, obuses `+7B = 10`, tipo `+7C = 4`, torreta
`0x03B4E0` con `+6E = &tanque+86`, `+72 = 0`. Espera `0x092B0E` (sin salud →
`0x0938DA`; `+5F` bit 7 → `0x093AFA`) → `0x0AA610` (rect. `0x092A2A`) →
`0x0AA760(0,0)` → `0x0AA83A(tipo 1, +7B, +7C, 0x30)` → `0x092C18`
(`0x0954DC`: animación de entrada según `+93` del jugador). Montado
`0x092CC2`: derecha/izquierda, B → `0x09333A`, abajo → `0x093138`, A+B →
`0x093A08` (salida), C → `0x092934`: con `+8A = 0` y obús disponible
(`0x0AA956`: `+1F − 1`, salvo `10F001` bit 2), `+8A = 0x18`, destello
`0x0952F8` (paleta `0x208`, programa `0x29090E`) y obús `0x094EAA`
(`0x0950DC` si `+7C = 5`; programa `0x290FC0`, paleta `0x258`). Daño
`0x093772` → `0x0AA9E4`. `0x0929E6` (bombas por `0x0C0808`) no se observó.

Vulcan: la torreta `0x03B4E0/0x03B57C` (común a la Camel) busca su jinete
(`0x0AAB84`), lee el mando: A nuevo → ráfaga (`+76 = +74`, `+77 = +75`, por
defecto 5 balas cada `+73 = 2`); A mantenido repite (`0x0CA614`); la
palanca da el ángulo destino (tabla `0x03B4B0[+72]`) y `+7C` gira hacia él.
Bala `0x01B6A0` (programa `0x22C1B8`, paleta `0x3D6`) o `0x01BE24` si
`+72 = 5` (Camel), velocidad `0x1C0000` en el ángulo `+7C` ± 2 alterno.

Observado (`vehicles2`, `tankfam`): subida 10375; 9 obuses
(`tank_cannon`, 10416..11057, `+1F` 10→1); 152 balas `0x01B6A0` de la
torreta `101784`; 23 `0x0956CE` (programa `0x291AF0`) del estado
`0x092DDA`; salida A+B 11099 (tipo 1). La salud quedó en `0x30`. Una
ejecución más larga (`tanklong`: `PLAN=vehicles RIDE=2400`, igual que
`vehicles2` hasta 10376, tanque 10376..12777, salida A+B 12777) tampoco
recibió daño: **daño y destrucción del SV-001 no observados**. La
voladura tras la salida (`0x093AFA`), los saltos, el agachado con bombas y
el empuje del cuerpo del lanzacohetes sí se capturaron después (paso 10).

## 8. Desconocido / no observado

- Nombres de las armas 2, 4, 6, D, E, F y significado de las variantes
  7..B; arma 1.
- Tabla de disparo `0x0C42EA`, H diagonal con `+93 = 2`, rayo, granada
  `0x0C48C0`, tipos de bomba 2–5 (`0x0C0A7C`).
- Formas 2 y 3 del jugador; comida en misiones 4–10.
- Registro `10C2E8`; tipo de vehículo 4; salidas 2–5.
- Daño, destrucción y bombas (`0x0929E6`) del SV-001.
- `0x034FD2` de la Camel; `0x059E30` (¿polvo?); `0x0956CE` del tanque.
- Recogida por el jugador 2.

## 9. Plan de port recomendado

Orden por dependencias; cada familia con la fixture que usaría (todas
extraídas de `/home/user/captures/weapons/`, fuera de Git, como
`data/*_fixture.json.gz` privados).

1. **Bloque de armas y concesión** (`0x0C0928`, `0x0C0B32`, `0x0C0EE2`,
   `0x0C0D34/0x0C0D48`, la comprobación `0x0C337A`; ampliar
   `marco_ammo.c`). Fixture: `weapons.csv` de `pickups` y `pickups2` (bloque
   `10C718..10C72F` antes/después de cada `BP_0C0928/0C0B32/0C0EE2`), prueba
   de tabla sin emulador.
2. **Recogida de objetos** `0x08B568..0x08B9B0` + rótulo `0x08D330` +
   `0x057948`; sustituir la parada explícita de `neo_item.c`. Fixture:
   `hfam` (`extract_family_fixture.py --range 8c37c-8c700`: 67
   actualizaciones de objeto, `0x08C53C`) y la `hitem` existente sin
   `--stop`.
3. **Disparo** (`0x0C4376`, `0x0C453A`, `0x0C4610`; retrollamadas de
   programa en el motor de animación de Marco) y **balas** pistola/H
   `0x01B6DC..0x01BD00`, cohete `0x06381C`, C `0x063F32`, destello
   `0x0C58D2`. Fixture: `hfam` (`--range c0000-c5000 --range 1b600-1bd00`:
   2773 actualizaciones, 200 constructores `0x01B716`, 1250 `0x01B882`,
   64 impactos `0x01BC2E`, 576 del jugador) y `cfam` para C (`--range
   c0000-c5000 --range 1b600-1c400 --range 63e00-64200 --range c5800-c5a00`:
   40 `0x063F32`, 619 `0x063FDC`, 4484 `0x063E48`, 40 destellos `0x0C58D2`).
4. **Granadas** `0x0C4786` + `0x01BF10/0x01BF46/0x01BFD0`. Fixture: `cfam`
   (10 granadas `0x01BF46`, 176 actualizaciones `0x01BFD0`, 81 `0x01C19A`,
   3 `0x01C152`).
5. **HUD ARMS/BOMB** `0x0C9650..0x0C9844` con `0x0C071C/0x0C0764`. Fixture:
   capa fija de los volcados `vram*.bin` en los eventos (cambio de arma,
   bombas, subida) y `weapons.csv`.
6. **Ranura de vehículo y jugador montado** (`0x0AA458..0x0AABF2`,
   `0x0C3078..0x0C3260`, salida `0x0C30F0`, mando `0x0CA21E..0x0CA2DE`).
   Fixture: `camelfam`/`tankfam` (nodo `1014C4`) y los bytes de ranura de
   `weapons.csv`.
7. **Torreta vulcan** `0x03B4E0/0x03B57C` y balas `0x01B6A0/0x01BE24`.
   Fixture: `tankfam` (`--range 3b4e0-3b900 --range 1b600-1bd00`: 314
   actualizaciones de torreta, 152 `0x01B6A0`, 1111 `0x01B882`) y
   `camelfam` (229 de torreta, 83 `0x01BE24`).
8. **Camel Slug** `0x03458E` (`0x034308..0x035600`, hijos `0x034F1A`,
   `0x034FD2`, `0x059E30`). Fixture: `camelfam` (`--range 34300-35600
   --range 59e30-59f00`: 1865 actualizaciones con torreta y balas).
9. **SV-001** `0x092A32` (`0x092A00..0x095800`, obús `0x094EAA`, destello
   `0x0952F8`, `0x0954DC`, `0x0956CE`). Fixture: `tankfam`
   (`--range 92a00-95800`: 3050 actualizaciones con torreta y balas) más
   `data/cannon_fixture.json.gz`. Antes,
   capturar daño y destrucción (falta evidencia).

## 10. Port

### Paso 1: bloque de armas (`src/marco_ammo.c`)

`marco_weapons_t` guarda los bytes originales `10C718..10C72B` de los dos
jugadores (`10C72C`/`10C72E` no son de armas). Traducido: partida nueva
`0x0C0E20`, vida `0x0C0E6E`, concesión `0x0C0928` (tablas `0x0C08A8..0x0C0927`
con los valores originales), bombas `0x0C0B32` con `0x0AA898` sobre la ranura
de vehículo (tipo 4: lee `0x0C0764`, **parada explícita** `MW_STOP`), pistola
`0x0C0EE2`, comprobación de fin `0x0C337A`, lanzamiento `0x0C478E`/`0x0C0808`,
granadas en vuelo `0x0C0D34`/`0x0C0D48` y la muerte `0x0C2FA4` (en vuelo, id,
bombas y munición a 0; la encontró la prueba en `vehicles2` 10194).

Prueba `tools/test_marco_weapons.c` (`make check-core`): fixture
`data/weapon_block_fixture.json.gz` (`tools/extract_weapon_block.py
pickups pickups2:9400 vehicles2:9400`), bucle cerrado sobre 1840 fotogramas
con cambio de bloque y 1849 eventos, bit a bit. La fila de `weapons.csv` del
fotograma f se lee en un punto del fotograma: un evento de f puede quedar
antes o después; el extractor lo decide con el estado que registra el propio
punto de ruptura. Los decrementos sin punto de ruptura (`0x0C0D48`,
`0x0C0DFC`, reservas rechazadas por el tope, la muerte) se infieren del
bloque y la prueba los aplica con las rutinas nativas.

### Paso 2: recogida de objetos (`src/neo_item.c`)

La parada explícita de la recogida desaparece para todo lo que la Misión 1
usa. `neo_genv_t.pickup` (`neo_pickup_env_t`) lleva el bloque de armas, los
campos del jugador, las ranuras de vehículo, la puntuación BCD `10C46A`, el
valor `10C5A0`, el sello `10C594`, el contador `10C590`, el combo
`10C5AC/10C5BE` y la misión `10C2F6`. Traducido:

- Armas `0x08B568..0x08B5BA` → `0x0C0928`. **Aceptada no da puntos**
  (`0x08B5CA BCS` → `0x08B87A`, valor a 0); rechazada, puntos (`0x08B896`,
  `0x08B850`) y sonido `0x1121`. Comprobado en `pickups2`: puntuación igual
  en 11193/11200 y `10C5A0` 300 → 0 al tomar la H.
- Bombas `0x08B6D0` → `0x0C0B32`; aceptadas: valor 0 y el rótulo `0x08C752`
  de `0x08CD10` (actor externo, contado en `labels`); rechazadas, puntos.
- Puntos/comida `0x08B7FC`: sonido `0x1121`, comida solo en las misiones 4–7,
  9, 10 (`0x0C0BAE`; en ellas **parada explícita** `0x0C0C14`), valor y suma
  BCD `0x057948` (con desbordamiento a 999999).
- Valor `0x08B896`: registro `+0C`, tabla especial `0xE5526` con `+7B`, o
  combo `0xE5556`/`0xE559A` (registro byte 0 y ≤ 15 fotogramas). El rótulo de
  puntuación `0x08D330` es un actor externo contado (`popups`); un valor
  `> 0x50000` es el `TRAP #F` del original (parada explícita).
- Después `0x08C628`: chispa `0x08C68E/0x08C6B2` (programa `0x28617E` hasta su
  fin; la comprobación de cartucho `2FFFE0` acaba en `0x004188` en hardware
  genuino, como la otra rama) y dos actualizaciones de `0x08C66A`.
- Paradas explícitas que quedan: tipos de bomba `0x08B5F4..0x08B60C`
  (`0x0C0A7C`), `0x08B650`, `0x08B710`, `0x08B750`, `0x08B78E`, `0x08B7CC`,
  vehículo de tipo 4 (`0x0C0764`), recogida del jugador 2.

Pruebas (`make check`): familia `hitem` (H tomada en 8081, ya sin `--stop`:
arma 0A con 200 balas y las dos actualizaciones de `0x08C66A`) y familia nueva
`citem` (`data/citem_fixture.json.gz`, de `cfam`: C tomada en 8370, arma 0C con
40 balas, la chispa comparada byte a byte). `hfam` no contiene ninguna
recogida en la familia de objetos (la H de 11199 se creó antes de la ventana
de la traza; su único objeto, `2A`, no se toma): no aporta fixture de
recogida. Los puntos de los objetos `2A`/`2B` (100) los suman las carreras de
`tools/test_gameplay.c` (puntuación 100 y 300).

### Jugador del port (`src/gameplay.c`)

`neo_world_t` guarda el bloque de armas y los campos del jugador 1
(`0x0C0E20`/`0x0C0E6E` al empezar; `0x0C2FA4` + `0x0C0E6E` al morir). Cada
actualización del jugador copia `+94` a `+93` (lo observado en
`weapons.csv`) y aplica `0x0C337A`. Un disparo reserva con `0x0C0D64` del
arma actual; las balas del port liberan con `0x0C0DFC` al desaparecer. El
botón Z lanza una bomba (`0x0C478E`).

### Paso 3: balas (`src/neo_bullet.c`)

Traducidas: constructores `0x01B6DC`, `0x01B716` (sonido `0x10F4` limitado
por `0x01B60E`, RAM `10C33E`), `0x01B750`, `0x01B782`, común `0x01B7BA`, vuelo
`0x01B882` con la tabla de filas `0x01B842` por `0x0049E8` (soldados,
cajas registradas —cúpulas, tienda, blindado— y, para estas balas, las del
jefe), rebote `0x01BBE2`, impactos `0x01BC2E/0x01BC82`, impacto en pared y
límites de la vista. `0x013CF8` (agua bajo la bala) está en `neo_kernel.c`;
la salpicadura `0x03C962` es **parada explícita** (no hay celdas de clase 5 en
la Misión 1).

Pruebas de familia: `hbullet` (`hfam`: 200 balas H, 64 impactos, 2197
actualizaciones) y `pbullet` (`cfam`: 36 balas de pistola). Las balas las crea
el jugador (fuera de la familia): empiezan con sus bytes grabados. El impacto
se crea con `0x00411C` en la cabeza de la lista y ningún enlace nombra a su
creador: `compile_family_fixture.py --hit-child/--hit-from` lo asigna a la
bala muerta más cercana (como mucho 3 fotogramas antes) y solo esa muerte
responde un contacto de `0x0049E8`.

En el juego, la H (`0x0C4454/0x0C447C`) dispara balas nativas `0x01B716` con
el abanico `0x0C44A6` (`+95` dos veces por disparo); se insertan tras la
cabeza de la lista al final del tick del mundo y corren desde el siguiente.
**H en diagonal** (`src/marco_hgun.c`): los estados del torso con la H del
original: disparo `0x0C3E9C`, ráfaga `0x0C3F1A` (antes de `+76` un cambio de
puntería abre un barrido; después, una nueva pulsación `+9B` bit 4 reinicia
la ráfaga), barridos `0x0C3FA6..0x0C3FDE` / `0x0C3FFA` / `0x0C4006`, con los
programas de Marco `+93 = 1` de las tablas `0x0BFDD8`, `0x0BFE18`,
`0x0BFE58` (ráfagas: `0x0C4372` en las actualizaciones 0/2/4/6, `+76` en la
7, fin en 12/16/19) y `0x0BFE98`, `0x0BFED8`, `0x0BFF18`, `0x0BFF58`
(barridos: `0x0C453A` con los ángulos 08/18/28/38, F8/E8/D8/C8, 30/20/10,
D0/E0/F0; fin en 8 o 6). `0x0C4244` da 0x40/0/−0x40 según `+92` (a la
izquierda `0x80 − ángulo`) y `0x0C44A6` suma `+95` **una** vez por bala
(el port sumaba dos: `hfam` muestra 0, +2, −2) y el abanico 2·((+95 mod 3)−1);
`0x0C453A` refleja su ángulo con la orientación de ese momento y también
cuenta `+95`. Una granada pone `+92 = 0`. La pulsación en reposo arranca el
estado en la actualización siguiente; el fin de un programa con la petición
de disparo encadena el siguiente estado en la misma actualización (`hfam`:
la ráfaga hacia arriba dispara dos actualizaciones después del último
diagonal). Prueba `tools/test_marco_hgun.c` (`make check-core`): estados con
entradas guionizadas y las 22 balas diagonales registradas en `hfam`
(`data/hgun_fixture.json`), todas pasos de un barrido en orden y con el
reflejo de su orientación. `hfam` no tiene la columna `10000E`, así que los
fotogramas de las actualizaciones no se comparan.
**Aproximaciones declaradas**: el reposo `0x0C38B2` no se traduce (`+92` se
conserva, como en `hfam` 12308..12312); el giro `0x0C3BCC` no existe en el
modelo de jugador del port (la puntería hacia abajo en el aire ya sí: ver
«Puntería hacia abajo en el aire» más abajo); la boca
sigue siendo la de pistola del port (adelante (24,80), arriba (3,108),
interpolada para un diagonal), porque el port no tiene `+72/+74`. Disparar
exige pulsar (como en el arcade); `tools/test_gameplay.c` pulsa cada 8
pasos. La pistola sigue
con la bala del port (la del jefe y el blindado de referencia pasan por
`neo_world_shot`). **Paradas explícitas**: H con `+93 = 2` (`0x01B668/0x01BDD2`),
cohete `0x06381C`, C con `+93 = 2` (`0x063F10`; la C normal es nativa, paso 11) y las demás armas
(`weapon_stops`, se disparan como bala de pistola del port).

Gráficos: `tools/export_rest_sprites.py` añade los programas de la chispa
`0x28617E`, las balas, los impactos, la granada y su explosión (paletas
`0x202`, `0x27C`, `0x378`). Orden documentado en `MISION1_ACTORES.md`
(con `veh2@88b52-89940 box1@9a67e-9ae80`) más `hfam@1b600-1bd00
cfam@1bd00-1c400 cfam@8c37c-8c700`: 1330 descriptores (98 nuevos sobre los
1232 de la integración), 42 paletas (faltan las mismas 4 sin captura), 161
volcados validados contra VRAM (1409085 píxeles iguales, 8068 distintos).

### Paso 4: granadas (`src/neo_bullet.c`)

Traducidas: `0x01BF10/0x01BF46` (programa `0x22C950`, paleta `0x27C`),
`0x03C526` (en la Misión 1, `10C2F6 = 0`: sin onda de agua; otras misiones,
parada explícita), `0x0C0D34`, vuelo `0x01BFD0` (fila `0x01B5D2`, rebote
`0x01C6AC` con la palabra de superficie D4 = RAM `10C068` de `0x013968`,
`0x00505A`, giro `+72/+74`, dos botes), `0x0C0D48` y la explosión
`0x01C152/0x01C19A` (sonido `0x1027`, programa `0x22CB60`, filas `0x01B5E2`
—tipo 9, daño 100— y `0x01B5F2`). La muerte de soldado en el suelo por tipo
9 es `0x040C8E` (tabla `0x0406EA`), como el tipo 1: se añadió a
`neo_soldier_ai.c` (antes era una parada).

Prueba de familia `grenade` (`cfam`: 10 granadas, 270 actualizaciones, tres
explosiones). `--hit-to 1c152`: la granada que explota por `0x0049E8` antes
de volar es el contacto. La traza guarda ahora la lectura de `10C068`.

En el juego, Z lanza `0x01BF46` (o `0x01BF10` si `+90 = FF`: quieto) con la
posición de `0x0C4806` aproximada (`+72/+74` del jugador = −1/0x15, el valor
de pie de `cfam`; el port no tiene esos campos) y la velocidad del jugador
(`0x0C4834`). Otros tipos de bomba: parada explícita (`grenade_stops`).

### `tools/test_gameplay.c`

La carrera de la misión toma la H y dispara sus 200 balas nativas (18
aciertan), vuelve a la pistola (`0x0C337A`) y lanza sus 10 granadas al
blindado de la parada 1304, a las cúpulas y a la tienda (9 filas de
explosión aciertan). Las carreras de lanzamiento sirven todos los objetos que
tocan (ninguna parada de recogida; puntuación 100/300 con `2A`/`2B`),
disparan la H y lanzan granadas al azar y al blindado.

Duración de la parada 1304 (`2D67CA`, la arcade: 385): carrera de la misión
668 → **194**; carreras de lanzamiento 588/746/544/752 → **210/740/312/198**
(pasos totales 40084/44750/40398/38494 → 30704/35146/29054/30210). Con la H
sola (sin granadas) no cambiaba: la H se toma después de 1304.

### Paso 5: HUD de capa fija — ver §11

### Semántica del disparo (captura `hold`, `tools/capture_weapons.lua` plan `hold`)

`/home/user/captures/weapons/hold2`: el jugador quieto en el inicio de la
misión **mantiene A** (280 de cada 300 fotogramas) de pie, agachado, apuntando
arriba y saltando, con la pistola y luego con la H (id `0A`, 200 balas, `+94 =
1` escrito en 3400), y al final pulsa A cada 4 fotogramas. Resultado
(`debug.log`, puntos de ruptura `0x0C438E`, `0x01B782`, `0x01B716`):

- **Pistola: un disparo por pulsación, nunca autodisparo.** Pulsación en
  2190 → retrollamada `0C4372` en 2191 (misma actualización) → constructor
  `01B782` en 2193 (una actualización después, `00411C` en la retrollamada);
  mantener A 280 fotogramas no dispara más (2491, 2791, 3091, 3391: uno por
  pulsación en cada postura). Mecanismo: `0x0C0FEA` hace `+9B |= nuevas
  pulsaciones & +9C` (`10E9EC+0` son las pulsaciones nuevas, `+2` las
  mantenidas, `0x0CA1B8`; `+9C` vuelve a `FF` en `0x0C3770` cada
  actualización); el estado `0x0C3CD0` borra `+9B` bit 4 al empezar el
  programa y `0x0C3D9A` solo vuelve a disparar con `+76` del programa **y**
  `+9B` bit 4 (una pulsación guardada). Programas: nivel `24C70A` (`0C4372`
  en 0, `+76` en 1, fin 20), arriba `24C7F2` y abajo `24EABC` (`+76` en 2):
  cadencia mínima 2 (3) actualizaciones entre retrollamadas.
- **H: una ráfaga por pulsación** (de pie/agachado 4 balas en 3693, 3697,
  3701, 3705; apuntando arriba 4; en el aire 3), nada más mientras se
  mantiene; pulsando cada 4 fotogramas dispara una bala cada 4 fotogramas
  (4595..). `+95` sube **una** vez por bala (`0C4434` entra en `jsr 1AC94`,
  no en el `addq` de `0C4430`): ángulos 0, +2, −2 como en `hfam`. Lo que ya
  hacía `marco_hgun.c`; las capturas anteriores no mantenían A (`a =
  frame%12<6`).
- **Boca** (`0x0C4244`, tabla `0x0C4204`, 3 filas por `+92` × 4 formas,
  palabras (x,y) en píxeles respecto a `+28/+2C`, y hacia arriba): abajo
  `(0,-16) (-4,-20) (16,0) (8,-16)`, nivel `(16,10) (24,4) (24,8) (20,4)`,
  arriba `(0,40) (0,48) (8,32) (5,32)`; barridos `0x0C453A` tabla `0x0C4234`
  `(16,28) (16,16) (16,20) (16,46)`; se suma `+72/+74` del programa de
  piernas y en `0C43CC` se resta otra vez salvo `+90 = FF`. Medido: pistola
  de pie `(16,27)` (`+74 = 17`), agachado `(16,16)` (`+74 = 6`); H de pie
  `(28,16)` (`+72/+74 = 4/12`), corriendo `(26,23..25)`. El ángulo del
  abanico (±2) y de los barridos sale de ese único punto.

Port (`src/gameplay.c`): `fire_request` es `+9B` bit 4 (pulsación nueva,
guardada hasta consumirse; mantener no dispara), `pistol_wait` las
actualizaciones hasta `+76` + 1 (2 nivel, 3 arriba/agachado) y
`pistol_pending` retrasa la bala una actualización.

**Boca (`src/marco_legs.c`).** `+72/+74` los escriben los programas de
cuerpo en cada pose (`WRITE_WORD`/`WRITE_LONG`): reposo `24C5EA` (0,17),
carrera `24DD18` (−1, 17..21, bucle desde la quinta pose), salto `24E4E0`
(1, 24..18, espera arriba mientras está en el aire), salto con carrera
`24E858` (−3/−4, 15..11) y después `24DF6C` (−1,15). `marco_legs.c` lleva
esas líneas de tiempo con los mismos arranques que `marco_visual.c`.
Agachado ningún programa los escribe: se usan los valores medidos (H (4,12),
pistola (0,6)), **aproximación declarada** (escritor no localizado).
`0C4244` (con `+90 ≠ FF`, todos los nacimientos medidos): tabla `0C4204` por
`+92` y forma más `+72/+74`, x negada mirando a la izquierda (la H y la
pistola: de pie (16,27)/(0,57), como el original; la carrera guionizada de
`tools/test_gameplay.c` sigue liberando a los prisioneros). `0C453A`
(barridos) **no usa la tabla `0C4234`**: x = parte entera de 12·cos θ más
`+72`, y = parte entera de 12·sen θ (más la mitad si θ < 0x80) más `+74` + 2,
con θ ya reflejado a la izquierda (`002E7C`, tabla de senos `0029FC`); el
port usaba (16,16) reflejado, de ahí los diagonales erráticos en hardware.
Prueba `tools/test_marco_muzzle.c` (`make check`,
`data/hmuzzle_fixture.json` de `tools/extract_hmuzzle_fixture.py` sobre
`hold2`, `hfam`, `hudp2`, `hudv2`): los **34 nacimientos de balas H** (29 del
abanico ±2, 5 de barrido, a izquierda y derecha) son, píxel a píxel, la boca
de su puntería con el `+72/+74` de una pose del programa de cuerpo de la
rutina del jugador. La velocidad y la trayectoria de cada bala las cubre la
familia `hbullet`.

**Forma 1 y muertes (resuelto):** la forma 1 (H) y las muertes tienen sus
programas y poses en el catálogo; ver [MARCO_CAMPAIGN.md](MARCO_CAMPAIGN.md),
«Forma 1 (H) y muerte». El renderizador usa la fila `+0x10` con `+93 = 1`;
mientras muere, yace (dibujo cada dos actualizaciones: parpadeo) y entra,
dibuja la pose del programa de `marco_death.c` (`marco_death_draw`, paleta
quemada en `0BF358`). `tools/test_marco_death.c` compara además, en las 8
muertes de `marco-d2`, la pose de cada actualización mientras muere (315
poses) y el número de dibujos mientras yace, iguales.

## 11. HUD de capa fija (`src/neo_hud.c`, `src/hud_renderer.c`)

El HUD del original es la capa fija del Neo Geo (VRAM `7000..74FF`, 40
columnas de 32 filas, palabra = paleta<<12 | tile S-ROM). `neo_hud.c` es un
modelo de ese mapa escrito por traducciones de las rutinas del juego:

- panel en juego `0x0C9650..0x0C983E` (vidas `0C96BE`, puntuación `0C96FC`,
  ARMS `0C9738` con `0C071C`, BOMB `0C979E` con `0C0764`, indicador del
  vehículo `0C9B94`, estrellas de prisioneros `0C9D08`);
- panel P2 inactivo `0C9380` (INSERT COIN / PRESS START / GAME OVER con el
  parpadeo `0C9228`), texto de créditos `01A7F4`;
- cifras del tiempo `0C8FDC` del actor de misión (`101364` +7A = `1013DE`),
  cuenta atrás `0186FA..0187A2` (condiciones `0193FE`, `104303` bit 7,
  `10C2E7`, `104300`, `0C8CFC`, `107003`; pitido `0x10D7`), reiniciado por la
  entrada `01913C` de la tabla de apariciones (`neo_world.c` la cuenta en
  `timer_resets`; valor inicial `003586`, índice DIP `10FD8C` = 0: 60);
- servicios de texto `03CF12/03CF60/03CF9A/03D004/03D050/03D068/03D0D0/
  03D244/03D25A(03DD96)`.

Paradas explícitas (contadas en `unsupported`, registradas una vez): escrituras
fuera de `7000..74FF` (TRAP #F del original), páginas de glifo distintas de
<10 y `a`, fin de tiempo `0C0A2C`. El fin de partida `0C9848` (continuación y GAME
OVER) está traducido: ver MISION1_COMPLETA.md, «Fin de partida y continuación».

**Orden en el fotograma.** El juego corre cada dos fotogramas de vídeo y una
actualización ocupa dos: `10000E` cambia en el fotograma N, pero algunos
actores (el jugador) terminan en N+1. Los paneles leen el estado completo de
la actualización anterior, es decir, la fila de `weapons.csv` del fotograma
N-1. Las apariciones `01913C` corren después del actor de misión (la
actualización del reinicio muestra `xxFF`, no `xxFD`).

**Prueba** (`tools/test_neo_hud.c`, `make check-core`): `data/hud_fixture.json.gz`
(`tools/extract_hud_fixture.py` sobre las capturas `hudc`, `hudp2`, `hudv2`
de `tools/capture_weapons.lua`; solo RAM/VRAM de las capturas) tiene 993
intervalos de 30 fotogramas entre volcados. Cada intervalo parte del mapa,
los campos de los paneles, los contadores de parpadeo y el tiempo del
volcado f0 y reproduce cada actualización con las entradas de `0C071C`/
`0C0764`; las filas 3, 4, 5, 28 y 29 deben coincidir palabra a palabra con el
volcado f1 (198600 palabras) y el tiempo con `1013DE` (989 intervalos; 4 se
excluyen porque un indicador de RAM de la cuenta atrás, conocido solo en f0 y
f1, cambia dentro del intervalo). Además, la primera actualización con el HUD
encendido redibuja el marco completo del primer volcado (178 palabras; se
excluye la fase de parpadeo de INSERT COIN). Resultado: 0 diferencias.

**Dibujo** (`hud_renderer.c`): tiles de `rom:/fixed.bin` (S-ROM,
`tools/import_rom.py`) con las paletas fijas `rom:/hud-palette.bin`
(`tools/build_hud_assets.py` desde volcados de paleta de las capturas; fuera
de Git). Una celda se rasteriza en una tira RGBA16 de 304x224 solo cuando su
palabra cambia; cada fotograma copia (modo COPY) el tramo de columnas no
vacías de cada fila visible, sobre todas las capas, con tijera en la ventana
304x224.

## 12. Vehículos del jugador: port (pasos 6–9)

### Paso 6: ranura de vehículo y jinete (`src/neo_vslot.c`)

Traducción manual de los servicios `0x0AA458..0x0AABF2` sobre la ranura de
0x24 bytes guardada con sus bytes originales (`neo_vslots_t`: `10C608`,
`10C62C` y los registros `10C2E8/10C2EC`): aceptar/soltar del jugador
`0x0AA458` (con `0x018F24`), bajada `0x0AA4C8`, reinicio `0x0AA4F6/0x0AA520`,
seguimiento del montado `0x0AA550`, lectura de la salida `0x0AA5AC` (hasta su
`0x0143EE`), selección `0x0AA610` con la prueba `0x0C0556` (rectángulos
`0x034320`/`0x092A2A` en `data/region_rom.json`), atadura `0x0AA760`, salida
`0x0AA7FC` (el original nombra el nodo del P1 también para el P2: sin efecto),
subida `0x0AA83A`, obús `0x0AA956`, daño `0x0AA994`/`0x0AA9E4`, `0x0AAA88`
(tipo 0: el `TRAP #F` del original, parada explícita), `0x0AAB66`,
`0x0AAB84`, `0x0AABDC`, y el mando `0x0CA1B8` (máscaras `0x0CA1B0`: bit 7 =
A+B a la vez; con la tabla por defecto C no anula B).

Prueba `tools/test_neo_vslot.c` (`make check-core`): fixture
`data/vslot_fixture.json.gz` (`tools/extract_vslot_fixture.py
camelfam=… tankfam=… --routing vehicles2/weapons.csv --routing
pickups2/weapons.csv`): **las 3918 llamadas** a estos servicios de las
trazas de familia `camelfam` y `tankfam` (Camel, SV-001, torretas y el nodo
del jugador `1014C4`: 476 ataduras, 551 aceptaciones, 64 selecciones, las
salidas de tipo 0 —muerte del jinete, 10091— y 1 —A+B, 11099—, subidas,
obuses y daño), con los argumentos del llamador (inmediatos del sitio de la
`JSR`, `+7B/+7C` del SV-001) y lo que cada llamada leyó del nodo y de los
jugadores. Las ranuras son estado nativo en bucle cerrado: toda lectura de
la ranura por el original ve el valor nativo, y tras cada llamada ranuras,
registros y bytes de nodo escritos coinciden byte a byte (282096 bytes de
ranura). Además, 3533 muestras de `weapons.csv` (entradas estables) dan la
palabra mantenida de `0x0CA1B8`, 3 de ellas con A+B (`0xB8`).

En el juego (`neo_world.c`): ranuras libres al empezar, el jugador 1 llama a
`0x0AA458` en cada actualización, la recogida de bombas (`0x0AA898`) y el HUD
(`0x0C071C`/`0x0C0764`: ARMS ∞ y CANNON) leen la ranura del P1. Ningún
vehículo nativo la ata todavía.

### Paso 7: torreta vulcan (`src/neo_vulcan.c`)

Traducido el control de `0x03B57C..0x03B808`: fin si el vehículo no existe,
`+5F` bit 7 o el ancla `0x8000`; espera con `0x8080`; `+84 >>= 1`; jinete
`0x0AAB84` (sin jinete: ráfaga a 0); ángulo a la ranura; A nuevo → ráfaga
(`+76 = +74` si no había, `+77 = +75`, `+89 = 8`); A mantenido con
`0x0CA614` (`10F084+p`) repite cada 8; destino por la palanca (tablas
`0x03B4B0[+72]`: `0x03B424`, `0x03B464`, `0x097E08`, `0x097E48`), giro con
velocidad `+7E + +80` (aceleraciones hacia `0xFA00`/`0x3000`), sentido `+79`
(el `spl` mira los indicadores de `cmpi.w #$8000`: `FF` con diferencia
negativa); disparo cada `+73` con el abanico `+7A` (−2, 0, +2 solo si el
ángulo no cambió), bala `0x01B6A0` (`0x01BE24` con `+72 = 5`) con `+6E` y
`+5E`; `+88` = pose. Prueba `tools/test_neo_vulcan.c` (`make check-core`,
`data/vulcan_fixture.json.gz` de `tools/extract_vulcan_fixture.py`): las 543
actualizaciones de torreta de `camelfam` (229) y `tankfam` (314), campos de
control `+5E`, `+72..+89` en bucle cerrado, las **235 balas** (83 `0x01BE24`,
152 `0x01B6A0`) con su `+6E`, 101 con abanico, y la chispa.

**Paradas explícitas** (sin traducir, no se adivinan): la colocación de la
torreta sobre el ancla `0x03B80C..0x03B86C`, los constructores de bala
`0x01B6A0`/`0x01BE24`, el barrido `0x014A02`, la chispa `0x03BA76` y el fin
`0x03B86E`; la Camel Slug `0x03458E` y el SV-001 `0x092A32` (con su obús
`0x094EAA`, destello `0x0952F8`, `0x0954DC`, `0x0956CE`): el mundo no los
crea, cuenta cada aparición del guion en `vehicle_stops` y el juego lo
registra (`MSX vehicle stopped explicitly`); `tools/audit_script_events.py`
los lista como paradas explícitas. Daño y destrucción del SV-001 siguen sin
captura.

### Paso 8: Camel Slug (`src/neo_slug.c`)

Traducción manual de `0x034408..0x0354B8` (Camel, jinete gráfico
`0x034F1A`, efecto `0x034FD2`, marcador `0x03523C`) y de la torreta
`0x03B4E0/0x03B57C` completa salvo su fin y su chispa (control en
`neo_vulcan.c`; colocación sobre el ancla `+78` de la Camel, destello
`0x0044F6`, programa `0x23CE0C`), las balas `0x01BE24` (y `0x01B6A0` del
SV-001) en `neo_bullet.c` y el polvo `0x059E30` en `neo_rocket.c`. La Camel
lee la ranura y el mando del jinete por `neo_genv_t.vehicle`
(`neo_vehicle_env_t`: ranuras, nodos de los jugadores, palabras de mando,
`10F084`, `10C300/10C301`, `10F000/10F001`); las bombas del jinete por
`0x0C0808` (bloque de armas). Pasos de física: `0x014594`/`0x0143EE`
(`0x0128E8`: `10C00D` puesto y el recuadro de pantalla del registro del
jinete, `0x0138A0`, ahora en `neo_kernel.c`; `0x004ED2` con la tabla de
cuerpos sólidos vacía en todas las trazas) y `0x0143AC`/`0x014574`
(`10C00B` y `10C00D`: sin recuadro). Paradas explícitas: agua (`0x034AC0`,
tipo 5), fin `0x03B86E` y chispa `0x03BA76` de la torreta, el recuadro
vertical de `0x0138A0` (`10C00C` = 0), un cuerpo sólido registrado
(`0x004ED2`), los hijos `0x03533A..` no creados en las trazas. No se
reproducen `0x009CCE` (rampa de paleta en `1068EA`, ningún byte de nodo) ni
las comprobaciones de cartucho antes de `0x004188`.

Prueba de familia `slug` (`make check`, `data/slug_fixture.json.gz` de
`camelfam` con `extract_family_fixture.py --ram ...`, la nueva opción que
registra las lecturas y escrituras de la RAM fuera de los nodos): 1304
actualizaciones en bucle cerrado (9414..10094: espera, subida en 9530,
paseo, salto, vulcan con 83 balas `0x01BE24` —su actualización
constructora—, bombas y efectos, 15 polvos, el jinete muerto en 10091 y la
vuelta a la espera), 178220 bytes de nodo y 636352 bytes de RAM fuera de los
nodos (ranuras, palabras de mando, bloque de armas, nodos de los jugadores)
iguales. El vuelo posterior de las balas (`0x01B882`, ya probado en
`hbullet`) se deja fuera: en 9620 una bala choca en el original con una
celda con la marca `0x10` que la rejilla del port no tiene en x≈2575 (dato
de colisión de esa zona, no de la Camel).

En el juego la Camel sigue sin crearse (parada explícita registrada): falta
el jugador montado del port (`0x0C3078..0x0C3260`: seguir la ranura, la
pose del jinete y las salidas) y sus gráficos en el paquete de escena.

### Paso 9: jugador montado y Camel Slug en el juego (`src/marco_mount.c`)

Traducción manual de `0x0C3078..0x0C3260` y del relevo de la ranura del
procesamiento común `0x0C3348..0x0C3376` sobre el registro del nodo del
jugador (`neo_world_t.player_node`, disposición original): acarreo de
`0x0AA458` → `+9A` bit 7 (recién puesto → `+00 = 0x0C3078`; borrado tras
estar puesto → `0x0C30F0`), `0x0C3078` (`+7A/+7B`, `+15` bit 0, `+50 = 0`,
`0x0C0F16`), `0x0C309C` (`0x0AA550`, `0x019424/0x018F24`, `0x0C30C0`),
`0x0C30F0` (`0x0AA5AC`; tipo 0 muerte con `+5A/+5F`, tipo 1 expulsión;
tipos 2–5 paradas explícitas `0x0C32D0`, `0x0C1F4A`, `0x0C315A`),
`0x0C3232/0x0C3250` (`0x0AA4C8`, `+84 |= 0x1E`, `0x0C0FEA`, el arco y la
palanca, `0x0C109C`). Prueba `tools/test_marco_mount.c` (`make
check-core`, `data/mount_fixture.json.gz`: `extract_family_fixture.py`
sobre el nodo `1014C4` de `camelfam` y `tankfam`, rutinas `c2692` y
`c3000..c3300`, con la RAM de ranura, registro, `10C300/10C301`, mando,
bloque de armas y `10000E`): **517 actualizaciones** (36 libres, 2
subidas, 472 montado, la muerte del jinete en 10091, la salida A+B en 11099
y 3 del salto de expulsión), 12470 bytes de nodo y 691 de ranura/registro
iguales. Lo que reescriben después las partes no traducidas de la misma
actualización (`0x0143EE` y `0x0427A` del llamador, la muerte `0x0C2956`,
`+84/+85` del procesamiento común) no se compara.

En el juego (`gameplay.c`): el registro se llena con el jugador del port
antes del mundo (`+3C/+40` proyectados y `+44/+48` los anteriores —la
prueba de segmento de `0x0C0556`—, `+15`, `+28..+38` en la raíz como en
las trazas, `+5E = 1`, `+5F` bit 7, `+93`); las palabras de mando
`10E9EC` salen de los botones con `0x0CA1B8` (A disparo, B salto, C bomba,
A+B juntos bit 7). La Camel se crea en su registro del guion
(`neo_world_spawn_stop` solo para el SV-001). El jugador 1 corre **después**
de los actores (en las trazas `1014C4` va tras la Camel): `0x0C309C`, el
relevo y las salidas; montado está oculto (lo dibuja el jinete gráfico
`0x034F1A`), sin caja de daño (`+50 = 0`) y sin controles propios. Muerto
o entrando suelta la ranura (`0x0C2F6C`, `0x0C149E`). Desviaciones
declaradas: el barrido `0x0143EE` de `0x0AA5AC` es el desplazamiento recto
por el asiento, el de `0x0C109C` es el paso aéreo del jugador del port, y
el programa de la expulsión `0x0BF618` es la pose de salto del port. Solo
en el cartucho de referencia: `10F000` bit 7 (indicador de depuración del
original, `0x03519E`: el vehículo destruido no mata al jinete).

Núcleo: la cota vertical de `0x0138A0` (`0x013906..0x013966`: `10C01E`,
`10C022`, temblor `10E9CC`; abajo el original compara antes de acotar)
traducida; `10C00C` vale 0 en todos los volcados de Misión 1 (la prueba de
familia lo ponía a FF y ahora usa 0: `slug` sigue bit a bit). Fin de la
torreta `0x03B86E` y su pieza `0x03B936/0x03B97C` (programa `0x23D46C`,
explosión `0x01C706`) traducidos de la ROM (ninguna captura los tiene).

Gráficos: `tools/export_rest_sprites.py` añade los programas de la Camel,
el jinete, la torreta, la pieza, las balas y el polvo (paletas `0x3FF`,
`0x376`, `0x3D6`, `0x3AD`, `0x884`, `0x170`; `0x377`, jinete del P2, sin
captura): 2768 descriptores (413 nuevos). Validación contra VRAM de
`camelfam` (`docs/slug-sprites-validation.json`, sin tocar el informe
general): 24 volcados, 21206 píxeles iguales, 695 distintos (los volcados
de la subida 9531, de la muerte 10092 y de balas cuyo volcado está lejos de
su actualización).

### Paso 10: SV-001 `0x092A32` (`src/neo_tank.c`, `src/neo_tread.c`)

Traducción manual de `0x092810..0x095800` (tanque, cañón, obús, destello,
entrada del jinete, polvo) y de su cuerpo de orugas `0x014AE8..0x0153EE`
(`neo_tread.c`); la torreta vulcan `0x03B4E0/0x03B57C` (`+72 = 0`, ancla
`tanque+86`) y las balas `0x01B6A0` son las de la Camel (`neo_slug.c`,
`neo_bullet.c`), ahora con la chispa `0x03BA76/0x03BA9A` (antes parada
explícita). El mundo ya no para la aparición `0x092A32`
(`neo_world_spawn_stop` no para nada) y el jugador montado es el mismo de
la Camel (`marco_mount.c`, tipo 1: salida A+B o de un salto expulsa al
jugador); la HUD ya mostraba CANNON, los obuses (`+1F` de la ranura) y la
salud del vehículo (`neo_hud.c`, tipo 1).

**Estados.** Constructor `092A32` (paletas `0x3D3/0x39D/0x203`, suelo
`0146E8`, orugas `014AE8` en `+8C`, salud `0x30`, tipo 4, 10 obuses,
torreta), espera `092AD8/092B0E` (marcador `0353A4`, recorte `092A1A`,
`0AA610` con el rectángulo `092A2A`), subida `092C18..092C76` (hijo de
entrada `0954DC/095580`: programas `0954A4/0954B4/0954C4` por el `+93` del
jugador y el `+6E` del jinete, paleta `0954D4`, desplazamiento `095460[+74]`;
al terminar pone `tanque+60 = FF`), reposo `093278/09329E` (sonido `1092`),
parado `092C9A/092CC2`, marcha `092DDA` (entradas `092D24..092DA2` que caen
en `092DDA` en la misma actualización; polvo `0956CE/095768` por `+95`),
frenada `092F94`, disparo `093054/09307C` (`+79`), agachado
`093138/09315E/093204/09321A` (bombas `0929E6` → `0C0808` con el
desplazamiento `+70[2..3]`), salto `09333A/093362` (sonido `1090`), aire
`09339E/0933C2` (`0153D2` con `092A10[+75]`, gravedad, palanca, C), aterrizaje
`09347A/0934A2` (sonido `1091`), salida de un salto `0937C6/0937E4`,
salida A+B `093A08/093A46` (sonido `10E9`, lista de paletas `093882`) y
voladura `093AFA` (sonido `1034`, fila `0939F8`, `0AA7FC` tipo 0, destello
de pantalla `0AE1DC/0AE1F0` por `009BB0`, cuatro piezas `0AD9B8/0AD88A`
—programa `0AD8CE` del bloque fijo; su retrollamada `0AD96C` crea la
siguiente mientras `+4E`— y estallido `093BC2/093BF2`, programa `22CF2C`).
Cola común `093692`: enfriamiento `+8A`, `+5F` bit 6, lista de paletas
`+90` (`009D1E`: la ranura de paleta `+14` toma el recurso de la lista;
en el port, `a->palette[+14 − 1]`), parpadeo `+7E`, `0AA760`, `0AAA88`,
daño `0AA9E4`, fila de empuje `0939E8`, ancla `0939BA`. Cañón `092934`:
`0AA956`, destello `0952F8/09533C/095448` (zoom `290F3A[+74]` por la
retrollamada `290F4C`), obús `094EAA/094F1C/094FF8/09503C` (sonidos
`1084/10F2`; una pared, `+5F` bit 1, también lo hace estallar). Las
retrollamadas `09280E/092820` de los programas eligen el registro de pose
`+70` en tablas del banco `0x29xxxx` (`extract_soldier_programs.py`: el
destino es la palabra baja de la tabla tras la llamada).

**Orugas.** El nodo `014A4E` es un nodo de familia cuyos campos escribe el
tanque (`compile_family_fixture.py --shared 14a4e`). Cada paso (`014C0C`
aire con `0128E8` + `004ED2`, `014C2E` aire, `014C48` suelo con `004ED2`,
`014C6A` suelo) lee el modo `10C004` de la tabla `014522` por la palabra
de superficie de la sonda `013C1A` (`neo_kernel_probe`; sin superficie la
palabra es `0x4000` y se lee el código en `014522+0x4000`, blob
`0x018522`), barre el punto delantero y el trasero (`014B12`, dos barridos
absolutos de `013968`), mueve el ángulo con el muelle `014B90` y barre el
vehículo hasta el punto medio.

**Cuerpos sólidos `004ED2`.** Nuevo `ga_push_solids` (`neo_gactor.c`): la
tabla `106756` llega por `neo_vehicle_env_t.solid_node/solid_desc` (el
mundo la llena con sus `solids[]`); por cuerpo, la caja del descriptor
(espejo por `+15` bit 0 del dueño), los bits `+15` 3/4 de ambos, el empuje
de 1/16 y el barrido `013968` desde `+44/+48` (más el movimiento del padre)
con A0 = 0, de vuelta relativo a `+38` (`01AC4C`). `10C008 = FF` y
`10C00B = 0` en ese barrido (globales del barrido anterior en el
original): coinciden en los 16 empujes medidos. La Camel sigue con su
parada `0x004ED2` (su paso aéreo `0143EE` usa el resultado D7, sin
traducir).

**Capturas** (`tools/capture_weapons.lua`, plan `vehicles`,
`ASSIST_CAMERA=2000`; `MSX_WEAPONS_RIDE_JUMPS=1/2` añade saltos y
agachados sólo dentro del SV-001; `MSX_WEAPONS_DEBUG_CMDS` añade órdenes
del depurador, p. ej. trazas del núcleo):

| Captura | Variables | Contenido |
| --- | --- | --- |
| `tank3` | `LAST=11500 FAMILY 10330..11490 ROOTS=92a32,ae1dc,ad9b8,93bc2 FIRST_DUMP=11150` | subida 10376, marcha, 9 obuses, salida A+B 11099, voladura 11209 |
| `tank5` | igual con `RIDE_JUMPS=1`, `LAST=11250` | saltos, aire, aterrizajes, salida de un salto (937E4) |
| `tank6` | `RIDE_JUMPS=2`, `LAST=11130` | agachado 9315E/93204, 4 bombas `01BF10` |
| `tank7` | `RIDE=2400 LAST=12310 FAMILY 10330..12300` | el lanzacohetes `05ED84` registra su cuerpo `05EB7C`: 83 comprobaciones y 16 empujes 11994..12210 |
| `tank4` | `LAST=11145 FIRST_DUMP=11100 PERIOD=3` | solo volcados: paletas `0x396..0x39E` de la lista `093882` |

**Pruebas de familia** (`make check`, `extract_family_fixture.py --attacks
--ram 10c608-10c650 … 10000e-100010`, rangos `92810-95800`, `14a4e`,
`3b4e0-3bb00`, `3523c-352ff`, `1b6a0`, `ae1dc-ae220`, `ad884-ad9e6`;
`FAMILY_ARGS_tank… := --script-actor 101414 --script-routines 1b6a0
--node-fields 6e,8c --shared 14a4e --attacks` y los sellos de la tienda
`0B6566` de la familia `prisoner`):

| Familia | Nodos | Actualizaciones (fotogramas) | Bytes de nodo | RAM fuera de nodos |
| --- | --- | --- | --- | --- |
| `tank` | 235 | 2603 (10337..11358) | 350560 | 1228616 |
| `tankjump` | 70 | 1015 (10337..10839) | 138740 | 479080 |
| `tankcrouch` (+ `--script-routines 1bf10`) | 213 | 1855 (10337..11117) | 253120 | 875560 |
| `tankpush` (+ `--ram 106756-1067d6` y campos del cuerpo `102C24`) | 501 | 4104 (10337..12299) | 554680 | 2467704 |

Todas iguales bit a bit. Hallazgos de la traza del núcleo (`013968` con
`bpset 129bc`): la bandera 8 de un punto de las orugas venía de la celda
`(2576, fila 11)` marcada `0x10` por el sello de la tienda `0B6566`, que
el port aplica pero la prueba no tenía (`--stamp`); también explica la
celda «`0x10` en x≈2575» de la prueba `slug`. La prueba de familia ya no
compara los enlaces `+04..+0B` de los nodos de jugador (los cambia el
pool al insertar o borrar un nodo vecino) ni la RAM registrada que cae
dentro de un nodo de la familia vivo.

**Sprites.** `export_rest_sprites.py` con las raíces del tanque, `0AD8CE` y
`22CF2C`, y las capturas `tank3`, `tank1` (paletas) y `tank4`
(`--palette-capture`), con `--keep`: 4603 descriptores (1443 nuevos, los
anteriores idénticos), 98 paletas (12 nuevas: `0x203`, `0x204`, `0x258`,
`0x396..0x39A`, `0x39C..0x39E`, `0x3D3`; `0x143` se deja con el valor de
la exportación anterior, la de más volcados). Faltan `0x4/0x5` (entrada de
los personajes 2/3, `0954D4`). Validación contra VRAM: 49 volcados, 198118
píxeles iguales, 7803 distintos (solapes y parpadeo `+7E`).

**Paradas explícitas** (sin captura): reacción al daño `09351A` (y su
caída `0935A8..093684`), destrucción con jinete `0938DA` (hijo `03533A` de
`0353C2`, no traducido), obús de tipo 5 `0950DC`, `0933D6` sin nodo de
orugas. No se reproducen las comprobaciones de cartucho antes de
`004188`.

### Paso 11: arma C de Marco (`src/neo_chaser.c`)

El arma C (id `0C`, 40 obuses, tope 3 en vuelo) es nativa. Medido y
traducido de `hudc`/`cfam` (`--range 63600-64300 --range c5800-c5a00`: 6584
actualizaciones):

- **Jugador** (`gameplay.c`): el estado general `0x0C3CD0` con los programas
  de las tablas `0C3C10/0C3C50/0C3C90` para el id `0C` (forma 1): nivel
  `253A56` (`CALL 0C4372` en 0, `+76` en 5: otra pulsación dispara 6
  actualizaciones después), arriba `253AC6` y abajo `255FDE` (`+76` en 2).
  `0C4372` → `0C0D64`; tras la cabeza de la lista (`00411C`) el destello
  `0C58D2` (tabla `0C432A`) en el punto de `0C4244` menos `+72/+74`
  (`0C43CC`: mirando a la izquierda queda −(t+x72)−x72) con el movimiento
  del jugador (`01ACF2`), y el obús `063F32` (tabla `0C42AA`) en el punto de
  `0C4244` con `+95 + 1`, `+6E` = 0/40 reflejado, `+70`, `+5E` del jugador.
  `+93 = 2` (`063F10`, tabla `0C42EA`) sigue siendo parada explícita.
  **Aproximación declarada**: la colocación `0C482E` (`014A02`) es la
  colocación directa de las balas H; el agachado usa el `+72/+74` medido de
  la H (forma 1).
- **Obús** `063F32`/`063FDC`, **explosión** `064128`/`06414C` (programa
  `268622`, `CALL 0636B0` → onda `06371E/063740/063756`, fila `063690`,
  una sola consulta), **estela** `063D9E`/`063E48` (atraída por el nodo
  anterior de la lista, `+04`: la bocanada más nueva o el obús, también
  como marcador `003F1E`) y **destello** `0C58D2`/`0C5930`; detalles en la
  cabecera de `neo_chaser.c`. El obús devuelve su proyectil (`0C0DFC`) al
  explotar o salir de `063814`.
- **Consulta de objetivos** (`0049E8`, `004928..004952`): `neo_genv_t` lleva
  ahora `target[0]/target[1]` (A0: mayor `+4E` con `>=`; A1: menor con
  `<=`; gana el último visitado en un empate) con lo que `01B162` lee
  (`+3C/+40` y el primer rectángulo de `+50`), pedidos con
  `ga_attack_targets`. Solo cuentan los cuerpos que pasan la fase estrecha,
  la matriz `004582`, el multiplicador `+54` y `+5F` bits 7/6 a 0. Comprobado
  en la traza: la regla reproduce los 454 A0/A1 registrados (435 con varios
  candidatos, 217 con empates). En el mundo (`neo_world.c`) el registro
  conserva el orden de las ranuras de `004C24` (soldados, actores genéricos y
  nodos del jefe según se registran, máximo 0x40) y `004B06` las visita en
  el orden 16..31, 0..15, 48..63, 32..47. La fila de búsqueda (clase 1, tipo
  0, daño 0) no tiene efectos en el original y no se envía al jefe; la de
  contacto y la onda sí (`neo_chaser_attacker`). `neo_genv_t.previous` da
  el nodo anterior de la lista (`+04`).
- Capacidad del mundo: 64 actores genéricos, 112 nodos de orden y 128
  dibujos (el original tiene 0x70 nodos; la estela pide `0040BC` con
  prioridad 0x2A, que el port no modela).

Pruebas: familia `chaser` (`make check`, `data/chaser_fixture.json.gz`,
`extract_family_fixture.py ... --ram 10c718-10c72c --attacks`, nuevas
opciones `--attacks`/`--previous` de `compile_family_fixture.py`): **398
nodos, 6584 actualizaciones en bucle cerrado, 868560 bytes** iguales, las
respuestas de `0049E8` en el orden de las llamadas (D0 y los A0/A1 que
`01B162` leyó; un objetivo que el original no leyó vale 7FFF,7FFF: elegir el
otro falla), el bloque de armas (131680 bytes de RAM: `0C0DFC`). Mutaciones
comprobadas: elegir A0 en vez de A1, olvidar `+04`, la deriva de la estela
o la devolución del proyectil fallan. `tools/test_gameplay.c` (carrera C):
la C concedida y recargada (`0C0928`) con la política de las cúpulas:
765 obuses nativos, 445 ondas, 61 muertes, sin paradas ni dibujos ausentes,
hasta la cámara 3520.

Gráficos: `tools/export_rest_sprites.py` añade los programas `267F92`,
`268622`, `22CE6C`, `26863A`, `0C586C`, `0C5806` y la paleta `0x363`, con
`$W/cfam@63600-64300 $W/cfam@c5800-c5a00` en la exportación completa:
2870 descriptores (57 nuevos), 78 paletas, 617 volcados validados (2936091
píxeles iguales, 150936 distintos).

### Puntería hacia abajo en el aire (pistola, H y C)

Captura `air` (`tools/capture_weapons.lua`, plan `air`, MAME 0.264,
`MSX_WEAPONS_LAST=5080`): desde el inicio de la misión, saltos quieto y con
carrera manteniendo abajo, paseo con arriba alterno (barridos) y saltos con
arriba y luego abajo, A pulsado cada 8 fotogramas, con la pistola, la H
(dada en 3150) y la C (en 4110). En el aire, abajo es `0C0FEA` bit 1: el
estado `0C3CD0` pone `+92 = FF` y toma las tablas `0C3C90` (pistola
`24EABC`, C `255FDE`: `+76` en 2) y la H barre adelante→abajo
(`C8/D8/E8/F8`, reflejados `B8..88`) y dispara su abanico `BE/C0/C2`; en
tierra abajo es agacharse. Todos los nacimientos: ángulo `C0` (las dos
orientaciones) y la boca de la fila abajo de `0C4204` (pistola (0,−16), forma 1
(−4,−20)) más el `+72/+74` de los programas de salto (sobre todo `24E858`
(−4,12)).

Port (`gameplay.c`): `aim_down` = abajo en el aire (arriba gana, como
`0C3D30`); la pistola dispara entonces la bala nativa `01B782` a `C0` (las
demás siguen con la bala del port), la H recibe `down` en
`marco_hgun_input_t` (sus programas hacia abajo ya estaban traducidos) y la
C usa `C0` con la espera de 3 actualizaciones. El cuerpo dibuja `24EABC`
para la pistola (`marco_visual.c`); los de la forma 1 (H `2560C0/2561DC`,
C `255FDE`, y los demás disparos de la C) están ahora en el catálogo (ver
MARCO_CAMPAIGN.md, «Granadas, arma C y la H hacia abajo»); `25624E` sigue
sin captura. Sonido: `game_audio.c` ya no pide
`0x10F7` por los disparos nativos (H, C, pistola abajo), cuyos constructores
piden el suyo.

Prueba `tools/test_marco_muzzle.c` (`data/hmuzzle_fixture.json`, ahora con
`air` y con los nodos reutilizados, no solo los nacidos): **1012
nacimientos** de H, pistola y C (839 de puntería, 173 de barrido; 213 hacia
abajo en el aire) iguales píxel a píxel. Se excluyen los nacimientos con el
jugador contra una pared (`+15` bit 7): el barrido de colocación `014A02`
los detiene en la pared (`hfam` 11118..11152), la aproximación declarada.
`tools/test_gameplay.c` (carrera `airdown`): pistola, H y C disparan hacia
abajo saltando, sin paradas ni dibujos ausentes.

### H en diagonal: repetición de la captura `air` y la palanca

Informe de MiSTer: «el arma H sigue teniendo mal el disparo en diagonal».
Comprobación de extremo a extremo contra la captura `air` (paseo con arriba
alterno, saltos con arriba y luego abajo, A cada 8 fotogramas):

- **Bocas**: los 1012 nacimientos de `test_marco_muzzle.c` (paso anterior),
  173 de ellos de barrido, iguales píxel a píxel (ángulo y origen).
- **Estados** (`tools/test_marco_hgun.c`, parte 3,
  `data/hgun_replay_fixture.json` de `tools/extract_hgun_replay.py air 3150
  4100`): las palabras de mando `10E9EC` de cada actualización por
  `marco_hgun.c` dan cada bala H del original en la misma actualización y
  con el mismo ángulo (144 actualizaciones, 71 balas, 32 pasos de barrido:
  adelante↔arriba andando, abajo en el aire, abajo→adelante al aterrizar).
  Quedan fuera, y se resincroniza tras 8 actualizaciones sin balas, las
  ventanas en que el original usa estados que el jugador del port no tiene:
  **el cuchillo** `0x0C3B46` (`+9A` bit 0, puesto por `0x0C3744` con la fila
  `2D0AE4` de `0049E8`: con un enemigo al alcance una pulsación acuchilla en
  vez de disparar; en `air` 3706..3736 tres pulsaciones no dan balas) y **el
  aterrizaje agachado** con abajo (`0x0C1EBE` → `0x0C2236`: `+92 = 0` sin
  barrido). Ambos siguen sin traducir (el port dispara en esos casos).
- **Causa en el hardware**: la palanca. `actor.c` leía el stick analógico
  como una puerta cuadrada (cada eje pasado 25 por su cuenta): empujar a la
  derecha con 17° de holgura ya era arriba-derecha, y la H barría
  adelante→arriba y volvía mientras se andaba. Ahora `src/msx_stick.h` lo
  lee como la palanca de 8 direcciones del arcade (zona muerta de radio 25,
  sectores de 45°: un eje cuenta si el stick está a menos de 67,5° de él);
  comprobado en `tools/test_gameplay.c`. La cruceta digital no cambia.

### Camel Slug: Marco doble y atascado (informe de MiSTer)

Reproducido en el anfitrión (`tools/test_gameplay.c`, carreras `camel`:
montar entre las cúpulas y 2336 y cabalgar al azar, saltando abajo+B o
expulsando con A+B): al llegar a la cámara 2688 el lanzacohetes `05ED84`
registra su cuerpo sólido (`5EB7C`) y la Camel se para en `0x004ED2` (el
empuje de los cuerpos sólidos, **parada explícita**: ninguna traza de la
Camel lo ejerce). El mundo quitaba la Camel pero su hijo, el jinete gráfico
`034F1A/034F96` (que dibuja a Marco), seguía vivo en el sitio de la parada
y el Marco del port, expulsado, se dibujaba a la vez: dos Marcos, uno
quieto. Ahora una parada explícita de un actor genérico borra también sus
descendientes (`0041AC`, como al borrar un nodo con hijos): en las tres
carreras ningún jinete gráfico sobrevive a su Camel (antes, 5421
actualizaciones) ni se dibuja a Marco a pie con la ranura atada.
`0x004ED2` ya es nativo (`ga_push_solids`, medido con el SV-001 contra el
mismo cuerpo, paso 10): la Camel pasa el lanzacohetes (sección «deja de
disparar», abajo); falta una captura de la Camel ejerciéndolo.

### Paso 12: arma R (`0x06381C`, rama `work-marco12`)

Informe de MiSTer: con la R no se veían el disparo ni los cohetes (el port
caía en la bala de pistola con parada explícita). La R (id 3) usa las mismas
tablas de programas de disparo que la C (`0x0C3C10/0C3C50/0C3C90` → `0C02D8`,
`0C0318`, `0C0358` para ambas) y el mismo destello `0x0C58D2`: `gameplay.c`
la dispara por la ruta de la C con el cohete. `neo_chaser.c` traduce el
cohete: constructor `06381C` (sonido `10F5`, paleta `37C`, programa
`2683F2` horizontal / `2683B6` vertical), vuelo `0638B4` (búsqueda `+76` =
`063660`/`063670`; horizontal: `+34` hacia el centro del objetivo (`01B162`
y su `+42`) con el empuje `+7E/+82`; vertical: `+30` hacia el `+3C` largo
del objetivo; empuje `0x6000` en el eje; pared → explosión en la misma
actualización; contacto `063620/063630`; fuera de `063804` devuelve el
proyectil `0C0DFC` id 3; estela `063D9E` cada préstamo de `+74`) y
explosión `063A92/063AD6` (`26858A`/`2685CA`, tres llamadas `0636B0` en
actualizaciones distintas con las cargas `±0x10/0x20/0x30`, programas
`22CDD4/22CE6C/22CF2C`, filas `063680/063690/0636A0`).

Captura `r1` (`PLAN=pickups ASSIST_CAMERA=600` da la R hasta la cámara 600,
`FAMILY 2150..2690 ROOTS=6381c,c58d2,63d9e`): familia `rlaunch` (`--attacks
--previous --spawn-window 2`): 498 nodos, 8516 actualizaciones, 1129100
bytes iguales (41 cohetes, 5 explosiones, estelas y destellos). Nuevo: las
respuestas de `0049E8` registran también `+42`/`+3E` del objetivo (los lee
el cohete); `--spawn-window` (por defecto 8): con el pool lleno el original
no crea el hijo, y un nodo registrado más tarde es de otra actualización.
`tools/test_gameplay.c`: carrera R hasta la cámara 3520 (564 ondas, 56
muertes, sin paradas ni dibujos ausentes).

### La R «sigue sin verse» (informe de MiSTer, ROM 13)

Qué parte faltaba, por pruebas:

- **Objeto R.** Es el tipo `02` (registro `0E584A`, programa `28632A`,
  recogida `08B57A` → id 3), y su programa está en las raíces exportadas.
  La Misión 1 no lo suelta: en las semillas 1 y 2 de `release_run`
  aparecen los tipos `00` (id 5), `4B` (C), `4F` (H) y `51` (id 8),
  y nunca el `02`. Por eso hace falta un gancho para darla.
- **Cohetes.** Con el cartucho de captura (`MSX_DEBUG_GIVE_R=1` en
  `gameplay.c`, solo para capturas y nunca en un *release*: da la R como
  un objeto tras el aterrizaje), ares dibuja el cohete `06381C`, su estela,
  el destello y la explosión de pie, apuntando arriba y corriendo. Lo mismo
  se ve en MAME (`run_marco_catalog.py --give-rgun`, nuevo, como
  `--give-cgun`: id 03, 30 balas, tope 2).
- **Marco disparando la R.** Este era el fallo. En MAME, con `kind=03`, cada
  disparo entra en el estado de fuego de la C `0C3D9A` y en el programa de
  arma `253A56` (cursores `253A70..253ABA`, `--upper-trace`). Las tablas
  `0C3C10/0C3C50/0C3C90` dan `0C02D8/0C0318/0C0358` a los ids 3 y 0C. El
  port solo pasaba `cgun` a `marco_visual` con el id 0C. Con la R, el cuerpo
  forma 1 no arrancaba ningún programa de disparo: Marco quedaba en reposo
  mientras salían los cohetes.

Corrección: `msx_game_c_programs()` (ids 0C y 3) decide `cgun` en
`actor.c`, y la R usa los programas de disparo de la C
(`MARCO_CAMPAIGN_W1C_FIRE*`, `B1C_CROUCH_FIRE`). `chaser_run` lo comprueba.

### Camel Slug: «deja de disparar» (informe de MiSTer, ROM 10)

Reproducción en el anfitrión (paseos largos en la Camel con el disparo
pulsado, mantenido o al azar, saltos sin A+B, sin morir): la Camel
desaparecía en la cámara ≈2693, al llegar al lanzacohetes `0x05ED84`, que
registra su cuerpo sólido (`0x05EB7C`, tipo 1) en la tabla `106756`: el
paso de la Camel llamaba a `0x004ED2` y eso era una parada explícita, el
mundo borraba la Camel y su torreta y el jugador salía expulsado. Con el
empuje nativo (`ga_push_solids`, medido con el SV-001 en `tank7`) la Camel
pasa: tres semillas por modo llegan a la cámara 3520 disparando (2348–2991
balas, ninguna racha de más de 2 actualizaciones sin bala con el disparo
pulsado a golpes). En el aire, un cuerpo de tipo 1 bajo la Camel la
lanza hacia arriba (`0x014408..0x014446`: `+34 = 0x60000`, bits `+15` 4 y
3, `+30` contra el movimiento): traducido del código, sin captura que lo
recorra. `tools/test_gameplay.c` (`camel_run`) ya no admite la parada
`0x004ED2` y el modo 0 sigue montado hasta el final.

Mantener A pulsado dispara una sola ráfaga de 5 balas, igual que el
original: la repetición de `0x03B622` depende de `10F084+p` (`0x0CA614`),
una opción del menú `0x066742` (`0x0CA5DC/0x0CA5EE`) que vale 0 en todas
las capturas de MAME (`slug`, `tank`). Con el bote de nodos lleno (96
actores genéricos; en la arena del jefe, con fuego continuo, hasta 43
impactos `0x01C9E6` y 22 nodos `0x003F1E` a la espera de sus hijos) una
bala puede no crearse (`overflow`, 2 veces en 20000 actualizaciones de una
semilla); el original reparte 100 nodos entre todos sus actores.

### Paso 13: cuchillo `0x0C3B46` (rama `work-marco12`)

Informe de MiSTer: no se podía matar con el cuchillo. `gameplay.c` lo hace
nativo: cada actualización del jugador `0x0C3744` consulta la fila `2D0AE4`
(clase 1, sin daño) en su origen; un *hurtbox* de tipo 4 o 5 al alcance pone
`+9A` bit 0, y una pulsación de disparo (cualquier arma salvo `+93 = 3`,
`0x0C3CD0`) corta en vez de disparar: la primera retrollamada `0x0C419E`
del programa (una actualización después) golpea con la fila `2D0AF4` (tipo
2, daño 0x1E; `neo_world_player_attack`) y suena `10FB` (tipo 5) o `1069`
(tipo 4); las pulsaciones durante el corte esperan a su fin (`+76`).
Captura `knife1` (`run_marco_catalog.py --plan` con andar y disparar hacia
los primeros rebeldes): 10 cortes, todos de la tabla `0x0BF958` en reposo
(`0x24D486`), 21 fotogramas hasta la siguiente elección de disparo.
`tools/test_gameplay.c` (`knife_run`): cortes que golpean, sin paradas.
Sonidos `10FB`, `10F9`, `1069` renderizados y medidos contra todos
(`data/sound_pairs.json`, 2000 pares). Poses del corte: MARCO_CAMPAIGN.md, «Cuchillo».

### Paso 14: armas 0D, 02/07 y 04/09 (`src/neo_pweapon.c`, rama `work-m2mummy`)

La Misión 2 da las armas `06` (tipo `04`, sección 1), `07` (`50`), `09`
(`52`), `0D` (`4A`), `02` (`01`), `04` (`03`) y las conocidas H y R
(guion `data/mission2_script.json`: objetos `08C37C/08C168` y prisioneros
`099DF0/099E06`). Todas pasan por `0C4372` → tabla `0C42AA` como la C; las
tablas de programas de disparo `0C3C10/0C3C50/0C3C90` (+0x10, forma 1) y la
agachada `0C246A` (`C0098..`) dan tres grupos: `C02D8..` (ids 3, 8, C, D,
E: la C), `0BFFD8..` (02, 07, 0F) y `0C00D8..` (04, 09); `0C01D8..` es el
rayo (06, 0B).

Capturas MAME 0.264 (`/home/user/captures-weap2`, privadas):
`capture_weapons.lua` con la nueva `MSX_WEAPONS_ASSIST_ID=<id>` (la ayuda
escribe ese id; con ella, en el primer fotograma también el tope `0C08A8` y
`+94` de `0C0918`, como `0C0928`), plan `air`, `ASSIST_CAMERA=9999`,
`FAMILY 2150..3140`: `d1` (0D), `s2`/`s7` (02/07), `f4`/`f9` (04/09).

| Arma | Proyectil | Familia (`make check`) |
| --- | --- | --- |
| 0D | bola `08606A` (nivel/arriba/abajo, rueda `0861C6` con la pose de pendiente `08661C`, estalla `086216`), resplandor `08651C`, explosiones `086304` con las tres llamadas `0866A6` (filas `E4E54/64/74`) | `dball`: 222 nodos, 3952 actualizaciones, 522900 bytes |
| 02 / 07 | estallido `0644C4`/`0646B0` (fila `E1412..` una vez, pantalla blanca `009BB0` una actualización, devuelve el id de `E1442`) | `sgun2`/`sgun7`: 23 nodos, 437 actualizaciones cada una |
| 04 / 09 | llama `0A71F8`/`0A716E` (suelo y aire, 12 actualizaciones), primera llama `0A74E4` y bocanadas `0A751C` atraídas por el nodo anterior (`+04`), velocidad de programa `0A7756`; espera a sus hijos (`00408C`) | `flame4`/`flame9`: 258 nodos, 3711/3459 actualizaciones |

En el juego (`gameplay.c`, `fire_kind`): constructor, destello `0C58D2`
solo en el grupo de la C, y la espera hasta `+76` de cada programa (02/07:
7 actualizaciones de pie, arriba y agachado, 3 abajo; 04/09: 3).
`0C4526` (sin nodo) devuelve el proyectil del arma actual.
`tools/test_gameplay.c` (`chaser`, `pweapon`): 0D, 02, 07, 04 y 09
concedidas y disparadas hasta la cámara 3520 (0D: 1920), matan, sin dibujos
ausentes. Gráficos: `export_rest_sprites.py` (raíces y paletas `822`,
`828`, `204`, `39B`, `384`, `876`; valida ahora el giro vertical, bit 1
del atributo): `d1` 71222 píxeles iguales / 1044 distintos, `s2+s7` 86231 /
0, `f4+f9` 63967 / 6237 (sin explicar uno por uno). Al `filesystem/rest-native.json`
de la Misión 1 solo se añaden los descriptores de esas raíces.
Sonidos: `10F5`, `10F6`, `10F3`, `116A`, `1168`, `112F`, `112C`, `115A` en
`data/sound_map.json` (render aislado `OK`).

**Paradas explícitas que quedan** (las muertes de soldado por filas de tipo
7, `0A` y `0B` y el rayo `06/0B` son nativos desde el Paso 16): el agua bajo la bola
0D (`03C5CC` → salpicadura `03C962`); la misión 9 de `0644EE` (`012022`);
las variantes `+93 = 2` (tabla `0C42EA`: `0646C8/0646E0`, `0A7226/0A719C`,
`063F10`, `0C44C2/0C44E0`); las armas `01` (`0C5508`),
`0E` (`0DE4DA`) y `0F` (`0DEA20`) (la `08` es nativa desde el Paso 15): siguen disparando la
bala de pistola del port (`weapon_stops`).

**Rayo 06/0B: análisis (rama `work-m2gate`; traducido en el Paso 16).** Captura MAME 0.264 (privada,
`/home/user/captures-m2gate/w6`, borrada al acabar; repetible):

```sh
MSX_WEAPONS_PLAN=air MSX_WEAPONS_ASSIST_ID=6 MSX_WEAPONS_ASSIST_CAMERA=9999 MSX_WEAPONS_FAMILY_FROM=2150 \
  MSX_WEAPONS_FAMILY_TO=3140 MSX_WEAPONS_FAMILY_ROOTS=1e22c,c58d2 MSX_WEAPONS_LAST=3150 \
  MSX_WEAPONS_FIRST_DUMP=999999 MSX_WEAPONS_STATE_FROM=999999 sh tools/run_capture_weapons.sh OUT
```

En 2150..3140: 9 rayos `01E22C` (el primero en 2193), 445 actualizaciones
en `01E28E/01E2FC/01E398/01E41C`, 58 chispas `01E582/01E58A/01E596`
(1075 actualizaciones en `01E604`). Estructura leída de la ROM:
`0C46D6` (con el arma 06/0B: +95 + 1 impar, o acarreo de `0C0D64`)
llama a `01E19E` (D0 = jugador, D1 = 0, D2 = 4: bloque RAM 10C340/10C348
del jugador, +6 vivo, +7 sonido 10F8 si el otro jugador no suena, +5 vida
D2 & 7F, +4 dirección; crea `01E22C` con `0040BC` reserva 0xE, `01B220`,
+5E del jugador, +6E jugador, +70 dirección) y a `01E166` (D0/D1 = el
punto del arma de la tabla que deja `0C46D6` en A0, más +72/+74 si +90 ≠
FF; +4 = D3, +92 si +90 ≠ FF, y si cambia con +5 ≤ 0, +6 = 0). `01E22C`: capas E000, paletas 0x98/0x99/0x9A (`009F88` y
de nuevo según +88, `01F002`: arma 0B → bit 0, `0C05B8` = 2 → bit 1),
onda `03C526`; por actualización `01F002`, `01EC1E` (una vez por
fotograma de vídeo 10000E: lee el bloque, copia el giro del jugador, mide
con `014A02` desde el jugador con +2C/+40 + 1, `01AD6E`/`01AD52`/
`01ABC8`), `01ED32` (fin si +6 = 0 → `01E4D4`: sonido 10F8 parado por
`012192` si el otro jugador no lo usa, `004188`); horizontal
`01ED60`/vertical `01EDF6`: recorre el rayo en pasos de 16 px
(`014906`, `01AC38`, un `00427A` por paso, filas `1F096..1F116` de
`0049E8` según +7A y +88, hasta la pared o el borde 0x10..0x130 /
0x100..0x200), con programas `22EBB6/22EC92/22ED6E` (+70 = 0),
`22F016/22F0F2/22F1CE` (+70 ≠ 0) y los de la fase `01E398`
(`22EE6A/22EF3A/22F00A`, `22F2CA/22F39A/22F46A`), chispas `01EF68` cada 4
fotogramas de vídeo. Falta: traducirlo con la familia de `w6` (dibujos
múltiples por actualización, `014A02` sobre el nodo del jugador), los papeles
de las poses `marco-w6` en `compile_marco_campaign.py`, el sonido 10F8 en
`data/sound_map.json` y la llamada de `fire_kind`.

**Muertes de soldado por fila de tipo 7, 0A y 0B (análisis; traducidas en
el Paso 16).** Tablas de `0406B6`/`0407CA` (destino de cada
tipo 0..13; ≥ 14 va a `040DC0`/`040A28`): suelo `0406EA` = 40DC0 40C8E
40DC0 40B46 40B46 40BA8 40D66 **40A40** 40DC0 40C8E **40AF6 40A7C** 40DD8
40DD8 40DC0×6; aire `040750` = 40DC0 40C8E 40DC0 40B46 40B46 40DC0 40A10
**40A40** 40DC0 40DC0 **40AF6 40A7C** 40DC0×8; agachado `0407F4` = 40A28×3
40B46 40B46 40BE6 40A28 **40A40** 40A28 40A28 **40AF6 40A7C** 40A28×8.
`040A40` (tipo 7): +76 = (`0029C6` & FFFF) << 2, negado si +5B < 0; +34 =
0; programa `240E8E`; → `040DF0`. `040AF6` (0A): `040F04` (hijo `040F60`
a (8, 0) con +15 bit 0 invertido: programa `24039C`, paleta 0x37A/0x3BA
por `01B2BE`), programa `240D2E` o `240C6E` según +5B ^ giro, +34 = 2,0,
+15 bit 6 a 0, → `040DF0`. `040A7C` (0B): `040F04`, programa `240DC0` o
`240AE6` según el signo de `0029C6`, +30 = ±(`0029C6` & FFFF) << 2 por
+5B, +34 = 16,0, +15 bit 6 a 0; `040AE0`: +34 −= +34 >> 4 mientras sea ≥ 0
y `040DF0`. Falta la evidencia de MAME (un soldado muerto por la llama o el
estallido con volcados por fotograma, `extract_soldier_ticks.py`) y la
variante de `NEO_SOLDIER_EVENT_DEATH_CHILD` con el programa `24039C`.

### Paso 16: el rayo 06/0B y las muertes por llama y estallido (rama `work-laser`, B11 y B12)

**Rayo (`neo_pweapon.c`, `beam_tick`).** Traducción de `01E22C..01F094`
con el análisis de arriba: constructor `01E22C`, fase de disparo
`01E28E`/`01E2FC` (horizontal/vertical), fase de apagado `01E398`
(`01E3B2`/`01E41C`), fin `01E4D4` y chispas `01E582/01E58A/01E596` +
`01E604`. Cada actualización vuelve a arrancar sus programas (`004256`) y
dibuja varias veces (`00427A`): la boca, un segmento por cada paso de 16 px
del recorrido `01ED60`/`01EDF6` y el extremo cuando acaba en una pared
(`014906`, D6 bit 1) o en un blanco (`0049E8` ≠ 0). `01EC1E` lee el bloque
RAM `10C340`/`10C348` (`neo_genv_t.laser`, una vez por valor de `10000E`),
copia el giro del jugador y coloca el rayo con `014A02` desde el nodo del
jugador (`+2C`/`+40` + 1, solo para `+84`), `01AD6E` sobre su bloque `+28`,
`01AD52` a la raíz y `01ABC8` dos veces. Ayudas nuevas: `ga_sweep_probe`
(`014906`). Filas `1F096..1F156`, rectángulo `1F166` y tabla de chispas
`1F16E` en `data/region_rom.json`. El cambio de forma (`+88` bit 1:
`009F88` y paleta `875`) está traducido pero no aparece en las capturas.

**El jugador (`gameplay.c`).** `fire_kind` da a `06`/`0B` el grupo
`MSX_FIRE_L`: la pulsación arranca el programa del rayo (`laser_step`,
`laser_aim`) y cada una de las 8 actualizaciones siguientes hace la llamada
`0C46D6` (`laser_call`) con el punto de la tabla del programa (`253594`
nivel, `2536FC` arriba, `256132` abajo en el aire, `256DE8` agachado: los
`dx, dy` de cada `CALL`) más `+72/+74`; `+95` impar no gasta munición, par
`0C0D64`; `neo_world_laser` hace `01E19E` (sonido `10F8`, el rayo después
de la cabeza de la lista, `01B220` (0, 0) desde el registro del jugador) y
`01E166`. La espera de `+76`: 18 tics de nivel, 16 arriba, abajo y
agachado (`SET_BYTE +76` tras 8 / 7 esperas). El registro del jugador
tiene ahora `+6A` = FF (volcados de MAME). Visual: papeles `W1L_FIRE`
`253594`, `W1L_FIRE_UP` `2536FC`, `W1L_FIRE_DOWN` `256132` y
`B1L_CROUCH_FIRE` `256DE8` de `marco-w6` en `compile_marco_campaign.py`
(`--w6-capture`), `cgroup` 4 en `marco_visual.c`.

**Evidencia del rayo.** Capturas MAME 0.264 `w6` y `wb` (privadas,
`/home/user/cap-laser`, borradas tras extraer; repetibles):

```sh
MSX_WEAPONS_PLAN=air MSX_WEAPONS_ASSIST_ID=6 MSX_WEAPONS_ASSIST_CAMERA=9999 MSX_WEAPONS_FAMILY_FROM=2150 \
  MSX_WEAPONS_FAMILY_TO=3140 MSX_WEAPONS_FAMILY_ROOTS=1e22c,c58d2 MSX_WEAPONS_LAST=3150 \
  MSX_WEAPONS_FIRST_DUMP=2160 MSX_WEAPONS_STATE_FROM=999999 sh tools/run_capture_weapons.sh OUT   # y ASSIST_ID=B
python3 tools/extract_family_fixture.py OUT/boss-trace.txt --output data/laser6_fixture.json.gz --range 1e100-1f100 \
  --ram 10c340-10c350 --ram 1014c4-101574 --ram 10000e-100010 --ram 10c718-10c730 --attacks
python3 tools/export_rest_sprites.py ZIP OUT6@1e100-1f100 OUTB@1e100-1f100 --output SCRATCH/rest.json   # solo los descriptores del rayo
```

- Familias `laser6` y `laserb` (`make check`, terreno `10C72E`): **74
  nodos, 1669 actualizaciones en bucle cerrado (2193..3139), 223860 bytes
  iguales, 3488 dibujos**, 9 rayos, 65 chispas; las escrituras al bloque
  `10C340` y al nodo del jugador (350490 bytes de RAM) iguales a las del
  original. Sin cubrir en las capturas: `01E3B2` (apagado horizontal),
  `01E58A` y `+88` bit 1.
- Gráficos: 224 descriptores de los programas `22EBB6..22F476` y las
  paletas `98/99/9A` añadidos a `filesystem/rest-native.json`
  (`export_rest_sprites.py`, raíces y paletas nuevas). Validación contra la
  VRAM de `w6`/`wb`: **45230 píxeles iguales y 1996 distintos en 62
  volcados (50 volcados exactos)**; la paleta `875` (forma 2) no se capturó.
- Sonido `10F8`: render aislado `ACTIVE_AT_WINDOW_END` (sonido sostenido:
  ventana de 5,015 s, sin bucle exacto medible); `data/sound_map.json`
  `"stop": "012192"` y `pack_game_audio.py` lo empaqueta como su ventana
  (32 kHz: 30 % de la energía por encima de 11 kHz; no residente, desde
  `rom:/`); `game_audio.c` atiende
  la petición de parada `palabra | 0x8000` (`012192`). Pares contra las
  referencias y los primeros de cada canal nuevo: todos `unclear` u
  `overlap`: canal propio `ch_10f8` (`data/sound_policy.json`).
- `tools/test_gameplay.c pweapon`: `06` 4052 llamadas `0C46D6`, 8 rayos,
  8773 actualizaciones de rayo, 47 muertes; `0B` 3966, 9 rayos, 47 muertes;
  sin paradas ni dibujos ausentes (antes: `weapon_stops`).
- **B29 (MiSTer, ROM 22: al coger la L no se ve el arma).** Esos
  descriptores y paletas solo entraron en la exportación de la Misión 1. En
  la Misión 2 los actores dibujan con `romfs/mission2.pack`, hecho de
  `filesystem/mission2/rest-native.json`, y la copia de la ROM 22
  (`/home/user/ws-logo`) era anterior a `work-laser`: le faltaban **235
  descriptores** (los del rayo `22EBB6..22F476` y el cohete del arma 8
  `2684D2/2684AA`) y **7 paletas** (`98 99 9A` del rayo, `384 39B 822 828`
  de las demás armas). `actor.c` cuenta el dibujo que no encuentra y no
  pinta nada: en la Misión 2 el rayo de la L no se veía (la pistola del
  jugador y sus poses `W1L_*` sí, vienen del catálogo de Marco). `test_gameplay`
  prueba el láser contra la escena de la Misión 1 (`scene_meta.h`), así que no
  lo veía. Arreglo (`work-m2fix2`): `filesystem/mission2/rest-native.json`
  regenerado con todas las raíces y `--keep filesystem/rest-native.json`
  (las paletas de la Misión 1), `romfs/mission2.pack` reempaquetado y la
  comprobación `tools/check_mission_sprites.py` en `make check`: la
  exportación de la Misión 2 debe llevar todos los descriptores y paletas de
  la de la Misión 1 (con la de la ROM 22 falla con esos 235 + 7).

**Muertes de soldado por filas de tipo 7, 0A y 0B (`neo_soldier_ai.c`).**
`death_dispatch` traduce las entradas `040A40` (7: `+76` =
(`0029C6` & FFFF) << 2 con el signo de `+5B`, `+34` = 0, programa
`240E8E`), `040AF6` (`0A`: hijo `040F04`, programa `240C6E`/`240D2E` por
`+5B` ^ giro, `+34` = 0x20000, `+15` bit 6 a 0) y `040A7C` (`0B`: hijo,
programa `240AE6`/`240DC0` por el signo de `0029C6`, `+30`, `+34` =
0x100000, `+15` bit 6 a 0, y **en la misma actualización** `040AE0` y el
cuerpo `040DF0`; la rutina queda en `040AE0`: `+34` −= `+34` >> 4 y
`040DF0`). Las tres tablas (suelo, aire, agachado) usan las mismas
entradas. El hijo `040F60` (programa `24039C`, paleta `37A/3BA` por
`01B2BE`) es la salpicadura del mundo con su programa
(`NEO_SOLDIER_EVENT_DEATH_CHILD` con valor 1). `death_dispatch` devuelve
ahora borrado cuando ese cuerpo llega a `004188`.

Evidencia (MAME 0.264, `tools/mame/trace_soldier_campaign.lua` con las
opciones nuevas `MSX_ASSIST_WEAPON` (la ayuda de `capture_weapons.lua`),
`MSX_SOLDIER_ENTRY=1` (los 0xB0 bytes de cada soldado al empezar su
actualización: el golpe de la llama o del estallido, que corren antes en la
lista, ya está en ellos) y `MSX_FORCE_DEATH` (escritura de referencia: cada
37 fotogramas el primer soldado en pantalla recibe `+5A` = tipo y `+5F` bit
7, como lo dejaría `0049E8`; el estallido hacia arriba no alcanza a ningún
soldado con las entradas porque de cerca el jugador acuchilla)):

```sh
D=OUT; MSX_CAPTURE_DIR=$D MSX_RAM_FIRST=2100 MSX_CAPTURE_END_FRAME=3400 MSX_FIRE_FRAME=2190 MSX_RIGHT_FRAME=2280 \
  MSX_UP_FRAME=2400 MSX_ASSIST_WEAPON=4 MSX_SOLDIER_ENTRY=1 mame mslugx -rompath ROMS -bios euro -video none -sound none \
  -skip_gameinfo -nothrottle -autoboot_delay 0 -autoboot_script tools/mame/trace_soldier_campaign.lua \
  -cfg_directory $D/cfg -nvram_directory $D/nvram -debug -debugger none -debuglog -debugscript tools/mame/start.cmd
# b12w4 (ASSIST_WEAPON=4), b12w2 (=2), b12f (=2, END 3250, MSX_FORCE_DEATH=b MSX_FORCE_PERIOD=37)
python3 tools/extract_soldier_ticks.py b12w4:3270 b12w2:3270 b12f:3250 --entry --only-deaths \
  --output data/soldier_death_tick_fixture.json.gz
```

`extract_soldier_ticks.py --entry` parte de los bytes de entrada y deja
fuera las actualizaciones con el jugador 1 muriendo (`10C33C` = 0: el
arnés no modela sus entradas); el corte en 3270 deja fuera la capa
`10D7B6` (más allá de la cámara 1023, tampoco modelada por el arnés).
`make check` (`soldier_death`): **1468 actualizaciones de soldados
moribundos iguales (estado, cursor de animación y RNG)**: tipo 7 en el
suelo 5 y en el aire 1, `0A` en el suelo 5, `0B` en el suelo 7 y en el
aire 1 (forzadas) con 813 actualizaciones de `040AE0`, y 628 de `040DF0`.
`tools/test_gameplay.c pweapon`: 02, 07, 04 y 09 con **0 paradas de
muerte** (antes 13/40/68/91) y sin dibujos ausentes.

Paradas que quedan: el cuerpo en llamas `040920` (`+74`: un soldado
muerto en el suelo con `+15` bit 5 y una fila de tipo 7 o 9 en `+6C`; la
raíz `01AC4C` y `01AC38`) no aparece en ninguna captura: sigue como parada
explícita (`UNSUPPORTED_AT`). El hijo `040F60` no se compara con MAME
(solo sus descriptores, sin dibujos ausentes). Las entradas `040B46`,
`040BA8` y `040BE6` (tipos 3, 4, 5) siguen sin traducir.

### Paso 15: arma 08, el lanzacohetes del final de la Misión 1 (`0x0641EC`, rama `work-m1m2flow`)

Informe del usuario (hardware real, ROM 20): al llegar al jefe final no hay
lanzacohetes. El guion de la Misión 1 (`data/mission1_script.json`) crea en
la cámara 3224, justo antes del jefe `0D5976` (cámara 3308), un prisionero
`099DCC` (registro `2D70C4`, palabras `0 0 0 81 180`): `+74` = `0x51`. Su
saludo (`09ACD6`) suelta el objeto de tipo `51`, cuya recogida `08B5B0` da
el arma `0x08` (`0C0928`: 30 cohetes, tope 2, voz `112E` «Rocket
launcher»). La R del port era la `3` (`06381C`); la `8` caía en la parada
explícita `weapon_stops` y disparaba la bala de pistola: en la recreativa
hay lanzacohetes, en el port no.

`0641EC..0644C2` es `06381C..063AEC` con cuatro cambios (desensamblado
comparado instrucción a instrucción): programas `2684D2` (horizontal) /
`2684AA` (vertical) con cohetes más grandes (descriptores `142910..14299E`,
`145066`), filas de contacto `063640/063650` (`0x30×0x10` en vez de
`0x18×0x08`), rectángulo de recorte `06380C` (sus ocho primeros bytes son
los de `063804`) y el id `8` devuelto a `0C0DFC`; la búsqueda (`063660/
063670`), el vuelo, la estela `063D9E` y la explosión (`26858A/2685CA`) son
los de la R, y la comprobación de cartucho tras `0C0DFC` desaparece
(`004188` directo). Las tablas `0C3C10/0C3C50/0C3C90` dan a la `8` los
programas de disparo de la `3` (`0C02D8/0C0318/0C0358`) y `0C432A` el mismo
destello `0C58D2`.

- `neo_chaser.c`: `06381C`/`0641EC`, `0638B4`/`064284`, `063A92`/`0643DA`,
  `063AD6`/`06441E` con esos parámetros; `gameplay.c` (`fire_kind`): id 8
  por el grupo de la C.
- Programas `2684D2/2684AA` en `extract_soldier_programs.py` y
  `export_rest_sprites.py`; `data/region_rom.json`: `063804` ampliado a 16
  bytes (`06380C`). `extract_family_fixture.py` registra también las
  lecturas del objetivo en `0642AE/0642EC` (como `0638DE/06391C`).
- Captura MAME 0.264 `r8` (privada, `/home/user/cap-m1m2/r8`, repetible):

```sh
MSX_WEAPONS_PLAN=air MSX_WEAPONS_ASSIST_ID=8 MSX_WEAPONS_ASSIST_CAMERA=9999 MSX_WEAPONS_FAMILY_FROM=2150 \
  MSX_WEAPONS_FAMILY_TO=3140 MSX_WEAPONS_FAMILY_ROOTS=641ec,c58d2,63d9e MSX_WEAPONS_LAST=3150 \
  MSX_WEAPONS_FIRST_DUMP=2160 MSX_WEAPONS_STATE_FROM=999999 sh tools/run_capture_weapons.sh OUT
python3 tools/extract_family_fixture.py OUT/boss-trace.txt --output data/rlaunch8_fixture.json.gz \
  --range 63600-64700 --range c5800-c5a00 --ram 10c718-10c72c --attacks
```

  Familia `rlaunch8` (`make check`, terreno `10C72E`: la captura está en el
  pueblo): **505 nodos, 8747 actualizaciones en bucle cerrado (2195..3139),
  1156120 bytes iguales**, 42 cohetes, 489 borrados.
- Gráficos: los 11 descriptores nuevos se añaden a la exportación privada
  `filesystem/rest-native.json` (`export_rest_sprites.py ZIP OUT@641ec-6441e
  --keep <anterior>`, de la salida solo los descriptores de `2684D2/
  2684AA`; paleta `0x37C` ya presente). Validación contra la VRAM de `r8`:
  9655 píxeles iguales y 2742 distintos en 31 volcados; la misma medida con
  la R ya validada (`r3`, misma captura con `ASSIST_ID=3`) da 5466/1518: la
  diferencia es el desfase de una actualización de los cohetes rápidos en
  el validador, no los descriptores.
- `tools/test_gameplay.c` (`chaser`): carrera con la `8` hasta la cámara
  3520: 1134 cohetes, 711 explosiones, 58 muertes, sin paradas ni dibujos
  ausentes.
