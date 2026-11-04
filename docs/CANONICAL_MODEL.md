# Modelo de memoria y raíces de ejecución

El punto 1 del plan está **en curso**. Se han construido contextos separados,
contrastado sus reglas de selección y comparado la decodificación estática
con MAME 0.276. Esto no completa la
decompilación ni certifica que las instrucciones propuestas por Ghidra sean
código ejecutado. Los resultados están en `decomp/canonical-model/`.

## Verificado frente al dispositivo original de MAME

- P1 fijo y cuatro bancos físicos P2 mantienen identidades distintas, aunque
  los bancos ocupen las mismas direcciones CPU 200000..2FFFFF. Se contrastaron
  los ocho valores del selector: 4..7 seleccionan el banco físico 0 con esta ROM.
- La protección del cartucho intercepta **lecturas** 2FFFE0..2FFFEF; esos 16
  bytes se excluyen de la comparación con P2. Las escrituras 2FFFF0..2FFFFF
  seleccionan banco; sus lecturas siguen viendo ROM. No se ha reconstruido
  aquí la semántica completa de la protección.
- El cambio de vectores BIOS/cartucho afecta 000000..00007F. El intervalo
  000080..0000FF permanece en P1. Se contrastaron ambos modos.
- La ventana fija Z80 0000..7FFF puede ver **SM1 BIOS o M1 del juego**, según
  el latch. Se contrastaron ambos modos. SM1 es una dependencia de plataforma,
  no otro programa del juego recuperado.
- Las cuatro ventanas de sonido se contrastaron para los 256 valores de cada
  selector: **1.024 vistas ventana/selector**, no 1.024 partidas ni todos los
  estados combinados de los cuatro selectores.

Las pruebas escriben selectores de forma controlada en un callback de MAME;
comparan las vistas CPU con los archivos aportados. No prueban ejecución
natural, comportamiento del juego, síntesis de sonido ni fidelidad en N64.
El mapa inicial Z80 usa los selectores establecidos por MAME; no se supone
que esa inicialización sea el estado eléctrico de reset de la consola original.

## Construido como modelo candidato

`layout.json` describe nueve contextos: P1, cuatro bancos P2, dos copias RAM y
dos mapas iniciales Z80. Las copias se importan a sus direcciones de ejecución,
en espacios overlay distintos de la RAM sin versión:

| Contexto | Fuente P1 | CPU | Contenedor |
| --- | --- | --- | --- |
| RAM_COPY_100000 | 001158 | 100000 | 4.426 bytes |
| RAM_COPY_10FC00 | 0010EE | 10FC00 | 106 bytes |

El modelo contiene instrucciones y propietarios candidatos para los tres
destinos de vectores 0022A2, 0022AC, 002322 y ambas entradas RAM. En total se
sembraron quince raíces de contexto. No se ha declarado ejecutable todo el
contenido de los contenedores: pueden mezclar código y datos.

Los cuatro bancos P2 se importan sin permiso de ejecución para evitar que el
análisis convierta automáticamente datos en funciones. Esto **no demuestra**
que P2 carezca de código. El grafo nuevo tampoco sustituye al catálogo previo
de miles de funciones candidatas: propaga estas raíces en proyectos nuevos.

Ghidra usa el lenguaje MC68020 disponible como superset. El contraste estático
descrito abajo coincide en límites/tamaños y nombres de operación; sus
propuestas no se publican como C nativo verificado.
`frontier.json` enumera 39 transferencias calculadas o sin propietario en este
grafo, incluidos imports y referencias a RAM sin versión. Algunas pueden tener
evidencia parcial en otros artefactos; la lista no es un total global pendiente.

## Decodificación y estados de descriptores RAM

`decoder/validation.json` y las salidas `.asm` conservan la comparación de
**13.463 instrucciones candidatas** con los decodificadores M68000/Z80 de
MAME. Coinciden todos los límites y tamaños y no hay diferencias de mnemónico
tras normalizar alias. Los operandos se conservan para revisión, sin declarar
equivalencia semántica. Los bytes leídos de cada contexto coinciden con su
imagen esperada; las copias RAM se instalan explícitamente en el ensayo y
**no se ejecutan**. Que una secuencia de bytes pueda decodificarse tampoco
demuestra que sea código en vez de datos.

