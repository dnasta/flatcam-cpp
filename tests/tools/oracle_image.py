#!/usr/bin/env python3
"""Oráculo de Importar imagen: arma imágenes chicas en `tests/data/images/` (gris, RGB, RGBA, con paleta) y ejecuta
`Geometry.import_image` ORIGINAL de `camlib.py` (tomado con `ast`, con `rasterio` real). Escribe en
`tests/golden_image.h` las bandas que lee rasterio y el resultado (área, límites, polígonos y agujeros).

Necesita rasterio, que no está en el entorno `old`:
  micromamba create -n raster -c conda-forge python=3.11 rasterio shapely numpy pillow
Uso:  MAMBA_ROOT_PREFIX=$HOME/.local/share/mamba micromamba run -n raster python tests/tools/oracle_image.py
"""
import ast
import logging
import os

import numpy as np
from PIL import Image

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
REF = os.path.join(ROOT, "reference", "FlatCAM_2024_4", "FlatCAM")
DIR = os.path.join(ROOT, "tests", "data", "images")
OUT = os.path.join(ROOT, "tests", "golden_image.h")


def make_images():
    rng = np.random.default_rng(7)
    w, h = 23, 17
    gray = np.full((h, w), 255, np.uint8)
    gray[3:9, 2:12] = 0          # rectángulo
    gray[5:7, 5:8] = 255         # agujero
    gray[12:15, 14:20] = 100     # otro valor (región aparte que también entra)
    gray[10, 1] = 40             # píxel suelto
    gray[11, 2] = 40             # tocándose en diagonal
    gray[0:2, 20:23] = 200       # no entra con 128
    Image.fromarray(gray, "L").save(os.path.join(DIR, "gray.png"))
    rgb = np.full((h, w, 3), 255, np.uint8)
    rgb[2:8, 3:10, 0] = 50       # rojo bajo
    rgb[9:14, 12:21, 1] = 30     # verde bajo
    rgb[4:6, 15:22, 2] = 10      # azul bajo
    rgb[12:16, 1:4] = rng.integers(0, 255, (4, 3, 3))
    Image.fromarray(rgb, "RGB").save(os.path.join(DIR, "rgb.png"))
    rgba = np.dstack([rgb, np.full((h, w), 255, np.uint8)])
    Image.fromarray(rgba, "RGBA").save(os.path.join(DIR, "rgba.png"))
    pal = Image.fromarray(gray, "L").convert("P", palette=Image.ADAPTIVE, colors=8)
    pal.save(os.path.join(DIR, "palette.png"))
    Image.fromarray(rgb, "RGB").save(os.path.join(DIR, "rgb.bmp"))


def main():
    make_images()
    import rasterio
    from rasterio.features import shapes
    from shapely.geometry import shape
    from shapely.affinity import scale, translate
    from shapely.ops import unary_union
    src = os.path.join(REF, "camlib.py")
    tree = ast.parse(open(src, encoding="utf-8").read())
    cls = next(n for n in tree.body if isinstance(n, ast.ClassDef) and n.name == "Geometry")
    fn = next(f for f in cls.body if isinstance(f, ast.FunctionDef) and f.name == "import_image")
    flat = next(f for f in cls.body if isinstance(f, ast.FunctionDef) and f.name == "flatten_list")
    m = ast.Module(body=[ast.ClassDef(name="G", bases=[], keywords=[], body=[fn, flat], decorator_list=[])],
                   type_ignores=[])
    ast.fix_missing_locations(m)
    ns = dict(rasterio=rasterio, shapes=shapes, shape=shape, scale=scale, translate=translate,
              unary_union=unary_union, np=np, log=logging.getLogger("x"), Iterable=__import__("collections").abc.Iterable)
    exec(compile(m, src, "exec"), ns)
    G = ns["G"]
    cases = [("gray.png", "MM", 96, "black", [128, 0, 0, 0]), ("gray.png", "IN", 300, "black", [250, 0, 0, 0]),
             ("rgb.png", "MM", 96, "color", [0, 128, 128, 128]), ("rgba.png", "MM", 72, "color", [0, 60, 40, 20]),
             ("palette.png", "MM", 96, "black", [3, 0, 0, 0]), ("rgb.png", "MM", 96, "black", [128, 0, 0, 0]),
             ("rgb.bmp", "MM", 96, "color", [0, 128, 128, 128])]
    lines = []
    bands = {}
    for f, units, dpi, mode, mask in cases:
        path = os.path.join(DIR, f)
        with rasterio.open(path) as s:
            b = [s.read(i + 1).astype(float).ravel().tolist() for i in range(min(s.count, 3))]
            bands[f] = (s.width, s.height, b)
        g = G()
        g.solid_geometry = None
        g.import_image(path, units=units, dpi=dpi, mode=mode, mask=mask)
        sg = g.solid_geometry
        polys = list(sg.geoms) if hasattr(sg, "geoms") else ([sg] if not sg.is_empty else [])
        holes = sum(len(p.interiors) for p in polys)
        lines.append("  {%s, %s, %r, %s, {%s}, %r, {%s}, %d, %d}," % (
            '"%s"' % f, '"%s"' % units, float(dpi), '"%s"' % mode, ", ".join(repr(float(x)) for x in mask), sg.area,
            ", ".join(repr(x) for x in (sg.bounds if not sg.is_empty else (0, 0, 0, 0))), len(polys), holes))
    out = ["// GENERADO por tests/tools/oracle_image.py. No editar.", "#pragma once", "#include <map>", "#include <string>",
           "#include <vector>", "namespace golden_image {",
           "struct Bands { int width; int height; std::vector<std::vector<double>> bands; };",
           "inline const std::map<std::string, Bands> kBands = {"]
    for f, (w, h, b) in bands.items():
        out.append('  {"%s", {%d, %d, {%s}}},' % (f, w, h, ", ".join("{%s}" % ", ".join("%g" % v for v in band)
                                                                     for band in b)))
    out += ["};", "struct Case { const char* file; const char* units; double dpi; const char* mode; std::vector<double> mask;"
            " double area; std::vector<double> bounds; int polygons; int holes; };",
            "inline const std::vector<Case> kCases = {"] + lines + ["};", "}  // namespace golden_image", ""]
    open(OUT, "w").write("\n".join(out))
    print("escrito", OUT)


if __name__ == "__main__":
    main()
