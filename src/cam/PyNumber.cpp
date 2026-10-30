#include "cam/PyNumber.h"

#include <charconv>
#include <cmath>
#include <cstdio>
#include <cstdlib>

namespace flatcam {

std::string pyFloatRepr(double value) {
    if (std::isnan(value)) return "nan";
    if (std::isinf(value)) return value < 0 ? "-inf" : "inf";
    if (value == 0.0) return std::signbit(value) ? "-0.0" : "0.0";

    // Dígitos más cortos que reproducen el valor: "d.ddde±XX" (notación científica de to_chars).
    char buf[64];
    const auto res = std::to_chars(buf, buf + sizeof(buf), value, std::chars_format::scientific);
    const std::string sci(buf, res.ptr);

    const bool negative = sci[0] == '-';
    const std::string body = negative ? sci.substr(1) : sci;
    const std::size_t epos = body.find('e');
    std::string mantissa = body.substr(0, epos);
    const int exp10 = std::atoi(body.c_str() + epos + 1);

    std::string digits;
    for (char c : mantissa) {
        if (c != '.') digits += c;
    }

    std::string out;
    if (exp10 >= -4 && exp10 < 16) {
        if (exp10 >= 0) {
            // La parte entera tiene exp10 + 1 dígitos.
            const std::size_t intLen = static_cast<std::size_t>(exp10) + 1;
            if (digits.size() <= intLen) {
                out = digits + std::string(intLen - digits.size(), '0') + ".0";
            } else {
                out = digits.substr(0, intLen) + "." + digits.substr(intLen);
            }
        } else {
            out = "0." + std::string(static_cast<std::size_t>(-exp10 - 1), '0') + digits;
        }
    } else {
        out = digits.substr(0, 1);
        if (digits.size() > 1) out += "." + digits.substr(1);
        char e[16];
        std::snprintf(e, sizeof(e), "e%c%02d", exp10 < 0 ? '-' : '+', std::abs(exp10));
        out += e;
    }
    return negative ? "-" + out : out;
}

std::string PyNumber::str() const {
    if (isInt_) return std::to_string(static_cast<long long>(value_));
    return pyFloatRepr(value_);
}

}  // namespace flatcam
