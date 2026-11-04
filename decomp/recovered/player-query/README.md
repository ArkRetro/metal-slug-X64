# Consulta de jugador nativa: 0x0AAA88

[src/neo_player_query.c](../../../src/neo_player_query.c) traduce
0x0AAA88..0x0AAADE con la validación de objeto de 0x0040AC. Usa estructuras
con alineación natural y enteros de ancho fijo; no ejecuta direcciones Neo Geo
ni lee una ROM en tiempo de ejecución. Los campos deben proceder del mundo
nativo. El formato de entrada no es una estructura packed de RAM original.

96 casos ejecutan ambas rutinas originales en MAME sin sustituir auxiliares:
selección por byte bajo, objeto inválido con cabecera 0x00003F1E, tabla inactiva,
estados de byte positivos/negativos y conservación parcial de registros.
Se comparan los registros y SR a la salida o justo antes de TRAP #15.
La excepción se devuelve como estado explícito; no se ejecuta ni se afirma
haber recuperado su manejador original. No se compara el residuo de pila.
Las entradas/salidas sintéticas están en `data/player_query_fixture.json`.

El C se compila con GCC MIPS y pasa en ares junto al despacho de condiciones:
447 casos de condiciones y 96 consultas. Las condiciones siguen usando sus
respuestas auxiliares sintéticas para conservar la regresión independiente;
la nueva consulta no se declara integrada en ese despacho ni en campaña.
Informe: `docs/native-logic-mips-validation.json`.

```sh
make check-recovered
make native-logic-regression.z64
python3 tools/test_marco_body_ares.py native-logic-regression.z64 \
  --suite nativelogic --output /ruta/nueva-prueba
```

El cartucho de regresión es interno, no una demo. La cabecera de pruebas se
regenera desde fixtures públicas, sin necesitar ROM. Los cambios de RAM de
los casos de condiciones son deltas sintéticos de las salidas originales;
el generador comprueba sus SHA-256 antes de producir los arrays C.
