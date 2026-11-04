# Observación original en MAME

Captura con BIOS MVS euro y entradas de mando, sin inyección de RAM o registros
de CPU. Se observa la ejecución de 10FC00 dos veces y el destino IRQ 002850;
100000 y el despachador 10081A no se observan en este recorrido.

Los índices conservan hashes de 2.460 volcados RAM privados. La traza cubre un
intervalo parcial de VI tras la primera copia; no todas las activaciones.
El PC interno del debugger y la dirección del breakpoint se guardan separados.
No es una prueba de equivalencia C, replay integral ni campaña N64 completa.

Consultar observations.json y docs/CANONICAL_MODEL.md. El observador ejecutado
está archivado en capture.lua; las órdenes y sus constantes no alteran el
estado del juego, salvo las entradas de mando programadas.
