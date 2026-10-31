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
| Marco | movimiento, pistola, H (barridos), C (proyectil buscador), R, granadas, cuchillo con pose, disparo abajo en el aire, muerte/reaparición | tabla de cuchillo 0BF918 sin captura |
| Vehículos | Camel Slug y tanque SV-001 montables | reacciones de daño del SV-001 sin captura |
| Enemigos y actores | soldados, camión, helicópteros, lanzacohetes, camelleros, civiles, prisioneros, premios | rótulo de bombas 08C752, puesto de fruta 03FC5A |
| Jefe | completo (torreta, fases, explosiones, prisioneros de la escotilla que caen al suelo) | — |
| Fin de misión | MISSION COMPLETE, recuento de prisioneros | textos japoneses, 2 jugadores, medallas |
| Continuar | CONTINUE? con cuenta atrás | objeto 019798 |
| Marcador | HUD original en modo consola doméstica (PRESS START, sin CREDIT) | — |
| Sonido | música y 57 efectos originales residentes en RAM | — |
| Capas | orden por prioridad +1E como el arcade | — |

## Rendimiento (ares)

Peor fotograma del recorrido 35–39 ms (objetivo 16,7 ms); 0 fallos síncronos del
fondo; detalle en RENDIMIENTO.md.

## Historial de ROMs de prueba

| ROM | Contenido principal |
| --- | --- |
| 9 | Misión completa, Camel Slug, torreta del jefe |
| 10 | Logo NEOGEO, arma C, civiles, explosiones, premios, sonidos, Expansion Pak |
| 10b–10d | sonido sin distorsión, título sin cortes, balas H/C orientadas, recuento, CONTINUE? |
| 11 / 11b | SV-001, HOW TO PLAY, SOLDIER SELECT, tejado, capas, sin INSERT COIN, sin HUD de depuración |
| 12 (en curso) | aterrizaje, descenso jugable, MISSION 1 START, cuchillo con pose, lanzacohetes, brazo de granadas |

## Después de la Misión 1

Misiones 2–6 (herramientas de captura generalizadas en `work-fullgame`) y
efecto diorama (en pausa, `work-diorama`).
