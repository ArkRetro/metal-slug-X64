# Rendimiento de CPU por fotograma (misión 1, soldados nativos)

Medido en ares 146 (Vulkan/paraLLEl-RDP, Expansion Pak) con
`tools/test_mission_native_ares.py` sobre `mslugx-explorer.z64`. La
temporización de ares es aproximada, pero es la referencia del repositorio.

## Qué medía `CPU=` de la línea `MSX RDP`

`scene_render_end` imprime cada 300 fotogramas el tiempo transcurrido desde
`scene_render_begin` hasta `scene_render_end` **de ese único fotograma**: es
la construcción de comandos rdpq en CPU (más cualquier espera de
`scene_service`/DMA o de rspq si el búfer se llena). No incluye la simulación
(`msx_game_update`), que corre antes, ni la espera de `display_get`. Además es
una muestra, no un máximo.

## Instrumentación: línea `MSX PERF`

`src/msx_perf.c` reparte cada fotograma en fases consecutivas con
`TICKS_READ()` (`msx_perf_mark`) y acumula media y máximo de 300 fotogramas;
una sola línea `debugf` cada 300 fotogramas, sin registro por fotograma.
Se elimina por completo compilando con `MSX_EXTRA_CFLAGS=-DMSX_PROFILE=0`.

| Campo | Contenido |
| --- | --- |
| `steps=N/M multi=K` | pasos de simulación en la ventana / máximo por fotograma / fotogramas con más de un paso (recuperación de `scene_clock`) |
| `cpu` | todo `actor_draw`: simulación + preparación + construcción del render |
| `build` | `begin`+`bgreq`+`bg`+`mesh`+`actors`+`hud`+`end` (lo que medía `CPU=`) |
| `sim` | todos los `msx_game_update` del fotograma (mundo, soldados, jugador) |
| `prep` | registro de eventos, animación de Marco, peticiones de prefetch, marco 2.5D |
| `begin` | `scene_render_begin`: servicio de assets, attach/clear, modo |
| `bgreq` | prefetch de celdas del mapa + `scene_service` |
| `bg` | fondo y celdas del mapa (blits) |
| `mesh` | sombras de contacto (triángulos) |
| `actors` | vehículo, dibujos nativos del mundo, Marco, proyectiles |
| `hud` | cajas y texto |
| `end` | `scene_render_end` (estadísticas y `rdpq_detach_cb`) |
| `wait` | fuera de `actor_draw`: espera de `display_get`, audio, mando |
| `asset` | (solapado) descodificación+CRC de bloques y esperas de `require` |
| `step` | (solapado) el paso de simulación más lento |

Todos los valores en µs, `media/máximo`. El primer fotograma incluye el
calentamiento de la caché (`asset` ≈ 1,56 s): es arranque, no juego.

## Diagnóstico

Línea base (`e77dd6c` + instrumentación), recorrido completo:

    steps=593/2 multi=293 cpu=24090/26882 build=23377/25715 sim=546/1284
    bgreq=3190/3282 bg=19276/20932 mesh=388/768 actors=407/835 wait=9322/11384

- El fondo dominaba: el prefetch recorría las 2770 celdas y el dibujo
  recorría el array completo **una vez por capa** (5×2770 iteraciones), con
  ~55 KB de celdas que no caben en los 8 KB de caché D: ~16.600 visitas y
  fallos de caché por fotograma para ~450 celdas visibles.
- Con >16,7 ms el juego presentaba a 30 Hz: 293 de 300 fotogramas ejecutaban
  2 pasos de simulación. No había espiral (la simulación cuesta ≤1,3 ms por
  fotograma, ≈0,5–1 ms por paso), pero se perdía la mitad de los fotogramas.
- La simulación (mundo, soldados, colisiones) es pequeña: media 0,2–0,4 ms
  por paso, máximo ≈1,1 ms. No justifica tocar `neo_kernel`/`neo_collision`.
- Cada blit volvía a enviar `rdpq_mode_tlut` (y cada sprite
  `rdpq_mode_filter`): el RSP recalculaba el modo y emitía
  `SET_OTHER_MODES`/`SET_COMBINE` idénticos con su `SYNC_PIPE`.
- Con el Expansion Pak todo el pack (100 bloques entonces) quedaba residente
  tras el arranque; aun así el prefetch hacía ~900 peticiones nulas por
  fotograma. (Histórico: el pack actual, 665 bloques, ya no cabe; ver
  «Expansion Pak: presupuesto de RAM medido» al final.)

Perfil fino de `blit_scaled` (experimento no integrado, 500 blits): modo
TLUT+paleta ≈1,5 ms, subida de textura (~50 por fotograma) ≈1,6 ms,
rectángulo ≈1,5 ms.

## Cambios

1. **Recorte por cámara con *runs* ordenados** (`src/scene_cull.c`). En el
   arranque, las celdas de cada capa (orden del pack) se dividen en tramos con
   `x` no decreciente (103 tramos). Toda prueba de recorte del renderer es
   `v = x*escala + desplazamiento` contra límites fijos, monótona en `x`
   (escala > 0, redondeo IEEE monótono): dentro de un tramo las celdas
   visibles forman un rango contiguo que se busca por bisección **con la
   misma expresión float**. Recorrer los tramos en orden visita exactamente
   las mismas celdas en el mismo orden. El pack es de capas consecutivas
   (comprobado al iniciar; si no, el modo clásico vuelve al recorrido
   completo). `tools/test_scene_cull.c` (en `make check`) compara contra los
   recorridos originales en las cámaras −64..1984 de ambos modos y en 300
   conjuntos aleatorios.
2. **Modo TLUT/filtro solo cuando cambia.** El renderer recuerda el valor que
   fijó; se invalida al empezar el fotograma, tras texto y en
   `scene_render_forget_texture` (vehículo, niebla). Las cajas y la malla
   usan `push/pop`, que restauran el modo. Los draws, subidas y TLUT son los
   mismos; solo desaparecen comandos de modo redundantes.
3. **Prefetch sin peticiones nulas.** `request()` es un máximo idempotente y
   nada se sirve dentro del bucle: se omite repetir el bloque anterior, y si
   `scene_assets_all_resident()` (todos los bloques descodificados) se omite el
   recorrido: ninguna petición tendría efecto y sin peticiones no hay
   desalojos. Con 4 MiB el prefetch sigue igual.

## Resultados (recorrido completo; por columna, el peor valor entre todas las ventanas de 300 fotogramas)

| Métrica (µs) | Antes media | Antes máx. | Después media | Después máx. |
| --- | ---: | ---: | ---: | ---: |
| `cpu` (total `actor_draw`) | 24.090 | 26.882 | 7.337 | 8.743 |
| `build` (lo que medía `CPU=`) | 23.377 | 25.715 | 6.818 | 7.718 |
| `sim` por fotograma | 588 | 1.284 | 417 | 1.067 |
| `step` (un paso) | 478 | 1.039 | 400 | 1.049 |
| `bgreq` | 3.190 | 3.282 | 31 | 34 |
| `bg` | 19.276 | 20.932 | 6.098 | 6.615 |
| `mesh` | 388 | 768 | 314 | 480 |
| `actors` | 407 | 835 | 297 | 488 |
| pasos por fotograma | 2 (293/300) | 2 | 1 (multi=0) | 1 |

Muestras `MSX RDP` (`CPU=`, compilación sin perfil): antes 19.021–19.845 µs,
después 6.420–7.495 µs (perfil activo) y máx. 7.302 µs con `MSX_PROFILE=0`.
El juego pasa de 30 a 60 fotogramas presentados por segundo; queda ≈48 % de
margen sobre 16,7 ms en el peor fotograma, así que la recuperación de
`scene_clock` (hasta 8 pasos) no puede entrar en espiral: un fotograma con dos
pasos añade ≈1 ms.

`draws`/`tex`/`TLUT` siguen en el mismo rango (≤516 / 39–50 / 12–24) porque no
cambia la lista de dibujos. Equivalencia de píxeles: un arnés temporal sobre
`tools/test_scene_release.c` dibujó 642 fotogramas (cámaras 0..1920 cada 6 px,
ambos modos, fondo + 12 sprites nativos con espejo/flash + caja + texto +
Marco) en la ROM base y en la nueva: los 642 CRC32 del framebuffer coinciden.

Nota de medida: en la base, añadir solo la instrumentación subió `CPU=` de
≈19,5 a ≈24 ms. Con el bucle de 16.600 iteraciones el coste dependía mucho de
la disposición del código en la caché I (16 KB, mapeo directo); tras el
recorte la diferencia con y sin perfil es <0,3 ms.

## Lo que queda

- `bg` ≈6 ms para ~450 blits (≈13 µs por blit en ares): subida de página
  con `rdpq_tex_upload_sub` (~50 por fotograma, ≈30 µs cada una) y
  `rdpq_texture_rectangle_scaled` (dos divisiones enteras por rectángulo).
  Se podría usar el rectángulo sin escala para las capas a escala 1, pero
  cambiaría la forma de calcular los comandos; no hace falta con el margen
  actual.
- Con 4 MiB el prefetch del fondo vuelve a ejecutarse (≈1,5 ms medido con
  el recorte de tramos).

## Cuelgue del RSP en hardware: `rspq_highpri_sync` desde el mezclador

### Síntoma

