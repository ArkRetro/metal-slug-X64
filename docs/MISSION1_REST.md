# Resto de la Misión 1: captura, mapa, colisión, cámara e inventario

> La integración en el runtime está en [MISION1_COMPLETA.md](MISION1_COMPLETA.md).

Este paso recupera, desde el arcade original en MAME, los datos del tramo que
sigue a la cámara 1920 (donde termina hoy la región jugable del port) hasta el
final de la misión: el pueblo con el edificio de cúpulas, la calle del mercado,
la zona del camión, el tramo del helicóptero y la arena del jefe. **No modifica
el runtime** (`src/`): produce capturas reproducibles, extractores, datos
validados y el inventario para planificar la integración. El resumen numérico
está en [mission1-rest-validation.json](mission1-rest-validation.json).

## Captura reproducible

`tools/capture_mission_rest.lua` (MAME 0.264, `mslugx`, BIOS `euro`) juega la
misión completa. `tools/run_capture_rest.sh SALIDA` lo lanza sin vídeo ni sonido
con `cfg`/`nvram` nuevos. El plan de entradas: correr a la derecha disparando,
saltar cada 2 s, granada cada segundo; si la cámara no se mueve durante 4 s
(una parada de scroll que espera un encuentro), barrer la pantalla a izquierda
y derecha alternando disparo hacia arriba. El plan solo **lee** la cámara y la X
proyectada del jugador.

Las ayudas de referencia son las mismas de `capture_advance.lua` y nunca se
incorporan al juego N64: temporizador `0x1013DE`, bit de invulnerabilidad del
jugador (`0x101523 |= 0x40`), `0x101549`, `0x101288`, arma `0x10C71E=3` y sus
contadores de munición/bombas. No se escribe estado de nivel, guion, enemigos
ni colisión.

Salidas: `trace.csv` (cada fotograma: progreso de cámara `0x10E99E`, palabras
`+84/+86` de los cuatro objetos de capa, posición de cámara `0x10E98E/92`,
temblor `0x10E9CC`, parada `0x10E9BC` y operandos, autoscroll, puntero de la
lista de comandos de mapa, jugador, guion y grupos), `actors.csv` y
`actorstate.csv` (recorrido de la lista de actores), `script.csv`,
`layers.csv` (cabeceras de capa), `platforms.csv` (plataformas de objeto),
`lspc.csv`/`lspc-writes.csv` (registro LSPC) y volcados VRAM/RAM/paleta/PNG cada
30 fotogramas. Opciones: `MSX_REST_TRACE_WINDOWS` (taps de escritura en la
colisión y el puerto VRAM), `MSX_REST_PREV_DUMPS=1` (VRAM del fotograma
anterior a cada volcado) y `MSX_REST_DEBUG=1` (sondas del depurador: apariciones
en `0x01A132`, dibujos en `0x004406`, rutinas de mapa).

Reproducibilidad medida en ocho partidas: hasta el fotograma 12684, dentro
del combate con el jefe, todas las partidas sin depurador tienen trazas
idénticas de cámara y guion (la X del jugador difiere en dos fotogramas
aislados entre algunas). Desde 12684 aparecen dos desarrollos del combate, también
entre dos partidas con las mismas opciones (`run8` y `run9`), así que el
emulador no es determinista ahí; el mapa, las variantes y la colisión extraídos
de `run8` y `run9` son idénticos (salvo el número de observaciones por celda).
Todas terminan la misión (guion
`0x2D7132`, actor del guion retirado en 13635, guion de la Misión 2 en 14482).
Las rejillas de colisión de tres partidas son idénticas byte a byte. Con
`-debug` la partida es otra, también estable (difiere desde el fotograma 2333).

## Guion de nivel

`tools/decode_level_script.py` decodifica sin ejecutar nada el guion completo
de la misión desde la ROM (`0x2D6314..0x2D7132`, banco 1 del P2) con la
gramática de los recorridos `0x019F7E` (ops bloqueantes 0..3), `0x01A0A6`
(ops 6..0x11), `0x01A372` y las condiciones de `0x01A5CA`: 178 sentencias de
primer nivel y 125 registros de aparición ([data/mission1_script.json](../data/mission1_script.json)).
Todas las posiciones de reposo del actor del guion (`0x101414 +6E`) observadas
en las capturas son sentencias bloqueantes decodificadas.

