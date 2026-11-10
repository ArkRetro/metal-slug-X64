# Plan de ataque: M3 completa y luego M4–M6

Reglas: `CLAUDE.md` (sin recursos inventados ni aproximaciones; todo de la ROM
del usuario y de capturas de MAME 0.264; un agente cada vez).

## Fase 0: exactitud (en curso)
Auditoría de aproximaciones → `docs/APROXIMACIONES.md`. Cada caso se cambia por
la traducción de su rutina 68K con prueba bit a bit (onda del agua +6D, hundido
de Marco en el agua de la M2, cadencia de las armas finas, ...).

## Fase 1: capturas completas con el TAS
- Fuente: TAS de 1 jugador de TASVideos (#6663, BizHawk 2.9.1, núcleo MAME,
  arranque en frío, RTC 2010-01-01, 57035 fotogramas, 6 misiones). Privado en
  `/home/user/tas/` (no va a git).
- Lector Lua para MAME 0.264 que inyecta `Input Log.txt` fotograma a fotograma
  desde el encendido. Comprobación de sincronía: monedas/Start, MISSION n
  START y MISSION COMPLETE en los fotogramas esperados, y RAM de puntuación.
- Si sincroniza: estados guardados al inicio de cada misión, cada jefe y cada
  vehículo, y capturas de familias por tramos, sin tocar la RAM.
- Si no sincroniza: localizar el primer fotograma divergente (BIOS, reloj,
  versión) y corregir el arranque; si es imposible, registrarlo y seguir con
  capturas propias.
- **Estado (`docs/TAS.md`):** lector y reloj fijo hechos; el TAS no sincroniza
  en MAME 0.264 (juega bien hasta ~4316, el grupo de helicópteros de la M1;
  fin de partida en 10469). Causa probable: BizHawk 2.9.1 usa MAME 0.252 y la
  ralentización del bucle de 30 Hz depende de los ciclos del 68000. Pendiente:
  comparar con 0.251/0.252 (ejecutar ese binario necesita permiso). Se sigue con
  capturas propias (`run4`).

## Fase 2: cerrar la M3 (en este orden)
1. Jefe final que no ataca (cartucho normal, sin piloto).
2. Fondos inestables detrás del tren en marcha.
3. Música de la M3: identificar las palabras de sonido en la captura,
   renderizar desde la ROM, asignar.
4. Resto de la lista G de `MISSION3_DEBUG.md` (G15, G21, G23, ...).
5. Comparación de la M3 entera con la captura del TAS: actores por cámara,
   paradas explícitas = 0, dibujos ausentes = 0, fondos por sección.
6. ROM de prueba al usuario; la M3 se da por cerrada con su visto bueno en
   MiSTer.

## Fase 3: M4, M5 y M6 (una tras otra, mismo método que la M3)
Por misión:
- captura del TAS;
- mapa, colisión y cámara (`test_mN_map` al 100 %);
- guion y apariciones;
- actores nativos con familias bit a bit;
- vehículos y armas nuevas;
- jefe;
- música;
- `missionN.pack` y enlace desde la misión anterior;
- prueba corta en ares;
- ROM al usuario.

## Estado
Se actualiza al final de cada fase aquí y en `MISSION3_DEBUG.md`.

## Fase 2b: compresión y datos repetidos (antes de la M4)
Objetivo: que quepan las misiones 4–6 en ROM y en RAM sin perder exactitud
(todo sin pérdida; los datos siguen siendo los de la ROM del usuario).
1. Medir: tamaño por sección de cada `missionN.pack`, de `filesystem/` y del
   audio; RAM por tabla al cargar. Informe en `docs/COMPRESION.md`.
2. Detectar repetidos por hash: piezas (tiles), paletas, programas de
   sprites, bloques de mapa y tablas MSXD iguales entre misiones y dentro de
   cada una; wav64 iguales o contenidos en otros.
3. Deduplicar: un banco común (`common.pack`) para lo compartido entre
   misiones y referencias dentro de cada pack; el cargador resuelve las
   referencias.
4. Comprimir sin pérdida lo que queda (el códec que ya trae libdragon, por
   bloques, descompresión durante la carga, sin coste por fotograma).
5. Puertas: los datos descomprimidos son idénticos byte a byte a los de hoy
   (prueba en `make check`), `make -j4`, pruebas cortas en ares, y que los
   tiempos de carga y las ranuras de caché no empeoren.

## Fase 3 (orden): M4 → M5 → M6 (última), con las capturas del TAS.

## Estado de la Fase 3

- **M4, fase 1 hecha** (`work-m4`, [MISION4.md](MISION4.md)): `run4` ya tenía la
  M4 entera; estados por sección, vehículo y jefe en `/home/user/captures-m4`;
  mapa y colisión de la ROM (`export_stage_rom.py`), `test_m4_map` 11 540/11 540
  y `test_m4_camera` 4314/4314 (+ 15 798 estados de capa) iguales a MAME;
  `mission4.pack` enlazado tras la M3 y en el menú; inventario de 151
  constructores (87 nativos, 56 nuevos) en [MISION4_ACTORES.md](MISION4_ACTORES.md).
  Siguiente: fase 2 (parches `0C70FC`, llamadas `0BA8xx/0BA9xx`, actores
  nuevos, jefe `0BB028`, música 0x25/0x2C).
