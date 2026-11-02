# Marco en la campaña: programas originales de cuerpo y arma

La ROM jugable deja de usar el selector histórico de seis estados (tres bancos
de poses capturadas) para dibujar a Marco. Ahora ejecuta los **programas de
animación originales** del cuerpo y del arma (el grafo convertido de
[MARCO_CONTROLLER.md](MARCO_CONTROLLER.md)) con el motor nativo
(`marco_animation_step`, una llamada por actualización lógica, cada dos
fotogramas de vídeo como el original) y dibuja sus emisiones con
`marco_renderer_draw_native` (piezas CI4 ancladas, catálogo auditado).
Los seis estados antiguos quedan solo como reserva si falta `romfs/marco`.

## Qué decide el port y qué es original

| Pieza | Origen |
| --- | --- |
| Comandos de animación (poses, esperas, campos +72/+74/+76..+79, saltos, fin) | originales, convertidos (`convert_marco_programs.py`) |
| Programa elegido en cada situación | uno que el código original de selección (`0x0C0F32`/`0x0C0F62`) eligió en la captura con la misma condición de mando; `tools/compile_marco_campaign.py` lo comprueba contra las 376 selecciones grabadas y falla si no aparece |
| Situación (suelo/aire, carrera, agachado, arriba, disparo, salto con carrera) | estado de juego del port (`gameplay.c`), **no** el controlador original `0x0C1xxx..0x0C3xxx`, cuyos servicios de mundo siguen sin conectar |
| Física, disparos e impactos | sin cambios (los de antes), salvo el agachado |

Funciones (ROM, parte, condición de la captura que lo prueba):

| Papel | ROM | Situación en el port |
| --- | --- | --- |
| reposo | `0x24C5EA` cuerpo, `0x24C60E` arma | en el suelo, quieto |
| carrera | `0x24DD18` cuerpo, `0x24E2C8` arma | en el suelo, moviéndose |
| disparo | `0x24C70A` arma | cada bala de pie, corriendo o en el aire |
| apuntar arriba / disparo arriba | `0x24C6AA` / `0x24C7F2` arma | ARRIBA (sin / con bala) |
| agacharse, agachado, disparo agachado | `0x24ECD6`, `0x24ECF6`, `0x24EE42` cuerpo (sin hilo de arma, como en la captura) | ABAJO en el suelo |
| salto parado / con carrera | `0x24E4E0` + `0x24E5BC`, `0x24E858` + `0x24E974` | despegue quieto / moviéndose |

Agachado: el jugador del port no se desplaza agachado (el original repta, no
portado) pero puede girarse; las balas y la caja de impacto siguen siendo las
de pie (no recuperadas para el port).

Un programa que termina (`FINISH`) deja su última pose hasta que la
situación elige otro, y una orden sin recuperar (`UNRECOVERED`, dos en el
grafo, ambas en programas que la campaña no usa) detiene el programa: la
pose se descarta, se cuenta y se registra (`MSX marco program stopped`). Los
eventos medidos (`EVENT`, peticiones de efecto como el fogonazo) siguen
adelante sin crear el efecto.

## Datos (privados, generados en local)

Capturas MAME 0.264 (`tools/mame/run_marco_catalog.py`, misma lógica que las de
0.276: 2948 fotogramas, 172 descriptores y 1.972.752 píxeles iguales,
idénticos a los documentados) en `captures-m1full/marco-cat1` (29 condiciones)
y `marco-cat2` (26 condiciones de un solo disparo y reposo: el final del
disparo agachado y el arranque de la carrera; 1108 fotogramas, 140
descriptores, 691.096 píxeles iguales). Catálogo de poses unido:
**209 poses, 221 piezas CI4, 142.784 bytes** (`build_marco_poses.py
--extra-audit`). El descriptor `0x188930` (tercera pose del arranque de la
carrera, un solo paso) no llegó a la pantalla en ninguna captura: el render
mantiene la pose anterior en ese paso y lo registra.

```sh
python3 tools/mame/run_marco_catalog.py --rom-dir ROMS --output CAP1 --command-trace --paired-trace --controller-trace
python3 tools/audit_marco_catalog.py CAP1 --rom mslugx.zip --output CAP1/audit.json
python3 tools/extract_marco_steps.py CAP1 --trace CAP1 --output CAP1/diary
python3 tools/convert_marco_programs.py CAP1 --event-evidence CAP1/diary/effects.json --output CAP1/graph
python3 tools/extract_marco_controller.py CAP1 --output CAP1/controller.json
# CAP2: el mismo runner con --plan (condiciones de un disparo), y su auditoría
cd tools && python3 build_marco_poses.py CAP1 --extra-audit CAP2/audit.json --rom mslugx.zip --mksprite $N64_INST/bin/mksprite && cd ..
python3 tools/compile_marco_campaign.py CAP1 --output build/marco_campaign.h
make check-marco-campaign && make
```

`make check-marco-campaign` recorre doce situaciones (reposo, carrera,
disparo, arriba, disparo arriba, agachado, disparo agachado, saltos, saltos
con disparo, carrera a la izquierda): ninguna se queda sin dibujo ni detiene
un programa, y todos los descriptores salvo `0x188930` están en el catálogo.

