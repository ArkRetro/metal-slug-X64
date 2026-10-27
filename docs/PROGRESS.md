# Avance del port

## v2: evidencia del usuario

MiSTer: fotos de las páginas FIX 1, 2, 3, 4, 6, 7, 8, 9, 10, 11, 12,
13, 14 y 15 muestran arranque y navegación. No hay evidencia de las páginas
5 y 16 ni del retorno 16->1. Versión del core no registrada.
La página 8 (tiles 1792-2047) contiene exactamente 8192 bytes cero en S1:
el fondo negro coincide con el contenido, no es un fallo de lectura.

## v3: alcance

- Importación de las seis ROM C en un archivo intercalado de 48 MiB.
- Decodificación nativa de tiles C de 16x16, 4 bits por píxel.
- 393216 tiles / 6144 páginas; caché de lectura de solo 8192 bytes.
- Modo FIX anterior conservado. Páginas independientes por modo.
- Márgenes horizontales de 32 píxeles y textos dentro de y=24..222.
- Unión geométrica de tiles consecutivos, sin reconstruir ensamblajes originales.
- Paleta gris e inversa para inspección; no son paletas del juego.

## Validación de host

262144 comprobaciones de píxeles FIX y fila conocida.
262144 comprobaciones de planos C: activar cada bit por separado y comprobar
que no aparecen píxeles o planos ajenos. AddressSanitizer y UndefinedBehaviorSanitizer.
En este sandbox LeakSanitizer falla bajo ptrace: se ejecuta con
`ASAN_OPTIONS=detect_leaks=0` (las comprobaciones ASan/UBSan siguen activas).
Verificada la igualdad de cada vía par/impar del archivo generado con las
seis ROM originales, cubriendo los 48 MiB, después de validar CRC32 y SHA1.

## Pendiente para el juego

Identificar y reconstruir ensamblajes de sprites, paletas, animaciones,
escenarios, movimiento, disparos, colisiones, enemigos y audio. No hay aún
emulación del 68000 ni el juego completo. No se afirma un port completo.

## Prueba de ejecución v3 completada

Emulador: ares 146 oficial compilado con `ARES_CORES=n64`, perfil de rendimiento,
Homebrew mode, Vulkan paraLLEl-RDP sobre lavapipe, OpenGL por software y Xvfb.
La build Debian ares 134 se descartó porque no incluye N64.

Comprobado con la ROM de entrega (no una variante de prueba):

- Arranque y renderizado de la primera página C (0-63).
- Lectura de la última página (393152-393215), retorno a la primera.
- Saltos +256 y +16 páginas y conservación de página al volver desde FIX.
- Lectura en ambos lados de los límites de los bancos: páginas internas
  2047/2048 y 4095/4096; capturas de los tres bancos.
- Unión de tiles e inversión de grises, con comprobación visual.
- Modo FIX: primera/última página, retorno circular y letras en página 4.
- Sin errores del programa ni asserts del SDK en el registro de ejecución.

No se ha comprobado exhaustivamente cada página, ni rendimiento en consola,
ni ejecución v3 en MiSTer. VPS de ares no representa el rendimiento en N64.
No hay audio implementado, por lo que no se valida salida de sonido.

El emulador preparado se conserva en `/workspace/emulator/`. Para repetir en
esta máquina con una pantalla X11 disponible, usar `bash tools/run_ares.sh`.
Si falta pantalla, iniciar el Xvfb local en :99 con:

```sh
LD_LIBRARY_PATH=/workspace/emulator/root/usr/lib/x86_64-linux-gnu \
/workspace/emulator/root/usr/bin/Xvfb :99 -screen 0 1024x768x24 -ac -nolisten tcp \
-xkbdir /workspace/emulator/root/usr/share/X11/xkb
```

Este helper depende del emulador retenido en la máquina, no instala ares en
una máquina ajena. El código de ares proviene de la release oficial v146;
las dependencias ares-deps 2025-07-14 se verificaron con el SHA256 publicado
`c4679098d2a9cd51c1cacb70555e302e78501e481dcaabc060ca16d6f620128f`.

SHA256 de la ROM v3 probada: `86d280d3f7ffc3a83b2187d1364d2819e9efe3c50783b2012f55e8c5e9012f77`.


## v3: confirmación posterior del usuario

El usuario confirma visualización de tiles y cambio de páginas en MiSTer.

## v4: Marco nativo en color

