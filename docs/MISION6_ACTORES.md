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

## Resumen

| | Constructores | Creaciones |
| --- | --- | --- |
| nativos (compartidos con misiones anteriores) | 115 | 16790 |
| nativos en otro módulo (fin de misión, aterrizaje, guion, efectos) | 8 | 83 |
| **nuevos, sin portar** | 134 | 3582 |
| total | 257 | 20455 |

De los 134 sin portar, 11 ya estaban en el inventario de una misión anterior (los porta su fase 2: M3, M4, M5) y **123 son propios de esta misión**.

Secciones (fotograma de `run4` en que empieza cada una): 1 costa desde 292684, 2 fabrica desde 304273, 3 final desde 322341.

Registros del guion que `run4` no ejecutó (ramas `si`/azar; a capturar en la fase 2): 66.

## Nuevos (sin portar): 134 constructores, 3582 creaciones

Por familia (raíz = registro del guion, llamada de la lista de cámara o actor de misión del que descienden), en orden de primera aparición:

- `01F836` (2): `01F836` `01FBAC`; sección 1, primera cámara 0, 3 creaciones
- `0BCE2C` (1): `0BCE2C`; sección 1, primera cámara 0, 1 creaciones
- `0BD18E` (1): `0BD18E`; sección 1, primera cámara 0, 1 creaciones
- `0BD2BA` (3): `0BD2BA` `0BCFA4` `0BCFC4`; sección 1, primera cámara 0, 13 creaciones
- `0BD2CE` (3): `0BD2CE` `0BCFE4` `0BD004`; sección 1, primera cámara 0, 17 creaciones
- `0BD822` (2): `0BD822` `0BD8F4`; sección 1, primera cámara 0, 2 creaciones
- `0BD836` (2): `0BD836` `0BD906`; sección 1, primera cámara 0, 2 creaciones
- `0BD84A` (2): `0BD84A` `0BD918`; sección 1, primera cámara 0, 2 creaciones
- `0BDA4E` (1): `0BDA4E`; sección 1, primera cámara 0, 3 creaciones
- `common` (12): `01FF82` `01C950` `0BE700` `0BE7CC` `087458` `0876CC` `087460` `087716` `0BE592` `045CD2` `01C854` `08CF6E`; sección 1, primera cámara 0, 493 creaciones
- `0BDAB8` (1): `0BDAB8`; sección 1, primera cámara 32, 1 creaciones
- `01F844` (2): `01F844` `01FABA`; sección 1, primera cámara 224, 9 creaciones
- `0BD4D4` (1): `0BD4D4`; sección 1, primera cámara 1024, 2 creaciones
- `0BD416` (1): `0BD416`; sección 1, primera cámara 1056, 1 creaciones
- `0BD444` (1): `0BD444`; sección 1, primera cámara 1056, 1 creaciones
- `0BD472` (1): `0BD472`; sección 1, primera cámara 1056, 1 creaciones
- `020166` (2): `020166` `020372`; sección 1, primera cámara 1329, 10 creaciones
- `03B05A` (1): `03B05A`; sección 1, primera cámara 1360, 1 creaciones
- `08571C` (1): `08571C`; sección 1, primera cámara 1537, 2 creaciones
- `0AF1C4` (1): `03C16C`; sección 1, primera cámara 1608, 4 creaciones
- `0BD4A6` (1): `0BD4A6`; sección 1, primera cámara 1792, 1 creaciones
- `0530F4` (1): `0530F4`; sección 1, primera cámara 2352, 1 creaciones
- `05310E` (16): `05310E` `0539C4` `0539D2` `0539E0` `0539EE` `053A48` `053CB2` `053B38` `053A5C` `05390C` `053E40` `053F0A` `0540EA` `0542B0` `05439E` `0541F2`; sección 1, primera cámara 2352, 140 creaciones
- `player` (1): `0C53F6`; sección 1, primera cámara 2352, 91 creaciones
- `0BD6E4` (1): `0BD6E4`; sección 1, primera cámara 2560, 1 creaciones
- `0BD590` (1): `0BD590`; sección 1, primera cámara 3440, 1 creaciones
- `0BD5CA` (1): `0BD5CA`; sección 1, primera cámara 3440, 1 creaciones
- `0BE02C` (1): `0BE02C`; sección 2, primera cámara 0, 1 creaciones
- `0BE082` (1): `0BE082`; sección 2, primera cámara 0, 1 creaciones
- `0BE0D8` (2): `0BE0D8` `0BE5E4`; sección 2, primera cámara 0, 2 creaciones
- `0BE180` (1): `0BE180`; sección 2, primera cámara 0, 1 creaciones
- `0BE1D6` (1): `0BE1D6`; sección 2, primera cámara 0, 1 creaciones
- `0BE27E` (1): `0BE27E`; sección 2, primera cámara 0, 1 creaciones
- `032946` (2): `032946` `032AA4`; sección 2, primera cámara 290, 2 creaciones
- `086712` (1): `086712`; sección 2, primera cámara 1040, 58 creaciones
- `0BE460` (1): `0BE460`; sección 2, primera cámara 1040, 4 creaciones
- `086728` (1): `086728`; sección 2, primera cámara 1104, 42 creaciones
- `04115A` (1): `03BEB4`; sección 2, primera cámara 1512, 5 creaciones
- `0B0B94` (1): `0B0B94`; sección 2, primera cámara 1792, 1 creaciones
- `0A2EFA` (1): `0A2EFA`; sección 2, primera cámara 2672, 1 creaciones
- `0676A8` (1): `0676A8`; sección 2, primera cámara 3600, 1 creaciones
- `0676B6` (1): `0676B6`; sección 2, primera cámara 3642, 1 creaciones
- `08FF7E` (4): `08FF7E` `090A62` `091290` `0912EE`; sección 2, primera cámara 3872, 8 creaciones
- `0BE8A2` (1): `0BE8A2`; sección 2, primera cámara 3952, 1 creaciones
- `08AFA8` (1): `08AFA8`; sección 2, primera cámara 4101, 1 creaciones
- `0AF1A8` (1): `0AF1A8`; sección 2, primera cámara 4101, 4 creaciones
- `0BE8F6` (1): `0BE8F6`; sección 2, primera cámara 4240, 1 creaciones
- `0BEADC` (1): `0BEADC`; sección 3, primera cámara 0, 1 creaciones
- `04153E` (45): `04153E` `04157A` `042BDC` `042E22` `04307E` `0431E0` `0444E8` `045428` `045608` `0BEA82` `0436B6` `037404` `03762A` `03790C` `037FBC` `03848A` `03858A` `03825C` `037CEC` `038604` `038680` `0438A4` `038702` `04416E` `0441CA` `038092` `0381D2` `037E04` `037B00` `043860` `042F04` `0443BE` `042AEA` `04445A` `044A08` `044B32` `045EF6` `046112` `0465FE` `04359E` `08DD2C` `08EB18` `04611C` `046510` `08F1A0`; sección 3, primera cámara 1, 2639 creaciones

### Sección 1 (costa): 53 constructores nuevos que aparecen aquí por primera vez, por cámara