## Forma 1 (H) y muerte

Con la H el original pone `+93 = 1` y la selección `0C0F62`/`0C0F32` toma la
fila `+0x10` de cada tabla. Captura MAME 0.264 `marco-h2`
(`tools/mame/plans/marco-hgun.json`, `run_marco_catalog.py --give-hgun`: la
H se escribe en RAM en el primer fotograma del plan como en el plan `hold` de
`capture_weapons.lua`, solo captura; 22 condiciones, 920 fotogramas) y
`marco-h3` (`marco-hgun-hold.json`: agachado y arriba mantenidos 112
fotogramas). Auditoría (`audit_marco_catalog.py --sparse`): **916 y 324
fotogramas, 105 y 19 descriptores, 786.992 y 241.010 píxeles iguales**.
Papeles de forma 1 (cada uno comprobado contra las selecciones grabadas,
`compile_marco_campaign.py --hgun-capture`):

| Papel | Tabla + 0x10 | ROM |
| --- | --- | --- |
| reposo / carrera / salto / salto con carrera (arma) | `0BF858` / `0BF8D8` / `0BFC58` / `0BFC98` | `253402` / `255ABE` / `255CD6` / `255E96` |
| subir, mantener, bajar la H | `0BF9D8` / `0BFA18` / `0BFA98` | `25349E` / `253450` / `2534B4` |
| ráfaga, ráfaga arriba | `0BFDD8` / `0BFE18` | `2534FE` / `253662` |
| barrido adelante→arriba, arriba→adelante | `0BFE98` / `0BFF18` | `2537CE` / `253840` |
| agacharse, agachado, ráfaga agachado (cuerpo) | `0BEED8` / `0BEF18` / `0BFF98` | `256BE6` / `256C06` / `256D52` |

Cuerpo de pie, carrera y salto: la tabla no tiene fila 1 (fallback
`0C0F4C`, los mismos programas). La ráfaga o el barrido los arranca el
estado de disparo de la H (`marco_hgun.c`) en la actualización en que
empieza su programa; agachado no hay hilo de arma (el cuerpo `256D52` lleva
los `0C4372`). Los programas hacia abajo (`2560C0`, `2561DC`, `25624E`)
necesitan apuntar abajo en el aire: el jugador ya apunta abajo
(ARMAS_VEHICULOS.md), pero estos programas no están en el catálogo y se
dibuja el de reposo.
Llamadas: los sitios de forma 1 medidos en `marco-h2/h3` y los de la misma
rutina y carga útil que un sitio medido (`convert_marco_programs.py
--extra-capture --equivalent-calls`) son `EVENT`; queda un `UNRECOVERED`
(`24EDBA`, reposo largo agachado de la forma 0, no usado).

Muertes (`marco_death.c`): captura `marco-d2` (`capture_weapons.lua` con
`MSX_WEAPONS_MARCO_DUMP=c2956-c2f05`, el entorno de `death3`: VRAM solo
mientras muere). Auditoría dispersa: 540 fotogramas, 102 descriptores,
477.099 píxeles; se excluyen, listadas en el informe, las dos muertes de tipo
9 (el sprite aparece 32 píxeles por encima de la `y` enviada: sin explicar) y
cuatro volcados de eventos. `0BF318` (`25016A`, en el aire) y `0BF358`
(`2501DC`, tipos 7/0x10, con su paleta quemada, perfil `BURNT`) entran en el
catálogo; `0BF298` y la entrada `0BF498` ya estaban. `0BF3D8` (`2503D4`,
tipo 2 en el suelo) no apareció en ninguna captura: sus poses no están y se
mantiene la anterior (registrado). Catálogo: **329 poses, 349 piezas CI4,
267.144 bytes, 5 perfiles de paleta** (`build_marco_poses.py
--extra-profile-min-uses 40 --name-palette BURNT=...`).

```sh
python3 tools/mame/run_marco_catalog.py --rom-dir ROMS --output H2 --command-trace --paired-trace --controller-trace --plan tools/mame/plans/marco-hgun.json --give-hgun
python3 tools/mame/run_marco_catalog.py ... --output H3 --plan tools/mame/plans/marco-hgun-hold.json --give-hgun
MSX_WEAPONS_MORTAL_CAMERA=3300 MSX_WEAPONS_ASSIST_CAMERA=3300 MSX_WEAPONS_LAST=16700 MSX_WEAPONS_FIRST_DUMP=99999 MSX_WEAPONS_MARCO_DUMP=c2956-c2f05 tools/run_capture_weapons.sh D2
# D2: plan.json/capture.json de un caso (2100..16700), luego
cd tools; python3 audit_marco_catalog.py H2 --rom mslugx.zip --output H2/audit.json --sparse   # y H3
python3 audit_marco_catalog.py D2 --rom mslugx.zip --output D2/audit.json --sparse --exclude 12987-12987 --exclude 15514-15700 --exclude 16086-16086 --exclude 16146-16146 --exclude 16207-16460
python3 extract_marco_steps.py H2 --trace H2 --output H2/diary; python3 extract_marco_controller.py H2 --output H2/controller.json   # y H3
python3 convert_marco_programs.py CAP1 --event-evidence CAP1/diary/effects.json --extra-capture H2 --extra-capture H3 --equivalent-calls --output GRAFO
python3 build_marco_poses.py CAP1 --extra-audit CAP2/audit.json --extra-audit H2/audit.json --extra-audit H3/audit.json --extra-audit D2/audit.json --extra-profile-min-uses 40 --name-palette BURNT=<colores 1..3 del perfil de 2501DC> --rom mslugx.zip --mksprite $N64_INST/bin/mksprite
cd ..; python3 tools/compile_marco_campaign.py CAP1 --graph GRAFO --hgun-capture H2 --output build/marco_campaign.h
make check-marco-campaign
```

