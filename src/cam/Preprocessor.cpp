#include "cam/Preprocessor.h"

#include <cmath>
#include <cstdio>

namespace flatcam {

namespace {

/// Equivale a `'%.*f' % (decimals, value)`.
std::string fmt(int decimals, double value) {
    char buf[64];
    std::snprintf(buf, sizeof(buf), "%.*f", decimals, value);
    return buf;
}

/// Equivale a `'{: >9s}'.format(text)`.
std::string right9(const std::string& text) {
    return text.size() >= 9 ? text : std::string(9 - text.size(), ' ') + text;
}

std::string lower(std::string s) {
    for (char& c : s) c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    return s;
}

std::string upper(std::string s) {
    for (char& c : s) c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
    return s;
}

/// `str(p['startz'])`, `str(p['spindlespeed'])`...: "None" si no hay valor.
std::string strOrNone(const std::optional<PyNumber>& v) { return v ? v->str() : "None"; }

/// `"%.*f, %.*f" % (decimals, a, decimals, b)`.
std::string pair(int decimals, const std::array<PyNumber, 2>& xy) {
    return fmt(decimals, xy[0].value()) + ", " + fmt(decimals, xy[1].value());
}

std::string positionCodeCommon(const PreprocessorParams& p) {
    return "X" + fmt(p.coordsDecimals, p.x) + " Y" + fmt(p.coordsDecimals, p.y);
}

std::string spindleCodeCommon(const PreprocessorParams& p) {
    const std::string sdir = (p.spindleDir == "CCW") ? "M04" : "M03";
    if (p.spindleSpeed && p.spindleSpeed->truthy()) return sdir + " S" + p.spindleSpeed->str();
    return sdir;
}

std::string endCodeCommon(const PreprocessorParams& p) {
    std::string gcode = "G00 Z" + fmt(p.frDecimals, p.zEnd.value()) + "\n";
    if (p.xyEnd) gcode += "G00 X" + (*p.xyEnd)[0].str() + " Y" + (*p.xyEnd)[1].str() + "\n";
    return gcode;
}

}  // namespace

// ------------------------------------------------------------------------------------- default

std::string DefaultPreprocessor::startCode(const PreprocessorParams& p) const {
    const std::string units = " " + lower(p.units);
    std::string gcode = "(This preprocessor is the default preprocessor used by FlatCAM.)\n";
    gcode += "(It is made to work with MACH3 compatible motion controllers.)\n\n";

    const std::string xmin = fmt(p.coordsDecimals, p.options.xmin.value());
    const std::string xmax = fmt(p.coordsDecimals, p.options.xmax.value());
    const std::string ymin = fmt(p.coordsDecimals, p.options.ymin.value());
    const std::string ymax = fmt(p.coordsDecimals, p.options.ymax.value());

    if (p.options.type == "Geometry") {
        gcode += "(TOOL DIAMETER: " + p.options.toolDia.str() + units + ")\n";
        gcode += "(Feedrate_XY: " + p.feedrate.str() + units + "/min)\n";
        gcode += "(Feedrate_Z: " + p.zFeedrate.str() + units + "/min)\n";
        gcode += "(Feedrate rapids " + p.feedrateRapid.str() + units + "/min)\n\n";
        gcode += "(Z_Cut: " + p.zCut.str() + units + ")\n";
        if (p.multidepth) {
            const long passes = static_cast<long>(std::ceil(std::fabs(p.zCut.value()) / p.zDepthPerCut.value()));
            gcode += "(DepthPerCut: " + p.zDepthPerCut.str() + units + " <=>" + std::to_string(passes) + " passes)\n";
        }
        gcode += "(Z_Move: " + p.zMove.str() + units + ")\n";
    } else if (p.options.type == "Excellon" && p.useUi) {
        gcode += "\n(TOOLS DIAMETER: )\n";
        for (const auto& t : p.excTools) gcode += "(Tool: " + std::to_string(t.tool) + " -> Dia: " + t.tooldia.str() + ")\n";
        gcode += "\n(FEEDRATE Z: )\n";
        for (const auto& t : p.excTools) gcode += "(Tool: " + std::to_string(t.tool) + " -> Feedrate: " + t.feedrateZ.str() + ")\n";
        gcode += "\n(FEEDRATE RAPIDS: )\n";
        for (const auto& t : p.excTools) gcode += "(Tool: " + std::to_string(t.tool) + " -> Feedrate Rapids: " + t.feedrateRapid.str() + ")\n";
        gcode += "\n(Z_CUT: )\n";
        for (const auto& t : p.excTools) gcode += "(Tool: " + std::to_string(t.tool) + " -> Z_Cut: " + t.cutZ.str() + ")\n";
        gcode += "\n(Tools Offset: )\n";
        for (const auto& t : p.excCncTools) gcode += "(Tool: " + std::to_string(t.tool) + " -> Offset Z: " + t.offsetZ.str() + ")\n";
        if (p.multidepth) {
            gcode += "\n(DEPTH_PER_CUT: )\n";
            for (const auto& t : p.excTools) gcode += "(Tool: " + std::to_string(t.tool) + " -> DeptPerCut: " + t.depthPerPass.str() + ")\n";
        }
        gcode += "\n(Z_MOVE: )\n";
        for (const auto& t : p.excTools) gcode += "(Tool: " + std::to_string(t.tool) + " -> Z_Move: " + t.travelZ.str() + ")\n";
        gcode += "\n";
    }

    if (p.toolchange) {
        gcode += "(Z Toolchange: " + p.zToolchange.str() + units + ")\n";
        if (p.xyToolchange) gcode += "(X,Y Toolchange: " + pair(p.decimals, *p.xyToolchange) + units + ")\n";
        else gcode += "(X,Y Toolchange: None" + units + ")\n";
    }
    gcode += "(Z Start: " + strOrNone(p.startZ) + units + ")\n";
    gcode += "(Z End: " + p.zEnd.str() + units + ")\n";
    if (p.xyEnd) gcode += "(X,Y End: " + pair(p.decimals, *p.xyEnd) + units + ")\n";
    else gcode += "(X,Y End: None" + units + ")\n";
    gcode += "(Steps per circle: " + std::to_string(p.stepsPerCircle) + ")\n";
    if (p.options.type == "Excellon" || p.options.type == "Excellon Geometry") {
        gcode += "(Preprocessor Excellon: " + p.ppExcellonName + ")\n\n";
    } else {
        gcode += "(Preprocessor Geometry: " + p.ppGeometryName + ")\n\n";
    }
    gcode += "(X range: " + right9(xmin) + " ... " + right9(xmax) + " " + units + ")\n";
    gcode += "(Y range: " + right9(ymin) + " ... " + right9(ymax) + " " + units + ")\n\n";
    gcode += "(Spindle Speed: " + strOrNone(p.spindleSpeed) + " RPM)\n";
    gcode += (upper(p.units) == "IN") ? "G20\n" : "G21\n";
    gcode += "G90\n";
    gcode += "G94";
    return gcode;
}

std::string DefaultPreprocessor::startzCode(const PreprocessorParams& p) const {
    if (p.startZ) return "G00 Z" + fmt(p.coordsDecimals, p.startZ->value());
    return "";
}

std::string DefaultPreprocessor::liftCode(const PreprocessorParams& p) const {
    return "G00 Z" + fmt(p.coordsDecimals, p.zMove.value());
}

std::string DefaultPreprocessor::downCode(const PreprocessorParams& p) const {
    return "G01 Z" + fmt(p.coordsDecimals, p.zCut.value());
}

std::string DefaultPreprocessor::toolchangeCode(const PreprocessorParams& p) const {
    double zToolchange = p.zToolchange.value();
    double xToolchange = 0.0, yToolchange = 0.0;
    if (p.xyToolchange) {
        xToolchange = (*p.xyToolchange)[0].value();
        yToolchange = (*p.xyToolchange)[1].value();
    }
    int noDrills = 1;
    if (p.tool == 1 && p.startZ) zToolchange = p.startZ->value();
    const std::string toolC = fmt(p.decimals, p.toolC.value());
    const std::string z = fmt(p.coordsDecimals, zToolchange);
    const std::string x = fmt(p.coordsDecimals, xToolchange);
    const std::string y = fmt(p.coordsDecimals, yToolchange);
    const std::string tool = std::to_string(p.tool);

    std::string gcode;
    if (p.options.type == "Excellon") {
        for (const ToolInUse& i : p.options.toolsInUse) {
            if (i.tool == p.tool) noDrills = i.drills;
        }
        const std::string msg = "(MSG, Change to Tool Dia = " + toolC + " ||| Total drills for tool T" + tool + " = " +
                                std::to_string(noDrills) + ")\n";
        if (p.xyToolchange) {
            gcode = "\nM5\nG00 Z" + z + "\nT" + tool + "\nG00 X" + x + " Y" + y + "                \nM6\n" + msg +
                    "M0\nG00 Z" + z + "\n";
        } else {
            gcode = "\nM5       \nG00 Z" + z + "\nT" + tool + "\nM6\n" + msg + "M0\nG00 Z" + z + "\n";
        }
    } else {
        const std::string msg = "(MSG, Change to Tool Dia = " + toolC + ")\n";
        if (p.xyToolchange) {
            gcode = "\nM5\nG00 Z" + z + "\nG00 X" + x + " Y" + y + "\nT" + tool + "\nM6    \n" + msg + "M0\nG00 Z" + z + "\n";
        } else {
            gcode = "\nM5\nG00 Z" + z + "\nT" + tool + "\nM6    \n" + msg + "M0\nG00 Z" + z + "\n";
        }
    }
    if (p.fPlunge) gcode += "\nG00 Z" + fmt(p.coordsDecimals, p.zMove.value());
    return gcode;
}

std::string DefaultPreprocessor::upToZeroCode(const PreprocessorParams&) const { return "G01 Z0"; }

std::string DefaultPreprocessor::positionCode(const PreprocessorParams& p) const { return positionCodeCommon(p); }

std::string DefaultPreprocessor::rapidCode(const PreprocessorParams& p) const { return "G00 " + positionCode(p); }

std::string DefaultPreprocessor::linearCode(const PreprocessorParams& p) const { return "G01 " + positionCode(p); }

std::string DefaultPreprocessor::endCode(const PreprocessorParams& p) const { return endCodeCommon(p); }

std::string DefaultPreprocessor::feedrateCode(const PreprocessorParams& p) const {
    return "G01 F" + fmt(p.frDecimals, p.feedrate.value());
}

std::string DefaultPreprocessor::zFeedrateCode(const PreprocessorParams& p) const {
    return "G01 F" + fmt(p.frDecimals, p.zFeedrate.value());
}

std::string DefaultPreprocessor::spindleCode(const PreprocessorParams& p) const { return spindleCodeCommon(p); }

std::string DefaultPreprocessor::dwellCode(const PreprocessorParams& p) const {
    if (p.dwellTime.truthy()) return "G4 P" + p.dwellTime.str();
    return "";
}

std::string DefaultPreprocessor::spindleStopCode(const PreprocessorParams&) const { return "M05"; }

// ---------------------------------------------------------------------------------- GRBL_laser

std::string GrblLaserPreprocessor::startCode(const PreprocessorParams& p) const {
    const std::string units = " " + lower(p.units);
    std::string gcode = "(This preprocessor is used with a motion controller loaded with GRBL firmware. )\n";
    gcode += "(It is for the case when it is used together with a LASER connected on the SPINDLE connector.)\n\n";

    const std::string xmin = fmt(p.coordsDecimals, p.options.xmin.value());
    const std::string xmax = fmt(p.coordsDecimals, p.options.xmax.value());
    const std::string ymin = fmt(p.coordsDecimals, p.options.ymin.value());
    const std::string ymax = fmt(p.coordsDecimals, p.options.ymax.value());

    gcode += "(Feedrate: " + p.feedrate.str() + units + "/min)\n";
    gcode += "(Feedrate rapids: " + p.feedrateRapid.str() + units + "/min)\n\n";
    gcode += "(Z Focus: " + p.zMove.str() + units + ")\n";
    gcode += "(Steps per circle: " + std::to_string(p.stepsPerCircle) + ")\n";
    if (p.options.type == "Excellon" || p.options.type == "Excellon Geometry") {
        gcode += "(Preprocessor Excellon: " + p.ppExcellonName + ")\n";
    } else {
        gcode += "(Preprocessor Geometry: " + p.ppGeometryName + ")\n\n";
    }
    gcode += "(X range: " + right9(xmin) + " ... " + right9(xmax) + " " + units + ")\n";
    gcode += "(Y range: " + right9(ymin) + " ... " + right9(ymax) + " " + units + ")\n\n";
    gcode += "(Laser Power - Spindle Speed: " + strOrNone(p.spindleSpeed) + ")\n\n";
    if (flavor_ == GcodeFlavor::FlatCAM8_994) gcode += "G00 Z0\n\n";  // la 8.994 lo emite en la cabecera del láser
    gcode += (upper(p.units) == "IN") ? "G20\n" : "G21\n";
    gcode += "G90\n";
    gcode += "G17\n";
    gcode += "G94";
    return gcode;
}

std::string GrblLaserPreprocessor::startzCode(const PreprocessorParams&) const { return ""; }
std::string GrblLaserPreprocessor::liftCode(const PreprocessorParams&) const { return "M5"; }
std::string GrblLaserPreprocessor::downCode(const PreprocessorParams& p) const { return spindleCodeCommon(p); }
std::string GrblLaserPreprocessor::toolchangeCode(const PreprocessorParams&) const { return ""; }
std::string GrblLaserPreprocessor::upToZeroCode(const PreprocessorParams&) const { return "M5"; }
std::string GrblLaserPreprocessor::positionCode(const PreprocessorParams& p) const { return positionCodeCommon(p); }
std::string GrblLaserPreprocessor::rapidCode(const PreprocessorParams& p) const { return "G00 " + positionCode(p); }

std::string GrblLaserPreprocessor::linearCode(const PreprocessorParams& p) const {
    return "G01 " + positionCode(p) + " F" + fmt(p.frDecimals, p.feedrate.value());
}

std::string GrblLaserPreprocessor::endCode(const PreprocessorParams& p) const {
    if (flavor_ == GcodeFlavor::FlatCAM8_994) {
        // La 8.994 no sube Z al terminar con el láser: solo vuelve a las coordenadas finales.
        std::string gcode;
        if (p.xyEnd) gcode += "G00 X" + (*p.xyEnd)[0].str() + " Y" + (*p.xyEnd)[1].str() + "\n";
        return gcode;
    }
    return endCodeCommon(p);
}

std::string GrblLaserPreprocessor::feedrateCode(const PreprocessorParams& p) const {
    return "G01 F" + fmt(p.frDecimals, p.feedrate.value());
}

std::string GrblLaserPreprocessor::zFeedrateCode(const PreprocessorParams& p) const {
    return "G01 F" + fmt(p.frDecimals, p.zFeedrate.value());
}

std::string GrblLaserPreprocessor::spindleCode(const PreprocessorParams& p) const { return spindleCodeCommon(p); }
std::string GrblLaserPreprocessor::dwellCode(const PreprocessorParams&) const { return ""; }
std::string GrblLaserPreprocessor::spindleStopCode(const PreprocessorParams&) const { return "M5"; }

std::unique_ptr<Preprocessor> makePreprocessor(const std::string& name, GcodeFlavor flavor) {
    if (name == "default") return std::make_unique<DefaultPreprocessor>();
    if (name == "GRBL_laser") return std::make_unique<GrblLaserPreprocessor>(flavor);
    return nullptr;
}

}  // namespace flatcam
