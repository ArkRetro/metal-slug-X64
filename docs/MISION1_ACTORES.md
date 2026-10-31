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

Render: desde las páginas altas (columnas del mapa fusionadas en un solo
dibujo), `hidden()` se evaluaba solo en la primera celda de cada columna
fusionada; tras la destrucción de la tienda una columna que empezaba en una
celda del parche se saltaba entera (agujero del color de fondo en ares).
Ahora, con un reemplazo activo (parche en la capa 2, escombros en la 3), esa
capa se dibuja celda a celda.

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

## Espadachín de túnica sobre el carro `0x0A8C62` y subguiones (`src/neo_rest.c`)

`0x0A8C62` es la misma figura que `0x0A8C46` (arena del jefe) pero en el
suelo (`0x0146E8`, `+66 = 8838`): registro `0x2D6E3C` (progreso 2336, grupo
0, `+74 = 1`, `+7C = 6`). Su gesto invierte `10C606` y crea el subguion
`0x019E72` con el registro `[0x00087A + 4]` = `0x2D719A`, que ya no es un op
3 aislado: crea la criatura `0x035668` y, mientras el carro (grupo 7) vive,
tres espadachines `0x039590` (grupo 6) desde el carro. Cuando el grupo 6
queda vacío (y el subguion terminó: `+60` bit 7), `0x0A8FE0` con `+72 = 0`
→ `0x0A935A` (re-padre a la raíz `10E968`, `0x01AD52` = `0x01AC4C` +
proyección) → `0x0A9390` (se va andando) hasta salir de la vista.

- Subguiones generales: el actor genérico `0x019E82` ejecuta su registro
  con el mismo intérprete del guion (`neo_genv_t.subscript`, una instancia
  `neo_script_t` por subguion en `neo_world_t.subs`, con los recuentos de
  grupo del tick y el subguion como creador de sus apariciones). La tabla de
  apariciones importada incluye ahora los desplazamientos de los subguiones
  (`0x22C` → `0x039590`, `0x250` → `0x035668`).
- Prueba de familia `swordgen` (`data/swordgen_fixture.json.gz`, `trk3`,
  rango `a8c62-a9a00`, `--tree`): **1 nodo, 178 actualizaciones en bucle
  cerrado (6832..7296), 24780 bytes iguales**; el subguion queda fuera (su
  final llega como bytes externos `+60`). La prueba genérica toma ahora
  también los recuentos de grupo `10C326..` y el progreso de cámara `10E99E`
  leídos por el original.
- Detenciones explícitas: `0x0A8FE0` con `+72 ≠ 0` (combate de la figura, no
  alcanzado en las grabaciones), su muerte `0x0A93DC` y la rama `+93 = 0`.
- La criatura `0x035668` del subguion: ver abajo.

## Espadachines árabes del carro `0x039590/0x0395B6` (`src/neo_arab.c`)

`0x039590` espera el progreso `+74`, `0x0395B6` el bit `+74` de `10C606`;
luego (`0x03965C`) anda hacia los jugadores (`0x039694`, velocidad `+86` de
la tabla de rango `0x2D257C`, programa por `+90`), gira (`0x039796`), espera
(`0x0397CC`) y ataca (`0x03973A`: tajo `0x039B60`, cuya fila de ataque
`0x03A004` actúa mientras su `+91` está puesto y cuyo final pone `+60` del
espadachín). Todos los estados terminan en `0x039B16` (corte `0x03A014`,
registro `0x004C24`; un golpe mortal lleva a `0x039820`), que elige por el
tipo de golpe `+5A`, la dirección `+5B` y un `0x0029F2` entre la muerte en
trozos `0x0A93CE` (compartida con las figuras de túnica: lista `0x0E6220`,
trozos `0x0A9510..0x0A9814` movidos por `0x0A98BE`, caída `0x2A769C`) y el
vuelo `0x03993A` (espada `0x039C62`, sangre `0x040F88` externa, parpadeo
`+8E` y `0x004188`). `0x03C526` pregunta a `0x019240` por el bono de misión:
RAM `10C2F6` = 0 en la Misión 1 (volcados), no crea `0x03C62A`. RAM
`10FD8F` (opción BLOOD) es `MSX_BLOOD` (Makefile, 1 = ON por defecto):
`src/neo_blood.c` da la paleta de cada rama de `0x01B2BE` (muerte `0x0A9436`,
trozos `0x0A95F2/0x0A969A/0x0A9742/0x0A9876`); ON viene de
`tools/capture_blood.lua` (`data/blood_fixture.json`), OFF de las capturas
euro, que las pruebas reproducen con la opción en OFF.

Los espadachines salen del carro y su primer suelo es el objeto dinámico
del carro (`+38 = 10C22E`). La prueba genérica recibe ahora también las
lecturas de los objetos dinámicos `10C06E..10C291` (bloques de 64 bytes,
casillas) y con ellas reconstruye los objetos del núcleo y sus bloques de
posición; y los nodos que crea un actor fuera de la familia (aquí el
subguion) empiezan con sus bytes grabados en su primera actualización.

Prueba `arab` (`data/arab_fixture.json.gz`, `trk3`, rangos `39500-3a100`,
`a93c0-a9a00`): **22 nodos, 491 actualizaciones en bucle cerrado
(7003..7311), 65660 bytes iguales**, 22 borrados, 3 espadachines creados
por el subguion. Gráficos: 433 descriptores, 19 paletas; los 13 volcados
nuevos dan 646 píxeles distintos en 4 fotogramas (7080, 7140, 7200, 7260;
no investigado: posible orden/prioridad con el carro).

### Parada 2336 bloqueada (ROM de publicación)

El tajo `0x039B60` (programa `0x23C836`) llama en su orden 10 a `0x039FA6`
(`JSR 0x039ED0; SCC +92`, con a6 = el propio tajo): `+92 = FF` si un jugador
está en el rectángulo `0x03A04C` (segundo corte), 0 si no (el salto
condicional va al final del programa). La traza `trk3` nunca llegó a esa
orden, así que la llamada se detenía explícitamente: el tajo se borraba sin
poner `+60` del espadachín y este quedaba para siempre en `0x03975E` (grupo
6), y la espera "grupos 0..7 a cero" de `0x2D6E8A` no terminaba si el
espadachín quedaba fuera del alcance de los disparos. Traducida. Con reglas
de publicación (sin invulnerabilidad), `tools/test_gameplay.c` juega ahora
cuatro partidas con un "humano" aleatorio sembrado (anda, se para, se da la
vuelta, salta, apunta arriba al azar y bajo las cúpulas): las cuatro llegan
al final del guion y del jefe (14–19 muertes) y solo se detienen los dos
nodos documentados de la torreta. Sin la traducción la prueba falla (el tajo
se detiene). Paradas que exigen acción del jugador, como en la recreativa:
1920 (destruir las tres cúpulas: hay que disparar hacia arriba, el port no
tiene granadas) y 2336 (`0x2D6D78` espera también al grupo 1: hay que
destruir la tienda `0x0B6566` del borde derecho antes de que llegue el carro).

## Criatura `0x035668` (`src/neo_arab.c`)

Creada por el subguion (`0x2D719E`, `+6E = 0x80`): anda a `+70 = +6E << 8`
(programa `0x23B3C6` a la velocidad de la tabla `0x035658`) durante `+74 =
100..199` actualizaciones (`0x035A3A`; `+76` cuenta su edad hasta 1000), se
da la vuelta (`0x0359A4`, `0x035894`), y golpeada con tipo 1 o 9 (`0x004C24`)
salta (`0x03581C`). Prueba `creature` (`trk3`, rango `35600-35a00`): **1
nodo, 216 actualizaciones en bucle cerrado (7004..7528), 30100 bytes
iguales**; gráficos (paleta `0x93`, 9 volcados): 2044 píxeles iguales, 0
distintos.

## Prisionero `0x099862` (`src/neo_prisoner.c`)

Registro `0x2D6E6E` (progreso 2336, grupo 9 no contado, palabras `1, 6, 3,
320`). Atado (programa `29F6D0`, retrollamada `0x099BE6` que anda `+62` y
gasta el presupuesto `+76`; `+80` = índice de forcejeo) hasta que lo golpean
(`0x004C24` con acarreo → `0x099922`) o lo toca un jugador (`0x099C3C`:
rectángulo `0x099D7E`; `0x09AB38` cuenta el rescate en RAM `10C5FC/10C600`,
ahora `neo_ga_flags_t.rescued`). Suelto (`0x099956`) forcejea con la tabla
`0x099D86` y cada `+7A` actualizaciones (rango `2D0BB8`) da uno de sus `+72`
objetos (`0x099CEE` → `0x099D12` → `0x08CDB2`: objeto `0x08C374` tras el
actor del guion, tipo de la lista `[2D0BEC + 4·+70]` por el RNG; ≥ `0x5A`
no da nada). Tocado: saludo `29F9AC` (su retrollamada `0x099CCC` lanza el
regalo restante, `0x099C9C`), salto (`29FA22/29FA40`, `0x0143E4`) y carrera
(`0x099A8A`: velocidad `2D0B9E`, programa `29FA78`) hasta chocar con una
pared (`+5F` bit 0: 30 actualizaciones más) o salir de la vista
(`0x099AF8`). Sin presupuesto estando atado (`0x099A22`): un objeto y salto.
`0x01AF9A` con los dos jugadores vivos se detiene explícitamente (el port
tiene uno). No llama a `0x01B2BE` (sangre).

