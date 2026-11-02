#include "gui/CanvasPicker.h"

#include "gui/MainGUI.h"
#include "gui/PlotCanvas.h"
#include "gui/Translation.h"
#include "gui/objects/FlatCAMObj.h"

namespace flatcam::gui {

CanvasPicker::CanvasPicker(MainGUI* ui, QObject* parent) : QObject(parent), ui_(ui) {
    connect(ui_->plotCanvas(), &PlotCanvas::mouseReleased, this,
            [this](QPointF w, Qt::MouseButton b, bool dragged) { onRelease(w, b, dragged); });
}

void CanvasPicker::start(Mode mode, ShapeList candidates, std::function<void(const ShapeList&)> done) {
    cancel();
    active_ = true;
    mode_ = mode;
    candidates_ = std::move(candidates);
    done_ = std::move(done);
    selected_.clear();
    points_.clear();
}

bool CanvasPicker::cancel() {
    if (!active_) return false;
    active_ = false;
    selected_.clear();
    points_.clear();
    ui_->plotCanvas()->removeLayer(kLayerId);
    return true;
}

void CanvasPicker::onRelease(const QPointF& world, Qt::MouseButton button, bool dragged) {
    if (!active_) return;
    const Point2D p{world.x(), world.y()};
    if (button == Qt::LeftButton) {
        if (mode_ == Mode::Rectangles) {
            if (points_.empty()) {
                points_.push_back(p);
                ui_->inform("[WARNING_NOTCL] " + tr_("Click the end point of the area."));
            } else {
                const Point2D a = points_.front();
                selected_.push_back(Shape::polygon({a, {p.x, a.y}, p, {a.x, p.y}}));
                points_.clear();
                ui_->inform(tr_("Zone added. Click to start adding next zone or right click to finish."));
            }
        } else if (mode_ == Mode::Polygons) {
            points_.push_back(p);
            if (points_.size() > 1) ui_->inform(tr_("Click on next Point or click right mouse button to complete ..."));
        } else {
            // find_polygon: el polígono que contiene el punto; marcar o desmarcar.
            const Shape pt = Shape::point(p);
            const Shape* hit = nullptr;
            for (const Shape& c : candidates_)
                if (pt.within(c)) hit = &c;
            if (!hit) {
                ui_->inform(tr_("No polygon detected under click position."));
            } else {
                auto it = std::find_if(selected_.begin(), selected_.end(),
                                       [&](const Shape& s) { return s.toWkt() == hit->toWkt(); });
                if (it == selected_.end()) {
                    selected_.push_back(*hit);
                    ui_->inform(QString("%1: %2. %3")
                                    .arg(tr_("Added polygon"))
                                    .arg(selected_.size())
                                    .arg(tr_("Click to add next polygon or right click to start.")));
                } else {
                    selected_.erase(it);
                    ui_->inform(QString("%1. %2").arg(tr_("Removed polygon"),
                                                      tr_("Click to add/remove next polygon or right click to start.")));
                }
            }
        }
        redraw();
        return;
    }
    if (button != Qt::RightButton || dragged) return;
    if (mode_ == Mode::Polygons && points_.size() > 1) {
        // Cierra el polígono en curso (con el punto del clic derecho si es distinto del último).
        if (!(points_.back() == p)) points_.push_back(p);
        if (points_.size() > 2) {
            const Shape pol = Shape::polygon(points_);
            if (pol.isValid()) {
                selected_.push_back(pol);
                ui_->inform(tr_("Zone added. Click to start adding next zone or right click to finish."));
            }
        }
        points_.clear();
        redraw();
        return;
    }
    finish();
}

void CanvasPicker::finish() {
    const ShapeList chosen = selected_;
    auto done = done_;
    const Mode mode = mode_;
    cancel();
    if (chosen.empty()) {
        if (mode == Mode::PickPolygons)
            ui_->inform("[ERROR_NOTCL] " + tr_("List of single polygons is empty. Aborting."));
        return;
    }
    if (done) done(chosen);
}

void CanvasPicker::redraw() {
    PlotLayerData d;
    d.fillColor = colorFromFlatCam("#a5a5ffbf");  // global_sel_fill
    d.lineColor = colorFromFlatCam("#0000ffbf");  // global_sel_line
    PlotGroup g;
    QRectF bounds;
    for (const Shape& s : selected_) FlatCAMObj::addShapeTo(s, g, bounds, true);
    if (points_.size() >= 2) FlatCAMObj::addShapeTo(Shape::lineString(points_), g, bounds, false);
    d.fill = std::move(g.fill);
    d.lines = std::move(g.lines);
    ui_->plotCanvas()->setLayer(kLayerId, std::move(d));
}

}  // namespace flatcam::gui
