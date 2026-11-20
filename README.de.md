<p align="right"><a href="README.md">🇪🇸 Español</a> · <a href="README.en.md">🇬🇧 English</a> · <a href="README.de.md">🇩🇪 Deutsch</a> · <a href="README.fr.md">🇫🇷 Français</a> · <a href="README.it.md">🇮🇹 Italiano</a> · <a href="README.pt-BR.md">🇧🇷 Português</a> · <a href="README.ro.md">🇷🇴 Română</a> · <a href="README.ru.md">🇷🇺 Русский</a> · <a href="README.tr.md">🇹🇷 Türkçe</a></p>

<p align="center">
  <img src="src/gui/resources/images/flatcam_icon256.png" width="140" alt="FlatCAM">
</p>

<h1 align="center">FlatCAM-cpp</h1>

<p align="center">
  Nativer Port von <strong>FlatCAM 2024.4</strong> nach C++17, Qt 6 und GEOS zur Herstellung von Leiterplatten mit einer CNC-Fräse oder einem Laser.
</p>

<p align="center">
  <a href="https://github.com/dnasta/flatcam-cpp/releases/latest"><img alt="Release" src="https://img.shields.io/badge/release-v1.0.0-b4771b"></a>
  <a href="LICENSE"><img alt="Lizenz" src="https://img.shields.io/badge/Lizenz-MIT-2f6f4f"></a>
  <a href="https://github.com/dnasta/flatcam-cpp/actions/workflows/ci.yml"><img alt="CI" src="https://github.com/dnasta/flatcam-cpp/actions/workflows/ci.yml/badge.svg"></a>
  <img alt="macOS" src="https://img.shields.io/badge/macOS-15%2B%20arm64-16202b">
  <img alt="Windows" src="https://img.shields.io/badge/Windows-10%20%7C%2011-0078D4">
  <img alt="Linux" src="https://img.shields.io/badge/Linux-deb%20%7C%20rpm%20%7C%20AppImage-FCC624">
  <img alt="C++" src="https://img.shields.io/badge/C%2B%2B-17-00599C">
  <img alt="Qt" src="https://img.shields.io/badge/Qt-6-41CD52">
</p>

<p align="center">
  <a href="https://github.com/dnasta/flatcam-cpp/releases/latest"><strong>⬇ v1.0.0 herunterladen</strong></a> ·
  <a href="docs/NOTAS_1.0.0.md"><strong>📝 Versionshinweise (Spanisch)</strong></a> ·
  <a href="https://github.com/dnasta/flatcam-cpp/issues"><strong>🐞 Problem melden</strong></a>
</p>

---

## Inhalt

