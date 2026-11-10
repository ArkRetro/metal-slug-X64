# Misión 3: depuración

Punto de continuidad de la Misión 3. Se actualiza al final de cada sesión.
Referencias: `MISION3.md`, `MISION3_ACTORES.md`, `JEFE_MISION3.md`,
`PLAN_MISION3.md`, `BUG_MATRIX.md` (B18, B56–B58, B59, B62, B63, B65).

## Estado (ROM 44, `ccr-09eb2f6b-8rwl8l`; lista cerrada de `work-m3gaps` abajo)

- **Carga:** tras la M2, o desde el menú de pruebas (L+R o Z+R y A en
  SOLDIER SELECT). Pack v4 con el bloque MSXD, 821 de 832 trozos (G22).
- **Mapa y colisión:** 100 % frente a MAME en las 5 secciones (`test_m3_map`).
- **Actores:** todos los del recorrido de `run4` antes del jefe son nativos.
  Faltan las ramas al azar no vistas y `03C306` (granada, sin traza).
- **Jefe intermedio y final:** nativos (familias m3mid1..4, m3boss0..6; la 6 con el
  jugador en el SV-001: orden 6 de la lanzadera, G15).
  El fin de misión cae en la parada explícita "falta mission4.pack", que
  reinicia la M1.
- **Vehículos:** `093C9A` y `0AC866`. Arma E. Marco gordo y sus armas (bomba,
  molotov y polvo nativos desde `work-m3g7`; disparo revisado en G21). Salpicadura
  `03C962` nativa.
- **Fondo:** columnas pre-renderizadas en las capas 0 y 1 (`build/bgp/m3s1.bgp`);
  la capa del tren se dibuja por su anillo de columnas y el cielo por su propio
  `+14` (G24). El resto va por la caché de piezas.

## Verificado

- PC: `make check` con todas las familias M3 bit-exact.
- ares: recorridos con la ROM de referencia (piloto, invulnerable) hasta el
  jefe.
- Ojo: la ROM de referencia **escondió** fallos de la ROM normal. Hay que
  probar siempre con `mslugx-explorer.z64` o con `mslugx-m3-release.z64`.

## Fallos abiertos (MiSTer, ROM 41, cartucho normal)

1. ~~**Fondo de la M3 ausente**~~ (B60, corregido en `work-m3doors`). La foto
   (`scratchpad/rom41-photos/m3-black-bg.jpg`) es el **inicio** de la M3
   (cajas, cerdos, fogata con su humo). Experimento: `mslugx-m3-release.z64`
   (juego normal desde el registro de la M3) en ares, cámara 0: mismo negro
   (`MSX prerender: layers 2, ring 94 KB`); sin `m3s1.bgp` (`missing: tile
   path`) también negro; píxeles del fondo `(8,0,0)`. MAME (vídeo 8:04,
   `run4` cámara 0): fondo con cielo, árbol, locomotora. Causa: `map.json`
   toma la primera observación de cada celda y en x 0..304 era un fotograma
   del fundido de entrada (`run4` 117990/118020): 583 celdas con paletas
   negras (`0001`). Arreglo: `extract_mission_map.py --skip-fade` y
   reempaquetado (796 trozos). Ya se veía así en ares el 4-XI (`cap-m3s1/ares7`).
2. ~~**Puertas de los vagones con palanca**~~ (B57, corregido en
   `work-m3doors`): la puerta central es `0391EA`, que solo cae con un golpe
   de tipo 13; se lo da el empujón `0AF2BA` (fila `0AE9B0`, clase 1) de los
   soldados `+76` = 7 de dentro. El port lo comprobaba contra el jugador.
   MAME `run4` 121357 (`104BC4` → `101E64`): `test_neo_m3push.c` igual. En
   ares (referencia, piloto) las dos puertas de los vagones 7-24 se abren y
   salen los soldados.
3. **Tren en el minuto 8:30 del vídeo:** en ares arranca también con el
   cartucho normal sin piloto (ver «Lista cerrada»). Lo que sí faltaba en
   esa zona era la cámara baja del puente (G1, corregido) y el cielo (G2).

## Lista cerrada de diferencias con MAME (sesión `work-m3gaps`, 7-XI)

Fuentes: MAME `run4` desde `states4/sec04_117966` con
`tools/capture_m3_actors.sh` (233 constructores, 10 871 creaciones) y
`tools/compare_m3_events.py --list/--port`; ares con
`mslugx-m3-release.z64` (juego normal, mando `--hop`, 200 s: muere en la
espera de 1520) y con `mslugx-m3-reference.z64` (piloto, 330–480 s, hasta
5980); fotos de ares por cámara contra `tools/render_m3_sections.py`
(MAME | ROM | SCB de fondo, 4 por sección); capturas MAME cortas para las
paletas (`at_119570` con el plan `to:6c0`, `at_124100`). Orden: visibilidad.

