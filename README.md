<p align="right"><a href="README.md">🇪🇸 Español</a> · <a href="README.en.md">🇬🇧 English</a> · <a href="README.de.md">🇩🇪 Deutsch</a> · <a href="README.fr.md">🇫🇷 Français</a> · <a href="README.it.md">🇮🇹 Italiano</a> · <a href="README.pt-BR.md">🇧🇷 Português</a> · <a href="README.ro.md">🇷🇴 Română</a> · <a href="README.ru.md">🇷🇺 Русский</a> · <a href="README.tr.md">🇹🇷 Türkçe</a></p>

<p align="center">
  <img src="src/gui/resources/images/flatcam_icon256.png" width="140" alt="FlatCAM">
</p>

<h1 align="center">FlatCAM-cpp</h1>

<p align="center">
  Port nativo de <strong>FlatCAM 2024.4</strong> a C++17, Qt 6 y GEOS para fabricar placas de circuito impreso con una CNC o un láser.
</p>

<p align="center">
  <a href="https://github.com/dnasta/flatcam-cpp/releases/latest"><img alt="Release" src="https://img.shields.io/badge/release-v1.0.0-b4771b"></a>
  <a href="LICENSE"><img alt="licencia" src="https://img.shields.io/badge/licencia-MIT-2f6f4f"></a>
  <a href="https://github.com/dnasta/flatcam-cpp/actions/workflows/ci.yml"><img alt="CI" src="https://github.com/dnasta/flatcam-cpp/actions/workflows/ci.yml/badge.svg"></a>
  <img alt="macOS" src="https://img.shields.io/badge/macOS-15%2B%20arm64-16202b">
  <img alt="Windows" src="https://img.shields.io/badge/Windows-10%20%7C%2011-0078D4">
  <img alt="Linux" src="https://img.shields.io/badge/Linux-deb%20%7C%20rpm%20%7C%20AppImage-FCC624">
  <img alt="C++" src="https://img.shields.io/badge/C%2B%2B-17-00599C">
  <img alt="Qt" src="https://img.shields.io/badge/Qt-6-41CD52">
</p>

<p align="center">
  <a href="https://github.com/dnasta/flatcam-cpp/releases/latest"><strong>⬇ Descargar v1.0.0</strong></a> ·
  <a href="docs/NOTAS_1.0.0.md"><strong>📝 Notas de la versión</strong></a> ·
  <a href="https://github.com/dnasta/flatcam-cpp/issues"><strong>🐞 Informar un problema</strong></a>
</p>

---

## Índice

