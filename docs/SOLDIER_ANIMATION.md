# Animación nativa: temporizador, dibujo y callback del primer soldado

Se continúa [SOLDIER_RNG.md](SOLDIER_RNG.md). El nuevo módulo
`src/neo_animation.c` convierte el subconjunto observado del helper de animación
a C: conserva el estado entre llamadas y emite descriptores de dibujo sin
escribir registros de Neo Geo. Todavía no sustituye el controlador provisional
de enemigos ni ejecuta la IA completa.

## VERIFICADO con direcciones y probes

| Elemento | Dirección original | Comprobación |
| --- | --- | --- |
| Inicio de animación | ROM `0x004256` | Seis pares de breakpoints entrada/retorno |
| Estado del script | Actor RAM `0x101F14 + 0x20` | Cursor antes/después de cada llamada |
| Temporizador | Actor `+0x24` | Palabra completa antes/después; valores negativos incluidos |
| Velocidad de animación | Actor `+0x26` | Dos cambios externos: `0x0100→0x013E` y `0x0100→0x0123` |
| Actualización | ROM `0x00427A` | 54 pares de entrada/salida, fotogramas 2353..2459 |
| Recarga de tiempo | ROM `0x00431E` | Suma solo al byte alto; no se propaga carry al byte bajo |
| Consumo de tiempo | ROM `0x00432C` | Resta de palabra con envoltura a 16 bits |
| Fin de secuencia | ROM `0x004338..0x00435A` | Dos salidas con carry y conservación del byte fraccional |
| Preparación del dibujo | ROM `0x004364..0x004406` | 54 descriptores, posiciones, atributos y palabras `+0x1E` |
| Escritura del comando `0x20` | ROM `0x004470..0x004476` | Escritura observada en `+0x50`; se conserva como campo neutral |
| Salto del script `0x08` | ROM `0x00449E..0x0044A4` | Desensamblado y correspondencia del cursor en el replay |
| Callback | Llamada ROM `0x0044EC`, destino `0x01ADAA` | Probes antes/después, atributo `0x7140→0x7141` |
| Cambio de orientación | ROM `0x01ADAA` | `BCHG #0,+0x15`; comprobado en la llamada capturada |

Las direcciones de campos pertenecen a este actor y esta partida. Los probes
se habilitan después del watchpoint de su constructor, no solo por A6: el
mismo slot se reutiliza durante la atracción antes de Misión 1.

Ghidra se ejecutó sobre el mapa capturado y el mismo lenguaje superset
`68000:BE:32:MC68020`. Se definió como datos el texto diagnóstico situado tras
el `TRAP` en `0x0042AA..0x0042BB`, para evitar que sus bytes se desensamblasen
como ramas falsas hacia el interior de instrucciones reales. Los resultados
decompilados siguen siendo tentativos; la comparación usa instrucciones,
probes y resultados, no tipos propuestos por el decompilador.

## Conversión a datos y C nativos

El importador convierte cinco secuencias observadas en comandos tipados:

- espera/duración;
- pose y descriptor de asset;
- salto a un índice del programa nativo;
- escritura del campo `field50`;
- fin de secuencia;
- cambio de orientación recuperado;
- callback aún no recuperado, que produce un error explícito al alcanzarlo.

Los programas proceden de las direcciones CPU `0x23D510`, `0x23D594`,
`0x23D5E2`, `0x23DDB4` y `0x23EF98` en la ventana bancaria capturada. No se
ejecutan instrucciones 68000 ni se convierten sus punteros a funciones MIPS.
Los saltos son índices y los descriptores son identificadores de assets.

`neo_anim_start()` reinicia cursor, temporizador y campo `+0x18`, y conserva
flash y `field50`, como las escrituras del helper original. `neo_anim_step()`
conserva temporizador y cursor; admite redespachos en una misma actualización,
incluidos los necesarios para omitir una pose cuyo tiempo ya ha expirado.
Devuelve el fin de secuencia como un valor C equivalente al carry usado por
los llamadores observados.

