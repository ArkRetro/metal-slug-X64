# Descriptores de la copia RAM en C

`decomp/recovered/ram_descriptors.c` recupera manualmente tres servicios de la
imagen inicial RAM_COPY_100000:

| Entrada RAM | Servicio reconstruido | Límite de esta versión |
| --- | --- | --- |
| 10082E | Inserción por prioridad y asignación de descriptor | Antes de los RTS 100834/1008D6 |
| 1008D8 | Desenlace del descriptor y actualización de metadatos | Antes de RTS 100914 |
| 10080E | Recorrido y despacho de descriptores | Callbacks 10008C/100158 y fronteras explícitas |

Las direcciones y operaciones proceden del desensamblado MAME publicado en
`decomp/canonical-model/decoder/`. La imagen se copia desde P1:001158 y tiene
4.426 bytes, incluyendo datos. Recuperar estos servicios no clasifica todo
el contenedor como código ni completa sus 32 destinos candidatos.

Este módulo corresponde a los descriptores de esa copia RAM. El asignador de
actores principal 0040BC y el scheduler de campaña son ámbitos diferentes que
continúan abiertos. Los nombres del C son descriptivos, no símbolos originales.

## Contratos conservados

- Los enlaces de un byte sustituyen el byte bajo de un puntero; no se
  reinterpretan como índices multiplicados por el tamaño de una estructura.
- La inserción compara prioridades como bytes sin signo y conserva el orden
  de lecturas y escrituras cuando coinciden direcciones.
- Con contador disponible cero, 10082E conserva A1 y retorna tras TST.B.
  No se inventa un puntero NULL ni un booleano de éxito. Una asignación que
  consume la última unidad también puede devolver Z=1: el flag no basta para
  interpretar el resultado sin conocer el contrato del llamador.
- Los MOVEM guardan y restauran registros a través de la RAM, incluyendo las
  modificaciones que una dirección solapada haya hecho en esos valores.
  En la referencia se observa palabra baja primero para esos guardados con
  predecremento, y palabra alta primero para el retorno que escribe JSR.
- El contador disponible se vuelve a leer antes de su decremento final.
  Se conserva el efecto de que el nuevo descriptor solape los metadatos.
- El cursor de asignación se limpia cuando el resultado es >=7; el de retirada
  se limpia cuando es exactamente 7. No se sustituyen ambas reglas por módulo 7.
- Se mantienen las anchuras de byte/palabra, wrapping y CCR, incluidos X y V.
  Restaurar D0 desde pila no recalcula los flags de la retirada.
- El despachador calcula A4 con el A5 entrante antes de fijar A5=108000. Lee el
  siguiente enlace después del callback. Se contrastan alias en el byte alto
  de punteros de datos y callbacks; los mirrors RAM y alias de PC no se declaran cerrados.

La interfaz utiliza enteros explícitos y accesos a RAM mediante callbacks;
no hace casts de bytes ROM a estructuras ni interpreta instrucciones 68000.
El C se compila directamente para MIPS. La ABI de registros/direcciones es
un puente mientras se reconstruyen los llamadores y los productores nativos.

## Callbacks y continuaciones

Se enlazan en C la inicialización 10008C y la rutina RTS 100158. El primero
limpia sus campos y cambia el estado a 1000A4. Los demás destinos devuelven
una frontera pendiente antes del JSR de 10081A, sin fabricar una respuesta.

Se conserva el retorno escrito en RAM por JSR. Si la inicialización lo altera
por solapamiento, se devuelve una frontera antes del RTS del callback, con el
destino leído y SP sin consumirlo. Estas fronteras son resultados del puente
de recuperación, no errores ni retornos que se atribuyan al programa original.

Los accesos deben ser a RAM de datos pura, las palabras deben estar alineadas
y los recorridos deben terminar. No se ha probado que todos los llamadores
originales satisfagan esos dominios. No se modelan interrupciones, efectos
observables de lecturas de RAM, flags intermedios, ciclos de bus ni el pop del
RTS externo. Las escrituras ordenadas sí se contrastan con MAME.

## Validación original y MIPS

El corpus contiene 118 transiciones originales: 64 de asignación, 33 de
retirada y 21 del despachador. Hay 105 retornos de kernel, 10 fronteras antes
de callbacks pendientes y tres antes de retornos alterados. No son 118
callbacks ni funciones padre completas.

MAME ejecuta las rutinas y los callbacks originales sin interceptar auxiliares
para devolver resultados sintéticos. Se inyectan RAM y registros iniciales y
se enmascaran IRQ. Se comparan todos los registros/SR, los 64 KiB de RAM y
la secuencia binaria completa de escrituras. Los casos incluyen prioridades
iguales, recorridos en ambos sentidos, metadatos extremos, ausencia de espacio,
alias de direcciones, solapamientos con pila/metadatos y retornos modificados.
Estos estados controlados no se declaran todos estados naturales del juego.

Los tres últimos pasos forman asignación -> despacho -> siguiente despacho.
RAM y registros proceden del resultado real anterior, sin reinyección entre
pasos. El último alcanza 1000A4 y se detiene antes de ejecutarlo: ese callback
es el siguiente trabajo pendiente, no un no-op.

Pasan host con UBSan y ares 146 con ELF MIPS-III big endian. La prueba MIPS
conserva la secuencia encadenada y verifica registros, SR, RAM y número/FNV32
de escrituras; la comparación exacta del flujo completo se hace en MAME/host.
Los deltas se verifican contra SHA-256 antes de generar el fixture C.
La imagen ROM original se excluye del ejecutable de prueba MIPS: sus accesos
de datos rechazan esa región y los hashes normalizados conservan esta distinción.

```sh
python3 tools/mame/probe_ram_descriptors.py --rom-dir /ruta/roms --output /ruta/nueva-captura
make check-ram-descriptors
make ram-descriptors-regression.z64
python3 tools/test_marco_body_ares.py ram-descriptors-regression.z64 --suite descriptors --output /ruta/nueva-prueba-ares
```

Para revisar otro C contra la misma captura original, el probe admite
`--reuse-capture`, verificando las mismas entradas y la terminación del ensayo.
No vuelve a ejecutar MAME ni sustituye los resultados originales.

Los fixtures están en `data/ram_descriptors_fixture.json`, la evidencia en
`decomp/recovered/ram-descriptors/` y el resultado MIPS en
`docs/ram-descriptors-mips-validation.json`. Los dumps binarios quedan privados.

Faltan el resto de callbacks/servicios, los dominios de los productores,
las vidas de RAM y la integración de campaña. La auditoría global sigue en
`BLOCKED_INCOMPLETE`. No se publica una demo ni una decompilación completa.
