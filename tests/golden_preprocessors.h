// ARCHIVO GENERADO por tests/tools/oracle_preprocessors.py - no editar a mano.
// Referencia: postprocesadores originales de FlatCAM (default y GRBL_laser).
#pragma once
#include <array>
#include <vector>
#include "cam/Preprocessor.h"
namespace golden_pp {
inline flatcam::PreprocessorParams scenario0() {  // cutout_mm
    using flatcam::PyNumber; using flatcam::ToolInUse; using flatcam::ExcToolInfo;
    flatcam::PreprocessorParams p;
    p.units = "MM";
    p.coordsDecimals = 4;
    p.decimals = 4;
    p.frDecimals = 2;
    p.feedrate = PyNumber(100.0);
    p.zFeedrate = PyNumber(100.0);
    p.feedrateRapid = PyNumber(1500.0);
    p.zCut = PyNumber(-4.5);
    p.zDepthPerCut = PyNumber(0.6);
    p.zMove = PyNumber(2.0);
    p.zToolchange = PyNumber(19.0);
    p.zEnd = PyNumber(19.0);
    p.dwellTime = PyNumber(1000);
    p.toolC = PyNumber(1.2);
    p.multidepth = false;
    p.useUi = false;
    p.toolchange = false;
    p.fPlunge = false;
    p.stepsPerCircle = 64;
    p.ppExcellonName = "default";
    p.ppGeometryName = "default";
    p.spindleDir = "CW";
    p.tool = 1;
    p.x = 1.5;
    p.y = -2.25;
    p.startZ.reset();
    p.spindleSpeed = PyNumber(1000.0);
    p.xyToolchange.reset();
    p.xyEnd = std::array<PyNumber, 2>{PyNumber(0.0), PyNumber(0.0)};
    p.options.xmin = PyNumber(-37.65);
    p.options.ymin = PyNumber(-39.15);
    p.options.xmax = PyNumber(37.65);
    p.options.ymax = PyNumber(39.15);
    p.options.type = "Geometry";
    p.options.toolDia = PyNumber(1.2);
    return p;
}
inline flatcam::PreprocessorParams scenario1() {  // cutout_inches_multidepth
    using flatcam::PyNumber; using flatcam::ToolInUse; using flatcam::ExcToolInfo;
    flatcam::PreprocessorParams p;
    p.units = "IN";
    p.coordsDecimals = 4;
    p.decimals = 4;
    p.frDecimals = 2;
    p.feedrate = PyNumber(3.9);
    p.zFeedrate = PyNumber(2.36);
    p.feedrateRapid = PyNumber(59);
    p.zCut = PyNumber(-0.07);
    p.zDepthPerCut = PyNumber(0.025);
    p.zMove = PyNumber(0.08);
    p.zToolchange = PyNumber(0.75);
    p.zEnd = PyNumber(0.75);
    p.dwellTime = PyNumber(1000);
    p.toolC = PyNumber(0.047);
    p.multidepth = true;
    p.useUi = false;
    p.toolchange = false;
    p.fPlunge = false;
    p.stepsPerCircle = 64;
    p.ppExcellonName = "default";
    p.ppGeometryName = "default";
    p.spindleDir = "CW";
    p.tool = 1;
    p.x = 1.5;
    p.y = -2.25;
    p.startZ.reset();
    p.spindleSpeed = PyNumber(1000);
    p.xyToolchange.reset();
    p.xyEnd = std::array<PyNumber, 2>{PyNumber(0), PyNumber(0)};
    p.options.xmin = PyNumber(-37.65);
    p.options.ymin = PyNumber(-39.15);
    p.options.xmax = PyNumber(37.65);
    p.options.ymax = PyNumber(39.15);
    p.options.type = "Geometry";
    p.options.toolDia = PyNumber(1.2);
    return p;
}
inline flatcam::PreprocessorParams scenario2() {  // drill_ui_t1
    using flatcam::PyNumber; using flatcam::ToolInUse; using flatcam::ExcToolInfo;
    flatcam::PreprocessorParams p;
    p.units = "MM";
    p.coordsDecimals = 4;
    p.decimals = 4;
    p.frDecimals = 2;
    p.feedrate = PyNumber(100.0);
    p.zFeedrate = PyNumber(100.0);
    p.feedrateRapid = PyNumber(1500.0);
    p.zCut = PyNumber(-3.5);
    p.zDepthPerCut = PyNumber(0.6);
    p.zMove = PyNumber(2.0);
    p.zToolchange = PyNumber(19.0);
    p.zEnd = PyNumber(19.0);
    p.dwellTime = PyNumber(1000);
    p.toolC = PyNumber(0.8);
    p.multidepth = false;
    p.useUi = true;
    p.toolchange = true;
    p.fPlunge = false;
    p.stepsPerCircle = 64;
    p.ppExcellonName = "default";
    p.ppGeometryName = "default";
    p.spindleDir = "CW";
    p.tool = 1;
    p.x = 1.5;
    p.y = -2.25;
    p.startZ.reset();
    p.spindleSpeed = PyNumber(1000);
    p.xyToolchange = std::array<PyNumber, 2>{PyNumber(0.0), PyNumber(0.0)};
    p.xyEnd = std::array<PyNumber, 2>{PyNumber(0.0), PyNumber(0.0)};
    p.options.xmin = PyNumber(-33.962);
    p.options.ymin = PyNumber(-33.554);
    p.options.xmax = PyNumber(32.24);
    p.options.ymax = PyNumber(32.286);
    p.options.type = "Excellon";
    p.options.toolDia = PyNumber(0.8);
    p.options.toolsInUse.push_back(ToolInUse{1, PyNumber(0.8), 90, 0});
    p.options.toolsInUse.push_back(ToolInUse{2, PyNumber(1.0), 96, 0});
    p.options.toolsInUse.push_back(ToolInUse{3, PyNumber(1.5), 2, 0});
    p.options.toolsInUse.push_back(ToolInUse{4, PyNumber(3.4), 4, 0});
    p.excTools.push_back(ExcToolInfo{1, PyNumber(0.8), PyNumber(100.0), PyNumber(1500), PyNumber(-3.5), PyNumber(0.0), PyNumber(0.7), PyNumber(2)});
    p.excTools.push_back(ExcToolInfo{2, PyNumber(1.0), PyNumber(100.0), PyNumber(1500), PyNumber(-3.5), PyNumber(0.0), PyNumber(0.7), PyNumber(2)});
    p.excTools.push_back(ExcToolInfo{3, PyNumber(1.5), PyNumber(100.0), PyNumber(1500), PyNumber(-3.5), PyNumber(0.0), PyNumber(0.7), PyNumber(2)});
    p.excTools.push_back(ExcToolInfo{4, PyNumber(3.4), PyNumber(100.0), PyNumber(1500), PyNumber(-4.5), PyNumber(0.0), PyNumber(0.7), PyNumber(2)});
    p.excCncTools.push_back(ExcToolInfo{1, PyNumber(), PyNumber(), PyNumber(), PyNumber(), PyNumber(0.0), PyNumber(), PyNumber()});
    p.excCncTools.push_back(ExcToolInfo{2, PyNumber(), PyNumber(), PyNumber(), PyNumber(), PyNumber(0.0), PyNumber(), PyNumber()});
    p.excCncTools.push_back(ExcToolInfo{3, PyNumber(), PyNumber(), PyNumber(), PyNumber(), PyNumber(0.5), PyNumber(), PyNumber()});
    p.excCncTools.push_back(ExcToolInfo{4, PyNumber(), PyNumber(), PyNumber(), PyNumber(), PyNumber(0.0), PyNumber(), PyNumber()});
    return p;
}
inline flatcam::PreprocessorParams scenario3() {  // drill_ui_t4
    using flatcam::PyNumber; using flatcam::ToolInUse; using flatcam::ExcToolInfo;
    flatcam::PreprocessorParams p;
    p.units = "MM";
    p.coordsDecimals = 4;
    p.decimals = 4;
    p.frDecimals = 2;
    p.feedrate = PyNumber(100.0);
    p.zFeedrate = PyNumber(100.0);
    p.feedrateRapid = PyNumber(1500.0);
    p.zCut = PyNumber(-4.5);
    p.zDepthPerCut = PyNumber(0.6);
    p.zMove = PyNumber(2.0);
    p.zToolchange = PyNumber(19.0);
    p.zEnd = PyNumber(19.0);
    p.dwellTime = PyNumber(1000);
    p.toolC = PyNumber(3.4);
    p.multidepth = false;
    p.useUi = true;
    p.toolchange = true;
    p.fPlunge = false;
    p.stepsPerCircle = 64;
    p.ppExcellonName = "default";
    p.ppGeometryName = "default";
    p.spindleDir = "CW";
    p.tool = 4;
    p.x = 1.5;
    p.y = -2.25;
    p.startZ.reset();
    p.spindleSpeed = PyNumber(1000);
    p.xyToolchange = std::array<PyNumber, 2>{PyNumber(0.0), PyNumber(0.0)};
    p.xyEnd = std::array<PyNumber, 2>{PyNumber(0.0), PyNumber(0.0)};
    p.options.xmin = PyNumber(-33.962);
    p.options.ymin = PyNumber(-33.554);
    p.options.xmax = PyNumber(32.24);
    p.options.ymax = PyNumber(32.286);
    p.options.type = "Excellon";
    p.options.toolDia = PyNumber(0.8);
    p.options.toolsInUse.push_back(ToolInUse{1, PyNumber(0.8), 90, 0});
    p.options.toolsInUse.push_back(ToolInUse{2, PyNumber(1.0), 96, 0});
    p.options.toolsInUse.push_back(ToolInUse{3, PyNumber(1.5), 2, 0});
    p.options.toolsInUse.push_back(ToolInUse{4, PyNumber(3.4), 4, 0});
    p.excTools.push_back(ExcToolInfo{1, PyNumber(0.8), PyNumber(100.0), PyNumber(1500), PyNumber(-3.5), PyNumber(0.0), PyNumber(0.7), PyNumber(2)});
    p.excTools.push_back(ExcToolInfo{2, PyNumber(1.0), PyNumber(100.0), PyNumber(1500), PyNumber(-3.5), PyNumber(0.0), PyNumber(0.7), PyNumber(2)});
    p.excTools.push_back(ExcToolInfo{3, PyNumber(1.5), PyNumber(100.0), PyNumber(1500), PyNumber(-3.5), PyNumber(0.0), PyNumber(0.7), PyNumber(2)});
    p.excTools.push_back(ExcToolInfo{4, PyNumber(3.4), PyNumber(100.0), PyNumber(1500), PyNumber(-4.5), PyNumber(0.0), PyNumber(0.7), PyNumber(2)});
    p.excCncTools.push_back(ExcToolInfo{1, PyNumber(), PyNumber(), PyNumber(), PyNumber(), PyNumber(0.0), PyNumber(), PyNumber()});
    p.excCncTools.push_back(ExcToolInfo{2, PyNumber(), PyNumber(), PyNumber(), PyNumber(), PyNumber(0.0), PyNumber(), PyNumber()});
    p.excCncTools.push_back(ExcToolInfo{3, PyNumber(), PyNumber(), PyNumber(), PyNumber(), PyNumber(0.5), PyNumber(), PyNumber()});
    p.excCncTools.push_back(ExcToolInfo{4, PyNumber(), PyNumber(), PyNumber(), PyNumber(), PyNumber(0.0), PyNumber(), PyNumber()});
    return p;
}
inline flatcam::PreprocessorParams scenario4() {  // drill_multidepth_startz
    using flatcam::PyNumber; using flatcam::ToolInUse; using flatcam::ExcToolInfo;
    flatcam::PreprocessorParams p;
    p.units = "MM";
    p.coordsDecimals = 4;
    p.decimals = 4;
    p.frDecimals = 2;
    p.feedrate = PyNumber(100.0);
    p.zFeedrate = PyNumber(100.0);
    p.feedrateRapid = PyNumber(1500.0);
    p.zCut = PyNumber(-3.5);
    p.zDepthPerCut = PyNumber(0.7);
    p.zMove = PyNumber(2.0);
    p.zToolchange = PyNumber(19.0);
    p.zEnd = PyNumber(19.0);
    p.dwellTime = PyNumber(1000);
    p.toolC = PyNumber(0.8);
    p.multidepth = true;
    p.useUi = true;
    p.toolchange = true;
    p.fPlunge = true;
    p.stepsPerCircle = 64;
    p.ppExcellonName = "default";
    p.ppGeometryName = "default";
    p.spindleDir = "CW";
    p.tool = 1;
    p.x = 1.5;
    p.y = -2.25;
    p.startZ = PyNumber(1.5);
    p.spindleSpeed = PyNumber(1000.0);
    p.xyToolchange.reset();
    p.xyEnd = std::array<PyNumber, 2>{PyNumber(0.0), PyNumber(0.0)};
    p.options.xmin = PyNumber(-33.962);
    p.options.ymin = PyNumber(-33.554);
    p.options.xmax = PyNumber(32.24);
    p.options.ymax = PyNumber(32.286);
    p.options.type = "Excellon";
    p.options.toolDia = PyNumber(0.8);
    p.options.toolsInUse.push_back(ToolInUse{1, PyNumber(0.8), 90, 0});
    p.options.toolsInUse.push_back(ToolInUse{2, PyNumber(1.0), 96, 0});
    p.options.toolsInUse.push_back(ToolInUse{3, PyNumber(1.5), 2, 0});
    p.options.toolsInUse.push_back(ToolInUse{4, PyNumber(3.4), 4, 0});
    p.excTools.push_back(ExcToolInfo{1, PyNumber(0.8), PyNumber(100.0), PyNumber(1500), PyNumber(-3.5), PyNumber(0.0), PyNumber(0.7), PyNumber(2)});
    p.excTools.push_back(ExcToolInfo{2, PyNumber(1.0), PyNumber(100.0), PyNumber(1500), PyNumber(-3.5), PyNumber(0.0), PyNumber(0.7), PyNumber(2)});
    p.excTools.push_back(ExcToolInfo{3, PyNumber(1.5), PyNumber(100.0), PyNumber(1500), PyNumber(-3.5), PyNumber(0.0), PyNumber(0.7), PyNumber(2)});
    p.excTools.push_back(ExcToolInfo{4, PyNumber(3.4), PyNumber(100.0), PyNumber(1500), PyNumber(-4.5), PyNumber(0.0), PyNumber(0.7), PyNumber(2)});
    p.excCncTools.push_back(ExcToolInfo{1, PyNumber(), PyNumber(), PyNumber(), PyNumber(), PyNumber(0.0), PyNumber(), PyNumber()});
    p.excCncTools.push_back(ExcToolInfo{2, PyNumber(), PyNumber(), PyNumber(), PyNumber(), PyNumber(0.0), PyNumber(), PyNumber()});
    p.excCncTools.push_back(ExcToolInfo{3, PyNumber(), PyNumber(), PyNumber(), PyNumber(), PyNumber(0.5), PyNumber(), PyNumber()});
    p.excCncTools.push_back(ExcToolInfo{4, PyNumber(), PyNumber(), PyNumber(), PyNumber(), PyNumber(0.0), PyNumber(), PyNumber()});
    return p;
}
inline flatcam::PreprocessorParams scenario5() {  // drill_noui
    using flatcam::PyNumber; using flatcam::ToolInUse; using flatcam::ExcToolInfo;
    flatcam::PreprocessorParams p;
    p.units = "MM";
    p.coordsDecimals = 4;
    p.decimals = 4;
    p.frDecimals = 2;
    p.feedrate = PyNumber(100.0);
    p.zFeedrate = PyNumber(100.0);
    p.feedrateRapid = PyNumber(1500.0);
    p.zCut = PyNumber(-4.5);
    p.zDepthPerCut = PyNumber(0.6);
    p.zMove = PyNumber(2.0);
    p.zToolchange = PyNumber(19.0);
    p.zEnd = PyNumber(19.0);
    p.dwellTime = PyNumber(1000);
    p.toolC = PyNumber(1.0);
    p.multidepth = false;
    p.useUi = false;
    p.toolchange = false;
    p.fPlunge = false;
    p.stepsPerCircle = 64;
    p.ppExcellonName = "default";
    p.ppGeometryName = "default";
    p.spindleDir = "CW";
    p.tool = 2;
    p.x = 1.5;
    p.y = -2.25;
    p.startZ.reset();
    p.spindleSpeed = PyNumber(1000);
    p.xyToolchange.reset();
    p.xyEnd = std::array<PyNumber, 2>{PyNumber(0.0), PyNumber(0.0)};
    p.options.xmin = PyNumber(-33.962);
    p.options.ymin = PyNumber(-33.554);
    p.options.xmax = PyNumber(32.24);
    p.options.ymax = PyNumber(32.286);
    p.options.type = "Excellon";
    p.options.toolDia = PyNumber(0.8);
    p.options.toolsInUse.push_back(ToolInUse{1, PyNumber(0.8), 90, 0});
    p.options.toolsInUse.push_back(ToolInUse{2, PyNumber(1.0), 96, 0});
    p.options.toolsInUse.push_back(ToolInUse{3, PyNumber(1.5), 2, 0});
    p.options.toolsInUse.push_back(ToolInUse{4, PyNumber(3.4), 4, 0});
    return p;
}
inline flatcam::PreprocessorParams scenario6() {  // laser_geometry
    using flatcam::PyNumber; using flatcam::ToolInUse; using flatcam::ExcToolInfo;
    flatcam::PreprocessorParams p;
    p.units = "MM";
    p.coordsDecimals = 4;
    p.decimals = 4;
    p.frDecimals = 2;
    p.feedrate = PyNumber(500.0);
    p.zFeedrate = PyNumber(100.0);
    p.feedrateRapid = PyNumber(1500.0);
    p.zCut = PyNumber(-4.5);
    p.zDepthPerCut = PyNumber(0.6);
    p.zMove = PyNumber(2.0);
    p.zToolchange = PyNumber(19.0);
    p.zEnd = PyNumber(19.0);
    p.dwellTime = PyNumber(1000);
    p.toolC = PyNumber(0.1);
    p.multidepth = false;
    p.useUi = false;
    p.toolchange = false;
    p.fPlunge = false;
    p.stepsPerCircle = 64;
    p.ppExcellonName = "default";
    p.ppGeometryName = "GRBL_laser";
    p.spindleDir = "CW";
    p.tool = 1;
    p.x = 1.5;
    p.y = -2.25;
    p.startZ.reset();
    p.spindleSpeed = PyNumber(300.0);
    p.xyToolchange.reset();
    p.xyEnd = std::array<PyNumber, 2>{PyNumber(0.0), PyNumber(0.0)};
    p.options.xmin = PyNumber(-35.4124);
    p.options.ymin = PyNumber(-36.7244);
    p.options.xmax = PyNumber(35.652);
    p.options.ymax = PyNumber(36.906);
    p.options.type = "Geometry";
    p.options.toolDia = PyNumber(0.1);
    return p;
}
inline flatcam::PreprocessorParams scenario7() {  // laser_ccw_nopower
    using flatcam::PyNumber; using flatcam::ToolInUse; using flatcam::ExcToolInfo;
    flatcam::PreprocessorParams p;
    p.units = "MM";
    p.coordsDecimals = 4;
    p.decimals = 4;
    p.frDecimals = 2;
    p.feedrate = PyNumber(500.0);
    p.zFeedrate = PyNumber(100.0);
    p.feedrateRapid = PyNumber(1500.0);
    p.zCut = PyNumber(-4.5);
    p.zDepthPerCut = PyNumber(0.6);
    p.zMove = PyNumber(2.0);
    p.zToolchange = PyNumber(19.0);
    p.zEnd = PyNumber(19.0);
    p.dwellTime = PyNumber(0);
    p.toolC = PyNumber(1.2);
    p.multidepth = false;
    p.useUi = false;
    p.toolchange = false;
    p.fPlunge = false;
    p.stepsPerCircle = 64;
    p.ppExcellonName = "default";
    p.ppGeometryName = "default";
    p.spindleDir = "CCW";
    p.tool = 1;
    p.x = 1.5;
    p.y = -2.25;
    p.startZ.reset();
    p.spindleSpeed.reset();
    p.xyToolchange.reset();
    p.xyEnd.reset();
    p.options.xmin = PyNumber(-37.65);
    p.options.ymin = PyNumber(-39.15);
    p.options.xmax = PyNumber(37.65);
    p.options.ymax = PyNumber(39.15);
    p.options.type = "Geometry";
    p.options.toolDia = PyNumber(1.2);
    return p;
}
inline flatcam::PreprocessorParams scenario(int i) {
    switch (i) {
        case 0: return scenario0();
        case 1: return scenario1();
        case 2: return scenario2();
        case 3: return scenario3();
        case 4: return scenario4();
        case 5: return scenario5();
        case 6: return scenario6();
        case 7: return scenario7();
    }
    return {};
}
inline const char* const kScenarioNames[] = {"cutout_mm", "cutout_inches_multidepth", "drill_ui_t1", "drill_ui_t4", "drill_multidepth_startz", "drill_noui", "laser_geometry", "laser_ccw_nopower"};
struct Case { const char* pp; int scenario; const char* method; const char* expected; };
inline const std::vector<Case> kCases = {
{"default", 0, "startCode", "(This preprocessor is the default preprocessor used by FlatCAM.)\n(It is made to work with MACH3 compatible motion controllers.)\n\n(TOOL DIAMETER: 1.2 mm)\n(Feedrate_XY: 100.0 mm/min)\n(Feedrate_Z: 100.0 mm/min)\n(Feedrate rapids 1500.0 mm/min)\n\n(Z_Cut: -4.5 mm)\n(Z_Move: 2.0 mm)\n(Z Start: None mm)\n(Z End: 19.0 mm)\n(X,Y End: 0.0000, 0.0000 mm)\n(Steps per circle: 64)\n(Preprocessor Geometry: default)\n\n(X range:  -37.6500 ...   37.6500  mm)\n(Y range:  -39.1500 ...   39.1500  mm)\n\n(Spindle Speed: 1000.0 RPM)\nG21\nG90\nG94"},
{"default", 0, "startzCode", ""},
{"default", 0, "liftCode", "G00 Z2.0000"},
{"default", 0, "downCode", "G01 Z-4.5000"},
{"default", 0, "toolchangeCode", "\nM5\nG00 Z19.0000\nT1\nM6    \n(MSG, Change to Tool Dia = 1.2000)\nM0\nG00 Z19.0000\n"},
{"default", 0, "upToZeroCode", "G01 Z0"},
{"default", 0, "rapidCode", "G00 X1.5000 Y-2.2500"},
{"default", 0, "linearCode", "G01 X1.5000 Y-2.2500"},
{"default", 0, "endCode", "G00 Z19.00\nG00 X0.0 Y0.0\n"},
{"default", 0, "feedrateCode", "G01 F100.00"},
{"default", 0, "zFeedrateCode", "G01 F100.00"},
{"default", 0, "spindleCode", "M03 S1000.0"},
{"default", 0, "dwellCode", "G4 P1000"},
{"default", 0, "spindleStopCode", "M05"},
{"GRBL_laser", 0, "startCode", "(This preprocessor is used with a motion controller loaded with GRBL firmware. )\n(It is for the case when it is used together with a LASER connected on the SPINDLE connector.)\n\n(Feedrate: 100.0 mm/min)\n(Feedrate rapids: 1500.0 mm/min)\n\n(Z Focus: 2.0 mm)\n(Steps per circle: 64)\n(Preprocessor Geometry: default)\n\n(X range:  -37.6500 ...   37.6500  mm)\n(Y range:  -39.1500 ...   39.1500  mm)\n\n(Laser Power - Spindle Speed: 1000.0)\n\nG21\nG90\nG17\nG94"},
{"GRBL_laser", 0, "startzCode", ""},
{"GRBL_laser", 0, "liftCode", "M5"},
{"GRBL_laser", 0, "downCode", "M03 S1000.0"},
{"GRBL_laser", 0, "toolchangeCode", ""},
{"GRBL_laser", 0, "upToZeroCode", "M5"},
{"GRBL_laser", 0, "rapidCode", "G00 X1.5000 Y-2.2500"},
{"GRBL_laser", 0, "linearCode", "G01 X1.5000 Y-2.2500 F100.00"},
{"GRBL_laser", 0, "endCode", "G00 Z19.00\nG00 X0.0 Y0.0\n"},
{"GRBL_laser", 0, "feedrateCode", "G01 F100.00"},
{"GRBL_laser", 0, "zFeedrateCode", "G01 F100.00"},
{"GRBL_laser", 0, "spindleCode", "M03 S1000.0"},
{"GRBL_laser", 0, "dwellCode", ""},
{"GRBL_laser", 0, "spindleStopCode", "M5"},
{"default", 1, "startCode", "(This preprocessor is the default preprocessor used by FlatCAM.)\n(It is made to work with MACH3 compatible motion controllers.)\n\n(TOOL DIAMETER: 1.2 in)\n(Feedrate_XY: 3.9 in/min)\n(Feedrate_Z: 2.36 in/min)\n(Feedrate rapids 59 in/min)\n\n(Z_Cut: -0.07 in)\n(DepthPerCut: 0.025 in <=>3 passes)\n(Z_Move: 0.08 in)\n(Z Start: None in)\n(Z End: 0.75 in)\n(X,Y End: 0.0000, 0.0000 in)\n(Steps per circle: 64)\n(Preprocessor Geometry: default)\n\n(X range:  -37.6500 ...   37.6500  in)\n(Y range:  -39.1500 ...   39.1500  in)\n\n(Spindle Speed: 1000 RPM)\nG20\nG90\nG94"},
{"default", 1, "startzCode", ""},
{"default", 1, "liftCode", "G00 Z0.0800"},
{"default", 1, "downCode", "G01 Z-0.0700"},
{"default", 1, "toolchangeCode", "\nM5\nG00 Z0.7500\nT1\nM6    \n(MSG, Change to Tool Dia = 0.0470)\nM0\nG00 Z0.7500\n"},
{"default", 1, "upToZeroCode", "G01 Z0"},
{"default", 1, "rapidCode", "G00 X1.5000 Y-2.2500"},
{"default", 1, "linearCode", "G01 X1.5000 Y-2.2500"},
{"default", 1, "endCode", "G00 Z0.75\nG00 X0 Y0\n"},
{"default", 1, "feedrateCode", "G01 F3.90"},
{"default", 1, "zFeedrateCode", "G01 F2.36"},
{"default", 1, "spindleCode", "M03 S1000"},
{"default", 1, "dwellCode", "G4 P1000"},
{"default", 1, "spindleStopCode", "M05"},
{"GRBL_laser", 1, "startCode", "(This preprocessor is used with a motion controller loaded with GRBL firmware. )\n(It is for the case when it is used together with a LASER connected on the SPINDLE connector.)\n\n(Feedrate: 3.9 in/min)\n(Feedrate rapids: 59 in/min)\n\n(Z Focus: 0.08 in)\n(Steps per circle: 64)\n(Preprocessor Geometry: default)\n\n(X range:  -37.6500 ...   37.6500  in)\n(Y range:  -39.1500 ...   39.1500  in)\n\n(Laser Power - Spindle Speed: 1000)\n\nG20\nG90\nG17\nG94"},
{"GRBL_laser", 1, "startzCode", ""},
{"GRBL_laser", 1, "liftCode", "M5"},
{"GRBL_laser", 1, "downCode", "M03 S1000"},
{"GRBL_laser", 1, "toolchangeCode", ""},
{"GRBL_laser", 1, "upToZeroCode", "M5"},
{"GRBL_laser", 1, "rapidCode", "G00 X1.5000 Y-2.2500"},
{"GRBL_laser", 1, "linearCode", "G01 X1.5000 Y-2.2500 F3.90"},
{"GRBL_laser", 1, "endCode", "G00 Z0.75\nG00 X0 Y0\n"},
{"GRBL_laser", 1, "feedrateCode", "G01 F3.90"},
{"GRBL_laser", 1, "zFeedrateCode", "G01 F2.36"},
{"GRBL_laser", 1, "spindleCode", "M03 S1000"},
{"GRBL_laser", 1, "dwellCode", ""},
{"GRBL_laser", 1, "spindleStopCode", "M5"},
{"default", 2, "startCode", "(This preprocessor is the default preprocessor used by FlatCAM.)\n(It is made to work with MACH3 compatible motion controllers.)\n\n\n(TOOLS DIAMETER: )\n(Tool: 1 -> Dia: 0.8)\n(Tool: 2 -> Dia: 1.0)\n(Tool: 3 -> Dia: 1.5)\n(Tool: 4 -> Dia: 3.4)\n\n(FEEDRATE Z: )\n(Tool: 1 -> Feedrate: 100.0)\n(Tool: 2 -> Feedrate: 100.0)\n(Tool: 3 -> Feedrate: 100.0)\n(Tool: 4 -> Feedrate: 100.0)\n\n(FEEDRATE RAPIDS: )\n(Tool: 1 -> Feedrate Rapids: 1500)\n(Tool: 2 -> Feedrate Rapids: 1500)\n(Tool: 3 -> Feedrate Rapids: 1500)\n(Tool: 4 -> Feedrate Rapids: 1500)\n\n(Z_CUT: )\n(Tool: 1 -> Z_Cut: -3.5)\n(Tool: 2 -> Z_Cut: -3.5)\n(Tool: 3 -> Z_Cut: -3.5)\n(Tool: 4 -> Z_Cut: -4.5)\n\n(Tools Offset: )\n(Tool: 1 -> Offset Z: 0.0)\n(Tool: 2 -> Offset Z: 0.0)\n(Tool: 3 -> Offset Z: 0.5)\n(Tool: 4 -> Offset Z: 0.0)\n\n(Z_MOVE: )\n(Tool: 1 -> Z_Move: 2)\n(Tool: 2 -> Z_Move: 2)\n(Tool: 3 -> Z_Move: 2)\n(Tool: 4 -> Z_Move: 2)\n\n(Z Toolchange: 19.0 mm)\n(X,Y Toolchange: 0.0000, 0.0000 mm)\n(Z Start: None mm)\n(Z End: 19.0 mm)\n(X,Y End: 0.0000, 0.0000 mm)\n(Steps per circle: 64)\n(Preprocessor Excellon: default)\n\n(X range:  -33.9620 ...   32.2400  mm)\n(Y range:  -33.5540 ...   32.2860  mm)\n\n(Spindle Speed: 1000 RPM)\nG21\nG90\nG94"},
{"default", 2, "startzCode", ""},
{"default", 2, "liftCode", "G00 Z2.0000"},
{"default", 2, "downCode", "G01 Z-3.5000"},
{"default", 2, "toolchangeCode", "\nM5\nG00 Z19.0000\nT1\nG00 X0.0000 Y0.0000                \nM6\n(MSG, Change to Tool Dia = 0.8000 ||| Total drills for tool T1 = 90)\nM0\nG00 Z19.0000\n"},
{"default", 2, "upToZeroCode", "G01 Z0"},
{"default", 2, "rapidCode", "G00 X1.5000 Y-2.2500"},
{"default", 2, "linearCode", "G01 X1.5000 Y-2.2500"},
{"default", 2, "endCode", "G00 Z19.00\nG00 X0.0 Y0.0\n"},
{"default", 2, "feedrateCode", "G01 F100.00"},
{"default", 2, "zFeedrateCode", "G01 F100.00"},
{"default", 2, "spindleCode", "M03 S1000"},
{"default", 2, "dwellCode", "G4 P1000"},
{"default", 2, "spindleStopCode", "M05"},
{"GRBL_laser", 2, "startCode", "(This preprocessor is used with a motion controller loaded with GRBL firmware. )\n(It is for the case when it is used together with a LASER connected on the SPINDLE connector.)\n\n(Feedrate: 100.0 mm/min)\n(Feedrate rapids: 1500.0 mm/min)\n\n(Z Focus: 2.0 mm)\n(Steps per circle: 64)\n(Preprocessor Excellon: default)\n(X range:  -33.9620 ...   32.2400  mm)\n(Y range:  -33.5540 ...   32.2860  mm)\n\n(Laser Power - Spindle Speed: 1000)\n\nG21\nG90\nG17\nG94"},
{"GRBL_laser", 2, "startzCode", ""},
{"GRBL_laser", 2, "liftCode", "M5"},
{"GRBL_laser", 2, "downCode", "M03 S1000"},
{"GRBL_laser", 2, "toolchangeCode", ""},
{"GRBL_laser", 2, "upToZeroCode", "M5"},
{"GRBL_laser", 2, "rapidCode", "G00 X1.5000 Y-2.2500"},
{"GRBL_laser", 2, "linearCode", "G01 X1.5000 Y-2.2500 F100.00"},
{"GRBL_laser", 2, "endCode", "G00 Z19.00\nG00 X0.0 Y0.0\n"},
{"GRBL_laser", 2, "feedrateCode", "G01 F100.00"},
{"GRBL_laser", 2, "zFeedrateCode", "G01 F100.00"},
{"GRBL_laser", 2, "spindleCode", "M03 S1000"},
{"GRBL_laser", 2, "dwellCode", ""},
{"GRBL_laser", 2, "spindleStopCode", "M5"},
{"default", 3, "startCode", "(This preprocessor is the default preprocessor used by FlatCAM.)\n(It is made to work with MACH3 compatible motion controllers.)\n\n\n(TOOLS DIAMETER: )\n(Tool: 1 -> Dia: 0.8)\n(Tool: 2 -> Dia: 1.0)\n(Tool: 3 -> Dia: 1.5)\n(Tool: 4 -> Dia: 3.4)\n\n(FEEDRATE Z: )\n(Tool: 1 -> Feedrate: 100.0)\n(Tool: 2 -> Feedrate: 100.0)\n(Tool: 3 -> Feedrate: 100.0)\n(Tool: 4 -> Feedrate: 100.0)\n\n(FEEDRATE RAPIDS: )\n(Tool: 1 -> Feedrate Rapids: 1500)\n(Tool: 2 -> Feedrate Rapids: 1500)\n(Tool: 3 -> Feedrate Rapids: 1500)\n(Tool: 4 -> Feedrate Rapids: 1500)\n\n(Z_CUT: )\n(Tool: 1 -> Z_Cut: -3.5)\n(Tool: 2 -> Z_Cut: -3.5)\n(Tool: 3 -> Z_Cut: -3.5)\n(Tool: 4 -> Z_Cut: -4.5)\n\n(Tools Offset: )\n(Tool: 1 -> Offset Z: 0.0)\n(Tool: 2 -> Offset Z: 0.0)\n(Tool: 3 -> Offset Z: 0.5)\n(Tool: 4 -> Offset Z: 0.0)\n\n(Z_MOVE: )\n(Tool: 1 -> Z_Move: 2)\n(Tool: 2 -> Z_Move: 2)\n(Tool: 3 -> Z_Move: 2)\n(Tool: 4 -> Z_Move: 2)\n\n(Z Toolchange: 19.0 mm)\n(X,Y Toolchange: 0.0000, 0.0000 mm)\n(Z Start: None mm)\n(Z End: 19.0 mm)\n(X,Y End: 0.0000, 0.0000 mm)\n(Steps per circle: 64)\n(Preprocessor Excellon: default)\n\n(X range:  -33.9620 ...   32.2400  mm)\n(Y range:  -33.5540 ...   32.2860  mm)\n\n(Spindle Speed: 1000 RPM)\nG21\nG90\nG94"},
{"default", 3, "startzCode", ""},
{"default", 3, "liftCode", "G00 Z2.0000"},
{"default", 3, "downCode", "G01 Z-4.5000"},
{"default", 3, "toolchangeCode", "\nM5\nG00 Z19.0000\nT4\nG00 X0.0000 Y0.0000                \nM6\n(MSG, Change to Tool Dia = 3.4000 ||| Total drills for tool T4 = 4)\nM0\nG00 Z19.0000\n"},
{"default", 3, "upToZeroCode", "G01 Z0"},
{"default", 3, "rapidCode", "G00 X1.5000 Y-2.2500"},
{"default", 3, "linearCode", "G01 X1.5000 Y-2.2500"},
{"default", 3, "endCode", "G00 Z19.00\nG00 X0.0 Y0.0\n"},
{"default", 3, "feedrateCode", "G01 F100.00"},
{"default", 3, "zFeedrateCode", "G01 F100.00"},
{"default", 3, "spindleCode", "M03 S1000"},
{"default", 3, "dwellCode", "G4 P1000"},
{"default", 3, "spindleStopCode", "M05"},
{"GRBL_laser", 3, "startCode", "(This preprocessor is used with a motion controller loaded with GRBL firmware. )\n(It is for the case when it is used together with a LASER connected on the SPINDLE connector.)\n\n(Feedrate: 100.0 mm/min)\n(Feedrate rapids: 1500.0 mm/min)\n\n(Z Focus: 2.0 mm)\n(Steps per circle: 64)\n(Preprocessor Excellon: default)\n(X range:  -33.9620 ...   32.2400  mm)\n(Y range:  -33.5540 ...   32.2860  mm)\n\n(Laser Power - Spindle Speed: 1000)\n\nG21\nG90\nG17\nG94"},
{"GRBL_laser", 3, "startzCode", ""},
{"GRBL_laser", 3, "liftCode", "M5"},
{"GRBL_laser", 3, "downCode", "M03 S1000"},
{"GRBL_laser", 3, "toolchangeCode", ""},
{"GRBL_laser", 3, "upToZeroCode", "M5"},
{"GRBL_laser", 3, "rapidCode", "G00 X1.5000 Y-2.2500"},
{"GRBL_laser", 3, "linearCode", "G01 X1.5000 Y-2.2500 F100.00"},
{"GRBL_laser", 3, "endCode", "G00 Z19.00\nG00 X0.0 Y0.0\n"},
{"GRBL_laser", 3, "feedrateCode", "G01 F100.00"},
{"GRBL_laser", 3, "zFeedrateCode", "G01 F100.00"},
{"GRBL_laser", 3, "spindleCode", "M03 S1000"},
{"GRBL_laser", 3, "dwellCode", ""},
{"GRBL_laser", 3, "spindleStopCode", "M5"},
{"default", 4, "startCode", "(This preprocessor is the default preprocessor used by FlatCAM.)\n(It is made to work with MACH3 compatible motion controllers.)\n\n\n(TOOLS DIAMETER: )\n(Tool: 1 -> Dia: 0.8)\n(Tool: 2 -> Dia: 1.0)\n(Tool: 3 -> Dia: 1.5)\n(Tool: 4 -> Dia: 3.4)\n\n(FEEDRATE Z: )\n(Tool: 1 -> Feedrate: 100.0)\n(Tool: 2 -> Feedrate: 100.0)\n(Tool: 3 -> Feedrate: 100.0)\n(Tool: 4 -> Feedrate: 100.0)\n\n(FEEDRATE RAPIDS: )\n(Tool: 1 -> Feedrate Rapids: 1500)\n(Tool: 2 -> Feedrate Rapids: 1500)\n(Tool: 3 -> Feedrate Rapids: 1500)\n(Tool: 4 -> Feedrate Rapids: 1500)\n\n(Z_CUT: )\n(Tool: 1 -> Z_Cut: -3.5)\n(Tool: 2 -> Z_Cut: -3.5)\n(Tool: 3 -> Z_Cut: -3.5)\n(Tool: 4 -> Z_Cut: -4.5)\n\n(Tools Offset: )\n(Tool: 1 -> Offset Z: 0.0)\n(Tool: 2 -> Offset Z: 0.0)\n(Tool: 3 -> Offset Z: 0.5)\n(Tool: 4 -> Offset Z: 0.0)\n\n(DEPTH_PER_CUT: )\n(Tool: 1 -> DeptPerCut: 0.7)\n(Tool: 2 -> DeptPerCut: 0.7)\n(Tool: 3 -> DeptPerCut: 0.7)\n(Tool: 4 -> DeptPerCut: 0.7)\n\n(Z_MOVE: )\n(Tool: 1 -> Z_Move: 2)\n(Tool: 2 -> Z_Move: 2)\n(Tool: 3 -> Z_Move: 2)\n(Tool: 4 -> Z_Move: 2)\n\n(Z Toolchange: 19.0 mm)\n(X,Y Toolchange: None mm)\n(Z Start: 1.5 mm)\n(Z End: 19.0 mm)\n(X,Y End: 0.0000, 0.0000 mm)\n(Steps per circle: 64)\n(Preprocessor Excellon: default)\n\n(X range:  -33.9620 ...   32.2400  mm)\n(Y range:  -33.5540 ...   32.2860  mm)\n\n(Spindle Speed: 1000.0 RPM)\nG21\nG90\nG94"},
{"default", 4, "startzCode", "G00 Z1.5000"},
{"default", 4, "liftCode", "G00 Z2.0000"},
{"default", 4, "downCode", "G01 Z-3.5000"},
{"default", 4, "toolchangeCode", "\nM5       \nG00 Z1.5000\nT1\nM6\n(MSG, Change to Tool Dia = 0.8000 ||| Total drills for tool T1 = 90)\nM0\nG00 Z1.5000\n\nG00 Z2.0000"},
{"default", 4, "upToZeroCode", "G01 Z0"},
{"default", 4, "rapidCode", "G00 X1.5000 Y-2.2500"},
{"default", 4, "linearCode", "G01 X1.5000 Y-2.2500"},
{"default", 4, "endCode", "G00 Z19.00\nG00 X0.0 Y0.0\n"},
{"default", 4, "feedrateCode", "G01 F100.00"},
{"default", 4, "zFeedrateCode", "G01 F100.00"},
{"default", 4, "spindleCode", "M03 S1000.0"},
{"default", 4, "dwellCode", "G4 P1000"},
{"default", 4, "spindleStopCode", "M05"},
{"GRBL_laser", 4, "startCode", "(This preprocessor is used with a motion controller loaded with GRBL firmware. )\n(It is for the case when it is used together with a LASER connected on the SPINDLE connector.)\n\n(Feedrate: 100.0 mm/min)\n(Feedrate rapids: 1500.0 mm/min)\n\n(Z Focus: 2.0 mm)\n(Steps per circle: 64)\n(Preprocessor Excellon: default)\n(X range:  -33.9620 ...   32.2400  mm)\n(Y range:  -33.5540 ...   32.2860  mm)\n\n(Laser Power - Spindle Speed: 1000.0)\n\nG21\nG90\nG17\nG94"},
{"GRBL_laser", 4, "startzCode", ""},
{"GRBL_laser", 4, "liftCode", "M5"},
{"GRBL_laser", 4, "downCode", "M03 S1000.0"},
{"GRBL_laser", 4, "toolchangeCode", ""},
{"GRBL_laser", 4, "upToZeroCode", "M5"},
{"GRBL_laser", 4, "rapidCode", "G00 X1.5000 Y-2.2500"},
{"GRBL_laser", 4, "linearCode", "G01 X1.5000 Y-2.2500 F100.00"},
{"GRBL_laser", 4, "endCode", "G00 Z19.00\nG00 X0.0 Y0.0\n"},
{"GRBL_laser", 4, "feedrateCode", "G01 F100.00"},
{"GRBL_laser", 4, "zFeedrateCode", "G01 F100.00"},
{"GRBL_laser", 4, "spindleCode", "M03 S1000.0"},
{"GRBL_laser", 4, "dwellCode", ""},
{"GRBL_laser", 4, "spindleStopCode", "M5"},
{"default", 5, "startCode", "(This preprocessor is the default preprocessor used by FlatCAM.)\n(It is made to work with MACH3 compatible motion controllers.)\n\n(Z Start: None mm)\n(Z End: 19.0 mm)\n(X,Y End: 0.0000, 0.0000 mm)\n(Steps per circle: 64)\n(Preprocessor Excellon: default)\n\n(X range:  -33.9620 ...   32.2400  mm)\n(Y range:  -33.5540 ...   32.2860  mm)\n\n(Spindle Speed: 1000 RPM)\nG21\nG90\nG94"},
{"default", 5, "startzCode", ""},
{"default", 5, "liftCode", "G00 Z2.0000"},
{"default", 5, "downCode", "G01 Z-4.5000"},
{"default", 5, "toolchangeCode", "\nM5       \nG00 Z19.0000\nT2\nM6\n(MSG, Change to Tool Dia = 1.0000 ||| Total drills for tool T2 = 96)\nM0\nG00 Z19.0000\n"},
{"default", 5, "upToZeroCode", "G01 Z0"},
{"default", 5, "rapidCode", "G00 X1.5000 Y-2.2500"},
{"default", 5, "linearCode", "G01 X1.5000 Y-2.2500"},
{"default", 5, "endCode", "G00 Z19.00\nG00 X0.0 Y0.0\n"},
{"default", 5, "feedrateCode", "G01 F100.00"},
{"default", 5, "zFeedrateCode", "G01 F100.00"},
{"default", 5, "spindleCode", "M03 S1000"},
{"default", 5, "dwellCode", "G4 P1000"},
{"default", 5, "spindleStopCode", "M05"},
{"GRBL_laser", 5, "startCode", "(This preprocessor is used with a motion controller loaded with GRBL firmware. )\n(It is for the case when it is used together with a LASER connected on the SPINDLE connector.)\n\n(Feedrate: 100.0 mm/min)\n(Feedrate rapids: 1500.0 mm/min)\n\n(Z Focus: 2.0 mm)\n(Steps per circle: 64)\n(Preprocessor Excellon: default)\n(X range:  -33.9620 ...   32.2400  mm)\n(Y range:  -33.5540 ...   32.2860  mm)\n\n(Laser Power - Spindle Speed: 1000)\n\nG21\nG90\nG17\nG94"},
{"GRBL_laser", 5, "startzCode", ""},
{"GRBL_laser", 5, "liftCode", "M5"},
{"GRBL_laser", 5, "downCode", "M03 S1000"},
{"GRBL_laser", 5, "toolchangeCode", ""},
{"GRBL_laser", 5, "upToZeroCode", "M5"},
{"GRBL_laser", 5, "rapidCode", "G00 X1.5000 Y-2.2500"},
{"GRBL_laser", 5, "linearCode", "G01 X1.5000 Y-2.2500 F100.00"},
{"GRBL_laser", 5, "endCode", "G00 Z19.00\nG00 X0.0 Y0.0\n"},
{"GRBL_laser", 5, "feedrateCode", "G01 F100.00"},
{"GRBL_laser", 5, "zFeedrateCode", "G01 F100.00"},
{"GRBL_laser", 5, "spindleCode", "M03 S1000"},
{"GRBL_laser", 5, "dwellCode", ""},
{"GRBL_laser", 5, "spindleStopCode", "M5"},
{"default", 6, "startCode", "(This preprocessor is the default preprocessor used by FlatCAM.)\n(It is made to work with MACH3 compatible motion controllers.)\n\n(TOOL DIAMETER: 0.1 mm)\n(Feedrate_XY: 500.0 mm/min)\n(Feedrate_Z: 100.0 mm/min)\n(Feedrate rapids 1500.0 mm/min)\n\n(Z_Cut: -4.5 mm)\n(Z_Move: 2.0 mm)\n(Z Start: None mm)\n(Z End: 19.0 mm)\n(X,Y End: 0.0000, 0.0000 mm)\n(Steps per circle: 64)\n(Preprocessor Geometry: GRBL_laser)\n\n(X range:  -35.4124 ...   35.6520  mm)\n(Y range:  -36.7244 ...   36.9060  mm)\n\n(Spindle Speed: 300.0 RPM)\nG21\nG90\nG94"},
{"default", 6, "startzCode", ""},
{"default", 6, "liftCode", "G00 Z2.0000"},
{"default", 6, "downCode", "G01 Z-4.5000"},
{"default", 6, "toolchangeCode", "\nM5\nG00 Z19.0000\nT1\nM6    \n(MSG, Change to Tool Dia = 0.1000)\nM0\nG00 Z19.0000\n"},
{"default", 6, "upToZeroCode", "G01 Z0"},
{"default", 6, "rapidCode", "G00 X1.5000 Y-2.2500"},
{"default", 6, "linearCode", "G01 X1.5000 Y-2.2500"},
{"default", 6, "endCode", "G00 Z19.00\nG00 X0.0 Y0.0\n"},
{"default", 6, "feedrateCode", "G01 F500.00"},
{"default", 6, "zFeedrateCode", "G01 F100.00"},
{"default", 6, "spindleCode", "M03 S300.0"},
{"default", 6, "dwellCode", "G4 P1000"},
{"default", 6, "spindleStopCode", "M05"},
{"GRBL_laser", 6, "startCode", "(This preprocessor is used with a motion controller loaded with GRBL firmware. )\n(It is for the case when it is used together with a LASER connected on the SPINDLE connector.)\n\n(Feedrate: 500.0 mm/min)\n(Feedrate rapids: 1500.0 mm/min)\n\n(Z Focus: 2.0 mm)\n(Steps per circle: 64)\n(Preprocessor Geometry: GRBL_laser)\n\n(X range:  -35.4124 ...   35.6520  mm)\n(Y range:  -36.7244 ...   36.9060  mm)\n\n(Laser Power - Spindle Speed: 300.0)\n\nG21\nG90\nG17\nG94"},
{"GRBL_laser", 6, "startzCode", ""},
{"GRBL_laser", 6, "liftCode", "M5"},
{"GRBL_laser", 6, "downCode", "M03 S300.0"},
{"GRBL_laser", 6, "toolchangeCode", ""},
{"GRBL_laser", 6, "upToZeroCode", "M5"},
{"GRBL_laser", 6, "rapidCode", "G00 X1.5000 Y-2.2500"},
{"GRBL_laser", 6, "linearCode", "G01 X1.5000 Y-2.2500 F500.00"},
{"GRBL_laser", 6, "endCode", "G00 Z19.00\nG00 X0.0 Y0.0\n"},
{"GRBL_laser", 6, "feedrateCode", "G01 F500.00"},
{"GRBL_laser", 6, "zFeedrateCode", "G01 F100.00"},
{"GRBL_laser", 6, "spindleCode", "M03 S300.0"},
{"GRBL_laser", 6, "dwellCode", ""},
{"GRBL_laser", 6, "spindleStopCode", "M5"},
{"default", 7, "startCode", "(This preprocessor is the default preprocessor used by FlatCAM.)\n(It is made to work with MACH3 compatible motion controllers.)\n\n(TOOL DIAMETER: 1.2 mm)\n(Feedrate_XY: 500.0 mm/min)\n(Feedrate_Z: 100.0 mm/min)\n(Feedrate rapids 1500.0 mm/min)\n\n(Z_Cut: -4.5 mm)\n(Z_Move: 2.0 mm)\n(Z Start: None mm)\n(Z End: 19.0 mm)\n(X,Y End: None mm)\n(Steps per circle: 64)\n(Preprocessor Geometry: default)\n\n(X range:  -37.6500 ...   37.6500  mm)\n(Y range:  -39.1500 ...   39.1500  mm)\n\n(Spindle Speed: None RPM)\nG21\nG90\nG94"},
{"default", 7, "startzCode", ""},
{"default", 7, "liftCode", "G00 Z2.0000"},
{"default", 7, "downCode", "G01 Z-4.5000"},
{"default", 7, "toolchangeCode", "\nM5\nG00 Z19.0000\nT1\nM6    \n(MSG, Change to Tool Dia = 1.2000)\nM0\nG00 Z19.0000\n"},
{"default", 7, "upToZeroCode", "G01 Z0"},
{"default", 7, "rapidCode", "G00 X1.5000 Y-2.2500"},
{"default", 7, "linearCode", "G01 X1.5000 Y-2.2500"},
{"default", 7, "endCode", "G00 Z19.00\n"},
{"default", 7, "feedrateCode", "G01 F500.00"},
{"default", 7, "zFeedrateCode", "G01 F100.00"},
{"default", 7, "spindleCode", "M04"},
{"default", 7, "dwellCode", ""},
{"default", 7, "spindleStopCode", "M05"},
{"GRBL_laser", 7, "startCode", "(This preprocessor is used with a motion controller loaded with GRBL firmware. )\n(It is for the case when it is used together with a LASER connected on the SPINDLE connector.)\n\n(Feedrate: 500.0 mm/min)\n(Feedrate rapids: 1500.0 mm/min)\n\n(Z Focus: 2.0 mm)\n(Steps per circle: 64)\n(Preprocessor Geometry: default)\n\n(X range:  -37.6500 ...   37.6500  mm)\n(Y range:  -39.1500 ...   39.1500  mm)\n\n(Laser Power - Spindle Speed: None)\n\nG21\nG90\nG17\nG94"},
{"GRBL_laser", 7, "startzCode", ""},
{"GRBL_laser", 7, "liftCode", "M5"},
{"GRBL_laser", 7, "downCode", "M04"},
{"GRBL_laser", 7, "toolchangeCode", ""},
{"GRBL_laser", 7, "upToZeroCode", "M5"},
{"GRBL_laser", 7, "rapidCode", "G00 X1.5000 Y-2.2500"},
{"GRBL_laser", 7, "linearCode", "G01 X1.5000 Y-2.2500 F500.00"},
{"GRBL_laser", 7, "endCode", "G00 Z19.00\n"},
{"GRBL_laser", 7, "feedrateCode", "G01 F500.00"},
{"GRBL_laser", 7, "zFeedrateCode", "G01 F100.00"},
{"GRBL_laser", 7, "spindleCode", "M04"},
{"GRBL_laser", 7, "dwellCode", ""},
{"GRBL_laser", 7, "spindleStopCode", "M5"},
};
struct FloatCase { double value; const char* repr; };
inline const std::vector<FloatCase> kFloats = {
{0.0, "0.0"},
{1.0, "1.0"},
{100.0, "100.0"},
{0.8, "0.8"},
{1500.0, "1500.0"},
{1e-05, "1e-05"},
{0.0001, "0.0001"},
{0.00012345, "0.00012345"},
{123456789012345.0, "123456789012345.0"},
{1000000000000000.0, "1000000000000000.0"},
{1e+16, "1e+16"},
{1.5e+16, "1.5e+16"},
{1.2345678901234567e+19, "1.2345678901234567e+19"},
{-3.5, "-3.5"},
{0.30000000000000004, "0.30000000000000004"},
{2.54, "2.54"},
{0.03937007874015748, "0.03937007874015748"},
{19.0, "19.0"},
{0.07, "0.07"},
{25.4, "25.4"},
{3e-10, "3e-10"},
{123.456, "123.456"},
{-0.0, "-0.0"},
{5e-324, "5e-324"},
{1.7976931348623157e+308, "1.7976931348623157e+308"},
{0.5, "0.5"},
{2.5e-05, "2.5e-05"},
{1234567.125, "1234567.125"},
};
}  // namespace golden_pp
