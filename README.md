# Metal Slug X para Nintendo 64: trabajo en curso

La demo jugable del tramo actual se publica por petición del usuario el 4 de noviembre de 2026.
Incluye título X64 con audio, entrada en paracaídas y recorrido parcial con tres guardias.
**No es la Misión 1 completa.** Descarga, controles y límites en
[la guía de la demo](docs/PLAYABLE_DEMO.md).
El objetivo de completar la misión y recuperar la lógica original sigue pendiente.
Véase [estado del port y entrega](docs/BETA_STATUS.md).
La prioridad actual es [completar todas las animaciones de Marco](docs/MARCO_ANIMATIONS.md);
el controlador completo todavía está pendiente y no se declara un 100%.
El [pipeline nativo de entradas, selección y cuerpo/arma](docs/MARCO_CONTROLLER.md)
ya pasa comparaciones con MAME y con framebuffer real del RDP en ares.
La demo usa [capas de paralaje y sombras de contacto](docs/RETRO25D.md),
sin extrusión de edificios, niebla ni iluminación dinámica adicional.
El pixel art y la acción se mantienen en 2D.
`make MSX_VISUAL_MODE=classic` compila la ruta plana original.
Los [proyectiles de pistola](docs/PROJECTILES.md) usan ya el sprite original
con contorno, paleta y transparencia, también al disparar hacia la izquierda.
El [backend nativo del blindado](docs/NATIVE_BACKEND.md) ya separa lógica C,
gráficos `.sprite` de libdragon y colisiones de los restos. Se valida en un
escenario interno; su oleada y aparición originales siguen pendientes.
El [primer soldado de Misión 1](docs/FIRST_SOLDIER.md) ya está localizado mediante
watchpoints, traza MAME y Ghidra. El informe separa hechos verificados de hipótesis;
la IA completa sigue pendiente.
El [RNG original y los siguientes bloques del soldado](docs/SOLDIER_RNG.md)
ya tienen código C y fixtures medidos que pasan en host y como MIPS en ares.
El [helper de animación nativo](docs/SOLDIER_ANIMATION.md) conserva cursor y
temporizador durante 54 actualizaciones y reproduce los dibujos y el callback
de orientación observados. La integración de la IA completa sigue pendiente.
El [movimiento y contacto de suelo recuperados](docs/SOLDIER_CONTACT.md)
ya se encadenan con la animación: 54 actualizaciones y dibujos coinciden con
MAME en host y ares. El solver genérico y el controlador completo siguen pendientes.

El [bloque común del soldado](docs/SOLDIER_CONTROL.md) calcula ahora sus flags
con las condiciones originales del RNG: 55 ciclos alimentan 54 movimientos y
dibujos nativos, verificados en host y ares. Las consultas delanteras y el orden
RNG global siguen siendo entradas externas; aún no es la IA completa.

La [máquina de estados recuperada](docs/SOLDIER_STATE.md) calcula ahora las
velocidades, seis inicios de animación y las transiciones observadas durante
54 actualizaciones. El selector de acciones se compara además con 84 llamadas
originales, incluida una selección de ataque. Esto aún no sustituye los guardias
provisionales ni completa la Misión 1.

La entrega pendiente incluye la **intro completa y sonido**. El nuevo
[backend de audio y las referencias de intro](docs/INTRO_AUDIO.md) ya tienen
pruebas internas en ares. La [entrada en paracaídas](docs/MISSION_ENTRY.md) ya
entrega el control a Marco con audio; la presentación completa y el resto de
la misión siguen pendientes.

El [ataque observado](docs/SOLDIER_ATTACK.md) ya produce eventos nativos de paleta
y creación diferida, con 20 dibujos del soldado y 41 del objeto hasta su retirada.
Las respuestas de impacto siguen externas y la integración jugable está pendiente.

El [reposicionamiento nativo](docs/SOLDIER_LOCOMOTION.md) añade 34 actualizaciones
con selección de poses, RNG y contacto de pendiente. La escena jugable parcial
existente conserva su recorrido en ares tras corregir el sesgo fraccionario.

