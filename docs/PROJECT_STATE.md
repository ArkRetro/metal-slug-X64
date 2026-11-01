# Estado del proyecto (punto de entrada para una sesión nueva)

- **Qué hay hecho y qué falta en el juego:** `docs/ESTADO.md` (arranque,
  misión 1 completa, misión 2 en curso).
- **Arquitectura:** `docs/N64_ARCHITECTURE.md`, `docs/NATIVE_BACKEND.md`;
  rendimiento de CPU/RDP y presupuesto de la caché de escena en
  `docs/RENDIMIENTO.md`.
- **Memoria y ROM:** `docs/MEMORY_OPTIMIZATION.md` (auditoría medida, los cinco
  problemas mayores, plan por impacto/riesgo, resultados y siguiente paso).

## Decisiones vigentes de memoria

- 8 MiB obligatorios; todas las funciones del Expansion Pak se conservan. No
  se persigue un perfil de 4 MiB (motivo en MEMORY_OPTIMIZATION).
- La caché de escena se dimensiona la última con todo el montón libre menos
  reservas medidas (juego 640, en juego 320, margen **256 KB**); mínimo 208
  ranuras de 16 KiB.
- Datos de cada misión en `missionN.pack` (DFS), cargados en ranuras
  reservadas de la caché al cambiar de fase; nunca tablas de misión en el ELF.
- Código de actores y jefes `-Os` (`COLD_OBJS` del Makefile); el núcleo por
  fotograma (`HOT_SRCS`) -O2.
- Películas en ROM como MSXC v2 (LZ4 por paquete), fotogramas idénticos a v1.

## Problemas abiertos

- Pico de RAM de las cinemáticas (película descomprimida en caché +
  fotogramas preparados, ≈5,2 MB en el título): plan #3.
- `tools/test_logo_ares.py` espera `MSX mission handoff x=`; el juego imprime
  `MSX mission handoff (descent) x=`.
