# Misión 4: inventario de actores (fase 1)

Fuente: `data/mission4_actors.json` (MAME 0.264, `tools/capture_m4_actors.sh` desde `states4/sec05_149829`,
réplica fotograma a fotograma de `run4`, 149829..160500: el recorrido entero y los primeros 2900 fotogramas del jefe;
`tools/compare_m3_events.py --sections`). Guion: `data/mission4_script.json` (sección `2DB696..2DCB3A`, 300 sentencias,
216 registros de aparición, 34 constructores). Estado del port: `build/host/script-native` (`neo_world_spawn_native`).

Regla: un actor sin portar es una parada explícita contada (`world unsupported routine=...`, registro saltado con su
dirección en el log); nunca se oculta.

## Resumen

| | Constructores | Creaciones |
| --- | --- | --- |
| nativos (compartidos con M1–M3) | 87 | 5285 |
| nativos en otro módulo (fin de misión, aterrizaje, guion, efectos) | 7 | 49 |
| **nuevos, sin portar** | 56 | 3471 |
| externos (contados, no creados) | 1 | 1 |
| total | 151 | 8806 |

Secciones (orden de la lista de cámara `0E741A` que las abre; fotograma de `run4`): 1 calle desde 149831, 2 subida desde 152145, 3 ruinas desde 153826, 4 colina desde 155803, 5 jefe desde 157581.

Registros del guion que `run4` no ejecutó (ramas `si`/azar, a capturar en la fase 2): 54.

## Nuevos (sin portar): 56 constructores

Por familia (raíz = registro del guion, llamada de la lista de cámara o actor de misión del que descienden):

- `033526` (4): `033526` `03378A` `099DD6` `033864`; primera cámara 0, 7 creaciones
- `0B9F4A` (1): `0B9F4A`; primera cámara 272, 32 creaciones
- `036C34` (2): `036C34` `036EA8`; primera cámara 464, 7 creaciones
- `0B9B24` (1): `0B9B24`; primera cámara 928, 1 creaciones
- `08391C` (1): `08391C`; primera cámara 938, 1 creaciones
- `common` (11): `0BB106` `08CF6E` `01F9B8` `01FA08` `01FABA` `01FF82` `020004` `08C954` `0859E4` `085ACE` `0259F2`; primera cámara 1000, 345 creaciones
- `0A9F0C` (3): `0A9F0C` `0AA37C` `0AA2C6`; primera cámara 1136, 3 creaciones
- `0B9AD6` (1): `0B9AD6`; primera cámara 1344, 1 creaciones
- `01F828` (1): `01F828`; primera cámara 1456, 2 creaciones
- `0B9C7A` (1): `0B9C7A`; primera cámara 1488, 1 creaciones
- `0B9D88` (1): `0B9D88`; primera cámara 1488, 22 creaciones
- `01F866` (1): `01F866`; primera cámara 1648, 2 creaciones
- `07E554` (3): `07E554` `07EDF0` `07EC6A`; primera cámara 2333, 18 creaciones
- `0B99EE` (1): `0B99EE`; primera cámara 2496, 1 creaciones
- `09A71E` (1): `09A71E`; primera cámara 2632, 3 creaciones
- `0AF1A8` (1): `0AF1A8`; primera cámara 2632, 6 creaciones
- `099E06` (2): `0DE0FE` `0DE24A`; primera cámara 2632, 32 creaciones
- `08571C` (2): `08571C` `085952`; primera cámara 3194, 10 creaciones
- `085522` (1): `085522`; primera cámara 3256, 10 creaciones
- `08514A` (2): `08514A` `085294`; primera cámara 4571, 18 creaciones
- `0587C4` (3): `0587C4` `0592C6` `059B5C`; primera cámara 4713, 3 creaciones
- `02916E` (1): `02A448`; primera cámara 5091, 2 creaciones
- `02267E` (11): `02267E` `022CB6` `023526` `02391E` `024226` `0246D0` `023EB4` `023FDE` `025DC2` `024F0E` `02505C`; primera cámara 5552, 2944 creaciones