| # | Diferencia | Evidencia | Estado |
| --- | --- | --- | --- |
| G1 | **Altura de la cámara**: `0B8EBC(n)` de la lista de cámara (puente 122145, hueco 123924..126262, final) no se ejecutaba; la cámara seguía a nivel y no se veían el puente, los caballetes, el río ni las balsas | MAME `trace.csv` `fore_y`: 320 en 2888..3103, 224 en 3296..3463, 256 en 3716; ares antes: 48→39 | **corregido**: `neo_camera.c` (0C8764: A0 = orden + 8; `MOVE.L (A0),6960`); `test_m3_camera_bridge` (−64 / +32 / 0 como MAME); ares: −64 en 2833..3136, +32 en 3171..3585, 0 en 3712 |
| G2 | **Cielo desde el progreso 0x140** (secciones 2–5): MAME cambia el cuadro del cielo (nubes y colinas, cañón, pueblo...) y el port pinta el color de fondo o restos del bosque | `0B8EDC` (creado por `0B8D34`, falta): con la columna del tren `10D7B6` pasando 0x50/0xB0/0x110/0x160 (tabla `0B8EC2`) carga paletas (`009D8C` `0B8948..0B8960`: recursos 0x480/0x484/0x48A/0x48E) y copia un mapa de 32×20 (`0C714E`) a la capa del cielo `10E83E`: `0B8592` (`31F2A0`), `0B85A8` (`31FCA0`), `0B85BE` (`3206A0`), `0B85D4` (`331A68`). Fotos: ares 387/1520/2888/3455 contra MAME 336/1520/2888/3463 | **corregido**: disparo nativo bit-exact (`neo_train_sky`, `test_neo_train`: los 4 cuadros con los `+32/+36` de MAME en 118892/121768/124930/144579); los 5 cuadros (0 nubes y colinas, 1 cañón, 2 pueblo con río, 3 pueblo al atardecer, 4 de noche) van **solo** a las columnas pre-renderizadas de la capa 0 (`prerender_stage.py sky_cells`, en 0x6000 + 1024 k, con los primeros 352 px repetidos; texeles de la C-ROM): el paquete solo gana sus 15 paletas (796 trozos, tablas +0,5 KB); anillo de la capa 0 en la RAM del paquete (anillo total 192 KB, caché 211 ranuras). `actor.c` dibuja la capa 0 desde 0x140 en x = tren / 4 − 240 módulo 512 (run4). ares (piloto): cañón en 833, pueblo y río en 2888 como MAME. El cuadro 4 (de noche, solo en el jefe) lleva 352 celdas animadas con su primera imagen: abierto (las columnas son estáticas; habría que llevar esas celdas por el camino de piezas; no sale en el tramo del tren). La x del cielo ya no es la del tren / 4 − 240: ver G24 |
| G3 | Paletas `3CF` (bomba `0286F8` del coche `027F54`) y `348` (pieza `028C76`) | ares release: 959 dibujos con la paleta de reserva en la espera de 1520; en `run4` el coche muere antes de tirar | **corregido** (export): captura `at_119570` + `to:6c0` (el jugador no dispara), paletas en `filesystem/mission3/rest-native.json`, paquete rehecho |
| G4 | Paletas `898/899` (bombarderos `0DC97E`), `87B` (torreta `03B4E0` del tanque), `4B4` (`0B91FA`) | ares piloto: 466+466 y 128 dibujos sin paleta | **corregido** (export, captura `at_124100`; las 6 paletas en `mission3.pack`); `4B4` sin uso hasta G9 |
| G5 | Soldados del campamento (`01F386/01F2D6` → `0B05FE`) desaparecen al explotar el fogón | ares (los dos cartuchos, f≈223): `world unsupported routine=b05fe`; MAME `m3camp` 118741: `+38 = 0010D7B6` (la capa del tren); el módulo de soldados no tiene padre tren | **corregido**: `neo_world.c` `convert_soldier` lo deja sin padre en su posición proyectada (como lo que aterriza en el tren); ares: sin `b05fe` ni aserción |
| G6 | Salpicadura `03C962` (balas, granadas y armas en el agua) | ahora que se ve el río: 21 paradas `3c962` en ares (granadas de `08A732`); `run4` no tiene ninguna | **corregido** (`work-m3g7`): `neo_gate_splash` (`03C5DE`: `03C55C` agua de clase 5 en y−1/+7/+15, sube de 8 en 8 por el objeto de la celda, y = (y & ~7FFFF) − 1; `03C962` tras el actor del guion en píxeles enteros con `+4C` = 0 y `+6E`) y `03C962/03C9BA` (paleta D1 de `019240`: 151 / 152, programas `03C952`, `+26`); las paradas de las balas `01B8AE`, granadas `08AD4A`, cohetes `05E04C`, `09C83E`, arma E `0DE656/0DE660`, `0862F2` y la granada `03C3DC` (G16: `009F88`, E0ED, `01C706`) pasan a `SCS`. Familia `m3splash` igual a MAME: `at_122200` (puente) con `MSX_FULL_PLAN=dive:bb2`, `MSX_FULL_GIVE_FORM=122505:0` (vuelve a flaco) y `MSX_FULL_GIVE_WEAPON=122510:0` (pistola): tiros hacia abajo por el hueco entre vagones (una salpicadura, 13 actualizaciones). Sprites validados (243 píxeles iguales). Sin `08A732` en el río: aparcado a la izquierda (`to:734/780/7c0`) solo tira 4 granadas al suelo; `08AD4A` y los demás no tienen traza propia |
| G7 | Marco gordo: bomba `01C362`→`01C654` (36/71), polvo de aterrizaje `0C4E86` (88), `0C5064` (al dejar la forma, 1); molotov gordo `01E680/01E69C` (no sale en `run4`) | `compare_m3_events.py --list`: `port=missing`; `gameplay.c` parada | **corregido salvo `0C5064`** (`work-m3g7`, `neo_bullet.c`): bomba tipo 3 (`0C476E`; `009EA8` deja en D0.w la ranura de paleta y en D1.w 20A, que el original suma a la velocidad: `neo_genv_t.palette_slot`, en el mundo 0), su pieza `01C654`, el polvo `0C4E86` (`+1E` del jugador − 1; `gameplay.c` lo crea al aterrizar con `+93 = 2` y guarda `depth1e` en el nodo del jugador) y el molotov gordo (`+88 = 1`, paleta 872, `22F65E`; las llamas `01EA7A` ya eran nativas). Familias `m3bomb` (`sec04_117966`, 124100..124700, raíces `1c362,1c654,c4e86,c5064` y nodo `1014C4`) y `m3molotov` (igual con `MSX_FULL_GIVE_BOMB=124740:2`, 124750..125350) iguales a MAME; sprites del export privado (paletas 872/873, 152), paquete de 811 trozos (`SCENE_MISSION_CHUNKS_MAX` 832). Falta `0C5064` (`0C359A`): `MSX_FULL_GIVE_FORM=122505:0` deja la forma por `0C1D82` sin crearlo; en `run4` sale en 146069 (jefe). El `01C3BE/01C390` flaco comparte código, sin captura (sigue parada en `gameplay.c`) |
| G8 | Sonidos sin efecto: 31 palabras | SND de la captura contra `msx_sfx_policy.h`: `1020` ×13 y `102D`/`109E` (aeronave/jeep), `10EB` ×33, `10EC` ×32, `10ED` (jefe intermedio), `1169` ×40 (arma E), `1062/1065/1078/109A/109F/10A1/10EE/10EF/1026` (jefe), `1079/107A/107B` (`0186A0`, órdenes de cámara 0B8D10..28), `1029` (puertas), `105E` (`04E990`), `108B` (`0AC866`), `109D` (`027F54`), `10BA` (`097FCC`), `10D5` (flecha), `10FC`, `1133` (voz MISSION 3 START), `1139`, `1153`, `1155`, `116D` | **en parte corregido** (`work-m3music`, AUDIO_JUEGO.md 17): render aislado de las 30 y pares medidos (4472) contra la política: **18 con política** (`1169`, `10EB/10EC/10ED`, `1065`, `1029`, `102D`, `10EF`, `10D5`, `10BA`, `105E`, `109F`, `109D/109E/10EE`, `10FC`, `1133`, `1153`; desde `rom:/`); sin política porque moverían sonidos medidos: `1020`, `1062`, `109A`, `116D`, `108B`, `10A1`, `1139`, `1155` (y `1026`); bucles sin soporte: `1078/1079/107A/107B`. Música (B66): `0024` (descriptor `016E0E` +C, latch 117968) y `0028` del jefe (`059F76`: `0A 80` en 144484, `00 28` en 144677) renderizadas y en `game_audio.c`; sin más peticiones de música en la M3. Pendiente: el fundido `0A` del controlador (el port para la música) |
| G9 | `0B91FA` (adorno en (0xE00, 0x40) de la raíz, programa `2A8E40`, paleta 4B4, hasta el progreso 0xF80): es el **frente del vagón 7908** de la sección 3 | `0B91D0` de la lista (124034); el port no atendía la llamada | **corregido** (`work-m3lever`): llamada `0B91D0` en `neo_world.c`, `0B91FA/0B9236` en `neo_m3props.c`, raíz `2A8E40` (descriptor `1CF510`) en `extract_soldier_programs.py`/`export_rest_sprites.py`. Captura `captures-m3lever/g9` (`sec04_117966`, `MSX_FULL_FAMILY_ROOTS=b91fa`): nodo en (3584, 64) de 124036 a 124400 como el port; el validador de export salta los volcados (paleta 4B4 compartida con celdas auto-animadas), así que se comparó a mano: 84 % de los píxeles opacos iguales en `game124050.png` (el resto, tapado por soldados y humo) |
| G10 | Rutinas de soldado `0AF7B0` y `0AF99A` (acciones de `0AF1A8`, externo) | ares piloto: 2 + 1 paradas | abierto |
| G11 | `routine=800040` (nodo con rutina basura) en 3296 (jeep `0A2F2E` + `08AFB6`), con 16 externos `02A448` | ares piloto f=6643 | abierto (sin diagnosticar) |
| G12 | Paleta `0A7` (piloto `04EF88` de `04E990` con sangre) | ares: ~2400 dibujos; `run4` va sin sangre | **corregido** (`work-m3lever`): `tools/capture_fullgame_blood.lua` (10FD8F = 1, como `capture_blood.lua`) desde `sec04_117966`, volcados 122230..123300 (`captures-m3lever/g12`): 100 volcados con 0A7, un solo juego de colores; `export_rest_sprites.py --palette-capture`; las otras paletas, sin cambios |
| G13 | Etiquetas: letras `08C954` del rótulo de bombas (`08C752`, externo, 5), puntos `08D0D2` de los cañones del jefe intermedio (2) | `--list` | abierto |
| G14 | Fuego `01C950` del molotov `056B00` de los lanzadores (3) | `--list` | abierto |
| G15 | **El jefe deja de atacar con el jugador montado** (MiSTer, ROM 44; B64): orden 6 de la lanzadera (`04BA94`, misil `04C220`) | las tablas de elección con un jugador en vehículo (`04DA38`: `2C1556`/`2C15A4`; agachado `04DC5C`: `2C168E`/`2C16DC`) dan la orden 6 en 3 de 8 casillas; `run4` va a pie (piloto). En el port era una parada explícita (`04BAB2`): la lanzadera se borraba con su árbol y el `+9E = 7F` que le había puesto al cuerpo no se limpiaba nunca, así que `04DA38` no volvía a elegir: el jefe se quedaba quieto. Captura nueva `capture_m3_boss.sh ridestates/ride` (plan `boss` de `capture_fullgame.lua`: sube al SV-001 `093C9A`, avanza disparando y espera en x 96 sin disparar; estado `prog_140352`): MAME usa la orden 6 al menos 18 veces en 140352..152000 | **corregido** (`work-m3order6`): `04BAB2` (lado del jugador por `01B04A`/`01B12E` desde el punto de la pose: `+9C` = 0 u 8), `04BB88/04BBAA` (abre), `04BBFC/04BC68` (espera `2C230E`/`2C2340`, barridos `2C243A`/`2C2454`), `04BCA2..04BD32` (uno de 8 patrones `2C25E2`/`2C2602` con `0029F2(8)`; programa `245DE6` = `CALL 04E140` → misil `04C220`, paleta 202, programa `246794`, velocidad `2C22A6`/`2C22C0`) y `04BF66` (una andanada de la orden 2). Familia `m3boss6` (140589..141879: 99 nodos, 8898 actualizaciones, 16 misiles `04C220`) igual a MAME; sprites ya en el export (volcados 141420..141690: 0 píxeles distintos). `04C952` (bola recta) y `04E1E6` (piezas `04C2F6/04C30A`) no tienen ninguna referencia en la ROM (código muerto): no se portan. Con el código anterior la familia se para en 141431 (`4BA94 unsupported 4BAB2`). ares (`mslugx-m3-reference.z64`, piloto hasta 4914 y luego mando: sube al SV-001, llega a 6496 montado y espera a la izquierda 150 s sin disparar): el jefe ataca todo el rato (misiles de la orden 6 y de la 2, lanzallamas, mortero), sin paradas del jefe; a pie, lejos y pegado a él, también. El cartucho normal no llega solo al jefe (el mando automático pierde las vidas en 2888 con la barra de vida); el código del jefe es el mismo. De paso: el gancho de referencia `MSX_REFERENCE_STAGE_EXIT` terminaba la M3 al llegar al borde derecho de la arena con el jefe recién nacido (`gameplay.c`: la M3 queda fuera, como la M2). El aterrizaje de la bola (`0143E4`) quedó comprobado en `work-m3land` (`m3boss1` hasta 145599 con el anillo `10C7B6..` en la extracción: el banco no tenía el suelo; ver `JEFE_MISION3.md`); el tramo de `m3boss6` desde 141895 sigue sin rehacer (falta `prog_140352`) |
| G16 | Granada `03C306` de la muerte de `04115A` | sin traza en `run4`; ares (cartucho normal) `native draw missing last=162D90..162DCC` | **sprites corregidos** (`work-m3lever`): raíz `23F2A0` en `export_rest_sprites.py` (18 descriptores de la ROM, sin captura que validar; paleta 20E ya estaba). La conducta sigue sin traza |
| G17 | Superposición `10E8C6` (`0A6C8C`, 34 bloques de submapa en 3944) y 41 bloques del tren con otra paleta | `render_m3_sections.py`: 98,84 % / 99,49 % de píxeles en las secciones 4 / 3 | abierto (poco visible) |
| G19 | En 1520 el port mostraba un bosque espeso detrás del tren donde MAME ve el cañón con un árbol | el mapa del tren `0B8566` tiene tramos de bosque; 209 celdas observadas de la capa 1 no eran el bloque de ROM de su x (vistas a través de un paso del origen `+32`) | **corregido**: `pack_mission.py fill_loop_layer` les pone el bloque de ROM con sus colores votados (797 trozos); ares (piloto) en 1520: cañón y árbol como MAME |
| G20 | **Tablas de la M3 sin sitio** cuando la caché de escenas empieza pronto (`begun early`, cinemáticos de CONFIRM/ENTRY; el menú de pruebas nunca la empieza pronto). Causa (`work-g20`, log `MSX cache block`): no hay ranuras fijadas; las 212 ranuras son 12 bloques del heap (53, 6, 6, 37, 9, 4, 2, 1, 47, 23, 23, 1) puestos en los huecos que dejan los búferes de las películas, y la reserva exigía un solo bloque de 55 ranuras (892 272 bytes) | ares, cartucho release con el menú sin saltar CONFIRM/ENTRY (copia de diagnóstico, no en git): 2/2 `begun early`, racha 53, `55 contiguous slots not found` | **corregido (`work-g20`)**: `reserve_slots` (scene_assets.c) prueba primero una racha (igual que antes); si no hay, toma varias rachas, la más larga primero, cada una de al menos la tabla más grande (`largest`, 197 104 B en la M3), y `take` pasa a la siguiente racha cuando una tabla no cabe: n rachas piden need+(n-1)·largest. Camino temprano: `67 slots in 2 runs`, M3 cargada y jugable (3/3); menú normal 6/6 en una racha de 106; caché sin cambios (212, suelo 208) |
| G21 | **Marco gordo dispara sin balas** (MiSTer, ROM 44: hace la animación y no sale nada) y revisión de todo su disparo | captura MAME nueva `tools/capture_fat_weapons.lua` (`run_capture_fat_weapons.sh`, desde `at_122100`, armas 0, A, 3, 4, 2, 6, C, D, E dadas por RAM, patrón fijo: de pie, rápido, arriba, agachado, abajo y recto en el aire, andando, a la izquierda, granada; contador `10C728` fijo y al final suelto) contra el port | **corregido**: (1) la bala gorda `01B62C` (programa `22C0A6`, descriptores `1B13BE/1B13C8`) no estaba en el export de la M3: se creaba y no se dibujaba (la causa del informe); `export_rest_sprites.py` añade `22C0A6/22C000/22C4C6/22C040`, las llamas gordas `2A6B8A..2A6D04` y las paletas `874/877/871` (más `86C/870/875`, antes `missing`) de los volcados de la captura; paquete de 818 trozos. (2) `+72/+74` gordos (`259D4C`/`259E64` (0,16), andar `259D68` 15/17/17/17/17/16, velocidad 1C1): `marco_legs.c` (`marco_legs_set_fat`), antes los del flaco (1 px de pie, hasta 8 en el aire). (3) Cadencia: un disparo cada 5 actualizaciones (10 ticks; `+76` en la cuarta, 0C3D9A lo ve en la quinta), no 12. (4) Destello `0C58D2` en el punto del proyectil (0C43CC resta `+72/+74` y 01ACF2 suma `+3C/+40`, que llevan el `+74`). (5) La H gorda es la H entera (`marco_hgun.c` `fat`: ráfagas de 4 / 6 hacia abajo, `+76` en 7/11, barridos `0C4610` (`marco_muzzle_sweep_fat`), abanico ×3), antes una bala por pulsación. (6) Nativas `0646C8/0646E0` (S, `+72` = 2/3, paleta 86C), `0A7226/0A719C` (F, paleta 877, `+7B` = FF), `0DE4FE` (E, paleta 870, `+7D` = 1) y el rayo gordo (`25A0E0..25D2B2`, 9 actualizaciones); antes paradas sin disparo. (7) Granada/molotov/bomba gordas: el segundo constructor solo agachado (`+90` = FF) y en (`+72`, 4 + `+74`) / (0,16). (8) `0C1D82` (vuelta a flaco, 126505): la retención de 19 actualizaciones en vez de parada. Pruebas: `test_fat_muzzle` (249 nacimientos y 124 pasos de andar iguales, `data/fatmuzzle_fixture.json`), `test_gameplay` (cada arma crea su proyectil, 8 tiros en 80 ticks, vuelta con retención). Sonidos `10F7/10F4/10F5/10F3/10F6/116A/10F8` iguales. Siguen abiertos: `0A7960` (arma 0D gorda) y `0DEA44` (arma 0F gorda; la 0F flaca tampoco es nativa), paradas explícitas; las poses gordas de la H, el rayo, el cuchillo y `25CEB0` no están en el catálogo (se ve `25A8F4`, la del disparo gordo); el cuchillo gordo (`25A632`) sin captura con enemigo cerca; `0C5064` no es de la forma gorda (sale al dejar la forma 1, `0C359A`). Observación sin comprobar: con la misma cuenta las cadencias flacas de `fire_kind` darían 2 ticks más |
| G22 | **Plataforma de la palanca invisible** (MiSTer, ROM 44; B63): al romper la palanca `038B06` sale una plataforma para subir al techo del vagón; en el port no se veía (Marco sí subía) | no era un programa ni una paleta: los estampadores `0B90BC/0B90FA/0B9106/0B9112` (y `0B911E/0B9140` de las cajas) copian con `0C70FC` cuadros 4×4 del submapa `0B85EA` (bloques del mapa, parte de teselas `3301E8`) al mismo destino: `0B9040` (0,4)(4,0)(4,4)(8,0), `0B9064` (C,4)(10,0)(10,4)(14,0), `0B9090` (0,8)(4,8)(8,8)(10,8)(C,8). `map.json` toma la primera observación: los bloques de antes (`run4` vio el parche en 1 de 11 observaciones). La M3 no tenía `patches.json`; la clave `g_map_patch` (submapa + D0/D1 & 15) no distinguía el destino (dos puertas por programa) y los 26 cuadros pasaban de los 16 registros | **corregido** (`work-m3lever`): `extract_map_patches.py` saca los 6 cuadros finales (posición comprobada con `map.json`: 16/16, 16/16, 13, 8, 13, 8 bloques observados); `pack_mission.py` y `g_map_patch` llevan la columna destino D4 en los bits 21..31 de la palabra de capa (`scene_renderer.c`: una entrada sin destino, la M2 anterior, vale para cualquiera) y un registro por destino (D4, D5): el cuadro nuevo sustituye al anterior, como sus palabras en SCB1. Diagnóstico nuevo `MSX map patches drawn mask=…`. ares (cartucho normal, menú de pruebas, `captures-m3lever/run5`): caja `0001`, puertas `0005`, `0015`; la plataforma se ve y Marco sube por ella al techo. Barrido de la misma pasada: `actorstate` de `run4` en toda la M3 (22 837 pares rutina/programa/paleta) contra los export: 0 descriptores y 0 paletas que falten; en ares solo G12, G16 y G23 |
| G23 | Paleta `133` del objeto `287BC4` (`native draw missing last=18749A palette_id=133`, 28 dibujos en un recorrido) | ninguna captura la tiene (también en la M1, MISION1_ACTORES.md) | abierto: común a todas las misiones |
| G18 | HUD: `continue item 019798` (parada al acabar las vidas) | ares release | abierto (común a todas las misiones) |
| G24 | **Fondo inestable detrás del tren** (MiSTer; B65): el cañón y los árboles de la capa del tren `10D7B6` saltaban de golpe (bosque ↔ cañón) varias veces por recorrido | VRAM de `run4` (SCB1 de los 32 sprites de `10D7B6`, ranura `+2C` + 1 + k, contra las columnas del mapa `0B8566`): cuando el origen `+32` sube 0x20 (`0B8DD0`), las columnas ya cargadas **no se recargan**; solo las que entran por el borde (`0C840C` → `0C748E/0C74C4` → `0C6BDE/0C714E`, columna de mapa = columna local − `+32` del momento de la carga) toman el origen nuevo, así que durante ~600 fotogramas el anillo mezcla dos orígenes (p. ej. 120480: 6 sprites con el origen 64 y 6 con el 96). El port dibujaba toda la capa con el origen actual (`−x − 16·+32`): al subir el origen cambiaba de golpe todo lo visible (440 de las 1390 columnas comprobables de `run4` distintas). ares antes (cartucho normal, menú de pruebas, cámara 1280..1346): el bosque pasa a cañón en un fotograma. El cielo usaba «x del tren / 4 − 240» (ajuste de `run4`) | **corregido** (`work-m3scroll`): `neo_train.c` lleva el anillo (`+36`, `ring[32]`: columna de mapa por ranura; fuera del mapa la ranura conserva lo que tenía, `+82` bit 3 = 0 por la op 0x1C con `+D` = 0) y la capa del cielo `10E83E` (`sky_x` por `0C6F9E` con su factor `+26` de la op 0x1C, 0x40 desde 0x140, y `10E94E` con el bit 0 de `10E958`; `neo_layer_step`). `scene_render_set_layer_ring`: la capa 1 se dibuja columna a columna (columna k en `16·(+36 + k) + +84`, `+84` = palabra de `+14`, `0C75B2/0C75EE`), cada una con su columna de mapa, en la ranura `(+36 + k) & 31` del anillo pre-renderizado (la del original: una ranura solo cambia fuera de la pantalla); por la ruta de piezas, tramos de un mismo desplazamiento con su tijera. El cielo: `x` del cuadro = `(x − +84) mod 512` del cielo. `pack_mission.py --loose-layers` ya no ajusta: las reglas salen de la lista de cámara (`rom_layer_rules`: ops 0x1C y 0x18; factor 0x100 = la raíz exacta); el único cambio del paquete es el fin del cielo por regla, 321 → 320 (progreso 0x140). `test_neo_train` (fixture rehecho desde `run4`): 14252 actualizaciones con `+14/+32/+36` exactos, **1390 columnas de 117 volcados de VRAM iguales a MAME** y el `+14` del cielo exacto. ares (cartucho normal, menú de pruebas): el cañón entra por la derecha columna a columna, sin saltos, y el cielo sigue sin hueco al pasar el progreso 0x140; `sync=0` en el anillo. `make check` y `test_m1m2_ares.py` (`mslugx-m1end.z64`) pasan: la M1/M2 no usan el anillo ni el cielo |