Prueba `prisoner` (`data/prisoner_fixture.json.gz`, MAME `pri1`, rango
`99862-99dc0`): **1 nodo, 124 actualizaciones en bucle cerrado
(7772..8073), 17220 bytes iguales**, liberado por un disparo, tocado,
saludo con un objeto lanzado (externo, sus números aleatorios consumidos),
carrera y salida. El prisionero anda sobre los restos del carro (objeto
dinámico `10C22E`, mapa `0x033ADC`) junto a la columna de pared de la
tienda caída: la prueba genérica acepta ahora `--stamp X,Y,PLANTILLA`
(sellos `0x0C88B0` de otra familia anteriores a la primera actualización;
aquí los cuatro de la tienda en `+28/+2C = 0A48/0040`, la posición del
tejado `0x0B680E` en la traza `barrel`). Comprobado en los volcados de RAM
de `pri1`: la columna `x = 2576` vale `1F` en 7200 y `10` desde 7500; sin
ella el bit 2 de `+5F` (bandera 8 del núcleo) no aparece en 7872.

Gráficos: `tools/export_rest_sprites.py mslugx.zip robe1 bld1@54800-55700
bld1@b6500-b6b00 trk3@33c98-34200 trk3@39500-3a100 trk3@a93c0-a9a00
trk3@35600-35a00 pri1@99862-99dc0`: 547 descriptores (los 491 anteriores
idénticos), paleta `0x9B`; los 10 volcados del prisionero: 7508 píxeles
iguales, 0 distintos salvo el de 8040 (892), cuya actualización es del
mismo fotograma del volcado (la lista mostrada es aún la anterior).

## Objetos `0x08C374/0x08C37C` (`src/neo_item.c`)

El tipo `+6E` elige un registro de `0x0E581A` (`0x08D2DA`, 0x18 bytes:
`+1` lista de objetos `10C5C2`, `+2` aparición `2861BC`, `+4` programa,
`+8` paletas, `+10` rutina de recogida, `+14` movimiento). `0x08C53C`: vida
`+70` (0x258 por defecto, FFFF = siempre; a 0 parpadea, `+5F` bit 3, 30
actualizaciones más), `+86`/`+7C`, el movimiento, `+78` actualizaciones de
espera y `0x08C0CC`: a tamaño completo (`+12 = FF`), rectángulo `0x0E561C`
contra las cajas de recogida de los jugadores (RAM `10C5EC/10C5F4`, que
escribe el código del jugador original; ahora `neo_genv_t.pickup_box`),
`0x00288A`; sale fuera de `0x0E5624` o, con `+8B`, sin creador.

- **Parada explícita: la recogida.** Las rutinas `+10` dan armas (`0x0C0928`),
  bombas o puntos (`0x0C0BF4`, `0x057948`) al jugador original, que el port
  no tiene: el objeto se retira y se cuenta (`item_pickups`, registro `MSX
  item taken`), sin efecto. `tools/test_gameplay.c` lo informa aparte.
- Solo son nativos los tipos con movimiento `0x08B9B2` (caída con gravedad
  `0xA000`, crece `+12/+13` de 8 en 8): `06, 18, 1C, 29, 2A, 2B, 2F, 4B, 4F
  (H), 51`; con otro movimiento (`0x08BA10`, `0x08BA38`, `0x08BAB2`) el
  constructor lo devuelve al mundo como externo. La lista `10C5C2` no tiene
  lector en el port y no se mantiene.
- Nativos en toda la misión (antes solo pasada `MISSION1_REGION_END`): la
  prueba de mundo de la región los compara ahora como actores del mundo (ver
  «Prisioneros `0x099DCC/0x099E06`»). Movimientos nativos: `0x08B9B2`,
  `0x08BA10` (tipo 1A, el del bombardero), `0x08BA38` (24) y `0x08BAB2` (las
  comidas 35, 38, 3A..3C, 3E, 43, 45..47, 58) y `0x08BD5A`; `0x08BAAA`
  (`0x08CCD4`) y los demás siguen devolviendo el objeto como externo.
  Retrollamadas nuevas de las comidas: `2881E6` (la 35 al caer suelta tres
  36 con velocidades del RNG), `28A61A`, `289A5E`, `2887C2`, `288786`,
  `28879A`, `288502`, `288DEC`, `288BEE`, `28904E`, `28906E`, `287D5C`,
  `28774C`, `28AB28`, `28AB36` (`0x002F1A`). `0x08C66A` ejecuta el programa
  con sus retrollamadas (antes sin ellas).
- Caja del jugador del port: la de pie de las capturas (`FFF8,0000,0010,
  0020`, 158 de 167 volcados de `pri1/trk3/bld1/robe1`; las otras son el
  tanque y otra postura que el port no tiene); sin jugador 2 (`8000`).
- No llama a `0x01B2BE` (sangre).

Prueba `hitem` (`data/hitem_fixture.json.gz` re-extraído de `pri1` con las
lecturas de `10C5EC..10C5FB`; rango `8c37c-8c700`): **1 nodo, 128
actualizaciones en bucle cerrado (7772..8081), 17780 bytes iguales** y la
parada explícita exactamente en la recogida de 8081 (rutina `0x08B59E`, H).
La prueba genérica acepta `--stop FOTOGRAMA,RUTINA` (la actualización de
ese fotograma debe detenerse en esa rutina; las posteriores se descartan).
Gráficos: programas de los tipos nativos y paletas `0x37D, 0x3A6, 0x380,
0x37E, 0x3A7, 0x3AC, 0xD0, 0x103` (`0x3AC` de los volcados de
`captures-boss/runA`, sin validar nodos allí); los 9 volcados del H:
3636 píxeles iguales, 0 distintos. Total: 652 descriptores, 29 paletas, los
anteriores idénticos.

## Parada 1304: espera larga sin enemigo visible (diagnóstico)

Síntoma (informe del usuario y sonda de anfitrión): en la parada de cámara
1304 el guion espera en `0x2D67CA` unas 2000–2400 actualizaciones sin
enemigo a la vista. Evidencia de MAME 0.264 (`captures-m1rest/run1, run4,
run8, run9` y la nueva `captures-m1full/veh1`, idénticas): la recreativa
espera en `0x2D67CA` del fotograma **4582 al 4967 (385)**, tras `0x2D673E`
(4330..4581). Las apariciones de 4582 son el vehículo blindado `0x087E36`
(grupo 1, nodo `103884`), el soldado tumbado contado `0x04115A` (grupo 0) y
un soldado `0x0AF…` (grupo 2). En la recreativa el soldado tumbado **se
sube a la plataforma dinámica del vehículo** en su primera actualización
(`+38 = 10C22E`, posición relativa `(-8, 80)`), el vehículo lo trae a la
vista (X 1656 → 1558: `0x088120` → `0x0881E6`/`0x088232`), el jugador lo
mata (`0x040D06` en 4842) y la espera termina cuando el vehículo es
destruido (`0x08845C` en 4966). En el port `0x087E36` es externo: el
soldado se queda en el suelo en x = 342 de pantalla, fuera de la vista,
dispara desde allí y solo entra cuando `+74` llega a 0. **Causa: falta el
vehículo `0x087E36`** (hipótesis (b)); la cámara (1304 en ambos) y la X de
aparición (1646 en ambos) coinciden.

Preparado para portarlo: traza de familia `captures-m1full/veh1`
(`MSX_REST_FAMILY_ROOTS=87e36 MSX_REST_BOSS_FROM=4500 MSX_REST_LAST=5000`) →
`data/armored_fixture.json.gz` (rangos `87e36-89c00`, `40396-403d0`: 328
actualizaciones 4581..4967, estados `087F2E, 08806E, 088120, 0881E6,
088232, 08830C, 08833A, 0883E8, 08845C, 08889C, 0888C8, 08890E, 08893A,
088A30` y su hijo `0x040396`). Constructor: salud y tres palabras por rango
(`2D0E40/2D0EA2/2D0F04/2D0F66`), cuerpo sólido `0x004E76` (`0E4E90`),
caja `0E4E84`, plataforma dinámica `0x0140C4` con el mapa `0E4FF2`, ataque
`0E4E9E` mientras avanza.

### Vehículo blindado `0x087E36` nativo

