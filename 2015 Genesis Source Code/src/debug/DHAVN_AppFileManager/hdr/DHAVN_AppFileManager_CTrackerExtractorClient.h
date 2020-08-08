#ifndef DHAVN_APPFILEMANAGER_CTRACKEREXTRACTORCLIENT_H
#define DHAVN_APPFILEMANAGER_CTRACKEREXTRACTORCLIENT_H

#include <QObject>
#include <QStringList>
#include <QVector>
#include <QTimer>
#include <QtDBus/QDBusConnection>
#include <QtDBus/QDBusConnectionInterface> // added by Dmitry 23.10.13
#include "DHAVN_AppFileManager_Logger.h"


class CTrackerExtractorClient : public QObject
{
    Q_OBJECT
    // USE_FM_LOG_UTIL
public:
    explicit CTrackerExtractorClient(QObject *parent = 0);
    QDBusConnection getConnection() { return connection; } // added by Dmitry 23.10.13
// removed by Dmitry 21.08.13

signals:
    void showNotifyPopup();
    void hideNotifyPopup();
    void updateUIData( const QVector<QStringList>  *list );
    void MinerStatusFinished();

private slots:
    void onMinerStatus(QString minerType, QString minerStatus);
    void onChangeNotify(int ChangeStatus, bool ChangedType,
                        QVector<QStringList> UpdtaedUrlList);
    void onMiningStatusInDetail(QString minerType, QString minerStatus, int percentage);
    void onTimeout();

private:
    QDBusConnection connection;
    bool trackingFinished;
    QTimer popupTimer;
// { added by Puneet 2012.12.28 for CR 14749
private slots:
    void onThumbnailsGenerated();
signals:
    void ThumbNailGenerated();
//}
};

#endif // DHAVN_APPFILEMANAGER_CTRACKEREXTRACTORCLIENT_H
