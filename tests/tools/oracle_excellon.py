#!/usr/bin/env python3
"""Oráculo: corre el parser de Excellon ORIGINAL de FlatCAM sobre los archivos de prueba.

Genera `tests/golden_excellon.h` con las herramientas, los agujeros y las ranuras que lee el original, para que
los tests de C++ comprueben que el port lee lo mismo. Incluye el Excellon real del proyecto y archivos
sintéticos en `tests/data/excellon_samples/` que ejercitan las ramas menos comunes del parser (coordenadas sin
punto, ranuras, repeticiones, archivos sin cabecera...).

Requisitos (solo para regenerar el archivo): python3 -m pip install shapely numpy
Uso:  python3 tests/tools/oracle_excellon.py
"""
import os
import sys

from shapely.ops import unary_union

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from oracle_common import DATA, REF, ROOT, FakeApp, install_stubs, load  # noqa: E402

OUT = os.path.join(ROOT, "tests", "golden_excellon.h")
FILES = ["PCB principal-Drill.drl"] + ["excellon_samples/" + f for f in sorted(os.listdir(os.path.join(DATA, "excellon_samples")))]


def run(path, camlib, parse_exc):
    app = FakeApp()
    parse_exc.Excellon.app = app
    camlib.Geometry.app = app
    d = app.defaults
    ex = parse_exc.Excellon(zeros=d["excellon_zeros"], excellon_format_upper_mm=d["excellon_format_upper_mm"],
                            excellon_format_lower_mm=d["excellon_format_lower_mm"],
                            excellon_format_upper_in=d["excellon_format_upper_in"],
                            excellon_format_lower_in=d["excellon_format_lower_in"],
                            excellon_units=d["excellon_units"], geo_steps_per_circle=64)
    ex.default_data = {}
    status = ex.parse_file(path)
    if status != "fail":
        ex.create_geometry()
    tools = []
    for tid in sorted(ex.tools):
        t = ex.tools[tid]
        solids = t.get("solid_geometry", [])
        tools.append({
            "id": tid, "dia": t.get("tooldia", 0.0),
            "drills": [(p.x, p.y) for p in t.get("drills", [])],
            "slots": [(a.x, a.y, b.x, b.y) for a, b in t.get("slots", [])],
            "area": unary_union(solids).area if solids else 0.0,
        })
    return {"failed": status == "fail", "units": ex.units, "zeros": ex.zeros or "",
            "upper_mm": int(ex.excellon_format_upper_mm), "lower_mm": int(ex.excellon_format_lower_mm),
            "upper_in": int(ex.excellon_format_upper_in), "lower_in": int(ex.excellon_format_lower_in),
            "n_solid": len(ex.solid_geometry or []), "tools": tools}


def main():
    install_stubs()
    sys.path.insert(0, REF)
    camlib = load("camlib", os.path.join(REF, "camlib.py"))
    parse_exc = load("ParseExcellon", os.path.join(REF, "appParsers", "ParseExcellon.py"))
    num = lambda v: repr(float(v))
    out = ["// ARCHIVO GENERADO por tests/tools/oracle_excellon.py - no editar a mano.",
           "// Referencia: parser de Excellon original de FlatCAM.",
           "#pragma once", "#include <string>", "#include <vector>", "namespace golden_excellon {",
           "struct Pt { double x, y; };", "struct Slot { double x0, y0, x1, y1; };",
           "struct ToolG { int id; double dia; std::vector<Pt> drills; std::vector<Slot> slots; double area; };",
           "struct FileG { const char* file; bool failed; const char* units; const char* zeros; int upperMm, lowerMm, upperIn, lowerIn; int nSolid; std::vector<ToolG> tools; };",
           "inline const std::vector<FileG> kFiles = {"]
    for name in FILES:
        print("Procesando", name, flush=True)
        r = run(os.path.join(DATA, name), camlib, parse_exc)
        tools = ",\n  ".join('{%d, %s, {%s}, {%s}, %s}' % (
            t["id"], num(t["dia"]), ", ".join("{%s, %s}" % (num(x), num(y)) for x, y in t["drills"]),
            ", ".join("{%s, %s, %s, %s}" % tuple(num(v) for v in s) for s in t["slots"]), num(t["area"])) for t in r["tools"])
        out.append('{"%s", %s, "%s", "%s", %d, %d, %d, %d, %d, {\n  %s}},' % (
            name, "true" if r["failed"] else "false", r["units"], r["zeros"], r["upper_mm"], r["lower_mm"],
            r["upper_in"], r["lower_in"], r["n_solid"], tools))
        print("   unidades=%s ceros=%r herramientas=%s agujeros=%s ranuras=%s" % (
            r["units"], r["zeros"], [t["id"] for t in r["tools"]], [len(t["drills"]) for t in r["tools"]],
            [len(t["slots"]) for t in r["tools"]]))
    out.append("};")
    out.append("}  // namespace golden_excellon")
    with open(OUT, "w") as f:
        f.write("\n".join(out) + "\n")
    print("Escrito", OUT, "(%.0f KB)" % (os.path.getsize(OUT) / 1024))


if __name__ == "__main__":
    main()
