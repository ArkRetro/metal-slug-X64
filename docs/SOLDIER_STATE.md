# Estados nativos y selección de acciones del primer soldado

Este bloque continúa [SOLDIER_CONTROL.md](SOLDIER_CONTROL.md). El código C
calcula ahora las transiciones de rutina, la velocidad escalar, la velocidad
de animación y seis inicios de programa durante la secuencia observada. Esos
valores ya no se introducen desde el fixture en cada actualización.

La prueba integrada mantiene 54 ciclos de estado, 55 ciclos comunes y 54
movimientos/dibujos. Una segunda captura, manteniendo a Marco cerca del soldado
mediante el mando, aporta 84 consultas del selector de acciones: 83 resultados
cero y una selección de ataque. No equivale al primer nivel completo.

## Verificado en ROM/RAM

El slot es RAM `0x101F14`, identificado por su constructor `0x0AF17E`.
Las etiquetas ROM que conserva el código nativo son identificadores de
procedencia; no son punteros ejecutables ni instrucciones interpretadas.

| Rutina ROM | Comportamiento traducido y cobertura |
| --- | --- |
| `0x0AF65A` → `0x0AF6C0` | Inicio del programa `0x23D510`, dos llamadas RNG, cálculo de `+0x62` y `+0x26`; dos entradas medidas. |
| `0x0AF6C0..0x0AF7AC` | Orden de decisiones tras contacto/animación, comparación del objetivo, consultas de entorno y transición final. |
| `0x0AF890` → `0x0AF8A6` | Inicio `0x23D594`, frenado con ASR.L #3 y salida al terminar la animación. |
| `0x0AF380` → `0x0AF39E` | Inicio `0x23D5E2`, escalar cero y decisiones con distancias de palabra de 16, 48 y 72 píxeles. |
| `0x0AF8D4` → `0x0AF8EA` | Inicio `0x23DDB4`, frenado con ASR.L #2 y vuelta a `0x0AF65A` al terminar. El cambio de orientación sigue viniendo del callback nativo de animación. |
| `0x0B0336` → `0x0B0354` | Inicio `0x23EF98`, escalar cero y salida condicionada por animación/apoyo. |
| `0x0AF02A..0x0AF070` | Predicado de contacto sobre `+0x5F`, `+0x7C` y el byte `+0x6D`. |
| `0x0AF0A8..0x0AF0E2` | Predicado que consulta la cuadrícula original mediante `0x00510C`. |
| `0x0AF108..0x0AF14A` | Traducción de la respuesta del servicio `0x040200` a `+0x7E` y rutinas siguientes. El servicio sigue externo. |
| `0x0AEE0C..0x0AEF12` | Selector nativo de acciones, incluido el consumo original del RNG. |
| `0x01AFFC`, `0x01AF3C` | Filtrado de jugadores mediante rectángulo relativo, con inversiones y comparaciones de palabra originales. |
| `0x0AEE04` | Datos numéricos del rectángulo: `(-56, 0, 112, 80)`. |
| `0x0AF494` | Tabla de ocho etiquetas de rutina; el índice 0 contiene cero y el 1 apunta a `0x0AF918`. |

Los estados de entrada inicializan la animación y caen en el cuerpo del estado
en **la misma actualización**, como el original. El código no añade un tick
entre ambas fases. Se conserva el orden de escrituras: una decisión posterior
puede sobrescribir la rutina elegida por una comparación anterior.

## Selector de acciones

El original descarta primero posiciones/selecciones no elegibles. La consulta
pura de jugadores no altera campos del actor; cuando `+0x70` no es cero, la
traducción puede devolver cero anticipadamente sin modificar la secuencia RNG.
Con temporizador cero se ejecuta el filtrado de posiciones y la selección.

