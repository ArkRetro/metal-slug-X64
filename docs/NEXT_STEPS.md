# Siguientes pasos

1. **Cerrar B04–B07** (rama `work-m1m2flow`, árbol `/home/user/ws-world`):
   - R al llegar al jefe de la M1.
   - Transición M1 → M2.
   - Música al empezar la M2.
   - Desplazamiento del suelo al inicio de la M2.

   Prueba: con la ROM normal desde el arranque, ares jugando la M1 hasta el
   final y comparando la secuencia con una captura de MAME.
2. **Cerrar B08–B10** (rama `work-m2events`, árbol `/home/user/ws-m3`):
   - Inventario de actores de la M2 en MAME frente al port.
   - Tabla de eventos.
   - Dibujo de la forma momia.
3. **Fusionar las dos ramas en `ccr-09eb2f6b-8rwl8l`** y construir la ROM
   21 en `/home/user/ws-logo`. Copiar las entradas privadas con `cp` sin
   `-a` ni `-p`: si se conservan las fechas antiguas, `make` no recompila
   (fue la causa de B01).
   Antes de entregar:
   - Pasar todas las puertas (`PORT_STATUS.md`).
   - Comprobar `native animation loaded (562 poses)` en el registro.
   - Probar la ROM normal, no solo la de referencia.
4. **Hecho (rama `work-diag`):** modo de diagnóstico `MSX_DIAG`
   (`PORT_STATUS.md` «Instrumentación») y RAM de la caché recuperada: 218
   ranuras en la ROM normal y 217 en las de referencia (215/216 con
   `work-laser` fusionada; `RENDIMIENTO.md`). Al fusionar `work-laser`,
   resolver el conflicto de `neo_world.c` añadiendo
   `MSX_DIAG_POOL_FAIL(MSX_POOL_SHOT)` a sus peticiones de disparo. Lo
   siguiente que señalan los datos: el pool de actores genéricos (pico
   91/96 antes del jefe), las balas de pistola (8/8, rechazos) y los
   fotogramas de render con descodificación síncrona (40–45 ms en 1920 y
   3030).
5. **B11 y B12 corregidos en el PC (familias y soldados contra MAME)** (rama `work-laser`, ARMAS_VEHICULOS.md
   Paso 16): rayo `06/0B` nativo y muertes de soldado por llama y estallido.
   Falta: probar el láser en la M2 en MiSTer (la L de la sección 1), copiar
   a `/home/user/ws-logo` las entradas privadas regeneradas
   (`build/marco_campaign.h` con los papeles `W1L_*`/`B1L_*`,
   `filesystem/rest-native.json` con los descriptores del rayo; el audio
   `sfx_10f8` ya está en el directorio compartido) y fusionar en
   `ccr-09eb2f6b-8rwl8l`. Paradas que quedan de B12: el cuerpo en llamas
   `040920` (sin captura) y las entradas `040B46/040BA8/040BE6`.
6. **Hecho (rama `work-perf21`):** B21 (salto del título en la ROM 21:
   tablas de cinemática por trozos; `build/cold-objs.flags` recompila los
   objetos fríos), B22 (montón: HUD CI8, reserva de `actor_init` 672 KB,
   bloques del fondo con ≥ 288 KB libres; mínimo 344 KB en la M1 y 283 KB en
   la M2, 213 ranuras) y B23 (balas de pistola 16, como el arcade sin tope).
   Al reconstruir la ROM en `/home/user/ws-logo`, comprobar que se rehacen
   los diez objetos fríos (`neo_hud.o` de texto 7675 B, no 10982).
7. **Hecho (rama `work-m2fix2`):** B27 (monedas `56` de la etapa 3 y el
   tipo `28` de las momias; inventario de premios en MISION2_ACTORES.md),
   B28 = B13 (humo `0C4DD8` y marcador de la transformación) y B29 (la
   exportación privada de la Misión 2 no tenía el rayo de la L). Al
   reconstruir la ROM en `/home/user/ws-logo` copiar (`cp`, sin `-p`):
   `filesystem/mission2/rest-native.json`, `romfs/mission2.pack`,
   `romfs/scene.pack` (nuevas raíces de programas) y el audio compartido ya
   tiene `sfx_1085.wav64`. Quedan de B27, con captura dirigida: el andador
   `40` de las momias y sus `3D/3F` (`032FB4`), la caja `3F` de la etapa 2,
   la bomba de tipo 2 (`09`, `0C0A7C`: lanzamiento y HUD) y `032DE2`.