Foto de un N64 real (núcleo MiSTer) durante la misión 1 de la ROM de
prueba 6, con el mismo cuelgue visto de forma intermitente en ares ("RSP
stall vía `rspq_highpri_sync` desde `mixer_poll` en el jefe"):

    RSP CRASH | rsp_queue | rspq_highpri_sync   Crash symptom: wait loop timed out (200 ms)
    PC:0B8 | STATUS:3403 [halt broke sig3 sig5 sig6] | DP_STATUS: 00 [ready]
    t0=00800000 t1=100 s4=E8 sp=5E4 ra=103C
    RSPQ: Normal DRAM 021dc5c8; Highpri DRAM 021dd168; Current = highpri + GP=0; Current Overlay: rsp_mixer (1)

Lectura con `include/rspq_constants.h` y `include/rsp_queue.inc` del commit
fijado (`e356bf3`):

- `sig3` = `SIG_HIGHPRI_RUNNING` encendido, `sig4` = `SIG_HIGHPRI_REQUESTED`
  apagado, `sig7` = `SIG_MORE` apagado; `sig5`/`sig6` son los `BUFDONE` de
  las dos colas (normales en reposo). `halt`+`broke`: el RSP ejecutó el
  `break` de `RSPQCmd_WaitNewInput`, es decir, leyó un comando 0 (cola
  vacía) y no había `SIG_MORE`.
- Los registros coinciden con el ucode del pin: `ra=103C` es `RSPQ_Loop`
  (IMEM 0x3C), `s4=E8` es `RSPQ_DMEM_BUFFER`, `t0=00800000` es
  `SP_WSTATUS_CLEAR_SIG_MORE` (cargado en el *delay slot* antes del `break`).
- "Current = Highpri + GP=0": `RSPQ_RDRAM_PTR` es exactamente el puntero
  guardado en el *slot* highpri (`RSPQ_POINTER_STACK[HIGHPRI_CALL_SLOT]`),
  con índice 0 en el búfer DMEM. El RSP acaba de **entrar** en modo highpri
  por `rspq_switch_highpri` (cargó el slot y saltó a él) y se ha encontrado
  la cola vacía: el slot apunta justo detrás del último epílogo
  (`SWAP_BUFFERS`) que el CPU escribió en `rspq_highpri_end`.
- El RDP está libre (`DP_STATUS ready`) y la cola lowpri está sana: no es un
  desbordamiento del búfer de comandos RDP ni una espera al RDP.

El CPU gira en `rspq_highpri_sync` (llamada desde `mixer_exec`,
`src/audio/mixer.c:696`, dentro de `mixer_poll` ← `msx_audio_mix` ←
`msx_audio_service`, bucle principal) esperando a que `RUNNING` y
`REQUESTED` se apaguen; `RUNNING` ya no se apagará nunca.

### Mecanismo (fallo de libdragon `e356bf3`, corregido aguas arriba en `preview`)

`rspq_highpri_begin` del pin hacía, en este orden:

1. cambiar de contexto y, si el epílogo anterior sigue en el búfer,
   sobrescribirlo con `JUMP cur` (para encadenar secuencias highpri);
2. **encolar** `WRITE_STATUS(CLEAR_REQUESTED | SET_RUNNING)` en `cur`;
3. **escribir** `SP_STATUS = SET_SIG_HIGHPRI_REQUESTED`;
4. `rspq_flush_internal`.

Si el RSP ya está en modo highpri cuando llega una nueva secuencia, el
`JUMP cur` del paso 1 lo lleva al `WRITE_STATUS` del paso 2 en cuanto lo
alcanza. Si ejecuta ese comando **antes** de que el CPU complete el paso 3
(basta una interrupción VI/AI/SP/temporizador entre los pasos 2 y 3, decenas
de µs frente a los ~20 ns de la ventana), el `CLEAR_REQUESTED` no apaga nada
y el `SET_REQUESTED` posterior queda **colgado** mientras el RSP está en
highpri (el bucle principal del RSP solo cambia de cola si `REQUESTED` está
encendido **y** `RUNNING` apagado, así que no lo consume). Al ejecutar el
epílogo de esa secuencia (`SWAP_BUFFERS` apaga `RUNNING`), el RSP ve
`REQUESTED && !RUNNING`, vuelve a entrar en highpri en el slot guardado
(justo detrás del epílogo, cola vacía), lee 0, hace `break` con `RUNNING`
encendido. Es el estado de la foto, bit a bit.

Por qué el RSP está en highpri cuando el CPU llama a `begin`: todo el audio
del juego es VADPCM (`audioconv64 --wav-compress 1`,
`tools/pack_game_audio.py`). En el pin, `wav64_vadpcm.c` descodifica cada
canal en el RSP con su propio `rspq_highpri_begin`/`end` **sin**
`rspq_highpri_sync`, y `mixer_exec` llama después a su `begin` para la
mezcla mientras el RSP aún descodifica el último canal. Solo el `begin` de la
mezcla, el último antes del `sync`, puede dejar el cuelgue (un `REQUESTED`
colgado por un `begin` de VADPCM lo repara el `begin` siguiente al encolar
otro `WRITE_STATUS` donde el RSP está parado). Con ~60 mezclas por segundo y
una ventana de ~10 instrucciones, el cuelgue aparece tras minutos de juego
con sonido: intermitente, y más frecuente donde hay más canales activos
(jefe), como se observó.

Descartado tras revisar `src/rspq/rspq.c` y `include/rsp_queue.inc`:

- La patada `rspq_flush` del vigilante de `msx_display_get`
  (`src/scene_renderer.c`): solo escribe `SET_SIG_MORE | CLEAR_HALT |
  CLEAR_BROKE`, que el protocolo del RSP tolera en cualquier momento
  (`WaitNewInput` vuelve a leer el búfer); no toca `REQUESTED`/`RUNNING` ni
  los punteros. El síntoma que tapaba (`display_get` sin búfer libre en ares
  largos, RSP parado con comandos pendientes) es el *wake-up* de
  `rspq_flush_internal` que el JIT de ares se pierde, con solución aguas
  arriba (`cd5b8f567`). Se mantiene el vigilante; su comentario lo explica.
- Reentrada: `mixer_poll` solo se llama desde `msx_audio_service` en los
  bucles principales (`src/mission_main.c`, `src/main.c`); ninguna llamada
  rspq desde interrupciones. `rdpq_attach_clear`/`rdpq_detach_cb` van en
  orden y solo en lowpri.
- La otra carrera corregida en `preview` (`7c57c409d`, traspaso de búfer
  lowpri partido por una preempción highpri entre `WRITE_STATUS(BUFDONE)` y
  `JUMP`) deja al RSP parado en **lowpri** (no coincide con la foto) y no es
  alcanzable con el mezclador del pin: `mixer_exec` hace `rspq_highpri_sync`
  tras cada mezcla, así que el CPU no puede llegar al `memset` del búfer
  antes de que el RSP relea su `JUMP` (<2 µs después de apagar `RUNNING`).
  Requiere además `RSPQ_CMD_WRITE_WORD`, que no existe en el pin; no se
  porta.

### Corrección

Se mantiene el commit `e356bf3` y se le aplican dos parches de la rama
`preview` de libdragon, guardados en `tools/libdragon-patches/` y aplicados
por `tools/setup.sh` (`git am` sobre el pin, rama local `msx-pin`; el `.deb`
del toolchain gcc no cambia, solo se reconstruye e instala `libdragon.a`
con `make -C <sdk>/libdragon -j4 install`):

1. `43c67dcf5` *rspq: fix highpri REQUESTED-clear race (set signal before
   queueing the clear)*: `rspq_highpri_begin` escribe `SET_REQUESTED` **antes**
   de encolar el `WRITE_STATUS` que lo apaga. El RSP no puede ejecutar el
   `clear` antes de que exista, y existe después del `set`: la carrera queda
   cerrada por construcción, sin tiempos de espera.
2. `cd5b8f567` *rspq: add workaround for Ares bug causing RSP crashes*: un
   salto en la secuencia de `nop` de `rspq_flush_internal` para que el JIT
   de ares resincronice CPU y RSP (el fallo de `display_get` que motivó el
   vigilante). Inocuo en hardware.

Un SDK ya preparado (árbol en `e356bf3`) recibe los parches al volver a
ejecutar `tools/setup.sh`; el script comprueba que `HEAD` sea el pin más
exactamente esos dos commits.

### Evidencia

Reproducción en ares (`tools/test_mission_native_ares.py`, misión 1): para
hacer alcanzable la ventana de ~10 instrucciones se añadió temporalmente
`wait_ticks(2000)` (≈43 µs, el coste de una interrupción) entre los dos
pasos de `rspq_highpri_begin`, sin tocar el juego.

- Sin el parche 1 (`e356bf3` + espera): `RSP CRASH` en los primeros segundos
  de juego, antes de la primera ventana de 300 fotogramas, con la firma de
  la foto:

      RSP CRASH | rsp_queue | rspq_highpri_sync (src/rspq/rspq.c:1081)
      Crash symptom: wait loop timed out (200 ms)
      PC:018 | STATUS:3403 [halt broke sig3 sig5 sig6 ] | DP_STATUS:  80 [ready ]
      RSPQ: Highpri DRAM address: 021dc428
      RSPQ: Current DRAM address: 021dc428 + GP=0 = 021dc428
      RSPQ: Current Overlay: rsp_mixer (1)

  (`PC:018` es la instrucción siguiente al `break` de `WaitNewInput`; los
  registros `t0`, `t1`, `s4`, `sp`, `ra` coinciden con la foto.)
- Con el parche 1 y la **misma** espera (12 minutos de ares, hasta que el
  guion agotó su tiempo en la zona del jefe): 59 ventanas de 300
  fotogramas (17.700 fotogramas), `crash=0`, 16 *underruns* en 10.894
  búferes de AI con los 43 µs añadidos a cada `begin`.
- Recorridos completos con la librería final (sin espera), dos pasadas de
  30 minutos con el anfitrión muy cargado (carga media 9–20 en 4 núcleos,
  otras sesiones de ares en paralelo): 71 y 90 ventanas de 300 fotogramas
  (21.300 y 27.000 fotogramas), `RSP CRASH`=0, sin patadas del vigilante
  (`MSX rspq kick`=0), *underruns* 5/9.127 y 26/13.305 búferes de AI
  (0,05 % y 0,2 %, con `audio=` medio ≈0,9–1,2 ms y máx. ≈4 ms por
  servicio). La segunda pasada llegó al jefe (`boss.png`); la primera se
  quedó en el derrumbe de las cúpulas por el guion de entrada (sin
  enemigos ni errores en el registro), no por el RSP: en la pasada con
  espera añadida la misma ROM parcheada superó cúpulas, tienda, camión y
  helicóptero hasta dañar al jefe.

### Peor fotograma (62,9 ms en el jefe de la ROM 6)

No está relacionado con la cola RSP: en estas pasadas el máximo de `cpu`
por ventana (44–66 ms) coincide siempre con `asset=…/29.000–44.000` µs
(descodificación síncrona de un bloque del pack en un `require` fallido:
`misses=26–29`), repartido en `prep`/`begin`/`bgreq`/`actors` según dónde
cae la carga; `audio` (todo `msx_audio_service`, incluida la espera
`rspq_highpri_sync` de cada mezcla) es ≈1 ms de media y ≤4–5 ms de
máximo. El resto de fotogramas de cada ventana queda en 13–15 ms de
`cpu`. Reducir ese pico es cuestión de prefetch/residencia del pack
(`scene_assets.c`), no del mezclador ni de rspq.

## Picos del combate con el jefe: CRC por tabla

**Causa.** Cada bloque del pack (16 KiB descodificados) se verificaba con un
CRC-32 bit a bit: 131.072 pasos, ≈8 ms en el N64, más el RLE byte a byte.
Una descodificación costaba ≈14 ms, tanto en un fallo síncrono (`require`)
como en el prefetch (`scene_service`, hasta 3 por fotograma), de ahí los
`asset=…/29–44 ms` (2–3 bloques en un fotograma). Con 606 bloques
(9,7 MiB descodificados) y ≈277 ranuras de caché (4,4 MiB) el pack no cabe:
553 bloques son del banco nativo, con un 40 % de ocupación real de página
(3,6 MB de píxeles en 9,0 MB de páginas de 64×64).

**Cambio.** `scene_crc32` usa una tabla de 256 entradas (idéntica bit a bit
al CRC de referencia `scene_crc32_bitwise`; `test_scene_codec` lo compara en
búferes aleatorios y `test_scene_pack`, en `make check`, descodifica los 606
bloques del pack y compara tabla, bit a bit y el CRC de `pack_scene.py`); el
RLE rellena cada tramo con `memset`. Mismo resultado, sin cambios visuales.

**Medida** (ares 146, `mslugx-reference.z64`, recorrido completo con
`test_mission_native_ares.py`, ambas PASS):

| | Antes (HEAD 895d076) | Después |
| --- | ---: | ---: |
| Peor fotograma (`cpu` máx.) | 103.180 µs | 57.849 µs |
| `asset` máx. en un fotograma | 70.147 µs | 36.411 µs |
| Peor fotograma de las demás ventanas | 66–82 ms | 39–48 ms |
| `cpu` media máx. (ventana del jefe) | 22.771 µs | 22.435 µs |
| fallos de caché (`misses`) | 100 | 101 |
| *underruns* de audio | 41 | 13 |

**Pendiente.** El objetivo de <33 ms no se alcanza: el pico restante es un
fotograma con ≈36 ms de `asset` (varios fallos síncronos seguidos al cambiar
de animación el jefe, cada uno con su DMA y, si no hay ranura libre de
fotogramas en vuelo, `rspq_wait`). Y la media del combate con el jefe ya es
≈22 ms (`bg` ≈10,7 ms, `sim` ≈3,8 ms, `actors` ≈4 ms: el juego presenta a
30 Hz ahí), así que <16,7 ms exige trabajo de dibujo además del pack. Pasos
siguientes: empaquetar varias piezas nativas por página (el pack bajaría a
≈5 MiB descodificados y cabría casi entero), prefetch de los descriptores
del jefe por fase, y un presupuesto de descodificación por fotograma.

## Combate con el jefe, segunda ronda: presupuesto de descodificación y fondos grabados

**Cambios** (`65c657d`, `e0dc093`, `ffbc8ed`):

1. **Presupuesto de descodificación por fotograma con alternativa no
   bloqueante** (`scene_assets.c`). Como mucho una descodificación de bloque
   por fotograma entre el prefetch (`scene_service`; si ya se gastó, el
   bloque leído por DMA espera en su búfer: `held=`) y las piezas de sprite
   de los actores (`scene_page_try`, activo en `draw_native`): una pieza cuyo
   bloque no está residente cuando ya se gastó el presupuesto se pide con
   prioridad máxima y no se dibuja ese fotograma (`deferred=`), en lugar de
   encadenar varias esperas DMA+descodificación. El fondo, Marco y el HUD
   siguen siendo síncronos (`scene_page`).
2. **Prefetch del jefe**: mientras dura el combate se piden con la prioridad
   más baja todos los bloques de las poses del jefe
   (`SCENE_BOSS_CHUNK_LIST`, 65 bloques, `pack_scene.py`); con 8 MiB caben
   junto al mapa de la arena, así que un cambio de animación encuentra sus
   piezas residentes. Con 4 MiB no se hace (no caben, MISION1_COMPLETA.md).
   Es «por fase» en el sentido amplio: el conjunto de todas las fases.
3. **Fondos estáticos grabados** (`scene_renderer.c`): el coste de dibujo
   del combate era el fondo (`bg` ≈10 ms con la cámara quieta). Si las
   entradas de las capas (desplazamientos, subidas, visibilidad, celdas de
   sustitución, vistas de profundidad) se repiten, cada pasada (capas 0..2 y
   3..4) y fase de animación (`video_tick & 7`) se graba una vez como bloque
   rspq y se reproduce: los mismos comandos RDP sin reconstruirlos. El estado
   TMEM/TLUT del renderer se reinicia antes de grabar y tras reproducir (cada
   bloque sube lo que dibuja); los bloques de memoria que lee un bloque
   quedan fijados por generación (4 grupos) y se liberan, con los bloques
   rspq, cuando el RDP terminó el último fotograma que los usó. Desactivado
   durante un fundido o el destello blanco. Con todas las fases grabadas se
   omite además el recorrido de prefetch del fondo (`bgreq` ≈1,8 → 0,1 ms).
   Línea `MSX RDP`: `bgblocks=reproducciones/grabaciones`.

**No hecho**: empaquetar varias piezas nativas por página (cambio de formato
del pack y del renderer: `scene_image_t` sin desplazamiento s/t); con los dos
primeros cambios el pico ya no lo pide.

**Medida** (ares 146, `mslugx-reference.z64`, recorrido completo con
`test_mission_native_ares.py`; «antes» = `abe9403` sin presupuesto, mismo
ares, misma sesión):

| | Antes | `e0dc093` | `ffbc8ed` |
| --- | ---: | ---: | ---: |
| Peor fotograma (`cpu` máx., todo el recorrido) | 57.174 µs | 37.817 µs | 35.304 µs |
| Peor fotograma en el combate | 40.632 µs | 33.894 µs | 27.054 µs |
| `cpu` media del combate (media de ventanas) | 20.633 µs | 13.474 µs | 10.905 µs |
| `cpu` media, peor ventana | 22.623 µs | 18.768 µs | 18.788 µs |
| `bg` medio en el combate | 9.959 µs | 4.119 µs | 2.858 µs |
| fotogramas con 2 pasos (media por ventana del combate, de 300) | 282 | 122 | 83 |
| *underruns* de audio | 9 | 3 | 2 |
| resultado | PASS | PASS | PASS |

Los picos que quedaban en `e0dc093` (31–38 ms) eran una descodificación de
prefetch en `bgreq` (≈8 ms) más la síncrona de una pieza (≈8–18 ms de
`asset`) en el mismo fotograma, sobre la simulación del jefe (`sim` hasta
10–13 ms en las ventanas de las explosiones): de ahí el presupuesto
compartido de `ffbc8ed`. La simulación del jefe (`step` máx. ≈13 ms) es lo
siguiente si se quiere bajar más.

Con `ffbc8ed` el combate queda por debajo de 33 ms en todos sus fotogramas
(27,1 ms el peor) y su media de ventanas en 10,9 ms (60 Hz salvo las
ventanas de las explosiones). El peor fotograma del recorrido (35,3 ms) ya
no es del jefe: es en el tramo con desplazamiento de la cámara 2336, una
falta síncrona de un bloque del **fondo** (`asset` 17,9 ms; el fondo, Marco y
el HUD no se aplazan) sumada a la simulación. `deferred=157` piezas
aplazadas un fotograma y `held=238` descodificaciones de prefetch movidas al
fotograma siguiente en todo el recorrido. Pantalla de MISSION 1 COMPLETE con
las letras nativas: `mission-complete-letters.png` del recorrido (fuera de
Git: deriva de la ROM).

## Expansion Pak: presupuesto de RAM medido, mapa fijado y precarga por tiempo libre

**Problema.** El diseño «todo el pack residente» ya no se cumplía: el pack
tiene 665 bloques de 16 KiB (10,4 MiB descodificados) y la consola tiene
8 MiB. `scene_assets_init` seguía cargando el pack entero al arrancar (las
`startup evictions` ≈ 380-400 del registro eran eso: 665 descodificaciones
síncronas, ≈5,3 s con la pantalla en negro tras el logo NEOGEO) y la caché se
dimensionaba con una reserva fija adivinada (`SCENE_HEAP_RESERVE`, 1,5 MiB).
El peor fotograma del recorrido (≈35 ms, cámara 2336) era una falta síncrona
de un bloque del fondo: el banco nativo (612 bloques) recicla la caché LRU y
podía echar bloques del mapa, y al llegar a 2336 el guion crea seis tipos de
actor a la vez (66 bloques nuevos).

**Cambios** (`src/scene_assets.c`, `src/main.c`, `src/neogeo_logo.c`,
`tools/pack_scene.py`):

1. **Presupuesto de RAM medido y comprobado.** Orden de arranque: pantalla y
   mezclador → audio del juego (`game_audio_init`; lo que deje residente en
   RAM ya cuenta) → caché de escena (`scene_assets_begin`) → logo → juego
   (`actor_init`). La caché se dimensiona la **última** con lo que queda del
   montón menos las reservas medidas de lo que se asigna después (tabla de
   abajo, constantes `SCENE_RESERVE_*_KB`) y un margen de 192 KB; con menos de
   208 ranuras falla con un `assertf` que da los KB. `scene_assets_check_budget`
   comprueba (assert) tras cargar el juego que queda la reserva de juego más
   medio margen, y la línea `MSX RDP` informa del mínimo de montón libre visto
   (`heap_low=`, muestreado cada 30 fotogramas). Líneas `MSX MEM <etapa>` en el
   arranque.
2. **Mapa entero fijado y residente** (Expansion Pak): los 37 bloques del mapa
   (celdas, parche de la tienda y escombros de las cúpulas) se fijan como los
   bancos de Marco y se cargan antes del primer fotograma, en orden de cámara.
   El fondo ya no puede fallar y su recorrido de prefetch (`bgreq`) se omite
   (`scene_assets_map_resident`). Con 4 MiB sigue el camino anterior: 32
   ranuras, vista inicial cargada, el mapa se transmite con el margen −48..352.
3. **Precarga por el guion** (`scene_prefetch_ahead`): `pack_scene.py` genera
   `scene_ahead` (cámara de aparición, bloque) desde el inventario del resto de
   la misión (`data/mission1_rest_inventory.json`: los registros de aparición
   del guion, la cámara de cada uno y los descriptores que dibujó cada tipo de
   actor en MAME): 487 pares, 251 bloques. Cada fotograma se piden, con la
   prioridad más baja, los bloques de lo que el guion crea en los próximos 384
   píxeles de cámara, y los ya residentes se marcan como recién usados para
   que la LRU no los recicle antes de la aparición. Solo afecta al dibujo.
4. **Descodificación en tiempo libre** (`scene_assets_idle`): mientras la CPU
   espera un framebuffer (`msx_display_get`) se descodifican bloques pedidos,
   solo si la estimación (la descodificación más lenta reciente, ≤7,3 ms medida)
   termina antes del próximo borrado vertical menos 1,2 ms (una interrupción
   VI marca cada borrado): el fotograma que espera ese borrado nunca se
   retrasa. No cuenta en el presupuesto por fotograma (`SCENE_SERVICE_DECODES`).
5. **Calentamiento durante el logo NEOGEO**: el logo llama a
   `scene_assets_idle` en el tiempo que le sobra de cada fotograma (≈7 ms). La
   lista de calentamiento es: bancos fijados, mapa en orden de cámara y luego
   los bloques de `scene_ahead` en orden de cámara (estos solo en ranuras
   libres: calentar nunca expulsa). Si el logo se salta con Start,
   `scene_assets_init` carga de forma síncrona solo la parte obligatoria
   (bancos y mapa, 49 bloques).

La simulación no lee nada de esto (`make check`, -O0/-O2 idénticos).

### Orden de arranque

- **Sin las cinemáticas privadas** (`filesystem/cinematics` ausente): pantalla
  → audio del juego → caché de escena → logo NEOGEO (calentando la caché en
  su tiempo libre) → Misión 1.
- **Con ellas** (`title-motion.*`, `entry.*`): logo → título X64
  (`msx_title_intro`) → HOW TO PLAY → SOLDIER SELECT → elección → entrada en
  paracaídas → Misión 1 (`actor_begin_descent` con el traspaso medido de
  `entry.json`). Lo que el juego carga se adelanta al tiempo libre de las
  cinemáticas (sección «Cinemáticas sin parones»): efectos residentes, piezas
  de Marco y copia de la ROM S del HUD desde SOLDIER SELECT; la caché de
  escena empieza durante la elección con las ranuras que caben (calentadas
  allí) y se completa al terminar la entrada, con el mismo tamaño que tendría
  creada entonces.

### Presupuesto de RAM (8 MiB, ares 146, `mslugx-reference.z64`, líneas `MSX MEM`)

Columna «rama»: esta rama sola (`7b47b06`). Columna «fusión»: con
`work-world`/`work-weapons` (efectos residentes, mundo 96/160/160, pack de 711
bloques) y las cinemáticas de arranque.

| Concepto | Rama (KB) | Fusión (KB) | Notas |
| --- | ---: | ---: | --- |
| RDRAM | 8192 | 8192 | Expansion Pak |
| Imagen del programa (texto+datos+bss) | 1618 | 1681 | `mips64-elf-size` |
| Pila | 64 | 64 | `STACK_SIZE` de libdragon |
| **Montón total** | **6534** | **6446** | `sys_get_heap_stats` |
| Pantalla (2 framebuffers 320×240×16), mezclador, rdpq | 450 | 451 | antes de todo |
| Logo NEOGEO (z-buffer, malla, flujo del jingle) | 169 | 169 | se libera al acabar |
| Título X64 (película + 16 fotogramas preparados) | — | ≈5500 | se libera al acabar |
| Entrada (película + 21 fotogramas preparados) | — | ≈4100 | se libera al acabar |
| Audio del juego | 4 | 843 | efectos residentes (`msx_ramfs`, 851.270 B) en la fusión |
| **Caché de escena** | **4816** (301 ranuras) | **3872** (242 ranuras) | + ≈40 KB de ranuras y staging; se dimensiona la última |
| `actor_init` (poses de Marco, ROM S del HUD + tira, índice del mapa, fuente) | 560 | 552 | reserva 640 |
| Libre tras cargar el juego | 659 | 676 | assert ≥ 320 + 128 |
| Mínimo libre en el recorrido (`heap_low`) | 465 (hasta 1920) | 416 | crecimiento en juego ≤ 260 KB; reserva 320 |
| Margen | 256 | 256 | |

Las reservas (`SCENE_RESERVE_LOGO_KB` 224, `SCENE_RESERVE_GAME_KB` 640,
`SCENE_RESERVE_RUNTIME_KB` 320, `SCENE_HEAP_MARGIN_KB` 192) suman 1152 KB
fuera de la caché. Lo que se cargue antes de dimensionarla (más efectos, otro
audio) la reduce automáticamente; por debajo de 208 ranuras el arranque falla
con un mensaje que da los KB. Con 4 MiB nada de esto se aplica: 32 ranuras,
como antes.

### Medidas

Arranque (`tools/test_logo_ares.py`, líneas `MSX MEM`/`t=`):

| | Antes (`436c69a`) | Rama, sin cinemáticas | Fusión, con cinemáticas |
| --- | ---: | ---: | ---: |
| Negro tras la última pantalla de arranque hasta el primer fotograma | ≈5,5 s (5,29 s solo del pack) | 283–301 ms | 894 ms tras la entrada (efectos 283 ms, 50 bloques 281 ms, resto de `actor_init` 280 ms) |
| Bloques cargados en el tiempo libre del logo | 0 | 243–299 | 0 (no hay caché aún) |
| Fotogramas tardíos del logo (`late`) | 0 | 0 | 0 |
| Fotogramas perdidos del título / de la entrada | — | — | 0 / 0 |

Recorrido completo (`tools/test_mission_native_ares.py`, `mslugx-reference.z64`):

| | Antes (`436c69a`, registro anterior) | Fusión sin esta rama (coordinador) | Esta rama + fusión, sin cinemáticas (`780633c`) | Final, con cinemáticas |
| --- | ---: | ---: | ---: | ---: |
| Resultado | PASS | PASS | PASS, crash=0 | PASS, crash=0 |
| Pack / caché | 665 / 266 ranuras | 711 / 227 | 711 / 243 | 711 / 242 |
| Faltas síncronas del **fondo** (`sync=`) | ≥1 (la de 2336) | sin medir | **0** | **0** |
| `misses` (todas, piezas de actor aplazables incluidas) | 133 | 166 | 149 | 137 |
| `evictions` | 379 | — | 460 | 444 |
| `deferred` / `held` | 113 / 207 | — | 36 / 231 | 31 / 374 |
| Descodificaciones en tiempo libre (`idle=`) | — | — | 152 | 262 |
| Peor fotograma del recorrido | 35,3 ms (2336: `asset` 17,9 ms, fondo) | 42,7 ms | 53,0 ms (2336: `sim` 28,0 ms, dos pasos de 15,6 ms; `asset` 12,0 ms) | 35,9 ms (primera ventana, arranque) |
| Peor fotograma en 2336 | 35,3 ms | — | 53,0 ms (simulación) | 33,8 ms (`sim` 18,9 ms, `asset` 11,9 ms) |
| Mínimo de montón libre | — | 709 KB | 416 KB | 421 KB |
| *underruns* de audio | 2 | — | 8 | 4 |

**Lectura.** El objetivo de la rama se cumple: ninguna falta síncrona del
fondo en todo el recorrido (el mapa está fijado) y el pico de 2336 ya no es
del fondo. Lo que queda en los picos es (1) la **simulación** del mundo en
2336 (seis tipos de actor creados a la vez por el guion; `step` hasta 12,5–15,6
ms, dos pasos en un fotograma), que no es de esta rama: es el coste nuevo de
`work-world`/`work-weapons` (más actores nativos, restos y etiquetas; la
fusión sin esta rama ya daba 42,7 ms), y (2) **una** descodificación síncrona
de una pieza de actor aplazable (`asset` ≈10–12 ms con su DMA; el presupuesto
`SCENE_SYNC_DECODES=1` la permite para no dejar al actor sin dibujar un
fotograma). La precarga por el guion cubre el resto de la misión (cámara ≥
1920); la región (0..1920) no tiene inventario de apariciones, así que sus
soldados siguen dependiendo de la petición del mismo fotograma. Siguiente
paso posible: un inventario de la región como `mission1_rest_inventory.json`,
o aplazar siempre (`SCENE_SYNC_DECODES=0`) ahora que el tiempo libre
descodifica 150–260 bloques por recorrido.

## ROM 12: picos de 53,7 ms en 2336 (búsquedas lineales y dos actualizaciones del mundo por fotograma)

El recorrido de ROM 12 (`e61a00c`) dio un peor fotograma de 53,7 ms frente a
38,8–40,1 ms de ROM 11. Comparando ventana a ventana (cámara de la última
línea `MSX camera` antes de cada `MSX PERF`), las dos ROM tienen el mismo
perfil: el pico está siempre en 2336 (y 3376) y es de **simulación**, no del
fondo ni de la caché (`asset` ≈ 12 ms en ambas). Una línea de diagnóstico
temporal por paso lento (> 7 ms: actor por actor dentro de `neo_world_tick`)
mostró:

- Cada actualización del mundo (una de cada dos llamadas a
  `msx_game_update`: el mundo avanza con `ticks` par) cuesta 9–13 ms en
  2336 (premios cayendo 08C53C, proyectiles C 063FDC con su estela 063E48,
  restos 01D6E4…): 50–60 actores a 150–400 µs cada uno. Los pasos impares
  cuestan ≈ 0,1 ms.
- El fotograma de 53,7 ms ejecutó **cuatro** pasos (`steps=404/4`): dos
  actualizaciones del mundo (`sim` 31,2 ms) más una descodificación síncrona
  (`asset` 11,8 ms). ROM 11 llegó a tres pasos (`sim` 20,9 ms, 40,1 ms). La
  diferencia entre ROM 11 y ROM 12 es sobre todo cuántos pasos de
  recuperación caen en el mismo fotograma, no un actor nuevo.
- En el perfil del anfitrión (`tools/test_gameplay.c`, gprof/cachegrind con
  cachés de 16/8 KB como la VR4300) la función más cara era `ga_rom`
  (25 % del tiempo, 4,6 M llamadas): recorría las 113 regiones de ROM de
  forma lineal en cada lectura de tabla (senos, rectángulos, descriptores);
  `program()` recorría igual los 665 programas de animación.

Cambios (sin tocar la simulación; `make check` da salidas idénticas):

1. `ga_rom`: búsqueda binaria sobre las regiones ordenadas una vez, con el
   máximo acumulado de sus finales para recorrer hacia atrás solo las que
   aún pueden contener la dirección; misma respuesta que el recorrido
   lineal (la región con más bytes tras la dirección, la primera en caso de
   empate). `program()`: índice ordenado por etiqueta y búsqueda binaria (la
   primera entrada de cada etiqueta). En el anfitrión `ga_rom` pasa del
   25 % al 3,5 %; en ares la media de `sim` en la ventana estable de 3072
   baja de 2,9 a 2,2 ms y su máximo de 8,2 a 5,1 ms.
2. Un fotograma ejecuta como mucho **dos** pasos (`MSX_FRAME_STEPS`,
   `actor.c`): así nunca hay dos actualizaciones del mundo en el mismo
   fotograma. Los pasos sobrantes vuelven al reloj
   (`scene_clock_defer`) y se ejecutan en los fotogramas siguientes: no se
   pierde ningún paso (prueba en `tools/test_scene_clock.c`). La
   recuperación es más lenta (un paso extra por fotograma en vez de siete).

| Recorrido completo (ares) | ROM 11 (`r11ref`) | ROM 12 (`e61a00c`) | Con los cambios |
| --- | ---: | ---: | ---: |
| Peor fotograma | 40,1 ms | 53,7 ms (2336, 4 pasos) | 36,2 ms¹ (2336, 2 pasos; arranque 36,2 ms) |
| Pasos máx. por fotograma | 3 | 4 | 2 |
| `sim` máx. en 2336 | 20,9 ms | 31,2 ms | 14,7 ms¹ |
| `sim` media / máx. en 3072 | 2,8 / 7,5 ms | 3,0 / 8,2 ms | 2,2 / 5,1 ms¹ |

¹ Con la instrumentación de diagnóstico activa (un poco más lenta). Medida
final sin ella (`mslugx-reference.z64`, PASS, crash=0): peor fotograma
35,2 ms (primera ventana, arranque) y 35,2 ms en 2336 (`sim` 11,8 ms, 2
pasos; `asset` 12,3 ms); 3376: 32,8 ms; `sim` en 3072 ≈ 2,4 / 5,4 ms;
212 ranuras, 1227 KB libres tras la caché, mínimo libre 357 KB.

Queda el coste propio de la actualización del mundo en 2336 (≈ 11 ms con
50–60 actores): es trabajo distribuido entre muchos actores, sin una
función dominante tras los cambios.

### Caché de escena y montón (ROM 12)

Mapa del montón medido en el arranque con cinemáticas (bloques libres,
reservando el mayor cada vez): tras la entrada, 3368 + 1976 + 148 + 136 KB;
un bloque pequeño (≈ 11 KB) que queda vivo de las cinemáticas parte el
montón en dos. Los 60 efectos (1003 KB, ROM 11: 57 y 851 KB) llenan los
huecos bajos y 1976 KB → 1220 KB. Las ranuras de la caché (`e61a00c`) se
reparten en dos bloques de 105 y una suelta, y tras la caché quedan 1240 KB
libres = la reserva (1216 KB) + < 1 ranura. Es decir, la fragmentación ya no
cuesta ranuras: el número lo fija el total libre menos las reservas
(`actor_init` 615 KB de 640; crecimiento en juego ≈ 260 KB de 320; margen
256 KB; mínimo libre en el recorrido 367 KB). Reservar los efectos en un
solo bloque o la caché antes que ellos no daría ninguna ranura más; para
subir de 211–212 ranuras (mínimo 208) hay que bajar el margen (128 KB
darían 8 ranuras) o reducir lo residente.

## Bloques del fondo y montón (rama `work-m2eng3`)

**Margen 192 KB.** Tras la fusión de los actores de la Misión 2 el ELF creció
≈ 93 KB y el arranque fallaba (`RAM budget: 3295 KB left for the scene cache
(minimum 3328 KB)`): `SCENE_HEAP_MARGIN_KB` baja de 256 a 192 (la salida que
ya proponía la sección anterior). Medido en ares: 3344 KB de caché (209
ranuras), 560 KB libres tras cargar el juego, mínimo en la etapa 2 de la
Misión 2 302 KB.

**Bloques del fondo liberados con el RSP escribiendo en ellos.** Tras
ejecutar un bloque, libdragon deja que el RSP escriba los comandos RDP
siguientes (el resto del fotograma, su `SYNC_FULL`, los fotogramas
siguientes hasta otro bloque) en el espacio libre del último búfer estático
de ese bloque (`__rdpq_block_end`); solo `rspq_wait` lo devuelve al búfer
dinámico. `bg_collect` liberaba los bloques retirados en cuanto su
fotograma terminaba, sin esa espera: el RSP escribía comandos en montón
libre. Con un detector (`mallinfo` en cada grabación) el montón aparece
corrupto ya en la etapa 1 de la Misión 2 (enlace de bloque = 0x0025A680, la
dirección física de un framebuffer). En la etapa 2 los bloques se retiraban
cada dos fotogramas (el juego actualiza uno de cada dos y la clave del fondo
se repite una vez: se grababan las dos pasadas en cada repetición, 2 de cada
3 ejecuciones eran grabaciones) y el cuelgue llegaba siempre: 20 `MSX rspq
kick` y el diagnóstico de `display_get` (sin excepción; el inspector ni
siquiera podía reservar sus búferes). Es probablemente también el origen
de los cuelgues raros de recorridos largos de la Misión 1 que el *kick*
intentaba despertar.

Corrección (`scene_renderer.c`): tras cada `rspq_block_run` del fondo se
ejecuta un bloque permanente `bg_tail` (el `rdpq_set_scissor(8,8,312,232)`
que ya seguía), así el puntero de escritura del RSP queda siempre en un
búfer que nunca se libera; `scene_renderer_stage_changed` ya libera tras
`rspq_wait`. Además se graba solo cuando la clave se ha mantenido 3
fotogramas (`BG_RECORD_HELD`, dos actualizaciones: el desplazamiento ya no
graba) y nunca con menos de 192 KB libres (`BG_RECORD_MIN_FREE`: libdragon
no comprueba los `malloc_uncached` de `rspq_block_begin` ni de
`__rdpq_block_next_buffer`; un NULL escribiría en la dirección física 0).
Comprobado en ares con el comportamiento antiguo de grabación más el
detector: sin corrupción ni cuelgue hasta el final de la etapa 2.

## Presupuesto de RAM por misión: imagen del programa reducida, margen 256 KB

**Problema.** Cada misión nueva añade código C nativo y tablas a un único ELF
que se carga entero en RDRAM. La caché de escena se dimensiona la última con
lo que queda, así que cada KB de imagen es un KB menos de caché: con la
misión 2 el arranque fallaba («RAM budget 3303 KB < 3328 KB», 208 ranuras) y
el margen del montón se había bajado 256 → 192 → 160 KB para compensar.

**Medida** (`mips64-elf-size`, `nm --size-sort` por objeto, `mslugx-explorer.elf`
en `f30a292`): texto 826 KB, `.rodata` 1,18 MB, bss 158 KB. Lo grande de
`.rodata`: `neo_world.o` 592 KB (programas de animación del banco P2 de la
misión 1, `build/soldier_ai_tables.h`: 760 programas, 28.796 comandos de 16 B
= 460 KB, más 115 KB de direcciones ROM por comando), `scene_assets.o` 344 KB
(`build/scene_meta.h`: imágenes, celdas del mapa de la misión 1,
descriptores), `neo_region.o` 96 KB, `neo_boss.o` 40 KB. El código propio de
la misión 2 (`neo_rebel2/3`, `neo_m2boss`, `neo_gate`, `neo_drone`,
`neo_hostage`, `neo_crate`, `neo_m2props`) son 57 KB; sus tablas de escena ya
van en `mission2.pack` y se cargan en ranuras reservadas de la caché
(`scene_stage_load`), sin coste en el ELF.

**Cambios** (sin tocar la simulación; `make check` -O0/-O2 idéntico):

1. **`neo_anim_command_t` de 16 a 12 B** (`src/neo_animation.h`,
   `packed,aligned(4)`, `kind:8`): mismo orden de campos y mismos valores,
   `asset` se lee sin alinear. Afecta a los programas de soldado, jefe y Marco.
2. **Direcciones ROM de los programas de soldado solo en el host**
   (`tools/compile_soldier_ai_tables.py`): únicamente `test_neo_family` las lee;
   en N64 `address` es `NULL`. Las del jefe (`neo_boss.c` las usa) se quedan.
3. **`-Os` para el código frío** (`Makefile`, `COLD_OBJS`): módulos de
   actores, jefes, fin de misión, cinemáticas y logo. El núcleo por fotograma
   (`HOT_SRCS`: mundo, IA de soldado, núcleo, animación, contacto, control,
   cámara, guion…) y `gameplay`/`actor`/renderers siguen en -O2.
4. **Margen del montón restaurado a 256 KB** (`SCENE_HEAP_MARGIN_KB`).

| Imagen del programa (`mslugx-explorer.elf`) | Antes (`f30a292`) | Después |
| --- | ---: | ---: |
| texto | 826.360 B | 753.240 B |
| datos (`.rodata`+`.data`) | 1.228.836 B | 985.180 B |
| bss | 157.884 B | 157.884 B |
| **total** | **2.213.080 B (2161 KB)** | **1.896.304 B (1852 KB, −309 KB)** |

### Presupuesto con la misión 2 (8 MiB, ares 146, cartucho de lanzamiento con cinemáticas, líneas `MSX MEM`)

| Concepto | Antes (KB) | Después (KB) | Notas |
| --- | ---: | ---: | --- |
| Imagen del programa | 2161 | 1852 | tabla de arriba |
| **Montón total** | ≈5966 | **6275** | `sys_get_heap_stats` |
| Pantalla, mezclador, rdpq | 452 | 452 | |
| Audio del juego (efectos residentes) | ≈993 | 993 | `MSX MEM audio` − `title done` |
| `actor_init` (Marco, HUD, índice del mapa, fuente) | — | 615 | `game loaded` − `scene cache`; reserva 640 |
| Reservas fuera de la caché | 960+160 | 960+256 | juego 640 + juego en marcha 320 + margen |
| **Caché de escena** | 3303 con margen 192 (falla, < 3328); 3335 con margen 160 | **3536 (221 ranuras) con margen 256** | mínimo 208 ranuras = 3328 KB |
| Libre tras cargar el juego | — | 624 | assert ≥ 320 + 128 |
| Margen | 160 | **256** | |

**Holgura para misiones futuras.** Con margen 256 quedan 13 ranuras
(208 KB) sobre el mínimo de 208. La misión 2 añadió 57 KB de código (≈52 KB
con -Os), así que caben ≈4 misiones más de ese tamaño **siempre que sus datos
no vayan al ELF**. Reglas para las misiones 3–6:

- Tablas de escena y datos derivados de la ROM de una misión: en
  `missionN.pack` (DFS), cargados en `scene_stage_load` en ranuras reservadas
  de la caché, como la misión 2. Nada de cabeceras generadas con datos de
  misión en `.rodata`.
- Módulos de actores nuevos: a `WORLD_SRCS` y, salvo que estén en el camino de
  cada fotograma, fuera de `HOT_SRCS` (-Os).
- Revisar en cada misión `mips64-elf-size build/mslugx-explorer.elf` y la línea
  `MSX MEM scene cache` (ranuras ≥ 208 con margen 256). Si un día no basta, el
  siguiente paso es cargar el código de cada misión como módulo dinámico
  (`dlopen` de un `.dso` en DFS; el libdragon fijado trae `n64dso`) en ranuras
  liberadas de la caché al cambiar de misión: hoy no compensa (57 KB por
  misión frente a la complejidad de exportar la API del mundo).

Opciones medidas y no tomadas: `-Os` también en el núcleo por fotograma
ahorraría ≈31 KB más (`neo_world` 9 KB, `neo_soldier_ai` 7 KB, `gameplay`
4 KB…), a costa del tiempo de simulación del pico de 2336; sacar
`scene_meta.h` de la misión 1 a DFS no libera nada mientras se juega la
misión 1 (se necesita entero); los efectos residentes (≈850 KB) son una
decisión del audio (`docs/AUDIO_JUEGO.md`).

## Misión 2, etapa 3: fondo de la torre y subbloques de audio (rama `work-m2pol`)

Recorrido de referencia en ares (`mslugx-m2-reference.z64`, `--stage-seconds
400`, ventanas de 300 fotogramas). Antes: cpu media de la peor ventana 53.384
µs, de ella `bg` 39.981 µs; 888 subbloques de audio en todo el recorrido (741
en la subida a la torre). La línea nueva `MSX BG` (µs/dibujos/subidas/celdas
visitadas por capa en el fotograma de muestra) mostró que el coste era de la
CPU emitiendo celdas, no del RDP:

- **El modo de profundidad no recortaba en vertical** (`depth_cell` solo
  descarta por x): con el mapa de 272 filas de la etapa 3 todas las celdas de
  las columnas visibles se dibujaban fuera de pantalla. Ahora las dos rutas
  (clásica y de profundidad) saltan sin visitarlas las celdas de una columna
  que quedan por encima (búsqueda binaria hasta la primera que alcanza la
  pantalla) o por debajo (salto al final de la columna). Requisito: dentro de
  un tramo del índice (`scene_cull.c`) cada columna está ordenada por y; el
  tramo se parte también donde la y deja de crecer (18 sitios en el paquete
  de la Misión 2). `tools/test_scene_cull.c` compara el salto con el recorrido
  completo en 300 conjuntos aleatorios.
- **Torre**: las piezas visibles se dibujaban celda a celda en orden de
  pieza y cada celda volvía a subir su página (≈145–210 subidas de TMEM por
  fotograma). Las celdas de la torre nunca se solapan (rejilla de 16 px,
  piezas a 48 px), así que se agrupan por página y paleta y se fusionan las
  tiras de una columna: 11–15 subidas. Las piezas fuera de pantalla también
  se descartan en el modo de profundidad.

Audio: `msx_audio_service` se llamaba una vez por fotograma con dos búferes de
40 ms; un fotograma de más de ~80 ms (dos actualizaciones más el fondo
completo) los vaciaba. Ahora hay una segunda llamada entre la simulación y el
dibujo (`msx_audio_idle`, actor.c) y otra entre carga y carga de bloques en
`scene_stage_load` (`scene_assets_set_load_idle`): las cargas de etapa (0,14 /
0,29 / 0,64 s) ya no cortan la música.

| Misión 2 (ares, referencia) | antes | después |
|---|---|---|
| cpu media, peor ventana | 53.384 µs | 24.727 µs |
| `bg` medio, peor ventana | 39.981 µs | ≈11.200 µs |
| subidas de TMEM por fotograma (torre) | 207 | 15–75 |
| subbloques de audio (todo el recorrido) | 888 | 9 |

Las cargas de etapa siguen siendo un fotograma de 0,3–1,07 s con la pantalla
en negro (el fundido ya terminó, como en el original detrás de `3AED8`/
`3AEF0`); lo que se oía (la música cortada) queda resuelto con el audio entre
cargas. Hacerlas incrementales queda pendiente.

## Cinemáticas sin parones (rama `work-introperf`)

Informe de MiSTer (N64 real, Expansion Pak): la intro seguía con parones. En
ares (`tools/test_logo_ares.py`, reloj emulado; el PI de MiSTer es otro, por
eso las correcciones quitan trabajo bloqueante en vez de ajustar tiempos) el
arranque tenía estas pausas y saltos:

| Tramo | Antes | Después |
|---|---|---|
| logo → título | 1881 ms en negro (la preparación del título seguía en la lectura) | 0 ms (título leído y 17 fotogramas hechos durante el logo); `gap_vb=3` (los 2 negros del final del logo) |
| logo (fotogramas tardíos propios) | 2 | 0 |
| título | 0 saltados | 0 saltados, `max_vb=1` |
| título → HOW TO PLAY | 140 ms (apertura síncrona en streaming) | `gap_vb=1` (sin pausa) |
| HOW TO PLAY | 35 fotogramas saltados (descodificación de hasta 160 ms en el fundido 2638–2667; 1878, 2310) | 0 |
| HOW TO PLAY → SOLDIER SELECT | 654 ms (preparación atascada) | `gap_vb=1` |
| SOLDIER SELECT | 12 saltados | 0 |
| elección | 4 saltados | 0 |
| elección → entrada | 509 ms | `gap_vb=1` |
| entrada → juego | 1133 ms (`gap_vb=68`: audio 340, caché 310, Marco 306, HUD 40, reinicio 60…) | ≈130 ms (`gap_vb=7–8`: reinicio del mundo 60, renderizador 17, audio 17, primer fotograma 29) |
| primeros 2 s de juego | 22 de 120 fotogramas tardíos | 3–8 |
| 8 s siguientes | 0 | 0 |

`gap_vb`/`late` son las líneas nuevas `MSX FRAMES <tramo>` (`msx_perf.c`):
blancos verticales entre el último fotograma de un tramo y el primero del
siguiente (1 = sin pausa) y fotogramas que llegaron 2 o más blancos después
del anterior. `test_logo_ares.py` las guarda en `presentation` y exige
`late=0` en cada película.

### Causas

1. **Preparación que no avanzaba.** Cada paso de la preparación en tiempo
   libre (lectura+LZ4 de un paquete, CRC, aplicar) era indivisible y se
   rechazaba si no cabía; los fotogramas clave (≈110 KB: en ares ~17 ms de
   LZ4, ~19 ms de CRC por byte, ~20 ms de aplicación) nunca cabían en los
   ~5 ms libres del logo, así que la apertura lo hacía todo de golpe
   (1,9 s, 0,65 s, 0,5 s).
2. **HOW TO PLAY en streaming** (4,7 MB sin comprimir no entraban en caché):
   cada fotograma leía del PI con espera, descomprimía, hacía CRC y aplicaba
   en el tiempo del propio fotograma; los fotogramas clave costaban 70 ms.
3. **Copias de 136 KB por CPU** (`memcpy` ≈12 ms en ares) tras cada
   fotograma preparado.
4. **Traspaso al juego síncrono**: 1 MB de efectos, 585 piezas de Marco, la
   ROM S del HUD y 50 bloques de la caché de escena leídos con la pantalla
   quieta; el reinicio del mundo se hacía dos veces.
5. **Calentamiento en el tiempo del fotograma**: tras el traspaso,
   `scene_service` descodificaba un bloque de la lista de calentamiento
   (≈5 ms) en cada fotograma de los primeros ~70.
6. **Elección del fotograma tras mezclar audio**: con el tiempo libre ya
   usado, mezclar 2–3 ms antes de `display_get` retrasaba la elección del
   fotograma y saltaba uno.

### Correcciones

- **Reproductor nuevo (`src/cinematic.c`)**: por película, un *cargador*
  (DMA asíncrono del PI en trozos de 8 KB a un anillo de paquetes v1, LZ4 en
  sitio reanudable con CRC por trozos y validación por trozos:
  `src/cinematic_stream.h`) y un *productor* que decodifica fotogramas
  completos por adelantado en un conjunto de lienzos (el RDP copia el
  anterior —copia de 4 píxeles/ciclo— y la CPU aplica el paquete por trozos;
  los paquetes «sin cambio» comparten lienzo). Todo trabajo corre en piezas
  de 2–4 KB hasta el plazo del blanco siguiente (coste por KB aprendido, que
  sube enseguida y baja despacio); mostrar un fotograma solo elige su lienzo.
  El anillo ocupa la película entera si cabe (título bajo el logo: 3 MB) o
  2 MB (HOW TO PLAY: el fundido de 650 KB de 2638 se carga antes); una
  película preparada al lado de otra crece al abrirse. Los lienzos por
  película salen de sus paquetes pesados (>16 KB): 4 para la elección,
  8 para las demás. El título prepara HOW TO PLAY en su tiempo libre.
- **CRC de 4 en 4 bytes** (tablas de 4 KB) y **aplicación con `memcpy`/
  relleno por palabras**; CRC del primer y último fotograma calculados en
  segundo plano (la comprobación de la entrada y los registros costaban
  20 ms en el corte).
- **Las películas acaban en su último fotograma** sin una vuelta más de
  trabajo libre, y el fotograma se elige justo tras el blanco, antes de
  mezclar audio (`main.c`, `title_intro.c`).
- **Carga del juego adelantada** (`main.c`): desde SOLDIER SELECT,
  `msx_ramfs_preload` lee por DMA asíncrono los 60 efectos y `fixed.bin`
  (la ROM S del HUD, usada tal cual), `marco_renderer_preload` carga piezas
  de Marco mientras caben antes del blanco, y la lista de calentamiento de la
  caché se calcula en pasos (`scene_assets_plan_step`, 27 ms de golpe antes).
  Durante la elección, `scene_assets_begin_early` crea la caché con las
  ranuras que caben dejando sitio a las películas, la calienta en tiempo
  libre (`scene_assets_idle`) y crece al cerrarse la elección;
  `scene_assets_begin` la completa tras la entrada comprobando el tamaño
  contra el montón de entonces. Lo ya cargado de `actor_init` se descuenta de
  su reserva (`scene_assets_game_preloaded`), así que la caché y el montón
  libre final son los de antes (214–217 ranuras, ≥ 448 KB libres).
- `actor_prepare_descent`: el reinicio de `actor_init` ya empieza la bajada y
  `actor_begin_descent` no la repite.
- `scene_service` deja los bloques de calentamiento (los que nadie pidió) al
  tiempo libre antes del blanco; una petición o una espera síncrona los toma
  igual.

Píxeles: idénticos (CRC de cada película igual que antes en ares:
título `c0a8e830`, HOW TO PLAY `47fe0a98`, selección `ad129003`, elección
`342b3c8e`, entrada `be25a626`; `cinematic-regression.z64` compara los 30
fotogramas de la entrada; `make check-cinematic` compara LZ4/CRC/validación/
aplicación por trozos con las formas de una pasada en cada paquete).

Queda: el traspaso al juego son ~8 blancos (el reinicio del mundo, 60 ms en
N64 y despreciable en el host, y el primer dibujado del HUD, 19 ms); en los
primeros 2 s del juego, 3–8 fotogramas tardíos por la grabación de los
bloques del fondo (≈7 ms cada uno mientras la cámara baja) y algunas
peticiones de bloques de soldados aún no calentados.

## RAM de la caché de escena recuperada (rama `work-diag`, base `ab76871`)

`ab76871` ya no arrancaba la M1 en las ROM de referencia: `RAM budget: 3326 KB
left for the scene cache (minimum 3328 KB)`, es decir, 207 ranuras. ROM 20
(`5b49ffb`) tenía 214–219.

**Qué creció desde ROM 20** (montón total de `MSX MEM display`: 6258 KB en
ROM 20, 6141 KB en `ab76871` con objetos limpios; la referencia, 6136 KB):

| Origen | Crecimiento |
| --- | ---: |
| Código `5b49ffb..ab76871` con las mismas entradas (ELF de referencia 2.009.504 → 2.033.680 B): `neo_world.o` +11,3 KB (programas de soldado y código), `neo_drone` +3,8, `neo_hostage` +2,3, `marco_visual` +1,8, `gameplay` +1,2, `neo_prisoner` +1,1, `neo_m2props` +1,0, `actor` +0,8 | +24 KB |
| Entradas privadas regeneradas (`rest-native.json`, catálogo de Marco; diferencia restante del montón) | ≈ +95 KB |
| Efectos residentes: 1.031.042 B (61) → 1.215.338 B (63) | +184 KB |
| Registros de ranura reservados de más: `sizeof(cache_slot_t)` × 1007 trozos, cuando solo se crean las ranuras usadas | 16 KB |

**Recuperado sin quitar funciones** (la imagen de la referencia sin
diagnóstico pasa de 2.033.680 a 1.889.632 B, −141 KB):

- `neo_anim_command_t` de 12 a 10 bytes: `asset` es un campo con signo de 24
  bits junto al tipo (direcciones 68K y constantes `SET/WRITE_LONG`
  extendidas en signo; los generadores comprueban el rango). 34.220
  órdenes: −67 KB.
- Programas de soldado que son la cola de otro apuntan a su tabla
  (`compile_soldier_ai_tables.py`; los saltos son relativos y las órdenes
  idénticas): 1062 órdenes, −10,6 KB.
- Celdas del mapa de 22 a 8 bytes (`scene_map_cell_t`: imagen de la primera
  fase y patrón de deltas compartido; la M1 tiene uno, la M2 uno). Se
  comprobó que las 4699 celdas de la M1 dan las mismas 8 imágenes que antes;
  las de los paquetes de misión se convierten al cargar la etapa: −61 KB.
- Cálculo de la caché con los registros de las ranuras que se crean: +12 KB.

Ni `SCENE_EXPANDED_MIN_SLOTS` (208) ni el margen de 256 KB cambian. Nueva
línea `MSX CACHE slots=N` al cargar el juego:

| ROM (ares, arranque completo) | Antes | Después |
| --- | ---: | ---: |
| `mslugx-explorer.z64` | sin medir (≈210) | **218** |
| `mslugx-reference.z64` (con `MSX_DIAG`) | 207 (aserción) | **217** |
| `mslugx-m2-reference.z64` (con `MSX_DIAG`) | 207 (aserción) | **217** |
| Con `work-laser` (`686c0fd`) fusionada, sus entradas de `/home/user/ws-m3`: referencia / normal | 206 (3310 KB) | **215 / 216** |

`sfx_10a5` y `sfx_1026` siguen sin empaquetar: son bucles (motor),
`pack_game_audio.py` los descarta a propósito. Con 1–3 ranuras sobre 214
hay sitio para ellos (≈14 KB cada uno a 24 kHz VADPCM si se hacen bucles
residentes).

## Modo de diagnóstico (`MSX_DIAG`, rama `work-diag`)

Coste, recorrido completo de la M1 en ares, `mslugx-reference.z64` del mismo
código con `MSX_REFERENCE_DIAG=0` (sin diagnóstico) y con él. Media de las
medias por ventana de 300 fotogramas (sin la de arranque), µs:

| Fase | Sin diagnóstico (58 ventanas) | Con diagnóstico (47 ventanas) |
| --- | ---: | ---: |
| `sim` (lógica) | 2890 | 2929 (+39) |
| `prep` (incluye `msx_diag_game`) | 1923 | 2033 (+110) |
| `build` (render) | 6869 | 6053 |
| `cpu` | 11683 | 11016 |

El coste propio es ≈0,15 ms por fotograma (≈0,9 % de 16,7 ms): el recorrido
de los pools del mundo y la comparación de contadores. `build` y `cpu`
varían más entre recorridos (ruta del piloto, ritmo de ares) que esa cifra.
Imagen: +11,5 KB (código y anillo de 1,5 KB). La ROM normal no lleva nada.

**Lo que mide (recorrido con diagnóstico, 208 ranuras, antes de recuperar
la RAM):**

1. **Montón.** Pico usado en juego 5891 KB; **mínimo libre 238 KB**, por
   debajo del margen de 256 KB, en la zona del jefe. El juego crece ≈263 KB
   sobre los 512 KB libres al cargar (reserva 320). Pico de arranque 5628
   KB. Tras la recuperación (217 ranuras, recorrido PASS) el pico usado es 6036 KB y el mínimo libre **234 KB** al final del jefe: las ranuras ganadas salen del mismo montón, así que el crecimiento en juego (≈263 KB, reserva 320) es ahora la cota.
2. **Pools.** Actores genéricos con pico **91/96** (95 %) antes del jefe
   (cámara ≈3192–3376): el siguiente en rechazar. Balas de pistola **8/8
   con 5 rechazos** (ráfagas de disparo). Lista de nodos 92/160, nodos del
   jefe 45/96, dibujos 61/160; el resto por debajo del 25 %. Ninguna parada
   explícita salvo 18 apariciones de familias sin portar (`02A3CE`).
3. **Fotogramas lentos: render, no lógica.** Los cinco peores son de
   render con descodificación síncrona: 44,6 ms en 1920 (render 40,0 ms,
   `asset` 26,1 ms), 42,7 ms en 3030 (34,7 / 25,3), 40,9 ms en 2336 (lógica
   9,5, render 27,4), 40,8 ms en 3041, 38,8 ms en 3432. Lógica máxima 15,5
   ms (dos pasos en 2336).

## Título sin saltos y margen del montón en juego (rama `work-perf21`, base `1afa21a`)

### Salto del título en la ROM 21 (B21)

`test_logo_ares.py` fallaba con la ROM de `/home/user/ws-logo`:
`MSX TITLE miss from=2 to=4 audio_us=2878`, `dropped=1`, `late=1`, y justo
antes `MSX CINEMATIC rom:/howto.msxc step past the blank margin by 2989 us
(loader 0 producer 0)`. El mismo `1afa21a` compilado limpio pasaba, así que
no hay un commit culpable entre `ab76871` y `1afa21a`: la ROM se reconstruyó
byte a byte en `/home/user/ws-world` y la diferencia eran **diez objetos
fríos (`neo_hud`, `marco_*`, `neo_vslot`…) aún compilados -O2** de antes de
`38ceb91` (make no ve un cambio de opciones; +4,5 KB de imagen) y
`mission3.pack`. Con ellos el logo gasta ≈0,7 ms más por fotograma, el
título no termina de cargarse bajo el logo (`loaded_before=374` de 601) y
su trabajo libre sigue durante el título.

Lo que pasaba el blanco era el paso `L_TABLE` del cargador de HOW TO PLAY:
la comprobación de sus tablas (2691 entradas, v2) y la reserva del anillo
se hacían de una vez, sin plazo: **3,4 ms** medidos (título 0,9 ms).

Corrección:

- `cinematic.c`: las tablas se comprueban en trozos de 256 entradas con
  coste aprendido (`check_part`, `cost_check`) y la reserva del anillo y los
  lienzos solo empieza con ≥ 700 µs antes del plazo. Reserva medida tras el
  cambio: 100–560 µs. `ensure()` (sin plazo) lo sigue haciendo todo junto.
- `Makefile`: `build/cold-objs.flags` guarda la lista -Os; los objetos del
  mundo, cinemáticas, título y logo dependen de él, así que cambiar la lista
  recompila (la siguiente compilación en `/home/user/ws-logo` rehace los diez
  objetos).

Con la ROM idéntica a la que fallaba más la corrección: 2/2 PASS, 0
saltados, `late=0` en todas las películas; ROM normal limpia: PASS.

### Montón en juego (B22)

`MSX_DIAG` (ROM de referencia) daba 234 KB libres al final del jefe de la
M1, bajo el margen de 256. La nueva cuenta por partes de `MSX actor_init`
(`heap +N KB (scene … renderer … hud …)`) mostró por qué: la reserva de
`actor_init` (640 KB, de cuando medía 560) no incluía la **tira del HUD**
(`hud_renderer.c`, 304×224 RGBA16 = 133 KB). Total real 719 KB (Marco y
`fixed.bin` precargados 559, renderer 28, HUD 133): el juego empezaba con
510 KB libres en vez de 576 (320 de crecimiento + 256 de margen). El
crecimiento en juego (≈263 KB) es casi todo bloques grabados del fondo, que
se grababan mientras quedaran 192 KB.

Correcciones:

- Tira del HUD en **CI8** (índice = paleta << 4 | pluma, las 256 entradas de
  la capa fija como TLUT; copia con TLUT): 67 KB. Píxeles del HUD iguales en
  la captura de `test_logo_ares.py`.
- `SCENE_RESERVE_GAME_KB` 640 → **672** (medido 654).
- `BG_RECORD_MIN_FREE` 192 → **288 KB**: margen + un bloque (`bgmax`, nuevo
  en `MSX RDP` con `MSX_DIAG`: 23 KB el mayor en la M1).

| Recorrido (ares, `MSX_DIAG`) | Antes | Después |
| --- | ---: | ---: |
| M1 referencia: ranuras | 217 (ROM obsoleta) / 215 | **213** |
| M1: libre al cargar | 510 KB | 607 KB |
| M1: mínimo libre (recorrido PASS) | 234 KB | **344 KB** |
| M2 referencia (`test_mission2_ares.py` PASS): ranuras / mínimo libre | 215 / — | **213 / 283 KB** |
| ROM normal: ranuras | 216 | **214** (libre 608 KB) |

Paso intermedio medido: reserva 736 con la tira RGBA16 daba 209 ranuras y
342 KB de mínimo; la tira CI8 devuelve 4 ranuras.

### Balas de la pistola (B23)

El pool de 8 rechazaba disparos en ráfagas. En el arcade la pistola no
tiene tope propio: `0C0D64` concede si `10C71A < 10C71C` y `10C71C` es
`0C08A8[0]` = `FF` (sin límite); la bala `01B782` sale del pool de objetos
con `00411C`. A 6 px por tick y una pulsación cada 4 ticks caben hasta 10 en
pantalla. `MSX_BULLETS` = 16: pico **10/16** en el recorrido M1 y **14/16**
en la M2 de referencia, 0 rechazos (antes 8/8 con 3–5).

## Fondo de la Misión 2: listas por capa y subidas de página directas (B47, rama `work-m3step3`)

**Síntoma (B47).** En la M2 el juego se aceleraba a ratos: `MSX multi-step`
(línea nueva de `msx_perf.c`: cámara y coste del fotograma anterior a cada
fotograma de dos pasos) señalaba al **fondo**: `bg` 12,6–18,9 ms con las dos
capas de la etapa 2 o la 3 a la vista, no la simulación.

**Medida (línea `MSX BG`, ares, `MSX_DIAG`).** Con el tiempo de las
subidas de textura y de los rectángulos contado aparte, el coste del fondo
no estaba en los comandos RDP sino en el **recorrido del índice de celdas**:
la capa 2 de la etapa 1 tardaba 3,2 ms por fotograma de los que 1,2 ms eran
`rdpq_tex_upload_sub` + `rdpq_texture_rectangle` (187 dibujos, 13
subidas); el resto, visitar 280 celdas (lecturas aleatorias de 8 bytes, su
imagen, la fusión de columnas de `column_run`) y las dos búsquedas binarias
de cada *run* (~2 µs por celda visitada; la capa 3 de la etapa 3, con 0
dibujos y 1941 celdas visitadas, costaba 1,9 ms).

**Corrección.**

- `scene_strips.c` (nuevo, comprobable en el anfitrión): por capa, una
  **lista de dibujo** (*strip*) con las columnas ya fusionadas (posición,
  imagen, celdas) de una ventana 96 px mayor que la pantalla por cada lado,
  agrupadas como los *runs* del índice (x no decrece dentro de un grupo).
  Cada fotograma descarta un grupo por su rango de x o encuentra sus
  elementos visibles con una búsqueda binaria sobre un vector compacto
  (8 bytes por elemento). La lista se reconstruye cuando la cámara sale de
  su ventana (cada ~96 px de desplazamiento de esa capa), al cambiar el
  estado de reemplazo (`collapsed`, `patched`, parches de mapa) o la etapa;
  una capa por fotograma se reconstruye por adelantado (mitad del margen)
  para que las capas 2 y 3, que se desplazan juntas, no coincidan. *Pool*
  de 2048 elementos y 512 grupos (20 KB) con compactación, reservado con
  las tablas de la etapa cargada (`scene_stage_t`, ranuras de la caché:
  +2 ranuras de tablas por etapa), no en la imagen del programa, que con
  las familias de la Misión 3 ya dejaba el presupuesto en el suelo (3314 KB
  con el *pool* estático: `RAM budget` falla por debajo de 3332); la Misión
  1, sin etapa, sigue con el recorrido directo. Si una capa no cabe ni sin
  margen, esa capa dibuja con el recorrido directo (`draw_layer_direct`,
  el código anterior, que sigue siendo la referencia).
- **Subidas de página directas** (`page_upload`): los mismos comandos que
  `rdpq_tex_upload_sub` emite para las tres superficies fijas (64×64 CI4,
  32×32 RGBA16, 16×256 CI4: `SET_TEXTURE_IMAGE`, dos `SET_TILE`,
  `LOAD_BLOCK`, `SET_TILE_SIZE`), sin su preparación por llamada y sin
  repetir los descriptores de *tile* mientras formato y ranura de paleta
  son los de la subida anterior.
- Torre de la etapa 3 (`draw_tower`): la lista ordenada (`qsort` de hasta
  300 celdas por fotograma) se conserva mientras no cambien las piezas que
  llegan a la pantalla, la fase de vídeo ni la cámara horizontal.

**Comprobaciones.** `tools/test_scene_strips.c` (`make check`): para el
mapa de la Misión 1 (todas las cámaras) y 200 mapas aleatorios al estilo de
los packs (páginas altas, columnas de códigos consecutivos, celdas animadas,
columnas reemplazadas) los elementos de la lista dibujan exactamente los
texels del recorrido directo bajo cada celda en pantalla, en todas las
fases. En ares, compilación local `MSX_EXTRA_CFLAGS=-DMSX_BG_CRC=1`: cada 97
fotogramas se dibujan las dos pasadas con la lista + subidas directas y con
el recorrido + `rdpq_tex_upload_sub`, y se compara el *hash* del
framebuffer: **312 comparaciones iguales** en el recorrido completo de la M2
(`test_mission2_ares.py` PASS).

**Resultado (ares 146, `MSX PERF` por ventanas de 300 fotogramas; `multi`
= fotogramas de dos pasos por ventana, `bg` media/máximo).**

| Tramo | Antes | Después |
| --- | --- | --- |
| Etapa 1 (capas 2+3) | `multi` 121–171, `bg` 4,8–6,3 / 15–26 ms | `multi` 0–44, `bg` 0,9–2,9 / 6–14 ms |
| Etapa 2 (capa 3, propias) | `multi` 5–30, `bg` 1,2–2,5 / 8–11 ms | `multi` 0–10, `bg` 0,8–2 / 1–7 ms |
| Etapa 3, arranque (capas 0+3 densas) | `multi` 272–300, `cpu` 26 ms media, `bg` 10–13 / 21–24 ms | `multi` 159–217, `cpu` 16–19 ms media, `bg` 4,3–5,6 / 12–22 ms |
| Etapa 3, torre | `bg` 3,1 / 22 ms | `bg` 1,4 / 14 ms |

Por capa (línea `MSX BG`, medias por fotograma): etapa 1, capa 2 3,2 → 2,1
ms (187 dibujos, 15 subidas), capa 3 3,6 → 2,0 ms; etapa 3, capa 0 7,6 → 2,0–2,5
ms (183–208 dibujos). Lo que queda es casi todo el coste de los comandos
RDP (`upload` 0,3–0,5 ms, `rect` 1,1–1,4 ms) más la espera del RSP/RDP
dentro de ellos. Las reconstrucciones de las listas cuestan 2–6 ms cada una
(una capa por fotograma, 6–9 por ventana de 300 en la etapa 3). El arranque
de la etapa 3 sigue en dos pasos: `sim` 7,4 ms de media (dos pasos),
`prep` 2,5 ms, actores 2 ms y el fondo 4–6 ms.

Probado y descartado: el **modo copia** del RDP para las capas 1:1
(`rdpq_set_mode_copy`, 4 píxeles por ciclo). En ares no cambia el coste
por capa (2,2 ms con copia, 2,1 sin) y la comparación `MSX_BG_CRC` da
píxeles distintos en 18 de 26 fotogramas (solo RGB, aun con el *scissor*
reasignado tras cada cambio de modo, que el RSP corrige según el modo);
queda como opción para hardware real si se identifica la diferencia.

Ares añade su propia espera de pantalla (`wait` 6–16 ms), así que la
medida real sigue siendo la del usuario.

## Fondos pre-renderizados (prototipo) (rama `work-prebg`)

*Estado: en producción (ver «En producción» al final de esta sección).*

**Pregunta.** Con la RAM en el suelo (208 ranuras, 486–496 KB libres tras
cargar), ¿libera RAM sustituir las celdas del fondo (mapa entero fijado en la
caché de escena) por las capas de cada etapa pre-renderizadas y leídas del
cartucho por columnas según avanza la cámara?

**Estimación** (`tools/prerender_stage.py estimate romfs/mission2.pack
romfs/mission3.pack`; «fijado»: fragmentos de 16 KiB del mapa que la etapa
fija hoy; «resto»: los que siguen haciendo falta para las celdas animadas
(LSPC) y las de parches, que comparten fragmentos con las fijas; «denso»: esas
mismas imágenes reempaquetadas sin huecos; ROM: celdas estáticas sin la
deduplicación de páginas; anillo: 26–29 columnas de 16 px por capa,
304 px + 64 px de margen a cada lado, o una ventana 2-D de 352 px de alto en
las capas altas):

| Etapa | Fijado hoy | Resto / denso | ROM CI4 (CI8) | Anillo | Balance |
| --- | ---: | ---: | ---: | ---: | --- |
| M2 e1 (4 capas, 138 celdas animadas, parches) | 14 (224 KB) | 14 / 10 | 113 KB (223) | 114–135 KB | pierde |
| M2 e2 (capas 2+3, sin animadas) | 25 (400 KB) | 5 / 1 | 499 KB (986) | 139–175 KB | **gana 9 ranuras (medido)** |
| M2 e3 (torre, 559 animadas en la capa 3) | 54 (864 KB) | 35 / 32 | 1060 KB (2106) | 168 KB (2-D) | gana ≈ 8 ranuras con anillo 2-D |
| M3 e1 (1232 animadas en la capa 3) | 44 (704 KB) | 28 / 25 | 758 KB (1479) | 158 KB | gana ≈ 6 ranuras |

CI8 por capa duplica la ROM y, con la TLUT de 256 entradas, solo deja 2 KB de
TMEM (16×128 texeles por subida): CI4 con una paleta por *run* conserva los
mismos texeles y las mismas subidas de 2048 bytes que hoy. Crecimiento del
cartucho para las misiones 2 y 3 en CI4: ≈ 2,4 MB (sin comprimir; gzip deja
M2 e2 en 193 KB de 501).

**Lo que el pre-renderizado no cubre directamente.**
- *Celdas animadas (auto-animación del LSPC):* fuera de las columnas, por el
  camino de las celdas; sus fragmentos siguen fijados (en M2 e1, e3 y M3 son la
  mayoría). Reempaquetarlas densas (columna «denso») apenas reduce: hay 8
  imágenes por celda. Alternativa: una variante de columna por fase (×8 en ROM
  de esas columnas) o dejarlas en la caché.
- *Paletas y fundidos:* sin cambio (las columnas guardan índices CI4 y el id de
  paleta; la TLUT se sube con `scene_render_fade_palette`).
- *Parches del mapa (puertas, roca, capas de la M3), derrumbe, tienda:* las
  celdas sustitutas siguen por el camino de las celdas; en las columnas, una
  columna que un parche activo puede tapar se dibuja celda a celda con el mismo
  `hidden()` (probado en la e2 de la M2).
- *Desplazamiento vertical (torre de la M2 e3, M3):* las columnas enteras de la
  e3 ocupan 23 KB (capa 0) y 12,6 KB (capa 3) cada una: el anillo de columnas
  llegaría a 1,1 MB; hace falta un anillo 2-D de bloques (≈ 84 KB por capa).
- *Modo de profundidad de la M1:* las etapas de los packs usan vistas 1:1, que
  el prototipo dibuja igual; la M1 (tablas compiladas, planos escalados) queda
  fuera.
- *Tren y capas dinámicas (M3, `MSXR`):* mapas que el juego reescribe en
  tiempo de ejecución; no se pueden pre-renderizar, siguen como celdas.

**Prototipo** (M2 e2, la más sencilla: dos capas, ninguna celda animada).
`tools/prerender_stage.py render romfs/mission2.pack 2 romfs/m2s2.bgp`
(privado, `romfs/` no se versiona) escribe las celdas estáticas de cada capa
por columnas de 16 px (*runs* de celdas consecutivas con una paleta, texeles
CI4 con la misma disposición de las páginas). `src/scene_prerender.c`
(solo con `MSX_EXTRA_CFLAGS=-DMSX_BG_PRERENDER=1`; sin la marca no entra en
la imagen) lee la cabecera al cargar la etapa, reserva tablas y anillos
(32 columnas por capa) en las ranuras de la etapa en lugar de fijar sus
fragmentos estáticos, adelanta por DMA asíncrona del PI las columnas que
entran en el margen y, si una no ha llegado, la lee en el acto (contada).
`draw_layer_prerender` sube cada *run* con `page_upload` y dibuja un
rectángulo; las celdas animadas y de parche siguen por su camino.

**Medidas** (ares 146, `mslugx-m2-reference.z64` con `MSX_REFERENCE_DIAG=0`,
`tools/test_mission2_ares.py`; local: con estos datos privados ese cartucho
reserva 207 ranuras y falla el mínimo de 208 también sin el prototipo, así que
las tres compilaciones se midieron con el mínimo bajado a 200, sin publicar).
- Píxeles: con `-DMSX_BG_CRC=1` cada 97 fotogramas la pasada con columnas se
  compara con la de recorrido directo + `rdpq_tex_upload_sub`: **140
  comparaciones iguales, 0 distintas** (M2 e1 8, e2 132, cámara de la e2 0–2368).
- RAM, e2: fragmentos fijados del mapa 25 → 5 y tablas 22 → 33 ranuras
  (+11 del anillo, 175 KB): **47 → 38 ranuras (−144 KB)** para la caché de
  actores; carga de la etapa 328 → 131 ms; lecturas síncronas 0 (232
  columnas leídas por adelantado en el recorrido). e1: 14 fijados siguen
  (animadas y parches) + 8 ranuras de anillo: **pierde 128 KB**; en la
  versión sin prototipo se descarta.
- CPU, e2 (`MSX BG`, media por fotograma en ventanas de 300): capa 2
  217 → 79 µs, capa 3 486 → 278 µs (menos rectángulos: *runs* de hasta 16
  celdas por paleta, sin recorrer índice ni listas); `bg` 1917 → 1746 µs,
  `cpu` 8523 → 8294 µs, `multi` 17 → 12.

**Recomendación: parcial.** Solo capas sin celdas animadas y que no se
reescriben: M2 e2 (−9 ranuras, ≈ 0,5 MB de ROM) y, con un anillo 2-D, la
capa 0 de la torre de la M2 e3 y la capa 1 de la M3 e1 (≈ −8 y −6 ranuras,
1,1 y 0,5 MB de ROM). No compensa en la M2 e1 ni en capas dominadas por
celdas animadas (capa 3 de la M2 e3 y de la M3), y la migración completa
liberaría ≈ 8 ranuras en la etapa peor (la e3 de la M2 fija el
suelo de la misión): ayuda, pero no saca la RAM del suelo por sí sola.

**En producción** (decisión del usuario: solo donde ahorra). `MSX_BG_PRERENDER`
vale 1 por defecto. El Makefile genera `build/bgp/` desde los packs privados
(`tools/prerender_stage.py build`, `PRERENDER_SPECS`: M2 etapa 2 y todas las
de la M3) y solo copia al cartucho esos ficheros; sin packs no hay ninguno.
`build` elige por etapa, de forma voraz, las capas cuyas columnas liberan más
fragmentos de los que cuesta su anillo: M2 e2 capas 2+3 (175 KB), M3 e1 capa 1
(90 KB; la 3, con 1232 celdas animadas, no ahorra). M2 e1 y e3 se quedan con
las teselas (en la e3, con columnas enteras, el anillo no compensa; el
anillo 2-D queda pendiente). La M1 no cambia (no tiene etapa).

Ranuras de los cartuchos de referencia: con los datos de la Misión 3 paso 5
la M2 y la M3 se quedaban en 207 (3313 KB de 3332). Ahora se piden ≥ 210. Para
llegar, `10A3` (voz del derrumbe de la cúpula y del destello del cohete,
43 KB, una vez por partida) y `1034` (destrucción del tanque, el Slugnoid y
los vehículos de la M3, 34 KB, una vez por vehículo) se leen de `rom:/`, como
los demás efectos raros (`game_audio.c`): **211 ranuras** (3376 KB) en las
dos.

Medidas (ares 146, referencias con `MSX_DIAG`):
- Píxeles con `-DMSX_BG_CRC=1`: en la M2 e2, 130 iguales y 0 distintas
  (cámara 0–2368); en la M3 e1, 236 iguales y 0 distintas (cámara 0–4393,
  `test_mission3_ares.py` PASS).
- M2 e2: mapa fijado 25 → 5 fragmentos, 0 lecturas síncronas.
- M3 e1: mapa fijado 44 → 31 fragmentos, a cambio de 6 ranuras de anillo.

## Caché de instrucciones: el logo tras la fusión de ccr (12-XI, rama `work-m5`)

**Síntoma.** Tras fusionar ccr `6e0e2d9` en `work-m5` (`2cb2aa0`), `tools/test_logo_ares.py` fallaba siempre igual:
`late=211`, `cpu_mean_us=11986` (cartucho de `ws-m4`: 7310), 335 fotogramas en 9,15 s. Antes de la fusión, PASS.

**Bisección (una hipótesis cada vez, mismo código, solo se mueve la imagen).**

| Variante sobre `2cb2aa0` | qué se mueve | `cpu_mean_us` | `late` |
| --- | --- | --- | --- |
| tal cual | — | 11986 | 211 |
| 0x32E0 bytes de `.text` entre el juego y libdragon (libdragon ≡ `ws-m4` módulo 16 KB) | código de libdragon y datos | 7148 | 0 |
| 0x32E0 bytes en `keep.text` (tras todo el código) | solo datos | 11625 | 0 |
| 0x400 bytes entre el juego y libdragon | código de libdragon y datos | 7215 | 0 |

(Las variantes con relleno no caben en el presupuesto de RAM de la escena: solo valen para el logo.) Es la caché de
instrucciones (16 KB, mapeo directo, líneas de 32 bytes). Por vértice el logo ejecuta `project`, `metal`, `gradient`,
`draw_cap`/`draw_sides` y `rdpq_triangle_rsp` de libdragon, que llama a `floor` y `floorf` de libm. libm se compila sin
`-ffunction-sections`: su `.text` simple va al principio del programa (0x80000800), mientras libdragon va detrás del
código del juego. Con el tamaño del juego de `2cb2aa0`, `floorf` (0x80000BB0) y `rdpq_triangle_rsp` (0x801349C0)
compartían 9 conjuntos de la caché y se expulsaban en cada vértice. 0x400 bytes bastan para separarlos.

Quedaba un segundo efecto, en la caché de datos (8 KB, mapeo directo): con el arreglo de abajo `cpu_mean_us` era 8744,
y 7103 con 0x400 bytes solo en los datos. `TRIFMT_ZBUF_SHADE` (`.rodata` de libdragon, 0x801D5D48) caía en las mismas
líneas que el marco de pila de `rdpq_triangle_rsp` (0x807FFCD8..0x807FFDC0, fijo en lo alto de la RAM), y se lee en
cada triángulo.

**Corrección.**
- `floor` y `floorf` se sacan de `libm.a` del propio toolchain y se enlazan con su sección renombrada a
  `keep.text.libm_*` (`LIBM_KEEP_OBJS` en el Makefile; mismo código máquina, solo cambia la sección), primeros del
  bloque `keep.text`, que el guion de enlace pone detrás de todo.
- Todo `src/neogeo_logo.c` va a `keep.text.neogeo_logo`. Así el camino por vértice entero (logo, `floor`, `floorf`,
  `rdpq_triangle_rsp`) queda a una distancia fija entre sí: ya no depende del tamaño del código del juego.
- Las dos copias de color por arista son asignaciones (antes `memcpy`, que está al principio del programa).
- El logo pasa a `rdpq_triangle` una copia de `TRIFMT_ZBUF_SHADE` en su propia pila, contigua al marco de
  `rdpq_triangle_rsp`.

Resultado en ares (`--skip title,entry`): 6911, 6996, 6958 y 6938 µs con el cartucho tal cual, con 0x400 bytes solo
en los datos, y con 0x400 y 0xA60 bytes antes de libdragon. `late=0` en las cuatro. La prueba completa del logo da
PASS con 6876 µs (`late=0`).

**Lo que lo detecta.** `tools/check_icache.py` se ejecuta tras enlazar `mslugx-explorer` y `mslugx-m1end` (regla
`$(BUILD_DIR)/%.icache`, de la que depende cada `.z64`). Falla si dos funciones del camino por vértice ocupan el mismo
conjunto de la caché de instrucciones en líneas distintas, o si llaman a código de fuera del camino (permitidos:
`rspq_next_buffer`, una vez por búfer, y el `memset` de entrada de `draw_sides`, una vez por fotograma, que pone
`-ftrivial-auto-var-init=pattern`). Con el ELF de `2cb2aa0` informa de las 9 líneas compartidas entre
`rdpq_triangle_rsp` y `floorf`.
