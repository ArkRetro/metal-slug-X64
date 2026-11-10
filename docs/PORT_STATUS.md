# Estado del port (resumen técnico para retomar)

Detalle de contenido: `ESTADO.md`. Memoria: `MEMORY_OPTIMIZATION.md`,
`PROJECT_STATE.md`. Fallos: `BUG_MATRIX.md`. Siguiente acción: `NEXT_STEPS.md`.

## Arquitectura

- **Enfoque:** traducción nativa a C del código 68000 de Metal Slug X (Neo
  Geo). No es emulación ni recompilación: cada rutina se identifica por su
  dirección en la ROM 68K y se traduce a mano.
- **Comprobación:** cada rutina se valida bit a bit contra capturas Lua de
  MAME 0.264. Las capturas por familia de actores van a fixtures en
  `data/*_fixture.json.gz` y las prueba `tools/test_neo_family.c`.
- **MAME:** solo sirve de referencia y de herramienta de captura. No hay
  código de MAME en `src/`. La lógica del juego está en el programa 68K y se
  traduce. El driver de MAME solo aporta el hardware: LSPC, Z80/YM2610 y la
  BIOS.
- **Lógica:**
  - `src/gameplay.c` lleva a Marco: armas, formas, suelo y paredes.
  - `src/neo_world.c` lleva la lista de nodos de actores y su intérprete de
    rutinas (`ga_*`), con contadores de rutinas no soportadas.
  - Los actores viven en `src/neo_*.c`.
  - El flujo de misión está en `src/neo_mission.c`.
  - Las colisiones están en `src/neo_collision.c`: anillo de 64×64 y barrido
    del núcleo 013968.
- **Animación:**
  - Programas de sprites del original, interpretados por `actor.c`.
  - Para Marco, catálogo de poses capturado: `marco_visual.c` y
    `marco_renderer.c`.
- **Render:** libdragon (commit e356bf3 más parches), con RDP y bloques de
  rspq. El fondo usa una caché de escena de 16 KiB por ranura. Las capas se
  ordenan por la prioridad +1E.
- **Audio:**
  - La música y los efectos son wav64 renderizados de la ROM del usuario.
  - Los efectos están residentes en RAM.
  - `game_audio.c` mapea las palabras de sonido del 68K mediante
    `data/sound_map.json`.
- **Datos:**
  - Se extraen de la ROM del usuario con las herramientas de `tools/`.
  - Son privados y no van en git: `filesystem/`, `romfs/`, `build/marco_*.h`.
  - Cada misión tiene su `romfs/missionN.pack` (formato v5: fragmentos LZ4
    y páginas de actores compartidas entre misiones, `docs/COMPRESION.md`;
    se reconstruyen con `tools/build_packs.py`).

## Compilación y pruebas

- **Toolchain:** `scratchpad/env.sh` (N64_INST), gcc MIPS de libdragon y
  `make`.
- **Puertas antes de fusionar:**
  - `make check`: pruebas en el PC, familias, determinismo -O0/-O2 y
    `missing_draws`.
  - `make check-core` en un clon limpio.
  - `make -j4`, que genera la ROM normal `mslugx-explorer.z64`.
  - `make mslugx-reference.z64`: invulnerable y con piloto automático.
  - `make mslugx-m2-reference.z64`.
- **Pruebas en ares 146:**
  - `tools/test_logo_ares.py`: arranque, con fotogramas tarde por película.
  - Recorrido de la Misión 1 (`tools/test_mission_native_ares.py`).
  - `tools/test_mission2_ares.py --stage-seconds 800`.
- **Hardware real:** el usuario prueba en MiSTer N64.
- **Expansion Pak (8 MiB):** obligatorio, y todas sus funciones se conservan.

## Última compilación funcional

- **Commit:** `5b49ffb` (rama `ccr-09eb2f6b-8rwl8l`). Es la ROM de prueba 20.
- **Puertas:** pasan todas.
- **Recorrido de la Misión 1 en ares:** PASS, crash=0, máximo de 1 fallo de
  audio.
- **Punto de recuperación:** etiqueta local `base-rom20`.

## Instrumentación

- **Registro de la ROM de referencia:**
  - `PERF`: peor fotograma y fallos de la caché.
  - `MSX FRAMES`: fotogramas tarde por etapa.
  - Contadores de paradas: `unsupported`, `weapon_stops`,
    `neo_soldier_death_stops` y `boss_unsupported`.
  - Rescates y muertes por caída, con x, y y la cámara.
  - `MSX respawn`: cada vida nueva con su entrada de la tabla de la etapa,
    tipo (0/4/5), x/y y si quedó en el suelo (B33).
  - `MSX AUDIO voice`: las voces del jugador (muerte `1123`, armas
    `0C08F8`, bombas `1124`) con el veredicto de la política (B30/B34) y
    `MSX AUDIO not resident` por cada efecto que no cabe en `ram:/`.
- Las paradas explícitas se cuentan y se registran con su rutina 68K. Nunca
  se ocultan devolviendo éxito.