| Constructor | Creaciones | Primer fotograma | Cámara | Por sección | Creadores | Programas | Sonidos | Familia (raíz) |
| --- | --- | --- | --- | --- | --- | --- | --- | --- |
| `01F836` | 1 | 292686 | 0 | 1:1 | `script`×1 | 0 |  | 01F836 |
| `0BCE2C` | 1 | 292686 | 0 | 1:1 | `mission`×1 | 0 |  | 0BCE2C |
| `0BD18E` | 1 | 292686 | 0 | 1:1 | `mission`×1 | 0 |  | 0BD18E |
| `0BD2BA` | 1 | 292686 | 0 | 1:1 | `mission`×1 | 0 |  | 0BD2BA |
| `0BD2CE` | 1 | 292686 | 0 | 1:1 | `mission`×1 | 0 |  | 0BD2CE |
| `0BD822` | 1 | 292686 | 0 | 1:1 | `mission`×1 | 0 |  | 0BD822 |
| `0BD836` | 1 | 292686 | 0 | 1:1 | `mission`×1 | 0 |  | 0BD836 |
| `0BD84A` | 1 | 292686 | 0 | 1:1 | `mission`×1 | 0 |  | 0BD84A |
| `0BD8F4` | 1 | 292686 | 0 | 1:1 | `0BD822`×1 | 1 |  | 0BD822 |
| `0BD906` | 1 | 292686 | 0 | 1:1 | `0BD836`×1 | 1 |  | 0BD836 |
| `0BD918` | 1 | 292686 | 0 | 1:1 | `0BD84A`×1 | 1 |  | 0BD84A |
| `0BDA4E` | 3 | 292686 | 0 | 1:3 | `mission`×3 | 1 |  | 0BDA4E |
| `01FBAC` | 2 | 292689 | 0 | 1:2 | `01F836`×2 | 1 | 1053 | 01F836 |
| `01FF82` (M4) | 9 | 292689 | 0 | 1:9 | `01FABA`×7, `01FBAC`×2 | 1 |  | common |
| `0BDAB8` | 1 | 292824 | 32 | 1:1 | `mission`×1 | 1 |  | 0BDAB8 |
| `01F844` | 2 | 293025 | 224 | 1:2 | `script`×2 | 0 |  | 01F844 |
| `01FABA` (M4) | 7 | 293029 | 228 | 1:7 | `01F844`×7 | 2 | 1021 1053 | 01F844 |
| `0BD4D4` | 2 | 293934 | 1024 | 1:2 | `mission`×2 | 1 |  | 0BD4D4 |
| `0BD416` | 1 | 293958 | 1056 | 1:1 | `mission`×1 | 1 |  | 0BD416 |
| `0BD444` | 1 | 293958 | 1056 | 1:1 | `mission`×1 | 1 |  | 0BD444 |
| `0BD472` | 1 | 293958 | 1056 | 1:1 | `mission`×1 | 1 |  | 0BD472 |
| `0BCFA4` | 6 | 293994 | 1056 | 1:6 | `0BD2BA`×6 | 1 |  | 0BD2BA |
| `0BCFC4` | 6 | 293994 | 1056 | 1:6 | `0BD2BA`×6 | 1 |  | 0BD2BA |
| `020166` | 5 | 294458 | 1329 | 1:5 | `01A422`×5 | 2 | 1021 | 020166 |
| `020372` | 5 | 294458 | 1329 | 1:5 | `020166`×5 | 1 |  | 020166 |
| `03B05A` | 1 | 294486 | 1360 | 1:1 | `mission`×1 | 0 |  | 03B05A |
| `08571C` (M4) | 2 | 294666 | 1537 | 1:2 | `script`×2 | 1 |  | 08571C |
| `03C16C` | 4 | 294745 | 1608 | 1:4 | `0AF1C4`×4 | 1 |  | 0AF1C4 |
| `0BCFE4` | 8 | 294835 | 1701 | 1:8 | `0BD2CE`×8 | 1 |  | 0BD2CE |
| `0BD004` | 8 | 294835 | 1701 | 1:8 | `0BD2CE`×8 | 1 |  | 0BD2CE |
| `0BD4A6` | 1 | 294931 | 1792 | 1:1 | `mission`×1 | 1 |  | 0BD4A6 |
| `0530F4` | 1 | 296857 | 2352 | 1:1 | `script`×1 | 0 |  | 0530F4 |
| `05310E` | 1 | 296857 | 2352 | 1:1 | `script`×1 | 12 | 10F4 113C 113D 113F | 05310E |
| `0539C4` | 1 | 296857 | 2352 | 1:1 | `05310E`×1 | 0 |  | 05310E |
| `0539D2` | 1 | 296857 | 2352 | 1:1 | `05310E`×1 | 0 |  | 05310E |
| `0539E0` | 1 | 296857 | 2352 | 1:1 | `05310E`×1 | 0 |  | 05310E |
| `0539EE` | 1 | 296857 | 2352 | 1:1 | `05310E`×1 | 0 |  | 05310E |
| `053A48` | 5 | 297436 | 2352 | 1:5 | `05310E`×5 | 1 |  | 05310E |
| `053CB2` | 76 | 297439 | 2352 | 1:76 | `05310E`×76 | 3 |  | 05310E |
| `053B38` | 8 | 297536 | 2352 | 1:8 | `05310E`×8 | 2 | 1027 | 05310E |
| `053A5C` | 14 | 297916 | 2352 | 1:14 | `05310E`×14 | 1 |  | 05310E |
| `05390C` | 1 | 301751 | 2352 | 1:1 | `05310E`×1 | 0 |  | 05310E |
| `053E40` | 6 | 301813 | 2352 | 1:6 | `053F0A`×6 | 1 |  | 05310E |
| `053F0A` | 1 | 301813 | 2352 | 1:1 | `05439E`×1 | 0 | 1051 1052 | 05310E |
| `0540EA` | 7 | 301813 | 2352 | 1:7 | `05439E`×7 | 1 |  | 05310E |
| `0542B0` | 1 | 301813 | 2352 | 1:1 | `05439E`×1 | 1 |  | 05310E |
| `05439E` | 1 | 301813 | 2352 | 1:1 | `05310E`×1 | 3 | 1161 | 05310E |
| `0C53F6` | 91 | 301871 | 2352 | 1:12 3:79 | `player`×91 | 1 |  | player |
| `0541F2` | 15 | 302035 | 2352 | 1:15 | `05439E`×15 | 1 |  | 05310E |
| `0BD6E4` | 1 | 302469 | 2560 | 1:1 | `mission`×1 | 1 |  | 0BD6E4 |
| `01C950` (M3) | 3 | 303107 | 3265 | 1:3 | `056B00`×3 | 1 |  | common |
| `0BD590` | 1 | 303319 | 3440 | 1:1 | `mission`×1 | 1 |  | 0BD590 |
| `0BD5CA` | 1 | 303319 | 3440 | 1:1 | `mission`×1 | 2 | 1074 | 0BD5CA |

### Sección 2 (fabrica): 32 constructores nuevos que aparecen aquí por primera vez, por cámara

| Constructor | Creaciones | Primer fotograma | Cámara | Por sección | Creadores | Programas | Sonidos | Familia (raíz) |
| --- | --- | --- | --- | --- | --- | --- | --- | --- |
| `0BE02C` | 1 | 304274 | 0 | 2:1 | `mission`×1 | 2 | 1021 | 0BE02C |
| `0BE082` | 1 | 304274 | 0 | 2:1 | `mission`×1 | 2 | 1021 | 0BE082 |
| `0BE0D8` | 1 | 304274 | 0 | 2:1 | `mission`×1 | 1 |  | 0BE0D8 |
| `0BE180` | 1 | 304274 | 0 | 2:1 | `mission`×1 | 2 | 1021 | 0BE180 |
| `0BE1D6` | 1 | 304274 | 0 | 2:1 | `mission`×1 | 3 | 102D | 0BE1D6 |
| `0BE27E` | 1 | 304274 | 0 | 2:1 | `mission`×1 | 2 |  | 0BE27E |
| `0BE5E4` | 1 | 304274 | 0 | 2:1 | `0BE0D8`×1 | 4 |  | 0BE0D8 |
| `0BE700` | 2 | 304274 | 0 | 2:2 | `0BE27E`×1, `0BE1D6`×1 | 1 |  | common |
| `0BE7CC` | 3 | 304274 | 0 | 2:3 | `0BE27E`×1, `0BE1D6`×1, `0BE0D8`×1 | 1 |  | common |
| `032946` | 1 | 304699 | 290 | 2:1 | `script`×1 | 1 |  | 032946 |
| `032AA4` | 1 | 304699 | 290 | 2:1 | `032946`×1 | 1 |  | 032946 |
| `086712` | 58 | 305250 | 1040 | 2:58 | `script`×57, `01A422`×1 | 14 | 10D1 1142 116B | 086712 |
| `0BE460` | 4 | 305250 | 1040 | 2:4 | `script`×4 | 2 | 10A8 | 0BE460 |
| `087458` | 50 | 305502 | 1104 | 2:50 | `086728`×36, `086712`×14 | 3 |  | common |
| `0876CC` | 50 | 305502 | 1104 | 2:50 | `086728`×36, `086712`×14 | 1 |  | common |
| `087460` | 264 | 305574 | 1104 | 2:264 | `086728`×140, `086712`×124 | 3 |  | common |
| `087716` | 90 | 305894 | 1104 | 2:90 | `086712`×59, `086728`×30, `0BE700`×1 | 2 |  | common |
| `086728` | 42 | 305972 | 1104 | 2:42 | `script`×41, `01A422`×1 | 12 | 10D1 1142 116B | 086728 |
| `0BE592` | 2 | 308551 | 1512 | 2:2 | `0BE27E`×1, `0BE1D6`×1 | 1 | 102C | common |
| `03BEB4` | 5 | 308591 | 1512 | 2:5 | `04115A`×5 | 4 | 1022 | 04115A |
| `0B0B94` | 1 | 309913 | 1792 | 2:1 | `mission`×1 | 0 |  | 0B0B94 |
| `0A2EFA` | 1 | 313313 | 2672 | 2:1 | `script`×1 | 5 | 1021 109E | 0A2EFA |
| `0676A8` | 1 | 314099 | 3600 | 2:1 | `script`×1 | 1 |  | 0676A8 |
| `0676B6` | 1 | 314129 | 3642 | 2:1 | `script`×1 | 1 |  | 0676B6 |
| `08FF7E` (M5) | 2 | 320043 | 3872 | 2:2 | `script`×2 | 9 | 1021 1033 | 08FF7E |
| `090A62` (M5) | 2 | 320043 | 3872 | 2:2 | `08FF7E`×2 | 4 | 1064 | 08FF7E |
| `091290` (M5) | 2 | 320375 | 3872 | 2:2 | `090A62`×2 | 1 |  | 08FF7E |
| `0912EE` (M5) | 2 | 320375 | 3872 | 2:2 | `090A62`×2 | 2 | 1025 | 08FF7E |
| `0BE8A2` | 1 | 321563 | 3952 | 2:1 | `mission`×1 | 0 |  | 0BE8A2 |
| `08AFA8` | 1 | 321595 | 4101 | 2:1 | `script`×1 | 2 |  | 08AFA8 |
| `0AF1A8` (M5) | 4 | 321595 | 4101 | 2:4 | `script`×4 | 10 | 103E 1042 1043 | 0AF1A8 |
| `0BE8F6` | 1 | 322150 | 4240 | 2:1 | `script`×1 | 0 |  | 0BE8F6 |

### Sección 3 (final): 49 constructores nuevos que aparecen aquí por primera vez, por cámara

