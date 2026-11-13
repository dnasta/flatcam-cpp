// Importar imagen contra el original (tests/tools/oracle_image.py, con rasterio).
#include <gtest/gtest.h>

#include "golden_image.h"
#include "parsers/ImageImport.h"

using namespace flatcam;

TEST(ImageImport, MatchesTheOriginal) {
    for (const auto& c : golden_image::kCases) {
        SCOPED_TRACE(std::string(c.file) + " " + c.mode);
        const auto& b = golden_image::kBands.at(c.file);
        ImageBands img;
        img.width = b.width;
        img.height = b.height;
        img.red = b.bands[0];
        if (b.bands.size() > 1) img.green = b.bands[1];
        if (b.bands.size() > 2) img.blue = b.bands[2];
        const Shape s = importImage(img, c.units, c.dpi, c.mode, {c.mask[0], c.mask[1], c.mask[2], c.mask[3]});
        EXPECT_NEAR(s.area(), c.area, 1e-12);
        const BoundingBox bb = s.bounds().value_or(BoundingBox{});
        EXPECT_NEAR(bb.xmin, c.bounds[0], 1e-12);
        EXPECT_NEAR(bb.ymin, c.bounds[1], 1e-12);
        EXPECT_NEAR(bb.xmax, c.bounds[2], 1e-12);
        EXPECT_NEAR(bb.ymax, c.bounds[3], 1e-12);
        const ShapeList polys = s.type() == ShapeType::Polygon ? ShapeList{s} : s.parts();
        EXPECT_EQ(static_cast<int>(polys.size()), c.polygons);
        int holes = 0;
        for (const Shape& p : polys) holes += static_cast<int>(p.interiors().size());
        EXPECT_EQ(holes, c.holes);
    }
}
