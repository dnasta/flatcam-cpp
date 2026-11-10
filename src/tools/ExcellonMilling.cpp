#include "tools/ExcellonMilling.h"

#include <cmath>
#include <cstdio>
#include <string>

namespace flatcam {

namespace {

/// `float('%.*f' % (dec, v))`.
double decFormat(double v, int dec) {
    char buf[64];
    std::snprintf(buf, sizeof buf, "%.*f", dec, v);
    return std::stod(buf);
}

}  // namespace

MillingResult millDrills(const Excellon& exc, const std::vector<int>& tools, double tooldia) {
    MillingResult r;
    if (tools.empty()) {
        r.error = MillingResult::Error::NoTools;
        return r;
    }
    const auto& all = exc.drillTools();
    for (int t : tools) {
        if (tooldia > all.at(t).tooldia) {
            r.error = MillingResult::Error::ToolTooLarge;
            r.tool = t;
            return r;
        }
    }
    for (int t : tools) {
        const ExcellonTool& et = all.at(t);
        const double buffer = et.tooldia / 2 - tooldia / 2;
        for (const Point2D& d : et.drills)
            r.rings.push_back(Shape::point(d).buffer(buffer == 0 ? 0.0000001 : buffer).exterior());
    }
    return r;
}

MillingResult millSlots(const Excellon& exc, const std::vector<int>& tools, double tooldia, int decimals,
                        int stepsPerCircle) {
    MillingResult r;
    if (tools.empty()) {
        r.error = MillingResult::Error::NoTools;
        return r;
    }
    const auto& all = exc.drillTools();
    // Se suma 0,0001 por el error de redondeo al pasar de pulgadas a mm y al revés.
    const double table = decFormat(tooldia, decimals);
    for (int t : tools) {
        if (table > decFormat(all.at(t).tooldia, decimals) + 0.0001) {
            r.error = MillingResult::Error::ToolTooLarge;
            r.tool = t;
            return r;
        }
    }
    for (int t : tools) {
        const ExcellonTool& et = all.at(t);
        const double file = decFormat(et.tooldia, decimals);
        const double buffer = file / 2 - table / 2 + 0.0001;
        for (const auto& [start, stop] : et.slots)
            r.rings.push_back(
                Shape::lineString({start, stop}).buffer(buffer == 0 ? 0.0000001 : buffer, stepsPerCircle).exterior());
    }
    return r;
}

}  // namespace flatcam
