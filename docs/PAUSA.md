# Pausa: la pausa propia de la ROM (`015520`), agente C

Estado: **hecha y fusionada**. La ROM de arcade trae la pausa del sistema
doméstico (AES): no se ha inventado ninguna pantalla. La pausa anterior del
port (fotograma atenuado a la mitad, PAUSE centrado, silencio) se ha quitado.

## 1. Lo que hace la ROM

- **Dónde.** El bucle principal (`003A42`) lee los puertos (`03CB3A`: `380000`
  → `10C366` pulsado, `10C367` pulsaciones nuevas) y llama a `015520`
  (`003A88`). Su resultado (0 en pausa, 1 corre) se guarda en `10008D`.
- **Botón.** En el camino doméstico (`10FD82` = 0, el que ya usa el port:
  MISION1_COMPLETA «HUD sin monedas») `015448` toma de `10C367` el SELECT del
  jugador 1 (bit 1) y el del 2 (bit 3), y quita el de un jugador cuyo modo de
  BIOS `10FDB6`/`10FDB7` es 0 (no juega) o 3 (fin de partida). El mismo SELECT
  de quien pausó reanuda (`10C2BF`: bit 0 jugador 1, bit 1 jugador 2).
- **Cuándo no.** Con `10C2BD` ≠ 0 no se entra en pausa (`01554A`; `10F000`
  bit 1 lo saltaría, pero la ROM nunca lo pone). `015418` lo pone y `015422`
  lo quita: puesto en la cortinilla de entrada de etapa (`01833E`) y en el fin
  de misión (`018852`, `01888E`) hasta acabar el recuento (`0188B8`); quitado
  al empezar el bucle de etapa (`01867A`). También en el fin de partida
  (`018AF0`), el final (`0189C6`) y la demostración (`01691E`/`016C88`).
- **Al pausar** (`003A9A`, vuelta en que `015520` da 0 y antes daba 1):
  - `002836`/`002802(0)`: guarda la velocidad de la auto-animación del LSPC y
    la apaga (las teselas animadas quedan en su fase 0);
  - `009BBC` guarda el fundido `10991A..` y `009C0A(-8,-8,-8,4)`: la paleta
    baja 8 niveles por canal en cuatro vueltas (las paletas con el bit 1 de su
    bandera, las del HUD, no cambian);
  - cada vuelta en pausa: `00698C` (paso del fundido) y espera de **dos
    vblanks** (`003AE0`); el juego no corre.
- **PAUSE.** `10C2C2` baja 1 por vuelta desde 0. Con `(10C2C2 & 0x18) == 0`:
  si `(10C2C2+1) & 7` es 0 escribe cinco espacios y si es 1 escribe `PAUSE`
  (`03D244`, cadenas `01543B`/`01542A`: `\l` columna 0x12 fila 0x1A = VRAM
  fix `725A`, paleta 0, página 3 → palabras `0350 0341 0355 0353 0345` y
  `0320`). Periodo de 32 vueltas (64 fotogramas): visible 25, oculto 7; la
  primera vez aparece 32 vueltas después de pausar.
- **Botones** (`0156FA`, cada vuelta en pausa y la que reanuda): un botón
  pulsado ahora y no pulsado al pausar (`10C2CA`/`10C2CB`) cuenta como
  pulsación nueva (`10C35D`/`10C363`).
- **Al reanudar**: borra PAUSE (`03D25A` con cinco espacios; las celdas quedan
  en `0320`), `0C8E5C` pone a 0 el +88 de los dos paneles (se redibujan),
  `002802` con la velocidad guardada, `009BD4` devuelve el fundido guardado
  (si no había ninguno en marcha, cuenta 1: el siguiente `00698C` repone la
  paleta) y `10008E` = FF esa vuelta (`0C8AE4`: redibujo de la capa fix del
  escenario; el port la redibuja siempre).
