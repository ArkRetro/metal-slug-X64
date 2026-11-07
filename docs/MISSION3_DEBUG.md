# Misión 3: depuración

Punto de continuidad de la Misión 3. Se actualiza al final de cada sesión.
Referencias: `MISION3.md`, `MISION3_ACTORES.md`, `JEFE_MISION3.md`,
`PLAN_MISION3.md`, `BUG_MATRIX.md` (B18, B56–B58, B59).

## Estado (ROM 41, `ccr-09eb2f6b-8rwl8l`)

- **Carga:** tras la M2, o desde el menú de pruebas (L+R o Z+R y A en
  SOLDIER SELECT). Pack v4 con el bloque MSXD, 798 de 800 trozos.
- **Mapa y colisión:** 100 % frente a MAME en las 5 secciones (`test_m3_map`).
- **Actores:** todos los del recorrido de `run4` antes del jefe son nativos.
  Faltan las ramas al azar no vistas y `03C306` (granada, sin traza).
- **Jefe intermedio y final:** nativos (familias m3mid1..4, m3boss0..5).
  El fin de misión cae en la parada explícita "falta mission4.pack", que
  reinicia la M1.
- **Vehículos:** `093C9A` y `0AC866`. Arma E. Marco gordo y sus armas, salvo
  la bomba, el molotov y el polvo.
- **Fondo:** columnas pre-renderizadas en la capa 1 (`build/bgp/m3s1.bgp`).
  El resto va por la caché de piezas.

## Verificado

- PC: `make check` con todas las familias M3 bit-exact.
- ares: recorridos con la ROM de referencia (piloto, invulnerable) hasta el
  jefe.
- Ojo: la ROM de referencia **escondió** fallos de la ROM normal. Hay que
  probar siempre con `mslugx-explorer.z64` o con `mslugx-m3-release.z64`.

## Fallos abiertos (MiSTer, ROM 41, cartucho normal)

1. **Fondo de la M3 ausente** en una sección. Foto:
   `scratchpad/rom41-photos/m3-black-bg.jpg`. Solo se ven los sprites y la
   franja del suelo. En curso (rama `work-m3doors`).
2. **Puertas de los vagones con palanca** (B57): los soldados empujan la
   puerta para abrirla. Foto: `scratchpad/rom41-photos/m3-wagon-doors.jpg`.
   En curso, después del punto 1.
3. **Tren en el minuto 8:30 del vídeo:** no se reproduce en ares. Se espera
   a que muera el grupo 0..7 y luego el soldado del escudo `09AE2C`. El
   registro de la etapa imprime el tren y los grupos.

## Hipótesis

| Hipótesis | Evidencia | Estado |
| --- | --- | --- |
| H1: el anillo pre-renderizado deja columnas sin dibujar en el hardware | Si falta el archivo, `scene_prerender.c` vuelve al camino de piezas (registro `missing: tile path`). Si la lectura llega tarde, se lee en el acto. No se ha visto en ares. | abierta |
| H2: la sección de la foto usa capas o reglas que el pack no tiene (cielo dibujado por un escritor sin identificar, capa de superposición `0A6C8C`) | `MISION3.md`: el cielo a partir de la cámara 320 no tiene mapa de ROM | abierta |
| H3: falta de ranuras de caché en ese tramo | Con 210 ranuras al cargar, no medido en esa sección | abierta |

## Siguiente experimento (uno solo)

Con el cartucho normal y el menú de pruebas, entrar en la M3 en ares, ir a la
sección de la foto y registrar a la vez:
- `MSX BG`, la línea del prerender, `MSX CACHE` y las capas visibles;
- una captura de pantalla.

Comparar con MAME en la misma cámara. Con eso se aceptan o descartan H1, H2
y H3 antes de tocar código.

## Archivos tocados por la M3

`src/neo_m3props.c`, `neo_m3enemy.c`, `neo_m3mid.c`, `neo_m3veh.c`,
`neo_m3boss.c`, `neo_train.c`, `scene_prerender.c`, `scene_assets.h`,
`tools/pack_mission.py`, `tools/prerender_stage.py`.