| Constructor | Creaciones | Primer fotograma | Cámara | Por sección | Creadores | Programas | Sonidos | Familia (raíz) |
| --- | --- | --- | --- | --- | --- | --- | --- | --- |
| `033526` | 2 | 149833 | 0 | 1:2 | `script`×2 | 2 | 1025 | 033526 |
| `03378A` | 2 | 149992 | 26 | 1:2 | `033526`×2 | 1 |  | 033526 |
| `099DD6` | 1 | 150017 | 66 | 1:1 | `033526`×1 | 6 | 1048 113A | 033526 |
| `033864` | 2 | 150049 | 129 | 1:2 | `033526`×2 | 2 |  | 033526 |
| `0B9F4A` | 32 | 150149 | 272 | 1:6 3:19 4:7 | `mission`×32 | 0 | 1030 | 0B9F4A |
| `036C34` | 5 | 150322 | 464 | 1:1 2:3 4:1 | `script`×5 | 5 | 1036 109F | 036C34 |
| `036EA8` | 2 | 150436 | 592 | 1:1 4:1 | `036C34`×2 | 1 |  | 036C34 |
| `0B9B24` | 1 | 150732 | 928 | 1:1 | `mission`×1 | 1 |  | 0B9B24 |
| `08391C` | 1 | 150744 | 938 | 1:1 | `script`×1 | 1 | 1021 105E | 08391C |
| `0BB106` | 122 | 151222 | 1000 | 1:7 2:29 3:30 4:36 5:20 | `0B9F4A`×93, `0B9D88`×29 | 6 |  | common |
| `08CF6E` | 48 | 151236 | 1000 | 1:3 3:45 | `08C374`×48 | 1 |  | common |
| `0A9F0C` | 1 | 151715 | 1136 | 1:1 | `script`×1 | 6 | 102C 102F 1075 | 0A9F0C |
| `0AA37C` | 1 | 151715 | 1136 | 1:1 | `0A9F0C`×1 | 6 |  | 0A9F0C |
| `0AA2C6` | 1 | 152049 | 1256 | 1:1 | `0A9F0C`×1 | 0 |  | 0A9F0C |
| `0B9AD6` | 1 | 152087 | 1344 | 1:1 | `mission`×1 | 1 |  | 0B9AD6 |
| `01F828` | 2 | 152208 | 1456 | 2:2 | `script`×2 | 0 |  | 01F828 |
| `01F9B8` | 4 | 152208 | 1456 | 2:4 | `01F828`×2, `01F866`×2 | 4 |  | common |
| `01FA08` | 4 | 152208 | 1456 | 2:4 | `01F828`×2, `01F866`×2 | 4 |  | common |
| `01FABA` | 15 | 152211 | 1456 | 2:15 | `01F866`×13, `01F828`×2 | 1 | 1053 | common |
| `01FF82` | 15 | 152211 | 1456 | 2:15 | `01FABA`×15 | 1 |  | common |
| `020004` | 7 | 152250 | 1456 | 2:7 | `01F9B8`×7 | 1 |  | common |
| `0B9C7A` | 1 | 152268 | 1488 | 2:1 | `mission`×1 | 0 |  | 0B9C7A |
| `0B9D88` | 22 | 152268 | 1488 | 2:22 | `mission`×22 | 6 | 102E | 0B9D88 |
| `01F866` | 2 | 152367 | 1648 | 2:2 | `script`×2 | 0 |  | 01F866 |
| `08C954` | 5 | 153079 | 2318 | 2:5 | `08C752`×5 | 5 |  | common |
| `07E554` | 4 | 153085 | 2333 | 2:4 | `01A422`×4 | 4 | 103E | 07E554 |
| `07EDF0` | 10 | 153112 | 2364 | 2:10 | `07E554`×8, `07EC6A`×2 | 1 |  | 07E554 |
| `07EC6A` | 4 | 153115 | 2365 | 2:4 | `07E554`×4 | 1 |  | 07E554 |
| `0B99EE` | 1 | 153238 | 2496 | 2:1 | `mission`×1 | 1 |  | 0B99EE |
| `09A71E` | 3 | 153376 | 2632 | 2:3 | `script`×3 | 0 |  | 09A71E |
| `0AF1A8` | 6 | 153471 | 2632 | 2:4 3:2 | `script`×6 | 19 | 103E 1041 1042 1043 | 0AF1A8 |
| `0DE0FE` | 23 | 153704 | 2632 | 2:2 3:4 4:1 5:16 | `099E06`×23 | 2 |  | 099E06 |
| `08571C` | 6 | 154171 | 3194 | 3:6 | `script`×6 | 1 |  | 08571C |
| `085952` | 4 | 154222 | 3254 | 3:4 | `08571C`×4 | 1 |  | 08571C |
| `085522` | 10 | 154344 | 3256 | 3:6 4:4 | `script`×7, `01A422`×3 | 2 | 102C | 085522 |
| `0859E4` | 7 | 154535 | 3256 | 3:4 4:3 | `085522`×6, `085294`×1 | 1 | 10D1 | common |
| `085ACE` | 6 | 154555 | 3256 | 3:3 4:3 | `0859E4`×6 | 3 |  | common |
| `0DE24A` | 9 | 156339 | 4360 | 4:3 5:6 | `099E06`×9 | 1 |  | 099E06 |
| `08514A` | 9 | 156772 | 4571 | 4:9 | `script`×9 | 0 |  | 08514A |
| `085294` | 9 | 156772 | 4571 | 4:9 | `08514A`×9 | 2 | 102C | 08514A |
| `0587C4` | 1 | 156874 | 4713 | 4:1 | `script`×1 | 5 | 1021 102F 105E 1060 | 0587C4 |
| `0592C6` | 1 | 156874 | 4713 | 4:1 | `0587C4`×1 | 4 |  | 0587C4 |
| `059B5C` | 1 | 157018 | 4904 | 4:1 | `0587C4`×1 | 1 |  | 0587C4 |
| `02A448` | 2 | 157239 | 5091 | 4:2 | `02916E`×2 | 2 |  | 02916E |
| `02267E` | 1 | 157581 | 5552 | 5:1 | `script`×1 | 0 |  | 02267E |
| `022CB6` | 1 | 157582 | 5552 | 5:1 | `02267E`×1 | 1 | 1071 | 02267E |
| `023526` | 3 | 157582 | 5552 | 5:3 | `02267E`×3 | 12 | 1066 | 02267E |
| `02391E` | 4 | 157582 | 5552 | 5:4 | `02267E`×4 | 15 | 1066 | 02267E |
| `024226` | 1 | 157582 | 5552 | 5:1 | `02267E`×1 | 1 |  | 02267E |
| `0246D0` | 1 | 157582 | 5552 | 5:1 | `02267E`×1 | 1 |  | 02267E |
| `0259F2` | 112 | 157592 | 5559 | 5:112 | `01D764`×112 | 1 |  | common |
| `023EB4` | 30 | 157832 | 5620 | 5:30 | `023526`×16, `02391E`×14 | 11 |  | 02267E |
| `023FDE` | 29 | 157866 | 5629 | 5:29 | `023526`×15, `02391E`×14 | 4 |  | 02267E |
| `025DC2` | 29 | 157884 | 5633 | 5:29 | `023526`×15, `02391E`×14 | 3 |  | 02267E |
| `024F0E` | 43 | 157888 | 5634 | 5:43 | `02391E`×28, `023526`×15 | 3 | 1022 | 02267E |
| `02505C` | 2802 | 157888 | 5634 | 5:2802 | `024F0E`×2802 | 1 |  | 02267E |