`src/neo_armored.c`, traducción manual de las ~930 instrucciones
(`0x087E36..0x089940`) con sus 27 etiquetas de rutina: constructor (trampa
`trap #15` si `+72 < +70`, rangos `2D0E40/2D0EA2/2D0F04/2D0F66`, caja
`0E4E84`, cuerpo sólido `0x004E76` con `0E4E90`, paletas `0x24`/`0x30B`,
plataforma `0x0140C4` con `0E4FF2`; `0x03C532` no crea nada en la Misión 1,
como en el espadachín), marcador hijo `0x040396` (`0x04036C`, ya nativo en
`neo_props.c`), avance con humo `0x08982A` cada 8 actualizaciones y
ataque `0E4E9E`, parada (`0x0881E6`, tabla `0x0881D6`), espera
(`0x08833A`), ráfagas (`0x08889C..0x088A4A`: disparo `0x088978` con
`0x0896A0/0x088B52/0x088ED8` tras el actor del guion), retroceso ante un
jugador delante (`0x088AD2`, rectángulo `0E4ED6`), retroceso por impacto de
los tipos de `0x01B1F2` (programa `2846BA` y reanudación del interrumpido;
`+86` guarda la etiqueta del programa y el punto de reanudación es nativo)
y destrucción (`0x0883E8`/`0x0886EC` según el tipo `0x0D`: `0x019DFC`,
seis piezas `0x08943C` con la tabla `0x089554`, restos `0E50B2`, plataforma
liberada y mapa de restos `0E4EDE/0E4F36/0E4F8E/0E4FC0` si `+74`, ataque
`0E4EBE`; al acabar el programa el objeto `0x08CDB2` (`+76/+78`) y con
`+74 = 0` la explosión final `0x088598`). La retrollamada de programa
`0x089412` fija `+A4/+A6` (tabla `0E5104`); `0x08938A` crea diez piezas
`0x088F20`. La comprobación de protección (`2FFFE0`) antes de algunos
borrados no se reproduce (en placa original cae en `0x004188`). No llega a
`0x01B2BE` (sangre). Los hijos tras el actor del guion (`0x0896A0`,
`0x088B52`, `0x088ED8`, `0x08982A`, `0x08943C`, `0x088F20`, `0x0897E2`)
no son nativos todavía: se cuentan como externos, así que el vehículo no
hace daño con sus proyectiles.

Prueba `armored` (`FAMILY_ARGS_armored := --tree --marker-seq 2`; la traza
no guarda la lectura de `10C446`, y el arnés modela la cuenta atrás de 2
del objeto dinámico liberado y el registro del cuerpo sólido): **2 nodos,
328 actualizaciones en bucle cerrado (4581..4998), 45780 bytes iguales**,
incluido el retroceso por impacto de 4963 y la destrucción.

En el mundo, el soldado tumbado `0x04115A` encuentra la plataforma del
vehículo en su búsqueda de suelo (ranura 7, `10C22E`, como en la
recreativa) y entra subido a ella. Al destruirse el vehículo la explosión
`0E4EBE` (tipo 1) lo alcanza: la entrada `0x040C8E` de las tablas de muerte
en suelo y en el aire (`0x0406EA`/`0x040750`, índice 1) y su rutina
`0x040D06` quedan traducidas (`neo_soldier_ai.c`). `tools/test_gameplay.c`
mide la espera en `0x2D67CA`: **538 pasos** en la partida de referencia (de
pie a la izquierda del vehículo con la pistola; cota `< 2·385`) y **748,
810, 764, 952** en las cuatro partidas con reglas de lanzamiento (cota
`< 3·385`); antes eran unas 2000. Como el port aún no empuja al jugador
fuera del cuerpo sólido `0x004E76`, las políticas de la prueba se quedan a
su izquierda (las de lanzamiento solo desde el suelo: una reaparición puede
caer en la cornisa de x≈1435 o sobre la plataforma del vehículo).

Gráficos: `tools/export_rest_sprites.py mslugx.zip robe1 bld1@54800-55700
bld1@b6500-b6b00 trk3@33c98-34200 trk3@39500-3a100 trk3@a93c0-a9a00
trk3@35600-35a00 pri1@99862-99dc0 pri1@8c37c-8c700 captures-boss/runA@0-1
veh1@87e36-89c00`: 701 descriptores (los 652 anteriores idénticos), 31
paletas (nuevas `0x24`, `0x30B`; las anteriores idénticas). Los 14 volcados
de `veh1`: 62245 píxeles iguales, 162 distintos, todos en 4950 y 4980,
cuando el contador de destello `+17` está activo (3 y FF en las
actualizaciones previas); el validador dibuja con la paleta base.

### Hijos del vehículo blindado (`src/neo_armored.c`)

Todos tras el actor del guion (`0x00411C`): proyectil `0x088B52` (cae con
gravedad `0xB000`, rueda con la velocidad de rango `2D102C` reducida en la
primera pendiente y luego por el ángulo D7 de `0x0143E4`; explota
(`0x088E0E`: sonido 1022, explosión `0x01C706`) al tocar a un jugador
(fila `0E4EAE`, clase 0), con `0x004C0E` (RAM `1067E4`, copia de `1067E5`
hecha por `0x0046E0`: ahora `neo_world_t.shots_clear`/`neo_genv_t.clear_shots`),
contra una pared, al frenarse por debajo de 0x1000 o tras 0x140
actualizaciones en el suelo), vaina `0x0896A0` (rebota, `+6E` = 30
rebotes), humos `0x088ED8`, `0x08982A` (paleta de la palabra 1 de
`0x0334C0`: 0x13F) y `0x0897E2`, piezas `0x08943C` (crecen `+12/+13` de 8 en
8) y las diez piezas `0x088F20` (tabla `0E516E`). Ya hacen daño.

Prueba `armored` re-extraída de la nueva captura `captures-m1full/veh2`
(`MSX_REST_FAMILY_ROOTS=87e36,896a0,88b52,88ed8,8982a,8943c,88f20,897e2
MSX_REST_BOSS_FROM=4500 MSX_REST_LAST=5000`; las 328 actualizaciones del
vehículo son idénticas a `veh1`), con `--script-actor 101414`: los nodos
creados tras el actor del guion tienen el creador −2 y el código nativo debe
crearlos con `after_script`. **15 nodos, 609 actualizaciones en bucle
cerrado (4581..4998), 84560 bytes iguales**, 5 borrados. `0x088F20` y
`0x0897E2` no aparecen en la traza (el vehículo grabado no ejecutó
`0x08938A` ni la explosión final con `+74 = 0`): traducción sin validar.

Gráficos: `... veh1@87e36-89c00 veh2@88b52-89940`: 829 descriptores, 36
paletas (nuevas `0xD3, 0x208, 0x113, 0x13F, 0x37C`), 153 volcados:
1407960 píxeles iguales, 6326 distintos (los mismos de antes).

Al hacer daño, las partidas de lanzamiento mueren más en la parada 1304
(esperas de 506 a 2900 pasos; las partidas difieren entre `-O0` y `-O2`
del anfitrión, sin investigar): `tools/test_gameplay.c` solo exige que la
pasen (cota inferior 200). Una de ellas llegó a la entrada 9 de la tabla de muerte en suelo
`0x0406EA`, que es `0x040C8E` como la 1 (antes detención explícita):
traducida.

## Parada 2336: espera en `0x2D6E8A` (diagnóstico)

MAME: `0x2D6E8A` (grupos 0..7 vacíos tras el carro) dura 197 fotogramas en
`pri1` y `fam2` (7773..7970) y 26 en `trk3`; el espadachín de túnica
`0x0A8C62` está en x = 2615 (pantalla 279, `0x0A8E1A/0x0A8ECE`), pasa a
`0x0A8FE0` en 7867 y sale andando (`0x0A9390`) hasta 7968. En el port, la
sonda de anfitrión reproduce una espera sin fin solo con el jugador quieto
a la derecha de la columna x = 2576 de la tienda caída mirando a la
derecha: un espadachín `0x039590` que bajó del carro a su izquierda (el
carro para en 2610 y los espadachines salen en −27..−46) no puede volver a
cruzarla (celda `0x10`, la misma en los volcados de RAM de MAME desde 7500;
la cruza hacia la izquierda pero no hacia la derecha) y gira o pasea entre
x ≈ 2455 y 2542. Es la lógica original (prueba `arab` bit-exacta) con el
terreno original; el jugador original también puede estar ahí (`pri1`, 7410:
x = 2620). Con el jugador vuelto hacia el enemigo visible más cercano la
espera dura 0, 102 y 166 pasos (pantalla 160, 220, 280): prueba nueva en
`tools/test_gameplay.c` (`stand_2336`, cota `< 3·197`).

## Cajas ocultas `0x09A67E` (`src/neo_secret.c`)

Entrada `0x2E8` de la tabla de apariciones, en la cámara 1920 (registros
`0x2D6ABC/B02/B48/B8E`, uno al azar, grupo 1 contado, en 2072,136 sobre la
ruina de las cúpulas; luego hasta diez sin contar mientras el grupo 1 está
vacío). Invisible: caja `0x09ADA0..` por `+6E`, salud `+72` (`0x09A688`
cuenta golpes), `+70` objetos (0: 1..3 al azar). Abierta (`+5F` bit 7, por
disparos o por el mata-todo de `0x004BAC` de la tienda caída) deja un
objeto `0x08C374` por actualización (`0x08CD58` → `0x08CDB2`, tipo `+74`,
dispersión `+80/+82` de `0x09AE08`, movimiento `0x09A8E0` por `+7A`) y se va
fuera del rectángulo `0x09ADD8`. Al abrirse `0x09A956` → `0x03C5D2` busca
una celda del grupo 5 en `+3C/+40` −1, +7 y +15 (`0x013CF8`, ahora
`neo_kernel_point_group`); no hay ninguna en la Misión 1 (rejilla observada)
y, encontrada, crearía `0x03C962`: detención explícita, como la variante
`0x09A692` (prisionero `0x099E1C`). En la recreativa la caja contada vive
hasta que cae la tienda (su mata-todo la abre), así que también sostiene la
espera del grupo 1 de `0x2D6D78`.

