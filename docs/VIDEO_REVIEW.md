# Revisión del vídeo de MiSTer

Fuente: vídeo del usuario `4_5983091817892030027.mp4` (44,03 segundos).
El [informe numérico](video-review-validation.json) conserva su hash y las
pruebas. El vídeo y sus fotogramas privados no se añaden al repositorio.

| Evidencia | Conclusión | Estado |
| --- | --- | --- |
| Paneles sobre cielo y suelo durante la partida | Los diagnósticos ocultan parte de los 304×224 píxeles activos. | Ocultos por defecto; L los alterna. No se sustituye todavía por un HUD arcade inventado. |
| Escenario desplazándose durante el recorrido | La inspección de código confirma profundidad 8 en la capa 2 a velocidad completa, frente a profundidad 0 de actores/colisiones. La fórmula produce 21,27 px de deriva con cámara 872. El vídeo por sí solo no proporciona coordenadas de colisión. | Capas 2/3/4 fijadas al plano jugable; perspectiva conservada para fondos lejanos. |
| Silueta blanca del soldado durante el combate, por ejemplo alrededor de 18 s | El prototipo usa destello blanco y desaparición de un actor de prueba; no representa la muerte completa recuperada del original. | Pendiente de soldados nativos, en el trabajo coordinado con Claude. No se inventan sus frames de muerte. |
| Arranque directo a la región y final «REGION END», aproximadamente 40 s | La grabación muestra el laboratorio/prototipo parcial. El menú nombra `mslugx-3d-dev.z64`; el nombre no permite identificar su commit. | El cartucho interno de entrada ya tiene 230 fotogramas con audio, pero título/tutorial/selección y Misión 1 completa siguen pendientes. |
| Audio grabado: media −72,8 dBFS y pico −56,9 dBFS | Prácticamente silencioso; el archivo no demuestra si la causa es el enrutamiento de captura. El laboratorio no inicializa audio y los SFX/música continua todavía faltan en el juego activo. | Pendiente, sin atribuirlo a un fallo de MiSTer ni darlo por resuelto con el audio de entrada. |

Verificación: `make check`, compilaciones MIPS, ocho CRC de framebuffer RDP
normales/NDEBUG idénticos en ares 146, entrada de 230 fotogramas sin saltos y
L mostrando/ocultando paneles (registro y capturas inspeccionadas). También
pasa el recorrido de pendientes, aterrizaje en tejado, tres encuentros
provisionales, límite de región, reinicio y laboratorio, con cero fallos de
caché en los diagnósticos. La primera
prueba del botón utilizó una tecla sin asignación: se corrigió a Q, que este
entorno asigna al L del mando, y se repitió con éxito.

No se ha cambiado la física recuperada para disimular desajustes gráficos,
ni se ha retirado el aviso de región incompleta. No se publica otra demo.
