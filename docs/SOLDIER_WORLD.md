# Mundo nativo de la región jugable: guion, soldados y objetos lanzados

Continúa [SOLDIER_KERNEL.md](SOLDIER_KERNEL.md). Este paso cierra el bucle de
los soldados rebeldes de la Misión 1: el guion de nivel, el constructor y la
familia de rutinas del soldado, su muerte, los objetos que lanza y la salpicadura
de muerte se ejecutan en C en el mismo orden de lista del original. Los tres
"guardias" provisionales de `gameplay.c` desaparecen. No se interpreta código
68000 durante el juego; cada pieza traducida tiene un fixture obtenido de MAME.

## Componentes traducidos

| ROM | Componente nativo | Evidencia dinámica |
| --- | --- | --- |
| `0x019E72/0x019F1A`, `0x01A056/0x01A0A6/0x01A372`, `0x01A422/0x01A438`, `0x01A5CA` | `src/neo_script.c`: actor del guion (ops 0..3), sentencias 6..10 (aparición, lanzador periódico, bloque, elección aleatoria `0x29F2`, condicional, `F`/`10`), actores lanzadores y condiciones (recuento por grupo, total, siempre, "sin jugador"). | 1811 actualizaciones, 52 apariciones y 19 tiradas del RNG en cuatro capturas (`tools/test_neo_script.c`). |
| `0x0AF17E/0x0AEF30` | Constructor del soldado: tablas `0x2D3C16/0x2D3B54` por rango, RNG, paleta `0x20E`, búsqueda de suelo `0x0146E8`, orientación, objetivo y prefijo de control. | 24 construcciones desde apariciones nativas (campos, bloque `+6E..+93`, RNG). |
| `0x0AE9C0..0x0B09B0` | 36 rutinas de la familia del soldado (`src/neo_soldier_ai.c`), colas de retirada/eliminación, daño `0x4892..0x499A`, salpicadura y decaimiento de la rejilla de retardo. | 2596 actualizaciones abiertas (`tools/test_neo_soldier_ai.c`). |
| `0x0406B6/0x0407CA`, `0x04097A`, `0x040D66/0x040A10`, `0x040DB4/0x040DF0` | Muerte por pistola en suelo y en aire; entradas `0x040DC0/0x040DD8/0x040A28` de las tablas de despacho (solo arrancan un programa). | Pistola: capturas `fire3`/`entry-fire3`. El cuchillo del jugador (tipo 2) solo se observa una vez. |
| `0x040ED0`, `0x040F88/0x040FD4` | Salpicadura de muerte: hijo insertado tras el soldado (`0x0040BC`), programa `0x2402FA`, paleta `0x3BA`. | Orden de lista y RNG del mundo. |
| `0x03BB8E`, `0x03BC28..0x03BE8E`, `0x01C8FC` | Objeto lanzado: insertado tras el actor del guion (`0x00411C`), vuelo completo por el kernel nativo, ataque de vuelo `0x0E0894` contra el jugador, explosión por contacto (`0x03BE52`) o impacto (`0x03BE6C`). | 152 posiciones en las capturas. |
| `0x00469C`, `0x019E40` | Actor de misión: decaimiento de una fila de la rejilla y recuento de grupos al inicio de cada actualización. | Rejilla comparada tras cada actualización. |

`src/neo_world.c` ordena todo como la lista original: actor de misión, guion,
y después cada actor en orden de creación, con los nuevos insertados justo
detrás de su creador (`0x0040BC`); los objetos lanzados se insertan detrás del
guion y entran en la actualización siguiente. El constructor del soldado se
ejecuta cuando el recorrido alcanza el nodo, como en el original.

## Prueba cerrada del mundo

`tools/extract_soldier_world.py` genera `data/soldier_world_fixture.json.gz`
con, por actualización: desplazamiento de escena, época, contador de vídeo,
jugador (palabras proyectadas, `+15`), orden de lista de los actores del mundo,
registro completo de llamadas a `0x0029C6` con el valor devuelto (reconstruido
avanzando el anillo volcado; se comprueba la igualdad con el volcado siguiente),
balas del jugador `0x01B882` con su posición en la lista, y los resultados
esperados: anillo del RNG, posición del guion, contadores de grupo, rejilla de
retardo, estado de cada soldado y posición de cada objeto lanzado.

`tools/test_neo_world.c` ejecuta el mundo en bucle cerrado durante las cuatro
capturas. Se inyectan únicamente las entradas de actores que el port no tiene,
en su posición original de la lista: 419 llamadas al RNG (jugador, chispas de
bala, entidades `0x0400AA/0x099DCC/0x099E06`), las balas del jugador (paso
aéreo por el kernel y `0x0049E8` como en la prueba abierta), un impacto de
cuchillo y cuatro salpicaduras de rejilla por muertes de entidades externas.

