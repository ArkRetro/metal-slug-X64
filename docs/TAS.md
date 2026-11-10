# Fase 1: el TAS de TASVideos #6663 en MAME 0.264

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

## Resultado: no sincroniza en MAME 0.264 (registrado)

- Alineación: desfase 0 (el fotograma k del TAS se emula como el fotograma
  k + 1 de MAME). Desfases −3..+8 y desfases distintos para la BIOS y el
  juego: el juego no arranca, cae en la demo o muere antes; 0 es el mejor.
- Con desfase 0 el TAS juega como un TAS hasta el fotograma ~4316: moneda,
  Start, SOLDIER SELECT (Fio), MISSION 1 (sección 0 en 649), SV-001 y el grupo
  de helicópteros del progreso 3192 (puntuación 14500 en 4316). Desde ahí ya
  no puntúa: el SV-001 dispara en horizontal mientras los helicópteros siguen
  arriba, pierde las vidas en 7356 y 7834 y la partida acaba en 10469. En el
  TAS la misión 1 acaba antes de 5453 (primer tramo largo sin entradas,
  5453..6026, la cuenta de MISSION COMPLETE).
- Causa probable: la versión del emulador. BizHawk 2.9.1 lleva MAME de la
  rama `mamehawk0252` (`.gitmodules` de la etiqueta 2.9.1). El juego corre a
  30 Hz y su bucle principal (`003A42`) se alarga a 3 o 4 vblanks en 794 de
  los 7300 fotogramas jugados hasta 8000 (primeros en 1272, 1447, 1634..1683):
  la ralentización depende de los ciclos del 68000. Además, el bucle de espera
  (`0039B0..003A34`) suma 0x3EF a `10F00C` por vuelta y, en cada acarreo, hace
  la comprobación de la protección (`2FFFE0`, tabla `0DEDD2`) sumando a
  `10F00A` la línea de barrido `3C0006`: ese trabajo cambia el instante en que
  empieza cada bucle respecto al vblank. Un núcleo 68000 con otros tiempos
  mueve los límites de ralentización y con ellos qué fotograma del TAS lee cada
  vuelta del juego. Re-temporizar por fase (desplazar las entradas para que los
  cambios del TAS caigan en las lecturas del bucle de 0.264) empeora (fin de
  partida en 4924): no es una regla simple.
- Pendiente para confirmarlo: reproducir el TAS en MAME 0.251/0.252 y comparar
  la RAM fotograma a fotograma con 0.264 (primer fotograma divergente). El
  binario 0.251 de Debian se descargó pero ejecutarlo quedó bloqueado por los
  permisos de la sesión (código externo); se borró. Aunque sincronizara, sus
  estados no sirven en 0.264 y las capturas deben ser de 0.264 (`CLAUDE.md`).

## Consecuencia

Sin sincronía no hay estados del TAS por misión ni captura de la M3 del TAS.
Se sigue con las capturas propias (plan de `PLAN_ATAQUE.md`): la M3 se compara
con `run4` (`/home/user/captures-fullgame/run4`, MAME 0.264) y con las capturas
cortas de cada hueco (`MISSION3_DEBUG.md`).

Reproducir: `tools/run_tas_play.sh /home/user/captures-tas/full` (2 min, 4 MB;
`MSX_TAS_LAST`, `MSX_TAS_SNAP_AT=4320,4360,...` para ver el jefe).
