# Movimiento, contacto estático y animación encadenada del soldado

El siguiente bloque continúa [SOLDIER_ANIMATION.md](SOLDIER_ANIMATION.md).
`src/neo_contact.c` implementa preparación del movimiento, un subconjunto de
contacto con terreno, actualización del actor y proyección. No es una emulación
del 68000 y no incluye el solver genérico completo ni la IA completa.

## VERIFICADO

La captura usa MAME 0.276 con debugger y Lua, después del constructor que
identifica el actor RAM `0x101F14`. Ghidra 11.4.2 se ejecutó sobre el mapa Neo
Geo anterior, conservando su limitación de decoder MC68020 superset. No se usa
su C decompilado como código validado.

| Bloque / dato | Dirección original | Evidencia |
| --- | --- | --- |
| Entrada del helper de suelo | ROM `0x01456A` | 54 entradas y salidas, fotogramas 2353..2459 |
| Preparación y bloqueo por banderas previas | ROM `0x0145BC..0x01469E` | Solicitud al kernel medida en cada llamada |
| Vector de movimiento | ROM `0x002E98..0x002EB2` | 54 pares de registros de entrada/salida |
| Tabla de coeficientes enteros | ROM `0x0029FC` | 320 palabras del bloque CPU fijo capturado |
| Límite del kernel genérico | ROM `0x013968`, retorno a `0x014492` | 54 resultados completos de movimiento, separados de otras llamadas |
| Escritura de posición, velocidad y padre | ROM `0x014492` | Campos del actor `+0x28..+0x38` antes/después |
| Actualización de banderas | ROM `0x014498..0x0144F8` | Campos `+0x15`, `+0x5F` y `+0x5A` |
| Conversión a coordenadas de presentación | ROM `0x01ABC8..0x01AC36` | Campos `+0x3C..+0x4D`, acarreo fraccional y caché |
| Palabra de salida `+0x1E` | ROM `0x01AD8C..0x01ADA8` | Resta de palabra condicionada por `+0x66` |
| Tabla de ángulo por forma | ROM `0x012E6A`, leída desde `0x012E64` | Valores capturados; formas del terreno comprobadas |
| Mapa de grupos | RAM `0x10C00E →` ROM `0x0B5332` | Puntero medido y ocho bytes del bloque fijo |

Los 54 resultados conservan el padre RAM `0x10C72E`, usado como objeto de
terreno en esta partida. El programa nativo conserva una etiqueta de objeto
y recibe una estructura C; **no desreferencia una dirección CPU original**.
Las otras 110 entradas observadas en `0x013968` no están dentro de este helper
de movimiento y se excluyen del fixture. No se les atribuye una función sin
seguir sus llamadores.

## Aritmética y proyección nativas

La conversión de velocidad conserva las operaciones originales:

```text
q  = signo16(parte_baja16(desplazamiento_aritmético(scalar62, 8)))
vx = q * coeficiente[ángulo]
vy = -q * coeficiente[ángulo + 64]
```

No se sustituyen por funciones trigonométricas ni velocidades elegidas a mano.
El desplazamiento de negativos y la truncación a palabra antes de multiplicar
son parte del resultado. Posición y velocidad siguen en Q16.16. Las sumas y
restas que necesitan envoltura de 32 bits se expresan sin overflow firmado de C.
Los campos se guardan como números C con alineación natural, no copiando el
layout binario del nodo Neo Geo. Por ejemplo, `scalar62` identifica el campo de
origen `+0x62`, pero no obliga a realizar una lectura MIPS de 32 bits en ese
offset alineado solo a dos bytes.

La preparación produce una estructura con posición, desplazamiento y modos de
consulta. La actualización consume un resultado tipado con contacto, ángulo y
objeto padre. Se separan así los campos del actor de la RAM global de scratch
usada por el arcade.

La proyección conserva la suma del objeto padre, los acarreos de las palabras
fraccionales y el marcador de época que decide cuándo guardar las coordenadas
anteriores. La palabra `+0x1E` se calcula con la condición original sobre
`+0x66`; no se le asigna una semántica inventada de profundidad 3D.

## Subconjunto de contacto comprobado

El kernel genérico `0x013968` **no está portado íntegramente**. El nuevo solver
usa la cuadrícula nativa recuperada y solo cubre un contexto estático con una
superficie única, grupo `0xC0`, destino plano de forma 11 o pendiente de formas
1..4. La primera muestra comienza en una pendiente 6 y termina sobre plano.
Comprueba apoyo o cruce del plano; no introduce un margen de ajuste elegido
para hacer coincidir las muestras.