- [Qué es FlatCAM-cpp](#qué-es-flatcam-cpp)
- [Capturas](#capturas)
- [Características](#características)
- [Idiomas](#idiomas)
- [Formatos soportados](#formatos-soportados)
- [Instalación](#instalación)
- [Inicio rápido](#inicio-rápido)
- [Línea de comandos y automatización](#línea-de-comandos-y-automatización)
- [Compilar desde el código fuente](#compilar-desde-el-código-fuente)
- [Tests y verificación](#tests-y-verificación)
- [Estructura del proyecto](#estructura-del-proyecto)
- [Licencia, autoría y créditos](#licencia-autoría-y-créditos)

---

## Qué es FlatCAM-cpp

FlatCAM convierte los archivos de diseño de una PCB —capas **Gerber** y taladros **Excellon**— en **G-code** para una fresadora CNC, un router o un láser: aislamiento de pistas, limpieza de cobre, perforación, corte del contorno y mucho más.

FlatCAM-cpp es un port **completo** de FlatCAM 2024.4 (Python + PyQt5) a **C++17**, con **Qt 6** para la interfaz y **GEOS** para la geometría. La interfaz es la del original —se generó a partir de la interfaz original real—, con los mismos menús, paneles, atajos y preferencias, y lee y escribe los mismos proyectos `.FlatPrj`.

**En una frase:** abrís tu placa, elegís la herramienta, mirás los recorridos y guardás el G-code.

### Por qué un port nativo

| Aspecto | FlatCAM 2024.4 (Python) | FlatCAM-cpp |
|---|---|---|
| Ejecución | Python 3 + PyQt5 + Shapely + numpy + vispy | Ejecutable nativo |
| Instalación | Instalar Python y sus dependencias | Instalador para macOS, Windows y Linux |
| Geometría | Shapely | GEOS (API C reentrante) |
| Interfaz | Qt 5 | Qt 6 con canvas OpenGL |
| Velocidad (limpieza de cobre de una placa real) | 567 s | 56 s |
| Errores conocidos del original | Presentes | Más de 200 corregidos (`--original-bugs` vuelve al comportamiento original) |

---

## Capturas

<p align="center">
  <img src="docs/images/interfaz.png" alt="Interfaz principal" width="90%">
  <br><em>Interfaz principal: el cobre de una placa real con sus propiedades.</em>
</p>

<p align="center">
  <img src="docs/images/ncc.png" alt="Limpieza de cobre" width="90%">
  <br><em>Herramienta de limpieza de cobre (NCC).</em>
</p>

<p align="center">
  <img src="docs/images/cncjob.png" alt="G-code" width="90%">
  <br><em>G-code de la limpieza de cobre, el taladrado y el corte (en amarillo, los viajes).</em>
</p>

<p align="center">
  <img src="docs/images/deutsch.png" alt="Interfaz en alemán" width="90%">
  <br><em>La misma interfaz en alemán, con la herramienta de perforación.</em>
</p>

---

## Características

### Archivos

- **Gerber RS-274X**: aperturas C/R/O/P, macros `%AM`, regiones `G36/G37`, arcos `G02/G03`, polaridad y atributos X2.
- **Excellon**: herramientas, agujeros, ranuras (`G85` y ruteo), formatos y ceros, archivos sin cabecera.
- **G-code**: abrir, ver, editar y exportar.
- **SVG, DXF, PDF, HPGL2 e imágenes** (PNG, JPG, BMP), como Geometría o como Gerber.
- **Proyectos `.FlatPrj`** del FlatCAM en Python (comprimidos con LZMA, también de cientos de MB); el FlatCAM original abre los que guarda el port.
- **Exportar** Gerber, Excellon, SVG, DXF, PNG y PDF.

### Herramientas CAM

- **Aislamiento**: pasadas, solape, fresa en V, combinado, mecanizado de resto, por área o por polígono.
- **Limpieza de cobre (NCC)** y **Paint**: métodos Estándar, Semilla, Líneas y Combo, mecanizado de resto, área y objeto de referencia; "Líneas láser".
- **Perforación**: orden optimizado, profundidad múltiple, ranuras como agujeros; fresado de agujeros y ranuras.
- **Corte del contorno**: libre o rectangular, puentes, "mouse bites" y puentes manuales.
- **PCB a 2 caras**: reflejo por punto, caja o agujero, y agujeros de alineación.
- **Y además**: Panelizar, Film, Robo de cobre, Pasta de soldar, Código QR, Fiduciales, Marcas de esquina, Extraer taladros, Perforar Gerber, Restar, Óptimo, Verificación de reglas, Calibración, Compensación de grabado, Alinear objetos, Invertir Gerber, Calculadoras, Transformaciones, Distancia y Autonivelación (sondeo con GRBL, MACH3/4 y LinuxCNC).
- **Base de datos de herramientas** y **áreas de exclusión**.

### G-code

- Los **20 postprocesadores** del original (`default`, `GRBL_laser`, `grbl_11`, `Marlin`, `Repetier`, `Roland_MDX_20`, `hpgl`, `ISEL_CNC`, `Toolchange_*`, `Z_laser`…), idénticos línea por línea.
- Profundidad múltiple, corte extra, fragmentos de código de inicio y de fin, milímetros o pulgadas.

### Editores

- **Geometría** (con textos escritos con las fuentes del sistema), **Excellon**, **Gerber** y **G-code**.

### Interfaz

- La de FlatCAM 2024.4: menús, barras, pestañas Proyecto / Propiedades / Herramienta y los mismos atajos.
- Canvas OpenGL con zoom, paneo, grilla, ajuste a la grilla, HUD y área de trabajo.
- Preferencias (485 opciones), niveles Básico y Avanzado, milímetros o pulgadas, archivos recientes y marcadores.

### Automatización

- **Consola Tcl** con el intérprete Tcl real y los **65 comandos** del original; scripts `.tcl`.

---

## Idiomas

La interfaz está en los **9 idiomas** del original, con sus mismas traducciones: alemán, inglés, español, francés, italiano, portugués de Brasil, rumano, ruso y turco.

- Arranca en el **idioma del sistema** (si no está entre los nueve, en inglés).
- Para cambiarlo: **Preferencias → General →** elegir el idioma **→ Aplicar idioma** (la aplicación se reinicia).
- Para forzar uno al arrancar: variable de entorno `FLATCAM_LANGUAGE` (por ejemplo `FLATCAM_LANGUAGE=de`).

---

## Formatos soportados

| Formato | Abrir / importar | Exportar | Notas |
|---|:---:|:---:|---|
| Gerber RS-274X | ✅ | ✅ | Aperturas, macros, regiones, polaridad |
| Excellon | ✅ | ✅ | Agujeros y ranuras |
| G-code | ✅ | ✅ | 20 postprocesadores |
| SVG | ✅ | ✅ | Como Geometría o Gerber |
| DXF | ✅ | ✅ | DXF ASCII |
| PDF | ✅ | ✅ | Importación vectorial; impresión y film |
| HPGL2 | ✅ | — | El postprocesador `hpgl` genera código para plotters |
| PNG / JPG / BMP | ✅ | ✅ | Vectorización por umbral; captura del canvas y film |
| `.FlatPrj` | ✅ | ✅ | Compatibles con el FlatCAM en Python |

---

## Instalación

Los instaladores están en la página de [**Releases**](https://github.com/dnasta/flatcam-cpp/releases/latest).

### macOS

- **macOS 15 o posterior**, con **Apple Silicon** (M1 a M4).
- Abrí `FlatCAM-1.0.0-macOS-arm64.dmg` y arrastrá **FlatCAM** a *Aplicaciones*.
- La primera vez: clic derecho → **Abrir** (la firma es ad hoc, no está notarizada).
- Incluye Qt, GEOS, FreeType y Tcl: no hace falta Homebrew.

### Windows

- **Windows 10 (1809 o posterior) y Windows 11**, de 64 bits.
- `FlatCAM-1.0.0-Windows-x64-setup.exe` (instalador) o `FlatCAM-1.0.0-Windows-x64.zip` (portable, sin instalar).
- Si aparece "Windows protegió su PC": **Más información → Ejecutar de todos modos** (no está firmado con certificado).

### Linux

| Distribución | Archivo | Instalar |
|---|---|---|
| Ubuntu 24.04+, Debian 13, Linux Mint 22 | `flatcam-cpp_1.0.0_amd64.deb` | `sudo apt install ./flatcam-cpp_1.0.0_amd64.deb` |
| Fedora 41+ | `flatcam-cpp-1.0.0-1.x86_64.rpm` | `sudo dnf install ./flatcam-cpp-1.0.0-1.x86_64.rpm` |
| Otras (glibc 2.39+) | `FlatCAM-1.0.0-Linux-x86_64.AppImage` | `chmod +x FlatCAM-*.AppImage && ./FlatCAM-*.AppImage` |

Con el `.deb` o el `.rpm`, FlatCAM aparece en el menú de aplicaciones y se abre con el comando `flatcam`.

---

## Inicio rápido

```text
1. Archivo → Abrir → Abrir Gerber      cobre y contorno de la placa
   Archivo → Abrir → Abrir Excellon    taladros
2. Elegí el cobre en Proyecto → Propiedades → Enrutamiento de aislamiento (o Herramienta NCC)
   diámetro de la fresa, pasadas y solape → Generar Geometría
3. En la Geometría nueva: Propiedades → Z de corte, avance → Generar objeto CNCJob
4. En el CNC Job: Guardar código CNC
5. Taladros: Herramienta de Perforación → Generar objeto CNCJob
6. Contorno: Herramienta de Corte → puentes → Generar Geometría
```

> **Antes de mecanizar:** revisá el G-code, el cero de la pieza y la altura Z. Un error de 0,1 mm puede arruinar la placa.

---

## Línea de comandos y automatización

```bash
FlatCAM placa-B_Cu.gbr placa-Edge_Cuts.gbr placa.drl   # abre los archivos (según la extensión)
FlatCAM proyecto.FlatPrj                                # abre un proyecto
FlatCAM script.tcl                                      # abre y ejecuta un script Tcl
FlatCAM --original-bugs                                 # se comporta exactamente como el FlatCAM original
```

El ejecutable es `FlatCAM` (en macOS, `/Applications/FlatCAM.app/Contents/MacOS/FlatCAM`), `FlatCAM.exe` en Windows y `flatcam` en Linux.

### Consola Tcl

Se abre con **Línea de Comando** (tecla `S`). Ejemplo:

```tcl
open_gerber placa-B_Cu.gbr -outname cobre
isolate cobre -dia 0.2 -passes 2 -overlap 10 -combine True -outname cobre_iso
cncjob cobre_iso -dia 0.2 -z_cut -0.1 -z_move 2 -feedrate 120 -pp default -outname cobre_cnc
write_gcode cobre_cnc cobre.nc

open_excellon placa.drl -outname taladros
drillcncjob taladros -drillz -1.8 -travelz 2 -feedrate_z 100 -pp default -outname taladros_cnc
write_gcode taladros_cnc taladros.nc
```

`help` lista los 65 comandos y `help <comando>` explica cada uno.

---

## Compilar desde el código fuente

| Dependencia | Uso |
|---|---|
| Compilador C++17 y CMake 3.20+ | AppleClang, GCC o MSVC 2022 |
| GEOS 3.11+ | Geometría |
| Qt 6.4+ | Interfaz (Widgets, OpenGLWidgets, Svg, PrintSupport; SerialPort opcional) |
| Tcl 8.6 o 9 | Consola Tcl |
| liblzma | Proyectos `.FlatPrj` comprimidos |
| FreeType (opcional) | Textos con fuentes en el editor de Geometría |
| GoogleTest | Tests (si falta, CMake lo descarga) |

**macOS (Homebrew)**

```zsh
brew install cmake ninja geos qt tcl-tk freetype xz googletest
cmake -S . -B build -G Ninja
cmake --build build
ctest --test-dir build --output-on-failure
open build/src/gui/FlatCAM.app
tools/package_macos.sh            # build/dist/FlatCAM-1.0.0-macOS-arm64.dmg
```

**Linux (Ubuntu / Debian)**

```bash
sudo apt install build-essential cmake ninja-build libgeos-dev liblzma-dev libgtest-dev \
  qt6-base-dev qt6-svg-dev qt6-serialport-dev libgl-dev tcl-dev libfreetype-dev
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build build
ctest --test-dir build --output-on-failure
tools/package_linux.sh            # .deb y AppImage (en Fedora: .rpm)
```

**Windows (Visual Studio 2022 + vcpkg + Qt)**

```bat
vcpkg install geos liblzma freetype gtest tcl --triplet x64-windows
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=%VCPKG_ROOT%\scripts\buildsystems\vcpkg.cmake
cmake --build build
ctest --test-dir build --output-on-failure
pwsh tools/package_windows.ps1    # instalador (Inno Setup) y .zip
```

Opciones de CMake: `-DFLATCAM_BUILD_TESTS=OFF`, `-DFLATCAM_BUILD_DOCS=OFF`, `-DFLATCAM_BUILD_GUI=OFF` (solo el núcleo).

**Documentación del código** (Doxygen, en español): `cmake --build build --target docs` → `build/doxygen/html/index.html`.

---

## Tests y verificación

- **2.854 tests** (GoogleTest): núcleo e interfaz. Corren en el CI (GitHub Actions) en Linux y Windows, y con dos versiones de GEOS.
- Los valores esperados **no se escribieron a mano**: los oráculos de `tests/tools/` ejecutan el código **original** de FlatCAM (parsers, herramientas, postprocesadores, editores, consola Tcl y la interfaz con PyQt5) y guardan sus resultados.
- Contra el **ejecutable FlatCAM 2024.4** con una placa real: el corte del contorno sale **idéntico byte por byte**, Paint y NCC con los mismos trazados y el taladrado con los mismos agujeros.
- Los errores del original están corregidos en la aplicación; los tests los activan para seguir comparando con el original, y cada corrección tiene su propio test.

---

## Estructura del proyecto

```text
src/core/        Núcleo sin Qt: figuras (GEOS), geometría, arcos, transformaciones
src/parsers/     Gerber, Excellon, SVG, DXF, PDF, HPGL2, imágenes y fuentes
src/tools/       Herramientas CAM (aislamiento, NCC, Paint, corte, 2 caras, panelizar, film...)
src/cam/         G-code, postprocesadores, orden de los agujeros, autonivelación
src/editors/     Modelos de los editores (sin Qt)
src/io/          Proyectos .FlatPrj y exportación
src/gui/         Interfaz Qt 6 (generated/: generada desde la interfaz original)
tests/           GoogleTest, datos reales y oráculos en Python
tools/           Generadores de la interfaz, traducciones y empaquetado
reference/       Fuentes de FlatCAM 2024.4 (solo lectura)
```

---

## Licencia, autoría y créditos

FlatCAM-cpp se distribuye bajo la **licencia MIT**, igual que FlatCAM. Ver [`LICENSE`](LICENSE) y [`NOTICE`](NOTICE).

Es un port de una cadena de versiones de FlatCAM:

1. **FlatCAM**, creado por **Juan Pablo Caram** (2014–2016). <http://flatcam.org>
2. **FlatCAM Beta** (8.9xx y 2024.x), de **Marius Adrian Stanciu** y colaboradores (2019–2024).
3. **FlatCAM β 2024.4**, mantenida por **vika-sonne** (2023–2024).
4. **FlatCAM-cpp**: el port a C++17 con Qt 6 y GEOS.

**Créditos:** los autores de FlatCAM y sus traductores; [GEOS](https://libgeos.org) (OSGeo); [Qt](https://www.qt.io) (The Qt Company); [Tcl](https://www.tcl-lang.org); [FreeType](https://freetype.org); la biblioteca [qrcode](https://github.com/lincolnloop/python-qrcode) de Lincoln Loop (portada a C++).

<p align="center"><sub>C++17 · Qt 6 · GEOS · macOS · Windows · Linux · v1.0.0</sub></p>
