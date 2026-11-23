// Errores del original encontrados en la auditoría de la 1.0.0 (sección 14): cada test los prueba con el interruptor
// apagado (la aplicación) y, donde se puede comparar, que con el interruptor encendido siga como el original.
#include <gtest/gtest.h>

#include <regex>
#include <sstream>
#include <string>
#include <vector>

#include "cam/Autolevel.h"
#include "cam/CncJob.h"
#include "core/OriginalBugs.h"
#include "core/Shape.h"
#include "io/FileExport.h"
#include "io/SvgExport.h"
#include "parsers/Excellon.h"
#include "tools/Film.h"
#include "parsers/Gerber.h"

using namespace flatcam;

namespace {

/// Las Z de las bajadas `G01 Z...` de un G-code.
std::vector<std::string> plungeDepths(const std::string& gcode) {
    std::vector<std::string> out;
    std::istringstream in(gcode);
    std::string line;
    static const std::regex re(R"(^G01 Z(-[0-9.]+)$)");
    std::smatch m;
    while (std::getline(in, line))
        if (std::regex_match(line, m, re)) out.push_back(m[1]);
    return out;
}

/// Taladra dos agujeros con profundidad múltiple.
std::string drill(double cutZ, double perPass) {
    ExcellonOptions opt;
    opt.zeros = "T";
    opt.excellonUnits = "METRIC";
    Excellon ex(opt);
    EXPECT_TRUE(ex.parseLines({"M48", "METRIC", "T1C1.0", "%", "T1", "X10.0Y10.0", "X20.0Y10.0", "M30"}));
    DrillJobSettings s;
    s.order = DrillOrder::FileOrder;
    DrillToolSettings t;
    t.cutZ = PyNumber(cutZ);
    t.feedrateZ = PyNumber(100.0);
    t.feedrateRapid = PyNumber(1000.0);
    t.travelZ = PyNumber(2.0);
    t.depthPerPass = PyNumber(perPass);
    t.offsetZ = PyNumber(0.0);
    t.multidepth = true;
    t.toolchangeZ = PyNumber(15.0);
    t.endZ = PyNumber(10.0);
    t.spindleSpeed = PyNumber(1000.0);
    s.tools[1] = t;
    CncJob job(s);
    EXPECT_TRUE(job.generateFromExcellon(ex)) << job.lastError();
    return job.gcode();
}

/// Un trazado abierto y uno cerrado con profundidad múltiple (-1 en pasadas de 0,4) y el postprocesador dado.
std::string geometry(const std::string& pp, double perPass = 0.4) {
    GeometryJobSettings js;
    js.preprocessor = pp;
    js.order = DrillOrder::FileOrder;
    auto& t = js.toolSettings;
    t.toolDia = 1;
    t.cutZ = -1.0;
    t.multidepth = true;
    t.depthPerPass = perPass;
    t.travelZ = 2;
    t.feedrate = 100;
    t.feedrateZ = 50;
    t.feedrateRapid = 1000;
    t.spindleSpeed = 1000;
    t.endZ = 10;
    t.toolchangeZ = 15;
    const ShapeList g{Shape::lineString({{0, 0}, {10, 0}, {10, 10}}),
                      Shape::lineString({{20, 0}, {30, 0}, {30, 10}, {20, 10}, {20, 0}})};
    CncJob job;
    EXPECT_TRUE(job.generateFromGeometry(g, js)) << job.lastError();
    return job.gcode();
}

}  // namespace

TEST(DrillMultiDepthFixed, LastPassIsTheCutDepthWhenItIsAMultipleOfTheStep) {
    // El original resta en coma flotante: -2,1 en pasadas de 0,7 llega a -2,0999... y hace otra pasada a -2,8.
    {
        OriginalBugsScope scope(true);
        const auto z = plungeDepths(drill(-2.1, 0.7));
        ASSERT_EQ(z.size(), 8u);
        EXPECT_EQ(z[3], "-2.8000");
    }
    OriginalBugsScope scope(false);
    for (const auto& [cut, step, expected] : std::vector<std::tuple<double, double, std::vector<std::string>>>{
             {-2.1, 0.7, {"-0.7000", "-1.4000", "-2.1000"}},
             {-1.8, 0.6, {"-0.6000", "-1.2000", "-1.8000"}},
             {-1.7, 0.6, {"-0.6000", "-1.2000", "-1.7000"}},
             {-3.6, 1.2, {"-1.2000", "-2.4000", "-3.6000"}}}) {
        const auto z = plungeDepths(drill(cut, step));
        ASSERT_EQ(z.size(), 2 * expected.size()) << cut << " / " << step;
        for (std::size_t i = 0; i < expected.size(); ++i) EXPECT_EQ(z[i], expected[i]) << cut << " / " << step;
    }
}

