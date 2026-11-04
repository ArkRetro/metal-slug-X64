# Recuperación del código del juego a C

El objetivo es sustituir la lógica provisional por traducciones nativas del
programa original, conservando física, prioridades y temporización. La salida
automática de un descompilador se mantiene separada de las unidades verificadas
que compila GCC para MIPS. No se declara una conversión completa del juego.

## Primera pasada automática reproducible

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

## Ampliación al conjunto de programa completo

La petición posterior incluye TODO el código del juego. Se inventarían P1,
los cuatro bancos físicos de P2 y M1/Z80: 5.373.952 bytes de programa en total.
No se aplanan las ventanas bancadas ni se presupone que todos esos bytes sean
instrucciones. Véanse `decomp/program-set.json` y `decomp/coverage.json`.

Una búsqueda de firmas sobre todos los offsets pares de P1 reúne 6.145 raíces
candidatas y conserva 28.108 sitios de evidencia. El análisis dirigido posterior
procesa 7.944 entradas y produce 5.169 propietarios: 5.168 tienen C tentativo,
83 de ellos con diagnósticos, y 0x008512 no obtiene C tras 120 segundos.
Hay 2.774 entradas de propietario compartido y una raíz de datos conocida
(0x01A074). Se recuperan 534 entradas de tablas BRA.W; quedan 101 sitios sin
resolver bajo esta política. Los candidatos por firma pueden ser datos y los
límites de función aún requieren revisión: no se declara cobertura completa.

0x008512 contiene un núcleo de E/S y subida de paletas según sus instrucciones.
Se conserva el desensamblado del M68000 de MAME en
`decomp/tentative/all_p1/unresolved/rom_008512.asm`. La salida de una pasada
intermedia sin C no se contabiliza como una traducción válida.

Para Z80 se recuperan nueve tablas de despacho (118 entradas) y se exportan
154 propietarios del mapa inicial: 130 tentativos y 24 con diagnósticos.
Las tablas con prefijo EXX conservan ese intercambio de registros. Que no
queden saltos calculados en esa frontera no cubre todos los bancos de M1,
RAM ejecutable ni efectos hardware. No es un driver nativo de audio terminado.

Los índices globales separan los archivos con diagnósticos; ninguno se marca
como C nativo verificado. El exportador imprime el C de Ghidra sin sustituciones
de caracteres; se comprobó que el empaquetado de los 5.168 archivos P1 conserva
el cuerpo y solo elimina espacios finales. La exportación global se regeneró
desde el proyecto antes de publicarse para corregir el formateador preliminar.
Los hashes y recuentos están en `decomp/full-p1-inventory.json` y
`decomp/audio-inventory.json`. La primera pasada histórica no se suma al total.

La traducción manual y las pruebas de `marco_environment.c` descritas arriba
siguen siendo la evidencia nativa nueva. El C automático global no sustituye
el controlador de campaña ni constituye una demo completa.

## Recuperación manual posterior: 0x008512..0x009B82

La función que agotaba el descompilador ya tiene reconstrucción C en
`decomp/recovered/rom_008512.c/.h`. Los 256 bloques de paletas modificadas
(008540..009738) y las 256 repeticiones de ocho longwords
(009772..009B70) se convierten en bucles, tras comprobar todos sus opcodes,
desplazamientos y máscaras MOVEM contra P1. El borrado de la marca de paleta,
la máscara del primer word, el orden de carga completa antes de escribir y
el word cero final del relleno se conservan. ADDA.W extiende el signo; la
aritmética y las direcciones de bus usan anchos explícitos.

La captura privada `palette-kernel-132` ejecuta las instrucciones originales
con 18 conjuntos de RAM y registros inyectados, interrupciones enmascaradas y
contador 0/0x2000. Se detiene en 009758 antes del RTS. Coinciden los 64 KiB de
RAM, ambos bancos de 8 KiB de paletas, D0..D7/A0..A7, SR, banco seleccionado y
escrituras de watchdog/selección de banco. En MAME el banco se guarda como un
offset de words (0/4096), no como índice 0/1; las escrituras I/O se comparan
aplicando mem_mask a las líneas activas. Las primeras capturas con errores del
harness no se aceptaron como evidencia; el informe publicado procede de 132.

`tools/test_recovered_palette.py` reproduce las entradas sintéticas y compara
el C contra hashes/registros originales almacenados, sin requerir ROM ni
emulador. Pasa también con UndefinedBehaviorSanitizer. El archivo compila como
objeto MIPS-III big endian para R4300, pero no se ha enlazado a una ROM de N64
ni constituye una prueba en ares. El contrato sigue necesitando un backend de
bus y no valida ciclos, orden eléctrico de los words MOVEM, otros valores de
contador ni efectos de una partida. El índice diferencia esta reconstrucción
manual del fallo histórico del descompilador automático.

## Hallazgo inicial de retorno por flags: tabla 0x01A5DE