`make check-marco-campaign` añade 14 situaciones con la H (ráfagas y barridos
arrancados por `marco_hgun.c`) y las muertes/entrada: ningún programa se
detiene y todo dibujo salvo `188930` está en el catálogo.
`tools/check_marco_dumps.py` identifica los sprites del jugador en los
volcados de `hfam`, `hold2` y `death3` por sus códigos de tile columna a
columna (también reflejados): 71 volcados, todos los descriptores del jugador
están en el catálogo; en 5 quedan 1–3 columnas de una fila de otros actores
con la paleta del jugador (partículas).

## Granadas, arma C y la H hacia abajo

Informe de MiSTer: «hay una animación del brazo de Marco cuando lanza
granadas que no está implementada». El lanzamiento (`0C3BCC`, tabla
`0BF998`; agachado `0C2458`, tablas `0BF098` y, al acabar, `0C2294` →
`0BEF98`) no tenía papel en la campaña y se dibujaba el reposo. Tampoco
estaban los programas de disparo de la C ni los de la H hacia abajo en el
aire.

Capturas MAME 0.264 (privadas, `/home/user/captures-grenade`):
`marco-g1` (`tools/mame/plans/marco-hgun-grenade.json`, `--give-hgun
--keep-bombs`: granadas de pie, corriendo, en el aire y agachado con la H,
saltos con abajo y disparo; 768 fotogramas) y `marco-c1`
(`marco-cgun.json`, `--give-cgun --keep-bombs`: la C dada como en el plan
`air` de `capture_weapons.lua`, disparos en todas las punterías y
granadas; 1224 fotogramas). `--give-cgun`/`--keep-bombs` son escrituras
solo de captura (`10C71E/10C724/10C71C/10C71A/+94`, bombas `10C720` = 10).
Auditoría (`audit_marco_catalog.py`): **764 y 1208 fotogramas, 79 y 109
descriptores, 596.066 y 904.263 píxeles iguales**; en `marco-c1` se
excluyen 2536 y 2876..2886 (el destello `0C58D2` de la C sobre Marco).
Grafo convertido con `--extra-capture` de ambas (559 programas, el mismo
`UNRECOVERED` `24EDBA`). Catálogo: **376 poses (las 329 anteriores
idénticas), 396 piezas CI4, 305.656 bytes**.

| Papel | Tabla | Forma 0 (pistola) | Forma 1 (H y C) |
| --- | --- | --- | --- |
| lanzar de pie, corriendo o en el aire (arma) | `0BF998` | `24C862` | `253B3A` |
| lanzar agachado (cuerpo) | `0BF098` | `24EF98` | `25707C` |
| agachado tras lanzar (cuerpo, en bucle) | `0BEF98` | `24ED44` | `256C54` |
| H: ráfaga abajo / barrido adelante→abajo | `0BFE58` / `0BFED8` | — | `2560C0` / `2561DC` |
| C: disparo, arriba, abajo (arma) | `C02D8` / `C0318` / `C0358` | — | `253A56` / `253AC6` / `255FDE` |
| C: disparo agachado (cuerpo) | `C0398` | — | `256EB6` |

`marco_visual.c`: la granada de esta actualización (`gameplay.c`, la que
`0C47BA` lanza) arranca el programa de arma (o de cuerpo agachado) con
prioridad sobre el disparo, como `0C3A5E` (bit 6 antes que el 4); el estado
de disparo de la C (`0C3CD0`, contador `c_fires`, aunque `0C0D64` rechace
el obús) arranca el suyo según la puntería; la H toma `2560C0`/`2561DC`
de `marco_hgun.c` (programas 2 y 4), y en el aire con abajo un programa
hacia abajo terminado mantiene su última pose. Ningún programa de lanzar
escribe `+72/+74` (los de agachado tampoco): `marco_legs.c` no cambia.

**Paradas explícitas**: el barrido abajo→adelante `25624E` no apareció en
ninguna captura (no está en el catálogo, no se arranca); bajar la H en el
aire sin disparar (`0BFB98` `255F8E`) solo se eligió en la misma
actualización que una ráfaga (nunca dibujado, una pose sin capturar): se
mantiene el programa de salto; el asentamiento con puntería abajo
(`0C38B2`, tabla `38DA`) no se capturó. La pose `189FE0` de `2560C0` no se
vio en ninguna captura.

**Prueba** (`make check-marco-campaign`): `tools/extract_throw_fixture.py`
→ `data/marco_throw_fixture.json`: 15 casos (pistola, H y C; de pie,
corriendo, en el aire y agachado; disparos de la C; H hacia abajo), desde
la actualización en que el original eligió el programa, la pose que dibujó
esa parte en cada paso: **239 pasos iguales, 0 distintos** con
`marco_visual_step`.

