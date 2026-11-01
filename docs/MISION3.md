# Misión 3: fase 1 (flujo, escena, paquete, auditoría, recorrido)

Rama `work-m3` (desde `ccr-09eb2f6b-8rwl8l`). Misma regla que en la
[Misión 2](MISION2.md): lo traducido tiene evidencia de MAME; lo no
recuperado se detiene y se cuenta. ROM, capturas y derivados (mapa,
rejilla, paquete) siguen fuera de git.

## Flujo

`run4` (`data/mission_flow_fixture.json`): la Misión 3 es **un solo
registro de etapa**, `0173F6` (tipo 0, entrada 0, salida 8, como la etapa 1
de la Misión 2), seguido del fin de misión `017412` (tipo 4, `+4` = 0).
Descriptor `016E0E`: lista de cámara `0B8968`, jugadores `2D5E86`, guion
`0x810` (sección `2D9CDA..2DB694`), música `24`, etapa **4** (42F6). En
MAME: carga en 117975, bucle `0186A0` en 118023, fin de misión `018870` en
149172 (unos 31 000 fotogramas).

- `01891E` ya elegía la Misión 3 tras la Misión 2; `actor.c` carga
  `romfs/mission3.pack`. El índice de etapa del paquete es ahora 42FA
  (registro dentro de la misión: Misión 2 0..2, Misión 3 0), no 42F6 − 1
  (daba 3 en la Misión 3), y el descriptor del paquete debe ser el del flujo
  (si no, parada explícita).
- Fin de misión: el 018F92 (10C300) de la Misión 3 levanta
  `mission_complete` (única etapa).
- `scene_assets.c`: una tabla vacía del paquete (la Misión 3 aún no tiene
  sección de actores) no se lee (`dma_read` de 0 bytes era un assert).

## Escena (privada, `filesystem/mission3/s1`)

```sh
C=/home/user/captures-fullgame/run4
python3 tools/extract_mission_map.py $C --ram-layers --allow-backdrop-timeline --frames 117966:149171 --output filesystem/mission3/s1
python3 tools/extract_region_collision.py $C --rom mslugx.zip --descriptor-layers --layer-camera --both-layers foreground --frames 117966:149171 --output filesystem/mission3/s1/collision.json
F=filesystem/mission3/s1/collision.json
python3 tools/fill_collision_rom.py $F --rom mslugx.zip --descriptor b853a --top 45 --columns 860
python3 tools/fill_collision_rom.py $F --rom mslugx.zip --descriptor b8550 --column 520 --top 33 --outside-empty
python3 tools/pack_mission.py 3 filesystem/mission3/s1 --loose-layers --report build/mission3_pack_report.json
```

**Novedades del motor que la Misión 3 usa** (es la etapa del tren):

1. **Cuarto objeto de capa `10E8C6`** activo en toda la misión (ranuras 1,
   33, 113, 145 de `10E83E/10D7B6/10E8C6/10C72E`). `neo_scene.py` lo toma
   como capa lógica 2 y `10D7B6` como capa 1 a cualquier velocidad (orden de
   dibujo = orden de ranuras); pistas de traza `obj4_x/obj4_y`. Su mapa
   (`2A6142`) solo aparece un momento (cámara ≈ 3944): superposición **no
   empaquetada**.
2. **Capas que se mueven solas**: el cielo (`10E83E`) pasa del mapa `0B857C`
   al puntero 0 en la cámara ≈ 320 (lo dibuja otro escritor, no un mapa) y
   `10D7B6` (mapa `0B8566`, **el tren**) avanza con su propio movimiento: su
   origen de columnas (+32) se mueve durante todo el recorrido y da la vuelta
   cada 4096 columnas (65536 px). Los fotogramas cuyo origen cambia dentro de
   la ventana de retraso se saltan (19). `pack_mission.py --loose-layers`
   desenrolla los desplazamientos, considera una capa activa solo con su
   primer mapa y guarda el mejor ajuste por mediana: capa 0 x 1 hasta la
   cámara 321; **capa 1 x 1 con error mediano 532 px (peor 2240)**: el
   movimiento del tren no es una regla de cámara (pendiente: su escritor).
3. **Cambios de mapa del primer plano** `0B853A → 0B8600 → 0B8550` (op 0x18
   de la lista de cámara en `0B8BB8`/`0B8C1C`): las celdas coinciden con el
   mapa principal donde ambos se vieron (0 diferencias), así que el paquete
   las une al mapa (1063 celdas).
4. **Dos rejillas de colisión**: `10C72E` (`0B853A`, luego `0B8550`) y también
   `10D7B6` (`0B8566`, el tren). La lista de objetos del núcleo (`01382A`,
   `neo_kernel.c`) solo modela `10C72E` y un `10D7B6` inactivo: la rejilla
   de la etapa es la del primer plano (`--both-layers foreground`) y **la
   colisión móvil del tren queda pendiente**.

