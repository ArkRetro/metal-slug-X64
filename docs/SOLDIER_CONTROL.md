# Bloque común nativo del primer soldado

Continúa [movimiento y contacto](SOLDIER_CONTACT.md). Se traduce a C el bloque
común observado, sin interpretar instrucciones 68000 durante el juego. El
estado persiste durante 55 ciclos; sus decisiones alimentan 54 movimientos y
54 registros de dibujo mediante los módulos nativos de contacto y animación.
La IA completa y su integración en la campaña siguen pendientes.

## Verificado con dirección

Actor observado: RAM `0x101F14`, constructor ROM `0x0AF17E`. Los offsets de esta
tabla son relativos a ese actor; no constituyen un formato universal de actores.

| ROM / RAM | Evidencia y traducción |
| --- | --- |
| ROM `0x0AE9C0..0x0AEB3E` | Actualización de campos `+0x84`, `+0x88`, `+0x8C`: selección y suavizado de objetivo, aritmética de palabra y punto fijo. |
| ROM `0x01903A`, retorno observado `0x0AE9DA` | 44 consultas de posiciones; con solo P1 activo, ambas salidas reproducen la posición de P1. |
| ROM `0x0AEB5E..0x0AED1C` | Selección `+0x80`, bits `+0x7C`, temporizadores `+0x70/+0x74`, consulta de probabilidad y decisiones RNG. No se atribuye salud a estos campos. |
| ROM `0x0260C8`, `0x025FA6`, `0x2D3CD8` | Consulta de probabilidad, tabla de recuento de bits y cabecera de tabla de palabras `01 FF`. Datos desde `0x2D3CDA`. |
| ROM `0x0AEC88..0x0AECCE` | Decisión sobre bits 4/5 de `+0x5F`. Watchpoint de escritura en RAM `0x101F73`: 164 escrituras observadas. |
| ROM `0x0AECAE`, `0x0AECE2` | Los saltos BPL consumen el signo del XOR final del RNG, no el signo de su valor devuelto. |
| ROM `0x00510C..0x00514A`, RAM `0x1067E6..0x1068E5` | Consulta de cuadrícula de 16×8 palabras, capturada exactamente en cada entrada del controlador. No se ha recuperado su productor ni su significado general. |
| ROM `0x0AED1C..0x0AEDA6` | Conversión de dos resultados de consultas delanteras a bits de `+0x7C`; resultados observados en `0x0AED54` y `0x0AED98`. Los kernels de estas consultas siguen externos. |

La consulta de probabilidad multiplica su índice base por **12 bytes**, y añade
`variante * 2` bytes. En el array C de palabras equivale a `base * 6 + variante`.
Se importan 102 palabras que cubren las lecturas admitidas por esta traducción;
no se afirma que sean la longitud universal de la tabla original.

## Condiciones del RNG

ROM `0x0029C6` devuelve en D0 una palabra antigua del anillo. Sin embargo, su
último `EOR.W`, en `0x0029EE`, actualiza N/Z según la palabra modificada del
anillo. En **128 de 276 llamadas observadas**, ambos signos son distintos.
Usar el signo de D0 produciría decisiones incorrectas aunque el valor aleatorio
fuese correcto.

`neo_rng_next_result()` conserva ambos valores; `neo_rng_next()` mantiene su
API anterior. Se comparan los valores, la palabra mezclada, N/Z y todos los
estados del anillo con MAME. No se implementa un emulador de CCR completo.
Los tipos C tienen alineación natural; las palabras big endian se decodifican
fuera del runtime. Las operaciones conservan truncamiento de 16 bits,
desplazamiento aritmético y envoltura de 32 bits, sin divisiones flotantes.

## Prueba integrada

`tools/test_neo_control.c` mantiene el controlador, movimiento y animación entre
ciclos. El movimiento recibe `flags5f` **calculado por el controlador anterior**;
no lo introduce desde la captura. Conserva también el cambio de orientación
calculado por el callback de animación. Compara objetivos, contadores, flags,
coordenadas, contactos, cursores de animación y registros de dibujo.

El perfil medido tiene P1 activo, modo `2`, rango `3`, variante `0` y selector
`+0x6E = 2`. El umbral es `0x004C` en los 55 ciclos; se observan nueve éxitos
probabilísticos y ambas ramas de signo del selector. La cuadrícula capturada
está a cero y `+0x92` permanece cero en todos los ciclos.

`make check` pasa las regresiones anteriores y la nueva. La ROM interna MIPS
pasa en **ares 146** con el marcador:

```
MSX CONTROL REGRESSION PASS cycles=55 rng=276 moves=54 draws=54
```

Hashes y alcance: [soldier-control-validation.json](soldier-control-validation.json).
Es una prueba del backend de CPU; no mide el rendimiento gráfico del juego
completo ni constituye una beta. No se publica este cartucho.

## Entradas externas y cobertura pendiente

Siguen externos el estado inicial, las decisiones escalares de la máquina de
estados principal, seis inicios de programa de animación y dos escrituras de
velocidad, posiciones de jugadores/configuración/cámara, cuadrícula de consulta,
y los dos resultados de consultas delanteras. El orden global de llamadas RNG
de otros actores se reproduce desde la captura: 165 llamadas ocurren dentro de
este bloque común y 111 fuera. Recuperar la rutina RNG no recupera ese orden.

Las ramas de dos jugadores, selectores 0/1, retardos distintos de cero y valores
positivos de cuadrícula están traducidas desde instrucciones identificadas,
pero **no verificadas dinámicamente aquí**. Ninguna palabra XOR final es cero
en esta muestra; la condición Z cero tampoco tiene cobertura dinámica. El
subconjunto de contacto conserva los límites de su informe: suelo estático,
sin resolver paredes, plataformas móviles o pisos superpuestos.

Los guardias activos del prototipo continúan con IA provisional. Faltan la
máquina de estados principal del soldado, aparición de oleadas, impactos y las
restantes entidades/misiones. No se deducen direcciones, salud ni límites del
pool a partir de estos resultados.

## Reproducción y procedencia

```
python3 tools/mame/run_first_soldier.py --rom-dir /ruta/roms \
  --script tools/mame/trace_soldier_control.lua --rng-flags --output /ruta/captura-nueva
python3 tools/ghidra/run_first_soldier.py /ruta/captura-nueva \
  --output /ruta/ghidra-nuevo --ghidra /ruta/ghidra_11.4.2_PUBLIC \
  --jdk /ruta/jdk-21
python3 tools/extract_soldier_control.py /ruta/captura-nueva /ruta/ghidra-nuevo \
  --output data/soldier_control_fixture.json
make check
make control-regression.z64
python3 tools/test_rng_ares.py control-regression.z64 --suite control \
  --output /ruta/validacion-nueva
```

Requiere también los fixtures de contacto y animación documentados previamente.
Capturas locales definitivas: `/workspace/research/soldier-control-debug-v2` y
`/workspace/research/soldier-control-ghidra-v2`. MAME 0.276 ejecutó breakpoints,
watchpoint y traza sin modificar RAM de juego. Ghidra 11.4.2 usa el decodificador
`68000:BE:32:MC68020`, un superconjunto disponible en esa distribución; las
instrucciones ejecutadas se contrastan con el M68000 de MAME. El C descompilado
por sí solo no se trata como evidencia de comportamiento.

Git contiene scripts, fixtures numéricos acotados y hashes. ROM, volcados RAM,
trazas completas y proyectos Ghidra permanecen locales.
