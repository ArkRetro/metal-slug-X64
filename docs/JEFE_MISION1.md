# Jefe de la Misión 1: port nativo, fixtures y gráficos

El jefe de la Misión 1 (registro de guion `0x2D70FC`, constructor
`0x0D5976`, X=3720, Y=64, grupo 6) se ejecuta en C nativo en
`src/neo_boss.{c,h}`: cuerpo, ocho partes, misiles, obús de cañón con su
estela de humo, plataformas de objeto, recepción de daño, cambio de fase,
muerte y la señal de misión completada. Se comprueba **en bucle cerrado**
contra dos combates grabados en MAME y sus gráficos están en el pack de
escena, validados píxel a píxel contra la VRAM. El módulo es independiente:
**no está conectado todavía** al mundo (`neo_world`), al guion ni a la cámara
(ver «Integración»). Resumen numérico en
[jefe-mision1-validation.json](jefe-mision1-validation.json).

## Captura

`tools/capture_mission_rest.lua` con `MSX_REST_BOSS_FROM=<fotograma>` añade
una traza de la familia del jefe (`boss-trace.txt`) con *taps* de
lectura/escritura sobre la RAM de trabajo (no cambian datos):

- `U`/`V`: los 0xB0 bytes del nodo antes y después de cada actualización
  (despacho `0x004246` y lectura de `+8` en `0x00424A`);
- `R`/`W`: cada acceso a RAM durante la actualización con el PC de la
  instrucción original;
- `X`/`A`: escrituras de otros actores en nodos del jefe (impactos de balas,
  `0x004892..0x0049E0`) con A0 (fila de ataque), D3 (daño/clase) y los bytes
  del atacante;
- `B`: nodos creados por la familia (`0x0040EC`).

La familia es el nodo que despacha `0x0D5976` más todo nodo creado mientras un
nodo de la familia es el creador. Dos detalles medidos: la llamada de fin de
fotograma de MAME puede caer entre `0x0040EC` y `0x00410E` de una asignación
(por eso no se poda la familia al final del fotograma), y una interrupción
apila su marco con el PC de la instrucción interrumpida (el extractor ignora
la pila `0x10F100..0x10F3FF`).

Partidas (≈4 min cada una, `/home/user/captures-boss`): las partidas sin
depurador reproducen la variante de `run9` (la de `run8` apareció 1 vez en 15
partidas y no se repitió en las 7 nuevas); la partida con `-debug` es otra
partida estable distinta desde el fotograma 2333 y da un segundo combate
completo. Fixtures:

| Fixture | Captura | Actualizaciones nativas | Nodos | Impactos | Rutinas |
| --- | --- | ---: | ---: | ---: | ---: |
| `data/boss_fight_a.json.gz` | `runJ` (sin depurador, variante `run9`) | 32406 | 1989 | 200 | 68 |
| `data/boss_fight_b.json.gz` | `runDbg2` (con `-debug`) | 35054 | 2181 | 299 | 68 |

## Del registro a los fixtures

`tools/extract_boss_fight.py` convierte la traza en casos: por cada
actualización de un nodo del jefe, la imagen de campos tras la actualización
(y la previa cuando difiere de la anterior), los impactos recibidos desde la
actualización anterior y la secuencia ordenada de resultados de los servicios
del motor, reconocidos por el PC original (`0x0029D2` RNG, `0x0040C2`
asignador, `0x009F78` paleta, `0x01ABEA/0x01AC00/0x01AD12..` objetos de escena,
`0x014492..0x014502` barrido del núcleo de colisión, `0x014726` suelo,
`0x0140C6/0x0140F2` plataformas, `0x013F4E`, `0x004C24/0x004DA4/0x004BE2`
registro de impacto, `0x0049E8/0x004B96` ataque, `0x019504` límites
(puntero de RAM `0x10C2F0` a un bloque de ROM), `0x00A078/0x00A080` entradas
de la lista de sprites, `0x018F92/0x018F40` fin de misión). Los nodos nuevos
heredan los bytes del ocupante anterior (`0x0040BC` no borra): el fixture
lleva esos bytes, tomados de lo que el original leyó antes de escribirlos.

