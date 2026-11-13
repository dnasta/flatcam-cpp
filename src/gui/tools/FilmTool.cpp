#include "gui/tools/FilmTool.h"

#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QGuiApplication>
#include <QImage>
#include <QPainter>
#include <QPdfWriter>
#include <QScreen>
#include <QSvgRenderer>

#include "gui/App.h"
#include "gui/MainGUI.h"
#include "gui/Preferences.h"
#include "gui/Translation.h"
#include "gui/objects/FlatCAMObj.h"
#include "gui/objects/GeometryObject.h"
#include "gui/objects/ObjectCollection.h"

namespace flatcam::gui {

namespace {

ShapeList flat(const ShapeList& l) {
    ShapeList out;
    for (const Shape& s : l) {
        if (s.isEmpty()) continue;
        if (s.type() == ShapeType::Polygon || s.type() == ShapeType::LineString || s.type() == ShapeType::LinearRing ||
            s.type() == ShapeType::Point)
            out.push_back(s);
        else
            for (const Shape& p : s.parts()) out.push_back(p);
    }
    return out;
}

/// `flatten()` del objeto (en una Geometría, las de sus herramientas).
ShapeList shapesOf(FlatCAMObj* o) {
    if (auto* g = dynamic_cast<GerberObject*>(o)) return flat(g->gerber().solidGeometry());
    if (auto* geo = dynamic_cast<GeometryObject*>(o)) {
        ShapeList all;
        for (const auto& [id, t] : geo->tools()) {
            const ShapeList f = flat(t.solidGeometry);
            all.insert(all.end(), f.begin(), f.end());
        }
        return all;
    }
    if (auto* e = dynamic_cast<ExcellonObject*>(o)) return flat(e->excellon().solidGeometry());
    return {};
}

std::string uomOf(FlatCAMObj* o) {
    if (auto* g = dynamic_cast<GerberObject*>(o)) return g->gerber().units() == Units::Inches ? "in" : "mm";
    return appUnits().toLower() == "in" ? "in" : "mm";
}

/// Los tamaños de hoja del original, en puntos (mm = 72 / 25,4; inch = 72).
QSizeF pageSize(const QString& name) {
    const double mm = 72.0 / 25.4, in = 72.0;
    static const std::map<QString, std::pair<double, double>> mmSizes = {
        {"A0", {841, 1189}}, {"A1", {594, 841}}, {"A2", {420, 594}}, {"A3", {297, 420}}, {"A4", {210, 297}},
        {"A5", {148, 210}},  {"A6", {105, 148}}, {"A7", {74, 105}},  {"A8", {52, 74}},   {"A9", {37, 52}},
        {"A10", {26, 37}},   {"B0", {1000, 1414}}, {"B1", {707, 1000}}, {"B2", {500, 707}}, {"B3", {353, 500}},
        {"B4", {250, 353}},  {"B5", {176, 250}}, {"B6", {125, 176}}, {"B7", {88, 125}},  {"B8", {62, 88}},
        {"B9", {44, 62}},    {"B10", {31, 44}},  {"C0", {917, 1297}}, {"C1", {648, 917}}, {"C2", {458, 648}},
        {"C3", {324, 458}},  {"C4", {229, 324}}, {"C5", {162, 229}}, {"C6", {114, 162}}, {"C7", {81, 114}},
        {"C8", {57, 81}},    {"C9", {40, 57}},   {"C10", {28, 40}}};
    static const std::map<QString, std::pair<double, double>> inSizes = {
        {"LETTER", {8.5, 11}},      {"LEGAL", {8.5, 14}},     {"ELEVENSEVENTEEN", {11, 17}},
        {"JUNIOR_LEGAL", {5, 8}},   {"HALF_LETTER", {5.5, 8}}, {"GOV_LETTER", {8, 10.5}},
        {"GOV_LEGAL", {8.5, 13}},   {"LEDGER", {17, 11}}};
    if (auto it = mmSizes.find(name); it != mmSizes.end()) return {it->second.first * mm, it->second.second * mm};
    if (auto it = inSizes.find(name); it != inSizes.end()) return {it->second.first * in, it->second.second * in};
    return {};
}

}  // namespace

bool renderSvgToPdf(const QByteArray& svg, const QString& filename, const QSizeF& drawing, const QString& p,
                    bool portrait, const QPointF& atIn) {
    QSvgRenderer renderer(svg);
    QSizeF page = drawing;
    QPointF at(0, 0);
    if (p != "Bounds") {
        page = pageSize(p);
        if (page.isEmpty()) return false;
        const double w = portrait ? std::min(page.width(), page.height()) : std::max(page.width(), page.height());
        const double h = portrait ? std::max(page.width(), page.height()) : std::min(page.width(), page.height());
        page = {w, h};
        at = atIn;
    }
    QPdfWriter pdf(filename);
    pdf.setPageSize(QPageSize(page, QPageSize::Point));
    pdf.setPageMargins(QMarginsF(0, 0, 0, 0));
    pdf.setResolution(72);
    QPainter painter(&pdf);
    if (!painter.isActive()) return false;
    // reportlab tiene el origen abajo a la izquierda.
    renderer.render(&painter, QRectF(QPointF(at.x(), page.height() - at.y() - drawing.height()), drawing));
    painter.end();
    return true;
}

FilmTool::FilmTool(App* app, QWidget* parent) : FilmToolUIBase(parent), app_(app) {
    setupUi();
    connect(film_object_button, &QPushButton::clicked, this, [this] { createFilm(); });
    connect(tf_type_obj_combo, &RadioSet::activatedCustom, this,
            [this](const QString& v) { fillCombo(tf_object_combo, v, false); });
    connect(tf_type_box_combo, &RadioSet::activatedCustom, this,
            [this](const QString& v) { fillCombo(tf_box_combo, v, false); });
    connect(film_type, &RadioSet::activatedCustom, this, [this](const QString& v) { onFilmType(v); });
    connect(source_punch, &RadioSet::activatedCustom, this, [this](const QString& v) { onPunchSource(v); });
    connect(file_type_radio, &RadioSet::activatedCustom, this, [this](const QString& v) { onFileType(v); });
    connect(reset_button, &QPushButton::clicked, this, [this] { setToolUi(); });
    // OptionalHideInputSection
    for (FCCheckBox* cb : {film_scale_cb, film_skew_cb, film_mirror_cb, punch_cb})
        connect(cb, &QCheckBox::checkStateChanged, this, [this] { updateSections(); });
    updateSections();
}

ToolInfo FilmTool::info() { return {tr_("Film PCB"), tr_("Film Tool")}; }

void FilmTool::updateSections() {
    const bool sc = film_scale_cb->isChecked(), sk = film_skew_cb->isChecked(), mi = film_mirror_cb->isChecked();
    for (QWidget* w : std::initializer_list<QWidget*>{film_scalex_label, film_scalex_entry, film_scaley_label,
                                                      film_scaley_entry})
        w->setVisible(sc);
    for (QWidget* w : std::initializer_list<QWidget*>{film_skewx_label, film_skewx_entry, film_skewy_label,
                                                      film_skewy_entry, film_skew_ref_label, film_skew_reference})
        w->setVisible(sk);
    for (QWidget* w : std::initializer_list<QWidget*>{film_mirror_axis_label, film_mirror_axis}) w->setVisible(mi);
    punch_frame->setVisible(punch_cb->isChecked());
}

void FilmTool::fillCombo(FCComboBox* combo, const QString& type, bool last) {
    const ObjectKind k = type == "geo" ? ObjectKind::Geometry : type == "exc" ? ObjectKind::Excellon : ObjectKind::Gerber;
    combo->clear();
    for (FlatCAMObj* o : app_->collection()->objects())
        if (o->kind() == k) combo->addItem(o->name());
    combo->setCurrentIndex(last ? combo->count() - 1 : 0);
}

void FilmTool::onFilmType(const QString& v) {
    if (v == "neg") {
        boundary_label->show();
        boundary_entry->show();
        punch_cb->setChecked(false);
        punch_cb->hide();
    } else {
        boundary_label->hide();
        boundary_entry->hide();
        punch_cb->show();
    }
}

void FilmTool::onFileType(const QString& v) {
    orientation_label->setVisible(v == "pdf");
    orientation_radio->setVisible(v == "pdf");
    pagesize_label->setVisible(v == "pdf");
    pagesize_combo->setVisible(v == "pdf");
    png_dpi_label->setVisible(v == "png");
    png_dpi_spinner->setVisible(v == "png");
}

void FilmTool::onPunchSource(const QString& v) {
    const bool pad = v == "pad" && punch_cb->isChecked();
    punch_size_label->setVisible(pad);
    punch_size_spinner->setVisible(pad);
    exc_label->setVisible(!pad);
    exc_combo->setVisible(!pad);
    if (v == "pad" && tf_type_obj_combo->value().toString() == "geo") {
        source_punch->setValue(QString("exc"));
        app_->ui()->inform("[WARNING_NOTCL] " + tr_("Using the Pad center does not work on Geometry objects. "
                                                   "Only a Gerber object has pads."));
    }
}

void FilmTool::setToolUi() {
    const QString ft = defStr("tools_film_type").isEmpty() ? QString("neg") : defStr("tools_film_type");
    film_type->setValue(ft);
    onFilmType(ft);
    boundary_entry->setValue(defNum("tools_film_boundary"));
    film_scale_stroke_entry->setValue(static_cast<int>(defNum("tools_film_scale_stroke")));
    punch_cb->setChecked(false);
    source_punch->setValue(QString("exc"));
    film_scale_cb->setChecked(defBool("tools_film_scale_cb"));
    film_scalex_entry->setValue(defNum("tools_film_scale_x_entry"));
    film_scaley_entry->setValue(defNum("tools_film_scale_y_entry"));
    film_skew_cb->setChecked(defBool("tools_film_skew_cb"));
    film_skewx_entry->setValue(defNum("tools_film_skew_x_entry"));
    film_skewy_entry->setValue(defNum("tools_film_skew_y_entry"));
    film_skew_reference->setValue(defVariant("tools_film_skew_ref_radio"));
    film_mirror_cb->setChecked(defBool("tools_film_mirror_cb"));
    film_mirror_axis->setValue(defVariant("tools_film_mirror_axis_radio"));
    file_type_radio->setValue(defVariant("tools_film_file_type_radio"));
    onFileType(file_type_radio->value().toString());
    orientation_radio->setValue(defVariant("tools_film_orientation"));
    pagesize_combo->setValue(defStr("tools_film_pagesize"));
    png_dpi_spinner->setValue(static_cast<int>(defNum("tools_film_png_dpi")));
    tf_type_obj_combo->setValue(QString("grb"));
    tf_type_box_combo->setValue(QString("grb"));
    fillCombo(tf_object_combo, "grb", false);
    fillCombo(tf_box_combo, "grb", false);
    fillCombo(exc_combo, "exc", true);  // is_last
    updateSections();
}

FilmOptions FilmTool::transforms() const {
    FilmOptions o;
    if (film_scale_cb->isChecked()) {
        if (film_scalex_entry->value() != 1.0) o.scaleX = film_scalex_entry->value();
        if (film_scaley_entry->value() != 1.0) o.scaleY = film_scaley_entry->value();
    }
    if (film_skew_cb->isChecked()) {
        if (film_skewx_entry->value() != 0.0) o.skewX = film_skewx_entry->value();
        if (film_skewy_entry->value() != 0.0) o.skewY = film_skewy_entry->value();
        o.skewReference = film_skew_reference->value().toString().toStdString();
    }
    if (film_mirror_cb->isChecked() && film_mirror_axis->value().toString() != "none")
        o.mirror = film_mirror_axis->value().toString().toStdString();
    return o;
}

QString FilmTool::createFilm() {
    MainGUI* ui = app_->ui();
    const QString name = tf_object_combo->currentText(), boxname = tf_box_combo->currentText();
    if (name.isEmpty() || boxname.isEmpty()) {
        ui->inform("[ERROR_NOTCL] " + tr_("No object is selected."));
        return QString();
    }
    const double factor = film_scale_stroke_entry->value();
    const QString ftype = file_type_radio->value().toString();
    ui->inform(tr_("Generating Film ..."));
    if (film_type->value().toString() == "pos") {
        if (!punch_cb->isChecked()) return positiveNormal(name, boxname, factor, ftype);
        return positivePunched(name, boxname, factor, ftype);
    }
    return negative(name, boxname, factor, ftype);
}

QString FilmTool::askFile(const QString& caption, const QString& name, const QString& ftype) {
    if (!saveOverride_.isEmpty()) return saveOverride_;
    const QString filter = ftype == "svg"   ? "SVG Files (*.SVG);;All Files (*.*)"
                           : ftype == "png" ? "PNG Files (*.PNG);;All Files (*.*)"
                                            : "PDF Files (*.PDF);;All Files (*.*)";
    return QFileDialog::getSaveFileName(this, caption, app_->lastFolder() + "/" + name + "_film", filter);
}

QString FilmTool::positiveNormal(const QString& name, const QString& boxname, double factor, const QString& ftype) {
    FilmOptions o = transforms();
    o.negative = false;
    o.strokeFactor = factor;
    const QString filename = askFile(tr_("Export positive film"), name, ftype);
    if (filename.isEmpty()) {
        app_->ui()->inform("[WARNING_NOTCL] " + tr_("Cancelled."));
        return QString();
    }
    o.color = defStr("tools_film_color").toStdString();
    return exportFilm(false, name, boxname, filename, o, ftype);
}

QString FilmTool::positivePunched(const QString& name, const QString& boxname, double factor, const QString& ftype) {
    MainGUI* ui = app_->ui();
    FlatCAMObj* film = app_->collection()->byName(name);
    auto* grb = dynamic_cast<GerberObject*>(film);
    Shape punched;
    if (source_punch->value().toString() == "exc") {
        FlatCAMObj* exc = app_->collection()->byName(exc_combo->currentText());
        auto* e = dynamic_cast<ExcellonObject*>(exc);
        if (!e || !film) return QString();  // el original falla (AttributeError)
        const ShapeList filmSolid = grb ? grb->gerber().solidGeometry() : shapesOf(film);
        punched = filmPunchExcellon(filmSolid, e->excellon().solidGeometry());
    } else {
        if (!grb) return QString();
        std::string err;
        const auto r = filmPunchPads(grb->gerber(), punch_size_spinner->value(), err);
        if (!r) {
            if (err == "size")
                ui->inform("[ERROR_NOTCL] " + tr_("Failed. Punch hole size is bigger than some of the apertures in "
                                                  "the Gerber object."));
            else if (err == "same")
                ui->inform("[WARNING_NOTCL] " + tr_("Failed. The new object geometry is the same as the one in the "
                                                    "source object geometry..."));
            return QString();  // "keyerror": el original falla sin aviso
        }
        punched = *r;
    }
    // new_object('gerber'): el objeto nace en gerber_def_units (pulgadas de fábrica) y se convierte a las unidades
    // de la aplicación, así que en mm la geometría queda 25,4 veces más grande, como en el original.
    const QString defUnits = defStr("gerber_def_units").toUpper();
    if (defUnits != appUnits().toUpper())
        punched = punched.transform(AffineTransform::scaling(defUnits == "IN" ? 25.4 : 1 / 25.4,
                                                             defUnits == "IN" ? 25.4 : 1 / 25.4, {0, 0}));
    auto g = std::make_unique<Gerber>();
    g->restore(flat({punched}), {}, {}, 2, 4, std::string());
    if (appUnitsKind() == Units::Inches) g->convertUnits(Units::Inches);
    const QString outname = name + "_punched";
    app_->addObject(std::make_unique<GerberObject>(outname, std::move(g)));
    return positiveNormal(outname, boxname, factor, ftype);
}

QString FilmTool::negative(const QString& name, const QString& boxname, double factor, const QString& ftype) {
    FilmOptions o = transforms();
    o.negative = true;
    o.strokeFactor = factor;
    o.boundary = boundary_entry->value();
    const QString filename = askFile(tr_("Export negative film"), name, ftype);
    if (filename.isEmpty()) {
        app_->ui()->inform("[WARNING_NOTCL] " + tr_("Cancelled."));
        return QString();
    }
    return exportFilm(true, name, boxname, filename, o, ftype);
}

QString FilmTool::exportFilm(bool negative, const QString& objName, const QString& boxName, const QString& filename,
                             FilmOptions o, const QString& ftype) {
    MainGUI* ui = app_->ui();
    FlatCAMObj* obj = app_->collection()->byName(objName);
    FlatCAMObj* box = app_->collection()->byName(boxName);
    if (!obj) return QString();
    if (!box) {
        ui->inform(QString("[WARNING_NOTCL] %1: %2").arg(tr_("No object Box. Using instead"), obj->name()));
        box = obj;
    }
    o.fileType = ftype.toStdString();
    o.pngDpi = png_dpi_spinner->value();
    if (QScreen* s = QGuiApplication::primaryScreen()) o.screenDpi = s->logicalDotsPerInch();
    const QRectF bb = box->bounds();
    const ShapeList boxShapes = shapesOf(box);
    const FilmResult r = filmSvg(shapesOf(obj), boxShapes, {bb.left(), bb.top(), bb.right(), bb.bottom()},
                                 uomOf(obj), o);
    lastSvg_ = r.svg;
    const QByteArray svg = QByteArray::fromStdString(r.svg);
    if (ftype == "svg") {
        QFile f(filename);
        if (!f.open(QIODevice::WriteOnly | QIODevice::Text)) {
            ui->inform("[WARNING] " + tr_("Permission denied, saving not possible.\nMost likely another app is "
                                          "holding the file open and not accessible."));
            return QString();
        }
        f.write(svg);
    } else {
        // svglib + reportlab: el dibujo mide lo que dice el SVG, en puntos (72 por pulgada).
        QSvgRenderer renderer(svg);
        const double toPt = uomOf(obj) == "in" ? 72.0 : 72.0 / 25.4;
        const QSizeF drawing(r.width * toPt, r.height * toPt);
        if (ftype == "png") {
            // renderPM.drawToFile: 72 dpi, fondo blanco.
            QImage img(drawing.toSize().expandedTo(QSize(1, 1)), QImage::Format_RGB32);
            img.fill(Qt::white);
            QPainter p(&img);
            renderer.render(&p, QRectF(QPointF(0, 0), drawing));
            p.end();
            if (!img.save(filename, "PNG")) return QString();
        } else {
            // La negativa usa siempre mm (compara self.units con 'INCH'); la positiva, pulgadas si son las unidades.
            const double unit = !negative && appUnits().toUpper() == "IN" ? 72.0 : 72.0 / 25.4;
            if (!renderSvgToPdf(svg, filename, drawing, pagesize_combo->currentText(),
                                orientation_radio->value().toString() == "p",
                                {r.bounds.xmin * unit, r.bounds.ymin * unit}))
                return QString();
        }
    }
    app_->setLastFolder(QFileInfo(filename).absolutePath());
    ui->inform(QString("[success] %1: %2").arg(tr_("Film file exported to"), filename));
    return filename;
}

}  // namespace flatcam::gui
