# Integración de campaña: objetivo y límites actuales

El objetivo solicitado es el recorrido completo: título, How to Play,
selección, entrada en paracaídas, todos los actores y eventos, premios,
armas, HUD y jefe final. El ejecutable `campaign-internal.z64` sirve para
integrar ese trabajo; **no declara completada la primera misión**.

## Cambios conectados al ejecutable

- Presentación X64 existente con audio.
- Tutorial original, revelado de selección y confirmación de Marco:
  [implementación y fuentes](FRONTEND.md). La selección queda limitada a Marco.
- Entrada en paracaídas existente y entrega del control nativo.
- [Escenario prerenderizado por capas](PRERENDER_BACKGROUNDS.md), con colisiones
  independientes; no se convierte la partida en una reproducción de vídeo.
- HUD con tipografía FIX original y **vida/bajas reales del prototipo**.
  La disposición y los rótulos `HP`, `KILLS`, `PISTOL` son una adaptación;
  no se inventan puntuación, tiempo, vidas ni munición del arcade.

`src/frontend.c` coordina las pantallas anteriores al juego. Sus películas
se reconstruyen desde gráficos y estados medidos del original y se almacenan
como streams; no son una recompilación de la lógica de esas pantallas.
El controlador jugable sigue en C nativo. El HUD se dibuja después del mundo
y se invalida la residencia de TMEM/TLUT antes de cualquier dibujo posterior.

## Trabajo que sigue abierto

| Sistema | Límite pendiente |
| --- | --- |
| Marco | Conectar el controlador recuperado completo a campaña; actualmente siguen los seis estados históricos. |
| Otros personajes | Recursos, controladores e integración jugable; no basta con mostrar sus retratos. |
| Enemigos | Sustituir los tres guardias provisionales por IA, daño, muerte, creación y orden de actualización originales. |
| Vehículos y superficies de evento | Conectar las oleadas y los eventos originales con gráficos y colisiones. |
| Premios y armas | Creación, recogida, inventario, comportamiento y efectos originales. |
| HUD original | Productores reales de puntuación, vidas, tiempo y munición. |
| Resto del mapa | Cerrar discrepancias de composición y registrar cambios de fondo, animación y escenas sin mezclar capturas. |
| Encuentros y jefe final | Rutinas, estados, ataques, daños y condiciones de cierre recuperados e integrados. |
| Audio de campaña | Música y efectos conectados a eventos nativos; la presentación con audio no completa este sistema. |

No se utilizará un vídeo del jefe o de la misión como sustituto de una
implementación jugable, ni guardias copiados como sustituto de todos los
enemigos. Las capturas de MAME son referencia de recuperación y comparación.

## Comprobación de cada integración

```sh
make campaign-internal.z64 MSX_BACKGROUND=prerender
python3 tools/test_frontend_ares.py campaign-internal.z64 --region --output /tmp/msx-front-full
python3 tools/test_frontend_ares.py campaign-internal.z64 --skip-tutorial --output /tmp/msx-front-skip
```

`--region` recorre también el tramo disponible hasta cámara 1920, exige las
tres bajas provisionales y comprueba muestras de caché y presupuesto visual.
Para probar por separado el recorrido jugable parcial sin las pantallas
añadidas, sigue disponible el objetivo anterior y `test_entry_ares.py --region`.
Una prueba de esa región no equivale a superar la misión ni valida la IA
original. Las mediciones de ares no sustituyen la prueba posterior en MiSTer.

Los recursos se preparan con `pack_frontend.py`, `pack_scene.py` y
`build_hud.py`. `stage_frontend.py` incluye únicamente los streams de runtime;
los WAV PCM intermedios no entran en la ROM. El mapa actual y la ruta de
prerender necesitan 8 MiB; las referencias CI4 se pueden compilar por separado.

La fuente del HUD se configura con `MSX_HUD_CAPTURE=/ruta/a/captura` y
`MSX_HUD_FRAME=2100`. Se requieren `palette2100.bin`, `vram2100.bin` y
`game2100.png` de esa captura para verificar los glifos originales. El build
registra esa selección y depende de los tres archivos y del conversor de color;
cambiar la captura o la conversión invalida el header generado.
