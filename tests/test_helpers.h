// Utilidades compartidas por los tests.
#pragma once

#include <gtest/gtest.h>

#include <vector>

#include "core/Types.h"

/// Compara dos listas de puntos con una tolerancia absoluta.
inline void expectPointsNear(const std::vector<flatcam::Point2D>& actual,
                             const std::vector<flatcam::Point2D>& expected, double tol) {
    ASSERT_EQ(actual.size(), expected.size());
    for (std::size_t i = 0; i < expected.size(); ++i) {
        EXPECT_NEAR(actual[i].x, expected[i].x, tol) << "x en el índice " << i;
        EXPECT_NEAR(actual[i].y, expected[i].y, tol) << "y en el índice " << i;
    }
}

/// Compara dos números con tolerancia relativa (para áreas).
inline void expectRelNear(double actual, double expected, double rel = 1e-9) {
    EXPECT_NEAR(actual, expected, std::abs(expected) * rel + 1e-12);
}
