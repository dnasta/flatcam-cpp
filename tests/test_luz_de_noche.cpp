// Pruebas de punta a punta contra los G-code que generó el FlatCAM 2024.4 del usuario (placa "luz de noche Pascual").
//
// A diferencia de los archivos del dosificador (FlatCAM 8.994), estos salieron del mismo FlatCAM que se porta, así que
// sirven para medir el port sin diferencias de versión. Parámetros: capturas del usuario y `luz_de_noche.params.json`.
// En ese proyecto el cobre inferior, el logo y el taladro están reflejados en X, igual que en el dosificador.

#include <gtest/gtest.h>

#include <algorithm>
#include <array>
#include <cmath>
#include <cstdio>
#include <fstream>
#include <sstream>

#include "cam/CncJob.h"
#include "parsers/Excellon.h"
#include "parsers/Gerber.h"
#include "tools/CutOut.h"
#include "tools/Ncc.h"
#include "tools/Paint.h"

using namespace flatcam;

namespace {

const std::string kDir = std::string(FLATCAM_TEST_DATA_DIR) + "/luz_de_noche/luz de noche Pascual-";

std::vector<std::string> splitLines(const std::string& text) {
    std::vector<std::string> lines;
    std::string cur;
    for (char c : text) {
        if (c == '\r') continue;
        if (c == '\n') {
            lines.push_back(cur);
            cur.clear();
        } else {
            cur += c;
        }
    }
    lines.push_back(cur);
    return lines;
}

std::string readFile(const std::string& path) {
    std::ifstream in(path, std::ios::binary);
    std::stringstream buf;
    buf << in.rdbuf();
    return buf.str();
}

/// Cabecera de comentarios de un archivo de la 2024.4 (nombre y fecha salen del propio archivo).
GcodeHeaderInfo headerOf(const std::vector<std::string>& lines) {
    GcodeHeaderInfo h;
    h.version = "2024.4";
    h.versionDate = "2024/04/15";
    for (const std::string& l : lines) {
        if (l.rfind("(Name: ", 0) == 0) h.name = l.substr(7, l.size() - 8);
        if (l.rfind("(Created on ", 0) == 0) h.createdOn = l.substr(12, l.size() - 13);
    }
    return h;
}

/// Trazados de un G-code de láser (empiezan en `G00 X.. Y..` + `G01 F..` + `M03`).
std::vector<std::vector<Point2D>> laserPaths(const std::vector<std::string>& L) {
    std::vector<std::vector<Point2D>> out;
    std::vector<Point2D> cur;
    for (std::size_t i = 0; i < L.size(); ++i) {
        double x = 0, y = 0;
        if (std::sscanf(L[i].c_str(), "G00 X%lf Y%lf", &x, &y) == 2 && i + 2 < L.size() && L[i + 1].rfind("G01 F", 0) == 0 &&
            L[i + 2].rfind("M03", 0) == 0) {
            if (cur.size() >= 2) out.push_back(cur);
            cur = {{x, y}};
        } else if (!cur.empty() && std::sscanf(L[i].c_str(), "G01 X%lf Y%lf F", &x, &y) == 2) {
            cur.push_back({x, y});
        }
    }
    if (cur.size() >= 2) out.push_back(cur);
    return out;
}

double pathLength(const std::vector<Point2D>& p) {
    double l = 0;
    for (std::size_t i = 0; i + 1 < p.size(); ++i) l += std::hypot(p[i + 1].x - p[i].x, p[i + 1].y - p[i].y);
    return l;
}

double distanceToPolyline(const Point2D& p, const std::vector<Point2D>& line) {
    double d = 1e300;
    for (std::size_t i = 0; i + 1 < line.size(); ++i) {
        const Point2D a = line[i], b = line[i + 1];
        const double dx = b.x - a.x, dy = b.y - a.y, len2 = dx * dx + dy * dy;
        const double t = len2 > 0 ? std::clamp(((p.x - a.x) * dx + (p.y - a.y) * dy) / len2, 0.0, 1.0) : 0.0;
        d = std::min(d, std::hypot(p.x - (a.x + t * dx), p.y - (a.y + t * dy)));
    }
    return d;
}

/// Parámetros de los trabajos de láser del usuario (avance 500, potencia 300, `GRBL_laser`, orden R).
GeometryJobSettings laserSettings(double cutZ, bool endAtOrigin, GcodeFlavor flavor) {
    GeometryJobSettings s;
    s.preprocessor = "GRBL_laser";
    s.order = DrillOrder::RTree;
    s.toolchange = false;
    s.flavor = flavor;
    GeometryToolSettings& t = s.toolSettings;
    t.toolDia = 0.1;
    t.cutZ = cutZ;
    t.depthPerPass = 0.8;
    t.travelZ = 2.0;
    t.feedrate = 500.0;
    t.feedrateZ = 60.0;
    t.feedrateRapid = 1500.0;
    t.spindleSpeed = 300.0;
    t.endZ = 15.0;
    t.toolchangeZ = 15.0;
    t.toolchangeXY = std::array<double, 2>{0.0, 0.0};
    if (endAtOrigin) t.endXY = std::array<double, 2>{0.0, 0.0};
    t.tolerance = 0.00025;  // global_tolerance / 20
    return s;
}

/// Hasta la primera trayectoria (la cabecera y el comienzo del trabajo).
std::vector<std::string> preamble(const std::vector<std::string>& L) {
    std::vector<std::string> out;
    for (const std::string& l : L) {
        if (l.rfind("G00 X", 0) == 0) break;
        out.push_back(l);
    }
    return out;
}

}  // namespace

