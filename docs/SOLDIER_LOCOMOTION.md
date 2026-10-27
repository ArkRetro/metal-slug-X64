# Reposicionamiento nativo y tablas de poses

Se continúa la recuperación de [SOLDIER_STATE.md](SOLDIER_STATE.md) y
[SOLDIER_ATTACK.md](SOLDIER_ATTACK.md). ROM `0x0AF4F6` inicia un movimiento de
reposicionamiento y cae en `0x0AF572` durante el mismo tick. Ahora ese recorrido
calcula velocidad, programa, contacto, selección de pose y transición en C.

## Verificado con dirección

| ROM | Traducción y evidencia |
| --- | --- |
| `0x0AF4F6..0x0AF532` | Comparación de palabra entre objetivo y posición, orientada por bit 0; elige `0x23E0DA` o `0x23E01E`. La captura ejecuta `0x23E0DA`. |
| `0x0AF532..0x0AF56C` | ASR.L #5 de la diferencia, una llamada RNG y ajuste firmado `((random & 0x7FFF) - 0x4000)`. Calcula `+0x62` y la velocidad de animación `+0x26`. |
| `0x0AF582..0x0AF5B8` | Calcula `+0x61` desde el resultado angular del contacto y la orientación, antes de llamar a animación. |
| `0x0AF5B8..0x0AF656` | Orden de decisiones: fin de animación, comparación orientada, consulta de región, selector de acción, cuadrícula, consulta de objeto y contador `+0x74`. |
| `0x004412..0x004450` | Selección de pose inline desde el byte `+0x61`, sin cambiar el cursor al interior de la tabla. |
| `0x004428..0x00442A` | Si el índice supera la tabla, usa la **última** entrada: longitud menos ocho bytes. No selecciona la primera. |
| `0x012DEE..0x012E08` | Comparación de plano descendente con `0x7FFFF`, desplazamiento y resta. Explica el sesgo fraccionario diferente respecto a los planos ascendentes. |
| `0x01319E` | El recorte vertical descendente observado aplica `ORI.W #0x0042` a los flags antes de añadir el contacto adjunto. |

La tabla inline de la animación se convierte fuera del runtime a un comando
`NEO_ANIM_SELECT_POSE` y entradas `NEO_ANIM_POSE`. El render recibe `variant61`
como dato nativo; no accede a offsets de un actor 68000. La conversión solo
admite el campo y el formato de tabla recuperados; otros formatos fallan.

Ghidra define `0x004438..0x004445` como 14 bytes de datos, el texto
`pat index err` seguido de cero. Siguen al TRAP de error; no son instrucciones
ni ramas válidas. Las instrucciones del helper se contrastan con la traza MAME.

## Cobertura dinámica

La captura aporta **34 actualizaciones y dibujos**, dentro de cinco segmentos
de reposicionamiento. Los cinco inicios consumen una llamada RNG cada uno y
el selector de acción consume otra en 2531: seis llamadas nativas en total,
con comparación de las 55 palabras y del cursor después de cada tramo principal.

Se conservan movimiento, historial y animación entre actualizaciones contiguas
de cada segmento. Los cinco estados de entrada y los límites entre segmentos
proceden de la captura. La prueba no afirma reproducir los estados intermedios
que todavía no se han integrado.

Las transiciones finales medidas son 29 permanencias en `0x0AF572`, tres a
`0x0AF380`, una al ataque `0x0AF918` y una al giro `0x0AF8D4`. Se conserva el
orden original de sobrescritura de la rutina. La señal de fin se toma del
resultado de animación nativo, no del oráculo esperado del test.

En las 34 actualizaciones, `+0x61=0`, la cuadrícula externa es cero, las consultas
de región devuelven cero y las consultas de objeto `0xFFFF`. Solo un tick tiene
`+0x70=0`, el que selecciona el ataque. El otro programa, variantes 1/2, clamp de
índice y ramas alternativas tienen cobertura estática. La rama de pared que
llama a `0x0AF072` continúa devolviendo unsupported en este componente.

## Corrección de pendiente

La geometría anterior añadía una unidad Q16.16 a todos los planos de pendiente.
La traza `0x012DEE` usa `0x7FFFF` al comparar la pendiente descendente, por lo
que su corrección de contacto ya incorpora esa unidad. La consulta nativa
conserva el sesgo en planos ascendentes y lo elimina en los descendentes 6..9.
La diferencia era de **1/65536 de píxel**, no de un píxel entero.

El resolver estático admite ahora destinos 6..9. También recupera el recorte
observado al avanzar desde una pendiente descendente a un piso plano: limita Y,
ajusta la velocidad vertical y devuelve flags `0x0043`. Se apoya en la escritura
`0x01319E`, no en una asignación ligada al número de fotograma. El caso dinámico
medido ocurre en 2479. Se conserva el rechazo de pisos superpuestos, paredes,
padres móviles, otros recortes no recuperados y candidatos dinámicos relevantes.

## Entradas aún externas

El bloque común y la escena siguen suministrando sus campos de control y
`flags5f` en esta prueba de reposicionamiento; el RNG se inicia desde la frontera
medida de cada actualización principal. Las llamadas internas se ejecutan de
forma nativa, pero el orden global de otros actores no se recupera aquí. Los
servicios de región/objeto, nivel/cámara y recursos de paleta también son entradas.
No es una IA cerrada de campaña ni una Misión 1 jugable completa.

## Validación

`make check` conserva las regresiones anteriores y añade este recorrido.
La compilación MIPS se ejecuta en ares 146; el ataque con su actor de 41 pasos
se vuelve a verificar después del cambio de geometría. El frontend existente
se compila y recorre su región parcial en ares: controles, disparos, dunas,
tejado, tres encuentros provisionales, límites, reinicio y laboratorio.
No se interpreta su marcador REGION END como el final de la misión original.

Informes: [soldier-locomotion-validation.json](soldier-locomotion-validation.json)
y [slope-gameplay-validation.json](slope-gameplay-validation.json).
Los informes de ataque conservan el nuevo hash de ROM y fixtures.

```
python3 tools/mame/run_first_soldier.py --rom-dir /ruta/roms \
  --script tools/mame/trace_soldier_locomotion.lua --end-frame 2700 \
  --stop-right-frame 2360 --output /ruta/captura
python3 tools/ghidra/run_first_soldier.py /ruta/captura --output /ruta/ghidra \
  --ghidra /ruta/ghidra_11.4.2_PUBLIC --jdk /ruta/jdk-21
python3 tools/extract_soldier_locomotion.py /ruta/captura /ruta/ghidra \
  --output data/soldier_locomotion_fixture.json
make check
make locomotion-regression.z64
python3 tools/test_rng_ares.py locomotion-regression.z64 --suite locomotion \
  --output /ruta/validacion
```

El script divide los snapshots de RNG/campos y de cuadrícula en dos acciones
printf consecutivas del mismo breakpoint para respetar el límite de parámetros
de MAME. El extractor exige que coincidan sitio y fotograma; no rellena datos
perdidos. Las consultas del kernel que ocurren fuera del tramo de movimiento
se excluyen expresamente.

Evidencia local final: `/workspace/research/soldier-locomotion-debug-v2` y
`/workspace/research/soldier-locomotion-ghidra-reviewed`. Se usaron MAME 0.276 y
Ghidra 11.4.2 con el superset `68000:BE:32:MC68020`; el descompilado sigue siendo
tentativo. ROM/RAM y trazas completas permanecen locales; Git conserva scripts,
fixtures numéricos acotados y hashes. No se publica otro cartucho de demo.
