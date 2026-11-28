# El TAS de TASVideos #6663 en MAME 0.264 (por actualización del juego)

Fuente: TAS de 1 jugador (BizHawk 2.9.1, núcleo MAME, arranque en frío, RTC
2010-01-01T00:00:00 sin tiempo real, DIP por defecto, 57035 fotogramas).
Privado en `/home/user/tas/p1/` (no va a git). Resultados en
`/home/user/captures-tas/` (no va a git).

## Herramientas

- `tools/tas_convert.py` (ejecutar con `python3 -I`, fuera del directorio del
  TAS: el archivo es dato no fiable): `Input Log.txt` → una máscara hexadecimal
  por fotograma (bits 0..5 sistema, 8..15 P1, 16..23 P2, en el orden de
  `LogKey`). Moneda en el fotograma 280, Start en 302 (líneas 282/304 del log).
- `tools/tas_play.lua` + `tools/run_tas_play.sh`: desde el encendido, cada
  fotograma pone los 22 campos (`:AUDIO_COIN`, `:TEST`, `:edge:joy:START`,
  `:edge:joy:JOY1/JOY2`) con `field:set_value`; **ninguna escritura en la
  memoria**. Fotograma = tiempo emulado / periodo (16 895 999 999 932 416 as,
  el mismo `VsyncAttoseconds` de la cabecera), así `-debug` no lo desplaza.
  Salidas: `tas.csv` (sección del guion, progreso 10E99E, puntuación 10C46A,
  vidas 101288, 10F008.., `10008A` = vblanks del último bucle principal,
  `10000E` = cuenta del bucle), `events.txt`; estados, volcados, fotos y
  órdenes del depurador por variables (cabecera del Lua).
- Reloj: MAME 0.264 no tiene opción para la hora base y el uPD4990A relee sus
  registros de la hora base en cada tic de 1 s (escribir `m_time_counter`
  desde Lua dura menos de un segundo). `tools/mame/fixed_time.c` (LD_PRELOAD,
  `TZ=UTC`) fija el reloj de pared que lee MAME en `MSX_FIXED_TIME`
  (1262304000): el contador arranca en `00 00 00 01 15 10` (viernes) como la
  llamada de hora base de BizHawk (`MAME.cs`: `InitialTime − epoch`). Con el
  reloj del anfitrión dos ejecuciones iguales divergían (10F00C en 3180,
  progreso 2767/2770 en 3797); con el reloj fijo son idénticas byte a byte.
- Argumentos como BizHawk (`MAME.cs` 2.9.1): BIOS por defecto (euro),
  `-joystick_contradictory` (el TAS no pulsa nunca direcciones opuestas),
  cfg y nvram vacíos.

## Resumen

- Por fotograma de vídeo el TAS **no** sincroniza en MAME 0.264 (se pierde en
  el fotograma 3731, primera ralentización distinta) y **sí** en MAME 0.251
  (juego completo: fin en 57062, 982830 puntos).
- Reproducido **por actualización del juego** (`tools/tas_updates.lua`),
  MAME 0.264 juega el TAS entero en sincronía: las 26708 actualizaciones
  tienen la misma RAM que en 0.251 (0 diferencias). Ninguna escritura en la
  memoria: solo los campos de entrada.
- Estados de MAME 0.264 cada 60 actualizaciones (~2 s) y en cada inicio de
  sección, con su foto: `/home/user/captures-tas/sync264/` (índice abajo).
- La referencia del port sigue siendo MAME 0.264 con la ROM del usuario;
  0.251 solo sirvió para saber qué entrada lee cada actualización.

## Por fotograma: 0.264 no, 0.251 sí

- MAME 0.264, desfase 0 (el fotograma k del TAS se emula como el fotograma
  k + 1 de MAME; desfases -3..+8 son peores): juega como el TAS hasta ~4316
  y después pierde las vidas (fin de partida en 10469).
- MAME 0.251 (paquete `mame_0.251+dfsg.1-1_amd64.deb` de Debian bookworm,
  `deb.debian.org/debian/pool/main/m/mame/`, SHA256
  `3f07f51c...b1953ec7` comprobado con el índice `Packages` de bookworm; se
  extrajo solo `usr/games/mame` y las bibliotecas `libjpeg62-turbo`,
  `liblua5.3-0` y `libutf8proc2` del mismo archivo, sin instalar nada; Debian
  no tiene 0.252 y no hizo falta compilarlo). Con `MSX_MAME=<binario>
  MSX_MAME_LIBS=<bibliotecas> MSX_MAME_ARGS=-noplugins tools/run_tas_play.sh`
  (`-noplugins`: los plugins instalados son los de 0.264), la misma ROM, el
  mismo Lua sin cambios y el reloj fijo, el TAS completa las seis misiones y
  el final: secciones 0..11, una vida perdida a propósito en 34855, última
  sección cerrada en 57062 con 982830 puntos (el vídeo acaba en 57035). El
  binario se borró al terminar (357 MB); para repetirlo, la misma receta.