TEST(LuzDeNoche, CutoutFromGerberIsByteForByteTheRelease) {
    // Edge_Cuts -> CutOut libre (fresa 1,2, margen 0, 4 puentes de 0,6) -> G-code `default`. Idéntico al de la 2024.4.
    Gerber g;
    ASSERT_TRUE(g.parseFile(kDir + "Edge_Cuts.gbr"));
    g.mirror(Axis::Y, {0, 0});
    CutoutOptions o;
    o.toolDia = 1.2;
    o.margin = 0.0;
    o.gaps = CutoutGaps::Four;
    o.gapSize = 0.6;
    CutoutResult r;
    std::string error;
    ASSERT_TRUE(CutOut::freeform(g.solidGeometry(), true, o, r, error)) << error;
    const ShapeList lines = r.solidGeometry.parts().empty() ? ShapeList{r.solidGeometry} : r.solidGeometry.parts();

    GeometryJobSettings s;
    s.order = DrillOrder::RTree;
    s.flavor = GcodeFlavor::FlatCAM2024_4Release;
    GeometryToolSettings& t = s.toolSettings;
    t.toolDia = 1.2;
    t.cutZ = -1.8;
    t.depthPerPass = 0.6;
    t.travelZ = 2.0;
    t.feedrate = 20.0;
    t.feedrateZ = 60.0;
    t.feedrateRapid = 1500.0;
    t.spindleSpeed = 10000.0;
    t.dwellTime = 1.0;
    t.endZ = 19.0;
    t.toolchangeZ = 19.0;
    t.endXY = std::array<double, 2>{0.0, 0.0};
    t.toolchangeXY = std::array<double, 2>{0.0, 0.0};
    t.tolerance = 0.00025;  // sin la simplificación quedan 16 vértices de más por esquina
    CncJob job;
    ASSERT_TRUE(job.generateFromGeometry(lines, s)) << job.lastError();

    const std::vector<std::string> real = splitLines(readFile(kDir + "Edge_Cuts.gbr_cutout_cnc.nc"));
    ASSERT_GT(real.size(), 100u);
    EXPECT_EQ(splitLines(job.exportGcode(headerOf(real))), real);
}

