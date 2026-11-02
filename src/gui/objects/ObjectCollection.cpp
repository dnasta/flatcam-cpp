#include "gui/objects/ObjectCollection.h"

#include <QMimeData>
#include <QRegularExpression>

#include "gui/Translation.h"

namespace flatcam::gui {

namespace {

const char* const kMime = "application/x-flatcam-object";

// Los índices de grupo no tienen puntero interno; los de objeto apuntan al objeto.
int kindIndex(ObjectKind k) { return static_cast<int>(k); }

}  // namespace

ObjectCollection::ObjectCollection(QObject* parent) : QAbstractItemModel(parent) {
    // ObjectCollection.groups e icon_files
    titles_ = {tr_("Gerber"), tr_("Excellon"), tr_("Geometry"), "CNC Job", tr_("Script"), tr_("Document")};
    const char* icons[kGroups] = {":/images/flatcam_icon16.png", ":/images/drill16.png", ":/images/geometry16.png",
                                  ":/images/cnc16.png", ":/images/script_new16.png", ":/images/notes16_1.png"};
    for (int i = 0; i < kGroups; ++i) icons_[i] = QPixmap(icons[i]);
}

ObjectCollection::~ObjectCollection() = default;

void ObjectCollection::rename(FlatCAMObj* obj, const QString& name) {
    if (!obj || name.isEmpty() || name == obj->name()) return;
    obj->setName(name);
    const QModelIndex idx = indexOf(obj);
    Q_EMIT dataChanged(idx, idx);
}

QStringList ObjectCollection::names() const {
    QStringList out;
    for (const auto& g : groups_)
        for (const auto& o : g) out << o->name();
    return out;
}

std::vector<FlatCAMObj*> ObjectCollection::objects() const {
    std::vector<FlatCAMObj*> out;
    for (const auto& g : groups_)
        for (const auto& o : g) out.push_back(o.get());
    return out;
}

FlatCAMObj* ObjectCollection::byName(const QString& name) const {
    for (FlatCAMObj* o : objects())
        if (o->name() == name) return o;
    return nullptr;
}

FlatCAMObj* ObjectCollection::append(std::unique_ptr<FlatCAMObj> obj) {
    // Mismo nombre: si termina en número se incrementa; si no, se agrega "_1" (ObjectCollection.append).
    QString name = obj->name();
    static const QRegularExpression re(R"(^(.*[^\d])?(\d+)$)");
    while (names().contains(name)) {
        const auto m = re.match(name);
        if (m.hasMatch()) name = m.captured(1) + QString::number(m.captured(2).toLongLong() + 1);
        else name += "_1";
    }
    obj->setName(name);
    const int g = kindIndex(obj->kind());
    const int row = static_cast<int>(groups_[g].size());
    beginInsertRows(groupIndex(obj->kind()), row, row);
    groups_[g].push_back(std::move(obj));
    endInsertRows();
    return groups_[g].back().get();
}

void ObjectCollection::remove(FlatCAMObj* obj) {
    for (int g = 0; g < kGroups; ++g) {
        auto& v = groups_[g];
        for (int r = 0; r < static_cast<int>(v.size()); ++r) {
            if (v[r].get() != obj) continue;
            beginRemoveRows(index(g, 0), r, r);
            v.erase(v.begin() + r);
            endRemoveRows();
            return;
        }
    }
}

QModelIndex ObjectCollection::indexOf(FlatCAMObj* obj) const {
    if (!obj) return {};
    const auto& v = groups_[kindIndex(obj->kind())];
    for (int r = 0; r < static_cast<int>(v.size()); ++r)
        if (v[r].get() == obj) return index(r, 0, groupIndex(obj->kind()));
    return {};
}

QModelIndex ObjectCollection::groupIndex(ObjectKind kind) const { return index(kindIndex(kind), 0); }

FlatCAMObj* ObjectCollection::objectAt(const QModelIndex& idx) const {
    return idx.isValid() ? static_cast<FlatCAMObj*>(idx.internalPointer()) : nullptr;
}

QModelIndex ObjectCollection::index(int row, int column, const QModelIndex& parent) const {
    if (column != 0 || row < 0) return {};
    if (!parent.isValid()) return row < kGroups ? createIndex(row, 0, nullptr) : QModelIndex();
    if (parent.internalPointer() != nullptr) return {};  // los objetos no tienen hijos
    const auto& v = groups_[parent.row()];
    return row < static_cast<int>(v.size()) ? createIndex(row, 0, v[row].get()) : QModelIndex();
}

QModelIndex ObjectCollection::parent(const QModelIndex& child) const {
    FlatCAMObj* o = objectAt(child);
    return o ? groupIndex(o->kind()) : QModelIndex();
}

int ObjectCollection::rowCount(const QModelIndex& parent) const {
    if (!parent.isValid()) return kGroups;
    if (parent.internalPointer() != nullptr) return 0;
    return static_cast<int>(groups_[parent.row()].size());
}

int ObjectCollection::columnCount(const QModelIndex&) const { return 1; }

QVariant ObjectCollection::data(const QModelIndex& idx, int role) const {
    if (!idx.isValid()) return {};
    FlatCAMObj* o = objectAt(idx);
    const int g = o ? kindIndex(o->kind()) : idx.row();
    switch (role) {
        case Qt::DisplayRole:
        case Qt::EditRole:
            return o ? o->name() : titles_[g];
        case Qt::DecorationRole:
            return icons_[g];
        case Qt::ToolTipRole:
            return o ? QVariant(o->name()) : QVariant();
        default:
            return {};
    }
}

bool ObjectCollection::setData(const QModelIndex& idx, const QVariant& value, int role) {
    FlatCAMObj* o = objectAt(idx);
    if (!o || role != Qt::EditRole) return false;
    const QString oldName = o->name(), newName = value.toString();
    if (oldName != newName && !newName.isEmpty()) {
        o->setName(newName);
        Q_EMIT objectRenamed(o, oldName);
    }
    Q_EMIT dataChanged(idx, idx);
    return true;
}

Qt::ItemFlags ObjectCollection::flags(const QModelIndex& idx) const {
    if (!idx.isValid()) return Qt::ItemIsEnabled;
    // Los grupos no se seleccionan; reciben objetos soltados para poder reordenar entre filas.
    if (!objectAt(idx)) return Qt::ItemIsEnabled | Qt::ItemIsDropEnabled;
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsDragEnabled |
           Qt::ItemIsDropEnabled;
}

Qt::DropActions ObjectCollection::supportedDropActions() const { return Qt::MoveAction; }

QStringList ObjectCollection::mimeTypes() const { return {kMime}; }

QMimeData* ObjectCollection::mimeData(const QModelIndexList& indexes) const {
    // Se arrastra un solo objeto (el primero de la selección): grupo y fila.
    for (const QModelIndex& i : indexes) {
        FlatCAMObj* o = objectAt(i);
        if (!o) continue;
        auto* m = new QMimeData;
        m->setData(kMime, QByteArray::number(kindIndex(o->kind())) + ',' + QByteArray::number(i.row()));
        return m;
    }
    return nullptr;
}

std::pair<FlatCAMObj*, int> ObjectCollection::dropTarget(const QMimeData* data, int row,
                                                         const QModelIndex& parent) const {
    if (!data || !data->hasFormat(kMime)) return {nullptr, -1};
    const QList<QByteArray> f = data->data(kMime).split(',');
    if (f.size() != 2) return {nullptr, -1};
    const int g = f[0].toInt(), r = f[1].toInt();
    if (g < 0 || g >= kGroups || r < 0 || r >= static_cast<int>(groups_[g].size())) return {nullptr, -1};
    FlatCAMObj* o = groups_[g][r].get();
    // Soltar sobre otro objeto lo pone en su lugar; entre filas del grupo, en esa posición; sobre el grupo, al final.
    if (FlatCAMObj* target = objectAt(parent)) {
        if (kindIndex(target->kind()) != g) return {nullptr, -1};
        const int tr = parent.row();
        return {o, tr > r ? tr + 1 : tr};
    }
    if (!parent.isValid() || parent.row() != g) return {nullptr, -1};
    return {o, row < 0 ? static_cast<int>(groups_[g].size()) : row};
}

bool ObjectCollection::canDropMimeData(const QMimeData* data, Qt::DropAction action, int row, int,
                                       const QModelIndex& parent) const {
    return action == Qt::MoveAction && dropTarget(data, row, parent).first != nullptr;
}

bool ObjectCollection::dropMimeData(const QMimeData* data, Qt::DropAction action, int row, int,
                                    const QModelIndex& parent) {
    if (action != Qt::MoveAction) return false;
    const auto [o, to] = dropTarget(data, row, parent);
    // Se devuelve false aunque se haya movido: así la vista no intenta borrar la fila de origen.
    if (o) moveObject(o, to);
    return false;
}

bool ObjectCollection::moveObject(FlatCAMObj* obj, int row) {
    const QModelIndex idx = indexOf(obj);
    if (!idx.isValid()) return false;
    auto& v = groups_[kindIndex(obj->kind())];
    const int n = static_cast<int>(v.size()), from = idx.row();
    if (row < 0 || row > n) row = n;
    if (row == from || row == from + 1) return false;  // queda donde estaba
    const QModelIndex g = groupIndex(obj->kind());
    if (!beginMoveRows(g, from, from, g, row)) return false;
    auto item = std::move(v[from]);
    v.erase(v.begin() + from);
    v.insert(v.begin() + (row > from ? row - 1 : row), std::move(item));
    endMoveRows();
    Q_EMIT orderChanged();
    return true;
}

std::vector<int> ObjectCollection::drawOrder() const {
    std::vector<int> out;
    for (const auto& g : groups_)
        for (auto it = g.rbegin(); it != g.rend(); ++it) out.push_back((*it)->id());
    return out;
}

}  // namespace flatcam::gui
