# Fuentes del port y estado de la descompilación

**Entrega parcial. La descompilación completa del juego NO está terminada.**

Este repositorio/ZIP contiene el código fuente disponible, las herramientas y
las evidencias. No contiene una conversión completa del juego a C/MIPS ni una
primera misión terminada. El estado estructurado es `decomp/coverage.json`.

- `src/`: componentes nativos C del port, con límites documentados por módulo.
- `decomp/recovered/`: recuperaciones manuales con contratos y pruebas.
- `decomp/tentative/`: C automático sin tipos/ABI completos; no se compila.
- `tools/`, `data/`, `docs/`: extractores, fixtures sintéticas y evidencia.

Última prueba MIPS: ares 146 reproduce 447 casos de condiciones con auxiliares
controladas y 96 consultas de jugador más 96 consultas de posiciones contrastadas
con sus rutinas y auxiliares originales.
El guardia 0040AC sí se ejecuta en las referencias de la consulta AAA88.
Esto es verificación de CPU, no campaña ni rendimiento de hardware físico.

Faltan código/datos bancados, destinos indirectos, tipos y ABI pendientes,
conexión de AAA88/01903A con sus productores de datos y servicios nativos, campaña, actores y audio completos.
Los 5168 archivos automáticos del 68000 y 154 del Z80 son material tentativo,
no un porcentaje de juego nativo terminado.

Para la regresión host: `make check-recovered` (Python 3 y compilador C).
Para la regresión MIPS: `make native-logic-regression.z64` con libdragon.
El cartucho de pruebas es interno; no se incluye en el ZIP como demo.
Las ROM de juego, BIOS, SDK, archivos de build y assets locales ignorados
no están incluidos. Una compilación completa del prototipo requiere recuperar
sus assets mediante las herramientas y los archivos originales del usuario.

El ZIP parcial histórico no satisface la entrega completa solicitada. No se
presenta como resultado final ni se genera otra entrega mientras siga faltando
código por recuperar. El trabajo de recuperación permanece en las fuentes.
