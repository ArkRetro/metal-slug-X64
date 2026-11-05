# Jefe de la Misión 3 (`04A2C2`) y jefe intermedio (`0A4A94`): investigación

Encargo (rama `work-m3boss`, solo investigación y documentación, sin código de
juego): el jefe final de la Misión 3 (el robot andante de la pantalla
`game146490`; registro de guion `2DB5E4`, constructor `04A2C2`) y, más corto,
la aeronave «96» (`0A4A94`). Modelos: [JEFE_MISION2.md](JEFE_MISION2.md) y
[JEFE_MISION1.md](JEFE_MISION1.md); inventario de la misión en
[MISION3_ACTORES.md](MISION3_ACTORES.md). Regla de siempre: solo lo que dicen
MAME 0.264 y la ROM del usuario (`mslugx.zip`); lo no comprobado se marca
**(sin comprobar)**. Los nombres («brazo lanzallamas», «mortero», «trampilla»)
describen lo que se ve o lo que hace el código, no son nombres del juego.

## Captura

`tools/capture_m3_boss.sh` (MAME 0.264, BIOS `euro`, `capture_fullgame.lua`):

```sh
# 1) estados pequeños (≈ 35 KB): at_132300 (antes de la aeronave) y at_144400 (antes del jefe)
sh tools/capture_m3_boss.sh states /home/user/captures-m3boss/states-run      # ≈ 2 min
# 2) traza de la familia + depurador (ALLOC/FREE/ANIM/SND/END), un tramo
sh tools/capture_m3_boss.sh boss OUT [FROM LAST]   # carga at_144400; por defecto 144410..149300
sh tools/capture_m3_boss.sh mid  OUT [FROM LAST]   # repite run4 desde sec04_117966; 132310..141100
# investigación: MSX_M3BOSS_FILTER=1 filtra boss-trace.txt (FIFO) a las líneas U/V/B/X/A/F
```

- `states` reproduce `run4` fotograma a fotograma desde `states4/sec04_117966`
  (comprobado: `trace.csv` igual en 132300, 140000 y 144400). Los estados van a
  `$MSX_M3BOSS_STATES` (por defecto `/home/user/captures-m3boss/states`).
- `boss` carga `at_144400` con el Lua recién iniciado y **sigue siendo `run4`**:
  `018F92` en 148579, `018F40` en 149169, `018870` en 149171 y las mismas
  cuentas de creaciones que el inventario (`04B87E` ×176, `04CD32` ×126…).
- `mid` no carga `at_132300`: con el Lua nuevo el plan de entradas cambia y la
  pelea dura 3500 fotogramas en vez de 8615. Repite `run4` desde
  `sec04_117966` (≈ 6,5 min) y traza desde `FROM`; así coincide con el
  inventario (`0A64E2` ×286, `0A6598` ×56, 30 objetos, muerte en 140653).
- Puntos de parada del depurador: solo registros, RAM y pila (`ALLOC 00410E`,
  `FREE 004126`, `ANIM 004256`, `SND 0120F8`, `END 018F92/018F40/018870`).
  Leer memoria en un puntero arbitrario desde un punto de parada cambió la
  partida en la captura del inventario.
- Tamaño: la traza completa ocupa ≈ 40 MB por cada 1000 fotogramas del jefe
  (12 MB en 144800..145100); filtrada, 30 MB para todo el combate. Tras
  extraer se borraron las trazas (disco); quedan los estados y los
  `debug.log`.

Fotogramas en la convención de `temp9` (la de MISION3_ACTORES.md). **El mundo
actualiza a los actores cada 2 fotogramas** (el cuerpo se actualiza en los
impares 144483, 144485…): «actualización» = 2 fotogramas. Partida de
referencia: 1 jugador, `100088 = 2`, rango `10FD8B = 3`, `+68 = 0` en todos
los nodos de las dos familias; los valores de las tablas de rango
(`0260C8`) que se dan abajo son los de ese caso, con los de otros rangos
cuando cambian.

Datos de ROM: el código está en P1; las tablas `2C1380..2C25C8` están en la
**ventana 0x200000 con el banco 1 de P2** (offset `0x100000 + (a − 0x200000)`
de `250-p2.ep1` con los bytes intercambiados); el banco 0 tiene otra cosa en
esas direcciones (el volcado `bank147000.bin` de `run4` es del banco 0).

## Llegada

- Sección 5 de la lista de cámara (144371): sonido 6, `call 0B8EA4`,
  `call 0B8D28`; la cámara se para en 6496. El guion (registro `2DB5E4`) crea
  el cuerpo en **144483** (nodo `1018E4`).
