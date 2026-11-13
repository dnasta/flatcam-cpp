// Film contra el original (tests/tools/oracle_film.py).
#include <gtest/gtest.h>

#include <cmath>
#include <cstdlib>
#include <map>

#include "golden_film.h"
#include "parsers/Excellon.h"
#include "parsers/Gerber.h"
#include "tools/Film.h"

using namespace flatcam;

namespace {

std::string data(const std::string& f) { return std::string(FLATCAM_TEST_DATA_DIR) + "/" + f; }

ShapeList flat(const ShapeList& l) {
    ShapeList out;
    for (const Shape& s : l) {
        if (s.isEmpty()) continue;
        if (s.type() == ShapeType::Polygon || s.type() == ShapeType::LineString || s.type() == ShapeType::LinearRing)
            out.push_back(s);
        else
            for (const Shape& p : s.parts()) out.push_back(p);
    }
    return out;
}

BoundingBox boundsOf(const ShapeList& l) { return Shape::unaryUnion(l).bounds().value_or(BoundingBox{}); }

unsigned long long fnv(const std::string& s) {
    unsigned long long h = 0xcbf29ce484222325ULL;
    for (unsigned char c : s) h = (h ^ c) * 0x100000001b3ULL;
    return h;
}

bool exactGeos() { return Shape::geosVersion().rfind("3.11", 0) == 0; }

/// Los textos por fuera de los números iguales y los números a menos de `tol` (sin depender de la versión de GEOS).
::testing::AssertionResult similar(const std::string& a, const std::string& b, double tol) {
    std::size_t i = 0, j = 0;
    auto isNum = [](char c) { return (c >= '0' && c <= '9') || c == '-' || c == '.' || c == 'e'; };
    while (i < a.size() && j < b.size()) {
        if (isNum(a[i]) && isNum(b[j]) && (std::isdigit(static_cast<unsigned char>(a[i])) || a[i] == '-')) {
            char* ea;
            char* eb;
            const double x = std::strtod(a.c_str() + i, &ea), y = std::strtod(b.c_str() + j, &eb);
            if (ea == a.c_str() + i || eb == b.c_str() + j) {
                if (a[i] != b[j]) return ::testing::AssertionFailure() << "distinto en " << i;
                ++i, ++j;
                continue;
            }
            if (std::abs(x - y) > tol) return ::testing::AssertionFailure() << x << " != " << y << " en " << i;
            i = ea - a.c_str();
            j = eb - b.c_str();
            continue;
        }
        if (a[i] != b[j]) return ::testing::AssertionFailure() << "distinto en " << i << ": " << a.substr(i, 40);
        ++i, ++j;
    }
    if (i != a.size() || j != b.size()) return ::testing::AssertionFailure() << "largo distinto";
    return ::testing::AssertionSuccess();
}

/// Sin el contenido de los atributos d="..." (y sin lo que queda cortado al final de un texto parcial).
std::string stripPaths(const std::string& s) {
    std::string out;
    std::size_t i = 0;
    while (true) {
        const std::size_t d = s.find(" d=\"", i);
        if (d == std::string::npos) {
            out += s.substr(i);
            return out;
        }
        out += s.substr(i, d - i) + " d=\"\"";
        const std::size_t e = s.find('"', d + 4);
        if (e == std::string::npos) return out;
        i = e + 1;
    }
}

std::size_t count(const std::string& s, const std::string& what) {
    std::size_t n = 0;
    for (std::size_t i = s.find(what); i != std::string::npos; i = s.find(what, i + 1)) ++n;
    return n;
}

struct Inputs {
    Gerber cu, edge, logo, pads;
    Excellon pth;
    Inputs() {
        cu.parseFile(data("luz_de_noche/luz de noche Pascual-B_Cu.gbr"));
        edge.parseFile(data("luz_de_noche/luz de noche Pascual-Edge_Cuts.gbr"));
        logo.parseFile(data("luz_de_noche/luz de noche Pascual-Logo.gbr"));
        pads.parseFile(data("film/pads.gbr"));
        pth.parseFile(data("luz_de_noche/luz de noche Pascual-PTH.drl"));
    }
};

}  // namespace

