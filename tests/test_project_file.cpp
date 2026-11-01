// Tests de la lectura y escritura de proyectos de FlatCAM (.FlatPrj).
#include <gtest/gtest.h>

#include <cstdio>
#include <filesystem>

#include <lzma.h>

#include <fstream>
#include <sstream>

#include "core/Shape.h"
#include "io/ProjectFile.h"

using namespace flatcam;

namespace {

const std::string kProject = std::string(FLATCAM_TEST_DATA_DIR) + "/luz_de_noche/luz_de_noche_reducido.FlatPrj";

/// El JSON tal como lo escribió Python, descomprimido directamente con liblzma.
std::string pythonText(const std::string& path) {
    std::ifstream in(path, std::ios::binary);
    std::stringstream ss;
    ss << in.rdbuf();
    const std::string packed = ss.str();
    std::string out(16u << 20, '\0');
    std::size_t inPos = 0, outPos = 0;
    uint64_t memlimit = UINT64_MAX;
    if (lzma_stream_buffer_decode(&memlimit, 0, nullptr, reinterpret_cast<const uint8_t*>(packed.data()), &inPos,
                                  packed.size(), reinterpret_cast<uint8_t*>(out.data()), &outPos, out.size()) != LZMA_OK)
        return {};
    out.resize(outPos);
    return out;
}

}  // namespace

TEST(ProjectFile, ParsesValuesLikePython) {
    JsonValue v;
    std::string err;
    ASSERT_TRUE(ProjectFile::parse(
        R"({"a": [1, -3.400000000000001, 2e-05, true, false, null, NaN, -Infinity], "s": "\u00e1\"\\\n\ud83d\ude00", "o": {}})",
        v, err))
        << err;
    const JsonValue& a = v["a"];
    ASSERT_EQ(a.items().size(), 8u);
    EXPECT_TRUE(a.items()[0].isInteger());
    EXPECT_FALSE(a.items()[1].isInteger());
    EXPECT_EQ(a.items()[1].text(), "-3.400000000000001");
    EXPECT_DOUBLE_EQ(a.items()[1].toDouble(), -3.400000000000001);
    EXPECT_DOUBLE_EQ(a.items()[2].toDouble(), 2e-05);
    EXPECT_TRUE(a.items()[3].toBool());
    EXPECT_TRUE(a.items()[5].isNull());
    EXPECT_TRUE(std::isnan(a.items()[6].toDouble()));
    EXPECT_TRUE(std::isinf(a.items()[7].toDouble()));
    EXPECT_EQ(v["s"].text(), "\xC3\xA1\"\\\n\xF0\x9F\x98\x80");
    EXPECT_TRUE(v["o"].isObject());
    EXPECT_FALSE(ProjectFile::parse("{\"a\": [1, 2}", v, err));
    EXPECT_NE(err.find("JSON"), std::string::npos);
}

TEST(ProjectFile, DumpsLikePythonJsonDumpsIndent2SortKeys) {
    // json.dumps({"b": [1, 2.5, {"z": None, "a": "ñ"}], "a": {}, "c": [], "d": True}, indent=2, sort_keys=True)
    JsonValue v = JsonValue::object();
    JsonValue b = JsonValue::array();
    b.push(JsonValue::integer(1));
    b.push(JsonValue::real(2.5));
    JsonValue inner = JsonValue::object();
    inner.set("z", JsonValue());
    inner.set("a", JsonValue::string("\xC3\xB1"));
    b.push(inner);
    v.set("b", b);
    v.set("a", JsonValue::object());
    v.set("c", JsonValue::array());
    v.set("d", JsonValue::boolean(true));
    EXPECT_EQ(ProjectFile::dump(v),
              "{\n  \"a\": {},\n  \"b\": [\n    1,\n    2.5,\n    {\n      \"a\": \"\\u00f1\",\n      \"z\": null\n    }\n"
              "  ],\n  \"c\": [],\n  \"d\": true\n}");
}

TEST(ProjectFile, ReadsTheUsersProjectAndSkipsWhatIsNotNeeded) {
    // Proyecto real del usuario (FlatCAM 2024.4), reducido: 3 Gerber, el Excellon, la Geometría del corte y 2 CNC Job.
    JsonValue root;
    std::string err;
    ASSERT_TRUE(ProjectFile::read(kProject, root, err)) << err;
    ASSERT_TRUE(root["objs"].isArray());
    ASSERT_EQ(root["objs"].items().size(), 7u);
    EXPECT_EQ(root["version"].text(), "2024.4");
    EXPECT_EQ(root["options"]["units"].text(), "MM");
    const JsonValue& edge = root["objs"].items()[0];
    EXPECT_EQ(edge["kind"].text(), "gerber");
    EXPECT_EQ(edge["options"]["name"].text(), "luz de noche Pascual-Edge_Cuts.gbr");
    const JsonValue& solid = edge["solid_geometry"];
    EXPECT_EQ(solid["__class__"].text(), "Shply");
    const Shape s = Shape::fromWkt(solid["__inst__"].text());
    EXPECT_FALSE(s.isEmpty());
    EXPECT_NEAR(s.bounds()->xmax, 35.025, 1e-3);

    // Salteando el G-code interpretado y la geometría de los CNC Job (solo se guarda lo demás).
    JsonValue lean;
    std::size_t skipped = 0;
    ASSERT_TRUE(ProjectFile::read(kProject, lean, err, [&](const std::vector<std::string>& path, const JsonValue& parent) {
        const bool cnc = parent["kind"].text() == "cncjob";
        const bool skip = path.back() == "gcode_parsed" || (cnc && path.back() == "solid_geometry");
        skipped += skip;
        return skip;
    })) << err;
    EXPECT_GT(skipped, 3u);
    const JsonValue& job = lean["objs"].items()[5];
    EXPECT_EQ(job["kind"].text(), "cncjob");
    EXPECT_TRUE(job["gcode_parsed"].isNull());
    EXPECT_FALSE(root["objs"].items()[5]["gcode_parsed"].isNull());
    EXPECT_EQ(job["gcode"].text(), root["objs"].items()[5]["gcode"].text());
}

TEST(ProjectFile, WritesWhatPythonWritesAndReadsItBack) {
    JsonValue root;
    std::string err;
    ASSERT_TRUE(ProjectFile::read(kProject, root, err)) << err;
    const auto dir = std::filesystem::temp_directory_path();
    const std::string plain = (dir / "flatcam_test_plain.FlatPrj").string();
    const std::string xz = (dir / "flatcam_test_xz.FlatPrj").string();
    ASSERT_TRUE(ProjectFile::write(plain, root, false, 9, err)) << err;
    ASSERT_TRUE(ProjectFile::write(xz, root, true, 6, err)) << err;
    // El texto es el mismo que escribió Python (json.dumps con indent=2 y sort_keys): se lee y se vuelve a escribir igual.
    JsonValue again;
    ASSERT_TRUE(ProjectFile::read(xz, again, err)) << err;
    EXPECT_EQ(ProjectFile::dump(again), ProjectFile::dump(root));
    std::FILE* f = std::fopen(plain.c_str(), "rb");
    ASSERT_NE(f, nullptr);
    std::string text;
    char buf[65536];
    for (std::size_t n; (n = std::fread(buf, 1, sizeof buf, f)) > 0;) text.append(buf, n);
    std::fclose(f);
    // Byte por byte igual al JSON que escribió Python (9.426.607 bytes).
    const std::string original = pythonText(kProject);
    ASSERT_EQ(original.size(), 9426607u);
    EXPECT_TRUE(text == original);
    std::filesystem::remove(plain);
    std::filesystem::remove(xz);
}