Ops que el intérprete nativo de la rama `work-soldiers` todavía detiene y que
este tramo usa: **op 0 con parada** (`0x0C7B0C`: `0x10E9BC` = parada, operandos
`0x10E9C0..C6` = desplazamientos de los límites de cámara respecto al progreso,
`0x8000` = sin cambio), **op 0x0C** (`0x0C7C12`, libera la parada), **op 0x0D/0x0E**
(`0x0C7B4E/0x0C7BBA`, autoscroll a 3 px/fotograma en la dirección `0x10E9B6`) y
las **condiciones 0x0B/0x0C** (posición media de los jugadores frente a
`0x10C330`, fijada por la op 0x10).

Paradas del tramo: 1920 (operandos `0,0,0,32`: la vista sube a altura 32 y
espera a que los grupos 0..7 lleguen a cero), 2240→2336 (autoscroll y parada
del camión), 3192 (helicópteros) y 3376 (arena del jefe, operandos `56,·,144,·`).

### Lista de comandos de mapa/cámara

`tools/decode_map_commands.py` decodifica la lista del intérprete `0x0C83E6`
(puntero `0x10E964`) que empieza en `0x0B57BE`: esperas de progreso 944/1008/
1024/1920/2304/3520, límites de cámara, carga del mapa delantero, la llamada
condicional `0x0B58CC` en 1920, y en 2304 la orden de sonido 3 (`0x002802`) y la
carga de paleta `0x0B56CA` (`0x009D8C`), que cambia el color de fondo de
`0x2337` a `0x1EA4`. La espera de 3520 nunca se alcanza porque el guion detiene
el progreso en 3376.

## Mapa del resto y su validación

`tools/extract_mission_map.py` se amplió sin cambiar su comportamiento con las
capturas anteriores (el mapa de la región se revalida con los mismos 122
fotogramas y números):

- `--frames`: excluye el fundido final (desde 13770 cambian 1400 colores).
- Con `trace.csv`, cada transformación medida en los SCB debe coincidir con un
  valor de RAM de los cuatro últimos fotogramas: resuelve la ambigüedad de 16 px
  del temblor del derrumbe (fotograma 6900 daba 42 en lugar de 26).
- Destello blanco (6960): fondo y todas las paletas de fondo a `0x7FFF`; se
  registra y no se mezcla.
- Con `layers.csv`, si el origen de columnas `+32` de una capa cambia sin cambiar
  su puntero de mapa `+2E`, las celdas se guardan en coordenadas del mapa. Es el
  caso del cielo tras el derrumbe: `0x0B58CC` llama a `0x0C6F82` con `+2` sobre
  `0x10E83E`, y su contenido queda desplazado 32 px. Las 105 celdas que antes
  aparecían como variantes quedan explicadas sin excepción.
- Con `lspc.csv`/`lspc-writes.csv` se guarda el contador de autoanimación y si
  el bit 3 de `0x3C0006` la desactiva: el juego la desactiva en el fotograma 764
  y la reactiva a velocidad 2 en 7226 (los SCB se muestran tal cual). El
  validador usa ese estado en lugar de `frame & 7` cuando la captura lo tiene.
- Con `prev/vram<f>.bin`, el validador excluye también los píxeles opacos de la
  lista de sprites anterior: en fotogramas cargados la pantalla aún muestra
  sprites de esa lista (comprobado en 12120: los 200 píxeles discrepantes están
  todos cubiertos por la lista de 12119).

Resultado (captura `run8`, fotogramas 5100..13740): **2680 celdas, 49 paletas,
62 variantes de coordenadas, cámara máxima 3520**. Validación exacta de SCB:
288 fotogramas, 19611648 píxeles de mapa, 208339 en variantes registradas,
**10252768 píxeles de MAME sin cubrir comparados con ≤1 por canal** y 301696
excluidos por la lista anterior. Las 62 variantes son exactamente el parche del
edificio destruido: `decode_map_commands.py --map` comprueba que la variante
posterior de cada una es la celda del submapa de ROM `0x0B6AD2` (datos en
`0x206480`, banco 2) que `0x0B6882` copia con `0x0C70FC` en la columna `0x9F`,
fila `0x0B`, de las capas `0x10D7B6` y `0x10E8C6`.

