# Misión 1 completa: cámara nativa, guion hasta el final, jefe y fin de misión

Continúa [MISSION1_REST.md](MISSION1_REST.md), [SOLDIER_WORLD.md](SOLDIER_WORLD.md)
y [JEFE_MISION1.md](JEFE_MISION1.md). La región jugable ya no termina en la
cámara 1920: el guion original se ejecuta entero (`0x2D6314..0x2D7132`), la
cámara es la rutina original traducida, el mapa y la colisión llegan a 3840 px,
el jefe nativo está conectado al mundo y la partida termina con la señal de
misión completada del original (`0x018F92`, `0x018F40`). Resumen numérico en
[mision1-completa-validation.json](mision1-completa-validation.json).

Regla de siempre: cada pieza traducida tiene evidencia de MAME (fixture en bucle
cerrado o validación píxel a píxel) y todo lo que no está recuperado se detiene
explícitamente y se cuenta; nada se aproxima.

## Cámara nativa (`src/neo_camera.{c,h}`)

Traducción instrucción a instrucción de la cámara de misión:

| ROM | Pieza |
| --- | --- |
| `0x018D96..0x018F0A` | movimiento pedido: punto de los jugadores (`0x01903A`), zonas muertas alrededor de `69B8/69BA` (−0x2C/−0x113 en X, 0/−0x1DF en Y), ×3/16, límite de velocidad `+72/+76` del actor de misión (10,0 px) y suavizado con la velocidad anterior de la raíz |
| `0x0C81C0` | autoscroll (`695A/695C/6960`) en lugar del movimiento pedido |
| `0x0C7CA6`, `0x0C7ECE`, `0x0C7F3A`, `0x0C7EBC` | posición, progreso por camino (modos 0..7 de `69B6`; el progreso no supera el objetivo `69A2` ni retrocede), límites `69AE..69B4` y operandos de la parada del guion `69BC/69C0..69C6` (`0x8000` = sin cambio), velocidad de la raíz `10E968` y rectángulo visible `69D2..69D8` |
| `0x0C804A`, `0x0C801A`, `0x0C8022` | temblor (decaimiento y peticiones) |
| `0x0C83E6` | lista de comandos de mapa/cámara (`0x0B57BE`): esperas de progreso, modo de camino, límites, punto `69B8`, llamadas condicionales `0x54/0x58` (`69D0`); las órdenes de capas, paleta y sonido se devuelven como eventos |
| `0x0C7B0C`, `0x0C7C12`, `0x0C7B4E`, `0x0C7BBA`, `0x0C7B48` | parada/liberación del guion, autoscroll de las ops D/E, modo fijado por un actor |

Evidencia: `tools/capture_mission_rest.lua` con `MSX_REST_CAMERA=1` escribe
`camera.csv` desde un *tap* de escritura en el último `store` de `0x0C7CA6`
(`10E9D8`): entradas que lee `0x01903A` y el bloque `10E94E..10E9DB` tras cada
actualización. `tools/extract_camera_fixture.py` →
`data/camera_fixture.json.gz`; `tools/test_neo_camera.c` (en `make check`)
ejecuta la cámara **en bucle cerrado** durante toda la misión: **5085
actualizaciones (fotogramas 1837..13721), 0 diferencias** en posición, delta,
progreso, objetivo, punto del camino, límites, modo, temblor, raíz y
rectángulo visible. Las escrituras de otros actores entre dos actualizaciones
se aplican con los puntos de entrada originales y se cuentan: 3 puntos del
guion (op F), 13 paradas, 2 autoscroll (ops D/E), 14 escrituras `0x0C7B48` del
edificio de cúpulas `0x05486A` (modo 2, `vy`) y 1 temblor.

En el juego, `neo_world_camera` hace el paso de cámara del actor de misión con
las palabras proyectadas del jugador; la cámara mostrada es `ceil(10E98E)` y
la altura `ceil(10E992)`; el mundo usa la raíz exacta (`10E97C/10E980`, con
el temblor) como desplazamiento de escena, de modo que las palabras proyectadas
ya incluyen la altura. Las capas siguen las reglas medidas de
`mission_camera.h` a partir de esa cámara. El evento de paleta `0x0B56CA` del
progreso 2304 cambia el color de fondo (`0x2337` → `0x1EA4`). La subida a 32
en la parada 1920 la hace el edificio de cúpulas (no portado): sin él la
cámara se queda en 16 (14 sin saltar), como corresponde a la rutina original
sin esas escrituras.

## Guion completo