```sh
python3 tools/mame/run_marco_catalog.py --rom-dir ROMS --output G1 --command-trace --paired-trace --controller-trace --plan tools/mame/plans/marco-hgun-grenade.json --give-hgun --keep-bombs
python3 tools/mame/run_marco_catalog.py --rom-dir ROMS --output C1 --command-trace --paired-trace --controller-trace --plan tools/mame/plans/marco-cgun.json --give-cgun --keep-bombs
cd tools; python3 extract_marco_controller.py G1 --output G1/controller.json; python3 extract_marco_steps.py G1 --trace G1 --output G1/diary   # y C1
python3 audit_marco_catalog.py G1 --rom mslugx.zip --output G1/audit.json
python3 audit_marco_catalog.py C1 --rom mslugx.zip --output C1/audit.json --exclude 2536-2536 --exclude 2876-2886
python3 convert_marco_programs.py CAP1 --event-evidence CAP1/diary/effects.json --extra-capture H2 --extra-capture H3 --extra-capture G1 --extra-capture C1 --equivalent-calls --output GRAFO
python3 build_marco_poses.py CAP1 --extra-audit CAP2/audit.json --extra-audit H2/audit.json --extra-audit H3/audit.json --extra-audit D2/audit.json --extra-audit G1/audit.json --extra-audit C1/audit.json --extra-profile-min-uses 40 --name-palette BURNT=7FFF,6FF0,4840 --rom mslugx.zip --mksprite $N64_INST/bin/mksprite
cd ..; python3 tools/compile_marco_campaign.py CAP1 --graph GRAFO --hgun-capture H2 --grenade-capture G1 --cgun-capture C1 --output build/marco_campaign.h
make check-marco-campaign
```

## Cuchillo (rama `work-rom12`)

Un disparo con `+9A` bit 0 (un *hurtbox* de tipo 4/5 al alcance) corta
(`0C3B46`): de pie el arma toma la tabla `0BF958` (pistola `24D486`, forma 1
`255472`), agachado el cuerpo la tabla `0BF118` (pistola `2505A6`, forma 1
`258088`); al acabar vuelven el programa de reposo y el agachado.
`marco_visual.c` los arranca con `knife` (los cortes de `gameplay.c`).

Capturas `marco-k1` (pistola) y `marco-k2` (`--give-hgun`) con
`tools/mame/plans/marco-knife.json`, generado por
`tools/mame/plan_knife_cuts.py`: cada corte (de pie y agachado alternos) se
coloca en el primer fotograma posterior con `+9A` bit 0 cuyo prefijo de
entradas no cambia, y se comprueba en la captura siguiente (4 cortes en cada
una). La otra tabla `0BF918` (`+9A` bit 1) no apareció: parada explícita.
Catálogo: las capturas H `marco-h2/h3` siguen en disco (no hizo falta
recapturarlas); 376 → 402 poses, las 376 anteriores idénticas (CRC, límites,
ancla y partes); 4 programas nuevos en el grafo. `make check-marco-campaign`:
**293 pasos iguales, 0 distintos** (20 casos; 54 de los 5 cortes).

```sh
python3 tools/mame/plan_knife_cuts.py K1 p 8 3700; python3 tools/mame/plan_knife_cuts.py K2 h 8 3700
cd tools; python3 extract_marco_steps.py K1 --trace K1 --output K1/diary; python3 audit_marco_catalog.py K1 --rom mslugx.zip --output K1/audit.json   # y K2
# convert_marco_programs.py / build_marco_poses.py: las órdenes de arriba con --extra-capture K1 --extra-capture K2 / --extra-audit K1/audit.json --extra-audit K2/audit.json
cd ..; python3 tools/compile_marco_campaign.py CAP1 --graph GRAFO --hgun-capture H2 --grenade-capture G1 --cgun-capture C1 --knife-capture K1 --knife-h-capture K2 --output build/marco_campaign.h
python3 tools/extract_throw_fixture.py --output /tmp/k.json p_knife_stand=K1:k_cut_stand_0:1:24d486:0:stand ...   # añadidos a data/marco_throw_fixture.json
```

## Ciclo de carrera

El original arranca la carrera (`C285C`, traducido en `marco_motion.c`) con
la velocidad de cuerpo `+26 = 0x1C1`: en `marco-cat1` todas las
actualizaciones de `24DD18` la tienen. El port la arrancaba a 0x100 (un
ciclo 1,76 veces más lento). `marco_visual.c` la usa ahora para `B_RUN`, y
`src/marco_legs.c` lleva con ella los `+72/+74` del programa (y los del salto
y del salto con carrera, que esperan arriba mientras `+78 = FF`, es decir
mientras sube). Pruebas: `tools/test_marco_legs.c` (`make check-core`,
`data/legs_fixture.json` de `tools/extract_legs_fixture.py` sobre
`marco-cat1`): 194 pasos de cuerpo con `+72/+74` iguales en la carrera (a
derecha e izquierda), el salto y el salto con carrera hasta el aterrizaje;
`make check-marco-campaign`: la pose de cada actualización de `walk_right` y
`run_cycle_right` (68) igual a la del original. La pose `188930` del
arranque dura menos de una actualización a 0x1C1: tampoco se dibuja en el
original.

