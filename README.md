# FlatCAM-cpp

Port a **C++17 / Qt6** de [FlatCAM](http://flatcam.org) 2024.4 beta (CAM 2D para fabricar PCB con CNC o
láser), pensado para compilar y ejecutarse en **macOS (Apple Silicon)**, **Windows** y **Linux**.

El código fuente original en Python está en `reference/FlatCAM_2024_4/` y sirve como referencia: cada
clase del port indica en su documentación de qué archivo y función de Python proviene.

## Estado: versión 1.0.0

El port está completo: todo lo que tiene FlatCAM 2024.4 (abrir y exportar Gerber, Excellon, G-code, SVG, DXF, PDF,
HPGL2 e imágenes; Isolation, NCC, Paint, Perforación, Corte, PCB a 2 caras y las demás herramientas; los editores de
Geometría, Excellon, Gerber y G-code; los 20 postprocesadores; las Preferencias; la consola Tcl con sus 65 comandos;
abrir y guardar proyectos `.FlatPrj` compatibles con el original), en los 9 idiomas del original (alemán, inglés,
español, francés, italiano, portugués de Brasil, rumano, ruso y turco: de fábrica el del sistema, y se cambia en
Preferencias -> General -> Idiomas). Ver `docs/NOTAS_1.0.0.md`.

Cada parte se verificó contra el código **original** con oráculos en Python y, donde había, contra el G-code real
generado por FlatCAM 2024.4. Los errores del FlatCAM original (lista en `AGENTS.md`, sección 14) están **corregidos**
en la aplicación; para comportarse exactamente como el original (por ejemplo, para comparar el G-code) se puede
arrancar con `--original-bugs` o con la variable `FLATCAM_ORIGINAL_BUGS=1`.

## Estructura

```
src/core/        Núcleo sin dependencias de Qt (Shape, Geometry, ArcMath, AffineTransform, Types)
src/core/internal/  Detalles internos de GEOS (no forman parte de la API)
src/parsers/     Lectores de Gerber, Excellon, SVG, DXF, PDF, HPGL2, imágenes y fuentes
src/tools/       Herramientas (NCC, Paint, CutOut, Isolation, PCB a 2 caras, Panelizar, Film, ...)
src/editors/     Modelos de los editores de Geometría, Excellon y Gerber (sin Qt)
src/io/          Proyectos .FlatPrj y exportación (Gerber, Excellon, SVG, DXF)
src/cam/         Generación de G-code y postprocesadores
src/gui/         Interfaz Qt6: ventana principal, canvas OpenGL, widgets de FlatCAM; generated/ sale de tools/gui_dump/
tools/gui_dump/  Generadores que construyen la interfaz ORIGINAL con PyQt5 y la vuelcan a C++ (y extraen íconos y textos)
tests/           Tests unitarios (GoogleTest); los golden_*.h tienen valores de referencia de Shapely y de los
                 parsers originales de FlatCAM
tests/data/      Gerber, Excellon y G-code reales de un proyecto (más parámetros en PARAMETROS.md)
tests/tools/     Oráculos: ejecutan el FlatCAM original y regeneran esos valores de referencia
reference/       Fuentes originales de FlatCAM 2024.4 (solo lectura)
```

## Compilar

Requisitos: compilador C++17, CMake 3.20 o superior, GEOS 3.11 o superior, Qt 6.2 o superior para la interfaz y,
opcionalmente, Doxygen. Sin Qt6 se compila solo el núcleo (y sus tests). La interfaz usa QtWidgets, QtOpenGLWidgets y
QtSvg (Film); QtSerialPort es opcional (GRBL en la autonivelación). La consola Tcl necesita Tcl 8.6 o 9. FreeType es
opcional: sin ella la herramienta Texto del editor de Geometría no puede escribir textos.

**macOS (Homebrew)**
```zsh
brew install cmake ninja geos doxygen googletest qt tcl-tk freetype
cmake -S . -B build -G Ninja
cmake --build build
ctest --test-dir build --output-on-failure
open build/src/gui/FlatCAM.app      # la aplicación
```

Para llevar la aplicación a otra Mac (sin Homebrew): `tools/package_macos.sh` arma `build/dist/FlatCAM.app`
autocontenido (Qt, GEOS, FreeType, Tcl con su biblioteca de scripts) y `build/dist/FlatCAM-<versión>-macOS-arm64.dmg`.
La firma es ad hoc: la primera vez macOS pide abrirla con clic derecho -> Abrir (no está notarizada).

**Linux (Ubuntu/Debian)**
```bash
sudo apt install build-essential cmake ninja-build libgeos-dev liblzma-dev doxygen libgtest-dev qt6-base-dev qt6-svg-dev qt6-serialport-dev tcl-dev libfreetype-dev
cmake -S . -B build -G Ninja
cmake --build build
ctest --test-dir build --output-on-failure
```

**Windows (vcpkg + Visual Studio)**
```bat
vcpkg install geos gtest liblzma qtbase qtsvg qtserialport tcl freetype
cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE=%VCPKG_ROOT%\scripts\buildsystems\vcpkg.cmake
cmake --build build --config Release
ctest --test-dir build -C Release --output-on-failure
```

Si GoogleTest no está instalado, CMake lo descarga solo. Opciones: `-DFLATCAM_BUILD_TESTS=OFF`,
`-DFLATCAM_BUILD_DOCS=OFF`, `-DFLATCAM_BUILD_GUI=OFF`.

## Documentación

Todo el código está documentado con Doxygen (en español). Para generarla:

```zsh
cmake --build build --target docs
open build/doxygen/html/index.html      # en Linux: xdg-open
```

## Cómo se verifica

Los valores esperados de los tests no se escribieron a mano. `tests/tools/gen_golden.py` los calcula con Shapely y
numpy (buffers, uniones, transformaciones, arcos) y los oráculos de `tests/tools/` (`oracle_*.py`) ejecutan el código
**original** de FlatCAM (parsers, herramientas, postprocesadores, editores, consola Tcl, la interfaz con PyQt5) sobre los
archivos de `tests/data/`. Los tests corren con los errores del original activados (así se comparan con él) y cada
corrección tiene su test aparte (`*Fixed.*`). El CI (GitHub Actions) compila y prueba en Linux y Windows.

## Licencia

MIT. Es una obra derivada de FlatCAM (MIT): ver `LICENSE` y `NOTICE` para los avisos de copyright de los
autores originales.
