# RNG original y siguientes bloques del primer soldado

Se continúa la cadena de [FIRST_SOLDIER.md](FIRST_SOLDIER.md) usando MAME
0.276 con debugger, registros de entrada/salida y RAM por fotograma, y Ghidra
11.4.2 con el mismo mapa Neo Geo. El lenguaje disponible de Ghidra sigue siendo
`68000:BE:32:MC68020`, superset; no se presenta su C decompilado como validado.

## VERIFICADO: RNG y contexto nativo

| Elemento | Dirección original | Evidencia |
| --- | --- | --- |
| Inicializador | ROM `0x002976` | Ghidra; cuatro pares de breakpoints entrada/retorno |
| Contador usado cuando D0.W no es cero | RAM `0x10F006` | Valores antes/después en los cuatro pares |
| Estado de 55 palabras | RAM `0x100016..0x100082` | Bucle `DBF` en ROM `0x0029BA`; comparación de las 55 palabras |
| Cursor | RAM `0x100084` | Valor capturado antes/después de cada llamada |
| Generador | ROM `0x0029C6` | Llamadas desde `0x0AF66A/0x0AF676` en la traza anterior; 276 pares entrada/retorno |
| Lectura y avance | ROM `0x0029D2..0x0029DE` | Devuelve la palabra anterior al avance y guarda el cursor siguiente |
| Destino XOR | ROM `0x0029E0..0x0029EE` | Cursor nuevo más `0x2E` bytes, con retorno al inicio del anillo |

`neo_rng_t` guarda 55 `uint16_t` y un índice; no contiene direcciones de CPU,
ROM ni punteros a RAM emulada. `neo_rng_next()` lee la palabra actual, avanza
una posición módulo 55 y aplica XOR al destino situado 23 palabras después
del cursor nuevo. Todas las operaciones del estado son de 16 bits.

`neo_rng_seed()` conserva la distinción original: si D0.W vale cero, usa cero
sin modificar el contador; si no, incrementa el contador de 16 bits y usa su
resultado. No interpreta D0.W como una semilla elegida libremente por el llamador.
El núcleo conserva `ADD.W` con carry, `NOT.W`, XOR y rotaciones de una posición;
no usa coma flotante, el generador de la biblioteca C ni un estado global oculto.

Las cuatro llamadas observadas son:

| Fotograma del debugger | D0.W de entrada | Contador antes → después |
| --- | --- | --- |
| 302 | `0x0000` | `0x0000 → 0x0000` |
| 303 | `0xFFFF` | `0x0000 → 0x0001` |
| 720 | `0x0000` | `0x0001 → 0x0001` |
| 760 | `0xFFFF` | `0x0001 → 0x0002` |

La secuencia posterior contiene **276 llamadas consecutivas**, sin saltos de
estado entre retorno y entrada siguiente, y cinco vueltas del cursor al inicio.
El test compara el valor devuelto, el cursor y **las 55 palabras después de
cada llamada**. El fixture almacena el estado inicial y las mutaciones medidas,
sin calcularlas mediante una segunda implementación del algoritmo.

`neo_soldier_seed_from_rng()` llama dos veces al RNG original y extiende con
signo los resultados antes de ejecutar el bloque ya recuperado. Para el actor
RAM `0x101F14` reproduce dos inicializaciones completas de los campos observados:

| RAM posterior al fotograma | `+0x15` | `+0x92` | Salida `+0x62` con signo | Salida `+0x26` |
| --- | --- | --- | --- | --- |
| 2354 | `0x40` | `0x0000` | −194883 | 318 |
| 2442 | `0x41` | `0x0000` | 167909 | 291 |

Esto ejercita ambos signos de `+0x15`; la rama `+0x92!=0` sigue sin ejecución
observada. Las otras llamadas que pasan por la misma rutina se distinguen por
A6; no se atribuyen todas al primer soldado.

## VERIFICADO: decisiones parciales y desaceleración

El bloque ROM `0x0AF6D0..0x0AF724`, posterior al helper `0x01456A`, se ha
traducido a `neo_soldier_ground_select_prefix()`. Se conservan el orden de las
asignaciones, la resta de 16 bits entre `+0x84` y `+0x3C` y la comprobación de
signo mediante XOR con la **palabra alta** de `+0x62` en Big Endian.

