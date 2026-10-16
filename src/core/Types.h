/**
 * @file Types.h
 * @brief Tipos básicos compartidos por todo el núcleo: puntos, cajas, unidades y enumeraciones.
 *
 * @copyright Copyright (c) 2026 dnasta. Licencia MIT (ver LICENSE y NOTICE).
 *            Port de FlatCAM, Copyright (c) 2014-2016 Juan Pablo Caram y colaboradores.
 */
#pragma once

#include <cmath>
#include <string>

namespace flatcam {

/**
 * @brief Punto (o vector) 2D con coordenadas de doble precisión.
 *
 * Equivale a las tuplas `(x, y)` que FlatCAM usa en Python para puntos y vectores.
 */
struct Point2D {
    double x{0.0};  ///< Coordenada X.
    double y{0.0};  ///< Coordenada Y.

    /** @brief Construye el punto (0, 0). */
    constexpr Point2D() = default;

    /**
     * @brief Construye un punto con las coordenadas dadas.
     * @param x_ Coordenada X.
     * @param y_ Coordenada Y.
     */
    constexpr Point2D(double x_, double y_) : x(x_), y(y_) {}

    /** @brief Igualdad exacta componente a componente. @param o Punto a comparar. @return `true` si ambas coordenadas coinciden. */
    constexpr bool operator==(const Point2D& o) const { return x == o.x && y == o.y; }
    /** @brief Desigualdad exacta componente a componente. @param o Punto a comparar. @return `true` si alguna coordenada difiere. */
    constexpr bool operator!=(const Point2D& o) const { return !(*this == o); }
    /** @brief Suma vectorial. @param o Sumando. @return Suma de ambos vectores. */
    constexpr Point2D operator+(const Point2D& o) const { return {x + o.x, y + o.y}; }
    /** @brief Resta vectorial. @param o Sustraendo. @return Diferencia entre ambos vectores. */
    constexpr Point2D operator-(const Point2D& o) const { return {x - o.x, y - o.y}; }
    /** @brief Producto por un escalar. @param k Escalar. @return Vector escalado. */
    constexpr Point2D operator*(double k) const { return {x * k, y * k}; }
    /** @brief División por un escalar. @param k Divisor. @return Vector dividido. */
    constexpr Point2D operator/(double k) const { return {x / k, y / k}; }

    /** @brief Longitud euclídea del vector. @return Longitud. */
    double norm() const { return std::hypot(x, y); }
};

/**
 * @brief Caja delimitadora alineada con los ejes `(xmin, ymin, xmax, ymax)`.
 */
struct BoundingBox {
    double xmin{0.0};  ///< Límite izquierdo.
    double ymin{0.0};  ///< Límite inferior.
    double xmax{0.0};  ///< Límite derecho.
    double ymax{0.0};  ///< Límite superior.

    /** @brief Ancho de la caja (`xmax - xmin`). @return Ancho. */
    double width() const { return xmax - xmin; }
    /** @brief Alto de la caja (`ymax - ymin`). @return Alto. */
    double height() const { return ymax - ymin; }
    /** @brief Centro de la caja. @return Punto central. */
    Point2D center() const { return {(xmin + xmax) / 2.0, (ymin + ymax) / 2.0}; }

    /**
     * @brief Devuelve la caja más chica que contiene a esta y a @p o.
     * @param o Otra caja.
     * @return Caja unión.
     */
    BoundingBox merged(const BoundingBox& o) const {
        return {std::fmin(xmin, o.xmin), std::fmin(ymin, o.ymin),
                std::fmax(xmax, o.xmax), std::fmax(ymax, o.ymax)};
    }
};

/** @brief Unidades de longitud soportadas (equivalen a "MM" e "IN" de FlatCAM). */
enum class Units {
    Millimeters,  ///< Milímetros.
    Inches        ///< Pulgadas.
};

/**
 * @brief Convierte un texto ("MM", "mm", "IN", "in") en un valor de `Units`.
 * @param text Texto a interpretar, sin distinguir mayúsculas.
 * @return Unidad correspondiente.
 * @throws std::invalid_argument si el texto no es una unidad conocida.
 */
Units unitsFromString(const std::string& text);

/**
 * @brief Convierte una unidad en su texto estándar de FlatCAM.
 * @param units Unidad.
 * @return `"MM"` o `"IN"`.
 */
std::string toString(Units units);

/** @brief Tipo de unión de esquinas al hacer un buffer (valores idénticos a los de GEOS/Shapely). */
enum class JoinStyle {
    Round = 1,  ///< Esquinas redondeadas.
    Mitre = 2,  ///< Esquinas en ingletes (cuadradas/puntiagudas).
    Bevel = 3   ///< Esquinas biseladas.
};

/** @brief Tipo de extremo al hacer un buffer de una línea (valores idénticos a los de GEOS/Shapely). */
enum class CapStyle {
    Round = 1,   ///< Extremo redondeado.
    Flat = 2,    ///< Extremo plano, sin extensión.
    Square = 3   ///< Extremo cuadrado, extendido medio ancho.
};

/** @brief Sentido de giro de un arco. */
enum class ArcDirection {
    Clockwise,         ///< Horario ("cw" en FlatCAM, G02).
    CounterClockwise   ///< Antihorario ("ccw" en FlatCAM, G03).
};

/** @brief Eje alrededor del cual se refleja una geometría. */
enum class Axis {
    X,  ///< Eje X: invierte las coordenadas Y.
    Y   ///< Eje Y: invierte las coordenadas X.
};

}  // namespace flatcam