**v10 incorpora la cuadrícula original de colisiones de 8×8**, la velocidad
de marcha sobre pendientes y los gráficos del carro y el toldo del mercado.
Marco puede aterrizar en superficies a distintas alturas y caer de ellas.
La v9 anterior está confirmada por el usuario en MiSTer.

Se conserva el escenario observado hasta el pueblo de la misión 1, tres capas
con parallax, CI4/RDP y streaming PI DMA. El salto usa la ecuación aérea original.

| Control | Acción |
| --- | --- |
| Cruceta o stick | Mover a Marco y avanzar la cámara |
| A | Saltar; soltar antes del siguiente salto |
| B | Disparar; mantener para repetir |
| R | Reiniciar la zona |
| START | Alternar escena y laboratorio de tiles |

Puedes recorrer el tramo, saltar y combatir con los tres guardias provisionales.
El tramo llega ahora a la cámara 1920 (antes 872), con el cambio de banco medio
y la subida de cámara medidos; véase [la ampliación](docs/MISSION1.md#ampliación-hasta-la-cámara-1920).
`REGION END` marca el límite del mapa observado, no el final de la misión original.
Las cajas de prueba de v8 se han retirado. **Sigue siendo un port en desarrollo**:
faltan mapa completo, resolvedor de colisiones completo, lógica original y cambios dinámicos del
escenario, HUD original, audio, granadas, vehículos y el resto de enemigos/jefes.
No se afirma igualdad 1:1 del juego ni rendimiento garantizado en hardware.

Véase [colisiones y objetos v10](docs/COLLISION.md),
[recuperación del escenario y movimiento v9](docs/MISSION1.md),
[núcleo RDP de v8](docs/RDP_IMPLEMENTATION.md) y
[esquema de portabilidad](docs/ESQUEMA_PORT_N64.md) y [arquitectura objetivo](docs/N64_ARCHITECTURE.md).

## Compilar

Linux x86-64 con GCC/G++, make, Python 3, Git, curl y dpkg-deb:

```sh
bash tools/setup.sh
python3 tools/import_rom.py /ruta/a/mslugx.zip
# Si faltan assets/sidecars: seguir docs/RDP_IMPLEMENTATION.md, docs/MISSION1.md y docs/COLLISION.md.
# python3 tools/export_marco.py /ruta/capturas
make -j4
make check-core
make check
```

`make check-core` no requiere SDK ni ROMs: usa código y los CSV numéricos
versionados. CI lo ejecuta en cada push y PR. `make check` también es host y
requiere los metadatos/assets derivados que utilizan las pruebas completas.
`make check-cinematic` añade los 230 frames del segmento local de entrada.
`make -j4` compila el modo 2.5D (`depth25d`); `make -j4 MSX_VISUAL_MODE=classic`,
la ruta plana. `make preview-depth` genera vistas previas offline de los
efectos sin arte original.
Las [correcciones del PR #2](docs/PR2_REVIEW.md) están adaptadas a esta rama.

Resultado: `mslugx-explorer.z64`, aproximadamente 50 MiB. Las ROM originales
se comprueban contra CRC32 y SHA1 de MAME. ZIP, assets y ROM generada quedan
ignorados por Git. La ROM compilada contiene gráficos aportados por el usuario.

El SDK se instala fuera del checkout en `/workspace/toolchains`. Puede cambiarse
con `MSX_SDK_ROOT` y pasando `N64_INST` a make. El script fija un commit y checksum;
si cambia la release continua, hay que revisar la versión, no omitir el checksum.

## Controles del laboratorio gráfico

Entrar con START desde la escena de Marco. Modo C16: tiles de 16x16 y paleta gris.

| Control N64 | Acción |
| --- | --- |
| Cruceta izquierda/derecha | Página anterior/siguiente |
| Cruceta arriba/abajo | Saltar +/-16 páginas |
| L/R | Saltar -/+256 páginas |
| A | Alternar FIX y C16 |
| Z | Unir/separar tiles consecutivos en la cuadrícula |
| B | Invertir la escala de grises |

Cada modo conserva su página. Navegación circular en ambos sentidos.
Unir tiles no reconstruye automáticamente una animación: faltan las tablas de
ensamblaje del juego. La escena jugable utiliza las paletas originales recuperadas; el laboratorio es gris.

## MiSTer

Usar el core **N64**, con SDRAM y arranque configurados según su documentación.
Copiar el `.z64` a `games/N64/` y cargarlo en el OSD. La ROM mide más de 16 MiB:
según el core, un módulo SDRAM de 32 MiB solo admite cartuchos de hasta 16 MiB;
para esta versión se necesita SDRAM suficiente (64/128 MiB).

Comprobar título `MSX V10 - MISSION 1`, avance de cámara, salto, disparo y combate contra los soldados.
Con START comprobar `MSX v10 - Graphics lab`, cuadrícula y contador. Probar cambio de
página, salto L/R, cambio A, unión Z e inversión B. Comparar las letras en FIX
con la v2 ya probada. Los márgenes están ajustados para CRT; registrar cualquier
recorte, versión del core y tamaño de SDRAM.

Para emulador usar ares con Homebrew mode y core N64 incluido; el paquete Debian
134 no lo incluye. No basta con que el proceso ares abra una ventana: debe
mostrar el contenido y responder a entradas. Consola N64 con flashcart sigue
siendo una prueba adicional pendiente.

## Estado y siguientes pasos

El usuario confirmó v9 en MiSTer; v10 pendiente de prueba física; véase [evidencia y pruebas](docs/PROGRESS.md).
La página FIX 8 es negra porque sus datos son todos cero.

El siguiente trabajo es completar el mapa y su resolvedor de colisiones, ampliar animaciones
de combate y añadir audio. El original incluye
68000, Z80 y YM2610: disponer de las ROM y la BIOS no equivale a tener código
fuente recompilable. Se está investigando una reconstrucción nativa 2D.

## Fuentes

- [libdragon](https://github.com/DragonMinded/libdragon), commit `e356bf3f56f7afbf7e5246329562f145965cfdfc`.
- [Instalación](https://github.com/DragonMinded/libdragon/wiki/Installing-libdragon).
- [Toolchain](https://github.com/DragonMinded/libdragon/releases/tag/toolchain-continuous-prerelease).
- [ares](https://github.com/ares-emulator/ares).
- [Core N64 MiSTer](https://github.com/MiSTer-devel/N64_MiSTer).
- [MAME: hashes del set](https://github.com/mamedev/mame/blob/master/src/mame/snk/neogeo.cpp).
- [MAME: disposición gráfica FIX y C](https://github.com/mamedev/mame/blob/master/src/mame/snk/neogeo_spr.cpp).

## Recuperación del código original

El [índice de descompilación](decomp/README.md) contiene 5.168 cuerpos C
tentativos del 68000 y 154 del Z80. Se han inventariado P1, P2 y M1 completos;
la clasificación de bancos, tipos, límites y destinos indirectos sigue pendiente.
Esta salida automática no es todo el juego convertido a C nativo compilable.
El [estado de cobertura](decomp/coverage.json) mantiene `complete: false`.

La rutina de paletas [0x008512](decomp/recovered/rom_008512.c) ya tiene una
reconstrucción C manual contrastada con 18 ejecuciones controladas del M68000.
El [despacho de condiciones](decomp/recovered/condition-dispatch/README.md)
ya conserva el retorno CCR.C en C y coincide con 447 llamadas controladas.
Sus auxiliares se interceptaron con respuestas sintéticas y siguen pendientes
de recuperación/enlace nativo.

La [estrategia de recuperación completa](docs/WHOLE_GAME_RECOVERY.md) organiza
el trabajo por modelo, contratos y subsistemas integrados. Los
[análisis de dependencias y raíces](decomp/analysis/README.md) son evidencia
para planificar; no una declaración de que la descompilación esté terminada.
