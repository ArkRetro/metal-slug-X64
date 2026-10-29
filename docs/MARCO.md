Estado actual y trabajo prioritario: [conjunto completo de animaciones de Marco](MARCO_ANIMATIONS.md).
La extracción descrita abajo es histórica; no constituye el catálogo completo.

# Marco: extracción v4

Se reconstruye el personaje con los datos originales de Metal Slug X:

- MAME 0.276, set `mslugx`, BIOS `euro` y misión 1.
- Capturas de VRAM y paleta activa por el bus 68000, no sprites elegidos a ojo.
- Selección por paleta 0x63 en esta escena; se registran slots, coordenadas,
  índices de tile y atributos en `filesystem/marco.json`.
- SCB2/3/4 resuelven posición y columnas encadenadas; SCB1 resuelve tiles,
  paleta y flips. El extractor rechaza zoom o autoanimación no implementados.
- Conversión de color con el modelo de resistencias Neo Geo documentado por
  MAME; cuantización final al RGBA5551 de N64.
- 30 muestras de reposo y 30 de carrera, tomadas cada cuatro frames del original.
  Las muestras pueden repetir poses. No equivalen a 60 dibujos únicos.

Antes de exportar, cada píxel opaco reconstruido se compara con la imagen
independiente producida por MAME: 39.344 coincidencias, con tolerancia máxima
de 1 por canal. La imagen de MAME solo sirve de referencia: el asset se genera
desde ROM C, VRAM y paleta, con transparencia en el índice cero.

Los frames se alinean por centro inferior dentro de 48x64, con los pies en y=56.
Este pivote es provisional, no se ha recuperado el pivote lógico del programa.
La secuencia captura una ventana temporal del juego, no una tabla completa de
animación. La dirección izquierda se obtiene reflejando horizontalmente.

## Repetir la extracción

Requisitos adicionales del host: MAME 0.276 con Lua y Python con Pillow.
En el entorno actual MAME está en `/workspace/emulator/root/usr/games/mame`;
sus bibliotecas se encuentran en el `usr/lib/x86_64-linux-gnu` de ese mismo root.
El script de instalación del SDK no instala MAME ni Pillow en otra máquina.

Usar los ZIP propios `mslugx.zip` y `neogeo.zip` dentro del directorio roms.
No añadirlos a Git. Crear directorios independientes de salida, cfg y nvram.
El juego debe arrancar desde BIOS, no desde un savestate desconocido.

```sh
mkdir -p /ruta/capturas /ruta/cfg /ruta/nvram
MSX_CAPTURE_DIR=/ruta/capturas mame mslugx -rompath /ruta/roms -bios euro \
  -video soft -sound none -window -skip_gameinfo -nothrottle \
  -autoboot_delay 0 -autoboot_script "$PWD/tools/capture_marco.lua" \
  -cfg_directory /ruta/cfg -nvram_directory /ruta/nvram
python3 tools/export_marco.py /ruta/capturas
make -j4
```

El Lua introduce moneda, pulsa START, confirma a Marco y luego mueve al personaje.
Si la selección o el arranque no coinciden, revisar las capturas: el exportador
no debe usarse con imágenes de otra escena. Se verificó la extracción en el
entorno actual; no se afirma compatibilidad con otras versiones de MAME.

`marco.bin` lleva cabecera MSXA, dimensiones, número de muestras por animación
y duración de muestra, todos los enteros big endian. El runtime valida formato,
límites y tamaño. Guarda los frames en RAM (360 KiB); no requiere las ROM P,
BIOS ni MAME durante la ejecución en N64.

## Estado de la demo

El movimiento lateral, la velocidad de dos píxeles por actualización, el suelo
y el cambio entre reposo/carrera son una implementación de prueba. No son la
lógica original del juego. El siguiente paso será añadir disparo y salto,
identificar pivotes y extender las animaciones antes de montar una escena real.
