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
necesitan apuntar abajo en el aire, que el jugador del port no tiene.
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

## Pendiente

- Controlador original completo (`0x0C1xxx..0x0C3xxx`, ya traducido en
  partes) en lugar de la situación del port: giros (`0x24EF0E`, `0x24EF50`),
  variantes de reposo largo (`0x24F072`, `0x24ED92`), aterrizajes,
  transiciones de apuntado (`0x24C65C`/`0x24C6C0`).
- Acciones sin juego en el port: cuchillo (necesita enemigo al alcance),
  granadas (`0x24C862`, `0x24ED44`), disparo hacia abajo en el aire
  (`0x24EABC`: el port no tiene esa bala), reptar, la muerte de tipo 2 en el
  suelo (`2503D4`, sin captura), otras armas y la Camel Slug.
- Efectos de los eventos de animación (fogonazo, casquillos).
- Cambio de banco de paleta al reaparecer (perfil 1).