JSR indexado en 01A5D6 selecciona stubs BRA.W consecutivos de cuatro bytes:
22 entradas visibles y 14 destinos únicos en 01A636..01A7C8. Los tres
consumidores identificados (01A026, 01A2E8, 01A4FA) usan BCS después de llamar
a 01A5CA. Los manejadores escriben CCR.C mediante ORI/ANDI; ese retorno no
aparece en el C automático y algunos cuerpos quedan vacíos incorrectamente.
En esa primera pasada se publicó el mapa y el desensamblado, descartando esos cuerpos como C recuperado.
La tabla está verificada estáticamente; su dominio de índices en ejecución y
las rutinas auxiliares 01903A/0AAA88 siguen pendientes. La reconstrucción
manual posterior se documenta en el apartado siguiente. Véase
`decomp/recovered/condition-dispatch/README.md`.

## Traducción de los 14 manejadores de condiciones

`decomp/recovered/rom_01a5ca.c/.h` reconstruye 01A5CA..01A7F2. El despacho
conserva las 22 entradas conocidas en un switch de lógica del juego, sin
fetch/decode de instrucciones M68000. Expone registros y CCR para recuperar
la ABI, incluyendo escrituras de pila; eso todavía requiere una adaptación
posterior al backend nativo. Los índices no recuperados >=22 y una auxiliar
no disponible producen errores explícitos, distintos de carry=0 válido.

La captura `condition-dispatch-138` compara **447 llamadas** originales con
el C: toda la RAM, D0..D7/A0..A7, SR y el contexto registrado en las entradas auxiliares
(D0, A0, A7, SR y dirección de retorno) coinciden. Cubre las 22 entradas; todas salvo el caso incondicional 10 tienen
resultados observados de carry=0 y carry=1 en estos ensayos. El dominio válido
en partida no está demostrado. Las auxiliares 01903A/0AAA88 se interceptan al
entrar, devolviendo registros sintéticos: no se verifica su implementación.
Se enmascaran IRQ, se atiende el watchdog entre llamadas y se detiene antes
del RTS externo 01A5DC. Las capturas preliminares con resets o etiquetas de
caso ambiguas se rechazaron; solo 138 alimenta las fixtures publicadas.

Se preservan ADD.W, ROXR.W con X, comparación por N (no una comparación C
con signo que introduzca semántica N xor V), SPL/EOR de bytes y las escrituras
parciales de D0/D1/D5/D6/D7. En la suma de contadores, MOVE.B en 01A684
conserva los bits altos del word y la lectura temporal puede salir de los
cuatro flags apilados; se conserva la lectura con el índice original.
A0 y A7 se restauran y los retornos auxiliares se contrastan por dirección.

`tools/test_recovered_conditions.py` verifica las fixtures originales sin ROM
ni MAME y pasa también con UndefinedBehaviorSanitizer. El módulo compila para
R4300/MIPS-III big endian. No está enlazado a campaña ni probado como juego
en ares. Los cuerpos automáticos que perdían CCR permanecen rechazados; el
índice principal dirige ahora al módulo manual, compartido por el despacho
y los 14 destinos, sin inflar el recuento de archivos C.

## Código nativo MIPS: consulta de jugador AAA88

`src/neo_player_query.c/.h` sustituye los accesos originales por dos estructuras
nativas de entrada. Conserva los resultados de los cinco registros y SR hasta
recuperar completamente la ABI de los consumidores. 96 casos controlados
MAME ejecutan AAA88 y su guardia 0040AC reales, sin interceptar auxiliares.
La comparación se detiene en los RTS o antes de TRAP #15: la excepción se
representa como error explícito, sin inventar su manejador ni descartarla.

`native-logic-regression.z64` compila este módulo y el despacho de condiciones
para R4300. En ares 146 pasa 447 casos del despacho y 96 de la consulta. Las
5374 diferencias de bytes RAM de las condiciones proceden del original y se
verifican contra los hashes de salida antes de compilar las fixtures. No se
utiliza el C traducido para generar los resultados esperados. Los servicios de
las condiciones siguen siendo sintéticos en su suite; la prueba no demuestra
su integración con esta consulta ni con campaña. Es un cartucho interno de
regresión de CPU, sin entrega de demo. Informe native-logic-mips-validation.json.

## Consulta de posiciones y auxiliares reales

`src/neo_player_positions.c/.h` traduce 01903A..0190BA y las condiciones de
01964A/0040AC a campos nativos. Conserva fallback por eje, duplicación del
jugador ausente, firma D7.word, registros parciales y flags finales. Se
comparan 96 llamadas con MAME sin auxiliares sintéticas; pasan en host/UBSan
y, compiladas para R4300, en ares junto a las 447 condiciones y 96 AAA88.
No se ha enlazado la consulta a campaña ni recuperado sus productores de
estado global. Véase NATIVE_PLAYER_POSITIONS.md. La cobertura global sigue
siendo incompleta; no se genera otro ZIP como entrega final.

## Conexión real de los servicios y verificación global

`condition_services.c` elimina las respuestas sintéticas del camino integrado
del despacho. 451 llamadas adicionales ejecutan las rutinas originales completas
con sus auxiliares reales y coinciden en RAM, registros y SR, incluidas cuatro
ramas de excepción antes de TRAP #15. Pasan en MIPS/ares y UBSan host.
La suite anterior de 447 casos se conserva independiente, con sus límites.
Sigue pendiente el productor/scheduler de campaña y la recuperación restante.