TEST(Film, MatchesTheOriginal) {
    Inputs in;
    std::map<std::string, const golden_film::Case*> g;
    for (const auto& c : golden_film::kCases) g[c.name] = &c;
    const ShapeList edge = flat(in.edge.solidGeometry()), cu = flat(in.cu.solidGeometry());
    auto check = [&](const std::string& name, const FilmResult& r) {
        SCOPED_TRACE(name);
        const golden_film::Case& c = *g.at(name);
        EXPECT_EQ(std::string(c.error), "");
        if (exactGeos()) {
            // Con la GEOS del oráculo: los grandes byte por byte; los chicos, con las últimas cifras de las
            // transformaciones (el original y el port redondean distinto a*x + b*y + xoff).
            if (c.svg.empty()) {
                EXPECT_EQ(r.svg.size(), c.length);
                EXPECT_EQ(fnv(r.svg), c.fnv);
            } else {
                EXPECT_TRUE(similar(r.svg, c.svg, 1e-12));
            }
        } else {
            // Otras GEOS empiezan algunos anillos en otro vértice: todo igual salvo los trayectos.
            const std::string want = c.svg.empty() ? c.head : c.svg;
            const std::string got = c.svg.empty() ? r.svg.substr(0, want.size()) : r.svg;
            EXPECT_TRUE(similar(stripPaths(got), stripPaths(want), 1e-6));
            EXPECT_EQ(count(r.svg, "<path"), c.svg.empty() ? count(r.svg, "<path") : count(c.svg, "<path"));
        }
        if (c.translate[0] != 0.0 || c.translate[1] != 0.0) {
            // my_canvas.translate(bounds[0] * mm, bounds[1] * mm), con mm = 72 / 25,4.
            EXPECT_NEAR(r.bounds.xmin * 72.0 / 25.4, c.translate[0], 1e-9);
            EXPECT_NEAR(r.bounds.ymin * 72.0 / 25.4, c.translate[1], 1e-9);
        }
    };
    FilmOptions neg;
    check("neg", filmSvg(edge, edge, boundsOf(edge), "mm", neg));
    check("neg_copper", filmSvg(cu, edge, boundsOf(edge), "mm", neg));
    check("neg_no_box", filmSvg(edge, edge, boundsOf(edge), "mm", neg));
    FilmOptions pos;
    pos.negative = false;
    check("pos", filmSvg(edge, edge, boundsOf(edge), "mm", pos));
    {
        FilmOptions o = neg;
        o.scaleX = 1.02;
        o.scaleY = 0.98;
        o.skewX = 1.0;
        o.skewReference = "bottomleft";
        o.mirror = "x";
        check("neg_transformed", filmSvg(edge, edge, boundsOf(edge), "mm", o));
    }
    {
        FilmOptions o = pos;
        o.skewX = 2.0;
        o.skewY = -1.5;
        o.skewReference = "topright";
        o.strokeFactor = 0.05;
        check("pos_skew_topright", filmSvg(edge, edge, boundsOf(edge), "mm", o));
    }
    {
        FilmOptions o = pos;
        o.scaleY = 1.1;
        o.skewReference = "center";
        o.mirror = "both";
        check("pos_scale_only_y", filmSvg(edge, edge, boundsOf(edge), "mm", o));
    }
    {
        FilmOptions o = neg;
        o.fileType = "png";
        o.screenDpi = 72;
        check("neg_png_72dpi", filmSvg(edge, edge, boundsOf(edge), "mm", o));
    }
    {
        FilmOptions o = pos;
        o.fileType = "pdf";
        check("pos_pdf_a4", filmSvg(edge, edge, boundsOf(edge), "mm", o));
    }
    {
        FilmOptions o = neg;
        o.boundary = 2.5;
        // Una Geometría con el contorno de un cuadrado de 80 mm como caja.
        const ShapeList box = {Shape::polygon({{-40, -40}, {40, -40}, {40, 40}, {-40, 40}}).exterior()};
        check("neg_logo_box_geo", filmSvg(flat(in.logo.solidGeometry()), box, boundsOf(box), "mm", o));
    }
    // Perforado con el Excellon: el Gerber nuevo nace en pulgadas (gerber_def_units) y se convierte a mm (x 25,4).
    {
        const Shape punched = filmPunchExcellon(in.cu.solidGeometry(), in.pth.solidGeometry());
        const Shape mm = punched.transform(AffineTransform::scaling(25.4, 25.4, {0, 0}));
        const golden_film::Case& c = *g.at("pos_punch_exc");
        EXPECT_NEAR(mm.area(), c.area, c.area * 1e-6);
        EXPECT_EQ(static_cast<int>(mm.parts().size()), c.parts);
        const BoundingBox b = *mm.bounds();
        EXPECT_NEAR(b.xmin, c.bounds[0], 1e-6);
        EXPECT_NEAR(b.ymax, c.bounds[3], 1e-6);
        check("pos_punch_exc", filmSvg(flat({mm}), edge, boundsOf(edge), "mm", pos));
    }
    {
        std::string err;
        const auto punched = filmPunchPads(in.pads, 0.3, err);
        ASSERT_TRUE(punched.has_value()) << err;
        const Shape mm = punched->transform(AffineTransform::scaling(25.4, 25.4, {0, 0}));
        const golden_film::Case& c = *g.at("pos_punch_pad");
        EXPECT_NEAR(mm.area(), c.area, c.area * 1e-9);
        EXPECT_EQ(static_cast<int>(mm.parts().size()), c.parts);
        const ShapeList pads = flat(in.pads.solidGeometry());
        check("pos_punch_pad", filmSvg(flat({mm}), pads, boundsOf(pads), "mm", pos));
        EXPECT_FALSE(filmPunchPads(in.pads, 1.0, err).has_value());
        EXPECT_EQ(err, "size");
        EXPECT_NE(std::string(g.at("pos_punch_pad_big")->warning).find("Punch hole size"), std::string::npos);
        EXPECT_FALSE(filmPunchPads(in.cu, 0.3, err).has_value());
        EXPECT_EQ(err, "keyerror");
        EXPECT_EQ(std::string(g.at("pos_punch_pad_regions")->error), "KeyError");
    }
}

