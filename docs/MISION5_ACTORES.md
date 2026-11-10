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

## Resumen

| | Constructores | Creaciones |
| --- | --- | --- |
| nativos (compartidos con misiones anteriores) | 106 | 16659 |
| nativos en otro módulo (fin de misión, aterrizaje, guion, efectos) | 13 | 130 |
| **nuevos, sin portar** | 103 | 2583 |
| total | 222 | 19372 |

De los 103 sin portar, 1 ya estaban en el inventario de una misión anterior (los porta su fase 2: M4) y **102 son propios de esta misión**.

Secciones (fotograma de `run4` en que empieza cada una): 1 calle desde 247663, 2 metro desde 252828, 3 alcantarilla desde 274388.

Registros del guion que `run4` no ejecutó (ramas `si`/azar; a capturar en la fase 2): 141.

## Nuevos (sin portar): 103 constructores, 2583 creaciones

Por familia (raíz = registro del guion, llamada de la lista de cámara o actor de misión del que descienden), en orden de primera aparición:

- `050542` (6): `050542` `05090A` `05092C` `05094E` `0509A8` `050B52`; sección 1, primera cámara 0, 65 creaciones
- `09624E` (1): `09624E`; sección 1, primera cámara 0, 4 creaciones
- `0BBE5E` (1): `0BBE5E`; sección 1, primera cámara 0, 2 creaciones
- `08DCB2` (2): `08DCB2` `08EB18`; sección 1, primera cámara 408, 2 creaciones
- `08FF7E` (4): `08FF7E` `090A62` `091290` `0912EE`; sección 1, primera cámara 600, 16 creaciones
- `0474B6` (3): `0474B6` `0479B8` `047AFC`; sección 1, primera cámara 880, 388 creaciones
- `0AF1A8` (1): `0AF1A8`; sección 1, primera cámara 880, 52 creaciones
- `047F7E` (3): `047F7E` `047A12` `047B10`; sección 1, primera cámara 1208, 293 creaciones
- `0BBED0` (1): `0BBED0`; sección 1, primera cámara 1584, 1 creaciones
- `0BBE16` (1): `0BBE16`; sección 1, primera cámara 1640, 1 creaciones
- `0BC1E6` (1): `0BC1E6`; sección 2, primera cámara 44, 7 creaciones
- `051344` (12): `051344` `0515CE` `051688` `051690` `051772` `0527A2` `052128` `051D70` `051DF2` `051C00` `051FF4` `051FFE`; sección 2, primera cámara 528, 148 creaciones
- `0D3B36` (14): `0D3B36` `0D4396` `0D43C4` `0D43F2` `0D4420` `0D447C` `0D4762` `0D498C` `0D5018` `0D4A82` `0D4C26` `0D4A1E` `0D4ABA` `0D4F1C`; sección 2, primera cámara 992, 51 creaciones
- `common` (9): `01CDF0` `01CDD8` `01CDE4` `01CDFC` `01CDC0` `01CDCC` `08A264` `0A1F98` `0A2134`; sección 2, primera cámara 992, 90 creaciones
- `player` (3): `0DEA44` `0DEC14` `0DEA20`; sección 2, primera cámara 1280, 172 creaciones
- `0BBF12` (3): `0BBF12` `0BC192` `0BC100`; sección 2, primera cámara 1520, 3 creaciones
- `032B46` (1): `032B46`; sección 3, primera cámara 0, 1 creaciones
- `032B5A` (1): `032B5A`; sección 3, primera cámara 0, 1 creaciones
- `032B6E` (1): `032B6E`; sección 3, primera cámara 0, 1 creaciones
- `095C98` (2): `095C98` `095F8C`; sección 3, primera cámara 0, 8 creaciones
- `0BC296` (1): `0BC296`; sección 3, primera cámara 0, 1 creaciones
- `0BC4F4` (1): `0BC4F4`; sección 3, primera cámara 0, 5 creaciones
- `08346E` (2): `08346E` `0834F4`; sección 3, primera cámara 0, 717 creaciones
- `08996A` (1): `08996A`; sección 3, primera cámara 129, 18 creaciones
- `0836E8` (2): `0836E8` `083820`; sección 3, primera cámara 337, 64 creaciones
- `0BC3A6` (1): `0BC3A6`; sección 3, primera cámara 440, 3 creaciones
- `089948` (1): `089948`; sección 3, primera cámara 545, 19 creaciones
- `08995A` (1): `08995A`; sección 3, primera cámara 1057, 8 creaciones
- `0A0C00` (22): `0A0C00` `0A14FE` `0A167C` `0A1830` `0A21F0` `0A21A8` `0A21CE` `0A1E92` `0A1E7E` `0A1F0E` `0A1958` `0A1EA6` `0A192E` `0A203A` `0A1DF2` `0A1B3A` `0A1BEC` `0A1C0C` `0A1C4C` `0A1D20` `0A1756` `0A18D8`; sección 3, primera cámara 2560, 434 creaciones
- `0308C0` (1): `030FB2`; sección 3, primera cámara 2560, 8 creaciones

### Sección 1 (calle): 21 constructores nuevos que aparecen aquí por primera vez, por cámara

| Constructor | Creaciones | Primer fotograma | Cámara | Por sección | Creadores | Programas | Sonidos | Familia (raíz) |
| --- | --- | --- | --- | --- | --- | --- | --- | --- |
| `050542` | 9 | 247664 | 0 | 1:9 | `script`×9 | 24 | 1021 | 050542 |
| `09624E` | 4 | 247664 | 0 | 1:4 | `script`×4 | 1 |  | 09624E |
| `0BBE5E` | 2 | 247664 | 0 | 1:2 | `script`×2 | 2 |  | 0BBE5E |
| `05090A` | 14 | 247904 | 137 | 1:14 | `050542`×14 | 1 |  | 050542 |
| `05092C` | 14 | 247904 | 137 | 1:14 | `050542`×14 | 1 |  | 050542 |
| `05094E` | 7 | 247904 | 137 | 1:7 | `050542`×7 | 1 | 1025 | 050542 |
| `0509A8` | 13 | 247911 | 143 | 1:13 | `050542`×13 | 6 |  | 050542 |
| `050B52` | 8 | 248028 | 265 | 1:8 | `050542`×8 | 6 |  | 050542 |
| `08DCB2` | 1 | 248181 | 408 | 1:1 | `script`×1 | 3 | 1021 1033 | 08DCB2 |
| `08EB18` | 1 | 248181 | 408 | 1:1 | `08DCB2`×1 | 3 | 1025 | 08DCB2 |
| `08FF7E` | 6 | 248364 | 600 | 1:2 3:4 | `script`×6 | 7 | 1021 1033 108A | 08FF7E |
| `090A62` | 6 | 248364 | 600 | 1:2 3:4 | `08FF7E`×6 | 4 | 1064 | 08FF7E |
| `091290` | 2 | 248714 | 600 | 1:2 | `090A62`×2 | 1 |  | 08FF7E |
| `0912EE` | 2 | 248714 | 600 | 1:2 | `090A62`×2 | 2 | 1025 | 08FF7E |
| `0474B6` | 8 | 249523 | 880 | 1:3 2:5 | `script`×8 | 5 | 1021 1022 10E8 | 0474B6 |
| `0AF1A8` (M4) | 52 | 249614 | 880 | 1:10 2:42 | `01A422`×32, `script`×20 | 23 | 103E 1041 1042 1043 | 0AF1A8 |
| `0479B8` | 190 | 249882 | 880 | 1:22 2:168 | `0474B6`×190 | 1 | 10FF | 0474B6 |
| `047AFC` | 190 | 249887 | 880 | 1:22 2:168 | `0479B8`×190 | 3 |  | 0474B6 |
| `047F7E` | 5 | 250762 | 1208 | 1:1 2:4 | `script`×5 | 9 | 1021 1022 10E8 | 047F7E |
| `0BBED0` | 1 | 251766 | 1584 | 1:1 | `script`×1 | 1 |  | 0BBED0 |
| `0BBE16` | 1 | 252604 | 1640 | 1:1 | `script`×1 | 0 |  | 0BBE16 |

### Sección 2 (metro): 40 constructores nuevos que aparecen aquí por primera vez, por cámara

