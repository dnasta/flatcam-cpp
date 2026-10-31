@AGENTS.md

## Claude Code

Las instrucciones del proyecto están en `AGENTS.md` (se importan arriba). En Claude Code podés commitear y hacer
`push` directamente a `main` (sin generar archivos `.patch`) y compilar y ejecutar la aplicación en la propia
máquina del usuario.

## Reglas permanentes del usuario

1. Verificá y corregí todo lo que haga falta **apenas lo detectes**, sin esperar a que lo pida (código, tests y docs).
2. Al terminar cada respuesta, actualizá el estado del proyecto en este archivo y en `AGENTS.md`.

## Estado actual (se actualiza al final de cada respuesta)

- **Fase 3 en curso** (fases 1 y 2 hechas). 454 tests corren siempre (+1 lenta con `FLATCAM_SLOW_TESTS=1`) y pasan en Linux con
  GEOS 3.12 y con 3.15.0; en el Mac del usuario (GEOS 3.15.0) pasaron 445 de 445 con `d8f4630`. Falta confirmar el parche `fase3n`.
- **Hecho:** núcleo de geometría, parsers de Gerber y Excellon, transformaciones, postprocesadores `default` y `GRBL_laser`, G-code
  de taladrado y de geometrías (`CncJob`), **CutOut**, capa de G-code del láser, **Paint estándar** y **NCC estándar** (`src/tools/Ncc.*`,
  `PreparedShape` en `Shape`).
- **Regla de entrega:** correr toda la suite con GEOS 3.12 (sistema) **y** 3.15.0 (micromamba) antes de entregar; tolerancias en micras.
- **NCC de `B_Cu`:** el cobre inferior está reflejado en X; los rangos del encabezado coinciden exactamente; 0,05 % de puntos y 0,015 % de
  longitud de diferencia con el `.nc` real, pero 746 trazados contra 723 (la 8.994 une ~23 tramos más; regla no deducible). Los empates de
  `rtree.nearest` cambian 1 o 2 uniones (no se puede emular sin el R*-tree de libspatialindex).
- **Decisión del usuario:** el G-code del láser sale como el de su FlatCAM 8.994 (con `G00 Z0`), variante por defecto.
- **Siguiente:** `Alinea` (PCB a 2 caras, `ToolDblSided`, aclarar el taladro en (0,0)); después Isolation si el usuario lo confirma.
  Pendientes menores: Paint/NCC con los métodos de semilla y líneas, mecanizado de resto, selección por área.
- **Pendiente de que el usuario aplique:** parche `fase3n-ncc`.
- Detalle completo en `AGENTS.md`, secciones 4, 6, 7, 10 y 11.
