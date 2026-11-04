# Consumidores de condiciones del intérprete de eventos

`decomp/recovered/condition_consumers.c/.h` recupera tres entradas observadas:

| Entrada ROM | Comportamiento conservado | Límites de salida |
| --- | --- | --- |
| 01A01E | Consulta el descriptor en script+2. Si C=1 avanza A0 por el byte script+1; si C=0 cede la ejecución. | 01A056 / RTS en 01A02A |
| 01A2D8 | Conserva A1 apuntando al descriptor antiguo, avanza A0 antes de consultar y elige continuación según C. | 01A0A6 / 01A372 |
| 01A4F2 | Consulta script+0A. Si C=0 solicita 004188; si C=1 decrementa +72 o recarga desde script+4, avanza y borra +74. | 004188 / RTS en 01A510 / llamada pendiente en 01A526 |

No se ejecutan direcciones como instrucciones en N64. El resultado es una
etiqueta de continuación que requiere su propietario nativo. Los bloques no
se presentan como funciones completas del intérprete ni se marcan como tales
en el índice global. 004188 conserva un nombre neutral hasta recuperar su
semántica. La ruta de excepción de la consulta se comunica explícitamente.

En +72=1, SUBQ produce cero y termina esa llamada: la recarga no se adelanta.
Se conservan el overflow de palabra (8000 -> 7FFF), las escrituras parciales,
la pila de retorno de la consulta y los flags. La frecuencia del scheduler
original sigue pendiente; no se presupone una ejecución por fotograma.

264 ensayos controlados ejecutan consumidores, despacho y auxiliares originales
reales en MAME. Coinciden los 64 KiB RAM, D0..D7/A0..A7, SR y la continuación:
44 casos por cada una de las primeras dos entradas y 176 de la tercera.
Se cubren todos los destinos declarados y las rutas anteriores a TRAP #15.
Los índices de condición en las entradas de ensayo cubren 0..21; el dominio
de juego completo permanece sin demostrar.

La dirección de continuación se identifica por el breakpoint disparado. El
registro PC crudo del M68000 muestra prefetch (+2) en este entorno; se registra
aparte como debug_pc y no se confunde con la dirección etiquetada. La captura
148 se rechazó por esa confusión del harness; la fixture publicada procede de
149. No se compara temporización de bus ni se ejecuta la continuación pendiente.

La prueba compilada para R4300 pasa en ares, junto a 447+96+96+451 casos de
los módulos anteriores. Host/UBSan también pasa. El informe está en
`docs/native-logic-mips-validation.json`; las entradas/salidas originales en
`data/condition_consumers_fixture.json`. Los deltas de memoria se contrastan
con hashes originales antes de generar las fixtures C.

Siguen pendientes los propietarios de las continuaciones, el resto de comandos,
la producción de los scripts y el scheduler de campaña. Esto no completa la
misión ni la descompilación y no genera otro ZIP de entrega.