- **Modo de diagnóstico (`MSX_DIAG`, `src/msx_diag*.{c,h}`):** activo en las
  ROM de referencia (M1/M2/M3) y en `mslugx-diag.z64` (el juego normal con
  diagnóstico); apagado en `mslugx-explorer.z64`, donde cada gancho es una
  función vacía en línea (ningún símbolo `msx_diag` en el ELF).
  `make MSX_REFERENCE_DIAG=0 mslugx-reference.z64` enlaza la referencia sin él.
  - Cada 300 fotogramas, línea `MSX DIAG`: lógica (`sim`) y render (`build`)
    por fotograma (media/máx.), fotogramas ≥25 ms, montón usado/pico en
    juego/pico de arranque/mínimo libre/arena (muestra cada 30 fotogramas y
    en cada `MSX MEM`), ranuras de la caché de escena usadas/total/pico.
  - Línea `MSX DIAG pools`: por pool `usado/pico/capacidad` y `!fallos`
    (asignaciones rechazadas) de la lista de nodos, soldados, lanzamientos,
    salpicaduras, actores genéricos, nodos del jefe, dibujos, cajas,
    registro 004C24, sonidos, disparos, subguiones, sólidos, objetos
    dinámicos y balas de pistola.
  - Cada 30 s: `MSX DIAG stops` (paradas explícitas unificadas con la
    rutina: mundo, jefe, muerte de soldado, arma, granada, montura,
    vehículo, objeto, HUD, guion detenido/saltado, familias sin portar,
    cámara, Marco, 004E76) y `MSX DIAG slowest` (5 peores fotogramas con
    lógica/render/asset y cámara).
  - Anillo fijo de 128 eventos (etapa, flujo, reinicio, apariciones del
    guion, soldados, bajas, muerte, arma, montura, jefe, paradas, rechazos
    de pool, fotogramas lentos). Se vuelca (`MSX DIAG dump`) en una
    aserción (`--wrap=debug_assert_func_f` y `assert`), una excepción, la
    primera parada de cada tipo y con C-izquierda + C-derecha. Como mucho
    16 volcados por sesión, salvo los fatales.
  - C-arriba + C-abajo selecciona el siguiente nodo de la lista y registra
    su tipo, rutina, posición y estado.
  - `MSX_DIAG_CHECK`: comprobaciones baratas de índices de la lista y de
    los pools en `neo_world.c`; solo en diagnóstico.
  - No cambia la simulación: `make check` compila la prueba de juego con
    `MSX_DIAG=1` y exige la misma salida. Prueba del anillo:
    `tools/test_msx_diag.c` (`make check-core`). Coste medido en
    `RENDIMIENTO.md` («Modo de diagnóstico»).

## Modo opcional «Barra de vida» (no arcade, B42)

- Por defecto el juego es el arcade (un golpe mata) y ningún camino del modo
  corre (`msx_game_t.lifebar` = falso). Se activa en SOLDIER SELECT
  manteniendo Z al confirmar a Marco con A/Start (`main.c`; registro
  `MSX LIFEBAR on`). Marco aguanta 10 golpes: cada golpe que lo mataría
  (`msx_game_hurt`) quita uno y da la protección con parpadeo de una vida
  nueva (`2*0x78` actualizaciones). Matan igual que en el arcade: los tipos
  8 y 0x12 (muertes propias `0C2ACC`/`0C2A70`), las caídas, el fin de tiempo
  y el vehículo destruido. Con 0 muere normal (vidas y continuación del
  arcade); cada vida nueva vuelve a 10. El HUD muestra la vida en el
  indicador `0C9B94` a pie (vida/10); montado, el del vehículo. La comida
  (objetos con el byte 3 del registro, tabla en ARMAS_VEHICULOS.md) devuelve
  max(1, puntos/100) golpes hasta 10; la puntuación es la del arcade.
  Prueba: `test_gameplay lifebar` (misma partida igual hasta el golpe con el
  modo apagado y encendido; reglas del modo).

## Menú de pruebas (no arcade)

- En SOLDIER SELECT, mantener L+R (o Z+R) y pulsar A/Start abre MISSION SELECT (`main.c`): MISSION 1, MISSION 2 STAGE 1/2/3, MISSION 3; cruceta mueve, A/Start empieza, B vuelve, Z alterna la barra de vida (combina con B42; sin menú sigue siendo Z al confirmar).
- Misión N>1: se saltan las películas de M1 y el primer fotograma entra por el camino del cambio de misión (`actor.c` `selected_mission`: 42F4/42F8, registro 0 con 1805C, pack, fundido, MISSION n START, paracaídas en M3; etapas previas cerradas con 018F40/018722). Registro `MSX MSELECT mission=N stage=S`.
- Sin el combo nada cambia; R no reinicia hasta soltarlo. Comprobado en ares (cartucho normal): M2 E1, M2 E3 y M3 llegan a jugar.

## Distinción de "memoria"

- **Contexto de la conversación:** se resuelve con estos documentos.
- **RAM del host:** el disco y los procesos de MAME/ares en el contenedor.
  El disco tiene unos 2 GB libres; hay que borrar las capturas después de
  usarlas.
- **RAM de la N64:** 8 MiB. Margen de montón de 256 KB, caché de escena de
  208 o más ranuras (213 en las ROM de referencia y 214 en la normal en `work-perf21`; mínimo libre en juego 344 KB en la M1 y 283 KB en la M2).

## Vídeo de referencia del usuario (privado, no en git)

Partida completa del arcade, 44 min, 640×360, 30 fps:
`/home/user/ref-video/videoplayback.mp4`. Índice de fotogramas cada 15 s en
`/home/user/ref-video/idx/tNNNN.jpg` (segundo = (NNNN−1)×15). Uso: localizar
visualmente un momento que el usuario cite por minuto; la prueba exacta sigue
siendo la captura de MAME de la ROM del usuario. Fotograma suelto:
`ffmpeg -ss MM:SS -i videoplayback.mp4 -frames:v 1 out.png`.