- Constructor `04A2C2`: música 0x28 por `059F4E` (nodo diferido `059F76`,
  192 fotogramas), paleta (`009F88`), `+66/+1E = FFF1`, crea en este orden
  el torso `04B252` y las patas `04AF02` y `04B0E6`
  (`0040BC`, reserva 0xE); `+9D = 3` (de pie), `+60` bit 0 (entrada), `+5F`
  bit 6 (**no se le puede dar**), `01ABC8`, programa `2451DC`, **vida `+4E`
  = `2C1380`** (5500), `+8A = 0x100`, velocidad base `+62 = 0x8000` y el
  humo de daño común `01DD10` → `01D86C` (ya nativo; humo `01D944` por debajo
  de media vida, `01D958` por debajo de un cuarto).
- Entra andando desde `+3C = 384` (fuera por la derecha) hacia 0x100 con la
  velocidad doble (`04D66E`: `+62·2` con `+60` bit 0) en **276 fotogramas**
  (144483..144759). Al llegar (`04A448`: `+3C ≤ 0x100`) quita `+60` bit 0 y
  `+5F` bit 6 (ya recibe daño: primer impacto en 144757) y llama a `004520`
  (0x148).
- El torso crea el brazo `04B3BC`, la lanzadera `04B920`, la trampilla
  `04C38A` (que crea `04C69E`) y el mortero `04CF2C`, todos en 144483.

## Piezas

| Nodo (run4) | Constructor | Pieza | Papel `+A1` | Atiende `+9D` | Notas |
| --- | --- | --- | --- | --- | --- |
| `1018E4` | `04A2C2` | cuerpo | — | elige | única pieza con vida; caja `+50` por pose (`04E008`), mapa dinámico `0E0D00` (`04E04E`) |
| `104384` | `04AF02` | pata 1 | 1 | 3 / 4 | programas `2451E8`, de pie `24585A`, agachada `24569C`, muerte `2459D0`; paletas 0xBA/0x157 |
| `103514` | `04B0E6` | pata 2 (`+15` bit 2) | 2 | 3 / 4 | `245294`, `2458C8`, `24571E`, muerte `2459DC` |
| `104BC4` | `04B252` | torso | 0 | 3 / 4 | `245340`, `245936`/`2457A0` (sonido 1078 ×9); pasa su pose `+98` al cuerpo; se borra con `+9D = 7F` |
| `102A14` | `04B3BC` | brazo lanzallamas | 3 | 0 / 1 | `2459F4`/`2459E8` (según `+98 ≥ 0x25`), disparo `245A48` (sonido 1065) |
| `103D54` | `04B920` | lanzadera de misiles | 4 | 6 / 2 | `245D86`, abre `2460D8` (1075), `245EFC`, `245DF2` |
| `1040C4` | `04C38A` | trampilla delantera | 5 | 7 | `246262`, `2463EA/2463EE`, `246480/246484`, cierra `246326`; escribe `+A3` en el cuerpo |
| `101834` | `04C69E` | suelo de la trampilla | — | (lee 7) | mapa dinámico `0E0E36` (`0140C4`, altura `+9F` de la trampilla) mientras el cuerpo tiene `+9D = 7` |
| `102D84` | `04CF2C` | mortero (sin sprite) | 6 | 5 | lanza `04D1DA` desde el punto de la pose (`E0F9A[+98]`) |

**Sincronización** (`+9D` orden, `+9E` estado del apretón de manos): el
cuerpo pone `+9D = orden` y `+9E = FF`; cada pieza mira la orden de su padre
o abuelo (`04DDD4` patas y torso, `04DF02` hijos del torso, por `+A1`), la
copia y pone `+9E = 7F` en el cuerpo; al acabar el ataque la pieza borra el
`+9E` del cuerpo. Posturas: 3 = de pie, 4 = agachado (las patas y el torso
cambian de programa con la velocidad `+26` del cuerpo: 0x200 en fase 1, 0x300
en fase 2 al agacharse; 0x100/0x200 al levantarse); `7F` = muerte.

## Estados del cuerpo

