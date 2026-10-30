# Backend nativo con libdragon

La lógica nueva se traduce a C y se compila para MIPS con el SDK libdragon ya
instalado. No se ejecuta un M68000/Z80 dentro de N64, ni se interpretan las
instrucciones originales durante el juego. La recuperación es manual y
verificada; no es un recompilador estático universal ni un port completo.

## A. Lógica y hardware separados

`src/neo_vehicle.c/.h` implementa el caso observado del blindado terrestre:

- Movimiento Q16.16 y parada al cruzar el objetivo, conservando el sobrepaso.
- Salud, umbral estricto de daño por debajo de la mitad y animación por ticks.
- Dos cajas de impacto originales, para el cuerpo y la cabina, con reflejo.
- Destrucción y eventos independientes de humo, geometría de restos y explosión.

No incluye libdragon, registros Neo Geo, direcciones de objetos RAM ni funciones
que pinten píxeles. `neo_vehicle_step()` devuelve eventos; `gameplay.c` aplica
la huella de colisión al recibir `NEO_VEHICLE_STAMP`. El evento se emite una vez.
`neo_collision.c` conserva la cuadrícula inicial y crea una copia mutable al
primer cambio. Los restos sobreviven a la pérdida de vida del jugador; el
reinicio completo restaura las colisiones originales.

El renderer (`src/vehicle_renderer.c`) consulta posición, pose y orientación;
no avanza la física ni resta vida. Los estados de animación y las huellas de
colisión se compilan a tablas C desde datos recuperados. Los números de rutina
del original quedan en las herramientas de análisis, no como punteros N64.

El vehículo está conectado al motor mediante `msx_game_vehicle_reference()` en
una **configuración interna de referencia**. El escenario predeterminado no
inventa un evento de oleada ni utiliza los tres guardias provisionales para
hacerlo aparecer. Falta recuperar esa parte de la misión.

## B. Tipos, endian y alineación

N64 también usa big endian con este toolchain. Eso no convierte las estructuras
de un motor M68000 en estructuras C utilizables por un cast: sus campos,
padding, punteros y reglas de alineación son distintos.

| Dato nativo | Tipo | Significado |
| --- | --- | --- |
| Posición, velocidad, objetivo | `int32_t` | Q16.16, unidades del mundo |
| Vida y vida inicial | `uint16_t` | Enteros sin signo, daño saturado a cero |
| Orientación | `int8_t` | -1 izquierda, +1 derecha |
| Estado, pose, fase, duración | `uint8_t` | Índices explícitos, sin punteros 68000 |
| Eventos | `unsigned` | Máscara devuelta al motor |

Se usa el alineamiento natural del ABI MIPS y se comprueba con `_Static_assert`
la alineación de los campos de 32 bits. No se usa `packed` para la lógica.
Los snapshots se decodifican por campos en las herramientas. Para importar
campos binarios en C, el patrón es leer bytes, por ejemplo:

```c
static uint16_t read_be16(const uint8_t *p) {
    return ((uint16_t)p[0] << 8) | p[1];
}
static uint32_t read_be32(const uint8_t *p) {
    return ((uint32_t)p[0] << 24) | ((uint32_t)p[1] << 16) |
           ((uint32_t)p[2] << 8) | p[3];
}
```

Ese código admite direcciones de archivo sin alinear. Para convertir un valor
sin signo mayor que `INT32_MAX` a un negativo de 32 bits, se resta 2^32 en un
intermedio `int64_t`; no se depende de un cast de memoria o del endian del host.
Los assets `.sprite` los carga `sprite_load()` de libdragon. Los buffers PI y
los datos para RDP mantienen sus requisitos de alineación separados del estado
lógico de los actores.

## C. Punto fijo y colisiones

El controlador no usa coma flotante ni divisiones flotantes. La velocidad
observada de 2,5 píxeles por tick de objeto se expresa como `5 * 65536 / 2`;
la posición conserva su fracción al detenerse. En la referencia, desde X=855
avanza a X=930 en 30 ticks, y se detiene en X=937,5 al superar el objetivo 936.
No se fuerza el resultado a 936 ni se redondea cada paso a píxeles enteros.

Las cajas de impacto usan intermediarios `int64_t` para las comparaciones de
segmentos barridos y un anclaje entero, como las palabras de coordenadas que
lee la rutina de colisión original. El render convierte las posiciones a píxeles al
pintar; el objeto permanece en coordenadas del mundo, con Y positivo hacia
arriba. La conversión del punto de apoyo al lienzo N64 es `screen_y=248-y`.

El suelo anterior puede existir debajo de la rampa nueva. Al caminar se elige
el contacto superior alcanzable dentro de la tolerancia de paso de cuatro
píxeles, evitando atravesar los restos para seguir pegado al suelo inferior.
No se amplía esa tolerancia ni se modifica la ecuación del salto. Las 350
comparaciones de marcha, los 24 estados de tejado y los 22 estados aéreos
recuperados siguen pasando después del ajuste.

## D. Tick arcade, VBlank y controles