Registros del guion por sección (`--port`): release (hasta 1520) sección 1
28/28 en el mismo orden; piloto: 1 28/28, 2 106/112 (difieren solo ramas
`si`/azar), 3 1/1, 4 48/55 (no llegó al jefe en 330 s). Sin registros de
constructores sin portar saltados; `camera events unsupported` 0.

**Tren del minuto 8:30**: en el cartucho normal (`--hop`, sin piloto ni
invulnerable) arranca: el grupo 0..7 y el soldado del escudo `09AE2C`
mueren, la cámara se suelta en 312 y el tren pasa a modo 1 en 320 (`train
on=1 mode=1 own=1`). No hay diferencia en ares: falta verlo en MiSTer.

## Hipótesis

| Hipótesis | Evidencia | Estado |
| --- | --- | --- |
| H1: el anillo pre-renderizado deja columnas sin dibujar en el hardware | Sin `m3s1.bgp` el camino de piezas da el mismo negro | descartada |
| H2: la sección de la foto usa capas o reglas que el pack no tiene | La foto es la cámara 0, con sus capas en el pack; solo las paletas eran negras | descartada |
| H3: falta de ranuras de caché en ese tramo | Tablas y mapa cargados (774 KB en 49 ranuras, 31 trozos); el dibujo existe, con paletas negras | descartada |
| H4: paletas de las celdas tomadas de un fotograma del fundido | 583 celdas de x 0..304 con paletas `0001`; con `--skip-fade` el fondo es el de MAME | **confirmada, corregida (B60)** |

