#!/usr/bin/env python3
"""Vuelca a JSON el árbol de widgets de una interfaz ORIGINAL de FlatCAM ya construida con PyQt5.

El JSON describe todo lo que hace falta para reconstruir la misma interfaz en C++ (lo usa `gen_cpp.py`):

- cada widget con su clase de Python y su clase base de Qt, el nombre del atributo con que el original lo guarda
  (`menufilenewproject`, `toolbarfile`...) y **solo las propiedades Qt que difieren** de una instancia nueva de esa clase
  base (textos, tooltips, íconos por ruta de recurso, tamaños, estilos, políticas de tamaño, fuentes, etc.);
- los layouts con sus márgenes, espaciados, estiramientos y la posición de cada elemento;
- los contenedores especiales (pestañas, divisores, áreas con desplazamiento, pilas, barras de herramientas, menús,
  barra de estado y paneles acoplables);
- las acciones (`QAction`), compartidas entre menús y barras, con su texto, ícono, tooltip y estado.
"""
import json

from PyQt5 import QtCore, QtGui, QtWidgets

import fc_runtime as fr

# Propiedades que dependen del layout, del estado de la ventana o que no se fijan desde el código.
SKIP_PROPS = {
    "geometry", "pos", "size", "x", "y", "width", "height", "rect", "frameGeometry", "normalGeometry", "childrenRect",
    "childrenRegion", "frameSize", "sizeHint", "minimumSizeHint", "visible", "isActiveWindow", "focus", "updatesEnabled",
    "palette", "cursor", "locale", "inputMethodHints", "windowModified", "windowFilePath", "windowIconText",
    "accessibleName", "accessibleDescription", "currentIndex", "count", "currentRow", "currentColumn", "verticalScrollBarPolicy_",
    "sliderPosition", "documentTitle", "html", "markdown", "placeholderText_", "hasSelectedText", "selectedText", "cursorPosition",
    "undoAvailable", "redoAvailable", "modified", "acceptableInput", "displayText", "hasFrame_", "windowOpacity",
    "contextMenuPolicy_", "mouseTracking", "tabletTracking", "focusPolicy_", "autoFillBackground_", "windowIcon",
    "windowTitle_", "fullScreen", "maximized", "minimized", "animated_", "windowModality",
    "iconText",  # Qt lo deriva del texto: el original nunca lo fija
}
LAYOUT_SKIP = {"margin", "spacing", "horizontalSpacing", "verticalSpacing"}


def py_class(obj):
    return type(obj).__name__


def qt_class(obj):
    """Primera clase de Qt (no de Python) en la cadena de herencia del objeto."""
    mo = obj.metaObject()
    while mo is not None:
        name = mo.className()
        cls = getattr(QtWidgets, name, None) or getattr(QtCore, name, None) or getattr(QtGui, name, None)
        if cls is not None and cls.__module__.startswith("PyQt5"):
            return name, cls
        mo = mo.superClass()
    return "QWidget", QtWidgets.QWidget


_DEFAULTS = {}


def default_instance(cls):
    if cls not in _DEFAULTS:
        try:
            _DEFAULTS[cls] = cls()
        except Exception:  # noqa: BLE001  (constructores que piden argumentos o la aplicación)
            _DEFAULTS[cls] = None
    return _DEFAULTS[cls]


