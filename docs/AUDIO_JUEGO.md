# Audio de juego de la Misión 1: efectos originales y música en bucle

Continúa [INTRO_AUDIO.md](INTRO_AUDIO.md) (backend WAV64/RSP de
`src/msx_audio.c`) y [SOLDIER_WORLD.md](SOLDIER_WORLD.md) (peticiones de
sonido del mundo nativo). El juego reproduce ahora los efectos originales en
los eventos de juego y la música de la Misión 1 en bucle. No se emula el Z80
ni el YM2610: cada orden de sonido original se graba aislada en MAME desde la
ROM del usuario y se reproduce como WAV64. El audio derivado de la ROM queda
en `filesystem/` (ignorado por Git); en Git solo hay herramientas, números y
código.

## 1. Cómo pide sonidos el original

El 68000 llama a `0120F8` con una palabra de petición en `D0` (p. ej.
`0x10F7`). Si la palabra es `>= 0x2000` o su byte bajo `< 0x20`, se descarta
(así el impacto del objeto lanzado `03BE6C` pide `0x0000` y no suena). Si no,
`01209C` encola en el anillo `0x10BF68` el byte alto (selector de banco del
driver, `< 0x20`, solo si cambia respecto al último, `0120A4`) y el byte
bajo. `01221A` envía un byte por fotograma al latch `0x320000`. Órdenes
sueltas `< 0x20`: `03` reinicia el driver (silencio inmediato), `06`, `0C`.

## 2. Captura de órdenes en partidas reales

`tools/mame/capture_gameplay_audio.lua` (MAME 0.264, BIOS euro, solo
entradas salvo el plan `vehicle`/`survive`, que reutiliza las escrituras de
supervivencia de `tools/capture_vehicle.lua`) registra con *watchpoints* del
depurador cada byte encolado (`REQ`, con `D0` y la dirección de retorno leída
de la pila) y cada escritura al latch (`LATCH`), más `-wavwrite` a 48 kHz.
Los *write taps* de Lua 0.264 no sirven aquí: en 0.264 el PC del 68000 es
`rPC` y el *callback* fallaba en silencio. El contador de fotogramas usa
`screen:frame_number()` porque los *callbacks* pueden saltarse fotogramas con
`-nothrottle` en una máquina cargada.

Capturas usadas (`/home/user/captures-audio`): `fight1` (9600 fotogramas,
avanzar disparando, granadas, saltos, pausas), `knife1` (quedarse quieto ante
el primer rebelde), `survive1` (12000 fotogramas), `vehicle1` (encuentro del
blindado hasta su destrucción). `tools/extract_sound_map.py` resume palabras,
llamadores y fotogramas.

| Evento del port | Palabra | Llamador observado | Duración render |
| --- | --- | --- | --- |
| Disparo de pistola del jugador | `10F7` | `01B78C` (601+203 veces) | 0,489 s |
| Explosión de objeto lanzado al tocar suelo / granada | `1027` | `03BE68`, `01C15C` | 0,639 s |
| Cuchillo rebelde alcanza al jugador | `10FA` | `0AEF2E` (1 vez, `knife1`) | 0,381 s |
| Programa de animación rebelde, orden `0x30` | `103E` | `004498` | 0,668 s |
| ídem (no observado en captura) | `103F` | — | 0,327 s |
| ídem (no observado en captura) | `1040` | — | 1,160 s |
| ídem | `1041` | `004498` | 0,560 s |
| ídem | `1042` | `004498` | 1,111 s |
| ídem | `1043` | `004498` | 0,375 s |
| Bala contra el blindado (vida −10) | `108D` | `01BB34` | 0,296 s |
| Blindado bajo la mitad de vida / destruido | `1023` | `029AEE` | 1,178 s |
| Estallido del blindado, 12–13 fotogramas tras vida 0 | `1033` | `029B1E` | 2,981 s |
| Música de la Misión 1 | `0021` (tras `03`,`06`) | `018434` | 87,26 s (bucle) |

`103F` y `1040` están en los programas de animación originales convertidos
(`data/soldier_programs.json`) pero no aparecieron en las capturas: su audio
es el render aislado de la orden; no hay contraste con una partida.

## 3. Render aislado de cada orden