Prueba `secret` (`data/secret_fixture.json.gz`, MAME `captures-m1full/box1`,
`MSX_REST_FAMILY_ROOTS=9a67e,9a688,9a692 MSX_REST_BOSS_FROM=6900
MSX_REST_LAST=7450`, rango `9a5ee-9a9c2`): **15 nodos, 416 actualizaciones
en bucle cerrado (6989..7397), 56140 bytes iguales**, 15 borrados, 32
objetos fuera de la familia. La prueba genérica lee ahora RAM `1067E2`
(mata-todo de otra familia) cuando la traza la tiene.

Objetos: los programas de todos los tipos con movimiento `0x08B9B2` se
importan (antes solo diez; la caja da el tipo 36 y las partidas de
lanzamiento llegaron al 37 y al 54). Retrollamadas traducidas: `2881CA`,
`287D50`, `28AA7E` y las del tipo 37 (`2884AA`, `28849A`, `2884BE`,
`288BAA`, `286B44`, `286AE8`, `286B2C`, `2884C8`: pasa a tipo 28, cuyo
movimiento `0x08BD5A` anda por el suelo); las de los tipos 31, 39 y 44
son detenciones explícitas. Gráficos: 1232 descriptores, 39 paletas
(faltan `0x37F, 0x133, 0x38E, 0x823`: ninguna captura las tiene).

## Espadachines `0x09D846/0x09D862/0x09D876` (`src/neo_swords.c`)

Registros del guion en las cámaras 2556..2789 (`0x2D6EFC`, `0x2D6F28`,
`0x2D6F6A..0x2D6FA6`, `0x2D7000`, `0x2D702C`; grupos 0, 2 y 5 contados) y
los que suelta el helicóptero `0x04857C` (creadores `0x04870C/0x04871C/
0x047A6C`, aún externos). Traducción manual de `0x09D846..0x09EC34`:
constructores `0x09EEB8` (palabras `+6E..+76` → banderas `+96/+98`,
temporizador `+8A`, `+8C = FFFF`) y `0x09EF6A` (`0x09D876`: punto objetivo
`+7E`, objeto `+8C/+8E`, cuchillos `+95`), `0x09D89A` (suelo, proyección,
paleta por `0x01B2BE` en `0x09EFE4`: **0x3FE con BLOOD ON, 0x0B8 con OFF**,
caja `0x09F610`, palabras de rango `2D2244..2D24B0`, jugador más cercano
`0x01AF9A` guardado en `+6E` como el nodo original `1014C4/101574`).
Estados: patrulla `0x09D964`, reposo `0x09D9C0/0x09DA3A`, acercamiento
`0x09DB44`, persecución `0x09DC5C`, carrera `0x09DD12`, salto de ataque
`0x09DDCE`, huida `0x09DE84` (temporizador `+8A`), aterrizaje `0x09DF5E`,
guardia `0x09DFB2`, giro `0x09E00A`, lanzamiento de cuchillo `0x09E062`
(retrollamadas `0x09F508` → cuchillo `0x09ECCC`, `0x09F536`), tajo
`0x09E122..0x09E1FE` (fila de ataque `0x09F62C`), saltito `0x09E25A/
0x09E250`, caída `0x09E34C`; colas `0x09E77E/0x09E79C/0x09E7AC` (cualquier
golpe de `0x004C24` mata: objeto `0x09F53E` → `0x08CDB2`, muerte por el
tipo `+5A` o una de las cuatro de `0x09F66C` por el RNG) y `0x09E81C`
(retirada fuera de `0x09F64C`). Muertes `0x09E842/0x09E8C2/0x09E920/0x09EAB8`
con la salpicadura `0x01C97A` (`src/neo_effects.c`; paleta por `0x01B2BE`
en `0x01C996`: **0x37A ON, 0x3BA OFF**). `0x09EC34` (comprobación del
cartucho) y `0x09ECC4` (`0x0041AC`) borran. Servicio nuevo del actor
genérico: `ga_probe_forward` (`0x014850`, sonda hacia delante y hacia
abajo, la misma que usa el soldado).

- **Parada explícita**: `0x09E382` (bandera `+96` bit 3, palabra `+72` del
  registro distinta de 0: salto a un punto fijo `0DD8/0EE0` por `0x0B64BA`),
  no alcanzada en las grabaciones ni en las partidas del port.
- Sin validar por la prueba (no aparecen en `fam2`): el lanzamiento de
  cuchillo y el cuchillo `0x09ECCC` (paletas `0x16C..0x16E` sin captura),
  el tajo, la huida y el salto de ataque.
- Sangre: `data/blood_fixture.json` regenerado
  (`tools/extract_blood_fixture.py /home/user/captures/blood`, con las
  llamadas del lado ON en `0x09EFFE/0x09F010` y `0x01C9A8`): sitios
  `0x09EFE4` (13 peticiones ON, 0x3FE) y `0x01C996` (11, 0x37A) pasan a
  traducidos; `0x09F598` (`0x09F562`, otra familia) sigue sin traducir.

Prueba `swords` (`data/swords_fixture.json.gz`, MAME `captures-m1full/fam2`,
rango `9d800-9ef00`): **13 nodos, 818 actualizaciones en bucle cerrado
(8199..8605), 112700 bytes iguales**, 13 borrados, 8 salpicaduras fuera de la
familia; muertes por las cuatro rutinas, carrera, guardia, giro y patrulla.

Gráficos: `... cfam@8c37c-8c700 fam2@9d800-9ef00 fam2@1c97a-1c97a
--palette-capture /home/user/captures/blood` (opción nueva: paletas de una
captura sin traza, aquí las ON `0x3FE` y `0x37A`): 1546 descriptores (los
1330 anteriores idénticos), 45 paletas (las anteriores idénticas; nuevas
`0xB8`, `0x37A`, `0x3FE`), 175 volcados; los 14 de `fam2` (13 de los espadachines, 1 de la
salpicadura) dan 26143 píxeles iguales y 3529 distintos en 5 volcados (8310..8400, 8520),
cuando bajan del helicóptero (sin investigar: probablemente el orden de
sprites con el helicóptero, que aún no es nativo).

## Prisioneros `0x099DCC/0x099E06` (`src/neo_prisoner.c`)

Registros del guion en toda la misión (desierto y aldea incluidos:
`0x2D6384`, `0x2D639E`, `0x2D6666` (`0x099E06`), `0x2D66D6`, `0x2D67DA`,
`0x2D69D0`, `0x2D6ED0`, `0x2D6F42`, `0x2D70C4`; grupo 9 sin contar) y los
tres de la escotilla del jefe. Palabras `+6E..+76`: `+6E` cara/vagar (3 =
`0x0029F2(3)`), `+72` mira a la izquierda, `+74` objeto que regala (`FFFF`
ninguno, `FFFE` se lo da al jugador), `+76` su vida, `+78` detrás. Traducción
manual de `0x099DCC..0x09A5EE`: constructor `0x09AC16` (paletas `0x382/0x3FA`,
profundidad `8820`; `0x03C526` no crea nada en la Misión 1), colgado atado
`29FAD8` hasta un golpe (`0x09AD2A` → `0x004C24`; `+88` cuenta el
estremecimiento de `0x09AD1C`), se suelta (`29FB34`); `0x099E06` atado a la
silla (`29FCBE`, contador `+18` del programa), golpeado cae (`29FDE6`) y deja
la cuerda `0x09A97C` (`29FE0A`). Suelto (`0x099FD8`): paletas `0x381/0x3FA`,
caja `0x09AD4C`, vaga (`29FE52`, `+62` = 1,5 px) `+7A` actualizaciones, se
para (`29FED0`) en una pared o al acabarse, cae de las cornisas (`29FBEC`);
en un marcador de la lista de objetos (`0x040200`, ahora
`ga_marker_cross`) sube (bit 0: `29FFE4/29FFFA` con `0x0402E6`, ahora
`ga_marker_follow`; luego salta `2A0074/2A011C`) o baja (bit 1:
`2A0128/2A01D0/2A024A`). Tocado (`0x09AB9A`: rectángulo `0x09ADE0`; RAM
`10C303` vale FF en las 91 lecturas de `fam2`) se cuenta el rescate
(`0x09AAD6`: `10E9E8/10E9EA` por `0x0C8CC2`, `10C5FC/10C600`, `10C5FF/10C603`
con `+74 = FFFE`; ahora `neo_ga_flags_t.tally/rescued/gifts`), gira, saluda
(`29FEEC`: su retrollamada `0x09ACD6` da el objeto `+74` por `0x08CDB2`
colocado por `0x014A02`, ahora `ga_sweep_place`), saluda con la mano
(`2A02B6`) y corre a la izquierda (`2A0260`) hasta una pared, 30
actualizaciones más. Fuera de `0x09ADD0` (`0x0C8BEC`) acaba en `0x09A5EE`
(comprobación del cartucho → `0x004188`: borrar).

