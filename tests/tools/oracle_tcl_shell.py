#!/usr/bin/env python3
"""Oráculo de la consola Tcl (`appTools/ToolShell.FCShell`, `tclCommands`), con la interfaz PyQt5 y el Tcl de
`tkinter` reales.

Crea la consola original (registra los 66 comandos y redefine `puts`) y guarda:
- la ayuda (`get_decorated_help`) y la descripción de cada alias;
- el texto de la consola (`toPlainText` y el HTML de lo agregado) después de escribir, como el usuario (Enter en la
  línea de comandos: `exec_current_command`), una serie de órdenes: Tcl puro (`set`, `expr`, `puts`, bucles,
  procedimientos), `help`, `help <comando>`, errores de argumentos (falta uno obligatorio, opción desconocida, tipo
  que no se puede convertir), un comando que no existe, `get_sys` / `set_sys` / `list_sys`, `get_names` y `clear`;
- `exec_command(..., no_echo=True)` (lo que usan los scripts) con su resultado.

Escribe `tests/data/tcl_shell/oracle.json`.
Uso: MAMBA_ROOT_PREFIX=$HOME/.local/share/mamba micromamba run -n old env QT_QPA_PLATFORM=offscreen \\
     python tests/tools/oracle_tcl_shell.py
"""
import json
import os
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(os.path.dirname(HERE))
sys.path.insert(0, os.path.join(ROOT, "tools", "gui_dump"))
import fc_runtime as fr  # noqa: E402

OUT = os.path.join(ROOT, "tests", "data", "tcl_shell", "oracle.json")

SESSION = [
    "set a 5",
    "expr {$a * 2}",
    "puts hola",
    "puts \"dos palabras\"",
    "for {set i 0} {$i < 3} {incr i} {puts $i}",
    "proc doble {x} {return [expr {$x * 2}]}",
    "doble 21",
    "set l [list a b c]; llength $l",
    "string toupper flatcam",
    "help",
    "help get_sys",
    "help no_existe",
    "help add_circle",
    "get_sys units",
    "get_sys no_existe",
    "getsys excellon_zeros",
    "set_sys global_gridx 2.5",
    "get_sys global_gridx",
    "set_sys no_existe 1",
    "set_sys",
    "get_sys",
    "list_sys cncjob_pl",
    "get_names",
    "set_active",
    "get_sys units extra",
    "list_sys -x 1",
    "no_existe 1 2",
    "add_circle obj abc 1 2",
    "error \"mi error\"",
    "clear",
    "set b 7",
]


SCRIPT = """# un comentario
set x 3

proc triple {v} {
    return [expr {$v * 3}]
}
triple $x
get_sys units
puts "listo"
set_sys no_existe 1
expr {1 +
"""


def main():
    qapp, fa, gui = fr.build_app()
    app_main = fr.import_original("app_Main")
    real_app = app_main.App
    tc = fr.import_original("tclCommands.TclCommand")
    tc.app_Main.App = type(fa)  # el isinstance(app, App) de TclCommand.__init__
    fa.defaults.propagate_defaults = lambda: None  # FlatCAMDefaults.propagate_defaults (avisa a la interfaz)
    fa.myKeywords = []
    fa.ui.app_icon = gui.app_icon if hasattr(gui, "app_icon") else None
    fa.defaults["global_verbose_error_level"] = fa.defaults.get("global_verbose_error_level", 0)
    shell_mod = fr.import_original("appTools.ToolShell")
    shell = shell_mod.FCShell(app=fa, version=fa.version)
    fa.shell = shell
    fa.inform.emit.side_effect = lambda m, *a: real_app.info(fa, m)  # como App.info: también a la consola
    fa.shell_message = lambda *a, **k: real_app.shell_message(fa, *a, **k)
    out = {"help": {}, "description": {}, "start": shell._browser.toPlainText(), "session": []}
    for alias, d in shell.tcl_commands_storage.items():
        out["help"][alias] = d["help"]
        out["description"][alias] = d["description"]
    for cmd in SESSION:
        before = shell._browser.toPlainText()
        shell._edit.setPlainText(cmd)
        shell.exec_current_command()
        after = shell._browser.toPlainText()
        added = after[len(before):] if after.startswith(before) else after
        out["session"].append({"cmd": cmd, "text": after, "added": added,
                               "gridx": str(fa.defaults.get("global_gridx"))})
    # exec_command sin eco: lo que usa un script.
    out["no_echo"] = []
    for cmd in ("expr {3 + 4}", "set c [get_sys units]", "puts $c"):
        out["no_echo"].append({"cmd": cmd, "result": shell.exec_command(cmd, no_echo=True)})
    # ScriptObject.handle_run_code (botón "Ejecutar" del editor de scripts): línea por línea; lo que da error se junta
    # con las líneas que siguen.
    from types import SimpleNamespace
    script = SCRIPT
    stub = SimpleNamespace(app=fa, script_editor_tab=SimpleNamespace(
        code_editor=SimpleNamespace(toPlainText=lambda: script)))
    shell._browser.clear()
    before = shell._browser.toPlainText()
    fr.import_original("appObjects.FlatCAMScript").ScriptObject.handle_run_code(stub)
    out["run_code"] = {"script": script, "added": shell._browser.toPlainText()[len(before):]}
    os.makedirs(os.path.dirname(OUT), exist_ok=True)
    with open(OUT, "w", encoding="utf-8") as f:
        json.dump(out, f, ensure_ascii=False, indent=1)


if __name__ == "__main__":
    main()