`tools/compile_boss_fixture.py` lo compila a `build/boss_fixture.h` (host) y a
`build/boss-romfs/boss-*.bin` (cartucho). `tools/extract_boss_programs.py`
convierte los 91 programas de animación (`data/boss_programs.json`) y
`tools/extract_boss_tables.py` los datos de ROM que lee el jefe
(`data/boss_tables.json`: tablas de dificultad `0x2CAxxx`, trayectorias de
misiles `0x0E95E2..`, filas de velocidad del obús, rectángulos, seno `0x0029FC`).

## Módulo nativo

Cada nodo guarda su imagen de campos `+10..+AF` en orden big-endian (el
original reutiliza bytes con varios anchos y los nodos heredan bytes); los
punteros son etiquetas (`NEO_BOSS_TAG`, jugadores `0x1014C4/0x101574`, objetos
de escena, etiquetas de ROM). El programa de animación es nativo
(`neo_animation`, ampliado para `SELECT_FIELD_POSE` por la interfaz de actor:
selecciones por `+83`/`+A6`). Cada rutina lleva su etiqueta de ROM y sigue el
orden de instrucciones original.

| Constructor | Pieza | Rutinas | Servicios principales |
| --- | --- | --- | --- |
| `0x0D5976` | cuerpo (5000 PV ajustados por `0x0260C8`, fase 2 en `+86`) | `0D5A92..0D5EDA` | plataforma `0E93B2`→`0E94AC`, colisión, ataque `0E90D4/0E90C4` |
| `0x0D600C` | blindaje frontal (100 PV) | `0D605E/0D60B4` | impacto `0E9144` |
| `0x0D6464` / `0x0D61F2` / `0x0D6130` | torreta: soporte, base, cañón que apunta (`0D60C8`) | `0D64D8`, `0D6254..0D63C4`, `0D6168` | jugador más cercano `0x01AF9A` |
| `0x0D6500` | lanzamisiles (`0D726E`) | `0D6568` | tablas `2CAB9E/2CAC00/2CAC96` |
| `0x0D6A84` | misil con trayectoria (`0x021288`) | `0D6AC6..0D6C66` | ataque, límites `0E918C` |
| `0x0D680A` | cañón principal (`0D739C` crea fogonazo y obús) | `0D6882..0D697A` | tablas `2CAD58..2CAEF0` |
| `0x0D6FFC` / `0x0D6F90` | fogonazo y obús (`0x02A260`, `0x02A2AC/02A2FE/02A244`) | `0D7020`, `02A2AC..` | colisión, ataque `02A794` |
| `0x02A3B4` | estela del obús | `02A3CE` | — |
| `0x0D65EE` / `0x0D6D30` | escotilla (plataforma `0E95A6`, suelta 3 prisioneros) y respiradero | `0D6634..0D67E6`, `0D6D6A..0D6EBE` | ataque `0E917C` |
| `0x0D69DC` | tapa | `0D6A16..0D6A54` | — |

Servicios (`neo_boss_services_t`): RNG `0x0029C6`; asignador `0x0040BC`
(reserva y bytes heredados); paletas `0x009EA8/0x009F88`; sonido `0x0120F8`;
objetos de escena para la proyección; barrido `0x0143E4` y suelo `0x0146E8`;
plataformas de objeto `0x0140C0/0x013F1E`; registro de impacto `0x004C24`
(con la condición «matar todo» `0x004BAC`); ataque `0x0049E8`; límites de
escena (`0x019504`); entradas de la lista de sprites; actores genéricos
(`NEO_BOSS_EFFECT_*`); fin de misión `0x018F92` (paso 1) y `0x018F40` (paso 2).
`neo_boss_env_t` lleva los globales que lee (`10C606`, `10C33C`, `10C33A`,
palabras proyectadas de los jugadores, `10C300`, `10FD8B`, `100088`, `10000E`).

Daño: `neo_boss_receive` aplica `0x004892..0x0049E0` (tipo de ataque, destello
`+17`, dirección `+5B`, punto `+5C/+5D`, vida `+4E`, `+5F` bit 7, `+5E`) desde
la fase estrecha existente del atacante. Fases: el cuerpo pasa a la fase 2
cuando `+4E ≤ +86` (`0D5DF0`); la escotilla destruida pone `+79=5` en el
cuerpo; con `+5F` bit 7 y el temporizador `+9E` agotado empieza la muerte
(`0D5E5C`: misión completada, `+79=6`, dos programas, tres explosiones,
escombros y borrado).