| Constructor | Creaciones | Primer fotograma | Cámara | Por sección | Creadores | Programas | Sonidos | Familia (raíz) |
| --- | --- | --- | --- | --- | --- | --- | --- | --- |
| `0BC1E6` | 7 | 253146 | 44 | 2:7 | `script`×7 | 2 | 107D | 0BC1E6 |
| `051344` | 4 | 262428 | 528 | 2:4 | `script`×4 | 6 | 1030 1034 1078 107C | 051344 |
| `0515CE` | 4 | 262568 | 528 | 2:4 | `051344`×4 | 7 | 1078 | 051344 |
| `051688` | 4 | 262568 | 528 | 2:4 | `051344`×4 | 1 |  | 051344 |
| `051690` | 4 | 262568 | 528 | 2:4 | `051344`×4 | 1 |  | 051344 |
| `051772` | 64 | 262656 | 528 | 2:64 | `051344`×64 | 8 |  | 051344 |
| `0527A2` | 16 | 262656 | 528 | 2:16 | `051344`×16 | 0 |  | 051344 |
| `052128` | 4 | 262974 | 528 | 2:4 | `051344`×4 | 1 |  | 051344 |
| `051D70` | 12 | 263046 | 528 | 2:12 | `051DF2`×12 | 1 |  | 051344 |
| `051DF2` | 4 | 263046 | 528 | 2:4 | `051344`×4 | 0 | 1022 | 051344 |
| `051C00` | 4 | 263136 | 528 | 2:4 | `051344`×4 | 0 | 1022 | 051344 |
| `051FF4` | 8 | 263176 | 528 | 2:8 | `051344`×8 | 1 |  | 051344 |
| `051FFE` | 20 | 263176 | 528 | 2:20 | `051344`×20 | 5 |  | 051344 |
| `047A12` | 144 | 264353 | 808 | 2:144 | `047F7E`×144 | 1 | 10FF | 047F7E |
| `047B10` | 144 | 264357 | 808 | 2:144 | `047A12`×144 | 3 |  | 047F7E |
| `0D3B36` | 1 | 269327 | 992 | 2:1 | `script`×1 | 6 | 102F 1033 1071 | 0D3B36 |
| `0D4396` | 1 | 269327 | 992 | 2:1 | `0D3B36`×1 | 4 | 1066 | 0D3B36 |
| `0D43C4` | 1 | 269327 | 992 | 2:1 | `0D3B36`×1 | 4 |  | 0D3B36 |
| `0D43F2` | 1 | 269327 | 992 | 2:1 | `0D3B36`×1 | 4 | 1064 | 0D3B36 |
| `0D4420` | 1 | 269327 | 992 | 2:1 | `0D3B36`×1 | 4 | 10C6 | 0D3B36 |
| `0D447C` | 1 | 269327 | 992 | 2:1 | `0D3B36`×1 | 6 | 1041 1042 | 0D3B36 |
| `0D4762` | 4 | 269473 | 992 | 2:4 | `0D4420`×4 | 3 | 1022 | 0D3B36 |
| `0D498C` | 4 | 269473 | 992 | 2:4 | `0D4420`×4 | 1 |  | 0D3B36 |
| `0D5018` | 18 | 269491 | 992 | 2:18 | `0D4396`×12, `0D43F2`×6 | 1 |  | 0D3B36 |
| `0D4A82` | 6 | 269521 | 992 | 2:6 | `0D4396`×6 | 1 |  | 0D3B36 |
| `0D4C26` | 6 | 269521 | 992 | 2:6 | `0D4396`×6 | 3 |  | 0D3B36 |
| `0D4A1E` | 3 | 269597 | 992 | 2:3 | `0D43F2`×3 | 1 |  | 0D3B36 |
| `0D4ABA` | 3 | 269597 | 992 | 2:3 | `0D43F2`×3 | 3 |  | 0D3B36 |
| `0D4F1C` | 1 | 270894 | 992 | 2:1 | `0D447C`×1 | 1 |  | 0D3B36 |
| `01CDF0` | 4 | 271119 | 992 | 2:4 | `01D764`×4 | 1 |  | common |
| `0DEA44` | 16 | 273772 | 1280 | 2:16 | `player`×16 | 2 | 1084 10F2 | player |
| `0DEC14` | 56 | 273775 | 1280 | 2:9 3:47 | `0DEA20`×47, `0DEA44`×9 | 1 |  | player |
| `0BBF12` | 1 | 274042 | 1520 | 2:1 | `script`×1 | 4 | 1027 102D | 0BBF12 |
| `01CDD8` | 2 | 274054 | 1545 | 2:2 | `01D764`×2 | 1 |  | common |
| `01CDE4` | 3 | 274054 | 1545 | 2:3 | `01D764`×3 | 1 |  | common |
| `01CDFC` | 1 | 274061 | 1552 | 2:1 | `01D764`×1 | 1 |  | common |
| `0BC192` | 1 | 274061 | 1552 | 2:1 | `0BBF12`×1 | 1 |  | 0BBF12 |
| `01CDC0` | 1 | 274110 | 1602 | 2:1 | `01D764`×1 | 1 |  | common |
| `01CDCC` | 1 | 274110 | 1602 | 2:1 | `01D764`×1 | 1 |  | common |
| `0BC100` | 1 | 274110 | 1602 | 2:1 | `0BBF12`×1 | 1 |  | 0BBF12 |

### Sección 3 (alcantarilla): 42 constructores nuevos que aparecen aquí por primera vez, por cámara

| Constructor | Creaciones | Primer fotograma | Cámara | Por sección | Creadores | Programas | Sonidos | Familia (raíz) |
| --- | --- | --- | --- | --- | --- | --- | --- | --- |
| `032B46` | 1 | 274388 | 0 | 3:1 | `script`×1 | 1 |  | 032B46 |
| `032B5A` | 1 | 274388 | 0 | 3:1 | `script`×1 | 1 |  | 032B5A |
| `032B6E` | 1 | 274388 | 0 | 3:1 | `script`×1 | 1 |  | 032B6E |
| `095C98` | 4 | 274388 | 0 | 3:4 | `script`×4 | 1 |  | 095C98 |
| `095F8C` | 4 | 274388 | 0 | 3:4 | `095C98`×4 | 0 |  | 095C98 |
| `0BC296` | 1 | 274388 | 0 | 3:1 | `script`×1 | 1 | 1027 102D | 0BC296 |
| `0BC4F4` | 5 | 274388 | 0 | 3:5 | `script`×5 | 1 |  | 0BC4F4 |
| `08346E` | 51 | 274389 | 0 | 3:51 | `01A422`×49, `script`×2 | 0 |  | 08346E |
| `0834F4` | 666 | 274389 | 0 | 3:666 | `08346E`×666 | 2 |  | 08346E |
| `0DEA20` | 100 | 274609 | 3 | 3:100 | `player`×100 | 2 | 1084 10F2 | player |
| `08996A` | 18 | 274696 | 129 | 3:18 | `script`×12, `01A422`×6 | 7 | 10F1 1144 | 08996A |
| `08A264` | 53 | 274696 | 129 | 3:53 | `089948`×19, `08996A`×18, `08995A`×16 | 0 |  | common |
| `0836E8` | 4 | 274836 | 337 | 3:4 | `script`×4 | 0 |  | 0836E8 |
| `083820` | 60 | 274836 | 337 | 3:60 | `0836E8`×60 | 1 |  | 0836E8 |
| `0BC3A6` | 3 | 274909 | 440 | 3:3 | `script`×3 | 1 | 1027 102D | 0BC3A6 |
| `089948` | 19 | 274971 | 545 | 3:19 | `01A422`×10, `script`×9 | 10 | 10F1 1144 | 089948 |
| `08995A` | 8 | 275353 | 1057 | 3:8 | `01A422`×5, `script`×3 | 3 | 1144 | 08995A |
| `0A0C00` | 1 | 277086 | 2560 | 3:1 | `script`×1 | 1 | 1020 1026 1051 1052 | 0A0C00 |
| `0A14FE` | 5 | 277086 | 2560 | 3:5 | `0A0C00`×5 | 5 | 1021 | 0A0C00 |
| `0A167C` | 1 | 277086 | 2560 | 3:1 | `0A0C00`×1 | 8 | 1075 | 0A0C00 |
| `0A1830` | 1 | 277086 | 2560 | 3:1 | `0A0C00`×1 | 2 |  | 0A0C00 |
| `0A21F0` | 6 | 277086 | 2560 | 3:6 | `0A0C00`×6 | 1 |  | 0A0C00 |
| `0A21A8` | 34 | 277206 | 2560 | 3:34 | `0A0C00`×34 | 1 |  | 0A0C00 |
| `0A21CE` | 9 | 277458 | 2560 | 3:9 | `0A0C00`×9 | 1 |  | 0A0C00 |
| `0A1E92` | 33 | 277838 | 2560 | 3:33 | `0A167C`×33 | 1 |  | 0A0C00 |
| `0A1E7E` | 12 | 277900 | 2560 | 3:12 | `0A0C00`×12 | 1 |  | 0A0C00 |
| `0A1F0E` | 104 | 277960 | 2560 | 3:104 | `0A167C`×104 | 1 |  | 0A0C00 |
| `0A1958` | 96 | 277996 | 2560 | 3:96 | `0A167C`×96 | 2 | 1022 | 0A0C00 |
| `0A1EA6` | 32 | 278082 | 2560 | 3:32 | `0A167C`×32 | 1 |  | 0A0C00 |
| `0A192E` | 79 | 279090 | 2560 | 3:79 | `0A167C`×79 | 3 | 1022 | 0A0C00 |
| `0A203A` | 1 | 284635 | 2560 | 3:1 | `0A0C00`×1 | 2 |  | 0A0C00 |
| `0A1F98` | 16 | 284663 | 2560 | 3:16 | `01D764`×16 | 2 |  | common |
| `0A1DF2` | 3 | 284916 | 2560 | 3:3 | `0A0C00`×3 | 1 |  | 0A0C00 |
| `0A1B3A` | 3 | 284943 | 2560 | 3:3 | `0A0C00`×3 | 1 |  | 0A0C00 |
| `0A1BEC` | 3 | 284943 | 2560 | 3:3 | `0A0C00`×3 | 1 |  | 0A0C00 |
| `0A1C0C` | 3 | 284943 | 2560 | 3:3 | `0A0C00`×3 | 1 |  | 0A0C00 |
| `0A1C4C` | 3 | 284943 | 2560 | 3:3 | `0A0C00`×3 | 1 |  | 0A0C00 |
| `0A1D20` | 3 | 284943 | 2560 | 3:3 | `0A0C00`×3 | 1 |  | 0A0C00 |
| `0A1756` | 1 | 291485 | 2560 | 3:1 | `0A0C00`×1 | 2 |  | 0A0C00 |
| `0A18D8` | 1 | 291485 | 2560 | 3:1 | `0A0C00`×1 | 1 |  | 0A0C00 |
| `0A2134` | 9 | 291485 | 2560 | 3:9 | `01D764`×7, `0A0C00`×2 | 5 |  | common |
| `030FB2` | 8 | 292098 | 2560 | 3:8 | `030B0A`×8 | 1 | 10D6 | 0308C0 |

## Nativos ya portados (compartidos): 106 constructores, 16659 creaciones

Los que crea directamente el guion, un corredor del guion o el actor de misión:

| Constructor | Creaciones | Primer fotograma | Cámara | Por sección | Creadores | Programas | Sonidos | Familia (raíz) |
| --- | --- | --- | --- | --- | --- | --- | --- | --- |
| `04115A` | 15 | 247664 | 0 | 1:5 2:10 | `script`×15 | 10 | 103E 1041 1042 1043 | 04115A |
| `08AFB6` | 4 | 247664 | 0 | 1:4 | `script`×4 | 2 |  | 08AFB6 |
| `08C37C` | 5 | 247664 | 0 | 1:5 | `script`×4, `01A422`×1 | 5 |  | 08C37C |
| `0AF17E` | 65 | 247664 | 0 | 1:37 2:22 3:6 | `01A422`×50, `script`×15 | 29 | 103E 1041 1042 1043 | 0AF17E |
| `05287C` | 3 | 247715 | 0 | 1:1 2:1 3:1 | `mission`×3 | 2 | 10D5 | 05287C |
| `05C53C` | 11 | 247842 | 67 | 1:11 | `script`×11 | 7 | 103E 1041 1042 1043 | 05C53C |
| `08DA12` | 8 | 247892 | 123 | 1:8 | `script`×7, `01A422`×1 | 8 | 103E 1041 1042 1043 | 08DA12 |
| `092A32` | 1 | 248043 | 280 | 1:1 | `01A422`×1 | 1 |  | 092A32 |
| `09A692` | 11 | 249258 | 600 | 1:4 2:6 3:1 | `script`×11 | 0 |  | 09A692 |
| `05D226` | 2 | 250542 | 968 | 1:2 | `script`×2 | 2 | 1041 | 05D226 |
| `09A67E` | 6 | 251724 | 1530 | 1:5 2:1 | `script`×6 | 0 |  | 09A67E |
| `087E36` | 2 | 252038 | 1640 | 1:1 3:1 | `script`×2 | 10 | 1021 102F 1072 | 087E36 |
| `099862` | 1 | 252829 | 0 | 2:1 | `script`×1 | 4 | 1139 | 099862 |
| `099DCC` | 6 | 252829 | 0 | 2:3 3:3 | `script`×6 | 7 | 1048 1056 113A | 099DCC |
| `04857C` | 1 | 255550 | 384 | 2:1 | `script`×1 | 0 |  | 04857C |
| `05D76A` | 27 | 255670 | 384 | 2:27 | `01A422`×24, `script`×3 | 6 | 1041 1042 1043 | 05D76A |
| `035B66` | 8 | 262368 | 528 | 2:8 | `script`×8 | 0 |  | 035B66 |
| `01913C` | 5 | 263183 | 528 | 2:2 3:3 | `script`×5 | 0 |  | 01913C |
| `027F54` | 1 | 263232 | 688 | 2:1 | `script`×1 | 5 | 1021 1033 109D | 027F54 |
| `09692C` | 3 | 275061 | 557 | 3:3 | `script`×3 | 4 | 1034 1092 | 09692C |

