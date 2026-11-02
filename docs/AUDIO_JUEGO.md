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

## 8. Efectos del resto de la Misión 1 y residencia en RAM (rama `work-world`)

Informe del usuario (ROM 9 en MiSTer): solo sonaban los disparos, algunas
explosiones y los gritos. El mundo nativo ya pedía muchas más órdenes
(`sounds[]`), pero solo 12 tenían render.

**Órdenes nuevas.** Recorrido de `tools/test_gameplay.c` instrumentado: 40
palabras distintas pedidas por el mundo nativo que no tenían render. Se
descartan las `>= 0x2000` (`90A5`, `90E8`: `0120F8` las tira) y `002C`
(banco 00, música). Las 37 restantes más `10A5/1026` están en
`data/sound_map.json` (`sfx_<palabra>`, con las rutinas ROM que las piden en
el port) y se renderizan aisladas como antes (`render_sound_commands.py
--labels`). `10A5` (motor del blindado) y `1026` siguen sonando al final de la
ventana de 300 fotogramas: son bucles, no efectos, y no se empaquetan
(`pack_game_audio.py`: estado `LOOP`). Total de efectos: 49 renders, 63 s.

**Política medida.** `tools/measure_sound_pairs.py` renderiza pares (A y,
12 fotogramas después, B) y los ajusta contra los renders aislados con dos
criterios: la forma de onda (las muestras ADPCM-A son idénticas entre renders)
y la envolvente de potencia en ventanas de 10 ms (las partes FM/SSG no lo
son). `tools/derive_sound_policy.py` decide cada par (onda si explica la
mezcla, si no envolvente con margen ≥ 0,05, si no «dudoso»), une en un canal
los sonidos con algún par que no solapa y ordena los niveles (B corta a A →
nivel(B) ≥ nivel(A); B descartado → nivel(B) < nivel(A)) respetando los de la
sección 4. Medidas: cada sonido nuevo consigo mismo y contra pistola,
explosión y `1041` en ambos órdenes; los que solapan con los tres, todos
contra todos; los de cada canal conocido contra los demás niveles del canal.
Pares en `data/sound_pairs.json` (1673: 259 de la primera ronda, 630 de la
segunda y 784 de las dos del SV-001: cada sonido nuevo contra todos los
demás en ambos órdenes), resultado en `data/sound_policy.json` (solo números) y
`src/msx_sfx_policy.h` (`tools/compile_sound_policy.py`):

| Canal | Sonidos (nivel) |
| --- | --- |
| arma | `10F7` (0); `108D`, `108C`, `10F4`, `1084` (1); `10F2` (2) |
| impacto | `1027`, `10E9` (0); `10FA`, `1022` (1); `1023`, `1033`, `1021`, `1025`, `102C`, `1035`, `1064`, `1067`, `10FF` (2); `102F`, `1032`, `1034` (3) |
| voz | `103F`, `1043`, `10A3`, `10A4`, `10C6`, `116E`, `1092`, `108F`, `1090`, `1091` (0); `103E`, `1040..1042`, `1048`, `1073`, `108E` (1); `104A`, `104F`, `105D`, `1072`, `1074`, `1075`, `1142` (2) |
| `1056` | `1056` (0); `1124`, `112D`, `112E`, `113A`, `1158` (1) |
| `10E8` | `10EA` (0); `10E8` (1) |
| `109B`, `10A7`, `1121` | uno cada uno |

263 pares quedan dudosos (sobre todo los sonidos FM/SSG largos); 38
relaciones medidas no caben en un orden total de niveles (ciclos entre
sonidos nuevos del canal de impacto y de `1056`; `derive_sound_policy.py`
las lista en `contradictions`): el nivel de un sonido nuevo nunca sale del
intervalo que dan sus pares con los sonidos ya medidos. Es una aproximación
documentada de la regla del controlador, no su código (no se ha leído el
Z80).

**Sonidos del SV-001** (rama `work-sv001`): `1084` (obús), `10F2`
(estallido del obús), `1092` (reposo), `10E9` (salida A+B), `1034`
(voladura), `108F` (agachado), `1090` (salto) y `1091` (aterrizaje) en
`data/sound_map.json`, renderizados aislados (`render_sound_commands.py
--labels`, 0,49–2,74 s, todos `OK`) y medidos contra los sonidos
anteriores (por lotes de 60 pares: un lote grande llenó el disco y dejó
`render.jsonl` cortado). Un par uniría dos canales medidos
(`10F2 > 10A4`: la onda dice «descarte», la envolvente «solapa»):
`derive_sound_policy.py` ya no une grupos con un par así y lo lista en
`bridges`. Política: 45 efectos con canal, 471 pares dudosos.

