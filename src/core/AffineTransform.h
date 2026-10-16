/**
 * @file AffineTransform.h
 * @brief Transformaciones afines 2D (traslación, escala, rotación, inclinación, reflexión).
 */
#pragma once

#include "Types.h"

namespace flatcam {

/**
 * @brief Transformación afín 2D representada por la matriz
 * \f$\begin{pmatrix} a & b & x_{off} \\ d & e & y_{off} \\ 0 & 0 & 1 \end{pmatrix}\f$.
 *
 * Reproduce las funciones `shapely.affinity.scale/rotate/skew/translate` que FlatCAM usa en
 * `camlib.py`, incluido el detalle de anular los valores de seno/coseno menores a 2.5e-16 para que las
 * rotaciones de múltiplos de 90° den resultados exactos.
 *
 * Las transformaciones se pueden componer con el operador `*`: `(A * B).apply(p)` equivale a
 * `A.apply(B.apply(p))`.
 */
class AffineTransform {
public:
    /** @brief Construye la transformación identidad. */
    AffineTransform() = default;

    /**
     * @brief Construye la transformación a partir de los seis coeficientes.
     * @param a Coeficiente de x en la nueva x.
     * @param b Coeficiente de y en la nueva x.
     * @param d Coeficiente de x en la nueva y.
     * @param e Coeficiente de y en la nueva y.
     * @param xoff Desplazamiento en X.
     * @param yoff Desplazamiento en Y.
     */
    AffineTransform(double a, double b, double d, double e, double xoff, double yoff)
        : a_(a), b_(b), d_(d), e_(e), xoff_(xoff), yoff_(yoff) {}

    /** @brief Transformación identidad. @return La identidad. */
    static AffineTransform identity() { return {}; }

    /**
     * @brief Traslación.
     * @param dx Desplazamiento en X.
     * @param dy Desplazamiento en Y.
     * @return Transformación que traslada por `(dx, dy)`.
     */
    static AffineTransform translation(double dx, double dy);

    /**
     * @brief Escala respecto de un punto fijo.
     * @param sx Factor en X.
     * @param sy Factor en Y.
     * @param origin Punto que permanece fijo.
     * @return Transformación de escala.
     */
    static AffineTransform scaling(double sx, double sy, const Point2D& origin = {});

    /**
     * @brief Rotación antihoraria alrededor de un punto.
     * @param angleDegrees Ángulo en grados; positivo es antihorario.
     * @param origin Centro de giro.
     * @return Transformación de rotación.
     */
    static AffineTransform rotation(double angleDegrees, const Point2D& origin = {});

    /**
     * @brief Inclinación (cizalla) respecto de un punto fijo.
     * @param xAngleDegrees Ángulo de cizalla a lo largo de X, en grados.
     * @param yAngleDegrees Ángulo de cizalla a lo largo de Y, en grados.
     * @param origin Punto que permanece fijo.
     * @return Transformación de inclinación.
     */
    static AffineTransform skew(double xAngleDegrees, double yAngleDegrees, const Point2D& origin = {});

    /**
     * @brief Reflexión alrededor de un eje que pasa por un punto.
     * @param axis Eje de reflexión: X invierte Y, Y invierte X.
     * @param point Punto por el que pasa el eje.
     * @return Transformación de reflexión.
     */
    static AffineTransform mirror(Axis axis, const Point2D& point);

    /**
     * @brief Aplica la transformación a un punto.
     * @param p Punto de entrada.
     * @return Punto transformado.
     */
    Point2D apply(const Point2D& p) const {
        return {a_ * p.x + b_ * p.y + xoff_, d_ * p.x + e_ * p.y + yoff_};
    }

    /**
     * @brief Compone dos transformaciones.
     * @param rhs Transformación que se aplica primero.
     * @return Transformación equivalente a aplicar @p rhs y luego esta.
     */
    AffineTransform operator*(const AffineTransform& rhs) const;

    /** @brief Coeficiente `a`. @return Valor de `a`. */
    double a() const { return a_; }
    /** @brief Coeficiente `b`. @return Valor de `b`. */
    double b() const { return b_; }
    /** @brief Coeficiente `d`. @return Valor de `d`. */
    double d() const { return d_; }
    /** @brief Coeficiente `e`. @return Valor de `e`. */
    double e() const { return e_; }
    /** @brief Desplazamiento en X. @return Valor de `xoff`. */
    double xoff() const { return xoff_; }
    /** @brief Desplazamiento en Y. @return Valor de `yoff`. */
    double yoff() const { return yoff_; }

private:
    double a_{1.0}, b_{0.0}, d_{0.0}, e_{1.0}, xoff_{0.0}, yoff_{0.0};
};

}  // namespace flatcam
