# Fin de misión y fin de partida: recuento de prisioneros y CONTINUE?

Continúa [MISION1_COMPLETA.md](MISION1_COMPLETA.md) («Fin de misión
`0x018846`» y «Fin de partida y continuación `0x0C9848`»). Dos piezas que
eran paradas explícitas son ahora nativas y se comprueban bit a bit contra
MAME 0.264:

1. El **recuento de prisioneros `0x0308C0`** tras MISSION 1 COMPLETE, con el
   **motor de texto completo `0x03DD96`** (`src/neo_tally.c`,
   `src/neo_text.c`).
2. El **CONTINUE? grande y la cifra de la cuenta atrás**: no son el actor
   `0x019798` (como decía la documentación anterior) sino el programa de
   animación del propio actor de misión `101364`, `0x018120`
   (`src/neo_continue.c`).

Regla de siempre: cada pieza traducida a mano de la ROM tiene evidencia de
MAME en bucle cerrado; lo que no se ha podido probar se detiene
explícitamente, se registra y se cuenta.

## Motor de texto `0x03DD96` (`src/neo_text.c`)

Traducción de `03D25A` (cadena con los escapes de `03D1B8`: `\n`, `\c`,
`\p`, `\l`), `03CFBA` (cifras BCD a través de `03DD96`, con `A0` apuntando
a la tabla `03D056` como en el original) y del escritor de glifos completo:

| Página (`D1` bajo) | Pieza |
| --- | --- |
| `< 0x10` | `03CF9A`: un tile `paleta<<12 \| página<<8 \| glifo` |
| `0x30` y por defecto | `03CF9A` con la página 3 |
| `'a'` (`0x61`) | fuente de dos filas desde `0xA00` |
| `0x31` | fuente de tres filas desde `0x480` |
| `'A'` (`0x41`) | bloques 2×2 desde `0x800` (dos columnas por glifo) |
| `'X'` (`0x58`) | como `'A'`, y los códigos de dos bytes de la tabla `03D512` |
| `'H'`/`'K'` | bloques 2×2 de las tablas `03D596`/`03D796` |
| `'h'`/`'k'` | un tile de las tablas `03DB96`/`03D996`, acentos con `@` (`.`, `:`, `~`, `-` y la vocal) |
| `0x32` | marcas `@!`, `@c`/`@C` y los pares de acento `03E194..03E2A7` |
| prefijos `DE`/`DF` | dakuten/handakuten del glifo siguiente |

Las escrituras `move.l d0,$3C0000` (dirección en la palabra alta, dato en la
baja) se modelan como aritmética de 32 bits, de modo que los acarreos entre
ambas palabras son los del original. Las tablas de fuentes son datos de ROM
importados por `tools/extract_region_rom.py` (`03D512..03DD96`,
`03E194..03E2A7`). `neo_hud.c` conserva su escritor reducido (las páginas
`< 0x10` y `'a'`).

## Recuento de prisioneros `0x0308C0` (`src/neo_tally.c`)

El actor de misión lo crea en `01889A` (`0040BC`, tras él en la lista de fin
de misión de `neo_end.c`) y espera en `0188B0` a que ponga su `+60`.

