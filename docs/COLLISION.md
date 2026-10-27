# v10: colisiones del mapa, pendientes y objetos del mercado

V9 está confirmada por el usuario en MiSTer. V10 reemplaza el perfil de suelo
interpolado por la cuadrícula de colisiones observada en el arcade y añade el
carro/toldo originales. El tramo jugable sigue terminando en el pueblo.

## Recuperación

La rutina original en 0xC79FE consulta una cuadrícula circular de 64×64 celdas
de 8×8 píxeles. Para el primer mapa está en RAM 0x10C72E + 0x88; su índice es
`((x & 504) << 3) | ((~y & 504) >> 3)`. X/Y son coordenadas enteras locales del
mapa, con Y creciente hacia arriba. La cuadrícula se reutiliza al hacer scroll:
no se puede volcar una instantánea y repetirla para toda la misión.

tools/extract_collision.py fusiona únicamente ventanas observadas en 36 capturas
de MAME. Obtiene 149×32 celdas, 182 no vacías, sin conflictos entre capturas.
Conserva el byte completo de cada celda. El compilador genera una tabla de
4768 bytes; el ejecutable no contiene el programa 68000 ni necesita BIOS.

El nibble bajo identifica la forma: 1..4 son cuartos de una pendiente ascendente
1:4; 6..9 son cuartos descendentes; 11 es una superficie plana. El código C
también expresa las formas de pendiente 1:1 y otras superficies planas, aunque
no aparecen en esta región y no se presentan como verificadas contra MAME.
Los flags altos se conservan, pero su semántica completa sigue pendiente.

La altura usa Q16.16 y la fracción X real, no muestras ni interpolación entre
pasos. El contacto de las pendientes conserva el desplazamiento de una unidad
fraccional observado en el arcade. La marcha toma los coeficientes de la rutina
original: velocidad horizontal 3 en plano o 2,90625 en pendientes 1:4, con
componente vertical ±0,7265625. La ecuación aérea recuperada en v9 permanece.

La búsqueda usa la altura actual, no el suelo más alto de toda la columna:
permite caminar bajo el toldo y aterrizar encima al descender. Un barrido vertical
selecciona la primera superficie cruzada en la bajada. Caminar más allá del carro
activa la caída. No hay ajuste hacia superficies superiores mientras se asciende.
Los límites laterales observados mantienen al jugador dentro de la cámara;
el límite derecho del tramo conserva la fracción original (1164−1/65536).

## Gráficos asociados

El carro y el toldo no pertenecen a las tres capas de fondo recuperadas en v9:
se dibujan mediante otros SCB. tools/extract_market_props.py recupera 51 tiles
de las paletas 0x76/0x78 del frame 2520. Los gráficos proceden de C-ROM, las
paletas de RAM y las posiciones/orden de SCB. Los PNG son referencias de prueba.

Esos tiles comparten los atlas CI4 existentes. A diferencia de las celdas de
fondo, se solapan: se conserva su orden de dibujo original, sin ordenar por
textura. Se dibujan sobre las capas del fondo y antes de los actores nativos.
Se verificaron 3830 píxeles visibles y 828 tapados por otros sprites, resolviendo
el sprite superior y comprobándolo también contra MAME. No se reconstruyen aún
las interacciones, destrucción, fuego ni personajes que ocupan el puesto.

Conjunto total: 234 páginas, 74 paletas, 30 chunks. 491520 bytes decodificados
(480 KiB) y 275264 bytes comprimidos. Cabe completo en las cachés de 4/8 MiB
configuradas; este conjunto todavía no ejercita evicción.

## Validación y reproducción

```sh
python3 tools/extract_collision.py /workspace/research/mission1
python3 tools/extract_market_props.py /workspace/research/mission1
make check
make -j4
```

Los datos de prueba originales son posiciones y velocidades en data/:

- 350 pasos de marcha: error máximo 2 unidades Q16.16 en X/Vx y 1 en Y/Vy,
  es decir, menos de 0,00004 píxeles. La comparación usa pasos con movimiento;
  no acredita la planificación temporal de todas las entidades del arcade.
- 24 estados de salto vertical sobre el carro, incluido contacto y reposo:
  X/Y/Vx/Vy coinciden bit por bit con la captura roof-original de MAME.
- Los 22 estados aéreos originales de v9 siguen coincidiendo bit por bit.
- Pruebas de límites de mapa, superficies apiladas, paso hacia la izquierda,
  salida de plataforma, margen de cámara, reaparición y apoyo tras knockback.
  Combate/reaparición continúan siendo sistemas provisionales.

La referencia del carro se obtiene con tools/capture_roof.lua. No dispara,
se detiene cerca de X=600 y salta verticalmente. La invulnerabilidad de captura
preserva los otros bits del estado con OR; no se incorpora a la ROM N64.

Para el recorrido de entrega en ares 146:

```sh
bash tools/run_ares.sh > /workspace/emulator/run-v10-release.log 2>&1
# En otra terminal, con la ROM recién iniciada:
DISPLAY=:99 LD_LIBRARY_PATH=/workspace/emulator/root/usr/lib/x86_64-linux-gnu \
  python3 tools/test_mission_ares.py --version 10
```

La prueba recorre dunas y pueblo, aterriza en el carro, cae al suelo, combate
contra tres guardias provisionales, llega al límite, reinicia y vuelve desde
el laboratorio. Capturas: /workspace/emulator/v10-release. Se inspecciona también
el gráfico del carro/toldo: una superficie de colisión sin su objeto no basta.

## Lo que sigue pendiente

Este es un resolvedor nativo de superficies observadas, no la rutina general
completa del arcade: faltan objetos dinámicos, paredes/techos generales, casos
de esquina y todos los flags/materiales. El error fraccional de la marcha
demuestra que aún no se ha reproducido todo el algoritmo original.
IA, armas/daño, checkpoint y animaciones/pivotes no son todavía lógica 1:1.
Faltan el encuentro del pueblo, el resto del mapa, HUD, audio, granadas, vehículos
y jefes. No se garantiza rendimiento en consola N64 por haber pasado ares.

Las nuevas referencias de avance en /workspace/research/mission1-advance llegan
más allá de la cámara 872, pero no se incorporaron a la entrega: muestran
redondeos de parallax y cambios en la cuadrícula que los extractores actuales
detectan y rechazan. Se conserva la aserción en lugar de aceptar datos ambiguos.
