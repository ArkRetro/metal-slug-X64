# Misión 3: depuración

Punto de continuidad de la Misión 3. Se actualiza al final de cada sesión.
Referencias: `MISION3.md`, `MISION3_ACTORES.md`, `JEFE_MISION3.md`,
`PLAN_MISION3.md`, `BUG_MATRIX.md` (B18, B56–B58, B59).

## Estado (ROM 41, `ccr-09eb2f6b-8rwl8l`)

- **Carga:** tras la M2, o desde el menú de pruebas (L+R o Z+R y A en
  SOLDIER SELECT). Pack v4 con el bloque MSXD, 796 de 800 trozos.
- **Mapa y colisión:** 100 % frente a MAME en las 5 secciones (`test_m3_map`).
- **Actores:** todos los del recorrido de `run4` antes del jefe son nativos.
  Faltan las ramas al azar no vistas y `03C306` (granada, sin traza).
- **Jefe intermedio y final:** nativos (familias m3mid1..4, m3boss0..5).
  El fin de misión cae en la parada explícita "falta mission4.pack", que
  reinicia la M1.
- **Vehículos:** `093C9A` y `0AC866`. Arma E. Marco gordo y sus armas, salvo
  la bomba, el molotov y el polvo.
- **Fondo:** columnas pre-renderizadas en la capa 1 (`build/bgp/m3s1.bgp`).
  El resto va por la caché de piezas.

## Verificado

- PC: `make check` con todas las familias M3 bit-exact.
- ares: recorridos con la ROM de referencia (piloto, invulnerable) hasta el
  jefe.
- Ojo: la ROM de referencia **escondió** fallos de la ROM normal. Hay que
  probar siempre con `mslugx-explorer.z64` o con `mslugx-m3-release.z64`.

## Fallos abiertos (MiSTer, ROM 41, cartucho normal)

1. ~~**Fondo de la M3 ausente**~~ (B60, corregido en `work-m3doors`). La foto
   (`scratchpad/rom41-photos/m3-black-bg.jpg`) es el **inicio** de la M3
   (cajas, cerdos, fogata con su humo). Experimento: `mslugx-m3-release.z64`
   (juego normal desde el registro de la M3) en ares, cámara 0: mismo negro
   (`MSX prerender: layers 2, ring 94 KB`); sin `m3s1.bgp` (`missing: tile
   path`) también negro; píxeles del fondo `(8,0,0)`. MAME (vídeo 8:04,
   `run4` cámara 0): fondo con cielo, árbol, locomotora. Causa: `map.json`
   toma la primera observación de cada celda y en x 0..304 era un fotograma
   del fundido de entrada (`run4` 117990/118020): 583 celdas con paletas
   negras (`0001`). Arreglo: `extract_mission_map.py --skip-fade` y
   reempaquetado (796 trozos). Ya se veía así en ares el 4-XI (`cap-m3s1/ares7`).
2. ~~**Puertas de los vagones con palanca**~~ (B57, corregido en
   `work-m3doors`): la puerta central es `0391EA`, que solo cae con un golpe
   de tipo 13; se lo da el empujón `0AF2BA` (fila `0AE9B0`, clase 1) de los
   soldados `+76` = 7 de dentro. El port lo comprobaba contra el jugador.
   MAME `run4` 121357 (`104BC4` → `101E64`): `test_neo_m3push.c` igual. En
   ares (referencia, piloto) las dos puertas de los vagones 7-24 se abren y
   salen los soldados.
3. **Tren en el minuto 8:30 del vídeo:** no se reproduce en ares. Se espera
   a que muera el grupo 0..7 y luego el soldado del escudo `09AE2C`. El
   registro de la etapa imprime el tren y los grupos.

## Hipótesis

| Hipótesis | Evidencia | Estado |
| --- | --- | --- |
| H1: el anillo pre-renderizado deja columnas sin dibujar en el hardware | Sin `m3s1.bgp` el camino de piezas da el mismo negro | descartada |
| H2: la sección de la foto usa capas o reglas que el pack no tiene | La foto es la cámara 0, con sus capas en el pack; solo las paletas eran negras | descartada |
| H3: falta de ranuras de caché en ese tramo | Tablas y mapa cargados (774 KB en 49 ranuras, 31 trozos); el dibujo existe, con paletas negras | descartada |
| H4: paletas de las celdas tomadas de un fotograma del fundido | 583 celdas de x 0..304 con paletas `0001`; con `--skip-fade` el fondo es el de MAME | **confirmada, corregida (B60)** |

## Siguiente experimento (uno solo)

Cartucho normal en MiSTer (ROM 42): inicio de la M3 (fondo) y vagones 7-24
(puertas centrales). Después, el punto 3 (tren del minuto 8:30).

## Archivos tocados por la M3

`src/neo_m3props.c`, `neo_m3enemy.c`, `neo_m3mid.c`, `neo_m3veh.c`,
`neo_m3boss.c`, `neo_train.c`, `scene_prerender.c`, `scene_assets.h`,
`tools/pack_mission.py`, `tools/prerender_stage.py`; B60: `filesystem/mission3/s1/map.json` y
`romfs/mission3.pack` (privados); B57: `src/neo_soldier_ai.c/.h`, `src/neo_world.c/.h`,
`tools/test_neo_m3push.c`, `Makefile`.
