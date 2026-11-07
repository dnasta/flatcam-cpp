#!/usr/bin/env python3
"""Genera `src/io/generated/DxfR2010Template.h`: el DXF vacío que escribe `ezdxf.new('R2010').saveas()` (ezdxf 0.14, el
que usa el original para exportar DXF), partido en las marcas que cambian con cada exportación: el `$HANDSEED`, los dos
GUID del encabezado, el identificador del registro APPID que ezdxf crea al guardar y las entidades.

Uso:  MAMBA_ROOT_PREFIX=$HOME/.local/share/mamba micromamba run -n old python tools/gen_dxf_template.py
"""
import os
import tempfile

import ezdxf

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
OUT = os.path.join(ROOT, "src", "io", "generated", "DxfR2010Template.h")


def main():
    fd, path = tempfile.mkstemp(suffix=".dxf")
    os.close(fd)
    doc = ezdxf.new("R2010")
    doc.modelspace()  # como export_dxf: pedir el espacio modelo cambia el orden de dos objetos al guardar
    doc.saveas(path)
    lines = open(path, encoding="utf-8").read().split("\n")
    os.remove(path)
    seed = None
    for i, l in enumerate(lines):
        if l == "$HANDSEED":
            seed = i + 2
    assert lines[seed - 1].strip() == "5"
    first = lines[seed]  # el próximo identificador libre de un documento vacío
    guids = [i + 2 for i, l in enumerate(lines) if l in ("$FINGERPRINTGUID", "$VERSIONGUID")]
    dates = [i + 2 for i, l in enumerate(lines) if l in ("$TDCREATE", "$TDUPDATE")]
    appid = [i for i, l in enumerate(lines) if l == "APPID" and lines[i + 2] == "%X" % (int(first, 16) - 1)]
    assert len(appid) == 1, appid
    appid_handle = appid[0] + 2
    entities = [i for i, l in enumerate(lines) if l == "ENTITIES"][0]
    marks = {seed: "@HANDSEED@", appid_handle: "@APPID@", guids[0]: "@GUID0@", guids[1]: "@GUID1@",
             dates[0]: "@DATE0@", dates[1]: "@DATE1@"}
    for k in marks:
        lines[k] = marks[k]
    lines.insert(entities + 1, "@ENTITIES@")
    text = "\n".join(lines)
    parts = []
    rest = text
    order = []
    while True:
        idx = [(rest.find(m), m) for m in ("@HANDSEED@", "@APPID@", "@GUID0@", "@GUID1@", "@DATE0@", "@DATE1@",
                                                  "@ENTITIES@") if m in rest]
        if not idx:
            parts.append(rest)
            break
        pos, m = min(idx)
        parts.append(rest[:pos])
        order.append(m.strip("@"))
        rest = rest[pos + len(m):]
    with open(OUT, "w", encoding="utf-8") as f:
        f.write("// ARCHIVO GENERADO por tools/gen_dxf_template.py (ezdxf %s) - no editar a mano.\n" % ezdxf.__version__)
        f.write("#pragma once\n#include <string>\n#include <vector>\nnamespace flatcam::dxf_template {\n")
        f.write("/// Primer identificador de entidad libre en un documento vacío (hexadecimal).\n")
        f.write('inline const char* kFirstHandle = "%s";\n' % "%X" % (int(first, 16) - 1))
        f.write("/// Qué va entre cada pedazo: %s.\n" % ", ".join(order))
        f.write("inline const std::vector<std::string> kMarks = {%s};\n" % ", ".join('"%s"' % o for o in order))
        f.write("/// Los pedazos del documento (el texto entre las marcas).\n")
        f.write("inline const std::vector<std::string> kParts = {\n")
        for p in parts:
            f.write('R"DXF(%s)DXF",\n' % p)
        f.write("};\n}  // namespace flatcam::dxf_template\n")
    print("Escrito", OUT, "marcas:", order, "primer identificador:", "%X" % (int(first, 16) - 1))


if __name__ == "__main__":
    main()
