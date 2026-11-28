# Plan de trabajo: ROM 54 (fallos de la revisión de la ROM 53)

Informe del usuario (27-XI, ROM 53 en MiSTer) y plan del ingeniero jefe.
Reglas: CLAUDE.md (nada inventado ni ajustado a ojo; cada cambio, bit a bit
contra MAME 0.264; como máximo 3 agentes, cada uno en su worktree y su Xvfb).
Método de cada fallo: `docs/PLAN_CORRECCIONES.md` (8 pasos: reproducir →
captura MAME → rutina 68K → causa → traducción → prueba de familia → ares →
matriz).

## Fallos registrados

| ID | Síntoma (MiSTer, ROM 53) | Carril | Prio |
| --- | --- | --- | --- |
| B86 | Audio roto en la pantalla de selección de personaje | M | 2 |
| B87 | A veces el protagonista solo muestra las piernas, sin torso | J | 1 |
| B88 | El juego se acelera en unos momentos y se ralentiza en otros | R | 1 |
| B89 | Agachado, las balas y el aliento de las momias le alcanzan (según el usuario no deberían) | J | 2 |
| B90 | Saltar y disparar hacia abajo ralentiza el juego a veces | R | 1 |
| B91 | Misión 5, metro: el evento no avanza y la misión no se puede terminar | M | 1 (bloquea la partida) |
| B92 | Objetivo: FPS estables, sin bajones ni aceleraciones | R | 1 |

## Evidencia de partida (matriz `gates4`, ROM 53, ares)

- **B88 / B92: causa probable localizada.** `src/scene_clock.h` reparte la
  simulación con un reloj de pared a 59,185606 Hz y **conserva la deuda**:
  tras un fotograma lento, los siguientes ejecutan 2 pasos de simulación
  (hasta 8) para recuperar. Eso es la «aceleración» que ve el usuario después
  de cada «bajón». En `m5s2`: `steps=302..312` por 300 fotogramas,
  `multi=6..16` fotogramas con doble paso, y un pico de `sim` de 1 077 440 µs
  (más de 1 s en un solo fotograma). Además, como 59,19 Hz < 60 Hz, cada ~73
  fotogramas de la N64 hay uno sin paso de simulación (tirón periódico).
  - En el arcade, cuando el 68K no llega al vblank, el juego **se ralentiza**
    y nunca recupera. Hay que confirmarlo con la rutina de espera de vblank de
    la ROM (no suponerlo) antes de cambiar el reloj.
- **B90**: misma vía (fotograma lento → recuperación). Falta saber qué hace
  lento ese fotograma (proyectiles, dibujos o carga de assets); se mide con la
  línea `MSX PERF` (`docs/RENDIMIENTO.md`).
- **B87**: hipótesis por orden: (1) pose del torso que no está en el catálogo
  (ya se sabe que falta la 189D88, la del disparo agachado con la H); (2) la
  ranura del torso se pierde de la caché de escena, que está justo en el
  mínimo (216); (3) la combinación de piernas y torso de B83/B84. Cada una se
  descarta con una captura, no por suposición.
- **B89**: hay que **comprobar primero en MAME** qué hace el original
  (posiciones y cajas de choque agachado frente a cada tipo de bala y al
  aliento). Si el arcade también alcanza al personaje agachado, se registra
  como comportamiento original y no se cambia.
- **B91**: la prueba `m5s2` pasa porque solo comprueba el tramo, no el evento
  que termina el metro. Hay que reproducir la partida completa de la M5 hasta
  el metro (estados `272000` de `docs/MISION5.md`), comparar los registros de
  evento con MAME y localizar la condición que no se cumple.
- **B86**: zona del B44 (música de SOLDIER SELECT, `docs/AUDIO_JUEGO.md`).
  Hay que distinguir si falla la música, las voces de selección o ambas, y si
  pasa también en ares o solo en MiSTer (rendimiento del audio frente a los
  `underruns` que ya se ven en los registros).