El chequeo de la protección de la placa original (`0D5F6C`, `0D63D4`: RAM
`10F00A/10F00C` y `0x2FFFE0`) solo decide entre el borrado y un cuelgue; no se
porta y sus dos contadores de RAM no se comparan.

## Verificación

`tools/test_neo_boss.c` (en `make check`, objetivo `check-boss`) ejecuta los
dos combates en bucle cerrado: el estado nativo nunca se recarga. Antes de
cada actualización la imagen nativa debe ser la grabada (tras aplicar los
impactos del intervalo con `neo_boss_receive`); después, el resultado, la
siguiente rutina y la imagen deben coincidir; cada petición a un servicio debe
ser la siguiente de la secuencia grabada, y cada entrada de la lista de
sprites (descriptor, x, y, paleta/atributos, z) debe ser la original.

**Resultado:** 67460 actualizaciones (32406 + 35054), 0 fallos, 499 impactos,
63896 entradas de sprite iguales, 68 rutinas por combate, 81 traspasos a
código genérico, 3489 borrados y 26 semillas de actores genéricos iguales. Tolerancias documentadas: punteros a nodos ya
borrados (`+8E` de la torreta tras salir su cañón) se comparan con cualquiera
de los ocupantes previos de esa ranura nativa; `+00..+0F` (enlaces de lista)
no forman parte de la imagen.

`boss-regression.z64` es el mismo test compilado para MIPS con los combates en
DFS; `tools/test_boss_ares.py` lo ejecuta en ares 146 (ver
[jefe-mision1-validation.json](jefe-mision1-validation.json) para el
resultado).

## Cobertura (inventario de 86 rutinas)

- **56 de 86 nativas y verificadas**: 50 del jefe (`0D5A92..0D7020`) y 6 del
  obús y su estela (`02A18A`, `02A244`, `02A2AC`, `02A2FE`, `02A3B4`,
  `02A3CE`). Además se verifican los 12 constructores, que el inventario no
  lista.
- **30 de 86 son actores genéricos** que el jefe crea o en los que convierte
  un nodo; pasan al mundo por `services.effect` con su imagen y no se portan
  aquí: explosiones/escombros/humo `01C9E6`, `01D1B0`, `01D7B8`, `01D8A4`,
  `01D8D4`, `01D8E4`, `01D908`, `01D934`, `01D93C`, `01D944`, `01D958`,
  `01D9C6`; desvanecido de la torreta al acabar la misión `08EEBC`, `08EEEA`,
  `08F1E0`; los tres prisioneros `099DCC` (`099E48..09A5EE`, 15 rutinas);
  `00A18A`.
- Bloques traducidos desde la ROM que ningún combate ejecutó (no verificados):
  disparo de la torreta `0D6292..0D637E` (termina en `0x08FA1A`, que crea la
  bala genérica `0x08EFFA`: **falla explícitamente**, `NEO_BOSS_UNSUPPORTED`),
  reapertura de la escotilla `0D6710..0D6756` y reinicio del cañón por `+A1`,
  fuego del respiradero `0D6DC0..0D6E86`, golpe con armadura en fase 2
  (`0D5DE0`), muerte del blindaje frontal por daño (`0D6086`), fin de
  trayectoria del misil (`0D6BB6`), estallido del obús en el suelo
  (`02A32A..`, falla explícitamente) y llamada de trayectoria `op 8`
  (`0D6C0E`, guiado; falla explícitamente). `0D6C74` y `0D6ED6` no se
  referencian desde el jefe.

## Gráficos

`tools/export_boss_sprites.py` decodifica los 264 descriptores dibujados por
los programas del jefe (3430 teselas únicas de 16×16; `0x1C3F06` es
transparente) desde la P-ROM y la C-ROM y 13 paletas lógicas (`0x2EC..0x2EF`,
`0x319..0x31B`, `0x347`, `0x223`, `0x357..0x359`, `0x393`) desde los volcados
(tabla del asignador `0x1068EA`), y los valida contra la VRAM: en cada volcado,
las entradas de la lista de sprites de la pasada lógica cuya lista ya se
había confirmado (la anterior o la previa: la pasada puede acabar tras el
vblank), rasterizadas con los píxeles exportados, deben dar la misma ranura de
paleta y el mismo índice que el sprite reconstruido de la VRAM. El original
descarta las columnas de 16 px cuyo borde izquierdo queda fuera de −8..311
(`0x00A17A..0x00A18E`); el exportador y el dibujo nativo lo reproducen.

