# AGENTS.md — FlatCAM-cpp

Guía para cualquier agente de programación (Claude en el chat, Claude Code, o una terminal) que continúe este
proyecto. Resume qué se quiere, qué decisiones se tomaron, qué está hecho y cómo seguir. **Leelo completo antes
de tocar código** y actualizá las secciones "Estado" y "Pendientes" al terminar cada tanda de trabajo.

## 1. Objetivo

Portar **FlatCAM 2024.4 β** (CAM 2D para fabricar PCB con CNC o láser; Python + PyQt5) a **C++ con Qt6**, para
que se pueda ejecutar en **macOS con Apple Silicon**, y que además compile en **Windows** y **Linux**.

- Repositorio: https://github.com/dnasta/flatcam-cpp (público). Licencia **MIT** (FlatCAM también es MIT).
- El código original en Python está en `reference/FlatCAM_2024_4/` (solo lectura; **no editarlo**). El resto de la
  referencia histórica (FlatCAM 8.5 de 2016) no se usa.
- Es una obra derivada: hay que conservar los copyright originales (ver `NOTICE`) y la licencia (`LICENSE`).

## 2. Requisitos del usuario (no negociables)

1. **Todo en C++** (C++17), con el código **organizado en clases** (una clase por responsabilidad).
2. **Muy bien documentado con Doxygen**: `@brief`, `@param`, `@return` en cada elemento público. Los comentarios
   están **en español**; los identificadores (clases, funciones, variables) **en inglés**, como en FlatCAM.
3. **Multiplataforma**: CMake + Qt6; debe compilar en macOS arm64, Windows y Linux.
4. Se busca un **clon funcional y testeado** (no una copia línea por línea). Las diferencias de comportamiento
   deben ser conscientes y estar documentadas.
5. Que el progreso quede **siempre en Git** (GitHub), para poder seguir desde el chat, Claude Code o una terminal.

### Alcance

Entran: lectura de **Gerber** y **Excellon**, y las herramientas **NCC**, **Paint**, **Drilling** (perforación),
**PCB a 2 caras** (`ToolDblSided`) y **CutOut** (herramienta de corte). **Isolation** se mantiene en el plan
(confirmado por el usuario; ya está portado). Salida de **G-code** con los
postprocesadores que usa el usuario (`default` y `GRBL_laser`; luego el resto) y una **GUI Qt6**.

**Editores** de Gerber, Geometría y Excellon (`appEditors`, ~17.600 líneas): al principio quedaron fuera del alcance (el
usuario no los usa); **después decidió incluirlos en la 1.0.0** (sección 13, ítem 8). Su uso habitual: solo carga archivos y el programa genera la geometría y el G-code. A veces edita a mano el G-code para
láser: con la 2024.4 agrega `G00 Z0` al empezar y quita el `G00 Z..` final. Eso es exactamente la variante
`GcodeFlavor::FlatCAM8_994` del port (la predeterminada), comprobado con su `B_Cu.gbr_ncc_cnc.nc` de la placa "luz de noche".

### Flujo de trabajo del usuario (así usa FlatCAM)

Gerber (cobre inferior, contorno, logo) + Excellon (taladros) → Geometrías con **NCC** (láser), **Paint** (láser,
logo) y **CutOut** → **CNC Job** con G-code. Taladros y alineación con **PCB a 2 caras**. Los taladros salen
**reflejados en X** (cara inferior).

## 3. Convenciones de trabajo con el usuario

- Hablale en **español rioplatense informal**, con "vos".
- Los comandos de terminal van en **zsh** (macOS), no bash. `pip` siempre como `python3 -m pip install ...`.
- La ruta local del repo tiene **espacios** (está dentro de una carpeta de Google Drive): poné las rutas **entre
  comillas** y probá cualquier script o CMake con rutas con espacios.
- Se propuso (y es recomendable) que el repo no esté dentro de Drive, porque Drive sincroniza `build/`.
- **Regla permanente (pedida por el usuario):** verificá y corregí todo lo que haga falta verificar o corregir **apenas lo
  detectes**, sin esperar a que lo pida. Si una afirmación tuya anterior resulta falsa o no comprobada, decilo, corregila
  y arreglá el código, los tests y la documentación en el mismo turno.
- **Regla permanente (pedida por el usuario):** al terminar cada respuesta, el estado del proyecto tiene que estar
  actualizado en `CLAUDE.md` y en `AGENTS.md` (secciones 4 y 11 de este archivo y "Estado actual" de `CLAUDE.md`). Hacelo
  **antes** del mensaje final y entregá esos cambios en el mismo parche.
- **Regla permanente (pedida por el usuario): cómo tratar los errores.** (1) Los errores **del port** se corrigen **apenas se
  encuentran**, sin esperar a que el usuario lo pida. (2) Los errores **del FlatCAM original** se siguen reproduciendo a
  propósito mientras se porta (así el G-code y los archivos salen iguales y se puede verificar contra el original). (3)
  **Cuando el proyecto esté completamente terminado** y se haya verificado que el G-code del port sale igual al del original,
  se corrigen en el código del port **todos** esos errores del original (lista en la sección 14), como desvíos documentados;
  donde sirva, con una opción para volver al comportamiento original y seguir comparando (como `parseHpgl2(...,
  fixOriginalBugs)`). Los archivos de `reference/` no se modifican nunca.
- Nunca pidas ni uses tokens o contraseñas del usuario. En el chat de claude.ai el entorno **puede leer GitHub pero
  no puede hacer `push`** (ver sección 9).

## 4. Estado

| Fase | Contenido | Estado |
|---|---|---|
| 1 | CMake, Doxygen y núcleo de geometría | **Hecha**, verificada en macOS (arm64) y Linux |
| 2 | Parsers de Gerber y Excellon con tests contra archivos reales | **Hecha** |
| 3 | Herramientas (NCC, Paint, Drilling, PCB a 2 caras, CutOut, Isolation) y G-code | **Hecha**: transformaciones de Gerber, postprocesadores, G-code de taladrado y de geometrías, CutOut, capa de láser, Paint, NCC, PCB a 2 caras e Isolation |
| 4 | Interfaz Qt6 (ventana principal, árbol de proyecto, canvas, paneles) | **En curso**: ventana principal idéntica a la original (generada desde la interfaz Python real), widgets de `GUIElements`, canvas `QOpenGLWidget` (ejes, grilla, HUD, zoom, paneo y dibujo de objetos con buffers de vértices), abrir Gerber y Excellon (menú, barra, arrastrar y soltar, línea de comandos) con el árbol del Proyecto; Propiedades de Gerber y Excellon (paneles generados + `build_ui`); paneles de NCC, Paint, Perforación y Corte en la pestaña Herramienta (generados). **Perforación completa:** objeto → tabla de herramientas → parámetros por herramienta → CNC Job (objeto, dibujo y Propiedades) → "Guardar código CNC"; desde la interfaz reproduce el `.nc` de la 2024.4 del usuario (salvo fecha y orden de agujeros). **Corte completo:** Corte libre o rectangular → objeto Geometría (Propiedades con su tabla y parámetros por herramienta) → "Generar objeto CNCJob" → exportar; reproduce el `.nc` del contorno de la 2024.4 **línea por línea salvo la fecha**. **Agregado pedido por el usuario:** los objetos de cada grupo del Proyecto se reordenan arrastrándolos y ese orden es el de dibujo (el primero queda arriba). **Paint y NCC completos** (método estándar, "Todos" / "Sí mismo"): el flujo del usuario sobre Logo y B_Cu da sus `.nc` de la 2024.4 (Paint: mismas líneas salvo la variante del láser y últimas cifras de GEOS; NCC: misma cabecera y los mismos trazados ±2 %). **Reflejar:** Opciones → Voltear en el eje X / Y y las teclas X / Y (también desde el árbol), alrededor del centro de la caja de los objetos elegidos; con las cuatro capas de "luz de noche" elegidas da exactamente el reflejo en x = 0 de los `.nc` del usuario. **PCB a 2 caras:** panel generado y lógica completa (reflejar con punto, caja o agujero elegido con el mouse; límites de lo elegido; agujeros de alineación "Alignment Drills"). **Isolation:** panel generado (nivel Básico) y lógica sobre `src/tools/Isolation`: combinado (`_iso_combined`), pasadas separadas (`_<diá>_isoN`), resto (`_iso_rest`), resta de otro objeto y objeto de referencia. **Abrir proyectos** `.FlatPrj` de la versión Python (también el real del usuario, 535 MB comprimido / 1,7 GB: 24 s y 1,8 GB de memoria): lectura en streaming (`src/io/`) que descarta `gcode_parsed` y las copias de geometría que no se usan. **Guardar proyectos** (Archivo → Guardar proyecto / Guardar como, comprimido con XZ como el original): cada objeto como su `to_dict` original, conservando lo que vino del proyecto; el FlatCAM en Python abre lo que guarda el port (`tests/tools/check_project_in_original.py`) |
| 5 | Herramientas secundarias y postprocesadores restantes | **En curso** (plan en la sección 13): ítems 1, 2, 3, 5 (todas las herramientas) y 8 (editores de Gerber, Excellon y Geometría) hechos (Transformaciones, Mover, Distancia, Distancia mínima, Propiedades, escala y desplazamiento del panel de cada objeto; distancia recorrida y tiempo estimado del CNC Job, idénticos al original); **los 20 postprocesadores hechos**; Preferencias en curso (pestaña, archivos y manejadores hechos), cada método idéntico al original (2.251 casos) y de punta a punta en geometrías (39 escenarios más) y taladrado (17 postprocesadores) |

Fase 1: 46 tests (GoogleTest) pasan en Linux (GEOS 3.12) y en macOS arm64 (AppleClang, GEOS 3.15). Doxygen genera
la documentación sin warnings propios (queda uno informativo: la traducción al español de Doxygen está vieja).
Fase 2: 84 tests pasan en Linux (GEOS 3.12) y en macOS arm64 (1,2 s; `B_Cu` se parsea en 0,6 s). Los parsers coinciden
con los originales de FlatCAM sobre los 5 Gerber y el Excellon reales y sobre 7 Excellon sintéticos (ver sección 7).
**Última confirmación en el Mac del usuario (M4, AppleClang):** 2.763 casos de `ctest` (2.661 del núcleo y 101 de la GUI pasan; 1, la
lenta del NCC del dosificador, queda saltada a propósito), con GEOS 3.15.0 (Homebrew) y con GEOS 3.12 (micromamba, solo el núcleo), y Qt 6.12.

**Contra el FlatCAM 2024.4 real del usuario** (placa "luz de noche", `tests/test_luz_de_noche.cpp`, ver sección 7): CutOut idéntico
byte por byte; Paint idéntico con GEOS 3.11 (17 líneas con cambios de última cifra con 3.12/3.15); NCC con los mismos 272 trazados
(GEOS 3.11/3.12; solo cambian empates de `rtree`); taladro con los mismos 120 agujeros y un recorrido más corto.

Fase 3: 502 tests (501 corren siempre y 1 es lenta; GEOS 3.12 y 3.15.0 en Linux y en el Mac); transformaciones de `Gerber` (24 casos contra el original), postprocesadores
`default` y `GRBL_laser` (224 líneas de G-code idénticas al original) y G-code de taladrado (`CncJob`): idéntico línea por
línea al generador original con el orden del archivo, e igual al `.nc` real del usuario salvo el orden de los agujeros. El G-code de geometrías (`generateFromGeometry`) reproduce
byte por byte `DosCau_PPal-Edge_Cuts.nc` a partir de su propio recorrido.

**Próximos pasos de la fase 3, en orden:** (1) ~~CutOut~~ (hecho: libre, rectangular y puente manual, reproduce el `.nc` del contorno); (2a) ~~capa de G-code del láser~~ (hecha: `B_Cu.nc` y `Logo.nc` se reproducen byte por byte desde sus propios trazados, con `GcodeFlavor::FlatCAM8_994`); (2b) ~~**Paint**~~ (hecho, método estándar: `src/tools/Paint.*`, `src/cam/PolygonClearing.*`, `src/cam/EndpointStorage.*`, orden `R` en `CncJob`); (2c) ~~**NCC**~~ (hecho, "Él mismo" sobre un Gerber con el método estándar: `src/tools/Ncc.*`; verificado exacto contra el original con casos sintéticos y equivalente a `B_Cu.nc`, ver sección 7); (3) ~~PCB a 2 caras (`Alinea`)~~ (hecho: `src/tools/DoubleSided.*`, agujeros de alineación y su G-code; el taladro en el origen lo agregó el usuario a mano, ver sección 11); (4) ~~Isolation~~ (hecho: `src/tools/Isolation.*`, los tres modos del original, verificado contra su código, ver sección 7). Dato del `.nc` del contorno: tiene
**dos** trazados (puentes arriba y abajo), no uno.

## 5. Arquitectura

Se separa el **núcleo sin Qt** (se prueba sin interfaz) de la **capa de objetos y GUI** (futura, con Qt6). En el
original Python las clases mezclan ambas cosas (`GerberObject(FlatCAMObj, Gerber)`, donde `FlatCAMObj` es un
`QObject`); en C++ los parsers y la geometría viven en el núcleo y los objetos Qt los envolverán después.

