# Marco: entradas, selección de programas y salida nativa al RDP

El nuevo backend ejecuta el filtrado de entradas, la selección del programa y
los pasos de cuerpo/arma como funciones C. Sus emisiones ya se conectan al RDP
con anclajes originales y coordenadas de 9 bits. **Esto no completa el controlador
del jugador ni sustituye aún el selector histórico de seis estados de campaña.**

## Verificado mediante ejecución

La captura `marco-controller-coupled-10` usa MAME 0.276, BIOS euro y el mismo
plan de 29 condiciones de mando, sin escribir RAM del juego. Todos sus estados
de actor al final de los 2.952 frames coinciden byte a byte con la captura visual
anterior. La traza conjunta registra 6.539 llamadas en orden:

- 3.711 filtrados de entrada (`0x0C0FEA`, después de elegir el buffer en
  `0x0C0FF4`, hasta `0x0C109A`). Coinciden la palabra devuelta y los campos
  +5F, +9A y +9B. El AND final modifica solo el byte bajo del resultado,
  después de actualizar los flags: adelantar esa máscara cambiaría el comportamiento.
- 376 selecciones de programa: cuerpo `0x0C0F32..0x0C0F60` y arma
  `0x0C0F62..0x0C0F94`. Se registran tabla, índices, palabra realmente elegida
  y estado de animación después de inicializar, no solo el puntero de entrada.
  Hay 29 tablas diferentes; las ejecuciones observadas usan +93=0 y +6E=0.
- 2.452 pasos completos: 2.574 dibujos y 152 solicitudes explícitas de efectos.
  2.231 pasos conservan íntegro el estado nativo previo; 221 reciben cambios
  registrados de la lógica exterior. El ensayo anterior sin selección nativa
  necesitaba reconciliar 317 pasos.

Las referencias ejecutadas a `0x0CA2CE` condujeron a su análisis en Ghidra:
`0x0CA2CE..0x0CA2DE` selecciona uno de dos buffers según el byte bajo del selector.
La generación de su contenido y el control exterior siguen siendo límites
pendientes; no se confunde el dword registrado con un muestreo crudo de joypad.

`marco_program_select` trabaja con referencias nativas, nunca con direcciones
68000 ejecutables. Replica la aritmética de 16 bits y el fallback cuando la
entrada original es NULL. Una entrada nativa desconocida se detiene: no activa
ese fallback, porque un recurso sin recuperar no equivale a un NULL original.
Los dos tipos de inicialización conservan su diferencia en el campo compartido
+18. La consulta `0x0C0F96` se traduce sin fallback; su cobertura actual es
estática y sintética, no una serie de llamadas MAME registradas.

## Composición gráfica completa de las emisiones observadas

`marco_renderer_draw_native` acepta directamente `neo_anim_draw_t`. Aplica el
origen vertical Neo Geo, los reflejos y el wrap de 512 unidades antes del recorte.
Cada parte CI4 se sube una vez aunque cruce un borde del contador. El llamador
elige la paleta nativa, el desplazamiento del viewport y el scissor; el renderer
no recupera por sí mismo la asignación de paletas del juego. Modifica TMEM/TLUT,
por lo que sigue siendo necesaria la invalidación de residencia del escenario.

El ensayo conjunto usa la lógica C para producir las piezas de cuerpo y arma.
Las referencias visuales proceden independientemente de SCB/VRAM, C-ROM y paletas
de la captura auditada de MAME, no de las imágenes generadas por N64. Se recorta
el área Neo Geo `[8,16)..[312,240)` y se sitúa en `[8,8)..[312,232)` de la salida
320x240, con margen negro. Se mantiene el retardo observado de tres ticks entre
la emisión original y la lectura del buffer de vídeo.

