// Selección de objetos con el mouse en el canvas: on_mouse_click_over_plot, on_mouse_move_over_plot,
// on_mouse_click_release_over_plot, selection_area_handler, select_objects, draw_selection_shape y
// draw_moving_selection_shape de app_Main.py.
#include <QApplication>
#include <QClipboard>
#include <QSplitter>

#include "gui/App.h"
#include "gui/CanvasPicker.h"
#include "gui/MainGUI.h"
#include "gui/PlotCanvas.h"
#include "gui/Translation.h"
#include "gui/objects/FlatCAMObj.h"
#include "gui/objects/ObjectCollection.h"
#include "gui/tools/DblSidedTool.h"
#include "gui/tools/MoveTool.h"

namespace flatcam::gui {

namespace {

/// Un rectángulo (o la figura) convertido en grupo de dibujo con sus colores.
PlotGroup groupOf(const Shape& s, const char* line, const char* fill) {
    PlotGroup g;
    QRectF ignored;
    FlatCAMObj::addShapeTo(s, g, ignored, true);
    g.lineColor = colorFromFlatCam(line);
    g.fillColor = colorFromFlatCam(fill);
    return g;
}

Shape rectangle(double x0, double y0, double x1, double y1) {
    return Shape::polygon({{x0, y0}, {x1, y0}, {x1, y1}, {x0, y1}});
}

}  // namespace

void App::onCanvasPress(const QPointF& world, Qt::MouseButton button) {
    if (!ui_->canvasHandlersEnabled()) return;
    // self.pos: la posición del clic (ajustada a la grilla); rel_point1 la guarda MainGUI.
    if (button == Qt::LeftButton) lastClick_ = world;
}

void App::onCanvasMove(const QPointF& world, bool leftDragging) {
    if (!ui_->canvasHandlersEnabled()) return;
    mouse_ = world;
    // global_selection_shape = True. Con el botón izquierdo arrastrando: el rectángulo de selección.
    if (leftDragging) {
        deleteSelectionShape();
        const double dx = world.x() - lastClick_.x();
        const Shape r = rectangle(lastClick_.x(), lastClick_.y(), world.x(), world.y());
        if (dx < 0) {
            // global_alt_sel_line / global_alt_sel_fill con transparencia 0,3 (hex(int(0.3 * 255)) = 4c).
            movingSelection_ = groupOf(r, "#006E20BF", "#BBF2684c");
            selectionType_ = false;
        } else {
            movingSelection_ = groupOf(r, "#0000ffbf", "#a5a5ff4c");
            selectionType_ = true;
        }
        redrawSelectionShapes();
    } else {
        selectionType_.reset();
    }
}

void App::onCanvasDoubleClick(Qt::MouseButton button) {
    if (button == Qt::LeftButton) doubleClick_ = true;
}

void App::onCanvasRelease(const QPointF& world, Qt::MouseButton button, Qt::KeyboardModifiers modifiers, bool dragged) {
    if (!ui_->canvasHandlersEnabled()) return;
    // Las herramientas que eligen con el mouse desconectan este manejador mientras lo hacen.
    if (picker_->active() || dblsidedTool->picking()) return;
    // El clic derecho sin desplazar la vista abre el menú del canvas, con la lista de grillas al día.
    if (button == Qt::RightButton && !dragged) {
        popupCanvasMenu();
        return;
    }
    if (button != Qt::LeftButton) return;
    if (modifiers == Qt::ShiftModifier) {
        clickNoProject_ = true;
        // global_point_clipboard_format "(%.*f, %.*f)" con el clic (self.pos).
        QApplication::clipboard()->setText(QString::asprintf("(%.4f, %.4f)", lastClick_.x(), lastClick_.y()));
        ui_->inform("[success] " + tr_("Coordinates copied to clipboard."));
        return;
    }
    if (doubleClick_) {
        doubleClick_ = false;
        if (!selectedObjects().empty()) {
            ui_->notebook->setCurrentWidget(ui_->properties_tab);
            if (ui_->splitter->sizes().value(0) == 0) ui_->splitter->setSizes({1, 1});
            deleteSelectionShape();
        }
        return;
    }
    if (selectionType_) {
        const bool enclosure = *selectionType_;
        selectionType_.reset();
        movingSelection_.reset();
        selectionAreaHandler(lastClick_, world, enclosure);
        return;
    }
    // global_mselect_key = "Control" (en el Mac, la tecla Cmd).
    if (!moveTool->active()) selectObjects(modifiers == Qt::ControlModifier);  // command_active is None
}

void App::setActive(FlatCAMObj* obj, bool active) {
    ui_->collection_view->selectionModel()->select(collection_->indexOf(obj), active ? QItemSelectionModel::Select
                                                                                     : QItemSelectionModel::Deselect);
}

void App::selectionAreaHandler(const QPointF& start, const QPointF& end, bool enclosure) {
    const Shape sel = rectangle(start.x(), start.y(), end.x(), end.y());
    deleteSelectionShape();
    for (FlatCAMObj* o : collection_->objects()) setActive(o, false);  // set_all_inactive
    for (FlatCAMObj* o : collection_->objects()) {
        if (!o->plotEnabled()) continue;
        const QRectF b = o->bounds();
        Shape poly = (b.left() == b.right() || b.top() == b.bottom())
                         ? Shape::unaryUnion(o->solidShapes()).buffer(0.001)
                         : rectangle(b.left(), b.top(), b.right(), b.bottom());
        if (poly.isEmpty() || !poly.isValid()) continue;
        if (enclosure ? poly.within(sel) : sel.intersects(poly)) {
            drawSelectionShape(o);
            setActive(o, true);
        }
        shapeDrawn_.insert(o);
    }
}

void App::selectObjects(bool multisel) {
    if (!multisel) objectsUnderClick_.clear();
    const double x = lastClick_.x(), y = lastClick_.y();
    for (FlatCAMObj* o : collection_->objects()) {
        if (o->kind() == ObjectKind::Script || o->kind() == ObjectKind::Document) continue;
        if (multisel && objectsUnderClick_.contains(o->name())) continue;
        const QRectF b = o->bounds();
        if (x >= b.left() && x <= b.right() && y >= b.top() && y <= b.bottom() && !objectsUnderClick_.contains(o->name()) &&
            o->plotEnabled())
            objectsUnderClick_.push_back(o->name());
    }
    auto clearSelection = [this] {
        // on_objects_selection(False)
        const bool any = !collection_->objects().empty();
        for (FlatCAMObj* o : collection_->objects()) setActive(o, false);
        for (QAction* a : ui_->menuobjects->actions()) a->setChecked(false);
        ui_->inform(any ? tr_("Objects selection is cleared.") : QString());
    };
    if (objectsUnderClick_.isEmpty()) {
        clearSelection();
        deleteSelectionShape();
        shapeDrawn_.clear();
        if (!clickNoProject_) {
            if (ui_->notebook->currentWidget() != ui_->tool_tab) ui_->notebook->setCurrentWidget(ui_->project_tab);
        } else {
            clickNoProject_ = false;
        }
        return;
    }
    FlatCAMObj* cur = activeObject();
    if (objectsUnderClick_.size() == 1) {
        FlatCAMObj* under = collection_->byName(objectsUnderClick_.front());
        if (!cur) {
            setActive(under, true);
            cur = activeObject();
            drawSelectionShape(cur);
            shapeDrawn_.insert(cur);
        } else if (!objectsUnderClick_.contains(cur->name())) {
            clearSelection();
            deleteSelectionShape();
            shapeDrawn_.erase(cur);
            setActive(under, true);
            cur = activeObject();
            drawSelectionShape(cur);
            shapeDrawn_.insert(cur);
            selectedMessage(cur);
        } else if (!shapeDrawn_.count(cur)) {
            drawSelectionShape(cur);
            shapeDrawn_.insert(cur);
        } else {
            // Un clic sobre el objeto ya elegido: se quita la selección (y el original igual avisa que está elegido).
            clearSelection();
            deleteSelectionShape();
        }
        selectedMessage(cur);
        return;
    }
    // Varios objetos debajo del clic: cada clic pasa al siguiente.
    if (!activeObject()) {
        FlatCAMObj* first = collection_->byName(objectsUnderClick_.front());
        setActive(first, true);
        shapeDrawn_.insert(first);
    }
    QString name = activeObject()->name();
    if (!objectsUnderClick_.contains(name)) {
        setActive(activeObject(), false);
        setActive(collection_->byName(objectsUnderClick_.front()), true);
    } else {
        const int idx = static_cast<int>(objectsUnderClick_.indexOf(name));
        for (FlatCAMObj* o : collection_->objects()) setActive(o, false);
        setActive(collection_->byName(objectsUnderClick_[(idx + 1) % objectsUnderClick_.size()]), true);
    }
    cur = activeObject();
    deleteSelectionShape();
    shapeDrawn_.erase(cur);
    drawSelectionShape(cur);
    shapeDrawn_.insert(cur);
    selectedMessage(cur);
}

void App::selectedMessage(FlatCAMObj* obj) {
    if (!obj || obj->kind() == ObjectKind::Script || obj->kind() == ObjectKind::Document) return;
    static const char* colors[] = {"green", "brown", "red", "blue"};  // Gerber, Excellon, Geometría, CNC Job
    ui_->inform(QString("[selected]<span style=\"color:%1;\">%2</span> %3")
                    .arg(colors[static_cast<int>(obj->kind())], obj->name(), tr_("selected")));
}

void App::deleteSelectionShape() {
    selShapes_.clear();
    movingSelection_.reset();
    ui_->plotCanvas()->removeLayer(kSelectionLayerId);
}

void App::drawSelectionShape(FlatCAMObj* obj) {
    if (!obj) return;
    const QRectF b = obj->bounds();
    const Shape rect = (b.left() == b.right() || b.top() == b.bottom())
                           ? Shape::unaryUnion(obj->solidShapes()).buffer(0.100001)
                           : rectangle(b.left(), b.top(), b.right(), b.bottom());
    // mm: buffer(-0.1) y buffer(0.2); global_sel_fill con 0,2 de transparencia y global_sel_line con 0,8.
    Shape buffered = rect.buffer(-0.1).buffer(0.2);
    if (buffered.isEmpty() || !buffered.isValid()) buffered = rect;
    selShapes_.push_back(groupOf(buffered, "#0000ffcc", "#a5a5ff33"));
    redrawSelectionShapes();
}

void App::redrawSelectionShapes() {
    PlotLayerData d;
    d.groups = selShapes_;
    if (movingSelection_) d.groups.push_back(*movingSelection_);
    if (d.groups.empty()) {
        ui_->plotCanvas()->removeLayer(kSelectionLayerId);
        return;
    }
    ui_->plotCanvas()->setLayer(kSelectionLayerId, std::move(d));
}

}  // namespace flatcam::gui