El resto (hijos, disparos, efectos, premios): `08B3E6` `01B782` `01BF46` `01BC2E` `040F88` `05DD2A` `01C796` `01D764` `06381C` `0C58D2` `063D9E` `06371E` `014A4E` `03B4E0` `08C374` `03523C` `08D330` `01CE08` `01CE44` `01CE20` `01C706` `03B936` `01C71E` `01CE38` `01CE14` `05DF3E` `05DFC2` `047874` `01BEDA` `063AEE` `0C4E86` `047DE0` `08C68E` `048EDA` `048EFE` `048F26` `048F4E` `01C6EE` `099E1C` `040396` `08982A` `08943C` `01D31C` `0897E2` `01E22C` `03C306` `01E582` `01E58A` `04870C` `048882` `047A6C` `047B24` `01E596` `035BF8` `01D86C` `01D944` `01D958` `0287FA` `028C3E` `028A88` `028BBC` `01B668` `063F10` `063B22` `01BEA4` `01CF88` `01CF7C` `0B6ED2` `03C62A` `01CE5C` `01CE68` `01CE74` `01CE80` `01CE8C` `097830` `01CE50` `059F76` `01B6A0` `097C46` `097D22` `097DD0` `0AE1DC` `0AD9B8` `0AD8A2` `0641EC` `01BF10`

En otro módulo: `01A422` (neo_script.c), `0308C0` (neo_tally.c), `030B08` (neo_tally.c), `030B0A` (neo_tally.c), `03AED8` (neo_wipe.c), `03AEF0` (neo_wipe.c), `03BC28` (neo_soldier_effects.c), `0607DE` (neo_end.c), `06090A` (neo_end.c), `060A72` (neo_end.c), `060B06` (neo_end.c), `0C4A58` (neo_landing.c), `0C5286` (neo_landing.c).

## Constructores del guion por estado

| Constructor del guion | Estado | Creaciones en `run4` | Secciones |
| --- | --- | --- | --- |
| `01913C` | native | 5 | 2 3 |
| `027F54` | native | 1 | 2 |
| `032B46` | missing | 1 | 3 |
| `032B5A` | missing | 1 | 3 |
| `032B6E` | missing | 1 | 3 |
| `035B66` | native | 8 | 2 |
| `04115A` | native | 15 | 1 2 |
| `0474B6` | missing | 8 | 1 2 |
| `047F7E` | missing | 5 | 1 2 |
| `04857C` | native | 1 | 2 |
| `050542` | missing | 9 | 1 |
| `051344` | missing | 4 | 2 |
| `05C53C` | native | 11 | 1 |
| `05D226` | native | 2 | 1 |
| `05D76A` | native | 27 | 2 |
| `08346E` | missing | 51 | 3 |
| `0836E8` | missing | 4 | 3 |
| `087E36` | native | 2 | 1 3 |
| `089948` | missing | 19 | 3 |
| `08995A` | missing | 8 | 3 |
| `08996A` | missing | 18 | 3 |
| `08AFB6` | native | 4 | 1 |
| `08C37C` | native | 5 | 1 |
| `08DA12` | native | 8 | 1 |
| `08DCB2` | missing | 1 | 1 |
| `08FF7E` | missing | 6 | 1 3 |
| `092A32` | native | 1 | 1 |
| `095C98` | missing | 4 | 3 |
| `09624E` | missing | 4 | 1 |
| `09692C` | native | 3 | 3 |
| `099862` | native | 1 | 2 |
| `099DCC` | native | 6 | 2 3 |
| `09A67E` | native | 6 | 1 2 |
| `09A692` | native | 11 | 1 2 3 |
| `0A0C00` | missing | 1 | 3 |
| `0AF17E` | native | 65 | 1 2 3 |
| `0AF1A8` | missing (M4) | 52 | 1 2 |
| `0BBE16` | missing | 1 | 1 |
| `0BBE5E` | missing | 2 | 1 |
| `0BBED0` | missing | 1 | 1 |
| `0BBF12` | missing | 1 | 2 |
| `0BC1E6` | missing | 7 | 2 |
| `0BC296` | missing | 1 | 3 |
| `0BC3A6` | missing | 3 | 3 |
| `0BC4F4` | missing | 5 | 3 |
| `0D3B36` | missing | 1 | 2 |

Constructores del guion que `run4` no creó nunca (solo en ramas no ejecutadas): `0633E2` `06341A` `063478`.

## Premios (`08C380`)

| Tipo `+6E` | Origen | Soltado | Veces | Secciones |
| --- | --- | --- | --- | --- |
| 00 | `027F54` | sí | 1 | 2 |
| 00 | `script` | no | 1 | 1 |
| 02 | `01A422` | no | 1 | 1 |
| 02 | `script` | no | 1 | 1 |
| 03 | `095F8C` | sí | 1 | 3 |
| 06 | `050542` | sí | 2 | 1 |
| 06 | `051344` | sí | 1 | 2 |
| 06 | `095F8C` | sí | 1 | 3 |
| 06 | `099DCC` | sí | 1 | 3 |
| 06 | `099E1C` | sí | 1 | 1 |
| 18 | `099E1C` | sí | 1 | 2 |
| 1A | `099862` | sí | 1 | 2 |
| 1B | `087E36` | sí | 1 | 1 |
| 23 | `099862` | sí | 1 | 2 |
| 23 | `script` | no | 1 | 1 |
| 28 | `051344` | sí | 1 | 2 |
| 2A | `099862` | sí | 1 | 2 |
| 30 | `script` | no | 1 | 1 |
| 37 | `050542` | sí | 5 | 1 |
| 38 | `050542` | sí | 7 | 1 |
| 38 | `09A67E` | sí | 13 | 1 2 |
| 3A | `050542` | sí | 8 | 1 |
| 3C | `050542` | sí | 8 | 1 |
| 4B | `099DCC` | sí | 1 | 2 |
| 4C | `051344` | sí | 1 | 2 |
| 51 | `099DCC` | sí | 2 | 2 3 |
| 52 | `095F8C` | sí | 1 | 3 |
| 53 | `095F8C` | sí | 1 | 3 |
| 53 | `099DCC` | sí | 1 | 2 |
| 53 | `099E1C` | sí | 1 | 2 |
| 55 | `050542` | sí | 6 | 1 |

## Registros no ejecutados (141)