- **Sonido.** La pausa no manda ninguna orden de sonido: el Z80 sigue. La
  música continúa y los efectos ya empezados terminan (captura: nivel RMS
  igual antes, durante y después).
- **No traducido** (estados que el port no tiene): el modo de desarrollo
  `10C2BE` (BIOS `10FE80`: avance por fotogramas con START), el menú de los
  modos de dos jugadores `10C2E7` ≠ 0 (`0155B0`, `015750`, cadenas `015824`)
  y la petición forzada `10C2C0` (`0154F8`, solo desde `018AB0` de esos modos).

## 2. Captura (MAME 0.264)

`tools/mame/capture_pause.lua` (órdenes en su cabecera) desde el estado
`u1020` del TAS (Misión 1, sin más entradas). El set MVS de un cartucho no
tiene SELECT ni modo doméstico, así que la captura usa dos artificios
(solo captura): escribe `10FD82` = 0 cada fotograma y baja el bit 1 de la
lectura de `380000` (SELECT del jugador 1) en 120–124 y 420–424. Resultado
(`/home/user/captures-chars/pause1`, privado; `pause.csv`, una fila por
fotograma): pausa en 121, fundido FE00/FC00/FA00/F800, espacios en 171,
PAUSE en 185, …, reanudación en 421 con `10008E` = FF.
`tools/pause_fixture.py --extract` → `data/pause_fixture.json`: 274 vueltas
(150 en pausa) con dueño, parpadeo, resultados, fundido, LSPC y las cinco
celdas tras cada vuelta.

Sin captura (solo desensamblado): pausa con un fundido en marcha, con la
auto-animación encendida (en `u1020` ya estaba apagada) y la negativa por
`10C2BD`.

## 3. Port

- `src/neo_pause.c/.h`: `015520` (camino doméstico) y la vuelta del bucle
  `003A88..003B20`, con el fundido (`neo_fade.c`), el LSPC (`neo_lspc.h`) y
  el texto (`neo_text.c`) que el port ya tenía.
- `src/neo_mission.c`, `src/gameplay.c`: `pause_off` (`10C2BD`) en los sitios
  de `015418`/`015422` que el port ejecuta (`01833E`, `01867A`, `018852`,
  `01888E`, `0188B8`). `msx_game_resume_inputs`: el historial de botones de
  la actualización que sigue a la pausa (`0156FA`).
- `src/actor.c`: una vuelta por fotograma presentado al empezar una
  actualización original (ticks pares); en pausa, una vuelta cada dos vblanks
  del reloj de simulación y el contador del LSPC sigue (es hardware). El
  audio no se toca: la música sigue; `game_audio_update` no corre (no hay
  órdenes nuevas).
- **Añadidos de la N64 (no arcade):** (1) el mando de N64 no tiene SELECT:
  **START hace de SELECT** mientras el panel del jugador 1 está en su rutina
  de juego (`0C9650`, donde el original no lee START) y mientras dura la
  pausa; en la continuación START sigue siendo el START del original. (2) R
  reinicia también desde la pausa. (3) En las ROM de referencia internas,
  donde START ya tiene otro uso, no hay pausa. (4) Las pulsaciones se miran
  por fotograma presentado, como el resto de botones del port.

## 4. Pruebas

- `make check-pause` (dentro de `make check`, `test_gameplay pause`):
  `neo_pause.c` contra las 274 vueltas de MAME (todo igual), y el juego con
  24 pausas en 6000 actualizaciones (el juego no se mueve en pausa y sigue
  igual que la partida sin pausas; botón nuevo al reanudar; negativas por
  `10C2BD`, modos 0 y 3 y START).
- ares, cartucho normal (`tools/test_pause_ares.py`): START pausa; la imagen
  se oscurece, queda quieta salvo las celdas de PAUSE, que parpadea; sin
  registro de simulación con botones pulsados; START reanuda, la imagen
  recupera el brillo y el juego sigue; R reinicia desde la pausa. **PASS.**
