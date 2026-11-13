#include "parsers/ImageImport.h"

namespace flatcam {

Shape importImage(const ImageBands& img, const std::string& units, double dpi, const std::string& mode,
                  const std::array<double, 4>& mask) {
    const double f = (units == "MM" || units == "mm") ? 25.4 / dpi : 1 / dpi;
    const std::vector<double>& r = img.red;
    const std::vector<double>& g = img.green.empty() ? img.red : img.green;
    const std::vector<double>& b = img.blue.empty() ? img.red : img.blue;
    // shapes(total, mask=...): las regiones de píxeles con el mismo valor; como al final se unen todas, alcanza con la
    // unión de los píxeles elegidos. Se arma con un rectángulo por tramo de una fila.
    ShapeList runs;
    for (int row = 0; row < img.height; ++row) {
        int start = -1;
        for (int col = 0; col <= img.width; ++col) {
            bool on = false;
            if (col < img.width) {
                const std::size_t i = static_cast<std::size_t>(row) * img.width + col;
                on = mode == "black" ? r[i] <= mask[0] : (r[i] <= mask[1] || g[i] <= mask[2] || b[i] <= mask[3]);
            }
            if (on && start < 0) start = col;
            if (!on && start >= 0) {
                // scale(f, f, origin=(0, 0)) y después scale(1, -1): x = col * f, y = -(fila * f).
                const double x0 = start * f, x1 = col * f;
                const double y0 = -(row * f), y1 = -((row + 1) * f);
                runs.push_back(Shape::polygon({{x0, y0}, {x0, y1}, {x1, y1}, {x1, y0}}));
                start = -1;
            }
        }
    }
    if (runs.empty()) return Shape::collection({});
    return Shape::unaryUnion(runs);
}

}  // namespace flatcam
