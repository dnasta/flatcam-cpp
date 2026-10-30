#!/usr/bin/env python3
"""Oráculo: corre el parser de Gerber ORIGINAL de FlatCAM sobre los archivos de prueba y resume el resultado.

Sirve para generar `tests/golden_gerber.h`, que usan los tests de C++ para comprobar que el port produce la
misma geometría. Carga `reference/FlatCAM_2024_4/FlatCAM/appParsers/ParseGerber.py` y `camlib.py` sin abrir la
interfaz (ver `oracle_common.py`).

Requisitos (solo para regenerar el archivo): python3 -m pip install shapely numpy
Uso:  python3 tests/tools/oracle_gerber.py
"""
import json
import os
import random
import sys

import shapely
from shapely.geometry import Point, Polygon
from shapely.ops import unary_union

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from oracle_common import DATA, REF, ROOT, FakeApp, install_stubs, load  # noqa: E402

OUT = os.path.join(ROOT, "tests", "golden_gerber.h")

FILES = ["PCB principal-B_Cu.gbl", "PCB principal-Edge_Cuts.gm1", "PCB principal-B_Logo.gbr",
         "PCB principal-B_Mask.gbs", "PCB principal-F_Silkscreen.gto"]


def flat_polys(g):
    """Aplana cualquier resultado de Shapely en una lista de polígonos."""
    out = []
    def rec(o):
        if o is None:
            return
        if isinstance(o, (list, tuple)):
            for x in o:
                rec(x)
        elif hasattr(o, "geoms"):
            for x in o.geoms:
                rec(x)
        elif isinstance(o, Polygon):
            if not o.is_empty:
                out.append(o)
    rec(g)
    return out


def summarize(path, camlib, parse_gerber):
    app = FakeApp()
    parse_gerber.Gerber.app = app
    camlib.Geometry.app = app
    g = parse_gerber.Gerber(steps_per_circle=64)
    g.parse_file(path)
    polys = flat_polys(g.solid_geometry)
    union = unary_union(polys)
    follow = []
    def rec(o):
        if isinstance(o, (list, tuple)):
            for x in o: rec(x)
        elif hasattr(o, "geoms"):
            for x in o.geoms: rec(x)
        else:
            follow.append(o)
    rec(g.follow_geometry)
    lines = [f for f in follow if f.geom_type in ("LineString", "LinearRing")]
    rings = [f for f in follow if f.geom_type == "LinearRing"]
    points = [f for f in follow if f.geom_type == "Point"]
    ap_types = {}
    for k, v in g.apertures.items():
        ap_types[k] = {kk: vv for kk, vv in v.items() if kk in ("type", "size", "width", "height", "diam", "nVertices")}
        ap_types[k]["n_geometry"] = len(v.get("geometry", []))
        if v.get("type") == "AM":
            ap_types[k]["macro"] = v["macro"].name
            ap_types[k]["modifiers"] = v["modifiers"]
    rng = random.Random(12345)
    minx, miny, maxx, maxy = union.bounds
    inside, outside = [], []
    boundary = union.boundary
    tries = 0
    # Puntos de muestra: la mitad adentro y la mitad afuera, evitando los que quedan casi sobre el borde.
    while (len(inside) < 600 or len(outside) < 600) and tries < 400000:
        tries += 1
        p = Point(rng.uniform(minx, maxx), rng.uniform(miny, maxy))
        if boundary.distance(p) < 1e-4:
            continue
        if union.contains(p):
            if len(inside) < 600:
                inside.append([round(p.x, 6), round(p.y, 6)])
        elif len(outside) < 600:
            outside.append([round(p.x, 6), round(p.y, 6)])
    return {
        "units": g.units, "int_digits": g.int_digits, "frac_digits": g.frac_digits, "zeros": g.gerber_zeros,
        "macros": sorted(g.aperture_macros.keys()),
        "apertures": ap_types,
        "n_polygons": len(polys),
        "n_polygons_after_union": len(flat_polys(union)),
        "area": union.area,
        "area_sum_of_parts": sum(p.area for p in polys),
        "bounds": list(union.bounds),
        "n_holes": sum(len(p.interiors) for p in flat_polys(union)),
        "n_vertices": sum(len(p.exterior.coords) + sum(len(i.coords) for i in p.interiors) for p in flat_polys(union)),
        "follow_n_lines": len(lines), "follow_n_points": len(points),
        "follow_length": sum(l.length for l in lines),
        "inside": inside, "outside": outside,
    }


