/**
 * @file DialogBoxRadio.h
 * @brief Diálogos de "Saltar a" y "Ubicar" (port de `DialogBoxRadio` de `appGUI/GUIElements.py` y de
 * `DialogBoxChoice` de `App.on_locate`).
 */
#pragma once

#include <QDialog>
#include <QIcon>
#include <QLineEdit>

#include "gui/widgets/RadioSet.h"

namespace flatcam::gui {

/**
 * @brief Pide una posición `x, y` y si es absoluta o relativa al mouse (`DialogBoxRadio`). A diferencia del
 * original, el constructor no lo muestra: se llama a `exec()` (así se puede probar).
 */
class DialogBoxRadio : public QDialog {
    Q_OBJECT
public:
    /**
     * @brief Arma el diálogo.
     * @param title Título. @param label Mensaje (en negrita). @param icon Ícono. @param initialText Texto inicial (sin
     * paréntesis); vacío: `0, 0`. @param reference `"abs"` o `"rel"`. @param parent Padre.
     */
    DialogBoxRadio(const QString& title, const QString& label, const QIcon& icon, const QString& initialText,
                   const QString& reference, QWidget* parent = nullptr);
    RadioSet* ref_radio;   ///< `ref_radio`: Abs / Relativo.
    QLineEdit* lineEdit;   ///< `lineEdit`: la posición.
};

/** @brief Elige una esquina o el centro (`DialogBoxChoice` de `on_locate`). Se muestra con `exec()`. */
class DialogBoxChoice : public QDialog {
    Q_OBJECT
public:
    /** @brief Arma el diálogo. @param title Título. @param icon Ícono. @param choice `bl`, `tl`, `br`, `tr` o `c`. @param parent Padre. */
    DialogBoxChoice(const QString& title, const QIcon& icon, const QString& choice, QWidget* parent = nullptr);
    RadioSet* ref_radio;  ///< `ref_radio`.
};

}  // namespace flatcam::gui