def ser(v):
    """Valor de una propiedad en un formato JSON con tipo."""
    if isinstance(v, bool):
        return {"t": "bool", "v": v}
    if isinstance(v, int):
        return {"t": "int", "v": v}
    if isinstance(v, float):
        return {"t": "double", "v": v}
    if isinstance(v, str):
        return {"t": "str", "v": v}
    if isinstance(v, QtCore.QSize):
        return {"t": "QSize", "v": [v.width(), v.height()]}
    if isinstance(v, QtCore.QPoint):
        return {"t": "QPoint", "v": [v.x(), v.y()]}
    if isinstance(v, QtWidgets.QSizePolicy):
        return {"t": "QSizePolicy", "v": [int(v.horizontalPolicy()), int(v.verticalPolicy()), v.horizontalStretch(),
                                          v.verticalStretch()]}
    if isinstance(v, QtGui.QFont):
        # Solo lo que difiere de la fuente por defecto: la familia del sistema donde corre el volcado no sirve en otro.
        base = QtWidgets.QApplication.font()
        d = {}
        if v.family() != base.family():
            d["family"] = v.family()
        if v.bold() != base.bold():
            d["bold"] = v.bold()
        if v.italic() != base.italic():
            d["italic"] = v.italic()
        if v.pixelSize() > 0 and v.pixelSize() != base.pixelSize():
            d["pixelSize"] = v.pixelSize()
        elif v.pointSizeF() > 0 and v.pointSizeF() != base.pointSizeF():
            d["pointSize"] = v.pointSizeF()
        return {"t": "QFont", "v": d} if d else None
    if isinstance(v, fr._QIcon):
        return {"t": "QIcon", "v": fr.icon_path(v)}
    if isinstance(v, fr._QPixmap):
        return {"t": "QPixmap", "v": fr.pixmap_path(v)}
    if isinstance(v, QtGui.QKeySequence):
        return {"t": "QKeySequence", "v": v.toString()}
    if isinstance(v, QtGui.QColor):
        return {"t": "QColor", "v": v.name(QtGui.QColor.HexArgb)}
    if isinstance(v, QtCore.QMargins):
        return {"t": "QMargins", "v": [v.left(), v.top(), v.right(), v.bottom()]}
    if hasattr(v, "__int__"):  # enums y flags de Qt
        return {"t": "enum", "v": int(v), "cls": type(v).__name__}
    return None


def same(a, b):
    if isinstance(a, fr._QPixmap) and isinstance(b, fr._QPixmap):
        return fr.pixmap_path(a) == fr.pixmap_path(b) and a.isNull() == b.isNull()
    if isinstance(a, fr._QIcon) and isinstance(b, fr._QIcon):
        return fr.icon_path(a) == fr.icon_path(b) and a.isNull() == b.isNull()
    if isinstance(a, QtGui.QFont) and isinstance(b, QtGui.QFont):
        return a.toString() == b.toString()
    try:
        return a == b
    except Exception:  # noqa: BLE001
        return False


def props_of(obj):
    """Propiedades Qt escribibles que difieren de una instancia nueva de la misma clase.

    Se compara contra la clase de Python del objeto (por ejemplo `FCDetachableTab`, cuyo constructor ya cambia valores
    de Qt) si se puede crear sin argumentos; si no, contra su clase base de Qt.
    """
    _, cls = qt_class(obj)
    base = default_instance(type(obj)) if not type(obj).__module__.startswith("PyQt5") else None
    if base is None:
        base = default_instance(cls)
    out = {}
    mo = obj.metaObject()
    for i in range(mo.propertyCount()):
        p = mo.property(i)
        name = p.name()
        if name in SKIP_PROPS or not p.isWritable() or not p.isDesignable():
            continue
        # En un widget, `enabled` se lee efectivo (falso si el padre está deshabilitado): se guarda aparte, solo si el
        # widget mismo está deshabilitado (ver abajo). En una acción o un menú sí es la propiedad propia.
        if name == "enabled" and isinstance(obj, QtWidgets.QWidget):
            continue
        try:
            v = p.read(obj)
        except Exception:  # noqa: BLE001
            continue
        if base is not None:
            try:
                bv = p.read(base)
            except Exception:  # noqa: BLE001
                bv = None
            if same(v, bv):
                continue
        s = ser(v)
        if s is not None:
            if s["t"] in ("QIcon", "QPixmap") and s["v"] is None:
                continue
            out[name] = s
    if isinstance(obj, QtWidgets.QWidget):
        if obj.isHidden() and obj.parentWidget() is not None and not obj.isWindow():
            out["hidden"] = {"t": "bool", "v": True}
        if not obj.isEnabled() and (obj.parentWidget() is None or obj.parentWidget().isEnabled()):
            out["enabled"] = {"t": "bool", "v": False}
    return out


