#!/usr/bin/env bash
# Arma los paquetes de Linux: .deb o .rpm (con CPack, según la distribución donde corre) y un AppImage.
#
# Uso:  tools/package_linux.sh [carpeta de build] [carpeta de salida] [deb|rpm|appimage]...
#       (por defecto build, build/dist y lo que corresponda: deb en Debian/Ubuntu, rpm en Fedora/openSUSE, y appimage).
#       El build tiene que estar configurado con -DCMAKE_BUILD_TYPE=Release (el ejecutable se instala como "flatcam").
#
# El AppImage se arma con linuxdeploy y su complemento de Qt (los baja si no están) y lleva la biblioteca de scripts
# de Tcl en usr/share/flatcam/tcl<versión> (FCShell la busca ahí).
set -euo pipefail

ROOT=$(cd "$(dirname "$0")/.." && pwd)
BUILD=$(realpath -m "${1:-$ROOT/build}")
OUT=$(realpath -m "${2:-$BUILD/dist}")
shift $(( $# > 2 ? 2 : $# )) || true
KINDS=("$@")
if [ ${#KINDS[@]} -eq 0 ]; then
    if command -v dpkg-deb > /dev/null; then KINDS=(deb appimage); else KINDS=(rpm); fi
fi
mkdir -p "$OUT"
VERSION=$(sed -nE 's/^project\(.*VERSION ([0-9.]+).*/\1/p; s/^ +VERSION ([0-9.]+).*/\1/p' "$ROOT/CMakeLists.txt" | head -1)

for kind in "${KINDS[@]}"; do
    case "$kind" in
    deb | rpm)
        G=$(echo "$kind" | tr a-z A-Z)
        (cd "$BUILD" && cpack -G "$G" -B "$OUT/cpack")
        cp "$OUT"/cpack/*."$kind" "$OUT"/
        rm -rf "$OUT/cpack"
        ;;
    appimage)
        APPDIR="$OUT/AppDir"
        rm -rf "$APPDIR"
        DESTDIR="$APPDIR" cmake --install "$BUILD" --prefix /usr
        # La biblioteca de scripts de Tcl con que se compiló (y sus módulos .tm, en la carpeta hermana tcl<mayor>).
        TCLVER=$(ldd "$APPDIR/usr/bin/flatcam" | sed -nE 's/.*libtcl([0-9]+\.[0-9]+)\.so.*/\1/p' | head -1)
        INIT=$(find /usr/share/tcltk /usr/share /usr/lib /usr/lib64 -path "*tcl$TCLVER/init.tcl" 2>/dev/null | head -1)
        if [ -n "$INIT" ]; then
            TCLDIR=$(dirname "$INIT")
            mkdir -p "$APPDIR/usr/share/flatcam"
            cp -rL "$TCLDIR" "$APPDIR/usr/share/flatcam/tcl$TCLVER"
            MODS="$(dirname "$TCLDIR")/tcl${TCLVER%%.*}"
            [ -d "$MODS" ] && cp -rL "$MODS" "$APPDIR/usr/share/flatcam/tcl${TCLVER%%.*}"
        else
            echo "Aviso: no encontré la biblioteca de scripts de Tcl $TCLVER" >&2
        fi
        TOOLS="$OUT/.linuxdeploy"
        mkdir -p "$TOOLS"
        ARCH=$(uname -m)
        for t in linuxdeploy linuxdeploy-plugin-qt; do
            f="$TOOLS/$t-$ARCH.AppImage"
            [ -x "$f" ] || { curl -fsSL -o "$f" \
                "https://github.com/linuxdeploy/$t/releases/download/continuous/$t-$ARCH.AppImage"; chmod +x "$f"; }
        done
        export PATH="$TOOLS:$PATH" APPIMAGE_EXTRACT_AND_RUN=1 QMAKE=${QMAKE:-$(command -v qmake6 || command -v qmake)}
        export EXTRA_PLATFORM_PLUGINS="libqwayland-generic.so" LINUXDEPLOY_OUTPUT_VERSION="$VERSION"
        export EXTRA_QT_MODULES="svg"
        (cd "$OUT" && "$TOOLS/linuxdeploy-$ARCH.AppImage" --appdir "$APPDIR" --plugin qt --output appimage \
            -d "$APPDIR/usr/share/applications/flatcam.desktop" -i "$APPDIR/usr/share/icons/hicolor/256x256/apps/flatcam.png")
        for f in "$OUT"/FlatCAM-*.AppImage; do mv "$f" "$OUT/FlatCAM-$VERSION-Linux-$ARCH.AppImage"; done
        rm -rf "$APPDIR"
        ;;
    *) echo "Tipo desconocido: $kind (deb, rpm o appimage)" >&2; exit 1 ;;
    esac
done
ls -la "$OUT"
