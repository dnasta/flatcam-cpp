#include <gtest/gtest.h>

#include <functional>
#include <map>
#include <string>

#include "cam/Preprocessor.h"
#include "golden_preprocessors.h"

using namespace flatcam;

TEST(PyNumber, FloatReprMatchesPython) {
    for (const auto& f : golden_pp::kFloats) {
        EXPECT_EQ(pyFloatRepr(f.value), f.repr) << "valor " << f.repr;
    }
}

TEST(PyNumber, IntegersAndFloatsPrintDifferently) {
    EXPECT_EQ(PyNumber(1500).str(), "1500");
    EXPECT_EQ(PyNumber(1500.0).str(), "1500.0");
    EXPECT_EQ(PyNumber(-4).str(), "-4");
    EXPECT_TRUE(PyNumber(3).isInt());
    EXPECT_FALSE(PyNumber(3.0).isInt());
    EXPECT_FALSE(PyNumber(0).truthy());
    EXPECT_TRUE(PyNumber(0.1).truthy());
}

TEST(Preprocessor, FactoryFindsKnownNames) {
    EXPECT_NE(makePreprocessor("default"), nullptr);
    EXPECT_EQ(makePreprocessor("GRBL_laser")->name(), "GRBL_laser");
    EXPECT_EQ(makePreprocessor("no_existe"), nullptr);
}

class PreprocessorGolden : public ::testing::TestWithParam<std::size_t> {};

TEST_P(PreprocessorGolden, MatchesOriginalPreprocessor) {
    static const std::map<std::string, std::function<std::string(const Preprocessor&, const PreprocessorParams&)>> calls = {
        {"startCode", [](const Preprocessor& pp, const PreprocessorParams& p) { return pp.startCode(p); }},
        {"startzCode", [](const Preprocessor& pp, const PreprocessorParams& p) { return pp.startzCode(p); }},
        {"liftCode", [](const Preprocessor& pp, const PreprocessorParams& p) { return pp.liftCode(p); }},
        {"downCode", [](const Preprocessor& pp, const PreprocessorParams& p) { return pp.downCode(p); }},
        {"toolchangeCode", [](const Preprocessor& pp, const PreprocessorParams& p) { return pp.toolchangeCode(p); }},
        {"upToZeroCode", [](const Preprocessor& pp, const PreprocessorParams& p) { return pp.upToZeroCode(p); }},
        {"rapidCode", [](const Preprocessor& pp, const PreprocessorParams& p) { return pp.rapidCode(p); }},
        {"linearCode", [](const Preprocessor& pp, const PreprocessorParams& p) { return pp.linearCode(p); }},
        {"endCode", [](const Preprocessor& pp, const PreprocessorParams& p) { return pp.endCode(p); }},
        {"feedrateCode", [](const Preprocessor& pp, const PreprocessorParams& p) { return pp.feedrateCode(p); }},
        {"zFeedrateCode", [](const Preprocessor& pp, const PreprocessorParams& p) { return pp.zFeedrateCode(p); }},
        {"spindleCode", [](const Preprocessor& pp, const PreprocessorParams& p) { return pp.spindleCode(p); }},
        {"dwellCode", [](const Preprocessor& pp, const PreprocessorParams& p) { return pp.dwellCode(p); }},
        {"spindleStopCode", [](const Preprocessor& pp, const PreprocessorParams& p) { return pp.spindleStopCode(p); }},
    };
    const auto& c = golden_pp::kCases.at(GetParam());
    SCOPED_TRACE(std::string(c.pp) + " / " + golden_pp::kScenarioNames[c.scenario] + " / " + c.method);
    const auto pp = makePreprocessor(c.pp);
    ASSERT_NE(pp, nullptr);
    EXPECT_EQ(calls.at(c.method)(*pp, golden_pp::scenario(c.scenario)), c.expected);
}

INSTANTIATE_TEST_SUITE_P(Original, PreprocessorGolden, ::testing::Range<std::size_t>(0, golden_pp::kCases.size()));
