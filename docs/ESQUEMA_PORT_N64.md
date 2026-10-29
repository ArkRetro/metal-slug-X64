# Esquema de portabilidad Neo Geo → Nintendo 64

Documento de arquitectura consolidado. Responde a tres preguntas: cómo entra el
arte de 16 colores en los 4 KiB de TMEM, qué camino RCP conviene para miles de
rectángulos 2D, y cómo se hace streaming de animaciones desde el cartucho con
8 MiB. Distingue en cada punto lo **implementado y medido** en este repositorio
de lo **propuesto**. Complementa a [N64_ARCHITECTURE.md](N64_ARCHITECTURE.md)
(especificación original) y [RDP_IMPLEMENTATION.md](RDP_IMPLEMENTATION.md)
(núcleo v8).

## 0. Corregir primero el modelo de origen

Antes de dimensionar nada hay que medir el origen con precisión, porque las
cifras habituales de Neo Geo exageran el problema:

| Afirmación habitual | Hardware real (MAME `neogeo_spr.cpp`) |
| --- | --- |
| 384 sprites por línea | 96 columnas activas por línea; el generador tiene 381 entradas |
| 1536 sprites simultáneos | 381 columnas de 16 px de ancho, cada una hasta 32 tiles de 16x16 encadenados verticalmente |
| «Todo son sprites» | Cierto para fondos, enemigos y vehículos; además existe la capa FIX de tiles 8x8 (texto/HUD) |
| Zoom libre | Zoom por tablas de selección/descarte de líneas y columnas, no muestreo afín |

El techo geométrico es 96 columnas × 16 px × 224 líneas = **344.064 muestras de
sprite por frame**, más FIX. Esa es la cifra que hay que cubrir, no «1536 sprites».

Este repositorio no emula el 68000: reconstruye el arte desde las C-ROM, recupera
la lógica con trazas de MAME y la reescribe en C nativo. La ROM final no
contiene BIOS ni programa 68000.

## A. TMEM (4 KiB) y segmentación de sprites

### Formato: CI4 + TLUT RGBA16

Implementado. El arte original es de 16 colores por tile con paleta por sprite;
CI4 lo representa sin pérdida de índices. Comparativa con TLUT activo (la
mitad superior de TMEM queda reservada a paletas):

| Formato | TMEM útil para texels | Tile 16x16 | Tiles residentes | Fidelidad |
| --- | ---: | ---: | ---: | --- |
| **CI4 + TLUT** | 2048 B | 128 B | 16 | Índices exactos; color cuantizado a RGBA5551 en la TLUT |
| RGBA16 sin TLUT | 4096 B | 512 B | 8 | Color igual que CI4, 4× más RDRAM y ROM |
| IA8 sin TLUT | 4096 B | 256 B | 16 | Pierde la paleta cromática: inservible como formato general |
| CI8 + TLUT | 2048 B | 256 B | 8 | No aporta nada a imágenes de 16 colores |

La TLUT admite 16 paletas CI4 (256 entradas × 8 bytes físicos). Un gestor LRU
por frame asigna slots y los invalida cuando la fuente (texto, niebla) pisa TMEM.
El color final es RGBA5551 en TLUT y framebuffer: el modelo de resistencias y el
bit *dark* de Neo Geo se cuantizan. La igualdad RGB8 con MAME exigiría framebuffer
de 32 bits y un camino de paleta expandido; no está implementado y no se afirma.

### Página natural: 64x64 CI4 = 2048 bytes

Cualquier gráfico se parte offline en páginas de 64x64 CI4 (16 tiles de 16x16),
alineadas a 64 bytes en RDRAM. Fórmulas, con b bits por texel:

    stride(W,b) = 8 * ceil(W*b/64)         bytes por fila en TMEM
    bytes(W,H,b) = stride(W,b) * H
    Hmax(W,b) = floor(2048 / stride(W,b))  con TLUT activo

Para CI4: una fila de 64 texels ocupa 32 bytes; 64 filas, 2048 bytes. Una columna
Neo Geo de 16×512 px (32 tiles encadenados) se divide en ventanas de 16×256
(2048 B cada una) o, mejor, se reempaqueta en páginas de 64×64 agrupando
columnas vecinas de la misma paleta, lo que reduce las cargas de TMEM a una por
cada 16 tiles en lugar de una por columna.

Reglas de corte adicionales, todas aplicadas por `tools/pack_scene.py`:

1. Cortar al cambiar de paleta: un rectángulo CI4 usa una sola paleta.
2. Recortar el lienzo transparente y deduplicar tiles idénticos (índices,
   independientemente de la paleta).