`tools/audit_decompilation.py` comprueba índices, hashes de fuentes, clasificación
física de código/datos, disposición documentada de flujos pendientes y evidencia
de programa completo. Rechaza el estado actual por entradas automáticas sin
verificar, bancos sin partición exhaustiva y ausencia de validación integral.
Contar funciones o poner complete=true no satisface esos requisitos. El informe
es `decomp/completeness-audit.json`; no se entrega un ZIP como completo.

## Consumidores del despacho

01A01E, 01A2D8 y la parte recuperada de 01A4F2 se traducen en
`condition_consumers.c`. 264 transiciones originales con auxiliares reales
coinciden en RAM, registros, SR y etiqueta de continuación. Pasan en host/UBSan
y MIPS/ares. Se conservan el orden descriptor/avance y la recarga diferida de
+72, y se devuelven límites explícitos antes de sus propietarios pendientes.
Los bloques no se contabilizan como funciones padre completamente recuperadas.
Véase CONDITION_CONSUMERS.md y el mapa de rangos en
`decomp/recovered/condition-consumers/blocks.json`.

## Enfoque por programa completo

Los servicios de descriptores de RAM_COPY_100000 disponen de C para 10082E y
1008D8, más 10080E con bindings 10008C/100158 y fronteras explícitas para lo
pendiente. Coinciden 118 transiciones originales controladas en RAM, registros,
SR y escrituras ordenadas. Incluyen 105 retornos de kernel y 13 fronteras;
pasan host/UBSan y MIPS/ares, incluida una secuencia sin reinyección entre pasos.
El asignador principal de actores 0040BC y la campaña siguen abiertos.
Véase NATIVE_RAM_DESCRIPTORS.md.

RAM_COPY_10FC00 dispone ahora de C estructurado manual, con los 106 bytes de
P1:0010EE como procedencia. Coinciden 48 llamadas originales con imports de
plataforma sintéticos: registros/SR, RAM/NV y flujos completos de accesos.
Pasan host/UBSan y MIPS/ares. El registro distingue el contexto RAM del
contenedor P1; la integración de plataforma y campaña continúa pendiente.
Véase NATIVE_BOOT_SELECTOR.md.

La observación original con entradas de mando confirma dos ejecuciones de la
copia 10FC00 y la llamada indirecta 0022DA -> 002850. La hoja RTS se incorpora
al modelo con bytes P1 contrastados. La copia 100000 sigue sin observarse en
ese recorrido; sus candidatos no se declaran ejecutados. Dos argumentos del
asignador añaden 10015A y 1005E6 al dominio candidato y dan propietarios a los
fragmentos previos. El contraste estático actualizado cubre 13.482 límites y
tamaños. Los contratos nativos siguen abiertos; véase CANONICAL_MODEL.md.

El contraste estático MAME del modelo ampliado coincide en 13.463 límites y
tamaños, sin diferencias de mnemónico normalizado. Se siguen 33 escrituras de
punteros de descriptores RAM a 30 destinos candidatos; dos fuentes siguen sin
propietario de función. También se documentan dos escrituras que invalidarían
otra imagen RAM si se ejecuta ese prefijo. Esto no prueba ejecución natural,
equivalencia C ni completitud; véase CANONICAL_MODEL.md.

Actualización del modelo: se contrastaron con MAME ocho selectores P2, 1.024
vistas de ventanas Z80, ambos modos de vectores y ambos modos de fuente fija
de sonido. Se construyeron nueve contextos con quince raíces, incluidas las
copias RAM y los tres destinos de vector que faltaban. Son modelos candidatos;
la decompilación sigue incompleta. CANONICAL_MODEL.md documenta direcciones,
reproducción y las puertas de cierre pendientes.

Se introduce un grafo reproducible del modelo P1 (funciones, rangos, llamadas,
asignaciones de estado y usos de registros tras retorno), más un registro
curado inicial de componentes nativos. El análisis reconoce consumidores CCR
ya contrastados y detecta ámbitos que el índice de C no representaba bien.
Estas son hipótesis del modelo MC68020 de Ghidra, no una partición verificada.

La inspección de raíces encuentra destinos de vectores 0022A2/0022AC/002322
sin modelo y dos rutas de copia de código a RAM: P1:001158 -> RAM:100000
(4426 bytes) y P1:0010EE -> RAM:10FC00 (106 bytes, hasta el primer word RTS).
Los operandos y el contexto M68000 se conservan como evidencia; la presencia
de la segunda copia en RAM no demuestra toda su ejecución. El siguiente
modelo debe incluir mapeos/vidas de esas copias, además de los bancos.

El catálogo del intérprete de animación contiene trece stubs, con valores
00,04,...,30. El conversor de Marco actual no reconoce 2C, y los servicios
14/30 tienen dominios parcialmente recuperados. El plan prioriza cerrar esos
contratos y el núcleo común, reutilizando `neo_animation.c`, sobre multiplicar
traducciones parciales aisladas. Véase WHOLE_GAME_RECOVERY.md.
