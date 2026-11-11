#include "gui/tools/QRCodeTool.h"

#include <QColorDialog>
#include <QFile>
#include <QFileDialog>
#include <QImage>
#include <QPainter>

#include "gui/App.h"
#include "gui/Preferences.h"
#include "gui/Translation.h"
#include "gui/objects/FlatCAMObj.h"
#include "gui/objects/ObjectCollection.h"

namespace flatcam::gui {

QRCodeTool::QRCodeTool(App* app, QWidget* parent) : QRCodeToolUIBase(parent), app_(app) {
    setupUi();
    connect(qrcode_button, &QPushButton::clicked, this, [this] { execute(); });
    connect(export_cb, &QCheckBox::checkStateChanged, this, [this](Qt::CheckState state) {
        export_frame->setVisible(state != Qt::Unchecked);
        qrcode_button->setVisible(state == Qt::Unchecked);
    });
    connect(export_png_button, &QPushButton::clicked, this, [this] {
        if (text_data->value().isEmpty()) {
            app_->ui()->inform("[ERROR_NOTCL] " + tr_("Cancelled. There is no QRCode Data in the text box."));
            return;
        }
        const QString f = QFileDialog::getSaveFileName(app_->ui(), tr_("Export PNG"), app_->lastFolder() + "/qr_code_png",
                                                       "PNG File (*.png);;All Files (*.*)");
        if (f.isEmpty()) app_->ui()->inform("[WARNING_NOTCL] " + tr_("Cancelled."));
        else exportPng(f);
    });
    connect(export_svg_button, &QPushButton::clicked, this, [this] {
        if (text_data->value().isEmpty()) {
            app_->ui()->inform("[ERROR_NOTCL] " + tr_("Cancelled. There is no QRCode Data in the text box."));
            return;
        }
        const QString f = QFileDialog::getSaveFileName(app_->ui(), tr_("Export SVG"), app_->lastFolder() + "/qr_code_svg",
                                                       "SVG File (*.svg);;All Files (*.*)");
        if (f.isEmpty()) app_->ui()->inform("[WARNING_NOTCL] " + tr_("Cancelled."));
        else exportSvg(f);
    });
    // Los colores: el campo pinta el botón y el botón abre el selector.
    auto colorEntry = [](FCEntry* e, QPushButton* b) {
        connect(e, &QLineEdit::editingFinished, b, [e, b] { b->setStyleSheet("background-color:" + e->value()); });
    };
    colorEntry(fill_color_entry, fill_color_button);
    colorEntry(back_color_entry, back_color_button);
    auto colorButton = [this](FCEntry* e, QPushButton* b) {
        connect(b, &QPushButton::clicked, this, [e, b] {
            const QColor c = QColorDialog::getColor(QColor(e->value()));
            if (!c.isValid()) return;
            b->setStyleSheet("background-color:" + c.name());
            e->setValue(c.name());
        });
    };
    colorButton(fill_color_entry, fill_color_button);
    colorButton(back_color_entry, back_color_button);
    connect(transparent_cb, &QCheckBox::checkStateChanged, this,
            [this](Qt::CheckState s) { onTransparent(s != Qt::Unchecked); });
    connect(reset_button, &QPushButton::clicked, this, [this] { setToolUi(); });
}

ToolInfo QRCodeTool::info() { return {tr_("QRCode Tool"), tr_("QRCode Tool")}; }

void QRCodeTool::setToolUi() {
    border_size_entry->setValue(4);
    version_entry->setValue(defInt("tools_qrcode_version", 1));
    error_radio->setValue(defVariant("tools_qrcode_error"));
    bsize_entry->setValue(defInt("tools_qrcode_box_size", 3));
    border_size_entry->setValue(defInt("tools_qrcode_border_size", 4));
    pol_radio->setValue(defVariant("tools_qrcode_polarity"));
    bb_radio->setValue(defVariant("tools_qrcode_rounded"));
    text_data->setValue(defStr("tools_qrcode_qrdata"));
    fill_color_entry->setValue(defStr("tools_qrcode_fill_color"));
    fill_color_button->setStyleSheet("background-color:" + defStr("tools_qrcode_fill_color").left(7));
    back_color_entry->setValue(defStr("tools_qrcode_back_color"));
    back_color_button->setStyleSheet("background-color:" + defStr("tools_qrcode_back_color").left(7));
    const QString cur = grb_object_combo->currentText();
    grb_object_combo->clear();
    for (FlatCAMObj* o : app_->collection()->objects())
        if (o->kind() == ObjectKind::Gerber) grb_object_combo->addItem(o->name());
    if (!cur.isEmpty()) grb_object_combo->setValue(cur);
}

std::optional<QrMatrix> QRCodeTool::makeMatrix() {
    try {
        const QString ec = error_radio->value().toString();
        return qrMake(text_data->value().toStdString(), version_entry->value(),
                      qrErrorCorrection(ec.isEmpty() ? 'M' : ec.at(0).toLatin1()));
    } catch (const QrError& e) {
        // El original falla en su hilo de trabajo (solo queda en el registro).
        app_->ui()->inform(QString("[ERROR_NOTCL] %1").arg(QString::fromStdString(e.what())));
        return std::nullopt;
    }
}

bool QRCodeTool::execute() {
    MainGUI* ui = app_->ui();
    if (text_data->value().isEmpty()) {
        ui->inform("[ERROR_NOTCL] " + tr_("Cancelled. There is no QRCode Data in the text box."));
        return false;
    }
    grb_ = app_->collection()->byName(grb_object_combo->currentText());
    if (!grb_ || grb_->kind() != ObjectKind::Gerber) {
        ui->inform("[WARNING_NOTCL] " + tr_("There is no Gerber object loaded ..."));
        return false;
    }
    const auto qr = makeMatrix();
    if (!qr) return false;
    geo_ = qrGeometry(*qr, bsize_entry->value(), border_size_entry->value(), defInt("geometry_circle_steps", 64));
    // Los manejadores del mouse de la aplicación siguen conectados, como en el original.
    disconnect(moveConn_);
    disconnect(releaseConn_);
    PlotCanvas* canvas = ui->plotCanvas();
    moveConn_ = connect(canvas, &PlotCanvas::mouseMoved, this, [this](QPointF w) { onMove(w); });
    releaseConn_ = connect(canvas, &PlotCanvas::mouseReleased, this,
                           [this](QPointF w, Qt::MouseButton b, bool) { onRelease(w, b); });
    placing_ = true;
    ui->inform(tr_("Click on the DESTINATION point ..."));
    return true;
}

void QRCodeTool::onMove(const QPointF& world) {
    if (!placing_ || !geo_) return;
    // draw_utility_geo: los contornos del código (o su caja, si tiene más módulos que tools_qrcode_sel_limit).
    PlotGroup g;
    QRectF bounds;
    ShapeList shapes;
    if (static_cast<int>(geo_->modules.size()) <= defInt("tools_qrcode_sel_limit", 330)) {
        const ShapeList parts = geo_->utility.parts();
        for (const Shape& p : parts.empty() ? ShapeList{geo_->utility} : parts) {
            shapes.push_back(p.exterior().translate(world.x(), world.y()));
            for (const Shape& in : p.interiors()) shapes.push_back(in.translate(world.x(), world.y()));
        }
    } else {
        const BoundingBox b = geo_->utility.bounds().value_or(BoundingBox{});
        shapes.push_back(Shape::box(b.xmin, b.ymin, b.xmax, b.ymax).translate(world.x(), world.y()));
    }
    for (const Shape& s : shapes) FlatCAMObj::addShapeTo(s, g, bounds, false);
    PlotLayerData d;
    d.lineColor = colorFromFlatCam("#0000FFAF");
    d.lines = std::move(g.lines);
    app_->ui()->plotCanvas()->setLayer(kLayerId, std::move(d));
}

void QRCodeTool::onRelease(const QPointF& world, Qt::MouseButton b) {
    // Solo el botón izquierdo (el derecho y el del medio son para mover la vista); no hay forma de cancelar.
    if (!placing_ || b != Qt::LeftButton) return;
    app_->ui()->plotCanvas()->removeLayer(kLayerId);
    make(world);
}

void QRCodeTool::make(const QPointF& pos) {
    onExit();
    if (!grb_ || !geo_) return;
    Gerber& g = static_cast<GerberObject*>(grb_)->gerber();
    QrGerber r = qrInsert(g, *geo_, {pos.x(), pos.y()}, bsize_entry->value(), border_size_entry->value(),
                          bb_radio->value().toString() == "r", pol_radio->value().toString() != "pos");
    g.replaceGeometry(std::move(r.solid), std::move(r.apertures), {r.newAperture, "0"});
    app_->refreshSourceFile(grb_);
    app_->objectChanged(grb_);
    app_->ui()->inform("[success] " + tr_("QRCode Tool done."));
}

void QRCodeTool::onExit() {
    disconnect(moveConn_);
    disconnect(releaseConn_);
    placing_ = false;
    app_->ui()->plotCanvas()->removeLayer(kLayerId);
}

bool QRCodeTool::exportPng(const QString& path) {
    const auto qr = makeMatrix();
    if (!qr) return false;
    const int box = bsize_entry->value(), border = border_size_entry->value();
    const int n = static_cast<int>(qr->modules.size());
    const int px = (n + border * 2) * box;
    // PilImage: fondo transparente con "transparent" (RGBA); si no, RGB con los dos colores.
    const QString back = back_color_entry->value();
    const bool transparent = back.toLower() == "transparent";
    QImage img(px, px, transparent ? QImage::Format_ARGB32 : QImage::Format_RGB32);
    img.fill(transparent ? QColor(0, 0, 0, 0) : QColor(back));
    QPainter p(&img);
    const QColor fill(fill_color_entry->value());
    for (int r = 0; r < n; ++r)
        for (int c = 0; c < n; ++c)
            if (qr->modules[r][c]) p.fillRect((c + border) * box, (r + border) * box, box, box, fill);
    p.end();
    return img.save(path, "PNG");
}

bool QRCodeTool::exportSvg(const QString& path) {
    const auto qr = makeMatrix();
    if (!qr) return false;
    const std::string s = qrSvgPath(*qr, bsize_entry->value(), border_size_entry->value(),
                                    fill_color_entry->value().toStdString(), back_color_entry->value().toStdString());
    QFile f(path);
    if (!f.open(QIODevice::WriteOnly)) return false;
    f.write(s.data(), static_cast<qint64>(s.size()));
    return true;
}

void QRCodeTool::onTransparent(bool on) {
    back_color_entry->setDisabled(on);
    back_color_button->setDisabled(on);
    if (on) {
        oldBackColor_ = back_color_entry->value();
        back_color_entry->setValue(QString("transparent"));
    } else {
        back_color_entry->setValue(oldBackColor_);
    }
}

}  // namespace flatcam::gui
