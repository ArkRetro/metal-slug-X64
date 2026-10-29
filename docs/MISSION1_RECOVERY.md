# Recuperación del recorrido posterior al mercado

La captura privada `mission1-gate-sweep-47` alcanza visualmente el jefe y la
secuencia de final de la primera misión original. **Esto no completa el port:**
el ejecutable nativo sigue terminando en la región parcial de cámara 1920.

Se ejecutó MAME 0.276, BIOS euro, durante 36.000 fotogramas y se conservaron
566 muestras de RAM, VRAM, paleta y pantalla. `capture_advance.lua` permite
`MSX_ADVANCE_LAST` y `MSX_ADVANCE_SWEEP=1`. El barrido usa controles para atacar
las torres elevadas cuando la cámara permanece detenida. Mantiene los overrides
históricos de tiempo, invulnerabilidad y munición de la herramienta de referencia;
no es una partida sin modificaciones ni prueba de rendimiento nativo.

Observaciones visuales, sin atribuir todavía direcciones de rutinas del jefe:

- Fotograma 13.800: jefe del mercado recibiendo daño.
- Fotograma 14.220: animación del texto de final de misión.
- Fotograma 14.400: secuencia posterior al jefe, con bandera y rescatados.
- Fotograma 15.000: escenario siguiente; la cámara de RAM vuelve a cero.

El experimento previo `mission1-tower-sweep-46` supera las torres de cámara 1920.
En su fotograma 9.000, Marco montado en el camello tiene X local negativa:
`actor+28` ya no puede interpretarse como posición mundial independiente.
El piloto experimental usa la X proyectada de `actor+3C` (RAM `0x101500`)
para decidir los controles; no inventa una transformación del padre para el motor.

## Extractor y auditoría

Los tramos posteriores contienen al menos dos colores de fondo, palabras
`0x2337` y `0x1EA4`. `extract_mission_map.py --allow-backdrop-timeline` conserva
el valor observado por fotograma. El empaquetador rechaza mapas con varios
valores hasta recuperar su evento nativo; no deduce un umbral de cámara de unas
pocas muestras ni fija el primer color para toda la misión.

El jefe reutiliza slots que en el tramo inicial alojaban fondos. La auditoría
solo exige zoom unitario cuando el tile pertenece también al banco de fondo;
la pertenencia al rango de slots por sí sola no basta. Caso observado:
`vram11700.bin`, slot 105, zoom `0x0AB0`, atributo de tile `0x7201`.

La auditoría ampliada sigue **sin pasar**: en el fotograma 13.320, píxel
(175,48), encuentra RGB (217,169,79) frente a MAME (176,103,65). No se ha
suprimido esa discrepancia. Se debe recuperar la composición/oclusión aplicable
antes de convertir este mapa en recursos de campaña. La referencia de 122
fotogramas del tramo integrado permanece validada por su informe anterior.

Pendiente: identificar rutinas y eventos del jefe y del cierre, reconstruir la
relación con vehículos/padres, completar el controlador de Marco, integrar los
actores y sonidos y validar la misión nativa entera en ares. Una captura de MAME
o una reproducción de sus imágenes no sustituye esa implementación.