8. **Rama `work-m2fix1` (B24–B26 del informe de la ROM 22):**
   - B24: el fin del jefe de la M2 lleva al recuento, a MISSION 2 COMPLETE
     y a la Misión 3 (antes se quedaba parado en cualquier muerte del jefe).
   - B25: la escopeta y las demás armas de `0C42AA` disparan con el giro del
     jugador (+15).
   - B26: orden de dibujo por ranuras SCB en la M2 y Marco en su +1E.

   Falta:
   - Confirmarlo en MiSTer.
   - Aplicar a la Misión 3 su fila de la tabla de ventanas (`MISION2.md`,
     «Orden de dibujo»): con 10E8C6 la capa 2 pasa delante de la ventana
     B. Hay que comprobarlo en ares antes.
   - Traducir el repartidor `01A97A`.
   - Las balas de pistola siguen dibujándose al final.
9. **Rama `work-m2fix4` (B31, fotos de la ROM 23):** `0995EC` (tesoro),
   `032DE2` (soldado envuelto), `03A05A` (respiradero de gas) y `082BCE`
   (soldado de la cuerda) nativos (MISION2_ACTORES.md, «Etapa 3: actores
   de las fotos»). Al reconstruir la ROM en `/home/user/ws-logo` copiar
   (`cp`, sin `-p`) `filesystem/mission2/rest-native.json` y
   `romfs/mission2.pack`. Quedan, por orden:
   - `03A2AA` (roca de la etapa 3): `g_map_patch` con desplazamientos de
     origen (D0/D1 de `0C70FC`) y el dibujo de los parches de tiles; luego
     la traza (ver la orden de `rope2`).
   - El nido `035D12` y el murciélago de puntos `0364D2`: forzar la rama
     vacía de las cajas `08C154` (`2D9206`/`2D9272`).
   - B27: el andador `40` y `3D/3F` (`032FB4`), la caja `3F`, la bomba de
     tipo 2 (`09`, `0C0A7C`) y la muerte quemada `083018`.
10. **Siguientes fallos de la matriz por orden:** B14, B03, B15, B20
   (bucles de motor 10A5/1026), pool de actores genéricos (pico 93/96 en la
   M1).
10. **Rama `work-m2fix3` (B30, B32–B34 del informe de la ROM 23):**
    - B33: la reaparición sigue la tabla de la etapa (`019604`/`019658`/
      `0196E8`) y el tipo 4 busca el suelo como `014742` (JEFE_MISION2.md,
      «Reaparición en la torre del jefe»).
    - B30: la voz de muerte `1123` (AUDIO_JUEGO.md §12).
    - B34: `msx_ramfs` cargaba 63 efectos (faltaban `1158`, `115A`, `1142`,
      `1168`, `116A`, `116E`); registro `MSX AUDIO voice` de las voces del
      jugador.
    - B32: no reproducido (BUG_MATRIX). Falta saber el sitio exacto: pedir
      al usuario una foto antes de ir a la derecha y la pantalla donde no
      puede volver, o una captura de MAME en ese punto.

    Al reconstruir la ROM en `/home/user/ws-logo` (audio compartido de
    `/home/user/metal-slug-X64/filesystem/audio`, ya con `sfx_1123.wav64`):
    comprobar `MSX AUDIO game sounds=71/71` y `MSX CACHE slots` ≥ 208 en las
    dos ROM de referencia.
11. **Rama `work-m2fix5`:**
    - B14 etapa 2 (x 2281): la parada en pared del barrido de suelo toma
      x/y (MISION2.md, «Caída en x 2281 resuelta»). Etapa 3 (x 348/490/611):
      ver BUG_MATRIX B14.
    - B35: voz MISSION 2 START `1132` pedida por las letras, de `rom:/`.
    - B20: `10A5` y `1026` no son bucles (11,6 s y 6,2 s); `10A5` de
      `rom:/`; `1026` sin política (sus pares unen dos canales del port).
    Al reconstruir en `/home/user/ws-logo` (audio compartido ya con
    `sfx_1132`, `sfx_10a5`, `sfx_1026`): `MSX AUDIO game sounds=73/73`,
    `MSX CACHE slots=209` en la referencia de la M2.