TEST(DrillMultiDepthFixed, ZeroStepIsASinglePassInsteadOfAnEndlessLoop) {
    // El original queda en un bucle infinito (z -= 0); en los dos modos es una sola pasada.
    for (bool original : {true, false}) {
        OriginalBugsScope scope(original);
        const auto z = plungeDepths(drill(-1.5, 0.0));
        ASSERT_EQ(z.size(), 2u);
        EXPECT_EQ(z[0], "-1.5000");
    }
}

TEST(GeometryMultiDepthFixed, FinalLiftIsWhereTheToolIs) {
    // line_xyz sube con X,Y: el original usaba las del trazado anterior (un rápido en diagonal con la fresa abajo).
    // Cuenta las subidas desde la profundidad final con X,Y distintas de las del último corte.
    auto diagonalLifts = [](const std::string& g) {
        static const std::regex xy(R"(X(-?[0-9.]+) Y(-?[0-9.]+))");
        std::istringstream in(g);
        std::string line, prev;
        int lifts = 0, diagonal = 0;
        std::smatch a, b;
        while (std::getline(in, line)) {
            if (line.rfind("G00", 0) == 0 && line.find("Z2.0000") != std::string::npos &&
                prev.find("Z-1.0000") != std::string::npos && std::regex_search(prev, a, xy) &&
                std::regex_search(line, b, xy)) {
                ++lifts;
                if (a[0] != b[0]) ++diagonal;
            }
            prev = line;
        }
        return std::pair{lifts, diagonal};
    };
    {
        OriginalBugsScope scope(true);
        const auto [lifts, diagonal] = diagonalLifts(geometry("line_xyz"));
        EXPECT_GT(lifts, 0);
        EXPECT_EQ(diagonal, 2);  // una por trazado
    }
    OriginalBugsScope scope(false);
    const auto [lifts, diagonal] = diagonalLifts(geometry("line_xyz"));
    EXPECT_GE(lifts, 2);
    EXPECT_EQ(diagonal, 0);
}

TEST(GeometryMultiDepthFixed, ZeroStepCutsAtTheCutDepth) {
    // Con 0 por pasada el original corta una sola vez en Z 0 (sobre la pieza).
    {
        OriginalBugsScope scope(true);
        EXPECT_NE(geometry("default", 0.0).find("G01 Z0.0000"), std::string::npos);
    }
    OriginalBugsScope scope(false);
    const std::string g = geometry("default", 0.0);
    EXPECT_EQ(g.find("G01 Z0.0000"), std::string::npos);
    EXPECT_NE(g.find("G01 Z-1.0000"), std::string::npos);
}

TEST(RolandFixed, InchCoordinatesAreConvertedToMachineUnits) {
    // La Roland trabaja en unidades de 0,025 mm: el original dividía las pulgadas por 25,4 en vez de multiplicarlas.
    GeometryJobSettings js;
    js.units = "IN";
    js.preprocessor = "Roland_MDX_20";
    js.order = DrillOrder::FileOrder;
    auto& t = js.toolSettings;
    t.toolDia = 0.04;
    t.cutZ = -0.02;
    t.travelZ = 0.1;
    t.feedrate = 10;  // pulgadas/min = 254 mm/min = 4,2 mm/s
    t.feedrateZ = 10;
    t.feedrateRapid = 100;
    t.spindleSpeed = 1000;
    t.endZ = 1;
    t.toolchangeZ = 1;
    const ShapeList g{Shape::lineString({{0, 0}, {1, 0}})};
    auto run = [&] {
        CncJob job;
        EXPECT_TRUE(job.generateFromGeometry(g, js)) << job.lastError();
        return job.gcode();
    };
    {
        OriginalBugsScope scope(true);
        EXPECT_NE(run().find("Z1.6,0.0,"), std::string::npos);  // 1" / 25,4 * 40
    }
    OriginalBugsScope scope(false);
    const std::string g2 = run();
    EXPECT_NE(g2.find("Z1016.0,0.0,-20.32"), std::string::npos) << g2;  // 1" = 25,4 mm = 1016 unidades
    EXPECT_NE(g2.find("V4.2;"), std::string::npos) << g2;
}

