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

**Fuera de alcance:** los editores de Gerber, Geometría y Excellon (`appEditors`, ~17.600 líneas): el usuario no
los usa; solo carga archivos y el programa genera la geometría y el G-code. A veces edita a mano el G-code para
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
- Nunca pidas ni uses tokens o contraseñas del usuario. En el chat de claude.ai el entorno **puede leer GitHub pero
  no puede hacer `push`** (ver sección 9).

## 4. Estado

| Fase | Contenido | Estado |
|---|---|---|
| 1 | CMake, Doxygen y núcleo de geometría | **Hecha**, verificada en macOS (arm64) y Linux |
| 2 | Parsers de Gerber y Excellon con tests contra archivos reales | **Hecha** |
| 3 | Herramientas (NCC, Paint, Drilling, PCB a 2 caras, CutOut, Isolation) y G-code | **Hecha**: transformaciones de Gerber, postprocesadores, G-code de taladrado y de geometrías, CutOut, capa de láser, Paint, NCC, PCB a 2 caras e Isolation |
| 4 | Interfaz Qt6 (ventana principal, árbol de proyecto, canvas, paneles) | **En curso**: ventana principal idéntica a la original (generada desde la interfaz Python real), widgets de `GUIElements`, canvas `QOpenGLWidget` (ejes, grilla, HUD, zoom, paneo y dibujo de objetos con buffers de vértices), abrir Gerber y Excellon (menú, barra, arrastrar y soltar, línea de comandos) con el árbol del Proyecto; Propiedades de Gerber y Excellon (paneles generados + `build_ui`); paneles de NCC, Paint, Perforación y Corte en la pestaña Herramienta (generados). **Perforación completa:** objeto → tabla de herramientas → parámetros por herramienta → CNC Job (objeto, dibujo y Propiedades) → "Guardar código CNC"; desde la interfaz reproduce el `.nc` de la 2024.4 del usuario (salvo fecha y orden de agujeros). Faltan: la lógica de NCC, Paint y Corte, el objeto Geometría y las preferencias del usuario |
| 5 | Herramientas secundarias y postprocesadores restantes | Pendiente |

Fase 1: 46 tests (GoogleTest) pasan en Linux (GEOS 3.12) y en macOS arm64 (AppleClang, GEOS 3.15). Doxygen genera
la documentación sin warnings propios (queda uno informativo: la traducción al español de Doxygen está vieja).
Fase 2: 84 tests pasan en Linux (GEOS 3.12) y en macOS arm64 (1,2 s; `B_Cu` se parsea en 0,6 s). Los parsers coinciden
con los originales de FlatCAM sobre los 5 Gerber y el Excellon reales y sobre 7 Excellon sintéticos (ver sección 7).
**Última confirmación en el Mac del usuario (M4, AppleClang):** 517 tests (502 del núcleo y 14 de la GUI pasan; 1, la lenta del NCC
del dosificador, queda saltada a propósito), con GEOS 3.15.0 (Homebrew) y con GEOS 3.12 (micromamba, solo el núcleo), y Qt 6.12.

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
  Gerber.*               Parser de Gerber RS-274X (aperturas, trazos, arcos, regiones, polaridad)
  Excellon.*             Parser de Excellon (herramientas, agujeros, ranuras) y sus transformaciones
  (Gerber.* también trae scale, offset, mirror, rotate, skew, buffer y getBoundingBox)
src/tools/              Herramientas de FlatCAM (sin Qt)
  Paint.*                Pintado de polígonos con el método estándar (`ToolPaint.py`)
  Ncc.*                  Limpieza de cobre de toda la placa, método estándar (`ToolNCC.py`)
  DoubleSided.*          PCB a 2 caras: agujeros de alineación con sus reflejos (`ToolDblSided.py`)
  CutOut.*               Corte de contorno con puentes: libre, rectangular y puente manual (`ToolCutOut.py`)
  Isolation.*            Aislamiento: pasadas separadas, combinadas y mecanizado de resto (`ToolIsolation.py`)
src/cam/                Generación de G-code (sin Qt)
  PyNumber.*             Número que recuerda si era int o float, para imprimir como `str()` de Python
  Preprocessor.*         Interfaz `Preprocessor` y los postprocesadores `default` y `GRBL_laser`
  PathOptimizer.*        Orden de los agujeros (vecino más cercano + 2-opt; reemplaza a OR-Tools)
  EndpointStorage.*      Almacén de líneas por extremos con `nearest` y borrado (`FlatCAMRTreeStorage`, con una grilla)
  PolygonClearing.*      `clear_polygon` (espiral) y `paint_connect` del original
  CncJob.*               G-code de taladrado (`excellon_tool_gcode_gen`) y de geometrías (`geometry_tool_gcode_gen`, `linear2gcode`), y archivo exportado