`tools/render_sound_commands.py` lanza `tools/mame/render_sound_commands.lua`:
llega a la Misión 1 con las mismas entradas, suspende el 68000 con el
depurador (`suspend 0`; el script patea el *watchdog* `0x300001`, si no la
máquina se reinicia) y escribe en el latch `03`, `06`, 120 fotogramas de
silencio (comprobado) y la secuencia de la orden, un byte por fotograma. El
recorte va del primer al último sample por encima de 48 (latencia orden →
sonido 9–16 ms). Todos los efectos salen mono (L = R) y con ≤ 3,8 % de
energía por encima de 8 kHz (ADPCM-A a 18,5 kHz).

`tools/verify_sound_render.py` correlaciona cada render con el PCM de las
partidas en cada escritura real de esa orden (−20..+80 ms). Mediana de la
correlación normalizada (con la música y otros efectos mezclados):

| Render | Apariciones | Mediana | Máx. |
| --- | --- | --- | --- |
| Línea base (posiciones aleatorias) | 600 | 0,052 | 0,603 |
| `pistol` | 201 | 0,266 | 0,608 |
| `explosion` | 30 | 0,115 | 0,822 |
| `knife_hit` | 1 | 0,250 | 0,250 |
| `rebel_103e` / `1041` / `1042` | 6 / 13 / 9 | 0,516 / 0,617 / 0,492 | 0,764 |
| `rebel_1043` | 1 | 0,117 | 0,117 |
| `vehicle_hit` / `smoke` / `burst` | 12 / 2 / 1 | 0,343 / 0,370 / 0,522 | 0,522 |

`0x10A5` del blindado no es un efecto puntual: sola suena continua (motor) y
`0C`+`10A5`, lo que se envía al destruirlo, la detiene (render silencioso).
Como el blindado nativo no tiene evento de aparición y no se midió su bucle,
**no se incluye**. `06` y `0C` no paran la música.

## 4. Política de canales medida

Pares de órdenes separadas 0,2 s (12 fotogramas, el ritmo de disparo),
ajustados contra los renders aislados: *solapan* (mezcla = A + B), *corta*
(A termina al empezar B) o *descarta* (B no suena). Resultado:

- Canal ARMA: `10F7` se reinicia a sí mismo; `108D` corta a `10F7`; `10F7`
  se descarta mientras suena `108D`.
- Canal IMPACTO: `1027` < `10FA` < `1023` ≤ `1033` (cada uno corta a los
  menores y los menores se descartan; misma orden = reinicio).
- Canal VOZ: `103E`, `1040`, `1041`, `1042` se cortan entre sí; `103F` y
  `1043` se descartan bajo `1041`.
- Entre canales distintos siempre solapan (pistola + explosión + grito, cuchillo + pistola).

`src/msx_sfx.c` aplica exactamente esa regla (prioridad ≥ reemplaza, menor se
descarta) con tres voces de efectos de `msx_audio` (`EFFECT_FIRST..+2`); los
pares no medidos siguen los mismos niveles. Cada sonido suena solo en su
canal, así el estado VADPCM de un WAV64 nunca se comparte entre voces.

## 5. Bucle de la música

El render de 405 s de `0021` no se repite muestra a muestra (fase FM y
*jitter* del temporizador del driver: correlación por muestra ≈0,4), así que
el bucle se mide con autosemejanza de espectrograma logarítmico (64 bandas,
10 ms): los mejores desfases son 78,8 / 157,7 / 236,5 / 315,3 s, múltiplos de
un periodo de **78,79 s**; la parte repetida empieza en **5,97 s** desde el
inicio del sonido (antes la semejanza es 0,2–0,5; después 0,7–0,8; mediana
0,979 en el tramo medido). El empalme se coloca en el primer punto cuya
correlación de onda con el mismo punto un periodo después llega a 0,9
(0,930, a 8,47 s). El WAV64 contiene [0, 87,26 s) y vuelve a 8,47 s
(`--wav-loop-offset`, múltiplo de 16 para VADPCM). No se ha visto el código
del secuenciador: el punto es medido, no leído del driver.

## 6. Integración

- `src/neo_world.c` añade una lista `sounds[]` con las palabras originales
  (orden `0x30`, `0x10FA`, `0x1027` del objeto lanzado); el consumidor la
  vacía. `src/gameplay.c` solo cuenta `shots`.
