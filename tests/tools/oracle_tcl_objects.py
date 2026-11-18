#!/usr/bin/env python3
"""Oráculo de los comandos Tcl de objetos (`new_geometry`, `add_circle`, `add_rectangle`, `add_polygon`,
`add_polyline`, `geo_union`, `exteriors`, `interiors`, `subtract_poly`, `subtract_rectangle`, `bounds`, `offset`,
`scale`, `skew`, `mirror`, `set_origin`, `delete`, `get_names`), con la consola original (`FCShell` y el Tcl de
`tkinter`) y la clase `GeometryObject` ORIGINAL con su `set_ui`.

La aplicación es la simulada de `fc_runtime` con una colección mínima y `app_obj.new_object` como el original:
las opciones de fábrica, `initialize(obj, app)` y después `ObjectCollection.append`, que llama a `set_ui` (que le
pone al objeto su herramienta 1 con `'solid_geometry': self.solid_geometry`). Después de cada orden guarda el texto
agregado a la consola y, de cada objeto, `multigeo`, `solid_geometry` (WKT con 17 cifras; si no es una lista, la
figura sola) y la geometría de la herramienta 1.

Escribe `tests/data/tcl_shell/objects.json`.
Uso: MAMBA_ROOT_PREFIX=$HOME/.local/share/mamba micromamba run -n old env QT_QPA_PLATFORM=offscreen \\
     python tests/tools/oracle_tcl_objects.py
"""
import json
import os
import sys

# Shapely entera antes de build_app: import_original descarta los módulos importados a medias.
import shapely.affinity  # noqa: F401
import shapely.geometry  # noqa: F401
import shapely.ops  # noqa: F401
import shapely.wkt  # noqa: F401
# El rtree real (fc_runtime lo simula si no está importado): lo usa el almacén de trazados del G-code.
import rtree.index  # noqa: F401

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(os.path.dirname(HERE))
sys.path.insert(0, os.path.join(ROOT, "tools", "gui_dump"))
import fc_runtime as fr  # noqa: E402
import dump_panels as dp  # noqa: E402

OUT = os.path.join(ROOT, "tests", "data", "tcl_shell", "objects.json")
DATA = os.path.join(ROOT, "tests", "data")

