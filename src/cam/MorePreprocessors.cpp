// Los postprocesadores de FlatCAM 2024.4 que no son `default` ni `GRBL_laser` (`preprocessors/*.py`). Cada uno reproduce el
// texto del original; las plantillas multilínea salen de src/cam/generated/PreprocessorTemplates.h (tools/gen_pp_templates.py).
// Verificados contra el original con tests/tools/oracle_preprocessors.py.
#include <memory>

#include "cam/Preprocessor.h"
#include "cam/generated/PreprocessorTemplates.h"
#include "cam/internal/PpCommon.h"

namespace flatcam {
namespace pp_internal {

std::string defaultLikeStart(const PreprocessorParams& p, const StartStyle& st) {
    const std::string units = " " + lower(p.units);
    // Una línea de comentario en el estilo de la cabecera: "(texto)" o ";texto".
    auto c = [&](const std::string& text) { return st.semicolon ? ";" + text + "\n" : "(" + text + ")\n"; };
    std::string gcode = st.intro + st.preamble;

    const std::string xmin = fmt(p.coordsDecimals, p.options.xmin.value());
    const std::string xmax = fmt(p.coordsDecimals, p.options.xmax.value());
    const std::string ymin = fmt(p.coordsDecimals, p.options.ymin.value());
    const std::string ymax = fmt(p.coordsDecimals, p.options.ymax.value());

    if (p.options.type == "Geometry") {
        gcode += c("TOOL DIAMETER: " + p.options.toolDia.str() + units);
        gcode += c("Feedrate_XY: " + p.feedrate.str() + units + "/min");
        gcode += c("Feedrate_Z: " + p.zFeedrate.str() + units + "/min");
        gcode += c("Feedrate rapids " + p.feedrateRapid.str() + units + "/min") + "\n";
        gcode += c("Z_Cut: " + p.zCut.str() + units);
        if (p.multidepth) {
            const long passes = static_cast<long>(std::ceil(std::fabs(p.zCut.value()) / p.zDepthPerCut.value()));
            gcode += c("DepthPerCut: " + p.zDepthPerCut.str() + units + " <=>" + std::to_string(passes) + " passes");
        }
        gcode += c("Z_Move: " + p.zMove.str() + units);
    } else if (p.options.type == "Excellon" && p.useUi) {
        auto block = [&](const std::string& title, const std::vector<ExcToolInfo>& tools, const char* label,
                         PyNumber ExcToolInfo::*field) {
            gcode += "\n" + c(title);
            for (const auto& t : tools) gcode += c("Tool: " + std::to_string(t.tool) + " -> " + label + (t.*field).str());
        };
        block("TOOLS DIAMETER: ", p.excTools, "Dia: ", &ExcToolInfo::tooldia);
        block("FEEDRATE Z: ", p.excTools, "Feedrate: ", &ExcToolInfo::feedrateZ);
        block("FEEDRATE RAPIDS: ", p.excTools, "Feedrate Rapids: ", &ExcToolInfo::feedrateRapid);
        block("Z_CUT: ", p.excTools, "Z_Cut: ", &ExcToolInfo::cutZ);
        block("Tools Offset: ", p.excCncTools, "Offset Z: ", &ExcToolInfo::offsetZ);
        if (p.multidepth) block("DEPTH_PER_CUT: ", p.excTools, "DeptPerCut: ", &ExcToolInfo::depthPerPass);
        block("Z_MOVE: ", p.excTools, "Z_Move: ", &ExcToolInfo::travelZ);
        gcode += "\n";
    }

    if (p.toolchange) {
        gcode += c("Z Toolchange: " + p.zToolchange.str() + units);
        gcode += c("X,Y Toolchange: " + (p.xyToolchange ? pair(p.decimals, *p.xyToolchange) : std::string("None")) + units);
    }
    gcode += c("Z Start: " + strOrNone(p.startZ) + units);
    gcode += c("Z End: " + p.zEnd.str() + units);
    // Berta_CNC pregunta por el X,Y del cambio de herramienta pero imprime el final (si falta el final, el original falla;
    // el port escribe "None").
    const bool showEnd = st.endXYGatedByToolchange ? p.xyToolchange.has_value() : p.xyEnd.has_value();
    gcode += c("X,Y End: " + (showEnd && p.xyEnd ? pair(p.decimals, *p.xyEnd) : std::string("None")) + units);
    const std::string steps = "Steps per circle: " + std::to_string(p.stepsPerCircle);
    switch (st.steps) {
        case StepsLines::One: gcode += c(steps); break;
        case StepsLines::Two: gcode += c(steps) + c(steps); break;
        case StepsLines::ParenThenComment: gcode += "(" + steps + ")\n;" + steps + "\n"; break;
    }
    if (p.options.type == "Excellon" || p.options.type == "Excellon Geometry") {
        gcode += c("Preprocessor Excellon: " + p.ppExcellonName) + "\n";
    } else {
        gcode += c("Preprocessor Geometry: " + p.ppGeometryName) + "\n";
    }
    gcode += c("X range: " + right9(xmin) + " ... " + right9(xmax) + " " + units);
    gcode += c("Y range: " + right9(ymin) + " ... " + right9(ymax) + " " + units) + "\n";
    // En el estilo con punto y coma el original deja el paréntesis de cierre.
    gcode += std::string(st.semicolon ? ";" : "(") + "Spindle Speed: " + strOrNone(p.spindleSpeed) + " RPM)\n";
    gcode += st.tail;
    return gcode;
}

std::string defaultLikeToolchange(const PreprocessorParams& p, const char* const* templates, const std::string& plunge,
                                  const std::map<std::string, std::string>& extra) {
    double zToolchange = p.zToolchange.value();
    double xToolchange = 0.0, yToolchange = 0.0;
    if (p.xyToolchange) {
        xToolchange = (*p.xyToolchange)[0].value();
        yToolchange = (*p.xyToolchange)[1].value();
    }
    int noDrills = 1;
    if (p.tool == 1 && p.startZ) zToolchange = p.startZ->value();
    std::map<std::string, std::string> v = {{"z_toolchange", fmt(p.coordsDecimals, zToolchange)},
                                            {"x_toolchange", fmt(p.coordsDecimals, xToolchange)},
                                            {"y_toolchange", fmt(p.coordsDecimals, yToolchange)},
                                            {"tool", std::to_string(p.tool)},
                                            {"toolC", fmt(p.decimals, p.toolC.value())}};
    v.insert(extra.begin(), extra.end());
    std::string gcode;
    if (p.options.type == "Excellon") {
        for (const ToolInUse& i : p.options.toolsInUse)
            if (i.tool == p.tool) noDrills = i.drills;
        v["t_drills"] = std::to_string(noDrills);
        gcode = pyFormat(templates[p.xyToolchange ? 0 : 1], v);
    } else {
        gcode = pyFormat(templates[p.xyToolchange ? 2 : 3], v);
    }
    if (p.fPlunge && !plunge.empty()) gcode += "\n" + plunge + fmt(p.coordsDecimals, p.zMove.value());
    return gcode;
}

namespace {

using namespace pp_templates;

std::string unitsLine(const PreprocessorParams& p) { return upper(p.units) == "IN" ? "G20\n" : "G21\n"; }

std::string excellonDrills(const PreprocessorParams& p) {
    int noDrills = 1;
    for (const ToolInUse& i : p.options.toolsInUse)
        if (i.tool == p.tool) noDrills = i.drills;
    return std::to_string(noDrills);
}

StartStyle style(std::string intro, StepsLines steps, std::string tail) {
    StartStyle st;
    st.intro = std::move(intro);
    st.steps = steps;
    st.tail = std::move(tail);
    return st;
}

/// Default_no_M6: como default sin M6 en el cambio de herramienta.
class DefaultNoM6 : public DefaultPreprocessor {
public:
    std::string name() const override { return "Default_no_M6"; }
    std::string toolchangeCode(const PreprocessorParams& p) const override {
        return defaultLikeToolchange(p, kDefault_no_M6_toolchange_code);
    }
};

/// grbl_11: GRBL; avance en cada G01, G17 y la línea "Steps per circle" repetida.
class Grbl11 : public DefaultPreprocessor {
public:
    explicit Grbl11(const char* const* templates = kgrbl_11_toolchange_code, std::string name = "grbl_11")
        : templates_(templates), name_(std::move(name)) {}
    std::string name() const override { return name_; }
    std::string startCode(const PreprocessorParams& p) const override {
        return defaultLikeStart(p, style("(This preprocessor is used with a motion controller loaded with GRBL firmware.)\n"
                                         "(It is configured to be compatible with almost any version of GRBL firmware.)\n\n",
                                         StepsLines::Two, unitsLine(p) + "G90\nG17\nG94\n"));
    }
    std::string toolchangeCode(const PreprocessorParams& p) const override {
        return defaultLikeToolchange(p, templates_);
    }
    std::string linearCode(const PreprocessorParams& p) const override {
        return "G01 " + positionCode(p) + " F" + fmt(p.frDecimals, p.feedrate.value());
    }

private:
    const char* const* templates_;
    std::string name_;
};

/// Toolchange_Manual: sin presentación, G17, y el cambio de herramienta con las pausas para ajustar la fresa.
class ToolchangeManual : public DefaultPreprocessor {
public:
    std::string name() const override { return "Toolchange_Manual"; }
    std::string startCode(const PreprocessorParams& p) const override {
        return defaultLikeStart(p, style("", StepsLines::One, unitsLine(p) + "G90\nG17\nG94\n"));
    }
    std::string toolchangeCode(const PreprocessorParams& p) const override {
        return defaultLikeToolchange(p, kToolchange_Manual_toolchange_code);
    }
};

/// Toolchange_Custom: solo `M6` en el cambio de herramienta (y solo si hay X,Y de cambio); sin Z inicial.
class ToolchangeCustom : public DefaultPreprocessor {
public:
    std::string name() const override { return "Toolchange_Custom"; }
    std::string startCode(const PreprocessorParams& p) const override {
        return defaultLikeStart(p, style("", StepsLines::One, unitsLine(p) + "G90\nG17\nG94\n"));
    }
    std::string startzCode(const PreprocessorParams&) const override { return ""; }
    std::string toolchangeCode(const PreprocessorParams& p) const override {
        std::string gcode = p.xyToolchange ? "\nM6\n" : "";
        if (p.fPlunge) gcode += "\nG00 Z" + fmt(p.coordsDecimals, p.zMove.value());
        return gcode;
    }
};

/// Toolchange_Probe_MACH3: MACH3 con medición de la altura de la herramienta con sonda en cada cambio.
class ToolchangeProbeMach3 : public DefaultPreprocessor {
public:
    std::string name() const override { return "Toolchange_Probe_MACH3"; }
    std::string startCode(const PreprocessorParams& p) const override {
        return defaultLikeStart(p, style("(This preprocessor is used with MACH3 with probing height.)\n\n",
                                         StepsLines::One, unitsLine(p) + "G90\nG17\nG94\n"));
    }
    std::string startzCode(const PreprocessorParams&) const override {
        return "(MSG, WARNING: Make sure you do zero on all axis. For Z axis, since it will be probed, make a rough "
               "estimate and do a zero.)\nM0";
    }
    std::string toolchangeCode(const PreprocessorParams& p) const override {
        const int cd = p.coordsDecimals, fd = p.frDecimals;
        return defaultLikeToolchange(p, kToolchange_Probe_MACH3_toolchange_code, "",
                                     {{"z_move", fmt(cd, p.zMove.value())},
                                      {"z_in_between", fmt(cd, p.zMove.value() / 2)},
                                      {"feedrate_probe", fmt(fd, p.feedrateProbe.value())},
                                      {"feedrate_probe_slow", fmt(fd, p.feedrateProbe.value() / 2)},
                                      {"z_pdepth", fmt(cd, p.zPdepth.value())}});
    }
};

/// Berta_CNC: router BERTA.
class BertaCnc : public DefaultPreprocessor {
public:
    std::string name() const override { return "Berta_CNC"; }
    std::string startCode(const PreprocessorParams& p) const override {
        const std::string units = " " + lower(p.units);
        StartStyle st = style("(This preprocessor is used with a BERTA CNC router.)\n\n", StepsLines::Two,
                              unitsLine(p) + "\nG90 G17 G91.1\nG64 P0.03\nM110\nG54\nG0\n(Berta)\nG94");
        if (p.options.type == "Geometry") st.preamble = "(TOOL DIAMETER: " + p.options.toolDia.str() + units + ")\n";
        st.preamble += "(Feedrate: " + p.feedrate.str() + units + "/min)\n";
        st.endXYGatedByToolchange = true;
        return defaultLikeStart(p, st);
    }
    std::string toolchangeCode(const PreprocessorParams& p) const override {
        return defaultLikeToolchange(p, kBerta_CNC_toolchange_code);
    }
    std::string endCode(const PreprocessorParams& p) const override {
        return endCodeCommon(p) + "(Berta)\nM111\nM30\n(Berta)\n";
    }
};

/// ISEL_CNC: router ISEL (G71, sin Z ni X,Y de cambio de herramienta).
class IselCnc : public DefaultPreprocessor {
public:
    std::string name() const override { return "ISEL_CNC"; }
    std::string startCode(const PreprocessorParams& p) const override {
        return defaultLikeStart(p, style("(This preprocessor is used with a ISEL CNC router.)\n\n", StepsLines::Two,
                                         "G71\nG90\nG94\n"));
    }
    std::string toolchangeCode(const PreprocessorParams& p) const override {
        std::map<std::string, std::string> v = {{"tool", std::to_string(p.tool)},
                                                {"toolC", fmt(p.decimals, p.toolC.value())}};
        std::string gcode;
        if (p.options.type == "Excellon") {
            v["t_drills"] = excellonDrills(p);
            gcode = pyFormat(kISEL_CNC_toolchange_code[0], v);
        } else {
            gcode = pyFormat(kISEL_CNC_toolchange_code[1], v);
        }
        if (p.fPlunge) gcode += "\nG00 Z" + fmt(p.coordsDecimals, p.zMove.value());
        return gcode;
    }
};

/// ISEL_ICP_CNC: lenguaje ISEL ICP (no es G-code): unidades en micras y velocidades en micras por segundo.
class IselIcpCnc : public Preprocessor {
public:
    std::string name() const override { return "ISEL_ICP_CNC"; }
    std::string startCode(const PreprocessorParams& p) const override {
        StartStyle st = style("; This preprocessor is used with a ISEL ICP CNC router.\n\nIMF_PBL flatcam\n\n",
                              StepsLines::ParenThenComment, "");
        st.semicolon = true;
        return defaultLikeStart(p, st);
    }
    std::string startzCode(const PreprocessorParams& p) const override {
        return p.startZ ? "FASTABS Z" + pyInt(p.startZ->value() * 1000) : "";
    }
    std::string liftCode(const PreprocessorParams& p) const override {
        return "FASTABS Z" + pyInt(p.zMove.value() * 1000);
    }
    std::string downCode(const PreprocessorParams& p) const override { return "MOVEABS Z" + pyInt(p.zCut.value() * 1000); }
    std::string toolchangeCode(const PreprocessorParams& p) const override {
        const std::map<std::string, std::string> v = {{"tool", std::to_string(p.tool)},
                                                      {"toolC", fmt(p.decimals, p.toolC.value())}};
        std::string gcode = pyFormat(kISEL_ICP_CNC_toolchange_code[p.options.type == "Excellon" ? 0 : 1], v);
        if (p.fPlunge) gcode += "\nFASTABS Z" + pyInt(p.zMove.value() * 1000);
        return gcode;
    }
    std::string upToZeroCode(const PreprocessorParams&) const override { return "MOVEABS Z0"; }
    std::string positionCode(const PreprocessorParams& p) const override {
        return "X" + pyInt(p.x * 1000) + " Y" + pyInt(p.y * 1000);
    }
    std::string rapidCode(const PreprocessorParams& p) const override { return "FASTABS " + positionCode(p); }
    std::string linearCode(const PreprocessorParams& p) const override { return "MOVEABS " + positionCode(p); }
    std::string endCode(const PreprocessorParams&) const override {
        return "WPCLEAR\nFASTABS Z0\nFASTABS X0 Y0\nPROGEND";
    }
    std::string feedrateCode(const PreprocessorParams& p) const override {
        return "VEL " + pyInt(p.feedrate.value() / 60 * 1000);
    }
    std::string zFeedrateCode(const PreprocessorParams& p) const override {
        return "VEL " + pyInt(p.zFeedrate.value() / 60 * 1000);
    }
    std::string spindleCode(const PreprocessorParams& p) const override {
        const std::string sdir = p.spindleDir == "CCW" ? "SPINDLE CCW" : "SPINDLE CW";
        if (p.spindleSpeed && p.spindleSpeed->truthy()) return sdir + " RPM" + pyInt(p.spindleSpeed->value());
        return sdir;
    }
    std::string dwellCode(const PreprocessorParams& p) const override {
        return p.dwellTime.truthy() ? "WAIT " + pyInt(p.dwellTime.value() * 1000) : "";
    }
    std::string spindleStopCode(const PreprocessorParams&) const override { return "SPINDLE OFF"; }
};

/// line_xyz: cada movimiento lleva X, Y y Z.
class LineXyz : public DefaultPreprocessor {
public:
    std::string name() const override { return "line_xyz"; }
    std::string startCode(const PreprocessorParams& p) const override {
        // El original lee `p['end_xy']`, que CNCjob no define (falla con KeyError); el port usa `xy_end`.
        return defaultLikeStart(p, style("", StepsLines::Two, unitsLine(p) + "G90\nG94\n"));
    }
    std::string startzCode(const PreprocessorParams& p) const override {
        return p.startZ ? "G00 " + xy(p) + " Z" + fmt(p.coordsDecimals, p.startZ->value()) : "";
    }
    std::string liftCode(const PreprocessorParams& p) const override {
        return "G00 " + xy(p) + " Z" + fmt(p.coordsDecimals, p.zMove.value());
    }
    std::string downCode(const PreprocessorParams& p) const override {
        return "G01 " + xy(p) + " Z" + fmt(p.coordsDecimals, p.zCut.value());
    }
    std::string toolchangeCode(const PreprocessorParams& p) const override {
        double x = p.x, y = p.y;
        if (p.xyToolchange) {
            x = (*p.xyToolchange)[0].value();
            y = (*p.xyToolchange)[1].value();
        } else if (p.options.type == "Excellon") {
            x = p.oldX;
            y = p.oldY;
        }
        double z = p.zToolchange.value();
        if (p.tool == 1 && p.startZ) z = p.startZ->value();
        const int cd = p.coordsDecimals;
        std::map<std::string, std::string> v = {{"x_toolchange", fmt(cd, x)}, {"y_toolchange", fmt(cd, y)},
                                                {"z_toolchange", fmt(cd, z)}, {"z_move", fmt(cd, p.zMove.value())},
                                                {"tool", std::to_string(p.tool)},
                                                {"toolC", fmt(p.decimals, p.toolC.value())}};
        const bool exc = p.options.type == "Excellon";
        if (exc) v["t_drills"] = excellonDrills(p);
        // Las plantillas del original ponen x_toolchange también en la Y (se reproduce).
        std::string gcode = pyFormat(kline_xyz_toolchange_code[exc ? 0 : 2], v);
        if (p.fPlunge) gcode += pyFormat(kline_xyz_toolchange_code[exc ? 1 : 3], v);
        return gcode;
    }
    std::string upToZeroCode(const PreprocessorParams& p) const override { return "G01 " + xy(p) + " Z0"; }
    std::string rapidCode(const PreprocessorParams& p) const override {
        return "G00 " + positionCode(p) + " Z" + fmt(p.coordsDecimals, p.zMove.value());
    }
    std::string linearCode(const PreprocessorParams& p) const override {
        return "G01 " + positionCode(p) + " Z" + fmt(p.coordsDecimals, p.zCut.value());
    }
    std::string endCode(const PreprocessorParams& p) const override {
        std::string g = p.xyEnd ? "G00 X" + (*p.xyEnd)[0].str() + " Y" + (*p.xyEnd)[1].str() + "\n"
                                : "G00 " + positionCode(p);
        return g + " Z" + fmt(p.coordsDecimals, p.zEnd.value());
    }

private:
    static std::string xy(const PreprocessorParams& p) { return positionCodeCommon(p); }
};

/// Marlin: firmware Marlin (G0/G1 con su avance en cada línea).
class Marlin : public DefaultPreprocessor {
public:
    std::string name() const override { return "Marlin"; }
    std::string startCode(const PreprocessorParams& p) const override {
        StartStyle st = style("", StepsLines::ParenThenComment, unitsLine(p) + "G90");
        st.semicolon = true;
        return defaultLikeStart(p, st);
    }
    std::string startzCode(const PreprocessorParams& p) const override {
        return p.startZ ? "G0 Z" + fmt(p.coordsDecimals, p.startZ->value()) : "";
    }
    std::string liftCode(const PreprocessorParams& p) const override {
        return "G0 Z" + fmt(p.coordsDecimals, p.zMove.value()) + " " + rapidFeed(p);
    }
    std::string downCode(const PreprocessorParams& p) const override {
        return "G1 Z" + fmt(p.coordsDecimals, p.zCut.value()) + " F" + fmt(p.frDecimals, p.zFeedrate.value());
    }
    std::string toolchangeCode(const PreprocessorParams& p) const override {
        return defaultLikeToolchange(p, kMarlin_toolchange_code, "G0 Z");
    }
    std::string upToZeroCode(const PreprocessorParams& p) const override { return "G1 Z0 " + feedrateCode(p); }
    std::string rapidCode(const PreprocessorParams& p) const override {
        return "G0 " + positionCode(p) + " " + rapidFeed(p);
    }
    std::string linearCode(const PreprocessorParams& p) const override {
        return "G1 " + positionCode(p) + " F" + fmt(p.frDecimals, p.feedrate.value());
    }
    std::string endCode(const PreprocessorParams& p) const override {
        std::string gcode = "G0 Z" + fmt(p.frDecimals, p.zEnd.value()) + " " + rapidFeed(p) + "\n";
        if (p.xyEnd) gcode += "G0 X" + (*p.xyEnd)[0].str() + " Y" + (*p.xyEnd)[1].str() + " " + rapidFeed(p) + "\n";
        return gcode;
    }
    std::string feedrateCode(const PreprocessorParams& p) const override {
        return "G1 F" + fmt(p.frDecimals, p.feedrate.value());
    }
    std::string zFeedrateCode(const PreprocessorParams& p) const override {
        return "G1 F" + fmt(p.frDecimals, p.zFeedrate.value());
    }
    std::string spindleCode(const PreprocessorParams& p) const override {
        const std::string sdir = p.spindleDir == "CCW" ? "M4" : "M3";
        if (p.spindleSpeed && p.spindleSpeed->truthy()) return sdir + " S" + p.spindleSpeed->str();
        return sdir;
    }
    std::string spindleStopCode(const PreprocessorParams&) const override { return "M400\nM5"; }

protected:
    static std::string rapidFeed(const PreprocessorParams& p) { return "F" + fmt(p.frDecimals, p.feedrateRapid.value()); }
};

/// Repetier: firmware Repetier (como Marlin; el husillo con M106/M107 y la pausa con `@pause`).
class Repetier : public Marlin {
public:
    std::string name() const override { return "Repetier"; }
    std::string startCode(const PreprocessorParams& p) const override {
        StartStyle st = style(";This preprocessor is used with a motion controller loaded with REPETIER firmware.\n\n",
                              StepsLines::One, unitsLine(p) + "G90\nG94\n");
        st.semicolon = true;
        return defaultLikeStart(p, st);
    }
    std::string toolchangeCode(const PreprocessorParams& p) const override {
        return defaultLikeToolchange(p, kRepetier_toolchange_code, "G0 Z");
    }
    std::string spindleCode(const PreprocessorParams& p) const override {
        // 'M106 S%d' % spindlespeed: trunca.
        if (p.spindleSpeed && p.spindleSpeed->truthy()) return "M106 S" + pyInt(p.spindleSpeed->value());
        return "M106";
    }
    std::string spindleStopCode(const PreprocessorParams&) const override { return "M107"; }
};

/// Marlin_laser_FAN_pin y Marlin_laser_Spindle_pin: Marlin con un láser en un pin de ventilador o en el del husillo.
class MarlinLaser : public Marlin {
public:
    explicit MarlinLaser(bool fanPin) : fan_(fanPin) {}
    std::string name() const override { return fan_ ? "Marlin_laser_FAN_pin" : "Marlin_laser_Spindle_pin"; }
    std::string startCode(const PreprocessorParams& p) const override {
        const std::string units = " " + lower(p.units);
        std::string gcode = ";This preprocessor is used with a motion controller loaded with MARLIN firmware.\n";
        gcode += fan_ ? ";It is for the case when it is used together with a LASER connected on one of the FAN pins.\n\n"
                      : ";It is for the case when it is used together with a LASER connected on the SPINDLE connector.\n\n";
        gcode += ";Feedrate: " + p.feedrate.str() + units + "/min\n";
        gcode += ";Feedrate rapids: " + p.feedrateRapid.str() + units + "/min\n\n";
        gcode += ";Z Focus: " + p.zMove.str() + units + "\n";
        gcode += ";Steps per circle: " + std::to_string(p.stepsPerCircle) + "\n";
        if (p.options.type == "Excellon" || p.options.type == "Excellon Geometry")
            gcode += ";Preprocessor Excellon: " + p.ppExcellonName + "\n";
        else
            gcode += ";Preprocessor Geometry: " + p.ppGeometryName + "\n";
        if (p.xyEnd) gcode += "(X,Y End: " + pair(p.decimals, *p.xyEnd) + units + ")\n";
        else gcode += "(X,Y End: None" + units + (fan_ ? ")\n" : ")\n\n");
        if (fan_) gcode += "(Steps per circle: " + std::to_string(p.stepsPerCircle) + ")\n";
        gcode += ";X range: " + right9(fmt(p.coordsDecimals, p.options.xmin.value())) + " ... " +
                 right9(fmt(p.coordsDecimals, p.options.xmax.value())) + " " + units + "\n";
        gcode += ";Y range: " + right9(fmt(p.coordsDecimals, p.options.ymin.value())) + " ... " +
                 right9(fmt(p.coordsDecimals, p.options.ymax.value())) + " " + units + "\n\n";
        gcode += ";Laser Power (Spindle Speed): " + strOrNone(p.spindleSpeed) + "\n\n";
        gcode += unitsLine(p) + "G90";
        return gcode;
    }
    std::string startzCode(const PreprocessorParams& p) const override {
        // Así en el original: con Z inicial va a la Z de viaje.
        return p.startZ ? "G0 Z" + fmt(p.coordsDecimals, p.zMove.value()) : "";
    }
    std::string liftCode(const PreprocessorParams&) const override { return fan_ ? "M400\nM107" : "M400\nM5"; }
    std::string downCode(const PreprocessorParams& p) const override { return spindleCode(p); }
    std::string toolchangeCode(const PreprocessorParams&) const override { return ""; }
    std::string upToZeroCode(const PreprocessorParams& p) const override { return liftCode(p); }
    std::string spindleCode(const PreprocessorParams& p) const override {
        const std::string sdir = fan_ ? "M106" : (p.spindleDir == "CCW" ? "M4" : "M3");
        if (p.spindleSpeed && p.spindleSpeed->truthy()) return sdir + " S" + p.spindleSpeed->str();
        return sdir;
    }
    std::string dwellCode(const PreprocessorParams&) const override { return ""; }
    std::string spindleStopCode(const PreprocessorParams&) const override { return fan_ ? "M400\nM106 S0" : "M400\nM5"; }

private:
    bool fan_;
};

/// Z_laser: GRBL con láser en el husillo; en el cambio de herramienta sube a la Z de enfoque.
class ZLaser : public Preprocessor {
public:
    std::string name() const override { return "Z_laser"; }
    std::string startCode(const PreprocessorParams& p) const override {
        const std::string units = " " + lower(p.units);
        std::string gcode = "(This preprocessor is used with a motion controller loaded with GRBL firmware. )\n";
        gcode += "(It is for the case when it is used together with a LASER connected on the SPINDLE connector.)\n";
        gcode += "(On toolchange event the laser will move to a defined Z height to change the laser dot size.)\n\n";
        gcode += "(Feedrate: " + p.feedrate.str() + units + "/min)\n";
        gcode += "(Feedrate rapids: " + p.feedrateRapid.str() + units + "/min)\n\n";
        gcode += "(Z Focus: " + p.zMove.str() + units + ")\n";
        gcode += "(Steps per circle: " + std::to_string(p.stepsPerCircle) + ")\n";
        if (p.options.type == "Excellon" || p.options.type == "Excellon Geometry")
            gcode += "(Preprocessor Excellon: " + p.ppExcellonName + ")\n";
        else
            gcode += "(Preprocessor Geometry: " + p.ppGeometryName + ")\n\n";
        gcode += "(X range: " + right9(fmt(p.coordsDecimals, p.options.xmin.value())) + " ... " +
                 right9(fmt(p.coordsDecimals, p.options.xmax.value())) + " " + units + ")\n";
        gcode += "(Y range: " + right9(fmt(p.coordsDecimals, p.options.ymin.value())) + " ... " +
                 right9(fmt(p.coordsDecimals, p.options.ymax.value())) + " " + units + ")\n\n";
        gcode += "(Laser Power (Spindle Speed): " + strOrNone(p.spindleSpeed) + ")\n\n";
        gcode += unitsLine(p) + "G90\nG17\nG94";
        return gcode;
    }
    std::string startzCode(const PreprocessorParams&) const override { return ""; }
    std::string liftCode(const PreprocessorParams&) const override { return "M5"; }
    std::string downCode(const PreprocessorParams& p) const override { return spindleCodeCommon(p); }
    std::string toolchangeCode(const PreprocessorParams& p) const override {
        return "G00 Z" + fmt(p.coordsDecimals, p.zMove.value());
    }
    std::string upToZeroCode(const PreprocessorParams&) const override { return "M5"; }
    std::string positionCode(const PreprocessorParams& p) const override { return positionCodeCommon(p); }
    std::string rapidCode(const PreprocessorParams& p) const override { return "G00 " + positionCode(p); }
    std::string linearCode(const PreprocessorParams& p) const override {
        return "G01 " + positionCode(p) + " F" + fmt(p.frDecimals, p.feedrate.value());
    }
    std::string endCode(const PreprocessorParams& p) const override { return endCodeCommon(p); }
    std::string feedrateCode(const PreprocessorParams& p) const override {
        return "G01 F" + fmt(p.frDecimals, p.feedrate.value());
    }
    std::string zFeedrateCode(const PreprocessorParams& p) const override {
        return "G01 F" + fmt(p.frDecimals, p.zFeedrate.value());
    }
    std::string spindleCode(const PreprocessorParams& p) const override { return spindleCodeCommon(p); }
    std::string dwellCode(const PreprocessorParams&) const override { return ""; }
    std::string spindleStopCode(const PreprocessorParams&) const override { return "M5"; }
};

/// Roland_MDX_20: lenguaje RML-1 de Roland (unidades de 0,025 mm y velocidad en mm/s, entre 6 y 15).
class RolandMdx20 : public Preprocessor {
public:
    std::string name() const override { return "Roland_MDX_20"; }
    std::string startCode(const PreprocessorParams&) const override { return ";;^IN;\n^PA;"; }
    std::string startzCode(const PreprocessorParams&) const override { return ""; }
    std::string liftCode(const PreprocessorParams& p) const override { return move(p, rapidFeed(p), p.zMove.value()); }
    std::string downCode(const PreprocessorParams& p) const override { return move(p, feedrateCode(p), p.zCut.value()); }
    std::string toolchangeCode(const PreprocessorParams&) const override { return ""; }
    std::string upToZeroCode(const PreprocessorParams& p) const override {
        return feedrateCode(p) + "\n" + positionCode(p) + ",0;";
    }
    std::string positionCode(const PreprocessorParams& p) const override {
        const double x = inches(p) ? p.x / 25.4 : p.x;
        const double y = inches(p) ? p.y / 25.4 : p.y;
        return "Z" + fmt(1, x * 40.0) + "," + fmt(1, y * 40.0);
    }
    std::string rapidCode(const PreprocessorParams& p) const override { return move(p, rapidFeed(p), p.zMove.value()); }
    std::string linearCode(const PreprocessorParams& p) const override {
        // Sin `z` el original falla (AttributeError) y doformat devuelve "" (queda una línea vacía).
        return p.z ? move(p, feedrateCode(p), p.z->value()) : "";
    }
    std::string endCode(const PreprocessorParams& p) const override { return move(p, rapidFeed(p), p.zEnd.value()); }
    std::string feedrateCode(const PreprocessorParams& p) const override { return speed(p.feedrate.value()); }
    std::string zFeedrateCode(const PreprocessorParams& p) const override { return speed(p.zFeedrate.value()); }
    std::string spindleCode(const PreprocessorParams&) const override { return "!MC1"; }
    std::string dwellCode(const PreprocessorParams&) const override { return ""; }
    std::string spindleStopCode(const PreprocessorParams&) const override { return "!MC0"; }

private:
    static bool inches(const PreprocessorParams& p) { return upper(p.units) == "IN"; }
    static std::string speed(double feedrate) {
        double frSec = feedrate / 60;
        if (feedrate >= 900) frSec = 15;
        if (feedrate < 6) frSec = 6;
        return "V" + fmt(1, frSec) + ";";
    }
    static std::string rapidFeed(const PreprocessorParams& p) { return speed(p.feedrateRapid.value()); }
    std::string move(const PreprocessorParams& p, const std::string& feed, double z) const {
        if (inches(p)) z = z / 25.4;
        return feed + "\n" + positionCode(p) + "," + pyFloatRepr(z * 40.0) + ";";
    }
};

/// hpgl: plotter HPGL (unidades de 0,025 mm, recortadas a -32767..32768).
class Hpgl : public Preprocessor {
public:
    std::string name() const override { return "hpgl"; }
    std::string startCode(const PreprocessorParams&) const override { return "IN;\nPU;"; }
    std::string startzCode(const PreprocessorParams&) const override { return ""; }
    std::string liftCode(const PreprocessorParams&) const override { return "PU;\n"; }
    std::string downCode(const PreprocessorParams&) const override { return "PD;\n"; }
    std::string toolchangeCode(const PreprocessorParams& p) const override { return "SP" + std::to_string(p.tool) + ";"; }
    std::string upToZeroCode(const PreprocessorParams&) const override { return ""; }
    std::string positionCode(const PreprocessorParams& p) const override {
        const bool in = lower(p.units) == "in";
        auto unit = [&](double v) {
            // round() de Python redondea al par en los empates (como nearbyint con el modo por defecto).
            double r = std::nearbyint((in ? v * 25.4 : v) * 40);
            if (r <= -32767) r = -32767;
            if (r >= 32768) r = 32768;
            return fmt(p.coordsDecimals, r);
        };
        return "PA" + unit(p.x) + "," + unit(p.y) + ";";
    }
    std::string rapidCode(const PreprocessorParams& p) const override { return positionCode(p); }
    std::string linearCode(const PreprocessorParams& p) const override { return positionCode(p); }
    std::string endCode(const PreprocessorParams& p) const override { return positionCode(p); }
    std::string feedrateCode(const PreprocessorParams&) const override { return ""; }
    std::string zFeedrateCode(const PreprocessorParams&) const override { return ""; }
    std::string spindleCode(const PreprocessorParams&) const override { return ""; }
    std::string dwellCode(const PreprocessorParams&) const override { return ""; }
    std::string spindleStopCode(const PreprocessorParams&) const override { return ""; }
};

}  // namespace

std::unique_ptr<Preprocessor> makeOtherPreprocessor(const std::string& name, GcodeFlavor /*flavor*/) {
    if (name == "Default_no_M6") return std::make_unique<DefaultNoM6>();
    if (name == "grbl_11") return std::make_unique<Grbl11>();
    if (name == "GRBL_11_no_M6") return std::make_unique<Grbl11>(kGRBL_11_no_M6_toolchange_code, "GRBL_11_no_M6");
    if (name == "Toolchange_Manual") return std::make_unique<ToolchangeManual>();
    if (name == "Toolchange_Custom") return std::make_unique<ToolchangeCustom>();
    if (name == "Toolchange_Probe_MACH3") return std::make_unique<ToolchangeProbeMach3>();
    if (name == "Berta_CNC") return std::make_unique<BertaCnc>();
    if (name == "ISEL_CNC") return std::make_unique<IselCnc>();
    if (name == "ISEL_ICP_CNC") return std::make_unique<IselIcpCnc>();
    if (name == "line_xyz") return std::make_unique<LineXyz>();
    if (name == "Marlin") return std::make_unique<Marlin>();
    if (name == "Repetier") return std::make_unique<Repetier>();
    if (name == "Marlin_laser_FAN_pin") return std::make_unique<MarlinLaser>(true);
    if (name == "Marlin_laser_Spindle_pin") return std::make_unique<MarlinLaser>(false);
    if (name == "Z_laser") return std::make_unique<ZLaser>();
    if (name == "Roland_MDX_20") return std::make_unique<RolandMdx20>();
    if (name == "hpgl") return std::make_unique<Hpgl>();
    if (name == "Paste_1") return std::make_unique<SolderPastePreprocessor>();
    return nullptr;
}

}  // namespace pp_internal

// ------------------------------------------------------------------------------------- Paste_1

std::string SolderPastePreprocessor::startCode(const PreprocessorParams& p) const {
    using namespace pp_internal;
    const std::string units = " " + lower(p.units);
    std::string gcode;
    gcode += "(TOOL DIAMETER: " + p.options.toolDia.str() + units + ")\n";
    gcode += "(Feedrate_XY: " + p.frXY.str() + units + "/min)\n";
    gcode += "(Feedrate_Z: " + p.frZ.str() + units + "/min)\n";
    gcode += "(Feedrate_Z_Dispense: " + p.frZDispense.str() + units + "/min)\n";
    gcode += "(Z_Dispense_Start: " + p.zStart.str() + units + ")\n";
    gcode += "(Z_Dispense: " + p.zDispense.str() + units + ")\n";
    gcode += "(Z_Dispense_Stop: " + p.zStop.str() + units + ")\n";
    gcode += "(Z_Travel: " + p.zTravel.str() + units + ")\n";
    gcode += "(Z Toolchange: " + p.zToolchange.str() + units + ")\n";
    // El original lee el X,Y como texto "x,y" (sin él falla); el port escribe "None".
    gcode += "(X,Y Toolchange: " + (p.xyToolchange ? pair(p.decimals, *p.xyToolchange) : std::string("None")) + units +
             ")\n";
    if (p.ppSolderpasteName.find("Paste") != std::string::npos)
        gcode += "(Preprocessor SolderPaste Dispensing Geometry: " + p.ppSolderpasteName + ")\n\n";
    gcode += "(X range: " + right9(fmt(p.coordsDecimals, p.options.xmin.value())) + " ... " +
             right9(fmt(p.coordsDecimals, p.options.xmax.value())) + " " + units + ")\n";
    gcode += "(Y range: " + right9(fmt(p.coordsDecimals, p.options.ymin.value())) + " ... " +
             right9(fmt(p.coordsDecimals, p.options.ymax.value())) + " " + units + ")\n\n";
    gcode += "(Spindle Speed FWD: " + p.speedFwd.str() + " RPM)\n";
    gcode += "(Spindle Speed REV: " + p.speedRev.str() + " RPM)\n";
    gcode += "(Dwell FWD: " + p.dwellFwd.str() + " RPM)\n";
    gcode += "(Dwell REV: " + p.dwellRev.str() + " RPM)\n";
    gcode += upper(p.units) == "IN" ? "G20\n" : "G21\n";
    gcode += "G90\nG94\n";
    return gcode;
}

std::string SolderPastePreprocessor::liftCode(const PreprocessorParams& p) const {
    return "G00 Z" + pp_internal::fmt(p.coordsDecimals, p.zTravel.value());
}
std::string SolderPastePreprocessor::downZStartCode(const PreprocessorParams& p) const {
    return "G01 Z" + pp_internal::fmt(p.coordsDecimals, p.zStart.value());
}
std::string SolderPastePreprocessor::liftZDispenseCode(const PreprocessorParams& p) const {
    return "G01 Z" + pp_internal::fmt(p.coordsDecimals, p.zDispense.value());
}
std::string SolderPastePreprocessor::downZStopCode(const PreprocessorParams& p) const {
    return "G01 Z" + pp_internal::fmt(p.coordsDecimals, p.zStop.value());
}

std::string SolderPastePreprocessor::toolchangeCode(const PreprocessorParams& p) const {
    using namespace pp_internal;
    double x = 0.0, y = 0.0;
    if (p.xyToolchange) {
        x = (*p.xyToolchange)[0].value();
        y = (*p.xyToolchange)[1].value();
    }
    const std::map<std::string, std::string> v = {{"x_toolchange", fmt(p.coordsDecimals, x)},
                                                  {"y_toolchange", fmt(p.coordsDecimals, y)},
                                                  {"z_toolchange", fmt(p.coordsDecimals, p.zToolchange.value())},
                                                  {"tool", std::to_string(p.tool)},
                                                  {"toolC", fmt(p.decimals, p.toolC.value())}};
    // En el original la lista leída del texto nunca es None: siempre usa la plantilla con X,Y.
    return pyFormat(pp_templates::kPaste_1_toolchange_code[0], v);
}

std::string SolderPastePreprocessor::positionCode(const PreprocessorParams& p) const {
    return pp_internal::positionCodeCommon(p);
}
std::string SolderPastePreprocessor::rapidCode(const PreprocessorParams& p) const {
    return "G00 " + positionCode(p) + "\nG00 Z" + pp_internal::fmt(p.coordsDecimals, p.zTravel.value());
}
std::string SolderPastePreprocessor::linearCode(const PreprocessorParams& p) const { return "G01 " + positionCode(p); }

std::string SolderPastePreprocessor::endCode(const PreprocessorParams& p) const {
    std::string gcode = "G00 Z" + pp_internal::fmt(p.frDecimals, p.zToolchange.value()) + "\n";
    if (p.xyEnd)
        gcode += "G00 X" + pyFloatRepr((*p.xyEnd)[0].value()) + " Y" + pyFloatRepr((*p.xyEnd)[1].value()) + "\n";
    return gcode;
}

std::string SolderPastePreprocessor::feedrateXYCode(const PreprocessorParams& p) const {
    return "G01 F" + pp_internal::fmt(p.frDecimals, p.frXY.value());
}
std::string SolderPastePreprocessor::zFeedrateCode(const PreprocessorParams& p) const {
    return "G01 F" + pp_internal::fmt(p.frDecimals, p.frZ.value());
}
std::string SolderPastePreprocessor::feedrateZDispenseCode(const PreprocessorParams& p) const {
    return "G01 F" + pp_internal::fmt(p.frDecimals, p.frZDispense.value());
}
std::string SolderPastePreprocessor::spindleFwdCode(const PreprocessorParams& p) const {
    if (p.spindleSpeed && p.spindleSpeed->truthy()) return "M03 S" + pyFloatRepr(p.speedFwd.value());
    return "M03";
}
std::string SolderPastePreprocessor::spindleRevCode(const PreprocessorParams& p) const {
    if (p.spindleSpeed && p.spindleSpeed->truthy()) return "M04 S" + pyFloatRepr(p.speedRev.value());
    return "M04";
}
std::string SolderPastePreprocessor::dwellFwdCode(const PreprocessorParams& p) const {
    return p.dwellTime.truthy() ? "G4 P" + pyFloatRepr(p.dwellFwd.value()) : "";
}
std::string SolderPastePreprocessor::dwellRevCode(const PreprocessorParams& p) const {
    return p.dwellTime.truthy() ? "G4 P" + pyFloatRepr(p.dwellRev.value()) : "";
}

}  // namespace flatcam
