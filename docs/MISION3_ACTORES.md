# Misión 3: eventos, personajes y actores (inventario)

Encargo (rama `work-m3inv`, solo investigación y documentación): todo lo
que la Misión 3 crea, desde la salida hasta el jefe y el fin de misión, con
la cadena de eventos del guion, el inventario de constructores, los premios
y un plan por prioridades. Modelo: [MISION2_ACTORES.md](MISION2_ACTORES.md)
y la cadena de eventos de [MISION2.md](MISION2.md). El mapa y la colisión
están en [MISION3.md](MISION3.md) («Reconocimiento del mapa»). Regla de
siempre: solo lo que dicen MAME 0.264 y la ROM; lo no comprobado se marca.

## Captura

`tools/capture_m3_actors.sh OUT` (MAME 0.264, `mslugx` BIOS `euro`,
`tools/capture_fullgame.lua` desde `states4/sec04_117966` hasta el estado de
la Misión 4, 149830; unos 9 min) pone solo puntos de parada del depurador
(`MSX_FULL_NO_PROBES=1`, sin volcados ni toma de rejilla):

| Línea | Dirección | Qué registra |
| --- | --- | --- |
| `ALLOC` | `00410E` (dentro de `0040BC`) | nodo, rutina (= constructor), creador; por `00411C` (retorno `004122`, inserta tras otro nodo, p. ej. tras el guion `101414`) el creador real es el `A6` guardado en la pila |
| `FREE` | `004126` | nodo liberado |
| `SPAWN` | `01A132` | registro op 7 del guion, nodo, creador |
| `OP` / `WAIT` | `01A0A6` / `019F7E` | orden no bloqueante (todas) / orden bloqueante del guion principal `101414` (al cambiar) |
| `ANIM` | `004256` | programa de sprites que arranca un nodo (`A1`) |
| `SND` | `0120F8` | palabra de sonido pedida (`D0`) y nodo |
| `ITEM` | `08C380` | tipo `+6E` de cada objeto en su constructor (`08C374`/`08C37C`), `+8A` |
| `CAM` | escritura de `10E964` | puntero de la lista de cámara `0B8968` (el punto de vigilancia imprime el valor que se deja: la orden que acaba de ejecutarse) |
| `END` | `018F92` / `018870` | fin de misión |

Fotogramas en la convención de `temp9` (último fotograma completo; uno menos
que los de `MISION3.md`). **Reproducción**: dos ejecuciones dan el mismo
registro línea a línea y coinciden con `run4` (secciones en 118732, 124728,
126288, 144371; `018870` en 149171). Una tercera, que además leía la palabra
apuntada por un valor de la pila en `ALLOC`, **cambió la partida desde
el fotograma 138446**: leer memoria arbitraria desde el depurador tiene
efectos (registros de E/S); el guion solo lee RAM y pila.

```sh
sh tools/capture_m3_actors.sh /home/user/captures-m3inv/full
# build/host/script-native: lo construye make check
python3 tools/compare_m3_events.py OUT/debug.log --json data/mission3_actors.json --markdown
python3 tools/compare_m3_events.py OUT/debug.log --chain --list   # cadena completa y una línea por constructor
python3 tools/compare_m3_events.py OUT/debug.log --port ares.log  # registros arcade contra port (como compare_m2_events.py)
```

`tools/compare_m3_events.py` saca de la captura la cadena de eventos, el
inventario (veces, primer fotograma y cámara, secciones, creadores,
programas, sonidos, familia raíz) y el estado en el port
(`neo_world_spawn_native` del ayudante `build/host/script-native`, más una
lista corta de constructores que lleva otro módulo). Resumen en
`data/mission3_actors.json` (un constructor por línea; tipos de objeto por
fuente; los 72 registros del guion que no se ejecutaron). La captura se
borró tras extraer.

## Resumen

- **233 constructores** distintos, **10 871 creaciones**, 184 registros del
  guion ejecutados (200 con los corredores), 43 de los 44 constructores del
  guion (falta `05D234`: solo en un corredor que no se ejecutó).
- **Port**: 63 nativos en el mundo (`neo_world_spawn_native`) + 13 en otro
  módulo (`neo_train.c`, `neo_end.c`, `neo_tally.c`, `neo_landing.c`,
  `neo_script.c`, la muerte y el efecto diferido del soldado) = **76
  nativos**; 4 solo en la rama sin fusionar `work-m3c` (cajas `039382/03938A`
  y estampadores `0B911E/0B9140` de la repisa `0B8FA8`); 1 externo contado
  (`08C752`); **152 faltan** (2 621 de las 10 871 creaciones).
- Lo que falta, por familias: jefe `04A2C2` (21), efectos comunes (19, sobre
  todo `01C8D2` ×235 de la explosión `01D764`), aeronave «96» `0A4A94` (15),
  `097FCC` (12), `04E990` (10), vehículo `0AC866` (10), armas del jugador con
  la forma `+93 = 2` y el arma E (9), `084588` (6), `01F1D4` (5), `08A732`
  (5), `0A2F2E` (5), `05D916` (4), plataforma `0B9286` (3), el resto 1–3.
- **Reutilizado** de las Misiones 1 y 2 y ya nativo: soldado `0AF17E` y
  `04115A`, prisioneros `099DCC/099862/099E1C`, cajas ocultas
  `09A67E/09A692`, caja de suministros `08C172` (M2), objetos y etiquetas,
  marcadores `0403A0`, camión `02916E`, bombarderos `0DC97E`, lanzacohetes
  `05ED84`, blindado `087E36`, piezas de tanque `0AD9B8`, torreta `03B4E0` y
  oruga `014A4E` del SV-001, balas, granadas, cohete R, explosiones.

## 1. Cadena de eventos por sección

Secciones de la lista de cámara `0B8968` (empieza cuando el puntero deja la
orden indicada) y lo que la lista hace en cada una (órdenes no triviales;
`call` = rutina de 68000; «→» lo que crea, con el creador `0B8xxx/0B9xxx`):

| Sección | Fotogramas | Cámara | Órdenes de la lista de cámara (fotograma: orden) |
| --- | --- | --- | --- |
| 1 salida | 117967..118731 | 0..320 | 117968: mapas `0B853A` (`10C72E`), `0B8566` (tren `10D7B6`), `0B857C` (cielo `10E83E`); 117969: `call 0B8CE8`, `call 0B8EAC`, `call 0B8D34` → `0B8EDC` y `0B8D64` (conductor del tren), `call 0BC548`; el actor de misión crea (118022) `0607DE` (MISSION 3 START, voz 1133) y la flecha GO `05287C` |
| 2 vagones | 118732..124727 | 320..3392 | 118732 (progreso 0x140): banderas de capa (`10E957/10E958`), cielo a mapa 0, `call 0B8CFC` (modo 1), sonido 4, `call 0B8E7E`; 120945: `call 0B918C` → caja `039382`; 121005: sonido 3, `call 0B8D06` (modo 2); 121188: `call 0B91AE` → caja `03938A`; 121466: `call 0B9148` → `038D1E`; 121504: sonido 2; 121700: `call 0B916A` → `038D26`; 122145: modo automático 0x200, `call 0B8EBC(−8)`; 123924: `call 0B8EBC(8)`; 124034: `call 0B91D0` → `0B91FA` |
| 3 hueco | 124728..126287 | 3392..3712 | 124728: `call 0B8E9C`, primer plano a `0B8600` (sin mosaicos ni colisión); 126224: espera del jugador en el rectángulo (`0C84BE`: x 0xE28, y 0x50, 0xD0 × 0x40); 126244: `call 0B925C` → plataforma `0B9286`; 126262: `call 0B8EBC(−4)` |
| 4 tramo final | 126288..144370 | 3712..6304 | 126288: primer plano a `0B8550`; 132302: límites, `call 0B8EBC(8)`; 141008: `call 0B8EBC(−8)`; 141260: modo automático 0; 142206: punto del jugador |
| 5 jefe | 144371..149828 | 6304..6496 | 144439: sonido 6, `call 0B8EA4`, `call 0B8D28`; 149828: `hold` |

Hitos: **jefe intermedio** `0A4A94` (registro `2DAE20`, cámara 3944,
132378; el guion espera `group0 == 0` 8615 fotogramas); **jefe** `04A2C2`
(registro `2DB5E4`, 144149, cámara 5980 → 6496; música `059F76` en 144483,
palabra 0028); **fin**: el nodo del jefe (`1018E4`, rutina `04A9D4`) llama a
`018F92` en 148579; `018870` en 149171; recuento `0308C0` (149229) y
MISSION COMPLETE `06090A` (149428). En la sección 5 el guion solo espera
tiempo (`timer_wait 900` ×3) y crea los prisioneros `099862`/`099E1C`: **el
fin de la misión depende del jefe**, no del guion.

Tablas generadas por `compare_m3_events.py --markdown`: una fila por cada
orden bloqueante del guion principal (fotograma y cámara al empezar a
esperar, fotogramas que espera) y lo que se ejecuta hasta la siguiente:
registros op 7 (`registro`→`constructor`; `\*` = no lo crea
`neo_world_spawn_native`, aunque otro módulo lo lleve), elecciones al
azar (`azar` op 10 → alternativa tomada), `si` (op 11, tomado o no), los
registros de los corredores (op 8), lo que crean la lista de cámara y el
actor de misión, y el fin. `player_query_NN == V` son las condiciones de
posición del jugador (`01903A`, tipos 0B..15 de `01A5CA`).

#### Sección 1 (salida)

| Fotograma | Cámara | Orden que espera | Fotogramas | Después |
| --- | --- | --- | --- | --- |
| 117969 | 0 | (inicio) | — | cámara `0B8A40`→`0B8EDC`\*; cámara `0B8A40`→`0B8D64`\* |
| 117969 | 0 | camera wait 0 | 0 | azar `2D9CE0`→`2D9CEE`; `2D9CEE`→`08C172`; `2D9CFC`→`08C172`; `2D9D08`→`08C37C`; azar `2D9D18`→`2D9D26`; `2D9D26`→`08C172`; azar `2D9D34`→`2D9D42`; `2D9D42`→`08C172`; `2D9D50`→`0AF17E`; `2D9D64`→`08C37C`; azar `2D9D74`→`2D9D86`; `2D9D86`→`08C37C`; `2D9DA8`→`099DCC`; `2D9DBC`→`08C37C`; `2D9DCC`→`08C172`; `2D9DD8`→`01F1D4`\*; `2D9DE2`→`09A692` |
| 117970 | 0 | camera wait 32 | 181 | misión→`0607DE`\*; misión→`05287C`; `2D9DFA`→`0AF17E`; azar `2D9E0E`→`2D9E20`; `2D9E20`→`0214EA`\* |
| 118151 | 35 | camera wait 96 | 49 | `2D9E38`→`0403A0` |
| 118200 | 97 | camera wait 112 | 10 | `2D9E4A`→`03EE8A`\*; `2D9E58`→`03EE8A`\*; `2D9E66`→`03EE8A`\* |
| 118210 | 112 | camera wait 160 | 26 | `2D9E7A`→`0403A0` |
| 118236 | 161 | camera wait 168 | 4 | `2D9E8C`→`03EE8A`\*; `2D9E9A`→`03EE8A`\*; `2D9EA8`→`03EE8A`\* |
| 118240 | 169 | camera wait 184 | 8 | `2D9EBC`→`03EE8A`\*; `2D9ECA`→`03EE8A`\* |
| 118248 | 185 | camera wait 272 | 54 | `2D9EDE`→`0AF17E` |
| 118302 | 273 | camera wait 288 | 10 | `2D9EFA`→`0AF17E` |
| 118312 | 288 | camera wait 312 + lock 312 | 16 | — |
| 118328 | 312 | camera wait 312 + lock 312 | 0 | set region 4330 |
| 118328 | 312 | condition wait groups0..7 < 1 | 324 | `2D9F38`→`09AE2C`\* |
| 118652 | 312 | condition wait groups0..7 == 0 | 74 | camera unlock |
| 118726 | 312 | camera wait 312 | 0 | autoscroll start |
| 118726 | 312 | camera wait 320 | 6 | autoscroll stop |