3. Subir solo la subregión útil (`rdpq_tex_upload_sub`), con `assert` de ≤2048 B.
4. No reordenar globalmente por textura: el orden de composición original manda.
   Solo se fusionan rectángulos contiguos con la misma página y paleta.

Cifras actuales del pack de la misión 1 (cámara 0..1920): **296 páginas, 101
paletas, 37 bloques, 606.208 B descodificados, 388.352 B en ROM**. Marco, sus
transiciones y el soldado ocupan 12 bloques fijados.

### Jefes y metasprites grandes

Propuesto. Un jefe de 256×512 px son 64 KiB CI4 = 32 páginas. Se dibuja en 32
rectángulos (o menos, tras recortar transparencias), una carga de 2 KiB por
página. La carga de TMEM cuesta aproximadamente lo mismo que rasterizar la
página, por lo que el coste total es lineal en texels visibles, no en «número de
sprites». El cuello de botella no es la TMEM sino evitar cargar dos veces la
misma página en un frame: por eso el empaquetado agrupa por paleta y vecindad.

## B. Microcódigo RCP para 2D

### Decisión: rdpq/rspq de libdragon, no Fast3D ni S2DEX

Implementado. Fast3D es geometría 3D con Z y transformaciones que aquí sobran.
S2DEX es la alternativa del SDK oficial; sobre libdragon no hace falta
reconstruir su API. `rdpq` encola comandos RDP a través de un overlay RSP
(`rspq`) con *fixups* y sincronización automática (`LoadSync`, `TileSync`,
`PipeSync`); el RSP no rasteriza píxeles, solo expande y envía comandos.

Modo fijo por frame: 1-cycle, point sampling, alpha compare (pen 0 transparente),
sin Z, sin filtrado 3-point. Cada sprite es un `TextureRectangle` con flip por
coordenadas S invertidas. COPY mode solo sirve para blits 1:1; el zoom de Neo
Geo se reproducirá con bandas precalculadas, no con escalado afín.

Reparto de trabajo:

- **CPU (R4300i):** lógica del juego a la cadencia original (59,185606 Hz,
  acumulador entero separado de la presentación), cámara, selección de frames,
  visibilidad, y una cola compacta de *draw items* (página, subrectángulo,
  paleta, posición, flip, prioridad).
- **RSP:** expansión de la cola rdpq y mezcla de audio (`mixer` + WAV64), en
  planificación compartida.
- **RDP:** cargas TMEM y rasterizado.

### Mitigar la latencia de RDRAM en LoadTile/LoadBlock

Implementado:

1. Las páginas ya están en el orden de lectura del RDP y alineadas a 64 bytes:
   una carga de página es una ráfaga lineal, sin acceso aleatorio.
2. El renderizador recuerda la última página/paleta/formato cargada y omite la
   carga si el siguiente rectángulo la reutiliza. Los frames de animación
   consecutivos comparten página siempre que es posible.
3. Un solo *fence* por frame (`rdpq_detach_cb`): ninguna página RDRAM se libera
   hasta que el RDP ha terminado el frame que la lee. No hay esperas de CPU por
   sprite ni `FullSync` intermedios.
4. Los datos generados por CPU (paletas, páginas descodificadas) reciben
   `writeback` de caché D antes de que el RDP los lea; los destinos DMA se
   invalidan antes de la transferencia.

Propuesto: fusionar runs horizontales compatibles y usar `LoadBlock` para
páginas lineales completas; medir, no asumir, cuál de las dos cargas es más
rápida en cada caso. Instrumentación existente por frame: draws, uploads de
textura, cambios TLUT, bytes TMEM, microsegundos de CPU. Desglose por fases (simulación, fondo, actores, esperas) en la línea
`MSX PERF`; ver [RENDIMIENTO.md](RENDIMIENTO.md).

### Microcódigo propio

No justificado por ahora. Solo tendría sentido si el perfil mostrase que la
expansión de metasprites en CPU domina el frame; con 344.064 muestras/frame el
límite previsible es el RDP y el ancho de banda RDRAM compartido con VI, PI y
audio, no la CPU.

## C. Streaming desde el cartucho con 8 MiB

### Dos cachés, no una

- **ROM → RDRAM** (PI DMA): bloques independientes de 16 KiB, RLE o crudo según
  cuál sea menor, con CRC32 por bloque. Un DMA activo, *staging* ping-pong de
  16 KiB y descodificación del bloque anterior solapada con el DMA del siguiente.
  La cola de peticiones tiene prioridades; `require` bloquea solo si el bloque
  es imprescindible para el frame actual y registra el *miss*.
