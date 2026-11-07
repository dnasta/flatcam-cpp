// Exportar a SVG (SvgExport) contra export_svg original (golden_svg.h).
#include <gtest/gtest.h>

#include <cmath>
#include <set>
#include <sstream>

#include "golden_svg.h"
#include "io/SvgExport.h"
#include "parsers/Excellon.h"
#include "parsers/Gerber.h"

using namespace flatcam;

TEST(SvgExport, MatchesOriginal) {
    for (const auto& c : golden_svg::kCases) {
        SCOPED_TRACE(c.file);
        const std::string path = std::string(FLATCAM_TEST_DATA_DIR) + "/" + c.file;
        ShapeList shapes;
        if (c.excellon) {
            Excellon e;
            ASSERT_TRUE(e.parseFile(path));
            e.createGeometry();
            shapes = e.solidGeometry();
        } else {
            Gerber g;
            ASSERT_TRUE(g.parseFile(path));
            shapes = g.solidGeometry();
        }
        const std::string mine = exportSvg(shapes, "mm");
        if (mine == c.svg) continue;
        // La unión de GEOS cambia de versión en versión el vértice en que empieza cada anillo (el oráculo usa la 3.11):
        // se comparan la cabecera y el conjunto de coordenadas escritas (cada una con el texto de Python).
        // (y las últimas cifras del buffer de GEOS): coordenadas redondeadas a la micra.
        auto tokens = [](const std::string& t) {
            std::set<std::pair<long long, long long>> out;
            std::stringstream ss(t);
            std::string w;
            while (ss >> w) {
                const auto comma = w.find(',');
                if (comma == std::string::npos || w.find('=') != std::string::npos) continue;
                try {
                    out.insert({std::llround(std::stod(w.substr(0, comma)) * 1e6), std::llround(std::stod(w.substr(comma + 1)) * 1e6)});
                } catch (...) {
                }
            }
            return out;
        };
        const std::string ref = c.svg;
        EXPECT_EQ(mine.substr(0, mine.find("<g transform")), ref.substr(0, ref.find("<g transform")));
        EXPECT_EQ(tokens(mine), tokens(ref));
    }
}