#### Sección 2 (vagones)

| Fotograma | Cámara | Orden que espera | Fotogramas | Después |
| --- | --- | --- | --- | --- |
| 118732 | 320 | camera wait 320 + lock 320 | 0 | — |
| 118732 | 320 | camera wait 320 + lock 320 | 0 | camera unlock |
| 118732 | 320 | camera wait 344 | 10 | `2D9F86`→`099DCC` |
| 118742 | 351 | camera wait 360 | 2 | `2D9FA0`→`09BA2A`\* |
| 118744 | 360 | camera wait 408 | 12 | `2D9FB8`→`09BA2A`\*; si player_query_12 == 1812: sí; `2D9FD0`→`099DCC`; si player_query_13 == 1812: no |
| 118756 | 416 | camera wait 480 | 14 | `2DA004`→`0403A0` |
| 118770 | 486 | camera wait 544 | 18 | `2DA016`→`099DCC` |
| 118788 | 545 | camera wait 624 | 48 | `2DA030`→`08DA12`\* |
| 118836 | 627 | camera wait 640 | 6 | azar `2DA048`→`2DA08C` |
| 118842 | 640 | camera wait 656 | 8 | `2DA098`→`0AF17E`; `2DA0AE`→`0AF17E` |
| 118850 | 656 | camera wait 704 | 42 | azar `2DA0CA`→`2DA0F8`; `2DA0F8`→`09A67E` |
| 118892 | 704 | camera wait 752 | 49 | `2DA11A`→`08DA12`\* |
| 118941 | 752 | camera wait 824 | 73 | azar `2DA132`→`2DA162` |
| 119014 | 825 | camera wait 848 | 36 | si group2 == 0: sí; `2DA174`→`0AF1B6`\*; `2DA18A`→`02B202`\* |
| 119050 | 849 | camera wait 848 | 0 | si group1 < 1: no |
| 119050 | 849 | camera wait 880 | 40 | `2DA1E0`→`0403A0` |
| 119090 | 882 | camera wait 888 | 8 | azar `2DA1F2`→`2DA1F4`; `2DA1F4`→`09A67E` |
| 119098 | 890 | camera wait 936 | 47 | `2DA22C`→`0403A0` |
| 119145 | 938 | camera wait 944 | 7 | `2DA23E`→`08DA12`\* |
| 119152 | 946 | camera wait 992 | 102 | si group2 == 0: no; corredor `2DA288` (cada 120, 4 veces, hasta 1184) |
| 119254 | 993 | camera wait 1024 | 66 | si group3 < 1: sí; `2DA29A`→`0AF17E` (corredor); azar `2DA2B6`→`2DA2E6` |
| 119320 | 1024 | camera wait 1040 | 15 | si group1 < 1: sí; `2DA2F8`→`0AF17E`; `2DA30E`→`0AF17E` |
| 119335 | 1043 | camera wait 1040 | 0 | `2DA32C`→`04115A`; `2DA342`→`04115A`; `2DA358`→`04115A` |
| 119335 | 1043 | camera wait 1072 | 47 | `2DA374`→`0403A0` |
| 119382 | 1073 | camera wait 1088 | 26 | azar `2DA386`→`2DA388`; `2DA388`→`09A67E` |
| 119408 | 1089 | camera wait 1088 | 0 | si group2 == 0: sí; `2DA3C8`→`0AF17E`; `2DA3DE`→`02B202`\* |
| 119408 | 1089 | camera wait 1128 | 44 | `2DA3F8`→`0403A0` |
| 119452 | 1129 | camera wait 1184 | 42 | si group2 == 0: no; corredor `2DA43C` (cada 90, 5 veces, hasta 1543) |
| 119494 | 1185 | camera wait 1248 | 69 | si group3 < 1: sí; `2DA44E`→`0AF17E` (corredor); si group1 < 1: sí; `2DA472`→`0AF17E`; `2DA488`→`0AF17E` |
| 119563 | 1251 | camera wait 1264 | 16 | `2DA4A6`→`027F54`\* |
| 119579 | 1267 | camera wait 1264 | 0 | `2DA4C2`→`0403A0` |
| 119579 | 1267 | camera wait 1265 | 0 | corredor `2DA4D4` (cada 90, 5 veces, hasta 1519) |
| 119579 | 1267 | camera wait 1320 | 52 | si group1 < 1: no; `2DA51C`→`0403A0` |
| 119631 | 1320 | camera wait 1472 | 147 | `2DA52E`→`0403A0` |
| 119778 | 1472 | camera wait 1520 + lock 1520 | 42 | si group3 < 1: sí; `2DA44E`→`0AF17E` (corredor) |
| 119820 | 1520 | camera wait 1520 | 0 | — |
| 119820 | 1520 | condition wait group7 < 1 | 184 | — |
| 120004 | 1520 | timer wait 60 | 120 | si group3 < 1: sí; `2DA44E`→`0AF17E` (corredor); `2DA558`→`0AF17E`; `2DA56E`→`0AF17E`; `2DA584`→`0AF17E`; `2DA59A`→`02916E`; `2DA5AE`→`0AF17E`; `2DA5C4`→`0AF17E`; `2DA5DA`→`0AF17E`; `2DA5F0`→`02916E` |
| 120124 | 1520 | condition wait group2 < 1 | 430 | si group3 < 1: sí (×2); `2DA44E`→`0AF17E` (corredor) (×2); `2DA60A`→`05D916`\*; `2DA61E`→`05D916`\*; `2DA632`→`05D916`\*; `2DA646`→`05D916`\* |
| 120554 | 1520 | condition wait groups0..7 < 1 | 365 | camera unlock |
| 120919 | 1520 | camera wait 1520 | 0 | autoscroll start |
| 120919 | 1520 | camera wait 1600 | 58 | cámara `0B8AC4`→`039382`\*; autoscroll stop |
| 120977 | 1600 | camera wait 1600 | 0 | `2DA678`→`0403A0` |
| 120977 | 1600 | camera wait 1616 | 10 | `2DA68A`→`09BA2A`\* |
| 120987 | 1616 | camera wait 1624 | 6 | `2DA6A2`→`0403A0` |
| 120993 | 1625 | camera wait 1632 | 4 | `2DA6B4`→`0391EA`\* |
| 120997 | 1633 | camera wait 1640 | 4 | `2DA6C6`→`04115A` |
| 121001 | 1642 | camera wait 1680 | 25 | `2DA6E2`→`09BA2A`\* |
| 121026 | 1684 | camera wait 1712 | 19 | `2DA6FA`→`08A732`\* |
| 121045 | 1712 | camera wait 1712 | 0 | `2DA712`→`0403A0` |
| 121045 | 1712 | camera wait 1736 | 31 | `2DA724`→`0AF17E`; corredor `2DA73A` (cada 30, 2 veces, hasta 3000); corredor `2DA762` (cada 30, 2 veces, hasta 3000) |
| 121076 | 1738 | camera wait 1768 | 33 | si group1 < 2: sí; `2DA74C`→`0AF17E` (corredor); si group1 < 2: no; `2DA790`→`09AE1E`\* |
| 121109 | 1768 | camera wait 1792 | 65 | si group1 < 2: no; `2DA7A8`→`099DCC` |
| 121174 | 1792 | camera wait 1836 | 43 | si group1 < 2: no; cámara `0B8B08`→`03938A`\*; `2DA7C2`→`0AF17E` |
| 121217 | 1836 | camera wait 1856 | 18 | `2DA7DC`→`0403A0` |
| 121235 | 1856 | camera wait 1880 | 21 | si group1 < 2: sí; `2DA74C`→`0AF17E` (corredor); `2DA7EE`→`0403A0` |
| 121256 | 1881 | camera wait 1888 | 6 | `2DA800`→`0391EA`\* |
| 121262 | 1890 | camera wait 1928 | 27 | si group1 < 2: no; azar `2DA812`→`2DA83C`; `2DA83E`→`09BA2A`\*; `2DA850`→`05D226`\* |
| 121289 | 1929 | camera wait 1968 | 52 | `2DA86C`→`0403A0` |
| 121341 | 1968 | camera wait 1976 | 12 | `2DA880`→`0AF17E`; `2DA896`→`0AF17E`; corredor `2DA8AE` (cada 60, 3 veces, hasta 2352); corredor `2DA8D6` (cada 60, 3 veces, hasta 2352) |
| 121353 | 1977 | camera wait 2112 | 151 | si group2 < 2: sí (×2); `2DA8C0`→`0AF17E` (corredor); si group1 < 2: no; `2DA8E8`→`0AF17E` (corredor); cámara `0B8B28`→`038D1E`\*; `2DA904`→`08C37C`; `2DA914`→`08C37C` |
| 121504 | 2112 | camera wait 2148 | 30 | si group1 < 2: sí; `2DA774`→`0AF17E` (corredor); `2DA92A`→`08C37C`; `2DA93A`→`08C37C` |
| 121534 | 2148 | camera wait 2184 | 35 | `2DA950`→`08C37C`; `2DA960`→`08C37C`; set region 4330 |
| 121569 | 2184 | camera wait 2184 | 0 | `2DA97C`→`04115A` |
| 121569 | 2184 | camera wait 2344 | 179 | si group2 < 2: no (×2); si group1 < 2: sí; `2DA774`→`0AF17E` (corredor); cámara `0B8B4A`→`038D26`\*; si player_query_0F == 1810: no; si player_query_0E == 1814: sí; `2DA9B2`→`04115A` |
| 121748 | 2347 | camera wait 2360 | 14 | `2DA9CE`→`08C37C`; `2DA9DE`→`08C37C` |
| 121762 | 2363 | camera wait 2392 | 25 | `2DA9F4`→`09AE1E`\* |
| 121787 | 2395 | camera wait 2408 | 12 | `2DAA0C`→`099DCC` |
| 121799 | 2408 | camera wait 2648 | 232 | azar `2DAA26`→`2DAA64`; `2DAA64`→`099DCC` |
| 122031 | 2650 | camera wait 2720 | 66 | azar `2DAA80`→`2DAA96`; `2DAA96`→`099DCC`; `2DAAC0`→`01913C` |
| 122097 | 2721 | camera wait 2768 | 36 | `2DAAD2`→`097FCC`\* |
| 122133 | 2769 | camera wait 2784 | 12 | `2DAAEE`→`097FCC`\* |
| 122145 | 2784 | camera wait 2888 + lock 2888 | 87 | — |
| 122232 | 2888 | camera wait 2888 + lock 2888 | 0 | `2DAB20`→`05C53C`\*; `2DAB30`→`04E990`\* |
| 122232 | 2888 | condition wait group0 == 0 | 872 | `2DAB44`→`04E990`\*; `2DAB52`→`04E990`\* |
| 123104 | 2888 | condition wait group0 == 0 | 732 | camera unlock |
| 123836 | 2888 | camera wait 2952 | 20 | `2DAB6E`→`0AF17E`; `2DAB82`→`0AF17E` |
| 123856 | 2959 | camera wait 3048 | 20 | azar `2DAB9C`→`2DAB9E`; `2DAB9E`→`08C172` |
| 123876 | 3052 | camera wait 3120 | 36 | `2DABBE`→`08DA12`\*; `2DABD0`→`09A67E` |
| 123912 | 3122 | camera wait 3192 | 58 | `2DABEA`→`0AF17E`; `2DABFE`→`0AF17E` |
| 123970 | 3194 | camera wait 3224 | 18 | `2DAC18`→`08DA12`\* |
| 123988 | 3225 | camera wait 3240 | 8 | `2DAC30`→`09A67E` |
| 123996 | 3241 | camera wait 3296 + lock 3296 | 38 | cámara `0B8BA2`→`0B91FA`\* |
| 124034 | 3296 | camera wait 3296 | 0 | `2DAC58`→`0A2F2E`\*; `2DAC6A`→`08AFB6`\*; corredor `2DAC7A` (cada 2, 1 veces, hasta 3536) |
| 124034 | 3296 | condition wait group7 < 1 | 650 | si group5 < 1: no (×23); si group5 < 1: sí; si group7 == 1: sí; azar `2DAC94`→`2DAC96`; `2DAC96`→`099E1C` (corredor); si group7 < 1: no; camera unlock |
| 124684 | 3296 | camera wait 3304 | 6 | `2DACE0`→`099DCC` |
| 124690 | 3307 | camera wait 3328 | 8 | `2DACFA`→`099DCC` |
| 124698 | 3331 | camera wait 3400 | 36 | `2DAD14`→`04115A` |