```
src/core/              Núcleo (biblioteca estática flatcam_core, namespace flatcam)
  Types.h/.cpp           Point2D, BoundingBox, Units, JoinStyle, CapStyle, ArcDirection, Axis
  GeometryError.h        Excepción de las operaciones geométricas
  Logger.h               Registro con destino reemplazable (reemplaza a `logging`)
  AffineTransform.*      Escala/rotación/inclinación/reflexión (misma matemática que shapely.affinity)
  Shape.*                Figura 2D inmutable sobre GEOS (reemplaza a los objetos de Shapely)
  ArcMath.*              arc, arc2, arc_angle, three_point_circle, distance (de camlib.py)
  Geometry.*             Port de camlib.Geometry (contenedor de geometría sólida)
  internal/GeosContext.* Contexto GEOS por hilo (no es API pública)
src/parsers/           Lectores de archivos (sin Qt, derivan de Geometry)
  ApertureMacro.*        Macros de apertura de Gerber (`%AM`), con su evaluador de expresiones
  DxfImport.*            Importar DXF (ParseDXF + ParseDXF_Spline + import_dxf_as_geo/as_gerber), lector DXF propio
  PdfImport.*            Importar PDF (PdfParser.parse_pdf y las capas de ToolPDF): los operadores del contenido, una capa por
                         color de trazo y la 0 con los rellenos blancos (agujeros)
  ImageImport.*          Importar imagen (Geometry.import_image): la unión de los píxeles que pasan el umbral
  Hpgl2.*                Importar HPGL2 (ParseHPGL2), con sus rarezas
  SvgImport.*            Importar SVG (ParseSVG + svg.path 7.1 + Geometry/Gerber.import_svg), con un lector XML mínimo
  Gerber.*               Parser de Gerber RS-274X (aperturas, trazos, arcos, regiones, polaridad)
  Excellon.*             Parser de Excellon (herramientas, agujeros, ranuras) y sus transformaciones
  (Gerber.* también trae scale, offset, mirror, rotate, skew, buffer y getBoundingBox)
src/tools/              Herramientas de FlatCAM (sin Qt)
  Paint.*                Pintado de polígonos (`ToolPaint.py`): todos los polígonos, resto y "Líneas láser" (`fill_with_lines`
                         sobre las líneas centrales de los trazos del Gerber; necesita sus aperturas)
  Ncc.*                  Limpieza de cobre (`ToolNCC.py`): toda la placa, zona o referencia, resto y la operación
                         "Aislamiento" (recorrido de aislamiento y limpieza afuera de su envolvente)
  DoubleSided.*          PCB a 2 caras: agujeros de alineación con sus reflejos (`ToolDblSided.py`)
  CutOut.*               Corte de contorno con puentes: libre, rectangular y puente manual (`ToolCutOut.py`)
  Isolation.*            Aislamiento: pasadas separadas, combinadas y mecanizado de resto (`ToolIsolation.py`)
  Panelize.*             Panelizar (`ToolPanelize.on_panelize`): grilla con restricción de área, Excellon, Gerber (sólido y
                         aperturas) y Geometría por herramienta (con "Optimizar": `snap` + `linemerge` + `unary_union`)
  QrCode.*               Código QR: port de la biblioteca `qrcode` 7.4.2 (BSD; matriz, SVG de fragmentos y de trayecto) y
                         de `ToolQRCode` (geometría y `make`); tablas en generated/QrTables.h (tools/gen_qr_tables.py)
  CopperThieving.*       Robo de cobre (`copper_thieving`, `on_add_robber_bar_click`, `on_new_pattern_plating_object`):
                         relleno sólido, de puntos, cuadrados o líneas, barra de robo y máscara para el galvanizado
  Fiducials.*            Fiduciales (`add_fiducials_geo`): circulares, en cruz o de tablero, y los puntos automáticos
  AlignObjects.*         Alinear objetos: los pads o agujeros bajo el clic y el desplazamiento y giro que resultan
  EtchCompensation.*     Compensación de grabado (`on_compensate`): el desplazamiento y el cobre y las aperturas engrosados
  OptimalDistance.*      Óptimo: la distancia mínima entre las figuras de cobre (`find_minimum_distance`)
  Subtract.*             Restar: Gerber menos Gerber (`aperture_intersection` + `new_gerber_object`) y Geometría menos
                         Geometría (`toolgeo_intersection`)
  PunchGerber.*          Perforar un Gerber (con un Excellon, fijo, anillo o proporcional) y sus aperturas `clear`
  ExtractDrills.*        Extraer taladros de los pads de un Gerber (fijo, anillo, proporcional)
  CornerMarkers.*        Marcas de esquina (`add_corners_geo`) y sus taladros (`on_create_drill_object`)
  InvertGerber.*         Invertir un Gerber (`on_grb_invert`): la caja con margen menos el cobre
  ExcellonMilling.*      Fresar agujeros y ranuras de un Excellon (`generate_milling_drills` / `_slots` de FlatCAMExcellon)
  RulesCheck.*           Verificación de reglas (`check_*` de ToolRulesCheck) y el informe HTML (`on_tool_finished`)
  SolderPaste.*          Pasta de soldar: trazos por boquilla (`on_create_geo`) y G-code con Paste_1
  PcbWizard.*            Asistente de PCB: el `.INF` de PcbWizard (`load_inf`) y la cabecera agregada al Excellon
  Calibration.*          Calibración: G-code de verificación, factores (`calculate_factors`) y puntos escalados/inclinados
  Film.*                 Film: el SVG positivo o negativo (`export_positive` / `export_negative` + `Geometry.export_svg`,
                         con lxml y minidom imitados) y el perforado con un Excellon o con el centro de los pads
src/editors/            Modelos de los editores (sin Qt)
  EditorStorage.*        FlatCAMRTreeStorage + DrawToolShape.get_pts: figuras e índice de vértices (tomado al insertar)
  GeoEditorModel.*       Editor de Geometría: figuras de las herramientas de dibujo, elección, unir, intersecar, restar,
                         cortar, explotar, goma, pintar, transformar, on_shape_complete (fresado por ascenso)
  ExcEditorModel.*       Editor de Excellon: agujeros (cruces) y ranuras por diámetro, grillas, cambio de diámetro, guardar
  GrbEditorModel.*       Editor de Gerber: elementos por apertura, pads, codos de pistas y regiones, semidiscos, engrosar,
                         escalar, poligonizar, marcar área, goma, mover, guardar
src/io/                 Proyectos (sin Qt; depende de liblzma)
  JsonValue.*            Valor JSON liviano (los números conservan su texto, para escribirlos igual que Python)
  DxfExport.*            Exportar una Geometría a DXF (export_dxf sobre la plantilla R2010 generada de ezdxf)
  SvgExport.*            Exportar a SVG (Geometry.export_svg + App.export_svg)
  FileExport.*           Exportar Gerber y Excellon (export_gerber / export_excellon), idéntico al original
  ProjectFile.*          .FlatPrj: lectura en streaming de JSON (con o sin XZ) que puede descartar valores sin guardarlos;
                         escritura idéntica a json.dumps(indent=2, sort_keys=True) de Python, con o sin XZ
src/cam/                Generación de G-code (sin Qt)
  PyNumber.*             Número que recuerda si era int o float, para imprimir como `str()` de Python
  Preprocessor.*         Interfaz `Preprocessor`, `default`, `GRBL_laser` y `SolderPastePreprocessor` (Paste_1)
  MorePreprocessors.cpp  Los otros 17 (Berta_CNC, Default_no_M6, grbl_11, GRBL_11_no_M6, hpgl, ISEL_CNC, ISEL_ICP_CNC,
                         line_xyz, Marlin, Marlin_laser_FAN_pin/Spindle_pin, Repetier, Roland_MDX_20, Toolchange_Custom,
                         Toolchange_Manual, Toolchange_Probe_MACH3, Z_laser) y la cabecera/cambio de herramienta comunes
  internal/PpCommon.h    Ayudantes (formato como Python, `StartStyle` de la cabecera, `pyFormat` de las plantillas)
  generated/PreprocessorTemplates.h  Plantillas multilínea originales tal cual (GENERADO por tools/gen_pp_templates.py)
  Autolevel.*            Autonivelación: grilla de puntos de sondeo, celdas de Voronoi (Shape::voronoiDiagram), G-code de
                         sondeo (MACH3/MACH4/LinuxCNC), mapa de alturas y comandos de avance de GRBL
  ExclusionAreas.*       Áreas de exclusión: `travel_coordinates` (rodear o pasar por encima), usado en los viajes del CNC Job
  PathOptimizer.*        Orden de los agujeros (vecino más cercano + 2-opt; reemplaza a OR-Tools)
  EndpointStorage.*      Almacén de líneas por extremos con `nearest` y borrado (`FlatCAMRTreeStorage`, con una grilla)
  PolygonClearing.*      `clear_polygon` (estándar), `clear_polygon2` (semilla), `clear_polygon3` (líneas), combinado y
                         `paint_connect` del original
  CncJob.*               G-code de taladrado (`excellon_tool_gcode_gen`) y de geometrías (`geometry_tool_gcode_gen`, `linear2gcode`), y archivo exportado
src/gui/               Interfaz Qt6 (biblioteca flatcam_gui y ejecutable FlatCAM; namespace flatcam::gui)
  generated/MainGUIBase.*  Ventana principal GENERADA por tools/gui_dump/gen_cpp.py (no editar): menús, acciones,
                         barras, pestañas, barra de estado y consola, con los nombres de atributo del original
  MainGUI.*              Lógica de la ventana (deriva de MainGUIBase): canvas, coordenadas, botones de la barra de estado
  ProjectIO.*            .FlatPrj: objetos desde su from_dict y hacia su to_dict (Gerber, Excellon, Geometría, CNC Job), qué
                         se descarta al leer, gcode_parse portado (sin arcos) para el gcode_parsed que dibuja el original
  resources/factory_defaults.json  Opciones de fábrica de FlatCAM 2024.4 (tools/gui_dump/extract_defaults.py)
  AppRecent.cpp          Archivos y proyectos recientes (register_recent / setup_recent_items): recent.json y
                         recent_projects.json en la carpeta de datos (QStandardPaths AppDataLocation; en los tests, test mode)
  ExclusionAreasManager.* Áreas de exclusión de la aplicación (`app.exc_areas`): agregarlas con el mouse, tabla, borrar; las
                         usan todos los CNC Job de Perforación y Geometría
  CanvasPicker.*         Elegir con el mouse: zonas (rectángulos de dos clics o polígonos punto a punto) o polígonos
                         (marcar/desmarcar); clic derecho termina, Escape cancela; capa auxiliar de selección encima de todo
  AppProjectMenu.cpp     Menú contextual del árbol del Proyecto (menuproject): mostrar/ocultar, color, crear CNC Job, ver
                         fuente, copiar, borrar (también Supr), guardar el objeto, propiedades
  App.*                  La aplicación (partes de app_Main.App, AppIO y AppObject): abrir Gerber/Excellon, crear, dibujar, encuadrar
  objects/               FlatCAMObj (base), GerberObject, ExcellonObject y ObjectCollection (modelo del árbol del Proyecto)
  PlotCanvas.*           Canvas QOpenGLWidget: ejes con números, grilla, líneas por el origen, cursor, HUD, zoom y paneo;
                         cada objeto es una capa con triángulos (Shape::triangulate, Delaunay restringido de GEOS) y contornos
                         en buffers de vértices, con sus colores (#RRGGBBAA de FlatCAM)
  Translation.*          tr_(): los mismos textos en español de FlatCAM (resources/es.json, de translate/es.bin)
  widgets/               Port de GUIElements.py: FCLabel, FCEntry/FCEntry2, FCCheckBox, VerticalScrollArea, FCDock,
                         FCTree, KeySensitiveListView, FCDetachableTab(2), FlatCAMInfoBar, FlatCAMActivityView, FCButton,
                         FCSpinner/FCDoubleSpinner, FCComboBox(2), RadioSet, FCTable, NumericalEval(Tuple)Entry (con un
                         evaluador propio de fórmulas en vez del eval de Python)
  generated/*UIBase.*    Paneles GENERADOS: GerberObjectUI, ExcellonObjectUI, GeometryObjectUI, CNCObjectUI (Propiedades)
                         y NccToolUI, PaintToolUI, DrillingToolUI, CutOutToolUI (pestaña Herramienta), desde
                         tools/gui_dump/dump_panels.py + gen_cpp.py --panels (las herramientas después de su run() original;
                         los de Propiedades después del set_ui del objeto original: nivel Básico y valores de fábrica)
  objects/ObjectUIs.*    build_ui de Gerber (tabla de aperturas), Excellon (tabla de herramientas con totales), Geometría
                         (tabla, formulario por herramienta, parámetros comunes, postprocesador) y CNC Job
  objects/GeometryObject.* Objeto Geometría (multigeo): herramientas con sus parámetros y su geometría; dibujo en #FFAAAA;
                         makeCncJob = mtool_gen_cncjob (una herramienta por vez)
  tools/ToolPanels.*     Incluye todas las herramientas; lista de postprocesadores del combo
  tools/ClearingTools.*  Lo común de Paint y NCC: almacén de herramientas, tabla (#, Diámetro, TT), orden; BusyScope
  tools/PaintTool.*      Paint (port de ToolPaint: método estándar, selección "Todos") → `<objeto>_mt_paint`
  tools/NccTool.*        NCC (port de ToolNCC: "Limpiar", estándar, "Sí mismo") → `<objeto>_ncc`
  tools/IsolationTool.*  Isolation (port de ToolIsolation sobre src/tools/Isolation; falta la selección por área y polígono)
  AppEditors.cpp         object2editor / editor2object: entrar y salir de los editores (pestaña "Editor" en rojo)
  editors/GeoEditor.*    Editor de Geometría (AppGeoEditor) y sus paneles de Pintar, Transformar y Texto (generados)
  editors/ExcEditor.*    Editor de Excellon (AppExcEditor): tabla de herramientas, agujeros, ranuras, grillas
  editors/GrbEditor.*    Editor de Gerber (AppGerberEditor): tabla de aperturas y sus herramientas
  tools/RulesTool.*      Verificación de reglas (ToolRulesCheck, Alt+R): el informe en un Documento `Rules_check_results`
  objects/DocumentObject.* Objeto Documento mínimo (texto HTML en un QTextBrowser)
  tools/SolderPasteTool.* Pasta de soldar (ToolSolderPaste, Alt+K): boquillas, `<objeto>_solderpaste` y su CNC Job
  tools/CalibrationTool.* Calibración (ToolCalibration, sin atajo): los cuatro puntos con el mouse, el "Visor de Gcode" y
                         `<objeto>_calibrated`
  tools/ImageTool.*      Importar imagen (ToolImage, Archivo -> Importar): las bandas con QImage como las lee rasterio
  tools/FilmTool.*       Film (ToolFilm, Alt+L): SVG; PNG y PDF dibujando ese SVG con QtSvg (Qt6::Svg)
  tools/PcbWizardTool.*  Asistente de PCB (ToolPcbWizard, Archivo -> Importar): cargar el Excellon y el `.INF`, importar
  tools/QRCodeTool.*     Código QR (ToolQRCode, Alt+Q) sobre src/tools/QrCode: vista previa, clic, PNG y SVG
  tools/ThievingTool.*   Robo de cobre (ToolCopperThieving, Alt+J) sobre src/tools/CopperThieving; el área con CanvasPicker
  tools/FiducialsTool.*  Fiduciales (ToolFiducials, Alt+F) sobre src/tools/Fiducials: automático o con clics
  tools/AlignTool.*      Alinear objetos (AlignObjects, Alt+A) sobre src/tools/AlignObjects: elegir con el mouse
  tools/EtchTool.*       Compensación de grabado (ToolEtchCompensation, sin atajo) sobre src/tools/EtchCompensation
  tools/OptimalTool.*    Óptimo (ToolOptimal, Alt+O) sobre src/tools/OptimalDistance
  tools/SubTool.*        Restar (ToolSub, Alt+W) sobre src/tools/Subtract
  tools/PunchTool.*      Perforar Gerber (ToolPunchGerber, Alt+H) sobre src/tools/PunchGerber
  tools/ExtractDrillsTool.* Extraer taladros (ToolExtractDrills, Alt+E) sobre src/tools/ExtractDrills
  tools/CornersTool.*    Marcas de esquina (ToolCorners, Alt+M) sobre src/tools/CornerMarkers
  tools/CalculatorsTool.* Calculadoras (ToolCalculators, Alt+C): fresa en V, mm/pulgadas y galvanizado
  tools/InvertTool.*     Invertir Gerber (panel generado de ToolInvertGerber, Alt+G) sobre src/tools/InvertGerber
  tools/PanelizeTool.*   Panelizar (panel generado de ToolPanelize, Alt+Z) sobre src/tools/Panelize
  tools/DblSidedTool.*   PCB a 2 caras (port de ToolDblSided, sobre src/tools/DoubleSided): reflejar y "Alignment Drills"
  tools/TransformTool.*  Transformaciones (ToolTransform): rotar, inclinar, escalar, desplazar, reflejar y engrosar
  tools/DistanceTool.*   Distancia (ToolDistance: dos clics, Ctrl+M) y Distancia mínima (ToolDistanceMin: entre dos objetos,
                         Shift+M; salta al punto medio con App::jumpTo, el on_jump_to del original)
  tools/MoveTool.*       Mover (ToolMove, sin panel; tecla M): dos clics, arrastra la caja de la selección
  tools/PropertiesTool.* Propiedades (ToolProperties, tecla P y "Propiedades" del menú del Proyecto): árbol con los datos
  AppCanvasSelection.cpp Elegir objetos con el mouse en el canvas (select_objects, selection_area_handler, recuadros)
  AppEditMenu.cpp        Archivo -> Nuevo proyecto, menús Editar y Ver y el menú del canvas (popMenu, con las grillas)
  widgets/DialogBoxRadio.* Diálogos de "Saltar a" (DialogBoxRadio), "Ubicar" (DialogBoxChoice) y FCInputDoubleSpinner
  AppJoin.cpp / AppConvert.cpp  Editar -> Unir y Editar -> Convertir
  AppImport.cpp          Archivo -> Importar -> SVG como Geometría o Gerber, DXF, HPGL2 y PDF (`openPdf`: los streams FlateDecode
                         con qUncompress, Ctrl+Q)
  AppExport.cpp          Archivo -> Exportar -> Gerber / Excellon (sobre src/io/FileExport) y el texto fuente regenerado
  Preferences.*          Preferencias (FlatCAMDefaults + PreferencesUIManager): app.defaults, current_defaults.FlatConfig y
                         factory_defaults.FlatConfig en la carpeta de datos, la pestaña (generated/PreferencesTabUIBase, de
                         tools/gui_dump/dump_preferences.py), leer/escribir los 485 campos con el tipo de Python (resources/
                         preferences_fields.json), Guardar/Aplicar/Cerrar/Restaurar, "editado" en rojo
  AppUnits.cpp           Cambiar las unidades (on_toggle_units, scale_defaults) y convertir los objetos
  PreferencesGroups.cpp  Las señales de cada grupo de Preferencias: colores y alfa (__init_color_pickers), formatos de
                         Excellon, asociaciones de archivos y palabras clave
  widgets/FCPrefWidgets.* FCColorEntry, FCSliderWithSpinner, FCTextArea y FCTab de GUIElements
  AppHelp.cpp            Ayuda: Acerca de y Cómo empezar (generated/AboutDialogUIBase, HowtoDialogUIBase), enlaces, atajos
  tools/CutOutTool.*     Corte completo (port de ToolCutOut: libre y rectangular → `<objeto>_cutout`)
  tools/DrillingTool.*   Perforación completa (port de ToolDrilling: tabla, almacén de parámetros con el tipo de Python de
                         cada valor, formulario, postprocesador, generar el CNC Job)
  objects/CNCAutolevel.* Autonivelación del panel del CNC Job: puntos en grilla o con clics, tabla, celdas en el canvas,
                         guardar/ver el G-code de sondeo, mapa de alturas y GRBL por QSerialPort (opcional)
  objects/CNCJobObject.* Objeto CNC Job: G-code, dibujo (viajes, cortes, agujeros), exportación con la cabecera de la 2024.4
  resources/             Íconos de FlatCAM (extraídos de resources.py), es.json y flatcam.qrc (generados)
  main.cpp               Arranque; estilo Fusion por defecto; --screenshot <png> [ancho alto] para pruebas
tools/gui_dump/        fc_runtime.py (carga la interfaz original con PyQt5 y emula el arranque de App), dump_tree.py
                       (árbol de widgets -> JSON), gen_cpp.py (JSON -> C++), extract_resources.py (íconos y textos)
tests/                 GoogleTest; golden_*.h generados; data/ con archivos reales; tools/ con los oráculos; gui/ (Qt)
reference/             Fuentes Python originales (solo lectura)
```

Decisiones de diseño ya tomadas:

- **GEOS** reemplaza a Shapely, usando la **API C reentrante** (estable entre versiones). `geos_c.h` **no** se
  incluye en los encabezados públicos: `Shape.h` solo declara `struct GEOSGeom_t;`. Se exige GEOS >= 3.11
  (`GEOSGeom_transformXY_r`). Cada hilo tiene su propio contexto.
- `Shape` es **inmutable** y de **propiedad compartida** (`shared_ptr`); toda operación devuelve una figura nueva.
  Los errores de GEOS lanzan `GeometryError`.
- Las listas (anidadas) de objetos Shapely se representan con `ShapeList = std::vector<Shape>` plana; las figuras
  múltiples se expanden al aplanar (`parts()`, `Geometry::flatten`).
- Los anillos exigen **al menos 4 coordenadas** (como Shapely) y se cierran solos.
- `AffineTransform::rotation` anula seno/coseno menores a 2.5e-16 (igual que Shapely) para que los giros de
  múltiplos de 90° sean exactos; esto afecta al G-code.
- Desvíos conscientes respecto de Python (documentados en `Geometry.h`): `bounds()` devuelve `(0,0,0,0)` sin
  geometría en vez de infinitos; `union()` se llama `unionAll()` (palabra reservada); `solid_geometry = None` y
  `[]` son equivalentes; lo que depende de la aplicación (progreso, opciones, mensajes) queda para capas superiores.
- Las subclases de `Geometry` deben reimplementar `scale` y `offset` (en la base no hacen nada, como en Python).

## 6. Cómo compilar, probar y documentar

Requisitos: compilador C++17, CMake >= 3.20, GEOS >= 3.11, liblzma (xz; `brew install xz`, `apt install liblzma-dev`,
vcpkg `liblzma`), opcional Doxygen y GoogleTest (si falta, CMake lo baja).

```zsh
# macOS (Homebrew)
brew install cmake ninja geos doxygen googletest
cmake -S . -B build -G Ninja
cmake --build build
ctest --test-dir build --output-on-failure
cmake --build build --target docs        # HTML en build/doxygen/html/index.html
```

Linux: `sudo apt install build-essential cmake ninja-build libgeos-dev doxygen libgtest-dev`. Windows: ver `README.md`
(vcpkg + Visual Studio). Flags: `-DFLATCAM_BUILD_TESTS=OFF`, `-DFLATCAM_BUILD_DOCS=OFF`.

Reglas de calidad (deben cumplirse antes de dar una tanda por terminada):

- Compilar **sin warnings** con `-Wall -Wextra -Wpedantic`.
- **Todos los tests en verde** (`ctest`). Cada clase nueva lleva sus tests.
- **Doxygen sin warnings de elementos sin documentar** (`WARN_IF_UNDOCUMENTED` y `WARN_NO_PARAMDOC` están activos).
  No usar fórmulas LaTeX en los comentarios (el usuario no tiene LaTeX instalado; usar texto plano o `@code`).
  `Doxyfile.in` pone entre comillas las rutas por los espacios; mantenerlo así.
- Los `.cpp` no se procesan con Doxygen: la API se documenta en los encabezados.

**Interfaz (Qt6):** `brew install qt` (Qt 6.12 en el Mac del usuario; trae QtSvg, que usa Film, y QtSerialPort, opcional, para
GRBL); CMake la encuentra solo y arma `build/src/gui/FlatCAM.app`
y `gui_tests` (corren con `QT_QPA_PLATFORM=offscreen`). Sin Qt6 se compila solo el núcleo (`-DFLATCAM_BUILD_GUI=OFF` lo fuerza).
Para regenerar la ventana principal desde el original: `MAMBA_ROOT_PREFIX=$HOME/.local/share/mamba micromamba run -n old python
tools/gui_dump/dump_tree.py` (desde `tools/gui_dump/`) y después `python3 tools/gui_dump/gen_cpp.py`. Íconos y traducción:
`micromamba run -n old python tools/gui_dump/extract_resources.py`. Captura para comparar con el original:
`build/src/gui/FlatCAM.app/Contents/MacOS/FlatCAM --screenshot salida.png 1366 705`.

**Regla de entrega: probar con dos versiones de GEOS.** El `buffer` y el resto de GEOS cambian de versión en versión, y el
Mac del usuario trae la más nueva (3.15.0). Un test con tolerancia de 1e-9 que pasa con la GEOS del sistema (3.12) puede fallar
con la 3.15 (pasó con el Paint: un vértice de 2.041 difería 6,5e-5 mm en el último anillo, que casi se colapsa). Antes de entregar
hay que correr toda la suite contra las dos:
`micromamba create -n geos315 -c conda-forge geos=3.15`, y después
`cmake -S . -B /tmp/b315 -G Ninja -DCMAKE_PREFIX_PATH=$E -DGEOS_DIR=$E/lib/cmake/GEOS -DCMAKE_BUILD_RPATH=$E/lib` con
`E=<raíz del entorno>` y `LD_LIBRARY_PATH=$E/lib /tmp/b315/tests/core_tests`. **En el Mac del usuario** la GEOS de Homebrew es la
3.15.0 (build normal en `build/`) y la segunda es la 3.12 de micromamba (ya instalada, entorno `geos312`):
`E=$HOME/.local/share/mamba/envs/geos312; cmake -S . -B /tmp/b312 -G Ninja -DCMAKE_PREFIX_PATH=$E -DGEOS_DIR=$E/lib/cmake/GEOS
-DCMAKE_BUILD_RPATH=$E/lib -DFLATCAM_BUILD_DOCS=OFF && cmake --build /tmp/b312 && /tmp/b312/tests/core_tests` (con el rpath no hace
falta `DYLD_LIBRARY_PATH`). También está el entorno `geos311` (la GEOS del oráculo) para separar diferencias de versión. Las tolerancias de los tests de geometría se
fijan con sentido físico (micras), no en el último bit.

## 7. Cómo se verifica que el port reproduce a FlatCAM

- `tests/tools/gen_golden.py` calcula con **Shapely y numpy** los valores esperados (áreas de buffers, uniones,
  transformaciones, arcos) y escribe `tests/golden_data.h`. Las funciones de arcos son copias fieles de `camlib.py`.
  Para regenerar: `python3 -m pip install shapely numpy` y `python3 tests/tools/gen_golden.py`. No editar a mano.
- **Oráculos de Python (fase 2):** `tests/tools/oracle_gerber.py` y `oracle_excellon.py` ejecutan el parser
  **original** de FlatCAM (con las dependencias de Qt, rtree, etc. reemplazadas por módulos vacíos; ver
  `oracle_common.py`) sobre `tests/data/` y escriben `tests/golden_gerber.h` y `tests/golden_excellon.h`. Los tests
  de C++ comparan áreas, límites, cantidad de figuras, aperturas, taladros y 1.200 puntos de muestra por archivo.
  El original usa **Shapely 1.8.4**, y algunos resultados cambian con Shapely 2 (p. ej. `buffer` de Gerber: 31 polígonos
  con 1.8.4 contra 3 con 2.x). Por eso los goldens se generan con 1.8.4. Entorno: bajar `micromamba` de
  `conda.anaconda.org/conda-forge/linux-64/` y `micromamba create -n old -c conda-forge python=3.10 shapely=1.8.4 numpy=1.22`
  (en su GEOS 3.11.0); con `python3 -m pip install shapely numpy` (Shapely 2.x) casi todo coincide, pero no esa cantidad.
  Todos los goldens están generados con Shapely 1.8.4 (GEOS 3.11.0), incluido `golden_gerber.h` (cantidades de polígonos,
  agujeros y vértices iguales a los de Shapely 2; solo cambian las últimas cifras de las áreas, ~1e-9 relativo).
  Verificado: con Shapely 1.8.4 (GEOS 3.11.0) y con 2.2 (GEOS 3.14.1) salen idénticos los goldens de preprocesadores,
  taladrado, geometría, Excellon y los 26 escenarios de `geometry_modes` (incluidos los desplazamientos de herramienta).
  No editar los `golden_*.h` a mano. Para cada herramienta de la fase 3 conviene hacer un oráculo igual (ver
  `tests/tools/oracle_common.py`) además de comparar contra los `.nc` de `tests/data/`.
- `tests/tools/oracle_gerber_ops.py` genera `tests/golden_gerber_ops.h`: aplica las operaciones originales (scale, offset,
  mirror, rotate, skew, buffer con y sin factor) y resume el resultado (polígonos tras unir, área, límites, aperturas).
