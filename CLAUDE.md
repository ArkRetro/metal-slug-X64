# Reglas del port (obligatorias para toda sesión y agente)

- No inventar recursos ni lógica. Todo sale de la ROM del usuario: código 68K
  traducido rutina a rutina y datos (sprites, paletas, mapas, tablas, sonido)
  extraídos de la ROM o de capturas de MAME 0.264 de esa ROM.
- Nada de cálculos aproximados: ni constantes elegidas porque "coinciden casi
  siempre", ni desfases, tiempos o cadencias ajustados a ojo. Si un valor no
  está en la ROM o en una captura, se desensambla la rutina que lo produce y se
  traduce. Si falta la captura, se hace; si no se puede, se registra como
  pendiente (no se rellena).
- Cada cambio se prueba bit a bit contra MAME (familias, `make check`).
- Un solo agente cada vez. ROM y datos derivados nunca en git.
- Protocolo de la M3: `docs/MISSION3_DEBUG.md`.
