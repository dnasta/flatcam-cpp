@AGENTS.md

## Claude Code

Las instrucciones del proyecto están en `AGENTS.md` (se importan arriba). En Claude Code podés commitear y hacer
`push` directamente a `main` (sin generar archivos `.patch`) y compilar y ejecutar la aplicación en la propia
máquina del usuario.

## Reglas permanentes del usuario

1. Verificá y corregí todo lo que haga falta **apenas lo detectes**, sin esperar a que lo pida (código, tests y docs).
2. Al terminar cada respuesta, actualizá el estado del proyecto en este archivo y en `AGENTS.md`.

## Estado actual (se actualiza al final de cada respuesta)

- **Fase 3 en curso** (fases 1 y 2 hechas). 463 tests corren siempre (+1 lenta con `FLATCAM_SLOW_TESTS=1`) y pasan en Linux con
  GEOS 3.12 y con 3.15.0; en el Mac del usuario (GEOS 3.15.0) pasaron 455 con `99e4799`. Falta confirmar el parche `fase3o`.
- **Hecho:** núcleo de geometría, parsers de Gerber y Excellon, transformaciones, postprocesadores `default` y `GRBL_laser`, G-code
  de taladrado y de geometrías (`CncJob`), **CutOut**, capa de G-code del láser, **Paint** y **NCC** estándar, y **PCB a 2 caras**
  (`src/tools/DoubleSided.*`: agujeros de alineación con sus reflejos; `Excellon::addDrillTool`).
- **Regla de entrega:** correr toda la suite con GEOS 3.12 (sistema) **y** 3.15.0 (micromamba) antes de entregar; tolerancias en micras.
- **Alinea:** el G-code sale igual que tu `Alinea.nc` salvo (a) líneas en blanco tras cada agujero y (b) un agujero en el origen (0, 0)
  que viene de otro camino de código de la 8.994. **Pregunta abierta al usuario:** ¿querés ese agujero en el centro de la placa? El
  port no lo taladra. `tools/extract_flatprj_params.py --con-agujeros` vuelca los agujeros del proyecto para verlo.
- **Decisión del usuario:** el G-code del láser sale como el de su FlatCAM 8.994 (con `G00 Z0`), variante por defecto.
- **Siguiente:** cerrar la pregunta de Alinea; después Isolation si el usuario lo confirma, y la fase 4 (GUI Qt6; preguntas abiertas
  en `AGENTS.md` sección 11). Pendientes menores: Paint/NCC con los métodos de semilla y líneas, mecanizado de resto, selección por área.
- **Pendiente de que el usuario aplique:** parche `fase3o-alinea`.
- Detalle completo en `AGENTS.md`, secciones 4, 6, 7, 10 y 11.
