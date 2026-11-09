/**
 * @file FCPrefWidgets.h
 * @brief Widgets de `appGUI/GUIElements.py` que usan las Preferencias: `FCColorEntry`, `FCSliderWithSpinner`,
 * `FCTextArea` y `FCTab`.
 */
#pragma once

#include <QFrame>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QSlider>
#include <QTabWidget>

#include "gui/widgets/FCEntry.h"
#include "gui/widgets/FCSpinners.h"

namespace flatcam::gui {

/**
 * @brief Campo de color `#RRGGBB[AA]` con un botón que muestra el color y abre el selector (`FCColorEntry`). El
 * selector conserva la transparencia escrita.
 */
class FCColorEntry : public QFrame {
    Q_OBJECT
public:
    /** @brief Crea el campo. @param parent Widget padre. */
    explicit FCColorEntry(QWidget* parent = nullptr);
    /** @brief El texto (`get_value`). @return El color. */
    QString value() const { return entry->text(); }
    /** @brief Cambia el texto y el color del botón (`set_value`). @param v El color. */
    void setValue(const QString& v);
    FCEntry* entry;       ///< `entry`.
    QPushButton* button;  ///< `button`.
Q_SIGNALS:
    /** @brief Se terminó de editar (`editingFinished`). */
    void editingFinished();

private:
    void syncButtonColor();
    void onButtonClicked();
};

/** @brief Deslizador con su número (`FCSliderWithSpinner`): los dos muestran el mismo valor. */
class FCSliderWithSpinner : public QFrame {
    Q_OBJECT
public:
    /** @brief Crea el control. @param min Mínimo. @param max Máximo. @param step Paso. @param parent Widget padre. */
    FCSliderWithSpinner(int min = 0, int max = 100, int step = 1, QWidget* parent = nullptr);
    /** @brief El valor (`get_value`). @return El valor. */
    int value() const { return spinner->value(); }
    /** @brief Cambia el valor (`set_value`). @param v El valor. */
    void setValue(int v) { spinner->setValue(v); }
    QSlider* slider;     ///< `slider`.
    FCSpinner* spinner;  ///< `spinner`.
Q_SIGNALS:
    /** @brief Cambió el valor (`valueChanged` del número). @param v El valor. */
    void valueChanged(int v);
};

/** @brief Texto de varias líneas (`FCTextArea`). */
class FCTextArea : public QPlainTextEdit {
    Q_OBJECT
public:
    /** @brief Crea el campo. @param parent Widget padre. */
    explicit FCTextArea(QWidget* parent = nullptr) : QPlainTextEdit(parent) {}
    /** @brief El texto (`get_value`). @return El texto. */
    QString value() const { return toPlainText(); }
    /** @brief Cambia el texto (`set_value`). @param v El texto. */
    void setValue(const QString& v) { setPlainText(v); }
    /** @brief Ancho sugerido `EDIT_SIZE_HINT` (70). @return El tamaño. */
    QSize sizeHint() const override { return {70, QPlainTextEdit::sizeHint().height()}; }
};

/** @brief Pestañas que se cierran con su botón (`FCTab`). */
class FCTab : public QTabWidget {
    Q_OBJECT
public:
    /** @brief Crea las pestañas. @param parent Widget padre. */
    explicit FCTab(QWidget* parent = nullptr);
    /** @brief Quita el botón de cerrar de una pestaña (`protectTab`). @param index Pestaña. */
    void protectTab(int index);
};

}  // namespace flatcam::gui
