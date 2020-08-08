#ifndef DHAVN_APPFILEMANAGER_VIDEOLISTMODEL_H
#define DHAVN_APPFILEMANAGER_VIDEOLISTMODEL_H

#include <QAbstractListModel>
#include <QList>
#include <QThread>
// added by Dmitry 23.10.13
#include <QDBusInterface>
#include <QDBusMessage>

#include "DHAVN_AppFileManager_Engine.h"
#include "DHAVN_AppFileManager_CStateManager.h"
#include "DHAVN_AppFileManager_VideoModelItem.h"
#include "DHAVN_AppFileManager_BaseListModel.h"
#include "DHAVN_AppFileManager_HashList.h"
#include "DHAVN_TrackerAbstractor.h"

class QTrackerProcessor;

class VideoListModel : public BaseListModel
{
    Q_OBJECT
public:
    enum VideModelRoles {
        VideoItemsCount = BaseListModel::BaseUserRole,
        VideoThumbPicture1,
        VideoThumbPicture2,
        VideoIsFolder
    };

    Q_PROPERTY(bool layoutReady READ getLayoutReady WRITE setLayoutReady NOTIFY layoutReadyChanged); // added by Dmitry 15.10.13 for ITS0195216
    explicit VideoListModel(CStateManager *pStateManager, bool bFront, QObject *parent = 0);

    VideoModelItem* getVideoItem(int index) const;//Changed by Alexey Edelev 2012.10.18 CR14766
    void addDataItem(const VideoModelItem &data);
    void append(const QList<VideoModelItem> &list);
    virtual int rowCount(const QModelIndex & parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role);
    Q_INVOKABLE void clear();
    bool updateElement(int index, const VideoModelItem &data);
    void updateElementNotifyer(int index);
    virtual int getListCount() const;//Changed by Alexey Edelev 2012.10.18 CR14766
    void removeDataItem(int index);

    void exec(EMediaDataRequestType requestType,
              const QString& rootFolder);
    void cancel();
    EMediaDataRequestType getRequestType();
    Q_INVOKABLE void updateThumbnails(int start, int end); // added by Dmitry 26.07.13

    int totalFiles() { return m_totalFiles; } // added by Dmitry 27.07.13
    int totalFolders() { return m_totalFolders; } // added by Dmitry 27.07.13
    void setCurrentReference(bool copyinit); // modified by ravikanth for ITS 0184642
// added by Dmitry 15.10.13 for ITS0195216
    bool getLayoutReady() { return m_bLayoutReady; }
    void setLayoutReady(bool layoutReady) { m_bLayoutReady = layoutReady; emit layoutReadyChanged(); }

protected:
    virtual const BaseModelItem* getListItem(int index) const;
    virtual BaseModelItem* getListItem(int index);

public slots:
    void onError(int error, const QString &errorString);
    void onThumbnailReceived(int index, QString path);
    void onFolderContentReceived(QVector<QStringList> dataList, int reqId);

signals:
    void append(VideoListModel *, const QList<VideoModelItem> &list);
    void requestThumbnails(QStringList path, int reqId);
    void requestFolderContent(QString path, int uid, int reqId);
    void layoutReadyChanged(); // added by Dmitry 15.10.13 for ITS0195216

private:
    HashList<VideoModelItem, QString> m_ListItems;
    EMediaDataRequestType requestType;
    QString baseFolder;
    // removed by Dmitry 26.07.13
    QScopedPointer<CStateManager> m_pStateManager ;
    int m_Index;
    QTrackerProcessor* m_pTrackerProcessor;
    QThread m_trackerThread;
    int m_requestId;
    int m_totalFiles; // added by Dmitry 27.07.13
    int m_totalFolders; // added by Dmitry 27.07.13
    bool m_bLayoutReady; // added by Dmitry 15.10.13 for ITS0195216
};

class QTrackerProcessor : public QObject
{
   Q_OBJECT

public:
   QTrackerProcessor(int frontID, QObject* parent = NULL):
      QObject(parent),
      abstractor(new CTrackerAbstractor),
      frontID(frontID),
      // modified by Dmitry 23.10.13
      m_requestId(0)
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
   }
   CTrackerAbstractor* getAbstractor() { return this->abstractor.data(); }   // added by sungha.choi 2013.08.06 for ITS0182950 AFM crash issue


private:
   void handleThumbnailerResponse(QVector<QStringList>& dataList, int index); // modified by Dmitry 26.07.13
   QString GetSWVersion();  // added by sungha.choi 2013.08.25 for blank list after booting
// removed by Dmitry 23.10.13


signals:
   void thumbnailReceived(int, QString);
   void folderContentReceived(QVector<QStringList> dataList, int req);

public slots:
   void onRequestFinished(QVector<QStringList> dataList,
                          ETrackerAbstarctor_Query_Ids queryId,
                          uint reqUID);

   void onRequestThumbnails(QStringList path, int reqId);
   void onRequestFolderContent(QString path, int uid, int reqId);

private:
   QScopedPointer<CTrackerAbstractor> abstractor;
   int frontID;
   int m_requestId;
   QString m_strSWVersion;  // added by sungha.choi 2013.08.25 for blank list after booting
};

#endif // DHAVN_APPFILEMANAGER_VIDEOLISTMODEL_H
