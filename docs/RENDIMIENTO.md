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
  (`msx_title_intro`) → entrada en paracaídas → audio del juego → caché →
  Misión 1 (`actor_begin_mission` con el traspaso medido de `entry.json`). El
  título precarga su película (3,4 MB) y 16 fotogramas preparados (2,1 MB):
  casi todo el montón, así que la caché no puede existir durante el logo ni
  las cinemáticas; se dimensiona después, con lo que queda, y la parte
  obligatoria (bancos y mapa, 50 bloques) se carga entonces (≈0,28 s); el
  resto del calentamiento sigue en el tiempo libre del juego. La preparación
  de fotogramas de la cinemática se limita además al montón libre (−256 KB).

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
