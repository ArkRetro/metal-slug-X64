# Descompilación y traducción nativa

Esta carpeta contiene la **salida preliminar de análisis**, separada del C nativo
compilado por el port. No representa el juego completo convertido a C.

- [tentative/fixed_p1.c](tentative/fixed_p1.c): C tentativo generado por Ghidra.
- [tentative/functions.tsv](tentative/functions.tsv): entrada solicitada, función
  propietaria y resultado del descompilador. Varias entradas comparten cuerpo.
- [tentative/unresolved.tsv](tentative/unresolved.tsv): llamadas/saltos indirectos
  que esta pasada no resuelve.
- [inventory.json](inventory.json): límites, recuentos y hashes reproducibles.

La pasada parte de 184 entradas del análisis anterior y sigue llamadas directas
y asignaciones de rutinas a A6. Recorre 395 entradas y exporta 268 cuerpos;
126 entradas comparten un propietario y una no obtiene función. Hay 17 sitios
indirectos sin resolver en esta frontera. Estos recuentos **no son porcentajes
ni cobertura total del juego**. P2 bancado, tablas indirectas no recuperadas y
el programa Z80 quedan fuera de esta pasada.

El C automático tiene registros `unaff_*`, tipos inferidos, posibles límites de
función erróneos y diagnósticos de instrucciones. Ghidra usa un decodificador
MC68020 de conjunto ampliado; cada traducción nativa se contrasta con el M68000
real de MAME. **No compilar ni enlazar este borrador con la ROM de N64.** El
Makefile solo compila las unidades nativas seleccionadas de `src/`.

El bloque nuevo traducido manualmente está en
[../src/marco_environment.c](../src/marco_environment.c), con su contrato en
[../src/marco_environment.h](../src/marco_environment.h). Usa estados, servicios
y aritmética C de ancho fijo; no ejecuta instrucciones de un M68000 emulado.
Su evidencia y límites se explican en [../docs/DECOMPILATION.md](../docs/DECOMPILATION.md).

Reproducir el análisis requiere la ROM aportada, Ghidra 11.4.2 y JDK 21:

```sh
python3 tools/ghidra/export_game_c.py \
  --rom /ruta/mslugx.zip --seeds decomp/tentative/seeds.txt \
  --output /ruta/nueva-exportacion --limit 1000
```

La lista de semillas incluida fija las entradas de esta pasada; `--research`
permite reunir nuevas raíces desde análisis posteriores.

La salida completa incluye instrucciones, proyecto Ghidra y manifiesto. La ROM,
RAM y trazas permanecen fuera de Git. El C incluido elimina únicamente espacios
al final de línea; el inventario distingue los hashes originales y publicados.