| Rutina | Qué hace | Salida |
| --- | --- | --- |
| `04A380` | `+A0` bit 6 → `+9D = 7`, `+9E = FF`; `+30 = ±+62` hacia `+8A = 0x100`; `+80/+84` = periodo de decisión (`2C1444` / `2C14A6` en fase 2: 1) | `04A3F0` |
| `04A3F0` | anda (`04D66E`), fase (`04D6F2`), caja y mapa, `004C24`; tras la entrada decide un ataque cada `+80+1` actualizaciones (`04DA38`) | orden 8 → `04A488`; orden 4 → `04A668` con `+A0` bit 3 |
| `04A488` / `04A4C2` | paso a la izquierda (hacia el jugador): a `+28 − 0x30` a 3 px/act. si queda sitio (`+3C − 0x30 > 0x30`); si no, `04A512/04A57C` (espera `+88` de `2C17FA`/`2C1814` = 5, al azar `+A0` bit 1) | `04A668`; desde `04A57C`, `04A94C` o `04A5DE` |
| `04A5DE` / `04A618` | paso a la derecha: a `+28 + 0x30` si `+3C + 0x30 < 0x100` | `04A668` o `04A94C` |
| `04A668` | parada: `+88/+82` = `2C17FA/2C182E` (5/1) con `+A0` bit 3, si no `2C1862/2C1896` (fase 2 `2C187C/2C18B0`: 8/10); pide las pisadas (`+60` bits 2/4) | `04A74A` |
| `04A74A` | espera las pisadas y `+82`; luego `+9D = 4` (agacharse), `+A1 = 1`, `+9E = FF` | `04A7CC` |
| `04A7CC` | agachado: espera a las piezas (`+9E = 0`), cuenta `+88`; segunda elección `04DC5C` (con `+A0` bit 3) o, si un jugador apareció o desapareció (`04AE98`), se levanta | `04A89E` |
| `04A89E` / `04A8CA` | `+9D = 3`, `+A1 = 0`, `+9E = FF`; espera a las piezas | `04A94C`, `04A488` o `04A5DE` según `+A0` bits 3/2/1 |
| `04A94C` / `04A974` | vuelve a x 0x100 a 3 px/act. | `04A380` |
| `04A9D4` | muerte (abajo) | `04AA3A` |

**Elección de ataque** (`04DA38`, cada 2 actualizaciones; solo si `+9E = 0`
y hay un jugador, `01B04A` → `+6E`): si el cuerpo está agachado
(`+A1 = 1`) primero se pone de pie (orden 3). Si el jugador está a menos de
0x40 por la izquierda del cuerpo (o detrás): orden 5 si está 0x60 o más por
encima de `+40`, si no 4. Si está lejos: tabla de azar (`0260C8` tipo 2:
`0029C6 & 0xE` elige una de 8 palabras), índice → orden por la tabla de bytes
`04DC54` = `[0, 1, 7, 6, 2, 5, 4, 8]`:

| Tabla | Caso | Palabras (1 jugador) | Órdenes |
| --- | --- | --- | --- |
| `2C15F2` | a pie, fase 1 | 2 1 6 2 1 0 6 2 | 7 1 4 7 1 0 4 7 |
| `2C1640` | a pie, fase 2 | 1 0 1 2 0 6 2 4 | 1 0 1 7 0 4 7 2 |
| `2C1556` | en vehículo, fase 1 | 4 0 3 5 3 3 4 0 | 2 0 6 5 6 6 2 0 |
| `2C15A4` | en vehículo, fase 2 | 6 0 6 4 5 6 3 6 | 4 0 4 2 5 4 6 4 |

(«en vehículo»: `0AAA88(jugador) ≠ 0`.) La orden 7 se cancela si algún
jugador está en un vehículo (`04DD86`). Agachado, `04DC5C` elige de
`2C172A`/`2C1778` (a pie) o `2C168E`/`2C16DC` (vehículo), con 4 y 8 → 7 (y
7 → 0 si hay un jugador en vehículo). La tabla `2C1508` (todo 1) hace que
siempre se elija al agacharse.

**Fase 2**: `04D6F2` pone `+A4 = 1` cuando `+4E ≤ 2C13E2` (2500 en los
rangos 0–3, 3000 en 4–5, 4000 en 6–7; 3500 con dos jugadores en el rango 3).
Solo se evalúa en `04A3F0`. En la captura, 146469 (vida 2460). Cambia las
tablas de elección, los tiempos de parada, la velocidad de los programas,
las ráfagas del brazo, las andanadas de misiles, los disparos de la
trampilla y la espera del mortero (abajo).

### Cronología observada (run4)

| Fotograma | Cuerpo | Orden | Vida |
| --- | --- | --- | ---: |
| 144483 | nace, entra | 3 | 5500 |
| 144759 | para en x 254, se agacha | 4 | 5490 |
| 144885 | agachado | **0** lanzallamas ×3 | 5430 |
| 145091 | se levanta, vuelve | 3 | 4840 |
| 145161..145467 | anda | **5** mortero ×4 andanadas | 4620 |
| 145569 / 145681 | se agacha / ataca | 4 / **0** ×3 | 3530 |
| 145965 / 146061 | se agacha (sin ataque) | 4 | 3160 |
| 146239 | anda | **7** trampilla | 2930 |
| 146469 | **fase 2** | — | 2460 |
| 146589 / 146615 | se agacha / ataca | 4 / **2** misiles (3 andanadas de 3) | 2320 |
| 146983 / 147065 | se agacha; anda | 4; **1** lanzallamas al jugador ×2 | 1810 |
| 147283 / 147583 / 147609 | se agacha ×2; agachado | 4; **5** mortero | 1510 |
| 147703 | anda | **7** trampilla | 1380 |
| 148063 / 148089 | se agacha / ataca | 4 / **1** ×2 | 740 |
| 148255, 148309 | anda | **5** ×2 | 430 |
| 148487 / 148513 | se agacha / ataca | 4 / **0** ×2 | 200 |
| 148577 | vida 0 | → `04A9D4` | 0 |

