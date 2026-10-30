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
(enlaces de nodos liberados). Detenciones explícitas observadas en el
recorrido: el cañón de la torreta al disparar (`0x08FA1A`, bala genérica
`0x08EFFA` no portada) y, sin su cañón, la base (`0x0D6254`). Dibujo con
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
| `0x0D5976` jefe (86 rutinas, ver JEFE_MISION1) | **nativo y conectado** (salvo disparo de torreta) |
| `0x05486A`/`0x054904` edificio de cúpulas y derrumbe `0x0B58CC` | **nativo** (MISION1_ACTORES.md) |
| `0x0B6566` edificio con barriles (parche y sellos) | **nativo** (MISION1_ACTORES.md) |
| `0x033C98` carro con lona (plataformas, restos, piezas) | **nativo** (MISION1_ACTORES.md) |
| `0x0A8C62` espadachín de túnica sobre el carro + subguion `0x2D719A` | **nativo** (MISION1_ACTORES.md) |
| `0x092A32` cañón, `0x04857C` helicóptero | externos |
| `0x05ED84` lanzacohetes, cohetes `0x05FD8A`, destello `0x060490`, fogonazo `0x0602E4`, estelas `0x059C82` | **nativo** (MISION1_ACTORES.md) |
| `0x039590/0x0395B6` espadachines árabes del carro y muerte en trozos `0x0A93CE` | **nativo** (MISION1_ACTORES.md) |
| `0x035668` criatura del subguion | **nativo** (MISION1_ACTORES.md) |
| `0x09D846/62/76` espadachines, cuchillo `0x09ECCC`, salpicadura `0x01C97A` | **nativo** (MISION1_ACTORES.md; `0x09E382` parada explícita) |
| `0x09F682` camello | externo |
| `0x099862` prisionero (suelta, objeto) | **nativo** (MISION1_ACTORES.md) |
| `0x08C374/0x08C37C` objetos (tras la región; recogida = parada explícita) | **nativo** (MISION1_ACTORES.md) |
| `0x087E36` vehículo blindado de la parada 1304 (proyectiles y piezas externos) | **nativo** (MISION1_ACTORES.md) |
| `0x099DCC`, `0x099E06` prisioneros (y la cuerda `0x09A97C`), en toda la misión | **nativos** (MISION1_ACTORES.md) |
| `0x031D92` (objeto de escena), `0x03458E` Camel Slug | externos |
| `0x01913C` (temporizador), `0x0B63A0/0x0B639A` toldos, piezas `0x08732C` | externos |

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
cd tools && python3 export_rest_sprites.py /ruta/mslugx.zip /ruta/robe1 && cd ..
make check-core && make check && make -j4 && make mslugx-reference.z64
python3 tools/test_mission_native_ares.py --log /ruta/ares.log --output /ruta/ares
```

## Pendiente

- Actores del resto aún externos (tabla de cobertura), en especial el camión `0x033C98` y sus plataformas, el cañón `0x092A32`,
  el helicóptero `0x04857C`, la infantería (`0x09D846/62/76`, `0x0A8C62`,
  `0x09F682`), prisioneros/objetos y la Camel Slug `0x03458E`.
- Disparo de la torreta del jefe (`0x08FA1A` → bala `0x08EFFA`/`0x08F058`/
  `0x08F1A0`): necesita una captura en la que dispare.
- Bytes heredados de nodos reutilizados del jefe (se usa 0), orden de lista
  del jefe como bloque, salida del espadachín (`0x0A8FE0`).
- Presupuesto de 4 MiB en la arena del jefe (37–38 chunks de 32).
- El jugador no usa plataformas dinámicas ni se sube al jefe.
- Rendimiento del combate con el jefe (peor fotograma 16,7 ms > 12 ms).