`tools/extract_soldier_ai_tables.py` importa ahora el guion hasta su registro
op 3 (`0x2D7132`, fin `0x2D7134`); `MISSION1_REGION_END` (`0x2D6A28`) marca el
fin de la región del pueblo. `neo_script` pasa las ops D/E al autoscroll de la
cámara (`0x01A34A/0x01A35E`) y admite registros de aparición de hasta 12
palabras (`0x2D6E3C`, `0x2D70D8`). Recorrido nativo: el guion llega a su op 3
sin detenerse.

## Mapa, colisión y presupuesto

- Franja inferior tras el camión `0x02916E` (informe del usuario en MiSTer:
  subido a los restos, «la cámara sube y no se dibuja una franja abajo»).
  Durante la primera subida de cámara (944..1008, RAM `10E980` de 256 a
  240-242) la vista aún no ha subido del todo cuando entran las columnas
  x ≥ 1216, cuya capa 3 acaba en y = 192 en el mapa recuperado: la fila
  y = 208 nunca se observó en las capturas del mapa (subieron la cámara en
  otro momento) y el port dejaba ver el fondo abajo a la derecha. La captura
  de soldados `m2x` tiene VRAM durante la subida: `extract_mission_map.py m2x
  --frames 6370:9400` y el nuevo `tools/merge_observed_cells.py` añaden 100
  celdas no observadas antes (capa 3: fila 208 en 1216..1440 y filas sobre
  la vista; capas 2, 1 y 0), comprobando idénticas las 1861 ya conocidas
  (o una de sus variantes registradas). El recorrido de ares toma
  `truck-wreck.png` (jugador sobre los restos en la parada 872) y
  `camera-risen.png` (progreso 1024, la zona de la franja).

- `tools/pack_scene.py` une el mapa de la región y el del resto
  (`filesystem/mission1/rest/map.json`; las 1061 celdas comunes son
  idénticas); `SCENE_MAP_CAMERA_MAX` = `MSX_CAMERA_MAX` = 3520. Los atlas de
  cada paleta se ordenan por la primera X en pantalla de cada tesela.
- `tools/compile_collision.py` usa la rejilla de 480 columnas
  (`filesystem/mission1/rest/collision.json`, sus 280 primeras columnas son la
  de la región).
- No aplicados todavía (dependen de actores no portados): parche del edificio
  `0x0B6566` (62 celdas variantes) y sus sellos de colisión
  `building_raise/building_clear`; desplazamiento del cielo y mapa delantero
  del derrumbe `0x0B58CC`.

Pack: **223 chunks, 1777 páginas, 155 paletas** (≈1,9 MB empaquetados). Con la
Expansion Pak la caché de escena pasa de 3 a 3,5 MiB y **todo el pack queda
residente** (224 ranuras). Con 4 MiB (32 ranuras) el mapa cabe (peor conjunto
de trabajo 28 chunks); en la arena del jefe mapa+fijos ocupan 22–23 chunks y
el jefe 14–15 por pasada lógica (`tools/check_boss_budget.py`): **37–38 > 32,
no cabe** sin rediseñar los fijos o el tamaño de caché (pendiente; ares
siempre usa 8 MiB).

## Jefe conectado al mundo

`neo_world` crea el jefe con el registro `0x2D70FC` (constructor `0x0D5976`):
nodo con la imagen del asignador (`0x004126`), palabras en `+6E..`, grupo 6,
variante, posición en la raíz y proyección. Sus nodos se ejecutan como un
bloque en la posición de lista del cuerpo. Servicios (`neo_boss_services_t`)
contestados por el mundo:

| Servicio | Implementación |
| --- | --- |
| RNG, sonido | anillo nativo; peticiones a `sounds[]` |
| asignador `0x0040BC` | ranuras del propio jefe; bytes heredados = los que `0x004126` deja (los del ocupante anterior no se siguen: 0) |
| paletas `0x009EA8/0x009F88` | ranura por recurso lógico con contador; el dibujo resuelve ranura → recurso |
| objetos de escena, barrido y suelo | raíz/terreno; `10D7B6/10E8C6` = terreno (banco medio a velocidad completa); núcleo de colisión nativo |
| plataformas `0x0140C0/0x013F1E` | objetos dinámicos del núcleo (`0E93B2/0E94AC/0E95A6`), visibles para soldados y actores genéricos |
| registro `0x004C24` y ataque `0x0049E8` | casillas de la ventana de registro; filas de ataque de clase 0 contra el jugador |
| efectos | `01C706/01C71E/01D764/01D86C…` siguen como actores genéricos nativos; `02A18A` es `JMP 0041AC`; las estelas convertidas `02A3CE` cuentan como externas; los prisioneros `099DCC` son nativos (MISION1_ACTORES.md) |
| misión `0x018F92/0x018F40` | `mission_step` 1 y 2 → `mission_complete` / `complete` |

