#include "util/DHAVN_AppMediaPlayer_LogHelper.h"
#include "util/video/DHAVN_AppMediaPlayer_Video_ListModelItem.h"
#include "util/video/DHAVN_AppMediaPlayer_Video_ListModel.h"

#include <QStringList>


VListModel::VListModel(QObject *parent) : QAbstractListModel(parent)
{
    roles[Text1Role]     = "text1";
    roles[Text2Role]     = "text2";
    setRoleNames(roles);
    setSelectedIndex(-1);
}


VListModel::~VListModel()
{
}


QVariant VListModel::data(const QModelIndex & index, int role) const
{
    const VListModelItem* item = getListItem(index.row());

    if (item != NULL)
    {
        switch(role)
        {
        case Text1Role:
        {
            MP_LOG << "Text1Role =" << item->text1() << LOG_ENDL;
            return item->text1();
        }
        case Text2Role:
        {
            MP_LOG << "Text2Role =" << item->text2() << LOG_ENDL;
            return item->text2();
        }
        default:
            break;
        }
    }

    return QVariant();
}


QVariant VListModel::data(int index, int role)
{
    const VListModelItem* item = getListItem(index);

    if (item != NULL)
    {
        switch(role)
        {
            case Text1Role:
        {
            MP_LOG << "Text1Role =" << item->text1() << LOG_ENDL;
                return item->text1();
        }
            case Text2Role:
        {
            MP_LOG << "Text2Role =" << item->text2() << LOG_ENDL;
                return item->text2();
        }
            default:
                break;
        }
    }

    return QVariant();
}


bool VListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    VListModelItem* item = getListItem(index.row());

    if (item != NULL)
    {
        switch(role)
        {
            case Text1Role:
            {
                item->setText1(value.toString());
                emit dataChanged(index, index);
                return true;
            }
            case Text2Role:
            {
                item->setText2(value.toString());
                emit dataChanged(index, index);
                return true;
            }
            return true;
        }
    }

    return false;
}


int VListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_itemList.count();
}


void VListModel::addItem(const QString text1, const QString text2)
{
   MP_LOG << "text1 =" << text1 << LOG_ENDL;
   MP_LOG << "text2 =" << text2 << LOG_ENDL;

   MP_LOG << "ROWS START" << LOG_ENDL;
   beginInsertRows(QModelIndex(), rowCount(QModelIndex()), rowCount(QModelIndex()));
   VListModelItem *pNewItem = new VListModelItem(text1, text2);
   m_itemList.append(pNewItem);
   endInsertRows();
   MP_LOG << "ROWS END" << LOG_ENDL;
}


void VListModel::clear()
{
    MP_LOG << "ROWS START" << LOG_ENDL;
    beginRemoveRows(QModelIndex(), 0, rowCount(QModelIndex()) - 1);
    while(!m_itemList.isEmpty())
    {
        VListModelItem* tmp = m_itemList.takeFirst();
        if(tmp)
            delete tmp;
    }
    endRemoveRows();
    MP_LOG << "ROWS END" << LOG_ENDL;
}


VListModelItem* VListModel::getListItem(int index) const
{
    if ( index < 0 || index > m_itemList.count() || m_itemList.count() <= 0 )
    {
        MP_LOG << "something is wrong. Return NULL." << LOG_ENDL;
        return NULL;
    }

    return m_itemList.at(index);
}


void VListModel::updateModel(QStringList *list)
{
    clear();

    for(int i = 0; i < list->count(); i++)
    {
        addItem(list->at(i));
    }
}

