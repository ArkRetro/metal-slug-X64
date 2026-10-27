# v6: zona de encuentros

Nuevo: coordenadas de mundo, cámara horizontal, combate con soldados,
proyectiles enemigos, tres puntos de vida para Marco, invulnerabilidad breve al
recibir daño, checkpoint, reaparición y condición de salida. R reinicia todo.

## Recorrido

Zona de 1400 píxeles. Cámara acotada al mundo, proyectiles en coordenadas de
mundo y descartados al salir del área de cámara. Aparecen tres soldados de
forma sucesiva cerca de x=420, 780 y 1140. Cada uno tiene dos puntos de vida.
Se aproximan al entrar en rango y disparan hacia Marco. Los impactos y el
contacto dañan a Marco; tras perder toda la vida reaparece con tres puntos.
El checkpoint x=640 se activa al llegar si ya se derrotó al primer soldado.
Los guardias derrotados se conservan al reaparecer; el encuentro actual reinicia.

Al derrotar a los tres guardias y llegar a x=1280 por el suelo, aparece AREA CLEAR.
La simulación se detiene hasta reiniciar con R. START mantiene disponible el
laboratorio de gráficos; pausa la simulación de la zona mientras se utiliza.

## Gráficos y limitaciones

El soldado verde se reconstruye desde la paleta 0x71, VRAM y ROM C de las
capturas originales de carrera. Exportación con:

```sh
python3 tools/export_enemy.py /workspace/research/captures
make -j4
```

Siete muestras de la misión 1, frames 2392-2416 cada cuatro frames. Se selecciona
el primer soldado por coordenadas x=160..255 para excluir al segundo. Los
3631 píxeles opacos coinciden con la referencia independiente MAME antes de
cuantizar a RGBA5551. El fichero reserva una segunda animación duplicada que
no se usa como animación de muerte. Al recibir daño se ilumina; al morir
parpadea y desaparece. Todavía no están extraídas las animaciones de disparo
y muerte del soldado. Su IA es nueva y provisional, no la lógica del 68000.

El fondo repite el fragmento de 304 píxeles útiles de la escena v5. La cámara
avanza sobre ese arte: esto no es un mapa completo extraído ni reproduce el
scroll/parallax original. Habrá discontinuidades en las uniones del fragmento.
El terreno dibujado no determina colisiones: se conserva un suelo plano.
No hay audio, granadas, vehículos ni transición a otra misión.

## Validación de esta versión

Una prueba de lógica recorre los tres combates, comprueba salida y límites de
cámara, checkpoint, pérdida de vida, reaparición, reinicio y salto por pulsación.
Se ejecuta con el comando de test_gameplay de README. No se repitieron los
suites gráficos anteriores. Ares 146 se utiliza para recorrer la ROM de entrega
con los controles normales y capturar combate, checkpoint, reaparición y salida.
La confirmación en MiSTer de esta versión queda pendiente.
