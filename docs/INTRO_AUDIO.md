# Intro completa y sonido: alcance y evidencia

La siguiente entrega solicitada es una demo jugable de **toda la Misión 1,
con intro entera y sonido**, comprobada en ares antes de MiSTer. No está lista.
No se publica el cartucho interno de audio ni la región parcial como esa demo.

## Referencia original sin saltar escenas

`tools/mame/run_intro_audio.py` ejecuta MAME 0.276 con BIOS euro, directorios de
configuración y NVRAM nuevos. Solo inyecta moneda, Start y confirmación de Marco.
No modifica RAM de juego ni mantiene A para saltar el tutorial. Las pulsaciones
quedan registradas por fotograma. En la captura completa, confirmar en 3620
ocurre después de observar la pantalla de selección, no durante el tutorial.

La captura conserva 4800 fronteras de vídeo, 2400 pares VRAM/paleta y 81,08 s de
PCM estéreo a 48 kHz. Las muestras visuales muestran:

| Fotograma observado | Contenido visible |
| --- | --- |
| 360–660 | Animación del título Metal Slug X |
| 780–3360 | Tutorial, incluido el tanque y sus controles |
| 3480–3600 | Selección de soldado |
| 3660–3780 | Marco confirmado y transición |
| 3900–4080 | Misión 1 y descenso en paracaídas |
| 4140 | Marco ya en el suelo |

Son observaciones de imágenes concretas; no establecen todavía el tick exacto
que habilita controles ni todos los límites de escena. Los fotogramas de captura
no se convierten en disparadores del combate nativo.

La VRAM se lee mediante el elemento guardado `:spritegen/0/m_videoram` de MAME.
Una primera captura que accedía al registro de dirección durante el autotest
alteraba la ejecución: se descartó. Esta herramienta no escribe los registros
`0x3C0000/0x3C0002` para leer vídeo.

