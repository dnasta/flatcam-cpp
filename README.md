# FlatCAM-cpp

Port a **C++17 / Qt6** de [FlatCAM](http://flatcam.org) 2024.4 beta (CAM 2D para fabricar PCB con CNC o
láser), pensado para compilar y ejecutarse en **macOS (Apple Silicon)**, **Windows** y **Linux**.

El código fuente original en Python está en `reference/FlatCAM_2024_4/` y sirve como referencia: cada
clase del port indica en su documentación de qué archivo y función de Python proviene.

## Estado

| Fase | Contenido | Estado |
|---|---|---|
| 1 | CMake, Doxygen y núcleo de geometría (`Shape`, `Geometry`, `ArcMath`, `AffineTransform`) | Hecha |
| 2 | Parsers de Gerber y Excellon, con tests contra archivos reales | Hecha |
| 3 | Herramientas (Isolation, NCC, Paint, Drilling, CutOut, PCB a 2 caras) y salida de G-code | Hecha |
| 4 | Interfaz Qt6 (ventana principal, árbol de proyecto, canvas `QOpenGLWidget`, paneles) | En curso: ventana principal idéntica a la original y canvas |
| 5 | Herramientas secundarias y postprocesadores restantes | Pendiente |

Quedan fuera del alcance los editores de Gerber, Geometría y Excellon.

## Estructura

```
src/core/        Núcleo sin dependencias de Qt (Shape, Geometry, ArcMath, AffineTransform, Types)
src/core/internal/  Detalles internos de GEOS (no forman parte de la API)
src/parsers/     Lectores de Gerber, Excellon y macros de apertura
src/tools/       Herramientas (NCC, Paint, CutOut, Isolation, PCB a 2 caras)
src/cam/         Generación de G-code y postprocesadores
src/gui/         Interfaz Qt6: ventana principal, canvas OpenGL, widgets de FlatCAM; generated/ sale de tools/gui_dump/
tools/gui_dump/  Generadores que construyen la interfaz ORIGINAL con PyQt5 y la vuelcan a C++ (y extraen íconos y textos)
tests/           Tests unitarios (GoogleTest); los golden_*.h tienen valores de referencia de Shapely y de los
                 parsers originales de FlatCAM
tests/data/      Gerber, Excellon y G-code reales de un proyecto (más parámetros en PARAMETROS.md)
tests/tools/     Script que regenera esos valores de referencia
reference/       Fuentes originales de FlatCAM 2024.4 (solo lectura)
```

## Compilar

Requisitos: compilador C++17, CMake 3.20 o superior, GEOS 3.11 o superior, Qt 6.2 o superior para la interfaz y,
opcionalmente, Doxygen. Sin Qt6 se compila solo el núcleo (y sus tests).

**macOS (Homebrew)**
```zsh
brew install cmake ninja geos doxygen googletest qt
cmake -S . -B build -G Ninja
cmake --build build
ctest --test-dir build --output-on-failure
open build/src/gui/FlatCAM.app      # la aplicación
```

**Linux (Ubuntu/Debian)**
```bash
sudo apt install build-essential cmake ninja-build libgeos-dev doxygen libgtest-dev qt6-base-dev
cmake -S . -B build -G Ninja
cmake --build build
ctest --test-dir build --output-on-failure
```

**Windows (vcpkg + Visual Studio)**
```bat
vcpkg install geos gtest qtbase
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
numpy (buffers, uniones, transformaciones, arcos) y los oráculos `oracle_gerber.py` y `oracle_excellon.py` ejecutan
los parsers **originales** de FlatCAM sobre los archivos de `tests/data/`. Así se comprueba que el port reproduce
los resultados del programa original.

## Licencia

MIT. Es una obra derivada de FlatCAM (MIT): ver `LICENSE` y `NOTICE` para los avisos de copyright de los
autores originales.