| Constructor | Creaciones | Primer fotograma | Cámara | Por sección | Creadores | Programas | Sonidos | Familia (raíz) |
| --- | --- | --- | --- | --- | --- | --- | --- | --- |
| `0BEADC` | 1 | 322342 | 0 | 3:1 | `mission`×1 | 1 |  | 0BEADC |
| `04153E` | 1 | 322502 | 1 | 3:1 | `script`×1 | 0 |  | 04153E |
| `04157A` | 1 | 322502 | 1 | 3:1 | `04153E`×1 | 3 | 0029 002C 102F 1075 | 04153E |
| `042BDC` | 1 | 322502 | 1 | 3:1 | `04157A`×1 | 5 |  | 04153E |
| `042E22` | 1 | 322502 | 1 | 3:1 | `04157A`×1 | 1 |  | 04153E |
| `04307E` | 1 | 322502 | 1 | 3:1 | `04157A`×1 | 0 |  | 04153E |
| `0431E0` | 1 | 322502 | 1 | 3:1 | `04157A`×1 | 7 | 10CD 10CE 10D0 10D2 | 04153E |
| `0444E8` | 1 | 322502 | 1 | 3:1 | `04157A`×1 | 0 | 1033 | 04153E |
| `045428` | 1 | 322504 | 2 | 3:1 | `04157A`×1 | 0 |  | 04153E |
| `045608` | 8 | 322504 | 2 | 3:8 | `045428`×8 | 21 | 1021 102F 1041 104A | 04153E |
| `0BEA82` | 1 | 322990 | 320 | 3:1 | `045608`×1 | 0 |  | 04153E |
| `0436B6` | 13 | 323520 | 320 | 3:13 | `0431E0`×13 | 6 |  | 04153E |
| `045CD2` | 2 | 323614 | 320 | 3:2 | `01D764`×2 | 1 | 1025 | common |
| `037404` | 1 | 324636 | 320 | 3:1 | `0444E8`×1 | 0 |  | 04153E |
| `03762A` | 42 | 324636 | 320 | 3:42 | `037404`×42 | 2 |  | 04153E |
| `03790C` | 144 | 324636 | 320 | 3:144 | `037404`×144 | 2 |  | 04153E |
| `037FBC` | 1 | 324636 | 320 | 3:1 | `037404`×1 | 0 |  | 04153E |
| `03848A` | 29 | 324636 | 320 | 3:29 | `037FBC`×29 | 3 |  | 04153E |
| `03858A` | 29 | 324636 | 320 | 3:29 | `03848A`×29 | 3 |  | 04153E |
| `03825C` | 488 | 324682 | 320 | 3:488 | `038680`×155, `0381D2`×135, `03848A`×102 | 4 |  | 04153E |
| `037CEC` | 202 | 324806 | 320 | 3:202 | `03762A`×202 | 2 |  | 04153E |
| `038604` | 44 | 324902 | 320 | 3:44 | `037FBC`×44 | 3 |  | 04153E |
| `038680` | 44 | 324902 | 320 | 3:44 | `038604`×44 | 3 |  | 04153E |
| `0438A4` | 61 | 324938 | 320 | 3:61 | `04307E`×61 | 3 | 102C 10D1 | 04153E |
| `038702` | 23 | 324974 | 320 | 3:23 | `038604`×23 | 3 |  | 04153E |
| `04416E` | 370 | 325184 | 320 | 3:370 | `0438A4`×370 | 1 |  | 04153E |
| `0441CA` | 362 | 325204 | 320 | 3:362 | `04416E`×362 | 3 |  | 04153E |
| `038092` | 41 | 325376 | 320 | 3:41 | `037FBC`×41 | 3 |  | 04153E |
| `0381D2` | 41 | 325376 | 320 | 3:41 | `038092`×41 | 2 |  | 04153E |
| `037E04` | 131 | 325392 | 320 | 3:131 | `03790C`×131 | 2 |  | 04153E |
| `037B00` | 379 | 325436 | 320 | 3:379 | `03762A`×199, `03790C`×180 | 1 |  | 04153E |
| `043860` | 12 | 330446 | 320 | 3:12 | `0436B6`×12 | 1 |  | 04153E |
| `042F04` | 55 | 334674 | 320 | 3:55 | `042E22`×55 | 2 | 1022 | 04153E |
| `0443BE` | 1 | 347134 | 320 | 3:1 | `04157A`×1 | 0 |  | 04153E |
| `042AEA` | 1 | 347136 | 320 | 3:1 | `04157A`×1 | 1 |  | 04153E |
| `04445A` | 2 | 347178 | 320 | 3:2 | `04157A`×2 | 0 |  | 04153E |
| `01C854` | 14 | 347404 | 320 | 3:14 | `01D764`×14 | 1 |  | common |
| `044A08` | 5 | 347404 | 320 | 3:5 | `04157A`×5 | 0 |  | 04153E |
| `044B32` | 2 | 347625 | 320 | 3:2 | `04157A`×2 | 1 |  | 04153E |
| `045EF6` | 1 | 349091 | 320 | 3:1 | `04157A`×1 | 0 |  | 04153E |
| `046112` | 15 | 349111 | 320 | 3:15 | `045EF6`×15 | 6 | 103E 1041 1043 | 04153E |
| `0465FE` | 8 | 349331 | 320 | 3:8 | `046112`×8 | 2 | 0000 | 04153E |
| `04359E` | 25 | 349569 | 320 | 3:25 | `0431E0`×25 | 3 |  | 04153E |
| `08DD2C` | 13 | 349723 | 320 | 3:13 | `045EF6`×13 | 9 | 1021 1033 | 04153E |
| `08EB18` (M5) | 13 | 349725 | 320 | 3:13 | `08DD2C`×13 | 4 | 1025 1064 | 04153E |
| `04611C` | 11 | 349878 | 320 | 3:11 | `045EF6`×11 | 8 | 1041 1043 | 04153E |
| `046510` | 6 | 350883 | 320 | 3:6 | `04611C`×5, `09BA40`×1 | 1 |  | 04153E |
| `08F1A0` | 7 | 352895 | 320 | 3:7 | `08EB18`×7 | 2 |  | 04153E |
| `08CF6E` (M4) | 4 | 354692 | 320 | 3:4 | `08C374`×4 | 1 |  | common |

## Nativos ya portados (compartidos): 115 constructores, 16790 creaciones

Los que crea directamente el guion, un corredor del guion o el actor de misión:

| Constructor | Creaciones | Primer fotograma | Cámara | Por sección | Creadores | Programas | Sonidos | Familia (raíz) |
| --- | --- | --- | --- | --- | --- | --- | --- | --- |
| `036C34` | 17 | 292686 | 0 | 1:13 2:4 | `script`×17 | 4 | 1036 109F | 036C34 |
| `08A732` | 16 | 292686 | 0 | 1:16 | `script`×16 | 5 | 103E 1041 1042 1043 | 08A732 |
| `08C172` | 1 | 292686 | 0 | 1:1 | `script`×1 | 1 |  | 08C172 |
| `09A67E` | 3 | 292686 | 0 | 1:1 2:2 | `script`×3 | 0 |  | 09A67E |
| `0B833A` | 1 | 292686 | 0 | 1:1 | `script`×1 | 1 |  | 0B833A |
| `05287C` | 3 | 292736 | 0 | 1:1 2:1 3:1 | `mission`×3 | 2 | 10D5 | 05287C |
| `04115A` | 31 | 292824 | 32 | 1:3 2:28 | `01A422`×20, `script`×11 | 15 | 1041 1042 1043 | 04115A |
| `05C58C` | 1 | 292863 | 90 | 1:1 | `script`×1 | 5 | 1041 1042 | 05C58C |
| `0AF1B6` | 2 | 292869 | 99 | 1:2 | `script`×2 | 10 | 103E 1041 1042 | 0AF1B6 |
| `099E06` | 3 | 293417 | 609 | 1:2 2:1 | `script`×3 | 6 | 1048 113A | 099E06 |
| `0AF17E` | 59 | 293543 | 736 | 1:40 2:19 | `01A422`×42, `script`×17 | 29 | 103E 1040 1041 1042 | 0AF17E |
| `09A692` | 5 | 293665 | 752 | 1:3 2:2 | `script`×5 | 0 |  | 09A692 |
| `0569E6` | 53 | 293748 | 817 | 1:10 2:43 | `script`×33, `01A422`×20 | 14 | 103E 1040 1041 1042 | 0569E6 |
| `05D916` | 4 | 293913 | 980 | 1:4 | `script`×2, `01A422`×2 | 9 | 103E 1041 1042 1043 | 05D916 |
| `01913C` | 7 | 294286 | 1056 | 1:4 2:2 3:1 | `script`×7 | 0 |  | 01913C |
| `0AF1C4` | 12 | 294407 | 1265 | 1:12 | `01A422`×12 | 19 | 103E 1041 1042 1043 | 0AF1C4 |
| `09AE2C` | 5 | 294458 | 1329 | 1:5 | `script`×5 | 6 | 103E 1041 1042 1043 | 09AE2C |
| `09AE1E` | 6 | 296711 | 1923 | 1:6 | `script`×5, `01A422`×1 | 11 | 103E 1041 1042 1043 | 09AE1E |
| `01F1D4` | 1 | 302399 | 2441 | 1:1 | `script`×1 | 0 |  | 01F1D4 |
| `09BA2A` | 2 | 302508 | 2690 | 1:2 | `script`×2 | 2 | 103E 1042 | 09BA2A |
| `099DCC` | 6 | 302559 | 2752 | 1:1 2:5 | `script`×6 | 8 | 1048 1056 113A | 099DCC |
| `05ED84` | 1 | 303344 | 3464 | 1:1 | `script`×1 | 18 | 1021 102F 1064 | 05ED84 |
| `0569B4` | 5 | 303351 | 3473 | 1:3 2:2 | `01A422`×3, `script`×2 | 12 | 1040 1041 1042 1043 | 0569B4 |
| `0B6ED2` | 2 | 304151 | 3632 | 1:1 2:1 | `mission`×1, `0BE8A2`×1 | 0 |  | common |
| `08DA12` | 2 | 304274 | 0 | 2:2 | `script`×2 | 6 | 1040 1043 | 08DA12 |
| `08DA34` | 8 | 304274 | 0 | 2:8 | `script`×8 | 5 | 103E 1040 1043 | 08DA34 |
| `099DF0` | 1 | 305091 | 786 | 2:1 | `script`×1 | 6 | 1048 1056 113A | 099DF0 |
| `08C37C` | 6 | 308097 | 1104 | 2:6 | `script`×6 | 5 | 1121 | 08C37C |
| `0A2F2E` | 2 | 312657 | 2672 | 2:2 | `script`×2 | 9 | 1021 1033 1064 109E | 0A2F2E |
| `08AFDE` | 2 | 321607 | 4131 | 2:2 | `script`×2 | 2 |  | 08AFDE |

