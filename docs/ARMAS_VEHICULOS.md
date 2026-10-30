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
| `51` | `0x08B5B0` | arma `0x08` | no tomado en estas capturas |

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
| 8 | 30 | 20 | 2 | 112E | `0x0641EC` + `0x0C58D2` | variante de 3 (tipo `51`, no tomada) |
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
recibió daño: **daño y destrucción del SV-001 no observados**.

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
**Aproximación declarada**: la boca (`0x0C4244`, `0x014A02`) usa la boca de
pistola del port, porque el modelo de jugador del port no tiene `+72/+74/+92`;
la cadencia es la del port, no la de las retrollamadas de los programas de
Marco (`0x0C4372`), que el port no ejecuta como disparo. La pistola sigue
con la bala del port (la del jefe y el blindado de referencia pasan por
`neo_world_shot`). **Paradas explícitas**: H con `+93 = 2` (`0x01B668/0x01BDD2`),
cohete `0x06381C`, C `0x063F32` (y su destello `0x0C58D2`) y las demás armas
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

### Paso 5: HUD — pendiente

El port no tiene ruta de HUD de capa fija (`actor.c` solo dibuja el texto de
diagnóstico). Los contadores ARMS/BOMB (`0x0C071C`, `0x0C0764`) están
correctos en el estado (`marco_weapons_rounds`, `marco_weapons_bomb_count`) y
`actor.c` registra sus cambios. El panel `0x0C9650..0x0C9844` es el paso
siguiente.