Eventos de mapa identificados:

| Evento | Disparo (ROM) | Efecto |
| --- | --- | --- |
| Derrumbe del edificio de cúpulas | muerte de `0x05486A` → `0x0552CC` → `0x0B595A` crea el actor `0x0B5972`/`0x0B5982` → `0x0B58CC` | paleta/gráficos `0x0B530C` (`0x01AA28`), cielo +2 columnas, recarga de capas, mapa delantero `0x0B53A8/0x0B54D6`, la rejilla de colisión se borra (`0x0C702A`) y se recarga con los mismos valores |
| Edificio destructible `0x0B6566` | muerte del edificio (`+5F` bit 7) → `0x0B66F2` | parche de fondo `0x0B6882` y borrado de colisión (abajo) |
| Cambio de fondo | comando de mapa en progreso 2304 | paleta `0x0B56CA`, fondo `0x2337`→`0x1EA4` |
| Destello | explosión del derrumbe | un fotograma con todo el fondo blanco |

## Colisión

`tools/extract_region_collision.py` acepta, además del vehículo, sellos de la
rutina original **`0x0C88B0`** (escritura en `0x0C897E`) cuando la captura trae
la traza de escrituras: cada escritura del sello debe reproducirse exactamente,
en orden, por un grupo de plantillas de ROM en una única posición
(`tools/collision_stamp.py`, modelo exacto de `0x0C88B0`; reproduce también
el sello del vehículo desde la plantilla `0x029BF4`). Las celdas escritas
conservan su valor previo en la tabla estática. `--layer-camera` toma el
recuadro observado de la posición de la capa activa: dentro de la parada 3376
la cámara está entre 3432 y 3520, no en el progreso.

Resultado: **480×32 celdas (3840 px), 679 no vacías; las 280 primeras columnas
son idénticas a `collision.json`**; 389 volcados; 39 celdas mutables, todas
explicadas; ninguna desconocida. Eventos:

- vehículo (`0x029BF4`, ancla 146,8), ya documentado;
- `building_raise` en la construcción del hijo `0x0B680E` del edificio
  `0x0B6566`: plantillas `0x0B69F8` (tejado, 16 celdas forma 0xB) y `0x0B6A2C`
  (pared, 12 celdas forma 0xF), ancla 329,8 (X=2632);
- `building_clear` cuando muere el edificio (`0x0B685A`): plantillas `0x0B6A12` y
  `0x0B6A42` escriben celdas vacías con material (`0x10`).

Plataformas de objeto (ranuras `0x10C06E + n*0x40`, `0x0140C0`/`0x013F1E`): el
edificio `0x0B6566` (`0x0B6A60`), el camión `0x033C98` (`0x033C00`,
`0x033924`, `0x033A00`, `0x033ADC`, referencias en `0x0342xx/0x033F44`) y el jefe
(`0x0E93B2`, `0x0E94AC`, `0x0E95A6`, desde `0x0D599E/0x0D5D76/0x0D65FE`). No
forman parte de la rejilla. **Hallazgo en la región ya integrada:** el toldo y
el puesto del mercado (`0x0400AA`, `0x03FC5A`) se construyen con sellos
(`0x0E0932`, `0x0E091E`) que la tabla estática actual guarda como terreno, y el
código `0x08842C` (familia del constructor `0x087E36`) registra la plataforma
`0x0E4EDE` en la cámara 1394. No se ha
cambiado nada de esa región.

## Modelo de cámara medido

`tools/measure_camera_rest.py` comprueba fórmulas fijas fotograma a fotograma
(5300..13740, 8441 fotogramas; X = cámara mostrada `-(0x10C72E+84)`, P =
progreso, H = `0x10E992`, S = temblor `0x10E9CC`):

