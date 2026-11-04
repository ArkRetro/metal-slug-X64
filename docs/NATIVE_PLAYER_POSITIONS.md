# Consulta nativa de posiciones: 01903A

`src/neo_player_positions.c/.h` recupera 01903A..0190BA, la consulta de
habilitación de 01964A y la guardia de objeto 0040AC, sin instrucciones M68000
ni punteros a registros de hardware en el runtime. Entradas y salidas son
estructuras con alineación natural y enteros de ancho fijo.

Direcciones originales verificadas:

- A5+69B8/+69BA: coordenadas de respaldo; A5+4322: habilitación.
- A5-6B3C / A5-6A8C: los dos objetos consultados por la guardia.
- Objeto+7A/+7B selecciona por separado las coordenadas de +3C/+40.
- La firma D7.word es FF00/00FF/FFFF/0000 para P1/P2/ambos/ninguno.
- Si falta un jugador se duplican las coordenadas del otro. Si la consulta
  está deshabilitada, MOVEQ borra D7 completo; habilitada conserva su word alto.
- La comparación final por signo de D7 y las escrituras parciales de D0..D3
  afectan a la ABI. Se conservan los resultados y SR en el contrato transitorio.

96 ensayos controlados MAME ejecutan 01903A y ambas auxiliares reales, sin
interceptar servicios: habilitación 0/1/80, ambos objetos válidos/inválidos,
flags de ejes independientes y coordenadas límite. Coinciden todos los
registros y SR al detenerse antes del RTS. No se compara residuo de pila ni
se demuestra el productor de esos datos en una partida.

Las fixtures originales están en `data/player_positions_fixture.json`.
`native-logic-regression.z64` pasa en ares con 447 condiciones, 96 consultas
AAA88 y 96 consultas de posiciones. También pasa la regresión host con UBSan.
El informe es `docs/native-logic-mips-validation.json`. Las condiciones aún
usan respuestas auxiliares sintéticas en su suite independiente: sigue
pendiente enlazar los dos servicios nativos a sus consumidores de campaña.
Esta traducción no completa la descompilación ni genera un nuevo ZIP de entrega.