Los disparos del jugador llegan al jefe por la fase estrecha original
(`0x004762` + `0x004892..0x0049E0`, matriz de impactos, multiplicador `+54`).
Tras un nodo que se detiene, el recorrido sigue los enlaces como `0x00424A`
(enlaces de nodos liberados). El disparo del cañón de la torreta
(`0x08FA1A`: destello `0x08EFFA` y proyectil `0x08F058`) está traducido
desde la ROM (ninguna captura lo tiene); ningún nodo del jefe se detiene
(antes: el cañón en `0x08FA1A` y, sin su cañón, la base en `0x0D6254` o
`0x0D62F2`). Dibujo con
`boss_render_draw` (recorte de columnas original).

## Espadachín de túnica `0x0A8C46` (arranque del jefe)

El jefe espera en `0x0D5A92` a que RAM `10C606` cambie. Lo cambia una sola vez
el espadachín de túnica del registro `0x2D70D8` con el gesto `2A73B4` (la
llamada `0x0A9EDA` pone `+60` bit 0; con `+93 = 1`, `0x0A8F12` invierte
`10C606` y pone `+60` bit 1) y lanza un subguion `0x019E72` con el registro
`[0x00087A + 4·(+74)]` = `0x2D7214`, que es un op 3 aislado: en su primera
actualización (`0x01A02C`) pone `+60` bit 7 del espadachín. Traducido como
actor genérico en `src/neo_rest.c` (constructor, entrada, espera, giro hacia
el jugador más cercano `0x01AF9A`, gesto, bucle, retirada `0x0A9A56`) con el
subguion; la salida `0x0A8FE0` (cuando el grupo 6 llega a 0) y la rama `+93 =
0` (espadachines del mercado `0x0A8C62/84`) se detienen explícitamente.
`10C606` vale `0xFF` al aparecer el jefe en los dos combates grabados.

Evidencia: traza de familia (`MSX_REST_FAMILY_ROOTS=a8c46
MSX_REST_BOSS_FROM=10550`) → `tools/extract_rest_family.py` →
`data/robe_fixture.json.gz`; `tools/test_neo_rest.c` (en `make check`):
**231 actualizaciones en bucle cerrado (10683..11197), 32802 bytes iguales**,
`10C606` invertido una vez en la misma actualización, subguion terminado.
Excluidos de la comparación: números de ranura de paleta (`+14`, `+16`,
`+19..+1C`), puntero de animación `+20` y posición en caché `+44/+48`
(posición anterior de la raíz, no está en la traza). Gráficos:
`tools/export_rest_sprites.py` exporta sus 29 descriptores y las paletas
`0x1B/0x20B/0x3BA` (`filesystem/rest-native.json`, en el banco
`SCENE_NATIVE`) y los valida contra la VRAM de 17 volcados: **19450 píxeles
iguales, 0 distintos**
([rest-sprites-validation.json](rest-sprites-validation.json)).

## Cobertura del resto de la misión

