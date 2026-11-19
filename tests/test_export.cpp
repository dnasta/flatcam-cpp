// Exportar Gerber y Excellon (FileExport) contra export_gerber / export_excellon originales (golden_export.h).
#include <gtest/gtest.h>

#include <sstream>

#include "golden_export.h"
#include "io/FileExport.h"
#include "parsers/Excellon.h"
#include "parsers/Gerber.h"

using namespace flatcam;

namespace {

/// El texto sin la línea con la fecha ("Created on"), como el golden.
std::string withoutDate(const std::string& text) {
    std::stringstream in(text);
    std::string line, out;
    bool first = true;
    while (std::getline(in, line)) {
        if (line.find("Created on") != std::string::npos) continue;
        if (!first) out += "\n";
        out += line;
        first = false;
    }
    if (!text.empty() && text.back() == '\n') out += "\n";
    return out;
}

ExportHeader header() {
    ExportHeader h;
    h.name = "obj";
    h.createdOn = "x";
    return h;
}

}  // namespace

TEST(FileExport, GerberMatchesOriginal) {
    for (const auto& c : golden_export::kGerber) {
        SCOPED_TRACE(std::string(c.file) + " " + c.units + " " + c.zeros);
        Gerber g;
        ASSERT_TRUE(g.parseFile(std::string(FLATCAM_TEST_DATA_DIR) + "/" + c.file));
        GerberExportFormat f;
        f.units = c.units;
        f.whole = c.whole;
        f.fract = c.fract;
        f.zeros = c.zeros;
        EXPECT_EQ(withoutDate(exportGerber(g, f, header())), c.text);
    }
}

TEST(FileExport, ExcellonMatchesOriginal) {
    for (const auto& c : golden_export::kExcellon) {
        SCOPED_TRACE(std::string(c.file) + " " + c.units + " " + c.zeros + " " + c.format);
        Excellon e;
        ASSERT_TRUE(e.parseFile(std::string(FLATCAM_TEST_DATA_DIR) + "/" + c.file));
        e.createGeometry();
        ExcellonExportFormat f;
        f.units = c.units;
        f.whole = c.whole;
        f.fract = c.fract;
        f.zeros = c.zeros;
        f.format = c.format;
        EXPECT_EQ(withoutDate(exportExcellon(e, f, header())), c.text);
    }
}

// ---------------------------------------------------------------- errores del original corregidos (sección 14)
#include "core/OriginalBugs.h"

TEST(FileExportFixed, ExcellonSlotsAndInchFactor) {
    OriginalBugsScope fixed(false);
    for (const char* file : {"excellon_samples/slots_g85.drl", "excellon_samples/routed_slots.drl"}) {
        SCOPED_TRACE(file);
        Excellon e;
        ASSERT_TRUE(e.parseFile(std::string(FLATCAM_TEST_DATA_DIR) + "/" + file));
        e.createGeometry();
        std::vector<std::pair<Point2D, Point2D>> slots;
        for (const auto& [id, t] : e.drillTools()) slots.insert(slots.end(), t.slots.begin(), t.slots.end());
        ASSERT_FALSE(slots.empty());
        for (const char* type : {"routing", "drilling"}) {
            for (const char* units : {"METRIC", "INCH"}) {
                SCOPED_TRACE(std::string(type) + " " + units);
                ExcellonExportFormat f;
                f.units = units;
                f.whole = units == std::string("METRIC") ? 3 : 2;
                f.fract = units == std::string("METRIC") ? 4 : 6;
                f.slotType = type;
                const std::string text = exportExcellon(e, f, header());
                // Se vuelve a leer con las mismas ranuras (en mm; en pulgadas, con el factor exacto 1 / 25,4).
                Excellon back;
                std::vector<std::string> lines;
                std::stringstream ss(text);
                for (std::string l; std::getline(ss, l);) lines.push_back(l);
                ASSERT_TRUE(back.parseLines(lines));
                if (units == std::string("INCH")) back.convertUnits(Units::Millimeters);
                std::vector<std::pair<Point2D, Point2D>> got;
                for (const auto& [id, t] : back.drillTools()) got.insert(got.end(), t.slots.begin(), t.slots.end());
                ASSERT_EQ(got.size(), slots.size()) << text;
                for (std::size_t i = 0; i < got.size(); ++i) {
                    EXPECT_NEAR(got[i].first.x, slots[i].first.x, 1e-3);
                    EXPECT_NEAR(got[i].first.y, slots[i].first.y, 1e-3);
                    EXPECT_NEAR(got[i].second.x, slots[i].second.x, 1e-3);
                    EXPECT_NEAR(got[i].second.y, slots[i].second.y, 1e-3);
                }
            }
        }
    }
}
