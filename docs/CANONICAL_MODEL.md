# Modelo de memoria y raíces de ejecución

El punto 1 del plan está **en curso**. Se han construido contextos separados y
contrastado sus reglas de selección con MAME 0.276. Esto no completa la
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

Ghidra usa el lenguaje MC68020 disponible como superset; sus propuestas aún
requieren contraste con el M68000 real. No se publican como C nativo verificado.
`frontier.json` enumera 37 transferencias calculadas o sin propietario en este
grafo, incluidos imports y referencias a RAM sin versión. Algunas pueden tener
evidencia parcial en otros artefactos; la lista no es un total global pendiente.

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
directorios de contexto nuevos; para una regeneración, conservar la publicación
anterior fuera de esos directorios y revisar las diferencias. La comprobación
de lo publicado no necesita ROM, Ghidra ni acceso a red:

```sh
python3 tools/ghidra/audit_canonical_model.py --output decomp/canonical-model/frontier.json
python3 tools/audit_decompilation.py
```

La primera comprueba hashes, límites, propietarios, raíces y frontera candidata.
La segunda debe seguir terminando con `BLOCKED_INCOMPLETE`: ya reconoce las
raíces modeladas, pero mantiene abiertos sus contratos nativos y el resto del
programa. Ninguna de estas comprobaciones ejecuta la campaña en ares.

## Puerta de cierre pendiente

Faltan la partición física completa código/datos, destinos indirectos e imports,
versiones y vidas de código RAM, estados de banca Z80 usados en ejecución y
equivalencia de sus contratos nativos. El siguiente trabajo del punto 1 debe
resolver esos dominios; después se cierra el núcleo común del punto 2. No se
genera un ZIP completo ni otra demo parcial a partir de este modelo.