| Constructor | Estado |
| --- | --- |
| `0x0AF17E`, `0x04115A` (soldados), `0x02916E`, `0x0DC97E`, `0x0403A0`, efectos | nativos (ya existían) |
| `0x0A8C46` espadachín de túnica + subguion `0x019E72` | **nativo** (nuevo) |
| `0x0D5976` jefe (86 rutinas, ver JEFE_MISION1) | **nativo y conectado** (disparo de la torreta traducido sin captura) |
| `0x05486A`/`0x054904` edificio de cúpulas y derrumbe `0x0B58CC` | **nativo** (MISION1_ACTORES.md) |
| `0x0B6566` edificio con barriles (parche y sellos) | **nativo** (MISION1_ACTORES.md) |
| `0x033C98` carro con lona (plataformas, restos, piezas) | **nativo** (MISION1_ACTORES.md) |
| `0x0A8C62` espadachín de túnica sobre el carro + subguion `0x2D719A` | **nativo** (MISION1_ACTORES.md) |
| `0x092A32` cañón | externo |
| `0x04857C` formación de helicópteros (`0x04870C` guías, `0x048882` helicópteros, `0x047874` rotores, `0x047A6C` lanzadores, `0x047B24` bombas, `0x047DE0` casquillos) | **nativo** (MISION1_ACTORES.md) |
| `0x05ED84` lanzacohetes, cohetes `0x05FD8A`, destello `0x060490`, fogonazo `0x0602E4`, estelas `0x059C82` | **nativo** (MISION1_ACTORES.md) |
| `0x039590/0x0395B6` espadachines árabes del carro y muerte en trozos `0x0A93CE` | **nativo** (MISION1_ACTORES.md) |
| `0x035668` criatura del subguion | **nativo** (MISION1_ACTORES.md) |
| `0x09D846/62/76` espadachines, cuchillo `0x09ECCC`, salpicadura `0x01C97A` | **nativo** (MISION1_ACTORES.md; `0x09E382` parada explícita) |
| `0x09F682` jinetes de camello (jinete `0x09FEAA`, cabeza `0x09FF92`, disparos `0x0A0034`, piezas `0x0A016A`, espada `0x09EE34`) | **nativo** (MISION1_ACTORES.md) |
| `0x099862` prisionero (suelta, objeto) | **nativo** (MISION1_ACTORES.md) |
| `0x08C374/0x08C37C` objetos (tras la región; recogida = parada explícita) | **nativo** (MISION1_ACTORES.md) |
| `0x087E36` vehículo blindado de la parada 1304 (proyectiles y piezas externos) | **nativo** (MISION1_ACTORES.md) |
| `0x099DCC`, `0x099E06` prisioneros (y la cuerda `0x09A97C`), en toda la misión | **nativos** (MISION1_ACTORES.md) |
| `0x031D92` (objeto de escena), `0x03458E` Camel Slug | externos |
| `0x01913C` (temporizador), `0x0B63A0/0x0B639A` toldos, piezas `0x08732C` | externos |
| `0x0B6298` decoración del pueblo, `0x0400AA` toldo del puesto, `0x03264E` porteador | **nativos** (MISION1_ACTORES.md) |