El resto (hijos, disparos, efectos, premios): `03C62A` `03C962` `01BF46` `01B782` `01BC2E` `08AA6E` `03C306` `040F88` `05DF3E` `05DFC2` `06381C` `0C58D2` `063D9E` `06371E` `036EA8` `05DD2A` `01C706` `01C71E` `01CE38` `01CE44` `01D764` `09A97C` `056E5A` `08C374` `05DDA4` `056B00` `056C94` `056A6A` `08AB70` `08AC54` `08AE70` `09C46C` `09B5FC` `08C68E` `01B6DC` `040F60` `01F2D6` `01F386` `01F4DA` `01F562` `09C6B0` `060490` `0602E4` `05FD8A` `059C82` `01D31C` `0897E2` `01C6EE` `08D330` `099E1C` `09A9C4` `01E22C` `01E58A` `01E582` `01B716` `01D86C` `0A38F4` `0A3A0A` `0A3BD6` `0A3C24` `01D944` `0A716E` `0A74E4` `0A751C` `0646B0` `014A4E` `01CE20` `01CE08` `01CE14` `08B3E6` `01C796` `0AE1DC` `01BF10` `063F32` `0641EC` `01D00C` `01D018` `01D024` `01D078` `01D030` `01D06C` `09BA40` `09C76C` `09CAFC` `08EFFA`

En otro módulo: `01A422` (neo_script.c), `03AED8` (neo_wipe.c), `03AEF0` (neo_wipe.c), `03BC28` (neo_soldier_effects.c), `0607DE` (neo_end.c), `060A72` (neo_end.c), `0C4A58` (neo_landing.c), `0C5286` (neo_landing.c).

## Constructores del guion por estado

| Constructor del guion | Estado | Creaciones en `run4` | Secciones |
| --- | --- | --- | --- |
| `01913C` | native | 7 | 1 2 3 |
| `01F1D4` | native | 1 | 1 |
| `01F836` | missing | 1 | 1 |
| `01F844` | missing | 2 | 1 |
| `020166` | missing | 5 | 1 |
| `032946` | missing | 1 | 2 |
| `036C34` | native | 17 | 1 2 |
| `04115A` | native | 31 | 1 2 |
| `04153E` | missing | 1 | 3 |
| `0530F4` | missing | 1 | 1 |
| `05310E` | missing | 1 | 1 |
| `0569B4` | native | 5 | 1 2 |
| `0569E6` | native | 53 | 1 2 |
| `05C58C` | native | 1 | 1 |
| `05D916` | native | 4 | 1 |
| `05ED84` | native | 1 | 1 |
| `0676A8` | missing | 1 | 2 |
| `0676B6` | missing | 1 | 2 |
| `08571C` | missing (M4) | 2 | 1 |
| `086712` | missing | 58 | 2 |
| `086728` | missing | 42 | 2 |
| `08A732` | native | 16 | 1 |
| `08AFA8` | missing | 1 | 2 |
| `08AFDE` | native | 2 | 2 |
| `08C172` | native | 1 | 1 |
| `08C37C` | native | 6 | 2 |
| `08DA12` | native | 2 | 2 |
| `08DA34` | native | 8 | 2 |
| `08FF7E` | missing (M5) | 2 | 2 |
| `099DCC` | native | 6 | 1 2 |
| `099DF0` | native | 1 | 2 |
| `099E06` | native | 3 | 1 2 |
| `09A67E` | native | 3 | 1 2 |
| `09A692` | native | 5 | 1 2 |
| `09AE1E` | native | 6 | 1 |
| `09AE2C` | native | 5 | 1 |
| `09BA2A` | native | 2 | 1 |
| `0A2EFA` | missing | 1 | 2 |
| `0A2F2E` | native | 2 | 2 |
| `0AF17E` | native | 59 | 1 2 |
| `0AF1A8` | missing (M5) | 4 | 2 |
| `0AF1B6` | native | 2 | 1 |
| `0AF1C4` | native | 12 | 1 |
| `0B833A` | native | 1 | 1 |
| `0BE460` | missing | 4 | 2 |
| `0BE8F6` | missing | 1 | 2 |

Constructores del guion que `run4` no creó nunca (solo en ramas no ejecutadas): ninguno.

## Premios (`08C380`)

| Tipo `+6E` | Origen | Soltado | Veces | Secciones |
| --- | --- | --- | --- | --- |
| 00 | `09AE1E` | sí | 1 | 1 |
| 04 | `0438A4` | sí | 2 | 3 |
| 04 | `099DF0` | sí | 1 | 2 |
| 06 | `0438A4` | sí | 1 | 3 |
| 06 | `099DCC` | sí | 1 | 2 |
| 06 | `099E06` | sí | 2 | 1 2 |
| 06 | `0A2F2E` | sí | 1 | 2 |
| 06 | `0BE1D6` | sí | 1 | 2 |
| 16 | `script` | no | 1 | 2 |
| 1A | `0438A4` | sí | 1 | 3 |
| 1B | `script` | no | 1 | 2 |
| 20 | `script` | no | 1 | 2 |
| 22 | `script` | no | 2 | 2 |
| 23 | `script` | no | 1 | 2 |
| 30 | `032946` | sí | 1 | 2 |
| 37 | `0438A4` | sí | 2 | 3 |
| 40 | `0438A4` | sí | 1 | 3 |
| 49 | `05310E` | sí | 1 | 1 |
| 4A | `099E06` | sí | 1 | 1 |
| 4A | `09AE1E` | sí | 1 | 1 |
| 4B | `0438A4` | sí | 2 | 3 |
| 4B | `099DCC` | sí | 1 | 1 |
| 4F | `0438A4` | sí | 5 | 3 |
| 4F | `099DCC` | sí | 1 | 2 |
| 50 | `0A2F2E` | sí | 1 | 2 |
| 51 | `0438A4` | sí | 4 | 3 |
| 52 | `0A2EFA` | sí | 1 | 2 |
| 53 | `0438A4` | sí | 2 | 3 |

## Registros no ejecutados (66)

| Registro | Constructor | Por qué |
| --- | --- | --- |
| `2DF6A2` | `09A67E` | random 2DF6A0 |
| `2DF8AC` | `09A692` | random 2DF894 |
| `2DF8C2` | `09A692` | random 2DF894 |
| `2DF978` | `0AF17E` | random 2DF960 |
| `2DFB9C` | `05310E` | if 2DFB98 |
| `2DFC30` | `0AF17E` | if 2DFC2C |
| `2DFC48` | `0AF17E` | if 2DFC44 |
| `2DFC7C` | `09A692` | random 2DFC7A |
| `2E0156` | `09A67E` | random 2E0142 |
| `2E0168` | `09A67E` | random 2E0142 |
| `2E045A` | `086712` | runner/block 2E0458 |
| `2E0464` | `086712` | runner/block 2E0458 |
| `2E050E` | `099DCC` | if 2E050A |
| `2E0568` | `099DCC` | random 2E0566 |
| `2E057C` | `099DCC` | random 2E0566 |
| `2E0698` | `08C37C` | random 2E0688 |
| `2E07F4` | `09A67E` | random 2E07F2 |
| `2E0808` | `09A67E` | random 2E07F2 |
| `2E0834` | `09A692` | random 2E0832 |
| `2E085C` | `09A692` | random 2E0832 |
| `2E0878` | `09A67E` | random 2E0876 |
| `2E088C` | `09A67E` | random 2E0876 |
| `2E08A0` | `09A67E` | random 2E0876 |
| `2E0A40` | `0569E6` | runner/block 2E0A3E |
| `2E0A50` | `0569E6` | runner/block 2E0A3E |
| `2E0A60` | `0569E6` | runner/block 2E0A3E |
| `2E0A88` | `0569B4` | runner/block 2E0A86 |
| `2E0A9A` | `0569E6` | runner/block 2E0A98 |
| `2E0AAA` | `0569E6` | runner/block 2E0A98 |
| `2E0ABA` | `0569E6` | runner/block 2E0A98 |
| `2E0B8E` | `04115A` | if 2E0B88 |
| `2E0C30` | `0569E6` | runner/block 2E0C2E |
| `2E0C40` | `0569E6` | runner/block 2E0C2E |
| `2E0D12` | `086728` | if 2E0D0E |
| `2E0D2C` | `086728` | if 2E0D28 |
| `2E0E2E` | `086712` | random 2E0E2C |
| `2E0E42` | `086712` | random 2E0E2C |
| `2E0E70` | `086728` | random 2E0E5C |
| `2E0EA0` | `086712` | random 2E0E9E |
| `2E0EB2` | `086728` | random 2E0E9E |
| `2E0EE0` | `086712` | random 2E0ECA |
| `2E10DA` | `0AF17E` | runner/block 2E10D8 |
| `2E10EC` | `0AF17E` | runner/block 2E10D8 |
| `2E10FE` | `0AF17E` | runner/block 2E10D8 |
| `2E13B8` | `086712` | runner/block 2E13B6 |
| `2E13CC` | `086712` | runner/block 2E13B6 |
| `2E1452` | `086712` | runner/block 2E1450 |
| `2E1468` | `086712` | runner/block 2E1450 |
| `2E15B6` | `086712` | runner/block 2E15B4 |
| `2E15CC` | `086712` | runner/block 2E15B4 |
| `2E15E6` | `086712` | runner/block 2E15E4 |
| `2E15FC` | `086712` | runner/block 2E15E4 |
| `2E1680` | `086712` | runner/block 2E167E |
| `2E1696` | `086712` | runner/block 2E167E |
| `2E16B0` | `086712` | runner/block 2E16AE |
| `2E16C6` | `086712` | runner/block 2E16AE |
| `2E16E6` | `086712` | runner/block 2E16E4 |
| `2E16FA` | `086712` | runner/block 2E16E4 |
| `2E174A` | `086712` | runner/block 2E1748 |
| `2E1760` | `086712` | runner/block 2E1748 |
| `2E177A` | `086712` | runner/block 2E1778 |
| `2E1790` | `086712` | runner/block 2E1778 |
| `2E1840` | `086712` | if 2E183C |
| `2E1946` | `08C37C` | runner/block 2E1944 |
| `2E1956` | `08C37C` | runner/block 2E1944 |
| `2E1966` | `08C37C` | runner/block 2E1944 |

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
sections (start frame): 1@292684 2@304273 3@322341
constructors allocated: 257, allocations 20455, native 115