Colisión: `0B853A` encaja en la columna 0 con la fila superior 45 (13448
celdas observadas iguales, 0 conflictos); `0B8550` en la columna 520 con la
fila 33 (6640 iguales, 0 conflictos). Las 8668 celdas restantes están fuera
de las dos rejillas de ROM (filas por encima/debajo del mapa y las columnas
480..519 de `0B8600`, que no tiene puntero de colisión) y se marcan vacías
(`--outside-empty`). La rejilla observada tenía 4046 columnas (columnas
envueltas pasado el final): se recorta a 860.

| Etapa | Mapa | Variantes | Fondos | Rejilla | Desconocidas |
| --- | --- | --- | --- | --- | --- |
| 4 (tren) | 7150 celdas, cámara 0..6496 | 2043 | 7 valores | 860 × 47 | 0 (145 mutables) |

Paquete `romfs/mission3.pack` (v3, sin actores aún): 352 páginas, 44
bloques, 46 paletas, 5558 imágenes, **0,93 MiB**.

## Auditoría del guion

`tools/audit_script_events.py build/host/script-native --script
data/mission3_script.json`: 256 registros, 44 constructores, 13 nativos;
**77 apariciones contadas que el port pierde** (en `make check`, solo informe). Las más frecuentes:

| Constructor | Contadas | | Constructor | Contadas |
| --- | --- | --- | --- | --- |
| `097FCC` | 20 | | `0A2F2E` | 5 |
| `09BA2A` | 9 | | `05D916` | 4 |
| `02B202` | 6 | | `05C53C`, `04E990` | 3 |
| `08A732` | 6 | | `0391EA`, `0569D0`, `084588` | 2 |
| `08DA12` | 5 | | `09AE2C 0AF1B6 027F54 09AE1E 05D234 05D226 08AFB6 0B9400` | 1 |

Ninguno comparte cuerpo con una familia nativa (`097FCC` → `0120F8` +
`0985A0`; `09BA2A` → `09CB70`; `02B202` → `02BDA6`; `08A732` y `08DA12` con
sus tablas propias; `0A2F2E` → `0A3DB0`): **no se ha portado ninguna
familia de la Misión 3 en esta fase** y `filesystem/mission3/rest-native.json`
no existe todavía (el paquete se construye sin sección de actores). Sin
contar: `01913C 01F1D4 0214EA 03EE8A 0569B4 0569D0 0569E6 093C9A 099E1C
09AE1E 0AC866`.

Tabla de spawn: `compile_soldier_ai_tables.py` añade los desplazamientos del
guion de la Misión 3 (sin ellos el guion paraba en su primer registro,
`2D9CEE`).

## Recorrido (`tools/test_mission3_ares.py`, `mslugx-m3-reference.z64`)

Cartucho de referencia (nunca una versión): `MSX_REFERENCE_START_MISSION=3`
salta al registro `0173F6`; mismos ganchos que el de la Misión 2 (jugador
invulnerable, registros de constructores no portados saltados y registrados
con su dirección, paradas de cámara del guion soltadas). Mando: el de la
prueba de la Misión 2 sin piloto.

Resultado (300 s, `RESULT: PASS`, umbral de progreso 1000): la etapa carga
en 506 ms (tablas 364 KB en 23 ranuras, mapa 44 bloques), Marco entra
andando y la cámara (modo 0) llega a **x 1901 (progreso 1900 de 6496)**,
0 celdas desconocidas leídas, 0 eventos de cámara sin soporte. Ahí para: el
jugador queda entre los vagones (x 1929..2012), sin subir a la rampa de
x 2000..2072. 18 registros saltados (constructor, registro):
`09AE2C 2D9F38`, `09BA2A 2D9FA0/2D9FB8/2DA68A/2DA6E2`, `08DA12
2DA030/2DA11A/2DA23E`, `02B202 2DA18A/2DA274/2DA3DE/2DA428`, `027F54
2DA4A6`, `05D916 2DA646`, `0391EA 2DA6B4/2DA800`, `08A732 2DA6FA`,
`09AE1E 2DA790`. Rutina sin soporte del mundo: `0AF17E`. 54 dibujos sin
descriptor (`descriptor` 10 y 6). Rendimiento: peor media de ventana 16,8
ms de CPU.

## Presupuesto

ROM: 31 948 800 B (30,5 MiB) con `mission2.pack` (3,1 MB) y `mission3.pack`
(0,93 MB); la Misión 3 no añade datos al programa salvo ≈ 1 KB de la tabla
de spawn (`mslugx-explorer.elf`: texto 1 724 844 B, datos 19 328 B, bss
160 788 B, total 1 904 960 B). RAM: tablas de la etapa 364 KB en 23 ranuras de la caché (219
ranuras, 3504 KB); el presupuesto fijo no cambia.

## Fase 2: el tren (rama `work-m3b`)