## Forma 3: la momia (rama `work-m2mummy`)

Informe de MiSTer: «cuando una momia te echa el aliento, Marco debe
convertirse en momia». En el port el contacto mataba a Marco.

- **La momia y su aliento ya son nativos.** Son la familia `0613CA/0613D8/
  0613E6` de `neo_rebel2.c` (documentada como «rebeldes» en
  MISION2_ACTORES.md): su «disparo» `061E8C` golpea con la fila `E1252`
  (clase 0, tipo `0x10`). El tipo de la fila es el `+5A` que `004C24`
  devuelve al jugador (`04DAA`), y `0C36C0` compara ese valor con `0x10`.
  También dan `0x10` las filas `035AB8/035AC8` de la bomba `0369AC` del dron.
  Los candidatos `035B66/035C68/035D12` son otra familia (voladores y
  saltadores con `+96` = 0..3, filas de tipo 7): no transforman.
- **Contacto** (`gameplay.c`, `msx_game_hurt`): tipo `0x10` sin forma 3 →
  `+94 = 3`, arma a pistola (`marco_weapons_pistol`), la H se reinicia; en
  forma 3 → la muerte `0C2956`. Mientras cuenta `+84` (la transformación y
  después `0x5A` actualizaciones) `+5F` bit 6 descarta todo contacto.
- **Transformación** medida en MAME (`run_marco_catalog.py --give-mummy`,
  nuevo: escribe `+94 = 3` en el primer fotograma del plan, solo captura;
  captura lógica `marco-mz1`, `--logic-only --controller-trace`):
  `0C17A0` en la actualización siguiente, `0C17F0` espera 17
  actualizaciones al efecto `C4DD8`, `0C182A` pone `+84 = 0x5A` y `0C18B4`
  sigue una actualización después: 19 actualizaciones sin control ni
  movimiento.
- **Mando de la momia** (`0C18B4`): andar a `+62 = 0x18000` (la mitad de
  `0x30000`; `neo_walk_step_shift`); salto `0C1A0C` (tres actualizaciones
  agachado) y `0C1A62` con `+34 = 0x90000` y `+34 += (−14 px − +34) >> 5`
  (dos pasos medidos exactos); en el aire, el avance horizontal a la mitad
  es **aproximación declarada** (la captura saltó en vertical). Dispara la
  pistola (`0C3CE8` solo salta el cuchillo). Ya estaban: la concesión
  `0C0928` rechaza armas, `knife_ok` excluye la forma 3 y `neo_vslot.c`
  no deja subir a un vehículo (`0C06E8`).
- **Cura**: el objeto de tipo `0x32` (registro `E5CCA`, recogida
  `08B750`): sonido `1121` y `0C0C92`: con el jugador vivo en forma 3,
  `+94 = 0`, `08B87A` y la etiqueta `08CD10` (externa, contada en
  `labels`, y `cures`); si no, el valor `08B896` y `08B850`. No hay cura por
  tiempo en el código de formas (`0C32E4`).
- **Poses** (captura completa `marco-mz2`: `run_marco_catalog.py
  --command-trace --paired-trace --controller-trace --give-mummy`, plan por
  defecto, 2948 fotogramas auditados, 92 descriptores, 1765484 píxeles
  iguales). Filas `+0x30` de las tablas: cuerpo de la transformación
  `0BF258` `24EDE0` (los cuerpos de la momia: abajo, «dos cuerpos a la
  vez»), arma en reposo `0BF858` `25F520` (también las tablas de salto
  `0BFC58/0BFC98`), andar `0BF8D8` `25F562`, disparo `0BFD18` `25F5CA`,
  arriba `0BFD58` `25F89E`, abajo en el aire `0BFD98` `25F932`, granada
  `0BF998` `25F626`, subir/mantener/bajar el arma `0BF9D8` `25F74C`,
  `0BFA18` `25F8F0`, `0BFA98` `25F7AC` (papeles `B3_*`/`W3_*` de
  `compile_marco_campaign.py --mummy-capture`, `mummy_step` de
  `marco_visual.c`). `25F794/25F7A0` (`0BFB58/0BFB98`, el arma en el aire)
  están en el catálogo pero no se eligen (condición de `0C3A5E` sin
  traducir). Paletas: las ranuras `+19..+1B` del jugador son `63 64 65`
  (volcados de actor); en forma 3 la `63` y la `64` tienen las dos paletas
  de la momia (`--name-palette MUMMY=7FFE,6CB8,3863 MUMMY2=7FFE,3DC9,7986`);
  `marco_visual.c` pone esas ranuras en `+19..+1C` y `actor.c` dibuja cada
  parte con la de su byte alto; durante la transformación, las normales
  (abajo). Los efectos `C4DD8`/`C5286` son nativos desde `work-m2fix2`
  (abajo, «el humo de la transformación»).
Prueba: `tools/test_gameplay.c` (`mummy_run`): el aliento pide la forma 3 y
deja la pistola, 19 actualizaciones quieto, protección `0x5A`, la H
rechazada, 29,7 px en 20 actualizaciones andando y la muerte con el
segundo aliento.

