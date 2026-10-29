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