El lector de descriptores RAM carga `(A4)` en A0 en 100818 y llama a A0 en
10081A. La inicialización guarda 10008C en 10003E y 100158 en 10005A.
Seguir esos destinos y las escrituras constantes posteriores descubre **33
escrituras a `(A4)` y 30 destinos distintos** dentro del contenedor. Se
contrastó también la codificación de cada escritura y su inmediato con MAME.
Los destinos son candidatos del dominio indirecto, todavía no un conjunto
exhaustivo ni una reconstrucción de la lógica de campaña.

`descriptor-seeds.tsv` registra cada origen y pasada de análisis. Las escrituras
100166 y 100630 están en fragmentos sin propietario de función; sus
instrucciones se exportan con propietario `-`. No se inventa una función para
esos fragmentos. Las quince raíces explícitas originales siguen siendo quince;
los nuevos destinos son semillas derivadas, con su evidencia separada.

`ram-lifetimes.json` documenta además dos invalidaciones condicionales de la
otra imagen RAM: 10003E escribe `0010008C` en 10FC00 y 10005A escribe
`00100158` en 10FC20. Ambos intervalos contienen instrucciones de la copia
inicial RAM_COPY_10FC00. Si se ejecuta ese prefijo, sin un clobber intermedio
de registros, no se puede seguir usando aquella imagen como código inmutable.
No se ha probado aquí el orden natural de esas rutas ni todas las escrituras
o interrupciones que afectan a sus vidas.

Durante la preparación se detectó que un número hexadecimal sin prefijo,
como `a6`, podía resolverse como un registro del debugger. Las órdenes `dasm`
ahora usan `0x` explícito y la captura rechaza direcciones fuera del intervalo
solicitado. La captura inicial defectuosa se descartó y no forma parte de la
evidencia publicada.

## Reproducción

Las ROM y los proyectos binarios quedan fuera de Git. Con los binarios
normalizados de `decomp/program-set.json`, las ROM aportadas y los toolchains
instalados en el entorno:

```sh
python3 tools/mame/probe_program_mappings.py --rom-dir /ruta/roms --output /tmp/msx-mappings
```

Conservar `validation.json` como `decomp/canonical-model/mapping-validation.json`
antes de preparar las entradas. Las carpetas de salida deben ser nuevas.

```sh
python3 tools/ghidra/prepare_canonical_model.py --inputs /ruta/binarios-normalizados --bios-zip /ruta/roms/neogeo.zip --output /tmp/msx-context-inputs --layout decomp/canonical-model/layout.json
python3 tools/ghidra/build_canonical_model.py --inputs /tmp/msx-context-inputs --output /tmp/msx-context-model
```

`publish_canonical_model.py --model /tmp/msx-context-model` publica tablas en
directorios de contexto nuevos. Su opción `--output` permite preparar una
publicación aparte: copiar allí `layout.json` y `mapping-validation.json`,
publicar, auditar con `--base` y revisar antes de actualizar las tablas activas.

Para generar la evidencia de decodificación:

```sh
python3 tools/mame/compare_context_decoders.py --rom-dir /ruta/roms --output /tmp/msx-context-decoders
```

Publicar sus `validation.json`, `comparison.tsv` y `assembly/` en
`decomp/canonical-model/decoder/`, conservando la publicación anterior para
revisión. La comprobación de lo publicado no necesita ROM, Ghidra ni red:

```sh
python3 tools/ghidra/audit_canonical_model.py --output decomp/canonical-model/frontier.json
python3 tools/mame/audit_context_decoders.py
python3 tools/mame/catalog_ram_lifetimes.py
python3 tools/audit_decompilation.py
```

Se comprueban hashes, límites, propietarios, raíces, codificación de punteros
y frontera candidata, y se reproduce el catálogo de invalidaciones RAM.
La auditoría global debe seguir terminando con `BLOCKED_INCOMPLETE`: reconoce las
raíces modeladas, pero mantiene abiertos sus contratos nativos y el resto del
programa. Ninguna de estas comprobaciones ejecuta la campaña en ares.

## Puerta de cierre pendiente

Faltan la partición física completa código/datos, destinos indirectos e imports,
versiones y vidas de código RAM, estados de banca Z80 usados en ejecución y
equivalencia de sus contratos nativos. El siguiente trabajo del punto 1 debe
resolver esos dominios; después se cierra el núcleo común del punto 2. No se
genera un ZIP completo ni otra demo parcial a partir de este modelo.
