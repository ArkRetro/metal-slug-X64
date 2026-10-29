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

## Controlador superior y cuerpo agachado recuperados

`marco_upper.c` traduce `0x0C37CE..0x0C419C` a estados y funciones C. Incluye
reposo, disparo, granada, cuerpo a cuerpo, cambios de puntería y las ramas de
repetición que el original distingue para los valores de arma 5/10 y 6/11.
Esas ramas tienen evidencia estática; **las capturas actuales no verifican todas
las armas**. El tipo de arma procede del byte global `0x10C71E` para el jugador
0, no del campo +6C del actor. La tabla de compatibilidad está en `0x0BEDC0`.
Las direcciones documentan procedencia: no son punteros ejecutables en N64.

Se han observado y comparado 1.450 llamadas completas (57 reinicios) en la
secuencia de referencia y 642 (14 reinicios) en combate. Los ensayos privados
acoplan estos estados al motor de animación C real: respectivamente 993 y
604 dibujos, con 119 y 147 solicitudes de efectos. Ambos pasan en MIPS/ares.
Las decisiones exteriores del cuerpo, las respuestas del RNG mundial y la
reserva de actores siguen siendo entradas registradas de esos ensayos.

Dos detalles comprobados evitan alterar las transiciones: el controlador de arma
comprueba `D7.w`, mientras el cuerpo comprueba carry; una animación NULL no tiene
el mismo resultado para ambos. El contador +95 avanza únicamente si se reserva
el proyectil (`0x0C4430` / `0x0C44A6`). El permiso y consumo de granadas
(`0x0C478E..0x0C47A8`) respeta la opción global observada; no sustituye la física
ni la inicialización del actor hijo.

`marco_crouch.c` traduce `0x0C2214..0x0C263E`: entrar, esperar, girarse,
desplazarse, disparar, lanzar una granada y las transiciones posteriores.
La prioridad de salto, giro, disparo y granada se conserva por estado; no se
sustituye por una prioridad común. Las solicitudes de levantarse, saltar o caer
son destinos explícitos que el controlador general debe despachar **en la misma
actualización**. El procesamiento común `0x0C32F4` sigue pendiente de integración.

Dos planes con entradas reales, sin escribir RAM, comprueban 227 y 416 bloques
completos. En el segundo, los programas nativos reales producen los 414 dibujos
y 38 solicitudes de efectos registrados; pasa también en MIPS/ares. Los siete
bancos usados enlazan 204 comandos convertidos, sin instrucciones sin recuperar
en ese grafo. Esto acredita ese recorrido, no el inventario completo de Marco.
Los ensayos host con AddressSanitizer y UndefinedBehaviorSanitizer pasan;
LeakSanitizer se desactiva porque el entorno ejecuta bajo ptrace. Estas rutas no
reservan memoria dinámica.

`make check-core` reproduce los cuatro conjuntos públicos de llamadas del
controlador sin SDK ni recursos privados. Esas pruebas públicas reproducen los
servicios de animación y mundo; los ensayos privados anteriores ejecutan además
los programas de animación. Para reproducir el cuerpo con los recursos privados:

```sh
python3 tools/extract_marco_crouch.py CAPTURA --output data/marco_crouch_turn_fixture.json
python3 tools/extract_marco_steps.py CAPTURA --trace TRAZA --output DIARIO
python3 tools/convert_marco_programs.py CAPTURA --event-evidence DIARIO/effects.json --output GRAFO
python3 tools/compile_marco_crouch.py --fixture data/marco_crouch_turn_fixture.json --output build/marco-crouch-turn/marco_crouch_fixture.h
python3 tools/compile_marco_upper_native.py --part crouch CAPTURA GRAFO/graph.json --fixture data/marco_crouch_turn_fixture.json --output build/marco-crouch-turn
make marco-crouch-regression.z64
python3 tools/test_rng_ares.py marco-crouch-regression.z64 --suite crouch --crouch-native --fixture data/marco_crouch_turn_fixture.json --output RESULTADO_NUEVO
```