- [Was ist FlatCAM-cpp](#was-ist-flatcam-cpp)
- [Bildschirmfotos](#bildschirmfotos)
- [Funktionen](#funktionen)
- [Sprachen](#sprachen)
- [Unterstützte Formate](#unterstützte-formate)
- [Installation](#installation)
- [Schnellstart](#schnellstart)
- [Kommandozeile und Automatisierung](#kommandozeile-und-automatisierung)
- [Aus dem Quellcode bauen](#aus-dem-quellcode-bauen)
- [Tests und Überprüfung](#tests-und-überprüfung)
- [Projektstruktur](#projektstruktur)
- [Lizenz, Urheberschaft und Danksagungen](#lizenz-urheberschaft-und-danksagungen)

---

## Was ist FlatCAM-cpp

FlatCAM wandelt die Entwurfsdateien einer Leiterplatte —**Gerber**-Lagen und **Excellon**-Bohrdateien— in **G-Code** für eine CNC-Fräse, einen Router oder einen Laser um: Isolationsfräsen, Kupferentfernung, Bohren, Ausschneiden der Platine und vieles mehr.

FlatCAM-cpp ist ein **vollständiger** Port von FlatCAM 2024.4 (Python + PyQt5) nach **C++17**, mit **Qt 6** für die Oberfläche und **GEOS** für die Geometrie. Die Oberfläche ist die des Originals —sie wurde aus der echten Originaloberfläche erzeugt—, mit denselben Menüs, Panels, Tastenkürzeln und Einstellungen, und sie liest und schreibt dieselben `.FlatPrj`-Projekte.

**In einem Satz:** Platine öffnen, Werkzeug wählen, Fräsbahnen prüfen und den G-Code speichern.

### Warum ein nativer Port

| Aspekt | FlatCAM 2024.4 (Python) | FlatCAM-cpp |
|---|---|---|
| Laufzeit | Python 3 + PyQt5 + Shapely + numpy + vispy | Natives Programm |
| Installation | Python und seine Abhängigkeiten installieren | Installer für macOS, Windows und Linux |
| Geometrie | Shapely | GEOS (reentrante C-API) |
| Oberfläche | Qt 5 | Qt 6 mit OpenGL-Zeichenfläche |
| Geschwindigkeit (Kupferentfernung einer echten Platine) | 567 s | 56 s |
| Bekannte Fehler des Originals | Vorhanden | Über 200 behoben (`--original-bugs` stellt das Originalverhalten wieder her) |

---

## Bildschirmfotos

<p align="center">
  <img src="docs/images/interfaz.png" alt="Hauptfenster" width="90%">
  <br><em>Hauptfenster: das Kupfer einer echten Platine mit seinen Eigenschaften (spanische Oberfläche).</em>
</p>

<p align="center">
  <img src="docs/images/ncc.png" alt="Kupferentfernung" width="90%">
  <br><em>Werkzeug zur Kupferentfernung (NCC).</em>
</p>

<p align="center">
  <img src="docs/images/cncjob.png" alt="G-Code" width="90%">
  <br><em>G-Code der Kupferentfernung, der Bohrungen und des Ausschnitts (Leerfahrten in Gelb).</em>
</p>

<p align="center">
  <img src="docs/images/deutsch.png" alt="Deutsche Oberfläche" width="90%">
  <br><em>Dieselbe Oberfläche auf Deutsch, mit dem Bohrwerkzeug.</em>
</p>

---

## Funktionen

### Dateien

- **Gerber RS-274X**: Blenden C/R/O/P, Makros `%AM`, Regionen `G36/G37`, Bögen `G02/G03`, Polarität und X2-Attribute.
- **Excellon**: Werkzeuge, Bohrungen, Schlitze (`G85` und Routing), Formate und Nullen, Dateien ohne Kopf.
- **G-Code**: öffnen, anzeigen, bearbeiten und exportieren.
- **SVG, DXF, PDF, HPGL2 und Bilder** (PNG, JPG, BMP), als Geometrie oder als Gerber.
- **`.FlatPrj`-Projekte** des Python-FlatCAM (LZMA-komprimiert, auch Hunderte von MB); das Original-FlatCAM öffnet die vom Port gespeicherten.
- **Export** von Gerber, Excellon, SVG, DXF, PNG und PDF.

### CAM-Werkzeuge

- **Isolation**: Durchgänge, Überlappung, V-Fräser, kombiniert, Restbearbeitung, nach Bereich oder Polygon.
- **Kupferentfernung (NCC)** und **Malen (Paint)**: Methoden Standard, Saat, Linien und Kombination, Restbearbeitung, Bereich und Referenzobjekt; „Laserlinien“.
- **Bohren**: optimierte Reihenfolge, mehrere Tiefen, Schlitze als Bohrungen; Fräsen von Bohrungen und Schlitzen.
- **Ausschneiden**: frei oder rechteckig, Stege, „Mouse Bites“ und manuelle Stege.
- **Zweiseitige Platine**: Spiegeln an Punkt, Box oder Bohrung, und Ausrichtungsbohrungen.
- **Außerdem**: Nutzen (Panelisierung), Film, Kupferfüllung (Thieving), Lötpaste, QR-Code, Passermarken, Eckmarken, Bohrungen extrahieren, Gerber stanzen, Subtrahieren, Optimal, Regelprüfung, Kalibrierung, Ätzkompensation, Objekte ausrichten, Gerber invertieren, Rechner, Transformationen, Abstand und automatische Nivellierung (Antasten mit GRBL, MACH3/4 und LinuxCNC).
- **Werkzeugdatenbank** und **Ausschlussbereiche**.

### G-Code

- Die **20 Postprozessoren** des Originals (`default`, `GRBL_laser`, `grbl_11`, `Marlin`, `Repetier`, `Roland_MDX_20`, `hpgl`, `ISEL_CNC`, `Toolchange_*`, `Z_laser`…), Zeile für Zeile identisch.
- Mehrere Tiefen, Zusatzschnitt, Start- und Endcode, Millimeter oder Zoll.

### Editoren

- **Geometrie** (mit Texten in den Systemschriften), **Excellon**, **Gerber** und **G-Code**.

### Oberfläche

- Die von FlatCAM 2024.4: Menüs, Werkzeugleisten, Reiter Projekt / Eigenschaften / Werkzeug und dieselben Tastenkürzel.
- OpenGL-Zeichenfläche mit Zoom, Verschieben, Raster, Rasterfang, HUD und Arbeitsbereich.
- Einstellungen (485 Optionen), Stufen Einfach und Erweitert, Millimeter oder Zoll, zuletzt verwendete Dateien und Lesezeichen.

### Automatisierung

- **Tcl-Konsole** mit dem echten Tcl-Interpreter und den **65 Befehlen** des Originals; `.tcl`-Skripte.

---

## Sprachen

Die Oberfläche gibt es in den **9 Sprachen** des Originals, mit denselben Übersetzungen: Deutsch, Englisch, Spanisch, Französisch, Italienisch, brasilianisches Portugiesisch, Rumänisch, Russisch und Türkisch.

- Sie startet in der **Systemsprache** (auf Englisch, wenn diese nicht dabei ist).
- Ändern: **Einstellungen → Allgemeines →** Sprache wählen **→ Sprache anwend** (die Anwendung startet neu).
- Beim Start erzwingen: Umgebungsvariable `FLATCAM_LANGUAGE` (zum Beispiel `FLATCAM_LANGUAGE=de`).

---

## Unterstützte Formate

| Format | Öffnen / importieren | Exportieren | Hinweise |
|---|:---:|:---:|---|
| Gerber RS-274X | ✅ | ✅ | Blenden, Makros, Regionen, Polarität |
| Excellon | ✅ | ✅ | Bohrungen und Schlitze |
| G-Code | ✅ | ✅ | 20 Postprozessoren |
| SVG | ✅ | ✅ | Als Geometrie oder Gerber |
| DXF | ✅ | ✅ | ASCII-DXF |
| PDF | ✅ | ✅ | Vektorimport; Drucken und Film |
| HPGL2 | ✅ | — | Der Postprozessor `hpgl` erzeugt Plotter-Code |
| PNG / JPG / BMP | ✅ | ✅ | Vektorisierung mit Schwellwert; Bild der Zeichenfläche und Film |
| `.FlatPrj` | ✅ | ✅ | Kompatibel mit dem Python-FlatCAM |

---

## Installation

Die Installer stehen auf der Seite [**Releases**](https://github.com/dnasta/flatcam-cpp/releases/latest).

### macOS

- **macOS 15 oder neuer**, mit **Apple Silicon** (M1 bis M4).
- `FlatCAM-1.0.0-macOS-arm64.dmg` öffnen und **FlatCAM** in *Programme* ziehen.
- Beim ersten Mal: Rechtsklick → **Öffnen** (Ad-hoc-Signatur, nicht notarisiert).
- Qt, GEOS, FreeType und Tcl sind enthalten: Homebrew wird nicht benötigt.

### Windows

- **Windows 10 (1809 oder neuer) und Windows 11**, 64 Bit.
- `FlatCAM-1.0.0-Windows-x64-setup.exe` (Installer) oder `FlatCAM-1.0.0-Windows-x64.zip` (portabel, ohne Installation).
- Wenn „Der Computer wurde durch Windows geschützt“ erscheint: **Weitere Informationen → Trotzdem ausführen** (nicht mit einem Zertifikat signiert).

### Linux

| Distribution | Datei | Installieren |
|---|---|---|
| Ubuntu 24.04+, Debian 13, Linux Mint 22 | `flatcam-cpp_1.0.0_amd64.deb` | `sudo apt install ./flatcam-cpp_1.0.0_amd64.deb` |
| Fedora 41+ | `flatcam-cpp-1.0.0-1.x86_64.rpm` | `sudo dnf install ./flatcam-cpp-1.0.0-1.x86_64.rpm` |
| Andere (glibc 2.39+) | `FlatCAM-1.0.0-Linux-x86_64.AppImage` | `chmod +x FlatCAM-*.AppImage && ./FlatCAM-*.AppImage` |

Mit dem `.deb` oder dem `.rpm` erscheint FlatCAM im Anwendungsmenü und startet mit dem Befehl `flatcam`.

---

## Schnellstart

```text
1. Datei → Öffnen → Gerber öffnen      Kupfer und Umriss der Platine
   Datei → Öffnen → Excellon öffnen    Bohrungen
2. Kupfer in Projekt wählen → Eigenschaften → Isolierungsrouting (oder NCC Werkzeug)
   Fräserdurchmesser, Durchgänge und Überlappung → Geometrie erzeugen
3. In der neuen Geometrie: Eigenschaften → Schnitttiefe, Vorschub → Generieren des CNC-Job-Objekts
4. Im CNC-Job: CNC-Code speichern
5. Bohrungen: Bohrwerkzeug → Generieren des CNC-Job-Objekts
6. Umriss: Ausschnittwerkzeug → Stege → Geometrie erzeugen
```

> **Vor dem Fräsen:** G-Code, Werkstücknullpunkt und Z-Höhe prüfen. Ein Fehler von 0,1 mm kann die Platine ruinieren.

---

## Kommandozeile und Automatisierung

```bash
FlatCAM platine-B_Cu.gbr platine-Edge_Cuts.gbr platine.drl   # öffnet die Dateien (nach Endung)
FlatCAM projekt.FlatPrj                                       # öffnet ein Projekt
FlatCAM skript.tcl                                            # öffnet und startet ein Tcl-Skript
FlatCAM --original-bugs                                       # verhält sich genau wie das Original-FlatCAM
```

Das Programm heißt `FlatCAM` (unter macOS `/Applications/FlatCAM.app/Contents/MacOS/FlatCAM`), `FlatCAM.exe` unter Windows und `flatcam` unter Linux.

### Tcl-Konsole

Öffnen mit **Befehlszeile** (Taste `S`). Beispiel:

```tcl
open_gerber platine-B_Cu.gbr -outname kupfer
isolate kupfer -dia 0.2 -passes 2 -overlap 10 -combine True -outname kupfer_iso
cncjob kupfer_iso -dia 0.2 -z_cut -0.1 -z_move 2 -feedrate 120 -pp default -outname kupfer_cnc
write_gcode kupfer_cnc kupfer.nc

open_excellon platine.drl -outname bohrungen
drillcncjob bohrungen -drillz -1.8 -travelz 2 -feedrate_z 100 -pp default -outname bohrungen_cnc
write_gcode bohrungen_cnc bohrungen.nc
```

`help` listet die 65 Befehle auf und `help <befehl>` erklärt jeden.

---

## Aus dem Quellcode bauen

| Abhängigkeit | Zweck |
|---|---|
| C++17-Compiler und CMake 3.20+ | AppleClang, GCC oder MSVC 2022 |
| GEOS 3.11+ | Geometrie |
| Qt 6.4+ | Oberfläche (Widgets, OpenGLWidgets, Svg, PrintSupport; SerialPort optional) |
| Tcl 8.6 oder 9 | Tcl-Konsole |
| liblzma | Komprimierte `.FlatPrj`-Projekte |
| FreeType (optional) | Texte mit Schriften im Geometrie-Editor |
| GoogleTest | Tests (CMake lädt es bei Bedarf herunter) |

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
tools/package_linux.sh            # .deb und AppImage (unter Fedora: .rpm)
```

**Windows (Visual Studio 2022 + vcpkg + Qt)**

```bat
vcpkg install geos liblzma freetype gtest tcl --triplet x64-windows
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=%VCPKG_ROOT%\scripts\buildsystems\vcpkg.cmake
cmake --build build
ctest --test-dir build --output-on-failure
pwsh tools/package_windows.ps1    # Installer (Inno Setup) und .zip
```

CMake-Optionen: `-DFLATCAM_BUILD_TESTS=OFF`, `-DFLATCAM_BUILD_DOCS=OFF`, `-DFLATCAM_BUILD_GUI=OFF` (nur der Kern).

**Code-Dokumentation** (Doxygen, auf Spanisch): `cmake --build build --target docs` → `build/doxygen/html/index.html`.

---

## Tests und Überprüfung

- **2.854 Tests** (GoogleTest): Kern und Oberfläche. Sie laufen in der CI (GitHub Actions) unter Linux und Windows und mit zwei GEOS-Versionen.
- Die erwarteten Werte wurden **nicht von Hand geschrieben**: Die Orakel in `tests/tools/` führen den **Original**-Code von FlatCAM aus (Parser, Werkzeuge, Postprozessoren, Editoren, Tcl-Konsole und die PyQt5-Oberfläche) und speichern seine Ergebnisse.
- Gegen das **Programm FlatCAM 2024.4** mit einer echten Platine: der Ausschnitt ist **Byte für Byte identisch**, Paint und NCC haben dieselben Bahnen und das Bohren dieselben Löcher.
- Die Fehler des Originals sind in der Anwendung behoben; die Tests schalten sie ein, um weiter mit dem Original zu vergleichen, und jede Korrektur hat ihren eigenen Test.

---

## Projektstruktur

```text
src/core/        Kern ohne Qt: Formen (GEOS), Geometrie, Bögen, Transformationen
src/parsers/     Gerber, Excellon, SVG, DXF, PDF, HPGL2, Bilder und Schriften
src/tools/       CAM-Werkzeuge (Isolation, NCC, Paint, Ausschnitt, zweiseitig, Nutzen, Film...)
src/cam/         G-Code, Postprozessoren, Bohrreihenfolge, Nivellierung
src/editors/     Modelle der Editoren (ohne Qt)
src/io/          .FlatPrj-Projekte und Export
src/gui/         Qt-6-Oberfläche (generated/: aus der Originaloberfläche erzeugt)
tests/           GoogleTest, echte Daten und Python-Orakel
tools/           Generatoren für Oberfläche, Übersetzungen und Pakete
reference/       Quellen von FlatCAM 2024.4 (nur lesen)
```

---

## Lizenz, Urheberschaft und Danksagungen

FlatCAM-cpp steht wie FlatCAM unter der **MIT-Lizenz**. Siehe [`LICENSE`](LICENSE) und [`NOTICE`](NOTICE).

Es ist ein Port einer Reihe von FlatCAM-Versionen:

1. **FlatCAM**, geschaffen von **Juan Pablo Caram** (2014–2016). <http://flatcam.org>
2. **FlatCAM Beta** (8.9xx und 2024.x), von **Marius Adrian Stanciu** und Mitwirkenden (2019–2024).
3. **FlatCAM β 2024.4**, gepflegt von **vika-sonne** (2023–2024).
4. **FlatCAM-cpp**: der Port nach C++17 mit Qt 6 und GEOS.

**Danksagungen:** die Autoren von FlatCAM und ihre Übersetzer; [GEOS](https://libgeos.org) (OSGeo); [Qt](https://www.qt.io) (The Qt Company); [Tcl](https://www.tcl-lang.org); [FreeType](https://freetype.org); die Bibliothek [qrcode](https://github.com/lincolnloop/python-qrcode) von Lincoln Loop (nach C++ portiert).

<p align="center"><sub>C++17 · Qt 6 · GEOS · macOS · Windows · Linux · v1.0.0</sub></p>