| Registro | Constructor | Por qué |
| --- | --- | --- |
| `2DCB9E` | `08C37C` | random 2DCB8E |
| `2DCBEA` | `08C37C` | random 2DCBDC |
| `2DCC18` | `08C37C` | random 2DCC16 |
| `2DCC9E` | `050542` | random 2DCC9C |
| `2DCD0C` | `0AF17E` | if 2DCD06 |
| `2DCDC0` | `050542` | random 2DCDBE |
| `2DCDE4` | `050542` | random 2DCDBE |
| `2DCE7A` | `0AF17E` | if 2DCE74 |
| `2DCED8` | `0AF17E` | if 2DCED2 |
| `2DCFA4` | `08FF7E` | runner/block 2DCFA2 |
| `2DCFB4` | `08AFB6` | runner/block 2DCFA2 |
| `2DCFFE` | `08FF7E` | runner/block 2DCFFC |
| `2DD00E` | `08AFB6` | runner/block 2DCFFC |
| `2DD090` | `0AF17E` | if 2DD08A |
| `2DD0B8` | `0AF17E` | if 2DD0B2 |
| `2DD168` | `0AF17E` | if 2DD162 |
| `2DD1C6` | `0AF17E` | if 2DD1C0 |
| `2DD37A` | `0AF17E` | random 2DD378 |
| `2DD390` | `0AF17E` | random 2DD378 |
| `2DD3C0` | `0AF17E` | random 2DD3BE |
| `2DD3D6` | `0AF17E` | random 2DD3BE |
| `2DD43E` | `0AF17E` | if 2DD438 |
| `2DD49C` | `0AF17E` | if 2DD496 |
| `2DD4E6` | `0AF1A8` | random 2DD4CE |
| `2DD516` | `0AF1A8` | random 2DD514 |
| `2DD52C` | `0AF1A8` | random 2DD514 |
| `2DD5B2` | `08C37C` | if 2DD5AE |
| `2DD6D8` | `0AF1A8` | random 2DD6D6 |
| `2DD720` | `0AF17E` | if 2DD71A |
| `2DD748` | `0AF17E` | if 2DD742 |
| `2DD772` | `0AF1A8` | random 2DD770 |
| `2DD788` | `0AF1A8` | random 2DD770 |
| `2DD808` | `0AF17E` | if 2DD802 |
| `2DD898` | `0AF17E` | if 2DD892 |
| `2DD92A` | `0633E2` | random 2DD928 |
| `2DD936` | `06341A` | random 2DD928 |
| `2DD942` | `063478` | random 2DD928 |
| `2DD9AC` | `0474B6` | if 2DD9A8 |
| `2DD9E0` | `0474B6` | if 2DD9DC |
| `2DDA28` | `0AF17E` | if 2DDA22 |
| `2DDA5A` | `0AF17E` | if 2DDA54 |
| `2DDAA8` | `0474B6` | if 2DDAA4 |
| `2DDAC2` | `0474B6` | if 2DDABE |
| `2DDB8C` | `09A692` | if 2DDB88 |
| `2DDD66` | `0AF17E` | random 2DDD64 |
| `2DDD7C` | `0AF17E` | random 2DDD64 |
| `2DDE5A` | `0AF17E` | random 2DDE58 |
| `2DDEA0` | `0AF17E` | random 2DDE9E |
| `2DDEB6` | `0AF17E` | random 2DDE9E |
| `2DDFB6` | `0AF17E` | random 2DDFB4 |
| `2DDFFC` | `0AF17E` | random 2DDFFA |
| `2DE012` | `0AF17E` | random 2DDFFA |
| `2DE17C` | `0AF1A8` | if 2DE176 |
| `2DE1FE` | `0AF17E` | random 2DE1FC |
| `2DE214` | `0AF17E` | random 2DE1FC |
| `2DE362` | `0AF1A8` | random 2DE360 |
| `2DE378` | `0AF1A8` | random 2DE360 |
| `2DE40C` | `0474B6` | if 2DE408 |
| `2DE4FC` | `0AF1A8` | runner/block 2DE4FA |
| `2DE512` | `0AF1A8` | runner/block 2DE4FA |
| `2DE54C` | `09A67E` | random 2DE54A |
| `2DE562` | `09A67E` | random 2DE54A |
| `2DE7AA` | `09A692` | random 2DE7A8 |
| `2DE7C0` | `09A692` | random 2DE7A8 |
| `2DE8B0` | `08346E` | random 2DE84C |
| `2DE920` | `08346E` | random 2DE84C |
| `2DE92E` | `08346E` | random 2DE84C |
| `2DE990` | `08346E` | random 2DE84C |
| `2DE9BA` | `08346E` | random 2DE84C |
| `2DEAD6` | `08996A` | random 2DEAD4 |
| `2DEAE2` | `08996A` | random 2DEAD4 |
| `2DEAEE` | `08996A` | random 2DEAD4 |
| `2DEAFA` | `08996A` | random 2DEAD4 |
| `2DEB20` | `08996A` | random 2DEB1E |
| `2DEB2C` | `08996A` | random 2DEB1E |
| `2DEB38` | `08996A` | random 2DEB1E |
| `2DEB44` | `08996A` | random 2DEB1E |
| `2DEBCA` | `08996A` | random 2DEBC8 |
| `2DEBD6` | `08996A` | random 2DEBC8 |
| `2DEBFC` | `08996A` | random 2DEBFA |
| `2DEC08` | `08996A` | random 2DEBFA |
| `2DEC42` | `089948` | random 2DEC40 |
| `2DEC8E` | `08996A` | random 2DEC8C |
| `2DECA6` | `08996A` | random 2DEC8C |
| `2DECB2` | `08996A` | random 2DEC8C |
| `2DED98` | `08996A` | runner/block 2DED96 |
| `2DEDA4` | `089948` | runner/block 2DED96 |
| `2DEE06` | `08996A` | random 2DEDF8 |
| `2DEE72` | `089948` | random 2DEE58 |
| `2DEE7E` | `089948` | random 2DEE58 |
| `2DEEA4` | `089948` | random 2DEEA2 |
| `2DEEB0` | `089948` | random 2DEEA2 |
| `2DEEBC` | `089948` | random 2DEEA2 |
| `2DEEC8` | `089948` | random 2DEEA2 |
| `2DEEEE` | `08995A` | random 2DEEEC |
| `2DEEFA` | `08995A` | random 2DEEEC |
| `2DEF1E` | `08995A` | random 2DEEEC |
| `2DEF2A` | `08995A` | random 2DEEEC |
| `2DEF50` | `08995A` | random 2DEF4E |
| `2DEF5C` | `08995A` | random 2DEF4E |
| `2DEF68` | `08995A` | random 2DEF4E |
| `2DEF74` | `08995A` | random 2DEF4E |
| `2DEF80` | `08995A` | random 2DEF4E |
| `2DEF8C` | `08995A` | random 2DEF4E |
| `2DEFB8` | `08996A` | random 2DEFB6 |
| `2DEFD0` | `08996A` | random 2DEFB6 |
| `2DF008` | `08996A` | random 2DF006 |
| `2DF014` | `08996A` | random 2DF006 |
| `2DF020` | `08996A` | random 2DF006 |
| `2DF02C` | `08996A` | random 2DF006 |
| `2DF098` | `09692C` | if 2DF094 |
| `2DF12E` | `089948` | random 2DF12C |
| `2DF13A` | `089948` | random 2DF12C |
| `2DF15C` | `08995A` | random 2DF15A |
| `2DF174` | `08995A` | random 2DF15A |
| `2DF196` | `08996A` | random 2DF194 |
| `2DF1DC` | `089948` | random 2DF1DA |
| `2DF1E8` | `089948` | random 2DF1DA |
| `2DF1F4` | `089948` | random 2DF1DA |
| `2DF22E` | `08995A` | random 2DF220 |
| `2DF23A` | `08995A` | random 2DF220 |
| `2DF246` | `08995A` | random 2DF220 |
| `2DF252` | `08995A` | random 2DF220 |
| `2DF25E` | `08995A` | random 2DF220 |
| `2DF286` | `08996A` | random 2DF284 |
| `2DF292` | `08996A` | random 2DF284 |
| `2DF29E` | `08996A` | random 2DF284 |
| `2DF2AA` | `08996A` | random 2DF284 |
| `2DF2F6` | `089948` | random 2DF2F4 |
| `2DF302` | `089948` | random 2DF2F4 |
| `2DF30E` | `089948` | random 2DF2F4 |
| `2DF33C` | `08995A` | random 2DF33A |
| `2DF348` | `08995A` | random 2DF33A |
| `2DF354` | `08995A` | random 2DF33A |
| `2DF382` | `08996A` | random 2DF380 |
| `2DF38E` | `08996A` | random 2DF380 |
| `2DF39A` | `08996A` | random 2DF380 |
| `2DF3A6` | `08996A` | random 2DF380 |
| `2DF5A2` | `08FF7E` | if 2DF59C |
| `2DF5E2` | `0AF17E` | random 2DF5CA |
| `2DF628` | `099DCC` | if 2DF624 |

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
sections (start frame): 1@247663 2@252828 3@274388
constructors allocated: 222, allocations 19372, native 106

#### Sección 1 (calle)

