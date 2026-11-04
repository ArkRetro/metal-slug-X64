# Servicios de descriptores RAM

Fuentes: ram_descriptors.c/.h, en el directorio superior. Entradas 10082E,
1008D8 y 10080E, con callbacks C 10008C/100158 y fronteras pendientes explícitas.

118 transiciones originales coinciden en RAM/registros/SR y escrituras:
105 retornos de kernel y 13 fronteras pendientes. Pasan host/UBSan y MIPS/ares,
incluida la secuencia encadenada sin reiniciar datos entre sus tres pasos.

Los logs y el Lua son del ensayo ejecutado. El tracer puede imprimir la línea
del RTS antes de que el breakpoint detenga su ejecución. No se prueba la
campaña, el dominio completo de callbacks ni temporización del hardware.

Consultar validation.json, data/ram_descriptors_fixture.json y
docs/NATIVE_RAM_DESCRIPTORS.md para alcance y reproducción.
