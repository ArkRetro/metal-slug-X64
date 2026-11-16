# Plan final del port (16-XI)

Dos agentes a la vez (los reinicios de la máquina aumentan con más carga),
`make -j2`, commit y push en cuanto algo pasa sus pruebas.

## Fase 1: integración (agente I, ws-m4)
Unir en `ccr` work-m4 (f5d4f0a), work-m5 (e13b3d9) y work-chars (c952bea:
menú de opciones + M6). Fallos conocidos de la unión: m6s1 con 1312 dibujos
ausentes (exportación M6 desfasada), m5boss 8 dibujos, m4boss `02058E`
(trampilla). Matriz completa en verde, push. Luego M4: torre del terreno
delante del casco del jefe y el Slugnoid bajo fuego (B69).

## Fase 2: Misión 6 hasta el final (agente C, ws-chars)
040920 con prueba bit a bit, último enemigo 04153E (45 constructores, captura
por ventanas), créditos y final del juego.

## Fase 3: ROM 52
Solo con todos los jefes derrotables y la matriz en verde.

## Fase 4: cierre de fidelidad
Repaso de `APROXIMACIONES.md` (A4..A17), huecos de la M3, voz de Tarma 1087,
menú casero AES 065E80, recorrido completo M1→fin en ares y en MiSTer.
