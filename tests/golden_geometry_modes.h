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
};
}  // namespace golden_geometry_modes
