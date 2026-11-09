#!/usr/bin/env python3
"""Genera código C++ (Qt6) a partir del JSON de `dump_tree.py`: la misma interfaz que el FlatCAM original.

Para la ventana principal escribe `src/gui/generated/MainGUIBase.h` y `.cpp`: una clase `MainGUIBase` (un `QMainWindow`)
cuyo `setupUi()` arma menús, acciones, barras de herramientas, pestañas, barra de estado y paneles acoplables con los
mismos textos (en español), íconos, tooltips, tamaños y estilos que el original. Los objetos que el original guarda en
atributos (`self.menufilenewproject`, `self.toolbartools`...) quedan como miembros públicos con el mismo nombre, para
que la lógica portada a mano (`MainGUI`) los use igual que en Python.

Uso:  python3 tools/gui_dump/gen_cpp.py tools/gui_dump/main_gui.json
"""
import json
import os
import sys

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", ".."))
OUT_DIR = os.path.join(ROOT, "src", "gui", "generated")

# Clases propias de FlatCAM con port en C++ (src/gui/widgets/). Su constructor arma lo de adentro, así que el
# generador no repite su layout interno. Valor: (encabezado, expresión de construcción con {parent}).
CUSTOM = {
    "FCLabel": ("gui/widgets/FCLabel.h", "new FCLabel({parent})"),
    "FCEntry": ("gui/widgets/FCEntry.h", "new FCEntry({parent})"),
    "FCEntry2": ("gui/widgets/FCEntry.h", "new FCEntry2({parent})"),
    "FCCheckBox": ("gui/widgets/FCCheckBox.h", "new FCCheckBox({parent})"),
    "VerticalScrollArea": ("gui/widgets/VerticalScrollArea.h", "new VerticalScrollArea({parent})"),
    "FCDock": ("gui/widgets/FCDock.h", "new FCDock({parent})"),
    "FCTree": ("gui/widgets/FCTree.h", "new FCTree({parent})"),
    "KeySensitiveListView": ("gui/widgets/KeySensitiveListView.h", "new KeySensitiveListView({parent})"),
    "FCDetachableTab": ("gui/widgets/FCDetachableTab.h", "new FCDetachableTab({parent})"),
    "FCDetachableTab2": ("gui/widgets/FCDetachableTab.h", "new FCDetachableTab2({parent})"),
    "FlatCAMInfoBar": ("gui/widgets/FlatCAMInfoBar.h", "new FlatCAMInfoBar({parent})"),
    "FlatCAMActivityView": ("gui/widgets/FlatCAMActivityView.h", "new FlatCAMActivityView({parent})"),
    "FCButton": ("gui/widgets/FCButton.h", "new FCButton({parent})"),
    "FCSpinner": ("gui/widgets/FCSpinners.h", "new FCSpinner({parent})"),
    "FCDoubleSpinner": ("gui/widgets/FCSpinners.h", "new FCDoubleSpinner({parent})"),
    "FCComboBox": ("gui/widgets/FCComboBox.h", "new FCComboBox({parent})"),
    "FCComboBox2": ("gui/widgets/FCComboBox.h", "new FCComboBox2({parent})"),
    "FCTable": ("gui/widgets/FCTable.h", "new FCTable({parent})"),
    "NumericalEvalEntry": ("gui/widgets/NumericalEval.h", "new NumericalEvalEntry({parent})"),
    "NumericalEvalTupleEntry": ("gui/widgets/NumericalEval.h", "new NumericalEvalTupleEntry({parent})"),
    "RadioSet": ("gui/widgets/RadioSet.h", None),
    "FCColorEntry": ("gui/widgets/FCPrefWidgets.h", "new FCColorEntry({parent})"),
    "FCSliderWithSpinner": ("gui/widgets/FCPrefWidgets.h", None),  # se construye con su rango (ver Gen.construct)
    "FCTextArea": ("gui/widgets/FCPrefWidgets.h", "new FCTextArea({parent})"),
    "FCTab": ("gui/widgets/FCPrefWidgets.h", "new FCTab({parent})"),  # se construye con sus opciones (ver Gen.construct)
}
# Clases cuyo contenido interno arma el propio constructor del port (no se generan sus hijos).
SELF_BUILT = {"FCColorEntry", "FCSliderWithSpinner", "FlatCAMInfoBar", "FlatCAMActivityView", "FCDetachableTab", "FCDetachableTab2", "FCTree",
              "KeySensitiveListView", "RadioSet"}

