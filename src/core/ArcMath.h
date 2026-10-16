/**
 * @file ArcMath.h
 * @brief Funciones matemáticas de arcos y círculos portadas de `camlib.py`.
 */
#pragma once

#include <optional>
#include <vector>

#include "Types.h"

namespace flatcam {

/** @brief Círculo calculado a partir de tres puntos de su circunferencia. */
struct Circle3P {
    Point2D center;    ///< Centro del círculo.
    double radius{0};  ///< Radio.
    double t{0};       ///< Parámetro `T[0]` de la solución (se conserva por compatibilidad con camlib).
};

/**
 * @brief Genera los puntos de un arco (port de `camlib.arc`).
 *
 * El arco se aproxima con segmentos rectos; la cantidad de segmentos es proporcional al ángulo
 * barrido, con un mínimo de 2.
 *
 * @param center Centro del arco.
 * @param radius Radio.
 * @param start Ángulo inicial, en radianes.
 * @param stop Ángulo final, en radianes.
 * @param direction Sentido de giro.
 * @param stepsPerCircle Segmentos para representar una circunferencia completa.
 * @return Puntos del arco, desde el inicial hasta el final (ambos incluidos).
 */
std::vector<Point2D> arc(const Point2D& center, double radius, double start, double stop,
                         ArcDirection direction, int stepsPerCircle);

/**
 * @brief Arco definido por sus extremos y su centro (port de `camlib.arc2`).
 * @param p1 Punto inicial.
 * @param p2 Punto final.
 * @param center Centro.
 * @param direction Sentido de giro.
 * @param stepsPerCircle Segmentos por circunferencia completa.
 * @return Puntos del arco.
 */
std::vector<Point2D> arc2(const Point2D& p1, const Point2D& p2, const Point2D& center,
                          ArcDirection direction, int stepsPerCircle);

/**
 * @brief Ángulo barrido por un arco (port de `camlib.arc_angle`).
 * @param start Ángulo inicial, en radianes.
 * @param stop Ángulo final, en radianes.
 * @param direction Sentido de giro.
 * @return Ángulo positivo barrido, en radianes.
 */
double arcAngle(double start, double stop, ArcDirection direction);

/**
 * @brief Círculo que pasa por tres puntos (port de `camlib.three_point_circle`).
 * @param p1 Primer punto.
 * @param p2 Segundo punto.
 * @param p3 Tercer punto.
 * @return El círculo, o `std::nullopt` si los puntos son colineales.
 */
std::optional<Circle3P> threePointCircle(const Point2D& p1, const Point2D& p2, const Point2D& p3);

/**
 * @brief Distancia entre dos puntos (port de `camlib.distance`).
 * @param a Primer punto.
 * @param b Segundo punto.
 * @return Distancia euclídea.
 */
double distance(const Point2D& a, const Point2D& b);

/**
 * @brief Distancia euclídea entre `(x1, y1)` y `(x2, y2)` (port de `camlib.distance_euclidian`).
 * @param x1 X del primer punto.
 * @param y1 Y del primer punto.
 * @param x2 X del segundo punto.
 * @param y2 Y del segundo punto.
 * @return Distancia euclídea.
 */
double distanceEuclidian(double x1, double y1, double x2, double y2);

}  // namespace flatcam
