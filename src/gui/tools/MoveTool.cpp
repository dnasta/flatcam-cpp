#include "gui/tools/MoveTool.h"

#include <limits>

#include "gui/App.h"
#include "gui/MainGUI.h"
#include "gui/PlotCanvas.h"
#include "gui/Translation.h"
#include "gui/objects/FlatCAMObj.h"

namespace flatcam::gui {

MoveTool::MoveTool(App* app, QObject* parent) : QObject(parent), app_(app) {}

void MoveTool::toggle() {
    PlotCanvas* canvas = app_->ui()->plotCanvas();
    if (visible_) {
        visible_ = false;
        for (auto& c : connections_) disconnect(c);
        connections_.clear();
        clickedMove_ = 0;
        canvas->removeLayer(kLayerId);
        app_->deleteSelectionShape();  // delete_shape borra toda la sel_shapes, también los recuadros de selección
        return;
    }
    visible_ = true;
    if (!app_->selectedObjects().empty()) {
        app_->ui()->inform(tr_("MOVE: Click on the Start point ..."));
        connections_.push_back(connect(canvas, &PlotCanvas::mouseMoved, this, [this](QPointF w) { onMove(w); }));
        connections_.push_back(connect(canvas, &PlotCanvas::mouseClicked, this,
                                       [this](QPointF w, Qt::MouseButton b) { onClick(w, b); }));
        drawSelectionBox();
    } else {
        toggle();
        app_->ui()->inform("[WARNING_NOTCL] " + tr_("Cancelled. No object(s) to move."));
    }
}

void MoveTool::cancel() {
    app_->ui()->inform("[WARNING_NOTCL] " + tr_("Cancelled."));
    toggle();
}

void MoveTool::onClick(const QPointF& pos, Qt::MouseButton button) {
    if (!visible_ || button != Qt::LeftButton) return;
    if (clickedMove_ == 0) {
        point1_ = pos;  // el original guarda además el anterior en point2, que no usa
        app_->ui()->inform(tr_("Click on the DESTINATION point ..."));
        clickedMove_ = 1;
        return;
    }
    app_->ui()->plotCanvas()->removeLayer(kLayerId);
    app_->deleteSelectionShape();
    const double dx = pos.x() - point1_->x(), dy = pos.y() - point1_->y();
    std::vector<FlatCAMObj*> objs;
    for (FlatCAMObj* o : app_->selectedObjects())
        if (o->plotEnabled()) objs.push_back(o);
    if (objs.empty()) {
        app_->ui()->inform("[ERROR_NOTCL] " + tr_("Failed.") + " " + tr_("No object is selected."));
    } else {
        for (FlatCAMObj* o : objs) {
            if (!o->offset(dx, dy)) continue;  // un CNC Job no se mueve (ver la clase)
            app_->objectChanged(o);
            app_->refreshSourceFile(o);  // source_file = export_gerber / export_excellon(local_use=...)
        }
        // str(sel_obj.kind).capitalize() del último objeto.
        static const char* kinds[] = {"Gerber", "Excellon", "Geometry", "Cncjob", "Script", "Document"};
        app_->ui()->inform(QString("[success] %1 %2 ...")
                               .arg(kinds[static_cast<int>(objs.back()->kind())], tr_("object was moved")));
    }
    clickedMove_ = 0;
    toggle();
}

void MoveTool::onMove(const QPointF& pos) {
    if (!visible_ || clickedMove_ != 1) return;
    app_->deleteSelectionShape();  // update_sel_bbox empieza con delete_shape
    drawBox(pos.x() - point1_->x(), pos.y() - point1_->y());  // update_sel_bbox
}

void MoveTool::drawSelectionBox() {
    // draw_sel_bbox: la caja de los objetos elegidos que se dibujan.
    double xmin = std::numeric_limits<double>::infinity(), ymin = xmin, xmax = -xmin, ymax = -xmin;
    for (FlatCAMObj* o : app_->selectedObjects()) {
        if (!o->plotEnabled()) continue;
        const QRectF b = o->bounds();
        xmin = std::min(xmin, b.left());
        ymin = std::min(ymin, b.top());
        xmax = std::max(xmax, b.right());
        ymax = std::max(ymax, b.bottom());
    }
    oldCoords_.clear();
    if (xmin > xmax) return;  // ninguno se dibuja: el original falla en min([])
    oldCoords_ = {{xmin, ymin}, {xmax, ymin}, {xmax, ymax}, {xmin, ymax}};
    drawBox(0, 0);
}

void MoveTool::drawBox(double dx, double dy) {
    if (oldCoords_.empty()) return;
    std::vector<Point2D> pts;
    for (const Point2D& p : oldCoords_) pts.push_back({p.x + dx, p.y + dy});
    // draw_shape (mm): buffer(-0.1) y buffer(0.2): esquinas redondeadas; relleno #0000FF33, contorno #0000FFAF.
    const Shape shape = Shape::polygon(pts).buffer(-0.1).buffer(0.2);
    PlotLayerData d;
    d.fillColor = colorFromFlatCam("#0000FF33");
    d.lineColor = colorFromFlatCam("#0000FFAF");
    PlotGroup g;
    QRectF bounds;
    FlatCAMObj::addShapeTo(shape, g, bounds, true);
    d.fill = std::move(g.fill);
    d.lines = std::move(g.lines);
    app_->ui()->plotCanvas()->setLayer(kLayerId, std::move(d));
}

}  // namespace flatcam::gui