## Carriles (3 agentes como máximo, sin pisarse)

| Carril | Fallos | Ficheros principales | Worktree / pantalla |
| --- | --- | --- | --- |
| **R: reloj y rendimiento** | B88, B90, B92 | `scene_clock.h`, `main.c`, `msx_perf.c`, `scene_assets.c` | `ws-r` / `:95` |
| **J: jugador** | B87, B89 | `marco_visual.c`, `actor.c`, colisión del jugador | `ws-j` / `:96` |
| **M: misión y audio** | B91, B86 | M5 (`neo_train.c`, eventos), `msx_sfx.c`, música | `ws-m` / `:97` |

R va primero: cambia la temporización de todo, y las demás pruebas en ares
deben medirse ya con el reloj corregido.

### R: reloj y rendimiento

**Estado (work-r):** 1–2 hechos (el original ralentiza: `003A48` borra los
vblanks; `scene_clock` ya no recupera). 3: perfilado, sin optimización
integrada (el coste está repartido en `run_gactor`). 4: `make check`, caché
216/216 y matriz PASS, pero `multi` no es 0. 5: medido (VI 59,846 Hz, un
fotograma sin paso cada ≈ 91), opciones en `docs/RENDIMIENTO.md`. Opción 3
hecha: VI a 59,183296 Hz (265 × 3104 relojes, `src/msx_vi.h`), 1 paso por campo.

1. Desensamblar la espera de vblank y el bucle principal del 68K. Decide qué
   pasa en el original cuando un fotograma no llega (ralentiza o recupera).
2. Traducir ese comportamiento al reloj. Si el original ralentiza, se elimina
   la recuperación de deuda (`scene_clock`), sin ningún valor ajustado a ojo.
3. Perfilar los picos: el `sim` de más de 1 s en la M5 y saltar disparando
   hacia abajo. Optimizar la fase culpable, sin cambiar la lógica.
4. Prueba: familia del reloj (fixture de pasos por fotograma) y
   `MSX PERF` en ares sin `multi` sobre la matriz completa, con los mismos
   resultados que `gates4`.
5. El tirón de 59,19 Hz frente a 60 Hz es una decisión aparte: se presenta al
   usuario con medidas y no se resuelve a ojo.

### J: jugador
1. Reproducir B87 en ares con el registro de poses, cuando falta el torso
   (pose pedida, ranura y caché).
2. Arreglarlo según la causa: añadir al catálogo las poses de la ROM que
   falten (incluida la 189D88) o asegurar la ranura del torso.
3. B89: captura MAME agachado frente a soldados y momias; comparar las cajas
   de choque y traducir la rutina si difiere.
4. Prueba: familias de Marco (`test_run_turn`, poses), ares con los cuatro
   personajes.

### M: misión y audio
1. B91: estados de la M5, metro → registro de eventos en MAME frente a N64 →
   condición que falta → rutina → traducción.
2. Prueba nueva en la matriz: la M5 completa hasta el jefe (`m5full`), para
   que este fallo no vuelva a pasar sin verse.
3. B86: render aislado de la música y de las voces de SOLDIER SELECT,
   comparado con la grabación de MAME, y medida de `underruns` en esa
   pantalla.

## Criterios para dar por buena la ROM 54
- `make check` completo (salvo los 2 pasos bloqueados por datos perdidos, ya
  registrados).
- Matriz ares igual o mejor que `gates4`, más `m5full`.
- `MSX PERF`: `multi=0` en toda la matriz y ningún paso de simulación por
  encima del presupuesto del fotograma.
- Caché de escena ≥ 216 ranuras en los dos arranques.
- Nueva versión del generador de Windows (`tools/winbuilder/build.sh`), como
  `metalslugx64_rev1.1`.
- Confirmación del usuario en MiSTer.

## Orden
1. R (crítico: afecta a todo) y M/B91 (bloquea la partida), en paralelo.
2. J, cuando R deje el reloj estable.
3. M/B86 al final de M.
