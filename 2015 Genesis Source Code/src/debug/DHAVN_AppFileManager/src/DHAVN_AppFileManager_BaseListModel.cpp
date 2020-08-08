#include "hdr/DHAVN_AppFileManager_BaseListModel.h"
#include <QDebug>

// modified by Dmitry 17.08.13
BaseListModel::BaseListModel(QObject *parent) :
    QAbstractListModel(parent),
    checkedRef(0),
    isFileSelected(false),
    CheckBoxSelectable_count(0)
{
    // FM_LOG_INIT_V2("AppFileManager");
    this->roles[BaseNameRole] = "itemName";
    this->roles[BaseIdRole] = "itemId";
    this->roles[BaseCheckedRole] = "itemChecked";
    this->roles[BasePathRole] = "itemPath";
    this->roles[BasePlayed] = "itemPlayed";
    this->roles[BaseSupported] = "itemSupported"; // added by Dmitry 23.08.13
}

QVariant BaseListModel::data(const QModelIndex & index, int role) const
{
    const BaseModelItem* data = getListItem(index.row());
    if (data != NULL)
    {
        if (role == BaseNameRole)
        {
            return data->getItemName();
        }
        else if (role == BaseIdRole)
        {
            return data->getItemId();
        }
        else if (role == BaseCheckedRole)
        {
            return (bool) data->getItemChecked();
        }
        else if (role == BasePathRole)
        {
            return data->getItemPath();
        }
        else if (role == BasePlayed)
        {
            return (bool) data->getItemPlay();
        }
// added by Dmitry 23.08.13
        else if (role == BaseSupported)
        {
           return (bool) data->getItemSupported();
        }
    }

    return QVariant();
}

bool BaseListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role == BaseCheckedRole)
    {
        BaseModelItem *item = getListItem(index.row());
        if (item != NULL)
        {
            bool v = value.toBool();

            if (item->getItemChecked() && !v)
            {
                if(this->checkedRef > 0) // modified by ravikanth for ITS 0184642
                    this->checkedRef--;
            }
            else if (!item->getItemChecked() && v)
            {
                this->checkedRef++;
            }

            item->setItemChecked(QBool(v));

            emit dataChanged(index, index);
            return true;
        }
    }
    else if(role == BasePlayed)
    {
       BaseModelItem *item = getListItem(index.row());
       if (item != NULL)
       {
          bool v = value.toBool();
          item->setItemPlay(QBool(v));

          emit dataChanged(index, index);
          return true;
       }
    }
// added by Dmitry 23.08.13
    else if (role == BaseSupported)
    {
       BaseModelItem *item = getListItem(index.row());
       if (item != NULL)
       {
          item->setItemSupported(QBool(value.toBool()));
          emit dataChanged(index, index);
          return true;
       }
    }

    return false;
}

Q_INVOKABLE int BaseListModel::getPlayedItemIndex()
{
    int res_index = -1;
    int i = 0;

    while( ( i < getListCount() ) && ( res_index == -1 ) )
    {
        BaseModelItem* item = getListItem(i);
        if (item != NULL && item->getItemPlay())
        {
            res_index = i;
        }
        i++;
    }

    return res_index;
}

Q_INVOKABLE void BaseListModel::checkElement( int index, bool isChecked)
{
   bool previousFlag = checkedRef;

   QModelIndex modelIndex = this->index(index);
   setData(modelIndex, isChecked, BaseListModel::BaseCheckedRole);
   isChecked ? this->CheckBoxSelectable_count++ : this->CheckBoxSelectable_count--; //added by yungi 2013.03.08 for New UX FileCount

   if ( previousFlag != isChecked )
   {
      emit checkedItemsUpdated(checkedRef);
   }

   if (this->checkedRef == 1)
   {
       if (isChecked)
       {
           BaseModelItem* item = getListItem(index);
           notifyFileSelected(item);
       }
       else
       {
           for (int i = 0; i < getListCount(); i++)
           {
               BaseModelItem* item = getListItem(i);
               if (item != NULL && item->getItemChecked())
               {
                   notifyFileSelected(item);
               }
           }
       }
   }
   else
   {
       emit fileSelectedUpdated(false);
   }

   // MP_LOG << "BaseListModel::checkElement: item" << index << "   " << isChecked << LOG_ENDL;
}

bool BaseListModel::getItemsAreChecked() const
{
   //MP_LOG << "BaseListModel::getItemsAreChecked checkedref = " << checkedRef << LOG_ENDL;
   return checkedRef;
}

void BaseListModel::clearCheckedElements()
{
   int oldValue = checkedRef;

   // MP_LOG << "BaseListModel::clearCheckedElements checkedref = " << oldValue << LOG_ENDL;

   for (int i = 0; i < getListCount(); i++)
   {
       QModelIndex modelIndex = index(i);
       setData(modelIndex, false, BaseListModel::BaseCheckedRole);
   }

   this->checkedRef = 0;
   this->isFileSelected = false;
   this->CheckBoxSelectable_count = 0; // modified by ravikanth 06-07-13 for Sanity fix selected items logic was not proper.
   if ( oldValue )
   {
      emit checkedItemsUpdated(0);
      emit fileSelectedUpdated(false);
   }
}

void BaseListModel::removeDataItem(int)
{
   // MP_LOG << "BaseListModel::removeDataItem  function is not implemented" << LOG_ENDL;
}

bool BaseListModel::getFileSelected() const
{
    //MP_LOG << "BaseListModel::getFileSelected() checked count=" << this->checkedRef << LOG_ENDL;
    return (this->checkedRef == 1) && this->isFileSelected;
}

void BaseListModel::notifyFileSelected(const BaseModelItem *item)
{
    if (item != NULL && !item->isFolder())
    {
        this->isFileSelected = true;
        emit fileSelectedUpdated(true);
    }
    else
    {
        this->isFileSelected = false;
        emit fileSelectedUpdated(false);
    }
}

// modified by ravikanth 26-06-13 for ITS 0175972
bool BaseListModel::isElementChecked( int index)
{
    if (index >= 0 && index < getListCount())
    {
        BaseModelItem* item = getListItem(index);
        if (item != NULL )
            return item->getItemChecked();
    }
    return false;
}

// modified by ravikanth 28.08.13 for ITS 0184642
void BaseListModel::setCheckedRef(bool copyinit)
{
    this->checkedRef = copyinit ? 1 : this->checkedRef;
    //MP_LOG << this->checkedRef << LOG_ENDL;
    emit checkedItemsUpdated(this->checkedRef);
}