- `tests/tools/oracle_preprocessors.py` genera `tests/golden_preprocessors.h`: ejecuta los **20** postprocesadores originales con 8
  juegos de parámetros y guarda el texto de cada método (2.251 casos, más una lista de números para `pyFloatRepr`). Paste_1 tiene
  otra interfaz (`AppPreProcTools`): se le pasan el X,Y de cambio y el final como texto `"x,y"` y se comparan sus métodos propios.
  Las plantillas multilínea (con sus espacios finales) no se copian a mano: `tools/gen_pp_templates.py` las extrae con `ast`.
  De punta a punta: `oracle_geometry_modes.py` suma 39 escenarios con los otros postprocesadores (con y sin cambio de herramienta,
  corte extra con Roland, profundidad múltiple con Marlin y line_xyz, hpgl en pulgadas) y `oracle_drill.py` taladra el Excellon del
  usuario con los 17 de Excellon: todo idéntico al original. Los oráculos le dan a la app simulada un `log`, porque `doformat`
  atrapa los errores de un postprocesador y deja una línea vacía (pasa con Roland en los tramos del corte extra, sin `z`).
- `tests/tools/oracle_drill.py` genera `tests/golden_drill.h`: ejecuta `CNCjob.excellon_tool_gcode_gen` **original** sobre el
  Excellon del usuario (reflejado en X) con los parámetros del CNC Job guardado en el `.FlatPrj` (la herramienta 4 corta
  a -4,5; el Excellon guarda el mismo diccionario para todas). La rama `"No"` del original está rota, así que el oráculo usa
  la rama `'T'` con un optimizador que devuelve el orden del archivo. El archivo trae además la función que arma esos
  mismos parámetros en C++.
- `tests/tools/oracle_geometry.py` genera `tests/golden_geometry.h`: toma los dos trazados del contorno de
  `DosCau_PPal-Edge_Cuts.nc` como geometría de entrada y ejecuta `CNCjob.geometry_tool_gcode_gen` **original** con los
  parámetros del proyecto. Como la entrada sale del propio `.nc`, el G-code generado debe ser idéntico a ese archivo
  (salvo los `\r` de Windows): lo es. Lo que NO prueba es la creación de la geometría del CutOut (ver sección 10).
- `tests/tools/oracle_ncc.py` genera `tests/golden_ncc.h` (1,8 MB): transcribe `ToolNCC.clear_copper` (región = envolvente convexa del
  cobre, agrandada el margen con esquinas en ingletes, menos el cobre; después `clear_polygon` por polígono) sobre 8 casos
  sintéticos. Con GEOS 3.12 los 8 coinciden con el original vértice por vértice. **Con GEOS 3.15 el contorno de algunos polígonos
  empieza en otro vértice**, la región se compara sin depender de eso y, cuando cambia la unión de anillos (±1 trazado, hasta
  1,2 % de longitud), el test compara el total de puntos (exacto), la longitud (2 %) y la cantidad de trazados (±1).
- **NCC de `B_Cu` contra el `.nc` real** (`NccEndToEnd`, solo con `FLATCAM_SLOW_TESTS=1`, ~100 s): el cobre inferior está
  **reflejado en X** como el Logo y la envolvente convexa da exactamente los rangos del encabezado (X −35,4124…35,6520; Y
  −36,7244…36,9060). Mi resultado: 498.847 puntos contra 499.094 (0,05 %), 68.460,9 mm contra 68.471,2 mm (0,015 %) y 746 trazados
  contra 723: la 8.994 une unos 23 tramos más (~0,45 mm cada uno). Probé en el `paint_connect` otros radios, otros largos máximos y
  otras resoluciones del engrosado (16 a 64) y ninguna da 723 (el conteo salta de forma no monótona), así que la regla de la 8.994 no
  se pudo deducir. Original (Python) 567 s, port 56 s.
- **Empates de `rtree.nearest`**: repitiendo mis 5.509 elecciones del NCC contra el `rtree` real, solo ~15 difieren y todas son
  empates exactos (0,02 mm) entre líneas distintas. El `rtree` real elige a veces la línea insertada antes y a veces la insertada
  después (7 u 8 de ~15 para cada regla simple), porque depende de la estructura de su árbol; reproducirlo exigiría emular el
  R*-tree de libspatialindex. Efecto: 1 o 2 uniones de unos 670 trazados (670 contra 668 sin reflejo).
- `tests/tools/oracle_paint.py` genera `tests/golden_paint.h` (3,3 MB): ejecuta `Geometry.clear_polygon` (con `rtree` real) y
  `geometry_tool_gcode_gen` con orden `R` y tolerancia **originales** sobre el `Logo` del usuario (reflejado en X) y 8 casos
  sintéticos. Las trayectorias coinciden con el original vértice por vértice (< 1e-9) y el G-code con el orden `R`, la
  simplificación y la inversión de líneas coincide texto por texto. Entornos: `micromamba create -n old -c conda-forge
  python=3.10 shapely=1.8.4 numpy=1.22 rtree` (con GEOS 3.11) y `-n g310 ... geos=3.10.3` (mismo resultado).
  **Contra el `Logo.nc` real de la 8.994** (`PaintEndToEnd` en `test_paint.cpp`): 38 trazados contra 38, mismo orden y
  orientación; con GEOS 3.12: distancia máxima entre trazados 3,5 micras, 6.408 puntos contra 6.402 y 5 trazados que arrancan en
  el vértice vecino (hasta 0,05 mm); con GEOS 3.15.0: 5,0 micras, 6.404 puntos y 7 inicios distintos (hasta 0,098 mm). Umbrales del
  test: 10 micras, 0,2 mm y 1 % de los puntos. **No se puede igualar byte a byte** y la causa no es la versión de GEOS (3.10.3 y 3.11 dan lo
  mismo): la 8.994 decide distinto al simplificar. Hechos medidos: la tolerancia es `global_tolerance / 20 = 0,00025`; el
  proyecto trae el `Logo` reflejado en X; `contour` no se usa en `clear_polygon`; `rtree.nearest` devuelve los empates en un
  orden interno (el port usa el primero insertado). El criterio de aceptación para NCC es el mismo: cantidad, orden, orientación
  y micras, no texto idéntico.
- `tests/tools/oracle_cutout.py` genera `tests/golden_cutout.h`: ejecuta `CutOut.on_freeform_cutout` y `on_rectangular_cutout`
  **originales** (con una interfaz simulada) sobre `Edge_Cuts.gm1` y 38 casos, además de 3 de puente manual. Requiere
  Shapely 1.8.4 (con 2.x el original no itera las figuras múltiples). Comprueba contorno, puentes finos y agujeros
  vértice por vértice. Casos donde el original **falla** (y el port también): rectangular sin puentes, con margen
  negativo en varias figuras y con mouse bites en varias figuras. **Prueba de punta a punta** en `test_cutout.cpp`:
  `Edge_Cuts.gm1` -> CutOut rectangular (fresa 1,2, puentes arriba y abajo de 0,4) -> G-code == `DosCau_PPal-Edge_Cuts.nc`
  del usuario, byte por byte. El usuario usó el corte **rectangular** (esquinas de radio 0,6 sobre esquinas cuadradas); el
  libre redondea con el radio 0,05 del Gerber.
- `tests/tools/oracle_laser_lines.py` genera `tests/golden_laser_lines.h`: toma `ToolPaint.paint_polygon_worker` del fuente con
  `ast` y lo ejecuta (método "Líneas láser", con el `fill_with_lines` real) sobre Gerbers leídos por el parser original: uno con
  trazos, pads C/R/O y una región, uno con una macro, un trazo suelto y el Logo del usuario (36.870 trazados: solo totales).
  Coinciden vértice por vértice con GEOS 3.12; con 3.15 la Semilla de los pads une distinto (largo a menos del 1 %). **GEOS 3.11.0
  (la del oráculo) tiene un error**: el desplazamiento 0 de una línea de más de dos puntos da vacío (desde 3.12 da la línea), y
  `fill_with_lines` pierde la pasada central del trazo; el oráculo emula la GEOS corregida y el port se queda con la pasada central.
- `tests/tools/oracle_ncc_isolation.py` genera `tests/golden_ncc_isolation.h`: toma de `ToolNCC` con `ast` `get_tool_empty_area`,
  `generate_envelope`, `poly2rings`, `get_ncc_empty_area`, `calculate_bounding_box` y `apply_margin_to_bounding_box` y los ejecuta
  sobre una clase derivada del `Gerber` original (ToolNCC deriva de Gerber). 6 casos (una y dos herramientas de aislamiento,
  concordancia y convencional, agrandado del cobre, margen chico que rompe el aislamiento, y el Logo del usuario): el recorrido de
  cada herramienta y el área a limpiar coinciden con GEOS 3.12 (también el inicio de cada recorrido); con 3.15 algunos anillos
  empiezan en otro vértice y un anillo cortado por la caja sale como una sola línea en vez de dos: el test comprueba entonces la
  misma geometría y el mismo sentido (el primer tramo de cada parte aparece igual en el original).
- `tests/tools/oracle_milling.py` genera `tests/golden_milling.h`: toma `generate_milling_drills` y `generate_milling_slots`
  de `ExcellonObject` con `ast` y los ejecuta sobre el PTH del usuario y dos Excellon con ranuras (8 casos, incluidos la
  fresa igual al agujero, más grande y sin herramientas): mismos anillos vértice por vértice con GEOS 3.12 y 3.15.
- `tests/tools/oracle_panelize.py` genera `tests/golden_panelize.h`: ejecuta `Panelize.on_panelize` original (tomado con `ast`)
  con una interfaz simulada (8 casos): las copias, las aperturas, los agujeros y ranuras y la unión de los cortes coinciden.
  **Ojo con los oráculos de Excellon:** `create_geometry` copia `default_data` en cada herramienta; sin ese atributo la
  excepción corta el bucle y cada herramienta queda con una sola figura (así salían mal los límites del Excellon).
- `tests/tools/oracle_qrcode.py` genera `tests/golden_qrcode.h`: ejecuta la biblioteca `qrcode` 7.4.2 real (`python -m pip
  install qrcode==7.4.2` en `old`) y `execute`, `convert_svg_to_geo`, `make` y `export_svg_file` originales sobre el cobre de
  "luz de noche" (13 casos: modos numérico, alfanumérico y de bytes con trozos, UTF-8, "\n" final, versiones 1 a 21, los
  cuatro niveles, borde 0, polaridad negativa, esquinas redondeadas y un texto que no entra): la misma versión, los mismos
  módulos, el SVG exportado texto por texto, la geometría y el Gerber con el código (polígonos, aperturas y borrado).
- **Editores:** `tests/tools/oracle_geo_editor.py` (56 pasos encadenados sobre un `FlatCAMRTreeStorage` real: las
  herramientas de dibujo, `on_shape_complete` con fresado por ascenso y convencional, elegir con clic y área, unir,
  intersecar, restar, cortar, explotar, goma, pintar con los tres métodos, transformar y `update_fcgeometry`; exacto con
  GEOS 3.11, con 3.12/3.15 algunos anillos arrancan en otro vértice y el método de líneas pierde una línea),
  `oracle_exc_editor.py` (`edit_fcexcellon`, elegir con un clic, `ResizeEditorExc.make` y `update_fcexcellon` sobre el
  taladro de "luz de noche" y un Excellon con ranuras, más las figuras de agujeros, ranuras y grillas) y
  `oracle_grb_editor.py` (la carga de `edit_fcgerber` con la resta de los borrados, los pads C/R/O, los semidiscos y
  `new_edited_gerber` sobre los dos cobres del usuario).
- `tests/tools/oracle_thieving.py` genera `tests/golden_thieving.h`: ejecuta `on_add_copper_thieving_click`,
  `copper_thieving`, `on_mouse_release` (los rectángulos del área), `on_add_robber_bar_click`,
  `on_new_pattern_plating_object` y `flatten` originales **encadenados** sobre una misma herramienta (24 operaciones: el
  relleno, la barra y la lista aplanada pasan de una a otra), sobre el cobre y el logo de "luz de noche" y la máscara del
  dosificador: los cuatro rellenos, caja rectangular y mínima, área, referencia Gerber, Geometría y Excellon (falla), la
  barra nueva y en una apertura existente, la máscara con separación positiva, negativa y una que falla. **Con GEOS 3.11
  (la del oráculo) todo coincide exacto** (polígonos, vértices, áreas, aperturas en orden); con 3.12 y 3.15 cambian
  hasta 2 % de los vértices de los engrosados y, en el relleno con líneas, cuántos polígonos superpuestos une el engrosado
  de un `MultiPolygon` inválido (ahí el test compara las aperturas y lo agregado). `Shape::geosVersion` dice cuál corre.
- `tests/tools/oracle_rules.py` genera `tests/golden_rules.h`: ejecuta los `check_*` originales de `ToolRulesCheck` (con
  `ast`) sobre los archivos de las dos placas (12 casos: pistas, separación interna, entre Gerber, tamaño y separación de
  agujeros, anillo mínimo): las mismas violaciones como conjunto (el punto representativo difiere ~22 nm, tolerancia 1e-4).
- `tests/tools/oracle_solderpaste.py` genera `tests/golden_solderpaste.h`: `on_create_geo` y
  `generate_gcode_from_solderpaste_geo` originales con Paste_1 sobre la máscara del dosificador: los mismos trazos (como
  conjunto: cambia el orden de los pads) y el mismo G-code (con empates de `rtree`, como conjunto de líneas).
- `tests/tools/oracle_pcbwizard.py` genera `tests/golden_pcbwizard.h`: `load_inf`, `load_excellon`, `on_file_loaded` y
  `update_params` originales sobre `tests/data/pcbwizard/` (5 casos: pulgadas, métrico con CRLF y una herramienta repetida,
  ceros TZ, formato sin unidades, `.INF` sin herramientas) y el parser original sobre el texto armado: el texto idéntico y
  los mismos agujeros.
- `tests/tools/oracle_calibration.py` genera `tests/golden_calibration.h`: `gcode_header`, `generate_verification_gcode`,
  `calculate_factors`, `target_values_in_table`, `on_scale_button`, `on_skew_button` y `check_points` originales (7
  escenarios: los dos órdenes del segundo punto, Z cero con X,Y de cambio en pulgadas, factores con y sin lo medido,
  escalar e inclinar encadenados, una división por cero y 2 decimales): campos, puntos y G-code idénticos.
- `tests/tools/oracle_film.py` genera `tests/golden_film.h`: ejecuta `on_film_creation` original completo (con los
  `generate_*_film`, `export_negative` / `export_positive`, el lxml real y el diálogo, los hilos, `new_object` y
  svglib/reportlab simulados; ojo: `oracle_common` reemplaza `lxml` por un simulacro, hay que importarlo antes) sobre "luz
  de noche" y `tests/data/film/pads.gbr` (14 casos: negativa y positiva, escala, inclinación con cada referencia, espejo,
  PNG con otra densidad de pantalla, PDF, una Geometría como caja, sin caja, perforado con Excellon y con pads, y sus
  fallas). Con GEOS 3.11 los SVG grandes salen idénticos byte por byte y los chicos a menos de 1e-12 (el redondeo de
  `a*x + b*y + xoff`); con 3.12/3.15 cambia el vértice inicial de algunos anillos (se compara sin los trayectos).
- `tests/tools/oracle_autolevel.py` genera `tests/golden_autolevel.h`: `on_add_al_probepoints` (grilla), los dos
  métodos, `probing_gcode`, `import_height_map` y `on_grbl_jog` originales sobre líneas de corte sintéticas: grillas
  4x4 (MACH3), 3x2 en pulgadas (LinuxCNC), 1x3 bilineal (MACH4) y 2x2 con GRBL (sin G-code), puntos a mano (el primero
  queda sin celda) y el mapa de alturas: los mismos puntos, celdas (área y límites) y G-code; los comandos de avance,
  idénticos (con la tupla del original).
- `tests/tools/oracle_cnc_svg.py` genera `tests/golden_cnc_svg.h`: `CNCjob.gcode_parse`, `export_svg`, `bounds`, `size` y
  el cuerpo de `App.export_svg` originales sobre cuatro `.nc` del usuario abiertos como archivo (como `open_gcode`:
  `pp_solderpaste_name = None`; con otro valor el original lo interpreta todo como láser). Con GEOS 3.11 el SVG del port
  es idéntico (build de la GUI con esa GEOS: `/tmp/b311g`, como `/tmp/b311` pero sin `-DFLATCAM_BUILD_GUI=OFF` y con
  `-DCMAKE_PREFIX_PATH="$E;/opt/homebrew"`); con 3.12/3.15 la unión de líneas fusiona tramos colineales y el test compara
  el encabezado y el largo total de viajes y cortes.
- `tests/tools/oracle_image.py` genera `tests/golden_image.h` (necesita `rasterio`, que no está en `old`: entorno
  `raster`, `micromamba create -n raster -c conda-forge python=3.11 rasterio shapely numpy pillow`): arma imágenes chicas en
  `tests/data/images/` (gris con agujero y píxeles en diagonal, RGB, RGBA, con paleta, BMP) y ejecuta `import_image`
  original (con `ast`): las bandas que lee rasterio (el port las lee igual con `QImage`: índices en las de paleta, una
  banda en las grises) y el resultado (área, límites, polígonos y agujeros) idénticos.
- `tests/tools/oracle_pdf.py` genera `tests/golden_pdf.h` y los PDF de `tests/data/pdf/` (streams FlateDecode con colores,
  `cm` con `q`/`Q`, `m`, `l`, `h`, `c`, `v`, `y`, `re`, `w`, `S`, `f`, `B`, `W n` y rellenos blancos en dos páginas): ejecuta
  `parse_pdf`, `layer_rendering_as_gerber` y `layer_rendering_as_excellon` originales (con `ast`). Las capas, las aperturas
  (en orden, con su tamaño, elementos y áreas), el Gerber de cada capa y las herramientas del Excellon: idénticos, con GEOS
  3.11, 3.12 y 3.15. Ojo: un trazo de grosor 0 hace caer al original (ver la sección 14).
- `tests/tools/oracle_fiducials.py` genera `tests/golden_fiducials.h`: ejecuta `add_fiducials`, `add_fiducials_geo` y
  `add_soldermask_opening` originales sobre el cobre de "luz de noche" (11 casos: las tres formas, los tres segundos puntos,
  márgenes 0 y negativo, aperturas que ya existen, cruces superpuestas que fallan, la máscara): los mismos puntos, textos,
  polígonos, aperturas (con los elementos agregados vértice por vértice) y geometría de seguimiento.
- `tests/tools/oracle_align.py` genera `tests/golden_align.h`: ejecuta `on_mouse_click_release`, `check_points`,
  `align_translate` y `align_rotate` originales (con `ast`): qué puntos elige un clic sobre el cobre y el taladro de "luz de
  noche" (pads, trazos, afuera) y el desplazamiento y el giro de 10 juegos de puntos, también los que fallan: idénticos.
- `tests/tools/oracle_etch.py` genera `tests/golden_etch.h`: ejecuta `on_compensate` original (tomado con `ast`) sobre los
  cobres de "luz de noche" y del dosificador con los tres tipos (factor, grabador CuCl2 y Fe3Cl, a mano): los mismos polígonos
  tras la unión con las mismas áreas y las mismas aperturas (tamaño, ancho, alto, área), con GEOS 3.12 y 3.15.
- `tests/tools/oracle_safe_tooldia.py` genera `tests/golden_safe_tooldia.h`: ejecuta `ToolNCC.find_optim_mp` original (el
  diámetro seguro, igual en Isolation) sobre un Gerber sintético, uno de un solo polígono (el original avisa y no sigue), el Logo
  (0,0285) y `B_Cu` (0,1994) del usuario: idéntico.
- **Áreas de exclusión:** `tests/tools/oracle_exclusion.py` toma `ExclusionAreas.travel_coordinates`, `nearest_point` y
  `farthest_point` de `appCommon/Common.py` con `ast`; `oracle_geometry_modes.py` suma 5 escenarios (por encima, alrededor, dos
  áreas, profundidad múltiple, sin cruce) y `oracle_drill.py` 3 corridas del taladro del usuario con áreas: idénticos.
- `tests/tools/oracle_isolation.py` genera `tests/golden_isolation.h` (0,7 MB): **no transcribe nada**; toma del fuente original con
  `ast` los métodos `isolate`, `combined_normal`, `combined_rest`, `generate_envelope`, `generate_rest_geometry`, `area_subtraction`,
  `area_intersection` y `poly2rings` de `ToolIsolation` y los ejecuta con una interfaz simulada (tabla, casillas, `new_object`), con el
  `isolation_geometry` de `camlib`. 27 escenarios sintéticos (exterior/interior/completo, concordancia/convencional, varias pasadas,
  pasadas separadas, varias herramientas, resta de taladros, recorte a un área, seguimiento de líneas, resto en los tres órdenes, resto
  forzado y sin forzar, selección por polígono, diámetro negativo y objeto de referencia): coinciden figura por figura, con el mismo
  sentido y los mismos vértices, con GEOS 3.12 y 3.15. Además, `B_Cu` real (fresa 0,1 por defecto, y resto con 0,4/0,2/0,1): con GEOS
  3.11 coincide vértice por vértice; con 3.12 y 3.15 el buffer de 16 segmentos del resto da hasta 0,35 % menos vértices con la misma
  longitud a menos de una micra (el test admite 0,5 % de puntos y 1 micra de longitud), y GEOS 3.15 devuelve los 179 polígonos del
  Gerber en otro orden (con 3.11 y 3.12 el orden es idéntico al del original: el parser está bien).