#### Sección 1 (costa)

| Fotograma | Cámara | Orden que espera | Fotogramas | Después |
| --- | --- | --- | --- | --- |
| 292686 | 0 | (inicio) | — | cámara `0BCB0C`→`0BD2CE`\*; cámara `0BCB0C`→`0BD2BA`\*; cámara `0BCB0C`→`0BCE2C`\*; cámara `0BCB0C`→`0BD18E`\*; cámara `0BCB24`→`0BD822`\*; cámara `0BCB24`→`0BD836`\*; cámara `0BCB24`→`0BD84A`\*; cámara `0BCB24`→`0BDA4E`\* (×3) |
| 292686 | 0 | camera wait 0 | 0 | `2DF67E`→`01F836`\*; `2DF690`→`0B833A`; azar `2DF6A0`→`2DF6B4`; `2DF6B4`→`09A67E`; `2DF6C8`→`08C172` |
| 292686 | 0 | camera wait 0 | 0 | `2DF6DA`→`08A732`; `2DF6E8`→`036C34` |
| 292686 | 0 | camera wait 32 | 138 | misión→`0607DE`\*; misión→`05287C`; cámara `0BCB32`→`0BDAB8`\*; `2DF6F8`→`036C34`; `2DF702`→`04115A` |
| 292824 | 32 | camera wait 88 | 39 | `2DF71E`→`05C58C` |
| 292863 | 90 | camera wait 96 | 6 | `2DF734`→`0AF1B6` |
| 292869 | 99 | camera wait 128 | 40 | `2DF750`→`0AF1B6` |
| 292909 | 128 | camera wait 152 | 44 | `2DF76C`→`04115A` |
| 292953 | 155 | camera wait 224 | 72 | `2DF788`→`01F844`\* |
| 293025 | 224 | camera wait 272 | 54 | `2DF7A0`→`036C34`; `2DF7AA`→`04115A` |
| 293079 | 272 | camera wait 384 | 99 | `2DF7C6`→`036C34`; `2DF7D0`→`08A732` |
| 293178 | 385 | camera wait 432 | 53 | `2DF7E4`→`01F844`\* |
| 293231 | 433 | camera wait 536 | 120 | `2DF7FC`→`036C34`; `2DF806`→`08A732` |
| 293351 | 539 | camera wait 608 | 66 | `2DF81A`→`099E06` |
| 293417 | 609 | camera wait 736 | 126 | corredor `2DF832` (cada 45, 3 veces, hasta 864) |
| 293543 | 736 | camera wait 736 | 0 | `2DF860`→`036C34`; `2DF86C`→`08A732` |
| 293543 | 736 | camera wait 744 | 107 | si group1 < 2: sí; `2DF844`→`0AF17E` (corredor); `2DF880`→`08A732` |
| 293650 | 744 | camera wait 752 | 15 | si group1 < 2: sí; `2DF844`→`0AF17E` (corredor); azar `2DF894`→`2DF896`; `2DF896`→`09A692` |
| 293665 | 752 | camera wait 816 | 83 | `2DF8E0`→`036C34`; `2DF8EC`→`0569E6` |
| 293748 | 817 | camera wait 888 | 66 | si group1 < 2: no; `2DF902`→`0569E6` |
| 293814 | 890 | camera wait 944 | 70 | `2DF918`→`036C34`; `2DF924`→`0569E6` |
| 293884 | 945 | camera wait 976 | 29 | `2DF93A`→`05D916`; corredor `2DF94E` (cada 30, 3 veces, hasta 1048) |
| 293913 | 980 | camera wait 992 | 6 | si group1 < 3: sí; azar `2DF960`→`2DF962`; `2DF962`→`0AF17E` (corredor); `2DF996`→`05D916` |
| 293919 | 994 | camera wait 1056 + lock 1056 | 39 | cámara `0BCB72`→`0BD4D4`\*; cámara `0BCBB2`→`0BD416`\*; cámara `0BCBB2`→`0BD444`\*; cámara `0BCBB2`→`0BD472`\* |
| 293958 | 1056 | camera wait 1056 + lock 1056 | 0 | corredor `2DF9C6` (cada 30, 1 veces, hasta 65535); corredor `2DF9E6` (cada 30, 1 veces, hasta 65535) |
| 293958 | 1056 | condition wait groups0..7 == 0 | 328 | `2DF9D2`→`05D916` (corredor); `2DF9F2`→`05D916` (corredor); camera unlock; `2DFA0E`→`09A692`; `2DFA24`→`01913C` |
| 294286 | 1056 | camera wait 1264 | 121 | corredor `2DFA36` (cada 5, 12 veces, hasta 2064) |
| 294407 | 1265 | camera wait 1328 | 51 | si group1 < 2: sí (×2); `2DFA48`→`0AF1C4` (corredor) (×2); si group1 < 2: no (×2); corredor `2DFA62` (cada 10, 12 veces, hasta 2048) |
| 294458 | 1329 | camera wait 1328 | 0 | `2DFA8E`→`09AE2C`; `2DFAA0`→`08A732` |
| 294458 | 1329 | camera wait 1328 | 0 | corredor `2DFAB4` (cada 30, 32 veces, hasta 1792) |
| 294458 | 1329 | camera wait 1536 | 208 | si group5 < 7: sí (×2); azar `2DFAC6`→`2DFAD4`; `2DFAD4`→`020166`\* (corredor); si group2 < 1: sí (×3); `2DFA74`→`0AF17E` (corredor) (×3); si group1 < 2: no (×8); cámara `0BCBEA`→`03B05A`\*; si group2 < 1: no (×3); si group1 < 2: sí (×2); `2DFA48`→`0AF1C4` (corredor) (×2); azar `2DFAC6`→`2DFAE0`; `2DFAE0`→`020166`\* (corredor); `2DFAF4`→`08571C`\*; `2DFB04`→`08571C`\* |
| 294666 | 1537 | camera wait 1800 | 282 | si group5 < 7: sí (×3); azar `2DFAC6`→`2DFAC8` (×2); `2DFAC8`→`020166`\* (corredor) (×2); si group1 < 2: no (×9); si group2 < 1: sí (×5); `2DFA74`→`0AF17E` (corredor) (×5); si group1 < 2: sí (×4); `2DFA48`→`0AF1C4` (corredor) (×4); si group2 < 1: no (×2); azar `2DFAC6`→`2DFAD4`; `2DFAD4`→`020166`\* (corredor); cámara `0BCC1A`→`0BD4A6`\*; cámara `0BCC22`→`0BD4D4`\*; `2DFB1A`→`036C34`; `2DFB26`→`08A732` |
| 294948 | 1802 | camera wait 1816 | 24 | si group2 < 1: no; si group1 < 2: no; `2DFB3A`→`08A732` |
| 294972 | 1819 | camera wait 1920 | 1739 | si group1 < 2: no (×27); si group2 < 1: no (×20); si group1 < 2: sí (×4); `2DFA48`→`0AF1C4` (corredor) (×4); si group2 < 1: sí (×4); `2DFA74`→`0AF17E` (corredor) (×4); `2DFB4E`→`09AE1E` |
| 296711 | 1923 | camera wait 2352 + lock 2352 | 146 | `2DFB6E`→`01913C` |
| 296857 | 2352 | camera wait 2352 + lock 2352 | 0 | `2DFB88`→`0530F4`\*; set region 4330; si player_query_0B == 1802: no; si player_query_0C == 1802: sí; `2DFBAA`→`05310E`\* |
| 296857 | 2352 | timer wait 120 | 240 | si player_query_12 == 2060: sí; corredor `2DFBBC` (cada 60, 16 veces, hasta 2353); si player_query_13 == 2060: no |
| 297097 | 2352 | condition wait group7 < 1 | 4656 | si group7 == 1: sí (×19); si group1 < 2: sí (×16); si player_query_0B == 1812: no (×10); si player_query_0C == 1812: sí (×10); `2DFBF4`→`0AF17E` (corredor) (×10); si group1 < 2: no (×3); si player_query_0B == 1812: sí (×6); `2DFBDC`→`0AF17E` (corredor) (×6); si player_query_0C == 1812: no (×6); camera unlock; `2DFC68`→`01913C` |
| 301753 | 2352 | camera wait 2353 | 618 | azar `2DFC7A`→`2DFC92`; `2DFC92`→`09A692` |
| 302371 | 2353 | camera wait 2432 | 28 | `2DFCB0`→`01F1D4` |
| 302399 | 2441 | camera wait 2480 | 46 | `2DFCC0`→`099E06` |
| 302445 | 2483 | camera wait 2560 | 24 | cámara `0BCD4E`→`0BD6E4`\*; `2DFCD8`→`0AF17E`; `2DFCEE`→`0AF17E` |
| 302469 | 2560 | camera wait 2656 | 24 | `2DFD0A`→`08A732` |
| 302493 | 2656 | camera wait 2672 | 8 | `2DFD1E`→`09AE1E` |
| 302501 | 2676 | camera wait 2688 | 7 | `2DFD36`→`09BA2A`; `2DFD48`→`09BA2A` |
| 302508 | 2690 | camera wait 2688 | 0 | corredor `2DFD60` (cada 30, 6 veces, hasta 3424) |
| 302508 | 2690 | camera wait 2704 | 9 | si group2 < 1: sí; `2DFD72`→`0AF17E` (corredor); `2DFD8C`→`09AE1E` |
| 302517 | 2705 | camera wait 2704 | 0 | corredor `2DFDA4` (cada 90, 6 veces, hasta 2824) |
| 302517 | 2705 | camera wait 2720 | 15 | si group1 < 2: sí; `2DFDB6`→`0AF17E` (corredor); `2DFDD0`→`036C34`; `2DFDDA`→`08A732` |
| 302532 | 2721 | camera wait 2736 | 15 | `2DFDEE`→`09AE1E` |
| 302547 | 2737 | camera wait 2752 | 12 | `2DFE06`→`099DCC` |
| 302559 | 2752 | camera wait 2976 | 211 | si group2 < 1: sí (×2); `2DFD72`→`0AF17E` (corredor) (×2); `2DFE1E`→`036C34`; `2DFE28`→`08A732`; `2DFE36`→`0569E6` |
| 302770 | 2977 | camera wait 3008 | 24 | si group2 < 1: sí; `2DFD72`→`0AF17E` (corredor); `2DFE4C`→`0569E6` |
| 302794 | 3008 | camera wait 3040 | 20 | `2DFE62`→`0569E6` |
| 302814 | 3042 | camera wait 3056 | 11 | `2DFE78`→`036C34`; `2DFE82`→`08A732`; `2DFE90`→`0569E6` |
| 302825 | 3057 | camera wait 3072 | 64 | si group2 < 1: sí; `2DFD72`→`0AF17E` (corredor); `2DFEA6`→`0569E6` |
| 302889 | 3072 | camera wait 3072 | 0 | corredor `2DFEBC` (cada 30, 4 veces, hasta 3264) |
| 302889 | 3072 | camera wait 3120 | 46 | si group4 < 1: sí; `2DFECE`→`09AE1E` (corredor); corredor `2DFEE6` (cada 30, 8 veces, hasta 3200) |
| 302935 | 3121 | camera wait 3136 | 18 | si group3 < 3: no; si group2 < 1: no; `2DFF0E`→`036C34`; `2DFF18`→`08A732`; `2DFF26`→`0569E6`; `2DFF36`→`08A732` |
| 302953 | 3138 | camera wait 3184 | 54 | si group4 < 1: no; `2DFF4A`→`09AE1E` |
| 303007 | 3185 | camera wait 3288 | 144 | si group3 < 3: sí; `2DFEF8`→`0569E6` (corredor); si group2 < 1: sí; `2DFD72`→`0AF17E` (corredor); si group4 < 1: no; `2DFF62`→`09AE2C`; `2DFF74`→`09AE2C`; `2DFF86`→`08A732` |
| 303151 | 3290 | camera wait 3352 | 94 | `2DFF9A`→`09AE2C`; `2DFFAC`→`09AE2C`; `2DFFBE`→`08A732` |
| 303245 | 3353 | camera wait 3464 | 99 | cámara `0BCD9E`→`0BD590`\*; cámara `0BCD9E`→`0BD5CA`\*; `2DFFD2`→`05ED84` |
| 303344 | 3464 | camera wait 3472 | 7 | corredor `2DFFE8` (cada 30, 3 veces, hasta 3625) |
| 303351 | 3473 | camera wait 3624 + lock 3624 | 156 | si group3 < 2: sí (×2); `2DFFFA`→`0569B4` (corredor) (×2) |
| 303507 | 3624 | camera wait 3624 + lock 3624 | 0 | — |
| 303507 | 3624 | condition wait group7 < 1 | 635 | si group3 < 2: sí; `2DFFFA`→`0569B4` (corredor); camera unlock; `2E002C`→`01913C` |
| 304142 | 3624 | end | 132 | cámara `0BCDAC`→`0B6ED2`; **fin `018F92`**; misión→`03AED8`\*; cámara `0BDD78`→`0BE02C`\*; cámara `0BDD78`→`0BE082`\*; cámara `0BDD78`→`0BE0D8`\*; cámara `0BDD78`→`0BE180`\*; cámara `0BDD78`→`0BE1D6`\*; cámara `0BDD78`→`0BE27E`\* |

