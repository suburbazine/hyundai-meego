#include "hdr/DHAVN_AppFileManager_VideoListModel.h"
#include <QDateTime>

#include <QDebug>
#include <QMutexLocker>
#include <QUrl>
#include <QDBusPendingCall>
#include "hdr/DHAVN_AppFileManager_Logger.h"

VideoListModel::VideoListModel(CStateManager *pStateManager, bool bFront, QObject *parent) :
    BaseListModel(parent),
    requestType(eRequestInvalid),
    m_pStateManager(pStateManager),
    m_Index(0),
    m_pTrackerProcessor(new QTrackerProcessor((bFront == true) ? 1 : 0)),
    m_requestId(0),
    m_totalFiles(0),
// modified by Dmitry 15.10.13 for ITS0195216
    m_totalFolders(0),
    m_bLayoutReady(false)
{
// modified by Dmitry 23.08.13
    connect(this, SIGNAL(requestThumbnails(QStringList,int)), m_pTrackerProcessor, SLOT(onRequestThumbnails(QStringList,int)));
    connect(this, SIGNAL(requestFolderContent(QString,int,int)), m_pTrackerProcessor, SLOT(onRequestFolderContent(QString,int,int)));
    connect(m_pTrackerProcessor, SIGNAL(thumbnailReceived(int,QString)), this, SLOT(onThumbnailReceived(int,QString)));
    connect(m_pTrackerProcessor,SIGNAL(folderContentReceived(QVector<QStringList>,int)), this, SLOT(onFolderContentReceived(QVector<QStringList>,int)));

    this->roles[VideoThumbPicture1] = "itemThumbPicture1";
    this->roles[VideoThumbPicture2] = "itemThumbPicture2";
    this->roles[VideoItemsCount] = "itemElementsCount";
    this->roles[VideoIsFolder] = "itemIsFolder";

    //m_pTrackerProcessor->moveToThread(&m_trackerThread);
    //m_pTrackerProcessor->getAbstractor()->moveToThread(&m_trackerThread);   // added by sungha.choi 2013.08.06 for ITS0182950 AFM crash issue
    //m_trackerThread.setObjectName("m_trackerThread");   // added by sungha.choi 2013.08.06
    //m_trackerThread.start(QThread::IdlePriority);
    setRoleNames(this->roles);
}

void VideoListModel::cancel()
{
   requestType = eRequestInvalid;
}

EMediaDataRequestType VideoListModel::getRequestType()
{
    return this->requestType;
}

