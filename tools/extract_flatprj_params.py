#!/usr/bin/env python3
"""Extrae los PARÁMETROS de un proyecto de FlatCAM (.FlatPrj), descartando la geometría.

Un .FlatPrj de FlatCAM 2024.4 es un JSON (indent=2, claves ordenadas) que, por defecto, se guarda comprimido
con LZMA/XZ. Con NCC por láser puede pesar 1 GB o más, y descomprimido varios GB, así que no se carga entero
en memoria: se lee línea por línea y se conserva solo lo chico (opciones, herramientas, avances, Z, etc.).
Se descartan las figuras (`solid_geometry`, `follow_geometry`), el G-code y las cadenas gigantes.

Uso (solo usa la biblioteca estándar de Python 3):
    python3 tools/extract_flatprj_params.py "ruta/al/proyecto.FlatPrj" [salida.json] [--todas-las-opciones] [--con-agujeros]

Si no se indica la salida, escribe `<nombre del proyecto>.params.json` en la carpeta actual.
El resultado es chico (unos cientos de KB) y se puede subir al repositorio.

Con `--con-agujeros` se conservan además las posiciones de los agujeros y ranuras (`drills`, `slots`) de los objetos
Excellon, que son pocas; sirve, por ejemplo, para ver qué agujeros tiene realmente el objeto de alineación.

Privacidad: las opciones globales de FlatCAM pueden incluir rutas de carpetas y otros datos personales.
Por defecto solo se conservan las de las herramientas (`tools_*`, `geometry_*`, `excellon_*`, `gerber_*`,
`cncjob_*`, `units`); con `--todas-las-opciones` se conservan todas. Revisá el resultado antes de subirlo
a un repositorio público.
"""
import json
import lzma
import os
import sys
import time

# Claves que se descartan en cualquier nivel (contienen geometría o G-code).
SKIP_KEYS = {
    "solid_geometry", "follow_geometry", "gcode", "gcode_parsed", "geometry", "apertures",
    "drills", "slots", "geo_dict", "flat_geometry", "gerber_solid_geometry",
}
# Prefijos de las opciones globales de la aplicación que se conservan por defecto.
APP_OPTION_PREFIXES = ("tools_", "geometry_", "excellon_", "gerber_", "cncjob_", "global_gcode", "units")
# Cadenas más largas que esto (en caracteres de la línea) se reemplazan por un marcador.
MAX_LINE = 2000
XZ_MAGIC = b"\xfd7zXZ\x00"


def open_project(path):
    """Abre el proyecto en binario, descomprimiendo si es un archivo XZ/LZMA."""
    with open(path, "rb") as f:
        magic = f.read(6)
    return lzma.open(path, "rb") if magic == XZ_MAGIC else open(path, "rb")


def lines_of(f):
    """Entrega las líneas sin el salto final."""
    for raw in f:
        yield raw.decode("utf-8", errors="replace").rstrip("\r\n")


def shorten(line):
    """Si la línea es enorme, reemplaza su cadena por un marcador y conserva la coma final."""
    if len(line) <= MAX_LINE:
        return line
    comma = "," if line.endswith(",") else ""
    idx = line.find('": "')
    if 0 <= idx < 300:
        return line[: idx + 3] + '"<omitido: %d caracteres>"' % len(line) + comma
    indent = line[: len(line) - len(line.lstrip())]
    return indent + '"<omitido: %d caracteres>"' % len(line) + comma


def closing_of(indent):
    """Líneas que cierran un valor multilínea abierto con esta sangría."""
    pad = " " * indent
    return {pad + "}", pad + "},", pad + "]", pad + "],"}


def key_of(line):
    """Devuelve (sangría, clave, resto) si la línea es `    "clave": resto`, o None."""
    stripped = line.lstrip(" ")
    if not stripped.startswith('"'):
        return None
    end = stripped.find('": ', 1)
    if end < 0:
        return None
    indent = len(line) - len(stripped)
    return indent, stripped[1:end], stripped[end + 3:]


def skip_block(it, indent):
    """Consume líneas hasta el cierre del bloque abierto con la sangría `indent`; devuelve si traía coma."""
    closers = closing_of(indent)
    for line in it:
        if line in closers:
            return line.endswith(",")
    return False