def main():
    install_stubs()
    sys.path.insert(0, REF)
    camlib = load("camlib", os.path.join(REF, "camlib.py"))
    parse_gerber = load("ParseGerber", os.path.join(REF, "appParsers", "ParseGerber.py"))
    result = {"shapely": shapely.__version__, "geos": shapely.geos_version_string, "files": {}}
    for name in FILES:
        print("Procesando", name, flush=True)
        result["files"][name] = summarize(os.path.join(DATA, name), camlib, parse_gerber)
        r = result["files"][name]
        print("   polígonos: %d  área: %.6f  límites: %s" % (r["n_polygons_after_union"], r["area"], [round(b, 4) for b in r["bounds"]]))
    write_header(result)
    print("Escrito", OUT, "(%.0f KB)" % (os.path.getsize(OUT) / 1024))


def write_header(result):
    """Escribe los valores de referencia como un encabezado C++ (sin dependencias de JSON)."""
    def num(v):
        return repr(float(v))
    out = ["// ARCHIVO GENERADO por tests/tools/oracle_gerber.py - no editar a mano.",
           "// Referencia: parser de Gerber original de FlatCAM con Shapely %s (GEOS %s)." % (result["shapely"], result["geos"]),
           "#pragma once", "#include <string>", "#include <vector>", "namespace golden_gerber {",
           "struct Sample { double x, y; };",
           "struct ApertureInfo { const char* id; const char* type; double size; double width; double height; int nGeometry; };",
           "struct FileGolden {",
           "    const char* file; const char* units; int intDigits; int fracDigits; char zeros;",
           "    std::vector<std::string> macros; int nPolygons; double area; double bounds[4]; int nHoles;",
           "    int followLines; int followPoints; double followLength;",
           "    std::vector<ApertureInfo> apertures; std::vector<Sample> inside; std::vector<Sample> outside;",
           "};",
           "inline const std::vector<FileGolden> kFiles = {"]
    for name, r in result["files"].items():
        aps = ", ".join('{"%s", "%s", %s, %s, %s, %d}' % (
            k, v.get("type", ""), num(v.get("size", 0.0)), num(v.get("width", 0.0)), num(v.get("height", 0.0)), v["n_geometry"])
            for k, v in sorted(r["apertures"].items(), key=lambda kv: kv[0]))
        macros = ", ".join('"%s"' % m for m in r["macros"])
        pts = lambda L: ", ".join("{%s, %s}" % (num(x), num(y)) for x, y in L)
        out.append('{"%s", "%s", %d, %d, \'%s\', {%s}, %d, %s, {%s, %s, %s, %s}, %d, %d, %d, %s,\n {%s},\n {%s},\n {%s}},' % (
            name, r["units"], r["int_digits"], r["frac_digits"], r["zeros"], macros, r["n_polygons_after_union"],
            num(r["area"]), num(r["bounds"][0]), num(r["bounds"][1]), num(r["bounds"][2]), num(r["bounds"][3]),
            r["n_holes"], r["follow_n_lines"], r["follow_n_points"], num(r["follow_length"]), aps, pts(r["inside"]), pts(r["outside"])))
    out.append("};")
    out.append("}  // namespace golden_gerber")
    with open(OUT, "w") as f:
        f.write("\n".join(out) + "\n")


if __name__ == "__main__":
    main()