Los breakpoints antes del bloque, después de la animación y en `0x0AF72A`
aportan **26 comparaciones de entrada/salida**: 25 conservan `0x0AF6C0` y una
selecciona `0x0AF890`. La escritura de esta última en ROM `0x0AF702` también
aparece en `state-instructions.log`. Las etiquetas de rutina sirven como
identificadores de procedencia, nunca como punteros ejecutables nativos.

**Solo desensamblado, sin cobertura dinámica en esta captura:** selección de
`0x0AF7F6`, rama a `0x0AF8D4` y sustitución por tipo `+0x6C==3`. El resto de
`0x0AF6C0` llama otros helpers y puede sobrescribir el resultado. Este prefijo
no se presenta como una máquina de estados completa ni identifica aún sus
estados como «perseguir», «disparar» o «morir».

La traza ejecuta después ROM `0x0AF8A6..0x0AF8AC`: lee `+0x62`, hace
`ASR.L #3` y resta el resultado al campo original. `neo_soldier_damp_motion()`
conserva el redondeo hacia menos infinito; dividir un negativo entre ocho con
truncamiento hacia cero daría otro resultado. Coincide exactamente con **17
pares de RAM consecutivos a la cadencia observada**, de los fotogramas
2398→2400 hasta 2430→2432. Esto verifica la desaceleración, no la colisión ni
la animación ejecutadas por los helpers posteriores.

## Comprobaciones y reproducción

`make check` pasa, incluidos los módulos anteriores. El mismo test de fixtures
se compila para R4300i/libdragon y se ejecuta en **ares 146** en un cartucho
interno sin gráficos ni ROM originales. Comprueba las cuatro inicializaciones
del RNG, 276 estados completos, dos inicializaciones del soldado, 26 decisiones
parciales y 17 actualizaciones de desaceleración. Los hashes y resultados están
en [soldier-rng-validation.json](soldier-rng-validation.json).

Se corrigió una regla de compilación: libdragon heredaba el compilador MIPS
desde el objetivo `.z64`, pero un objetivo `.o` pedido directamente podía usar
`cc` del host. Por eso el antiguo comando aislado no demostraba compilación
MIPS. Ahora se fija explícitamente para los objetos de `src/`; `file` confirma
ELF MIPS Big Endian y ares ejecuta las aserciones del binario enlazado.

```sh
python3 tools/mame/run_first_soldier.py --rom-dir /ruta/roms \
  --script tools/mame/trace_soldier_rng.lua --output /ruta/nueva-captura
python3 tools/ghidra/prepare_neogeo_inputs.py /ruta/mslugx.zip /ruta/nueva-captura
python3 tools/ghidra/run_first_soldier.py /ruta/nueva-captura \
  --ghidra /ruta/ghidra_11.4.2_PUBLIC --jdk /ruta/jdk21 --output /ruta/nuevo-ghidra
python3 tools/extract_soldier_rng.py /ruta/nueva-captura /ruta/nuevo-ghidra \
  --output data/soldier_rng_fixture.json
make check
make rng-regression.z64
python3 tools/test_rng_ares.py rng-regression.z64 --output /ruta/nueva-validacion
```

Las capturas definitivas locales están en `/workspace/research/soldier-rng-debug-v3`;
el desensamblado en `/workspace/research/soldier-rng-ghidra`. Los logs completos,
ROM, RAM y proyectos Ghidra permanecen locales; GitHub contiene scripts,
fixtures numéricos acotados y hashes. El reloj del debugger sigue la convención
anterior de último fotograma completado. El log conserva por separado la
dirección del breakpoint (`site`) y el PC informado con prefetch (`pc`).

## Pendiente para integrarlo en el juego

El seguimiento de animación está en [SOLDIER_ANIMATION.md](SOLDIER_ANIMATION.md):
ya se reproduce en C el subconjunto observado del helper, con estado persistente,
fin de secuencia y callback de orientación. No cubre todos los comandos del juego.

La campaña nativa aún no reproduce el orden global de llamadas al RNG de todos
los actores. Tener la rutina correcta no garantiza la misma secuencia si el
orden de actualización cambia. Faltan helpers de movimiento/contacto, lectura
completa de animaciones, impactos, transiciones posteriores y eventos de aparición.
Los nuevos módulos no sustituyen aún la IA provisional de los tres guardias.
No se publica un cartucho de demostración ni se afirma rendimiento del juego
completo a partir de una prueba interna del backend.
