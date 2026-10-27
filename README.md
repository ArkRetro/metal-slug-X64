# Metal Slug X para Nintendo 64: trabajo en curso

La próxima entrega de ROM será una **beta del port completo**. No se publican
más demos intermedias. Véase [estado de la beta](docs/BETA_STATUS.md).
El [modo visual 2.5D opcional](docs/RETRO25D.md) añade profundidad, normales,
iluminación y niebla dentro de las restricciones N64; sigue en validación interna.
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
`REGION END` marca el límite del mapa observado, no el final de la misión original.
Las cajas de prueba de v8 se han retirado. **Sigue siendo un port en desarrollo**:
faltan mapa completo, resolvedor de colisiones completo, lógica original y cambios dinámicos del
escenario, HUD original, audio, granadas, vehículos y el resto de enemigos/jefes.
No se afirma igualdad 1:1 del juego ni rendimiento garantizado en hardware.

Véase [colisiones y objetos v10](docs/COLLISION.md),
[recuperación del escenario y movimiento v9](docs/MISSION1.md),
[núcleo RDP de v8](docs/RDP_IMPLEMENTATION.md) y
[arquitectura objetivo](docs/N64_ARCHITECTURE.md).

## Compilar

Linux x86-64 con GCC/G++, make, Python 3, Git, curl y dpkg-deb:

```sh
bash tools/setup.sh
python3 tools/import_rom.py /ruta/a/mslugx.zip
# Si faltan assets/sidecars: seguir docs/RDP_IMPLEMENTATION.md, docs/MISSION1.md y docs/COLLISION.md.
# python3 tools/export_marco.py /ruta/capturas
make -j4
cc -std=c11 -Wall -Wextra -Werror tools/test_fixed.c -o /tmp/msx-test-fixed
/tmp/msx-test-fixed
cc -std=c11 -Wall -Wextra -Werror tools/test_sprites.c -o /tmp/msx-test-sprites
/tmp/msx-test-sprites
make check
```

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
