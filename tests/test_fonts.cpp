/**
 * @file test_fonts.cpp
 * @brief Textos con fuentes (FontParser) contra el ParseFont original (tests/tools/oracle_fonts.py).
 */
#include <gtest/gtest.h>

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <sstream>

#include "io/JsonValue.h"
#include "io/ProjectFile.h"
#include "parsers/FontParser.h"

using namespace flatcam;

namespace {

const std::string kDir = std::string(FLATCAM_TEST_DATA_DIR) + "/fonts";

JsonValue oracle() {
    std::ifstream f(kDir + "/oracle.json", std::ios::binary);
    std::stringstream ss;
    ss << f.rdbuf();
    JsonValue v;
    std::string err;
    EXPECT_TRUE(ProjectFile::parse(ss.str(), v, err)) << err;
    return v;
}

std::string base(const std::string& p) { return std::filesystem::path(p).filename().string(); }

void expectDict(const std::map<std::string, std::string>& port, const JsonValue& orig) {
    std::map<std::string, std::string> o;
    for (const auto& [k, v] : orig.members()) o[k] = v.text();
    std::map<std::string, std::string> p;
    for (const auto& [k, v] : port) p[k] = base(v);
    EXPECT_EQ(p, o);
}

}  // namespace

TEST(Fonts, NamesAndTypesLikeTheOriginal) {
    const JsonValue o = oracle();
    std::vector<std::string> files = FontParser::fontFiles({kDir});
    std::vector<std::string> names;
    for (const auto& f : files) names.push_back(base(f));
    std::sort(names.begin(), names.end());
    std::vector<std::string> expected;
    for (const JsonValue& f : o["files"].items()) expected.push_back(f.text());
    EXPECT_EQ(names, expected);
    for (const auto& [file, nf] : o["names"].members()) {
        const auto [n, fam] = FontParser::fontName(kDir + "/" + file);
        EXPECT_EQ(n, nf.items()[0].text()) << file;
        EXPECT_EQ(fam, nf.items()[1].text()) << file;
    }
    FontParser fp;
    fp.loadFonts(files);
    expectDict(fp.regular, o["regular"]);
    expectDict(fp.bold, o["bold"]);
    expectDict(fp.italic, o["italic"]);
    expectDict(fp.boldItalic, o["bi"]);
}

TEST(Fonts, TextToGeometryLikeTheOriginal) {
    if (!FontParser::available()) GTEST_SKIP() << "compilado sin FreeType";
    const JsonValue o = oracle();
    FontParser fp;
    fp.loadFonts(FontParser::fontFiles({kDir}));
    for (const JsonValue& c : o["cases"].items()) {
        SCOPED_TRACE(c["text"].text() + " / " + c["font"].text() + " / " + c["type"].text());
        const auto r = fp.fontToGeometry(c["text"].text(), c["font"].text(), c["type"].text(),
                                         std::stoi(c["size"].text()), c["units"].text() == "MM", c["x"].toDouble(),
                                         c["y"].toDouble());
        if (c.find("result")) {
            EXPECT_FALSE(r.has_value());
            continue;
        }
        ASSERT_TRUE(r.has_value());
        const auto& polys = c["polygons"].items();
        ASSERT_EQ(r->size(), polys.size());
        for (std::size_t i = 0; i < polys.size(); ++i) {
            const auto pts = (*r)[i].exterior().coordinates();
            ASSERT_EQ(pts.size(), polys[i].items().size()) << "polígono " << i;
            for (std::size_t k = 0; k < pts.size(); ++k) {
                EXPECT_NEAR(pts[k].x, polys[i].items()[k].items()[0].toDouble(), 1e-12) << i << " " << k;
                EXPECT_NEAR(pts[k].y, polys[i].items()[k].items()[1].toDouble(), 1e-12) << i << " " << k;
            }
        }
    }
}

// ---------------------------------------------------------------- errores del original corregidos (sección 14)
#include "core/OriginalBugs.h"

TEST(FontsFixed, CurvesHolesKerningObliqueAndSizes) {
    if (!FontParser::available()) GTEST_SKIP() << "sin FreeType";
    auto load = [] {
        FontParser p;
        p.loadFonts(FontParser::fontFiles({kDir}));
        return p;
    };
    auto area = [](const std::vector<Shape>& l) {
        double a = 0;
        for (const Shape& s : l) a += s.area();
        return a;
    };
    auto width = [](const std::vector<Shape>& l) {
        double lo = 1e9, hi = -1e9;
        for (const Shape& s : l)
            if (auto b = s.bounds()) {
                lo = std::min(lo, b->xmin);
                hi = std::max(hi, b->xmax);
            }
        return hi - lo;
    };
    FontParser orig;
    std::optional<std::vector<Shape>> oO, oAV, oSize;
    {
        OriginalBugsScope o(true);
        orig = load();
        oO = orig.fontToGeometry("o", "DejaVu Sans", "regular", 20, true);
        oAV = orig.fontToGeometry("AV", "DejaVu Sans", "regular", 20, true);
        oSize = orig.fontToGeometry("o", "DejaVu Sans", "regular", 20.7, true);
    }
    OriginalBugsScope fixed(false);
    const FontParser fp = load();
    // La cursiva "Oblique" queda con el nombre de la familia.
    EXPECT_TRUE(fp.italic.count("DejaVu Sans"));
    EXPECT_FALSE(orig.italic.count("DejaVu Sans"));
    // "o": un solo polígono con su agujero, con las curvas aplanadas (más puntos), y menos área que los dos contornos.
    const auto o = fp.fontToGeometry("o", "DejaVu Sans", "regular", 20, true);
    ASSERT_TRUE(o && oO);
    ASSERT_EQ(o->size(), 1u);
    EXPECT_EQ((*o)[0].interiors().size(), 1u);
    EXPECT_EQ(oO->size(), 2u);
    EXPECT_LT(area(*o), area(*oO));
    EXPECT_GT((*o)[0].coordinates().size() + (*o)[0].interiors()[0].coordinates().size(),
              (*oO)[0].coordinates().size() + (*oO)[1].coordinates().size());
    // Kerning de "AV": más angosto que sin kerning (el original lo buscaba entre otras letras).
    const auto av = fp.fontToGeometry("AV", "DejaVu Sans", "regular", 20, true);
    ASSERT_TRUE(av && oAV);
    EXPECT_LT(width(*av), width(*oAV) - 0.01);
    // Un tamaño con decimales se usa tal cual (el original lo trunca).
    const auto big = fp.fontToGeometry("o", "DejaVu Sans", "regular", 20.7, true);
    ASSERT_TRUE(big && oSize);
    EXPECT_GT(width(*big), width(*o) + 0.01);
    EXPECT_NEAR(width(*oSize), width(*oO), 1e-9);
}
