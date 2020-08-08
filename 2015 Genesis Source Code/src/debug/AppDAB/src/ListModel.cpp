#include "ListModel.h"


ListModel::ListModel(ListItem* prototype, QObject *parent) :
    QAbstractListModel(parent), m_prototype(prototype)
{
    setRoleNames(m_prototype->roleNames());
}

int ListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_list.size();
}

QVariant ListModel::data(const QModelIndex &index, int role) const
{
    if(index.row() < 0 || index.row() >= m_list.size())
        return QVariant();
    return m_list.at(index.row())->data(role);
}

ListModel::~ListModel() {
    delete m_prototype;
    clear();
}

void ListModel::appendRow(ListItem *item)
{
    appendRows(QList<ListItem*>() << item);
}

void ListModel::appendRows(const QList<ListItem *> &items)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount()+items.size()-1);
    foreach(ListItem *item, items) {
        connect(item, SIGNAL(dataChanged()), SLOT(handleItemChange()));
        m_list.append(item);
    }
    endInsertRows();
}

void ListModel::insertRow(int row, ListItem *item)
{
    beginInsertRows(QModelIndex(), row, row);
    connect(item, SIGNAL(dataChanged()), SLOT(handleItemChange()));
    m_list.insert(row, item);
    endInsertRows();
}

void ListModel::handleItemChange()
{
    ListItem* item = static_cast<ListItem*>(sender());
    QModelIndex index = indexFromItem(item);
    if(index.isValid())
        emit dataChanged(index, index);
}

ListItem * ListModel::find(const QString &id) const
{
    foreach(ListItem* item, m_list) {
        if(item->id() == id) return item;
    }
    return 0;
}

ListItem * ListModel::findItem(const quint32 freq, const quint32 subChId,const QString &id) const
{
    foreach(ListItem* item, m_list) {
        if(item->key() == freq && item->id() == id && item->subKey() == subChId) return item;
    }
    return NULL;
}

ListItem * ListModel::findItem(const quint32 freq, const QString &id) const
{
    foreach(ListItem* item, m_list) {
        if(item->key() == freq && item->id() == id) return item;
    }
    return NULL;
}

int ListModel::findIndexOnlyPreset(const quint32 freq, const QString ensemble)
{
    int count = 0;
    foreach(ListItem* item, m_list) {
        if(item->ensemble() != ensemble && item->key() == freq) return count;
        count++;
    }
    return -1;
}

int ListModel::findIndex(const quint32 freq, const quint32 subChId,const QString &id)
{
    int count = 0;
    foreach(ListItem* item, m_list) {
        if(item->key() == freq && item->id() == id && item->subKey() == subChId) return count;
        count++;
    }
    return -1;
}

int ListModel::findIndex(const quint32 freq, const QString &id)
{
    int count = 0;
    foreach(ListItem* item, m_list) {
        if(item->key() == freq && item->id() == id) return count;
        count++;
    }
    return -1;
}

int ListModel::findIndex(const quint16 flag)
{
    int index = 0;
    foreach(ListItem* item, m_list) {
        quint16 ausFlags = item->ausFlag();
        if(ausFlags & flag)
            return index;
        else
            index++;
    }
    return -1;
}

int ListModel::findIndex(const QString &id)
{
    int count = 0;
    foreach(ListItem* item, m_list) {
        if(item->id() == id) return count;
        count++;
    }
    return -1;
}

QModelIndex ListModel::indexFromItem(const ListItem *item) const
{
    Q_ASSERT(item);
    for(int row=0; row<m_list.size(); ++row) {
        if(m_list.at(row) == item) return index(row);
    }
    return QModelIndex();
}

void ListModel::clear()
{
    qDeleteAll(m_list);
    m_list.clear();
}

bool ListModel::removeRow(int row, const QModelIndex &parent)
{
    Q_UNUSED(parent);
    if(row < 0 || row >= m_list.size()) return false;
    beginRemoveRows(QModelIndex(), row, row);
    delete m_list.takeAt(row);
    endRemoveRows();
    return true;
}

bool ListModel::removeRows(int row, int count, const QModelIndex &parent)
{
    Q_UNUSED(parent);
    if(row < 0 || (row+count) >= m_list.size()) return false;
    beginRemoveRows(QModelIndex(), row, row+count-1);
    for(int i=0; i<count; ++i) {
        delete m_list.takeAt(row);
    }
    endRemoveRows();
    return true;
}

bool ListModel::removeAllRows(const QModelIndex &parent)
{
    Q_UNUSED(parent);
    bool ret = false;
    int count = m_list.size();
    for(int i = 0; i < count; i++)
    {
        ret = this->removeRow(0);
    }

    return ret;
}

ListItem * ListModel::takeRow(int row)
{
    beginRemoveRows(QModelIndex(), row, row);
    ListItem* item = m_list.takeAt(row);
    endRemoveRows();
    return item;
}

ListItem * ListModel::dataItem(int row) const
{
    ListItem* item = m_list.at(row);
    return item;
}
