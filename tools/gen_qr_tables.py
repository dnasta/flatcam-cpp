#!/usr/bin/env python3
"""Genera src/tools/generated/QrTables.h con las tablas de la biblioteca `qrcode` 7.4.2 (la que usa FlatCAM 2024.4):
RS_BLOCK_TABLE (bloques de Reed-Solomon por versión y nivel) y rsPoly_LUT (polinomios generadores precalculados).

Uso:  MAMBA_ROOT_PREFIX=$HOME/.local/share/mamba micromamba run -n old python tools/gen_qr_tables.py
(con `python -m pip install qrcode==7.4.2` en el entorno).
"""
import os

import qrcode
from qrcode import LUT, base

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
OUT = os.path.join(ROOT, "src", "tools", "generated", "QrTables.h")


def main():
    assert qrcode.__version__ if hasattr(qrcode, "__version__") else True
    lines = ["// ARCHIVO GENERADO por tools/gen_qr_tables.py desde qrcode 7.4.2 - no editar a mano.",
             "// Copyright (c) 2011, Lincoln Loop (licencia BSD de la biblioteca qrcode; ver NOTICE).",
             "#pragma once", "#include <map>", "#include <vector>", "namespace flatcam::qr_tables {",
             "/// RS_BLOCK_TABLE: por versión y nivel (L, M, Q, H), tríos (cantidad, total, datos).",
             "inline const std::vector<std::vector<int>> kRsBlockTable = {"]
    for row in base.RS_BLOCK_TABLE:
        lines.append("    {%s}," % ", ".join(str(v) for v in row))
    lines += ["};", "/// rsPoly_LUT: polinomio generador para cada cantidad de palabras de corrección.",
              "inline const std::map<int, std::vector<int>> kRsPolyLut = {"]
    for k in sorted(LUT.rsPoly_LUT):
        lines.append("    {%d, {%s}}," % (k, ", ".join(str(v) for v in LUT.rsPoly_LUT[k])))
    lines += ["};", "}  // namespace flatcam::qr_tables"]
    os.makedirs(os.path.dirname(OUT), exist_ok=True)
    with open(OUT, "w") as fh:
        fh.write("\n".join(lines) + "\n")
    print("Escrito", OUT)


if __name__ == "__main__":
    main()
