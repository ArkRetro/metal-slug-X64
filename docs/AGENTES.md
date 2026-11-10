# Normas comunes de los agentes (leer junto con CLAUDE.md)

- Entorno: `source /tmp/claude-0/-home-user-metal-slug-X64/804c143c-3a6f-5233-84b0-807dc4f0a010/scratchpad/env.sh`.
  Datos privados y árbol de las ROM: `/home/user/ws-logo` (packs con
  `tools/build_packs.py`). Estados del TAS: `/home/user/captures-tas/sync264/states/mslugx/`
  (`docs/TAS.md`).
- Tres agentes en paralelo: A = Misión 4 (`/home/user/ws-m4`, `work-m4`, Xvfb :71),
  B = Misión 5 (`/home/user/ws-m5`, `work-m5`, :72), C = personajes y pausa
  (`/home/user/ws-chars`, `work-chars`, :73). Fusionar ccr a menudo; cambios
  mínimos y probados en el motor compartido.
- Puertas antes de fusionar en ccr: `make check`, `make -j4`,
  `tools/test_m1m2_ares.py` con `mslugx-m1end.z64` recién hecho
  (`rm -rf build/m1end`), y la prueba corta en ares de la tarea. Empujar ccr
  tras cada fusión.
- ares: `/workspace/emulator/data146/ares/settings.bml` debe conservar
  L = `0x1/0/38` (tecla d) y R = `0x1/0/39` (tecla e); no quitar asignaciones.
- Disco compartido: mirar `df`; borrar trazas crudas tras usarlas.
- Procesos: parar por PID; nunca `pgrep -f` ni `pkill -f`.
- Datos de la ROM nunca en git. Sin nombres de modelo en los commits; todo
  commit (también las fusiones) termina con:
  `Co-Authored-By: Claude Fable 5.2 <noreply@anthropic.com>` y
  `Claude-Session: https://claude.ai/code/session_014oBkvYWFkZ4jtciR7AtEF3`.
- No lanzar subagentes. Hacer commits a menudo. Antes de agotar el contexto,
  actualizar el documento de la tarea (hecho, falta, siguiente paso).
- Informe final: 6 líneas como máximo.
