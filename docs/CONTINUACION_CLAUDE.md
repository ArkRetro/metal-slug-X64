# Continuación del trabajo de Claude

Base descargada: `ccr-09eb2f6b-8rwl8l`, commit
`bea67fe072db933097623162d542f5fd1ca9c5ce`. La continuación vive en
`codex/continue-claude`, en `/workspace/metal-slug-X64-claude`.
Se conserva la rama de Claude. La rama anterior `beta-work` no se ha
fusionado sobre su arquitectura ni se han copiado sus assets privados.

El punto de continuación es el paso 2 de [PLAN_MISION3.md](PLAN_MISION3.md).
Se mantienen las políticas de Claude: 8 MiB, paquetes por misión, rutinas
nativas y paradas explícitas para lo pendiente.

## Evento de puntos de la plataforma

`neo_m3reward.c` traduce `0B9748/0B9780` y sus comprobaciones hasta
`0B99CA`. La plataforma `0B9286` ya lo creaba como externo; ahora el mundo
lo ejecuta como un actor nativo invisible.

El evento conserva por jugador si se aproximó desde la izquierda sobre un
suelo, espera el contacto con la plataforma y consume su oportunidad una
sola vez. La recompensa especial requiere además superar el límite del
original. Se conservan las solicitudes ordinarias de valor cero: también
crean etiqueta y piden sonido. `08CECC` suma el valor BCD, pide `1121` y crea
`08D330` después del guion, en coordenadas de pantalla sin padre.

El mundo proporciona una vista de las coordenadas reales `+3C/+40` y del
byte `+15` de cada jugador. Esta vista precede a la sustitución de objetivos
por el punto del guion; un objetivo de IA no permite inferir si el jugador
está sobre un suelo. El mundo jugable actual sigue siendo de un jugador;
las pruebas controladas incluyen ambos jugadores y su orden de evaluación.

## Verificación y límites

Dos referencias independientes de MAME 0.276 ejecutan 140 llamadas con RAM
de entrada inyectada y las interrupciones enmascaradas:

- Estados y solicitudes: los 192 bytes del actor, secuencias de aproximación
  y aterrizaje, 45 recompensas y 31 solicitudes de borrado.
- Efectos originales de `08CECC`: suma BCD `057948`, posición `01ACBE`,
  45 sonidos solicitados y 45 etiquetas. El asignador `00411C` devuelve un
  nodo vacío válido y `0120F8` registra la solicitud; no se acredita el
  agotamiento del pool, el enlace del asignador ni la reproducción acústica.

Host, UBSan y el ejecutable MIPS en ares pasan las mismas comparaciones.
También pasa `make check-core` y compilan los objetos MIPS de `neo_world`
y `neo_m3props`. [Informe y hashes](m3-platform-reward-validation.json).

```sh
make check-core
make check-m3-reward
make m3-reward-regression.z64 N64_INST=/workspace/claude-toolchains/package/opt/libdragon
python3 tools/test_m3_reward_ares.py m3-reward-regression.z64 --output /tmp/m3-reward-ares
```

El SDK aislado de este entorno tiene la base `e356bf3` y los dos parches
de `tools/libdragon-patches`, como exige `tools/setup.sh`. El SDK anterior
permanece intacto. Las herramientas de captura admiten `MSX_MAME` y
`MSX_XDOTOOL`; los estados guardados deben coincidir con la versión de MAME.

No se ha reconstruido ni recorrido el cartucho completo de Claude: sus
mapas, paquetes de misión, catálogo gráfico y capturas privadas no están
en este checkout. Esta prueba MIPS es un test de lógica, no una demo del
juego. El resto de los enemigos del paso 2, el paracaídas de tipo 2 y los
dos jefes de la Misión 3 siguen pendientes.