TEST(LuzDeNoche, DrillingHasTheSameHolesAndAShorterRoute) {
    // PTH.drl reflejado en X -> taladrado con cambio de herramienta. La 2024.4 ordenó con OR-Tools; el port usa su
    // optimizador: mismos agujeros, mismas profundidades, mismas líneas fijas y un recorrido que no es más largo.
    ExcellonOptions eo;
    eo.zeros = "T";
    eo.excellonUnits = "METRIC";
    Excellon ex(eo);
    ASSERT_TRUE(ex.parseFile(kDir + "PTH.drl"));
    ex.mirror(Axis::Y, {0, 0});
    DrillJobSettings s;
    s.order = DrillOrder::Optimized;
    for (int i = 1; i <= 4; ++i) {
        DrillToolSettings t;
        // Así figuran en el proyecto (la interfaz guardó el resultado de una resta en coma flotante).
        t.cutZ = PyNumber(i == 4 ? -3.400000000000001 : -2.4000000000000004);
        t.feedrateZ = PyNumber(10.0);
        t.feedrateRapid = PyNumber(1500);
        t.travelZ = PyNumber(2);
        t.depthPerPass = PyNumber(0.7);
        t.offsetZ = PyNumber(0.0);
        t.dwellTime = PyNumber(1);
        t.toolchangeZ = PyNumber(19.0);
        t.endZ = PyNumber(19.0);
        t.spindleSpeed = PyNumber(10000);
        t.toolchangeXY = std::array<double, 2>{0.0, 0.0};
        t.endXY = std::array<double, 2>{0.0, 0.0};
        s.tools[i] = t;
    }
    CncJob job(s);
    ASSERT_TRUE(job.generateFromExcellon(ex)) << job.lastError();
    const std::vector<std::string> real = splitLines(readFile(kDir + "PTH.drl_cnc.nc"));
    const std::vector<std::string> mine = splitLines(job.exportGcode(headerOf(real)));
    ASSERT_EQ(mine.size(), real.size());

    // Cada agujero son 4 líneas (`G00 X.. Y..`, `G01 Z-..`, `G01 Z0`, `G00 Z..`); lo demás tiene que ser idéntico.
    auto split = [](const std::vector<std::string>& L, std::vector<std::string>& holes, std::vector<std::string>& fixed,
                    double& route) {
        double x0 = 0, y0 = 0;
        for (std::size_t i = 0; i < L.size(); ++i) {
            double x = 0, y = 0;
            if (std::sscanf(L[i].c_str(), "G00 X%lf Y%lf", &x, &y) == 2) {
                route += std::hypot(x - x0, y - y0);
                x0 = x;
                y0 = y;
            }
            if (L[i].rfind("G00 X", 0) == 0 && i + 1 < L.size() && L[i + 1].rfind("G01 Z-", 0) == 0) {
                holes.push_back(L[i] + "|" + L[i + 1]);
                ++i;
            } else {
                fixed.push_back(L[i]);
            }
        }
        std::sort(holes.begin(), holes.end());
    };
    std::vector<std::string> holesReal, holesMine, fixedReal, fixedMine;
    double routeReal = 0, routeMine = 0;
    split(real, holesReal, fixedReal, routeReal);
    split(mine, holesMine, fixedMine, routeMine);
    EXPECT_EQ(holesMine.size(), 120u);
    EXPECT_EQ(holesMine, holesReal);
    EXPECT_EQ(fixedMine, fixedReal);
    EXPECT_LE(routeMine, routeReal) << "recorrido en vacío (mm)";  // medido: 828,6 contra 1.323,1
}