| Regla | Exactos | Distintos |
| --- | --- | --- |
| banco medio a velocidad completa = X | 8440 | 1 |
| cielo = (X+1)//2 | 8439 | 2 |
| objeto `0x10E8C6` = X | 8438 | 3 |
| Y medio = Y delantero | 8441 | 0 |
| Y delantero = 256 − ceil(H) − S (fotograma anterior) | 8370 | 71 (50 en temblor) |
| Y cielo = 264 − ceil(ceil(H)/2) − S (fotograma anterior) | 8375 | 66 (50 en temblor) |
| X = ceil(cámara X del fotograma anterior) | 8317 | 124 |
| \|X − P\| ≤ 3 sin operandos de parada | 3863 | 41 (alcance tras soltar una parada) |
| P+op1 ≤ X ≤ P+op3 con operandos | 4512 | 25 (entrada en la parada) |

Comportamiento vertical medido: altura 16 desde 1023; en la parada 1920 sube
a 32 a 1 px cada 2 fotogramas y vuelve a 16 al soltarse; durante el resto sigue
al jugador cuando salta (hasta 35) y vuelve a 16. En la arena del jefe la
cámara sigue al jugador entre 3432 y 3520. Rutina original: `0x0C7CA6`.

## Inventario desde la cámara 1920

`tools/inventory_mission_rest.py` usa la partida con depurador: cada aparición
es el registro op 7 ejecutado (sonda en `0x01A132`), su constructor de la tabla
`0x0197DC`, los actores que crea (cadena `+0C`), los descriptores que dibujan
(`0x004406`) y las paletas (tabla `0x1068EA`). Se ejecutaron 59 registros de 26
constructores, todos los que el guion contiene desde 1920. P = progreso de
cámara en la primera aparición (partida con depurador). Las etiquetas son
identificaciones visuales de los descriptores decodificados; los datos completos
están en [data/mission1_rest_inventory.json](../data/mission1_rest_inventory.json).

| Constructor | Identificación visual | N | P | Registro | Rutinas | Descr. | Teselas | Nativo |
| --- | --- | --- | --- | --- | --- | --- | --- | --- |
| `0x01913C` | ajuste del temporizador `0x1013DE` (actor de un solo paso) | 4 | 1920 | `0x2D6A3C` | 3 | — | — | no |
| `0x05486A` | edificio de cúpulas (objetivo de la parada 1920) | 1 | 1920 | `0x2D6A48` | 1 | 16 | 25 | no |
| `0x04115A` | soldado tumbado que entra por un lateral | 2 | 1920 | `0x2D6A86` | 3 | 18 | 105 | no |
| `0x09A67E` | emisor fijo en (2072,136); su único descriptor es un cohete | 16 | 1920 | `0x2D6B48` | 6 | 1 | 3 | no |
| `0x09F682` | soldado en camello | 2 | 1939 | `0x2D6C44` | 19 | 66 | 489 | no |
| `0x031D92` | prisionero | 1 | 1982 | `0x2D6C76` | 5 | 8 | 104 | no |
| `0x0AF17E` | soldado rebelde | 1 | 1982 | `0x2D6C86` | 6 | 30 | 160 | **sí** (`neo_soldier*`; IA completa en `work-soldiers`) |
| `0x03458E` | Camel Slug (vehículo del jugador, cartel IN) | 1 | 2029 | `0x2D6D14` | 13 | 57 | 356 | no |
| `0x0B6566` | edificio destructible con barriles | 1 | 2238 | `0x2D6D3E` | 16 | 80 | 858 | no |
| `0x0395B6` | objetos pequeños a Y=144 (descriptores de 1–3 teselas) | 3 | 2336 | `0x2D6D84` | 8 | 87 | 356 | no |
| `0x033C98` | camión con lona | 1 | 2336 | `0x2D6E1E` | 12 | 58 | 425 | no |
| `0x0A8C62` | generador de espadachines (polvo y 23 actores hijos) | 1 | 2336 | `0x2D6E3C` | 34 | 149 | 728 | no |
| `0x08C37C` | objeto de prisionero (H) | 1 | 2336 | `0x2D6E60` | 2 | 11 | 52 | no |
| `0x099862` | prisionero atado | 1 | 2336 | `0x2D6E6E` | 14 | 50 | 309 | no |
| `0x092A32` | vehículo con cañón | 1 | 2336 | `0x2D6E80` | 19 | 82 | 662 | no |
| `0x099DCC` | prisionero | 3 | 2486 | `0x2D6ED0` | 18 | 61 | 310 | no |
| `0x09D862` | espadachín árabe (variante 3) | 8 | 2553 | `0x2D6EFC` | 19 | 89 | 619 | no |
| `0x09D846` | espadachín árabe | 2 | 2557 | `0x2D6F28` | 16 | 42 | 278 | no |
| `0x0403A0` | cohete | 1 | 2565 | `0x2D6F56` | 3 | 2 | 7 | no |
| `0x09D876` | espadachín árabe en altura | 1 | 2605 | `0x2D6F7E` | 8 | 12 | 84 | no |
| `0x05ED84` | vehículo lanzacohetes | 2 | 2685 | `0x2D6FD6` | 6 | 53 | 1332 | no |
| `0x0B63A0` / `0x0B639A` | toldos rayados del mercado | 1+1 | 2924 | `0x2D7046/50` | 1 | 4 | 113/60 | no |
| `0x04857C` | helicóptero | 1 | 3189 | `0x2D7092` | 23 | 81 | 732 | no |
| `0x0A8C46` | espadachines de túnica blanca | 1 | 3219 | `0x2D70D8` | 10 | 25 | 199 | no |
| `0x0D5976` | **jefe**: blindado con patas | 1 | 3302 | `0x2D70FC` | 86 | 347 | 3623 | no |

