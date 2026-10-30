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
- Con el Expansion Pak todo el pack (100 bloques) queda residente tras el
  arranque; aun así el prefetch hacía ~900 peticiones nulas por fotograma.

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
- Con el parche 1 y la **misma** espera: EXP2_PENDIENTE
- Recorrido completo con la librería final (sin espera): FINAL_PENDIENTE
