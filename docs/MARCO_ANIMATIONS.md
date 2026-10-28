# Marco: recuperación del conjunto completo de animaciones

**Prioridad actual: terminar Marco antes de ampliar el resto del port. El 100%
todavía no está conseguido.** El ejecutable de campaña conserva los seis estados
visuales históricos. La biblioteca nueva se comprueba aparte; todavía no está
conectada al controlador completo del jugador. No se publica otra demo parcial.

La [continuación del controlador y la composición nativa](MARCO_CONTROLLER.md)
añade filtrado de entradas, selección original de programas y dibujo conjunto
cuerpo/arma. Dos recorridos pasan en ares; el catálogo combinado llega a 178
descriptores. El recorrido de combate usa publicaciones reales de cola para
resolver las llamadas que atraviesan VBlank. Esto no declara completo Marco.

## Resultado comprobado

La captura ampliada usa MAME 0.276, BIOS euro, entradas de mando programadas y
lectura de VRAM mediante el elemento guardado del dispositivo. No escribe RAM
de juego ni los registros de dirección de VRAM. Sus 29 condiciones de entrada
cubren reposo, carrera, cambios de dirección, agachado, disparo de pie/agachado,
apuntado vertical, saltos con/sin carrera, disparos en el aire y granadas. Son
condiciones de entrada, no 29 animaciones completas ni un denominador del 100%.
Durante la secuencia se produce además una muerte/reaparición real.

De 2.952 fronteras observadas se contrastan 2.948: las cuatro iniciales no tienen
un comando previo registrado para su buffer de dibujo. Hay 2.916 fotogramas con
Marco visible y 32 sin emisión de dibujo durante el parpadeo. Coinciden
1.972.752 píxeles con la imagen independiente de MAME; otros 7.062 quedan
explicados por sprites posteriores o FIX, comprobando también su color real.
La referencia PNG solo compara; no se convierte en textura.

Se han observado **172 descriptores** (piezas/poses, no 172 animaciones), dos
paletas y el cambio de banco de paleta al reaparecer. Los comandos deben caducar
al vaciarse el buffer: retener el último descriptor produciría una pose congelada
en los fotogramas que deben estar vacíos. Esa caducidad se contrasta con SCB,
no se acepta una máscara para ocultar diferencias.

[Informe con hashes y límites](marco-animation-validation.json).

## Direcciones verificadas y frontera nativa

| Evidencia original | Dirección | Uso |
| --- | --- | --- |
| Actor principal de esta partida | RAM `0x1014C4` | Las trazas previas de entrada y física identifican esta instancia. No se generaliza a un pool inventado. |
| Estado principal de animación | RAM `0x1014E4` (`actor+0x20`) | Cursor/temporizador/velocidad del cuerpo. |
| Segundo estado observado | RAM `0x10154C` (`actor+0x88`) | Cursor/temporizador/velocidad de la otra parte; no comparte el temporizador principal. |
| Despacho de comando | ROM `0x00429C` | Probe antes de leer opcode y operandos. |
| Escritura de byte/word/long | ROM `0x00445E`, `0x00446A`, `0x004476` | 734 casos de escrituras y saltos de campo, con entrada/salida medidas. |
| Condicional de byte | ROM `0x0044AE..0x0044B8` | Compara cero/no cero, no el valor literal completo. |
| Selector de pose | ROM `0x004412..0x004450` | 366 casos con el campo leído **antes** de seleccionarlo; campos `+0x77` y `+0x79`. |
| Emisión de descriptor | ROM `0x004406` | Descriptor, coordenadas, atributos y orden de las piezas. |

Ghidra exporta instrucciones con dirección y C **tentativo**, usando el lenguaje
superset `68000:BE:32:MC68020`. Sus avisos de flujo/decodificación no se presentan
como lógica recuperada. Los fixtures comparan los probes ejecutados por el
68000 y los operandos originales, no los tipos sugeridos por el decompilador.

`neo_anim_step_fields()` amplía el motor C existente con escritura tipada,
condicionales y selección por campo. El servicio de campos asigna identificadores
a miembros C; no proporciona RAM emulada ni ejecuta punteros 68000. Las variantes
sin ese servicio rechazan esos comandos explícitamente. Se conservan las pruebas
del backend anterior. Los nombres `field72/76/77/78` son neutrales: todavía no se
atribuye a todos una semántica completa de colisión o control.

