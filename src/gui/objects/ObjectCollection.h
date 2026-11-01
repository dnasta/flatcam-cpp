/**
 * @file ObjectCollection.h
 * @brief Árbol de objetos del proyecto (port de `ObjectCollection` de `appObjects/ObjectCollection.py`).
 */
#pragma once

#include <QAbstractItemModel>
#include <QPixmap>
#include <array>
#include <memory>
#include <vector>

#include "gui/objects/FlatCAMObj.h"

namespace flatcam::gui {

/**
 * @brief Modelo de la pestaña Proyecto: seis grupos (Gerber, Excellon, Geometría, CNC Job, Secuencia de Comandos y
 * Documento) con los objetos de cada tipo debajo.
 *
 * Igual que el original: los grupos tienen ícono y no se pueden seleccionar; los objetos muestran su nombre (también
 * como tooltip), el ícono de su grupo y se pueden renombrar; los nombres repetidos se cambian al agregar ("x" pasa a
 * "x_1"; "x1" a "x2").
 */
class ObjectCollection : public QAbstractItemModel {
    Q_OBJECT
public:
    /** @brief Crea la colección vacía. @param parent Objeto padre. */
    explicit ObjectCollection(QObject* parent = nullptr);
    ~ObjectCollection() override;

    /**
     * @brief Agrega un objeto (`append`), cambiándole el nombre si ya existe.
     * @param obj Objeto (la colección pasa a ser su dueña).
     * @return El objeto agregado.
     */
    FlatCAMObj* append(std::unique_ptr<FlatCAMObj> obj);

    /** @brief Quita y destruye un objeto. @param obj Objeto. */
    void remove(FlatCAMObj* obj);

    /** @brief Nombres de todos los objetos (`get_names`). @return Los nombres. */
    QStringList names() const;

    /** @brief Todos los objetos, en el orden de los grupos (`get_list`). @return Los objetos. */
    std::vector<FlatCAMObj*> objects() const;

    /** @brief Busca por nombre (`get_by_name`). @param name Nombre. @return El objeto o `nullptr`. */
    FlatCAMObj* byName(const QString& name) const;

    /** @brief El objeto de un índice del modelo, o `nullptr` si es un grupo. @param index Índice. @return El objeto. */
    FlatCAMObj* objectAt(const QModelIndex& index) const;

    /** @brief Índice del grupo de un tipo (para expandirlo). @param kind Tipo. @return El índice. */
    QModelIndex groupIndex(ObjectKind kind) const;

    /** @name Interfaz de QAbstractItemModel */
    ///@{
    /** @brief Índice hijo. @param row Fila. @param column Columna. @param parent Padre. @return El índice. */
    QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const override;
    /** @brief Índice padre. @param child Hijo. @return El padre. */
    QModelIndex parent(const QModelIndex& child) const override;
    /** @brief Filas. @param parent Padre. @return Cantidad. */
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    /** @brief Columnas (una). @param parent Padre. @return 1. */
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    /** @brief Datos de un elemento. @param index Índice. @param role Rol. @return El dato. */
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    /** @brief Renombra un objeto. @param index Índice. @param value Nombre. @param role Rol. @return Si cambió. */
    bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;
    /** @brief Banderas: los grupos no se seleccionan. @param index Índice. @return Banderas. */
    Qt::ItemFlags flags(const QModelIndex& index) const override;
    ///@}

Q_SIGNALS:
    /** @brief Se renombró un objeto. @param obj Objeto. @param oldName Nombre anterior. */
    void objectRenamed(FlatCAMObj* obj, const QString& oldName);

private:
    static constexpr int kGroups = 6;
    std::array<std::vector<std::unique_ptr<FlatCAMObj>>, kGroups> groups_;
    std::array<QString, kGroups> titles_;
    std::array<QPixmap, kGroups> icons_;
};

}  // namespace flatcam::gui