| Combate | Volcados | Píxeles iguales | Distintos |
| --- | ---: | ---: | ---: |
| a | 102 | 1700890 | 0 |
| b | 116 | 1786817 | 190 (un volcado, 13410, entradas de obús/estela ausentes en la VRAM; sin explicar) |

Pack (`tools/pack_scene.py`): los descriptores del jefe entran en el banco
`SCENE_NATIVE` en streaming (no fijado), en orden de programa, partidos en
partes ≤64×64 CI4: **508 páginas de 2048 bytes, 65 chunks de 16 KiB** (pack
completo: 1300 páginas, 163 chunks, 1326912 bytes empaquetados). Cada parte es
una carga de TMEM de 2048 bytes como máximo (comprobado). Conjunto de trabajo
por pasada lógica en los combates grabados (`tools/check_boss_budget.py`, en
`check-boss`): **peor 16–17 chunks y 31–33 páginas**, 56 chunks distintos en
todo el combate; ida y vuelta pack→píxeles exacta para los 264 descriptores.
Con la caché de 8 MiB (192 ranuras) cabe el pack entero; con 4 MiB (32
ranuras) el jefe se transmite bajo demanda junto al mapa de la arena.

Dibujo: `src/boss_renderer.{c,h}` (`boss_render_draw`) usa el renderizador
existente (`scene_render_image`): partes del banco nativo, paleta por recurso
lógico, espejo un píxel a la izquierda y el recorte de columnas por *scissor*.

## Integración

Hecha en `neo_world` (ver [MISION1_COMPLETA.md](MISION1_COMPLETA.md)); la
lista siguiente es el plan original, con las diferencias documentadas allí.


1. **Guion**: el registro op 7 `0x2D70FC` (constructor `0x0D5976` de la tabla
   `0x0197DC`) debe llamar a `neo_boss_spawn` con la imagen que deja
   `0x01A136/0x01ABB6` (palabras del registro en `+6E..`: casa X 3712/3718,
   posición 3720,64, grupo 6) en la posición de lista del original (tras el
   actor del guion) y contar el grupo 6.
2. **Lista de actores**: `neo_boss_step` en orden de lista intercalado con el
   resto de actores (los hijos nuevos se ejecutan en la misma pasada, como en
   `0x0040BC`); los actores genéricos de `services.effect` (explosiones
   `01C706/01C71E`, escombros `01D764`, humo `01D86C`, prisioneros `099DCC`,
   nodos convertidos a `01C71E/01C706/08EEBC` y sus descendientes) los crea y
   continúa el mundo. El asignador debe respetar la reserva (`d0`) y
   proporcionar los bytes heredados.
3. **Colisión**: barrido y suelo con la colisión del resto de la misión
   (`filesystem/mission1/rest/collision.json`) y el objeto de capa
   `0x10D7B6` como padre del cuerpo; las plataformas de objeto `0E93B2`,
   `0E94AC`, `0E95A6` deben entrar en el núcleo como plataformas dinámicas
   (el jugador se sube al jefe).
4. **Combate**: balas y granadas del jugador → fase estrecha contra los
   rectángulos `+50` del jefe → `neo_boss_receive`; `services.attack` aplica al
   jugador las filas `0E90D4/0E90C4/0E917C/0E916C/0E915C` y `02A794`;
   `services.hurtbox` puede devolver «matar todo» tras el fin de misión.
5. **Cámara**: la parada 3376 (operandos 56,·,144,·) fija la arena; la cámara
   sigue al jugador entre 3432 y 3520; el jefe aparece en el progreso
   3302–3308. Los límites de `0x019504` vienen del bloque de ROM al que apunta
   `0x10C2F0` (`0x017FC4` en los combates).