`CAPTURA` se obtiene con `run_marco_catalog.py --upper-trace --body-trace
--controller-trace --paired-trace --command-trace --logic-only --plan
tools/mame/plans/marco-crouch.json`. Los ROM de regresión permanecen privados.
La campaña activa aún no usa estos controladores; las transiciones generales,
variantes, acciones especiales y su integración no están al 100%.

## Carrera, salto y unión de controladores

`marco_motion.c` traduce las rutinas ROM `0x0C2642..0x0C294A` a C nativo.
La captura `marco-motion-calls-48`, sin escrituras de RAM, aporta 437 llamadas,
890 servicios y 434 dibujos. Los estados y animaciones coinciden en host y
MIPS/ares (`marco-motion-dispatch-51`). La fixture pública conserva valores
numéricos; los programas originales siguen fuera del repositorio.

Se preservan las operaciones de 32 bits con wrap y desplazamiento aritmético,
la velocidad horizontal al saltar corriendo, la velocidad de animación de
carrera y el contador saturado +86. La condición de animación de caída compara
la palabra alta de la velocidad vertical con -1: no equivale a comparar la
velocidad con cero. La validación dinámica cubre las ramas realmente presentes
en esta captura, no todas las variantes de +6C o +93.

`marco_body.c` comparte un único actor entre carrera/salto y agachado. Diferencia
las ramas inmediatas (`tail_call`) de los cambios de rutina para el próximo tick.
En particular, `0x0C2692` puede programar el aterrizaje `0x0C1EBE` y terminar el
tick actual; no debe ejecutar el aterrizaje inmediatamente. La fixture contrasta
esa distinción con el punto real de salida de cada llamada. Los ensayos
sintéticos adicionales comprueban salto desde agachado, agacharse corriendo,
aterrizaje diferido y parada ante servicios ausentes.

**Límite:** los resultados de colisión de `0x014594`, `0x0C109C` y `0x0143EE`
siguen siendo entradas grabadas en el replay. La actualización de reposo, el
ataque de cuerpo y el procesamiento común necesitan todavía su propietario nativo.
Las entradas de incorporación/aterrizaje ya se resuelven como se describe abajo.
El dispatcher devuelve `MARCO_BODY_EXTERNAL` en esas fronteras; no las disfraza
de éxito ni sustituye todavía el controlador de seis estados de campaña.

## Contabilidad de munición y proyectiles

`marco_ammo_reserve` traduce ROM `0x0C0D64..0x0C0DE2`. La captura de solo
lectura `marco-ammo-calls-53` contiene 134 llamadas con arma 0 (pistola),
munición `0xFFFF` y límite `0xFF`. Coinciden en host y MIPS/ares. Esta evidencia
no prueba el comportamiento dinámico de armas especiales.

El desensamblado dirigido en Ghidra confirma el resto de ramas: arma distinta
rechaza sin consumir; cantidad cero rechaza; `0xFFFF` evita el decremento;
RAM `0x10F001` bit 3 o RAM `0x10C2E7` no nulo omiten esa contabilidad.
El límite de proyectiles se comprueba **después** del decremento. El límite
`0xFF` permite el disparo sin incrementar el contador de proyectiles activos.
Los casos finitos/rechazados se prueban sintéticamente, no se etiquetan como
observaciones de MAME.

`marco_ammo_release` procede de ROM `0x0C0DFC..0x0C0E1E`: únicamente reduce
el contador activo si coincide el tipo de arma y no está a cero. No devuelve
munición. Esta función tiene evidencia estática y ensayos sintéticos; falta
capturar sus llamadas originales. El caller nativo recibe un objeto de jugador,
en vez de reproducir el acceso indexado a RAM (el original exige índice <2).

Las rutinas aún no están conectadas al allocator de proyectiles de campaña.
No se ha completado la física, impacto, daño ni sonido de todas las armas.

## Incorporación, aterrizaje y entradas al reposo

`marco_stance.c` recupera las entradas `0x0C1E34` (incorporación), `0x0C1EBE`
(aterrizaje), `0x0C1EFA` y `0x0C1F4A` (dos entradas a reposo), además de la
rama estática `0x0C1E94`. Todas terminan antes de ejecutar `0x0C1FA6`.
`marco_body.c` las conecta con carrera/salto/agachado sin copiar el actor ni
introducir un tick extra. Un aterrizaje programado sigue esperando al siguiente
tick; la incorporación y la entrada al reposo desde carrera son inmediatas.

