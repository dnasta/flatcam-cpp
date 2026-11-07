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