- **Contra el ejecutable FlatCAM 2024.4 del usuario** (`tests/test_luz_de_noche.cpp`, datos en `tests/data/luz_de_noche/`, parámetros
  en `luz_de_noche.params.json` y en las capturas descritas en `docs/fase4_gui_referencia.md`). En esa placa el cobre, el logo y el
  taladro también están **reflejados en X**. Resultados (con la GEOS que se indica):
  - CutOut libre del contorno (fresa 1,2, 4 puentes de 0,6) → G-code **idéntico byte por byte** (GEOS 3.12 y 3.15). Hizo falta la
    tolerancia de simplificación `global_tolerance / 20 = 0,00025` también en el contorno: sin ella quedan 16 vértices de más por
    esquina (el fuente da los mismos 36 puntos por tramo que el port; los descarta la simplificación del CNC Job).
  - Paint del Logo → 72 trazados y 13.553 puntos iguales; con GEOS 3.11 el archivo es idéntico, con 3.12 y 3.15 cambian 17 líneas
    en las últimas cifras (menos de 1 micra).
  - NCC de `B_Cu` → con GEOS 3.11 y 3.12, **272 trazados como el real** y 261.093 puntos contra 261.089; las únicas diferencias son
    empates exactos de `rtree.nearest`. Con 3.15, 275 trazados (0,07 % menos puntos). El archivo real está editado a mano por el
    usuario (`G00 Z0`, `G21f`, sin `G00 Z15.00` final) y coincide con la variante 8.994.
  - Taladro → los mismos 120 agujeros con las mismas profundidades y las mismas líneas fijas; el orden es el de OR-Tools en el real
    y el del optimizador propio en el port (828,6 mm de recorrido contra 1.323,1).
  - **El ejecutable 2024.4 no es idéntico al fuente de referencia**: emite el Z inicial (línea vacía) antes del `M5`, como la 8.994.
    Se agregó `GcodeFlavor::FlatCAM2024_4Release` para reproducirlo; `FlatCAM2024_4` sigue siendo el fuente (lo que ejecutan los
    oráculos).
  - Con Shapely 2 el CutOut original no corre, así que el ejecutable usa casi seguro Shapely 1.8; que coincida mejor con GEOS 3.11
    sugiere que trae esa GEOS (no está comprobado: no se puede ver dentro del ejecutable desde acá).
- `tests/tools/oracle_geometry_multitool.py` genera `tests/golden_geometry_multitool.h`: una Geometría con varias herramientas
  en un mismo CNC Job (`mtool_gen_cncjob` multigeo: `geometry_tool_gcode_gen` con `is_first`/`is_last`, `toolchange=True`), con el
  corte con puentes finos (T1 + T9999), dos herramientas con parámetros distintos y tres de láser. Coinciden texto por texto.
- `tests/tools/oracle_geometry_modes.py` genera `tests/golden_geometry_modes.h`: 26 escenarios sintéticos (profundidad múltiple,
  corte extra, puntos, tolerancia, segmentación, láser, sin cambio de herramienta, pulgadas) ejecutados con
  `geometry_tool_gcode_gen` **original**; los tests comparan el texto completo. Los desplazamientos de herramienta
  (`in`/`out`/`custom`) sí se comparan en texto (7 escenarios): el orden de vértices del buffer coincide entre GEOS 3.11.0,
  3.12.1 y 3.14.1. La cabecera imprime los límites de la geometría de origen, no los del recorrido desplazado.
- **Fase 3:** comparar el G-code generado contra los `.nc` de `tests/data/` (ver sección 8).
- Cuando se porte una función de `camlib.py` o de las herramientas, leer el original **completo** y respetar sus
  detalles numéricos (redondeos, tolerancias, orden de operaciones): el G-code debe coincidir.

## 8. Datos de prueba reales (`tests/data/`)

Proyecto real del usuario (placa de un dosificador caudalímetro, KiCad 6.0.6): ver `tests/data/PARAMETROS.md`
(parámetros extraídos de los encabezados de los `.nc`).

| Archivo | Qué es |
|---|---|
| `PCB principal-B_Cu.gbl` | Cobre inferior (15.052 líneas): macros de apertura (HorizOval, RotRect...), regiones G36/G37, flashes D03 |
| `PCB principal-B_Logo.gbr` | Logo, solo trazos con apertura circular de 0,15 mm |
| `PCB principal-Edge_Cuts.gm1` | Contorno: rectángulo ±37,0 × ±38,5, trazo 0,1 mm |
| `PCB principal-B_Mask.gbs` | Máscara negativa (flashes) |
| `PCB principal-F_Silkscreen.gto` | Serigrafía con arcos G75/G03 |
| `PCB principal-Drill.drl` | Excellon KiCad (métrico, absoluto, decimal): 4 herramientas, 192 agujeros |
| `DosCau_PPal-*.nc` | G-code generado por FlatCAM: B_Cu (NCC láser), Logo (Paint láser), Edge_Cuts (CutOut), Drill, Alinea |
| `luz_de_noche/` | Otra placa del usuario (KiCad 10): Gerber `Edge_Cuts`, `B_Cu`, `Logo`, `PTH.drl`/`NPTH.drl` y los 4 `.nc` que generó su **FlatCAM 2024.4** (CutOut, Perforación, Paint, NCC); `luz_de_noche.params.json` extraído de su `.FlatPrj`. Subido al repo con permiso del usuario |

Hallazgos que condicionan el trabajo:

- Los Gerber son de KiCad 6: `FSLAX46Y46` (4.6, ceros iniciales omitidos), `MOMM`, polaridad `LPD`, atributos X2
  (`%TF`, `%TA`, `%TO`, `%TD`), arcos multicuadrante (`G75`, `G03`), regiones y macros de apertura.
- Postprocesadores usados: **`GRBL_laser`** (B_Cu y Logo: avance 500, rápidos 1500, Z Focus 2,0, potencia 300, sin
  movimientos en Z) y **`default`** (Mach3; Drill, Alinea, Edge_Cuts).
- Taladros reflejados en X (el `.drl` trae `X-26.239` y el G-code `X26.2390`).
- `Alinea` no tiene `.drl`: lo genera la herramienta "PCB a 2 caras" (`on_create_alignment_holes`): el usuario tipea los agujeros y la
  herramienta agrega el reflejo de cada uno respecto de un eje que pasa por un punto (`affinity.scale`, fórmula exacta
  `factor * x + (pivote - pivote * factor)`). Con `(-38, 39.5)` y `(38, 39.5)`, eje X por el origen, salen los 4 agujeros de las esquinas.
  **El `Alinea.nc` real no se reproduce byte a byte, por dos diferencias que vienen de otro camino de código de la 8.994** (el de la
  pestaña del propio objeto Excellon, que ya no existe en la 2024.4): (a) deja una línea en blanco después de cada agujero y (b)
  taladra primero en el origen (0, 0), aunque el objeto cuenta 4 agujeros ("Total drills = 4"). Sin esas dos cosas el port da
  exactamente tu archivo (orden de los agujeros, cabecera y rangos). **Resuelto:** el usuario agregó ese agujero central
  a mano, con el editor de G-code de FlatCAM, después de generar el archivo; el port no tiene que generarlo.
- **Los `.nc` los generó FlatCAM v8.994 (2020/11/7), no la 2024.4** cuyos fuentes tenemos. Puede haber diferencias.
  Se le pidió al usuario re-exportar con la 2024.4 a `tests/data/2024_4/`. Si no hay diferencias, usar cualquiera.
- **Parámetros confirmados** (leídos del `.FlatPrj` del dosificador, que también guardó la 8.994; ver
  `tests/data/PARAMETROS.md` y `tests/data/dosificador_PCB_principal.params.json`): NCC y Paint con herramienta de
  0,1 mm, superposición 80 %, método estándar, margen 0, conectar y contorno, `GRBL_laser`, avance 500, potencia
  300; CutOut con fresa 1,2, Z -4,5, avance 100, **puente de 0,4 mm** (hueco de 1,6 mm en el recorrido), 2 puentes
  arriba y abajo; taladros con Z -3,5 (T4 de 3,4 mm: -4,5) y avance Z 100; Alinea con fresa 0,4 y avance 50.
- Reflejo verificado: el logo y los 192 agujeros están reflejados con `x → -x` respecto de X = 0. El cobre `B_Cu`
  es simétrico en sus límites, hay que confirmarlo con la geometría.
- El `.FlatPrj` es JSON, comprimido con LZMA si está activa la opción de guardar comprimido (el valor por defecto).
  Los del dosificador pesan ~1 GB y el entorno del chat no puede leer el disco del usuario, por eso los
  parámetros se extrajeron en su Mac con `tools/extract_flatprj_params.py` (biblioteca estándar, streaming,
  filtra las opciones globales por privacidad; test en `tests/tools/test_extract_flatprj_params.py`). El proyecto
  que corresponde a los `.nc` es `dosificador caudalimetro - PCB principal.FlatPrj`.
- Las capturas de FlatCAM 2024.4 que pasó el usuario eran del proyecto *luz de noche Pascual*; solo sirven para ver
  la interfaz y los valores habituales (los de NCC y Paint coinciden con los del dosificador).
- `DosCau_PPal-B_Cu.nc` pesa 15,7 MB (502.025 líneas): sirve también de prueba de rendimiento.

## 9. Flujo de entrega y Git

- Rama `main`. Mensajes de commit **en español**, con el prefijo de la fase: `Fase 2: parser de Gerber`.
- **No usar `git push --force`**. No modificar `reference/`.
- **Desde Claude Code o una terminal con acceso al repo:** commitear y hacer `push` directamente.
- **Desde el chat de claude.ai:** el entorno clona el repo público por HTTPS (puede leer GitHub, no puede escribir
  ni llegar a GitLab). Entonces: clonar `https://github.com/dnasta/flatcam-cpp`, trabajar con commits ordenados,
  compilar y correr los tests, generar `git format-patch origin/main --stdout > archivo.patch`, **verificar que el
  parche aplica limpio sobre un clon nuevo** y entregarlo. El usuario lo aplica con
  `git am ~/Downloads/archivo.patch` y hace `git push`.
- Los archivos del entorno del chat **no persisten** entre conversaciones: el estado real es lo que está en GitHub.
  Al empezar una sesión, clonar de nuevo y leer este archivo.
- Al cerrar cada tanda: actualizar la tabla de la sección 4, los "Pendientes" de la sección 10 y, si cambió algo,
  `README.md`.
- **Desde que el usuario pasó a Claude Code (CLI) en su Mac**: se commitea y se hace `push` directo, sin `.patch`. El flujo
  de parches de arriba era solo para el chat de claude.ai y queda como referencia.
- Claude Code lee `CLAUDE.md`, no `AGENTS.md`; por eso `CLAUDE.md` solo importa a este archivo con `@AGENTS.md`.

## 10. Fase 2 hecha y siguiente paso: fase 3

> **Sección histórica.** Describe el plan con que se arrancó la fase 3. El estado vigente está en las secciones 4, 11 y 12.

### Lo que quedó de la fase 2 (`src/parsers/`)

| Clase | Origen en Python | Notas |
|---|---|---|
| `ApertureMacro` | `camlib.py` (clase en la línea ~75) | 8 primitivas y evaluador propio de expresiones (variables solo `$` + dígitos) |
| `Gerber` | `appParsers/ParseGerber.py` | `parse_lines` portado con el mismo orden de comprobaciones; guarda la geometría de cada apertura |
| `Excellon` | `appParsers/ParseExcellon.py` | incluye `scale`, `offset`, `mirror`, `rotate`, `skew` y `convertUnits` de los taladros |

Decisiones y desvíos (también documentados en los encabezados):

- Se reproducen comportamientos raros del original: `parse_number` con `"L"` divide por `10^(dígitos - enteros)`; en un
  Excellon sin cabecera, volver a elegir una herramienta pisa su diámetro con uno inventado (en pulgadas, porque
  compara `excellon_units` con `"MM"` cuando vale `"METRIC"`); los trazos se engrosan con `ancho / 1.999`.
- `extra_buffering` se aplica polígono por polígono (como Shapely 1.8). La lista `solidGeometry()` puede traer
  polígonos vacíos (parte que desaparece al achicar 1e-7); los tests los descartan, igual que el oráculo.
- `Shape::unite` y `difference` devuelven directo el otro operando cuando uno está vacío (con GEOS 3.12 aparecían
  miembros vacíos que el original no tiene).
- Los parsers usan `std::regex` (más lento con `libc++` que con `libstdc++`); si en macOS el parseo de `B_Cu` (15.000
  líneas) tarda varios segundos, agregar un despacho por primer carácter.

### Transformaciones de `Gerber` (hechas al empezar la fase 3)

`scale`, `offset`, `mirror`, `rotate`, `skew`, `buffer` y `getBoundingBox` mueven la geometría sólida, la de seguimiento y la
de cada apertura, como el original. `convert_units` y `bounds` no hacen falta: los de `Geometry` ya hacen lo mismo.
Desvíos: `scale` no multiplica `nVertices` de las aperturas P (el original lo dejaba como número no entero); en `buffer`
con factor, las aperturas de tipo no soportado quedan sin geometría sólida en vez de abortar. `buffer` se aplica
polígono por polígono (Shapely 1.8), igual que el original; el golden sale de Shapely 1.8.4, así que la cantidad de
polígonos se compara exacta (el área y los límites sobre la unión).

### Fase 3: qué portar (todo en `reference/FlatCAM_2024_4/FlatCAM/`)

| Qué | Origen en Python | Líneas |
|---|---|---|
| Generación de G-code (`CNCjob`) | `camlib.py` (clase desde la línea ~2619) | ~5.400 |
| Postprocesadores `default` y `GRBL_laser` | `preprocessors/default.py`, `GRBL_laser.py`, `appPreProcessor.py` | 253 / 110 / 164 |
| Objetos Geometry y CNCJob | `appObjects/FlatCAMGeometry.py`, `FlatCAMCNCJob.py` | 3.310 / 2.594 |
| NCC | `appTools/ToolNCC.py` | 4.607 |
| Paint | `appTools/ToolPaint.py` | 3.261 |
| Isolation | `appTools/ToolIsolation.py` | 3.604 |
| Perforación | `appTools/ToolDrilling.py` | 2.685 |
| CutOut | `appTools/ToolCutOut.py` | 2.465 |
| PCB a 2 caras | `appTools/ToolDblSided.py` | 975 |

Orden sugerido: (1) métodos de `Gerber` (hecho); (1b) postprocesadores `default` y `GRBL_laser` (hecho, `src/cam/`); (2a) taladrado con `CncJob` (hecho); (2b) G-code de geometrías para un trazado de una herramienta (hecho, con profundidad
múltiple decimal exacta, corte extra, puntos, tolerancia y segmentación); (2c) creación de la geometría del CutOut (puentes) desde el Gerber del contorno; (2) resto de `CNCjob` + postprocesador `default` y probarlo con el taladro
(`DosCau_PPal-Drill.nc`, el más simple), luego `Alinea` y `Edge_Cuts` (CutOut); (3) `GRBL_laser` + NCC y Paint
con `B_Cu` y `Logo`. Parámetros exactos en `tests/data/PARAMETROS.md`. El G-code de referencia viene de la 8.994:
confirmar si la 2024.4 da lo mismo.

Fases 3 a 5: ver la tabla de la sección 4. Dependencias de Python a reemplazar: Shapely→GEOS (hecho), numpy→Eigen
o `std::vector`, vispy/OpenGL→`QOpenGLWidget`/`QGraphicsView`, ezdxf→libdxfrw, rtree→Boost.Geometry,
lxml→pugixml o `QXmlStreamReader`, svglib→parser SVG o QtSvg, reportlab→`QPdfWriter`, ortools→OR-Tools C++,
simplejson→nlohmann/json, qrcode→port propio de la biblioteca (`src/tools/QrCode.*`). `winreg` y `tkinter` no se portan.

### Plan para NCC y Paint (lo que falta de la fase 3)

Datos del usuario (`tests/data/PARAMETROS.md`): herramienta de 0,1 mm, método estándar, solape 0,8 (paso 0,02 mm), margen 0,
conectar sí, contorno sí, láser `GRBL_laser`. Los `.nc` de referencia traen 723 trazados y 499.094 puntos (`B_Cu`) y 38 trazados
y 6.402 puntos (`Logo`); esos números ya se verificaron al reproducir los archivos desde sus trazados.

Algoritmo que hay que portar, en este orden (`reference/FlatCAM_2024_4/FlatCAM/`):
1. `FlatCAMRTree` / `FlatCAMRTreeStorage` (`camlib.py` 8538): índice de los extremos de cada línea, con `nearest`, `insert` y
   `remove`. El original usa `rtree.Index.nearest`, que ante puntos **exactamente equidistantes** devuelve todos en un orden
   interno de libspatialindex (con 4 puntos equidistantes dio los ids 0, 2, 1, 3, no por índice). El port puede usar una grilla
   o un k-d con menor índice como desempate: solo cambia el resultado con empates exactos.
2. `Geometry.paint_connect` (`camlib.py` 1967): une anillos vecinos si el tramo de unión cabe dentro del polígono
   (`walk_cut.within(boundary)`) y mide menos de `10 * tooldia`.
3. `Geometry.clear_polygon` (`camlib.py` 1407): `buffer(-tooldia / 1.999999)` y luego `buffer(-tooldia * (1 - overlap))`
   repetido hasta que el área sea cero; cada anillo va al almacén; después `paint_connect` si `connect`.
4. El flujo de la herramienta: `ToolPaint` (3261 líneas) para `Logo` (empezar por acá, es chico) y `ToolNCC` (4607 líneas;
   `clear_polygon_worker` en la línea 2053) para `B_Cu`, incluyendo el contorno, la validez y el margen.

Oráculo: hay que correr el original con el paquete `rtree` **real**; `tests/tools/oracle_common.py` lo reemplaza por un simulacro
vacío. En el entorno de Shapely 1.8.4: `micromamba install -n old -c conda-forge rtree` (rtree 1.4.1, probado).
Criterio de aceptación (corregido): el G-code que sale de `B_Cu.gbl` y `B_Logo.gbr` coincide con `DosCau_PPal-B_Cu.nc` y `-Logo.nc` en
cantidad de trazados, orden, orientación y distancia de micras (ver la sección 7); la igualdad byte a byte no es alcanzable.

## 11. Pendientes y preguntas abiertas

1. **¿La 2024.4 genera el mismo G-code que la 8.994?** **Resuelto** con los `.nc` de la 2024.4 de la placa "luz de noche" (sección 7):
   el port reproduce la 2024.4 (CutOut byte por byte, Paint y NCC salvo GEOS y empates). Además, el **ejecutable** 2024.4 difiere del
   fuente en el orden de dos líneas (variante `FlatCAM2024_4Release`). Lo que sigue es el análisis original contra la 8.994.
   Respuesta: casi, y las diferencias están medidas. Con el postprocesador
   `GRBL_laser` y en un trabajo de geometría sin cambio de herramienta la 8.994 (a) agrega `G00 Z0` en la cabecera, (b) emite
   el Z inicial *antes* de subir el láser (`M5`) y (c) cierra sin `G00 Z19.00`. El código lo cubre con `GcodeFlavor`
   (`FlatCAM8_994` **por defecto**, por decisión del usuario: "prefiero G00 Z0"; `FlatCAM2024_4` sigue disponible y es la que
   usan los tests contra el código original). El postprocesador `default` es idéntico en las dos. **Evidencia de lo que
   coincide:** el taladro (todo
   salvo el orden de los agujeros) y el contorno (byte por byte, desde el Gerber hasta el G-code pasando por el CutOut
   rectangular) coinciden con el generador de la 2024.4. **NCC, Paint y Alinea ya se midieron** (secciones 7 y 11.3): coinciden en geometría con
   diferencias de micras; la 8.994 une unos 23 tramos más en `B_Cu` y `Alinea` salió de otro camino de código. El usuario puede
   exportar de nuevo desde la 2024.4 a `tests/data/2024_4/` para cerrarlo del todo.
2. ~~Reflejo de `B_Cu`~~ **Resuelto**: está reflejado en X (`mirror(Axis::Y, {0, 0})`), igual que el Logo y los 192 agujeros; la envolvente
   convexa del cobre reflejado da exactamente los rangos del encabezado de `B_Cu.nc`.
3. ~~**Taladro `(0, 0)` en `Alinea`**~~ **Resuelto (respuesta del usuario):** "ese agujero central lo creé después de generar el
   G-code, con el editor de G-code de FlatCAM". No se agrega nada al port. Lo que sigue es el análisis previo, como referencia.
   (El G-code taladra 5 posiciones, el job cuenta 4.) Leído `ToolDblSided.py`: la herramienta solo
   crea los agujeros pedidos y sus reflejos, así que el (0, 0) no sale de ahí ni de OR-Tools (el `Drill.nc`, que usa la misma
   optimización `B`, no taladra en el origen). Probablemente es un agujero que el objeto Excellon de alineación tenía en su lista o
   una rareza del camino viejo de la 8.994. Para saberlo: `python3 tools/extract_flatprj_params.py proyecto.FlatPrj salida.json
   --con-agujeros` y mirar `drills` de `PCB ppal-Alinea.drl`; y preguntar si el usuario quiere ese agujero en el centro de la placa.
   El port NO lo taladra.