class Dumper:
    """Recorre el árbol y arma el JSON. `names` asocia cada objeto con el atributo del original que lo guarda."""

    def __init__(self, names):
        self.names = names
        self.actions = {}
        self.seen_widgets = set()
        self._keep = []  # envoltorios temporales vivos: si se liberan, Python reutiliza su id()

    def name(self, obj):
        return self.names.get(id(obj))

    # ---------------------------------------------------------------- acciones
    def action(self, a):
        # La clave es la dirección del QAction de Qt, no id(a): PyQt devuelve envoltorios temporales (menuAction(),
        # separadores) cuyo id() se reutiliza cuando se liberan, y así un submenú se confundía con otro ya visto.
        from PyQt5 import sip
        self._keep.append(a)
        key = str(sip.unwrapinstance(a))
        if key in self.actions:
            return key
        entry = {"name": self.name(a), "props": props_of(a)}
        if a.isSeparator():
            entry["separator"] = True
        if a.menu() is not None:
            entry["menu"] = self.menu(a.menu())
        self.actions[key] = entry
        return key

    def menu(self, m):
        return {"name": self.name(m), "props": props_of(m), "py": py_class(m),
                "items": [self.action(a) for a in m.actions()]}

    # ---------------------------------------------------------------- layouts
    def layout(self, lay):
        qt, _ = qt_class(lay)
        node = {"qt": qt, "py": py_class(lay), "name": self.name(lay),
                "props": {k: v for k, v in props_of(lay).items() if k not in LAYOUT_SKIP}}
        m = lay.contentsMargins()
        node["margins"] = [m.left(), m.top(), m.right(), m.bottom()]
        node["spacing"] = lay.spacing()
        if isinstance(lay, QtWidgets.QFormLayout):
            node["hspacing"] = lay.horizontalSpacing()
            node["vspacing"] = lay.verticalSpacing()
        if isinstance(lay, QtWidgets.QGridLayout):
            node["hspacing"] = lay.horizontalSpacing()
            node["vspacing"] = lay.verticalSpacing()
            node["rowStretch"] = [lay.rowStretch(r) for r in range(lay.rowCount())]
            node["colStretch"] = [lay.columnStretch(c) for c in range(lay.columnCount())]
            node["colMinWidth"] = [lay.columnMinimumWidth(c) for c in range(lay.columnCount())]
        items = []
        for i in range(lay.count()):
            it = lay.itemAt(i)
            entry = {"align": int(it.alignment())}
            if isinstance(lay, QtWidgets.QGridLayout):
                r, c, rs, cs = lay.getItemPosition(i)
                entry.update(row=r, col=c, rs=rs, cs=cs)
            elif isinstance(lay, QtWidgets.QFormLayout):
                r, role = lay.getItemPosition(i)
                entry.update(row=r, role=int(role))
            elif isinstance(lay, QtWidgets.QBoxLayout):
                entry["stretch"] = lay.stretch(i)
            if it.widget() is not None:
                entry["widget"] = self.widget(it.widget())
            elif it.layout() is not None:
                entry["layout"] = self.layout(it.layout())
            elif it.spacerItem() is not None:
                sp = it.spacerItem()
                pol = sp.sizePolicy()
                entry["spacer"] = [sp.sizeHint().width(), sp.sizeHint().height(), int(pol.horizontalPolicy()),
                                   int(pol.verticalPolicy())]
            else:
                continue
            items.append(entry)
        node["items"] = items
        if isinstance(lay, QtWidgets.QFormLayout):
            node["formRows"] = True
        return node

    # ---------------------------------------------------------------- widgets
    def widget(self, w):
        self.seen_widgets.add(id(w))
        qt, _ = qt_class(w)
        node = {"qt": qt, "py": py_class(w), "name": self.name(w), "props": props_of(w)}
        if isinstance(w, QtWidgets.QTabWidget):
            tb = QtWidgets.QTabWidget.tabBar(w)  # FCDetachableTab pisa `tabBar` con un atributo
            node["tabbar"] = {"py": py_class(tb), "props": props_of(tb)}
            node["tabs"] = [{"text": w.tabText(i), "icon": fr.icon_path(w.tabIcon(i)), "tooltip": w.tabToolTip(i),
                             "protected": w.tabsClosable() and tb.tabButton(i, QtWidgets.QTabBar.RightSide) is None,
                             "widget": self.widget(w.widget(i))} for i in range(w.count())]
            return node
        if isinstance(w, QtWidgets.QSplitter):
            node["widgets"] = [self.widget(w.widget(i)) for i in range(w.count())]
            node["sizes"] = w.sizes()
            return node
        if isinstance(w, QtWidgets.QScrollArea):
            if w.widget() is not None:
                node["widget"] = self.widget(w.widget())
            return node
        if isinstance(w, QtWidgets.QStackedWidget):
            node["pages"] = [self.widget(w.widget(i)) for i in range(w.count())]
            return node
        if isinstance(w, QtWidgets.QToolBar):
            items = []
            for a in w.actions():
                ww = w.widgetForAction(a)
                if isinstance(a, QtWidgets.QWidgetAction) and ww is not None and not isinstance(ww, QtWidgets.QToolButton):
                    node_w = self.widget(ww)
                    # QToolBar oculta sus widgets hasta que se muestra la ventana: la visibilidad real es la de la acción.
                    node_w["props"].pop("hidden", None)
                    items.append({"widget": node_w, "action": self.name(a), "visible": a.isVisible()})
                else:
                    items.append({"action": self.action(a)})
            node["items"] = items
            return node
        if isinstance(w, QtWidgets.QMenuBar):
            node["items"] = [self.action(a) for a in w.actions()]
            return node
        if isinstance(w, QtWidgets.QDockWidget):
            if w.widget() is not None:
                node["widget"] = self.widget(w.widget())
            return node
        if py_class(w) == "RadioSet":
            # Las opciones no son propiedades de Qt: se leen del objeto del original.
            node["choices"] = [{"label": c["label"], "value": c["value"]} for c in w.choices]
            node["vertical"] = isinstance(QtWidgets.QWidget.layout(w), QtWidgets.QVBoxLayout)
            lay = QtWidgets.QWidget.layout(w)
            node["stretch"] = lay.count() > 0 and lay.itemAt(lay.count() - 1).spacerItem() is not None
            node["value"] = w.get_value()
            return node
        if isinstance(w, QtWidgets.QTableWidget):
            hh, vh = w.horizontalHeader(), w.verticalHeader()
            node["table"] = {
                "cols": w.columnCount(),
                "labels": [w.horizontalHeaderItem(i).text() if w.horizontalHeaderItem(i) else "" for i in range(w.columnCount())],
                "hheader": not hh.isHidden(), "vheader": not vh.isHidden(),
                "resize": [int(hh.sectionResizeMode(i)) for i in range(w.columnCount())],
                "stretchLast": hh.stretchLastSection(),
                "hiddenCols": [i for i in range(w.columnCount()) if w.isColumnHidden(i)],
            }
            return node
        if isinstance(w, (QtWidgets.QAbstractItemView, QtWidgets.QComboBox, QtWidgets.QAbstractSpinBox,
                          QtWidgets.QLineEdit, QtWidgets.QAbstractButton, QtWidgets.QLabel)):
            if isinstance(w, QtWidgets.QComboBox):
                node["items"] = [w.itemText(i) for i in range(w.count())]
                node["currentIndex"] = w.currentIndex()
            if isinstance(w, QtWidgets.QPushButton) and QtWidgets.QPushButton.menu(w) is not None:
                node["menu"] = self.menu(QtWidgets.QPushButton.menu(w))
            if QtWidgets.QWidget.layout(w) is None:
                return node
        if QtWidgets.QWidget.layout(w) is not None:
            node["layout"] = self.layout(QtWidgets.QWidget.layout(w))
        # Widgets hijos que no están en el layout (los agrega el código con `setParent`, o son internos)
        return node

    # ---------------------------------------------------------------- ventana principal
    def main_window(self, mw):
        node = {"qt": "QMainWindow", "py": py_class(mw), "name": None, "props": props_of(mw)}
        node["menubar"] = self.widget(mw.menuBar())
        tbs = []
        for t in mw.findChildren(QtWidgets.QToolBar, options=QtCore.Qt.FindDirectChildrenOnly):
            area = int(mw.toolBarArea(t))
            if area == 0 or t.parent() is not mw:
                continue  # sacada por on_layout o puesta en la barra de estado
            tbs.append({"area": area, "break": bool(mw.toolBarBreak(t)), "node": self.widget(t), "y": t.y(),
                        "x": t.x()})
        tbs.sort(key=lambda e: (e["area"], e["y"], e["x"]))
        node["toolbars"] = tbs
        node["central"] = self.widget(mw.centralWidget())
        sb = mw.statusBar()
        node["statusbar"] = {"props": props_of(sb), "items": [self.widget(c) for c in sb.children()
                                                              if isinstance(c, QtWidgets.QWidget)
                                                              and not isinstance(c, QtWidgets.QSizeGrip)]}
        docks = []
        for d in mw.findChildren(QtWidgets.QDockWidget, options=QtCore.Qt.FindDirectChildrenOnly):
            docks.append({"area": int(mw.dockWidgetArea(d)), "node": self.widget(d)})
        node["docks"] = docks
        return node