| Fotograma | Cámara | Orden que espera | Fotogramas | Después |
| --- | --- | --- | --- | --- |
| 247664 | 0 | camera wait 0 | 0 | `2DCB42`→`0BBE5E`\*; `2DCB4E`→`04115A`; `2DCB60`→`04115A`; `2DCB72`→`08AFB6`; set region 4330 |
| 247664 | 0 | camera wait 0 | 0 | azar `2DCB8E`→`2DCB90`; `2DCB90`→`08C37C`; `2DCBAE`→`09624E`\*; `2DCBBA`→`050542`\*; corredor `2DCBCA` (cada 2, 1 veces, hasta 160) |
| 247664 | 0 | camera wait 0 | 0 | `2DCBFE`→`050542`\* |
| 247664 | 0 | camera wait 0 | 0 | azar `2DCC16`→`2DCC26`; `2DCC26`→`08C37C`; `2DCC36`→`09624E`\*; `2DCC42`→`050542`\*; `2DCC54`→`0AF17E` |
| 247664 | 0 | camera wait 16 | 153 | si group5 < 1: no (×23); misión→`0607DE`\*; misión→`05287C`; `2DCC70`→`0AF17E` |
| 247817 | 18 | camera wait 64 | 25 | si group5 < 1: no (×4); `2DCC8C`→`05C53C`; azar `2DCC9C`→`2DCCB0`; `2DCCB0`→`050542`\* |
| 247842 | 67 | camera wait 72 | 6 | si group5 < 1: no; si player_query_12 == 2060: sí; corredor `2DCCCE` (cada 90, 8 veces, hasta 288); si player_query_13 == 2060: no |
| 247848 | 75 | camera wait 88 | 9 | si group2 < 1: sí; `2DCCE0`→`0AF17E` (corredor); si group5 < 1: no; `2DCD28`→`0AF17E`; `2DCD3E`→`0AF17E`; `2DCD54`→`0AF17E` |
| 247857 | 90 | camera wait 120 | 35 | si group5 < 1: sí; azar `2DCBDC`→`2DCBDE`; `2DCBDE`→`08C37C` (corredor); `2DCD70`→`08DA12` |
| 247892 | 123 | camera wait 144 | 33 | `2DCD88`→`05C53C`; `2DCD98`→`08AFB6` |
| 247925 | 144 | camera wait 168 | 30 | `2DCDAE`→`05C53C`; azar `2DCDBE`→`2DCDD2`; `2DCDD2`→`050542`\* |
| 247955 | 171 | camera wait 176 | 6 | corredor `2DCDFE` (cada 2, 1 veces, hasta 520) |
| 247961 | 179 | camera wait 272 | 75 | si group5 < 1: no (×9); `2DCE20`→`08DA12` |
| 248036 | 274 | camera wait 288 | 17 | si group5 < 1: sí; `2DCE10`→`092A32` (corredor); si player_query_12 == 2060: sí; corredor `2DCE3C` (cada 60, 8 veces, hasta 600); si player_query_13 == 2060: no |
| 248053 | 288 | camera wait 288 | 0 | si player_query_12 == 2060: sí; corredor `2DCE9A` (cada 90, 8 veces, hasta 600); si player_query_13 == 2060: no |
| 248053 | 288 | camera wait 304 | 20 | si group2 < 1: sí; `2DCEAC`→`0AF17E` (corredor); si group1 < 2: sí; `2DCE4E`→`0AF17E` (corredor); `2DCEF4`→`05C53C` |
| 248073 | 305 | camera wait 408 | 108 | `2DCF0A`→`05C53C`; `2DCF1A`→`08DCB2`\* |
| 248181 | 408 | camera wait 448 | 44 | `2DCF30`→`0BBE5E`\* |
| 248225 | 448 | camera wait 528 | 78 | si group1 < 2: sí; `2DCE4E`→`0AF17E` (corredor); `2DCF42`→`05C53C` |
| 248303 | 528 | camera wait 600 + lock 600 | 61 | — |
| 248364 | 600 | camera wait 600 + lock 600 | 0 | — |
| 248364 | 600 | condition wait group7 < 1 | 0 | si player_query_0B == 2306: sí; `2DCF7C`→`08FF7E`\*; `2DCF8C`→`08AFB6`; si player_query_0C == 2306: no |
| 248364 | 600 | timer wait 30 | 95 | si group2 < 1: sí; `2DCEAC`→`0AF17E` (corredor); si group6 < 1: sí; `2DCFD4`→`08FF7E`\*; `2DCFE4`→`08AFB6`; si group7 < 1: no; si player_query_12 == 2306: sí; corredor `2DD026` (cada 90, 8 veces, hasta 601); corredor `2DD04E` (cada 120, 8 veces, hasta 601); si player_query_13 == 2306: no |
| 248459 | 600 | condition wait group6 < 1 | 387 | si group2 < 1: no (×2); si group1 < 2: sí (×4); `2DD038`→`0AF17E` (corredor) (×2); `2DCE4E`→`0AF17E` (corredor) (×2); si group2 < 1: sí; `2DD060`→`0AF17E` (corredor) |
| 248846 | 600 | condition wait group7 < 1 | 412 | si group1 < 2: sí (×4); `2DCE4E`→`0AF17E` (corredor) (×3); si group2 < 1: no (×2); `2DD038`→`0AF17E` (corredor); si group2 < 1: sí; `2DD060`→`0AF17E` (corredor); `2DD0DC`→`09A692`; `2DD0F2`→`09A692`; `2DD108`→`09A692`; camera unlock |
| 249258 | 600 | camera wait 608 | 18 | si player_query_12 == 2060: sí; corredor `2DD12A` (cada 120, 8 veces, hasta 880); si player_query_13 == 2060: no |
| 249276 | 609 | camera wait 608 | 0 | si player_query_12 == 2060: sí; corredor `2DD188` (cada 120, 8 veces, hasta 880); si player_query_13 == 2060: no |
| 249276 | 609 | camera wait 608 | 0 | `2DD1E2`→`08DA12` |
| 249276 | 609 | camera wait 664 | 51 | si group2 < 1: sí; `2DD19A`→`0AF17E` (corredor); si group1 < 2: sí; `2DD13C`→`0AF17E` (corredor); `2DD1FA`→`08C37C`; `2DD208`→`09624E`\*; `2DD214`→`05C53C`; `2DD224`→`050542`\*; `2DD236`→`05C53C` |
| 249327 | 667 | camera wait 800 | 118 | `2DD24C`→`08DA12`; `2DD25E`→`08DA12`; `2DD270`→`050542`\* |
| 249445 | 800 | camera wait 880 + lock 880 | 78 | — |
| 249523 | 880 | camera wait 880 + lock 880 | 0 | `2DD29E`→`0474B6`\* |
| 249523 | 880 | timer wait 30 | 91 | `2DD2B4`→`0AF1A8`\* |
| 249614 | 880 | timer wait 15 | 45 | si group2 < 1: sí; `2DD19A`→`0AF17E` (corredor); si group1 < 2: sí; `2DD13C`→`0AF17E` (corredor); `2DD2CE`→`0AF1A8`\* |
| 249659 | 880 | timer wait 30 | 70 | `2DD2E8`→`0474B6`\* |
| 249729 | 880 | timer wait 30 | 60 | `2DD2FE`→`0AF1A8`\* |
| 249789 | 880 | timer wait 15 | 31 | `2DD318`→`0AF1A8`\* |
| 249820 | 880 | timer wait 30 | 61 | `2DD332`→`0AF1A8`\* |
| 249881 | 880 | timer wait 15 | 44 | si group2 < 1: no; si group1 < 2: sí; `2DD13C`→`0AF17E` (corredor); `2DD34C`→`0AF1A8`\*; si player_query_12 == 2060: sí; corredor `2DD366` (cada 120, 8 veces, hasta 881); si player_query_13 == 2060: no |
| 249925 | 880 | condition wait group7 < 1 | 571 | si group1 < 1: no (×2); si group2 < 1: sí; `2DD19A`→`0AF17E` (corredor); si group1 < 2: sí; `2DD13C`→`0AF17E` (corredor); camera unlock |
| 250496 | 880 | camera wait 889 | 7 | si player_query_12 == 2060: sí; corredor `2DD400` (cada 120, 8 veces, hasta 1208); si player_query_13 == 2060: no |
| 250503 | 892 | camera wait 889 | 0 | si player_query_12 == 2060: sí; corredor `2DD45E` (cada 120, 8 veces, hasta 1208); si player_query_13 == 2060: no |
| 250503 | 892 | camera wait 889 | 0 | si player_query_12 == 2060: sí; corredor `2DD4BC` (cada 120, 8 veces, hasta 1208); si player_query_13 == 2060: no |
| 250503 | 892 | camera wait 896 | 3 | si group1 < 2: sí; `2DD412`→`0AF17E` (corredor); `2DD54A`→`08DA12`; `2DD55C`→`08DA12` |
| 250506 | 899 | camera wait 968 | 36 | `2DD574`→`05D226`; `2DD586`→`05C53C` |
| 250542 | 968 | camera wait 1040 | 75 | si group3 < 2: sí; azar `2DD4CE`→`2DD4D0`; `2DD4D0`→`0AF1A8`\* (corredor); si group2 < 1: sí; `2DD470`→`0AF17E` (corredor); si player_query_15 == 1806: sí; `2DD5A0`→`08C37C`; si player_query_14 == 1806: no; `2DD5C0`→`09624E`\*; `2DD5CC`→`05D226`; `2DD5DE`→`05C53C`; `2DD5EE`→`050542`\* |
| 250617 | 1043 | camera wait 1096 | 43 | `2DD606`→`04115A` |
| 250660 | 1097 | camera wait 1144 | 54 | `2DD61E`→`05C53C`; `2DD62E`→`050542`\* |
| 250714 | 1147 | camera wait 1208 + lock 1208 | 48 | — |
| 250762 | 1208 | camera wait 1208 + lock 1208 | 0 | `2DD65C`→`047F7E`\*; si player_query_12 == 2306: sí; corredor `2DD674` (cada 90, 8 veces, hasta 1209); corredor `2DD69C` (cada 90, 8 veces, hasta 1209); corredor `2DD6C4` (cada 120, 8 veces, hasta 1209); si player_query_13 == 2306: no |
| 250762 | 1208 | timer wait 15 | 55 | si group2 < 1: sí; `2DD6AE`→`0AF17E` (corredor); si group1 < 1: sí; `2DD686`→`0AF17E` (corredor); `2DD7A6`→`0474B6`\* |
| 250817 | 1208 | condition wait group7 < 1 | 652 | si group3 < 2: sí (×2); azar `2DD6D6`→`2DD6EE`; `2DD6EE`→`0AF1A8`\* (corredor); si group1 < 2: sí (×2); `2DD412`→`0AF17E` (corredor) (×2); azar `2DD4CE`→`2DD4D0`; `2DD4D0`→`0AF1A8`\* (corredor); si group2 < 1: no (×4); si group1 < 1: sí; `2DD686`→`0AF17E` (corredor); si group3 < 2: no (×2); si group1 < 1: no; camera unlock |
| 251469 | 1208 | camera wait 1216 | 9 | si player_query_12 == 2060: sí; corredor `2DD7CA` (cada 120, 8 veces, hasta 1360); si player_query_13 == 2060: no |
| 251478 | 1216 | camera wait 1472 | 204 | si group2 < 1: sí; `2DD7DC`→`0AF17E` (corredor); `2DD824`→`0AF17E`; `2DD83A`→`0AF17E` |
| 251682 | 1474 | camera wait 1488 | 12 | si player_query_12 == 2060: sí; corredor `2DD85A` (cada 90, 8 veces, hasta 1632); si player_query_13 == 2060: no |
| 251694 | 1490 | camera wait 1528 | 30 | si group2 < 1: sí; `2DD86C`→`08DA12` (corredor); `2DD8B4`→`09A67E`; `2DD8CA`→`09A67E`; `2DD8E0`→`09A67E`; `2DD8F6`→`09A67E`; `2DD90C`→`09A67E` |
| 251724 | 1530 | camera wait 1560 | 24 | azar `2DD928`→`2DD952` |
| 251748 | 1562 | camera wait 1584 | 18 | `2DD962`→`0BBED0`\* |
| 251766 | 1584 | camera wait 1632 | 42 | `2DD972`→`09A692` |
| 251808 | 1632 | camera wait 1640 + lock 1640 | 6 | — |
| 251814 | 1640 | camera wait 1640 + lock 1640 | 0 | — |
| 251814 | 1640 | timer wait 30 | 90 | si player_query_14 == 1810: no; si player_query_15 == 1814: sí; `2DD9C2`→`04115A` |
| 251904 | 1640 | timer wait 30 | 74 | si player_query_14 == 1810: no; si player_query_15 == 1814: sí; `2DD9F6`→`04115A`; si player_query_12 == 2060: sí; corredor `2DDA10` (cada 90, 16 veces, hasta 1641); si player_query_13 == 2060: no; corredor `2DDA70` (cada 120, 12 veces, hasta 1641) |
| 251978 | 1640 | condition wait group7 < 1 | 0 | si player_query_14 == 1810: no |
| 251978 | 1640 | timer wait 30 | 60 | si group2 < 1: no; si player_query_14 == 1810: no |
| 252038 | 1640 | condition wait group7 < 1 | 0 | `2DDADA`→`087E36` |
| 252038 | 1640 | condition wait groups0..7 == 0 | 566 | si group3 < 1: sí; si group7 == 1: sí; `2DDA88`→`0AF1A8`\* (corredor); si group2 < 1: no; si group3 < 1: no; si group2 < 1: sí (×2); si group7 == 1: no (×2); `2DDAF4`→`0BBE16`\*; camera unlock |
| 252604 | 1640 | end | 225 | si group3 < 1: sí; si group7 == 1: no; **fin `018F92`**; misión→`03AED8`\* |

#### Sección 2 (metro)

