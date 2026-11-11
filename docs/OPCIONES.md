# Menú de opciones antes de jugar (petición del usuario, 11-XI)

## Lo que trae la ROM (cabecera de `250-p1.p1`, tablas de «soft DIP»)

Punteros en 0x116 (JP, 0x3388), 0x11A (US, 0x2F78) y 0x11E (EU, 0x316E).
Formato del BIOS: nombre de 16 bytes; dos ajustes de tiempo (FFFF: sin uso);
dos de cuenta (FF: sin uso); diez bytes de lista (nibble bajo = número de
opciones, alto = la de por defecto; 00 termina); luego las cadenas de 12
caracteres (nombre de cada ajuste y, tras el de cada lista, sus opciones).
El BIOS copia la elección a la RAM `10FD84..`: cuenta 1 en `10FD88`, cuenta 2
en `10FD89`, lista n en `10FD8A+n` (índice de la opción).
Cabecera EU: `ff ff ff ff 03 ff 12 38 04 12 12 02 01 24 14 02` (536 bytes con
las cadenas, hasta 0x3386).

| Ajuste (texto de la ROM) | Valores | EU por defecto | RAM | Quién la lee (verificado) |
| --- | --- | --- | --- | --- |
| HERO | 1..99 | 3 | `10FD88` | `0C942C`/`0C9420`: vidas al empezar y al continuar (0 cuenta como 1) |
| CONTINUE | OFF / ON | ON | `10FD8A` | `0186D4`, `019008`, `0C9848` |
| DIFFICULTY | LEVEL 1..8 | LEVEL 4 (3) | `10FD8B` | `0260D8` (tablas `0260C8`), con modo `100088 = 2` |
| PLAY TIME | 60 / 70 / 80 / 90 | 60 | `10FD8C` | `003586` (tabla `00357E`) |
| DEMO SOUND | OFF / ON | ON | `10FD8D` | `012160`, `0122AE`: con `100088 = 1` (atracción) y 0, el comando de sonido no sale |
| PLAY MANUAL | OFF / ON | ON | `10FD8E` | `016BD4`: tras Start en la ruta MVS, HOW TO PLAY (`0C8CE6`) solo si no es 0 |
| BLOOD | OFF / ON | OFF (JP: ON) | `10FD8F` | `01B2BE`, `032B94` |
| NEXT | (`\x13`) | — | `10FD90` | ninguna: separador de página del BIOS |
| CREDIT/LEVEL | OFF/OFF..ON/ON | ON/OFF (2) | `10FD91` | `00354E` (bit 1), `003566` (bit 0): textos CREDIT y LEVEL de la ruta MVS |
| LANGUAGE | JAPANESE / ENGLISH / PORTUGUESE / SPANISH | ENGLISH | `10FD92` | `0023C8`: copia a `100000` (región de textos; el juego solo distingue 0 = japonés de no 0) |
| COUNTRY (solo EU) | OTHERS / USA | OTHERS | `10FD93` | `00241C`: `100001` con región EU |

La pantalla muestra los siete de `10FD88..10FD8F` (los pedidos); los cuatro
últimos quedan fuera: NEXT no es un ajuste, CREDIT/LEVEL y COUNTRY solo
cambian textos de la ruta MVS que el port no dibuja, y LANGUAGE ver abajo.

Pruebas: lecturas por desensamblado de `250-p1.p1` y captura de MAME 0.264
(BIOS euro, `tools/mame/run_softdip.sh`, `tools/mame/capture_softdip.lua`:
fuerza bytes de `10FD8x` y registra cada primera lectura por PC, el modo
`100088` y el latch de sonido `320000`):

- Sin forzar: `10FD8E` leído en 760 (valor 1) y vidas `0C942C` en 3449 (tras
  HOW TO PLAY). Con `10FD8E = 0`: `0C942C` en 763 y SOLDIER SELECT en
  pantalla en 840 (sin HOW TO PLAY).
- `10FD8D = 0` desde el fotograma 301: ningún comando de sonido de 335 a 633
  (título de atracción, `100088 = 1`; sin forzar: `10`, `E1` ×10, `E5`); desde
  761 (`100088 = 2`) los mismos comandos que sin forzar.
