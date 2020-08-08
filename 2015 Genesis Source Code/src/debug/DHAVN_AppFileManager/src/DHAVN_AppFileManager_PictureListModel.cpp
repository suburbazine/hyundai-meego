#include "hdr/DHAVN_AppFileManager_PictureListModel.h"
#include <QDebug>
#include <QMutexLocker>
#include <DHAVN_AppFileManager_Engine.h>
#include "DHAVN_AppFileManager_Thumbnailer.h"//Added by Alexey Edelev 2012.10.25 Fix thumbnails issue

// modified by Dmitry 23.08.13
PictureListModel::PictureListModel(CStateManager *pStateManager, bool bFront, QObject *parent) :
    BaseListModel(parent),
    requestType(eRequestInvalid),
    m_pStateManager(pStateManager),
    abstractor(new CTrackerAbstractor),
    baseFolder(""),
    m_totalFiles(0),
    m_totalFolders(0),
    m_bFront(bFront),
// modified by Dmitry 15.10.13 for ITS0195216
    folderRequestIdx(0),
    m_bLayoutReady(false)
{
    if (!abstractor.isNull())
    {
      this->abstractor.data()->SetFileSystemScanMode(false); // added by sungha.choi 2013.08.18 for blank list after booting
      connect(this->abstractor.data(),
          SIGNAL(RequesteComplete(QVector<QStringList>, ETrackerAbstarctor_Query_Ids, uint)),
          SLOT(onRequestFinished(QVector<QStringList>, ETrackerAbstarctor_Query_Ids, uint)));
      abstractor.data()->SetCountryVariant(AppEngineBase::GetCountryVariant());
      abstractor.data()->SetAVNSWVersion(GetSWVersion()); // added by sungha.choi 2013.08.25 for blank list after booting
    }
    connect(Thumbnailer::instance(), SIGNAL(thumbnailReady(QString,bool,int,bool)), this, SLOT(onThumbnailReady(QString,bool,int,bool)));   // modified by sungha.choi 2013.09.05 for ITS 0188265

    this->roles[PictureThumbPicture1] = "itemThumbPicture1";
    this->roles[PictureItemsCount] = "itemElementsCount";
    this->roles[PictureIsFolder] = "itemIsFolder";

    setRoleNames(this->roles);
}

void PictureListModel::cancel()
{
   requestType = eRequestInvalid;
}

void PictureListModel::onRequestFinished(QVector<QStringList> dataList, ETrackerAbstarctor_Query_Ids queryId, uint reqUID)
{
    MP_MEDIUM << queryId << dataList.count() << LOG_ENDL;
    Thumbnailer::instance()->createDirIfAbsent(); // modified by ravikanth 05-04-13
    QList<PictureModelItem> items;

    switch(queryId)
    {
       case eGetImagesByFolderQuery_Id:
       {
          if ((int)reqUID == requestType + m_bFront) // modified by Dmitry 17.08.13
          {
             if (dataList.size() >= 3)
             {
                QStringList itemNameList    = dataList[0];
                QStringList itemPathList    = dataList[1];
                QStringList folderFlag      = dataList[2];

                QString curPlayFile = m_pStateManager->GetCurrentPlayFolder().prepend("file://").append("/").append(m_pStateManager->GetCurrentPlayName());
                QString curPlayFolder = m_pStateManager->GetCurrentPlayFolder();

                for (int i = 0; i < itemNameList.size(); i++)
                {
                    PictureModelItem item;

                    item.setItemName(itemNameList[i]);
                    item.setItemPath(itemPathList[i]);
                    item.setItemChecked(QBool(false));
                    item.setItemSupported(QBool(true));
                    if (folderFlag[i] == "true")
                        item.setFolder(QBool(true));
                    else
                        item.setThumbPicture1(Thumbnailer::instance()->getThumbnailPath(itemPathList[i]));
                    if (curPlayFile == itemPathList[i] || curPlayFolder == itemPathList[i])
                    {
                       item.setItemPlay(QBool(true));
                       // modified by ravikanth for ITS 0184642
                       if(m_pStateManager->editModeLaunchFromAVP())
                       {
                           item.setItemChecked(QBool(true));
                           setFileURLCount(1);
                       }
                    }
                    items << item;
                }
                append(items);

                for (int i = 0; i < itemNameList.size(); i++)
                {
                    if (folderFlag[i] == "true")
                    {
                        this->abstractor->RequestData(itemPathList[i], eGetFirstImageAndCountByGivenFolder, i);
                        folderRequestIdx = ++i;
                        break;
                    }
                }
             }
          }
          break;
       }

       case eGetFirstImageAndCountByGivenFolder:
       {
          if (dataList.size() >= 1)
          {
             QStringList list = dataList[0];
             //MP_CRITICAL << "ZZZ filePath = " << list[0];
             //MP_CRITICAL << "ZZZ count[0] = " << list[1];
             this->setData(this->index(reqUID), list[1], PictureItemsCount);
             this->setData(this->index(reqUID), Thumbnailer::instance()->getThumbnailPath(list[0]), PictureThumbPicture1);
             Thumbnailer::instance()->generateThumbnail(list[0], reqUID, m_bFront); // added by Dmitry 27.08.13 // modified by sungha.choi 2013.09.05 for ITS 0188265 (added "m_bFront")
          }

          for (int i = folderRequestIdx; i < rowCount(); i++)   // moved by sungha.choi 2013.09.23 for ITS 0190203 (do not show first image & count after folder that has no image)
          {
             if (getListItem(i)->isFolder())
             {
                abstractor->RequestData(getListItem(i)->getItemPath(), eGetFirstImageAndCountByGivenFolder, i);
                folderRequestIdx = ++i;
                break;
             }
          }
          break;
       }

       default:
          break;
    }
}
// modified by Dmitry 22.08.13

