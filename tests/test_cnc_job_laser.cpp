#include <gtest/gtest.h>

#include <cstdio>
#include <fstream>
#include <sstream>

#include "cam/CncJob.h"

using namespace flatcam;

namespace {

std::string readWithoutCr(const std::string& name) {
    std::ifstream in(std::string(FLATCAM_TEST_DATA_DIR) + "/" + name, std::ios::binary);
    std::stringstream buf;
    buf << in.rdbuf();
    std::string out;
    out.reserve(buf.str().size());
    for (char c : buf.str()) {
        if (c != '\r') out += c;
    }
    return out;
}

std::vector<std::string> splitLines(const std::string& text) {
    std::vector<std::string> lines;
    std::size_t pos = 0;
    while (true) {
        const std::size_t nl = text.find('\n', pos);
        if (nl == std::string::npos) {
            lines.push_back(text.substr(pos));
            break;
        }
        lines.push_back(text.substr(pos, nl - pos));
        pos = nl + 1;
    }
    return lines;
}

/// Trazados de un G-code de láser: empiezan en un `G00 X.. Y..` seguido de `G01 F..` y `M03`, y siguen con `G01 X.. Y.. F..`.
ShapeList pathsOf(const std::vector<std::string>& lines) {
    ShapeList out;
    std::vector<Point2D> cur;
    auto flush = [&] {
        if (cur.size() >= 2) out.push_back(Shape::lineString(cur));
        cur.clear();
    };
    for (std::size_t i = 0; i < lines.size(); ++i) {
        double x = 0, y = 0;
        if (std::sscanf(lines[i].c_str(), "G00 X%lf Y%lf", &x, &y) == 2 && i + 2 < lines.size() &&
            lines[i + 1].rfind("G01 F", 0) == 0 && lines[i + 2].rfind("M03", 0) == 0) {
            flush();
            cur.push_back({x, y});
        } else if (!cur.empty() && std::sscanf(lines[i].c_str(), "G01 X%lf Y%lf F", &x, &y) == 2) {
            cur.push_back({x, y});
        }
    }
    flush();
    return out;
}

/// Configuración de los trabajos de láser del usuario (NCC y Paint con `GRBL_laser`), según su proyecto.
GeometryJobSettings laserSettings(const std::vector<std::string>& lines, GcodeFlavor flavor) {
    GeometryJobSettings s;
    s.preprocessor = "GRBL_laser";
    s.order = DrillOrder::FileOrder;
    s.toolchange = false;
    s.flavor = flavor;
    GeometryToolSettings& t = s.toolSettings;
    t.toolDia = 0.1;
    t.cutZ = -3.5;
    t.depthPerPass = 0.8;
    t.travelZ = 2.0;
    t.feedrate = 500.0;
    t.feedrateZ = 100.0;
    t.feedrateRapid = 1500.0;
    t.spindleSpeed = 300.0;
    t.endZ = 19.0;
    t.endXY = std::array<double, 2>{0.0, 0.0};
    t.toolchangeZ = 19.0;
    t.toolchangeXY = std::array<double, 2>{0.0, 0.0};
    // La cabecera imprime los límites del objeto, que no se pueden recuperar de los trazados redondeados a 4 decimales.
    BoundingBox b{0, 0, 0, 0};
    for (const std::string& l : lines) {
        double a = 0, c = 0;
        if (std::sscanf(l.c_str(), "(X range: %lf ... %lf", &a, &c) == 2) {
            b.xmin = a;
            b.xmax = c;
        } else if (std::sscanf(l.c_str(), "(Y range: %lf ... %lf", &a, &c) == 2) {
            b.ymin = a;
            b.ymax = c;
        }
    }
    s.headerBounds = b;
    return s;
}

GcodeHeaderInfo headerOf(const std::vector<std::string>& lines) {
    GcodeHeaderInfo h;
    for (const std::string& l : lines) {
        if (l.rfind("(Name: ", 0) == 0) h.name = l.substr(7, l.size() - 8);
        if (l.rfind("(Created on ", 0) == 0) h.createdOn = l.substr(12, l.size() - 13);
    }
    return h;
}

void expectReproduces(const std::string& file) {
    const std::string real = readWithoutCr(file);
    const std::vector<std::string> lines = splitLines(real);
    const ShapeList paths = pathsOf(lines);
    ASSERT_GT(paths.size(), 10u);
    CncJob job;
    ASSERT_TRUE(job.generateFromGeometry(paths, laserSettings(lines, GcodeFlavor::FlatCAM8_994))) << job.lastError();
    const std::string mine = job.exportGcode(headerOf(lines));
    if (mine != real) {
        const std::vector<std::string> m = splitLines(mine);
        for (std::size_t i = 0; i < std::min(m.size(), lines.size()); ++i) {
            if (m[i] != lines[i]) {
                ADD_FAILURE() << file << ": primera diferencia en la línea " << i + 1 << "\n  real: " << lines[i] << "\n  mío : " << m[i];
                return;
            }
        }
        ADD_FAILURE() << file << ": distinta cantidad de líneas (real " << lines.size() << ", mío " << m.size() << ")";
    }
}

}  // namespace

TEST(CncJobLaser, ReproducesTheUsersPaintFileFromItsOwnPaths) { expectReproduces("DosCau_PPal-Logo.nc"); }

TEST(CncJobLaser, ReproducesTheUsersNccFileFromItsOwnPaths) { expectReproduces("DosCau_PPal-B_Cu.nc"); }

TEST(CncJobLaser, FlavorsDifferOnlyWhereFlatcamChanged) {
    const std::vector<std::string> lines = splitLines(readWithoutCr("DosCau_PPal-Logo.nc"));
    const ShapeList paths = pathsOf(lines);
    CncJob legacy, current;
    ASSERT_TRUE(legacy.generateFromGeometry(paths, laserSettings(lines, GcodeFlavor::FlatCAM8_994)));
    ASSERT_TRUE(current.generateFromGeometry(paths, laserSettings(lines, GcodeFlavor::FlatCAM2024_4)));
    const std::string a = legacy.exportGcode({}), b = current.exportGcode({});
    EXPECT_NE(a.find("G00 Z0\n\nG21"), std::string::npos);
    EXPECT_EQ(b.find("G00 Z0"), std::string::npos);
    EXPECT_EQ(a.find("G00 Z19.00"), std::string::npos);
    EXPECT_NE(b.find("G00 Z19.00"), std::string::npos);
    // Los trazados son idénticos: se compara el cuerpo, desde el primer `G00 X` hasta el último `M5`.
    auto body = [](const std::string& g) {
        const std::size_t first = g.find("G00 X");
        const std::size_t last = g.rfind("M5\n");
        return g.substr(first, last - first);
    };
    EXPECT_FALSE(body(a).empty());
    EXPECT_EQ(body(a), body(b));
}
