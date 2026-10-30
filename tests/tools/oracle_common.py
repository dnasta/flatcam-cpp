#!/usr/bin/env python3
"""Utilidades comunes de los oráculos: cargan el código ORIGINAL de FlatCAM sin abrir la interfaz.

Las dependencias de Qt, rtree, ezdxf, etc. se reemplazan por módulos vacíos y se inyectan los nombres que el
original obtiene con `from ... import *` de módulos que aquí no se cargan.
"""
import importlib.util
import logging
import os
import re
import sys
import types
import warnings
from unittest import mock

import numpy as np
from shapely.geometry import LineString, MultiLineString, MultiPolygon, Point, Polygon

warnings.filterwarnings("ignore")
sys.dont_write_bytecode = True  # no dejar __pycache__ dentro de reference/

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", ".."))
REF = os.path.join(ROOT, "reference", "FlatCAM_2024_4", "FlatCAM")
DATA = os.path.join(ROOT, "tests", "data")

# Valores por defecto de FlatCAM 8.994/2024.4 usados por el usuario (tests/data/PARAMETROS.md).
DEFAULTS = {
    "units": "MM", "gerber_circle_steps": 64, "gerber_def_units": "MM", "gerber_def_zeros": "T",
    "gerber_use_buffer_for_union": True, "gerber_buffering": "full", "gerber_extra_buffering": True,
    "gerber_simplification": False, "gerber_simp_tolerance": 0.0005, "gerber_clean_apertures": True,
    "excellon_zeros": "T", "excellon_units": "METRIC", "excellon_format_upper_mm": 3,
    "excellon_format_lower_mm": 3, "excellon_format_upper_in": 2, "excellon_format_lower_in": 4,
    "geometry_circle_steps": 64,
}


def make_stub(name, **attrs):
    mod = types.ModuleType(name)
    for k, v in attrs.items():
        setattr(mod, k, v)
    sys.modules[name] = mod
    return mod


def install_stubs():
    # El original usa nombres que NumPy 2 eliminó.
    for old, new in (('Inf', np.inf), ('NaN', np.nan)):
        if not hasattr(np, old):
            setattr(np, old, new)
    """Reemplaza las dependencias que no hacen falta para parsear."""
    for name in ["PyQt5", "PyQt5.QtWidgets", "PyQt5.QtCore", "PyQt5.QtGui", "rtree", "lxml", "lxml.etree",
                 "descartes", "descartes.patch", "rasterio", "rasterio.features", "ezdxf", "ortools",
                 "ortools.constraint_solver", "svg", "svg.path"]:
        sys.modules[name] = mock.MagicMock()
    sys.modules["PyQt5"].QtWidgets = sys.modules["PyQt5.QtWidgets"]
    sys.modules["PyQt5"].QtCore = sys.modules["PyQt5.QtCore"]
    make_stub("appLogger", getLogger=lambda n: logging.getLogger(n))
    make_stub("appTranslation", apply_language=lambda *a, **k: (lambda s: s))
    make_stub("appCommon")
    make_stub("appCommon.Common", GracefulException=type("GracefulException", (Exception,), {}))
    make_stub("appParsers")
    make_stub("appParsers.ParseSVG", svgparselength=None, getsvggeo=None, svgparse_viewbox=None)
    make_stub("appParsers.ParseDXF")
    make_stub("appGUI")
    make_stub("appGUI.PlotCanvasLegacy", ShapeCollectionLegacy=mock.MagicMock())
    import builtins
    builtins._ = lambda s: s


def load(name, path):
    spec = importlib.util.spec_from_file_location(name, path)
    mod = importlib.util.module_from_spec(spec)
    sys.modules[name] = mod
    # Nombres que en el original llegan por `from ... import *` de módulos que aquí se reemplazan.
    extras = dict(LineString=LineString, MultiLineString=MultiLineString, MultiPolygon=MultiPolygon,
                  Polygon=Polygon, Point=Point, np=np, re=re, sys=sys, getLogger=logging.getLogger,
                  fcTranslate=sys.modules["appTranslation"])
    for k, v in extras.items():
        setattr(mod, k, v)
    spec.loader.exec_module(mod)
    for k, v in extras.items():
        if not hasattr(mod, k):
            setattr(mod, k, v)
    return mod


class FakeApp:
    """Lo mínimo de la aplicación que necesita el parser."""
    def __init__(self):
        self.defaults = dict(DEFAULTS)
        self.decimals = 4
        self.abort_flag = False
        self.is_legacy = True
        self.inform = mock.MagicMock()
        self.proc_container = mock.MagicMock()


