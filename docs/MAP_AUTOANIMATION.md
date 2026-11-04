# Corrección de la autoanimación del decorado

El auditor anterior deducía la fase como `frame & 7`. Esa relación no es una
regla del hardware: el contador puede avanzar a otra velocidad y la sustitución
de bits del tile puede estar deshabilitada aunque el contador siga avanzando.

## Verificado

La captura privada `mission1-device-phase-191` registra los campos reales
`m_auto_animation_counter` y `m_auto_animation_disabled` del dispositivo
spritegen de MAME 0.276. En las 86 observaciones entre los fotogramas 2100 y
7200, correspondientes al tramo de cámara 0..1920, la sustitución está
deshabilitada. En observaciones posteriores cambia ese estado y se observa
otra cadencia del contador. No se inventa un divisor global para el port.

La semántica está respaldada por el [driver de MAME 0.276](https://github.com/mamedev/mame/blob/mame0276/src/mame/neogeo/neogeo_spr.cpp):
el temporizador actualiza su contador según la velocidad y el dibujado solo
sustituye bits cuando la autoanimación está habilitada. El hash del archivo
y las líneas consultadas están en
[la referencia de hardware](campaign-validation/animation-hardware-source.json).
Esto no identifica todavía la rutina del juego que cambia esos registros.

En el mapa existente había 160 celdas lejanas con máscara 3. Se cambia
**únicamente su máscara de animación de 3 a 0**: se mantienen sus códigos
originales, coordenadas, reflejos, paletas y todas las demás celdas.
La comparación de capturas antiguas cada 60 fotogramas coincidía siempre
con la fase cero y ocultaba el error entre muestras.

`audit_map_phase_correction.py` comprueba la procedencia SCB original de las
160 celdas y determina la visibilidad con zoom, límite de sprites, transparencia
y FIX originales. Coinciden 22376 píxeles visibles de 68 celdas con los PNG
independientes de MAME. Las otras 92 tienen procedencia directa de estados
capturados, pero permanecen tapadas o fuera de vista; no se cuentan como
comparaciones visuales. Los PNG no seleccionan fases ni generan texturas.

[Informe de la corrección](campaign-validation/map-phase-correction.json).
`apply_map_phase_correction.py` verifica los hashes de ambas entradas y del
informe y rechaza cambios adicionales antes de aplicar las máscaras.

## Límites que permanecen abiertos

Esta corrección específica pasa; **la auditoría del mapa completo sigue sin
pasar**. La nueva captura parcial contiene una discrepancia independiente
en el fotograma 4140, píxel (145,48). El recorrido posterior presenta además
una transformación Y ambigua en 8100 y una discrepancia en 12960, píxel
(17,48). No se eliminan esas observaciones ni se convierten en excepciones
para declarar válida toda la misión.

`extract_mission_map.py` ahora permite elegir intervalos explícitos de
fotogramas para evitar mezclar la primera misión con la escena siguiente.
Conserva la procedencia del contador y su bandera; `validate_mission_map.py`
rechaza las observaciones sin esos datos en lugar de adivinarlos.

Los cambios de fondo, las fases posteriores, los eventos y la integración
del resto de la campaña permanecen pendientes. La comparación entre el
backend de tiles y el prerender verifica equivalencia entre renderers;
no sustituye esta validación independiente contra el arcade.
