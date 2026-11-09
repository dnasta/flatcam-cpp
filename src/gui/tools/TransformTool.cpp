#include "gui/Preferences.h"
#include "gui/tools/TransformTool.h"

#include <QApplication>
#include <QClipboard>
#include <limits>

#include "gui/App.h"
#include "gui/MainGUI.h"
#include "gui/Translation.h"
#include "gui/objects/FlatCAMObj.h"
#include "gui/objects/ObjectCollection.h"
#include "gui/widgets/NumericalEval.h"

namespace flatcam::gui {

TransformTool::TransformTool(App* app, QWidget* parent) : TransformToolUIBase(parent), app_(app) {
    setupUi();
    connect(ref_combo, &QComboBox::currentIndexChanged, this, [this](int i) { onReferenceChanged(i); });
    connect(type_obj_combo, &QComboBox::currentIndexChanged, this, [this] { fillObjects(); });
    connect(point_button, &QPushButton::clicked, this, [this] {
        point_entry->setText(QApplication::clipboard()->text());  // on_add_coords
    });
    connect(rotate_button, &QPushButton::clicked, this, [this] { onRotate(); });
    connect(skewx_button, &QPushButton::clicked, this, [this] { onSkewX(); });
    connect(skewy_button, &QPushButton::clicked, this, [this] { onSkewY(); });
    connect(scalex_button, &QPushButton::clicked, this, [this] { onScaleX(); });
    connect(scaley_button, &QPushButton::clicked, this, [this] { onScaleY(); });
    connect(offx_button, &QPushButton::clicked, this, [this] { onOffsetX(); });
    connect(offy_button, &QPushButton::clicked, this, [this] { onOffsetY(); });
    connect(flipx_button, &QPushButton::clicked, this, [this] { onFlipX(); });
    connect(flipy_button, &QPushButton::clicked, this, [this] { onFlipY(); });
    connect(buffer_button, &QPushButton::clicked, this, [this] { onBufferDistance(); });
    connect(buffer_factor_button, &QPushButton::clicked, this, [this] { onBufferFactor(); });
    connect(reset_button, &QPushButton::clicked, this, [this] { setToolUi(); });
    // OptionalInputSection con logic=False: con "Enlazar" la Y queda deshabilitada (usa el valor de la X).
    auto link = [](FCCheckBox* cb, std::initializer_list<QWidget*> ws) {
        const std::vector<QWidget*> v(ws);
        auto apply = [v](bool on) {
            for (QWidget* w : v) w->setEnabled(!on);
        };
        apply(cb->isChecked());
        QObject::connect(cb, &QCheckBox::toggled, cb, apply);
    };
    link(skew_link_cb, {skewy_label, skewy_entry, skewy_button});
    link(scale_link_cb, {scaley_label, scaley_entry, scaley_button});
}

ToolInfo TransformTool::info() { return {tr_("Object Transform"), tr_("Transform Tool")}; }

void TransformTool::setToolUi() {
    // tools_transform_* de las preferencias (de fábrica: Selección, 90°, escala 1, engrosar 0 con 100 %).
    ref_combo->setValue(defStr("tools_transform_reference"));
    type_obj_combo->setValue(defStr("tools_transform_ref_object"));
    point_entry->setText(defStr("tools_transform_ref_point"));  // EvalEntry.set_value: str(val)
    rotate_entry->setValue(defNum("tools_transform_rotate"));
    skewx_entry->setValue(defNum("tools_transform_skew_x"));
    skewy_entry->setValue(defNum("tools_transform_skew_y"));
    skew_link_cb->setChecked(defBool("tools_transform_skew_link"));
    scalex_entry->setValue(defNum("tools_transform_scale_x"));
    scaley_entry->setValue(defNum("tools_transform_scale_y"));
    scale_link_cb->setChecked(defBool("tools_transform_scale_link"));
    offx_entry->setValue(defNum("tools_transform_offset_x"));
    offy_entry->setValue(defNum("tools_transform_offset_y"));
    buffer_entry->setValue(defNum("tools_transform_buffer_dis"));
    buffer_factor_entry->setValue(defNum("tools_transform_buffer_factor"));
    buffer_rounded_cb->setChecked(defBool("tools_transform_buffer_corner"));
    onReferenceChanged(ref_combo->currentIndex());
    fillObjects();
}

void TransformTool::onReferenceChanged(int index) {
    // TransformUI.on_reference_changed: 0 Origen, 1 Selección, 2 Punto, 3 Objeto.
    for (QWidget* w : std::initializer_list<QWidget*>{point_label, point_entry, point_button}) w->setVisible(index == 2);
    for (QWidget* w : std::initializer_list<QWidget*>{type_object_label, type_obj_combo, object_combo})
        w->setVisible(index == 3);
}

void TransformTool::fillObjects() {
    const ObjectKind kinds[] = {ObjectKind::Gerber, ObjectKind::Excellon, ObjectKind::Geometry};
    const ObjectKind kind = kinds[std::clamp(type_obj_combo->currentIndex(), 0, 2)];
    object_combo->clear();
    for (FlatCAMObj* o : app_->collection()->objects())
        if (o->kind() == kind) object_combo->addItem(o->name());
    object_combo->setCurrentIndex(0);
}

std::optional<Point2D> TransformTool::reference() {
    // on_calculate_reference
    switch (ref_combo->currentIndex()) {
        case 0: return Point2D{0, 0};
        case 1: {
            // alt_bounds de la selección (sin los CNC Job), y su centro.
            double xmin = std::numeric_limits<double>::infinity(), ymin = xmin, xmax = -xmin, ymax = -xmin;
            const auto sel = app_->selectedObjects();
            if (sel.empty()) {
                app_->ui()->inform("[ERROR_NOTCL] " + tr_("No object is selected."));
                return std::nullopt;
            }
            for (FlatCAMObj* o : sel) {
                if (o->kind() == ObjectKind::CNCJob) continue;
                const QRectF b = o->bounds();
                xmin = std::min(xmin, b.left());
                ymin = std::min(ymin, b.top());
                xmax = std::max(xmax, b.right());
                ymax = std::max(ymax, b.bottom());
            }
            return Point2D{(xmax + xmin) * 0.5, (ymax + ymin) * 0.5};
        }
        case 2: {
            // El original lee `self.uipoint_entry` (error de tipeo: falla y no hace nada); el port lee el campo.
            const auto v = NumericExpression::evaluateTuple(point_entry->text());
            if (!v || v->size() != 2) {
                app_->ui()->inform("[WARNING_NOTCL] " + tr_("Incorrect format for Point value. Needs format X,Y"));
                return std::nullopt;
            }
            return Point2D{(*v)[0], (*v)[1]};
        }
        default: {
            FlatCAMObj* o = app_->collection()->byName(object_combo->currentText());
            if (!o) return std::nullopt;
            const QRectF b = o->bounds();
            return Point2D{(b.right() + b.left()) * 0.5, (b.bottom() + b.top()) * 0.5};
        }
    }
}

namespace {
/// Para cada objeto elegido: los CNC Job avisan y no cambian; los demás se transforman y se vuelven a dibujar.
template <typename Fn>
bool forSelected(App* app, const QString& cncMessage, Fn fn) {
    const auto sel = app->selectedObjects();
    if (sel.empty()) {
        app->ui()->inform("[WARNING_NOTCL] " + tr_("No object is selected."));
        return false;
    }
    for (FlatCAMObj* o : sel) {
        if (o->kind() == ObjectKind::CNCJob) {
            app->ui()->inform(cncMessage);
            continue;
        }
        fn(o);
        app->objectChanged(o);
    }
    return true;
}
}  // namespace

void TransformTool::onRotate() {
    const double value = rotate_entry->value();
    if (value == 0) {
        app_->ui()->inform("[WARNING_NOTCL] " + tr_("Rotate transformation can not be done for a value of 0."));
        return;
    }
    const auto p = reference();
    if (!p) return;
    // sel_obj.rotate(-num): un ángulo positivo gira en sentido horario.
    if (forSelected(app_, tr_("CNCJob objects can't be rotated."), [&](FlatCAMObj* o) { o->rotate(-value, *p); }))
        app_->ui()->inform("[success] " + tr_("Rotate done") + "...");
}

void TransformTool::onFlipX() {
    const auto p = reference();
    if (!p) return;
    forSelected(app_, tr_("CNCJob objects can't be mirrored/flipped."), [&](FlatCAMObj* o) {
        o->mirror(Axis::Y, *p);
        app_->ui()->inform("[success] " + tr_("Flip on X axis done") + "...");
    });
}

void TransformTool::onFlipY() {
    const auto p = reference();
    if (!p) return;
    forSelected(app_, tr_("CNCJob objects can't be mirrored/flipped."), [&](FlatCAMObj* o) {
        o->mirror(Axis::X, *p);
        app_->ui()->inform("[success] " + tr_("Flip on Y axis done") + "...");
    });
}

void TransformTool::skewAction(char axis, double ax, double ay) {
    if (ax == 90 || ax == 180 || ay == 90 || ay == 180 || (ax == 0 && ay == 0)) {
        app_->ui()->inform("[WARNING_NOTCL] " + tr_("Skew transformation can not be done for 0, 90 and 180 degrees."));
        return;
    }
    const auto p = reference();
    if (!p) return;
    if (forSelected(app_, tr_("CNCJob objects can't be skewed."), [&](FlatCAMObj* o) { o->skew(ax, ay, *p); }))
        app_->ui()->inform(QString("[success] %1 %2 %3...").arg(tr_("Skew on the")).arg(QChar(axis)).arg(tr_("axis done")));
}

void TransformTool::onSkewX() {
    const double x = skewx_entry->value();
    if (x == 0) return;
    skewAction('X', x, skew_link_cb->isChecked() ? x : 0);
}

void TransformTool::onSkewY() {
    const double y = skewy_entry->value();
    if (y == 0) return;
    skewAction('Y', 0, y);
}

void TransformTool::scaleAction(char axis, double fx, double fy) {
    const auto p = reference();
    if (!p) return;
    if (forSelected(app_, tr_("CNCJob objects can't be scaled."), [&](FlatCAMObj* o) { o->scale(fx, fy, *p); }))
        app_->ui()->inform(QString("[success] %1 %2 %3...").arg(tr_("Scale on the")).arg(QChar(axis)).arg(tr_("axis done")));
}

void TransformTool::onScaleX() {
    const double x = scalex_entry->value();
    if (x == 0 || x == 1) {
        app_->ui()->inform("[WARNING_NOTCL] " + tr_("Scale transformation can not be done for a factor of 0 or 1."));
        return;
    }
    scaleAction('X', x, scale_link_cb->isChecked() ? x : 1);
}

void TransformTool::onScaleY() {
    const double y = scaley_entry->value();
    if (y == 0 || y == 1) {
        app_->ui()->inform("[WARNING_NOTCL] " + tr_("Scale transformation can not be done for a factor of 0 or 1."));
        return;
    }
    scaleAction('Y', 1, y);
}

void TransformTool::onOffsetX() {
    const double v = offx_entry->value();
    if (v == 0) {
        app_->ui()->inform("[WARNING_NOTCL] " + tr_("Offset transformation can not be done for a value of 0."));
        return;
    }
    if (forSelected(app_, tr_("CNCJob objects can't be offset."), [&](FlatCAMObj* o) { o->offset(v, 0); }))
        app_->ui()->inform(QString("[success] %1 X %2...").arg(tr_("Offset on the"), tr_("axis done")));
}

void TransformTool::onOffsetY() {
    const double v = offy_entry->value();
    if (v == 0) {
        app_->ui()->inform("[WARNING_NOTCL] " + tr_("Offset transformation can not be done for a value of 0."));
        return;
    }
    if (forSelected(app_, tr_("CNCJob objects can't be offset."), [&](FlatCAMObj* o) { o->offset(0, v); }))
        app_->ui()->inform(QString("[success] %1 Y %2...").arg(tr_("Offset on the"), tr_("axis done")));
}

void TransformTool::bufferAction(double value, bool rounded, bool factor) {
    const JoinStyle join = rounded ? JoinStyle::Round : JoinStyle::Mitre;
    if (forSelected(app_, tr_("CNCJob objects can't be buffered."),
                    [&](FlatCAMObj* o) { o->buffer(value, join, factor); }))
        app_->ui()->inform("[success] " + tr_("Buffer done") + "...");
}

void TransformTool::onBufferDistance() { bufferAction(buffer_entry->value(), buffer_rounded_cb->isChecked(), false); }

void TransformTool::onBufferFactor() {
    bufferAction(1 + buffer_factor_entry->value() / 100.0, buffer_rounded_cb->isChecked(), true);
}

}  // namespace flatcam::gui
