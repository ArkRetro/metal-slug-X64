# Misión 1: actores del resto de la misión (rama `work-m1actors`)

Continuación de [MISION1_COMPLETA.md](MISION1_COMPLETA.md): las apariciones
del guion que seguían "externas" se portan por familias, cada una como
traducción manual de sus rutinas originales sobre el actor genérico
(`neo_gactor`, registro `+00..+BF` con la disposición original) y validada
contra una traza de familia de MAME.

## Prueba genérica de familias

`tools/extract_family_fixture.py` (traza `boss-trace.txt` de
`tools/run_capture_rest.sh` con `MSX_REST_FAMILY_ROOTS`) →
`data/<familia>_fixture.json.gz` → `tools/compile_family_fixture.py` →
`tools/test_neo_family.c` (en `make check`, variable `FAMILIES`).

- Bucle cerrado: el nodo raíz parte de sus bytes grabados y no se recarga;
  los demás nodos existen solo si el código nativo los crea. Cada creación
  nativa se asocia al siguiente nodo grabado con el mismo creador y la misma
  primera rutina (en orden de creación: `0x0040BC` inserta detrás del
  creador, así que los hermanos de una misma actualización aparecen en orden
  inverso). Una dirección reutilizada tras un borrado es otra instancia (el
  borrado se reconoce por la rutina `0x003F1E` o por un salto de rutina).
- Entradas por actualización: las palabras de RAM que leyó el original (raíz
  `10E968`; los objetos de capa `10C72E/10D7B6/10E8C6` tienen la misma
  posición que la raíz en la Misión 1, comprobado en los volcados de RAM),
  jugador 1 (`+3C/+40`), banderas del guion `10C44E/10C44F`, `10B954/10B95C`,
  época, cursor y palabras del anillo del RNG, y los bytes que otros actores
  (disparos del jugador, `0x0049E8`) escribieron en el nodo entre dos
  actualizaciones.
- Comparación tras cada actualización: rutina y bytes `+10..+AF` salvo
  ranuras de paleta (`+14`, `+16`, `+19..+1C`), puntero de animación
  (`+20..+23`), caché de posición (`+44..+4B`) y ranura de registro de
  impactos (`+58/+59`, la lista nativa es otra); las etiquetas `+0C/+38` se
  comparan a través de la correspondencia de nodos. Un borrado debe borrar.
  Las variables globales vigiladas (modo y `vy` de cámara `10E95A..10E963`,
  temblor `10E9C8`, mata-todo `1067E3`, destello `109944`, bloqueo del
  jugador `10E9E3`, manejadores `10E9F8..10E9FF`) deben tener tras cada
  actualización los bytes que escribió el original.
- El contacto con el jugador (clase 0) se toma del resultado de la
  actualización (el original borra al atacante en ella): el rectángulo del
  jugador no está en la traza.

## Edificio de cúpulas `0x054904` (`src/neo_dome.c`)

Registro `0x2D698A` (progreso 1600) crea el actor principal `0x054904`:
tres cúpulas `0x054C2C`, dos estandartes `0x054B1C`. La entrada `0x05486A`
(registro `0x2D6A48`, grupo 7) pone `10C44E = FF` y `10C44F` = su grupo y se
va (el mundo lo hace directamente, sin nodo). Con esa señal: modo de cámara 2
(`0x0C7B48`), las cúpulas suben (la cúpula 0 escribe `10E960`, la `vy` del
autoscroll: la vista sube 16 px), se convierten en torretas (escotilla
`0x054F60`, lanzador `0x055056`, cierre `0x05518A`, misiles `0x055638` con
zoom por altura, persecución `0x0366AC`), y con las tres destruidas el
derrumbe: bloqueo del jugador (`0x0C8CD6`, manejadores `0x054B18` por
`0x0CA2E0`), escombros, destellos blancos `0x0551E0` (`0x009BB0`), temblor
`0x0C801A`, mata-todo `0x004BFE`, cambio de mapa (`0x0552CC` → `0x0B595A` →
`0x0B5972/0x0B5982` → `0x0B58CC`) y liberación.

Resultado: **35 nodos, 7216 actualizaciones en bucle cerrado (5192..7245),
1005340 bytes iguales**, 6235 dibujos, escrituras globales idénticas.
Los caminos que irían a la comprobación del cartucho (`0x2FFFE0`) terminan
en `0x004188` (borrado) en el hardware original: es lo traducido.

En el mundo:

- `004BAC`: con `1067E2` (copia de `1067E3` hecha por `0x0046D4` en el
  actor de misión) cada cuerpo que se registra con clase distinta de 1 muere
  (`+4E = 0`, `+5F` bit 7, `+5A = 6`); a los soldados se les aplica antes de
  su actualización (el original lo hace dentro, al registrarse).
