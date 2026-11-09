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
