# RAM:10FC00 → C

Reconstrucción manual de la imagen copiada desde P1:0010EE. El C está en
ram_10fc00.c/.h, un nivel por encima de esta carpeta.

48 llamadas originales de MAME con imports sintéticos coinciden en registros,
SR, flujos completos de accesos, RAM y NV. También pasan en host/UBSan y
MIPS/ares. Los servicios de plataforma nativos y los llamadores siguen sin
integrar; el dominio no está cerrado.

Véanse validation.json, data/boot_select_fixture.json y
docs/NATIVE_BOOT_SELECTOR.md para límites y reproducción. Los logs y el Lua
son del ensayo ejecutado. La ejecución se detiene antes del RTS externo;
el tracer puede imprimir su línea antes de que actúe el breakpoint.
