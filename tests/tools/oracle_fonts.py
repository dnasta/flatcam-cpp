#!/usr/bin/env python3
"""Oráculo de los textos con fuentes (`appParsers/ParseFont.py`, la herramienta Texto del editor de Geometría).

Arma en `tests/data/fonts/` copias reducidas (solo ASCII y algunas letras latinas, con fontTools) de fuentes DejaVu
(licencia libre: ver `tests/data/fonts/LICENSE`) y ejecuta el `ParseFont` ORIGINAL con freetype-py y fontTools:
`get_fonts`, `get_fonts_by_types` (qué nombre queda en cada lista) y `font_to_geometry` (los polígonos de cada contorno de
cada letra, con sus puntos tal cual). Escribe `tests/data/fonts/oracle.json`.

Uso:  MAMBA_ROOT_PREFIX=$HOME/.local/share/mamba micromamba run -n old python tests/tools/oracle_fonts.py
(necesita `python -m pip install freetype-py fonttools` en el entorno `old`).
"""
import glob
import json
import os
import sys
from unittest import mock

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from oracle_common import DATA, REF, install_stubs, load  # noqa: E402

FONTS = os.path.join(DATA, "fonts")
OUT = os.path.join(FONTS, "oracle.json")

# Fuentes de matplotlib (entorno `old`) que se copian reducidas.
SOURCES = ["DejaVuSans.ttf", "DejaVuSans-Bold.ttf", "DejaVuSans-Oblique.ttf", "DejaVuSans-BoldOblique.ttf",
           "DejaVuSerif-Italic.ttf", "DejaVuSansMono.ttf"]
TEXT = "".join(chr(c) for c in range(32, 127)) + "ñÑáéíóúü°"


def make_fonts():
    import matplotlib
    from fontTools import subset
    src = os.path.join(os.path.dirname(matplotlib.__file__), "mpl-data", "fonts", "ttf")
    os.makedirs(FONTS, exist_ok=True)
    lic = None
    for name in SOURCES:
        out = os.path.join(FONTS, name)
        opts = subset.Options()
        opts.hinting = True          # el original carga las letras con hinting (FT_LOAD_RENDER)
        opts.legacy_kern = True      # la tabla `kern` (la que lee FT_Get_Kerning)
        opts.name_IDs = ["*"]
        opts.name_languages = ["*"]
        opts.notdef_outline = True
        font = subset.load_font(os.path.join(src, name), opts)
        sub = subset.Subsetter(opts)
        sub.populate(text=TEXT)
        sub.subset(font)
        subset.save_font(font, out, opts)
        if lic is None:
            lic = next(r.toUnicode() for r in font["name"].names if r.nameID == 13)
    with open(os.path.join(FONTS, "LICENSE"), "w", encoding="utf-8") as f:
        f.write("Copias reducidas (fontTools.subset) de las fuentes DejaVu que trae matplotlib, para los tests.\n"
                "Licencia de las fuentes (tabla 'name', identificador 13):\n\n" + lic + "\n")


def poly_state(g):
    """Los polígonos de un MultiPolygon, con el anillo exterior tal cual (los puntos de FreeType escalados)."""
    return [[[x, y] for x, y in p.exterior.coords] for p in g.geoms]


def main():
    make_fonts()
    install_stubs()
    mod = load("ParseFont", os.path.join(REF, "appParsers", "ParseFont.py"))
    app = mock.MagicMock()
    pf = mod.ParseFont(app)
    files = pf.get_fonts(paths=FONTS)
    pf.get_fonts = lambda paths=None: files  # en vez de las carpetas del sistema
    pf.get_fonts_by_types()
    base = lambda d: {k: os.path.basename(v) for k, v in sorted(d.items())}  # noqa: E731
    out = {"files": sorted(os.path.basename(f) for f in files),
           "regular": base(pf.regular_f), "bold": base(pf.bold_f), "italic": base(pf.italic_f),
           "bi": base(pf.bold_italic_f), "names": {}, "cases": []}
    for f in sorted(files):
        out["names"][os.path.basename(f)] = list(mod.ParseFont.get_font_name(f))

    cases = [
        ("FlatCAM", "DejaVu Sans", "regular", "10", "MM", 0, 0),
        ("AV To.", "DejaVu Sans", "regular", "10", "MM", 0, 0),        # pares con kerning
        ("Bg 1", "DejaVu Sans", "bold", "12", "MM", 0, 0),
        ("ok", "DejaVu Sans", "bi", "8", "MM", 0, 0),
        ("Italic", "DejaVu Sans", "italic", "10", "MM", 0, 0),           # se guardó con otro nombre: falla
        ("Ñandú°", "DejaVu Serif", "italic", "20", "IN", 0, 0),
        ("x y", "DejaVu Sans Mono", "regular", "14", "MM", 5, -3),
        ("a\nb", "DejaVu Sans", "regular", "10", "MM", 0, 0),            # el salto de línea es la letra 0
        ("", "DejaVu Sans", "regular", "10", "MM", 0, 0),
        ("hola", "No Existe", "regular", "10", "MM", 0, 0),
        ("o", "DejaVu Sans", "regular", "72", "MM", 0, 0),
    ]
    for txt, name, kind, size, units, cx, cy in cases:
        app.inform.emit.reset_mock()
        r = pf.font_to_geometry(txt, font_name=name, font_type=kind, font_size=size, units=units, coordx=cx,
                                coordy=cy)
        case = {"text": txt, "font": name, "type": kind, "size": size, "units": units, "x": cx, "y": cy,
                "inform": [c.args[0] for c in app.inform.emit.call_args_list]}
        if isinstance(r, str):
            case["result"] = r
            # Lo que pasa al hacer clic con esa "geometría" (AppGeoEditor.FCText.click).
            from shapely import affinity
            try:
                affinity.translate(r, xoff=1, yoff=1)
            except Exception as e:  # noqa: BLE001
                case["click_error"] = str(e)
        else:
            case["empty"] = bool(r.is_empty)
            case["polygons"] = poly_state(r)
        out["cases"].append(case)
    with open(OUT, "w", encoding="utf-8") as f:
        json.dump(out, f, indent=1, ensure_ascii=False)
        f.write("\n")
    print("escrito", OUT, len(out["cases"]), "casos")


if __name__ == "__main__":
    main()