**Canales de mezcla.** Los efectos son mono: cada canal del controlador usa un
canal mono del mezclador desde el 6 (los 32 del mezclador; antes un par
estéreo por voz con 5 voces de efectos).

**Residencia en RAM (Expansion Pak).** `src/msx_ramfs.c` monta `ram:/`: al
arrancar (`game_audio_init`, antes de dimensionar la caché de escena) cada
WAV64 de efecto se lee entero de la ROM una sola vez y el decodificador VADPCM
lo lee desde RAM en cada reproducción: ninguna reproducción toca el bus del
cartucho ni abre ficheros. La música sigue en streaming desde la ROM.

| Medida (ares, `MSX AUDIO sfx resident`) | Valor |
| --- | --- |
| Efectos residentes | 851 270 B (49 WAV64 VADPCM 24 kHz mono) |
| Montón total / usado antes → después de cargar | 6 678 976 B / 770 936 → 1 633 728 B |
| Caché de escena (`scene_assets_init`, reserva `SCENE_HEAP_RESERVE` 1,5 MiB) | 3584 KB (224 *chunks*; antes 4096 KB, 256) |
| Montón libre tras la caché / en juego | 1307 KB / 948 KB |

`scene_assets_all_resident()` **no puede ser cierto** con este paquete: 703
*chunks* de 16 KiB son 11,5 MiB decodificados, más que toda la RDRAM (ya en
`main`: 665 *chunks*, caché 4256 KB, falso). La caché de 224 *chunks* sigue
por encima del peor conjunto de trabajo (30, `check_scene_budget.py`).

## 9. Fin de misión: «MISSION COMPLETE» (informe de MiSTer, ROM 13)

No sonaba la voz de fin de misión. El actor de misión `101364`, en `0188B8`
(la actualización en que `0188B0` ve el `+60` del recuento de prisioneros),
llama a `15422` (solo borra `10C2BD`; `15418` lo pone a FF: no es música)
y luego a `0120F8` con `0x0020` y `0x1137`, antes de crear las letras
`06090A`. El port no pedía ninguna.

- `data/sound_map.json`: `music_mission_complete` (`0020`, banco 00: orden
  de música, sustituye a la de la misión en el controlador) y `sfx_1137`
  (la voz). Render aislado (`render_sound_commands.py --rom-dir ROMS
  --labels music_mission_complete sfx_1137`): `0020` 6,77 s estéreo,
  latencia 355 ms, termina dentro de la ventana de 900 fotogramas (no es un
  bucle); `1137` 2,05 s mono, `OK`.
- `pack_game_audio.py`: una música que termina en su ventana (`OK`) se
  empaqueta entera, estéreo y sin bucle (`one_shot`); un render parcial
  (`--labels`) conserva en `manifest.json` los paquetes de los demás
  sonidos. `music_mission_complete.wav64` 183 160 B, `sfx_1137.wav64`
  27 872 B (residente en RAM como los demás efectos).
- Canal de `1137`, medido (`measure_sound_pairs.py --pairs`, 7 pares en
  `data/sound_pairs.json`): consigo misma *corta* (onda y envolvente,
  margen 0,31); con pistola `10F7`, explosión `1027` y la voz rebelde `1041`,
  en ambos órdenes, *solapa* (onda). No comparte canal con nada medido:
  `derive_sound_policy.py` le da un canal propio `ch_1137` (nivel 0; una
  segunda petición la reinicia). No va al canal VOZ de los rebeldes porque
  `1041` y `1137` suenan juntos en el original.
- `gameplay.c` (`mission_end_step`, paso `0188B8`): pide `0x0020` y
  `0x1137` en ese orden, en la lista de sonidos del mundo, en la misma
  actualización que crea las letras. `game_audio.c`: `0x0020` para la
  música de la misión y toca el *jingle* una vez en la voz de la intro
  (libre en juego; `game_audio_sync` la para al reiniciar); `0x1137` pasa
  por la política como cualquier efecto.
- Comprobación (`tools/test_gameplay.c`, `release_run`): cada orden se pide
  una sola vez, `0020` justo antes de `1137`, en la actualización 30 +
  recuento (la siguiente a la última del recuento; semilla 1: actualización
  259, recuento 229), con las letras ya creadas (`+7C` = 0x9F tras su primer
  `subq`).

## 10. Música al empezar la Misión 2 (informe del usuario, ROM 20)