- **Parada explícita**: `+74 = FFFE` al acabar el saludo (`0x0DDCDA`: el
  prisionero sigue al jugador para darle el objeto), no grabado; ambos
  jugadores dentro del rectángulo (`0x09ABD6`, el port tiene uno).
- Servicio del motor de animación: el contador `+18` del programa se lee y
  escribe ahora a través del estado del programa (antes `SET_BYTE/DEC_BYTE`
  sobre `+18` se perdían al acabar el paso); una retrollamada sin traducir se
  detiene en su propia dirección (antes `0x00427A`).
- `0x0040BC` copia `+68` de su A6: con `0x00411C` es el actor del guion
  (`+68 = 0`), no el creador (`neo_world.c`).

Prueba `prisoner2` (`data/prisoner2_fixture.json.gz`, MAME
`captures-m1full/fam2`, rangos `99dcc-9a5ee`, `9a97c-9a9c2`): **2 nodos, 259
actualizaciones en bucle cerrado (8079..8444), 35980 bytes iguales**, con
bajada por un marcador (`0x09A4FA`), rescate, saludo con su objeto y salida.
La prueba genérica lee ahora la lista de objetos (`10C3A2/10C3F4`) que leyó la
actualización (`tools/extract_family_fixture.py` junta los bytes de altura e
identificador leídos aparte).

Prueba de mundo de la región (`tools/test_neo_world.c`,
`data/soldier_world_fixture.json.gz` re-extraído de `walk3`, `fire3`,
`entry-walk3`, `entry-fire3`, `m2x`, `m3x` con los prisioneros, la cuerda y
los objetos `0x08C374` como actores del mundo): **5248 actualizaciones,
22880 estados de actores genéricos** (antes 17934), 15168 llamadas RNG
nativas (873 inyectadas, antes 879). La extracción distingue `0x09A5EE`
(compartida con las cajas `0x09A67E`, externas allí), marca `pending` los
objetos creados detrás del recorrido (tras el actor del guion por un actor
posterior; corren desde la actualización siguiente), deja fuera los objetos
que crea `0x0C962C` (externo) y graba las cajas de recogida `10C5EC/10C5F4`
de cada actualización (las restaura el sistema de continuación antes que los
actores del mundo: `m2x` 7525). La prueba inyecta el golpe externo de un
cuerpo sin salud (`+5F` bit 7) y compara la capa `10D7B6` como `10C72E`
cuando el extractor ya las igualó.

Gráficos: `... fam2@1c97a-1c97a fam2@99dcc-9a9c2 --palette-capture
/home/user/captures/blood --keep <exportación anterior>` (opción nueva
`--keep`: las paletas que ninguna captura dada tiene se conservan; las
capturas `hfam/cfam` ya no están): 1807 descriptores (los 1546 anteriores
idénticos), 49 paletas (nuevas `0x381`, `0x382`, `0x3FA`, `0x117`; las
anteriores idénticas); los volcados de `fam2` 8100..8190 del prisionero:
1402 píxeles iguales, 0 distintos.

Partida de `tools/test_gameplay.c`: los rescates cambian el recorrido; la
parada 1304 dura 146 (cota inferior 140; la recreativa 385, ver arriba), la
cámara sube a 32 y las armas 0C y 08 (constructor `0x0641EC`) de los regalos se disparan
como paradas explícitas contadas (sus disparos no están portados; la H sigue
siendo nativa entera). `armored_x` acepta el blindado hasta x < 352 (su
cuerpo entra en la vista 40 px antes que `+3C`).

## Frutas de la ventana central de la ruina (cajas `0x09A67E` + objetos)

Informe del usuario: tras el derrumbe, disparando a la ventana central de la
ruina caen manzanas en la recreativa y en el port no. Son las cajas ocultas
`0x09A67E` del guion en (2072,136) (`0x2D6ABC..0x2D6BD2`, ya nativas en
`neo_secret.c`), cuyos tipos `+74` son 85, 54 y 56 (0x55, 0x36, 0x38), y las
de (1984,104) y (2340,176) (0x2C..0x2E, 0x37, 0x45, 0x54, 0x24). Los tipos
con movimiento `0x08BAB2/0x08BA38` se devolvían como externos: no aparecía
ningún objeto. Con los movimientos y retrollamadas de las comidas
(prisioneros, arriba) son nativos; además:

- El paso de animación del actor genérico pone en el nodo el temporizador
  `+24`, la velocidad `+26`, el destello `+17`, el contador `+18` y los
  atributos `+15` antes de cada retrollamada y los recoge después (antes una
  retrollamada que escribía la velocidad, `28AB36`, o giraba el objeto,
  `286AE8`, se perdía al acabar el paso).
- `0x08BCCC` (tipo 0x55 y su tabla `0x08BCC0`) sigue sin traducir: el objeto
  se devuelve como externo (contado).

Captura nueva `captures-m1full/food1` (`MSX_REST_FAMILY_ROOTS=9a67e,9a688,
9a692,8c374,8c37c MSX_REST_BOSS_FROM=6900 MSX_REST_LAST=7800`): tras el
derrumbe salen de la ventana cinco objetos 0x58 (7037..7045), luego 0x37,
0x54, 0x38/0x36/0x2E y 0x24. Prueba `food` (`data/food_fixture.json.gz`,
rangos `9a5ee-9a9c2`, `8c374-8c700`, `--script-routines 8c374,8c37c`: las
cajas las crea el guion, los objetos las cajas): **28 nodos, 1762
actualizaciones en bucle cerrado (6989..7338), 244580 bytes iguales**, tres
recogidas de comida (puntos: el arma no cambia). **Parada de la prueba** en
7339 (corte del fixture): los objetos de la ventana tocan allí la colisión de
los escombros `0B53A8` (bit 2 de `+5F`), que la rejilla del port no tiene
(la colisión de la capa 10C72E cambia con el mapa; sin traducir).
Gráficos: `... food1@8c374-8c700`: 1807 descriptores, 49 paletas, sin
cambios; los 26 volcados de `food1`: 10576 píxeles iguales, 1631 distintos
(volcados de 7110..7260 con muchos objetos superpuestos y actualizaciones del
mismo fotograma, como en `pri1`).

## Lanzacohetes `0x05ED84` (`src/neo_rocket.c`)

Registro del guion `0x2D6FC2` (cámara 2688, corredor op 8: uno cada vez
mientras el grupo 4 está vacío; palabras `1, 256, 312`: rango absoluto
`+70/+72` → `+76/+78`; `+74 = 3` lo fuerza el constructor). Traducción manual
de `0x05ED84..0x05FD48` con sus hijos: destello de puntería `0x060490`
(`0x0040BC`, vive `+82` actualizaciones, programas `60348/6035C` por disparo
y postura), cohetes `0x05FD8A` (tras el actor del guion, retrollamada
`0x05FCC0` de los programas de recarga, desplazamientos `5ED46/5ED56`),
fogonazo `0x0602E4` (retrollamadas `264320..2648DE` del banco: solo
`264320` lo crea tras el actor del guion, las otras siete tras el
lanzacohetes), estelas `0x059C82` (`0x059E98`: `0x01ACF2` tras escribir la
velocidad deja en `+30/+34` el desplazamiento del cohete) y el humo de la
muerte `0x0897E2` (`neo_armored.c`).

- Postura `+88` (FF/00): programas, caja `5EB46/5EB62`, cuerpo sólido
  `5EB7C/5EB6E` (`0x004E76`), paletas `0x9C, 0x9D, 0x1D` y destello `0x30B`
  (`0x004520`). Estados: anda (`0x05EF5C/0x05EF74`, velocidad de rango
  `2D1372` hacia `+76 + +7A`, `0x05FB26`; filas de empuje `5EB8A/5EB9A`),
  espera (`0x05F0A8/0x05F0F6`, rango `2D13A4`), primera espera dentro del
  rango (`0x05F21E/0x05F25C`), parada (`0x05F364/0x05F39A`), giro
  (`0x05F486/0x05F4BC`, invierte `+88`), disparo (`0x05F5F0/0x05F5FC/0x05F694`
  con el destello), recarga (`0x05F780/0x05F7BA`: `0x004192` borra el
  destello, el programa dispara el cohete; `2D13D6` disparos por ráfaga),
  muertes `0x05F8D4` (tipo `0x0D`) / `0x05F904` → `0x05F94E` (sonidos
  `1021/102F`, 16 explosiones `0x01DBAA`, humo). La decisión `0x05FC2C`
  (`+74 = 3`: lado del jugador más cercano `0x01AF9A`) elige giro o disparo
  cuando `+7E` llega a 0. Todo estado acaba en `0x05FD48` (muerte por `+5F`
  bit 7, recorte `5EBC6` → `0x004188`).
- Cohete: velocidad por postura, ángulo `+77` = `0x002EB4(+30>>12, +34>>12)
  >> 2` en el aire (pose por `+77`, fila de ataque `600A0[+77]`), estela cada
  dos actualizaciones (`5EBD6[+77]`), gravedad `0xB000`; en el suelo, pared,
  golpe o contacto explota en el sitio: `0x009F88` (paletas liberadas) y
  `jmp 0x01C706` (la explosión de `neo_effects.c` corre en la misma
  actualización sobre el mismo nodo).
