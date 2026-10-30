#include <gtest/gtest.h>

#include <cmath>

#include "core/GeometryError.h"
#include "parsers/ApertureMacro.h"
#include "test_helpers.h"

using namespace flatcam;

namespace {
constexpr double kPi = 3.14159265358979323846;

ApertureMacro make(const std::string& raw) {
    ApertureMacro m("T");
    m.append(raw);
    return m;
}
}  // namespace

TEST(ApertureMacroExpr, Arithmetic) {
    const std::map<std::string, double> v{{"1", 4.0}, {"2", 0.5}};
    EXPECT_DOUBLE_EQ(ApertureMacro::evaluate("1+2*3", v), 7.0);
    EXPECT_DOUBLE_EQ(ApertureMacro::evaluate("(1+2)x3", v), 9.0);
    EXPECT_DOUBLE_EQ(ApertureMacro::evaluate("$1x$2", v), 2.0);
    EXPECT_DOUBLE_EQ(ApertureMacro::evaluate("$1/8", v), 0.5);
    EXPECT_DOUBLE_EQ(ApertureMacro::evaluate("-$2", v), -0.5);
    EXPECT_DOUBLE_EQ(ApertureMacro::evaluate("--3", v), 3.0);
    EXPECT_DOUBLE_EQ(ApertureMacro::evaluate("1e-3x1000", v), 1.0);
    EXPECT_DOUBLE_EQ(ApertureMacro::evaluate("$9", v), 0.0);  // variable sin definir
    EXPECT_THROW(ApertureMacro::evaluate("1+", v), GeometryError);
    EXPECT_THROW(ApertureMacro::evaluate("2 3", v), GeometryError);
}

TEST(ApertureMacroExpr, MultiDigitVariables) {
    std::map<std::string, double> v;
    for (int i = 1; i <= 12; ++i) v[std::to_string(i)] = i;
    EXPECT_DOUBLE_EQ(ApertureMacro::evaluate("$10+$1", v), 11.0);  // $10 no se confunde con $1
}

TEST(ApertureMacroPrimitives, CircleAndRectangle) {
    // Rectángulo 4 x 2 menos un círculo de diámetro 1 en el centro.
    const auto m = make("0 comentario*21,1,4,2,0,0,0*1,0,1,0,0*");
    const Shape s = m.makeGeometry({});
    EXPECT_NEAR(s.area(), 8.0 - kPi * 0.25, 0.01);
    EXPECT_EQ(s.interiors().size(), 1u);
}

TEST(ApertureMacroPrimitives, VectorLineWithRotation) {
    // Línea de 3 de largo y 1 de ancho, rotada 90 grados alrededor del origen.
    const auto m = make("20,1,1,0,0,3,0,90*");
    const Shape s = m.makeGeometry({});
    EXPECT_NEAR(s.area(), 3.0, 1e-9);
    const auto b = s.bounds();
    ASSERT_TRUE(b.has_value());
    EXPECT_NEAR(b->xmin, -0.5, 1e-9);
    EXPECT_NEAR(b->xmax, 0.5, 1e-9);
    EXPECT_NEAR(b->ymax, 3.0, 1e-9);
}

TEST(ApertureMacroPrimitives, LowerLeftLineAndOutlineAndPolygon) {
    EXPECT_NEAR(make("22,1,2,3,1,1,0*").makeGeometry({}).area(), 6.0, 1e-9);
    // Contorno triangular: (0,0), (4,0), (0,3) y vuelve a (0,0). n = 3 puntos.
    EXPECT_NEAR(make("4,1,3,0,0,4,0,0,3,0,0,0*").makeGeometry({}).area(), 6.0, 1e-9);
    // Hexágono regular de diámetro circunscrito 2: área = 3*sqrt(3)/2 * r^2
    EXPECT_NEAR(make("5,1,6,0,0,2,0*").makeGeometry({}).area(), 3 * std::sqrt(3.0) / 2, 1e-9);
}

TEST(ApertureMacroPrimitives, ThermalAndMoire) {
    // Térmica: anillo (diámetros 4 y 2) cortado por una cruz de 0,5 de ancho.
    const Shape th = make("7,0,0,4,2,0.5,0*").makeGeometry({});
    const double ring = kPi * (4 - 1);
    EXPECT_LT(th.area(), ring);
    EXPECT_GT(th.area(), ring - 4 * 0.5 * 1.0 - 0.1);
    EXPECT_EQ(th.numParts(), 4u);
    // Moiré: cruz más anillos concéntricos.
    const Shape mo = make("6,0,0,6,0.5,0.5,3,0.2,7,0*").makeGeometry({});
    EXPECT_FALSE(mo.isEmpty());
    EXPECT_GT(mo.area(), 0.0);
}

TEST(ApertureMacroInstances, VariablesFromModifiers) {
    // Como la macro RotRect de KiCad: rectángulo $1 x $2 rotado $3 grados.
    const auto m = make("21,1,$1,$2,0,0,$3*");
    const Shape s = m.makeGeometry({"3.0", "1.0", "90"});
    EXPECT_NEAR(s.area(), 3.0, 1e-9);
    const auto b = s.bounds();
    ASSERT_TRUE(b.has_value());
    EXPECT_NEAR(b->width(), 1.0, 1e-9);
    EXPECT_NEAR(b->height(), 3.0, 1e-9);
}

TEST(ApertureMacroInstances, UnknownPrimitiveThrows) {
    EXPECT_THROW(make("99,1,1*").makeGeometry({}), GeometryError);
}

TEST(ApertureMacroInstances, EmptyMacroIsEmpty) {
    EXPECT_TRUE(make("0 solo un comentario*").makeGeometry({}).isEmpty());
}
