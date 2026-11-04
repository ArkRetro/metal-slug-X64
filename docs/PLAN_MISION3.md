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

## Estado

- **Paso 1: hecho** (rama `work-m3step1`, [MISION3.md](MISION3.md) «Paso 1»).
  `work-m3c` fusionada sin desfase de filas; vagones `038D1E/038D26` (puertas,
  blancos, piezas, 4 estampadores), plataforma `0B9286` (mapa `0E6896`),
  `0B93B4` y `0B9400` nativos; familias `m3wagon` (3515 actualizaciones) y
  `m3platform` (14 506) iguales a MAME en `make check`. La M3 carga tras la M2
  con la rejilla nueva, el tren y el bloque `MSXR` (mapas dinámicos). En ares
  (`mslugx-m3-reference.z64`, `test_mission3_ares.py --hop --pilot`) Marco
  cruza las 5 secciones hasta la arena del jefe con 0 rescates y 0 caídas
  (con el mando ciego `--hop`, 0 o 1 rescate según la pasada). Paradas
  explícitas: la entrada en paracaídas (tipo 2), `0B9748` (puntos), los
  enemigos (registros saltados), el guion en `2DB148`.
- **Paso 2: en curso** (rama `work-m3step2`, [MISION3.md](MISION3.md) «Paso
  2»). Nativos en `neo_m3enemy.c`: la muerte de soldado `0406B6` sobre un
  nodo genérico (compartida), el granadero `08A732` (casco, obús, destello,
  humo) y los soldados del escudo `09AE2C/09AE1E/09BA2A` (escudo `09C46C`,
  capa, disparos); familias `m3grenade`, `m3grenade4`, `m3shield`,
  `m3shield4` iguales a MAME (4180 actualizaciones). Faltan `08DA12`,
  `05D916` (y `05C53C/05D226`), `04E990`, `097FCC`, `0569xx`; la pieza común
  que falta es convertir un nodo genérico en el soldado nativo (`0B05FE`,
  `0B0626`, `0B0632`: el escudo roto, `08DA12`, `05D916`).