#### Sección 2 (fabrica)

| Fotograma | Cámara | Orden que espera | Fotogramas | Después |
| --- | --- | --- | --- | --- |
| 304274 | 0 | camera wait 0 | 0 | set region 4330 |
| 304274 | 0 | camera wait 0 | 0 | `2E004C`→`036C34`; `2E0056`→`08DA12`; `2E0068`→`08DA12`; `2E007A`→`08DA34` |
| 304274 | 0 | camera wait 96 | 256 | misión→`03AEF0`\*; misión→`05287C`; `2E0092`→`08DA34`; `2E00A4`→`036C34`; `2E00AE`→`0AF17E`; `2E00C2`→`0AF17E` |
| 304530 | 96 | camera wait 128 | 24 | `2E00DC`→`0569E6`; `2E00EC`→`0569E6` |
| 304554 | 129 | camera wait 192 | 51 | `2E0102`→`08DA34`; `2E0114`→`0AF17E`; `2E0128`→`0AF17E` |
| 304605 | 192 | camera wait 248 | 53 | azar `2E0142`→`2E0144`; `2E0144`→`09A67E` |
| 304658 | 249 | camera wait 264 | 15 | `2E0182`→`099E06` |
| 304673 | 264 | camera wait 288 | 26 | `2E019C`→`032946`\* |
| 304699 | 290 | camera wait 360 | 46 | `2E01AE`→`08DA34` |
| 304745 | 362 | camera wait 392 | 20 | `2E01C6`→`0AF17E` |
| 304765 | 393 | camera wait 400 | 6 | `2E01E0`→`08DA34` |
| 304771 | 402 | camera wait 432 | 20 | `2E01F8`→`08DA34` |
| 304791 | 432 | camera wait 440 | 6 | `2E0210`→`09A692` |
| 304797 | 442 | camera wait 488 | 26 | `2E022A`→`0AF17E` |
| 304823 | 489 | camera wait 520 | 20 | `2E0244`→`0AF17E` |
| 304843 | 520 | camera wait 576 | 46 | `2E025E`→`0569E6` |
| 304889 | 577 | camera wait 592 | 14 | `2E0274`→`08DA34` |
| 304903 | 592 | camera wait 616 | 31 | `2E028C`→`0569E6` |
| 304934 | 618 | camera wait 624 | 6 | `2E02A2`→`036C34` |
| 304940 | 624 | camera wait 656 | 27 | `2E02B2`→`0569E6` |
| 304967 | 656 | camera wait 680 | 23 | `2E02C8`→`0569E6` |
| 304990 | 680 | camera wait 680 | 0 | `2E02DE`→`08DA34` |
| 304990 | 680 | camera wait 704 | 24 | corredor `2E02F6` (cada 30, 4 veces, hasta 800) |
| 305014 | 705 | camera wait 704 | 0 | `2E0322`→`04115A` |
| 305014 | 705 | camera wait 736 | 32 | si group1 < 2: sí; `2E0308`→`0AF17E` (corredor); `2E033C`→`04115A` |
| 305046 | 736 | camera wait 784 | 45 | `2E0356`→`099DF0` |
| 305091 | 786 | camera wait 792 | 6 | `2E0370`→`04115A` |
| 305097 | 794 | camera wait 880 | 61 | corredor `2E038A` (cada 60, 4 veces, hasta 1040); corredor `2E03B2` (cada 90, 2 veces, hasta 1040) |
| 305158 | 880 | camera wait 1040 | 92 | si group2 < 2: sí; `2E03C4`→`0AF17E` (corredor); si group1 < 1: sí; `2E039C`→`04115A` (corredor); `2E03DE`→`086712`\*; `2E03EA`→`086712`\*; `2E03F6`→`086712`\*; `2E0402`→`0BE460`\* |
| 305250 | 1040 | camera wait 1104 + lock 1104 | 42 | — |
| 305292 | 1104 | camera wait 1104 + lock 1104 | 0 | set region 4330 |
| 305292 | 1104 | timer wait 60 | 120 | `2E0436`→`086712`\*; `2E0440`→`086712`\*; `2E044A`→`086712`\*; si player_query_13 == 2306: no |
| 305412 | 1104 | condition wait group5 < 2 | 560 | `2E0476`→`086728`\*; `2E048C`→`086712`\*; `2E04A2`→`086712`\*; `2E04B8`→`086712`\* |
| 305972 | 1104 | condition wait group5 < 3 | 448 | `2E04D4`→`086712`\*; `2E04DE`→`086712`\*; `2E04E8`→`086712`\*; si player_query_12 == 1812: sí; `2E04F6`→`099DCC`; si player_query_13 == 1812: no; `2E0522`→`0BE460`\*; `2E0530`→`086712`\*; `2E0542`→`086712`\*; `2E0554`→`086712`\*; azar `2E0566`→`2E0590`; `2E0590`→`099DCC`; `2E05A6`→`0BE460`\* |
| 306420 | 1104 | condition wait group5 < 2 | 486 | `2E05BA`→`086712`\*; `2E05CC`→`086728`\*; `2E05DE`→`086712`\*; `2E05F0`→`099DCC`; `2E0604`→`0BE460`\* |
| 306906 | 1104 | condition wait group5 < 2 | 535 | `2E0618`→`086712`\*; `2E062E`→`086712`\* |
| 307441 | 1104 | condition wait group5 < 2 | 134 | `2E064A`→`086712`\*; `2E0660`→`086712`\* |
| 307575 | 1104 | condition wait group5 < 1 | 522 | `2E067C`→`01913C`; azar `2E0688`→`2E068A`; `2E068A`→`08C37C`; camera unlock |
| 308097 | 1104 | camera wait 1120 | 8 | corredor `2E06B0` (cada 90, 3 veces, hasta 1511) |
| 308105 | 1123 | camera wait 1512 + lock 1512 | 182 | si group1 < 2: sí; `2E06C2`→`04115A` (corredor) |
| 308287 | 1512 | camera wait 1512 + lock 1512 | 0 | corredor `2E06F4` (cada 45, 12 veces, hasta 1512); corredor `2E0736` (cada 45, 12 veces, hasta 1512); `2E0778`→`086712`\*; `2E0784`→`086712`\*; `2E0790`→`086712`\* |
| 308287 | 1512 | condition wait group5 < 1 | 450 | si group2 < 1: sí (×2); azar `2E0748`→`2E074A`; `2E074A`→`04115A` (corredor); si group1 < 1: sí (×4); azar `2E0706`→`2E0708` (×3); `2E0708`→`04115A` (corredor) (×3); si group2 < 1: no (×3); azar `2E0706`→`2E071E`; `2E071E`→`04115A` (corredor); si group1 < 1: no; azar `2E0748`→`2E0760`; `2E0760`→`04115A` (corredor); `2E07A2`→`086712`\*; `2E07B8`→`086712`\*; `2E07CE`→`086712`\* |
| 308737 | 1512 | condition wait group5 < 1 | 932 | si group2 < 1: sí (×6); azar `2E0748`→`2E0760` (×4); `2E0760`→`04115A` (corredor) (×4); si group1 < 1: no (×5); si group2 < 1: no (×4); si group1 < 1: sí (×4); azar `2E0706`→`2E071E` (×2); `2E071E`→`04115A` (corredor) (×2); azar `2E0706`→`2E0708` (×2); `2E0708`→`04115A` (corredor) (×2); azar `2E0748`→`2E074A` (×2); `2E074A`→`04115A` (corredor) (×2); camera unlock |
| 309669 | 1512 | camera wait 1616 | 142 | si group1 < 1: sí; azar `2E0706`→`2E0708`; `2E0708`→`04115A` (corredor); si group2 < 1: sí; azar `2E0748`→`2E074A`; `2E074A`→`04115A` (corredor); azar `2E07F2`→`2E081C`; `2E081C`→`09A67E`; azar `2E0832`→`2E0848`; `2E0848`→`09A692`; si player_query_13 == 2562: no |
| 309811 | 1617 | camera wait 1632 | 10 | `2E08BC`→`04115A` |
| 309821 | 1632 | camera wait 1664 | 22 | `2E08D8`→`04115A` |
| 309843 | 1665 | camera wait 1696 | 20 | `2E08F4`→`04115A` |
| 309863 | 1699 | camera wait 1792 | 50 | cámara `0BDD86`→`0B0B94`\*; set region 4330; corredor `2E0916` (cada 90, 6 veces, hasta 1983); `2E093A`→`0AF17E` |
| 309913 | 1792 | camera wait 1808 | 14 | si group2 < 2: sí; `2E0928`→`0AF17E` (corredor); `2E0952`→`0569E6`; `2E0962`→`0569E6`; `2E0972`→`0569E6` |
| 309927 | 1810 | camera wait 1872 | 39 | `2E0988`→`0AF17E`; `2E099A`→`0AF17E`; `2E09AC`→`0AF17E` |
| 309966 | 1872 | camera wait 1904 | 22 | si group1 < 1: sí; `2E09CC`→`0569E6`; `2E09DC`→`0569E6`; `2E09EC`→`0569E6` |
| 309988 | 1906 | camera wait 1920 | 12 | `2E0A04`→`0569E6`; `2E0A14`→`0569E6`; `2E0A24`→`0569E6` |
| 310000 | 1922 | camera wait 1984 | 53 | si player_query_0E == 2306: no; corredor `2E0A72` (cada 90, 4 veces, hasta 2255) |
| 310053 | 1984 | camera wait 2112 | 70 | si group1 < 2: sí; azar `2E0A84`→`2E0ACC`; `2E0ACE`→`0569E6` (corredor); `2E0ADE`→`0569E6` (corredor); `2E0AEE`→`0569E6` (corredor); `2E0B08`→`036C34`; corredor `2E0B12` (cada 90, 6 veces, hasta 2255); `2E0B36`→`0AF17E` |
| 310123 | 2113 | camera wait 2256 + lock 2256 | 102 | si group2 < 2: sí; `2E0B24`→`0AF17E` (corredor) |
| 310225 | 2256 | camera wait 2256 + lock 2256 | 0 | `2E0B64`→`099DCC`; corredor `2E0B78` (cada 120, 6 veces, hasta 2256); corredor `2E0BA0` (cada 60, 8 veces, hasta 2256); `2E0C56`→`086728`\*; `2E0C6C`→`086728`\* |
| 310225 | 2256 | timer wait 60 | 120 | si player_query_13 == 2822: no; si group1 < 2: sí; si player_query_0C == 2562: sí; azar `2E0BB8`→`2E0BDE`; `2E0BE0`→`0569E6` (corredor); `2E0BF0`→`0569E6` (corredor); si player_query_0B == 2562: no; `2E0C86`→`086728`\*; `2E0C9C`→`086728`\* |
| 310345 | 2256 | condition wait group5 < 3 | 748 | si group1 < 2: no (×2); si player_query_13 == 2822: no (×3); si group1 < 2: sí (×4); si player_query_0C == 2562: sí (×2); azar `2E0BB8`→`2E0BBA`; `2E0BBC`→`0569E6` (corredor); `2E0BCC`→`0569E6` (corredor); si player_query_0B == 2562: no (×2); si player_query_0C == 2562: no (×2); si player_query_0B == 2562: sí (×2); azar `2E0C08`→`2E0C0A` (×2); `2E0C0C`→`0569E6` (corredor) (×2); `2E0C1C`→`0569E6` (corredor) (×2); azar `2E0BB8`→`2E0BDE`; `2E0BE0`→`0569E6` (corredor); `2E0BF0`→`0569E6` (corredor); `2E0CB8`→`086728`\*; `2E0CCE`→`086728`\* |
| 311093 | 2256 | timer wait 60 | 124 | si group1 < 2: sí; si player_query_0C == 2562: no; si player_query_0B == 2562: sí; azar `2E0C08`→`2E0C0A`; `2E0C0C`→`0569E6` (corredor); `2E0C1C`→`0569E6` (corredor); `2E0CE8`→`086728`\* |
| 311217 | 2256 | timer wait 60 | 120 | si player_query_13 == 2822: no; si group1 < 2: sí; si player_query_0C == 2562: sí; azar `2E0BB8`→`2E0BDE`; `2E0BE0`→`0569E6` (corredor); `2E0BF0`→`0569E6` (corredor); si player_query_0B == 2562: no |
| 311337 | 2256 | condition wait group5 < 3 | 518 | si group1 < 2: no (×2); si player_query_13 == 2822: no (×2); si group1 < 2: sí; si player_query_0C == 2562: no; si player_query_0B == 2562: sí; azar `2E0C08`→`2E0C0A`; `2E0C0C`→`0569E6` (corredor); `2E0C1C`→`0569E6` (corredor); si player_query_13 == 2306: no |
| 311855 | 2256 | condition wait group5 < 1 | 524 | si player_query_13 == 2822: no (×2); `2E0D4A`→`099DCC` |
| 312379 | 2256 | timer wait 60 | 120 | si player_query_13 == 2822: no; camera unlock |
| 312499 | 2256 | camera wait 2320 | 20 | `2E0D6A`→`086728`\* |
| 312519 | 2327 | camera wait 2384 | 12 | `2E0D86`→`086728`\*; `2E0D98`→`086728`\* |
| 312531 | 2386 | camera wait 2496 | 26 | `2E0DB2`→`086728`\*; `2E0DC8`→`086728`\* |
| 312557 | 2497 | camera wait 2672 + lock 2672 | 100 | — |
| 312657 | 2672 | camera wait 2672 + lock 2672 | 0 | `2E0DFA`→`0A2F2E` |
| 312657 | 2672 | condition wait group7 < 1 | 476 | corredor `2E0E14` (cada 60, 3 veces, hasta 2672); corredor `2E0E86` (cada 75, 3 veces, hasta 2672) |
| 313133 | 2672 | condition wait group7 < 1 | 0 | — |
| 313133 | 2672 | timer wait 90 | 180 | si group5 < 1: sí; si player_query_0E == 2562: no (×2); si player_query_0F == 2562: sí (×2); azar `2E0E5C`→`2E0E5E`; `2E0E5E`→`086712`\* (corredor); si group6 < 1: sí; azar `2E0ECA`→`2E0ECC`; `2E0ECC`→`086728`\* (corredor); si group5 < 1: no; `2E0F00`→`0A2EFA`\* |
| 313313 | 2672 | condition wait group7 < 1 | 126 | si group6 < 1: no; si group5 < 1: no |
| 313439 | 2672 | timer wait 90 | 180 | si group6 < 1: no; si group5 < 1: no; camera unlock; `2E0F20`→`01913C` |
| 313619 | 2672 | camera wait 2800 | 32 | si group6 < 1: no; `2E0F32`→`0569B4`; `2E0F40`→`0569B4` |
| 313651 | 2802 | camera wait 2848 | 10 | `2E0F54`→`086728`\* |
| 313661 | 2852 | camera wait 2864 | 4 | `2E0F6A`→`086728`\* |
| 313665 | 2872 | camera wait 2912 | 10 | `2E0F80`→`0569E6`; `2E0F90`→`0569E6`; `2E0FA0`→`0569E6`; `2E0FB0`→`04115A`; `2E0FC6`→`04115A` |
| 313675 | 2916 | camera wait 3072 | 93 | `2E0FE2`→`086712`\* |
| 313768 | 3073 | timer wait 10 | 20 | `2E0FF6`→`086712`\* |
| 313788 | 3102 | timer wait 10 | 20 | `2E100A`→`086712`\* |
| 313808 | 3132 | timer wait 10 | 20 | `2E101E`→`086712`\* |
| 313828 | 3168 | camera wait 3200 | 16 | `2E1034`→`086712`\* |
| 313844 | 3201 | timer wait 10 | 20 | `2E104E`→`086712`\* |
| 313864 | 3239 | timer wait 10 | 20 | `2E1068`→`086712`\* |
| 313884 | 3269 | timer wait 10 | 20 | `2E1082`→`086712`\* |
| 313904 | 3299 | camera wait 3280 | 0 | `2E109E`→`0569E6`; `2E10AE`→`0569E6`; `2E10BE`→`0569E6` |
| 313904 | 3299 | camera wait 3344 | 30 | si player_query_13 == 2306: no |
| 313934 | 3345 | camera wait 3360 | 8 | `2E1118`→`086712`\* |
| 313942 | 3360 | timer wait 10 | 20 | `2E112C`→`086712`\* |
| 313962 | 3400 | timer wait 10 | 20 | `2E1140`→`086712`\* |
| 313982 | 3438 | timer wait 10 | 20 | `2E1154`→`086712`\* |
| 314002 | 3469 | camera wait 3456 | 0 | `2E116A`→`086728`\*; `2E117E`→`086728`\* |
| 314002 | 3469 | camera wait 3488 | 14 | `2E1198`→`0AF17E`; `2E11AA`→`0AF17E`; `2E11BC`→`0AF17E` |
| 314016 | 3490 | camera wait 3536 | 35 | `2E11D4`→`086728`\* |
| 314051 | 3539 | camera wait 3568 | 27 | `2E11EE`→`0569E6`; `2E11FE`→`0569E6`; `2E120E`→`0569E6` |
| 314078 | 3571 | camera wait 3600 | 21 | `2E1224`→`086728`\*; `2E1238`→`0676A8`\* |
| 314099 | 3600 | camera wait 3640 | 30 | `2E124A`→`0676B6`\* |
| 314129 | 3642 | camera wait 3792 | 88 | `2E125E`→`086728`\* |
| 314217 | 3794 | camera wait 3872 + lock 3872 | 50 | — |
| 314267 | 3872 | camera wait 3872 + lock 3872 | 0 | `2E128E`→`086712`\*; `2E129E`→`086712`\*; `2E12AE`→`086712`\* |
| 314267 | 3872 | condition wait group5 < 2 | 602 | `2E12C4`→`0A2F2E` |
| 314869 | 3872 | condition wait group7 < 1 | 82 | — |
| 314951 | 3872 | condition wait group5 < 1 | 778 | `2E12E4`→`086728`\* |
| 315729 | 3872 | timer wait 10 | 20 | `2E12FE`→`086728`\* |
| 315749 | 3872 | timer wait 10 | 20 | `2E1318`→`086728`\* |
| 315769 | 3872 | timer wait 10 | 20 | `2E1332`→`086728`\* |
| 315789 | 3872 | condition wait group5 < 2 | 414 | `2E134E`→`086728`\* |
| 316203 | 3872 | timer wait 10 | 20 | `2E1368`→`086728`\* |
| 316223 | 3872 | timer wait 10 | 20 | `2E1382`→`086728`\* |
| 316243 | 3872 | timer wait 10 | 20 | `2E139C`→`086728`\*; si player_query_13 == 2306: no |
| 316263 | 3872 | condition wait group5 < 2 | 660 | `2E13E8`→`086728`\* |
| 316923 | 3872 | timer wait 10 | 20 | `2E1402`→`086728`\* |
| 316943 | 3872 | timer wait 10 | 20 | `2E141C`→`086728`\* |
| 316963 | 3872 | timer wait 10 | 20 | `2E1436`→`086728`\*; si player_query_13 == 2306: no |
| 316983 | 3872 | condition wait group5 < 2 | 444 | `2E1486`→`086712`\* |
| 317427 | 3872 | timer wait 10 | 20 | `2E14A0`→`086712`\* |
| 317447 | 3872 | timer wait 10 | 20 | `2E14BA`→`086712`\* |
| 317467 | 3872 | timer wait 10 | 20 | `2E14D4`→`086712`\* |
| 317487 | 3872 | condition wait group5 < 2 | 612 | `2E14F0`→`086728`\*; `2E1506`→`086728`\* |
| 318099 | 3872 | timer wait 10 | 20 | `2E1520`→`086728`\*; `2E1536`→`086728`\* |
| 318119 | 3872 | timer wait 10 | 20 | `2E1550`→`086728`\*; `2E1566`→`086728`\* |
| 318139 | 3872 | timer wait 10 | 20 | `2E1580`→`086728`\*; `2E1596`→`086728`\* |
| 318159 | 3872 | condition wait group5 < 2 | 682 | azar `2E15B2`→`2E1614`; `2E1616`→`086712`\*; `2E162C`→`086712`\* |
| 318841 | 3872 | condition wait group7 < 2 | 276 | azar `2E164C`→`2E164E`; `2E1650`→`086712`\*; `2E1666`→`086712`\*; si player_query_13 == 2306: no |
| 319117 | 3872 | condition wait group7 < 2 | 440 | azar `2E1716`→`2E1718`; `2E171A`→`086712`\*; `2E1730`→`086712`\* |
| 319557 | 3872 | condition wait group6 < 1 | 0 | — |
| 319557 | 3872 | condition wait group7 < 1 | 486 | `2E17B6`→`08FF7E`\*; `2E17C6`→`08FF7E`\* |
| 320043 | 3872 | timer wait 90 | 180 | `2E17DA`→`086712`\*; `2E17F0`→`086712`\* |
| 320223 | 3872 | condition wait group7 < 2 | 466 | `2E180C`→`086712`\*; `2E1822`→`086712`\* |
| 320689 | 3872 | timer wait 90 | 180 | si player_query_13 == 1814: no; `2E1856`→`086712`\* |
| 320869 | 3872 | condition wait group7 < 1 | 552 | — |
| 321421 | 3872 | timer wait 60 | 120 | `2E1876`→`08C37C`; `2E1886`→`08C37C`; camera unlock; set region 4330 |
| 321541 | 3872 | camera wait 4096 | 54 | cámara `0BDD9C`→`0BE8A2`\*; `2E18A4`→`0AF1A8`\*; `2E18BA`→`08AFA8`\* |
| 321595 | 4101 | camera wait 4128 | 12 | si player_query_0F == 2306: sí; `2E18DC`→`08C37C`; `2E18EC`→`08C37C`; `2E18FC`→`08C37C`; `2E190E`→`0AF1A8`\*; `2E1924`→`08AFDE` |
| 321607 | 4131 | camera wait 4144 | 10 | si player_query_0F == 2306: no; `2E1978`→`0AF1A8`\*; `2E198E`→`08AFDE` |
| 321617 | 4146 | camera wait 4192 | 38 | `2E19AA`→`0AF1A8`\* |
| 321655 | 4192 | camera wait 4240 + lock 4240 | 37 | — |
| 321692 | 4240 | camera wait 4240 | 0 | — |
| 321692 | 4240 | condition wait group1 < 1 | 458 | `2E19DA`→`0BE8F6`\* |
| 322150 | 4240 | end | 192 | misión→`03AED8`\*; cámara `0BEA4A`→`0BEADC`\* |

#### Sección 3 (final)

| Fotograma | Cámara | Orden que espera | Fotogramas | Después |
| --- | --- | --- | --- | --- |
| 322342 | 0 | camera wait 1 | 160 | misión→`03AEF0`\*; misión→`05287C`; `2E19EC`→`01913C`; `2E19F8`→`04153E`\* |
| 322502 | 1 | end | — | — |
