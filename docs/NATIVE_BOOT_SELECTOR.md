# Selector de arranque recuperado en C

`decomp/recovered/ram_10fc00.c` reconstruye manualmente los 106 bytes copiados
de P1:0010EE..001157 a RAM:10FC00..10FC69. Ejecuta lógica C estructurada sin
fetch ni decodificación de instrucciones 68000. Compila para MIPS R4300.
Las direcciones y los valores literales proceden del desensamblado MAME de
esa imagen; los nombres descriptivos de la reconstrucción no son símbolos
recuperados del código fuente original.

## Contrato y comportamiento

El módulo recibe D0..D7, A0..A7 y SR como tipos enteros explícitos. Sus imports
de lectura/escritura reciben direcciones físicas de 24 bits y valores enteros,
sin casts de buffers ROM a estructuras del host. El estado ABI se mantiene
hasta enlazar una política de arranque nativa. No se accede directamente al
hardware Neo Geo desde N64.

- Lee los bytes D00108 y D00047; conserva las partes superiores de D7 y D6.
- Recorre selecciones mediante 380021 y comprueba la firma 4E454F2D en 000100,
  la palabra 000108 y el byte 000114. La coincidencia con 0240 sale antes del
  resto de pruebas. Se conserva la segunda lectura de 000108 cuando procede.
- Conserva la primera selección ordinaria en el byte bajo de D2. Si no hay
  coincidencia, usa el valor de D2 que traía el llamador; no supone cero.
- SUBQ.B altera solo el byte bajo del contador, mientras DBRA decrementa su
  palabra baja. El byte alto de esa palabra puede producir hasta 65.536
  iteraciones; D0 sigue incrementándose como byte con desbordamiento.
- Escribe el resultado en D00058, escribe el byte guardado de D00108 en
  380021 y conserva el orden de las escrituras de control relativas a A1.
- El último MOVE.B determina N/Z y borra V/C. X procede del SUBQ inicial o
  del último ADDQ.B, según la salida. Se conservan los restantes bits de SR.

La función termina en el límite **anterior al RTS de 10FC68**. No incluye el
pop de pila del retorno, interrupciones, temporización ni observación de SR
intermedio por los imports. El ensayo enmascara interrupciones y comprueba el
estado al retorno; ese contrato atómico no certifica todos los contextos del
llamador ni una implementación completa del hardware.

## Evidencia

La captura natural anterior observa dos entradas en 10FC00. El nuevo ensayo
controlado ejecuta los mismos bytes originales y compara 48 casos con C:

- D0..D7, A0..A7 y SR completos.
- Secuencia binaria completa de lecturas/escrituras normalizadas, incluidos
  ancho, dirección y valor; la comparación host es byte a byte.
- Los 64 KiB de RAM de trabajo, incluida la imagen ejecutada, y 64 KiB de NV.
- Salidas tempranas, firmas inválidas, prioridad entre coincidencias,
  lecturas repetidas, fallback no nulo, índices con bit de signo, alias de
  dirección de 24 bits y contadores de 256 y 65.536 iteraciones.

Las cabeceras y los dos bytes leídos de NV son **imports sintéticos**. Las
escrituras de selección se registran, pero MAME recibe físicamente el slot 0
para evitar seleccionar dispositivos inexistentes. El ensayo inyecta RAM,
NV y registros; no es la captura natural ni una validación de una MVS con
varios cartuchos reales. Los taps Lua permanecen referenciados durante toda
la ejecución para impedir que el recolector los retire.

El corpus está en `data/boot_select_fixture.json`. El observador ejecutado,
logs y trazas representativas se conservan en `decomp/recovered/boot-select/`.
Los dumps binarios y la ROM de regresión quedan fuera de Git.

Pasan los 48 casos en host con UBSan y en ares 146 usando un ELF MIPS-III,
big endian. La regresión MIPS comprueba registros/SR, memoria NV, número de
accesos y FNV32 del flujo de imports; la comparación exacta de los flujos
completos se realiza en el ensayo MAME/host. El resultado está en
`docs/boot-select-mips-validation.json`.

```sh
python3 tools/mame/probe_boot_select.py --rom-dir /ruta/roms --output /ruta/nueva-captura
make check-boot-select
make boot-select-regression.z64
python3 tools/test_marco_body_ares.py boot-select-regression.z64 --suite bootselect --output /ruta/nueva-prueba-ares
```

El generador de fixtures verifica los hashes de los fuentes recuperados.
`check-recovered` incluye la nueva comprobación host. El registro nativo usa
CPU, contexto RAM, PC y hash de código; no marca 0010EE como una función
nativa verificada ejecutada desde P1.

## Pendiente

Faltan la política/imports de arranque para N64 y su enlace con los llamadores,
las vidas alternativas de RAM y los contratos de interrupciones/tiempos. La
campaña no llama todavía a este módulo. La auditoría global conserva
`BLOCKED_INCOMPLETE` y el dominio nativo permanece abierto. No se ha generado
otra demo ni un ZIP presentado como decompilación completa.
