# Personajes jugables: Tarma, Eri y Fio (agente C, rama `work-chars`)

Estado: **solo Marco es jugable.** Hecho en esta rama: el análisis de la ROM
(selección y tablas por personaje), la herramienta de captura que elige el
personaje con el mando, y la comprobación en MAME 0.264 de que la elección por
entradas da cada personaje. Falta todo el catálogo visual de los otros tres.
SOLDIER SELECT sigue aceptando solo a Marco: activar los otros sin sus
sprites sería inventar (Marco con otra voz).

## 1. La elección en SOLDIER SELECT (MAME 0.264, solo entradas)

Sonda `tools/mame/capture_marco_catalog.lua` con `MSX_CHARACTER=n` (y
`run_marco_catalog.py --character n --first 2350`): moneda 700, Start 760,
A 1450 (salta HOW TO PLAY). Las puertas se abren hacia 1500 y el cursor P1
aparece sobre Marco hacia 1650. Orden en pantalla: **Marco, Eri, Tarma,
Fio**. Derecha una vez por casilla desde 1700 (pulsada 10 fotogramas, cada
20) y A en 1800. Resultado en el nodo del jugador `1014C4`, campo **+6E**
(palabra) leído a 1850..2600:

| Derechas | +6E | Personaje |
| --- | --- | --- |
| 0 | 0 | Marco |
| 1 | 2 | Eri |
| 2 | 1 | Tarma |
| 3 | 3 | Fio |

El aterrizaje va 250 fotogramas más tarde que con el A en 1550 de las
capturas de Marco (rutina del nodo `0C1FA6` a 2600; Marco con A en 1550:
2039). Con A en 1550 el cursor aún no existe y la elección es Marco (por eso
las capturas antiguas eran siempre Marco). En el ataque (`attract`) el nodo
lleva +6E = 2 (Eri).

## 2. Qué cambia por personaje en la ROM (todo indexado por +6E)

Barrido con capstone de `move.w $6e(a6)` en `0BD000..0CC000` (lista en el
scratchpad, regenerable con un desensamblado lineal de P1):

- **Programas de sprites.** `0C0FB6` (cuerpo, `004256`) y `0C0FCE` (arma,
  +88, `004260`) toman `a0` = tabla de filas de 0x10 bytes: 4 largos, uno
  por personaje, fila = forma × 0x10 (la "fila +0x20" de la forma gorda en
  `marco_visual.c` es esta). Ejemplos (columnas Marco, Tarma, Eri, Fio):
  - cuerpo reposo `0BF6D8`: 24C5EA, 24C5EA, 24C5FC, 24C5FC (piernas
    compartidas Marco/Tarma y Eri/Fio);
  - carrera `0BF718`: 24DD18, 24DD18, 24DFF0, 24DFF0;
  - arma reposo `0BF858`: 24C60E, 24C9AC, 24CD4A, 24D0E8 (torso propio);
  - muertes `0BF298`/`0BF318`, entrada `0BF498`, victoria `0BF658`: los
    cuatro distintos.
  Así que cada rol del catálogo de Marco (`tools/compile_marco_campaign.py`,
  `build/marco_campaign.h`) es «tabla + forma × 0x10 + 4 × personaje»: la
  generalización es elegir la columna, no inventar roles.
- **Paletas** (`009EA8` con d0 = número de paleta, `009D1E` con d1):
  `0BEDD0` 0376/0377/0004/0005 (normal), `0BEDD8` 03F8/03F9/01BE/01BF,
  `0BEDE0` 0053/0054/0050/0052, `0BEDE8` 0056/0056/0055/0056,
  `0BEDF0` 0136..0139, `0BEDF8` 0391/0392/00A0/00A1, `0BEE00` 0070..0073,
  `0BEE08` 0132/0132/0131/0132, `0BEE10` 0119..011C (transformación
  `0C17B0`), `0C3152` 01D4/01D5/01D4/01D6 (`0C3176`). Usos: `0C1142..0C121A`,
  `0C1760`, `0C182E`, `0C2AE2..0C2E0C` (muertes), `0C3620`, `0C507E`.
- **Voces.** Muerte `0C294E`: Marco 1123, Tarma 1087, Eri 1125, Fio 114E
  (`0C2962` contacto, `0C2D82` caída). Las voces de las armas `0C08F8` (por
  id de arma, `0C09BA`) son del locutor: comunes. Falta comprobar en una
  captura las voces de victoria/«mission complete» por personaje.
- **Fio en el aterrizaje.** `0C4C28`: solo con +6E = 3 crea el actor
  `0C4C64` (programa `253322`, paleta 5 por `009EA8`), una pieza extra del
  paracaídas que cae con su propia física (`0C4CBA..`). Hay que traducirla
  junto a `neo_landing.c`.
- **Otros lectores de +6E** sin analizar: `0BD21E`/`0BD26C` (tablas con
  terminador FFFF/FFFFFFFF), `0C5064` (programa `0C5054`: 0C4F58 Marco y
  Tarma, 0C4FD6 Eri y Fio), `0C35AE`/`0C48FA` (copian +6E a hijos: disparos,
  granadas).
  Ninguna diferencia de juego (velocidad, salto, armas) apareció en el
  barrido del código del jugador; queda por confirmar con capturas de
  movimiento por personaje (`--motion-trace`) frente a las de Marco.

## 3. Trabajo pendiente (en orden)

1. Capturas por personaje con `run_marco_catalog.py --character n --first
   2350` y las mismas variantes que Marco (catálogo base, granada/H abajo,
   C, H, cuchillo, armas 02/04/06, gorda, momia, descenso, victoria pistola
   y H): ~15 capturas × 3. Ojo al disco: borrar los volcados tras convertir.
   `run_marco_catalog.py` lanza su propio Xvfb (`-displayfd`); para usar
   solo :73 hay que darle la opción de un DISPLAY existente.
2. `convert_marco_programs.py`/`compile_marco_campaign.py` con un parámetro
   de personaje (la columna de cada tabla) y salida `build/<pj>_campaign.h`
   o, mejor, un bloque binario en el romfs (programas, descriptores y
   paletas) cargado al elegir, para no triplicar el programa.
3. Exportar piezas y paletas (`export_marco.py`, `build_marco_poses.py`) de
   las mismas capturas: las piernas Marco/Tarma y Eri/Fio se comparten.
4. `marco_visual`/`marco_renderer` con un índice de personaje (Marco = 0
   con la salida actual idéntica: todas las pruebas de Marco en verde), voz
   de muerte por la tabla `0C294E`, paletas de `0BEDD0..`, la pieza de Fio.
5. Pruebas bit a bit de poses y disparo por personaje contra las capturas.
6. SOLDIER SELECT: la película capturada solo muestra a Marco elegido. Para
   los cuatro hace falta capturar la elección y la entrada de cada uno
   (`select`/`confirm`/`entry.msxc` por personaje) o traducir la pantalla
   de selección; el traspaso `MSX_ENTRY_*` es por personaje.

**Siguiente paso:** la captura del catálogo base de Tarma (`--character 1`)
y medir cuánto del conversor de Marco funciona cambiando solo la columna.