Órdenes 6 y 8 no salieron en esta partida (**sin comprobar** en ejecución).

## Ataques

| Orden | Pieza → proyectil | Puntería y ritmo (1 jugador, rango 3) | Daño al jugador | Sonidos | Visto |
| --- | --- | --- | --- | --- | ---: |
| 0 / 1 | brazo `04B3BC` → emisor `04B79C` → tramos `04B87E` | apunta girando `+9A` hacia `+9C` (un paso cada 3 act., `04D768`); con 0 el blanco es un punto al azar 0x10..0xAF delante del brazo, con 1 el jugador (`04D798`, ángulo `01B12E`, tabla `04D8C6`); tiempo de puntería `+80` = `2C21AE` 5 / fase 2 `2C21C8` 1; ráfagas `+82` = `2C21E2` 3 / `2C2244` 2 | cada `04B87E` vive una actualización con la fila `E0F4A` (rect. −4,−1, 4×2) en el punto de la pose `E0F9A[+98]` menos `2·+9A` | 1065 | 12 emisores, 176 tramos |
| 0 / 1 | brazo (programa `245A48`, llamada `04CEC6`) → bola `04CAB4` → estela `04CD32`, charco `04CCE4` | parábola hacia `(+72, 0x120)`: `+72` = x del brazo − distancia por tablas (`04D91A` fase 1: ≤120→100, ≤160→140, ≤200→180, si no 220; `04D92A` fase 2); velocidad `2C20EA`/`2C214C` (0x800) y `03B36E`; una estela cada 0x1200 de recorrido | `E0F5A` (−8,−8, 16×16) en vuelo; al tocar → `01C706` | 1021 (explota), 1025 (charco `24855C`) | 11 bolas, 126 estelas |
| 2 (6) | lanzadera `04B920` → misil `04C194` | abre (`2460D8`, 1075, 10A1), apunta (`04D94A` hacia el jugador, paso cada 4 act.) `+80` = `2C2372`/`2C23A4` (4) act.; andanadas `+82` = `2C257A` 2 / `2C2594` 3 (vehículo `2C24B6` 8 / `2C2518` 12); 3 misiles por andanada (`04E154`: ángulos y puntos de `04E19E`) en línea recta a `2C22DA`/`2C22F4` (1310·256 por act.); pausa entre andanadas `2C23D6`/`2C2408` (12/11) | `E0F8A` (−4,−4, 8×8) | 1062 por misil, 109F al explotar (`01C6EE`) | 9 |
| 5 | mortero `04CF2C` → bomba `04D1DA` | `+82` = `2C1CF4` 4 bombas por andanada (`2C1DBA` 4), espera `+88` = `2C1CC0` 48 / `2C1CDA` 24 act.; sube con gravedad (`+34 += 0x4000`) hasta `+40 ≥ 0x1E0`, se para y se lanza `+84` = `2C1E1E` 5 veces hacia el jugador (`01B12E`, velocidad `+8E` de tabla), con esperas al azar `2C1EEE` (20..60) | `E0F7A` (−6,−6, 12×12); si una toca, explotan todas (`+9F` del mortero) | 109A (lanza), 1022 (explota, `01C6EE`) | 28 |
| 7 | trampilla `04C38A` (`04CE78`/`04CE98`) → `04C7B6` y `04C840` | abre `246262`; `+84` = `2C25AE` 7 / `2C25C8` 5 disparos de `04C7B6` (−0xE000 por act.) y 16 de `04C840` (−10 px por act.), desde (−0x38/−0x34, −0x44) | `E0F6A` (8×8) y `E0F5A` (16×16) | 10EF en `04C566`/`04C5E6` (probablemente del programa, **sin comprobar**); `012192(10EF)` al morir la pieza | 12 + 32 |
| — | pisadas `04E2EA`/`04E3BE` (al agacharse) → polvo `04AD12`/`04AD94` ×3 | — | ninguno | 10EE | 9+9, 27+27 |

Filas de ataque `E0F4A..E0F8A` (para `0049E8`): rectángulo (x, y, ancho,
alto), luego 7 (6 la del misil), 0, 0x10, FFFF (campos sin interpretar). El
cuerpo y las piezas **no llaman a `0049E8`**: no hay daño por contacto en su
código (**sin comprobar** si el núcleo del jugador lo hace por el mapa
dinámico `0E0D00`). Alternativas en la ROM que no se ejecutaron: `04C952`
(bola recta, `2C20B6`/`2C20D0`), `04E140` (`04C220`), la orden 6 de la
lanzadera (`04BA94`).

## Vida y daño