def collect_names(root_obj, prefix=""):
    """Nombres de atributos de un objeto de Python que apuntan a objetos Qt (`self.menufile` -> 'menufile')."""
    names = {}
    for k, v in vars(root_obj).items():
        if isinstance(v, (QtCore.QObject,)) and id(v) not in names:
            names[id(v)] = prefix + k
    return names


def dump_widget(root, *name_sources):
    """Vuelca un panel (por ejemplo `GerberObjectUI` o el de una herramienta) como raíz de una clase propia.

    `name_sources` son los objetos de Python cuyos atributos dan nombre a los widgets (el primero tiene prioridad).
    """
    names = {}
    for src in reversed(name_sources):
        names.update(collect_names(src))
    names.pop(id(root), None)
    d = Dumper(names)
    return {"root": d.widget(root), "actions": d.actions}


def dump_main_window(gui, app=None):
    names = collect_names(gui)
    if app is not None:
        # La vista del árbol del proyecto es de la colección (`app.collection.view`), no de la ventana.
        coll = getattr(app, "collection", None)
        if coll is not None and isinstance(getattr(coll, "view", None), QtCore.QObject):
            names[id(coll.view)] = "collection_view"
        # Las acciones del menú Herramienta las crea cada herramienta (`appTool.install`): `ncc_tool.menuAction` -> ncc_tool_menuAction
        for k, v in list(vars(app).items()):
            act = getattr(v, "menuAction", None) if not isinstance(v, QtCore.QObject) or hasattr(v, "toolName") else None
            if isinstance(act, QtWidgets.QAction) and id(act) not in names:
                names[id(act)] = k + "_menuAction"
    d = Dumper(names)
    tree = d.main_window(gui)
    return {"window": tree, "actions": d.actions}


if __name__ == "__main__":
    import os
    import sys
    qapp, fa, gui = fr.build_app()
    out = sys.argv[1] if len(sys.argv) > 1 else os.path.join(fr.ROOT, "tools", "gui_dump", "main_gui.json")
    with open(out, "w", encoding="utf-8") as f:
        json.dump(dump_main_window(gui, fa), f, ensure_ascii=False, indent=1)
    print("Escrito", out)