Un detalle del ensamblador importa: se cuenta un byte de opciones filtrado,
pero en ROM `0x0AEEB4` se vuelve a leer el **byte original `+0x81`** para enumerar
la opción elegida. La traducción mantiene ese orden. No se sustituye por un
selector aleatorio genérico que enumere otra máscara.

En la captura de combate, fotograma de debugger **2531**, `+0x70=0`, `+0x81=2`,
P1 activo y resultado **1** en ROM `0x0AEF12`. El cursor RNG avanza de RAM
`0x100050` a `0x100052`; se compara también todo el anillo de 55 palabras.
La siguiente entrada observada es `0x0AF918` en 2533. Se confirma la selección,
no la implementación completa de ese ataque.

Las 84 muestras tienen máscara `0x02`: 83 con P1 activo y una sin jugadores
activos. Las ramas de dos jugadores,
otras máscaras y modificación del retardo por el filtrado están traducidas
estáticamente, pero no verificadas dinámicamente en esta muestra.

## Validación y límites

`tools/test_neo_state.c` reutiliza el harness de control añadiendo las entradas
y salidas de estado. No introduce `scalar62`, velocidad de animación, programa
ni rutina siguiente después de la condición inicial. Los resultados esperados
proceden de probes MAME independientes del código C. Se comprueban también
las decisiones de acción y los estados RNG completos de cada consulta.

Siguen entrando desde fuera el constructor inicial, jugadores/cámara/nivel,
las respuestas de `0x0049E8` y `0x040200`, las dos consultas delanteras comunes
y el orden RNG de otros actores. En el tramo integrado, cuatro llamadas RNG
que antes se reproducían externamente ya ocurren dentro de los dos estados de
inicialización nativos. Otras 107 llamadas siguen con orden externo; las 165
del bloque común también son nativas.

Los guardias del juego activo siguen siendo provisionales. No están integradas
las ramas restantes, golpes, daño, callbacks de ataque, proyectiles enemigos,
apariciones de oleadas o campaña. Las rutas sin apoyo y rutinas no recuperadas
fallan explícitamente al entrar; el código no ejecuta direcciones originales.

El ataque observado usa callbacks ROM `0x040648` y `0x03BB8E`. El segundo
llama al asignador `0x00411C` con entrada `0x03BC28`. El seguimiento en
[SOLDIER_ATTACK.md](SOLDIER_ATTACK.md) ya traduce esos efectos, el lanzamiento
y el recorrido observado hasta el borrado. Las consultas de impacto siguen
externas y el ataque aún no está integrado en la IA activa.

## Identidad y borrado

La captura ampliada que mantiene avance a la derecha llega a la entrada de
borrado ROM **`0x004126`**, llamada desde `0x004188`, con A6=`0x101F14`, en el
fotograma de debugger **2561**. El script cierra allí la traza y su puerta de
identidad (`temp1=2`). No atribuye las futuras reutilizaciones del slot a este
soldado. No se deduce tamaño de pool ni identidad por direcciones contiguas.

## Retirada direccional fuera de pantalla

`src/neo_cull.c` traduce ROM `0x01945C..0x019502`. Se suministran límites de
escena y transformaciones del padre mediante datos C alineados; no se leen
punteros de la RAM original. Conserva la posición anterior y la inversión de
los márgenes por orientación. Un eje estacionario no dispara la retirada.
Las ramas BMI/BPL usan el signo de la resta de palabra, conservando su envoltura.

Se comparan 105 consultas exactas: 104 resultados negativos y uno positivo en
2561. Los primeros 54 se encadenan con las coordenadas e historial calculados
por movimiento nativo. El resultado positivo coincide con la posterior entrada
a `0x004126`. En las 105 muestras no hay padre de límites; se cubren ambas orientaciones
horizontales y el rectángulo `(-32, -16, 64, 80)`. La transformación de un padre
y la inversión vertical están traducidas estáticamente, sin cobertura dinámica
en esta captura. El asignador enlazado y la limpieza de recursos no están portados
por este predicado. La traza usa límites desde el descriptor apuntado por RAM
`0x10C2F0`, además de su padre si existe; no inventa un rectángulo de pantalla.

