/**
 * @file FCTree.h
 * @brief Árbol de dos columnas de FlatCAM (port de `FCTree` de `appGUI/GUIElements.py`).
 */
#pragma once

#include <QFont>
#include <QTreeWidget>
#include <optional>
#include <vector>

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

    /**
     * @brief `addParent`: un elemento con un título, que siempre muestra el indicador de hijos.
     * @param parent Padre (`nullptr`: la raíz invisible).
     * @param title Título.
     * @param expanded Desplegado.
     * @param color Color del texto (inválido: el de siempre).
     * @param font Letra (vacía: la de siempre).
     * @return El elemento.
     */
    QTreeWidgetItem* addParent(QTreeWidgetItem* parent, const QString& title, bool expanded = false,
                               const QColor& color = QColor(), const std::optional<QFont>& font = std::nullopt);

    /**
     * @brief `addChild`: un elemento con el texto de la columna 0 y, si @p column1, el de la 1.
     * @param parent Padre.
     * @param title Textos.
     * @param column1 Si se escribe la segunda columna.
     * @param font Letra para las columnas @p fontItems.
     * @param fontItems Columnas con esa letra.
     */
    void addChild(QTreeWidgetItem* parent, const QStringList& title, bool column1 = false,
                  const std::optional<QFont>& font = std::nullopt, std::vector<int> fontItems = {});

    /** @brief `on_resize`: cada columna al ancho de su contenido y después ajustable a mano. */
    void onResize();

protected:
    /** @brief Ajusta las columnas al cambiar de tamaño (`resizeEvent`). @param event Evento. */
    void resizeEvent(QResizeEvent* event) override;

private:
    void onDoubleClick(QTreeWidgetItem* item, int column);
    int protectedColumn_;
};

}  // namespace flatcam::gui
