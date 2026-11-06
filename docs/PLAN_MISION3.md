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
  `m3shield4`, `m3window` iguales a MAME (4417 actualizaciones); el soldado
  de ventana `08DA12` y la conversión de un nodo genérico en el soldado
  nativo (`0B05FE/0B0626/0B0632`); los soldados `05D916/05D226/05C53C`
  (paracaidista con su vela, soldado de a pie, casco, balas; `m3para`) y la
  máquina voladora `04E990` (piloto, suelo dinámico `0E112E`, misiles,
  bombas, piezas; `m3pod`), 12 437 actualizaciones más iguales a MAME. En
  ares PASS con 208 ranuras, pero el piloto ciego ya no llega a la arena del
  jefe en 300 s. El helicóptero `097FCC` (`m3gun`, `m3gun4`), los lanzadores
  de molotov `0569B4/0569D0/0569E6` (`m3thrower`) y el bombardero `084588`
  (`m3bomber`), los bidones `0391EA` (`m3drum`), el camión armado `02B202`
  (`m3truck`), la plataforma flotante `08AFB6` (`m3raft`) y el coche con
  pasajero `027F54` (`m3car`, el pasajero pasa a soldado), el jeep armado
  `0A2F2E` (`m3jeep`), los pájaros `03EE8A` (`m3bird`), los sacos `0214EA`
  (`m3sack`) y el campamento `01F1D4` (`m3camp`): 16 823 actualizaciones
  más iguales a MAME. El bloqueo de la cámara en 4380 está resuelto (la ROM
  de referencia pide la muerte general antes de soltar un bloqueo). Paradas
  del mundo resueltas (regalos 28/39, soldados con `+76` = 5/6/7, `08DBC8`,
  granadas de los soldados de ventana, el pasajero convertido): la
  granada `03C306` de los soldados va traducida sin traza. `0B9748`
  (`m3bonus`), las entradas de soldado `0AF17E..0AF1E0` y las acciones
  `0AF92C/0AF940` también. La entrada en paracaídas (`0C15B4`) arranca
  como en la recreativa; las paletas de las familias nuevas están
  exportadas. Faltan `03C306` con traza, `348/3CC/3CF` y `05D234`
  (MISION3.md «Paso 2»).
- **Paso 3: lógica hecha** (rama `work-m3step3`, [JEFE_MISION3.md](JEFE_MISION3.md)
  «Jefe intermedio: estado del port»): `neo_m3mid.c`, familias `m3mid1..4`
  (60 191 actualizaciones) iguales a MAME en `make check`; la aeronave se
  crea desde el guion (`2DAE20`) y la capa `10E8C6` se dibuja; los
  programas y datos de la M2 y la M3 van en sus paquetes (`MSXD`): 210
  ranuras con el jefe intermedio dentro.
- **Paso 5: sin empezar** (vehículos `093C9A`/`0AC866`, la forma `+93 = 2`
  —probablemente Marco gordo tras la comida, sin comprobar— y el arma E
  `0DE4DA`): siguen como paradas explícitas (`weapon_stops`, constructores
  externos). Visto: `093C9A` no es una entrada de `neo_tank.c` sino una
  copia aparte del código del SV-001 (`093C9A..094xxx`: paletas
  `0x87D/0x39D/0x87A`, torreta `03B4E0` con `+72 = 4`, sin la comprobación
  del cartucho, rutinas `094A26/094B9A/0949EA/094872/094ABA/094CDA`): hace
  falta traducirla y capturarla como el SV-001 (y caber en la RAM).
