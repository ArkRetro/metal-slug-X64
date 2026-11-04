# Contextos candidatos del programa

Vistas CPU contrastadas con MAME, mapas de nueve contextos y quince raíces
sembradas. Las tablas de instrucciones/funciones son modelos Ghidra candidatos,
no código nativo recuperado ni una partición completa de las ROM.

El modelo sigue 33 escrituras y dos argumentos del asignador hacia 32 destinos
candidatos RAM. Los inicializadores proporcionan propietarios candidatos a los
dos fragmentos anteriores. `decoder/` contrasta 13.482 límites y tamaños con
MAME; `ram-lifetimes.json` documenta sobrescrituras condicionales de otra imagen
RAM. `runtime/` conserva observación original: dos entradas a 10FC00, ninguna
a 100000 en este recorrido, y una llamada IRQ a 002850 incorporada al modelo.
Los dominios nativos y la decompilación completa siguen abiertos.

Véase [CANONICAL_MODEL.md](../../docs/CANONICAL_MODEL.md) para evidencia,
direcciones, reproducción y límites. `frontier.json` deja explícitos los bytes
sin instrucciones candidatas y las transferencias pendientes de este grafo.
