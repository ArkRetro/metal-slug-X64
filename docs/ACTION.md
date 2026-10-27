# v5: escena de acción

Nuevas capacidades: movimiento lateral, salto con aterrizaje, disparo sostenido,
proyectiles en ambas direcciones y un blanco con detección de impactos.
START conserva el acceso al laboratorio C/FIX. El estado se pausa mientras se
usa el laboratorio.

Marco vuelve al tamaño nativo de los gráficos (la v4 lo dibujaba al doble).
Las poses de salto y disparo proceden del original: se capturan usando A y B
de Neo Geo con `tools/capture_actions.lua`. La asignación en N64 es A=salto,
B=disparo. Hay 30 muestras por acción, cada dos frames del original.
Los 42.168 píxeles opacos se contrastaron contra la salida independiente de MAME.
Los disparos usan un lienzo de 80x64 para incluir el fogonazo. El pivote horizontal
se fija en x=120 del personaje inmóvil de referencia; el inferior sigue siendo
provisional. El renderer convierte los colores a RGBA5551 de N64.

El fondo se reconstruye con SCB1/2/3/4, tiles C, paletas y fase de autoanimación,
excluyendo a Marco, el indicador P1 y GO. `tools/export_scene.py` contrasta
48.795 píxeles de cielo, señal y paisaje con el original; excluye del contraste
el HUD FIX y el área del personaje. Es una composición estática del comienzo
capturado, no un mapa de nivel extraído ni una captura de pantalla incrustada.

## Regenerar assets

Usar la misma versión y opciones MAME descritas en MARCO.md, con salida nueva
para las acciones y el script tools/capture_actions.lua. Después:

```sh
python3 tools/export_marco.py /ruta/capturas-acciones --actions
python3 tools/export_scene.py /ruta/capturas-reposo-carrera
make -j4
```

En esta máquina las capturas se conservan en /workspace/research/actions y
/workspace/research/captures respectivamente. Se requieren además los assets
base de v4. Ninguna BIOS ni CPU emulada se incluye en la ROM N64.

## Lógica provisional

La física usa pasos de simulación por actualización: movimiento de 2 px, impulso
vertical -6 px y gravedad 0,5 px. Un A mantenido no provoca saltos automáticos
al aterrizar. B puede generar una bala cada 8 actualizaciones; hay 8 slots
reutilizables, balas a 6 px por paso y descarte al salir del área visible.
La prueba barrida horizontal detecta impactos sobre un blanco situado en x=264.
Las balas disparadas por encima no cuentan como impacto. El blanco no es un
enemigo del original. No hay sonido, scroll ni colisión con las dunas dibujadas.

## Comprobación breve de la entrega

Ares 146, core N64, Homebrew mode y paraLLEl-RDP: arranque de la escena,
movimiento, salto y aterrizaje, disparo e impactos visibles en el contador,
START al laboratorio y regreso. Capturas incluidas en el ZIP.
La prueba de lógica cubre el salto por pulsación, aterrizaje, límites, cadencia,
liberación de proyectiles, impactos y disparos que fallan el blanco.
No se repitió toda la batería de FIX/C ya validada. MiSTer v5 pendiente.
