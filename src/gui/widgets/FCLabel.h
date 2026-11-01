/**
 * @file FCLabel.h
 * @brief Etiqueta que avisa de los clics (port de `FCLabel` de `appGUI/GUIElements.py`).
 */
#pragma once

#include <QLabel>

namespace flatcam::gui {

/**
 * @brief `QLabel` que emite una señal por cada botón del ratón y recuerda un estado alternado por clic.
 *
 * FlatCAM la usa como botón liviano (por ejemplo el ícono de actividad o el rótulo del área de trabajo).
 */
class FCLabel : public QLabel {
    Q_OBJECT
public:
    /** @brief Crea la etiqueta. @param parent Widget padre. */
    explicit FCLabel(QWidget* parent = nullptr);

    /** @brief Texto de la etiqueta (`get_value`). @return El texto. */
    QString value() const { return text(); }

    /** @brief Cambia el texto (`set_value`). @param val Texto nuevo. */
    void setValue(const QString& val) { setText(val); }

signals:
    /** @brief Clic izquierdo. @param state Estado alternado después del clic. */
    void clicked(bool state);
    /** @brief Clic derecho. @param state Siempre `true`, como en el original. */
    void rightClicked(bool state);
    /** @brief Clic del medio. @param state Siempre `true`, como en el original. */
    void middleClicked(bool state);

protected:
    /** @brief Emite la señal del botón presionado. @param event Evento del ratón. */
    void mousePressEvent(QMouseEvent* event) override;

private:
    bool clickedState_{false};
    bool middleClickedState_{false};
    bool rightClickedState_{false};
};

}  // namespace flatcam::gui
