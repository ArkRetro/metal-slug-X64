> Active title: METAL SLUG X64 with original animated letters, particles, flash
> and sound; authored falling 6/4 digits and PRESS START. The generic voice and
> static card below are historical rejected experiments. The original spoken
> announcement is retained, followed by a synthetic "sixty-four"; this is not a complete Mission 1 demo.

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
silencio posterior, 24 buffers enviados al AI y 55 presentaciones RDP mientras
suena la referencia. La prueba no es una captura acústica ni una medición de
rendimiento del escenario completo. Ver [audio-native-validation.json](audio-native-validation.json).

## Reconstrucción gráfica y estado de integración

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
idéntico al volcado siguiente. La nueva captura también registra las escrituras
de paleta y las reproduce por línea cuando hay fundidos. Para cada línea,
la lista de sprites se selecciona con el estado inicial; los píxeles usan el
estado posterior a las escrituras de esa línea, conforme al orden del timer de
sprites de MAME. Esa distinción explica el último píxel que aún difería después
de introducir las escrituras por línea. No se copian píxeles de la captura ni
se utiliza una máscara para aceptar discrepancias.

Las primeras capturas no guardaban el indicador de autoanimación; sus once
comparaciones usaban una hipótesis de fase explícitamente limitada. Las nuevas
capturas guardan ese indicador y la selección de FIX. En la entrada de misión
se comprueba que autoanimación está deshabilitada y se conservan directamente
los códigos de tile escritos por el juego. Ese tramo ya se contrasta fotograma
a fotograma, sin inferir la fase desde el contador de vídeo.

La [entrada en paracaídas](MISSION_ENTRY.md) sí dispone ahora de un reproductor
N64 integrado con audio y transición al jugador. La presentación completa de
título/tutorial/selección aún no está integrada. El juego activo sigue limitado
a la región provisional y falta completar la Misión 1. No se publica otra demo.

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


## Pantalla de título original adaptada a N64

El arranque interno ahora presenta el título original reconstruido desde C-ROM,
S-ROM y paletas de la referencia del frame 660. Antes de adaptar los controles,
`pack_title_reference.py` compara la reconstrucción con la captura independiente
de MAME (tolerancia RGB <=1 en los 304x224 píxeles activos). Luego cambia solo
los caracteres FIX de INSERT COIN a PRESS START y elimina el contador de créditos;
no redibuja el logotipo ni altera el copyright. Los PNG no se usan como texturas.

Se mantiene la pantalla cinco segundos, con avance anticipado al pulsar Start
tras el primer medio segundo y detección de flanco para ignorar una pulsación
mantenida al arrancar. Se reproduce el fragmento original de audio de título ya
convertido. El reproductor cierra los recursos de título antes de abrir la
entrada en paracaídas. Ambos usan el reproductor nativo/RDP y audio WAV64.

**Alcance:** es una pantalla original adaptada y estática, no la animación completa
del título arcade, tutorial o selección de personaje. La demo de toda la misión,
la lógica original completa y los sonidos del juego siguen pendientes. No se
publica una ROM ni un vídeo parcial como entrega terminada.

Preparación privada:

```sh
python3 tools/pack_title_reference.py CAPTURA_COMPLETA --bios ROMS/neogeo.zip
python3 tools/prepare_audio_reference.py CAPTURA_COMPLETA
make mission1-internal.z64
python3 tools/test_entry_ares.py mission1-internal.z64 --region --output NUEVO_RESULTADO
python3 tools/test_entry_ares.py mission1-internal.z64 --skip-title --output NUEVO_RESULTADO_START
```

La prueba revisa título, audio activo al inicio, CRC de la imagen adaptada,
avance automático/Start y conservación de la entrada de 230 frames y su handoff.
No es una captura acústica ni una medición de rendimiento en hardware.


## Experimento rechazado: placa X64 y locución genérica

La propuesta anterior usaba el arte de `art/title/metal-slug-x64.png`, adaptado
mediante generación de imagen y convertido a 320x240/RGBA5551. El nombre visible
es METAL SLUG X64. La tipografía evoca los títulos de la época N64; no se atribuye
a una fuente oficial de Nintendo. Sustituía la placa original de la sección
anterior, que se conserva como herramienta de reconstrucción.

La voz del título dice “Metal Slug! X sixty-four!” y es una voz sintética genérica
de Piper. No se clonó al actor original. El canal ANNOUNCER reproducía el WAV64
convertido desde `art/audio/title-announcement.wav`. No se mezcla con el antiguo
fragmento de título, que podía contener la locución original; el audio de entrada
en paracaídas sigue siendo la referencia original.

`intro-preview.z64` es exclusivamente una prueba interna para grabar la
presentación: mantiene el último frame de la entrada y no ejecuta campaña.
Durante dos capturas con salida ALSA se observó un timeout RSP al entrar al juego;
sigue abierto. La prueba estándar sin ese dispositivo de captura sí atravesó el
tramo parcial, pero no demuestra que el fallo de audio/captura esté corregido.
El vídeo de muestra es un montaje de frames de ares y su PCM capturado, alineado
al tiempo de presentación nativo; no representa rendimiento real en hardware.
No se publica el cartucho de prueba ni se presenta como demo completa.


## Reconstrucción de la animación original (dirección vigente)