| ROM | Pieza |
| --- | --- |
| `0308C0`, `0308E4` | orden de audio `01228C(0x60)` (contada, no reproducida: la música del port ya está parada), fundido `009C0A(-8,-8,-8,5)`, espera de 16 actualizaciones, `+90` = byte de región (RAM `100000`), «RECAPTURED PRISONER» (columna `0x0A`, fila 6, paleta 4, fuente de dos filas) |
| `030946..0309C6` | un contador `030B0A` por jugador presente (`01AEB0`); el ausente pone su cuenta a 0 |
| `0309CC` | cada contador suma 1 al `+60` de la raíz en cada actualización; con `+60`, A o B recién pulsados (RAM `10C35D & 0x30`) ponen `+61` = FF (los contadores se saltan sus esperas) |
| `030A34..030B00` | sin contadores vivos: borra la ventana de 34×22 desde `7066` (`030284`, tile `0820`), fundido `(0,0,0,5)`, 16 actualizaciones y `+60` del actor de misión |
| `030B0A` | el prisionero grande (`+3C/+40` = `30/190`, paleta `0x381`, programa `237AEC`), el marco `0301AE` (tiles `0BF0..0BF7`, interior `0B60`) y la regla `0302C2`, «1P» (2×2), `+82` = RAM `10C5FC`, `+84` = prisioneros especiales (`10C5FE`: el prisionero atado `099862` que cuenta `09AB38`, bit 0 de `+8F`; `10C5FF`: el prisionero con `+74 = FFFE`, bit 1), las cifras `+8C`, los bits de resultado de `+8F` (un jugador: 10 o más = GREAT!, bits 4/7; ninguno = NO PRISONER, bits 6/7), el nodo almacén `030B08` (sus `+6E..` guardan los siete pares nombre/grado visibles), «×10000» (`030374`) y el primer icono (`030608`) |
| `030D20`, `0306D0` | cada icono que llega (`+60` = FF) cuenta un prisionero: nombre aleatorio de los 989 de `02CAD6` y grado de `02C982`/`02C9F8` (`0029F2`, dos números aleatorios), o los especiales «Rumi Aikawa SGT.»/«Hyakutaro 2LT.»; lista de siete filas en anillo (`0304D6`, `030548`: grado con la página `'k'`, nombre con `'h'` rellenado a 11 columnas); `+82` y RAM `10C5FC` menos 1; `+8C` más 1 (`030300`, con su comparación `cmpi.b #$90,d1` tal cual) |
| `03040E`, `0303AA`, `0303DE` | 10000 por prisionero (`+72` BCD y la puntuación `057948`), 30000 por uno especial («+30000»), el total en seis cifras 2×2 en `70EF` |
| `030608` | el icono siguiente a las `+7E` actualizaciones (`0xF`); un especial cambia el prisionero grande a `237BD0`/`0x3FA` (`0x3C` actualizaciones) o `237B84`/`0x9B` (`0x3D`) |
| `030DBA..030F9A` | cifras finales, mensaje (`030E3A`: GREAT! con el sonido `0x1150`, DRAW!, NO PRISONER) que parpadea con el bit 3 del contador de vídeo `10000E` cada 8 fotogramas durante `0x5A` actualizaciones (`030EA8`; GREAT! suma 100000 y el sonido `0x10D8`), 60 actualizaciones y se borra |
| `030FB2`, `031060`, `03108A` | icono del desfile: en `(min(+82,14)·8 + 0x1C, 0x11C)` en pantalla, vuela hacia `(0x60, +0x7C)` a velocidad `0x1000` (ángulo `002EB4`, tabla de senos `0029FC`), paleta `0x2A2`, programa `237C48`; en `y >= 0x198` (o con el salto) sonido `0x10D6`, `+60` = FF del contador y se borra |

**Paradas explícitas** (nunca en el port, sin captura): textos japoneses
(`+90` = 0, `030906`), ningún jugador en juego (`03096C`), el contador del
jugador 2 y el resultado a dos jugadores (`030988`, `030C66`: DRAW!, `030B1A`,
`030FDE`), y las medallas de `018FEA` (RAM `10C306/10C307`: `030CAE`,
`030DC8`, `030E0E`; actor `03112E` y la tabla `2D0A74`). En el juego, una
parada de un nodo del recuento deja seguir al actor de misión (se cuenta en
`end_stops`). **No reproducido:** las comprobaciones de cartucho `030A78` y
`03109E` (solo actúan si fallan) y la orden de audio `01228C`. Los sonidos
`0x10D6`, `0x10D8`, `0x1150` se piden a la lista de sonidos del mundo; el
port no tiene todavía su render (se cuentan como no asignados).

### Evidencia

`tools/capture_mission_rest.lua` admite ahora:

- `MSX_REST_VRAM_TRACE=1`: cada palabra de VRAM escrita durante la
  actualización de un nodo de la familia (registro `Z dirección dato`; se
  siguen el registro de dirección `3C0000` y el módulo `3C0004`).
