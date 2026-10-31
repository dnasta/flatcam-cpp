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

- **Tests:** 502 (501 corren siempre; 1 lenta con `FLATCAM_SLOW_TESTS=1`). Pasan con GEOS 3.12 y 3.15.0, en Linux y en el Mac del
  usuario (M4, AppleClang). En el Mac hay micromamba con los entornos `old` (Shapely 1.8.4, GEOS 3.11, para los oráculos), `sh2`
  (Shapely 2.0), `geos312` y `geos311` (`AGENTS.md`, secciones 6 y 12.4).
- **Hecho (fases 1, 2 y 3 completas):** parsers, transformaciones, postprocesadores, G-code de taladrado y de geometrías, CutOut,
  Paint, NCC, PCB a 2 caras e Isolation. **Verificado contra el ejecutable FlatCAM 2024.4 del usuario** (placa "luz de noche",
  `tests/data/luz_de_noche/`, `tests/test_luz_de_noche.cpp`): CutOut byte por byte, Paint idéntico con GEOS 3.11, NCC 272 = 272
  trazados, taladro con los mismos 120 agujeros.
- **Hallazgos de esta tanda:** el ejecutable 2024.4 difiere del fuente en el orden de dos líneas (nueva variante
  `GcodeFlavor::FlatCAM2024_4Release`); la simplificación `0,00025` aplica también al contorno; lo que el usuario edita a mano en sus
  `.nc` de láser es la variante 8.994 (la predeterminada); los 746 contra 723 trazados del dosificador eran de la 8.994.
- **Decisiones del usuario vigentes:** G-code del láser como la 8.994 (con `G00 Z0`, sin `G00 Z` final); optimizador propio de
  agujeros; método estándar de Paint/NCC; capas inferiores y taladros reflejados en X; Isolation dentro del alcance; el (0, 0) de
  `Alinea` no se genera; canvas `QOpenGLWidget`; los datos de "luz de noche" pueden estar en el repo público.
- **Regla de entrega:** correr toda la suite con GEOS 3.12 **y** 3.15.0 antes de dar algo por terminado; tolerancias en micras, no en el
  último bit (`AGENTS.md`, sección 6). Verificar y corregir apenas se detecte un problema. Actualizar este archivo y `AGENTS.md` antes
  del mensaje final de cada respuesta.
- **Fase 4 en curso** (GUI Qt6, `QOpenGLWidget`). Decidido por el usuario: interfaz en español, el alcance propuesto (ver
  `AGENTS.md` 12.2.4) y que la GUI sea **exactamente igual** a la del FlatCAM en Python. Qt 6.12 instalado con Homebrew; PyQt5 en el
  entorno `old`. Hecho: `tools/gui_dump/fc_runtime.py` construye la `MainGUI` original en español. Siguiente: volcar ese árbol a
  C++ generado, extraer íconos y traducción, objetivo `flatcam` en CMake, canvas. Pendientes menores: Paint/NCC con semilla y líneas,
  resto de Paint/NCC, selección por área, verificación de diámetro seguro (`AGENTS.md`, sección 11).
