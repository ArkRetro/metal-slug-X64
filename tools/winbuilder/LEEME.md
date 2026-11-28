# Generador de metalslugx64_rev1.0.z64 para Windows

Un único `.exe` sin dependencias (Go, solo biblioteca estándar). El usuario pone
sus ficheros de MAME 0.264 y obtiene la ROM 53 bit a bit
(sha256 `d3613c5b8fa18ff390c346347d6a75ccdc8a5df2527f787f5e3fbb8fa766e82e`).

Por qué es un paquete cifrado y no una regeneración: parte de los datos de la
ROM (audio grabado del driver de sonido, paletas y listas de capturas de MAME)
no se puede rehacer solo con los zips; ver `BLOQUEADOS` en la integración.
Decisión del usuario, 27-XI.

- `payload.bin` = AES-256-GCM(deflate(ROM)), en trozos de 4 MiB.
- Clave = SHA-256 de una etiqueta y del contenido de los 17 ficheros
  originales (`roms.go`: 13 de mslugx, 000-lo.lo, sfix.sfix, sm1.sm1 y
  sp-s2.sp1 de neogeo). Los SHA-1 publicados identifican los ficheros, pero no
  dan la clave.
- Los ficheros se reconocen por contenido (CRC-32 y SHA-1), no por nombre.
- La ROM resultante se comprueba con su SHA-256 antes de escribirla.

Compilar: `tools/winbuilder/build.sh ROM.z64 mslugx.zip neogeo.zip OUT.exe`.
`payload.bin` y el `.exe` llevan datos derivados de la ROM: nunca van a git.

Uso en Windows: copiar el `.exe` junto a `mslugx.zip` y `neogeo.zip` y
abrirlo, arrastrar los dos zips sobre el `.exe`, o abrirlo y elegirlos en
los diálogos. La ROM se escribe junto al primer zip.
