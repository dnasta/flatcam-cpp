// ARCHIVO GENERADO por tests/tools/oracle_geometry_multitool.py - no editar a mano.
// Referencia: mtool_gen_cncjob (multigeo) con CNCjob.geometry_tool_gcode_gen original (FlatCAM 2024.4).
#pragma once
#include <array>
#include <string>
#include <vector>
#include "cam/CncJob.h"
namespace golden_geometry_multitool {
inline std::vector<flatcam::GeometryToolJob> jobs0() {  // cutout_with_thin_gaps
    std::vector<flatcam::GeometryToolJob> v;
    {
        flatcam::GeometryToolJob j;
        j.geometry = {flatcam::Shape::fromWkt("POLYGON ((0 0, 10 0, 10 10, 0 10, 0 0))")};
        flatcam::GeometryJobSettings& s = j.settings;
        s.preprocessor = "default";
        s.order = flatcam::DrillOrder::FileOrder;
        s.tool = 1;
        s.toolchange = true;
        s.headerBounds = flatcam::BoundingBox{0.0, 0.0, 12.0, 10.0};
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
        t.multidepth = false;
        t.dwell = false;
        t.extracut = false;
        t.endXY = std::array<double, 2>{0.0, 0.0};
        t.toolchangeXY = std::array<double, 2>{0.0, 0.0};
        v.push_back(j);
    }
    {
        flatcam::GeometryToolJob j;
        j.geometry = {flatcam::Shape::fromWkt("LINESTRING (12 0, 12 3)")};
        flatcam::GeometryJobSettings& s = j.settings;
        s.preprocessor = "default";
        s.order = flatcam::DrillOrder::FileOrder;
        s.tool = 9999;
        s.toolchange = true;
        s.headerBounds = flatcam::BoundingBox{0.0, 0.0, 12.0, 10.0};
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
        t.extracutLength = 0.1;
        t.spindleDir = "CW";
        t.multidepth = false;
        t.dwell = false;
        t.extracut = false;
        t.endXY = std::array<double, 2>{0.0, 0.0};
        t.toolchangeXY = std::array<double, 2>{0.0, 0.0};
        v.push_back(j);
    }
    return v;
}
inline std::vector<flatcam::GeometryToolJob> jobs1() {  // two_tools_different_params
    std::vector<flatcam::GeometryToolJob> v;
    {
        flatcam::GeometryToolJob j;
        j.geometry = {flatcam::Shape::fromWkt("LINESTRING (0 0, 10 0, 10 5)")};
        flatcam::GeometryJobSettings& s = j.settings;
        s.preprocessor = "default";
        s.order = flatcam::DrillOrder::FileOrder;
        s.tool = 1;
        s.toolchange = true;
        s.headerBounds = flatcam::BoundingBox{0.0, 0.0, 10.0, 10.0};
        flatcam::GeometryToolSettings& t = s.toolSettings;
        t.toolDia = 2.0;
        t.cutZ = -0.5;
        t.depthPerPass = 0.6;
        t.travelZ = 2.0;
        t.feedrate = 100.0;
        t.feedrateZ = 100.0;
        t.feedrateRapid = 1500.0;
        t.spindleSpeed = 8000.0;
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
        v.push_back(j);
    }
    {
        flatcam::GeometryToolJob j;
        j.geometry = {flatcam::Shape::fromWkt("POLYGON ((0 0, 10 0, 10 10, 0 10, 0 0))")};
        flatcam::GeometryJobSettings& s = j.settings;
        s.preprocessor = "default";
        s.order = flatcam::DrillOrder::FileOrder;
        s.tool = 2;
        s.toolchange = true;
        s.headerBounds = flatcam::BoundingBox{0.0, 0.0, 10.0, 10.0};
        flatcam::GeometryToolSettings& t = s.toolSettings;
        t.toolDia = 0.8;
        t.cutZ = -1.0;
        t.depthPerPass = 0.4;
        t.travelZ = 2.0;
        t.feedrate = 50.0;
        t.feedrateZ = 100.0;
        t.feedrateRapid = 1500.0;
        t.spindleSpeed = 1000.0;
        t.dwellTime = 2.0;
        t.endZ = 19.0;
        t.toolchangeZ = 19.0;
        t.extracutLength = 0.1;
        t.spindleDir = "CW";
        t.multidepth = true;
        t.dwell = true;
        t.extracut = false;
        t.endXY = std::array<double, 2>{0.0, 0.0};
        t.toolchangeXY = std::array<double, 2>{0.0, 0.0};
        v.push_back(j);
    }
    return v;
}
inline std::vector<flatcam::GeometryToolJob> jobs2() {  // three_tools_laser
    std::vector<flatcam::GeometryToolJob> v;
    {
        flatcam::GeometryToolJob j;
        j.geometry = {flatcam::Shape::fromWkt("LINESTRING (0 0, 10 0, 10 5)")};
        flatcam::GeometryJobSettings& s = j.settings;
        s.preprocessor = "GRBL_laser";
        s.order = flatcam::DrillOrder::FileOrder;
        s.tool = 1;
        s.toolchange = true;
        s.headerBounds = flatcam::BoundingBox{0.0, 0.0, 12.0, 10.0};
        flatcam::GeometryToolSettings& t = s.toolSettings;
        t.toolDia = 0.1;
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
        v.push_back(j);
    }
    {
        flatcam::GeometryToolJob j;
        j.geometry = {flatcam::Shape::fromWkt("POLYGON ((0 0, 10 0, 10 10, 0 10, 0 0))")};
        flatcam::GeometryJobSettings& s = j.settings;
        s.preprocessor = "GRBL_laser";
        s.order = flatcam::DrillOrder::FileOrder;
        s.tool = 2;
        s.toolchange = true;
        s.headerBounds = flatcam::BoundingBox{0.0, 0.0, 12.0, 10.0};
        flatcam::GeometryToolSettings& t = s.toolSettings;
        t.toolDia = 0.2;
        t.cutZ = -0.5;
        t.depthPerPass = 0.6;
        t.travelZ = 2.0;
        t.feedrate = 400.0;
        t.feedrateZ = 100.0;
        t.feedrateRapid = 1500.0;
        t.spindleSpeed = 200.0;
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
        v.push_back(j);
    }
    {
        flatcam::GeometryToolJob j;
        j.geometry = {flatcam::Shape::fromWkt("LINESTRING (12 0, 12 3)")};
        flatcam::GeometryJobSettings& s = j.settings;
        s.preprocessor = "GRBL_laser";
        s.order = flatcam::DrillOrder::FileOrder;
        s.tool = 3;
        s.toolchange = true;
        s.headerBounds = flatcam::BoundingBox{0.0, 0.0, 12.0, 10.0};
        flatcam::GeometryToolSettings& t = s.toolSettings;
        t.toolDia = 0.3;
        t.cutZ = -0.5;
        t.depthPerPass = 0.6;
        t.travelZ = 2.0;
        t.feedrate = 300.0;
        t.feedrateZ = 100.0;
        t.feedrateRapid = 1500.0;
        t.spindleSpeed = 100.0;
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
        v.push_back(j);
    }
    return v;
}
struct Scenario { const char* name; std::vector<flatcam::GeometryToolJob> (*jobs)(); const char* expected; double travelDistance; double routingTime; };
inline const std::vector<Scenario> kScenarios = {
{"cutout_with_thin_gaps", jobs0, "(This preprocessor is the default preprocessor used by FlatCAM.)\n(It is made to work with MACH3 compatible motion controllers.)\n\n(TOOL DIAMETER: 1.2 mm)\n(Feedrate_XY: 100.0 mm/min)\n(Feedrate_Z: 100.0 mm/min)\n(Feedrate rapids 1500.0 mm/min)\n\n(Z_Cut: -1.8 mm)\n(Z_Move: 2.0 mm)\n(Z Start: None mm)\n(Z End: 19.0 mm)\n(X,Y End: 0.0000, 0.0000 mm)\n(Steps per circle: 64)\n(Preprocessor Geometry: default)\n\n(X range:    0.0000 ...   12.0000  mm)\n(Y range:    0.0000 ...   10.0000  mm)\n\n(Spindle Speed: 1000.0 RPM)\nG21\nG90\nG94\n\nG01 F100.00\n\nM5\nG00 Z19.0000\nG00 X0.0000 Y0.0000\nT1\nM6    \n(MSG, Change to Tool Dia = 1.2000)\nM0\nG00 Z19.0000\n\nM03 S1000.0\nG01 F100.00\nG00 X0.0000 Y0.0000\nG01 F100.00\nG01 Z-1.8000\nG01 F100.00\nG01 X10.0000 Y0.0000\nG01 X10.0000 Y10.0000\nG01 X0.0000 Y10.0000\nG01 X0.0000 Y0.0000\nG00 Z2.0000\nG01 F100.00\n\nM5\nG00 Z19.0000\nG00 X0.0000 Y0.0000\nT9999\nM6    \n(MSG, Change to Tool Dia = 1.2000)\nM0\nG00 Z19.0000\n\nM03 S1000.0\nG01 F100.00\nG00 X12.0000 Y0.0000\nG01 F100.00\nG01 Z-1.0000\nG01 F100.00\nG01 X12.0000 Y3.0000\nG00 Z2.0000\nM05\nG00 Z2.0000\nG00 Z19.00\nG00 X0.0 Y0.0\n\n", 67.36931687685298, 0.43000000000000005},
{"two_tools_different_params", jobs1, "(This preprocessor is the default preprocessor used by FlatCAM.)\n(It is made to work with MACH3 compatible motion controllers.)\n\n(TOOL DIAMETER: 2.0 mm)\n(Feedrate_XY: 100.0 mm/min)\n(Feedrate_Z: 100.0 mm/min)\n(Feedrate rapids 1500.0 mm/min)\n\n(Z_Cut: -0.5 mm)\n(Z_Move: 2.0 mm)\n(Z Start: None mm)\n(Z End: 19.0 mm)\n(X,Y End: 0.0000, 0.0000 mm)\n(Steps per circle: 64)\n(Preprocessor Geometry: default)\n\n(X range:    0.0000 ...   10.0000  mm)\n(Y range:    0.0000 ...   10.0000  mm)\n\n(Spindle Speed: 8000.0 RPM)\nG21\nG90\nG94\n\nG01 F100.00\n\nM5\nG00 Z19.0000\nG00 X0.0000 Y0.0000\nT1\nM6    \n(MSG, Change to Tool Dia = 2.0000)\nM0\nG00 Z19.0000\n\nM03 S8000.0\nG01 F100.00\nG00 X0.0000 Y0.0000\nG01 F100.00\nG01 Z-0.5000\nG01 F100.00\nG01 X10.0000 Y0.0000\nG01 X10.0000 Y5.0000\nG00 Z2.0000\nG01 F50.00\n\nM5\nG00 Z19.0000\nG00 X0.0000 Y0.0000\nT2\nM6    \n(MSG, Change to Tool Dia = 0.8000)\nM0\nG00 Z19.0000\n\nM03 S1000.0\nG4 P2.0\nG01 F50.00\nG00 X0.0000 Y0.0000\nG01 F100.00\nG01 Z-0.4000\nG01 F50.00\nG01 X10.0000 Y0.0000\nG01 X10.0000 Y10.0000\nG01 X0.0000 Y10.0000\nG01 X0.0000 Y0.0000\nG00 X0.0000 Y0.0000\nG01 F100.00\nG01 Z-0.8000\nG01 F50.00\nG01 X10.0000 Y0.0000\nG01 X10.0000 Y10.0000\nG01 X0.0000 Y10.0000\nG01 X0.0000 Y0.0000\nG00 X0.0000 Y0.0000\nG01 F100.00\nG01 Z-1.0000\nG01 F50.00\nG01 X10.0000 Y0.0000\nG01 X10.0000 Y10.0000\nG01 X0.0000 Y10.0000\nG01 X0.0000 Y0.0000\nG00 Z2.0000\nM05\nG00 Z2.0000\nG00 Z19.00\nG00 X0.0 Y0.0\n\n", 146.18033988749895, 2.55},
{"three_tools_laser", jobs2, "(This preprocessor is used with a motion controller loaded with GRBL firmware. )\n(It is for the case when it is used together with a LASER connected on the SPINDLE connector.)\n\n(Feedrate: 500.0 mm/min)\n(Feedrate rapids: 1500.0 mm/min)\n\n(Z Focus: 2.0 mm)\n(Steps per circle: 64)\n(Preprocessor Geometry: GRBL_laser)\n\n(X range:    0.0000 ...   12.0000  mm)\n(Y range:    0.0000 ...   10.0000  mm)\n\n(Laser Power - Spindle Speed: 300.0)\n\nG21\nG90\nG17\nG94\n\nG01 F500.00\n\nM5\nG01 F500.00\nG00 X0.0000 Y0.0000\nG01 F100.00\nM03 S300.0\nG01 F500.00\nG01 X10.0000 Y0.0000 F500.00\nG01 X10.0000 Y5.0000 F500.00\nM5\nG01 F400.00\n\nM5\nG01 F400.00\nG00 X0.0000 Y0.0000\nG01 F100.00\nM03 S200.0\nG01 F400.00\nG01 X10.0000 Y0.0000 F400.00\nG01 X10.0000 Y10.0000 F400.00\nG01 X0.0000 Y10.0000 F400.00\nG01 X0.0000 Y0.0000 F400.00\nM5\nG01 F300.00\n\nM5\nG01 F300.00\nG00 X12.0000 Y0.0000\nG01 F100.00\nM03 S100.0\nG01 F300.00\nG01 X12.0000 Y3.0000 F300.00\nM5\nM5\nM5\nG00 Z19.00\nG00 X0.0 Y0.0\n\n", 93.54965676435192, 0.14},
};
}  // namespace golden_geometry_multitool
