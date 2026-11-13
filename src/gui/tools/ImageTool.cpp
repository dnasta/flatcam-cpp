#include "gui/tools/ImageTool.h"

#include <QFileDialog>
#include <QFileInfo>
#include <QImage>

#include "gui/App.h"
#include "gui/MainGUI.h"
#include "gui/Preferences.h"
#include "gui/Translation.h"
#include "gui/objects/FlatCAMObj.h"
#include "gui/objects/GeometryObject.h"

namespace flatcam::gui {

ImageBands readImageBands(const QString& filename, bool* ok) {
    ImageBands b;
    QImage img(filename);
    if (ok) *ok = !img.isNull();
    if (img.isNull()) return b;
    b.width = img.width();
    b.height = img.height();
    const std::size_t n = static_cast<std::size_t>(b.width) * b.height;
    const QImage::Format f = img.format();
    if (f == QImage::Format_Indexed8 || f == QImage::Format_Mono || f == QImage::Format_MonoLSB) {
        // Con paleta, rasterio lee los índices (una sola banda).
        b.red.resize(n);
        for (int y = 0; y < b.height; ++y)
            for (int x = 0; x < b.width; ++x) b.red[static_cast<std::size_t>(y) * b.width + x] = img.pixelIndex(x, y);
        return b;
    }
    if (f == QImage::Format_Grayscale8 || f == QImage::Format_Grayscale16) {
        const bool wide = f == QImage::Format_Grayscale16;
        b.red.resize(n);
        for (int y = 0; y < b.height; ++y) {
            for (int x = 0; x < b.width; ++x) {
                const double v = wide ? reinterpret_cast<const quint16*>(img.constScanLine(y))[x]
                                      : img.constScanLine(y)[x];
                b.red[static_cast<std::size_t>(y) * b.width + x] = v;
            }
        }
        return b;
    }
    const bool wide = img.depth() > 32;  // RGB(A)64: los valores de 16 bits
    b.red.resize(n);
    b.green.resize(n);
    b.blue.resize(n);
    for (int y = 0; y < b.height; ++y) {
        for (int x = 0; x < b.width; ++x) {
            const std::size_t i = static_cast<std::size_t>(y) * b.width + x;
            if (wide) {
                const QRgba64 p = img.pixelColor(x, y).rgba64();
                b.red[i] = p.red();
                b.green[i] = p.green();
                b.blue[i] = p.blue();
            } else {
                const QRgb p = img.pixel(x, y);
                b.red[i] = qRed(p);
                b.green[i] = qGreen(p);
                b.blue[i] = qBlue(p);
            }
        }
    }
    return b;
}

ImageTool::ImageTool(App* app, QWidget* parent) : ImageToolUIBase(parent), app_(app) {
    setupUi();
    connect(import_button, &QPushButton::clicked, this, [this] {
        const QString f = QFileDialog::getOpenFileName(this, tr_("Import IMAGE"), app_->lastFolder(),
                                                       "Image Files(*.BMP *.PNG *.JPG *.JPEG);;Bitmap File (*.BMP);;"
                                                       "PNG File (*.PNG);;Jpeg File (*.JPG);;All Files (*.*)");
        if (f.isEmpty()) app_->ui()->inform(tr_("Cancelled."));
        else importImage(f);
    });
    connect(image_type, &RadioSet::activatedCustom, this, [this](const QString& v) { onImageType(v); });
}

ToolInfo ImageTool::info() { return {tr_("Image as Object"), tr_("Image Tool")}; }

void ImageTool::setToolUi() {
    dpi_entry->setValue(96);
    image_type->setValue(QString("black"));
    mask_bw_entry->setValue(250);
    mask_r_entry->setValue(250);
    mask_g_entry->setValue(250);
    mask_b_entry->setValue(250);
}

void ImageTool::onImageType(const QString& v) {
    const bool color = v == "color";
    for (QWidget* w : std::initializer_list<QWidget*>{mask_r_label, mask_r_entry, mask_g_label, mask_g_entry,
                                                      mask_b_label, mask_b_entry})
        w->setDisabled(!color);
    mask_bw_label->setDisabled(color);
    mask_bw_entry->setDisabled(color);
}

FlatCAMObj* ImageTool::importImage(const QString& filename) {
    bool ok = false;
    const ImageBands bands = readImageBands(filename, &ok);
    if (!ok) {
        app_->ui()->inform("[ERROR_NOTCL] " + tr_("Import failed."));
        return nullptr;
    }
    const bool gerber = tf_type_obj_combo->currentText() == tr_("Gerber");
    const Shape s = flatcam::importImage(bands, appUnits().toUpper().toStdString(), dpi_entry->value(),
                                         image_type->value().toString().toStdString(),
                                         {double(mask_bw_entry->value()), double(mask_r_entry->value()),
                                          double(mask_g_entry->value()), double(mask_b_entry->value())});
    ShapeList polys;
    if (!s.isEmpty()) polys = s.type() == ShapeType::Polygon ? ShapeList{s} : s.parts();
    // name = filename.split('/')[-1].split('\\')[-1]
    const QString name = QFileInfo(filename).fileName();
    FlatCAMObj* obj = nullptr;
    if (gerber) {
        // Un Gerber con solid_geometry y sin aperturas, como deja Geometry.import_image.
        auto g = std::make_unique<Gerber>();
        g->restore(polys, {}, {}, 2, 4, std::string());
        obj = app_->addObject(std::make_unique<GerberObject>(name, std::move(g)));
    } else {
        auto geo = std::make_unique<GeometryObject>(name);
        geo->solidGeometry() = polys;
        GeometryTool t = defaultGeometryTool();
        t.solidGeometry = polys;
        geo->tools()[1] = t;
        obj = app_->addObject(std::move(geo));
    }
    app_->setLastFolder(QFileInfo(filename).absolutePath());
    app_->registerRecent("image", filename);
    app_->ui()->inform(QString("[success] %1: %2").arg(tr_("Opened"), filename));
    return obj;
}

}  // namespace flatcam::gui