- Cambio de mapa: `0x0B5972` enciende el destello, `0x0B5982` aplica
  `0x0B58CC` en la siguiente actualización y lo apaga. El render sustituye la
  fachada (capa 3, mapa `0x0B5392` en la columna `0x74`) por los escombros
  del mapa `0x0B53A8` y desplaza el cielo 32 px (origen de columnas +2,
  celdas extraídas en coordenadas de mapa). El destello pinta el fondo de
  blanco.
- Escombros `0x0B53A8`: tras el cambio, el objeto de capa `10C72E` dibuja
  desde el SCB 65 (palabra `+2C` de su cabecera: `0x68` → `0x40`), no desde
  el 105; el extractor suponía el rango fijo 105..136 y daba la capa por
  inactiva, así que los escombros nunca se extrajeron (rectángulo de color de
  fondo en su lugar). `tools/neo_scene.py` toma ahora la base de `+2C`
  (`layers.csv`) y `tools/extract_mission_map.py` guarda aparte
  (`swapped_cells`) las celdas de una capa cuyo puntero de mapa difiere del
  primero observado: **112 celdas** de `0x0B53A8` (run8, 5100..13740, una sola
  variante por celda); las 2680 celdas, 62 variantes y 49 paletas del mapa
  no cambian. `tools/pack_scene.py` las empaqueta como
  `scene_collapse_cells` y el render las dibuja como capa 3 en lugar de la
  fachada una vez caído el edificio.
- Prioridad: el bit 2 de `+15` pasa al atributo del sprite (`0x00A03E`
  conserva `d1 & 7`); las cúpulas lo tienen y en MAME quedan detrás de la
  fachada. El render dibuja las capas 0..2, los sprites con ese bit, las
  capas 3..4 y el resto.
- Zoom `+12/+13` de los actores genéricos (misiles, estandartes): tamaño y
  desplazamientos escalados respecto al punto del actor (aproximación del
  encogido del hardware, no validada por píxel).
- `0x012022` (estandartes): `10B954 = 0x88`, `10B95C = 0x40` en la traza →
  bit 2 borrado.

## Edificio destructible `0x0B6566` (`src/neo_barrel.c`)

Vida por rango `0x2D3A5C`, tres estados al reducirse a la mitad (programas
`0x2A89B8/C4/D0`, escombros `0x0B69B6`, seis humos `0x01D488`), destrucción
`0x0B66F2` (mata-todo, seis piezas `0x08732C` —externas, pero se consumen sus
tres números aleatorios—, 16 humos, escombros, parche `0x0B6882`, ruina
`0x2A89DC`), capa superpuesta `0x0B6770` y tejado `0x0B680E`: sellos de
colisión `0x0B69F8/0x0B6A2C` (`0x0C88B0`, ahora genérico en
`neo_collision_stamp`), objeto dinámico `0x0B6A60` (`0x0140C0`) y borrado
(`0x0B6A12/0x0B6A42`, `0x013F1E`).

Resultado: **3 nodos, 226 actualizaciones (7163..7499), 31360 bytes
iguales**, 4 sellos. Encontrado al portarlo: `0x01448C` termina con
`0x01AD8C` (profundidad `+1E`) cuando `+15` bit 6 está activo; las consultas
al núcleo conservan el objeto de capa del padre (`10D7B6` a partir de la
cámara 1023, como responden las trazas).

Parche de mapa: las 62 variantes de coordenadas del mapa del resto son el
submapa `0x0B6AD2`; `tools/pack_scene.py` empaqueta la variante posterior
aparte (`scene_patch_cells`) y el render la dibuja en lugar de la celda
intacta cuando el edificio cae.

## Carro con lona `0x033C98` (`src/neo_wagon.c`)

Registro del guion en la cámara 2336 (fotograma ~6750 en las capturas).
`0x033C98`: paletas `0x121/0x122`, salud por rango `0x2D3E5C`, avanza a
`fffd4000` hacia el centro (`+62`, signo por `+3C < 0xA0`) hasta que la
distancia `+7A` pasa `+70`; `0x033D8A/0x033DB4` frena (las pruebas de signo
literales del original lo paran en la misma actualización salvo que la
velocidad cruce cero), abre la lona (programa `239E30`, retrollamada
`239ECE`: hijo `0x034022` y `+81`), y espera con caja de impacto
(`0x004C24`) mientras `+74` cuenta el tiempo invulnerable (`+80`: salud
restaurada). Destruido (`+5F` bit 7): `0x004192` borra los nodos más
profundos (servicio nuevo `kill_children` del entorno), 16 explosiones
`0x01DBAA`, seis piezas `0x034094` tras el actor del guion (`0x00411C`),
escombros `0x01DA40` (lista `0x033C46`), `0x019DFC`, y cae en `0x033F2E`
(restos con plataforma `0x033ADC/0x033B6E`; con `+72 = 0` se va tras `+7E`
actualizaciones por el camino de la comprobación del cartucho → `0x004188`).
El cuerpo es un objeto de colisión dinámico en cada actualización
(`0x034260`, mapas `0x033924/0x033992` cerrado, `0x033A00/0x033A6E`
abierto) y el hijo `0x03415A` lleva el del techo (`0x033C00/0x033C12`).

