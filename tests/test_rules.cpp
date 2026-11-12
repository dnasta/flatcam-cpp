// Verificación de reglas contra el original (tests/tools/oracle_rules.py -> golden_rules.h).
#include <gtest/gtest.h>

#include <algorithm>
#include <map>
#include <string>

#include "golden_rules.h"
#include "parsers/Excellon.h"
#include "parsers/Gerber.h"
#include "tools/RulesCheck.h"

using namespace flatcam;

namespace {

std::string base(const std::string& f) { return f.substr(f.find_last_of('/') + 1); }

}  // namespace

TEST(RulesCheck, MatchesTheOriginal) {
    std::map<std::string, RulesGerber> grb;
    std::map<std::string, RulesExcellon> exc;
    for (const auto& c : golden_rules::kCases)
        for (const std::string& f : c.files) {
            const std::string path = std::string(FLATCAM_TEST_DATA_DIR) + "/" + f;
            if (f.size() > 4 && f.substr(f.size() - 4) == ".drl") {
                if (exc.count(f)) continue;
                Excellon e;
                ASSERT_TRUE(e.parseFile(path));
                e.createGeometry();
                RulesExcellon r{base(f), {}};
                for (const auto& [id, t] : e.drillTools()) r.tools.push_back({t.tooldia, t.solidGeometry});
                exc[f] = r;
            } else {
                if (grb.count(f)) continue;
                Gerber g;
                ASSERT_TRUE(g.parseFile(path));
                grb[f] = {base(f), g.apertures(), g.apertureOrder()};
            }
        }
    for (const auto& c : golden_rules::kCases) {
        const std::string rule = c.rule;
        SCOPED_TRACE(rule + " " + c.files.front() + " " + std::to_string(c.size));
        RulesResult r;
        if (rule == "traces") r = rulesTracesSize({grb[c.files[0]]}, c.size).front();
        else if (rule == "inside") r = rulesInsideClearance(grb[c.files[0]], c.size, "x");
        else if (rule == "gerber") r = rulesGerberClearance({grb[c.files[0]], grb[c.files[1]]}, c.size, "x");
        else if (rule == "holes_size") r = rulesHolesSize({exc[c.files[0]]}, c.size).front();
        else if (rule == "holes_clearance") r = rulesHolesClearance({exc[c.files[0]]}, c.size);
        else r = rulesAnnularRing(grb[c.files[0]], exc[c.files[1]], c.size, "x");
        EXPECT_EQ(r.names, c.names);
        EXPECT_EQ(r.failed, c.failed);
        EXPECT_EQ(r.dias, c.dias);
        // Los puntos: el original los guarda en un set (el orden no se puede reproducir); se comparan ordenados.
        std::vector<std::pair<double, double>> got, want;
        for (const auto& p : r.points) got.push_back({p.p.x, p.p.y});
        for (std::size_t i = 0; i + 1 < c.points.size(); i += 2) want.push_back({c.points[i], c.points[i + 1]});
        std::sort(got.begin(), got.end());
        std::sort(want.begin(), want.end());
        ASSERT_EQ(got.size(), want.size());
        for (std::size_t i = 0; i < got.size(); ++i) {
            // El punto representativo de un trazo puede correrse unos nanómetros (0,1 micra de tolerancia).
            EXPECT_NEAR(got[i].first, want[i].first, 1e-4) << i;
            EXPECT_NEAR(got[i].second, want[i].second, 1e-4) << i;
        }
        int shp = 0;
        for (const auto& p : r.points) shp += p.shapely ? 1 : 0;
        int want_shp = 0;
        for (int v : c.shapelyPoints) want_shp += v;
        EXPECT_EQ(shp, want_shp);
    }
}
