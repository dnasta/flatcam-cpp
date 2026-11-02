#!/usr/bin/env python3
"""Extrae las plantillas de texto multilínea de los postprocesadores ORIGINALES de FlatCAM.

Cada postprocesador arma bloques como el cambio de herramienta con cadenas entre triple comilla y `.format(...)`. Esas
cadenas llevan espacios al final de línea y tabulaciones que importan (el G-code tiene que salir idéntico), así que no
se copian a mano: este script las lee con `ast` y escribe `src/cam/generated/PreprocessorTemplates.h` con un arreglo
por método y postprocesador, en el orden en que aparecen en el código (`k<pp>_<método>[i]`).

Uso:  python3 tools/gen_pp_templates.py
"""
import ast
import os

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
SRC = os.path.join(ROOT, "reference", "FlatCAM_2024_4", "FlatCAM", "preprocessors")
OUT = os.path.join(ROOT, "src", "cam", "generated", "PreprocessorTemplates.h")


def ident(name):
    return "".join(c if c.isalnum() else "_" for c in name)


def main():
    out = ["// ARCHIVO GENERADO por tools/gen_pp_templates.py - no editar a mano.",
           "// Plantillas multilínea de los postprocesadores originales de FlatCAM 2024.4, tal cual (con sus espacios).",
           "#pragma once", "", "namespace flatcam::pp_templates {", ""]
    for fname in sorted(os.listdir(SRC)):
        if not fname.endswith(".py") or fname == "__init__.py":
            continue
        tree = ast.parse(open(os.path.join(SRC, fname), encoding="utf-8").read())
        for cls in [n for n in tree.body if isinstance(n, ast.ClassDef)]:
            for fn in [n for n in cls.body if isinstance(n, ast.FunctionDef)]:
                strings = [n.value for n in ast.walk(fn)
                           if isinstance(n, ast.Constant) and isinstance(n.value, str) and "\n" in n.value
                           and len(n.value) > 2]
                # ast.walk no garantiza el orden del código: se ordena por posición.
                # Solo las plantillas de `.format` (las de `%` se arman a mano).
                nodes = sorted([n for n in ast.walk(fn) if isinstance(n, ast.Constant) and isinstance(n.value, str)
                                and "\n" in n.value and len(n.value) > 2 and "%" not in n.value
                                and "{" in n.value], key=lambda n: (n.lineno, n.col_offset))
                strings = [n.value for n in nodes]
                if not strings:
                    continue
                name = "k%s_%s" % (ident(fname[:-3]), fn.name)
                for s in strings:
                    assert ")PPT\"" not in s
                out.append("inline const char* const %s[] = {" % name)
                for s in strings:
                    out.append('    R"PPT(%s)PPT",' % s)
                out.append("};")
                out.append("")
    out.append("}  // namespace flatcam::pp_templates")
    with open(OUT, "w", encoding="utf-8") as f:
        f.write("\n".join(out) + "\n")
    print("Escrito", OUT)


if __name__ == "__main__":
    main()
