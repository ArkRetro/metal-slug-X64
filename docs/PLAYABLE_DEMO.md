# Metal Slug X64: demo jugable del tramo actual

Descarga: https://github.com/ArkRetro/metal-slug-X64/releases/tag/playable-demo-20261104

Necesita 8 MB de RAM (Expansion Pak). En MiSTer, cargar `msx64-demo.z64`
en el core Nintendo 64 con 8 MB habilitados. No necesita las ROM de Neo Geo
como archivos separados. La prueba de esta entrega se ha hecho en ares 146;
la compatibilidad de esta compilación concreta en MiSTer queda por comprobar.

## Controles

- Cruceta o stick: moverse.
- A: saltar; soltar antes del siguiente salto.
- B: disparar; mantener para repetir.
- R: reiniciar el tramo jugable.
- L: mostrar u ocultar diagnóstico.
- START: saltar el título, una vez iniciado.

## Qué incluye y qué falta

Título animado X64 con audio, entrada en paracaídas con audio y traspaso
al control de Marco. El tramo permite caminar, correr, saltar y disparar,
con proyectiles gráficos, colisiones, tres guardias provisionales, paralaje
por capas y sombras de contacto. Termina en el límite de cámara 1920
(`REGION END`); no tiene el jefe ni el resto de la Misión 1.

El controlador completo de Marco, agacharse, granadas, variantes de armas,
IA original de los soldados, secuencias de vehículos, música continua y efectos
del juego siguen pendientes. Los nuevos módulos C recuperados y verificados
por separado aún no sustituyen esa lógica provisional en campaña.
No se declara una adaptación 1:1 ni rendimiento de hardware verificado.

## Validación

`tools/test_entry_ares.py` comprobó el título completo, los 230 fotogramas
de la entrada sin saltos, entrega del control, salto y aterrizaje, carrera y
disparo, derrota de los tres guardias y llegada al límite del tramo.
Sin errores de caché de texturas detectados. Se comprobaron también diagnóstico
y reinicio. El audio se verificó mediante el estado del reproductor; esta
prueba no incluye captura acústica ni medición en hardware.

El informe y el log están en `playable-demo-validation/`. El campo `limits`
del informe conserva la etiqueta histórica de la herramienta interna; la
publicación de esta demo está autorizada por la petición posterior del usuario.
Su SHA-256 identifica exactamente la ROM entregada.

## Compilación

Con libdragon y los assets locales ya preparados:

```sh
make msx64-playable-demo.z64 MSX_VISUAL_MODE=depth25d MSX_SCENARIO=campaign
python3 tools/test_entry_ares.py msx64-playable-demo.z64 --region --output /tmp/msx64-demo-check
```

Los assets propietarios se generan localmente y no forman parte del código
fuente versionado. El ZIP publicado contiene la ROM, instrucciones y checksum;
no es un ZIP de una decompilación completa.