### Forma 3: dos cuerpos a la vez (rama `work-m2events`)

Informe del usuario (hardware real, ROM 20): al convertirse en momia el
sprite normal de Marco sigue visible bajo la momia. **Causa**: lo de arriba
(«el mando `0C18B4` no elige otro cuerpo: se queda») era falso. El cuerpo
de la transformación `24EDE0` dibuja `1887E0` y se queda en `1887CC`, que es
Marco **normal y entero** (29×39, la cara de susto); el port lo dejaba como
cuerpo de la momia toda la forma 3 y encima ponía el arma de la momia
(`14ADEE`, 35×26): dos cuerpos. En MAME (`marco-mz2`, `debug.log`:
`MARCO_INIT`/`MARCO_DRAW` por actualización) la transformación dibuja **solo**
`1887E0`, `1887CC` ×17 (sin arma: `0C17CA` llama a `0C0F16`) y, desde
`0C182A`, el mando de la momia arranca sus propios cuerpos con `0C0FB6`
desde las tablas de cuatro personajes `C0408..` (fila 0 = Marco; no pasan
por `0C0F32`/`0C0F62`, por eso `controller.json` no las tenía):

| Situación (rutina) | Tabla | Programa | Arma |
| --- | --- | --- | --- |
| reposo `0C18B4` (cada actualización) | `C0408` | `25F3B4` (`14AD2E`) | sí (+90 = 0) |
| andar `0C196A` (+26 = 0x80) | `C0418` | `25F3CC` | sí (+90 = 1; `25F562` elegido cada actualización por +77) |
| giro `0C1924` (+9A bit 3; +98 = 2 solo voltea) | `C0448` (`C0458` con +92) | `260B1C` | no (`0C0F16`) |
| despegue `0C19E4` / aire | `C0428` | `25F486` | sí |
| aterrizaje `0C1AC8` (3 actualizaciones, +62 = 0) | `C0478` | `25F4EE` | sí |
| agacharse `0C1B14` / agachado `0C1B66` | `C0488` / `C0498` | `260B5E` / `260BE6` | no |
| disparo agachado `0C1CCC` | `C04B8` | `260C8A` | no |
| levantarse `0C1882` | `C04D8` | `260BA2` | no |

Cambios: `convert_marco_programs.py` toma también los `MARCO_INIT` de las
capturas extra (las raíces de `0C0FB6`); `compile_marco_campaign.py` añade
los papeles `B3_IDLE/WALK/JUMP/TURN/LAND/CROUCH_IN/CROUCH/CROUCH_FIRE/STAND`
comprobados contra esos `MARCO_INIT` y la tabla de la ROM;
`marco_visual.c` (`mummy_step`) los elige como el mando: sin arma durante la
transformación, el giro y agachado; el arma se elige otra vez cuando cambia
+90; el disparo hacia arriba acabado baja el arma (`25F7AC`, 3555); el
aterrizaje empieza en la actualización siguiente a la que toca el suelo.
`actor.c` usa las paletas normales durante la transformación (el
`audit.json` de `marco-mz2` usa el perfil normal en sus 37 primeros
fotogramas y el de la momia después). `gameplay.c`: el aire de la momia
`0C1A72..0C1AA6` exacto (objetivo ±0x3E000 con desplazamiento 3 con
dirección, 0 con 5 sin ella; antes una aproximación declarada), el
aterrizaje sin andar tres actualizaciones (`0C1ADE`) y el despegue cuenta
como aire para apuntar abajo (3831: abajo y disparo dibujan `25F932`).

**Prueba** (`make check-marco-campaign`, privada): `tools/extract_mummy_fixture.py`
saca de `marco-mz2` las 1475 actualizaciones (2101..5049) con la situación
que el juego del port da a `marco_visual.c` (de la rutina del mando y los
mandos) y los descriptores que el original dibujó para el cuerpo y el arma
(`data/marco_mummy_fixture.json`); `test_marco_visual.c` las repite:
**cuerpo 1474 iguales, 0 distintos; arma 1462 iguales, 12 distintos**
(conocidos: el disparo de la actualización del aterrizaje apunta abajo en
el original, 4; el arma subida/bajada en el aire `25F7A0/25F794` sin
traducir, 2, y 6 de la fase del reposo que le sigue).

```sh
python3 tools/convert_marco_programs.py CAP1 --event-evidence CAP1/diary/effects.json --extra-capture H2 --extra-capture H3 --extra-capture G1 --extra-capture C1 --extra-capture K1 --extra-capture K2 --extra-capture DSC0 --extra-capture DSC1 --extra-capture $N/marco-w2 --extra-capture $N/marco-w4 --extra-capture $N/marco-w6 --extra-capture $N/marco-mz2 --equivalent-calls --output GRAFO
python3 tools/compile_marco_campaign.py CAP1 --graph GRAFO --hgun-capture H2 --grenade-capture G1 --cgun-capture C1 --knife-capture K1 --knife-h-capture K2 --descent-capture DSC0 --w2-capture $N/marco-w2 --w4-capture $N/marco-w4 --w6-capture $N/marco-w6 --mummy-capture $N/marco-mz2 --output build/marco_campaign.h
python3 tools/extract_mummy_fixture.py $N/marco-mz2 --output data/marco_mummy_fixture.json
```

