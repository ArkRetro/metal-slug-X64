# Rampa del vehículo enemigo: regla recuperada

La indicación del usuario permitió orientar la investigación hacia el estado
del combate. La rampa observada se escribe durante la destrucción del vehículo,
con su vida a cero. Su posición se calcula desde el vehículo, no desde un frame
de la captura ni desde una coordenada fija del nivel.

## Evidencia del original

Se ejecutaron tres partidas de referencia en MAME 0.276 (`mslugx`, BIOS `euro`):

- Combate continuo: activación registrada en el contador de traza 3097. El
  vehículo queda en X=1174, Y=64. La primera traza vigilaba solo las diez
  direcciones que habían cambiado en la auditoría del escenario.
- Ataques interrumpidos: el vehículo permanece vivo, con 300 puntos de vida,
  y no se registra activación hasta el final de esa prueba (frame 4800).
- Ataques interrumpidos y posterior disparo hacia el vehículo superviviente:
  activación en 3790, con vida cero y posición X=937,5, Y=64. Esta traza vigila
  toda la cuadrícula y registra trece escrituras modificadas.

La última partida reproduce **las trece escrituras en su orden exacto** y
coincide con los **4096 bytes de la cuadrícula** de colisión después del evento.
El informe verificable está en [vehicle-ramp-verification.json](vehicle-ramp-verification.json).
Los contadores de frame solo identifican observaciones; no se usarán como
condiciones del juego N64.

La escritura corresponde al PC observado `0x0c8982` (el estado del depurador
apunta a la instrucción posterior a la escritura), con plantilla `0x029bf4` y
objeto en rutina `0x0294e4`. La vida está en el desplazamiento `+0x4e` y la
posición Q16.16 en `+0x28/+0x2c`. La dirección del objeto cambia entre partidas;
no debe fijarse a una dirección RAM concreta en el port.

## Geometría y preservación del terreno

[data/vehicle_wreckage.json](../data/vehicle_wreckage.json) conserva la huella
recuperada: un bloque de 13x3 celdas de 8x8, con pendientes a ambos lados y una
plataforma superior. Las filas se expresan de mayor a menor altura del mundo.
Son datos de colisión; no son una nueva textura de rampa que deba dibujarse.
La colisión permite caminar sobre los restos gráficos del vehículo.

`tools/vehicle_wreckage.py` reproduce la aplicación a la cuadrícula circular:
anclaje respecto al vehículo, límites de la ventana RAM, protección de celdas
que ya tienen una superficie, conservación de metadatos superiores y la
variante de material seleccionada por los atributos del objeto. Una segunda
aplicación no duplica el efecto.

Las diez celdas de la auditoría anterior eran diferencias observadas en
ventanas del escenario, no la plantilla completa del vehículo. No deben
convertirse en una rampa fija que aparece al matar tres enemigos provisionales.
La interacción con la carga de columnas está medida: los restos viven en el
anillo frontal, que se copia al objeto medio en la cámara 1023 y después se
sobrescribe al cargar columnas nuevas. No hay restos fantasma 512 px después
(véase [COLLISION.md](COLLISION.md)).

## Repetir y verificar

Usar el MAME, ROMs propias y directorios separados descritos en
[MARCO.md](MARCO.md), con `tools/trace_vehicle_ramp.lua` como script de arranque.
`MSX_CAPTURE_DIR` selecciona la salida; `MSX_RAMP_DELAY_COMBAT=1` interrumpe los
ataques y después vuelve a disparar hacia el vehículo que quedó detrás.
Añadir `MSX_RAMP_RETURN_FIRE=0` conserva el control que mira hacia delante,
utilizado en la prueba donde el vehículo sobrevivió.

```sh
python3 tools/verify_vehicle_ramp.py /ruta/capturas-rampa \
  --output build/vehicle-ramp-verification.json
make check
```

El Lua utiliza ayudas de supervivencia y munición solo para la referencia.
No escribe la vida de los enemigos, las celdas de colisión ni el estado de
eventos del nivel. Las capturas locales están en
`/workspace/research/ramp-baseline`, `ramp-delayed` y `ramp-return`.

Las regresiones cubren vehículo vivo, atributos que no permiten esta huella,
posición variable, aplicación repetida, preservación del terreno y rechazo de
alias fuera de la ventana circular. La verificación con MAME cubre la variante
observada; no representa todos los vehículos del juego.

## Integración pendiente

Ya existe un componente nativo de movimiento, daño, animación y colisiones,
conectado a un escenario interno de referencia: véase
[NATIVE_BACKEND.md](NATIVE_BACKEND.md). Todavía falta integrar su aparición,
ataque y grupo original de soldados en la campaña. La traza demuestra el evento de destrucción que añade la geometría;
no establece que sea obligatorio eliminar todos los soldados cercanos como
condición independiente. Esa parte se debe recuperar del evento de oleada.

La configuración de referencia aplica la huella al destruir el vehículo y
permite caminar sobre sus restos. Con la cuadrícula ampliada a 2240 px, la
huella en X=1174 escribe las 13 celdas, como el original; antes se recortaban
las tres de X=1192..1208. El escenario predeterminado no inventa un
evento de oleada ni amplía todavía el tramo jugable. Las ROM de comprobación
en ares son internas; no se publica otra demo.
