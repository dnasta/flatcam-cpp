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
- Nunca pidas ni uses tokens o contraseñas del usuario. En el chat de claude.ai el entorno **puede leer GitHub pero
  no puede hacer `push`** (ver sección 9).

## 4. Estado

| Fase | Contenido | Estado |
|---|---|---|
| 1 | CMake, Doxygen y núcleo de geometría | **Hecha**, verificada en macOS (arm64) y Linux |
| 2 | Parsers de Gerber y Excellon con tests contra archivos reales | **Siguiente** |
| 3 | Herramientas (NCC, Paint, Drilling, PCB a 2 caras, CutOut, Isolation) y G-code | Pendiente |
| 4 | Interfaz Qt6 (ventana principal, árbol de proyecto, canvas, paneles) | Pendiente |
| 5 | Herramientas secundarias y postprocesadores restantes | Pendiente |

Fase 1: 46 tests (GoogleTest) pasan en Linux (GEOS 3.12) y en macOS arm64 (AppleClang, GEOS 3.15). Doxygen genera
la documentación sin warnings propios (queda uno informativo: la traducción al español de Doxygen está vieja).

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
tests/                 GoogleTest; golden_data.h generado; data/ con archivos reales
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
- **Fase 2 y 3:** comparar contra los archivos reales de `tests/data/` (ver sección 8).
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
- NCC y Paint: el diámetro de herramienta, solapamiento, método, margen, "conectar" y "contorno" **no figuran** en
  el G-code. El usuario los va a pasar con capturas de los paneles o con el `.FlatPrj` (es JSON, comprimido con
  LZMA si está activa la opción de guardar comprimido; se abre con `json` o `lzma` de Python).
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

## 10. Siguiente paso: fase 2 (parsers de Gerber y Excellon)

Qué portar y desde dónde (todo en `reference/FlatCAM_2024_4/FlatCAM/`):

| Qué | Origen en Python | Líneas |
|---|---|---|
| Macros de apertura (`ApertureMacro`) | `camlib.py` (clase en la línea ~75) | ~420 |
| Parser de Gerber (`Gerber(Geometry)`) | `appParsers/ParseGerber.py` | 2.536 |
| Parser de Excellon (`Excellon(Geometry)`) | `appParsers/ParseExcellon.py` | 1.597 |
| Objetos con lógica propia (scale, offset, mirror, buffer de aperturas...) | `appObjects/FlatCAMGerber.py`, `FlatCAMExcellon.py` | 1.550 / 1.302 |

Plan: clases `ApertureMacro`, `Gerber` y `Excellon` en `src/parsers/` (o dentro de `src/core/`), derivadas de
`Geometry`, sin Qt. Para el Gerber de KiCad hacen falta: formato `FS`, unidades `MO`, aperturas `AD` (C, R, O, P y
macros `AM` con primitivas de círculo, línea de centro/vector y polígono), regiones `G36/G37`, arcos `G02/G03` con
`G74/G75`, `D01/D02/D03`, polaridad `LP`, y los atributos X2 (se leen y se ignoran). Para el Excellon de KiCad:
`M48`, `METRIC`, `FMAT`, definición de herramientas `TnCd`, coordenadas decimales absolutas y `G05`.
Tests: parsear cada archivo de `tests/data/` y comparar áreas, límites, cantidad de figuras y agujeros contra lo que
calcula el FlatCAM original (se puede correr el Python original en una máquina con PyQt5, Shapely 1.8.4 y las
demás dependencias, o usar los datos del G-code).

Fases 3 a 5: ver la tabla de la sección 4. Dependencias de Python a reemplazar: Shapely→GEOS (hecho), numpy→Eigen
o `std::vector`, vispy/OpenGL→`QOpenGLWidget`/`QGraphicsView`, ezdxf→libdxfrw, rtree→Boost.Geometry,
lxml→pugixml o `QXmlStreamReader`, svglib→parser SVG o QtSvg, reportlab→`QPdfWriter`, ortools→OR-Tools C++,
simplejson→nlohmann/json, qrcode→libqrencode. `winreg` y `tkinter` no se portan.

## 11. Pendientes y preguntas abiertas

1. Valores de NCC y Paint (herramienta, solapamiento, método, margen, conectar, contorno): esperando capturas o
   el `.FlatPrj` del usuario.
2. Re-exportar los 5 `.nc` con FlatCAM 2024.4 y compararlos con los de la 8.994.
3. Confirmar eje y punto del reflejo para la cara inferior, y si `B_Cu` y `Logo` también se reflejan.
4. Tamaño del puente del CutOut (en el recorrido hay 1,6 mm de hueco) y origen del taladro (0,0) en `Alinea`.
5. Confirmar si Isolation entra en el alcance.
6. Elegir la estrategia de render del canvas (`QOpenGLWidget` o `QGraphicsView`) al llegar a la fase 4.