`main.c` inicializa `display_init(RESOLUTION_320x240, DEPTH_16_BPP, 2,
GAMMA_NONE, FILTERS_RESAMPLE)` y `joypad_init()`. Cada iteración usa
`joypad_poll()`, obtiene botones con `joypad_get_buttons()` y conserva el stick
analógico mediante `joypad_get_inputs()`.

La asignación existente se mantiene: D-pad/stick para dirección, A para salto,
B para disparo, START para el laboratorio y R para reinicio. Z queda reservado
para granadas cuando se integre su lógica. No se simula una granada al pulsarlo
mientras esa mecánica no está implementada.

El flujo real es:

```text
joypad_poll / botones -> display_get -> actor_draw
    reloj de ticks -> msx_game_update -> neo_vehicle_step / eventos
    superficie -> fondo -> vehicle_renderer_draw -> actores -> HUD
    scene_render_end -> fin de RDP -> display_show
```

`display_get()` espera un buffer disponible y libdragon presenta con el VI.
La llamada a `display_show()` del camino RDP se hace en el callback de finalización
para no mostrar un framebuffer que el RDP aún esté escribiendo.

No se avanza la lógica una vez por framebuffer: `scene_clock_steps()` mantiene
la cadencia arcade de 59,185606 ticks/s y el controlador de este objeto avanza
cada dos ticks. La presentación N64 puede ser aproximadamente 60 Hz NTSC o
50 Hz PAL, conservando la velocidad del juego mediante el acumulador. El
catch-up conserva la deuda de simulación. La sincronización por sí sola no
certifica 60 FPS: el trabajo CPU/RDP debe caber en el presupuesto de cada frame.

## Assets `.sprite` y TMEM

`tools/export_vehicle.py` recupera nueve poses desde los descriptores C-ROM y
las tablas de animación. Se contrastan 54.413 píxeles con MAME y 287 píxeles
ocultos con sus sprites oclusores originales. Se conservan también la paleta
normal y la paleta amarilla de impacto; no se usa un rectángulo blanco como
sustituto de esa paleta.

```sh
python3 tools/export_vehicle.py /ruta/mslugx.zip /ruta/capturas-ramp-return
make -j4 MSX_SCENARIO=vehicle MSX_VISUAL_MODE=classic
```

El build llama a `mksprite -f CI4 -D NONE -c 0`. Cada pose 128x64 se divide en
dos superficies de 64x64: **2048 bytes de texels por transferencia**, dejando
la otra mitad de TMEM para TLUT. Se comprueba el formato y el stride reales;
no se confunde el tamaño del archivo `.sprite` con su ocupación en TMEM.
En el SDK fijado, el contador devuelto por `rdpq_sprite_upload` acaba en una
función con retorno cero sin implementar, por lo que no se usa como medición.
El uploader mantiene sus propias comprobaciones de TMEM.

Los 36 fragmentos (9 poses, 2 paletas, 2 mitades) se precargan para el caso de
referencia y se mantienen vivos mientras RDP los usa. No se leen archivos en
el bucle de dibujo. La configuración cabe holgadamente en 8 MiB; el streaming
de la campaña completa requerirá ampliar la política de residencia existente.
El sampler es `FILTER_POINT`; después de dibujar se invalida el seguimiento de
texturas/paletas del renderer anterior para evitar reutilizar una TLUT errónea.

## Alcance y comprobación

`make check` incluye el controlador nativo, cajas reflejadas, movimiento
recuperado, daño, emisión única de eventos, mutación de 13 celdas, reinicio y
caminar sobre los restos. En la captura de pistola se observan decrementos de
10 por bala; la misma partida contiene granadas de 100, que no se confunden
con daño de pistola.

La prueba interna de ares se ejecuta con:

```sh
python3 tools/test_vehicle_ares.py /ruta/rom-interna.z64
# Volver al escenario y modo visual predeterminados (campaña, depth25d):
make -j4 MSX_SCENARIO=campaign MSX_VISUAL_MODE=depth25d
```

Sin `MSX_VISUAL_MODE=classic`, el escenario del blindado se dibuja en el
[modo 2.5D](RETRO25D.md): sombra de contacto bajo las ruedas y pulsos de luz en
los impactos, al empezar la destrucción y en la explosión. Son efectos de
dibujo; no cambian daño, colisión ni restos.

La cadencia de disparo, el retroceso del jugador y la duración del destello
de impacto conservan todavía las reglas provisionales del escenario anterior;
no se presentan como rutinas ya recuperadas del original.

Faltan la oleada y la aparición original del vehículo, su ataque y el resto de
la IA, así como los gráficos específicos de humo/explosiones asociados a los
eventos recuperados. Este componente no completa el port ni convierte el
escenario de referencia en una beta. No se publica una ROM o demo nueva.

## Recuperación de código ampliada

La exportación tentativa y la traducción nativa nueva de transiciones de Marco
se documentan en [DECOMPILATION.md](DECOMPILATION.md). El borrador Ghidra de
`decomp/` nunca se enlaza con el port; la unidad C comprobada es
`src/marco_environment.c`, con servicios de mundo explícitos y pendientes.
