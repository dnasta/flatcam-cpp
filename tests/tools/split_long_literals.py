#!/usr/bin/env python3
"""Parte los literales de texto largos de los goldens generados (MSVC no acepta literales de más de 16.380 bytes ni
concatenaciones de más de 65.535: errores C2026 y C1091).

Cada literal de más de 12.000 bytes se reemplaza por `::fctest::bigText({"trozo", "trozo", ...})` (tests/test_bigtext.h),
que los une en tiempo de ejecución y devuelve un `const char*` estable. Hay que correrlo después de regenerar un golden
(el test `SourceHygiene.NoStringLiteralTooLongForMsvc` avisa si quedó alguno):

    python3 tests/tools/split_long_literals.py            # todos los tests/golden_*.h
    python3 tests/tools/split_long_literals.py archivo.h ...
"""
import glob
import os
import re
import sys

LIMIT = 12000   # bytes de un literal entero a partir de los que se parte
CHUNK = 8000    # bytes de cada trozo
ONE = r'"(?:[^"\\\n]|\\.)*"'
# Una secuencia de literales pegados ("a" "b" o "a""b") es un solo texto.
LIT = re.compile(r'(?<![A-Za-z0-9_\'])' + ONE + r'(?:[ \t]*' + ONE + ')*')
PIECE = re.compile(ONE)
TOKEN = re.compile(r'\\x[0-9A-Fa-f]+|\\[0-7]{1,3}|\\u[0-9A-Fa-f]{4}|\\U[0-9A-Fa-f]{8}|\\.|.', re.S)
INCLUDE = '#include "test_bigtext.h"'


def split_body(pieces):
    """Trozos de hasta CHUNK bytes; cada trozo original se tokeniza aparte (un escape \\x seguido de un dígito
    hexadecimal se separa con "" como hacía el generador)."""
    parts, cur, size, prev = [], [], 0, ""
    for piece in pieces:
        for tok in TOKEN.findall(piece):
            n = len(tok.encode("utf-8"))
            if size + n > CHUNK and cur:
                parts.append("".join(cur))
                cur, size, prev = [], 0, ""
            if prev.startswith("\\x") and re.match(r"[0-9A-Fa-f]", tok):
                cur.append('""')
            cur.append(tok)
            size += n
            prev = tok
    parts.append("".join(cur))
    return parts


def process(path):
    text = open(path, encoding="utf-8").read()
    count = 0

    def repl(m):
        nonlocal count
        if len(m.group(0).encode("utf-8")) <= LIMIT:
            return m.group(0)
        count += 1
        pieces = [p[1:-1] for p in PIECE.findall(m.group(0))]
        return "::fctest::bigText({" + ", ".join('"%s"' % p for p in split_body(pieces)) + "})"

    # Los comentarios y las directivas (#include "...") no se tocan: se procesa línea por línea fuera de ellos.
    out = LIT.sub(repl, text)
    if count and INCLUDE not in out:
        lines = out.split("\n")
        idx = next((i for i, l in enumerate(lines) if l.startswith("#include")), None)
        if idx is None:
            idx = next(i for i, l in enumerate(lines) if l.startswith("#pragma once") or l.startswith("#define")) + 1
        lines.insert(idx, INCLUDE)
        out = "\n".join(lines)
    if out != text:
        open(path, "w", encoding="utf-8").write(out)
    return count


def main():
    root = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
    files = sys.argv[1:] or sorted(glob.glob(os.path.join(root, "golden_*.h")))
    for f in files:
        n = process(f)
        if n:
            print(f"{os.path.basename(f)}: {n} literales partidos")


if __name__ == "__main__":
    main()