## Nativos ya portados (compartidos): 87 constructores

Los que crea directamente el guion de la M4 o su lista de cámara:

| Constructor | Creaciones | Primer fotograma | Cámara | Por sección | Creadores | Programas | Sonidos | Familia (raíz) |
| --- | --- | --- | --- | --- | --- | --- | --- | --- |
| `0AF17E` | 27 | 149833 | 0 | 1:17 2:1 3:9 | `01A422`×14, `script`×13 | 26 | 103E 1041 1042 1043 | 0AF17E |
| `099DF0` | 5 | 150072 | 159 | 1:4 2:1 | `script`×5 | 7 | 1048 113A | 099DF0 |
| `083C8A` | 9 | 150141 | 258 | 1:9 | `script`×7, `01A422`×2 | 1 | 1021 105D 108D | 083C8A |
| `099DCC` | 6 | 150163 | 298 | 1:3 3:2 5:1 | `script`×5, `01A422`×1 | 11 | 1048 1056 113A | 099DCC |
| `0403A0` | 7 | 150165 | 302 | 1:3 2:1 3:1 4:2 | `script`×7 | 0 |  | 0403A0 |
| `04115A` | 14 | 150173 | 320 | 1:5 2:6 3:1 4:2 | `script`×14 | 9 | 103E 1040 1041 1042 | 04115A |
| `08DA12` | 10 | 150389 | 530 | 1:1 3:3 4:6 | `script`×10 | 10 | 103E 1040 1041 1042 | 08DA12 |
| `027F54` | 1 | 150789 | 1000 | 1:1 | `script`×1 | 8 | 1021 1033 109D | 027F54 |
| `01913C` | 4 | 151836 | 1256 | 1:2 2:1 5:1 | `script`×4 | 0 |  | 01913C |
| `09AE1E` | 2 | 152382 | 1668 | 2:2 | `01A422`×2 | 5 | 103E 1041 1042 | 09AE1E |
| `08A732` | 6 | 152433 | 1728 | 2:2 3:3 4:1 | `script`×6 | 5 | 103E 1041 1042 1043 | 08A732 |
| `08DA34` | 6 | 152517 | 1826 | 2:1 4:5 | `script`×6 | 6 | 103E 1040 1042 1043 | 08DA34 |
| `09AE2C` | 2 | 152560 | 1865 | 2:2 | `script`×2 | 4 | 103E 1041 1042 | 09AE2C |
| `05D234` | 1 | 152587 | 1898 | 2:1 | `script`×1 | 3 | 1043 | 05D234 |
| `087E36` | 1 | 152830 | 2160 | 2:1 | `script`×1 | 5 | 1021 1072 | 087E36 |
| `099E06` | 4 | 152830 | 2160 | 2:2 3:1 4:1 | `script`×4 | 9 | 1048 1056 113A 113B | 099E06 |
| `09A67E` | 6 | 153067 | 2280 | 2:6 | `script`×6 | 0 |  | 09A67E |
| `0569E6` | 4 | 153307 | 2569 | 2:2 4:2 | `script`×4 | 7 | 1041 1042 | 0569E6 |
| `092A32` | 1 | 155879 | 4288 | 4:1 | `script`×1 | 5 | 1034 1092 10E9 | 092A32 |
| `02916E` | 1 | 157048 | 4929 | 4:1 | `script`×1 | 4 | 1023 102F 1033 10A5 | 02916E |