- `MSX_REST_IDLE_TALLY=1`: ninguna entrada desde que se despacha `0308C0`
  (el recuento sin el salto A/B; solo captura).
- `MSX_REST_POKE=fotograma:dirección:valor`: escrituras de un byte solo en la
  captura (p. ej. la cuenta `10C5FC` antes del recuento).

Cuatro capturas (`MSX_REST_BOSS_FROM=13600 MSX_REST_FAMILY_ROOTS=308c0
MSX_REST_VRAM_TRACE=1`, unos 4 minutos cada una):

| Captura | Variante | Recuento |
| --- | --- | --- |
| `tal1` | `MSX_REST_IDLE_TALLY=1` | 11 prisioneros (1 especial), sin salto, GREAT! parpadeando |
| `talK` | (pulsa A/B como siempre) | el mismo con el salto |
| `tal0` | `IDLE_TALLY`, `MSX_REST_POKE=13800:10c5fd:0` | NO PRISONER |
| `tal2` | `IDLE_TALLY`, `MSX_REST_POKE=13800:10c5ff:1` | dos especiales (`+84` = 2: Hyakutaro y Rumi) |

`tools/extract_tally_fixture.py` → `data/tally_<captura>_fixture.json.gz`
(actualizaciones agrupadas por **pasada de la lista**: una pasada del
original puede cruzar el final de un fotograma cuando una actualización se
alarga, p. ej. al escribir la lista de nombres), `tools/compile_tally_fixture.py`
y `tools/test_neo_tally.c` (en `make check`, `TALLIES`). En bucle cerrado:
bytes `+10..+AF` de cada nodo (salvo ranuras de paleta, puntero de animación
y posición en caché; el creador y el puntero al almacén por la
correspondencia de vidas), **cada palabra de VRAM en orden**, los números
aleatorios consumidos, la puntuación, la cuenta `10C5FC` y los objetivos del
fundido:

| Captura | Pasadas | Actualizaciones | Bytes iguales | Palabras de VRAM | Aleatorios | Puntuación final |
| --- | ---: | ---: | ---: | ---: | ---: | --- |
| `tal1` | 601 | 1476 | 204062 | 2771 | 20 | `00285030` |
| `talK` | 310 | 513 | 70226 | 2463 | 20 | `00285330` |
| `tal0` | 390 | 740 | 103222 | 1501 | 0 | — |
| `tal2` | 646 | 1611 | 222782 | 2795 | 18 | `00305030` |

Única exclusión: la raíz nunca arranca un programa propio; su `00427A` de
`030A5E` ejecuta el puntero de programa que dejó el ocupante anterior del
nodo (`+20`); su temporizador (`+17`, `+18`, `+24..+27`) no se compara (el
asignador nativo limpia el nodo). Un borrado que vuelve por `00424A`
(`004188` → `004126`, `+1D` = 0) se compara sin `+1D`.

Sprites (`tools/export_rest_sprites.py`, programas `237AEC`, `237B84`,
`237BD0`, `237C48`, paleta `0x2A2`) contra la VRAM de `tal1`: 101 volcados,
66665 píxeles iguales, 608 distintos (13840: la primera actualización del
prisionero grande, la lista aún no está en la VRAM; 14120: 46 px del cambio
de programa).

`tools/test_gameplay.c`: en las cuatro partidas de publicación el recuento
termina sin paradas, la cuenta vuelve a 0 y la puntuación sube exactamente
10000 por prisionero (30000 por especial) y 100000 con 10 o más; después
siguen las letras y el fundido.

## CONTINUE? y la cuenta atrás (`src/neo_continue.c`)

El actor de misión arranca en `01868A` su programa `018120` (bloque fijo) y
solo lo avanza con su `00427A` de `01883C`, mientras corre la cuenta de la
continuación (`0187F0..01881C`: DIP de continuación, contador `+7E` distinto
de 0, ningún jugador en modo 1, `10F003`, `10E9E3` a 0; `neo_hud.c` pone
`continue_draw`). El programa: `012022` (bit 2 de `+15`), `0180F6` (zoom
`+12` = seno del byte bajo del contador ×2, saturado, menos 1), `+14` = FE,
`+3C/+40` = `A0/170`, la cifra elegida por el byte alto de `+7E`
(`1D913A..1D9120`), `+12` = FF, `+14` = 04, las nueve letras de CONTINUE?,
`WAIT 1` y vuelta. Lo convierte `tools/extract_soldier_programs.py` (raíces
`018120` y `0181E4`, GAME OVER, que el port no usa todavía).

