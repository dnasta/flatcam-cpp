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
(comparte mucho código con NCC) salvo que el usuario diga lo contrario. Salida de **G-code** con los
postprocesadores que usa el usuario (`default` y `GRBL_laser`; luego el resto) y una **GUI Qt6**.

**Fuera de alcance:** los editores de Gerber, Geometría y Excellon (`appEditors`, ~17.600 líneas): el usuario no
los usa; solo carga archivos y el programa genera la geometría y el G-code. A veces edita a mano el G-code para
láser (quitar el movimiento en Z); el postprocesador `GRBL_laser` ya omite la Z, hay que comprobar si cubre ese caso.

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
| 3 | Herramientas (NCC, Paint, Drilling, PCB a 2 caras, CutOut, Isolation) y G-code | **En curso** (hecho: transformaciones de Gerber, postprocesadores, G-code de taladrado y de geometrías) |
| 4 | Interfaz Qt6 (ventana principal, árbol de proyecto, canvas, paneles) | Pendiente |
| 5 | Herramientas secundarias y postprocesadores restantes | Pendiente |

Fase 1: 46 tests (GoogleTest) pasan en Linux (GEOS 3.12) y en macOS arm64 (AppleClang, GEOS 3.15). Doxygen genera
la documentación sin warnings propios (queda uno informativo: la traducción al español de Doxygen está vieja).
Fase 2: 84 tests pasan en Linux (GEOS 3.12) y en macOS arm64 (1,2 s; `B_Cu` se parsea en 0,6 s). Los parsers coinciden
con los originales de FlatCAM sobre los 5 Gerber y el Excellon reales y sobre 7 Excellon sintéticos (ver sección 7).
**Última confirmación en el Mac del usuario (M4, AppleClang, GEOS 3.15.0):** 376 de 376 tests pasan con el commit `95f5c79`
(incluye los desplazamientos de herramienta en texto completo). Falta que confirme el parche `fase3` de documentación y del
golden de Gerber con Shapely 1.8.4.

Fase 3 (hasta ahora): 376 tests en Linux; transformaciones de `Gerber` (24 casos contra el original), postprocesadores
`default` y `GRBL_laser` (224 líneas de G-code idénticas al original) y G-code de taladrado (`CncJob`): idéntico línea por
línea al generador original con el orden del archivo, e igual al `.nc` real del usuario salvo el orden de los agujeros. El G-code de geometrías (`generateFromGeometry`) reproduce
byte por byte `DosCau_PPal-Edge_Cuts.nc` a partir de su propio recorrido.

**Próximos pasos de la fase 3, en orden:** (1) CutOut: generar la geometría con los dos puentes desde `Edge_Cuts.gm1` y
comprobar que el recorrido coincide con el del `.nc` real; (2) NCC y Paint con láser (`GRBL_laser`) contra `B_Cu.nc` y
`B_Logo.nc`; (3) PCB a 2 caras (`Alinea`); (4) Isolation si el usuario lo confirma. Dato del `.nc` del contorno: tiene
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
src/cam/                Generación de G-code (sin Qt)
  PyNumber.*             Número que recuerda si era int o float, para imprimir como `str()` de Python
  Preprocessor.*         Interfaz `Preprocessor` y los postprocesadores `default` y `GRBL_laser`
  PathOptimizer.*        Orden de los agujeros (vecino más cercano + 2-opt; reemplaza a OR-Tools)
  CncJob.*               G-code de taladrado (`excellon_tool_gcode_gen`) y de geometrías (`geometry_tool_gcode_gen`, `linear2gcode`), y archivo exportado
tests/                 GoogleTest; golden_*.h generados; data/ con archivos reales; tools/ con los oráculos
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

Hallazgos que condicionan el trabajo:

- Los Gerber son de KiCad 6: `FSLAX46Y46` (4.6, ceros iniciales omitidos), `MOMM`, polaridad `LPD`, atributos X2
  (`%TF`, `%TA`, `%TO`, `%TD`), arcos multicuadrante (`G75`, `G03`), regiones y macros de apertura.
- Postprocesadores usados: **`GRBL_laser`** (B_Cu y Logo: avance 500, rápidos 1500, Z Focus 2,0, potencia 300, sin
  movimientos en Z) y **`default`** (Mach3; Drill, Alinea, Edge_Cuts).
- Taladros reflejados en X (el `.drl` trae `X-26.239` y el G-code `X26.2390`).
- `Alinea` no tiene `.drl`: lo genera la herramienta "PCB a 2 caras" (4 agujeros en las esquinas, más uno en 0,0
  cuyo origen hay que aclarar).
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
- Claude Code lee `CLAUDE.md`, no `AGENTS.md`; por eso `CLAUDE.md` solo importa a este archivo con `@AGENTS.md`.

## 10. Fase 2 hecha y siguiente paso: fase 3

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

## 11. Pendientes y preguntas abiertas

1. Confirmar si la 2024.4 genera el mismo G-code que la 8.994 con el mismo proyecto. **Evidencia a favor:** el taladro (todo
   salvo el orden de los agujeros) y el contorno (byte por byte) coinciden con el generador de la 2024.4. Falta NCC/Paint y
   `Alinea`. El usuario puede exportar de nuevo desde la 2024.4 a `tests/data/2024_4/` para cerrarlo.
2. Reflejo de `B_Cu` (simétrico en sus límites): confirmar con la geometría en la fase 3.
3. Taladro `(0, 0)` en `Alinea` (el G-code taladra 5 posiciones, el job cuenta 4): aclarar leyendo `ToolDblSided.py`.
4. Confirmar si Isolation entra en el alcance.
5. **Orden de los taladros (decidido).** El `.nc` de referencia lo ordenó OR-Tools (`excellon_optimization_type = B`). El usuario
   eligió un **optimizador propio** (`PathOptimizer`: 2-opt desde el vecino más cercano y desde el orden del archivo, el
   más corto gana). Los tests comparan el G-code como conjunto (mismos agujeros, profundidades y líneas fijas) y exigen
   que el recorrido no supere en más de 10 % al de OR-Tools. OR-Tools queda como posible opción futura.
6. Elegir la estrategia de render del canvas (`QOpenGLWidget` o `QGraphicsView`) al llegar a la fase 4.
