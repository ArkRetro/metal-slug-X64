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
| B08 | Faltan actores secundarios de la M2 (mineros, arqueólogos) | jugar la M2 | aparecen | no aparecen | `neo_world` (creadores no traducidos) | U | P4 | en curso (`work-m2events`) |
| B09 | El orden de los eventos de la M2 no es el del arcade | jugar la M2 | el orden de MAME | distinto | `neo_mission.c`, guion, lista de cámara | U | P3 | en curso (`work-m2events`) |
| B10 | Momia con el sprite de Marco superpuesto | recibir el aliento de la momia | solo la momia | dos cuerpos | `marco_visual.c`, forma 3 | U | P4 | en curso (`work-m2events`) |
| B11 | El láser (06/0B) dispara la bala de la pistola | coger la L en la M2 | rayo de varios segmentos | bala de pistola | parada en `gameplay.c` | H: cuenta en `weapon_stops` | P4 | abierto |
| B12 | Soldados muertos por fuego o explosión desaparecen | matar con F o S | animación de muerte (tipos 7, 0A, 0B) | desaparecen | `neo_soldier_ai.c` | H: `neo_soldier_death_stops` | P4 | abierto |
| B13 | Faltan los efectos de la transformación en momia (C4DD8, C5286) y el cambio de paleta | transformación | efectos | no hay | `gameplay.c`, render | doc | P5 | abierto |
| B14 | Caídas sueltas: M2 etapa 2 en x 2281, etapa 3 en x 348 y x 611 | `test_mission2_ares.py` | no hay caída | rescate de la ROM de referencia | `wall_stop` con el obstáculo dinámico del hueco 7 | A, M | P2 | abierto |
| B15 | Puerta de la etapa 2: no se dibujan sus tiles una vez abierta y falta el sonido 103B | final de la etapa 2 | tiles y sonido | no hay | render y audio | doc | P5 | abierto |
| B16 | Armas 01, 08, 0E y 0F | — | nativas | bala de pistola | `gameplay.c` | H | P4 | abierto (no salen en la M2) |
| B17 | Salto sin dirección después de correr: el arcade pone la velocidad horizontal a 0 | M | 0 | se conserva | `gameplay.c` | M | P4 | sospecha (rutina sin probar) |
| B18 | M3: el paseo se para en la cámara 1902 | M3 | sigue | se para | suelo de los vagones | A | P2 | en pausa (por orden del usuario) |

## Cobertura (P = probado, F = fallido, N = no implementado, ? = no probado)

| Sistema | M1 | M2 |
| --- | --- | --- |
| Pistola, H, C | P | P (fixtures) |
| R (lanzacohetes) | F (B04, al llegar al jefe) | ? |
| S/F/D (02/07, 04/09, 0D) | P (familias) | ? en juego real |
| Láser | — | N (B11) |
| Muerte de soldados por F/S | N (B12) | N |
| Forma momia | — | P (lógica) / F (dibujo, B10) |
| Vehículos | P | ? |
| Jefe | P | P (8 piezas en ares, ROM de referencia) |
| Transición entre misiones | F (B05) | — |
| Música | P | F al entrar desde la M1 (B06) |
| Eventos y oleadas | P | F (B09) |
| Reinicio tras morir / CONTINUE | P | ? |
