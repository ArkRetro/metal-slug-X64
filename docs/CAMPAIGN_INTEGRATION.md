# Integración de campaña: objetivo y límites actuales

El objetivo solicitado es el recorrido completo: título, How to Play,
selección, entrada en paracaídas, todos los actores y eventos, premios,
armas, HUD y jefe final. El ejecutable `campaign-internal.z64` sirve para
integrar ese trabajo; **no declara completada la primera misión**.

[Validación de esta integración](marco-campaign-integration-validation.json),
con hashes y [registros de las pruebas](marco-live-validation/).

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
- Controlador nativo de Marco conectado al mando y al mundo mediante
  `src/player_runtime.c` y `src/marco_campaign.c`. Encadena las rutinas C
  recuperadas de cuerpo, postura, movimiento, agachado, bloque común y arma;
  sustituye el selector de seis estados que conserva la demo publicada.
- Programas de animación, descriptores, paletas y sprites de Marco extraídos
  directamente del cartucho por `build_marco_campaign_assets.py`. Los datos
  generados son recursos y programas, no una captura temporal de la partida.
- Pistola normal con creación nativa, dirección y temporización recuperadas.
  Sus cuatro recursos cardinales conservan los índices, flags y pivotes
  originales: [procedencia gráfica](campaign-projectile-art-validation.json)
  y [comparación del backend](marco-pistol-validation.json).
- Seis efectos originales aislados mediante el controlador de sonido de MAME:
  `10F7`, `10FC`, `1146`, `10FB`, `10F9`, `1069`. El reproductor nativo los
  recibe como eventos; `1069` está preparado para un blanco de clase 4, que
  no existe todavía en los encuentros actuales.
  [Fuente, aislamiento y conversión](campaign-effects-validation.json).

`src/frontend.c` coordina las pantallas anteriores al juego. Sus películas
se reconstruyen desde gráficos y estados medidos del original y se almacenan
como streams; no son una recompilación de la lógica de esas pantallas.
El controlador jugable sigue en C nativo. El HUD se dibuja después del mundo
y se invalida la residencia de TMEM/TLUT antes de cualquier dibujo posterior.

## Alcance del jugador integrado

La forma normal de Marco admite reposo, marcha, giro, salto, agachado y
disparo de pistola horizontal, hacia arriba y hacia abajo en el aire. La
animación genera las solicitudes originales de ataque cercano y sus cajas
de contacto. La reacción del guardia a ese ataque sigue siendo la respuesta
provisional del prototipo: no implementa su manejador original de daño ni
su IA. Las cajas del jugador proceden de los descriptores activos originales.

La simulación recuperada del actor se ejecuta en pasos alternos del bucle de
vídeo; los proyectiles conservan su creación diferida y el retraso original
de sonido/dibujo. Las direcciones mantenidas y los flancos de los botones
se conservan en las entradas del controlador. B requiere pulsaciones; no se
añade el disparo automático del prototipo anterior.

El mundo aporta el subconjunto de suelo y superficies medidos, el recorte
recuperado y las consultas de escena implementadas. El orden global de RNG,
el resolvedor genérico del arcade y los demás servicios del mundo siguen
incompletos. La vida, invulnerabilidad, retroceso y reaparición del prototipo
se sincronizan con el actor, pero **no son las rutinas originales de daño y
muerte de Marco**. Un servicio, evento o recurso no implementado detiene la
ruta con un diagnóstico; no se sustituye por una animación genérica.

Controles en campaña: cruceta izquierda/derecha para caminar, arriba para
apuntar, abajo para agacharse o apuntar hacia abajo en el aire, A para saltar
y B para disparar por pulsación. La selección sigue restringida a Marco y las
granadas no están mapeadas al mando.

## Trabajo que sigue abierto