6. **Fin de misión**: `services.mission` paso 1 (`0x018F92`: marca `10C300`,
   música, «matar todo») y paso 2 (`0x018F40`); `neo_boss_t.mission_complete`.
7. **Dibujo**: `services.draw` → `boss_render_draw` con el recurso de la
   ranura de paleta asignada; las entradas se ordenan por `z` (`+1E`) con el
   resto de sprites, como la lista original.

## Reproducir

```sh
MSX_REST_BOSS_FROM=10150 tools/run_capture_rest.sh /ruta/runJ
MSX_REST_BOSS_FROM=9700 MSX_REST_DEBUG=1 tools/run_capture_rest.sh /ruta/runDbg2
python3 tools/extract_boss_programs.py /ruta/mslugx.zip
python3 tools/extract_boss_tables.py /ruta/mslugx.zip
python3 tools/extract_boss_fight.py /ruta/runJ/boss-trace.txt /ruta/mslugx.zip --name a
python3 tools/extract_boss_fight.py /ruta/runDbg2/boss-trace.txt /ruta/mslugx.zip --name b
cd tools && python3 export_boss_sprites.py /ruta/mslugx.zip /ruta/runJ --frames 10230:13770:30
python3 export_boss_sprites.py /ruta/mslugx.zip /ruta/runDbg2 --fixture ../data/boss_fight_b.json.gz \
  --frames 9720:13700:30 --report ../docs/boss-sprites-validation-b.json --no-export && cd ..
make check && make -j4 && make boss-regression.z64
python3 tools/test_boss_ares.py boss-regression.z64 --output /ruta/ares-boss
```

`filesystem/boss-native.json` (generado, no versionado como el resto de
`filesystem/`) lo necesita `pack_scene.py`.

## Pendiente (prueba 5 en MiSTer)

- **Marco cae sobre el jefe al reaparecer: corregido** (sección siguiente).
- **Explosiones del jefe sin color: identificado, no corregido.** Las
  manchas planas azules con borde dorado/rojo/cian y los círculos grandes
  planos (capturas del port en ares, combate del jefe) son el programa
  `0x22CF2C` (descriptores `0x1300AE..0x1306CE`) de los actores genéricos
  que crea el jefe, dibujado con los recursos de paleta `0x357`, `0x358` y
  `0x359`. El paquete de escena guarda la paleta más frecuente de las
  capturas (`tools/export_boss_sprites.py`): plumas 1..6 una rampa dorada,
  roja o cian y plumas 7..15 = `0x100F` (azul puro). En el original esas
  ranuras las anima el asignador de paletas: su entrada en `0x1068EA`
  (`0574 0574 0844 0844 nnnn 0357 0357`, `runDbg2` fotogramas 10950,
  13380..13440) lleva un contador `+8` (1, 0x0B, 2) y la paleta pasa de una
  de fuego (`7FFE 5FFA 4FD8 ...` en el fotograma 10950) a la apagada
  (`4EB0 ...`, `100E`, `100B`), así que la explosión se ve de fuego y se
  apaga. El port no tiene paletas animadas (TLUT estática por recurso):
  **parada explícita**; hace falta traducir el motor de fundidos del
  asignador (`0x009EA8`/`0x009F88` y su tabla `0x0574`/`0x0844`) y dar al
  renderizador paletas por fotograma.

## Muerte y reaparición del jugador (`src/marco_death.c`)

Antes el port daba 3 puntos de vida con un retroceso inventado de ±20 px y la
vida nueva caía desde lo alto de la vista (`camera_height+200`) al primer
suelo, incluidas las plataformas dinámicas del jefe. Ahora, como el original:

- **Un impacto mata** (`0x0C2956`). El tipo `+5A` (el mayor de la
  actualización, `neo_world.c player_hit`) elige la muerte: tipo 2
  `0x0C2BCE` (en el aire conserva `+30/+34`; en el suelo los anula; `+80` =
  0x100), tipos 7/0x10 `0x0C2C7A/0x0C2C40` (programa `0x0BF358`, `+80` =
  0x80), los demás `0x0C2DDA` (programas `0x0BF298` en el suelo /
  `0x0BF318` en el aire, `+80` = 0x20). Todas menos la de tipo 2 lanzan al
  jugador: `+34` = 0xA9000 y `+30` = ∓2 según `+5B` bit 7 (atacante a la
  derecha: mira a la derecha y sale hacia la izquierda). **Paradas
  explícitas**: tipo 8 (`0x0C2ACC`) y tipo 0x12 en el suelo (`0x0C2A70`)
  tienen muertes propias; se cuentan (`death_stops`) y usan `0x0C2DDA`.
