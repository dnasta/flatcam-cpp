#!/usr/bin/env python3
"""Carga las clases de interfaz ORIGINALES de FlatCAM 2024.4 con PyQt5, sin abrir la aplicación completa.

Lo usan los generadores de `tools/gui_dump/` para construir los widgets reales (ventana principal, paneles de las
herramientas, propiedades de los objetos) y volcar su estructura a C++. Reemplaza por módulos vacíos lo que no hace falta
para construir la interfaz (vispy, editores, OR-Tools, etc.) y deja la traducción al español de `translate/es.bin`.

Entorno:  micromamba install -n old -c conda-forge "pyqt=5.15" matplotlib  (además de lo de los oráculos)
"""
import builtins
import importlib
import logging
import os
import pickle
import sys
import types
import warnings
from unittest import mock

warnings.filterwarnings("ignore")
sys.dont_write_bytecode = True  # no dejar __pycache__ dentro de reference/
os.environ.setdefault("QT_QPA_PLATFORM", "offscreen")

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", ".."))
REF = os.path.join(ROOT, "reference", "FlatCAM_2024_4", "FlatCAM")

from PyQt5 import QtCore, QtGui, QtWidgets  # noqa: E402

# ------------------------------------------------------------------ traducción
with open(os.path.join(REF, "translate", "es.bin"), "rb") as f:
    SPANISH = pickle.load(f)
# Correcciones de la traducción pedidas por el usuario (por ejemplo "Open" -> "Abrir", no "Abierto").
import json  # noqa: E402

with open(os.path.join(os.path.dirname(os.path.abspath(__file__)), "es_corrections.json"), encoding="utf-8") as f:
    SPANISH.update({k: v for k, v in json.load(f).items() if not k.startswith("_")})


def gettext(text):
    """Igual que `appTranslation.gettext` con el idioma español cargado."""
    return SPANISH.get(text, text)


# ------------------------------------------------------------------ íconos: se recuerda la ruta de cada QIcon
ICON_PATHS = {}
_QIcon = QtGui.QIcon


class RecordingIcon(_QIcon):
    """`QIcon` que anota la ruta del recurso con que se creó (por su `cacheKey`, que sobrevive a las copias)."""

    def __init__(self, *args):
        super().__init__(*args)
        if args and isinstance(args[0], str):
            ICON_PATHS[self.cacheKey()] = args[0]

    def addFile(self, path, *args):
        super().addFile(path, *args)
        ICON_PATHS[self.cacheKey()] = path


QtGui.QIcon = RecordingIcon

PIXMAP_PATHS = {}
_QPixmap = QtGui.QPixmap


class RecordingPixmap(_QPixmap):
    """`QPixmap` que anota la ruta del recurso con que se creó (por su `cacheKey`)."""

    def __init__(self, *args):
        super().__init__(*args)
        if args and isinstance(args[0], str):
            PIXMAP_PATHS[self.cacheKey()] = args[0]


QtGui.QPixmap = RecordingPixmap


def pixmap_path(pm):
    """Ruta del recurso de una imagen, o `None` si no tiene."""
    if pm is None or pm.isNull():
        return None
    return PIXMAP_PATHS.get(pm.cacheKey())

# ------------------------------------------------------------------ QSettings aislado
# El original guarda el diseño y otras opciones en el QSettings del sistema ("Open Source", "FlatCAM"). Para no tocar
# la configuración del usuario, todos los QSettings van a un archivo INI temporal que arranca vacío (primera ejecución).
import tempfile  # noqa: E402

_SETTINGS_DIR = tempfile.mkdtemp(prefix="fc_gui_dump_")
_QSettings = QtCore.QSettings


class IsolatedSettings(_QSettings):
    """`QSettings` que siempre usa un INI temporal, sin importar la organización y la aplicación pedidas."""

    def __init__(self, *args, **kwargs):
        super().__init__(os.path.join(_SETTINGS_DIR, "settings.ini"), _QSettings.IniFormat)