| Sistema | Límite pendiente |
| --- | --- |
| Marco | Completar armas, formas y acciones especiales, efectos pendientes y rutinas originales de daño/muerte. La integración de la forma normal/pistola no acredita el controlador completo. |
| Otros personajes | Recursos, controladores e integración jugable; no basta con mostrar sus retratos. |
| Enemigos | Sustituir los tres guardias provisionales por IA, daño, muerte, creación y orden de actualización originales. |
| Vehículos y superficies de evento | Conectar las oleadas y los eventos originales con gráficos y colisiones. |
| Premios y armas | Creación, recogida, inventario, comportamiento y efectos originales. |
| HUD original | Productores reales de puntuación, vidas, tiempo y munición. |
| Resto del mapa | Cerrar discrepancias de composición y registrar cambios de fondo, animación y escenas sin mezclar capturas. |
| Encuentros y jefe final | Rutinas, estados, ataques, daños y condiciones de cierre recuperados e integrados. |
| Audio de campaña | Completar música y efectos del resto de actores/eventos. Los seis efectos conectados al jugador y el audio de presentación no completan este sistema. |

No se utilizará un vídeo del jefe o de la misión como sustituto de una
implementación jugable, ni guardias copiados como sustituto de todos los
enemigos. Las capturas de MAME son referencia de recuperación y comparación.

## Comprobación de cada integración

```sh
make campaign-internal.z64 MSX_BACKGROUND=prerender
make check-marco-live
python3 tools/test_live_marco_ares.py campaign-internal.z64 --region --output /tmp/msx-marco-live
```

Las huellas originales se escriben una sola vez en copias privadas de las
texturas del primer plano. Los píxeles transparentes borran el contenido
anterior y dejan ver las capas inferiores; las páginas compartidas del
cartucho permanecen intactas. Esta ruta conserva las sombras sin reconstruir
el fondo de cada huella en cada fotograma. Su comparación RDP se reproduce con:

```sh
make footprint-cow.z64
python3 tools/test_footprint_cow_ares.py footprint-cow.z64 --output /tmp/msx-footprint-cow
```

La prueba compara 22 casos completos en las rutas clásica y por capas, con
transparencia, cambios múltiples, entradas sin cambios, sombras y reinicio.

`check-marco-live` compara los módulos de runtime, pistola, ataque cercano,
mundo, consultas de escena y paleta en host. La prueba de ares usa el mando
del ejecutable real: verifica agachado, apuntado y disparos en salto, sin
reproducir entradas ni estados capturados del actor de MAME.
`--region` recorre también el tramo disponible hasta cámara 1920, exige las
tres bajas provisionales, comprueba que no haya fallos de caché y registra
muestras de tiempo de CPU. Registrar estas muestras no acredita 60 FPS.
La prueba `test_frontend_ares.py` conserva las comprobaciones específicas de
las pantallas previas. Detecta el controlador nativo por sus mensajes de
estado, usa pulsaciones de B, exige el evento original de sonido de pistola
y comprueba movimiento, salto y aterrizaje tras la entrega del control.
Su opción `--region` también adapta las entradas al controlador detectado;
conserva compatibilidad con el controlador histórico. No sustituye las
comprobaciones específicas de agachado y apuntado de `test_live_marco_ares.py`.
Para probar por separado el recorrido jugable parcial sin las pantallas
añadidas, sigue disponible el objetivo anterior y `test_entry_ares.py --region`.
Una prueba de esa región no equivale a superar la misión ni valida la IA
original. Las mediciones de ares no sustituyen la prueba posterior en MiSTer.

Los recursos se preparan con `pack_frontend.py`, `pack_scene.py`,
`build_hud.py`, `build_marco_campaign_assets.py`, `build_campaign_projectiles.py`,
`build_campaign_footprints.py` y `build_campaign_effects.py`.
`stage_live_player.py` añade los recursos del jugador y efectos al ROMFS de
campaña. `stage_frontend.py` incluye únicamente los streams de runtime;
los WAV PCM intermedios no entran en la ROM. El mapa actual y la ruta de
prerender necesitan 8 MiB; las referencias CI4 se pueden compilar por separado.

La fuente del HUD se configura con `MSX_HUD_CAPTURE=/ruta/a/captura` y
`MSX_HUD_FRAME=2100`. Se requieren `palette2100.bin`, `vram2100.bin` y
`game2100.png` de esa captura para verificar los glifos originales. El build
registra esa selección y depende de los tres archivos y del conversor de color;
cambiar la captura o la conversión invalida el header generado.