#### Sección 3 (hueco)

| Fotograma | Cámara | Orden que espera | Fotogramas | Después |
| --- | --- | --- | --- | --- |
| 124734 | 3400 | camera wait 3560 | 1520 | cámara `0B8BF0`→`0B9286`\*; set player reference 69b8 |
| 126254 | 3566 | camera wait 3716 + lock 3716 | 38 | — |

#### Sección 4 (tramo final)

| Fotograma | Cámara | Orden que espera | Fotogramas | Después |
| --- | --- | --- | --- | --- |
| 126292 | 3716 | camera wait 3716 + lock 3716 | 0 | si player_query_0B == 1818: sí; `2DAD56`→`0DC97E`; si player_query_0C == 1818: no; corredor `2DAD8E` (cada 30, 8 veces, hasta 3800) |
| 126292 | 3716 | condition wait group6 == 0 | 5966 | si group2 < 1: sí (×8); azar `2DADA2`→`2DADA4` (×8); `2DADA4`→`0AF17E` (corredor) (×8); si group2 < 1: no (×12); set player reference 69b8; camera unlock; `2DADCA`→`09A692` |
| 132258 | 3716 | camera wait 3904 + lock 3904 | 44 | — |
| 132302 | 3904 | camera wait 3904 | 0 | `2DADF4`→`0B9400`\* |
| 132302 | 3904 | condition wait group7 == 0 | 62 | camera unlock |
| 132364 | 3904 | camera wait 3944 + lock 3944 | 14 | — |
| 132378 | 3944 | camera wait 3944 | 0 | — |
| 132378 | 3944 | condition wait group0 == 0 | 0 | `2DAE20`→`0A4A94`\* |
| 132378 | 3944 | condition wait group0 == 0 | 8615 | camera unlock; `2DAE36`→`01913C` |
| 140993 | 3944 | camera wait 4112 | 133 | `2DAE48`→`08C172` |
| 141126 | 4113 | camera wait 4208 | 52 | `2DAE5C`→`0403A0` |
| 141178 | 4208 | camera wait 4248 | 24 | `2DAE6E`→`08A732`\* |
| 141202 | 4248 | camera wait 4280 | 22 | `2DAE86`→`0403A0` |
| 141224 | 4280 | camera wait 4296 | 12 | `2DAE98`→`0569E6`\* |
| 141236 | 4298 | camera wait 4312 | 10 | `2DAEAE`→`0403A0` |
| 141246 | 4313 | camera wait 4368 | 30 | `2DAEC0`→`0569D0`\* |
| 141276 | 4372 | camera wait 4392 | 12 | corredor `2DAED2` (cada 60, 3 veces, hasta 4432) |
| 141288 | 4392 | camera wait 4416 | 54 | si group1 < 2: sí; azar `2DAEE4`→`2DAEE6`; `2DAEE6`→`0AF17E` (corredor); `2DAF46`→`099DCC` |
| 141342 | 4418 | camera wait 4464 | 56 | `2DAF60`→`09BA2A`\* |
| 141398 | 4464 | camera wait 4496 | 16 | `2DAF78`→`0403A0` |
| 141414 | 4497 | camera wait 4552 | 72 | `2DAF8A`→`0569B4`\* |
| 141486 | 4553 | camera wait 4584 | 16 | `2DAF9E`→`0403A0` |
| 141502 | 4584 | camera wait 4656 | 38 | `2DAFB0`→`0569D0`\* |
| 141540 | 4659 | camera wait 4680 | 96 | corredor `2DAFC2` (cada 60, 3 veces, hasta 4720) |
| 141636 | 4680 | camera wait 4688 | 14 | si group1 < 2: sí; azar `2DAFD4`→`2DAFD6`; `2DAFD6`→`0AF17E` (corredor); corredor `2DB00A` (cada 60, 3 veces, hasta 4720) |
| 141650 | 4688 | camera wait 4752 | 68 | si group1 < 2: sí; azar `2DB01C`→`2DB01E`; `2DB01E`→`0AF17E` (corredor); `2DB052`→`0569E6`\* |
| 141718 | 4752 | camera wait 4856 | 96 | `2DB068`→`0403A0` |
| 141814 | 4856 | camera wait 4872 | 8 | `2DB07A`→`0403A0` |
| 141822 | 4872 | camera wait 4872 | 0 | `2DB08C`→`0569D0`\* |
| 141822 | 4872 | camera wait 4936 | 32 | `2DB09E`→`0569D0`\* |
| 141854 | 4936 | camera wait 5024 | 176 | `2DB0B0`→`09BA2A`\* |
| 142030 | 5025 | camera wait 5048 | 12 | `2DB0C8`→`08A732`\* |
| 142042 | 5048 | camera wait 5096 | 26 | `2DB0E0`→`093C9A`\* |
| 142068 | 5099 | camera wait 5112 | 20 | `2DB0F0`→`09BA2A`\* |
| 142088 | 5112 | camera wait 5160 | 38 | `2DB108`→`0AC866`\* |
| 142126 | 5160 | camera wait 5328 | 100 | `2DB118`→`05ED84` |
| 142226 | 5330 | camera wait 5372 | 20 | `2DB12E`→`08A732`\* |
| 142246 | 5373 | camera wait 5504 | 85 | si player_query_15 == 1812: sí; `2DB14A`→`087E36`; si player_query_14 == 1812: no |
| 142331 | 5504 | camera wait 5520 | 142 | `2DB17C`→`08A732`\* |
| 142473 | 5520 | camera wait 5544 | 24 | corredor `2DB194` (cada 15, 8 veces, hasta 5900) |
| 142497 | 5545 | camera wait 5600 | 49 | si group5 == 0: sí; si player_query_0C == 2562: no; si player_query_0B == 2562: sí; azar `2DB204`→`2DB206`; si player_query_14 == 1808: no; si player_query_15 == 1812: sí; `2DB220`→`0AF17E` (corredor); `2DB234`→`02B202`\* (corredor); si group5 == 0: no; `2DB260`→`099DCC` |
| 142546 | 5600 | camera wait 5664 | 44 | si group5 == 0: no; si player_query_12 == 2306: sí; si player_query_14 == 1812: no; si player_query_15 == 1812: sí; `2DB29E`→`0A2F2E`\*; si player_query_13 == 2306: no |
| 142590 | 5667 | camera wait 5704 | 20 | corredor `2DB2F4` (cada 30, 8 veces, hasta 5864) |
| 142610 | 5707 | camera wait 5912 | 180 | si group1 < 1: sí (×2); azar `2DB306`→`2DB31E` (×2); `2DB31E`→`0AF17E` (corredor) (×2); si group5 == 0: sí (×2); si player_query_0C == 2562: no (×2); si player_query_0B == 2562: sí (×2); azar `2DB204`→`2DB248`; `2DB248`→`084588`\* (corredor); si group5 == 0: no (×3); azar `2DB204`→`2DB206`; si player_query_14 == 1808: no; si player_query_15 == 1812: sí; `2DB220`→`0AF17E` (corredor); `2DB234`→`02B202`\* (corredor); set region 4330; si player_query_0B == 2306: sí; si player_query_0E == 2306: no; si player_query_0F == 2306: sí; si player_query_14 == 1814: no; si player_query_15 == 1814: sí; `2DB3AE`→`097FCC`\*; si player_query_0C == 2306: no |
| 142790 | 5912 | camera wait 5980 + lock 5980 | 39 | — |
| 142829 | 5980 | camera wait 5980 + lock 5980 | 0 | — |
| 142829 | 5980 | condition wait group0 == 0 | 484 | si player_query_0B == 2306: sí; si player_query_0E == 2306: no; si player_query_0F == 2306: sí; si player_query_14 == 1816: no; si player_query_15 == 1816: sí; `2DB4E2`→`097FCC`\*; si player_query_0C == 2306: no |
| 143313 | 5980 | condition wait group0 == 0 | 487 | si player_query_14 == 1816: no; si player_query_15 == 1816: sí; `2DB5B0`→`097FCC`\* |
| 143800 | 5980 | condition wait group0 == 0 | 349 | camera unlock |
| 144149 | 5980 | camera wait 6496 | 334 | `2DB5D8`→`01913C`; `2DB5E4`→`04A2C2`\* |

#### Sección 5 (jefe)

| Fotograma | Cámara | Orden que espera | Fotogramas | Después |
| --- | --- | --- | --- | --- |
| 144483 | 6496 | timer wait 900 | 1800 | si player_query_14 == 1810: no; azar `2DB60A`→`2DB622`; si player_query_15 == 1810: sí; `2DB626`→`099862` |
| 146283 | 6496 | timer wait 900 | 1800 | si player_query_14 == 1812: no; si player_query_15 == 2562: sí; azar `2DB65E`→`2DB660`; `2DB660`→`099E1C` |
| 148083 | 6496 | timer wait 900 | — | **fin `018F92`**; **fin `018870`**; misión→`0308C0`\*; misión→`06090A`\* |

## 2. Inventario de actores

Cada constructor que `0040BC` creó en la misión, agrupado por **familia**
(el registro del guion, la llamada de la lista de cámara o el actor de
misión del que desciende; «común» si desciende de varias; «jugador» si lo
crea el jugador). Programas: raíces que el nodo pasó a `004256` (los tres
primeros; la lista completa en `data/mission3_actors.json`); un programa de
otra familia (`2404FE`, `240DEE`, `22CE6C`...) indica código compartido.
Sonidos: palabras de `0120F8` pedidas con el nodo en `A6`. Port: «nativo»
= `neo_world_spawn_native`; «nativo (`fichero`)» = lo lleva otro módulo;
`work-m3c` = solo en esa rama sin fusionar. «Programas de soldado» = raíces que
también arranca el soldado `0AF17E` (código compartido, no identidad). «Qué es» solo dice lo que
muestran la captura, la ROM o una pantalla de `run4` (`game<f>.png`); «sin
identificar» cuando no hay más.

