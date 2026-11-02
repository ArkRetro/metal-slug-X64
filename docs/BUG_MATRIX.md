# Matriz de fallos

**Estados:**
- **abierto**: observado y sin corregir.
- **en curso**: un agente o una rama trabaja en él.
- **corregido (ares)**: probado en el emulador.
- **corregido (MiSTer)**: confirmado por el usuario en hardware real.
- **sospecha**: aún no reproducido.

**Prioridades:**
- **P1**: memoria o bloqueo.
- **P2**: impide avanzar.
- **P3**: causa compartida.
- **P4**: armas o actores.
- **P5**: render o audio.
- **P6**: rendimiento.

**Evidencia:**
- **U**: informe del usuario en MiSTer.
- **A**: ares.
- **M**: MAME.
- **H**: prueba en el PC.

## Rutas de los arcos y abreviaturas

Las rutas de los arcos 68K que se citan en la tabla:

- **Mujer momia (sección 2):** creación `0613CA`, aliento `061E8C` (tipo `0x10` vía `004C24`).
- **Puerta del final de la etapa 2:** actor `03A6EA`, apertura `03A776` → `0B7350`, salida `0B6ED2`.
- **Láser, arma 06:** rayo `01E19E` / `01E166`.
- **Rocas de la etapa 3:** `0C9AB4`, de tipo nodo `0000BA0`.
- **Puerta de la etapa 1:** `0B6F24`.

Abreviaturas:

- **rel.:** ROM normal (`mslugx-explorer.z64`).
- **ref.:** ROM de referencia, con invulnerabilidad y piloto automático.

## Tabla

