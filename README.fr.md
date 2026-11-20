<p align="right"><a href="README.md">🇪🇸 Español</a> · <a href="README.en.md">🇬🇧 English</a> · <a href="README.de.md">🇩🇪 Deutsch</a> · <a href="README.fr.md">🇫🇷 Français</a> · <a href="README.it.md">🇮🇹 Italiano</a> · <a href="README.pt-BR.md">🇧🇷 Português</a> · <a href="README.ro.md">🇷🇴 Română</a> · <a href="README.ru.md">🇷🇺 Русский</a> · <a href="README.tr.md">🇹🇷 Türkçe</a></p>

<p align="center">
  <img src="src/gui/resources/images/flatcam_icon256.png" width="140" alt="FlatCAM">
</p>

<h1 align="center">FlatCAM-cpp</h1>

<p align="center">
  Portage natif de <strong>FlatCAM 2024.4</strong> en C++17, Qt 6 et GEOS pour fabriquer des circuits imprimés avec une CNC ou un laser.
</p>

<p align="center">
  <a href="https://github.com/dnasta/flatcam-cpp/releases/latest"><img alt="Release" src="https://img.shields.io/badge/release-v1.0.0-b4771b"></a>
  <a href="LICENSE"><img alt="licence" src="https://img.shields.io/badge/licence-MIT-2f6f4f"></a>
  <a href="https://github.com/dnasta/flatcam-cpp/actions/workflows/ci.yml"><img alt="CI" src="https://github.com/dnasta/flatcam-cpp/actions/workflows/ci.yml/badge.svg"></a>
  <img alt="macOS" src="https://img.shields.io/badge/macOS-15%2B%20arm64-16202b">
  <img alt="Windows" src="https://img.shields.io/badge/Windows-10%20%7C%2011-0078D4">
  <img alt="Linux" src="https://img.shields.io/badge/Linux-deb%20%7C%20rpm%20%7C%20AppImage-FCC624">
  <img alt="C++" src="https://img.shields.io/badge/C%2B%2B-17-00599C">
  <img alt="Qt" src="https://img.shields.io/badge/Qt-6-41CD52">
</p>

<p align="center">
  <a href="https://github.com/dnasta/flatcam-cpp/releases/latest"><strong>⬇ Télécharger la v1.0.0</strong></a> ·
  <a href="docs/NOTAS_1.0.0.md"><strong>📝 Notes de version (espagnol)</strong></a> ·
  <a href="https://github.com/dnasta/flatcam-cpp/issues"><strong>🐞 Signaler un problème</strong></a>
</p>

---

## Sommaire