- **Solo el cuerpo tiene vida** (5500; tabla `2C1380`: 5000 en los rangos
  0–1, 5500 en 2–3, 6000 en 4–5, 7000 en 6–7; 7500 con dos jugadores en el
  rango 3). Las piezas tienen `+4E = 0` y no se registran.
- Registro: `004C24` en cada estado del cuerpo; la caja `+50` sale de
  `04E008`: tabla `E108A` por la pose `+98` (que pone el torso) → `E103A`; la
  entrada `E0E8A` se indexa además por `+A3` (estado de la trampilla).
- Impactos (`004892..0049E0`, escritura en `004996`): **pistola −10**
  (filas `01B4C2..01B542`, 215 balas), **granada −100** (`01B5E2`, 33; una
  de −50). 5500 = 215·10 + 33·100 + 50.
- `+5F` bit 7 (vida 0) → `04A9D4` desde cualquier estado.
- Las bombas del mortero sí se registran (`004C24` en `04D630`): vida
  `2C1D58` = 10 (un disparo) a pie, `2C1962` = 40 con un jugador en vehículo.

## Muerte y fin de la misión

| Fotograma | Rutina | Qué pasa |
| --- | --- | --- |
| 148579 | `04A9D4` | sonido 1026, **`018F92`** (fin de misión, paso 1), `013F1E` (quita su plataforma de objeto), `+61 = FF` (las piezas dejan de atacar: `04D74A`), explosión `01C71E` en (0, 0x96), lista de explosiones `01DA40(E0C9C)` |
| 148579..148835 | `04AA3A` | cuenta 0x80 actualizaciones; en 0x40, 0x28, 0x10 listas `E0C42`, `E0C54`, `E0C66` (explosiones `01D764` → restos `01CE08..01CE44`, `01C706`, `04E558/04E568` con 1022) |
| 148835 | `04AAAE/04AADE` | si estaba agachado, se levanta (orden 3) y espera; si no, 16 actualizaciones |
| 148871 | `04AB10` | `+9D = 7F`: las patas pasan a `2459D0/2459DC`, el torso y sus piezas se borran; temblor `+7A = 0x20000` (`+34` alterna cada act.), `E0C78`, `E0CAE`, `0C801A`, sonido 1032 |
| 148873.. | `04AB82/04ACCC` | se arrastra a la izquierda 0x20 y 0x30 px (tabla `04AD0C`, 2 px/act., pausa 16 act., 1032 cada tramo) y luego acelera (−0x2000/act.); al pasar `+3C ≤ 0x10`: `E0C8A`, `01DB74` (restos `01D3D2` ×16), `0C801A`, sonido 1033 |
| 149169 | `04AB82` | fuera de la vista (`01945C(E0E82)`): **`018F40`** y borrado (`004188`) |
| 149171 | — | `018870` (actor de misión), recuento `0308C0` (149229), MISSION COMPLETE `06090A` (149428), Misión 4 |

El jefe **no crea prisioneros ni objetos**. Los prisioneros de la sección 5
son del guion (`099862` en 146283, regalo `06`; `099E1C`). Tras `04AB82`
hay el chequeo de la protección (`04AC3C..04ACC4`, RAM 10F00A/10F00C, como en
la M2: no se porta). El flujo `018F40` → fin de misión ya está en el port
(B24, `neo_mission_ends_after_stage`; JEFE_MISION2.md: «Vale también para la
única etapa de la Misión 3»).

## Jefe intermedio `0A4A94` (aeronave «96»)

Registro `2DAE20` (cámara 3944, **132378**); el guion espera `group0 == 0`.
Pantallas `game133500`, `game135000`.

- **Piezas** (todas en 132378): cuerpo `0A4A94` (nodo `101834`, **vida
  3000**), `0A606E`/`0A6078` (programas `2A4D16..`/`2A4D22..`), cañones
  laterales `0A61E8` (`103514`) y `0A61F2` (`102A14`) con **400** cada uno;
  `0A5E88` (132674, programa `2A5950`, toda la pelea); bodega `0A4DB6`
  (134292). La superposición `10E8C6` la escribe la subrutina `0A6C8C`
  (`0C6F34` con el mapa `2A6142`).
- **Llegada**: baja con `+8A = −0x15000` de y 520 a 327 (132378..132672,
  `0A4B52`), sonido 10ED; en `0A4B68` deja de ser intocable (`+5F` bit 6).
- **Movimiento** (`0A4BCE`: `0A6A86` + `0A5830`): x 120..216, y 328..346;
  el ciclo `0A4BCE → 0A4B9C → (0A4BAA) → 0A4BC0 → 0A4BCE` se repite cada
  ≈ 440..600 fotogramas (sonidos 10EB/10EC).