- **Paradas explícitas**: palabra `+6E > 1` (`trap #15` del constructor). El
  estremecimiento `+8D` (programas `5ED16` por `0x01B206`) está traducido
  pero no grabado (el jugador nunca golpeó al lanzacohetes sin matarlo):
  `+84` guarda la etiqueta del programa y el punto de reanudación es nativo.
- Las tablas `0x5EB46..`, `0x5FFA0..`, `0x60348` y los rangos `2D1310..2D146C`
  pasan a `data/region_rom.json`; `tools/extract_soldier_programs.py` lee
  ahora también `program-fixed.bin` (los programas del destello y del
  fogonazo están en la ROM P1).

Captura nueva `captures-m1full/rl2` (`MSX_REST_FAMILY_ROOTS=5ed84,5fd8a,60490,
897e2,59c82 MSX_REST_BOSS_FROM=8200 MSX_REST_LAST=9900 MSX_REST_HOLD_FIRE=
8260-9300`: opción nueva de `tools/capture_mission_rest.lua`, solo para
capturas: sin disparos, granadas ni saltos en esas imágenes, con el jugador
invulnerable, para que el lanzacohetes viva y dispare; `fam2` solo tenía sus 66
actualizaciones de andar y morir). Tres lanzacohetes seguidos (el corredor
los repone), 15 cohetes, 10 fogonazos, 257 estelas. Prueba `rocket`
(`data/rocket_fixture.json.gz`, rangos `5ed84-5fa00`, `5fd8a-60100`,
`60490-605a4`, `602e4-60348`, `59c82-59e30`, `897e2-89900`, `--script-actor
101414`): **299 nodos, 5778 actualizaciones en bucle cerrado (8280..9503),
768880 bytes iguales**, 286 borrados, 1 contacto con el jugador (cohete),
21 hijos fuera de la familia (los fogonazos de `264320`, las explosiones).

Gráficos: `... food1@8c374-8c700 rl2@5ed84-60600 rl2@59c82-59e30`: 2018
descriptores (211 nuevos), 55 paletas (nuevas `0x9C`, `0x9D`, `0x1D`), 266
volcados: 1628998 píxeles iguales, 16996 distintos; los 61 de `rl2`: 177072
iguales, 4786 distintos (volcados con estelas y explosiones superpuestas,
como en `food1`).

Auditoría del guion tras esta familia: quedan sin crear las apariciones
contadas `0x09F682` (cámara 2184) y `0x04857C` ×2 (3192).


## Formación de helicópteros `0x04857C` (cámara 3192)

`src/neo_heli.c`: traducción manual de `0x047874..0x048FFE` con sus
servicios `0x04900C..0x04A1AC` y las llamadas de programa
`0x04A1AE/0x04A208/0x04A262`. Dos registros del guion la crean según la
mitad de pantalla del jugador (`2D7092`, consulta `0B`: jugador a la
izquierda, entra por `x=352`; `2D70A6`, consulta `0C`: por `x=-32`); palabras
`+6E` = 12 (altura `0x1C0`), `+70` = 0 (juego de rutas), `+72` = 5.

- **Controlador `0x04857C/0x0485EC`** (invisible): `+72` guías `0x04870C`
  (`+61` = índice), cuenta de vivos `+96` (al llegar a 0: `019DFC` y borrado),
  ranuras `+76+4i`/`+90+i` con la posición y el rumbo de cada guía, ritmo de
  bombardeo (`+8A/+8C/+8E/+95/+99`, rango `2D1656`) y la explosión en cadena
  (`+9A/+9B/+9C`: un bit cada 5 actualizaciones).
- **Guía `0x04870C`** (invisible): espera `+61×16`, crea el helicóptero
  `0x048882`, sube a la altura (`04982A`, dirección `0499A4`: ángulo `002EB4`
  + giro `+74`, velocidad = distancia × 0x1800 suavizada ⅛/¹⁄₁₆; alabeo
  `049A92`) y a 8 px (`049E46`) sigue la ruta `E0AF2/E0AFE[+70]` con el
  intérprete `021288` (`E0B0A..E0C32`, velocidad `+A8` hasta el rango `2D1A92`),
  anotando su posición en la ranura del controlador.
- **Helicóptero `0x048882`**: caja `E0AB2`, vida `2D1530`, paletas `0x15A`
  (guía 0, sonido `0x10E8`) o `0x20`, destello `0x15C`, rotor `0x047874`
  (programa `24483E`, índice de alabeo copiado y reflejado desde 7, destello
  compartido `0044F6`). Espera a que su guía llegue (`0x04893E`), pasa a la
  raíz (`01AD52`) y sigue la ranura `+95` del controlador (`049EFA`: recto a
  menos de 4 px, si no 4 px por el ángulo; alabeo `+96` con desplazamiento 3
  por la tabla `E09F4` del rumbo `+AA`). Bombardea (`04A064`: jugador a 32 px
  del lado del alabeo, `04A15A`; los siguientes se suman mientras
  `+95 × +8C <= +8A`) en `0x048A06/0x048A2C`: `2D1840` lanzadores `0x047A6C`
  cada `2D18D6` actualizaciones, alabeo hacia 4. Cola `049030`: caja (`004C24`),
  golpe → `0x048AF6` (o `0x048AB6` con los puntos `08D1D0`: formación completa
  y guía 0; entonces todos explotan por los bits `+9A`), recorte `E0AA2`.
  Muertes (`0x048B10`, moneda `0029C6`): explosión `0x048E22` (humo `E0A3E`,
  cuatro restos `0x048EDA/EFE/F26/F4E` con `0x180000` polar y bocanadas
  `01C6EE`, `01C71E` en el sitio) o caída `0x048B2E/0x048BBC` (`0x50000` polar
  a 0x20/0x60, humo `E0A50`, bocanadas cada 5, sonda vertical `01495C`,
  choque `0x048C8C` → restos `0x048D22/0x048D5E` con `22CF2C`, paleta `0x204`,
  fila de ataque `E0AE2`).
- **Lanzador `0x047A6C`**: programa `244F06`; su llamada `04A262` crea la
  bomba `0x047B24` (rango de velocidad `2D19B4` polar por `+80` del lanzador,
  desplazamiento `E0A1E[+96]`, fila `E0ABE`; impacto `0x048510` en el suelo
  (`0146E8`, programa `245106`) o explosión `0x047C48` (`22E504`) al tocar al
  jugador) y el casquillo `0x047DE0` tras el actor del guion (`00411C`,
  programa `244F9E`, gravedad `0143DC`, un rebote `+61`, 18 actualizaciones).

**Captura** `captures-m1full/heli2` (`MSX_REST_FAMILY_ROOTS=4857c,47de0
MSX_REST_BOSS_FROM=8500 MSX_REST_LAST=16000 MSX_REST_HOLD_FIRE=8600-9900
MSX_REST_ROM_PATCH=d70a4:0c:0b`): la opción nueva `MSX_REST_ROM_PATCH` (solo
para capturas) cambia en la imagen ROM de MAME el byte de la condición del
segundo registro (`2D70A4`: `0C` → `0B`) para que las dos formaciones (10
helicópteros, una por cada lado) aparezcan en una partida de un jugador; el
jugador no dispara entre 8600 y 9900 y las mata después (cámara liberada en
13103). Los casquillos van tras el actor del guion, así que `47de0` ha de
ser raíz de la traza (la primera captura `heli1` no los tenía).

**Prueba `heli`** (`data/heli_fixture.json.gz`, rangos `47800-49000`,
`49000-4a300`; `--script-actor 101414 --script-routines 47de0
--late-creator --hit-to 47c48 --hit-moving`): **634 nodos, 62823
actualizaciones en bucle cerrado (8591..13088), 8706880 bytes iguales**, 631
borrados, 17 contactos con el jugador (bombas), 101 hijos fuera de la
familia (humos, bocanadas, explosiones), 1 creado fuera (la segunda
formación, raíz inyectada). Novedades del banco de pruebas:
`--late-creator` (la bomba nace en la actualización en que el programa del
lanzador termina: el lanzador queda como marcador `003F1E` y sigue siendo el
creador), `--hit-moving` (la bomba vuela antes de su fila de ataque) y el
enlace de un hijo nativo solo a un nodo grabado que aparezca en los 8
fotogramas siguientes (en 11363 la ROM se quedó sin nodos para un casquillo:
el hijo nativo cuenta como externo). Corrección de la lectura: en `04855A`
y `048BB6` no hay `rts`: el constructor del impacto y el inicio de la caída
siguen en el mismo fotograma con `048560`/`048BBC`.

**Paradas explícitas**: helicóptero solitario `0x047F7E..0x048510` (estados
`0x047800/0x04783A/0x048002..0x0484FE`, lanzadores `0x0479B8/0x047A12`,
bombas `0x047AFC/0x047B10` de `04A1AE/04A208`; no aparece en la misión 1),
`+6E > 0x13` (`trap #15` "takasa err"), la explosión temporizada `0x048CD2`
(inalcanzable: `048C36` comprueba el byte alto de la palabra `+8C`). Los
puntos `0x048AB6` están traducidos (suma BCD `057948` a través de
`neo_item_add_score`, etiqueta `08D0D2` externa) pero no grabados.