La demo incorpora reconstrucción de sprites por VRAM y paleta 0x63, con
30 muestras de reposo y 30 de carrera extraídas del juego original. Se verifican
39.344 píxeles opacos contra capturas independientes de MAME antes de cuantizar
a RGBA5551. Las capturas no se usan como fuente de imágenes del personaje.
El script de captura guardado se ejecutó de nuevo: la exportación produjo
exactamente el mismo marco.bin (comparación byte por byte).

Se comprobó que una captura de referencia alterada provoca un fallo antes de
escribir el asset. Pruebas de FIX y C con ASan/UBSan siguen pasando.

Prueba de entrega en ares 146 (misma configuración de v3):

- Marco completo, en color, transparente y animado en reposo.
- Carrera, desplazamiento y orientación hacia ambos lados mediante cruceta.
- Límites de movimiento visibles dentro de los márgenes; pruebas de imagen.
- START entra en el laboratorio C/FIX y vuelve a Marco.
- Verificación visual de letras FIX en página 4 tras el cambio de modo.
- Sin errores del programa ni asserts del SDK en el registro.

Prueba automatizada del emulador preparada: `tools/test_actor_ares.py`.
Usa X11 :99, los mapeos de teclado retenidos y el registro final de v4 en
/workspace/emulator. Requiere iniciar ares con tools/run_ares.sh, redirigiendo
salida a /workspace/emulator/run-v4-final.log. Enter=START, flechas=cruceta,
a=A, b=B, z=Z, q=L, e=R. Las capturas se guardan fuera del checkout.

No se han probado el stick analógico en emulador, MiSTer v4 ni consola N64.
La lógica de movimiento, el suelo y los pivotes son provisionales. Todavía no
hay salto, disparo, enemigos, audio ni escenario original en esta demo.

SHA256 de la ROM v4 probada: `d14d68034fb3b9c140d4a1e77a8340453966381bf9ca4555d42b2cb82d64d25b`.

## v4: confirmación del usuario

El usuario confirma que Marco se ve en MiSTer.

## v5: acción

Ver docs/ACTION.md. Se han añadido salto, disparo, colisión de proyectiles con
un blanco de prueba y composición original estática del desierto. Comprobación
breve en ares completada con impactos, salto/aterrizaje y regreso al laboratorio.
Se mantienen provisionales física, cadencia, pivotes y suelo plano. Sin enemigos,
scroll, colisiones del mapa ni audio. No es aún la primera misión completa.

SHA256 ROM v5 comprobada: `e3f9aef5f35348142089e20fd22eb66d5d7137795044ecf450623bd2c23344ad`.

## v5: confirmación del usuario

El usuario comunica que la v5 funciona perfectamente en su equipo MiSTer.

## v6: encuentros y cámara

Ver docs/ENCOUNTER.md. Nuevos sistemas: mundo/cámara, soldados con proyectiles,
daño a Marco, guardias derrotados, checkpoint/reaparición, salida y reinicio.
Sprites originales del primer soldado extraídos y contrastados (3631 píxeles).
La primera misión no está recuperada: se repite el fragmento visual del desierto
sobre un suelo plano, con IA y colisiones provisionales.

La ROM final v6 se recorrió en ares 146 con controles normales: cámara,
soldado visible atacando, seis impactos para derrotar a tres guardias, checkpoint,
pérdida de vida y reaparición en x=640, reinicio R, salida AREA CLEAR y cambio al
laboratorio con START. Sin asserts del SDK ni errores del programa en el log.
La fase de salida deja a Marco en reposo. Las capturas de reaparición pueden
mostrarlo invisible durante el parpadeo de invulnerabilidad, que es intencionado.
MiSTer v6 y consola N64 pendientes de confirmar.

SHA256 ROM v6 comprobada: `a43c84fbd3a71a0740ffe035d7aa567873d3c9834935a8bcb2452a0abe4882b4`.

## v6: confirmación del usuario

El usuario confirma funcionamiento y solicita corregir la animación de salto
mientras Marco corre. La v8 incorpora esas secuencias combinadas.

## v8: CI4, RDP y caché PI implementados

Ver docs/RDP_IMPLEMENTATION.md. La escena deja de dibujarse por píxeles de CPU:
usa rectángulos RDP, texturas CI4, TLUT y fondo mixto CI4/RGBA16. Los índices
originales de los actores se preservan en MSXI y en el pack, no se recuperan a
partir de colores que podrían estar duplicados.

ROM de entrega recorrida en ares 146, Homebrew mode, paraLLEl-RDP/lavapipe:
arranque PI/CRC, salto en carrera, disparo corriendo, aterrizaje en cajas de 20
y 24 píxeles, combate, checkpoint, tres guardias, AREA CLEAR, reinicio, flip y
regreso desde el visor. Capturas guardadas. Codec y reloj tienen pruebas propias.

