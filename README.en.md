<p align="right"><a href="README.md">🇪🇸 Español</a> · <a href="README.en.md">🇬🇧 English</a> · <a href="README.de.md">🇩🇪 Deutsch</a> · <a href="README.fr.md">🇫🇷 Français</a> · <a href="README.it.md">🇮🇹 Italiano</a> · <a href="README.pt-BR.md">🇧🇷 Português</a> · <a href="README.ro.md">🇷🇴 Română</a> · <a href="README.ru.md">🇷🇺 Русский</a> · <a href="README.tr.md">🇹🇷 Türkçe</a></p>

<p align="center">
  <img src="src/gui/resources/images/flatcam_icon256.png" width="140" alt="FlatCAM">
</p>

<h1 align="center">FlatCAM-cpp</h1>

<p align="center">
  Native port of <strong>FlatCAM 2024.4</strong> to C++17, Qt 6 and GEOS, for making printed circuit boards with a CNC router or a laser.
</p>

<p align="center">
  <a href="https://github.com/dnasta/flatcam-cpp/releases/latest"><img alt="Release" src="https://img.shields.io/badge/release-v1.0.0-b4771b"></a>
  <a href="LICENSE"><img alt="license" src="https://img.shields.io/badge/license-MIT-2f6f4f"></a>
  <a href="https://github.com/dnasta/flatcam-cpp/actions/workflows/ci.yml"><img alt="CI" src="https://github.com/dnasta/flatcam-cpp/actions/workflows/ci.yml/badge.svg"></a>
  <img alt="macOS" src="https://img.shields.io/badge/macOS-15%2B%20arm64-16202b">
  <img alt="Windows" src="https://img.shields.io/badge/Windows-10%20%7C%2011-0078D4">
  <img alt="Linux" src="https://img.shields.io/badge/Linux-deb%20%7C%20rpm%20%7C%20AppImage-FCC624">
  <img alt="C++" src="https://img.shields.io/badge/C%2B%2B-17-00599C">
  <img alt="Qt" src="https://img.shields.io/badge/Qt-6-41CD52">
</p>

<p align="center">
  <a href="https://github.com/dnasta/flatcam-cpp/releases/latest"><strong>⬇ Download v1.0.0</strong></a> ·
  <a href="docs/NOTAS_1.0.0.md"><strong>📝 Release notes (Spanish)</strong></a> ·
  <a href="https://github.com/dnasta/flatcam-cpp/issues"><strong>🐞 Report an issue</strong></a>
</p>

---

## Contents