`004364` lee `+12..+15` y `+3C/+40` en cada pose: `neo_animation.c` aplica
ahora una escritura de esos campos (`SET_BYTE/WORD/LONG`) a la pose
siguiente de la misma pasada. Solo estos dos programas escriben `+14`,
`+3C` o `+40` (comprobado en `data/soldier_programs.json`).

Ranuras de paleta fijadas por el programa: la tabla del asignador las asocia
en todos los volcados de `go2` a los recursos `0x63` (ranura 04, letras) y
`0x25F` (ranura FE, cifra); se exportan con `--palette-capture` de `go2`.

**Evidencia.** `tools/mame/capture_gameover.lua` con `MSX_GO_DUMP=60`
(volcados completos: VRAM con las tablas de sprites, paletas, RAM y
pantallas; `actors.csv`, rutinas vivas de la reserva de actores) y
`MSX_GO_TRACE=1` (`trace.txt`: los bytes del actor de misión tras cada
fotograma y cada entrada de la lista de sprites que escribe en `00A078`):

```sh
MSX_CAPTURE_DIR=/ruta/go2 MSX_GO_LAST=4700 MSX_GO_DUMP=60 MSX_GO_TRACE=1 mame mslugx -bios euro \
  -video none -sound none -nothrottle -autoboot_script tools/mame/capture_gameover.lua
python3 tools/extract_continue_sprites.py /ruta/go2/trace.txt --output data/continue_sprites_fixture.json.gz
```

`tools/test_neo_continue_sprites.c` (en `make check`): **511
actualizaciones, 5110 entradas de la lista de sprites iguales a MAME**
(x, y, palabra de zoom, atributos y descriptor), las diez cifras 9..0.

En el juego (`gameplay.c`, `actor.c`): el programa arranca con la misión y
se avanza en cada actualización del HUD con `continue_draw`; sus dibujos van
por encima de los actores de fin de misión y por debajo de la capa fija.

**Parada explícita que queda: el objeto de `0x019798`.** Al empezar la
cuenta (`0C98A6`) el original suelta un objeto (`08CD58`: tipo
`019792[0]` = 0, `+70` = `0x384`, `+72` = `0x10`) en el punto de
reaparición (`0196E8`: registro `10C31A`, barrido `0143E4`); la captura
`go2` lo muestra (nodo `08C53C` creado en el fotograma 3554, la «H» junto
al camión). Crearlo en el mundo desde el HUD necesita el constructor de
objetos y la colocación `0196E8` con su barrido, que el port no tiene como
servicio: `neo_hud` lo detiene y lo cuenta («continue item 019798»).

## Reproducir

```sh
# Recuento (una por variante; ver la tabla de capturas)
MSX_REST_BOSS_FROM=13600 MSX_REST_FAMILY_ROOTS=308c0 MSX_REST_VRAM_TRACE=1 MSX_REST_IDLE_TALLY=1 \
  MSX_REST_FIRST_DUMP=13600 MSX_REST_PERIOD=8 MSX_REST_LAST=17000 tools/run_capture_rest.sh /ruta/tal1
python3 tools/extract_tally_fixture.py /ruta/tal1/boss-trace.txt --output data/tally_tal1_fixture.json.gz
python3 tools/extract_region_rom.py /ruta/captures-soldiers/walk3 --output data/region_rom.json
python3 tools/extract_soldier_programs.py /ruta/captures-soldiers/walk3 --output data/soldier_programs.json
make check
```

`data/region_rom.json` (97 bloques: cadenas `030800`, grados `02C982`,
nombres `02CAD6`, fuentes `03D512`, acentos `03E194`) y
`data/soldier_programs.json` (591 programas) son superconjuntos de los
anteriores (todo bloque y programa previo presente e idéntico).