El conversor privado ha recorrido 692 comandos tipados desde 531 posiciones de
comando observadas. Esas posiciones no son 531 animaciones. Hay 16 valores de
inicio principal vistos por el probe de `0x004256`; esto tampoco enumera todas
las entradas secundarias ni todos los programas del juego. La captura posterior
verifica diez sitios de solicitudes a callbacks/servicios; dos sitios estáticos
no ejecutados permanecen como `UNRECOVERED`. Los efectos completos del juego
siguen pendientes: registrar una solicitud no implementa su efecto.

## Cuerpo y arma: llamadas completas recuperadas

`marco_animation.c` coordina dos relojes nativos y sus campos compartidos.
La entrada principal `0x004256` borra el campo +18; la secundaria `0x004260`
lo conserva. El paso compartido `0x00427E` actualiza el contador de parpadeo
positivo por llamada, no una vez por actor. La salida con carry se observa en
`0x004350`; el retorno nulo de `0x004288` no modifica ese contador.

La rutina del arma `0x0C3778..0x0C37CC` toma los offsets firmados +72/+74,
aplica el reflejo y los suma temporalmente a X/Y y al cache de posición. Después
resta **los offsets guardados**, incluso si el programa cambió esos campos.
No resta los offsets nuevos ni restaura coordenadas absolutas que borrarían
un movimiento realizado por un servicio. Se verifica con traza de instrucciones,
registros del wrapper y entradas/salidas del helper; no se deduce de capturas PNG.

La captura privada `marco-paired-controller-06` aporta **2.452 llamadas completas**:
1.459 del cuerpo y 993 del arma, 2.574 dibujos y 152 solicitudes a servicios.
Los estados finales, cursores convertidos, temporizadores, coordenadas, campos
compartidos, descriptores y orden de solicitudes coinciden en host C y en
MIPS ejecutado por ares. Hay 213 cambios de offsets durante una llamada.
2.135 llamadas reutilizan el estado persistente sin reconciliación; las otras
317 reciben cambios externos registrados del controlador. La elección de programa,
el orden de llamadas, las posiciones del mundo y esos cambios siguen siendo
entradas de la prueba. La profundidad Z se suministra a partir del journal de
dibujo: esta prueba verifica su propagación, no su cálculo por el controlador.

Las 151 llamadas observadas a través de `0x0044EC` y la llamada de servicio
`0x004492` no cambian los campos de animación/render monitorizados. Sus diez
sitios se convierten en solicitudes nativas `NEO_ANIM_EVENT`, con identificadores
y argumentos documentados en el grafo privado. Esto **no** demuestra que sean
inocuos para munición, RNG o el pool de actores. Proyectiles, granadas y demás
efectos necesitan sus implementaciones propias. Dos sitios no observados no
reciben esa conversión. Un servicio ausente o un comando desconocido detiene
el actor; un reintento no repite efectos ya enviados.

Este corpus no contiene pasos con contador de flash positivo ni retorno nulo;
sus límites, el reflejo con envoltura, los offsets cambiados dentro del paso y
los fallos de servicio se prueban además con casos **sintéticos**, claramente
separados de la comparación MAME. ASan/UBSan y `make check` pasan.

El cartucho interno `marco-pair-regression.z64` pasa en ares 146 con 2.452 pasos,
2.574 dibujos y 152 solicitudes. Comprueba el backend de CPU: no es una prueba
de framebuffer, fluidez, controlador completo ni una demo de campaña.

## Anclajes y renderizado N64

`build_marco_poses.py` usa los descriptores de cartucho/C-ROM y las paletas
observadas. Mantiene offsets firmados respecto al origen lógico, sin centrar
por el pie visible ni por el rectángulo opaco. El cambio al agacharse contiene
ajustes transitorios que el volcado RAM al final del frame ya no conserva;
por eso los selectores registran también los registros/campos antes del dibujo.

La salida contiene 184 partes `.sprite` CI4 para los 172 descriptores. Cada parte
tiene como máximo 64×64 píxeles, 2.048 bytes de textura; TLUT permanece separada.
Las piezas altas del paracaídas se dividen, conservando su anclaje. Las imágenes
PNG intermedias mantienen los índices originales: se comprueban los 16 colores
y el hash de índices al cargar cada recurso. Así una paleta alternativa no
recolorea índices que `mksprite` hubiera fusionado o reordenado.