Gráficos: `tools/export_rest_sprites.py ... rl2@59c82-59e30 heli2@47800-49000`:
2100 descriptores (82 nuevos), 63 paletas (nuevas `0x15A`, `0x20`, `0x21`,
`0x22`, `0x23`, `0x178`, `0x179`, `0x15C`); los 150 volcados de `heli2`:
1039120 píxeles iguales, 128626 distintos (diez helicópteros con humos,
bocanadas y explosiones superpuestos, y el destello `+17` de los golpes; el
validador dibuja con la paleta base), 30 entradas sin descriptor (efectos
fuera de los programas exportados).

Tablas en `data/region_rom.json`: `E0962..E0C32`, `48E1A`, `49006`, `49762`
y los rangos `2D14CE..2D1AAC`; programas (19) en `data/soldier_programs.json`
(`HELI_ROOTS`). `neo_gactor.c`: `ga_probe_vertical` (`01495C`).

## Jinetes de camello `0x09F682` (cámara 2184, `src/neo_camel.c`)

Registro del guion `0x2D6D24` (cámara 2184) y su compañero de la cámara
1939: dos camellos, cada uno con tres nodos. Traducción manual de
`0x09F682..0x0A0AE8`:

- **Camello `0x09F682`** (invisible: lleva el estado y el trote): palabras
  `+6E` (lado), `+70` (lado lejano, `+90` bit 1), `+72` (bits de cambio de
  lado, `0A0664`), `+74` (punto de parada `+84`), `+76` (vida `+80`, 0 =
  7FFF). Constructor `0A023E/0A02D0` (suelo, jinete `0x09FDFE` con
  `0x0040BC`, paleta `0x117`; `03C532` → `019240`: sin onda `03C62A` en la
  misión 1), rangos `2D311C/2D30A0`. Entrada andando (`0x09F7C4/0x09F7F4`,
  `0A0532`) o esperando (`0x09F730/0x09F752`, `0A055C`) y parada
  (`0x09F76A/0x09F782`: espera a que acabe el programa del jinete, `+61`).
  Estados: andar `0x09F846/0x09F878` (movimientos `+7A`: vaivén `0A039E`,
  alejarse `0A03D6`, volver `0A043A`; `0A047E` velocidad de programa),
  alejarse `0x09F90A/0x09F940` (`0A0508`: seis veces cerca), parado
  `0x09F968/0x09F9A6`, giro `0x09F9FA/0x09FA28`, cambio de lado
  `0x09FA48/0x09FAC0`, sin cabeza `0x09FADE/0x09FB2C` (sonido `108E`),
  apuntar/disparar/bajar `0x09FB48..0x09FC6C` (rangos `2D3242`, `2D31E0`,
  `2D317E`; distancias `2D3306/2D3320`), huida `0x09FC9C..0x09FCFE`
  (`019DFC`, 6 px; también al morir el jinete, `+5F` bit 7). Colas
  `09FD0E` (vida, `0A072A`), `09FD3A`, `09FD4A` (recorte `A0B1E`) y
  `09FD60` (`004188`; la comprobación del cartucho previa no se reproduce).
  Trote `+8D` por la llamada `0A0774` (desplazamientos `A0B6A` → `+88/+89`).
- **Jinete `0x09FDFE/0x09FEAA`**: sigue al camello (`0A07D0`: `+88/+89`
  reflejados, capa `+1E`, banderas `+90`, caja `A0AEA/A0AF6`, sin caja en
  los estados `B/C/D` o FF) con el programa del estado `+8B` (tablas
  `A0B82/A0BBA`, 16 entradas: FF lee la 15). Al acabar el programa pone
  `+61` del camello. Muerte (`09FF0A`): `+5F` bit 7 del camello; espada
  `0x09EE34` (`09F562`, `neo_swords.c`) o `+96` bit 0 sin cabeza
  (`09F58C`), `009F88`, paleta por `01B2BE` en `09F598` (`neo_blood.c`: ON
  `0x3FE`, OFF `0x0B8`; la captura BLOOD ON pidió `0x3FE` en `09F5AA`, ya en
  `data/blood_fixture.json`), lado por `+5B` y las muertes de espadachín
  `0x09E920`/`0x09EAB8` al azar.
- **Cabeza `0x09FF26/0x09FF92`**: vida `2D303E`, sigue al jinete
  (`0A0866`, programas `2A24EC..2A25CE` por los cambios de `+90`), caja
  `A0B02` mientras `+8F` del jinete; golpeada con `+90` bit 3 cambia de
  programa y al morir pone el bit 4 del camello (→ sin cabeza).
- Llamadas de programa: `0A0AB6/0A0ABE` (`+90` bit 2), `0A0AC6/0A0AD8`
  (bit 3 del jinete y del camello), `0A09D0/0A0A02` (sonido `10FF`, disparo
  `0x0A0034` en (-27,31)/(-32,31): rango `2D30BA`, fila `A0B0E`, explosión
  `0x0A00E8` con `22E504`), `0A0A68` (3 o 4 piezas `0x0A016A`, velocidades
  `A0B2E`), `0A0A34/0A0A5C` (casquillo `0x0A020A`, `A0B5E`).
- `neo_gactor`: un barrido cuyo padre es el bloque de posición de otro
  actor (el jinete muere encima del camello: `01436A` con `+38` = camello
  `+28`) pasa a coordenadas absolutas como `01AC4C` (posiciones anterior y
  actual del bloque).
- `0AAA88` (vehículo del jugador, `0A0664`) responde 0: el port no tiene
  vehículos del jugador.

**Captura** `captures-m1full/camel2` (`MSX_REST_FAMILY_ROOTS=9f682
MSX_REST_BOSS_FROM=6900 MSX_REST_LAST=8400 MSX_REST_FIRST_DUMP=6900
MSX_REST_HOLD_FIRE=6950-7950`): sin disparos el segundo camello llega a la
parada 2336 y recorre andar, alejarse, girar, cambiar de lado, apuntar,
disparar (8 disparos, 7 tocan al jugador invulnerable), bajar y huir. En
`camel1` el jugador lo mataba en 7200. Nuevas opciones solo de captura
`MSX_REST_HOLD_STILL` y `MSX_REST_STAND_FIRE` (no usadas en `camel2`: con el
jugador quieto la cámara sigue hasta 2336 igualmente).

**Prueba `camel`** (`data/camel_fixture.json.gz`, rango `9ea00-a0800`,
`--hit-to a00e8 --hit-moving`): **18 nodos, 1970 actualizaciones en bucle
cerrado (7001..8150), 273280 bytes iguales**, 18 borrados, 7 contactos con
el jugador (disparos), 1 creado fuera (el segundo camello). No grabados
(traducidos): la entrada `0x09F6CA` (`0A0296`), la entrada andando
`0x09F7C4`, sin cabeza `0x09FADE`, el casquillo `0x0A020A`, la muerte
`0x09E920` del jinete y la espada en `0x09F58C`.

Gráficos: `tools/export_rest_sprites.py ... camel2@9ea00-a0800`: 2220
descriptores (120 nuevos, los 2100 anteriores idénticos), 64 paletas (nueva
`0x118`); los 37 volcados de `camel2`: 75084 píxeles iguales, 367 distintos
(8070 y 8100: muertes con efectos superpuestos).

Tablas en `data/region_rom.json`: `A0AEA..A0BF9` y los rangos
`2D303E..2D3417`; 36 programas (`CAMEL_ROOTS`).

Auditoría tras esta familia: **0 apariciones contadas perdidas**; sin
contar quedan `0x01913C 0x031D80 0x031D92 0x03264E 0x03458E 0x03FC5A
0x0400AA 0x092A32 0x0B6298 0x0B639A 0x0B63A0`.

## Apariciones no contadas del pueblo: `0x0B6298` y `0x0400AA`

Captura `captures-m1full/unc1` (`MSX_REST_FAMILY_ROOTS=b6298,400aa,3fc5a,
31d80,3264e MSX_REST_BOSS_FROM=2300 MSX_REST_LAST=4100
MSX_REST_FIRST_DUMP=2300`).

- **Decoración `0x0B6298`** (cámara 336, registro `0x2D63B8`; `neo_barrel.c`):
  paleta `0x442`, programa `2A89AC`, profundidad `01AD8C`, recorte `0B6948`
  → `0x0B6202`. Prueba `decor`: **665 actualizaciones en bucle cerrado
  (2496..4035), 92960 bytes iguales**, borrado en 4035.
- **Toldo del puesto `0x0400AA`** (cámara 192, registro `0x2D6364`;
  `neo_props.c`): suelo, `009F88`, paleta `0x13B`, programa `2402CA`, sello
  de colisión `0E0932` (`0C88B0`: no cambia ninguna celda de la rejilla
  estática, que ya lo contiene; comprobado), profundidad, recorte `0E08B8`
  (`03FADA` → `0041AC`). Prueba `awning` (`--terrain 10c72e`: la opción
  nueva lee el objeto de capa del pueblo `10C72E`, cuyas lecturas
  `10C742..` guarda ahora `extract_family_fixture.py`): **152
  actualizaciones (2349..2803), 21140 bytes iguales**, 1 sello.
