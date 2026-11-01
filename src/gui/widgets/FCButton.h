/**
 * @file FCButton.h
 * @brief Botón de FlatCAM (port de `FCButton` de `appGUI/GUIElements.py`).
 */
#pragma once

#include <QPushButton>

namespace flatcam::gui {

/** @brief `QPushButton` con la interfaz `get_value` (si está presionado) / `set_value` (texto) de FlatCAM. */
class FCButton : public QPushButton {
    Q_OBJECT
public:
    /** @brief Crea el botón. @param parent Widget padre. @param text Texto. */
    explicit FCButton(QWidget* parent = nullptr, const QString& text = QString()) : QPushButton(text, parent) {}

    /** @brief Si está presionado (`get_value`). @return El estado. */
    bool value() const { return isChecked(); }

    /** @brief Cambia el texto (`set_value`). @param val Texto. */
    void setValue(const QString& val) { setText(val); }
};

}  // namespace flatcam::gui
