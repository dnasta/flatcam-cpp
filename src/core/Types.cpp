#include "Types.h"

#include <algorithm>
#include <cctype>
#include <stdexcept>

namespace flatcam {

Units unitsFromString(const std::string& text) {
    std::string up = text;
    std::transform(up.begin(), up.end(), up.begin(),
                   [](unsigned char ch) { return static_cast<char>(std::toupper(ch)); });
    if (up == "MM") return Units::Millimeters;
    if (up == "IN") return Units::Inches;
    throw std::invalid_argument("Unidad no soportada: " + text);
}

std::string toString(Units units) {
    return units == Units::Millimeters ? "MM" : "IN";
}

}  // namespace flatcam
