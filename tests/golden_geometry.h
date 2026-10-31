// ARCHIVO GENERADO por tests/tools/oracle_geometry.py - no editar a mano.
// Referencia: CNCjob.geometry_tool_gcode_gen original (FlatCAM 2024.4) con el contorno del usuario.
#pragma once
#include <array>
#include <string>
#include <vector>
#include "cam/CncJob.h"
namespace golden_geometry {
inline const std::vector<std::vector<flatcam::Point2D>> kPaths = {{{-0.8, -39.15}, {-37.05, -39.15}, {-37.1088, -39.1471}, {-37.1671, -39.1385}, {-37.2242, -39.1242}, {-37.2796, -39.1044}, {-37.3329, -39.0792}, {-37.3834, -39.0489}, {-37.4307, -39.0138}, {-37.4743, -38.9743}, {-37.5138, -38.9307}, {-37.5489, -38.8834}, {-37.5792, -38.8329}, {-37.6044, -38.7796}, {-37.6242, -38.7242}, {-37.6385, -38.6671}, {-37.6471, -38.6088}, {-37.65, -38.55}, {-37.65, 38.55}, {-37.6471, 38.6088}, {-37.6385, 38.6671}, {-37.6242, 38.7242}, {-37.6044, 38.7796}, {-37.5792, 38.8329}, {-37.5489, 38.8834}, {-37.5138, 38.9307}, {-37.4743, 38.9743}, {-37.4307, 39.0138}, {-37.3834, 39.0489}, {-37.3329, 39.0792}, {-37.2796, 39.1044}, {-37.2242, 39.1242}, {-37.1671, 39.1385}, {-37.1088, 39.1471}, {-37.05, 39.15}, {-0.8, 39.15}}, {{0.8, 39.15}, {37.05, 39.15}, {37.1088, 39.1471}, {37.1671, 39.1385}, {37.2242, 39.1242}, {37.2796, 39.1044}, {37.3329, 39.0792}, {37.3834, 39.0489}, {37.4307, 39.0138}, {37.4743, 38.9743}, {37.5138, 38.9307}, {37.5489, 38.8834}, {37.5792, 38.8329}, {37.6044, 38.7796}, {37.6242, 38.7242}, {37.6385, 38.6671}, {37.6471, 38.6088}, {37.65, 38.55}, {37.65, -38.55}, {37.6471, -38.6088}, {37.6385, -38.6671}, {37.6242, -38.7242}, {37.6044, -38.7796}, {37.5792, -38.8329}, {37.5489, -38.8834}, {37.5138, -38.9307}, {37.4743, -38.9743}, {37.4307, -39.0138}, {37.3834, -39.0489}, {37.3329, -39.0792}, {37.2796, -39.1044}, {37.2242, -39.1242}, {37.1671, -39.1385}, {37.1088, -39.1471}, {37.05, -39.15}, {0.8, -39.15}}};
inline flatcam::GeometryJobSettings settings() {
    flatcam::GeometryJobSettings s;
    s.units = "MM";
    s.order = flatcam::DrillOrder::FileOrder;
    s.tool = 1;
    flatcam::GeometryToolSettings& t = s.toolSettings;
    t.toolDia = 1.2;
    t.offsetKind = "Path";
    t.offsetValue = 0.0;
    t.cutZ = -4.5;
    t.depthPerPass = 0.6;
    t.travelZ = 2.0;
    t.feedrate = 100.0;
    t.feedrateZ = 100.0;
    t.feedrateRapid = 1500.0;
    t.spindleSpeed = 1000.0;
    t.dwellTime = 1.0;
    t.endZ = 19.0;
    t.toolchangeZ = 19.0;
    t.spindleDir = "CW";
    t.multidepth = false;
    t.dwell = false;
    t.extracut = false;
    t.endXY = std::array<double, 2>{0.0, 0.0};
    t.toolchangeXY = std::array<double, 2>{0.0, 0.0};
    return s;
}
inline flatcam::GcodeHeaderInfo header() {
    flatcam::GcodeHeaderInfo h;
    h.version = "8.994";
    h.versionDate = "2020/11/7";
    h.name = "PCB ppal-Edge_Cuts.gm1_cutout_cnc";
    h.type = "Geometry";
    h.units = "MM";
    h.createdOn = "Monday, 11 November 2024 at 15:36";
    return h;
}
inline const std::string kStartGcode = "(This preprocessor is the default preprocessor used by FlatCAM.)\n(It is made to work with MACH3 compatible motion controllers.)\n\n(TOOL DIAMETER: 1.2 mm)\n(Feedrate_XY: 100.0 mm/min)\n(Feedrate_Z: 100.0 mm/min)\n(Feedrate rapids 1500.0 mm/min)\n\n(Z_Cut: -4.5 mm)\n(Z_Move: 2.0 mm)\n(Z Start: None mm)\n(Z End: 19.0 mm)\n(X,Y End: 0.0000, 0.0000 mm)\n(Steps per circle: 64)\n(Preprocessor Geometry: default)\n\n(X range:  -37.6500 ...   37.6500  mm)\n(Y range:  -39.1500 ...   39.1500  mm)\n\n(Spindle Speed: 1000.0 RPM)\nG21\nG90\nG94\n";
inline const std::string kToolGcode = "G01 F100.00\n\nM5\nG00 Z19.0000\nG00 X0.0000 Y0.0000\nT1\nM6    \n(MSG, Change to Tool Dia = 1.2000)\nM0\nG00 Z19.0000\n\nM03 S1000.0\nG01 F100.00\nG00 X-0.8000 Y-39.1500\nG01 F100.00\nG01 Z-4.5000\nG01 F100.00\nG01 X-37.0500 Y-39.1500\nG01 X-37.1088 Y-39.1471\nG01 X-37.1671 Y-39.1385\nG01 X-37.2242 Y-39.1242\nG01 X-37.2796 Y-39.1044\nG01 X-37.3329 Y-39.0792\nG01 X-37.3834 Y-39.0489\nG01 X-37.4307 Y-39.0138\nG01 X-37.4743 Y-38.9743\nG01 X-37.5138 Y-38.9307\nG01 X-37.5489 Y-38.8834\nG01 X-37.5792 Y-38.8329\nG01 X-37.6044 Y-38.7796\nG01 X-37.6242 Y-38.7242\nG01 X-37.6385 Y-38.6671\nG01 X-37.6471 Y-38.6088\nG01 X-37.6500 Y-38.5500\nG01 X-37.6500 Y38.5500\nG01 X-37.6471 Y38.6088\nG01 X-37.6385 Y38.6671\nG01 X-37.6242 Y38.7242\nG01 X-37.6044 Y38.7796\nG01 X-37.5792 Y38.8329\nG01 X-37.5489 Y38.8834\nG01 X-37.5138 Y38.9307\nG01 X-37.4743 Y38.9743\nG01 X-37.4307 Y39.0138\nG01 X-37.3834 Y39.0489\nG01 X-37.3329 Y39.0792\nG01 X-37.2796 Y39.1044\nG01 X-37.2242 Y39.1242\nG01 X-37.1671 Y39.1385\nG01 X-37.1088 Y39.1471\nG01 X-37.0500 Y39.1500\nG01 X-0.8000 Y39.1500\nG00 Z2.0000\nG00 X0.8000 Y39.1500\nG01 F100.00\nG01 Z-4.5000\nG01 F100.00\nG01 X37.0500 Y39.1500\nG01 X37.1088 Y39.1471\nG01 X37.1671 Y39.1385\nG01 X37.2242 Y39.1242\nG01 X37.2796 Y39.1044\nG01 X37.3329 Y39.0792\nG01 X37.3834 Y39.0489\nG01 X37.4307 Y39.0138\nG01 X37.4743 Y38.9743\nG01 X37.5138 Y38.9307\nG01 X37.5489 Y38.8834\nG01 X37.5792 Y38.8329\nG01 X37.6044 Y38.7796\nG01 X37.6242 Y38.7242\nG01 X37.6385 Y38.6671\nG01 X37.6471 Y38.6088\nG01 X37.6500 Y38.5500\nG01 X37.6500 Y-38.5500\nG01 X37.6471 Y-38.6088\nG01 X37.6385 Y-38.6671\nG01 X37.6242 Y-38.7242\nG01 X37.6044 Y-38.7796\nG01 X37.5792 Y-38.8329\nG01 X37.5489 Y-38.8834\nG01 X37.5138 Y-38.9307\nG01 X37.4743 Y-38.9743\nG01 X37.4307 Y-39.0138\nG01 X37.3834 Y-39.0489\nG01 X37.3329 Y-39.0792\nG01 X37.2796 Y-39.1044\nG01 X37.2242 Y-39.1242\nG01 X37.1671 Y-39.1385\nG01 X37.1088 Y-39.1471\nG01 X37.0500 Y-39.1500\nG01 X0.8000 Y-39.1500\nG00 Z2.0000\nM05\nG00 Z2.0000\nG00 Z19.00\nG00 X0.0 Y0.0\n\n";
inline const std::string kFullFile = "(G-CODE GENERATED BY FLATCAM v8.994 - www.flatcam.org - Version Date: 2020/11/7)\n\n(Name: PCB ppal-Edge_Cuts.gm1_cutout_cnc)\n(Type: G-code from Geometry)\n(Units: MM)\n\n(Created on Monday, 11 November 2024 at 15:36)\n\n(This preprocessor is the default preprocessor used by FlatCAM.)\n(It is made to work with MACH3 compatible motion controllers.)\n\n(TOOL DIAMETER: 1.2 mm)\n(Feedrate_XY: 100.0 mm/min)\n(Feedrate_Z: 100.0 mm/min)\n(Feedrate rapids 1500.0 mm/min)\n\n(Z_Cut: -4.5 mm)\n(Z_Move: 2.0 mm)\n(Z Start: None mm)\n(Z End: 19.0 mm)\n(X,Y End: 0.0000, 0.0000 mm)\n(Steps per circle: 64)\n(Preprocessor Geometry: default)\n\n(X range:  -37.6500 ...   37.6500  mm)\n(Y range:  -39.1500 ...   39.1500  mm)\n\n(Spindle Speed: 1000.0 RPM)\nG21\nG90\nG94\n\nG01 F100.00\n\nM5\nG00 Z19.0000\nG00 X0.0000 Y0.0000\nT1\nM6    \n(MSG, Change to Tool Dia = 1.2000)\nM0\nG00 Z19.0000\n\nM03 S1000.0\nG01 F100.00\nG00 X-0.8000 Y-39.1500\nG01 F100.00\nG01 Z-4.5000\nG01 F100.00\nG01 X-37.0500 Y-39.1500\nG01 X-37.1088 Y-39.1471\nG01 X-37.1671 Y-39.1385\nG01 X-37.2242 Y-39.1242\nG01 X-37.2796 Y-39.1044\nG01 X-37.3329 Y-39.0792\nG01 X-37.3834 Y-39.0489\nG01 X-37.4307 Y-39.0138\nG01 X-37.4743 Y-38.9743\nG01 X-37.5138 Y-38.9307\nG01 X-37.5489 Y-38.8834\nG01 X-37.5792 Y-38.8329\nG01 X-37.6044 Y-38.7796\nG01 X-37.6242 Y-38.7242\nG01 X-37.6385 Y-38.6671\nG01 X-37.6471 Y-38.6088\nG01 X-37.6500 Y-38.5500\nG01 X-37.6500 Y38.5500\nG01 X-37.6471 Y38.6088\nG01 X-37.6385 Y38.6671\nG01 X-37.6242 Y38.7242\nG01 X-37.6044 Y38.7796\nG01 X-37.5792 Y38.8329\nG01 X-37.5489 Y38.8834\nG01 X-37.5138 Y38.9307\nG01 X-37.4743 Y38.9743\nG01 X-37.4307 Y39.0138\nG01 X-37.3834 Y39.0489\nG01 X-37.3329 Y39.0792\nG01 X-37.2796 Y39.1044\nG01 X-37.2242 Y39.1242\nG01 X-37.1671 Y39.1385\nG01 X-37.1088 Y39.1471\nG01 X-37.0500 Y39.1500\nG01 X-0.8000 Y39.1500\nG00 Z2.0000\nG00 X0.8000 Y39.1500\nG01 F100.00\nG01 Z-4.5000\nG01 F100.00\nG01 X37.0500 Y39.1500\nG01 X37.1088 Y39.1471\nG01 X37.1671 Y39.1385\nG01 X37.2242 Y39.1242\nG01 X37.2796 Y39.1044\nG01 X37.3329 Y39.0792\nG01 X37.3834 Y39.0489\nG01 X37.4307 Y39.0138\nG01 X37.4743 Y38.9743\nG01 X37.5138 Y38.9307\nG01 X37.5489 Y38.8834\nG01 X37.5792 Y38.8329\nG01 X37.6044 Y38.7796\nG01 X37.6242 Y38.7242\nG01 X37.6385 Y38.6671\nG01 X37.6471 Y38.6088\nG01 X37.6500 Y38.5500\nG01 X37.6500 Y-38.5500\nG01 X37.6471 Y-38.6088\nG01 X37.6385 Y-38.6671\nG01 X37.6242 Y-38.7242\nG01 X37.6044 Y-38.7796\nG01 X37.5792 Y-38.8329\nG01 X37.5489 Y-38.8834\nG01 X37.5138 Y-38.9307\nG01 X37.4743 Y-38.9743\nG01 X37.4307 Y-39.0138\nG01 X37.3834 Y-39.0489\nG01 X37.3329 Y-39.0792\nG01 X37.2796 Y-39.1044\nG01 X37.2242 Y-39.1242\nG01 X37.1671 Y-39.1385\nG01 X37.1088 Y-39.1471\nG01 X37.0500 Y-39.1500\nG01 X0.8000 Y-39.1500\nG00 Z2.0000\nM05\nG00 Z2.0000\nG00 Z19.00\nG00 X0.0 Y0.0\n\n\n";
}  // namespace golden_geometry
