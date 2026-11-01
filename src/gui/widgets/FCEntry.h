/**
 * @file FCEntry.h
 * @brief Campos de texto de FlatCAM (port de `FCEntry` y `FCEntry2` de `appGUI/GUIElements.py`).
 */
#pragma once

#include <QLineEdit>

namespace flatcam::gui {

/**
 * @brief Campo de texto que selecciona todo al primer clic y pierde el foco al terminar de editar.
 */
class FCEntry : public QLineEdit {
    Q_OBJECT
public:
    /** @brief Ancho preferido de los campos de FlatCAM (`EDIT_SIZE_HINT`). */
    static constexpr int kEditSizeHint = 70;

    /** @brief Crea el campo. @param parent Widget padre. */
    explicit FCEntry(QWidget* parent = nullptr);

    /** @brief Decimales con que se muestran los números (`decimals`, 4 por defecto). @param d Cantidad. */
    void setDecimals(int d) { decimals_ = d; }

    /** @brief Texto del campo (`get_value`). @return El texto. */
    QString value() const { return text(); }

    /**
     * @brief Muestra un número con los decimales del campo (`set_value` con un `float`).
     * @param val Valor.
     * @param decimals Decimales; si es negativo se usan los del campo.
     */
    virtual void setValue(double val, int decimals = -1);

    /** @brief Muestra un texto tal cual (`set_value` con otro tipo). @param val Texto. */
    void setValue(const QString& val) { setText(val); }

    /** @brief Tamaño preferido: ancho fijo de 70 píxeles. @return El tamaño. */
    QSize sizeHint() const override;

protected:
    /** @brief Selecciona todo en el primer clic. @param e Evento. */
    void mousePressEvent(QMouseEvent* e) override;
    /** @brief Deselecciona al perder el foco (salvo por un menú emergente). @param e Evento. */
    void focusOutEvent(QFocusEvent* e) override;

    int decimals_{4};  ///< Decimales para mostrar números.

private:
    bool readyToEdit_{true};
};

/** @brief Igual que @ref FCEntry, pero `set_value` siempre formatea como número (4 decimales por defecto). */
class FCEntry2 : public FCEntry {
    Q_OBJECT
public:
    /** @brief Crea el campo. @param parent Widget padre. */
    explicit FCEntry2(QWidget* parent = nullptr) : FCEntry(parent) {}

    /**
     * @brief Muestra el número con @p decimals decimales (como el formato de Python del original).
     * @param val Valor.
     * @param decimals Decimales; si es negativo, 4.
     */
    void setValue(double val, int decimals = -1) override;
};

}  // namespace flatcam::gui
