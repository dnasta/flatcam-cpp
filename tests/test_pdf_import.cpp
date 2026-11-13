// Importar PDF contra el original (tests/tools/oracle_pdf.py).
#include <gtest/gtest.h>

#include <map>

#include "golden_pdf.h"
#include "parsers/PdfImport.h"

using namespace flatcam;

TEST(PdfImport, MatchesTheOriginal) {
    std::map<std::string, std::map<int, PdfApertures>> parsed;
    for (const auto& [file, text] : golden_pdf::kTexts) parsed[file] = parsePdf(text, "MM", 64);
    std::map<std::string, int> layersPerFile;
    for (const auto& L : golden_pdf::kLayers) {
        SCOPED_TRACE(std::string(L.file) + " capa " + std::to_string(L.layer));
        ++layersPerFile[L.file];
        const auto& layers = parsed.at(L.file);
        ASSERT_TRUE(layers.count(L.layer));
        const PdfApertures& aps = layers.at(L.layer);
        ASSERT_EQ(aps.size(), L.apertures.size());
        for (std::size_t i = 0; i < aps.size(); ++i) {
            const auto& [code, ap] = aps[i];
            const golden_pdf::Ap& g = L.apertures[i];
            EXPECT_EQ(code, g.code);
            EXPECT_EQ(ap.size.value_or(-1), g.size);
            EXPECT_EQ(static_cast<int>(ap.geometry.size()), g.elements);
            double solid = 0, clear = 0;
            for (const ApertureGeometry& e : ap.geometry) {
                if (e.solid) solid += e.solid->area();
                if (e.clear) clear += e.clear->area();
            }
            EXPECT_NEAR(solid, g.solidArea, 1e-6);
            EXPECT_NEAR(clear, g.clearArea, 1e-6);
        }
        if (L.layer == 0) {
            const auto tools = pdfLayerAsExcellon(aps);
            ASSERT_EQ(tools.size(), L.tools.size());
            for (std::size_t i = 0; i < tools.size(); ++i) {
                EXPECT_EQ(tools[i].first, L.tools[i].dia);
                ASSERT_EQ(tools[i].second.size() * 2, L.tools[i].drills.size());
                for (std::size_t j = 0; j < tools[i].second.size(); ++j) {
                    EXPECT_NEAR(tools[i].second[j].x, L.tools[i].drills[2 * j], 1e-9);
                    EXPECT_NEAR(tools[i].second[j].y, L.tools[i].drills[2 * j + 1], 1e-9);
                }
            }
        } else {
            const PdfGerberLayer g = pdfLayerAsGerber(aps);
            double area = 0;
            for (const Shape& s : g.solid) area += s.area();
            EXPECT_NEAR(area, L.area, 1e-6);
            EXPECT_EQ(static_cast<int>(g.solid.size()), L.parts[0]);
            const BoundingBox b = Shape::unaryUnion(g.solid).bounds().value_or(BoundingBox{});
            EXPECT_NEAR(b.xmin, L.bounds[0], 1e-6);
            EXPECT_NEAR(b.ymin, L.bounds[1], 1e-6);
            EXPECT_NEAR(b.xmax, L.bounds[2], 1e-6);
            EXPECT_NEAR(b.ymax, L.bounds[3], 1e-6);
        }
    }
    for (const auto& [file, n] : layersPerFile) EXPECT_EQ(static_cast<int>(parsed.at(file).size()), n) << file;
}

TEST(PdfImport, AbortsLikeTheOriginal) {
    // 'B' sobre un trazado de un punto: el original falla (LineString sin try) y no queda nada.
    EXPECT_TRUE(parsePdf("1 w\n0 0 m\n5 5 l\nS\n10 10 m\nB\n", "MM").empty());
    EXPECT_FALSE(parsePdf("1 w\n0 0 m\n5 5 l\nS\n", "MM").empty());
    // Bézier sin punto actual: TypeError.
    EXPECT_TRUE(parsePdf("1 2 3 4 5 6 c\n", "MM").empty());
}
