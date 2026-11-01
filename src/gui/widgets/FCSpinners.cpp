#include "gui/widgets/FCSpinners.h"

#include <QEvent>
#include <QKeyEvent>
#include <QLineEdit>
#include <QRegularExpressionValidator>

namespace flatcam::gui {

namespace {

/// Selección del primer clic, común a los dos campos (`eventFilter` del original).
bool firstClick(QAbstractSpinBox* box, QLineEdit* le, QEvent* ev, bool& ready, bool& prevReady) {
    if (ev->type() == QEvent::MouseButtonPress && prevReady) {
        prevReady = false;
        if (box->isEnabled()) {
            if (ready) {
                le->selectAll();
                ready = false;
            } else {
                le->deselect();
            }
            return true;
        }
    }
    return false;
}

}  // namespace

FCSpinner::FCSpinner(QWidget* parent) : QSpinBox(parent) {
    connect(this, &QSpinBox::editingFinished, this, [this] {
        clearFocus();
        Q_EMIT returnPressed();
    });
    lineEdit()->installEventFilter(this);
    setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Preferred);
}

bool FCSpinner::eventFilter(QObject* obj, QEvent* ev) {
    if (obj == lineEdit() && firstClick(this, lineEdit(), ev, readyToEdit_, prevReadyToEdit_)) return true;
    return QSpinBox::eventFilter(obj, ev);
}

void FCSpinner::keyPressEvent(QKeyEvent* ev) {
    if (ev->key() == Qt::Key_Enter) {
        Q_EMIT returnPressed();
        clearFocus();
    } else {
        QSpinBox::keyPressEvent(ev);
    }
}

void FCSpinner::wheelEvent(QWheelEvent* ev) {
    if (!readyToEdit_) QSpinBox::wheelEvent(ev);
}

void FCSpinner::focusOutEvent(QFocusEvent* ev) {
    if (ev->reason() != Qt::PopupFocusReason) {
        QSpinBox::focusOutEvent(ev);
        lineEdit()->deselect();
        readyToEdit_ = true;
        prevReadyToEdit_ = true;
    }
}

FCDoubleSpinner::FCDoubleSpinner(QWidget* parent) : QDoubleSpinBox(parent) {
    connect(this, &QDoubleSpinBox::editingFinished, this, [this] {
        clearFocus();
        Q_EMIT returnPressed();
    });
    lineEdit()->installEventFilter(this);
    updateValidator();
    setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Preferred);
}

void FCDoubleSpinner::updateValidator() {
    // Con valores negativos se permite el signo menos; si no, solo el más (set_precision / set_range).
    const QString sign = (minimum() < 0 || maximum() <= 0) ? "-?" : "\\+?";
    lineEdit()->setValidator(new QRegularExpressionValidator(
        QRegularExpression(sign + QString("[0-9]*[.,]?[0-9]{%1}").arg(decimals())), this));
}

void FCDoubleSpinner::setPrecision(int d) {
    setDecimals(d);
    updateValidator();
}

void FCDoubleSpinner::setRange_(double min, double max) {
    setRange(min, max);
    updateValidator();
}

double FCDoubleSpinner::valueFromText(const QString& text) const {
    QString t = text;
    t.replace(',', '.');
    t.remove('%');
    bool ok = false;
    const double v = t.trimmed().toDouble(&ok);
    return ok ? v : 0.0;
}

QValidator::State FCDoubleSpinner::validate(QString& input, int&) const {
    QString t = input;
    t.replace(',', '.');
    bool ok = false;
    const double v = t.toDouble(&ok);
    if (ok && (v < minimum() || v > maximum())) {
        Q_EMIT confirmationSignal(false, minimum(), maximum());
        return QValidator::Intermediate;
    }
    Q_EMIT confirmationSignal(true, minimum(), maximum());
    return QValidator::Acceptable;
}

bool FCDoubleSpinner::eventFilter(QObject* obj, QEvent* ev) {
    if (obj == lineEdit() && firstClick(this, lineEdit(), ev, readyToEdit_, prevReadyToEdit_)) return true;
    return QDoubleSpinBox::eventFilter(obj, ev);
}

void FCDoubleSpinner::keyPressEvent(QKeyEvent* ev) {
    if (ev->key() == Qt::Key_Enter) {
        Q_EMIT returnPressed();
        clearFocus();
    } else {
        QDoubleSpinBox::keyPressEvent(ev);
    }
}

void FCDoubleSpinner::wheelEvent(QWheelEvent* ev) {
    if (!readyToEdit_) QDoubleSpinBox::wheelEvent(ev);
}

void FCDoubleSpinner::focusOutEvent(QFocusEvent* ev) {
    if (ev->reason() != Qt::PopupFocusReason) {
        QDoubleSpinBox::focusOutEvent(ev);
        lineEdit()->deselect();
        readyToEdit_ = true;
        prevReadyToEdit_ = true;
    }
}

}  // namespace flatcam::gui
