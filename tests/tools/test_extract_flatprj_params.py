#!/usr/bin/env python3
"""Prueba de tools/extract_flatprj_params.py con un proyecto falso (texto plano y comprimido con LZMA).

Ejecutar:  python3 tests/tools/test_extract_flatprj_params.py
"""
import importlib.util
import json
import lzma
import os
import random
import sys
import tempfile
import unittest

HERE = os.path.dirname(os.path.abspath(__file__))
spec = importlib.util.spec_from_file_location(
    "extract_flatprj_params", os.path.join(HERE, "..", "..", "tools", "extract_flatprj_params.py"))
mod = importlib.util.module_from_spec(spec)
spec.loader.exec_module(mod)


def shply(rng, n=60000):
    """Imita una figura serializada por FlatCAM: una cadena enorme en una sola línea."""
    return {"__class__": "Shply", "__inst__": "".join(rng.choice("0123456789abcdef") for _ in range(n))}


def fake_project():
    rng = random.Random(1)
    return {
        "version": 2024.4,
        "options": {"units": "MM", "tools_ncc_overlap": 0.8, "global_last_folder": "/Users/alguien/privado",
                    "name": "acentuado: áé \"comillas\""},
        "objs": [
            {"kind": "gerber", "options": {"name": "B_Cu.gbr", "plot": True},
             "solid_geometry": [shply(rng), shply(rng)], "follow_geometry": [shply(rng)],
             "apertures": {"10": {"size": 0.15, "geometry": [shply(rng)]}}},
            {"kind": "geometry", "multigeo": True, "options": {"name": "B_Cu.gbr_ncc"},
             "tools": {"1": {"tooldia": 0.1, "data": {"tools_ncc_overlap": 0.8, "tools_ncc_method": "standard",
                                                      "feedrate": 500.0, "laser_power": 300},
                             "solid_geometry": [shply(rng), shply(rng)]}}},
            {"kind": "cncjob", "options": {"name": "B_Cu.nc"}, "gcode": "G01 X0 Y0\n" * 5000,
             "gcode_parsed": [{"geom": shply(rng)}],
             "cnc_tools": {"0.1": {"gcode": "G00 X1\n" * 20000, "data": {"feedrate": 500.0, "spindlespeed": 300},
                                   "solid_geometry": [shply(rng)]}}},
            {"kind": "excellon", "options": {"name": "Drill.drl"},
             "tools": {"1": {"tooldia": 0.8, "drills": [{"x": 1}] * 50, "slots": [], "solid_geometry": [shply(rng)]}}},
        ],
    }


def expected_of(d):
    skip = mod.SKIP_KEYS

    def clean(o):
        if isinstance(o, dict):
            return {k: ("<omitido>" if k in skip else clean(v)) for k, v in o.items()}
        if isinstance(o, list):
            return [clean(x) for x in o]
        return o
    return clean(d)


class ExtractTest(unittest.TestCase):
    def check(self, writer):
        d = fake_project()
        with tempfile.TemporaryDirectory() as tmp:
            path = os.path.join(tmp, "p.FlatPrj")
            writer(path, json.dumps(d, indent=2, sort_keys=True))
            res = mod.extract(path, progress=False)
        self.assertEqual(res, expected_of(d))

    def test_texto_plano(self):
        def w(p, t):
            with open(p, "w", encoding="utf-8") as f:
                f.write(t)
        self.check(w)

    def test_lzma(self):
        def w(p, t):
            with lzma.open(p, "wb", preset=3) as f:
                f.write(t.encode("utf-8"))
        self.check(w)

    def test_filtro_de_opciones_globales(self):
        opts = fake_project()["options"]
        self.assertEqual(mod.filter_app_options(opts), {"units": "MM", "tools_ncc_overlap": 0.8})


class KeepDrillsOption(unittest.TestCase):
    """`--con-agujeros` conserva las posiciones de los agujeros de los objetos Excellon."""

    def setUp(self):
        self.saved = set(mod.SKIP_KEYS)

    def tearDown(self):
        mod.SKIP_KEYS.clear()
        mod.SKIP_KEYS.update(self.saved)

    def run_extractor(self, extra):
        prj = {"version": 2024.4, "options": {},
               "objs": [{"kind": "excellon", "options": {"name": "Alinea"},
                         "tools": {"1": {"tooldia": 0.4, "drills": [{"x": 0.0, "y": 0.0}, {"x": -38.0, "y": 39.5}],
                                         "slots": [], "solid_geometry": [{"a": 1}]}}}]}
        with tempfile.TemporaryDirectory() as d:
            src = os.path.join(d, "p.FlatPrj")
            out = os.path.join(d, "o.json")
            with open(src, "w") as f:
                json.dump(prj, f, indent=2, sort_keys=True)
            mod.main(["x", src, out] + extra)
            with open(out) as f:
                return json.load(f)

    def test_drills_are_omitted_by_default(self):
        res = self.run_extractor([])
        self.assertEqual(res["objs"][0]["tools"]["1"]["drills"], "<omitido>")

    def test_drills_are_kept_with_the_option(self):
        res = self.run_extractor(["--con-agujeros"])
        self.assertEqual(res["objs"][0]["tools"]["1"]["drills"], [{"x": 0.0, "y": 0.0}, {"x": -38.0, "y": 39.5}])


if __name__ == "__main__":
    unittest.main()