void VideoListModel::onError(int error, const QString &errorString)
{
   Q_UNUSED( error )
   Q_UNUSED( errorString )
}
// modified by Dmitry 26.07.13
void VideoListModel::onFolderContentReceived(QVector<QStringList> dataList, int reqId)
{
   //MP_CRITICAL << "ZZZ onFolderContentReceived reqId = " << reqId;
   //MP_CRITICAL << "ZZZ onFolderContentReceived m_requestId = " << m_requestId;

   if (m_requestId != reqId) return;

   QList<VideoModelItem> items;

   items.clear();
   m_Index = 0;

    if(dataList.size() < 3) {
        MP_CRITICAL << " Error dataList.size() is " << dataList.size() << LOG_ENDL;
        return;
    }

// removed by Dmitry 29.07.13
    QStringList itemNameList    = dataList[0];
    QStringList itemPathList    = dataList[1];
    QStringList folderFlag      = dataList[2];


    //MP_CRITICAL << "list size " << itemNameList.size() << " for " << this->baseFolder;

// modified by Dmitry 29.07.13
    QString curPlayFile = m_pStateManager->GetCurrentPlayFolder().prepend("file://").append("/").append(m_pStateManager->GetCurrentPlayName());
    QString curPlayFolder = m_pStateManager->GetCurrentPlayFolder();
    for(int i = 0; i < itemNameList.size(); i++)
    {
        VideoModelItem item;
        item.setItemName(itemNameList[i]);
        item.setItemPath(itemPathList[i]);
        item.setItemChecked(QBool(false));
        if (folderFlag[i] == "true") item.setFolder(QBool(true));
        if (curPlayFile == itemPathList[i] || curPlayFolder == itemPathList[i])
        {
           m_Index = i;
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
// modified by Dmitry 29.07.13

    //MP_CRITICAL << "ZZZ m_Index = " << m_Index;
    if(items.size() > 0)
    {
         append(items);
         int start = m_Index - 5 < 0 ? 0 : m_Index - 5;
         int end = start + 10 >= m_ListItems.count() ? m_ListItems.count() : start + 10;
         for (int i = start; i < end; i++)
         {
            VideoModelItem* item = m_ListItems.GetItemByIndex(i);
            if (item && item->getThumbPicture1().isEmpty())
            {
// removed by Dmitry 22.08.13
               emit requestThumbnails(QStringList(item->getItemPath()), i);
            }
         }
    }
    else
    {
        emit layoutChanged(); // modified for ITS 0204918
    }
    if (m_requestId > 100) m_requestId = 0;
}

void VideoListModel::exec(EMediaDataRequestType requestType, const QString& rootFolder)
{
    MP_LOG << "exec" << requestType << rootFolder << LOG_ENDL;

    setLayoutReady(false); // added by Dmitry 15.10.13 for ITS0195216
    this->baseFolder = rootFolder;
    this->requestType = requestType;

    if(requestType == eVideoFilesInFolderRequest)
    {
        m_requestId++;
        // added by Dmitry 23.10.13
        QList<QVariant> arg1;
        arg1 << qVariantFromValue(baseFolder);
        QDBusMessage message = QDBusMessage::createMethodCall("org.lge.TrackerExtractor", "/", "org.lge.TrackerExtractor", "generateTNFromPath");
        message.setArguments(arg1);
        m_pStateManager->getEngine()->GetTrackerExtractorClient()->getConnection().asyncCall(message);
        emit requestFolderContent(this->baseFolder, requestType, m_requestId);
    }
    else
    {
        MP_CRITICAL << "requestType error" << requestType << LOG_ENDL;
    }
}

void VideoListModel::addDataItem(const VideoModelItem &data)
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

void VideoListModel::append(const QList<VideoModelItem> &list)
{
    if(list.count() <= 0 ) {
        return;
    }

    emit layoutAboutToBeChanged();

    int index;  // added by sungha.choi 2013.07.05
    beginInsertRows(QModelIndex(), (m_Index - 10) < 0 ? 0 : m_Index - 10,
                    (m_Index + 10) > (rowCount() + list.count() - 1) ? rowCount() + list.count() - 1 : m_Index + 10);
    for ( int i = 0; i < list.count(); i++ )
    {
        // { modified by sungha.choi 2013.07.05 : File manager should use the order from TA because of efficiency. (do not change the order from TA)
        //int index = m_ListItems.GetInsertIndex( *tmpItem );   // deleted by sungha.choi 2013.07.05
        //m_ListItems.insert( *tmpItem, index );
        if(list.at(i).isFolder())
        {
            m_totalFolders++;
            index = m_ListItems.GetInsertIndex( list.at(i));
            m_ListItems.insert( list.at(i), index );
        }
        else
        {
            m_totalFiles++;
            m_ListItems.append( list.at(i) );
        }
        // } modified by sungha.choi 2013.07.05
    }
    endInsertRows();

    emit layoutChanged();
    setLayoutReady(true); // added by Dmitry 15.10.13 for ITS0195216
}

int VideoListModel::rowCount(const QModelIndex & /* parent */) const
{
    return m_ListItems.count();
}

QVariant VideoListModel::data(const QModelIndex & index, int role) const
{
   const VideoModelItem * data = m_ListItems.GetItemByIndex(index.row());

   if (data)
   {
      if (role == VideoItemsCount)
      {
         return data->getItemsCount();
      }
      else if (role == VideoThumbPicture1)
      {
         return QUrl::fromEncoded(QUrl(data->getThumbPicture1()).toEncoded().replace("#", "%23"));
      }
      else if (role == VideoThumbPicture2)
      {
         return QUrl::fromEncoded(QUrl(data->getThumbPicture2()).toEncoded().replace("#", "%23"));
      }
      else if (role == VideoIsFolder)
      {
         return (bool) data->isFolder();
      }

      return BaseListModel::data( index, role );
   }

   return QVariant();
}

bool VideoListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
   VideoModelItem * data = m_ListItems.GetItemByIndex(index.row());

   if (data)
   {
      switch (role)
      {
         case VideoThumbPicture1:
            data->setThumbPicture1(value.toString());
            emit dataChanged(index, index);
            break;

         default:
            return BaseListModel::setData( index, value, role );
      }
   }

   return false;
}

void VideoListModel::clear()
{
   //MP_CRITICAL << "ZZZ clear";
   setLayoutReady(false); // added by Dmitry 15.10.13 for ITS0195216
   QMutexLocker(&m_ListItems.m_Mutex);
   beginRemoveRows(QModelIndex(), 0, rowCount() - 1);
   m_ListItems.clear();
   endRemoveRows();
   m_totalFiles = 0;
   m_totalFolders = 0;
   clearCheckedElements();
}

bool VideoListModel::updateElement(int index, const VideoModelItem &data)
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

void VideoListModel::updateElementNotifyer( int index )
{
   if ( index < 0 || index > m_ListItems.count() )
   {
      return;
   }
   QModelIndex modelIndex = this->index(index);
   emit dataChanged(modelIndex, modelIndex);
}

const BaseModelItem* VideoListModel::getListItem(int index) const
{
   if ( index < 0 || index > m_ListItems.count() )
   {
      return NULL;
   }

   return m_ListItems.GetItemByIndex( index );
}

BaseModelItem* VideoListModel::getListItem(int index)
{
   if ( index < 0 || index > m_ListItems.count() )
   {
      return NULL;
   }

   return m_ListItems.GetItemByIndex( index );
}

VideoModelItem* VideoListModel::getVideoItem(int index) const//Changed by Alexey Edelev 2012.10.18 CR14766
{
   if ( index < 0 || index > m_ListItems.count() )
   {
      return NULL;
   }
   return m_ListItems.GetItemByIndex( index );
}

int VideoListModel::getListCount() const//Changed by Alexey Edelev 2012.10.18 CR14766
{
   return m_ListItems.count();
}

void VideoListModel::removeDataItem(int index)
{
   QMutexLocker(&m_ListItems.m_Mutex);

   if ( index < 0 || index > m_ListItems.count() )
   {
      return;
   }

   // MP_LOG << "VideoListModel::removeDataItem index = " << index << LOG_ENDL;

   beginRemoveRows(QModelIndex(), index, index);
   m_ListItems.RemoveAt( index );
   endRemoveRows();
}

void VideoListModel::onThumbnailReceived(int index, QString path)
{
   setData(this->index(index), QVariant(path), VideoThumbPicture1);
}

void VideoListModel::updateThumbnails(int start, int end)
{
   if (start < 0) start = 0;
   end = start + end >= m_ListItems.count() ? m_ListItems.count() : start + end;
   //MP_LOG << "ZZZ start = " << start << LOG_ENDL;
   //MP_LOG << "ZZZ end = " << end << LOG_ENDL;
   for (int i = start; i < end; i++)
   {
      VideoModelItem* item = m_ListItems.GetItemByIndex(i);
      if (item && item->getThumbPicture1().isEmpty())
      {
         emit requestThumbnails(QStringList(item->getItemPath()), i);
      }
   }
}

// modified by ravikanth for ITS 0184642
void VideoListModel::setCurrentReference(bool copyinit)
{
    setCheckedRef(copyinit);
}

void QTrackerProcessor::onRequestFinished(QVector<QStringList> dataList, ETrackerAbstarctor_Query_Ids queryId, uint reqUID)
{
    //MP_CRITICAL << "ZZZ onRequestFinished dataList.size() = " << dataList.size();
    //MP_CRITICAL << "ZZZ m_requestId = " << m_requestId;
    //MP_CRITICAL << "ZZZ reqUID = " << reqUID;
    switch (queryId)
    {
    case eGetVideosByFolderQuery_Id:
        if((int)reqUID == eVideoFilesInFolderRequest + frontID + m_requestId) // modified by Dmitry 17.08.13
        {
            emit folderContentReceived(dataList, m_requestId);
        }
        else
        {
            MP_LOG << "VideoModel::onRequestFinished unknown reqUID" << reqUID << LOG_ENDL;
        }
        break;
    case eGetVideoFileListThumbNail_Id:
        handleThumbnailerResponse(dataList, reqUID);
        break;
    default:
        // MP_LOG << "VideoModel::onRequestFinished unknown request type" << queryId << LOG_ENDL;
        break;
    }
}

void QTrackerProcessor::handleThumbnailerResponse(QVector<QStringList> &dataList, int index)
{
    //MP_CRITICAL << "ZZZ START dataList.size() = " << dataList.size();

    if (dataList.size() > 0)
    {
       QStringList thumbInfo = dataList.at(0);
       if (thumbInfo.size() > 1)
       {
          QString thumbnailPath = FileManagerEngine::GetValidFilePath(thumbInfo.at(1));
          emit thumbnailReceived(index, thumbnailPath == "No Thumbnail Data" ? "" : thumbnailPath);
       }
    }

    //MP_CRITICAL << "END"; //commented by honggi.shin, 2014.02.27, for removing heavy log
}
// modified by Dmitry 26.07.13
void QTrackerProcessor::onRequestThumbnails(QStringList path, int reqId)
{
   //MP_CRITICAL << "ZZZ onRequestThumbnails reqId = " << reqId;
   this->abstractor->RequestData(path, eGetVideoFileListThumbNail_Id, reqId);
}

void QTrackerProcessor::onRequestFolderContent(QString path, int uid, int reqId)
{
    //MP_CRITICAL << "ZZZ onRequestFolderContent";

// removed by Dmitry 23.10.13
    m_requestId = reqId;
    this->abstractor->RequestData(path, eGetVideosByFolderQuery_Id, uid + frontID + reqId);
}

QString QTrackerProcessor::GetSWVersion() // added by sungha.choi 2013.08.25 for blank list after booting
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
