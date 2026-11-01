#include "gui/objects/ObjectCollection.h"

#include <QRegularExpression>

#include "gui/Translation.h"

namespace flatcam::gui {

namespace {

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
    if (!idx.isValid() || !objectAt(idx)) return Qt::ItemIsEnabled;  // los grupos no se seleccionan
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsDragEnabled |
           Qt::ItemIsDropEnabled;
}

}  // namespace flatcam::gui
