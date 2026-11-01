/**
 * @file FCSpinners.h
 * @brief Campos numéricos de FlatCAM (port de `FCSpinner` y `FCDoubleSpinner` de `appGUI/GUIElements.py`).
 */
#pragma once

#include <QDoubleSpinBox>
#include <QSpinBox>

namespace flatcam::gui {

/**
 * @brief Campo de números enteros: selecciona todo al primer clic, la rueda solo actúa mientras se edita y Enter
 * confirma (`returnPressed`).
 */
class FCSpinner : public QSpinBox {
    Q_OBJECT
public:
    /** @brief Crea el campo (política de tamaño horizontal ignorada, como el original). @param parent Widget padre. */
    explicit FCSpinner(QWidget* parent = nullptr);

    /** @brief Valor (`get_value`). @return El valor. */
    int value_() const { return value(); }

    /** @brief Cambia el valor (`set_value`). @param v Valor. */
    void setValue_(int v) { setValue(v); }

Q_SIGNALS:
    /** @brief Se confirmó el valor con Enter o al salir del campo. */
    void returnPressed();

protected:
    /** @brief Primer clic: selecciona todo. @param obj Objeto. @param ev Evento. @return Si se consumió. */
    bool eventFilter(QObject* obj, QEvent* ev) override;
    /** @brief Enter confirma. @param ev Evento. */
    void keyPressEvent(QKeyEvent* ev) override;
    /** @brief La rueda solo cambia el valor mientras se edita. @param ev Evento. */
    void wheelEvent(QWheelEvent* ev) override;
    /** @brief Deselecciona al perder el foco. @param ev Evento. */
    void focusOutEvent(QFocusEvent* ev) override;

private:
    bool readyToEdit_{true};
    bool prevReadyToEdit_{true};
};

/**
 * @brief Campo de números decimales de FlatCAM: como @ref FCSpinner, acepta coma o punto decimal y avisa si el valor
 * escrito está fuera de los límites (`confirmation_signal`, que la barra de estado muestra como "El valor editado está
 * dentro de los límites" o fuera de ellos).
 */
class FCDoubleSpinner : public QDoubleSpinBox {
    Q_OBJECT
public:
    /** @brief Crea el campo. @param parent Widget padre. */
    explicit FCDoubleSpinner(QWidget* parent = nullptr);

    /** @brief Decimales y validador acorde (`set_precision`). @param decimals Decimales. */
    void setPrecision(int decimals);

    /** @brief Límites y validador acorde (`set_range`). @param min Mínimo. @param max Máximo. */
    void setRange_(double min, double max);

    /** @brief Interpreta el texto (coma o punto decimal, sin `%`). @param text Texto. @return El valor (0 si no es número). */
    double valueFromText(const QString& text) const override;

    /** @brief Avisa si el valor está dentro de los límites. @param input Texto. @param pos Cursor. @return Estado. */
    QValidator::State validate(QString& input, int& pos) const override;

Q_SIGNALS:
    /** @brief Se confirmó el valor. */
    void returnPressed();
    /** @brief Resultado de validar lo escrito. @param ok Dentro de límites. @param min Mínimo. @param max Máximo. */
    void confirmationSignal(bool ok, double min, double max) const;

protected:
    /** @brief Primer clic: selecciona todo. @param obj Objeto. @param ev Evento. @return Si se consumió. */
    bool eventFilter(QObject* obj, QEvent* ev) override;
    /** @brief Enter confirma. @param ev Evento. */
    void keyPressEvent(QKeyEvent* ev) override;
    /** @brief La rueda solo cambia el valor mientras se edita. @param ev Evento. */
    void wheelEvent(QWheelEvent* ev) override;
    /** @brief Deselecciona al perder el foco. @param ev Evento. */
    void focusOutEvent(QFocusEvent* ev) override;

private:
    void updateValidator();
    bool readyToEdit_{true};
    bool prevReadyToEdit_{true};
};

}  // namespace flatcam::gui
