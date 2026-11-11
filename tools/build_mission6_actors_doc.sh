#!/bin/sh
# docs/MISION6_ACTORES.md and data/mission6_actors.json from the three stage captures of
# tools/capture_m6.sh (docs/MISION6.md). Needs build/host/script-native (make check builds it):
# the port column is the state of the tree it was built from.
# Usage: tools/build_mission6_actors_doc.sh [CAPTURES=/home/user/captures-m6]
set -eu
# Always the tree this script belongs to (not the caller's directory): data/ and docs/ are written here.
cd "$(dirname "$0")/.."
C=${1:-/home/user/captures-m6};T=$(mktemp -d)
cat $C/s9/debug.log $C/s10/debug.log $C/s11/debug.log | grep -v '^LATCH\|^STAMP2' > $T/debug.log
python3 tools/compare_m3_events.py $T/debug.log --script data/mission6_script.json \
 --sections "1:0:costa,2:0BDCE2:fabrica,3:0BE99E:final" \
 --source "MAME 0.264 mslugx euro, tools/capture_m6.sh from states4 sec09_292683 / sec10_304271 / sec11_322339 (frame-exact replays of run4) 292683..358820; tools/compare_m3_events.py --sections" \
 --json data/mission6_actors.json --markdown > $T/chain.md
