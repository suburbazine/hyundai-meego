#include "util/DHAVN_AppMediaPlayer_LogHelper.h"
#include "util/video/DHAVN_AppMediaPlayer_RadioListModelItem.h"
#include "util/video/DHAVN_AppMediaPlayer_RadioListModel.h"

#include <QStringList>

RadioListModel::RadioListModel(QObject *parent) : QAbstractListModel(parent)
{
    /* These roles are required by RadioList plugin. */
    roles[NameRole]     = "title_of_radiobutton";
    roles[CommentRole]  = "comment_of_radiobutton";
    roles[EnableRole]   = "enable";
    setRoleNames(roles);
    setSelectedIndex(-1);
}


RadioListModel::~RadioListModel()
{
}


QVariant RadioListModel::data(const QModelIndex & index, int role) const
{
    const RadioListModelItem* item = getListItem(index.row());

    if (item != NULL)
    {
        switch(role)
        {
            case NameRole:
                return item->getItemName();
            case CommentRole:
                return "";
            case EnableRole:
                // { modified by kihyung 2012.08.10 for CR 12705
                // return item->getEnable(); // added by kihyung 2012.07.24 for CR 12054
                return true;
                // } modified by kihyung
            default:
                break;
        }
    }

    return QVariant();
}


bool RadioListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    RadioListModelItem* item = getListItem(index.row());

    if (item != NULL)
    {
        switch(role)
        {
            case NameRole:
            {
                item->setItemName(value.toString());
                emit dataChanged(index, index);
                return true;
            }
            // { modified by kihyung 2012.08.10 for CR 12705
            // { modified by kihyung 2012.07.24 for CR 12054
            case CommentRole:
            case EnableRole:
                return true;
            /*
            case CommentRole:
            {
                return true;
            }
            case EnableRole:
            {
                item->setEnable(value.toBool());
                emit dataChanged(index, index);
                return true;
            } 
            */
            // } modified by kihyung
            // } modified by kihyung
        }
    }

    return false;
}


int RadioListModel::rowCount(const QModelIndex & /* parent */) const
{
    return m_itemList.count();
}


void RadioListModel::addItem(const QString name)
{
   beginInsertRows(QModelIndex(), rowCount(QModelIndex()), rowCount(QModelIndex()));
   RadioListModelItem *pNewItem = new RadioListModelItem(name);
   m_itemList.append(pNewItem);
   endInsertRows();
}


void RadioListModel::clear()
{
    beginRemoveRows(QModelIndex(), 0, rowCount(QModelIndex()) - 1);
    while(!m_itemList.isEmpty())
    {
        RadioListModelItem* tmp = m_itemList.takeFirst();
        if(tmp)
            delete tmp;
    }
    endRemoveRows();
}


RadioListModelItem* RadioListModel::getListItem(int index) const
{
    if ( index < 0 || index > m_itemList.count() || m_itemList.count() <= 0 )
    {
        MP_LOG << "something is wrong. Return NULL." << LOG_ENDL;
        return NULL;
    }

    return m_itemList.at(index);
}

void RadioListModel::updateModel(QStringList list, int selectedIndex)
{
    setSelectedIndex(-1);
    clear();

    for(int i = 0; i < list.count(); i++)
    {
        addItem(list.at(i));
    }

    setSelectedIndex(selectedIndex);
}
 // } modified by Sergey 20.08.2013 for ITS#184640 
