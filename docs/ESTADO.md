# Estado del port (Metal Slug X → Nintendo 64)

Resumen único y ordenado. El detalle técnico de cada parte está en el documento
enlazado. Requiere Expansion Pak (8 MiB). ROMs, capturas y assets derivados son
privados y no están en git.

## Arranque (como el arcade)

| Parte | Estado | Documento |
| --- | --- | --- |
| Logo NEOGEO 3D + música de la BIOS | nativo, 60 fps; el título se prepara en su tiempo libre | LOGO_NEOGEO.md |
| Título METAL SLUG X64 | cinemática de 601 fotogramas iguales a MAME | INTRO_AUDIO.md |
| HOW TO PLAY | cinemática (fotogramas 770..3459 de la captura con moneda) | MISSION_ENTRY.md |
| SOLDIER SELECT | cinemática que espera A/Start sobre Marco (solo Marco) | MISSION_ENTRY.md |
| Entrada en paracaídas | cinemática + traspaso nativo; tela y marcador 1P nativos; descenso jugable nativo desde 3860 (deriva, disparo/granada colgado, corte con A/B); letras MISSION 1 START y poses colgada/caída nativas | MISSION_ENTRY.md, MISION1_COMPLETA.md |

## Misión 1

| Área | Nativo | Pendiente / paradas explícitas |
| --- | --- | --- |
| Recorrido, cámara, guion, colisiones | completo de inicio a fin | — |
| Marco | movimiento, pistola, H (barridos), C (proyectil buscador), R, lanzacohetes 08 (prisionero antes del jefe), armas 0D, 02/07, 04/09 (con poses y sonidos), granadas, cuchillo con pose, disparo abajo en el aire, muerte/reaparición, momia (forma 3) con sus poses | tabla de cuchillo 0BF918 sin captura; armas 01, 06/0B (rayo), 0E, 0F; muertes de soldado por filas de tipo 7/0A/0B |
| Vehículos | Camel Slug y tanque SV-001 montables | reacciones de daño del SV-001 sin captura |
| Enemigos y actores | soldados, camión, helicópteros, lanzacohetes, camelleros, civiles, prisioneros, premios | rótulo de bombas 08C752, puesto de fruta 03FC5A |
| Jefe | completo (torreta, fases, explosiones, prisioneros de la escotilla que caen al suelo) | — |
| Fin de misión | MISSION COMPLETE, recuento de prisioneros, fundido, carga de la Misión 2 con su música, fundido de entrada y MISSION 2 START (MISION2.md, «Transición») | textos japoneses, 2 jugadores, medallas |
| Continuar | CONTINUE? con cuenta atrás | objeto 019798 |
| Marcador | HUD original en modo consola doméstica (PRESS START, sin CREDIT) | — |
| Sonido | música y 57 efectos originales residentes en RAM | — |
| Capas | orden por prioridad +1E como el arcade | — |

## Rendimiento (ares)

Peor fotograma del recorrido 35 ms en ROM 12 (objetivo 16,7 ms; como mucho dos
pasos de simulación por fotograma); 0 fallos síncronos del fondo; caché de
212 ranuras (mínimo 208); detalle en RENDIMIENTO.md.

## Historial de ROMs de prueba

| ROM | Contenido principal |
| --- | --- |
| 9 | Misión completa, Camel Slug, torreta del jefe |
| 10 | Logo NEOGEO, arma C, civiles, explosiones, premios, sonidos, Expansion Pak |
| 10b–10d | sonido sin distorsión, título sin cortes, balas H/C orientadas, recuento, CONTINUE? |
| 11 / 11b | SV-001, HOW TO PLAY, SOLDIER SELECT, tejado, capas, sin INSERT COIN, sin HUD de depuración |
| 12 (en curso) | aterrizaje, descenso jugable, MISSION 1 START, cuchillo con pose, lanzacohetes, brazo de granadas |

## Después de la Misión 1

Misión 2, fase 1 (rama `work-m2`, [MISION2.md](MISION2.md)): flujo de
misión nativo (`01829E..01891E`, probado contra la captura completa), datos
de las tres etapas exportados, paquete por misión y auditoría. Fase 2, motor
(rama `work-m2eng`): tras la Misión 1 el juego carga `romfs/mission2.pack`
(tablas de una etapa en ranuras de la caché, sin tocar el presupuesto de
RAM), con su guion, lista de cámara (incluido el `op 4` vertical) y reglas
de capa por etapa. En ares (`tools/test_mission2_ares.py`, cartucho de
referencia de la Misión 2): la etapa 1 se recorre hasta el final de su lista
(cámara 320), el gancho de referencia termina la etapa (el actor de salida
`0B6F24` no está portado) y la etapa 2 carga y avanza hasta la espera `op 4`
de la cámara 752; ahí para: el jugador no tiene aún el contacto lateral con
paredes del original (`+15` bits 3/4 de los barridos `0143EE/014594`), cruza
el bloque de x 944 y cae al foso de x 992.Fase 2, actores y datos (rama `work-m2act`, [MISION2_ACTORES.md](MISION2_ACTORES.md)): transiciones
`3AED8`/`3AEF0`, flecha GO `05287C` y marcador `0B6F6A` nativos; rejilla de
la etapa 2 completa desde la ROM. Misiones 3–6 y efecto diorama (en
pausa, `work-diorama`) después.

ROM 21 (rama `work-m1m2flow`, informe de ROM 20): lanzacohetes `08` del
prisionero de la cámara 3224 (ARMAS_VEHICULOS.md, Paso 15); transición
Misión 1 → 2 como el arcade (`1805C`, fundido de entrada, letras, música
`0022`) y suelo de la etapa 1 con el desplazamiento de MAME (MISION2.md).
