/**
 * @file test_source_hygiene.cpp
 * @brief Que los goldens generados compilen también con MSVC: ningún literal de texto (o secuencia de literales
 * pegados) de más de 12.000 bytes (MSVC: 16.380 por literal, 65.535 concatenados). Si falla, correr
 * `python3 tests/tools/split_long_literals.py` después de regenerar el golden.
 */
#include <gtest/gtest.h>

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>

namespace {

/// El largo en bytes de la secuencia de literales más larga del texto (sin contar comentarios de línea).
size_t longestLiteralRun(const std::string& s) {
    size_t best = 0, run = 0;
    bool inRun = false;
    for (size_t i = 0; i < s.size(); ++i) {
        const char c = s[i];
        if (c == '/' && i + 1 < s.size() && s[i + 1] == '/') {  // comentario: hasta el fin de línea
            while (i < s.size() && s[i] != '\n') ++i;
            inRun = false;
            continue;
        }
        if (c == '\'') {  // carácter: '"' no abre un texto
            i += (i + 1 < s.size() && s[i + 1] == '\\') ? 3 : 2;
            inRun = false;
            continue;
        }
        if (c == '"') {
            size_t j = i + 1;
            while (j < s.size() && s[j] != '"') j += (s[j] == '\\') ? 2 : 1;
            run = (inRun ? run : 0) + (j - i + 1);
            best = std::max(best, run);
            inRun = true;
            i = j;
            continue;
        }
        if (c != ' ' && c != '\t') inRun = false;
    }
    return best;
}

}  // namespace

TEST(SourceHygiene, NoStringLiteralTooLongForMsvc) {
    const std::filesystem::path dir = std::filesystem::path(FLATCAM_TEST_DATA_DIR).parent_path();
    int files = 0;
    for (const auto& e : std::filesystem::directory_iterator(dir)) {
        const std::string name = e.path().filename().string();
        if (name.rfind("golden_", 0) != 0 || e.path().extension() != ".h") continue;
        std::ifstream in(e.path(), std::ios::binary);
        std::stringstream ss;
        ss << in.rdbuf();
        EXPECT_LE(longestLiteralRun(ss.str()), 12000u) << name << ": correr tests/tools/split_long_literals.py";
        ++files;
    }
    EXPECT_GT(files, 20);
}