4. ~~Confirmar si Isolation entra en el alcance~~ **Resuelto:** entra y ya está portado (`src/tools/Isolation.*`). Falta solo la
   verificación de diámetro seguro (`tools_iso_check_valid`, que solo informa) y la selección interactiva, que va en la GUI.
5. **Orden de los taladros (decidido).** El `.nc` de referencia lo ordenó OR-Tools (`excellon_optimization_type = B`). El usuario
   eligió un **optimizador propio** (`PathOptimizer`: 2-opt desde el vecino más cercano y desde el orden del archivo, el
   más corto gana). Los tests comparan el G-code como conjunto (mismos agujeros, profundidades y líneas fijas) y exigen
   que el recorrido no supere en más de 10 % al de OR-Tools. OR-Tools queda como posible opción futura.
6. ~~Elegir el canvas~~ **Decidido por el usuario: `QOpenGLWidget`** (GPU, buffers de vértices; tiene que mover los 500.000 puntos de
   `B_Cu.nc` con fluidez). En macOS OpenGL está deprecado pero funciona; si algún día molesta, la alternativa es QRhi.
7. **Resuelto en lo que importa:** con la 2024.4 el NCC del port da los mismos trazados que el real (272 contra 272, placa "luz de
   noche"), así que la diferencia del dosificador es de la 8.994, no del port. Análisis original: **`B_Cu`: 746 trazados contra 723** del `.nc` real (puntos 0,05 %, longitud 0,015 % de diferencia). La 8.994 une unos 23 tramos más
   (~0,45 mm cada uno). No se pudo deducir la regla (probado: radio y largo máximo de la unión, resolución del engrosado de 4 a 64).
   Candidatas sin probar: otro criterio en `paint_connect` de la 8.994, o un `rtree` distinto. Si el usuario exporta desde la 2024.4 se
   podría saber si el cambio es de versión.
8. **Empates de `rtree.nearest`**: ~15 empates exactos en 5.509 consultas del NCC; el `rtree` real elige a veces la línea insertada
   antes y a veces la de después (depende de la estructura de su árbol). `EndpointStorage` elige la primera insertada. Cambia 1 o 2
   uniones de ~670. Emularlo exigiría portar el R*-tree de libspatialindex: no vale la pena salvo que el usuario lo pida.
9. **Métodos que faltan en Paint y NCC**: ~~semilla, líneas, combinado, mecanizado de resto, selección por área y por objeto de
   referencia, "Líneas láser" de Paint, operación "Aislamiento" de NCC (`isotooldia`)~~ (hechos). En "Aislamiento", como el
   original: hace falta al menos una herramienta de "Limpiar"; el área a limpiar excluye la envolvente de la **última**
   herramienta de aislamiento; el tipo de fresado es el que muestra el formulario (no el de cada herramienta); con un margen
   menor que la herramienta avisa que el aislamiento está roto. **Error del port corregido:** al cambiar un campo de Paint, NCC
   o Isolation se copiaban todos los campos del formulario a las herramientas elegidas (también la operación); el original solo
   copia el que cambió (`form_to_storage` con `sender()`).
   **Rarezas de "Líneas láser" que se reproducen:** las aperturas de tamaño 0 (regiones) se agregan a los trazos de **todos** los
   polígonos y la unión vacía de sus líneas hace fallar a `fill_with_lines` (IndexError): en un Gerber con regiones solo se pintan
   los pads (ya estaban en el almacén cuando falla); una macro (sin `size`) hace fallar todo el polígono; si la unión de las
   líneas de los trazos es una sola línea, `linemerge` falla y quedan solo los pads. Las aperturas se recorren en el orden del
   diccionario del original (`Gerber::apertureOrder`: el del archivo; el alfabético en un objeto leído de un proyecto). Con una
   Geometría el método se deshabilita en el combo y, si estaba elegido, pasa a Combo, como el original. El usuario solo
   usa el método estándar, "Él mismo" y una herramienta.
10. **Variante del G-code**: por defecto `GcodeFlavor::FlatCAM8_994` (decisión del usuario, y es lo que el usuario hace a mano con
    los archivos de láser de la 2024.4). Hay tres: `FlatCAM2024_4` (fuente), `FlatCAM2024_4Release` (ejecutable) y `FlatCAM8_994`. El `Alinea.nc` de la 8.994 tiene además
    líneas en blanco tras cada agujero (el port no las reproduce) y el taladro en (0, 0), que agregó el usuario a mano (ver 3).
11. **Isolation: desvíos conscientes** (documentados en `Isolation.h`): con pasadas separadas y un único polígono el original falla al
    iterar y el port lo devuelve; en el resto, si un buffer da varias partes (solo con diámetro negativo) el port las conserva.
12. **GUI: desvíos conscientes.** (a) Estilo **Fusion** por defecto en todas las plataformas (el original usa el nativo, que en el
    Windows del usuario es `windowsvista`; en macOS el nativo se ve muy distinto: íconos de 32 px y pestañas centradas). Con
    `-style macos` se usa el nativo. (b) En macOS la barra de menús es la del sistema, como en cualquier aplicación Qt (también la
    de Python). (c) `FCDetachableTab` desprende con doble clic a una ventana propia; falta arrastrar y soltar entre ventanas.
    (d) El canvas dibuja la grilla con dos escalas (gruesa y fina) en vez del shader de vispy, con los mismos colores.
    (f) Los archivos se leen en el hilo de la interfaz (el original usa hilos de trabajo): el Gerber más grande tarda 0,6 s.
    (g) La GUI compila con `QT_NO_KEYWORDS` (`Q_SIGNALS`, `Q_SLOTS`, `Q_EMIT`): con las palabras clave de Qt, `slots` es una macro
    y el campo `ExcellonTool::slots` del núcleo desaparecía en la GUI (estructura de otro tamaño: riesgo de caída).
    (e) **Traducción corregida a pedido del usuario:** `tools/gui_dump/es_corrections.json` se aplica sobre `translate/es.bin`
    ("Open" es "Abrir", no "Abierto"; también "Abrir proyecto", "Abrir Excellon", "Abrir código G" y los mensajes de error al
    abrir). Para corregir otro texto: agregarlo ahí y regenerar (`extract_resources.py`, `dump_tree.py`, `gen_cpp.py`).
    (h) **Agregado pedido por el usuario (no existe en el original, donde está comentado):** en la pestaña Proyecto los objetos
    se reordenan con arrastrar y soltar, solo dentro de su grupo (`ObjectCollection::moveObject`, `mimeData`/`dropMimeData`), y
    el orden del árbol es el orden de dibujo del canvas (`ObjectCollection::drawOrder` → `PlotCanvas::setLayerOrder`): dentro de
    un grupo, el primero queda arriba (Silkscreen primero → se ve sobre B_Cu); entre grupos, los posteriores (Excellon,
    Geometría, CNC Job) quedan sobre los anteriores, porque se generan a partir de ellos. Soltar archivos sigue abriéndolos.
    Test: `GuiApp.ReorderingTheProjectTreeChangesTheDrawOrder` (incluye un soltado con eventos de mouse sobre la vista).
    (i) El CNC Job dibuja cortes y viajes engrosados con el radio de la herramienta (como `plot2`: cortes azules, viajes
    amarillos translúcidos encima) con 16 segmentos por cuarto de círculo en vez de 64. Solo visual.
    (j) ~~Una herramienta por vez, color de los puentes finos, mouse bites~~: resuelto. Varias herramientas van en un mismo
    CNC Job (`CncJob::generateFromGeometryTools`, verificado contra el original con `oracle_geometry_multitool.py`); cada
    herramienta con `override_color` se dibuja con el suyo (`PlotGroup`); los "mouse bites" crean `<objeto>_mouse_bites`.
    (k) Dos errores del original que se reproducen a propósito: `geometry_plot_line` y `get_fg_blue()` usan `is_theme_black`
    sin llamarla, así que la geometría siempre se dibuja en `#FFAAAA` y el rótulo "Parámetros para" va en `#7777FF` (en
    Paint el color está fijo en el código: `#0000FF`). Paint agrega sus herramientas con forma "V" (así está en
    `on_tool_default_add`), y al abrir la Geometría la Z de corte se recalcula con la punta en V (`update_cutz`), como el original.
    (l) Paint y NCC: solo el método estándar, la selección "Todos" / "Sí mismo", "Limpiar" y sin mecanizado de resto; sin la
    base de datos de herramientas (agregar usa siempre los valores por defecto) ni la verificación del diámetro seguro
    (solo informa en el original). Se calculan en el hilo de la interfaz con el indicador "Ocupado" (el NCC de B_Cu: 9 s).
    (n) **Ajuste a la grilla** (como `App.grid_status` + `AppGeoEditor.snap` con solo la grilla; el ajuste a esquinas es del
    editor, que no está): con "Ajustar a la grilla" activo (de fábrica, grilla 1,0 enlazada) las posiciones de clic, de
    movimiento (barra de estado, HUD, cruz) y de las zonas elegidas salen ajustadas con `round(x / g) * g` (al par en los
    empates); el zoom y el paneo no. Error del original que se reproduce: "Elegir agujero" de PCB a 2 caras apaga la grilla y no la
    vuelve a encender.
    (o) **Áreas de exclusión:** un solo almacén en la aplicación, como `app.exc_areas`; las usan **todos** los CNC Job aunque se
    hayan agregado desde otro panel y aunque la casilla esté apagada (la casilla solo muestra la sección). La estrategia y la Z
    se leen del panel al terminar de dibujar (el original las lee en cada zona). Un tramo del viaje que corre sobre el contorno
    de un área hace fallar al original (itera un `LineString`); el port lo ignora. **Error del volcador corregido:** guardaba la
    propiedad `enabled` efectiva de los widgets (falsa si el padre estaba deshabilitado: la lista de exclusión decía
    `"enabled_"`), y en Perforación los parámetros quedaban deshabilitados para siempre aunque el marco se habilitara; ahora
    solo se guarda el `enabled` propio (en acciones y menús, la propiedad).
    (p) **Distancia, Mover y Propiedades** (fase 5, ítem 3). Distancia desconecta los manejadores del mouse de la aplicación
    (`MainGUI::setCanvasHandlersEnabled`) y pone Dx = Dy = 0 en el HUD mientras mide, como el original; "Ajustar al centro"
    solo existe en los editores (queda oculto); `grid_status_memory` nunca vuelve a `False` (se reproduce). El mensaje del
    resultado usa `'%*f'` (ancho 4, seis decimales), como el original. Distancia mínima: desvío, el original convierte la
    lista de figuras del segundo objeto solo si la del primero es una lista (si no, falla); el port convierte las dos. Un CNC
    Job no tiene `solid_geometry` en el port: no se puede medir. Mover: desvío, el original "mueve" un CNC Job con su
    `offset`, que solo mueve el dibujo de los de Geometría (el G-code exportado queda igual) y nada en los de Perforación; el
    port no lo mueve; además el original regenera el texto fuente del Gerber/Excellon movido (`export_gerber`), que el port
    todavía no tiene (ítem 4). Propiedades: las opciones salen del diccionario con que se guarda el objeto (`to_dict`), en su
    orden (el del original es el de inserción en `options`), escritas como `str()` de Python; las cuentas de `job_thread` se
    hacen en el mismo hilo. "Propiedades" del menú del Proyecto abre ahora esta herramienta (antes mostraba la pestaña).
    Escala y desplazamiento del panel de cada objeto: como `on_scale_button_click` (desde el origen) y
    `on_offset_button_click`; en un CNC Job no hacen nada (en el original cambian el texto de un G-code que no se exporta).
    (q) **Distancia recorrida y tiempo estimado del CNC Job** (`travel_distance`, `routing_time`): faltaban y se ven en las
    Propiedades del CNC Job y en la herramienta Propiedades. `CncJob::travelDistance` / `routingTime` hacen las mismas
    cuentas que `excellon_tool_gcode_gen` + `ToolDrilling` (bajadas, subidas, viajes; avances de la última herramienta) y que
    `geometry_tool_gcode_gen` (cortes por la cantidad de pasadas, viajes y la vuelta a (0, 0)); verificado contra el original
    con `oracle_drill.py`, `oracle_geometry_modes.py` (71 escenarios) y `oracle_geometry_multitool.py` (se acumulan entre
    herramientas). Rareza que se reproduce: con profundidad múltiple el original deja la línea invertida o no según la cantidad
    de pasadas, y la vuelta a (0, 0) sale de ese extremo. `travel_distance` no se guarda en los proyectos: un CNC Job abierto
    de un proyecto no lo muestra (igual que el original). Arreglado de paso: `oracle_geometry_multitool.py` no corría (importaba
    `ExcAreas`, que se había movido a `oracle_exclusion.py`).
    (r) **Selección en el canvas y menús Ver/Editar** (fase 5, 3b). Como el original: la selección por área compara las
    **cajas** de los objetos (no su geometría); un clic sobre el objeto ya elegido quita la selección (y aun así avisa que está
    elegido); con varios objetos bajo el clic, cada clic pasa al siguiente. Las Propiedades siguen ahora al cambio de selección
    (`selectionChanged`, como `on_list_selection_change`) y el objeto activo es el primero elegido (`get_active`), no el índice
    actual del árbol. Desvíos: los submenús de los editores del menú del canvas no se agregan (solo existen dentro de un
    editor); "Nuevo -> Geometría/Gerber/Excellon" (objetos vacíos para editar) avisa; el área de trabajo usa A4 vertical (sin
    preferencias); el diálogo de "Saltar a" no se abre en el constructor (se llama a `exec()`, así se prueba); mover al origen y
    fijar el origen no mueven los CNC Job (ver 11.12 p). **Volcador:** ahora guarda `objectName` (antes lo descartaba); el
    original usa algunos (`plotarea_tab`, `default_properties`, los `e_*` de Perforación...).
    (s) **Unir, convertir, Ayuda y salir.** Rarezas que se reproducen: al unir Gerber la geometría de seguimiento es la sólida;
    al convertir Geometría -> Excellon el diámetro se compara exacto (varía en la última cifra según la posición del
    círculo) y el agujero se agrega a todas las herramientas con el mismo diámetro redondeado (se repiten agujeros); una
    Geometría convertida a Gerber pone todo en la apertura "0". Desvíos: las Geometrías del port son siempre multiherramienta,
    así que "Convertir simple <-> multi" avisa; "Componentes" de Acerca de lista Qt, GEOS y liblzma (no las bibliotecas de
    Python); `should_we_save` se marca al crear, cambiar, borrar o renombrar objetos. **Volcador:** identificaba las acciones
    con `id()` de Python y los envoltorios temporales de PyQt lo reutilizan: se perdían los submenús "Unir objetos" y
    "Ayuda -> Marcadores" (aparecía "Archivo -> Nuevo" en su lugar). Ahora usa la dirección del objeto de Qt. Los paneles cuya
    raíz es un `QDialog` (Acerca de, Cómo empezar) se generan derivando de `QDialog`.
    (t) **Archivos (ítem 4).** Exportar: el factor a pulgadas es 0,03937 (no 1/25,4) y los tamaños de las aperturas van con
    `str()` de Python, como el original; de las ranuras del Excellon solo quedan `G05`/`Tnn` hasta la primera herramienta con
    ranuras (el original falla con `slot.x` sobre una tupla). Abrir G-code: ".tap" está en la lista de Excellon y en la de
    G-code y, como el original al soltarlo, se abre como los dos; al exportar un G-code abierto se le agrega la cabecera.
    **Error del port corregido:** un Gerber o Excellon en pulgadas no se convertía a milímetros al abrirlo (el original lo
    hace en `new_object` con `convert_units`); se notó al volver a abrir un Gerber exportado.
    (u) **Importar SVG.** Rarezas del original que se reproducen: el inicio de cada `Line` no se multiplica por el factor
    del `viewBox` (el de un `Move` después de cerrar, sí); un trayecto abierto igual se convierte en polígono (se cierra);
    `matrix(a,b,c,d,e,f)` se pasa a Shapely como `[a, b, d, e, xoff, yoff]` (queda traspuesta); un `rect` con `rx`/`ry` hace
    fallar la importación (el original multiplica el texto por el factor); `width`/`height` del `rect` no se multiplican por
    el factor; `parse_svg_point_list` busca los separadores en el texto recortado pero corta el original; los `defs` también
    se importan; como Gerber, con una sola figura sus agujeros pasan a ser polígonos aparte, cualquier línea hace fallar y
    al final queda el `Polygon()` vacío inicial del Gerber. Desvíos: con un `transform` que no entiende, el original queda en
    un bucle infinito (el port ignora el resto); los comentarios XML se saltean (lxml los cuenta como hijos y el original
    falla); los textos (`tspan`) avisan que no se importan. Además, el dibujo de un polígono inválido (que no se puede
    triangular) rellena su `buffer(0)` o solo dibuja el contorno.
    (v) **DXF.** Rarezas del original que se reproducen al importar: si todas las líneas se unen en una sola, falla (una
    `LineString` no es iterable en Shapely 1.8); `SOLID`/`TRACE` no aportan nada; las curvas (`bulge`) de las polilíneas
    se ignoran; los nudos de los splines siempre se reemplazan por uniformes (la lista de la DXF tiene un elemento menos de
    lo que pide el código); un spline por puntos de ajuste falla; en las inserciones las filas y columnas se copian antes
    de escalar y la escala es alrededor del centro de cada figura; como Gerber, cualquier polígono (un círculo) hace fallar.
    Desvío: solo DXF ASCII (ezdxf también lee binario). Exportar: el documento es el de `ezdxf.new('R2010')` (pedido el
    espacio modelo, que cambia el orden de dos objetos), con GUID nuevos y la hora en `$TDCREATE`/`$TDUPDATE`.
    (w) **HPGL2: cuatro errores del original corregidos a pedido del usuario** (desvíos conscientes; `parseHpgl2(...,
    fixOriginalBugs = true)` de fábrica; con `false` se comporta exactamente como el original y así se lo compara con
    `oracle_hpgl2.py`): (1) varias órdenes por línea, partidas en los `;` (el original ignoraba la línea), con coordenadas en
    `PU`/`PD` y varios pares en `PA`; (2) después de un `AA` la posición queda en el final del arco (el original la dejaba
    con dos puntos y casi todo fallaba); (3) `AT` dibuja el arco de la posición al final pasando por el intermedio (el
    original fallaba con `TypeError` al pasarle listas a `three_point_circle` y cortaba la lectura; además terminaba el arco
    en el punto intermedio); (4) el último trazo con la pluma abajo no se pierde. Siguen como el original: nada antes de `IN`,
    y cambiar de pluma con un trazo de un punto, bajar la pluma sin trazo o dibujar sin `SP` hacen fallar la lectura.
    Tests: `Hpgl2.MatchesOriginal` (modo original) y `Hpgl2Fixed.*` (las cuatro correcciones).
    (x) **Preferencias** (fase 5, ítem 6). Como el original: al arrancar se crea `current_defaults.FlatConfig` vacío y
    `factory_defaults.FlatConfig` (de solo lectura, versión "2024.4"); un archivo de otra versión se descarta (la 2024.4 es
    beta); al armar la pestaña los controles de alfa reescriben el alfa de los colores en minúsculas ("#BBF268bf") también
    en las opciones; "Guardar" lee el formulario, copia las opciones al proyecto (`on_options_app2project`), escribe el archivo
    (JSON con sangría 2 y claves ordenadas) y cierra la pestaña; Ctrl+S en la pestaña aplica; cerrar la pestaña con cambios
    pregunta si se guardan. Verificado con `oracle_preferences.py` (el `PreferencesUIManager` original con la interfaz PyQt5
    real): el valor y el tipo de Python de las 485 claves al arrancar, el texto de los campos, la vuelta por un archivo y el
    `eval` de fórmulas y tuplas escritas a mano (`GuiPreferences.*`). Desvíos: lo que el original guarda con `QSettings`
    (tema, estilo, HDPI, tamaños de letra, "machinist", pantalla de bienvenida) va a `settings.ini` de la carpeta de datos
    (no se pisa la configuración del FlatCAM en Python y los tests no tocan la del usuario); el tema de fábrica del port es el
    claro (el original trae el oscuro, pero el port solo dibuja el claro); registrar las asociaciones de archivos solo hace
    algo en Windows (el original falla en otros sistemas); "Abrir carpeta" usa `QDesktopServices`. Las herramientas leen sus
    valores de `app.defaults` (`appDefaults()`, `defNum`, `defBool`, `defStr`, `defPy`, `defPair`, `defDias` en
    `Preferences.h`): NCC, Paint, Isolation, Perforación, Corte, PCB a 2 caras, Transformaciones, Distancia, la herramienta
    de fábrica de las Geometrías (`geometry_*`), los colores de los objetos y de la selección, el zoom, el giro y la
    confirmación al borrar. **Error del port corregido:** cambiar una opción escribía siempre su campo; el `LoudDict` del
    original solo avisa si el valor cambió (`==` de Python), y sin eso, después de "Guardar", volver a cargar el archivo
    reescribía los campos de texto (que emiten `modificationChanged`) y al cerrar la pestaña preguntaba si guardar. **Unidades** (`AppUnits.cpp`: `on_toggle_units`,
    `on_toggle_units_click`, `scale_defaults`, `set_screen_units`): desde la opción de la pestaña (escala las 150 opciones
    con dimensiones) o con la tecla Q y Editar -> Cambiar unidades (sin escalarlas, como el original); convierte Gerber,
    Excellon y Geometría; lo que se abre después y los CNC Job nuevos usan las unidades de la aplicación (G20/G21, decimales
    `cncjob_coords_decimals` / `cncjob_fr_decimals`). Verificado con `oracle_units.py` (el `on_toggle_units` original con la
    pestaña real): las 551 opciones iguales a mm -> pulgadas, de vuelta y con la tecla Q (`ToggleUnitsMatchesTheOriginal`).
    Para eso el `LoudDict` del port distingue tupla de lista (`(0, 0) != [0.0, 0.0]` en Python) y `defaults_read_form`
    pasa por él (un `120 == 120.0` conserva el entero). Desvío: un CNC Job no se convierte (el original solo reescala su
    dibujo; el G-code queda igual). **Área de trabajo y cursor:** la hoja sale de `global_workspaceT`
    (`pagesize_dict`, en las unidades de la aplicación, apaisada con "l"), con la casilla, el tamaño y la orientación de la
    pestaña (`on_workspace`, `on_workspace_modified`, `on_workspace_toggle`, también el rótulo de la barra de estado); el
    cursor con su tipo (cruz chica de `global_cursor_size` / `global_cursor_width` o la grande), su color o negro.
    **Importar/exportar preferencias** (Archivo -> Copia de seguridad). "Opciones -> Transferir opciones" está comentado
    en el original (no se porta). **Corregido en el port:** "Archivos recientes" mostraba los tipos que el original no
    muestra (PNG, preferencias: `icons[kind]` da KeyError y no aparecen). Falta: el nivel Avanzado (`global_app_level`; los
    paneles se generaron en Básico).
    (y) **Error del port corregido (volcador):** el diálogo "Acerca de" mostraba `<MagicMock name='app_url' ...>` en vez de
    `http://flatcam.org`, porque la aplicación simulada de `fc_runtime.py` no tenía `app_url`.
    (m) Errores del original que se reproducen a propósito en Isolation: la herramienta nueva tiene tipo `"           Iso"`
    (con espacios) y forma "V", y el rótulo "Ninguna herramienta seleccionada" lleva `color=#7777FF""`.
14. **Postprocesadores: desvíos y rarezas del original.** (a) `line_xyz` lee `p['end_xy']`, que `CNCjob` no define: en el
    original la cabecera falla y `doformat` la deja **vacía** (sin G21/G90/G94). El port la escribe con `xy_end` (desvío
    consciente; los oráculos le dan `end_xy` para comparar). (b) Se reproducen a propósito: Berta_CNC decide "X,Y End" según el X,Y
    de cambio de herramienta; las plantillas de line_xyz ponen `x_toolchange` también en la Y; Marlin/Repetier/ISEL_ICP dejan el
    paréntesis en ";Spindle Speed: ... RPM)"; Toolchange_Custom solo emite `M6` si hay X,Y de cambio; Marlin_laser_* con Z inicial
    van a la Z de viaje. (c) `linear_code` recibe `z` solo en los tramos de `linear2gcode` (y el primero de `linear2gcode_extra`), y
    `z_cut` en `linear_code` es siempre la del trabajo, no la de la pasada (solo `down_code` recibe la de la pasada): lo notan
    Roland y line_xyz. (d) La sonda: en geometrías `mtool_gen_cncjob` usa las preferencias `geometry_z_pdepth` (-0,02) y
    `geometry_feedrate_probe` (75), no los campos del panel; en Perforación son parámetros generales (`tools_drill_z_pdepth`,
    `tools_drill_feedrate_probe`). (e) `oldx/oldy` del taladrado (line_xyz los usa sin X,Y de cambio): el X,Y de cambio (o 0, 0) y
    después el último agujero. (f) `geometry_tool_gcode_gen` controla la Z de corte (positiva o cero), la de viaje y la
    profundidad por pasada **antes** de leerlas de la herramienta, así que no tienen efecto: no se portan. (g) Combos de la GUI:
    `default` primero (lo reordena `App.__init__`) y el resto en el orden de Windows; Perforación sin `hpgl`; en Geometría, `hpgl`
    apaga la profundidad múltiple y fuerza el cambio de herramienta (y al cambiar de postprocesador se restauran). Antes la lista
    no tenía `hpgl` y ponía Berta_CNC antes que `default`: estaba mal. (h) El taladrado ganó `DrillJobSettings::flavor` (antes
    usaba siempre la 8.994; solo cambia `GRBL_laser`).
13. **GUI: pendientes, en orden:** ~~dibujar Gerber y Excellon~~, ~~abrir archivos y el árbol del Proyecto~~, ~~Propiedades de
    Gerber y Excellon~~ y ~~los paneles de las cuatro herramientas~~ (hechos; falta menú contextual del árbol, mostrar/ocultar y
    borrar). ~~Perforación → CNC Job → exportar~~ (hecho). ~~Corte → Geometría → CNC Job~~ (hecho; varias herramientas en un
    CNC Job, hecho). ~~Paint y NCC~~ (hechos). ~~Reflejar~~ (hecho: Opciones → Voltear y teclas X / Y). ~~PCB a 2 caras~~ (hecho). ~~Isolation~~ (hecho, salvo la selección por área y por polígono con el mouse). ~~Abrir
    proyectos~~ y ~~guardarlos~~ y ~~el menú contextual del árbol~~ (hechos; el original abre lo que guarda el port: `check_project_in_original.py`).
    Desvíos: un taladrado sin cambio de herramienta (un solo G-code) se guarda con el G-code en la primera herramienta y
    los agujeros en todas. Al reabrir, las Geometrías multiherramienta arman su geometría general desde las herramientas
    (la copia general se descarta al leer). **Decisión del usuario (opción 2):** en el original, al reabrir un proyecto el CNC
    Job pierde su código de inicio (`gc_start` no está en `ser_attrs`) y exporta sin `G21/G90/G94`; **el port lo reconstruye**
    (`ProjectIO::rebuildStartGcode`, `CncJob::geometryStartGcode` / `drillStartGcode`). En una geometría los `data` de
    `cnc_tools` son los usados; en un taladrado el `data` de cada herramienta queda con valores viejos y lo usado está en los
    atributos del trabajo (`z_feedrate`, `spindlespeed`, `z_toolchange`, `z_move`...), la Z de corte en cada herramienta, la Z
    y X,Y finales en el cierre del G-code y los límites en el Excellon de origen (`<nombre>` sin `_cnc`). Con eso los dos CNC
    Job del proyecto real del usuario se exportan **idénticos** a sus `.nc` (fecha incluida). Con láser se usa la variante
    8.994 (la predeterminada del port).
    Menú contextual del Proyecto (pedido del usuario, punto 1 de 3): igual al `menuproject` del original (transcripto de
    `MainGUI.py`, no generado: es un QMenu suelto que el volcador no recorre). Desvíos: "Editar" avisa que los editores no
    están en el port; "Propiedades" muestra la pestaña Propiedades (la herramienta Propiedades no está); "Ver fuente" abre
    el texto en solo lectura (el original abre su editor de texto); "Guardar" de una Geometría (DXF) avisa que falta.
    Renombrar: Enter en el campo Nombre de Propiedades, como el original (sin mensaje). Siguen (en este orden, pedido del
    usuario): ~~proyectos recientes~~ (hecho: Archivo → Proyectos recientes y Archivos recientes, los últimos 10, en
    `~/Library/Application Support/Open Source/FlatCAM/` en el Mac; el original usa `~/.FlatCAM`, no se pisan) y lo que falta de
    Paint/NCC/Isolation: ~~métodos Semilla, Líneas y Combo~~ y ~~mecanizado de resto de Paint y NCC~~ (hechos:
    `PolygonClearing::clearPolygonSeed/Lines/clear`, `Paint::paintRest`, `Ncc::clearCopperRest`; oráculos
    `oracle_clearing_methods.py` y `oracle_rest_machining.py`: exactos con GEOS 3.11 y 3.12; con 3.15 la semilla parte los
    círculos en otros puntos y el resto de NCC cambia alguna unión, con los vértices a menos de 1 micra). ~~Área y
    polígono con el mouse~~ (hecho, `CanvasPicker`, con `PlotCanvas::mouseReleased` que indica si hubo arrastre): Paint
    (polígono, área, objeto de referencia como `paint_poly_ref`), NCC (área y objeto de referencia: `Ncc::areaRegion`,
    `referenceRegion`), Isolation (área → `limitedArea`; polígono y, con "Interiores", los agujeros con diámetro negativo).
    **Los tres puntos pedidos están hechos.** Siguen sin portar (fase 5, sección 13; ya hechos "Líneas láser" de Paint y la operación "Aislamiento" de NCC):
    la base de datos de herramientas, la verificación del diámetro seguro y el ajuste a la grilla de los clics. **Corrección:** se
    decía que había que cargar "las preferencias del usuario" porque los paneles mostraban 40 % y "Semilla" en vez de sus 80 % y
    "Estándar": es falso. Las opciones guardadas en su proyecto (`luz_de_noche.params.json`, `options`) son las de fábrica; los
    valores de cada trabajo los escribe a mano. Un sistema de preferencias propio queda como mejora, no como requisito. Antes
    decía: reflejar (Transformaciones o PCB a 2 caras), cargar las preferencias del usuario (hoy los paneles tienen los valores de fábrica:
    por ejemplo NCC 40 % y "Semilla" en vez de sus 80 % y "Estándar"); pestaña Propiedades de cada objeto (`ObjectUI.py`, generada igual que la ventana); paneles de Perforación, Corte, Paint y NCC
    (generados desde `ToolDrilling`, `ToolCutOut`, `ToolPaint`, `ToolNCC` con sus valores de fábrica) y su lógica sobre el núcleo;
    CNC Job y exportar `.nc`; reflejar (herramienta PCB a 2 caras / Transformaciones).

## 12. Retomar el trabajo en Claude Code (CLI)

Esta sección es para empezar una sesión nueva sin haber visto la conversación anterior. El estado real es lo que está en GitHub
(`https://github.com/dnasta/flatcam-cpp`, rama `main`); lo que sigue lo resume.

### 12.1 Dónde estamos (al cerrar la sesión del chat)

- **2.763 casos de test** (`ctest`): 2.662 del núcleo (2.251 son los casos parametrizados de los postprocesadores) y 101 de la GUI;
  todos pasan salvo 1 lenta (`NccEndToEnd`, ~100 s) que queda saltada salvo con `FLATCAM_SLOW_TESTS=1`.
  Pasan con GEOS 3.12 y 3.15.0, en Linux y en el Mac del usuario. La GEOS 3.11 solo se usa para separar diferencias de versión.
- Fases 1, 2 y 3 hechas. Las herramientas portadas, todas verificadas contra el código **original** de 2024.4 con
  oráculos en Python (Shapely 1.8.4) y, donde existe, contra el `.nc` real del usuario (FlatCAM 8.994):

| Pieza | Dónde | Contra el original | Contra el `.nc` real |
|---|---|---|---|
| Parsers Gerber/Excellon, transformaciones | `src/parsers/` | exacto | — |
| Postprocesadores `default` y `GRBL_laser` | `src/cam/Preprocessor.*` | 224 líneas idénticas | `B_Cu.nc`, `Logo.nc` desde sus trazados: byte por byte |
| Taladrado | `CncJob::generateFromExcellon` | idéntico con el orden del archivo | igual salvo el **orden** de los agujeros (OR-Tools; optimizador propio) |
| G-code de geometrías | `CncJob::generateFromGeometry` | 26 escenarios exactos | contorno: byte por byte |
| CutOut | `src/tools/CutOut.*` | vértice por vértice | contorno: byte por byte (corte rectangular) |
| Paint estándar | `src/tools/Paint.*`, `PolygonClearing`, `EndpointStorage` | < 1e-9 (9 casos) | Logo: 38 trazados, 3,5–5 micras |
| NCC estándar | `src/tools/Ncc.*` | 8 casos exactos | `B_Cu`: rangos exactos, 0,05 % de puntos; 746 contra 723 trazados |
| PCB a 2 caras | `src/tools/DoubleSided.*` | 6 casos, bits exactos | `Alinea.nc` salvo líneas en blanco y el agujero (0, 0), que agregó el usuario a mano |
| Isolation | `src/tools/Isolation.*` | 27 casos exactos; `B_Cu` vértice por vértice con GEOS 3.11 | — (el usuario no tiene `.nc` de aislamiento) |

Contra el **ejecutable 2024.4** del usuario (placa "luz de noche"): CutOut byte por byte; Paint idéntico con GEOS 3.11; NCC 272 = 272
trazados; taladro con los mismos agujeros (sección 7).

- Decisiones del usuario vigentes: G-code del láser como el de su 8.994 (`G00 Z0`, variante por defecto); optimizador propio de
  agujeros; método estándar de Paint/NCC; los Gerber del usuario inferiores están **reflejados en X**; Isolation entra en el alcance;
  el agujero (0, 0) de `Alinea` no se genera (lo puso a mano); el canvas de la fase 4 es **`QOpenGLWidget`**.

### 12.2 Lo primero que hay que hacer

1. `cd` al repo (la ruta tiene espacios: siempre entre comillas), `git pull`, `cmake -S . -B build -G Ninja`, `cmake --build build`,
   `ctest --test-dir build --output-on-failure`. Tienen que dar 2.763 casos, 1 saltado.
2. Fase 4 en curso: seguir con los pendientes de la sección 11.13. Las capturas del usuario están en
   `docs/fase4_gui_referencia.md`.
3. Qt6 todavía no está en el `CMakeLists.txt`: la GUI va en una biblioteca/ejecutable aparte (`src/gui/`), el núcleo sigue sin Qt.
4. **Decisiones del usuario para la fase 4:** interfaz en **español**; alcance de la primera versión: abrir Gerber y Excellon, verlos
   en el canvas OpenGL (zoom y desplazamiento), reflejar, Perforación, Corte, Paint y NCC con sus paneles, CNC Job y exportar `.nc`
   (Isolation, PCB a 2 caras y proyectos van después); **la GUI tiene que ser exactamente igual a la del FlatCAM en Python**.
5. **Método elegido para la igualdad exacta** (en curso): construir las clases de interfaz ORIGINALES con PyQt5 real
   (`tools/gui_dump/fc_runtime.py`, entorno `old` con `pyqt=5.15` y `matplotlib`) y volcar su árbol de widgets (menús, acciones,
   íconos por ruta de recurso, textos en español de `translate/es.bin`, tooltips, atajos, barras, pestañas, paneles) a código C++
   generado; la lógica se porta a mano. Ya funciona `fc_runtime.build_main_gui()`: la `MainGUI` original se construye entera en
   español (menús `Panel de palanca`, `Archivo`, `Editar`, `Opciones`, `Ver`, `Objetos`, `Herramienta`, `Ayuda`). Siguen: el
   volcador (`dump_main_gui.py` → C++), extraer los íconos de `resources.py` (formato rcc: árbol + datos con qCompress) y la tabla
   `es.bin` para los mensajes de la lógica, el objetivo `flatcam` en CMake con Qt 6.12 (Homebrew, ya instalado) y el canvas.
   **Hecho:** todo eso. `fc_runtime.build_app()` emula el arranque de `App` con sus métodos originales (vista de la colección,
   `setup_default_properties_tab`, `install_tools`, `set_ui_title`, `on_layout('minimal')` de la primera ejecución, consola oculta,
   panel izquierdo cerrado). El volcado compara cada `Q_PROPERTY` con una instancia nueva de la clase base de Qt y guarda solo lo
   distinto; registra la ruta de cada `QIcon` y `QPixmap`. Notas: (a) los widgets dentro de un `QToolBar` aparecen ocultos hasta que
   la ventana se muestra: su visibilidad real es la de la acción; (b) `QSettings` del original se redirige a un INI temporal (la
   primera corrida escribió `com.open-source.FlatCAM.plist` en el Mac; se borró); (c) nunca borrar el módulo `resources` de
   `sys.modules`: Qt usa sus bytes sin copiarlos (si se libera, `QPixmap` se cae); (d) la consola Tcl (`FCShell`) se reemplaza.

### 12.3 Cómo se trabaja acá (reglas del usuario)

- Español rioplatense ("vos"), comandos para zsh, `python3 -m pip install`. Nunca pedir tokens ni contraseñas.
- **Verificar y corregir todo apenas se detecte**, sin esperar a que lo pida; si una afirmación previa resulta falsa, decirlo y corregir
  código, tests y docs en el mismo turno.
- **Antes del mensaje final de cada respuesta, actualizar `CLAUDE.md` (sección "Estado actual") y `AGENTS.md` (secciones 4, 11 y 12).**
- Commits en español con prefijo de fase, sin `--force`, sin tocar `reference/`. Doxygen en español en los encabezados, sin warnings.
- **Probar siempre con dos versiones de GEOS** (sección 6). La regla salió de un caso real: con GEOS 3.15 fallaban tests que pasaban con
  la 3.12. Las tolerancias de los tests de geometría se fijan en micras, no en el último bit; los tests que comparan con el original
  tienen un nivel estricto (misma estructura: igualdad al milmillonésimo) y uno relajado (agregados: puntos, longitud, cantidad).
- Para probar arrastrar y soltar en una vista sin `QDrag::exec`: mandar a `viewport()` un `QDragEnterEvent` (sin él la vista
  rechaza el `QDragMoveEvent`), luego el `QDragMoveEvent` y el `QDropEvent`. Los grupos del árbol necesitan
  `Qt::ItemIsDropEnabled` para que se pueda soltar entre filas de sus hijos.
- El `.nc` del contorno de "luz de noche" es de **una sola pasada** (Z -1,8): el usuario desactivó "Profundidad múltiple" en el
  Corte (de fábrica viene activada). El test de la GUI lo detectó: con pasadas de 0,6 salían 333 líneas en vez de 148.
- Los paneles de Propiedades se vuelcan después del `set_ui` del objeto original (que esconde lo del nivel Básico); para eso el
  objeto necesita sus opciones como en `AppObject.new_object` (`<tipo>_*` y las de herramientas) y vivir dentro del área de
  Propiedades (cambia los márgenes a 9).
- **Error corregido:** la primera versión del objeto Geometría simplificaba con `0,01 / 20`; `global_tolerance` vale **0,005**,
  así que es `0,00025` (lo que ya decía la sección 7). En el contorno del Corte no cambiaba nada (por eso su test pasaba); en Paint
  y NCC faltaba el 25 % de las líneas. Lo detectó el test de la GUI contra el `.nc` real.
- En "Voltear" el original llama `mirror('Y')` desde "Voltear en el eje X" (invierte las X) y `mirror('X')` desde "en el eje Y".
  Las teclas del árbol del Proyecto van a los atajos de la ventana (`view.keyPressed → ui.keyPressEvent`).
- Proyectos: `tests/data/luz_de_noche/luz_de_noche_reducido.FlatPrj` es el proyecto real del usuario sin la Geometría ni el
  CNC del Paint y del NCC (que pesan cientos de MB) y con las opciones globales filtradas como `extract_flatprj_params.py`
  (privacidad). El escritor de `ProjectFile` reproduce byte por byte el JSON de Python de ese archivo (test). El proyecto
  completo (535 MB) guarda tres copias de la geometría del NCC (540 MB de WKT cada una): la de la Geometría, la de su
  herramienta y la de la herramienta del CNC Job.
- Para verificar que el FlatCAM original abre un proyecto guardado por el port: `FLATCAM_SAVE_COPY=/tmp/p.FlatPrj
  build/tests/gui_tests --gtest_filter=GuiProject.SavesObjectsCreatedInThePort` (con `FLATCAM_SAVE_COPY2` guarda además el
  proyecto del usuario vuelto a guardar) y `micromamba run -n old env QT_QPA_PLATFORM=offscreen python
  tests/tools/check_project_in_original.py /tmp/p.FlatPrj` (from_dict, bounds, set_ui, build_ui y export_gcode de cada objeto).
  Así se encontró que al volver a guardar se perdía el `data` de las herramientas de los CNC Job (el original lo necesita).
  Curiosidad del original: `CNCjob.bounds` reinicia los mínimos en cada herramienta, así que da los límites de la última.
- Para mirar el canvas OpenGL desde un test: `QT_QPA_PLATFORM=cocoa build/tests/gui_tests --gtest_filter=...` (sin esa
  variable los tests corren en "offscreen", donde `QOpenGLWidget` no dibuja) y `ui()->grab().save(...)`.
- Un test que compara un texto consigo mismo siempre pasa: una vez se coló una aserción así; revisar que cada `EXPECT` pueda fallar.

- **Errores:** los del port, corregirlos apenas se encuentran; los del original, reproducirlos y anotarlos en la sección 14
  para corregirlos al terminar el proyecto (sección 3).

### 12.4 Entorno para regenerar los goldens (solo hace falta si se agrega un oráculo)

Los goldens (`tests/golden_*.h`) **están commiteados**; los tests no necesitan Python. Para regenerarlos o crear oráculos nuevos hace falta
el entorno fiel al original. **En el Mac ya está instalado** (micromamba de Homebrew, raíz `~/.local/share/mamba`, entornos `old` con
Python 3.10 + Shapely 1.8.4 + GEOS 3.11.0 + `rtree`, `geos312` y `geos311`). `shapely=1.8.4` sí existe para osx-arm64. Si no está
inicializado el shell, usar `MAMBA_ROOT_PREFIX=$HOME/.local/share/mamba micromamba run -n old python ...`. Para recrearlo:

```zsh
brew install micromamba                      # o el instalador oficial
micromamba create -n old -c conda-forge python=3.10 "shapely=1.8.4" numpy=1.22 rtree
micromamba run -n old python tests/tools/oracle_paint.py     # ejemplo
```

Importante: Shapely
1.8.x (no 2.x: da 3 polígonos donde 1.8.4 da 31 en el buffer de Gerber) y el paquete `rtree` **real** (el oráculo de Paint/NCC lo necesita;
`tests/tools/oracle_common.py` solo lo reemplaza por un simulacro si no está instalado). Para probar con otra GEOS en el Mac:
`micromamba create -n geos312 -c conda-forge geos=3.12` y compilar con `-DCMAKE_PREFIX_PATH=<raíz del entorno>
-DGEOS_DIR=<raíz>/lib/cmake/GEOS -DCMAKE_BUILD_RPATH=<raíz>/lib`.

### 12.5 Cosas que se aprendieron y conviene no repetir

- El original (Python) tarda ~9 min en el NCC de `B_Cu` (26 millones de puntos); correr de a uno (la memoria no alcanza para dos a la vez)
  y desacoplado de la terminal. El port tarda ~1 min.
- Mapa de parámetros reales del usuario: `tests/data/PARAMETROS.md` y `tests/data/dosificador_PCB_principal.params.json`. La tolerancia de
  simplificación del G-code es `global_tolerance / 20 = 0,00025`. Los pasos por círculo son 64.
- Diferencias 8.994 contra 2024.4 ya medidas: `GRBL_laser` (`G00 Z0` en la cabecera, orden del Z inicial, sin `G00 Z..` al final);
  `Alinea` (líneas en blanco por agujero y agujero en (0, 0)); `B_Cu` (23 uniones más). Todo lo demás coincide.
- GEOS 3.15 empieza algunos contornos en otro vértice que la 3.11/3.12; eso cambia en ±1 trazado la unión de anillos del NCC y hasta 1,2 %
  de longitud en casos chicos. No es un error.
- `PreparedShape` (GEOS preparado) da exactamente los mismos resultados que los predicados sueltos y es mucho más rápido.
- Para ver si una diferencia contra el original es del código o de GEOS: compilar contra `geos311` (la del oráculo) y comparar. Así se
  vio que en Isolation las diferencias de `B_Cu` (vértices del buffer de 16 segmentos con GEOS 3.12+, orden de los polígonos del Gerber
  con GEOS 3.15) son de GEOS: con 3.11 coincide todo.
- La 2024.4 simplifica **todas** las trayectorias de geometría del CNC Job con `global_tolerance / 20` (también el contorno del
  CutOut): sin eso sobran vértices en las esquinas redondeadas.
- En los proyectos del usuario las capas inferiores y el taladro siempre están reflejados en X (`mirror(Axis::Y, {0, 0})`).
- Al volcar la interfaz, comparar cada widget contra una instancia nueva de su **clase de Python** (no solo de la base de Qt):
  `FCDetachableTab` deja las pestañas cerrables y el original lo desactiva después; comparando contra `QTabWidget` se perdía.
- En el canvas, el blending tiene que dejar el alfa del destino en 1 (`glBlendFuncSeparate`): Qt compone el framebuffer de un
  `QOpenGLWidget` como premultiplicado y los colores semitransparentes de FlatCAM salían corridos (magenta).
- Algunas clases del original pisan métodos de Qt con atributos (`FCDetachableTab.tabBar`, `layout` en otras): en el volcador
  llamar siempre a los métodos de la clase base (`QTabWidget.tabBar(w)`, `QWidget.layout(w)`).
- Las fuentes se vuelcan solo con lo que difiere de la fuente por defecto (la familia del sistema donde corre Python no sirve).
- El núcleo ganó `DrillJobSettings::toolchange` (solo cambia la cabecera en `default`; sin cambio de herramienta el
  original junta todos los agujeros en un trabajo con la primera herramienta: portado pero NO verificado contra un oráculo) y
  `DrillJobSettings::toolOrder` ("Orden de la Herram").
- Las Z de corte del `.nc` real del usuario (`-2.4000000000000004`) vienen de una cuenta en coma flotante guardada en sus
  preferencias; desde el campo (4 decimales) sale `-2.4`. No es una diferencia del port.
- Para comparar la GUI con el original: correr `FlatCAM --screenshot` y mirar la captura junto a las del usuario (sus capturas
  son de Windows con el estilo nativo: por eso se usa Fusion).
- Para un oráculo de una herramienta con mucha interfaz conviene extraer los métodos del fuente con `ast` y darles una interfaz
  simulada (como `oracle_isolation.py`) en vez de transcribirlos: así se ejecuta el código original tal cual.

## 13. Port completo: lo que falta y en qué orden (pedido del usuario: "todo lo que falta portar")

El usuario pidió portar **absolutamente todo** lo que falta. Inventario del original (`reference/FlatCAM_2024_4/FlatCAM/`) y orden
de trabajo (de lo que más afecta al G-code a lo accesorio). Marcar cada ítem al terminarlo, con su verificación.

1. ~~**Postprocesadores restantes**~~ **Hecho:** los 18 que faltaban (20 en total). Verificado: cada método idéntico al original
   (`oracle_preprocessors.py`, 2.251 casos), y de punta a punta en geometrías y taladrado (`oracle_geometry_modes.py`,
   `oracle_drill.py`); desvíos en la sección 11.14. La interfaz de Paste_1 se usará con la herramienta de pasta de soldar (ítem 5).
2. **Pendientes de las herramientas ya portadas:** ~~"Líneas láser" de Paint (`fill_with_lines`)~~ (hecho, `oracle_laser_lines.py`), ~~operación "Aislamiento" de NCC
   (`isotooldia`)~~ (hecho, `oracle_ncc_isolation.py`), ~~verificación del diámetro seguro~~ (hecha: botón "Óptimo" y casilla
   "Comprobar validez" de NCC e Isolation, `Isolation::safeToolDiameter`, `oracle_safe_tooldia.py`), ~~ajuste a la grilla de los clics~~ (hecho: `MainGUI::snapToGrid`,
   las señales del canvas salen ajustadas, la cruz del cursor solo con la grilla; elegir polígonos la apaga y la restaura; elegir
   el agujero en PCB a 2 caras la apaga y, como el original, no la restaura), ~~áreas de exclusión~~ (hechas: núcleo, CNC Job de
   Perforación y Geometría, y la GUI de los dos paneles), ~~pulido~~ (hecho: "Agregar acabado" de la Geometría agrega la
   herramienta "Pulir" sobre la caja del objeto más el margen, como `on_polish`), ~~base de datos de herramientas~~ (hecha: `ToolsDatabase`, pestaña `ToolsDBTab` generada desde `ToolsDB2`, búsqueda al agregar y "desde la base" en NCC, Paint, Isolation, Corte y Geometría; NCC, Paint y Corte comparan el objetivo con un texto y, como el original, nunca encuentran; Geometría ganó "Buscar y agregar" y "Borrar"). ~~Fresado de ranuras~~ (hecho con el ítem 5). Lista vieja: verificación del diámetro seguro (NCC e Isolation), ajuste a la
   grilla de los clics, áreas de exclusión (Perforación y Geometría), pulido (`polish`) y fresado de ranuras/agujeros (Excellon).
3. ~~**Transformaciones**~~ (hecha: `tools/TransformTool.*`, panel generado de `ToolTransform`; rotar, inclinar, escalar,
   desplazar, reflejar y engrosar los objetos elegidos alrededor del origen, la selección, un punto u otro objeto; Gerber,
   Excellon, cuyo engrosado cambia el diámetro de las herramientas como el original, y Geometría; los CNC Job avisan. Desvío: con
   referencia "Punto" el original falla, porque lee `self.uipoint_entry`, y el port usa el punto). ~~Mover~~ (`ToolMove`),
   ~~escala y desplazamiento del panel Propiedades de cada objeto~~, ~~Distancia~~ y ~~Distancia mínima~~ (`ToolDistance`,
   `ToolDistanceMin`) y ~~Propiedades~~ (`ToolProperties`): hechos (sección 11.12 p). Se encontró y se hizo lo que faltaba del
   CNC Job: la distancia recorrida y el tiempo estimado (11.12 q, idénticos al original).
3b. **Lo que falta de la ventana principal** (encontrado al portar el ítem 3). **Hecho:** ~~elegir objetos con clic y con un
   rectángulo en el canvas~~ (`AppCanvasSelection.cpp`: `select_objects`, `selection_area_handler`, recuadros de selección,
   Ctrl suma, Shift copia las coordenadas, doble clic abre Propiedades), ~~el menú del canvas~~ (`popMenu` con Grillas,
   Ver, Copiar, Borrar, Mover, Propiedades), ~~Ver~~ (mostrar/ocultar todos y los otros, redibujar, limpiar, área de trabajo A4,
   pantalla completa, área de dibujo, panel, grilla, líneas de grilla, ejes, HUD), ~~Editar~~ (borrar, copiar, elegir todo,
   mover al origen, fijar el origen, saltar a con `DialogBoxRadio`, ubicar) y ~~Archivo -> Nuevo proyecto~~ (`AppEditMenu.cpp`).
   También ~~unir~~ (`AppJoin.cpp`: Geometría, Excellon y Gerber, con las reglas de `merge`), ~~convertir a Geometría, Gerber
   y Excellon~~ (`AppConvert.cpp`), ~~girar e inclinar con diálogo~~ (`FCInputDoubleSpinner`), ~~"Ver fuente" de Opciones~~,
   ~~Ayuda~~ (`AppHelp.cpp`: Acerca de y Cómo empezar generados del original, enlaces, lista de atajos con los textos del
   original en `resources/shortcuts.json`), ~~salir con `final_save`~~ (con `should_we_save`) y ~~los atajos de la aplicación~~
   de `MainGUI.keyPressEvent` (Ctrl, Shift, Alt y sin modificador, salvo los de herramientas o funciones sin portar).
   **Falta:** cambiar las unidades (Q; va con las preferencias, ítem 6), el administrador de marcadores (Ayuda), los objetos
   vacíos de Archivo -> Nuevo (para los editores, ítem 8). "Guardar copia del proyecto" está comentado en el original.
4. **Archivos:** ~~abrir G-code como CNC Job~~ (hecho: `App::openGcode`, menú, arrastrar y soltar, recientes; el texto tal
   cual, sin '%' ni MOIN/MOMM), ~~exportar Gerber y Excellon~~ (hecho: `src/io/FileExport.*`, idéntico al original en 15
   Gerber y 8 Excellon con `oracle_export.py`; mover o fijar el origen regenera el texto fuente como el original),
   ~~exportar SVG~~ de Gerber, Excellon y Geometría (`src/io/SvgExport.*`: el `svg()` de Shapely 1.8 y `toprettyxml`;
   `oracle_svg.py`: idéntico salvo el vértice inicial de los anillos y las últimas cifras del buffer, que cambian con la
   GEOS), ~~importar SVG~~ como Geometría o Gerber (`src/parsers/SvgImport.*`: un lector XML mínimo, el intérprete de
   trayectos de `svg.path` 7.1 y `ParseSVG`; `oracle_svg_import.py` sobre `tests/data/svg/`: idéntico vértice por vértice,
   salvo el vértice inicial de lo que pasa por `buffer(0)`). Falta: los textos de un SVG (fuentes TTF de `ParseFont`).
   ~~El SVG de un CNC Job~~ (hecho: `exportCncSvg` en `src/io/SvgExport.*`, `App::cncSvgText` con el `gcode_parsed` del
   último G-code; `oracle_cnc_svg.py`: idéntico con GEOS 3.11. Como el original: un taladrado no se exporta, porque sus
   cortes son polígonos y `Polygon.svg` no acepta `stroke_color`; el trazo es `tooldia / 2`, en un G-code abierto
   `cncjob_tooldia` (1,0). **Error del port corregido:** `ProjectIO::gcodeParse` no separaba las líneas en `\r` y leía
   `Z-3.5\r` como 0 en los `.nc` de Windows; también cambiaba el `gcode_parsed` guardado en los proyectos). ~~Importar DXF~~ (`src/parsers/DxfImport.*`: un lector propio de DXF ASCII
   en lugar de ezdxf, y `ParseDXF` + `ParseDXF_Spline`; `oracle_dxf_import.py` con ezdxf 0.14.2 sobre `tests/data/dxf/`,
   generados por `tests/tools/make_test_dxf.py`: idéntico), ~~exportar DXF~~ (`src/io/DxfExport.*`: la plantilla R2010 de
   ezdxf, `tools/gen_dxf_template.py` → `src/io/generated/DxfR2010Template.h`, con la numeración de ezdxf; idéntico salvo
   GUID y fechas), ~~exportar PNG~~ (la imagen del canvas, sin alfa, como `_screenshot`), ~~importar HPGL2~~ (`src/parsers/Hpgl2.*`,
   `oracle_hpgl2.py` sobre `tests/data/hpgl2/`: idéntico, también en sus fallas). ~~Importar imagen~~ (hecho: `src/parsers/ImageImport.*`, `tools/ImageTool.*`;
   `oracle_image.py`: idéntico. Como `rasterio.features.shapes` + `unary_union`, el resultado es la unión de los píxeles
   elegidos: el port la arma con un rectángulo por tramo de fila; las coordenadas son las mismas, `col * 25,4 / dpi` y
   `-(fila * 25,4 / dpi)`). ~~Importar PDF~~ (hecho: `src/parsers/PdfImport.*`, `App::openPdf`,
   Archivo -> Importar -> PDF y Ctrl+Q; `oracle_pdf.py`: idéntico. Desvíos: la pila de estados gráficos se vacía en cada
   archivo (en el original es del analizador y queda de un PDF al siguiente si no están balanceados los `q`/`Q`); un trazo
   de grosor 0 da un elemento vacío (el original se cae, ver la sección 14); se lee en el hilo de la interfaz). Falta: los
   textos de SVG y DXF (`ParseFont`), abrir/guardar scripts,
   imprimir a PDF (`on_file_save_objects_pdf`) y los objetos Script y Documento (`FlatCAMScript`, `FlatCAMDocument`).
   **Decidido por el usuario:** en HPGL2 se corrigen las cuatro fallas del original (sección 11.12 w).
5. **Herramientas restantes** (`appTools/`). ~~Fresado~~ (hecho): `ToolMilling` no se instala en la 2024.4 (no está en
   `install_tools` y su botón "Herramienta de fresado" del panel del Excellon está deshabilitado; su código usa la estructura
   vieja del Excellon, `self.drills`), así que el fresado que se usa es el de la sección "Geometría de fresado" del panel del
   Excellon: "Fresar agujeros" y "Fresar ranuras" (`src/tools/ExcellonMilling.*`, `App::millExcellon`), verificado con
   `oracle_milling.py` (el `generate_milling_drills` / `_slots` original: los mismos anillos vértice por vértice, también los
   errores) y `GuiMilling.*`. Como el original, al abrir el panel no queda ninguna herramienta elegida (`selectColumn(0)` no
   elige nada en una tabla de filas) y los botones quedan deshabilitados hasta elegir. ~~Panelizar~~ (hecho: `src/tools/Panelize.*`, `tools/PanelizeTool.*`, Alt+Z; verificado con `oracle_panelize.py`, que
   ejecuta el `on_panelize` original completo con una interfaz simulada: Gerber como Gerber y como Geometría, restricción de
   área, Excellon con agujeros y ranuras y Geometría con y sin "Optimizar", idénticos). ~~Invertir Gerber~~ (hecho: `src/tools/InvertGerber.*`, `tools/InvertTool.*`, Alt+G; `oracle_invert.py` sobre
   el contorno de "luz de noche" con los tres tipos de esquina y el logo del dosificador: idéntico con GEOS 3.12 salvo una
   figura; con 3.15 la resta da otros vértices con la misma geometría, a menos de media micra cuadrada). ~~Calculadoras~~
   (hecho: `tools/CalculatorsTool.*`, Alt+C; los resultados y formatos de las fórmulas del original, `GuiCalculators`).
   ~~Marcas de esquina~~ (hecho: `src/tools/CornerMarkers.*`, `tools/CornersTool.*`, Alt+M; `oracle_corners.py` ejecuta
   `add_markers`, `add_corners_geo` y `on_create_drill_object` originales: semicruz y cruz, apertura existente y nueva,
   taladros; mismas líneas, aperturas y polígonos). ~~Extraer taladros~~ (hecho: `src/tools/ExtractDrills.*`,
   `tools/ExtractDrillsTool.*`, Alt+E (el menú dice Alt+I, pero `keyPressEvent` usa E); `oracle_extract_drills.py` sobre los
   dos cobres del usuario con los tres modos y los filtros: mismas herramientas, diámetros y agujeros). ~~Perforar Gerber~~
   (hecho: `src/tools/PunchGerber.*`, `tools/PunchTool.*`, Alt+H; `oracle_punch.py` ejecuta `on_generate_object` y los cuatro
   métodos originales sobre los cobres del usuario y el taladro del dosificador, también los errores: mismos polígonos
   (cantidad y áreas) y las mismas aperturas nuevas con sus agujeros "clear"). Desvío: la columna "M" no marca las
   aperturas en el dibujo. Ojo: esta tabla elige celdas (un `FCTable` sin arrastrar y soltar), así que `selectColumn(0)`
   deja elegidas todas las aperturas; la del Excellon elige filas y no deja ninguna. ~~Restar~~ (hecho: `src/tools/Subtract.*`,
   `tools/SubTool.*`, Alt+W; `oracle_sub.py` ejecuta `aperture_intersection`, `new_gerber_object` y `toolgeo_intersection`
   originales: cobre menos máscara y al revés, cobre menos logo, y geometrías con y sin "Cerrar trayectos"; las mismas
   aperturas, elementos y áreas). Hizo falta reproducir el `MultiPolygon` de Shapely 1.8, que de un `MultiPolygon` dentro de
   la lista se queda solo con la primera parte. ~~Óptimo~~ (hecho: `src/tools/OptimalDistance.*`, `tools/OptimalTool.*`,
   Alt+O; `oracle_optimal.py` ejecuta el `find_minimum_distance` original sobre el cobre de "luz de noche" con 4 y 2
   decimales, el logo del dosificador y el contorno de un solo polígono: la misma mínima, cantidad, puntos y lista de las
   demás distancias). ~~Compensación de grabado~~ (hecha: `src/tools/EtchCompensation.*`, `tools/EtchTool.*`, sin
   atajo, como el original; `oracle_etch.py`: idéntica). ~~Alinear objetos~~ (hecho: `src/tools/AlignObjects.*`,
   `tools/AlignTool.*`, Alt+A; `oracle_align.py`: idéntico; desvío: arrastrar mientras se eligen puntos no dibuja el
   rectángulo de selección, como en el original tampoco elige nada). ~~Fiduciales~~ (hecho: `src/tools/Fiducials.*`,
   `tools/FiducialsTool.*`, Alt+F; `oracle_fiducials.py`: idénticos; desvíos: los campos de coordenadas de la tabla son
   `FCEntry` (en el original `EvalEntry`, que no se lee nunca) y los combos eligen el último Gerber al abrir la herramienta,
   no cada vez que se agrega uno). ~~Código QR~~ (hecho: `src/tools/QrCode.*` porta la biblioteca `qrcode` 7.4.2 de Python
   (licencia BSD, agregada a `NOTICE`), `tools/QRCodeTool.*`, Alt+Q; `oracle_qrcode.py`: idéntico. Desvíos: si el texto no
   entra, el original conecta el mouse antes de fallar en su hilo y el clic después falla; el port avisa y no espera el
   clic; el PNG sale siempre en 8 bits por canal (PIL usa 1 bit con los colores "black"/"white"), con los mismos píxeles). ~~Robo de cobre~~ (hecho: `src/tools/CopperThieving.*`, `tools/ThievingTool.*`, Alt+J;
   `oracle_thieving.py`: idéntico con GEOS 3.11. Desvíos: el texto fuente de una máscara con separación negativa sale
   completo (el original guarda `clear: []` en cada elemento y su `export_gerber` corta cada apertura en el primero); los
   demás atajos del teclado siguen activos mientras se elige el área (en el original solo G, I, J y otros pocos)).
   ~~Verificación de reglas~~ (hecha: `src/tools/RulesCheck.*`, `tools/RulesTool.*`, Alt+R, `oracle_rules.py`; desvío: los
   puntos se listan en el orden en que aparecen, el original usa un `set`). ~~Pasta de soldar~~ (hecha:
   `src/tools/SolderPaste.*`, `tools/SolderPasteTool.*`, Alt+K, `oracle_solderpaste.py`; desvío: los valores del formulario
   valen para todas las boquillas). ~~Asistente de PCB~~ (hecho: `src/tools/PcbWizard.*`, `tools/PcbWizardTool.*`, Archivo ->
   Importar, sin atajo; `oracle_pcbwizard.py`: idéntico). ~~Calibración~~ (hecha: `src/tools/Calibration.*`,
   `tools/CalibrationTool.*`, Herramienta -> Calibración y su botón, sin atajo (`install` dice Alt+E pero `keyPressEvent`
   no la usa); `oracle_calibration.py`: idéntica. Desvíos: la tabla de puntos se arma a mano (el volcador no recorre los
   widgets de las celdas); el "Visor de Gcode" es un texto con "Guardar" en vez del `AppTextEditor`; los campos
   `EvalEntry` solo evalúan números). ~~Film~~ (hecho: `src/tools/Film.*`, `tools/FilmTool.*`, Alt+L;
   `oracle_film.py`: idéntico. Desvíos: PNG y PDF se dibujan con QtSvg a partir del mismo SVG en vez de svglib/reportlab
   (mismo tamaño en puntos, 72 dpi, fondo blanco, la misma hoja y el mismo desplazamiento); el SVG se escribe en el
   hilo de la interfaz). ~~Autonivelación~~ (hecha: `src/cam/Autolevel.*`, `objects/CNCAutolevel.*`, en el panel del CNC
   Job con el botón de autonivelación, que como en el original solo se ve en el nivel Avanzado; `oracle_autolevel.py`:
   idéntica. En el original aplicar la nivelación al G-code no está hecho (`autolevell_*` son `pass` y "aplicar" solo
   muestra "Finished autolevelling."), así que el port tampoco lo hace. Desvíos: los mensajes de GRBL van a la barra de
   estado (la consola Tcl todavía no está, ítem 7); los puertos se listan con `QSerialPortInfo`; sin QtSerialPort no se
   puede conectar). **Ítem 5 completo.**
6. **Preferencias** (`appGUI/preferences/`, ~19.000 líneas). **Hecho:** la pestaña completa generada del original (485 campos),
   guardar/cargar `current_defaults.FlatConfig` y `factory_defaults.FlatConfig`, Guardar/Aplicar/Cerrar/Restaurar/Abrir
   carpeta/Borrar ajustes, Editar -> Preferencias, Shift+P, el rótulo de la barra de estado, los manejadores de cada grupo
   (11.12 x, `oracle_preferences.py`). También: las herramientas y los objetos leen sus valores de `app.defaults`.
   También: cambiar las unidades (tecla Q, Editar y la pestaña, `oracle_units.py`).
   También: área de trabajo, cursor e importar/exportar preferencias. **Falta:** el nivel Avanzado.
7. **Consola Tcl** (`ToolShell`, `tclCommands/`, 68 comandos) y scripts.
9. **Hacia la 1.0.0** (pedido del usuario; estimación dada: unas 6 sesiones sin editores y 8 a 9 con editores):
   (a) preferencias y unidades (ítem 6); (b) herramientas (ítem 5) en dos o tres sesiones; (c) lo que queda de archivos
   (ítem 4: PDF, imagen, textos con fuentes, SVG del CNC Job, Script, Documento, imprimir a PDF) y la consola Tcl (ítem 7);
   (d) cierre: comparar la interfaz con las capturas, compilar y probar en Windows y Linux (lo tiene que correr el
   usuario o un CI), empaquetar `FlatCAM.app` sin depender de Homebrew (`macdeployqt`), `CMakeLists` con versión 1.0.0,
   README y notas de la versión. **Decidido por el usuario: los editores (ítem 8) entran en la 1.0.0.** Al final, corregir los
   errores del original de la sección 14.
8. ~~**Editores** de Gerber, Excellon y Geometría~~ (hechos, pedido del usuario "de un tirón"): `src/editors/` (modelos sin Qt,
   contra el original con `oracle_geo_editor.py`, `oracle_exc_editor.py`, `oracle_grb_editor.py`) y `src/gui/editors/`
   (paneles generados del original con `dump_panels.py`: `GeoEditorUI`, `GeoPaintToolUI`, `GeoTransformToolUI`,
   `GeoTextToolUI`, `ExcEditorUI`, `GrbEditorUI`, `GrbTransformToolUI`). Entrar con E, Editar -> Editar objeto, el
   botón de la barra, el menú del Proyecto y del canvas; salir con Ctrl+S, Editar -> Guardar, el botón del panel o el menú
   del canvas (pregunta si guarda). Como el original, Excellon y Gerber guardan en un objeto nuevo (`_edit`). Desvíos: el
   texto del editor de Geometría sale de los contornos de Qt (curvas aplanadas) y no de los puntos de control de FreeType
   (`ParseFont`, que no está portado); las figuras elegidas no se dibujan más gruesas; con una sola herramienta, una
   Geometría se edita sin elegir la fila (en el port todas son multiherramienta); borrar una figura transformada saca sus
   vértices del índice (el original deja vértices viejos que apuntan a nada); al cargar un Gerber el port no modifica el
   objeto original (el original le resta los borrados y le cambia las unidades aunque no se guarde); cambiar el diámetro
   de varias herramientas a la vez las recorre en orden creciente (el original, en el orden de un `set` de Python); el
   panel de Transformaciones del editor de Gerber solo usa el origen o el centro de lo elegido; marcar área no escribe
   las áreas en el canvas; la edición de una apertura desde la tabla del editor de Gerber no está. Falta el editor de
   G-code (`appGCodeEditor`, editar un CNC Job avisa).

## 14. Errores del FlatCAM original que el port reproduce a propósito (corregir al terminar el proyecto)

Regla de la sección 3: se corrigen recién cuando el proyecto esté terminado y verificado el G-code contra el original. Al
encontrar uno nuevo, agregarlo acá con su sección. Ya corregidos a pedido del usuario: los cuatro de HPGL2 (11.12 w).

- **Parsers** (sección 10): `parse_number` con `"L"`; Excellon sin cabecera que pisa el diámetro con uno inventado.
- **Postprocesadores** (11.14): Berta_CNC y el X,Y final; `x_toolchange` en la Y de line_xyz; el paréntesis de "Spindle
  Speed" en Marlin/Repetier/ISEL_ICP; Toolchange_Custom sin `M6` sin X,Y; Marlin_laser con Z inicial; `z_cut` del trabajo en
  `linear_code`. (La cabecera vacía de line_xyz ya se corrigió: 11.14 a.)
- **G-code y CNC Job** (11.12 q): la línea con profundidad múltiple queda invertida o no según la paridad de pasadas.
- **Paint y NCC** (11.9, 11.12 k): rarezas de "Líneas láser" (aperturas de tamaño 0, macros, `linemerge`); herramientas en "V";
  la geometría siempre en `#FFAAAA` y el rótulo en `#7777FF` (`is_theme_black` sin llamar); NCC, Paint y Corte comparan el
  objetivo de la base de datos con un texto y nunca encuentran; en Geometría, la búsqueda en la base toma datos de la última
  herramienta recorrida.
- **Isolation** (11.12 m): tipo `"           Iso"` con espacios, forma "V", `color=#7777FF""`.
- **PCB a 2 caras** (11.12 n): "Elegir agujero" apaga la grilla y no la vuelve a encender.
- **Distancia** (11.12 p): `grid_status_memory` nunca vuelve a `False`; el mensaje con `'%*f'` (ancho en vez de decimales).
- **Selección y menús** (11.12 r, s): la selección por área compara cajas; un clic sobre el objeto elegido lo deselecciona y
  avisa que está elegido; al unir Gerber la geometría de seguimiento es la sólida; Geometría -> Excellon compara el diámetro
  exacto y repite agujeros; una Geometría convertida a Gerber pone todo en la apertura "0".
- **Exportar** (11.12 t): factor 0,03937 en vez de 1/25,4; las ranuras del Excellon (`slot.x` sobre una tupla); el SVG
  de un CNC Job de taladrado falla (`Polygon.svg` con `stroke_color`) y sus límites son los de la última herramienta.
- **Importar SVG** (11.12 u): el inicio de `Line` sin el factor del `viewBox`; trayectos abiertos convertidos en polígonos;
  `matrix()` traspuesta; `rect` con `rx`/`ry` falla; `width`/`height` del `rect` sin factor; `parse_svg_point_list` corta el
  texto equivocado; los `defs` se importan; como Gerber, agujeros como polígonos aparte, líneas que fallan y el `Polygon()`
  vacío inicial.
- **Importar DXF** (11.12 v): una sola línea unida falla; `SOLID`/`TRACE` vacíos; `bulge` ignorado; nudos uniformes;
  splines por puntos de ajuste fallan; filas/columnas de las inserciones antes de escalar; como Gerber, los polígonos fallan.
- **Preferencias** (11.12 x): "Aplicar" no lee el formulario (solo copia las opciones a `current_defaults`); "Cerrar"
  reescribe el formulario con las señales conectadas y deja las preferencias marcadas como editadas; editar el color de
  relleno de la selección (izquierda a derecha) deja solo su alfa (`global_sel_fill[7:9]`); el alfa de derecha a izquierda
  se carga de `global_sel_fill`; los alfa menores que 16 se escriben con un solo dígito (`hex()` sin completar).
- **Unidades** (11.12 x): la grilla se escala dos veces (`scale_defaults` y después `val_x = global_gridx * factor`: 1 mm
  queda en 0,00155 pulgadas); `geometry_cnctooldia` pasa a ser el texto `"[0.0945]"`; las herramientas de una Geometría
  pierden su geometría al convertirla (`solid_geometry` no pasa al diccionario nuevo); ida y vuelta acumula redondeos
  (-2,4 -> -2,4003); con el área de trabajo encendida, `on_toggle_units` llama a `draw_workspace(pagesize=...)`, que no
  tiene ese parámetro, y se corta después de convertir los objetos. El área de trabajo: cambiar el tamaño o la
  orientación dibuja la hoja aunque esté apagada.
- **Panelizar** (sección 13, ítem 5): "Optimizar" queda siempre marcado (`x if x else True`); una Geometría panelizada
  como Gerber da un Gerber vacío (solo copia las herramientas); con la referencia "caja" avisa "No object Box" si el combo
  de la caja está vacío.
- **Marcas de esquina** (sección 13, ítem 5): el número de la apertura nueva sale de `max()` de los números como texto
  ("9" > "10"); con una apertura circular del mismo grosor las marcas llevan esquinas en inglete y con una nueva, biseladas;
  "Reiniciar herramienta" no está conectado.
- **Extraer taladros** (sección 13, ítem 5): el primer agujero de cada herramienta no cuenta para saber si se extrajo
  algo (con un solo pad, o ninguna apertura con dos o más, avisa "No drills extracted"); el nombre es el del Gerber sin lo
  que sigue al último punto (sin punto, vacío); al abrir la herramienta el tipo de agujero no actualiza los campos visibles.
- **Perforar Gerber** (sección 13, ítem 5): con el diámetro fijo, una macro elegida con "otros" hace fallar todo
  (`KeyError: 'size'`, en la GUI solo queda en el registro); en anillo y proporcional un pad de un tipo con su filtro
  apagado se perfora igual por "otros" si ese filtro está activo, y el diámetro de la apertura anterior no se reinicia;
  con el diámetro fijo los agujeros "clear" se buscan en los pads de todas las aperturas, no solo de las elegidas.
- **Restar** (sección 13, ítem 5): Geometría menos Geometría nunca crea nada (`for key, v in tools[tool]` recorre las
  claves de un diccionario como pares: `ValueError`); en Gerber cada figura del sustraendo se resta por separado y se unen
  las restas (con dos superpuestas no se resta lo que tapa cada una); un elemento cubierto entero queda como estaba; de un
  elemento partido en varios polígonos solo entra el primero en la geometría sólida, y si es el primero de la lista falla.
- **Compensación de grabado** (sección 13, ítem 5): con el factor 0 (o una fórmula inválida) avisa y después falla con
  `NameError` (`offset` sin definir); el port avisa y no crea nada (el mismo efecto visible). El grabado engrosa también
  las aperturas de tamaño 0 (regiones) y el tamaño de P, pero no su diámetro; no copia la geometría de seguimiento.
- **Alinear objetos** (sección 13, ítem 5): el giro se omite si el segundo inicio desplazado y su destino coinciden en X
  **o** en Y (debería ser "y"); el ángulo sale de `atan` y no de `atan2` (con giros de más de 90 grados da otro
  cuadrante); si el destino o el inicio tiene la X del centro, divide por cero: el objeto queda desplazado sin girar y la
  herramienta sigue esperando clics (hay que cancelar con el botón derecho); un clic sobre dos figuras agrega dos puntos.
- **Fiduciales** (sección 13, ítem 5): la apertura nueva sale de `max()` de los números como texto (como en Marcas de
  esquina); si las cruces se unen en un solo polígono (puntos muy cerca) falla con `TypeError` y en el modo manual sigue
  esperando clics; "Agregar abertura de máscara" usa los puntos del último "Agregar fiducial" (sin ninguno, crea la
  apertura vacía); la geometría de seguimiento de la cruz guarda listas de dos líneas (el port las aplana) y la de los
  elementos de las aperturas es el centroide del engrosado.
- **Código QR** (sección 13, ítem 5): no se puede cancelar (solo cuenta el botón izquierdo y la tecla no hace nada);
  modifica el Gerber elegido en vez de crear uno; la geometría sólida queda sin unir (los módulos sueltos, y el cobre
  que no contiene entera la máscara queda debajo del código); la apertura nueva tiene ancho y alto agrandados 1 % pero
  el tamaño sin agrandar; un texto que no entra da `ValueError` de la versión 41 (no `DataOverflowError`).
- **Robo de cobre** (sección 13, ítem 5): Escape no cancela la elección del área (llama a `copperfill_tool`, que no
  existe); la lista donde se aplanan los rellenos con líneas (`flat_geometry`) no se vacía nunca, así que cada relleno
  con líneas suma los anteriores; con un solo polígono la caja "mínima" es el polígono y no su envolvente convexa; la
  geometría sólida sale de engrosar 1e-7 un `MultiPolygon` con figuras superpuestas (no las une del todo y el resultado
  cambia con la versión de GEOS); en la máscara, el área galvanizada suma el relleno y la barra sin agrandar, la barra
  que va a una apertura existente entra sin agrandar, la apertura nueva sale de `max()` de los números como texto, un
  relleno que al achicarse (separación negativa) se parte en varios hace fallar todo (`exterior` de un `MultiPolygon`),
  una máscara sin aperturas falla (`max()` vacío), el nombre de un objeto sin punto queda `_plating_mask.<nombre>` y el
  nombre del objeto se registra como archivo reciente (no es un archivo).
- **Verificación de reglas** (sección 13, ítem 5): las reglas cobre-cobre y serigrafía-serigrafía hacen fallar toda la
  verificación (`self.copper_t_cb` / `self.ss_t_cb` no existen; el port no hace nada, el mismo efecto visible), y la de
  máscara depende de la de serigrafía; serigrafía-contorno usa el valor de cobre-contorno; en el tamaño de pistas y de
  agujeros solo entra el primer cobre / Excellon; el informe del anillo mínimo falla si un agujero toca el borde del pad (un
  `Point` en vez de coordenadas); la separación interna con borrados solo mira los sólidos que contienen un borrado.
- **Pasta de soldar** (sección 13, ítem 5): si la boquilla no entra, el pad reusa la línea del pad anterior; el nombre del
  CNC Job se corta en el primer "_".
- **Asistente de PCB** (sección 13, ítem 5): la segunda importación del mismo texto sale vacía (se le pasa el mismo
  `StringIO`, ya leído) y avisa "No geometry found"; las herramientas de un `.INF` se suman a las del anterior (solo se
  vacían al abrir la herramienta); el archivo reciente que registra es el nombre, no la ruta (no se puede abrir).
- **Calibración** (sección 13, ítem 5): calibrar un Excellon falla siempre (`initialize_excellon` copia `obj.drills`, que
  la 2024.4 no tiene); los factores comparan el delta medido con la coordenada del objetivo, así que sin medir nada dan
  factores si los puntos no están redondeados (después de escalar o inclinar); la inclinación en Y suma el origen
  (`bot_right_dy + origin_y`) en vez de restarlo; un punto con la X o la Y del origen divide por cero y corta la cuenta;
  un clic sobre dos figuras agrega dos puntos (también después del cuarto); el botón derecho con cuatro puntos no hace
  nada.
- **Film** (sección 13, ítem 5): el texto de `ET.tostring` (bytes) se pega con `str()`, así que el SVG lleva `b'` y `'`
  como texto dentro del grupo; si el objeto es un solo polígono no hay hijos que recolorear y la negativa mete el
  rectángulo negro dentro del `<path>` (no se dibuja: la negativa sale casi vacía); en `export_svg` cada transformación
  parte de la geometría original (el espejo anula la inclinación y esta a la escala) mientras que la caja las acumula;
  en PNG, si la densidad de la pantalla no es la del campo, le **suma** la relación al factor de escala (en macOS, 72 dpi
  contra 96, la película sale 2,33 veces más grande); el Gerber `_punched` nace en `gerber_def_units` (pulgadas) y al
  pasarlo a mm queda 25,4 veces más grande; el perforado con pads falla (`KeyError`) con cualquier apertura que no sea
  C, R u O (regiones, macros, polígonos), y su chequeo de tamaño usa el ancho y el alto de R/O; la negativa en PDF usa
  siempre mm (compara las unidades con 'INCH'); "sin caja" avisa con la representación del objeto.
- **Autonivelación** (sección 13, ítem 5): en el modo manual el primer punto es `(xmin, xmin)` (repite xmin) y suele
  quedar fuera de la zona (sin celda); el avance manual de GRBL manda `X(5.0,)` porque `step` es una tupla (coma al final
  de la línea); conectar siempre da "conectado" si el puerto abre (un FIXME reemplaza la respuesta por `['ok']`);
  `send_grbl_command` compara `'ok'` con líneas en bytes y falla (`TypeError`) en cuanto GRBL responde, así que el cero de
  los ejes, el avance de dos pasos y el sondeo con GRBL se cortan en el primer comando con respuesta; en el sondeo,
  `output + '\n'` falla también si `output` es una lista; aplicar la nivelación no hace nada.
- **Importar PDF** (sección 13, ítem 4): un trazo de grosor 0 (después de un `Q` el grosor vuelve al guardado, que de
  entrada es 0) da un engrosado vacío y Shapely 1.8 se cae al copiarlo (`segfault`: se cierra FlatCAM entero); `y` no marca
  el trazado como curva; al ancho y al alto de `re` se les suma el desplazamiento; en `cm` el desplazamiento no se escala y
  solo se toma si `b` y `c` son 0; la escala solo si `a` y `d` son distintos de 1; cualquier línea con una `Q` restaura el
  estado; el número de apertura nueva sube una sola vez (puede pisar una existente); `B` sobre un trazado de un punto
  corta todo el archivo (`LineString` sin `try`), igual que una curva sin punto actual; con cualquier stream que no sea
  texto UTF-8 (fuentes, imágenes) no se abre nada; el archivo reciente se registra como Gerber o Excellon.
- **Editor de Geometría** (sección 13, ítem 8): la herramienta Buffer no se puede abrir (`BufferSelectionTool` falla con
  `'%S:'`); Explotar falla con cualquier figura que no sea un polígono (los polígonos abiertos se cargan como anillos) y
  con agujeros; transformar o borrar con la goma no actualiza el índice de vértices (elegir y ajustar a esquinas usan los
  vértices viejos); Alt+Y desplaza en X y Shift+Y inclina en X; elegir con un clic siempre toma la más cercana y la lista
  del panel nunca muestra lo elegido (busca el id en la columna del tipo); unir sin nada elegido agrega una figura vacía;
  con fresado por ascenso, mover o copiar convierte los anillos en polígonos y pierde las figuras múltiples (salvo que
  haya una línea, que deja todo igual); con negrita y cursiva el texto sale en negrita; el menú Objetos queda
  deshabilitado si el objeto elegido no se puede editar.
- **Editor de Excellon** (sección 13, ítem 8): una herramienta sin agujeros hace que los clics no elijan nada; cambiar el
  diámetro con una herramienta sin agujeros ni ranuras se cancela después de quitar lo elegido (se pierde); si se cambian
  todos los agujeros de una herramienta que también tiene ranuras, se borra la herramienta con sus ranuras; copiar suma a
  la cuenta de agujeros y a la de ranuras; Supr borra las herramientas elegidas en la tabla; la caja de un clic sobre un
  agujero está corrida hacia arriba y a la derecha; la tabla se ordena por el texto (también los totales, según la
  traducción); los agujeros de una grilla se cuentan con el último punto del mouse.
- **Editor de Gerber** (sección 13, ítem 8): guarda siempre en `<nombre>_edit`; al terminar una pista su último tramo se
  agrega dos veces; con la grilla apagada la región repite cada punto; en el modo de codo 1 (45°) hacia arriba el codo
  sale para abajo; engrosar también engrosa las líneas de seguimiento (pasan a ser polígonos).
- **HPGL2** (11.12 w): los que quedan (cambiar de pluma con un trazo de un punto, bajar la pluma sin trazo, dibujar sin `SP`).
