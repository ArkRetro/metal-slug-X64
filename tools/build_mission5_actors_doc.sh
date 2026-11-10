#!/bin/sh
# docs/MISION5_ACTORES.md and data/mission5_actors.json from the three stage captures of
# tools/capture_m5.sh (docs/MISION5.md). Needs build/host/script-native (make check builds it):
# the port column is the state of the tree it was built from.
# Usage: tools/build_mission5_actors_doc.sh [CAPTURES=/home/user/captures-m5]
set -eu
C=${1:-/home/user/captures-m5};T=$(mktemp -d)
cat $C/s6/debug.log $C/s7/debug.log $C/s8/debug.log > $T/debug.log
python3 tools/compare_m3_events.py $T/debug.log --script data/mission5_script.json \
 --sections "1:0:calle,2:0BB9BA:metro,3:0BBA24:alcantarilla" \
 --source "MAME 0.264 mslugx euro, tools/capture_m5.sh from states4 sec06_247661 / sec07_252827 / sec08_274387 (frame-exact replays of run4) 247661..292680; tools/compare_m3_events.py --sections" \
 --json data/mission5_actors.json --markdown > $T/chain.md
python3 tools/report_mission_actors.py 5 --names 1:calle,2:metro,3:alcantarilla --previous data/mission4_actors.json data/mission3_actors.json > $T/tables.md
{
cat <<'EOF'
# Misión 5: inventario de actores (fase 1)

Fuente: `data/mission5_actors.json` (MAME 0.264, `tools/capture_m5.sh` desde `states4/sec06_247661`, `sec07_252827` y
`sec08_274387`, réplicas fotograma a fotograma de `run4`, 247661..292680: las tres etapas enteras, el jefe y el fin de
misión; `tools/compare_m3_events.py --sections`, tablas de `tools/report_mission_actors.py`; se regenera con
`tools/build_mission5_actors_doc.sh`). Guion: `data/mission5_script.json` (secciones 6, 7 y 8 de la tabla `0x800`:
`2DCB3C..2DDB00`, `2DDB02..2DE832`, `2DE834..2DF676`; 542 sentencias, 444 registros de aparición, 49 constructores).
Estado del port: `build/host/script-native` (`neo_world_spawn_native`) del árbol en que se generó; lo que porte la
fase 2 de la M4 cambia de columna al regenerar.

Regla: un actor sin portar es una parada explícita contada (`world unsupported routine=...`, registro saltado con su
dirección en el log, o `MSX script stopped at ...` en el primer registro contado sin portar); nunca se oculta.

Las «secciones» de este inventario son las tres etapas (cada una con su lista de cámara: `0BB8E6`, `0BB9BA`,
`0BBA24`); la columna «Cámara» es el progreso `10E99E` de la etapa en la primera creación.

EOF
cat $T/tables.md
cat <<'EOF'

## Vehículos, trenes y jefe

- **SV-001 `092A32`** (calle): lo crea el corredor `01A422` con el registro `2DCE10` en 248043 (cámara 280); nativo
  (compartido con la M1). Estado MAME: `captures-m5/states/mslugx/sec06_247661` (382 fotogramas antes).
- **Slugnoid `09692C`** (alcantarilla): 3 creaciones del guion, la primera en 275061 (cámara 557); nativo (compartido
  con la M2). Es el desplazamiento `0x2FC` de la tabla `0197DC`, con el bit 31 (no cuenta en su grupo 6).
- **Trenes del metro**: familia `051344` (12 constructores, 148 creaciones, primera en la cámara 528) y la familia
  `0D3B36` (14 constructores); sin portar. Estados: `at_256000..272000`.
- **Jefe**: el registro del guion de la cámara 2560 crea `0A0C00` en 277086 (22 constructores, 434 creaciones) y este el
  `059F76` de la música (fundido `0A 80` en 277087, `00 2C` en 277279: el mismo actor que en la M2/M3/M4, ya nativo).
  Fin de misión `018870` en 292010; la cuenta `0308C0` (`neo_tally.c`) nace en 292068. Estados: `at_277000` (86
  fotogramas antes), `at_280000`, `283000`, `286000`, `289000`, `291500`.
- **Fin de etapa** (`018F92`): en la calle lo llama la rutina `0BBE22` en 252722 (cámara 1640); en el metro la rutina
  `0B6ED2` en 274222 (cámara 1608: la salida de etapa que ya usa la M2, `neo_goarrow.c`). Los barridos `03AED8` /
  `03AEF0` del actor de misión son nativos (`neo_wipe.c`).

## Plan de la fase 2

1. Los tres registros donde se para hoy el guion: `2DCB42` → `0BBE5E` (calle, cámara 0; 2 creaciones), `2DDB5E` →
   `0BC1E6` (metro; 7) y `2DE9D0` → `08346E` (alcantarilla; con `0834F4` 717 creaciones: lo más frecuente de la misión).
2. Los actores propios de la M5 (`0BBE5E`, `0BBED0`, `0BBE16`, `0BC1E6`, `0BBF12`, `0BC296`, `0BC4F4`, `0BC3A6`) con
   los estampadores `0C88B0` / `0C8A3C` ([MISION5.md](MISION5.md), pendiente 2).
3. Por frecuencia: `08346E`/`0834F4` (717), `0474B6` + `047F7E` (681 con sus hijos, calle y metro), `0A0C00` (jefe,
   434), `051344` (trenes, 148), `050542` (65), `0836E8` (64), `0AF1A8` (52: ya en el inventario de la M4), `0D3B36` (51).
4. Los registros que `run4` no ejecutó (ramas `si`/azar): captura propia desde los estados `at_*` con otro plan de
   entrada, o desde los estados del TAS (`sync264`, U 12887..18101), que recorre otras ramas.
5. Efectos y voces sin política ([MISION5.md](MISION5.md), «Sonido»).

## Cadena de sucesos por etapa (`run4`)

Una fila por orden bloqueante del guion principal (fotograma y cámara en que empezó, cuánto retuvo) y lo que corrió hasta
la siguiente: registros op 7 (registro → constructor; `*` = sin portar), elecciones al azar, condiciones, corredores,
creaciones del actor de misión, fin.
EOF
cat $T/chain.md
} > docs/MISION5_ACTORES.md
rm -f "$T/debug.log" "$T/chain.md" "$T/tables.md";rmdir "$T"
