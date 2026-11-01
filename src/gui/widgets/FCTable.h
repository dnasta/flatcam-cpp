/**
 * @file FCTable.h
 * @brief Tabla de FlatCAM (port de `FCTable` de `appGUI/GUIElements.py`).
 */
#pragma once

#include <QTableWidget>

namespace flatcam::gui {

/**
 * @brief `QTableWidget` de las tablas de herramientas: la selección se ve igual con o sin foco, un clic fuera de las filas
 * deselecciona y avisa cuando pierde el foco.
 */
class FCTable : public QTableWidget {
    Q_OBJECT
public:
    /** @brief Crea la tabla. @param parent Widget padre. */
    explicit FCTable(QWidget* parent = nullptr);

    /** @brief Ancho preferido de 70 píxeles (`EDIT_SIZE_HINT`). @return El tamaño. */
    QSize sizeHint() const override;

    /** @brief Alto de la cabecera más todas las filas (`getHeight`). @return Píxeles. */
    int contentHeight() const;

    /** @brief Ancho de la cabecera vertical más todas las columnas (`getWidth`). @return Píxeles. */
    int contentWidth() const;

Q_SIGNALS:
    /** @brief La tabla perdió el foco (`lost_focus`). */
    void lostFocus();

protected:
    /** @brief Clic fuera de las filas: deselecciona. @param ev Evento. */
    void mousePressEvent(QMouseEvent* ev) override;
    /** @brief Avisa al perder el foco. @param ev Evento. */
    void focusOutEvent(QFocusEvent* ev) override;
};

}  // namespace flatcam::gui
