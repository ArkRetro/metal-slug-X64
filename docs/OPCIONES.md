# Menú de opciones antes de jugar (petición del usuario, 11-XI)

## Lo que trae la ROM (cabecera de `250-p1.p1`, tablas de «soft DIP»)

Punteros en 0x116 (JP, 0x3388), 0x11A (US, 0x2F78) y 0x11E (EU, 0x316E).
El BIOS copia la elección a la RAM `10FD84..` y el juego la lee de ahí.

| Ajuste (texto de la ROM) | Valores | EU por defecto (byte de lista) | RAM que lee el port |
| --- | --- | --- | --- |
| HERO (vidas, ajuste de cuenta) | número | 3 (`03`) | `10FD88` |
| CONTINUE | OFF / ON | ON (`12`) | `10FD8A` |
| DIFFICULTY | LEVEL 1..8 | LEVEL 4 (`38`) | `10FD8B` (rango) |
| PLAY TIME | 60 / 70 / 80 / 90 | 60 (`04`) | `10FD8C` |
| DEMO SOUND | OFF / ON | ON (`12`) | `10FD8D` (sin verificar) |
| PLAY MANUAL | OFF / ON | ON (`12`) | `10FD8E` (sin verificar) |
| BLOOD | OFF / ON | OFF (`02`; JP `12`: ON) | `10FD8F` |

Byte de lista: nibble alto = opción por defecto, nibble bajo = número de
opciones. Cabecera EU completa: `ff ff ff ff 03 ff 12 38 04 12 12 02 01 24 14 02`.

**Idioma:** no hay ajuste de idioma. El texto solo depende de la región del
BIOS (JP: japonés; US/EU: inglés). No hay textos en español en la ROM: no se
añaden (regla de no inventar).

## Diseño (añadido N64, sin tocar la lógica del juego)

- Pantalla de opciones antes de SOLDIER SELECT (y accesible desde el título),
  con exactamente los ajustes y textos de la tabla de arriba (cadenas de la
  ROM, fuente fija de la ROM).
- Cada opción escribe en su byte de RAM lo mismo que escribiría el BIOS; las
  rutinas del juego que ya leen `10FD88..10FD8F` no cambian.
- Las direcciones marcadas «sin verificar» se confirman con una captura de
  MAME cambiando el soft DIP (o desensamblando quién las lee) antes de usarse.
- Guardado opcional en EEPROM del cartucho (libdragon), si cabe sin bajar
  mínimos de memoria.
- Región de texto (JP/EN) solo si el port ya dibuja el texto desde la ROM con
  la región elegida; si no, se registra como pendiente.
