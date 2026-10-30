/**
 * @file PyNumber.h
 * @brief Número que recuerda si era entero o decimal, para imprimirlo como lo hace `str()` de Python.
 */
#pragma once

#include <string>

namespace flatcam {

/**
 * @brief Texto de un número decimal igual al de `repr(float)` de Python (la forma más corta que lo representa).
 *
 * Ejemplos: `100.0`, `0.8`, `1e-05`, `1.5e+16`, `-3.5`.
 * @param value Número a imprimir.
 * @return El texto.
 */
std::string pyFloatRepr(double value);

/**
 * @brief Número que conserva su tipo (entero o decimal) de Python.
 *
 * FlatCAM imprime en los comentarios del G-code valores con `str()`, y el resultado depende del tipo: un
 * avance puede ser `1500` (entero) o `100.0` (decimal) según cómo se haya guardado en el proyecto. Esta clase
 * reproduce ese comportamiento.
 */
class PyNumber {
public:
    /** @brief Entero 0. */
    PyNumber() = default;

    /** @brief Crea un entero. @param v Valor. */
    PyNumber(int v) : value_(v), isInt_(true) {}

    /** @brief Crea un decimal. @param v Valor. */
    PyNumber(double v) : value_(v), isInt_(false) {}

    /** @brief Valor numérico. @return El valor como `double`. */
    double value() const { return value_; }

    /** @brief Indica el tipo. @return `true` si era un entero. */
    bool isInt() const { return isInt_; }

    /** @brief Equivale a `str(x)` de Python. @return El texto. */
    std::string str() const;

    /** @brief Equivale a `bool(x)` de Python. @return `false` si vale cero. */
    bool truthy() const { return value_ != 0.0; }

private:
    double value_{0.0};
    bool isInt_{true};
};

}  // namespace flatcam