- **RDRAM → TMEM**: el working set del grupo de draws actual, gestionado por el
  renderizador como se describe en A y B.

Capacidad de la caché RDRAM: 3 MiB con Expansion Pak (192 bloques), 512 KiB sin
él (32 bloques). Estados de un bloque: AUSENTE → PEDIDO → DMA → LISTO → FIJADO
/ DESALOJABLE; el desalojo exige que no haya referencias y que el *fence* del
último frame consumidor haya pasado.

### Presupuesto de RDRAM (8 MiB)

| Uso | MiB |
| --- | ---: |
| Código y estado del juego | 1,00 |
| Framebuffers RGBA16 320×240 ×2 y colas RSP/RDP | 0,75 |
| Assets fijados: Marco, HUD, efectos comunes | 0,75 |
| Caché CI4 de animaciones y escenario | 3,00 |
| Staging DMA y entrada comprimida | 0,50 |
| Audio (WAV64 en streaming) | 0,50 |
| Scratch, pilas, heap transitorio | 0,75 |
| Reserva | 0,75 |

El ELF actual mide 1,39 MB (texto 206 KB, datos 107 KB, BSS 150 KB); la ROM
51 MB, dentro del límite de 64 MiB de los flashcarts y de la SDRAM del core N64
de MiSTer.

### Prefetch por horizonte de cámara y por evento

Implementado para el escenario: el fondo pide las páginas de un margen de 48 px
a ambos lados de la cámara y la siguiente fase de autoanimación; Marco pide los
tres frames siguientes de su animación. El guión de nivel (`neo_world`, en
integración) conoce las posiciones de aparición de cada enemigo.

Propuesto para enemigos y explosiones:

    anticipación >= t_DMA + t_decode + espera_cola + margen
    t_DMA = bytes / ancho_de_banda_PI_medido

Con 8 MiB/s efectivos, 32 KiB tardan 3,9 ms y 256 KiB 31 ms: un arquetipo de
enemigo de 256 KiB no puede pedirse en el frame de su aparición. La regla es
cargar el arquetipo completo (idle, marcha, ataque, muerte) cuando la cámara
está a 3–6 ticks de su punto de aparición, no frame a frame. Las explosiones
críticas (granada, vehículo) permanecen fijadas durante toda la zona.

Prioridades de PI, de mayor a menor: audio con deadline, acciones inminentes de
Marco, enemigos activos, eventos próximos, escenario de entrada, precarga
especulativa. Una sola cola central; ningún actor inicia DMAs por su cuenta.

Medición actual con el pack de 37 bloques: peor working set estimado de 25
bloques en cámara 1056. Cabe sin desalojo con 8 MiB; con 4 MiB los 32 slots no
contienen el pack entero y el arranque carga la vista inicial en último lugar
para que quede residente. No hay todavía una prueba de streaming real en 4 MiB.

Un fallo de caché en juego no se resuelve omitiendo el sprite ni frenando la
simulación: en desarrollo deja evidencia del deadline incumplido y se corrige el
pack o la predicción.

## D. Cadencia, color y criterio de aceptación

- Simulación a 59,185606 Hz con acumulador entero; presentación VI a 59,94 Hz
  (NTSC) con repetición de un frame cada ~1,3 s. Verificado en host y en ares.
- Color: RGBA5551 en TLUT y framebuffer. Comparaciones con MAME a nivel de
  índices y orden; igualdad RGB8 no reclamada.
- Aceptación: medir máximos (no medias) de CPU, RDP, texels, uploads, bytes PI
  y *misses* en escenas de peor caso (jefes, explosiones), en N64 física.
  ares y MiSTer son evidencias adicionales, no garantía temporal.

## E. Estado y trabajo pendiente

| Área | Estado |
| --- | --- |
| Núcleo CI4/TLUT/rdpq, pack, caché PI, fences | Implementado y validado en ares (v8–v10) |
| Misión 1: escenario y colisión hasta cámara 1920 | Implementado; 122 frames validados contra MAME |
| Entrada en paracaídas, título animado y audio | Implementado por la rama `beta-work` |
| Marco: controlador y animaciones nativas | En curso en `beta-work` (prioridad actual) |
| Soldados nativos (IA, colisión, guión de nivel) | En integración (rama `work-soldiers`) |
| Zoom Neo Geo por bandas | Propuesto |
| Jefes, vehículos jugables, granadas, resto de encuentros | Pendiente |
| Streaming en 4 MiB y prueba en N64 física | Pendiente |
| Efectos 2.5D (perspectiva, luz, niebla) | Implementados tras una bandera; el renderizador activo usa capas 2D y sombras de contacto |
