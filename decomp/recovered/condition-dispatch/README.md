# Tabla de condiciones: retorno en CCR.C

**Mapa estático recuperado; traducción C pendiente.** El despacho de
0x01A5CA lee un byte de A1, lo multiplica por cuatro y ejecuta el JSR indexado
de 0x01A5D6 sobre los stubs en 0x01A5DE. Hay 22 BRA.W consecutivos, con
14 destinos únicos. [table.tsv](table.tsv) conserva los destinos comprobados
contra P1; [rom_01a5ca.asm](rom_01a5ca.asm) procede del decodificador M68000
de MAME y sus bytes seleccionados coinciden con la ROM aportada.

Los consumidores de 0x01A026, 0x01A2E8 y 0x01A4FA usan BCS. Los manejadores
producen ese resultado con ORI/ANDI sobre CCR. Ghidra elimina parte del cálculo
al inferir funciones void; por ejemplo, genera cuerpos aparentemente vacíos
para 0x01A636 y 0x01A644, aunque contienen trabajo y comparaciones reales.
Los 14 cuerpos automáticos no se publican como C recuperado.
[owners.tsv](owners.tsv) registra ese rechazo explícitamente.

La traducción necesita un retorno de condición explícito y contratos para
0x01903A y 0x0AAA88. Debe conservar ADD.W/ROXR.W, comparaciones por signo,
cambios parciales de registros y la restauración de A0. En 0x01A684 MOVE.B
solo sustituye el byte bajo de D0: no debe darse por hecho que el índice de
condición es siempre un byte limpio después de sumar los contadores.
Tampoco se ha demostrado el dominio válido de los índices en una partida.
No se han ejecutado estos manejadores para verificar su semántica.