- Mundo: un nodo que no es actor genérico (el corredor del guion que creó un
  soldado) y se borra con nodos más profundos detrás queda como marcador
  (`004126`), igual que los actores genéricos: el borrado en árbol `0041AC`
  del toldo se detiene en él y no se lleva al soldado (`walk3`, 2730).
  `data/soldier_world_fixture.json.gz` re-extraído con `0x0400AA/0x0B6298`
  como actores del mundo (el extractor anterior reproducía el fichero
  exacto): 5248 actualizaciones, 27193 estados de actores genéricos.
- Gráficos: `unc1@b6200-b6300 unc1@400aa-40100`: 2222 descriptores, paletas
  nuevas `0x442`, `0x13B`; 66 volcados: 51172 píxeles iguales, 0 distintos.
- **Paradas explícitas** (siguen externas): el puesto `0x03FC5A` (sus cajas
  de fruta `0x03FD6E` preguntan a `01AF1A` por el bit 6 de `+15` del
  jugador, que el entorno de los actores no lleva), los toldos rayados
  `0x0B639A/0x0B63A0` (velocidad de programa por RAM `100012/100013`,
  `002836`, y rebote por `10C6B0/10C6B1`, que escribe `0B64BA` desde el
  jugador), `0x031D80` (su registro `0x2D641E` no se ejecutó en `unc1`) y `0x031D92`.

## Porteador del pueblo `0x03264E` (cámara 440, `src/neo_porter.c`)

Registro `0x2D642E` (grupo 9, no contado; palabra `+6E` = 80). Traducción de
`0x03264E..0x032938` y de las llamadas de sus programas del banco
(`238632..2386EE`, `238AF0..238BAA`):

- **Figura `0x03264E`** (`019DFC`, paleta `0x92`, programa `2389CE` con
  `238B20/238B70/238AF0/238B08/238B78`: va y viene, `+7E` su desvío):
  `+6E` + progreso de cámara (RAM `10E99E`) es la cámara a la que envía al
  porteador `0x0327EC` (`0040BC`, desde `x = 0x180` o `-0x40` por `+70`,
  `01ACF2`); movimiento `08CCD4` hasta que el porteador llega (`+80`), luego
  `0x0327B0/0x0327CA` (programa `2386F0` con `238B8E`, `01436A`).
- **Porteador `0x0327EC`** (paleta `0x114`, programa `237F36`, 2,5 px):
  `23867C` (a menos de 0x28 px pone `+80` de ambos), `238632` le da un
  objeto `0x08C374` de tipo `0x30` (`08CD70`: `0040BC` tras él, `+70` =
  FFFF; objeto y creador en `+72/+76`), `2386A4/2386C2/2386CC/238666/
  2386E6/238670/2386D6`; mientras siga un nodo más profundo (`00408C`, nuevo
  servicio `has_children` del entorno) espera, luego `+83` de ambos.
  Recorte `032646` → `004188`.
- Prueba `porter` (`--terrain 10c72e --node-fields 72,76 --pickup
  2836,0,ffff`: `--node-fields` compara los punteros a nodos a través de la
  correspondencia): **246 actualizaciones (2601..3016), 34020 bytes
  iguales**, el objeto recogido en 2836 (puntos).
- Mundo: `extract_soldier_world.py` trata un creador marcador (`003F1E`)
  como su propio creador (el porteador sigue siendo del mundo cuando la
  figura se recorta); `test_neo_world.c` compara `+72/+76` del porteador por
  el mapa de nodos. 5248 actualizaciones, 28211 estados de actores
  genéricos, 6 llamadas al RNG menos inyectadas.
- Gráficos: `unc1@3264e-32940`: 2355 descriptores, paleta nueva `0x92`; 14
  volcados: 10495 píxeles iguales, 0 distintos.

## Otras paradas explícitas de esta tanda

- **Tanque SV-001 `0x092A32`** (la "cañón" de las notas anteriores; captura
  `pri1`: el jugador se sube en 7841 y la HUD pasa a CANNON). Desde la
  espera vacía (`0x092B0E`) usa el sistema de vehículos del jugador original
  (`0x0AA610` elige al jugador cercano, `0x0AA760/0x0AA83A` lo ata a la
  ranura `10C608/10C62C` y cambia la HUD, mando `0x0CA2CE`), que el port no
  tiene: sigue externo.
- **Prisionero `0x099DCC`**: la traza `pri1` solo tiene su espera atado (11
  actualizaciones). La nueva captura `captures-m1full/fam2`
  (`MSX_REST_FAMILY_ROOTS=99dcc,5ed84,403a0,9a67e,4857c,9d846,9d862,9d876,4115a,9f682,1913c,3458e
  MSX_REST_BOSS_FROM=8000 MSX_REST_LAST=10600`, cámara 2336 → 3192) recoge
  su liberación y las familias siguientes (helicóptero `0x0485xx..0x048Axx`,
  espadachines `0x09Dxxx/0x09Exxx`, lanzacohetes `0x05EFxx..0x05F9xx`,
  cohetes `0x09A0xx..0x09A5xx`) para la próxima tanda.

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

Ver la tabla de cobertura de MISION1_COMPLETA.md. Trazas de familia ya
extraídas para el siguiente paso (MAME `captures-m1full/pri1`,
`MSX_REST_FAMILY_ROOTS=8c37c,99862,92a32,99dcc,9d846,9d862,9d876,5ed84,403a0
MSX_REST_BOSS_FROM=7200 MSX_REST_LAST=8100`; en esa partida la cámara llega a
2336 en ~7770, así que solo aparecen las familias de tras el carro):

| Fichero | Rangos | Contenido |
| --- | --- | --- |
| `data/cannon_fixture.json.gz` | `92a00-95600`, `3b4e0-3b600`, `14a4e-14a60`, `3523c-35300` | cañón `0x092A32` (+ `0x093278`, `0x0954DC`) y sus hijos, 219 actualizaciones (7772..8100) |
| `data/prisoner_fixture.json.gz` | `99862-99dc0` | prisionero `0x099862`, 124 actualizaciones |
| `data/prisoner2_fixture.json.gz` | `99dcc-9a5ee`, `9a97c-9a9c2` | prisioneros `0x099DCC` de `fam2` (prueba `prisoner2`, más abajo) |
| `data/hitem_fixture.json.gz` | `8c37c-8c700` | objeto H `0x08C37C`, 129 actualizaciones (depende del sistema de armas del jugador, que el port no tiene) |

Los espadachines `0x09D846/62/76`, el lanzacohetes `0x05ED84` y el
helicóptero necesitan otra captura que llegue más lejos (en `pri1` la
partida no pasa de 2336 antes del fotograma 8100).

Resuelto (commit `159900f` y siguientes): los escombros `scene_collapse_cells`
y el parche de la tienda `scene_patch_cells` solo se dibujaban en el camino
clásico; el camino `depth25d` (el de publicación) los dibuja ahora en su capa.
Comprobado en ares (`/workspace/emulator/fixr/rubble.png`,
`/workspace/emulator/m1b/shots/shop-destroyed.png`). Además:

- Prioridad: el camino `depth25d` dibujaba las capas 0..4 en la primera
  pasada y nada en la segunda, así que los sprites con el bit 2 de `+15` (las
  cúpulas) quedaban delante de la fachada (visto en MiSTer). Ahora respeta
  `begin/end` como el clásico (capas 0..2, sprites bit 2, capas 3..4; la
  malla tras la capa 4); ares `m1b/shots/domes-risen.png`. El número de
  dibujos por fotograma no se duplica (máx. 400–443 en el jefe).
- Todos los sprites (actores, efectos, jefe, Marco) se recortan a la ventana
  activa de 304×224 (`8..312 × 8..232`), como las capas; el recorte por
  columnas del jefe se interseca con ella y se omite si queda vacío.
- Con un reemplazo activo solo las columnas dentro del rango reemplazado se
  dibujan celda a celda (antes toda la capa 2 o 3 desde la tienda/derrumbe):
  441 → 400 dibujos en la arena del jefe.

Resuelto después (informe del usuario en MiSTer: «falta parte del
edificio»): justo tras el derrumbe, con la cámara aún en 1920, la zona
`x < 2016` bajo la fachada desaparecida mostraba el color de fondo. La
captura `captures-m1full/col1` (volcado en cada fotograma 6980..7035,
`MSX_REST_EXTRA_DUMPS`) muestra que en la recreativa los escombros `0B53A8`
ocupan toda la pantalla desde el cambio (cámara 1922..2010 en 6994..7017):
`tools/extract_mission_map.py col1 --frames 6997:7035` da sus celdas de capa 3
y `tools/merge_rubble_cells.py` añade a `swapped_cells` las 46 de las columnas
1920..2000 (las 112 ya conocidas, idénticas en código, giro y paleta): 158
celdas. Las columnas 1856..1904 no se ven con la cámara en 1920 o más. Otros
problemas: 646 píxeles distintos en la validación de sprites de los
espadachines árabes (arriba).
