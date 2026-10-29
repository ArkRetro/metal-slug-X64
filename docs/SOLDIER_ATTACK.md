# Ataque observado: animación, objeto lanzado y vuelo nativo

Continúa [SOLDIER_STATE.md](SOLDIER_STATE.md). El selector ya recuperado elige
el índice 1 y entra en ROM `0x0AF918`. El programa de animación de ese ataque
es `0x23D7EE`; ahora se convierte fuera del runtime a comandos C tipados.
Los callbacks observados tienen efectos nativos explícitos: sustitución de
recurso de paleta y creación de un actor mediante una cola diferida.
No se ejecuta código 68000 ni se invocan direcciones de ROM en Nintendo 64.

## Verificado y traducido

| ROM / RAM | Evidencia y componente nativo |
| --- | --- |
| ROM `0x040648..0x04065C` | Libera el recurso de paleta anterior y solicita `0x035C`. `NEO_ANIM_REPLACE_PALETTE` lo expresa como una operación del gestor gráfico nativo. |
| ROM `0x03BB8E..0x03BBB6` | Asigna entrada `0x03BC28`, escribe `+0x6E=0` y desplaza el nuevo objeto `(0,32)` respecto al soldado. |
| ROM `0x01ACF2..0x01AD3C` | Calcula coordenadas locales, velocidad inicial e historial respecto al padre de escena. |
| ROM `0x01AC94..0x01ACA0` | Copia únicamente los bits 0/1 de orientación del actor de origen. |
| RAM `0x1016D4` | Slot del objeto realmente asignado en esta ejecución, identificado por el callback; no se presupone una posición fija de proyectiles. |
| ROM `0x03BC52..0x03BD34` | Inicializa capa, orientación y velocidad de lanzamiento según el jugador seleccionado y una cuantización original de distancia. |
| ROM `0x03BC20` | Rectángulo de selección de jugador: `(-320,-256,320,512)`. |
| ROM `0x03BD3A..0x03BD50` | Gravedad y frenado horizontal antes del contacto. |
| ROM `0x0143E4..0x01448C` | Preparación aérea: velocidades directas y borrado independiente de los dos bits de pared. |
| ROM `0x0139B8`, `0x012E64` | Valor inicial `0x4000` del kernel y consulta de palabra de salida; en este camino sin contacto produce la etiqueta `0x28`. Se importa el dato desde `0x012E6A+0x4000`, sin atribuirle una semántica inventada. |
| ROM `0x014492..0x014520` | Commit de posición, velocidades, flags y proyección ya recuperado. |

La traza registra la creación durante el ataque en el fotograma de debugger
**2557**; el nuevo actor entra por primera vez en **2559**. El test integrado
actualiza primero la animación del soldado, encola el objeto calculado y espera
al tick posterior para inicializarlo. No usa un evento de spawn inyectado desde
el fixture en el momento de crear el objeto.

## Controlador principal del ataque

La captura adicional de `trace_soldier_attack_state.lua` verifica la entrada
ROM `0x0AF918`: inicia `0x23D7EE` a velocidad `0x0100`, copia la palabra
`actor+0x72` a `actor+0x70`, pone el escalar `+0x62` a cero y pasa a `0x0AF964`.
En esta ejecución la palabra copiada vale `0x007F`; su productor anterior sigue
fuera de este componente. No se introduce como constante de temporización.

`0x0AF964` ejecuta dos consultas distintas: `0x01436A` prueba desplazamiento
cero, hace commit/proyección y restaura las velocidades anteriores en
`0x0143A2/0x0143A6`; después `0x01456A` calcula el paso de suelo con escalar cero.
Las 40 consultas de 20 actualizaciones coinciden con los resultados de MAME.
La animación genera su propio fin y el controlador vuelve a `0x0AF380`, mediante
la asignación de `0x0AF98E`. No hay llamadas al RNG antes del bloque común
`0x0B051E` en estas 20 trazas.

La prueba integrada conserva el actor del soldado entre pasos y calcula sus
coordenadas, historial, escalar, velocidad de animación y rutina. El evento de
lanzamiento usa ese actor calculado para crear el hijo. Los límites del bloque
común siguen aportando entradas externas de control; todavía no forman una IA
cerrada para todo el encuentro. `neo_contact_stationary_flat()` solo admite
apoyo estático plano de grupo `0xC0`, con bit 5 de `flags15` desactivado y sin
candidatos dinámicos relevantes. El camino aéreo del ataque no está recuperado.

El extractor exige que las fronteras de animación de la nueva captura sean
idénticas a las del fixture independiente anterior. Conserva hashes del ROM,
la captura y el ensamblado de Ghidra. Su lenguaje disponible es el superset
`68000:BE:32:MC68020`; las instrucciones ejecutadas se contrastan con las trazas
M68000 de MAME. Ver [soldier-attack-state-validation.json](soldier-attack-state-validation.json).