- `src/game_audio.c` (una llamada por fotograma desde `actor_draw`): disparos
  → `10F7`; lista del mundo; blindado: vida bajada por bala → `108D`, cruza la
  mitad o llega a 0 → `1023`, fotograma 1 de destrucción → `1033`; fin de
  región → para la música (el arcade no tiene ese fin; entre escenas el driver
  se reinicia con `03`, que la silencia). `R` no reinicia la música si sigue
  sonando.
- `main.c` y `mission_main.c` inicializan el audio y atienden el AI cada
  fotograma. Sin los archivos el juego funciona en silencio y lo registra.
- `msx_audio_play(loop=true)` conserva ahora el punto de bucle del WAV64.
- En `mission1-internal` la música empieza desde el principio al aterrizar
  (el VADPCM solo admite saltar a 0 o al bucle, así que no se continúa la
  posición del audio de la cinemática de entrada).

## 7. Presupuesto

| Concepto | Valor |
| --- | --- |
| ROM añadida (13 WAV64) | 2 496 346 B: música 2 356 300 B, efectos 140 046 B |
| Música | VADPCM estéreo 24 kHz: ≈27 KB/s, **1,62 MB por minuto** |
| Efectos | VADPCM mono 24 kHz: ≈13,6 KB/s, 10,9 s en total |
| ROM `mslugx-explorer.z64` | 3 768 320 B (antes 1,2 MB sin audio) |
| RDRAM | 5 canales del mezclador en uso (música estéreo 2 + 3 efectos mono), límite 8 KiB de muestras cada uno: ≤40 KiB; 2 buffers AI de ≈5 KiB (ya existentes); 12 `wav64_t` abiertos (sin precarga de PCM) |
| CPU (mezcla RSP síncrona + `game_audio_update`) | media 0,84–0,90 ms/fotograma, máx. 3,31 ms (primer intervalo) |
| Peor fotograma, ares | `cpu` máx. 8,05 ms; `cpu` máx. + `audio` máx. = 10,46 ms < 12 ms |
| Subdesbordamientos AI | 0 en todo el recorrido (631 buffers) |

Opus (`--wav-compress 3`) dejaría la misma música en 1,31 MB por 159 s
(≈0,5 MB/min), pero en este commit de libdragon no admite bucles con offset
ni *seek* y descodifica en CPU; no se usa.

## Regenerar

```sh
python3 tools/mame/run_gameplay_audio.py --rom-dir ROMS --output CAP/fight1 --end-frame 9600
python3 tools/mame/run_gameplay_audio.py --rom-dir ROMS --output CAP/knife1 --end-frame 6000 --plan knife
python3 tools/mame/run_gameplay_audio.py --rom-dir ROMS --output CAP/vehicle1 --end-frame 4800 --plan vehicle
python3 tools/extract_sound_map.py fight1=CAP/fight1 knife1=CAP/knife1 vehicle1=CAP/vehicle1 --output CAP/requests.json
python3 tools/render_sound_commands.py --rom-dir ROMS --work CAP/render   # -> filesystem/audio-src
python3 tools/verify_sound_render.py CAP/fight1 CAP/knife1 CAP/vehicle1
N64_INST=... python3 tools/pack_game_audio.py                             # -> filesystem/audio/*.wav64
make -j4 && make check-core && make check
python3 tools/test_game_audio_ares.py mslugx-explorer.z64 --output RESULT
```

Los pares de la sección 4 se reproducen con `MSX_RENDER_PLAN`, p. ej.
`pp=10,F7,+11,F7:120` (`+N` espera N fotogramas más).

## Pendiente

- Motor del blindado (`10A5`) y su parada; sonidos del jugador fuera de la
  pistola (granadas, muerte de Marco, `1121`/`1123`/`113A`…), voces de
  selección, música de jefe/continuación (`002D`) y del resto de misiones.
- Orden `104A` (rutina rebelde `0B0466`) no la emite el port: rutina sin traducir.
- En una de dos ejecuciones del recorrido en ares el mundo paró una rutina
  (`MSX world unsupported routine=afff0`): cobertura de la IA rebelde
  dependiente del ritmo de entradas, sin relación con el audio (0 underruns
  también en esa ejecución).
- Prioridades entre pares no medidos son una extensión de los niveles medidos.
- Validación acústica en hardware/MiSTer.