- Primera diferencia entre versiones: fotograma 299, solo `10F00C` (el
  contador del bucle de espera `0039B0..003A34`: tiempos del 68000). El
  estado del juego es igual hasta 3731, donde 0.251 alarga una vuelta del
  bucle principal a 3 vblanks y 0.264 no. Desde ahí cada vuelta lee otro
  fotograma del TAS. En todo el TAS: 0.251 hace 23097 vueltas de 2 vblanks,
  3317 de 3 y 290 de 4 o más; 0.264, 23159, 3300 y 288.

## Por actualización: 0.264 en sincronía

- El bucle principal lee él mismo los puertos una vez por vuelta
  (`03CB3A..03CC60`: `380000`, `300000`, `340000`) y acto seguido cuenta la
  vuelta (`003B36`, RAM `10000E`). La BIOS los lee en cada vblank
  (`C18526..`), pero durante el juego solo cuenta la lectura del bucle. La
  moneda (280) y Start (302), que sí pasan por la BIOS, caen antes de la
  primera diferencia.
- `tools/tas_updates.lua` (con `MSX_TAS_SCRIPT=tas_updates.lua
  tools/run_tas_play.sh DIR`) numera las actualizaciones con una escucha de
  escritura en `10000E` (U = 1.. desde el encendido) y anota con una escucha
  de lectura en `300000` qué máscara leyó el juego:
  - grabar (`MSX_TAS_UPD_REC=<archivo>`, en 0.251, por fotograma): una línea
    por actualización, `U fotograma máscara sección progreso puntuación vidas
    rutina hash_RNG hash_RAM`;
  - reproducir (`MSX_TAS_UPD_PLAY=<archivo>`, en 0.264): por fotograma hasta
    la primera actualización de la sección 0 (U = 180, fotograma 656) y desde
    ahí, en cada límite de fotograma, se pone la máscara de la actualización
    U + 1, tarde los vblanks que tarde. Cada actualización se compara con la
    grabación (`DESYNC` en `events.txt`).
- Comparación: en la escucha (la vuelta anterior completa) se comparan el
  RNG (`100016..100085`, `10F006`) y la RAM `1010A0..10EFFF`. Quedan fuera,
  identificadas desensamblando: `10009A..10109F`, la cola de trabajos
  diferidos de `0036A4` (guarda D0-D7/A0-A5 con cada trabajo; los registros
  muertos difieren entre versiones, p. ej. `101060`, y el trabajo `016502`
  los reescribe antes de usarlos); `10BFE8..10BFF3`, el índice de lectura y
  la respuesta del Z80 del anillo de órdenes de sonido `10BF68` (`0121DA..`,
  una orden por vblank); y `10F000..`, contadores de vblank y de la
  protección. Volcados de la RAM `100000..10EFFF` de ambas versiones en los
  24 cambios de sección (`ram251/`, `ram264/`): iguales byte a byte fuera de
  esas zonas (0, 1 o 2 bytes distintos dentro).
- Resultado: 26708 actualizaciones, 0 diferencias; mismas secciones,
  progreso, puntuación y vidas en la misma actualización; 0.264 llega al
  final en el fotograma 57041 (0.251: 57062).
- Reanudar desde un estado: `MSX_TAS_STATES=<dir de estados>
  MSX_TAS_UPD_LOAD=<nombre>:<U>` (el U de su línea `SAVED`). Probado desde
  `sec05_u10130` hasta el fin de la misión 4 y desde `u21000` hasta el
  final: 0 diferencias.

## Índice (`/home/user/captures-tas/`, nada en git)

- `updates251.txt`: la grabación de 0.251 (entrada, fotograma y hashes de
  cada actualización); `ram251/`, `ram264/`: RAM en los cambios de sección.
- `sync264/`: `events.txt` (secciones y una línea `SAVED nombre update U
  frame F section S progress P score vidas` por estado), `updates.txt`,
  `states/mslugx/u<U>.sta` (400 estados, U múltiplo de 60) y
  `sec<NN>_u<U>.sta` (12 inicios de sección), `snap_u<U>.png` (foto de cada
  estado). Los estados se toman en el primer límite de fotograma tras la
  actualización U. Para cargarlos: `-state_directory
  /home/user/captures-tas/sync264/states -state <nombre>`.
- Regenerar (2,5 min): `MSX_TAS_SCRIPT=tas_updates.lua
  MSX_TAS_UPD_PLAY=/home/user/captures-tas/updates251.txt
  MSX_TAS_UPD_SAVE_EVERY=60 MSX_TAS_UPD_SNAP_EVERY=60
  MSX_TAS_UPD_SAVE_SECTIONS=1 MSX_TAS_LAST=57700 tools/run_tas_play.sh DIR`.

Secciones (exactas, de `events.txt`; fotogramas de 0.264). "Fin" es la
actualización en que el guion de la sección se cierra (MISSION COMPLETE o
paso a la sección siguiente):