QtCore.QSettings = IsolatedSettings


def icon_path(icon):
    """Ruta del recurso de un ícono, o `None` si no tiene."""
    if icon is None or icon.isNull():
        return None
    return ICON_PATHS.get(icon.cacheKey())


# ------------------------------------------------------------------ módulos reemplazados
def _stub(name, **attrs):
    mod = types.ModuleType(name)
    for k, v in attrs.items():
        setattr(mod, k, v)
    sys.modules[name] = mod
    return mod


def _drawing_stub(name):
    """Módulo vacío que exporta (con `import *`) los nombres públicos del módulo original, como simulacros."""
    mod = types.ModuleType(name)
    path = os.path.join(REF, *name.split(".")) + ".py"
    names = []
    if os.path.exists(path):
        import re
        src = open(path, encoding="utf-8").read()
        names = sorted(set(re.findall(r"^(?:class|def)\s+([A-Za-z]\w*)", src, re.M) +
                           re.findall(r"^([A-Za-z]\w*)\s*=", src, re.M)))
    for n in names:
        setattr(mod, n, mock.MagicMock(name=n))
    mod.__all__ = names
    mod.__getattr__ = lambda n: mock.MagicMock(name=n)
    return mod


def install():
    """Prepara el intérprete para importar la interfaz original. Devuelve la `QApplication`."""
    app = QtWidgets.QApplication.instance() or QtWidgets.QApplication(["flatcam"])
    builtins._ = gettext
    _stub("appTranslation", apply_language=lambda *a, **k: gettext, gettext=gettext,
          get_languages_names=lambda: ("English", "Español"), get_current_language_name=lambda: "Spanish",
          on_language_apply_click=lambda *a, **k: None, restart_program=lambda *a, **k: None)
    for name in ["vispy", "vispy.scene", "vispy.scene.visuals", "vispy.visuals", "vispy.geometry", "vispy.color",
                 "vispy.gloo", "vispy.app", "rtree", "ortools", "ortools.constraint_solver", "ezdxf", "svg",
                 "svg.path", "lxml", "lxml.etree", "rasterio", "rasterio.features", "qrcode", "reportlab",
                 "reportlab.graphics", "reportlab.lib", "reportlab.lib.units", "reportlab.lib.pagesizes",
                 "svglib", "svglib.svglib", "pyqtgraph", "descartes", "descartes.patch"]:
        sys.modules.setdefault(name, mock.MagicMock())
    # El dibujo con vispy se reemplaza por el canvas OpenGL del port: no hace falta para construir la interfaz.
    for name in ["appGUI.VisPyVisuals", "appGUI.VisPyCanvas", "appGUI.VisPyPatches", "appGUI.VisPyTesselators",
                 "appGUI.PlotCanvas", "appGUI.PlotCanvasLegacy", "OpenGL", "OpenGL.GLU"]:
        sys.modules[name] = _drawing_stub(name)
    sys.path.insert(0, REF)
    importlib.import_module("resources")  # registra los íconos en ':/' (tema claro, como el usuario)
    return app


def defaults():
    """Diccionario de opciones de fábrica de FlatCAM (`FlatCAMDefaults.factory_defaults`) con el idioma en español."""
    mod = import_original("defaults")
    d = dict(mod.FlatCAMDefaults.factory_defaults)
    d["global_language"] = "Spanish"
    return d


class DefaultsDict(dict):
    """Diccionario de opciones con los métodos de `FlatCAMDefaults` que usa la interfaz."""

    def report_usage(self, *args, **kwargs):
        pass