Este solver calcula sus resultados; no recibe del fixture la posición,
velocidad ni banderas que debe devolver. Los **54 resultados completos**
coinciden con los de MAME: X/Y, VX/VY, padre, palabra de tipo/ángulo y banderas.
La actualización posterior coincide también en los campos del actor y de
presentación. Hay 43 salidas con palabra `0x00084A00` y 11 con `0x00084000`;
todas devuelven banderas de contacto `0x0001`.

La selección conservadora de superficie rechaza columnas con varios pisos.
Una prueba crea restos encima del terreno mediante el componente existente,
verifica que la llamada devuelve `false` y comprueba que deja intactos el actor
y la salida del llamador. Esto permite una futura ruta de solver más completa.
La función no tiene datos del broadphase de actores: **el llamador debe haber
establecido que no hay candidatos dinámicos relevantes**. No detecta por sí
sola todos los casos que quedan fuera de ese contexto.

## Movimiento y animación juntos

Además de probar preparación y actualización por separado, el test encadena
el movimiento nativo con `neo_anim_step()`. Conserva posición, velocidad,
estado de proyección, cursor y temporizador entre llamadas. La posición del
dibujo, el acarreo y la palabra `+0x1E` proceden del cálculo nativo de movimiento,
no de recargarlos desde el fixture de animación.

Se reproducen **54 movimientos y 54 dibujos** con los mismos descriptores,
coordenadas y atributos. El callback de animación conserva el cambio de
orientación para el movimiento siguiente.

Siguen siendo entradas externas medidas el `scalar62` y las banderas `+0x5F`
producidas por el controlador/sensado aún no recuperado. También se proporciona
el estado del objeto de terreno/cámara. Las dos escrituras externas de velocidad
de animación ya verificadas se conservan. **No se presenta este replay como una
IA cerrada que tome sus decisiones de forma autónoma.**

`make check` pasa. El cartucho interno de regresión enlaza estos módulos para
R4300i/libdragon y pasa en ares 146, incluyendo el rechazo de pisos superpuestos.
Hashes de ROM interna, log y ambos fixtures:
[soldier-contact-validation.json](soldier-contact-validation.json).
Esto no mide el rendimiento del juego completo ni valida un nuevo renderer.

## Reproducción

```sh
python3 tools/mame/run_first_soldier.py --rom-dir /ruta/roms \
  --script tools/mame/trace_soldier_contact.lua --output /ruta/nueva-captura
python3 tools/ghidra/prepare_neogeo_inputs.py /ruta/mslugx.zip /ruta/nueva-captura
python3 tools/ghidra/run_first_soldier.py /ruta/nueva-captura \
  --ghidra /ruta/ghidra_11.4.2_PUBLIC --jdk /ruta/jdk21 --output /ruta/nuevo-ghidra
python3 tools/extract_soldier_contact.py /ruta/nueva-captura /ruta/nuevo-ghidra \
  --output data/soldier_contact_fixture.json
make check
make contact-regression.z64
python3 tools/test_rng_ares.py contact-regression.z64 --suite contact \
  --output /ruta/nueva-validacion
```

Se usa también `data/soldier_animation_fixture.json`, generado mediante el
procedimiento documentado en el informe de animación. El compilador de fixtures
comprueba la correspondencia de las 54 fronteras movimiento→animación.

Evidencia local definitiva: `/workspace/research/soldier-contact-debug-final`
y `/workspace/research/soldier-contact-ghidra-final`. Solo se suben scripts,
tablas y fixtures numéricos acotados, informes y hashes; no ROM ni RAM completas.
No se publica otra demo.

## No verificado / pendiente

No hay cobertura dinámica aquí de la rama sin apoyo, interpolación del tipo
`0x0D`, bloqueos previos de pared, peligros, padre nulo, marcador de proyección
inicial cero ni otros modos del kernel. Sus instrucciones pueden estar
desensambladas sin quedar por ello validadas en ejecución.

Faltan paredes, techos, pisos superpuestos, plataformas móviles, otras formas y
grupos de terreno, contactos con actores, impactos y las transiciones restantes
del soldado. El controlador de campaña y sus apariciones siguen pendientes;
los tres guardias del prototipo continúan siendo provisionales.

## Continuación verificada

El seguimiento integrado en [SOLDIER_CONTROL.md](SOLDIER_CONTROL.md) ya calcula
`+0x5F` desde el bloque común y el RNG original antes del movimiento siguiente.
La prueba anterior de este informe conserva su frontera y sus hashes históricos.