## Aritmética y fidelidad

Todas las posiciones y velocidades son enteros Q16.16, con Y hacia arriba.
Se conservan las operaciones de palabra, su envoltura, las inversiones de
orientación y el desplazamiento aritmético. El vuelo observado aplica:

```
vy += ASR32((-12 << 16) - vy, 5)
vx -= ASR32(vx, 5)
```

El código C usa constantes y operaciones definidas para evitar desplazamientos
negativos o desbordamiento de enteros con signo. La selección de dos jugadores
se traduce desde los helpers `0x01AFFC/0x01AF3C/0x01AF9E`; esta captura solo
verifica P1 y `+0x6E=0`. Las ramas alternativas siguen siendo cobertura estática.

`neo_throw_spawn()` calcula el nuevo actor desde la posición e historial del
soldado y la escena. `neo_throw_initialize()` calcula la velocidad inicial;
`neo_throw_free_step()` conserva estado durante las 33 actualizaciones observadas.
El test comprueba cada posición, velocidad, flag, capa, marca e historial de
proyección, además de las fronteras de solicitud/resultado de contacto.

## Animación y adaptadores nativos

`neo_anim_step_events()` conserva la API anterior y añade eventos explícitos de
los dos callbacks recuperados. Sin un adaptador válido, el helper devuelve
`NEO_ANIM_UNSUPPORTED`; no salta callbacks desconocidos. El adaptador de paleta
resuelve un recurso a un identificador lógico del renderer. No reproduce el
asignador de bancos de paleta de Neo Geo. El test proporciona su asignación
observada para comparar los registros de dibujo originales.

`neo_soldier_animation_event()` realiza el cálculo de spawn en C y entrega un
valor de actor a la cola. La cola debe copiarlo y diferir la primera actualización.
El scheduler global de todos los actores sigue pendiente.

La prueba integrada compara **20 actualizaciones y dibujos** del ataque,
**dos eventos** y **41 actualizaciones y dibujos del actor creado**: 33 de vuelo
y ocho de la fase posterior. La paleta cambiada por el
primer evento persiste entre pasos; el cursor y temporizador también. El vuelo
parte del actor calculado por el segundo evento, sin reinyectar sus posiciones.

## Geometría de impacto recuperada

`trace_soldier_hit_query.lua` captura las 33 consultas reales del hijo a
`0x0049E8`, el banco de registro seleccionado y las ocho entradas a `0x004762`.
El candidato observado es RAM `0x1014C4`, cuyo descriptor en ROM `0x2D0AA8`
empieza por tipo `5`, flags `1` y rectángulo local `(-8,4,16,28)`.
La consulta usa ROM `0x0E0894`. No se interpreta la palabra `+0x4E` como una
salud universal: su significado depende del actor y sigue pendiente.

`neo_hit_rectangle()` traduce solamente ROM `0x004798..0x00484A`: transformación
por orientación, comparaciones sin signo de palabras, rechazo de bordes y punto
de impacto local con desplazamientos aritméticos y límites originales. Las ocho
pruebas independientes producen siete rechazos y una intersección. En la última,
los stores ejecutados `0x004816/0x00484A` contienen `(x,y)=(-2,18)` y coinciden
con C. Los dos caminos de espejo tienen análisis estático, sin cobertura dinámica
en esta captura. El helper no escribe al actor si no hay intersección.

Esta geometría todavía no sustituye las respuestas externas del controlador de
vuelo: faltan el registro de candidatos, filtros, prioridad, callbacks y efectos
de daño del resto de `0x004762`. La captura demuestra que el servicio hace más
que devolver un booleano: modifica campos del objetivo. Esos efectos no se
suprimen ni se dan por implementados. El límite de 64 entradas comparado en
`0x004C4C` corresponde al registro de impactos; no demuestra el tamaño del pool
global de actores.

## Límites materiales

El resolver de vuelo admite solo el recorrido aéreo **sin contacto de terreno**:
padre de escena estacionario, piso único, margen vertical comprobado y movimiento
horizontal de hasta cuatro píxeles por tick. El llamador debe establecer que no
hay candidatos dinámicos relevantes para el kernel. Un impacto, piso superpuesto,
pared o padre móvil devuelve `false` sin modificar el actor. No se sustituye el
kernel general por una integración libre incondicional.

La selección de impacto de región `0x0049E8` y el segundo servicio `0x004C0E`
siguen entrando desde la escena: 32 respuestas de región cero y una `5`, con
carry falso del segundo servicio en las 33 consultas. El daño no está portado
por este componente. El controlador nativo usa esas respuestas para decidir
la transición, sin introducir un cambio de fase desde el fixture. Tampoco se ha integrado este ataque en los guardias
provisionales del juego activo. La Misión 1 completa sigue pendiente.

