// Óptimo contra el original (tests/tools/oracle_optimal.py -> golden_optimal.h).
#include <gtest/gtest.h>

#include <algorithm>
#include <cstdio>
#include <string>

#include "cam/PyNumber.h"
#include "golden_optimal.h"
#include "parsers/Gerber.h"
#include "tools/OptimalDistance.h"

using namespace flatcam;

TEST(OptimalDistance, MatchesTheOriginal) {
    for (const auto& c : golden_optimal::kCases) {
        SCOPED_TRACE(std::string(c.file) + " " + std::to_string(c.precision));
        Gerber g;
        ASSERT_TRUE(g.parseFile(std::string(FLATCAM_TEST_DATA_DIR) + "/" + c.file));
        const OptimalResult r = findMinimumDistances(g, c.precision);
        EXPECT_EQ(!r.onePolygon && !r.distances.empty(), c.ok);
        if (!c.ok) continue;
        const auto& [minDist, locs] = *r.distances.begin();
        char buf[64];
        std::snprintf(buf, sizeof buf, "%.*f", c.precision, minDist);
        EXPECT_EQ(std::string(buf), c.result);
        EXPECT_EQ(std::to_string(locs.size()), c.freq);
        // Los puntos de la mínima (el orden de los polígonos puede cambiar con la GEOS: como conjunto).
        std::vector<std::string> mine, want(c.minLocations.begin(), c.minLocations.end());
        for (const auto& p : locs) mine.push_back(pyPointPair(p));
        std::sort(mine.begin(), mine.end());
        std::sort(want.begin(), want.end());
        EXPECT_EQ(mine, want);
        // Las demás distancias, de menor a mayor, como str() de Python.
        std::vector<std::string> others;
        for (auto it = std::next(r.distances.begin()); it != r.distances.end(); ++it) others.push_back(pyFloatRepr(it->first));
        EXPECT_EQ(others, std::vector<std::string>(c.others.begin(), c.others.end()));
    }
}