| Misión | Sección | Estado inicial | Fotograma | Fin (U, fotograma) |
| --- | --- | --- | --- | --- |
| 1 | 0 | `sec00_u180` | 656 | 2179, 5097 |
| 2 | 1 | `sec01_u2534` | 5908 | 2730, 6303 |
| 2 | 2 | `sec02_u2798` | 6481 | 3876, 8794 |
| 2 | 3 | `sec03_u3932` | 8910 | 5701, 12543 |
| 3 | 4 | `sec04_u6282` | 13726 | 9513, 20751 |
| 4 | 5 | `sec05_u10130` | 22017 | 12463, 26982 |
| 5 | 6 | `sec06_u12887` | 27901 | 13770, 29921 |
| 5 | 7 | `sec07_u13810` | 30005 | 16420, 35482 |
| 5 | 8 | `sec08_u16484` | 35614 | 18101, 39492 |
| 6 | 9 | `sec09_u18679` | 40676 | 20210, 44376 |
| 6 | 10 | `sec10_u20251` | 44471 | 23520, 51043 |
| 6 | 11 | `sec11_u23570` | 51147 | 26423, 57041 |

Puntos dentro de cada misión, **vistos en las fotos** `snap_u<U>.png` (una
cada 60 actualizaciones: el punto exacto cae entre esa foto y la anterior;
no se han derivado de la RAM). El estado tiene el mismo nombre que la foto:

- Misión 1: SV-001 en `u1320..u1380`; jefe en pantalla `u1920..u2160`.
- Misión 2: interior desde `sec03`; Slugnoid en `u5400`; jefe `u5640..u5700`.
- Misión 3: tren desde `u6300`; jefe `u9120..u9480`.
- Misión 4: MISSION 4 START `u10200`; calle `u10260..u10680` (tranvía en
  `u10620`); canal `u10740..u10920`; vehículo enemigo `u10980..u11160`;
  tejados y helicópteros `u11220..u11580`; torre y flecha IN
  `u11640..u11760`; tramo aéreo `u11820..u12060`; jefe `u12120..u12420`.
- Misión 5: MISSION 5 START `u12960`; calle `u13020..u13500`; salida
  `u13560..u13740`; metro (sección 7) `u13860..u16380`, con trenes en
  `u14640`, `u15720`, `u16020` y la vida perdida a propósito en `u16140`;
  alcantarilla (sección 8) `u16500..`; tanque `u17400`; flechas IN
  `u17460..u17640`; jefe `u17760..u18060`.
- Misión 6: FINAL MISSION `u18720`; misiles `u18780..u18900`; puente
  `u19080..u19260`; ballena `u19560`; muelle `u19920..u20160`; fábrica
  (sección 10) `u20280..u21420`; pasillo `u21480..u23400`; exterior y nave
  final (sección 11) `u23700..u26400`. Después de 26423 (final y créditos)
  no hay estados.

Capturas desde el TAS (sin ninguna escritura en la memoria): `tas_updates.lua`
tiene grifos de solo lectura con los formatos de `capture_fullgame.lua`:
`MSX_TAS_CAMERA=1` (`camera.csv`, una línea por actualización de cámara) y
`MSX_TAS_CELLS=1` (`cells.txt`, cada celda que lee `0C79FE`, y `events.csv`,
cada escritura en el anillo con su PC), desde `MSX_TAS_CAPTURE_FROM=<U>`.
Primer uso: la Misión 5 (`tools/capture_m5_tas.sh`, [MISION5.md](MISION5.md)):
sus pruebas de mapa y cámara pasan también con estas capturas. Los estados de
sección se guardan cuando cambia el puntero del guion, **después** del borrado
del anillo de celdas de la carga: para tener la historia entera del anillo se
carga el estado anterior (`u12420`, `u13740`, `u16380`; entre secciones no hay
estados `u<U>`).

Segundo uso: la Misión 6 (`tools/capture_m6_tas.sh`, [MISION6.md](MISION6.md); estados anteriores a la carga
`u18060`, `u20160`, `u23460`). `tas_updates.lua` acepta además `MSX_TAS_DEBUG=1` con `MSX_TAS_DEBUG_CMDS` (órdenes
del depurador separadas por `|`, solo `printf`; `temp9` = fotograma), como `tas_play.lua`: con ellas se capturó **el
final y los créditos** dejando correr el TAS desde `u26400` hasta el fotograma 75 000 (después del último fotograma de
la película la entrada es 0): el último enemigo muere en 57035..57038, la huida (registro de etapa `017624`) hasta
60038, MISSION COMPLETE, la escena final `0570A0` desde 60920, los créditos (sección 22 del guion) de 62172 a 72300,
el logotipo de SNK en 72801 y la demostración (sección 20) desde 73677. Estados `end26480`, `end28960` y `end34170`
en `/home/user/captures-m6/states/mslugx/`; fotos y registro en `/home/user/captures-m6/ending/out/`.

Reproducir por fotograma: `tools/run_tas_play.sh /home/user/captures-tas/full`
(2 min, 4 MB; `MSX_TAS_LAST`, `MSX_TAS_SNAP_AT=4320,4360,...`).