En hardware real la Misión 2 empezaba sin música tras jugar la Misión 1.
`game_audio_stage_music` abría la `0022` pero solo la tocaba si la música
anterior sonaba o si el juego no estaba en fin de misión: el *jingle* `0020`
la había parado y el fin de misión seguía marcado. En el original `018434`
pide la palabra y el controlador la toca; además `1805C` (primer registro de
cada misión) manda antes el reinicio `03` y `01829E` pone `+80` = FFFF.
Ahora: `game_audio_mission_start` (el `03`: efectos, voz del *jingle* y
música parados) y la música pedida en la carga suena siempre. En ares con el
cartucho de transición (`tools/test_m1m2_ares.py`): `MSX AUDIO stage music
0022` seguido de `MSX AUDIO music start` (antes del arreglo: sin `music
start`). Ver MISION2.md, «Transición Misión 1 → Misión 2».

## 11. El rayo `10F8` (sonido sostenido, rama `work-laser`)

`01E19E` pide `10F8` al crear el rayo de las armas `06/0B` (si el otro
jugador no lo tiene sonando) y `01E4D4` lo para con `012192` al acabar. El
render aislado (`render_sound_commands.py --labels sfx_10f8`) sigue sonando
al final de su ventana (`ACTIVE_AT_WINDOW_END`, 5,015 s) y no tiene un
periodo exacto medible (ruido modulado): no es un bucle de muestra. Se
empaqueta la ventana entera (`data/sound_map.json` `"stop": "012192"`,
`pack_game_audio.py`), a 32 kHz porque el 30 % de su energía está por encima
de 11 kHz, y `game_audio.c` atiende ahora la petición de parada de los
actores (`palabra | 0x8000`): el canal que suena esa palabra se para. Es
el único efecto no residente: se reproduce desde `rom:/` como la música
(88 KB que el presupuesto de la caché de escena, 208 ranuras, no tiene). Un
rayo que dure más de 5 s queda en silencio antes que en la recreativa
(desviación declarada). Pares medidos (`measure_sound_pairs.py --labels
sfx_10f8` y, en modo explícito, contra `1056`, `1069`, `109B`, `10A7`,
`10E8`, `1121`, `1137`): sin corte ni caída claros: canal propio
`ch_10f8`.

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

- Motor del blindado (`10A5`) y `1026` (bucles, sin empaquetar); sonidos
  del jugador fuera de la pistola que el port no pide (granadas, muerte de
  Marco, `1123`…), voces de selección, música de jefe/continuación
  (`002C`/`002D`) y del resto de misiones.
- En una de dos ejecuciones del recorrido en ares el mundo paró una rutina
  (`MSX world unsupported routine=afff0`): cobertura de la IA rebelde
  dependiente del ritmo de entradas, sin relación con el audio (0 underruns
  también en esa ejecución).
- Prioridades entre pares no medidos son una extensión de los niveles medidos.
- Validación acústica en hardware/MiSTer.

## Bucle de la música en la libdragon fijada

El lector VADPCM de la libdragon fijada solo da la vuelta a ficheros completos
(`wav64_vadpcm.c` exige `loop_len == len` al llegar al final y, si no, aborta).
El recorrido de la misión completa llega al final del fichero de música, cuyo
bucle medido es parcial: `msx_audio_play` repite ahora el fichero entero
cuando es VADPCM con bucle parcial (el punto de bucle medido no se respeta
hasta que el lector lo admita). Ver [MISION1_COMPLETA.md](MISION1_COMPLETA.md).

## Corrección: efectos residentes distorsionados (ROM 10)

En MiSTer los efectos sonaban completamente distorsionados durante el juego.
Grabando la salida de ares (ALSA a archivo, 48 kHz) y midiendo saltos de
muestra > 8000: ROM 9 0,03 %, rama de armas 0,03 %, rama del mundo 9,7 %,
ROM 10 11,9 % con 2 % de muestras recortadas. Leer los mismos WAV64 desde
`rom:/` en vez de `ram:/` daba 0,03 %: la causa era `msx_ramfs`. Su `read`
copiaba con la CPU y el dato quedaba en la caché de datos; el decodificador
VADPCM del RSP lo lee por DMA de la RDRAM y decodificaba memoria antigua.
`ram_read` hace ahora `data_cache_hit_writeback` del búfer copiado (como si
fuera una lectura PI). Con el arreglo: 0,04 %, sin recorte.

La entrada en paracaídas (`entry.wav64`, VADPCM) también medía 2–3,5 % en su
ventana frente a 0,33 % de la fuente; sin comprimir (como el título) mide
0,01–0,3 %. `pack_cinematic.py` usa ahora `--wav-compress 0` (≈800 KB).