- `10FD8B` en `0260D8` vale 3 por defecto; la rama `0260E8` (rango ≥ 7 y
  `d7 ≥ 2`) no cambia nada porque `0260A6` deja `d7` en 0 o 1: el rango 7
  usa la máscara de bits como cualquier otro (`ga_rank_word`, sin cambio).
- El BIOS euro (`sp-s2.sp1`, C1A324..C1A4BE) edita así: arriba/abajo sin dar
  la vuelta; A suma, B resta; una lista da la vuelta; la cuenta 1 va de 1 a
  99 y da la vuelta (C1A47C).

**Idioma:** LANGUAGE existe en la tabla (con SPANISH), pero el juego solo usa
`100000` como «japonés o no»: no hay textos en español ni portugués en la ROM.
No se añade (regla de no inventar); el port fija los textos en inglés.

**Ruta casera de la ROM (AES), no traducida:** el juego trae su propio menú
de consola en `065E80..` (GAME START / OPTION / HOW TO PLAY; OPTION con HERO,
DIFF. EASY/NORMAL/MVS/HARD y la configuración de botones), que lee y escribe
`10FD88`/`10FD8B`; en el arranque AES `0024A4` → `003596` pone `10FD8B = 2` y
`0CA812` restaura `10FD88`/`10FD8B` desde `10F086`/`10F087`. Pendiente si se
quiere ese menú en lugar del de soft DIP (el pedido por el usuario).

## Implementación (añadido N64, sin tocar la lógica del juego)

- `tools/extract_softdip.py mslugx.zip` (también `tools/import_rom.py`) copia
  la tabla EU tal cual a `filesystem/softdip.bin` (dato de la ROM: nunca en
  git); el Makefile la mete en el cartucho si existe. Sin ella no hay
  pantalla y los bytes son los de las capturas. En `ws-logo` falta correrlo.
- `neo_softdip[8]` (`src/neo_world.c`) es la RAM `10FD88..10FD8F`; la leen
  vidas y continuación (`gameplay.c`), rango (`neo_world.c`), tiempo, sangre,
  sonido del título (`title_intro.c`) y HOW TO PLAY (`main.c`). En las
  pruebas de host conserva los valores de las capturas (BLOOD: `MSX_BLOOD`).
- `src/msx_options.c`: al arrancar lee la tabla (valores por defecto: los de
  la tabla EU, BLOOD OFF incluido) y la EEPROM; la pantalla va entre el
  título y HOW TO PLAY / SOLDIER SELECT (se llega con Start desde el título),
  con el nombre del juego y los siete ajustes, textos de la tabla, fuente fija
  de la ROM (página 3, paleta 0, como PAUSE; cursor: glifo 31F). Mandos del
  BIOS con el mando del port: N64 B (= A del Neo Geo) suma, N64 A (= B)
  resta, arriba/abajo; Start sale.
- EEPROM 4 Kbit (cabecera del cartucho, `N64_ROM_SAVETYPE = eeprom4k`):
  bloque 0 `MSXO`, versión 1, 8, suma; bloque 1 los ocho bytes. Solo se
  escribe si la elección cambió; un contenido inválido da los de la tabla.
  Memoria: la tabla y la fuente (8 KB) solo mientras dura la pantalla; el
  código (pantalla + EEPROM, ~7 KB) llevó la caché de escena por debajo de su
  suelo (`RAM budget: 3326 KB left ... (minimum 3332 KB)`), y se recuperó
  compilando `-Os` código de arranque y de contabilidad por fotograma
  (`COLD_BOOT_SRCS` del Makefile): de nuevo «3331 KB foreseen» como antes
  y 208 ranuras.
- DEMO SOUND OFF: el título (la atracción del port) sin audio. PLAY MANUAL
  OFF: sin HOW TO PLAY. El logo NEOGEO (BIOS) no depende de `10FD8D`.
- Prueba: `tools/test_options_ares.py` (en `tools/regression_ares.py`): HERO
  5, LEVEL 8, PLAY MANUAL OFF, Start → bytes, guardado, SOLDIER SELECT sin
  HOW TO PLAY, juego con 4 vidas en pantalla y rango 7; segundo arranque con
  la EEPROM.