void PictureListModel::onError(int error, const QString &errorString)
{
   Q_UNUSED( error )
   Q_UNUSED( errorString )
}

EMediaDataRequestType PictureListModel::getRequestType()
{
    return this->requestType;
}

void PictureListModel::exec(EMediaDataRequestType requestType, const QString& rootFolder)
{
    this->baseFolder = rootFolder;
    this->requestType = requestType;

   setLayoutReady(false); // added by Dmitry 15.10.13 for ITS0195216
   ETrackerAbstarctor_Query_Ids requestId;

   switch ( requestType )
   {
      default:
      {
          requestId = eGetImagesByFolderQuery_Id;
          break;
      }
   }

   this->abstractor->RequestData(this->baseFolder, requestId, requestType + m_bFront);
}
// modified by Dmitry 07.08.13 for ITS0183065

void PictureListModel::addDataItem(const PictureModelItem &data)
{
   int index = 0;

   QMutexLocker(&m_ListItems.m_Mutex);

   // { modified by sungha.choi 2013.07.05 : File manager should use the order from TA because of efficiency. (do not change the order from TA)
   //index = m_ListItems.GetInsertIndex( data );    // deleted by sungha.choi 2013.07.05
   //beginInsertRows(QModelIndex(), index, index);
   //m_ListItems.insert( data, index );
   if(data.isFolder())
   {
      index = m_ListItems.GetInsertIndex( data );
      beginInsertRows(QModelIndex(), index, index);
      m_ListItems.insert( data, index );
   }
   else
   {
       index = m_ListItems.count();
       beginInsertRows(QModelIndex(), index, index);
       m_ListItems.append( data );
   }
   // } modified by sungha.choi 2013.07.05

   endInsertRows();
}

void PictureListModel::append(const QList<PictureModelItem> &list)
{
    if(list.count() < 0) {
        return;
    }

    emit layoutAboutToBeChanged();

    int index;  // added by sungha.choi 2013.07.05
    beginInsertRows(QModelIndex(), rowCount(), ((rowCount() + list.count() - 1) > 15) ? 15 : (rowCount() + list.count() - 1));
    for ( int i = 0; i < list.count(); i++ )
    {
        // { modified by sungha.choi 2013.07.05 : File manager should use the order from TA because of efficiency. (do not change the order from TA)
        //int index = m_ListItems.GetInsertIndex( *tmpItem );   // deleted by sungha.choi 2013.07.05
        //m_ListItems.insert( *tmpItem, index );
        if(list.at(i).isFolder())
        {
            index = m_ListItems.GetInsertIndex( list.at(i) );
            m_ListItems.insert( list.at(i), index );
            m_totalFolders++;
        }
        else
        {
            m_ListItems.append( list.at(i) );
            m_totalFiles++;
        }
        // } modified by sungha.choi 2013.07.05
    }
    endInsertRows();

    emit layoutChanged();
    setLayoutReady(true); // added by Dmitry 15.10.13 for ITS0195216
}

int PictureListModel::rowCount(const QModelIndex & /* parent */) const
{
   return m_ListItems.count();
}

QVariant PictureListModel::data(const QModelIndex & index, int role) const
{
   if ( index.row() < 0 || index.row() > m_ListItems.count() )
   {
      return QVariant();
   }

   const PictureModelItem * data = m_ListItems.GetItemByIndex( index.row() );

   if (role == PictureThumbPicture1)
   {
       QString thumbnailPath = data->getThumbPicture1();
       QFile fileInfo(thumbnailPath);    // added by sungha.choi 2013.09.24 for ITS 0189336

       //if(!QFile::exists(thumbnailPath))
       if(!fileInfo.exists() || fileInfo.size() == 0)    // modified by sungha.choi 2013.09.24 for ITS 0189336
       {
           if(!data->isFolder())
               Thumbnailer::instance()->generateThumbnail(data->getItemPath(), index.row(), m_bFront);  // modified by sungha.choi 2013.09.05 for ITS 0188265
           thumbnailPath = "";
       }
       return thumbnailPath;
   }
   else if (role == PictureIsFolder)
   {
      return (bool) data->isFolder();
   }
   else if (role == PictureItemsCount)
   {
      return data->getItemsCount();
   }

   return BaseListModel::data(index, role);
}

