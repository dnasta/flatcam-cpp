// Ayudantes internos de los postprocesadores (formato de números como Python y bloques comunes). No es API pública.
#pragma once

#include <cctype>
#include <cmath>
#include <cstdio>
#include <map>
#include <string>

#include "cam/Preprocessor.h"

namespace flatcam::pp_internal {

/// Equivale a `'%.*f' % (decimals, value)`.
inline std::string fmt(int decimals, double value) {
    char buf[64];
    std::snprintf(buf, sizeof(buf), "%.*f", decimals, value);
    return buf;
}

/// Equivale a `'{: >9s}'.format(text)`.
inline std::string right9(const std::string& text) {
    return text.size() >= 9 ? text : std::string(9 - text.size(), ' ') + text;
}

inline std::string lower(std::string s) {
    for (char& c : s) c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    return s;
}

inline std::string upper(std::string s) {
    for (char& c : s) c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
    return s;
}

/// `str(p['startz'])`, `str(p['spindlespeed'])`...: "None" si no hay valor.
inline std::string strOrNone(const std::optional<PyNumber>& v) { return v ? v->str() : "None"; }

/// `"%.*f, %.*f" % (decimals, a, decimals, b)`.
inline std::string pair(int decimals, const std::array<PyNumber, 2>& xy) {
    return fmt(decimals, xy[0].value()) + ", " + fmt(decimals, xy[1].value());
}

inline std::string positionCodeCommon(const PreprocessorParams& p) {
    return "X" + fmt(p.coordsDecimals, p.x) + " Y" + fmt(p.coordsDecimals, p.y);
}

inline std::string spindleCodeCommon(const PreprocessorParams& p) {
    const std::string sdir = (p.spindleDir == "CCW") ? "M04" : "M03";
    if (p.spindleSpeed && p.spindleSpeed->truthy()) return sdir + " S" + p.spindleSpeed->str();
    return sdir;
}

inline std::string endCodeCommon(const PreprocessorParams& p) {
    std::string gcode = "G00 Z" + fmt(p.frDecimals, p.zEnd.value()) + "\n";
    if (p.xyEnd) gcode += "G00 X" + (*p.xyEnd)[0].str() + " Y" + (*p.xyEnd)[1].str() + "\n";
    return gcode;
}

/// `plantilla.format(clave=valor, ...)` para plantillas con `{clave}` simples.
inline std::string pyFormat(const std::string& tpl, const std::map<std::string, std::string>& values) {
    std::string out;
    out.reserve(tpl.size() + 64);
    for (std::size_t i = 0; i < tpl.size();) {
        if (tpl[i] == '{') {
            const std::size_t j = tpl.find('}', i);
            if (j != std::string::npos) {
                const auto it = values.find(tpl.substr(i + 1, j - i - 1));
                if (it != values.end()) {
                    out += it->second;
                    i = j + 1;
                    continue;
                }
            }
        }
        out += tpl[i++];
    }
    return out;
}

/// Cómo se escriben las líneas "Steps per circle" en la cabecera.
enum class StepsLines {
    One,               ///< Una línea, en el estilo de comentario de la cabecera.
    Two,               ///< La misma línea dos veces (grbl_11, Berta_CNC, ISEL_CNC, line_xyz...).
    ParenThenComment   ///< "(Steps per circle: n)" y después ";Steps per circle: n" (Marlin, ISEL_ICP_CNC).
};

/// Variantes de la cabecera de `default` que usan los demás postprocesadores.
struct StartStyle {
    std::string intro;              ///< Texto del principio.
    std::string preamble;           ///< Texto entre la presentación y el bloque de parámetros (Berta_CNC).
    bool semicolon{false};          ///< Comentarios ";texto" en vez de "(texto)" (Marlin, Repetier, ISEL_ICP_CNC).
    StepsLines steps{StepsLines::One};  ///< Líneas "Steps per circle".
    bool endXYGatedByToolchange{false}; ///< "X,Y End" depende de si hay X,Y de cambio de herramienta (Berta_CNC).
    std::string tail;               ///< Texto final, después de "Spindle Speed".
};

/// Cabecera de `default` y de sus variantes (ver @ref StartStyle).
std::string defaultLikeStart(const PreprocessorParams& p, const StartStyle& style);

/// `toolchange_code` de `default` y sus variantes, con sus cuatro plantillas (Excellon con y sin X,Y; geometría con y sin
/// X,Y) en ese orden. @p plunge es el comando del "plunge" rápido (vacío si el postprocesador no lo tiene) y @p extra
/// agrega valores a la plantilla.
std::string defaultLikeToolchange(const PreprocessorParams& p, const char* const* templates,
                                  const std::string& plunge = "G00 Z",
                                  const std::map<std::string, std::string>& extra = {});

/// `int(v)` de Python (trunca hacia cero) como texto.
inline std::string pyInt(double v) { return std::to_string(static_cast<long long>(std::trunc(v))); }

/// Los postprocesadores que no son `default` ni `GRBL_laser`.
std::unique_ptr<Preprocessor> makeOtherPreprocessor(const std::string& name, GcodeFlavor flavor);

}  // namespace flatcam::pp_internal
