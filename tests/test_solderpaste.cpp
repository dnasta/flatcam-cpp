// Pasta de soldar contra el original (tests/tools/oracle_solderpaste.py -> golden_solderpaste.h).
#include <gtest/gtest.h>

#include <algorithm>
#include <string>
#include <vector>

#include "golden_solderpaste.h"
#include "parsers/Gerber.h"
#include "tools/SolderPaste.h"

using namespace flatcam;

namespace {

/// Los parámetros de fábrica de una boquilla (tools_solderpaste_*), con el tipo de Python de cada valor.
PreprocessorParams pasteParams(int uid, double dia, const std::vector<double>& b) {
    PreprocessorParams p;
    p.units = "MM";
    p.coordsDecimals = 4;
    p.frDecimals = 2;
    p.tool = uid;
    p.toolC = PyNumber(dia);
    p.options.xmin = PyNumber(b[0]);
    p.options.ymin = PyNumber(b[1]);
    p.options.xmax = PyNumber(b[2]);
    p.options.ymax = PyNumber(b[3]);
    p.options.toolDia = PyNumber(dia);
    p.zStart = PyNumber(0.05);
    p.zDispense = PyNumber(0.1);
    p.zStop = PyNumber(0.05);
    p.zTravel = PyNumber(0.1);
    p.zToolchange = PyNumber(1.0);
    p.xyToolchange = std::array<PyNumber, 2>{PyNumber(0.0), PyNumber(0.0)};
    p.xyEnd = std::array<PyNumber, 2>{PyNumber(0.0), PyNumber(0.0)};
    p.frXY = PyNumber(150);
    p.frZ = PyNumber(150);
    p.frZDispense = PyNumber(1.0);
    p.speedFwd = PyNumber(300);
    p.dwellFwd = PyNumber(1);
    p.speedRev = PyNumber(200);
    p.dwellRev = PyNumber(1);
    p.dwellTime = PyNumber(1000);  // CNCjob(dwelltime=1000): Paste_1 solo mira si es verdadero
    return p;
}

}  // namespace

TEST(SolderPaste, MatchesTheOriginal) {
    for (const auto& c : golden_solderpaste::kCases) {
        SCOPED_TRACE(std::string(c.file) + " " + std::to_string(c.nozzles.size()));
        Gerber g;
        ASSERT_TRUE(g.parseFile(std::string(FLATCAM_TEST_DATA_DIR) + "/" + c.file));
        std::vector<std::pair<int, double>> nz;
        for (std::size_t i = 0; i < c.nozzles.size(); ++i) nz.push_back({static_cast<int>(i) + 1, c.nozzles[i]});
        const PasteGeometry r = solderPasteGeometry(g.solidGeometry(), nz, 4, true);
        ASSERT_EQ(r.error.empty(), c.created) << r.error;
        if (!c.created) continue;
        ASSERT_EQ(r.tools.size(), c.tools.size());
        for (std::size_t i = 0; i < c.tools.size(); ++i) {
            const auto& want = c.tools[i];
            const PasteTool& t = r.tools[i];
            EXPECT_EQ(t.uid, want.uid);
            EXPECT_EQ(t.dia, want.dia);
            ASSERT_EQ(t.paths.size(), want.paths.size());
            // El orden de los pads es el de la geometría sólida, que el parser del port no da igual que la unión de
            // Shapely en todos los Gerber: se compara como conjunto (el G-code, que elige el más cercano, sí entero).
            for (const std::string& w : want.paths) {
                const Shape ws = Shape::fromWkt(w);
                bool found = false;
                for (const Shape& got : t.paths)
                    if (got.equalsExact(ws, 1e-9)) found = true;
                EXPECT_TRUE(found) << w;
            }
            // Igual texto por texto, salvo los empates exactos de rtree.nearest (un pad simétrico cuyos dos extremos quedan
            // a la misma distancia: el original lo recorre al revés; AGENTS.md 11.8): entonces, las mismas líneas (sin
            // distinguir el rápido del lineal hacia cada extremo).
            const std::string got = solderPasteGcode(t.paths, pasteParams(t.uid, t.dia, c.bounds), true);
            if (got != want.gcode) {
                auto lines = [](const std::string& x) {
                    std::vector<std::string> v;
                    std::size_t a = 0, b;
                    while ((b = x.find('\n', a)) != std::string::npos) {
                        std::string l = x.substr(a, b - a);
                        // Un trazo recorrido al revés cambia qué extremo es el rápido (G00) y cuál el lineal (G01).
                        if (l.rfind("G00 X", 0) == 0) l = "G01 X" + l.substr(5);
                        v.push_back(l);
                        a = b + 1;
                    }
                    std::sort(v.begin(), v.end());
                    return v;
                };
                EXPECT_EQ(lines(got), lines(want.gcode));
            }
        }
    }
}
