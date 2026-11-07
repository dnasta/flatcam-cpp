#!/usr/bin/env python3
"""Genera los DXF de prueba de `tests/data/dxf/` con ezdxf 0.14 (el mismo que usa el oráculo).

Uso:  MAMBA_ROOT_PREFIX=$HOME/.local/share/mamba micromamba run -n old python tests/tools/make_test_dxf.py
"""
import os

import ezdxf

OUT = os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", "data", "dxf")


def main():
    os.makedirs(OUT, exist_ok=True)
    d = ezdxf.new("R2010")
    m = d.modelspace()
    m.add_line((0, 0), (10, 0))
    m.add_line((10, 0), (10, 5))
    m.add_circle((20, 5), 3)
    m.add_circle((30, 5), 2, dxfattribs={"extrusion": (0, 0, -1)})
    m.add_arc((40, 5), 4, 30, 200)
    m.add_arc((50, 5), 3, 300, 60)
    m.add_arc((60, 5), 3, 10, 100, dxfattribs={"extrusion": (0, 0, -1)})
    m.add_ellipse((70, 5), major_axis=(5, 1), ratio=0.5, start_param=0.3, end_param=4.0)
    m.add_lwpolyline([(0, 20), (5, 25), (10, 20), (15, 25)])
    m.add_lwpolyline([(20, 20), (30, 20), (30, 30), (20, 30)], dxfattribs={"closed": True})
    m.add_polyline2d([(40, 20), (45, 28), (50, 20)], dxfattribs={"closed": True})
    m.add_open_spline([(60, 20, 0), (62, 28, 0), (68, 22, 0), (72, 30, 0), (75, 21, 0)], degree=3)
    m.add_closed_spline([(60, 35, 0), (66, 38, 0), (70, 32, 0), (64, 30, 0)], degree=2)
    m.add_point((80, 25))
    m.add_solid([(0, 40), (5, 40), (0, 45), (5, 45)])
    b = d.blocks.new(name="TRI", base_point=(1, 1))
    b.add_lwpolyline([(1, 1), (5, 1), (3, 4)], dxfattribs={"closed": True})
    b.add_circle((3, 2), 0.5)
    m.add_blockref("TRI", (20, 40))
    m.add_blockref("TRI", (30, 40), dxfattribs={"xscale": 2, "yscale": 1.5, "rotation": 30})
    ins = m.add_blockref("TRI", (50, 40))
    ins.dxf.row_count = 2
    ins.dxf.row_spacing = 6
    ins.dxf.column_count = 3
    ins.dxf.column_spacing = 7
    d.saveas(os.path.join(OUT, "entities.dxf"))

    d = ezdxf.new("R12")
    m = d.modelspace()
    m.add_line((0, 0), (5, 0))
    m.add_line((5, 0), (5, 5))
    m.add_line((20, 0), (25, 5))
    m.add_polyline2d([(30, 0), (35, 5), (40, 0)])
    m.add_arc((50, 0), 5, 0, 180)
    d.saveas(os.path.join(OUT, "lines_r12.dxf"))

    # Un spline con puntos de ajuste (sin puntos de control): el original falla (DXFIndexError).
    d = ezdxf.new("R2010")
    d.modelspace().add_spline([(0, 0, 0), (2, 3, 0), (5, 1, 0), (8, 4, 0)])
    d.saveas(os.path.join(OUT, "spline_fit_points.dxf"))
    print("Escritos en", OUT)


if __name__ == "__main__":
    main()
