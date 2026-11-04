# Criterios para declarar completa la recuperación C/MIPS

La integridad de archivos, el número de funciones y las pruebas de unos módulos
no demuestran que todo el programa original esté recuperado. La auditoría
`tools/audit_decompilation.py` comprueba requisitos estructurales independientes
del flag `complete`, y distingue esa integridad del estado global.

Bloqueos del estado actual:

- Índices con C tentativo sin tipos/ABI/semántica verificados.
- Sin partición física exhaustiva de P1/P2/M1 entre código y datos. No se
  debe descompilar como instrucciones una región que sea datos.
- Sitios históricos de flujo sin disposición completa documentada: llamada
  nativa, importación/servicio o dominio de despacho probado. Una recuperación
  parcial del destino no prueba todos los índices posibles.
- Sin evidencia de construcción/validación del programa nativo entero.
- Productores de datos, scheduler y otros bloques pendientes registrados.

Una partición futura `decomp/verified-rom-partition.json` debe contener regiones
por archivo físico (source, offset, size, kind=verified_code/verified_data,
proof, c_sources para código), sin huecos ni solapamientos. Cada prueba y fuente
referenciada debe existir. Las resoluciones futuras de flujo se registran en
`decomp/verified-flow-resolutions.json`, objeto sites con claves de PC del índice
histórico y valores con proof/disposition. Esos documentos deben contener
pruebas reales; crear etiquetas o cambiar flags no verifica semántica.

El escaneo léxico enumera tipos inferidos, registros implícitos, diagnósticos e
intrínsecos. Son señales de revisión y pueden aparecer también en comentarios:
no se convierten en porcentajes ni se consideran prueba de funciones rotas.
Incluso si un futuro estado satisface los requisitos estructurales, requiere
revisión de la evidencia semántica. Esta herramienta no certifica equivalencia
exhaustiva mediante un mero JSON ni genera un ZIP de entrega.

Actualmente devuelve código 1 y `BLOCKED_INCOMPLETE`. El informe reproducible
es `decomp/completeness-audit.json`. Los ensayos MIPS existentes son regresiones
de CPU de módulos, no evidencia de una partida nativa completa.

## Modelo de programa, no solo índice C

`docs/WHOLE_GAME_RECOVERY.md` establece el cierre por subsistemas. El registro
inicial `decomp/native-components.json` reconcilia implementaciones nativas
acotadas con modelos candidatos, sin marcarlos como funciones completas.
El porcentaje de entradas tentativas no mide avance de proyecto.

`decomp/analysis/program-roots.json` documenta tres destinos de vectores sin
modelo y dos caminos estáticos que copian contenedores de código a RAM.
Su identidad incluye CPU/mapeo/PC/origen/vida; no se puede aplanar su ejecución
sobre P1 fijo. El grafo actual es del modelo P1 almacenado y no prueba cierre
de esas raíces ni de bancos P2/Z80. La igualdad de 106 bytes en RAM solo prueba
presencia de bytes, no ejecución ni clasificación interna del contenedor.
