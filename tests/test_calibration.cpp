// Calibración contra el original (tests/tools/oracle_calibration.py).
#include <gtest/gtest.h>

#include <map>

#include "cam/PyNumber.h"
#include "golden_calibration.h"
#include "tools/Calibration.h"

using namespace flatcam;

namespace {

/// Lo que hace la herramienta con sus campos (los "tgt" muestran el valor redondeado; los puntos, el que se guardó).
struct Sim {
    int decimals{4};
    CalibrationPoints pts{};
    std::vector<std::string> tgt;
    double sx{1.0}, sy{1.0}, ax{0.0}, ay{0.0};
    void show() {
        tgt.clear();
        for (const Point2D& p : pts) {
            tgt.push_back(pyFloatRepr(calibrationRound(p.x, decimals)));
            tgt.push_back(pyFloatRepr(calibrationRound(p.y, decimals)));
        }
    }
    void click(const std::vector<Point2D>& in) {
        for (std::size_t i = 0; i < 4; ++i)
            pts[i] = {calibrationRound(in[i].x, decimals), calibrationRound(in[i].y, decimals)};
        show();
    }
    void readTable() {  // target_values_in_table (getter): los puntos pasan a ser los del campo.
        for (std::size_t i = 0; i < 4; ++i) pts[i] = {std::stod(tgt[2 * i]), std::stod(tgt[2 * i + 1])};
    }
    void apply(const CalibrationFactors& f) {
        if (f.scaleX) sx = *f.scaleX;
        if (f.scaleY) sy = *f.scaleY;
        if (f.skewX) ax = *f.skewX;
        if (f.skewY) ay = *f.skewY;
    }
    std::string gcode(CalibrationGcodeParams p) {
        p.decimals = decimals;
        p.version = "2024.4";
        p.versionDate = "2024/04/15";
        p.timeStr = golden_calibration::kTime;
        p.name = "Verification GCode for FlatCAM Calibration Tool";
        return calibrationGcode(pts, p);
    }
};

void check(const Sim& s, const golden_calibration::Case& c) {
    SCOPED_TRACE(c.name);
    EXPECT_EQ(s.tgt, c.tgt);
    std::vector<double> pts;
    for (const Point2D& p : s.pts) {
        pts.push_back(p.x);
        pts.push_back(p.y);
    }
    EXPECT_EQ(pts, c.points);
    EXPECT_EQ((std::vector<double>{s.sx, s.sy, s.ax, s.ay}), c.fields);
}

const std::vector<Point2D> P = {{-30.123456, -25.000049}, {31.5, -25.2}, {-29.8, 26.75}, {31.2, 27.0}};

}  // namespace

TEST(Calibration, MatchesTheOriginal) {
    std::map<std::string, const golden_calibration::Case*> g;
    for (const auto& c : golden_calibration::kCases) g[c.name] = &c;
    {
        Sim s;
        s.click(P);
        check(s, *g["gcode_tl"]);
        EXPECT_EQ(s.gcode({}), g["gcode_tl"]->gcode);
    }
    {
        Sim s;
        s.click(P);
        CalibrationGcodeParams p;
        p.units = "IN";
        p.zeroZ = true;
        p.toolchangeXy = {1.5, 2.0};
        p.secondPoint = "br";
        p.travelZ = 1.23456;
        p.toolchangeZ = 20;
        p.verificationZ = -0.05;
        check(s, *g["gcode_br_zeroz"]);
        EXPECT_EQ(s.gcode(p), g["gcode_br_zeroz"]->gcode);
    }
    {
        Sim s;
        s.click(P);
        s.apply(calibrationFactors(s.pts, 0.1, 0.2, 0.3, -0.1, 4));
        check(s, *g["factors_all"]);
    }
    {
        Sim s;
        s.click(P);
        s.apply(calibrationFactors(s.pts, {}, {}, {}, {}, 4));
        check(s, *g["factors_none"]);
    }
    {
        Sim s;
        s.click(P);
        s.sx = 1.0123;
        s.sy = 0.9876;
        s.readTable();
        s.pts = calibrationScale(s.pts, s.sx, s.sy);
        s.show();
        check(s, *g["scaled"]);
        s.ax = 0.5;
        s.ay = -0.25;
        s.readTable();
        s.pts = calibrationSkew(s.pts, s.ax, s.ay);
        s.show();
        check(s, *g["skewed"]);
        EXPECT_EQ(s.gcode({}), g["skewed"]->gcode);
        s.apply(calibrationFactors(s.pts, {}, {}, {}, {}, 4));
        check(s, *g["factors_after_skew"]);
    }
    {
        Sim s;
        s.click({P[0], {P[0].x, -25.2}, P[2], P[3]});
        const CalibrationFactors f = calibrationFactors(s.pts, {}, {}, 0.3, {}, 4);
        s.apply(f);
        EXPECT_EQ(f.failed, g["factors_zero"]->failed);
        check(s, *g["factors_zero"]);
    }
    {
        Sim s;
        s.decimals = 2;
        s.click(P);
        s.apply(calibrationFactors(s.pts, {}, 0.05 * 2, {}, {}, 2));
        check(s, *g["factors_2dec"]);
        EXPECT_EQ(s.gcode({}), g["factors_2dec"]->gcode);
    }
}