SESSION = [
    "new_geometry g",
    "add_circle g 0 0 1",
    "add_rectangle g 2 0 4 1",
    "add_polygon g 5 0 7 0 6 2",
    "add_polyline g 0 3 4 3 4 5",
    "add_polygon g 0 0 1",
    "add_circle no_existe 0 0 1",
    "add_polygon no_existe 0 0 1 1 2 0",
    "add_rectangle g 0 0 1 1 5",
    "bounds g",
    "new_geometry h",
    "add_rectangle h 0 0 10 1",
    "add_rectangle h 0 9 10 10",
    "add_rectangle h 0 0 1 10",
    "add_rectangle h 9 0 10 10",
    "geo_union h",
    "interiors h",
    "exteriors h -outname hx",
    "add_circle hx 20 20 1",
    "add_polyline hx 30 30 31 31",
    "bounds h,hx,h_interiors",
    "exteriors no_existe",
    "subtract_rectangle g 3 -1 6.5 4",
    "add_circle g 0 0 0.5",
    "add_polygon g 10 10 11 10 10 11",
    "subtract_poly g 0 0 1",
    "subtract_poly g 0 0 1 0 0 1",
    "bounds g",
    "new_geometry k",
    "add_rectangle k 1 1 3 2",
    "scale k -x 2",
    "add_rectangle k 1 1 3 2",
    "scale k 2 -origin origin",
    "scale k -x 2 -y 3 -origin min_bounds",
    "offset h 1 2",
    "skew h -x 10",
    "mirror h -axis X -origin 1,1",
    "mirror h -axis Y -box hx",
    "mirror hx",
    "bounds h",
    "scale k -x 2 -origin 3",
    "scale k -x 2 -origin (1,1)",
    "set_origin 1,2",
    "set_origin 1",
    "bounds g,h,hx,k",
    "bounding_box h",
    "bounding_box h -rounded true -margin 1 -outname hb",
    "bounding_box no_existe -rounded 0",
    "non_copper_regions h -rounded False",
    "non_copper_regions h -margin 0.5 -rounded 1 -outname hn",
    "add_circle hb 0 0 1",
    "add_polygon hn 0 0 1 0 0 1",
    "options hb",
    "options no_existe",
    # Para el G-code, un objeto armado solo con add_* (sin uniones: la GEOS 3.12 / 3.15 une distinto que la 3.11).
    "new_geometry c",
    "add_rectangle c 0 0 10 5",
    "add_circle c 20 20 3",
    "add_polyline c 0 10 5 15 10 10",
    "add_polygon c 30 0 35 0 32 4",
    "cncjob c",
    "write_gcode c_cnc @DIR@/c.nc",
    "cncjob c -dia 0.5 -z_cut 1 -z_move -3 -feedrate 300 -dpp 0.4 -spindlespeed 1000 -dwelltime 2 -toolchangez 10 "
    "-toolchangexy 1,2 -endxy (3,4) -endz 5 -startz 7 -extracut_length 0.2 -pp grbl_11 -outname cc",
    "write_gcode cc @DIR@/cc.nc -preamble {G04 P1} -postamble M30",
    "cncjob c -pp GRBL_11 -outname cx",
    "cncjob c -pp GRBL_laser -outname cl",
    "write_gcode cl @DIR@/cl.nc",
    "cncjob C -dpp 0.5 -extracut_length 0 -feedrate_z 30 -outname ck",
    "write_gcode ck @DIR@/ck.nc",
    "cncjob c -pp Marlin -toolchangez 5 -spindlespeed 200 -outname cm",
    "write_gcode cm @DIR@/cm.nc -preamble {M117 hola}",
    "cncjob hb -muted 1",
    "cncjob no_existe",
    "cncjob no_existe -muted 1",
    "cncjob hb -endxy 5",
    "cncjob k -toolchangexy 1,2,3",
    "write_gcode no_existe @DIR@/x.nc",
    "write_gcode h @DIR@/x.nc",
    "get_names",
    "delete hx",
    "get_names",
    "delete no_existe",
    "add_circle k 0 0 1",
    # drillcncjob: el taladro de "luz de noche" (@DATA@ es tests/data).
    "open_excellon {@DATA@/luz_de_noche/luz de noche Pascual-PTH.drl} -outname pth",
    "drillcncjob pth",
    "drillcncjob pth -drillz 1.5 -outname p1",
    "drillcncjob pth -drillz 1.5 -opt_type T -outname p1t",
    "write_gcode p1t @DIR@/p1t.nc",
    "drillcncjob pth -drillz 2.5 -opt_type T -dpp 0.8 -toolchangez 20 -pp Toolchange_Manual -outname p8",
    "write_gcode p8 @DIR@/p8.nc",
    "drillcncjob pth -drillz 2.5 -opt_type T -dpp 0.8 -pp GRBL_11 -spindlespeed 100 -outname p9",
    "write_gcode p9 @DIR@/p9.nc",
    "drillcncjob pth -drillz 2 -opt_type M -outname p10",
    "drillcncjob pth -drillz 2 -opt_type T -toolchangez 12 -travelz 3 -feedrate_z 80 -endz 9 -endxy 1,1 "
    "-spindlespeed 900 -dwelltime 1.5 -startz 4 -outname p2",
    "write_gcode p2 @DIR@/p2.nc",
    "drillcncjob pth -drillz 2 -opt_type N -dpp 0.7 -pp Marlin -outname p3",
    "write_gcode p3 @DIR@/p3.nc",
    "drillcncjob pth -drillz 2 -drilled_dias 0.8 -outname p4",
    "drillcncjob pth -drillz 2 -toolchangexy 1,2 -outname p5",
    "drillcncjob pth -drillz 0 -outname p6",
    "drillcncjob pth -drillz 2 -pp nada -outname p7",
    "drillcncjob k",
    "drillcncjob no_existe",
    "drillcncjob no_existe -muted 1",
    "drillcncjob PTH -drillz 2",
    # milldrills / millslots: obj.drills / obj.slots no existen en la 2024.4.
    "milldrills pth",
    "millslots pth -tooldia 0.5",
    "milldrills no_existe",
    # cutout (rectangular, sin puentes finos) y isolate sobre el contorno de "luz de noche".
    "open_gerber {@DATA@/luz_de_noche/luz de noche Pascual-Edge_Cuts.gbr} -outname ec",
    "cutout ec",
    "cutout ec -gaps tb -dia 1 -margin 0.5 -gapsize 1 -outname ec2",
    "cutout ec -gaps lr -outname ec3",
    "cutout ec -gaps 8 -outname ec4",
    "cutout no_existe",
    "cutout pth -outname ec5",
    "isolate ec",
    "isolate ec -combine 1 -dia 0.2 -passes 2 -overlap 10 -outname i1",
    "isolate ec -combine 0 -passes 2",
    "isolate ec -combine 0 -dia 0.3 -outname i3",
    "isolate ec -combine 1 -iso_type 0 -outname i4",
    "isolate ec -combine 1 -iso_type 1 -outname i5",
    "isolate ec -combine True -follow 1 -outname i6",
    "isolate ec -combine 1 -iso_type 5 -outname i7",
    "isolate ec -combine 0 -iso_type 5 -outname i8",
    "isolate pth -combine 1",
    # geocutout: de un Gerber (el aislamiento exterior con ingletes) y de una Geometría, con cada tipo de puentes.
    "geocutout ec -dia 1.2 -outname g1",
    "geocutout ec -gaps lr -outname g2",
    "geocutout ec -gaps tb -margin 1 -gapsize 2 -outname g3",
    "geocutout ec -gaps 2lr -outname g4",
    "geocutout ec -gaps 2tb -outname g5",
    "geocutout ec -gaps 8 -outname g6",
    "geocutout i1 -gaps 4 -outname g7",
    "geocutout ec -gaps 3",
    "geocutout ec -dia 0",
    "geocutout pth",
    "geocutout no_existe",
    # panelize: Gerber, Geometría de una sola geometría y multigeo, y Excellon; con caja y espaciados enteros.
    "panelize ec -rows 2 -columns 3 -outname pa1",
    "panelize ec_cutout -rows 1 -columns 2 -spacing_columns 2.7 -outname pa2",
    "panelize i1 -columns 2 -box ec -spacing_rows 1 -outname pa3",
    "panelize i1 -columns 2 -rows 2 -box ec -spacing_rows 1 -outname pa7",
    "panelize pth -rows 2 -columns 1 -outname pa4",
    "bounds pth",
    # aligndrill y aligndrillgrid: Excellon de alineación (los agujeros pedidos y sus reflejos, o una grilla).
    "aligndrill ec -axis X -holes (1,2),(3,4) -dia 1 -outname a1",
    "aligndrill ec -axis y -holes (1.5,-2) -dia 0.8 -dist 5 -outname a2",
    "aligndrill ec -axis X -box ec -grid 10 -gridoffset 0 -minoffset 2 -dia 1 -outname a3",
    "aligndrill ec -axis Y -box ec -grid 7 -gridoffset 1 -minoffset 0 -axisoffset 3 -dia 1.2 -outname a4",
    "aligndrill ec -axis X -box ec -holes (0,10) -dia 1 -outname a5",
    "aligndrill ec -axis X -grid 10 -gridoffset 0 -dia 1 -outname a6",
    "aligndrill ec -axis X -box ec -grid 10 -gridoffset 0 -dia 1 -outname a7",
    "aligndrill i1 -axis X -holes (1,2) -dia 1",
    "aligndrill ec -holes (1,2) -dia 1",
    "aligndrill ec -axis X -dia 1",
    "aligndrill ec -axis X -holes (1,2)",
    "aligndrill ec -axis Z -holes (1,2) -dia 1",
    "aligndrill ec -axis X -holes (1,2) -dia 1 -box no_existe",
    "aligndrill c_cnc -axis X -holes (1,2) -dia 1",
    "aligndrill no_existe -axis X",
    "aligndrillgrid -dia 1 -gridx 2 -gridy 3 -columns 3 -rows 2 -outname ag1",
    "aligndrillgrid -dia 0.5 -gridx 2.5 -gridy 3 -columns 2 -rows 1 -gridoffsetx 5 -gridoffsety -1",
    "aligndrillgrid -gridx 2 -gridy 3 -columns 2 -rows 1",
    "aligndrillgrid -dia 1 -gridx 2 -columns 2 -rows 1 -outname ag3",
    # ncc_clear (clear_copper_tcl). Con un solo polígono, "sí mismo" no usa la envolvente convexa: se limpian las
    # franjas del margen a los dos lados del contorno (rectángulos: las pasadas caen sobre sus bordes y la intersección
    # depende de la versión de GEOS, y el contorno es simétrico respecto de x = 0, así que paint_connect elige entre
    # empates exactos de rtree.nearest). Los casos con varias herramientas van sobre el logo, sin esas coincidencias.
    "ncc_clear ec -tooldia 2 -margin 5 -all 1 -outname n1",
    "open_gerber {@DATA@/luz_de_noche/luz de noche Pascual-Logo.gbr} -outname lg",
    "ncc_clear lg -tooldia 1,0.6 -overlap 20 -margin 0.5 -order fwd -all 1 -outname n2",
    "ncc_clear lg -tooldia 1,0.6 -order rev -margin 0.5 -connect 0 -contour 0 -all 1 -outname n3",
    "ncc_clear lg -tooldia 1,0.6 -margin 0.5 -rest 1 -all 1 -outname n4",
    "ncc_clear lg -tooldia 0.8 -margin 0.5 -offset 0.2 -all 1 -outname n5",
    "ncc_clear ec -tooldia 3 -all 1 -outname n6",
    "isolate lg -combine 1 -dia 0.5 -outname lgi",
    "ncc_clear lg -tooldia 0.4 -margin 0.3 -box lgi -outname n7",
    "ncc_clear ec -tooldia 3 -box ec -outname n8",
    "ncc_clear ec -tooldia 3 -box pth -outname n9",
    "ncc_clear ec -tooldia 3 -box no_existe -outname n10",
    "ncc_clear ec -tooldia 3",
    "ncc_clear ec_cutout -tooldia 3 -margin 2 -all 1 -outname n11",
    "ncc_clear pth -tooldia 3 -all 1 -outname n12",
    "ncc_clear lg -tooldia 0.8 -margin 0.5 -rest 1 -offset 0.2 -all 1 -outname n13",
    "ncc_clear no_existe -all 1",
    "ncc_clear pth -tooldia 3 -rest 1 -all 1 -outname n14",
    # paint: el método llega como texto y paint_polygon_worker lo compara con números: nunca pinta.
    "paint ec -all 1 -outname pt1",
    "paint ec -tooldia 2,1 -method lines -offset 0.5 -all 1 -outname pt2",
    "paint ec -tooldia 2 -offset 50 -all 1 -outname pt3",
    "paint ec_cutout -all 1 -outname pt4",
    "paint ec -single 0,0 -outname pt5",
    "paint ec -single 34.975,0 -outname pt6",
    "paint ec -single , -outname pt10",
    "paint ec -single 1,2,3",
    "paint ec -box g1 -outname pt7",
    "paint ec -box ec -outname pt8",
    "paint ec -box no_existe -outname pt9",
    "paint ec -tooldia 1",
    "paint no_existe -all 1",
    "panelize ec -outname pa5",
    "panelize ec -rows 1 -box no_existe",
    "panelize no_existe -rows 1",
    "isolate nada -combine 1",
]