"Externo" = el registro del guion se ejecuta y se cuenta (`external_spawns`),
el actor no se crea ni se dibuja y no cuenta en los grupos (las esperas "grupos
a cero" pasan). En un recorrido completo: 74 apariciones externas.

## Jugador del port

No es el Marco original (ver MARCO*.md); para atravesar el resto: disparo
hacia arriba con ARRIBA (fila 4 de la pistola `0x01B502`, rectángulo vertical),
los disparos usan la fase estrecha original también contra los actores
genéricos registrados (bombardero, camión, jefe) y la reaparición tras perder
las vidas es en la vista actual (la cámara original no retrocede). El jugador
no se sube a plataformas dinámicas (jefe/camión): solo usa la rejilla
estática.

## Recorrido en ares 146

`make mslugx-reference.z64` compila el mismo juego con
`-DMSX_REFERENCE_INVULNERABLE=1` solo en `gameplay.o` (el jugador ignora los
golpes; nunca es la ROM de publicación, que se sigue llamando
`mslugx-explorer.z64`). `tools/test_mission_native_ares.py` (ares con el
teclado enlazado al mando) camina disparando, apunta arriba cada medio segundo,
salta cada 4 s y en la arena alterna izquierda/derecha; comprueba en el
registro las paradas de cámara (872, 1304, 1920, 2336, 3192, 3376), el
autoscroll, la paleta de 2304, la aparición del jefe, la señal `10C606`, las
fases del jefe y la señal de misión completada, y que solo se detienen los
nodos documentados. Resultado y capturas en
[mision1-completa-validation.json](mision1-completa-validation.json).

**Resultado: PASS.** El guion llega al final, el espadachín arranca el jefe,
el jefe pasa por sus fases (5000 → 0 PV) y emite `0x018F92` y `0x018F40`;
34 rebeldes creados, 25 abatidos; solo se detienen los dos nodos de la
torreta documentados; 0 descriptores/paletas ausentes, 0 fallos de caché,
**0 *underruns* de audio**.

### Rendimiento (`MSX PERF`, ares 146, caché de 8 MiB)

| Tramo | `cpu` media máx. | `cpu` peor fotograma |
| --- | ---: | ---: |
| Inicio → arena (cámara 0..3376) | 8.836 µs | 10.538 µs |
| Combate con el jefe | 13.449 µs | 16.744 µs |

Cambios de rendimiento de este paso: páginas de mapa altas de 16×256
(teselas apiladas por código: una columna del mapa con códigos consecutivos
se dibuja en un solo rectángulo; ~530 → ~290 dibujos de fondo), atlas de la
región separados de los del resto, y el recorrido del jefe con un único
orden de lista por pasada (antes O(n²)). Referencia en el mismo ares: la ROM
de `origin/main` da 10,5–12,0 ms de media y 13,0 ms de peor fotograma en la
región, es decir, la medida de este ares es más lenta que la de
RENDIMIENTO.md. **El combate con el jefe supera los 12 ms** (simulación
del jefe hasta 5,6 ms, su dibujo hasta 4 ms, fondo 9,4 ms): pendiente.

ROM de publicación `mslugx-explorer.z64`: 5.111.808 bytes.

### Fusión ROM 8 + Camel Slug (rama `work-weapons`, tras `f9a1ac6`)

Exportación de sprites regenerada (orden de abajo, «Exportación completa»).
Puertas: `make check-core` en un clon limpio, `make check` (determinismo,
`missing_draws` = 0, auditoría: solo `0x092A32` parado explícitamente),
`make -j4` y `make mslugx-reference.z64` sin cambios de código. Recorrido
completo en ares (`trav-long.sh`, Xvfb :92, 6000 s): **PASS**, 37 rebeldes
creados, 43 abatidos, jefe derrotado, misión completada
(`018F92/018F40`), ningún nodo del jefe detenido, 0 *crash* del RSP,
*underruns* de audio máx. 1; `MSX PERF` 127 ventanas, `cpu` media máx.
18.777 µs, **peor fotograma 35.564 µs**; RDP: 399 dibujos máx., 133
fallos máx. El recorrido no subió a la Camel Slug (`mounts=0`). ROMs
`mslugx-explorer.z64` y `mslugx-reference.z64`: 8.683.520 bytes cada una.

## Audio

El reproductor de VADPCM de la libdragon fijada solo da la vuelta a ficheros
completos (`wav64_vadpcm.c` exige `loop_len == len` al llegar al final). La
música de la misión tiene un bucle parcial medido (`--wav-loop-offset`), y con
el recorrido completo se llegaba al final del fichero: `msx_audio_play` repite
ahora el fichero entero si es VADPCM con bucle parcial (ver AUDIO_JUEGO.md).

## Reproducir

```sh
MSX_REST_CAMERA=1 MSX_REST_FIRST_DUMP=99999 tools/run_capture_rest.sh /ruta/cam1
python3 tools/extract_camera_fixture.py /ruta/cam1 --output data/camera_fixture.json.gz
MSX_REST_FAMILY_ROOTS=a8c46 MSX_REST_BOSS_FROM=10550 MSX_REST_LAST=11200 MSX_REST_FIRST_DUMP=10560 \
  tools/run_capture_rest.sh /ruta/robe1
python3 tools/extract_rest_family.py /ruta/robe1/boss-trace.txt --root a8c46 --output data/robe_fixture.json.gz
python3 tools/extract_soldier_ai_tables.py /ruta/captures-soldiers/entry-walk3 --output data/soldier_ai_tables.json
python3 tools/extract_soldier_programs.py /ruta/captures-soldiers/walk3 --output data/soldier_programs.json
python3 tools/extract_region_rom.py /ruta/captures-soldiers/walk3 --output data/region_rom.json
cd tools && python3 export_rest_sprites.py /ruta/mslugx.zip /ruta/robe1 && cd ..   # solo el espadachín; la exportación completa, abajo
make check-core && make check && make -j4 && make mslugx-reference.z64
python3 tools/test_mission_native_ares.py --log /ruta/ares.log --output /ruta/ares
```

### Exportación completa de `filesystem/rest-native.json` (privada)

Unión de todas las capturas de ambas líneas de trabajo (misión: `robe1` …
`unc1`, `runK`; vehículos: `hfam`, `cfam`, `camelfam`, `death2`), sin
`--keep`: ninguna paleta depende de una exportación anterior. Tras una
fusión basta repetir esta orden (añadiendo las capturas nuevas de cada lado)
y comprobar que todo descriptor y paleta de las exportaciones de ambos lados
sigue presente e idéntico.

```sh
C=/home/user/captures-m1full W=/home/user/captures/weapons B=/home/user/captures-boss
cd tools && python3 export_rest_sprites.py /home/user/roms/mslugx.zip \
  $C/robe1 $C/bld1@54800-55700 $C/bld1@b6500-b6b00 $C/trk3@33c98-34200 $C/trk3@39500-3a100 \
  $C/trk3@a93c0-a9a00 $C/trk3@35600-35a00 $C/pri1@99862-99dc0 $C/pri1@8c37c-8c700 $B/runA@0-1 \
  $C/veh1@87e36-89c00 $C/veh2@88b52-89940 $C/box1@9a67e-9ae80 $W/hfam@1b600-1bd00 \
  $W/cfam@1bd00-1c400 $W/cfam@8c37c-8c700 $C/fam2@9d800-9ef00 $C/fam2@1c97a-1c97a \
  $C/fam2@99dcc-9a9c2 $C/food1@8c374-8c700 $C/rl2@5ed84-60600 $C/rl2@59c82-59e30 \
  $C/heli2@47800-49000 $C/camel2@9ea00-a0800 $C/unc1@b6200-b6300 $C/unc1@400aa-40100 \
  $C/unc1@3264e-32940 $B/runK@60900-60fff $W/camelfam@34300-35600 $W/camelfam@3b4e0-3b900 \
  $W/camelfam@1be24-1be30 $W/camelfam@59e30-59f00 \
  --palette-capture /home/user/captures/blood --palette-capture $W/death2 --keep /nonexistent && cd ..
```

Resultado (fusión ROM 8 + Camel Slug, 2 m 52 s): 2813 descriptores (los
2379 del lado de la misión y los 2789 del lado de vehículos, idénticos), 77
paletas (todas las de ambos lados, idénticas salvo `0x208`: el lado de
vehículos la tomaba solo del volcado de `death2`, la única de sus capturas
con ella; la unión toma el valor de `veh1`, `veh2`, `box1`, `food1`, `heli2`
y `runA`, el del lado de la misión, 18 volcados contra 1), faltan las mismas
`0x37F`, `0x133`, `0x38E`, `0x823`, `0x377`; 613 volcados validados
([rest-sprites-validation.json](rest-sprites-validation.json)): 2930624
píxeles iguales, 150106 distintos (los ya documentados por familia).

## Fin de partida y continuación (`0x0C9848`)

Antes el port reaparecía sin fin con 0 vidas. Ahora el panel P1
(`neo_hud.c`) ejecuta el fin de partida original cuando la vida nueva
`0C95E6` no encuentra vidas (`gameplay.c`: el jugador queda fuera hasta la
continuación):

| ROM | Pieza |
| --- | --- |
| `0C9848` | DIP `10FD8A` (continuación permitida), dos jugadores → GAME OVER; créditos `1A872` → `+8C`; `1A8AA` (CREDIT_CHECK de la BIOS en MVS) o AES → `019798` (actor CONTINUE?, **parada explícita**), `0C9254` limpia el panel, `19008` pone el contador `1013E2` = `0x9FF` |
| `0C98CC`/`0C9918` | modo del jugador `10FDB6+p` con la tabla `27B4` de `27C4`; moneda nueva → contador a 9 (`0C993E`); contador 0 → GAME OVER; texto `CONTINUE? ` (`0C90FE`, fuente de dos filas) una vez y la cifra `0C9112[contador>>8]` cada actualización |
| `0187A4..0187EC` | paso del contador en el actor de misión: −5 por actualización; por debajo de `0x780`, B (`190F0` D1 bit 5) salta a la cifra siguiente (`|0x80 − 0x100`); pitido `0x10D4` al cambiar de cifra |
| `0C98DE`, `0C8CB2`, `0C9420` | continuación tomada: la última cifra de la puntuación cuenta continuaciones (máx. 9), contador `10E9E4+2p`, vidas del DIP `10FD88` (1 si es 0), modo 1 |
| `0C99E6`, `0C9ABC`, `0C9ACE` | GAME OVER: modo 3, panel limpio, `+88 = 3`, `+80 = 0x1E` (se lee su byte alto: 0) y el texto ` GAME OVER ` cada actualización con el HUD encendido |

**Créditos en el port.** Cartucho doméstico = juego libre: la continuación
siempre se ofrece (`can_continue`, la rama AES de `0C987A`) y START la toma
(la BIOS pone el modo 1). La captura con la BIOS euro (MVS, 0 créditos)
muestra lo mismo: con `CREDIT 0` la BIOS ofrece la cuenta atrás y la
moneda la reinicia a 9; el panel P2 sigue con INSERT COIN y `CREDIT 0`
como en el resto del port. DIP de vidas 3. Sin START, tras el GAME OVER el
original apaga el HUD y devuelve la máquina a la BIOS 190 actualizaciones
después del modo 3 (`go1`: fotogramas 4578..4957; luego la tabla «BEST
TANK BUSTERS» y la demo): el port no tiene tabla ni demo y **reinicia la
misión 1** tras esas 190 actualizaciones.

**Paradas explícitas** (`neo_hud_t.unsupported`, registro `MSX HUD explicit
stops`): el actor de sprites CONTINUE?/cifra grande `019798` (`08CD58`), la
tabla de récords `057B44`/entrada de nombre `058132` (se sigue la rama de
puntuación que no entra, la de las capturas), el gráfico de unión `0C94D6`
tras la continuación (el panel de juego se redibuja en la actualización
siguiente), las ramas `0C9312`/`0C9406` y el retardo `0C9AD6`.

**Evidencia.** `tools/mame/capture_gameover.lua` (MAME 0.264, BIOS euro;
vidas `101288` forzadas a 0 una vez en el fotograma 2000, solo captura):
`go1` sin moneda (cuenta atrás completa y GAME OVER), `go2` moneda en 3800
y START en 3860 (continuación, puntuación `…500` → `…501`, vidas 3), `go3`
B cada 40 fotogramas (el salto de cifra por debajo de `0x780`).
`tools/extract_continue_fixture.py` → `data/continue_fixture.json.gz`;
`tools/test_neo_continue.c` (en `make check-core`): **1018 actualizaciones,
48816 palabras del panel P1 (16×3 desde `7043`), contador `1013E2` y `+88`
iguales a MAME**; en la actualización de la continuación se comprueban
puntuación, vidas y modo (sus palabras de panel son las de `0C94D6`, no
portado).

```sh
MSX_CAPTURE_DIR=/ruta/go1 MSX_GO_LAST=6000 mame mslugx -bios euro -video none -sound none \
  -nothrottle -autoboot_script tools/mame/capture_gameover.lua   # go2: MSX_GO_COIN=3800; go3: MSX_GO_TAPB=1
python3 tools/extract_continue_fixture.py /ruta/go1 /ruta/go2 /ruta/go3 --b-taps go3:3600:4400:40 \
  --output data/continue_fixture.json.gz
```

## Fin de misión (`0x018846`)

Tras `0x018F40` (paso 2; borra `4303`) el actor de misión pasa por su
registro de fin (`0x0182C4`) a `0x018846`. Traducido en
`gameplay.c` (`mission_end_step`, un paso por actualización desde la
siguiente al paso 2) con sus contadores:

| ROM | Pieza | Port |
| --- | --- | --- |
| `018846..018870` | `10E9E3`=FF, música `15418`, `+7C`=0x1E, `subq/bne` | 30 actualizaciones |
| `018884..0188B8` | `cae6c(misión)` (RAM `10EA1B`: misión más alta, byte con signo) y el actor de recuento de prisioneros `0308C0` (suma la bonificación a `10C46A`; espera su `+60`) | `cae6c` traducido; recuento = **parada explícita**, sigue en el acto |
| `0188BE..0188F4` | sonidos `0x20`, `0x1137`; `+7C`=0xA0; actor de letras MISSION COMPLETE `06090A`; `subq/bcc` | 161 actualizaciones; letras **nativas** (`src/neo_end.c`, abajo) |
| `0188FC..018916` | fundido `9C0A` (E1,E1,E1, 0x20), `+7C`=0x20 | 33 actualizaciones; fundido **nativo** (`src/neo_fade.c`) |
| `01891E` | misión `42F4`+1 < 6 → misión siguiente (`1829E`) | misiones 2–6 no portadas: **vuelve a empezar la misión 1** |

### Letras MISSION COMPLETE `06090A` (`src/neo_end.c`)

Traducción manual de `06090A..060F42`: la raíz (en pantalla en 0,0x1FF;
crea la cifra de misión `060B06` y una letra `060A72` por registro de 8
bytes de `606A6`: programa `+60`, `+61`, retardo `+72`, destino `+6E/+70`),
las letras (velocidad = distancia/4 hacia el destino, encogimiento `+12`
dentro de 0x30 px con `+76`/`+7A`, paletas `0x3BE` → `0x3AA` → `0x200`,
llegada: `+61` de la raíz + 1), el parpadeo por el bit 3 de `+72` de la raíz,
`+60`=FF de la raíz (que pone el `+60` del actor de misión y se borra) y la
salida acelerada (`+78`, ángulo desde (A0,190) + 0x80, `002EB4`/`002E7C`)
hasta el recorte `01945C` con el rectángulo `60F94`. Lista propia
(`neo_end_t`): los nodos los crea el actor de misión (`0040BC`, tras él) y un
nodo borrado conserva sus bytes (las letras leen `+60/+72` de la raíz ya
borrada, como en la RAM original). Programas `60F44`/`60F7C` añadidos a
`extract_soldier_programs.py` y a `export_rest_sprites.py` (ROOTS al final;
paletas `0x3BE/0x3AA/0x200`; 2355 → 2379 descriptores, ninguno quitado).

Evidencia: `runK` = `runJ` repetida con traza de familia
(`MSX_REST_BOSS_FROM=13690 MSX_REST_FAMILY_ROOTS=308e4,60974,308c0,6090a
MSX_REST_FIRST_DUMP=13690 MSX_REST_PERIOD=4 MSX_REST_LAST=14500`).
`tools/extract_end_fixture.py` → `data/end_letters_fixture.json.gz`;
`tools/test_neo_end.c` (en `make check`): **1672 actualizaciones en bucle
cerrado (fotogramas 14040..14230, 18 nodos), 235010 bytes iguales, 17
borrados**, mismas reglas de exclusión que las familias (`+14/+16/+19..+1C`,
`+20..+23`, `+44..+4B`). Sprites contra la VRAM de los volcados de `runK`
([end-sprites-validation.json](end-sprites-validation.json)): 48 volcados,
103424 píxeles iguales, 1680 distintos, todos en tres volcados: 14040 (la
primera actualización: la lista de sprites aún no ha llegado a la VRAM) y
14220/14224 (salida rápida, 2 px por actualización; el validador elige una
sola actualización por volcado). `tools/test_gameplay.c` comprueba que las 18
letras llegan, salen y la raíz marca el `+60`, sin paradas.

### Fundido `9C0A` (`src/neo_fade.c`)

`009C0A` fija los objetivos (cada canal limitado a −31..31, en 8.8), el
número de pasos (`d3`, 0/1 → 1) y el paso `divs.w`; `00698C` lo ejecuta el
actor cabeza de lista `003918` una vez por actualización (antes del actor de
misión) y el último paso cae en el objetivo. El dibujo suma el byte alto a
cada canal de 5 bits con saturación (como la conversión `005A0A`) en copias
de las TLUT que se suben mientras dura (también la paleta de Marco y el color
de fondo); la capa fija queda fuera. MAME `runK`: las paletas 0..9 no
cambian, la entrada de fondo 0xFFF sí, todo negro en 14452. Prueba
(`test_gameplay`): −248 por actualización y −31 al terminar.

### Paradas que quedan

- **Recuento de prisioneros `0308C0`** (y `cae6c` ya traducido): el actor
  dibuja en la capa fija con el motor de texto completo (`03DD96`: páginas
  0x30, `A`, `H`, `K`, `a`, `h`… y los prefijos DE/DF, de los que `neo_hud`
  solo tiene `<10` y `a`), nombres y grados aleatorios de los prisioneros
  (`2CAD6`/`2C982` con `0029F2`), el desfile `030FB2` (programas
  `237AEC/237B84/237BD0/237C48`, ya exportados por `extract_soldier_programs.py`)
  y la bonificación BCD (`057948`: 10000 por prisionero, 100000 y la tabla
  `2D0A74` por `+8E`). Su traza de familia está en `runK`
  (`MSX_REST_FAMILY_ROOTS=308e4`, fotogramas 13759..14453, 9 nodos) para cuando se
  traduzca. Sigue siendo la única parada del fin de misión.
- **`019798`** (fin de partida): no es un actor de sprites propio; crea un
  objeto `08C374` con `08CD58` (tipo de la tabla `019792`, `+70`=0x384,
  `+72`=0x10) y lo prepara con `0196E8`. Hace falta una captura de fin de
  partida con volcados de VRAM de sprites y traza de familia (las de `go1..3`
  ya no están en disco y `capture_gameover.lua` solo guarda la capa fija).

Captura `runJ` (volcados cada 30 fotogramas): `018870` en 13710, `0188B0`
(recuento) en 13770, `0188F4` en ≤14040, `018916` en 14370, misión 2 en
14460; la puntuación pasa de 55330 a 285330 mientras `10C5FC` baja de 11 a
0 (recuento). Prueba: `tools/test_gameplay.c` comprueba 30+160+32
actualizaciones y 1 parada (recuento).

## Pendiente

- Actores del resto aún externos (tabla de cobertura): el SV-001 `0x092A32`
  (no contado; la Camel Slug `0x03458E` es nativa y se puede montar).
- Disparo de la torreta del jefe (`0x08FA1A` → `0x08EFFA`/`0x08F058`):
  traducido sin captura (falta una captura en la que dispare para
  verificarlo); `0x08F1A0` sigue como parada explícita.
- Bytes heredados de nodos reutilizados del jefe (se usa 0), orden de lista
  del jefe como bloque, salida del espadachín (`0x0A8FE0`).
- Presupuesto de 4 MiB en la arena del jefe (37–38 chunks de 32).
- El jugador no usa plataformas dinámicas ni se sube al jefe.
- Rendimiento del combate con el jefe (ver RENDIMIENTO.md, «Picos del combate con el jefe»).
- Actores de fin de partida/misión: `019798`, `057B44`, `0308C0` (las letras `06090A` y el fundido `9C0A` ya son nativos).
