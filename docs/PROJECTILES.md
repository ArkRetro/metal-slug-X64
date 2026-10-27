# Proyectiles originales

Los disparos horizontales ya se dibujan con el sprite de la pistola de Metal
Slug X. Se retira el rectángulo uniforme de 8x3. El gráfico original ocupa
10x6 píxeles dentro de un tile 16x16, con 44 píxeles opacos y contorno ovalado.
Se conservan los índices originales, la transparencia del pen cero y los
colores de su paleta, cuantizados a RGBA5551 para N64. No se aplica filtrado
bilineal al proyectil.

La fuente es el tile C-ROM `0x2cbd3`, observado con paleta `0x71` en MAME
0.276, set `mslugx`, BIOS `euro`. `tools/export_projectiles.py` resuelve los
slots SCB, posición, zoom, atributos y flips. Contrasta cada píxel opaco con
la captura independiente de MAME; una discrepancia solo se acepta si otro
sprite posterior explica su color. Rechaza zoom, autoanimación o cambios de
paleta no implementados. En la captura actual hay **18 observaciones y 792
píxeles coincidentes**, sin discrepancias.

El empaquetador verifica el hash del CI4 y la conversión de ida y vuelta
índice/paleta. Recorta únicamente los márgenes transparentes, conserva su
desplazamiento dentro del tile y añade un banco de proyectiles a la caché
existente. El centro geométrico del tile, (8,8), se registra sobre la posición
actual de la bala; no se afirma haber recuperado el pivote lógico del programa
arcade. El reflejo horizontal conserva ese centro, también al desplazarse la
cámara. Cadencia, trayectorias y colisiones conservan la lógica provisional.

Los disparos horizontales del enemigo provisional comparten este gráfico de
pistola. Todavía no representan su arma específica original: el primer soldado
observado en la nueva captura lanza granadas. No se sustituyen esas granadas
por una falsa animación de bala ni se presenta este cambio como recuperación
completa del sistema de armas.

## Regeneración

Usar los ZIP propios ya importados y el MAME indicado en [MARCO.md](MARCO.md).
Crear directorios nuevos de capturas, configuración y NVRAM. El script arranca
desde BIOS, dispara al principio y se acerca a los enemigos sin eliminarlos.
Los ajustes de tiempo e invulnerabilidad son exclusivamente de captura.

```sh
MSX_CAPTURE_DIR=/ruta/capturas-proyectiles mame mslugx \
  -rompath /ruta/roms -bios euro -video soft -sound none -window \
  -skip_gameinfo -nothrottle -autoboot_delay 0 \
  -autoboot_script "$PWD/tools/capture_projectiles.lua" \
  -cfg_directory /ruta/cfg -nvram_directory /ruta/nvram
python3 tools/export_projectiles.py /ruta/capturas-proyectiles
make -j4
```

`filesystem/projectiles.json` contiene el asset generado y sus observaciones;
no se publica la ROM arcade ni la BIOS. En esta máquina las referencias están
en `/workspace/research/projectiles`.

## Validación interna

`make check` pasa. Ambas ROM internas (clásica y 2.5D) pasan en ares 146 con
paraLLEl-RDP usando `tools/test_mission_ares.py --version 10 --variant classic
--projectiles` y la variante `retro25d`: disparos del jugador a derecha e
izquierda, disparo enemigo, salto en carrera, tejado, tres encuentros
provisionales, límite de región, reinicio y regreso del laboratorio.

Las capturas internas muestran la silueta, transparencia y colores del
proyectil. No hubo aserciones de TMEM ni fallos de caché durante el recorrido.
El paquete conserva 30 chunks y 480 KiB de caché en esta escena. Esto no
certifica el rendimiento de una campaña completa. Los hashes de las ROM de
prueba están en [projectile-validation.json](projectile-validation.json).

No se publica otra demo ni una ROM descargable; continúa vigente la entrega
reservada para una beta del port completo.