class Collection:
    """Lo que usan los comandos de `ObjectCollection`."""

    def __init__(self, app, ou):
        self.app, self.ou, self.objs, self.active = app, ou, [], []

    def get_by_name(self, name, isCaseSensitive=None):
        # ObjectCollection.get_by_name
        if isCaseSensitive is None or isCaseSensitive is True:
            return next((o for o in self.objs if o.options["name"] == name), None)
        return next((o for o in self.objs if o.options["name"].lower() == name.lower()), None)

    def promise(self, name):
        # ObjectCollection.promise: solo anota el nombre que se va a crear.
        pass

    def get_names(self):
        return [o.options["name"] for o in self.objs]

    def get_list(self):
        return list(self.objs)

    def append(self, obj):
        import re
        name = obj.options["name"]
        while name in self.get_names():  # ObjectCollection.append: "Prevent same name"
            match = re.search(r'(.*[^\d])?(\d+)$', name)
            if match:
                name = (match.group(1) or '') + str(int(match.group(2)) + 1)
            else:
                name += "_1"
        obj.options["name"] = name
        self.objs.append(obj)
        obj.set_ui(obj.ui_type(app=self.app))  # ObjectCollection.append

    def set_active(self, name):
        obj = self.get_by_name(name)
        obj.item  # ObjectCollection.set_active: obj.item (AttributeError con None)
        self.active.append(obj)

    def set_all_active(self):
        self.active = list(self.objs)

    def set_all_inactive(self):
        self.active = []