12. **Pendiente de `work-m2fix5` (B14 etapa 3 y B17):** parche local
    `scratchpad/f5/b17-pilot.patch` (sin fusionar):
    - B17: `neo_jump_vx` (`neo_motion.h`): salto sin dirección → +30 = 0
      (`0C2654`); `test_m2_stairs` comprueba los 17 pasos del plan eE.
    - Piloto: despegar con la dirección del salto salvo repisa encima a
      ≤ 24 px (`ap_takeoff`), registro `MSX pilot jump`.
    Con B17 solo, el piloto de la etapa 3 saltaba en vertical en x 284 sin
    avanzar (recorrido sin terminar); con el parche del piloto el recorrido
    de 400 s se quedó en la puerta de la etapa 2 (x 2655; con 400 s la
    etapa 2 a veces no termina): falta un recorrido de la etapa 3 con
    `--stage-seconds 800`. La caída x 616 es el salto «bloqueado» del piloto
    sobre el soldado de la cuerda `082DB0`: mejorar esa regla (disparar o
    esperar en vez de saltar al hueco x 632..663).
13. **Rama `work-m2fix6` (informe de la ROM 24):**
    - Hecho: B36 (voz `1130` de la L, `6d00f4a`) y B37 = B15 (parches de
      mapa `0C70FC` dibujados: puertas de las etapas 1 y 2). Al reconstruir
      la ROM en `/home/user/ws-logo` copiar (`cp`, sin `-p`)
      `romfs/mission2.pack` y `filesystem/mission2/s1/patches.json`,
      `filesystem/mission2/s2/patches.json` (`tools/extract_map_patches.py 2
      filesystem/mission2/sK`, luego `pack_mission.py`).
    - B35 (molotov): **traducido** (rama `work-molotov`, ARMAS_VEHICULOS.md
      «Molotov»): recogida `08B5FA`/`0C0A7C`, lanzamiento `01E648`/`01E664`,
      llamas `01E9B6`, voz `1156` (de `rom:/`). Familia `molotov` igual a
      MAME. Falta oírlo/verlo en MiSTer; tipo 3 (`01E680`, `+88` = 1, llamas
      `01EA7A`) y la recogida de los tipos 3..5 siguen como paradas.
    - B38 (**Slugnoid** `09692C`): **traducido** (rama `work-slugnoid`,
      `neo_slugnoid.c`, ARMAS_VEHICULOS.md paso 17): familias `slugidle`,
      `slugfall`, `slugtour`, `sluglaunch` iguales a MAME; en ares nace y se
      dibuja (M2 PASS). Falta montarlo en ares/MiSTer (la repisa de x 392,
      y 1624 de la torre), capturar
      el daño (`097308/09737E`, `0977DC`), traducir `0353C2`/`03533A`
      (parada `0973E2`) y renderizar los sonidos `116C` y `1089`.
14. **Rama `work-m2fix7` (informe de la ROM 25):**
    - B40: el arma del láser (y de toda arma de forma 1) desaparecía al
      cambiar de etapa: `+93..+95` del jugador se reiniciaban con la etapa
      (`msx_game_begin_stage`). Ahora sobreviven, como en MAME (`run4`
      26130/26160).
    - Roca `03A2AA` nativa (`neo_m2props.c`): roca `03A344`, polvo,
      escombros, losa `03A528`, piedras `03A58A` y los parches `0B7A2C`
      (`0C70FC` con D0/D1: colisión en `neo_collision_patch` con paso de
      columna, clave del parche = submapa | D0 << 24 | D1 << 28 en
      `g_map_patch` y `pack_mission.py`). Familia `boulder` igual a MAME; las
      piedras `03A58A` no salieron en la captura (código compartido con la
      caída de la roca).
    - ares (ref. M2; Start en las etapas 1 y 2 de la referencia termina la
      etapa para llegar antes a la 3): `03A2AA native 1` en la cámara 956,
      la roca cae, la losa y el suelo roto se dibujan; registro
      `MSX mission 2 stage N start ... weapon= form=` para ver B40 en
      MiSTer. Sin paradas nuevas.
    - Al reconstruir en `/home/user/ws-logo` copiar (`cp`, sin `-p`)
      `romfs/mission2.pack`, `filesystem/mission2/rest-native.json` y
      `filesystem/mission2/s3/patches.json` (`extract_map_patches.py 2
      filesystem/mission2/s3 --capture-frame <vram38130.bin>` de la captura
      `boulder`, MISION2.md «Parches de mapa»).
15. **Rama `work-gauge` (B41):**
    - Indicador del vehículo: `hud_step` lee el hueco del jugador 1 con
      `neo_vslot_gauge` (`0AAAE0`). SV-001 y Slugnoid lo muestran; la Camel
      no (arcade). Comprobado con la captura `hudg` (fixture del HUD).
    - Bidón de gasolina (objeto 07, `08B710`) traducido con su prueba
      `test_neo_fuel`; no aparece en la M1 ni en la M2 (fuentes revisadas en
      la ROM), así que solo se verá en misiones siguientes.
    - Caja de recogida del jugador montado (`08D268`, blob `E541E`).
    - Pendiente: ver el indicador bajar con daño real en ares/MiSTer.
    - B42, modo opcional «Barra de vida» (PORT_STATUS.md): Z al confirmar a
      Marco en SOLDIER SELECT. Pendiente: probarlo en ares/MiSTer.