| Familia | Constructor | Veces | Primero (fotograma / cámara) | Secciones | Creador | Programas (`004256`) | Sonidos (`0120F8`) | Port | Qué es |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| `0B8D64` | `0B8D64` | 1 | 117969 / 0 | 1:1 | misión | — | — | nativo (`neo_train.c`) | conductor del tren (`10E94E`) |
| `0B8EDC` | `0B8EDC` | 1 | 117969 / 0 | 1:1 | misión | — | — | **falta** | creado con el conductor por la llamada `0B8D34`; probable escritor del cielo tras el progreso 0x140 (MISION3.md, sin comprobar) |
| `01F1D4` | `01F1D4` | 1 | 117970 / 0 | 1:1 | guion | — | — | **falta** | sin identificar; hijos `01F386 01F562 01F2D6 01F4DA` (programas `22F8xx`) |
| 〃 | `01F2D6` | 1 | 117970 / 0 | 1:1 | `01F1D4` | `22F81A` `23DB34` | — | **falta** |  |
| 〃 | `01F386` | 1 | 117970 / 0 | 1:1 | `01F1D4` | `22F868` | — | **falta** |  |
| 〃 | `01F4DA` | 1 | 117970 / 0 | 1:1 | `01F562` | `22F9D8` | — | **falta** |  |
| 〃 | `01F562` | 1 | 117970 / 0 | 1:1 | `01F1D4` | `22F8C2` `22F8F6` `22F98A` | — | **falta** |  |
| `08C172` | `08C172` | 7 | 117970 / 0 | 1:5 2:1 4:1 | guion | `286896` `2868A2` | — | nativo | caja de suministros (mapa dinámico `0E5640`, ROM `08C1DE`) |
| `08C37C` | `08C37C` | 12 | 117970 / 0 | 1:4 2:8 | guion | `2869D6` `286C6C` `287078` +1 | 1121 | nativo | objeto del guion |
| `099DCC` | `099DCC` | 12 | 117970 / 0 | 1:1 2:9 4:2 | guion | `29FAD8` `29FB34` `29FE52` +4 | 1048 1056 113A | nativo | prisionero (regalo en la palabra 3) |
| `09A692` | `09A692` | 2 | 117970 / 0 | 1:1 4:1 | guion | — | — | nativo | caja oculta → prisionero `099E1C` |
| `0AF17E` | `0AF17E` | 52 | 117970 / 0 | 1:4 2:33 4:15 | corredor×27 guion×25 | `23D510` `23D594` `23D5E2` +30 | 103E 1041 1042 1043 104A | nativo | soldado (constructor genérico de la tabla de spawn) |
| 〃 | `040F88` | 5 | 118104 / 0 | 1:2 4:3 | `0AF17E` | `2402FA` | — | nativo (`neo_world.c`) | sangre del soldado |
| `05287C` | `05287C` | 1 | 118022 / 0 | 1:1 | misión | `24953C` `249546` | 10D5 | nativo | flecha GO |
| `0607DE` | `0607DE` | 1 | 118022 / 0 | 1:1 | misión | — | 1133 | nativo (`neo_end.c`) | letras MISSION 3 START |
| `0214EA` | `0214EA` | 1 | 118151 / 35 | 1:1 | guion | `230238` `23035E` `2303CA` +1 | 103F | **falta** | sin identificar; regalo en la palabra 1 (`43`); hijos `0218C4` ×7 |
| 〃 | `0218C4` | 7 | 118248 / 185 | 1:7 | `0214EA` | `230538` | — | **falta** |  |
| `0403A0` | `0403A0` | 23 | 118200 / 97 | 1:2 2:14 4:7 | guion | — | — | nativo | marcador de escalera/repisa (lista de objetos) |
| `03EE8A` | `03EE8A` | 8 | 118210 / 112 | 1:8 | guion | `23FE6C` `23FEA0` `23FEAC` +2 | — | **falta** | sin identificar (8 en la salida, programas `23FE6C..23FEFA`, sin sonido) |
| `09AE2C` | `09AE2C` | 1 | 118652 / 312 | 1:1 | guion | `2404FE` `2A037A` `2A03C6` +2 | 1043 | **falta** | programas de soldado `2404FE` y propios `2A037A..2A048C`; hijo `09C46C` |
| `09BA2A` | `09BA2A` | 8 | 118744 / 360 | 2:5 4:3 | guion | `240DEE` `2A037A` `2A03C6` +3 | 103E 1041 1042 | **falta** | programas de soldado `240DEE` y propios `2A037A..2A0E04`; hijo `09C46C` |
| `08DA12` | `08DA12` | 5 | 118836 / 627 | 2:5 | guion | `23D510` `23D62A` `23DB34` +4 | 103E 1041 1042 1043 | **falta** | programas de soldado `23D510 23DB34 2404FE 240DEE` y propios `28AC56 28ACCE` |
| `09A67E` | `09A67E` | 5 | 118892 / 704 | 2:5 | guion | — | — | nativo | caja oculta (regalo en la palabra 3) |
| `02B202` | `02B202` | 4 | 119050 / 849 | 2:2 4:2 | guion×2 corredor×2 | `236D00` `236D88` | 1021 | **falta** | sin identificar; hijos `02B61C`, `02BBBA`, explosiones; mapa dinámico de la raíz `02C5BA` (y `02C5C8` en la sección 4) |
| 〃 | `02B61C` | 4 | 119050 / 849 | 2:2 4:2 | `02B202` | `23739E` | — | **falta** |  |
| 〃 | `02BBBA` | 6 | 119324 / 1028 | 2:2 4:4 | `02B202` | `2373CC` `2373E6` | — | **falta** |  |
| `0AF1B6` | `0AF1B6` | 1 | 119050 / 849 | 2:1 | guion | `23D5E2` `23E43C` | — | **falta** | programas de soldado (los de `0AF17E` `23D5E2 23E43C`) |
| `01A422` | `01A422` | 14 | 119254 / 993 | 2:8 4:6 | guion | — | — | nativo (`neo_script.c`) | corredor del guion (op 8) |
| `04115A` | `04115A` | 7 | 119335 / 1043 | 2:6 3:1 | guion | `240DEE` `240F8E` `24116E` +3 | 103E 1041 1042 | nativo | soldado (familia de la Misión 1) |
| `027F54` | `027F54` | 1 | 119579 / 1267 | 2:1 | guion | `235BAA` `235FE0` `23606E` +1 | 1021 1033 109D | **falta** | sin identificar; regalo `00` (H); hijo `0287FA`, explosiones; sonidos 1021/1033/109D |
| 〃 | `0287FA` | 1 | 119932 / 1520 | 2:1 | `027F54` | `236314` `23D510` `23DFE4` +1 | — | **falta** |  |
| `02916E` | `02916E` | 2 | 120124 / 1520 | 2:2 | guion | `2367E4` `2367FE` `236818` +1 | 1023 102F 1033 10A5 | nativo | camión (Misión 1); mapas dinámicos `029C9A/029CDC/029D1E` |
| 〃 | `029802` | 2 | 120124 / 1520 | 2:2 | `02916E` | — | — | nativo |  |
| `05D916` | `05D916` | 4 | 120554 / 1520 | 2:4 | guion | `2404FE` `240DEE` `262AD8` +3 | 1042 1043 | **falta** | programas de soldado `2404FE 240DEE` y propios `262AD8..263498`; hijos `05DDA4 05DF3E 05DFC2 05DD2A` |
| 〃 | `05DDA4` | 4 | 120554 / 1520 | 2:4 | `05D916` | `2634E0` `26379C` `2638B0` +1 | — | **falta** |  |
| 〃 | `05DF3E` | 1 | 120732 / 1520 | 2:1 | `05D916` | `263A2E` | — | **falta** |  |
| 〃 | `05DFC2` | 1 | 120732 / 1520 | 2:1 | `05D916` | `22CE6C` `263A00` | 1064 | **falta** |  |
| `039382` | `039382` | 1 | 120945 / 1552 | 2:1 | misión | `23C762` | 1069 | `work-m3c` | caja del vagón → estampador `0B911E` (repisa `0B8FA8`) |
| 〃 | `0B911E` | 1 | 121043 / 1709 | 2:1 | `039382` | — | — | `work-m3c` |  |
| `0391EA` | `0391EA` | 2 | 120997 / 1633 | 2:2 | guion | `23C658` `23C6B4` | — | **falta** | sin identificar; hijos `08AE76` ×8 |
| 〃 | `08AE76` | 8 | 120997 / 1633 | 2:8 | `0391EA` | `285BBE` | — | **falta** |  |
| `08A732` | `08A732` | 5 | 121045 / 1712 | 2:1 4:4 | guion | `2404FE` `240DEE` `285864` +1 | 103E 1041 1042 1043 10A4 | **falta** | programas de soldado `2404FE 240DEE` y propios `285864 2858FE`; hijos `08AA6E`, `08AC54`→`08AE70`, `08AB70` |
| 〃 | `08AA6E` | 5 | 121246 / 1864 | 2:1 4:4 | `08A732` | `285A0C` | — | **falta** |  |
| 〃 | `08AB70` | 1 | 142390 / 5508 | 4:1 | `08A732` | `2859BA` | — | **falta** |  |
| 〃 | `08AC54` | 1 | 142390 / 5508 | 4:1 | `08A732` | `285AB2` | — | **falta** |  |
| 〃 | `08AE70` | 13 | 142393 / 5508 | 4:13 | `08AC54` | `285BBE` | — | **falta** |  |
| `09AE1E` | `09AE1E` | 2 | 121109 / 1768 | 2:2 | guion | `240DEE` `2A037A` `2A0E04` | 103E 1041 1042 | **falta** | programas de soldado `240DEE` y propios `2A037A`, `2A0E04`; hijo `09C46C` |
| `03938A` | `03938A` | 1 | 121188 / 1808 | 2:1 | misión | `23C762` | 1069 | `work-m3c` | caja del vagón → estampador `0B9140` (repisa `0B8FA8`) |
| 〃 | `0B9140` | 1 | 121321 / 1956 | 2:1 | `03938A` | — | — | `work-m3c` |  |
| `05D226` | `05D226` | 1 | 121289 / 1929 | 2:1 | guion | `2404FE` `262B1A` `262DCC` | 1043 | **falta** | programas de soldado `2404FE` y propios `262B1A 262DCC` |
| `038D1E` | `038D1E` | 1 | 121466 / 2080 | 2:1 | misión | `23C624` `23C62C` | — | **falta** | llamada de cámara `0B9148`: hijos `038B06` (→ estampadores `0B90FA/0B90BC/0B9106/0B9112`, 4 de los 6 grupos de parches `0B85EA`), `038F38`→`0390A6` |
| `038D26` | `038D26` | 1 | 121700 / 2304 | 2:1 | misión | `23C624` `23C62C` | — | **falta** | llamada de cámara `0B916A`: como `038D1E` |
| `01913C` | `01913C` | 3 | 122097 / 2721 | 2:1 4:1 5:1 | guion | — | — | nativo | reinicio del tiempo |
| `097FCC` | `097FCC` | 5 | 122133 / 2769 | 2:2 4:3 | guion | `29ED1A` `29EE20` `29EF26` | 1023 10BA | **falta** | sin identificar (sonido 10BA); hijos `097FE4`, `098E4A`, `098886`, `098CB6`, `09879C`→`098974`→`098EA2`, `098C0C/20/34`, `098E18` |
| 〃 | `098886` | 10 | 122133 / 2769 | 2:2 4:8 | `097FCC`×5 `097FE4`×5 | `29EF7C` | — | **falta** |  |
| 〃 | `098E4A` | 10 | 122133 / 2769 | 2:2 4:8 | `097FCC`×5 `097FE4`×5 | — | — | **falta** |  |
| 〃 | `098CB6` | 25 | 122381 / 2888 | 2:1 4:24 | `097FCC`×19 `097FE4`×6 | `22DA42` `22DA98` `22DAEE` +2 | — | **falta** |  |
| 〃 | `097FE4` | 5 | 142790 / 5912 | 4:5 | `097FCC` | `29ED1A` `29EE20` `29EF26` | 1023 | **falta** |  |
| 〃 | `09879C` | 6 | 143069 / 5980 | 4:6 | `097FCC`×3 `097FE4`×3 | `29F032` | — | **falta** |  |
| 〃 | `098974` | 6 | 143081 / 5980 | 4:6 | `09879C` | `22CE6C` `29F06C` | 1022 | **falta** |  |
| 〃 | `098EA2` | 43 | 143083 / 5980 | 4:43 | `098974` | `29F192` | — | **falta** |  |
| 〃 | `098C0C` | 4 | 143187 / 5980 | 4:4 | `097FE4` | `29F090` | — | **falta** |  |
| 〃 | `098C20` | 2 | 143187 / 5980 | 4:2 | `097FE4` | `29F0E6` | — | **falta** |  |
| 〃 | `098C34` | 2 | 143187 / 5980 | 4:2 | `097FE4` | `29F13C` | — | **falta** |  |
| 〃 | `098E18` | 4 | 143310 / 5980 | 4:4 | `097FCC`×3 `097FE4`×1 | `22CF2C` | — | **falta** | (registros del guion y de `097FCC`) |
| `04E990` | `04E990` | 3 | 122232 / 2888 | 2:3 | guion | `247730` | 1021 105E | **falta** | sin identificar; mapa dinámico `0E112E` (ROM `04EEDC`); 9 hijos, `04F720` ×241 |
| 〃 | `04EE18` | 3 | 122232 / 2888 | 2:3 | `04E990` | `2477C2` | — | **falta** |  |
| 〃 | `04EE88` | 3 | 122232 / 2888 | 2:3 | `04E990` | — | — | **falta** |  |
| 〃 | `04EF88` | 3 | 122232 / 2888 | 2:3 | `04E990` | `247854` `2478B0` `24790C` +1 | 103F | **falta** |  |
| 〃 | `04F2DC` | 3 | 122232 / 2888 | 2:3 | `04E990` | `2479C4` | — | **falta** |  |
| 〃 | `04F358` | 12 | 122411 / 2888 | 2:12 | `04E990` | `247A76` | — | **falta** |  |
| 〃 | `04F44E` | 12 | 122411 / 2888 | 2:12 | `04E990` | `22CE6C` `247AFE` `247B18` +3 | 1022 | **falta** |  |
| 〃 | `04F464` | 12 | 122411 / 2888 | 2:12 | `04E990` | `22CE6C` `247AFE` `247B18` +3 | 1022 1067 | **falta** |  |
| 〃 | `04F720` | 241 | 122436 / 2888 | 2:241 | `04F464`×127 `04F44E`×114 | `247DA8` | — | **falta** |  |
| 〃 | `04EC8C` | 18 | 123036 / 2888 | 2:18 | `04E990` | `22DA42` `22DA98` `22DAEE` +2 | — | **falta** |  |
| `05C53C` | `05C53C` | 1 | 122232 / 2888 | 2:1 | guion | `2404FE` `240DEE` `262AD8` | 1042 1043 | **falta** | programas de soldado `2404FE 240DEE` y propio `262AD8` |
| `08AFB6` | `08AFB6` | 1 | 124034 / 3296 | 2:1 | guion | `285C6A` `285D3A` | — | **falta** | sin identificar; mapas dinámicos `0E53D4/0E53C2`; hijo `08B3E6` |
| 〃 | `08B3E6` | 1 | 124034 / 3296 | 2:1 | `08AFB6` | `285D96` `285EAA` | — | **falta** |  |
| `0A2F2E` | `0A2F2E` | 2 | 124034 / 3296 | 2:1 4:1 | guion | `2A392A` `2A3E92` `2A4056` +3 | 1021 1033 1064 109E | **falta** | sin identificar; regalo `00` (H); hijos `0A3A0A 0A38F4 0A3BD6 0A3C24` |
| 〃 | `0A38F4` | 2 | 124034 / 3296 | 2:1 4:1 | `0A2F2E` | `2A3958` `2A3EDA` `2A409E` +4 | 102D | **falta** |  |
| 〃 | `0A3A0A` | 2 | 124034 / 3296 | 2:1 4:1 | `0A2F2E` | `2404FE` `2A3990` `2A3FB4` +5 | 1043 | **falta** |  |
| 〃 | `0A3BD6` | 6 | 124176 / 3296 | 2:4 4:2 | `0A2F2E` | `2A4866` | — | **falta** |  |
| 〃 | `0A3C24` | 6 | 124176 / 3296 | 2:4 4:2 | `0A2F2E` | `22CE6C` `2A48FE` | 1022 | **falta** |  |
| `0B91FA` | `0B91FA` | 1 | 124034 / 3296 | 2:1 | misión | `2A8E40` | — | **falta** | llamada de cámara `0B8BA2` (progreso 3296), programa `2A8E40` |
| `0B9286` | `0B9286` | 1 | 126244 / 3536 | 3:1 | misión | `2A8E50` | — | **falta** | llamada de cámara `0B8BF0` (3536): plataforma del hueco, mapa dinámico `0E6896` (ROM `0B9376/0B93A8`, 126245..141259); hijos `0B9748` (sonido 1121), `0B93B4` |
| 〃 | `0B93B4` | 1 | 126244 / 3536 | 3:1 | `0B9286` | `2A8EA6` | — | **falta** |  |
| 〃 | `0B9748` | 1 | 126244 / 3536 | 3:1 | `0B9286` | — | 1121 | **falta** |  |
| `0DC97E` | `0DC97E` | 1 | 126292 / 3716 | 4:1 | guion | `2AF47A` `2AFB56` | 102F 10E8 | nativo | bombarderos (Misión 1, `neo_flyer.c`) |
| 〃 | `0DD3E0` | 1 | 126292 / 3716 | 4:1 | `0DC97E` | `2AF960` | — | nativo |  |
| 〃 | `0DD642` | 192 | 126372 / 3716 | 4:192 | `0DC97E` | `2919FE` `2AFC60` | 1021 1022 109B | nativo |  |
| 〃 | `0DD802` | 192 | 126372 / 3716 | 4:192 | `0DC97E` | `2AFCCA` | — | nativo |  |
| `0B9400` | `0B9400` | 1 | 132302 / 3904 | 4:1 | guion | — | — | **falta** | sin identificar (sin programa ni sonido ni hijos; antes de `group7 == 0`) |
| `0A4A94` | `0A4A94` | 1 | 132378 / 3944 | 4:1 | guion | — | 1023 1032 1033 107B 10EB 10EC 10ED | **falta** | jefe intermedio: la aeronave «96» (pantalla `game133500`); superposición `10E8C6` (`0A6C8C`, mapa `2A6142`); 8615 fotogramas de `group0 == 0` |
| 〃 | `0A606E` | 1 | 132378 / 3944 | 4:1 | `0A4A94` | `2A4D16` `2A4D2E` `2A4DC4` | — | **falta** |  |
| 〃 | `0A6078` | 1 | 132378 / 3944 | 4:1 | `0A4A94` | `2A4D22` `2A4DD0` `2A4E66` | — | **falta** |  |
| 〃 | `0A61E8` | 1 | 132378 / 3944 | 4:1 | `0A4A94` | `2A4EEE` `2A4F5E` `2A4FBE` | 1121 | **falta** |  |
| 〃 | `0A61F2` | 1 | 132378 / 3944 | 4:1 | `0A4A94` | `2A4F26` `2A4F8E` `2A50F6` | 1121 | **falta** |  |
| 〃 | `0A64E2` | 286 | 132380 / 3944 | 4:286 | `0A61F2`×125 `0A61E8`×125 `0A64E2`×32 `0A4A94`×4 | `2A4E72` `2A4EA6` `2A522E` +9 | — | **falta** |  |
| 〃 | `0A5E88` | 1 | 132674 / 3944 | 4:1 | `0A4A94` | `2A5950` | — | **falta** |  |
| 〃 | `0A6598` | 56 | 133780 / 3944 | 4:56 | `0A4A94` | `22DA42` `22DA98` `22DAEE` +2 | — | **falta** |  |
| 〃 | `0A4DB6` | 1 | 134292 / 3944 | 4:1 | `0A4A94` | — | — | **falta** |  |
| 〃 | `0A4F3A` | 7 | 134292 / 3944 | 4:7 | `0A4DB6` | `22CF2C` `2A5A62` `2A5A90` +7 | 1020 102F 1067 | **falta** |  |
| 〃 | `0A4F52` | 7 | 134292 / 3944 | 4:7 | `0A4DB6` | `22CF2C` `2A5A62` `2A5A90` +7 | 1020 102F 1067 | **falta** |  |
| 〃 | `0A5398` | 32 | 134527 / 3944 | 4:32 | `0A4F3A`×17 `0A4F52`×15 | `22CE6C` `2919FE` `2A5F54` | 1022 10F2 | **falta** |  |
| 〃 | `0A5D6A` | 2 | 134729 / 3944 | 4:2 | `0A4A94` | `2A5626` | — | **falta** |  |
| 〃 | `08D0D2` | 2 | 135108 / 3944 | 4:2 | `0A61E8`×1 `0A61F2`×1 | `285EC0` `285EE8` | — | **falta** |  |
| 〃 | `0A4DB0` | 1 | 140655 / 3944 | 4:1 | `0A4A94` | — | — | **falta** |  |
| `0569E6` | `0569E6` | 2 | 141236 / 4298 | 4:2 | guion | `240DEE` `24AC4C` `24AC56` +5 | 103E 1042 | **falta** | programas de soldado `240DEE` y propios `24AC4C..24B5CC`; hijos `056E5A 056C94 056B00 056A6A` |
| `0569D0` | `0569D0` | 4 | 141276 / 4372 | 4:4 | guion | `2404FE` `240DEE` `24AC4C` +7 | 103E 1042 1043 | **falta** | programas de soldado `2404FE 240DEE` y propios `24AC4C..24B7AE`; hijos `056E5A 056D76 056C94 056B00 056A6A` |
| 〃 | `056D76` | 4 | 141294 / 4392 | 4:4 | `0569D0` | — | — | **falta** |  |
| `0569B4` | `0569B4` | 1 | 141486 / 4553 | 4:1 | guion | `2404FE` `240DEE` `24AC4C` +5 | 1042 1043 | **falta** | programas de soldado `2404FE 240DEE` y propios `24AC4C..24B5CC` |
| `093C9A` | `093C9A` | 1 | 142068 / 5099 | 4:1 | guion | `291CE4` | — | **falta** | vehículo: crea la oruga `014A4E`, la torreta `03B4E0` y el marcador `03523C` del SV-001 (programa `291CE4`) |
| 〃 | `014A4E` | 1 | 142068 / 5099 | 4:1 | `093C9A` | — | — | nativo |  |
| `0AC866` | `0AC866` | 1 | 142126 / 5160 | 4:1 | guion | `0AB028` `0AB09E` `0AB0AC` +2 | 1034 108B 1090 10E9 | **falta** | vehículo con marcador «IN» (pantalla `game142140`); torreta `03B4E0`, marcador `03523C`, piezas `0AD9B8`; mapa dinámico `0AAC94` |
| 〃 | `0AD2AC` | 1 | 142246 / 5373 | 4:1 | `0AC866` | `0AB48C` `0AB654` | — | **falta** |  |
| 〃 | `0ADEDA` | 1 | 142246 / 5373 | 4:1 | `0AC866` | `0AC288` `0AC2D2` | — | **falta** |  |
| 〃 | `01BF10` | 1 | 142260 / 5400 | 4:1 | `0AC866` | `22C950` `22CB60` | 1027 | nativo | granada |
| 〃 | `0AE03A` | 1 | 142260 / 5400 | 4:1 | `0AC866` | `23A84E` | — | **falta** |  |
| 〃 | `0AD3E2` | 1 | 142266 / 5404 | 4:1 | `0AC866` | `0AC214` | 1090 | **falta** |  |
| 〃 | `0AD856` | 1 | 142268 / 5404 | 4:1 | `0AC866` | `22D372` | — | **falta** | registro del guion (rango de `0AC866`, sin identificar) |
| 〃 | `0AD776` | 1 | 142270 / 5404 | 4:1 | `0AC866` | `0AD738` | — | **falta** |  |
| 〃 | `059C82` | 6 | 142321 / 5487 | 4:6 | `0AC866` | `247DA8` | — | nativo | efecto (familia del lanzacohetes) |
| 〃 | `0AD5D4` | 1 | 142321 / 5487 | 4:1 | `0AC866` | `0AC61C` | — | **falta** | registro del guion (rango de `0AC866`, sin identificar) |
| 〃 | `0AD6E4` | 1 | 142321 / 5487 | 4:1 | `0AC866` | `0AC66E` | — | **falta** |  |
| 〃 | `0AD8A2` | 1 | 142353 / 5508 | 4:1 | `0AC866` | `22CF2C` | — | nativo | explosión del caminante |
| 〃 | `0AD9B8` | 20 | 142353 / 5508 | 4:20 | `0AD9B8`×17 `0AC866`×3 | `0AD8CE` | — | nativo | pieza de tanque |
| 〃 | `0AD58C` | 1 | 142408 / 5508 | 4:1 | `0AD5D4` | `0AD53A` | — | **falta** | hijo de `0AD5D4` |
| `05ED84` | `05ED84` | 1 | 142226 / 5330 | 4:1 | guion | `263C22` `263D7A` `264C94` | 1021 102F | nativo | lanzacohetes (Misión 1) |
| `087E36` | `087E36` | 1 | 142331 / 5504 | 4:1 | guion | `2842DE` `2846BA` `2847DE` | 1021 102F 1072 | nativo | blindado (Misión 1; mapa `0E4FF2`) |
| 〃 | `040396` | 1 | 142333 / 5506 | 4:1 | `087E36` | — | — | nativo |  |
| 〃 | `08982A` | 1 | 142347 / 5508 | 4:1 | `087E36` | `239BCA` | — | nativo | humo del blindado |
| 〃 | `08943C` | 6 | 142357 / 5508 | 4:6 | `087E36` | `247DA8` | — | nativo | piezas del blindado |
| 〃 | `0897E2` | 1 | 142431 / 5508 | 4:1 | `087E36` | `24855C` | 1025 | nativo | humo del blindado |
| `084588` | `084588` | 1 | 142611 / 5707 | 4:1 | corredor | `281FFC` | 1021 105D | **falta** | sin identificar (de un corredor); hijos `083B30`, `08471A`→`0848EE` ×18, `084948`, `08497E` |
| 〃 | `083B30` | 1 | 142611 / 5707 | 4:1 | `084588` | `28207A` | — | **falta** |  |
| 〃 | `08471A` | 1 | 142656 / 5782 | 4:1 | `084588` | `24855C` `2824F4` | 1021 1025 | **falta** |  |
| 〃 | `0848EE` | 18 | 142656 / 5782 | 4:18 | `08471A` | `282518` | — | **falta** |  |
| 〃 | `084948` | 1 | 142658 / 5785 | 4:1 | `084588` | `240C76` | 103F | **falta** |  |
| 〃 | `08497E` | 2 | 142658 / 5785 | 4:2 | `084588` | `28236E` `282388` | — | **falta** |  |
| `04A2C2` | `04A2C2` | 1 | 144483 / 6496 | 5:1 | guion | `2451DC` | 1026 1032 1033 | **falta** | jefe: robot andante (pantalla `game146490`); mapa dinámico `0E0D00` (ROM `04E06A`), música `059F76`; `018F92` lo llama `04A9D4` (nodo del jefe) |
| 〃 | `04AF02` | 1 | 144483 / 6496 | 5:1 | `04A2C2` | `2451E8` `24569C` `24585A` +1 | — | **falta** |  |
| 〃 | `04B0E6` | 1 | 144483 / 6496 | 5:1 | `04A2C2` | `245294` `24571E` `2458C8` +1 | — | **falta** |  |
| 〃 | `04B252` | 1 | 144483 / 6496 | 5:1 | `04A2C2` | `245340` `2457A0` `245936` | 1078 | **falta** |  |
| 〃 | `04B3BC` | 1 | 144483 / 6496 | 5:1 | `04B252` | `2459E8` `2459F4` `245A48` | 1065 | **falta** |  |
| 〃 | `04B920` | 1 | 144483 / 6496 | 5:1 | `04B252` | `2459E8` `245D86` `245D92` +4 | 1062 1075 10A1 | **falta** |  |
| 〃 | `04C38A` | 1 | 144483 / 6496 | 5:1 | `04B252` | `2459E8` `246262` `246326` +4 | 10EF | **falta** | pieza del jefe; mapa dinámico `0E0E36` (ROM `04C6F4`) |
| 〃 | `04C69E` | 1 | 144483 / 6496 | 5:1 | `04C38A` | `2451DC` | — | **falta** |  |
| 〃 | `04CF2C` | 1 | 144483 / 6496 | 5:1 | `04B252` | — | — | **falta** |  |
| 〃 | `059F76` | 1 | 144483 / 6496 | 5:1 | `04A2C2` | — | 0028 | nativo |  |
| 〃 | `04E2EA` | 9 | 144789 / 6496 | 5:9 | `04AF02` | `247034` | 10EE | **falta** |  |
| 〃 | `04E3BE` | 9 | 144789 / 6496 | 5:9 | `04B0E6` | `247598` | — | **falta** |  |
| 〃 | `04AD12` | 27 | 144801 / 6496 | 5:27 | `04E2EA` | `246AD2` | — | **falta** |  |
| 〃 | `04AD94` | 27 | 144801 / 6496 | 5:27 | `04E3BE` | `246AD2` | — | **falta** |  |
| 〃 | `04B79C` | 12 | 144903 / 6496 | 5:12 | `04B3BC` | `245BA0` | — | **falta** |  |
| 〃 | `04B87E` | 176 | 144907 / 6496 | 5:176 | `04B79C` | — | — | **falta** |  |
| 〃 | `04CAB4` | 11 | 144919 / 6496 | 5:11 | `04B3BC` | `22CE6C` `247656` `24855C` | 1021 1025 | **falta** |  |
| 〃 | `04CD32` | 126 | 144925 / 6496 | 5:126 | `04CAB4` | `24767A` | — | **falta** |  |
| 〃 | `04D1DA` | 28 | 145163 / 6496 | 5:28 | `04CF2C` | `22CDD4` `2464BA` | 1022 109A | **falta** |  |
| 〃 | `04C7B6` | 12 | 146269 / 6496 | 5:12 | `04C38A` | `246694` | — | **falta** |  |
| 〃 | `04C840` | 32 | 146395 / 6496 | 5:32 | `04C38A` | `2465DA` | — | **falta** |  |
| 〃 | `04C194` | 9 | 146695 / 6496 | 5:9 | `04B920` | `22CDD4` `24683A` | 109F | **falta** |  |
| 〃 | `01D3D2` | 16 | 149125 / 6496 | 5:16 | `04A2C2` | `22DAEE` `22DC2C` | — | nativo |  |
| `099862` | `099862` | 1 | 146283 / 6496 | 5:1 | guion | `29F6D0` `29F8A0` `29F9AC` +2 | 1139 | nativo | prisionero (regalo `06`) |
| `0308C0` | `0308C0` | 1 | 149229 / 6496 | 5:1 | misión | — | — | nativo (`neo_tally.c`) | recuento de prisioneros |
| 〃 | `030B08` | 1 | 149261 / 6496 | 5:1 | `030B0A` | — | — | nativo (`neo_tally.c`) |  |
| 〃 | `030B0A` | 1 | 149261 / 6496 | 5:1 | `0308C0` | `237AEC` | — | nativo (`neo_tally.c`) |  |
| `06090A` | `06090A` | 1 | 149428 / 6496 | 5:1 | misión | — | — | nativo (`neo_end.c`) | letras MISSION COMPLETE |
| común | `03C62A` | 524 | 117970 / 0 | 1:18 2:119 3:25 4:301 5:61 | `01BF46`×337 `01BEDA`×60 `0AF17E`×41 `01C362`×36 jugador×16 `099DCC`×11 `09BA2A`×7 `08DA12`×5 `099E1C`×3 `09AE2C`×1 `01F2D6`×1 `0287FA`×1 `05D916`×1 `09AE1E`×1 `05D226`×1 `01BF10`×1 `087E36`×1 | `23F774` `23F8D0` | — | nativo | ondas (efecto común, `neo_gate.c` y otros) |
| 〃 | `060A72` | 29 | 118022 / 0 | 1:13 5:16 | `06090A`×16 `0607DE`×13 | `264FF8` `265002` `26500C` +10 | — | nativo (`neo_end.c`) |  |
| 〃 | `060B06` | 2 | 118022 / 0 | 1:1 5:1 | `0607DE`×1 `06090A`×1 | `265098` | — | nativo (`neo_end.c`) |  |
| 〃 | `01BC2E` | 431 | 118090 / 0 | 1:3 4:204 5:224 | `01B782`×244 `01B6DC`×170 `01B6A0`×17 | `22E504` | — | nativo | impacto de bala |
| 〃 | `08D330` | 32 | 118149 / 31 | 1:3 2:15 4:13 5:1 | `08C374`×28 `08C37C`×3 `0B9748`×1 | `285EF2` `285EFE` `285F0A` +5 | — | nativo | etiqueta de puntos |
| 〃 | `08C374` | 76 | 118245 / 177 | 1:2 2:36 3:2 4:35 5:1 | `0A4A94`×30 `08C374`×15 `09A67E`×14 `099DCC`×9 `08C172`×2 `0214EA`×1 `027F54`×1 `099E1C`×1 `087E36`×1 `0A2F2E`×1 `099862`×1 | `286216` `286258` `28631E` +15 | 1121 112D 1155 1158 115B | nativo | objeto soltado |
| 〃 | `09C46C` | 11 | 118652 / 312 | 1:1 2:7 4:3 | `09BA2A`×8 `09AE1E`×2 `09AE2C`×1 | `2A0B34` `2A0E78` `2A0EDA` +1 | — | **falta** |  |
| 〃 | `01C71E` | 59 | 119324 / 1028 | 2:14 3:1 4:25 5:19 | `01D764`×35 `02916E`×6 `0A2F2E`×4 `02B202`×3 `04E990`×3 `027F54`×2 `097FE4`×2 `097FCC`×1 `0DC97E`×1 `084588`×1 `04A2C2`×1 | `22CF2C` | — | nativo | efecto |
| 〃 | `01D86C` | 16 | 119579 / 1267 | 2:6 4:9 5:1 | `097FCC`×5 `097FE4`×5 `02916E`×2 `0A2F2E`×2 `027F54`×1 `04A2C2`×1 | `22EAC2` | — | nativo | efecto |
| 〃 | `01D764` | 280 | 120062 / 1520 | 2:9 4:260 5:11 | `0A5D6A`×235 `0A4A94`×16 `04A2C2`×11 `097FCC`×5 `02916E`×4 `097FE4`×3 `08B3E6`×2 `087E36`×2 `027F54`×1 `0DC97E`×1 | — | — | nativo | explosión |
| 〃 | `01C706` | 108 | 120072 / 1520 | 2:22 4:66 5:20 | `01D764`×62 `0DD642`×35 `04E990`×10 `03B936`×1 | `22CE6C` | — | nativo | efecto |
| 〃 | `01D31C` | 181 | 120159 / 1520 | 2:16 3:16 4:149 | `0A4F52`×56 `0A4F3A`×56 `0A2F2E`×32 `027F54`×16 `05ED84`×13 `087E36`×8 | `22DAEE` `22DC2C` | — | nativo | efecto |
| 〃 | `03BC28` | 16 | 120275 / 1520 | 2:3 3:11 4:2 | `04115A`×11 `0AF17E`×5 | `22D2A6` `23F3F0` | 0000 1027 | nativo (`neo_soldier_effects.c`) | efecto diferido del soldado |
| 〃 | `01C6EE` | 49 | 120454 / 1520 | 2:16 4:33 | `01D764` | `22CDD4` | — | nativo | efecto |
| 〃 | `01D944` | 282 | 120463 / 1520 | 2:42 4:76 5:164 | `01D86C` | `22E7F2` | — | nativo | efecto |
| 〃 | `01D958` | 57 | 120488 / 1520 | 2:9 4:21 5:27 | `01D86C` | `22E8AE` | — | nativo | efecto |
| 〃 | `05DD2A` | 6 | 120807 / 1520 | 2:6 | `05D916`×4 `05D226`×1 `05C53C`×1 | `263AD0` | — | **falta** |  |
| 〃 | `038B06` | 4 | 121466 / 2080 | 2:4 | `038D1E`×2 `038D26`×2 | `23C57C` `23C5AA` `23C5D8` | 1029 | **falta** |  |
| 〃 | `038F38` | 14 | 121466 / 2080 | 2:14 | `038D1E`×7 `038D26`×7 | `23C640` `23C64C` | — | **falta** |  |
| 〃 | `0390A6` | 11 | 121589 / 2199 | 2:11 | `038F38` | `22D4E8` `22D552` `22D5BC` +1 | — | **falta** |  |
| 〃 | `0B90FA` | 1 | 121732 / 2331 | 2:1 | `038B06` | — | — | **falta** |  |
| 〃 | `0B90BC` | 1 | 121778 / 2384 | 2:1 | `038B06` | — | — | **falta** |  |
| 〃 | `0B9106` | 1 | 121814 / 2422 | 2:1 | `038B06` | — | — | **falta** |  |
| 〃 | `0B9112` | 1 | 121983 / 2592 | 2:1 | `038B06` | — | — | **falta** |  |
| 〃 | `01CE38` | 25 | 122243 / 2888 | 2:1 4:5 5:19 | `01D764` | `22DC2C` | — | nativo | efecto |
| 〃 | `08C68E` | 5 | 122580 / 2888 | 2:2 4:2 5:1 | `08C374` | `28617E` | — | nativo | destello de objeto |
| 〃 | `08C752` | 1 | 124085 / 3296 | 2:1 | `08C374` | — | — | externo contado |  |
| 〃 | `08C954` | 5 | 124086 / 3296 | 2:5 | `08C752` | `285F92` `285FEC` `285FF6` +2 | — | **falta** |  |
| 〃 | `01C796` | 4 | 124162 / 3296 | 2:4 | `01D764` | `22E48A` | — | nativo |  |
| 〃 | `099E1C` | 3 | 124176 / 3296 | 2:1 4:1 5:1 | corredor×1 `09A692`×1 guion×1 | `29FBEC` `29FE52` `29FEEC` +2 | 1056 113A | nativo | prisionero (de la caja oculta o del guion) |
| 〃 | `01CE44` | 17 | 132256 / 3716 | 4:4 5:13 | `01D764` | `22DC82` | — | nativo |  |
| 〃 | `01CF70` | 1 | 132256 / 3716 | 4:1 | `01D764` | `22DAEE` | — | nativo |  |
| 〃 | `01C8D2` | 235 | 134785 / 3944 | 4:235 | `01D764` | `22E8AE` | — | **falta** |  |
| 〃 | `0AE1DC` | 2 | 140890 / 3944 | 4:2 | `0A4A94`×1 `0AC866`×1 | — | — | nativo |  |
| 〃 | `056E5A` | 7 | 141236 / 4298 | 4:7 | `0569D0`×4 `0569E6`×2 `0569B4`×1 | — | — | **falta** |  |
| 〃 | `056B00` | 4 | 141310 / 4392 | 4:4 | `0569D0`×2 `0569E6`×1 `0569B4`×1 | `24BA1A` | 10FF | **falta** |  |
| 〃 | `056C94` | 4 | 141310 / 4392 | 4:4 | `0569D0`×2 `0569E6`×1 `0569B4`×1 | `24B966` | — | **falta** |  |
| 〃 | `01C950` | 3 | 141338 / 4412 | 4:3 | `056B00` | `22E504` | — | **falta** |  |
| 〃 | `056A6A` | 7 | 141397 / 4460 | 4:7 | `0569D0`×4 `0569E6`×2 `0569B4`×1 | `24B9B8` | — | **falta** |  |
| 〃 | `03B4E0` | 2 | 142068 / 5099 | 4:2 | `093C9A`×1 `0AC866`×1 | `23CE0C` | — | nativo |  |
| 〃 | `03523C` | 2 | 142070 / 5102 | 4:2 | `093C9A`×1 `0AC866`×1 | `23B288` | — | nativo |  |
| 〃 | `01B6A0` | 18 | 142248 / 5377 | 4:18 | `03B4E0` | `22C1B8` | 108C 108D | nativo | bala de vulcan |
| 〃 | `03B936` | 2 | 142321 / 5487 | 4:2 | `03B4E0` | `23D46C` | — | nativo | (familia del Camel Slug) |
| 〃 | `01CE20` | 14 | 143187 / 5980 | 4:1 5:13 | `01D764` | `22DAEE` | — | nativo |  |
| 〃 | `01CE14` | 9 | 143609 / 5980 | 4:1 5:8 | `01D764` | `22DA98` | — | nativo |  |
| 〃 | `01CE08` | 9 | 148593 / 6496 | 5:9 | `01D764` | `22DA42` | — | nativo |  |
| 〃 | `04E558` | 10 | 148757 / 6496 | 5:10 | `01D764` | `22CE6C` | 1022 | **falta** |  |
| 〃 | `04E568` | 32 | 148801 / 6496 | 5:32 | `01D764` | `22CDD4` | 1022 | **falta** |  |
| jugador | `0C4A58` | 2 | 118023 / 0 | 1:1 4:1 | jugador | `0C4904` `0C498C` | — | nativo (`neo_landing.c`) | del jugador (`neo_landing.c`) |
| 〃 | `01B782` | 1064 | 118033 / 0 | 1:16 4:905 5:143 | jugador | `22C12E` `22C81E` `22C884` | 108D 10F7 | nativo | bala de pistola |
| 〃 | `0C5286` | 17 | 118049 / 0 | 1:1 4:13 5:3 | jugador | `0C519E` | — | nativo (`neo_landing.c`) | del jugador (`neo_landing.c`) |
| 〃 | `01BF46` | 339 | 118084 / 0 | 1:11 2:6 4:265 5:57 | jugador | `22C950` `22CB60` | 1027 | nativo | granada |
| 〃 | `06381C` | 50 | 118275 / 231 | 1:25 2:25 | jugador | `2683B6` `2683F2` `26858A` | 1025 10F5 | nativo | cohete R |
| 〃 | `0C58D2` | 463 | 118275 / 231 | 1:25 2:269 3:80 4:89 | jugador | `0C5806` `0C586C` | — | nativo | fogonazo |
| 〃 | `063D9E` | 2688 | 118278 / 237 | 1:198 2:2293 3:170 4:27 | `063AEE`×1504 `063F10`×810 `06381C`×374 | `26863A` | — | nativo | estela |
| 〃 | `06371E` | 226 | 118288 / 252 | 1:9 2:167 3:46 4:4 | `063F10`×121 `063AEE`×79 `06381C`×26 | — | — | nativo | explosión del cohete |
| 〃 | `063AEE` | 153 | 119210 / 959 | 2:153 | jugador | `267C74` `26860A` | 1025 10F5 | **falta** | cohete R con `+93 = 2` (tabla `0C42EA`) |
| 〃 | `01BEDA` | 68 | 119225 / 964 | 2:68 | jugador | `22CA58` `22CB60` | 1027 | **falta** | granada con `+93 = 2` (tabla `0C4762`) |
| 〃 | `0C4E86` | 88 | 119246 / 984 | 2:54 3:33 4:1 | jugador | `0C4E48` | — | **falta** | del jugador con `+93 = 2` (`0C1ED6`) |
| 〃 | `063F10` | 188 | 122619 / 2888 | 2:106 3:80 4:2 | jugador | `2681A4` `268622` | 1025 10F5 | **falta** | arma C con `+93 = 2` (tabla `0C42EA`) |
| 〃 | `01C362` | 36 | 124142 / 3296 | 2:10 3:25 4:1 | jugador | `22CD04` | 116D | **falta** | bomba de la tabla `0C4762` (`+93 = 2`) |
| 〃 | `01C654` | 71 | 124148 / 3296 | 2:12 3:54 4:5 | `01C362` | `22CD5A` | — | **falta** | hijo de `01C362` |
| 〃 | `0DE4DA` | 87 | 141254 / 4326 | 4:87 | jugador | `2B01A6` `2B01B0` `2B01DE` | 1022 1169 116A | **falta** | arma E (`0C42AA`, objeto `4D`) |
| 〃 | `0DE8EE` | 57 | 141262 / 4339 | 4:57 | `0DE4DA` | `22CE6C` | — | **falta** | hijo de `0DE4DA` |
| 〃 | `01B6DC` | 565 | 143245 / 5980 | 4:219 5:346 | jugador | `22C1B6` | 108D 10F4 | nativo | bala H |
| 〃 | `0C5064` | 1 | 146069 / 6496 | 5:1 | jugador | `0C4F58` | — | **falta** | del jugador con la forma `+93 = 2` (`0C359A`) |

