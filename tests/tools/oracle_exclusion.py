"""`ExclusionAreas.travel_coordinates` ORIGINAL (appCommon/Common.py), tomado con `ast`, para los oráculos de G-code.

`exc_areas(app, areas)` devuelve un objeto con `exclusion_areas_storage` y el `travel_coordinates` original; `areas` es
una lista de (WKT, estrategia, Z por encima).
"""
import ast
import os
from copy import deepcopy

import numpy as np
from shapely import wkt as shapely_wkt
from shapely.geometry import LineString, Point

from oracle_common import REF


def _load():
    path = os.path.join(REF, "appCommon", "Common.py")
    tree = ast.parse(open(path, encoding="utf-8").read())
    funcs = [n for n in tree.body if isinstance(n, ast.FunctionDef) and n.name in ("nearest_point", "farthest_point")]
    cls = next(n for n in tree.body if isinstance(n, ast.ClassDef) and n.name == "ExclusionAreas")
    meth = next(f for f in cls.body if isinstance(f, ast.FunctionDef) and f.name == "travel_coordinates")
    module = ast.Module(body=funcs + [ast.ClassDef(name="ExcCore", bases=[], keywords=[], body=[meth],
                                                   decorator_list=[])], type_ignores=[])
    ast.fix_missing_locations(module)
    ns = dict(LineString=LineString, Point=Point, deepcopy=deepcopy, np=np)
    exec(compile(module, path, "exec"), ns)
    return ns["ExcCore"]


ExcCore = _load()


def exc_areas(app, areas, obj_type="geometry"):
    e = ExcCore()
    e.app = app
    e.exclusion_areas_storage = [{"obj_type": obj_type, "shape": shapely_wkt.loads(w), "strategy": s, "overz": z}
                                 for w, s, z in areas]
    return e


def cpp_areas(areas, obj_type, var):
    """Líneas de C++ que cargan las mismas áreas en `var` (un vector de flatcam::ExclusionArea)."""
    return ['    %s.push_back(flatcam::ExclusionArea{"%s", flatcam::Shape::fromWkt("%s"), "%s", %r});' % (
        var, obj_type, w, s, float(z)) for w, s, z in areas]