| Fotograma | Cámara | Orden que espera | Fotogramas | Después |
| --- | --- | --- | --- | --- |
| 252829 | 0 | camera wait 0 | 0 | `2DDB08`→`099DCC`; `2DDB1A`→`0AF17E`; `2DDB30`→`0AF17E`; `2DDB46`→`099862` |
| 252829 | 0 | camera wait 40 | 317 | misión→`03AEF0`\*; misión→`05287C`; `2DDB5E`→`0BC1E6`\*; si player_query_12 == 1814: sí; `2DDB72`→`09A692`; si player_query_13 == 1814: no |
| 253146 | 44 | camera wait 80 + lock 80 | 8 | — |
| 253154 | 80 | camera wait 80 + lock 80 | 0 | `2DDBBE`→`0474B6`\* |
| 253154 | 80 | timer wait 15 | 33 | `2DDBD4`→`04115A`; `2DDBEA`→`04115A` |
| 253187 | 80 | condition wait group1 < 1 | 453 | camera unlock |
| 253640 | 80 | camera wait 192 + lock 192 | 108 | — |
| 253748 | 192 | camera wait 192 + lock 192 | 0 | `2DDC24`→`0474B6`\* |
| 253748 | 192 | timer wait 15 | 37 | `2DDC3A`→`04115A` |
| 253785 | 192 | timer wait 15 | 30 | `2DDC54`→`04115A` |
| 253815 | 192 | timer wait 60 | 120 | `2DDC6E`→`0474B6`\* |
| 253935 | 192 | timer wait 30 | 60 | `2DDC84`→`04115A` |
| 253995 | 192 | timer wait 15 | 30 | `2DDC9E`→`04115A` |
| 254025 | 192 | timer wait 15 | 30 | `2DDCB8`→`04115A` |
| 254055 | 192 | condition wait group1 < 1 | 1443 | camera unlock |
| 255498 | 192 | camera wait 232 | 14 | `2DDCDC`→`0BC1E6`\*; `2DDCEC`→`09A692` |
| 255512 | 236 | camera wait 256 | 6 | si player_query_12 == 2060: sí; corredor `2DDD0C` (cada 60, 8 veces, hasta 384); si player_query_13 == 2060: no |
| 255518 | 263 | camera wait 384 + lock 384 | 32 | si group2 < 3: sí; azar `2DDD1E`→`2DDD20`; `2DDD20`→`0AF17E` (corredor) |
| 255550 | 384 | camera wait 384 + lock 384 | 0 | `2DDDB0`→`04857C`; set region 4330; corredor `2DDDC6` (cada 120, 12 veces, hasta 384) |
| 255550 | 384 | condition wait group1 < 1 | 6508 | si group2 < 3: sí (×7); azar `2DDD1E`→`2DDD20` (×3); `2DDD20`→`0AF17E` (corredor) (×3); si group5 < 1: sí (×12); si player_query_0B == 2306: no (×5); si player_query_0C == 2306: sí (×5); `2DDE0C`→`05D76A` (corredor) (×5); `2DDE1E`→`05D76A` (corredor) (×5); azar `2DDD1E`→`2DDD36` (×4); `2DDD36`→`0AF17E` (corredor) (×4); si player_query_0B == 2306: sí (×7); `2DDDE0`→`05D76A` (corredor) (×7); `2DDDF2`→`05D76A` (corredor) (×7); si player_query_0C == 2306: no (×7); si group5 < 1: no (×2); camera unlock |
| 262058 | 384 | camera wait 392 | 102 | si player_query_12 == 2060: sí; corredor `2DDE46` (cada 60, 8 veces, hasta 512); si player_query_13 == 2060: no |
| 262160 | 394 | camera wait 424 | 24 | si group2 < 3: sí; azar `2DDE58`→`2DDE70`; `2DDE70`→`0AF17E` (corredor); `2DDED4`→`0BC1E6`\* |
| 262184 | 424 | camera wait 520 | 60 | `2DDEEA`→`04115A` |
| 262244 | 520 | camera wait 528 + lock 528 | 4 | — |
| 262248 | 528 | camera wait 528 + lock 528 | 0 | `2DDF1C`→`0AF17E`; `2DDF32`→`0AF17E` |
| 262248 | 528 | timer wait 60 | 120 | `2DDF4C`→`035B66` |
| 262368 | 528 | timer wait 30 | 60 | `2DDF60`→`051344`\* |
| 262428 | 528 | condition wait group3 < 1 | 755 | `2DDF74`→`09A692`; camera unlock; `2DDF8C`→`01913C` |
| 263183 | 528 | camera wait 544 | 12 | si player_query_12 == 2060: sí; corredor `2DDFA2` (cada 60, 4 veces, hasta 672); si player_query_13 == 2060: no |
| 263195 | 547 | camera wait 616 | 21 | si group2 < 3: sí; azar `2DDFB4`→`2DDFCC`; `2DDFCC`→`0AF17E` (corredor); `2DE030`→`0BC1E6`\*; `2DE040`→`09A692` |
| 263216 | 618 | camera wait 640 | 6 | `2DE05C`→`0AF1A8`\*; `2DE072`→`0AF1A8`\* |
| 263222 | 648 | camera wait 672 | 6 | corredor `2DE08E` (cada 45, 8 veces, hasta 688) |
| 263228 | 674 | camera wait 688 + lock 688 | 4 | si group5 < 4: sí; azar `2DE0A0`→`2DE0B8`; `2DE0B8`→`0AF1A8`\* (corredor) |
| 263232 | 688 | camera wait 688 + lock 688 | 0 | `2DE0EC`→`027F54` |
| 263232 | 688 | timer wait 150 | 300 | si group5 < 4: sí (×3); azar `2DE0A0`→`2DE0A2` (×2); `2DE0A2`→`0AF1A8`\* (corredor) (×2); azar `2DE0A0`→`2DE0B8`; `2DE0B8`→`0AF1A8`\* (corredor); `2DE106`→`0AF1A8`\* |
| 263532 | 688 | timer wait 10 | 20 | `2DE120`→`0AF1A8`\* |
| 263552 | 688 | timer wait 10 | 20 | `2DE13A`→`0AF1A8`\* |
| 263572 | 688 | timer wait 10 | 20 | `2DE154`→`0AF1A8`\*; corredor `2DE16A` (cada 60, 8 veces, hasta 688) |
| 263592 | 688 | condition wait group4 < 1 | 62 | si group5 < 1: no; si group5 < 4: no; camera unlock |
| 263654 | 688 | camera wait 704 | 8 | si player_query_12 == 2060: sí; corredor `2DE1A4` (cada 60, 4 veces, hasta 808); si player_query_13 == 2060: no |
| 263662 | 707 | camera wait 808 | 22 | si group2 < 3: sí; azar `2DE1B6`→`2DE1CE`; `2DE1CE`→`0AF17E` (corredor); `2DE232`→`0BC1E6`\*; `2DE242`→`09A692` |
| 263684 | 808 | camera wait 808 + lock 808 | 0 | — |
| 263684 | 808 | camera wait 808 + lock 808 | 0 | `2DE274`→`0AF1A8`\* |
| 263684 | 808 | timer wait 15 | 30 | `2DE28E`→`0AF1A8`\* |
| 263714 | 808 | timer wait 15 | 30 | `2DE2A8`→`0AF1A8`\* |
| 263744 | 808 | timer wait 15 | 45 | `2DE2C2`→`0AF1A8`\* |
| 263789 | 808 | timer wait 45 | 118 | si group2 < 3: sí; azar `2DE1B6`→`2DE1B8`; `2DE1B8`→`0AF17E` (corredor); `2DE2DC`→`0474B6`\* |
| 263907 | 808 | timer wait 30 | 60 | si group2 < 3: sí; azar `2DE1B6`→`2DE1B8`; `2DE1B8`→`0AF17E` (corredor); `2DE2F2`→`047F7E`\*; si player_query_12 == 2060: sí; corredor `2DE308` (cada 120, 8 veces, hasta 809); si player_query_13 == 2060: no |
| 263967 | 808 | timer wait 60 | 120 | si group5 < 2: no; si group2 < 3: sí; azar `2DE1B6`→`2DE1CE`; `2DE1CE`→`0AF17E` (corredor); `2DE394`→`047F7E`\* |
| 264087 | 808 | condition wait group1 < 2 | 3671 | si group5 < 2: sí (×8); azar `2DE31A`→`2DE31C` (×6); `2DE31C`→`0AF1A8`\* (corredor) (×6); azar `2DE31A`→`2DE332` (×2); `2DE332`→`0AF1A8`\* (corredor) (×2); `2DE3AC`→`0474B6`\* |
| 267758 | 808 | timer wait 30 | 60 | `2DE3C2`→`099DCC` |
| 267818 | 808 | timer wait 30 | 60 | `2DE3DA`→`047F7E`\* |
| 267878 | 808 | timer wait 30 | 60 | `2DE3F0`→`047F7E`\* |
| 267938 | 808 | condition wait group1 < 2 | 1156 | si player_query_13 == 1810: no |
| 269094 | 808 | condition wait group1 < 1 | 149 | `2DE424`→`01913C`; `2DE430`→`099DCC`; camera unlock |
| 269243 | 808 | camera wait 992 + lock 992 | 58 | — |
| 269301 | 992 | camera wait 992 + lock 992 | 0 | `2DE460`→`05D76A`; `2DE472`→`05D76A`; `2DE484`→`05D76A` |
| 269301 | 992 | condition wait group2 < 2 | 26 | `2DE49C`→`0D3B36`\*; corredor `2DE4A6` (cada 120, 8 veces, hasta 992) |
| 269327 | 992 | condition wait group3 < 1 | 1795 | si group1 < 2: sí (×7); si player_query_12 == 2306: sí (×7); si player_query_0C == 1814: sí (×3); `2DE4C4`→`0AF1A8`\* (corredor) (×3); si player_query_0B == 1814: no (×3); si player_query_13 == 2306: no (×7); si player_query_0C == 1814: no (×4); si player_query_0B == 1814: sí (×4); `2DE4DE`→`0AF1A8`\* (corredor) (×4); camera unlock |
| 271122 | 992 | camera wait 1000 | 9 | `2DE53A`→`0BC1E6`\*; azar `2DE54A`→`2DE578`; `2DE578`→`09A67E` |
| 271131 | 1004 | camera wait 1072 | 30 | `2DE596`→`04115A`; `2DE5AC`→`04115A` |
| 271161 | 1077 | camera wait 1120 | 36 | `2DE5C8`→`0AF1A8`\*; `2DE5DE`→`0AF1A8`\* |
| 271197 | 1121 | camera wait 1128 | 9 | `2DE5FA`→`0AF1A8`\*; `2DE610`→`0AF1A8`\* |
| 271206 | 1128 | camera wait 1192 | 56 | `2DE62C`→`0BC1E6`\* |
| 271262 | 1194 | camera wait 1280 + lock 1280 | 63 | — |
| 271325 | 1280 | camera wait 1280 + lock 1280 | 0 | — |
| 271325 | 1280 | timer wait 30 | 60 | `2DE65C`→`035B66` |
| 271385 | 1280 | timer wait 30 | 60 | `2DE670`→`051344`\* |
| 271445 | 1280 | condition wait group3 < 1 | 761 | — |
| 272206 | 1280 | timer wait 30 | 88 | `2DE688`→`035B66` |
| 272294 | 1280 | timer wait 30 | 62 | `2DE69C`→`035B66` |
| 272356 | 1280 | timer wait 30 | 60 | `2DE6B0`→`051344`\*; corredor `2DE6BE` (cada 60, 16 veces, hasta 1281) |
| 272416 | 1280 | condition wait group3 < 1 | 669 | si group5 < 1: sí (×3); azar `2DE6D0`→`2DE6D2` (×2); `2DE6D2`→`0AF1A8`\* (corredor) (×2); si group5 < 1: no (×2); azar `2DE6D0`→`2DE6FE`; `2DE6FE`→`0AF1A8`\* (corredor) |
| 273085 | 1280 | timer wait 30 | 91 | si group5 < 1: sí; azar `2DE6D0`→`2DE6E8`; `2DE6E8`→`0AF1A8`\* (corredor); `2DE720`→`035B66` |
| 273176 | 1280 | timer wait 10 | 28 | `2DE734`→`035B66` |
| 273204 | 1280 | timer wait 10 | 24 | `2DE748`→`035B66` |
| 273228 | 1280 | timer wait 10 | 20 | `2DE75C`→`035B66`; `2DE76C`→`051344`\*; corredor `2DE77A` (cada 120, 16 veces, hasta 1296) |
| 273248 | 1280 | condition wait group3 < 1 | 713 | si group5 < 1: no; si group2 < 1: sí (×2); `2DE78C`→`0AF17E` (corredor) (×2); si group5 < 1: sí (×5); azar `2DE6D0`→`2DE6FE` (×2); `2DE6FE`→`0AF1A8`\* (corredor) (×2); si group2 < 1: no; azar `2DE6D0`→`2DE6D2` (×3); `2DE6D2`→`0AF1A8`\* (corredor) (×3); azar `2DE7A8`→`2DE7D6`; `2DE7D6`→`09A692`; camera unlock |
| 273961 | 1280 | camera wait 1472 | 71 | `2DE7F6`→`0AF17E`; `2DE80C`→`0AF17E` |
| 274032 | 1479 | camera wait 1520 | 10 | `2DE828`→`0BBF12`\* |
| 274042 | 1520 | end | 346 | misión→`03AED8`\* |