### Objetos dinámicos y parches del mapa: quién los pone

Los 19 usos de mapas dinámicos y los 28 parches `0C70FC` de
`filesystem/mission3/s1/dynamic.json` («Reconocimiento del mapa»), con su
dueño resuelto en esta captura (nodo `+28` → última creación del nodo; los
de la raíz `10E968` o de una capa, por la creación del mismo fotograma y la
referencia del mapa en la ROM):

| Mapa / submapa | Fotogramas | Dueño | Prueba | Port |
| --- | --- | --- | --- | --- |
| `0E5640` ×5 (capas) | 117971.. | caja `08C172` (5 en la salida) | ROM `08C1DE`/`08C22C`; `neo_crate.c` | nativo |
| `02C5BA`, `02C5C8` (raíz) | 119051..142717, 142539..142803 | `02B202` | `02C5BA` un fotograma después de la primera creación (119050), como los demás; `02C5C8` 42 fotogramas después de la del corredor (142497); sin referencia larga en la ROM: **sin confirmar** | falta |
| `029C9A/029CDC/029D1E` | 120125..120774 | camión `02916E` | nodo dueño | nativo (el jugador pisa `029CDC`: 61 barridos) |
| `0E112E` ×2 | 122233..123767 | `04E990` | nodo dueño; ROM `04EEDC` | falta |
| `0E53D4`, `0E53C2` | 124035..124161 | `08AFB6` | nodo dueño | falta |
| `0E6896` (raíz) | 126245..141259 | plataforma `0B9286` (llamada `0B925C`) | creada en 126244; ROM `0B9376/0B93A8` | **falta: el jugador está encima 2363 barridos (sección 4)** |
| `0AAC94` (`10C72E`) | 142247..142320 | `0AC866` (sus hijos `0ADEDA/0AD2AC` se crean el fotograma anterior, 142246) | creación; sin referencia larga: **sin confirmar** | falta |
| `0E4FF2` | 142332..142356 | blindado `087E36` | `neo_armored.c` | nativo |
| `0E0D00` (raíz) | 144486..148578 | jefe `04A2C2` | ROM `04E06A` | falta (el jugador está encima 98 barridos) |
| `0E0E36` | 146240..147948 | `04C38A` (pieza del jefe) | nodo dueño; ROM `04C6F4` | falta |
| parches `0B85EA` ×5 | 121043.., 121321.. | `0B911E`, `0B9140` (cajas `039382/03938A`) | nodo del parche | `work-m3c` |
| parches `0B85EA` ×18 | 121732..122007 | `0B90FA`, `0B90BC`, `0B9106`, `0B9112` (de `038B06`, de `038D1E/038D26`) | nodo del parche | falta |

