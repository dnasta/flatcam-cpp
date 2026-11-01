/**
 * @file FCTree.h
 * @brief Árbol de dos columnas de FlatCAM (port de `FCTree` de `appGUI/GUIElements.py`).
 */
#pragma once

#include <QTreeWidget>

namespace flatcam::gui {

/**
 * @brief `QTreeWidget` para mostrar propiedades: columnas ajustadas al contenido, filas completas seleccionables y la
 * selección del mismo color con o sin foco.
 */
class FCTree : public QTreeWidget {
    Q_OBJECT
public:
    /**
     * @brief Crea el árbol.
     * @param parent Widget padre.
     * @param columns Cantidad de columnas.
     * @param headerHidden Ocultar la cabecera.
     * @param extendedSel Permitir selección múltiple.
     * @param protectedColumn Columna que no se puede editar con doble clic (-1: ninguna).
     */
    explicit FCTree(QWidget* parent = nullptr, int columns = 2, bool headerHidden = true, bool extendedSel = false,
                    int protectedColumn = -1);

    /**
     * @brief Agrega un elemento con textos y, opcionalmente, en negrita y con color (`addParent` / `addChild`).
     * @param parent Elemento padre, o `nullptr` para la raíz.
     * @param texts Texto de cada columna.
     * @param bold Negrita en la primera columna.
     * @return El elemento creado.
     */
    QTreeWidgetItem* addItem(QTreeWidgetItem* parent, const QStringList& texts, bool bold = false);

private:
    void onDoubleClick(QTreeWidgetItem* item, int column);
    int protectedColumn_;
};

}  // namespace flatcam::gui