La captura de solo lectura `marco-stance-calls-57` registra **80 entradas**:
64 en `0x0C1F4A`, cuatro en `0x0C1E34`, tres en `0x0C1EFA` y nueve en
`0x0C1EBE`. Coinciden los campos finales y las 80 selecciones en host y ares.
La extracción verifica cada tabla y puntero elegido contra la ROM suministrada.
El ensayo nativo llama a la inicialización real del reloj, pero usa un programa
marcador: **no avanza ni valida visualmente estas animaciones**.

El camino `0x0C1F24` limpia +92, selecciona programa y reinicia el controlador
superior; `0x0C1F4A` conserva el controlador superior y +92. La prueba preserva
esta diferencia. La comparación de +92 es de byte con signo. El aterrizaje
escribe `0x8834` en +1E/+66 y `0xFF` en +7A/+7B antes de los servicios de efecto.

La rama +93=2 exige un servicio explícito para `0x0040BC`/`0x01ACF2`; su
contenido no se sustituye por un efecto inventado. Tiene evidencia estática y
pruebas sintéticas de orden y ausencia de servicio, no cobertura dinámica en
esta captura. Lo mismo aplica a la rama de +86 >= 32 hacia `0x0C1E94`.

**Pendiente:** ejecutar `0x0C1FA6` con sus consultas al mundo, ataque de cuerpo,
procesamiento común y conexión al controlador de campaña. `MARCO_BODY_EXTERNAL`
en el estado `MST_IDLE` exige resolver esa frontera en el mismo tick; no es un
reposo implementado ni un resultado válido para mostrar otro frame.

Reproducción (capturas y ROM de prueba privadas):

```sh
python3 tools/mame/run_marco_catalog.py --rom-dir ROMS --output CAPTURA --stance-trace --controller-trace --logic-only
python3 tools/extract_marco_stance.py CAPTURA --output data/marco_stance_fixture.json
make check-core
make marco-stance-regression.z64
python3 tools/test_rng_ares.py marco-stance-regression.z64 --suite stance --output RESULTADO_NUEVO
```

## Decisiones del reposo

`marco_stance_idle_branch` recupera ROM `0x0C1FA6..0x0C2022`. Una captura
independiente de 2.952 ticks aporta 800 decisiones: coinciden el destino y todos
los campos de entrada observados en host y MIPS/ares. La prioridad original es
suelo, variante +6C=12, dirección, salto, agachado y ataque cercano. Se conservan
los filtrados repetidos y sus efectos en +5F/+9A/+9B.

El dispatcher conecta las salidas de reposo a carrera/salto/agachado en el mismo
tick. `MSI_DRAW` señala la continuación en `0x0C2144`; `MSI_WORLD`, las consultas
que empiezan en `0x0C2022`. `MSI_WORLD` sigue devolviendo `MARCO_BODY_EXTERNAL` y requiere completar
la misma actualización, sin reintentar el filtrado ni presentar otro frame.
`MSI_DRAW` ya ejecuta los servicios de apoyo y animación en el orden de
`0x0C2144..0x0C215A`, programa `MST_IDLE_FAST` al completar y devuelve
`MARCO_BODY_COMMON`. La rama de ataque cercano también permanece externa.
Esta comprobación no avanza animaciones ni sustituye el controlador de campaña.
Ver `marco-idle-validation.json`. No se publica un cartucho parcial.


La continuación de reposo tiene ensayos sintéticos del despacho completo en
host y MIPS/ares (`marco-body-native-63`): incorporación seguida de apoyo y
animación en el mismo tick, aterrizaje diferido y reinicio de animación programado
para el próximo tick. No se atribuye a estos ensayos cobertura dinámica original
de `C2144` ni implementación nativa del terreno: los helpers siguen siendo
servicios explícitos. `make marco-body-regression.z64` y
`tools/test_marco_body_ares.py ROM --output RESULTADO_NUEVO` reproducen la prueba.