TEST(ZChecksFixed, TravelZeroFailsAndSignsAreCorrected) {
    // El original controla las Z antes de leerlas de la herramienta (no tiene efecto): rápidos a la Z 0 de la pieza.
    OriginalBugsScope scope(false);
    GeometryJobSettings js;
    js.order = DrillOrder::FileOrder;
    auto& t = js.toolSettings;
    t.toolDia = 1;
    t.cutZ = -1.0;
    t.travelZ = 0;
    t.feedrate = 100;
    t.feedrateZ = 50;
    t.feedrateRapid = 1000;
    t.endZ = 10;
    t.toolchangeZ = 15;
    const ShapeList g{Shape::lineString({{0, 0}, {10, 0}}), Shape::lineString({{20, 0}, {30, 0}})};
    {
        CncJob job;
        EXPECT_FALSE(job.generateFromGeometry(g, js));
        EXPECT_NE(job.lastError().find("Z Travel parameter is zero"), std::string::npos);
    }
    t.travelZ = -2;
    t.cutZ = 1;
    {
        CncJob job;
        ASSERT_TRUE(job.generateFromGeometry(g, js)) << job.lastError();
        EXPECT_NE(job.gcode().find("G01 Z-1.0000"), std::string::npos);
        EXPECT_NE(job.gcode().find("G00 Z2.0000"), std::string::npos);
        EXPECT_EQ(job.gcode().find("Z-2.0000"), std::string::npos);
    }
    // Con "machinist" se respeta lo pedido.
    js.machinist = true;
    CncJob job;
    ASSERT_TRUE(job.generateFromGeometry(g, js)) << job.lastError();
    EXPECT_NE(job.gcode().find("G00 Z-2.0000"), std::string::npos);
    // Taladrado: la Z de viaje 0 falla.
    ExcellonOptions opt;
    Excellon ex(opt);
    ASSERT_TRUE(ex.parseLines({"M48", "METRIC", "T1C1.0", "%", "T1", "X10.0Y10.0", "M30"}));
    DrillJobSettings ds;
    DrillToolSettings dt;
    dt.cutZ = PyNumber(-1.5);
    dt.travelZ = PyNumber(0.0);
    dt.feedrateZ = PyNumber(100.0);
    dt.feedrateRapid = PyNumber(1000.0);
    dt.toolchangeZ = PyNumber(15.0);
    dt.endZ = PyNumber(10.0);
    ds.tools[1] = dt;
    CncJob drillJob(ds);
    EXPECT_FALSE(drillJob.generateFromExcellon(ex));
    EXPECT_NE(drillJob.lastError().find("Z Travel parameter is zero"), std::string::npos);
}

TEST(GerberTrailingZerosFixed, NegativeNumbersCountOnlyTheirDigits) {
    // Formato 2.4 con ceros finales omitidos: "1" es 10 y "-1" tiene que ser -10 (el original contaba el signo).
    {
        OriginalBugsScope scope(true);
        EXPECT_DOUBLE_EQ(Gerber::parseGerberNumber("-1", 2, 4, 'T'), -1.0);
    }
    OriginalBugsScope scope(false);
    EXPECT_DOUBLE_EQ(Gerber::parseGerberNumber("1", 2, 4, 'T'), 10.0);
    EXPECT_DOUBLE_EQ(Gerber::parseGerberNumber("-1", 2, 4, 'T'), -10.0);
    EXPECT_DOUBLE_EQ(Gerber::parseGerberNumber("-15", 2, 4, 'T'), -15.0);
    EXPECT_NEAR(Gerber::parseGerberNumber("-123456", 2, 4, 'T'), -12.3456, 1e-12);
    EXPECT_NEAR(Gerber::parseGerberNumber("+123456", 2, 4, 'T'), 12.3456, 1e-12);
    EXPECT_NEAR(Gerber::parseGerberNumber("-123456", 2, 4, 'L'), -12.3456, 1e-12);
}