El driver original verifica la escritura de comandos en `0x320000` y los cuatro
puertos YM2610 `0x04..0x07`, reflejados en el byte alto de la dirección Z80:
[MAME mame0276 neogeo.cpp](https://github.com/mamedev/mame/blob/mame0276/src/mame/neogeo/neogeo.cpp).
Los watchpoints registran 83 escrituras de comandos y 213371 escrituras YM en la
captura completa. Sus valores siguen siendo **sin significado semántico
asignado**: proximidad a una escena no basta para nombrar una música o efecto.
Los registros completos permanecen fuera de Git.

## Audio nativo implementado

`src/msx_audio.c` utiliza `audio_init`, `wav64`, el mezclador por RSP y buffers
AI. No ejecuta Z80 ni sintetiza un YM2610 en tiempo real. Los archivos WAV64 se
leen desde ROM bajo demanda. Se reservan ocho voces mediante pares de canales
para poder admitir estéreo: intro, música, locución y cinco efectos.

La salida solicitada es 32 kHz; el DAC utiliza la frecuencia realizable por la
consola. Cada canal tiene un límite de 8192 bytes de caché de muestras: hasta
128 KiB entre los 16 canales, sin incluir buffers AI ni metadatos de recursos.
Se mezclan bloques de hasta 1024 muestras estéreo para conservar el margen de
VADPCM; un buffer AI mayor se divide. `msx_audio_service()` envía como máximo
dos buffers por llamada, sin esperar cuando no hay espacio. Debe llamarse fuera
del trabajo RDP pendiente. Las estadísticas cuentan muestras generadas, **no**
el instante exacto ya reproducido por el DAC.

El banco abre metadatos por voz, conserva recursos entre reproducciones y permite
fin natural, bucle, parada y cierre. La selección de efectos, prioridades,
interrupciones y sincronización con la lógica original aún no están integradas.
La disponibilidad de ocho voces no demuestra rendimiento con todas activas.

La prueba usa un fragmento mezclado del título original, convertido mediante
`audioconv64` a VADPCM estéreo de 32 kHz. No es una biblioteca de efectos aislados
ni una música de misión con bucle recuperado. El recorte usa los tiempos
registrados por MAME, no una división de fotogramas por 60.

En ares se comprueban dos voces estéreo, PCM no nulo, EOF, repetición, parada,
silencio posterior, 24 buffers enviados al AI y 45 presentaciones RDP mientras
suena la referencia. La prueba no es una captura acústica ni una medición de
rendimiento del escenario completo. Ver [audio-native-validation.json](audio-native-validation.json).

## Reconstrucción gráfica y diferencia pendiente

`tools/render_intro_reference.py` reconstruye fuera de la N64 los gráficos desde
C-ROM/S-ROM, paletas, encadenamiento, selección de líneas y las tablas originales
de zoom. Las capturas PNG sirven para comparar, nunca como texturas del port.
El comportamiento se contrasta con `neogeo_spr.cpp` de MAME 0.276; se conserva su
atribución en `LICENSES/MAME-NeoGeo-BSD-3-Clause.txt`.

Las once imágenes contrastadas coinciden en los 304×224 píxeles activos,
con tolerancia RGB máxima de una unidad. Es una comparación RGB8 de extracción,
no del framebuffer N64 cuantizado a RGBA5551.

En el fotograma 360, una instantánea de VRAM al final del frame no bastaba: la
traza contiene 137 escrituras de datos de vídeo entre fronteras 359 y 360,
algunas durante líneas visibles. `capture_intro_raster.lua` observa los puertos
`0x3C0000..0x3C0005`, su offset VRAM previo y el tiempo de pantalla. La línea se
deriva de `time_until_pos(0)`, `scan_period` y `frame_period`, APIs verificadas
en `luaengine.cpp` de MAME 0.276. No hay un método Lua `screen:vpos()` en esta
versión; la captura de prueba que intentaba usarlo se descartó y el ejecutor
ahora rechaza cualquier referencia con errores Lua.

El reconstructor aplica esas escrituras a la VRAM inicial y exige que cada
palabra previa coincida con la traza. También exige que el resultado final sea
idéntico al volcado siguiente y que la paleta no haya cambiado. Para cada línea,
la lista de sprites se selecciona con el estado inicial; los píxeles usan el
estado posterior a las escrituras de esa línea, conforme al orden del timer de
sprites de MAME. Esa distinción explica el último píxel que aún difería después
de introducir las escrituras por línea. No se copian píxeles de la captura ni
se utiliza una máscara para aceptar discrepancias.

La fase automática utiliza el contador registrado en la siguiente frontera de
frame, que coincide en estas muestras. La sonda de raster cubre un intervalo
específico del título, y el reconstructor supone autoanimación habilitada en el
perfil contrastado. No se afirma cobertura de todos los frames, modos o cambios
de paleta por línea de la intro.

Esta reconstrucción todavía no es un reproductor de intro en N64. Faltan la
conversión a recursos y comandos nativos acotados, la reproducción sincronizada,
la transición al controlador del jugador y la integración del sonido de juego.
También siguen pendientes daños, encuentros, eventos, resto del mapa y jefe para
completar la Misión 1. El frontend activo continúa siendo la región provisional.

## Reproducir en el entorno retenido

```sh
python3 tools/mame/run_intro_audio.py --rom-dir /ruta/roms \
  --output /ruta/nueva-captura --end-frame 4800 --confirm-frame 3620
python3 tools/prepare_audio_reference.py /ruta/nueva-captura
make audio-regression.z64
python3 tools/test_audio_ares.py audio-regression.z64 --output /ruta/nueva-prueba
python3 tools/mame/run_intro_audio.py --rom-dir /ruta/roms \
  --output /ruta/nuevo-raster --end-frame 400 --video-step 1 \
  --script tools/mame/capture_intro_raster.lua
python3 tools/render_intro_reference.py /ruta/nueva-captura \
  --raster-reference /ruta/nuevo-raster \
  --bios /ruta/roms/neogeo.zip --output /ruta/nueva-comparacion
```

Sin la sonda de raster, la comparación del frame 360 sigue fallando, como debe.
Los assets, WAV, ROMs de prueba, VRAM/paletas y volcados completos permanecen locales.