- [Qu’est-ce que FlatCAM-cpp](#quest-ce-que-flatcam-cpp)
- [Captures d’écran](#captures-décran)
- [Fonctionnalités](#fonctionnalités)
- [Langues](#langues)
- [Formats pris en charge](#formats-pris-en-charge)
- [Installation](#installation)
- [Démarrage rapide](#démarrage-rapide)
- [Ligne de commande et automatisation](#ligne-de-commande-et-automatisation)
- [Compiler depuis les sources](#compiler-depuis-les-sources)
- [Tests et vérification](#tests-et-vérification)
- [Organisation du projet](#organisation-du-projet)
- [Licence, auteurs et remerciements](#licence-auteurs-et-remerciements)

---

## Qu’est-ce que FlatCAM-cpp

FlatCAM transforme les fichiers de conception d’un circuit imprimé —couches **Gerber** et perçages **Excellon**— en **G-code** pour une fraiseuse CNC, une défonceuse ou un laser : isolation des pistes, nettoyage du cuivre, perçage, découpe de la carte et bien plus.

FlatCAM-cpp est un portage **complet** de FlatCAM 2024.4 (Python + PyQt5) en **C++17**, avec **Qt 6** pour l’interface et **GEOS** pour la géométrie. L’interface est celle de l’original —elle a été générée à partir de la véritable interface d’origine—, avec les mêmes menus, panneaux, raccourcis et préférences, et elle lit et écrit les mêmes projets `.FlatPrj`.

**En une phrase :** ouvrez votre carte, choisissez l’outil, vérifiez les trajectoires et enregistrez le G-code.

### Pourquoi un portage natif

| Aspect | FlatCAM 2024.4 (Python) | FlatCAM-cpp |
|---|---|---|
| Exécution | Python 3 + PyQt5 + Shapely + numpy + vispy | Exécutable natif |
| Installation | Installer Python et ses dépendances | Installateurs pour macOS, Windows et Linux |
| Géométrie | Shapely | GEOS (API C réentrante) |
| Interface | Qt 5 | Qt 6 avec un canevas OpenGL |
| Vitesse (nettoyage du cuivre d’une vraie carte) | 567 s | 56 s |
| Bogues connus de l’original | Présents | Plus de 200 corrigés (`--original-bugs` rétablit le comportement d’origine) |

---

## Captures d’écran

<p align="center">
  <img src="docs/images/interfaz.png" alt="Fenêtre principale" width="90%">
  <br><em>Fenêtre principale : le cuivre d’une vraie carte avec ses propriétés (interface en espagnol).</em>
</p>

<p align="center">
  <img src="docs/images/ncc.png" alt="Nettoyage du cuivre" width="90%">
  <br><em>Outil de nettoyage du cuivre (NCC).</em>
</p>

<p align="center">
  <img src="docs/images/cncjob.png" alt="G-code" width="90%">
  <br><em>G-code du nettoyage du cuivre, du perçage et de la découpe (déplacements en jaune).</em>
</p>

<p align="center">
  <img src="docs/images/deutsch.png" alt="Interface en allemand" width="90%">
  <br><em>La même interface en allemand, avec l’outil de perçage.</em>
</p>

---

## Fonctionnalités

### Fichiers

- **Gerber RS-274X** : ouvertures C/R/O/P, macros `%AM`, régions `G36/G37`, arcs `G02/G03`, polarité et attributs X2.
- **Excellon** : outils, trous, fentes (`G85` et détourage), formats et zéros, fichiers sans en-tête.
- **G-code** : ouvrir, afficher, modifier et exporter.
- **SVG, DXF, PDF, HPGL2 et images** (PNG, JPG, BMP), en Géométrie ou en Gerber.
- **Projets `.FlatPrj`** du FlatCAM en Python (compressés LZMA, même de plusieurs centaines de Mo) ; le FlatCAM d’origine ouvre ceux qu’enregistre le portage.
- **Exporter** en Gerber, Excellon, SVG, DXF, PNG et PDF.

### Outils FAO

- **Isolation** : passes, recouvrement, outils en V, combinée, usinage du reste, par zone ou par polygone.
- **Nettoyage du cuivre (NCC)** et **Peinture (Paint)** : méthodes Standard, Graine, Lignes et Combo, usinage du reste, zone et objet de référence ; « lignes laser ».
- **Perçage** : ordre optimisé, profondeur multiple, fentes comme trous ; fraisage des trous et des fentes.
- **Découpe** : libre ou rectangulaire, ponts, « mouse bites » et ponts manuels.
- **Circuit double face** : miroir par point, boîte ou trou, et trous d’alignement.
- **Et aussi** : Panelisation, Film, Remplissage de cuivre (thieving), Pâte à braser, Code QR, Repères (fiducials), Marques d’angle, Extraire les perçages, Poinçonner un Gerber, Soustraire, Optimal, Vérification des règles, Calibration, Compensation de gravure, Aligner les objets, Inverser un Gerber, Calculatrices, Transformations, Distance et Mise à niveau automatique (palpage avec GRBL, MACH3/4 et LinuxCNC).
- **Base de données d’outils** et **zones d’exclusion**.

### G-code

- Les **20 post-processeurs** de l’original (`default`, `GRBL_laser`, `grbl_11`, `Marlin`, `Repetier`, `Roland_MDX_20`, `hpgl`, `ISEL_CNC`, `Toolchange_*`, `Z_laser`…), identiques ligne par ligne.
- Profondeur multiple, coupe supplémentaire, code de début et de fin, millimètres ou pouces.

### Éditeurs

- **Géométrie** (avec des textes écrits dans les polices du système), **Excellon**, **Gerber** et **G-code**.

### Interface

- Celle de FlatCAM 2024.4 : menus, barres d’outils, onglets Projet / Propriétés / Outil et les mêmes raccourcis.
- Canevas OpenGL avec zoom, panoramique, grille, aimantation à la grille, HUD et espace de travail.
- Préférences (485 options), niveaux Basique et Avancé, millimètres ou pouces, fichiers récents et signets.

### Automatisation

- **Console Tcl** avec le véritable interpréteur Tcl et les **65 commandes** de l’original ; scripts `.tcl`.

---

## Langues

L’interface est disponible dans les **9 langues** de l’original, avec les mêmes traductions : allemand, anglais, espagnol, français, italien, portugais du Brésil, roumain, russe et turc.

- Elle démarre dans la **langue du système** (en anglais si elle n’en fait pas partie).
- Pour la changer : **Préférences → Général →** choisir la langue **→ Appliquer la langue** (l’application redémarre).
- Pour en imposer une au démarrage : variable d’environnement `FLATCAM_LANGUAGE` (par exemple `FLATCAM_LANGUAGE=fr`).

---

## Formats pris en charge

| Format | Ouvrir / importer | Exporter | Remarques |
|---|:---:|:---:|---|
| Gerber RS-274X | ✅ | ✅ | Ouvertures, macros, régions, polarité |
| Excellon | ✅ | ✅ | Trous et fentes |
| G-code | ✅ | ✅ | 20 post-processeurs |
| SVG | ✅ | ✅ | En Géométrie ou en Gerber |
| DXF | ✅ | ✅ | DXF ASCII |
| PDF | ✅ | ✅ | Import vectoriel ; impression et film |
| HPGL2 | ✅ | — | Le post-processeur `hpgl` génère du code pour traceurs |
| PNG / JPG / BMP | ✅ | ✅ | Vectorisation par seuil ; capture du canevas et film |
| `.FlatPrj` | ✅ | ✅ | Compatibles avec le FlatCAM en Python |

---

## Installation

Les installateurs se trouvent sur la page [**Releases**](https://github.com/dnasta/flatcam-cpp/releases/latest).

### macOS

- **macOS 15 ou ultérieur**, sur **Apple Silicon** (M1 à M4).
- Ouvrez `FlatCAM-1.0.0-macOS-arm64.dmg` et glissez **FlatCAM** dans *Applications*.
- La première fois : clic droit → **Ouvrir** (signature ad hoc, non notarisée).
- Qt, GEOS, FreeType et Tcl sont inclus : Homebrew n’est pas nécessaire.

### Windows

- **Windows 10 (1809 ou ultérieur) et Windows 11**, 64 bits.
- `FlatCAM-1.0.0-Windows-x64-setup.exe` (installateur) ou `FlatCAM-1.0.0-Windows-x64.zip` (portable, sans installation).
- Si « Windows a protégé votre ordinateur » apparaît : **Informations complémentaires → Exécuter quand même** (non signé avec un certificat).

### Linux

| Distribution | Fichier | Installer |
|---|---|---|
| Ubuntu 24.04+, Debian 13, Linux Mint 22 | `flatcam-cpp_1.0.0_amd64.deb` | `sudo apt install ./flatcam-cpp_1.0.0_amd64.deb` |
| Fedora 41+ | `flatcam-cpp-1.0.0-1.x86_64.rpm` | `sudo dnf install ./flatcam-cpp-1.0.0-1.x86_64.rpm` |
| Autres (glibc 2.39+) | `FlatCAM-1.0.0-Linux-x86_64.AppImage` | `chmod +x FlatCAM-*.AppImage && ./FlatCAM-*.AppImage` |

Avec le `.deb` ou le `.rpm`, FlatCAM apparaît dans le menu des applications et se lance avec la commande `flatcam`.

---

## Démarrage rapide

```text
1. Fichier → Ouvrir → Ouvrir Gerber      cuivre et contour de la carte
   Fichier → Ouvrir → Ouvrir Excellon    perçages
2. Choisissez le cuivre dans Projet → Propriétés → Routage d'isolement (ou Outil NCC)
   diamètre de l’outil, passes et recouvrement → Générer la Géométrie
3. Dans la nouvelle Géométrie : Propriétés → Z de coupe, avance → Générer l'objet CNC Job
4. Dans le CNC Job : Enregistrer le code CNC
5. Perçages : Outil Forage → Générer l'objet CNC Job
6. Contour : Outil de Découpe → ponts → Générer la Géométrie
```

> **Avant d’usiner :** vérifiez le G-code, le zéro pièce et la hauteur Z. Une erreur de 0,1 mm peut ruiner la carte.

---

## Ligne de commande et automatisation

```bash
FlatCAM carte-B_Cu.gbr carte-Edge_Cuts.gbr carte.drl   # ouvre les fichiers (selon l’extension)
FlatCAM projet.FlatPrj                                  # ouvre un projet
FlatCAM script.tcl                                      # ouvre et exécute un script Tcl
FlatCAM --original-bugs                                 # se comporte exactement comme le FlatCAM d’origine
```

L’exécutable est `FlatCAM` (sous macOS, `/Applications/FlatCAM.app/Contents/MacOS/FlatCAM`), `FlatCAM.exe` sous Windows et `flatcam` sous Linux.

### Console Tcl

Elle s’ouvre avec **Ligne de commande** (touche `S`). Exemple :

```tcl
open_gerber carte-B_Cu.gbr -outname cuivre
isolate cuivre -dia 0.2 -passes 2 -overlap 10 -combine True -outname cuivre_iso
cncjob cuivre_iso -dia 0.2 -z_cut -0.1 -z_move 2 -feedrate 120 -pp default -outname cuivre_cnc
write_gcode cuivre_cnc cuivre.nc

open_excellon carte.drl -outname percages
drillcncjob percages -drillz -1.8 -travelz 2 -feedrate_z 100 -pp default -outname percages_cnc
write_gcode percages_cnc percages.nc
```

`help` liste les 65 commandes et `help <commande>` explique chacune.

---

## Compiler depuis les sources

| Dépendance | Usage |
|---|---|
| Compilateur C++17 et CMake 3.20+ | AppleClang, GCC ou MSVC 2022 |
| GEOS 3.11+ | Géométrie |
| Qt 6.4+ | Interface (Widgets, OpenGLWidgets, Svg, PrintSupport ; SerialPort facultatif) |
| Tcl 8.6 ou 9 | Console Tcl |
| liblzma | Projets `.FlatPrj` compressés |
| FreeType (facultatif) | Textes avec polices dans l’éditeur de Géométrie |
| GoogleTest | Tests (CMake le télécharge s’il manque) |

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
tools/package_linux.sh            # .deb et AppImage (sous Fedora : .rpm)
```

**Windows (Visual Studio 2022 + vcpkg + Qt)**

```bat
vcpkg install geos liblzma freetype gtest tcl --triplet x64-windows
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=%VCPKG_ROOT%\scripts\buildsystems\vcpkg.cmake
cmake --build build
ctest --test-dir build --output-on-failure
pwsh tools/package_windows.ps1    # installateur (Inno Setup) et .zip
```

Options CMake : `-DFLATCAM_BUILD_TESTS=OFF`, `-DFLATCAM_BUILD_DOCS=OFF`, `-DFLATCAM_BUILD_GUI=OFF` (noyau seul).

**Documentation du code** (Doxygen, en espagnol) : `cmake --build build --target docs` → `build/doxygen/html/index.html`.

---

## Tests et vérification

- **2 854 tests** (GoogleTest) : noyau et interface. Ils tournent dans la CI (GitHub Actions) sous Linux et Windows, et avec deux versions de GEOS.
- Les valeurs attendues **n’ont pas été écrites à la main** : les oracles de `tests/tools/` exécutent le code **original** de FlatCAM (analyseurs, outils, post-processeurs, éditeurs, console Tcl et l’interface PyQt5) et enregistrent ses résultats.
- Face à l’**exécutable FlatCAM 2024.4** avec une vraie carte : la découpe est **identique octet par octet**, Paint et NCC donnent les mêmes trajectoires et le perçage les mêmes trous.
- Les bogues de l’original sont corrigés dans l’application ; les tests les réactivent pour continuer à comparer avec l’original, et chaque correction a son propre test.

---

## Organisation du projet

```text
src/core/        Noyau sans Qt : formes (GEOS), géométrie, arcs, transformations
src/parsers/     Gerber, Excellon, SVG, DXF, PDF, HPGL2, images et polices
src/tools/       Outils FAO (isolation, NCC, Paint, découpe, double face, panelisation, film...)
src/cam/         G-code, post-processeurs, ordre des perçages, mise à niveau
src/editors/     Modèles des éditeurs (sans Qt)
src/io/          Projets .FlatPrj et export
src/gui/         Interface Qt 6 (generated/ : générée depuis l’interface d’origine)
tests/           GoogleTest, données réelles et oracles en Python
tools/           Générateurs de l’interface, des traductions et des paquets
reference/       Sources de FlatCAM 2024.4 (lecture seule)
```

---

## Licence, auteurs et remerciements

FlatCAM-cpp est distribué sous **licence MIT**, comme FlatCAM. Voir [`LICENSE`](LICENSE) et [`NOTICE`](NOTICE).

C’est le portage d’une lignée de versions de FlatCAM :

1. **FlatCAM**, créé par **Juan Pablo Caram** (2014–2016). <http://flatcam.org>
2. **FlatCAM Beta** (8.9xx et 2024.x), par **Marius Adrian Stanciu** et ses contributeurs (2019–2024).
3. **FlatCAM β 2024.4**, maintenu par **vika-sonne** (2023–2024).
4. **FlatCAM-cpp** : le portage en C++17 avec Qt 6 et GEOS.

**Remerciements :** les auteurs de FlatCAM et ses traducteurs ; [GEOS](https://libgeos.org) (OSGeo) ; [Qt](https://www.qt.io) (The Qt Company) ; [Tcl](https://www.tcl-lang.org) ; [FreeType](https://freetype.org) ; la bibliothèque [qrcode](https://github.com/lincolnloop/python-qrcode) de Lincoln Loop (portée en C++).

<p align="center"><sub>C++17 · Qt 6 · GEOS · macOS · Windows · Linux · v1.0.0</sub></p>