| ID | Síntoma | Reproducción | Esperado (arcade) | Actual | Sistema | Evidencia | Prio | Estado |
| --- | --- | --- | --- | --- | --- | --- | --- | --- |
| B01 | ROM 18 sin animaciones de Marco | arrancar la ROM 18 | 562 poses | caía a las 6 poses de reserva | compilación (`.o` obsoleto) | A: el registro decía "six-state fallback" | P2 | corregido (ares), `9133e1f`; falta MiSTer |
| B02 | Parones en la intro | arrancar desde el logo | sin pausas | había pausas de 0,5 a 1,9 s | `cinematic.c` | A: `MSX FRAMES` | P5 | corregido (ares), `341dfc3`; falta MiSTer |
| B03 | Tirón al empezar a jugar | después del paracaídas | 60 fps | unos 8 fotogramas de vblank, más 10 de 120 fotogramas tarde | `world_reset`, init del renderer, init del audio, primer HUD | A | P6 | abierto |
| B04 | Lanzacohetes (R) no disponible al llegar al jefe de la M1 | jugar la M1 con la ROM normal hasta el jefe | el R se puede recoger y usar | no aparece | objetos o guion de la M1 | U | P4 | corregido (ares) `0b68f14`: era el arma 8 (cohete 0641EC), no la R |
| B05 | Transición M1 → M2 incorrecta | vencer al jefe de la M1 | MISSION COMPLETE → recuento → fundido → MISSION 2 START | distinta | `neo_mission.c`, `neo_end.c` | U | P2 | corregido (ares) `0b68f14`: fundido, 1805C, letras MISSION 2 START |
| B06 | Sin música al empezar la M2 | entrar en la M2 viniendo de la M1 | suena la palabra 0022 | silencio | `game_audio_stage_music` | U (A vio la música en la M2 de la ROM de referencia) | P5 | corregido (ares) `0b68f14`: la música de etapa siempre suena |
| B07 | El suelo del inicio de la M2 se desplaza raro | inicio de la etapa 1 de la M2 | desplazamiento igual al de MAME | raro | scroll o capas de la escena | U | P5 | corregido (ares) `0b68f14`: scroll por capa de la misión |
| B08 | Faltan actores secundarios de la M2 (mineros, arqueólogos) | jugar la M2 | aparecen | no aparecen | `neo_world` (creadores no traducidos) | U | P4 | corregido (ares) `ab76871`; faltan 7 actores de la etapa 3 |
| B09 | El orden de los eventos de la M2 no es el del arcade | jugar la M2 | el orden de MAME | distinto | `neo_mission.c`, guion, lista de cámara | U | P3 | corregido (ares) `ab76871`; por confirmar en MiSTer |
| B10 | Momia con el sprite de Marco superpuesto | recibir el aliento de la momia | solo la momia | dos cuerpos | `marco_visual.c`, forma 3 | U | P4 | corregido (ares) `ab76871`; por confirmar en MiSTer |
| B11 | El láser (06/0B) dispara la bala de la pistola | coger la L en la M2 | rayo de varios segmentos | bala de pistola | `neo_pweapon.c` (rayo 01E22C), `gameplay.c` (0C46D6) | H: familias `laser6`/`laserb`, `pweapon`; M: 45230/1996 píxeles | P4 | corregido en el PC (familias contra MAME) `8ceca05`; ares bloqueado por el presupuesto de RAM (NEXT_STEPS 6); falta MiSTer |
| B12 | Soldados muertos por fuego o explosión desaparecen | matar con F o S | animación de muerte (tipos 7, 0A, 0B) | desaparecen | `neo_soldier_ai.c` | H: `soldier_death` (1468 actualizaciones), `pweapon` 0 paradas | P4 | corregido en el PC (contra MAME) `8ceca05`; ares bloqueado por el presupuesto de RAM; queda la parada `040920` (cuerpo en llamas, sin captura) |
| B13 | Faltan los efectos de la transformación en momia (C4DD8, C5286) y el cambio de paleta | transformación | efectos | no hay | `gameplay.c`, render | doc | P5 | = B28 |
| B14 | Caídas sueltas: M2 etapa 2 en x 2281, etapa 3 en x 348 y x 611 | `test_mission2_ares.py` | no hay caída | rescate de la ROM de referencia | `wall_stop` con el obstáculo dinámico del hueco 7 | A, M | P2 | abierto |
| B15 | Puerta de la etapa 2: no se dibujan sus tiles una vez abierta y falta el sonido 103B | final de la etapa 2 | tiles y sonido | no hay | render y audio | doc | P5 | abierto |
| B16 | Armas 01, 08, 0E y 0F | — | nativas | bala de pistola | `gameplay.c` | H | P4 | abierto (no salen en la M2) |
| B17 | Salto sin dirección después de correr: el arcade pone la velocidad horizontal a 0 | M | 0 | se conserva | `gameplay.c` | M | P4 | sospecha (rutina sin probar) |
| B19 | La ROM de referencia se para al arrancar por falta de RAM | arrancar `mslugx-reference.z64` o `mslugx-m2-reference.z64` en `ab76871` | caché de escena de 208 ranuras o más | `ASSERTION room>=MIN_SLOTS`: faltan 2 KB (quedan 3326 KB y hacen falta 3328 KB) | presupuesto de RAM (ELF, efectos residentes) | A: `/home/user/g21` | P1 | en curso (`work-diag`) |
| B20 | Faltan los efectos `sfx_10a5` y `sfx_1026` | registro de audio | 69 de 69 | 63 de 69 | audio | A | P5 | en curso (`work-diag`) |
| B21 | El título salta un fotograma (2→4) en la ROM 21 | `tools/test_logo_ares.py mslugx-explorer.z64` (ROM de `/home/user/ws-logo`) | 0 saltados, `late=0` | `MSX TITLE miss from=2 to=4`, `dropped=1`, `late=1` | `cinematic.c`: la comprobación de las tablas de HOW TO PLAY (2691 entradas) y la reserva de su anillo en un solo paso (3,4 ms) pasaban el blanco; 10 `.o` fríos de `/home/user/ws-logo` seguían compilados -O2 (logo más lento, el título sin cargar entero al empezar) | A: ROM idéntica byte a byte reconstruida | P5 | corregido (ares) `work-perf21`: tablas por trozos con plazo, `Makefile` recompila los fríos al cambiar la lista -Os; falta MiSTer |
| B22 | Montón libre 234 KB al final del jefe de la M1 (margen 256) | `MSX_DIAG`, recorrido M1 | ≥ 256 KB | 234 KB | `scene_assets.c`: la reserva de `actor_init` (640 KB) no contaba la tira del HUD (133 KB); `scene_renderer.c`: los bloques del fondo grababan hasta 192 KB libres | A | P1 | corregido (ares) `work-perf21`: tira del HUD en CI8 (133 → 67 KB), reserva de `actor_init` 672 KB (medida 654), bloques del fondo solo con ≥ 288 KB libres; M1: mínimo 344 KB, M2: 283 KB, 213 ranuras (ref.) / 214 (rel.) |
| B23 | Balas de pistola 8/8 con rechazos | `MSX_DIAG` (pools) | el arcade no limita la pistola (`10C71C` = FF, `0C08A8[0]`): solo su pool de objetos | 8 balas, rechazos en ráfagas | `gameplay.h` `MSX_BULLETS` | A, ROM | P4 | corregido (ares) `work-perf21`: 16; pico 10/16 (M1) y 14/16 (M2), 0 rechazos |
| B24 | Al vencer al jefe de la M2 (columna donde se salta y se dispara hacia abajo) el juego no continúa | jugar el jefe de la M2 con la ROM 22 | fin del jefe → MISSION COMPLETE | se queda parado | `neo_m2boss.c`, flujo de misión | U | P2 | en curso (`work-m2fix1`) |
| B25 | La escopeta no dispara bien hacia la izquierda | coger la S y disparar a la izquierda | estallido a la izquierda | solo derecha, arriba y abajo | `neo_pweapon.c` 0644C4, `gameplay.c` | U | P4 | en curso (`work-m2fix1`) |
| B26 | Marco cruza el borde de las puertas en vez de pasar por dentro | atravesar una puerta de la M2 | Marco detrás del marco de la puerta | Marco delante del borde | prioridad +1E, capas | U | P5 | en curso (`work-m2fix1`) |
| B27 | Faltan premios en la M2 | jugar la M2 | premios del arcade (inventario MAME de las 3 etapas y el jefe: MISION2_ACTORES.md, «Premios») | faltan las monedas de la etapa 3 (tipo `56`, 47 registros) y el tipo `28` de las momias | `neo_item.c`: todo objeto de movimiento sin traducir se devolvía como externo | U, M, H: familia `coin` (13 nodos, 15523 actualizaciones, 6 recogidas con combo), píxeles 28193/0 | P4 | corregido en el PC `work-m2fix2`; paradas explícitas contadas por tipo (`MSX item kind`): `40`/`3D`/`3F` (andador de las momias y lo que suelta, sin captura), `59`, la recogida `09` (bomba de tipo 2) y el regalo de `032DE2`; falta MiSTer |
| B28 | No sale humo cuando Marco se convierte en momia | aliento de momia | humo `0C4DD8` (paleta B0, `2A97A0`, sonido 1085), después el marcador `0C5286` | no hay | `neo_landing.c` (nodo del humo), `gameplay.c` | U (= B13), M: `mz4` (`MSX_FULL_GIVE_MUMMY`), H: `test_neo_smoke` 33 actualizaciones / 4544 bytes, `smoke_done` en la 17.ª; píxeles 45983 iguales | P5 | corregido en el PC `work-m2fix2` (el cambio de paleta ya estaba: auditoría de `marco-mz2`); falta MiSTer |
| B29 | No sale el arma al coger el láser | coger la L en la M2 | el rayo `01E22C` se dibuja | no se ve | `filesystem/mission2/rest-native.json` de la ROM 22 anterior a `work-laser`: sin los 235 descriptores del rayo y del cohete 8 ni las paletas `98 99 9A 384 39B 822 828` | U, H: `check_mission_sprites.py` falla con la exportación de la ROM 22 y pasa con la nueva | P4 | corregido (datos privados regenerados y comprobación en `make check`) `work-m2fix2`; falta MiSTer |
| B18 | M3: el paseo se para en la cámara 1902 | M3 | sigue | se para | suelo de los vagones | A | P2 | en pausa (por orden del usuario) |

## Cobertura (P = probado, F = fallido, N = no implementado, ? = no probado)

| Sistema | M1 | M2 |
| --- | --- | --- |
| Pistola, H, C | P | P (fixtures) |
| R (lanzacohetes) | F (B04, al llegar al jefe) | ? |
| S/F/D (02/07, 04/09, 0D) | P (familias) | ? en juego real |
| Láser | — | P (familias, `pweapon`) |
| Muerte de soldados por F/S | P (B12) | ? en juego real |
| Forma momia | — | P (lógica) / F (dibujo, B10) |
| Vehículos | P | ? |
| Jefe | P | P (8 piezas en ares, ROM de referencia) |
| Transición entre misiones | F (B05) | — |
| Música | P | F al entrar desde la M1 (B06) |
| Eventos y oleadas | P | F (B09) |
| Reinicio tras morir / CONTINUE | P | ? |
