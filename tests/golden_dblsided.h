// ARCHIVO GENERADO por tests/tools/oracle_dblsided.py - no editar a mano.
// Referencia: ToolDblSided.on_create_alignment_holes (FlatCAM 2024.4), Shapely 1.8.4.
#pragma once
#include <vector>
#include "core/Types.h"
namespace golden_dblsided {
struct Scenario { const char* name; flatcam::Axis axis; flatcam::Point2D pivot; std::vector<flatcam::Point2D> holes; std::vector<flatcam::Point2D> drills; };
inline const std::vector<Scenario> kScenarios = {
{"user_alignment_axis_x_origin", flatcam::Axis::X, {0.0, 0.0}, {{-38.0, 39.5}, {38.0, 39.5}}, {{-38.0, 39.5}, {-38.0, -39.5}, {38.0, 39.5}, {38.0, -39.5}}},
{"axis_y_origin", flatcam::Axis::Y, {0.0, 0.0}, {{-38.0, 39.5}, {38.0, 39.5}}, {{-38.0, 39.5}, {38.0, 39.5}, {38.0, 39.5}, {-38.0, 39.5}}},
{"axis_x_offset_point", flatcam::Axis::X, {3.3, -1.7}, {{0.1, 0.3}, {5.5, 7.7}, {-12.345, 4.0}}, {{0.1, 0.3}, {0.1, -3.6999999999999997}, {5.5, 7.7}, {5.5, -11.1}, {-12.345, 4.0}, {-12.345, -7.4}}},
{"axis_y_offset_point", flatcam::Axis::Y, {-2.25, 8.125}, {{1.0, 2.0}, {30.7, -4.4}}, {{1.0, 2.0}, {-5.5, 2.0}, {30.7, -4.4}, {-35.2, -4.4}}},
{"single_hole", flatcam::Axis::X, {10.0, 10.0}, {{12.5, 14.5}}, {{12.5, 14.5}, {12.5, 5.5}}},
{"hole_on_the_axis", flatcam::Axis::Y, {5.0, 5.0}, {{5.0, 9.0}}, {{5.0, 9.0}, {5.0, 9.0}}},
};
}  // namespace golden_dblsided
