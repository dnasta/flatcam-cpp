#include "cam/EndpointStorage.h"

#include <algorithm>
#include <cmath>
#include <limits>

namespace flatcam {

std::size_t EndpointStorage::insert(std::vector<Point2D> points, bool linearRing) {
    lines_.push_back(Line{std::move(points), linearRing});
    removed_.push_back(false);
    ++alive_;
    gridDirty_ = true;
    return lines_.size() - 1;
}

void EndpointStorage::remove(std::size_t index) {
    if (index < removed_.size() && !removed_[index]) {
        removed_[index] = true;
        --alive_;
    }
}

std::vector<EndpointStorage::Line> EndpointStorage::objects() const {
    std::vector<Line> out;
    for (std::size_t i = 0; i < lines_.size(); ++i) {
        if (!removed_[i]) out.push_back(lines_[i]);
    }
    return out;
}

void EndpointStorage::rebuildGrid() const {
    cells_.clear();
    if (lines_.empty()) {
        gridDirty_ = false;
        return;
    }
    double minX = std::numeric_limits<double>::infinity(), minY = minX;
    double maxX = -minX, maxY = -minX;
    std::size_t entries = 0;
    for (std::size_t i = 0; i < lines_.size(); ++i) {
        if (removed_[i] || lines_[i].points.empty()) continue;
        for (const Point2D* p : {&lines_[i].points.front(), &lines_[i].points.back()}) {
            minX = std::min(minX, p->x);
            maxX = std::max(maxX, p->x);
            minY = std::min(minY, p->y);
            maxY = std::max(maxY, p->y);
        }
        entries += 2;
    }
    if (entries == 0) {
        gridDirty_ = false;
        return;
    }
    const double w = std::max(maxX - minX, 1e-9), h = std::max(maxY - minY, 1e-9);
    // Unas 2 entradas por celda en promedio.
    cell_ = std::max(std::sqrt(w * h / std::max<double>(1.0, static_cast<double>(entries) / 2.0)), std::max(w, h) / 2048.0);
    nx_ = static_cast<int>(w / cell_) + 1;
    ny_ = static_cast<int>(h / cell_) + 1;
    originX_ = minX;
    originY_ = minY;
    cells_.assign(static_cast<std::size_t>(nx_) * static_cast<std::size_t>(ny_), {});
    for (std::size_t i = 0; i < lines_.size(); ++i) {
        if (removed_[i] || lines_[i].points.empty()) continue;
        for (int end = 0; end < 2; ++end) {
            const Point2D& p = end == 0 ? lines_[i].points.front() : lines_[i].points.back();
            const int cx = std::min(nx_ - 1, static_cast<int>((p.x - originX_) / cell_));
            const int cy = std::min(ny_ - 1, static_cast<int>((p.y - originY_) / cell_));
            cells_[static_cast<std::size_t>(cy) * static_cast<std::size_t>(nx_) + static_cast<std::size_t>(cx)].push_back(2 * i + static_cast<std::size_t>(end));
        }
    }
    gridDirty_ = false;
}

std::optional<EndpointStorage::Nearest> EndpointStorage::nearest(const Point2D& q) const {
    if (alive_ == 0) return std::nullopt;
    if (gridDirty_) rebuildGrid();

    const int qx = std::clamp(static_cast<int>(std::floor((q.x - originX_) / cell_)), 0, nx_ - 1);
    const int qy = std::clamp(static_cast<int>(std::floor((q.y - originY_) / cell_)), 0, ny_ - 1);

    double bestD2 = std::numeric_limits<double>::infinity();
    std::size_t bestEntry = std::numeric_limits<std::size_t>::max();
    const int maxRing = std::max(nx_, ny_);
    for (int r = 0; r <= maxRing; ++r) {
        // Una vez conocido un candidato, el resto de los anillos queda a una distancia de al menos (r - 1) celdas.
        if (r > 0 && bestD2 < std::pow((r - 1) * cell_, 2)) break;
        for (int cy = qy - r; cy <= qy + r; ++cy) {
            if (cy < 0 || cy >= ny_) continue;
            const bool edgeRow = (cy == qy - r) || (cy == qy + r);
            for (int cx = qx - r; cx <= qx + r; ++cx) {
                if (cx < 0 || cx >= nx_) continue;
                if (!edgeRow && cx != qx - r && cx != qx + r) continue;  // solo el borde del anillo
                std::vector<std::size_t>& cell = cells_[static_cast<std::size_t>(cy) * static_cast<std::size_t>(nx_) + static_cast<std::size_t>(cx)];
                for (std::size_t k = 0; k < cell.size();) {
                    const std::size_t entry = cell[k];
                    const std::size_t lineIdx = entry / 2;
                    if (removed_[lineIdx]) {  // borrado perezoso
                        cell[k] = cell.back();
                        cell.pop_back();
                        continue;
                    }
                    const Point2D& p = (entry % 2 == 0) ? lines_[lineIdx].points.front() : lines_[lineIdx].points.back();
                    const double dx = p.x - q.x, dy = p.y - q.y;
                    const double d2 = dx * dx + dy * dy;
                    if (d2 < bestD2 || (d2 == bestD2 && entry < bestEntry)) {
                        bestD2 = d2;
                        bestEntry = entry;
                    }
                    ++k;
                }
            }
        }
    }
    if (bestEntry == std::numeric_limits<std::size_t>::max()) return std::nullopt;
    const std::size_t lineIdx = bestEntry / 2;
    const Point2D p = (bestEntry % 2 == 0) ? lines_[lineIdx].points.front() : lines_[lineIdx].points.back();
    return Nearest{p, lineIdx};
}

}  // namespace flatcam
