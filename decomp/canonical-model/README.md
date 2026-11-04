# Contextos candidatos del programa

Vistas CPU contrastadas con MAME, mapas de nueve contextos y quince raíces
sembradas. Las tablas de instrucciones/funciones son modelos Ghidra candidatos,
no código nativo recuperado ni una partición completa de las ROM.

El modelo ampliado sigue 33 escrituras de descriptores hacia 30 destinos
candidatos RAM. Dos fuentes no tienen propietario de función. `decoder/`
contrasta 13.463 límites y tamaños con MAME; `ram-lifetimes.json` documenta
dos sobrescrituras condicionales de otra imagen de código RAM. Nada de ello
establece ejecución natural ni cierra los dominios nativos.

Véase [CANONICAL_MODEL.md](../../docs/CANONICAL_MODEL.md) para evidencia,
direcciones, reproducción y límites. `frontier.json` deja explícitos los bytes
sin instrucciones candidatas y las transferencias pendientes de este grafo.