(CAP1 = `captures-m1full/marco-cat1`, H2/H3 = `captures/weapons/marco-h2/h3`,
G1/C1/K1/K2 = `captures-grenade/marco-*`, DSC0/DSC1 = `captures-end/marco-dsc*`;
el grafo sin el cambio reproduce byte a byte el de `marco-graph-w`.) El
catálogo de poses no cambia (las 92 poses de `marco-mz2` ya estaban): solo
`build/marco_campaign.h` (privado) debe regenerarse.

Paradas explícitas (el cuerpo se queda, contador `stops` de
`marco_visual_t`): bajar de un borde sin saltar (`C0438`, `25F4BC`), el giro
con +92 (`C0458`), reptar y su giro (`C04A8`/`C0468`; el port no repta) y la
granada agachada (`C04C8`): no aparecen en `marco-mz2`.

### Forma 3: el humo de la transformación (rama `work-m2fix2`, B28 = B13)

Informe de MiSTer (ROM 22): al convertirse en momia no sale humo. `0C17A0`
crea tras el jugador (`0040BC`) el efecto `0C4DD8`: paleta `0xB0`
(`009EA8`), programa `2A97A0` (32 poses), `+60 = 0`, `01AD6E` sobre el
bloque `+28` del jugador, `+6E = 0x10` y el sonido `1085`; `0C4E1C` copia
cada actualización el `+1E` del jugador, cuenta `+6E` y, en el préstamo (su
17.ª actualización), pone `+60 = FF` en el jugador y se borra; si no,
`01ABC8` y `00427A`. `0C17F0` espera ese `+60` y `0C182A` crea el marcador
`0C5286` (`0C186E`, el mismo «1P» del aterrizaje). El cambio de paleta a
mitad (las ranuras `63/64` del jugador pasan a las de la momia en
`0C182A`) ya lo hacía `actor.c`.

Port: `neo_landing.c` tiene un tercer nodo (`neo_landing_smoke`), que
`gameplay.c` arranca en la primera actualización de la transformación
(`mummy_transform` = 18) y cuyo `smoke_done` deja crear el marcador en la
18.ª (`neo_landing_marker`); sus dibujos van sobre el jugador (su `+1E` y
detrás de él en la lista) y su sonido a la cola de sonidos del mundo
(`sfx_1085`, render aislado `OK`, 0,85 s, 11672 bytes residentes).

Captura (MAME 0.264; `MSX_FULL_GIVE_MUMMY=<fotograma>`, nuevo en
`capture_fullgame.lua`, solo captura: `+94 = 3` como el aliento en
`0C36C0`):

```sh
export MSX_FULL_STATES=/home/user/captures-fullgame/states4 MSX_FULL_LOAD=sec01_19391 MSX_FULL_LOAD_FRAME=19391
MSX_FULL_FAMILY_FROM=19455 MSX_FULL_FAMILY_ROOTS=c4dd8,c5286 MSX_FULL_VRAM_TRACE=1 MSX_FULL_FIRST_DUMP=19455 MSX_FULL_PERIOD=1 \
  MSX_FULL_GRID_TAP=0 MSX_FULL_IDLE=19455-19530 MSX_FULL_GIVE_MUMMY=19460 MSX_FULL_LAST=19530 sh tools/run_capture_fullgame.sh CAP/mz4
python3 tools/extract_landing_fixture.py CAP/mz4/boss-trace.txt --output data/smoke_fixture.json.gz
```

**Prueba** `tools/test_neo_smoke.c` (`make check`; el extractor de
`landing` reconoce ahora el humo y anota las palabras `10B954/10B95C` que lee
`012022`): **33 actualizaciones (17 del humo, 16 del marcador), 4544 bytes
iguales, `smoke_done` en la 17.ª**. Píxeles (`export_rest_sprites.py ...
mz4@c4dd8-c4f60`, un volcado por fotograma): **33 volcados, 45983 píxeles
iguales**; los 1514 distintos son los dos volcados tomados en el mismo
fotograma de la actualización del constructor (la lista de sprites llega al
fotograma siguiente; 757 + 757).

## Disparo de las armas 02/07 y 04/09 (rama `work-m2mummy`)

Capturas `marco-w2`, `marco-w4` y `marco-w6` (`marco-cgun.json`,
`run_marco_catalog.py --give-weapon <id> --keep-bombs`, nuevo: concesión
genérica con las tablas `0C0928`): **1220 fotogramas auditados en cada una,
120/155/131 descriptores, 917499/870616/916270 píxeles iguales**. Papeles
(forma 1): `W1S_FIRE` `253896`, `W1S_FIRE_UP` `253962`, `W1S_FIRE_DOWN`
`256030`, `B1S_CROUCH_FIRE` `256F26` (tablas `0BFFD8..0C0098`, ids
02/07/0F) y `W1F_*`/`B1F_*` `25391A`, `2539FA`, `256078`, `256FAA`
(`0C00D8..0C0198`, ids 04/09); `marco_visual.c` los elige por `cgroup`.
Los programas del rayo (`253594`, `2536FC`, `256132`, `256DE8`) tienen papel
desde la rama `work-laser` (`W1L_FIRE`, `W1L_FIRE_UP`, `W1L_FIRE_DOWN`,
`B1L_CROUCH_FIRE`, comprobados contra `marco-w6` con `--w6-capture`;
`marco_visual.c` los elige con `cgroup` 4; ver ARMAS_VEHICULOS.md, Paso 16). Catálogo:
**562 poses (las 411 anteriores idénticas), 585 piezas CI4, 436304 bytes,
7 perfiles**. Grafo con un segundo `UNRECOVERED` (`256CCA`, retrollamada
`0C53C4` de un cuerpo agachado de `marco-w4` que la campaña no usa).
`make check-marco-campaign`: **578 pasos iguales, 0 distintos (34 casos)**,
con los disparos de 02 y 04 y el disparo y la granada de la momia.