## Siguiente experimento (uno solo)

G24 (fondo detrás del tren): confirmarlo en MiSTer con `mslugx-explorer.z64` (menú de pruebas, MISSION 3, tramo de los vagones 320..3392: el bosque y el cañón entran por la derecha sin saltos).

G15 (el jefe con el jugador montado en el SV-001), G22 (plataforma de la palanca) y G20: falta confirmarlos en MiSTer (disparar a las palancas de los vagones 7-24 y subir; la M3 tras arrancar con los cinemáticos completos). Luego G10, G11, G13, G14 y G23 por
visibilidad. Verificado en `work-m3g7`: ares (referencia, piloto, 330 s, sin
`begun early`) hasta el puente 2888 sin rutinas no soportadas (solo los 16
externos `02A448` de G11) y 1235 dibujos sin paleta `0A7` (G12) o sin los
sprites de `03C306` (`23F2A0`, G16).
Estados MAME de `run4` guardados: `/home/user/captures-m3g7/states/mslugx/at_120950|122100|122200.sta`
(con `MSX_FULL_STATES=/home/user/captures-m3g7/states`); los guiones de captura de esta sesión
están al lado (`run_f.sh`: G6, `run_c.sh`: molotov).

## Archivos tocados por la M3

`src/neo_m3props.c`, `neo_m3enemy.c`, `neo_m3mid.c`, `neo_m3veh.c`,
`neo_m3boss.c`, `neo_train.c`, `scene_prerender.c`, `scene_assets.h`,
`tools/pack_mission.py`, `tools/prerender_stage.py`; B60: `filesystem/mission3/s1/map.json` y
`romfs/mission3.pack` (privados); B57: `src/neo_soldier_ai.c/.h`, `src/neo_world.c/.h`,
`tools/test_neo_m3push.c`, `Makefile`; `work-m3gaps`: `src/neo_camera.c` (G1),
`src/neo_world.c` (G5), `src/neo_train.c/.h` (G2, disparo),
`tools/test_m3_camera_bridge.c`, `tools/test_neo_train.c`,
`tools/export_rest_sprites.py` (paletas), `tools/export_m3_layers.py` y
`tools/render_m3_sections.py` (cuadros del cielo); privados:
`filesystem/mission3/rest-native.json`, `filesystem/mission3/s1/layers_rom.json`,
`romfs/mission3.pack`.; `work-m3g7`: `src/neo_bullet.c` (G7), `src/neo_gate.c` (G6, `neo_gate_splash`),
`src/neo_m3enemy.c`, `src/neo_pweapon.c`, `src/gameplay.c`, `src/neo_gactor.h`
(`palette_slot`), `src/scene_assets.h` (832 trozos), `tools/test_neo_family.c`,
`tools/capture_fullgame.lua` (`MSX_FULL_PLAN=dive:<x>`, `MSX_FULL_GIVE_BOMB`,
`MSX_FULL_GIVE_FORM`), `tools/extract_soldier_programs.py`,
`tools/export_rest_sprites.py`, `data/m3bomb|m3molotov|m3splash_fixture.json.gz`;
privados: `filesystem/mission3/rest-native.json`, `romfs/mission3.pack`; `work-g20`: `src/scene_assets.c` (G20, reserva de las tablas en varias rachas); `work-fatfire` (G21): `src/gameplay.c`,
`src/marco_legs.c/.h`, `src/marco_hgun.c/.h`, `src/neo_pweapon.c`, `tools/capture_fat_weapons.lua`,
`tools/run_capture_fat_weapons.sh`, `tools/extract_fat_muzzle_fixture.py`, `tools/test_fat_muzzle.c`,
`tools/test_gameplay.c`, `tools/export_rest_sprites.py`, `tools/extract_soldier_programs.py`,
`data/soldier_programs.json`, `data/fatmuzzle_fixture.json`, `Makefile`; privados:
`filesystem/mission3/rest-native.json`, `romfs/mission3.pack`.; `work-m3lever` (G22, G9, G12, G16): `src/neo_world.c/.h` (registro de parches por destino, llamada
`0B91D0`), `src/scene_renderer.c`, `src/neo_m3props.c`, `tools/extract_map_patches.py`, `tools/pack_mission.py`,
`tools/extract_soldier_programs.py`, `tools/export_rest_sprites.py`, `tools/capture_fullgame_blood.lua`,
`data/soldier_programs.json`; privados: `filesystem/mission3/s1/patches.json`, `filesystem/mission3/rest-native.json`,
`romfs/mission3.pack`.; `work-m3order6` (G15, B64): `src/neo_m3boss.c`, `src/gameplay.c` (gancho de referencia), `data/region_rom.json`, `data/soldier_programs.json`,
`data/m3boss6_fixture.json.gz`, `Makefile`, `tools/capture_fullgame.lua` (plan `boss`, `MSX_FULL_SAVE_PROGRESS`), `tools/capture_m3_boss.sh` (`ridestates`, `ride`),
`tools/extract_soldier_programs.py`, `tools/export_rest_sprites.py`; privado: `romfs/mission3.pack` (bloque MSXD).; `work-m3scroll` (G24, B65): `src/neo_train.c/.h` (anillo, cielo, `neo_layer_step`), `src/neo_camera.c` (op 0x1C con su factor), `src/gameplay.c`,
`src/actor.c`, `src/scene_renderer.c/.h` (`scene_render_set_layer_ring`), `src/scene_prerender.c/.h` (ranura del original), `tools/pack_mission.py` (`rom_layer_rules`),
`tools/extract_train_fixture.py`, `tools/compile_train_fixture.py`, `tools/test_neo_train.c`, `data/train_fixture.json.gz`; privado: `romfs/mission3.pack` (fin del cielo por regla 320).
