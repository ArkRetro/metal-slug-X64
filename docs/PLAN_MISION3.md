# Plan de programación de la Misión 3

Fuentes: `MISION3.md` («Reconocimiento del mapa»), `MISION3_ACTORES.md`,
`JEFE_MISION3.md`, `data/mission3_actors.json`. Cada paso termina con una ROM
de prueba (prueba corta de la parte tocada, sin pruebas de la Misión 1).

| Paso | Contenido | Resultado esperado |
| --- | --- | --- |
| 1 | Suelos puestos por actores: fusionar `work-m3c` (cajas y salientes, quitando su desfase de filas), los cuatro estampadores de `038D1E/038D26`, la plataforma `0B9286` con su mapa dinámico `0E6896`; carga de la M3 tras la M2 con la rejilla nueva, el tren y la cámara | Marco recorre la M3 entera de principio a fin (sin enemigos) |
| 2 | Enemigos del camino más frecuentes: `09BA2A`, `08A732`, `08DA12`, `05D916`, `04E990`, `097FCC`, `0569xx` | La misión se juega con sus oleadas principales |
| 3 | Avión del jefe intermedio `0A4A94` (cuerpo, cañones, bodega, final con 30 premios) | Jefe intermedio completo |
| 4 | Jefe final `04A2C2` en las 7 piezas de `JEFE_MISION3.md` (familias m3boss1..5) y fin de misión `018F92`/`018F40` | La M3 termina y pasa a la M4 |
| 5 | Vehículos `093C9A` y `0AC866`; armas en la forma `+93 = 2`; arma E | Vehículos y armas de la M3 |
| 6 | Resto de actores, premios, prisioneros y ramas al azar | Inventario completo |