El resto (hijos, disparos, efectos, premios): `03C62A` `05287C` `01B782` `01BF46` `01BC2E` `08C374` `01D31C` `0897E2` `09A9C4` `040F88` `08D330` `06381C` `0C58D2` `063D9E` `084098` `08443E` `01C71E` `0843F8` `08497E` `06371E` `01D764` `083B30` `083B92` `01D86C` `01D944` `01C706` `028C76` `01C6EE` `01D958` `01BEDA` `063AEE` `0C4E86` `0287FA` `028BBC` `08C68E` `09C46C` `08AA6E` `05DF3E` `05DFC2` `05DD2A` `040396` `08982A` `09A97C` `08943C` `01C362` `03C962` `01C654` `056E5A` `056A6A` `056C94` `01E058` `014A4E` `03B4E0` `03523C` `056B00` `0954DC` `01B6A0` `094EAA` `0952F8` `03B936` `0AD9B8` `0AE1DC` `0A719C` `0A74E4` `0A751C` `029802` `059F76`.

En otro módulo: `01A422` (neo_script.c), `03BC28` (neo_soldier_effects.c), `0607DE` (neo_end.c), `060A72` (neo_end.c), `060B06` (neo_end.c), `0C4A58` (neo_landing.c), `0C5286` (neo_landing.c). Externo: `08C752`.

## Constructores del guion por estado

| Constructor del guion | Estado | Creaciones en `run4` |
| --- | --- | --- |
| `01913C` | native | 4 |
| `01F828` | missing | 2 |
| `01F866` | missing | 2 |
| `02267E` | missing | 1 |
| `027F54` | native | 1 |
| `02916E` | native | 1 |
| `033526` | missing | 2 |
| `036C34` | missing | 5 |
| `0403A0` | native | 7 |
| `04115A` | native | 14 |
| `0569E6` | native | 4 |
| `0587C4` | missing | 1 |
| `05D234` | native | 1 |
| `07E554` | missing | 4 |
| `08391C` | missing | 1 |
| `083C8A` | native | 9 |
| `08514A` | missing | 9 |
| `085522` | missing | 10 |
| `08571C` | missing | 6 |
| `087E36` | native | 1 |
| `08A732` | native | 6 |
| `08DA12` | native | 10 |
| `08DA34` | native | 6 |
| `092A32` | native | 1 |
| `099DCC` | native | 6 |
| `099DF0` | native | 5 |
| `099E06` | native | 4 |
| `09A67E` | native | 6 |
| `09A71E` | missing | 3 |
| `09AE1E` | native | 2 |
| `09AE2C` | native | 2 |
| `0A9F0C` | missing | 1 |
| `0AF17E` | native | 27 |
| `0AF1A8` | missing | 6 |

## Vehículos y jefe

- **SV-001 `092A32`**: nace en 155879 (cámara 4288, sección 4), nativo (compartido con la M1). Estado MAME: `captures-m4/states/mslugx/at_155850`.
- **Jefe**: la orden `0BB028` de la lista de cámara (progreso 0x15B0 = 5552, fotograma 157582) crea el acorazado; su cuerpo es el mapa `02225E` cargado por un actor en la capa `10D7B6` (byte de capa 6957 = 3: la capa se mueve con las velocidades propias 694E/6952); la velocidad del actor de misión pasa a 0x8000 y la cámara avanza sola hasta 27891 (límite de la lista 0x7FF0). En `run4` la pelea dura 157582..246905. Familia `02267E` (11 constructores nuevos vistos hasta 160500). Fase 2.
