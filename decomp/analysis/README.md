# Análisis para recuperación del programa completo

Estos archivos son **modelos y evidencia de análisis**, no código nativo
completo ni un nuevo porcentaje de descompilación.

- `p1-program-model/`: funciones/rangos/relaciones del proyecto Ghidra guardado,
  registros p-code y colas de revisión por dependencias y retorno de valores.
- `native-model-overlay.tsv`: asociaciones curadas con subconjuntos nativos.
- `recovery-worklist.json`: prioridades candidatas y componentes existentes.
- `animation-vm-command-map.json`: 13 entradas estáticas y reconciliación con
  la sintaxis del conversor; no certifica su equivalencia semántica.
- `program-roots.json`: vectores y dos rutas estáticas de copia de código a RAM,
  manteniendo separados sus orígenes físicos y sus PCs de ejecución.
- `root-evidence/`, `animation-vm-evidence/`: contexto M68000 canónico. Incluye
  texto/datos interpretados linealmente en algunos rangos; no todo es código.

Los 5.180 propietarios del modelo no equivalen a funciones verdaderas ni a toda
la ROM. El modelo Ghidra usa MC68020; los destinos, límites e ISA se contrastan
con MAME M68000. Los registros leídos/escritos no son un contrato completo de
llamada ni una prueba de liveness. Las asignaciones de estados son candidatas.

El script `ExportProgramGraph.java` opera en lectura del modelo existente;
`analyze_program_graph.py` genera los informes normalizando campos vacíos a '-'.
`reconcile_native_components.py` valida los enlaces del registro curado.
`catalog_animation_vm.py` y `catalog_program_roots.py` verifican hashes y
operandos exactos contra el P1 aportado. Las ROM, RAM y proyectos Ghidra no se
incluyen en el repositorio. El catálogo de raíces distingue presencia de bytes,
camino de copia estático y ejecución real: no intercambia esas evidencias.

Estrategia: [WHOLE_GAME_RECOVERY](../../docs/WHOLE_GAME_RECOVERY.md).
