# Correcciones revisadas del PR #2

Fuente: [Revisión: posición inicial nombrada, flash del último guardia y pruebas sin SDK](https://github.com/ArkRetro/metal-slug-X64/pull/2),
commit revisado `c826597fdea29108facb28a1bd5ce9d8fdd18771`.
En esa primera revisión el PR partía de `main`, anterior a los componentes
nativos nuevos. Ahora apunta a `beta-work`; no se ha fusionado en `main`.

| Corrección | Aplicación y comprobación |
| --- | --- |
| Tercer guardia desaparecía sin parpadeo | El prototipo mantiene el temporizador de muerte de 90 ticks y permanece muerto al agotarlo. La prueba alcanza la tercera baja mediante el combate existente y comprueba que no reaparece. |
| Cadáver congelado al cerrar la región | Completar la región limpia el temporizador. Es el cierre provisional de la región, no el final real de Misión 1. |
| Posición repetida como literal | `MSX_START_X` nombra el inicio histórico del prototipo y su reaparición. La entrada en paracaídas utiliza su posición medida propia. |
| Mensaje de error «v3» | Eliminado el prefijo obsoleto del ejecutable histórico. |
| Pruebas dependían innecesariamente del SDK | Los objetivos host no incluyen `n64.mk`. La primera integración de `check-core` ejecutaba seis pruebas, incluyendo el nuevo códec cinematográfico; `check-cinematic` verifica todos los fotogramas privados del segmento. |
| CI | `.github/workflows/check.yml` ejecuta `make check-core` en pushes y PRs. No usa ROMs ni el SDK. |

`check-core` requiere los CSV numéricos versionados que usa la prueba de salto,
además de código y compilador C. Se verificó en una copia limpia con esos archivos,
sin `build/`, sin SDK y sin los assets privados de `filesystem/`.
`make check` conserva todas las pruebas adicionales de la rama y requiere sus
assets y metadatos derivados; no se ha reducido al conjunto antiguo del PR.

Las pruebas host completas y las compilaciones N64 pasan. La integración en ares
comprueba además la nueva entrada, el traspaso al jugador y sus controles. El
parpadeo sigue siendo una corrección del guardia provisional: no convierte esa
IA ni sus parámetros en comportamiento verificado de Neo Geo.

## Segunda revisión: aserciones, recursos e historial

Se han integrado en `beta-work` los commits de Claude `886e9e3` y
`6e50a1c`, descritos en su [comentario de revisión](https://github.com/ArkRetro/metal-slug-X64/pull/1#issuecomment-6462874985).
La proyección 2.5D se ejecuta fuera de `assert`, y los capturadores aceptan
`rPC` en MAME 0.264. La misma auditoría encontró otras dos operaciones con
ese problema: descomprimir un chunk del escenario y reiniciar Marco en la
posición medida de entrada. Ambas se ejecutan ahora también con `NDEBUG`.
Los fallos de descompresión/CRC siguen siendo fatales en esa configuración.

Las dos observaciones pendientes de Claude quedan resueltas así:

- **Recursos del objeto lanzado:** `UNSUPPORTED` cancela el objeto y libera su
  propietario exactamente una vez, incluso si faltan los servicios en una
  llamada posterior. No se reintenta un estado parcialmente ejecutado.
  La sustitución de paleta precede a los efectos de la explosión y un paso
  fallido no emite solicitudes de sonido. Once casos de fallo/cancelación
  verifican este contrato nativo; no son comportamiento atribuido a la ROM.
- **Token de historial:** VERIFICADO en RAM `0x10C33A`, rutina ROM `0x01AB50`:
  `move.b ($433a,A5),D0; andi.b #1,D0; addq.b #1,D0; move.b D0,($433a,A5)`.
  La escritura está en `0x01AB5A`; MAME comunica el PC posterior `0x01AB5E`.
  Con A5=`0x108000`, el destino coincide con el watchpoint. Son 180 escrituras
  medidas entre los fotogramas 2101 y 2459, alternando 1/2. El primer registro
  duplicado por los dos watchpoints se descarta explícitamente. Ghidra confirma
  las instrucciones con dirección; el trace de MAME confirma su ejecución.
  No hace falta cambiar el centinela `mark==0`: el planificador nativo deberá
  avanzar el token con `(anterior & 1) + 1` por pasada original de actores,
  no por VBlank de N64. `neo_history.h` expresa esa operación; todavía no
  sustituye al planificador global pendiente.

La evidencia numérica del historial está en `data/soldier_history_fixture.json`;
los hashes y límites de captura figuran allí. ROM, RAM y trazas completas
permanecen fuera del repositorio. Herramientas reproducibles:
`trace_projection_epoch.lua`, `extract_history_token.py` y el punto de entrada
`0x01AB50` de `AnalyzeNeoGeoSoldier.java`.

`check-core` se amplía aquí a RNG/soldado/animación/historial, 2.5D y auditorías
Python, además de las seis pruebas anteriores. Se ha ejecutado en una copia
sin SDK, build ni assets privados. `make check` pasa. Los once fallos y la
secuencia original de 41 pasos pasan con ASan/UBSan; LeakSanitizer no puede
usarse bajo el ptrace de este entorno y se desactivó en esa ejecución.

En ares 146 pasan objeto (41 dibujos, 11 fallos, una solicitud de sonido)
y ataque (20 estados, 40 consultas de suelo, 41 dibujos del objeto hijo).
Informes locales: `claude-object-cleanup-approved/validation.json` y
`claude-attack-cleanup-approved/validation.json` bajo `/workspace/emulator/`.
El nuevo par interno `scene-debug.z64`/`scene-release.z64`, ejecutado mediante
`test_release_ares.py`, compara ocho CRC de framebuffer RDP entre módulos
normales y `NDEBUG`: cámaras 0/64/320/872, modos clásico y profundidad.
No publica cartuchos ni constituye una prueba de Misión 1 completa.

## Integración del modo 2.5D de Claude

También se integran `f2f7d30` y `91c730f`: sombras de contacto, pulsos de luz,
recorte de planos y modo `depth25d` predeterminado. Los conflictos de Makefile
se resuelven conservando tanto la ampliación de `check-core` como las pruebas
con geometría real. La ruta `MSX_VISUAL_MODE=classic` sigue disponible.
El código matemático nuevo se ejecuta en host y N64; los efectos no modifican
las colisiones ni convierten las reglas provisionales en lógica recuperada.

[Informe de integración](claude-integration-validation.json): ocho imágenes
RDP coinciden entre normal/NDEBUG con luz y sombra activas; entrada de 230
fotogramas sin saltos, CRC final `db681cc3` y audio activo al traspaso. Compilan
los dos cartuchos internos y el laboratorio. En ares también pasa el
encuentro del blindado: daño, destrucción, 13 celdas de restos, subida,
reinicio y laboratorio con el nuevo modo visual. `check-core` vuelve a pasar en
una copia sin SDK/assets tras incorporar los cambios de Claude.

El [comentario de coordinación](https://github.com/ArkRetro/metal-slug-X64/pull/1#issuecomment-6463701871)
reserva a Claude la región cámara 872→1920, soldados nativos y superficies de
esa región. Esta rama conserva título/tutorial/selección, SFX, música continua
y los encuentros/jefe pendientes. Así evitamos desarrollar dos versiones de
las mismas rutinas. No se presenta como terminado ningún componente que solo
figure en ese reparto: mapa extendido y soldados aún no están integrados.


## Integración del tramo hasta cámara 1920

Revisado el nuevo head de Claude `97f8396` (commits `62c1094`, `a05a06b5`
y `294539e`). Se conserva el conjunto ampliado de pruebas del backend al resolver
el conflicto de Makefile; el presupuesto de chunks se ejecuta en `check`, donde
están disponibles los assets privados, sin añadirlos como requisito de `check-core`.

Se regeneran mapa y colisión con las capturas existentes de MAME 0.276:
2.719 celdas, 48 paletas, 280×32 celdas de colisión y 13 celdas de evento
conservadas fuera del terreno estático. Los 122 fotogramas pasan la auditoría
independiente: 8.307.712 píxeles de mapa y 4.953.328 píxeles descubiertos de MAME.
`make check` y la compilación N64 pasan.

La prueba interna de entrada recorre ahora opcionalmente el tramo completo con
`test_entry_ares.py --region`. En ares pasan los 230 fotogramas únicos de entrada,
sin omisiones, con audio activo en el traspaso, los controles, las tres bajas
provisionales, los cambios de altura/banco y el límite exacto de cámara 1920 y
X=144965631. No se registran misses ni evicciones en la caché de esa ejecución.
No se publica el ROM interno. El modelo de cámara sigue teniendo las diferencias
documentadas y no se ha completado la misión ni integrado el controlador nuevo
de Marco en la campaña; tampoco se acredita funcionamiento en 4 MiB o hardware.