La captura `title-animation-banked-82` registra cada frame, escrituras de VRAM,
ambos bancos físicos de paleta y las conmutaciones de banco. El primer intento
sin ambos bancos falló una comprobación de preescritura: se descartó. El nuevo
replay conserva cada valor previo, cada máscara y ambas memorias finales; compara
cada uno de los 361 frames 300..660 con MAME, sin píxeles RGB activos fuera de
la tolerancia de una unidad. Son 24.582.656 píxeles activos contrastados.

El PCM se recorta con las marcas de tiempo de esos mismos frames, sin añadir
voz sintetizada ni sustituir los efectos originales. `pack_title_animation.py`
produce la secuencia nativa y el WAV64; los PNG solo verifican, no son texturas.
Los contadores nativos de ares verifican 361 frames únicos y cero omisiones,
además de conservar los 230 de la entrada en paracaídas y el recorrido parcial.
`check-cinematic` contrasta también todos los CRC decodificados por C.

Se amplía la caché de cinemática y se preparan hasta 32 frames pesados cuando
cabe en 8 MiB. El escenario se carga tras cerrar las cinemáticas, para que esa
memoria esté disponible. Se prepara el primer dibujo antes del reloj/audio y
se obtiene el búfer de pantalla **antes** de calcular/decodificar el frame.
Las versiones que lo hacían después omitían frames y no se aprobaron.

### Adaptación X64

La versión activa amplía la referencia hasta el frame 900 para conservar el
remate y el audio completo. Los 601 frames de origen pasan la comparación antes
de añadir los dígitos. El 6 y el 4 son sprites nuevos con trayectorias de caída
autorizadas por el diseño, rebote y revelado dorado. Ambos llegan al remate de la
X; no se sustituyen las partículas, las letras ni los impactos originales.
Las claves de movimiento están declaradas como autoría en `title_x64.py`, no
como direcciones o comportamiento recuperado de la ROM.

Se cambia el aviso a PRESS START y se elimina el contador de monedas. El fondo
del destello se conserva al borrar el texto; no se pinta una franja negra. El
empaquetador rechaza cambios fuera de esas regiones y de la zona de los dígitos.
El audio conserva los efectos y la locución original, que dice Metal Slug X;
no incluye una nueva locución «sixty-four».

La evidencia numérica de esta corrección está en [title-motion-validation.json](title-motion-validation.json). La prueba automática de avance y la de salto con Start pasan; no constituyen una captura acústica ni una prueba en MiSTer.

La ampliación de la secuencia cruzó por 2170 bytes el umbral anterior de 3 MiB;
la prueba inicial 92 preparó solo siete frames y omitió siete. Ese resultado se
rechazó. La caché ahora reparte un máximo conjunto de 6 MiB entre el archivo y
los frames pesados preparados, manteniendo margen para pantalla, audio y código.
La captura visual offline no es evidencia de rendimiento; la cadencia se valida
por separado en la ROM final de ares.

El título usa ahora WAV64 **PCM16 estéreo sin compresión, a 32 kHz**, en lugar
de VADPCM. El coste adicional es aproximadamente 0,9 MiB de cartucho; el flujo
se lee por el backend nativo sin precargar todo el audio en RDRAM. El vídeo
exporta los mismos frames compuestos, cuantizados a RGB555, con el PCM fuente;
no sustituye una medición en ares ni afirma rendimiento en MiSTer.

La captura de salida PCM de ares confirma una correlación de **0,992** con el
PCM fuente tras ajustar el pequeño desfase del reloj DAC. La versión VADPCM
anterior medía 0,524 con el mismo método y se sustituyó. El arranque y los 601
frames completos también pasan con PCM; el registro acústico está en
`x64-animated-audio-validation.json`. Es una medición de salida del emulador,
no una prueba de altavoces ni de hardware.

## Synthetic sixty-four addition

The user requested a similar synthetic voice immediately after the original X.
`art/audio/sixty-four.wav` supplies only that English phrase, not a replacement
for the original announcement. `title_announcement.py` mixes it at the authored
8.60 s insertion point, ending at 9.47 s, over the remaining original echo.
It starts from the verified source capture on every rebuild. Assertions verify
that earlier/later PCM is unchanged, that it does not clip, and that the tag
finishes before the title handoff. The N64 and exported video share this mix.
This supersedes the earlier note that a spoken sixty-four is absent.

The integrated synthetic-tail build passes automatic title playback and Start
handoff in ares. Captured output correlates 0.9923 with the composite source;
the insertion window alone correlates 0.9982. Both checks require >0.85.
`verify_title_pcm.py CAPTURE --rom intro-preview.z64` reproduces this comparison.
Build staging rejects a stale voice-asset hash or WAV64/metadata mismatch.

## Lower male announcer revision

The user rejected the higher Ryan tag. The active voice is now Piper stock
`en_US-hfc_male-medium`, lowered to 0.88 of its pitch with duration compensation,
body/presence EQ and compression. The tag peak rises from 8000 to 12500 PCM16.
A diffuse room impulse (380 ms T60, 22% wet RMS) replaces the small discrete echo.
The full tail ends at 9.8679 s; the title ends at 10.1545 s, so it is not cut.
`master_title_voice.py` and `art/audio/sixty-four.json` document this profile.