En ares coinciden 1.458 CRC del framebuffer real del RDP, con 2.572 dibujos
compuestos y las dos paletas observadas. Los últimos tres ticks de captura no
tienen una lectura posterior de VRAM: dos dibujos del ensayo de CPU quedan por
ese motivo fuera de esta comprobación visual. El máximo upload medido sigue en
1.024 bytes, con un límite impuesto de 2.048 para CI4/TLUT. Este ensayo valida
las emisiones; no constituye un juego completo ni mide fluidez en hardware.

Se añaden por separado casos sintéticos de bordes/reflejos usando los píxeles
SCB de una pose observada. No se presentan esos reflejos verticales o posiciones
artificiales como acciones que Marco haya ejecutado en la captura original.

## Recorrido de combate y publicación de los buffers

El plan de avance/disparo/salto/granadas aporta otra captura de 2.240 ticks.
La auditoría verifica 2.234 fronteras de vídeo: seis iniciales carecen de historial
completo de publicaciones. Se contrastan 1.134.432 píxeles visibles y se explican
47.436 oclusiones con otros sprites/FIX. Sus 79 descriptores incluyen seis que
no aparecían en el catálogo anterior. El catálogo combinado validado tiene
**178 descriptores, 190 partes CI4 y dos paletas**, 124.464 bytes de recursos.
En ares pasan sus **712 CRC** de recursos (178 × dos paletas × dos orientaciones).

Esta captura contiene 24 llamadas de animación que empiezan y terminan en ticks
de vídeo distintos. El retardo fijo de tres ticks del corpus anterior no sirve
como regla general bajo carga. `published_batches` usa las escrituras reales de
la cola y su publicación; nunca elige el fotograma que mejor se parezca al PNG.

Direcciones y comportamiento contrastados:

- `0x00A03E` encola la emisión. El vector observado `0x00A036` es un `RTS`;
  otro vector no recuperado detiene la auditoría.
- A5 vale `0x108000`. El flag de cola está en RAM `0x109946` y selecciona las
  bases `0x109948` / `0x10A94E`.
- La traza MAME muestra `NOT.B ($1946,A5)` en ROM **`0x011E30`**, seguida por
  la selección/reinicio de la cola. El watchpoint informa PC `0x011E36` por la
  precarga del 68000: ese valor no se usa como dirección inicial de desensamblado.
- El contador de fase leído en RAM **`0x10000F`** distingue la cola recién
  cerrada del siguiente update que la consume. En las fronteras observadas,
  delta de fase 0 muestra la penúltima cola cerrada y delta 1 la última.
  Se rechaza cualquier otro delta. Una cola vacía sigue vacía, sin conservar
  la última imagen de Marco.

La regla se comprueba contra todas las instantáneas SCB de la captura, con
1.118 cambios de cola observados. No presupone dos ticks físicos por llamada.
Los campos de tiempo `frame/phase` se mantienen en los diarios, pero no se
confunden con campos del actor modificados por un callback.

La segunda reproducción nativa ejecuta 1.632 filtrados, 202 selecciones de
programa y 1.430 pasos de cuerpo/arma: 1.430 dibujos y 147 solicitudes.
Las velocidades registradas incluyen 0x0100, 0x01C1 y otras velocidades variables;
se conservan los temporizadores y los residuos fraccionarios. También pasan
host con ASan/UBSan y MIPS/ares. El renderer recibe las emisiones por **cola lógica
publicada**, para no separar cuerpo y arma si la llamada original atravesó un
VBlank. Coinciden los **1.117 framebuffer de colas observables**, con 1.428 dibujos
compuestos y los 16 casos sintéticos de límites separados. Las últimas colas
sin SCB posterior no se cuentan como referencia visual.

Los valores antes de los límites externos del controlador siguen registrados,
no calculados por una IA de prueba. En este recorrido 1.084 pasos conservan el
estado nativo previo y 346 requieren reconciliación exterior. No se ha recuperado
un controlador completo de combate ni se ha demostrado adquirir otras armas;
no se asigna una semántica nueva a los valores de +6C solo por verlos cambiar.
El recorrido acaba tras consumir las vidas del jugador y tampoco equivale a
completar la misión.

