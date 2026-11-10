// ARCHIVO GENERADO por tests/tools/oracle_align.py - no editar a mano.
// Referencia: AlignObjects.on_mouse_click_release / check_points / align_translate / align_rotate.
#pragma once
#include <vector>
namespace golden_align {
struct Pick { const char* target; double x; double y; std::vector<double> points; };
struct Transform { const char* type; std::vector<double> points; bool fails; double dx; double dy; bool rotates; double angle; double ox; double oy; };
inline const std::vector<Pick> kPicks = {
{"gerber", -25.14, -29.18, {-25.14, -29.18}},
{"gerber", -25.09, -29.21, {-25.14, -29.18}},
{"gerber", 21.599999999999998, -28.32, {21.6, -28.32}},
{"gerber", 21.65, -28.35, {21.6, -28.32}},
{"gerber", 12.86, 27.919999999999998, {12.86, 27.92}},
{"gerber", 12.91, 27.889999999999997, {12.86, 27.92}},
{"gerber", -32.545, 21.415, {-32.545, 21.415}},
{"gerber", -32.495000000000005, 21.384999999999998, {-32.545, 21.415}},
{"gerber", 18.72, -5.2299999999999995, {18.72, -5.23}},
{"gerber", 18.77, -5.26, {18.72, -5.23}},
{"gerber", 5.22, 26.93, {5.22, 26.93}},
{"gerber", 5.27, 26.9, {5.22, 26.93}},
{"gerber", -2.28, -10.559999999999999, {}},
{"gerber", 1000.0, 1000.0, {}},
{"excellon", -32.545, 21.415, {-32.545, 21.415}},
{"excellon", -32.445, 21.515, {-32.545, 21.415}},
{"excellon", 2.13, -20.6, {2.13, -20.6}},
{"excellon", 2.23, -20.5, {2.13, -20.6}},
{"excellon", 10.23, -24.1, {10.23, -24.1}},
{"excellon", 10.33, -24.0, {10.23, -24.1}},
{"excellon", -25.14, -29.18, {-25.14, -29.18}},
{"excellon", -25.04, -29.08, {-25.14, -29.18}},
{"excellon", 1000.0, 1000.0, {}},
};
inline const std::vector<Transform> kTransforms = {
{"sp", {0.0, 0.0, 10.0, 5.0}, false, 10, 5, false, 0.0, 0.0, 0.0},
{"dp", {0.0, 0.0, 10.0, 5.0, 10.0, 0.0, 20.0, 10.0}, false, 10, 5, false, 0.0, 0.0, 0.0},
{"dp", {1.0, 1.0, 2.0, 3.0, 5.0, 1.0, 6.0, 4.0}, false, 1, 2, false, 0.0, 0.0, 0.0},
{"dp", {-3.5, 2.25, 1.125, -0.5, 4.0, 6.0, -2.0, 8.0}, false, 4.625, -2.75, true, -96.37924816761314, 1.125, -0.5},
{"dp", {0.0, 0.0, 0.0, 0.0, 3.0, 4.0, 3.0, 7.0}, false, 0, 0, false, 0.0, 0.0, 0.0},
{"dp", {0.0, 0.0, 0.0, 0.0, 3.0, 4.0, 5.0, 4.0}, false, 0, 0, false, 0.0, 0.0, 0.0},
{"dp", {0.0, 0.0, 1.0, 1.0, 4.0, 5.0, 1.0, 9.0}, true, 1, 1, false, 0.0, 0.0, 0.0},
{"dp", {0.0, 0.0, 1.0, 1.0, 1.0, 5.0, 6.0, 9.0}, false, 1, 1, true, -20.695450734063286, 1.0, 1.0},
{"dp", {0.0, 0.0, 0.0, 0.0, 2.0, 1.0, 1.0, 2.0}, false, 0, 0, true, 36.86989764584402, 0.0, 0.0},
{"dp", {0.0, 0.0, 1.0, 1.0, 0.0, 4.0, 3.0, 2.0}, true, 1, 1, false, 0.0, 0.0, 0.0},
};
}  // namespace golden_align
