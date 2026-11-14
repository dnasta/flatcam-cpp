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

    /**
     * @brief `FCTable(drag_drop=True)`: las filas elegidas se mueven arrastrándolas dentro de la tabla.
     * @param on Activarlo.
     */
    void setDragDrop(bool on);
    /** @brief Si está activo el arrastre de filas. @return `true` si lo está. */
    bool dragDrop() const { return dragDrop_; }
    /**
     * @brief La parte de `dropEvent` que mueve las filas elegidas antes de @p dropRow (`drop_on`): copia las celdas (los
     * combos y casillas se vuelven a crear; cualquier otro widget de celda se pierde, como en el original), quita las
     * filas, las inserta y las deja elegidas; emite @ref dragDropped.
     * @param dropRow Fila delante de la que se sueltan (`rowCount()` al final).
     */
    void moveSelectedRows(int dropRow);
    /** @brief `drop_on`: la fila delante de la que se suelta en @p pos. @param pos Punto del viewport. @return La fila. */
    int dropOn(const QPoint& pos) const;

Q_SIGNALS:
    /** @brief La tabla perdió el foco (`lost_focus`). */
    void lostFocus();
    /**
     * @brief Se soltaron filas (`drag_drop_sig`).
     * @param rowsDragged Las filas elegidas al apretar el mouse (`rows_dragged`). @param dropRow Dónde quedaron.
     */
    void dragDropped(const QList<int>& rowsDragged, int dropRow);

protected:
    /** @brief Solo acepta arrastres desde la propia tabla (y bloquea sus señales hasta soltar). @param e Evento. */
    void dragEnterEvent(QDragEnterEvent* e) override;
    /** @brief Mueve las filas (@ref moveSelectedRows). @param e Evento. */
    void dropEvent(QDropEvent* e) override;
    /** @brief Clic fuera de las filas: deselecciona. @param ev Evento. */
    void mousePressEvent(QMouseEvent* ev) override;
    /** @brief Avisa al perder el foco. @param ev Evento. */
    void focusOutEvent(QFocusEvent* ev) override;

private:
    bool dragDrop_{false};
    QList<int> rowsDragged_;
};

}  // namespace flatcam::gui
