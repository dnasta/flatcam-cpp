// Código QR contra la biblioteca qrcode 7.4.2 y ToolQRCode originales (tests/tools/oracle_qrcode.py -> golden_qrcode.h).
#include <gtest/gtest.h>

#include <cmath>
#include <string>

#include "golden_qrcode.h"
#include "parsers/Gerber.h"
#include "tools/QrCode.h"

using namespace flatcam;

TEST(QrCode, MatchesTheOriginal) {
    Gerber g;
    ASSERT_TRUE(g.parseFile(std::string(FLATCAM_TEST_DATA_DIR) + "/luz_de_noche/luz de noche Pascual-B_Cu.gbr"));
    for (const auto& c : golden_qrcode::kCases) {
        SCOPED_TRACE(c.data.substr(0, 30) + " v" + std::to_string(c.version) + " " + c.ec);
        if (c.overflow) {
            EXPECT_THROW(qrMake(c.data, c.version, qrErrorCorrection(c.ec)), QrError);
            continue;
        }
        const QrMatrix qr = qrMake(c.data, c.version, qrErrorCorrection(c.ec));
        // La misma versión y los mismos módulos (los trozos, la corrección y la máscara elegida).
        EXPECT_EQ(qr.version, c.finalVersion);
        ASSERT_EQ(qr.modules.size(), c.rows.size());
        for (std::size_t r = 0; r < c.rows.size(); ++r) {
            std::string row;
            for (bool v : qr.modules[r]) row += v ? '1' : '0';
            ASSERT_EQ(row, c.rows[r]) << "fila " << r;
        }
        // El archivo de "Exportar SVG", texto por texto.
        if (!c.svgPath.empty()) EXPECT_EQ(qrSvgPath(qr, c.box, c.border, "#000000", "#FFFFFF"), c.svgPath);
        // La geometría: un cuadrado por módulo y su unión.
        const QrGeometry geo = qrGeometry(qr, c.box, c.border);
        ASSERT_EQ(static_cast<int>(geo.modules.size()), c.modules);
        double area = 0.0;
        for (const Shape& s : geo.modules) area += s.area();
        EXPECT_NEAR(area, c.modulesArea, 1e-9);
        EXPECT_TRUE(geo.modules[0].equalsExact(Shape::fromWkt(c.firstModule), 1e-12));
        const ShapeList parts = geo.utility.parts();
        EXPECT_EQ(parts.empty() ? 1 : static_cast<int>(parts.size()), c.utilityParts);
        EXPECT_NEAR(geo.utility.area(), c.utilityArea, 1e-6);
        const BoundingBox b = *geo.utility.bounds();
        EXPECT_NEAR(b.xmin, c.utilityBounds[0], 1e-9);
        EXPECT_NEAR(b.ymin, c.utilityBounds[1], 1e-9);
        EXPECT_NEAR(b.xmax, c.utilityBounds[2], 1e-9);
        EXPECT_NEAR(b.ymax, c.utilityBounds[3], 1e-9);
        if (!c.make) continue;
        // make(): el cobre cortado, los módulos (o la máscara menos ellos) y las aperturas.
        const QrGerber r = qrInsert(g, geo, {c.px, c.py}, c.box, c.border, std::string(c.rounded) == "r",
                                    std::string(c.polarity) == "neg");
        EXPECT_EQ(static_cast<int>(r.solid.size()), c.solid);
        double sarea = 0.0;
        for (const Shape& s : r.solid) sarea += s.area();
        EXPECT_NEAR(sarea, c.solidArea, 1e-6);
        EXPECT_EQ(r.newAperture, c.newAp);
        const Aperture& ap = r.apertures.at(r.newAperture);
        EXPECT_EQ(ap.type, "R");
        EXPECT_EQ(*ap.size, c.apSize);
        EXPECT_EQ(ap.width, c.apWidth);
        EXPECT_EQ(ap.height, c.apHeight);
        ASSERT_EQ(static_cast<int>(ap.geometry.size()), c.apCount);
        EXPECT_TRUE(ap.geometry[0].solid->equalsExact(Shape::fromWkt(c.apFirst), 1e-12));
        EXPECT_TRUE(ap.geometry[0].follow->equalsExact(Shape::fromWkt(c.apFirstFollow), 1e-12));
        const Aperture& zero = r.apertures.at("0");
        const std::size_t before = g.apertures().count("0") ? g.apertures().at("0").geometry.size() : 0;
        EXPECT_EQ(static_cast<int>(zero.geometry.size() - before), c.zeroCount);
        EXPECT_TRUE(zero.geometry.back().clear->equalsExact(Shape::fromWkt(c.clear), 1e-9));
    }
}
