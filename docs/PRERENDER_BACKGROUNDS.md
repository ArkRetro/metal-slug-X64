# Decorado prerenderizado y colisiones independientes

La campaña permite componer previamente edificios, suelo y objetos estáticos
del tramo observado. El resultado no contiene jugadores, enemigos, vehículos,
HUD ni superficies de colisión inventadas. La lógica continúa en C; no se
sustituye por una película de la partida.

## Implementación

`tools/build_prerender_background.py` compone por capa, en coordenadas del
mundo, los tiles de C-ROM y sus paletas originales. Conserva transparencias,
reflejos, orden de objetos superpuestos y las ocho fases de autoanimación.
Una segunda composición independiente compara todos los píxeles antes de
aceptar los bloques. Los orígenes de cada capa conservan su alineación original.

Las capas principales usan en conjunto **226 colores RGBA5551**, por lo que
se indexan sin pérdida en una paleta compartida de 256 entradas. Cada bloque
CI8 mide 32×32: **1024 bytes de textura**, más 512 bytes de paleta transferida
una vez al cambiar de banco. Se respeta la división de TMEM entre textura y
TLUT. Si un mapa futuro supera 256 colores, se conserva el fallback RGBA16
de 2048 bytes por bloque; no se cuantizan ni descartan colores.

`pack_scene.py` deduplica páginas, agrupa ocho por bloque de streaming de
16 KiB y utiliza el codec y la caché PI existentes. Las páginas CI8 conservan
un slot de caché de 2048 bytes, con relleno compresible. Se solicitan las páginas
visibles y próximas; ninguna textura representa el mapa completo.

La integración es mixta: las capas lejanas 0/1 conservan los atlas CI4 de
referencia y sus rectángulos de 16×16. Las capas 2/3/4, sin reescalado, usan
los bloques prerenderizados. Agrupar también las capas escaladas alteraba
algunos píxeles de borde del muestreo fraccional del RDP. Se conserva su ruta
original para evitar cambiar el aspecto mientras se optimiza el escenario.
El tamaño de dibujo se declara explícitamente en cada celda del mapa.

En el tramo integrado, 1728 celdas de las capas 2/3/4 se convierten en 488
bloques y 446 páginas únicas; la comparación independiente cubre 6422528
píxeles. Las 1042 celdas lejanas conservan su representación CI4. Tras corregir la autoanimación de 160 celdas lejanas (véase
[el informe](MAP_AUTOANIMATION.md)), el paquete total de escena ocupa 498752 bytes y 1294336 bytes decodificados, incluidos
los bancos existentes del jugador y enemigos. Los informes se regeneran en
`build/scene_pack_report.json` y `build/prerender_background_report.json`.

El tamaño de la caché y los recursos integrados exige **Expansion Pak de 8 MiB** en
esta ruta. El comprobador conserva la comprobación de 4 MiB para el backend
CI4 de referencia; el runtime rechaza explícitamente el prerender con 4 MiB.
Menos rectángulos no implica automáticamente menos transferencias de TMEM:
las medidas de comandos se comparan por separado del rendimiento.

## Colisiones y eventos

`neo_collision.c` sigue usando la cuadrícula extraída del arcade. Las imágenes
no determinan dónde se puede caminar y este cambio no modifica alturas,
pendientes, aterrizajes ni hitboxes. El mapa integrado sigue limitado al
tramo observado de cámara 0..1920; no se rellenan las zonas desconocidas.

Una superficie que aparece al destruir un vehículo debe activarse mediante
su evento nativo y cambiar imagen y colisión conjuntamente. Prerenderizar el
decorado no recupera ese evento ni autoriza a inventar su condición de disparo.
Los cambios de paleta, fondo, objetos y destrucciones del resto de la misión
continúan pendientes de su integración original.

## Compilación y comprobación

```sh
make campaign-internal.z64 MSX_BACKGROUND=prerender
make background-regression.z64 MSX_BACKGROUND=tiles
cp background-regression.z64 /tmp/background-tiles.z64
make background-regression.z64 MSX_BACKGROUND=prerender
python3 tools/test_prerender_ares.py --tiles /tmp/background-tiles.z64 \
  --prerender background-regression.z64 --output /tmp/background-comparison
python3 tools/check_scene_budget.py
```

El ensayo lee el framebuffer real del RDP: dos presentaciones, ocho cámaras
y ocho fases, 128 combinaciones. También registra rectángulos, cargas de
textura, cargas de paleta y bytes enviados a TMEM. No mide rendimiento de una
N64 física ni verifica la campaña completa.

En el tramo actual la autoanimación del decorado está deshabilitada por el
original: las fases producen la misma imagen. Los casos sintéticos de host
comprueban por separado el cambio de fase cuando la animación está habilitada.

Las siete pruebas de host cubren alineación, coordenadas negativas, cruces
de bloques, reflejos, transparencias, animación y rechazo de recursos inválidos.
El ensayo adicional alterna fondo CI8, personaje CI4, fondo CI8 y personajes
normal/reflejado/con flash dentro de un mismo frame. Compara también ese
framebuffer con la referencia para detectar paletas obsoletas al cambiar
entre TLUT compartida y paletas individuales. Los recursos propietarios se
generan localmente y no se versionan.

## Medidas del caso integrado

Los 128 casos dan el mismo framebuffer, también al alternar CI8/CI4:

| Backend | Rectángulos | Cargas de textura | Bytes de textura |
| --- | ---: | ---: | ---: |
| Tiles CI4 de referencia | 57176 | 4120 | 8437760 |
| Prerender RGBA16 inicial | 37976 | 8968 | 18366464 |
| Prerender CI8 compartido | 37976 | 8968 | 10919936 |

CI8 reduce un 40,5% los bytes de textura del primer prerender. Respecto a los
tiles originales conserva un 33,6% menos de rectángulos, con más cargas y un
29,4% más de bytes de textura. Se cuentan aparte las cargas de paleta: esta
columna de bytes no las incluye. No se deducen FPS a partir de estos contadores.
El informe completo está en `campaign-validation/background.json`; el caso
RGBA anterior se conserva como referencia, no como entrega adicional.