Durante ese recorrido: 20 cargas PI / 122944 bytes, sin nuevos misses después
de precalentar la zona; 320 KiB decodificados. Ejemplos registrados: 78-85 draws,
77-84 uploads, 43856-49272 bytes a TMEM por frame; cada upload <=2048 bytes.
El tiempo de envío CPU indicado por ares ronda 4,5-4,8 ms en esos frames: no es
un benchmark físico ni el tiempo RDP aislado. La emulación por software no
acredita la ausencia de ralentizaciones en todos los escenarios del juego.

Reconstrucción limpia reproducible: make clean + make -j4 produce exactamente
la ROM que se recorrió en ares, comprobado por SHA256. MiSTer v8 pendiente.

SHA256 ROM v8 comprobada: `7af47603574dcadcb5aa7ee682064427cdd8139f24d8daf0dc4eb86b2f99d38a`.

## v9: mapa observado de misión 1 y ecuación aérea original

El usuario confirma la base anterior y solicita continuar el port completo.
V9 sustituye el fondo repetido por 1402 celdas, tres capas activas con parallax,
autoanimación y color de fondo original. Atlas CI4 por paleta: 230 páginas en
el conjunto total, 72 paletas, 29 chunks / 464 KiB decodificados. Pack: 272384 B.
Las cajas y banderas de prueba desaparecen. Suelo medido en 352 muestras;
colisiones y combate todavía provisionales. REGION END no es fin de misión.

La prueba aérea compara 22 estados completos Q16.16 con el arcade: coincidencia
exacta de X/Y/Vx/Vy. El jugador se actualiza cada dos ticks de vídeo. La ecuación
es exacta para ese tramo libre; marcha sobre pendientes, colisiones y pivotes
originales siguen pendientes. Ver docs/MISSION1.md.

Validación de capas: 36 frames, 2451456 píxeles; 17227 diferencias únicamente
sobre coordenadas con variantes registradas. Además 1741513 píxeles RGB8 visibles
coinciden con PNG originales de MAME, tolerancia <=1/canal. El RDP usa RGB5551:
no se presenta esta comparación como igualdad RGB8 de la salida N64.

make check pasa gameplay, ecuación aérea, codec/CRC y reloj. Recorrido de ROM
v9 en ares146: salto, dunas, pueblo, tres encuentros provisionales, checkpoint,
límite, reinicio, salto izquierdo, laboratorio y regreso. Sin asserts. PI:
29 cargas, 272384 bytes, cero misses tras precalentamiento, cero evicciones.
Capturas y logs: /workspace/emulator/v9-release y run-v9-release.log.

Reconstrucción limpia con make clean, make check y make -j4 produjo exactamente
el binario recorrido en ares. MiSTer v9 y N64 física pendientes.

SHA256 ROM v9: `de38e07e911b302688627501a3fa19bdeba639c50ec26bf930b549ef00f5ed14`.

## v9: confirmada en MiSTer; v10: cuadrícula y objetos

El usuario confirma funcionamiento de v9 en MiSTer. Se avanza con la geometría
original del tramo: cuadrícula 8x8 (149x32, 182 celdas no vacías), pendientes,
superficies apiladas y 51 tiles del carro/toldo. Se retira del runtime el perfil
interpolado de 352 muestras, que se conserva como referencia histórica.

350 pasos de marcha coinciden dentro de 2/65536 píxeles; 24 estados completos
del salto/aterrizaje sobre el carro coinciden bit por bit con MAME. Se conservan
las pruebas de vuelo, codec y reloj. El runtime todavía no es el resolvedor
completo de colisiones del arcade. Ver docs/COLLISION.md para límites.

ROM final recorrida en ares146: dunas, carro/toldo visibles, aterrizaje en el
carro, caída, pueblo, combate provisional, límite, reinicio y laboratorio.
Sin asserts; 30 lecturas PI / 275264 bytes, cero misses tras precalentamiento,
cero evicciones. 480 KiB de caché asignada. Capturas y logs de v10-release
retenidos en /workspace/emulator/. Las nuevas capturas de avance con vehículo
no se incorporan al mapa: los extractores detectan datos todavía ambiguos.

make clean + make check + make -j4 reproduce el SHA del binario recorrido.
V10 pendiente de MiSTer; no se amplía todavía el límite del tramo del pueblo.

SHA256 ROM v10: `3b41d823eec47eeebf4c6f9144f76834fe3f32bf12f0922ef2d4fa4f8b88ba60`.