`marco_renderer` carga los recursos observados (120.584 bytes de archivos en ROM)
y permite seleccionar las dos paletas comprobadas. Restaura el modo RDP, pero
modifica TMEM/TLUT: al integrarlo con el renderer de escenario habrá que invalidar
su residencia de texturas. Antes de cerrar los recursos debe terminar el RDP.
La búsqueda de un descriptor ausente falla, sin sustituirlo por reposo.

La comprobación de ares calcula **688 CRC de framebuffer real**: todos los
172 descriptores, las dos paletas y las dos orientaciones. Coinciden con los
píxeles originales cuantizados a RGBA5551. Esto demuestra la conversión y el
renderizado de este catálogo; no demuestra el controlador completo ni la tasa
de frames en una N64 física/MiSTer.

Se corrigen además dos fallos del camino anterior:

- El extractor omitía píxeles al cruzar los contadores de 9 bits de Neo Geo.
  Ahora envuelve X/Y antes de recortar, y el resolvedor de oclusiones hace lo mismo.
- El reflejo RDP muestreaba `width..0`; con filtro de punto repetía una columna
  y perdía otra. Debe muestrear `width-1..-1`. Se corrige en el renderer existente
  de Marco/proyectiles y en el del blindado, además del recurso nuevo.

## Trabajo necesario para poder declarar el 100%

- Sustituir la selección histórica de seis bancos/muestras por el controlador
  completo de cuerpo y arma: entradas, prioridades, transiciones, velocidad,
  temporizadores, origen lógico y parpadeo, sin mezclar secuencias provisionales.
- Resolver los callbacks pendientes y sus efectos/solicitudes. No dar por
  completada una acción que se detiene en un callback sin recuperar.
- Completar y contrastar las variantes de armas, ataques de proximidad, toma y
  cambios de objetos/armas, todas las muertes/reacciones/reapariciones, acciones
  de vehículos y transformaciones que existan en la ROM. El inventario debe
  salir de tablas/controladores y observaciones; las 29 condiciones actuales
  no justifican una lista exhaustiva.
- Integrar todas las entradas y combinaciones correspondientes en la campaña,
  y volver a contrastar las transiciones en el ejecutable final de ares.

No se oculta esa diferencia detrás de un porcentaje estimado ni del número de
imágenes renderizadas. Tampoco se entrega el cartucho de regresión como demo.

## Reproducción local

ROM/BIOS, gráficos, RAM completa, trazas completas y proyectos Ghidra permanecen
fuera de Git. `make check-core` usa únicamente los fixtures numéricos versionados;
`make check` añade las pruebas de assets privados. La captura/conversión se hace
con `run_marco_catalog.py`, `audit_marco_catalog.py`, `extract_marco_fields.py`,
`extract_marco_selectors.py`, `convert_marco_programs.py` y `build_marco_poses.py`.
`AnalyzeMarcoAnimations.java` admite un fichero opcional de semillas numéricas
para ampliar el análisis sin tratar direcciones supuestas como ejecutadas.

Los ejecutables `marco-regression.z64` y `marco-render.z64` son internos.
`test_rng_ares.py --suite marco` y `test_marco_render_ares.py` los ejecutan sin
publicarlos. El primero comprueba 734 operaciones y 366 selectores; el segundo,
los 688 framebuffer indicados. La mayor carga de textura registrada por el
uploader es 1.024 bytes, frente al límite de 2.048. Los casos adicionales de límites de contador,
servicio ausente y decremento con envoltura son sintéticos y se distinguen de
los casos originales medidos.

Para reproducir la comparación completa, con los datos privados disponibles:

```sh
python3 tools/extract_marco_steps.py CAPTURA --trace TRAZA --output DIARIO
python3 tools/convert_marco_programs.py CAPTURA --event-evidence DIARIO/effects.json --output GRAFO
python3 tools/compile_marco_steps.py DIARIO/steps.json GRAFO/graph.json --output build/marco_steps.h
cp GRAFO/marco_script.h build/marco_script.h
make marco-pair-regression.z64
python3 tools/test_rng_ares.py marco-pair-regression.z64 --suite pair --fixture DIARIO/steps.json --output RESULTADO_NUEVO
```

`CAPTURA` se obtiene con `run_marco_catalog.py --command-trace --paired-trace`.
La traza se limita al primer wrapper; las capturas históricas sin ella deben
acompañarse de una captura corta del mismo programa fijo y banco.
El estilo visual pendiente es **diorama**: terreno, fachadas y tejados con
volumen sencillo y planos separados en profundidad, manteniendo los personajes
nítidos y la acción 2D. Se retomará después de Marco, junto con soldados,
vehículos y el resto de la misión.