- **Vuelo** `0x0C2E8A`: en el aire `+34 += (−0x160000 − +34) >> 5`,
  `+30 −= +30 >> 5` y el paso aéreo `0x0143EE` (el del port, con las
  plataformas; sin el empujón hacia arriba de los cuerpos sólidos, `+5F` bit
  7); en el suelo el paso de suelo con velocidad 0. La velocidad del programa
  `+26` se acerca 1/16 a `+80` cada actualización (0x100 al aterrizar), así
  la duración de la animación depende de cuánto dura el vuelo. Los programas
  esperan con `BRANCH_FIELD +78` hasta el aterrizaje.
- **Tumbado** `0x0C2F06/0x0C2F12`: 31 actualizaciones (`+80` = 0x1E), `+30`
  = 0, `+34 −= 0x5000`; fin (`0x0C2F90`) también si sale de la vista
  (`0x0C2F72`).
- **Vida nueva** en la actualización siguiente (panel `0x0C95AE`): una vida
  menos (sin vidas, `0x0C9848` fin de partida: parada explícita, el port
  sigue con 0), el tiempo vuelve a su inicio (`0x019124`), armas de vida
  nueva, y la posición de `0x0196E8`: tabla de reaparición `10C31E`
  (Misión 1: banco 1 `2D5C22`, entradas de 12 bytes por progreso de cámara
  `10E99E`; con progreso ≥ 0x0D30 el registro `2D5C52`: x = raíz + 0x28,
  y = 0x50) más un paso `0x0143E4` con `+7` = 16 px, después la entrada de
  tipo 0 `0x0C1438`: suelo `0x0146EE` (búsqueda `0x013DB2` 256 px hacia
  abajo, terreno o plataforma) y el programa `0x0BF498` (12
  actualizaciones sin control, `+84` = 0x78). En la arena del jefe la vida
  nueva queda en la pantalla x = 56 sobre el suelo de la arena (y = 64), nunca
  sobre el jefe.
- **Fin de tiempo** (`0x0186FA` → `0x0C0A2C`, `+9E` = FF, `+5A` = 6): mata
  al jugador aunque sea invulnerable; el tiempo queda en 0 hasta la vida
  nueva. No en el cartucho de referencia, que sigue invulnerable.

Captura: `tools/capture_weapons.lua` con `MSX_WEAPONS_MORTAL_CAMERA=3300`
(deja de forzar la invulnerabilidad `101523`/`101549` en la arena y escribe
`deaths.csv`), `MSX_WEAPONS_ASSIST_CAMERA=3300`, `MSX_WEAPONS_LAST=17200`:
`/home/user/captures/weapons/death3` (7 muertes en la arena: tipos 2, 7, 9 y
0xD). Prueba `tools/test_marco_death.c` (`make check-core`,
`data/death_fixture.json`): 127 velocidades en vuelo, 302 velocidades de
programa `+26`, las actualizaciones de `0x0C2F06` y de `003F1E`, el punto de
reaparición y la duración de la entrada de las 7 muertes, iguales.
**Parada explícita**: la primera actualización de una muerte de tipo 7 tiene
`+30/+34` = ∓3,80 escritos fuera de `0x0C2C7A` (no localizado; las demás
actualizaciones coinciden). `tools/test_gameplay.c`: en las carreras de
lanzamiento cada vida nueva en la arena (13..15 por carrera) queda en el
suelo de la arena a x de pantalla 56..58.

**Desviaciones declaradas**: el jugador que muere también borra su bit de
`10C33C` y deja de existir una actualización (`0x0C2F96`); las rutas de los
soldados y objetos sin jugador activo no están traducidas (los estados
`0x0AF6C0..` de los soldados entran en bucle), así que el mundo sigue viendo
al jugador activo. Las poses de los programas de muerte no están en el
catálogo de poses auditado: el port mantiene la última pose mientras muere
(registrado una vez).
