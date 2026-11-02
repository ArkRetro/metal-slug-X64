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
  - Cada misión tiene su `romfs/missionN.pack` (formato v3).

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

## Instrumentación existente

- **Registro de la ROM de referencia:**
  - `PERF`: peor fotograma y fallos de la caché.
  - `MSX FRAMES`: fotogramas tarde por etapa.
  - Contadores de paradas: `unsupported`, `weapon_stops`,
    `neo_soldier_death_stops` y `boss_unsupported`.
  - Rescates y muertes por caída, con x, y y la cámara.
- Las paradas explícitas se cuentan y se registran con su rutina 68K. Nunca
  se ocultan devolviendo éxito.
- **Falta:** un modo de depuración en la ROM normal con memoria máxima, pools
  y anillo de eventos (ver `NEXT_STEPS.md`).

## Distinción de "memoria"

- **Contexto de la conversación:** se resuelve con estos documentos.
- **RAM del host:** el disco y los procesos de MAME/ares en el contenedor.
  El disco tiene unos 2 GB libres; hay que borrar las capturas después de
  usarlas.
- **RAM de la N64:** 8 MiB. Margen de montón de 256 KB, caché de escena de
  208 o más ranuras.