## Inventario estático: pendiente de ejecución

La inspección explícita de las filas 0, 1, 2 y 3 del selector +93, con +6E=0,
en las 29 tablas observadas lee 116 celdas y encuentra 77 punteros distintos.
No se infiere que esas filas agoten cada tabla ni se les asignan nombres de
armas o transformaciones. La ampliación del grafo contiene 1.533 comandos y
466 descriptores distintos, pero solo el subconjunto observado dispone de
contraste dinámico. **466 descriptores no son 466 animaciones ni un denominador
del 100%.** El catálogo gráfico validado conserva sus 172 descriptores.

El grafo ampliado mantiene 20 sitios sin recuperar como puntos de parada,
frente a los dos del grafo observado. Solo diez sitios observados generan
solicitudes nativas; sus efectos sobre munición, RNG y actores todavía deben
implementarse. La nueva exploración de filas no convierte callbacks no vistos
en solicitudes supuestamente inocuas.

Sigue pendiente recuperar las transiciones exteriores, cubrir armas/formas y
acciones especiales, integrar el control completo en campaña y contrastarlo.
La prioridad continúa siendo Marco al completo; después soldados y vehículos.

## Reproducción

`make check-core` usa el fixture numérico versionado y programas stub distintos
para verificar identidades de selección. No necesita SDK ni ROM y no pretende
validar el contenido de las animaciones con esos stubs. Los ensayos privados
usan las referencias reales del grafo convertido, sin stubs.

```sh
python3 tools/mame/run_marco_catalog.py --rom-dir ROMS --output CAPTURA --command-trace --paired-trace --controller-trace --logic-only
python3 tools/extract_marco_controller.py CAPTURA --output data/marco_controller_fixture.json
python3 tools/extract_marco_steps.py CAPTURA --trace CAPTURA --output DIARIO
python3 tools/convert_marco_programs.py CAPTURA --event-evidence DIARIO/effects.json --output GRAFO
python3 tools/compile_marco_steps.py DIARIO/steps.json GRAFO/graph.json --output build/marco_steps.h
cp GRAFO/marco_script.h build/marco_script.h
python3 tools/compile_marco_controller.py --graph GRAFO/graph.json --output build/marco_controller_private_fixture.h
python3 tools/compile_marco_timeline.py CAPTURA DIARIO/steps.json --output build/marco_controller_timeline.h
make marco-coupled-regression.z64
python3 tools/test_rng_ares.py marco-coupled-regression.z64 --suite coupled --fixture DIARIO/steps.json --output RESULTADO_NUEVO
```

Para la parte gráfica, `compile_marco_pixel_sequence.py` requiere además una
captura visual auditada con todos los estados de actor idénticos y los recursos
de `build_marco_poses.py` ya preparados. Después se construye
`marco-coupled-pixels.z64` y se ejecuta el mismo runner con `--pixels`.
Los cartuchos siguen siendo internos; no se publican como demos.

`inventory_marco_tables.py --forms 0 1 2 3` produce un inventario privado y
`roots.txt`, aceptado por `convert_marco_programs.py --additional-roots`. Su
cobertura se registra como estática. El plan `tools/mame/plans/marco-advance.json`
amplía las condiciones de mando para buscar nuevas armas/acciones mediante
avance real; sus resultados deben medirse, no darse por supuestos.

Para reproducir el recorrido de combate, se omite `--logic-only`, se añade
`--plan tools/mame/plans/marco-advance.json --queue-trace` y se audita con
`audit_marco_catalog.py --queue-publication`. El compilador de la línea temporal
y el de referencias de píxeles aceptan `--publication` para formar colas lógicas.
Los resultados privados se generan en `build/marco-advance/`; el cartucho interno
es `marco-advance-pixels.z64`. El runner acepta `--controller-fixture` y
`--pixel-reference` para distinguir este corpus del anterior. Los datos de
animación convertidos y los assets siguen fuera de Git.
