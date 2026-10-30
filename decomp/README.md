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
| 68000, P1 fijo | [Índice de 5.168 cuerpos C](full-p1-index.tsv): 5.085 tentativos sin diagnóstico del descompilador y 83 con diagnósticos. Ambos grupos requieren revisión. |
| Sonido Z80 | [Índice de 154 cuerpos C](full-audio-index.tsv): 130 tentativos y 24 con diagnósticos, únicamente para el mapa inicial. |
| Función pendiente 0x008512 | [Desensamblado M68000](tentative/all_p1/unresolved/rom_008512.asm), sin cuerpo C inventado; timeout del descompilador a 120 segundos. |
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
M1; recuperar 0x008512 y contrastar la semántica con ejecución original.
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