python3 tools/report_mission_actors.py 6 --names 1:costa,2:fabrica,3:final --previous data/mission5_actors.json data/mission4_actors.json data/mission3_actors.json > $T/tables.md
{
cat <<'EOF'
# Misión 6: inventario de actores (fase 1)

Fuente: `data/mission6_actors.json` (MAME 0.264, `tools/capture_m6.sh` desde `states4/sec09_292683`, `sec10_304271` y
`sec11_322339`, réplicas fotograma a fotograma de `run4`, 292683..358820: las etapas 1 y 2 enteras y la etapa 3 hasta el
final de `run4`, que **no vence al último enemigo**; `tools/compare_m3_events.py --sections`, tablas de
`tools/report_mission_actors.py`; se regenera con `tools/build_mission6_actors_doc.sh`). Guion:
`data/mission6_script.json` (secciones 9, 10 y 11 de la tabla `0x800`: `2DF678..2E0038`, `2E003A..2E19E4`,
`2E19E6..2E1A02`; 538 sentencias, 46 constructores). Estado del port: `build/host/script-native`
(`neo_world_spawn_native`) del árbol en que se generó; lo que porten las fases 2 de la M4 y la M5 cambia de columna al
regenerar. El final y los créditos (TAS, sin escrituras) están en [MISION6.md](MISION6.md), «Final y créditos».

Regla: un actor sin portar es una parada explícita contada (`world unsupported routine=...`, registro saltado con su
dirección en el log, o `MSX script stopped at ...` en el primer registro contado sin portar); nunca se oculta.

Las «secciones» de este inventario son las tres etapas (cada una con su lista de cámara: `0BCA00`, `0BDCE2`,
`0BE99E`); la columna «Cámara» es el progreso `10E99E` de la etapa en la primera creación.

EOF
cat $T/tables.md
cat <<'EOF'

## Vehículos, jefes y fin de etapa

- **Etapa 1 (costa y puente)**: el actor de misión (lista `0BCA00`, 18 llamadas) crea en la cabeza los actores propios
  `0BCE2C`, `0BD18E`, `0BD2BA` (+`0BCFA4`/`0BCFC4`), `0BD2CE` (+`0BCFE4`/`0BD004`), `0BD822`/`0BD836`/`0BD84A`
  (+`0BD8F4`/`0BD906`/`0BD918`: comprueban al jugador con `01B332` y suben o bajan) y `0BDA4E`×3, y en la cámara 32
  `0BDAB8` (registra el cuerpo `2D3FEE` con `0260C8`). **Sin ellos Marco cae al agua** (celdas `A0` del mapa, columnas
  20..43 y 66..105 de la rejilla): en MAME el jugador cruza esos tramos a la altura de la orilla sin leer ninguna celda
  de suelo, así que lo sostiene un actor; cuál de ellos es lo primero que hay que trazar en la fase 2.
- **Segundo objeto de terreno `10D7B6`** (mapa `0BC6E6`, solo colisión, columnas 88..147): los cables del puente; su
  dueño mueve el origen de columnas ([MISION6.md](MISION6.md), pendiente 2).
- **Jefe intermedio de la etapa 1**: `0530F4` + `05310E` (16 constructores, 140 creaciones) en 296857, cámara 2352, con
  la lista retenida allí (límites (0, −200) y parada del guion); sin portar. Estados: `at_296000` (antes), `at_298500`,
  `at_301000`.
- **Etapa 2 (fábrica)**: `0A2EFA` (cámara 2672, 313313) y `08FF7E` ×2 (cámara 3872, 320043: 4 constructores) son sus
  encuentros grandes; el bucle del pasillo es el actor `0B0B94` (lista, progreso 1792). Estados: `at_312000`, `at_318000`,
  `at_321000`.
- **Fin de etapa** (`018F92`): en las etapas 1 y 2 lo llama `0B6ED2` (la salida que ya usan la M2 y la M5,
  `neo_goarrow.c`) en 304151 (cámara 3632) y 322152 (cámara 4240); en la etapa 3 el propio último enemigo (TAS:
  fotograma 57038).
- **Último enemigo (etapa 3)**: el registro del guion crea `04153E` (familia de 45+ constructores: `04157A`, `042BDC`,
  `0444E8`, `045428`, `0436B6`, la familia `037404..038702` de sus soldados, `04445A` el deslizamiento de la cámara,
  `0465FE`, `046112`…). Música: `0415CC` pide `0x2C` (324634) y `0417B8` pide `0x29` (348849). `run4` pelea 36 000
  fotogramas sin vencerlo; el TAS lo vence en la actualización 26423. Estados: `at_323500`, `at_325000`, `at_330000`,
  `at_340000`, `at_350000` (`run4`), `u23580..u26400` (TAS) y `end26480` (tras su muerte).

## Plan de la fase 2

1. Etapa 1, por orden de cámara: los actores de la cabeza (arriba) y qué sostiene al jugador sobre el agua; después
   `01F836`/`01F844` (misiles), `020166`, el segundo terreno `10D7B6` con su dueño, `0BD416..0BD4D4`, y el jefe
   intermedio `05310E`.
2. Etapa 2: el registro donde se para hoy el guion (`2E03DE`), `086712`/`086728` (100 creaciones), `0BE02C..0BE460`
   (actores propios de la fábrica), los 4 parches `0C70FC` del submapa `0BDBB2` y los 4 sellos `0C88B0`, `0A2EFA` y
   `08FF7E`.
3. Etapa 3: la familia `04153E` entera (lo más grande de la misión), con `04445A` sobre `neo_camera_place_*`.
4. Final y créditos ([MISION6.md](MISION6.md)): `04677A`, `0570A0`, la sección 22 y `0646FE..0648B6`.
5. Los registros que `run4` no ejecutó (ramas `si`/azar): captura propia desde los estados `at_*` o desde el TAS
   (`sync264`, U 18679..26423), que recorre otras ramas.

## Cadena de sucesos por etapa (`run4`)

Una fila por orden bloqueante del guion principal (fotograma y cámara en que empezó, cuánto retuvo) y lo que corrió hasta
la siguiente: registros op 7 (registro → constructor; `*` = sin portar), elecciones al azar, condiciones, corredores,
creaciones del actor de misión, fin.
EOF
cat $T/chain.md
} > docs/MISION6_ACTORES.md
rm -f "$T/debug.log" "$T/chain.md" "$T/tables.md";rmdir "$T"