**Capa `10D7B6` nativa** (`src/neo_train.c`). El escritor de su `+14` es
`0C81C0`/`0C6F9E` (paso de capa: bit 0 de `10E956+i` → la capa suma su
velocidad propia `10E94E` en vez de la de la raíz `10E970`; op 0x3C de la
lista de cámara en el progreso 0x140: `10E957 = 10E958 = 1`). `10E94E` lo
escribe el conductor `0B8D64` (creado por `0B8D34` desde la lista de
cámara): raíz (× 3/4 en el modo 2) + `10C6B8`, que con `10E9D0 = 0` salta al
suelo del modo (`0B8CFC` modo 1: −2 px; `0B8D06` modo 2: −12 px). El
conductor corre tras las capas: la capa usa la raíz de la actualización
anterior con el modo vigente. El origen de columnas `+32` lo mueve `0B8DD0`:
mientras `10C6B6 ≠ 0`, si `tabla[10C6B4] + 32 − 0x20 < +36` (con `+36 =
(−x >> 20) − 4`, `0C748E`) suma 0x20 (el tren repite 32 columnas antes de la
columna de la tabla `0B8E36`: 0x50, 0x78, 0xB0, 0xE0, 0x138, 0x188; las
elige `0B8E7E..0B8EA4`). `test_neo_train` (`data/train_fixture.json.gz`,
`tools/extract_train_fixture.py` desde run4): **14252 actualizaciones, `x`
y `+32` exactos**. El desplazamiento observado de la capa es `−x − 16·(+32)`
con un paso de retraso (553 de 614 observaciones exactas, el resto es el
retraso de la instantánea): el renderizador dibuja la capa 1 así en vez de
la regla ajustada (deriva mediana anterior 532 px). Las celdas del paquete
siguen siendo las primeras vistas por posición (módulo 65536).

**Colisión** (`neo_kernel.c`): `01382A` recorre `10D7B6` tras `10C72E` con
la ventana vertical de `013856..013896` (`(+34 − 0x11 + 1)·16 + y`, 7 filas
de 16 px). La rejilla `0B8566` (784 × 48) es una plataforma `0x2B` en la
fila 36 de cada columna salvo 384..447 (`0xA0` en las filas 46/47):
`neo_kernel_train_cell`, comparada con la ROM por `tools/test_train_grid.py`
(`make check MSX_ROM_ZIP=...`). En el anillo la fila de mapa r ocupa la fila
r + 20 (RAM `10D83E` de run4). El jugador puede aterrizar en ella pero no
viaja con el tren (en run4 el jugador tiene siempre `10C72E` como padre en
este tramo: x 1338..3179 con `+38 = 10C72E`).

**Descriptor que faltaba**: los 59 dibujos sin descriptor del recorrido eran
todos `18653C` (de `filesystem/mission2/rest-native.json`; `actor.c` registra
ahora la última etiqueta que falta). `filesystem/mission3/rest-native.json`
(privado) lleva ese marco y las paletas de la Misión 2; el paquete
`mission3.pack` tiene ya sección de actores (1 descriptor, 0,95 MB).

**Recorrido** (`mslugx-m3-reference.z64`, 300 s): igual que en la fase 1,
cámara 1901, jugador hasta x 2017. El bloqueo no es el tren: en x 1930..2000
el jugador **cae** (y hasta −93, 147 rescates) a través del suelo del vagón,
celdas `0x31..0x3B` del grupo 1 en las filas 17/18 de `10C72E`. El barrido del
jugador (`0128E8` pone `10C00D` siempre) borra el grupo 1 en `0129A2`, y la
Misión 3 no tiene tabla de grupos (`10C00E = 0` en la RAM de run4), así que
`player_surface` (`neo_collision.c`) las ignora igual que el original. En
run4, sin embargo, el jugador pasa por encima (x 1953, `+38 = 10C72E`): falta
identificar sobre qué celda se apoya (la conversión de y entre la RAM y la
rejilla del paquete no está cerrada; con y local = y + `10C72E +18` − 256 da
la fila 17).

## Pendiente

1. Recorrido: el suelo de grupo 1 del vagón (x 1904..2095, ver arriba); luego
   más allá de la cámara 1900 hasta el jefe y el fin de misión.
2. Capas: el cielo (`10E83E`) usa también `10E94E` desde el progreso 0x140
   (velocidad 0x40) pero su mapa pasa a 0; su escritor (probablemente el actor
   `0B8EDC` de `0B8D34`, tabla `0B8EC2`) y la superposición `2A6142` de
   `10E8C6`; los 7 valores de fondo. Las celdas empaquetadas del tren son las
   primeras vistas por posición; el bucle de columnas no se refleja en ellas.
3. Familias nativas, por frecuencia: `097FCC` (sonido 10BA, `0985A0`, hijo
   `098E4A`), `09BA2A`, `02B202`, `08A732`, `08DA12`, `0A2F2E`, `05D916`, y
   las rutinas `0AF17E` (constructor de soldado) y `09A842` (trazas con
   `MSX_FULL_FAMILY_ROOTS` desde `states4/sec04_117966`, como
   [MISION2_ACTORES.md](MISION2_ACTORES.md)).