16. **Rama `work-m1fix` (informe de la ROM 28):**
    - B43: el motor `10A5` del camión sonaba con ráfagas: su VADPCM no
      descodifica (fallo del codificador de audioconv64 con esa señal).
      `pack_game_audio.py` descodifica cada efecto (`tools/wav64_vadpcm.py`)
      y guarda en PCM16 el que falla; `make check` lo comprueba.
    - B44: SOLDIER SELECT sigue con la música `0030` (render aislado, bucle
      de 50 s) hasta la elección (`tools/pack_select_music.py`).
    - B45: el arcade no crea actores en llamas al destruir el camión
      (55 capturas). Nada que traducir.
    - Pendiente: oír los dos arreglos en MiSTer.
17. **Rama `work-m1boss` (B46, informe de la ROM 28):**
    - El mundo sigue durante el fin de misión (antes se congelaba en
      `018F40`): hundimiento/escombros del jefe de la M1, el espadachín que
      se rinde con bandera blanca (`0A9010`, familia `robeflag`) y la
      victoria de Marco (`0BF658`, pistola y forma 1; catálogo 570 poses).
      Vale también para el fin de la M2.
    - Pendiente: verlo en MiSTer; paradas explícitas: muerte del espadachín
      rendido `0A93DC` (en una captura le alcanzó el último obús del jefe) y
      sus ramas `0A9B60`/`+80 > 1`.
    - Capturas privadas de Marco repetidas (se habían borrado): guardar las
      de `captures-weap2`/`captures-grenade` fuera de los directorios que se
      limpian, o el catálogo no puede regenerarse.

- **Misión 3, mapa reconocido (rama `work-m3map`, MISION3.md «Reconocimiento
  del mapa»)**: rejilla sin el desplazamiento de 64 px, anillo del tren
  corregido, mapas completos de ROM, colisión dinámica (tren, 14 mapas
  dinámicos con su recorrido, 28 parches de repisa), lista de cámara y
  secciones guardados en `filesystem/mission3/s1` y en el bloque `MSXR` de
  `romfs/mission3.pack`; `test_m3_map` (make check) y `make check-m3sweeps`
  al 100 % en las 5 secciones. Siguiente: el escritor del cielo tras el
  progreso 0x140, el actor `0A6C8C` de la superposición `10E8C6` y el mapa
  dinámico del jefe `0E0D00`.

- **Misión 3, paso 1 hecho (rama `work-m3step1`, MISION3.md «Paso 1»,
  B18)**: `work-m3c` fusionada, vagones y plataforma nativos (familias
  `m3wagon`/`m3platform` iguales a MAME), bloque `MSXR` leído, Marco recorre
  la M3 hasta la arena del jefe en ares (0 rescates). Siguiente (paso 2 de
  [PLAN_MISION3.md](PLAN_MISION3.md)): enemigos del camino (`09BA2A`,
  `08A732` —lee `10C700`, ya escrito por `038D26`—, `08DA12`, `05D916`,
  `04E990`, `097FCC`, `0569xx`); pendientes del paso 1: la entrada en
  paracaídas de tipo 2 (`0C15B4`, hoy el jugador aparece en x 0x48), `0B9748`
  (necesita `+15` del jugador en el entorno de los actores), el paso
  plataforma → suelo de x 4272 (comprobar en MAME) y el op 0x15 del guion
  (`2DB148`).

