/**
 * @file FCCheckBox.h
 * @brief Casilla de verificación de FlatCAM (port de `FCCheckBox` de `appGUI/GUIElements.py`).
 */
#pragma once

#include <QCheckBox>

namespace flatcam::gui {

/** @brief `QCheckBox` con la interfaz `get_value` / `set_value` de FlatCAM. */
class FCCheckBox : public QCheckBox {
    Q_OBJECT
public:
    /**
     * @brief Crea la casilla.
     * @param parent Widget padre.
     * @param label Texto de la casilla.
     */
    explicit FCCheckBox(QWidget* parent = nullptr, const QString& label = QString()) : QCheckBox(label, parent) {}

    /** @brief Si está tildada (`get_value`). @return El estado. */
    bool value() const { return isChecked(); }

    /** @brief Tilda o destilda (`set_value`). @param val Estado nuevo. */
    void setValue(bool val) { setChecked(val); }

    /** @brief Invierte el estado (`toggle`). */
    void toggleValue() { setValue(!value()); }
};

}  // namespace flatcam::gui
