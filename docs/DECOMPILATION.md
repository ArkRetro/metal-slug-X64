# Recuperación del código del juego a C

El objetivo es sustituir la lógica provisional por traducciones nativas del
programa original, conservando física, prioridades y temporización. La salida
automática de un descompilador se mantiene separada de las unidades verificadas
que compila GCC para MIPS. No se declara una conversión completa del juego.

## Pasada automática reproducible

`tools/ghidra/export_game_c.py` valida el SHA-1 de P1 aportado y convierte el orden
de sus palabras a la vista CPU. `ExportGameCode.java` recorre semillas previas,
llamadas directas y asignaciones de estados en A6. Protege cadenas/tablas ya
identificadas y registra los saltos indirectos que no resuelve. Los cuerpos,
propietarios compartidos y fallos están en `decomp/`.

Esta pasada exporta 268 cuerpos tentativos desde 395 entradas alcanzadas a partir
de 184 raíces previas. La cola se agota para esa política, no para todo el juego:
quedan tablas indirectas, bancos P2 y el programa de sonido Z80. Tampoco se conoce
la partición completa entre código y datos. Por eso no se calcula un porcentaje
de descompilación ni se presenta el C automático como código compilable de N64.
Los avisos del análisis se cuentan en el inventario, no se descartan como éxito.

## Traducción nueva: transiciones de Marco

`src/marco_environment.c/.h` traduce estáticamente **0x0C3078..0x0C32E0**:
entrada/salida del estado +9A bit 7, despacho de respuestas del entorno, dos
secuencias de movimiento/animación, selección de paleta y petición de efecto.
Los nombres permanecen neutrales hasta recuperar la semántica completa del mundo.

Puntos conservados del original:

- Entradas que continúan en el mismo tick, frente a C1EBE programado para el
  siguiente. La finalización de C31B8 también se programa, no se ejecuta antes.
- D1.byte de 019424 en la rama de daño; no se reutiliza el D0 del despacho.
- `ASR.L` con redondeo hacia negativo y wrapping de 32 bits. No hay float ni
  dependencia del overflow signed de C.
- El long +80 comparte sus bytes altos con +80/+81 del controlador existente;
  se ensambla por shifts y un word +82. No se duplica ni se usa una struct packed.
- Tabla real de cuatro paletas en C3152, selección directa por personaje y
  preservación del orden paleta -> programa -> movimiento -> animación.
- La continuación C271C se dirige a `MMR_FALL`, conservando la velocidad;
  C2706/`MMR_FALL_ACTION` es otra entrada y la sobrescribiría. El router nativo
  también resuelve C1F4A; daño y C160A siguen necesitando propietario.
- El filtro de entrada se ejecuta incluso cuando la consulta posterior evita
  calcular movimiento horizontal. Derecha tiene prioridad si ambas direcciones
  aparecen simultáneamente.

Las consultas AA550/AA4C8/19424/18F16/AAA88/AA5AC, programas convertidos,
colisiones y asignador de efectos conservan contratos explícitos. Un servicio
que falta produce error. C160A, C2956 y las demás continuaciones pertenecen a sus
propietarios nativos; no se ejecutan direcciones originales ni se finge éxito.
La nueva unidad todavía no sustituye al controlador de campaña.

## Evidencia y pruebas

1. Ghidra produce el listado dirigido y el borrador C de las rutinas.
2. `disassemble_program.py` obtiene desensamblado con el **M68000 de MAME** y
   contrasta los bytes seleccionados con P1. En el instante de arranque, 24 bytes
   de vectores bajo 0x80 difieren en la vista CPU; los rangos de código utilizados
   coinciden exactamente. No se confunde esa vista con el archivo ROM completo.
3. `probe_marco_environment.py` ejecuta las instrucciones originales
   C31BE..C31DC con 256 pares de entradas controladas, incluyendo negativos,
   extremos y aleatorios. La sonda inyecta RAM/registros, enmascara interrupciones
   e intercepta el helper siguiente. Es una **sonda aritmética**, no una partida
   ni cobertura de los servicios de mundo.
4. El C nativo coincide con los 256 resultados. Las pruebas adicionales de ramas,
   prioridades, orden de llamadas y servicios ausentes son **sintéticas**.
5. `make check-core`, ASan/UBSan y la compilación MIPS pasan. En este sandbox se
   desactiva LeakSanitizer por su incompatibilidad con ptrace; la unidad no hace
   asignaciones dinámicas. El cartucho interno ejecuta las pruebas en ares.

La primera sonda 113 fue descartada: al reanudar el debugger se omitía el
breakpoint de reentrada y no se cargaban las siguientes entradas. La sonda 114
carga y registra cada entrada en el límite de salida y verifica sus valores.
Solo los resultados de 114 se incluyen en el fixture.

```sh
python3 tools/mame/disassemble_program.py --rom-dir /ruta/roms \
  --output /ruta/nuevo-desensamblado --range c3078:27c --range c1740:ea
python3 tools/mame/probe_marco_environment.py --rom-dir /ruta/roms \
  --output /ruta/nueva-sonda
make check-core marco-environment-regression.z64
python3 tools/test_marco_body_ares.py marco-environment-regression.z64 \
  --suite environment --output /ruta/nueva-prueba-ares
```

`docs/marco-environment-validation.json` registra hashes, alcance y resultados.
El fixture numérico está en `data/marco_environment_fixture.json`. La ROM de
regresión es interna; no se publica como demo del juego.

## Trabajo que sigue pendiente

Resolver las tablas/saltos indirectos, traducir los propietarios de daño/muerte
y acciones especiales, recuperar los servicios de mundo y conectar los estados
al controlador/renderizador de campaña. Soldados, vehículos, jefes y guiones de
misión también tienen cobertura parcial. La exportación actual y el bloque nuevo
no equivalen a tener todo Metal Slug X en C nativo.
