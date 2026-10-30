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

## Pendiente

- Controlador original completo (`0x0C1xxx..0x0C3xxx`, ya traducido en
  partes) en lugar de la situación del port: giros (`0x24EF0E`, `0x24EF50`),
  variantes de reposo largo (`0x24F072`, `0x24ED92`), aterrizajes,
  transiciones de apuntado (`0x24C65C`/`0x24C6C0`).
- Acciones sin juego en el port: cuchillo (necesita enemigo al alcance),
  granadas (`0x24C862`, `0x24ED44`, `0x25009E`), disparo hacia abajo en el aire
  (`0x24EABC`: el port no tiene esa bala), reptar, muerte (secuencia vista en
  la captura pero no aislada), otras armas (H) y la Camel Slug.
- Efectos de los eventos de animación (fogonazo, casquillos).
- Cambio de banco de paleta al reaparecer (perfil 1).