TEST(AutolevelFixed, LinuxCncNoteIsAComment) {
    // El original escribe el aviso de LinuxCNC sin paréntesis (texto que el controlador rechaza).
    ProbingGcodeParams p;
    p.controller = "LinuxCNC";
    const std::vector<Point2D> pts{{0, 0}, {10, 0}};
    {
        OriginalBugsScope scope(true);
        EXPECT_NE(autolevelProbingGcode(pts, p).find("\nThe file with"), std::string::npos);
    }
    OriginalBugsScope scope(false);
    const std::string g = autolevelProbingGcode(pts, p);
    std::istringstream in(g);
    std::string line;
    while (std::getline(in, line))
        if (line.find("probing points") != std::string::npos || line.find("LinuxCNC") != std::string::npos ||
            line.find("name of the file") != std::string::npos)
            EXPECT_TRUE(line.front() == '(' && line.back() == ')') << line;
}

TEST(GerberStepRepeatFixed, BlocksAreRepeatedWithTheirPolarity) {
    // %SR no está en el original: deja una sola copia.
    const std::vector<std::string> lines{"%FSLAX24Y24*%", "%MOMM*%", "%ADD10C,0.5*%", "%ADD11R,4X4*%", "D11*",
                                         "X0Y0D03*", "%SRX3Y2I10J5*%", "X0Y200000D03*", "%SR*%", "M02*"};
    auto box = [](const Gerber& g) {
        BoundingBox b{1e9, 1e9, -1e9, -1e9};
        for (const Shape& s : g.solidGeometry())
            if (auto sb = s.bounds()) b = b.merged(*sb);
        return b;
    };
    {
        OriginalBugsScope scope(true);
        Gerber g;
        ASSERT_TRUE(g.parseLines(lines));
        EXPECT_NEAR(box(g).xmax, 2.0, 1e-6);
    }
    OriginalBugsScope scope(false);
    Gerber g;
    ASSERT_TRUE(g.parseLines(lines));
    const BoundingBox b = box(g);
    EXPECT_NEAR(b.xmin, -2.0, 1e-6);
    EXPECT_NEAR(b.xmax, 22.0, 1e-6);  // 3 columnas cada 10
    EXPECT_NEAR(b.ymax, 27.0, 1e-6);  // 2 filas cada 5
    double area = 0;
    for (const Shape& s : g.solidGeometry()) area += s.area();
    EXPECT_NEAR(area, 7 * 16.0, 1e-4);  // el pad del origen y las 6 copias
    EXPECT_EQ(g.apertures().at("11").geometry.size(), 7u);
    EXPECT_EQ(g.followGeometry().size(), 7u);

    // Polaridad clara dentro del bloque: cada copia borra su parte (sin cerrar el bloque: se cierra al final).
    Gerber h;
    ASSERT_TRUE(h.parseLines({"%FSLAX24Y24*%", "%MOMM*%", "%ADD10C,1*%", "%ADD11R,30X4*%", "D11*", "X100000Y0D03*",
                              "%SRX3Y1I10J0*%", "%LPC*%", "D10*", "X0Y0D03*", "%LPD*%", "M02*"}));
    double ha = 0;
    for (const Shape& s : h.solidGeometry()) ha += s.area();
    EXPECT_NEAR(ha, 120.0 - 3 * 0.785, 0.01);  // el rectángulo menos tres círculos de diámetro 1
}