def flat(lst):
    for g in lst:
        if isinstance(g, list):
            yield from flat(g)
        else:
            yield g


def geo_state(geo):
    if geo is None:
        return None
    if isinstance(geo, list):
        # Las listas anidadas (panelize) se aplanan, como la ShapeList del port.
        return [wkt(g) if hasattr(g, "wkt") else repr(g) for g in flat(geo)]
    return {"single": wkt(geo)}


def wkt(g):
    return shapely.wkt.dumps(g, trim=True, rounding_precision=17)


def main():
    qapp, fa, gui = fr.build_app()
    app_main = fr.import_original("app_Main")
    real_app = app_main.App
    tc = fr.import_original("tclCommands.TclCommand")
    tc.app_Main.App = type(fa)
    fa.defaults.propagate_defaults = lambda: None
    fa.myKeywords = []
    fa.abort_flag = False
    fa.dec_format = lambda val, dec=None: float('%.*f' % (fa.decimals if dec is None else dec, val))
    fr.import_original("appObjects.FlatCAMObj").FlatCAMObj.app = fa
    ou = fr.import_original("appGUI.ObjectUI")
    geo_mod = fr.import_original("appObjects.FlatCAMGeometry")
    coll = Collection(fa, ou)
    fa.collection = coll

    cnc_mod = fr.import_original("appObjects.FlatCAMCNCJob")
    exc_mod = fr.import_original("appObjects.FlatCAMExcellon")
    grb_mod = fr.import_original("appObjects.FlatCAMGerber")

    class FileHandlers:
        """MenuFileHandlers.open_excellon original sobre la aplicación simulada."""
        def __init__(self):
            self.app, self.inform, self.defaults, self.log = fa, fa.inform, fa.defaults, fa.log

        def open_excellon(self, *a, **k):
            return app_main.MenuFileHandlers.open_excellon(self, *a, **k)

        def open_gerber(self, *a, **k):
            return app_main.MenuFileHandlers.open_gerber(self, *a, **k)
    fa.f_handlers = FileHandlers()
    # OR-Tools no está en el entorno: los órdenes 'B' y 'M' quedan en el orden del archivo (el port usa su propio
    # optimizador, así que de esos trabajos solo se comparan los avisos).
    camlib = fr.import_original("camlib")
    camlib.CNCjob.optimized_ortools_basic = lambda self, locations: list(range(len(locations)))
    camlib.CNCjob.optimized_ortools_meta = lambda self, locations, opt_time=None: list(range(len(locations)))

    def new_object(kind, name, initialize, plot=True, autoselected=True, callback=None, callback_params=None):
        assert kind in ("geometry", "cncjob", "excellon", "gerber")
        obj = (geo_mod.GeometryObject(name) if kind == "geometry" else cnc_mod.CNCJobObject(name) if kind == "cncjob"
               else exc_mod.ExcellonObject(name) if kind == "excellon" else grb_mod.GerberObject(name))
        for option, value in fa.options.items():
            if option.startswith(kind + "_"):
                obj.options[option[len(kind) + 1:]] = value
            elif option.startswith(dp.EXTRA_OPTIONS[kind]):
                obj.options[option] = value
        obj.options["name"] = name
        obj.item = object()
        obj.plot = lambda *a, **k: None
        obj.units = fa.options["units"]
        try:
            ret = initialize(obj, fa)
        except Exception as e:
            # AppObject.new_object (sin el traceback de Python, que el port no muestra)
            msg = '[ERROR_NOTCL] %s' % fr.gettext("An internal error has occurred. See shell.\n")
            msg += fr.gettext("Object ({kind}) failed because: {error} \n\n").format(kind=kind, error=str(e))
            fa.inform.emit(msg)
            return "fail"
        # AppObject.new_object: el aviso en la consola (también si initialize devuelve "fail").
        fa.inform_shell.emit("%s %s. %f seconds executing initialize()." % (fr.gettext("New object with name:"), name, 0.0))
        if ret == "fail":
            return "fail"
        try:
            xmin, ymin, xmax, ymax = obj.bounds()
            obj.options['xmin'], obj.options['ymin'], obj.options['xmax'], obj.options['ymax'] = xmin, ymin, xmax, ymax
        except Exception:
            return "fail"
        if kind == "cncjob":
            coll.objs.append(obj)  # (los nombres de los CNC Job de esta sesión no se repiten)  # sin set_ui (el CNC Job no lo necesita para exportar)
        else:
            coll.append(obj)
    fa.app_obj.new_object = new_object
    fa.preprocessors = fr.import_original("appPreProcessor").load_preprocessors()
    # Los postprocesadores usan math.ceil sin importar math (error del original: con profundidad múltiple la cabecera
    # queda vacía); el port y los demás oráculos se comportan como si estuviera importado.
    import math
    for pp in fa.preprocessors.values():
        sys.modules[type(pp).__module__].math = math
    # Las áreas de exclusión con el travel_coordinates original (un MagicMock no devuelve viajes).
    from oracle_exclusion import exc_areas
    fa.exc_areas = exc_areas(fa, [])
    # La herramienta NCC original sin su interfaz (clear_copper_tcl de ncc_clear), como la deja __init__.
    ncc_mod = fr.import_original("appTools.ToolNCC")
    nt = ncc_mod.NonCopperClear.__new__(ncc_mod.NonCopperClear)
    nt.app = fa
    nt.decimals = 4
    ncc_mod.Gerber.__init__(nt, steps_per_circle=fa.defaults["gerber_circle_steps"])
    nt.obj_name, nt.ncc_obj, nt.sel_rect = "", None, []
    nt.circle_steps = int(fa.defaults["gerber_circle_steps"])
    fa.ncclear_tool = nt
    fa.abort_flag = False

    fa.on_set_zero_click = lambda *a, **k: real_app.on_set_zero_click(fa, *a, **k)

    def on_delete(force_deletion=False):
        for o in coll.active:
            coll.objs.remove(o)
    fa.on_delete = on_delete

    # TclCommandSignaled: execute corre en el hilo de trabajo y la consola espera `shell_command_finished` con un
    # QEventLoop; aquí se corre en el mismo hilo, pero después de entrar en el bucle (como las señales encoladas).
    from PyQt5 import QtCore

    class Signals(QtCore.QObject):
        shell_command_finished = QtCore.pyqtSignal(object)
        thread_exception = QtCore.pyqtSignal(object)
    sig = Signals()
    fa._signals = sig
    fa.shell_command_finished = sig.shell_command_finished
    fa.thread_exception = sig.thread_exception
    fa.worker_task.emit = lambda d: QtCore.QTimer.singleShot(0, lambda: d['fcn'](*d['params']))
    fa.defaults["global_background_timeout"] = 300000

    shell_mod = fr.import_original("appTools.ToolShell")
    shell = shell_mod.FCShell(app=fa, version=fa.version)
    fa.shell = shell
    fa.inform.emit.side_effect = lambda m, *a: real_app.info(fa, m)
    fa.inform_shell.emit.side_effect = lambda m, *a: real_app.info_shell(fa, m, *a)
    fa.shell_message = lambda *a, **k: real_app.shell_message(fa, *a, **k)
    out = {"session": []}
    import tempfile
    tmp = tempfile.mkdtemp()
    last = {}
    for cmd in SESSION:
        written = set(os.listdir(tmp))
        cmd_real = cmd.replace("@DIR@", tmp).replace("@DATA@", DATA)
        before = shell._browser.toPlainText()
        shell._edit.setPlainText(cmd_real)
        shell.exec_current_command()
        after = shell._browser.toPlainText()
        files = {}
        for fn in sorted(set(os.listdir(tmp)) - written):
            with open(os.path.join(tmp, fn), encoding="utf-8", newline="") as f:
                # Sin la fecha ni la versión de la cabecera.
                files[fn] = "".join(l for l in f.read().splitlines(True)
                                    if "Created on" not in l and "GENERATED BY FLATCAM" not in l)
        objs = []
        for o in coll.objs:
            if o.kind not in ("geometry",):
                st = {"name": o.options["name"], "kind": o.kind}
                if o.kind == "excellon" and o.options["name"] != "pth":
                    # Los Excellon que crean los comandos: diámetro, agujeros y área de cada herramienta.
                    st["tools"] = [{"key": str(k), "tooldia": t.get("tooldia"),
                                    "drills": [[p.x, p.y] for p in t.get("drills", [])],
                                    "area": sum(g.area for g in t.get("solid_geometry", []))}
                                   for k, t in o.tools.items()]
                objs.append(st)
                continue
            t1 = o.tools.get(1, o.tools.get('1', {})).get("solid_geometry") if o.tools else None
            objs.append({"name": o.options["name"], "multigeo": bool(o.multigeo),
                         "solid": geo_state(o.solid_geometry), "tool1": geo_state(t1),
                         "tool1_is_solid": bool(o.tools) and 1 in o.tools and
                         o.tools[1]["solid_geometry"] is o.solid_geometry,
                         "tool_keys": [str(k) for k in o.tools] if o.tools else []})
        for o in objs:
            # Los trazados del NCC (paint_connect elige entre empates exactos de rtree.nearest, que el port no
            # reproduce): se comparan los totales.
            if cmd.startswith("ncc_clear") and o["name"] not in last and "solid" in o:
                o["relaxed"] = True
            elif o["name"] in last and '"relaxed": true' in last[o["name"]]:
                o["relaxed"] = True
        added = after[len(before):] if after.startswith(before) else after
        added = added.replace(tmp, "@DIR@").replace(DATA, "@DATA@")
        # El traceback de Python (rutas locales) no se guarda: el port no lo muestra.
        cut = added.upper().find("\nPYTHON TRACEBACK")
        out["session"].append({"cmd": cmd, "added": added if cut < 0 else added[:cut + 1], "traceback": cut >= 0,
                               # on_delete está simulado: sus avisos no son los del original
                               "simulated": cmd.startswith("delete ") and "failed" not in added,
                               # los nombres de todos y el estado de los que cambiaron (el test recuerda el resto)
                               "names": [o["name"] for o in objs],
                               "objects": [o for o in objs if last.get(o["name"]) != json.dumps(o, sort_keys=True)],
                               "files": files})
        last = {o["name"]: json.dumps(o, sort_keys=True) for o in objs}
    os.makedirs(os.path.dirname(OUT), exist_ok=True)
    with open(OUT, "w", encoding="utf-8") as f:
        json.dump(out, f, ensure_ascii=False, indent=1)


if __name__ == "__main__":
    main()