Fuera de esta tabla: `neo_vehicle` (main) cubre el blindado `0x02916E` del
tramo anterior; `neo_script`/`neo_world` (rama `work-soldiers`) cubren el
intérprete del guion hasta `0x2D651A` y solo el constructor del soldado.

### Jefe `0x0D5976`

Registro `0x2D70FC`, grupo 6, X=3720, Y=64, palabras `3712, 3718`; aparece en
el progreso 3308 y la parada 3376 fija la arena. Nodo principal (partida con
depurador): vida `+4E` 5000 al empezar. Fases observadas por rutina:
`0x0D5A92` (espera), `0x0D5AD4`, ciclo `0x0D5BBA → 0x0D5BF8 → 0x0D5B1E` mientras
la vida baja de 5000 a 3580; con 3480 pasa por `0x0D5C2C`, `0x0D5CD2`,
`0x0D5CEC`, `0x0D5D04`, `0x0D5D24`, `0x0D5D5C` (el hijo `0x0D605E`→`0x0D60B4`
cambia al mismo tiempo) y queda en `0x0D5D84`; al morir `0x0D5E5C`, `0x0D5E88`,
`0x0D5EA6`, `0x0D5EBC`, `0x0D5EDA`. Partes hijas: `0x0D6882/0x0D691E/0x0D697A/
0x0D687C` (ciclo de arma), `0x0D6568`, `0x0D64D8`, `0x0D6254`, `0x0D6168`,
`0x0D7020/0x0D6004` (proyectiles), `0x0D6AC6..0x0D6C66`. Descriptores del
cuerpo: `0x1819C0`, `0x180674`, `0x181F4C`, `0x1807D2`, `0x180B52`,
`0x180040..0x18016A`, `0x17FE30/0x17FEA8`, `0x183C8E/0x183CC4` (95 descriptores
de 64×64 px o más). Paletas lógicas principales: `0x0357`, `0x0358`, `0x0359`,
`0x0347`, `0x020D`, `0x02EC..0x02EE`, `0x031A`. Plataformas `0x0E93B2`,
`0x0E94AC`, `0x0E95A6`.

## Presupuesto de escena

`tools/estimate_mission_budget.py` une el mapa de la región y el del resto (las
1061 celdas solapadas coinciden), añade las variantes, ejecuta `pack_scene.py`
sin cambios en un árbol temporal y la regla de `check_scene_budget.py` hasta
3520. Región actual: 296 páginas, 101 paletas, 37 chunks, 606208 bytes
decodificados, 388352 empaquetados, peor conjunto 25 en 1056. **Misión
completa (solo mapa y actores ya empaquetados): 434 páginas, 123 paletas, 55
chunks, 901120 bytes decodificados, 679552 empaquetados; peor conjunto de
trabajo 27 chunks en la cámara 1920.** Cabe con evicción en la caché de 4 MiB
(32 ranuras) y entero en la de 8 MiB (55). Los enemigos nuevos y el jefe no
están en el pack: sus descriptores usan 9972 teselas únicas de 16×16 (jefe
3623), 1276416 bytes en CI4, que obligan a cargarlos por tramos.