src/gui/               Interfaz Qt6 (biblioteca flatcam_gui y ejecutable FlatCAM; namespace flatcam::gui)
  generated/MainGUIBase.*  Ventana principal GENERADA por tools/gui_dump/gen_cpp.py (no editar): menús, acciones,
                         barras, pestañas, barra de estado y consola, con los nombres de atributo del original
  MainGUI.*              Lógica de la ventana (deriva de MainGUIBase): canvas, coordenadas, botones de la barra de estado
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
                         tools/gui_dump/dump_panels.py + gen_cpp.py --panels (las herramientas después de su run() original)
  objects/ObjectUIs.*    build_ui de Gerber (tabla de aperturas) y Excellon (tabla de herramientas con totales)
  tools/ToolPanels.*     Paneles de NCC, Paint y Corte (por ahora solo interfaz); lista de postprocesadores del combo
  tools/DrillingTool.*   Perforación completa (port de ToolDrilling: tabla, almacén de parámetros con el tipo de Python de
                         cada valor, formulario, postprocesador, generar el CNC Job)
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

Requisitos: compilador C++17, CMake >= 3.20, GEOS >= 3.11, opcional Doxygen y GoogleTest (si falta, CMake lo baja).

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

**Interfaz (Qt6):** `brew install qt` (Qt 6.12 en el Mac del usuario); CMake la encuentra solo y arma `build/src/gui/FlatCAM.app`
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
- `tests/tools/oracle_preprocessors.py` genera `tests/golden_preprocessors.h`: ejecuta los postprocesadores originales con 8
  juegos de parámetros y guarda el texto de cada método (más una lista de números para `pyFloatRepr`).
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
simplejson→nlohmann/json, qrcode→libqrencode. `winreg` y `tkinter` no se portan.

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
9. **Métodos que faltan en Paint y NCC** (hoy devuelven error o no existen): semilla (`clear_polygon2`), líneas (`clear_polygon3`),
   combinado, mecanizado de resto, selección por área y por objeto de referencia, aislamiento previo (`isotooldia`). El usuario solo
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
13. **GUI: pendientes, en orden:** ~~dibujar Gerber y Excellon~~, ~~abrir archivos y el árbol del Proyecto~~, ~~Propiedades de
    Gerber y Excellon~~ y ~~los paneles de las cuatro herramientas~~ (hechos; falta menú contextual del árbol, mostrar/ocultar y
    borrar). ~~Perforación → CNC Job → exportar~~ (hecho). Sigue: Corte, Paint y NCC (crean objetos Geometría: falta el objeto
    Geometría con sus Propiedades y su "Generar objeto CNCJob"), reflejar (Transformaciones o PCB a 2 caras), cargar las preferencias del usuario (hoy los paneles tienen los valores de fábrica:
    por ejemplo NCC 40 % y "Semilla" en vez de sus 80 % y "Estándar"); pestaña Propiedades de cada objeto (`ObjectUI.py`, generada igual que la ventana); paneles de Perforación, Corte, Paint y NCC
    (generados desde `ToolDrilling`, `ToolCutOut`, `ToolPaint`, `ToolNCC` con sus valores de fábrica) y su lógica sobre el núcleo;
    CNC Job y exportar `.nc`; reflejar (herramienta PCB a 2 caras / Transformaciones).

## 12. Retomar el trabajo en Claude Code (CLI)

Esta sección es para empezar una sesión nueva sin haber visto la conversación anterior. El estado real es lo que está en GitHub
(`https://github.com/dnasta/flatcam-cpp`, rama `main`); lo que sigue lo resume.

### 12.1 Dónde estamos (al cerrar la sesión del chat)

- **517 tests** (503 del núcleo y 14 de la GUI): 516 corren siempre y pasan; 1 es lenta (`NccEndToEnd`, ~100 s) y queda saltada salvo con `FLATCAM_SLOW_TESTS=1`.
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
   `ctest --test-dir build --output-on-failure`. Tienen que dar 517 tests, 1 saltado.
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
- Un test que compara un texto consigo mismo siempre pasa: una vez se coló una aserción así; revisar que cada `EXPECT` pueda fallar.

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