`0A6C8C` no es un constructor: es una subrutina de la familia `0A4A94`
(`0A6C8C`: `0C6F34` con el mapa `2A6142`, `0C73CE`, `0C6CFC` y `01AB98`
sobre `10E8C6`): la superposición de la aeronave. Lo mismo `0B8FA8`: el
estado de los estampadores `0B911E/0B9140/0B90BC..0B9112`.

### Solo en ramas que la captura no tomó

72 registros op 7 no se ejecutaron (lista en `data/mission3_actors.json`):
alternativas al azar no elegidas (`08C172` ×4, `08C37C` ×2, `09A67E` ×11,
`099DCC` ×5, `099E1C` ×2, `0214EA`, `084588`, `0AF17E` ×6), `if` de grupo o
de posición del jugador no cumplidos (`097FCC` ×15 de sus 20 registros, variantes por
posición del jugador, `0A2F2E` ×3, `05C53C` ×2, `087E36`, `08A732`, `099862` ×2,
`0DC97E`, `08C37C`, `0AF17E`) y corredores no lanzados (`02B202` ×3,
`09BA2A`, `0AF17E` ×6, `05D234`). **Ningún constructor queda solo en ramas
no vistas salvo `05D234`** (registro `2DA828`, corredor del bloque
`2DA814`): sin captura, no se sabe qué crea. Los hijos de una familia sí
pueden depender de la rama (p. ej. los regalos de los registros no
ejecutados, abajo).