El dibujo se entrega a un callback nativo con descriptor, coordenadas y
atributos. Ese callback es la frontera para el renderer libdragon/RDP: el
módulo no contiene acceso a SCB, VRAM ni registros de video Neo Geo. La palabra
procedente de `+0x1E` se transporta sin asignarle una nueva semántica de Z 3D.
El cambio de orientación actualiza los atributos de entrada/salida; el futuro
controlador debe conservar ese cambio también para físicas e IA.

El nombre neutral `field50` evita presentar como verificada su semántica
completa. Un primer log utilizaba la etiqueta especulativa `collision`; el
extractor acepta esa etiqueta histórica y la normaliza sin cambiar su valor.

## Replay y pruebas realizadas

El fixture contiene cinco programas, seis inicios y 54 actualizaciones. El test
conserva el estado C entre **las 60 llamadas**, sin recargar los temporizadores
ni los cursores esperados entre pasos. Solo recibe las dos escrituras externas
de velocidad ya medidas en los inicializadores del soldado. Se comparan:

- cursor, temporizador, velocidad, flash, campo `+0x18` y `field50`;
- los 54 dibujos: descriptor, X/Y, atributos de paleta/orientación y palabra `+0x1E`;
- las dos señales de fin y el callback de orientación.

Hay 14 actualizaciones que entran con temporizador negativo. Los 54 dibujos
usan flags de pose `0x40`, con atributos `0x7140` o `0x7141`. Por tanto, los
offsets de pose no nulos, otras rutas de paleta y el parpadeo suprimido **no
quedan cubiertos dinámicamente** por esta captura, aunque sus instrucciones
están desensambladas y su traducción está separada en el módulo.

`make check` pasa. El mismo replay se enlaza para MIPS/R4300i en un cartucho
interno y pasa en ares 146. Los hashes están en
[soldier-animation-validation.json](soldier-animation-validation.json).
Esto verifica la ejecución del helper y sus salidas numéricas: no afirma
validación del renderer del soldado ni rendimiento del juego completo.

```sh
python3 tools/mame/run_first_soldier.py --rom-dir /ruta/roms \
  --script tools/mame/trace_soldier_animation.lua --output /ruta/nueva-captura
python3 tools/ghidra/prepare_neogeo_inputs.py /ruta/mslugx.zip /ruta/nueva-captura
python3 tools/ghidra/run_first_soldier.py /ruta/nueva-captura \
  --ghidra /ruta/ghidra_11.4.2_PUBLIC --jdk /ruta/jdk21 --output /ruta/nuevo-ghidra
python3 tools/extract_soldier_animation.py /ruta/nueva-captura /ruta/nuevo-ghidra \
  --output data/soldier_animation_fixture.json
make check
make animation-regression.z64
python3 tools/test_rng_ares.py animation-regression.z64 --suite animation \
  --output /ruta/nueva-validacion
```

La evidencia definitiva local está en `/workspace/research/soldier-animation-debug-v3`
y `/workspace/research/soldier-animation-ghidra-final`. Se publican scripts,
datos numéricos convertidos y hashes, sin ROM, RAM completa ni trazas completas.
No se publica el cartucho interno como demo.

## Pendiente / no verificado

El callback identificado por ROM `0x0AEF14` aparece en los datos importados, pero
no se alcanza en estas 54 actualizaciones y no está portado. Si se alcanza en
C, el módulo devuelve `NEO_ANIM_UNSUPPORTED`, sin simular efectos inventados.
Tampoco se cubren todos los comandos de animación del juego, el scratch global
del intérprete ni todos los efectos de sus callbacks. El presupuesto de comandos
es una protección del llamador ante datos malformados, no un límite atribuido
al arcade.

El [seguimiento de movimiento/contacto](SOLDIER_CONTACT.md) ya reproduce el
recorrido de suelo observado y lo encadena con estas animaciones. El kernel
genérico y el controlador completo permanecen pendientes.

Faltan el helper completo de movimiento/contacto del soldado, impactos, el
resto de sus transiciones y la integración con assets/renderizado de la
campaña. Este avance no convierte los tres guardias provisionales en IA original.
