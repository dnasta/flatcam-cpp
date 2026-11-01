/**
 * @file FCComboBox.h
 * @brief Listas desplegables de FlatCAM (port de `FCComboBox` y `FCComboBox2` de `appGUI/GUIElements.py`).
 */
#pragma once

#include <QComboBox>

namespace flatcam::gui {

/**
 * @brief `QComboBox` que no cambia con la rueda del ratón, ignora el clic derecho en la lista y cuyo valor es el texto
 * elegido (`get_value` / `set_value`).
 */
class FCComboBox : public QComboBox {
    Q_OBJECT
public:
    /** @brief Crea la lista (política horizontal ignorada, como el original). @param parent Widget padre. */
    explicit FCComboBox(QWidget* parent = nullptr);

    /** @brief Texto elegido (`get_value`). @return El texto. */
    QString value() const { return currentText(); }

    /** @brief Elige por texto; si no está, el primero (`set_value`). @param val Texto. */
    void setValue(const QString& val);

protected:
    /** @brief La rueda no cambia la selección. @param ev Evento. */
    void wheelEvent(QWheelEvent* ev) override;
    /** @brief Ignora el clic derecho en la lista. @param obj Objeto. @param ev Evento. @return Si se consumió. */
    bool eventFilter(QObject* obj, QEvent* ev) override;
};

/** @brief Igual que @ref FCComboBox pero su valor es el índice elegido. */
class FCComboBox2 : public FCComboBox {
    Q_OBJECT
public:
    /** @brief Crea la lista. @param parent Widget padre. */
    explicit FCComboBox2(QWidget* parent = nullptr) : FCComboBox(parent) {}

    /** @brief Índice elegido (`get_value`). @return El índice. */
    int indexValue() const { return currentIndex(); }

    /** @brief Elige por índice (`set_value`). @param i Índice. */
    void setIndexValue(int i) { setCurrentIndex(i); }
};

}  // namespace flatcam::gui