## 3. Premios de la Misión 3

Cada objeto (`08C374` soltado / `08C37C` del guion) con su tipo `+6E` en el
constructor (`08C380`) y su creador real; registro `0E581A + 0x18·tipo`
(`+10` recogida, `+14` movimiento) leído de la ROM; «Port» según
`neo_item.c` (movimientos `08B9B2 08BA10 08BA38 08BAB2 08BCCC 08BD32
08BD5A`; recogidas de arma `08B568..08B5B6`, bombas `08B6D0`, gasolina
`08B710`, medicina `08B750`, puntos/comida `08B7FC`). 88 objetos:

| Tipo | Movimiento | Recogida | Creados s1/s2/s3/s4/s5 | Fuente (creador) | Port |
| --- | --- | --- | --- | --- | --- |
| `00` | `08B9B2` | arma 5 (H) | 0/1/0/3/0 | `027F54` `087E36` `099DCC` `0A2F2E` | nativo |
| `06` | `08B9B2` | bombas | 0/0/0/1/1 | `099DCC` `099862` | nativo |
| `08` | `08B9B2` | `08B5F4`: bomba tipo 1 (`0C0A7C`, `D1 = 1`) | 0/0/1/0/0 | `099DCC` | **recogida sin traducir** |
| `09` | `08B9B2` | `08B5FA`: bomba tipo 2 | 1/0/0/0/0 | `099DCC` | parada en la recogida (como en la M2) |
| `11` | `08B9B2` | puntos/comida | 1/0/0/0/0 | guion | nativo |
| `18` | `08B9B2` | puntos/comida | 1/0/0/0/0 | guion | nativo |
| `19` | `08B9B2` | puntos/comida | 0/1/0/0/0 | `099DCC` | nativo |
| `1A` | `08BA10` | puntos/comida | 0/1/0/0/0 | `099DCC` | nativo |
| `1E` | `08B9B2` | puntos/comida | 2/4/0/0/0 | guion | nativo |
| `1F` | `08B9B2` | puntos/comida | 0/4/0/0/0 | `08C172` `09A67E` | nativo |
| `26` | `08B9B2` | puntos/comida | 0/1/0/0/0 | `099E1C` | nativo |
| `28` | `08BD5A` | puntos/comida | 0/4/0/0/0 | guion | nativo |
| `35` | `08BAB2` | puntos/comida | 0/5/0/0/0 | `09A67E` | nativo |
| `36` | `08B9B2` | puntos/comida | 0/15/0/0/0 | `08C374` (cada `35` suelta tres) | nativo |
| `3B` | `08BAB2` | puntos/comida | 0/5/0/0/0 | `09A67E` | nativo |
| `43` | `08BAB2` | puntos/comida | 1/0/0/0/0 | `0214EA` | nativo (la fuente falta) |
| `4B` | `08B9B2` | arma C | 0/1/0/0/0 | `099DCC` | nativo |
| `4D` | `08B9B2` | arma E | 0/0/0/1/0 | `08C172` | nativo el objeto; **el arma E (`0DE4DA`) falta** |
| `4E` | `08B9B2` | `08B60C`: bomba tipo 3 (`0C0A7C`, `D1 = 3`) | 0/1/0/0/0 | `09A67E` | **recogida sin traducir** |
| `4F` | `08B9B2` | arma A (H) | 0/0/1/0/0 | `099DCC` | nativo |
| `50` | `08B9B2` | arma 7 | 0/1/0/0/0 | `099DCC` | nativo |
| `55` | `08BCCC` | puntos/comida | 0/0/0/30/0 | `0A4A94` (aeronave) | nativo el objeto; la fuente falta |

