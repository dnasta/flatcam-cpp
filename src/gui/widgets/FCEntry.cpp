#include "gui/widgets/FCEntry.h"

#include <QFocusEvent>

namespace flatcam::gui {

FCEntry::FCEntry(QWidget* parent) : QLineEdit(parent) {
    connect(this, &QLineEdit::editingFinished, this, [this] { clearFocus(); });
}

void FCEntry::setValue(double val, int decimals) {
    setText(QString::number(val, 'f', decimals >= 0 ? decimals : decimals_));
}

QSize FCEntry::sizeHint() const { return QSize(kEditSizeHint, QLineEdit::sizeHint().height()); }

void FCEntry::mousePressEvent(QMouseEvent* e) {
    QLineEdit::mousePressEvent(e);  // hace falta para deseleccionar en el segundo clic
    if (readyToEdit_) {
        selectAll();
        readyToEdit_ = false;
    }
}

void FCEntry::focusOutEvent(QFocusEvent* e) {
    if (e->reason() != Qt::PopupFocusReason) {
        QLineEdit::focusOutEvent(e);
        deselect();
        readyToEdit_ = true;
    }
}

void FCEntry2::setValue(double val, int decimals) { setText(QString::number(val, 'f', decimals >= 0 ? decimals : 4)); }

}  // namespace flatcam::gui
