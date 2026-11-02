// ARCHIVO GENERADO por tests/tools/oracle_geometry_modes.py - no editar a mano.
// Referencia: CNCjob.geometry_tool_gcode_gen original (FlatCAM 2024.4) con varias geometrías y opciones.
#pragma once
#include <array>
#include <string>
#include <vector>
#include "cam/CncJob.h"
namespace golden_geometry_modes {
inline flatcam::ShapeList geometry0() {  // multidepth_open_line
    return {flatcam::Shape::fromWkt("LINESTRING (0 0, 10 0, 10 5)")};
}
inline flatcam::GeometryJobSettings settings0() {
    flatcam::GeometryJobSettings s;
    s.units = "MM";
    s.preprocessor = "default";
    s.order = flatcam::DrillOrder::FileOrder;
    s.tool = 1;
    s.toolchange = true;
    flatcam::GeometryToolSettings& t = s.toolSettings;
    t.toolDia = 1.2;
    t.offsetKind = "Path";
    t.offsetValue = 0.0;
    t.cutZ = -1.8;
    t.depthPerPass = 0.6;
    t.travelZ = 2.0;
    t.feedrate = 100.0;
    t.feedrateZ = 100.0;
    t.feedrateRapid = 1500.0;
    t.spindleSpeed = 1000.0;
    t.dwellTime = 1.0;
    t.endZ = 19.0;
    t.toolchangeZ = 19.0;
    t.extracutLength = 0.1;
    t.spindleDir = "CW";
    t.multidepth = true;
    t.dwell = false;
    t.extracut = false;
    t.endXY = std::array<double, 2>{0.0, 0.0};
    t.toolchangeXY = std::array<double, 2>{0.0, 0.0};
    t.tolerance = 0.0;
    t.segX = 0.0;
    t.segY = 0.0;
    return s;
}
inline flatcam::ShapeList geometry1() {  // multidepth_boundary_float
    return {flatcam::Shape::fromWkt("LINESTRING (0 0, 10 0, 10 5)")};
}
inline flatcam::GeometryJobSettings settings1() {
    flatcam::GeometryJobSettings s;
    s.units = "MM";
    s.preprocessor = "default";
    s.order = flatcam::DrillOrder::FileOrder;
    s.tool = 1;
    s.toolchange = true;
    flatcam::GeometryToolSettings& t = s.toolSettings;
    t.toolDia = 1.2;
    t.offsetKind = "Path";
    t.offsetValue = 0.0;
    t.cutZ = -1.0;
    t.depthPerPass = 0.1;
    t.travelZ = 2.0;
    t.feedrate = 100.0;
    t.feedrateZ = 100.0;
    t.feedrateRapid = 1500.0;
    t.spindleSpeed = 1000.0;
    t.dwellTime = 1.0;
    t.endZ = 19.0;
    t.toolchangeZ = 19.0;
    t.extracutLength = 0.1;
    t.spindleDir = "CW";
    t.multidepth = true;
    t.dwell = false;
    t.extracut = false;
    t.endXY = std::array<double, 2>{0.0, 0.0};
    t.toolchangeXY = std::array<double, 2>{0.0, 0.0};
    t.tolerance = 0.0;
    t.segX = 0.0;
    t.segY = 0.0;
    return s;
}
inline flatcam::ShapeList geometry2() {  // multidepth_ring_and_hole
    return {flatcam::Shape::fromWkt("POLYGON ((0 0, 10 0, 10 10, 0 10, 0 0), (2 2, 4 2, 4 4, 2 4, 2 2))")};
}
inline flatcam::GeometryJobSettings settings2() {
    flatcam::GeometryJobSettings s;
    s.units = "MM";
    s.preprocessor = "default";
    s.order = flatcam::DrillOrder::FileOrder;
    s.tool = 1;
    s.toolchange = true;
    flatcam::GeometryToolSettings& t = s.toolSettings;
    t.toolDia = 1.2;
    t.offsetKind = "Path";
    t.offsetValue = 0.0;
    t.cutZ = -1.0;
    t.depthPerPass = 0.4;
    t.travelZ = 2.0;
    t.feedrate = 100.0;
    t.feedrateZ = 100.0;
    t.feedrateRapid = 1500.0;
    t.spindleSpeed = 1000.0;
    t.dwellTime = 1.0;
    t.endZ = 19.0;
    t.toolchangeZ = 19.0;
    t.extracutLength = 0.1;
    t.spindleDir = "CW";
    t.multidepth = true;
    t.dwell = false;
    t.extracut = false;
    t.endXY = std::array<double, 2>{0.0, 0.0};
    t.toolchangeXY = std::array<double, 2>{0.0, 0.0};
    t.tolerance = 0.0;
    t.segX = 0.0;
    t.segY = 0.0;
    return s;
}
inline flatcam::ShapeList geometry3() {  // multidepth_closed_linestring
    return {flatcam::Shape::fromWkt("LINESTRING (0 0, 6 0, 6 6, 0 6, 0 0)")};
}
inline flatcam::GeometryJobSettings settings3() {
    flatcam::GeometryJobSettings s;
    s.units = "MM";
    s.preprocessor = "default";
    s.order = flatcam::DrillOrder::FileOrder;
    s.tool = 1;
    s.toolchange = true;
    flatcam::GeometryToolSettings& t = s.toolSettings;
    t.toolDia = 1.2;
    t.offsetKind = "Path";
    t.offsetValue = 0.0;
    t.cutZ = -1.5;
    t.depthPerPass = 0.5;
    t.travelZ = 2.0;
    t.feedrate = 100.0;
    t.feedrateZ = 100.0;
    t.feedrateRapid = 1500.0;
    t.spindleSpeed = 1000.0;
    t.dwellTime = 1.0;
    t.endZ = 19.0;
    t.toolchangeZ = 19.0;
    t.extracutLength = 0.1;
    t.spindleDir = "CW";
    t.multidepth = true;
    t.dwell = false;
    t.extracut = false;
    t.endXY = std::array<double, 2>{0.0, 0.0};
    t.toolchangeXY = std::array<double, 2>{0.0, 0.0};
    t.tolerance = 0.0;
    t.segX = 0.0;
    t.segY = 0.0;
    return s;
}
inline flatcam::ShapeList geometry4() {  // multidepth_points
    return {flatcam::Shape::fromWkt("POINT (3 4)"), flatcam::Shape::fromWkt("POINT (5 6)")};
}
inline flatcam::GeometryJobSettings settings4() {
    flatcam::GeometryJobSettings s;
    s.units = "MM";
    s.preprocessor = "default";
    s.order = flatcam::DrillOrder::FileOrder;
    s.tool = 1;
    s.toolchange = true;
    flatcam::GeometryToolSettings& t = s.toolSettings;
    t.toolDia = 1.2;
    t.offsetKind = "Path";
    t.offsetValue = 0.0;
    t.cutZ = -2.0;
    t.depthPerPass = 0.5;
    t.travelZ = 2.0;
    t.feedrate = 100.0;
    t.feedrateZ = 100.0;
    t.feedrateRapid = 1500.0;
    t.spindleSpeed = 1000.0;
    t.dwellTime = 1.0;
    t.endZ = 19.0;
    t.toolchangeZ = 19.0;
    t.extracutLength = 0.1;
    t.spindleDir = "CW";
    t.multidepth = true;
    t.dwell = false;
    t.extracut = false;
    t.endXY = std::array<double, 2>{0.0, 0.0};
    t.toolchangeXY = std::array<double, 2>{0.0, 0.0};
    t.tolerance = 0.0;
    t.segX = 0.0;
    t.segY = 0.0;
    return s;
}
inline flatcam::ShapeList geometry5() {  // single_points
    return {flatcam::Shape::fromWkt("POINT (3 4)"), flatcam::Shape::fromWkt("POINT (5 6)")};
}
inline flatcam::GeometryJobSettings settings5() {
    flatcam::GeometryJobSettings s;
    s.units = "MM";
    s.preprocessor = "default";
    s.order = flatcam::DrillOrder::FileOrder;
    s.tool = 1;
    s.toolchange = true;
    flatcam::GeometryToolSettings& t = s.toolSettings;
    t.toolDia = 1.2;
    t.offsetKind = "Path";
    t.offsetValue = 0.0;
    t.cutZ = -1.2;
    t.depthPerPass = 0.6;
    t.travelZ = 2.0;
    t.feedrate = 100.0;
    t.feedrateZ = 100.0;
    t.feedrateRapid = 1500.0;
    t.spindleSpeed = 1000.0;
    t.dwellTime = 1.0;
    t.endZ = 19.0;
    t.toolchangeZ = 19.0;
    t.extracutLength = 0.1;
    t.spindleDir = "CW";
    t.multidepth = false;
    t.dwell = false;
    t.extracut = false;
    t.endXY = std::array<double, 2>{0.0, 0.0};
    t.toolchangeXY = std::array<double, 2>{0.0, 0.0};
    t.tolerance = 0.0;
    t.segX = 0.0;
    t.segY = 0.0;
    return s;
}
inline flatcam::ShapeList geometry6() {  // single_two_lines
    return {flatcam::Shape::fromWkt("LINESTRING (0 0, 10 0, 10 5)"), flatcam::Shape::fromWkt("LINESTRING (20 0, 25 5)")};
}
inline flatcam::GeometryJobSettings settings6() {
    flatcam::GeometryJobSettings s;
    s.units = "MM";
    s.preprocessor = "default";
    s.order = flatcam::DrillOrder::FileOrder;
    s.tool = 1;
    s.toolchange = true;
    flatcam::GeometryToolSettings& t = s.toolSettings;
    t.toolDia = 1.2;
    t.offsetKind = "Path";
    t.offsetValue = 0.0;
    t.cutZ = -0.5;
    t.depthPerPass = 0.6;
    t.travelZ = 2.0;
    t.feedrate = 100.0;
    t.feedrateZ = 100.0;
    t.feedrateRapid = 1500.0;
    t.spindleSpeed = 1000.0;
    t.dwellTime = 1.0;
    t.endZ = 19.0;
    t.toolchangeZ = 19.0;
    t.extracutLength = 0.1;
    t.spindleDir = "CW";
    t.multidepth = false;
    t.dwell = false;
    t.extracut = false;
    t.endXY = std::array<double, 2>{0.0, 0.0};
    t.toolchangeXY = std::array<double, 2>{0.0, 0.0};
    t.tolerance = 0.0;
    t.segX = 0.0;
    t.segY = 0.0;
    return s;
}
inline flatcam::ShapeList geometry7() {  // extracut_default_length
    return {flatcam::Shape::fromWkt("POLYGON ((0 0, 10 0, 10 10, 0 10, 0 0))")};
}
inline flatcam::GeometryJobSettings settings7() {
    flatcam::GeometryJobSettings s;
    s.units = "MM";
    s.preprocessor = "default";
    s.order = flatcam::DrillOrder::FileOrder;
    s.tool = 1;
    s.toolchange = true;
    flatcam::GeometryToolSettings& t = s.toolSettings;
    t.toolDia = 1.2;
    t.offsetKind = "Path";
    t.offsetValue = 0.0;
    t.cutZ = -1.0;
    t.depthPerPass = 0.6;
    t.travelZ = 2.0;
    t.feedrate = 100.0;
    t.feedrateZ = 100.0;
    t.feedrateRapid = 1500.0;
    t.spindleSpeed = 1000.0;
    t.dwellTime = 1.0;
    t.endZ = 19.0;
    t.toolchangeZ = 19.0;
    t.extracutLength = 0.5;
    t.spindleDir = "CW";
    t.multidepth = false;
    t.dwell = false;
    t.extracut = true;
    t.endXY = std::array<double, 2>{0.0, 0.0};
    t.toolchangeXY = std::array<double, 2>{0.0, 0.0};
    t.tolerance = 0.0;
    t.segX = 0.0;
    t.segY = 0.0;
    return s;
}
inline flatcam::ShapeList geometry8() {  // extracut_zero_length
    return {flatcam::Shape::fromWkt("POLYGON ((0 0, 10 0, 10 10, 0 10, 0 0))")};
}
inline flatcam::GeometryJobSettings settings8() {
    flatcam::GeometryJobSettings s;
    s.units = "MM";
    s.preprocessor = "default";
    s.order = flatcam::DrillOrder::FileOrder;
    s.tool = 1;
    s.toolchange = true;
    flatcam::GeometryToolSettings& t = s.toolSettings;
    t.toolDia = 1.2;
    t.offsetKind = "Path";
    t.offsetValue = 0.0;
    t.cutZ = -1.0;
    t.depthPerPass = 0.6;
    t.travelZ = 2.0;
    t.feedrate = 100.0;
    t.feedrateZ = 100.0;
    t.feedrateRapid = 1500.0;
    t.spindleSpeed = 1000.0;
    t.dwellTime = 1.0;
    t.endZ = 19.0;
    t.toolchangeZ = 19.0;
    t.extracutLength = 0.0;
    t.spindleDir = "CW";
    t.multidepth = false;
    t.dwell = false;
    t.extracut = true;
    t.endXY = std::array<double, 2>{0.0, 0.0};
    t.toolchangeXY = std::array<double, 2>{0.0, 0.0};
    t.tolerance = 0.0;
    t.segX = 0.0;
    t.segY = 0.0;
    return s;
}
inline flatcam::ShapeList geometry9() {  // extracut_longer_than_ring
    return {flatcam::Shape::fromWkt("POLYGON ((0 0, 10 0, 10 10, 0 10, 0 0))")};
}
inline flatcam::GeometryJobSettings settings9() {
    flatcam::GeometryJobSettings s;
    s.units = "MM";
    s.preprocessor = "default";
    s.order = flatcam::DrillOrder::FileOrder;
    s.tool = 1;
    s.toolchange = true;
    flatcam::GeometryToolSettings& t = s.toolSettings;
    t.toolDia = 1.2;
    t.offsetKind = "Path";
    t.offsetValue = 0.0;
    t.cutZ = -1.0;
    t.depthPerPass = 0.6;
    t.travelZ = 2.0;
    t.feedrate = 100.0;
    t.feedrateZ = 100.0;
    t.feedrateRapid = 1500.0;
    t.spindleSpeed = 1000.0;
    t.dwellTime = 1.0;
    t.endZ = 19.0;
    t.toolchangeZ = 19.0;
    t.extracutLength = 500.0;
    t.spindleDir = "CW";
    t.multidepth = false;
    t.dwell = false;
    t.extracut = true;
    t.endXY = std::array<double, 2>{0.0, 0.0};
    t.toolchangeXY = std::array<double, 2>{0.0, 0.0};
    t.tolerance = 0.0;
    t.segX = 0.0;
    t.segY = 0.0;
    return s;
}
inline flatcam::ShapeList geometry10() {  // extracut_with_multidepth
    return {flatcam::Shape::fromWkt("POLYGON ((0 0, 10 0, 10 10, 0 10, 0 0))")};
}
inline flatcam::GeometryJobSettings settings10() {
    flatcam::GeometryJobSettings s;
    s.units = "MM";
    s.preprocessor = "default";
    s.order = flatcam::DrillOrder::FileOrder;
    s.tool = 1;
    s.toolchange = true;
    flatcam::GeometryToolSettings& t = s.toolSettings;
    t.toolDia = 1.2;
    t.offsetKind = "Path";
    t.offsetValue = 0.0;
    t.cutZ = -1.0;
    t.depthPerPass = 0.5;
    t.travelZ = 2.0;
    t.feedrate = 100.0;
    t.feedrateZ = 100.0;
    t.feedrateRapid = 1500.0;
    t.spindleSpeed = 1000.0;
    t.dwellTime = 1.0;
    t.endZ = 19.0;
    t.toolchangeZ = 19.0;
    t.extracutLength = 1.0;
    t.spindleDir = "CW";
    t.multidepth = true;
    t.dwell = false;
    t.extracut = true;
    t.endXY = std::array<double, 2>{0.0, 0.0};
    t.toolchangeXY = std::array<double, 2>{0.0, 0.0};
    t.tolerance = 0.0;
    t.segX = 0.0;
    t.segY = 0.0;
    return s;
}
inline flatcam::ShapeList geometry11() {  // extracut_ignored_for_open_line
    return {flatcam::Shape::fromWkt("LINESTRING (0 0, 10 0, 10 5)")};
}
inline flatcam::GeometryJobSettings settings11() {
    flatcam::GeometryJobSettings s;
    s.units = "MM";
    s.preprocessor = "default";
    s.order = flatcam::DrillOrder::FileOrder;
    s.tool = 1;
    s.toolchange = true;
    flatcam::GeometryToolSettings& t = s.toolSettings;
    t.toolDia = 1.2;
    t.offsetKind = "Path";
    t.offsetValue = 0.0;
    t.cutZ = -1.0;
    t.depthPerPass = 0.6;
    t.travelZ = 2.0;
    t.feedrate = 100.0;
    t.feedrateZ = 100.0;
    t.feedrateRapid = 1500.0;
    t.spindleSpeed = 1000.0;
    t.dwellTime = 1.0;
    t.endZ = 19.0;
    t.toolchangeZ = 19.0;
    t.extracutLength = 1.0;
    t.spindleDir = "CW";
    t.multidepth = false;
    t.dwell = false;
    t.extracut = true;
    t.endXY = std::array<double, 2>{0.0, 0.0};
    t.toolchangeXY = std::array<double, 2>{0.0, 0.0};
    t.tolerance = 0.0;
    t.segX = 0.0;
    t.segY = 0.0;
    return s;
}
inline flatcam::ShapeList geometry12() {  // tolerance_simplifies
    return {flatcam::Shape::fromWkt("LINESTRING (0 0, 1 0.01, 2 -0.01, 3 0.02, 4 0, 5 0.01, 6 -0.02, 7 0, 8 0.01, 9 0, 10 0)")};
}
inline flatcam::GeometryJobSettings settings12() {
    flatcam::GeometryJobSettings s;
    s.units = "MM";
    s.preprocessor = "default";
    s.order = flatcam::DrillOrder::FileOrder;
    s.tool = 1;
    s.toolchange = true;
    flatcam::GeometryToolSettings& t = s.toolSettings;
    t.toolDia = 1.2;
    t.offsetKind = "Path";
    t.offsetValue = 0.0;
    t.cutZ = -0.5;
    t.depthPerPass = 0.6;
    t.travelZ = 2.0;
    t.feedrate = 100.0;
    t.feedrateZ = 100.0;
    t.feedrateRapid = 1500.0;
    t.spindleSpeed = 1000.0;
    t.dwellTime = 1.0;
    t.endZ = 19.0;
    t.toolchangeZ = 19.0;
    t.extracutLength = 0.1;
    t.spindleDir = "CW";
    t.multidepth = false;
    t.dwell = false;
    t.extracut = false;
    t.endXY = std::array<double, 2>{0.0, 0.0};
    t.toolchangeXY = std::array<double, 2>{0.0, 0.0};
    t.tolerance = 0.05;
    t.segX = 0.0;
    t.segY = 0.0;
    return s;
}
inline flatcam::ShapeList geometry13() {  // segmentation
    return {flatcam::Shape::fromWkt("LINESTRING (0 0, 9 0, 9 7)")};
}
inline flatcam::GeometryJobSettings settings13() {
    flatcam::GeometryJobSettings s;
    s.units = "MM";
    s.preprocessor = "default";
    s.order = flatcam::DrillOrder::FileOrder;
    s.tool = 1;
    s.toolchange = true;
    flatcam::GeometryToolSettings& t = s.toolSettings;
    t.toolDia = 1.2;
    t.offsetKind = "Path";
    t.offsetValue = 0.0;
    t.cutZ = -0.5;
    t.depthPerPass = 0.6;
    t.travelZ = 2.0;
    t.feedrate = 100.0;
    t.feedrateZ = 100.0;
    t.feedrateRapid = 1500.0;
    t.spindleSpeed = 1000.0;
    t.dwellTime = 1.0;
    t.endZ = 19.0;
    t.toolchangeZ = 19.0;
    t.extracutLength = 0.1;
    t.spindleDir = "CW";
    t.multidepth = false;
    t.dwell = false;
    t.extracut = false;
    t.endXY = std::array<double, 2>{0.0, 0.0};
    t.toolchangeXY = std::array<double, 2>{0.0, 0.0};
    t.tolerance = 0.0;
    t.segX = 2.0;
    t.segY = 1.5;
    return s;
}
inline flatcam::ShapeList geometry14() {  // segmentation_x_only
    return {flatcam::Shape::fromWkt("LINESTRING (0 0, 9 0, 9 7)")};
}
inline flatcam::GeometryJobSettings settings14() {
    flatcam::GeometryJobSettings s;
    s.units = "MM";
    s.preprocessor = "default";
    s.order = flatcam::DrillOrder::FileOrder;
    s.tool = 1;
    s.toolchange = true;
    flatcam::GeometryToolSettings& t = s.toolSettings;
    t.toolDia = 1.2;
    t.offsetKind = "Path";
    t.offsetValue = 0.0;
    t.cutZ = -0.5;
    t.depthPerPass = 0.6;
    t.travelZ = 2.0;
    t.feedrate = 100.0;
    t.feedrateZ = 100.0;
    t.feedrateRapid = 1500.0;
    t.spindleSpeed = 1000.0;
    t.dwellTime = 1.0;
    t.endZ = 19.0;
    t.toolchangeZ = 19.0;
    t.extracutLength = 0.1;
    t.spindleDir = "CW";
    t.multidepth = false;
    t.dwell = false;
    t.extracut = false;
    t.endXY = std::array<double, 2>{0.0, 0.0};
    t.toolchangeXY = std::array<double, 2>{0.0, 0.0};
    t.tolerance = 0.0;
    t.segX = 2.5;
    t.segY = 0.0;
    return s;
}
inline flatcam::ShapeList geometry15() {  // no_toolchange_with_startz
    return {flatcam::Shape::fromWkt("LINESTRING (0 0, 10 0, 10 5)")};
}
inline flatcam::GeometryJobSettings settings15() {
    flatcam::GeometryJobSettings s;
    s.units = "MM";
    s.preprocessor = "default";
    s.order = flatcam::DrillOrder::FileOrder;
    s.tool = 1;
    s.toolchange = false;
    flatcam::GeometryToolSettings& t = s.toolSettings;
    t.toolDia = 1.2;
    t.offsetKind = "Path";
    t.offsetValue = 0.0;
    t.cutZ = -0.5;
    t.depthPerPass = 0.6;
    t.travelZ = 2.0;
    t.feedrate = 100.0;
    t.feedrateZ = 100.0;
    t.feedrateRapid = 1500.0;
    t.spindleSpeed = 1000.0;
    t.dwellTime = 2.0;
    t.endZ = 19.0;
    t.toolchangeZ = 19.0;
    t.extracutLength = 0.1;
    t.spindleDir = "CW";
    t.multidepth = false;
    t.dwell = true;
    t.extracut = false;
    t.startZ = 1.5;
    t.endXY = std::array<double, 2>{0.0, 0.0};
    t.toolchangeXY = std::array<double, 2>{0.0, 0.0};
    t.tolerance = 0.0;
    t.segX = 0.0;
    t.segY = 0.0;
    return s;
}
inline flatcam::ShapeList geometry16() {  // laser_single_pass
    return {flatcam::Shape::fromWkt("LINESTRING (0 0, 10 0, 10 5)")};
}
inline flatcam::GeometryJobSettings settings16() {
    flatcam::GeometryJobSettings s;
    s.units = "MM";
    s.preprocessor = "GRBL_laser";
    s.order = flatcam::DrillOrder::FileOrder;
    s.tool = 1;
    s.toolchange = true;
    flatcam::GeometryToolSettings& t = s.toolSettings;
    t.toolDia = 1.2;
    t.offsetKind = "Path";
    t.offsetValue = 0.0;
    t.cutZ = -0.5;
    t.depthPerPass = 0.6;
    t.travelZ = 2.0;
    t.feedrate = 500.0;
    t.feedrateZ = 100.0;
    t.feedrateRapid = 1500.0;
    t.spindleSpeed = 300.0;
    t.dwellTime = 1.0;
    t.endZ = 19.0;
    t.toolchangeZ = 19.0;
    t.extracutLength = 0.1;
    t.spindleDir = "CW";
    t.multidepth = false;
    t.dwell = false;
    t.extracut = false;
    t.endXY = std::array<double, 2>{0.0, 0.0};
    t.toolchangeXY = std::array<double, 2>{0.0, 0.0};
    t.tolerance = 0.0;
    t.segX = 0.0;
    t.segY = 0.0;
    return s;
}
inline flatcam::ShapeList geometry17() {  // laser_no_toolchange
    return {flatcam::Shape::fromWkt("LINESTRING (0 0, 10 0, 10 5)")};
}
inline flatcam::GeometryJobSettings settings17() {
    flatcam::GeometryJobSettings s;
    s.units = "MM";
    s.preprocessor = "GRBL_laser";
    s.order = flatcam::DrillOrder::FileOrder;
    s.tool = 1;
    s.toolchange = false;
    flatcam::GeometryToolSettings& t = s.toolSettings;
    t.toolDia = 1.2;
    t.offsetKind = "Path";
    t.offsetValue = 0.0;
    t.cutZ = -0.5;
    t.depthPerPass = 0.6;
    t.travelZ = 2.0;
    t.feedrate = 500.0;
    t.feedrateZ = 100.0;
    t.feedrateRapid = 1500.0;
    t.spindleSpeed = 300.0;
    t.dwellTime = 1.0;
    t.endZ = 19.0;
    t.toolchangeZ = 19.0;
    t.extracutLength = 0.1;
    t.spindleDir = "CW";
    t.multidepth = false;
    t.dwell = false;
    t.extracut = false;
    t.endXY = std::array<double, 2>{0.0, 0.0};
    t.toolchangeXY = std::array<double, 2>{0.0, 0.0};
    t.tolerance = 0.0;
    t.segX = 0.0;
    t.segY = 0.0;
    return s;
}
inline flatcam::ShapeList geometry18() {  // inches
    return {flatcam::Shape::fromWkt("LINESTRING (0 0, 10 0, 10 5)")};
}
inline flatcam::GeometryJobSettings settings18() {
    flatcam::GeometryJobSettings s;
    s.units = "IN";
    s.preprocessor = "default";
    s.order = flatcam::DrillOrder::FileOrder;
    s.tool = 1;
    s.toolchange = true;
    flatcam::GeometryToolSettings& t = s.toolSettings;
    t.toolDia = 1.2;
    t.offsetKind = "Path";
    t.offsetValue = 0.0;
    t.cutZ = -0.07;
    t.depthPerPass = 0.02;
    t.travelZ = 0.08;
    t.feedrate = 100.0;
    t.feedrateZ = 100.0;
    t.feedrateRapid = 1500.0;
    t.spindleSpeed = 1000.0;
    t.dwellTime = 1.0;
    t.endZ = 19.0;
    t.toolchangeZ = 19.0;
    t.extracutLength = 0.1;
    t.spindleDir = "CW";
    t.multidepth = true;
    t.dwell = false;
    t.extracut = false;
    t.endXY = std::array<double, 2>{0.0, 0.0};
    t.toolchangeXY = std::array<double, 2>{0.0, 0.0};
    t.tolerance = 0.0;
    t.segX = 0.0;
    t.segY = 0.0;
    return s;
}
inline flatcam::ShapeList geometry19() {  // offset_out_square
    return {flatcam::Shape::fromWkt("POLYGON ((0 0, 10 0, 10 10, 0 10, 0 0))")};
}
inline flatcam::GeometryJobSettings settings19() {
    flatcam::GeometryJobSettings s;
    s.units = "MM";
    s.preprocessor = "default";
    s.order = flatcam::DrillOrder::FileOrder;
    s.tool = 1;
    s.toolchange = true;
    flatcam::GeometryToolSettings& t = s.toolSettings;
    t.toolDia = 1.2;
    t.offsetKind = "out";
    t.offsetValue = 0.0;
    t.cutZ = -1.0;
    t.depthPerPass = 0.6;
    t.travelZ = 2.0;
    t.feedrate = 100.0;
    t.feedrateZ = 100.0;
    t.feedrateRapid = 1500.0;
    t.spindleSpeed = 1000.0;
    t.dwellTime = 1.0;
    t.endZ = 19.0;
    t.toolchangeZ = 19.0;
    t.extracutLength = 0.1;
    t.spindleDir = "CW";
    t.multidepth = false;
    t.dwell = false;
    t.extracut = false;
    t.endXY = std::array<double, 2>{0.0, 0.0};
    t.toolchangeXY = std::array<double, 2>{0.0, 0.0};
    t.tolerance = 0.0;
    t.segX = 0.0;
    t.segY = 0.0;
    return s;
}
inline flatcam::ShapeList geometry20() {  // offset_in_square_with_hole
    return {flatcam::Shape::fromWkt("POLYGON ((0 0, 10 0, 10 10, 0 10, 0 0), (2 2, 4 2, 4 4, 2 4, 2 2))")};
}
inline flatcam::GeometryJobSettings settings20() {
    flatcam::GeometryJobSettings s;
    s.units = "MM";
    s.preprocessor = "default";
    s.order = flatcam::DrillOrder::FileOrder;
    s.tool = 1;
    s.toolchange = true;
    flatcam::GeometryToolSettings& t = s.toolSettings;
    t.toolDia = 1.2;
    t.offsetKind = "in";
    t.offsetValue = 0.0;
    t.cutZ = -1.0;
    t.depthPerPass = 0.6;
    t.travelZ = 2.0;
    t.feedrate = 100.0;
    t.feedrateZ = 100.0;
    t.feedrateRapid = 1500.0;
    t.spindleSpeed = 1000.0;
    t.dwellTime = 1.0;
    t.endZ = 19.0;
    t.toolchangeZ = 19.0;
    t.extracutLength = 0.1;
    t.spindleDir = "CW";
    t.multidepth = false;
    t.dwell = false;
    t.extracut = false;
    t.endXY = std::array<double, 2>{0.0, 0.0};
    t.toolchangeXY = std::array<double, 2>{0.0, 0.0};
    t.tolerance = 0.0;
    t.segX = 0.0;
    t.segY = 0.0;
    return s;
}
inline flatcam::ShapeList geometry21() {  // offset_custom_open_line
    return {flatcam::Shape::fromWkt("LINESTRING (0 0, 10 0, 10 5)")};
}
inline flatcam::GeometryJobSettings settings21() {
    flatcam::GeometryJobSettings s;
    s.units = "MM";
    s.preprocessor = "default";
    s.order = flatcam::DrillOrder::FileOrder;
    s.tool = 1;
    s.toolchange = true;
    flatcam::GeometryToolSettings& t = s.toolSettings;
    t.toolDia = 1.2;
    t.offsetKind = "custom";
    t.offsetValue = 0.3;
    t.cutZ = -1.0;
    t.depthPerPass = 0.6;
    t.travelZ = 2.0;
    t.feedrate = 100.0;
    t.feedrateZ = 100.0;
    t.feedrateRapid = 1500.0;
    t.spindleSpeed = 1000.0;
    t.dwellTime = 1.0;
    t.endZ = 19.0;
    t.toolchangeZ = 19.0;
    t.extracutLength = 0.1;
    t.spindleDir = "CW";
    t.multidepth = false;
    t.dwell = false;
    t.extracut = false;
    t.endXY = std::array<double, 2>{0.0, 0.0};
    t.toolchangeXY = std::array<double, 2>{0.0, 0.0};
    t.tolerance = 0.0;
    t.segX = 0.0;
    t.segY = 0.0;
    return s;
}
inline flatcam::ShapeList geometry22() {  // offset_out_closed_linestring
    return {flatcam::Shape::fromWkt("LINESTRING (0 0, 6 0, 6 6, 0 6, 0 0)")};
}
inline flatcam::GeometryJobSettings settings22() {
    flatcam::GeometryJobSettings s;
    s.units = "MM";
    s.preprocessor = "default";
    s.order = flatcam::DrillOrder::FileOrder;
    s.tool = 1;
    s.toolchange = true;
    flatcam::GeometryToolSettings& t = s.toolSettings;
    t.toolDia = 1.2;
    t.offsetKind = "out";
    t.offsetValue = 0.0;
    t.cutZ = -1.0;
    t.depthPerPass = 0.6;
    t.travelZ = 2.0;
    t.feedrate = 100.0;
    t.feedrateZ = 100.0;
    t.feedrateRapid = 1500.0;
    t.spindleSpeed = 1000.0;
    t.dwellTime = 1.0;
    t.endZ = 19.0;
    t.toolchangeZ = 19.0;
    t.extracutLength = 0.1;
    t.spindleDir = "CW";
    t.multidepth = false;
    t.dwell = false;
    t.extracut = false;
    t.endXY = std::array<double, 2>{0.0, 0.0};
    t.toolchangeXY = std::array<double, 2>{0.0, 0.0};
    t.tolerance = 0.0;
    t.segX = 0.0;
    t.segY = 0.0;
    return s;
}
inline flatcam::ShapeList geometry23() {  // offset_out_two_shapes
    return {flatcam::Shape::fromWkt("POLYGON ((0 0, 10 0, 10 10, 0 10, 0 0))"), flatcam::Shape::fromWkt("POLYGON ((20 0, 28 0, 24 6, 20 0))")};
}
inline flatcam::GeometryJobSettings settings23() {
    flatcam::GeometryJobSettings s;
    s.units = "MM";
    s.preprocessor = "default";
    s.order = flatcam::DrillOrder::FileOrder;
    s.tool = 1;
    s.toolchange = true;
    flatcam::GeometryToolSettings& t = s.toolSettings;
    t.toolDia = 1.2;
    t.offsetKind = "out";
    t.offsetValue = 0.0;
    t.cutZ = -1.0;
    t.depthPerPass = 0.6;
    t.travelZ = 2.0;
    t.feedrate = 100.0;
    t.feedrateZ = 100.0;
    t.feedrateRapid = 1500.0;
    t.spindleSpeed = 1000.0;
    t.dwellTime = 1.0;
    t.endZ = 19.0;
    t.toolchangeZ = 19.0;
    t.extracutLength = 0.1;
    t.spindleDir = "CW";
    t.multidepth = false;
    t.dwell = false;
    t.extracut = false;
    t.endXY = std::array<double, 2>{0.0, 0.0};
    t.toolchangeXY = std::array<double, 2>{0.0, 0.0};
    t.tolerance = 0.0;
    t.segX = 0.0;
    t.segY = 0.0;
    return s;
}
inline flatcam::ShapeList geometry24() {  // offset_out_multidepth
    return {flatcam::Shape::fromWkt("POLYGON ((0 0, 10 0, 10 10, 0 10, 0 0))")};
}
inline flatcam::GeometryJobSettings settings24() {
    flatcam::GeometryJobSettings s;
    s.units = "MM";
    s.preprocessor = "default";
    s.order = flatcam::DrillOrder::FileOrder;
    s.tool = 1;
    s.toolchange = true;
    flatcam::GeometryToolSettings& t = s.toolSettings;
    t.toolDia = 1.2;
    t.offsetKind = "out";
    t.offsetValue = 0.0;
    t.cutZ = -1.0;
    t.depthPerPass = 0.5;
    t.travelZ = 2.0;
    t.feedrate = 100.0;
    t.feedrateZ = 100.0;
    t.feedrateRapid = 1500.0;
    t.spindleSpeed = 1000.0;
    t.dwellTime = 1.0;
    t.endZ = 19.0;
    t.toolchangeZ = 19.0;
    t.extracutLength = 0.1;
    t.spindleDir = "CW";
    t.multidepth = true;
    t.dwell = false;
    t.extracut = false;
    t.endXY = std::array<double, 2>{0.0, 0.0};
    t.toolchangeXY = std::array<double, 2>{0.0, 0.0};
    t.tolerance = 0.0;
    t.segX = 0.0;
    t.segY = 0.0;
    return s;
}
inline flatcam::ShapeList geometry25() {  // offset_custom_negative_ring
    return {flatcam::Shape::fromWkt("POLYGON ((0 0, 10 0, 10 10, 0 10, 0 0))")};
}
inline flatcam::GeometryJobSettings settings25() {
    flatcam::GeometryJobSettings s;
    s.units = "MM";
    s.preprocessor = "default";
    s.order = flatcam::DrillOrder::FileOrder;
    s.tool = 1;
    s.toolchange = true;
    flatcam::GeometryToolSettings& t = s.toolSettings;
    t.toolDia = 1.2;
    t.offsetKind = "custom";
    t.offsetValue = -0.4;
    t.cutZ = -1.0;
    t.depthPerPass = 0.6;
    t.travelZ = 2.0;
    t.feedrate = 100.0;
    t.feedrateZ = 100.0;
    t.feedrateRapid = 1500.0;
    t.spindleSpeed = 1000.0;
    t.dwellTime = 1.0;
    t.endZ = 19.0;
    t.toolchangeZ = 19.0;
    t.extracutLength = 0.1;
    t.spindleDir = "CW";
    t.multidepth = false;
    t.dwell = false;
    t.extracut = false;
    t.endXY = std::array<double, 2>{0.0, 0.0};
    t.toolchangeXY = std::array<double, 2>{0.0, 0.0};
    t.tolerance = 0.0;
    t.segX = 0.0;
    t.segY = 0.0;
    return s;
}
inline flatcam::ShapeList geometry26() {  // pp_Berta_CNC
    return {flatcam::Shape::fromWkt("LINESTRING (0 0, 10 0, 10 5)"), flatcam::Shape::fromWkt("POINT (3 4)")};
}
inline flatcam::GeometryJobSettings settings26() {
    flatcam::GeometryJobSettings s;
    s.units = "MM";
    s.preprocessor = "Berta_CNC";
    s.order = flatcam::DrillOrder::FileOrder;
    s.tool = 1;
    s.toolchange = true;
    flatcam::GeometryToolSettings& t = s.toolSettings;
    t.toolDia = 1.2;
    t.offsetKind = "Path";
    t.offsetValue = 0.0;
    t.cutZ = -0.5;
    t.depthPerPass = 0.6;
    t.travelZ = 2.0;
    t.feedrate = 100.0;
    t.feedrateZ = 100.0;
    t.feedrateRapid = 1500.0;
    t.spindleSpeed = 1000.0;
    t.dwellTime = 1.5;
    t.endZ = 19.0;
    t.toolchangeZ = 19.0;
    t.extracutLength = 0.1;
    t.spindleDir = "CW";
    t.multidepth = false;
    t.dwell = true;
    t.extracut = false;
    t.endXY = std::array<double, 2>{0.0, 0.0};
    t.toolchangeXY = std::array<double, 2>{0.0, 0.0};
    t.tolerance = 0.0;
    t.segX = 0.0;
    t.segY = 0.0;
    return s;
}
inline flatcam::ShapeList geometry27() {  // pp_Berta_CNC_no_toolchange
    return {flatcam::Shape::fromWkt("LINESTRING (0 0, 10 0, 10 5)")};
}
inline flatcam::GeometryJobSettings settings27() {
    flatcam::GeometryJobSettings s;
    s.units = "MM";
    s.preprocessor = "Berta_CNC";
    s.order = flatcam::DrillOrder::FileOrder;
    s.tool = 1;
    s.toolchange = false;
    flatcam::GeometryToolSettings& t = s.toolSettings;
    t.toolDia = 1.2;
    t.offsetKind = "Path";
    t.offsetValue = 0.0;
    t.cutZ = -0.5;
    t.depthPerPass = 0.6;
    t.travelZ = 2.0;
    t.feedrate = 100.0;
    t.feedrateZ = 100.0;
    t.feedrateRapid = 1500.0;
    t.spindleSpeed = 1000.0;
    t.dwellTime = 1.0;
    t.endZ = 19.0;
    t.toolchangeZ = 19.0;
    t.extracutLength = 0.1;
    t.spindleDir = "CW";
    t.multidepth = false;
    t.dwell = false;
    t.extracut = false;
    t.startZ = 1.0;
    t.endXY = std::array<double, 2>{0.0, 0.0};
    t.toolchangeXY = std::array<double, 2>{0.0, 0.0};
    t.tolerance = 0.0;
    t.segX = 0.0;
    t.segY = 0.0;
    return s;
}
inline flatcam::ShapeList geometry28() {  // pp_Default_no_M6
    return {flatcam::Shape::fromWkt("LINESTRING (0 0, 10 0, 10 5)"), flatcam::Shape::fromWkt("POINT (3 4)")};
}
inline flatcam::GeometryJobSettings settings28() {
    flatcam::GeometryJobSettings s;
    s.units = "MM";
    s.preprocessor = "Default_no_M6";
    s.order = flatcam::DrillOrder::FileOrder;
    s.tool = 1;
    s.toolchange = true;
    flatcam::GeometryToolSettings& t = s.toolSettings;
    t.toolDia = 1.2;
    t.offsetKind = "Path";
    t.offsetValue = 0.0;
    t.cutZ = -0.5;
    t.depthPerPass = 0.6;
    t.travelZ = 2.0;
    t.feedrate = 100.0;
    t.feedrateZ = 100.0;
    t.feedrateRapid = 1500.0;
    t.spindleSpeed = 1000.0;
    t.dwellTime = 1.5;
    t.endZ = 19.0;
    t.toolchangeZ = 19.0;
    t.extracutLength = 0.1;
    t.spindleDir = "CW";
    t.multidepth = false;
    t.dwell = true;
    t.extracut = false;
    t.endXY = std::array<double, 2>{0.0, 0.0};
    t.toolchangeXY = std::array<double, 2>{0.0, 0.0};
    t.tolerance = 0.0;
    t.segX = 0.0;
    t.segY = 0.0;
    return s;
}
inline flatcam::ShapeList geometry29() {  // pp_Default_no_M6_no_toolchange
    return {flatcam::Shape::fromWkt("LINESTRING (0 0, 10 0, 10 5)")};
}
inline flatcam::GeometryJobSettings settings29() {
    flatcam::GeometryJobSettings s;
    s.units = "MM";
    s.preprocessor = "Default_no_M6";
    s.order = flatcam::DrillOrder::FileOrder;
    s.tool = 1;
    s.toolchange = false;
    flatcam::GeometryToolSettings& t = s.toolSettings;
    t.toolDia = 1.2;
    t.offsetKind = "Path";
    t.offsetValue = 0.0;
    t.cutZ = -0.5;
    t.depthPerPass = 0.6;
    t.travelZ = 2.0;
    t.feedrate = 100.0;
    t.feedrateZ = 100.0;
    t.feedrateRapid = 1500.0;
    t.spindleSpeed = 1000.0;
    t.dwellTime = 1.0;
    t.endZ = 19.0;
    t.toolchangeZ = 19.0;
    t.extracutLength = 0.1;
    t.spindleDir = "CW";
    t.multidepth = false;
    t.dwell = false;
    t.extracut = false;
    t.startZ = 1.0;
    t.endXY = std::array<double, 2>{0.0, 0.0};
    t.toolchangeXY = std::array<double, 2>{0.0, 0.0};
    t.tolerance = 0.0;
    t.segX = 0.0;
    t.segY = 0.0;
    return s;
}
inline flatcam::ShapeList geometry30() {  // pp_grbl_11
    return {flatcam::Shape::fromWkt("LINESTRING (0 0, 10 0, 10 5)"), flatcam::Shape::fromWkt("POINT (3 4)")};
}
inline flatcam::GeometryJobSettings settings30() {
    flatcam::GeometryJobSettings s;
    s.units = "MM";
    s.preprocessor = "grbl_11";
    s.order = flatcam::DrillOrder::FileOrder;
    s.tool = 1;
    s.toolchange = true;
    flatcam::GeometryToolSettings& t = s.toolSettings;
    t.toolDia = 1.2;
    t.offsetKind = "Path";
    t.offsetValue = 0.0;
    t.cutZ = -0.5;
    t.depthPerPass = 0.6;
    t.travelZ = 2.0;
    t.feedrate = 100.0;
    t.feedrateZ = 100.0;
    t.feedrateRapid = 1500.0;
    t.spindleSpeed = 1000.0;
    t.dwellTime = 1.5;
    t.endZ = 19.0;
    t.toolchangeZ = 19.0;
    t.extracutLength = 0.1;
    t.spindleDir = "CW";
    t.multidepth = false;
    t.dwell = true;
    t.extracut = false;
    t.endXY = std::array<double, 2>{0.0, 0.0};
    t.toolchangeXY = std::array<double, 2>{0.0, 0.0};
    t.tolerance = 0.0;
    t.segX = 0.0;
    t.segY = 0.0;
    return s;
}
inline flatcam::ShapeList geometry31() {  // pp_grbl_11_no_toolchange
    return {flatcam::Shape::fromWkt("LINESTRING (0 0, 10 0, 10 5)")};
}
inline flatcam::GeometryJobSettings settings31() {
    flatcam::GeometryJobSettings s;
    s.units = "MM";
    s.preprocessor = "grbl_11";
    s.order = flatcam::DrillOrder::FileOrder;
    s.tool = 1;
    s.toolchange = false;
    flatcam::GeometryToolSettings& t = s.toolSettings;
    t.toolDia = 1.2;
    t.offsetKind = "Path";
    t.offsetValue = 0.0;
    t.cutZ = -0.5;
    t.depthPerPass = 0.6;
    t.travelZ = 2.0;
    t.feedrate = 100.0;
    t.feedrateZ = 100.0;
    t.feedrateRapid = 1500.0;
    t.spindleSpeed = 1000.0;
    t.dwellTime = 1.0;
    t.endZ = 19.0;
    t.toolchangeZ = 19.0;
    t.extracutLength = 0.1;
    t.spindleDir = "CW";
    t.multidepth = false;
    t.dwell = false;
    t.extracut = false;
    t.startZ = 1.0;
    t.endXY = std::array<double, 2>{0.0, 0.0};
    t.toolchangeXY = std::array<double, 2>{0.0, 0.0};
    t.tolerance = 0.0;
    t.segX = 0.0;
    t.segY = 0.0;
    return s;
}
inline flatcam::ShapeList geometry32() {  // pp_GRBL_11_no_M6
    return {flatcam::Shape::fromWkt("LINESTRING (0 0, 10 0, 10 5)"), flatcam::Shape::fromWkt("POINT (3 4)")};
}
inline flatcam::GeometryJobSettings settings32() {
    flatcam::GeometryJobSettings s;
    s.units = "MM";
    s.preprocessor = "GRBL_11_no_M6";
    s.order = flatcam::DrillOrder::FileOrder;
    s.tool = 1;
    s.toolchange = true;
    flatcam::GeometryToolSettings& t = s.toolSettings;
    t.toolDia = 1.2;
    t.offsetKind = "Path";
    t.offsetValue = 0.0;
    t.cutZ = -0.5;
    t.depthPerPass = 0.6;
    t.travelZ = 2.0;
    t.feedrate = 100.0;
    t.feedrateZ = 100.0;
    t.feedrateRapid = 1500.0;
    t.spindleSpeed = 1000.0;
    t.dwellTime = 1.5;
    t.endZ = 19.0;
    t.toolchangeZ = 19.0;
    t.extracutLength = 0.1;
    t.spindleDir = "CW";
    t.multidepth = false;
    t.dwell = true;
    t.extracut = false;
    t.endXY = std::array<double, 2>{0.0, 0.0};
    t.toolchangeXY = std::array<double, 2>{0.0, 0.0};
    t.tolerance = 0.0;
    t.segX = 0.0;
    t.segY = 0.0;
    return s;
}
inline flatcam::ShapeList geometry33() {  // pp_GRBL_11_no_M6_no_toolchange
    return {flatcam::Shape::fromWkt("LINESTRING (0 0, 10 0, 10 5)")};
}
inline flatcam::GeometryJobSettings settings33() {
    flatcam::GeometryJobSettings s;
    s.units = "MM";
    s.preprocessor = "GRBL_11_no_M6";
    s.order = flatcam::DrillOrder::FileOrder;
    s.tool = 1;
    s.toolchange = false;
    flatcam::GeometryToolSettings& t = s.toolSettings;
    t.toolDia = 1.2;
    t.offsetKind = "Path";
    t.offsetValue = 0.0;
    t.cutZ = -0.5;
    t.depthPerPass = 0.6;
    t.travelZ = 2.0;
    t.feedrate = 100.0;
    t.feedrateZ = 100.0;
    t.feedrateRapid = 1500.0;
    t.spindleSpeed = 1000.0;
    t.dwellTime = 1.0;
    t.endZ = 19.0;
    t.toolchangeZ = 19.0;
    t.extracutLength = 0.1;
    t.spindleDir = "CW";
    t.multidepth = false;
    t.dwell = false;
    t.extracut = false;
    t.startZ = 1.0;
    t.endXY = std::array<double, 2>{0.0, 0.0};
    t.toolchangeXY = std::array<double, 2>{0.0, 0.0};
    t.tolerance = 0.0;
    t.segX = 0.0;
    t.segY = 0.0;
    return s;
}
inline flatcam::ShapeList geometry34() {  // pp_hpgl
    return {flatcam::Shape::fromWkt("LINESTRING (0 0, 10 0, 10 5)"), flatcam::Shape::fromWkt("POINT (3 4)")};
}
inline flatcam::GeometryJobSettings settings34() {
    flatcam::GeometryJobSettings s;
    s.units = "MM";
    s.preprocessor = "hpgl";
    s.order = flatcam::DrillOrder::FileOrder;
    s.tool = 1;
    s.toolchange = true;
    flatcam::GeometryToolSettings& t = s.toolSettings;
    t.toolDia = 1.2;
    t.offsetKind = "Path";
    t.offsetValue = 0.0;
    t.cutZ = -0.5;
    t.depthPerPass = 0.6;
    t.travelZ = 2.0;
    t.feedrate = 100.0;
    t.feedrateZ = 100.0;
    t.feedrateRapid = 1500.0;
    t.spindleSpeed = 1000.0;
    t.dwellTime = 1.5;
    t.endZ = 19.0;
    t.toolchangeZ = 19.0;
    t.extracutLength = 0.1;
    t.spindleDir = "CW";
    t.multidepth = false;
    t.dwell = true;
    t.extracut = false;
    t.endXY = std::array<double, 2>{0.0, 0.0};
    t.toolchangeXY = std::array<double, 2>{0.0, 0.0};
    t.tolerance = 0.0;
    t.segX = 0.0;
    t.segY = 0.0;
    return s;
}
inline flatcam::ShapeList geometry35() {  // pp_hpgl_no_toolchange
    return {flatcam::Shape::fromWkt("LINESTRING (0 0, 10 0, 10 5)")};
}
inline flatcam::GeometryJobSettings settings35() {
    flatcam::GeometryJobSettings s;
    s.units = "MM";
    s.preprocessor = "hpgl";
    s.order = flatcam::DrillOrder::FileOrder;
    s.tool = 1;
    s.toolchange = false;
    flatcam::GeometryToolSettings& t = s.toolSettings;
    t.toolDia = 1.2;
    t.offsetKind = "Path";
    t.offsetValue = 0.0;
    t.cutZ = -0.5;
    t.depthPerPass = 0.6;
    t.travelZ = 2.0;
    t.feedrate = 100.0;
    t.feedrateZ = 100.0;
    t.feedrateRapid = 1500.0;
    t.spindleSpeed = 1000.0;
    t.dwellTime = 1.0;
    t.endZ = 19.0;
    t.toolchangeZ = 19.0;
    t.extracutLength = 0.1;
    t.spindleDir = "CW";
    t.multidepth = false;
    t.dwell = false;
    t.extracut = false;
    t.startZ = 1.0;
    t.endXY = std::array<double, 2>{0.0, 0.0};
    t.toolchangeXY = std::array<double, 2>{0.0, 0.0};
    t.tolerance = 0.0;
    t.segX = 0.0;
    t.segY = 0.0;
    return s;
}
inline flatcam::ShapeList geometry36() {  // pp_ISEL_CNC
    return {flatcam::Shape::fromWkt("LINESTRING (0 0, 10 0, 10 5)"), flatcam::Shape::fromWkt("POINT (3 4)")};
}
inline flatcam::GeometryJobSettings settings36() {
    flatcam::GeometryJobSettings s;
    s.units = "MM";
    s.preprocessor = "ISEL_CNC";
    s.order = flatcam::DrillOrder::FileOrder;
    s.tool = 1;
    s.toolchange = true;
    flatcam::GeometryToolSettings& t = s.toolSettings;
    t.toolDia = 1.2;
    t.offsetKind = "Path";
    t.offsetValue = 0.0;
    t.cutZ = -0.5;
    t.depthPerPass = 0.6;
    t.travelZ = 2.0;
    t.feedrate = 100.0;
    t.feedrateZ = 100.0;
    t.feedrateRapid = 1500.0;
    t.spindleSpeed = 1000.0;
    t.dwellTime = 1.5;
    t.endZ = 19.0;
    t.toolchangeZ = 19.0;
    t.extracutLength = 0.1;
    t.spindleDir = "CW";
    t.multidepth = false;
    t.dwell = true;
    t.extracut = false;
    t.endXY = std::array<double, 2>{0.0, 0.0};
    t.toolchangeXY = std::array<double, 2>{0.0, 0.0};
    t.tolerance = 0.0;
    t.segX = 0.0;
    t.segY = 0.0;
    return s;
}
inline flatcam::ShapeList geometry37() {  // pp_ISEL_CNC_no_toolchange
    return {flatcam::Shape::fromWkt("LINESTRING (0 0, 10 0, 10 5)")};
}
inline flatcam::GeometryJobSettings settings37() {
    flatcam::GeometryJobSettings s;
    s.units = "MM";
    s.preprocessor = "ISEL_CNC";
    s.order = flatcam::DrillOrder::FileOrder;
    s.tool = 1;
    s.toolchange = false;
    flatcam::GeometryToolSettings& t = s.toolSettings;
    t.toolDia = 1.2;
    t.offsetKind = "Path";
    t.offsetValue = 0.0;
    t.cutZ = -0.5;
    t.depthPerPass = 0.6;
    t.travelZ = 2.0;
    t.feedrate = 100.0;
    t.feedrateZ = 100.0;
    t.feedrateRapid = 1500.0;
    t.spindleSpeed = 1000.0;
    t.dwellTime = 1.0;
    t.endZ = 19.0;
    t.toolchangeZ = 19.0;
    t.extracutLength = 0.1;
    t.spindleDir = "CW";
    t.multidepth = false;
    t.dwell = false;
    t.extracut = false;
    t.startZ = 1.0;
    t.endXY = std::array<double, 2>{0.0, 0.0};
    t.toolchangeXY = std::array<double, 2>{0.0, 0.0};
    t.tolerance = 0.0;
    t.segX = 0.0;
    t.segY = 0.0;
    return s;
}
inline flatcam::ShapeList geometry38() {  // pp_ISEL_ICP_CNC
    return {flatcam::Shape::fromWkt("LINESTRING (0 0, 10 0, 10 5)"), flatcam::Shape::fromWkt("POINT (3 4)")};
}
inline flatcam::GeometryJobSettings settings38() {
    flatcam::GeometryJobSettings s;
    s.units = "MM";
    s.preprocessor = "ISEL_ICP_CNC";
    s.order = flatcam::DrillOrder::FileOrder;
    s.tool = 1;
    s.toolchange = true;
    flatcam::GeometryToolSettings& t = s.toolSettings;
    t.toolDia = 1.2;
    t.offsetKind = "Path";
    t.offsetValue = 0.0;
    t.cutZ = -0.5;
    t.depthPerPass = 0.6;
    t.travelZ = 2.0;
    t.feedrate = 100.0;
    t.feedrateZ = 100.0;
    t.feedrateRapid = 1500.0;
    t.spindleSpeed = 1000.0;
    t.dwellTime = 1.5;
    t.endZ = 19.0;
    t.toolchangeZ = 19.0;
    t.extracutLength = 0.1;
    t.spindleDir = "CW";
    t.multidepth = false;
    t.dwell = true;
    t.extracut = false;
    t.endXY = std::array<double, 2>{0.0, 0.0};
    t.toolchangeXY = std::array<double, 2>{0.0, 0.0};
    t.tolerance = 0.0;
    t.segX = 0.0;
    t.segY = 0.0;
    return s;
}
inline flatcam::ShapeList geometry39() {  // pp_ISEL_ICP_CNC_no_toolchange
    return {flatcam::Shape::fromWkt("LINESTRING (0 0, 10 0, 10 5)")};
}
inline flatcam::GeometryJobSettings settings39() {
    flatcam::GeometryJobSettings s;
    s.units = "MM";
    s.preprocessor = "ISEL_ICP_CNC";
    s.order = flatcam::DrillOrder::FileOrder;
    s.tool = 1;
    s.toolchange = false;
    flatcam::GeometryToolSettings& t = s.toolSettings;
    t.toolDia = 1.2;
    t.offsetKind = "Path";
    t.offsetValue = 0.0;
    t.cutZ = -0.5;
    t.depthPerPass = 0.6;
    t.travelZ = 2.0;
    t.feedrate = 100.0;
    t.feedrateZ = 100.0;
    t.feedrateRapid = 1500.0;
    t.spindleSpeed = 1000.0;
    t.dwellTime = 1.0;
    t.endZ = 19.0;
    t.toolchangeZ = 19.0;
    t.extracutLength = 0.1;
    t.spindleDir = "CW";
    t.multidepth = false;
    t.dwell = false;
    t.extracut = false;
    t.startZ = 1.0;
    t.endXY = std::array<double, 2>{0.0, 0.0};
    t.toolchangeXY = std::array<double, 2>{0.0, 0.0};
    t.tolerance = 0.0;
    t.segX = 0.0;
    t.segY = 0.0;
    return s;
}
inline flatcam::ShapeList geometry40() {  // pp_line_xyz
    return {flatcam::Shape::fromWkt("LINESTRING (0 0, 10 0, 10 5)"), flatcam::Shape::fromWkt("POINT (3 4)")};
}
inline flatcam::GeometryJobSettings settings40() {
    flatcam::GeometryJobSettings s;
    s.units = "MM";
    s.preprocessor = "line_xyz";
    s.order = flatcam::DrillOrder::FileOrder;
    s.tool = 1;
    s.toolchange = true;
    flatcam::GeometryToolSettings& t = s.toolSettings;
    t.toolDia = 1.2;
    t.offsetKind = "Path";
    t.offsetValue = 0.0;
    t.cutZ = -0.5;
    t.depthPerPass = 0.6;
    t.travelZ = 2.0;
    t.feedrate = 100.0;
    t.feedrateZ = 100.0;
    t.feedrateRapid = 1500.0;
    t.spindleSpeed = 1000.0;
    t.dwellTime = 1.5;
    t.endZ = 19.0;
    t.toolchangeZ = 19.0;
    t.extracutLength = 0.1;
    t.spindleDir = "CW";
    t.multidepth = false;
    t.dwell = true;
    t.extracut = false;
    t.endXY = std::array<double, 2>{0.0, 0.0};
    t.toolchangeXY = std::array<double, 2>{0.0, 0.0};
    t.tolerance = 0.0;
    t.segX = 0.0;
    t.segY = 0.0;
    return s;
}
inline flatcam::ShapeList geometry41() {  // pp_line_xyz_no_toolchange
    return {flatcam::Shape::fromWkt("LINESTRING (0 0, 10 0, 10 5)")};
}
inline flatcam::GeometryJobSettings settings41() {
    flatcam::GeometryJobSettings s;
    s.units = "MM";
    s.preprocessor = "line_xyz";
    s.order = flatcam::DrillOrder::FileOrder;
    s.tool = 1;
    s.toolchange = false;
    flatcam::GeometryToolSettings& t = s.toolSettings;
    t.toolDia = 1.2;
    t.offsetKind = "Path";
    t.offsetValue = 0.0;
    t.cutZ = -0.5;
    t.depthPerPass = 0.6;
    t.travelZ = 2.0;
    t.feedrate = 100.0;
    t.feedrateZ = 100.0;
    t.feedrateRapid = 1500.0;
    t.spindleSpeed = 1000.0;
    t.dwellTime = 1.0;
    t.endZ = 19.0;
    t.toolchangeZ = 19.0;
    t.extracutLength = 0.1;
    t.spindleDir = "CW";
    t.multidepth = false;
    t.dwell = false;
    t.extracut = false;
    t.startZ = 1.0;
    t.endXY = std::array<double, 2>{0.0, 0.0};
    t.toolchangeXY = std::array<double, 2>{0.0, 0.0};
    t.tolerance = 0.0;
    t.segX = 0.0;
    t.segY = 0.0;
    return s;
}
inline flatcam::ShapeList geometry42() {  // pp_Marlin
    return {flatcam::Shape::fromWkt("LINESTRING (0 0, 10 0, 10 5)"), flatcam::Shape::fromWkt("POINT (3 4)")};
}
inline flatcam::GeometryJobSettings settings42() {
    flatcam::GeometryJobSettings s;
    s.units = "MM";
    s.preprocessor = "Marlin";
    s.order = flatcam::DrillOrder::FileOrder;
    s.tool = 1;
    s.toolchange = true;
    flatcam::GeometryToolSettings& t = s.toolSettings;
    t.toolDia = 1.2;
    t.offsetKind = "Path";
    t.offsetValue = 0.0;
    t.cutZ = -0.5;
    t.depthPerPass = 0.6;
    t.travelZ = 2.0;
    t.feedrate = 100.0;
    t.feedrateZ = 100.0;
    t.feedrateRapid = 1500.0;
    t.spindleSpeed = 1000.0;
    t.dwellTime = 1.5;
    t.endZ = 19.0;
    t.toolchangeZ = 19.0;
    t.extracutLength = 0.1;
    t.spindleDir = "CW";
    t.multidepth = false;
    t.dwell = true;
    t.extracut = false;
    t.endXY = std::array<double, 2>{0.0, 0.0};
    t.toolchangeXY = std::array<double, 2>{0.0, 0.0};
    t.tolerance = 0.0;
    t.segX = 0.0;
    t.segY = 0.0;
    return s;
}
inline flatcam::ShapeList geometry43() {  // pp_Marlin_no_toolchange
    return {flatcam::Shape::fromWkt("LINESTRING (0 0, 10 0, 10 5)")};
}
inline flatcam::GeometryJobSettings settings43() {
    flatcam::GeometryJobSettings s;
    s.units = "MM";
    s.preprocessor = "Marlin";
    s.order = flatcam::DrillOrder::FileOrder;
    s.tool = 1;
    s.toolchange = false;
    flatcam::GeometryToolSettings& t = s.toolSettings;
    t.toolDia = 1.2;
    t.offsetKind = "Path";
    t.offsetValue = 0.0;
    t.cutZ = -0.5;
    t.depthPerPass = 0.6;
    t.travelZ = 2.0;
    t.feedrate = 100.0;
    t.feedrateZ = 100.0;
    t.feedrateRapid = 1500.0;
    t.spindleSpeed = 1000.0;
    t.dwellTime = 1.0;
    t.endZ = 19.0;
    t.toolchangeZ = 19.0;
    t.extracutLength = 0.1;
    t.spindleDir = "CW";
    t.multidepth = false;
    t.dwell = false;
    t.extracut = false;
    t.startZ = 1.0;
    t.endXY = std::array<double, 2>{0.0, 0.0};
    t.toolchangeXY = std::array<double, 2>{0.0, 0.0};
    t.tolerance = 0.0;
    t.segX = 0.0;
    t.segY = 0.0;
    return s;
}
inline flatcam::ShapeList geometry44() {  // pp_Marlin_laser_FAN_pin
    return {flatcam::Shape::fromWkt("LINESTRING (0 0, 10 0, 10 5)"), flatcam::Shape::fromWkt("POINT (3 4)")};
}
inline flatcam::GeometryJobSettings settings44() {
    flatcam::GeometryJobSettings s;
    s.units = "MM";
    s.preprocessor = "Marlin_laser_FAN_pin";
    s.order = flatcam::DrillOrder::FileOrder;
    s.tool = 1;
    s.toolchange = true;
    flatcam::GeometryToolSettings& t = s.toolSettings;
    t.toolDia = 1.2;
    t.offsetKind = "Path";
    t.offsetValue = 0.0;
    t.cutZ = -0.5;
    t.depthPerPass = 0.6;
    t.travelZ = 2.0;
    t.feedrate = 100.0;
    t.feedrateZ = 100.0;
    t.feedrateRapid = 1500.0;
    t.spindleSpeed = 1000.0;
    t.dwellTime = 1.5;
    t.endZ = 19.0;
    t.toolchangeZ = 19.0;
    t.extracutLength = 0.1;
    t.spindleDir = "CW";
    t.multidepth = false;
    t.dwell = true;
    t.extracut = false;
    t.endXY = std::array<double, 2>{0.0, 0.0};
    t.toolchangeXY = std::array<double, 2>{0.0, 0.0};
    t.tolerance = 0.0;
    t.segX = 0.0;
    t.segY = 0.0;
    return s;
}
inline flatcam::ShapeList geometry45() {  // pp_Marlin_laser_FAN_pin_no_toolchange
    return {flatcam::Shape::fromWkt("LINESTRING (0 0, 10 0, 10 5)")};
}
inline flatcam::GeometryJobSettings settings45() {
    flatcam::GeometryJobSettings s;
    s.units = "MM";
    s.preprocessor = "Marlin_laser_FAN_pin";
    s.order = flatcam::DrillOrder::FileOrder;
    s.tool = 1;
    s.toolchange = false;
    flatcam::GeometryToolSettings& t = s.toolSettings;
    t.toolDia = 1.2;
    t.offsetKind = "Path";
    t.offsetValue = 0.0;
    t.cutZ = -0.5;
    t.depthPerPass = 0.6;
    t.travelZ = 2.0;
    t.feedrate = 100.0;
    t.feedrateZ = 100.0;
    t.feedrateRapid = 1500.0;
    t.spindleSpeed = 1000.0;
    t.dwellTime = 1.0;
    t.endZ = 19.0;
    t.toolchangeZ = 19.0;
    t.extracutLength = 0.1;
    t.spindleDir = "CW";
    t.multidepth = false;
    t.dwell = false;
    t.extracut = false;
    t.startZ = 1.0;
    t.endXY = std::array<double, 2>{0.0, 0.0};
    t.toolchangeXY = std::array<double, 2>{0.0, 0.0};
    t.tolerance = 0.0;
    t.segX = 0.0;
    t.segY = 0.0;
    return s;
}
inline flatcam::ShapeList geometry46() {  // pp_Marlin_laser_Spindle_pin
    return {flatcam::Shape::fromWkt("LINESTRING (0 0, 10 0, 10 5)"), flatcam::Shape::fromWkt("POINT (3 4)")};
}
inline flatcam::GeometryJobSettings settings46() {
    flatcam::GeometryJobSettings s;
    s.units = "MM";
    s.preprocessor = "Marlin_laser_Spindle_pin";
    s.order = flatcam::DrillOrder::FileOrder;
    s.tool = 1;
    s.toolchange = true;
    flatcam::GeometryToolSettings& t = s.toolSettings;
    t.toolDia = 1.2;
    t.offsetKind = "Path";
    t.offsetValue = 0.0;
    t.cutZ = -0.5;
    t.depthPerPass = 0.6;
    t.travelZ = 2.0;
    t.feedrate = 100.0;
    t.feedrateZ = 100.0;
    t.feedrateRapid = 1500.0;
    t.spindleSpeed = 1000.0;
    t.dwellTime = 1.5;
    t.endZ = 19.0;
    t.toolchangeZ = 19.0;
    t.extracutLength = 0.1;
    t.spindleDir = "CW";
    t.multidepth = false;
    t.dwell = true;
    t.extracut = false;
    t.endXY = std::array<double, 2>{0.0, 0.0};
    t.toolchangeXY = std::array<double, 2>{0.0, 0.0};
    t.tolerance = 0.0;
    t.segX = 0.0;
    t.segY = 0.0;
    return s;
}
inline flatcam::ShapeList geometry47() {  // pp_Marlin_laser_Spindle_pin_no_toolchange
    return {flatcam::Shape::fromWkt("LINESTRING (0 0, 10 0, 10 5)")};
}
inline flatcam::GeometryJobSettings settings47() {
    flatcam::GeometryJobSettings s;
    s.units = "MM";
    s.preprocessor = "Marlin_laser_Spindle_pin";
    s.order = flatcam::DrillOrder::FileOrder;
    s.tool = 1;
    s.toolchange = false;
    flatcam::GeometryToolSettings& t = s.toolSettings;
    t.toolDia = 1.2;
    t.offsetKind = "Path";
    t.offsetValue = 0.0;
    t.cutZ = -0.5;
    t.depthPerPass = 0.6;
    t.travelZ = 2.0;
    t.feedrate = 100.0;
    t.feedrateZ = 100.0;
    t.feedrateRapid = 1500.0;
    t.spindleSpeed = 1000.0;
    t.dwellTime = 1.0;
    t.endZ = 19.0;
    t.toolchangeZ = 19.0;
    t.extracutLength = 0.1;
    t.spindleDir = "CW";
    t.multidepth = false;
    t.dwell = false;
    t.extracut = false;
    t.startZ = 1.0;
    t.endXY = std::array<double, 2>{0.0, 0.0};
    t.toolchangeXY = std::array<double, 2>{0.0, 0.0};
    t.tolerance = 0.0;
    t.segX = 0.0;
    t.segY = 0.0;
    return s;
}
inline flatcam::ShapeList geometry48() {  // pp_Repetier
    return {flatcam::Shape::fromWkt("LINESTRING (0 0, 10 0, 10 5)"), flatcam::Shape::fromWkt("POINT (3 4)")};
}
inline flatcam::GeometryJobSettings settings48() {
    flatcam::GeometryJobSettings s;
    s.units = "MM";
    s.preprocessor = "Repetier";
    s.order = flatcam::DrillOrder::FileOrder;
    s.tool = 1;
    s.toolchange = true;
    flatcam::GeometryToolSettings& t = s.toolSettings;
    t.toolDia = 1.2;
    t.offsetKind = "Path";
    t.offsetValue = 0.0;
    t.cutZ = -0.5;
    t.depthPerPass = 0.6;
    t.travelZ = 2.0;
    t.feedrate = 100.0;
    t.feedrateZ = 100.0;
    t.feedrateRapid = 1500.0;
    t.spindleSpeed = 1000.0;
    t.dwellTime = 1.5;
    t.endZ = 19.0;
    t.toolchangeZ = 19.0;
    t.extracutLength = 0.1;
    t.spindleDir = "CW";
    t.multidepth = false;
    t.dwell = true;
    t.extracut = false;
    t.endXY = std::array<double, 2>{0.0, 0.0};
    t.toolchangeXY = std::array<double, 2>{0.0, 0.0};
    t.tolerance = 0.0;
    t.segX = 0.0;
    t.segY = 0.0;
    return s;
}
inline flatcam::ShapeList geometry49() {  // pp_Repetier_no_toolchange
    return {flatcam::Shape::fromWkt("LINESTRING (0 0, 10 0, 10 5)")};
}
inline flatcam::GeometryJobSettings settings49() {
    flatcam::GeometryJobSettings s;
    s.units = "MM";
    s.preprocessor = "Repetier";
    s.order = flatcam::DrillOrder::FileOrder;
    s.tool = 1;
    s.toolchange = false;
    flatcam::GeometryToolSettings& t = s.toolSettings;
    t.toolDia = 1.2;
    t.offsetKind = "Path";
    t.offsetValue = 0.0;
    t.cutZ = -0.5;
    t.depthPerPass = 0.6;
    t.travelZ = 2.0;
    t.feedrate = 100.0;
    t.feedrateZ = 100.0;
    t.feedrateRapid = 1500.0;
    t.spindleSpeed = 1000.0;
    t.dwellTime = 1.0;
    t.endZ = 19.0;
    t.toolchangeZ = 19.0;
    t.extracutLength = 0.1;
    t.spindleDir = "CW";
    t.multidepth = false;
    t.dwell = false;
    t.extracut = false;
    t.startZ = 1.0;
    t.endXY = std::array<double, 2>{0.0, 0.0};
    t.toolchangeXY = std::array<double, 2>{0.0, 0.0};
    t.tolerance = 0.0;
    t.segX = 0.0;
    t.segY = 0.0;
    return s;
}
inline flatcam::ShapeList geometry50() {  // pp_Roland_MDX_20
    return {flatcam::Shape::fromWkt("LINESTRING (0 0, 10 0, 10 5)"), flatcam::Shape::fromWkt("POINT (3 4)")};
}
inline flatcam::GeometryJobSettings settings50() {
    flatcam::GeometryJobSettings s;
    s.units = "MM";
    s.preprocessor = "Roland_MDX_20";
    s.order = flatcam::DrillOrder::FileOrder;
    s.tool = 1;
    s.toolchange = true;
    flatcam::GeometryToolSettings& t = s.toolSettings;
    t.toolDia = 1.2;
    t.offsetKind = "Path";
    t.offsetValue = 0.0;
    t.cutZ = -0.5;
    t.depthPerPass = 0.6;
    t.travelZ = 2.0;
    t.feedrate = 100.0;
    t.feedrateZ = 100.0;
    t.feedrateRapid = 1500.0;
    t.spindleSpeed = 1000.0;
    t.dwellTime = 1.5;
    t.endZ = 19.0;
    t.toolchangeZ = 19.0;
    t.extracutLength = 0.1;
    t.spindleDir = "CW";
    t.multidepth = false;
    t.dwell = true;
    t.extracut = false;
    t.endXY = std::array<double, 2>{0.0, 0.0};
    t.toolchangeXY = std::array<double, 2>{0.0, 0.0};
    t.tolerance = 0.0;
    t.segX = 0.0;
    t.segY = 0.0;
    return s;
}
inline flatcam::ShapeList geometry51() {  // pp_Roland_MDX_20_no_toolchange
    return {flatcam::Shape::fromWkt("LINESTRING (0 0, 10 0, 10 5)")};
}
inline flatcam::GeometryJobSettings settings51() {
    flatcam::GeometryJobSettings s;
    s.units = "MM";
    s.preprocessor = "Roland_MDX_20";
    s.order = flatcam::DrillOrder::FileOrder;
    s.tool = 1;
    s.toolchange = false;
    flatcam::GeometryToolSettings& t = s.toolSettings;
    t.toolDia = 1.2;
    t.offsetKind = "Path";
    t.offsetValue = 0.0;
    t.cutZ = -0.5;
    t.depthPerPass = 0.6;
    t.travelZ = 2.0;
    t.feedrate = 100.0;
    t.feedrateZ = 100.0;
    t.feedrateRapid = 1500.0;
    t.spindleSpeed = 1000.0;
    t.dwellTime = 1.0;
    t.endZ = 19.0;
    t.toolchangeZ = 19.0;
    t.extracutLength = 0.1;
    t.spindleDir = "CW";
    t.multidepth = false;
    t.dwell = false;
    t.extracut = false;
    t.startZ = 1.0;
    t.endXY = std::array<double, 2>{0.0, 0.0};
    t.toolchangeXY = std::array<double, 2>{0.0, 0.0};
    t.tolerance = 0.0;
    t.segX = 0.0;
    t.segY = 0.0;
    return s;
}
inline flatcam::ShapeList geometry52() {  // pp_Toolchange_Custom
    return {flatcam::Shape::fromWkt("LINESTRING (0 0, 10 0, 10 5)"), flatcam::Shape::fromWkt("POINT (3 4)")};
}
inline flatcam::GeometryJobSettings settings52() {
    flatcam::GeometryJobSettings s;
    s.units = "MM";
    s.preprocessor = "Toolchange_Custom";
    s.order = flatcam::DrillOrder::FileOrder;
    s.tool = 1;
    s.toolchange = true;
    flatcam::GeometryToolSettings& t = s.toolSettings;
    t.toolDia = 1.2;
    t.offsetKind = "Path";
    t.offsetValue = 0.0;
    t.cutZ = -0.5;
    t.depthPerPass = 0.6;
    t.travelZ = 2.0;
    t.feedrate = 100.0;
    t.feedrateZ = 100.0;
    t.feedrateRapid = 1500.0;
    t.spindleSpeed = 1000.0;
    t.dwellTime = 1.5;
    t.endZ = 19.0;
    t.toolchangeZ = 19.0;
    t.extracutLength = 0.1;
    t.spindleDir = "CW";
    t.multidepth = false;
    t.dwell = true;
    t.extracut = false;
    t.endXY = std::array<double, 2>{0.0, 0.0};
    t.toolchangeXY = std::array<double, 2>{0.0, 0.0};
    t.tolerance = 0.0;
    t.segX = 0.0;
    t.segY = 0.0;
    return s;
}
inline flatcam::ShapeList geometry53() {  // pp_Toolchange_Custom_no_toolchange
    return {flatcam::Shape::fromWkt("LINESTRING (0 0, 10 0, 10 5)")};
}
inline flatcam::GeometryJobSettings settings53() {
    flatcam::GeometryJobSettings s;
    s.units = "MM";
    s.preprocessor = "Toolchange_Custom";
    s.order = flatcam::DrillOrder::FileOrder;
    s.tool = 1;
    s.toolchange = false;
    flatcam::GeometryToolSettings& t = s.toolSettings;
    t.toolDia = 1.2;
    t.offsetKind = "Path";
    t.offsetValue = 0.0;
    t.cutZ = -0.5;
    t.depthPerPass = 0.6;
    t.travelZ = 2.0;
    t.feedrate = 100.0;
    t.feedrateZ = 100.0;
    t.feedrateRapid = 1500.0;
    t.spindleSpeed = 1000.0;
    t.dwellTime = 1.0;
    t.endZ = 19.0;
    t.toolchangeZ = 19.0;
    t.extracutLength = 0.1;
    t.spindleDir = "CW";
    t.multidepth = false;
    t.dwell = false;
    t.extracut = false;
    t.startZ = 1.0;
    t.endXY = std::array<double, 2>{0.0, 0.0};
    t.toolchangeXY = std::array<double, 2>{0.0, 0.0};
    t.tolerance = 0.0;
    t.segX = 0.0;
    t.segY = 0.0;
    return s;
}
inline flatcam::ShapeList geometry54() {  // pp_Toolchange_Manual
    return {flatcam::Shape::fromWkt("LINESTRING (0 0, 10 0, 10 5)"), flatcam::Shape::fromWkt("POINT (3 4)")};
}
inline flatcam::GeometryJobSettings settings54() {
    flatcam::GeometryJobSettings s;
    s.units = "MM";
    s.preprocessor = "Toolchange_Manual";
    s.order = flatcam::DrillOrder::FileOrder;
    s.tool = 1;
    s.toolchange = true;
    flatcam::GeometryToolSettings& t = s.toolSettings;
    t.toolDia = 1.2;
    t.offsetKind = "Path";
    t.offsetValue = 0.0;
    t.cutZ = -0.5;
    t.depthPerPass = 0.6;
    t.travelZ = 2.0;
    t.feedrate = 100.0;
    t.feedrateZ = 100.0;
    t.feedrateRapid = 1500.0;
    t.spindleSpeed = 1000.0;
    t.dwellTime = 1.5;
    t.endZ = 19.0;
    t.toolchangeZ = 19.0;
    t.extracutLength = 0.1;
    t.spindleDir = "CW";
    t.multidepth = false;
    t.dwell = true;
    t.extracut = false;
    t.endXY = std::array<double, 2>{0.0, 0.0};
    t.toolchangeXY = std::array<double, 2>{0.0, 0.0};
    t.tolerance = 0.0;
    t.segX = 0.0;
    t.segY = 0.0;
    return s;
}
inline flatcam::ShapeList geometry55() {  // pp_Toolchange_Manual_no_toolchange
    return {flatcam::Shape::fromWkt("LINESTRING (0 0, 10 0, 10 5)")};
}
inline flatcam::GeometryJobSettings settings55() {
    flatcam::GeometryJobSettings s;
    s.units = "MM";
    s.preprocessor = "Toolchange_Manual";
    s.order = flatcam::DrillOrder::FileOrder;
    s.tool = 1;
    s.toolchange = false;
    flatcam::GeometryToolSettings& t = s.toolSettings;
    t.toolDia = 1.2;
    t.offsetKind = "Path";
    t.offsetValue = 0.0;
    t.cutZ = -0.5;
    t.depthPerPass = 0.6;
    t.travelZ = 2.0;
    t.feedrate = 100.0;
    t.feedrateZ = 100.0;
    t.feedrateRapid = 1500.0;
    t.spindleSpeed = 1000.0;
    t.dwellTime = 1.0;
    t.endZ = 19.0;
    t.toolchangeZ = 19.0;
    t.extracutLength = 0.1;
    t.spindleDir = "CW";
    t.multidepth = false;
    t.dwell = false;
    t.extracut = false;
    t.startZ = 1.0;
    t.endXY = std::array<double, 2>{0.0, 0.0};
    t.toolchangeXY = std::array<double, 2>{0.0, 0.0};
    t.tolerance = 0.0;
    t.segX = 0.0;
    t.segY = 0.0;
    return s;
}
inline flatcam::ShapeList geometry56() {  // pp_Toolchange_Probe_MACH3
    return {flatcam::Shape::fromWkt("LINESTRING (0 0, 10 0, 10 5)"), flatcam::Shape::fromWkt("POINT (3 4)")};
}
inline flatcam::GeometryJobSettings settings56() {
    flatcam::GeometryJobSettings s;
    s.units = "MM";
    s.preprocessor = "Toolchange_Probe_MACH3";
    s.order = flatcam::DrillOrder::FileOrder;
    s.tool = 1;
    s.toolchange = true;
    flatcam::GeometryToolSettings& t = s.toolSettings;
    t.toolDia = 1.2;
    t.offsetKind = "Path";
    t.offsetValue = 0.0;
    t.cutZ = -0.5;
    t.depthPerPass = 0.6;
    t.travelZ = 2.0;
    t.feedrate = 100.0;
    t.feedrateZ = 100.0;
    t.feedrateRapid = 1500.0;
    t.spindleSpeed = 1000.0;
    t.dwellTime = 1.5;
    t.endZ = 19.0;
    t.toolchangeZ = 19.0;
    t.extracutLength = 0.1;
    t.spindleDir = "CW";
    t.multidepth = false;
    t.dwell = true;
    t.extracut = false;
    t.endXY = std::array<double, 2>{0.0, 0.0};
    t.toolchangeXY = std::array<double, 2>{0.0, 0.0};
    t.tolerance = 0.0;
    t.segX = 0.0;
    t.segY = 0.0;
    return s;
}
inline flatcam::ShapeList geometry57() {  // pp_Toolchange_Probe_MACH3_no_toolchange
    return {flatcam::Shape::fromWkt("LINESTRING (0 0, 10 0, 10 5)")};
}
inline flatcam::GeometryJobSettings settings57() {
    flatcam::GeometryJobSettings s;
    s.units = "MM";
    s.preprocessor = "Toolchange_Probe_MACH3";
    s.order = flatcam::DrillOrder::FileOrder;
    s.tool = 1;
    s.toolchange = false;
    flatcam::GeometryToolSettings& t = s.toolSettings;
    t.toolDia = 1.2;
    t.offsetKind = "Path";
    t.offsetValue = 0.0;
    t.cutZ = -0.5;
    t.depthPerPass = 0.6;
    t.travelZ = 2.0;
    t.feedrate = 100.0;
    t.feedrateZ = 100.0;
    t.feedrateRapid = 1500.0;
    t.spindleSpeed = 1000.0;
    t.dwellTime = 1.0;
    t.endZ = 19.0;
    t.toolchangeZ = 19.0;
    t.extracutLength = 0.1;
    t.spindleDir = "CW";
    t.multidepth = false;
    t.dwell = false;
    t.extracut = false;
    t.startZ = 1.0;
    t.endXY = std::array<double, 2>{0.0, 0.0};
    t.toolchangeXY = std::array<double, 2>{0.0, 0.0};
    t.tolerance = 0.0;
    t.segX = 0.0;
    t.segY = 0.0;
    return s;
}
inline flatcam::ShapeList geometry58() {  // pp_Z_laser
    return {flatcam::Shape::fromWkt("LINESTRING (0 0, 10 0, 10 5)"), flatcam::Shape::fromWkt("POINT (3 4)")};
}
inline flatcam::GeometryJobSettings settings58() {
    flatcam::GeometryJobSettings s;
    s.units = "MM";
    s.preprocessor = "Z_laser";
    s.order = flatcam::DrillOrder::FileOrder;
    s.tool = 1;
    s.toolchange = true;
    flatcam::GeometryToolSettings& t = s.toolSettings;
    t.toolDia = 1.2;
    t.offsetKind = "Path";
    t.offsetValue = 0.0;
    t.cutZ = -0.5;
    t.depthPerPass = 0.6;
    t.travelZ = 2.0;
    t.feedrate = 100.0;
    t.feedrateZ = 100.0;
    t.feedrateRapid = 1500.0;
    t.spindleSpeed = 1000.0;
    t.dwellTime = 1.5;
    t.endZ = 19.0;
    t.toolchangeZ = 19.0;
    t.extracutLength = 0.1;
    t.spindleDir = "CW";
    t.multidepth = false;
    t.dwell = true;
    t.extracut = false;
    t.endXY = std::array<double, 2>{0.0, 0.0};
    t.toolchangeXY = std::array<double, 2>{0.0, 0.0};
    t.tolerance = 0.0;
    t.segX = 0.0;
    t.segY = 0.0;
    return s;
}
inline flatcam::ShapeList geometry59() {  // pp_Z_laser_no_toolchange
    return {flatcam::Shape::fromWkt("LINESTRING (0 0, 10 0, 10 5)")};
}
inline flatcam::GeometryJobSettings settings59() {
    flatcam::GeometryJobSettings s;
    s.units = "MM";
    s.preprocessor = "Z_laser";
    s.order = flatcam::DrillOrder::FileOrder;
    s.tool = 1;
    s.toolchange = false;
    flatcam::GeometryToolSettings& t = s.toolSettings;
    t.toolDia = 1.2;
    t.offsetKind = "Path";
    t.offsetValue = 0.0;
    t.cutZ = -0.5;
    t.depthPerPass = 0.6;
    t.travelZ = 2.0;
    t.feedrate = 100.0;
    t.feedrateZ = 100.0;
    t.feedrateRapid = 1500.0;
    t.spindleSpeed = 1000.0;
    t.dwellTime = 1.0;
    t.endZ = 19.0;
    t.toolchangeZ = 19.0;
    t.extracutLength = 0.1;
    t.spindleDir = "CW";
    t.multidepth = false;
    t.dwell = false;
    t.extracut = false;
    t.startZ = 1.0;
    t.endXY = std::array<double, 2>{0.0, 0.0};
    t.toolchangeXY = std::array<double, 2>{0.0, 0.0};
    t.tolerance = 0.0;
    t.segX = 0.0;
    t.segY = 0.0;
    return s;
}
inline flatcam::ShapeList geometry60() {  // pp_Roland_MDX_20_extracut
    return {flatcam::Shape::fromWkt("POLYGON ((0 0, 10 0, 10 10, 0 10, 0 0))")};
}
inline flatcam::GeometryJobSettings settings60() {
    flatcam::GeometryJobSettings s;
    s.units = "MM";
    s.preprocessor = "Roland_MDX_20";
    s.order = flatcam::DrillOrder::FileOrder;
    s.tool = 1;
    s.toolchange = true;
    flatcam::GeometryToolSettings& t = s.toolSettings;
    t.toolDia = 1.2;
    t.offsetKind = "Path";
    t.offsetValue = 0.0;
    t.cutZ = -1.0;
    t.depthPerPass = 0.6;
    t.travelZ = 2.0;
    t.feedrate = 100.0;
    t.feedrateZ = 100.0;
    t.feedrateRapid = 1500.0;
    t.spindleSpeed = 1000.0;
    t.dwellTime = 1.0;
    t.endZ = 19.0;
    t.toolchangeZ = 19.0;
    t.extracutLength = 0.5;
    t.spindleDir = "CW";
    t.multidepth = false;
    t.dwell = false;
    t.extracut = true;
    t.endXY = std::array<double, 2>{0.0, 0.0};
    t.toolchangeXY = std::array<double, 2>{0.0, 0.0};
    t.tolerance = 0.0;
    t.segX = 0.0;
    t.segY = 0.0;
    return s;
}
inline flatcam::ShapeList geometry61() {  // pp_Roland_MDX_20_extracut_zero
    return {flatcam::Shape::fromWkt("POLYGON ((0 0, 10 0, 10 10, 0 10, 0 0))")};
}
inline flatcam::GeometryJobSettings settings61() {
    flatcam::GeometryJobSettings s;
    s.units = "MM";
    s.preprocessor = "Roland_MDX_20";
    s.order = flatcam::DrillOrder::FileOrder;
    s.tool = 1;
    s.toolchange = true;
    flatcam::GeometryToolSettings& t = s.toolSettings;
    t.toolDia = 1.2;
    t.offsetKind = "Path";
    t.offsetValue = 0.0;
    t.cutZ = -1.0;
    t.depthPerPass = 0.6;
    t.travelZ = 2.0;
    t.feedrate = 100.0;
    t.feedrateZ = 100.0;
    t.feedrateRapid = 1500.0;
    t.spindleSpeed = 1000.0;
    t.dwellTime = 1.0;
    t.endZ = 19.0;
    t.toolchangeZ = 19.0;
    t.extracutLength = 0.0;
    t.spindleDir = "CW";
    t.multidepth = false;
    t.dwell = false;
    t.extracut = true;
    t.endXY = std::array<double, 2>{0.0, 0.0};
    t.toolchangeXY = std::array<double, 2>{0.0, 0.0};
    t.tolerance = 0.0;
    t.segX = 0.0;
    t.segY = 0.0;
    return s;
}
inline flatcam::ShapeList geometry62() {  // pp_Marlin_multidepth
    return {flatcam::Shape::fromWkt("POLYGON ((0 0, 10 0, 10 10, 0 10, 0 0))")};
}
inline flatcam::GeometryJobSettings settings62() {
    flatcam::GeometryJobSettings s;
    s.units = "MM";
    s.preprocessor = "Marlin";
    s.order = flatcam::DrillOrder::FileOrder;
    s.tool = 1;
    s.toolchange = true;
    flatcam::GeometryToolSettings& t = s.toolSettings;
    t.toolDia = 1.2;
    t.offsetKind = "Path";
    t.offsetValue = 0.0;
    t.cutZ = -1.0;
    t.depthPerPass = 0.4;
    t.travelZ = 2.0;
    t.feedrate = 100.0;
    t.feedrateZ = 100.0;
    t.feedrateRapid = 1500.0;
    t.spindleSpeed = 1000.0;
    t.dwellTime = 1.0;
    t.endZ = 19.0;
    t.toolchangeZ = 19.0;
    t.extracutLength = 0.1;
    t.spindleDir = "CW";
    t.multidepth = true;
    t.dwell = false;
    t.extracut = false;
    t.endXY = std::array<double, 2>{0.0, 0.0};
    t.toolchangeXY = std::array<double, 2>{0.0, 0.0};
    t.tolerance = 0.0;
    t.segX = 0.0;
    t.segY = 0.0;
    return s;
}
inline flatcam::ShapeList geometry63() {  // pp_line_xyz_extracut_multidepth
    return {flatcam::Shape::fromWkt("POLYGON ((0 0, 10 0, 10 10, 0 10, 0 0))")};
}
inline flatcam::GeometryJobSettings settings63() {
    flatcam::GeometryJobSettings s;
    s.units = "MM";
    s.preprocessor = "line_xyz";
    s.order = flatcam::DrillOrder::FileOrder;
    s.tool = 1;
    s.toolchange = true;
    flatcam::GeometryToolSettings& t = s.toolSettings;
    t.toolDia = 1.2;
    t.offsetKind = "Path";
    t.offsetValue = 0.0;
    t.cutZ = -1.0;
    t.depthPerPass = 0.5;
    t.travelZ = 2.0;
    t.feedrate = 100.0;
    t.feedrateZ = 100.0;
    t.feedrateRapid = 1500.0;
    t.spindleSpeed = 1000.0;
    t.dwellTime = 1.0;
    t.endZ = 19.0;
    t.toolchangeZ = 19.0;
    t.extracutLength = 1.0;
    t.spindleDir = "CW";
    t.multidepth = true;
    t.dwell = false;
    t.extracut = true;
    t.endXY = std::array<double, 2>{0.0, 0.0};
    t.toolchangeXY = std::array<double, 2>{0.0, 0.0};
    t.tolerance = 0.0;
    t.segX = 0.0;
    t.segY = 0.0;
    return s;
}
inline flatcam::ShapeList geometry64() {  // pp_hpgl_inches
    return {flatcam::Shape::fromWkt("LINESTRING (0 0, 10 0, 10 5)")};
}
inline flatcam::GeometryJobSettings settings64() {
    flatcam::GeometryJobSettings s;
    s.units = "IN";
    s.preprocessor = "hpgl";
    s.order = flatcam::DrillOrder::FileOrder;
    s.tool = 1;
    s.toolchange = true;
    flatcam::GeometryToolSettings& t = s.toolSettings;
    t.toolDia = 1.2;
    t.offsetKind = "Path";
    t.offsetValue = 0.0;
    t.cutZ = -0.07;
    t.depthPerPass = 0.6;
    t.travelZ = 0.08;
    t.feedrate = 100.0;
    t.feedrateZ = 100.0;
    t.feedrateRapid = 1500.0;
    t.spindleSpeed = 1000.0;
    t.dwellTime = 1.0;
    t.endZ = 19.0;
    t.toolchangeZ = 19.0;
    t.extracutLength = 0.1;
    t.spindleDir = "CW";
    t.multidepth = false;
    t.dwell = false;
    t.extracut = false;
    t.endXY = std::array<double, 2>{0.0, 0.0};
    t.toolchangeXY = std::array<double, 2>{0.0, 0.0};
    t.tolerance = 0.0;
    t.segX = 0.0;
    t.segY = 0.0;
    return s;
}
struct Scenario { const char* name; flatcam::ShapeList (*geometry)(); flatcam::GeometryJobSettings (*settings)(); const char* expected; };
inline const std::vector<Scenario> kScenarios = {
{"multidepth_open_line", geometry0, settings0, "(This preprocessor is the default preprocessor used by FlatCAM.)\n(It is made to work with MACH3 compatible motion controllers.)\n\n(TOOL DIAMETER: 1.2 mm)\n(Feedrate_XY: 100.0 mm/min)\n(Feedrate_Z: 100.0 mm/min)\n(Feedrate rapids 1500.0 mm/min)\n\n(Z_Cut: -1.8 mm)\n(DepthPerCut: 0.6 mm <=>3 passes)\n(Z_Move: 2.0 mm)\n(Z Start: None mm)\n(Z End: 19.0 mm)\n(X,Y End: 0.0000, 0.0000 mm)\n(Steps per circle: 64)\n(Preprocessor Geometry: default)\n\n(X range:    0.0000 ...   10.0000  mm)\n(Y range:    0.0000 ...    5.0000  mm)\n\n(Spindle Speed: 1000.0 RPM)\nG21\nG90\nG94\n\nG01 F100.00\n\nM5\nG00 Z19.0000\nG00 X0.0000 Y0.0000\nT1\nM6    \n(MSG, Change to Tool Dia = 1.2000)\nM0\nG00 Z19.0000\n\nM03 S1000.0\nG01 F100.00\nG00 X0.0000 Y0.0000\nG01 F100.00\nG01 Z-0.6000\nG01 F100.00\nG01 X10.0000 Y0.0000\nG01 X10.0000 Y5.0000\nG00 X10.0000 Y5.0000\nG01 F100.00\nG01 Z-1.2000\nG01 F100.00\nG01 X10.0000 Y0.0000\nG01 X0.0000 Y0.0000\nG00 X0.0000 Y0.0000\nG01 F100.00\nG01 Z-1.8000\nG01 F100.00\nG01 X10.0000 Y0.0000\nG01 X10.0000 Y5.0000\nG00 Z2.0000\nM05\nG00 Z2.0000\nG00 Z19.00\nG00 X0.0 Y0.0\n\n"},
{"multidepth_boundary_float", geometry1, settings1, "(This preprocessor is the default preprocessor used by FlatCAM.)\n(It is made to work with MACH3 compatible motion controllers.)\n\n(TOOL DIAMETER: 1.2 mm)\n(Feedrate_XY: 100.0 mm/min)\n(Feedrate_Z: 100.0 mm/min)\n(Feedrate rapids 1500.0 mm/min)\n\n(Z_Cut: -1.0 mm)\n(DepthPerCut: 0.1 mm <=>10 passes)\n(Z_Move: 2.0 mm)\n(Z Start: None mm)\n(Z End: 19.0 mm)\n(X,Y End: 0.0000, 0.0000 mm)\n(Steps per circle: 64)\n(Preprocessor Geometry: default)\n\n(X range:    0.0000 ...   10.0000  mm)\n(Y range:    0.0000 ...    5.0000  mm)\n\n(Spindle Speed: 1000.0 RPM)\nG21\nG90\nG94\n\nG01 F100.00\n\nM5\nG00 Z19.0000\nG00 X0.0000 Y0.0000\nT1\nM6    \n(MSG, Change to Tool Dia = 1.2000)\nM0\nG00 Z19.0000\n\nM03 S1000.0\nG01 F100.00\nG00 X0.0000 Y0.0000\nG01 F100.00\nG01 Z-0.1000\nG01 F100.00\nG01 X10.0000 Y0.0000\nG01 X10.0000 Y5.0000\nG00 X10.0000 Y5.0000\nG01 F100.00\nG01 Z-0.2000\nG01 F100.00\nG01 X10.0000 Y0.0000\nG01 X0.0000 Y0.0000\nG00 X0.0000 Y0.0000\nG01 F100.00\nG01 Z-0.3000\nG01 F100.00\nG01 X10.0000 Y0.0000\nG01 X10.0000 Y5.0000\nG00 X10.0000 Y5.0000\nG01 F100.00\nG01 Z-0.4000\nG01 F100.00\nG01 X10.0000 Y0.0000\nG01 X0.0000 Y0.0000\nG00 X0.0000 Y0.0000\nG01 F100.00\nG01 Z-0.5000\nG01 F100.00\nG01 X10.0000 Y0.0000\nG01 X10.0000 Y5.0000\nG00 X10.0000 Y5.0000\nG01 F100.00\nG01 Z-0.6000\nG01 F100.00\nG01 X10.0000 Y0.0000\nG01 X0.0000 Y0.0000\nG00 X0.0000 Y0.0000\nG01 F100.00\nG01 Z-0.7000\nG01 F100.00\nG01 X10.0000 Y0.0000\nG01 X10.0000 Y5.0000\nG00 X10.0000 Y5.0000\nG01 F100.00\nG01 Z-0.8000\nG01 F100.00\nG01 X10.0000 Y0.0000\nG01 X0.0000 Y0.0000\nG00 X0.0000 Y0.0000\nG01 F100.00\nG01 Z-0.9000\nG01 F100.00\nG01 X10.0000 Y0.0000\nG01 X10.0000 Y5.0000\nG00 X10.0000 Y5.0000\nG01 F100.00\nG01 Z-1.0000\nG01 F100.00\nG01 X10.0000 Y0.0000\nG01 X0.0000 Y0.0000\nG00 Z2.0000\nM05\nG00 Z2.0000\nG00 Z19.00\nG00 X0.0 Y0.0\n\n"},
{"multidepth_ring_and_hole", geometry2, settings2, "(This preprocessor is the default preprocessor used by FlatCAM.)\n(It is made to work with MACH3 compatible motion controllers.)\n\n(TOOL DIAMETER: 1.2 mm)\n(Feedrate_XY: 100.0 mm/min)\n(Feedrate_Z: 100.0 mm/min)\n(Feedrate rapids 1500.0 mm/min)\n\n(Z_Cut: -1.0 mm)\n(DepthPerCut: 0.4 mm <=>3 passes)\n(Z_Move: 2.0 mm)\n(Z Start: None mm)\n(Z End: 19.0 mm)\n(X,Y End: 0.0000, 0.0000 mm)\n(Steps per circle: 64)\n(Preprocessor Geometry: default)\n\n(X range:    0.0000 ...   10.0000  mm)\n(Y range:    0.0000 ...   10.0000  mm)\n\n(Spindle Speed: 1000.0 RPM)\nG21\nG90\nG94\n\nG01 F100.00\n\nM5\nG00 Z19.0000\nG00 X0.0000 Y0.0000\nT1\nM6    \n(MSG, Change to Tool Dia = 1.2000)\nM0\nG00 Z19.0000\n\nM03 S1000.0\nG01 F100.00\nG00 X0.0000 Y0.0000\nG01 F100.00\nG01 Z-0.4000\nG01 F100.00\nG01 X10.0000 Y0.0000\nG01 X10.0000 Y10.0000\nG01 X0.0000 Y10.0000\nG01 X0.0000 Y0.0000\nG00 X0.0000 Y0.0000\nG01 F100.00\nG01 Z-0.8000\nG01 F100.00\nG01 X10.0000 Y0.0000\nG01 X10.0000 Y10.0000\nG01 X0.0000 Y10.0000\nG01 X0.0000 Y0.0000\nG00 X0.0000 Y0.0000\nG01 F100.00\nG01 Z-1.0000\nG01 F100.00\nG01 X10.0000 Y0.0000\nG01 X10.0000 Y10.0000\nG01 X0.0000 Y10.0000\nG01 X0.0000 Y0.0000\nG00 Z2.0000\nG00 X2.0000 Y2.0000\nG01 F100.00\nG01 Z-0.4000\nG01 F100.00\nG01 X4.0000 Y2.0000\nG01 X4.0000 Y4.0000\nG01 X2.0000 Y4.0000\nG01 X2.0000 Y2.0000\nG00 X2.0000 Y2.0000\nG01 F100.00\nG01 Z-0.8000\nG01 F100.00\nG01 X4.0000 Y2.0000\nG01 X4.0000 Y4.0000\nG01 X2.0000 Y4.0000\nG01 X2.0000 Y2.0000\nG00 X2.0000 Y2.0000\nG01 F100.00\nG01 Z-1.0000\nG01 F100.00\nG01 X4.0000 Y2.0000\nG01 X4.0000 Y4.0000\nG01 X2.0000 Y4.0000\nG01 X2.0000 Y2.0000\nG00 Z2.0000\nM05\nG00 Z2.0000\nG00 Z19.00\nG00 X0.0 Y0.0\n\n"},
{"multidepth_closed_linestring", geometry3, settings3, "(This preprocessor is the default preprocessor used by FlatCAM.)\n(It is made to work with MACH3 compatible motion controllers.)\n\n(TOOL DIAMETER: 1.2 mm)\n(Feedrate_XY: 100.0 mm/min)\n(Feedrate_Z: 100.0 mm/min)\n(Feedrate rapids 1500.0 mm/min)\n\n(Z_Cut: -1.5 mm)\n(DepthPerCut: 0.5 mm <=>3 passes)\n(Z_Move: 2.0 mm)\n(Z Start: None mm)\n(Z End: 19.0 mm)\n(X,Y End: 0.0000, 0.0000 mm)\n(Steps per circle: 64)\n(Preprocessor Geometry: default)\n\n(X range:    0.0000 ...    6.0000  mm)\n(Y range:    0.0000 ...    6.0000  mm)\n\n(Spindle Speed: 1000.0 RPM)\nG21\nG90\nG94\n\nG01 F100.00\n\nM5\nG00 Z19.0000\nG00 X0.0000 Y0.0000\nT1\nM6    \n(MSG, Change to Tool Dia = 1.2000)\nM0\nG00 Z19.0000\n\nM03 S1000.0\nG01 F100.00\nG00 X0.0000 Y0.0000\nG01 F100.00\nG01 Z-0.5000\nG01 F100.00\nG01 X6.0000 Y0.0000\nG01 X6.0000 Y6.0000\nG01 X0.0000 Y6.0000\nG01 X0.0000 Y0.0000\nG00 X0.0000 Y0.0000\nG01 F100.00\nG01 Z-1.0000\nG01 F100.00\nG01 X0.0000 Y6.0000\nG01 X6.0000 Y6.0000\nG01 X6.0000 Y0.0000\nG01 X0.0000 Y0.0000\nG00 X0.0000 Y0.0000\nG01 F100.00\nG01 Z-1.5000\nG01 F100.00\nG01 X6.0000 Y0.0000\nG01 X6.0000 Y6.0000\nG01 X0.0000 Y6.0000\nG01 X0.0000 Y0.0000\nG00 Z2.0000\nM05\nG00 Z2.0000\nG00 Z19.00\nG00 X0.0 Y0.0\n\n"},
{"multidepth_points", geometry4, settings4, "(This preprocessor is the default preprocessor used by FlatCAM.)\n(It is made to work with MACH3 compatible motion controllers.)\n\n(TOOL DIAMETER: 1.2 mm)\n(Feedrate_XY: 100.0 mm/min)\n(Feedrate_Z: 100.0 mm/min)\n(Feedrate rapids 1500.0 mm/min)\n\n(Z_Cut: -2.0 mm)\n(DepthPerCut: 0.5 mm <=>4 passes)\n(Z_Move: 2.0 mm)\n(Z Start: None mm)\n(Z End: 19.0 mm)\n(X,Y End: 0.0000, 0.0000 mm)\n(Steps per circle: 64)\n(Preprocessor Geometry: default)\n\n(X range:    3.0000 ...    5.0000  mm)\n(Y range:    4.0000 ...    6.0000  mm)\n\n(Spindle Speed: 1000.0 RPM)\nG21\nG90\nG94\n\nG01 F100.00\n\nM5\nG00 Z19.0000\nG00 X0.0000 Y0.0000\nT1\nM6    \n(MSG, Change to Tool Dia = 1.2000)\nM0\nG00 Z19.0000\n\nM03 S1000.0\nG01 F100.00\nG00 X3.0000 Y4.0000\nG01 F100.00\nG01 Z-2.0000\nG01 F100.00\nG00 Z2.0000\nG00 Z2.0000\nG00 X5.0000 Y6.0000\nG01 F100.00\nG01 Z-2.0000\nG01 F100.00\nG00 Z2.0000\nG00 Z2.0000\nM05\nG00 Z2.0000\nG00 Z19.00\nG00 X0.0 Y0.0\n\n"},
{"single_points", geometry5, settings5, "(This preprocessor is the default preprocessor used by FlatCAM.)\n(It is made to work with MACH3 compatible motion controllers.)\n\n(TOOL DIAMETER: 1.2 mm)\n(Feedrate_XY: 100.0 mm/min)\n(Feedrate_Z: 100.0 mm/min)\n(Feedrate rapids 1500.0 mm/min)\n\n(Z_Cut: -1.2 mm)\n(Z_Move: 2.0 mm)\n(Z Start: None mm)\n(Z End: 19.0 mm)\n(X,Y End: 0.0000, 0.0000 mm)\n(Steps per circle: 64)\n(Preprocessor Geometry: default)\n\n(X range:    3.0000 ...    5.0000  mm)\n(Y range:    4.0000 ...    6.0000  mm)\n\n(Spindle Speed: 1000.0 RPM)\nG21\nG90\nG94\n\nG01 F100.00\n\nM5\nG00 Z19.0000\nG00 X0.0000 Y0.0000\nT1\nM6    \n(MSG, Change to Tool Dia = 1.2000)\nM0\nG00 Z19.0000\n\nM03 S1000.0\nG01 F100.00\nG00 X3.0000 Y4.0000\nG01 F100.00\nG01 Z-1.2000\nG01 F100.00\nG00 Z2.0000\nG00 X5.0000 Y6.0000\nG01 F100.00\nG01 Z-1.2000\nG01 F100.00\nG00 Z2.0000\nM05\nG00 Z2.0000\nG00 Z19.00\nG00 X0.0 Y0.0\n\n"},
{"single_two_lines", geometry6, settings6, "(This preprocessor is the default preprocessor used by FlatCAM.)\n(It is made to work with MACH3 compatible motion controllers.)\n\n(TOOL DIAMETER: 1.2 mm)\n(Feedrate_XY: 100.0 mm/min)\n(Feedrate_Z: 100.0 mm/min)\n(Feedrate rapids 1500.0 mm/min)\n\n(Z_Cut: -0.5 mm)\n(Z_Move: 2.0 mm)\n(Z Start: None mm)\n(Z End: 19.0 mm)\n(X,Y End: 0.0000, 0.0000 mm)\n(Steps per circle: 64)\n(Preprocessor Geometry: default)\n\n(X range:    0.0000 ...   25.0000  mm)\n(Y range:    0.0000 ...    5.0000  mm)\n\n(Spindle Speed: 1000.0 RPM)\nG21\nG90\nG94\n\nG01 F100.00\n\nM5\nG00 Z19.0000\nG00 X0.0000 Y0.0000\nT1\nM6    \n(MSG, Change to Tool Dia = 1.2000)\nM0\nG00 Z19.0000\n\nM03 S1000.0\nG01 F100.00\nG00 X0.0000 Y0.0000\nG01 F100.00\nG01 Z-0.5000\nG01 F100.00\nG01 X10.0000 Y0.0000\nG01 X10.0000 Y5.0000\nG00 Z2.0000\nG00 X20.0000 Y0.0000\nG01 F100.00\nG01 Z-0.5000\nG01 F100.00\nG01 X25.0000 Y5.0000\nG00 Z2.0000\nM05\nG00 Z2.0000\nG00 Z19.00\nG00 X0.0 Y0.0\n\n"},
{"extracut_default_length", geometry7, settings7, "(This preprocessor is the default preprocessor used by FlatCAM.)\n(It is made to work with MACH3 compatible motion controllers.)\n\n(TOOL DIAMETER: 1.2 mm)\n(Feedrate_XY: 100.0 mm/min)\n(Feedrate_Z: 100.0 mm/min)\n(Feedrate rapids 1500.0 mm/min)\n\n(Z_Cut: -1.0 mm)\n(Z_Move: 2.0 mm)\n(Z Start: None mm)\n(Z End: 19.0 mm)\n(X,Y End: 0.0000, 0.0000 mm)\n(Steps per circle: 64)\n(Preprocessor Geometry: default)\n\n(X range:    0.0000 ...   10.0000  mm)\n(Y range:    0.0000 ...   10.0000  mm)\n\n(Spindle Speed: 1000.0 RPM)\nG21\nG90\nG94\n\nG01 F100.00\n\nM5\nG00 Z19.0000\nG00 X0.0000 Y0.0000\nT1\nM6    \n(MSG, Change to Tool Dia = 1.2000)\nM0\nG00 Z19.0000\n\nM03 S1000.0\nG01 F100.00\nG00 X0.0000 Y0.0000\nG01 F100.00\nG01 Z-1.0000\nG01 F100.00\nG01 X10.0000 Y0.0000\nG01 X10.0000 Y10.0000\nG01 X0.0000 Y10.0000\nG01 X0.0000 Y0.0000\nG00 Z2.0000\nG00 X0.0000 Y0.2500\nG01 F100.00\nG01 Z-1.0000\nG01 F100.00\nG01 X0.0000 Y0.0000\nG01 X0.2500 Y0.0000\nG01 X0.0000 Y0.0000\nG00 Z2.0000\nM05\nG00 Z2.0000\nG00 Z19.00\nG00 X0.0 Y0.0\n\n"},
{"extracut_zero_length", geometry8, settings8, "(This preprocessor is the default preprocessor used by FlatCAM.)\n(It is made to work with MACH3 compatible motion controllers.)\n\n(TOOL DIAMETER: 1.2 mm)\n(Feedrate_XY: 100.0 mm/min)\n(Feedrate_Z: 100.0 mm/min)\n(Feedrate rapids 1500.0 mm/min)\n\n(Z_Cut: -1.0 mm)\n(Z_Move: 2.0 mm)\n(Z Start: None mm)\n(Z End: 19.0 mm)\n(X,Y End: 0.0000, 0.0000 mm)\n(Steps per circle: 64)\n(Preprocessor Geometry: default)\n\n(X range:    0.0000 ...   10.0000  mm)\n(Y range:    0.0000 ...   10.0000  mm)\n\n(Spindle Speed: 1000.0 RPM)\nG21\nG90\nG94\n\nG01 F100.00\n\nM5\nG00 Z19.0000\nG00 X0.0000 Y0.0000\nT1\nM6    \n(MSG, Change to Tool Dia = 1.2000)\nM0\nG00 Z19.0000\n\nM03 S1000.0\nG01 F100.00\nG00 X0.0000 Y0.0000\nG01 F100.00\nG01 Z-1.0000\nG01 F100.00\nG01 X10.0000 Y0.0000\nG01 X10.0000 Y10.0000\nG01 X0.0000 Y10.0000\nG01 X0.0000 Y0.0000\nG00 Z2.0000\nG00 X0.0000 Y0.0000\nG01 F100.00\nG01 Z-1.0000\nG01 F100.00\nG01 X0.0000 Y0.0000\nG01 X10.0000 Y0.0000\nG01 X0.0000 Y0.0000\nG00 Z2.0000\nM05\nG00 Z2.0000\nG00 Z19.00\nG00 X0.0 Y0.0\n\n"},
{"extracut_longer_than_ring", geometry9, settings9, "(This preprocessor is the default preprocessor used by FlatCAM.)\n(It is made to work with MACH3 compatible motion controllers.)\n\n(TOOL DIAMETER: 1.2 mm)\n(Feedrate_XY: 100.0 mm/min)\n(Feedrate_Z: 100.0 mm/min)\n(Feedrate rapids 1500.0 mm/min)\n\n(Z_Cut: -1.0 mm)\n(Z_Move: 2.0 mm)\n(Z Start: None mm)\n(Z End: 19.0 mm)\n(X,Y End: 0.0000, 0.0000 mm)\n(Steps per circle: 64)\n(Preprocessor Geometry: default)\n\n(X range:    0.0000 ...   10.0000  mm)\n(Y range:    0.0000 ...   10.0000  mm)\n\n(Spindle Speed: 1000.0 RPM)\nG21\nG90\nG94\n\nG01 F100.00\n\nM5\nG00 Z19.0000\nG00 X0.0000 Y0.0000\nT1\nM6    \n(MSG, Change to Tool Dia = 1.2000)\nM0\nG00 Z19.0000\n\nM03 S1000.0\nG01 F100.00\nG00 X0.0000 Y0.0000\nG01 F100.00\nG01 Z-1.0000\nG01 F100.00\nG01 X10.0000 Y0.0000\nG01 X10.0000 Y10.0000\nG01 X0.0000 Y10.0000\nG01 X0.0000 Y0.0000\nG00 Z2.0000\nG00 X10.0000 Y10.0000\nG01 F100.00\nG01 Z-1.0000\nG01 F100.00\nG01 X0.0000 Y10.0000\nG01 X0.0000 Y0.0000\nG01 X10.0000 Y0.0000\nG01 X10.0000 Y10.0000\nG01 X10.0000 Y0.0000\nG01 X0.0000 Y0.0000\nG00 Z2.0000\nM05\nG00 Z2.0000\nG00 Z19.00\nG00 X0.0 Y0.0\n\n"},
{"extracut_with_multidepth", geometry10, settings10, "(This preprocessor is the default preprocessor used by FlatCAM.)\n(It is made to work with MACH3 compatible motion controllers.)\n\n(TOOL DIAMETER: 1.2 mm)\n(Feedrate_XY: 100.0 mm/min)\n(Feedrate_Z: 100.0 mm/min)\n(Feedrate rapids 1500.0 mm/min)\n\n(Z_Cut: -1.0 mm)\n(DepthPerCut: 0.5 mm <=>2 passes)\n(Z_Move: 2.0 mm)\n(Z Start: None mm)\n(Z End: 19.0 mm)\n(X,Y End: 0.0000, 0.0000 mm)\n(Steps per circle: 64)\n(Preprocessor Geometry: default)\n\n(X range:    0.0000 ...   10.0000  mm)\n(Y range:    0.0000 ...   10.0000  mm)\n\n(Spindle Speed: 1000.0 RPM)\nG21\nG90\nG94\n\nG01 F100.00\n\nM5\nG00 Z19.0000\nG00 X0.0000 Y0.0000\nT1\nM6    \n(MSG, Change to Tool Dia = 1.2000)\nM0\nG00 Z19.0000\n\nM03 S1000.0\nG01 F100.00\nG00 X0.0000 Y0.0000\nG01 F100.00\nG01 Z-0.5000\nG01 F100.00\nG01 X10.0000 Y0.0000\nG01 X10.0000 Y10.0000\nG01 X0.0000 Y10.0000\nG01 X0.0000 Y0.0000\nG00 Z2.0000\nG00 X0.0000 Y0.5000\nG01 F100.00\nG01 Z-0.5000\nG01 F100.00\nG01 X0.0000 Y0.0000\nG01 X0.5000 Y0.0000\nG01 X0.0000 Y0.0000\nG00 X0.0000 Y0.0000\nG01 F100.00\nG01 Z-1.0000\nG01 F100.00\nG01 X10.0000 Y0.0000\nG01 X10.0000 Y10.0000\nG01 X0.0000 Y10.0000\nG01 X0.0000 Y0.0000\nG00 Z2.0000\nG00 X0.0000 Y0.5000\nG01 F100.00\nG01 Z-1.0000\nG01 F100.00\nG01 X0.0000 Y0.0000\nG01 X0.5000 Y0.0000\nG01 X0.0000 Y0.0000\nG00 Z2.0000\nM05\nG00 Z2.0000\nG00 Z19.00\nG00 X0.0 Y0.0\n\n"},
{"extracut_ignored_for_open_line", geometry11, settings11, "(This preprocessor is the default preprocessor used by FlatCAM.)\n(It is made to work with MACH3 compatible motion controllers.)\n\n(TOOL DIAMETER: 1.2 mm)\n(Feedrate_XY: 100.0 mm/min)\n(Feedrate_Z: 100.0 mm/min)\n(Feedrate rapids 1500.0 mm/min)\n\n(Z_Cut: -1.0 mm)\n(Z_Move: 2.0 mm)\n(Z Start: None mm)\n(Z End: 19.0 mm)\n(X,Y End: 0.0000, 0.0000 mm)\n(Steps per circle: 64)\n(Preprocessor Geometry: default)\n\n(X range:    0.0000 ...   10.0000  mm)\n(Y range:    0.0000 ...    5.0000  mm)\n\n(Spindle Speed: 1000.0 RPM)\nG21\nG90\nG94\n\nG01 F100.00\n\nM5\nG00 Z19.0000\nG00 X0.0000 Y0.0000\nT1\nM6    \n(MSG, Change to Tool Dia = 1.2000)\nM0\nG00 Z19.0000\n\nM03 S1000.0\nG01 F100.00\nG00 X0.0000 Y0.0000\nG01 F100.00\nG01 Z-1.0000\nG01 F100.00\nG01 X10.0000 Y0.0000\nG01 X10.0000 Y5.0000\nG00 Z2.0000\nM05\nG00 Z2.0000\nG00 Z19.00\nG00 X0.0 Y0.0\n\n"},
{"tolerance_simplifies", geometry12, settings12, "(This preprocessor is the default preprocessor used by FlatCAM.)\n(It is made to work with MACH3 compatible motion controllers.)\n\n(TOOL DIAMETER: 1.2 mm)\n(Feedrate_XY: 100.0 mm/min)\n(Feedrate_Z: 100.0 mm/min)\n(Feedrate rapids 1500.0 mm/min)\n\n(Z_Cut: -0.5 mm)\n(Z_Move: 2.0 mm)\n(Z Start: None mm)\n(Z End: 19.0 mm)\n(X,Y End: 0.0000, 0.0000 mm)\n(Steps per circle: 64)\n(Preprocessor Geometry: default)\n\n(X range:    0.0000 ...   10.0000  mm)\n(Y range:   -0.0200 ...    0.0200  mm)\n\n(Spindle Speed: 1000.0 RPM)\nG21\nG90\nG94\n\nG01 F100.00\n\nM5\nG00 Z19.0000\nG00 X0.0000 Y0.0000\nT1\nM6    \n(MSG, Change to Tool Dia = 1.2000)\nM0\nG00 Z19.0000\n\nM03 S1000.0\nG01 F100.00\nG00 X0.0000 Y0.0000\nG01 F100.00\nG01 Z-0.5000\nG01 F100.00\nG01 X10.0000 Y0.0000\nG00 Z2.0000\nM05\nG00 Z2.0000\nG00 Z19.00\nG00 X0.0 Y0.0\n\n"},
{"segmentation", geometry13, settings13, "(This preprocessor is the default preprocessor used by FlatCAM.)\n(It is made to work with MACH3 compatible motion controllers.)\n\n(TOOL DIAMETER: 1.2 mm)\n(Feedrate_XY: 100.0 mm/min)\n(Feedrate_Z: 100.0 mm/min)\n(Feedrate rapids 1500.0 mm/min)\n\n(Z_Cut: -0.5 mm)\n(Z_Move: 2.0 mm)\n(Z Start: None mm)\n(Z End: 19.0 mm)\n(X,Y End: 0.0000, 0.0000 mm)\n(Steps per circle: 64)\n(Preprocessor Geometry: default)\n\n(X range:    0.0000 ...    9.0000  mm)\n(Y range:    0.0000 ...    7.0000  mm)\n\n(Spindle Speed: 1000.0 RPM)\nG21\nG90\nG94\n\nG01 F100.00\n\nM5\nG00 Z19.0000\nG00 X0.0000 Y0.0000\nT1\nM6    \n(MSG, Change to Tool Dia = 1.2000)\nM0\nG00 Z19.0000\n\nM03 S1000.0\nG01 F100.00\nG00 X0.0000 Y0.0000\nG01 F100.00\nG01 Z-0.5000\nG01 F100.00\nG01 X2.0000 Y0.0000\nG01 X4.0000 Y0.0000\nG01 X6.0000 Y0.0000\nG01 X7.5000 Y0.0000\nG01 X9.0000 Y0.0000\nG01 X9.0000 Y1.5000\nG01 X9.0000 Y3.0000\nG01 X9.0000 Y4.5000\nG01 X9.0000 Y5.7500\nG01 X9.0000 Y7.0000\nG00 Z2.0000\nM05\nG00 Z2.0000\nG00 Z19.00\nG00 X0.0 Y0.0\n\n"},
{"segmentation_x_only", geometry14, settings14, "(This preprocessor is the default preprocessor used by FlatCAM.)\n(It is made to work with MACH3 compatible motion controllers.)\n\n(TOOL DIAMETER: 1.2 mm)\n(Feedrate_XY: 100.0 mm/min)\n(Feedrate_Z: 100.0 mm/min)\n(Feedrate rapids 1500.0 mm/min)\n\n(Z_Cut: -0.5 mm)\n(Z_Move: 2.0 mm)\n(Z Start: None mm)\n(Z End: 19.0 mm)\n(X,Y End: 0.0000, 0.0000 mm)\n(Steps per circle: 64)\n(Preprocessor Geometry: default)\n\n(X range:    0.0000 ...    9.0000  mm)\n(Y range:    0.0000 ...    7.0000  mm)\n\n(Spindle Speed: 1000.0 RPM)\nG21\nG90\nG94\n\nG01 F100.00\n\nM5\nG00 Z19.0000\nG00 X0.0000 Y0.0000\nT1\nM6    \n(MSG, Change to Tool Dia = 1.2000)\nM0\nG00 Z19.0000\n\nM03 S1000.0\nG01 F100.00\nG00 X0.0000 Y0.0000\nG01 F100.00\nG01 Z-0.5000\nG01 F100.00\nG01 X2.5000 Y0.0000\nG01 X5.0000 Y0.0000\nG01 X7.0000 Y0.0000\nG01 X9.0000 Y0.0000\nG01 X9.0000 Y7.0000\nG00 Z2.0000\nM05\nG00 Z2.0000\nG00 Z19.00\nG00 X0.0 Y0.0\n\n"},
{"no_toolchange_with_startz", geometry15, settings15, "(This preprocessor is the default preprocessor used by FlatCAM.)\n(It is made to work with MACH3 compatible motion controllers.)\n\n(TOOL DIAMETER: 1.2 mm)\n(Feedrate_XY: 100.0 mm/min)\n(Feedrate_Z: 100.0 mm/min)\n(Feedrate rapids 1500.0 mm/min)\n\n(Z_Cut: -0.5 mm)\n(Z_Move: 2.0 mm)\n(Z Start: 1.5 mm)\n(Z End: 19.0 mm)\n(X,Y End: 0.0000, 0.0000 mm)\n(Steps per circle: 64)\n(Preprocessor Geometry: default)\n\n(X range:    0.0000 ...   10.0000  mm)\n(Y range:    0.0000 ...    5.0000  mm)\n\n(Spindle Speed: 1000.0 RPM)\nG21\nG90\nG94\n\nG01 F100.00\nG00 Z2.0000\nG00 Z1.5000\nM03 S1000.0\nG4 P2.0\nG01 F100.00\nG00 X0.0000 Y0.0000\nG01 F100.00\nG01 Z-0.5000\nG01 F100.00\nG01 X10.0000 Y0.0000\nG01 X10.0000 Y5.0000\nG00 Z2.0000\nM05\nG00 Z2.0000\nG00 Z19.00\nG00 X0.0 Y0.0\n\n"},
{"laser_single_pass", geometry16, settings16, "(This preprocessor is used with a motion controller loaded with GRBL firmware. )\n(It is for the case when it is used together with a LASER connected on the SPINDLE connector.)\n\n(Feedrate: 500.0 mm/min)\n(Feedrate rapids: 1500.0 mm/min)\n\n(Z Focus: 2.0 mm)\n(Steps per circle: 64)\n(Preprocessor Geometry: GRBL_laser)\n\n(X range:    0.0000 ...   10.0000  mm)\n(Y range:    0.0000 ...    5.0000  mm)\n\n(Laser Power - Spindle Speed: 300.0)\n\nG21\nG90\nG17\nG94\n\nG01 F500.00\n\nM5\nG01 F500.00\nG00 X0.0000 Y0.0000\nG01 F100.00\nM03 S300.0\nG01 F500.00\nG01 X10.0000 Y0.0000 F500.00\nG01 X10.0000 Y5.0000 F500.00\nM5\nM5\nM5\nG00 Z19.00\nG00 X0.0 Y0.0\n\n"},
{"laser_no_toolchange", geometry17, settings17, "(This preprocessor is used with a motion controller loaded with GRBL firmware. )\n(It is for the case when it is used together with a LASER connected on the SPINDLE connector.)\n\n(Feedrate: 500.0 mm/min)\n(Feedrate rapids: 1500.0 mm/min)\n\n(Z Focus: 2.0 mm)\n(Steps per circle: 64)\n(Preprocessor Geometry: GRBL_laser)\n\n(X range:    0.0000 ...   10.0000  mm)\n(Y range:    0.0000 ...    5.0000  mm)\n\n(Laser Power - Spindle Speed: 300.0)\n\nG21\nG90\nG17\nG94\n\nG01 F500.00\nM5\n\nG01 F500.00\nG00 X0.0000 Y0.0000\nG01 F100.00\nM03 S300.0\nG01 F500.00\nG01 X10.0000 Y0.0000 F500.00\nG01 X10.0000 Y5.0000 F500.00\nM5\nM5\nM5\nG00 Z19.00\nG00 X0.0 Y0.0\n\n"},
{"inches", geometry18, settings18, "(This preprocessor is the default preprocessor used by FlatCAM.)\n(It is made to work with MACH3 compatible motion controllers.)\n\n(TOOL DIAMETER: 1.2 in)\n(Feedrate_XY: 100.0 in/min)\n(Feedrate_Z: 100.0 in/min)\n(Feedrate rapids 1500.0 in/min)\n\n(Z_Cut: -0.07 in)\n(DepthPerCut: 0.02 in <=>4 passes)\n(Z_Move: 0.08 in)\n(Z Start: None in)\n(Z End: 19.0 in)\n(X,Y End: 0.0000, 0.0000 in)\n(Steps per circle: 64)\n(Preprocessor Geometry: default)\n\n(X range:    0.0000 ...   10.0000  in)\n(Y range:    0.0000 ...    5.0000  in)\n\n(Spindle Speed: 1000.0 RPM)\nG20\nG90\nG94\n\nG01 F100.00\n\nM5\nG00 Z19.0000\nG00 X0.0000 Y0.0000\nT1\nM6    \n(MSG, Change to Tool Dia = 1.2000)\nM0\nG00 Z19.0000\n\nM03 S1000.0\nG01 F100.00\nG00 X0.0000 Y0.0000\nG01 F100.00\nG01 Z-0.0200\nG01 F100.00\nG01 X10.0000 Y0.0000\nG01 X10.0000 Y5.0000\nG00 X10.0000 Y5.0000\nG01 F100.00\nG01 Z-0.0400\nG01 F100.00\nG01 X10.0000 Y0.0000\nG01 X0.0000 Y0.0000\nG00 X0.0000 Y0.0000\nG01 F100.00\nG01 Z-0.0600\nG01 F100.00\nG01 X10.0000 Y0.0000\nG01 X10.0000 Y5.0000\nG00 X10.0000 Y5.0000\nG01 F100.00\nG01 Z-0.0700\nG01 F100.00\nG01 X10.0000 Y0.0000\nG01 X0.0000 Y0.0000\nG00 Z0.0800\nM05\nG00 Z0.0800\nG00 Z19.00\nG00 X0.0 Y0.0\n\n"},
{"offset_out_square", geometry19, settings19, "(This preprocessor is the default preprocessor used by FlatCAM.)\n(It is made to work with MACH3 compatible motion controllers.)\n\n(TOOL DIAMETER: 1.2 mm)\n(Feedrate_XY: 100.0 mm/min)\n(Feedrate_Z: 100.0 mm/min)\n(Feedrate rapids 1500.0 mm/min)\n\n(Z_Cut: -1.0 mm)\n(Z_Move: 2.0 mm)\n(Z Start: None mm)\n(Z End: 19.0 mm)\n(X,Y End: 0.0000, 0.0000 mm)\n(Steps per circle: 64)\n(Preprocessor Geometry: default)\n\n(X range:    0.0000 ...   10.0000  mm)\n(Y range:    0.0000 ...   10.0000  mm)\n\n(Spindle Speed: 1000.0 RPM)\nG21\nG90\nG94\n\nG01 F100.00\n\nM5\nG00 Z19.0000\nG00 X0.0000 Y0.0000\nT1\nM6    \n(MSG, Change to Tool Dia = 1.2000)\nM0\nG00 Z19.0000\n\nM03 S1000.0\nG01 F100.00\nG00 X-0.6000 Y-0.6000\nG01 F100.00\nG01 Z-1.0000\nG01 F100.00\nG01 X-0.6000 Y10.6000\nG01 X10.6000 Y10.6000\nG01 X10.6000 Y-0.6000\nG01 X-0.6000 Y-0.6000\nG00 Z2.0000\nM05\nG00 Z2.0000\nG00 Z19.00\nG00 X0.0 Y0.0\n\n"},
{"offset_in_square_with_hole", geometry20, settings20, "(This preprocessor is the default preprocessor used by FlatCAM.)\n(It is made to work with MACH3 compatible motion controllers.)\n\n(TOOL DIAMETER: 1.2 mm)\n(Feedrate_XY: 100.0 mm/min)\n(Feedrate_Z: 100.0 mm/min)\n(Feedrate rapids 1500.0 mm/min)\n\n(Z_Cut: -1.0 mm)\n(Z_Move: 2.0 mm)\n(Z Start: None mm)\n(Z End: 19.0 mm)\n(X,Y End: 0.0000, 0.0000 mm)\n(Steps per circle: 64)\n(Preprocessor Geometry: default)\n\n(X range:    0.0000 ...   10.0000  mm)\n(Y range:    0.0000 ...   10.0000  mm)\n\n(Spindle Speed: 1000.0 RPM)\nG21\nG90\nG94\n\nG01 F100.00\n\nM5\nG00 Z19.0000\nG00 X0.0000 Y0.0000\nT1\nM6    \n(MSG, Change to Tool Dia = 1.2000)\nM0\nG00 Z19.0000\n\nM03 S1000.0\nG01 F100.00\nG00 X0.6000 Y0.6000\nG01 F100.00\nG01 Z-1.0000\nG01 F100.00\nG01 X0.6000 Y9.4000\nG01 X9.4000 Y9.4000\nG01 X9.4000 Y0.6000\nG01 X0.6000 Y0.6000\nG00 Z2.0000\nG00 X2.6000 Y2.6000\nG01 F100.00\nG01 Z-1.0000\nG01 F100.00\nG01 X2.6000 Y3.4000\nG01 X3.4000 Y3.4000\nG01 X3.4000 Y2.6000\nG01 X2.6000 Y2.6000\nG00 Z2.0000\nM05\nG00 Z2.0000\nG00 Z19.00\nG00 X0.0 Y0.0\n\n"},
{"offset_custom_open_line", geometry21, settings21, "(This preprocessor is the default preprocessor used by FlatCAM.)\n(It is made to work with MACH3 compatible motion controllers.)\n\n(TOOL DIAMETER: 1.2 mm)\n(Feedrate_XY: 100.0 mm/min)\n(Feedrate_Z: 100.0 mm/min)\n(Feedrate rapids 1500.0 mm/min)\n\n(Z_Cut: -1.0 mm)\n(Z_Move: 2.0 mm)\n(Z Start: None mm)\n(Z End: 19.0 mm)\n(X,Y End: 0.0000, 0.0000 mm)\n(Steps per circle: 64)\n(Preprocessor Geometry: default)\n\n(X range:    0.0000 ...   10.0000  mm)\n(Y range:    0.0000 ...    5.0000  mm)\n\n(Spindle Speed: 1000.0 RPM)\nG21\nG90\nG94\n\nG01 F100.00\n\nM5\nG00 Z19.0000\nG00 X0.0000 Y0.0000\nT1\nM6    \n(MSG, Change to Tool Dia = 1.2000)\nM0\nG00 Z19.0000\n\nM03 S1000.0\nG01 F100.00\nG00 X9.7000 Y0.3000\nG01 F100.00\nG01 Z-1.0000\nG01 F100.00\nG01 X9.7000 Y5.0000\nG01 X9.7014 Y5.0294\nG01 X9.7058 Y5.0585\nG01 X9.7129 Y5.0871\nG01 X9.7228 Y5.1148\nG01 X9.7354 Y5.1414\nG01 X9.7506 Y5.1667\nG01 X9.7681 Y5.1903\nG01 X9.7879 Y5.2121\nG01 X9.8097 Y5.2319\nG01 X9.8333 Y5.2494\nG01 X9.8586 Y5.2646\nG01 X9.8852 Y5.2772\nG01 X9.9129 Y5.2871\nG01 X9.9415 Y5.2942\nG01 X9.9706 Y5.2986\nG01 X10.0000 Y5.3000\nG01 X10.0294 Y5.2986\nG01 X10.0585 Y5.2942\nG01 X10.0871 Y5.2871\nG01 X10.1148 Y5.2772\nG01 X10.1414 Y5.2646\nG01 X10.1667 Y5.2494\nG01 X10.1903 Y5.2319\nG01 X10.2121 Y5.2121\nG01 X10.2319 Y5.1903\nG01 X10.2494 Y5.1667\nG01 X10.2646 Y5.1414\nG01 X10.2772 Y5.1148\nG01 X10.2871 Y5.0871\nG01 X10.2942 Y5.0585\nG01 X10.2986 Y5.0294\nG01 X10.3000 Y5.0000\nG01 X10.3000 Y-0.3000\nG01 X0.0000 Y-0.3000\nG01 X-0.0294 Y-0.2986\nG01 X-0.0585 Y-0.2942\nG01 X-0.0871 Y-0.2871\nG01 X-0.1148 Y-0.2772\nG01 X-0.1414 Y-0.2646\nG01 X-0.1667 Y-0.2494\nG01 X-0.1903 Y-0.2319\nG01 X-0.2121 Y-0.2121\nG01 X-0.2319 Y-0.1903\nG01 X-0.2494 Y-0.1667\nG01 X-0.2646 Y-0.1414\nG01 X-0.2772 Y-0.1148\nG01 X-0.2871 Y-0.0871\nG01 X-0.2942 Y-0.0585\nG01 X-0.2986 Y-0.0294\nG01 X-0.3000 Y0.0000\nG01 X-0.2986 Y0.0294\nG01 X-0.2942 Y0.0585\nG01 X-0.2871 Y0.0871\nG01 X-0.2772 Y0.1148\nG01 X-0.2646 Y0.1414\nG01 X-0.2494 Y0.1667\nG01 X-0.2319 Y0.1903\nG01 X-0.2121 Y0.2121\nG01 X-0.1903 Y0.2319\nG01 X-0.1667 Y0.2494\nG01 X-0.1414 Y0.2646\nG01 X-0.1148 Y0.2772\nG01 X-0.0871 Y0.2871\nG01 X-0.0585 Y0.2942\nG01 X-0.0294 Y0.2986\nG01 X0.0000 Y0.3000\nG01 X9.7000 Y0.3000\nG00 Z2.0000\nM05\nG00 Z2.0000\nG00 Z19.00\nG00 X0.0 Y0.0\n\n"},
{"offset_out_closed_linestring", geometry22, settings22, "(This preprocessor is the default preprocessor used by FlatCAM.)\n(It is made to work with MACH3 compatible motion controllers.)\n\n(TOOL DIAMETER: 1.2 mm)\n(Feedrate_XY: 100.0 mm/min)\n(Feedrate_Z: 100.0 mm/min)\n(Feedrate rapids 1500.0 mm/min)\n\n(Z_Cut: -1.0 mm)\n(Z_Move: 2.0 mm)\n(Z Start: None mm)\n(Z End: 19.0 mm)\n(X,Y End: 0.0000, 0.0000 mm)\n(Steps per circle: 64)\n(Preprocessor Geometry: default)\n\n(X range:    0.0000 ...    6.0000  mm)\n(Y range:    0.0000 ...    6.0000  mm)\n\n(Spindle Speed: 1000.0 RPM)\nG21\nG90\nG94\n\nG01 F100.00\n\nM5\nG00 Z19.0000\nG00 X0.0000 Y0.0000\nT1\nM6    \n(MSG, Change to Tool Dia = 1.2000)\nM0\nG00 Z19.0000\n\nM03 S1000.0\nG01 F100.00\nG00 X-0.6000 Y-0.6000\nG01 F100.00\nG01 Z-1.0000\nG01 F100.00\nG01 X-0.6000 Y6.6000\nG01 X6.6000 Y6.6000\nG01 X6.6000 Y-0.6000\nG01 X-0.6000 Y-0.6000\nG00 Z2.0000\nM05\nG00 Z2.0000\nG00 Z19.00\nG00 X0.0 Y0.0\n\n"},
{"offset_out_two_shapes", geometry23, settings23, "(This preprocessor is the default preprocessor used by FlatCAM.)\n(It is made to work with MACH3 compatible motion controllers.)\n\n(TOOL DIAMETER: 1.2 mm)\n(Feedrate_XY: 100.0 mm/min)\n(Feedrate_Z: 100.0 mm/min)\n(Feedrate rapids 1500.0 mm/min)\n\n(Z_Cut: -1.0 mm)\n(Z_Move: 2.0 mm)\n(Z Start: None mm)\n(Z End: 19.0 mm)\n(X,Y End: 0.0000, 0.0000 mm)\n(Steps per circle: 64)\n(Preprocessor Geometry: default)\n\n(X range:    0.0000 ...   28.0000  mm)\n(Y range:    0.0000 ...   10.0000  mm)\n\n(Spindle Speed: 1000.0 RPM)\nG21\nG90\nG94\n\nG01 F100.00\n\nM5\nG00 Z19.0000\nG00 X0.0000 Y0.0000\nT1\nM6    \n(MSG, Change to Tool Dia = 1.2000)\nM0\nG00 Z19.0000\n\nM03 S1000.0\nG01 F100.00\nG00 X-0.6000 Y-0.6000\nG01 F100.00\nG01 Z-1.0000\nG01 F100.00\nG01 X-0.6000 Y10.6000\nG01 X10.6000 Y10.6000\nG01 X10.6000 Y-0.6000\nG01 X-0.6000 Y-0.6000\nG00 Z2.0000\nG00 X18.8789 Y-0.6000\nG01 F100.00\nG01 Z-1.0000\nG01 F100.00\nG01 X24.0000 Y7.0817\nG01 X29.1211 Y-0.6000\nG01 X18.8789 Y-0.6000\nG00 Z2.0000\nM05\nG00 Z2.0000\nG00 Z19.00\nG00 X0.0 Y0.0\n\n"},
{"offset_out_multidepth", geometry24, settings24, "(This preprocessor is the default preprocessor used by FlatCAM.)\n(It is made to work with MACH3 compatible motion controllers.)\n\n(TOOL DIAMETER: 1.2 mm)\n(Feedrate_XY: 100.0 mm/min)\n(Feedrate_Z: 100.0 mm/min)\n(Feedrate rapids 1500.0 mm/min)\n\n(Z_Cut: -1.0 mm)\n(DepthPerCut: 0.5 mm <=>2 passes)\n(Z_Move: 2.0 mm)\n(Z Start: None mm)\n(Z End: 19.0 mm)\n(X,Y End: 0.0000, 0.0000 mm)\n(Steps per circle: 64)\n(Preprocessor Geometry: default)\n\n(X range:    0.0000 ...   10.0000  mm)\n(Y range:    0.0000 ...   10.0000  mm)\n\n(Spindle Speed: 1000.0 RPM)\nG21\nG90\nG94\n\nG01 F100.00\n\nM5\nG00 Z19.0000\nG00 X0.0000 Y0.0000\nT1\nM6    \n(MSG, Change to Tool Dia = 1.2000)\nM0\nG00 Z19.0000\n\nM03 S1000.0\nG01 F100.00\nG00 X-0.6000 Y-0.6000\nG01 F100.00\nG01 Z-0.5000\nG01 F100.00\nG01 X-0.6000 Y10.6000\nG01 X10.6000 Y10.6000\nG01 X10.6000 Y-0.6000\nG01 X-0.6000 Y-0.6000\nG00 X-0.6000 Y-0.6000\nG01 F100.00\nG01 Z-1.0000\nG01 F100.00\nG01 X-0.6000 Y10.6000\nG01 X10.6000 Y10.6000\nG01 X10.6000 Y-0.6000\nG01 X-0.6000 Y-0.6000\nG00 Z2.0000\nM05\nG00 Z2.0000\nG00 Z19.00\nG00 X0.0 Y0.0\n\n"},
{"offset_custom_negative_ring", geometry25, settings25, "(This preprocessor is the default preprocessor used by FlatCAM.)\n(It is made to work with MACH3 compatible motion controllers.)\n\n(TOOL DIAMETER: 1.2 mm)\n(Feedrate_XY: 100.0 mm/min)\n(Feedrate_Z: 100.0 mm/min)\n(Feedrate rapids 1500.0 mm/min)\n\n(Z_Cut: -1.0 mm)\n(Z_Move: 2.0 mm)\n(Z Start: None mm)\n(Z End: 19.0 mm)\n(X,Y End: 0.0000, 0.0000 mm)\n(Steps per circle: 64)\n(Preprocessor Geometry: default)\n\n(X range:    0.0000 ...   10.0000  mm)\n(Y range:    0.0000 ...   10.0000  mm)\n\n(Spindle Speed: 1000.0 RPM)\nG21\nG90\nG94\n\nG01 F100.00\n\nM5\nG00 Z19.0000\nG00 X0.0000 Y0.0000\nT1\nM6    \n(MSG, Change to Tool Dia = 1.2000)\nM0\nG00 Z19.0000\n\nM03 S1000.0\nG01 F100.00\nG00 X0.4000 Y0.4000\nG01 F100.00\nG01 Z-1.0000\nG01 F100.00\nG01 X0.4000 Y9.6000\nG01 X9.6000 Y9.6000\nG01 X9.6000 Y0.4000\nG01 X0.4000 Y0.4000\nG00 Z2.0000\nM05\nG00 Z2.0000\nG00 Z19.00\nG00 X0.0 Y0.0\n\n"},
{"pp_Berta_CNC", geometry26, settings26, "(This preprocessor is used with a BERTA CNC router.)\n\n(TOOL DIAMETER: 1.2 mm)\n(Feedrate: 100.0 mm/min)\n(TOOL DIAMETER: 1.2 mm)\n(Feedrate_XY: 100.0 mm/min)\n(Feedrate_Z: 100.0 mm/min)\n(Feedrate rapids 1500.0 mm/min)\n\n(Z_Cut: -0.5 mm)\n(Z_Move: 2.0 mm)\n(Z Start: None mm)\n(Z End: 19.0 mm)\n(X,Y End: 0.0000, 0.0000 mm)\n(Steps per circle: 64)\n(Steps per circle: 64)\n(Preprocessor Geometry: Berta_CNC)\n\n(X range:    0.0000 ...   10.0000  mm)\n(Y range:    0.0000 ...    5.0000  mm)\n\n(Spindle Speed: 1000.0 RPM)\nG21\n\nG90 G17 G91.1\nG64 P0.03\nM110\nG54\nG0\n(Berta)\nG94\n\nG01 F100.00\n\nM5\nG00 Z19.0000\nG00 X0.0000 Y0.0000\nT1\nM6    \n(MSG, Change to Tool Dia = 1.2000)\nM0\nM03 S1000.0\nG4 P1.5\nG01 F100.00\nG00 X0.0000 Y0.0000\nG01 F100.00\nG01 Z-0.5000\nG01 F100.00\nG01 X10.0000 Y0.0000\nG01 X10.0000 Y5.0000\nG00 Z2.0000\nG00 X3.0000 Y4.0000\nG01 F100.00\nG01 Z-0.5000\nG01 F100.00\nG00 Z2.0000\nM05\nG00 Z2.0000\nG00 Z19.00\nG00 X0.0 Y0.0\n(Berta)\nM111\nM30\n(Berta)\n\n"},
{"pp_Berta_CNC_no_toolchange", geometry27, settings27, "(This preprocessor is used with a BERTA CNC router.)\n\n(TOOL DIAMETER: 1.2 mm)\n(Feedrate: 100.0 mm/min)\n(TOOL DIAMETER: 1.2 mm)\n(Feedrate_XY: 100.0 mm/min)\n(Feedrate_Z: 100.0 mm/min)\n(Feedrate rapids 1500.0 mm/min)\n\n(Z_Cut: -0.5 mm)\n(Z_Move: 2.0 mm)\n(Z Start: 1.0 mm)\n(Z End: 19.0 mm)\n(X,Y End: 0.0000, 0.0000 mm)\n(Steps per circle: 64)\n(Steps per circle: 64)\n(Preprocessor Geometry: Berta_CNC)\n\n(X range:    0.0000 ...   10.0000  mm)\n(Y range:    0.0000 ...    5.0000  mm)\n\n(Spindle Speed: 1000.0 RPM)\nG21\n\nG90 G17 G91.1\nG64 P0.03\nM110\nG54\nG0\n(Berta)\nG94\n\nG01 F100.00\nG00 Z2.0000\nG00 Z1.0000\nM03 S1000.0\nG01 F100.00\nG00 X0.0000 Y0.0000\nG01 F100.00\nG01 Z-0.5000\nG01 F100.00\nG01 X10.0000 Y0.0000\nG01 X10.0000 Y5.0000\nG00 Z2.0000\nM05\nG00 Z2.0000\nG00 Z19.00\nG00 X0.0 Y0.0\n(Berta)\nM111\nM30\n(Berta)\n\n"},
{"pp_Default_no_M6", geometry28, settings28, "(This preprocessor is the default preprocessor used by FlatCAM.)\n(It is made to work with MACH3 compatible motion controllers.)\n\n(TOOL DIAMETER: 1.2 mm)\n(Feedrate_XY: 100.0 mm/min)\n(Feedrate_Z: 100.0 mm/min)\n(Feedrate rapids 1500.0 mm/min)\n\n(Z_Cut: -0.5 mm)\n(Z_Move: 2.0 mm)\n(Z Start: None mm)\n(Z End: 19.0 mm)\n(X,Y End: 0.0000, 0.0000 mm)\n(Steps per circle: 64)\n(Preprocessor Geometry: Default_no_M6)\n\n(X range:    0.0000 ...   10.0000  mm)\n(Y range:    0.0000 ...    5.0000  mm)\n\n(Spindle Speed: 1000.0 RPM)\nG21\nG90\nG94\n\nG01 F100.00\n\nM5\nG00 Z19.0000\nG00 X0.0000 Y0.0000\nT1\n(MSG, Change to Tool Dia = 1.2000)\nM0\nG00 Z19.0000\n		\nM03 S1000.0\nG4 P1.5\nG01 F100.00\nG00 X0.0000 Y0.0000\nG01 F100.00\nG01 Z-0.5000\nG01 F100.00\nG01 X10.0000 Y0.0000\nG01 X10.0000 Y5.0000\nG00 Z2.0000\nG00 X3.0000 Y4.0000\nG01 F100.00\nG01 Z-0.5000\nG01 F100.00\nG00 Z2.0000\nM05\nG00 Z2.0000\nG00 Z19.00\nG00 X0.0 Y0.0\n\n"},
{"pp_Default_no_M6_no_toolchange", geometry29, settings29, "(This preprocessor is the default preprocessor used by FlatCAM.)\n(It is made to work with MACH3 compatible motion controllers.)\n\n(TOOL DIAMETER: 1.2 mm)\n(Feedrate_XY: 100.0 mm/min)\n(Feedrate_Z: 100.0 mm/min)\n(Feedrate rapids 1500.0 mm/min)\n\n(Z_Cut: -0.5 mm)\n(Z_Move: 2.0 mm)\n(Z Start: 1.0 mm)\n(Z End: 19.0 mm)\n(X,Y End: 0.0000, 0.0000 mm)\n(Steps per circle: 64)\n(Preprocessor Geometry: Default_no_M6)\n\n(X range:    0.0000 ...   10.0000  mm)\n(Y range:    0.0000 ...    5.0000  mm)\n\n(Spindle Speed: 1000.0 RPM)\nG21\nG90\nG94\n\nG01 F100.00\nG00 Z2.0000\nG00 Z1.0000\nM03 S1000.0\nG01 F100.00\nG00 X0.0000 Y0.0000\nG01 F100.00\nG01 Z-0.5000\nG01 F100.00\nG01 X10.0000 Y0.0000\nG01 X10.0000 Y5.0000\nG00 Z2.0000\nM05\nG00 Z2.0000\nG00 Z19.00\nG00 X0.0 Y0.0\n\n"},
{"pp_grbl_11", geometry30, settings30, "(This preprocessor is used with a motion controller loaded with GRBL firmware.)\n(It is configured to be compatible with almost any version of GRBL firmware.)\n\n(TOOL DIAMETER: 1.2 mm)\n(Feedrate_XY: 100.0 mm/min)\n(Feedrate_Z: 100.0 mm/min)\n(Feedrate rapids 1500.0 mm/min)\n\n(Z_Cut: -0.5 mm)\n(Z_Move: 2.0 mm)\n(Z Start: None mm)\n(Z End: 19.0 mm)\n(X,Y End: 0.0000, 0.0000 mm)\n(Steps per circle: 64)\n(Steps per circle: 64)\n(Preprocessor Geometry: grbl_11)\n\n(X range:    0.0000 ...   10.0000  mm)\n(Y range:    0.0000 ...    5.0000  mm)\n\n(Spindle Speed: 1000.0 RPM)\nG21\nG90\nG17\nG94\n\n\nG01 F100.00\n\nM5             \nG00 Z19.0000\nG00 X0.0000 Y0.0000                \nT1\nM6\n(MSG, Change to Tool Dia = 1.2000)\nM0\nG00 Z19.0000\n\nM03 S1000.0\nG4 P1.5\nG01 F100.00\nG00 X0.0000 Y0.0000\nG01 F100.00\nG01 Z-0.5000\nG01 F100.00\nG01 X10.0000 Y0.0000 F100.00\nG01 X10.0000 Y5.0000 F100.00\nG00 Z2.0000\nG00 X3.0000 Y4.0000\nG01 F100.00\nG01 Z-0.5000\nG01 F100.00\nG00 Z2.0000\nM05\nG00 Z2.0000\nG00 Z19.00\nG00 X0.0 Y0.0\n\n"},
{"pp_grbl_11_no_toolchange", geometry31, settings31, "(This preprocessor is used with a motion controller loaded with GRBL firmware.)\n(It is configured to be compatible with almost any version of GRBL firmware.)\n\n(TOOL DIAMETER: 1.2 mm)\n(Feedrate_XY: 100.0 mm/min)\n(Feedrate_Z: 100.0 mm/min)\n(Feedrate rapids 1500.0 mm/min)\n\n(Z_Cut: -0.5 mm)\n(Z_Move: 2.0 mm)\n(Z Start: 1.0 mm)\n(Z End: 19.0 mm)\n(X,Y End: 0.0000, 0.0000 mm)\n(Steps per circle: 64)\n(Steps per circle: 64)\n(Preprocessor Geometry: grbl_11)\n\n(X range:    0.0000 ...   10.0000  mm)\n(Y range:    0.0000 ...    5.0000  mm)\n\n(Spindle Speed: 1000.0 RPM)\nG21\nG90\nG17\nG94\n\n\nG01 F100.00\nG00 Z2.0000\nG00 Z1.0000\nM03 S1000.0\nG01 F100.00\nG00 X0.0000 Y0.0000\nG01 F100.00\nG01 Z-0.5000\nG01 F100.00\nG01 X10.0000 Y0.0000 F100.00\nG01 X10.0000 Y5.0000 F100.00\nG00 Z2.0000\nM05\nG00 Z2.0000\nG00 Z19.00\nG00 X0.0 Y0.0\n\n"},
{"pp_GRBL_11_no_M6", geometry32, settings32, "(This preprocessor is used with a motion controller loaded with GRBL firmware.)\n(It is configured to be compatible with almost any version of GRBL firmware.)\n\n(TOOL DIAMETER: 1.2 mm)\n(Feedrate_XY: 100.0 mm/min)\n(Feedrate_Z: 100.0 mm/min)\n(Feedrate rapids 1500.0 mm/min)\n\n(Z_Cut: -0.5 mm)\n(Z_Move: 2.0 mm)\n(Z Start: None mm)\n(Z End: 19.0 mm)\n(X,Y End: 0.0000, 0.0000 mm)\n(Steps per circle: 64)\n(Steps per circle: 64)\n(Preprocessor Geometry: GRBL_11_no_M6)\n\n(X range:    0.0000 ...   10.0000  mm)\n(Y range:    0.0000 ...    5.0000  mm)\n\n(Spindle Speed: 1000.0 RPM)\nG21\nG90\nG17\nG94\n\n\nG01 F100.00\n\nM5             \nG00 Z19.0000\nG00 X0.0000 Y0.0000                \nT1\n(MSG, Change to Tool Dia = 1.2000)\nM0\nG00 Z19.0000\n		\nM03 S1000.0\nG4 P1.5\nG01 F100.00\nG00 X0.0000 Y0.0000\nG01 F100.00\nG01 Z-0.5000\nG01 F100.00\nG01 X10.0000 Y0.0000 F100.00\nG01 X10.0000 Y5.0000 F100.00\nG00 Z2.0000\nG00 X3.0000 Y4.0000\nG01 F100.00\nG01 Z-0.5000\nG01 F100.00\nG00 Z2.0000\nM05\nG00 Z2.0000\nG00 Z19.00\nG00 X0.0 Y0.0\n\n"},
{"pp_GRBL_11_no_M6_no_toolchange", geometry33, settings33, "(This preprocessor is used with a motion controller loaded with GRBL firmware.)\n(It is configured to be compatible with almost any version of GRBL firmware.)\n\n(TOOL DIAMETER: 1.2 mm)\n(Feedrate_XY: 100.0 mm/min)\n(Feedrate_Z: 100.0 mm/min)\n(Feedrate rapids 1500.0 mm/min)\n\n(Z_Cut: -0.5 mm)\n(Z_Move: 2.0 mm)\n(Z Start: 1.0 mm)\n(Z End: 19.0 mm)\n(X,Y End: 0.0000, 0.0000 mm)\n(Steps per circle: 64)\n(Steps per circle: 64)\n(Preprocessor Geometry: GRBL_11_no_M6)\n\n(X range:    0.0000 ...   10.0000  mm)\n(Y range:    0.0000 ...    5.0000  mm)\n\n(Spindle Speed: 1000.0 RPM)\nG21\nG90\nG17\nG94\n\n\nG01 F100.00\nG00 Z2.0000\nG00 Z1.0000\nM03 S1000.0\nG01 F100.00\nG00 X0.0000 Y0.0000\nG01 F100.00\nG01 Z-0.5000\nG01 F100.00\nG01 X10.0000 Y0.0000 F100.00\nG01 X10.0000 Y5.0000 F100.00\nG00 Z2.0000\nM05\nG00 Z2.0000\nG00 Z19.00\nG00 X0.0 Y0.0\n\n"},
{"pp_hpgl", geometry34, settings34, "IN;\nPU;\n\n\nSP1;\n\n\n\nPA0.0000,0.0000;\n\nPD;\n\n\nPA400.0000,0.0000;\nPA400.0000,200.0000;\nPU;\n\nPA120.0000,160.0000;\n\nPD;\n\n\nPU;\n\n\nPU;\n\nPA0.0000,0.0000;\n"},
{"pp_hpgl_no_toolchange", geometry35, settings35, "IN;\nPU;\n\n\nPU;\n\n\n\n\nPA0.0000,0.0000;\n\nPD;\n\n\nPA400.0000,0.0000;\nPA400.0000,200.0000;\nPU;\n\n\nPU;\n\nPA0.0000,0.0000;\n"},
{"pp_ISEL_CNC", geometry36, settings36, "(This preprocessor is used with a ISEL CNC router.)\n\n(TOOL DIAMETER: 1.2 mm)\n(Feedrate_XY: 100.0 mm/min)\n(Feedrate_Z: 100.0 mm/min)\n(Feedrate rapids 1500.0 mm/min)\n\n(Z_Cut: -0.5 mm)\n(Z_Move: 2.0 mm)\n(Z Start: None mm)\n(Z End: 19.0 mm)\n(X,Y End: 0.0000, 0.0000 mm)\n(Steps per circle: 64)\n(Steps per circle: 64)\n(Preprocessor Geometry: ISEL_CNC)\n\n(X range:    0.0000 ...   10.0000  mm)\n(Y range:    0.0000 ...    5.0000  mm)\n\n(Spindle Speed: 1000.0 RPM)\nG71\nG90\nG94\n\n\nG01 F100.00\n\nM05\nT1\nM06    \n(MSG, Change to Tool Dia = 1.2000)\nM01\nM03 S1000.0\nG4 P1.5\nG01 F100.00\nG00 X0.0000 Y0.0000\nG01 F100.00\nG01 Z-0.5000\nG01 F100.00\nG01 X10.0000 Y0.0000\nG01 X10.0000 Y5.0000\nG00 Z2.0000\nG00 X3.0000 Y4.0000\nG01 F100.00\nG01 Z-0.5000\nG01 F100.00\nG00 Z2.0000\nM05\nG00 Z2.0000\nG00 Z19.00\nG00 X0.0 Y0.0\n\n"},
{"pp_ISEL_CNC_no_toolchange", geometry37, settings37, "(This preprocessor is used with a ISEL CNC router.)\n\n(TOOL DIAMETER: 1.2 mm)\n(Feedrate_XY: 100.0 mm/min)\n(Feedrate_Z: 100.0 mm/min)\n(Feedrate rapids 1500.0 mm/min)\n\n(Z_Cut: -0.5 mm)\n(Z_Move: 2.0 mm)\n(Z Start: 1.0 mm)\n(Z End: 19.0 mm)\n(X,Y End: 0.0000, 0.0000 mm)\n(Steps per circle: 64)\n(Steps per circle: 64)\n(Preprocessor Geometry: ISEL_CNC)\n\n(X range:    0.0000 ...   10.0000  mm)\n(Y range:    0.0000 ...    5.0000  mm)\n\n(Spindle Speed: 1000.0 RPM)\nG71\nG90\nG94\n\n\nG01 F100.00\nG00 Z2.0000\nG00 Z1.0000\nM03 S1000.0\nG01 F100.00\nG00 X0.0000 Y0.0000\nG01 F100.00\nG01 Z-0.5000\nG01 F100.00\nG01 X10.0000 Y0.0000\nG01 X10.0000 Y5.0000\nG00 Z2.0000\nM05\nG00 Z2.0000\nG00 Z19.00\nG00 X0.0 Y0.0\n\n"},
{"pp_ISEL_ICP_CNC", geometry38, settings38, "; This preprocessor is used with a ISEL ICP CNC router.\n\nIMF_PBL flatcam\n\n;TOOL DIAMETER: 1.2 mm\n;Feedrate_XY: 100.0 mm/min\n;Feedrate_Z: 100.0 mm/min\n;Feedrate rapids 1500.0 mm/min\n\n;Z_Cut: -0.5 mm\n;Z_Move: 2.0 mm\n;Z Start: None mm\n;Z End: 19.0 mm\n;X,Y End: 0.0000, 0.0000 mm\n(Steps per circle: 64)\n;Steps per circle: 64\n;Preprocessor Geometry: ISEL_ICP_CNC\n\n;X range:    0.0000 ...   10.0000  mm\n;Y range:    0.0000 ...    5.0000  mm\n\n;Spindle Speed: 1000.0 RPM)\n\n\nVEL 1666\nGETTOOL 1\n; Changed to Tool Dia = 1.2000\nSPINDLE CW RPM1000\nWAIT 1500\nVEL 1666\nFASTABS X0 Y0\nVEL 1666\nMOVEABS Z-500\nVEL 1666\nMOVEABS X10000 Y0\nMOVEABS X10000 Y5000\nFASTABS Z2000\nFASTABS X3000 Y4000\nVEL 1666\nMOVEABS Z-500\nVEL 1666\nFASTABS Z2000\nSPINDLE OFF\nFASTABS Z2000\nWPCLEAR\nFASTABS Z0\nFASTABS X0 Y0\nPROGEND\n"},
{"pp_ISEL_ICP_CNC_no_toolchange", geometry39, settings39, "; This preprocessor is used with a ISEL ICP CNC router.\n\nIMF_PBL flatcam\n\n;TOOL DIAMETER: 1.2 mm\n;Feedrate_XY: 100.0 mm/min\n;Feedrate_Z: 100.0 mm/min\n;Feedrate rapids 1500.0 mm/min\n\n;Z_Cut: -0.5 mm\n;Z_Move: 2.0 mm\n;Z Start: 1.0 mm\n;Z End: 19.0 mm\n;X,Y End: 0.0000, 0.0000 mm\n(Steps per circle: 64)\n;Steps per circle: 64\n;Preprocessor Geometry: ISEL_ICP_CNC\n\n;X range:    0.0000 ...   10.0000  mm\n;Y range:    0.0000 ...    5.0000  mm\n\n;Spindle Speed: 1000.0 RPM)\n\n\nVEL 1666\nFASTABS Z2000\nFASTABS Z1000\nSPINDLE CW RPM1000\nVEL 1666\nFASTABS X0 Y0\nVEL 1666\nMOVEABS Z-500\nVEL 1666\nMOVEABS X10000 Y0\nMOVEABS X10000 Y5000\nFASTABS Z2000\nSPINDLE OFF\nFASTABS Z2000\nWPCLEAR\nFASTABS Z0\nFASTABS X0 Y0\nPROGEND\n"},
{"pp_line_xyz", geometry40, settings40, "(TOOL DIAMETER: 1.2 mm)\n(Feedrate_XY: 100.0 mm/min)\n(Feedrate_Z: 100.0 mm/min)\n(Feedrate rapids 1500.0 mm/min)\n\n(Z_Cut: -0.5 mm)\n(Z_Move: 2.0 mm)\n(Z Start: None mm)\n(Z End: 19.0 mm)\n(X,Y End: 0.0000, 0.0000 mm)\n(Steps per circle: 64)\n(Steps per circle: 64)\n(Preprocessor Geometry: line_xyz)\n\n(X range:    0.0000 ...   10.0000  mm)\n(Y range:    0.0000 ...    5.0000  mm)\n\n(Spindle Speed: 1000.0 RPM)\nG21\nG90\nG94\n\n\nG01 F100.00\n\nM5\nG00 X0.0000 Y0.0000 Z19.0000\nT1\nM6    \n(MSG, Change to Tool Dia = 1.2000)\nM0\nM03 S1000.0\nG4 P1.5\nG01 F100.00\nG00 X0.0000 Y0.0000 Z2.0000\nG01 F100.00\nG01 X0.0000 Y0.0000 Z-0.5000\nG01 F100.00\nG01 X10.0000 Y0.0000 Z-0.5000\nG01 X10.0000 Y5.0000 Z-0.5000\nG00 X10.0000 Y5.0000 Z2.0000\nG00 X3.0000 Y4.0000 Z2.0000\nG01 F100.00\nG01 X3.0000 Y4.0000 Z-0.5000\nG01 F100.00\nG00 X3.0000 Y4.0000 Z2.0000\nM05\nG00 X3.0000 Y4.0000 Z2.0000\nG00 X0.0 Y0.0\n Z19.0000\n"},
{"pp_line_xyz_no_toolchange", geometry41, settings41, "(TOOL DIAMETER: 1.2 mm)\n(Feedrate_XY: 100.0 mm/min)\n(Feedrate_Z: 100.0 mm/min)\n(Feedrate rapids 1500.0 mm/min)\n\n(Z_Cut: -0.5 mm)\n(Z_Move: 2.0 mm)\n(Z Start: 1.0 mm)\n(Z End: 19.0 mm)\n(X,Y End: 0.0000, 0.0000 mm)\n(Steps per circle: 64)\n(Steps per circle: 64)\n(Preprocessor Geometry: line_xyz)\n\n(X range:    0.0000 ...   10.0000  mm)\n(Y range:    0.0000 ...    5.0000  mm)\n\n(Spindle Speed: 1000.0 RPM)\nG21\nG90\nG94\n\n\nG01 F100.00\nG00 X0.0000 Y0.0000 Z2.0000\nG00 X0.0000 Y0.0000 Z1.0000\nM03 S1000.0\nG01 F100.00\nG00 X0.0000 Y0.0000 Z2.0000\nG01 F100.00\nG01 X0.0000 Y0.0000 Z-0.5000\nG01 F100.00\nG01 X10.0000 Y0.0000 Z-0.5000\nG01 X10.0000 Y5.0000 Z-0.5000\nG00 X10.0000 Y5.0000 Z2.0000\nM05\nG00 X10.0000 Y5.0000 Z2.0000\nG00 X0.0 Y0.0\n Z19.0000\n"},
{"pp_Marlin", geometry42, settings42, ";TOOL DIAMETER: 1.2 mm\n;Feedrate_XY: 100.0 mm/min\n;Feedrate_Z: 100.0 mm/min\n;Feedrate rapids 1500.0 mm/min\n\n;Z_Cut: -0.5 mm\n;Z_Move: 2.0 mm\n;Z Start: None mm\n;Z End: 19.0 mm\n;X,Y End: 0.0000, 0.0000 mm\n(Steps per circle: 64)\n;Steps per circle: 64\n;Preprocessor Geometry: Marlin\n\n;X range:    0.0000 ...   10.0000  mm\n;Y range:    0.0000 ...    5.0000  mm\n\n;Spindle Speed: 1000.0 RPM)\nG21\nG90\n\nG1 F100.00\n\nM5\nG0 Z19.0000\nG0 X0.0000 Y0.0000\nT1\nM6    \n;MSG, Change to Tool Dia = 1.2000\nM0\nG0 Z19.0000\n\nM3 S1000.0\nG4 P1.5\nG1 F100.00\nG0 X0.0000 Y0.0000 F1500.00\nG1 F100.00\nG1 Z-0.5000 F100.00\nG1 F100.00\nG1 X10.0000 Y0.0000 F100.00\nG1 X10.0000 Y5.0000 F100.00\nG0 Z2.0000 F1500.00\nG0 X3.0000 Y4.0000 F1500.00\nG1 F100.00\nG1 Z-0.5000 F100.00\nG1 F100.00\nG0 Z2.0000 F1500.00\nM400\nM5\nG0 Z2.0000 F1500.00\nG0 Z19.00 F1500.00\nG0 X0.0 Y0.0 F1500.00\n\n"},
{"pp_Marlin_no_toolchange", geometry43, settings43, ";TOOL DIAMETER: 1.2 mm\n;Feedrate_XY: 100.0 mm/min\n;Feedrate_Z: 100.0 mm/min\n;Feedrate rapids 1500.0 mm/min\n\n;Z_Cut: -0.5 mm\n;Z_Move: 2.0 mm\n;Z Start: 1.0 mm\n;Z End: 19.0 mm\n;X,Y End: 0.0000, 0.0000 mm\n(Steps per circle: 64)\n;Steps per circle: 64\n;Preprocessor Geometry: Marlin\n\n;X range:    0.0000 ...   10.0000  mm\n;Y range:    0.0000 ...    5.0000  mm\n\n;Spindle Speed: 1000.0 RPM)\nG21\nG90\n\nG1 F100.00\nG0 Z2.0000 F1500.00\nG0 Z1.0000\nM3 S1000.0\nG1 F100.00\nG0 X0.0000 Y0.0000 F1500.00\nG1 F100.00\nG1 Z-0.5000 F100.00\nG1 F100.00\nG1 X10.0000 Y0.0000 F100.00\nG1 X10.0000 Y5.0000 F100.00\nG0 Z2.0000 F1500.00\nM400\nM5\nG0 Z2.0000 F1500.00\nG0 Z19.00 F1500.00\nG0 X0.0 Y0.0 F1500.00\n\n"},
{"pp_Marlin_laser_FAN_pin", geometry44, settings44, ";This preprocessor is used with a motion controller loaded with MARLIN firmware.\n;It is for the case when it is used together with a LASER connected on one of the FAN pins.\n\n;Feedrate: 100.0 mm/min\n;Feedrate rapids: 1500.0 mm/min\n\n;Z Focus: 2.0 mm\n;Steps per circle: 64\n;Preprocessor Geometry: Marlin_laser_FAN_pin\n(X,Y End: 0.0000, 0.0000 mm)\n(Steps per circle: 64)\n;X range:    0.0000 ...   10.0000  mm\n;Y range:    0.0000 ...    5.0000  mm\n\n;Laser Power (Spindle Speed): 1000.0\n\nG21\nG90\n\nG1 F100.00\n\nM400\nM107\n\nG1 F100.00\nG0 X0.0000 Y0.0000 F1500.00\nG1 F100.00\nM106 S1000.0\nG1 F100.00\nG1 X10.0000 Y0.0000 F100.00\nG1 X10.0000 Y5.0000 F100.00\nM400\nM107\nG0 X3.0000 Y4.0000 F1500.00\nG1 F100.00\nM106 S1000.0\nG1 F100.00\nM400\nM107\nM400\nM106 S0\nM400\nM107\nG0 Z19.00 F1500.00\nG0 X0.0 Y0.0 F1500.00\n\n"},
{"pp_Marlin_laser_FAN_pin_no_toolchange", geometry45, settings45, ";This preprocessor is used with a motion controller loaded with MARLIN firmware.\n;It is for the case when it is used together with a LASER connected on one of the FAN pins.\n\n;Feedrate: 100.0 mm/min\n;Feedrate rapids: 1500.0 mm/min\n\n;Z Focus: 2.0 mm\n;Steps per circle: 64\n;Preprocessor Geometry: Marlin_laser_FAN_pin\n(X,Y End: 0.0000, 0.0000 mm)\n(Steps per circle: 64)\n;X range:    0.0000 ...   10.0000  mm\n;Y range:    0.0000 ...    5.0000  mm\n\n;Laser Power (Spindle Speed): 1000.0\n\nG21\nG90\n\nG1 F100.00\nM400\nM107\nG0 Z2.0000\nG1 F100.00\nG0 X0.0000 Y0.0000 F1500.00\nG1 F100.00\nM106 S1000.0\nG1 F100.00\nG1 X10.0000 Y0.0000 F100.00\nG1 X10.0000 Y5.0000 F100.00\nM400\nM107\nM400\nM106 S0\nM400\nM107\nG0 Z19.00 F1500.00\nG0 X0.0 Y0.0 F1500.00\n\n"},
{"pp_Marlin_laser_Spindle_pin", geometry46, settings46, ";This preprocessor is used with a motion controller loaded with MARLIN firmware.\n;It is for the case when it is used together with a LASER connected on the SPINDLE connector.\n\n;Feedrate: 100.0 mm/min\n;Feedrate rapids: 1500.0 mm/min\n\n;Z Focus: 2.0 mm\n;Steps per circle: 64\n;Preprocessor Geometry: Marlin_laser_Spindle_pin\n(X,Y End: 0.0000, 0.0000 mm)\n;X range:    0.0000 ...   10.0000  mm\n;Y range:    0.0000 ...    5.0000  mm\n\n;Laser Power (Spindle Speed): 1000.0\n\nG21\nG90\n\nG1 F100.00\n\nM400\nM5\n\nG1 F100.00\nG0 X0.0000 Y0.0000 F1500.00\nG1 F100.00\nM3 S1000.0\nG1 F100.00\nG1 X10.0000 Y0.0000 F100.00\nG1 X10.0000 Y5.0000 F100.00\nM400\nM5\nG0 X3.0000 Y4.0000 F1500.00\nG1 F100.00\nM3 S1000.0\nG1 F100.00\nM400\nM5\nM400\nM5\nM400\nM5\nG0 Z19.00 F1500.00\nG0 X0.0 Y0.0 F1500.00\n\n"},
{"pp_Marlin_laser_Spindle_pin_no_toolchange", geometry47, settings47, ";This preprocessor is used with a motion controller loaded with MARLIN firmware.\n;It is for the case when it is used together with a LASER connected on the SPINDLE connector.\n\n;Feedrate: 100.0 mm/min\n;Feedrate rapids: 1500.0 mm/min\n\n;Z Focus: 2.0 mm\n;Steps per circle: 64\n;Preprocessor Geometry: Marlin_laser_Spindle_pin\n(X,Y End: 0.0000, 0.0000 mm)\n;X range:    0.0000 ...   10.0000  mm\n;Y range:    0.0000 ...    5.0000  mm\n\n;Laser Power (Spindle Speed): 1000.0\n\nG21\nG90\n\nG1 F100.00\nM400\nM5\nG0 Z2.0000\nG1 F100.00\nG0 X0.0000 Y0.0000 F1500.00\nG1 F100.00\nM3 S1000.0\nG1 F100.00\nG1 X10.0000 Y0.0000 F100.00\nG1 X10.0000 Y5.0000 F100.00\nM400\nM5\nM400\nM5\nM400\nM5\nG0 Z19.00 F1500.00\nG0 X0.0 Y0.0 F1500.00\n\n"},
{"pp_Repetier", geometry48, settings48, ";This preprocessor is used with a motion controller loaded with REPETIER firmware.\n\n;TOOL DIAMETER: 1.2 mm\n;Feedrate_XY: 100.0 mm/min\n;Feedrate_Z: 100.0 mm/min\n;Feedrate rapids 1500.0 mm/min\n\n;Z_Cut: -0.5 mm\n;Z_Move: 2.0 mm\n;Z Start: None mm\n;Z End: 19.0 mm\n;X,Y End: 0.0000, 0.0000 mm\n;Steps per circle: 64\n;Preprocessor Geometry: Repetier\n\n;X range:    0.0000 ...   10.0000  mm\n;Y range:    0.0000 ...    5.0000  mm\n\n;Spindle Speed: 1000.0 RPM)\nG21\nG90\nG94\n\n\nG1 F100.00\n\nG0 Z19.0000\nG0 X0.0000 Y0.0000\nM84\n@pause Change to tool T1 with Tool Dia = 1.2000\nG0 Z19.0000\n\nM106 S1000\nG4 P1.5\nG1 F100.00\nG0 X0.0000 Y0.0000 F1500.00\nG1 F100.00\nG1 Z-0.5000 F100.00\nG1 F100.00\nG1 X10.0000 Y0.0000 F100.00\nG1 X10.0000 Y5.0000 F100.00\nG0 Z2.0000 F1500.00\nG0 X3.0000 Y4.0000 F1500.00\nG1 F100.00\nG1 Z-0.5000 F100.00\nG1 F100.00\nG0 Z2.0000 F1500.00\nM107\nG0 Z2.0000 F1500.00\nG0 Z19.00 F1500.00\nG0 X0.0 Y0.0 F1500.00\n\n"},
{"pp_Repetier_no_toolchange", geometry49, settings49, ";This preprocessor is used with a motion controller loaded with REPETIER firmware.\n\n;TOOL DIAMETER: 1.2 mm\n;Feedrate_XY: 100.0 mm/min\n;Feedrate_Z: 100.0 mm/min\n;Feedrate rapids 1500.0 mm/min\n\n;Z_Cut: -0.5 mm\n;Z_Move: 2.0 mm\n;Z Start: 1.0 mm\n;Z End: 19.0 mm\n;X,Y End: 0.0000, 0.0000 mm\n;Steps per circle: 64\n;Preprocessor Geometry: Repetier\n\n;X range:    0.0000 ...   10.0000  mm\n;Y range:    0.0000 ...    5.0000  mm\n\n;Spindle Speed: 1000.0 RPM)\nG21\nG90\nG94\n\n\nG1 F100.00\nG0 Z2.0000 F1500.00\nG0 Z1.0000\nM106 S1000\nG1 F100.00\nG0 X0.0000 Y0.0000 F1500.00\nG1 F100.00\nG1 Z-0.5000 F100.00\nG1 F100.00\nG1 X10.0000 Y0.0000 F100.00\nG1 X10.0000 Y5.0000 F100.00\nG0 Z2.0000 F1500.00\nM107\nG0 Z2.0000 F1500.00\nG0 Z19.00 F1500.00\nG0 X0.0 Y0.0 F1500.00\n\n"},
{"pp_Roland_MDX_20", geometry50, settings50, ";;^IN;\n^PA;\n\nV1.7;\n\n!MC1\n\nV1.7;\nV15.0;\nZ0.0,0.0,80.0;\nV1.7;\nV1.7;\nZ0.0,0.0,-20.0;\nV1.7;\nV1.7;\nZ400.0,0.0,-20.0;\nV1.7;\nZ400.0,200.0,-20.0;\nV15.0;\nZ400.0,200.0,80.0;\nV15.0;\nZ120.0,160.0,80.0;\nV1.7;\nV1.7;\nZ120.0,160.0,-20.0;\nV1.7;\nV15.0;\nZ120.0,160.0,80.0;\n!MC0\nV15.0;\nZ120.0,160.0,80.0;\nV15.0;\nZ0.0,0.0,760.0;\n"},
{"pp_Roland_MDX_20_no_toolchange", geometry51, settings51, ";;^IN;\n^PA;\n\nV1.7;\nV15.0;\nZ0.0,0.0,80.0;\n\n!MC1\nV1.7;\nV15.0;\nZ0.0,0.0,80.0;\nV1.7;\nV1.7;\nZ0.0,0.0,-20.0;\nV1.7;\nV1.7;\nZ400.0,0.0,-20.0;\nV1.7;\nZ400.0,200.0,-20.0;\nV15.0;\nZ400.0,200.0,80.0;\n!MC0\nV15.0;\nZ400.0,200.0,80.0;\nV15.0;\nZ0.0,0.0,760.0;\n"},
{"pp_Toolchange_Custom", geometry52, settings52, "(TOOL DIAMETER: 1.2 mm)\n(Feedrate_XY: 100.0 mm/min)\n(Feedrate_Z: 100.0 mm/min)\n(Feedrate rapids 1500.0 mm/min)\n\n(Z_Cut: -0.5 mm)\n(Z_Move: 2.0 mm)\n(Z Start: None mm)\n(Z End: 19.0 mm)\n(X,Y End: 0.0000, 0.0000 mm)\n(Steps per circle: 64)\n(Preprocessor Geometry: Toolchange_Custom)\n\n(X range:    0.0000 ...   10.0000  mm)\n(Y range:    0.0000 ...    5.0000  mm)\n\n(Spindle Speed: 1000.0 RPM)\nG21\nG90\nG17\nG94\n\n\nG01 F100.00\n\nM6\n\nM03 S1000.0\nG4 P1.5\nG01 F100.00\nG00 X0.0000 Y0.0000\nG01 F100.00\nG01 Z-0.5000\nG01 F100.00\nG01 X10.0000 Y0.0000\nG01 X10.0000 Y5.0000\nG00 Z2.0000\nG00 X3.0000 Y4.0000\nG01 F100.00\nG01 Z-0.5000\nG01 F100.00\nG00 Z2.0000\nM05\nG00 Z2.0000\nG00 Z19.00\nG00 X0.0 Y0.0\n\n"},
{"pp_Toolchange_Custom_no_toolchange", geometry53, settings53, "(TOOL DIAMETER: 1.2 mm)\n(Feedrate_XY: 100.0 mm/min)\n(Feedrate_Z: 100.0 mm/min)\n(Feedrate rapids 1500.0 mm/min)\n\n(Z_Cut: -0.5 mm)\n(Z_Move: 2.0 mm)\n(Z Start: 1.0 mm)\n(Z End: 19.0 mm)\n(X,Y End: 0.0000, 0.0000 mm)\n(Steps per circle: 64)\n(Preprocessor Geometry: Toolchange_Custom)\n\n(X range:    0.0000 ...   10.0000  mm)\n(Y range:    0.0000 ...    5.0000  mm)\n\n(Spindle Speed: 1000.0 RPM)\nG21\nG90\nG17\nG94\n\n\nG01 F100.00\nG00 Z2.0000\n\nM03 S1000.0\nG01 F100.00\nG00 X0.0000 Y0.0000\nG01 F100.00\nG01 Z-0.5000\nG01 F100.00\nG01 X10.0000 Y0.0000\nG01 X10.0000 Y5.0000\nG00 Z2.0000\nM05\nG00 Z2.0000\nG00 Z19.00\nG00 X0.0 Y0.0\n\n"},
{"pp_Toolchange_Manual", geometry54, settings54, "(TOOL DIAMETER: 1.2 mm)\n(Feedrate_XY: 100.0 mm/min)\n(Feedrate_Z: 100.0 mm/min)\n(Feedrate rapids 1500.0 mm/min)\n\n(Z_Cut: -0.5 mm)\n(Z_Move: 2.0 mm)\n(Z Start: None mm)\n(Z End: 19.0 mm)\n(X,Y End: 0.0000, 0.0000 mm)\n(Steps per circle: 64)\n(Preprocessor Geometry: Toolchange_Manual)\n\n(X range:    0.0000 ...   10.0000  mm)\n(Y range:    0.0000 ...    5.0000  mm)\n\n(Spindle Speed: 1000.0 RPM)\nG21\nG90\nG17\nG94\n\n\nG01 F100.00\n\nM5\nG00 Z19.0000\nT1\nG00 X0.0000Y0.0000    \n(MSG, Change to Tool Dia = 1.2000)\nM0\nG01 Z0\n(MSG, Adjust the tool T1 to touch the material and then tighten it slightly.)\nM0\nG00 Z19.0000\n(MSG, Now the tool can be tightened more securely.)\nM0\n\nM03 S1000.0\nG4 P1.5\nG01 F100.00\nG00 X0.0000 Y0.0000\nG01 F100.00\nG01 Z-0.5000\nG01 F100.00\nG01 X10.0000 Y0.0000\nG01 X10.0000 Y5.0000\nG00 Z2.0000\nG00 X3.0000 Y4.0000\nG01 F100.00\nG01 Z-0.5000\nG01 F100.00\nG00 Z2.0000\nM05\nG00 Z2.0000\nG00 Z19.00\nG00 X0.0 Y0.0\n\n"},
{"pp_Toolchange_Manual_no_toolchange", geometry55, settings55, "(TOOL DIAMETER: 1.2 mm)\n(Feedrate_XY: 100.0 mm/min)\n(Feedrate_Z: 100.0 mm/min)\n(Feedrate rapids 1500.0 mm/min)\n\n(Z_Cut: -0.5 mm)\n(Z_Move: 2.0 mm)\n(Z Start: 1.0 mm)\n(Z End: 19.0 mm)\n(X,Y End: 0.0000, 0.0000 mm)\n(Steps per circle: 64)\n(Preprocessor Geometry: Toolchange_Manual)\n\n(X range:    0.0000 ...   10.0000  mm)\n(Y range:    0.0000 ...    5.0000  mm)\n\n(Spindle Speed: 1000.0 RPM)\nG21\nG90\nG17\nG94\n\n\nG01 F100.00\nG00 Z2.0000\nG00 Z1.0000\nM03 S1000.0\nG01 F100.00\nG00 X0.0000 Y0.0000\nG01 F100.00\nG01 Z-0.5000\nG01 F100.00\nG01 X10.0000 Y0.0000\nG01 X10.0000 Y5.0000\nG00 Z2.0000\nM05\nG00 Z2.0000\nG00 Z19.00\nG00 X0.0 Y0.0\n\n"},
{"pp_Toolchange_Probe_MACH3", geometry56, settings56, "(This preprocessor is used with MACH3 with probing height.)\n\n(TOOL DIAMETER: 1.2 mm)\n(Feedrate_XY: 100.0 mm/min)\n(Feedrate_Z: 100.0 mm/min)\n(Feedrate rapids 1500.0 mm/min)\n\n(Z_Cut: -0.5 mm)\n(Z_Move: 2.0 mm)\n(Z Start: None mm)\n(Z End: 19.0 mm)\n(X,Y End: 0.0000, 0.0000 mm)\n(Steps per circle: 64)\n(Preprocessor Geometry: Toolchange_Probe_MACH3)\n\n(X range:    0.0000 ...   10.0000  mm)\n(Y range:    0.0000 ...    5.0000  mm)\n\n(Spindle Speed: 1000.0 RPM)\nG21\nG90\nG17\nG94\n\n\nG01 F100.00\n\nM5\nT1\nM6\nG00 Z19.0000\nG00 X0.0000 Y0.0000\n(MSG, Change to Tool Dia = 1.2000 ||| CONNECT THE PROBE)\nM0\nF75.00\nG31 Z-0.0200\nG92 Z0\nG00 Z1.0000\nF37.50\nG31 Z-0.0200\nG92 Z0\nG00 Z2.0000\n(MSG, Remove any clips or other devices used for probing. CNC work is resuming ...)\nM0\n\nM03 S1000.0\nG4 P1.5\nG01 F100.00\nG00 X0.0000 Y0.0000\nG01 F100.00\nG01 Z-0.5000\nG01 F100.00\nG01 X10.0000 Y0.0000\nG01 X10.0000 Y5.0000\nG00 Z2.0000\nG00 X3.0000 Y4.0000\nG01 F100.00\nG01 Z-0.5000\nG01 F100.00\nG00 Z2.0000\nM05\nG00 Z2.0000\nG00 Z19.00\nG00 X0.0 Y0.0\n\n"},
{"pp_Toolchange_Probe_MACH3_no_toolchange", geometry57, settings57, "(This preprocessor is used with MACH3 with probing height.)\n\n(TOOL DIAMETER: 1.2 mm)\n(Feedrate_XY: 100.0 mm/min)\n(Feedrate_Z: 100.0 mm/min)\n(Feedrate rapids 1500.0 mm/min)\n\n(Z_Cut: -0.5 mm)\n(Z_Move: 2.0 mm)\n(Z Start: 1.0 mm)\n(Z End: 19.0 mm)\n(X,Y End: 0.0000, 0.0000 mm)\n(Steps per circle: 64)\n(Preprocessor Geometry: Toolchange_Probe_MACH3)\n\n(X range:    0.0000 ...   10.0000  mm)\n(Y range:    0.0000 ...    5.0000  mm)\n\n(Spindle Speed: 1000.0 RPM)\nG21\nG90\nG17\nG94\n\n\nG01 F100.00\nG00 Z2.0000\n(MSG, WARNING: Make sure you do zero on all axis. For Z axis, since it will be probed, make a rough estimate and do a zero.)\nM0\nM03 S1000.0\nG01 F100.00\nG00 X0.0000 Y0.0000\nG01 F100.00\nG01 Z-0.5000\nG01 F100.00\nG01 X10.0000 Y0.0000\nG01 X10.0000 Y5.0000\nG00 Z2.0000\nM05\nG00 Z2.0000\nG00 Z19.00\nG00 X0.0 Y0.0\n\n"},
{"pp_Z_laser", geometry58, settings58, "(This preprocessor is used with a motion controller loaded with GRBL firmware. )\n(It is for the case when it is used together with a LASER connected on the SPINDLE connector.)\n(On toolchange event the laser will move to a defined Z height to change the laser dot size.)\n\n(Feedrate: 100.0 mm/min)\n(Feedrate rapids: 1500.0 mm/min)\n\n(Z Focus: 2.0 mm)\n(Steps per circle: 64)\n(Preprocessor Geometry: Z_laser)\n\n(X range:    0.0000 ...   10.0000  mm)\n(Y range:    0.0000 ...    5.0000  mm)\n\n(Laser Power (Spindle Speed): 1000.0)\n\nG21\nG90\nG17\nG94\n\nG01 F100.00\nG00 Z2.0000\nM5\n\nG01 F100.00\nG00 X0.0000 Y0.0000\nG01 F100.00\nM03 S1000.0\nG01 F100.00\nG01 X10.0000 Y0.0000 F100.00\nG01 X10.0000 Y5.0000 F100.00\nM5\nG00 X3.0000 Y4.0000\nG01 F100.00\nM03 S1000.0\nG01 F100.00\nM5\nM5\nM5\nG00 Z19.00\nG00 X0.0 Y0.0\n\n"},
{"pp_Z_laser_no_toolchange", geometry59, settings59, "(This preprocessor is used with a motion controller loaded with GRBL firmware. )\n(It is for the case when it is used together with a LASER connected on the SPINDLE connector.)\n(On toolchange event the laser will move to a defined Z height to change the laser dot size.)\n\n(Feedrate: 100.0 mm/min)\n(Feedrate rapids: 1500.0 mm/min)\n\n(Z Focus: 2.0 mm)\n(Steps per circle: 64)\n(Preprocessor Geometry: Z_laser)\n\n(X range:    0.0000 ...   10.0000  mm)\n(Y range:    0.0000 ...    5.0000  mm)\n\n(Laser Power (Spindle Speed): 1000.0)\n\nG21\nG90\nG17\nG94\n\nG01 F100.00\nM5\n\nG01 F100.00\nG00 X0.0000 Y0.0000\nG01 F100.00\nM03 S1000.0\nG01 F100.00\nG01 X10.0000 Y0.0000 F100.00\nG01 X10.0000 Y5.0000 F100.00\nM5\nM5\nM5\nG00 Z19.00\nG00 X0.0 Y0.0\n\n"},
{"pp_Roland_MDX_20_extracut", geometry60, settings60, ";;^IN;\n^PA;\n\nV1.7;\n\n!MC1\nV1.7;\nV15.0;\nZ0.0,0.0,80.0;\nV1.7;\nV1.7;\nZ0.0,0.0,-40.0;\nV1.7;\nV1.7;\nZ400.0,0.0,-40.0;\nV1.7;\nZ400.0,400.0,-40.0;\nV1.7;\nZ0.0,400.0,-40.0;\nV1.7;\nZ0.0,0.0,-40.0;\nV15.0;\nZ0.0,0.0,80.0;\nV15.0;\nZ0.0,10.0,80.0;\nV1.7;\nV1.7;\nZ0.0,10.0,-40.0;\nV1.7;\n\n\n\nV15.0;\nZ0.0,0.0,80.0;\n!MC0\nV15.0;\nZ0.0,0.0,80.0;\nV15.0;\nZ0.0,0.0,760.0;\n"},
{"pp_Roland_MDX_20_extracut_zero", geometry61, settings61, ";;^IN;\n^PA;\n\nV1.7;\n\n!MC1\nV1.7;\nV15.0;\nZ0.0,0.0,80.0;\nV1.7;\nV1.7;\nZ0.0,0.0,-40.0;\nV1.7;\nV1.7;\nZ400.0,0.0,-40.0;\nV1.7;\nZ400.0,400.0,-40.0;\nV1.7;\nZ0.0,400.0,-40.0;\nV1.7;\nZ0.0,0.0,-40.0;\nV15.0;\nZ0.0,0.0,80.0;\nV15.0;\nZ0.0,0.0,80.0;\nV1.7;\nV1.7;\nZ0.0,0.0,-40.0;\nV1.7;\n\n\n\nV15.0;\nZ0.0,0.0,80.0;\n!MC0\nV15.0;\nZ0.0,0.0,80.0;\nV15.0;\nZ0.0,0.0,760.0;\n"},
{"pp_Marlin_multidepth", geometry62, settings62, ";TOOL DIAMETER: 1.2 mm\n;Feedrate_XY: 100.0 mm/min\n;Feedrate_Z: 100.0 mm/min\n;Feedrate rapids 1500.0 mm/min\n\n;Z_Cut: -1.0 mm\n;DepthPerCut: 0.4 mm <=>3 passes\n;Z_Move: 2.0 mm\n;Z Start: None mm\n;Z End: 19.0 mm\n;X,Y End: 0.0000, 0.0000 mm\n(Steps per circle: 64)\n;Steps per circle: 64\n;Preprocessor Geometry: Marlin\n\n;X range:    0.0000 ...   10.0000  mm\n;Y range:    0.0000 ...   10.0000  mm\n\n;Spindle Speed: 1000.0 RPM)\nG21\nG90\n\nG1 F100.00\n\nM5\nG0 Z19.0000\nG0 X0.0000 Y0.0000\nT1\nM6    \n;MSG, Change to Tool Dia = 1.2000\nM0\nG0 Z19.0000\n\nM3 S1000.0\nG1 F100.00\nG0 X0.0000 Y0.0000 F1500.00\nG1 F100.00\nG1 Z-0.4000 F100.00\nG1 F100.00\nG1 X10.0000 Y0.0000 F100.00\nG1 X10.0000 Y10.0000 F100.00\nG1 X0.0000 Y10.0000 F100.00\nG1 X0.0000 Y0.0000 F100.00\nG0 X0.0000 Y0.0000 F1500.00\nG1 F100.00\nG1 Z-0.8000 F100.00\nG1 F100.00\nG1 X10.0000 Y0.0000 F100.00\nG1 X10.0000 Y10.0000 F100.00\nG1 X0.0000 Y10.0000 F100.00\nG1 X0.0000 Y0.0000 F100.00\nG0 X0.0000 Y0.0000 F1500.00\nG1 F100.00\nG1 Z-1.0000 F100.00\nG1 F100.00\nG1 X10.0000 Y0.0000 F100.00\nG1 X10.0000 Y10.0000 F100.00\nG1 X0.0000 Y10.0000 F100.00\nG1 X0.0000 Y0.0000 F100.00\nG0 Z2.0000 F1500.00\nM400\nM5\nG0 Z2.0000 F1500.00\nG0 Z19.00 F1500.00\nG0 X0.0 Y0.0 F1500.00\n\n"},
{"pp_line_xyz_extracut_multidepth", geometry63, settings63, "(TOOL DIAMETER: 1.2 mm)\n(Feedrate_XY: 100.0 mm/min)\n(Feedrate_Z: 100.0 mm/min)\n(Feedrate rapids 1500.0 mm/min)\n\n(Z_Cut: -1.0 mm)\n(DepthPerCut: 0.5 mm <=>2 passes)\n(Z_Move: 2.0 mm)\n(Z Start: None mm)\n(Z End: 19.0 mm)\n(X,Y End: 0.0000, 0.0000 mm)\n(Steps per circle: 64)\n(Steps per circle: 64)\n(Preprocessor Geometry: line_xyz)\n\n(X range:    0.0000 ...   10.0000  mm)\n(Y range:    0.0000 ...   10.0000  mm)\n\n(Spindle Speed: 1000.0 RPM)\nG21\nG90\nG94\n\n\nG01 F100.00\n\nM5\nG00 X0.0000 Y0.0000 Z19.0000\nT1\nM6    \n(MSG, Change to Tool Dia = 1.2000)\nM0\nM03 S1000.0\nG01 F100.00\nG00 X0.0000 Y0.0000 Z2.0000\nG01 F100.00\nG01 X0.0000 Y0.0000 Z-0.5000\nG01 F100.00\nG01 X10.0000 Y0.0000 Z-1.0000\nG01 X10.0000 Y10.0000 Z-1.0000\nG01 X0.0000 Y10.0000 Z-1.0000\nG01 X0.0000 Y0.0000 Z-1.0000\nG00 X0.0000 Y0.0000 Z2.0000\nG00 X0.0000 Y0.5000 Z2.0000\nG01 F100.00\nG01 X0.0000 Y0.5000 Z-0.5000\nG01 F100.00\nG01 X0.0000 Y0.0000 Z-1.0000\nG01 X0.5000 Y0.0000 Z-1.0000\nG01 X0.0000 Y0.0000 Z-1.0000\nG00 X0.0000 Y0.0000 Z2.0000\nG01 F100.00\nG01 X0.0000 Y0.0000 Z-1.0000\nG01 F100.00\nG01 X10.0000 Y0.0000 Z-1.0000\nG01 X10.0000 Y10.0000 Z-1.0000\nG01 X0.0000 Y10.0000 Z-1.0000\nG01 X0.0000 Y0.0000 Z-1.0000\nG00 X0.0000 Y0.0000 Z2.0000\nG00 X0.0000 Y0.5000 Z2.0000\nG01 F100.00\nG01 X0.0000 Y0.5000 Z-1.0000\nG01 F100.00\nG01 X0.0000 Y0.0000 Z-1.0000\nG01 X0.5000 Y0.0000 Z-1.0000\nG01 X0.0000 Y0.0000 Z-1.0000\nG00 X0.0000 Y0.0000 Z2.0000\nM05\nG00 X0.0000 Y0.0000 Z2.0000\nG00 X0.0 Y0.0\n Z19.0000\n"},
{"pp_hpgl_inches", geometry64, settings64, "IN;\nPU;\n\n\nSP1;\n\n\nPA0.0000,0.0000;\n\nPD;\n\n\nPA10160.0000,0.0000;\nPA10160.0000,5080.0000;\nPU;\n\n\nPU;\n\nPA0.0000,0.0000;\n"},
};
}  // namespace golden_geometry_modes