- **Ataques**: los cañones ciclan `0A6260 → 0A6270 → 0A628C → 0A62A4 →
  0A62BE → 0A62D6 → 0A62F0 → 0A6308 → 0A6326` (≈ 442 fotogramas) y disparan
  `0A64E2` (286, 32 de ellos creados por otro `0A64E2`); la bodega suelta
  `0A4F3A`/`0A4F52` (7 + 7, sonidos 1067, 1020, 102F), que tiran `0A5398`
  (32, sonido 10F2; 1022 al explotar) y restos `01D31C` (112).
  **Sin comprobar** qué son en pantalla (paracaidistas o bombas).
- **Daño**: pistola −10, granada −100 al cuerpo y a los cañones. Etapas de
  daño del cuerpo (`0A5C04`, sonido 1023 ×14); en dos de ellas crea el motor
  ardiendo `0A5D6A` (134729 con vida 1950, 136002 con 1560), que crea las
  explosiones `01D764` ×235 (→ `01C8D2`). Cañón a 0: `08D0D2` (135108 el
  izquierdo, 138797 el derecho); el nodo sigue vivo.
- **Muerte** (140653): `0A4BDA` → `0A4DB0` (`004BFE`), `0A6F1A`, `013F1E`,
  `0C8022`, 20 act.; sonido 1032 (`0A4C3A`), 30 act.; explosiones
  `01DA40(2A647A)` (`0A4C7E`), 60 act.; pantalla blanca `0AE1DC` (140890,
  `0AE220`); `0A4D04`: sonido 1033, **30 objetos `08C374` de tipo `55`**
  (140898, puntos/comida; sus etiquetas `08D330`); 107B y borrado de la
  familia (`0041AC`, 140988). El guion sigue al vaciarse el grupo 0.

## Jefe intermedio: estado del port (paso 3, rama `work-m3step3`)

**Nativo** en `src/neo_m3mid.c` (traducción a mano de `0A4A94..0A7140`):
cuerpo y su objeto de capa `10E8C6` (`0A6C18`: `0C7BD0`, `0C6F34`,
`0C6BDE`, `0C73CE`, `0C6CFC`, `01AB9C`; el paso `+8/+C` de `0A5B3C`; en el
mundo el objeto se mueve en la cámara con `10E959` bit 7, `0C82F4`), el
ciclo `2A610A` (`0A5882..0A59CE`), daño y etapas (`0A56E4`, `0A6BE2`,
`0A5C04`: parches `0A6D7C`, restos `0A6598`, motores `0A5D6A`), piezas
laterales `0A606E/0A6078`, cañones `0A61E8/0A61F2` (rayo `2A5FAE`, puntos
`08D1D0`), piezas `0A64E2`, escotilla `0A5E88`, bodega `0A4DB6` y sus
soldados `0A4F3A/0A4F52` (bombas `0A5398` por la llamada `0A530C`, objetos
`2C0C6C`), muerte `0A4BDA..0A4D52` (`0A4DB0`, listas `2A6526/2A647A`, mapa
del naufragio `2A619A`, `0AE220(3,1)`, 30 objetos `55`, `0A5604`, `0041AC`)
y el efecto común `01C8D2` (`neo_effects.c`). Las lecturas de la ROM nuevas
están en `data/region_rom.json` (`2A5FAA..2A6559`, `2C001A..2C0C7F`,
`0A534E`, `E61BC`, el mapa de la plataforma `0E6896`) y los programas en
`data/soldier_programs.json`.

**Comprobado** (`make check`, bucle cerrado contra MAME 0.264, capturas
`capture_m3_boss.sh mid` desde `sec04_117966`; extracción con
`--range a4a94-a7140 --range 1d764-1d7b8 --range 1c6ee-1c796 --range
1d31c-1d6e4 --range 1c8d2-1c8d2 --range 8c374-8c37c --range 94ff8-95060
--attacks --ram 10000e-100010 --ram 10c7b6-10d7b6 --ram 10d83e-10e83e --ram
10e8c6-10e94e --ram 10e958-10e95a --ram 10e9c8-10e9d0 --ram 1063d8-1063da`):

| Fixture | Fotogramas | Nodos | Actualizaciones | Contenido |
| --- | --- | ---: | ---: | --- |
| `m3mid1` | 132378..134399 | 95 | 10 181 | llegada, cañones, piezas, bodega (134292) |
| `m3mid2` | 134400..136599 | 288 | 16 809 | soldados, bombas, motor 134729, cañón izquierdo a 0 (135108) |
| `m3mid3` | 136600..138799 | 331 | 17 469 | fase 2, motores, bombas |
| `m3mid4` | 138801..140988 | 373 | 15 732 | cañón derecho a 0, muerte 140653, pantalla blanca, 30 objetos, borrado |

