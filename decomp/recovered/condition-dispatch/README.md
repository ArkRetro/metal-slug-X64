# Tabla de condiciones: retorno en CCR.C

El despacho de **0x01A5CA..0x01A7F2** y sus 14 manejadores ya tienen
[reconstrucción C](../rom_01a5ca.c), con contrato en
[rom_01a5ca.h](../rom_01a5ca.h). La traducción recorre las 22 entradas conocidas
y conserva explícitamente CCR.C, los demás flags, registros parciales y las
escrituras temporales de pila. No ejecuta instrucciones de una CPU emulada.
Sigue siendo C con direcciones y ABI explícitas; no está integrado en campaña.

El JSR indexado de 0x01A5D6 selecciona stubs BRA.W en 0x01A5DE.
[table.tsv](table.tsv) conserva los 22 destinos comprobados contra P1;
[rom_01a5ca.asm](rom_01a5ca.asm) procede del decodificador M68000 de MAME.
Los consumidores de 0x01A026, 0x01A2E8 y 0x01A4FA usan BCS tras llamar al
despacho. Los manejadores producen ese resultado con ORI/ANDI sobre CCR.
El C automático anterior eliminaba parte del cálculo al inferir funciones void;
los cuerpos aparentemente vacíos no se aceptaron como traducción válida.

## Evidencia y alcance

[447 casos controlados](../rom_01a5ca.validation.json) ejecutan el despacho y
los manejadores originales en MAME y se comparan con el C. Coinciden los
64 KiB de RAM, D0..D7/A0..A7, SR y el contexto registrado en las entradas de los servicios
(D0, A0, A7, SR y dirección de retorno).
Se verifican todas las entradas y, salvo la condición incondicional 10,
resultados de acarreo tanto a cero como a uno para cada índice.

**0x01903A y 0x0AAA88 están interceptadas con salidas sintéticas**, registrando
sus argumentos, flags y dirección de retorno. La prueba no verifica la
semántica de esas auxiliares. Se inyectan RAM/registros, se enmascaran las IRQ
y se atiende el watchdog entre llamadas; se detiene antes del RTS externo
0x01A5DC. No es una captura de partida ni prueba de temporización de bus.

La traducción preserva ADD.W/ROXR.W y las comparaciones por N del original,
que no equivalen siempre a comparaciones C con signo en presencia de overflow.
En 0x01A684 MOVE.B conserva el byte alto de D0.word: sumas >=256 pueden
leer fuera de los cuatro bytes temporales de condiciones. La lectura indexada
original se conserva y se contrasta con memoria sintética; no se presume que
esos valores aparezcan en una partida.

No está demostrado el dominio de índices durante el juego. Índices >=22 se
rechazan explícitamente como destinos no recuperados. Una auxiliar ausente
produce un estado de error distinto de una condición válida con carry=0.
La integración futura debe enlazar auxiliares nativas y retirar la ABI de
registros/pila cuando sus consumidores estén reconstruidos.

```sh
python3 tools/test_recovered_conditions.py
python3 tools/test_recovered_conditions.py --sanitize
```

Las fixtures reproducen entradas sintéticas y comparan contra resultados
originales almacenados, sin ROM ni MAME. Pasan con UndefinedBehaviorSanitizer.
El módulo compila como objeto MIPS-III big endian para R4300; no se ha
integrado ni probado como campaña en ares.