```sh
N=/home/user/captures-weap2   # capturas nuevas; el resto como arriba
python3 tools/mame/run_marco_catalog.py --rom-dir ROMS --output $N/marco-w2 --command-trace --paired-trace --controller-trace --plan tools/mame/plans/marco-cgun.json --give-weapon 2 --keep-bombs   # y 4, 6
python3 tools/mame/run_marco_catalog.py --rom-dir ROMS --output $N/marco-mz2 --command-trace --paired-trace --controller-trace --give-mummy
# auditoría, extract_marco_steps.py y extract_marco_controller.py de cada una
python3 convert_marco_programs.py CAP1 ... --extra-capture DSC0 --extra-capture DSC1 --extra-capture $N/marco-w2 --extra-capture $N/marco-w4 --extra-capture $N/marco-w6 --extra-capture $N/marco-mz2 --equivalent-calls --output GRAFO
python3 build_marco_poses.py CAP1 ... --extra-audit DSC1/audit.json --extra-audit $N/marco-w2/audit.json --extra-audit $N/marco-w4/audit.json --extra-audit $N/marco-w6/audit.json --extra-audit $N/marco-mz2/audit.json --extra-profile-min-uses 40 --name-palette BURNT=7FFF,6FF0,4840 --name-palette MUMMY=7FFE,6CB8,3863 --name-palette MUMMY2=7FFE,3DC9,7986 --rom mslugx.zip --mksprite $N64_INST/bin/mksprite
python3 compile_marco_campaign.py CAP1 --graph GRAFO --hgun-capture H2 --grenade-capture G1 --cgun-capture C1 --knife-capture K1 --knife-h-capture K2 --descent-capture DSC0 --w2-capture $N/marco-w2 --w4-capture $N/marco-w4 --w6-capture $N/marco-w6 --mummy-capture $N/marco-mz2 --output build/marco_campaign.h
```

## Entradas privadas: comprobación por contenido

Informe de MiSTer sobre la ROM de prueba 18: «Marco ha perdido animaciones».
En ares la 18 escribe `MSX marco part 0: format/size 35x26` y
`native animation missing: six-state fallback (0 poses)`: el ELF llevaba un
`marco_renderer.o` compilado con el catálogo anterior (411 poses, parte 0 de
29x29) y el DFS las 585 piezas del catálogo de 562 poses. Las entradas
privadas (`build/marco_pose_meta.h`, `build/marco_campaign.h`,
`romfs/marco`) se habían copiado conservando su fecha (`cp -p`), más antigua
que el objeto, y `make` no lo recompiló. Sin el renderizador nativo el port
vuelve a los seis estados: sin agacharse, apuntar arriba, disparo hacia abajo,
granada, cuchillo, paracaídas, muerte… La ROM 16 (y la 17) cargan el
catálogo nativo.

- `tools/marco_inputs_sum.sh` (en el análisis del Makefile) escribe
  `build/marco-inputs.sum` solo si cambia el contenido de esas entradas;
  `marco_renderer.o`, `marco_visual.o`, `test_marco_render.o` y los sellos de
  romfs dependen de él.
- `tools/check_marco_romfs.py` repite las pruebas de `marco_renderer_load`
  (número, CI4, tamaño, huella de píxeles) al construir `game-romfs` y en
  `make check-marco-campaign`: un catálogo y unas piezas distintos paran la
  construcción en lugar de dar una ROM con seis estados.
- `tools/test_marco_inputs_sum.sh` (`make check`): una copia con fecha antigua
  y contenido distinto renueva la suma, y las reglas dependen de ella.

## Pendiente

- Controlador original completo (`0x0C1xxx..0x0C3xxx`, ya traducido en
  partes) en lugar de la situación del port: giros (`0x24EF0E`, `0x24EF50`),
  variantes de reposo largo (`0x24F072`, `0x24ED92`), aterrizajes,
  transiciones de apuntado (`0x24C65C`/`0x24C6C0`).
- Acciones sin juego en el port: cuchillo (necesita enemigo al alcance),
  disparo hacia abajo en el aire
  (`0x24EABC`: el port no tiene esa bala), reptar, la muerte de tipo 2 en el
  suelo (`2503D4`, sin captura), otras armas y la Camel Slug.
- Efectos de los eventos de animación (fogonazo, casquillos).
- Cambio de banco de paleta al reaparecer (perfil 1).
