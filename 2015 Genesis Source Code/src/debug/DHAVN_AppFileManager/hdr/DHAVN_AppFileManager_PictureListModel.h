#ifndef DHAVN_APPFILEMANAGER_PICTURELISTMODEL_H
#define DHAVN_APPFILEMANAGER_PICTURELISTMODEL_H

#include <QAbstractListModel>
#include <QList>

#include "DHAVN_TrackerAbstractor.h"
#include "DHAVN_AppFileManager_CStateManager.h"
#include "DHAVN_AppFileManager_PictureModelItem.h"
#include "DHAVN_AppFileManager_BaseListModel.h"
#include "DHAVN_AppFileManager_HashList.h"
#include "DHAVN_AppFileManager_Thumbnailer.h"//Added by Alexey Edelev 2012.10.25 Fix thumbnails issue

// modified by Dmitry 23.08.13
class PictureListModel : public BaseListModel
{
    Q_OBJECT

public:
    enum PictureModelRoles {
        PictureItemsCount = BaseListModel::BaseUserRole,
        PictureThumbPicture1,
        PictureIsFolder,
        ThumbnailPath//Added by Alexey Edelev 2012.10.25 Fix thumbnails issue
    };

    Q_PROPERTY(bool layoutReady READ getLayoutReady WRITE setLayoutReady NOTIFY layoutReadyChanged); // added by Dmitry 15.10.13 for ITS0195216

    explicit PictureListModel(CStateManager *pStateManager, bool bFront, QObject *parent = 0); // modified by Dmitry 08.08.13 for ITS0183013

    void addDataItem(const PictureModelItem &data);
    void append(const QList<PictureModelItem> &list);
    virtual int rowCount(const QModelIndex & parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role);
    void clear();
    bool updateElement(int index, const PictureModelItem &data);
    PictureModelItem* getPictureItem(int index) const;//Changed by Alexey Edelev 2012.10.18 CR14766
    void updateElementNotifyer(int index);
    virtual int getListCount() const;//Changed by Alexey Edelev 2012.10.18 CR14766
    void removeDataItem(int index);

    void cancel();
    EMediaDataRequestType getRequestType();
    void exec(EMediaDataRequestType requestType, const QString& rootFolder);

    int totalFiles() { return m_totalFiles; } // added by Dmitry 27.07.13
    int totalFolders() { return m_totalFolders; } // added by Dmitry 27.07.13

    void setCurrentReference(bool copyinit); // modified by ravikanth for ITS 0184642

// added by Dmitry 15.10.13 for ITS0195216
    bool getLayoutReady() { return m_bLayoutReady; }
    void setLayoutReady(bool layoutReady) { m_bLayoutReady = layoutReady; emit layoutReadyChanged(); }

signals:
    void getFirstImageAndCount(QVector<QStringList> dataList);   // added by sungha.choi 2013.08.20 for ITS 0183127
    void layoutReadyChanged(); // added by Dmitry 15.10.13 for ITS0195216

private slots:
    void onRequestFinished(QVector<QStringList> dataList,
                           ETrackerAbstarctor_Query_Ids queryId,
                           uint reqUID); // modified by kihyung 2013.2.17
// removed by Dmitry 22.08.13
    void onError(int error, const QString &errorString);
    void onThumbnailReady(const QString& thumbnail, bool unsupported, int index, bool bFront);  // modified by sungha.choi 2013.09.05 for ITS 0188265

protected:
    virtual const BaseModelItem* getListItem(int index) const;
    virtual BaseModelItem* getListItem(int index);

private:
    QString GetSWVersion();  // added by sungha.choi 2013.08.25 for blank list after booting

private:
    HashList<PictureModelItem, QString> m_ListItems;
    EMediaDataRequestType requestType;
    CStateManager* m_pStateManager; // added by Dmitry 08.08.13 for ITS0183013
    QScopedPointer<CTrackerAbstractor> abstractor;
    QString baseFolder;
    int m_totalFiles; // added by Dmitry 27.07.13
    int m_totalFolders; // added by Dmitry 27.07.13
    bool m_bFront; // added by Dmitry 07.08.13 for ITS0183065
    int folderRequestIdx; // added by Dmitry 22.08.13
    QString m_strSWVersion;  // added by sungha.choi 2013.08.25 for blank list after booting
    bool m_bLayoutReady; // added by Dmitry 15.10.13 for ITS0195216
};

#endif // DHAVN_APPFILEMANAGER_PICTURELISTMODEL_H
