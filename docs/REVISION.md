# Revisión de trabajo y optimización (11-XI)

Fuente: rama `ccr-09eb2f6b-8rwl8l` en `ea69b0e`, unión de prueba con
`work-chars` (rama `review-int` en `ws-logo`) y los documentos de cada misión.

## 1. Integración

| Rama | Por delante de ccr | Unión de prueba con ccr |
| --- | --- | --- |
| work-chars (tamaño, duplicados) | 16 commits | limpia; cartucho 60 833 792 bytes (6,2 MB bajo 67 043 328) |
| work-m4 (programs.bin, vehículo) | 3 commits + unión en curso | 5 conflictos: Makefile, data/soldier_programs.json, src/neo_region.c, src/scene_assets.h, tools/extract_soldier_programs.py |
| work-m5 | 1 commit | (agente B en curso) |

Paquetes con ccr + work-chars: M2 366, M3 821, M4 895, M5 1066, M6 98
fragmentos. El límite `SCENE_MISSION_CHUNKS_MAX` es 1088: a la M5 le quedan
22 fragmentos.

`make check` sobre la unión: **falla** en `tools/test_pack_compression.py`: la única diferencia entre los volcados del formato antiguo y del comprimido son 3 líneas `blob 2d388e served=0` / `served=1`; tablas, imágenes y páginas iguales. Asignado al agente de tamaño, que debe arreglar la causa antes de subir (sin rebajar la prueba).

## 2. Lo abierto, por documento

| Documento | «pendiente» | sin captura / sin verificar |
| --- | --- | --- |
| APROXIMACIONES.md (A1–A17) | 14 | — |
| MISSION3_DEBUG.md (G1–G25) | 2 | 3 |
| MISION4.md | 8 | 5 |
| MISION5.md | 12 | 1 |
| MISION6.md | 4 | 0 |
| PERSONAJES.md | 5 | 1 |
| PAUSA.md | 0 | 1 |

Aproximaciones aún abiertas (A4, A6–A15, A16, A17): cada una debe acabar
traducida desde su rutina o registrada como imposible de capturar.

## 3. Optimización del trabajo

1. **Unión temprana.** El coordinador prueba la unión de cada rama de agente
   con ccr en `ws-logo/review-int` antes de que el agente la suba; los
   conflictos se avisan en cuanto aparecen, no al final del lote.
2. **Orden de subida.** Primero el tamaño del cartucho (sin él no hay pruebas en
   ares con la M6 dentro), después la memoria de programa (`programs.bin`), y
   después las misiones.
3. **Un solo `make check` por lote**, con `-pipe`, en el árbol del agente; el
   coordinador repite solo la unión.
4. **Holgura**: la M5 está a 22 fragmentos del límite y la caché de escena
   a 1 hueco del mínimo; ninguna subida puede bajar un mínimo para caber.
   Primero se buscan duplicados (`docs/COMPRESION.md`, «Duplicados»).
5. **Disco**: worktrees viejos retirados (con permiso), capturas idénticas
   enlazadas; las trazas en bruto se borran al tener su fixture.
6. **Pendiente de captura**: lo traducido sin captura (M4: puñetazo 0DDF14,
   salida 0DE064, frutas 3D/3F, 289A80; pausa: tres casos; voz de Tarma 1087)
   se captura en lote desde los estados del TAS en una sola sesión de MAME por
   misión, no una por rutina.
