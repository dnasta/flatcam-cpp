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

- **Tests:** 464 (463 corren siempre; 1 lenta con `FLATCAM_SLOW_TESTS=1`). Pasan con GEOS 3.12 y 3.15.0. En el Mac del usuario
  (M4, AppleClang, GEOS 3.15.0) pasaron con el commit `e5cbae8`.
- **Hecho (fases 1 y 2 completas; fase 3 casi):** núcleo de geometría, parsers de Gerber y Excellon, transformaciones, postprocesadores
  `default` y `GRBL_laser`, G-code de taladrado y de geometrías (`CncJob`), CutOut, capa de G-code del láser, Paint estándar, NCC
  estándar y PCB a 2 caras (`src/tools/DoubleSided.*`). Todo verificado contra el código original (oráculos en Python con
  Shapely 1.8.4) y, donde hay, contra los `.nc` reales del usuario (FlatCAM 8.994).
- **Preguntas abiertas al usuario (hacerlas primero):**
  1. **¿Querés el agujero en (0, 0) que tu `Alinea.nc` taladra primero?** Es el centro de la placa. El port no lo taladra. Para ver qué tenía
     el objeto: `python3 tools/extract_flatprj_params.py "proyecto.FlatPrj" alinea.params.json --con-agujeros`.
  2. **¿Isolation entra en el alcance?**
  3. **Canvas de la fase 4:** `QOpenGLWidget` o `QGraphicsView`.
- **Decisiones del usuario vigentes:** G-code del láser como el de su 8.994 (con `G00 Z0`); optimizador propio de agujeros; método
  estándar de Paint/NCC; sus Gerber inferiores (`B_Cu`, Logo) y los agujeros están reflejados en X.
- **Diferencias medidas 8.994 contra 2024.4:** `GRBL_laser` (cabecera y orden del Z inicial); `Alinea` (líneas en blanco y el (0, 0));
  `B_Cu` 746 trazados contra 723 (la 8.994 une ~23 tramos más; regla no deducible). Todo lo demás coincide.
- **Regla de entrega:** correr toda la suite con GEOS 3.12 **y** 3.15.0 antes de dar algo por terminado; tolerancias en micras, no en el
  último bit (`AGENTS.md`, sección 6). Verificar y corregir apenas se detecte un problema. Actualizar este archivo y `AGENTS.md` antes
  del mensaje final de cada respuesta.
- **Siguiente:** cerrar las preguntas de arriba; después Isolation (si entra) y la fase 4 (GUI Qt6). Pendientes menores: Paint/NCC con
  semilla y líneas, mecanizado de resto, selección por área (`AGENTS.md`, sección 11).
- **Pendiente de que el usuario aplique:** parche `fase3p-estado` (solo documentación).