TEST(ExportUnits, ObjectsInInchesAreScaledLikeTheOriginal) {
    // Con la aplicación en pulgadas los objetos están en pulgadas: el original multiplica por 25,4 al exportar en mm
    // (el port suponía siempre mm). Vale en los dos modos (era un error del port).
    ExcellonOptions opt;
    Excellon ex(opt);
    ASSERT_TRUE(ex.parseLines({"M48", "METRIC", "T1C1.0", "%", "T1", "X1.0Y2.0", "M30"}));  // tomado como pulgadas
    ExcellonExportFormat f;
    f.units = "METRIC";
    f.appUnits = "IN";
    f.format = "dec";
    const std::string text = exportExcellon(ex, f, {});
    EXPECT_NE(text.find("X25.4000Y50.8000"), std::string::npos) << text;
    f.units = "INCH";
    EXPECT_NE(exportExcellon(ex, f, {}).find("X1.0000Y2.0000"), std::string::npos);
    Gerber g;
    ASSERT_TRUE(g.parseLines({"%FSLAX24Y24*%", "%MOIN*%", "%ADD10C,0.1*%", "D10*", "X10000Y20000D03*", "M02*"}));
    GerberExportFormat gf;
    gf.units = "MM";
    gf.appUnits = "IN";
    const std::string gt = exportGerber(g, gf, {});
    EXPECT_NE(gt.find("X254000Y508000D03"), std::string::npos) << gt;
}

TEST(ExportFixed, NegativeCoordinatesWithLeadingZeros) {
    // Excellon LZ (ceros a la izquierda) y Gerber con ceros finales omitidos ("T"): el signo no es un dígito.
    ExcellonOptions opt;
    Excellon ex(opt);
    ASSERT_TRUE(ex.parseLines({"M48", "METRIC", "T1C1.0", "%", "T1", "X-1.5Y2.25", "M30"}));
    ExcellonExportFormat f;
    f.units = "METRIC";
    f.format = "ndec";
    f.zeros = "LZ";
    f.whole = 3;
    f.fract = 3;
    {
        OriginalBugsScope scope(true);
        EXPECT_NE(exportExcellon(ex, f, {}).find("X0-1500"), std::string::npos);
    }
    OriginalBugsScope scope(false);
    const std::string text = exportExcellon(ex, f, {});
    EXPECT_NE(text.find("X-001500Y002250"), std::string::npos) << text;
    // Y se vuelve a leer igual.
    Excellon back(opt);
    ASSERT_TRUE(back.parseLines([&] {
        std::vector<std::string> v;
        std::istringstream in(text);
        for (std::string l; std::getline(in, l);) v.push_back(l);
        return v;
    }()));
    ASSERT_FALSE(back.drillTools().empty());
    const auto& d = back.drillTools().begin()->second.drills;
    ASSERT_EQ(d.size(), 1u);
    EXPECT_NEAR(d[0].x, -1.5, 1e-9);
    EXPECT_NEAR(d[0].y, 2.25, 1e-9);
}

TEST(FilmFixed, MirrorAndSkewUseTheBoxAsReference) {
    // Un cuadrado chico en una esquina de la caja: reflejado en X tiene que ir a la otra esquina de la caja, no
    // quedarse en su lugar (el original lo refleja alrededor de su propio centro).
    const ShapeList obj{Shape::box(1, 1, 2, 2)};
    const ShapeList box{Shape::box(0, 0, 10, 10)};
    FilmOptions o;
    o.negative = false;
    o.mirror = "y";
    auto svgOf = [&] { return filmSvg(obj, box, BoundingBox{0, 0, 10, 10}, "mm", o).svg; };
    {
        OriginalBugsScope scope(true);
        EXPECT_EQ(svgOf().find("9.0,"), std::string::npos);
    }
    OriginalBugsScope scope(false);
    const std::string s = svgOf();
    EXPECT_NE(s.find("9.0,"), std::string::npos) << s;
    EXPECT_NE(s.find("8.0,"), std::string::npos) << s;
}

TEST(CncSvgFixed, EveryToolKeepsItsStrokeWidth) {
    CncSvgTool a, b;
    a.cuts = {Shape::lineString({{0, 0}, {10, 0}})};
    a.stroke = 0.25;
    b.cuts = {Shape::lineString({{0, 5}, {10, 5}})};
    b.travels = {Shape::lineString({{10, 0}, {0, 5}})};
    b.stroke = 0.6;
    const std::string s = exportCncSvg({a, b}, BoundingBox{0, 0, 10, 5}, "mm");
    // Shapely escribe stroke-width = 2 * factor.
    EXPECT_NE(s.find("stroke-width=\"0.5\""), std::string::npos) << s;
    EXPECT_NE(s.find("stroke-width=\"1.2\""), std::string::npos) << s;
    // Los viajes van debajo de todos los cortes.
    EXPECT_LT(s.find("#F0E24D"), s.find("#5E6CFF"));
}