QT_HEADERS = ["QDialog", "QAction", "QPixmap", "QFormLayout", "QHeaderView", "QTableWidget", "QDoubleSpinBox", "QSpinBox", "QRadioButton",
              "QTextEdit", "QPlainTextEdit", "QSlider", "QBoxLayout", "QCheckBox", "QComboBox", "QDockWidget", "QFont", "QFrame", "QGridLayout", "QGroupBox",
              "QHBoxLayout", "QIcon", "QLabel", "QLineEdit", "QMainWindow", "QMenu", "QMenuBar", "QPushButton",
              "QScrollArea", "QSizePolicy", "QSplitter", "QStackedWidget", "QStatusBar", "QTabBar", "QTabWidget",
              "QToolBar", "QToolButton", "QTreeView", "QTreeWidget", "QVBoxLayout", "QWidget"]


def cstr(s):
    """Literal de C++ con el texto en UTF-8 (el proyecto compila con /utf-8 en MSVC)."""
    out = []
    for ch in s:
        if ch == "\\":
            out.append("\\\\")
        elif ch == '"':
            out.append('\\"')
        elif ch == "\n":
            out.append("\\n")
        elif ch == "\t":
            out.append("\\t")
        elif ch == "\r":
            out.append("\\r")
        elif ord(ch) < 32:
            out.append("\\x%02x" % ord(ch))
        else:
            out.append(ch)
    return 'QString::fromUtf8("%s")' % "".join(out).replace("??", '?""?')  # evita trigrafos