TEST(LuzDeNoche, LogoPaintMatchesTheRelease) {
    // Logo reflejado en X -> Paint estándar (0,1 mm, solape 80 %, conectar) -> G-code de láser.
    // Medido: con GEOS 3.11 (la de Shapely 1.8) el archivo sale idéntico; con 3.12 y 3.15, 17 líneas cambian en las
    // últimas cifras (mismos trazados y misma cantidad de puntos).
    Gerber g;
    ASSERT_TRUE(g.parseFile(kDir + "Logo.gbr"));
    g.mirror(Axis::Y, {0, 0});
    PaintOptions o;
    o.toolDia = 0.1;
    o.overlap = 0.8;
    o.connect = true;
    ShapeList paths;
    std::string error;
    ASSERT_TRUE(Paint::paintAll(g.solidGeometry(), o, paths, error)) << error;
    CncJob job;
    ASSERT_TRUE(job.generateFromGeometry(paths, laserSettings(-0.05, false, GcodeFlavor::FlatCAM2024_4Release)))
        << job.lastError();

    const std::vector<std::string> real = splitLines(readFile(kDir + "Logo.gbr_mt_paint_cnc.nc"));
    const std::vector<std::string> mine = splitLines(job.exportGcode(headerOf(real)));
    ASSERT_EQ(mine.size(), real.size());
    EXPECT_EQ(preamble(mine), preamble(real));
    std::size_t differing = 0;
    for (std::size_t i = 0; i < real.size(); ++i) differing += mine[i] != real[i];
    EXPECT_LE(differing, real.size() / 200) << "líneas distintas";

    const auto a = laserPaths(mine), b = laserPaths(real);
    ASSERT_EQ(a.size(), b.size());
    EXPECT_EQ(a.size(), 72u);
    double worst = 0;
    for (std::size_t i = 0; i < a.size(); ++i) {
        ASSERT_EQ(a[i].size(), b[i].size()) << "puntos del trazado " << i;
        for (const Point2D& p : a[i]) worst = std::max(worst, distanceToPolyline(p, b[i]));
        for (const Point2D& p : b[i]) worst = std::max(worst, distanceToPolyline(p, a[i]));
    }
    EXPECT_LT(worst, 0.001) << "mm entre trazados";
}

TEST(LuzDeNoche, BottomCopperNccMatchesTheUsersFile) {
    // B_Cu reflejado en X -> NCC "Sí mismo" (0,1 mm, solape 80 %, margen 0, conectar, contorno) -> G-code de láser.
    // El usuario editó este archivo a mano: agregó `G00 Z0` (y por error escribió `G21f`) y quitó el `G00 Z15.00` final,
    // que es justo la variante 8.994 del port (la predeterminada). Medido: con GEOS 3.11 y 3.12 salen 272 trazados como
    // los suyos y solo cambian empates de `rtree.nearest`; con GEOS 3.15, 275 trazados y 0,07 % menos puntos.
    Gerber g;
    ASSERT_TRUE(g.parseFile(kDir + "B_Cu.gbr"));
    g.mirror(Axis::Y, {0, 0});
    NccOptions o;
    o.toolDia = 0.1;
    o.overlap = 0.8;
    o.connect = true;
    o.margin = 0.0;
    ShapeList paths;
    std::string error;
    ASSERT_TRUE(Ncc::clearCopper(g.solidGeometry(), o, paths, error)) << error;
    CncJob job;
    ASSERT_TRUE(job.generateFromGeometry(paths, laserSettings(-2.4, true, GcodeFlavor::FlatCAM8_994))) << job.lastError();

    std::vector<std::string> real = splitLines(readFile(kDir + "B_Cu.gbr_ncc_cnc.nc"));
    const std::vector<std::string> mine = splitLines(job.exportGcode(headerOf(real)));
    auto typo = std::find(real.begin(), real.end(), "G21f");
    ASSERT_NE(typo, real.end());
    *typo = "G21";  // la errata del usuario
    EXPECT_EQ(preamble(mine), preamble(real));
    ASSERT_GE(mine.size(), 7u);  // el cierre, sin el último punto (que con GEOS 3.15 es otro)
    EXPECT_EQ(std::vector<std::string>(mine.end() - 7, mine.end()), std::vector<std::string>(real.end() - 7, real.end()))
        << "cierre del archivo";

    const auto a = laserPaths(mine), b = laserPaths(real);
    std::size_t pa = 0, pb = 0;
    double la = 0, lb = 0;
    for (const auto& p : a) {
        pa += p.size();
        la += pathLength(p);
    }
    for (const auto& p : b) {
        pb += p.size();
        lb += pathLength(p);
    }
    EXPECT_NEAR(static_cast<double>(a.size()), static_cast<double>(b.size()), b.size() * 0.02) << "trazados";
    EXPECT_NEAR(static_cast<double>(pa), static_cast<double>(pb), pb * 0.002) << "puntos";
    EXPECT_NEAR(la, lb, lb * 0.002) << "longitud total (mm)";
}
