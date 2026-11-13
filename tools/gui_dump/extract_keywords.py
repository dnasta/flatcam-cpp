#!/usr/bin/env python3
"""Extrae de `app_Main.App.__init__` (con `ast`, sin ejecutarlo) las listas de palabras del autocompletado:
`tcl_commands_list` (los comandos de FlatCAM) y `tcl_keywords` (las palabras de Tcl), en
`src/gui/resources/tcl_keywords.json`. Con `util_autocomplete_keywords` forman `App.myKeywords`.

Uso: python3 tools/gui_dump/extract_keywords.py
"""
import ast
import json
import os

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", ".."))
SRC = os.path.join(ROOT, "reference", "FlatCAM_2024_4", "FlatCAM", "app_Main.py")
OUT = os.path.join(ROOT, "src", "gui", "resources", "tcl_keywords.json")


def main():
    tree = ast.parse(open(SRC, encoding="utf-8").read())
    found = {}
    for node in ast.walk(tree):
        if isinstance(node, ast.Assign) and len(node.targets) == 1:
            t = node.targets[0]
            if isinstance(t, ast.Attribute) and t.attr in ("tcl_commands_list", "tcl_keywords") and \
                    isinstance(t.value, ast.Name) and t.value.id == "self" and t.attr not in found:
                found[t.attr] = ast.literal_eval(node.value)
    with open(OUT, "w", encoding="utf-8") as f:
        json.dump({"tcl_commands_list": found["tcl_commands_list"], "tcl_keywords": found["tcl_keywords"]}, f,
                  indent=1)
        f.write("\n")
    print(len(found["tcl_commands_list"]), len(found["tcl_keywords"]))


if __name__ == "__main__":
    main()
