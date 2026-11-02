#include "cam/Preprocessor.h"

#include "cam/generated/PreprocessorTemplates.h"
#include "cam/internal/PpCommon.h"

#include <cmath>
#include <cstdio>

namespace flatcam {

using namespace pp_internal;

// ------------------------------------------------------------------------------------- default

std::string DefaultPreprocessor::startCode(const PreprocessorParams& p) const {
    StartStyle st;
    st.intro = "(This preprocessor is the default preprocessor used by FlatCAM.)\n"
               "(It is made to work with MACH3 compatible motion controllers.)\n\n";
    st.tail = std::string(upper(p.units) == "IN" ? "G20\n" : "G21\n") + "G90\nG94";
    return pp_internal::defaultLikeStart(p, st);
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
    return pp_internal::defaultLikeToolchange(p, pp_templates::kdefault_toolchange_code);
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
    return pp_internal::makeOtherPreprocessor(name, flavor);
}

}  // namespace flatcam
