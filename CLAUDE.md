@AGENTS.md

## Claude Code

Las instrucciones del proyecto están en `AGENTS.md` (se importan arriba). En Claude Code podés commitear y hacer
`push` directamente a `main` (sin generar archivos `.patch`) y compilar y ejecutar la aplicación en la propia
máquina del usuario.

## Reglas permanentes del usuario

1. Verificá y corregí todo lo que haga falta **apenas lo detectes**, sin esperar a que lo pida (código, tests y docs).
2. Al terminar cada respuesta, actualizá el estado del proyecto en este archivo y en `AGENTS.md`.

## Estado actual (se actualiza al final de cada respuesta)

**Lo que hay que saber para retomar (el detalle está en `AGENTS.md`, sección 12).**

- **Tests:** 498 (497 corren siempre; 1 lenta con `FLATCAM_SLOW_TESTS=1`). Pasan con GEOS 3.12 y 3.15.0, en Linux y en el Mac del
  usuario (M4, AppleClang). En el Mac hay micromamba con los entornos `old` (Shapely 1.8.4, GEOS 3.11, para los oráculos), `geos312` y
  `geos311` (`AGENTS.md`, secciones 6 y 12.4).
- **Hecho (fases 1, 2 y 3 completas):** núcleo de geometría, parsers de Gerber y Excellon, transformaciones, postprocesadores
  `default` y `GRBL_laser`, G-code de taladrado y de geometrías (`CncJob`), CutOut, capa de G-code del láser, Paint estándar, NCC
  estándar, PCB a 2 caras (`src/tools/DoubleSided.*`) e **Isolation** (`src/tools/Isolation.*`: pasadas separadas, combinadas y resto;
  27 escenarios idénticos al código original, que el oráculo ejecuta tal cual extrayéndolo con `ast`).
- **Preguntas que estaban abiertas, ya respondidas por el usuario:**
  1. Agujero (0, 0) de `Alinea.nc`: lo agregó a mano con el editor de G-code de FlatCAM. El port no lo genera.
  2. Isolation: entra en el alcance (ya portado).
  3. Canvas de la fase 4: **`QOpenGLWidget`**.
- **Decisiones del usuario vigentes:** G-code del láser como el de su 8.994 (con `G00 Z0`); optimizador propio de agujeros; método
  estándar de Paint/NCC; sus Gerber inferiores (`B_Cu`, Logo) y los agujeros están reflejados en X; canvas `QOpenGLWidget`.
- **Diferencias medidas 8.994 contra 2024.4:** `GRBL_laser` (cabecera y orden del Z inicial); `Alinea` (líneas en blanco; el (0, 0) lo
  puso el usuario); `B_Cu` 746 trazados contra 723 (la 8.994 une ~23 tramos más; regla no deducible). Todo lo demás coincide.
- **Regla de entrega:** correr toda la suite con GEOS 3.12 **y** 3.15.0 antes de dar algo por terminado; tolerancias en micras, no en el
  último bit (`AGENTS.md`, sección 6). Verificar y corregir apenas se detecte un problema. Actualizar este archivo y `AGENTS.md` antes
  del mensaje final de cada respuesta.
- **Siguiente:** fase 4 (GUI Qt6 con `QOpenGLWidget`). Primero pedirle al usuario capturas de cómo usa FlatCAM y acordar el alcance
  de la primera versión. Pendientes menores: Paint/NCC con semilla y líneas, mecanizado de resto de Paint/NCC, selección por área,
  verificación de diámetro seguro de Isolation (`AGENTS.md`, sección 11).