Las cabeceras de la capa (`10E8C6..10E94D`), `10E959` y el temblor
`10E9C8..10E9CF` (`0C8022`) se comparan byte a byte. La prueba de familia
gana `--m3mid` (piezas que se reanudan como `--m2boss`, nodos reutilizados,
`0041AC` borra los descendientes, el bit 7 de `+5F` que pone un disparo
entre dos actualizaciones del cuerpo). Dos fugas de registro del original
reproducidas: `0A4F1E` (bits 8..11 de D0 en `+8B` de la bodega) y
`0A5604` (D0/D1 que deja el último `01DA40`: `+32/+34` de la capa).

**RAM (parada explícita del cartucho)**: el módulo, sus bloques de ROM y
sus 38 programas suman ≈ 48 KB de imagen (texto 23 KB de `neo_m3mid.o`,
1000 órdenes de programa, 9 KB de datos); con ellos el cartucho de
referencia de la M3 no arranca (`RAM budget: 3296 KB left for the scene
cache (minimum 3332 KB)`, 208 ranuras). Por eso van en el grupo `m3mid`
(`group` en `data/region_rom.json` y `data/soldier_programs.json`,
`compile_region_rom.py`/`compile_soldier_ai_tables.py`) y la consola los
compila solo con `make MSX_M3MID=1`; sin él el registro `2DAE20` sigue
saltado (constructor no portado) y la prueba de familia del anfitrión los
compila siempre. Para meterlo en la imagen hace falta sacar a los paquetes
de misión los programas y bloques de ROM de cada misión (o liberar ≈ 40 KB
en otra parte). El código del mundo y del renderizador para la capa
(`MSX_HAS_M3MID`, `neo_gactor.h`) va en el mismo grupo. Con el grupo fuera:
`mslugx-m3-reference.z64` arranca con 208 ranuras y `test_mission3_ares.py
--hop --pilot` (300 s) da PASS (registro `2DAE20` saltado y registrado).

**Paradas explícitas**: la segunda tabla del ciclo `2A6126` (`+A3` solo
vale 0), la apertura de la escotilla `0A5EDA` (soldados `09D846..` de
`2A63D0`, nunca alcanzada), las trampas de depuración de `0A6918`/
`0A6A32`, un `divu` desbordado en `0A6918`; la comprobación del cartucho
antes de los borrados no se porta. **Dibujo de la capa `10E8C6`** (el
avión en sí: mapa `2A6142` de 19 × 10 bloques, parches `0C714E`, naufragio
`2A619A`): el mundo lleva la rejilla de 19 × 10 bloques que deja
`0C714E` (`m3grid`) y `actor.c` la dibuja en `x = +14 + 16c`, `y = 337 −
+18 + 16r` (SCB de MAME en los volcados 134100/135000) con 390 marcos
`F0000000 | código << 2 | giro` y la paleta `F05E` que
`tools/export_m3_overlay.py` añade al export privado de la M3 (paleta igual
a la RAM de MAME); bajo todos los sprites (el original la pone bajo la capa
`10C72E`: aproximación). `mission3.pack` pasa a 446 trozos (límite 480).
Sonidos `10EB/10EC/10ED/107B/1020` sin entrada en `data/sound_map.json`. Las piezas con
sprite (cañones, piezas, soldados, bombas, motores) sí: `export_rest_sprites.py`
con las raíces nuevas (captura `MSX_M3BOSS_DUMPS=133500`, 133500..136000:
15 453 píxeles iguales, 1339 distintos en 3 de 10 volcados, donde la
actualización coincide con el volcado).

## Implementación propuesta (piezas comprobables)

Como el jefe de la M2: módulo nativo `neo_m3boss.c` (actor genérico, rutinas
con su etiqueta de ROM), datos nuevos en `region_rom.json` (código
`04A2C2..04E577`, tablas `2C1380..2C25DF` del **banco 1**, filas `E0C42..E108A`,
`E0F4A..E0FFF`, `04D8C6..04D944`, `04DC54`, `04E19E`, `04AD0C`) y programas
`2451DC..2486xx` en `soldier_programs.json`. Cada pieza con su fixture de
familia en bucle cerrado (`--m2boss`-like en `test_neo_family.c`):

1. **Esqueleto**: cuerpo (entrada, andar, pasos, parada, agacharse,
   levantarse, vuelta; `04DA38` con `0260C8` tipo 2; fase), patas, torso,
   apretón de manos `04DDD4/04DF02`, pisadas y polvo, caja `+50` y mapa
   `0E0D00`, `004C24`, música `059F4E` (nativa), humo `01DD10` (nativo).
   Fixture `m3boss1` (144410..145600).
2. **Brazo**: `04B3BC`, `04B79C`/`04B87E`, `04CAB4`/`04CD32`/`04CCE4`
   (`03B36E`, `04E5D0`, `04E648`, `002F1A`). `m3boss1`, `m3boss2`, `m3boss4`.
3. **Mortero**: `04CF2C`, `04D1DA` (registro de impactos). `m3boss1`,
   `m3boss3`, `m3boss4`.