Prueba de familia: `data/truck_fixture.json.gz` (MAME `trk3`, raíces
`33c98`, rangos `33c98-35a00`, `39500-39d00`) con `--tree` (solo el nodo
raíz y sus descendientes; los nodos `0x0395xx/0x0356xx` del mismo fichero
los crea el subguion del espadachín `0x0A8C62`, otra familia).
**3 nodos, 621 actualizaciones en bucle cerrado (6752..7558), 86800 bytes
iguales**, 2 nodos borrados por `0x004192`, 23 hijos fuera de la familia.
Las piezas `0x034094` (creadas tras el actor del guion) no están en la
traza de familia: traducción literal sin validar por la prueba.

Cambios en la prueba genérica: el extractor guarda también las lecturas del
objeto de capa `10D7B6` (`+14..+24`), y la prueba usa esa posición para las
capas y el terreno del núcleo cuando la traza la tiene (el carro se apoya en
`10D7B6`, cuya Y difiere de la raíz cuando la cámara sube). Las trazas
anteriores (cúpulas, tienda) no la tienen y siguen usando la de la raíz.

Gráficos: `tools/export_rest_sprites.py ... trk3@33c98-34200`: 312
descriptores y 15 paletas; validación contra 98 volcados (11 nuevos del
carro): **1326539 píxeles iguales, 4575 distintos** (los mismos 4575 de
las cúpulas tapadas por la fachada; ninguno del carro).

### El jugador sobre plataformas dinámicas

El cuerpo original que pisa un objeto dinámico (`0x0140C0`) toma ese objeto
como padre (`+38`) y guarda su posición relativa (`+28/+2C`), así que se
mueve con él. `msx_game_update` hace ahora lo mismo con el núcleo del mundo:
al caer, la búsqueda de suelo `0x013DB2` (`neo_kernel_floor`) desde los
pies; si el primer suelo es un objeto dinámico, el jugador aterriza en él
(`ride`, posición relativa al objeto); cada paso lógico los pies siguen al
objeto (posición proyectada + relativa − terreno), anda sobre su superficie
plana y se cae si deja de encontrarlo (o si el objeto desaparece). El
terreno estático sigue con `neo_collision`. Límites: sin paredes del objeto
(el jugador del port tampoco las tiene con la rejilla estática) y con un
paso de retraso (el jugador va antes que el objeto en la lista, como en el
original). En el recorrido nativo el jugador se sube a las plataformas del
jefe (33 veces): el combate dura más (40508 pasos, límite de la prueba 60000)
y la altura máxima de 30 se comprueba antes del jefe. El carro no llega a
pisarse en el recorrido (su techo está por encima del salto).

## Gráficos

`tools/export_rest_sprites.py` acepta varias capturas (`dir:nodos` o
`dir@lo-hi` por rango de rutinas): 268 descriptores, 12 paletas. Validación
contra VRAM de 87 volcados: **1260561 píxeles iguales, 4575 distintos**; los
distintos son exactamente la parte de las cúpulas tapada por la fachada (la
prioridad descrita arriba; el validador no dibuja el fondo). Excluidos:
píxeles con dos sprites de la familia (el orden del hardware decide) y
sprites con zoom.

## Música

El lector VADPCM fijado solo repite ficheros completos. `tools/pack_game_audio.py`
genera ahora `music_mission1_intro.wav64` (muestras `[0, 203280)`) y
`music_mission1.wav64` (cuerpo del bucle, repetido entero); `msx_audio`
arranca el cuerpo en el mismo canal con un evento del mezclador exactamente
al acabar la introducción (`mixer_add_event`, retardo = longitud × 32000 /
24000). El punto de bucle medido (5,97 s + 78,79 s) queda así exacto.

## Recorrido

`tools/test_gameplay.c` y `tools/test_mission_native_ares.py`: en la parada
1920 el jugador recorre las cúpulas disparando hacia arriba hasta el
derrumbe; en la parada 2336 retrocede y dispara a la tienda hasta su
destrucción.

## Pendiente

Ver la tabla de cobertura de MISION1_COMPLETA.md. Captura preparada para el
siguiente paso: `captures-m1full/trk2` (familias `0x033C98`, `0x0A8C62`,
`0x09D846/62/76`, fotogramas 6950..8600).