## Orden de integración recomendado

1. Intérprete del guion: ops 0 (parada con operandos), 0x0C, 0x0D/0x0E y
   condiciones 0x0B/0x0C; lista de comandos de mapa (`0x0B57BE`) con paleta de
   fondo en 2304. Sin esto no se puede pasar de 1920.
2. Cámara: paradas, límites relativos al progreso, subida vertical en 1920,
   autoscroll 2240→2336 y el seguimiento entre 3432 y 3520.
3. Mapa del resto en el pack (cielo +32 px tras el derrumbe, parche del
   edificio, fondo `0x1EA4`) y colisión de 480 columnas con los sellos.
4. Edificio de cúpulas `0x05486A` y el cambio de mapa `0x0B58CC` (bloquea 1920).
5. Enemigos de infantería restantes (`0x04115A`, `0x09D846/62/76`,
   `0x0A8C46/62`, `0x09A67E`) sobre la base de `neo_soldier`.
6. Edificio `0x0B6566` (sellos y parche), camión `0x033C98` y sus plataformas,
   vehículos `0x09F682`, `0x092A32`, `0x05ED84`, helicóptero `0x04857C`.
7. Prisioneros y objetos (`0x031D92`, `0x099862`, `0x099DCC`, `0x08C37C`) y la
   Camel Slug `0x03458E`.
8. Jefe `0x0D5976` con sus plataformas y el final de misión.

## Reproducir

```sh
tools/run_capture_rest.sh /ruta/run          # MSX_REST_PREV_DUMPS=1 MSX_REST_TRACE_WINDOWS=6980-7030,7150-7340
MSX_REST_DEBUG=1 MSX_REST_DRAW_FROM=5000 MSX_REST_FIRST_DUMP=5000 tools/run_capture_rest.sh /ruta/run-debug
python3 tools/decode_level_script.py /ruta/mslugx.zip --output data/mission1_script.json
python3 tools/decode_map_commands.py /ruta/mslugx.zip --map filesystem/mission1/rest/map.json
python3 tools/extract_mission_map.py /ruta/run --frames 5100:13740 --allow-backdrop-timeline --output filesystem/mission1/rest  # incluye swapped_cells (escombros 0B53A8)
python3 tools/validate_mission_map.py /ruta/run --model filesystem/mission1/rest/map.json --output build/mission-rest-validation
python3 tools/extract_region_collision.py /ruta/run --rom /ruta/mslugx.zip --frames 2100:13740 --layer-camera \
  --expect-prefix filesystem/mission1/collision.json --output filesystem/mission1/rest/collision.json
python3 tools/measure_camera_rest.py /ruta/run/trace.csv
python3 tools/inventory_mission_rest.py /ruta/mslugx.zip /ruta/run-debug --output data/mission1_rest_inventory.json
python3 tools/estimate_mission_budget.py
make check-core && make check
```

Capturas locales: `/home/user/captures-m1rest/run8` (canónica), `run4`/`run6`
(reproducibilidad), `run5-debug` (inventario). No se suben a GitHub.

## Límites

- El mapa y la colisión del resto son datos para la integración; el runtime
  sigue terminando en 1920.
- Las identificaciones visuales de la tabla no son nombres del original. La
  atribución de actores hijos sigue la cadena `+0C`; con nodos reutilizados
  puede asignar algún dibujo suelto a otro tipo (p. ej. una paleta `0x046E` en
  `0x031D92`).
- No se ha recuperado la semántica de los descriptores de plataforma de objeto
  ni la IA de ningún enemigo nuevo; las fases del jefe son secuencias de rutinas
  observadas, no nombres funcionales.
- La cámara vertical durante el seguimiento del salto se describe midiendo, no
  con la rutina completa `0x0C7CA6`.