def collect_block(it, first_line, indent):
    """Junta un valor multilínea (dict o lista) descartando claves pesadas. Devuelve su texto JSON."""
    out = [shorten(first_line)]
    closers = closing_of(indent)
    for line in it:
        if line in closers:
            out.append(line)
            break
        short_line = len(line) <= 300
        info = key_of(line[:300])
        if info and info[1] in SKIP_KEYS:
            if short_line and info[2] in ("[", "{"):
                comma = skip_block(it, info[0])
            else:
                comma = line.endswith(",")
            out.append(" " * info[0] + '"%s": "<omitido>"%s' % (info[1], "," if comma else ""))
            continue
        if info and short_line and info[2] in ("[", "{"):
            # Bloque anidado: se procesa recursivamente.
            out.append(collect_block(it, line, info[0]))
            continue
        out.append(shorten(line))
    text = "\n".join(out)
    return text


def strip_comma(text):
    text = text.rstrip()
    return text[:-1] if text.endswith(",") else text


def extract(path, progress=True):
    """Lee el proyecto y devuelve {"version", "options", "objs": [...]}."""
    result = {"objs": [], "options": None, "version": None}
    t0 = time.time()
    count = 0
    with open_project(path) as f:
        it = lines_of(f)
        section = None
        for line in it:
            count += 1
            if progress and count % 2_000_000 == 0:
                sys.stderr.write("  ... %d millones de líneas, %d objetos, %.0f s\n"
                                 % (count // 1_000_000, len(result["objs"]), time.time() - t0))
            if line == '  "objs": [':
                section = "objs"
                continue
            if section == "objs" and line == "  ],":
                section = None
                continue
            if section == "objs" and line in ("    {",):
                pieces = []
                for l2 in it:
                    count += 1
                    if l2 in ("    }", "    },"):
                        break
                    short_line = len(l2) <= 300
                    info = key_of(l2[:300])
                    if not info or info[0] != 6:
                        continue
                    _, key, rest = info
                    if key in SKIP_KEYS:
                        if short_line and rest in ("[", "{"):
                            skip_block(it, 6)
                        pieces.append('"%s": "<omitido>"' % key)
                    elif short_line and rest in ("[", "{"):
                        block = collect_block(it, l2, 6)
                        pieces.append(strip_comma(block))
                    else:
                        pieces.append(strip_comma(shorten(l2)))
                try:
                    result["objs"].append(json.loads("{" + ",\n".join(pieces) + "}"))
                except ValueError as e:
                    result["objs"].append({"__error__": "no se pudo leer el objeto: %s" % e})
                continue
            info = key_of(line[:300])
            if info and info[0] == 2 and info[1] == "options" and info[2] == "{" and len(line) <= 300:
                block = collect_block(it, line, 2)
                result["options"] = json.loads("{" + strip_comma(block) + "}")["options"]
            elif info and info[0] == 2 and info[1] == "version":
                result["version"] = json.loads(strip_comma(info[2]))
    return result


def filter_app_options(options):
    """Deja solo las opciones globales relacionadas con las herramientas (evita rutas y datos personales)."""
    if not isinstance(options, dict):
        return options
    return {k: v for k, v in options.items() if k.startswith(APP_OPTION_PREFIXES)}


def summarize(res):
    """Resumen legible por pantalla."""
    print("Versión de FlatCAM que guardó el proyecto:", res["version"])
    print("Objetos encontrados:", len(res["objs"]))
    for i, o in enumerate(res["objs"]):
        opts = o.get("options") or {}
        print("  %2d. [%s] %s" % (i + 1, o.get("kind", "?"), opts.get("name", "(sin nombre)")))


def main(argv):
    args = [a for a in argv[1:] if not a.startswith("--")]
    keep_all = "--todas-las-opciones" in argv
    if "--con-agujeros" in argv:
        SKIP_KEYS.discard("drills")
        SKIP_KEYS.discard("slots")
    if not args or "-h" in argv or "--help" in argv:
        print(__doc__)
        return 1
    path = args[0]
    out = args[1] if len(args) > 1 else os.path.splitext(os.path.basename(path))[0] + ".params.json"
    print("Leyendo:", path)
    res = extract(path)
    if not keep_all:
        res["options"] = filter_app_options(res["options"])
    with open(out, "w", encoding="utf-8") as f:
        json.dump(res, f, indent=2, sort_keys=True, ensure_ascii=False)
    summarize(res)
    print("Escrito: %s (%.1f KB)" % (out, os.path.getsize(out) / 1024))
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv))