## Fase posterior y vida de recursos

`neo_throw_actor_step()` reproduce el recorrido observado `0x03BC28` →
`0x03BD3A` → `0x03BE6C` → `0x01C9E6`, hasta la petición de borrado en 2639.
La marca de fase cambia tras el impacto, pero su inicialización ocurre en el
**tick siguiente**, como las asignaciones de rutina originales.

El programa de vuelo `0x23F3F0` conserva su animación cíclica mientras se mueve.
Tras cada dibujo, ROM `0x03BD98..0x03BDAE` selecciona la paleta mediante el
contador de vídeo: índice `phase & 3`, sustituyendo `3` por `1`. El código C
calcula esa elección; no introduce una paleta por fotograma desde la captura.

La entrada `0x03BE6C` cambia la capa a `0x8820` y emite la petición de sonido con
índice `0`. La inicialización genérica `0x01C8FC` comienza `0x22D2A6`, sustituye
los tres recursos de vuelo (`0x35D/0x364/0x365`) por `0x36C`, borra la inversión
vertical y limpia el tag de `+0x50`. Los ocho dibujos posteriores y su fin se
calculan con el mismo motor de animación nativo. `neo_contact_project()` actualiza
proyección e historial sin realizar una integración de velocidad durante esa fase.

El renderer recibe identificadores lógicos de paleta desde su adaptador. La
prueba contabiliza cuatro adquisiciones y cuatro liberaciones, sin recursos
activos después del fin. Eso valida la vida de recursos del componente, no el
asignador original de Neo Geo. El audio es una petición de evento; todavía no
se afirma reproducción de la muestra original en N64.

La comprobación de visibilidad `0x019504` utiliza rectángulos importados de ROM
`0x03BC18` y `0x01DEC0`, y se contrasta con las 40 consultas realizadas antes del
fin de animación. La última actualización termina antes de consultar visibilidad.
Esta captura solo cubre el resultado visible; la salida por culling tiene cobertura
estática. En el backend se conservan las respuestas externas de colisión y el
resolver de vuelo sigue limitado a su subconjunto sin contacto de terreno.

## Validación y reproducción

`make check`, la compilación MIPS y las pruebas internas en ares 146 validan estos
componentes. Los informes conservan hashes y límites; no son una ROM de la demo:
[soldier-throw-validation.json](soldier-throw-validation.json) y
[soldier-attack-validation.json](soldier-attack-validation.json) y
[soldier-object-validation.json](soldier-object-validation.json).

```
python3 tools/mame/run_first_soldier.py --rom-dir /ruta/roms \
  --script tools/mame/trace_soldier_attack.lua --end-frame 2700 \
  --stop-right-frame 2360 --output /ruta/captura
python3 tools/ghidra/run_first_soldier.py /ruta/captura --output /ruta/ghidra \
  --ghidra /ruta/ghidra_11.4.2_PUBLIC --jdk /ruta/jdk-21
python3 tools/extract_soldier_throw.py /ruta/captura /ruta/ghidra \
  --output data/soldier_throw_fixture.json
python3 tools/extract_soldier_attack.py /ruta/captura /ruta/ghidra \
  --output data/soldier_attack_fixture.json
make check
make throw-regression.z64 attack-regression.z64 object-regression.z64
python3 tools/test_rng_ares.py throw-regression.z64 --suite throw --output /ruta/vuelo
python3 tools/test_rng_ares.py attack-regression.z64 --suite attack --output /ruta/ataque
python3 tools/test_rng_ares.py object-regression.z64 --suite object --output /ruta/objeto
```

MAME solo ejecuta la primera acción de breakpoint que coincide en una dirección;
el script sustituye los probes heredados que se superponían, conservando los del
otro actor. Los extractores exigen todas las entradas/salidas y no completan
registros ausentes con valores supuestos.

Evidencia definitiva local: `/workspace/research/soldier-attack-debug-final` y
`/workspace/research/soldier-attack-ghidra-final`. Ghidra 11.4.2 utiliza su
superset `68000:BE:32:MC68020`; las instrucciones ejecutadas se contrastan con
MAME 0.276 M68000. Los binarios, RAM y trazas completas permanecen locales.

Para regenerar el fixture de vida completa, se usa también
`trace_soldier_throw_lifecycle.lua` con los mismos argumentos del runner y
`tools/extract_throw_lifecycle.py CAPTURA GHIDRA --output data/soldier_object_fixture.json`.
Su evidencia definitiva está en `/workspace/research/soldier-throw-lifecycle-final`
y `/workspace/research/soldier-throw-lifecycle-ghidra`.