## Primer registro de aparición

Los probes en ROM `0x01A134` y `0x01A152` identifican el registro ejecutado
**`0x2D631A`**, opcode `7`, longitud **22 bytes**, cuando se crea el slot
`0x101F14` desde el actor raíz `0x101414`, en 2351. Sus seis palabras finales
se copian exactamente a `+0x6E..+0x78`: `2, 90, 120, 150, 1, 0`.
Las palabras de coordenadas en `+6/+8` del registro son `352/80`;
`0x01A184/0x01A188` las cargan y la traza confirma la transformación posterior.
El registro siguiente comienza en `0x2D6330`. No se atribuye significado inventado
a la palabra `+4`, cuyo valor es `0x031C`.

[La evidencia numérica](../data/soldier_spawn_evidence.json) y el extractor
conservan direcciones y hashes. Este hallazgo prepara la recuperación de oleadas;
no implementa aún el planificador del script, sus filtros ni el asignador.
Ghidra marca `0x01A074..0x01A085` como **18 bytes de datos**: las instrucciones
`0x01A060/0x01A064` indexan allí flags de opcode. No se desensamblan como código.

Se reproduce con `trace_soldier_spawn.lua` en el runner MAME y
`tools/extract_soldier_spawn.py CAPTURA GHIDRA --output data/soldier_spawn_evidence.json`.
La evidencia local está en `soldier-spawn-debug` y `soldier-spawn-ghidra-final`.

## Reproducción

```
python3 tools/mame/run_first_soldier.py --rom-dir /ruta/roms \
  --script tools/mame/trace_soldier_state.lua --end-frame 2800 --output /ruta/estado
python3 tools/mame/run_first_soldier.py --rom-dir /ruta/roms \
  --script tools/mame/trace_soldier_action.lua --end-frame 2700 \
  --stop-right-frame 2360 --output /ruta/combate
python3 tools/ghidra/run_first_soldier.py /ruta/combate --output /ruta/ghidra \
  --ghidra /ruta/ghidra_11.4.2_PUBLIC --jdk /ruta/jdk-21
python3 tools/extract_soldier_state.py /ruta/estado /ruta/ghidra \
  --output data/soldier_state_fixture.json
python3 tools/extract_soldier_action.py /ruta/combate /ruta/ghidra \
  --output data/soldier_action_fixture.json
python3 tools/mame/run_first_soldier.py --rom-dir /ruta/roms \
  --script tools/mame/trace_soldier_cull.lua --end-frame 2800 --output /ruta/retirada
python3 tools/extract_soldier_cull.py /ruta/retirada /ruta/ghidra \
  --output data/soldier_cull_fixture.json
make check
make state-regression.z64
python3 tools/test_rng_ares.py state-regression.z64 --suite state \
  --output /ruta/validacion
```

La captura ampliada es de descubrimiento; el fixture de estados conserva la
ventana integrada 2353..2459, mientras el de acciones usa sus 84 llamadas.
Los intervalos posteriores no se cuentan como ciclos integrados recuperados.
Las entradas del mando y duración se registran en `capture-options.json`.

Evidencia local: `soldier-state-long-final`, `soldier-action-combat` y
`soldier-state-combat-ghidra` y `soldier-cull-debug`, bajo `/workspace/research`. MAME 0.276 ejecutó las
trazas y Ghidra 11.4.2 el análisis. Su lenguaje disponible es
`68000:BE:32:MC68020`, un superconjunto; la ejecución se contrasta con el M68000
de MAME. No se trata el C descompilado como prueba de comportamiento.
ROM, RAM, proyectos y trazas completas permanecen locales. Los fixtures e
informes numéricos tienen hashes en [soldier-state-validation.json](soldier-state-validation.json).
