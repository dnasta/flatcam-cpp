#include "gui/objects/DocumentObject.h"

#include <QTextBrowser>

namespace flatcam::gui {

DocumentObject::DocumentObject(QString name, QString text, bool readOnly)
    : FlatCAMObj(ObjectKind::Document, std::move(name)), text_(std::move(text)), readOnly_(readOnly) {}

QWidget* DocumentObject::propertiesUi() {
    if (!ui_) ui_ = std::make_unique<QTextBrowser>();
    auto* b = static_cast<QTextBrowser*>(ui_.get());
    b->setReadOnly(readOnly_);
    b->setHtml(text_);
    return b;
}

}  // namespace flatcam::gui