4. **Trampilla**: `04C38A`, `04C69E` (mapa `0E0E36`), `04C7B6`, `04C840`.
   `m3boss2`, `m3boss4`.
5. **Lanzadera**: `04B920`, `04C194` (`04E154`). `m3boss3`. Paradas
   explícitas para `04C952`, `04C220` y la orden 6 hasta verlas.
6. **Muerte**: `04A9D4..04ACDC`, listas `01DA40` (`E0C42..E0CAE`),
   `01DB74`, `04E558/04E568`, `018F92`/`018F40`. `m3boss4`, `m3boss5`; y la
   prueba de flujo `release_run` hasta la Misión 4.
7. **Aeronave**: `0A4A94` y sus 15 constructores, `0A6C8C`. `m3mid1..4`.

Tramos (todos cargan el mismo estado; ≈ 40 MB de traza por 1000
fotogramas, extraer y borrar uno a uno):

| Fixture | Tramo | Contenido |
| --- | --- | --- |
| `m3boss1` | 144410..145600 | llegada, primera agachada, lanzallamas (0), mortero ×4 |
| `m3boss2` | 145600..146600 | lanzallamas (0), trampilla (7), fase 2 (146469) |
| `m3boss3` | 146600..147700 | misiles (2), lanzallamas al jugador (1), mortero |
| `m3boss4` | 147700..148600 | trampilla, (1), mortero ×2, (0), vida 0 y `018F92` |
| `m3boss5` | 148600..149300 | muerte, arrastre, `018F40` (149169), `018870` |
| `m3mid1..4` | 132310..134400..136600..138800..141100 | llegada, cañones y bodega; motores; cañones a 0; muerte, objetos, borrado |

```sh
sh tools/capture_m3_boss.sh states /home/user/captures-m3boss/states-run   # una vez
sh tools/capture_m3_boss.sh boss /home/user/captures-m3boss/b1 144410 145600
python3 tools/extract_family_fixture.py /home/user/captures-m3boss/b1/boss-trace.txt \
  --output data/m3boss1_fixture.json.gz --range 4a2c2-4e577 --range 59f76-59fa8 \
  --range 1d86c-1da3f --range 1d764-1d7b8 --range 1c6ee-1c796 --range 1ce08-1ce44 \
  --range 1d3d2-1d3d2 --attacks --ram 10e99a-10e99e --ram 10c300-10c308 --ram 1063d8-1063da
rm -rf /home/user/captures-m3boss/b1      # tras extraer
sh tools/capture_m3_boss.sh mid /home/user/captures-m3boss/m1 132310 134400
python3 tools/extract_family_fixture.py /home/user/captures-m3boss/m1/boss-trace.txt \
  --output data/m3mid1_fixture.json.gz --range a4a94-a6ce9 --range 1d764-1d7b8 \
  --range 1c6ee-1c796 --range 1d31c-1d6e4 --range 1c8d2-1c8d2 --range 8c374-8c37c --attacks \
  --ram 10e99a-10e99e --ram 10e8c6-10e94e --ram 1063d8-1063da
```

Probado en esta rama: el extractor sobre un tramo de 300 fotogramas
(144800..145100) da 2509 actualizaciones de 41 rutinas del jefe. Los
`--ram` y las rutinas genéricas de la lista son una propuesta: el módulo
dirá cuáles lee de verdad. **Límite**: `MSX_FULL_FAMILY_RANGE` admite un
solo intervalo; en los tramos 2..5 el humo `01D86C` (creado en 144483) no
entra en la familia, y los nodos vivos al empezar arrancan de sus bytes
grabados, como en la M2.

## Dificultades

- **Coordinación por `+9D/+9E`** entre ocho nodos (y el abuelo en los hijos
  del torso): el orden de actualización de la lista decide en qué
  actualización ve cada pieza la orden; el bucle cerrado tiene que repetirlo.
- **Tablas de rango en el banco 1** de P2 y tablas de azar tipo 2 (la RNG
  `0029C6` se consume en la elección, en la bola y en el polvo).
- **Geometría que depende de la pose** (`+98` del programa del torso): caja
  `E108A/E103A`, puntos `E0F9A` del brazo, del mortero y de la lanzadera, y
  el mapa dinámico `0E0D00` sobre el que se pone el jugador; el de la
  trampilla `0E0E36` solo con la orden 7.
- Trayectorias: parábola `03B36E` + divisiones `04E5D0/04E648` de la bola y
  bombas que se reorientan; las bombas se pueden destruir.
- La muerte llama a `018F92` al principio y a `018F40` 590 fotogramas
  después, al salir de la vista: no cortar la prueba en `018F92` (B24).
- Aeronave: 8615 fotogramas, la superposición `10E8C6`, 286 balas y 235
  explosiones encadenadas; con el plan de entradas reiniciado la pelea es
  otra (por eso `mid` repite desde `sec04_117966`).