- [What is FlatCAM-cpp](#what-is-flatcam-cpp)
- [Screenshots](#screenshots)
- [Features](#features)
- [Languages](#languages)
- [Supported formats](#supported-formats)
- [Installation](#installation)
- [Quick start](#quick-start)
- [Command line and automation](#command-line-and-automation)
- [Building from source](#building-from-source)
- [Tests and verification](#tests-and-verification)
- [Project layout](#project-layout)
- [License, authorship and credits](#license-authorship-and-credits)

---

## What is FlatCAM-cpp

FlatCAM turns PCB design files —**Gerber** layers and **Excellon** drill files— into **G-code** for a CNC mill, a router or a laser: trace isolation, copper clearing, drilling, board cutout and much more.

FlatCAM-cpp is a **complete** port of FlatCAM 2024.4 (Python + PyQt5) to **C++17**, with **Qt 6** for the interface and **GEOS** for geometry. The interface is the original one —it was generated from the real original interface—, with the same menus, panels, shortcuts and preferences, and it reads and writes the same `.FlatPrj` projects.

**In one sentence:** open your board, pick the tool, check the toolpaths and save the G-code.

### Why a native port

| Aspect | FlatCAM 2024.4 (Python) | FlatCAM-cpp |
|---|---|---|
| Runtime | Python 3 + PyQt5 + Shapely + numpy + vispy | Native executable |
| Installation | Install Python and its dependencies | Installers for macOS, Windows and Linux |
| Geometry | Shapely | GEOS (reentrant C API) |
| Interface | Qt 5 | Qt 6 with an OpenGL canvas |
| Speed (copper clearing of a real board) | 567 s | 56 s |
| Known bugs of the original | Present | More than 200 fixed (`--original-bugs` restores the original behaviour) |

---

## Screenshots

<p align="center">
  <img src="docs/images/interfaz.png" alt="Main window" width="90%">
  <br><em>Main window: the copper of a real board with its properties (Spanish interface).</em>
</p>

<p align="center">
  <img src="docs/images/ncc.png" alt="Copper clearing" width="90%">
  <br><em>Non-copper clearing (NCC) tool.</em>
</p>

<p align="center">
  <img src="docs/images/cncjob.png" alt="G-code" width="90%">
  <br><em>G-code of the copper clearing, the drilling and the cutout (travel moves in yellow).</em>
</p>

<p align="center">
  <img src="docs/images/deutsch.png" alt="German interface" width="90%">
  <br><em>The same interface in German, with the drilling tool.</em>
</p>

---

## Features

### Files

- **Gerber RS-274X**: C/R/O/P apertures, `%AM` macros, `G36/G37` regions, `G02/G03` arcs, polarity and X2 attributes.
- **Excellon**: tools, holes, slots (`G85` and routing), formats and zeros, files without a header.
- **G-code**: open, view, edit and export.
- **SVG, DXF, PDF, HPGL2 and images** (PNG, JPG, BMP), as Geometry or as Gerber.
- **`.FlatPrj` projects** from the Python FlatCAM (LZMA-compressed, even hundreds of MB); the original FlatCAM opens the ones saved by the port.
- **Export** Gerber, Excellon, SVG, DXF, PNG and PDF.

### CAM tools

- **Isolation**: passes, overlap, V-shaped tools, combined, rest machining, by area or by polygon.
- **Non-copper clearing (NCC)** and **Paint**: Standard, Seed, Lines and Combo methods, rest machining, area and reference object; "Laser lines".
- **Drilling**: optimized order, multi-depth, slots as drills; milling of holes and slots.
- **Cutout**: freeform or rectangular, gaps, mouse bites and manual gaps.
- **2-sided PCB**: mirroring by point, box or hole, and alignment drills.
- **And also**: Panelize, Film, Copper Thieving, Solder Paste, QR Code, Fiducials, Corner Markers, Extract Drills, Punch Gerber, Subtract, Optimal, Rules Check, Calibration, Etch Compensation, Align Objects, Invert Gerber, Calculators, Transform, Distance and Autolevelling (probing with GRBL, MACH3/4 and LinuxCNC).
- **Tools database** and **exclusion areas**.

### G-code

- The original's **20 preprocessors** (`default`, `GRBL_laser`, `grbl_11`, `Marlin`, `Repetier`, `Roland_MDX_20`, `hpgl`, `ISEL_CNC`, `Toolchange_*`, `Z_laser`…), identical line by line.
- Multi-depth, extra cut, start and end code snippets, millimetres or inches.

### Editors

- **Geometry** (with text written with the system fonts), **Excellon**, **Gerber** and **G-code**.

### Interface

- FlatCAM 2024.4's: menus, toolbars, Project / Properties / Tool tabs and the same shortcuts.
- OpenGL canvas with zoom, pan, grid, grid snapping, HUD and workspace.
- Preferences (485 options), Basic and Advanced levels, millimetres or inches, recent files and bookmarks.

### Automation

- **Tcl shell** with the real Tcl interpreter and the original's **65 commands**; `.tcl` scripts.

---

## Languages

The interface comes in the original's **9 languages**, with the same translations: German, English, Spanish, French, Italian, Brazilian Portuguese, Romanian, Russian and Turkish.

- It starts in the **system language** (English if it is not one of the nine).
- To change it: **Preferences → General →** choose the language **→ Apply Language** (the application restarts).
- To force one at startup: the `FLATCAM_LANGUAGE` environment variable (for example `FLATCAM_LANGUAGE=de`).

---

## Supported formats

| Format | Open / import | Export | Notes |
|---|:---:|:---:|---|
| Gerber RS-274X | ✅ | ✅ | Apertures, macros, regions, polarity |
| Excellon | ✅ | ✅ | Holes and slots |
| G-code | ✅ | ✅ | 20 preprocessors |
| SVG | ✅ | ✅ | As Geometry or Gerber |
| DXF | ✅ | ✅ | ASCII DXF |
| PDF | ✅ | ✅ | Vector import; printing and film |
| HPGL2 | ✅ | — | The `hpgl` preprocessor generates plotter code |
| PNG / JPG / BMP | ✅ | ✅ | Threshold vectorization; canvas snapshot and film |
| `.FlatPrj` | ✅ | ✅ | Compatible with the Python FlatCAM |

---

## Installation

The installers are on the [**Releases**](https://github.com/dnasta/flatcam-cpp/releases/latest) page.

### macOS

- **macOS 15 or later**, on **Apple Silicon** (M1 to M4).
- Open `FlatCAM-1.0.0-macOS-arm64.dmg` and drag **FlatCAM** to *Applications*.
- The first time: right click → **Open** (ad hoc signature, not notarized).
- Qt, GEOS, FreeType and Tcl are included: Homebrew is not needed.

### Windows

- **Windows 10 (1809 or later) and Windows 11**, 64-bit.
- `FlatCAM-1.0.0-Windows-x64-setup.exe` (installer) or `FlatCAM-1.0.0-Windows-x64.zip` (portable, no installation).
- If "Windows protected your PC" appears: **More info → Run anyway** (it is not signed with a certificate).

### Linux

| Distribution | File | Install |
|---|---|---|
| Ubuntu 24.04+, Debian 13, Linux Mint 22 | `flatcam-cpp_1.0.0_amd64.deb` | `sudo apt install ./flatcam-cpp_1.0.0_amd64.deb` |
| Fedora 41+ | `flatcam-cpp-1.0.0-1.x86_64.rpm` | `sudo dnf install ./flatcam-cpp-1.0.0-1.x86_64.rpm` |
| Others (glibc 2.39+) | `FlatCAM-1.0.0-Linux-x86_64.AppImage` | `chmod +x FlatCAM-*.AppImage && ./FlatCAM-*.AppImage` |

With the `.deb` or the `.rpm`, FlatCAM shows up in the applications menu and runs with the `flatcam` command.

---

## Quick start

```text
1. File → Open → Open Gerber      board copper and outline
   File → Open → Open Excellon    drills
2. Select the copper in Project → Properties → Isolation Routing (or NCC Tool)
   tool diameter, passes and overlap → Generate Geometry
3. In the new Geometry: Properties → cut Z, feedrate → Generate CNCJob object
4. In the CNC Job: Save CNC Code
5. Drills: Drilling Tool → Generate CNCJob object
6. Outline: Cutout Tool → gaps → Generate Geometry
```

> **Before machining:** check the G-code, the work zero and the Z height. A 0.1 mm mistake can ruin the board.

---

## Command line and automation

```bash
FlatCAM board-B_Cu.gbr board-Edge_Cuts.gbr board.drl   # opens the files (by extension)
FlatCAM project.FlatPrj                                 # opens a project
FlatCAM script.tcl                                      # opens and runs a Tcl script
FlatCAM --original-bugs                                 # behaves exactly like the original FlatCAM
```

The executable is `FlatCAM` (on macOS, `/Applications/FlatCAM.app/Contents/MacOS/FlatCAM`), `FlatCAM.exe` on Windows and `flatcam` on Linux.

### Tcl shell

Open it with **Command Line** (key `S`). Example:

```tcl
open_gerber board-B_Cu.gbr -outname copper
isolate copper -dia 0.2 -passes 2 -overlap 10 -combine True -outname copper_iso
cncjob copper_iso -dia 0.2 -z_cut -0.1 -z_move 2 -feedrate 120 -pp default -outname copper_cnc
write_gcode copper_cnc copper.nc

open_excellon board.drl -outname drills
drillcncjob drills -drillz -1.8 -travelz 2 -feedrate_z 100 -pp default -outname drills_cnc
write_gcode drills_cnc drills.nc
```

`help` lists the 65 commands and `help <command>` explains each one.

---

## Building from source

| Dependency | Use |
|---|---|
| C++17 compiler and CMake 3.20+ | AppleClang, GCC or MSVC 2022 |
| GEOS 3.11+ | Geometry |
| Qt 6.4+ | Interface (Widgets, OpenGLWidgets, Svg, PrintSupport; SerialPort optional) |
| Tcl 8.6 or 9 | Tcl shell |
| liblzma | Compressed `.FlatPrj` projects |
| FreeType (optional) | Text with fonts in the Geometry editor |
| GoogleTest | Tests (CMake downloads it if missing) |

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
tools/package_linux.sh            # .deb and AppImage (on Fedora: .rpm)
```

**Windows (Visual Studio 2022 + vcpkg + Qt)**

```bat
vcpkg install geos liblzma freetype gtest tcl --triplet x64-windows
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=%VCPKG_ROOT%\scripts\buildsystems\vcpkg.cmake
cmake --build build
ctest --test-dir build --output-on-failure
pwsh tools/package_windows.ps1    # installer (Inno Setup) and .zip
```

CMake options: `-DFLATCAM_BUILD_TESTS=OFF`, `-DFLATCAM_BUILD_DOCS=OFF`, `-DFLATCAM_BUILD_GUI=OFF` (core only).

**Code documentation** (Doxygen, in Spanish): `cmake --build build --target docs` → `build/doxygen/html/index.html`.

---

## Tests and verification

- **2,854 tests** (GoogleTest): core and interface. They run in CI (GitHub Actions) on Linux and Windows, and with two GEOS versions.
- The expected values were **not written by hand**: the oracles in `tests/tools/` run the **original** FlatCAM code (parsers, tools, preprocessors, editors, Tcl shell and the PyQt5 interface) and store its results.
- Against the **FlatCAM 2024.4 executable** with a real board: the cutout comes out **byte for byte identical**, Paint and NCC with the same toolpaths and the drilling with the same holes.
- The original's bugs are fixed in the application; the tests turn them on to keep comparing with the original, and each fix has its own test.

---

## Project layout

```text
src/core/        Qt-free core: shapes (GEOS), geometry, arcs, transforms
src/parsers/     Gerber, Excellon, SVG, DXF, PDF, HPGL2, images and fonts
src/tools/       CAM tools (isolation, NCC, Paint, cutout, 2-sided, panelize, film...)
src/cam/         G-code, preprocessors, drill ordering, autolevelling
src/editors/     Editor models (Qt-free)
src/io/          .FlatPrj projects and export
src/gui/         Qt 6 interface (generated/: generated from the original interface)
tests/           GoogleTest, real data and Python oracles
tools/           Interface, translation and packaging generators
reference/       FlatCAM 2024.4 sources (read only)
```

---

## License, authorship and credits

FlatCAM-cpp is distributed under the **MIT license**, like FlatCAM. See [`LICENSE`](LICENSE) and [`NOTICE`](NOTICE).

It is a port of a chain of FlatCAM versions:

1. **FlatCAM**, created by **Juan Pablo Caram** (2014–2016). <http://flatcam.org>
2. **FlatCAM Beta** (8.9xx and 2024.x), by **Marius Adrian Stanciu** and contributors (2019–2024).
3. **FlatCAM β 2024.4**, maintained by **vika-sonne** (2023–2024).
4. **FlatCAM-cpp**: the port to C++17 with Qt 6 and GEOS.

**Credits:** the FlatCAM authors and translators; [GEOS](https://libgeos.org) (OSGeo); [Qt](https://www.qt.io) (The Qt Company); [Tcl](https://www.tcl-lang.org); [FreeType](https://freetype.org); Lincoln Loop's [qrcode](https://github.com/lincolnloop/python-qrcode) library (ported to C++).

<p align="center"><sub>C++17 · Qt 6 · GEOS · macOS · Windows · Linux · v1.0.0</sub></p>