#include "golden_print_pdf.h"
#include "io/SvgExport.h"

TEST(PrintPdf, MatchesTheOriginal) {
    // Archivo -> Imprimir (save_pdf): el SVG que se le pasa a svglib.
    Inputs in;
    const ShapeList edge = flat(in.edge.solidGeometry()), pth = flat(in.pth.solidGeometry());
    auto bboxOf = [](std::initializer_list<const ShapeList*> lists) {
        BoundingBox b{1e300, 1e300, -1e300, -1e300};
        for (const ShapeList* l : lists) {
            const BoundingBox o = boundsOf(*l);
            b = {std::min(b.xmin, o.xmin), std::min(b.ymin, o.ymin), std::max(b.xmax, o.xmax), std::max(b.ymax, o.ymax)};
        }
        return b;
    };
    const PrintSvgItem e{edge, "#BBF268", "BF"}, p{pth, "#C40000", "BF"};
    std::map<std::string, std::string> got = {
        {"edge_pth", exportPrintSvg({e, p}, bboxOf({&edge, &pth}), "mm")},
        {"pth", exportPrintSvg({p}, bboxOf({&pth}), "mm")},
        {"edge", exportPrintSvg({e}, bboxOf({&edge}), "mm")}};
    for (const auto& c : golden_print_pdf::kCases) {
        SCOPED_TRACE(c.name);
        const std::string& svg = got.at(c.name);
        if (exactGeos()) EXPECT_TRUE(similar(svg, c.svg, 1e-12));
        else EXPECT_TRUE(similar(stripPaths(svg), stripPaths(c.svg), 1e-6));
    }
}