**Resultado:** 1811 actualizaciones, 2696 estados de soldado y 152 posiciones
de objetos lanzados coinciden; el anillo del RNG coincide tras cada
actualización. Dos bytes de nodos reutilizados (`+24/+26` hasta el primer
arranque de animación; `+5B..+5D` hasta el primer impacto) quedan como los
dejó el ocupante anterior y no se comparan hasta que el soldado los escribe.

## Integración en el juego

`msx_game_t` sustituye `msx_enemy_t` y las balas enemigas por `neo_world_t`.
El mundo se actualiza en los mismos ticks alternos que el jugador con sus
palabras proyectadas (`x - cámara`, `256 + y original`). Los disparos del
jugador se resuelven con la fase estrecha original contra los soldados
registrados (`0x01B4C2`: tipo 6, clase 1, daño 10; vida 0 → muerte de un
impacto). Los impactos de cuchillo (`0x0AEF14`) y del objeto en vuelo llaman a
`msx_game_hurt` con la posición del atacante. La región termina cuando el guion
ha pasado sus esperas "todos los grupos a cero" (`0x2D650E/0x2D6514`) y el
jugador está en el límite del mundo. La cámara y los límites del mundo no se
tocan.

Valores re-medidos en `tools/test_gameplay.c` (andar a la derecha disparando):
8 soldados creados por el guion, 7 muertos de un impacto, uno sale de escena,
7 apariciones externas contadas, sin daño recibido, checkpoint 640, cámara 872.
Un jugador parado tras la primera aparición recibe el cuchillo.

## Gráficos

`tools/export_soldier_sprites.py` decodifica los 487 descriptores de sprite
que dibujan los programas convertidos (ROM P `[grupos][atributo]`, grupos
`x,y,cols,filas` y teselas `palabra | (atributo & 0xF0) << 12`) y resuelve las
paletas lógicas (`0x009EA8`) por la tabla del asignador en RAM `0x1068EA`
(índice de entrada = ranura de paleta, `+A` = recurso). Se verifican contra las
imágenes de VRAM de las capturas `mission1`: 25 actores coinciden píxel a píxel
(11643 píxeles) en la posición en caché `+44/+48` (la VRAM volcada contiene la
lista de sprites de la actualización anterior); los no comparados son grupos
de soldados superpuestos con la misma paleta. Los dibujos reflejados quedan un
píxel a la izquierda. El pack de escena añade el banco `SCENE_NATIVE`
(descriptores partidos en partes ≤64×64, paleta sustituida por dibujo) y las
tablas `scene_native_descriptors`/`scene_native_palettes`.

```sh
python3 tools/export_soldier_sprites.py /ruta/mslugx.zip /ruta/captures/mission1 \
  --palettes /ruta/captures-soldiers/entry-walk3 /ruta/captures-soldiers/entry-fire3 \
  /ruta/captures-soldiers/walk3 /ruta/captures-soldiers/fire3
python3 tools/extract_soldier_ai_tables.py /ruta/captures-soldiers/entry-walk3 --output data/soldier_ai_tables.json
python3 tools/extract_soldier_script.py /ruta/walk3:3320 /ruta/fire3:2880 /ruta/entry-walk3:4970 /ruta/entry-fire3:4660 --output data/soldier_script_fixture.json.gz
python3 tools/extract_soldier_world.py  /ruta/walk3:3320 /ruta/fire3:2880 /ruta/entry-walk3:4970 /ruta/entry-fire3:4660 --output data/soldier_world_fixture.json.gz
make check && make -j4
```

## Pendiente (sin implementar nada especulativo)

- Entradas de la tabla de apariciones `0x0197DC` que el guion usa en la región
  y no están portadas: `0x099DCC/0x099E06`, `0x0400AA`, `0x03FC5A`, `0x0B6298`,
  `0x08C37C`, `0x03264E`, `0x031D80` (prisioneros, objetos de escena, entidades
  auxiliares). Se cuentan como externas; consumen RNG en el original, así que
  la secuencia aleatoria nativa diverge del arcade desde su primera aparición.
- Entradas de las tablas de muerte no observadas (`0x040C8E`, `0x040B46`,
  `0x040BA8`, `0x040A40`, `0x040AF6`, `0x040A7C`, `0x040BE6`): detienen al
  soldado explícitamente (`NEO_SOLDIER_UNSUPPORTED`, contador `unsupported`).
- Paletas lógicas `0x214/0x21A/0x221/0x385/0x37A`: no se observaron asignadas
  en ninguna captura; el renderer usa `0x20E` y lo cuenta.
- Rutinas del soldado no alcanzadas en las capturas (`0x0AF7B0`, `0x0AF7F6..`,
  `0x0B0396..`, `0x0B079E`): siguen explícitamente sin soporte.
- Sonidos (`0x30` de animación, `0x10FA`, `0x1027`): se emiten como peticiones;
  no hay reproducción nativa.
- Segundo jugador y otros rangos de dificultad: solo está verificado el perfil
  de un jugador (`0x100088=2`, `0x10FD8B=3`).