class FakeApp:
    """Lo mínimo de `App` que necesitan las clases de interfaz para construirse."""

    def __init__(self):
        self.defaults = DefaultsDict(defaults())
        self.options = self.defaults
        self.decimals = 4
        self.fractional_digits = 4
        self.version = "2024.4"  # app_Main.App.version
        self.version_date = "2024/04/15"
        self.app_url = "http://flatcam.org"  # App.app_url (atributo de la clase)
        self.engine = "3D"  # global_graphic_engine
        self.beta = True
        self.is_legacy = False
        self.log = logging.getLogger("fc")
        for name in ["inform", "inform_shell", "proc_container", "worker_task", "collection", "plotcanvas", "ui",
                     "f_handlers", "app_obj", "object_status_changed", "geo_editor", "exc_editor", "grb_editor",
                     "tools_db_tab", "preprocessors", "on_workspace_toggle", "final_save", "app_units"]:
            setattr(self, name, mock.MagicMock())


STUBBED = []


def import_original(modname):
    """Importa un módulo del original reemplazando, uno por uno, los módulos externos que falten (se anotan en STUBBED)."""
    for _ in range(100):
        before = set(sys.modules)
        try:
            return importlib.import_module(modname)
        except ModuleNotFoundError as e:
            missing = e.name
            if missing is None or os.path.exists(os.path.join(REF, *missing.split("."))) or \
                    os.path.exists(os.path.join(REF, *missing.split(".")) + ".py"):
                raise
            for m in set(sys.modules) - before:  # descartar lo importado a medias
                if m not in ("resources", "resources_dark"):  # Qt usa sus bytes sin copiarlos: no liberarlos
                    del sys.modules[m]
            parts = missing.split(".")
            for i in range(1, len(parts) + 1):
                sys.modules.setdefault(".".join(parts[:i]), mock.MagicMock())
            STUBBED.append(missing)
    raise RuntimeError("demasiados módulos faltantes")


def _fake_getattr(self, name):
    """Cualquier otro atributo de la aplicación (señales, manejadores) es un simulacro."""
    if name.startswith("__"):
        raise AttributeError(name)
    m = mock.MagicMock(name=name)
    setattr(self, name, m)
    return m


FakeApp.__getattr__ = _fake_getattr


def build_main_gui():
    """Construye la `MainGUI` original (en español). Devuelve `(qapp, fake_app, main_gui)`."""
    qapp = install()
    fa = FakeApp()
    import_original("appGUI.preferences.OptionsGroupUI").OptionsGroupUI.app = fa  # atributo de clase en el original
    gui = import_original("appGUI.MainGUI").MainGUI(fa)
    return qapp, fa, gui


def build_app():
    """Reproduce el arranque de `App.__init__` que arma la interfaz (primera ejecución, diseño `minimal`).

    Usa los métodos ORIGINALES de `app_Main.App` sobre la aplicación simulada, en el mismo orden que el original:
    `MainGUI`, vista de la colección en la pestaña Proyecto, `setup_default_properties_tab`, `install_tools`,
    `set_ui_title` y `on_layout('minimal')`. Devuelve `(qapp, fake_app, main_gui)`.
    """
    qapp, fa, gui = build_main_gui()
    fa.ui = gui
    app_main = import_original("app_Main")
    App = app_main.App
    app_main.FCShell = mock.MagicMock()  # la consola Tcl está fuera del alcance (y exige una App real)
    fa.collection = import_original("appObjects.ObjectCollection").ObjectCollection(app=fa)
    gui.project_tab_layout.addWidget(fa.collection.view)
    fa.collection.view.setMinimumWidth(290)
    # app_Main: después de on_plotcanvas_setup (el canvas lo agrega el port en C++)
    gui.splitter.setStretchFactor(1, 2)
    gui.splitter.setSizes([1, 1] if fa.defaults["global_project_at_startup"] else [0, 1])
    for name in ("setup_default_properties_tab", "install_tools"):
        getattr(App, name)(fa)
    if not fa.defaults["global_shell_at_startup"]:
        gui.shell_dock.hide()
    gui.set_ui_title(name=gettext("New Project - Not saved"))
    App.on_layout(fa, lay="minimal")
    return qapp, fa, gui
