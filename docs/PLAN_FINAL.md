# Plan final del port (rehecho el 19-XI)

Reglas: `CLAUDE.md` y `docs/AGENTES.md`. Tres agentes como máximo, cada uno
con su worktree, su rama y su pantalla Xvfb. El coordinador integra, monta las
ROM y avisa al usuario.

## Estado en ccr (20c330c)
- Misiones 1 a 5 jugables, con sus jefes derrotables en ares.
- Misión 6: etapas 1 y 2 hasta su final en ares.
- Auditoría de daño y muerte: los cuatro personajes, con sonidos.
- Menú de opciones y paquetes de personaje v2.
- `make check` en verde, CI `core` en verde, matriz de ares con línea base.

## Flujo de trabajo

Cada tarea pasa por los mismos pasos, sin saltarse ninguno:

1. Captura de MAME 0.264 de la ROM del usuario, por ventanas pequeñas.
2. Traducción rutina a rutina.
3. Familia bit a bit en `make check`.
4. Prueba corta en ares.
5. Commit y push a la rama del agente.

Después, el agente de integración une las ramas en ccr con la matriz completa
(`tools/regression_ares.py --m1end`), en una sola pasada por tanda. Una
regresión no se tapa cambiando la línea base: se arregla, o se apunta como
fallo conocido en `BUG_MATRIX.md`. Lo crudo se borra en cuanto su fixture está
en `data/`.

| Carril | Árbol, rama y Xvfb | Trabajo, en orden |
| --- | --- | --- |
| Integración | ws-m4, work-m4, :71 | 1. Unir work-chars (bcc3849 y lo que siga), work-audit (c777140: humo 05A0B4) y work-mem. 2. Matriz completa y push a ccr. 3. Repetir por tandas. |
| Memoria | ws-m5, work-mem, :95 | 1. Liberar ≥25 KB, sin pérdida y sin bajar del mínimo de la caché de escena (3332 KB), con superposiciones de código por misión. 2. Encender `MSX_M6BOSS` en N64. 3. `check_icache.py` y la matriz completa. |
| Final del juego | ws-chars, work-chars, :94 | 1. Muerte del jefe 04180A (familia desde el TAS). 2. Pelea aérea 037404. 3. Proyectil 08F1A0. 4. Manejadores de entrada de la etapa 3. 5. Huida 017624. 6. Créditos y vuelta al título. 7. Prueba ares m6boss derrotable. |

Cuando un carril quede libre, se le asigna lo siguiente de esta cola, por orden:

1. Fallos abiertos de `BUG_MATRIX.md`:
   - B14: caídas de la M2. Antes hay que traducir los estados de giro y de
     andar hacia atrás (0C2746..0C27FA, B17).
   - B37: puerta de la pirámide.
   - B03: tirón al empezar.
   - Paleta 0x86F del jugador gordo con la F.
2. Auditoría:
   - Muerte en el agua de Tarma, Eri y Fio.
   - Pilotos del Slugnoid con Eri y Fio.
   - Eri en el tanque en cada fotograma.
   - Los 7 px del SV-001.
   - 37 diferencias de sonido (reparto de canales del chip, 4422).
3. Huecos de la M6 sin captura:
   - Paletas 214, 852 y 860.
   - Parches 0BDE16 y 0BDEEA.
   - Rutinas 032AA4 y 0406B6.
4. Aproximaciones A4..A17 (`APROXIMACIONES.md`): cada una se sustituye por la
   rutina de la ROM que produce el valor.
5. Huecos de la M3 y menú casero AES 065E80.

## ROM 52
Solo se monta cuando se cumple todo esto:
- Todos los jefes se pueden derrotar, el último incluido.
- El juego llega a los créditos en ares.
- La matriz está en verde.
- No queda ningún fallo abierto que pare el juego.

Montaje, en ws-logo:
1. Copiar las exportaciones como copias, nunca como enlaces. El catálogo de
   Marco y los paquetes `romfs/chars` van juntos.
2. Generar `softdip.bin`.
3. `build_packs.py`, `make` y `m1end`.
4. Matriz completa.
5. Empaquetar en `downloads/prueba52` y borrar el zip 51.

## Cierre
- Recorrido completo M1 → final en ares y en el MiSTer del usuario.
- Revisión final de `BUG_MATRIX.md` y `APROXIMACIONES.md`, las dos sin
  entradas abiertas.