#### Sección 3 (alcantarilla)

| Fotograma | Cámara | Orden que espera | Fotogramas | Después |
| --- | --- | --- | --- | --- |
| 274388 | 0 | camera wait 0 | 0 | corredor `2DE83A` (cada 0, 999 veces, hasta 2560) |
| 274388 | 0 | camera wait 0 | 0 | `2DE9D0`→`0BC4F4`\*; `2DE9DC`→`032B46`\*; `2DE9E6`→`032B5A`\*; `2DE9F0`→`032B6E`\* |
| 274388 | 0 | camera wait 0 | 0 | `2DEA00`→`0BC296`\*; `2DEA0A`→`095C98`\* |
| 274388 | 0 | camera wait 8 + lock 8 | 227 | si group8 < 3: sí (×3); azar `2DE84C`→`2DE982`; `2DE982`→`08346E`\* (corredor); misión→`03AEF0`\*; azar `2DE84C`→`2DE8CC`; `2DE8CC`→`08346E`\* (corredor); azar `2DE84C`→`2DE878`; `2DE878`→`08346E`\* (corredor); si group8 < 3: no (×110); misión→`05287C` |
| 274615 | 8 | camera wait 8 + lock 8 | 0 | — |
| 274615 | 8 | condition wait group7 == 0 | 49 | si group8 < 3: no (×24); camera unlock |
| 274664 | 8 | camera wait 16 | 6 | si group8 < 3: no (×3); `2DEA54`→`095C98`\* |
| 274670 | 20 | camera wait 80 | 16 | si group8 < 3: no (×7); si group8 < 3: sí; azar `2DE84C`→`2DE99E`; `2DE99E`→`08346E`\* (corredor); `2DEA7A`→`0BC4F4`\* |
| 274686 | 89 | camera wait 128 | 10 | si group8 < 3: no (×5); `2DEA8C`→`08996A`\*; `2DEA98`→`08996A`\* |
| 274696 | 129 | camera wait 152 | 15 | si group8 < 3: no (×4); si group8 < 3: sí; azar `2DE84C`→`2DE974`; `2DE974`→`08346E`\* (corredor); `2DEAAA`→`08346E`\* |
| 274711 | 155 | camera wait 320 | 117 | si group8 < 3: no (×47); si group8 < 3: sí (×2); azar `2DE84C`→`2DE8CC`; `2DE8CC`→`08346E`\* (corredor); azar `2DE84C`→`2DE894`; `2DE894`→`08346E`\* (corredor); si player_query_12 == 2060: sí; corredor `2DEAC2` (cada 30, 6 veces, hasta 424); si player_query_13 == 2060: no |
| 274828 | 321 | camera wait 336 | 8 | si group3 < 2: no; si group8 < 3: no (×3); si group8 < 3: sí; azar `2DE84C`→`2DE8BE`; `2DE8BE`→`08346E`\* (corredor); `2DEB58`→`0836E8`\* |
| 274836 | 337 | camera wait 384 | 24 | si group8 < 3: no (×12); `2DEB6E`→`08996A`\*; `2DEB7A`→`08996A`\* |
| 274860 | 385 | camera wait 400 | 10 | si group8 < 3: no (×5); si always == 2306: sí; `2DEB92`→`08996A`\*; `2DEB9E`→`08996A`\* |
| 274870 | 402 | camera wait 424 | 24 | si group8 < 3: no (×8); si player_query_12 == 2060: sí; corredor `2DEBB6` (cada 30, 6 veces, hasta 551); si player_query_13 == 2060: no |
| 274894 | 425 | camera wait 440 | 15 | si group3 < 4: no; si group8 < 3: no (×5); `2DEC1C`→`0BC3A6`\* |
| 274909 | 440 | camera wait 543 | 62 | si group8 < 3: no (×26); si group8 < 3: sí (×3); azar `2DE84C`→`2DE84E`; `2DE84E`→`08346E`\* (corredor); azar `2DE84C`→`2DE966`; `2DE966`→`08346E`\* (corredor); si group3 < 4: no; azar `2DE84C`→`2DE94A`; `2DE94A`→`08346E`\* (corredor); corredor `2DEC2E` (cada 30, 8 veces, hasta 936); corredor `2DEC74` (cada 30, 8 veces, hasta 936) |
| 274971 | 545 | camera wait 544 + lock 544 | 0 | — |
| 274971 | 545 | camera wait 544 + lock 544 | 0 | — |
| 274971 | 545 | condition wait group7 == 0 | 81 | si group7 == 0: no (×2); si group1 < 4: sí (×2); azar `2DEC40`→`2DEC5A`; `2DEC5A`→`089948`\* (corredor); si group8 < 3: no (×40); si group3 < 4: no; azar `2DEC40`→`2DEC4E`; `2DEC4E`→`089948`\* (corredor); camera unlock |
| 275052 | 545 | camera wait 552 | 9 | si group8 < 3: no (×3); `2DECEA`→`09692C`; `2DECF4`→`095C98`\* |
| 275061 | 557 | camera wait 592 | 15 | si group8 < 3: no (×5); `2DED1A`→`0BC4F4`\* |
| 275076 | 598 | camera wait 640 | 15 | si group8 < 3: sí (×2); azar `2DE84C`→`2DE982`; `2DE982`→`08346E`\* (corredor); si group8 < 3: no (×3); azar `2DE84C`→`2DE904`; `2DE904`→`08346E`\* (corredor); `2DED2C`→`08346E`\* |
| 275091 | 645 | camera wait 752 | 39 | si group8 < 3: no (×13); si group7 == 0: sí; si group3 < 5: no; si group1 < 4: sí; azar `2DEC40`→`2DEC5A`; `2DEC5A`→`089948`\* (corredor); `2DED40`→`0836E8`\* |
| 275130 | 752 | camera wait 880 | 103 | si group8 < 3: no (×34); si group8 < 3: sí (×3); azar `2DE84C`→`2DE8CC`; `2DE8CC`→`08346E`\* (corredor); azar `2DE84C`→`2DE8DA`; `2DE8DA`→`08346E`\* (corredor); azar `2DE84C`→`2DE9AC`; `2DE9AC`→`08346E`\* (corredor); si group7 == 0: sí; si group3 < 5: sí; azar `2DEC8C`→`2DEC9A`; `2DEC9A`→`08996A`\* (corredor); si group1 < 4: sí; azar `2DEC40`→`2DEC66`; `2DEC66`→`089948`\* (corredor); `2DED56`→`08996A`\*; `2DED62`→`08996A`\* |
| 275233 | 880 | camera wait 896 | 15 | si group8 < 3: no (×4); si group8 < 3: sí; azar `2DE84C`→`2DE99E`; `2DE99E`→`08346E`\* (corredor); `2DED74`→`089948`\*; `2DED80`→`089948`\* |
| 275248 | 898 | camera wait 912 | 15 | si group8 < 3: no (×5); si player_query_13 == 2306: no |
| 275263 | 914 | camera wait 936 | 21 | si group8 < 3: no (×8); corredor `2DEDB8` (cada 30, 8 veces, hasta 1055); corredor `2DEDE6` (cada 30, 8 veces, hasta 1055) |
| 275284 | 937 | camera wait 960 | 16 | si group3 < 5: sí; azar `2DEDF8`→`2DEDFA`; `2DEDFA`→`08996A`\* (corredor); si group1 < 4: sí; azar `2DEDCA`→`2DEDD8`; `2DEDD8`→`089948`\* (corredor); si group8 < 3: no (×7); `2DEE1A`→`0BC3A6`\* |
| 275300 | 960 | camera wait 1000 | 23 | si group8 < 3: no (×10); `2DEE2C`→`0836E8`\* |
| 275323 | 1000 | camera wait 1055 | 30 | si group8 < 3: no (×13); si group8 < 3: sí (×2); azar `2DE84C`→`2DE8CC`; `2DE8CC`→`08346E`\* (corredor); azar `2DE84C`→`2DE8DA`; `2DE8DA`→`08346E`\* (corredor); si group3 < 5: sí; azar `2DEDF8`→`2DEDFA`; `2DEDFA`→`08996A`\* (corredor); si group1 < 4: sí; azar `2DEDCA`→`2DEDCC`; `2DEDCC`→`089948`\* (corredor); si player_query_12 == 2060: sí (×3); corredor `2DEE46` (cada 30, 8 veces, hasta 1448); si player_query_13 == 2060: no (×3); corredor `2DEEDA` (cada 90, 8 veces, hasta 1448); corredor `2DEF9E` (cada 30, 8 veces, hasta 1448) |
| 275353 | 1057 | camera wait 1056 + lock 1056 | 0 | — |
| 275353 | 1057 | camera wait 1056 + lock 1056 | 0 | — |
| 275353 | 1057 | condition wait group7 == 0 | 84 | si group7 == 0: no (×2); si group2 < 1: sí; azar `2DEEEC`→`2DEF06`; `2DEF06`→`08995A`\* (corredor); si group1 < 4: sí (×2); azar `2DEE58`→`2DEE5A`; `2DEE5A`→`089948`\* (corredor); si group8 < 3: no (×41); azar `2DEE58`→`2DEE66`; `2DEE66`→`089948`\* (corredor); camera unlock; `2DF05E`→`01913C` |
| 275437 | 1057 | camera wait 1104 | 24 | si group8 < 3: no (×7); si group8 < 3: sí; azar `2DE84C`→`2DE99E`; `2DE99E`→`08346E`\* (corredor); `2DF070`→`0BC4F4`\* |
| 275461 | 1110 | camera wait 1344 | 120 | si group8 < 3: no (×40); si group8 < 3: sí (×5); azar `2DE84C`→`2DE8CC`; `2DE8CC`→`08346E`\* (corredor); azar `2DE84C`→`2DE85C`; `2DE85C`→`08346E`\* (corredor); azar `2DE84C`→`2DE894` (×2); `2DE894`→`08346E`\* (corredor) (×2); si group7 == 0: sí (×2); si group3 < 4: sí (×2); azar `2DEFB6`→`2DEFDC`; `2DEFDC`→`08996A`\* (corredor); si group1 < 4: no (×2); azar `2DE84C`→`2DE8BE`; `2DE8BE`→`08346E`\* (corredor); si group2 < 1: sí; azar `2DEEEC`→`2DEF12`; `2DEF12`→`08995A`\* (corredor); azar `2DEFB6`→`2DEFC4`; `2DEFC4`→`08996A`\* (corredor); si player_query_12 == 2820: sí; si player_query_15 == 1802: sí; `2DF08A`→`09692C`; si player_query_13 == 1802: no; `2DF0A2`→`095C98`\* |
| 275581 | 1347 | camera wait 1360 | 12 | si group8 < 3: no (×4); `2DF0C8`→`0836E8`\* |
| 275593 | 1363 | camera wait 1392 | 25 | si group8 < 3: no (×9); `2DF0DE`→`08996A`\*; `2DF0EA`→`08996A`\* |
| 275618 | 1393 | camera wait 1408 | 18 | si group8 < 3: no (×5); si group8 < 3: sí; azar `2DE84C`→`2DE974`; `2DE974`→`08346E`\* (corredor); `2DF0FC`→`089948`\*; `2DF108`→`089948`\* |
| 275636 | 1410 | camera wait 1448 | 36 | si group8 < 3: no (×11); si group8 < 3: sí; azar `2DE84C`→`2DE974`; `2DE974`→`08346E`\* (corredor); si group7 == 0: sí; si group3 < 4: no; si group1 < 4: no; corredor `2DF11A` (cada 30, 12 veces, hasta 1567); corredor `2DF148` (cada 60, 12 veces, hasta 1567); corredor `2DF182` (cada 30, 12 veces, hasta 1567) |
| 275672 | 1449 | camera wait 1472 | 18 | si group3 < 4: no; si group2 < 2: sí; azar `2DF15A`→`2DF168`; `2DF168`→`08995A`\* (corredor); si group1 < 4: no; si group8 < 3: no (×6); `2DF1B6`→`0BC3A6`\* |
| 275690 | 1472 | camera wait 1567 | 81 | si group8 < 3: no (×26); si group8 < 3: sí; azar `2DE84C`→`2DE8DA`; `2DE8DA`→`08346E`\* (corredor); si group3 < 4: sí; azar `2DF194`→`2DF1A2`; `2DF1A2`→`08996A`\* (corredor); si group1 < 4: no; corredor `2DF1C8` (cada 30, 12 veces, hasta 1569); corredor `2DF20E` (cada 60, 12 veces, hasta 1569); corredor `2DF26C` (cada 30, 12 veces, hasta 1569) |
| 275771 | 1568 | camera wait 1568 + lock 1568 | 0 | — |
| 275771 | 1568 | camera wait 1568 + lock 1568 | 0 | — |
| 275771 | 1568 | condition wait group7 == 0 | 125 | si group7 == 0: no (×2); si group2 < 2: sí; azar `2DF220`→`2DF222`; `2DF222`→`08995A`\* (corredor); si group1 < 4: no; si group8 < 3: no (×42); si group1 < 4: sí; azar `2DF1DA`→`2DF200`; `2DF200`→`089948`\* (corredor); camera unlock |
| 275896 | 1568 | camera wait 1569 | 3 | si group8 < 3: no; corredor `2DF2E2` (cada 30, 8 veces, hasta 1872); corredor `2DF328` (cada 60, 8 veces, hasta 1872); corredor `2DF36E` (cada 30, 8 veces, hasta 1872) |
| 275899 | 1571 | camera wait 1584 | 9 | si group3 < 4: no; si group2 < 2: sí; azar `2DF33A`→`2DF360`; `2DF360`→`08995A`\* (corredor); si group1 < 4: sí; azar `2DF2F4`→`2DF31A`; `2DF31A`→`089948`\* (corredor); si group8 < 3: no (×2); si group8 < 3: sí; azar `2DE84C`→`2DE94A`; `2DE94A`→`08346E`\* (corredor); `2DF3BA`→`089948`\*; `2DF3C6`→`089948`\*; `2DF3D2`→`08995A`\*; `2DF3DE`→`08996A`\*; `2DF3EA`→`08996A`\* |
| 275908 | 1588 | camera wait 1616 | 13 | si group8 < 3: no (×4); `2DF3FC`→`0BC4F4`\* |
| 275921 | 1621 | camera wait 1712 | 35 | si group8 < 3: no (×7); si group8 < 3: sí (×3); azar `2DE84C`→`2DE86A`; `2DE86A`→`08346E`\* (corredor); azar `2DE84C`→`2DE878`; `2DE878`→`08346E`\* (corredor); azar `2DE84C`→`2DE904`; `2DE904`→`08346E`\* (corredor); `2DF40E`→`089948`\*; `2DF41A`→`089948`\*; `2DF426`→`089948`\*; `2DF432`→`08995A`\*; `2DF43E`→`08995A`\* |
| 275956 | 1718 | camera wait 1760 | 24 | si group8 < 3: no (×5); si group8 < 3: sí; azar `2DE84C`→`2DE85C`; `2DE85C`→`08346E`\* (corredor); `2DF450`→`09A692` |
| 275980 | 1765 | camera wait 1808 | 42 | si group8 < 3: no (×12); si group8 < 3: sí; azar `2DE84C`→`2DE974`; `2DE974`→`08346E`\* (corredor); si group3 < 4: no; si group1 < 4: no; `2DF46C`→`0AF17E`; `2DF482`→`0AF17E` |
| 276022 | 1809 | camera wait 1888 | 63 | si group8 < 3: no (×19); si group8 < 3: sí (×2); azar `2DE84C`→`2DE912`; `2DE912`→`08346E`\* (corredor); azar `2DE84C`→`2DE8A2`; `2DE8A2`→`08346E`\* (corredor); `2DF49E`→`087E36` |
| 276085 | 1890 | camera wait 2000 | 78 | si group8 < 3: no (×31); si group8 < 3: sí; azar `2DE84C`→`2DE93C`; `2DE93C`→`08346E`\* (corredor); corredor `2DF4B8` (cada 90, 16 veces, hasta 2448) |
| 276163 | 2001 | camera wait 2064 + lock 2064 | 37 | si group4 < 3: sí; `2DF4CA`→`0AF17E` (corredor); si group8 < 3: no (×17); si group8 < 3: sí; azar `2DE84C`→`2DE9AC`; `2DE9AC`→`08346E`\* (corredor) |
| 276200 | 2064 | camera wait 2064 + lock 2064 | 0 | — |
| 276200 | 2064 | condition wait group5 < 1 | 0 | `2DF502`→`08FF7E`\*; `2DF512`→`099DCC` |
| 276200 | 2064 | timer wait 90 | 220 | si group8 < 3: no (×90); si group4 < 3: sí; `2DF4CA`→`0AF17E` (corredor); `2DF528`→`08FF7E`\* |
| 276420 | 2064 | condition wait group5 < 2 | 0 | `2DF53E`→`08FF7E`\* |
| 276420 | 2064 | condition wait group5 < 1 | 95 | si group8 < 3: no (×46); camera unlock; `2DF556`→`01913C`; `2DF562`→`08FF7E`\* |
| 276515 | 2064 | camera wait 2168 | 51 | si group8 < 3: no (×16); si group8 < 3: sí; azar `2DE84C`→`2DE8E8`; `2DE8E8`→`08346E`\* (corredor); `2DF578`→`099DCC` |
| 276566 | 2172 | camera wait 2192 | 9 | si group8 < 3: no (×3); corredor `2DF590` (cada 120, 5 veces, hasta 2464) |
| 276575 | 2199 | camera wait 2448 | 186 | si group5 < 1: no; si group8 < 3: no (×56); si group8 < 3: sí (×6); azar `2DE84C`→`2DE886`; `2DE886`→`08346E`\* (corredor); si group4 < 3: sí; `2DF4CA`→`0AF17E` (corredor); azar `2DE84C`→`2DE8F6`; `2DE8F6`→`08346E`\* (corredor); azar `2DE84C`→`2DE958` (×2); `2DE958`→`08346E`\* (corredor) (×2); azar `2DE84C`→`2DE982`; `2DE982`→`08346E`\* (corredor); azar `2DE84C`→`2DE85C`; `2DE85C`→`08346E`\* (corredor); corredor `2DF5B8` (cada 90, 16 veces, hasta 2464) |
| 276761 | 2448 | camera wait 2472 + lock 2472 | 13 | si group4 < 3: sí; azar `2DF5CA`→`2DF5CC`; `2DF5CC`→`0AF17E` (corredor); si group8 < 3: sí; azar `2DE84C`→`2DE8E8`; `2DE8E8`→`08346E`\* (corredor); si group8 < 3: no (×5) |
| 276774 | 2472 | camera wait 2472 + lock 2472 | 0 | — |
| 276774 | 2472 | condition wait groups0..7 == 0 | 288 | si group8 < 3: no (×144); camera unlock |
| 277062 | 2472 | camera wait 2496 | 10 | si group8 < 3: no (×4); si group8 < 3: sí; azar `2DE84C`→`2DE8E8`; `2DE8E8`→`08346E`\* (corredor); si player_query_13 == 1810: no; si player_query_15 == 1802: sí; `2DF63E`→`09692C` |
| 277072 | 2499 | camera wait 2560 | 14 | si group8 < 3: no (×7); `2DF64E`→`099DCC`; `2DF660`→`01913C`; `2DF66C`→`0A0C00`\* |
| 277086 | 2560 | end | — | si group8 < 3: sí (×2); azar `2DE84C`→`2DE8E8`; `2DE8E8`→`08346E`\* (corredor); azar `2DE84C`→`2DE8A2`; `2DE8A2`→`08346E`\* (corredor); si group8 < 3: no (×7057); **fin `018870`**; misión→`0308C0`\*; misión→`06090A`\* |