- **Misión 3, paso 2 en curso (rama `work-m3step2`, MISION3.md «Paso 2»)**:
  `neo_m3enemy.c` con la muerte `0406B6` sobre nodos genéricos, el
  granadero `08A732` y los soldados del escudo `09AE2C/09AE1E/09BA2A`
  (familias `m3grenade`, `m3grenade4`, `m3shield`, `m3shield4` iguales a
  MAME), el soldado de ventana `08DA12` (`m3window`), la conversión nodo
  genérico → soldado nativo, las ondas (10C2F6 = 4) y las condiciones
  0x14/0x15 del guion (`2DB148`), los soldados `05D916/05D226/05C53C`
  (`m3para`), la máquina voladora `04E990` (`m3pod`), el helicóptero
  `097FCC` (`m3gun`, `m3gun4`), los lanzadores de molotov `0569xx`
  (`m3thrower`), el bombardero `084588` (`m3bomber`), los bidones `0391EA`
  (`m3drum`), el camión `02B202` (`m3truck`) y la plataforma `08AFB6`
  (`m3raft`), el coche con pasajero `027F54` (`m3car`, con `--soldier`
  para la conversión en soldado), el jeep `0A2F2E` (`m3jeep`), los pájaros
  `03EE8A` (`m3bird`), los sacos `0214EA` (`m3sack`) y el campamento
  `01F1D4` (`m3camp`); paradas del mundo resueltas salvo `040DB4` (el
  resto `03C306` de la muerte de `04115A`: capturar y traducir). Siguiente:
  la entrada en paracaídas (`0C15B4`), `03C306` con traza (traducido sin
  ella), los sprites de vuelo de los pájaros (`23FECC`), premios y ramas al
  azar. `0B9748` ya es nativo (`m3bonus`). El bloqueo de la cámara en 4380 está
  resuelto: lo soltaba el tiempo de espera de la ROM de referencia; ahora el
  primer tiempo de espera pide la muerte general `1067E3` y el guion sigue a
  la par de la cámara (MISION3.md «Bloqueo de la cámara en 4380»; ares r9
  llega a 3716 esperando al jefe intermedio); `1032` suena desde rom:/ (RAM de
  la caché).

- **Misión 3, inventario de eventos y actores (rama `work-m3inv`,
  [MISION3_ACTORES.md](MISION3_ACTORES.md), `data/mission3_actors.json`)**:
  captura `tools/capture_m3_actors.sh` (creaciones `0040BC`, guion, programas,
  sonidos, objetos, lista de cámara) y `tools/compare_m3_events.py`: 233
  constructores (10 871 creaciones), 76 nativos, 4 en `work-m3c`, 152 faltan.
  Orden: (1) suelos de actores: fusionar `work-m3c`, estampadores de
  `038D1E/038D26` y la **plataforma `0B9286`** (mapa `0E6896`: sin ella no se
  cruza el hueco de la sección 3); (2) `0B8EDC` y las llamadas del tren;
  (3) enemigos del camino (`09BA2A`, `08A732`, `08DA12`, `05D916`,
  `04E990`, `097FCC`, `0569xx`...); (4) aeronave `0A4A94` (`0A6C8C` es su
  subrutina de la superposición); (5) vehículos `093C9A`/`0AC866`; (6) jefe
  `04A2C2`, del que depende el fin de misión (`018F92`); (7) la forma
  `+93 = 2` del jugador y el arma E.

- **M2 (rama `work-m3step3`, BUG_MATRIX B47–B49)**: B48 corregido (los
  actores con parada explícita se borran con `ga_kill`: sin cuerpos sólidos
  fantasma); B49 corregido (paletas `0x4BA`/`0x456` en el export de la M2);
  B47 medido (`MSX multi-step`: el fondo de dos capas de la M2 supera los
  16,7 ms al desplazarse; abaratar `draw_layers` queda pendiente). El
  recorrido de la M2 con los datos en `MSXD` da PASS entero (420 s: ares va a
  ≈30 VPS, los 150 s por defecto de `test_mission2_ares.py` ya no llegan a
  la etapa 3; no es una regresión del paquete).

- **Misión 3, paso 3 (rama `work-m3step3`)**: la aeronave `0A4A94` nativa
  (`neo_m3mid.c`, 15 constructores y la capa `10E8C6`), familias `m3mid1..4`
  iguales a MAME (JEFE_MISION3.md «Jefe intermedio: estado del port»).
  Los programas y bloques de ROM de la M2 y la M3 van en el bloque `MSXD`
  de sus paquetes (MEMORY_OPTIMIZATION.md 5b): 210 ranuras con el jefe
  dentro. Siguiente: el paso 5 (vehículos `093C9A`/`0AC866`, forma
  `+93 = 2`, arma E).

- **Misión 3, jefe `04A2C2` y aeronave `0A4A94` investigados (rama
  `work-m3boss`, [JEFE_MISION3.md](JEFE_MISION3.md))**: 9 piezas coordinadas
  por `+9D/+9E`, 5500 de vida solo en el cuerpo (fase 2 con ≤ 2500), órdenes
  0/1 lanzallamas, 2 misiles, 5 bombas, 7 trampilla; muerte `018F92` →
  `018F40` 590 fotogramas después. Estados pequeños y tramos con
  `tools/capture_m3_boss.sh` (`states`, `boss`, `mid`). Siguiente: módulo
  nativo por piezas (esqueleto, brazo, mortero, trampilla, lanzadera,
  muerte) con los fixtures `m3boss1..5`, luego la aeronave (`m3mid1..4`).
