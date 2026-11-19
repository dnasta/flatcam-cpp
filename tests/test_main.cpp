// main de core_tests: los tests comparan con los oráculos del FlatCAM original, así que reproducen sus errores
// (core/OriginalBugs.h); los que prueban las correcciones usan OriginalBugsScope(false).
#include <gtest/gtest.h>

#include "core/OriginalBugs.h"

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    flatcam::setReproduceOriginalBugs(true);
    return RUN_ALL_TESTS();
}
