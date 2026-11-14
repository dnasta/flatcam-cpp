#!/usr/bin/env python3
"""Extrae de `tclCommands/TclCommand*.py` (con `ast`, sin ejecutarlo) los datos de cada comando de la consola Tcl:
la clase, la base (`TclCommand` o `TclCommandSignaled`), `aliases`, `description`, `arg_names` y `option_types` (nombre
y tipo de Python, en orden), `required`, `help` (`main`, `args` en orden y `examples`) y si `tclCommands/__init__.py` lo importa (solo esos se
registran). También el orden de las claves de `defaults.py` (`list_sys`). Escribe
`src/gui/resources/tcl_commands.json`; el port arma con eso el análisis de argumentos y la ayuda (`get_decorated_help`).

Uso: python3 tools/gui_dump/extract_tcl_commands.py
"""
import ast
import glob
import json
import os

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", ".."))
SRC = os.path.join(ROOT, "reference", "FlatCAM_2024_4", "FlatCAM", "tclCommands")
OUT = os.path.join(ROOT, "src", "gui", "resources", "tcl_commands.json")


def ev(node):
    """Evalúa las expresiones constantes que usan los comandos."""
    if isinstance(node, ast.Constant):
        return node.value
    if isinstance(node, (ast.List, ast.Tuple)):
        return [ev(e) for e in node.elts]
    if isinstance(node, ast.Dict):
        return {ev(k): ev(v) for k, v in zip(node.keys, node.values)}
    if isinstance(node, ast.Name):
        return {"__type__": node.id}  # str, float, int...
    if isinstance(node, ast.Attribute):
        return {"__type__": node.attr}
    if isinstance(node, ast.Call):
        fn = node.func
        name = fn.attr if isinstance(fn, ast.Attribute) else getattr(fn, "id", "")
        if name == "OrderedDict":
            return [list(p) for p in ev(node.args[0])] if node.args else []
        if name == "_":
            return ev(node.args[0])
        raise ValueError("llamada %s" % name)
    if isinstance(node, ast.BinOp) and isinstance(node.op, ast.Mod):
        return ev(node.left) % tuple(ev(node.right) if isinstance(node.right, ast.Tuple) else [ev(node.right)])
    if isinstance(node, ast.BinOp) and isinstance(node.op, ast.Add):
        return ev(node.left) + ev(node.right)
    raise ValueError(ast.dump(node))


def registered():
    """Los módulos que importa `tclCommands/__init__.py`: `register_all_commands` solo registra esos."""
    tree = ast.parse(open(os.path.join(SRC, "__init__.py"), encoding="utf-8").read())
    mods = set()
    for node in tree.body:
        if isinstance(node, ast.Import):
            for a in node.names:
                if a.name.startswith("tclCommands.TclCommand"):
                    mods.add(a.name.split(".")[1])
    return mods


def defaults_order():
    """El orden de las claves de `FlatCAMDefaults.factory_defaults` (el de `app.defaults`, que muestra `list_sys`)."""
    tree = ast.parse(open(os.path.join(SRC, "..", "defaults.py"), encoding="utf-8").read())
    for node in ast.walk(tree):
        if isinstance(node, ast.Assign) and any(getattr(t, "id", None) == "factory_defaults" for t in node.targets):
            return [k.value for k in node.value.keys]
    return []


def main():
    reg = registered()
    out = []
    for path in sorted(glob.glob(os.path.join(SRC, "TclCommand?*.py"))):
        tree = ast.parse(open(path, encoding="utf-8").read())
        for cls in tree.body:
            if not isinstance(cls, ast.ClassDef) or not cls.name.startswith("TclCommand"):
                continue
            d = {"class": cls.name, "base": [getattr(b, "id", getattr(b, "attr", "")) for b in cls.bases][0]}
            for st in cls.body:
                if isinstance(st, ast.Assign) and len(st.targets) == 1 and isinstance(st.targets[0], ast.Name):
                    key = st.targets[0].id
                    if key in ("aliases", "description", "arg_names", "option_types", "required", "help"):
                        d[key] = ev(st.value)
            for key in ("arg_names", "option_types"):
                d[key] = [[n, t["__type__"]] for n, t in d.get(key, [])]
            d.setdefault("required", [])
            d.setdefault("description", "")
            d["registered"] = cls.name in reg
            out.append(d)
    with open(OUT, "w", encoding="utf-8") as f:
        json.dump({"commands": out, "defaults_order": defaults_order()}, f, ensure_ascii=False, indent=1)
        f.write("\n")
    print(len(out), "comandos")


if __name__ == "__main__":
    main()