bool PictureListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
   if ( index.row() < 0 || index.row() > m_ListItems.count() )
   {
      return false;
   }

   PictureModelItem * data = m_ListItems.GetItemByIndex( index.row() );

   switch(role)
   {
      case PictureItemsCount:
         data->setItemsCount(value.toInt());
         emit dataChanged(index, index);
         break;

      case PictureThumbPicture1:
         data->setThumbPicture1(value.toString());
         emit dataChanged(index, index);
         break;

      default:
         return BaseListModel::setData( index, value, role );
   }

   return true;
}

void PictureListModel::clear()
{
   setLayoutReady(false); // added by Dmitry 15.10.13 for ITS0195216
   QMutexLocker(&m_ListItems.m_Mutex);

   beginRemoveRows(QModelIndex(), 0, rowCount() - 1);
   m_ListItems.clear();
   endRemoveRows();
   folderRequestIdx = 0; // added by Dmitry 22.08.13
   m_totalFiles = 0;
   m_totalFolders = 0;
   Thumbnailer::instance()->clearQueue();
   clearCheckedElements();
}

bool PictureListModel::updateElement(int index, const PictureModelItem &data)
{
   QMutexLocker(&m_ListItems.m_Mutex);

   if ( index < 0 || index > m_ListItems.count() )
   {
      return false;
   }
   else
   {
      m_ListItems.UpdateItemByIndex( index, data );
      QModelIndex modelIndex = this->index(index);
      emit dataChanged(modelIndex, modelIndex);
   }

   return true;
}

void PictureListModel::updateElementNotifyer( int index )
{
   if ( index < 0 || index > m_ListItems.count() )
   {
      return;
   }
   QModelIndex modelIndex = this->index(index);
   emit dataChanged(modelIndex, modelIndex);
}

const BaseModelItem* PictureListModel::getListItem( int index ) const
{
   if (index < 0 || index > m_ListItems.count() )
   {
      return NULL;
   }

   return m_ListItems.GetItemByIndex( index );
}

BaseModelItem* PictureListModel::getListItem( int index )
{
   if (index < 0 || index > m_ListItems.count() )
   {
      return NULL;
   }

   return m_ListItems.GetItemByIndex( index );
}

PictureModelItem* PictureListModel::getPictureItem( int index ) const//Changed by Alexey Edelev 2012.10.18 CR14766
{
   if (index < 0 || index > m_ListItems.count() )
   {
      return NULL;
   }
   return m_ListItems.GetItemByIndex( index );
}

int PictureListModel::getListCount() const//Changed by Alexey Edelev 2012.10.18 CR14766
{
   return m_ListItems.count();
}

void PictureListModel::removeDataItem( int index )
{
   QMutexLocker(&m_ListItems.m_Mutex);

   if ( index < 0 || index > m_ListItems.count() )
   {
      return;
   }

   //FM_LOG << "PictureListModel::removeDataItem index = " << index << LOG_ENDL;

   beginRemoveRows(QModelIndex(), index, index);
   m_ListItems.RemoveAt( index );
   endRemoveRows();
}

// modified by ravikanth for ITS 0184642
void PictureListModel::setCurrentReference(bool copyinit)
{
    setCheckedRef(copyinit);
}

void PictureListModel::onThumbnailReady(const QString& thumbnail, bool unsupported, int index, bool bFront)  // modified by sungha.choi 2013.09.05 for ITS 0188265
{
    if(bFront == m_bFront) {    // added by sungha.choi 2013.09.05 for ITS 0188265
   !unsupported ? setData(this->index(index), thumbnail, PictureThumbPicture1) :
                  setData(this->index(index), false, BaseSupported);
}
}
// modified by Dmitry 23.08.13

QString PictureListModel::GetSWVersion() // added by sungha.choi 2013.08.25 for blank list after booting
{
    if(m_strSWVersion.isEmpty() == true)
    {
        QFile inFile("/etc/version.txt");
        if(inFile.open(QIODevice::ReadOnly) == false) {
            MP_LOG << "file not open" << LOG_ENDL;
            return ""; // modified by Dmitry 17.08.13
        }

        QTextStream inFileStream(&inFile);
        QString     strVersion;

        inFileStream.skipWhiteSpace();
        inFileStream >> strVersion;

        inFile.close();

        if(strVersion.isEmpty() == true) {
            m_strSWVersion = "unknown";
        }
        else {
            m_strSWVersion = strVersion;
        }
    }

    MP_LOG << m_strSWVersion << LOG_ENDL;

    return m_strSWVersion;
}