Fuentes del guion (palabra del tipo deducida de la captura: la única palabra
del registro igual al tipo soltado en todas las observaciones; los
registros no ejecutados cuentan):

| Fuente | Palabra | Tipos en sus registros (veces) | Sin traducir |
| --- | --- | --- | --- |
| `08C37C` | 0 | `11`×1 `16`×1 `18`×1 `1E`×6 `1F`×1 `24`×1 `28`×4 | — |
| `08C172` | 0 | `0F` `11` `1A` `1F` `38` `39` `3A` `3B` `4D` (×1 cada uno), `FFFF`×2 | — (`4D`: el arma E) |
| `099DCC` | 3 | `00`×1 `06`×1 `08`×1 `09`×2 `15`×1 `19`×1 `1A`×2 `23`×1 `33`×1 `35`×1 `4B`×2 `4F`×2 `50`×1 `51`×1 | `08` (`08B5F4`), `09` (`08B5FA`), `33` (`08B78E`: sonido 1121 y `0C0C46`; no observado) |
| `099E1C` | 3 | `00`×2 `06`×1 `26`×2 | — |
| `09A67E` | 3 | `15`×2 `1F`×1 `24`×1 `35`×2 `38`×2 `39`×1 `3A`×2 `3B`×2 `3C`×1 `4E`×1 `FFFF`×1 | `4E` (`08B60C`) |
| `0214EA` | 1 | `43`×1 `56`×1 | — |

Sin palabra única (una observación, varias palabras iguales al tipo):
`027F54` (`00`), `087E36` (`00`), `0A2F2E` (`00`); `099862` y `0A4A94` no
llevan el tipo en el registro (`06`; `55` ×30 de la aeronave).

## 4. Plan por prioridades

Lo que bloquea el recorrido primero (el cartucho de referencia de
`MISION3.md` para en la cámara 1901), después lo que vacía la misión.

1. **Suelos que ponen los actores** (sin ellos el jugador no pasa):
   - Fusionar `work-m3c` (cajas `039382/03938A` de las llamadas `0B918C`/
     `0B91AE`, estampadores `0B911E/0B9140` → repisa `0B8FA8`; quitar su
     `origin_rows`, la rejilla ya empieza en la fila 0). Repisas de x
     1864..1903 y 2120..2159, cámara 1552/1808.
   - `038D1E`/`038D26` (llamadas `0B9148`/`0B916A`, cámara 2080/2304) →
     `038B06` (sonido 1029, programas `23C57C..23C5D8`) → estampadores
     `0B90FA/0B90BC/0B9106/0B9112` (los otros 18 parches `0B85EA`, mismo
     `0B8FA8`), y `038F38` → `0390A6`.
   - **Plataforma `0B9286`** (llamada `0B925C`, cámara 3536): mapa dinámico
     `0E6896` de 126245 a 141259. La sección 3 es el primer plano `0B8600`
     sin mosaicos ni colisión y la lista de cámara espera al jugador en un
     rectángulo (`0C84BE`, 126224): sin la plataforma no se cruza el hueco,
     y en la sección 4 el jugador la pisa en 2363 barridos (toda la pelea
     con la aeronave). Hijos `0B9748` (sonido 1121) y `0B93B4`.
   - Las llamadas de la lista de cámara que las crean (`0B8AC4`, `0B8B08`,
     `0B8B28`, `0B8B4A`, `0B8BF0`) y la espera del rectángulo.
2. **El tren y la lista de cámara**: `0B8EDC` (con el conductor ya nativo;
   probable escritor del cielo, sin comprobar), las llamadas `0B8EBC(±n)` y
   los modos automáticos 0x200/0 (122145, 141260), `0B91FA` (3296).
3. **Enemigos del camino**, por apariciones y por esperas del guion que
   retienen (contados; en el port se sueltan y la cámara no espera):
   `09BA2A` (8, grupo 0) con `09AE2C/09AE1E` (hijo común `09C46C`);
   `08A732` (5) y `08DA12` (5); `05D916` (4, grupo 1: la espera
   `groups0..7 < 1` de 365 fotogramas en 1520); `04E990` (3, grupo 0: el
   cierre de 1604 fotogramas en 2888, con `0E112E`); `097FCC` (5 de 20
   registros, tres esperas `group0 == 0` en 5980); `0569B4/0569D0/0569E6`
   (7); `027F54` (grupo 7: 184 fotogramas en 1520); `0A2F2E` + `08AFB6`
   (grupo 7: 650 fotogramas en 3296); `02B202` (4); `0391EA` (2);
   `05C53C`, `05D226`, `0AF1B6`, `084588`, `0B9400`, `01F1D4`, `0214EA`,
   `03EE8A` (8 en la salida). Muchos comparten programas de soldado
   (`2404FE`, `240DEE`, `23D510`): empezar por el código común con
   `0AF17E`/`04115A`.
4. **Jefe intermedio `0A4A94`** (aeronave «96», 8615 fotogramas, 15
   constructores: `0A606E/0A6078/0A61E8/0A61F2`, `0A64E2` ×286, `0A6598`
   ×56, `0A4DB6`→`0A4F3A/0A4F52`→`0A5398`, `0A5D6A`, `0A5E88`, `0A4DB0`,
   `08D0D2`; la pantalla blanca `0AE1DC` ya es nativa) y su superposición `10E8C6` (`0A6C8C`).
   Estados, ataques, vida y muerte: [JEFE_MISION3.md](JEFE_MISION3.md).
5. **Vehículos**: `093C9A` (oruga, torreta y marcador del SV-001 ya
   nativos: probablemente una entrada nueva de `neo_tank.c`) y `0AC866`
   (14 constructores, 10 sin traducir; programas en P1 `0AB028..0AD738`, mapa
   `0AAC94`).
6. **Jefe `04A2C2`** (23 constructores, 21 sin traducir, mapa dinámico `0E0D00` que el
   jugador pisa, `0E0E36`): **sin él no hay fin de misión** (`018F92` lo
   llama su nodo). Música `059F76` ya nativa.
   Piezas, estados, ataques, vida, muerte, plan por piezas y tramos de
   captura (`tools/capture_m3_boss.sh`): [JEFE_MISION3.md](JEFE_MISION3.md).
7. **Jugador**: la forma `+93 = 2` (tabla de disparo `0C42EA`: cohete
   `063AEE` ×153, C `063F10` ×188; granadas de la tabla `0C4762`: `01BEDA`
   ×68, `01C362` ×36 → `01C654`; `0C4E86` ×88 de `0C1ED6`; `0C5064`) es lo
   más frecuente que falta (605 creaciones): identificar qué la pone (sin
   comprobar en esta captura); el arma E `0DE4DA` (objeto `4D`) y su hijo
   `0DE8EE`; recogidas `08B5F4`/`08B60C` (bombas tipo 1/3).
8. **Efectos comunes que faltan**: `01C8D2` (×235, de la explosión
   `01D764`), `04E558/04E568` (de `01D764` en el jefe), `056xxx` (hijos de
   los `0569xx`), `05DD2A`, `09C46C`, `01C950`, `08C954` (de la etiqueta
   `08C752`).

## Límites

- Una sola partida (la de `run4`): las ramas al azar y los `if` de posición
  dan otros registros en otra partida (72 no ejecutados, arriba); `05D234`
  no se ha visto nunca.
- Programas: solo los que pasan por `004256` (un programa que salta a otra
  raíz o un `+20` escrito a mano no se ven). Sonidos: solo `0120F8`.
- Las descripciones «sin identificar» no se han traducido ni mirado más
  allá de la captura; los nombres de las pantallas (aeronave «96», robot
  andante, «IN») son lo que se ve, no nombres del juego.
- Cadena del port: `compare_m3_events.py --port` compara con un registro de
  ares (líneas `MSX script spawn record ...`); no se ha corrido en esta rama
  (sin cambios de código del juego).
