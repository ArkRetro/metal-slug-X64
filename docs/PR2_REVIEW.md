# Correcciones revisadas del PR #2

Fuente: [Revisión: posición inicial nombrada, flash del último guardia y pruebas sin SDK](https://github.com/ArkRetro/metal-slug-X64/pull/2),
commit revisado `c826597fdea29108facb28a1bd5ce9d8fdd18771`.
El PR parte de `main`, anterior a los componentes nativos nuevos. Sus cambios se
han adaptado a `beta-work`; el PR no se ha fusionado en `main`.

| Corrección | Aplicación y comprobación |
| --- | --- |
| Tercer guardia desaparecía sin parpadeo | El prototipo mantiene el temporizador de muerte de 90 ticks y permanece muerto al agotarlo. La prueba alcanza la tercera baja mediante el combate existente y comprueba que no reaparece. |
| Cadáver congelado al cerrar la región | Completar la región limpia el temporizador. Es el cierre provisional de la región, no el final real de Misión 1. |
| Posición repetida como literal | `MSX_START_X` nombra el inicio histórico del prototipo y su reaparición. La entrada en paracaídas utiliza su posición medida propia. |
| Mensaje de error «v3» | Eliminado el prefijo obsoleto del ejecutable histórico. |
| Pruebas dependían innecesariamente del SDK | Los objetivos host no incluyen `n64.mk`. `check-core` ejecuta seis pruebas, incluyendo el nuevo códec cinematográfico; `check-cinematic` verifica todos los fotogramas privados del segmento. |
| CI | `.github/workflows/check.yml` ejecuta `make check-core` en pushes y PRs. No usa ROMs ni el SDK. |

`check-core` requiere los CSV numéricos versionados que usa la prueba de salto,
además de código y compilador C. Se verificó en una copia limpia con esos archivos,
sin `build/`, sin SDK y sin los assets privados de `filesystem/`.
`make check` conserva todas las pruebas adicionales de la rama y requiere sus
assets y metadatos derivados; no se ha reducido al conjunto antiguo del PR.

Las pruebas host completas y las compilaciones N64 pasan. La integración en ares
comprueba además la nueva entrada, el traspaso al jugador y sus controles. El
parpadeo sigue siendo una corrección del guardia provisional: no convierte esa
IA ni sus parámetros en comportamiento verificado de Neo Geo.
