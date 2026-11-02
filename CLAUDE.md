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

- **Tests:** 537 (509 del núcleo + 27 de la GUI corren siempre; 1 lenta con `FLATCAM_SLOW_TESTS=1`). Pasan con GEOS 3.12 y 3.15.0
  y Qt 6.12 en el Mac del usuario (M4, AppleClang). micromamba: `old` (Shapely 1.8.4, GEOS 3.11, PyQt5 5.15, para oráculos y para
  volcar la interfaz original), `sh2`, `geos312`, `geos311` (`AGENTS.md`, secciones 6 y 12.4).
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
- **Fase 4 en curso** (GUI Qt6). Decidido por el usuario: interfaz en español, el alcance propuesto (`AGENTS.md` 12.2.4) y GUI
  **exactamente igual** a la del FlatCAM en Python. **Hecho:** la ventana principal se genera desde la interfaz original real
  (`tools/gui_dump/` → `src/gui/generated/MainGUIBase.*`: 345 acciones, 267 íconos, mismos textos, barras del diseño `minimal`,
  barra de estado), widgets de `GUIElements` portados, canvas `QOpenGLWidget` (ejes, grilla, HUD, zoom, paneo), estilo Fusion
  (desvío consciente para verse como en Windows). Traducción corregida a pedido del usuario ("Abrir", no "Abierto") con
  `tools/gui_dump/es_corrections.json`. `build/src/gui/FlatCAM.app` abre y se ve como las capturas del usuario.
  **También hecho:** abrir Gerber y Excellon (menú, barra, arrastrar y soltar, línea de comandos), árbol del Proyecto y dibujo
  de los objetos con sus colores (la placa "luz de noche" se ve como en las capturas del usuario).
  **También:** Propiedades de Gerber y Excellon y los paneles de NCC, Paint, Perforación y Corte, generados igual (8 paneles).
  **Perforación completa:** desde la interfaz reproduce el `.nc` de la 2024.4 del usuario (salvo fecha y orden de agujeros).
  **Agregado pedido por el usuario:** reordenar los objetos de un grupo del Proyecto con arrastrar y soltar; el orden del árbol
  es el de dibujo (el primero queda arriba; entre grupos, Excellon/Geometría/CNC Job sobre Gerber) (`AGENTS.md` 11.12 h).
  **Corte completo:** Corte → Geometría (Propiedades, parámetros por herramienta) → CNC Job → exportar; reproduce el `.nc` del
  contorno de la 2024.4 línea por línea salvo la fecha (el usuario lo hizo sin profundidad múltiple).
  **También:** varias herramientas en un CNC Job (verificado contra el original), color propio de los puentes delgados,
  viajes engrosados en el dibujo y "mouse bites" con su Excellon (el usuario preguntó si esas limitaciones quedaban: no).
  **Paint y NCC completos** (estándar, "Todos" / "Sí mismo"): el flujo del usuario con Logo y B_Cu da sus `.nc` de la 2024.4.
  Corregida la tolerancia de simplificación de la Geometría (era 0,0005; es 0,00025).
  **Reflejar:** Opciones → Voltear en el eje X / Y y teclas X / Y (también en el árbol). **El alcance v1 acordado está
  completo.** Corrección: las preferencias guardadas del usuario son las de fábrica (los 80 %, Estándar, etc. los escribe a
  mano), así que "cargar sus preferencias" no hacía falta. **PCB a 2 caras** hecho (reflejar con punto/caja/agujero y
  "Alignment Drills"), aceptado por el usuario. **Isolation** hecho (combinado, pasadas separadas, resto, referencia; falta la
  selección por área/polígono con el mouse). **Abrir proyectos** `.FlatPrj` hecho (también el real del usuario de 535 MB: 24 s);
  liblzma pasa a ser dependencia. **Guardar proyectos** hecho; el FlatCAM original abre lo que guarda el port
  (`tests/tools/check_project_in_original.py`). **Todo lo acordado para la fase 4 está hecho.** Decisión del usuario:
  al reabrir un proyecto el port **reconstruye el código de inicio** de los CNC Job (el original lo pierde); los dos CNC Job
  del proyecto real se exportan idénticos a sus `.nc` (`AGENTS.md` 11.13). **En curso (pedido del usuario, en orden):**
  (1) ~~menú contextual del árbol del Proyecto~~ (hecho); (2) ~~proyectos recientes~~ (hecho); (3) Paint/NCC/Isolation: área y polígono
  con el mouse, métodos Semilla y Líneas, mecanizado de resto. Pendientes menores: Paint/NCC con semilla y líneas, resto de Paint/NCC, selección por área,
  verificación de diámetro seguro (`AGENTS.md`, sección 11).
