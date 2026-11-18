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
