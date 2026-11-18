#!/bin/zsh
# Arma FlatCAM.app autocontenido (sin depender de Homebrew) y una imagen .dmg.
#
# Uso:  tools/package_macos.sh [carpeta de build] [carpeta de salida]
#       (por defecto build y build/dist). Necesita el build de la GUI y macdeployqt (viene con Qt).
#
# Pasos: copia el paquete, macdeployqt (Qt y las bibliotecas de Homebrew: GEOS, FreeType, Tcl, liblzma...), saca los
# complementos que no se usan (teclado virtual, que arrastra QtQuick/QML, y PDF de imageformats), copia la biblioteca
# de scripts de Tcl (init.tcl) y sus módulos a Contents/Resources, firma con firma ad hoc, comprueba que nada apunte a /opt/homebrew
# ni a /usr/local y arma el .dmg.
set -euo pipefail

ROOT=${0:A:h:h}
BUILD=${1:-$ROOT/build}
OUT=${2:-$BUILD/dist}
APP_SRC="$BUILD/src/gui/FlatCAM.app"
[[ -d "$APP_SRC" ]] || { print -u2 "No está $APP_SRC: compilá la GUI primero (cmake --build \"$BUILD\")."; exit 1; }

DEPLOY=$(command -v macdeployqt6 || command -v macdeployqt || true)
[[ -n "$DEPLOY" ]] || { print -u2 "Falta macdeployqt (brew install qt)."; exit 1; }

VERSION=$(/usr/libexec/PlistBuddy -c "Print :CFBundleShortVersionString" "$APP_SRC/Contents/Info.plist")
APP="$OUT/FlatCAM.app"
rm -rf "$APP"
mkdir -p "$OUT"
cp -R "$APP_SRC" "$APP"

print "macdeployqt..."
# Los avisos de rpath sin resolver son del complemento del teclado virtual, que se borra después.
"$DEPLOY" "$APP" -always-overwrite 2>&1 | grep -v "Cannot resolve rpath\|using QList\|codesign" || true

C="$APP/Contents"
rm -rf "$C/PlugIns/platforminputcontexts" "$C/PlugIns/imageformats/libqpdf.dylib"
for fw in QtQml QtQmlMeta QtQmlModels QtQmlWorkerScript QtQuick QtPdf QtVirtualKeyboard QtVirtualKeyboardQml; do
    rm -rf "$C/Frameworks/$fw.framework"
done

# La biblioteca de scripts de Tcl: la de la misma Tcl con que se compiló (FCShell la busca en Resources/tcl<versión>).
TCLLIB=$(otool -L "$C/MacOS/FlatCAM" | awk '/libtcl/ {print $1}' | head -1)
TCLVER=$(basename "$TCLLIB" | sed -E 's/libtcl([0-9.]+)\.dylib/\1/')
TCLSRC=""
for d in /opt/homebrew/opt/tcl-tk/lib /usr/local/opt/tcl-tk/lib /opt/homebrew/lib /usr/local/lib; do
    [[ -f "$d/tcl$TCLVER/init.tcl" ]] && { TCLSRC="$d/tcl$TCLVER"; break; }
done
if [[ -n "$TCLSRC" ]]; then
    cp -RL "$TCLSRC" "$C/Resources/tcl$TCLVER"
    # Los módulos (.tm: msgcat, que usa clock...) van en la carpeta hermana tcl<mayor>/<mayor>.<menor> (tm.tcl los
    # busca al lado de la biblioteca).
    TCLMAJOR=${TCLVER%%.*}
    [[ -d "${TCLSRC:h}/tcl$TCLMAJOR" ]] && cp -RL "${TCLSRC:h}/tcl$TCLMAJOR" "$C/Resources/tcl$TCLMAJOR"
else
    print -u2 "Aviso: no encontré la biblioteca de scripts de Tcl $TCLVER; la consola usará la del sistema."
fi

# macdeployqt no reescribe el nombre propio (id) de algunas bibliotecas que llegan por @rpath (libgeos).
for lib in "$C"/Frameworks/*.dylib; do
    install_name_tool -id "@rpath/${lib:t}" "$lib" 2>/dev/null
done
# Los rpath del build (Homebrew) se borran: si no, en una máquina con Homebrew se cargarían esas bibliotecas.
for f in "$C/MacOS/FlatCAM" "$C"/Frameworks/*.dylib; do
    for rp in $(otool -l "$f" | awk '/LC_RPATH/ {getline; getline; print $2}' | grep -E "^(/opt/homebrew|/usr/local)"); do
        install_name_tool -delete_rpath "$rp" "$f" 2>/dev/null || true
    done
done
rpaths() { otool -l "$1" | awk '/LC_RPATH/ {getline; getline; print $2}'; }
if ! rpaths "$C/MacOS/FlatCAM" | grep -qx "@executable_path/../Frameworks"; then
    install_name_tool -add_rpath "@executable_path/../Frameworks" "$C/MacOS/FlatCAM"
fi

# Que nada quede apuntando a Homebrew.
BAD=$(find "$C" -type f \( -name "*.dylib" -o -perm +111 \) -print0 | xargs -0 otool -L 2>/dev/null |
      grep -E "^\s+(/opt/homebrew|/usr/local)" | sort -u || true)
BAD+=$(for f in "$C/MacOS/FlatCAM" "$C"/Frameworks/*.dylib; do rpaths "$f"; done |
       grep -E "^(/opt/homebrew|/usr/local)" || true)
if [[ -n "$BAD" ]]; then
    print -u2 "Quedan bibliotecas fuera del paquete:"; print -u2 "$BAD"; exit 1
fi

print "Firma ad hoc..."
codesign --force --deep --sign - "$APP"
codesign --verify --deep --strict "$APP"

DMG="$OUT/FlatCAM-$VERSION-macOS-arm64.dmg"
rm -f "$DMG"
STAGE=$(mktemp -d)
cp -R "$APP" "$STAGE/"
ln -s /Applications "$STAGE/Aplicaciones"
hdiutil create -quiet -volname "FlatCAM $VERSION" -srcfolder "$STAGE" -ov -format UDZO "$DMG"
rm -rf "$STAGE"
print "Listo: $APP ($(du -sh "$APP" | cut -f1)) y $DMG ($(du -sh "$DMG" | cut -f1))"
