#include "cam/PathOptimizer.h"

#include <algorithm>
#include <cmath>
#include <limits>

namespace flatcam {

namespace {
double dist(const Point2D& a, const Point2D& b) { return std::hypot(a.x - b.x, a.y - b.y); }
}  // namespace

double PathOptimizer::pathLength(const std::vector<Point2D>& points, const std::vector<std::size_t>& order,
                                 const Point2D& start) {
    double total = 0.0;
    Point2D prev = start;
    for (std::size_t i : order) {
        total += dist(prev, points[i]);
        prev = points[i];
    }
    return total;
}

namespace {

/// 2-opt sobre un camino abierto que parte de `start`: se invierte el tramo [i, j] mientras acorte el recorrido.
void improveTwoOpt(const std::vector<Point2D>& points, std::vector<std::size_t>& path, const Point2D& start) {
    const std::size_t n = path.size();
    auto at = [&](std::size_t k) -> const Point2D& { return k == 0 ? start : points[path[k - 1]]; };
    bool improved = true;
    int passes = 0;
    while (improved && passes++ < 200) {
        improved = false;
        for (std::size_t i = 1; i <= n; ++i) {
            for (std::size_t j = i + 1; j <= n; ++j) {
                // Camino: ... a=at(i-1) -> b=at(i) ... c=at(j) -> d=at(j+1) ...  se cambia por a->c ... b->d
                const Point2D& a = at(i - 1);
                const Point2D& b = at(i);
                const Point2D& c = at(j);
                double before = dist(a, b);
                double after = dist(a, c);
                if (j < n) {
                    const Point2D& d = at(j + 1);
                    before += dist(c, d);
                    after += dist(b, d);
                }
                if (after + 1e-12 < before) {
                    std::reverse(path.begin() + static_cast<std::ptrdiff_t>(i - 1),
                                 path.begin() + static_cast<std::ptrdiff_t>(j));
                    improved = true;
                }
            }
        }
    }
}

}  // namespace

std::vector<std::size_t> PathOptimizer::order(const std::vector<Point2D>& points, const Point2D& start) {
    const std::size_t n = points.size();

    // Candidato 1: vecino más cercano (en empates gana el índice menor, para que el resultado sea determinista).
    std::vector<std::size_t> nearest;
    nearest.reserve(n);
    std::vector<bool> used(n, false);
    Point2D current = start;
    for (std::size_t step = 0; step < n; ++step) {
        std::size_t best = n;
        double bestDist = std::numeric_limits<double>::infinity();
        for (std::size_t i = 0; i < n; ++i) {
            if (used[i]) continue;
            const double d = dist(current, points[i]);
            if (d < bestDist) {
                bestDist = d;
                best = i;
            }
        }
        used[best] = true;
        nearest.push_back(best);
        current = points[best];
    }

    // Candidato 2: el orden del archivo (los CAD suelen exportarlo ya ordenado en barridos).
    std::vector<std::size_t> fileOrder(n);
    for (std::size_t i = 0; i < n; ++i) fileOrder[i] = i;

    improveTwoOpt(points, nearest, start);
    improveTwoOpt(points, fileOrder, start);
    return pathLength(points, nearest, start) <= pathLength(points, fileOrder, start) ? nearest : fileOrder;
}

}  // namespace flatcam
