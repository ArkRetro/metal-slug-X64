# Condiciones con servicios nativos reales

`decomp/recovered/condition_services.c` enlaza el despacho 01A5CA con
`neo_player_query` (AAA88/0040AC) y `neo_player_positions` (01903A/01964A/0040AC).
Las respuestas proceden de esos módulos C: no de valores prefijados ni de
interceptar auxiliares. Se mantiene el contrato de memoria y pila del C
recuperado mientras se reconstruyen los productores nativos de campaña.
No hay fetch/decode de instrucciones M68000.

451 llamadas originales ejecutan el despacho, manejadores y auxiliares reales
en MAME. Coinciden los 64 KiB de RAM, D0..D7/A0..A7 y SR. Incluyen cuatro casos
que se detienen antes de TRAP #15; el puente comunica esa excepción sin
convertirla en una condición falsa. El último retorno interno escrito en pila
se conserva como dato, sin afirmar equivalencia temporal del bus. Las muestras
están en `data/native_conditions_fixture.json`; no es una partida natural.

La regresión MIPS ejecutada en ares pasa las 451 llamadas integradas, además de
las 447 pruebas anteriores con auxiliares controladas y las 96+96 pruebas de
las consultas individuales. El generador verifica los deltas RAM originales
contra sus SHA-256 antes de producir C. UBSan también pasa en host.

```sh
make check-recovered
make native-logic-regression.z64
python3 tools/test_marco_body_ares.py native-logic-regression.z64 \
  --suite nativelogic --output /ruta/nueva-prueba
```

Faltan el productor de datos globales/actores, su scheduler y el resto del
intérprete de eventos. Los índices >=22 siguen sin dominio original demostrado.
La interfaz de memoria de esta recuperación no equivale a integración en
campaña ni a descompilar completamente el juego.