class Gen:
    def __init__(self, data):
        self.data = data
        self.actions = data["actions"]
        self.lines = []
        self.members = {}      # nombre -> tipo C++
        self.includes = set()
        self.counter = 0
        self.action_var = {}   # id -> expresión C++

    # ------------------------------------------------------------------ utilidades
    def emit(self, s=""):
        self.lines.append("    " + s if s else "")

    def var(self, node_name, cpp_type, prefix):
        """Nombre de la variable: el miembro con el nombre del original, o una local numerada."""
        if node_name:
            if node_name in self.members and self.members[node_name] != cpp_type:
                node_name = node_name + "_" + str(self.counter)
            self.members[node_name] = cpp_type
            return node_name, node_name
        self.counter += 1
        v = "%s%d" % (prefix, self.counter)
        return v, "auto* " + v

    def cpp_type(self, node):
        py = node.get("py")
        if py in CUSTOM:
            self.includes.add(CUSTOM[py][0])
            return py
        return node["qt"]

    def construct(self, node, parent="nullptr"):
        py = node.get("py")
        if py == "RadioSet":
            self.includes.add(CUSTOM[py][0])
            def val(x):
                if isinstance(x, bool):
                    return "QVariant(%s)" % ("true" if x else "false")
                if isinstance(x, int):
                    return "QVariant(%d)" % x
                if isinstance(x, float):
                    return "QVariant(%r)" % x
                return "QVariant(%s)" % cstr(str(x))
            choices = ", ".join("{%s, %s}" % (cstr(c["label"]), val(c["value"])) for c in node["choices"])
            return "new RadioSet({%s}, %s, %s, %s)" % (choices, "Qt::Vertical" if node.get("vertical") else "Qt::Horizontal",
                                                      "true" if node.get("stretch") else "false", parent)
        if py == "FCSliderWithSpinner":
            self.includes.add(CUSTOM[py][0])
            sp = self.inner(node, 1)
            mn = sp.get("minimum", {}).get("v", 0)
            mx = sp.get("maximum", {}).get("v", 99)
            st = sp.get("singleStep", {}).get("v", 1)
            return "new FCSliderWithSpinner(%d, %d, %d, %s)" % (mn, mx, st, parent)
        if py in CUSTOM:
            self.includes.add(CUSTOM[py][0])
            return CUSTOM[py][1].format(parent=parent)
        return "new %s(%s)" % (node["qt"], parent)

    @staticmethod
    def inner(node, i):
        """Propiedades del hijo i de un widget compuesto (el campo y el botón de FCColorEntry, el deslizador y el número
        de FCSliderWithSpinner)."""
        items = node.get("layout", {}).get("items", [])
        return items[i]["widget"].get("props", {}) if i < len(items) and "widget" in items[i] else {}

    # ------------------------------------------------------------------ propiedades
    def props(self, v, node, kind="widget"):
        p = node.get("props", {})
        order = sorted(p.items(), key=lambda kv: (kv[0] == "hidden", kv[0]))
        for name, val in order:
            t, x = val["t"], val["v"]
            if name == "text":
                self.emit("%s->setText(%s);" % (v, cstr(x)))
            elif name == "toolTip":
                self.emit("%s->setToolTip(%s);" % (v, cstr(x)))
            elif name == "statusTip":
                self.emit("%s->setStatusTip(%s);" % (v, cstr(x)))
            elif name == "whatsThis":
                self.emit("%s->setWhatsThis(%s);" % (v, cstr(x)))
            elif name == "title":
                self.emit("%s->setTitle(%s);" % (v, cstr(x)))
            elif name == "windowTitle":
                self.emit("%s->setWindowTitle(%s);" % (v, cstr(x)))
            elif name == "styleSheet":
                self.emit("%s->setStyleSheet(%s);" % (v, cstr(x)))
            elif name == "pixmap":
                self.emit('%s->setPixmap(QPixmap(%s));' % (v, cstr(x)))
            elif name == "icon":
                self.emit('%s->setIcon(QIcon(%s));' % (v, cstr(x)))
            elif name == "hidden":
                if kind == "action":
                    self.emit("%s->setVisible(false);" % v)
                else:
                    self.emit("%s->setHidden(true);" % v)
            elif name == "enabled":
                self.emit("%s->setEnabled(%s);" % (v, "true" if x else "false"))
            elif name == "font":
                parts = ["QFont f = %s->font();" % v]
                if "family" in x:
                    parts.append("f.setFamily(%s);" % cstr(x["family"]))
                if "bold" in x:
                    parts.append("f.setBold(%s);" % ("true" if x["bold"] else "false"))
                if "italic" in x:
                    parts.append("f.setItalic(%s);" % ("true" if x["italic"] else "false"))
                if "pixelSize" in x:
                    parts.append("f.setPixelSize(%d);" % x["pixelSize"])
                if "pointSize" in x:
                    parts.append("f.setPointSizeF(%r);" % x["pointSize"])
                self.emit("{ %s %s->setFont(f); }" % (" ".join(parts), v))
            elif t == "QSize":
                setter = "set" + name[0].upper() + name[1:]
                self.emit("%s->%s(QSize(%d, %d));" % (v, setter, x[0], x[1]))
            elif t == "QSizePolicy":
                self.emit("{ QSizePolicy sp(QSizePolicy::Policy(%d), QSizePolicy::Policy(%d)); sp.setHorizontalStretch(%d); "
                          "sp.setVerticalStretch(%d); %s->setSizePolicy(sp); }" % (x[0], x[1], x[2], x[3], v))
            elif t == "bool":
                setter = "set" + name[0].upper() + name[1:]
                self.emit("%s->%s(%s);" % (v, setter, "true" if x else "false"))
            elif t in ("int", "enum", "double", "str"):
                # Enumeraciones y demás: por el sistema de propiedades de Qt (convierte el entero al enum).
                lit = cstr(x) if t == "str" else repr(x)
                self.emit('%s->setProperty("%s", %s);' % (v, name, lit))
            elif t == "QKeySequence":
                self.emit("%s->setShortcut(QKeySequence(%s));" % (v, cstr(x)))
            else:
                self.emit("// propiedad %s (%s) sin traducir" % (name, t))

    # ------------------------------------------------------------------ acciones y menús
    def action_expr(self, aid):
        return self.action_var[aid]

    def define_actions(self):
        self.emit("// ---- acciones (compartidas entre menús y barras)")
        for aid, a in self.actions.items():
            if a.get("separator") or "menu" in a:
                continue
            v, decl = self.var(a.get("name"), "QAction", "act")
            self.emit("%s = new QAction(this);" % decl)
            self.props(v, a, kind="action")
            self.action_var[aid] = v

    def build_menu(self, entry, parent_expr):
        """Crea el menú de una acción con submenú y devuelve la variable."""
        m = entry["menu"]
        v, decl = self.var(m.get("name") or entry.get("name"), "QMenu", "menu")
        self.emit("%s = new QMenu(%s);" % (decl, parent_expr))
        self.props(v, m)
        # La acción del menú (ícono, texto y tooltip con que aparece en el menú padre)
        self.props(v + "->menuAction()", entry, kind="action")
        self.fill_menu(v, m)
        return v

    def fill_menu(self, v, m):
        for aid in m["items"]:
            a = self.actions[aid]
            if a.get("separator"):
                if "hidden" in a.get("props", {}):
                    self.emit("%s->addSeparator()->setVisible(false);" % v)
                else:
                    self.emit("%s->addSeparator();" % v)
            elif "menu" in a:
                if aid not in self.action_var:
                    sub = self.build_menu(a, v)
                    self.action_var[aid] = sub + "->menuAction()"
                self.emit("%s->addAction(%s);" % (v, self.action_var[aid]))
            else:
                self.emit("%s->addAction(%s);" % (v, self.action_expr(aid)))

    def fresh(self, prefix):
        self.counter += 1
        return "%s%d" % (prefix, self.counter)

    # ------------------------------------------------------------------ widgets y layouts
    def widget(self, node, parent="nullptr"):
        """Crea un widget (y su contenido) y devuelve la variable."""
        ctype = self.cpp_type(node)
        v, decl = self.var(node.get("name"), ctype, "w")
        self.emit("%s = %s;" % (decl, self.construct(node, parent)))
        py = node.get("py")
        qt = node["qt"]
        if py in SELF_BUILT and "tabs" not in node:
            self.props(v, node)
            if py == "FCColorEntry" and "text" in self.inner(node, 0):
                self.emit("%s->setValue(%s);" % (v, cstr(self.inner(node, 0)["text"]["v"])))
            if py == "FCSliderWithSpinner" and "value" in self.inner(node, 1):
                self.emit("%s->setValue(%d);" % (v, self.inner(node, 1)["value"]["v"]))
            if py == "RadioSet" and node.get("value") is not None:
                x = node["value"]
                lit = ("true" if x else "false") if isinstance(x, bool) else (repr(x) if isinstance(x, (int, float)) else cstr(str(x)))
                self.emit("%s->setValue(QVariant(%s));" % (v, lit))
            return v
        if "table" in node:
            self.props(v, node)
            t = node["table"]
            self.emit("%s->setColumnCount(%d);" % (v, t["cols"]))
            if any(t["labels"]):
                self.emit("%s->setHorizontalHeaderLabels({%s});" % (v, ", ".join(cstr(x) for x in t["labels"])))
            if not t["hheader"]:
                self.emit("%s->horizontalHeader()->setVisible(false);" % v)
            if not t["vheader"]:
                self.emit("%s->verticalHeader()->setVisible(false);" % v)
            for i, m in enumerate(t["resize"]):
                if m:
                    self.emit("%s->horizontalHeader()->setSectionResizeMode(%d, QHeaderView::ResizeMode(%d));" % (v, i, m))
            for c in t.get("hiddenCols", []):
                self.emit("%s->setColumnHidden(%d, true);" % (v, c))
            if t["stretchLast"]:
                self.emit("%s->horizontalHeader()->setStretchLastSection(true);" % v)
            return v
        self.props(v, node)
        if "tabs" in node:
            tb = node.get("tabbar", {})
            if tb.get("props"):
                self.props(v + "->tabBar()", tb)
            for t in node["tabs"]:
                child = self.widget(t["widget"])
                icon = "QIcon(%s), " % cstr(t["icon"]) if t.get("icon") else ""
                self.emit("%s->addTab(%s, %s%s);" % (v, child, icon, cstr(t["text"])))
                if t.get("protected"):
                    self.emit("%s->protectTab(%s->count() - 1);" % (v, v))
                if t.get("tooltip"):
                    self.emit("%s->setTabToolTip(%s->count() - 1, %s);" % (v, v, cstr(t["tooltip"])))
            return v
        if qt == "QSplitter":
            for c in node["widgets"]:
                self.emit("%s->addWidget(%s);" % (v, self.widget(c)))
            return v
        if qt == "QScrollArea" and "widget" in node:
            self.emit("%s->setWidget(%s);" % (v, self.widget(node["widget"])))
            return v
        if qt == "QStackedWidget":
            for c in node.get("pages", []):
                self.emit("%s->addWidget(%s);" % (v, self.widget(c)))
            return v
        if qt == "QDockWidget" and "widget" in node:
            self.emit("%s->setWidget(%s);" % (v, self.widget(node["widget"])))
            return v
        if qt == "QToolBar":
            self.fill_toolbar(v, node)
            return v
        if "menu" in node:
            mv = self.fresh("pbmenu")
            self.emit("auto* %s = new QMenu(%s);" % (mv, v))
            self.fill_menu(mv, node["menu"])
            self.emit("%s->setMenu(%s);" % (v, mv))
        if "items" in node and qt == "QComboBox":
            for it in node["items"]:
                self.emit("%s->addItem(%s);" % (v, cstr(it)))
            if node.get("currentIndex", 0) > 0:
                self.emit("%s->setCurrentIndex(%d);" % (v, node["currentIndex"]))
        if "layout" in node:
            lv = self.layout(node["layout"])
            self.emit("%s->setLayout(%s);" % (v, lv))
        return v

    def layout(self, node):
        qt = node["qt"] if node["qt"] in ("QVBoxLayout", "QHBoxLayout", "QGridLayout", "QFormLayout") else "QVBoxLayout"
        if node["qt"] == "QBoxLayout":
            qt = "QVBoxLayout"
        v, decl = self.var(node.get("name"), qt, "lay")
        self.emit("%s = new %s();" % (decl, qt))
        m = node["margins"]
        self.emit("%s->setContentsMargins(%d, %d, %d, %d);" % (v, m[0], m[1], m[2], m[3]))
        if qt in ("QGridLayout", "QFormLayout") and "hspacing" in node:
            self.emit("%s->setHorizontalSpacing(%d);" % (v, node["hspacing"]))
            self.emit("%s->setVerticalSpacing(%d);" % (v, node["vspacing"]))
        else:
            self.emit("%s->setSpacing(%d);" % (v, node["spacing"]))
        self.props(v, node, kind="layout")
        for it in node["items"]:
            align = ", Qt::Alignment(%d)" % it["align"] if it.get("align") else ""
            if "widget" in it and qt == "QFormLayout":
                c = self.widget(it["widget"])
                self.emit("%s->setWidget(%d, QFormLayout::ItemRole(%d), %s);" % (v, it["row"], it["role"], c))
            elif "layout" in it and qt == "QFormLayout":
                c = self.layout(it["layout"])
                self.emit("%s->setLayout(%d, QFormLayout::ItemRole(%d), %s);" % (v, it["row"], it["role"], c))
            elif "widget" in it:
                c = self.widget(it["widget"])
                if qt == "QGridLayout":
                    self.emit("%s->addWidget(%s, %d, %d, %d, %d%s);" % (v, c, it["row"], it["col"], it["rs"], it["cs"], align))
                else:
                    self.emit("%s->addWidget(%s, %d%s);" % (v, c, it.get("stretch", 0), align))
            elif "layout" in it:
                c = self.layout(it["layout"])
                if qt == "QGridLayout":
                    self.emit("%s->addLayout(%s, %d, %d, %d, %d%s);" % (v, c, it["row"], it["col"], it["rs"], it["cs"], align))
                else:
                    self.emit("%s->addLayout(%s, %d);" % (v, c, it.get("stretch", 0)))
            elif "spacer" in it:
                w, h, hp, vp = it["spacer"]
                sp = "new QSpacerItem(%d, %d, QSizePolicy::Policy(%d), QSizePolicy::Policy(%d))" % (w, h, hp, vp)
                if qt == "QGridLayout":
                    self.emit("%s->addItem(%s, %d, %d, %d, %d);" % (v, sp, it["row"], it["col"], it["rs"], it["cs"]))
                else:
                    self.emit("%s->addItem(%s);" % (v, sp))
                    if it.get("stretch"):
                        self.emit("%s->setStretch(%s->count() - 1, %d);" % (v, v, it["stretch"]))
        if qt == "QGridLayout":
            for r, s in enumerate(node.get("rowStretch", [])):
                if s:
                    self.emit("%s->setRowStretch(%d, %d);" % (v, r, s))
            for c, s in enumerate(node.get("colStretch", [])):
                if s:
                    self.emit("%s->setColumnStretch(%d, %d);" % (v, c, s))
            for c, s in enumerate(node.get("colMinWidth", [])):
                if s:
                    self.emit("%s->setColumnMinimumWidth(%d, %d);" % (v, c, s))
        return v

    def fill_toolbar(self, v, node):
        for it in node["items"]:
            if "widget" in it:
                c = self.widget(it["widget"])
                act = it.get("action")
                if act:
                    self.members[act] = "QAction"
                    self.emit("%s = %s->addWidget(%s);" % (act, v, c))
                    if not it.get("visible", True):
                        self.emit("%s->setVisible(false);" % act)
                elif not it.get("visible", True):
                    self.emit("%s->addWidget(%s)->setVisible(false);" % (v, c))
                else:
                    self.emit("%s->addWidget(%s);" % (v, c))
                continue
            aid = it["action"]
            a = self.actions[aid]
            if a.get("separator"):
                if "hidden" in a.get("props", {}):
                    self.emit("%s->addSeparator()->setVisible(false);" % v)
                else:
                    self.emit("%s->addSeparator();" % v)
            elif "menu" in a:
                if aid not in self.action_var:
                    sub = self.build_menu(a, v)
                    self.action_var[aid] = sub + "->menuAction()"
                self.emit("%s->addAction(%s);" % (v, self.action_var[aid]))
            else:
                self.emit("%s->addAction(%s);" % (v, self.action_expr(aid)))

    # ------------------------------------------------------------------ ventana principal
    def main_window(self):
        w = self.data["window"]
        self.define_actions()
        self.emit()
        self.emit("// ---- menú principal")
        mb = w["menubar"]
        self.props("menuBar()", mb)
        for aid in mb["items"]:
            a = self.actions[aid]
            if "menu" in a:
                sub = self.build_menu(a, "this")
                self.action_var[aid] = sub + "->menuAction()"
                self.emit("menuBar()->addMenu(%s);" % sub)
            elif not a.get("separator"):
                self.emit("menuBar()->addAction(%s);" % self.action_expr(aid))
        self.emit()
        self.emit("// ---- barras de herramientas (en el orden y las filas de la ventana original)")
        for t in w["toolbars"]:
            if t["break"]:
                self.emit("addToolBarBreak(Qt::ToolBarArea(%d));" % t["area"])
            tb = self.widget(t["node"], "this")
            self.emit("addToolBar(Qt::ToolBarArea(%d), %s);" % (t["area"], tb))
        self.emit()
        self.emit("// ---- área central")
        self.emit("setCentralWidget(%s);" % self.widget(w["central"]))
        self.emit()
        self.emit("// ---- barra de estado (MainGUI.py: todos con addWidget; el de mensajes con estiramiento 1)")
        for it in w["statusbar"]["items"]:
            c = self.widget(it)
            stretch = 1 if it.get("name") == "fcinfo" else 0
            self.emit("statusBar()->addWidget(%s, %d);" % (c, stretch))
        self.emit()
        self.emit("// ---- paneles acoplables")
        for d in w["docks"]:
            hidden = d["node"]["props"].pop("hidden", None)
            dv = self.widget(d["node"], "this")
            self.emit("addDockWidget(Qt::DockWidgetArea(%d), %s);" % (d["area"], dv))
            if hidden:
                self.emit("%s->hide();" % dv)
        self.emit()
        self.props("this", w)

    def panel(self):
        """Arma un panel: las propiedades y el layout van sobre `this`."""
        root = self.data["root"]
        self.define_actions()
        self.props("this", root)
        if "layout" in root:
            lv = self.layout(root["layout"])
            self.emit("setLayout(%s);" % lv)

    def write_panel(self, out_dir, cls):
        self.panel()
        base = cls + "Base"
        # Los diálogos (Acerca de, Cómo empezar) derivan de QDialog; los demás paneles, de QWidget.
        qbase = "QDialog" if self.data["root"].get("qt") == "QDialog" else "QWidget"
        os.makedirs(out_dir, exist_ok=True)
        header = ["// ARCHIVO GENERADO por tools/gui_dump/gen_cpp.py a partir de la interfaz original - no editar a mano.",
                  "#pragma once", ""]
        header += ["#include <%s>" % h for h in QT_HEADERS]
        header += ['#include "%s"' % h for h in sorted(self.includes)]
        header += ["", "namespace flatcam::gui {", "",
                   "/**",
                   " * @brief Panel `%s` de FlatCAM 2024.4 (generado desde la interfaz original)." % self.data.get("class", cls),
                   " *",
                   " * `setupUi()` arma los mismos widgets, textos, valores de fábrica y layouts que el original. Los miembros tienen",
                   " * el nombre del atributo del original; la lógica va en una clase derivada.",
                   " */",
                   "class %s : public %s {" % (base, qbase),
                   "public:",
                   "    /** @brief Crea el panel vacío. @param parent Widget padre. */",
                   "    explicit %s(QWidget* parent = nullptr) : %s(parent) {}" % (base, qbase),
                   "",
                   "    /** @brief Construye el panel. */",
                   "    void setupUi();",
                   "", "    /** @name Objetos del panel (mismo nombre que en el original) */", "    ///@{"]
        for name in sorted(self.members):
            header.append("    %s* %s{nullptr};  ///< `%s` del original." % (self.members[name], name, name))
        header += ["    ///@}", "",
                   "    /** @brief Los miembros por su nombre (después de setupUi). @return Nombre -> objeto. */",
                   "    std::map<std::string, QObject*> membersByName() const;",
                   "};", "", "}  // namespace flatcam::gui", ""]
        header.insert(3, "#include <map>\n#include <string>")
        body = ["// ARCHIVO GENERADO por tools/gui_dump/gen_cpp.py a partir de la interfaz original - no editar a mano.",
                '#include "gui/generated/%s.h"' % base, "", "#include <QKeySequence>", "#include <QSpacerItem>", "",
                "namespace flatcam::gui {", "", "void %s::setupUi() {" % base]
        body += self.lines
        body += ["}", "", "std::map<std::string, QObject*> %s::membersByName() const {" % base, "    return {"]
        body += ['        {"%s", %s},' % (n, n) for n in sorted(self.members)]
        body += ["    };", "}", "", "}  // namespace flatcam::gui", ""]
        with open(os.path.join(out_dir, base + ".h"), "w", encoding="utf-8") as f:
            f.write("\n".join(header))
        with open(os.path.join(out_dir, base + ".cpp"), "w", encoding="utf-8") as f:
            f.write("\n".join(body))
        print("Generado %s: %d miembros, %d líneas" % (base, len(self.members), len(self.lines)))

    def write(self, out_dir):
        self.main_window()
        os.makedirs(out_dir, exist_ok=True)
        header = ["// ARCHIVO GENERADO por tools/gui_dump/gen_cpp.py a partir de la interfaz original - no editar a mano.",
                  "#pragma once", ""]
        header += ["#include <%s>" % h for h in QT_HEADERS]
        header += ['#include "%s"' % h for h in sorted(self.includes)]
        header += ["", "namespace flatcam::gui {", "",
                   "/**",
                   " * @brief Ventana principal con la misma estructura que `MainGUI` de FlatCAM 2024.4 (generada).",
                   " *",
                   " * `setupUi()` arma menús, acciones, barras, pestañas, barra de estado y paneles tal como quedan en el",
                   " * original después del arranque (primera ejecución, diseño `minimal`). Los miembros tienen el nombre del",
                   " * atributo del original. La lógica (señales, estados) va en la clase derivada `MainGUI`.",
                   " */",
                   "class MainGUIBase : public QMainWindow {",
                   "public:",
                   "    /** @brief Crea la ventana vacía. @param parent Widget padre. */",
                   "    explicit MainGUIBase(QWidget* parent = nullptr) : QMainWindow(parent) {}",
                   "",
                   "    /** @brief Construye toda la interfaz (equivale a `MainGUI.__init__` más el arranque de `App`). */",
                   "    void setupUi();",
                   "", "    /** @name Objetos de la interfaz (mismo nombre que en el original) */", "    ///@{"]
        for name in sorted(self.members):
            header.append("    %s* %s{nullptr};  ///< `%s` del original." % (self.members[name], name, name))
        header += ["    ///@}", "};", "", "}  // namespace flatcam::gui", ""]
        body = ["// ARCHIVO GENERADO por tools/gui_dump/gen_cpp.py a partir de la interfaz original - no editar a mano.",
                '#include "gui/generated/MainGUIBase.h"', "", "#include <QKeySequence>", "#include <QSpacerItem>", "",
                "namespace flatcam::gui {", "", "void MainGUIBase::setupUi() {"]
        body += self.lines
        body += ["}", "", "}  // namespace flatcam::gui", ""]
        with open(os.path.join(out_dir, "MainGUIBase.h"), "w", encoding="utf-8") as f:
            f.write("\n".join(header))
        with open(os.path.join(out_dir, "MainGUIBase.cpp"), "w", encoding="utf-8") as f:
            f.write("\n".join(body))
        print("Generado: %d miembros, %d líneas" % (len(self.members), len(self.lines)))


if __name__ == "__main__":
    if len(sys.argv) > 1 and sys.argv[1] == "--panels":
        pdir = os.path.join(os.path.dirname(os.path.abspath(__file__)), "panels")
        for cls in open(os.path.join(pdir, "index.txt")).read().split():
            Gen(json.load(open(os.path.join(pdir, cls + ".json"), encoding="utf-8"))).write_panel(OUT_DIR, cls)
    else:
        src = sys.argv[1] if len(sys.argv) > 1 else os.path.join(os.path.dirname(__file__), "main_gui.json")
        Gen(json.load(open(src, encoding="utf-8"))).write(OUT_DIR)
