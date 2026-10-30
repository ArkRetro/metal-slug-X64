# Recuperación del programa original a C

**La conversión completa todavía no está terminada.** Este directorio contiene
C automático tentativo, separado del código nativo que compila el port de N64.
El estado explícito de cobertura está en [coverage.json](coverage.json).

La entrada incluye las tres ROM de programa: P1 (1 MiB), P2 (4 MiB, cuatro
bancos físicos) y M1/Z80 (128 KiB). El [inventario de entradas](program-set.json)
registra sus hashes, orden de bytes y ventanas bancadas. Inventariar todos los
bytes no demuestra que se haya identificado todo el código ejecutable.

| Resultado | Archivos y límites |
| --- | --- |
| 68000, P1 fijo | [Índice: 5.168 cuerpos automáticos y una recuperación manual](full-p1-index.tsv): 5.085 tentativos sin diagnóstico del descompilador y 83 con diagnósticos. Ambos grupos requieren revisión. |
| Sonido Z80 | [Índice de 154 cuerpos C](full-audio-index.tsv): 130 tentativos y 24 con diagnósticos, únicamente para el mapa inicial. |
| Rutina 0x008512 | [C reconstruido manualmente](recovered/rom_008512.c), contrastado con 18 ejecuciones controladas originales. El descompilador automático sigue fallando. |
| Frontera inicial histórica | [268 cuerpos de la primera pasada](tentative/fixed_p1.c); se conserva para trazabilidad, no se suma al recuento global. |

Cada archivo global identifica la dirección original y su estado. El
[inventario P1](full-p1-inventory.json) documenta 7.944 entradas candidatas
procesadas, 534 entradas de tablas recuperadas y 101 sitios sin resolver.
La búsqueda por firmas puede confundir datos con instrucciones: los recuentos
no son porcentajes de cobertura. 0x01A074 está identificado como datos de
banderas de animación y se excluye como función.

El [inventario de sonido](audio-inventory.json) registra nueve tablas de despacho
con 118 entradas recuperadas. Agotar esa frontera del mapa inicial no verifica
los cambios de banco ni convierte el driver en un backend nativo de sonido.

## Qué falta para tener TODO en C utilizable

Resolver los límites de funciones, tipos, registros implícitos, destinos
indirectos y bancos; distinguir código de datos en P2 y los mapas restantes de
M1 y contrastar la semántica con ejecución original. Recuperar también la ABI
de retorno por CCR de la tabla 0x01A5DE (22 entradas, 14 destinos).
Después hay que sustituir los servicios hardware por backends de libdragon.
El borrador automático conserva símbolos `unaff_*` y dependencias inferidas;
**no se compila ni se enlaza con la ROM de N64**.

La traducción manual nueva está en
[marco_environment.c](../src/marco_environment.c), direcciones
0x0C3078..0x0C32E0. Su aritmética coincide con 256 sondas controladas del M68000
original; los servicios de mundo y su integración en campaña siguen pendientes.
Véase [evidencia y pruebas](../docs/DECOMPILATION.md).

## Herramientas reproducibles

Se requieren las ROM aportadas, Ghidra 11.4.2 y JDK 21. Los scripts están en
[tools/ghidra](../tools/ghidra). `prepare_program_set.py` valida e inventaría P1,
P2 y M1; `discover_program_roots.py` registra las firmas y sitios candidatos;
`export_game_c.py --whole-candidates` analiza la frontera global;
`ExportProgramFrontier.java` recupera también tablas BRA.W.
`export_audio_c.py` y `ExportZ80Code.java` analizan el mapa inicial Z80.
`ReexportExistingC.java` permite volver a exportar los propietarios del proyecto
sin repetir la búsqueda. `package_whole_decomp.py` separa los cuerpos para revisión.

Los [sitios candidatos](candidate-sites.tsv) permiten revisar el origen de las
raíces. El índice distingue propietarios compartidos y fallos. La exportación
conserva los caracteres del C; solo normaliza espacios finales. Se comprobó
esa preservación en los 5.168 cuerpos P1. Los inventarios distinguen los hashes
de artefactos originales privados de los publicados cuando corresponde.
Las ROM binarias, volcados RAM y proyectos Ghidra permanecen fuera de Git.

## Recuperación manual del núcleo de paletas

[rom_008512.c](recovered/rom_008512.c) reconstruye 0x008512..0x009B82 sin
interpretar instrucciones: sustituye dos secuencias desenrolladas por bucles C.
Los 18 casos controlados de MAME coinciden en 64 KiB de RAM, ambos bancos de
paletas, los 16 registros D/A, SR y la secuencia de escrituras de control.
Los opcodes de los 256 bloques de cada secuencia se comprueban contra P1.
El [informe](recovered/rom_008512.validation.json) conserva hashes y límites.

La traducción aún conserva una interfaz explícita de bus Neo Geo; **no está
integrada en libdragon ni valida temporización de bus**. Los casos cubren el
contador 0/0x2000 y se detienen antes del RTS. El C compila para MIPS R4300 y
pasa las fixtures originales en host, también con UndefinedBehaviorSanitizer.

```sh
make check-recovered
python3 tools/test_recovered_palette.py --sanitize
```

La [tabla de condiciones](recovered/condition-dispatch/README.md) documenta otro
pendiente: Ghidra elimina resultados que se devuelven en CCR.C. Sus cuerpos
aparentemente vacíos no se publican como traducciones válidas.
